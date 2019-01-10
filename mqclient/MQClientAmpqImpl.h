#ifndef AMPQ_CLIENT_WRAPPER_H
#define AMPQ_CLIENT_WRAPPER_H

/*
MQ Client(using rabbitmq with simpleAmqpClient)
*/

#include "MQClientWrapper.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <common/usual_macro.h>

#define MSG_TYPE "msg_type"
#define MULTI_FLAG "multi_flag"
#define TARGET_QUEUE "target_queue"
#define SOURCE_SESSION "source_session"
#define SEQUENCE_SERIES "sequence_series"



using namespace AmqpClient;

class ReceiveClientAmpqImpl : public ReceiveClient
{
public:
	ReceiveClientAmpqImpl(const QueueParameter& par);
	~ReceiveClientAmpqImpl();
	void registerCallback(const ReceiveCallback& callback);
	void registerDirectQueue(const std::string& queueName);
	void registerFanoutExchange(const std::string& exchangeName);
	void start();
	void stop();
private:
	void declareAndBasicConsumeDirectQueue(const std::string& queue);
	void decalreExchangeAndQueueThenConsumeQueue(const std::string& exchange);
	bool connect();
	bool disconnect();
	void run();
	static void formatHeaders(const Table& table, PlainHeaders& headers);
	
	DISABLE_COPY_AND_ASSIGN(ReceiveClientAmpqImpl)

private:
	std::atomic<bool> m_stopping;
	ReceiveCallback m_callback;
	std::string m_host;
	int m_port;
	std::string m_user;
	std::string m_password;
	AmqpClient::Channel::ptr_t m_channel;
	std::vector<std::string> m_directQueues;
	std::vector<std::string> m_fanoutExchanges;
	std::thread m_workThread;
};


class SendClientAmpqImpl : public SendClient
{
public:
	SendClientAmpqImpl(const QueueParameter& par);
	~SendClientAmpqImpl();
	void submitTask(const DeliveryPack& sendTask);
	void start();
	void stop();
private:
	bool connect();
	bool disconnect();
	void run();
	bool send(const DeliveryPack& task);
	static void formatTable(const PlainHeaders& headers, Table& table);

	DISABLE_COPY_AND_ASSIGN(SendClientAmpqImpl)

private:
	std::atomic<bool> m_stopping;
	std::string m_host;
	int m_port;
	std::string m_user;
	std::string m_password;
	std::thread m_workThread;
	AmqpClient::Channel::ptr_t m_channel;
	ThreadsafeQueue<DeliveryPack> m_taskQueue;
private:
	
};

#endif
