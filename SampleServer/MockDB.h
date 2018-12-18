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

struct ReportKey
{
	int serie;
	int sno;
	friend bool operator<(const ReportKey&, const ReportKey&);
	friend bool operator==(const ReportKey&, const ReportKey&);
	friend bool operator!=(const ReportKey&, const ReportKey&);
};
inline bool operator<(const ReportKey& lv, const ReportKey& rv)
{
	return (lv.serie < rv.serie)
		|| (lv.serie == rv.serie && lv.sno < rv.sno);
}

inline bool operator==(const ReportKey& lv, const ReportKey& rv)
{
	return  (lv.serie == rv.serie && lv.sno == rv.sno );
}

inline bool operator!=(const ReportKey& lv, const ReportKey& rv)
{
	return (lv.serie != rv.serie || lv.sno != rv.sno);
}

class MockDB
{
public:
	void processReqUerLogin(int frontID, int sessionID, const ReqUserLogin& req, RspUserLogin& rsp);

	void processReqInputOrder(int frontID, int sessionID, const ReqOrderInsert& req, RspOrderInsert& rsp, std::vector<CFtdcExecutionReportField>& reports);

	void processReqQryOrder(int frontID, int sessionID, const ReqQryOrder& req, RspQryOrder& rsp);

	void processReqInitialPrivateData(int frontID, int sessionID, const ReqQryPrivateInitialData& req, RspQryPrivateInitialData& rsp);

private:
	int m_frontID;
	void QryMaxOrderRef(int frontID, int sessionID, int& result);
	void QryExecutionReport(int sserie, int sno, std::vector<CFtdcExecutionReportField>& result);
	std::map<OrderKey, CFtdcOrderField> m_orderMap;
	std::map<int, CFtdcTradeField> m_tradeMap;
	std::map<ReportKey, CFtdcExecutionReportField> m_executionReportMap;
	int m_orderSeq;
	int m_tradeSeq;
	std::map<int, int> m_privateSeq; //每个客户一个


};