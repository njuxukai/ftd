#include "ftdc_all.h"
#include "db_util.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;

void verifyAndUpdateFields(mco_trans_h t, FTD::CFtdcInnerExecutionReportField& innerReport, 
	FTD::CFtdcOrderField& order, FTD::CFtdcExecutionReportField& report);

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
	//1.2 ��ѯInnerExecutionReport 
	InnerExecutionReport dbInnerExecutionReport;
	rc = InnerExecutionReport::ExchangeIdx::find(t, innerReport.ExchangeType, innerReport.PbuID, dbOrder.order_sys_id, innerReport.ExecType, innerReport.ReportExchangeID, strlen(innerReport.ReportExchangeID), dbInnerExecutionReport);
	if (rc == MCO_S_OK)
	{
		throw(dbcore::IndexFindError("�ڲ��ɽ��ر��Ѵ���"));
	}

	//2 �����µ��ڲ�ExecutionReport
	dbInnerExecutionReport.create(t);
	//2.1 �� ExchangeType, PbuId �Ǵӻر���ȡ�û���ԭʼί����ȡ��
	dbInnerExecutionReport.ier_sys_id = get_next_sno(SEQ_INNER_EXECUTION_REPORT_TAG, t);
	dbInnerExecutionReport.exchange_type = innerReport.ExchangeType;
	dbInnerExecutionReport.pbu_id = innerReport.PbuID;
	dbInnerExecutionReport.exec_type = innerReport.ExecType;
	dbInnerExecutionReport.report_exchange_id = innerReport.ReportExchangeID;
	dbInnerExecutionReport.order_sys_id = dbOrder.order_sys_id;
	dbInnerExecutionReport.investor_id = dbOrder.investor_id;
	//
	dbInnerExecutionReport.volume_last = innerReport.VolumeLast;
	dbInnerExecutionReport.price_last = innerReport.PriceLast;
	dbInnerExecutionReport.volume_cum = innerReport.VolumeCum;
	dbInnerExecutionReport.volume_leaves = innerReport.VolumeLeaves;
	dbInnerExecutionReport.volume_cancelled = innerReport.VolumeCancelled;
	dbInnerExecutionReport.status = innerReport.OrderStatus;
	//2 ����order
	if (innerReport.ExecType == FTDC_ET_Trade)
	{
		dbOrder.volume_cum = dbOrder.volume_cum + dbInnerExecutionReport.volume_cum;
		dbOrder.amount_cum = dbOrder.amount_cum + dbInnerExecutionReport.volume_cum * dbInnerExecutionReport.price_last;
	}
	
	if (innerReport.ExecType == FTDC_ET_Trade && dbOrder.exchange_type == FTDC_ET_SH)
	{
		
		if (dbOrder.volume_cum == dbOrder.volume_total_original)
		{
			dbOrder.status = FTDC_OS_ALL_TRADED;
		}
		else
		{
			dbOrder.status = FTDC_OS_PART_TRADED;
		}
	}
	else
	{
		dbOrder.status = innerReport.OrderStatus;
	}
	//3 db ���첢���userreport
	UserExecutionReport dbUserReport;
	dbUserReport.create(t);
	dbUserReport.uer_sys_id = get_next_sno(SEQ_USER_EXECUTION_REPORT_TAG, t);
	dbUserReport.ier_sys_id = dbInnerExecutionReport.ier_sys_id;
	dbUserReport.exec_type = dbInnerExecutionReport.exec_type;
	dbUserReport.investor_id = dbOrder.investor_id;
	dbUserReport.order_sys_id = dbOrder.order_sys_id;
	dbUserReport.volume_cum = dbOrder.volume_cum;
	dbUserReport.amount_cum = dbOrder.amount_cum;
	dbUserReport.status = dbOrder.status;
	dbUserReport.front_id = dbOrder.front_id;
	dbUserReport.session_id = dbOrder.session_id;
	dbUserReport.order_ref = dbOrder.order_ref;
	dbUserReport.security_account = (std::string)dbOrder.security_account;
	dbUserReport.instrument_code = (std::string)dbOrder.instrument_code;
	dbUserReport.bs_flag = dbOrder.direction;
	dbUserReport.pbu_id = dbOrder.pbu_id;
	dbUserReport.branch_id = dbOrder.branch_id;
	//TODO timestamp important

	// ���report
	report.ReportSysID = dbUserReport.uer_sys_id;
	report.InvestorID = dbUserReport.investor_id;
	report.FrontID = dbUserReport.front_id;
	report.SessionID = dbUserReport.session_id;
	report.OrderRef = dbUserReport.order_ref;
	report.OrderSysID = dbUserReport.order_sys_id;
	strcpy(report.InstrumentCode, ((std::string)dbUserReport.instrument_code).data());
	report.ExchangeType = dbUserReport.exchange_type;
	report.Direction = dbUserReport.bs_flag;
	report.PriceType = dbUserReport.price_type;
	report.LimitPrice = dbUserReport.price;
	report.VolumeTotalOrginal = dbUserReport.volume;
	report.VolumeCum = dbUserReport.volume_cum;
	report.AmountCum = dbUserReport.amount_cum;
	report.OrderStatus = dbUserReport.status;
	report.SequenceSeries = dbUserReport.investor_id;
	report.SequenceNo = dbUserReport.uer_sys_id;
}

