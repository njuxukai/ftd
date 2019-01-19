using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SampleClientGui2
{
    public class UILogEventArgs : EventArgs
    {
        public UILogEventArgs(string content)
        {
            m_line = content;// String.Format("{0:mm:ss} {1}\n", DateTime.Now, content);

        }
        public string Line
        {
            get { return m_line; }
        }
        private string m_line;
    }
    public class FundUpdateEventArgs : EventArgs
    {
        public FundUpdateEventArgs(Xcp.FundField fund)
        {
            m_fund = fund;
        }

        public Xcp.FundField Fund { get { return m_fund; } }
        Xcp.FundField m_fund;
    }

    public class PositionUpdateEventArgs : EventArgs
    {
        public PositionUpdateEventArgs(List<Xcp.PositionField> positions, int requestID)
        {
            m_positions = positions;
            m_requestID = requestID;
        }

        public List<Xcp.PositionField> Positions { get { return m_positions; } }
        public int RequestID { get { return m_requestID; } }
        List<Xcp.PositionField> m_positions;
        int m_requestID;
    }

    public class OrderUpdateEventArgs : EventArgs
    {
        public OrderUpdateEventArgs(List<Xcp.OrderField> orders, int requestID)
        {
            m_orders = orders;
            m_requestID = requestID;
        }

        public List<Xcp.OrderField> Orders { get { return m_orders; } }
        public int RequestID { get { return m_requestID; } }
        List<Xcp.OrderField> m_orders;
        int m_requestID;
    }

    public class TradeUpdateEventArgs : EventArgs
    {
        public TradeUpdateEventArgs(List<Xcp.TradeField> trades, int requestID)
        {
            m_trades = trades;
            m_requestID = requestID;
        }

        public List<Xcp.TradeField> Trades { get { return m_trades; } }
        public int RequestID { get { return m_requestID; } }
        List<Xcp.TradeField> m_trades;
        int m_requestID;
    }

    public class TraderEventBus
    {
        public TraderEventBus()
        {
        }
        public void Connect(Xcp.Trader trader)
        {
            trader.onFrontConnected += OnFrontConnected;
            trader.onFrontDisconnected += OnFrontDisconnected;
            trader.onRspUserLogin += OnRspUserLogin;
            trader.onHeartBeat += OnHeartBeat;
            trader.onRspQryFund += OnRspQryFund;
            trader.onRspQryPosition += OnRspQryPosition;
            trader.onRspQryTrade += OnRspQryTrade;
            trader.onRspQryOrder += OnRspQryOrder;
        }

        public void Disconnect(Xcp.Trader trader)
        {
            trader.onFrontConnected -= OnFrontConnected;
            trader.onFrontDisconnected -= OnFrontDisconnected;
            trader.onRspUserLogin -= OnRspUserLogin;
            trader.onHeartBeat -= OnHeartBeat;
            trader.onRspQryFund -= OnRspQryFund;
            trader.onRspQryPosition -= OnRspQryPosition;
            trader.onRspQryTrade -= OnRspQryTrade;
            trader.onRspQryOrder -= OnRspQryOrder;
        }
        #region callback for trader
        private void OnFrontConnected(object sender, EventArgs eventArgs)
        {
            RaiseUILogAddNewLine("前置会话连接成功");
        }

        private void OnFrontDisconnected(object sender, Xcp.FrontDisconnectedEventArgs e)
        {
            RaiseUILogAddNewLine(String.Format("前置会话断开[{0}]", e.Reason));
            RaiseUserLogout();
        }
        private void OnHeartBeat(object sender, EventArgs eventArgs)
        {
            RaiseUILogAddNewLine("收到心跳");
        }

        private void OnRspUserLogin(object sender, Xcp.RspUserLoginEventArgs e)
        {
            string line = "";
            if (e.ErrorField.HasValue && e.ErrorField.Value.ErrorCode != 0)
            {
                line = String.Format("登录失败[{0}][{1}]", e.ErrorField.Value.ErrorCode,
                    e.ErrorField.Value.ErrorText);
                RaiseUILogAddNewLine(line);
            }
            else
            {
                line = String.Format("登录成功,FrontID={0},SessionID={1},MaxOrderRef={2},HeartBeat={3}",
                    e.RspUserLoginField.FrontID,
                    e.RspUserLoginField.SessionID,
                    e.RspUserLoginField.MaxOrderRef,
                    e.RspUserLoginField.HeartbeatInterval);
                RaiseUILogAddNewLine(line);
                RaiseUserLogin(e);
            }
            
        }

        private void OnRspUserLogout(object sender, Xcp.RspUserLogoutEventArgs e)
        {
            RaiseUserLogout();
            RaiseUILogAddNewLine("用户已登出");
        }

        private void OnRspQryFund(object sender, Xcp.RspQryFundEventArgs e)
        {
            if (e.ErrorField.HasValue && e.ErrorField.Value.ErrorCode != 0)
            {
                RaiseUILogAddNewLine(String.Format("OnRspQryFund Error[{0}][{1}].", e.ErrorField.Value.ErrorCode, e.ErrorField.Value.ErrorText));
                return;
            }

            string line = String.Format("OnRspQryFund.[ReqID={0}] Succeed.", e.RequestID);
            RaiseUILogAddNewLine(line);

            if (e.FundField.HasValue && 
                e.FundField.Value.CurrencyType == (char)Xcp.Enums.CurrencyType.RMB)
            {
                currentFund = e.FundField.Value;
                RaiseFundUpdate();
            }
        }

        private void OnRspQryPosition(object sender, Xcp.RspQryPositionEventArgs eventArgs)
        {
            if (eventArgs.ErrorField.HasValue && eventArgs.ErrorField.Value.ErrorCode != 0)
            {
                RaiseUILogAddNewLine(String.Format("OnRspQryFund Error.[{0}][{1}].", 
                    eventArgs.ErrorField.Value.ErrorCode, eventArgs.ErrorField.Value.ErrorText));
                return;
            }

            if (eventArgs.RequestID != receivePositionRequestID)
            {
                receivePositionBuffer.Clear();
                receivePositionRequestID = eventArgs.RequestID;
            }
            if (eventArgs.PositionField.HasValue)
                receivePositionBuffer.Add(eventArgs.PositionField.Value);

            if (eventArgs.IsLast)
            {
                currentPositions.Clear();
                currentPositions.AddRange(receivePositionBuffer);
                receivePositionBuffer.Clear();

                RaisePositionUpdate();
            }
        }

        private void OnRspQryTrade(object sender, Xcp.RspQryTradeEventArgs eventArgs)
        {
            if (eventArgs.ErrorField.HasValue && eventArgs.ErrorField.Value.ErrorCode != 0)
            {
                RaiseUILogAddNewLine(String.Format("OnRspQryTrade Error.[{0}][{1}].",
                    eventArgs.ErrorField.Value.ErrorCode, eventArgs.ErrorField.Value.ErrorText));
                return;
            }
            if (eventArgs.RequestID != receiveTradeRequestID)
            {
                receiveTradeBuffer.Clear();
                receiveTradeRequestID = eventArgs.RequestID;
            }
            if (eventArgs.TradeField.HasValue)
                receiveTradeBuffer.Add(eventArgs.TradeField.Value);

            if (eventArgs.IsLast)
            {
                currentTrades.Clear();
                currentTrades.AddRange(receiveTradeBuffer);
                receiveTradeBuffer.Clear();

                RaiseTradeUpdate();
            }
        }

        private void OnRspQryOrder(object sender, Xcp.RspQryOrderEventArgs eventArgs)
        {
            if (eventArgs.ErrorField.HasValue && eventArgs.ErrorField.Value.ErrorCode != 0)
            {
                RaiseUILogAddNewLine(String.Format("OnRspQryOrder Error.[{0}][{1}].",
                    eventArgs.ErrorField.Value.ErrorCode, eventArgs.ErrorField.Value.ErrorText));
                return;
            }
            if (eventArgs.RequestID != receiveOrderRequestID)
            {
                receiveOrderBuffer.Clear();
                receiveOrderRequestID = eventArgs.RequestID;
            }
            if (eventArgs.OrderField.HasValue)
            {
                receiveOrderBuffer.Add(eventArgs.OrderField.Value);
            }
            if (eventArgs.IsLast)
            {
                currentOrderDict.Clear();
                foreach (var order in receiveOrderBuffer)
                {
                    string key = string.Format("{0}:{1}:{2}", order.FrontID, order.SessionID, order.OrderRef);
                    currentOrderDict.Add(key, order);
                }
                RaiseOrderUpdate();
            }
        }

        private void onRtnExecutionReport(object sender, Xcp.RtnOrderExecutionEventArgs eventArgs)
        {
            Xcp.ExecutionReportField report = eventArgs.ExecutionReportField;
            Xcp.OrderField order;
            string key = String.Format("{0}:{1}:{2}", report.FrontID, report.SessionID, report.OrderRef);
            if (!currentOrderDict.ContainsKey(key))
            {
                order = new Xcp.OrderField();
                updateOrder(ref report, ref order, false);
            }
            else
            {
                order = currentOrderDict[key];
                updateOrder(ref report, ref order, true);
            }
            currentOrderDict[key] = order;
            RaiseOrderUpdate();
        }

        static void updateOrder(ref Xcp.ExecutionReportField report, ref Xcp.OrderField order, bool isAll)
        {
            //必须更新的部分
            order.VolumeTraded = report.VolumeTraded;
            order.AmountTraded = report.AmountTraded;
            order.OrderStatus = report.OrderStatus;
            order.OrderSysID = report.OrderSysID;
            order.OrderExchangeID = report.OrderExchangeID;
            //order.
            if (isAll)
            {
                order.InstrumentCode = report.InstrumentCode;
                order.ExchangeType = report.ExchangeType;
                order.FrontID = report.FrontID;
                order.SessionID = report.SessionID;
                order.OrderRef = report.OrderRef;
                order.Direction = report.Direction;
                order.VolumeTotalOrginal = report.VolumeTotalOrginal;
                order.InvestorID = report.InvestorID;
            }
        }

        #endregion


        #region EventRaiser
        private void RaiseUILogAddNewLine(string content)
        {
            UILogEventArgs args = new UILogEventArgs(content);
            Volatile.Read(ref onUILogAddNewLine)?.Invoke(this, args);
        }

        private void RaiseUserLogin(Xcp.RspUserLoginEventArgs e)
        {
            Volatile.Read(ref onUserLogin)?.Invoke(this, e);
        }

        private void RaiseUserLogout()
        {
            Volatile.Read(ref onUserLogout)?.Invoke(this, EventArgs.Empty);
        }

        private void RaiseFundUpdate()
        {
            FundUpdateEventArgs args = new FundUpdateEventArgs(currentFund);
            Volatile.Read(ref onFundUpdate)?.Invoke(this, args);
        }

        private void RaisePositionUpdate()
        {
            PositionUpdateEventArgs args = new PositionUpdateEventArgs(currentPositions, receivePositionRequestID);
            Volatile.Read(ref onPositionUpdate)?.Invoke(this, args);
        }

        private void RaiseOrderUpdate()
        {
            OrderUpdateEventArgs eventArgs = new OrderUpdateEventArgs(currentOrderDict.Values.ToList(),
                receiveOrderRequestID);
            Volatile.Read(ref onOrderUpdate)?.Invoke(this, eventArgs);
        }

        private void RaiseTradeUpdate()
        {
            TradeUpdateEventArgs args = new TradeUpdateEventArgs(currentTrades, receiveTradeRequestID);
            Volatile.Read(ref onTradeUpdate)?.Invoke(this, args);
        }
        #endregion

        private Xcp.FundField currentFund;

        private List<Xcp.PositionField> currentPositions = new List<Xcp.PositionField>();
        private List<Xcp.PositionField> receivePositionBuffer = new List<Xcp.PositionField>();
        private int receivePositionRequestID = 0;

        private Dictionary<String, Xcp.OrderField> currentOrderDict = new Dictionary<String, Xcp.OrderField>();
        private List<Xcp.OrderField> receiveOrderBuffer = new List<Xcp.OrderField>();
        private int receiveOrderRequestID = 0;

        private List<Xcp.TradeField> currentTrades = new List<Xcp.TradeField>();
        private List<Xcp.TradeField> receiveTradeBuffer = new List<Xcp.TradeField>();
        private int receiveTradeRequestID = 0;

        #region 
        public event EventHandler<Xcp.RspUserLoginEventArgs> onUserLogin;
        public event EventHandler<EventArgs> onUserLogout;
        public event EventHandler<UILogEventArgs> onUILogAddNewLine;
        public event EventHandler<FundUpdateEventArgs> onFundUpdate;
        public event EventHandler<PositionUpdateEventArgs> onPositionUpdate;
        public event EventHandler<OrderUpdateEventArgs> onOrderUpdate;
        public event EventHandler<TradeUpdateEventArgs> onTradeUpdate;
        #endregion


    }
}
