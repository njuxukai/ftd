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

           

            //trader = new Trader();
            //trader.Init();
        }


        private void RegisterCallback()
        {
            eventBus.onUILogAddNewLine += OnLog;
        }

        private void dataGrid_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void txtLog_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void button_login_Click(object sender, RoutedEventArgs e)
        {
            CreateTrader();

            ConnetTrader();
        }

        private void button_reqOrderInsert_Click(object sender, RoutedEventArgs e)
        {
            if (trader != null)
            { }
        }

        private void button_qryOrder_Click(object sender, RoutedEventArgs e)
        {
            if (trader != null)
            {
                Xcp.QryOrderField qry = new QryOrderField();
                qry.BrokerID = BrokerID;
                qry.UserID = UserID;
                qry.InvestorID = InvestorID;
                qry.RequestID = NextRequestID;
                trader.ReqQryOrder(qry, NextRequestID++); 
            }
        }

        private void button_qryPosition_Click(object sender, RoutedEventArgs e)
        {
            if (trader != null)
            {
                Xcp.QryPositionField qry = new QryPositionField();
                qry.BrokerID = BrokerID;
                qry.UserID = UserID;
                qry.InvestorID = InvestorID;
                qry.RequestID = NextRequestID;
                trader.ReqQryPosition(qry, NextRequestID++);
            }
        }

        private void button_qryFund_Click(object sender, RoutedEventArgs e)
        {
            if (trader != null)
            {
                Xcp.QryFundField qry = new QryFundField();
                qry.BrokerID = BrokerID;
                qry.UserID = UserID;
                qry.InvestorID = InvestorID;
                qry.RequestID = NextRequestID;
                qry.CurrencyType = (char)Xcp.Enums.CurrencyType.RMB;
                trader.ReqQryFund(qry, NextRequestID++);
            }
        }

        private void button_qryTrade_Click(object sender, RoutedEventArgs e)
        {
            if (trader != null)
            {
                Xcp.QryTradeField qry = new QryTradeField();
                qry.BrokerID = BrokerID;
                qry.UserID = UserID;
                qry.InvestorID = InvestorID;
                qry.RequestID = NextRequestID;
                trader.ReqQryTrade(qry, NextRequestID++);
            }
        }

        private void positionDataGrid_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        #region log 
        private delegate void ShowLogDelegate(UILogEventArgs e);
        public void OnLog(object sender, UILogEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new ShowLogDelegate(DoShowLog), new object[] { e });
            }
            else
            {
                DoShowLog(e);
            }
        }

        private void DoShowLog(UILogEventArgs e)
        {
            if (txtLog.Text.Length > 10000)
                txtLog.Text = "";
            String line = String.Format("{0:hh:mm:ss} {1}\n", DateTime.Now, e.Line);
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
        #endregion

        #region order callback
        private void CreateTrader()
        {
            if (trader != null)
                eventBus.Disconnect(trader);
            trader = new Trader();
            eventBus.Connect(trader);
            trader.onFrontConnected += OnFrontConnected;
            trader.RegisterFront(FrontAddress);
            trader.RegisterFront(FrontAddress2);
            trader.SubscribePrivateTopic(THOST_TE_RESUME_TYPE.THOST_TERT_RESTART);
            trader.SubscribePublicTopic(THOST_TE_RESUME_TYPE.THOST_TERT_RESTART);
        }

        private void OnFrontConnected(object sender, EventArgs e)
        {
            NextRequestID = 1;
            ReqUserLoginField field = new ReqUserLoginField();
            field.BrokerID = BrokerID;
            field.UserID = UserID;
            field.Password = Password;
            if (trader != null)
                trader.ReqUserLogin(field, NextRequestID);
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

        public delegate void EvenBusDelegate();
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

        
        private void OnUserLogin(object sender, EventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new EvenBusDelegate(SetTradeButtonsOn));
            }
            else
            {
                SetTradeButtonsOn();
            }
        }

        private void DoShowFund(FundField fund)
        {
            labelAvailable.Content = fund.AmountAvailable.ToString("N2");
            labelDrawable.Content = fund.AmountDrawable.ToString("N2");
            labelFrozen.Content = fund.AmountFrozen.ToString("N2");
            labelPre.Content = fund.AmountPre.ToString("N2");
        }

        public delegate void ShowFundDelegate(FundField fund);
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


        #endregion

        #region data

        
        public void ConnetTrader()
        {
            
            trader.Init();
        }

        public void DisconnectTrader()
        {
            trader = null;
        }

        
        public int BrokerID { get; set; }
        public int UserID { get; set; }
        public int InvestorID { get; set; }
        public string Password { get; set; }

        public int NextRequestID { get; set; }

        public string FrontAddress { get; set; }

        public string FrontAddress2 { get; set; }

        public TraderEventBus EventBus { get { return eventBus; } }

        private TraderEventBus eventBus = new TraderEventBus();
        private Trader trader;


        #endregion

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            var section = ConfigurationManager.GetSection("USER_INFO") as NameValueCollection;
            BrokerID = int.Parse(section["BrokerID"]);
            UserID = int.Parse(section["UserID"]);
            Password = section["Password"];
            FrontAddress = section["FrontAddress"];
            FrontAddress2 = section["FrontAddress2"];
            InvestorID = UserID;

            eventBus.onUILogAddNewLine += OnLog;
            eventBus.onUserLogin += OnUserLogin;
            eventBus.onUserLogout += OnUserLogout;
            eventBus.onFundUpdate += OnFundUpdate;

            SetTradeButtonsOff();
        }
    }
}
