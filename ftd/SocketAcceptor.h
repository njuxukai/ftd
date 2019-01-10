#ifndef FTD_SOCKETACCEPTOR_H
#define FTD_SOCKETACCEPTOR_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include <common/usual_macro.h>
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
	SocketAcceptor( Application&, const PortSettings& ) throw( ConfigError );

	virtual ~SocketAcceptor();
	Session* lookupSession(int socket);
private:
	DISABLE_COPY_AND_ASSIGN(SocketAcceptor)

	bool readSettings( const PortSettings& );

	typedef std::set < SessionID > Sessions;
	typedef std::map < int, Sessions > PortToSessions;
	typedef std::map < int, SocketConnection* > SocketConnections;
	typedef std::map < int, SessionID> SocketSessionIDMap;
	typedef std::map <ClientID, Sessions> ClientIDToSessions;
	void onConfigure( const PortSettings& ) throw ( ConfigError );
	void onInitialize( const PortSettings& ) throw ( RuntimeError );

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
	ClientIDToSessions m_clientIDToSessions;
};
/*! @} */
}

#endif //FIX_SOCKETACCEPTOR_H
