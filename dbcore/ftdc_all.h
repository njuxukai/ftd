#ifndef FTDC_ALL_H_
#define FTDC_ALL_H_


#include "DBWrapper.h"
#include "genericdb.hpp"
#include <ftd/FTD30/Packages.h>
#include <mqclient/MQClientWrapper.h>
#include <ftd/Errors.h>
#ifdef _DEBUG
#include <iostream>
#endif



using namespace FTD;

void ftdcAll(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db);

void processUserLogin(const PlainHeaders& headers, FTD::PackageSPtr pReq, DBWrapper* pWrapper, mco_db_h db);
//void processUserLogout(PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processOrderInsert(const PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processOrderAction(const PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processFundTransfer(const PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processQryFund(const PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processQryPosition(const PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processQryOrder(const PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processQryTrade(const PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processQryPrivateInitialData(const PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);
void processQrySecurityAccount(const PlainHeaders& headers, FTD::PackageSPtr, DBWrapper* pWrapper, mco_db_h db);

void populate_db(mco_db_h db);

void populate2_db(mco_db_h db);
#endif