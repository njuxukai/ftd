#ifndef DB_WRAPPER_H_
#define DB_WRAPPER_H_

#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <thread>
#include <functional>
#include <chrono> 
#include <condition_variable>
#include <queue>

#include <ftd/FTD30/packages.h>
#include <ftdc_all.h>

using namespace FTD;

#define  MAX_DEVICES           10
#define  N_DEVICES             4

template<typename T>
class ThreadsafeQueue
{
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	ThreadsafeQueue()
	{}

	void push(T data)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::move(data));
		data_cond.notify_one();
	}

	void wait_and_pop(T& value, int milliSeconds)
	{
		std::unique_lock<std::mutex> lk(mut);
		bool wait_result = data_cond.wait_for(lk, std::chrono::milliseconds(milliSeconds),
			[this] {return !data_queue.empty(); });
		if (wait_result)
		{
			value = std::move(data_queue.front());
			data_queue.pop();
		}
	}

	std::shared_ptr<T> wait_and_pop(int milliSeconds)
	{
		std::unique_lock<std::mutex> lk(mut);
		bool wait_result = data_cond.wait_for(lk, std::chrono::milliseconds(milliSeconds),
			[this] {return !data_queue.empty(); });
		std::shared_ptr<T> res;
		if (wait_result)
		{
			res = std::make_shared<T>(std::move(data_queue.front()));
			data_queue.pop();
		}
		return res;
	}

	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(
			std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};


class JoinThreads
{
	std::vector<std::thread>& threads;
public:
	explicit JoinThreads(std::vector<std::thread>& threads_) :
		threads(threads_)
	{}

	~JoinThreads()
	{
		for (unsigned long i = 0; i<threads.size(); ++i)
		{
			if (threads[i].joinable())
				threads[i].join();
		}
	}
};

class McoDBWrapper
{
public:
	typedef std::function<void(PackageSPtr)> Callback;
	McoDBWrapper();
	~McoDBWrapper();
	void submit(PackageSPtr pReq);
	void registerResponseCallback(Callback callback);
private:
	bool m_done;
	ThreadsafeQueue<PackageSPtr> m_reqQueue;
	JoinThreads m_joiner;
	std::vector<std::thread> m_threads;
	void InitDB();
	void InitThreads();
private:
	mco_device_t       dev[N_DEVICES]; 
	mco_db_params_t    db_params;
private:
	Callback m_respCallback;
	//线程工作函数
	void worker();
	
	McoDBWrapper(const McoDBWrapper&) = delete;
	McoDBWrapper& operator=(const McoDBWrapper&) = delete;
};

#endif