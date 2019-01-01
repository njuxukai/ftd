#include "ftdc_all.h"


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