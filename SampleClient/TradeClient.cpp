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

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void TraderClient::OnFrontConnected()
{
	std::lock_guard<std::mutex> lock(m_tradeMutex);
	m_sessionContext.next_req_id = 1;
	m_pApi->ReqUserLogin(&m_parameter.loginField, m_sessionContext.next_req_id);
	m_sessionContext.next_req_id += 1;
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void TraderClient::OnFrontDisconnected(int nReason)
{}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void TraderClient::OnHeartBeatWarning(int nTimeLapse) {}

///客户登录请求响应
void TraderClient::OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{
	if (pRspInfo == nullptr)
	{
		std::cout << "登录失败,pRspInfo为空";
		return;
	}
	if (pRspInfo->ErrorCode != 0)
	{
		std::cout << boost::format("登录失败[%d][%s]\n") % pRspInfo->ErrorCode % pRspInfo->ErrorText;
		return;
	}

	m_sessionContext.front_id = pRspUserLogin->FrontID;
	m_sessionContext.session_id = pRspUserLogin->SessionID;
	m_sessionContext.max_order_ref = pRspUserLogin->MaxOrderRef;
	std::cout << boost::format( "登录柜台成功[FrontID=%d][SessionID=%d][MaxOrderRef=%d]\n") %
		m_sessionContext.front_id % m_sessionContext.session_id % m_sessionContext.max_order_ref;
}

///客户登出请求响应
void TraderClient::OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{}

///报单委托请求响应
void TraderClient::OnRspInputOrder(CXcpFtdcInputOrderField* pInputOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast)
{}