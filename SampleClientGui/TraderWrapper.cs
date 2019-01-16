using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleClientGui
{
    class TraderWrapper
    {

        public Xcp.Trader Trader
        {
            get { return trader; }
        }
         
        private TraderWrapper()
        {
            trader = new Xcp.Trader();
        }

        private void Init(int brokerid, int userid, int investorid, string password)
        {
            trader.SubscribePrivateTopic(Xcp.THOST_TE_RESUME_TYPE.THOST_TERT_RESTART);
            trader.SubscribePublicTopic(Xcp.THOST_TE_RESUME_TYPE.THOST_TERT_RESTART);
            trader.Init();
        }

        private static Lazy<TraderWrapper> instance = new Lazy<TraderWrapper>(() => new TraderWrapper());
        private Xcp.Trader trader = null;
    }
}
