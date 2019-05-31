#include "ftdc_all.h"
#include "db_util.h"
#include "Exceptions.h"
#include "genericdb.hpp"

using namespace genericdb;

void processOrderActionTransaction(const ReqUserLogin* pReq, mco_trans_h t, RspUserLogin* pRsp);

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
	//1
}