// SampleServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "EchoServer.h"

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
	EchoServer server(file);
	server.connect();
	
	while (1 == 1)
	{
		char c = getchar();
	}
}

