#include "ftdc_all.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;


void insertToOrderInsert(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp);
void verifyInputOrderAndDeliverToExchange(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp);

void processOrderInsert(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processOrderInsert] called\n";
#endif

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
	try 
	{
		//插入委托表
		try
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
			catch (McoException e)
			{
				pRsp->pErrorField->ErrorCode = -1;
				strncpy(pRsp->pErrorField->ErrorText, e.what(), sizeof(CFtdcErrorField::ErrorText));
			}
		}
		catch (std::exception& e)
		{
			throw;
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
	catch (McoException& e)
	{
		pRsp->pErrorField->ErrorCode = -1;
		strncpy(pRsp->pErrorField->ErrorText, e.what(), sizeof(CFtdcErrorField::ErrorText));
	}
	//上行到消息队列
	pWrapper->uplink(rspHeaders, pRsp);
}


void insertToOrderInsert(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp)
{
	MCO_RET rc;
	InputOrder orderInsert;
	/*
	rc = InputOrder_SessionIdx_find(t, pReq->inputOrderField.FrontID, pReq->inputOrderField.SessionID,
		pReq->inputOrderField.OrderRef, &orderInsert);
	if (rc != MCO_S_NOTFOUND)
	{
		throw(MCO::IndexFindError("委托主键已存在"));
	}
	*/
	orderInsert.create(t);
	orderInsert.front_id = pReq->inputOrderField.FrontID;
	orderInsert.session_id = pReq->inputOrderField.SessionID;
	orderInsert.order_ref = pReq->inputOrderField.OrderRef;
}


void verifyInputOrderAndDeliverToExchange(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp) {}
