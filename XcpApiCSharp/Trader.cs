using System;
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
        public Trader(int brokerID, int userID, int investorID, String password, String pswDir="") 
        {
            m_brokerID = brokerID;
            m_userID = userID;
            m_investorID = investorID;
            m_password = password;
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

        private void RegisterEventHandlers()
        {
            if (m_handler != IntPtr.Zero)
            {
                TraderDllWrapper.RegisterOnFrontConnectedCallback(m_handler, RaiseFrontConnected);
                TraderDllWrapper.RegisterOnFrontDisconnectedCallback(m_handler, RaiseFrontDisconnected);
                TraderDllWrapper.RegisterOnHeartBeatWarningCallback(m_handler, RaiseHeartBeatWarning);
                TraderDllWrapper.RegisterOnRspUserLoginCallback(m_handler, RaiseRspUserLogin);
                TraderDllWrapper.RegisterOnRspUserLogoutCallback(m_handler, RaiseRspUserLogout);
                TraderDllWrapper.RegisterOnRspInputOrderCallback(m_handler, RaiseRspInputOrder);
                TraderDllWrapper.RegisterOnRspInputOrderActionCallback(m_handler, RaiseRspInputOrderAction);
                TraderDllWrapper.RegisterOnRspInputFundTransferCallback(m_handler, RaiseRspInputFundTransfer);
                TraderDllWrapper.RegisterOnRspQryFundCallback(m_handler, RaiseRspQryFund);
                TraderDllWrapper.RegisterOnRspQryPositionCallback(m_handler, RaiseRspQryPosition);
                TraderDllWrapper.RegisterOnRspQryOrderCallback(m_handler, RaiseRspQryOrder);
                TraderDllWrapper.RegisterOnRspQryTradeCallback(m_handler, RaiseRspQryTrade);
                TraderDllWrapper.RegisterOnRspQryFundTransferCallback(m_handler, RaiseRspQryFundTransfer);
                TraderDllWrapper.RegisterOnRspQryHisOrderCallback(m_handler, RaiseRspQryHisOrder);
                TraderDllWrapper.RegisterOnRspQryHisTradeCallback(m_handler, RaiseRspQryHisTrade);
                TraderDllWrapper.RegisterOnRspQryHisFundTransferCallback(m_handler, RaiseRspQryHisFundTransfer);
                TraderDllWrapper.RegisterOnRspQryInstrumentCallback(m_handler, RaiseRspQryInstrument);
                TraderDllWrapper.RegisterOnRspQryStructuredFundCallback(m_handler, RaiseRspQryStructuredFund);
                TraderDllWrapper.RegisterOnRspQryETFCallback(m_handler, RaiseRspQryETF);
                TraderDllWrapper.RegisterOnRspQryETFCompositionCallback(m_handler, RaiseRspQryETFComposition);
                TraderDllWrapper.RegisterOnRspQryPurchasableNewIssueSecurityCallback(m_handler, RaiseRspQryPurchasableNewIssueSecurity);
                TraderDllWrapper.RegisterOnRspQryPurchaseQuotaCallback(m_handler, RaiseRspQryPurchaseQuota);
                TraderDllWrapper.RegisterOnRtnOrderExecutionReportCallback(m_handler, RaiseRtnOrderExecutionReport);
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
        public event EventHandler<EventArgs> OnFrontConnected;
        public event EventHandler<FrontDisconnectedEventArgs> OnFrontDisconnected;
        public event EventHandler<HeartBeatWarningEventArgs> onHeartBeatWarning;
        public event EventHandler<RspUserLoginEventArgs> onRspUserLogin;
        public event EventHandler<RspUserLogoutEventArgs> onRspUserLogout;
        public event EventHandler<RspInputOrderEventArgs> onRspInputOrder;
        public event EventHandler<RspInputOrderActionEventArgs> onRspInputOrderAction;
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
        #endregion

        #region callback 回调函数触发事件 FrontConnected时应自动登录
        private void RaiseFrontConnected()
        {
            //TODO Login here
            m_nextReqID = 1;
            ReqUserLoginField field = new ReqUserLoginField
                {
                    BrokerID = m_brokerID,
                    UserID = m_userID,
                    Password = m_password
                };
            ReqUserLogin(field, m_nextReqID++);
            EventHandler<EventArgs> temp = Volatile.Read(ref OnFrontConnected);
            if (temp != null)
                temp(this, EventArgs.Empty);
        }

        private void RaiseFrontDisconnected(int nReason)
        {
            FrontDisconnectedEventArgs eventArgs = new FrontDisconnectedEventArgs(nReason);
            Volatile.Read(ref OnFrontDisconnected)?.Invoke(this, eventArgs);
        }

        private void RaiseHeartBeatWarning(int nTimeLapse)
        {
            HeartBeatWarningEventArgs eventArgs = new HeartBeatWarningEventArgs(nTimeLapse);
            Volatile.Read(ref onHeartBeatWarning)?.Invoke(this, eventArgs);
        }

        private void RaiseRspUserLogin(IntPtr rspUserLogin, IntPtr errorField, int nRequestID, bool isLast)
        {
            RspUserLoginField? rsp = null;
            ErrorField? error = null;
            if (rspUserLogin != IntPtr.Zero)
            {
                RspUserLoginField data = new RspUserLoginField();
                Marshal.PtrToStructure(rspUserLogin, data);
                rsp = data;
            }
            if (errorField != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(errorField, data);
                error = data;
            }
            RspUserLoginEventArgs eventArgs = new RspUserLoginEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspUserLogin)?.Invoke(this, eventArgs);
        }

        private void RaiseRspUserLogout(IntPtr rspUserLogout, IntPtr errorField, int nRequestID, bool isLast)
        {

            RspUserLogoutField? rsp = null;
            ErrorField? error = null;
            if (rspUserLogout != IntPtr.Zero)
            {
                RspUserLogoutField data = new RspUserLogoutField();
                Marshal.PtrToStructure(rspUserLogout, data);
                rsp = data;
            }
            if (errorField != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(errorField, data);
                error = data;
            }
            RspUserLogoutEventArgs eventArgs = new RspUserLogoutEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspUserLogout)?.Invoke(this, eventArgs);

        }

        private void RaiseRspInputOrder(IntPtr inputOrder, IntPtr errorField, int nRequestID, bool isLast)
        {
            InputOrderField? rsp = null;
            ErrorField? error = null;
            if (inputOrder != IntPtr.Zero)
            {
                InputOrderField data = new InputOrderField();
                Marshal.PtrToStructure(inputOrder, data);
                rsp = data;
            }
            if (errorField != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(errorField, data);
                error = data;
            }
            RspInputOrderEventArgs eventArgs = new RspInputOrderEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspInputOrder)?.Invoke(this, eventArgs);
        }

        private void RaiseRspInputOrderAction(IntPtr inputOrderAction, IntPtr errorField, int nRequestID, bool isLast)
        {
            InputOrderActionField? rsp = null;
            ErrorField? error = null;
            if (inputOrderAction != IntPtr.Zero)
            {
                InputOrderActionField data = new InputOrderActionField();
                Marshal.PtrToStructure(inputOrderAction, data);
                rsp = data;
            }
            if (errorField != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(errorField, data);
                error = data;
            }
            RspInputOrderActionEventArgs eventArgs = new RspInputOrderActionEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspInputOrderAction)?.Invoke(this, eventArgs);
        }

        private void RaiseRspInputFundTransfer(IntPtr inputFundTransfer, IntPtr errorField, int nRequestID, bool isLast)
        {
            InputFundTransferField? rsp = null;
            ErrorField? error = null;
            if (inputFundTransfer != IntPtr.Zero)
            {
                InputFundTransferField data = new InputFundTransferField();
                Marshal.PtrToStructure(inputFundTransfer, data);
                rsp = data;
            }
            if (errorField != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(errorField, data);
                error = data;
            }
            RspInputFundTransferEventArgs eventArgs = new RspInputFundTransferEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspInputFundTransfer)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryFund(IntPtr fund, IntPtr errorField, int nRequestID, bool isLast)
        {
            FundField? rsp = null;
            ErrorField? error = null;
            if (fund != IntPtr.Zero)
            {
                FundField data = new FundField();
                Marshal.PtrToStructure(fund, data);
                rsp = data;
            }
            if (errorField != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(errorField, data);
                error = data;
            }
            RspQryFundEventArgs eventArgs = new RspQryFundEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryFund)?.Invoke(this, eventArgs);
        }

        private void RaiseRspQryPosition(IntPtr position, IntPtr errorField, int nRequestID, bool isLast)
        {
            PositionField? rsp = null;
            ErrorField? error = null;
            if (position != IntPtr.Zero)
            {
                PositionField data = new PositionField();
                Marshal.PtrToStructure(position, data);
                rsp = data;
            }
            if (errorField != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(errorField, data);
                error = data;
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
                OrderField data = new OrderField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                TradeField data = new TradeField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                FundTransferField data = new FundTransferField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                OrderField data = new OrderField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                TradeField data = new TradeField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                FundTransferField data = new FundTransferField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                InstrumentField data = new InstrumentField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                StructuredFundField data = new StructuredFundField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                ETFField data = new ETFField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                ETFCompositionField data = new ETFCompositionField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                NewIssueSecurityField data = new NewIssueSecurityField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
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
                PurchaseQuotaField data = new PurchaseQuotaField();
                Marshal.PtrToStructure(pRsp, data);
                rsp = data;
            }
            if (pError != IntPtr.Zero)
            {
                ErrorField data = new ErrorField();
                Marshal.PtrToStructure(pError, data);
                error = data;
            }
            var eventArgs = new RspQryPurchaseQuotaEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref onRspQryPurchaseQuota)?.Invoke(this, eventArgs);
        }

        private void RaiseRtnOrderExecutionReport(IntPtr pOrderExecution)
        {
            if (pOrderExecution == IntPtr.Zero)
                return;
            ExecutionReportField data = new ExecutionReportField();
            Marshal.PtrToStructure(pOrderExecution, data);  
            var eventArgs = new RtnOrderExecutionEventArgs(data);
            Volatile.Read(ref onRtnOrderExecution)?.Invoke(this, eventArgs);
        }
        #endregion

        private int m_brokerID;
        private int m_userID;
        private int m_investorID;
        private String m_password;

        private int m_nextReqID;
        private IntPtr m_handler;
    }
}
