#include "ftdc_all.h"

void processQryPrivateIntialDataTransaction(const ReqQryPrivateInitialData* pReq, 
	mco_trans_h t, RspQryPrivateInitialData* pRsp)
{}

Package* processQryPrivateInitialData(const Package* pReq, mco_db_h db)
{
	RspQryPrivateInitialData* pRsp = new RspQryPrivateInitialData();
	return pRsp;
}

