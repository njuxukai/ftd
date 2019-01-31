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

Application::Application(int platformID) : m_platformID(platformID)
{
	m_stateInfo.platformID = m_platformID;
	m_stateInfo.platformStatus = SZStep::STATUS_OPEN;

	m_info.platformID = m_platformID;
	m_info.partitionIDs.clear();
	m_info.partitionIDs.push_back(1);
	m_info.partitionIDs.push_back(2);
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
}


void Application::onStepNewOrderSingle(const FIX::Message& message,
	const FIX::SessionID& sessionID)
{
	FTD::CFtdcInputOrderField order = { 0 };
	bool convertResult = SZStep::FromFix::convertInputOrderField(message, order);
	if (convertResult)
	{
		
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