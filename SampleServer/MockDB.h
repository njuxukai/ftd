#pragma once
#include <map>
#include <ftd/FTD30/Packages.h>

using namespace FTD;

struct SessionKey
{
	int frontID;
	int sessionID;
};
struct OrderKey
{
	int frontID;
	int sessionID;
	int orderRef;

	friend bool operator<(const OrderKey&, const OrderKey&);
	friend bool operator==(const OrderKey&, const OrderKey&);
	friend bool operator!=(const OrderKey&, const OrderKey&);
};

inline bool operator<(const OrderKey& lv, const OrderKey& rv)
{
	return (lv.frontID < rv.frontID)
		|| (lv.frontID == rv.frontID && lv.sessionID < rv.sessionID)
		|| (lv.frontID == rv.frontID && lv.sessionID == rv.sessionID && lv.orderRef < rv.orderRef);
}

inline bool operator==(const OrderKey& lv, const OrderKey& rv)
{
	return  (lv.frontID == rv.frontID && lv.sessionID == rv.sessionID && lv.orderRef == rv.orderRef);
}

inline bool operator!=(const OrderKey& lv, const OrderKey& rv)
{
	return (lv.frontID != rv.frontID || lv.sessionID != rv.sessionID || lv.orderRef != rv.orderRef);
}


class MockDB
{
public:
	void processReqUerLogin(int frontID, int sessionID, const ReqUserLogin& req, RspUserLogin& rsp);

	void processReqInputOrder(int frontID, int sessionID, const ReqOrderInsert& req, RspOrderInsert& rsp);

	void processReqQryOrder(int frontID, int sessionID, const ReqQryOrder& req, RspQryOrder& rsp);

	void processReqInitialPrivateData(int frontID, int sessionID, const ReqQryPrivateInitialData& req, RspQryPrivateInitialData& rsp);

private:
	int m_frontID;
	int QryMaxOrderRef(int frontID, int sessionID);
	std::map<OrderKey, CFtdcOrderField> m_orderMap;


};