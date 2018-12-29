// TestDB.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TestDB.h"
#include "DBWrapper.h"

#include <iostream>
#include <functional>

#include <common.h>
#include "genericdb.hpp"


#include "ftdc_all.h"



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

void processTask(mco_db_h db, PackageSPtr pPackage)
{
	PackageSPtr pRsp = PackageSPtr(ftdcAll(pPackage, db));
	printRspPackage(pRsp.get());
}


//test user login
void testUserLogin(McoDBWrapper* wrapper)
{
	using namespace std::placeholders;
	ReqUserLogin* reqLogin = new ReqUserLogin();
	wrapper->submit(std::bind(processTask, _1, PackageSPtr(reqLogin)));
}

void printRspUserLogin(RspUserLogin* pRsp)
{
	std::cout << "TID_UserLogin ";
	if (pRsp->pErrorField && pRsp->pErrorField->ErrorCode != 0)
	{
		printf("Failure.[ErrorCode=%d,ErrorText=%s]\n", pRsp->pErrorField->ErrorCode,
			pRsp->pErrorField->ErrorText);
	}
	else
	{
		printf("Succeed.");
	}
}

void printRspPackage(Package* pRsp)
{
	if (pRsp->m_transactionId == TID_UserLogin && pRsp->isResponse())
	{
		printRspUserLogin((RspUserLogin*)pRsp);
	}
}



