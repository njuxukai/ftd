// SampleClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MdUserClient.h"

int main()
{
	MdUserClient client;
	ApiParameter parameter = { 0 };
	parameter.loginField.UserID = 100;
	strcpy(parameter.loginField.Password, "test");
	parameter.loginField.BrokerID = 8080;
	strcpy(parameter.frontAddress, "tcp:\\127.0.0.1:8000");
	strcpy(parameter.frontAddress2, "tcp:\\127.0.0.1:9000");
	strcpy(parameter.fileDirectory, ".\\");
	client.setParameter(parameter);
	client.connect();
	while (1 == 1)
	{
		int c = getchar();
		if (c == 'a')
		{
		}
	}
    return 0;
}

