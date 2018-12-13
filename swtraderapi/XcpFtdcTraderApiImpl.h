#ifndef XCP_FTDC_TRADER_API_IMPL_H
#define XCP_FTDC_TRADER_API_IMPL_H

#include "XcpFtdcTraderApi.h"
#include <ftd/Application.h>
#include <ftd/Initiator.h>
#include <ftd/FTD30/PackageCracker.h>

class CXcpFtdcTraderApiImpl : public CXcpFtdcTraderApi, 
	public FTD::Application, public FTD::PackageCracker
{
public:
	CXcpFtdcTraderApiImpl(const char* pswDir);

	virtual const char* GetApiVersion();

	virtual void Release();

	virtual void Init();

	virtual void Join();

	virtual const char* GetTradingDay();


	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CXcpFtdcTraderSpi *pSpi);

	virtual void RegisterFront(const char* frontAddr);

	virtual void SubscribePrivateTopic();

	virtual void SubscribePublicTopic();

    //


	//
private:
	std::string m_pswDir;
	std::vector<std::string> m_frontAddresses;
	CXcpFtdcTraderSpi* m_pSpi;
	FTD::Initiator* m_pInitiator;
};
#endif