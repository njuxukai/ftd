using System;
using System.Runtime.InteropServices;

using Xcp;

namespace TestMarshal
{
    struct A
    {
        public int x;
    };
    class Program
    {
        static void OnFrontConnected(object sender, EventArgs eventArgs)
        {
            ReqUserLoginField field = new ReqUserLoginField { BrokerID = 99, UserID = 99, Password="test"};
            trader.ReqUserLogin(field, 1);
        }

        static void OnRspUserLogin(object sender, RspUserLoginEventArgs eventArgs)
        {
            Console.WriteLine("OnRspUserLogin");
        }
        static void Main(string[] args)
        {
            
            trader.onRspUserLogin += OnRspUserLogin;
            trader.OnFrontConnected += OnFrontConnected;
            trader.RegisterFront ("tcp:\\127.0.0.1:8000");
            trader.Init();
            while (1 == 1)
            {
                Console.ReadLine();
            }
        }
        static Trader trader = new Trader();
    }
}
