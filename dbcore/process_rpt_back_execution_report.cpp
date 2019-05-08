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
		//1 找到原始的委托,根据原始委托，填充资金账号等信息后， 确认该成交回报不重复
		//2 对原始委托记录更新
		//3 插入内部执行报告表
		//4 形成客户执行报告记录，并插入客户执行报告表
		//5 广播客户执行报告
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
	//1 查询order
	rc = Order_clOrdIDIdx_find(t, innerReport.ClOrdID, strlen(innerReport.ClOrdID), &dbOrder);
	if (rc != MCO_S_OK)
	{
		throw(dbcore::IndexFindError("未找到相应的委托"));
	}
	//1.2 查询InnerExecutionReport 
	InnerExecutionReport dbInnerExecutionReport;
	rc = InnerExecutionReport::ExchangeIdx::find(t, innerReport.ExchangeType, innerReport.PbuID, dbOrder.order_sys_id, innerReport.ExecType, innerReport.ReportExchangeID, strlen(innerReport.ReportExchangeID), dbInnerExecutionReport);
	if (rc == MCO_S_OK)
	{
		throw(dbcore::IndexFindError("内部成交回报已存在"));
	}

	//2 构建新的内部ExecutionReport
	dbInnerExecutionReport.create(t);
	//2.1 键 ExchangeType, PbuId 是从回报中取得还是原始委托中取得
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
	//2 更新order信息
	dbOrder.status = innerReport.OrderStatus;
	if (innerReport.ExecType == FTDC_ET_Trade)
	{
		dbOrder.volume_cum = dbOrder.volume_cum + dbInnerExecutionReport.volume_cum;
		dbOrder.amount_cum = dbOrder.amount_cum + dbInnerExecutionReport.volume_cum * dbInnerExecutionReport.price_last;
	}
	//3 补全innerReport信息

	//4 填充report
	UserExecutionReport dbUserReport;
	dbUserReport.create(t);

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