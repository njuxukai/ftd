#include "XcpFtdcMdUserApiImpl.h"
#include <ftd/SocketInitiator.h>

#define PRIVATE_CONN_FNAME "private.conn"
#define PUBLIC_CONN_FNAME "public.conn"


#define TOTAL_SIZE  12 

ConnMmap::ConnMmap(const std::string& fpath) :m_fpath(fpath)
{
	//if file not exist,create file and fill data
	boost::filesystem::path boostPath(m_fpath);
	if (!boost::filesystem::is_regular_file(boostPath))
	{
		boost::interprocess::file_mapping::remove(m_fpath.data());
		{
			std::ofstream file(m_fpath, std::ios::binary | std::ios::trunc);
			//file.seekp(static_cast<std::streamoff>(TOTAL_SIZE - 1));
			//file.write("", 1);
			std::fill_n(std::ostream_iterator<char>(file), TOTAL_SIZE, 0);
		}
	}
	fmap = boost::interprocess::file_mapping(m_fpath.data(), boost::interprocess::read_write);
	mreg = boost::interprocess::mapped_region(fmap, boost::interprocess::read_write);
	m_addr = mreg.get_address();
	m_size = mreg.get_size();
}


bool ConnMmap::writeDate(const std::string& tradeDate)
{
	if (!m_addr || m_size != TOTAL_SIZE || tradeDate.size() != 8)
		return false;
	memcpy(m_addr, tradeDate.data(), 8);
	return true;
}

bool ConnMmap::readDate(std::string& tradeDate)
{
	if (!m_addr || m_size != TOTAL_SIZE)
		return false;
	char buffer[9];
	memcpy(buffer, m_addr, 8);
	tradeDate.assign(buffer, 8);
	return true;
}

bool ConnMmap::writeSequenceSno(int sno)
{
	if (!m_addr || m_size != TOTAL_SIZE)
		return false;
	memcpy((char*)m_addr + 8, &sno, 4);
	return true;
}

bool ConnMmap::readSequenceSno(int& sno)
{
	if (!m_addr || m_size != TOTAL_SIZE)
		return false;
	memcpy(&sno, (char*)m_addr + 8, 4);
	return true;
}


CXcpFtdcMdUserApiImpl::CXcpFtdcMdUserApiImpl(const char* pswDir)
{
	m_pswDir = pswDir;
	//check(and create directory) if exception m_pswDir = ""

	//open mmap files read last time trade day's last record serial number
	m_publicResumeType = -1;
	m_privateResumeType = -1;
	m_pPrivateConn = new ConnMmap(std::string(pswDir) + PUBLIC_CONN_FNAME);
	m_pPublicConn = new ConnMmap(std::string(pswDir) + PRIVATE_CONN_FNAME);
}

CXcpFtdcMdUserApiImpl::~CXcpFtdcMdUserApiImpl()
{
	Release();
	if (m_pPrivateConn)
	{
		delete m_pPrivateConn;
		m_pPrivateConn = 0;
	}
	if (m_pPublicConn)
	{
		delete m_pPublicConn;
		m_pPublicConn = 0;
	}
}

const char* CXcpFtdcMdUserApiImpl::GetApiVersion()
{
	return "";
}

void CXcpFtdcMdUserApiImpl::Release()
{
	
	if (m_pInitiator)
	{
		m_pInitiator->stop(true);
		delete m_pInitiator;
		m_pInitiator = 0 ;
	}
	m_pSpi = NULL;
}

void CXcpFtdcMdUserApiImpl::Init()
{
	//1 create initiator 
	FTD::PortSettings settings;
	FTD::Dictionary defaultDictionary;
	//TODO default setting
	defaultDictionary.setBool(FTD::SCREEN_LOG_SHOW_OUTGOING, false);
	defaultDictionary.setBool(FTD::SCREEN_LOG_SHOW_INCOMING, false);
	defaultDictionary.setBool(FTD::SCREEN_LOG_SHOW_EVENTS, false);
	settings.set(defaultDictionary);
	for (auto it = m_frontAddresses.begin(); it != m_frontAddresses.end(); it++)
	{
		FTD::PortID id;
		id.fromString(*it);
		FTD::Dictionary d;
		d.merge(defaultDictionary);
		d.setString(FTD::SOCKET_CONNECT_HOST, id.getAddress());
		d.setInt(FTD::SOCKET_CONNECT_PORT, id.getPort());
		settings.set(id, d);
	}
	m_pInitiator = new FTD::SocketInitiator((FTD::Application&)*this,  settings);
	m_pInitiator->start();
}

void CXcpFtdcMdUserApiImpl::Join()
{
	if (m_pInitiator != nullptr)
	{
		m_pInitiator->join();
	}
}

const char* CXcpFtdcMdUserApiImpl::GetTradingDay()
{
	return m_tradingDay.data();
}


///注册回调接口
///@param pSpi 派生自回调接口类的实例
void CXcpFtdcMdUserApiImpl::RegisterSpi(CXcpFtdcMdUserSpi *pSpi)
{
	m_pSpi = pSpi;
}

void CXcpFtdcMdUserApiImpl::RegisterFront(const char* frontAddr)
{
	m_frontAddresses.push_back(frontAddr);
}



////////////////////////////////////////////////////////////////////////////
///FTD::Application FTD -->ApiImpl
///
////////////////////////////////////////////////////////////////////////////

/// Notification of a session begin created
void CXcpFtdcMdUserApiImpl::onCreate(const FTD::SessionID&)
{

}

void CXcpFtdcMdUserApiImpl::onConnect(const FTD::SessionID& id)
{
	m_connectedSessionID = id;
	if (m_pSpi)
	{
		m_pSpi->OnFrontConnected();
	}
}

void CXcpFtdcMdUserApiImpl::onDisconnect(const FTD::SessionID& id, int reason)
{
	if (m_pSpi)
	{
		m_pSpi->OnFrontDisconnected(reason);
	}
}
/// Notification of a session successfully logging on
void CXcpFtdcMdUserApiImpl::onLogon(const FTD::SessionID&)
{}
/// Notification of a session logging off or disconnecting
void CXcpFtdcMdUserApiImpl::onLogout(const FTD::SessionID&)
{}

/// Notification of admin message being sent to target
void CXcpFtdcMdUserApiImpl::toAdmin(FTD::Package& package, const FTD::SessionID& id)
{}

/// Notification of app message being sent to target
void CXcpFtdcMdUserApiImpl::toApp(FTD::Package&, const FTD::SessionID&)
{}

/// Notification of admin message being received from target
void CXcpFtdcMdUserApiImpl::fromAdmin(const FTD::Package& package, const FTD::SessionID& id)
{
	crack(package, id);
}

/// Notification of app message being received from target
void CXcpFtdcMdUserApiImpl::fromApp(const FTD::Package& package, const FTD::SessionID& id)
{
	crack(package, id);
}

void CXcpFtdcMdUserApiImpl::onHeartBeat()
{
	if (m_pSpi)
	{
		m_pSpi->OnHeartBeat();
	}

}

void CXcpFtdcMdUserApiImpl::onHeartBeatWarning()
{}


///
int CXcpFtdcMdUserApiImpl::send(FTD::Package& package, bool checkLogged)
{
	//if (package.isNoneMode())
	//	package.formatFtdcMessages();

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
int CXcpFtdcMdUserApiImpl::ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequestID)
{
	FTD::ReqUserLogin package;
	package.clear();
	memcpy(&package.reqUserLoginField, pReqUserLogin, sizeof(CXcpFtdcReqUserLoginField));
	package.reqUserLoginField.TargetFrontTag = FTDC_FT_Md;
	return send(package, false);
}

///用户登出请求
int CXcpFtdcMdUserApiImpl::ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID)
{
	FTD::ReqUserLogout package;
	package.clear();
	memcpy(&package.reqUserLogoutField, pReqUserLogout, sizeof(CXcpFtdcReqUserLoginField));
	return send(package, false);
}

///报单请求
int CXcpFtdcMdUserApiImpl::ReqSubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID)
{
	FTD::ReqSubMarketData package;
	package.clear();
	//memcpy(&package.inputOrderField, pInputOrder, sizeof(CXcpFtdcInputOrderField));
	FTD::CFtdcInstrumentField ftdc_instrument = { 0 };
	for (int i = 0; i < size; i++)
	{
		memcpy(&ftdc_instrument, instrument + i, sizeof(CXcpFtdcInstrumentField));
		package.instrumentFields.push_back(ftdc_instrument);
	}
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}

///报单操作请求
int CXcpFtdcMdUserApiImpl::ReqUnsubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID)
{
	FTD::ReqUnsubMarketData package;
	package.clear();
	//memcpy(&package.inputOrderField, pInputOrder, sizeof(CXcpFtdcInputOrderField));
	FTD::CFtdcInstrumentField ftdc_instrument = { 0 };
	for (int i = 0; i < size; i++)
	{
		memcpy(&ftdc_instrument, instrument + i, sizeof(CXcpFtdcInstrumentField));
		package.instrumentFields.push_back(ftdc_instrument);
	}
	package.requestSourceField.RequestID = nRequestID;
	return send(package);
}



void CXcpFtdcMdUserApiImpl::OnPackage(const FTD::RspUserLogin& package, const FTD::SessionID& id)
{
	if (package.pErrorField && package.pErrorField->ErrorCode != 0)
	{
		OnRspUserLoginFailure(package, id);
	}
	else
	{
		OnRspUserLoginSucceed(package, id);
	}

	if (m_pSpi)
	{
		CXcpFtdcRspUserLoginField contentField = { 0 };
		CXcpFtdcErrorField errorField = { 0 };
		memcpy(&contentField, &package.rspUserLoginField, sizeof(CXcpFtdcRspUserLoginField));
		if (package.pErrorField.get())
			memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
		m_pSpi->OnRspUserLogin(&contentField, &errorField, package.rspUserLoginField.RequestID, true);
	}
	
}

void CXcpFtdcMdUserApiImpl::OnRspUserLoginFailure(const FTD::RspUserLogin& package, const FTD::SessionID& id)
{
	if (m_pInitiator)
	{
		m_pInitiator->stopDoConnect();
	}
}

void CXcpFtdcMdUserApiImpl::OnRspUserLoginSucceed(const FTD::RspUserLogin& package, const FTD::SessionID& id)
{
}


void CXcpFtdcMdUserApiImpl::OnPackage(const FTD::ForceExit& package, const FTD::SessionID& id)
{
}

void CXcpFtdcMdUserApiImpl::OnPackage(const FTD::RspUserLogout& package, const FTD::SessionID& id)
{
	if (!m_pSpi)
		return;
	CXcpFtdcRspUserLogoutField contentField;
	CXcpFtdcErrorField errorField = { 0 };
	memcpy(&contentField, &package.rspUserLogoutField, sizeof(CXcpFtdcRspUserLogoutField));
	if (package.pErrorField.get())
		memcpy(&errorField, package.pErrorField.get(), sizeof(CXcpFtdcErrorField));
	m_pSpi->OnRspUserLogout(&contentField, &errorField, package.rspUserLogoutField.RequestID, true);
}

void CXcpFtdcMdUserApiImpl::OnPackage(const FTD::RspSubMarketData& package, const FTD::SessionID& id)
{
	if (!m_pSpi)
		return;
	if (package.instrumentFields.size() != package.errorFields.size())
		return;
	int size = package.instrumentFields.size();
	if (size == 0)
		return;
	for (int i = 0; i < size - 1; i++)
	{
		m_pSpi->OnRspSubMarketData((CXcpFtdcInstrumentField*)&package.instrumentFields[i],
			(CXcpFtdcErrorField*)&package.errorFields[i],
			package.requestSourceField.RequestID,
			false);
	}
	m_pSpi->OnRspSubMarketData((CXcpFtdcInstrumentField*)&package.instrumentFields[size - 1],
		(CXcpFtdcErrorField*)&package.errorFields[size - 1],
		package.requestSourceField.RequestID,
		true);
}

void CXcpFtdcMdUserApiImpl::OnPackage(const FTD::RspUnsubMarketData& package, const FTD::SessionID& id)
{
	if (!m_pSpi)
		return;
	if (package.instrumentFields.size() != package.errorFields.size())
		return;
	int size = package.instrumentFields.size();
	if (size == 0)
		return;
	for (int i = 0; i < size - 1; i++)
	{
		m_pSpi->OnRspSubMarketData((CXcpFtdcInstrumentField*)&package.instrumentFields[i],
			(CXcpFtdcErrorField*)&package.errorFields[i],
			package.requestSourceField.RequestID,
			false);
	}
	m_pSpi->OnRspUnsubMarketData((CXcpFtdcInstrumentField*)&package.instrumentFields[size - 1],
		(CXcpFtdcErrorField*)&package.errorFields[size - 1],
		package.requestSourceField.RequestID,
		true);
}

void CXcpFtdcMdUserApiImpl::OnPackage(const FTD::IncMarketData& package, const FTD::SessionID& id)
{
	if (!m_pSpi)
		return;
	m_pSpi->OnMaketData((CXcpFtdcMarketDataField*)&package.marketDataField);	
}
