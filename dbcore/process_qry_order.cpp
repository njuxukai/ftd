#include "ftdc_all.h"

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

	for (int i = 0; i < 10; i++)
	{
		CFtdcOrderField order = { 0 };
		sprintf(order.InstrumentCode, "%d", 600000 + i);
		order.Direction = FTDC_D_BUY;
		order.OrderStatus = FTDC_OS_PART_TRADED;
		//order.OrderStatus = FTDC_OS_0
		order.AmountTraded = 900;
		order.VolumeTraded = 100;
		order.VolumeTotalOrginal = 200;
		order.LimitPrice = 9;
		order.OrderExchangeID = i * 10 + 9;
		order.OrderSysID = 100 + i;
		order.FrontID = 1;
		order.SessionID = 1;
		order.OrderRef = i + 1;
		pRsp->orderFields.push_back(order);
	}
	pWrapper->uplink(rspHeaders, pRsp);
}