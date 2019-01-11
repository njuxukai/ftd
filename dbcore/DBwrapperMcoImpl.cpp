#include "DBWrapperMcoImpl.h"


DBWrapperMcoImpl::DBWrapperMcoImpl(): m_done(false), m_joiner(new JoinThreads(m_threads))
{
	initDB();
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
	mco_runtime_stop();
	free(dev[0].dev.conv.ptr);
	free(dev[1].dev.conv.ptr);
	sample_os_shutdown();
}
 
void DBWrapperMcoImpl::initDB()
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
	while (!m_done || !m_reqQueue.empty())
	{
		auto pTask = m_reqQueue.wait_and_pop(20);
		if (pTask.get())
		{
			processTaskPack(*pTask.get(), db);
		}
	}
	mco_db_disconnect(db);
}

void DBWrapperMcoImpl::processTaskPack(DBTaskPack& pack, mco_db_h db)
{

}