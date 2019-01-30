#ifndef SZ_STEP_H
#define SZ_STEP_H

#include <string>
#include <quickfix/Message.h>
#include <ftd/DerivativeType.h>
#include <ftd/FTD30/Packages.h>

namespace SZStep
{

	namespace ApplID
	{
		static const std::string XH = "010";
	};

	namespace ToFix
	{
		void formatReqRptOrderInsert(const FTD::CFtdcInputOrderField& inputOrder, FIX::Message& msg);
		void formatInnerExecutionReport(const FTD::CFtdcInnerExecutionReportField& report, FIX::Message& msg);
	};

	namespace FromFix
	{
		bool convertReqRptOrderInsert(const FIX::Message& msg, FTD::CFtdcInputOrderField& req);
	};
	
};

#endif

