#pragma once
// add by hxh 20160801
#define POINTER_64 __ptr64

typedef void *PVOID;
typedef void * POINTER_64 PVOID64;
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
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

//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���

#if _MSC_VER == 1400

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../VC8���ӿ�/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../VC8���ӿ�/Ansi/KernelEngine.lib")
#else
	#pragma comment (lib,"../../VC8���ӿ�/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../VC8���ӿ�/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../VC8���ӿ�/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../VC8���ӿ�/Ansi/KernelEngineD.lib")
#else
	#pragma comment (lib,"../../VC8���ӿ�/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../VC8���ӿ�/Unicode/KernelEngineD.lib")
#endif
#endif

#else

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/KernelEngine.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/KernelEngineD.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/KernelEngineD.lib")
#endif
#endif

#endif

//////////////////////////////////////////////////////////////////////////////////
