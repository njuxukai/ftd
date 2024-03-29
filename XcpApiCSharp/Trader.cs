﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Xcp
{


    public class Trader : IDisposable
    {
        public Trader(String pswDir="") 
        {
            InitDelegates();
            TraderDllWrapper.CreateTrader(out m_handler, pswDir);
            RegisterEventHandlers();
        }

        ~Trader()
        {
            Dispose(false);
        }

        public void Init()
        {
            TraderDllWrapper.InitTrader(m_handler);
        }

        public void Release()
        {
            if (m_handler != IntPtr.Zero)
            {
                TraderDllWrapper.ReleaseTrader(out m_handler);
                m_handler = IntPtr.Zero;
            }
        }
        protected virtual void Dispose(bool disposing)
        {
            //use called(release managed object)
            if (disposing) { }
            //Release Unmanaged objects
            if (m_handler != IntPtr.Zero)
            {
                TraderDllWrapper.ReleaseTrader(out m_handler);
                m_handler = IntPtr.Zero;
            }

            if (disposing)
            {
                GC.SuppressFinalize(this);
            }
        }

        public void Dispose()
        {
            Dispose(true);
        }

        private void InitDelegates()
        {
            onFronConnectedDelegate = RaiseFrontConnected;
            onFrontDisconnectedDelegate = RaiseFrontDisconnected;
            onHeartBeatWarningDelegate = RaiseHeartBeatWarning;
            onHeartBeatDelegate = RaiseHeartBeat;
            onRspUserLoginDelegate = RaiseRspUserLogin;

            onRspUserLogoutDelegate = RaiseRspUserLogout;
            onRspOrderInsertDelegate = RaiseRspOrderInsert;
            onRspOrderActionDelegate = RaiseRspOrderAction;
            onRspInputFundTransferDelegate = RaiseRspInputFundTransfer;
            onRspQryFundDelegate = RaiseRspQryFund;
            onRspQryPositionDelegate = RaiseRspQryPosition;
            onRspQryOrderDelegate = RaiseRspQryOrder;
            onRspQryTradeDelegate = RaiseRspQryTrade;
            onRspQryFundTransferDelegate = RaiseRspQryFundTransfer;
            onRspQryHisOrderDelegate = RaiseRspQryHisOrder;
            onRspQryHisTradeDelegate = RaiseRspQryHisTrade;
            onRspQryHisFundTransferDelegate = RaiseRspQryHisFundTransfer;
            onRspQryInstrumentDelegate = RaiseRspQryInstrument;
            onRspQryStructuredFundDelegate = RaiseRspQryInstrument;
            onRspQryETFDelegate = RaiseRspQryETF;
            onRspQryETFCompositionDelegate = RaiseRspQryETFComposition;
            onRspQryPurchasableNewIssueSecurityDelegate = RaiseRspQryPurchasableNewIssueSecurity;
            onRspQryPurchaseQuotaDelegate = RaiseRspQryPurchaseQuota;
            onRtnOrderExecutionReportDelegate = RaiseRtnOrderExecutionReport;
            onRspQrySecurityAccountDelegate = RaiseRspQrySecurityAccount;
        }
        private void RegisterEventHandlers()
        {
            if (m_handler != IntPtr.Zero)
            {
                TraderDllWrapper.RegisterOnFrontConnectedCallback(m_handler, onFronConnectedDelegate);
                TraderDllWrapper.RegisterOnFrontDisconnectedCallback(m_handler, onFrontDisconnectedDelegate);
                TraderDllWrapper.RegisterOnHeartBeatWarningCallback(m_handler, onHeartBeatWarningDelegate);
                TraderDllWrapper.RegisterOnHeartBeatCallback(m_handler, onHeartBeatDelegate);
                TraderDllWrapper.RegisterOnRspUserLoginCallback(m_handler, onRspUserLoginDelegate);
                TraderDllWrapper.RegisterOnRspUserLogoutCallback(m_handler, onRspUserLogoutDelegate);
                TraderDllWrapper.RegisterOnRspInputOrderCallback(m_handler, onRspOrderInsertDelegate);
                TraderDllWrapper.RegisterOnRspInputOrderActionCallback(m_handler, onRspOrderActionDelegate);
                TraderDllWrapper.RegisterOnRspInputFundTransferCallback(m_handler, onRspInputFundTransferDelegate);
                TraderDllWrapper.RegisterOnRspQryFundCallback(m_handler, onRspQryFundDelegate);
                TraderDllWrapper.RegisterOnRspQryPositionCallback(m_handler, onRspQryPositionDelegate);
                TraderDllWrapper.RegisterOnRspQryOrderCallback(m_handler, onRspQryOrderDelegate);
                TraderDllWrapper.RegisterOnRspQryTradeCallback(m_handler, onRspQryTradeDelegate);
                TraderDllWrapper.RegisterOnRspQryFundTransferCallback(m_handler, onRspQryFundTransferDelegate);
                TraderDllWrapper.RegisterOnRspQryHisOrderCallback(m_handler, onRspQryHisOrderDelegate);
                TraderDllWrapper.RegisterOnRspQryHisTradeCallback(m_handler, onRspQryHisTradeDelegate);
                TraderDllWrapper.RegisterOnRspQryHisFundTransferCallback(m_handler, onRspQryHisFundTransferDelegate);
                TraderDllWrapper.RegisterOnRspQryInstrumentCallback(m_handler, onRspQryInstrumentDelegate);
                TraderDllWrapper.RegisterOnRspQryStructuredFundCallback(m_handler, onRspQryStructuredFundDelegate);
                TraderDllWrapper.RegisterOnRspQryETFCallback(m_handler, onRspQryETFDelegate);
                TraderDllWrapper.RegisterOnRspQryETFCompositionCallback(m_handler, onRspQryETFCompositionDelegate);
                TraderDllWrapper.RegisterOnRspQryPurchasableNewIssueSecurityCallback(m_handler, onRspQryPurchasableNewIssueSecurityDelegate);
                TraderDllWrapper.RegisterOnRspQryPurchaseQuotaCallback(m_handler, onRspQryPurchaseQuotaDelegate);
                TraderDllWrapper.RegisterOnRtnOrderExecutionReportCallback(m_handler, onRtnOrderExecutionReportDelegate);
                TraderDllWrapper.RegisterOnRspQrySecurityAccountCallback(m_handler, onRspQrySecurityAccountDelegate);
            }
        }

        #region TraderApi
        public String GetApiVersion()
        {
            return TraderDllWrapper.GetApiVersion(m_handler);
        }

        public String GetTradingDay()
        {
            return TraderDllWrapper.GetTradingDay(m_handler);
        }

        public int ReqUserLogin(ReqUserLoginField field, int reqID)
        {
            return TraderDllWrapper.ReqUserLogin(m_handler, ref field, reqID);
        }

        public int ReqUserLogout(ReqUserLogoutField field, int reqID)
        {
            return TraderDllWrapper.ReqUserLogout(m_handler, ref field, reqID);
        }

        public int ReqOrderInsert(InputOrderField field, int reqID)
        {
            return TraderDllWrapper.ReqOrderInsert(m_handler, ref field, reqID);
        }

        public int ReqOrderAction(InputOrderActionField field, int reqID)
        {
            return TraderDllWrapper.ReqOrderAction(m_handler, ref field, reqID);
        }

        public int ReqFundTransfer(InputFundTransferField field, int reqID)
        {
            return TraderDllWrapper.ReqFundTransfer(m_handler, ref field, reqID);
        }

        public int ReqQryOrder(QryOrderField field, int reqID)
        {
            return TraderDllWrapper.ReqQryOrder(m_handler, ref field, reqID);
        }

        public int ReqQryTrade(QryTradeField field, int reqID)
        {
            return TraderDllWrapper.ReqQryTrade(m_handler, ref field, reqID);
        }

        public int ReqQryFund(QryFundField field, int reqID)
        {
            return TraderDllWrapper.ReqQryFund(m_handler, ref field, reqID);
        }

        public int ReqQryPosition(QryPositionField field, int reqID)
        {
            return TraderDllWrapper.ReqQryPosition(m_handler, ref field, reqID);
        }

        public int ReqQryFundTransfer(QryFundTransferField field, int reqID)
        {
            return TraderDllWrapper.ReqQryFundTransfer(m_handler, ref field, reqID);
        }

        public int ReqQryHisOrder(QryHisOrderField field, int reqID)
        {
            return TraderDllWrapper.ReqQryHisOrder(m_handler, ref field, reqID);
        }

        public int ReqQryHisTrade(QryHisTradeField field, int reqID)
        {
                return TraderDllWrapper.ReqQryHisTrade(m_handler, ref field, reqID);
        }

        public int ReqQryHisFundTransfer(QryHisFundTransferField field, int reqID)
        {
            return TraderDllWrapper.ReqQryHisFundTransfer(m_handler, ref field, reqID);
        }

        public int ReqQryInstrument(QryInstrumentField field, int reqID)
        {
            return TraderDllWrapper.ReqQryInstrument(m_handler, ref field, reqID);
        }

        public int ReqQryETF(QryETFField field, int reqID)
        {
            return TraderDllWrapper.ReqQryETF(m_handler, ref field, reqID);
        }

        public int ReqQryETFComposition(QryETFCompositionField field, int reqID)
        {
            return TraderDllWrapper.ReqQryETFComposition(m_handler, ref field, reqID);
        }

        public int ReqQryStructuredFund(QryStructuredFundField field, int reqID)
        {
            return TraderDllWrapper.ReqQryStructuredFund(m_handler, ref field, reqID);
        }

        public int ReqQryPurchasableNewIssueSecurity(QryNewIssueSecurityField field, int reqID)
        {
            return TraderDllWrapper.ReqQryPurchasableNewIssueSecurity(m_handler, ref field, reqID);
        }

        public int ReqQryPurchaseQuota(QryPurchaseQuotaField field, int reqID)
        {
            return TraderDllWrapper.ReqQryPurchaseQuota(m_handler, ref field, reqID);
        }

        public int ReqQrySecurityAccount(QrySecurityAccountField field, int reqID)
        {
            return TraderDllWrapper.ReqQrySecurityAccount(m_handler, ref field, reqID);
        }

        public void RegisterFront(String frontAddress)
        {
            TraderDllWrapper.RegisterFront(m_handler, frontAddress);
        }

        public void SubscribePrivateTopic(THOST_TE_RESUME_TYPE resumeType)
        {
            TraderDllWrapper.SubscribePrivateTopic(m_handler, resumeType);
        }

        public void SubscribePublicTopic(THOST_TE_RESUME_TYPE resumeType)
        {
            TraderDllWrapper.SubscribePublicTopic(m_handler, resumeType);
        }
        #endregion

        #region event
        public event EventHandler<EventArgs> onFrontConnected;
        public event EventHandler<FrontDisconnectedEventArgs> onFrontDisconnected;
        public event EventHandler<HeartBeatWarningEventArgs> onHeartBeatWarning;
        public event EventHandler<EventArgs> onHeartBeat;
        public event EventHandler<RspUserLoginEventArgs> onRspUserLogin;
        public event EventHandler<RspUserLogoutEventArgs> onRspUserLogout;
        public event EventHandler<RspOrderInsertEventArgs> onRspOrderInsert;
        public event EventHandler<RspOrderActionEventArgs> onRspOrderAction;
        public event EventHandler<RspInputFundTransferEventArgs> onRspInputFundTransfer;
        public event EventHandler<RspQryFundEventArgs> onRspQryFund;
        public event EventHandler<RspQryPositionEventArgs> onRspQryPosition;
        public event EventHandler<RspQryOrderEventArgs> onRspQryOrder;
        public event EventHandler<RspQryTradeEventArgs> onRspQryTrade;
        public event EventHandler<RspQryFundTransferEventArgs> onRspQryFundTransfer;
        public event EventHandler<RspQryHisOrderEventArgs> onRspQryHisOrder;
        public event EventHandler<RspQryHisTradeEventArgs> onRspQryHisTrade;
        public event EventHandler<RspQryHisFundTransferEventArgs> onRspQryHisFundTransfer;
        public event EventHandler<RspQryInstrumentEventArgs> onRspQryInstrument;
        public event EventHandler<RspQryStructuredFundEventArgs> onRspQryStructuredFund;
        public event EventHandler<RspQryETFEventArgs> onRspQryETF;
        public event EventHandler<RspQryETFCompositionEventArgs> onRspQryETFComposition;
        public event EventHandler<RspQryPurchasableNewIssueSecurityEventArgs> onRspQryPurchasableNewIssueSecurity;
        public event EventHandler<RspQryPurchaseQuotaEventArgs> onRspQryPurchaseQuota;
        public event EventHandler<RtnOrderExecutionEventArgs> onRtnOrderExecution;
        public event EventHandler<RspQrySecurityAccountEventArgs> onRspQrySecurityAccount;
        #endregion

        #region callback 回调函数触发事件 FrontConnected时应自动登录
        private void RaiseFrontConnected()
        {
            /*
            //TODO Login here
            m_nextReqID = 1;
            ReqUserLoginField field = new ReqUserLoginField
                {
                    BrokerID = m_brokerID,
                    UserID = m_userID,
                    Password = m_password
                };
            ReqUserLogin(field, m_nextReqID++);
            */
            EventHandler<EventArgs> temp = Volatile.Read(ref onFrontConnected);
            if (temp != null)
                temp(this, EventArgs.Empty);
        }

        private void RaiseHeartBeat()
        {
            Volatile.Read(ref onHeartBeat)?.Invoke(this, EventArgs.Empty);
        }

        private void RaiseFrontDisconnected(int nReason)
        {
            FrontDisconnectedEventArgs eventArgs = new FrontDisconnectedEventArgs(nReason);
            Volatile.Read(ref onFrontDisconnected)?.Invoke(this, eventArgs);
        }

        private void RaiseHeartBeatWarning(int nTimeLapse)
        {
            HeartBeatWarningEventArgs eventArgs = new HeartBeatWarningEventArgs(nTimeLapse);
            Volatile.Read(ref onHeartBeatWarning)?.Invoke(this, eventArgs);
        }

        private void RaiseRspUserLogin(IntPtr rspUserLogin, IntPtr errorField, int nRequestID, bool isLast)
        {
            RspUserLoginField  rsp = Marshal.PtrToStructure<RspUserLoginField>(rspUserLogin);
            ErrorField? error = null;
            if (errorField != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(errorField);
            }
            RspUserLoginEventArgs eventArgs = new RspUserLoginEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspUserLogin)?.Invoke(this, eventArgs);
        }

        private void RaiseRspUserLogout(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {

            RspUserLogoutField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<RspUserLogoutField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspUserLogoutEventArgs eventArgs = new RspUserLogoutEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspUserLogout)?.Invoke(this, eventArgs);

        }

        private void RaiseRspOrderInsert(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            InputOrderField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<InputOrderField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspOrderInsertEventArgs eventArgs = new RspOrderInsertEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspOrderInsert)?.Invoke(this, eventArgs);
        }

        private void RaiseRspOrderAction(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            InputOrderActionField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<InputOrderActionField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspOrderActionEventArgs eventArgs = new RspOrderActionEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspOrderAction)?.Invoke(this, eventArgs);
        }

        private void RaiseRspInputFundTransfer(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            InputFundTransferField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<InputFundTransferField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspInputFundTransferEventArgs eventArgs = new RspInputFundTransferEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspInputFundTransfer)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryFund(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            FundField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<FundField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryFundEventArgs eventArgs = new RspQryFundEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryFund)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryPosition(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            PositionField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<PositionField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryPositionEventArgs eventArgs = new RspQryPositionEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryPosition)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryOrder(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            OrderField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<OrderField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryOrderEventArgs eventArgs = new RspQryOrderEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryOrder)?.Invoke(this, eventArgs);
        }
    
        private void RaiseRspQryTrade(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            TradeField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<TradeField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryTradeEventArgs eventArgs = new RspQryTradeEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryTrade)?.Invoke(this, eventArgs);
        }


        private void RaiseRspQryFundTransfer(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            FundTransferField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<FundTransferField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryFundTransferEventArgs eventArgs = new RspQryFundTransferEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryFundTransfer)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryHisOrder(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            OrderField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<OrderField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryHisOrderEventArgs eventArgs = new RspQryHisOrderEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryHisOrder)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryHisTrade(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            TradeField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<TradeField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryHisTradeEventArgs eventArgs = new RspQryHisTradeEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryHisTrade)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryHisFundTransfer(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            FundTransferField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<FundTransferField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryHisFundTransferEventArgs eventArgs = new RspQryHisFundTransferEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryHisFundTransfer)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryInstrument(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            InstrumentField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<InstrumentField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryInstrumentEventArgs eventArgs = new RspQryInstrumentEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryInstrument)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryStructuredFund(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            StructuredFundField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<StructuredFundField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryStructuredFundEventArgs eventArgs = new RspQryStructuredFundEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryStructuredFund)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryETF(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            ETFField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<ETFField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryETFEventArgs eventArgs = new RspQryETFEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryETF)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryETFComposition(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            ETFCompositionField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<ETFCompositionField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            RspQryETFCompositionEventArgs eventArgs = new RspQryETFCompositionEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryETFComposition)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryPurchasableNewIssueSecurity(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            NewIssueSecurityField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<NewIssueSecurityField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            var eventArgs = 
                new RspQryPurchasableNewIssueSecurityEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryPurchasableNewIssueSecurity)?.Invoke(this, eventArgs);
        }


        private void RaiseRspQryPurchaseQuota(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            PurchaseQuotaField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<PurchaseQuotaField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            var eventArgs = new RspQryPurchaseQuotaEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryPurchaseQuota)?.Invoke(this, eventArgs);
        }

        private void RaiseRtnOrderExecutionReport(IntPtr pOrderExecution)
        {
            if (pOrderExecution == IntPtr.Zero)
                return;
            ExecutionReportField data = Marshal.PtrToStructure<ExecutionReportField>(pOrderExecution);  
            var eventArgs = new RtnOrderExecutionEventArgs(data);
            Volatile.Read(ref onRtnOrderExecution)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQrySecurityAccount(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
        {
            SecurityAccountField? rsp = null;
            ErrorField? error = null;
            if (pRsp != IntPtr.Zero)
            {
                rsp = Marshal.PtrToStructure<SecurityAccountField>(pRsp);
            }
            if (pError != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(pError);
            }
            var eventArgs = new RspQrySecurityAccountEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQrySecurityAccount)?.Invoke(this, eventArgs);
        }
        #endregion

        #region  delegate save
        OnFrontConnectedDelegate onFronConnectedDelegate;// = RaiseFrontConnected;
        OnFrontDisconnectedDelegate onFrontDisconnectedDelegate;
        OnHeartBeatWarningDelegate onHeartBeatWarningDelegate;
        OnHeartBeatDelegate onHeartBeatDelegate;
        OnRspUserLoginDelegate onRspUserLoginDelegate;
        OnRspUserLogoutDelegate onRspUserLogoutDelegate;
        OnRspOrderInsertDelegate onRspOrderInsertDelegate;
        OnRspOrderActionDelegate onRspOrderActionDelegate;
        OnRspInputFundTransferDelegate onRspInputFundTransferDelegate;
        OnRspQryFundDelegate onRspQryFundDelegate;
        OnRspQryPositionDelegate onRspQryPositionDelegate;
        OnRspQryOrderDelegate onRspQryOrderDelegate;
        OnRspQryTradeDelegate onRspQryTradeDelegate;
        OnRspQryFundTransferDelegate onRspQryFundTransferDelegate;
        OnRspQryHisOrderDelegate onRspQryHisOrderDelegate;
        OnRspQryHisTradeDelegate onRspQryHisTradeDelegate;
        OnRspQryHisFundTransferDelegate onRspQryHisFundTransferDelegate;
        OnRspQryInstrumentDelegate onRspQryInstrumentDelegate;
        OnRspQryStructuredFundDelegate onRspQryStructuredFundDelegate;
        OnRspQryETFDelegate onRspQryETFDelegate;
        OnRspQryETFCompositionDelegate onRspQryETFCompositionDelegate;
        OnRspQryPurchasableNewIssueSecurityDelegate onRspQryPurchasableNewIssueSecurityDelegate;
        OnRspQryPurchaseQuotaDelegate onRspQryPurchaseQuotaDelegate;
        OnRtnOrderExecutionReportDelegate onRtnOrderExecutionReportDelegate;
        OnRspQrySecurityAccountDelegate onRspQrySecurityAccountDelegate;
        #endregion
        private IntPtr m_handler;
    }
}
