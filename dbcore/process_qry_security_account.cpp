#include "ftdc_all.h"

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


		CFtdcSecurityAccountField shAccount = { 0 };
		shAccount.BrokerID = pReqQrySecurityAccount->qrySecurityAccountField.BrokerID;
		shAccount.InvestorID = pReqQrySecurityAccount->qrySecurityAccountField.InvestorID;
		shAccount.ExchangeType = FTDC_ET_SH;
		strcpy(shAccount.SecurityAccount, "A000000001");
		pRsp->securityAccountFields.push_back(shAccount);

		CFtdcSecurityAccountField szAccount = { 0 };
		szAccount.BrokerID = pReqQrySecurityAccount->qrySecurityAccountField.BrokerID;
		szAccount.InvestorID = pReqQrySecurityAccount->qrySecurityAccountField.InvestorID;
		strcpy(szAccount.SecurityAccount, "0000000008");
		szAccount.ExchangeType = FTDC_ET_SZ;
		pRsp->securityAccountFields.push_back(szAccount);


	pWrapper->uplink(rspHeaders, pRsp);
}