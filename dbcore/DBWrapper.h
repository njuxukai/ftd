#ifndef DB_WRAPPER_H_
#define DB_WRAPPER_H_

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_API_EXPORT
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif
#else
#define API_EXPORT 
#endif

#include <common/usual_macro.h>

#include <ftd/FTD30/Packages.h>
#include <mqclient/MQClientWrapper.h>
#include <mco.hpp>


typedef std::function<void(PlainHeaders&, FTD::PackageSPtr)> UplinkFunction;
typedef std::function<void(PlainHeaders&, FTD::PackageSPtr)> DownlinkFunction;

class API_EXPORT DBWrapper
{
public:
	typedef std::shared_ptr<DBWrapper> SPtr;

	SPtr CreateWrapper();
	virtual ~DBWrapper() {}
	virtual void submit(PlainHeaders& headers, FTD::PackageSPtr) = 0;
	virtual void registerUplinkFunction(const UplinkFunction& function) = 0 ;
	virtual void uplink(PlainHeaders& headers, FTD::PackageSPtr pPackage) = 0 ;
protected:
	DBWrapper() {}

private:
	DISABLE_COPY_AND_ASSIGN(DBWrapper)
};



#endif