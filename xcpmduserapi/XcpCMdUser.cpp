#include "XcpCMdUser.h"

CXcpMdUser::CXcpMdUser(const char* psw)
	:m_pApi(CXcpFtdcMdUserApi::CreateFtdcMdUserApi(psw))
{
	
	m_pApi->RegisterSpi(this);
	//spi init
	m_onFrontConnectedRegistered = false;
	m_onFrontDisconnectedRegistered = false;
	m_onHeartBeatWarningRegistered = false;
	m_onRspUserLoginRegistered = false;
	m_onRspUserLogoutRegistered = false;
	m_onRtnMarketDataRegistered = false;
	m_onRspSubMarketDataRegistered = false;
	m_onRspUnsubMarketDataRegistered = false;
}

//TODO
CXcpMdUser::~CXcpMdUser()
{
	if (m_pApi)
		m_pApi->Release();
}

const char* CXcpMdUser::GetApiVersion()
{
	if (m_pApi)
		return m_pApi->GetApiVersion();
	else
		return "";
}

const char* CXcpMdUser::GetTradingDay()
{
	if (m_pApi)
		return m_pApi->GetTradingDay();
	else
		return "";
}

void CXcpMdUser::RegisterFront(const char* frontAddress)
{
	m_pApi->RegisterFront(frontAddress);
}




void CXcpMdUser::Init()
{
	m_pApi->Init();
}

//TODO
void CXcpMdUser::Release()
{
	if (m_pApi)
	{
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;
	}
}

//api export
int CXcpMdUser::ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqUserLogin(pReqUserLogin, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpMdUser::ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqUserLogout(pReqUserLogout, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpMdUser::ReqSubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqSubMarketData(instrument, size, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpMdUser::ReqUnsubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID)
{
	{
		if (m_pApi)
			return m_pApi->ReqUnsubMarketData(instrument, size, nRequestID);
		else
			return C_API_ERROR_NULL_TRADER_API;
	}
}

void CXcpMdUser::OnFrontConnected()
{
	if (m_onFrontConnectedRegistered)
		m_fpOnFrontConnected();
}

void CXcpMdUser::OnFrontDisconnected(int nReason)
{
	if (m_onFrontDisconnectedRegistered)
		m_fpOnFrontDisconnected(nReason);
}

void CXcpMdUser::OnHeartBeatWarning(int nTimeLapse)
{
	if (m_onHeartBeatWarningRegistered)
		m_fpOnHeartBeatWarning(nTimeLapse);
}
void CXcpMdUser::OnHeartBeat()
{
	if (m_onHeartBeatRegistered)
		m_fpOnHeartBeat();
}

void CXcpMdUser::OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspUserLoginRegistered)
		m_fpOnRspUserLogin(pRspUserLogin, pRspInfo, nRequestID, isLast);
}

void CXcpMdUser::OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspUserLogoutRegistered)
		m_fpOnRspUserLogout(pRspUserLogout, pRspInfo, nRequestID, isLast);
}

void CXcpMdUser::OnRtnMarketData(CXcpFtdcMarketDataField* pMarketData)
{
	if (m_onRtnMarketDataRegistered)
		m_fpOnRtnMarketData(pMarketData);
}

void CXcpMdUser::OnRspSubMarketData(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int RequestID, bool isLast)
{
	if (m_onRspSubMarketDataRegistered)
		m_fpOnRspSubMarketData(pInstrument, pRspInfo, RequestID, isLast);
}

void CXcpMdUser::OnRspUnsubMarketData(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int RequestID, bool isLast)
{
	if (m_onRspUnsubMarketDataRegistered)
		m_fpOnRspUnsubMarketData(pInstrument, pRspInfo, RequestID, isLast);
}


//register callback function pointers{}
void CXcpMdUser::registerFP_OnFrontConnected(FuncPtrOnFrontConnected fp)
{
	m_fpOnFrontConnected = fp;
	m_onFrontConnectedRegistered = true;
}

void CXcpMdUser::registerFP_OnFrontDisconnected(FuncPtrOnFrontDisconnected fp)
{
	m_fpOnFrontDisconnected = fp;
	m_onFrontDisconnectedRegistered = true;
}

void CXcpMdUser::registerFP_OnHeartBeatWarning(FuncPtrOnHeartBeatWarning fp)
{
	m_fpOnHeartBeatWarning = fp;
	m_onHeartBeatWarningRegistered = true;
}

void CXcpMdUser::registerFP_OnHeartBeat(FuncPtrOnHeartBeat fp)
{
	m_fpOnHeartBeat = fp;
	m_onHeartBeatRegistered = true;
}

void CXcpMdUser::registerFP_OnRspUserLogin(FuncPtrOnRspUserLogin fp)
{
	m_fpOnRspUserLogin = fp;
	m_onRspUserLoginRegistered = true;
}

void CXcpMdUser::registerFP_OnRspUserLogout(FuncPtrOnRspUserLogout fp)
{
	m_fpOnRspUserLogout = fp;
	m_onRspUserLogoutRegistered = true;
}

void CXcpMdUser::registerFP_OnRtnMarketData(FuncPtrOnRtnMarketData fp)
{
	m_fpOnRtnMarketData = fp;
	m_onRtnMarketDataRegistered = true;
}

void CXcpMdUser::registerFP_OnRspSubMarketData(FuncPtrOnRspSubMarketData fp)
{
	m_fpOnRspSubMarketData = fp;
	m_onRspSubMarketDataRegistered = true;
}

void CXcpMdUser::registerFP_OnRspUnsubMarketData(FuncPtrOnRspUnsubMarketData fp)
{
	m_fpOnRspUnsubMarketData = fp;
	m_onRspUnsubMarketDataRegistered = true;
}

