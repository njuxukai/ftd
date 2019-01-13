using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Xcp
{

    

    public class Trader : IDisposable
    {
        public Trader() 
        {
            TraderDllWrapper.CreateTrader(out handler);
        }

        ~Trader()
        {
            Dispose(false);
        }

        void Connect()
        {
            TraderDllWrapper.InitTrader(handler);
        }


        protected virtual void Dispose(bool disposing)
        {
            //use called(release managed object)
            if (disposing) { }
            //Release Unmanaged objects
            if (handler != IntPtr.Zero)
            {
                TraderDllWrapper.ReleaseTrader(out handler);
                handler = IntPtr.Zero;
            }

            if (disposing)
            {
                GC.SuppressFinalize(this);
            }
        }

        public void Dispose()
        {
            Dispose(true);
        }


        #region callback
        #endregion

        #region event
        #endregion
        private IntPtr handler;
    }
}
