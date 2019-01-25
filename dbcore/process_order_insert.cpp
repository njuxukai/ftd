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
	std::cout << pReqOrderInsert->inputOrderField.FrontID
		<< ":" << pReqOrderInsert->inputOrderField.SessionID
		<< ":" << pReqOrderInsert->inputOrderField.OrderRef << std::endl;
	memcpy(&pRsp->inputOrderField, &pReqOrderInsert->inputOrderField, sizeof(CFtdcInputOrderField));
	try 
	{
		//插入委托表
		{
			McoTrans t(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
			try
			{
				insertToOrderInsert((const ReqOrderInsert*)pReq.get(), t, pRsp.get());
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

	/*
	orderInsert.create(t);
	orderInsert.front_id = pReq->inputOrderField.FrontID;
	orderInsert.session_id = pReq->inputOrderField.SessionID;
	orderInsert.order_ref = pReq->inputOrderField.OrderRef;
	*/
	Order order;
	rc = Order_SessionIdx_find(t, pReq->inputOrderField.FrontID,
		pReq->inputOrderField.SessionID,
		pReq->inputOrderField.OrderRef,
		&order);
	if (rc == MCO_S_OK)
	{
		throw(dbcore::IndexFindError("存在相同主键的Order"));
	}
	order.create(t);
	order.front_id = pReq->inputOrderField.FrontID;
	order.session_id = pReq->inputOrderField.SessionID;
	order.order_ref = pReq->inputOrderField.OrderRef;
	order.investor_id = pReq->inputOrderField.InvestorID;
}


void verifyInputOrderAndDeliverToExchange(const ReqOrderInsert* pReq, mco_trans_h t, RspOrderInsert* pRsp) {}
