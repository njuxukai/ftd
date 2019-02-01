#include "ReporterSZImpl.h"

#include <iostream> 
#include <boost/lexical_cast.hpp>
#include "ftd/Settings.h"
#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/fix50sp2/NewOrderSingle.h"
#include "SzStep.h"


ReporterSZSTEPImpl::ReporterSZSTEPImpl(const std::string& cfgFname)
	: ReporterWrapper()
{
	//TODO
	m_reporterCfgFname = cfgFname;
	m_pInitiator = 0;

	m_stepLogged = false;
}


ReporterSZSTEPImpl::~ReporterSZSTEPImpl()
{

}

void ReporterSZSTEPImpl::submit(FTD::PackageSPtr pPackage)
{
	std::cout << pPackage->m_transactionId << std::endl;
	if (!m_stepLogged)
		return;
	doSubmit(pPackage);
}

void ReporterSZSTEPImpl::doSubmit(FTD::PackageSPtr pPackage)
{
	
	if (pPackage->m_transactionId == TID_RptOrderInsert && pPackage->isRequest())
	{
		FIX50SP2::Message newOrder(FIX::MsgType("D"));
		SZStep::ToFix::formatInputOrderField(((FTD::ReqRptOrderInsert*)pPackage.get())->inputOrderField, newOrder);
		FIX::Session::sendToTarget(newOrder, m_loggedSessionID);
	}
	
}

void ReporterSZSTEPImpl::registerUplinkCallback(const ReporterUplinkCallback& function)
{}


void ReporterSZSTEPImpl::uplink(FTD::PackageSPtr pPackage)
{}


void ReporterSZSTEPImpl::start()
{
	bool parseResult = parseCfgFname();
	if (parseResult)
	{
		FIX::SessionSettings settings(m_stepCfgFname);
		FIX::MemoryStoreFactory storeFactory;
		//FIX::FileStoreFactory storeFactory(settings);
		FIX::FileLogFactory logFactory(settings);
		//::ScreenLogFactory logFactory(settings);
		m_pInitiator = new FIX::SocketInitiator(*this, storeFactory, settings, logFactory);
		m_pInitiator->start();
	}
	
}


void ReporterSZSTEPImpl::stop()
{
	if (m_pInitiator)
	{
		m_pInitiator->stop();
		delete m_pInitiator;
		m_pInitiator = 0;
	}
}

void ReporterSZSTEPImpl::onCreate(const FIX::SessionID&)
{}

void ReporterSZSTEPImpl::onLogon(const FIX::SessionID& sessionID)
{
	m_loggedSessionID = sessionID;
	m_stepLogged = true;
}

void ReporterSZSTEPImpl::onLogout(const FIX::SessionID& sessionID)
{
	m_stepLogged = false;
}

void ReporterSZSTEPImpl::toAdmin(FIX::Message& message, const FIX::SessionID& sessionID)
{
	FIX::MsgType msgType;
	message.getHeader().getField(msgType);
	if (msgType.getString() == "A")
	{
		message.setField(FIX::DefaultCstmApplVerID(m_defaultCstmApplVerID));
	}
}

void ReporterSZSTEPImpl::toApp(FIX::Message&, const FIX::SessionID&)
throw(FIX::DoNotSend)
{}

void ReporterSZSTEPImpl::fromAdmin(const FIX::Message&, const FIX::SessionID&)
throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon)
{}

void ReporterSZSTEPImpl::fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)
throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType)
{
	FIX::MsgType msgType;
	message.getHeader().getField(msgType);
	std::string strMsgType = msgType.getString();
	//PlatformStateInfo
	if (strMsgType == "U102")
	{
		onStepPlatformStateInfo(message, sessionID);
		return;
	}
	//ReportFinished TODO
	if (strMsgType == "U103")
	{
		onStepReportFinished(message, sessionID);
		return;
	}
	//PlatformInfo
	if (strMsgType == "U104")
	{
		onStepPlatformInfo(message, sessionID);
		return;
	}
	//ExecutionReport
	if (strMsgType == "8")
	{
		onStepExecutionReport(message, sessionID);
		return;
	}
	//CancelReject
	if (strMsgType == "9")
	{
		onStepCancelReject(message, sessionID);
		return;
	}
	if (strMsgType == "j")
	{
		onStepBusinessReject(message, sessionID);
		return;
	}
}


bool ReporterSZSTEPImpl::parseCfgFname()
{
	using namespace FTD;
	bool parseResult = true;
	try
	{
		std::ifstream stream(m_reporterCfgFname);
		FTD::Settings settings(false);
		stream >> settings;
		Settings::Sections section;
		Dictionary dict;

		section = settings.get("REPORTER");
		if (section.size() != 1)
			return false;
		Dictionary reporterDict = section[0];
		m_stepCfgFname = reporterDict.getString("StepCfgFile");
		m_defaultCstmApplVerID = reporterDict.getString("DefaultCstmApplVerID");
	}
	catch (...)
	{
		parseResult = false;
	}
	return parseResult;
}

void ReporterSZSTEPImpl::onStepPlatformStateInfo(const FIX::Message& message, const FIX::SessionID& sessionID)
{
	bool convertResult = SZStep::FromFix::convertPlatformStateInfo(message, m_stateInfo);
}

void ReporterSZSTEPImpl::onStepPlatformInfo(const FIX::Message& message, const FIX::SessionID& sessionID)
{
	using namespace SZStep;
	bool convertResult = SZStep::FromFix::convertPlatformInfo(message, m_info);
	if (convertResult)
	{
		SZStep::ReportSynchronization reportSync;
		for (int i = 0; i < m_info.partitionIDs.size(); i++)
		{
			int parID = m_info.partitionIDs[i];
			if (m_partitionIndexMap.find(parID) == m_partitionIndexMap.end())
			{
				reportSync.partitionNoReportIndexMap[parID] = 1;
			}
			else
			{
				reportSync.partitionNoReportIndexMap[parID] = m_partitionIndexMap[parID] + 1;
			}
		}
		FIX50SP2::Message syncMsg(FIX::MsgType("U101"));
		SZStep::ToFix::formatReportSynchronization(reportSync, syncMsg);
		FIX::Session::sendToTarget(syncMsg, sessionID);
	}
	else
	{
		//TODO : log fatal error
	}
}

void ReporterSZSTEPImpl::onStepReportFinished(const FIX::Message& message, const FIX::SessionID& sessionID)
{}

void ReporterSZSTEPImpl::onStepExecutionReport(const FIX::Message& message, const FIX::SessionID& sessionID)
{}

void ReporterSZSTEPImpl::onStepCancelReject(const FIX::Message& message, const FIX::SessionID& sessionID)
{}

void ReporterSZSTEPImpl::onStepBusinessReject(const FIX::Message& message, const FIX::SessionID& sessionID)
{}

	
