
#ifndef FTD_APPLICATION_H
#define FTD_APPLICATION_H

//#include "Message.h"
#include "SessionID.h"
#include "Mutex.h"
#include "Exceptions.h"
#include "Package.h"

namespace FTD
{
	/**
	* This interface must be implemented to define what your %FIX application
	* does.
	*
	* These methods notify your application about events that happen on
	* active %FIX sessions. There is no guarantee how many threads will be calling
	* these functions. If the application is sharing resources among multiple sessions,
	* you must synchronize those resources. You can also use the SynchronizedApplication
	* class to automatically synchronize all function calls into your application.
	* The various MessageCracker classes can be used to parse the generic message
	* structure into specific %FIX messages.
	*/
	class Application
	{
	public:
		virtual ~Application() {};

		/// Notification of a session begin created
		virtual void onCreate(const SessionID&) = 0;
		/// Notification of a session successfully logging on
		virtual void onLogon(const SessionID&) = 0;
		/// Notification of a session logging off or disconnecting
		virtual void onLogout(const SessionID&) = 0;
		/// Notification of admin message being sent to target
		virtual void toAdmin(Package&, const SessionID&) = 0;
		/// Notification of app message being sent to target
		virtual void toApp(Package&, const SessionID&)
			throw(DoNotSend) = 0;
		/// Notification of admin message being received from target
		virtual void fromAdmin(const Package&, const SessionID&)
			throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, RejectLogon) = 0;
		/// Notification of app message being received from target
		virtual void fromApp(const Package&, const SessionID&)
			throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, UnsupportedMessageType) = 0;

		virtual void onHeartBeat() = 0;

		virtual void onHeartBeatWarning() = 0;
	};


	/**
	* An empty implementation of an Application. This can be used if you
	* do not want to provide an implementation for all the callback methods.
	* It is also useful for unit tests writing unit tests where the callback
	* values of some or all methods are not of interest.
	*/
	class NullApplication : public Application
	{
		void onCreate(const SessionID&) {}
		void onLogon(const SessionID&) {}
		void onLogout(const SessionID&) {}
		void toAdmin(Package&, const SessionID&) {}
		void toApp(Package&, const SessionID&)
			throw(DoNotSend) {}
		void fromAdmin(const Package&, const SessionID&)
			throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, RejectLogon) {}
		void fromApp(const Package&, const SessionID&)
			throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, UnsupportedMessageType) {}
	};
	/*! @} */
}

#endif //FTD_APPLICATION_H
