#pragma once

#include <common/usual_macro.h>
#include <mqclient/MQClientWrapper.h>
#include "FtdRouter.h"


class FrontServer
{
public:
	FrontServer(const std::string& cfgFname);
	~FrontServer();
	void start();
	void stop();
private:
	void uplink(PlainHeaders& headers, const std::string& ftdcMsg);
	void downlink(const PlainHeaders& headers, const std::string& ftdcMsg);
	bool parseCfgFile(const std::string& fname);
	bool m_parseResult;
	SendClient::Sptr m_pSender;
	ReceiveClient::Sptr m_pReceiver;
	std::shared_ptr<FtdRouter> m_pRouter;
	
	//AppID
	FtdRouterParameter m_routeParameter;
	//
	QueueParameter m_qParameter;
	std::string m_reqQueue;
	std::string m_rspQueue;
	std::string m_privateExchange;
	std::string m_boardcastExchange;

	DISABLE_COPY_AND_ASSIGN(FrontServer)
};