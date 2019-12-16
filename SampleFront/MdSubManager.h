#pragma once

#include <ftd/FTD30/Fields.h>
#include <ftd/SessionID.h>

#include <map>
#include <set>
#include <tuple>

#define UNLIMIT_QUOTA -1

#define QUOTA_ALL_USED -9001

struct InstrumentLess {
	bool operator() (const FTD::CFtdcInstrumentField& lhs, const FTD::CFtdcInstrumentField& rhs) const
	{
		if (lhs.ExchangeType < rhs.ExchangeType)
			return true;
		if (lhs.ExchangeType == rhs.ExchangeType && strcmp(lhs.InstrumentCode, rhs.InstrumentCode) < 0)
			return true;
		return false;
	}
};

class MdSubManager
{
public:
	MdSubManager();
	~MdSubManager();

	void sessionSubInstrument(const FTD::SessionID& id, 
		const FTD::CFtdcInstrumentField& instrument, 
		int& errCode, char* errorInfo);
	void sessionUnsubInstrument(const FTD::SessionID& id, 
		const FTD::CFtdcInstrumentField& instrument, 
		int& errCode, char* errorInfo);
	void sessionOffline(const FTD::SessionID& id);
	void setSessionQuota(const FTD::SessionID& in, int quota);
	std::set<FTD::SessionID> getSubSessionSet(const FTD::CFtdcInstrumentField& instrument);
private:
	struct QuotaRecord
	{
		int quota;
		int used;
	};
	std::map<FTD::CFtdcInstrumentField, std::set<FTD::SessionID>, InstrumentLess> m_instrument_sub_records_dict;
	std::map<FTD::SessionID, QuotaRecord> m_sub_quota_dict;
	bool sessionQuotaAllUsed(const FTD::SessionID& id);
};