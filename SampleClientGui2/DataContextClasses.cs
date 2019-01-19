using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleClientGui2
{
    class Position
    {
        public Position(Xcp.PositionField field)
        {
            field_ = field;
        }
        public string InstrumentCode { get { return field_.InstrumentCode; } }
        public string InstrumentName { get { return field_.InstrumentName; } }
        public int Volume { get { return field_.Volume; } }
        public int VolumeAvailable { get { return field_.VolumeAvailable; } }
        public int VolumeFrozen { get { return field_.VolumeFrozen; } }
        public int VolumePre { get { return field_.VolumePre; } }
        Xcp.PositionField field_;
    }

    class Order
    {
        public Order(Xcp.OrderField field)
        {
            field_ = field;
        }
        public string InstrumentCode { get { return field_.InstrumentCode; } }
        public Xcp.Enums.Direction Direction { get { return (Xcp.Enums.Direction)field_.Direction; } }
        public int VolumeTotalOriginal { get { return field_.VolumeTotalOrginal; } }
        public double LimitPrice { get { return field_.LimitPrice; } }
        public double VolumeTraded { get { return field_.VolumeTraded; } }
        public double AmountTraded { get { return field_.AmountTraded; } }
        public Xcp.Enums.OrderStatus OrderStatus { get {return (Xcp.Enums.OrderStatus)field_.OrderStatus; } }
        public int OrderSysID { get { return field_.OrderSysID; } }
        public int OrderExchangeID { get { return field_.OrderExchangeID; } }


        Xcp.OrderField field_;
    }

    class Trade
    {
        public Trade(Xcp.TradeField field)
        {
            field_ = field;
        }

        Xcp.TradeField field_;
    }
}
