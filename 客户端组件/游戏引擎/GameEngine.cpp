// GameEngine.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

static AFX_EXTENSION_MODULE GameEngineDLL = { NULL, NULL };

//////////////////////////////////////////////////////////////////////////////////

//��������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameEngineDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameEngineDLL);

	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameEngineDLL);
	}

	return 1;
}