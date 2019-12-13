using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Xcp
{


    public class MdUser : IDisposable
    {
        public MdUser(String pswDir="") 
        {
            InitDelegates();
            MdUserDllWrapper.CreateMdUser(out m_handler, pswDir);
            RegisterEventHandlers();
        }

        ~MdUser()
        {
            Dispose(false);
        }

        public void Init()
        {
            MdUserDllWrapper.InitMdUser(m_handler);
        }

        public void Release()
        {
            if (m_handler != IntPtr.Zero)
            {
                MdUserDllWrapper.ReleaseMdUser(out m_handler);
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
                MdUserDllWrapper.ReleaseMdUser(out m_handler);
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
            onRtnMarketDataDelegate = RaiseRtnMarketData;
            onRspSubMarketDataDelegate = RaiseRspSubMarketData;
            onRspUnsubMarketDataDelegate = RaiseRspUnsubMarketData;

        }
        private void RegisterEventHandlers()
        {
            if (m_handler != IntPtr.Zero)
            {
                MdUserDllWrapper.RegisterOnFrontConnectedCallback(m_handler, onFronConnectedDelegate);
                MdUserDllWrapper.RegisterOnFrontDisconnectedCallback(m_handler, onFrontDisconnectedDelegate);
                MdUserDllWrapper.RegisterOnHeartBeatWarningCallback(m_handler, onHeartBeatWarningDelegate);
                MdUserDllWrapper.RegisterOnHeartBeatCallback(m_handler, onHeartBeatDelegate);
                MdUserDllWrapper.RegisterOnRspUserLoginCallback(m_handler, onRspUserLoginDelegate);
                MdUserDllWrapper.RegisterOnRspUserLogoutCallback(m_handler, onRspUserLogoutDelegate);
                MdUserDllWrapper.RegisterOnRtnMarketDataCallback(m_handler, onRtnMarketDataDelegate);
                MdUserDllWrapper.RegisterOnRspSubMarketDataCallback(m_handler, onRspSubMarketDataDelegate);
                MdUserDllWrapper.RegisterOnRspUnsubMarketDataCallback(m_handler, onRspUnsubMarketDataDelegate);
            }
        }

        #region MdUserApi
        public String GetApiVersion()
        {
            return MdUserDllWrapper.GetApiVersion(m_handler);
        }

        public String GetTradingDay()
        {
            return MdUserDllWrapper.GetTradingDay(m_handler);
        }

        public int ReqUserLogin(ReqUserLoginField field, int reqID)
        {
            return MdUserDllWrapper.ReqUserLogin(m_handler, ref field, reqID);
        }

        public int ReqSubMarketData(InstrumentField[] fields, int size, int reqID)
        {
            return MdUserDllWrapper.ReqSubMarketData(m_handler, fields, size, reqID);
        }

        public int ReqUnsubMarketData(InstrumentField[] fields, int size, int reqID)
        {
            return MdUserDllWrapper.ReqUnsubMarketData(m_handler, fields, size, reqID);
        }


        public void RegisterFront(String frontAddress)
        {
            MdUserDllWrapper.RegisterFront(m_handler, frontAddress);
        }

        #endregion

        #region event
        public event EventHandler<EventArgs> OnFrontConnected;
        public event EventHandler<MdUserFrontDisconnectedEventArgs> OnFrontDisconnected;
        public event EventHandler<MdUserHeartBeatWarningEventArgs> OnHeartBeatWarning;
        public event EventHandler<EventArgs> OnHeartBeat;
        public event EventHandler<MdUserRspUserLoginEventArgs> OnRspUserLogin;
        public event EventHandler<MdUserRspUserLogoutEventArgs> OnRspUserLogout;
        public event EventHandler<MdUserRtnMarketDataEventArgs> OnRtnMarketData;
        public event EventHandler<MdUserRspSubMarketDataEventArgs> OnRspSubMarketData;
        public event EventHandler<MdUserRspUnsubMarketDataEventArgs> OnRspUnsubMarketData;
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
            EventHandler<EventArgs> temp = Volatile.Read(ref OnFrontConnected);
            if (temp != null)
                temp(this, EventArgs.Empty);
        }

        private void RaiseHeartBeat()
        {
            Volatile.Read(ref OnHeartBeat)?.Invoke(this, EventArgs.Empty);
        }

        private void RaiseFrontDisconnected(int nReason)
        {
            MdUserFrontDisconnectedEventArgs eventArgs = new MdUserFrontDisconnectedEventArgs(nReason);
            Volatile.Read(ref OnFrontDisconnected)?.Invoke(this, eventArgs);
        }

        private void RaiseHeartBeatWarning(int nTimeLapse)
        {
            MdUserHeartBeatWarningEventArgs eventArgs = new MdUserHeartBeatWarningEventArgs(nTimeLapse);
            Volatile.Read(ref OnHeartBeatWarning)?.Invoke(this, eventArgs);
        }

        private void RaiseRspUserLogin(IntPtr rspUserLogin, IntPtr errorField, int nRequestID, bool isLast)
        {
            RspUserLoginField  rsp = Marshal.PtrToStructure<RspUserLoginField>(rspUserLogin);
            ErrorField? error = null;
            if (errorField != IntPtr.Zero)
            {
                error = Marshal.PtrToStructure<ErrorField>(errorField);
            }
            MdUserRspUserLoginEventArgs eventArgs = new MdUserRspUserLoginEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref OnRspUserLogin)?.Invoke(this, eventArgs);
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
            MdUserRspUserLogoutEventArgs eventArgs = new MdUserRspUserLogoutEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref OnRspUserLogout)?.Invoke(this, eventArgs);

        }

        private void RaiseRtnMarketData(IntPtr pdata)
        {

            MarketDataField? marketData = null;
            if (pdata != IntPtr.Zero)
            {
                marketData = Marshal.PtrToStructure<MarketDataField>(pdata);
            }
            MdUserRtnMarketDataEventArgs eventArgs = new MdUserRtnMarketDataEventArgs(marketData);
            Volatile.Read(ref OnRtnMarketData)?.Invoke(this, eventArgs);

        }

        private void RaiseRspSubMarketData(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
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
            MdUserRspSubMarketDataEventArgs eventArgs = new MdUserRspSubMarketDataEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref OnRspSubMarketData)?.Invoke(this, eventArgs);

        }

        private void RaiseRspUnsubMarketData(IntPtr pRsp, IntPtr pError, int nRequestID, bool isLast)
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
            MdUserRspUnsubMarketDataEventArgs eventArgs = new MdUserRspUnsubMarketDataEventArgs(rsp, error, nRequestID, isLast);
            Volatile.Read(ref OnRspUnsubMarketData)?.Invoke(this, eventArgs);

        }

        #endregion

        #region  delegate save
        MdUserOnFrontConnectedDelegate onFronConnectedDelegate;// = RaiseFrontConnected;
        MdUserOnFrontDisconnectedDelegate onFrontDisconnectedDelegate;
        MdUserOnHeartBeatWarningDelegate onHeartBeatWarningDelegate;
        MdUserOnHeartBeatDelegate onHeartBeatDelegate;
        MdUserOnRspUserLoginDelegate onRspUserLoginDelegate;
        MdUserOnRspUserLogoutDelegate onRspUserLogoutDelegate;
        MdUserOnRtnMarketDataDelegate onRtnMarketDataDelegate;
        MdUserOnRspSubMarketDataDelegate onRspSubMarketDataDelegate;
        MdUserOnRspUnsubMarketDataDelegate onRspUnsubMarketDataDelegate;
        #endregion
        private IntPtr m_handler;
    }
}
