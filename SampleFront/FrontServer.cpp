#include "stdafx.h"
#include "FrontServer.h"
#include <fstream>
#include <ftd/Settings.h>



FrontServer::FrontServer(const std::string& cfgFname)
{
	m_parseResult = parseCfgFile(cfgFname);
	m_routeParameter.cfgFname = cfgFname;
	if (m_parseResult)
	{
		m_pSender = SendClient::CreateClient(m_qParameter);
		m_pReceiver = ReceiveClient::CreateClient(m_qParameter);
		m_pReceiver->registerDirectQueue(m_rspQueue);
		m_pReceiver->registerFanoutExchange(m_privateExchange);
		m_pReceiver->registerFanoutExchange(m_boardcastExchange);

		m_pRouter = std::make_shared<FtdRouter>(m_routeParameter);
		m_pRouter->registerUplinkCallback(std::bind(&FrontServer::routerUplinkCallback, this,
			std::placeholders::_1, std::placeholders::_2));
		m_pReceiver->registerCallback(std::bind(&FrontServer::queueReceiveCallback, this,
			std::placeholders::_1, std::placeholders::_2));
	}
}


FrontServer::~FrontServer()
{
	stop();
}


//TODO 启动过程应分阶段返回状态，如果启动失败应显式报错，待改进
void FrontServer::start()
{
	m_pSender->start();
	m_pReceiver->start();
	m_pRouter->start();
	
}

void FrontServer::stop()
{
	m_pRouter->stop();
	m_pReceiver->stop();
	m_pSender->stop();
}

//在acceptor的工作线程上执行
void FrontServer::routerUplinkCallback(PlainHeaders& headers, const std::string& ftdcMsg)
{
	DeliveryPack pack;
	//headers & body
	//注意 如果要实现当前库和历史库的分离，则需要在Router内指定target_queue, source_queue
	strncpy(headers.target_queue, m_rspQueue.data(), sizeof(headers.target_queue));
	strncpy(headers.source_queue, m_reqQueue.data(), sizeof(headers.source_queue));
	memcpy(&pack.plain_headers, &headers, sizeof(PlainHeaders));
	pack.body = ftdcMsg;
	//exchange & routingKey
	pack.exchange = "";
	pack.routing_key = m_reqQueue;	
	m_pSender->submitTask(pack);
}

//在receive线程中顺序执行
void FrontServer::queueReceiveCallback(const PlainHeaders& headers, const std::string& ftdcMsgs)
{	
	m_pRouter->processDownlink(headers, ftdcMsgs);
}


bool FrontServer::parseCfgFile(const std::string& fname)
{
	bool parseResult = true;
	try
	{
		std::ifstream stream(fname);
		Settings settings(false);
		stream >> settings;
		Settings::Sections section;

		section = settings.get("FRONT");
		if (section.size() == 0)
			return false;
		Dictionary frontDict = section[0];
		m_routeParameter.frontID = frontDict.getInt("FrontID");
		for (auto it = frontDict.begin(); it != frontDict.end(); it++)
		{
			if (it->first.find("BrokerID") == 0)
			{
				m_routeParameter.allowedSessionIDs.insert(IntConvertor::convert(it->second));
			}
		}
		section = settings.get("QUEUE");
		if (section.size() == 0)
			return false;
		Dictionary queueDict = section[0];
		m_qParameter.host = queueDict.getString("Host");
		m_qParameter.port = queueDict.getInt("Port");
		m_qParameter.user = queueDict.getString("User");
		m_qParameter.password = queueDict.getString("Password");

		m_reqQueue = queueDict.getString("ReqQueue");
		m_rspQueue = queueDict.getString("RspQueue");
		m_privateExchange = queueDict.getString("PrivateExchange");
		m_boardcastExchange = queueDict.getString("BoardcastExchange");
	}

	catch (...)
	{
		parseResult = false;
	}
	return parseResult;
}

