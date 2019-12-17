#pragma once
#include <functional>
#include <ftd/FTD30/Packages.h>

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_API_EXPORT
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif
#else
#define API_EXPORT 
#endif

class MdProducerSpi
{
public:
	MdProducerSpi() {}
	virtual ~MdProducerSpi() {}
	virtual void OnNewMd(FTD::Package* pPack) {}
};

class API_EXPORT MdProducerApi
{
public:
	static MdProducerApi* CreateApi(const char* fname);
	virtual ~MdProducerApi() {}
	virtual void RegisterSpi(MdProducerSpi* spi) = 0;
	virtual void Init() = 0;
	virtual void Release() = 0;
protected:
	MdProducerApi() {}
};