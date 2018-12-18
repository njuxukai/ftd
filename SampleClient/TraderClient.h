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
		CXcpFtdcReqUserLoginField field = { 0 };
		if (m_pApi)
		{
			return m_pApi->ReqUserLogin(&field, 0);
		}
		return -1;
	}
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///客户登录请求响应
	virtual void OnRspUserLogin(CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);

	///客户登出请求响应
	virtual void OnRspUserLogout(CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);

	///报单委托请求响应
	virtual void OnRspInputOrder(CXcpFtdcInputOrderField* pInputOrder, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
private:
	CXcpFtdcTraderApi* m_pApi;
	ApiParameter m_parameter;
	SessionContext m_sessionContext;
	std::mutex m_tradeMutex;
};