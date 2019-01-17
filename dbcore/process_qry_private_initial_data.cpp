#include "ftdc_all.h"

void processQryPrivateIntialDataTransaction(const ReqQryPrivateInitialData* pReq,
	mco_trans_h t, RspQryPrivateInitialData* pRsp) {}

void processQryPrivateInitialData(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue, sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspQryPrivateInitialData> pRsp = std::make_shared<RspQryPrivateInitialData>();
	ReqQryPrivateInitialData *pReqQryPrivateInitialData = (ReqQryPrivateInitialData*)pReq.get();
	pWrapper->uplink(rspHeaders, pRsp);
}

