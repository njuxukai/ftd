#ifndef FTD_IO_H
#define FTD_IO_H

#include "Utility.h"
#include "ftd.h"

static int readUInt32(const char* buffer, uint32_t& result)
{
	memcpy(&result, buffer, 4);
	result = ntohl(result);
	return 4;
}

static int readInt32(const char* buffer, int32_t& result)
{
	memcpy(&result, buffer, 4);
	result = ntohl(result);
	return 4;
}

static int readUInt16(const char* buffer, uint16_t & result)
{
	memcpy(&result, buffer, 2);
	result = ntohs(result);
	return 2;
}

static int readInt16(const char* buffer, int16_t & result)
{
	memcpy(&result, buffer, 2);
	result = ntohs(result);
	return 2;
}

static int readChar(const char* buffer, char& result)
{
	memcpy(&result, buffer, 1);
	return 1;
}

static int readUInt8(const char* buffer, uint8_t& result)
{
	memcpy(&result, buffer, 1);
	return 1;

}


static int writeUInt32(const uint32_t& result, char* buffer)
{
	uint32_t nvalue = htonl(result);
	memcpy(buffer, &nvalue, 4);
	return 4;
}

static int writeInt32(const int32_t& result, char* buffer)
{
	int32_t nvalue = htonl(result);
	memcpy(buffer, &nvalue, 4);
	return 4;
}

static int writeUInt16(const uint16_t & result, char* buffer)
{
	uint16_t nvalue = htons(result);
	memcpy(buffer, &nvalue, 2);
	return 2;
}

static int writeInt16(const int16_t & result, char* buffer)
{
	int16_t nvalue = htons(result);
	memcpy(buffer, &nvalue, 2);
	return 2;
}

static int writeChar(const char& result, char* buffer)
{
	memcpy(buffer, &result, 1);
	return 1;
}

static int writeUInt8(const uint8_t& result, char* buffer)
{
	memcpy(buffer, &result, 1);
	return 1;

}

static int readFtdHeader(const char* buffer, FtdHeader& header)
{
	int readLen = 0;

	const char* pos = buffer;
	readLen += readUInt8(pos, header.FTDType);

	pos = buffer + readLen;	
	readLen += readUInt8(pos, header.FTDExtHeaderLength);

	pos = buffer + readLen;	
	readLen += readInt16(pos, header.FTDCLength);
	
	return readLen;
}

static int writeFtdHeader(const FtdHeader& header, char* buffer)
{
	int writeLen = 0;
	writeUInt8(header.FTDType, buffer + writeLen);
	writeLen += 1;
	writeUInt8(header.FTDExtHeaderLength, buffer + writeLen);
	writeLen += 1;
	writeInt16(header.FTDCLength, buffer + writeLen);
	writeLen += 2;
	return writeLen;
}

static int readFtdExt(const char* buffer, int extLen,
	FtdExt& ext)
{
	int readLen = 0;
	uint8_t extEntryType;
	uint8_t extEntryDataLen;
	while (readLen < extLen)
	{
		if ((readLen + 2) > extLen)
			break;
		readUInt8(buffer + readLen, extEntryType);
		readLen += 1;
		readUInt8(buffer + readLen, extEntryDataLen);
		readLen += 1;
		if ((readLen + extEntryDataLen) > extLen)
			break;
		if (extEntryType == FTDTagNone || extEntryType == FTDTagKeepAlive)
		{
		}
		if (extEntryType == FTDTagDatetime && extEntryDataLen > 0)
		{
			readInt32(buffer + readLen, ext.dateTime);
		}
		if (extEntryType == FTDTagComressMethod && extEntryDataLen > 0 )
		{
			readChar(buffer + readLen, ext.compressMethod);
		}
		if (extEntryType == FTDTagSessionState && extEntryDataLen > 0 )
		{
			readChar(buffer + readLen, ext.sessionState);
		}
		if (extEntryType == FTDTagTradedate && extEntryDataLen > 0 )
		{
			readInt32(buffer + readLen, ext.tradeDate);

		}
		if (extEntryType == FTDTagTarget && extEntryDataLen > 0)
		{			
			memcpy(ext.target, buffer + readLen, 2);
			ext.target[2] = '\0';
		}
		readLen += extEntryDataLen;
	}
	return readLen;
}

static int writeFtdExt(const FtdExt& ext, char* buffer)
{
	int writeLen = 0;
	if (ext.dateTime > 0)
	{
		writeUInt8(FTDTagDatetime, buffer + writeLen);
		writeLen += 1;
		writeUInt8(FTDTagDatetimeLength, buffer + writeLen);
		writeLen += 1;
		writeInt32(ext.dateTime, buffer + writeLen);
		writeLen += 4;
	}
	if (ext.tradeDate > 0)
	{
		writeUInt8(FTDTagTradedate, buffer + writeLen);
		writeLen += 1;
		writeUInt8(FTDTagTradedateLength, buffer + writeLen);
		writeLen += 1;
		writeInt32(ext.tradeDate, buffer + writeLen);
		writeLen += 4;
	}
	if (ext.sessionState != 0)
	{
		writeUInt8(FTDTagSessionState, buffer + writeLen);
		writeLen += 1;
		writeUInt8(FTDTagSessionStateLength, buffer + writeLen);
		writeLen += 1;
		writeChar(ext.sessionState, buffer + writeLen);
		writeLen += 1;
	}
	if (ext.compressMethod != 0)
	{
		writeUInt8(FTDTagComressMethod, buffer + writeLen);
		writeLen += 1;
		writeUInt8(FTDTagComressMethodLength, buffer + writeLen);
		writeLen += 1;
		writeChar(ext.compressMethod, buffer + writeLen);
		writeLen += 1;
	}
	if (strlen(ext.target) > 0)
	{
		writeUInt8(FTDTagTarget, buffer + writeLen);
		writeLen += 1;
		writeUInt8(FTDTagTargetLength , buffer + writeLen);
		writeLen += 1;
		memcpy(buffer + writeLen, ext.target, 2);
		writeLen += 2;
	}
	return writeLen;
}

static int readFtdcHeader(const char* buffer, FtdcHeader& header)
{
	int readLen = 0;
	const char* pos = buffer;
	readLen += readUInt8(pos, header.version);

	pos = buffer + readLen;
	readLen += readUInt32(pos, header.transactionId);

	pos = buffer + readLen;
	readLen += readUInt8(pos, header.chain);

	pos = buffer + readLen;
	readLen += readUInt16(pos, header.sequenceSeries);

	pos = buffer + readLen;
	readLen += readUInt32(pos, header.sequenceNO);

	pos = buffer + readLen;
	readLen += readUInt16(pos, header.fieldCount);

	pos = buffer + readLen;
	readLen += readUInt16(pos, header.contentLength);

	return readLen;
}


static int writeFtdcHeader(const FtdcHeader& header, char* buffer)
{
	int writeLen = 0;

	char* pos = buffer;
	writeLen += writeUInt8(header.version, pos);

	pos = buffer + writeLen;
	writeLen += writeUInt32(header.transactionId, pos);

	pos = buffer + writeLen;
	writeLen += writeUInt8(header.chain, pos);

	pos = buffer + writeLen;
	writeLen += writeUInt16(header.sequenceSeries, pos);

	pos = buffer + writeLen;
	writeLen += writeUInt32(header.sequenceNO, pos);

	pos = buffer + writeLen;
	writeLen += writeUInt16(header.fieldCount, pos);

	pos = buffer + writeLen;
	writeLen += writeUInt16(header.contentLength, pos);

	return writeLen;
}


static int readFtdcFieldHeader(const char* buffer, FtdcFieldHeader& header)
{
	int readLen = 0;
	const char* pos = buffer;
	readLen += readInt32(pos, header.fid);

	pos = buffer + readLen;
	readLen += readUInt16(pos, header.fidLength);
	return readLen;
}


static int writeFtdcFieldHeader(const FtdcFieldHeader& header, char* buffer)
{
	int writeLen = 0;
	char* pos = buffer;
	writeLen += writeInt32(header.fid, pos);

	pos = buffer + writeLen;
	writeLen += writeUInt16(header.fidLength, pos);
	return writeLen;
}

#endif