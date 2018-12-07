#include "SwhyFtdcTraderApiImpl.h"
#include <ftd/SocketInitiator.h>
//#include <ftd/Port.h>

CSwhyFtdcTraderApiImpl::CSwhyFtdcTraderApiImpl(const char* pswDir)
{
	m_pswDir = pswDir;
	//check(and create directory) if exception m_pswDir = ""

	//open mmap files read last time trade day's last record serial number
}

CSwhyFtdcTraderApiImpl::~CSwhyFtdcTraderApiImpl()
{
	Release();
		
}

const char* CSwhyFtdcTraderApiImpl::GetApiVersion()
{
	return "";
}

void CSwhyFtdcTraderApiImpl::Release()
{
	m_pSpi = 0;
	if (m_pInitiator)
	{
		m_pInitiator->stop();
		delete m_pInitiator;
		m_pInitiator = 0;
	}
}

void CSwhyFtdcTraderApiImpl::Init()
{
	//1 create initiator 
	FTD::PortSettings settings;
	FTD::Dictionary defaultDictionary;
	//TODO default setting
	settings.set(defaultDictionary);
	for (auto it = m_frontAddresses.begin(); it != m_frontAddresses.end(); it++)
	{
		FTD::PortID id;
		id.fromString(*it);
		FTD::Dictionary d;
		d.setString(FTD::SOCKET_CONNECT_HOST, id.getAddress());
		d.setInt(FTD::SOCKET_CONNECT_PORT, id.getPort());
		settings.set(id, d);
	}
	FTD::ScreenLogFactory logFactory(settings);
	FTD::MemoryStoreFactory storeFactory;
	m_pInitiator = new FTD::SocketInitiator((FTD::Application&)*this, storeFactory, settings, logFactory);
	m_pInitiator->start();
}

void CSwhyFtdcTraderApiImpl::Join()
{
	if (m_pInitiator != nullptr)
	{
		m_pInitiator->join();
	}
}

const char* CSwhyFtdcTraderApiImpl::GetTradingDay()
{
	return "";
}


///注册回调接口
///@param pSpi 派生自回调接口类的实例
void CSwhyFtdcTraderApiImpl::RegisterSpi(CSwhyFtdcTraderSpi *pSpi)
{
	m_pSpi = pSpi;
}

void CSwhyFtdcTraderApiImpl::RegisterFront(const char* frontAddr)
{
	m_frontAddresses.push_back(frontAddr);
}

void CSwhyFtdcTraderApiImpl::SubscribePrivateTopic()
{
}


void CSwhyFtdcTraderApiImpl::SubscribePublicTopic()
{
}