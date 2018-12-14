#include "stdafx.h"
#include "TraderClient.h"
#include <boost/format.hpp>

void TraderClient::connect()
{
	m_sessionContext.next_req_id = 0;

	m_pApi = CXcpFtdcTraderApi::CreateFtdcTraderApi(m_parameter.fileDirectory);
	m_pApi->RegisterSpi(this);
	m_pApi->RegisterFront(m_parameter.frontAddress);
	m_pApi->SubscribePrivateTopic(THOST_TERT_RESTART);
	m_pApi->SubscribePublicTopic(THOST_TERT_RESTART);
	m_pApi->Init();
}

void TraderClient::disconnect()
{
	if (m_pApi != NULL)
	{
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;
	}
}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void TraderClient::OnFrontConnected()
{
	std::lock_guard<std::mutex> lock(m_tradeMutex);
	m_sessionContext.next_req_id = 1;
	m_pApi->ReqUserLogin(&m_parameter.loginField, m_sessionContext.next_req_id);
	m_sessionContext.next_req_id += 1;
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void TraderClient::OnFrontDisconnected(int nReason)
{}

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void TraderClient::OnHeartBeatWarning(int nTimeLapse) {}

///�ͻ���¼������Ӧ
void TraderClient::OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (pRspInfo == nullptr)
	{
		std::cout << "��¼ʧ��,pRspInfoΪ��";
		return;
	}
	if (pRspInfo->ErrorCode != 0)
	{
		std::cout << boost::format("��¼ʧ��[%d][%s]\n") % pRspInfo->ErrorCode % pRspInfo->ErrorText;
		return;
	}

	m_sessionContext.front_id = pRspUserLogin->FrontID;
	m_sessionContext.session_id = pRspUserLogin->SessionID;
	m_sessionContext.max_order_ref = pRspUserLogin->MaxOrderRef;
	std::cout << boost::format( "��¼��̨�ɹ�[FrontID=%d][SessionID=%d][MaxOrderRef=%d]\n") %
		m_sessionContext.front_id % m_sessionContext.session_id % m_sessionContext.max_order_ref;
}

///�ͻ��ǳ�������Ӧ
void TraderClient::OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{}

///����ί��������Ӧ
void TraderClient::OnRspInputOrder(CXcpFtdcInputOrderField* pInputOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{}