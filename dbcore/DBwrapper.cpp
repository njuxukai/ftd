#include "DBWrapper.h"
#include <common.h>
#include <iostream>


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

const int nThreadCount = 1;

using namespace genericdb;

McoDBWrapper::McoDBWrapper(): m_done(false),m_joiner(m_threads)
{
	InitDB();
	InitThreads();
}

McoDBWrapper::~McoDBWrapper()
{
	m_done = true;
	mco_runtime_stop();
	free(dev[0].dev.conv.ptr);
	free(dev[1].dev.conv.ptr);
	sample_os_shutdown();
}

void McoDBWrapper::InitDB()
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
	db_params.db_max_connections = 1;                   /* Set total number of connections to the database */
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

void McoDBWrapper::InitThreads()
{
	try 
	{
		for (int i = 0; i < nThreadCount; i++)
		{
			m_threads.push_back(
				std::thread(&McoDBWrapper::worker, this));
		}
	}
	catch (...)
	{
		m_done = true;
	}
	
}

void McoDBWrapper::worker()
{
	mco_db_h db = 0;
	auto rc = mco_db_connect(db_name, &db);
	if (MCO_S_OK != rc)
		return;
	while (!m_done || !m_reqQueue.empty())
	{
		/*
		auto ppReq = m_reqQueue.wait_and_pop(20);
		if (ppReq.get())
		{
		PackageSPtr pReq = std::move(*(ppReq.get()));
		PackageSPtr pRsp = ftdcAll(pReq, db);
		if(m_respCallback)
		m_respCallback(pRsp);
		}
		*/
		auto pTask = m_reqQueue.wait_and_pop(20);
		if (pTask.get())
		{
			(*pTask)(db);
		}
	}
	mco_db_disconnect(db);
}


/*
void McoDBWrapper::submit(const DBTask& task)
{
m_reqQueue.push(task);
}*/


/*
void McoDBWrapper::registerResponseCallback(Callback callback)
{
m_respCallback = callback;
}
*/
