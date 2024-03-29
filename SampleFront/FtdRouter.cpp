#include "stdafx.h"
#include "FtdRouter.h"
#include <boost/format.hpp>
#include <ftd/session.h>
#include <functional>


FtdRouter::FtdRouter(const FtdRouterParameter& parameter) :
		m_parameter(parameter), 
		m_acceptor(0)
{
}

FtdRouter::~FtdRouter()
{}

void FtdRouter::registerUplinkCallback(RouterUplinkCallback func)
{
	m_uplinkCallback = func;
}

void FtdRouter::uplink(Package& package, const SessionID& id)
{
	package.formatFtdcMessages();
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
	if (headers.msg_type == QMSG_TYPE_MARKET_DATA)
	{
		processDownlinkMarketData(headers, body);
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
			ftdMsgs.push_back(FtdMessageUtil::formatFtdMessage(ftdcMsgs[i]));
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

void FtdRouter::processDownlinkMarketData(const PlainHeaders& headers, const std::string& body)
{
	if (!m_parameter.isMdFront)
		return;
	//行情数据目前支持单个证券一个pack 20191214
	if (headers.multi_flag == QMSG_FLAG_MULTI_FTDC)
	{
		return;
	}
	Package* pPack = m_downlinkAdminBuffer.OnFtdcMessage(body);
	if (pPack && pPack->m_transactionId == TID_IncMarketData)
	{
		IncMarketData* pMd = (IncMarketData*)pPack;
		CFtdcInstrumentField instrument = { 0 };
		instrument.ExchangeType = pMd->marketDataField.ExchangeType;
		strcpy(instrument.InstrumentCode, pMd->marketDataField.InstrumentCode);
		std::set<FTD::SessionID> subSessionIDs = m_mdSubManager.getSubSessionSet(instrument);
		if (subSessionIDs.size() > 0)
		{
			std::string ftdMsg = FtdMessageUtil::formatFtdMessage(body);
			for (auto id : subSessionIDs)
			{
				Session::sendToTarget(ftdMsg, id);
			}
		}		
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
void FtdRouter::onDisconnect(const SessionID& id, int reason)
{
	unresigterSequenceSubscription(id);
	m_mdSubManager.sessionOffline(id);
	std::cout << boost::format("SessionID[%d]断开,原因[%d]\n") % id %reason;
}

/// Notification of a session successfully logging on
void FtdRouter::onLogon(const SessionID& id)
{
	if (m_parameter.isMdFront)
	{
		m_mdSubManager.setSessionQuota(id, m_parameter.userMdQuota);
	}
}
/// Notification of a session logging off or disconnecting
void FtdRouter::onLogout(const SessionID& id)
{
	///TODO 行情订阅关掉
}
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
	
	std::cout << "At router, ON Package[ReqUserLogin]\n";
	// broker id error
	if (m_parameter.allowedBrokerIDs.find(req.reqUserLoginField.BrokerID)
		== m_parameter.allowedBrokerIDs.end())
	{
		RspUserLogin rsp;
		rsp.rspUserLoginField.BrokerID = req.reqUserLoginField.BrokerID;
		rsp.rspUserLoginField.UserID = req.reqUserLoginField.UserID;
		rsp.pErrorField = CFtdcErrorFieldPtr(new CFtdcErrorField());
		rsp.pErrorField->ErrorCode = -100;
		strcpy(rsp.pErrorField->ErrorText, "BrokerID错误");
		Session::sendToTarget(rsp, id);
		return;
	}
	//1 自身具备MdFront角色 2 跳过认证
	if (m_parameter.isMdFront && m_parameter.mdFrontSkipAuth 
		&& req.reqUserLoginField.TargetFrontTag == FTDC_FT_Md)
	{
		RspUserLogin rsp;
		rsp.rspUserLoginField.BrokerID = req.reqUserLoginField.BrokerID;
		rsp.rspUserLoginField.UserID = req.reqUserLoginField.UserID;
		rsp.pErrorField = CFtdcErrorFieldPtr(new CFtdcErrorField());
		memset(rsp.pErrorField.get(), 0, sizeof(CFtdcErrorField));
		Session::sendToTarget(rsp, id);
		return;
	}
	std::shared_ptr<ReqUserLogin> pCopy = std::shared_ptr<ReqUserLogin>((ReqUserLogin*)req.clone());
	pCopy->reqUserLoginField.FrontID = m_parameter.frontID;
	pCopy->reqUserLoginField.SessionID = id;
	uplink(*pCopy, id);
}

void FtdRouter::OnPackage(const ReqQryPrivateInitialData& req, const SessionID& id)
{
	//1 服务器端注册会话私有流订阅
	std::cout << "At router, ON Package[ReqQryPrivateInitialData]\n";
	resigterSequenceSubscription(id, req.dissenminationstartField.SequenceSeries);
	
	std::shared_ptr<ReqQryPrivateInitialData> pCopy 
		= std::shared_ptr<ReqQryPrivateInitialData>((ReqQryPrivateInitialData*)req.clone());
	uplink(*pCopy, id);
}

void FtdRouter::OnPackage(const ReqOrderInsert& req, const SessionID& id)
{
	std::shared_ptr<ReqOrderInsert> pCopy
		= std::shared_ptr<ReqOrderInsert>((ReqOrderInsert*)req.clone());
	pCopy->inputOrderField.FrontID = m_parameter.frontID;
	pCopy->inputOrderField.SessionID = id;
	uplink(*pCopy, id);
}

void FtdRouter::OnPackage(const ReqOrderAction& req, const SessionID& id)
{
	std::shared_ptr<ReqOrderAction> pCopy = 
		std::shared_ptr<ReqOrderAction>((ReqOrderAction*)req.clone());
	uplink(*pCopy, id);
}

void FtdRouter::OnPackage(const ReqQryFund& req, const SessionID& id)
{
	std::shared_ptr<ReqQryFund> pCopy = 
		std::shared_ptr<ReqQryFund>((ReqQryFund*)req.clone());
	uplink(*pCopy, id);
}

void FtdRouter::OnPackage(const ReqQryPosition& req, const SessionID& id)
{
	std::shared_ptr<ReqQryPosition> pCopy 
		= std::shared_ptr<ReqQryPosition>((ReqQryPosition*)req.clone());
	uplink(*pCopy, id);
}

void FtdRouter::OnPackage(const ReqQryOrder& req, const SessionID& id)
{
	std::shared_ptr<ReqQryOrder> pCopy 
		= std::shared_ptr<ReqQryOrder>((ReqQryOrder*)req.clone());
	uplink(*pCopy, id);
}

void FtdRouter::OnPackage(const ReqQryTrade& req, const SessionID& id)
{
	std::shared_ptr<ReqQryTrade> pCopy = 
		std::shared_ptr<ReqQryTrade>((ReqQryTrade*)req.clone());
	uplink(*pCopy, id);
}

void FtdRouter::OnPackage(const ReqQrySecurityAccount& req, const SessionID& id)
{
	std::shared_ptr<ReqQrySecurityAccount> pCopy =
		std::shared_ptr<ReqQrySecurityAccount>((ReqQrySecurityAccount*)req.clone());
	uplink(*pCopy, id);
}

void FtdRouter::OnPackage(const ReqSubMarketData& package, const SessionID& id)
{
	//订阅
	RspSubMarketData rsp;
	rsp.requestSourceField.RequestID = package.requestSourceField.RequestID;
	CFtdcErrorField error = { 0 };
	int size = package.instrumentFields.size();
	for (int i = 0; i < size; i++)
	{
		m_mdSubManager.sessionSubInstrument(id, package.instrumentFields[i], error.ErrorCode,
			error.ErrorText);
		rsp.instrumentFields.push_back(package.instrumentFields[i]);
		rsp.errorFields.push_back(error);
	}
	Session::sendToTarget(rsp, id);
}

void FtdRouter::OnPackage(const ReqUnsubMarketData& package, const SessionID& id)
{
	//退订
	RspUnsubMarketData rsp;
	rsp.requestSourceField.RequestID = package.requestSourceField.RequestID;
	CFtdcErrorField error = { 0 };
	int size = package.instrumentFields.size();
	for (int i = 0; i < size; i++)
	{
		m_mdSubManager.sessionUnsubInstrument(id, package.instrumentFields[i], error.ErrorCode,
			error.ErrorText);
		rsp.instrumentFields.push_back(package.instrumentFields[i]);
		rsp.errorFields.push_back(error);
	}
	Session::sendToTarget(rsp, id);
}

/******************************************************************************
***即将发送到session上的Package，这里应只接受管理消息RspUseLogin
***  注: 应用消息直接转发至各session(rsp按sessionid;private,boardcast按series_id)
******************************************************************************/
void FtdRouter::OnPackage(RspUserLogin& package, const SessionID& id)
{
	root_log(LOG_DEBUG, "OnPackage RspUserLogin,front_id=%d", m_parameter.frontID);
	package.rspUserLoginField.FrontID = m_parameter.frontID;
	package.rspUserLoginField.SessionID = id;
#ifdef _DEBUG
	package.rspUserLoginField.HeartbeatInterval = 0;
#endif
}