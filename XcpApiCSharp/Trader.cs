using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Xcp
{

    class TraderDllWrapper
    {
        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint ="create_trader",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void CreateTrader(out IntPtr phandler);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "connect_trader",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitTrader(IntPtr phandler);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "release_trader",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void ReleaseTrader(out IntPtr phandler);       

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "get_api_version",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern String GetApiVersion(IntPtr phandler);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "get_trading_day",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern String GetTradingDay(IntPtr phandler);


        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_user_login",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqUserLogin(IntPtr phandler, ref ReqUserLoginField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_user_logout",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqUserLogout(IntPtr phandler, ref ReqUserLogoutField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_order_insert",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqOrderInsert(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_order_action",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqOrderAction(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_fund_transfer",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqFundTransfer(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_order",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryOrder(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_trade",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryTrade(IntPtr phandler, ref InputOrderField field, int reqID);


        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_fund",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryFund(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_position",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryPosition(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_fund_transfer",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryFundTransfer(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_his_order",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryHisOrder(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_his_trade",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryHisTrade(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_his_fund_transfer",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryHisFundTransfer(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_instrument",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryInstrument(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_etf",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryETF(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_etf_composition",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryETFInstrument(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_structured_fund",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryStructuredFund(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_purchasable_new_issue_security",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryPurchasableNewIssueSecurity(IntPtr phandler, ref InputOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_purchase_quota",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryPurchaseQuota(IntPtr phandler, ref InputOrderField field, int reqID);


        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "register_front",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterFront(IntPtr phandler, String frontAddress);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "subscribe_private_topic",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int SubscribePrivateTopic(IntPtr phandler, THOST_TE_RESUME_TYPE resumeType);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "subscribe_public_topic",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int SubscribePublicTopic(IntPtr phandler, THOST_TE_RESUME_TYPE resumeType);


    }

    public class Trader : IDisposable
    {
        public Trader() 
        {
            TraderDllWrapper.CreateTrader(out handler);
        }

        ~Trader()
        {
            Dispose(false);
        }

        void Connect()
        {
            TraderDllWrapper.InitTrader(handler);
        }


        protected virtual void Dispose(bool disposing)
        {
            //use called(release managed object)
            if (disposing) { }
            //Release Unmanaged objects
            if (handler != IntPtr.Zero)
            {
                TraderDllWrapper.ReleaseTrader(out handler);
                handler = IntPtr.Zero;
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


        #region callback
        #endregion

        #region event
        #endregion
        private IntPtr handler;
    }
}
