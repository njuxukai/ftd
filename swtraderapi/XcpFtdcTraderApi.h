#ifndef XCP_FTDC_TRADER_API_H
#define XCP_FTDC_TRADER_API_H

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_TRADER_API_EXPORT
#define TRADER_API_EXPORT __declspec(dllexport)
#else
#define TRADER_API_EXPORT __declspec(dllimport)
#endif
#else
#define TRADER_API_EXPORT 
#endif

#include "XcpFtdcUserApiStruct.h"
class CXcpFtdcTraderSpi
{
	virtual void OnFrontConnected() {}

	virtual void OnFrontDisconnected(int nReason) {}

	virtual void OnHeartBeatWarning(int nTimeLapse) {}

	virtual void OnRspUserLogin() {}

	virtual void OnRspUserLogout() {}
};

class TRADER_API_EXPORT CXcpFtdcTraderApi
{
public:
	static CXcpFtdcTraderApi *CreateFtdcTraderApi(const char*pszFlowPath = "");

	virtual const char* GetApiVersion() = 0;

	virtual void Release() = 0;

	virtual void Init() = 0;

	virtual void Join() = 0;

	virtual const char* GetTradingDay() = 0;


	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CXcpFtdcTraderSpi *pSpi) = 0;

	virtual void RegisterFront(const char* frontAddr) = 0;

	virtual void SubscribePrivateTopic() = 0;


	virtual void SubscribePublicTopic() = 0;
	
protected:
	~CXcpFtdcTraderApi() {}
};
#endif