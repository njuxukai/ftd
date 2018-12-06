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

#ifndef FIX_ACCEPTOR_H
#define FIX_ACCEPTOR_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Application.h"
#include "PackageStore.h"
#include "Log.h"
#include "Responder.h"
#include "PortSettings.h"
#include "Exceptions.h"
#include "SessionFactory.h"
#include <map>
#include <string>

namespace FTD
{
	class Client;
	class Session;

	/**
	* Base for classes which act as an acceptor for incoming connections.
	*
	* Most users will not need to implement one of these.  The default
	* SocketAcceptor implementation will be used in most cases.
	*/
	class Acceptor
	{
	public:
		Acceptor(Application&, PackageStoreFactory&,
			const PortSettings&) throw(ConfigError);
		Acceptor(Application&, PackageStoreFactory&,
			const PortSettings&, LogFactory&) throw(ConfigError);

		virtual ~Acceptor();

		Log* getLog()
		{
			if (m_pLog) return m_pLog;
			return &m_nullLog;
		}

		/// Start acceptor.
		void start() throw (ConfigError, RuntimeError);
		/// Block on the acceptor
		void block() throw (ConfigError, RuntimeError);
		/// Poll the acceptor
		bool poll(double timeout = 0.0) throw (ConfigError, RuntimeError);

		/// Stop acceptor.
		void stop(bool force = false);

		/// Check to see if any sessions are currently logged on
		bool isLoggedOn();

		Session* getSession(const Package& msg, Responder&);

		const std::set<SessionID>& getSessions() const { return m_sessionIDs; }
		Session* getSession(const SessionID& sessionID) const;
		const Dictionary* const getPortSettings(const int& port) const;

		bool has(const SessionID& id)
		{
			return m_sessions.find(id) != m_sessions.end();
		}

		bool isStopped() { return m_stop; }

		Application& getApplication() { return m_application; }
		PackageStoreFactory& getPackageStoreFactory()
		{
			return m_packageStoreFactory;
		}
	protected:
		///fix:initialize create all sessions ftd:a new socket accepted,new session created
		Session* createSession(const SessionID& id, const Dictionary& settings);
		void destroySession(Session* pSession);
	private:
		void initialize() throw (ConfigError);
		/// Implemented to configure acceptor
		virtual void onConfigure(const PortSettings&) throw (ConfigError) {};
		/// Implemented to initialize acceptor
		virtual void onInitialize(const PortSettings&) throw (RuntimeError) {};
		/// Implemented to start listening for connections.
		virtual void onStart() = 0;
		/// Implemented to connect and poll for events.
		virtual bool onPoll(double second) = 0;
		/// Implemented to stop a running acceptor.
		virtual void onStop() = 0;

		static THREAD_PROC startThread(void* p);

		typedef std::set < SessionID > SessionIDs;
		typedef std::map < SessionID, Session* > Sessions;

		thread_id m_threadid;
		Sessions m_sessions;
		SessionIDs m_sessionIDs;
		Application& m_application;
		PackageStoreFactory& m_packageStoreFactory;
	protected:
		PortSettings m_portSettings;
		SessionFactory* m_pSessionFactory;
		std::map<int, Dictionary> m_setting;
	private:
		LogFactory* m_pLogFactory;
		Log* m_pLog;
		NullLog m_nullLog;
		bool m_firstPoll;
		bool m_stop;
		
	};
	/*! @} */
}

#endif // FIX_ACCEPTOR_H
