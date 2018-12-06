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

#include "Initiator.h"
#include "Utility.h"
#include "Session.h"
#include "SessionFactory.h"
#include <algorithm>
#include <fstream>

namespace FTD
{
Initiator::Initiator( Application& application,
                      PackageStoreFactory& packageStoreFactory,
                      const PortSettings& settings ) throw( ConfigError )
: m_threadid( 0 ),
  m_application( application ),
  m_packageStoreFactory( packageStoreFactory ),
  m_settings( settings ),
  m_pLogFactory( 0 ),
  m_pLog( 0 ),
  m_firstPoll( true ),
  m_stop( true )
{ initialize(); }

Initiator::Initiator( Application& application,
					  PackageStoreFactory& messageStoreFactory,
                      const PortSettings& settings,
                      LogFactory& logFactory ) throw( ConfigError )
: m_threadid( 0 ),
  m_application( application ),
  m_packageStoreFactory( messageStoreFactory ),
  m_settings( settings ),
  m_pLogFactory( &logFactory ),
  m_pLog( logFactory.create() ),
  m_firstPoll( true ),
  m_stop( true )
{ initialize(); }

void Initiator::initialize() throw ( ConfigError )
{
  

  m_pSessionFactory = new SessionFactory( m_application, m_packageStoreFactory,
                          m_pLogFactory );

  m_ports = m_settings.getPorts();
  std::set < SessionID > ::iterator i;

  if ( !m_sessions.size() )
    throw ConfigError( "No portid defined for initiator" );
}

Initiator::~Initiator()
{
  Sessions::iterator i;
  for ( i = m_sessions.begin(); i != m_sessions.end(); ++i )
    delete i->second;

  if (m_pSessionFactory)
  {
	  delete m_pSessionFactory;
	  m_pSessionFactory = nullptr;
  }
  if( m_pLogFactory && m_pLog )
    m_pLogFactory->destroy( m_pLog );
}

Session* Initiator::getSession( const SessionID& sessionID,
                                Responder& responder )
{
  Sessions::iterator i = m_sessions.find( sessionID );
  if ( i != m_sessions.end() )
  {
    i->second->setResponder( &responder );
    return i->second;
  }
  return 0;
}

Session* Initiator::getSession( const SessionID& sessionID ) const
{
  Sessions::const_iterator i = m_sessions.find( sessionID );
  if( i != m_sessions.end() )
    return i->second;
  else
    return 0;
}

const Dictionary* const Initiator::getPortSettings( const PortID& portID ) const
{
  try
  {
    return &m_settings.get(portID);
  }
  catch( ConfigError& )
  {
    return 0;
  }
}

void Initiator::connect()
{
  Locker l(m_mutex);

  PortIDs disconnected = m_ports;
  PortIDs::iterator i = disconnected.begin();
  for ( ; i != disconnected.end(); ++i )
  {
	  if (m_badPorts.find(*i) != m_badPorts.end())
		  continue;
      int socket = doConnect( *i, m_settings.get( *i ));
	  if (socket > 0)
	  {
		  return;
	  }
	  else
	  {
		  m_badPorts.insert(*i);
	  }
  }
}

void Initiator::setPending( const SessionID& sessionID )
{
  Locker l(m_mutex);

  m_pending.insert( sessionID );
  m_connected.erase( sessionID );
  //m_disconnected.erase( sessionID );
}

void Initiator::setConnected( const SessionID& sessionID )
{
  Locker l(m_mutex);

  m_pending.erase( sessionID );
  m_connected.insert( sessionID );
  //m_disconnected.erase( sessionID );
}

void Initiator::setDisconnected( const SessionID& sessionID )
{
  Locker l(m_mutex);

  m_pending.erase( sessionID );
  m_connected.erase( sessionID );
  //m_disconnected.insert( sessionID );
}

bool Initiator::isPending( const SessionID& sessionID )
{
  Locker l(m_mutex);
  return m_pending.find( sessionID ) != m_pending.end();
}

bool Initiator::isConnected( const SessionID& sessionID )
{
  Locker l(m_mutex);
  return m_connected.find( sessionID ) != m_connected.end();
}


void Initiator::start() throw ( ConfigError, RuntimeError )
{
  m_stop = false;
  onConfigure( m_settings );
  onInitialize( m_settings );


  if( !thread_spawn( &startThread, this, m_threadid ) )
    throw RuntimeError("Unable to spawn thread");
}


void Initiator::block() throw ( ConfigError, RuntimeError )
{
  m_stop = false;
  onConfigure( m_settings );
  onInitialize( m_settings );

  startThread(this);
}

bool Initiator::poll( double timeout ) throw ( ConfigError, RuntimeError )
{
  if( m_firstPoll )
  {
    m_stop = false;
    onConfigure( m_settings );
    onInitialize( m_settings );
    connect();
    m_firstPoll = false;
  }

  return onPoll( timeout );
}

void Initiator::stop( bool force )
{
  if( isStopped() ) return;


  std::vector<Session*> enabledSessions;

  SessionIDs connected;

  {
    Locker l(m_mutex);
    connected = m_connected;
  }

  SessionIDs::iterator i = connected.begin();
  for ( ; i != connected.end(); ++i )
  {
    Session* pSession = Session::lookupSession(*i);
    if( pSession && pSession->isEnabled() )
    {
      enabledSessions.push_back( pSession );
      pSession->logout();
    }
  }

  if( !force )
  {
    for ( int second = 1; second <= 10 && isLoggedOn(); ++second )
      process_sleep( 1 );
  }

  {
    Locker l(m_mutex);
    for ( i = connected.begin(); i != connected.end(); ++i )
      setDisconnected( Session::lookupSession(*i)->getSessionID() );
  }

  m_stop = true;
  onStop();
  if( m_threadid )
    thread_join( m_threadid );
  m_threadid = 0;

  std::vector<Session*>::iterator session = enabledSessions.begin();
  for( ; session != enabledSessions.end(); ++session )
    (*session)->logon();
}

bool Initiator::isLoggedOn()
{
  Locker l(m_mutex);

  SessionIDs connected = m_connected;
  SessionIDs::iterator i = connected.begin();
  for ( ; i != connected.end(); ++i )
  {
    if( Session::lookupSession(*i)->isLoggedOn() )
      return true;
  }
  return false;
}

THREAD_PROC Initiator::startThread( void* p )
{
  Initiator * pInitiator = static_cast < Initiator* > ( p );
  pInitiator->onStart();
  return 0;
}

Session* Initiator::createSession(const SessionID& id, const Dictionary& settings)
{
	if (!m_pSessionFactory)
		return nullptr;
	Session* pSession = m_pSessionFactory->create(id, settings, false);
	m_sessions[id] = pSession;
	m_sessionIDs.insert(id);
	setPending(id);
	return pSession;
}

void Initiator::destroySession(Session* pSession)
{
	if (!pSession)
		return;
	m_connected.erase(pSession->getSessionID());
	m_pending.erase(pSession->getSessionID());
	m_sessionIDs.erase(pSession->getSessionID());
	m_sessions.erase(pSession->getSessionID());
	m_pSessionFactory->destroy(pSession);
}

}
