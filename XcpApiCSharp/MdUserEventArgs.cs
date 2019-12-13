using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Xcp
{
    public class MdUserFrontDisconnectedEventArgs : EventArgs
    {
        public MdUserFrontDisconnectedEventArgs(int reason)
        {
            m_reason = reason;
        }
        private readonly int m_reason;
        public int Reason { get { return m_reason; } }
    }


    public class MdUserHeartBeatWarningEventArgs : EventArgs
    {
        public MdUserHeartBeatWarningEventArgs(int timeLapse)
        {
            m_timeLapse = timeLapse;
        }
        private readonly int m_timeLapse;
        public int TimeLapse { get { return m_timeLapse; } }
    }

    public class MdUserRspUserLoginEventArgs : EventArgs
    {
        public MdUserRspUserLoginEventArgs(RspUserLoginField rspUserLoginField,
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


    public class MdUserRspUserLogoutEventArgs : EventArgs
    {
        public MdUserRspUserLogoutEventArgs(RspUserLogoutField? rspUserLogoutField,
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

    public class MdUserRtnMarketDataEventArgs : EventArgs
    {
        public MdUserRtnMarketDataEventArgs(MarketDataField? marketDataField)
        {
            m_marketDataField = marketDataField;

        }
        public MarketDataField? MarketDataField { get { return m_marketDataField; } }


        private readonly MarketDataField? m_marketDataField;

    }

    public class MdUserRspSubMarketDataEventArgs : EventArgs
    {
        public MdUserRspSubMarketDataEventArgs(InstrumentField? instrumentField,
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

    public class MdUserRspUnsubMarketDataEventArgs : EventArgs
    {
        public MdUserRspUnsubMarketDataEventArgs(InstrumentField? instrumentField,
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


}
