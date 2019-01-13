#pragma once

#define POINTER_64 __ptr64

typedef void *PVOID;
typedef void * POINTER_64 PVOID64;


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0501 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0501	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////
//���뻷��
#include "..\..\SDKCondition.h"

//////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER == 1400

//////////////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/KernelEngine.lib")
#else
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/KernelEngineD.lib")
#else
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/KernelEngineD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../���ӿ�/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../../���ӿ�/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../���ӿ�/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../../���ӿ�/Unicode/KernelEngineD.lib")
#endif
#endif
//////////////////////////////////////////////////////////////////////////////////

#endif // SDK_CONDITION

//���ͷ�ļ�
#include "..\��Ϣ����\CMD_DZShowHand.h"
#include "..\..\..\���������\��Ϸ����\GameServiceHead.h"
#define USE_RS_PRINT
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<algorithm>
using namespace std;
//////////////////////////////////////////////////////////////////////////