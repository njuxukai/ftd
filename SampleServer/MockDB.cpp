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
	rsp.rspUserLoginField.MaxOrderRef = QryMaxOrderRef(frontID, sessionID);
}

void MockDB::processReqInputOrder(int frontID, int sessionID, const ReqOrderInsert& req, RspOrderInsert& rsp)
{
	rsp.clear();
}

void MockDB::processReqQryOrder(int frontID, int sessionID, const ReqQryOrder& req, RspQryOrder& rsp)
{
	rsp.clear();
}

int MockDB::QryMaxOrderRef(int frontid, int sessionid)
{
	int maxOrderRef = 1;
	for (auto it = m_orderMap.begin(); it != m_orderMap.end(); it++)
	{
		if (it->second.FrontID == frontid && it->second.SessionID && it->second.OrderRef >= maxOrderRef)
		{
			maxOrderRef += 1;
		}
	}
	return maxOrderRef;
}