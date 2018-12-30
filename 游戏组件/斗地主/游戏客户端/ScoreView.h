#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CWnd
{
	//��������
protected:
	__int64							m_lTax;								//��Ϸ˰��
	__int64							m_lScore[GAME_PLAYER];				//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][LEN_ACCOUNTS];//�û�����

	//��Դ����
protected:
	CBitImage						m_ImageBack;						//����ͼ��
	CSkinResourceManager *          m_pSkinResourceManager;

	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���ܺ���
public:
	//���û���
	void ResetScore();
	//����˰��
	void SetTax(__int64 lTax);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, __int64 lScore);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif