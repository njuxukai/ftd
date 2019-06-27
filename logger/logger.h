#pragma once

 
#define CALL_CONVENTION  __stdcall
#ifdef MALVA_EXPORT
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

#define LOG_TRACE 0
#define LOG_DEBUG 1
#define LOG_INFO 2
#define LOG_WARN 3
#define LOG_ERROR 4
#define LOG_FATAL 5

#ifdef __cplusplus
extern "C"{
#endif
	API_EXPORT void CALL_CONVENTION set_log_level(int log_level);
	API_EXPORT void CALL_CONVENTION tag_log(int log_level, const char* tag, const char* format, ...);
	API_EXPORT void CALL_CONVENTION  root_log(int log_level, const char* format, ...);


#ifdef __cplusplus
}
#endif

