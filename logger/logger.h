#pragma once


#define CALL_CONVENTION  __stdcall
#ifdef MALVA_EXPORT
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"{
#endif


	API_EXPORT void CALL_CONVENTION log_trace(const char* tag, const char* format, ...);
	API_EXPORT void CALL_CONVENTION  log_debug(const char* tag, const char* format, ...);
	API_EXPORT void CALL_CONVENTION  log_info(const char* tag, const char* format, ...);
	API_EXPORT void CALL_CONVENTION  log_warn(const char* tag, const char* format, ...);
	API_EXPORT void CALL_CONVENTION  log_error(const char* tag, const char* format, ...);
	API_EXPORT void CALL_CONVENTION  log_fatal(const char* tag, const char* format, ...);

	API_EXPORT void CALL_CONVENTION  root_log_trace(const char* format, ...);
	API_EXPORT void CALL_CONVENTION  root_log_debug(const char* format, ...);
	API_EXPORT void CALL_CONVENTION  root_log_info(const char* format, ...);
	API_EXPORT void CALL_CONVENTION  root_log_warn(const char* format, ...);
	API_EXPORT void CALL_CONVENTION  root_log_error(const char* format, ...);
	API_EXPORT void CALL_CONVENTION  root_log_fatal(const char* format, ...);

#ifdef __cplusplus
}
#endif

