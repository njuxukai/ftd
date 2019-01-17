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
        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "create_trader",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void CreateTrader(out IntPtr phandler, String pswDir);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "init_trader",
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
        public static extern int ReqOrderAction(IntPtr phandler, ref InputOrderActionField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_fund_transfer",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqFundTransfer(IntPtr phandler, ref InputFundTransferField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_order",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryOrder(IntPtr phandler, ref QryOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_trade",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryTrade(IntPtr phandler, ref QryTradeField field, int reqID);


        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_fund",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryFund(IntPtr phandler, ref QryFundField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_position",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryPosition(IntPtr phandler, ref QryPositionField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_fund_transfer",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryFundTransfer(IntPtr phandler, ref QryFundTransferField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_his_order",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryHisOrder(IntPtr phandler, ref QryHisOrderField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_his_trade",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryHisTrade(IntPtr phandler, ref QryHisTradeField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_his_fund_transfer",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryHisFundTransfer(IntPtr phandler, ref QryHisFundTransferField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_instrument",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryInstrument(IntPtr phandler, ref QryInstrumentField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_etf",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryETF(IntPtr phandler, ref QryETFField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_etf_composition",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryETFComposition(IntPtr phandler, ref QryETFCompositionField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_structured_fund",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryStructuredFund(IntPtr phandler, ref QryStructuredFundField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_purchasable_new_issue_security",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryPurchasableNewIssueSecurity(IntPtr phandler, ref QryNewIssueSecurityField field, int reqID);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_qry_purchase_quota",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqQryPurchaseQuota(IntPtr phandler, ref QryPurchaseQuotaField field, int reqID);


        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "register_front",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void RegisterFront(IntPtr phandler, [MarshalAs(UnmanagedType.LPStr)]String frontAddress);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "subscribe_private_topic",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void SubscribePrivateTopic(IntPtr phandler, THOST_TE_RESUME_TYPE resumeType);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "subscribe_public_topic",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void SubscribePublicTopic(IntPtr phandler, THOST_TE_RESUME_TYPE resumeType);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnFrontConnected",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnFrontConnectedCallback(IntPtr phandler,
            OnFrontConnectedDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnFrontDisconnected",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnFrontDisconnectedCallback(IntPtr phandler,
            OnFrontDisconnectedDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnHeartBeatWarning",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnHeartBeatWarningCallback(IntPtr phandler,
            OnHeartBeatWarningDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnHeartBeat",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnHeartBeatCallback(IntPtr phandler,
            OnHeartBeatDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspUserLogin",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspUserLoginCallback(IntPtr phandler,
            OnRspUserLoginDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspUserLogout",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspUserLogoutCallback(IntPtr phandler,
            OnRspUserLogoutDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspInputOrder",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspInputOrderCallback(IntPtr phandler,
            OnRspInputOrderDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspInputOrderAction",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspInputOrderActionCallback(IntPtr phandler,
            OnRspInputOrderActionDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspInputFundTransfer",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspInputFundTransferCallback(IntPtr phandler,
            OnRspInputFundTransferDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryFund",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryFundCallback(IntPtr phandler,
            OnRspQryFundDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryPosition",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryPositionCallback(IntPtr phandler,
            OnRspQryPositionDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryOrder",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryOrderCallback(IntPtr phandler,
            OnRspQryOrderDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryTrade",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryTradeCallback(IntPtr phandler,
            OnRspQryTradeDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryFundTransfer",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryFundTransferCallback(IntPtr phandler,
            OnRspQryFundTransferDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryHisOrder",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryHisOrderCallback(IntPtr phandler,
            OnRspQryHisOrderDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryHisTrade",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryHisTradeCallback(IntPtr phandler,
            OnRspQryHisTradeDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryHisFundTransfer",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryHisFundTransferCallback(IntPtr phandler,
            OnRspQryHisFundTransferDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryInstrument",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryInstrumentCallback(IntPtr phandler,
            OnRspQryInstrumentDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryStructuredFund",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryStructuredFundCallback(IntPtr phandler,
            OnRspQryStructuredFundDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryETF",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryETFCallback(IntPtr phandler,
            OnRspQryETFDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryETFComposition",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryETFCompositionCallback(IntPtr phandler,
            OnRspQryETFCompositionDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryPurchasableNewIssueSecurity",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryPurchasableNewIssueSecurityCallback(IntPtr phandler,
            OnRspQryPurchasableNewIssueSecurityDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspQryPurchaseQuota",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspQryPurchaseQuotaCallback(IntPtr phandler,
            OnRspQryPurchaseQuotaDelegate callback);

        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRtnOrderExecutionReport",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRtnOrderExecutionReportCallback(IntPtr phandler,
            OnRtnOrderExecutionReportDelegate callback);


    }
}
