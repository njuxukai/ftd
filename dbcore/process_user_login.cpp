#include "ftdc_all.h"
#include "genericdb.hpp"
#include "Exceptions.h"
using namespace genericdb;



void processUserLoginTransaction(const ReqUserLogin* pReq, mco_trans_h t, RspUserLogin* pRsp);

void processUserLogin(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processUserLogin] called\n";
#endif
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_ADMIN;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue, sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;
	std::shared_ptr<RspUserLogin> pRsp = std::make_shared<RspUserLogin>();
	const ReqUserLogin* pReqUserLogin = (const ReqUserLogin*)pReq.get();
	pRsp->rspUserLoginField.BrokerID = pReqUserLogin->reqUserLoginField.BrokerID;
	pRsp->rspUserLoginField.UserID = pReqUserLogin->reqUserLoginField.UserID;
	pRsp->pErrorField = CFtdcErrorFieldPtr(new CFtdcErrorField());
	memset(pRsp->pErrorField.get(), 0, sizeof(CFtdcErrorField));
	pRsp->rspUserLoginField.HeartbeatInterval = 10;
	try
	{
		McoTrans t(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND);
		try
		{
			processUserLoginTransaction(pReqUserLogin, t, pRsp.get());
		}
		catch (...)
		{
			t.rollback();
			throw;
		}
	}
	catch (MCO::Exception& e)
	{
		pRsp->pErrorField->ErrorCode = e.errorCode;
		strcpy(pRsp->pErrorField->ErrorText, e.what());
	}
	catch (McoException& e)
	{
		pRsp->pErrorField->ErrorCode = e.get_rc();
		strncpy(pRsp->pErrorField->ErrorText, e.what(), sizeof(CFtdcErrorField::ErrorText));
	}

	pWrapper->uplink(rspHeaders, pRsp);
}

void processUserLoginTransaction(const ReqUserLogin* pReq, mco_trans_h t, RspUserLogin* pRsp)
{
	User user;
	MCO_RET rc = User_UserIdIdx_find(t, pReq->reqUserLoginField.UserID, &user);
	if (MCO_S_OK != rc)
	{
		throw(MCO::IndexFindError("用户不存在"));

	}
	if (strcmp(((std::string)user.password).c_str(), pReq->reqUserLoginField.Password) != 0)
	{
		throw(MCO::AuthorizationDenied("密码错误"));
	}
	//查max order ref
	int maxOrderRef = 1;
	int targetFrontID = pReq->reqUserLoginField.FrontID;
	int targetSessionID = pReq->reqUserLoginField.SessionID;
	mco_cursor_t csr;
	rc = InputOrder::SessionIdx::cursor(t, &csr);
	rc = InputOrder::SessionIdx::search(t, &csr, MCO_GT, targetFrontID, targetSessionID, 0);
	for (; rc == MCO_S_OK; rc = mco_cursor_next(t, &csr))
	{
		InputOrder inputOrder;
		inputOrder.from_cursor(t, &csr);
		if (inputOrder.front_id != targetFrontID ||
			inputOrder.session_id != targetSessionID)
			break;
		if (inputOrder.order_ref >= maxOrderRef)
			maxOrderRef = inputOrder.order_ref + 1;
	}
	pRsp->rspUserLoginField.MaxOrderRef = maxOrderRef;
	//查交易日期
	SysConfig config;
	rc = SysConfig::Idx::find(t, 0, config);
	if (MCO_S_OK != rc)
	{
		throw(MCO::IndexFindError("交易日期不存在"));
	}
	strcpy(pRsp->rspUserLoginField.TradingDay, ((std::string)config.trade_date).data());
	pRsp->pErrorField->ErrorCode = 0;
	strcpy(pRsp->pErrorField->ErrorText, "登录成功");
}
