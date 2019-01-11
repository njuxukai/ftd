#include "ftdc_all.h"
using namespace genericdb;

void ftdcAll(PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
	switch (pReq->m_transactionId)
	{
	case TID_UserLogin:
		processUserLogin(headers, pReq, pWrapper, db);
		break;
	case TID_UserLogout:
		//processUserLogout(headers, pReq, pWrapper, db);
		break;
	case TID_QryPrivateInitialData:
		processQryPrivateInitialData(headers, pReq, pWrapper, db);
		break;
	case TID_OrderInsert:
		processOrderInsert(headers, pReq, pWrapper, db);
		break;
	case TID_OrderAction:
		processOrderAction(headers, pReq, pWrapper, db);
		break;
	case TID_FundTransfer:
		processFundTransfer(headers, pReq, pWrapper, db);
		break;
	case TID_QryOrder:
		processQryOrder(headers, pReq, pWrapper, db);
		break;
	case TID_QryTrade:
		processQryTrade(headers, pReq, pWrapper, db);
		break;
	default:
		break;
	}
}

void populate_db(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int  j;
	for (j = 0; j < 100 && MCO_S_OK == rc; j++)
	{
		User user;
		rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
		if (MCO_S_OK == rc)
		{
			mco_disk_transaction_policy(db, MCO_COMMIT_BUFFERED);
			//MCO_COMMIT_NO_SYNC
			user.create(t);
			user.broker_id = 8080;
			user.user_id = j + 1;
			user.password = "test";
			printf("[%d]To Insert[%d]\n", j, (uint4)user.user_id);
			rc = mco_trans_commit(t);
			if (MCO_S_OK == rc)
			{
				//printf("[%d]Insert Success[%d]\n", j, (uint4)user.user_id);
			}
			else
			{
				//printf("[%d]Insert failure[%d]\n", j, (uint4)user.user_id);
			}
		}
	}
}