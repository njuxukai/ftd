#ifndef SW_TRADER_API
#define SW_TRADER_API

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
class CSwhyFtdcTraderSpi
{
	virtual void OnFrontConnected() {}

	virtual void OnFrontDisconnected(int nReason) {}

	virtual void OnHeartBeatWarning(int nTimeLapse) {}

	virtual void OnRspUserLogin() {}

	virtual void OnRspUserLogout() {}
};

class TRADER_API_EXPORT CSwhyFtdcTraderApi
{
public:
	static CSwhyFtdcTraderApi *CreateFtdcTraderApi(const char*pszFlowPath = "");

	virtual const char* GetApiVersion() = 0;

	virtual void Release() = 0;

	virtual void Init() = 0;

	virtual void Join() = 0;

	virtual const char* GetTradingDay() = 0;


	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CSwhyFtdcTraderSpi *pSpi) = 0;

	virtual void RegisterFront(const char* frontAddr) = 0;

	virtual void SubscribePrivateTopic() = 0;


	virtual void SubscribePublicTopic() = 0;
	
protected:
	~CSwhyFtdcTraderApi() {}
};
#endif