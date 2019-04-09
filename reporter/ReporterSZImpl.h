#include "ReporterWrapper.h"

#include "quickfix/Application.h"
#include "quickfix/Initiator.h"
#include "SZStep.h"
#include <atomic>

class ReporterSZSTEPImpl : 
	public ReporterWrapper, 
	public FIX::Application
{
public:
	ReporterSZSTEPImpl(const std::string& cfgFname);
	virtual ~ReporterSZSTEPImpl();
	virtual void submit(FTD::PackageSPtr pPackage);
	virtual void registerUplinkCallback(const ReporterUplinkCallback& function);
	virtual void uplink(FTD::PackageSPtr pPackage);
	virtual void start();
	virtual void stop();

private:
	void onCreate(const FIX::SessionID&);
	void onLogon(const FIX::SessionID& sessionID);
	void onLogout(const FIX::SessionID& sessionID);
	void toAdmin(FIX::Message&, const FIX::SessionID&);
	void toApp(FIX::Message&, const FIX::SessionID&)
		throw(FIX::DoNotSend);
	void fromAdmin(const FIX::Message&, const FIX::SessionID&)
		throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon);
	void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)
		throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType);

private:
	void doSubmit(FTD::PackageSPtr pPackage);
private:
	bool parseCfgFname();
private:
	std::atomic<bool> m_stepLogged;
	FIX::SessionID m_loggedSessionID;

	std::string m_reporterCfgFname;
	std::string m_stepCfgFname;
	FIX::Initiator *m_pInitiator;

	std::string m_defaultCstmApplVerID;
	ReporterUplinkCallback m_uplinkCallback;
private:
	std::map<int, int> m_partitionIndexMap;
	SZStep::PlatformStateInfo m_stateInfo;
	SZStep::PlatformInfo m_info;
private:
	void onStepPlatformStateInfo(const FIX::Message& message, const FIX::SessionID& sessionID);
	void onStepPlatformInfo(const FIX::Message& message, const FIX::SessionID& sessionID);
	void onStepReportFinished(const FIX::Message& message, const FIX::SessionID& sessionID);
	void onStepExecutionReport(const FIX::Message& message, const FIX::SessionID& sessionID);
	void onStepCancelReject(const FIX::Message& message, const FIX::SessionID& sessionID);
	void onStepBusinessReject(const FIX::Message& message, const FIX::SessionID& sessionID);
};