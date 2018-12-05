#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#include "stdafx.h"
#else
#include "config.h"
#endif

#include <ftd/BaseType.h>
#include <UnitTest++.h>
#include <ftd/PortID.h>
using namespace FTD;

SUITE(FTDPortIDTest)
{

	TEST(Normal)
	{
		std::string frontAddress = "tcp:\\10.1.0.69:9000";
		PortID id;
		id.fromString(frontAddress);
		CHECK_EQUAL(id.toString(), frontAddress);
	}



}