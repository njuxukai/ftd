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
		//����ί�б�
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
		//�ж�ҵ��Ȩ��,У��۸������Ϸ��Ժ�ί�б���
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
	//���е���Ϣ����
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
		throw(MCO::IndexFindError("ί�������Ѵ���"));
	}

}
void insertToOrderInsert(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp) {}

void verifyInputOrderAndDeliverToExchange(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp) {}
