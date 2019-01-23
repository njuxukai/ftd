#include "ftdc_all.h"
#include "genericdb.hpp"
#include "Exceptions.h"
using namespace genericdb;




/*
Package* processUserLogin(const Package* pReq, mco_db_h db)
{
	RspUserLogin *pRsp = new RspUserLogin();
	const ReqUserLogin* pReqUserLogin = (const ReqUserLogin*)pReq;
	pRsp->rspUserLoginField.BrokerID = pReqUserLogin->reqUserLoginField.BrokerID;
	pRsp->rspUserLoginField.UserID = pReqUserLogin->reqUserLoginField.UserID;
	pRsp->pErrorField = CFtdcErrorFieldPtr(new CFtdcErrorField());
	memset(pRsp->pErrorField.get(), 0, sizeof(CFtdcErrorField));
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK != rc)
	{
		pRsp->pErrorField->ErrorCode = FTD_ERROR_CODE_TRANSACTION_ERROR;
		strcpy(pRsp->pErrorField->ErrorText, FTD_ERROR_TEXT_TRANSACTION_ERROR);
		return pRsp;
	}

	User user;
	rc = User_idx_user_id_find(t, pReqUserLogin->reqUserLoginField.UserID, &user);
	if (MCO_S_OK != rc)
	{
		pRsp->pErrorField->ErrorCode = FTD_ERROR_CODE_USER_ID_NOT_EXIST;
		strcpy(pRsp->pErrorField->ErrorText, FTD_ERROR_TEXT_USER_ID_NOT_EXIST);
		mco_trans_rollback(t);
		return pRsp;

	}
	if (strcmp(((std::string)user.password).c_str(), pReqUserLogin->reqUserLoginField.Password) != 0)
	{
		pRsp->pErrorField->ErrorCode = FTD_ERROR_CODE_PASSWORD_ERROR;
		strcpy(pRsp->pErrorField->ErrorText, FTD_ERROR_TEXT_PASSWORD_ERROR);
		mco_trans_rollback(t);
		return pRsp;
	}

	pRsp->pErrorField->ErrorCode = 0;
	strcpy(pRsp->pErrorField->ErrorText, "登录成功");
	mco_trans_commit(t);
	return pRsp;
}
*/

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
	InputOrder::SessionIdx::search(t, &csr, MCO_GT, targetFrontID, targetSessionID, 0);
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
		processUserLoginTransaction(pReqUserLogin, t, pRsp.get());
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