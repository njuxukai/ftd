#ifndef FTDC_ALL_H_
#define FTDC_ALL_H_
#include <genericdb.hpp>
#include <ftd/FTD30/Packages.h>

using namespace FTD;

Package* processUserLogin(const Package* pReq, mco_db_h db);
Package* processOrderInsert(const Package* pReq, mco_db_h db);
Package* processOrderAction(const Package* pReq, mco_db_h db);
Package* processFundTransfer(const Package* pReq, mco_db_h db);
Package* processQryOrder(const Package* pReq, mco_db_h db);
Package* processQryTrade(const Package* pReq, mco_db_h db);


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
#endif