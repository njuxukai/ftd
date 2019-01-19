using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Xcp;

namespace SampleClientGui2
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }


        private void RegisterWrapperEvent()
        {
            Wrapper.onUILogAddNewLine += OnLog;
            Wrapper.onUserLogin += OnUserLogin;
            Wrapper.onUserLogout += OnUserLogout;
            Wrapper.onFundUpdate += OnFundUpdate;
            Wrapper.onPositionUpdate += OnPositionUpdate;
            Wrapper.onOrderUpdate += OnOrderUpdate;
            Wrapper.onTradeUpdate += OnTradeUpdate;
        }

        private void dataGrid_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void txtLog_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void button_login_Click(object sender, RoutedEventArgs e)
        {
            Wrapper.Connect();
        }

        private void button_reqOrderInsert_Click(object sender, RoutedEventArgs e)
        {
            
        }

        private void button_qryOrder_Click(object sender, RoutedEventArgs e)
        {
            Wrapper.ReqQryOrder();
        }

        private void button_qryPosition_Click(object sender, RoutedEventArgs e)
        {
            Wrapper.ReqQryPosition();
        }

        private void button_qryFund_Click(object sender, RoutedEventArgs e)
        {
            Wrapper.ReqQryFund();
        }

        private void button_qryTrade_Click(object sender, RoutedEventArgs e)
        {
            Wrapper.ReqQryTrade();
        }

        private void positionDataGrid_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }



        #region eventBus callback

        public delegate void EvenBusDelegate();
        public delegate void ShowFundDelegate(FundField fund);
        public delegate void ShowPositionsDelegate(List<PositionField> positions, int requestID);
        public delegate void ShowOrdersDelegate(List<OrderField> orders, int requestID);
        public delegate void ShowTradesDelegate(List<TradeField> trades,int requestID);
        private delegate void ShowLogDelegate(String content);
        public void OnLog(object sender, UILogEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new ShowLogDelegate(DoShowLog), new object[] { e.Line });
            }
            else
            {
                DoShowLog(e.Line);
            }
        }



        private void OnUserLogout(object sender, EventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new EvenBusDelegate(SetTradeButtonsOff));
            }
            else
            {
                SetTradeButtonsOff();
            }
        }
        private void OnUserLogin(object sender, Xcp.RspUserLoginEventArgs e)
        {
            Wrapper.FrontID = e.RspUserLoginField.FrontID;
            Wrapper.SessionID = e.RspUserLoginField.SessionID;
            Wrapper.NextOrderRef = e.RspUserLoginField.MaxOrderRef;
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new EvenBusDelegate(SetTradeButtonsOn));
            }
            else
            {
                SetTradeButtonsOn();
            }
        }

        private void OnFundUpdate(object sender, FundUpdateEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new ShowFundDelegate(DoShowFund), new object[] { e.Fund });
            }
            else
            {
                DoShowFund(e.Fund);
            }
        }
        public void OnPositionUpdate(object sender, PositionUpdateEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new ShowPositionsDelegate(DoShowPositions), 
                    new object[] { e.Positions, e.RequestID });
            }
            else
            {
                DoShowPositions(e.Positions, e.RequestID);
            }
        }

        public void OnOrderUpdate(object sender, OrderUpdateEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new ShowOrdersDelegate(DoShowOrders),
                    new object[] { e.Orders, e.RequestID });
            }
            else
            {
                DoShowOrders(e.Orders, e.RequestID);
            }
        }

        public void OnTradeUpdate(object sender, TradeUpdateEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new ShowTradesDelegate(DoShowTrades),
                    new object[] { e.Trades, e.RequestID });
            }
            else
            {
                DoShowTrades(e.Trades, e.RequestID);
            }
        }

        private void SetTradeButtonsOn()
        {
            button_qryFund.IsEnabled = true;
            button_qryPosition.IsEnabled = true;
            button_qryOrder.IsEnabled = true;
            button_qryTrade.IsEnabled = true;
            button_reqOrderInsert.IsEnabled = true;
        }

        private void SetTradeButtonsOff()
        {
            button_qryFund.IsEnabled = false;
            button_qryPosition.IsEnabled = false;
            button_qryOrder.IsEnabled = false;
            button_qryTrade.IsEnabled = false;
            button_reqOrderInsert.IsEnabled = false;
        }

        private void DoShowFund(FundField fund)
        {
            labelAvailable.Content = fund.AmountAvailable.ToString("N2");
            labelDrawable.Content = fund.AmountDrawable.ToString("N2");
            labelFrozen.Content = fund.AmountFrozen.ToString("N2");
            labelPre.Content = fund.AmountPre.ToString("N2");
        }

        public void DoShowPositions(List<PositionField> positions, int requestID)
        {
            List<Position> showPositions = new List<Position>();
            foreach (PositionField f in positions)
            {
                showPositions.Add(new Position(f));
            }
            positionDataGrid.DataContext = showPositions;
            DoShowLog(String.Format("OnRspQryPositon.[ReqID={0}][Count={1}]", 
                requestID, positions.Count));
        }

        public void DoShowOrders(List<OrderField> orders, int requestID)
        {
            List<Order> showOrders = new List<Order>();
            foreach (OrderField f in orders)
            {
                showOrders.Add(new Order(f));
            }
            orderDataGrid.DataContext = showOrders;
            DoShowLog(String.Format("OnRspQryOrder.[ReqID={0}][Count={1}]",
                requestID, orders.Count));
        }

        public void DoShowTrades(List<TradeField> trades, int requestID)
        {
            List<Trade> showTrades = new List<Trade>();
            foreach (TradeField f in trades)
            {
                showTrades.Add(new Trade(f));
            }
            tradeDataGrid.DataContext = showTrades;
            DoShowLog(String.Format("OnRspQryTrade.[ReqID={0}][Count={1}]",
                requestID, trades.Count));
        }


        private void DoShowLog(String content)
        {
            if (txtLog.Text.Length > 10000)
                txtLog.Text = "";
            String line = String.Format("{0:hh:mm:ss} {1}\n", DateTime.Now, content);
            txtLog.AppendText(line);
            if (IsVerticalScrollBarAtBottom)
            {
                this.txtLog.ScrollToEnd();
            }
        }

        public bool IsVerticalScrollBarAtBottom
        {
            get
            {
                bool atBottom = false;
                double dVer = this.txtLog.VerticalOffset;       //获取竖直滚动条滚动位置
                double dViewport = this.txtLog.ViewportHeight;  //获取竖直可滚动内容高度
                double dExtent = this.txtLog.ExtentHeight;      //获取可视区域的高度

                if (dVer + dViewport >= dExtent)
                {
                    atBottom = true;
                }
                else
                {
                    atBottom = false;
                }


                return atBottom;
            }
        }





        


        public TradeWrapper Wrapper { get { return wrapper; } }
        private TradeWrapper wrapper = new TradeWrapper();


        #endregion

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            SetTradeButtonsOff();
            Wrapper.InitParameter();
            RegisterWrapperEvent();
        }

    }
}
