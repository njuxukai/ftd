#include "ftdc_all.h"
#include "genericdb.hpp"

using namespace genericdb;

Package* processUserLogin(const Package* pReq, mco_db_h db)
{
	RspUserLogin *pRsp = new RspUserLogin();
	const ReqUserLogin* pReqUserLogin = (const ReqUserLogin*)pReq;
	pRsp->rspUserLoginField.BrokerID = pReqUserLogin->reqUserLoginField.BrokerID;
	pRsp->rspUserLoginField.UserID = pReqUserLogin->reqUserLoginField.UserID;
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK == rc)
	{
		User user;
		rc = User_idx_user_id_find(t, pReqUserLogin->reqUserLoginField.UserID, &user);
		if (MCO_S_OK == rc)
		{
			
		}
		else
		{
			mco_trans_rollback(t);
		}
	}
	return pRsp;
}