// SampleServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FrontServer.h"
#include "logger/logger.h"

int main(int argc, char** argv)
{
#ifdef _DEBUG
	set_log_level(LOG_DEBUG);
#else
	set_log_level(LOG_INFO);
#endif

	std::string file = "";
	if (argc < 2)
	{
		file = "echo_server.cfg";
	}
	else
	{
		file = argv[1];
	}
	FrontServer server(file);
	server.start();
	
	while (1 == 1)
	{
		char c = getchar();
	}
}

