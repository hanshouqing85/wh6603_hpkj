#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0501		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0501 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0501	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
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

//////////////////////////////////////////////////////////////////////////////////

//���뻷��
#include "..\..\SDKCondition.h"

//////////////////////////////////////////////////////////////////////////////////
#if _MSC_VER == 1400

//////////////////////////////////////////////////////////////////////////////////
//��������

#include "..\..\..\�ͻ������\��Ϸ���\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/WHImage.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/GameEngine.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/SkinControl.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/WHImage.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/WHImageD.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../VC8���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif


//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����

#include "..\..\..\�ͻ������\��Ϸ���\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../���ӿ�/Ansi/WHImage.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameEngine.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/SkinControl.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../../���ӿ�/Unicode/WHImage.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../���ӿ�/Ansi/WHImageD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../../���ӿ�/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif


#endif

#define  __TEST__  1
#define  __BANKER___
#define  __SPECIAL___
//////////////////////////////////////////////////////////////////////////
#include "..\��Ϣ����\CMD_OxBattle.h"


//////////////////////////////////////////////////////////////////////////
//
