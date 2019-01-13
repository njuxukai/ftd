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
    public delegate void OnRspUserLoginDelegate ( ref RspUserLoginField rspUserLogin, 
        ref ErrorField rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspUserLogoutDelegate ( ref RspUserLogoutField rspUserLogout, 
        ref ErrorField rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspInputOrderDelegate ( ref InputOrderField  inputOrder, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspInputOrderActionDelegate ( ref InputOrderActionField  inputOrderAction, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspInputFundTransferDelegate ( ref InputFundTransferField  inputFundTransfer, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryFundDelegate (ref FundField  fund, ref ErrorField  rspInfo, 
        int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryPositionDelegate(ref PositionField?  position, ref ErrorField  rspInfo, 
        int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryOrderDelegate (ref OrderField  pOrder, ref ErrorField  rspInfo, 
        int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryTradeDelegate (ref TradeField  pTrade, ref ErrorField  rspInfo, 
        int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryFundTransferDelegate (ref FundTransferField  fundTransfer, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryHisOrderDelegate(ref OrderField  pOrder, ref ErrorField  rspInfo, 
        int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryHisTradeDelegate(ref TradeField  pTrade, ref ErrorField  rspInfo, 
        int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryHisFundTransferDelegate(ref FundTransferField  fundTransfer, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryInstrumentDelegate(ref InstrumentField  pInstrument, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryStructuredFundDelegate(ref StructuredFundField  pStructuredFund, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryETFDelegate(ref ETFField  pETF, ref ErrorField  rspInfo, 
        int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryETFCompositionDelegate(ref ETFCompositionField  pETFComposition, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryPurchasableNewIssueSecurityDelegate(ref NewIssueSecurityField  newIssueSecurity, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRspQryPurchaseQuotaDelegate(ref PurchaseQuotaField  pPurchaseQuota, 
        ref ErrorField  rspInfo, int nRequestID, bool isLast);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRtnOrderExecutionReportDelegate (ref ExecutionReportField pExecutionReport);


    
}
