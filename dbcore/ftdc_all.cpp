#include "ftdc_all.h"
using namespace genericdb;

Package* ftdcAll(const Package* pReq, mco_db_h db)
{
	Package* pRsp = 0;
	switch (pReq->m_transactionId)
	{
	case TID_UserLogin:
		pRsp = processUserLogin(pReq, db);
		break;
	case TID_UserLogout:
		pRsp = 0;
		break;
	case TID_OrderInsert:
		pRsp = processOrderInsert(pReq, db);
		break;
	case TID_OrderAction:
		pRsp = processOrderAction(pReq, db);
		break;
	case TID_FundTransfer:
		pRsp = processFundTransfer(pReq, db);
		break;
	case TID_QryOrder:
		pRsp = processQryOrder(pReq, db);
		break;
	case TID_QryTrade:
		pRsp = processQryTrade(pReq, db);
		break;
	default:
		pRsp = 0;
		break;
	}
	return pRsp;
}

void populate_db(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int i, j;
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
			//printf("[%d]To Insert[%d]\n", j, user.user_id);
			rc = mco_trans_commit(t);
			if (MCO_S_OK == rc)
			{
				printf("[%d]Insert Success[%d]\n", j, (uint4)user.user_id);
			}
			else
			{
				printf("[%d]Insert failure[%d]\n", j, (uint4)user.user_id);
			}
		}
	}
}