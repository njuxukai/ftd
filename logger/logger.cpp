// MalvaLogger.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "logger.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/initializer.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/loglevel.h>

#define PROPERTY_FILE "log4cplus.properties"

class MalvaLogger
{
public:
	MalvaLogger();
	~MalvaLogger();
	static MalvaLogger* get_instance();
	void set_log_level(int level) { m_log_level = level; }
	int get_log_level()const { return m_log_level; }

	void tag_log(const char* name, int level, const char* fmt, ...);
	void root_log(int level, const char* fmt, ...);
private:
	void log(log4cplus::Logger& logger, int level, const char* fmt, ...);
	log4cplus::Logger m_root_logger;
	static std::mutex s_mutex;
	static std::shared_ptr<MalvaLogger> s_instance;
	int m_log_level;
};

void CALL_CONVENTION set_log_level(int log_level)
{
	if (log_level >= LOG_TRACE && log_level <= LOG_OFF)
		MalvaLogger::get_instance()->set_log_level(log_level);
}

void CALL_CONVENTION tag_log(const char* tag, int log_level, const char* format, ...)
{
	va_list pArgs;
	va_start(pArgs, format);
	MalvaLogger::get_instance()->tag_log(tag, log_level,  format, pArgs);
	va_end(pArgs);
}

void CALL_CONVENTION root_log(int log_level, const char* format, ...)
{
	va_list pArgs;
	va_start(pArgs, format);
	MalvaLogger::get_instance()->root_log(log_level, format, pArgs);
	va_end(pArgs);
}



std::mutex MalvaLogger::s_mutex;

std::shared_ptr<MalvaLogger> MalvaLogger::s_instance;

MalvaLogger::MalvaLogger()
{
	m_log_level = LOG_TRACE;
	log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(PROPERTY_FILE));
	m_root_logger = log4cplus::Logger::getRoot();
}

MalvaLogger::~MalvaLogger()
{}

MalvaLogger* MalvaLogger::get_instance()
{
	if (s_instance.get() == nullptr)
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		if (nullptr == s_instance.get())
		{
			s_instance = std::shared_ptr<MalvaLogger>(new MalvaLogger());
		}
	}
	return s_instance.get();
}

void MalvaLogger::log(log4cplus::Logger& logger, int level, const char* fmt, ...)
{

	va_list pArgs;
	va_start(pArgs, fmt);

	switch (level)
	{
	case LOG_TRACE:
		LOG4CPLUS_TRACE_FMT(logger, fmt, pArgs);
		break;
	case LOG_DEBUG:
		LOG4CPLUS_DEBUG_FMT(logger, fmt, pArgs);
		break;
	case LOG_INFO:
		LOG4CPLUS_INFO_FMT(logger, fmt, pArgs);
		break;
	case LOG_WARN:
		LOG4CPLUS_WARN_FMT(logger, fmt, pArgs);
		break;
	case LOG_ERROR:
		LOG4CPLUS_ERROR_FMT(logger, fmt, pArgs);
		break;
	case LOG_FATAL:
		LOG4CPLUS_FATAL_FMT(logger, fmt, pArgs);
		break;
	case LOG_OFF:
	default:
		break;
}

va_end(pArgs);
}

void MalvaLogger::tag_log(const char* name, int level, const char* fmt, ...)
{
	if (level < m_log_level)
		return;
	auto logger = log4cplus::Logger::getInstance(name);

	va_list pArgs;
	va_start(pArgs, fmt);
	log(logger, level, fmt, pArgs);
	va_end(pArgs);

}

void MalvaLogger::root_log(int level, const char* fmt, ...)
{
	if (level < m_log_level)
	    return;

	va_list pArgs;
	va_start(pArgs, fmt);
	log(m_root_logger, level, fmt, pArgs);
	va_end(pArgs);

}

