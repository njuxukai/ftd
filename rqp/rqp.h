#ifndef RQP_H
#define RQP_H

#include <cstdint>
#include <cstdio>
#include <memory>

#define FTD_HEADER_LENGTH 4

#define MAX_FTD_LENGTH 4 + 127 + 4096
#define MAX_FTDC_LENGTH 4096
#define MAX_FTDC_CONTENT_LENGTH 4096 - 16

#define FTDC_FIELD_HEADER_LENGTH 6
#define FTDC_HEADER_LENGTH 16

/*
*1 心跳包（Type = 0） 仅有空投
*2 管理包
2.1    1  登录注册 ExtLength=0 ContentLength=x， char[10]（名字）(frontID) 注意frontid唯一登录
2.2    2  登出 ExtLength = 0   ContentLength=x   char[10] (名字）(frontID)
3 数据 
3.1 对话流  解析ExtHeader 发送到各自队列
3.2 私有/公共流 
*
*
*/
struct RqpHeader
{
	uint8_t m_rqpType;
	int16_t m_dialogExtLength;
	int16_t m_contentLength;
};


//rsp req队列里必须有这个包头+Ftdc包 rsp按照req的原路返回
//private,public 队列里是裸Ftdc包
struct DialogExtHeader
{
	int16_t m_queueAcceptorID;
	int16_t m_reqQueueID;
	int16_t m_rspQueueID;
	int16_t m_frontID;
	int32_t m_sessionID; //前置机端FtdSessionID
};

#endif