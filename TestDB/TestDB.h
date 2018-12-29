#ifndef TEST_DB_H_
#define TEST_DB_H_

#include <ftd/FTD30/Packages.h>

using namespace FTD;
class McoDBWrapper;

//≤‚ ‘∫Ø ˝
void testUserLogin(McoDBWrapper*);

void testOrderInsert(McoDBWrapper*);

void testOrderAction(McoDBWrapper*);

void testQryOrder(McoDBWrapper*);

//¥Ú”°respnse
void printRspPackage(Package* pRsp);
void printRspUserLogin(RspUserLogin* pRsp);

#endif