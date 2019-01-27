#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#include "stdafx.h"
#else
#include "config.h"
#endif

#include <ftd/BaseType.h>
#include <UnitTest++.h>
#include <ftd/PortID.h>
#include <xcp/XcpFtdcUserApiStruct.h>
#include <ftd/FTD30/Fields.h>
using namespace FTD;

SUITE(AutoGen)
{

	TEST(XcpStructure)
	{
            CHECK(sizeof(CXcpFtdcErrorField) >= 0);
            CHECK(sizeof(CXcpFtdcRequestSourceField) >= 0);
            CHECK(sizeof(CXcpFtdcReqUserLoginField) >= 0);
            CHECK(sizeof(CXcpFtdcDissenminationstartField) >= 0);
            CHECK(sizeof(CXcpFtdcRspUserLoginField) >= 0);
            CHECK(sizeof(CXcpFtdcReqUserLogoutField) >= 0);
            CHECK(sizeof(CXcpFtdcRspUserLogoutField) >= 0);
            CHECK(sizeof(CXcpFtdcForceExitField) >= 0);
            CHECK(sizeof(CXcpFtdcInputOrderField) >= 0);
            CHECK(sizeof(CXcpFtdcInputOrderActionField) >= 0);
            CHECK(sizeof(CXcpFtdcInputFundTransferField) >= 0);
            CHECK(sizeof(CXcpFtdcFundTransferField) >= 0);
            CHECK(sizeof(CXcpFtdcQryFundField) >= 0);
            CHECK(sizeof(CXcpFtdcFundField) >= 0);
            CHECK(sizeof(CXcpFtdcQryPositionField) >= 0);
            CHECK(sizeof(CXcpFtdcPositionField) >= 0);
            CHECK(sizeof(CXcpFtdcQryOrderField) >= 0);
            CHECK(sizeof(CXcpFtdcOrderField) >= 0);
            CHECK(sizeof(CXcpFtdcQryTradeField) >= 0);
            CHECK(sizeof(CXcpFtdcTradeField) >= 0);
            CHECK(sizeof(CXcpFtdcQryFundTransferField) >= 0);
            CHECK(sizeof(CXcpFtdcQryHisOrderField) >= 0);
            CHECK(sizeof(CXcpFtdcQryHisTradeField) >= 0);
            CHECK(sizeof(CXcpFtdcQryHisFundTransferField) >= 0);
            CHECK(sizeof(CXcpFtdcQryInstrumentField) >= 0);
            CHECK(sizeof(CXcpFtdcInstrumentField) >= 0);
            CHECK(sizeof(CXcpFtdcQryStructuredFundField) >= 0);
            CHECK(sizeof(CXcpFtdcStructuredFundField) >= 0);
            CHECK(sizeof(CXcpFtdcQryETFField) >= 0);
            CHECK(sizeof(CXcpFtdcETFField) >= 0);
            CHECK(sizeof(CXcpFtdcQryETFCompositionField) >= 0);
            CHECK(sizeof(CXcpFtdcETFCompositionField) >= 0);
            CHECK(sizeof(CXcpFtdcQryNewIssueSecurityField) >= 0);
            CHECK(sizeof(CXcpFtdcNewIssueSecurityField) >= 0);
            CHECK(sizeof(CXcpFtdcQryPurchaseQuotaField) >= 0);
            CHECK(sizeof(CXcpFtdcPurchaseQuotaField) >= 0);
            CHECK(sizeof(CXcpFtdcInnerExecutionReportField) >= 0);
            CHECK(sizeof(CXcpFtdcExecutionReportField) >= 0);
            CHECK(sizeof(CXcpFtdcQryInitialDataField) >= 0);
            CHECK(sizeof(CXcpFtdcDialogSpecField) >= 0);
            CHECK(sizeof(CXcpFtdcQrySecurityAccountField) >= 0);
            CHECK(sizeof(CXcpFtdcSecurityAccountField) >= 0);

            CHECK(sizeof(CXcpFtdcErrorField) == sizeof(CFtdcErrorField));
            CHECK(sizeof(CXcpFtdcRequestSourceField) == sizeof(CFtdcRequestSourceField));
            CHECK(sizeof(CXcpFtdcReqUserLoginField) == sizeof(CFtdcReqUserLoginField));
            CHECK(sizeof(CXcpFtdcDissenminationstartField) == sizeof(CFtdcDissenminationstartField));
            CHECK(sizeof(CXcpFtdcRspUserLoginField) == sizeof(CFtdcRspUserLoginField));
            CHECK(sizeof(CXcpFtdcReqUserLogoutField) == sizeof(CFtdcReqUserLogoutField));
            CHECK(sizeof(CXcpFtdcRspUserLogoutField) == sizeof(CFtdcRspUserLogoutField));
            CHECK(sizeof(CXcpFtdcForceExitField) == sizeof(CFtdcForceExitField));
            CHECK(sizeof(CXcpFtdcInputOrderField) == sizeof(CFtdcInputOrderField));
            CHECK(sizeof(CXcpFtdcInputOrderActionField) == sizeof(CFtdcInputOrderActionField));
            CHECK(sizeof(CXcpFtdcInputFundTransferField) == sizeof(CFtdcInputFundTransferField));
            CHECK(sizeof(CXcpFtdcFundTransferField) == sizeof(CFtdcFundTransferField));
            CHECK(sizeof(CXcpFtdcQryFundField) == sizeof(CFtdcQryFundField));
            CHECK(sizeof(CXcpFtdcFundField) == sizeof(CFtdcFundField));
            CHECK(sizeof(CXcpFtdcQryPositionField) == sizeof(CFtdcQryPositionField));
            CHECK(sizeof(CXcpFtdcPositionField) == sizeof(CFtdcPositionField));
            CHECK(sizeof(CXcpFtdcQryOrderField) == sizeof(CFtdcQryOrderField));
            CHECK(sizeof(CXcpFtdcOrderField) == sizeof(CFtdcOrderField));
            CHECK(sizeof(CXcpFtdcQryTradeField) == sizeof(CFtdcQryTradeField));
            CHECK(sizeof(CXcpFtdcTradeField) == sizeof(CFtdcTradeField));
            CHECK(sizeof(CXcpFtdcQryFundTransferField) == sizeof(CFtdcQryFundTransferField));
            CHECK(sizeof(CXcpFtdcQryHisOrderField) == sizeof(CFtdcQryHisOrderField));
            CHECK(sizeof(CXcpFtdcQryHisTradeField) == sizeof(CFtdcQryHisTradeField));
            CHECK(sizeof(CXcpFtdcQryHisFundTransferField) == sizeof(CFtdcQryHisFundTransferField));
            CHECK(sizeof(CXcpFtdcQryInstrumentField) == sizeof(CFtdcQryInstrumentField));
            CHECK(sizeof(CXcpFtdcInstrumentField) == sizeof(CFtdcInstrumentField));
            CHECK(sizeof(CXcpFtdcQryStructuredFundField) == sizeof(CFtdcQryStructuredFundField));
            CHECK(sizeof(CXcpFtdcStructuredFundField) == sizeof(CFtdcStructuredFundField));
            CHECK(sizeof(CXcpFtdcQryETFField) == sizeof(CFtdcQryETFField));
            CHECK(sizeof(CXcpFtdcETFField) == sizeof(CFtdcETFField));
            CHECK(sizeof(CXcpFtdcQryETFCompositionField) == sizeof(CFtdcQryETFCompositionField));
            CHECK(sizeof(CXcpFtdcETFCompositionField) == sizeof(CFtdcETFCompositionField));
            CHECK(sizeof(CXcpFtdcQryNewIssueSecurityField) == sizeof(CFtdcQryNewIssueSecurityField));
            CHECK(sizeof(CXcpFtdcNewIssueSecurityField) == sizeof(CFtdcNewIssueSecurityField));
            CHECK(sizeof(CXcpFtdcQryPurchaseQuotaField) == sizeof(CFtdcQryPurchaseQuotaField));
            CHECK(sizeof(CXcpFtdcPurchaseQuotaField) == sizeof(CFtdcPurchaseQuotaField));
            CHECK(sizeof(CXcpFtdcInnerExecutionReportField) == sizeof(CFtdcInnerExecutionReportField));
            CHECK(sizeof(CXcpFtdcExecutionReportField) == sizeof(CFtdcExecutionReportField));
            CHECK(sizeof(CXcpFtdcQryInitialDataField) == sizeof(CFtdcQryInitialDataField));
            CHECK(sizeof(CXcpFtdcDialogSpecField) == sizeof(CFtdcDialogSpecField));
            CHECK(sizeof(CXcpFtdcQrySecurityAccountField) == sizeof(CFtdcQrySecurityAccountField));
            CHECK(sizeof(CXcpFtdcSecurityAccountField) == sizeof(CFtdcSecurityAccountField));
		//CHECK(sizeof(CXcpFtdcOrderField) == sizeof(OrderField));
	}
}