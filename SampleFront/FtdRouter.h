#pragma once
#include <ftd/SocketAcceptor.h>
#include <ftd/FTD30/PackageCracker.h>
#include <ftd/session.h>

#include "FrontFunctions.h"

using namespace FTD;

typedef std::function<void(PlainHeaders& headers, const std::string&)> RouterUplinkCallback;

struct FtdRouterParameter
{
	std::string cfgFname;
	int frontID;
	std::set<SessionID> allowedSessionIDs;
	bool uplinkMultiFlag;
};

class FtdRouter : public FTD::Application, public FTD::PackageCracker
{
public:
	FtdRouter(const FtdRouterParameter& parameter);
	~FtdRouter();

	void registerUplinkCallback(RouterUplinkCallback func);
	void uplink(const Package& package, const SessionID& id);
	void uplink(PlainHeaders& headers, const std::string& body);
	void start();
	void stop();

	void processDownlink(const PlainHeaders& headers, const std::string& body);

	void processDownlinkAdmin(const PlainHeaders& headers, const std::string& body);

	void processDownlinkAppRsp(const PlainHeaders& headers, const std::string& body);

	void processDownlinkPrivateAndBoardcast(const PlainHeaders& headers, const std::string& body);

	//void DeliveryFtdcMessage(const )
	virtual void onCreate(const SessionID&);
	///connected
	virtual void onConnect(const SessionID&);
	//to be disconnected
	virtual void onDisconnect(const SessionID&);
	/// Notification of a session successfully logging on
	virtual void onLogon(const SessionID&);
	/// Notification of a session logging off or disconnecting
	virtual void onLogout(const SessionID&);
	/// Notification of admin message being sent to target
	virtual void toAdmin(Package&, const SessionID&);
	/// Notification of app message being sent to target
	virtual void toApp(Package&, const SessionID&);
	/// Notification of admin message being received from target
	virtual void fromAdmin(const Package&, const SessionID&);
	/// Notification of app message being received from target
	virtual void fromApp(const Package&, const SessionID&);

	virtual void onHeartBeat();

	virtual void onHeartBeatWarning();

	virtual void OnPackage(const ReqUserLogin& package, const SessionID& id);
	
	virtual void OnPackage(const ReqQryPrivateInitialData& package, const SessionID& id);

	virtual void OnPackage(const ReqOrderInsert& package, const SessionID& id);

	virtual void OnPackage(RspUserLogin& package, const SessionID& id);

	///void processReq(PackageSPtr pReq, mco_db_h db, SessionID sessionID);

	//私有流/公有流订阅管理
	void resigterSequenceSubscription(const SessionID& id,int sequenceSerie)
	{
		if (m_subMap.find(sequenceSerie) == m_subMap.end())
		{
			m_subMap[sequenceSerie] = std::set<SessionID>();
		}
		m_subMap[sequenceSerie].insert(id);
	}

	void unresigterSequenceSubscription(const SessionID& id)
	{
		for (auto it = m_subMap.begin(); it != m_subMap.end(); it++)
		{
			it->second.erase(id);
		}
	}

	void publishExecutionReport(const CFtdcExecutionReportField& field)
	{
		if (m_subMap.find(field.SequenceSeries) != m_subMap.end())
		{
			IncExecutionReports package;
			package.m_header.sequenceSeries = field.SequenceSeries;
			package.m_header.sequenceNO = field.SequenceNo;
			package.executionReportFields.push_back(field);
			for (auto it = m_subMap[field.SequenceSeries].begin(); it != m_subMap[field.SequenceSeries].end(); it++)
			{
				Session::sendToTarget(package, *it);
			}
		}
	}

private:
	FtdRouterParameter m_parameter;

	
	FTD::Acceptor* m_acceptor;
	std::map<int, std::set<SessionID>> m_subMap;
	//消息队列上传回调
	RouterUplinkCallback m_uplinkCallback;
	//处理下行的管理信息
	FTD::PackageBuffer m_downlinkAdminBuffer;
};