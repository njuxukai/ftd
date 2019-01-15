

#ifndef FTD_SESSIONFACTORY_H
#define FTD_SESSIONFACTORY_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Log.h"
#include "Exceptions.h"
#include "Dictionary.h"
#include "SessionID.h"
#include "session.h"

namespace FTD
{
class Session;
class Application;
class PackageStoreFactory;
class DataDictionaryProvider;

/** Responsible for creating Session objects.  This factory will use
 *  QuickFIX SessionID, Dictionary settings, MessageStoreFactory, and
 *  optional LogFactory to create all the required sessions for an
 *  Application.
 */
class SessionFactory
{
public:
  SessionFactory( Application& application,
	  PackageStoreFactory& packageStoreFactory,
                  LogFactory* pLogFactory )
: m_application( application ),
  m_packageStoreFactory(packageStoreFactory),
  m_pLogFactory( pLogFactory ) {}

  ~SessionFactory();

  Session::SPtr create( const SessionID& sessionID,
	               const Dictionary& settings,
	  bool receiveReq) throw( ConfigError );
  void destroy( Session::SPtr pSession );

private:
  Application& m_application;
  PackageStoreFactory& m_packageStoreFactory;
  LogFactory* m_pLogFactory;
};
}

#endif
