#ifndef WND_USER_INFO_CTRL_HEAD_FILE
#define WND_USER_INFO_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GamePlaza.h"
//////////////////////////////////////////////////////////////////////////////////
#define				IDM_REFRESH_YUE				113
#define				IDM_FRESH_GAME			11114
#define				IDM_ZHUANHUAN_TO_CAIPIAO			11115
#define				IDM_ZHUANHUAN_TO_QIPAI			11116
//�û���Ϣ
class CWndUserInfoCtrl : public CWnd
{
	//�ؼ�����
public:
	DOUBLE							m_fUserScore;						//�û�����
	BYTE							m_cbScoreType;
	CSkinButton						m_btnFlush;		//ˢ��

	CSkinButton						m_btnChange;		//ת��
	Bitmap* m_Face;

	DWORD							m_dwTickCount;						//����
	bool							m_bHitUserInfo1;					//�Ƿ���
	bool							m_bHitUserInfo2;					//�Ƿ���
	bool							m_bHitUserInfo3;					//�Ƿ���
	bool							m_bHitUserInfo4;					//�Ƿ���
	CFont							m_Font;
	//�ؼ�����
public:
	CDlgInsurePlaza *               m_pDlgInsurePlaza;                 //��������

	//��������
public:
	//���캯��
	CWndUserInfoCtrl();
	//��������
	virtual ~CWndUserInfoCtrl();

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//ת���ַ�
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, UINT uDecimalCount, LPTSTR pszBuffer, WORD wBufferSize);
	
public:
	void SetClicked(BOOL bShow, UINT nStatus);
	void ReleaseFace();
	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	//�Զ���Ϣ
protected:
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif