#pragma once

#include <common/usual_macro.h>
#include <mqclient/MQClientWrapper.h>
#include <reporter/ReporterWrapper.h>
#include <set>
#include <ftd/FTD30/Packages.h>

class ReportServer
{
public:
	ReportServer(const std::string& cfgFname);
	~ReportServer();
	void start();
	void stop();

private:
	//submit task to db
	void submitTaskToReporter(FTD::PackageSPtr ftdcMsg);
	//called by dbwrapper
	void reporterUplinkCallback(FTD::PackageSPtr pPackage);
	//called by queue receiver
	void queueReceiveCallback(const PlainHeaders& headers, const std::string& body);
	bool parseCfgFile(const std::string& fname);
	void init();
	bool m_parseResult;

	QueueParameter m_queueParameter;
	std::set<std::string> m_writeQueues;
	std::set<std::string> m_readQueues;
	std::pair<std::string, std::string> m_rptQueuePair;
	std::pair<std::string, std::string> m_rptBackQueuePair;
	SendClient::Sptr m_pSender;
	ReceiveClient::Sptr m_pReceiver;
	std::shared_ptr<ReporterWrapper> m_pReporter;
	std::map<std::string, std::shared_ptr<FTD::PackageBuffer>> m_queueBuffers;

	std::string m_cfgFname;
};