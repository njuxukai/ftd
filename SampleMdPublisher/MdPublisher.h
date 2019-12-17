#pragma once
#include <string>
#include <mqclient/MQClientWrapper.h>
#include <set>
#include <ftd/FTD30/Packages.h>
#include <mdproducer/MdProducer.h>

class MdPublisher : public MdProducerSpi
{
public:
	MdPublisher(const char* cfg_fname);

	~MdPublisher();

	void Init();

	void Release();

	virtual void OnNewMd(FTD::Package* pPack);

private:
	void init();
	bool ParseCfgFile(const char* fname);
	bool m_parseResult;
	QueueParameter m_qParameter;
	std::string m_mdExchange;
	std::vector<std::string> m_producer_cfg_fnames;

	SendClient::Sptr m_pSender;
	std::vector<std::shared_ptr<MdProducerApi>> m_mdProducers;
	
};

 