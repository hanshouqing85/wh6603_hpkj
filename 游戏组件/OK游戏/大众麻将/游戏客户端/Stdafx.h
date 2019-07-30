#pragma once

//////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
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
#include <AtlBase.h>
#include <AtlDbcli.h>
#include <AfxDtctl.h>


//////////////////////////////////////////////////////////////////////////


//���뻷��
#include "..\..\..\SDKCondition.h"//////////
#define USE_GB//by hxh
#include "..\��Ϣ����\CMD_Sparrow.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//������ӿ�

#ifdef SDK_CONDITION

//////////////////////////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////////////////////////
//��������

#include "..\..\..\������\Include\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../������/Lib/Ansi/WHImage.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameEngine.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/SkinControl.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/ServiceCore.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/VideoService.lib")///////
#else
#pragma comment (lib,"../../../������/Lib/Unicode/WHImage.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/ServiceCore.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/VideoService.lib")///////
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../������/Lib/Ansi/WHImageD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/ServiceCoreD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/VideoServiceD.lib")///////
#else
#pragma comment (lib,"../../../������/Lib/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/ServiceCoreD.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/VideoServiceD.lib")///////
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����

#include "..\..\..\..\�ͻ������\��Ϸ���\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../../���ӿ�/Ansi/WHImage.lib")
#pragma comment (lib,"../../../../���ӿ�/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../../���ӿ�/Ansi/GameEngine.lib")
#pragma comment (lib,"../../../../���ӿ�/Ansi/SkinControl.lib")
#pragma comment (lib,"../../../../���ӿ�/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../../../���ӿ�/Unicode/WHImage.lib")
#pragma comment (lib,"../../../../���ӿ�/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../../���ӿ�/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../../���ӿ�/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../../���ӿ�/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../../���ӿ�/Ansi/WHImageD.lib")
#pragma comment (lib,"../../../../���ӿ�/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../../���ӿ�/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../../../���ӿ�/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../../../���ӿ�/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../../../���ӿ�/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../../���ӿ�/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../../���ӿ�/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../../���ӿ�/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../../���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif

