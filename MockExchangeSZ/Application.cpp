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

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
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
	a = (int)(100 * new_ratio);
	b = (int)(100 * reject_ratio);
	c = (int)(100 * part_trade_ratio);
	d = (int)(100 * all_trade_ratio);
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
	m_orderID = 0;
	m_execID = 0;
	m_reportIndex = 0;
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
		std::cout << "r=" << r;
		if (m_newOrderProcessParameter.deal_as_new(r))
		{
			std::cout << "新委托到达[新建]" << std::endl;
			onStepNewOrderSingleNew(order, sessionID);
			return;
		}
		if (m_newOrderProcessParameter.deal_as_reject(r))
		{
			std::cout << "新委托到达[拒绝]" << std::endl;
			onStepNewOrderSingleReject(order, sessionID);
			return;
		}
		if (m_newOrderProcessParameter.deal_as_part_trade(r))
		{
			std::cout << "新委托到达[部成]" << std::endl;
			onStepNewOrderSinglePartTrade(order, sessionID);
			return;
		}
		if (m_newOrderProcessParameter.deal_as_all_trade(r))
		{
			std::cout << "新委托到达[全成]" << std::endl;
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
	FTD::CFtdcInputOrderActionField action = { 0 };
	bool convertResult = SZStep::FromFix::convertInputOrderActionField(message, action);
	if (convertResult)
	{
		onStepOrderAction(action, sessionID);
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
	std::string orderExchangeID = getNextOrderID();
	FTD::CFtdcInnerExecutionReportField report = { 0 };
	formatExecutionReport(order, report, orderExchangeID);
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
	std::string orderExchangeID = getNextOrderID();

	FTD::CFtdcInnerExecutionReportField report = { 0 };
	formatExecutionReport(order, report, orderExchangeID);
	report.OrderStatus = FTDC_OS_NEW;
	FIX50SP2::Message message(FIX::MsgType("8"));
	SZStep::ToFix::formatInnerExecutionReport(report, message);
	FIX::Session::sendToTarget(message, id);

	FTD::CFtdcInnerExecutionReportField report2 = { 0 };
	formatExecutionReport(order, report2, orderExchangeID);
	report2.ExecType = FTDC_ET_Trade;
	report2.VolumeLast = report2.VolumeTotalOrginal / 3;
	report2.PriceLast = report2.LimitPrice;
	report2.VolumeCum = report2.VolumeLast;
	report2.VolumeLeaves = report2.VolumeTotalOrginal - report2.VolumeCum;
	report2.OrderStatus = FTDC_OS_PART_TRADED;
	FIX50SP2::Message message2(FIX::MsgType("8"));
	SZStep::ToFix::formatInnerExecutionReport(report2, message2);
	FIX::Session::sendToTarget(message2, id);
}


void Application::onStepNewOrderSingleAllTrade(const FTD::CFtdcInputOrderField& order, const FIX::SessionID& id)
{
	

}

void Application::onStepOrderAction(const FTD::CFtdcInputOrderActionField& action, const FIX::SessionID& id)
{
	//1 撤单成功
	FTD::CFtdcInnerExecutionReportField report = { 0 };
	formatExecutionReport(action, report);
	report.PartitionNo = m_info.partitionIDs[0];
	report.ReportIndex = getNextReportIndex();

	FIX50SP2::Message message(FIX::MsgType("8"));
	SZStep::ToFix::formatInnerExecutionReport(report, message);
	FIX::Session::sendToTarget(message, id);
	//2 TODO 撤单失败
}


void Application::formatExecutionReport(const FTD::CFtdcInputOrderField& order, FTD::CFtdcInnerExecutionReportField& report,
	std::string orderID)
{
	report.PartitionNo = m_info.partitionIDs[0];
	report.ReportIndex = getNextReportIndex();
	strcpy(report.OrderRestrictions, "1");
	strcpy(report.ApplID, "010");
	strcpy(report.OrderExchangeID, orderID.data());
	if (order.InvestorID == order.UserID)
	{
		report.OwnerType = 1;
	}
	else
	{
		report.OwnerType = 102;
	}
	strcpy(report.ReportExchangeID, getNextExecID().data());
	
	if (orderID == "")
	{
		strcpy(report.OrderExchangeID, getNextOrderID().data());
	}
	else
	{
		strcpy(report.OrderExchangeID, orderID.data());
	}
	report.ExecType = FTDC_ET_New;
	report.OrderStatus = FTDC_OS_NEW;
	report.VolumeLeaves = order.VolumeTotalOrginal;
	report.VolumeCum = 0;
	report.Direction = order.Direction;
	strcpy(report.ClOrdID, order.ClOrdID);
	strcpy(report.InstrumentCode, order.InstrumentCode);
	report.ExchangeType = order.ExchangeType;
	strcpy(report.SecurityAccount, order.SecurityAccount);
	report.VolumeTotalOrginal = order.VolumeTotalOrginal;
	report.PriceType = order.PriceType;
	report.LimitPrice = order.LimitPrice;
}

void Application::formatExecutionReport(const FTD::CFtdcInputOrderActionField& action, FTD::CFtdcInnerExecutionReportField& report)
{
	report.PartitionNo = m_info.partitionIDs[0];
	report.ReportIndex = getNextReportIndex();
	strcpy(report.OrderRestrictions, "1");
	strcpy(report.ApplID, "010");
	strcpy(report.OrderExchangeID, action.OrderExchangeID);
	if (action.InvestorID == action.UserID)
	{
		report.OwnerType = 1;
	}
	else
	{
		report.OwnerType = 102;
	}
	strcpy(report.ReportExchangeID, getNextExecID().data());


	report.ExecType = FTDC_ET_Cancelled;
	report.OrderStatus = FTDC_OS_CANCELLED;
	report.VolumeLeaves = 0;
	report.VolumeCum = 0;
	report.Direction = action.Direction;
	strcpy(report.ClOrdID, action.ClOrdID);
	strcpy(report.InstrumentCode, action.InstrumentCode);
	report.ExchangeType = action.ExchangeType;
	strcpy(report.SecurityAccount, action.SecurityAccount);
	report.VolumeTotalOrginal = action.VolumeTotalOrginal;
	report.PriceType = FTDC_OPT_HS_Limit;
	report.LimitPrice = 0;
}

int Application::getNextReportIndex()
{
	return ++m_reportIndex;
}

std::string Application::getNextExecID() 
{
	m_execID++;
	return (boost::format("%016d") % m_execID).str();
}

std::string Application::getNextOrderID()
{
	m_orderID++;
	return (boost::format("%016d") % m_orderID).str();
}