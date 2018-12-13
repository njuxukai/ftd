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
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected() {};

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason) {};

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse) {};

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
	virtual void OnRtnOrderExecutionReport(CXcpFtdcExecutionReportField* pExecutionReport, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

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

	///
	virtual int ReqOrderInsert(CXcpFtdcInputOrderField* pInputOrder, int nRequestID) = 0;

	
	virtual int ReqOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID) = 0;

	
	virtual int ReqFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID) = 0;


	virtual int ReqQryOrder(CXcpFtdcQryOrderField* pQryOrder, int nRquestID) = 0;

	virtual int ReqQryTrade(CXcpFtdcQryTradeField* pQryTrade, int nRequestID) = 0;

	virtual int ReqQryFund(CXcpFtdcQryFundField *pQryFund, int nRequestID) = 0;

	virtual int ReqQryPosition(CXcpFtdcQryPositionField *pQryPosition, int nRequestID) = 0;

	virtual int ReqQryFundTransfer(CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID) = 0;

	virtual int ReqQryHisOrder(CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID) = 0;

	virtual int ReqQryHisTrade(CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID) = 0;

	virtual int ReqQryHisFundTransfer(CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID) = 0;

	virtual int ReqQryInstrument(CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID) = 0;

	virtual int ReqQryETF(CXcpFtdcQryETFField *pQryETF, int nRequestID) = 0;

	virtual int ReqQryETFCompisition(CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID) = 0;

	virtual int ReqQryStrucutredFund(CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID) = 0;

	virtual int ReqQryPurchasableNewIssueSecurity(CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID) = 0;

	virtual int ReqQryPurchaseQuota(CXcpFtdcQryPurchaseQuotaField *pQryPurchaseQuota, int nRequestID) = 0;

protected:
	~CXcpFtdcTraderApi() {}
};
#endif