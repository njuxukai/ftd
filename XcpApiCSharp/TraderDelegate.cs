using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Xcp
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnFrontConnectedDelegate ();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnFrontDisconnectedDelegate(int nReason);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnHeartBeatWarningDelegate(int nTimeLapse);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnHeartBeatDelegate();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspUserLoginDelegate ( IntPtr rspUserLogin,
        IntPtr rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspUserLogoutDelegate ( IntPtr rspUserLogout, 
        IntPtr rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspInputOrderDelegate ( IntPtr  inputOrder, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspInputOrderActionDelegate ( IntPtr  inputOrderAction, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspInputFundTransferDelegate ( IntPtr  inputFundTransfer, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryFundDelegate (IntPtr  fund, IntPtr  rspInfo, 
        int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryPositionDelegate(IntPtr   position, IntPtr  rspInfo, 
        int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryOrderDelegate (IntPtr  pOrder, IntPtr  rspInfo, 
        int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryTradeDelegate (IntPtr  pTrade, IntPtr  rspInfo, 
        int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryFundTransferDelegate (IntPtr  fundTransfer, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryHisOrderDelegate(IntPtr  pOrder, IntPtr  rspInfo, 
        int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryHisTradeDelegate(IntPtr  pTrade, IntPtr  rspInfo, 
        int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryHisFundTransferDelegate(IntPtr  fundTransfer, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryInstrumentDelegate(IntPtr  pInstrument, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryStructuredFundDelegate(IntPtr  pStructuredFund, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryETFDelegate(IntPtr  pETF, IntPtr  rspInfo, 
        int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryETFCompositionDelegate(IntPtr  pETFComposition, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryPurchasableNewIssueSecurityDelegate(IntPtr  newIssueSecurity, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryPurchaseQuotaDelegate(IntPtr  pPurchaseQuota, 
        IntPtr  rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRtnOrderExecutionReportDelegate (IntPtr pExecutionReport);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQrySecurityAccountDelegate(IntPtr pSecurityAccount,
        IntPtr rspInfo, int nRequestID, [MarshalAs(UnmanagedType.U1)]bool isLast);

}
