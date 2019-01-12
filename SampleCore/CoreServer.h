#pragma once

#include <common/usual_macro.h>
#include <mqclient/MQClientWrapper.h>
#include <dbcore/DBWrapper.h>
#include <set>
#include <ftd/FTD30/Packages.h>
class CoreServer
{
public:
	CoreServer(const std::string& cfgFname);
	~CoreServer();
	void start();
	void stop();

private:
	
	void downlink(const PlainHeaders& headers, FTD::PackageSPtr ftdcMsg);
	void uplink(PlainHeaders& headers, FTD::PackageSPtr pPackage);
	//called by receiver
	void receiveCallback(const PlainHeaders& headers, const std::string& body);
	bool parseCfgFile(const std::string& fname);
	bool m_parseResult;

	QueueParameter m_queueParameter;
	std::set<std::string> m_listenQueues;
	std::map<std::string, std::string> m_rpcQueuePairs;
	std::string m_boardcastExchange;
	std::string m_privateExchange;
	bool m_ftdcMulitFlag;

	SendClient::Sptr m_pSender;
	ReceiveClient::Sptr m_pReceiver;
	std::shared_ptr<DBWrapper> m_pDB;
	std::map<std::string, std::shared_ptr<FTD::PackageBuffer>> m_queueBuffers;
};