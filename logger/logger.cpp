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
	void set_log_level(int level) { log_level = level; }
	int get_log_level()const { return log_level; }
	void log_trace(const char* tag, const char* content);
	void log_trace(const char* content);
	void log_debug(const char* tag, const char* content);
	void log_debug(const char* content);
	void log_warn(const char* tag, const char* content);
	void log_warn(const char* content);
	void log_error(const char* tag, const char* content);
	void log_error(const char* content);
	void log_fatal(const char* tag, const char* content);
	void log_fatal(const char* content);
	void log_info(const char* tag, const char* content);
	void log_info(const char* content);
private:
	log4cplus::Logger root_logger;
	static std::mutex s_mutex;
	static std::shared_ptr<MalvaLogger> s_instance;
	int log_level;
};

void CALL_CONVENTION set_log_level(int log_level)
{
	if (log_level >= LOG_TRACE && log_level <= LOG_FATAL)
		MalvaLogger::get_instance()->set_log_level(log_level);
}

void CALL_CONVENTION tag_log(int log_level, const char* tag, const char* format, ...)
{
	if (log_level < MalvaLogger::get_instance()->get_log_level())
		return;
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	switch (log_level)
	{
	case LOG_TRACE:
		MalvaLogger::get_instance()->log_trace(tag, buffer);
		break;
	case LOG_DEBUG:
		MalvaLogger::get_instance()->log_debug(tag, buffer);
		break;
	case LOG_INFO:
		MalvaLogger::get_instance()->log_info(tag, buffer);
		break;
	case LOG_WARN:
		MalvaLogger::get_instance()->log_warn(tag, buffer);
		break;
	case LOG_ERROR:
		MalvaLogger::get_instance()->log_error(tag, buffer);
		break;
	case LOG_FATAL:
		MalvaLogger::get_instance()->log_fatal(tag, buffer);
		break;
	default:
		break;
	}
}

void CALL_CONVENTION root_log(int log_level, const char* format, ...)
{
	if (log_level < MalvaLogger::get_instance()->get_log_level())
		return;
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	switch (log_level)
	{
	case LOG_TRACE:
		MalvaLogger::get_instance()->log_trace(buffer);
		break;
	case LOG_DEBUG:
		MalvaLogger::get_instance()->log_debug(buffer);
		break;
	case LOG_INFO:
		MalvaLogger::get_instance()->log_info(buffer);
		break;
	case LOG_WARN:
		MalvaLogger::get_instance()->log_warn(buffer);
		break;
	case LOG_ERROR:
		MalvaLogger::get_instance()->log_error(buffer);
		break;
	case LOG_FATAL:
		MalvaLogger::get_instance()->log_fatal(buffer);
		break;
	default:
		break;
	}
}


std::mutex MalvaLogger::s_mutex;

std::shared_ptr<MalvaLogger> MalvaLogger::s_instance;

MalvaLogger::MalvaLogger()
{
	log_level = LOG_TRACE;
	log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(PROPERTY_FILE));
	root_logger = log4cplus::Logger::getRoot();
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

void MalvaLogger::log_trace(const char* name, const char* content)
{
	LOG4CPLUS_TRACE(log4cplus::Logger::getInstance(name), content);	
}

void MalvaLogger::log_trace(const char* content)
{
	LOG4CPLUS_TRACE(root_logger, content);
}

void MalvaLogger::log_debug(const char* name, const char* content)
{
	LOG4CPLUS_DEBUG(log4cplus::Logger::getInstance(name), content);
}

void MalvaLogger::log_debug(const char* content)
{
	LOG4CPLUS_DEBUG(root_logger, content);
}


void MalvaLogger::log_info(const char* name, const char* content)
{
	LOG4CPLUS_INFO(log4cplus::Logger::getInstance(name), content);
}

void MalvaLogger::log_info(const char* content)
{
	LOG4CPLUS_INFO(root_logger, content);
}

void MalvaLogger::log_warn(const char* name, const char* content)
{
	LOG4CPLUS_WARN(log4cplus::Logger::getInstance(name), content);
}

void MalvaLogger::log_warn(const char* content)
{
	LOG4CPLUS_WARN(root_logger, content);
}

void MalvaLogger::log_error(const char* name, const char* content)
{
	LOG4CPLUS_ERROR(log4cplus::Logger::getInstance(name), content);
}

void MalvaLogger::log_error(const char* content)
{
	LOG4CPLUS_ERROR(root_logger, content);
}

void MalvaLogger::log_fatal(const char* name, const char* content)
{
	LOG4CPLUS_FATAL(log4cplus::Logger::getInstance(name), content);
}

void MalvaLogger::log_fatal(const char* content)
{
	LOG4CPLUS_FATAL(root_logger, content);
}