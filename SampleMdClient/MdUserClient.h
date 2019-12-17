#pragma once
#include  <xcp\XcpFtdcMdUserApi.h>
#include <iostream>
#include <mutex>

struct ApiParameter
{
	char frontAddress[30];
	char frontAddress2[30];
	char fileDirectory[100];
	int publicResume;
	int privateResume;
	CXcpFtdcReqUserLoginField loginField;	
};

class MdUserClient :public CXcpFtdcMdUserSpi
{
	struct SessionContext
	{
		int front_id;
		int session_id;
		int max_order_ref;
		int next_req_id;
	};

public:
	MdUserClient() : m_pApi(0), m_parameter({ 0 }), m_sessionContext({ 0 })
	{}

	~MdUserClient()
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
			//field.UserID = 100;
			//field.BrokerID = 8000;
			return m_pApi->ReqUserLogin(&field, 0);
		}
		return -1;
	}


	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ���������
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
	virtual void OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///��������������Ӧ
	virtual void OnRspSubMarketData(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
	
	virtual void OnMaketData(CXcpFtdcMarketDataField* pmd);
private:
	CXcpFtdcMdUserApi* m_pApi;
	ApiParameter m_parameter;
	SessionContext m_sessionContext;
	std::mutex m_tradeMutex;
};