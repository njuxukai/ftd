#pragma once
#include "MdProducer.h"
#include <thread>
#include <atomic>
#include <condition_variable>
#include <vector>

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
	bool m_parseResult;
	bool ParseCfgFile(const char* fname);
	std::thread m_worker_thread;
	std::mutex m_mtx_working;
	std::condition_variable m_cv_working;
	bool m_is_working;
	void ProductWork();
	long long m_ms_interval;
	
	std::vector<FTD::CFtdcInstrumentField> m_candidates;
};