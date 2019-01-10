#pragma once

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_MQ_API_EXPORT
#define MQ_API_EXPORT __declspec(dllexport)
#else
#define MQ_API_EXPORT __declspec(dllimport)
#endif
#else
#define MQ_API_EXPORT 
#endif


#include <common/ThreadsafeQueue.h>
#include <map>
#include <functional>
#include <stdint.h>
#include <sstream>






//消息中额外头信息
//    ftd消息内容为业务层，本消息头提供补充信息
//    目的:
//      1 使得系统间传递消息逻辑简化
//      2 前置服务器得到交易服务器返回消息，无需进行解析直接转发客户端
struct PlainHeaders
{
	//0 req 1 rsp 2 private 3boardcast
	int msg_type;

	//在req消息中标记结果目标队列
	//在rsp/private/boardcast消息中无意义
	char target_queue[21];

	//在客户发起的req消息中标记前置会话编号
	//在客户发起的rsp消息中标记前置会话编号
	//在private/boardcast消息中无意义
	//在系统间的req/rsp消息中无意义
	int source_session;

	//在private/boardcast中表明私有/广播数据主题
	//   私有数据应同资金账号，广播数据=8
	//在req/rsp消息中无意义
	int sequence_series;
};

typedef std::function<void(const PlainHeaders&, const std::string& body)> ReceiveCallback;

struct SendTask
{
	std::string exchange;
	std::string routing_key;
	PlainHeaders plain_headers;
	std::string body;
};

class MQ_API_EXPORT ReceiveClient
{
public:
	typedef std::shared_ptr<ReceiveClient> Sptr;
	static Sptr CreateClient(const std::string& host = "localhost", int port = 5672, const std::string& user = "guest", const std::string& password = "guest");
	virtual ~ReceiveClient() {}
	virtual void registerCallback(const ReceiveCallback& callback) = 0;
	virtual void registerDirectQueue(const std::string& queueName) = 0;
	virtual void registerFanoutExchange(const std::string& exchangeName) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
protected:
	ReceiveClient() {}

private:
	ReceiveClient(const ReceiveClient&) = delete;
	ReceiveClient& operator=(const ReceiveClient&) = delete;

};


class MQ_API_EXPORT SendClient
{
public:
	typedef std::shared_ptr<SendClient> Sptr;
	static Sptr CreateClient(const std::string& host = "localhost", int port = 5672, const std::string& user = "guest", const std::string& password = "guest");
	
	virtual ~SendClient() {}
	virtual void submitTask(const SendTask& sendTask) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
protected:
	SendClient() {}
private:
	SendClient(const SendClient&) = delete;
	SendClient& operator=(const SendClient&) = delete;
};