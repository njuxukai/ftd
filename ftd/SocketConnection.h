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

#ifndef FTD_SOCKETCONNECTION_H
#define FTD_SOCKETCONNECTION_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Parser.h"
#include "Responder.h"
#include "SessionID.h"
#include "session.h"
#include "SocketMonitor.h"
#include "Utility.h"
#include "Mutex.h"
#include <set>
#include "FTD30/Packages.h"

namespace FTD
{

class SocketAcceptor;
class SocketServer;
class SocketConnector;
class SocketInitiator;
class Session;

/// Encapsulates a socket file descriptor (single-threaded).
class SocketConnection : public Responder
{
public:
	typedef std::shared_ptr<SocketConnection> SPtr;
	typedef std::set<SessionID> Sessions;

  SocketConnection( int s, Session::SPtr pSession, SocketMonitor* pMonitor);
  virtual ~SocketConnection();

  int getSocket() const { return m_socket; }
  void setSession(Session::SPtr session) { m_pSession = session; }
  Session::SPtr getSession() const { return m_pSession; }

  bool read( SocketConnector& s );
  bool read( SocketAcceptor&, SocketServer& );
  bool processQueue();

  void signal()
  {
    Locker l( m_mutex );
    if( m_sendQueue.size() == 1 )
      m_pMonitor->signal( m_socket );
  }

  void unsignal()
  {
    Locker l( m_mutex );
    if( m_sendQueue.size() == 0 )
      m_pMonitor->unsignal( m_socket );
  }

  void onTimeout();

private:
  typedef std::deque<std::string, ALLOCATOR<std::string> >
    Queue;

  bool isValidSession();
  void readFromSocket() throw( SocketRecvFailed );
  bool readMessage( std::string& msg );
  void readMessages( SocketMonitor& s );
  bool send( const std::string& );
  void disconnect();

  int m_socket;
  char m_buffer[BUFSIZ];

  Parser m_parser;
  Queue m_sendQueue;
  unsigned m_sendLength;
  Sessions m_sessions;
  Session::SPtr m_pSession;
  SocketMonitor* m_pMonitor;
  Mutex m_mutex;
  fd_set m_fds;
};

};

#endif //FIX_SOCKETCONNECTION_H
