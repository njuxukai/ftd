using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Xcp;

namespace SampleClientGui
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

    public class TradeWrapper
    {
        public TradeWrapper()
        {
        }

        public void InitParameter()
        {
            var section = ConfigurationManager.GetSection("USER_INFO") as NameValueCollection;

            BrokerID = int.Parse(section["BrokerID"]);
            UserID = int.Parse(section["UserID"]);
            Password = section["Password"];
            FrontAddress = section["FrontAddress"];
            FrontAddress2 = section["FrontAddress2"];
            InvestorID = UserID;
        }
        private void ConnectTraderEvent(Xcp.Trader trader)
        {
            trader.onFrontConnected += OnFrontConnected;
            trader.onFrontDisconnected += OnFrontDisconnected;
            trader.onRspUserLogin += OnRspUserLogin;
            trader.onHeartBeat += OnHeartBeat;
            trader.onRspQryFund += OnRspQryFund;
            trader.onRspQryPosition += OnRspQryPosition;
            trader.onRspQryTrade += OnRspQryTrade;
            trader.onRspQryOrder += OnRspQryOrder;
            trader.onRspQrySecurityAccount += OnRspQrySecurityAccount;
            trader.onRspOrderInsert += OnRspOrderInsert;
            trader.onRspOrderAction += OnRspOrderAction;
            
        }

        private void DisconnectTraderEvent(Xcp.Trader trader)
        {
            trader.onFrontConnected -= OnFrontConnected;
            trader.onFrontDisconnected -= OnFrontDisconnected;
            trader.onRspUserLogin -= OnRspUserLogin;
            trader.onHeartBeat -= OnHeartBeat;
            trader.onRspQryFund -= OnRspQryFund;
            trader.onRspQryPosition -= OnRspQryPosition;
            trader.onRspQryTrade -= OnRspQryTrade;
            trader.onRspQryOrder -= OnRspQryOrder;
            trader.onRspQrySecurityAccount -= OnRspQrySecurityAccount;
            trader.onRspOrderInsert -= OnRspOrderInsert;
            trader.onRspOrderAction -= OnRspOrderAction;
        }
        #region callback for trader

        private void OnFrontConnected(object sender, EventArgs e)
        {
            RaiseUILogAddNewLine("前置会话连接成功");
            NextRequestID = 1;
            ReqUserLoginField field = new ReqUserLoginField();
            field.BrokerID = BrokerID;
            field.UserID = UserID;
            field.Password = Password;
            if (m_trader != null)
                m_trader.ReqUserLogin(field, NextRequestID);
            NextRequestID++;
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
                line = String.Format("登录成功,FrontID={0},SessionID={1},MaxOrderRef={2},HeartBeat={3},TradingDay={4}",
                    e.RspUserLoginField.FrontID,
                    e.RspUserLoginField.SessionID,
                    e.RspUserLoginField.MaxOrderRef,
                    e.RspUserLoginField.HeartbeatInterval,
                    e.RspUserLoginField.TradingDay);
                RaiseUILogAddNewLine(line);

                QrySecurityAccountField field = new QrySecurityAccountField
                {
                    BrokerID = BrokerID,
                    InvestorID = InvestorID,
                    UserID = UserID,
                    RequestID = NextRequestID
                };

                m_trader.ReqQrySecurityAccount(field, NextRequestID);
                NextRequestID++;
                RaiseUserLogin(e);
            }
            
        }

        private void OnRspUserLogout(object sender, Xcp.RspUserLogoutEventArgs e)
        {
            RaiseUserLogout();
            RaiseUILogAddNewLine("用户已登出");
        }

        private void OnRspQrySecurityAccount(object sender, Xcp.RspQrySecurityAccountEventArgs e)
        {
            if (e.ErrorField.HasValue && e.ErrorField.Value.ErrorCode != 0)
            {
                RaiseUILogAddNewLine(String.Format("OnRspQryFund Error[{0}][{1}].", e.ErrorField.Value.ErrorCode, e.ErrorField.Value.ErrorText));
                return;
            }
            if (e.SecurityAccountField.HasValue)
            {
                if (securityAccounts.ContainsKey(e.SecurityAccountField.Value.ExchangeType))
                {
                    securityAccounts[e.SecurityAccountField.Value.ExchangeType] =
                        e.SecurityAccountField.Value.SecurityAccount;
                }
                else
                {
                    securityAccounts.Add(e.SecurityAccountField.Value.ExchangeType,
                        e.SecurityAccountField.Value.SecurityAccount);
                }
            }
            
            if (e.IsLast)
            {
                RaiseUILogAddNewLine(String.Format("OnRspQrySecurityAccount.[ReqID={0}]",
                    e.RequestID));
            }
        }

        private void OnRspOrderInsert(object sender, Xcp.RspOrderInsertEventArgs e)
        {
            if (e.ErrorField.HasValue && e.ErrorField.Value.ErrorCode != 0)
            {
                RaiseUILogAddNewLine(
                    String.Format(
                    "OnRspOrderInsert[ReqId={0}] Error.[{1}][{2}]",
                    e.RequestID,
                    e.ErrorField.Value.ErrorCode, e.ErrorField.Value.ErrorText));
            }
            else
            {
                RaiseUILogAddNewLine(
                    String.Format(
                    "OnRspOrderInsert[ReqId={0}] Succeed.",
                    e.RequestID));
            }
        }

        private void OnRspOrderAction(object sender, Xcp.RspOrderActionEventArgs e)
        {
            if (e.ErrorField.HasValue && e.ErrorField.Value.ErrorCode != 0)
            {
                RaiseUILogAddNewLine(
                    String.Format(
                    "OnRspOrderAction[ReqId={0}] Error.[{1}][{2}]",
                    e.RequestID,
                    e.ErrorField.Value.ErrorCode, e.ErrorField.Value.ErrorText));
            }
            else
            {
                RaiseUILogAddNewLine(
                    String.Format(
                    "OnRspOrderAction[ReqId={0}] Succeed.",
                    e.RequestID));
            }
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
                receiveOrderBuffer.Clear();
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
            order.VolumeCum = report.VolumeCum;
            order.AmountCum = report.AmountCum;
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

        public void Connect()
        {
            if (m_trader != null)
            {
                DisconnectTraderEvent(m_trader);
                m_trader.Release();
            }                
            m_trader = new Trader();
            ConnectTraderEvent(m_trader);

            m_trader.RegisterFront(FrontAddress);
            m_trader.RegisterFront(FrontAddress2);
            m_trader.SubscribePrivateTopic(THOST_TE_RESUME_TYPE.THOST_TERT_RESTART);
            m_trader.SubscribePublicTopic(THOST_TE_RESUME_TYPE.THOST_TERT_RESTART);
            m_trader.Init();
        }

        public void Release()
        {
            if (m_trader != null)
            {
                m_trader.Release();
            }
        }

        public int BrokerID { get; set; }
        public int UserID { get; set; }
        public int InvestorID { get; set; }
        public string Password { get; set; }
        public string FrontAddress { get; set; }
        public string FrontAddress2 { get; set; }
        public Int32 NextRequestID { get; set; }
        public Int32 NextOrderRef { get; set; }
        public Int32 FrontID { get; set; }
        public Int32 SessionID { get; set; }

        private Dictionary<char, String> securityAccounts = new Dictionary<char, string>();
        private Xcp.Trader m_trader;

        public void ReqQryFund()
        {
            if (m_trader != null)
            {
                Xcp.QryFundField qry = new QryFundField();
                qry.BrokerID = BrokerID;
                qry.UserID = UserID;
                qry.InvestorID = InvestorID;
                qry.RequestID = NextRequestID;
                qry.CurrencyType = (char)Xcp.Enums.CurrencyType.RMB;
                int returnValue = m_trader.ReqQryFund(qry, NextRequestID);
                RaiseUILogAddNewLine(String.Format("ReqQryFund.[ReqID={0}][Rtn={1}]",
                    NextRequestID, returnValue));
                NextRequestID++;
            }
        }

        public void ReqQryPosition()
        {
            if (m_trader != null)
            {
                Xcp.QryPositionField qry = new QryPositionField();
                qry.BrokerID = BrokerID;
                qry.UserID = UserID;
                qry.InvestorID = InvestorID;
                qry.RequestID = NextRequestID;
                int returnValue = m_trader.ReqQryPosition(qry, NextRequestID);
                RaiseUILogAddNewLine(String.Format("ReqQryPosition.[ReqID={0}][Rtn={1}]",
                    NextRequestID, returnValue));
                NextRequestID++;
            }
        }


        public void ReqQryOrder()
        {
            if (m_trader != null)
            {
                Xcp.QryOrderField qry = new QryOrderField();
                qry.BrokerID = BrokerID;
                qry.UserID = UserID;
                qry.InvestorID = InvestorID;
                qry.RequestID = NextRequestID;
                int returnValue = m_trader.ReqQryOrder(qry, NextRequestID);
                RaiseUILogAddNewLine(String.Format("ReqQryOrder.[ReqID={0}][Rtn={1}]",
                    NextRequestID, returnValue));
                NextRequestID++;
            }
        }

        public void ReqQryTrade()
        {
            if (m_trader != null)
            {
                Xcp.QryTradeField qry = new QryTradeField();
                qry.BrokerID = BrokerID;
                qry.UserID = UserID;
                qry.InvestorID = InvestorID;
                qry.RequestID = NextRequestID;
                int returnValue = m_trader.ReqQryTrade(qry, NextRequestID);
                RaiseUILogAddNewLine(String.Format("ReqQryTrade.[ReqID={0}][Rtn={1}]",
                    NextRequestID, returnValue));
                NextRequestID++;
            }
        }

        public void ReqOrderAction(InputOrderActionField action)
        {
            if (m_trader != null)
            {
                action.BrokerID = BrokerID;
                action.UserID = UserID;
                action.InvestorID = InvestorID;
                action.RequestID = NextRequestID;
                int returnValue = m_trader.ReqOrderAction(action, NextRequestID);
                RaiseUILogAddNewLine(String.Format("ReqOrderAction.[ReqID={0}][Rtn={1}]",
                    NextRequestID, returnValue));
                NextRequestID++;
            }
        }

        public void ReqOrderInsert(InputOrderField order)
        {
            if (m_trader != null)
            {
                order.BrokerID = BrokerID;
                order.UserID = UserID;
                order.InvestorID = InvestorID;
                order.RequestID = NextRequestID;
                order.OrderRef = NextOrderRef;
                //order.OrderRef = 1;
                order.FrontID = FrontID;
                order.SessionID = SessionID;
                if(securityAccounts.ContainsKey(order.ExchangeType))
                    order.SecurityAccount = securityAccounts[order.ExchangeType];
                int returnValue = m_trader.ReqOrderInsert(order, NextRequestID);
                RaiseUILogAddNewLine(String.Format("ReqOrderInsert.[ReqID={0}][Rtn={1}]",
                    NextRequestID, returnValue));
                NextRequestID++;
                NextOrderRef++;
            }
        }

        

        



    }
}
