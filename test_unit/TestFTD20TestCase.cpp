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
using namespace FTD;

SUITE(FTDTest)
{
	TEST(OrderFieldTest)
	{
		OrderField field ;
		//memset(&field, 0, sizeof(OrderField));
		field.LimitPrice = 30.56;
		char* buffer = new char[4096];
		int write_len = 0;
		int read_len = 0;
		OrderField field2;
		//memset(&field2, 0, sizeof(OrderField));
		OrderFieldHelper::writeBuffer(field, buffer, write_len);
		OrderFieldHelper::readBuffer(buffer, field2, read_len);
		CHECK(write_len > 0);
		CHECK_EQUAL(write_len, read_len);
		CHECK_CLOSE(30.56, field2.LimitPrice, 0.01);
	}

	TEST(PackageTest)
	{
		RspQryOrder package ;
		OrderField field = { 0 };
		int count = 1;
		for (int i = 0; i < count; i++)
		{
			package.orderFields.push_back(field);
		}
		std::vector<std::string> mss;
		package.toMessages(mss);
		RspQryOrder package2;
		for (int i = 0; i < mss.size(); i++)
		{
			package2.mergeFtdcMessage(mss[i]);
		}
		
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