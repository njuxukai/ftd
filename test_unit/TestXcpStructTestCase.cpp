#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#include "stdafx.h"
#else
#include "config.h"
#endif

#include <ftd/BaseType.h>
#include <UnitTest++.h>
#include <ftd/PortID.h>
#include <xcp/XcpFtdcUserApiStruct.h>
#include <ftd/FTD30/Fields.h>
using namespace FTD;

SUITE(AutoGen)
{

	TEST(XcpStructure)
	{
		CHECK(sizeof(CXcpFtdcOrderField) == sizeof(OrderField));
	}
}