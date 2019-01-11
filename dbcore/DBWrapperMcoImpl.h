#ifndef DB_WRAPPER_MCO_IMPL_H
#define DB_WRAPPER_MCO_IMPL_H

#include "DBWrapper.h"

#include <common/threadsafequeue.h>

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
#include <functional>


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



typedef std::function<void(mco_db_h)> DBTask;


class  DBWrapperMcoImpl
{
public:
	DBWrapperMcoImpl();

	~DBWrapperMcoImpl();

	void submit(PlainHeaders& headers, FTD::PackageSPtr pPackage);
private:
	void processTaskPack(PlainHeaders& headers, FTD::PackageSPtr pPackage, mco_db_h db);
	//void populate(const DBTask& pf);
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

	DISABLE_COPY_AND_ASSIGN(DBWrapperMcoImpl)
};

#endif