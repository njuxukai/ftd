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
*1 ��������Type = 0�� ���п�Ͷ
*2 �����
2.1    1  ��¼ע�� ExtLength=0 ContentLength=x�� char[10]�����֣�(frontID) ע��frontidΨһ��¼
2.2    2  �ǳ� ExtLength = 0   ContentLength=x   char[10] (���֣�(frontID)
3 ���� 
3.1 �Ի���  ����ExtHeader ���͵����Զ���
3.2 ˽��/������ 
*
*
*/
struct RqpHeader
{
	uint8_t m_rqpType;
	int16_t m_dialogExtLength;
	int16_t m_contentLength;
};


//rsp req����������������ͷ+Ftdc�� rsp����req��ԭ·����
//private,public ����������Ftdc��
struct DialogExtHeader
{
	int16_t m_queueAcceptorID;
	int16_t m_reqQueueID;
	int16_t m_rspQueueID;
	int16_t m_frontID;
	int32_t m_sessionID; //ǰ�û���FtdSessionID
};

#endif