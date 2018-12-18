#pragma once
#include  <xcp\XcpFtdcTraderApi.h>
#include <iostream>
#include <mutex>

struct ApiParameter
{
	char frontAddress[30];
	char fileDirectory[100];
	int publicResume;
	int privateResume;
	CXcpFtdcReqUserLoginField loginField;	
};

class TraderClient :public CXcpFtdcTraderSpi
{
	struct SessionContext
	{
		int front_id;
		int session_id;
		int max_order_ref;
		int next_req_id;
	};

public:
	TraderClient() : m_pApi(0), m_parameter({ 0 }), m_sessionContext({ 0 })
	{}

	~TraderClient()
	{
		disconnect();
	}

	void setParameter(const ApiParameter& parameter)
	{
		memcpy(&m_parameter, &parameter, sizeof(ApiParameter));
	}

	void connect();

	void disconnect();

	int manualTestLogin()
	{
		if (m_pApi)
		{
			CXcpFtdcReqUserLoginField field = { 0 };
			return m_pApi->ReqUserLogin(&field, 0);
		}
		return -1;
	}

	int manualTestInputOrder()
	{
		
		if (m_pApi)
		{
			CXcpFtdcInputOrderField field = { 0 };
			//user_info
			field.BrokerID = m_parameter.loginField.BrokerID;
			field.UserID = m_parameter.loginField.UserID;
			field.InvestorID = m_parameter.loginField.UserID;
			//session_info
			strcpy(field.InstrumentCode , "600000");
			strcpy(field.ExchangeCode, XCP_FTDC_EC_SH);
			field.VolumeTotalOrginal = 1000;
			field.PriceType = XCP_FTDC_OPT_2;
			field.LimitPrice = 10;
			field.Direction = XCP_FTDC_D_0;
			//order_ref request_id
			field.OrderRef = m_sessionContext.max_order_ref++;
			field.RequestID = ++m_sessionContext.next_req_id;
			return m_pApi->ReqOrderInsert(&field, 0);
		}
		return -1;
	}
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///�ͻ���¼������Ӧ
	virtual void OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);

	///�ͻ��ǳ�������Ӧ
	virtual void OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);

	///����ί��������Ӧ
	virtual void OnRspInputOrder(CXcpFtdcInputOrderField* pInputOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);

	virtual void OnRtnOrderExecutionReport(CXcpFtdcExecutionReportField* pExecutionReport);
private:
	CXcpFtdcTraderApi* m_pApi;
	ApiParameter m_parameter;
	SessionContext m_sessionContext;
	std::mutex m_tradeMutex;
};