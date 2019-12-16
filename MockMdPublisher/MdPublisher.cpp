#include "MdPublisher.h"

MdPublisher::MdPublisher(const std::string& cfgName)
{
	m_cfgFname = cfgName;
	m_parseResult = parseCfgFile(cfgName);
	if (m_parseResult)
	{
		init();
	}
}


MdPublisher::~MdPublisher()
{}

MdPublisher::~MdPublisher()
{

}