#include "Stdafx.h"
#include "GameClient.h"

//Ӧ�ó������
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientApp::CGameClientApp()
{
}

//��������
CGameClientApp::~CGameClientApp() 
{
}

CGameFrameEngine * CGameClientApp::GetGameFrameEngine(DWORD dwSDKVersion) 
{ 
	return new CGameClientDlg; 
}
//////////////////////////////////////////////////////////////////////////
