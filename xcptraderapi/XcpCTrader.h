#ifndef XCP_C_TRADER_H
#define XCP_C_TRADER_H

#include "XcpFtdcTraderApi.h"
#include <string>
#include <set>


#define C_API_ERROR_NULL_TRADER_API -1000
class CXcpTrader : public CXcpFtdcTraderSpi
{
public:
	CXcpTrader(const char* psw);
	~CXcpTrader();

	const char* GetApiVersion();
	const char* GetTradingDay();
	void RegisterFront(const char* frontAddress);
	void SubscribePrivateTopic(THOST_TE_RESUME_TYPE resumeType);
	void SubscribePublicTopic(THOST_TE_RESUME_TYPE resumeType);
	void Init();
	void Release();

	int ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequestID);
	int ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID);
	int ReqOrderInsert(CXcpFtdcInputOrderField* pInputOrder, int nRequestID) ;
	int ReqOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID) ;
	int ReqFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID) ;
	int ReqQryOrder(CXcpFtdcQryOrderField* pQryOrder, int nRquestID) ;
	int ReqQryTrade(CXcpFtdcQryTradeField* pQryTrade, int nRequestID) ;
	int ReqQryFund(CXcpFtdcQryFundField *pQryFund, int nRequestID) ;
	int ReqQryPosition(CXcpFtdcQryPositionField *pQryPosition, int nRequestID) ;
	int ReqQryFundTransfer(CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID) ;
	int ReqQryHisOrder(CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID) ;
	int ReqQryHisTrade(CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID) ;
	int ReqQryHisFundTransfer(CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID) ;
	int ReqQryInstrument(CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID) ;
	int ReqQryETF(CXcpFtdcQryETFField *pQryETF, int nRequestID) ;
	int ReqQryETFComposition(CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID) ;
	int ReqQryStructuredFund(CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID) ;
	int ReqQryPurchasableNewIssueSecurity(CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID) ;
	int ReqQryPurchaseQuota(CXcpFtdcQryPurchaseQuotaField *pQryPurchaseQuota, int nRequestID) ;

	//spi
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnHeartBeatWarning(int nTimeLapse);
	virtual void OnHeartBeat();
	virtual void OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspInputOrder(CXcpFtdcInputOrderField* pInputOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspInputOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspInputFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryFund(CXcpFtdcFundField* pFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryPosition(CXcpFtdcPositionField* pPosition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryOrder(CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryTrade(CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryFundTransfer(CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryHisOrder(CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryHisTrade(CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryHisFundTransfer(CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryInstrument(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryStructuredFund(CXcpFtdcStructuredFundField* pStructuredFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryETF(CXcpFtdcETFField* pETF, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryETFComposition(CXcpFtdcETFCompositionField* pETFComposition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryPurchasableNewIssueSecurity(CXcpFtdcNewIssueSecurityField* pPurchasableNewIssueSecurity, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspQryPurchaseQuota(CXcpFtdcPurchaseQuotaField* pPurchaseQuota, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRtnOrderExecutionReport(CXcpFtdcExecutionReportField* pExecutionReport);

	//register callback function pointers;
	void registerFP_OnFrontConnected(FuncPtrOnFrontConnected fp);
	void registerFP_OnFrontDisconnected(FuncPtrOnFrontDisconnected fp);
	void registerFP_OnHeartBeatWarning(FuncPtrOnHeartBeatWarning fp);
	void registerFP_OnHeartBeat(FuncPtrOnHeartBeat fp);
	void registerFP_OnRspUserLogin(FuncPtrOnRspUserLogin fp);
	void registerFP_OnRspUserLogout(FuncPtrOnRspUserLogout fp);
	void registerFP_OnRspInputOrder(FuncPtrOnRspInputOrder fp);
	void registerFP_OnRspInputOrderAction(FuncPtrOnRspInputOrderAction fp);
	void registerFP_OnRspInputFundTransfer(FuncPtrOnRspInputFundTransfer fp);
	void registerFP_OnRspQryFund(FuncPtrOnRspQryFund fp);
	void registerFP_OnRspQryPosition(FuncPtrOnRspQryPosition fp);
	void registerFP_OnRspQryOrder(FuncPtrOnRspQryOrder fp);
	void registerFP_OnRspQryTrade(FuncPtrOnRspQryTrade fp);
	void registerFP_OnRspQryFundTransfer(FuncPtrOnRspQryFundTransfer fp);
	void registerFP_OnRspQryHisOrder(FuncPtrOnRspQryHisOrder fp);
	void registerFP_OnRspQryHisTrade(FuncPtrOnRspQryHisTrade fp);
	void registerFP_OnRspQryHisFundTransfer(FuncPtrOnRspQryHisFundTransfer fp);
	void registerFP_OnRspQryInstrument(FuncPtrOnRspQryInstrument fp);
	void registerFP_OnRspQryStructuredFund(FuncPtrOnRspQryStructuredFund   fp);
	void registerFP_OnRspQryETF(FuncPtrOnRspQryETF fp);
	void registerFP_OnRspQryETFComposition(FuncPtrOnRspQryETFComposition fp);
	void registerFP_OnRspQryPurchasableNewIssueSecurity(FuncPtrOnRspQryPurchasableNewIssueSecurity fp);
	void registerFP_OnRspQryPurchaseQuota(FuncPtrOnRspQryPurchaseQuota  fp);
	void registerFP_OnRtnOrderExecutionReport(FuncPtrOnRtnOrderExecutionReport fp);
private:
	CXcpFtdcTraderApi* m_pApi;
	//std::string m_flowPath;
	//std::set<std::string> m_frontAddresses;
	//int m_privateResumeType;
	//int m_publicResumeType;
	CXcpFtdcReqUserLoginField m_userField;
	//int m_nextReqID;

	bool m_onFrontConnectedRegistered;
	bool m_onFrontDisconnectedRegistered;
	bool m_onHeartBeatWarningRegistered;
	bool m_onHeartBeatRegistered;
	bool m_onRspUserLoginRegistered;
	bool m_onRspUserLogoutRegistered;
	bool m_onRspInputOrderRegistered;
	bool m_onRspInputOrderActionRegistered;
	bool m_onRspInputFundTransferRegistered;
	bool m_onRspQryFundRegistered;
	bool m_onRspQryPositionRegistered;
	bool m_onRspQryOrderRegistered;
	bool m_onRspQryTradeRegistered;
	bool m_onRspQryFundTransferRegistered;
	bool m_onRspQryHisOrderRegistered;
	bool m_onRspQryHisTradeRegistered;
	bool m_onRspQryHisFundTransferRegistered;
	bool m_onRspQryInstrumentRegistered;
	bool m_onRspQryStructuredFundRegistered;
	bool m_onRspQryETFRegistered;
	bool m_onRspQryETFCompositionRegistered;
	bool m_onRspQryPurchasableNewIssueSecurityRegistered;
	bool m_onRspQryPurchaseQuotaRegistered;
	bool m_onRtnOrderExecutionReportRegistered;

	FuncPtrOnFrontConnected   m_fpOnFrontConnected;
	FuncPtrOnFrontDisconnected m_fpOnFrontDisconnected;
	FuncPtrOnHeartBeatWarning m_fpOnHeartBeatWarning;
	FuncPtrOnHeartBeat m_fpOnHeartBeat;
	FuncPtrOnRspUserLogin  m_fpOnRspUserLogin;
	FuncPtrOnRspUserLogout  m_fpOnRspUserLogout;
	FuncPtrOnRspInputOrder  m_fpOnRspInputOrder;
	FuncPtrOnRspInputOrderAction  m_fpOnRspInputOrderAction;
	FuncPtrOnRspInputFundTransfer m_fpOnRspInputFundTransfer;
	FuncPtrOnRspQryFund  m_fpOnRspQryFund;
	FuncPtrOnRspQryPosition m_fpOnRspQryPosition;
	FuncPtrOnRspQryOrder m_fpOnRspQryOrder;
	FuncPtrOnRspQryTrade m_fpOnRspQryTrade;
	FuncPtrOnRspQryFundTransfer m_fpOnRspQryFundTransfer;
	FuncPtrOnRspQryHisOrder m_fpOnRspQryHisOrder;
	FuncPtrOnRspQryHisTrade m_fpOnRspQryHisTrade;
	FuncPtrOnRspQryHisFundTransfer m_fpOnRspQryHisFundTransfer;
	FuncPtrOnRspQryInstrument m_fpOnRspQryInstrument;
	FuncPtrOnRspQryStructuredFund m_fpOnRspQryStructuredFund;
	FuncPtrOnRspQryETF m_fpOnRspQryETF;
	FuncPtrOnRspQryETFComposition m_fpOnRspQryETFComposition;
	FuncPtrOnRspQryPurchasableNewIssueSecurity m_fpOnRspQryPurchasableNewIssueSecurity;
	FuncPtrOnRspQryPurchaseQuota m_fpOnRspQryPurchaseQuota;
	FuncPtrOnRtnOrderExecutionReport m_fpOnRtnOrderExecutionReport;
};
#endif
