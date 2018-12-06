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

namespace FTD
{
SocketInitiator::SocketInitiator( Application& application,
                                  PackageStoreFactory& factory,
                                  const PortSettings& settings )
throw( ConfigError )
: Initiator( application, factory, settings ),
  m_connector( 1 ), m_lastConnect( 0 ),
  m_reconnectInterval( 30 ), m_noDelay( false ), m_sendBufSize( 0 ),
  m_rcvBufSize( 0 ) 
{
}

SocketInitiator::SocketInitiator( Application& application,
                                  PackageStoreFactory& factory,
                                  const PortSettings& settings,
                                  LogFactory& logFactory )
throw( ConfigError )
: Initiator( application, factory, settings, logFactory ),
  m_connector( 1 ), m_lastConnect( 0 ),
  m_reconnectInterval( 30 ), m_noDelay( false ), m_sendBufSize( 0 ),
  m_rcvBufSize( 0 )
{
}

SocketInitiator::~SocketInitiator()
{
  SocketConnections::iterator i;
  for (i = m_connections.begin();
       i != m_connections.end(); ++i)
    delete i->second;

  for (i = m_pendingConnections.begin();
       i != m_pendingConnections.end(); ++i)
    delete i->second;
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

  while ( !isStopped() ) {
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

	
	int socket;
    int result = m_connector.connect(socket, address, port, m_noDelay, m_sendBufSize, m_rcvBufSize, sourceAddress, sourcePort );
	if (result < 0)
	{
		return result;
	}

	SessionID s = Session::allocateNextSessionID();
	Session* pSession = createSession(s, d);
    m_pendingConnections[ socket ] 
      = new SocketConnection(socket, pSession, &m_connector.getMonitor(), false);
  }
  catch (std::exception&) { result = -1; }
  return result;
}

void SocketInitiator::onConnect( SocketConnector&, int s )
{
  SocketConnections::iterator i = m_pendingConnections.find( s );
  if( i == m_pendingConnections.end() ) return;
  SocketConnection* pSocketConnection = i->second;
  
  m_connections[s] = pSocketConnection;
  m_pendingConnections.erase( i );
  setConnected( pSocketConnection->getSession()->getSessionID() );
  pSocketConnection->onTimeout();
  //·¢ËÍµÇÂ¼ÐÅÏ¢
  //pSocketConnection->getSession()
}

void SocketInitiator::onWrite( SocketConnector& connector, int s )
{
  SocketConnections::iterator i = m_connections.find( s );
  if ( i == m_connections.end() ) return ;
  SocketConnection* pSocketConnection = i->second;
  if( pSocketConnection->processQueue() )
    pSocketConnection->unsignal();
}

bool SocketInitiator::onData( SocketConnector& connector, int s )
{
  SocketConnections::iterator i = m_connections.find( s );
  if ( i == m_connections.end() ) return false;
  SocketConnection* pSocketConnection = i->second;
  return pSocketConnection->read( connector );
}

void SocketInitiator::onDisconnect( SocketConnector&, int s )
{
  SocketConnections::iterator i = m_connections.find( s );
  SocketConnections::iterator j = m_pendingConnections.find( s );

  SocketConnection* pSocketConnection = 0;
  if( i != m_connections.end() ) 
    pSocketConnection = i->second;
  if( j != m_pendingConnections.end() )
    pSocketConnection = j->second;
  if( !pSocketConnection )
    return;

  Session* pSession = pSocketConnection->getSession();
  if ( pSession )
  {
    pSession->disconnect();
    setDisconnected( pSession->getSessionID() );
	m_sessions.erase( pSession->getSessionID() );
	m_pSessionFactory->destroy(pSession);
  }

  m_connections.erase( s );
  m_pendingConnections.erase( s );
  delete pSocketConnection;
}

void SocketInitiator::onError( SocketConnector& connector )
{
  onTimeout( connector );
}

void SocketInitiator::onTimeout( SocketConnector& )
{
  time_t now;
  ::time( &now );

  if ( (now - m_lastConnect) >= m_reconnectInterval )
  {
    connect();
    m_lastConnect = now;
  }

  SocketConnections::iterator i;
  for ( i = m_connections.begin(); i != m_connections.end(); ++i )
    i->second->onTimeout();
}

void SocketInitiator::getHost( const PortID& s, const Dictionary& d,
                               std::string& address, short& port,
                               std::string& sourceAddress, short& sourcePort)
{
  int num = 0;
  SessionToHostNum::iterator i = m_sessionToHostNum.find( s );
  if ( i != m_sessionToHostNum.end() ) num = i->second;

  std::stringstream hostStream;
  hostStream << SOCKET_CONNECT_HOST << num;
  std::string hostString = hostStream.str();

  std::stringstream portStream;
  portStream << SOCKET_CONNECT_PORT << num;
  std::string portString = portStream.str();

  sourcePort = 0;
  sourceAddress.empty();

  if( d.has(hostString) && d.has(portString) )
  {
    address = d.getString( hostString );
    port = ( short ) d.getInt( portString );

    std::stringstream sourceHostStream;
    sourceHostStream << SOCKET_CONNECT_SOURCE_HOST << num;
    hostString = sourceHostStream.str();
    if( d.has(hostString) )
      sourceAddress = d.getString( hostString );

    std::stringstream sourcePortStream;
    sourcePortStream << SOCKET_CONNECT_SOURCE_PORT << num;
    portString = sourcePortStream.str();
    if( d.has(portString) )
      sourcePort = ( short ) d.getInt( portString );
  }
  else
  {
    num = 0;
    address = d.getString( SOCKET_CONNECT_HOST );
    port = ( short ) d.getInt( SOCKET_CONNECT_PORT );

    if( d.has(SOCKET_CONNECT_SOURCE_HOST) )
      sourceAddress = d.getString( SOCKET_CONNECT_SOURCE_HOST );
    if( d.has(SOCKET_CONNECT_SOURCE_PORT) )
      sourcePort = ( short ) d.getInt( SOCKET_CONNECT_SOURCE_PORT );
  }

  m_sessionToHostNum[ s ] = ++num;
}
}
