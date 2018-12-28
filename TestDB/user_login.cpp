#include "ftdc_all.h"
#include "genericdb.hpp"

using namespace genericdb;





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
	strcpy(pRsp->pErrorField->ErrorText, "µÇÂ¼³É¹¦");
	mco_trans_commit(t);
	return pRsp;
}