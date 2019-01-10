#include "MQClientWrapper.h"
#include "MQClientAmpqImpl.h"

ReceiveClient::Sptr ReceiveClient::CreateClient(const QueueParameter& par)
{
	return std::make_shared<ReceiveClientAmpqImpl>(par);
}

SendClient::Sptr SendClient::CreateClient(const QueueParameter& par)
{
	return std::make_shared<SendClientAmpqImpl>(par);
}