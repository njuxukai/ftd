#pragma once
#ifndef FTD_MESSAGE_UTIL_H
#define FTD_MESSAGR_UTIL_H


#include "ftd.h"
#include "io.h" 
#include <string>
#include <sstream>
#include <vector>
class FtdMessageUtil
{
public:
	
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
};

#endif