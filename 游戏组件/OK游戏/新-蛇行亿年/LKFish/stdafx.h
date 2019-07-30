// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0410 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0400	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT
//�����ļ�
//
void TraceString(LPCTSTR pszMessage);
const float CLIENT_WIDTH = 1152;
const float CLIENT_HEIGHT = 720;
const int USERCOUNT = 6;

//������ű���
const float m_FishScale = 0.75f;
const float m_FishScale22 = 0.80f;
const float m_speedBili = 0.8f;
#define DIRPATH _T("SXfish\\Music\\")
//#define RARPASSWORD  "fidbdsblbyjlitjoalajgtin"
//�����ӿ�
interface BaseWnd
{
	//�����ӿ�
    public:
		//��������
		virtual bool  InitClientMessage()=NULL;
		//�����ӵ���Ϣ
		virtual bool  OnShoot(bool isSuper,float roalation,DWORD m_bulletID)=NULL;
		//�˳���Ϸ
		virtual bool CloseExit()=NULL;
		//�������
		virtual bool HitFirsh(int dwFishID,int dwBulletID,int ShootUSER,bool IsAndroid,int zidanID)=NULL;
		//���·�
		virtual bool AddOrRemoveScore(bool addorremove,int addscore,bool allscore=false)=NULL;
		//�л��ڱ���
		virtual bool ChangeFireBei(bool upordown)=NULL;
		//�л��ڵ��ٶ�
		virtual bool  ChangeFireSpeed(int index)=NULL;
		//��������ID
		virtual bool  SendLockFishID(int FishID,int PlayerID)=NULL;
};
