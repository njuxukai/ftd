using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Xcp
{

    public class TraderDllWrapper
    {
        [DllImport("xcptraderapi.dll", CharSet = CharSet.Auto, EntryPoint ="create_trader")]
        public static extern void CreateTrader(out IntPtr phandler);
    }

    public class Trader
    {
        public Trader()
        {
            TraderDllWrapper.CreateTrader(out handler);
        }

        private IntPtr handler;
    }
}
