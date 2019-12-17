#include "MdProducer.h"
#include "MdProducerMock.h"
#include <ftd/Settings.h>
#include <fstream>

std::string parseMdProducerTypeString(const char* fname)
{
	std::string typeString = "";
	using namespace FTD;
	bool parseResult = true;
	try
	{
		std::ifstream stream(fname);
		FTD::Settings settings(false);
		stream >> settings;
		Settings::Sections section;
		Dictionary dict;

		section = settings.get("PRODUCER");
		if (section.size() != 1)
			return "";
		Dictionary reporterDict = section[0];
		typeString = reporterDict.getString("Type");
	}
	catch (...)
	{
	}
	return typeString;
}


MdProducerApi* MdProducerApi::CreateApi(const char* fname)
{
	std::string typeString = parseMdProducerTypeString(fname);

	if (typeString == "MOCK")
		return new MdProducerMock(fname);
	return NULL;
}