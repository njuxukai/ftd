#include "ftdc_all.h"
#include "db_util.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;

void verifyOriginalOrder(FTD::CFtdcInnerExecutionReportField* pInnerReport);
void insertToInnerExecutionReport(FTD::CFtdcInnerExecutionReportField* pInnerReport);
void formatIncExecutionReport();
void privateBoardcaseIncExecutionReport();

void processRptBackExectionReport(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processRptBackExecutionReport] called\n";
#endif
	bool needPrivatePush = false;

	ReqRptBackExecutionReport* pReqRptBackExecutionReport = (ReqRptBackExecutionReport*)pReq.get();
	FTD::CFtdcInnerExecutionReportField& innerReport = pReqRptBackExecutionReport->innerExecutionReportField;
	try
	{
		//1 找到原始的委托
		//2 根据原始委托，填充资金账号等信息后， 确认该成交回报不重复

		//3 插入内部执行报告表

		//4 形成客户执行报告记录，并插入客户执行报告表

		//5 广播客户执行报告

	}
	catch (dbcore::Exception& e)
	{

	}
	catch (McoException& e)
	{
	}
}