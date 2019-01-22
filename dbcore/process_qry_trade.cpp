#include "ftdc_all.h"

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

	for (int i = 0; i < 10; i++)
	{
		CFtdcTradeField trade = { 0 };
		sprintf(trade.InstrumentCode, "%d", 600000 + i);
		trade.Direction = FTDC_D_BUY;
		trade.VolumeTrade = 100;
		trade.PriceTrade = 1.2;
		trade.OrderSysID = i * 10 + 1;
		trade.TradeSysID = (i+1) * 100;
		sprintf(trade.TradeExchangeID, "%d" , i * 100 + 99);
		pRsp->tradeFields.push_back(trade);
	}

	pWrapper->uplink(rspHeaders, pRsp);
}