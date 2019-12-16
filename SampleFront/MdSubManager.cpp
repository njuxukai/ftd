#include "stdafx.h"
#include "MdSubManager.h"

MdSubManager::MdSubManager()
{}

MdSubManager::~MdSubManager()
{}

void MdSubManager::sessionSubInstrument(const FTD::SessionID& id, 
	const FTD::CFtdcInstrumentField& instrument,
	int& errCode, char* errInfo)
{
	errCode = 0;
	strcpy(errInfo, "");
	if (m_instrument_sub_records_dict.find(instrument) != m_instrument_sub_records_dict.end()
		&& m_instrument_sub_records_dict[instrument].find(id) != m_instrument_sub_records_dict[instrument].end())
	{
		return;
	}
	if (sessionQuotaAllUsed(id))
	{
		errCode = QUOTA_ALL_USED;
		strcpy(errInfo , "已达订阅上限");
		return;
	}
	if (m_instrument_sub_records_dict.find(instrument) == m_instrument_sub_records_dict.end())
	{
		m_instrument_sub_records_dict[instrument] = std::set<FTD::SessionID>();
	}
	m_instrument_sub_records_dict[instrument].insert(id);
	m_sub_quota_dict[id].used += 1;
}

void MdSubManager::sessionUnsubInstrument(const FTD::SessionID& id, 
	const FTD::CFtdcInstrumentField& instrument,
	int& errCode,
	char* errInfo)
{
	errCode = 0;
	strcpy(errInfo, "");
	if (m_instrument_sub_records_dict.find(instrument) == m_instrument_sub_records_dict.end()
		|| m_instrument_sub_records_dict[instrument].find(id) == m_instrument_sub_records_dict[instrument].end())
		return;
	else
	{
		m_instrument_sub_records_dict[instrument].erase(id);
		if (m_sub_quota_dict.find(id) != m_sub_quota_dict.end())
			m_sub_quota_dict[id].used -= 1;
	}	
}

void MdSubManager::sessionOffline(const FTD::SessionID& id)
{
	m_sub_quota_dict.erase(id);
	for (auto it = m_instrument_sub_records_dict.begin();
		it != m_instrument_sub_records_dict.end();
		it++)
	{
		it->second.erase(id);
	}
}

void MdSubManager::setSessionQuota(const FTD::SessionID& id, int quota)
{
	QuotaRecord record = { 0 };
	record.quota = quota;
	record.used = 0;
	m_sub_quota_dict[id] = record;
}

bool MdSubManager::sessionQuotaAllUsed(const FTD::SessionID& id)
{
	if (m_sub_quota_dict.find(id) == m_sub_quota_dict.end())
		return true;
	if (m_sub_quota_dict[id].quota == UNLIMIT_QUOTA)
		return false;
	return m_sub_quota_dict[id].used >= m_sub_quota_dict[id].quota;
}

std::set<FTD::SessionID> MdSubManager::getSubSessionSet(const FTD::CFtdcInstrumentField& instrument)
{
	if (m_instrument_sub_records_dict.find(instrument) == m_instrument_sub_records_dict.end())
		return std::set<FTD::SessionID>();
	else
		return m_instrument_sub_records_dict[instrument];
}
