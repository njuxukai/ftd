#include "stdafx.h"
#include "MockDB.h"

using namespace FTD;

void MockDB::processReqUerLogin(int frontID, int sessionID, const ReqUserLogin& req, RspUserLogin& rsp)
{
	rsp.clear();
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

void MockDB::processReqInputOrder(int frontID, int sessionID, const ReqOrderInsert& req, RspOrderInsert& rsp)
{
	rsp.clear();
}

void MockDB::processReqQryOrder(int frontID, int sessionID, const ReqQryOrder& req, RspQryOrder& rsp)
{
	rsp.clear();
}

void MockDB::processReqInitialPrivateData(int frontID, int sessionID, const ReqQryPrivateInitialData& req, RspQryPrivateInitialData& rsp)
{
	rsp.clear();
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