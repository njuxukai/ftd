#ifndef SWHY_FTDC_TRADER_API_IMPL_H
#define SWHY_FTDC_TRADER_API_IMPL_H

#include "SwhyFtdcTraderApi.h"
#include "Initiator.h"
#include "Application.h"
#include "FTD20/PackageCracker.h"

class CSwhyFtdcTraderApiImpl : public CSwhyFtdcTraderApi, 
	public FTD::Application, public FTD::PackageCracker
{
public:
	CSwhyFtdcTraderApiImpl(const char* pswDir);

	virtual const char* GetApiVersion();

	virtual void Release();

	virtual void Init();

	virtual int Join();

	virtual const char* GetTradingDay();


	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CSwhyFtdcTraderSpi *pSpi);

	virtual void RegisterFront(const char* frontAddr);

	virtual void SubscribePrivateTopic();


	virtual void SubscribePublicTopic();
private:
	std::string m_pswDir;
	std::vector<std::string> m_frontAddresses;
	CSwhyFtdcTraderSpi* m_pSpi;
	FTD::Initiator* m_pInitiator;
	FTD::Session* m_pSession;
};
#endif