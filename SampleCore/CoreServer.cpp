#include "stdafx.h"
#include "CoreServer.h"
#include <fstream>
#include <ftd/settings.h>
CoreServer::CoreServer(const std::string& cfgName)
{
	m_parseResult = parseCfgFile(cfgName);
	if (m_parseResult)
	{
		init();
	}
}

CoreServer::~CoreServer()
{
	stop();
}

void CoreServer::start()
{
	if (m_parseResult)
	{
		m_pSender->start();
		m_pReceiver->start();
	}
}

void CoreServer::stop()
{
	if (m_parseResult)
	{
		m_pSender->stop();
		m_pReceiver->stop();
	}
}

//被pDB的线程池调用，需要线程安全
void CoreServer::dbUplinkCallback(PlainHeaders& headers, FTD::PackageSPtr pPackage)
{
	//报盘请求，需要补充报盘Rsp的期望队列 headers.target_queue
	if (headers.msg_type == QMSG_TYPE_REQ)
	{
		if (m_rpcQueuePairs.find(headers.source_queue) != m_rpcQueuePairs.end())
		{
			strncpy(headers.target_queue, m_rpcQueuePairs[headers.source_queue].data(), 
				sizeof(headers.target_queue));
		}
		else
		{
			//TODO ERROR HANDLE
			return;
		}
	}
	DeliveryPack pack;
	memcpy(&pack.plain_headers, &headers, sizeof(PlainHeaders));
	if (headers.msg_type == QMSG_TYPE_REQ || headers.msg_type == QMSG_TYPE_RSP)
	{
		pack.exchange = "";
		pack.routing_key = headers.source_queue;
	}
	if (headers.msg_type == QMSG_TYPE_PRIVATE)
	{
		pack.exchange = m_privateExchange;
		pack.routing_key = "";
	}
	if (headers.msg_type == QMSG_TYPE_BOARDCAST)
	{
		pack.exchange = m_boardcastExchange;
		pack.routing_key = "";
	}

	//Important
	pPackage->formatFtdcMessages();

	if (m_ftdcMulitFlag)
	{
		int ftdcCount = 0;
		pPackage->toSingleConcatFtdcMessage(pack.body, ftdcCount);
		if (ftdcCount > 1)
			pack.plain_headers.multi_flag = QMSG_FLAG_MULTI_FTDC;
		else
			pack.plain_headers.multi_flag = QMSG_FLAG_SINGLE_FTDC;
		m_pSender->submitTask(pack);
	}
	else
	{
		//在客户端有可能出现同一个tid的多个package的FtdMessage乱序
		//TODO 要保证正确性，当ftdcMessages.size()>0时 应该加锁
		std::vector<std::string> ftdcMessages;
		pPackage->toFtdcMessages(ftdcMessages);
		pack.plain_headers.multi_flag = QMSG_FLAG_SINGLE_FTDC;
		for (unsigned int i = 0; i < ftdcMessages.size(); i++)
		{
			DeliveryPack newPack = pack;
			newPack.body = ftdcMessages[i];
			m_pSender->submitTask(newPack);
		}
	}
	
}

//简单的链接到DBWrapper
void CoreServer::submitTaskToDB(const PlainHeaders& headers, FTD::PackageSPtr pPack)
{
	if(m_pDB)
		m_pDB->submit(headers, pPack);
}

//被m_pReceiver调用
void CoreServer::queueReceiveCallback(const PlainHeaders& headers, const std::string& body)
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
				submitTaskToDB(headers, FTD::PackageSPtr(pack->clone()));
		}
	}
	if (headers.multi_flag == QMSG_FLAG_SINGLE_FTDC)
	{
		FTD::Package* pack = m_queueBuffers[headers.source_queue]->OnFtdcMessage(body);
		if (pack)		
			submitTaskToDB(headers, FTD::PackageSPtr(pack->clone()));
	}	
}

void CoreServer::init()
{
	m_queueBuffers.clear();
	for (auto it = m_listenQueues.begin(); it != m_listenQueues.end(); it++)
	{
		m_queueBuffers[*it] = std::make_shared<FTD::PackageBuffer>(true);
	}

	m_pSender = SendClient::CreateClient(m_queueParameter);
	m_pReceiver = ReceiveClient::CreateClient(m_queueParameter);
	m_pDB = std::shared_ptr<DBWrapper>(DBWrapper::CreateWrapper());
	for (auto it = m_listenQueues.begin(); it != m_listenQueues.end(); it++)
	{
		m_pReceiver->registerDirectQueue(*it);
	}
	m_pDB->registerUplinkCallback(std::bind(&CoreServer::dbUplinkCallback, this, 
		std::placeholders::_1, std::placeholders::_2));
	m_pReceiver->registerCallback(std::bind(&CoreServer::queueReceiveCallback, this,
		std::placeholders::_1, std::placeholders::_2));
}

bool CoreServer::parseCfgFile(const std::string& fname)
{
	using namespace FTD;
	bool parseResult = true;
	try
	{
		std::ifstream stream(fname);
		FTD::Settings settings(false);
		stream >> settings;
		Settings::Sections section;
		Dictionary dict;

		section = settings.get("QUEUE_SERVER");
		if (section.size() != 1)
			return false;
		Dictionary queueDict = section[0];
		m_queueParameter.host = queueDict.getString("Host");
		m_queueParameter.port = queueDict.getInt("Port");
		m_queueParameter.user = queueDict.getString("User");
		m_queueParameter.password = queueDict.getString("Password");

		m_rpcQueuePairs.clear();
		m_listenQueues.clear();
		section = settings.get("LISTEN_QUEUES");
		if (section.size() != 1)
			return false;
		Dictionary listenQueueDict = section[0];
		for (auto it = listenQueueDict.begin(); it != listenQueueDict.end(); it++)
		{
			m_listenQueues.insert(it->second);
		}
		section = settings.get("RPC_QUEUE_PAIR");
		for (auto it = section.begin(); it != section.end(); it++)
		{
			Dictionary pairDict = *it;
			std::string reqQueue = pairDict.getString("ReqQueue");
			std::string rspQueue = pairDict.getString("RspQueue");
			m_listenQueues.insert(rspQueue);
			m_rpcQueuePairs[reqQueue] = rspQueue;
		}

		section = settings.get("FANOUT_EXCHANGES");
		if (section.size() != 1)
			return false;
		Dictionary exchangeDict = section[0];
		m_privateExchange = exchangeDict.getString("PrivateExchange");
		m_boardcastExchange = exchangeDict.getString("BoardcastExchange");

		section = settings.get("CONTROL");
		if (section.size() != 1)
			return false;
		Dictionary controlDict = section[0];
		m_ftdcMulitFlag = controlDict.getBool("AllowMultiFtdcPerMsg");
	}
	catch (...)
	{
		parseResult = false;
	}
	return parseResult;
}

