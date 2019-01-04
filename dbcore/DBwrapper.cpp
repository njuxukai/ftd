#include "DBWrapper.h"

#include <common.h>
#include "genericdb.hpp"

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono> 
#include <condition_variable>
#include <queue>
#include <atomic>



#define  MAX_DEVICES           10
#define  N_DEVICES             4

char sample_descr[] = {
	"Sample 'disk_file' opens a database using FILE memory devices.\n"
};
const char * db_name = "disk_file";

/* Define relatively small memory segment sizes to facilitate testing */


/* Define nRecords large enough to cause creation of sufficient data to necessitate
use of multifile segments */
const int nRecords = 1000000;
/* Define transaction block size to reduce time for inserts */
const int nInsertsPerTransaction = 10000;

const int nThreadCount = 2;


template<typename T>
class ThreadsafeQueue
{
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	ThreadsafeQueue()
	{}

	void push(T data)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::move(data));
		data_cond.notify_one();
	}

	void wait_and_pop(T& value, int milliSeconds)
	{
		std::unique_lock<std::mutex> lk(mut);
		bool wait_result = data_cond.wait_for(lk, std::chrono::milliseconds(milliSeconds),
			[this] {return !data_queue.empty(); });
		if (wait_result)
		{
			value = std::move(data_queue.front());
			data_queue.pop();
		}
	}

	std::shared_ptr<T> wait_and_pop(int milliSeconds)
	{
		std::unique_lock<std::mutex> lk(mut);
		bool wait_result = data_cond.wait_for(lk, std::chrono::milliseconds(milliSeconds),
			[this] {return !data_queue.empty(); });
		std::shared_ptr<T> res;
		if (wait_result)
		{
			res = std::make_shared<T>(std::move(data_queue.front()));
			data_queue.pop();
		}
		return res;
	}

	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(
			std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};


class JoinThreads
{
	std::vector<std::thread>& threads;
public:
	explicit JoinThreads(std::vector<std::thread>& threads_) :
		threads(threads_)
	{}

	~JoinThreads()
	{
		for (unsigned long i = 0; i<threads.size(); ++i)
		{
			if (threads[i].joinable())
				threads[i].join();
		}
	}
};

class  McoDBWrapperImpl
{
public:
	McoDBWrapperImpl();

	~McoDBWrapperImpl();

	void submit(const DBTask& f)
	{
		m_reqQueue.push(std::move(f));
	}

	void populate(const DBTask& pf);
private:
	std::atomic<bool> m_done;
	ThreadsafeQueue<DBTask> m_reqQueue;
	JoinThreads* m_joiner;
	std::vector<std::thread> m_threads;
	void initDB();
	void initThreads();
	
	
private:
	mco_device_t       dev[N_DEVICES];
	mco_db_params_t    db_params;
private:
	void worker();
	McoDBWrapperImpl(const McoDBWrapperImpl&) = delete;
	McoDBWrapperImpl& operator=(const McoDBWrapperImpl&) = delete;
};



McoDBWrapperImpl::McoDBWrapperImpl(): m_done(false), m_joiner(new JoinThreads(m_threads))
{
	initDB();
	initThreads();
}

McoDBWrapperImpl::~McoDBWrapperImpl()
{
	m_done = true;
	if (m_joiner)
	{
		delete m_joiner;
		m_joiner = 0;
	}
	mco_runtime_stop();
	free(dev[0].dev.conv.ptr);
	free(dev[1].dev.conv.ptr);
	sample_os_shutdown();
}
 
void McoDBWrapperImpl::initDB()
{
	MCO_RET            rc;

	sample_os_initialize(DEFAULT);

	sample_header(sample_descr);

	/* Set fatal error handler and start eXtremeDB runtime */
	mco_error_set_handler(&sample_errhandler);
	mco_runtime_start();

	/* Configure first memory device as a plain conventional memory region */
	dev[0].type = MCO_MEMORY_CONV;
	dev[0].assignment = MCO_MEMORY_ASSIGN_DATABASE;
	dev[0].size = DATABASE_SIZE;
	dev[0].dev.conv.ptr = (void*)malloc(DATABASE_SIZE);

	/* Configure conventional memory region for cache */
	dev[1].type = MCO_MEMORY_CONV;
	dev[1].assignment = MCO_MEMORY_ASSIGN_CACHE;
	dev[1].size = CACHE_SIZE;
	dev[1].dev.conv.ptr = (void*)malloc(CACHE_SIZE);

	/* Configure FILE memory device for main database storage */
	dev[2].type = MCO_MEMORY_FILE;
	dev[2].assignment = MCO_MEMORY_ASSIGN_PERSISTENT;
	sprintf(dev[2].dev.file.name, FILE_PREFIX "%s.dbs", db_name);
	dev[2].dev.file.flags = MCO_FILE_OPEN_DEFAULT;

	/* Configure FILE memory device for transaction log */
	dev[3].type = MCO_MEMORY_FILE;
	dev[3].assignment = MCO_MEMORY_ASSIGN_LOG;
	sprintf(dev[3].dev.file.name, FILE_PREFIX "%s.log", db_name);
	dev[3].dev.file.flags = MCO_FILE_OPEN_DEFAULT;

	/* Initialize and customize the database parameters */
	mco_db_params_init(&db_params);                  /* Initialize the params with default values */
	db_params.mem_page_size = MEMORY_PAGE_SIZE;    /* Set page size for in-memory part */
	db_params.disk_page_size = PSTORAGE_PAGE_SIZE;  /* Set page size for persistent storage */
	db_params.db_max_connections = 10;                   /* Set total number of connections to the database */
	db_params.db_log_type = UNDO_LOG;            /* Set log type */
#ifdef EXTREMEDB_LICENSE_KEY
	db_params.license_key = EXTREMEDB_LICENSE_KEY;
#endif

	/* Open a database on the configured devices with given params */
	rc = mco_db_open_dev(db_name, genericdb_get_dictionary(), dev, N_DEVICES, &db_params);
	if (MCO_S_OK == rc)
	{
		std::cout << "open db succ\n";
	}
}

void McoDBWrapperImpl::populate(const DBTask& task)
{
	mco_db_h db = 0;
	auto rc = mco_db_connect(db_name, &db);
	if (MCO_S_OK != rc)
	{
		printf("Populate mco_db_connect failure[%d]\n", (int)rc);
		return;
	}
	else
	{
		printf("Populate mco_db_connect succ.\n");
	}
		
	task(db);
	mco_db_disconnect(db);
}

void McoDBWrapperImpl::initThreads()
{
	try 
	{
		for (int i = 0; i < nThreadCount; i++)
		{
			m_threads.push_back(
				std::thread(&McoDBWrapperImpl::worker, this));
		}
	}
	catch (...)
	{
		m_done = true;
	}
	
}

void McoDBWrapperImpl::worker()
{
	mco_db_h db = 0;
	auto rc = mco_db_connect(db_name, &db);
	if (MCO_S_OK != rc)
	{
		printf("mco_db_connect failure[%d]\n", (int)rc);
		return;
	}
	mco_disk_transaction_policy(db, MCO_COMMIT_BUFFERED);
	while (!m_done || !m_reqQueue.empty())
	{
		auto pTask = m_reqQueue.wait_and_pop(20);
		if (pTask.get())
		{
			(*pTask)(db);
		}
	}
	mco_db_disconnect(db);
}

McoDBWrapper::McoDBWrapper(): m_pImpl(new McoDBWrapperImpl())
{}

McoDBWrapper::~McoDBWrapper()
{
	if (m_pImpl)
	{
		delete m_pImpl;
		m_pImpl = 0;
	}
}


void McoDBWrapper::submit(const DBTask& f)
{
	if (m_pImpl)
	{
		m_pImpl->submit(f);
	}
}

void McoDBWrapper::populate(const DBTask& f)
{
	if (m_pImpl)
	{
		m_pImpl->populate(f);
	}
}