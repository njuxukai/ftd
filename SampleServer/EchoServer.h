#pragma once
#include <ftd/SocketAcceptor.h>

class EchoServer
{
public:
	EchoServer(std::string cfgFile);
	~EchoServer();

	void connect();
	void disconnect();

private:
	std::string m_cfgFile;
	FTD::Acceptor* m_acceptor;
};