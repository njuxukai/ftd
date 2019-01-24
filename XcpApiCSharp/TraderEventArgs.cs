using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Xcp
{
    public class FrontDisconnectedEventArgs : EventArgs
    {
        public FrontDisconnectedEventArgs(int reason)
        {
            m_reason = reason;
        }
        private readonly int m_reason;
        public int Reason { get { return m_reason; } }
    }


    public class HeartBeatWarningEventArgs : EventArgs
    {
        public HeartBeatWarningEventArgs(int timeLapse)
        {
            m_timeLapse = timeLapse;
        }
        private readonly int m_timeLapse;
        public int TimeLapse { get { return m_timeLapse; } }
    }

    public class RspUserLoginEventArgs : EventArgs
    {
        public RspUserLoginEventArgs(RspUserLoginField rspUserLoginField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_rspUserLoginField = rspUserLoginField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public RspUserLoginField RspUserLoginField{get{ return m_rspUserLoginField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly RspUserLoginField m_rspUserLoginField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }


    public class RspUserLogoutEventArgs : EventArgs
    {
        public RspUserLogoutEventArgs(RspUserLogoutField? rspUserLogoutField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_rspUserLogoutField = rspUserLogoutField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public RspUserLogoutField? RspUserLogoutField { get { return m_rspUserLogoutField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly RspUserLogoutField? m_rspUserLogoutField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspOrderInsertEventArgs : EventArgs
    {
        public RspOrderInsertEventArgs(InputOrderField? inputOrderField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_inputOrderField = inputOrderField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public InputOrderField? InputOrderField { get { return m_inputOrderField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly InputOrderField? m_inputOrderField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspOrderActionEventArgs : EventArgs
    {
        public RspOrderActionEventArgs(InputOrderActionField? inputOrderField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_inputOrderActionField = inputOrderField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public InputOrderActionField? InputOrderActionField { get { return m_inputOrderActionField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly InputOrderActionField? m_inputOrderActionField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspInputFundTransferEventArgs : EventArgs
    {
        public RspInputFundTransferEventArgs(InputFundTransferField? inputFundTransferField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_inputFundTransferField = inputFundTransferField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public InputFundTransferField? InputFundTransferField { get { return m_inputFundTransferField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly InputFundTransferField? m_inputFundTransferField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryFundEventArgs : EventArgs
    {
        public RspQryFundEventArgs(FundField? fundField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_fundField = fundField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public FundField? FundField { get { return m_fundField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly FundField? m_fundField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryPositionEventArgs : EventArgs
    {
        public RspQryPositionEventArgs(PositionField? positionField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_positionField = positionField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public PositionField? PositionField { get { return m_positionField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly PositionField? m_positionField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryOrderEventArgs : EventArgs
    {
        public RspQryOrderEventArgs(OrderField? orderField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_orderField = orderField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public OrderField? OrderField { get { return m_orderField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly OrderField? m_orderField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryTradeEventArgs : EventArgs
    {
        public RspQryTradeEventArgs(TradeField? tradeField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_tradeField = tradeField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public TradeField? TradeField { get { return m_tradeField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly TradeField? m_tradeField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryFundTransferEventArgs : EventArgs
    {
        public RspQryFundTransferEventArgs(FundTransferField? fundTransferField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_fundTransferField = fundTransferField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public FundTransferField? FundTransferField { get { return m_fundTransferField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly FundTransferField? m_fundTransferField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    

    public class RspQryHisOrderEventArgs : EventArgs
    {
        public RspQryHisOrderEventArgs(OrderField? orderField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_orderField = orderField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public OrderField? OrderField { get { return m_orderField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly OrderField? m_orderField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryHisTradeEventArgs : EventArgs
    {
        public RspQryHisTradeEventArgs(TradeField? tradeField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_tradeField = tradeField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public TradeField? TradeField { get { return m_tradeField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly TradeField? m_tradeField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryHisFundTransferEventArgs : EventArgs
    {
        public RspQryHisFundTransferEventArgs(FundTransferField? fundTransferField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_fundTransferField = fundTransferField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public FundTransferField? FundTransferFieldradeField { get { return m_fundTransferField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly FundTransferField? m_fundTransferField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryInstrumentEventArgs : EventArgs
    {
        public RspQryInstrumentEventArgs(InstrumentField? instrumentField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_instrumentField = instrumentField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public InstrumentField? InstrumentField { get { return m_instrumentField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly InstrumentField? m_instrumentField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryStructuredFundEventArgs : EventArgs
    {
        public RspQryStructuredFundEventArgs(StructuredFundField? structuredFundField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_structuredFundField = structuredFundField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public StructuredFundField? FundTransferField { get { return m_structuredFundField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly StructuredFundField? m_structuredFundField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryETFEventArgs : EventArgs
    {
        public RspQryETFEventArgs(ETFField? etfField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_ETFField = etfField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public ETFField? ETFField { get { return m_ETFField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly ETFField? m_ETFField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryETFCompositionEventArgs : EventArgs
    {
        public RspQryETFCompositionEventArgs(ETFCompositionField? etfCompositionField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_ETFCompositionField = etfCompositionField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public ETFCompositionField? ETFCompositionField { get { return m_ETFCompositionField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly ETFCompositionField? m_ETFCompositionField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryPurchasableNewIssueSecurityEventArgs : EventArgs
    {
        public RspQryPurchasableNewIssueSecurityEventArgs(NewIssueSecurityField? newIssueSecurityField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_newIssueSecurityField = newIssueSecurityField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public NewIssueSecurityField? NewIssueSecurityField { get { return m_newIssueSecurityField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly NewIssueSecurityField? m_newIssueSecurityField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RspQryPurchaseQuotaEventArgs : EventArgs
    {
        public RspQryPurchaseQuotaEventArgs(PurchaseQuotaField? purchaseQuotaField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_purchaseQuotaField = purchaseQuotaField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public PurchaseQuotaField? PurchaseQuotaField { get { return m_purchaseQuotaField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly PurchaseQuotaField?m_purchaseQuotaField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

    public class RtnOrderExecutionEventArgs : EventArgs
    {
        public RtnOrderExecutionEventArgs(ExecutionReportField executionReportField)
        {
            m_executionReportField = executionReportField;

        }
        public ExecutionReportField ExecutionReportField { get { return m_executionReportField; } }

        private readonly ExecutionReportField m_executionReportField;

    }

    public class RspQrySecurityAccountEventArgs : EventArgs
    {
        public RspQrySecurityAccountEventArgs(SecurityAccountField? securityAccountField,
                                     ErrorField? errorField,
                                     int requestID,
                                     bool isLast)
        {
            m_securityAccountField = securityAccountField;
            m_errorField = errorField;
            m_requestID = requestID;
            m_isLast = isLast;
        }
        public SecurityAccountField? SecurityAccountField { get { return m_securityAccountField; } }
        public ErrorField? ErrorField { get { return m_errorField; } }
        public int RequestID { get { return m_requestID; } }
        public bool IsLast { get { return m_isLast; } }

        private readonly SecurityAccountField? m_securityAccountField;
        private readonly ErrorField? m_errorField;
        private readonly int m_requestID;
        private readonly bool m_isLast;
    }

}
