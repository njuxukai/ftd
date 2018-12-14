#include "XcpFtdcTraderApiImpl.h"

CXcpFtdcTraderApi* CXcpFtdcTraderApi::CreateFtdcTraderApi(const char* wd)
{
	return new CXcpFtdcTraderApiImpl(wd);
}