#include "FrontServer.h"
#include <fstream>
#include <ftd/Settings.h>
#include <fstream>


FrontServer::FrontServer(const std::string& cfgFname)
{
	m_parseResult = parseCfgFile(cfgFname);
	if (m_parseResult)
	{
		m_pSender = SendClient::CreateClient(m_qParameter);
		m_pReceiver = ReceiveClient::CreateClient(m_qParameter);
		m_pReceiver->registerDirectQueue(m_rspQueue);
		m_pReceiver->registerFanoutExchange(m_privateExchange);
		m_pReceiver->registerFanoutExchange(m_boardcastExchange);
		//TODO 
		//register callback
		//m_pReceiver->registerCallback()
	}
	m_pRouter = std::make_shared<FtdRouter>(cfgFname);

	m_pRouter->registerUplinkFunction(std::bind(&FrontServer::uplink, this));
	m_pReceiver->registerCallback(std::bind(&FrontServer::downlink, this));
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

void FrontServer::uplink(const PlainHeaders& headers, const std::string& ftdcMsg)
{
	//called by ftd router
	DeliveryPack pack;
	pack.body = ftdcMsg;
	pack.exchange = "";
	pack.routing_key = m_reqQueue;
	memcpy(&pack.plain_headers, &headers, sizeof(PlainHeaders));
	m_pSender->submitTask(pack);
}

//在receive线程中顺序执行
void FrontServer::downlink(const PlainHeaders& headers, const std::string& ftdcMsgs)
{
	if (headers.msg_type == QMSG_TYPE_M_RSP)
	{
		std::vector<std::string> ftdcMsgs;

	}
	std::string ftdMsg;
	//called by receive client
	if (headers.msg_type == QMSG_TYPE_RSP)
	{
		Session::sendToTarget(ftdMsg, headers.source_session);
		return;
	}
	if (headers.msg_type == QMSG_TYPE_PRIVATE || headers.msg_type == QMSG_TYPE_BOARDCAST)
	{
		//交给FtdRoute处理
		return;
	}
	std::cout << "在下行处理函数中出现不该出现的消息类型" << headers.msg_type << std::endl;
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
		m_frontID = frontDict.getInt("FrontID");
		for (auto it = frontDict.begin(); it != frontDict.end(); it++)
		{
			if (it->first.find("BrokerID") == 0)
			{
				m_validBrokerIDs.insert(IntConvertor::convert(it->second));
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
		m_rspQueue = queueDict.getInt("RspQueue");
		m_privateExchange = queueDict.getString("PrivateExchange");
		m_boardcastExchange = queueDict.getString("BoardcastExchange");
	}

	catch (...)
	{
		parseResult = false;
	}
	return parseResult;
}

