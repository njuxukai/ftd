using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using Xcp;

namespace SampleClientGui
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {

        public void ConnetTrader()
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

        public int BrokerID { get; }
        public int UserID { get; }
        public int InvestorID { get; }
        public string Password { get; }

        public int NextRequestID { get; set; }

        public string FrontAddress { get; }

        public string FrontAddress2 { get; }

        public TraderEventBus EventBus { get { return eventBus; } }

        private TraderEventBus eventBus = new TraderEventBus();
        private Trader trader;

        public App()
        {
            InitializeComponent();
            var section = ConfigurationManager.GetSection("USER_INFO") as NameValueCollection;
            BrokerID = int.Parse(section["BrokerID"]);
            UserID = int.Parse(section["UserID"]);
            Password = section["Password"];
            FrontAddress = section["FrontAddress"];
            FrontAddress2 = section["FrontAddress2"];
            InvestorID = UserID;
        }
    }
}
