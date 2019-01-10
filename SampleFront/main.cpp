// SampleServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FtdRouter.h"

int main(int argc, char** argv)
{
	std::string file = "";
	if (argc < 2)
	{
		file = "echo_server.cfg";
	}
	else
	{
		file = argv[1];
	}
	FtdRouter server(file);
	server.start();
	
	while (1 == 1)
	{
		char c = getchar();
	}
}

