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






//��Ϣ�ж���ͷ��Ϣ
//    ftd��Ϣ����Ϊҵ��㣬����Ϣͷ�ṩ������Ϣ
//    Ŀ��:
//      1 ʹ��ϵͳ�䴫����Ϣ�߼���
//      2 ǰ�÷������õ����׷�����������Ϣ��������н���ֱ��ת���ͻ���
struct PlainHeaders
{
	//0 req 1 rsp 2 private 3boardcast
	int msg_type;

	//��req��Ϣ�б�ǽ��Ŀ�����
	//��rsp/private/boardcast��Ϣ��������
	char target_queue[21];

	//�ڿͻ������req��Ϣ�б��ǰ�ûỰ���
	//�ڿͻ������rsp��Ϣ�б��ǰ�ûỰ���
	//��private/boardcast��Ϣ��������
	//��ϵͳ���req/rsp��Ϣ��������
	int source_session;

	//��private/boardcast�б���˽��/�㲥��������
	//   ˽������Ӧͬ�ʽ��˺ţ��㲥����=8
	//��req/rsp��Ϣ��������
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