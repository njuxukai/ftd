#ifndef FTDC_ALL_H_
#define FTDC_ALL_H_


#include "DBWrapper.h"
#include "genericdb.hpp"
#include <ftd/FTD30/Packages.h>
#include <mqclient/MQClientWrapper.h>
#include <ftd/Errors.h>
using namespace FTD;

void ftdcAll(PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db);

void processUserLogin(PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db);
//void processUserLogout(PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processOrderInsert(PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processOrderAction(PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processFundTransfer(PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processQryOrder(PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processQryTrade(PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processQryPrivateInitialData(PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);

void populate_db(mco_db_h db);

#endif