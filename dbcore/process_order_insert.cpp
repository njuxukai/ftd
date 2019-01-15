#include "ftdc_all.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;

void InsertToOrderInsert(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp);
void insertToOrderInsert(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp);
void verifyInputOrderAndDeliverToExchange(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp);

void processOrderInsert(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
	PlainHeaders rspHeaders;
	std::shared_ptr<RspOrderInsert> pRsp = std::make_shared<RspOrderInsert>();

	pRsp->m_sequenceNO = pReq->m_sequenceNO;
	pRsp->m_sequenceSeries = pReq->m_sequenceSeries;
	pRsp->pErrorField = CFtdcErrorFieldPtr(new CFtdcErrorField());
	try 
	{
		//插入委托表
		{
			McoTrans t(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
			try
			{
				insertToOrderInsert((const ReqOrderInsert*)pReq.get(), (mco_trans_h)t, pRsp.get());
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
				verifyInputOrderAndDeliverToExchange((const ReqOrderInsert*)pReq.get(), (mco_trans_h)t, pRsp.get());
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
	//上行到消息队列
	pWrapper->uplink(rspHeaders, pRsp);
}


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
void insertToOrderInsert(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp) {}

void verifyInputOrderAndDeliverToExchange(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp) {}
