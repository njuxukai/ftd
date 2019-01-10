#ifndef AMPQ_CLIENT_WRAPPER_H
#define AMPQ_CLIENT_WRAPPER_H

/*
MQ Client(using rabbitmq with simpleAmqpClient)
*/

#include "MQClientWrapper.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>

using namespace AmqpClient;

class ReceiveClientAmpqImpl : public ReceiveClient
{
public:
	ReceiveClientAmpqImpl(const std::string& host, int port, const std::string& user, const std::string& password);
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
	ReceiveClientAmpqImpl(const ReceiveClientAmpqImpl&) = delete;
	ReceiveClientAmpqImpl& operator=(const ReceiveClientAmpqImpl&) = delete;

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
	SendClientAmpqImpl(const std::string& host, int port, const std::string& user, const std::string& password);
	~SendClientAmpqImpl();
	void submitTask(const SendTask& sendTask);
	void start();
	void stop();
private:
	bool connect();
	bool disconnect();
	void run();
	bool send(const SendTask& task);
	static void formatTable(const PlainHeaders& headers, Table& table);

	SendClientAmpqImpl(const SendClientAmpqImpl&) = delete;
	SendClientAmpqImpl& operator=(const SendClientAmpqImpl&) = delete;
private:
	std::atomic<bool> m_stopping;
	std::string m_host;
	int m_port;
	std::string m_user;
	std::string m_password;
	std::thread m_workThread;
	AmqpClient::Channel::ptr_t m_channel;
	ThreadsafeQueue<SendTask> m_taskQueue;
private:
	
};

#endif
