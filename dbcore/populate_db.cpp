#include "ftdc_all.h"
using namespace genericdb;

#define MOCK_INVESTOR_ID 99
#define MOCK_SH_SECU_ACCOUNT "A000000001"
#define MOCK_SZ_SECU_ACCOUNT "2000000008"
#define MOCK_SH_PBU_ID   28888
#define MOCK_SZ_PBU_ID   1999
#define MOCK_BRANCH_ID 1672
#define MOCK_SH_BRANCH_ID 11672
#define MOCK_SZ_BRANCH_ID 1672

void populate_test(mco_db_h db);
void populate_db_sysconfig(mco_db_h db);
void populate_db_users(mco_db_h db);
void populate_db_fund(mco_db_h db);
void populate_db_position(mco_db_h db);
void populate_db_order(mco_db_h db);
void populate_db_inner_execution_report(mco_db_h db);
void populate_db_secu_account(mco_db_h db);


void populate2_db(mco_db_h db)
{
	//populate_test(db);
	populate_db_sysconfig(db);
	populate_db_users(db);
	populate_db_secu_account(db);
	populate_db_fund(db);
	populate_db_position(db);
	populate_db_order(db);
	populate_db_inner_execution_report(db);
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
	account.pbu_id = MOCK_SH_PBU_ID;

	account2.broker_id = 8080;
	account2.investor_id = 99;
	account2.exchange_type = FTDC_ET_SZ;
	account2.security_account = MOCK_SZ_SECU_ACCOUNT;
	account.pbu_id = MOCK_SZ_PBU_ID;
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
		order.volume_cum = 500;
		order.amount_cum = 9000.90;
		order.volume_leaves = 500;
		order.volume_cancelled = 0;
		order.status = FTDC_OS_PART_TRADED;
		order.front_id = 1;
		order.session_id = 1;
		order.order_ref = 1;
		order.order_sys_id = 1;
		order.client_order_id = "0000000001";
		order.order_exchange_id = "0000000000000001";
		order.pbu_id = MOCK_SH_PBU_ID;
		order.branch_id = MOCK_BRANCH_ID;
		order.exchange_branch_id = MOCK_SH_BRANCH_ID;

		order2.create(t);
		order2.investor_id = MOCK_INVESTOR_ID;
		order2.security_account = MOCK_SH_SECU_ACCOUNT;
		order2.instrument_code = "600000";
		order2.exchange_type = FTDC_ET_SH;
		order2.direction = FTDC_D_BUY;
		order2.price_type = FTDC_OPT_HS_Limit;
		order2.price = 17.60;
		order2.volume_total_original = 1000;
		order2.volume_cum = 500;
		order2.amount_cum = 9000.90;
		order2.volume_leaves = 500;
		order2.volume_cancelled = 0;
		order2.status = FTDC_OS_PART_TRADED;
		order.front_id = 1;
		order.session_id = 1;
		order.order_ref = 2;
		order2.order_sys_id = 2;
		order2.client_order_id = "0000000002";
		order2.order_exchange_id = "0000000000000002";
		order2.pbu_id = MOCK_SH_PBU_ID;
		order2.branch_id = MOCK_BRANCH_ID;
		order2.exchange_branch_id = MOCK_SH_BRANCH_ID;

		rc = mco_trans_commit(t);
		//order.order_sys_sno = 1;
	}
}


void populate_db_inner_execution_report(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int  j;

	Order order;
	Order order2;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK == rc)
	{
		InnerExecutionReport report;
		report.create(t);
		report.investor_id = MOCK_INVESTOR_ID;
		report.security_account = MOCK_SH_SECU_ACCOUNT;
		report.instrument_code = "600000";
		report.exchange_type = FTDC_ET_SH;
		report.bs_flag = FTDC_D_BUY;
		report.price_type = FTDC_OPT_HS_Limit;
		report.price = 17.6;
		report.branch_id = MOCK_BRANCH_ID;
		report.exchange_branch_id = MOCK_SH_BRANCH_ID;
		report.pbu_id = MOCK_SH_PBU_ID;
		report.exec_type = FTDC_ET_Trade;
		report.volume_last = 100;
		report.price_last = 15.5;
		report.order_exchange_id = "0000000000000001";
		report.report_exchange_id = "0000000000000000";
		rc = mco_trans_commit(t);
	}
	
}

void populate_test(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int  j;

	Order order;
	Order order2;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK == rc)
	{
		TestER test;
		TestER test2;
		TestER test3;
		TestER test4;
		test.create(t);
		test2.create(t);
		test3.create(t);
		test4.create(t);
		test.investor_id = 99;
		test.sno = 1;
		test2.investor_id = 99;
		test2.sno = 2;
		test3.investor_id = 99;
		test3.sno = 3;
		test4.investor_id = 100;
		test4.sno = 1;
		mco_trans_commit(t);
	}
}


void populate_db_sysconfig(mco_db_h db)
{
	mco_trans_h t = 0;
	MCO_RET rc = MCO_S_OK;
	int  j;

	Order order;
	Order order2;
	rc = mco_trans_start(db, MCO_READ_WRITE, MCO_TRANS_FOREGROUND, &t);
	if (MCO_S_OK == rc)
	{
		SysConfig config;
		config.create(t);
		config.config_id = 0;
		config.broker_id = 8080;
		config.trade_date = "20190123";
		config.system_date = "20190123";
		mco_trans_commit(t);
	}
}