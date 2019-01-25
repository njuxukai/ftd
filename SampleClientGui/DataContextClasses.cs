using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleClientGui
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
        public double VolumeTraded { get { return field_.VolumeCum; } }
        public double AmountTraded { get { return field_.AmountCum; } }
        public Xcp.Enums.OrderStatus OrderStatus { get {return (Xcp.Enums.OrderStatus)field_.OrderStatus; } }
        public int OrderSysID { get { return field_.OrderSysID; } }
        public string OrderExchangeID { get { return field_.OrderExchangeID; } }
        public int FrontID { get { return field_.FrontID; } }
        public int SessionID { get { return field_.SessionID; } }
        public int OrderRef { get { return field_.OrderRef; } }

        Xcp.OrderField field_;
    }

    class Trade
    {
        public Trade(Xcp.TradeField field)
        {
            field_ = field;
        }

        public string InstrumentCode { get { return field_.InstrumentCode; } }
        public Xcp.Enums.Direction Direction { get { return (Xcp.Enums.Direction)field_.Direction; } }
        public int VolumeTrade { get { return field_.VolumeLast; } }
        public double PriceTrade { get { return field_.PriceLast; } }
        public int OrderSysID { get { return field_.OrderSysID; } }
        public int TradeSysID { get { return field_.TradeSysID; } }
        public string TradeExchangeID { get { return field_.TradeExchangeID; } }


        Xcp.TradeField field_;
    }
}
