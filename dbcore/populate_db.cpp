#include "ftdc_all.h"
using namespace genericdb;

#define MOCK_INVESTOR_ID 99
#define MOCK_SH_SECU_ACCOUNT "A000000001"
#define MOCK_SZ_SECU_ACCOUNT "2000000008"

void populate_db_users(mco_db_h db);
void populate_db_fund(mco_db_h db);
void populate_db_position(mco_db_h db);
void populate_db_order(mco_db_h db);
void populate_db_trade(mco_db_h db);
void populate_db_secu_account(mco_db_h db);


void populate2_db(mco_db_h db)
{
	populate_db_users(db);
	populate_db_secu_account(db);
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

void populate_db_secu_account(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	SecurityAccount account;
	SecurityAccount account2;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	account.create(t);
	account2.create(t);
	account.broker_id = 8080;
	account.investor_id = 99;
	account.exchange_type = FTDC_ET_SH;
	account.security_account = MOCK_SH_SECU_ACCOUNT;

	account2.broker_id = 8080;
	account2.investor_id = 99;
	account2.exchange_type = FTDC_ET_SZ;
	account2.security_account = MOCK_SZ_SECU_ACCOUNT;
	rc = mco_trans_commit(t);
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
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int  j;

	Position position;
	Position position2;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK == rc)
	{
		//mco_disk_transaction_policy(db, MCO_COMMIT_BUFFERED);
		//MCO_COMMIT_NO_SYNC
		position.create(t);
		position.investor_id = 99;
		position.exchange_type = FTDC_ET_SH;
		position.security_account = MOCK_SH_SECU_ACCOUNT;
		position.instrument_code = "600000";
		position.instrument_name = "浦发银行";
		position.volume = 10000;
		position.volume_available = 10000;
		position.volume_frozen = 0;
		position.volume_pre = 10000;

		position2.create(t);
		position2.investor_id = 99;
		position2.exchange_type = FTDC_ET_SZ;
		position2.security_account = MOCK_SZ_SECU_ACCOUNT;
		position2.instrument_code = "000001";
		position2.instrument_name = "中国平安";
		position2.volume = 8000;
		position2.volume_available = 8000;
		position2.volume_frozen = 0;
		position2.volume_pre = 8000;

		rc = mco_trans_commit(t);
		if (MCO_S_OK == rc)
		{
		}
		else
		{
		}
	}

}

void populate_db_order(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int  j;

	Order order;
	Order order2;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK == rc)
	{
		order.create(t);
		order.investor_id = MOCK_INVESTOR_ID;
		order.security_account = MOCK_SH_SECU_ACCOUNT;
		order.instrument_code = "600000";
		order.exchange_type = FTDC_ET_SH;
		order.direction = FTDC_D_BUY;
		order.price_type = FTDC_OPT_HS_Limit;
		order.price = 17.60;
		order.volume_total_original = 1000;
		order.volume_traded = 500;
		order.amount_traded = 9000.90;
		order.volume_total = 500;
		order.volume_cancelled = 0;
		order.status = FTDC_OS_PART_TRADED;

		rc = mco_trans_commit(t);
		//order.order_sys_sno = 1;
	}
}


void populate_db_trade(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int  j;

	Trade trade;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK == rc)
	{
		trade.create(t);
		trade.investor_id = MOCK_INVESTOR_ID;
		trade.instrument_code = "600000";
		trade.exchange_type = FTDC_ET_SH;
		trade.price_type = FTDC_OPT_HS_Limit;
		trade.price = 17.60;
		trade.volume_traded = 500;
		trade.amount_traded = 9000.90;
		trade.status = FTDC_OS_PART_TRADED;

		rc = mco_trans_commit(t);
		//order.order_sys_sno = 1;
	}
}