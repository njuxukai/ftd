#ifndef FTDC_ALL_H_
#define FTDC_ALL_H_

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_API_EXPORT
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif
#else
#define API_EXPORT 
#endif


#include "genericdb.hpp"
#include <ftd/FTD30/Packages.h>
#include <ftd/Errors.h>
using namespace FTD;

API_EXPORT Package* ftdcAll(const Package* pReq, mco_db_h db);

Package* processUserLogin(const Package* pReq, mco_db_h db);
Package* processUserLogout(const Package* pReq, mco_db_h db);
Package* processOrderInsert(const Package* pReq, mco_db_h db);
Package* processOrderAction(const Package* pReq, mco_db_h db);
Package* processFundTransfer(const Package* pReq, mco_db_h db);
Package* processQryOrder(const Package* pReq, mco_db_h db);
Package* processQryTrade(const Package* pReq, mco_db_h db);
Package* processQryPrivateInitialData(const Package* pReq, mco_db_h db);

API_EXPORT void populate_db(mco_db_h db);

#endif