#include "stdafx.h"
#include "MockDB.h"

using namespace FTD;

void MockDB::processReqUerLogin(int frontID, int sessionID, const ReqUserLogin& req, RspUserLogin& rsp)
{
	m_orderSeq = 0;
	m_tradeSeq = 0;
	//m_privateSeq.clear();

	rsp.clear();
	rsp.m_header.sequenceSeries = 0;
	rsp.m_header.sequenceNO = 0;
	rsp.rspUserLoginField.UserID = req.reqUserLoginField.UserID;
	rsp.rspUserLoginField.BrokerID = req.reqUserLoginField.BrokerID;
	strcpy(rsp.rspUserLoginField.SystemName, "模拟交易服务器");
	//日期填写
	strcpy(rsp.rspUserLoginField.TradingDay, "");
	rsp.rspUserLoginField.HeartbeatInterval = 5;
	//
	rsp.rspUserLoginField.FrontID = frontID;
	rsp.rspUserLoginField.SessionID = sessionID;
	QryMaxOrderRef(frontID, sessionID, rsp.rspUserLoginField.MaxOrderRef);
}

void MockDB::processReqInputOrder(int frontID, int sessionID, const ReqOrderInsert& req, RspOrderInsert& rsp, 
	std::vector<CFtdcExecutionReportField>& reports)
{
	//1返回rspOrderInput
	rsp.clear();
	rsp.m_header.sequenceSeries = req.m_header.sequenceSeries;
	rsp.m_header.sequenceNO = req.m_header.sequenceNO;
	memcpy(&rsp.inputOrderField, &rsp.inputOrderField, sizeof(CFtdcInputOrderField));
	rsp.pErrorField = 0;
	//1 Order
	CFtdcOrderField order = { 0 };
	order.UserID = req.inputOrderField.UserID;
	order.FrontID = frontID;
	order.SessionID = sessionID;
	order.OrderRef = req.inputOrderField.OrderRef;
	order.OrderSysID = ++m_orderSeq;
	order.BranchID = 1672;
	order.BrokerID = req.inputOrderField.BrokerID;
	order.Direction = req.inputOrderField.Direction;
	order.LimitPrice = req.inputOrderField.LimitPrice;
	order.PriceType = req.inputOrderField.PriceType;
	strcpy(order.InstrumentCode, req.inputOrderField.InstrumentCode);
	strcpy(order.ExchangeCode, req.inputOrderField.ExchangeCode);
	order.VolumeTotalOrginal = req.inputOrderField.VolumeTotalOrginal;
	order.VolumeTraded = order.VolumeTotalOrginal / 2;
	order.VolumeTotal = order.VolumeTotalOrginal - order.VolumeTraded;
	OrderKey key = { 0 };
	key.frontID = frontID;
	key.sessionID = sessionID;
	key.orderRef = order.OrderRef;
	m_orderMap[key] = order;
	//2 Mock生成成交回报
	CFtdcTradeField trade = { 0 };
	trade.BranchID = 1672;
	trade.BrokerID = req.inputOrderField.BrokerID;
	trade.CancelFlag = FTDC_TCF_Deal;
	trade.Direction = req.inputOrderField.Direction;
	strcpy(trade.ExchangeCode, req.inputOrderField.ExchangeCode);
	strcpy(trade.InstrumentCode, req.inputOrderField.InstrumentCode);
	trade.InvestorID = req.inputOrderField.InvestorID;
	trade.InvestUnitID = 0;
	trade.LimitPrice = req.inputOrderField.LimitPrice;
	trade.OrderRef = req.inputOrderField.OrderRef;
	trade.PriceTrade = trade.LimitPrice;
	trade.TradeExchangeID = ++m_tradeSeq;
	trade.TradeSysID = trade.TradeExchangeID;
	trade.VolumeTotalOrginal = req.inputOrderField.VolumeTotalOrginal;
	trade.VolumeTrade = trade.VolumeTotalOrginal / 2;
	m_tradeMap[trade.TradeSysID] = trade;
	//3 Mock生成OrderReport
	CFtdcExecutionReportField report = { 0 };
	report.SequenceSeries = order.UserID;
	if (m_privateSeq.find(order.UserID) == m_privateSeq.end())
	{
		m_privateSeq[order.UserID] = 1;
	}
	report.OrderRef = order.OrderRef;
	report.SequenceNo = m_privateSeq[order.UserID];
	m_privateSeq[order.UserID] += 1;
	ReportKey reportKey = { 0 };
	reportKey.serie = report.SequenceSeries;
	reportKey.sno = report.SequenceNo;
	m_executionReportMap[reportKey] = report;
	reports.push_back(report);

}

void MockDB::processReqQryOrder(int frontID, int sessionID, const ReqQryOrder& req, RspQryOrder& rsp)
{
	rsp.clear();
}

void MockDB::processReqInitialPrivateData(int frontID, int sessionID, const ReqQryPrivateInitialData& req, RspQryPrivateInitialData& rsp)
{
	rsp.clear();
	rsp.m_header.sequenceSeries = req.m_header.sequenceSeries;
	rsp.m_header.sequenceNO = req.m_header.sequenceNO;
	if (req.qryInitialDataField.IsBackward == FTDC_BF_True)
	{
		std::vector<CFtdcExecutionReportField> result;
		QryExecutionReport(req.dissenminationstartField.SequenceSeries, req.dissenminationstartField.SequenceNo, result);
		for (auto it = result.begin(); it != result.end(); it++)
		{
			rsp.executionReportFields.push_back(*it);
		}
	}
}

void MockDB::QryMaxOrderRef(int frontid, int sessionid, int& result)
{
	result = 1;
	for (auto it = m_orderMap.begin(); it != m_orderMap.end(); it++)
	{
		if (it->second.FrontID == frontid && it->second.SessionID && it->second.OrderRef >= result)
		{
			result = it->second.OrderRef + 1;
		}
	}
}

void MockDB::QryExecutionReport(int userid, int sno, std::vector<CFtdcExecutionReportField>& result)
{
	result.clear();
	for (auto it = m_executionReportMap.begin(); it != m_executionReportMap.end(); it++)
	{
		if (it->first.serie == userid && it->first.sno > sno)
		{
			result.push_back(it->second);
		}
	}
}