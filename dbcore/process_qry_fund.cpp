#include "ftdc_all.h"
#include "genericdb.hpp"
#include "Exceptions.h"
using namespace genericdb;

void processQryFundTransaction(const ReqQryFund* pReq, mco_trans_h t, RspQryFund* pRsp);

void processQryFund(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processQryFund] called\n";
#endif
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue, sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspQryFund> pRsp = std::make_shared<RspQryFund>();
	ReqQryFund *pReqQryFund = (ReqQryFund*)pReq.get();

	pRsp->m_sequenceSeries = pReq->m_sequenceSeries;
	pRsp->m_sequenceNO = pReq->m_sequenceNO;
	pRsp->pErrorField = CFtdcErrorFieldPtr(new CFtdcErrorField());
	memset(pRsp->pErrorField.get(), 0, sizeof(CFtdcErrorField));
	
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK != rc)
	{
		pRsp->pErrorField->ErrorCode = FTD_ERROR_CODE_TRANSACTION_ERROR;
		strcpy(pRsp->pErrorField->ErrorText, FTD_ERROR_TEXT_TRANSACTION_ERROR);
		pWrapper->uplink(rspHeaders, pRsp);
		return;
	}
	try
	{
		processQryFundTransaction(pReqQryFund, t, pRsp.get());
		mco_trans_commit(t);
	}
	catch (MCO::Exception& e)
	{
		mco_trans_rollback(t);
		pRsp->pErrorField->ErrorCode = e.errorCode;
		strcpy(pRsp->pErrorField->ErrorText, e.what());
	}

	pWrapper->uplink(rspHeaders, pRsp);
}


void processQryFundTransaction(const ReqQryFund* pReq, mco_trans_h t, RspQryFund* pRsp)
{
	Fund fund;
	MCO_RET rc = Fund_FundIdx_find(t, pReq->qryFundField.InvestorID,
		pReq->qryFundField.CurrencyType, &fund);
	if (MCO_S_OK != rc)
	{
		throw(MCO::IndexFindError("资金账号或币种不存在"));

	}
	CFtdcFundField ftdcFund = { 0 };
	ftdcFund.InvestorID = (int)fund.investor_id;
	ftdcFund.CurrencyType = (char)fund.currency_type;
	ftdcFund.AmountAvailable = (double)fund.amount_available;
	ftdcFund.AmountDrawable = (double)fund.amount_drawable;
	ftdcFund.AmountFrozen = (double)fund.amount_frozen;
	ftdcFund.AmountPre = (double)fund.amount_pre;
	pRsp->fundFields.push_back(ftdcFund);
}