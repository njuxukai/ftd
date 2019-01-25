#include "ftdc_all.h"
#include "db_util.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;


void insertToInputOrder(ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp);
void verifyInputAndCreateOrder(ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp, bool& needPrivatePush, IncExecutionReport* privateReports);

void processOrderInsert(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processOrderInsert] called\n";
#endif
	bool needReport = false;
	bool needPrivatePush = false;

	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue,
		sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspOrderInsert> pRsp = std::make_shared<RspOrderInsert>();
	pRsp->m_sequenceNO = pReq->m_sequenceNO;
	pRsp->m_sequenceSeries = pReq->m_sequenceSeries;
	pRsp->pErrorField = CFtdcErrorFieldPtr(new CFtdcErrorField());
	ReqOrderInsert* pReqOrderInsert = (ReqOrderInsert*)pReq.get();
	memcpy(&pRsp->inputOrderField, &pReqOrderInsert->inputOrderField, sizeof(CFtdcInputOrderField));

	std::shared_ptr<IncExecutionReport>  pIncExecutionReport =
		std::shared_ptr<IncExecutionReport>(new IncExecutionReport());
	try 
	{
		//插入客户委托表
		{
			McoTrans t(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
			try
			{
				insertToInputOrder((ReqOrderInsert*)pReq.get(), t, pRsp.get());
			}
			catch (...)
			{
				t.rollback();
				throw;
			}

		}
		//插入委托表
		{
			McoTrans t(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
			try
			{
				verifyInputAndCreateOrder(
					(ReqOrderInsert*)pReq.get(), 
					t, 
					pRsp.get(),
					needPrivatePush,
					pIncExecutionReport.get()
					);
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
		pRsp->pErrorField->ErrorCode = e.errorCode;
		strcpy(pRsp->pErrorField->ErrorText, e.what());
	}
	catch (McoException& e)
	{
		pRsp->pErrorField->ErrorCode = e.get_rc();
		strncpy(pRsp->pErrorField->ErrorText, e.what(), sizeof(CFtdcErrorField::ErrorText));
	}
	//上行到消息队列
	pWrapper->uplink(rspHeaders, pRsp);

	//报盘
	PlainHeaders rptHeaders = { 0 };
	rptHeaders.admin_flag = QMSG_FLAG_APP;
	rptHeaders.msg_type = QMSG_TYPE_REQ;
	//strncpy(rspHeaders.source_queue, headers.target_queue,
	//	sizeof(rspHeaders.source_queue));
	//推送
	if (needPrivatePush)
	{
		PlainHeaders privateHeaders = { 0 };
		privateHeaders.admin_flag = QMSG_FLAG_APP;
		privateHeaders.msg_type = QMSG_TYPE_PRIVATE;
		privateHeaders.sequence_series = pIncExecutionReport->executionReportField.SequenceSeries;
		pWrapper->uplink(privateHeaders, pIncExecutionReport);
	}
}


void insertToInputOrder(ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp)
{
	MCO_RET rc;
	InputOrder orderInsert;
	rc = InputOrder_SessionIdx_find(t, pReq->inputOrderField.FrontID, pReq->inputOrderField.SessionID,
		pReq->inputOrderField.OrderRef, &orderInsert);
	if (rc == MCO_S_OK)
	{
		throw(dbcore::IndexFindError("委托主键已存在"));
	}
	orderInsert.create(t);
	orderInsert.front_id = pReq->inputOrderField.FrontID;
	orderInsert.session_id = pReq->inputOrderField.SessionID;
	orderInsert.order_ref = pReq->inputOrderField.OrderRef;
	orderInsert.investor_id = pReq->inputOrderField.InvestorID;
	orderInsert.security_account = pReq->inputOrderField.SecurityAccount;
	orderInsert.exchange_type = pReq->inputOrderField.ExchangeType;
	orderInsert.instrument_code = pReq->inputOrderField.InstrumentCode;
	orderInsert.direction = pReq->inputOrderField.Direction;
	orderInsert.price_type = pReq->inputOrderField.PriceType;
	orderInsert.price = pReq->inputOrderField.LimitPrice;
	orderInsert.volume = pReq->inputOrderField.VolumeTotalOrginal;
}


void verifyInputAndCreateOrder(ReqOrderInsert* pReq, 
	mco_trans_h t, 
	RspOrderInsert* pRsp,
	bool& needPrivatePush, 
	IncExecutionReport* privateReport)
{
	//验证权限，数量，价格，时间等
	//TODO

	//
	MCO_RET rc;
	Order order;
	rc = Order_SessionIdx_find(t, pReq->inputOrderField.FrontID, pReq->inputOrderField.SessionID,
		pReq->inputOrderField.OrderRef, &order);
	if (rc == MCO_S_OK)
	{
		throw(dbcore::IndexFindError("委托主键已存在"));
	}

	// InputOrder信息填充
	
	///1 补充pbu信息
	SecurityAccount securityAccount;
	rc = SecurityAccount::Idx::find(t, 
		pReq->inputOrderField.InvestorID, 
		pReq->inputOrderField.ExchangeType, 
		pReq->inputOrderField.SecurityAccount, 10,
		securityAccount);
	if (rc == MCO_S_NOTFOUND)
	{
		throw(dbcore::IndexFindError("找不到相应的股东账号"));
	}
	pReq->inputOrderField.PbuID = securityAccount.pbu_id;
	///2 补充营业部编码信息
	Investor investor;
	rc = Investor::Idx::find(t, pReq->inputOrderField.InvestorID, investor);
	if (rc == MCO_S_NOTFOUND)
		throw(dbcore::IndexFindError("找不到相应的资金账号"));
	int branchID = investor.branch_id;
	Branch branch;
	rc = Branch::Idx::find(t, branchID, branch);
	if (rc == MCO_S_NOTFOUND)
		throw(dbcore::IndexFindError("找不到相应的营业部代码"));
	pReq->inputOrderField.BranchID = branch.branch_id;
	if (pReq->inputOrderField.ExchangeType == FTDC_ET_SH)
		pReq->inputOrderField.ExchangeBranchID = branch.sh_branch_id;
	else
		pReq->inputOrderField.ExchangeBranchID = branch.sz_branch_id;
	///3 增加系统主键
		pReq->inputOrderField.OrderSysID = get_next_sno(SEQ_ORDER_TAG, t);
	strcpy(pReq->inputOrderField.ClOrdID, 
		generate_client_order_id(pReq->inputOrderField.OrderSysID).data());


	order.create(t);
	order.front_id = pReq->inputOrderField.FrontID;
	order.session_id = pReq->inputOrderField.SessionID;
	order.order_ref = pReq->inputOrderField.OrderRef;
	order.investor_id = pReq->inputOrderField.InvestorID;
	order.security_account = pReq->inputOrderField.SecurityAccount;
	order.exchange_type = pReq->inputOrderField.ExchangeType;
	order.instrument_code = pReq->inputOrderField.InstrumentCode;
	order.direction = pReq->inputOrderField.Direction;
	order.price_type = pReq->inputOrderField.PriceType;
	order.price = pReq->inputOrderField.LimitPrice;
	order.volume_total_original = pReq->inputOrderField.VolumeTotalOrginal;
	order.status = FTDC_OS_CREATED;
	order.order_sys_id = pReq->inputOrderField.OrderSysID;
	order.client_order_id = pReq->inputOrderField.ClOrdID;
	order.pbu_id = pReq->inputOrderField.PbuID;
	order.branch_id = pReq->inputOrderField.BranchID;
	order.exchange_branch_id = pReq->inputOrderField.ExchangeBranchID;
	

	UserExecutionReport userReport;
	userReport.create(t);
	userReport.exec_type = FTDC_ET_Create;
	userReport.uer_sys_id = get_next_sno(SEQ_USER_EXECUTION_REPORT_TAG, t);
	userReport.front_id = pReq->inputOrderField.FrontID;
	userReport.session_id = pReq->inputOrderField.SessionID;
	userReport.order_ref = pReq->inputOrderField.OrderRef;
	userReport.order_sys_id = pReq->inputOrderField.OrderSysID;
	userReport.ier_sys_id = 0;
	userReport.investor_id = pReq->inputOrderField.InvestorID;
	userReport.security_account = pReq->inputOrderField.SecurityAccount;
	userReport.exchange_type = pReq->inputOrderField.ExchangeType;
	userReport.instrument_code = pReq->inputOrderField.InstrumentCode;
	userReport.bs_flag = pReq->inputOrderField.Direction;
	userReport.price_type = pReq->inputOrderField.PriceType;
	userReport.price = pReq->inputOrderField.LimitPrice;
	userReport.volume = pReq->inputOrderField.VolumeTotalOrginal;
	userReport.status = FTDC_OS_CREATED;
	userReport.order_sys_id = pReq->inputOrderField.OrderSysID;
	userReport.pbu_id = pReq->inputOrderField.PbuID;
	userReport.branch_id = pReq->inputOrderField.BranchID;
	
	needPrivatePush = true;
	CFtdcExecutionReportField& reportField = privateReport->executionReportField;
	reportField.FrontID = userReport.front_id;
	reportField.SessionID = userReport.session_id;
	reportField.OrderRef = userReport.order_ref;
	reportField.OrderSysID = userReport.order_sys_id;
	strcpy(reportField.InstrumentCode, ((std::string)userReport.instrument_code).data());
	reportField.ExchangeType = userReport.exchange_type;
	reportField.Direction = userReport.bs_flag;
	reportField.InvestorID = userReport.investor_id;
	reportField.SequenceNo = userReport.uer_sys_id;
	reportField.SequenceSeries = userReport.investor_id;
	reportField.PriceType = userReport.price_type;
	reportField.LimitPrice = userReport.price;
	reportField.VolumeTotalOrginal = userReport.volume;
	reportField.OrderStatus = userReport.status;
	reportField.VolumeCum = userReport.volume_cum;
	reportField.AmountCum = userReport.amount_cum;
	reportField.ReportSysID = userReport.uer_sys_id;

}
