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
            CHECK(sizeof(CXcpFtdcSecurityAccountField) >= 0);
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
            CHECK(sizeof(CXcpFtdcQryPurchasableNewSecurityField) >= 0);
            CHECK(sizeof(CXcpFtdcPurchasableNewIssueSecurityField) >= 0);
            CHECK(sizeof(CXcpFtdcQryPurchaseQuotaField) >= 0);
            CHECK(sizeof(CXcpFtdcPurchaseQuotaField) >= 0);
            CHECK(sizeof(CXcpFtdcExecutionReportField) >= 0);
            CHECK(sizeof(CXcpFtdcQryInitialDataField) >= 0);

            CHECK(sizeof(CXcpFtdcErrorField) == sizeof(ErrorField));
            CHECK(sizeof(CXcpFtdcRequestSourceField) == sizeof(RequestSourceField));
            CHECK(sizeof(CXcpFtdcReqUserLoginField) == sizeof(ReqUserLoginField));
            CHECK(sizeof(CXcpFtdcDissenminationstartField) == sizeof(DissenminationstartField));
            CHECK(sizeof(CXcpFtdcRspUserLoginField) == sizeof(RspUserLoginField));
            CHECK(sizeof(CXcpFtdcSecurityAccountField) == sizeof(SecurityAccountField));
            CHECK(sizeof(CXcpFtdcReqUserLogoutField) == sizeof(ReqUserLogoutField));
            CHECK(sizeof(CXcpFtdcRspUserLogoutField) == sizeof(RspUserLogoutField));
            CHECK(sizeof(CXcpFtdcForceExitField) == sizeof(ForceExitField));
            CHECK(sizeof(CXcpFtdcInputOrderField) == sizeof(InputOrderField));
            CHECK(sizeof(CXcpFtdcInputOrderActionField) == sizeof(InputOrderActionField));
            CHECK(sizeof(CXcpFtdcInputFundTransferField) == sizeof(InputFundTransferField));
            CHECK(sizeof(CXcpFtdcFundTransferField) == sizeof(FundTransferField));
            CHECK(sizeof(CXcpFtdcQryFundField) == sizeof(QryFundField));
            CHECK(sizeof(CXcpFtdcFundField) == sizeof(FundField));
            CHECK(sizeof(CXcpFtdcQryPositionField) == sizeof(QryPositionField));
            CHECK(sizeof(CXcpFtdcPositionField) == sizeof(PositionField));
            CHECK(sizeof(CXcpFtdcQryOrderField) == sizeof(QryOrderField));
            CHECK(sizeof(CXcpFtdcOrderField) == sizeof(OrderField));
            CHECK(sizeof(CXcpFtdcQryTradeField) == sizeof(QryTradeField));
            CHECK(sizeof(CXcpFtdcTradeField) == sizeof(TradeField));
            CHECK(sizeof(CXcpFtdcQryFundTransferField) == sizeof(QryFundTransferField));
            CHECK(sizeof(CXcpFtdcQryHisOrderField) == sizeof(QryHisOrderField));
            CHECK(sizeof(CXcpFtdcQryHisTradeField) == sizeof(QryHisTradeField));
            CHECK(sizeof(CXcpFtdcQryHisFundTransferField) == sizeof(QryHisFundTransferField));
            CHECK(sizeof(CXcpFtdcQryInstrumentField) == sizeof(QryInstrumentField));
            CHECK(sizeof(CXcpFtdcInstrumentField) == sizeof(InstrumentField));
            CHECK(sizeof(CXcpFtdcQryStructuredFundField) == sizeof(QryStructuredFundField));
            CHECK(sizeof(CXcpFtdcStructuredFundField) == sizeof(StructuredFundField));
            CHECK(sizeof(CXcpFtdcQryETFField) == sizeof(QryETFField));
            CHECK(sizeof(CXcpFtdcETFField) == sizeof(ETFField));
            CHECK(sizeof(CXcpFtdcQryETFCompositionField) == sizeof(QryETFCompositionField));
            CHECK(sizeof(CXcpFtdcETFCompositionField) == sizeof(ETFCompositionField));
            CHECK(sizeof(CXcpFtdcQryPurchasableNewSecurityField) == sizeof(QryPurchasableNewSecurityField));
            CHECK(sizeof(CXcpFtdcPurchasableNewIssueSecurityField) == sizeof(PurchasableNewIssueSecurityField));
            CHECK(sizeof(CXcpFtdcQryPurchaseQuotaField) == sizeof(QryPurchaseQuotaField));
            CHECK(sizeof(CXcpFtdcPurchaseQuotaField) == sizeof(PurchaseQuotaField));
            CHECK(sizeof(CXcpFtdcExecutionReportField) == sizeof(ExecutionReportField));
            CHECK(sizeof(CXcpFtdcQryInitialDataField) == sizeof(QryInitialDataField));
		//CHECK(sizeof(CXcpFtdcOrderField) == sizeof(OrderField));
	}
}