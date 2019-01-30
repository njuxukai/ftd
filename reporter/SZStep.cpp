#include "SzStep.h"
#include <boost/lexical_cast.hpp>

namespace SZStep
{
namespace ToFix 
{
	void formatReqRptOrderInsert(const FTD::CFtdcInputOrderField& inputOrder, FIX::Message& msg)
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

};	
};
