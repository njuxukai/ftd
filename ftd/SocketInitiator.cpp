/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifdef _MSC_VER
#include "stdafx.h"
#else
#include "config.h"
#endif

#include "SocketInitiator.h"
#include "Session.h"
#include "Settings.h"
#include "SessionFactory.h"

#include "logger/logger.h"
namespace FTD
{
SocketInitiator::SocketInitiator( Application& application,
                                  const PortSettings& settings )
throw( ConfigError )
: Initiator( application, settings ),
  m_connector( 1 ), m_lastConnect( 0 ),
  m_reconnectInterval( 5 ), m_noDelay( false ), m_sendBufSize( 0 ),
  m_rcvBufSize( 0 ) 
{
}


SocketInitiator::~SocketInitiator()
{
	SocketConnections::iterator it;
	for (it = m_connections.begin(); it != m_connections.end(); it++)
	{
		it->second->setSession(0);
	}
	for (it = m_pendingConnections.begin(); it != m_pendingConnections.end(); it++)
	{
		it->second->setSession(0);
	}
	m_connections.clear();
	m_pendingConnections.clear();
}

void SocketInitiator::onConfigure( const PortSettings& s )
throw ( ConfigError )
{
  const Dictionary& dict = s.get();

  if( dict.has( RECONNECT_INTERVAL ) )
    m_reconnectInterval = dict.getInt( RECONNECT_INTERVAL );
  if( dict.has( SOCKET_NODELAY ) )
    m_noDelay = dict.getBool( SOCKET_NODELAY );
  if( dict.has( SOCKET_SEND_BUFFER_SIZE ) )
    m_sendBufSize = dict.getInt( SOCKET_SEND_BUFFER_SIZE );
  if( dict.has( SOCKET_RECEIVE_BUFFER_SIZE ) )
    m_rcvBufSize = dict.getInt( SOCKET_RECEIVE_BUFFER_SIZE );
}

void SocketInitiator::onInitialize( const PortSettings& s )
throw ( RuntimeError )
{
}

void SocketInitiator::onStart()
{
	
  connect();
  root_log(LOG_DEBUG, "first connect");
  while ( !isStopped() ) 
  {
    m_connector.block( *this, false, 1.0 );
    onTimeout( m_connector );
  }

  time_t start = 0;
  time_t now = 0;

  ::time( &start );
  while ( isLoggedOn() )
  {
    m_connector.block( *this );
    if( ::time(&now) -5 >= start )
      break;
  }
}

bool SocketInitiator::onPoll( double timeout )
{
  time_t start = 0;
  time_t now = 0;

  if( isStopped() )
  {
    if( start == 0 )
      ::time( &start );
    if( !isLoggedOn() )
      return false;
    if( ::time(&now) - 5 >= start )
      return false;
  }

  m_connector.block( *this, true, timeout );
  return true;
}

void SocketInitiator::onStop()
{
}

int SocketInitiator::doConnect( const PortID& p, const Dictionary& d )
{
	int result = -1;
  try
  {
    std::string address;
    short port = 0;
    std::string sourceAddress;
    short sourcePort = 0;




    getHost( p, d, address, port, sourceAddress, sourcePort );

    getLog()->onEvent( "Connecting to " + address + " on port " + IntConvertor::convert((unsigned short)port) + " (Source " + sourceAddress + ":" + IntConvertor::convert((unsigned short)sourcePort) + ")");

	
	int socket = m_connector.connect(address, port, m_noDelay, m_sendBufSize, m_rcvBufSize, sourceAddress, sourcePort );
	root_log(LOG_DEBUG, "创建socket[%d]", socket);

	SessionID s;
	std::string randomString;
	bool generateIDResult = Session::allocateNextSessionID(s, randomString);
	Session::SPtr pSession = createSession(s, d);
    m_pendingConnections[ socket ] 
      = std::make_shared<SocketConnection>(socket, pSession, &m_connector.getMonitor());
	pSession->setResponder(m_pendingConnections[socket]);
  }
  catch (std::exception&) { result = -1; }
  return result;
}

void SocketInitiator::onConnect( SocketConnector&, int s )
{
  SocketConnections::iterator i = m_pendingConnections.find( s );
  if( i == m_pendingConnections.end() ) return;
  SocketConnection::SPtr pSocketConnection = i->second;
  
  m_connections[s] = pSocketConnection;
  m_pendingConnections.erase( i );
  setConnected( pSocketConnection->getSession()->getSessionID() );
  m_application.onConnect(pSocketConnection->getSession()->getSessionID());
  pSocketConnection->onTimeout();
  //发送登录信息
  //pSocketConnection->getSession()
}

void SocketInitiator::onWrite( SocketConnector& connector, int s )
{
  SocketConnections::iterator i = m_connections.find( s );
  if ( i == m_connections.end() ) return ;
  SocketConnection::SPtr pSocketConnection = i->second;
  if( pSocketConnection->processQueue() )
    pSocketConnection->unsignal();
}

bool SocketInitiator::onData( SocketConnector& connector, int s )
{
  SocketConnections::iterator i = m_connections.find( s );
  if ( i == m_connections.end() ) return false;
  SocketConnection::SPtr pSocketConnection = i->second;
  return pSocketConnection->read( connector );
}

void SocketInitiator::onDisconnect( SocketConnector&, int s )
{
  SocketConnections::iterator i = m_connections.find( s );
  SocketConnections::iterator j = m_pendingConnections.find( s );

  SocketConnection::SPtr pSocketConnection = 0;
  if (i != m_connections.end())
  {
	  pSocketConnection = i->second;
  }
    
  if (j != m_pendingConnections.end())
  {
	  pSocketConnection = j->second;
  }
    
  if(pSocketConnection )
  {
	  Session::SPtr pSession = pSocketConnection->getSession();
	  if (pSession)
	  {
		  pSession->disconnect(DISCONNECT_DEFAULT);
		  destroySession(pSession);
	  }

	  m_connections.erase(s);
	  m_pendingConnections.erase(s);
  }
  //connect();
  //m_needReconnect = true;
  
}

void SocketInitiator::onError( SocketConnector& connector )
{
  onTimeout( connector );
}

void SocketInitiator::onTimeout( SocketConnector& )
{

	root_log(LOG_DEBUG, "SocketInitiator::onTimeout");
  time_t now;
  ::time( &now );

  
  if ( m_doConnect 
	  && (m_connections.size() + m_pendingConnections.size() == 0)
	  && (now - m_lastConnect) >= m_reconnectInterval )
  {
	  root_log(LOG_DEBUG, "当前满足重连条件:连接数[%d],待连接数[%d],重传间隔[%lld],重传间隔参数[%d]",
		  m_connections.size(), m_pendingConnections.size(), now - m_lastConnect,
		  m_reconnectInterval);
    connect();
    m_lastConnect = now;
	//m_needReconnect = false;
  }

  SocketConnections::iterator i;
  for ( i = m_connections.begin(); i != m_connections.end(); ++i )
    i->second->onTimeout();
}

void SocketInitiator::getHost( const PortID& s, const Dictionary& d,
                               std::string& address, short& port,
                               std::string& sourceAddress, short& sourcePort)
{
	address = d.getString(SOCKET_CONNECT_HOST);
	port = (short)d.getInt(SOCKET_CONNECT_PORT);

	if (d.has(SOCKET_CONNECT_SOURCE_HOST))
		sourceAddress = d.getString(SOCKET_CONNECT_SOURCE_HOST);
	if (d.has(SOCKET_CONNECT_SOURCE_PORT))
		sourcePort = (short)d.getInt(SOCKET_CONNECT_SOURCE_PORT);
}

}
