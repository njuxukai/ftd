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
#pragma warning( disable : 4503 4355 4786 )
#else
#include "config.h"
#endif

#include "Application.h"
#include "quickfix/Session.h"

#include "quickfix/fix50/ExecutionReport.h"
#include "quickfix/fix50sp2/ExecutionReport.h"

#include "SZStep.h"

void Application::onCreate( const FIX::SessionID& sessionID ) {}
void Application::onLogon( const FIX::SessionID& sessionID ) {}
void Application::onLogout( const FIX::SessionID& sessionID ) {}
void Application::toAdmin( FIX::Message& message,
                           const FIX::SessionID& sessionID ) {}
void Application::toApp( FIX::Message& message,
                         const FIX::SessionID& sessionID )
throw( FIX::DoNotSend ) {}

void Application::fromAdmin( const FIX::Message& message,
                             const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {}

void Application::fromApp( const FIX::Message& message,
                           const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{ crack( message, sessionID ); }




void Application::onMessage( const FIX50::NewOrderSingle& message,
                             const FIX::SessionID& sessionID )
{
  FIX::Symbol symbol;
  FIX::Side side;
  FIX::OrdType ordType;
  FIX::OrderQty orderQty;
  FIX::Price price;
  FIX::ClOrdID clOrdID;
  FIX::Account account;

  message.get( ordType );

  if ( ordType != FIX::OrdType_LIMIT )
    throw FIX::IncorrectTagValue( ordType.getField() );

  message.get( symbol );
  message.get( side );
  message.get( orderQty );
  message.get( price );
  message.get( clOrdID );

  FIX50::ExecutionReport executionReport = FIX50::ExecutionReport
      ( FIX::OrderID( genOrderID() ),
        FIX::ExecID( genExecID() ),
        FIX::ExecType( FIX::ExecType_TRADE ),
        FIX::OrdStatus( FIX::OrdStatus_FILLED ),
        side,
        FIX::LeavesQty( 0 ),
        FIX::CumQty( orderQty ) );
  
  executionReport.set( clOrdID );
  executionReport.set( symbol );
  executionReport.set( orderQty );
  executionReport.set( FIX::LastQty( orderQty ) );
  executionReport.set( FIX::LastPx( price ) );
  executionReport.set( FIX::AvgPx( price ) );

  if( message.isSet(account) )
    executionReport.setField( message.get(account) );

  try
  {
    FIX::Session::sendToTarget( executionReport, sessionID );
  }
  catch ( FIX::SessionNotFound& ) {}
}


void Application::onMessage(const FIX50SP2::NewOrderSingle& message,
	const FIX::SessionID& sessionID)
{
	FTD::CFtdcInputOrderField order = { 0 };
	bool convertResult = SZStep::FromFix::convertInputOrderField(message, order);


}
