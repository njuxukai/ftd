﻿using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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
using Xcp.Enums;

namespace SampleClientGui
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
            try
            {
                InputOrderField order = new InputOrderField();
                order.InstrumentCode = textBoxInstrumentCode.Text;
                order.ExchangeType = (char)((ExchangeType)comboBoxExchangeType.SelectedItem);
                order.Direction = (char)((Direction)comboBoxDirection.SelectedItem);
                order.PriceType = (char)((OrderPriceType)comboBoxPriceType.SelectedItem);
                order.LimitPrice = double.Parse(textBoxPrice.Text);
                order.VolumeTotalOrginal = int.Parse(textBoxVolume.Text);
                Wrapper.ReqOrderInsert(order);
            }
            catch (Exception ex)
            {
                DoShowLog(ex.ToString());
            }
            
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



        #region TradeWrapper callback

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
            DoShowLog(String.Format("DoShowOrders.[ReqID={0}][Count={1}]",
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
            ConfigureCustomUI();


        }

        private void ConfigureCustomUI()
        {
            DataObject.AddPastingHandler(textBoxPrice, OnPaste);
            DataObject.AddPastingHandler(textBoxVolume, OnPaste);
            //comboBoxDirection.ItemsSource = Enum.GetValues(typeof(Xcp.Enums.Direction)).Cast<Xcp.Enums.Direction>();
            //comboBoxPriceType.ItemsSource = Enum.GetValues(typeof(Xcp.Enums.OrderPriceType)).Cast<Xcp.Enums.Direction>();
            //comboBoxExchangeType.ItemsSource = Enum.GetValues(typeof(Xcp.Enums.ExchangeType)).Cast<Xcp.Enums.ExchangeType>();
            comboBoxExchangeType.SelectedItem = Xcp.Enums.ExchangeType.SH;
            comboBoxDirection.SelectedItem = Xcp.Enums.Direction.BUY;
            comboBoxPriceType.SelectedItem = Xcp.Enums.OrderPriceType.HS_Limit;
        }
        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            Order order = (Order)orderDataGrid.SelectedItem;
            InputOrderActionField action = new InputOrderActionField();
            action.FrontID = order.FrontID;
            action.SessionID = order.SessionID;
            action.OrderRef = order.OrderRef;
            action.OrderSysID = order.OrderSysID;
            Wrapper.ReqOrderAction(action);
        }

        private void orderDataGrid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Right)
            {
                Point pt = e.GetPosition(orderDataGrid);
                DataGridCell orderCell = null;
                //Do the hittest to find the DataGridCell
                VisualTreeHelper.HitTest(orderDataGrid, null, (result) =>
                    {
                        // Find the ancestor element form the hittested element
                        // e.g., find the DataGridCell if we hittest on the inner TextBlock
                        DataGridCell cell = FindVisualParent<DataGridCell>(result.VisualHit);
                        if (cell != null)
                        {
                            orderCell = cell;
                            return HitTestResultBehavior.Stop;
                        }
                        else
                            return HitTestResultBehavior.Continue;
                    },
                    new PointHitTestParameters(pt));

                if (orderCell != null)
                {

                    int rowIndex = DataGridRow.GetRowContainingElement(orderCell).GetIndex();
                    orderDataGrid.SelectedIndex = rowIndex;
                }
            }
        }

        private T FindVisualParent<T>(DependencyObject child) where T : DependencyObject
        {
            DependencyObject parentObject = VisualTreeHelper.GetParent(child);
            if (parentObject == null) return null;
            T parent = parentObject as T;
            if (parent != null)
                return parent;
            else
                return FindVisualParent<T>(parentObject);
        }

        private void NumericTextboxPreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !IsTextAllowed(e.Text);
        }

        private static readonly Regex _regex = new Regex("[^0-9.-]+"); //regex that matches disallowed text
        private static bool IsTextAllowed(string text)
        {
            return !_regex.IsMatch(text);
        }

        private void OnPaste(object sender, DataObjectPastingEventArgs e)
        {
            if (e.DataObject.GetDataPresent(typeof(String)))
            {
                String text = (String)e.DataObject.GetData(typeof(String));
                if (!IsTextAllowed(text))
                {
                    e.CancelCommand();
                }
            }
            else
            {
                e.CancelCommand();
            }
        }
    }
}
