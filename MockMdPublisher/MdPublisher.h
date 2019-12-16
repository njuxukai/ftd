#pragma once
#include <string>
#include <mqclient/MQClientWrapper.h>
#include <set>
#include <ftd/FTD30/Packages.h>

class MdPublisher
{
public:
	MdPublisher(const std::string& cfg_fname);

	~MdPublisher();

	void start();

	void stop();

private:
	void mdUplinkCallback(FTD::PackageSPtr pPack);
	bool parseCfgFile(const std::string& fname);
	void init();
	bool m_parseResult;

	QueueParameter m_queueParameter;
	std::string m_mdExchange;
	SendClient::Sptr m_pSender;
	std::shared_ptr<MdProducer> m_mdProducer;
	std::string m_cfgFname;
};

 