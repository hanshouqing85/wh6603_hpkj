#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//Ӧ�ó������
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientApp::CGameClientApp()
{
	//AfxMessageBox("ok");
}

//��������
CGameClientApp::~CGameClientApp() 
{
	AfxMessageBox(TEXT("111"));
}

//��������
CGameFrameEngine * CGameClientApp::GetGameFrameEngine(DWORD dwSDKVersion)
{
	//�汾���
	if (InterfaceVersionCompare(VERSION_FRAME_SDK,dwSDKVersion)==false)
	{
		ASSERT(FALSE);
		return NULL;
	}

	//��������
	return new CGameClientDlg;
}

//////////////////////////////////////////////////////////////////////////////////
