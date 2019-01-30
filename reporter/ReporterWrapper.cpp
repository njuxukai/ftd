#include "ReporterWrapper.h"
#include "ReporterSZImpl.h"

#include <fstream>
#include <ftd/Settings.h>
std::string parseReporterTypeString(const std::string& fname)
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

		section = settings.get("REPORTER");
		if (section.size() != 1)
			return false;
		Dictionary reporterDict = section[0];
		typeString = reporterDict.getString("Type");
	}
	catch (...)
	{
	}
	return typeString;
}

ReporterWrapper::SPtr ReporterWrapper::CreateWrapper(const std::string& cfgFname)
{
	std::string typeString = parseReporterTypeString(cfgFname);

	if (typeString == "SZSTEP")
		return std::make_shared<ReporterSZSTEPImpl>(cfgFname);

	return 0;
}

