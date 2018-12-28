#include "ftdc_all.h"


PackageSPtr ftdcAll(PackageSPtr pReq, mco_db_h db)
{
	Package* pRsp = 0;
	switch (pReq->m_transactionId)
	{
	case TID_UserLogin:
		pRsp = processUserLogin(pReq.get(), db);
		break;
	case TID_UserLogout:
		pRsp = 0;
		break;
	case TID_OrderInsert:
		pRsp = processOrderInsert(pReq.get(), db);
		break;
	case TID_OrderAction:
		pRsp = processOrderAction(pReq.get(), db);
		break;
	case TID_FundTransfer:
		pRsp = processFundTransfer(pReq.get(), db);
		break;
	case TID_QryOrder:
		pRsp = processQryOrder(pReq.get(), db);
		break;
	case TID_QryTrade:
		pRsp = processQryTrade(pReq.get(), db);
		break;
	default:
		pRsp = 0;
		break;
	}
	return PackageSPtr(pRsp);
}