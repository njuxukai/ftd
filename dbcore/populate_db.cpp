#include "ftdc_all.h"
using namespace genericdb;

void populate_db_users(mco_db_h db);
void populate_db_fund(mco_db_h db);
void populate_db_position(mco_db_h db);
void populate_db_order(mco_db_h db);
void populate_db_trade(mco_db_h db);

void populate2_db(mco_db_h db)
{
	populate_db_users(db);
	populate_db_fund(db);
	populate_db_position(db);
	populate_db_order(db);
	populate_db_trade(db);
}

void populate_db_users(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int  j;
	for (j = 90; j < 100 && MCO_S_OK == rc; j++)
	{
		User user;
		rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
		if (MCO_S_OK == rc)
		{
			//mco_disk_transaction_policy(db, MCO_COMMIT_BUFFERED);
			//MCO_COMMIT_NO_SYNC
			user.create(t);
			user.broker_id = 8080;
			user.user_id = j + 1;
			user.password = "test";
			//printf("[%d]To Insert[%d]\n", j, (uint4)user.user_id);
			rc = mco_trans_commit(t);
			if (MCO_S_OK == rc)
			{
				printf("User[%d]Insert Success\n", j);
			}
			else
			{
				//printf("[%d]Insert failure\n", j );
			}
		}
	}
}


void populate_db_fund(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int  j;
	for (j = 0; j < 100 && MCO_S_OK == rc; j++)
	{
		Fund fund;
		rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
		if (MCO_S_OK == rc)
		{
			//mco_disk_transaction_policy(db, MCO_COMMIT_BUFFERED);
			//MCO_COMMIT_NO_SYNC
			fund.create(t);
			fund.branch_id = 8080;
			fund.investor_id = 99;
			fund.currency_type = FTDC_CT_RMB;
			fund.amount_drawable = 10000000;
			fund.amount_available = 10000000;
			fund.amount_frozen = 0;
			printf("Fund[%d]To Insert[%d]\n", j, (uint4)fund.investor_id);
			rc = mco_trans_commit(t);
			if (MCO_S_OK == rc)
			{
				//printf("[%d]Insert Success\n", j);
			}
			else
			{
				//printf("[%d]Insert failure\n", j );
			}
		}
	}
}

void populate_db_position(mco_db_h db)
{}

void populate_db_order(mco_db_h db)
{}


void populate_db_trade(mco_db_h db)
{}