#include "ftdc_all.h"

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

	for (int i = 0; i < 100; i++)
	{
		CFtdcPositionField position = { 0 };
		sprintf(position.InstrumentCode, "%d", 600000 + i);
		sprintf(position.InstrumentName, "֤ȯ%d",  i);
		position.Volume = i * 100;
		position.VolumeAvailable = (i+1) * 100;
		position.VolumeFrozen = (i+1) * 10;
		position.VolumePre = (i+1) * 100;
		pRsp->positionFields.push_back(position);
	}
	
	pWrapper->uplink(rspHeaders, pRsp);
}