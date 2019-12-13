#include "XcpFtdcMdUserApi.h"
#include "XcpCMdUser.h"

void create_mduser(MDUserApi* p_user, const char* psw)
{
	*p_user = (MDUserApi)new CXcpMdUser(psw);
}

void release_mduser(MDUserApi* puser)
{
	CXcpMdUser* pImpl = (CXcpMdUser*)(*puser);
	if (pImpl)
		delete(pImpl);
	*puser = 0;
}

void register_front(MDUserApi user, const char* front_address)
{
	((CXcpMdUser*)user)->RegisterFront(front_address);
}

void init_mduser(MDUserApi puser)
{
	((CXcpMdUser*)puser)->Init();
}



//交易函数 应在connect后,OnRspUserLogin回调收到成功登录后再调用，如未登录调用返回错误代码
const char* get_api_version(MDUserApi trader)
{
	return ((CXcpMdUser*)trader)->GetApiVersion();
}

const char* get_trading_day(MDUserApi trader)
{
	return ((CXcpMdUser*)trader)->GetTradingDay();
}

int req_user_login(MDUserApi user, CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequestID)
{
	return ((CXcpMdUser*)user)->ReqUserLogin(pReqUserLogin, nRequestID);
}

int req_user_logout(MDUserApi user, CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID)
{
	return ((CXcpMdUser*)user)->ReqUserLogout(pReqUserLogout, nRequestID);
}

int req_sub_market_data(MDUserApi user, CXcpFtdcInstrumentField* pInstrument, int size, int nRequestID)
{
	return ((CXcpMdUser*)user)->ReqSubMarketData(pInstrument, size, nRequestID);
}


int req_unsub_market_data(MDUserApi user, CXcpFtdcInstrumentField* pInstrument, int size, int nRequestID)
{
	return ((CXcpMdUser*)user)->ReqUnsubMarketData(pInstrument, size, nRequestID);
}


//注册回调 应在connect_trader前调用（只需要注册需要的回调）
void registerFP_OnFrontConnected(MDUserApi user, FuncPtrOnFrontConnected fp)
{
	((CXcpMdUser*)user)->registerFP_OnFrontConnected(fp);
}

void registerFP_OnFrontDisconnected(MDUserApi user, FuncPtrOnFrontDisconnected fp)
{
	((CXcpMdUser*)user)->registerFP_OnFrontDisconnected(fp);
}

void registerFP_OnHeartBeatWarning(MDUserApi user, FuncPtrOnHeartBeatWarning fp)
{
	((CXcpMdUser*)user)->registerFP_OnHeartBeatWarning(fp);
}

void registerFP_OnHeartBeat(MDUserApi user, FuncPtrOnHeartBeat fp)
{
	((CXcpMdUser*)user)->registerFP_OnHeartBeat(fp);
}

void registerFP_OnRspUserLogin(MDUserApi user, FuncPtrOnRspUserLogin fp)
{
	((CXcpMdUser*)user)->registerFP_OnRspUserLogin(fp);
}

void registerFP_OnRspUserLogout(MDUserApi user, FuncPtrOnRspUserLogout fp)
{
	((CXcpMdUser*)user)->registerFP_OnRspUserLogout(fp);
}

void registerFP_OnRtnMarketData(MDUserApi user, FuncPtrOnRtnMarketData fp)
{
	((CXcpMdUser*)user)->registerFP_OnRtnMarketData(fp);
}

void registerFP_OnRspSubMarketData(MDUserApi user, FuncPtrOnRspSubMarketData fp)
{
	((CXcpMdUser*)user)->registerFP_OnRspSubMarketData(fp);
}

void registerFP_OnRspUnsubMarketData(MDUserApi user, FuncPtrOnRspUnsubMarketData fp)
{
	((CXcpMdUser*)user)->registerFP_OnRspUnsubMarketData(fp);
}