#include "MQClientAmpqImpl.h"

#include <exception>
#include <iostream>




ReceiveClientAmpqImpl::ReceiveClientAmpqImpl(const QueueParameter& par):
	m_host(par.host), m_port(par.port), m_user(par.user), m_password(par.password)
{}

//TODO
ReceiveClientAmpqImpl::~ReceiveClientAmpqImpl()
{
	stop();
}

void ReceiveClientAmpqImpl::registerCallback(const ReceiveCallback& callback)
{
	m_callback = callback;
}

void ReceiveClientAmpqImpl::registerDirectQueue(const std::string& queueName)
{
	m_directQueues.push_back(queueName);
}


void ReceiveClientAmpqImpl::registerFanoutExchange(const std::string& exchangeName)
{
	m_fanoutExchanges.push_back(exchangeName);
}

void ReceiveClientAmpqImpl::start()
{
	m_stopping = false;
	bool connectResult = connect();
	if (connectResult)
	{
		m_workThread = std::thread(std::bind(&ReceiveClientAmpqImpl::run, this));
	}
	
}

void ReceiveClientAmpqImpl::stop()
{
	m_stopping = true;
	if (m_workThread.joinable())
	{
		m_workThread.join();
	}
	disconnect();
}

void ReceiveClientAmpqImpl::run()
{
	Envelope::ptr_t pEnvelope;
	bool consumeResult = false;
	while (!m_stopping)
	{
		consumeResult = m_channel->BasicConsumeMessage(pEnvelope, 10);
		if (consumeResult)
		{
			//TODO  <header process and exception handle is not so good/>
			PlainHeaders headers;
			try
			{
				formatHeaders(pEnvelope->Message()->HeaderTable(), headers);
			}
			catch (...)
			{
			}
			if (m_callback)
			{
				m_callback(headers, pEnvelope->Message()->Body());
			}
		}
		
	}
}


bool ReceiveClientAmpqImpl::connect()
{
	if (m_directQueues.size() + m_fanoutExchanges.size() == 0)
		return false;
	bool connectResult = true;
	try 
	{
		m_channel = AmqpClient::Channel::Create(m_host, m_port, m_user, m_password);
		for (unsigned int i = 0; i < m_directQueues.size(); i++)
		{
			this->declareAndBasicConsumeDirectQueue(m_directQueues[i]);
		}
		for (unsigned int i = 0; i < m_fanoutExchanges.size(); i++)
		{
			this->decalreExchangeAndQueueThenConsumeQueue(m_fanoutExchanges[i]);
		}
	}
	catch (std::exception& e)
	{
		connectResult = false;
	}
	return connectResult;
}

bool ReceiveClientAmpqImpl::disconnect()
{
	if (m_channel.get())
	{
		m_channel = 0;
	}
	return true;
}

void ReceiveClientAmpqImpl::declareAndBasicConsumeDirectQueue(const std::string& queue)
{
	if (!m_channel.get())
		return;
	m_channel->DeclareQueue(queue, false, false, false, false);
	m_channel->BasicConsume(queue);
}

void ReceiveClientAmpqImpl::decalreExchangeAndQueueThenConsumeQueue(const std::string& exchange)
{
	if (!m_channel.get())
		return;
	m_channel->DeclareExchange(exchange, Channel::EXCHANGE_TYPE_FANOUT);
	std::string queueName = m_channel->DeclareQueue("", false, false, true, true);
	m_channel->BindQueue(queueName, exchange);
	m_channel->BasicConsume(queueName);
}

void ReceiveClientAmpqImpl::formatHeaders(const Table& table, PlainHeaders& headers)
{
	memset(&headers, 0, sizeof(headers));
	
	Table::const_iterator it = table.end();

	it = table.find(MSG_TYPE);
	if (it != table.end())
		headers.msg_type =  it->second.GetInt8();

	it = table.find(ADMIN_FLAG);
	if (it != table.end())
		headers.admin_flag = it->second.GetInt8();

	it = table.find(MULTI_FLAG);
	if (it != table.end())
		headers.multi_flag = it->second.GetInt8();

	it = table.find(TARGET_QUEUE);
	if (it != table.end())
		strncpy(headers.target_queue, it->second.GetString().data(), sizeof(headers.target_queue));

	it = table.find(SOURCE_SESSION);
	if (it != table.end())
		headers.source_session = it->second.GetInt32();

	it = table.find(SEQUENCE_SERIES);
	if (it != table.end())
		headers.sequence_series = it->second.GetInt32();
}


