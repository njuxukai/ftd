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
		//1 �ҵ�ԭʼ��ί��
		//2 ����ԭʼί�У�����ʽ��˺ŵ���Ϣ�� ȷ�ϸóɽ��ر����ظ�

		//3 �����ڲ�ִ�б����

		//4 �γɿͻ�ִ�б����¼��������ͻ�ִ�б����

		//5 �㲥�ͻ�ִ�б���

	}
	catch (dbcore::Exception& e)
	{

	}
	catch (McoException& e)
	{
	}
}