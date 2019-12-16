#pragma once

#include <common/usual_macro.h>
#include <mqclient/MQClientWrapper.h>
#include "FtdRouter.h"
#include <set>

class FrontServer
{
public:
	FrontServer(const std::string& cfgFname);
	~FrontServer();
	void start();
	void stop();
private:
	void routerUplinkCallback(PlainHeaders& headers, const std::string& ftdcMsg);
	void queueReceiveCallback(const PlainHeaders& headers, const std::string& ftdcMsg);
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
	//fan out exchanges
	std::set<std::string> m_subExchanges;
	DISABLE_COPY_AND_ASSIGN(FrontServer)
};