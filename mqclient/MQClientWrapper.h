#pragma once

#include <common/ThreadsafeQueue.h>
#include <map>
#include <functional>
#include <stdint.h>
#include <sstream>
#include <windows.h>

#define TARGET_QUEUE "target_queue"

static uint64_t now_microseconds(void) {
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return (((uint64_t)ft.dwHighDateTime << 32) | (uint64_t)ft.dwLowDateTime) /
		10;
}

struct PlainHeaders
{
	std::string rsp_target_queue;
};

typedef std::function<void(const PlainHeaders&, const std::string& body)> ReceiveCallback;

struct SendTask
{
	std::string exchange;
	std::string routing_key;
	PlainHeaders plain_headers;
	std::string body;
};

class ReceiveClientImpl;

class ReceiveClient
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


class SendClient
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