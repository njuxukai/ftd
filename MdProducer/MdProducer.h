#pragma once
#include <functional>
#include <ftd/FTD30/Packages.h>

class MdProducerSpi
{
public:
	MdProducerSpi() {}
	virtual ~MdProducerSpi() {}
	virtual void OnNewMd(FTD::Package* pPack) {}
};

class MdProducerApi
{
public:
	MdProducerApi* CreateApi(const char* fname);
	virtual ~MdProducerApi() {}
	virtual void RegisterSpi(MdProducerSpi* spi);
	virtual void Init();
	virtual void Release();
protected:
	MdProducerApi();
};