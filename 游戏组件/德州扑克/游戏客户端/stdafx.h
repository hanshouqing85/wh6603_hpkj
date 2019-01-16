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

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>
#include <AtlBase.h>
#include <AtlDbcli.h>
#include <AfxDtctl.h>

//////////////////////////////////////////////////////////////////////////////////

//编译环境
#include "..\..\SDKCondition.h"
//组件头文件
#include "..\..\..\客户端组件\游戏框架\GameFrameHead.h"
#include "..\消息定义\CMD_DZShowHand.h"
//////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER == 1400

//////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../VC8链接库/Ansi/WHImage.lib")
#pragma comment (lib,"../../../VC8链接库/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../VC8链接库/Ansi/GameEngine.lib")
#pragma comment (lib,"../../../VC8链接库/Ansi/SkinControl.lib")
#pragma comment (lib,"../../../VC8链接库/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../../VC8链接库/Unicode/WHImage.lib")
#pragma comment (lib,"../../../VC8链接库/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../VC8链接库/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../VC8链接库/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../VC8链接库/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../VC8链接库/Ansi/WHImageD.lib")
#pragma comment (lib,"../../../VC8链接库/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../VC8链接库/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../../VC8链接库/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../../VC8链接库/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../../VC8链接库/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../VC8链接库/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../VC8链接库/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../VC8链接库/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../VC8链接库/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../链接库/Ansi/WHImage.lib")
#pragma comment (lib,"../../../链接库/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../链接库/Ansi/GameEngine.lib")
#pragma comment (lib,"../../../链接库/Ansi/SkinControl.lib")
#pragma comment (lib,"../../../链接库/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../../链接库/Unicode/WHImage.lib")
#pragma comment (lib,"../../../链接库/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../链接库/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../链接库/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../链接库/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../链接库/Ansi/WHImageD.lib")
#pragma comment (lib,"../../../链接库/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../链接库/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../../链接库/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../../链接库/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../../链接库/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../链接库/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../链接库/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../链接库/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../链接库/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////