#include "ftdc_all.h"

Package* processOrderInsert(const Package* pReq, mco_db_h db)
{
	RspOrderInsert* pRsp = new RspOrderInsert();
	pRsp->m_header.sequenceNO = pReq->m_header.sequenceNO;
	pRsp->m_header.sequenceSeries = pReq->m_header.sequenceSeries;
	pRsp->m_header.transactionId = pReq->m_header.transactionId;

	return pRsp;
}