#include "stdafx.h"
#include "ReportServer.h"
#include <fstream>
#include <ftd/settings.h>

ReportServer::ReportServer(const std::string& cfgName)
{
	m_cfgFname = cfgName;
	m_parseResult = parseCfgFile(cfgName);
	if (m_parseResult)
	{
		init();
	}
}

ReportServer::~ReportServer()
{
	stop();
}

void ReportServer::start()
{
	if (m_parseResult)
	{
		m_pSender->start();
		m_pReceiver->start();
	}
}

void ReportServer::stop()
{
	if (m_parseResult)
	{
		m_pSender->stop();
		m_pReceiver->stop();
	}
}

//被报盘dll调用，需要线程安全
void ReportServer::reporterUplinkCallback(FTD::PackageSPtr pPackage)
{
	
	
}

//简单的链接到DBWrapper
void ReportServer::submitTaskToReporter(FTD::PackageSPtr pPack)
{
	if(m_pReporter)
		m_pReporter->submit(pPack);
}

//被m_pReceiver调用
void ReportServer::queueReceiveCallback(const PlainHeaders& headers, const std::string& body)
{
	if (m_queueBuffers.find(headers.source_queue) == m_queueBuffers.end())
		return;
	if (headers.multi_flag == QMSG_FLAG_MULTI_FTDC)
	{
		std::vector<std::string> ftdcMessages;
		FtdMessageUtil::splitFtdcMessages(body, ftdcMessages);
		for (unsigned int i = 0; i < ftdcMessages.size(); i++)
		{
			FTD::Package* pack = m_queueBuffers[headers.source_queue]->OnFtdcMessage(ftdcMessages[i]);
			if (pack)
				submitTaskToReporter(FTD::PackageSPtr(pack->clone()));
		}
	}
	if (headers.multi_flag == QMSG_FLAG_SINGLE_FTDC)
	{
		FTD::Package* pack = m_queueBuffers[headers.source_queue]->OnFtdcMessage(body);
		if (pack)		
			submitTaskToReporter(FTD::PackageSPtr(pack->clone()));
	}	
}

void ReportServer::init()
{
	m_queueBuffers.clear();
	for (auto it = m_readQueues.begin(); it != m_readQueues.end(); it++)
	{
		m_queueBuffers[*it] = std::make_shared<FTD::PackageBuffer>();
	}

	m_pSender = SendClient::CreateClient(m_queueParameter);
	m_pReceiver = ReceiveClient::CreateClient(m_queueParameter);

	m_pReporter = ReporterWrapper::CreateWrapper(m_cfgFname);

	for (auto it = m_readQueues.begin(); it != m_readQueues.end(); it++)
	{
		m_pReceiver->registerDirectQueue(*it);
	}
	for (auto it = m_writeQueues.begin(); it != m_writeQueues.end(); it++)
	{
		m_pSender->registerDirectQueue(*it);
	}
	m_pReporter->registerUplinkCallback(std::bind(&ReportServer::reporterUplinkCallback, this, 
		std::placeholders::_1));

	m_pReceiver->registerCallback(std::bind(&ReportServer::queueReceiveCallback, this,
		std::placeholders::_1, std::placeholders::_2));
}

bool ReportServer::parseCfgFile(const std::string& fname)
{
	using namespace FTD;
	bool parseResult = true;
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
		m_queueParameter.host = queueDict.getString("Host");
		m_queueParameter.port = queueDict.getInt("Port");
		m_queueParameter.user = queueDict.getString("User");
		m_queueParameter.password = queueDict.getString("Password");

		section = settings.get("RPT_BACK_QUEUE_PAIR");
		for (auto it = section.begin(); it != section.end(); it++)
		{
			Dictionary pairDict = *it;
			std::string reqQueue = pairDict.getString("ReqQueue");
			std::string rspQueue = pairDict.getString("RspQueue");
			m_writeQueues.insert(reqQueue);
			m_readQueues.insert(rspQueue);			
			m_rptBackQueuePairs[reqQueue] = rspQueue;
		}

		section = settings.get("RPT_QUEUE_PAIR");
		for (auto it = section.begin(); it != section.end(); it++)
		{
			Dictionary pairDict = *it;
			std::string reqQueue = pairDict.getString("ReqQueue");
			std::string rspQueue = pairDict.getString("RspQueue");
			m_readQueues.insert(reqQueue);
			m_writeQueues.insert(rspQueue);
			m_rptQueuePairs[reqQueue] = rspQueue;
		}		
	}
	catch (...)
	{
		parseResult = false;
	}
	return parseResult;
}

