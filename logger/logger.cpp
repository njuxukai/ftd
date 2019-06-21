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
};



void CALL_CONVENTION log_trace(const char* tag, const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_trace(tag, buffer);
}

void CALL_CONVENTION  log_debug(const char* tag, const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_debug(tag, buffer);
}

void CALL_CONVENTION  log_info(const char* tag, const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_info(tag, buffer);
}

void CALL_CONVENTION  log_warn(const char* tag, const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_warn(tag, buffer);
}

void CALL_CONVENTION  log_error(const char* tag, const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_error(tag, buffer);
}

void CALL_CONVENTION  log_fatal(const char* tag, const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_fatal(tag, buffer);
}

void CALL_CONVENTION  root_log_trace(const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_trace(buffer);
}

void CALL_CONVENTION  root_log_debug(const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_debug(buffer);
}

void CALL_CONVENTION  root_log_info(const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_info(buffer);
}

void CALL_CONVENTION  root_log_warn(const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_warn(buffer);
}

void CALL_CONVENTION  root_log_error(const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_error(buffer);
}

void CALL_CONVENTION  root_log_fatal(const char* format, ...)
{
	char buffer[1000];
	va_list pArgs;
	va_start(pArgs, format);
	vsprintf(buffer, format, pArgs);
	va_end(pArgs);
	MalvaLogger::get_instance()->log_fatal(buffer);
}


std::mutex MalvaLogger::s_mutex;

std::shared_ptr<MalvaLogger> MalvaLogger::s_instance;

MalvaLogger::MalvaLogger()
{
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