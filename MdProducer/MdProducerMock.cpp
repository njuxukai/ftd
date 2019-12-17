#include "MdProducerMock.h"
#include <chrono>
#include <fstream>
#include <ftd/Settings.h>


MdProducerMock::MdProducerMock(const char* fname)
{
	m_pSpi = NULL;
	m_parseResult = ParseCfgFile(fname);
}


MdProducerMock::~MdProducerMock()
{
	Release();
}

void MdProducerMock::RegisterSpi(MdProducerSpi* pSpi)
{
	m_pSpi = pSpi;
}

void MdProducerMock::Init()
{
	if (m_parseResult)
	{
		m_is_working = true;
		m_worker_thread =
			std::thread(std::bind(&MdProducerMock::ProductWork, this));
	}

}


void MdProducerMock::Release()
{
	std::unique_lock<std::mutex> lock(m_mtx_working);
	m_is_working = false;
	m_cv_working.notify_all();

	if (m_worker_thread.joinable())
		m_worker_thread.join();
	m_pSpi = NULL;
}


void MdProducerMock::ProductWork()
{
	while (true)
	{
		//Producd md and call spi
		if (m_pSpi)
		{
			std::cout << "定时生成行情" << std::endl;
			for (int i = 0; i < m_candidates.size(); i++)
			{
				FTD::IncMarketData pack;
				strcpy(pack.marketDataField.InstrumentCode, m_candidates[i].InstrumentCode);
				pack.marketDataField.ExchangeType = m_candidates[i].ExchangeType;
				pack.marketDataField.PriceLast = 10;
				m_pSpi->OnNewMd(&pack);
			}
		}
		std::unique_lock<std::mutex> lock(m_mtx_working);
		m_cv_working.wait_for(lock, std::chrono::milliseconds(m_ms_interval));
		if (!m_is_working)
		{
			std::cout << "退出生成行情" << std::endl;
			break;
		}
	}
}

bool MdProducerMock::ParseCfgFile(const char* fname)
{
	using namespace FTD;
	bool parseResult = true;
	try
	{
		std::ifstream stream(fname);
		FTD::Settings settings(false);
		stream >> settings;
		Settings::Sections section;

		section = settings.get("SPEC");
		if (section.size() == 0)
			return false;
		Dictionary specDict = section[0];
		m_ms_interval = specDict.getInt("MsInterval");

		section = settings.get("CANDIDATE");
		for (int i = 0; i < section.size(); i++)
		{
			Dictionary candidateDict = section[i];
			CFtdcInstrumentField instrument = { 0 };
			strcpy(instrument.InstrumentCode, candidateDict.getString("InstrumentCode").data());
			instrument.ExchangeType = candidateDict.getString("ExchangeType")[0];
			m_candidates.push_back(instrument);
		}
	}

	catch (std::exception& e)
	{
		parseResult = false;
	}
	return parseResult;
}