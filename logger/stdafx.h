// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>



//   �ڴ˴����ó�����Ҫ������ͷ�ļ�

#ifdef _DEBUG
#pragma comment(lib, "log4cplusD.lib")
#else
#pragma comment(lib, "log4cplus.lib")
#endif