/* -*- C++ -*- */

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

#ifndef FTD_INITIATOR_H
#define FTD_INITIATOR_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Application.h"
#include "PackageStore.h"
#include "Log.h"
#include "Responder.h"
#include "PortSettings.h"
#include "Exceptions.h"
#include "Mutex.h"
#include "Session.h"
#include <set>
#include <map>
#include <string>

namespace FTD
{
class Client;
class SessionFactory;

/**
 * Base for classes which act as an initiator for establishing connections.
 *
 * Most users will not need to implement one of these.  The default
 * SocketInitiator implementation will be used in most cases.
 */
class Initiator
{
public:
  Initiator( Application&, PackageStoreFactory&,
             const PortSettings& ) throw( ConfigError );
  Initiator( Application&, PackageStoreFactory&,
             const PortSettings&, LogFactory& ) throw( ConfigError );

  virtual ~Initiator();

  int send(Package& package) { return 0; }
  /// Start initiator.
  void start() throw ( ConfigError, RuntimeError );
  /// Block on the initiator
  void block() throw ( ConfigError, RuntimeError );
  /// Poll the initiator
  bool poll( double timeout = 0.0 ) throw ( ConfigError, RuntimeError );

  /// Stop initiator.
  void stop( bool force = false );

  //join thread 
  void join();

  /// Check to see if any sessions are currently logged on
  bool isLoggedOn();

  Session* getSession( const SessionID& sessionID, Responder& );

  const std::set<SessionID>& getSessions() const { return m_sessionIDs; }
  Session* getSession( const SessionID& sessionID ) const;
  const Dictionary* const getPortSettings( const PortID& portID ) const;

  bool has( const SessionID& id )
  { return m_sessions.find( id ) != m_sessions.end(); }

  bool isStopped() { return m_stop; }

public:
  Application& getApplication() { return m_application; }
  PackageStoreFactory& getPackageStoreFactory()
  { return m_packageStoreFactory; }

  Log* getLog() 
  { 
    if( m_pLog ) return m_pLog; 
    return &m_nullLog;
  }

protected:
  void setPending( const SessionID& );
  void setConnected( const SessionID& );
  void setDisconnected( const SessionID& );

  bool isPending( const SessionID& );
  bool isConnected( const SessionID& );
  bool isDisconnected( const SessionID& );
  void connect();
  
  Session* createSession(const SessionID& id, const Dictionary& settings);
  void destroySession(Session* pSession);
private:
  void initialize() throw ( ConfigError );

  /// Implemented to configure acceptor
  virtual void onConfigure( const PortSettings& ) throw ( ConfigError ) {};
  /// Implemented to initialize initiator
  virtual void onInitialize( const PortSettings& ) throw ( RuntimeError ) {};
  /// Implemented to start connecting to targets.
  virtual void onStart() = 0;
  /// Implemented to connect and poll for events.
  virtual bool onPoll( double timeout ) = 0;
  /// Implemented to stop a running initiator.
  virtual void onStop() = 0;
  /// Implemented to connect a session to its target.
  virtual int doConnect( const PortID&, const Dictionary& ) = 0;



  static THREAD_PROC startThread( void* p );

  

  thread_id m_threadid;
  Application& m_application;
  PackageStoreFactory& m_packageStoreFactory;
protected:
	typedef std::set < PortID > PortIDs;
	typedef std::set < SessionID> SessionIDs;
	typedef std::map < PortID, SessionID > PortIDSessionIDMap;
	typedef std::map < SessionID, Session* > Sessions;


	Sessions m_sessions;
	SessionIDs m_sessionIDs;
	SessionIDs m_pending;
	SessionIDs m_connected;
	PortIDs m_ports;
	PortIDs m_badPorts;
  PortSettings m_settings;
  SessionFactory* m_pSessionFactory;
private:
  LogFactory* m_pLogFactory;
  Log* m_pLog;
  NullLog m_nullLog;
  bool m_firstPoll;
  bool m_stop;
  Mutex m_mutex;
};
/*! @} */
}

#endif // FIX_INITIATOR_H
