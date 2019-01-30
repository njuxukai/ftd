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
#include <memory>
#include <string>
#include <functional>
#include <ftd/FTD30/Packages.h>

typedef std::function<void(FTD::PackageSPtr)> ReporterUplinkCallback;

class API_EXPORT ReporterWrapper
{
public:
	typedef std::shared_ptr<ReporterWrapper> SPtr;

	static SPtr CreateWrapper(const std::string& cfgFname);
	virtual ~ReporterWrapper() {}

	virtual void submit(FTD::PackageSPtr pPackage) = 0;
	virtual void registerUplinkCallback(const ReporterUplinkCallback& function) = 0;
	virtual void uplink(FTD::PackageSPtr pPackage) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
protected:
	ReporterWrapper() {}
private:
	DISABLE_COPY_AND_ASSIGN(ReporterWrapper)
};

#endif