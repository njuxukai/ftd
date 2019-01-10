#ifndef THREADSAFE_QUEUE_H
#define THREADSAFE_QUEUE_H

#include <memory>
#include <mutex>
#include <queue>

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

#endif