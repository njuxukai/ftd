#include "ftdc_all.h"


Package* ftdcAll(const Package* pReq, mco_db_h db)
{
	switch (pReq->m_transactionId)
	{
	case TID_UserLogin:
		return processUserLogin(pReq, db);
	case TID_UserLogout:
		return 0;
	case TID_OrderInsert:
		return processOrderInsert(pReq, db);
	case TID_OrderAction:
		return processOrderAction(pReq, db);
	case TID_FundTransfer:
		return processFundTransfer(pReq, db);
	case TID_QryOrder:
		return processQryOrder(pReq, db);
	case TID_QryTrade:
		return processQryTrade(pReq, db);
	default:
		return 0;
	}
}