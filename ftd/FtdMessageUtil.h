#pragma once
#ifndef FTD_MESSAGE_UTIL_H
#define FTD_MESSAGR_UTIL_H


#include "ftd.h"
#include "io.h" 
#include <string>
#include <sstream>
#include <vector>
#include <boost/format.hpp>

class FtdMessageUtil
{
public:
	//连接 未压缩的ftdcMesg
	static bool concatenateFtdcMessages(const std::vector<std::string>& msgs, std::string& result)
	{
		
		if (msgs.size() == 0)
		{
			result.clear();
			return true;
		}
			
		if (msgs.size() == 1)
		{
			result = msgs[0];
			return true;
		}
		std::ostringstream oss;
		for (auto it = msgs.begin(); it != msgs.end(); it++)
		{
			oss << *it;
		}
		result = oss.str();
		
		return true;
	}

	//分割 未压缩的ftdcMesg
	static bool splitFtdcMessages(const std::string& catedFtdcMsgsString, std::vector<std::string>& msgs)
	{
		msgs.clear();
		bool result = true;
		FtdcHeader ftdcHeader = { 0 };
		int totalLen = catedFtdcMsgsString.size();
		int readLen = 0;
		while (1 == 1)
		{
			if (readLen + FTDC_HEADER_LENGTH > totalLen)
				break;
			readFtdcHeader(catedFtdcMsgsString.c_str() + readLen, ftdcHeader);
			if (readLen + FTDC_HEADER_LENGTH  + ftdcHeader.contentLength > totalLen)
				break;
			std::string s;
			msgs.push_back(catedFtdcMsgsString.substr(readLen, FTDC_HEADER_LENGTH + ftdcHeader.contentLength));
			readLen += (FTDC_HEADER_LENGTH + ftdcHeader.contentLength);
		}
		if (readLen == totalLen)
			result = true;
		return result;
	}
	
	static std::string unCompressFtdcMessage(const std::string& compressedFtdcMsg, int algoType)
	{
		return compressedFtdcMsg;
	}

	static std::string compressFtdcMessage(const std::string& rawFtdcMsg, int algoType)
	{
		return rawFtdcMsg;
	}

	//为ftdc消息加上ftd ftd_ext头（可能还要压缩）
	static std::string formatFtdMessage(const std::string& rawFtdcMsg, const FtdExt* pExt = 0)
	{
		FtdHeader ftdHeader = { 0 };
		if (pExt && pExt->compressMethod != FTD_COMPRESS_METHOD_NONE)
		{
			ftdHeader.FTDType = FTDTypeCompressed;
		}
		else
		{
			ftdHeader.FTDType = FTDTypeFTDC;
		}
		int extLen = 0;
		int ftdcLen = 0;
		std::string extMsg = "";
		if (pExt)
		{
			char extBuffer[MAX_FTD_EXT_MSG_LENGTH + 1];
			int extLen = writeFtdExt(*pExt, extBuffer);
			extMsg.assign(extBuffer, extBuffer + extLen);
		}
		ftdHeader.FTDExtHeaderLength = (uint8_t)extMsg.size();
		std::vector<std::string> ftdcMsgs;
		char ftdBuffer[MAX_FTD_LENGTH + 1];
		int totalLen = 0;
		memset(ftdBuffer, 0, MAX_FTD_LENGTH + 1);

		//1 先写ext扩充头
		if (extMsg.size() > 0)
			memcpy(ftdBuffer + FTD_HEADER_LENGTH, extMsg.c_str(), extMsg.size());
		//2 写ftdc
		if (pExt && pExt->compressMethod != FTD_COMPRESS_METHOD_NONE)
		{
			//需要压缩
			std::string compressedFtdcMsg = FtdMessageUtil::compressFtdcMessage(rawFtdcMsg, pExt->compressMethod);
			ftdHeader.FTDCLength = (int16_t)compressedFtdcMsg.size();
			memcpy(ftdBuffer + FTD_HEADER_LENGTH + extMsg.size(), compressedFtdcMsg.c_str(), compressedFtdcMsg.size());
			writeFtdHeader(ftdHeader, ftdBuffer);
			totalLen = FTD_HEADER_LENGTH + extMsg.size() + compressedFtdcMsg.size();
		}
		else
		{
			//不需要压缩
			ftdHeader.FTDCLength = (int16_t)rawFtdcMsg.size();
			memcpy(ftdBuffer + FTD_HEADER_LENGTH + extMsg.size(), rawFtdcMsg.c_str(), rawFtdcMsg.size());
			writeFtdHeader(ftdHeader, ftdBuffer);
			totalLen = FTD_HEADER_LENGTH + extMsg.size() + rawFtdcMsg.size();
		}
		//写head
		writeFtdHeader(ftdHeader, ftdBuffer);
		return std::string(ftdBuffer, totalLen);
	}

	//从ftd消息中解析ftdc消息（可能还要解压）
	static std::string parseFtdMessage(const std::string& ftdMsg)
	{
		FtdHeader ftdHeader = { 0 };
		FtdExt ftdExt = { 0 };
		bool needUncompress = false;
		readFtdHeader(ftdMsg.c_str(), ftdHeader);
		if (ftdHeader.FTDCLength == 0)
			return "";
		if (ftdHeader.FTDType == FTDTypeCompressed)
			needUncompress = true;
		if (ftdHeader.FTDExtHeaderLength > 0)
		{
			readFtdExt(ftdMsg.c_str() + FTD_HEADER_LENGTH, ftdHeader.FTDExtHeaderLength, ftdExt);
		}
		std::string  rawFtdcMsg = std::string().assign(ftdMsg.c_str() + FTD_HEADER_LENGTH + ftdHeader.FTDExtHeaderLength, 
			ftdHeader.FTDCLength);
		if (needUncompress)
		{
			return unCompressFtdcMessage(rawFtdcMsg, ftdExt.compressMethod);
		}
		else
		{
			return rawFtdcMsg;
		}
	}

	static std::string generateHeartbeatMessage()
	{
		FtdHeader header = { 0 };
		header.FTDType = FTDTypeNone;
		char buffer[FTD_HEADER_LENGTH + 1];
		writeFtdHeader(header, buffer);
	}

	static std::string getFtdBriefInfo(const std::string& ftdMsg)
	{
		FtdHeader header = { 0 };
		readFtdHeader(ftdMsg.c_str(), header);
		std::ostringstream oss;
		oss << boost::format("FtdHeader[%d][%d][%d]---") % (int)header.FTDType % (int)header.FTDExtHeaderLength % header.FTDCLength;
		if (header.FTDType == FTDTypeFTDC)
		{
			oss << getFtdcBriefInfo(ftdMsg.substr(FTD_HEADER_LENGTH + header.FTDExtHeaderLength, header.FTDCLength));
		}
		if (header.FTDType == FTDTypeCompressed)
		{
			oss << "Compressed,Ignore detail";
		}
		if (header.FTDType == FTDTypeNone)
		{
			oss << "Heartbeat";
		}
		return oss.str();
	}

	static std::string getFtdcBriefInfo(const std::string& ftdcMsg)
	{
		FtdcHeader header = { 0 };
		readFtdcHeader(ftdcMsg.c_str(), header);
		std::ostringstream oss;
		oss << boost::format("[Tid=%d][sno=%d(%c)series=%d][Fields(count=%d,length=%d)][ver=%d]") % header.transactionId % header.sequenceSeries % header.chain % header.sequenceNO
			 % header.fieldCount % header.contentLength % (int)header.version;
		return oss.str();
	}
};

#endif