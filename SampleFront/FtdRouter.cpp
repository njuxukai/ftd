#include "stdafx.h"
#include "FtdRouter.h"
#include <boost/format.hpp>
#include <ftd/session.h>
#include <functional>


FtdRouter::FtdRouter(const FtdRouterParameter& parameter) :
		m_parameter(parameter), 
		m_acceptor(0),
		m_downlinkAdminBuffer(false)
{
}

FtdRouter::~FtdRouter()
{}

void FtdRouter::registerUplinkCallback(RouterUplinkCallback func)
{
	m_uplinkCallback = func;
}

void FtdRouter::uplink(const Package& package, const SessionID& id)
{
	PlainHeaders headers = { 0 };
	headers.source_session = id;
	if ( package.isNoneMode() )
		headers.admin_flag = QMSG_FLAG_ADMIN;
	else
		headers.admin_flag = QMSG_FLAG_APP;

	if ( package.isRequest() )
		headers.msg_type = QMSG_TYPE_REQ;
	else if ( package.isResponse() )
		headers.msg_type = QMSG_TYPE_RSP;
	else if ( package.isPrivteMode() )
		headers.msg_type = QMSG_TYPE_PRIVATE;
	else
		headers.msg_type = QMSG_TYPE_BOARDCAST;

	headers.multi_flag = QMSG_FLAG_SINGLE_FTDC;
	std::string body;
	int count;
	package.toSingleConcatFtdcMessage(body, count);

	uplink(headers, body);
}

void FtdRouter::uplink(PlainHeaders& headers, const std::string& body)
{
	if (m_uplinkCallback)
	{
		m_uplinkCallback(headers, body);
	}
}
void FtdRouter::start()
{
	//1 create initiator 
	FTD::PortSettings settings(m_parameter.cfgFname);
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

void FtdRouter::processDownlink(const PlainHeaders& headers, const std::string& body)
{
	if (headers.admin_flag == QMSG_FLAG_ADMIN)
	{
		processDownlinkAdmin(headers, body);
		return;
	}
	if (headers.msg_type == QMSG_TYPE_RSP)
	{
		processDownlinkAppRsp(headers, body);
		return;
	}
	if (headers.msg_type == QMSG_TYPE_PRIVATE 
		|| headers.msg_type == QMSG_TYPE_BOARDCAST)
	{
		processDownlinkPrivateAndBoardcast(headers, body);
		return;
	}
}
/*
必须调用 Session->send(Package&)，使Session的状态变化
*/
void FtdRouter::processDownlinkAdmin(const PlainHeaders& headers, const std::string& body) 
{
	if (headers.multi_flag == QMSG_FLAG_MULTI_FTDC)
	{
		return;
	}
	Package* pPack = m_downlinkAdminBuffer.OnFtdcMessage(body);
	if (pPack)
	{
		Session::sendToTarget(*pPack, headers.source_session);
	}
}

void FtdRouter::processDownlinkAppRsp(const PlainHeaders& headers, const std::string& body) 
{
	if (headers.multi_flag == QMSG_FLAG_MULTI_FTDC)
	{
		std::vector<std::string> ftdcMsgs;
		std::vector<std::string> ftdMsgs;
		FtdMessageUtil::splitFtdcMessages(body, ftdcMsgs);
		for (unsigned int i = 0; i < ftdcMsgs.size(); i++)
		{
			ftdMsgs.push_back(FtdMessageUtil::formatFtdMessage(ftdMsgs[i]));
		}
		Session::sendToTarget(ftdMsgs, headers.source_session);
	}
	else
	{
		Session::sendToTarget(FtdMessageUtil::formatFtdMessage(body), headers.source_session);
	}
}

void FtdRouter::processDownlinkPrivateAndBoardcast(const PlainHeaders& headers, const std::string& body) 
{
	//TODO consider m_subMap thread safe
	if (m_subMap.find(headers.sequence_series) == m_subMap.end())
		return;
	std::set<SessionID> ids = m_subMap[headers.sequence_series];
	if (headers.multi_flag == QMSG_FLAG_MULTI_FTDC)
	{
		std::vector<std::string> ftdcMsgs;
		std::vector<std::string> ftdMsgs;
		FtdMessageUtil::splitFtdcMessages(body, ftdcMsgs);
		for (unsigned int i = 0; i < ftdcMsgs.size(); i++)
		{
			ftdMsgs.push_back(FtdMessageUtil::formatFtdMessage(ftdMsgs[i]));
		}
		for (auto it = ids.begin(); it != ids.end(); it++)
			Session::sendToTarget(ftdMsgs, *it);
	}
	else
	{
		std::string ftdMsg = FtdMessageUtil::formatFtdMessage(body);
		for (auto it = ids.begin(); it != ids.end(); it++)
			Session::sendToTarget(ftdMsg, *it);
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
/// called by: session->sendRaw(Package&)
void FtdRouter::toAdmin(Package& package, const SessionID& id)
{
	crack(package, id);
}
/// called by: session->sendRaw(Package&) 
void FtdRouter::toApp(Package& package, const SessionID& id)
{
	crack(package, id);
}
/// called by: session->next(const Package&)
void FtdRouter::fromAdmin(const Package& package, const SessionID& id)
{
	crack(package, id);
}
/// called by: session->next(const Package&)
void FtdRouter::fromApp(const Package& package, const SessionID& id)
{
	crack(package, id);
}


void FtdRouter::onHeartBeat()
{}

void FtdRouter::onHeartBeatWarning()
{}

/******************************************************************************
***从session上接受到Package，简单的处理验证逻辑后，将数据上行到消息队列
******************************************************************************/
void FtdRouter::OnPackage(const ReqUserLogin& req, const SessionID& id)
{
	std::shared_ptr<ReqUserLogin> pCopy = std::shared_ptr<ReqUserLogin>((ReqUserLogin*)req.clone());
	pCopy->reqUserLoginField.FrontID = m_parameter.frontID;
	pCopy->reqUserLoginField.SessionID = id;
	pCopy->formatFtdcMessages();
	uplink(*pCopy, id);
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
	pClone->inputOrderField.FrontID = m_parameter.frontID;
	pClone->inputOrderField.SessionID = id;
	//m_DB2.submit(std::bind(&FtdRouter::processReq, this, PackageSPtr(pClone), std::placeholders::_1, id));
}


/******************************************************************************
***即将发送到session上的Package，这里应只接受管理消息RspUseLogin
***  注: 应用消息直接转发至各session(rsp按sessionid;private,boardcast按series_id)
******************************************************************************/
void FtdRouter::OnPackage(RspUserLogin& package, const SessionID& id)
{
	package.rspUserLoginField.FrontID = m_parameter.frontID;
	package.rspUserLoginField.SessionID = id;
	package.rspUserLoginField.HeartbeatInterval = 5;
}