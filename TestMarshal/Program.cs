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
        static void OnRspUserLogin(object sender, RspUserLoginEventArgs eventArgs)
        {
            Console.WriteLine("OnRspUserLogin");
        }
        static void Main(string[] args)
        {
            ReqUserLoginField field = new ReqUserLoginField();
            Console.WriteLine(Marshal.SizeOf(field));
            Trader trader = new Trader(99,99,99, "test");
            trader.onRspUserLogin += OnRspUserLogin;
            trader.RegisterFront ("tcp:\\127.0.0.1:8000");
            trader.Init();
            while (1 == 1)
            {
                Console.ReadLine();
            }
        }
    }
}
