#ifndef DB_WRAPPER_H_
#define DB_WRAPPER_H_

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_API_EXPORT
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif
#else
#define API_EXPORT 
#endif

#include <functional>
#include <mco.hpp>







class McoDBWrapperImpl;

typedef std::function<void(mco_db_h)> DBTask;

class API_EXPORT McoDBWrapper
{
public:
	McoDBWrapper();
	~McoDBWrapper();
	void submit(const DBTask& f);
private:
	McoDBWrapperImpl *m_pImpl;
};



#endif