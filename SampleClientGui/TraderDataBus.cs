using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SampleClientGui
{
    public class  UILogEventArgs : EventArgs
    {
        public UILogEventArgs(string content)
        {
            m_line = String.Format("{mm:ss} {1}\n", DateTime.Now, content);

        }
        public string Line {
            get { return m_line; } }
        private string m_line;
    }
    public class FundUpdateEventArgs : EventArgs
    {
        public FundUpdateEventArgs(Xcp.FundField fund)
        {
            m_fund = fund;
        }

        public Xcp.FundField Fund{ get { return m_fund ; } }
        Xcp.FundField m_fund;
    }

    public class PositionUpdateEventArgs : EventArgs
    {
        public PositionUpdateEventArgs(List<Xcp.PositionField> positions)
        {
            m_positions = positions;
        }

        public List<Xcp.PositionField> Fund { get { return m_positions; } }
        List<Xcp.PositionField> m_positions;
    }

    public class OrderUpdateEventArgs : EventArgs
    {
        public OrderUpdateEventArgs(List<Xcp.OrderField> orders)
        {
            m_orders = orders;
        }

        public List<Xcp.OrderField> Fund { get { return m_orders; } }
        List<Xcp.OrderField> m_orders;
    }

    public class TradeUpdateEventArgs : EventArgs
    {
        public TradeUpdateEventArgs(List<Xcp.TradeField> trades)
        {
            m_trades = trades;
        }

        public List<Xcp.TradeField> Fund { get { return m_trades; } }
        List<Xcp.TradeField> m_trades;
    }

    public class TraderEventBus
    {

        public void Connect(Xcp.Trader trader)
        {
            trader.onRspQryFund += OnRspQryFund;
            trader.onRspQryPosition += OnRspQryPosition;
            trader.onRspQryTrade += OnRspQryTrade;
            trader.onRspQryOrder += OnRspQryOrder;
        }

        public void Disconnect(Xcp.Trader trader)
        {
            trader.onRspQryFund -= OnRspQryFund;
            trader.onRspQryPosition -= OnRspQryPosition;
            trader.onRspQryTrade -= OnRspQryTrade;
            trader.onRspQryOrder -= OnRspQryOrder;
        }
        #region callback for trader
        private void OnFrontConnected(object sender, EventArgs eventArgs)
        {
            RaiseUILogAddNewLine("前置连接成功");
        }

        private void OnRspUserLogin(object sender, Xcp.RspUserLoginEventArgs e)
        {
            string line = "";
            if (e.ErrorField.HasValue && e.ErrorField.Value.ErrorCode != 0)
            {
                line = String.Format("登陆失败[{0}][{1}]", e.ErrorField.Value.ErrorCode,
                    e.ErrorField.Value.ErrorText);
            }
            else
            {
                line = "登录失败";
            }
            RaiseUILogAddNewLine(line);
        }

        private void OnRspQryFund(object sender, Xcp.RspQryFundEventArgs eventArgs)
        {
            RaiseUILogAddNewLine("OnRspQryFund");
            currentFund =  eventArgs.FundField.GetValueOrDefault();
            if (eventArgs.IsLast)
            {
                RaiseFundUpdate();
            }
        }

        private void OnRspQryPosition(object sender, Xcp.RspQryPositionEventArgs eventArgs)
        {
            if (eventArgs.RequestID != receivePositionRequestID)
            {
                receivePositionBuffer.Clear();
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
            if (eventArgs.RequestID != receiveTradeRequestID)
            {
                receiveTradeBuffer.Clear();
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
            if (eventArgs.RequestID != receiveOrderRequestID)
            {
                receiveOrderBuffer.Clear();
            }
            if (eventArgs.OrderField.HasValue)
            {
                receiveOrderBuffer.Add(eventArgs.OrderField.Value);
            }
            if (eventArgs.IsLast)
            {
                currentOrderDict.Clear();
                foreach(var order in receiveOrderBuffer)
                {
                    string key = string.Format("{0}:{1}:{2}", order.FrontID, order.SessionID, order.OrderRef);
                    currentOrderDict.Add(key, order);
                }
                RaiseOrderUpdate();
            }            
        }

        private void onRtnExecutionReport(object sender, Xcp.RtnOrderExecutionEventArgs eventArgs )
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
                order.ExchangeCode = report.ExchangeCode;
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
        private void RaiseFundUpdate()
        {
            FundUpdateEventArgs args = new FundUpdateEventArgs(currentFund);
            Volatile.Read(ref onFundUpdate)?.Invoke(this, args);
        }

        private void RaisePositionUpdate()
        {
            PositionUpdateEventArgs args = new PositionUpdateEventArgs(currentPositions);
            Volatile.Read(ref onPositionUpdate)?.Invoke(this, args);
        }

        private void RaiseOrderUpdate()
        {
            OrderUpdateEventArgs eventArgs = new OrderUpdateEventArgs(currentOrderDict.Values.ToList());
            Volatile.Read(ref onOrderUpdate)?.Invoke(this, eventArgs);
        }

        private void RaiseTradeUpdate()
        {
            TradeUpdateEventArgs args = new TradeUpdateEventArgs(currentTrades);
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
        public event EventHandler<UILogEventArgs> onUILogAddNewLine;
        public event EventHandler<FundUpdateEventArgs> onFundUpdate;
        public event EventHandler<PositionUpdateEventArgs> onPositionUpdate;
        public event EventHandler<OrderUpdateEventArgs> onOrderUpdate;
        public event EventHandler<TradeUpdateEventArgs> onTradeUpdate;
        #endregion
    }
}
