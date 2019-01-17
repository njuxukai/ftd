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
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected() {};

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason) {}

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse) {}

	///
	virtual void OnHeartBeat() {}
	///�ͻ���¼������Ӧ
	virtual void OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///�ͻ��ǳ�������Ӧ
	virtual void OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///����ί��������Ӧ
	virtual void OnRspInputOrder(CXcpFtdcInputOrderField* pInputOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///��������������Ӧ
	virtual void OnRspInputOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///�ʽ�ת������Ӧ
	virtual void OnRspInputFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///�ʽ��ѯ��Ӧ
	virtual void OnRspQryFund(CXcpFtdcFundField* pFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///��λ��ѯ��Ӧ
	virtual void OnRspQryPosition(CXcpFtdcPositionField* pPosition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///ί�в�ѯ��Ӧ
	virtual void OnRspQryOrder(CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///�ɽ���ѯ��Ӧ
	virtual void OnRspQryTrade(CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///�ʽ�ת��ѯ��Ӧ
	virtual void OnRspQryFundTransfer(CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///��ʷί�в�ѯ��Ӧ
	virtual void OnRspQryHisOrder(CXcpFtdcOrderField* pOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///��ʷ�ɽ���ѯ��Ӧ
	virtual void OnRspQryHisTrade(CXcpFtdcTradeField* pTrade, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///��ʷ�ʽ�ת��ѯ��Ӧ
	virtual void OnRspQryHisFundTransfer(CXcpFtdcFundTransferField* pFundTransfer, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///֤ȯ��Ϣ��ѯ������Ӧ
	virtual void OnRspQryInstrument(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///�ּ�������Ϣ��ѯ������Ӧ
	virtual void OnRspQryStructuredFund(CXcpFtdcStructuredFundField* pStructuredFund, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///ETF��Ϣ��ѯ������Ӧ
	virtual void OnRspQryETF(CXcpFtdcETFField* pETF, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///ETF��ɹɲ�ѯ������Ӧ
	virtual void OnRspQryETFComposition(CXcpFtdcETFCompositionField* pETFComposition, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///���տ��깺�¹ɲ�ѯ������Ӧ
	virtual void OnRspQryPurchasableNewIssueSecurity(CXcpFtdcNewIssueSecurityField* pPurchasableNewIssueSecurity, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///�ͻ��¹��깺��Ȳ�ѯ������Ӧ
	virtual void OnRspQryPurchaseQuota(CXcpFtdcPurchaseQuotaField* pPurchaseQuota, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///�����ɽ��ر�������Ӧ
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


	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CXcpFtdcTraderSpi *pSpi) = 0;

	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(const char* frontAddr) = 0;

	///����˽������
	///@param nResumeType ˽�����ش���ʽ  
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼��˽����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	virtual void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) = 0;

	///���Ĺ�������
	///@param nResumeType �������ش���ʽ  
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼�󹫹���������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
	virtual void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) = 0;

	///�û���¼����
	virtual int ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequstID) = 0;

	///�û��ǳ�����
	virtual int ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID) = 0;

	///��������
	virtual int ReqOrderInsert(CXcpFtdcInputOrderField* pInputOrder, int nRequestID) = 0;

	///������������
	virtual int ReqOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID) = 0;

	///�ʽ�ת����
	virtual int ReqFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID) = 0;

	///ί�в�ѯ����
	virtual int ReqQryOrder(CXcpFtdcQryOrderField* pQryOrder, int nRquestID) = 0;

	///�ɽ���ѯ����
	virtual int ReqQryTrade(CXcpFtdcQryTradeField* pQryTrade, int nRequestID) = 0;

	///�ʽ��ѯ����
	virtual int ReqQryFund(CXcpFtdcQryFundField *pQryFund, int nRequestID) = 0;

	///��λ��ѯ����
	virtual int ReqQryPosition(CXcpFtdcQryPositionField *pQryPosition, int nRequestID) = 0;

	///�ʽ�ת��ѯ����
	virtual int ReqQryFundTransfer(CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID) = 0;

	///��ѯ��ʷί������
	virtual int ReqQryHisOrder(CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID) = 0;

	///��ѯ��ʷ�ɽ�����
	virtual int ReqQryHisTrade(CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID) = 0;

	///��ѯ��ʷ�ʽ�ת����
	virtual int ReqQryHisFundTransfer(CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID) = 0;

	///��ѯ֤ȯ��Ϣ����
	virtual int ReqQryInstrument(CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID) = 0;

	///��ѯETF��Ϣ����
	virtual int ReqQryETF(CXcpFtdcQryETFField *pQryETF, int nRequestID) = 0;

	///��ѯETF�ɷֹ���Ϣ����
	virtual int ReqQryETFComposition(CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID) = 0;

	///��ѯ�ּ�������Ϣ����
	virtual int ReqQryStructuredFund(CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID) = 0;

	///��ѯ���깺�¹���Ϣ����
	virtual int ReqQryPurchasableNewIssueSecurity(CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID) = 0;

	///��ѯ�ͻ��¹��깺�������
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
	


	//���׺��� Ӧ��connect��,OnRspUserLogin�ص��յ��ɹ���¼���ٵ��ã���δ��¼���÷��ش������
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

	//���ú��� Ӧ��connect_traderǰ����
	TRADER_API_EXPORT void register_front(TraderApi trader, const char* front_address);
	TRADER_API_EXPORT void subscribe_private_topic(TraderApi trader, THOST_TE_RESUME_TYPE resume_type);
	TRADER_API_EXPORT void subscribe_public_topic(TraderApi trader, THOST_TE_RESUME_TYPE resume_type);
    //ע��ص� Ӧ��connect_traderǰ���ã�ֻ��Ҫע����Ҫ�Ļص���
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