#include "stdafx.h"
#include "EchoServer.h"

EchoServer::EchoServer(std::string cfgFile) :m_cfgFile(cfgFile), m_acceptor(0)
{
}

EchoServer::~EchoServer()
{}

void EchoServer::connect()
{
	//1 create initiator 
	FTD::PortSettings settings(m_cfgFile);
	m_acceptor = new FTD::SocketAcceptor((FTD::Application&)*this, settings);
	m_acceptor->start();
}

void EchoServer::disconnect()
{
	if (m_acceptor)
		m_acceptor->stop();
	delete m_acceptor;
	m_acceptor = 0;
}