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

#include "SocketAcceptor.h"
#include "Session.h"
#include "Settings.h"
#include "Utility.h"
#include "Exceptions.h"

#include "logger/logger.h"
namespace FTD
{
SocketAcceptor::SocketAcceptor( Application& application,
                                const PortSettings& settings ) throw( ConfigError )
: Acceptor( application, settings ),
  m_pServer( 0 ) {}


SocketAcceptor::~SocketAcceptor()
{

	SocketConnections::iterator iter;
	for (iter = m_connections.begin(); iter != m_connections.end(); ++iter)
	{
		iter->second->setSession(0);
	}

	m_connections.clear();
}

void SocketAcceptor::onConfigure( const PortSettings& s )
throw ( ConfigError )
{
  std::set<PortID> ports = s.getPorts();
  std::set<PortID>::iterator i;
  for( i = ports.begin(); i != ports.end(); ++i )
  {
    const Dictionary& settings = s.get( *i );
    settings.getInt( SOCKET_ACCEPT_PORT );
    if( settings.has(SOCKET_REUSE_ADDRESS) )
      settings.getBool( SOCKET_REUSE_ADDRESS );
    if( settings.has(SOCKET_NODELAY) )
      settings.getBool( SOCKET_NODELAY );
  }
}

void SocketAcceptor::onInitialize( const PortSettings& s )
throw ( RuntimeError )
{
  short port = 0;

  try
  {
    m_pServer = new SocketServer( 1 );

    std::set<PortID> ports = s.getPorts();
    std::set<PortID>::iterator i = ports.begin();
    for( ; i != ports.end(); ++i )
    {
      const Dictionary& settings = s.get( *i );
      port = i->getPort();

      const bool reuseAddress = settings.has( SOCKET_REUSE_ADDRESS ) ? 
        settings.getBool( SOCKET_REUSE_ADDRESS ) : true;

      const bool noDelay = settings.has( SOCKET_NODELAY ) ? 
        settings.getBool( SOCKET_NODELAY ) : false;

      const int sendBufSize = settings.has( SOCKET_SEND_BUFFER_SIZE ) ?
        settings.getInt( SOCKET_SEND_BUFFER_SIZE ) : 0;

      const int rcvBufSize = settings.has( SOCKET_RECEIVE_BUFFER_SIZE ) ?
        settings.getInt( SOCKET_RECEIVE_BUFFER_SIZE ) : 0;

	  m_portToSessions[port] = Sessions();
      m_pServer->add( port, reuseAddress, noDelay, sendBufSize, rcvBufSize );      
    }    
  }
  catch( SocketException& e )
  {
    throw RuntimeError( "Unable to create, bind, or listen to port "
                       + IntConvertor::convert( (unsigned short)port ) + " (" + e.what() + ")" );
  }
}

void SocketAcceptor::onStart()
{
  while ( !isStopped() && m_pServer && m_pServer->block( *this ) ) {}

  if( !m_pServer )
    return;

  time_t start = 0;
  time_t now = 0;

  ::time( &start );
  while ( isLoggedOn() )
  {
    m_pServer->block( *this );
    if( ::time(&now) -5 >= start )
      break;
  }

  m_pServer->close();
  delete m_pServer;
  m_pServer = 0;
}

bool SocketAcceptor::onPoll( double timeout )
{
  if( !m_pServer )
    return false;

  time_t start = 0;
  time_t now = 0;

  if( isStopped() )
  {
    if( start == 0 )
      ::time( &start );
    if( !isLoggedOn() )
    {
      start = 0;
      return false;
    }
    if( ::time(&now) - 5 >= start )
    {
      start = 0;
      return false;
    }
  }

  m_pServer->block( *this, true, timeout );
  return true;
}

void SocketAcceptor::onStop()
{
}

void SocketAcceptor::onConnect( SocketServer& server, int a, int s )
{
	root_log(LOG_DEBUG, "onConnect,acceptor=[%d],new socket=[%d]", a, s);
  if ( !socket_isValid( s ) ) return;
  SocketConnections::iterator i = m_connections.find( s );
  if ( i != m_connections.end() ) return;
  int port = server.socketToPort( a );

  SessionID id;
  std::string strRandom;
  bool generateIDResult = Session::allocateNextSessionID(id, strRandom);
  if (!generateIDResult)
	  return;
  Dictionary settings = m_setting[port];
  Sessions sessions = m_portToSessions[port];
  Session::SPtr pSession = createSession(id, settings);
  if (!pSession)
	  return;   
  SocketConnection::SPtr pSocketConnection = std::make_shared<SocketConnection>( s, pSession, &server.getMonitor());
  m_connections[s] = pSocketConnection;
  pSession->setResponder(pSocketConnection);


  std::stringstream stream;
  stream << "Accepted connection from " << socket_peername( s ) << " on port " << port;
  if( getLog() )
    getLog()->onEvent( stream.str() );
}

void SocketAcceptor::onWrite( SocketServer& server, int s )
{
  SocketConnections::iterator i = m_connections.find( s );
  if ( i == m_connections.end() ) return ;
  SocketConnection::SPtr pSocketConnection = i->second;
  if( pSocketConnection->processQueue() )
    pSocketConnection->unsignal();
}

bool SocketAcceptor::onData( SocketServer& server, int s )
{
  SocketConnections::iterator i = m_connections.find( s );
  if ( i == m_connections.end() ) return false;
  SocketConnection::SPtr pSocketConnection = i->second;
  return pSocketConnection->read( *this, server );
}

void SocketAcceptor::onDisconnect( SocketServer&, int s )
{
  SocketConnections::iterator i = m_connections.find( s );
  if ( i == m_connections.end() ) return ;
  SocketConnection::SPtr pSocketConnection = i->second;

  Session::SPtr pSession = pSocketConnection->getSession();
  pSocketConnection->setSession(0);
  if (pSession)
  {
	  pSession->disconnect(DISCONNECT_DEFAULT);
	  destroySession(pSession);
  }
  m_connections.erase( s );
  //delete pSocketConnection;
}

void SocketAcceptor::onError( SocketServer& ) 
{
}

void SocketAcceptor::onTimeout( SocketServer& )
{
  SocketConnections::iterator i;
  for ( i = m_connections.begin(); i != m_connections.end(); ++i )
    i->second->onTimeout();
}
}
