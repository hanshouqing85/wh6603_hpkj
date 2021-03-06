#pragma once

//////////////////////////////////////////////////////////////////////////////////

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

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////
// add by hxh 20160801
#define POINTER_64 __ptr64

typedef void *PVOID;
typedef void * POINTER_64 PVOID64;
//MFC �ļ�
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//�����ļ�
#include "Other\SkinSB.h"
#include "Other\Detours.h"

//ͼƬ�ؼ�
#import "ImageOle.dll" named_guids
#pragma comment (lib,"Winmm.lib")
//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���

#pragma comment (lib,"Other/SkinSB.lib")
#pragma comment (lib,"Other/Detours.lib")

#if _MSC_VER == 1400

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../VC8���ӿ�/Ansi/WHImage.lib")
	#pragma comment (lib,"../../VC8���ӿ�/Ansi/ServiceCore.lib")
#else
	#pragma comment (lib,"../../VC8���ӿ�/Unicode/WHImage.lib")
	#pragma comment (lib,"../../VC8���ӿ�/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../VC8���ӿ�/Ansi/WHImageD.lib")
	#pragma comment (lib,"../../VC8���ӿ�/Ansi/ServiceCoreD.lib")
#else
	#pragma comment (lib,"../../VC8���ӿ�/Unicode/WHImageD.lib")
	#pragma comment (lib,"../../VC8���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif

#else

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/WHImage.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCore.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/WHImage.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/WHImageD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCoreD.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/WHImageD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif


#endif

//////////////////////////////////////////////////////////////////////////////////
