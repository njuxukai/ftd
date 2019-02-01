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

#ifndef EXECUTOR_APPLICATION_H
#define EXECUTOR_APPLICATION_H

#include <boost/random.hpp>

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"


#include <quickfix/fix50sp2/NewOrderSingle.h>
#include "SZStep.h"

struct NewOrderProcessParameter
{
	double new_order_begin;
	double new_order_end;
	double reject_order_begin;
	double reject_order_end;
	double part_trade_order_begin;
	double part_trade_order_end;
	double all_trade_order_begin;
	double all_trade_order_end;

	void init(double new_ratio, double reject_ratio, double part_trade_ratio, double all_trade_ratio);

	bool deal_as_new(double r) 
	{ 
		return r >= new_order_begin && r <= new_order_end; 
	}

	bool deal_as_reject(double r) 
	{ 
		return r >= reject_order_begin && r <= reject_order_end; 
	}

	bool deal_as_part_trade(double r)
	{
		return r >= part_trade_order_begin && r <= part_trade_order_end;
	}

	bool deal_as_all_trade(double r)
	{
		return r >= all_trade_order_begin and r <= all_trade_order_end;
	}
};

class Application
: public FIX::Application, public FIX::MessageCracker
{
public:
	Application(const std::string& cfgFname);

  // Application overloads
  void onCreate( const FIX::SessionID& );
  void onLogon( const FIX::SessionID& sessionID );
  void onLogout( const FIX::SessionID& sessionID );
  void toAdmin( FIX::Message&, const FIX::SessionID& );
  void toApp( FIX::Message&, const FIX::SessionID& )
    throw( FIX::DoNotSend );
  void fromAdmin( const FIX::Message&, const FIX::SessionID& )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon );
  void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );


  std::string genOrderID() {
    std::stringstream stream;
    stream << ++m_orderID;
    return stream.str();
  }
  std::string genExecID() {
    std::stringstream stream;
    stream << ++m_execID;
    return stream.str();
  }
private:
  int m_orderID, m_execID;
  int m_platformID;
  SZStep::PlatformStateInfo m_stateInfo;
  SZStep::PlatformInfo m_info;

private:
	FIX50SP2::Message generatePlatformStateInfo();
	FIX50SP2::Message generatePlatformInfo();

private:
	void onStepNewOrderSingle(const FIX::Message& message, const FIX::SessionID& id);
	void onStepNewOrderSingleNew(const FTD::CFtdcInputOrderField order, const FIX::SessionID& id);
	void onStepNewOrderSingleReject(FTD::CFtdcInputOrderField order, const FIX::SessionID& id);
	void onStepNewOrderSinglePartTrade(FTD::CFtdcInputOrderField order, const FIX::SessionID& id);
	void onStepNewOrderSingleAllTrade(FTD::CFtdcInputOrderField order, const FIX::SessionID& id);
	void onStepReportSynchronization(const FIX::Message& message, const FIX::SessionID& id);
	void onStepOrderCancelRequest(const FIX::Message& message, const FIX::SessionID& id);

private:
	boost::uniform_01<boost::mt19937> m_randomGen;
	std::string m_cfgFname;
	bool init();
	NewOrderProcessParameter m_newOrderProcessParameter;
};

#endif
