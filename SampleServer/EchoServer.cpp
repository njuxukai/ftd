#include "stdafx.h"
#include "EchoServer.h"
#include <boost/format.hpp>
#include <ftd/session.h>

//void processReq(FTD::Package* package, )
EchoServer::EchoServer(std::string cfgFile, int frontID) :m_cfgFile(cfgFile), m_frontID(frontID), m_acceptor(0)
{
}

EchoServer::~EchoServer()
{}

void EchoServer::connect()
{
	//1 create initiator 
	FTD::PortSettings settings(m_cfgFile);
	m_acceptor = new FTD::SocketAcceptor((FTD::Application&)*this, settings);
	m_acceptor->start();
}

void EchoServer::disconnect()
{
	if (m_acceptor)
		m_acceptor->stop();
	delete m_acceptor;
	m_acceptor = 0;
}

void EchoServer::onCreate(const SessionID& id)
{
	std::cout << boost::format("SessionID[%d]创建\n") % id;
}
///connected
void EchoServer::onConnect(const SessionID& id)
{
	std::cout << boost::format("SessionID[%d]已连接\n") % id;
}


//to be disconnected
void EchoServer::onDisconnect(const SessionID& id)
{
	unresigterSequenceSubscription(id);
	std::cout << boost::format("SessionID[%d]已断开\n") % id;
}
/// Notification of a session successfully logging on
void EchoServer::onLogon(const SessionID& id)
{}
/// Notification of a session logging off or disconnecting
void EchoServer::onLogout(const SessionID& id)
{}
/// Notification of admin message being sent to target
void EchoServer::toAdmin(Package& package, const SessionID& id)
{
	crack(package, id);
}
/// Notification of app message being sent to target
void EchoServer::toApp(Package& package, const SessionID& id)
{
	crack(package, id);
}
/// Notification of admin message being received from target
void EchoServer::fromAdmin(const Package& package, const SessionID& id)
{
	crack(package, id);
}
/// Notification of app message being received from target
void EchoServer::fromApp(const Package& package, const SessionID& id)
{
	crack(package, id);
}

void EchoServer::onHeartBeat()
{}

void EchoServer::onHeartBeatWarning()
{}

void EchoServer::OnPackage(const ReqUserLogin& req, const SessionID& id)
{
	RspUserLogin rsp;
	//m_DB.processReqUerLogin(m_frontID, id, req, rsp);
	//Session::sendToTarget(rsp, id);
}

void EchoServer::OnPackage(const ReqQryPrivateInitialData& req, const SessionID& id)
{
	RspQryPrivateInitialData rsp;
	//1 服务器端注册会话私有流订阅
	resigterSequenceSubscription(id, req.dissenminationstartField.SequenceSeries);
	//2 按需查询私有数据返回
	m_DB.processReqInitialPrivateData(m_frontID, id, req, rsp);
	Session::sendToTarget((Package&)rsp, id);
}

void EchoServer::OnPackage(const ReqOrderInsert& req, const SessionID& id)
{
	RspOrderInsert rsp;
	std::vector<CFtdcExecutionReportField> reports;
	m_DB.processReqInputOrder(m_frontID, id, req, rsp, reports);
	Session::sendToTarget(rsp, id);
	for (unsigned int i = 0; i < reports.size(); i++)
	{
		publishExecutionReport(reports[i]);
	}

}