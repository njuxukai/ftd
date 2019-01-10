#include "MQClientWrapper.h"
#include "MQClientAmpqImpl.h"

ReceiveClient::Sptr ReceiveClient::CreateClient(const std::string& host, int port, const std::string& user, const std::string& password)
{
	return std::make_shared<ReceiveClientAmpqImpl>(host, port, user, password);
}

SendClient::Sptr SendClient::CreateClient(const std::string& host, int port, const std::string& user, const std::string& password)
{
	return std::make_shared<SendClientAmpqImpl>(host, port, user, password);
}