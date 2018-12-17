#pragma once
#include <ftd/FTD30/Packages.h>

using namespace FTD;
struct OrderKey
{
	int frontID;
	int sessionID;
	int orderRef;
};

class MockDB
{
	void ReqUerLogin(const ReqUserLogin& req, RspUserLogin& rsp)
	{
	}

	void ReqInputOrder(const ReqOrderInsert& req, RspOrderInsert& rsp)
	{
	}

	void ReqQryOrder(const ReqQryOrder& req, RspQryOrder& rsp)
	{
	}
};