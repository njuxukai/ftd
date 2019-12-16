#include "MdProducerMock.h"

MdProducerMock::MdProducerMock(const char* fname)
{}


MdProducerMock::~MdProducerMock()
{
	Release();
}

void RegisterSpi(MdProducerSpi* pSpi)
{
	m_pSpi = pSpi;
}

void MdProducerMock::Init()
{
	m_is_working = true;
	m_worker_thread = 
		std::thread(std::bind(&MdProducerMock::ProductWork, this));

}


void MdProducerMock::Release()
{
	m_is_working = false;
	if (m_worker_thread.joinable())
		m_worker_thread.join();
	m_pSpi = NULL;
}


void MdProducerMock::ProductWork()
{
	while ()
	{
	}
}