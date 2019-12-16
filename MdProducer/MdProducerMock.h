#pragma once
#include "MdProducer.h"
#include <thread>
#include <atomic>
#include <condition_variable>

class MdProducerMock : public MdProducerApi
{
public:
	MdProducerMock(const char* fname);
	~MdProducerMock();
	virtual void RegisterSpi(MdProducerSpi* pSpi);
	virtual void Init();
	virtual void Release();
private:
	MdProducerSpi* m_pSpi;
	std::thread m_worker_thread;
	std::condition_variable m_cv_working;
	bool m_is_working;
	void ProductWork();
};