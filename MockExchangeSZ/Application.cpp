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

#include <fstream>
#include "quickfix/Session.h"
#include "quickfix/fix50/ExecutionReport.h"
#include "quickfix/fix50sp2/ExecutionReport.h"

#include <ftd/Settings.h>
#include "SZStep.h"

using boost::mt19937;
using boost::uniform_01;

void NewOrderProcessParameter::init(double new_ratio, double reject_ratio, double part_trade_ratio, double all_trade_ratio)
{
	new_order_begin = 2;
	new_order_end = 2;
	reject_order_begin = 2;
	reject_order_end = 2;
	part_trade_order_begin = 2;
	part_trade_order_end = 2;
	all_trade_order_begin = 2;
	all_trade_order_end = 2;

	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	a = 100 * new_ratio;
	b = 100 * reject_ratio;
	c = 100 * part_trade_ratio;
	d = 100 * all_trade_ratio;
	if ((a + b + c + d) == 0)
	{
		reject_order_begin = 0;
		reject_order_end = 1;
		return;
	}
	new_ratio = (1.0 * a) / (a + b + c + d);
	reject_ratio = (1.0 * b) / (a + b + c + d);
	part_trade_ratio = (1.0 * c) / (a + b + c + d);
	all_trade_ratio = (1.0 * d) / (a + b + c + d);
	new_order_begin = 0;
	new_order_end = new_ratio;
	reject_order_begin = new_order_end;
	reject_order_end = reject_order_begin + reject_ratio;
	part_trade_order_begin = reject_order_end;
	part_trade_order_end = part_trade_order_begin + part_trade_ratio;
	all_trade_order_begin = part_trade_order_end;
	all_trade_order_end = all_trade_order_begin + all_trade_ratio;
}

Application::Application(const std::string& cfgFname) : m_cfgFname(cfgFname),
m_randomGen(boost::mt19937())
{
	init();

	m_stateInfo.platformID = m_platformID;
	m_stateInfo.platformStatus = SZStep::STATUS_OPEN;

	m_info.platformID = m_platformID;
	m_info.partitionIDs.clear();
	m_info.partitionIDs.push_back(1);
	m_info.partitionIDs.push_back(2);
}

bool Application::init()
{
	using namespace FTD;
	bool parseResult = true;
	try
	{
		std::ifstream stream(m_cfgFname);
		FTD::Settings settings(false);
		stream >> settings;
		Settings::Sections section;
		Dictionary dict;

		section = settings.get("Exchange");
		if (section.size() != 1)
			return false;
		Dictionary exchangeDict = section[0];
		//m_stepCfgFname = reporterDict.getString("StepCfgFile");
		m_platformID = exchangeDict.getInt("PlatformID");

		double newRatio = abs(exchangeDict.getDouble("NewRatio"));
		double rejectRatio = abs(exchangeDict.getDouble("RejectRatio"));
		double partTradeRatio = abs(exchangeDict.getDouble("PartTradeRatio"));
		double allTradeRatio = abs(exchangeDict.getDouble("AllTradeRatio"));
		m_newOrderProcessParameter.init(newRatio, rejectRatio, partTradeRatio, allTradeRatio);
	}
	catch (...)
	{
		parseResult = false;
	}
	return parseResult;
}

void Application::onCreate( const FIX::SessionID& sessionID ) 
{}

void Application::onLogon( const FIX::SessionID& sessionID ) 
{
	FIX50SP2::Message stateInfo = generatePlatformStateInfo();
	FIX50SP2::Message info = generatePlatformInfo();
	FIX::Session::sendToTarget(stateInfo, sessionID);
	FIX::Session::sendToTarget(info, sessionID);
}

void Application::onLogout( const FIX::SessionID& sessionID ) 
{
	
}

void Application::toAdmin( FIX::Message& message,
                           const FIX::SessionID& sessionID ) 
{}

void Application::toApp( FIX::Message& message,
                         const FIX::SessionID& sessionID )
throw( FIX::DoNotSend ) 
{}


void Application::fromAdmin( const FIX::Message& message,
                             const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {}

void Application::fromApp( const FIX::Message& message,
                           const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{ 
	FIX::MsgType msgType;
	message.getHeader().getField(msgType);
	std::string strMsgType = msgType.getString();
	if (strMsgType == "U101")
	{
		onStepReportSynchronization(message, sessionID);
		return;
	}
	if (strMsgType == "D")
	{
		onStepNewOrderSingle(message, sessionID);
		return;
	}
	if (strMsgType == "F")
	{
		onStepOrderCancelRequest(message, sessionID);
		return;
	}
}


void Application::onStepNewOrderSingle(const FIX::Message& message,
	const FIX::SessionID& sessionID)
{
	FTD::CFtdcInputOrderField order = { 0 };
	bool convertResult = SZStep::FromFix::convertInputOrderField(message, order);
	if (convertResult)
	{
		double r = m_randomGen();
		if (m_newOrderProcessParameter.deal_as_new(r))
		{
			onStepNewOrderSingleNew(order, sessionID);
			return;
		}
		if (m_newOrderProcessParameter.deal_as_reject(r))
		{
			onStepNewOrderSingleReject(order, sessionID);
			return;
		}
		if (m_newOrderProcessParameter.deal_as_part_trade(r))
		{
			onStepNewOrderSinglePartTrade(order, sessionID);
			return;
		}
		if (m_newOrderProcessParameter.deal_as_all_trade(r))
		{
			onStepNewOrderSingleAllTrade(order, sessionID);
			return;
		}
	}
}


void Application::onStepReportSynchronization(const FIX::Message& message, const FIX::SessionID& sessionID)
{
	SZStep::ReportSynchronization reportSync;
	bool convertResult = SZStep::FromFix::convertReportSynchronization(message, reportSync);
	if (convertResult)
	{
	}
}


void Application::onStepOrderCancelRequest(const FIX::Message& message, const FIX::SessionID& sessionID)
{
	SZStep::ReportSynchronization reportSync;
	bool convertResult = SZStep::FromFix::convertReportSynchronization(message, reportSync);
	if (convertResult)
	{
	}
}


FIX50SP2::Message Application::generatePlatformStateInfo()
{
	FIX50SP2::Message message(FIX::MsgType("U102"));
	SZStep::ToFix::formatPlatformStateInfo(m_stateInfo, message);
	return message;
}

FIX50SP2::Message Application::generatePlatformInfo()
{
	FIX50SP2::Message message(FIX::MsgType("U104"));
	SZStep::ToFix::formatPlatformInfo(m_info, message);
	return message;
}

void Application::onStepNewOrderSingleNew(const FTD::CFtdcInputOrderField& order, const FIX::SessionID& id)
{
	FTD::CFtdcInnerExecutionReportField report = { 0 };
	formatExecutionReport(order, report);
	report.OrderStatus = FTDC_OS_NEW;
	FIX50SP2::Message message(FIX::MsgType("8"));
	SZStep::ToFix::formatInnerExecutionReport(report, message);
	FIX::Session::sendToTarget(message, id);
}

void Application::onStepNewOrderSingleReject(const FTD::CFtdcInputOrderField& order, const FIX::SessionID& id)
{
}

void Application::onStepNewOrderSinglePartTrade(const FTD::CFtdcInputOrderField& order, const FIX::SessionID& id)
{
}


void Application::onStepNewOrderSingleAllTrade(const FTD::CFtdcInputOrderField& order, const FIX::SessionID& id)
{
}

//TODO
void Application::formatExecutionReport(const FTD::CFtdcInputOrderField& order, FTD::CFtdcInnerExecutionReportField& report)
{
	report.PartitionNo = m_info.partitionIDs[0];
	report.ReportIndex = getNextReportIndex();
	report.OwnerType = order.Owner
}