#pragma once
#include <ftd/SocketAcceptor.h>
#include <ftd/FTD30/PackageCracker.h>
#include "MockDB.h"

using namespace FTD;
class EchoServer : public FTD::Application, public FTD::PackageCracker
{
public:
	EchoServer(std::string cfgFile, int frontID=1);
	~EchoServer();

	void connect();
	void disconnect();

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

	virtual void OnPackage(const ReqUserLogin& package, const SessionID&);
	
	virtual void OnPackage(const ReqQryPrivateInitialData& package, const SessionID&);

	//私有流订阅管理
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

private:
	std::map<int, std::set<SessionID>> m_subMap;
	std::string m_cfgFile;
	FTD::Acceptor* m_acceptor;
	const int m_frontID;
	MockDB m_DB;
};