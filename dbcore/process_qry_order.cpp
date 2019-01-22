#include "ftdc_all.h"

using namespace genericdb;

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

	int searchInvestorID = pReqQryOrder->qryOrderField.InvestorID;

	mco_cursor_t csr;
	rc = Order::InvestorIdx::cursor(t, &csr);
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
			orderField.Direction = order.direction;
			orderField.PriceType = order.price_type;
			orderField.LimitPrice = order.price;
			orderField.OrderStatus = order.status;
			orderField.VolumeTotalOrginal = order.volume_total_original;
			orderField.VolumeTotal = order.volume_total;
			orderField.VolumeTraded = order.volume_traded;
			orderField.AmountTraded = order.amount_traded;

			pRsp->orderFields.push_back(orderField);
			
		}
	}
	pWrapper->uplink(rspHeaders, pRsp);
}