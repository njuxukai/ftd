#include "ftdc_all.h"
#include "Exceptions.h"

using namespace genericdb;

void processQryPositionTransaction(ReqQryPosition* pReq, mco_trans_h t, RspQryPosition* pRsp);

void processQryPosition(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db)
{
#ifdef _DEBUG
	std::cout << "[processQryPosition] called\n";
#endif
	PlainHeaders rspHeaders = { 0 };
	rspHeaders.admin_flag = QMSG_FLAG_APP;
	rspHeaders.msg_type = QMSG_TYPE_RSP;
	strncpy(rspHeaders.source_queue, headers.target_queue, 
		sizeof(rspHeaders.source_queue));
	rspHeaders.source_session = headers.source_session;

	std::shared_ptr<RspQryPosition> pRsp = 
		std::make_shared<RspQryPosition>();
	pRsp->m_sequenceSeries = pReq->m_sequenceSeries;
	pRsp->m_sequenceNO = pReq->m_sequenceNO;

	ReqQryPosition *pReqQryPosition = (ReqQryPosition*)pReq.get();
	pRsp->requestSourceField.RequestID = 
		pReqQryPosition->qryPositionField.RequestID;

	try
	{
		McoTrans t(db, MCO_READ_ONLY, MCO_TRANS_FOREGROUND);
		try
		{
			processQryPositionTransaction(pReqQryPosition, t, pRsp.get());
		}
		catch (...)
		{
			t.rollback();
			throw;
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
	
	pWrapper->uplink(rspHeaders, pRsp);
}

void processQryPositionTransaction(ReqQryPosition* pReq, mco_trans_h t, RspQryPosition* pRsp)
{
	int searchInvestorID = pReq->qryPositionField.InvestorID;

	mco_cursor_t csr;
	MCO_RET rc = Position::InvestorIdx::cursor(t, &csr);
	if (MCO_S_OK == rc)
	{
		for (rc = Position::InvestorIdx::search(t, &csr, MCO_EQ, searchInvestorID);
		rc == MCO_S_OK;
			rc = mco_cursor_next(t, &csr))
		{
			Position position;
			position.from_cursor(t, &csr);
			if (position.investor_id != searchInvestorID)
				break;
			CFtdcPositionField positionField = { 0 };
			positionField.InvestorID = position.investor_id;
			positionField.ExchangeType = position.exchange_type;
			strcpy(positionField.SecurityAccount, ((std::string) position.security_account).data());
			strcpy(positionField.InstrumentCode, ((std::string)position.instrument_code).data());
			strcpy(positionField.InstrumentName, ((std::string)position.instrument_name).data());
			positionField.Volume = position.volume;
			positionField.VolumeAvailable = position.volume_available;
			positionField.VolumeFrozen = position.volume_frozen;
			positionField.VolumePre = position.volume_pre;
			pRsp->positionFields.push_back(positionField);
		}
	}
}