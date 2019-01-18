#include "ftdc_all.h"

void processQryOrder(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
	#ifdef _DEBUG
	std::cout << "[processQryOrder] called\n";
	#endif
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue, sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspQryOrder> pRsp = std::make_shared<RspQryOrder>();
	pRsp->m_sequenceSeries = pReq->m_sequenceSeries;
	pRsp->m_sequenceNO = pReq->m_sequenceNO;

	ReqQryOrder *pReqQryOrder = (ReqQryOrder*)pReq.get();
	pWrapper->uplink(rspHeaders, pRsp);
}