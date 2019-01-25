#include "ftdc_all.h"
#include "Exceptions.h"

using namespace genericdb;

void processQryTradeTransaction(ReqQryTrade* pReq, mco_trans_h t, RspQryTrade* pRsp);

void processQryTrade(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processQryTrade] called\n";
#endif
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue, sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspQryTrade> pRsp = std::make_shared<RspQryTrade>();
	pRsp->m_sequenceSeries = pReq->m_sequenceSeries;
	pRsp->m_sequenceNO = pReq->m_sequenceNO;

	ReqQryTrade *pReqQryTrade = (ReqQryTrade*)pReq.get();
	pRsp->requestSourceField.RequestID = pReqQryTrade->qryTradeField.RequestID;

	try
	{
		McoTrans t(db, MCO_READ_ONLY, MCO_TRANS_FOREGROUND);
		try
		{
			processQryTradeTransaction(pReqQryTrade, t, pRsp.get());
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


void processQryTradeTransaction(ReqQryTrade* pReq, mco_trans_h t, RspQryTrade* pRsp)
{
	int searchInvestorID = pReq->qryTradeField.InvestorID;

	mco_cursor_t csr;
	MCO_RET rc = InnerExecutionReport::InvestorIdx::cursor(t, &csr);
	if (MCO_S_OK == rc)
	{
		for (rc = InnerExecutionReport::InvestorIdx::search(t, &csr, MCO_EQ, searchInvestorID);
		rc == MCO_S_OK;
			rc = mco_cursor_next(t, &csr))
		{
			InnerExecutionReport report;
			report.from_cursor(t, &csr);
			if (report.investor_id != searchInvestorID)
				break;

			CFtdcTradeField tradeField = { 0 };
			tradeField.InvestorID = report.investor_id;
			strcpy(tradeField.InstrumentCode, ((std::string)report.instrument_code).data());
			tradeField.ExchangeType = report.exchange_type;
			tradeField.PriceType = report.price_type;
			tradeField.LimitPrice = report.price;
			tradeField.Direction = report.bs_flag;
			tradeField.OrderSysID = report.order_sys_id;
			tradeField.TradeSysID = report.ier_sys_id;
			strcpy(tradeField.TradeExchangeID, ((std::string)report.report_exchange_id).data());
			tradeField.VolumeTrade = report.volume_last;
			tradeField.PriceTrade = report.price_last;
			pRsp->tradeFields.push_back(tradeField);

		}
	}
}