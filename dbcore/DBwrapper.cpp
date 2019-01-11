#include "DBWrapper.h"
#include "DBWrapperMcoImpl.h"


DBWrapper::SPtr DBWrapper::CreateWrapper()
{
	return std::make_shared<DBWrapperMcoImpl>();
}