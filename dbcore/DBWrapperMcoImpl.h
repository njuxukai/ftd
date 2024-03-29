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





typedef std::function<void(mco_db_h)> DBTask;


class  DBWrapperMcoImpl : public DBWrapper
{
public:
	DBWrapperMcoImpl();

	~DBWrapperMcoImpl();

	void submit(const PlainHeaders& headers, FTD::PackageSPtr pPackage);
	virtual void registerUplinkCallback(const DBUplinkCallback& function);
	virtual void uplink(PlainHeaders& headers, FTD::PackageSPtr pPackage);
private:
	static void processTaskPack(DBWrapperMcoImpl* pWrapper, 
		const PlainHeaders& headers, FTD::PackageSPtr pPackage, mco_db_h db);
	//void populate(const DBTask& pf);
private:

	DBUplinkCallback m_uplinkFunction;
	std::atomic<bool> m_done;
	ThreadsafeQueue<DBTask> m_packageQueue;
	JoinThreads* m_joiner;
	std::vector<std::thread> m_threads;
	void initDB2();
	void initThreads();

	void populateDB();

private:
	sample_memory_t m_dbmem;

private:
	void worker();

	DISABLE_COPY_AND_ASSIGN(DBWrapperMcoImpl)
};

#endif