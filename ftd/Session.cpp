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
  m_packageBuffer()
{
  m_state.heartBtInt( 0 );
  m_state.initiate(!receiveReq );
  m_state.store(m_packageStoreFactory.create( m_sessionID ) );
  if ( m_pLogFactory )
    m_state.log( m_pLogFactory->create( m_sessionID ) );

  m_application.onCreate( m_sessionID );
  m_state.onEvent( "Created session" );
  
}

Session::~Session()
{
	m_application.onDisconnect(m_sessionID);
	removeSession( m_sessionID );
	m_packageStoreFactory.destroy( m_state.store() );
	if ( m_pLogFactory && m_state.log() )
		m_pLogFactory->destroy( m_state.log() );
	std::cout << "~Session()\n";
}


void Session::next()
{
  next( UtcTimeStamp() );
}

void Session::next( const UtcTimeStamp& timeStamp )
{
	//Initiator & Acceptor 登录超时
	if (m_state.logonTimedOut())
	{
		m_state.onEvent("Timed out waiting for logon response");
		disconnect();
		return;
	}
	if (m_state.heartBtInt() == 0)
		return;
	if (m_state.withinHeartBeat())
		return;
	if (m_state.timedOut())
	{
		m_state.onEvent("Timed out waiting for heartbeat");
		disconnect();
		return;
	}
	if (m_state.warningTimeOut())
	{
		m_application.onHeartBeatWarning();
	}
	if (m_state.needHeartbeat())
	{
		generateHeartbeat();
	}
}

bool Session::send(Package& package)
{
	return sendRaw(package, 0);
}

bool Session::send(std::string& ftdMsg)
{
	return sendRaw(ftdMsg);
}

bool Session::send(std::vector<std::string>& ftdMsgs)
{
	return sendRaw(ftdMsgs);
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


	if (package.m_transactionId == TID_UserLogin)
	{
		onSendLogin(package);
	}
	if (package.m_transactionId == TID_UserLogout)
	{
		onSendLogout(package);
	}
	if (package.m_transactionId == TID_ForceExit)
	{
		onSendForceExit(package);
	}
	

	if (num > 0)
	{
		package.m_sequenceNO = num;
	}

	if (num == 0 && isInitiator() && package.isDialogMode() && package.isRequest())
	{
		package.m_sequenceNO = m_state.getNextSenderMsgSeqNum();
		m_state.incrNextSenderMsgSeqNum();
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
	//out message
	std::vector<std::string> ftdMsgs;
	package.formatFtdcMessages();
	package.toFtdMesssages(ftdMsgs);
	for (unsigned int i = 0; i < ftdMsgs.size(); i++)
	{
		responderSend(ftdMsgs[i]);
	}
	return true;
}

//直接发送ftdMsg 
//一般用于
// 1  1发送心跳包 
// 2 Acceptor发送消息（sequnce信息已设置ok）
bool Session::sendRaw(std::string& ftdMsg)
{
	Locker l(m_mutex);
	return responderSend(ftdMsg);
}

bool Session::sendRaw(std::vector<std::string>& ftdMsgs)
{
	Locker l(m_mutex);
	for (unsigned int i = 0; i < ftdMsgs.size(); i++)
	{
		if (!responderSend(ftdMsgs[i]))
			return false;
	}
	return true;
}

bool Session::responderSend( const std::string& string )
{
  if ( !m_pResponder ) 
	  return false;
  m_state.onOutgoing(string) ;
  bool result =  m_pResponder->send(string);
  if (result)
  {
	  UtcTimeStamp now;
	  m_state.lastSentTime(now);
  }
  return result;
}

void Session::disconnect()
{
  Locker l(m_mutex);

  if (m_pResponder)
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
	m_state.onIncoming(ftdMsg);
	m_state.lastReceivedTime(timeStamp);

	FtdHeader ftdHeader = { 0 };
	readFtdHeader(ftdMsg.c_str(), ftdHeader);
	if (ftdHeader.FTDType == FTDTypeNone)
	{
		nextHeartbeat(timeStamp);
		m_application.onHeartBeat();
		return;
	}
	Package* package = m_packageBuffer.OnFtdMessage(ftdMsg);
	if (package)
	{
		next(*package, timeStamp, queued);
	}

}

//接受到端发送的package(心跳包除外)
void Session::next( const Package& package, const UtcTimeStamp& timeStamp, bool queued )
{
	//1 合法性检查
	///Session是非登录状态忽略一切非管理信息
	if (!isLoggedOn() && !package.isNoneMode())
	{
		return;
	}
	///Acceptor方不该收到response消息
	if (isAcceptor() && package.isResponse())
	{
		return;
	}
	///Initiator方不该收到resquest消息
	if (isInitiator() && package.isRequest())
	{
		return;
	}

	

	///TODO 检查sequnceNo
	///<1> Initiator （boardcast/private,暂不实现20181217）
	///<2> Acceptor (dialog序号）
	if (isAcceptor() && package.isDialogMode())
	{
		if (package.m_sequenceNO != m_state.getNextTargetMsgSeqNum())
		{
			disconnect();
		}
		m_state.incrNextTargetMsgSeqNum();
	}
	//2 管理信息对会话状态进行管理
	if (package.m_transactionId == TID_UserLogin)
		nextLogin(package);
	if (package.m_transactionId == TID_UserLogout)
		nextLogout(package);
	if (package.m_transactionId == TID_ForceExit)
		nextForceExit(package);

	//3 回调函数
	if (package.isNoneMode())
	{
		m_application.fromAdmin(package, m_sessionID);
	}
	else
	{
		m_application.fromApp(package, m_sessionID);
	}
	
	//4 
	if( isLoggedOn() )
		next();
}

//收到的package tid为 TID_UserLogin
void Session::nextLogin(const Package& package)
{
	///客户端收到用户登录应答
	if (isInitiator() && package.isResponse())
	{
		const RspUserLogin&  rspUserLogin = (const RspUserLogin&)package;
		if (!rspUserLogin.pErrorField.get() || rspUserLogin.pErrorField->ErrorCode == 0)
		{
			m_state.heartBtInt(rspUserLogin.rspUserLoginField.HeartbeatInterval);
			m_state.receivedLogon(true);
			std::cout << "Log on received\n";
		}
	}
	///服务端收到用户登录请求
	if (isAcceptor() && package.isRequest())
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
		else
		{
			m_state.heartBtInt(rspUserLogin.rspUserLoginField.HeartbeatInterval);
		}
	}
	//std::cout << "Log on sent\n";
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
  Session::SPtr pSession = lookupSession( sessionID );
  if (!pSession)
	  return false;
  return pSession->send( message );
}

bool Session::sendToTarget(std::string& ftdMsg, const SessionID& sessionID)
throw(SessionNotFound)
{
	Session::SPtr pSession = lookupSession(sessionID);
	if (!pSession)
		return false;
	return pSession->send(ftdMsg);
}

bool Session::sendToTarget(std::vector<std::string>& ftdMsgs, const SessionID& sessionID)
{
	Session::SPtr pSession = lookupSession(sessionID);
	if (!pSession)
		return false;
	return pSession->send(ftdMsgs);
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

Session::SPtr Session::lookupSession( const SessionID& sessionID )
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

Session::SPtr Session::registerSession( const SessionID& sessionID )
{
  Locker locker( s_mutex );
  Session::SPtr pSession = lookupSession( sessionID );
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

bool Session::addSession( Session::SPtr s )
{
  Locker locker( s_mutex );
  Sessions::iterator it = s_sessions.find( s->m_sessionID );
  if ( it == s_sessions.end() )
  {
    s_sessions[ s->m_sessionID ] = s;
    s_sessionIDs.insert( s->m_sessionID );
    return true;
  }
  else
    return false;
}

void Session::removeSession( const SessionID& sessionID )
{
  Locker locker( s_mutex );
  s_sessions.erase(sessionID);
  s_sessionIDs.erase(sessionID );
  s_registered.erase( sessionID);
}
}
