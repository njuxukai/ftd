#include "ftdc_all.h"
#include "db_util.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;

void verifyAndUpdateFields(mco_trans_h t, FTD::CFtdcInnerExecutionReportField& innerReport, 
	FTD::CFtdcOrderField& order, FTD::CFtdcExecutionReportField& report);
void updateOrder(mco_trans_h t,	FTD::CFtdcOrderField& order);
void insertInnerExecutionReport(mco_trans_h t, FTD::CFtdcInnerExecutionReportField& innerReport);
void insertIncExecutionReport(mco_trans_h t, FTD::CFtdcExecutionReportField& report);

void processRptBackExectionReport(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processRptBackExecutionReport] called\n";
#endif
	bool needPrivatePush = false;

	ReqRptBackExecutionReport* pReqRptBackExecutionReport = (ReqRptBackExecutionReport*)pReq.get();
	FTD::CFtdcInnerExecutionReportField& innerReport = pReqRptBackExecutionReport->innerExecutionReportField;

	FTD::CFtdcOrderField order = { 0 };
	std::shared_ptr<IncExecutionReport>  pIncExecutionReport =
		std::shared_ptr<IncExecutionReport>(new IncExecutionReport());
	try
	{
		//1 �ҵ�ԭʼ��ί��,����ԭʼί�У�����ʽ��˺ŵ���Ϣ�� ȷ�ϸóɽ��ر����ظ�
		//2 ��ԭʼί�м�¼����
		//3 �����ڲ�ִ�б����
		//4 �γɿͻ�ִ�б����¼��������ͻ�ִ�б����
		//5 �㲥�ͻ�ִ�б���
		{
			McoTrans t(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
			try
			{
				verifyAndUpdateFields(t, innerReport, order, pIncExecutionReport->executionReportField);
				updateOrder(t, order);
				insertInnerExecutionReport(t, innerReport);
				insertIncExecutionReport(t, pIncExecutionReport->executionReportField);
				needPrivatePush = true;
			}
			catch (...)
			{
				t.rollback();
				throw;
			}
		}

	}
	catch (dbcore::Exception& e)
	{

	}
	catch (McoException& e)
	{
	}

	if (needPrivatePush)
	{
		PlainHeaders privateHeaders = { 0 };
		privateHeaders.admin_flag = QMSG_FLAG_APP;
		privateHeaders.msg_type = QMSG_TYPE_PRIVATE;
		privateHeaders.sequence_series = pIncExecutionReport->executionReportField.SequenceSeries;
		pWrapper->uplink(privateHeaders, pIncExecutionReport);
	}
}

void verifyAndUpdateFields(mco_trans_h t, FTD::CFtdcInnerExecutionReportField& innerReport,
	FTD::CFtdcOrderField& order, FTD::CFtdcExecutionReportField& report)
{
	MCO_RET rc;
	Order dbOrder;
	//1 ��ѯorder
	rc = Order_clOrdIDIdx_find(t, innerReport.ClOrdID, strlen(innerReport.ClOrdID), &dbOrder);
	if (rc != MCO_S_OK)
	{
		throw(dbcore::IndexFindError("δ�ҵ���Ӧ��ί��"));
	}
	//2 ��ѯ
	InnerExecutionReport dbInnerExecutionReport;
	dbInnerExecutionReport.create(t);
	dbInnerExecutionReport.ier_sys_id = get_next_sno(SEQ_INNER_EXECUTION_REPORT_TAG, t);
	dbInnerExecutionReport.exchange_type = innerReport.ExchangeType;
	//pbuid �Ǵӻر���ȡ�û���ԭʼί����ȡ��
	dbInnerExecutionReport.pbu_id = innerReport.PbuID;
	dbInnerExecutionReport.order_sys_id = dbOrder.order_sys_id;
	dbInnerExecutionReport.exec_type = innerReport.ExecType;
	dbInnerExecutionReport.report_exchange_id = innerReport.ReportExchangeID;
	//2 ����order��Ϣ
	dbOrder.volume_cum
	//3 ��ȫinnerReport��Ϣ

	//4 ���report
}

void updateOrder(mco_trans_h t, FTD::CFtdcOrderField& order)
{
}

void insertInnerExecutionReport(mco_trans_h t, FTD::CFtdcInnerExecutionReportField& innerReport)
{
}

void insertIncExecutionReport(mco_trans_h t, FTD::CFtdcExecutionReportField& report)
{
}