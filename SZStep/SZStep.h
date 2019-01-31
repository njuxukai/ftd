#ifndef SZ_STEP_H
#define SZ_STEP_H

#include <string>
#include <quickfix/Message.h>
#include <ftd/DerivativeType.h>
#include <ftd/FTD30/Packages.h>

namespace FIX
{
	namespace FIELD
	{
		const int DesignationInstruction = 8502 ;
		const int DesignationTransType = 8527;
		const int VotingProposal = 8529;
		const int VotingSubProposal = 8530;
		const int VotingPreference = 8531;
		const int ValidationCode = 8539;
		const int NoSecurity = 8902;
		const int DeliverQty = 8903;
		const int SubstCash = 8904;
		const int ExpirationType = 8906;
		const int ExpirationExecInst = 8907;
		const int ShareProperty = 8908;
		const int BidPositionEffect = 8909;
		const int OfferPositionEffect = 8910;
		const int ExpirationDays = 8911;
		const int TrdAckStatus = 8912;
		const int ReportIndex = 10179;
		const int PlatformID = 10180;
		const int PlatformStatus = 10181;
		const int OrigTradeReportID = 10182;
		const int PledgeeType = 10183;
		const int Contactor = 10184;
		const int ContactInfo = 10185;
		const int NoMarginItems = 10186;
		const int MarginItem = 10187;
		const int MarginAmount = 10188;
		const int IMCRejectText = 10189;
		const int InvestmentType = 10190;
		const int AlertRatio = 10191;
		const int SettlementRatio = 10192;
		const int NoBaskets = 10193;
		const int BasketID = 10194;
		const int DeliverySide = 10195;
		const int NoPartitions = 10196;
		const int PartitionNo = 10197;
	}

	USER_DEFINE_CHAR(DesignationInstruction, 8502);
	USER_DEFINE_INT(DesignationTransType, 8527);
	USER_DEFINE_INT(VotingProposal, 8529);
	USER_DEFINE_INT(VotingSubProposal, 8530);
	USER_DEFINE_CHAR(VotingPreference, 8531);
	USER_DEFINE_INT(ValidationCode, 8539);
	USER_DEFINE_NUMINGROUP(NoSecurity, 8902);
	USER_DEFINE_QTY(DeliverQty, 8903);
	USER_DEFINE_AMT(SubstCash, 8904);
	USER_DEFINE_INT(ExpirationType, 8906);
	USER_DEFINE_INT(ExpirationExecInst, 8907);
	USER_DEFINE_STRING(ShareProperty, 8908);
	USER_DEFINE_CHAR(BidPositionEffect, 8909);
	USER_DEFINE_CHAR(OfferPositionEffect, 8910);
	USER_DEFINE_INT(ExpirationDays, 8911);
	USER_DEFINE_CHAR(TrdAckStatus, 8912);

	USER_DEFINE_SEQNUM(ReportIndex, 10179);
	USER_DEFINE_INT(PlatformID, 10180);
	USER_DEFINE_INT(PlatformStatus, 10181);
	USER_DEFINE_STRING(OrigTradeReportID, 10182);
	USER_DEFINE_INT(PledgeeType, 10183);
	USER_DEFINE_STRING(Contactor, 10184);
	USER_DEFINE_STRING(ContactInfo, 10185);
	USER_DEFINE_NUMINGROUP(NoMarginItems, 10186);
	USER_DEFINE_INT(MarginItem, 10187);
	USER_DEFINE_AMT(MarginAmount, 10188);
	USER_DEFINE_STRING(IMCRejectText, 10189);
	USER_DEFINE_STRING(InvestmentType, 10190);
	USER_DEFINE_PERCENTAGE(AlertRatio, 10191);
	USER_DEFINE_PERCENTAGE(SettlementRatio, 10192);
	USER_DEFINE_NUMINGROUP(NoBaskets, 10193);
	USER_DEFINE_INT(BasketID, 10194);
	USER_DEFINE_CHAR(DeliverySide, 10195);
	USER_DEFINE_NUMINGROUP(NoPartitions, 10196);
	USER_DEFINE_INT(PartitionNo, 10197);
}

namespace SZStep
{
	const char MSG_TYPE_REPORT_SYNCHRONIZATION[] = "U101";
	const char MSG_TYPE_PLATFORM_STATE_INFO[] = "U102";
	const char MSG_TYPE_REPORT_FINISHED[] = "U103";
	const char MSG_TYPE_REPORT_PLATFORM_INFO[] = "U104";

	const int STATUS_PRE_OPEN = 0;
	const int STATUS_OPEN_UPCOMING = 1;
	const int STATUS_OPEN = 2;
	const int STATUS_HALT = 3;
	const int STATUS_CLOSE = 4;

	struct ReportSynchronization
	{
		std::map<int, int> partitionNoReportIndexMap;
	};

	struct PlatformStateInfo
	{
		int platformID;
		int platformStatus;
	};

	struct ReportFinished
	{
		int partitonNo;
		int reportIndex;
	};

	struct PlatformInfo
	{
		int platformID;
		std::vector<int> partitionIDs;
	};

	namespace ApplID
	{
		static const std::string XH = "010";
	};

	namespace ToFix
	{
		void formatReportSynchronization(const ReportSynchronization& reportSync, FIX::Message& msg);
		void formatReportFinished(const ReportFinished& reportFinished, FIX::Message& msg);
		void formatPlatformStateInfo(const PlatformStateInfo& stateInfo, FIX::Message& msg);
		void formatPlatformInfo(const PlatformInfo& info, FIX::Message& msg);

		void formatInputOrderField(const FTD::CFtdcInputOrderField& inputOrder, FIX::Message& msg);
		void formatInnerExecutionReport(const FTD::CFtdcInnerExecutionReportField& report, FIX::Message& msg);
	};

	namespace FromFix
	{
		bool convertReportSynchronization(const FIX::Message& msg, ReportSynchronization& reportSync);
		bool convertReportFinished(const FIX::Message& msg, ReportFinished& reportFinished);
		bool convertPlatformStateInfo(const FIX::Message& msg, PlatformStateInfo& stateInfo);
		bool convertPlatformInfo(const FIX::Message& msg, PlatformInfo& info);

		bool convertInputOrderField(const FIX::Message& msg, FTD::CFtdcInputOrderField& req);
	};
	
};

#endif

