#include "XcpFtdcTraderApi.h"
#include "XcpCTrader.h"

void create_trader(TraderApi* p_trader)
{
	*p_trader = (TraderApi)new CXcpTrader();
}

void release_trader(TraderApi* ptrader)
{
	CXcpTrader* pImpl = (CXcpTrader*)(*ptrader);
	if (pImpl)
		delete(pImpl);
	*ptrader = 0;
}

int connect_trader(TraderApi trader) 
{
	((CXcpTrader*)trader)->Connect();
	return 0;
}

int disconnect_trader(TraderApi trader)
{
	((CXcpTrader*)trader)->Disconnect();
	return 0;
}

//交易函数 应在connect后,OnRspUserLogin回调收到成功登录后再调用，如未登录调用返回错误代码
int req_order_insert(TraderApi trader, CXcpFtdcInputOrderField* pInputOrder, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqOrderInsert(pInputOrder, nRequestID);
}

int req_order_action(TraderApi trader, CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqOrderAction(pInputOrderAction, nRequestID);
}

int req_fund_transfer(TraderApi trader, CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqFundTransfer(pInputFundTransfer, nRequestID);
}

int req_qry_order(TraderApi trader, CXcpFtdcQryOrderField* pQryOrder, int nRquestID) 
{
	return ((CXcpTrader*)trader)->ReqQryOrder(pQryOrder, nRquestID);
}

int req_qry_trade(TraderApi trader, CXcpFtdcQryTradeField* pQryTrade, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryTrade(pQryTrade, nRequestID);
}

int req_qry_fund(TraderApi trader, CXcpFtdcQryFundField *pQryFund, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryFund(pQryFund, nRequestID);
}

int req_qry_position(TraderApi trader, CXcpFtdcQryPositionField *pQryPosition, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryPosition(pQryPosition, nRequestID);
}

int req_qry_fund_transfer(TraderApi trader, CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryFundTransfer(pQryFundTransfer, nRequestID);
}

int req_qry_his_order(TraderApi trader, CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryHisOrder(pQryHisOrder, nRequestID);
}

int req_qry_his_trade(TraderApi trader, CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryHisTrade(pQryHisTrade, nRequestID);
}

int req_qry_his_fund_transfer(TraderApi trader, CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryHisFundTransfer(pQryHisFundTransfer, nRequestID);
}

int req_qry_instrument(TraderApi trader, CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryInstrument(pQryInstrument, nRequestID);
}

int req_qry_etf(TraderApi trader, CXcpFtdcQryETFField *pQryETF, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryETF(pQryETF, nRequestID);
}

int req_qry_etf_composition(TraderApi trader, CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryETFComposition(pQryEtfComposition, nRequestID);
}

int req_qry_structured_fund(TraderApi trader, CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryStructuredFund(pQryStructuredFund, nRequestID);
}

int req_qry_purchasable_new_issue_security(TraderApi trader, CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryPurchasableNewIssueSecurity(pQryPurchasableNewSecurity, nRequestID);
}

int req_qry_purchase_quota(TraderApi trader, CXcpFtdcQryPurchaseQuotaField *pQryPurchaseQuota, int nRequestID) 
{
	return ((CXcpTrader*)trader)->ReqQryPurchaseQuota(pQryPurchaseQuota, nRequestID);
}


//配置函数 应在connect_trader前调用
void register_flow_path(TraderApi trader, const char* path)
{
	((CXcpTrader*)trader)->RegisterFlowPath(path);
}

void register_front(TraderApi trader, const char* front_address) 
{
	((CXcpTrader*)trader)->RegisterFront(front_address);
}

void subscribe_private_topic(TraderApi trader, THOST_TE_RESUME_TYPE resume_type) 
{
	((CXcpTrader*)trader)->SubscribePrivateTopic(resume_type);
}

void subscribe_public_topic(TraderApi trader, THOST_TE_RESUME_TYPE resume_type) 
{
	((CXcpTrader*)trader)->SubscribePublicTopic(resume_type);
}

void attach_userloginfield(TraderApi trader, const CXcpFtdcReqUserLoginField* field) 
{
	((CXcpTrader*)trader)->AttachUserLoginField(field);
}

//注册回调 应在connect_trader前调用（只需要注册需要的回调）
void registerFP_OnFrontConnected(TraderApi* trader, FuncPtrOnFrontConnected fp)
{
	((CXcpTrader*)trader)->registerFP_OnFrontConnected(fp);
}

void registerFP_OnFrontDisconnected(TraderApi* trader, FuncPtrOnFrontDisconnected fp) 
{
	((CXcpTrader*)trader)->registerFP_OnFrontDisconnected(fp);
}

void registerFP_OnHeartBeatWarning(TraderApi* trader, FuncPtrOnHeartBeatWarning fp) 
{
	((CXcpTrader*)trader)->registerFP_OnHeartBeatWarning(fp);
}

void registerFP_OnRspUserLogin(TraderApi* trader, FuncPtrOnRspUserLogin fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspUserLogin(fp);
}

void registerFP_OnRspUserLogout(TraderApi* trader, FuncPtrOnRspUserLogout fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspUserLogout(fp);
}

void registerFP_OnRspInputOrder(TraderApi* trader, FuncPtrOnRspInputOrder fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspInputOrder(fp);
}

void registerFP_OnRspInputOrderAction(TraderApi* trader, FuncPtrOnRspInputOrderAction fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspInputOrderAction(fp);
}

void registerFP_OnRspInputFundTransfer(TraderApi* trader, FuncPtrOnRspInputFundTransfer fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspInputFundTransfer(fp);
}

void registerFP_OnRspQryFund(TraderApi* trader, FuncPtrOnRspQryFund fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryFund(fp);
}

void registerFP_OnRspQryPosition(TraderApi* trader, FuncPtrOnRspQryPosition fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryPosition(fp);
}

void registerFP_OnRspQryOrder(TraderApi* trader, FuncPtrOnRspQryOrder fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryOrder(fp);
}

void registerFP_OnRspQryTrade(TraderApi* trader, FuncPtrOnRspQryTrade fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryTrade(fp);
}

void registerFP_OnRspQryFundTransfer(TraderApi* trader, FuncPtrOnRspQryFundTransfer fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryFundTransfer(fp);
}

void registerFP_OnRspQryHisOrder(TraderApi* trader, FuncPtrOnRspQryHisOrder fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryHisOrder(fp);
}

void registerFP_OnRspQryHisTrade(TraderApi* trader, FuncPtrOnRspQryHisTrade fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryHisTrade(fp);
}

void registerFP_OnRspQryHisFundTransfer(TraderApi* trader, FuncPtrOnRspQryHisFundTransfer fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryHisFundTransfer(fp);
}

void registerFP_OnRspQryInstrument(TraderApi* trader, FuncPtrOnRspQryInstrument fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryInstrument(fp);
}

void registerFP_OnRspQryStructuredFund(TraderApi* trader, FuncPtrOnRspQryStructuredFund   fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryStructuredFund(fp);
}

void registerFP_OnRspQryETF(TraderApi* trader, FuncPtrOnRspQryETF fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryETF(fp);
}

void registerFP_OnRspQryETFComposition(TraderApi* trader, FuncPtrOnRspQryETFComposition fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryETFComposition(fp);
}

void registerFP_OnRspQryPurchasableNewIssueSecurity(TraderApi* trader, FuncPtrOnRspQryPurchasableNewIssueSecurity fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryPurchasableNewIssueSecurity(fp);
}

void registerFP_OnRspQryPurchaseQuota(TraderApi* trader, FuncPtrOnRspQryPurchaseQuota  fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRspQryPurchaseQuota(fp);
}

void registerFP_OnRtnOrderExecutionReport(TraderApi* trader, FuncPtrOnRtnOrderExecutionReport fp) 
{
	((CXcpTrader*)trader)->registerFP_OnRtnOrderExecutionReport(fp);
}

