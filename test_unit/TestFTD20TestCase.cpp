#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#include "stdafx.h"
#else
#include "config.h"
#endif

#include <ftd/BaseType.h>
#include <UnitTest++.h>

#include <ftd/FTD30/Fields.h>
#include <ftd/FTD30/Packages.h>
#include <ftd/FTD30/Error.h>

#include <ftd/FtdMessageUtil.h>
using namespace FTD;

SUITE(FTDTest)
{
	TEST(FTDHeaderTest)
	{
		FtdHeader header = { 0 };
		FtdHeader header2 = { 0 };
		header.FTDCLength = 100;
		header.FTDExtHeaderLength = 126;
		header.FTDType = 0;
		char buffer[10];
		int writeLen = writeFtdHeader(header, buffer);
		int readLen = readFtdHeader(buffer, header2);
		CHECK_EQUAL(writeLen, readLen);
		CHECK_EQUAL(header.FTDCLength, header2.FTDCLength);
		CHECK_EQUAL(header.FTDExtHeaderLength, header2.FTDExtHeaderLength);
		CHECK_EQUAL(header.FTDType, header2.FTDType);
	}

	TEST(FTDExtHeaderTest)
	{
		FtdExt ext = { 0 };
		FtdExt ext2 = { 0 };
		ext.tradeDate = (1 << 17) + 1;
		ext.dateTime = 20003;
		ext.sessionState = 2;
		strcpy(ext.target, "aa");
		char buffer[128];
		int writeLen = writeFtdExt(ext, buffer);
		int readLen = readFtdExt(buffer, writeLen, ext2);
		CHECK_EQUAL(writeLen, readLen);
		CHECK_EQUAL((1 << 17) + 1, ext2.tradeDate);
		CHECK_EQUAL(ext.sessionState, ext2.sessionState);
		CHECK_EQUAL(strcmp(ext2.target, "aa") , 0);
	}
	
	TEST(SplitFtdcMessagesTest)
	{
		RspQryOrder package;
		CFtdcOrderField field = { 0 };
		int count = 10000;
		for (int i = 0; i < count; i++)
		{
			package.orderFields.push_back(field);
		}
		std::vector<std::string> mss;
		package.toMessages(mss);
		std::string catString;
		FtdMessageUtil::concateFtdcMessages(mss, catString);
		std::vector<std::string> mss2;
		bool splitResult = FtdMessageUtil::splitFtdcMessages(catString, mss2);
		RspQryOrder package2;
		for (auto it = mss2.begin(); it != mss2.end(); it++)
		{
			package2.mergeFtdcMessage(*it);
		}
		CHECK(splitResult);
		CHECK_EQUAL(package2.orderFields.size(), 10000);
	}
	

	TEST(OrderFieldTest)
	{
		CFtdcOrderField field ;
		//memset(&field, 0, sizeof(OrderField));
		field.LimitPrice = 30.56;
		char* buffer = new char[4096];
		int write_len = 0;
		int read_len = 0;
		CFtdcOrderField field2;
		//memset(&field2, 0, sizeof(OrderField));
		CFtdcOrderFieldHelper::writeBuffer(field, buffer, write_len);
		CFtdcOrderFieldHelper::readBuffer(buffer, field2, read_len);
		CHECK(write_len > 0);
		CHECK_EQUAL(write_len, read_len);
		CHECK_CLOSE(30.56, field2.LimitPrice, 0.01);
	}

	TEST(PackageTest)
	{
		RspQryOrder package ;
		CFtdcOrderField field = { 0 };
		field.TestTimestamp = 1 << 33;
		int count = 1;
		for (int i = 0; i < count; i++)
		{
			package.orderFields.push_back(field);
		}
		std::vector<std::string> mss;
		package.toMessages(mss);
		RspQryOrder package2;
		for (unsigned int i = 0; i < mss.size(); i++)
		{
			package2.mergeFtdcMessage(mss[i]);
		}
		CHECK_EQUAL(package2.orderFields[0].TestTimestamp, 1 << 32);
		CHECK_EQUAL(package2.orderFields.size(), package.orderFields.size());
	}
}
/*
SUITE(FTDTest)
{

	TEST(BulltinField)
	{
		FTD::BulletinField field = { 0 };
		char* buffer = new char[4096];
		int writeLen = 20;
		int readLen = 10;
		FTD::BulletinFieldHelper::writeBuffer(field, buffer, writeLen);
		FTD::BulletinFieldHelper::readBuffer(buffer, field, readLen);
		CHECK_EQUAL(writeLen, readLen);
		delete buffer;
	}

	TEST(ClientField)
	{
		FTD::ClientField field = { 0 };
		FTD::ClientField field2 = { 0 };
		strcpy(field.clientId, "12345678");
		char* buffer = new char[4096];
		int writeLen = 20;
		int readLen = 10;
		FTD::ClientFieldHelper::writeBuffer(field, buffer, writeLen);
		FTD::ClientFieldHelper::readBuffer(buffer, field2, readLen);
		CHECK_EQUAL(writeLen, readLen);
		CHECK_EQUAL("12345678", field2.clientId);
		delete buffer;
	}

	TEST(ClientField2)
	{
		FTD::ClientField field = { 0 };
		FTD::ClientField field2 = { 0 };
		strcpy(field.clientId, "1234");
		char* buffer = new char[4096];
		int writeLen = 20;
		int readLen = 10;
		FTD::ClientFieldHelper::writeBuffer(field, buffer, writeLen);
		FTD::ClientFieldHelper::readBuffer(buffer, field2, readLen);
		CHECK_EQUAL(writeLen, readLen);
		CHECK_EQUAL("1234", field2.clientId);
		delete buffer;
	}

	TEST(Error)
	{
		FTD::Error package;
		FTD::Error package2;
		FTD::ErrorField errorField;
		strcpy(errorField.errorCode, "ok");
		FTD::Field field;
		memcpy(&field.errorField, &errorField, sizeof(FTD::ErrorField));

		int len = 10000;
		//package.errorFields.reserve(len);
		for (int i = 0; i < len; i++)
		{
			package.mergeField(field, FID_ErrorField);
		}
		FTD::ErrorTargetOrderField errorTargetOrderField;
		strcpy(errorTargetOrderField.orderLocalId, "okokokokokokok");
		memcpy(&field.errorTargetOrderField, &errorTargetOrderField, sizeof(FTD::ErrorTargetOrderField));
		package.mergeField(field, FID_ErrorTargetOrderField);

		std::vector<std::string> msgs;
		package.toMessages(msgs);
		for (int i = 0; i < msgs.size(); i++)
		{
			;
			if (package2.mergeFtdcMessage(msgs[i]))
			{
				break;
			}
		}
		CHECK_EQUAL(len, package2.errorFields.size());
		//CHECK_EQUAL("ok", package2.errorFields[len-1].errorCode);
		//CHECK_EQUAL(nullptr, package2.errorTargetOrderFields.get());
		CHECK_EQUAL("okokokokokokok", package2.pErrorTargetOrderField->orderLocalId);
	}

	
}*/