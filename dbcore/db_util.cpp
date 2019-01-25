#include "db_util.h"
#include "Exceptions.h"
#include <boost/lexical_cast.hpp>

using namespace genericdb;

int get_next_sno(int topic, mco_trans_h t)
{
	int next_sno = 0;
	NextSNO entry;
	MCO_RET rc = NextSNO::Idx::find(t, topic, entry);
	if (rc == MCO_S_OK)
	{
		next_sno = entry.next_sno;
		entry.next_sno = next_sno + 1;
	}
	else if (rc == MCO_S_NOTFOUND)
	{
		next_sno = 1;
		entry.create(t);
		entry.topic_id = topic;
		entry.next_sno = next_sno + 1;
	}
	else
	{
		throw dbcore::SNoGenerateError(boost::lexical_cast<std::string>(topic));
	}
	return next_sno;
}


// Éú³É±¨ÅÌID  sys_id(int)->clordID(char<10>)
std::string generate_client_order_id(int order_sys_id)
{
	char buffer[11];
	sprintf(buffer, "%010d", order_sys_id);
	buffer[10] = 0;
	return std::string(buffer);
}

// int->char<5>
std::string generate_sh_branch_id_string(int sh_branch_id)
{
	char buffer[6];
	sprintf(buffer, "%05d", sh_branch_id);
	buffer[5] = 0;
	return std::string(buffer);
}

// int->char<4>
std::string generate_sz_branch_id_string(int sz_branch_id)
{
	char buffer[5];
	sprintf(buffer, "%04d", sz_branch_id);
	buffer[4] = 0;
	return std::string(buffer);
}

//int->char<5>
std::string generate_sh_pbu_id_string(int sh_pub_id)
{
	char buffer[6];
	sprintf(buffer, "%05d", sh_pub_id);
	buffer[5] = 0;
	return std::string(buffer);
}

//int->char<6>
std::string generate_sz_pbu_id_string(int sz_pub_id)
{
	char buffer[7];
	sprintf(buffer, "%06d", sz_pub_id);
	buffer[6] = 0;
	return std::string(buffer);
}