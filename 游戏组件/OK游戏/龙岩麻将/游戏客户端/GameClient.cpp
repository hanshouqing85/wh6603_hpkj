#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"

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
	return new CGameClientEngine; 
}
//////////////////////////////////////////////////////////////////////////
