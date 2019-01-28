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
	//submit task to db
	void submitTaskToDB(const PlainHeaders& headers, FTD::PackageSPtr ftdcMsg);
	//called by dbwrapper
	void dbUplinkCallback(PlainHeaders& headers, FTD::PackageSPtr pPackage);
	//called by queue receiver
	void queueReceiveCallback(const PlainHeaders& headers, const std::string& body);
	bool parseCfgFile(const std::string& fname);
	void init();
	bool m_parseResult;

	QueueParameter m_queueParameter;
	std::set<std::string> m_writeQueues;
	std::set<std::string> m_readQueues;
	std::map<std::string, std::string> m_frontQueuePairs;
	std::map<std::string, std::string> m_rptQueuePairs;
	std::map<std::string, std::string> m_rptBackQueuePairs;
	std::string m_boardcastExchange;
	std::string m_privateExchange;
	bool m_ftdcMulitFlag;

	SendClient::Sptr m_pSender;
	ReceiveClient::Sptr m_pReceiver;
	std::shared_ptr<DBWrapper> m_pDB;
	std::map<std::string, std::shared_ptr<FTD::PackageBuffer>> m_queueBuffers;
};