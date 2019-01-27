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
	//'0' req '1' rsp '2' private '3'boardcast
	char msg_type;

	//消息是否为管理信息 '0'管理信息 '1'业务信息
	//ftd package :None 为Admin 其他为APP 
	char admin_flag;

	//body是否为多条ftdc拼接
	char multi_flag;

	//在req/rsp消息中标记放入队列(主机报盘时需要用，其他场景可选)
	//在private/boardcast消息中无意义，应为空字符串
	char source_queue[21];

	//在req消息中标记结果rsp的期望source队列
	//在rsp/private/boardcast消息中无意义，应为空字符串
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

typedef std::shared_ptr<PlainHeaders> PlainHeadersSPtr;

#define QMSG_TYPE_REQ '0'
#define QMSG_TYPE_RSP '1'
#define QMSG_TYPE_PRIVATE '2'
#define QMSG_TYPE_BOARDCAST '3'

#define QMSG_FLAG_MULTI_FTDC '0'
#define QMSG_FLAG_SINGLE_FTDC '1'

#define QMSG_FLAG_ADMIN '0'
#define QMSG_FLAG_APP  '1'



typedef std::function<void(const PlainHeaders&, const std::string& body)> 
		ReceiveCallback;

struct DeliveryPack
{
	std::string exchange;
	std::string routing_key;
	PlainHeaders plain_headers;
	std::string body;
};

struct QueueParameter
{
	std::string host;
	int port;
	std::string user;
	std::string password;
};

class MQ_API_EXPORT ReceiveClient
{
public:
	typedef std::shared_ptr<ReceiveClient> Sptr;
	static Sptr CreateClient(const QueueParameter& parameter);
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
	static Sptr CreateClient(const QueueParameter& parameter);
	
	virtual ~SendClient() {}
	virtual void submitTask(const DeliveryPack& sendTask) = 0;
	virtual void registerDirectQueue(const std::string& queueName) = 0;
	virtual void registerFanoutExchange(const std::string& exchangeName) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
protected:
	SendClient() {}
private:
	SendClient(const SendClient&) = delete;
	SendClient& operator=(const SendClient&) = delete;
};