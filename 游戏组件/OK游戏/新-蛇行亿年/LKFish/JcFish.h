// Dsfish.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDsfishApp:
// �йش����ʵ�֣������ Dsfish.cpp
//

class CDsfishApp : public CWinApp
{
public:
	CDsfishApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDsfishApp theApp;
