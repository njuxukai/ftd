#include "MQClientAmpqImpl.h"
#include <iostream>

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <amqp.h>
#include <amqp_framing.h>
#include <atomic>
#include <thread>


SendClientAmpqImpl::SendClientAmpqImpl(const std::string& host, int port, const std::string& user, const std::string& password):
	m_host(host),
	m_port(port),
	m_user(user),
	m_password(password)
{

}

SendClientAmpqImpl::~SendClientAmpqImpl()
{}

void SendClientAmpqImpl::submitTask(const SendTask& sendTask)
{
	m_taskQueue.push(std::move(sendTask));
}

void SendClientAmpqImpl::start()
{
	m_stopping = false;
	bool connectResult = connect();
	if (connectResult)
	{
		m_workThread = std::thread(std::bind(&SendClientAmpqImpl::run, this));
	}	
}

void SendClientAmpqImpl::stop()
{
	m_stopping = true;
	if (m_workThread.joinable())
	{
		m_workThread.join();
	}
	disconnect();
}

void SendClientAmpqImpl::run()
{
	while (!m_stopping || !m_taskQueue.empty())
	{
		std::shared_ptr<SendTask> task = m_taskQueue.wait_and_pop(20);
		if (task.get())
		{
			send(*task);
		}
	}
}

bool SendClientAmpqImpl::connect()
{
	bool connectResult = true;
	try
	{
		m_channel = AmqpClient::Channel::Create(m_host, m_port, m_user, m_password);
	}
	catch (...)
	{
		connectResult = false;
	}
	return connectResult;
}

bool SendClientAmpqImpl::disconnect()
{
	m_channel = 0;
	return true;
}

bool SendClientAmpqImpl::send(const SendTask& task)
{
	if (!m_channel.get())
		return false;
	Table table;
	formatTable(task.plain_headers, table);
	BasicMessage::ptr_t pMsg = BasicMessage::Create();
	pMsg->Body(task.body);
	pMsg->HeaderTable(table);
	//TO REMOVE
	std::cout <<"At send " << now_microseconds() << std::endl;
	m_channel->BasicPublish(task.exchange, task.routing_key, pMsg, false, false);
	return true;
}

void SendClientAmpqImpl::formatTable(const PlainHeaders& headers, Table& table)
{
	table.clear();
	table["TARGET_QUEUE"] = "";
}