using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Xcp
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void MdUserOnFrontConnectedDelegate ();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void MdUserOnFrontDisconnectedDelegate(int nReason);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void MdUserOnHeartBeatWarningDelegate(int nTimeLapse);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void MdUserOnHeartBeatDelegate();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void MdUserOnRspUserLoginDelegate( IntPtr rspUserLogin,
        IntPtr rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void MdUserOnRspUserLogoutDelegate( IntPtr rspUserLogout, 
        IntPtr rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void MdUserOnRtnMarketDataDelegate ( IntPtr  pMarketData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void MdUserOnRspSubMarketDataDelegate(IntPtr pInstrument, 
        IntPtr pErrorInfo, int reqID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void MdUserOnRspUnsubMarketDataDelegate(IntPtr pInstrument,
        IntPtr pErrorInfo, int reqID, [MarshalAs(UnmanagedType.U1)]bool isLast);


}
