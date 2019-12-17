#include "stdafx.h"
#include "MdUserClient.h"
#include <boost/format.hpp>

void MdUserClient::connect()
{
	m_sessionContext.next_req_id = 0;

	m_pApi = CXcpFtdcMdUserApi::CreateFtdcMdUserApi(m_parameter.fileDirectory);
	m_pApi->RegisterSpi(this);
	m_pApi->RegisterFront(m_parameter.frontAddress);
	m_pApi->RegisterFront(m_parameter.frontAddress2);
	m_pApi->Init();
}

void MdUserClient::disconnect()
{
	if (m_pApi != NULL)
	{
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;
	}
}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void MdUserClient::OnFrontConnected()
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
void MdUserClient::OnFrontDisconnected(int nReason)
{}

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void MdUserClient::OnHeartBeatWarning(int nTimeLapse) {}

///�ͻ���¼������Ӧ
void MdUserClient::OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
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
	std::cout << boost::format( "��¼��̨�ɹ�[FrontID=%d][SessionID=%d][MaxOrderRef=%d][heartBeat=%d]\n") %
		m_sessionContext.front_id % m_sessionContext.session_id % m_sessionContext.max_order_ref % pRspUserLogin->HeartbeatInterval;

	CXcpFtdcInstrumentField *instruments = new CXcpFtdcInstrumentField[2];
	memset(instruments, 0, 2 * sizeof(CXcpFtdcInstrumentField));
	strcpy(instruments[0].InstrumentCode, "600000");
	instruments[0].ExchangeType = XCP_FTDC_ET_SH;
	strcpy(instruments[1].InstrumentCode, "000001");
	instruments[1].ExchangeType = XCP_FTDC_ET_SZ;
	m_pApi->ReqSubMarketData(instruments, 2, 0);

}

void MdUserClient::OnRspSubMarketData(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (pRspInfo == NULL || pRspInfo->ErrorCode == 0)
	{
		std::cout << boost::format("����[%c][%s]�г�����ɹ�")
			% pInstrument->ExchangeType
			% pInstrument->InstrumentCode
			<< std::endl;
	}
	else
	{
		std::cout << boost::format("����[%c][%s]�г�����ʧ��[%d][%s]")
			% pInstrument->ExchangeType
			% pInstrument->InstrumentCode
			% pRspInfo->ErrorCode
			% pRspInfo->ErrorText
			<< std::endl;
	}
}

void MdUserClient::OnMaketData(CXcpFtdcMarketDataField* pmd)
{
	std::cout << boost::format("[%s][%d][LastPrice=%lf]")
		% pmd->InstrumentCode
		% pmd->ExchangeType
		% pmd->PriceLast
		<< std::endl;
}
