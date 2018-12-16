#include "XcpFtdcTraderApiImpl.h"
#include <ftd/SocketInitiator.h>
//#include <ftd/Port.h>

CXcpFtdcTraderApiImpl::CXcpFtdcTraderApiImpl(const char* pswDir)
{
	m_pswDir = pswDir;
	//check(and create directory) if exception m_pswDir = ""

	//open mmap files read last time trade day's last record serial number
}

CXcpFtdcTraderApiImpl::~CXcpFtdcTraderApiImpl()
{
	Release();
		
}

const char* CXcpFtdcTraderApiImpl::GetApiVersion()
{
	return "";
}

void CXcpFtdcTraderApiImpl::Release()
{
	m_pSpi ;
	if (m_pInitiator)
	{
		m_pInitiator->stop();
		delete m_pInitiator;
		m_pInitiator ;
	}
}

void CXcpFtdcTraderApiImpl::Init()
{
	//1 create initiator 
	FTD::PortSettings settings;
	FTD::Dictionary defaultDictionary;
	//TODO default setting
	settings.set(defaultDictionary);
	for (auto it = m_frontAddresses.begin(); it != m_frontAddresses.end(); it++)
	{
		FTD::PortID id;
		id.fromString(*it);
		FTD::Dictionary d;
		d.setString(FTD::SOCKET_CONNECT_HOST, id.getAddress());
		d.setInt(FTD::SOCKET_CONNECT_PORT, id.getPort());
		settings.set(id, d);
	}
	m_pInitiator = new FTD::SocketInitiator((FTD::Application&)*this,  settings);
	m_pInitiator->start();
}

void CXcpFtdcTraderApiImpl::Join()
{
	if (m_pInitiator != nullptr)
	{
		m_pInitiator->join();
	}
}

const char* CXcpFtdcTraderApiImpl::GetTradingDay()
{
	return "";
}


///注册回调接口
///@param pSpi 派生自回调接口类的实例
void CXcpFtdcTraderApiImpl::RegisterSpi(CXcpFtdcTraderSpi *pSpi)
{
	m_pSpi = pSpi;
}

void CXcpFtdcTraderApiImpl::RegisterFront(const char* frontAddr)
{
	m_frontAddresses.push_back(frontAddr);
}

void CXcpFtdcTraderApiImpl::SubscribePrivateTopic(THOST_TE_RESUME_TYPE resumeType)
{
}


void CXcpFtdcTraderApiImpl::SubscribePublicTopic(THOST_TE_RESUME_TYPE resumeType)
{
}

////////////////////////////////////////////////////////////////////////////
///FTD::Application FTD -->ApiImpl
///
////////////////////////////////////////////////////////////////////////////

/// Notification of a session begin created
void CXcpFtdcTraderApiImpl::onCreate(const FTD::SessionID&)
{

}

void CXcpFtdcTraderApiImpl::onConnect(const FTD::SessionID& id)
{
	m_connectedSessionID = id;
	if (m_pSpi)
	{
		m_pSpi->OnFrontConnected();
	}
}

void CXcpFtdcTraderApiImpl::onDisconnect(const FTD::SessionID& id)
{

}
/// Notification of a session successfully logging on
void CXcpFtdcTraderApiImpl::onLogon(const FTD::SessionID&)
{}
/// Notification of a session logging off or disconnecting
void CXcpFtdcTraderApiImpl::onLogout(const FTD::SessionID&)
{}

/// Notification of admin message being sent to target
void CXcpFtdcTraderApiImpl::toAdmin(FTD::Package&, const FTD::SessionID&)
{}

/// Notification of app message being sent to target
void CXcpFtdcTraderApiImpl::toApp(FTD::Package&, const FTD::SessionID&)
{}

/// Notification of admin message being received from target
void CXcpFtdcTraderApiImpl::fromAdmin(const FTD::Package&, const FTD::SessionID& id)
{
	
}

/// Notification of app message being received from target
void CXcpFtdcTraderApiImpl::fromApp(const FTD::Package& package, const FTD::SessionID& id)
{
	crack(package, id);
}

void CXcpFtdcTraderApiImpl::onHeartBeat()
{}

void CXcpFtdcTraderApiImpl::onHeartBeatWarning()
{}


///
int CXcpFtdcTraderApiImpl::send(FTD::Package& package, bool checkLogged)
{
	if (!m_pInitiator)
	{
		return XCP_ERR_CODE_INITIATOR_NULL;
	}
	if (checkLogged && !m_pInitiator->isLoggedOn())
	{
		return XCP_ERR_CODE_INITIATOR_NOT_LOGGED;
	}

	if (FTD::Session::sendToTarget(package, m_connectedSessionID))
	{
		return XCP_OK;
	}
	else
	{
		return XCP_ERR_CODE_FTD_SEND_FAILURE;
	}
	
}
////////////////////////////////////////////////////////////////////////////
///Trader opertaion
///
////////////////////////////////////////////////////////////////////////////
///用户登录请求
int CXcpFtdcTraderApiImpl::ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequestID)
{
	FTD::ReqUserLogin package;
	package.clear();
	memcpy(&package.reqUserLoginField, pReqUserLogin, sizeof(CXcpFtdcReqUserLoginField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package, false);
}

///用户登出请求
int CXcpFtdcTraderApiImpl::ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID)
{
	FTD::ReqUserLogout package;
	package.clear();
	memcpy(&package.reqUserLogoutField, pReqUserLogout, sizeof(CXcpFtdcReqUserLoginField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package, false);
}

///报单请求
int CXcpFtdcTraderApiImpl::ReqOrderInsert(CXcpFtdcInputOrderField* pInputOrder, int nRequestID)
{
	FTD::ReqOrderInsert package;
	package.clear();
	memcpy(&package.inputOrderField, pInputOrder, sizeof(CXcpFtdcInputOrderField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///报单操作请求
int CXcpFtdcTraderApiImpl::ReqOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID)
{
	FTD::ReqOrderAction package;
	package.clear();
	memcpy(&package.inputOrderActionField, pInputOrderAction, sizeof(CXcpFtdcInputOrderActionField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///资金划转请求
int CXcpFtdcTraderApiImpl::ReqFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID)
{
	FTD::ReqFundTransfer package;
	package.clear();
	memcpy(&package.inputFundTransferField, pInputFundTransfer, sizeof(CXcpFtdcInputFundTransferField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///委托查询请求
int CXcpFtdcTraderApiImpl::ReqQryOrder(CXcpFtdcQryOrderField* pQryOrder, int nRequestID)
{
	FTD::ReqQryOrder package;
	package.clear();
	memcpy(&package.qryOrderField, pQryOrder, sizeof(CXcpFtdcQryOrderField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///成交查询请求
int CXcpFtdcTraderApiImpl::ReqQryTrade(CXcpFtdcQryTradeField* pQryTrade, int nRequestID)
{
	FTD::ReqQryTrade package;
	package.clear();
	memcpy(&package.qryTradeField, pQryTrade, sizeof(CXcpFtdcQryTradeField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///资金查询请求
int CXcpFtdcTraderApiImpl::ReqQryFund(CXcpFtdcQryFundField *pQryFund, int nRequestID)
{
	FTD::ReqQryFund package;
	package.clear();
	memcpy(&package.qryFundField, pQryFund, sizeof(CXcpFtdcQryFundField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///仓位查询请求
int CXcpFtdcTraderApiImpl::ReqQryPosition(CXcpFtdcQryPositionField *pQryPosition, int nRequestID)
{
	FTD::ReqQryPosition package;
	package.clear();
	memcpy(&package.qryPositionField, pQryPosition, sizeof(CXcpFtdcQryPositionField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///资金划转查询请求
int CXcpFtdcTraderApiImpl::ReqQryFundTransfer(CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID)
{
	FTD::ReqQryFundTransfer package;
	package.clear();
	memcpy(&package.qryFundTransferField, pQryFundTransfer, sizeof(CXcpFtdcQryFundTransferField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///查询历史委托请求
int CXcpFtdcTraderApiImpl::ReqQryHisOrder(CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID)
{
	FTD::ReqQryHisOrder package;
	package.clear();
	memcpy(&package.qryHisOrderField, pQryHisOrder, sizeof(CXcpFtdcQryHisOrderField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///查询历史成交请求
int CXcpFtdcTraderApiImpl::ReqQryHisTrade(CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID)
{
	FTD::ReqQryHisTrade package;
	package.clear();
	memcpy(&package.qryHisTradeField, pQryHisTrade, sizeof(CXcpFtdcQryHisTradeField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///查询历史资金划转请求
int CXcpFtdcTraderApiImpl::ReqQryHisFundTransfer(CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID)
{
	FTD::ReqQryHisFundTransfer package;
	package.clear();
	memcpy(&package.qryHisFundTransferField, pQryHisFundTransfer, sizeof(CXcpFtdcQryHisFundTransferField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///查询证券信息请求
int CXcpFtdcTraderApiImpl::ReqQryInstrument(CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
	FTD::ReqQryInstrument package;
	package.clear();
	memcpy(&package.qryInstrumentField, pQryInstrument, sizeof(CXcpFtdcQryInstrumentField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///查询ETF信息请求
int CXcpFtdcTraderApiImpl::ReqQryETF(CXcpFtdcQryETFField *pQryETF, int nRequestID)
{
	FTD::ReqQryETF package;
	package.clear();
	memcpy(&package.qryETFField, pQryETF, sizeof(CXcpFtdcQryETFField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///查询ETF成分股信息请求
int CXcpFtdcTraderApiImpl::ReqQryETFComposition(CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID)
{
	FTD::ReqQryETFComposition package;
	package.clear();
	memcpy(&package.qryETFCompositionField, pQryEtfComposition, sizeof(CXcpFtdcQryETFCompositionField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///查询分级基金信息请求
int CXcpFtdcTraderApiImpl::ReqQryStructuredFund(CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID)
{
	FTD::ReqQryStructuredFund package;
	package.clear();
	memcpy(&package.qryStructuredFundField, pQryStructuredFund, sizeof(CXcpFtdcQryStructuredFundField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///查询可申购新股信息请求
int CXcpFtdcTraderApiImpl::ReqQryPurchasableNewIssueSecurity(CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID)
{
	FTD::ReqQryPurchasableNewIssueSecurity package;
	package.clear();
	memcpy(&package.qryNewIssueSecurityField, pQryPurchasableNewSecurity, sizeof(CXcpFtdcQryNewIssueSecurityField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///查询客户新股申购额度请求
int CXcpFtdcTraderApiImpl::ReqQryPurchaseQuota(CXcpFtdcQryPurchaseQuotaField *pQryPurchaseQuota, int nRequestID)
{
	FTD::ReqQryPurchaseQuota package;
	package.clear();
	memcpy(&package.qryPurchaseQuotaField, pQryPurchaseQuota, sizeof(CXcpFtdcQryPurchaseQuotaField));
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspUserLogin& package, const FTD::SessionID& id)
{
	if (!m_pSpi)
		return;
	CXcpFtdcRspUserLoginField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	memcpy(&contentField, &package.rspUserLoginField, sizeof(CXcpFtdcRspUserLoginField));
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	m_pSpi->OnRspUserLogin(&contentField, &errorField, package.requestSourceField.RequestID, true);
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::ForceExit& package, const FTD::SessionID& id) 
{
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspUserLogout& package, const FTD::SessionID& id)
{
	if (!m_pSpi)
		return;
	CXcpFtdcRspUserLogoutField contentField;
	CXcpFtdcErrorField errorField = { 0 };
	memcpy(&contentField, &package.rspUserLogoutField, sizeof(CXcpFtdcRspUserLogoutField));
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	m_pSpi->OnRspUserLogout(&contentField, &errorField, package.requestSourceField.RequestID, true);
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspOrderInsert& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcInputOrderField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	memcpy(&contentField, &package.inputOrderField, sizeof(CXcpFtdcInputOrderField));
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	m_pSpi->OnRspInputOrder(&contentField, &errorField, package.requestSourceField.RequestID, true);
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspOrderAction& package, const FTD::SessionID& id)
{
	if (!m_pSpi)
		return;
	CXcpFtdcInputOrderActionField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	memcpy(&contentField, &package.inputOrderActionField, sizeof(CXcpFtdcInputOrderActionField));
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	m_pSpi->OnRspInputOrderAction(&contentField, &errorField, package.requestSourceField.RequestID, true);
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspFundTransfer& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcInputFundTransferField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	memcpy(&contentField, &package.inputFundTransferField, sizeof(CXcpFtdcInputFundTransferField));
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	m_pSpi->OnRspInputFundTransfer(&contentField, &errorField, package.requestSourceField.RequestID, true);
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryFund& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcFundField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	memcpy(&contentField, &package.fundField, sizeof(CXcpFtdcFundField));
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	m_pSpi->OnRspQryFund(&contentField, &errorField, package.requestSourceField.RequestID, true);
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryPosition& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcPositionField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.positionFields.size() == 0)
	{
		m_pSpi->OnRspQryPosition(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.positionFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.positionFields[i], sizeof(CXcpFtdcPositionField));
		m_pSpi->OnRspQryPosition(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}	
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryOrder& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcOrderField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.orderFields.size() == 0)
	{
		m_pSpi->OnRspQryOrder(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.orderFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.orderFields[i], sizeof(CXcpFtdcOrderField));
		m_pSpi->OnRspQryOrder(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryTrade& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcTradeField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.tradeFields.size() == 0)
	{
		m_pSpi->OnRspQryTrade(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.tradeFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.tradeFields[i], sizeof(CXcpFtdcTradeField));
		m_pSpi->OnRspQryTrade(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryFundTransfer& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcFundTransferField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.fundTransferFields.size() == 0)
	{
		m_pSpi->OnRspQryFundTransfer(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.fundTransferFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.fundTransferFields[i], sizeof(CXcpFtdcFundTransferField));
		m_pSpi->OnRspQryFundTransfer(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryHisOrder& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcOrderField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.orderFields.size() == 0)
	{
		m_pSpi->OnRspQryHisOrder(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.orderFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.orderFields[i], sizeof(CXcpFtdcOrderField));
		m_pSpi->OnRspQryHisOrder(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryHisTrade& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcTradeField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.tradeFields.size() == 0)
	{
		m_pSpi->OnRspQryHisTrade(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.tradeFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.tradeFields[i], sizeof(CXcpFtdcTradeField));
		m_pSpi->OnRspQryHisTrade(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryHisFundTransfer& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcFundTransferField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.fundTransferFields.size() == 0)
	{
		m_pSpi->OnRspQryHisFundTransfer(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.fundTransferFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.fundTransferFields[i], sizeof(CXcpFtdcFundTransferField));
		m_pSpi->OnRspQryHisFundTransfer(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryInstrument& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcInstrumentField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.instrumentFields.size() == 0)
	{
		m_pSpi->OnRspQryInstrument(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.instrumentFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.instrumentFields[i], sizeof(CXcpFtdcInstrumentField));
		m_pSpi->OnRspQryInstrument(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryStructuredFund& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcStructuredFundField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.structuredFundFields.size() == 0)
	{
		m_pSpi->OnRspQryStructuredFund(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.structuredFundFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.structuredFundFields[i], sizeof(CXcpFtdcStructuredFundField));
		m_pSpi->OnRspQryStructuredFund(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryETF& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcETFField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.eTFFields.size() == 0)
	{
		m_pSpi->OnRspQryETF(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.eTFFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.eTFFields[i], sizeof(CXcpFtdcETFField));
		m_pSpi->OnRspQryETF(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryETFComposition& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcETFCompositionField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.eTFCompositionFields.size() == 0)
	{
		m_pSpi->OnRspQryETFComposition(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.eTFCompositionFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.eTFCompositionFields[i], sizeof(CXcpFtdcETFCompositionField));
		m_pSpi->OnRspQryETFComposition(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryPurchasableNewIssueSecurity& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcNewIssueSecurityField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.newIssueSecurityFields.size() == 0)
	{
		m_pSpi->OnRspQryPurchasableNewIssueSecurity(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.newIssueSecurityFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.newIssueSecurityFields[i], sizeof(CXcpFtdcNewIssueSecurityField));
		m_pSpi->OnRspQryPurchasableNewIssueSecurity(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryPurchaseQuota& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	CXcpFtdcPurchaseQuotaField contentField = { 0 };
	CXcpFtdcErrorField errorField = { 0 };
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	if (package.purchaseQuotaFields.size() == 0)
	{
		m_pSpi->OnRspQryPurchaseQuota(0, &errorField, package.requestSourceField.RequestID, true);
		return;
	}
	int dataLen = package.purchaseQuotaFields.size();
	bool isLast = false;
	for (int i = 0; i < dataLen; i++)
	{
		if (i == dataLen - 1)
			isLast = true;
		memcpy(&contentField, &package.purchaseQuotaFields[i], sizeof(CXcpFtdcPurchaseQuotaField));
		m_pSpi->OnRspQryPurchaseQuota(&contentField, &errorField, package.requestSourceField.RequestID, isLast);
	}
}

///与本地缓存的增量数据进行合并后再调用spi
void CXcpFtdcTraderApiImpl::OnPackage(FTD::RspQryPrivateInitialData& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
	{
		m_privateDataSynced = true;
		return;
	}

	int dataLen = package.executionReportFields.size();
	CXcpFtdcExecutionReportField contentfield = { 0 };
	for (int i = 0; i < dataLen; i++)
	{
		memcpy(&contentfield, &package.executionReportFields[i], sizeof(CXcpFtdcExecutionReportField));
		m_BufferExecutionReport[contentfield.ReportSysID] = contentfield;
	}

	for (auto it = m_BufferExecutionReport.begin(); it != m_BufferExecutionReport.end(); it++)
	{
		m_pSpi->OnRtnOrderExecutionReport(&(it->second));
	}
	m_BufferExecutionReport.clear();
	m_privateDataSynced = true;
}

///增量数据，如果没有完成同步则需要缓存在本地
void CXcpFtdcTraderApiImpl::OnPackage(FTD::IncExecutionReports& package, const FTD::SessionID& id) 
{
	if (!m_pSpi)
		return;
	int dataLen = package.executionReportFields.size();
	if (dataLen == 0)
		return;
	CXcpFtdcExecutionReportField contentfield = { 0 };	
	for (int i = 0; i < dataLen; i++)
	{
		memcpy(&contentfield, &package.executionReportFields[i], sizeof(CXcpFtdcExecutionReportField));
		if (m_privateDataSynced)
		{
			m_pSpi->OnRtnOrderExecutionReport(&contentfield);
		} 
		else
		{
			m_BufferExecutionReport[contentfield.ReportSysID] = contentfield;
		}
	}
}

