#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "Resource.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó�����
class CGameClientApp : public CGameFrameApp
{
	//��������
public:
	//���캯��
	CGameClientApp();
	//��������
	virtual ~CGameClientApp();

	//�̳к���
public:
	////����������
	//virtual CGameFrameEngine * GetGameFrameDlg() { return new CGameClientDlg; }
	//��������
	virtual CGameFrameEngine * GetGameFrameEngine(DWORD dwSDKVersion);
	void CloseControl();
};

//Ӧ�ó������
extern CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////
