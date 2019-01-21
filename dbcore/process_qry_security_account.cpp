#include "ftdc_all.h"

using namespace genericdb;

void processQrySecurityAccount(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processQrySecurityAccount] called\n";
#endif
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue,
		sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspQrySecurityAccount> pRsp =
		std::make_shared<RspQrySecurityAccount>();
	pRsp->m_sequenceSeries = pReq->m_sequenceSeries;
	pRsp->m_sequenceNO = pReq->m_sequenceNO;

	ReqQrySecurityAccount *pReqQrySecurityAccount = (ReqQrySecurityAccount*)pReq.get();
	pRsp->requestSourceField.RequestID =
		pReqQrySecurityAccount->qrySecurityAccountField.RequestID;

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
	int searchInvestorID = pReqQrySecurityAccount->qrySecurityAccountField.InvestorID;
	mco_cursor_t csr;
	rc = SecurityAccount::InvestorIdx::cursor(t, &csr);
	if (MCO_S_OK == rc)
	{
		for (rc = SecurityAccount::InvestorIdx::search(t, &csr, MCO_EQ, searchInvestorID);
		     rc == MCO_S_OK; 
			rc = mco_cursor_next(t, &csr)) 
		{
			SecurityAccount account;
			account.from_cursor(t, &csr);
			if (account.investor_id != searchInvestorID)
			{
				break;
			}
			CFtdcSecurityAccountField accountField = { 0 };
			accountField.InvestorID = account.investor_id;
			accountField.ExchangeType = account.exchange_type;
			accountField.BrokerID = account.broker_id;
			strcpy(accountField.SecurityAccount, ((std::string)account.security_account).data());
			pRsp->securityAccountFields.push_back(accountField);
		}
		rc = MCO_S_CURSOR_END == rc ? MCO_S_OK : rc;
	}
	mco_trans_rollback(t);



	pWrapper->uplink(rspHeaders, pRsp);
}