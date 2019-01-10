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

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem.hpp>



class ConnMmap
{
public:
	ConnMmap(const std::string& fpath);

	bool writeDate(const std::string& tradeDate);

	bool readDate(std::string& tradeDate);

	bool writeSequenceSno(int sno);

	bool readSequenceSno(int& sno);

private:
	std::string m_fpath;
	boost::interprocess::file_mapping fmap;
	boost::interprocess::mapped_region mreg;
	void *m_addr;
	std::size_t m_size;
};



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


	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CXcpFtdcTraderSpi *pSpi) ;

	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(const char* frontAddr) ;

	///����˽������
	///@param nResumeType ˽�����ش���ʽ  
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼��˽����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	virtual void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) ;

	///���Ĺ�������
	///@param nResumeType �������ش���ʽ  
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼�󹫹���������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
	virtual void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) ;

	///�û���¼����
	virtual int ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequstID) ;

	///�û��ǳ�����
	virtual int ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID) ;

	///��������
	virtual int ReqOrderInsert(CXcpFtdcInputOrderField* pInputOrder, int nRequestID) ;

	///������������
	virtual int ReqOrderAction(CXcpFtdcInputOrderActionField* pInputOrderAction, int nRequestID) ;

	///�ʽ�ת����
	virtual int ReqFundTransfer(CXcpFtdcInputFundTransferField* pInputFundTransfer, int nRequestID) ;

	///ί�в�ѯ����
	virtual int ReqQryOrder(CXcpFtdcQryOrderField* pQryOrder, int nRquestID) ;

	///�ɽ���ѯ����
	virtual int ReqQryTrade(CXcpFtdcQryTradeField* pQryTrade, int nRequestID) ;

	///�ʽ��ѯ����
	virtual int ReqQryFund(CXcpFtdcQryFundField *pQryFund, int nRequestID) ;

	///��λ��ѯ����
	virtual int ReqQryPosition(CXcpFtdcQryPositionField *pQryPosition, int nRequestID) ;

	///�ʽ�ת��ѯ����
	virtual int ReqQryFundTransfer(CXcpFtdcQryFundTransferField *pQryFundTransfer, int nRequestID) ;

	///��ѯ��ʷί������
	virtual int ReqQryHisOrder(CXcpFtdcQryHisOrderField *pQryHisOrder, int nRequestID) ;

	///��ѯ��ʷ�ɽ�����
	virtual int ReqQryHisTrade(CXcpFtdcQryHisTradeField *pQryHisTrade, int nRequestID) ;

	///��ѯ��ʷ�ʽ�ת����
	virtual int ReqQryHisFundTransfer(CXcpFtdcQryHisFundTransferField *pQryHisFundTransfer, int nRequestID) ;

	///��ѯ֤ȯ��Ϣ����
	virtual int ReqQryInstrument(CXcpFtdcQryInstrumentField *pQryInstrument, int nRequestID) ;

	///��ѯETF��Ϣ����
	virtual int ReqQryETF(CXcpFtdcQryETFField *pQryETF, int nRequestID) ;

	///��ѯETF�ɷֹ���Ϣ����
	virtual int ReqQryETFComposition(CXcpFtdcQryETFCompositionField *pQryEtfComposition, int nRequestID) ;

	///��ѯ�ּ�������Ϣ����
	virtual int ReqQryStructuredFund(CXcpFtdcQryStructuredFundField *pQryStructuredFund, int nRequestID) ;

	///��ѯ���깺�¹���Ϣ����
	virtual int ReqQryPurchasableNewIssueSecurity(CXcpFtdcQryNewIssueSecurityField *pQryPurchasableNewSecurity, int nRequestID) ;

	///��ѯ�ͻ��¹��깺�������
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
	virtual void OnPackage(const FTD::RspUserLogin& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::ForceExit& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspUserLogout& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspOrderInsert& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspOrderAction& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspFundTransfer& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryFund& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryPosition& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryOrder& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryTrade& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryFundTransfer& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryHisOrder& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryHisTrade& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryHisFundTransfer& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryInstrument& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryStructuredFund& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryETF& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryETFComposition& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryPurchasableNewIssueSecurity& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryPurchaseQuota& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspQryPrivateInitialData& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::IncExecutionReports& package, const FTD::SessionID& id);

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
	ConnMmap* m_pPublicConn;
	ConnMmap* m_pPrivateConn;
	int m_publicResumeType;
	int m_privateResumeType;
	std::string m_tradingDay;
};
#endif