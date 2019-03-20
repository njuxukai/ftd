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
		msg.setField(FIX::PlatformID
			(reportFinished.platformID));
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
		case FTDC_OPT_S_LIMIT_FOK:
			msg.setField(FIX::OrdType('2'));
			break;
		case FTDC_OPT_S_SL1:
			msg.setField(FIX::OrdType('U'));
			break;
		case FTDC_OPT_S_L1_THEN_LIMIT:
		case FTDC_OPT_S_FAK:
		case FTDC_OPT_S_FOK:
		case FTDC_OPT_HS_L5_THEN_LIMIT:
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
		case FTDC_OPT_S_LIMIT_FOK:
			msg.setField(FIX::TimeInForce('3'));
			msg.setField(FIX::MaxPriceLevels(0));
			msg.setField(FIX::MinQty(inputOrder.VolumeTotalOrginal));
			break;
		case FTDC_OPT_HS_L5_THEN_LIMIT:
			msg.setField(FIX::TimeInForce('3'));
			msg.setField(FIX::MaxPriceLevels(5));
			msg.setField(FIX::MinQty(0));
			break;
		default:
			break;
		}
		//CashMargin
	}

	void formatInputOrderActionField(const FTD::CFtdcInputOrderActionField& inputOrderAction, FIX::Message& msg)
	{}

	//only used by mock exchange(SZ)
	void formatInnerExecutionReport(const FTD::CFtdcInnerExecutionReportField& report, FIX::Message& msg)
	{
		msg.getHeader().setField(FIX::MsgType("8"));
		msg.setField(FIX::PartitionNo(report.PartitionNo));
		msg.setField(FIX::ReportIndex(report.ReportIndex));
		msg.setField(FIX::ApplID(SZStep::ApplID::XH));
		msg.setField(FIX::OwnerType(report.OwnerType));
		msg.setField(FIX::OrderRestrictions(report.OrderRestrictions));
		msg.setField(FIX::ExecID(report.ReportExchangeID));
		msg.setField(FIX::OrderID(report.OrderExchangeID));
		switch (report.ExecType)
		{
		case FTDC_ET_Trade:
			msg.setField(FIX::ExecType('F'));
			break;
		case FTDC_ET_Create:
		case FTDC_ET_New:
			msg.setField(FIX::ExecType('0'));
			break;
		case FTDC_ET_Cancelled:
			msg.setField(FIX::ExecType('8'));
			break;
		case FTDC_ET_Rejected:
			msg.setField(FIX::ExecType('4'));
			break;
		default:
			break;
		}
		msg.setField(FIX::ExecType(report.ExecType));
		switch (report.OrderStatus)
		{
		case FTDC_OS_NEW:
		case FTDC_OS_CREATED:
			msg.setField(FIX::OrdStatus('0'));
			break;
		case FTDC_OS_ALL_TRADED:
			msg.setField(FIX::OrdStatus('2'));
			break;
		case FTDC_OS_PART_TRADED:
			msg.setField(FIX::OrdStatus('1'));
			break;
		case FTDC_OS_CANCELLED:
			msg.setField(FIX::OrdStatus('4'));
			break;
		case FTDC_OS_REJECTED:
			msg.setField(FIX::OrdStatus('8'));
			break;
		default:
			break;
		}

		if (report.ExecType == FTDC_ET_Trade)
		{
			msg.setField(FIX::LastPx(report.PriceLast));
			msg.setField(FIX::LastQty(report.VolumeLast));
		}
		msg.setField(FIX::CumQty(report.VolumeCum));
		msg.setField(FIX::LeavesQty(report.VolumeLeaves));
		if (report.ExecType == FTDC_ET_Cancelled || report.ExecType == FTDC_ET_Rejected)
		{
			msg.setField(FIX::OrdRejReason(report.OrderRejReason));
			msg.setField(FIX::RejectText(report.OrderRejText));
		}
		if (report.Direction == FTDC_D_BUY)
			msg.setField(FIX::Side('1'));
		else
			msg.setField(FIX::Side('2'));

		auto curTimeStamp = FIX::UtcTimeStamp();
		curTimeStamp.set(FIX::DateTime::nowLocal());
		msg.setField(FIX::TransactTime(curTimeStamp));
		
		//ClOrdID
		//OrigClOrdID
		if (strlen(report.ActionClOrdID) > 0) 
		{
			msg.setField(FIX::ClOrdID(report.ActionClOrdID));
			msg.setField(FIX::OrigClOrdID(report.ClOrdID));
		}
		else
		{
			msg.setField(FIX::ClOrdID(report.ClOrdID));
		}
		msg.setField(FIX::SecurityID(report.InstrumentCode));
		msg.setField(FIX::SecurityIDSource("102"));

		//Groups Parties
		FIX::Group party(FIX::FIELD::NoPartyIDs, FIX::FIELD::PartyID);
		party.setField(FIX::PartyID(report.SecurityAccount));
		party.setField(FIX::PartyIDSource('5'));
		party.setField(FIX::PartyRole(5));
		msg.addGroup(party);

		std::string strPbuID = boost::lexical_cast<std::string>(report.PbuID);
		party.setField(FIX::PartyID(strPbuID));
		party.setField(FIX::PartyIDSource('C'));
		party.setField(FIX::PartyRole(1));
		msg.addGroup(party);

		std::string strExchangeBranchID = boost::lexical_cast<std::string>(report.ExchangeBranchID);
		party.setField(FIX::PartyID(strExchangeBranchID));
		party.setField(FIX::PartyIDSource('D'));
		party.setField(FIX::PartyRole(4001));
		msg.addGroup(party);

		//
		msg.setField(FIX::OrderQty(report.VolumeTotalOrginal));
		switch (report.PriceType)
		{
		case FTDC_OPT_HS_Limit:
		case FTDC_OPT_S_LIMIT_FOK:
			msg.setField(FIX::OrdType('2'));
			break;
		case FTDC_OPT_S_SL1:
			msg.setField(FIX::OrdType('U'));
			break;
		case FTDC_OPT_S_L1_THEN_LIMIT:
		case FTDC_OPT_S_FAK:
		case FTDC_OPT_S_FOK:
		case FTDC_OPT_HS_L5_THEN_LIMIT:
			msg.setField(FIX::OrdType('1'));
			break;
		default:
			break;
		}
		//report.PriceType
	}

	void formatInnerOrderCancelReject(const FTD::CFtdcInnerOrderCancelRejectField& report, FIX::Message& msg)
	{}

	void formatInnerBusinessReject(const FTD::CFtdcInnerBusinessRejectField& report, FIX::Message& msg)
	{}


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
			reportFinished.platformID =
				FIX::IntConvertor::convert(msg.getField(FIX::FIELD::PlatformID));
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
			if (timeInForceData == '0' && ordTypeData == '2' 
				&& maxPriceLevelsData == 0 && minQtyData == 0)
			{
				order.PriceType = FTDC_OPT_HS_Limit;
			}
			if (timeInForceData == '0' && ordTypeData == 'U'
				&& maxPriceLevelsData == 0 && minQtyData == 0)
			{
				order.PriceType = FTDC_OPT_S_SL1;
			}
			if (timeInForceData == '0' && ordTypeData == '1'
				&& maxPriceLevelsData == 1 && minQtyData == 0)
			{
				order.PriceType = FTDC_OPT_S_L1_THEN_LIMIT;
			}
			if (timeInForceData == '3' && ordTypeData == '1'
				&& maxPriceLevelsData == 0 && minQtyData == 0)
			{
				order.PriceType = FTDC_OPT_S_FAK;
			}
			if (timeInForceData == '3' && ordTypeData == '2'
				&& maxPriceLevelsData == 0 && minQtyData == order.VolumeTotalOrginal)
			{
				order.PriceType = FTDC_OPT_S_LIMIT_FOK;
			}
			if (timeInForceData == '3' && ordTypeData == '1'
				&& maxPriceLevelsData == 0 && minQtyData == order.VolumeTotalOrginal)
			{
				order.PriceType = FTDC_OPT_S_FOK;
			}
			if (timeInForceData == '3' && ordTypeData == '1'
				&& maxPriceLevelsData == 5 && minQtyData == 0)
			{
				order.PriceType = FTDC_OPT_H_L5_FAK;
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


	bool convertInputOrderActionField(const FIX::Message& msg, FTD::CFtdcInputOrderActionField& req)
	{
		bool convertResult = true;
		try
		{
		}
		catch (...)
		{
			convertResult = false;
		}
		return convertResult;
	}

	bool convertInnerExecutionReport(const FIX::Message& msg, FTD::CFtdcInnerExecutionReportField& report)
	{
		bool convertResult = true;
		try
		{
			//PartitionNo
			report.PartitionNo = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::PartitionNo));
			//ReportIndex
			report.ReportIndex = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::ReportIndex));
			//ApplID
			strcpy(report.ApplID, msg.getField(FIX::FIELD::ApplID).data());
			//OwnerType
			report.OwnerType = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::OwnerType));
			//OrderRestrictions
			if (msg.isSetField(FIX::FIELD::OrderRestrictions))
			{
				strcpy(report.OrderRestrictions, msg.getField(FIX::FIELD::OrderRestrictions).data());
			}			
			//ExecID
			strcpy(report.ReportExchangeID, msg.getField(FIX::FIELD::ExecID).data());
			report.ReportExchangeSubID = 0;
			//OrderID
			strcpy(report.OrderExchangeID, msg.getField(FIX::FIELD::OrderID).data());
			//ExecType
			char fixExecType = FIX::CharConvertor::convert(msg.getField(FIX::FIELD::ExecID));
			switch (fixExecType)
			{
			case('0') :
				report.ExecType = FTDC_ET_New;
				break;
			case('4') :
				report.ExecType = FTDC_ET_Cancelled;
				break;
			case('8') :
				report.ExecType = FTDC_ET_Rejected;
				break;
			case('F') :
				report.ExecType = FTDC_ET_Trade;
				break;
			default:
				break;
			}
			//OrdStatus
			char fixOrdStatus = FIX::CharConvertor::convert(msg.getField(FIX::FIELD::OrdStatus));
			switch (fixOrdStatus)
			{
			case '0':
				report.OrderStatus = FTDC_OS_NEW;
				break;
			case '1':
				report.OrderStatus = FTDC_OS_PART_TRADED;
				break;
			case '2':
				report.OrderStatus = FTDC_OS_ALL_TRADED;
				break;
			case '4':
				report.OrderStatus = FTDC_OS_CANCELLED;
				break;
			case '8':
				report.OrderStatus = FTDC_OS_REJECTED;
				break;
			default:
				break;
			}
			//LastPx
			if (msg.isSetField(FIX::FIELD::LastPx))
			{
				report.PriceLast = FIX::DoubleConvertor::convert(msg.getField(FIX::FIELD::LastPx));
			}			
			//LastQty
			if (msg.isSetField(FIX::FIELD::LastQty))
			{
				report.VolumeLast = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::LastQty));
			}
			
			//LeavesQty
			report.VolumeLeaves = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::LeavesQty));
			//CumQty
			report.VolumeCum = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::CumQty));
			//OrdRejReasion
			if (msg.isSetField(FIX::FIELD::OrdRejReason))
			{
				report.OrderRejReason = FIX::IntConvertor::convert(msg.getField(FIX::FIELD::OrdRejReason));
			}
			//RejectText
			if (msg.isSetField(FIX::FIELD::RejectText))
			{
				strcpy(report.OrderRejText, msg.getField(FIX::FIELD::RejectText).data());
			}
			//Side
			char fixSide = FIX::CharConvertor::convert(msg.getField(FIX::FIELD::Side));
			if (fixSide == FIX::Side_BUY)
				report.Direction = FTDC_D_BUY;
			else
				report.Direction = FTDC_D_SELL;
			//TransactTime
			strcpy(report.TransactimeString, msg.getField(FIX::FIELD::TransactTime).data());
			
			//OrigClOrdID ClOrdID
			if (msg.isSetField(FIX::FIELD::OrigClOrdID))
			{
				strcpy(report.ClOrdID, msg.getField(FIX::FIELD::OrigClOrdID).data());
				strcpy(report.ActionClOrdID, msg.getField(FIX::FIELD::ClOrdID).data());
			}
			else
			{
				strcpy(report.ClOrdID, msg.getField(FIX::FIELD::ClOrdID).data());
			}
			//Instrument SecurityID SecurityIDSource
			strcpy(report.InstrumentCode, msg.getField(FIX::FIELD::SecurityID).data());
			report.ExchangeType = FTDC_ET_SZ;
			//Parties
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
					strcpy(report.SecurityAccount, source.data());
					break;
				case 'C':
					report.PbuID = atoi(source.data());
					break;
				case 'D':
					report.ExchangeBranchID = atoi(source.data());
				}
			}
		}
		catch (...)
		{
			convertResult = false;
		}
		return convertResult;
	}

	bool convertInnerOrderCancelReject(const FIX::Message& msg, FTD::CFtdcInnerOrderCancelRejectField& report)
	{
		bool convertResult = true;
		try
		{
		}
		catch (...)
		{
			convertResult = false;
		}
		return convertResult;
	}

	bool convertInnerBusinessReject(const FIX::Message& msg, FTD::CFtdcInnerBusinessRejectField& report)
	{
		bool convertResult = true;
		try
		{
		}
		catch (...)
		{
			convertResult = false;
		}
		return convertResult;
	}
}
}
