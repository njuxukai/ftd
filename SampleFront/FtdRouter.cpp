#include "stdafx.h"
#include "FtdRouter.h"
#include <boost/format.hpp>
#include <ftd/session.h>
#include <functional>

/*
void FtdRouter::processReq(PackageSPtr pReq, mco_db_h db, SessionID sessionID)
{
	PackageSPtr pRsp = PackageSPtr(ftdcAll(pReq.get(), db));
	if (pRsp.get())
		Session::sendToTarget(*(pRsp.get()), sessionID);
}
*/
FtdRouter::FtdRouter(const std::string& cfgFile, int frontID, 
	const std::set<int>& validBrokerIDs) :
		m_cfgFile(cfgFile), 
		m_frontID(frontID), 
		m_validBrokerIDs(validBrokerIDs), 
		m_acceptor(0)
{
	//m_DB2.populate(std::function<void(mco_db_h)>(populate_db));
}

FtdRouter::~FtdRouter()
{}

void FtdRouter::registerUplinkFunction(const UplinkFunction& func)
{
	m_uplinkFunction = func;
}

void FtdRouter::start()
{
	//1 create initiator 
	FTD::PortSettings settings(m_cfgFile);
	m_acceptor = new FTD::SocketAcceptor((FTD::Application&)*this, settings);
	m_acceptor->start();
}

void FtdRouter::stop()
{
	if (m_acceptor)
	{
		m_acceptor->stop();
		delete m_acceptor;
		m_acceptor = 0;
	}
}

void FtdRouter::onCreate(const SessionID& id)
{
	std::cout << boost::format("SessionID[%d]创建\n") % id;
}
///connected
void FtdRouter::onConnect(const SessionID& id)
{
	std::cout << boost::format("SessionID[%d]已连接\n") % id;
}


//to be disconnected
void FtdRouter::onDisconnect(const SessionID& id)
{
	unresigterSequenceSubscription(id);
	std::cout << boost::format("SessionID[%d]已断开\n") % id;
}
/// Notification of a session successfully logging on
void FtdRouter::onLogon(const SessionID& id)
{}
/// Notification of a session logging off or disconnecting
void FtdRouter::onLogout(const SessionID& id)
{}
/// Notification of admin message being sent to target
void FtdRouter::toAdmin(Package& package, const SessionID& id)
{
	crack(package, id);
}
/// Notification of app message being sent to target
void FtdRouter::toApp(Package& package, const SessionID& id)
{
	crack(package, id);
}
/// Notification of admin message being received from target
void FtdRouter::fromAdmin(const Package& package, const SessionID& id)
{
	crack(package, id);
}
/// Notification of app message being received from target
void FtdRouter::fromApp(const Package& package, const SessionID& id)
{
	crack(package, id);
}

void FtdRouter::onHeartBeat()
{}

void FtdRouter::onHeartBeatWarning()
{}

void FtdRouter::OnPackage(const ReqUserLogin& req, const SessionID& id)
{
	ReqUserLogin* pCopy = (ReqUserLogin*)req.clone();
	pCopy->reqUserLoginField.FrontID = m_frontID;
	pCopy->reqUserLoginField.SessionID = id;
	//m_DB2.submit(std::bind(&FtdRouter::processReq, this, PackageSPtr(pCopy), std::placeholders::_1, id));
}

void FtdRouter::OnPackage(const ReqQryPrivateInitialData& req, const SessionID& id)
{
	//1 服务器端注册会话私有流订阅
	resigterSequenceSubscription(id, req.dissenminationstartField.SequenceSeries);
	//2 按需查询私有数据返回
	//m_DB2.submit(std::bind(&FtdRouter::processReq, this, PackageSPtr(req.clone()), std::placeholders::_1, id));
}

void FtdRouter::OnPackage(const ReqOrderInsert& req, const SessionID& id)
{
	ReqOrderInsert *pClone = (ReqOrderInsert*)req.clone();
	pClone->inputOrderField.FrontID = m_frontID;
	pClone->inputOrderField.SessionID = id;
	//m_DB2.submit(std::bind(&FtdRouter::processReq, this, PackageSPtr(pClone), std::placeholders::_1, id));
}


void FtdRouter::OnPackage(RspUserLogin& package, const SessionID& id)
{
	package.rspUserLoginField.FrontID = m_frontID;
	package.rspUserLoginField.SessionID = id;
	package.rspUserLoginField.HeartbeatInterval = 5;
}