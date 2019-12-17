#include "MdPublisher.h"
#include <fstream>
#include <ftd/settings.h>

MdPublisher::MdPublisher(const char* fname)
{
	m_parseResult = ParseCfgFile(fname);
}

void MdPublisher::Init()
{
	if (!m_parseResult)
	{
		return;
	}
	Release();
	m_pSender = SendClient::CreateClient(m_qParameter);
	m_pSender->start();

	for (int i = 0; i < m_producer_cfg_fnames.size(); i++)
	{
		MdProducerApi *pApi = MdProducerApi::CreateApi(m_producer_cfg_fnames[i].data());
		pApi->RegisterSpi(this);
		m_mdProducers.push_back(std::shared_ptr<MdProducerApi>(pApi));
	}
	for (int i = 0; i < m_mdProducers.size(); i++)
	{
		m_mdProducers[i]->Init();
	}
}

void MdPublisher::Release()
{
	for (int i = 0; i < m_mdProducers.size(); i++)
	{
		m_mdProducers[i]->Release();
	}
	m_mdProducers.clear();
	if(m_pSender.get())
		m_pSender->stop();
}



MdPublisher::~MdPublisher()
{
	Release();
}

bool MdPublisher::ParseCfgFile(const char* fname)
{
	bool parseResult = true;
	using namespace FTD;
	try
	{
		std::ifstream stream(fname);
		FTD::Settings settings(false);
		stream >> settings;
		Settings::Sections section;

		section = settings.get("QUEUE_SERVER");
		if (section.size() != 1)
			return false;
		Dictionary queueDict = section[0];
		m_qParameter.host = queueDict.getString("Host");
		m_qParameter.port = queueDict.getInt("Port");
		m_qParameter.user = queueDict.getString("User");
		m_qParameter.password = queueDict.getString("Password");

		section = settings.get("PRODUCER");
		for (auto it = section.begin(); it != section.end(); it++)
		{
			Dictionary pairDict = *it;
			std::string fpath = pairDict.getString("CfgFile");
			m_producer_cfg_fnames.push_back(fpath);
		}

		section = settings.get("PUBLISH_EXCHANGE");
		if (section.size() != 1)
			return false;
		Dictionary exchangeDict = section[0];
		m_mdExchange = exchangeDict.getString("exchange");
	}
	catch (...)
	{
		parseResult = false;
	}
	return parseResult;
}


void MdPublisher::OnNewMd(FTD::Package* pPack)
{
	DeliveryPack pack;
	pack.exchange = m_mdExchange;
	pack.routing_key = "";
	memset(&pack.plain_headers, 0, sizeof(pack.plain_headers));
	pack.plain_headers.admin_flag = QMSG_FLAG_APP;
	pack.plain_headers.msg_type = QMSG_TYPE_MARKET_DATA;

	pPack->formatFtdcMessages();
	int ftdcCount = 0;
	pPack->toSingleConcatFtdcMessage(pack.body, ftdcCount);
	if (ftdcCount > 1)
		pack.plain_headers.multi_flag = QMSG_FLAG_MULTI_FTDC;
	else
		pack.plain_headers.multi_flag = QMSG_FLAG_SINGLE_FTDC;
	m_pSender->submitTask(pack);
}