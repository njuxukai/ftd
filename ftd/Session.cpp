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

#include "Session.h"
#include "Values.h"
#include "ID.h"
#include <algorithm>
#include <iostream>

namespace FTD
{
Session::Sessions Session::s_sessions;
Session::SessionIDs Session::s_sessionIDs;
Session::Sessions Session::s_registered;
Mutex Session::s_mutex;
std::hash<std::string> Session::s_strHash;
boost::uuids::random_generator Session::s_uuidGenerator;

#define LOGEX( method ) try { method; } catch( std::exception& e ) \
  { m_state.onEvent( e.what() ); }

Session::Session( Application& application,
                  PackageStoreFactory& packageStoreFactory,
                  const SessionID& sessionID,
	              LogFactory* pLogFactory,
	bool receiveReq)
: m_application( application ),
  m_packageStoreFactory(packageStoreFactory),
  m_sessionID( sessionID ),
  m_pLogFactory( pLogFactory ),
  m_pResponder( 0 ),
  m_packageBuffer(receiveReq)
{
  m_state.heartBtInt( 0 );
  m_state.initiate(!receiveReq );
  m_state.store(m_packageStoreFactory.create( m_sessionID ) );
  if ( m_pLogFactory )
    m_state.log( m_pLogFactory->create( m_sessionID ) );



  addSession( *this );
  m_application.onCreate( m_sessionID );
  m_state.onEvent( "Created session" );
}

Session::~Session()
{
  removeSession( *this );
  m_packageStoreFactory.destroy( m_state.store() );
  if ( m_pLogFactory && m_state.log() )
    m_pLogFactory->destroy( m_state.log() );
}


void Session::next()
{
  next( UtcTimeStamp() );
}

void Session::next( const UtcTimeStamp& timeStamp )
{

}

bool Session::send(Package& package)
{
	return sendRaw(package, 0);
}


bool Session::sendRaw(Package& package, int num)
{
	/*
	*1 none 管理数据 series=0 sequence=0
	*2 dialog 
	*  2.1 req  series=0 在这里给定序号
	*  2.2 rsp  series=0 同源req,函数外
	*3 private  series=(客户端唯一，用userid?) 
				sequence=对于客户端唯一
	*3 boardcast series>=1 
				sequence=对于客户端唯一
	*/
	Locker l(m_mutex);
	if (num > 0)
	{
		package.m_header.sequenceNO = num;
	}
	if (package.isNoneMode())
	{
		m_application.toAdmin(package, m_sessionID);
	}
	else
	{ 
		if (!isLoggedOn())
			return false;
		m_application.toApp(package, m_sessionID);
	}
	//app message
	std::vector<std::string> ftdMsgs;
	package.toFtdMesssages(ftdMsgs);
	for (int i = 0; i < ftdMsgs.size(); i++)
	{
		send(ftdMsgs[i]);
	}
	return true;
}

bool Session::send( const std::string& string )
{
  if ( !m_pResponder ) return false;
  m_state.onOutgoing( string );
  return m_pResponder->send( string );
}

void Session::disconnect()
{
  Locker l(m_mutex);

  if ( m_pResponder )
  {
    m_state.onEvent( "Disconnecting" );

    m_pResponder->disconnect();
    m_pResponder = 0;
  }

  if ( m_state.receivedLogon() || m_state.sentLogon() )
  {
    m_state.receivedLogon( false );
    m_state.sentLogon( false );
    m_application.onLogout( m_sessionID );
  }

  m_state.sentLogout( false );
  m_state.receivedReset( false );
  m_state.sentReset( false );
  m_state.clearQueue();
  m_state.logoutReason();
  //if ( m_resetOnDisconnect )
    m_state.reset();

  m_state.resendRange( 0, 0 );
}

bool Session::allocateNextSessionID(SessionID& id, std::string& randomString)
{
	Locker locker(s_mutex);
	int tryCount = 0;
	while (tryCount < 10)
	{
		randomString = boost::uuids::to_string(s_uuidGenerator());
		id = s_strHash(randomString);
		if (s_sessionIDs.find(id) == s_sessionIDs.end())
		{
			s_sessionIDs.insert(id);
			return true;
		}
		tryCount += 1;
	}
	return false;
}

///从对端收到心跳包
void Session::nextHeartbeat(const UtcTimeStamp& timestamp)
{}

void Session::next( const std::string& ftdMsg, const UtcTimeStamp& timeStamp, bool queued )
{
	FtdHeader ftdHeader = { 0 };
	readFtdHeader(ftdMsg.c_str(), ftdHeader);
	if (ftdHeader.FTDType == FTDTypeNone)
	{
		nextHeartbeat(timeStamp);
		return;
	}
	Package* package = m_packageBuffer.OnFtdMessage(ftdMsg);
	if (package)
	{
		next(*package, timeStamp, queued);
	}

}

//接受到对方
void Session::next( const Package& package, const UtcTimeStamp& timeStamp, bool queued )
{
	int tid = package.m_header.transactionId;
	if (!isLoggedOn() && !package.isNoneMode())
	{
		return;
	}
	/*
	InvalidMessageType
	UnsupportedMessageType
	InvalidMessage
	*/

	//from callback
	if (package.isNoneMode())
	{
		m_application.fromAdmin(package, m_sessionID);
	}
	else
	{
		m_application.fromApp(package, m_sessionID);
	}
	//admin types
	if (package.m_transactionId == TID_UserLogin)
		nextLogin(package);
	if (package.m_transactionId == TID_UserLogout)
		nextLogout(package);
	if (package.m_transactionId == TID_ForceExit)
		nextForceExit(package);

	if( isLoggedOn() )
		next();
}

void Session::nextLogin(const Package& package)
{
	//客户端收到用户登录应答
	//服务端收到用户登录请求
	//initiator received login deny notification
	if (isInitiator())
	{
		const RspUserLogin&  rspUserLogin = (const RspUserLogin&)package;
		if (rspUserLogin.pErrorField.get() && rspUserLogin.pErrorField->ErrorCode != 0)
		{
			return;
		}
		else
		{
			//m_state.heartBtInt(rspUserLogin.rspUserLoginField.heartber);
			m_state.heartBtInt(rspUserLogin.rspUserLoginField.HeartbeatInterval);
			m_state.receivedLogon(true);
			if(isLoggedOn())
				m_application.onLogon(m_sessionID);
		}
	}
	else
	{
		m_state.receivedLogon(true);
	}
}

void Session::nextLogout(const Package& package)
{
	//客户端收到用户登出应答
	//服务端收到用户登出请求
}

void Session::nextForceExit(const Package& package)
{
	//客户端收到强制退出通知
	if (isInitiator())
	{
	}
}

void Session::onSendLogin(Package& package)
{
	//acceptor going to send login deny notification
	if (isAcceptor())
	{
		const RspUserLogin& rspUserLogin = (const RspUserLogin&)package;
		if (rspUserLogin.pErrorField.get() && rspUserLogin.pErrorField->ErrorCode != 0)
		{
			return;
		}
	}
	m_state.sentLogon(true);
}

void Session::onSendLogout(Package& package)
{
	m_state.sentLogout(true);
}

void Session::onSendForceExit(Package& package)
{}

bool Session::sendToTarget( Package& message, const SessionID& sessionID )
throw( SessionNotFound )
{
  Session* pSession = lookupSession( sessionID );
  if ( !pSession ) throw SessionNotFound();
  return pSession->send( message );
}



std::set<SessionID> Session::getSessions()
{
  return s_sessionIDs;
}

bool Session::doesSessionExist( const SessionID& sessionID )
{
  Locker locker( s_mutex );
  return s_sessions.end() != s_sessions.find( sessionID );
}

Session* Session::lookupSession( const SessionID& sessionID )
{
  Locker locker( s_mutex );
  Sessions::iterator find = s_sessions.find( sessionID );
  if ( find != s_sessions.end() )
    return find->second;
  else
    return 0;
}


bool Session::isSessionRegistered( const SessionID& sessionID )
{
  Locker locker( s_mutex );
  return s_registered.end() != s_registered.find( sessionID );
}

Session* Session::registerSession( const SessionID& sessionID )
{
  Locker locker( s_mutex );
  Session* pSession = lookupSession( sessionID );
  if ( pSession == 0 ) return 0;
  if ( isSessionRegistered( sessionID ) ) return 0;
  s_registered[ sessionID ] = pSession;
  return pSession;
}

void Session::unregisterSession( const SessionID& sessionID )
{
  Locker locker( s_mutex );
  s_registered.erase( sessionID );
}

size_t Session::numSessions()
{
  Locker locker( s_mutex );
  return s_sessions.size();
}

bool Session::addSession( Session& s )
{
  Locker locker( s_mutex );
  Sessions::iterator it = s_sessions.find( s.m_sessionID );
  if ( it == s_sessions.end() )
  {
    s_sessions[ s.m_sessionID ] = &s;
    s_sessionIDs.insert( s.m_sessionID );
    return true;
  }
  else
    return false;
}

void Session::removeSession( Session& s )
{
  Locker locker( s_mutex );
  s_sessions.erase( s.m_sessionID );
  s_sessionIDs.erase( s.m_sessionID );
  s_registered.erase( s.m_sessionID );
}
}
