#include "SzStep.h"
#include <boost/lexical_cast.hpp>

namespace SZStep
{
namespace ToFix 
{
	void formatReportSynchronization(const ReportSynchronization& reportSync, FIX::Message& msg)
	{
		msg.getHeader().setField(FIX::MsgType(MSG_TYPE_REPORT_SYNCHRONIZATION));
		FIX::Group partition(FIX::FIELD::NoPartitions, FIX::FIELD::PartitionNo);
		for (auto it = reportSync.partitionNoReportIndexMap.begin(); 
		it != reportSync.partitionNoReportIndexMap.end(); it++)
		{
			partition.setField(FIX::PartitionNo(it->first));
			partition.setField(FIX::ReportIndex(it->second));
			msg.addGroup(partition);
		}
	}

	void formatReportFinished(const ReportFinished& reportFinished, FIX::Message& msg)
	{
		msg.getHeader().setField(FIX::MsgType(MSG_TYPE_REPORT_FINISHED));
		msg.setField(FIX::PlatformID(reportFinished.partitonNo));
		msg.setField(FIX::ReportIndex(reportFinished.reportIndex));
	}

	void formatPlatformStateInfo(const PlatformStateInfo& stateInfo, FIX::Message& msg)
	{
		msg.getHeader().setField(FIX::MsgType(MSG_TYPE_PLATFORM_STATE_INFO));
		FIX::PlatformID platformIDField(stateInfo.platformID);
		FIX::PlatformStatus platformStatusField(stateInfo.platformStatus);
		msg.setField(platformIDField);
		msg.setField(platformStatusField);
	}

	void formatPlatformInfo(const PlatformInfo& info, FIX::Message& msg)
	{
		msg.getHeader().setField(FIX::MsgType(MSG_TYPE_REPORT_PLATFORM_INFO));
		FIX::PlatformID platformIDField(info.platformID);
		msg.setField(platformIDField);
		FIX::Group partition(FIX::FIELD::NoPartitions, FIX::FIELD::PartitionNo);
		for (int i = 0; i < info.partitionIDs.size(); i++)
		{
			FIX::PartitionNo partitionNoField(info.partitionIDs[i]);
			partition.setField(partitionNoField);
			msg.addGroup(partition);
		}
	}

	void formatInputOrderField(const FTD::CFtdcInputOrderField& inputOrder, FIX::Message& msg)
	{
		//ApplID
		msg.setField(FIX::ApplID(SZStep::ApplID::XH));
		//ClOrdID
		msg.setField(FIX::ClOrdID(inputOrder.ClOrdID));
		//OrdType
		switch (inputOrder.PriceType)
		{
		case FTDC_OPT_HS_Limit:
			msg.setField(FIX::OrdType('2'));
			break;
		case FTDC_OPT_S_SL1:
			msg.setField(FIX::OrdType('U'));
			break;
		case FTDC_OPT_S_L1_THEN_LIMIT:
		case FTDC_OPT_S_FAK:
		case FTDC_OPT_S_FOK:
			msg.setField(FIX::OrdType('1'));
			break;
		default:
			break;
		}

		//Side
		if (inputOrder.Direction == FTDC_D_BUY)
			msg.setField(FIX::Side(FIX::Side_BUY));
		else
			msg.setField(FIX::Side(FIX::Side_SELL));

		//TODO OwnerType
		if (inputOrder.UserID == inputOrder.InvestorID)
			msg.setField(FIX::OwnerType(1));
		else
			msg.setField(FIX::OwnerType(102));
		//Instrument
		msg.setField(FIX::SecurityID(inputOrder.InstrumentCode));
		msg.setField(FIX::SecurityIDSource("102"));

		//Groups Parties
		FIX::Group party(FIX::FIELD::NoPartyIDs, FIX::FIELD::PartyID);
		party.setField(FIX::PartyID(inputOrder.SecurityAccount));
		party.setField(FIX::PartyIDSource('5'));
		party.setField(FIX::PartyRole(5));
		msg.addGroup(party);

		std::string strPbuID = boost::lexical_cast<std::string>(inputOrder.PbuID);
		party.setField(FIX::PartyID(strPbuID));
		party.setField(FIX::PartyIDSource('C'));
		party.setField(FIX::PartyRole(1));
		msg.addGroup(party);

		std::string strExchangeBranchID = boost::lexical_cast<std::string>(inputOrder.ExchangeBranchID);
		party.setField(FIX::PartyID(strExchangeBranchID));
		party.setField(FIX::PartyIDSource('D'));
		party.setField(FIX::PartyRole(4001));
		msg.addGroup(party);
		//OrderQty
		msg.setField(FIX::OrderQty(inputOrder.VolumeTotalOrginal));

		//Price ?
		if (inputOrder.PriceType == FTDC_OPT_HS_Limit)
			msg.setField(FIX::Price(inputOrder.LimitPrice));

		//TimeInForce
		//MaxPriceLevels
		//MinQty
		switch (inputOrder.PriceType)
		{
		case FTDC_OPT_HS_Limit:
		case FTDC_OPT_S_SL1:
			msg.setField(FIX::TimeInForce('0'));
			msg.setField(FIX::MaxPriceLevels(0));
			msg.setField(FIX::MinQty(0));
			break;

		case FTDC_OPT_S_L1_THEN_LIMIT:
			msg.setField(FIX::TimeInForce('0'));
			msg.setField(FIX::MaxPriceLevels(1));
			msg.setField(FIX::MinQty(0));
			break;
		case FTDC_OPT_S_FAK:
			msg.setField(FIX::TimeInForce('3'));
			msg.setField(FIX::MaxPriceLevels(0));
			msg.setField(FIX::MinQty(0));
			break;
		case FTDC_OPT_S_FOK:
			msg.setField(FIX::TimeInForce('3'));
			msg.setField(FIX::MaxPriceLevels(0));
			msg.setField(FIX::MinQty(inputOrder.VolumeTotalOrginal));
			break;
		default:
			break;
		}
		//CashMargin
	}

}

namespace FromFix
{
	bool convertReportSynchronization(const FIX::Message& msg, ReportSynchronization& reportSync)
	{
		bool convertResult = true;
		reportSync.partitionNoReportIndexMap.clear();
		try
		{
			int partitionCount = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::NoPartitions));
			FIX::Group partition(FIX::FIELD::NoPartitions, FIX::FIELD::PartitionNo);
			for (int i = 1; i <= partitionCount; i++)
			{
				msg.getGroup(i, partition);
				int partitionNo = FIX::IntConvertor::convert(partition.getField(FIX::FIELD::PartitionNo));
				int reportIndex = FIX::IntConvertor::convert(partition.getField(FIX::FIELD::ReportIndex));
				reportSync.partitionNoReportIndexMap[partitionNo] = reportIndex;
			}
		}
		catch (...)
		{
			convertResult = false;
		}
		return convertResult;
	}

	bool convertReportFinished(const FIX::Message& msg, ReportFinished& reportFinished)
	{
		bool convertResult = true;
		try
		{
			reportFinished.partitonNo = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::PartitionNo));
			reportFinished.reportIndex = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::ReportIndex));
		}
		catch (...)
		{
			convertResult = false;
		}
		return convertResult;
	}

	bool convertPlatformStateInfo(const FIX::Message& msg, PlatformStateInfo& stateInfo)
	{
		bool convertResult = true;

		try
		{
			stateInfo.platformID = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::PlatformID));
			stateInfo.platformStatus = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::PlatformStatus));
		}
		catch (...)
		{
			convertResult = false;
		}
		return convertResult;
	}

	bool convertPlatformInfo(const FIX::Message& msg, PlatformInfo& info)
	{
		bool convertResult = true;
		info.partitionIDs.clear();
		try
		{
			info.platformID = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::PlatformID));

			int partitionCount = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::NoPartitions));
			FIX::Group partition(FIX::FIELD::NoPartitions, FIX::FIELD::PartitionNo);
			for (int i = 1; i <= partitionCount; i++)
			{
				msg.getGroup(i, partition);
				info.partitionIDs.push_back(FIX::IntConvertor::convert(partition.getField(FIX::FIELD::PartitionNo)));
			}
		}
		catch (...)
		{
			convertResult = false;
		}
		return convertResult;
	}

	bool convertInputOrderField(const FIX::Message& msg, FTD::CFtdcInputOrderField& order)
	{
		bool convertResult = true;
		try 
		{
			//applid

			//ClOrdID
			strcpy(order.ClOrdID, msg.getField(FIX::FIELD::ClOrdID).data());

			//volume
			order.VolumeTotalOrginal = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::OrderQty));
			//OrdType, TimeInForce MaxPriceLevels MinQty 
			char ordTypeData = FIX::CharConvertor::convert(msg.getField(FIX::FIELD::OrdType));
			char timeInForceData = '0';
			int maxPriceLevelsData = 0;
			int minQtyData = 0;
			if (msg.isSetField(FIX::FIELD::TimeInForce))
				timeInForceData = FIX::CharConvertor::convert(msg.getField(FIX::FIELD::TimeInForce));
			if (msg.isSetField(FIX::FIELD::MaxPriceLevels))
				maxPriceLevelsData = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::MaxPriceLevels));
			if (msg.isSetField(FIX::FIELD::MinQty))
				minQtyData = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::MinQty));
			//Instrument
			strcpy(order.InstrumentCode, msg.getField(FIX::FIELD::SecurityID).data());
			order.ExchangeType = FTDC_ET_SZ;
			switch (ordTypeData)
			{
			case '2':
				order.PriceType = FTDC_OPT_HS_Limit;
				break;
			case 'U':
				order.PriceType = FTDC_OPT_S_SL1;
				break;
			case '1':
				if (timeInForceData == '0' && maxPriceLevelsData == 1 && minQtyData == 0)
					order.PriceType = FTDC_OPT_S_L1_THEN_LIMIT;
				else if (timeInForceData == '3' && maxPriceLevelsData == 0 && minQtyData == 0)
					order.PriceType = FTDC_OPT_S_FAK;
				else if (timeInForceData == '3' && maxPriceLevelsData == 0 && minQtyData == order.VolumeTotalOrginal)
					order.PriceType = FTDC_OPT_S_FOK;
				break;
			default:
				break;
			}
			//side
			char sideData = FIX::CharConvertor::convert(msg.getField(FIX::FIELD::Side));
			if (sideData == FIX::Side_SELL)
				order.Direction = FTDC_D_SELL;
			else
				order.Direction = FTDC_D_BUY;
			//Groups Parties
			int partyCount = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::NoPartyIDs));
			FIX::Group party(FIX::FIELD::NoPartyIDs, FIX::FIELD::PartyID);
			for (int i = 1; i <= partyCount; i++)
			{
				msg.getGroup(i, party);
				char sourceData = FIX::CharConvertor::convert(party.getField(FIX::FIELD::PartyIDSource));
				std::string source = party.getField(FIX::FIELD::PartyID);
				switch (sourceData)
				{
				case '5':
					strcpy(order.SecurityAccount, source.data());
					break;
				case 'C':
					order.PbuID = atoi(source.data());
					break;
				case 'D':
					order.ExchangeBranchID = atoi(source.data());
				}
			}
			//
			if (msg.isSetField(FIX::FIELD::Price))
				order.LimitPrice = FIX::DoubleConvertor::convert(msg.getField(FIX::FIELD::Price));
			else
				order.LimitPrice = 0;


		}
		catch (...)
		{
			convertResult = false;
		}
		return convertResult;
	}
}
}
