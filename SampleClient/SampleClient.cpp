// SampleClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TraderClient.h"

int main()
{
	TraderClient client;
	ApiParameter parameter = { 0 };
	parameter.loginField.UserID = 100;
	strcpy(parameter.loginField.Password, "test");
	parameter.loginField.BrokerID = 99;
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
			int result = client.manualTestInputOrder();
			std::cout << "result=" << result << std::endl;
		}
	}
    return 0;
}

