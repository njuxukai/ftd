// SampleServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "CoreServer.h"

int main(int argc, char** argv)
{
	std::string file = "";
	if (argc < 2)
	{
		file = "sample_core.cfg";
	}
	else
	{
		file = argv[1];
	}
	CoreServer server(file);
	server.start();

	while (1 == 1)
	{
		char c = getchar();
	}
}

