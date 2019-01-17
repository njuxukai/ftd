using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xcp;
namespace TestMarshal
{
    class Program
    {

        static void onFrontConnected(object sender, EventArgs e)
        {
            ReqUserLoginField req = new ReqUserLoginField { UserID = 99, Password ="test"};
            trader.ReqUserLogin(req, 1);
        }
        static void Main(string[] args)
        {
            trader = new Trader();
            trader.RegisterFront("tcp://localhost:8000");
            trader.onFrontConnected += onFrontConnected;
            trader.Init();
            while (1 == 1)
            {
                Console.ReadLine();
            }
        }

        private static Trader trader;
    }
}
