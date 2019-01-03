#include "ftdc_all.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;

void processOrderInsertTransaction(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp)
{
	MCO_RET rc;
	OrderInsert orderInsert;
	orderInsert.create(t);
	orderInsert.front_id = pReq->inputOrderField.FrontID;
	orderInsert.session_id = pReq->inputOrderField.SessionID;
	orderInsert.order_ref = pReq->inputOrderField.OrderRef;
	rc = OrderInsert_SessionIdx_find(t, pReq->inputOrderField.FrontID, pReq->inputOrderField.SessionID,
		pReq->inputOrderField.OrderRef, &orderInsert);
	if (rc != MCO_S_NOTFOUND)
	{
		throw(MCO::IndexFindError("委托主键已存在"));
	}
	
}

Package* processOrderInsert(const Package* pReq, mco_db_h db)
{

	RspOrderInsert* pRsp = new RspOrderInsert();
	pRsp->m_header.sequenceNO = pReq->m_header.sequenceNO;
	pRsp->m_header.sequenceSeries = pReq->m_header.sequenceSeries;
	pRsp->m_header.transactionId = pReq->m_header.transactionId;

	McoTrans trans(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK != rc)
	{
		pRsp->pErrorField->ErrorCode = FTD_ERROR_CODE_TRANSACTION_ERROR;
		strcpy(pRsp->pErrorField->ErrorText, FTD_ERROR_TEXT_TRANSACTION_ERROR);
		return pRsp;
	}
	try
	{
		processOrderInsertTransaction((const ReqOrderInsert*)pReq, t, pRsp);
		mco_trans_commit(t);
	}
	catch (MCO::Exception& e)
	{
		mco_trans_rollback(t);
		pRsp->pErrorField->ErrorCode = e.errorCode;
		strcpy(pRsp->pErrorField->ErrorText, e.what());
	}
	return pRsp;
}