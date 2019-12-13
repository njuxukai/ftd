#ifndef XCP_FTDC_TRADER_API_H
#define XCP_FTDC_TRADER_API_H

#if defined(WIN32)
#ifdef LIB_TRADER_API_EXPORT
#define TRADER_API_EXPORT __declspec(dllexport)
#else
#define TRADER_API_EXPORT __declspec(dllimport)
#endif
#else
#define TRADER_API_EXPORT 
#endif

#include "XcpFtdcUserApiStruct.h"

#ifdef __cplusplus
class CXcpFtdcMdUserSpi
{
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected() {};

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason) {}

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse) {}

	///
	virtual void OnHeartBeat() {}
	///客户登录请求响应
	virtual void OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///客户登出请求响应
	virtual void OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///报单委托请求响应
	virtual void OnMaketData(CXcpFtdcMarketDataField* pmd) {}

	///订阅行情请求响应
	virtual void OnRspSubMarketData(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///退订行情请求响应
	virtual void OnRspUnsubMarketData(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}
	
};

class TRADER_API_EXPORT CXcpFtdcMdUserApi
{
public:
	static CXcpFtdcMdUserApi *CreateFtdcMdUserApi(const char*pszFlowPath = "");

	virtual const char* GetApiVersion() = 0;

	virtual void Release() = 0;

	virtual void Init() = 0;

	virtual void Join() = 0;

	virtual const char* GetTradingDay() = 0;


	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CXcpFtdcMdUserSpi *pSpi) = 0;

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(const char* frontAddr) = 0;


	///用户登录请求
	virtual int ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequstID) = 0;

	///用户登出请求
	virtual int ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID) = 0;

	///订阅行情
	virtual int ReqSubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID) = 0;

	///退订行情
	virtual int ReqUnsubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID) = 0;

protected:
	~CXcpFtdcMdUserApi() {}
};
#endif

typedef void(__cdecl *FuncPtrOnFrontConnected) ();
typedef void(__cdecl *FuncPtrOnFrontDisconnected) (int nReason);
typedef void(__cdecl *FuncPtrOnHeartBeatWarning) (int nTimeLapse);
typedef void(__cdecl *FuncPtrOnHeartBeat) ();
typedef void(__cdecl *FuncPtrOnRspUserLogin) (CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspUserLogout) (CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRtnMarketData) (CXcpFtdcMarketDataField* pMarketData);
typedef void(__cdecl *FuncPtrOnRspSubMarketData) (CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int RequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspUnsubMarketData) (CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int RequestID, bool isLast);

#ifdef __cplusplus
extern "C"
{
#endif
	typedef void* MDUserApi;
	TRADER_API_EXPORT void create_mduser(MDUserApi* mduser, const char* psw="");
	TRADER_API_EXPORT void init_mduser(MDUserApi mduser);
	TRADER_API_EXPORT void release_mduser(MDUserApi* mduser);
	


	//交易函数 应在connect后,OnRspUserLogin回调收到成功登录后再调用，如未登录调用返回错误代码
	TRADER_API_EXPORT const char* get_api_version(MDUserApi trader);
	TRADER_API_EXPORT const char* get_trading_day(MDUserApi trader);
	TRADER_API_EXPORT int req_user_login(MDUserApi trader, CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequestID);
	TRADER_API_EXPORT int req_user_logout(MDUserApi trader, CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID);
	TRADER_API_EXPORT int req_sub_market_data(MDUserApi trader, CXcpFtdcInstrumentField* pInputOrder,int size,  int nRequestID);
	TRADER_API_EXPORT int req_unsub_market_data(MDUserApi trader, CXcpFtdcInstrumentField* pInstrumentID, int size,  int nRequestID);

	//配置函数 应在init_mduser前调用
	TRADER_API_EXPORT void register_front(MDUserApi trader, const char* front_address);

    //注册回调 应在init_mduser前调用（只需要注册需要的回调）
	TRADER_API_EXPORT void registerFP_OnFrontConnected(MDUserApi mduser, FuncPtrOnFrontConnected fp);
	TRADER_API_EXPORT void registerFP_OnFrontDisconnected(MDUserApi mduser, FuncPtrOnFrontDisconnected fp);
	TRADER_API_EXPORT void registerFP_OnHeartBeatWarning(MDUserApi mduser, FuncPtrOnHeartBeatWarning fp);
	TRADER_API_EXPORT void registerFP_OnHeartBeat(MDUserApi mduser, FuncPtrOnHeartBeat fp);
	TRADER_API_EXPORT void registerFP_OnRspUserLogin(MDUserApi mduser, FuncPtrOnRspUserLogin fp);
	TRADER_API_EXPORT void registerFP_OnRspUserLogout(MDUserApi mduser, FuncPtrOnRspUserLogout fp);
	TRADER_API_EXPORT void registerFP_OnRtnMarketData(MDUserApi mduser, FuncPtrOnRtnMarketData fp);
	TRADER_API_EXPORT void registerFP_OnRspSubMarketData(MDUserApi mduser, FuncPtrOnRspSubMarketData fp);
	TRADER_API_EXPORT void registerFP_OnRspUnsubMarketData(MDUserApi mduser, FuncPtrOnRspUnsubMarketData fp);

#ifdef __cplusplus
}
#endif

#endif