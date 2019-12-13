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
class CXcpFtdcMdUserSpi
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
	virtual void OnMaketData(CXcpFtdcMarketDataField* pmd) {}

	///��������������Ӧ
	virtual void OnRspSubMarketData(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}

	///�˶�����������Ӧ
	virtual void OnRspUnsubMarketData(CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast) {}
	
};

class TRADER_API_EXPORT CXcpFtdcMdUserApi
{
public:
	static CXcpFtdcMdUserApi *CreateFtdcMdUserApi(const char*pszFlowPath = "");

	virtual const char* GetApiVersion() = 0;

	virtual void Release() = 0;

	virtual void Init() = 0;

	virtual void Join() = 0;

	virtual const char* GetTradingDay() = 0;


	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CXcpFtdcMdUserSpi *pSpi) = 0;

	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(const char* frontAddr) = 0;


	///�û���¼����
	virtual int ReqUserLogin(CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequstID) = 0;

	///�û��ǳ�����
	virtual int ReqUserLogout(CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID) = 0;

	///��������
	virtual int ReqSubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID) = 0;

	///�˶�����
	virtual int ReqUnsubMarketData(CXcpFtdcInstrumentField* instrument, int size, int nRequestID) = 0;

protected:
	~CXcpFtdcMdUserApi() {}
};
#endif

typedef void(__cdecl *FuncPtrOnFrontConnected) ();
typedef void(__cdecl *FuncPtrOnFrontDisconnected) (int nReason);
typedef void(__cdecl *FuncPtrOnHeartBeatWarning) (int nTimeLapse);
typedef void(__cdecl *FuncPtrOnHeartBeat) ();
typedef void(__cdecl *FuncPtrOnRspUserLogin) (CXcpFtdcRspUserLoginField* pRspUserLogin, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspUserLogout) (CXcpFtdcRspUserLogoutField* pRspUserLogout, CXcpFtdcErrorField* pRspInfo, int nRequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRtnMarketData) (CXcpFtdcMarketDataField* pMarketData);
typedef void(__cdecl *FuncPtrOnRspSubMarketData) (CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int RequestID, bool isLast);
typedef void(__cdecl *FuncPtrOnRspUnsubMarketData) (CXcpFtdcInstrumentField* pInstrument, CXcpFtdcErrorField* pRspInfo, int RequestID, bool isLast);

#ifdef __cplusplus
extern "C"
{
#endif
	typedef void* MDUserApi;
	TRADER_API_EXPORT void create_mduser(MDUserApi* mduser, const char* psw="");
	TRADER_API_EXPORT void init_mduser(MDUserApi mduser);
	TRADER_API_EXPORT void release_mduser(MDUserApi* mduser);
	


	//���׺��� Ӧ��connect��,OnRspUserLogin�ص��յ��ɹ���¼���ٵ��ã���δ��¼���÷��ش������
	TRADER_API_EXPORT const char* get_api_version(MDUserApi trader);
	TRADER_API_EXPORT const char* get_trading_day(MDUserApi trader);
	TRADER_API_EXPORT int req_user_login(MDUserApi trader, CXcpFtdcReqUserLoginField* pReqUserLogin, int nRequestID);
	TRADER_API_EXPORT int req_user_logout(MDUserApi trader, CXcpFtdcReqUserLogoutField* pReqUserLogout, int nRequestID);
	TRADER_API_EXPORT int req_sub_market_data(MDUserApi trader, CXcpFtdcInstrumentField* pInputOrder,int size,  int nRequestID);
	TRADER_API_EXPORT int req_unsub_market_data(MDUserApi trader, CXcpFtdcInstrumentField* pInstrumentID, int size,  int nRequestID);

	//���ú��� Ӧ��init_mduserǰ����
	TRADER_API_EXPORT void register_front(MDUserApi trader, const char* front_address);

    //ע��ص� Ӧ��init_mduserǰ���ã�ֻ��Ҫע����Ҫ�Ļص���
	TRADER_API_EXPORT void registerFP_OnFrontConnected(MDUserApi mduser, FuncPtrOnFrontConnected fp);
	TRADER_API_EXPORT void registerFP_OnFrontDisconnected(MDUserApi mduser, FuncPtrOnFrontDisconnected fp);
	TRADER_API_EXPORT void registerFP_OnHeartBeatWarning(MDUserApi mduser, FuncPtrOnHeartBeatWarning fp);
	TRADER_API_EXPORT void registerFP_OnHeartBeat(MDUserApi mduser, FuncPtrOnHeartBeat fp);
	TRADER_API_EXPORT void registerFP_OnRspUserLogin(MDUserApi mduser, FuncPtrOnRspUserLogin fp);
	TRADER_API_EXPORT void registerFP_OnRspUserLogout(MDUserApi mduser, FuncPtrOnRspUserLogout fp);
	TRADER_API_EXPORT void registerFP_OnRtnMarketData(MDUserApi mduser, FuncPtrOnRtnMarketData fp);
	TRADER_API_EXPORT void registerFP_OnRspSubMarketData(MDUserApi mduser, FuncPtrOnRspSubMarketData fp);
	TRADER_API_EXPORT void registerFP_OnRspUnsubMarketData(MDUserApi mduser, FuncPtrOnRspUnsubMarketData fp);

#ifdef __cplusplus
}
#endif

#endif