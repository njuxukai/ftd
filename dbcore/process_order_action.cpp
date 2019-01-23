#include "ftdc_all.h"

using namespace genericdb;

void processOrderActionTransaction(const ReqUserLogin* pReq, mco_trans_h t, RspUserLogin* pRsp);

void processOrderAction(const PlainHeaders& headers, FTD::PackageSPtr pPackage, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processOrderAction] called\n";
#endif
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue,
		sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspQryPosition> pRsp =
		std::make_shared<RspQryPosition>();
	pRsp->m_sequenceSeries = pPackage->m_sequenceSeries;
	pRsp->m_sequenceNO = pPackage->m_sequenceNO;

	ReqOrderAction *pReq = (ReqOrderAction*)pPackage.get();
	pRsp->requestSourceField.RequestID =
		pReq->inputOrderActionField.RequestID;

	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;

	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK != rc)
	{
		pRsp->pErrorField->ErrorCode = FTD_ERROR_CODE_TRANSACTION_ERROR;
		strcpy(pRsp->pErrorField->ErrorText, FTD_ERROR_TEXT_TRANSACTION_ERROR);
		pWrapper->uplink(rspHeaders, pRsp);
		return;
	}
	//1
}