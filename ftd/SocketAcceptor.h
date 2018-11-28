#ifndef FTD_SOCKETACCEPTOR_H
#define FTD_SOCKETACCEPTOR_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Acceptor.h"
#include "SocketServer.h"
#include "SocketConnection.h"

namespace FTD
{
/// Socket implementation of Acceptor.
class SocketAcceptor : public Acceptor, SocketServer::Strategy
{
  friend class SocketConnection;
public:
  SocketAcceptor( Application&, PackageStoreFactory&,
                  const SessionSettings& ) throw( ConfigError );
  SocketAcceptor( Application&, PackageStoreFactory&,
                  const SessionSettings&, LogFactory& ) throw( ConfigError );

  virtual ~SocketAcceptor();

private:
  bool readSettings( const SessionSettings& );

  typedef std::set < SessionID > Sessions;
  typedef std::map < int, Sessions > PortToSessions;
  typedef std::map < int, SocketConnection* > SocketConnections;

  void onConfigure( const SessionSettings& ) throw ( ConfigError );
  void onInitialize( const SessionSettings& ) throw ( RuntimeError );

  void onStart();
  bool onPoll( double timeout );
  void onStop();

  void onConnect( SocketServer&, int, int );
  void onWrite( SocketServer&, int );
  bool onData( SocketServer&, int );
  void onDisconnect( SocketServer&, int );
  void onError( SocketServer& );
  void onTimeout( SocketServer& );

  SocketServer* m_pServer;
  PortToSessions m_portToSessions;
  SocketConnections m_connections;
};
/*! @} */
}

#endif //FIX_SOCKETACCEPTOR_H
