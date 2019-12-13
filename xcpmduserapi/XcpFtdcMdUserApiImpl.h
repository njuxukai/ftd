#ifndef XCP_FTDC_TRADER_API_IMPL_H
#define XCP_FTDC_TRADER_API_IMPL_H

#include "XcpFtdcMdUserApi.h"
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



class CXcpFtdcMdUserApiImpl : public CXcpFtdcMdUserApi,
	public FTD::Application, public FTD::PackageCracker
{
public:
	CXcpFtdcMdUserApiImpl(const char* pswDir);
	virtual ~CXcpFtdcMdUserApiImpl();

	////////////////////////////////////////////////////////////////////////////
	///MdUserApi
	///
	////////////////////////////////////////////////////////////////////////////
	virtual const char* GetApiVersion();

	virtual void Release();

	virtual void Init();

	virtual void Join();

	virtual const char* GetTradingDay();


	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CXcpFtdcMdUserSpi *pSpi);

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(const char* frontAddr);


	///用户登录请求
	virtual int ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequstID);

	///用户登出请求
	virtual int ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID);


	///订阅行情
	virtual int ReqSubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID);

	///退订行情
	virtual int ReqUnsubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID);

	////////////////////////////////////////////////////////////////////////////
	///FTD::Application FTD -->ApiImpl
	///
	////////////////////////////////////////////////////////////////////////////

	/// Notification of a session begin created
	virtual void onCreate(const FTD::SessionID&);
	///
	virtual void onConnect(const FTD::SessionID&);
	//
	virtual void onDisconnect(const FTD::SessionID&, int reason);
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
	void OnRspUserLoginFailure(const FTD::RspUserLogin& package, const FTD::SessionID& id);
	void OnRspUserLoginSucceed(const FTD::RspUserLogin& package, const FTD::SessionID& id);

	virtual void OnPackage(const FTD::ForceExit& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspUserLogout& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspSubMarketData& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::RspUnsubMarketData& package, const FTD::SessionID& id);
	virtual void OnPackage(const FTD::IncMarketData& package, const FTD::SessionID& id);
	///
private:
	int send(FTD::Package& package, bool checkLogged=true);
	FTD::SessionID m_connectedSessionID;
	std::string m_pswDir;
	std::vector<std::string> m_frontAddresses;
	CXcpFtdcMdUserSpi* m_pSpi;
	FTD::Initiator* m_pInitiator;
	bool m_privateDataSynced;
	//std::map<TXcpFtdcSysIDType, CXcpFtdcExecutionReportField> m_BufferExecutionReport;
	ConnMmap* m_pPublicConn;
	ConnMmap* m_pPrivateConn;
	int m_publicResumeType;
	int m_privateResumeType;
	std::string m_tradingDay;
};
#endif