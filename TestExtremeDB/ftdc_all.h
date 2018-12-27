#ifndef FTDC_ALL_H_
#define FTDC_ALL_H_
#include <genericdb.hpp>
#include <ftd/FTD30/Packages.h>

using namespace FTD;

Package* processUserLogin(const Package* pReq, mco_db_h db);
Package* processOrderInsert(const Package* pReq, mco_db_h db);
Package* processOrderAction(const Package* pReq, mco_db_h db);
Package* processFundTransfer(const Package* pReq, mco_db_h db);
Package* processQryOrder(const Package* pReq, mco_db_h db);
Package* processQryTrade(const Package* pReq, mco_db_h db);



#endif