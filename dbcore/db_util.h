#ifndef DB_UTIL_H
#define DB_UTIL_H

#include "genericdb.hpp"

#define SEQ_ORDER_TAG 1
#define SEQ_INNER_EXECUTION_REPORT_TAG 2
#define SEQ_USER_EXECUTION_REPORT_TAG 3

// Ϊί�С��ڲ��ɽ��ر����û��ɽ��ر��� �������к�
int get_next_sno(int topic, mco_trans_h t);

// ���ɱ���ID  sys_id(int)->clordID(char<10>)
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