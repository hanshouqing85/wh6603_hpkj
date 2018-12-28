#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include "HapiDump.h"

//Ӧ�ó������
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientApp::CGameClientApp()
{
//	AfxMessageBox("ok");
	DeclareDumpFile();
}

//��������
CGameClientApp::~CGameClientApp() 
{
}

//����������
CGameFrameEngine * CGameClientApp::GetGameFrameEngine( DWORD dwSDKVersion )
{
	//�汾���
	if (InterfaceVersionCompare(VERSION_FRAME_SDK,dwSDKVersion)==false)
	{
		ASSERT(FALSE);
		return NULL;
	}
	//��������
	return new CGameClientEngine;
}

//////////////////////////////////////////////////////////////////////////
