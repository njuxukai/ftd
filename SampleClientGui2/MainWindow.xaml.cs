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

        }

        private void button_qryOrder_Click(object sender, RoutedEventArgs e)
        {

        }

        private void button_qryPosition_Click(object sender, RoutedEventArgs e)
        {

        }

        private void button_qryFund_Click(object sender, RoutedEventArgs e)
        {

        }

        private void button_qryTrade_Click(object sender, RoutedEventArgs e)
        {

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
            txtLog.AppendText(e.Line);
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
        #endregion

        #region data

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
        public void ConnetTrader()
        {
            
            trader.Init();
        }

        public void DisconnectTrader()
        {
            trader = null;
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


            CreateTrader();
        }
    }
}
