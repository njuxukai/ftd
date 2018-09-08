#ifndef _FTD_PROPERTY_ID_H
#define _FTD_PROPERTY_ID_H

enum FTD_PROPERTY
{
	PROPERTY_ID_Abstract,
	PROPERTY_ID_ActionLocalId,
	PROPERTY_ID_ActionDate,
	PROPERTY_ID_ActiveTime,
	PROPERTY_ID_ActiveUserId,
	PROPERTY_ID_AddValue,
	PROPERTY_ID_AskLot,
	PROPERTY_ID_AskPrice,
	PROPERTY_ID_AutoSuspend,
	PROPERTY_ID_AveragePrice,
	PROPERTY_ID_BalanceValue,
	PROPERTY_ID_BidLot,
	PROPERTY_ID_BidPrice,
	PROPERTY_ID_BroadcastLoginTime,
	PROPERTY_ID_BroadcastLogOutTime,
	PROPERTY_ID_BroadcastSequenceNo,
	PROPERTY_ID_BroadcastSequenceSeries,
	PROPERTY_ID_BroadcastStreamStatus,
	PROPERTY_ID_BuyHedgeOffsetMargin,
	PROPERTY_ID_BuyHedgeOffsetProfit,
	PROPERTY_ID_BuyHedgeOpenFrozMargin,
	PROPERTY_ID_BuyHedgeOpenUsedMargin,
	PROPERTY_ID_BuyHedgePosition,
	PROPERTY_ID_BuyHedgeTurnOver,
	PROPERTY_ID_BuyHedgeVolume,
	PROPERTY_ID_BuyOffsHPosition,
	PROPERTY_ID_BuyOffsSPosition,
	PROPERTY_ID_BuySpecOffsetMargin,
	PROPERTY_ID_BuySpecOffsetProfit,
	PROPERTY_ID_BuySpecOpenFrozMargin,
	PROPERTY_ID_BuySpecOpenUsedMargin,
	PROPERTY_ID_BuySpecPosition,
	PROPERTY_ID_BuySpecTurnOver,
	PROPERTY_ID_BuySpecVolume,
	PROPERTY_ID_CancelDate,
	PROPERTY_ID_CancelFlag,
	PROPERTY_ID_CancelTime,
	PROPERTY_ID_ClearDate,
	PROPERTY_ID_ClearPrice,
	PROPERTY_ID_ClientId,
	PROPERTY_ID_ClientName,
	PROPERTY_ID_ClientType,
	PROPERTY_ID_ClosePrice,
	PROPERTY_ID_ComeFrom,
	PROPERTY_ID_ConnectionReference,
	PROPERTY_ID_Content,
	PROPERTY_ID_Currency,
	PROPERTY_ID_DataFlowFlag,
	PROPERTY_ID_DelValue,
	PROPERTY_ID_DialogLoginTime,
	PROPERTY_ID_DialogLogoutTime,
	PROPERTY_ID_DialogStreamStatus,
	PROPERTY_ID_Direction,
	PROPERTY_ID_EndDelvDate,
	PROPERTY_ID_EndTrdDate,
	PROPERTY_ID_ErrorCode,
	PROPERTY_ID_ErrorText,
	PROPERTY_ID_ExchangeDateTime,
	PROPERTY_ID_ExecOrderLocalID,
	PROPERTY_ID_ExecOrderSysID,
	PROPERTY_ID_Forcedistime,
	PROPERTY_ID_ForceExitCode,
	PROPERTY_ID_ForceExitMsg,
	PROPERTY_ID_ForceGroupId,
	PROPERTY_ID_FrontId,
	PROPERTY_ID_FuseFlag,
	PROPERTY_ID_Fuselimit,
	PROPERTY_ID_Fusepersisttime,
	PROPERTY_ID_Fusepoint,
	PROPERTY_ID_FuseRights,
	PROPERTY_ID_HedgeFlag,
	PROPERTY_ID_HighLimit,
	PROPERTY_ID_HighLimitRate,
	PROPERTY_ID_HighPrice,
	PROPERTY_ID_InitMargin,
	PROPERTY_ID_InsertTime,
	PROPERTY_ID_InstrStopCode,
	PROPERTY_ID_InstrumentId,
	PROPERTY_ID_InstrumentName,
	PROPERTY_ID_InstrumentStatus,
	PROPERTY_ID_InstrumentType,
	PROPERTY_ID_InstrumentVersion,
	PROPERTY_ID_IpAddr,
	PROPERTY_ID_IsLogin,
	PROPERTY_ID_LastLot,
	PROPERTY_ID_LastPrice,
	PROPERTY_ID_LifeHigh,
	PROPERTY_ID_LifeLow,
	PROPERTY_ID_LimitPrice,
	PROPERTY_ID_LongOffsFrozHPosition,
	PROPERTY_ID_LongOffsFrozSPosition,
	PROPERTY_ID_LongOpenFrozHPosition,
	PROPERTY_ID_LongOpenFrozSPosition,
	PROPERTY_ID_LowLimit,
	PROPERTY_ID_LowLimitRate,
	PROPERTY_ID_LowPrice,
	PROPERTY_ID_Margin,
	PROPERTY_ID_MarketId,
	PROPERTY_ID_MarketMatchDataStatus,
	PROPERTY_ID_MarketName,
	PROPERTY_ID_MarketStatusId,
	PROPERTY_ID_MatchCondition,
	PROPERTY_ID_MatchDate,
	PROPERTY_ID_MatchFlag,
	PROPERTY_ID_MatchSession,
	PROPERTY_ID_MatchTime,
	PROPERTY_ID_MaxLot,
	PROPERTY_ID_MaxOrderLocalId,
	PROPERTY_ID_MessageReference,
	PROPERTY_ID_MinimalVolume,
	PROPERTY_ID_MinLot,
	PROPERTY_ID_Name,
	PROPERTY_ID_NewPassword,
	PROPERTY_ID_NewsType,
	PROPERTY_ID_NewsUrgency,
	PROPERTY_ID_OffsetFlag,
	PROPERTY_ID_OldPassword,
	PROPERTY_ID_OpenInterest,
	PROPERTY_ID_OpenPrice,
	PROPERTY_ID_OptionsType,
	PROPERTY_ID_OrderActionCode,
	PROPERTY_ID_OrderLocalId,
	PROPERTY_ID_OrderStatus,
	PROPERTY_ID_OrderSysId,
	PROPERTY_ID_OrderType,
	PROPERTY_ID_ParticipantId,
	PROPERTY_ID_ParticipantName,
	PROPERTY_ID_ParticipantType,
	PROPERTY_ID_PartiTrdRight,
	PROPERTY_ID_Password,
	PROPERTY_ID_PreClose,
	PROPERTY_ID_PreOpenInterest,
	PROPERTY_ID_PreSettle,
	PROPERTY_ID_Price,
	PROPERTY_ID_PrivateLoginTime,
	PROPERTY_ID_PrivateLogoutTime,
	PROPERTY_ID_PrivateStreamStatus,
	PROPERTY_ID_ProtocolVersion,
	PROPERTY_ID_Pubstyle,
	PROPERTY_ID_Premium,
	PROPERTY_ID_QuoteSysID,
	PROPERTY_ID_QuoteLocalID,
	PROPERTY_ID_RequestBy,
	PROPERTY_ID_RateUnit,
	PROPERTY_ID_RatePrice,
	PROPERTY_ID_SellHedgeOffsetMargin,
	PROPERTY_ID_SellHedgeOffsetProfit,
	PROPERTY_ID_SellHedgeOpenFrozMargin,
	PROPERTY_ID_SellHedgeOpenUsedMargin,
	PROPERTY_ID_SellHedgePosition,
	PROPERTY_ID_SellHedgeTurnOver,
	PROPERTY_ID_SellHedgeVolume,
	PROPERTY_ID_SellOffsHPosition,
	PROPERTY_ID_SellOffsSPosition,
	PROPERTY_ID_SellSpecOffsetMargin,
	PROPERTY_ID_SellSpecOffsetProfit,
	PROPERTY_ID_SellSpecOpenFrozMargin,
	PROPERTY_ID_SellSpecOpenUsedMargin,
	PROPERTY_ID_SellSpecPosition,
	PROPERTY_ID_SellSpecTurnOver,
	PROPERTY_ID_SellSpecVolume,
	PROPERTY_ID_SendTime,
	PROPERTY_ID_SequenceNo,
	PROPERTY_ID_SequenceSeries,
	PROPERTY_ID_ServerAppName,
	PROPERTY_ID_SettlePrice,
	PROPERTY_ID_ShortCutKey,
	PROPERTY_ID_ShortOffsFrozHPosition,
	PROPERTY_ID_ShortOffsFrozSPosition,
	PROPERTY_ID_ShortOpenFrozHPosition,
	PROPERTY_ID_ShortOpenFrozSPosition,
	PROPERTY_ID_StartDelvDate,
	PROPERTY_ID_StartTime,
	PROPERTY_ID_StartTrdDate,
	PROPERTY_ID_StopPrice,
	PROPERTY_ID_SuspendTime,
	PROPERTY_ID_StrikePrice,
	PROPERTY_ID_ThroughTime,
	PROPERTY_ID_Tick,
	PROPERTY_ID_TimeOut,
	PROPERTY_ID_TimeSpan,
	PROPERTY_ID_TimeStamp,
	PROPERTY_ID_TotalMarketMatchData,
	PROPERTY_ID_TotalVolume,
	PROPERTY_ID_TradeDate,
	PROPERTY_ID_TradeId,
	PROPERTY_ID_TradeLot,
	PROPERTY_ID_TradePrice,
	PROPERTY_ID_TradeRights,
	PROPERTY_ID_TradeTurnover,
	PROPERTY_ID_TradeType,
	PROPERTY_ID_TradeVolume,
	PROPERTY_ID_Unit,
	PROPERTY_ID_UpdateTime,
	PROPERTY_ID_URLLink,
	PROPERTY_ID_UserId,
	PROPERTY_ID_UserStatus,
	PROPERTY_ID_UserType,
	PROPERTY_ID_UnderlyingInstrID,
	PROPERTY_ID_UnderlyingMultiple,
	PROPERTY_ID_ValidThrough,
	PROPERTY_ID_Volume,
	PROPERTY_ID_VolumeTotal,
	PROPERTY_ID_VolumeTotalOrginal,
	PROPERTY_ID_VolumeType,
	PROPERTY_ID_YdBuyHedgePosition,
	PROPERTY_ID_YdBuyOffsHPosition,
	PROPERTY_ID_YdBuyOffsSPosition,
	PROPERTY_ID_YdBuySpecPosition,
	PROPERTY_ID_YdLongOffsFrozHPosition,
	PROPERTY_ID_YdLongOffsFrozSPosition,
	PROPERTY_ID_YdSellHedgePosition,
	PROPERTY_ID_YdSellOffsHPosition,
	PROPERTY_ID_YdSellOffsSPosition,
	PROPERTY_ID_YdSellSpecPosition,
	PROPERTY_ID_YdShortOffsFrozHPosition,
	PROPERTY_ID_YdShortOffsFrozSPosition,
};

#endif