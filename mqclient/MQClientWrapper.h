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
	//'0' req '1' rsp '2' private '3'boardcast
	char msg_type;

	//��Ϣ�Ƿ�Ϊ������Ϣ '0'������Ϣ '1'ҵ����Ϣ
	//ftd package :None ΪAdmin ����ΪAPP 
	char admin_flag;

	//body�Ƿ�Ϊ����ftdcƴ��
	char multi_flag;

	//��req/rsp��Ϣ�б�Ƿ������(��������ʱ��Ҫ�ã�����������ѡ)
	//��private/boardcast��Ϣ�������壬ӦΪ���ַ���
	char source_queue[21];

	//��req��Ϣ�б�ǽ��rsp������source����
	//��rsp/private/boardcast��Ϣ�������壬ӦΪ���ַ���
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