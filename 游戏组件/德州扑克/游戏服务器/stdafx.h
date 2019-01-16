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
#define POINTER_64 __ptr64 

typedef void *PVOID;
typedef void * POINTER_64 PVOID64;
  
//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//////////////////////////////////////////////////////////////////////////////////

//编译环境
#include "..\..\SDKCondition.h"
//组件头文件
#include "..\..\..\服务器组件\游戏服务\GameServiceHead.h"
#include "..\消息定义\CMD_DZShowHand.h"

//////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER == 1400

//////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../../VC8链接库/Ansi/KernelEngine.lib")
#else
	#pragma comment (lib,"../../../VC8链接库/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../../VC8链接库/Ansi/KernelEngineD.lib")
#else
	#pragma comment (lib,"../../../VC8链接库/Unicode/KernelEngineD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../链接库/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../../链接库/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../链接库/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../../链接库/Unicode/KernelEngineD.lib")
#endif
#endif
//////////////////////////////////////////////////////////////////////////////////

#endif // SDK_CONDITION
//////////////////////////////////////////////////////////////////////////////////


#define USE_RS_PRINT
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<algorithm>
using namespace std;
///////////////////////////////////////////////////////////////////////////////////