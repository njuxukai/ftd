#include "CoreServer.h"

CoreServer::CoreServer(const std::string& cfgName)
{
	m_parseResult = parseCfgFile(cfgName);
	if (m_parseResult)
	{
		m_pSender = SendClient::CreateClient(m_queueParameter);
		m_pReceiver = ReceiveClient::CreateClient(m_queueParameter);
		m_pDB = std::make_shared<McoDBWrapper>();
	}
}

CoreServer::~CoreServer()
{
	stop();
}

void CoreServer::start()
{}

void CoreServer::stop()
{}

void CoreServer::uplink(PlainHeaders& headers, FTD::PackageSPtr pPack)
{}

void CoreServer::downlink(const PlainHeaders& headers, FTD::PackageSPtr pPack)
{
}

bool CoreServer::parseCfgFile(const std::string& fname)
{
	return true;
}

