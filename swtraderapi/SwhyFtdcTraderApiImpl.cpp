#include "SwhyFtdcTraderApiImpl.h"

CSwhyFtdcTraderApiImpl::CSwhyFtdcTraderApiImpl(const char* pswDir)
{
	m_pswDir = pswDir;
	//check(and create directory) if exception m_pswDir = ""

	//open mmap files read last time trade day's last record serial number
}

CSwhyFtdcTraderApiImpl::~CSwhyFtdcTraderApiImpl()
{

}

const char* CSwhyFtdcTraderApiImpl::GetApiVersion()
{
	return "";
}

void CSwhyFtdcTraderApiImpl::Release()
{
}

void CSwhyFtdcTraderApiImpl::Init()
{
	//create initiator 
	m_pInitiator = new FTD::SocketInitator((FTD::Application&)*this, );
}

int CSwhyFtdcTraderApiImpl::Join()
{
	if (m_pInitiator != nullptr)
	{
		return 0;
	}
	return -1;
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
{}


void CSwhyFtdcTraderApiImpl::SubscribePublicTopic()
{}