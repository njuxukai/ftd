#ifndef FTD20_REQQRYDEPOSITFIELD_H
#define FTD20_REQQRYDEPOSITFIELD_H

#include "FTDProperties.h"
#include "FTDFields.h"

using namespace FTD;

namespace FTD20 {

	struct ReqQryDepositField
	{
            TTPropertyParticipantId participantId;
	};

	class ReqQryDepositFieldHelper
	{
	public:
		static void writeBuffer(const ReqQryDepositField& field, char* buffer, int& writenLen)
		{
			int data_length = 0;
            TPropertyParticipantId::writeBuffer(field.participantId, buffer);
            buffer += TPropertyParticipantId::getMsgLength();
            data_length += TPropertyParticipantId::getMsgLength();
			writenLen = data_length;
		}

		static void readBuffer(const char* buffer, ReqQryDepositField& field, int& readLen)
		{
			int data_length = 0;
            TPropertyParticipantId::readBuffer(buffer, field.participantId);
            buffer += TPropertyParticipantId::getMsgLength();
            data_length += TPropertyParticipantId::getMsgLength();
			readLen = data_length;
		}
	};
}
#endif