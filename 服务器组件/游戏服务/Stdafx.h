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
//MFC ÎÄ¼þ
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//////////////////////////////////////////////////////////////////////////////////
//Á´½Ó´úÂë

#if _MSC_VER == 1400

//////////////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../VC8Á´½Ó¿â/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../VC8Á´½Ó¿â/Ansi/KernelEngine.lib")
#else
	#pragma comment (lib,"../../VC8Á´½Ó¿â/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../VC8Á´½Ó¿â/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../VC8Á´½Ó¿â/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../VC8Á´½Ó¿â/Ansi/KernelEngineD.lib")
#else
	#pragma comment (lib,"../../VC8Á´½Ó¿â/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../VC8Á´½Ó¿â/Unicode/KernelEngineD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/KernelEngine.lib")
#else
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/KernelEngineD.lib")
#else
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/KernelEngineD.lib")
#endif
#endif
//////////////////////////////////////////////////////////////////////////////////

#endif



//////////////////////////////////////////////////////////////////////////////////
