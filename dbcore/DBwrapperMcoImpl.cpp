#include "DBWrapperMcoImpl.h"
#include "functional"
#include "ftdc_all.h"

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

const int nThreadCount = 8;


DBWrapperMcoImpl::DBWrapperMcoImpl(): m_done(false), m_joiner(new JoinThreads(m_threads))
{
	initDB2();
	
	std::cout << "[DEBUG]Populate DB started.\n";
	populateDB();
	std::cout << "[DEBUG]Populate DB finished.\n";


	initThreads();
}

DBWrapperMcoImpl::~DBWrapperMcoImpl()
{
	m_done = true;
	if (m_joiner)
	{
		delete m_joiner;
		m_joiner = 0;

	}
	sample_close_database(db_name, &m_dbmem);
	mco_runtime_stop();
	sample_os_shutdown();
}

void DBWrapperMcoImpl::initDB2()
{
	MCO_RET         rc;
	

	sample_os_initialize(DEFAULT);

	rc = mco_runtime_start();
	sample_rc_check("\tmco_runtime_start", rc);
	sample_header("ExtremeDB opens.\n");
	rc = sample_open_database(db_name, genericdb_get_dictionary(), DATABASE_SIZE, CACHE_SIZE,
		MEMORY_PAGE_SIZE, PSTORAGE_PAGE_SIZE, 10, &m_dbmem);
	sample_rc_check("\tOpen database\n", rc);
}



void DBWrapperMcoImpl::populateDB()
{
	mco_db_h db = 0;
	auto rc = mco_db_connect(db_name, &db);
	if (MCO_S_OK != rc)
	{
		printf("mco_db_connect failure[%d]\n", (int)rc);
		return;
	}
	mco_disk_transaction_policy(db, MCO_COMMIT_SYNC_FLUSH);
	populate2_db(db);
}


void DBWrapperMcoImpl::initThreads()
{
	try 
	{
		for (int i = 0; i < nThreadCount; i++)
		{
			m_threads.push_back(
				std::thread(&DBWrapperMcoImpl::worker, this));
		}
	}
	catch (...)
	{
		m_done = true;
	}
	
}

void DBWrapperMcoImpl::worker()
{
	mco_db_h db = 0;
	auto rc = mco_db_connect(db_name, &db);
	if (MCO_S_OK != rc)
	{
		printf("mco_db_connect failure[%d]\n", (int)rc);
		return;
	}
	mco_disk_transaction_policy(db, MCO_COMMIT_BUFFERED);
	while (!m_done || !m_packageQueue.empty())
	{
		auto pTask = m_packageQueue.wait_and_pop(20);
		if (pTask.get())
		{
			(*pTask)(db);
		}
	}
	mco_db_disconnect(db);
}

void DBWrapperMcoImpl::submit(const PlainHeaders& pHeaders, FTD::PackageSPtr pPackage)
{
	m_packageQueue.push(std::bind(&DBWrapperMcoImpl::processTaskPack, this, 
		pHeaders, pPackage, std::placeholders::_1));
}

void DBWrapperMcoImpl::processTaskPack(DBWrapperMcoImpl* pWrapper, 
	const PlainHeaders& headers, FTD::PackageSPtr pPackage, mco_db_h db)
{
	ftdcAll(headers, pPackage, pWrapper, db);
}

void DBWrapperMcoImpl::registerUplinkCallback(const DBUplinkCallback& function)
{
	m_uplinkFunction = function;
}

void DBWrapperMcoImpl::uplink(PlainHeaders& headers, FTD::PackageSPtr pPackage)
{
	if (m_uplinkFunction)
	{
		m_uplinkFunction(headers, pPackage);
	}
}