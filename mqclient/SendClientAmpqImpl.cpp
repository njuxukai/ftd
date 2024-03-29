#include "MQClientAmpqImpl.h"
#include <iostream>

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <atomic>
#include <thread>


SendClientAmpqImpl::SendClientAmpqImpl(const QueueParameter& par):
	m_host(par.host),
	m_port(par.port),
	m_user(par.user),
	m_password(par.password)
{

}

SendClientAmpqImpl::~SendClientAmpqImpl()
{
	stop();
}

void SendClientAmpqImpl::submitTask(const DeliveryPack& sendTask)
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
		std::shared_ptr<DeliveryPack> task = m_taskQueue.wait_and_pop(20);
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
		for (auto it = m_directQueues.begin(); it != m_directQueues.end(); it++)
		{
			declareDirectQueue(*it);
		}
		for (auto it = m_fanoutExchanges.begin(); it != m_fanoutExchanges.end(); it++)
		{
			declareExchange(*it);
		}
	}
	catch (std::exception& e)
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

bool SendClientAmpqImpl::send(const DeliveryPack& task)
{
	if (!m_channel.get())
		return false;
	Table table;
	formatTable(task.plain_headers, table);
	BasicMessage::ptr_t pMsg = BasicMessage::Create();
	pMsg->Body(task.body);
	pMsg->HeaderTable(table);
	//TO REMOVE
	m_channel->BasicPublish(task.exchange, task.routing_key, pMsg, false, false);
	return true;
}

void SendClientAmpqImpl::formatTable(const PlainHeaders& headers, Table& table)
{
	table.clear();
	table[MSG_TYPE] = (boost::int8_t)headers.msg_type;
	table[ADMIN_FLAG] = (boost::int8_t)headers.admin_flag;
	table[MULTI_FLAG] = (boost::int8_t)headers.multi_flag;
	table[SOURCE_QUEUE] = headers.source_queue;
	table[TARGET_QUEUE] = headers.target_queue;
	table[SOURCE_SESSION] = (boost::int32_t)headers.source_session;
	table[SEQUENCE_SERIES] = (boost::int32_t)headers.sequence_series;
}

void SendClientAmpqImpl::registerDirectQueue(const std::string& queueName)
{
	m_directQueues.insert(queueName);
}

void SendClientAmpqImpl::registerFanoutExchange(const std::string& exchangeName)
{
	m_fanoutExchanges.insert(exchangeName);
}

void SendClientAmpqImpl::declareDirectQueue(const std::string& queue)
{
	if (!m_channel.get())
		return;
	m_channel->DeclareQueue(queue, false, false, false, false);
}

void SendClientAmpqImpl::declareExchange(const std::string& exchange)
{
	if (!m_channel.get())
		return;
	m_channel->DeclareExchange(exchange, Channel::EXCHANGE_TYPE_FANOUT);
}