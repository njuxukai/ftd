#include "ftdc_all.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;

void InsertToOrderInsert(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp)
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
void insertToOrderInsert(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp);

void verifyInputOrderAndDeliverToExchange(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp);

Package* processOrderInsert(const Package* pReq, mco_db_h db)
{

	RspOrderInsert* pRsp = new RspOrderInsert();
	pRsp->m_header.sequenceNO = pReq->m_header.sequenceNO;
	pRsp->m_header.sequenceSeries = pReq->m_header.sequenceSeries;
	pRsp->m_header.transactionId = pReq->m_header.transactionId;
	pRsp->pErrorField = CFtdcErrorFieldPtr(new CFtdcErrorField());
	try 
	{
		//插入委托表
		{
			McoTrans t(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
			try
			{
				insertToOrderInsert((const ReqOrderInsert*)pReq, (mco_trans_h)t, pRsp);
			}
			catch (MCO::Exception& e)
			{
				t.rollback();
				throw;
			}
		}
		//判断业务权限,校验价格数量合法性后委托报送
		{
			McoTrans t(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
			try
			{
				verifyInputOrderAndDeliverToExchange((const ReqOrderInsert*)pReq, (mco_trans_h)t, pRsp);
			}
			catch (MCO::Exception& e)
			{
				t.rollback();
				throw;
			}
		}
	}
	catch (MCO::Exception& e)
	{
		pRsp->pErrorField->ErrorCode = e.errorCode;
		strcpy(pRsp->pErrorField->ErrorText, e.errorText.data());
	}
	catch (McoException e)
	{
	}
	return pRsp;
	
	

}