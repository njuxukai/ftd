#include "ftdc_all.h"
using namespace genericdb;

void processQryPrivateIntialDataTransaction(const ReqQryPrivateInitialData* pReq,
	mco_trans_h t, RspQryPrivateInitialData* pRsp) {}

void processQryPrivateInitialData(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processQryPrivateInitialData] called\n";
#endif
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue, sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspQryPrivateInitialData> pRsp = std::make_shared<RspQryPrivateInitialData>();
	ReqQryPrivateInitialData *pReqQryPrivateInitialData = (ReqQryPrivateInitialData*)pReq.get();
	pRsp->m_sequenceSeries = pReq->m_sequenceSeries;
	pRsp->m_sequenceNO = pReq->m_sequenceNO;

	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;

	rc = mco_trans_start(db, MCO_READ_ONLY, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK != rc)
	{
		pRsp->pErrorField->ErrorCode = FTD_ERROR_CODE_TRANSACTION_ERROR;
		strcpy(pRsp->pErrorField->ErrorText, FTD_ERROR_TEXT_TRANSACTION_ERROR);
		pWrapper->uplink(rspHeaders, pRsp);
		return;
	}
	int searchInvestorID = pReqQryPrivateInitialData->qryInitialDataField.InvestorID;
	int lastReceivedID = pReqQryPrivateInitialData->dissenminationstartField.SequenceNo;

	mco_cursor_t csr;
	rc = TestER::InvIdx::cursor(t, &csr);
	if (MCO_S_OK == rc)
	{
		rc = TestER::InvIdx::search(t, &csr, MCO_GT, 99, 1);
		for (;
		rc == MCO_S_OK;
			rc = mco_cursor_next(t, &csr))
		{
			TestER report;
			report.from_cursor(t, &csr);
			if (report.investor_id != 99)
				break;


		}
	}

	pWrapper->uplink(rspHeaders, pRsp);
}

