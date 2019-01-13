#include "XcpCTrader.h"

CXcpTrader::CXcpTrader()
{
	m_pApi = 0;
	m_flowPath = "";
	m_privateResumeType = -1;
	m_publicResumeType = -1;

	//spi init
	m_onFrontConnectedRegistered = false;
	m_onFrontDisconnectedRegistered = false;
	m_onHeartBeatWarningRegistered = false;
	m_onRspUserLoginRegistered = false;
	m_onRspUserLogoutRegistered = false;
	m_onRspInputOrderRegistered = false;
	m_onRspInputOrderActionRegistered = false;
	m_onRspInputFundTransferRegistered = false;
	m_onRspQryFundRegistered = false;
	m_onRspQryPositionRegistered = false;
	m_onRspQryOrderRegistered = false;
	m_onRspQryTradeRegistered = false;
	m_onRspQryFundTransferRegistered = false;
	m_onRspQryHisOrderRegistered = false;
	m_onRspQryHisTradeRegistered = false;
	m_onRspQryHisFundTransferRegistered = false;
	m_onRspQryInstrumentRegistered = false;
	m_onRspQryStructuredFundRegistered = false;
	m_onRspQryETFRegistered = false;
	m_onRspQryETFCompositionRegistered = false;
	m_onRspQryPurchasableNewIssueSecurityRegistered = false;
	m_onRspQryPurchaseQuotaRegistered = false;
	m_onRtnOrderExecutionReportRegistered = false;
}

//TODO
CXcpTrader::~CXcpTrader()
{}

void CXcpTrader::RegisterFlowPath(const char* path)
{
	m_flowPath = path;
}

void CXcpTrader::RegisterFront(const char* frontAddress)
{
	m_frontAddresses.insert(frontAddress);
}

void CXcpTrader::SubscribePrivateTopic(THOST_TE_RESUME_TYPE resumeType)
{
	m_privateResumeType = resumeType;
}

void CXcpTrader::SubscribePublicTopic(THOST_TE_RESUME_TYPE resumeType)
{
	m_publicResumeType = resumeType;
}

void CXcpTrader::AttachUserLoginField(const CXcpFtdcReqUserLoginField* field)
{
	memcpy(&m_userField, field, sizeof(CXcpFtdcReqUserLoginField));
}

void CXcpTrader::Connect()
{
	m_pApi = CXcpFtdcTraderApi::CreateFtdcTraderApi(m_flowPath.data());
	m_pApi->RegisterSpi(this);
	for (auto it = m_frontAddresses.begin(); it != m_frontAddresses.end(); it++)
	{
		m_pApi->RegisterFront((*it).data());
	}
	if (m_privateResumeType > 0)
		m_pApi->SubscribePrivateTopic((THOST_TE_RESUME_TYPE)m_privateResumeType);
	if (m_publicResumeType > 0)
		m_pApi->SubscribePublicTopic((THOST_TE_RESUME_TYPE)m_publicResumeType);
	m_pApi->Init();
}

//TODO
void CXcpTrader::Disconnect()
{
	if (m_pApi)
	{
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;
	}
}

//api export
int CXcpTrader::ReqOrderInsert(CXcpFtdcInputOrderField* pInputOrder, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqOrderInsert(pInputOrder, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqOrderAction(pInputOrderAction, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID)

{
	if (m_pApi)
		return m_pApi->ReqFundTransfer(pInputFundTransfer, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryOrder(CXcpFtdcQryOrderField* pQryOrder, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryOrder(pQryOrder, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryTrade(CXcpFtdcQryTradeField* pQryTrade, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryTrade(pQryTrade, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryFund(CXcpFtdcQryFundField *pQryFund, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryFund(pQryFund, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryPosition(CXcpFtdcQryPositionField *pQryPosition, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryPosition(pQryPosition, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryFundTransfer(CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryFundTransfer(pQryFundTransfer, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryHisOrder(CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryHisOrder(pQryHisOrder, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryHisTrade(CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryHisTrade(pQryHisTrade, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryHisFundTransfer(CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryHisFundTransfer(pQryHisFundTransfer, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryInstrument(CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryInstrument(pQryInstrument, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryETF(CXcpFtdcQryETFField *pQryETF, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryETF(pQryETF, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryETFComposition(CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryETFComposition(pQryEtfComposition, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryStructuredFund(CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryStructuredFund(pQryStructuredFund, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryPurchasableNewIssueSecurity(CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryPurchasableNewIssueSecurity(pQryPurchasableNewSecurity, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}

int CXcpTrader::ReqQryPurchaseQuota(CXcpFtdcQryPurchaseQuotaField *pQryPurchaseQuota, int nRequestID)
{
	if (m_pApi)
		return m_pApi->ReqQryPurchaseQuota(pQryPurchaseQuota, nRequestID);
	else
		return C_API_ERROR_NULL_TRADER_API;
}



void CXcpTrader::OnFrontConnected() 
{
	if (m_onFrontConnectedRegistered)
		m_fpOnFrontConnected();
	//Automatic login!!!
	m_nextReqID = 1;
	m_pApi->ReqUserLogin(&m_userField, m_nextReqID++);
}

void CXcpTrader::OnFrontDisconnected(int nReason)
{
	if (m_onFrontDisconnectedRegistered)
		m_fpOnFrontDisconnected(nReason);
}

void CXcpTrader::OnHeartBeatWarning(int nTimeLapse) 
{
	if (m_onHeartBeatWarningRegistered)
		m_fpOnHeartBeatWarning(nTimeLapse);
}

void CXcpTrader::OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspUserLoginRegistered)
		m_fpOnRspUserLogin(pRspUserLogin, pRspInfo, nRequestID, isLast);
}

void CXcpTrader::OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspUserLogoutRegistered)
		m_fpOnRspUserLogout(pRspUserLogout, pRspInfo, nRequestID, isLast);
}

void CXcpTrader::OnRspInputOrder(CXcpFtdcInputOrderField* pInputOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspInputOrderRegistered)
		m_fpOnRspInputOrder(pInputOrder, pRspInfo, nRequestID, isLast);
}

void CXcpTrader::OnRspInputOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspInputOrderActionRegistered)
		m_fpOnRspInputOrderAction(pInputOrderAction, pRspInfo, nRequestID, isLast);
}

void CXcpTrader::OnRspInputFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspInputFundTransferRegistered)
	{
		m_fpOnRspInputFundTransfer( pInputFundTransfer,  pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryFund(CXcpFtdcFundField* pFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryFundRegistered)
	{
		m_fpOnRspQryFund(pFund, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryPosition(CXcpFtdcPositionField* pPosition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryPositionRegistered)
	{
		m_fpOnRspQryPosition( pPosition,  pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryOrder(CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryOrderRegistered)
	{
		m_fpOnRspQryOrder(pOrder, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryTrade(CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryTradeRegistered)
	{
		m_fpOnRspQryTrade(pTrade, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryFundTransfer(CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryFundTransferRegistered)
	{
		m_fpOnRspQryFundTransfer(pFundTransfer, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryHisOrder(CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryHisOrderRegistered)
	{
		m_fpOnRspQryHisOrder(pOrder, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryHisTrade(CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryHisTradeRegistered)
	{
		m_fpOnRspQryHisTrade(pTrade, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryHisFundTransfer(CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryHisFundTransferRegistered)
	{
		m_fpOnRspQryHisFundTransfer(pFundTransfer, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryInstrument(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryInstrumentRegistered)
	{
		m_fpOnRspQryInstrument(pInstrument, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryStructuredFund(CXcpFtdcStructuredFundField* pStructuredFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryStructuredFundRegistered)
	{
		m_fpOnRspQryStructuredFund(pStructuredFund, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryETF(CXcpFtdcETFField* pETF, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryETFRegistered)
	{
		m_fpOnRspQryETF(pETF,  pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryETFComposition(CXcpFtdcETFCompositionField* pETFComposition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryETFCompositionRegistered)
	{
		m_fpOnRspQryETFComposition(pETFComposition, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryPurchasableNewIssueSecurity(CXcpFtdcNewIssueSecurityField* pPurchasableNewIssueSecurity, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryPurchasableNewIssueSecurityRegistered)
	{
		m_fpOnRspQryPurchasableNewIssueSecurity(pPurchasableNewIssueSecurity, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRspQryPurchaseQuota(CXcpFtdcPurchaseQuotaField* pPurchaseQuota, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (m_onRspQryPurchaseQuotaRegistered)
	{
		m_fpOnRspQryPurchaseQuota(pPurchaseQuota, pRspInfo, nRequestID, isLast);
	}
}

void CXcpTrader::OnRtnOrderExecutionReport(CXcpFtdcExecutionReportField* pExecutionReport)
{
	if (m_onRtnOrderExecutionReportRegistered)
	{
		m_fpOnRtnOrderExecutionReport(pExecutionReport);
	}
}

//register callback function pointers{}
void CXcpTrader::registerFP_OnFrontConnected(FuncPtrOnFrontConnected fp)
{
	m_fpOnFrontConnected = fp;
	m_onFrontConnectedRegistered = true;
}

void CXcpTrader::registerFP_OnFrontDisconnected(FuncPtrOnFrontDisconnected fp)
{
	m_fpOnFrontDisconnected = fp;
	m_onFrontDisconnectedRegistered = true;
}

void CXcpTrader::registerFP_OnHeartBeatWarning(FuncPtrOnHeartBeatWarning fp)
{
	m_fpOnHeartBeatWarning = fp;
	m_onHeartBeatWarningRegistered = true;
}

void CXcpTrader::registerFP_OnRspUserLogin(FuncPtrOnRspUserLogin fp)
{
	m_fpOnRspUserLogin = fp;
	m_onRspUserLoginRegistered = true;
}

void CXcpTrader::registerFP_OnRspUserLogout(FuncPtrOnRspUserLogout fp)
{
	m_fpOnRspUserLogout = fp;
	m_onRspUserLogoutRegistered = true;
}

void CXcpTrader::registerFP_OnRspInputOrder(FuncPtrOnRspInputOrder fp)
{
	m_fpOnRspInputOrder = fp;
	m_onRspInputOrderRegistered = true;
}

void CXcpTrader::registerFP_OnRspInputOrderAction(FuncPtrOnRspInputOrderAction fp)
{
	m_fpOnRspInputOrderAction = fp;
	m_onRspInputOrderActionRegistered = true;
}

void CXcpTrader::registerFP_OnRspInputFundTransfer(FuncPtrOnRspInputFundTransfer fp)
{
	m_fpOnRspInputFundTransfer = fp;
	m_onRspInputFundTransferRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryFund(FuncPtrOnRspQryFund fp)
{
	m_fpOnRspQryFund = fp;
	m_onRspQryFundRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryPosition(FuncPtrOnRspQryPosition fp)
{
	m_fpOnRspQryPosition = fp;
	m_onRspQryPositionRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryOrder(FuncPtrOnRspQryOrder fp)
{
	m_fpOnRspQryOrder = fp;
	m_onRspQryOrderRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryTrade(FuncPtrOnRspQryTrade fp)
{
	m_fpOnRspQryTrade = fp;
	m_onRspQryTradeRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryFundTransfer(FuncPtrOnRspQryFundTransfer fp)
{
	m_fpOnRspQryFundTransfer = fp;
	m_onRspQryFundTransferRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryHisOrder(FuncPtrOnRspQryHisOrder fp)
{
	m_fpOnRspQryHisOrder = fp;
	m_onRspQryHisOrderRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryHisTrade(FuncPtrOnRspQryHisTrade fp)
{
	m_fpOnRspQryHisTrade = fp;
	m_onRspQryHisTradeRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryHisFundTransfer(FuncPtrOnRspQryHisFundTransfer fp)
{
	m_fpOnRspQryHisFundTransfer = fp;
	m_onRspQryHisFundTransferRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryInstrument(FuncPtrOnRspQryInstrument fp)
{
	m_fpOnRspQryInstrument = fp;
	m_onRspQryInstrumentRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryStructuredFund(FuncPtrOnRspQryStructuredFund   fp)
{
	m_fpOnRspQryStructuredFund = fp;
	m_onRspQryStructuredFundRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryETF(FuncPtrOnRspQryETF fp)
{
	m_fpOnRspQryETF = fp;
	m_onRspQryETFRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryETFComposition(FuncPtrOnRspQryETFComposition fp)
{
	m_fpOnRspQryETFComposition = fp;
	m_onRspQryETFCompositionRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryPurchasableNewIssueSecurity(FuncPtrOnRspQryPurchasableNewIssueSecurity fp)
{
	m_fpOnRspQryPurchasableNewIssueSecurity = fp;
	m_onRspQryPurchasableNewIssueSecurityRegistered = true;
}

void CXcpTrader::registerFP_OnRspQryPurchaseQuota(FuncPtrOnRspQryPurchaseQuota  fp)
{
	m_fpOnRspQryPurchaseQuota = fp;
	m_onRspQryPurchaseQuotaRegistered = true;
}

void CXcpTrader::registerFP_OnRtnOrderExecutionReport(FuncPtrOnRtnOrderExecutionReport fp)
{
	m_fpOnRtnOrderExecutionReport = fp;
	m_onRtnOrderExecutionReportRegistered = true;
}
