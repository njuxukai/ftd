#include "ftdc_all.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;

void processQryOrderTransaction(ReqQryOrder* pReq, mco_trans_h t, RspQryOrder* pRsp);

void processQryOrder(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processQryOrder] called\n";
#endif
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue,
		sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspQryOrder> pRsp = std::make_shared<RspQryOrder>();
	pRsp->m_sequenceSeries = pReq->m_sequenceSeries;
	pRsp->m_sequenceNO = pReq->m_sequenceNO;

	ReqQryOrder *pReqQryOrder = (ReqQryOrder*)pReq.get();
	pRsp->requestSourceField.RequestID = pReqQryOrder->qryOrderField.RequestID;

	try
	{
		McoTrans t(db, MCO_READ_ONLY, MCO_TRANS_FOREGROUND);
		try
		{
			processQryOrderTransaction(pReqQryOrder, t, pRsp.get());
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

void processQryOrderTransaction(ReqQryOrder* pReq, mco_trans_h t, RspQryOrder* pRsp)
{
	
	int searchInvestorID = pReq->qryOrderField.InvestorID;
	mco_cursor_t csr;
	MCO_RET rc = Order::InvestorIdx::cursor(t, &csr);
	if (MCO_S_OK == rc)
	{
		for (rc = Order::InvestorIdx::search(t, &csr, MCO_EQ, searchInvestorID);
		rc == MCO_S_OK;
			rc = mco_cursor_next(t, &csr))
		{
			Order order;
			order.from_cursor(t, &csr);
			if (order.investor_id != searchInvestorID)
				break;
			CFtdcOrderField orderField = { 0 };
			orderField.InvestorID = order.investor_id;
			orderField.FrontID = order.front_id;
			orderField.SessionID = order.session_id;
			orderField.OrderRef = order.order_ref;
			orderField.OrderSysID = order.order_sys_id;
			strcpy(orderField.InstrumentCode, ((std::string)order.instrument_code).data());
			strcpy(orderField.OrderExchangeID, ((std::string) order.order_exchange_id).data());
			strcpy(orderField.ClOrdID, ((std::string)order.client_order_id).data());
			orderField.Direction = order.direction;
			orderField.PriceType = order.price_type;
			orderField.LimitPrice = order.price;
			orderField.OrderStatus = order.status;
			orderField.VolumeTotalOrginal = order.volume_total_original;
			orderField.VolumeLeaves = order.volume_leaves;
			orderField.VolumeCum = order.volume_cum;
			orderField.AmountCum = order.amount_cum;

			pRsp->orderFields.push_back(orderField);
		}
	}
	
}