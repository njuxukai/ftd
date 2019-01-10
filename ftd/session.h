
#ifndef FTD_SESSION_H
#define FTD_SESSION_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "SessionState.h"
#include "SessionID.h"
#include "Responder.h"
#include "Application.h"
#include "Mutex.h"
#include "Log.h"
#include <utility>
#include <map>
#include <queue>
#include <set>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include "ftd30/Packages.h"
namespace FTD
{
/// Maintains the state and implements the logic of a %FIX %session.
class Session
{
public:
  Session( Application&, PackageStoreFactory&,
           const SessionID&,
           LogFactory* pLogFactory,
	       bool receiveReq);
  virtual ~Session();

  void logon() 
  { m_state.enabled( true ); m_state.logoutReason( "" ); }
  void logout( const std::string& reason = "" ) 
  { m_state.enabled( false ); m_state.logoutReason( reason ); }
  bool isEnabled() 
  { return m_state.enabled(); }

  bool sentLogon() { return m_state.sentLogon(); }
  bool sentLogout() { return m_state.sentLogout(); }
  bool receivedLogon() { return m_state.receivedLogon(); }
  bool isLoggedOn() { return receivedLogon() && sentLogon(); }
  void reset() throw( IOException ) 
  { 
	  ///generateLogout(); disconnect(); m_state.reset(); 
  }
  void refresh() throw( IOException )
  { m_state.refresh(); }
  void setNextSenderMsgSeqNum( int num ) throw( IOException )
  { m_state.setNextSenderMsgSeqNum( num ); }
  void setNextTargetMsgSeqNum( int num ) throw( IOException )
  { m_state.setNextTargetMsgSeqNum( num ); }

  const int& getSessionID() const
  { return m_sessionID; }

  static bool allocateNextSessionID(SessionID& sessionID, std::string& randomID);



  static bool sendToTarget( Package& message, const SessionID& sessionID )
  throw( SessionNotFound );

  static bool sendToTarget(std::string& ftdMsg, const SessionID& sessionID)
  throw( SessionNotFound );


  static std::set<SessionID> getSessions();
  static bool doesSessionExist( const SessionID& );
  static Session* lookupSession( const SessionID& );
  static bool isSessionRegistered( const SessionID& );
  static Session* registerSession( const SessionID& );
  static void unregisterSession( const SessionID& );

  static size_t numSessions();



  bool isInitiator()
    { return m_state.initiate(); }

  bool isAcceptor()
    { return !m_state.initiate(); }


  int getLogonTimeout()
    { return m_state.logonTimeout(); }
  void setLogonTimeout ( int value )
    { m_state.logonTimeout( value ); }

  int getLogoutTimeout()
    { return m_state.logoutTimeout(); }
  void setLogoutTimeout ( int value )
    { m_state.logoutTimeout( value ); }

  void setResponder( Responder* pR )
  {
    m_pResponder = pR;
  }

  bool send( Package& );
  bool send(std::string&);
  bool send(std::vector<std::string>&);
  
  void nextHeartbeat(const UtcTimeStamp& timestamp);
  void next();
  void next( const UtcTimeStamp& timeStamp );
  void next( const std::string&, const UtcTimeStamp& timeStamp, bool queued = false );
  void next( const Package&, const UtcTimeStamp& timeStamp, bool queued = false );
  void nextLogin(const Package&);
  void nextLogout(const Package&);
  void nextForceExit(const Package&);

  void onSendLogin(Package&);
  void onSendLogout(Package&);
  void onSendForceExit(Package&);

  void disconnect();



  Log* getLog() { return &m_state; }
  const PackageStore* getStore() { return &m_state; }

private:
  typedef std::map < SessionID, Session* > Sessions;
  typedef std::set < SessionID > SessionIDs;

  static bool addSession( Session& );
  static void removeSession( Session& );

  //synced atomic operation
  bool sendRaw(std::string& ftdMsg);
  bool sendRaw(Package& package, int num);
  bool sendRaw(std::vector<std::string>& ftdMsgs);
  //
  bool responderSend(const std::string & string);
  
  void generateHeartbeat()
  {
	  sendRaw(FtdMessageUtil::generateHeartbeatMessage());
  }



  Application& m_application;
  SessionID m_sessionID;



  SessionState m_state;
  PackageStoreFactory& m_packageStoreFactory;
  LogFactory* m_pLogFactory;
  Responder* m_pResponder;
  Mutex m_mutex;
  PackageBuffer m_packageBuffer;
  static Sessions s_sessions;
  //s_sessionIDs 全局静态sessionID池，并不保证所有id都有alive的session，应轮询清理
  static SessionIDs s_sessionIDs;
  static Sessions s_registered;
  static Mutex s_mutex;
  static std::hash<std::string> s_strHash;
  static boost::uuids::random_generator s_uuidGenerator;
};
}

#endif //FIX_SESSION_H
