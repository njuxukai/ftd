// SampleClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TraderClient.h"

int main()
{
	TraderClient client;
	ApiParameter parameter = { 0 };
	client.setParameter(parameter);
	client.connect();
	while (1 == 1)
	{
		int c = getchar();
		if (c == 'a')
		{
			std::cout << "InputOrder\n";
		}
	}
    return 0;
}

