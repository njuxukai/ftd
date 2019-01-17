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
class CXcpFtdcTraderSpi
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
	virtual void OnRspInputOrder(CXcpFtdcInputOrderField* pInputOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///报单撤单请求响应
	virtual void OnRspInputOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///资金划转请求响应
	virtual void OnRspInputFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///资金查询响应
	virtual void OnRspQryFund(CXcpFtdcFundField* pFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///仓位查询响应
	virtual void OnRspQryPosition(CXcpFtdcPositionField* pPosition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///委托查询响应
	virtual void OnRspQryOrder(CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///成交查询响应
	virtual void OnRspQryTrade(CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///资金划转查询响应
	virtual void OnRspQryFundTransfer(CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///历史委托查询响应
	virtual void OnRspQryHisOrder(CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///历史成交查询响应
	virtual void OnRspQryHisTrade(CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///历史资金划转查询响应
	virtual void OnRspQryHisFundTransfer(CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///证券信息查询请求响应
	virtual void OnRspQryInstrument(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///分级基金信息查询请求响应
	virtual void OnRspQryStructuredFund(CXcpFtdcStructuredFundField* pStructuredFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///ETF信息查询请求响应
	virtual void OnRspQryETF(CXcpFtdcETFField* pETF, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///ETF组成股查询请求响应
	virtual void OnRspQryETFComposition(CXcpFtdcETFCompositionField* pETFComposition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///当日可申购新股查询请求响应
	virtual void OnRspQryPurchasableNewIssueSecurity(CXcpFtdcNewIssueSecurityField* pPurchasableNewIssueSecurity, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///客户新股申购额度查询请求响应
	virtual void OnRspQryPurchaseQuota(CXcpFtdcPurchaseQuotaField* pPurchaseQuota, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///报单成交回报推送响应
	virtual void OnRtnOrderExecutionReport(CXcpFtdcExecutionReportField* pExecutionReport) {}

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

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(const char* frontAddr) = 0;

	///订阅私有流。
	///@param nResumeType 私有流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后私有流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	virtual void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) = 0;

	///订阅公共流。
	///@param nResumeType 公共流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	virtual void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) = 0;

	///用户登录请求
	virtual int ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequstID) = 0;

	///用户登出请求
	virtual int ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID) = 0;

	///报单请求
	virtual int ReqOrderInsert(CXcpFtdcInputOrderField* pInputOrder, int nRequestID) = 0;

	///报单操作请求
	virtual int ReqOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID) = 0;

	///资金划转请求
	virtual int ReqFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID) = 0;

	///委托查询请求
	virtual int ReqQryOrder(CXcpFtdcQryOrderField* pQryOrder, int nRquestID) = 0;

	///成交查询请求
	virtual int ReqQryTrade(CXcpFtdcQryTradeField* pQryTrade, int nRequestID) = 0;

	///资金查询请求
	virtual int ReqQryFund(CXcpFtdcQryFundField *pQryFund, int nRequestID) = 0;

	///仓位查询请求
	virtual int ReqQryPosition(CXcpFtdcQryPositionField *pQryPosition, int nRequestID) = 0;

	///资金划转查询请求
	virtual int ReqQryFundTransfer(CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID) = 0;

	///查询历史委托请求
	virtual int ReqQryHisOrder(CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID) = 0;

	///查询历史成交请求
	virtual int ReqQryHisTrade(CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID) = 0;

	///查询历史资金划转请求
	virtual int ReqQryHisFundTransfer(CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID) = 0;

	///查询证券信息请求
	virtual int ReqQryInstrument(CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID) = 0;

	///查询ETF信息请求
	virtual int ReqQryETF(CXcpFtdcQryETFField *pQryETF, int nRequestID) = 0;

	///查询ETF成分股信息请求
	virtual int ReqQryETFComposition(CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID) = 0;

	///查询分级基金信息请求
	virtual int ReqQryStructuredFund(CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID) = 0;

	///查询可申购新股信息请求
	virtual int ReqQryPurchasableNewIssueSecurity(CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID) = 0;

	///查询客户新股申购额度请求
	virtual int ReqQryPurchaseQuota(CXcpFtdcQryPurchaseQuotaField *pQryPurchaseQuota, int nRequestID) = 0;

protected:
	~CXcpFtdcTraderApi() {}
};
#endif

typedef void(__cdecl *FuncPtrOnFrontConnected) ();
typedef void(__cdecl *FuncPtrOnFrontDisconnected) (int nReason);
typedef void(__cdecl *FuncPtrOnHeartBeatWarning) (int nTimeLapse);
typedef void(__cdecl *FuncPtrOnHeartBeat) ();
typedef void(__cdecl *FuncPtrOnRspUserLogin) (CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspUserLogout) (CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspInputOrder) (CXcpFtdcInputOrderField* pInputOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspInputOrderAction) (CXcpFtdcInputOrderActionField* pInputOrderAction, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspInputFundTransfer) (CXcpFtdcInputFundTransferField* pInputFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryFund) (CXcpFtdcFundField* pFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryPosition) (CXcpFtdcPositionField* pPosition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryOrder) (CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryTrade) (CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryFundTransfer) (CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryHisOrder) (CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryHisTrade) (CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryHisFundTransfer) (CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryInstrument) (CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryStructuredFund) (CXcpFtdcStructuredFundField* pStructuredFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryETF) (CXcpFtdcETFField* pETF, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryETFComposition) (CXcpFtdcETFCompositionField* pETFComposition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryPurchasableNewIssueSecurity) (CXcpFtdcNewIssueSecurityField* pPurchasableNewIssueSecurity, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspQryPurchaseQuota) (CXcpFtdcPurchaseQuotaField* pPurchaseQuota, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRtnOrderExecutionReport) (CXcpFtdcExecutionReportField* pExecutionReport);

#ifdef __cplusplus
extern "C"
{
#endif
	typedef void* TraderApi;
	TRADER_API_EXPORT void create_trader(void** p_trader, const char* psw="");
	TRADER_API_EXPORT void init_trader(TraderApi trader);
	TRADER_API_EXPORT void release_trader(TraderApi* trader);	
	


	//交易函数 应在connect后,OnRspUserLogin回调收到成功登录后再调用，如未登录调用返回错误代码
	TRADER_API_EXPORT const char* get_api_version(TraderApi trader);
	TRADER_API_EXPORT const char* get_trading_day(TraderApi trader);
	TRADER_API_EXPORT int req_user_login(TraderApi trader, CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequestID);
	TRADER_API_EXPORT int req_user_logout(TraderApi trader, CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID);
	TRADER_API_EXPORT int req_order_insert(TraderApi trader, CXcpFtdcInputOrderField* pInputOrder, int nRequestID);
	TRADER_API_EXPORT int req_order_action(TraderApi trader, CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID);
	TRADER_API_EXPORT int req_fund_transfer(TraderApi trader, CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID);
	TRADER_API_EXPORT int req_qry_order(TraderApi trader, CXcpFtdcQryOrderField* pQryOrder, int nRquestID);
	TRADER_API_EXPORT int req_qry_trade(TraderApi trader, CXcpFtdcQryTradeField* pQryTrade, int nRequestID);
	TRADER_API_EXPORT int req_qry_fund(TraderApi trader, CXcpFtdcQryFundField *pQryFund, int nRequestID);
	TRADER_API_EXPORT int req_qry_position(TraderApi trader, CXcpFtdcQryPositionField *pQryPosition, int nRequestID);
	TRADER_API_EXPORT int req_qry_fund_transfer(TraderApi trader, CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID);
	TRADER_API_EXPORT int req_qry_his_order(TraderApi trader, CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID);
	TRADER_API_EXPORT int req_qry_his_trade(TraderApi trader, CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID);
	TRADER_API_EXPORT int req_qry_his_fund_transfer(TraderApi trader, CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID);
	TRADER_API_EXPORT int req_qry_instrument(TraderApi trader, CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID);
	TRADER_API_EXPORT int req_qry_etf(TraderApi trader, CXcpFtdcQryETFField *pQryETF, int nRequestID);
	TRADER_API_EXPORT int req_qry_etf_composition(TraderApi trader, CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID);
	TRADER_API_EXPORT int req_qry_structured_fund(TraderApi trader, CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID);
	TRADER_API_EXPORT int req_qry_purchasable_new_issue_security(TraderApi trader, CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID);
	TRADER_API_EXPORT int req_qry_purchase_quota(TraderApi trader, CXcpFtdcQryPurchaseQuotaField *pQryPurchaseQuota, int nRequestID);

	//配置函数 应在connect_trader前调用
	TRADER_API_EXPORT void register_front(TraderApi trader, const char* front_address);
	TRADER_API_EXPORT void subscribe_private_topic(TraderApi trader, THOST_TE_RESUME_TYPE resume_type);
	TRADER_API_EXPORT void subscribe_public_topic(TraderApi trader, THOST_TE_RESUME_TYPE resume_type);
    //注册回调 应在connect_trader前调用（只需要注册需要的回调）
	TRADER_API_EXPORT void registerFP_OnFrontConnected(TraderApi* trader, FuncPtrOnFrontConnected fp);
	TRADER_API_EXPORT void registerFP_OnFrontDisconnected(TraderApi* trader, FuncPtrOnFrontDisconnected fp);
	TRADER_API_EXPORT void registerFP_OnHeartBeatWarning(TraderApi* trader, FuncPtrOnHeartBeatWarning fp);
	TRADER_API_EXPORT void registerFP_OnHeartBeat(TraderApi* trader, FuncPtrOnHeartBeat fp);
	TRADER_API_EXPORT void registerFP_OnRspUserLogin(TraderApi* trader, FuncPtrOnRspUserLogin fp);
	TRADER_API_EXPORT void registerFP_OnRspUserLogout(TraderApi* trader, FuncPtrOnRspUserLogout fp);
	TRADER_API_EXPORT void registerFP_OnRspInputOrder(TraderApi* trader, FuncPtrOnRspInputOrder fp);
	TRADER_API_EXPORT void registerFP_OnRspInputOrderAction(TraderApi* trader, FuncPtrOnRspInputOrderAction fp);
	TRADER_API_EXPORT void registerFP_OnRspInputFundTransfer(TraderApi* trader, FuncPtrOnRspInputFundTransfer fp);
	TRADER_API_EXPORT void registerFP_OnRspQryFund(TraderApi* trader, FuncPtrOnRspQryFund fp);
	TRADER_API_EXPORT void registerFP_OnRspQryPosition(TraderApi* trader, FuncPtrOnRspQryPosition fp);
	TRADER_API_EXPORT void registerFP_OnRspQryOrder(TraderApi* trader, FuncPtrOnRspQryOrder fp);
	TRADER_API_EXPORT void registerFP_OnRspQryTrade(TraderApi* trader, FuncPtrOnRspQryTrade fp);
	TRADER_API_EXPORT void registerFP_OnRspQryFundTransfer(TraderApi* trader, FuncPtrOnRspQryFundTransfer fp);
	TRADER_API_EXPORT void registerFP_OnRspQryHisOrder(TraderApi* trader, FuncPtrOnRspQryHisOrder fp);
	TRADER_API_EXPORT void registerFP_OnRspQryHisTrade(TraderApi* trader, FuncPtrOnRspQryHisTrade fp);
	TRADER_API_EXPORT void registerFP_OnRspQryHisFundTransfer(TraderApi* trader, FuncPtrOnRspQryHisFundTransfer fp);
	TRADER_API_EXPORT void registerFP_OnRspQryInstrument(TraderApi* trader, FuncPtrOnRspQryInstrument fp);
	TRADER_API_EXPORT void registerFP_OnRspQryStructuredFund(TraderApi* trader, FuncPtrOnRspQryStructuredFund   fp);
	TRADER_API_EXPORT void registerFP_OnRspQryETF(TraderApi* trader, FuncPtrOnRspQryETF fp);
	TRADER_API_EXPORT void registerFP_OnRspQryETFComposition(TraderApi* trader, FuncPtrOnRspQryETFComposition fp);
	TRADER_API_EXPORT void registerFP_OnRspQryPurchasableNewIssueSecurity(TraderApi* trader, FuncPtrOnRspQryPurchasableNewIssueSecurity fp);
	TRADER_API_EXPORT void registerFP_OnRspQryPurchaseQuota(TraderApi* trader, FuncPtrOnRspQryPurchaseQuota  fp);
	TRADER_API_EXPORT void registerFP_OnRtnOrderExecutionReport(TraderApi* trader, FuncPtrOnRtnOrderExecutionReport fp);

	

#ifdef __cplusplus
}
#endif

#endif