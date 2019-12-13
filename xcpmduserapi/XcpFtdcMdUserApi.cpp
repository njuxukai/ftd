#include "XcpFtdcMdUserApiImpl.h"
#include "XcpFtdcMdUserApi.h"


CXcpFtdcMdUserApi* CXcpFtdcMdUserApi::CreateFtdcMdUserApi(const char* wd)
{
	return new CXcpFtdcMdUserApiImpl(wd);
}