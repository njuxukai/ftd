// TestDB.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <common.h>
#include "genericdb.hpp"

#include <ftd/FTD30/Packages.h>
#include "ftdc_all.h"
#include "DBWrapper.h"
#include <iostream>

using namespace genericdb;
using namespace FTD;

MCO_RET populate_db(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int i, j;
	for (j = 0; j < 10 && MCO_S_OK == rc; j++)
	{
		User user;
		rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
		if (MCO_S_OK == rc)
		{
			user.create(t);
			user.broker_id = 8080;
			user.user_id = j + 1;
			user.password = "test";
			//printf("[%d]To Insert[%d]\n", j, user.user_id);
			rc = mco_trans_commit(t);
			if (MCO_S_OK == rc)
			{
				//printf("[%d]Insert Success[%d]\n", j, user.user_id);
			}
			else
			{
				printf("[%d]Insert failure[%d]\n", j, (uint4)user.user_id);
			}
		}
	}
	return rc;
}

MCO_RET show_db(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;

	User user;
	rc = mco_trans_start(db, MCO_READ_ONLY, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK == rc)
	{
		mco_cursor_t c;
		mco_cursor_h csr = &c;
		rc = User_idx_user_id_index_cursor(t, csr);
		if (MCO_S_OK == rc)
		{
			rc = mco_cursor_first(t, csr);
			for (int i = 0; rc == MCO_S_OK; i++)
			{
				rc = user.from_cursor(t, csr);
				if (rc == MCO_S_OK)
				{
					printf("[i=%d]UserID[%d],Password[%s]\n", i, uint4(user.user_id), ((std::string)user.password).c_str());
					rc = mco_cursor_next(t, csr);
				}
			}
		}

		rc = mco_trans_commit(t);
		return rc;
	}
	return rc;
}

//int main(int argc, char* argv[])
//{
//	MCO_RET            rc;
//	mco_db_h db = 0;
//	mco_device_t       dev[N_DEVICES];  /* Memory devices for: 0) database, 1) cache, 2) main database storage, 3) transaction log */
//	mco_db_params_t    db_params;
//
//	sample_os_initialize(DEFAULT);
//
//	sample_header(sample_descr);
//
//	/* Set fatal error handler and start eXtremeDB runtime */
//	mco_error_set_handler(&sample_errhandler);
//	mco_runtime_start();
//
//	/* Configure first memory device as a plain conventional memory region */
//	dev[0].type = MCO_MEMORY_CONV;
//	dev[0].assignment = MCO_MEMORY_ASSIGN_DATABASE;
//	dev[0].size = DATABASE_SIZE;
//	dev[0].dev.conv.ptr = (void*)malloc(DATABASE_SIZE);
//
//	/* Configure conventional memory region for cache */
//	dev[1].type = MCO_MEMORY_CONV;
//	dev[1].assignment = MCO_MEMORY_ASSIGN_CACHE;
//	dev[1].size = CACHE_SIZE;
//	dev[1].dev.conv.ptr = (void*)malloc(CACHE_SIZE);
//
//	/* Configure FILE memory device for main database storage */
//	dev[2].type = MCO_MEMORY_FILE;
//	dev[2].assignment = MCO_MEMORY_ASSIGN_PERSISTENT;
//	sprintf(dev[2].dev.file.name, FILE_PREFIX "%s.dbs", db_name);
//	dev[2].dev.file.flags = MCO_FILE_OPEN_DEFAULT;
//
//	/* Configure FILE memory device for transaction log */
//	dev[3].type = MCO_MEMORY_FILE;
//	dev[3].assignment = MCO_MEMORY_ASSIGN_LOG;
//	sprintf(dev[3].dev.file.name, FILE_PREFIX "%s.log", db_name);
//	dev[3].dev.file.flags = MCO_FILE_OPEN_DEFAULT;
//
//	/* Initialize and customize the database parameters */
//	mco_db_params_init(&db_params);                  /* Initialize the params with default values */
//	db_params.mem_page_size = MEMORY_PAGE_SIZE;    /* Set page size for in-memory part */
//	db_params.disk_page_size = PSTORAGE_PAGE_SIZE;  /* Set page size for persistent storage */
//	db_params.db_max_connections = 1;                   /* Set total number of connections to the database */
//	db_params.db_log_type = UNDO_LOG;            /* Set log type */
//#ifdef EXTREMEDB_LICENSE_KEY
//	db_params.license_key = EXTREMEDB_LICENSE_KEY;
//#endif
//
//	/* Open a database on the configured devices with given params */
//	rc = mco_db_open_dev(db_name, genericdb_get_dictionary(), dev, N_DEVICES, &db_params);
//	if (MCO_S_OK == rc) {
//
//		/* Connect to the database, obtain a database handle */
//		rc = mco_db_connect(db_name, &db); /* No recovery connection data */
//		if (MCO_S_OK == rc) {
//
//			/* Show characteristics of opened database */
//			//sample_show_device_info("\n\tThe opened database has the following memory devices:", dev, N_DEVICES);
//			rc = populate_db(db);
//			rc = show_db(db);
//			auto pReqLogin = new ReqUserLogin();
//			std::shared_ptr<Package> pReq = std::shared_ptr<Package>(pReqLogin);
//			pReqLogin->reqUserLoginField.UserID = 8;
//			strcpy(pReqLogin->reqUserLoginField.Password, "test");
//			std::shared_ptr<Package> pRsp = ftdcAll(pReq, db);
//			if (MCO_S_OK != rc)
//			{
//				printf("[%d]error populate\n", rc);
//			}
//			RspUserLogin* result = (RspUserLogin*)pRsp.get();
//			printf("ProcessResult:%s[%d]", result->pErrorField->ErrorText, result->pErrorField->ErrorCode);
//			mco_db_disconnect(db);
//		}
//
//		/* Close the database */
//		mco_db_close(db_name);
//	}
//
//	/* Stop eXtremeDB runtime */
//	mco_runtime_stop();
//
//	/* Free allocated memory */
//	free(dev[0].dev.conv.ptr);
//	free(dev[1].dev.conv.ptr);
//
//	sample_pause_end("\n\nPress any key to continue . . . ");
//
//	sample_os_shutdown();
//
//	return (MCO_S_OK == rc ? 0 : 1);
//}
//
void printPackageSPtr(PackageSPtr pRsp)
{
	if (pRsp->m_transactionId == TID_UserLogin)
	{
		std::cout << "TID_UserLogin\n";
	}
}

int main(int argc, char* argv[])
{
	McoDBWrapper wrapper;
	ReqUserLogin* reqLogin = new ReqUserLogin();
	wrapper.registerResponseCallback(printPackageSPtr);
	wrapper.submit(PackageSPtr(reqLogin));
	while (char c = getchar())
	{
	}
}