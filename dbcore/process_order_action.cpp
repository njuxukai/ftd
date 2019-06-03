#include "ftdc_all.h"
#include "db_util.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;

void verifyAndUpdateOrderStatus(mco_trans_h t, FTD::CFtdcInputOrderActionField& inputOrderAction);

void processOrderAction(const PlainHeaders& headers, FTD::PackageSPtr pPackage, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processOrderAction] called\n";
#endif
	bool needReport = false;

	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue,
		sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspOrderAction> pRsp =
		std::make_shared<RspOrderAction>();
	pRsp->m_sequenceSeries = pPackage->m_sequenceSeries;
	pRsp->m_sequenceNO = pPackage->m_sequenceNO;
	pRsp->pErrorField = CFtdcErrorFieldPtr(new CFtdcErrorField());

	ReqOrderAction *pReqOrderAction = (ReqOrderAction*)pPackage.get();
	memcpy(&pRsp->inputOrderActionField, &pReqOrderAction->inputOrderActionField, sizeof(CFtdcInputOrderActionField));

	try
	{
		//找到原始委托，根据原始委托状态
		// 1 非终结状态 2 未撤单 3 交易所主键信息存在
		// 上述三个条件都满足，填充委托主键信息后报盘撤单
		{
			McoTrans t(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
			try
			{
				verifyAndUpdateOrderStatus(t, pReqOrderAction->inputOrderActionField);
				needReport = true;
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

	// 上行到消息队列
	pWrapper->uplink(rspHeaders, pRsp);

	//报盘
	if (needReport)
	{
		PlainHeaders rptHeaders = { 0 };
		rptHeaders.admin_flag = QMSG_FLAG_APP;
		rptHeaders.msg_type = QMSG_TYPE_REQ;
		strcpy(rptHeaders.source_queue, pReqOrderAction->inputOrderActionField.RptQueue);
		std::shared_ptr<ReqRptOrderAction> pReqRptOrderAction = std::make_shared<ReqRptOrderAction>();
		memcpy(&pReqRptOrderAction->inputOrderActionField, &pReqOrderAction->inputOrderActionField, sizeof(CFtdcInputOrderActionField));
		std::cout << "Send Report[Action] to " << rptHeaders.source_queue << std::endl;
		pWrapper->uplink(rptHeaders, pReqRptOrderAction);
	}
}

void verifyAndUpdateOrderStatus(mco_trans_h t, FTD::CFtdcInputOrderActionField& inputOrderAction)
{

	MCO_RET rc;
	Order dbOrder;

	if (inputOrderAction.FrontID != 0)
	{
		rc = Order::SessionIdx::find(t, inputOrderAction.FrontID, inputOrderAction.SessionID, inputOrderAction.OrderRef, dbOrder);

	}
	else if (inputOrderAction.OrderSysID != 0)
	{
		rc = Order::SysIdx::find(t, inputOrderAction.OrderSysID, dbOrder);

	}

	
	else if (strlen(inputOrderAction.OrderExchangeID) != 0)
	{
		mco_cursor_t csr;
		rc = Order::ExchangeIdx::search(t, &csr, MCO_EQ, inputOrderAction.ExchangeType, inputOrderAction.InvestorID, inputOrderAction.OrderExchangeID, strlen(inputOrderAction.OrderExchangeID));
		if (rc == MCO_S_OK)
		dbOrder.from_cursor(t, &csr);
	}
	
	if (rc != MCO_S_OK)
	{
		throw(dbcore::IndexFindError("未找到相应的原始委托"));
	}

	if (dbOrder.status == FTDC_OS_ALL_TRADED ||
		dbOrder.status == FTDC_OS_CANCELLED ||
		dbOrder.status == FTDC_OS_REJECTED)
	{
		throw(dbcore::StatusError("原始委托已经是终结状态"));
	}

	if (dbOrder.cancel_flag == FTDC_TCF_Cancel)
	{
		throw(dbcore::StatusError("原始委托撤单标志为待撤"));
	}
	strcpy(inputOrderAction.ClOrdID, ((std::string)dbOrder.client_order_id).data());
	strcpy(inputOrderAction.ActionClOrdID, generate_client_order_id(get_next_sno(SEQ_ORDER_TAG, t)).data());
	inputOrderAction.VolumeTotalOrginal = dbOrder.volume_total_original;
	inputOrderAction.Direction = dbOrder.direction;
	inputOrderAction.ExchangeType = dbOrder.exchange_type;
	strcpy(inputOrderAction.SecurityAccount, ((std::string)dbOrder.security_account).data());
	strcpy(inputOrderAction.InstrumentCode, ((std::string)dbOrder.instrument_code).data());
	strcpy(inputOrderAction.OrderExchangeID, ((std::string)dbOrder.order_exchange_id).data());
	inputOrderAction.PbuID = dbOrder.pbu_id;
	strcpy(inputOrderAction.RptQueue, ((std::string)dbOrder.rpt_queue).data());
	dbOrder.cancel_flag = FTDC_TCF_Cancel;
}