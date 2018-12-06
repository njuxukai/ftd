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

#include "Acceptor.h"
#include "Utility.h"
#include "Session.h"
#include "SessionFactory.h"
#include <algorithm>
#include <fstream>

namespace FTD
{
Acceptor::Acceptor( Application& application,
                    PackageStoreFactory& packageStoreFactory,
                    const PortSettings& setting )
throw( ConfigError )
  : m_threadid( 0 ),
  m_application( application ),
  m_packageStoreFactory(packageStoreFactory),
  m_portSettings( setting ),
  m_pLogFactory( 0 ),
  m_pLog( 0 ),
  m_firstPoll( true ),
  m_stop( true )
{
  initialize();
}

Acceptor::Acceptor( Application& application,
					PackageStoreFactory& packageStoreFactory,
                    const PortSettings& setting,
                    LogFactory& logFactory )
throw( ConfigError )
: m_threadid( 0 ),
  m_application( application ),
  m_packageStoreFactory(packageStoreFactory),
  m_portSettings(setting),
  m_pLogFactory( &logFactory ),
  m_pLog( logFactory.create() ),
  m_firstPoll( true ),
  m_stop( true )
{
  initialize();
}

void Acceptor::initialize() throw ( ConfigError )
{
	std::set<PortID> ids = m_portSettings.getPorts();
	for (auto it = ids.begin(); it != ids.end(); it++)
	{
		m_setting[it->getPort()] = m_portSettings.get(*it);
	}
	m_pSessionFactory = new SessionFactory(m_application, m_packageStoreFactory, m_pLogFactory);
  
}

Acceptor::~Acceptor()
{
  Sessions::iterator i;
  for ( i = m_sessions.begin(); i != m_sessions.end(); ++i )
    delete i->second;

  if( m_pLogFactory && m_pLog )
    m_pLogFactory->destroy( m_pLog );

  if (m_pSessionFactory)
	  delete m_pSessionFactory;
  m_pSessionFactory = nullptr;
}

/*
Session* Acceptor::getSession( const SessionID& sessionID ) const
{
  Sessions::const_iterator i = m_sessions.find( sessionID );
  if( i != m_sessions.end() )
    return i->second;
  else
    return 0;
}

*/

Session* Acceptor::createSession(const SessionID& id, const Dictionary& settings)
{
	if (!m_pSessionFactory)
		return nullptr;
	Session* pSession = m_pSessionFactory->create(id, settings, true);
	m_sessions[id] = pSession;
	m_sessionIDs.insert(id);
	return pSession;
}

void Acceptor::destroySession(Session* pSession)
{
	if (!pSession)
		return;
	m_sessions.erase(pSession->getSessionID());
	m_sessionIDs.erase(pSession->getSessionID());
	if (m_pSessionFactory)
	{
		m_pSessionFactory->destroy(pSession);
	}
}

///效率不高
/*
int Acceptor::allocateNextSessionID()
{
	int nextSessionID = 1;
	for (auto it = m_sessionIDs.begin(); it != m_sessionIDs.end(); it++)
	{
		if (*it >= nextSessionID)
			nextSessionID = *it + 1;
	}
	return nextSessionID;
}
*/

void Acceptor::start() throw ( ConfigError, RuntimeError )
{
  m_stop = false;
  onConfigure( m_portSettings );
  onInitialize(m_portSettings);


  if( !thread_spawn( &startThread, this, m_threadid ) )
    throw RuntimeError("Unable to spawn thread");
}

void Acceptor::block() throw ( ConfigError, RuntimeError )
{
  m_stop = false;
  onConfigure(m_portSettings);
  onInitialize(m_portSettings);

  startThread(this);
}

bool Acceptor::poll( double timeout ) throw ( ConfigError, RuntimeError )
{
  if( m_firstPoll )
  {
    m_stop = false;
    onConfigure(m_portSettings);
    onInitialize(m_portSettings);
    m_firstPoll = false;
  }

  return onPoll( timeout );
}

void Acceptor::stop( bool force )
{
  if( isStopped() ) return;


  std::vector<Session*> enabledSessions;

  Sessions sessions = m_sessions;
  Sessions::iterator i = sessions.begin();
  for ( ; i != sessions.end(); ++i )
  {
    Session* pSession = Session::lookupSession(i->first);
    if( pSession && pSession->isEnabled() )
    {
      enabledSessions.push_back( pSession );
      pSession->logout();
      Session::unregisterSession( pSession->getSessionID() );
    }
  }

  if( !force )
  {
    for ( int second = 1; second <= 10 && isLoggedOn(); ++second )
      process_sleep( 1 );
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

bool Acceptor::isLoggedOn()
{
  Sessions sessions = m_sessions;
  Sessions::iterator i = sessions.begin();
  for ( ; i != sessions.end(); ++i )
  {
    if( i->second->isLoggedOn() )
      return true;
  }
  return false;
}


THREAD_PROC Acceptor::startThread( void* p )
{
  Acceptor * pAcceptor = static_cast < Acceptor* > ( p );
  pAcceptor->onStart();
  return 0;
}
}
