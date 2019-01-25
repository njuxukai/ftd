#ifndef DB_UTIL_H
#define DB_UTIL_H

#include "genericdb.hpp"

#define SEQ_ORDER_TAG 1
#define SEQ_INNER_EXECUTION_REPORT_TAG 2
#define SEQ_USER_EXECUTION_REPORT_TAG 3

// 为委托、内部成交回报、用户成交回报等 生成序列号
int get_next_sno(int topic, mco_trans_h t);

// 生成报盘ID  sys_id(int)->clordID(char<10>)
std::string generate_client_order_id(int order_sys_id);

// int->char<5>
std::string generate_sh_branch_id_string(int sh_branch_id);

// int->char<4>
std::string generate_sz_branch_id_string(int sz_branch_id);

//int->char<5>
std::string generate_sh_pbu_id_string(int sh_pub_id);

//int->char<6>
std::string generate_sz_pbu_id_string(int sz_pub_id);

#endif