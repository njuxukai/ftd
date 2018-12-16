#ifndef XCP_FTDC_TRADER_API_IMPL_H
#define XCP_FTDC_TRADER_API_IMPL_H

#include "XcpFtdcTraderApi.h"
#include <ftd/Application.h>
#include <ftd/Initiator.h>
#include <ftd/FTD30/PackageCracker.h>

#define XCP_OK 0
#define XCP_ERR_CODE_INITIATOR_NULL -8001
#define XCP_ERR_CODE_INITIATOR_NOT_LOGGED -8002
#define XCP_ERR_CODE_FTD_SEND_FAILURE -8003


class CXcpFtdcTraderApiImpl : public CXcpFtdcTraderApi,
	public FTD::Application, public FTD::PackageCracker
{
public:
	CXcpFtdcTraderApiImpl(const char* pswDir);
	virtual ~CXcpFtdcTraderApiImpl();

	////////////////////////////////////////////////////////////////////////////
	///TraderApi
	///
	////////////////////////////////////////////////////////////////////////////
	virtual const char* GetApiVersion();

	virtual void Release() ;

	virtual void Init() ;

	virtual void Join() ;

	virtual const char* GetTradingDay() ;


	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CXcpFtdcTraderSpi *pSpi) ;

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(const char* frontAddr) ;

	///订阅私有流。
	///@param nResumeType 私有流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后私有流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	virtual void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) ;

	///订阅公共流。
	///@param nResumeType 公共流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	virtual void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) ;

	///用户登录请求
	virtual int ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequstID) ;

	///用户登出请求
	virtual int ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID) ;

	///报单请求
	virtual int ReqOrderInsert(CXcpFtdcInputOrderField* pInputOrder, int nRequestID) ;

	///报单操作请求
	virtual int ReqOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID) ;

	///资金划转请求
	virtual int ReqFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID) ;

	///委托查询请求
	virtual int ReqQryOrder(CXcpFtdcQryOrderField* pQryOrder, int nRquestID) ;

	///成交查询请求
	virtual int ReqQryTrade(CXcpFtdcQryTradeField* pQryTrade, int nRequestID) ;

	///资金查询请求
	virtual int ReqQryFund(CXcpFtdcQryFundField *pQryFund, int nRequestID) ;

	///仓位查询请求
	virtual int ReqQryPosition(CXcpFtdcQryPositionField *pQryPosition, int nRequestID) ;

	///资金划转查询请求
	virtual int ReqQryFundTransfer(CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID) ;

	///查询历史委托请求
	virtual int ReqQryHisOrder(CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID) ;

	///查询历史成交请求
	virtual int ReqQryHisTrade(CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID) ;

	///查询历史资金划转请求
	virtual int ReqQryHisFundTransfer(CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID) ;

	///查询证券信息请求
	virtual int ReqQryInstrument(CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID) ;

	///查询ETF信息请求
	virtual int ReqQryETF(CXcpFtdcQryETFField *pQryETF, int nRequestID) ;

	///查询ETF成分股信息请求
	virtual int ReqQryETFComposition(CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID) ;

	///查询分级基金信息请求
	virtual int ReqQryStructuredFund(CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID) ;

	///查询可申购新股信息请求
	virtual int ReqQryPurchasableNewIssueSecurity(CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID) ;

	///查询客户新股申购额度请求
	virtual int ReqQryPurchaseQuota(CXcpFtdcQryPurchaseQuotaField *pQryPurchaseQuota, int nRequestID) ;
    


	////////////////////////////////////////////////////////////////////////////
	///FTD::Application FTD -->ApiImpl
	///
	////////////////////////////////////////////////////////////////////////////

	/// Notification of a session begin created
	virtual void onCreate(const FTD::SessionID&);
	///
	virtual void onConnect(const FTD::SessionID&);
	//
	virtual void onDisconnect(const FTD::SessionID&);
	/// Notification of a session successfully logging on

	virtual void onLogon(const FTD::SessionID&);
	/// Notification of a session logging off or disconnecting
	virtual void onLogout(const FTD::SessionID&);
	/// Notification of admin message being sent to target
	virtual void toAdmin(FTD::Package&, const FTD::SessionID&);
	/// Notification of app message being sent to target
	virtual void toApp(FTD::Package&, const FTD::SessionID&);
	/// Notification of admin message being received from target
	virtual void fromAdmin(const FTD::Package&, const FTD::SessionID&);
	/// Notification of app message being received from target
	virtual void fromApp(const FTD::Package&, const FTD::SessionID&);

	virtual void onHeartBeat();

	virtual void onHeartBeatWarning();


	////////////////////////////////////////////////////////////////////////////
	///PackageCracker
	///
	////////////////////////////////////////////////////////////////////////////
	virtual void OnPackage(FTD::RspUserLogin& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::ForceExit& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspUserLogout& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspOrderInsert& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspOrderAction& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspFundTransfer& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryFund& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryPosition& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryOrder& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryTrade& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryFundTransfer& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryHisOrder& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryHisTrade& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryHisFundTransfer& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryInstrument& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryStructuredFund& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryETF& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryETFComposition& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryPurchasableNewIssueSecurity& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryPurchaseQuota& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::RspQryPrivateInitialData& package, const FTD::SessionID& id);
	virtual void OnPackage(FTD::IncExecutionReports& package, const FTD::SessionID& id);

	///
private:
	int send(FTD::Package& package, bool checkLogged=true);
	FTD::SessionID m_connectedSessionID;
	std::string m_pswDir;
	std::vector<std::string> m_frontAddresses;
	CXcpFtdcTraderSpi* m_pSpi;
	FTD::Initiator* m_pInitiator;
	bool m_privateDataSynced;
	std::map<TXcpFtdcSysIDType, CXcpFtdcExecutionReportField> m_BufferExecutionReport;
};
#endif