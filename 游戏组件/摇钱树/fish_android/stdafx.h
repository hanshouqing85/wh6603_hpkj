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

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//编译环境
#include "..\..\SDKCondition.h"

//////////////////////////////////////////////////////////////////////////////////

#ifdef SDK_CONDITION

//////////////////////////////////////////////////////////////////////////////////
//开发环境

//平台环境
#include "..\..\..\开发库\Include\GameServiceHead.h"
#include "..\..\..\开发库\Include\IGameServiceManager.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../开发库/Lib/Ansi/KernelEngine.lib")
#pragma comment (lib,"../../../开发库/Lib/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../../开发库/Lib/Unicode/KernelEngine.lib")
#pragma comment (lib,"../../../开发库/Lib/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../开发库/Lib/Ansi/KernelEngineD.lib")
#pragma comment (lib,"../../../开发库/Lib/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../../开发库/Lib/Unicode/KernelEngineD.lib")
#pragma comment (lib,"../../../开发库/Lib/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////
//系统环境

//平台环境
#include "..\..\..\服务器组件\游戏服务\GameServiceHead.h"
#include "..\..\..\公共组件\服务核心\WHThread.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../链接库/Ansi/KernelEngine.lib")
#pragma comment (lib,"../../../链接库/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../../链接库/Unicode/KernelEngine.lib")
#pragma comment (lib,"../../../链接库/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../链接库/Ansi/KernelEngineD.lib")
#pragma comment (lib,"../../../链接库/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../../链接库/Unicode/KernelEngineD.lib")
#pragma comment (lib,"../../../链接库/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif // SDK_CONDITION

//////////////////////////////////////////////////////////////////////////////////

//游戏文件
#include "..\Command\CMD_Fish.h"

//////////////////////////////////////////////////////////////////////////////////
