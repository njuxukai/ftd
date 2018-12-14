#include "XcpFtdcTraderApiImpl.h"
#include "XcpFtdcTraderApi.h"
CXcpFtdcTraderApi* CXcpFtdcTraderApi::CreateFtdcTraderApi(const char* wd)
{
	return new CXcpFtdcTraderApiImpl(wd);
}