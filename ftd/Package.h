#ifndef FTD_PACKAGE_H
#define FTD_PACKAGE_H

#include <vector>
#include <string>

#include "ftd.h"
#include "IO.h"
#include "FtdMessageUtil.h"

namespace FTD
{
	struct Package
	{
		const uint8_t m_version;
		const uint32_t m_transactionId;
		const uint32_t m_mode;
		uint16_t m_sequenceSeries;
	    uint32_t m_sequenceNO;
		Package(uint8_t version_, uint32_t transactionId_, uint32_t mode_) :
			m_version(version_), m_transactionId(transactionId_), m_mode(mode_)
		{
			m_sequenceSeries = 0;
			m_sequenceNO = 0;
		}
		virtual ~Package() {};
		virtual Package* clone() const = 0;
		virtual void clear() = 0;

		void toFtdMesssages(std::vector<std::string>& ftdMsgs, const FtdExt* pExt = 0)const
		{
			ftdMsgs.clear();
			for (unsigned int i = 0; i < m_ftdcMessages.size(); i++)
			{
				ftdMsgs.push_back(FtdMessageUtil::formatFtdMessage(m_ftdcMessages[i], pExt));
			}
		}

		void toFtdcMessages(std::vector<std::string>& ftdcMsgs)const
		{
			ftdcMsgs.clear();
			for (unsigned int i = 0; i < m_ftdcMessages.size(); i++)
			{
				ftdcMsgs.push_back(m_ftdcMessages[i]);
			}
		}

		void toSingleConcatFtdcMessage(std::string& result, int& count)const
		{
			count = m_ftdcMessages.size();
			if (count == 1)
				result = m_ftdcMessages[0];
			else
				FtdMessageUtil::concateFtdcMessages(m_ftdcMessages, result);
		}

		void formatFtdcMessages()
		{
			toMessages(m_ftdcMessages);
		}

		virtual void toMessages(std::vector<std::string>& ftdcMsgs) = 0;




		bool mergeFtdcMessage(const std::string& ftdcMsg)
		{
			FtdcHeader header;
			const char* ftdcBegin = ftdcMsg.c_str() + readFtdcHeader(ftdcMsg.c_str(), header);
			std::string ftdcContent(ftdcBegin, header.contentLength);
			bool mergeResult = false;
			bool packageFinished = false;
			mergeFtdcMessage(header, ftdcContent, mergeResult, packageFinished);
			if (mergeResult)
			{
				m_ftdcMessages.push_back(ftdcMsg);
			}
			return packageFinished;
		}

		bool isPrivteMode()const
		{
			return (m_mode & FTD_MODE_PRIVATE) == FTD_MODE_PRIVATE;
		}

		bool isBoardcastMode()const
		{
			return (m_mode & FTD_MODE_BROADCAST) == FTD_MODE_BROADCAST;
		}

		bool isDialogMode()const
		{
			return (m_mode & FTD_MODE_DIALOG) == FTD_MODE_DIALOG;
		}

		bool isNoneMode()const
		{
			return (m_mode & FTD_MODE_NONE) == FTD_MODE_NONE;
		}

		bool isRequest() const
		{
			return (m_mode & FTD_REQUEST) == FTD_REQUEST;
		}

		bool isResponse() const
		{
			return (m_mode & FTD_RESPONSE) == FTD_RESPONSE;
		}


	protected:
		virtual bool mergeFieldMessage(const FtdcFieldHeader& header, const char* msg) = 0;
		std::vector<std::string> m_ftdcMessages;
	private:

		Package(const Package&) = delete;
		Package& operator=(const Package&) = delete;

		void mergeFtdcMessage(const FtdcHeader& ftdcHeader, const std::string& ftdcContent, bool& mergeSucceed, bool& packageFinished)
		{
			mergeSucceed = false;
			packageFinished = false;
			if (ftdcHeader.contentLength != ftdcContent.size())
				return;
			if (m_transactionId != ftdcHeader.transactionId)
			{
				return;
			}
			if (ftdcHeader.chain == FTDCChainSingle || ftdcHeader.chain == FTDCChainFirst)
			{
				clear();
				m_ftdcMessages.clear();
				m_sequenceNO = ftdcHeader.sequenceNO;
				m_sequenceSeries = ftdcHeader.sequenceSeries;
				//m_header.version = ftdcHeader.version;
			}
			if (m_sequenceNO != ftdcHeader.sequenceNO
				|| m_sequenceSeries != ftdcHeader.sequenceSeries)
				return;
			FtdcFieldHeader fieldHeader = { 0 };
			const char* buffer = ftdcContent.c_str();
			const char* pos = buffer;
			for (int i = 0; i < ftdcHeader.fieldCount; i++)
			{
				pos += readFtdcFieldHeader(pos, fieldHeader);
				int readLen = 0;
				mergeFieldMessage(fieldHeader, pos);
				pos += fieldHeader.fidLength;
			}
			if (pos - buffer != ftdcHeader.contentLength)
				return;
			mergeSucceed = true;
			if (ftdcHeader.chain == FTDCChainSingle || ftdcHeader.chain == FTDCChainLast)
			{
				packageFinished = true;
			}
		}
	};
}


#endif