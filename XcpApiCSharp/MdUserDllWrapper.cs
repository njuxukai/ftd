using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Xcp
{
    class MdUserDllWrapper
    {
        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "create_mduser",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void CreateMdUser(out IntPtr phandler, String pswDir);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "init_mduser",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitMdUser(IntPtr phandler);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "release_mduser",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void ReleaseMdUser(out IntPtr phandler);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "get_api_version",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern String GetApiVersion(IntPtr phandler);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "get_trading_day",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern String GetTradingDay(IntPtr phandler);


        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_user_login",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqUserLogin(IntPtr phandler, ref ReqUserLoginField field, int reqID);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_user_logout",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqUserLogout(IntPtr phandler, ref ReqUserLogoutField field, int reqID);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_sub_market_data",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqSubMarketData(IntPtr phandler,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)]InstrumentField[] instruments, int size, int reqID);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "req_unsub_market_data",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReqUnsubMarketData(IntPtr phandler,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)]InstrumentField[] instruments, int size, int reqID);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "register_front",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern void RegisterFront(IntPtr phandler, [MarshalAs(UnmanagedType.LPStr)]String frontAddress);


        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnFrontConnected",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnFrontConnectedCallback(IntPtr phandler,
            MdUserOnFrontConnectedDelegate callback);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnFrontDisconnected",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnFrontDisconnectedCallback(IntPtr phandler,
            MdUserOnFrontDisconnectedDelegate callback);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnHeartBeatWarning",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnHeartBeatWarningCallback(IntPtr phandler,
            MdUserOnHeartBeatWarningDelegate callback);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnHeartBeat",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnHeartBeatCallback(IntPtr phandler,
            MdUserOnHeartBeatDelegate callback);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspUserLogin",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspUserLoginCallback(IntPtr phandler,
            MdUserOnRspUserLoginDelegate callback);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspUserLogout",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspUserLogoutCallback(IntPtr phandler,
            MdUserOnRspUserLogoutDelegate callback);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRtnMarketData",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRtnMarketDataCallback(IntPtr phandler,
            MdUserOnRtnMarketDataDelegate callback);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspSubMarketData",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspSubMarketDataCallback(IntPtr phandler,
            MdUserOnRspSubMarketDataDelegate callback);

        [DllImport("xcpmduserapi.dll", CharSet = CharSet.Auto, EntryPoint = "registerFP_OnRspUnsubMarketData",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterOnRspUnsubMarketDataCallback(IntPtr phandler,
            MdUserOnRspUnsubMarketDataDelegate callback);
    }
}
        