#ifndef XCP_C_TRADER_H
#define XCP_C_TRADER_H

#include "XcpFtdcMdUserApi.h"
#include <string>
#include <set>


#define C_API_ERROR_NULL_TRADER_API -1000
class CXcpMdUser : public CXcpFtdcMdUserSpi
{
public:
	CXcpMdUser(const char* psw);
	~CXcpMdUser();

	const char* GetApiVersion();
	const char* GetTradingDay();
	void RegisterFront(const char* frontAddress);
	void Init();
	void Release();

	int ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequestID);
	int ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID);
	int ReqSubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID);
	int ReqUnsubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID);
	//spi
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnHeartBeatWarning(int nTimeLapse);
	virtual void OnHeartBeat();
	virtual void OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	virtual void OnRtnMarketData (CXcpFtdcMarketDataField* pMarketData);
	virtual void OnRspSubMarketData (CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int RequestID, bool isLast);
	virtual void OnRspUnsubMarketData (CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int RequestID, bool isLast);

	//register callback function pointers;
	void registerFP_OnFrontConnected(FuncPtrOnFrontConnected fp);
	void registerFP_OnFrontDisconnected(FuncPtrOnFrontDisconnected fp);
	void registerFP_OnHeartBeatWarning(FuncPtrOnHeartBeatWarning fp);
	void registerFP_OnHeartBeat(FuncPtrOnHeartBeat fp);
	void registerFP_OnRspUserLogin(FuncPtrOnRspUserLogin fp);
	void registerFP_OnRspUserLogout(FuncPtrOnRspUserLogout fp);
	void registerFP_OnRtnMarketData(FuncPtrOnRtnMarketData fp);
	void registerFP_OnRspSubMarketData(FuncPtrOnRspSubMarketData fp);
	void registerFP_OnRspUnsubMarketData(FuncPtrOnRspUnsubMarketData fp);

private:
	CXcpFtdcMdUserApi* m_pApi;
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
	bool m_onRtnMarketDataRegistered;
	bool m_onRspSubMarketDataRegistered;
	bool m_onRspUnsubMarketDataRegistered;

	FuncPtrOnFrontConnected   m_fpOnFrontConnected;
	FuncPtrOnFrontDisconnected m_fpOnFrontDisconnected;
	FuncPtrOnHeartBeatWarning m_fpOnHeartBeatWarning;
	FuncPtrOnHeartBeat m_fpOnHeartBeat;
	FuncPtrOnRspUserLogin  m_fpOnRspUserLogin;
	FuncPtrOnRspUserLogout  m_fpOnRspUserLogout;
	FuncPtrOnRtnMarketData  m_fpOnRtnMarketData;
	FuncPtrOnRspSubMarketData  m_fpOnRspSubMarketData;
	FuncPtrOnRspUnsubMarketData m_fpOnRspUnsubMarketData;
};
#endif
