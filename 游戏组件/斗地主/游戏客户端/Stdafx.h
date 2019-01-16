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
#include <AtlBase.h>
#include <AtlDbcli.h>
#include <AfxDtctl.h>

//���뻷��
#include "..\..\SDKCondition.h"
#include "..\..\..\�ͻ������\��Ϸ���\GameFrameHead.h"
//////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER == 1400

//////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/WHImage.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/GameFrame.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/GameEngine.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/SkinControl.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/ShareControl.lib")
#else
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/WHImage.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/GameFrame.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/GameEngine.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/SkinControl.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/ShareControl.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/WHImageD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/GameFrameD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/GameEngineD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/SkinControlD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Ansi/ShareControlD.lib")
#else
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/WHImageD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/GameFrameD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/GameEngineD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/SkinControlD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../../VC8���ӿ�/Unicode/ShareControlD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../../���ӿ�/Ansi/WHImage.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/GameFrame.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/GameEngine.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/SkinControl.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/ShareControl.lib")
#else
	#pragma comment (lib,"../../../���ӿ�/Unicode/WHImage.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/GameFrame.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/GameEngine.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/SkinControl.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/ShareControl.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../../���ӿ�/Ansi/WHImageD.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/GameFrameD.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/GameEngineD.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/SkinControlD.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../../���ӿ�/Ansi/ShareControlD.lib")
#else
	#pragma comment (lib,"../../../���ӿ�/Unicode/WHImageD.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/GameFrameD.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/GameEngineD.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/SkinControlD.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../../���ӿ�/Unicode/ShareControlD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif // SDK_CONDITION

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ�ļ�
#include "..\��Ϣ����\CMD_Game.h"

//////////////////////////////////////////////////////////////////////////////////
