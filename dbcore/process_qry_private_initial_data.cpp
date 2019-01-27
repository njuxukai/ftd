#include "ftdc_all.h"
#include "Exceptions.h"

using namespace genericdb;

void processQryPrivateIntialDataTransaction(const ReqQryPrivateInitialData* pReq,
	mco_trans_h t, RspQryPrivateInitialData* pRsp);

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

	pRsp->requestSourceField.RequestID = 0;

	try
	{
		McoTrans t(db, MCO_READ_ONLY, MCO_TRANS_FOREGROUND);
		try
		{
			processQryPrivateIntialDataTransaction(pReqQryPrivateInitialData, t, pRsp.get());
		}
		catch (...)
		{
			t.rollback();
			throw;
		}
	}
	catch (dbcore::Exception& e)
	{
		pRsp->pErrorField->ErrorCode = e.errorCode;
		strcpy(pRsp->pErrorField->ErrorText, e.what());
	}
	catch (McoException& e)
	{
		pRsp->pErrorField->ErrorCode = e.get_rc();
		strncpy(pRsp->pErrorField->ErrorText, e.what(), sizeof(CFtdcErrorField::ErrorText));
	}
	pWrapper->uplink(rspHeaders, pRsp);
}


void processQryPrivateIntialDataTransaction(const ReqQryPrivateInitialData* pReq,
	mco_trans_h t, RspQryPrivateInitialData* pRsp) 
{
	int searchInvestorID = pReq->dissenminationstartField.SequenceSeries;
	int searchSNo = pReq->dissenminationstartField.SequenceNo;
	mco_cursor_t csr;
	MCO_RET rc = UserExecutionReport::InvestorIdx::cursor(t, &csr);
	if (MCO_S_OK == rc)
	{
		for (rc = UserExecutionReport::InvestorIdx::search(t, &csr, MCO_GT,
			searchInvestorID, searchSNo);
			rc == MCO_S_OK;
			rc = mco_cursor_next(t, &csr))
		{
			UserExecutionReport report;
			report.from_cursor(t, &csr);
			if (report.investor_id != searchInvestorID)
				break;
			CFtdcExecutionReportField reportField = { 0 };
			reportField.FrontID = report.front_id;
			reportField.SessionID = report.session_id;
			reportField.OrderRef = report.order_ref;
			reportField.OrderSysID = report.order_sys_id;
			strcpy(reportField.InstrumentCode, ((std::string)report.instrument_code).data());
			reportField.ExchangeType = report.exchange_type;
			reportField.Direction = report.bs_flag;
			reportField.InvestorID = report.investor_id;
			reportField.SequenceNo = report.uer_sys_id;
			reportField.SequenceSeries = report.investor_id;
			reportField.PriceType = report.price_type;
			reportField.LimitPrice = report.price;
			reportField.VolumeTotalOrginal = report.volume;
			reportField.OrderStatus = report.status;
			reportField.VolumeCum = report.volume_cum;
			reportField.AmountCum = report.amount_cum;
			reportField.ReportSysID = report.uer_sys_id;

			pRsp->executionReportFields.push_back(reportField);
		}
	}
}
