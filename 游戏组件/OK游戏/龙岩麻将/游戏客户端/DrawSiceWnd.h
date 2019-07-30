#ifndef DRAW_WND_HEAD_FILE
#define DRAW_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

#define IDM_SICE_TWO			(WM_USER+403)						//�ƶ�����

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CDrawSiceWnd : public CWnd
{
	//���ñ���
protected:
	CBitImage						m_ImageBack;						//����ͼ��
	CBitImage						m_ImageDrawSiceFirst;				//��������
	CBitImage						m_ImageDrawSiceSecond;				//
	CBitImage						m_ImageDrawSiceUserFirst;				//��������
	CBitImage						m_ImageDrawSiceUserSecond;				//��������
	BYTE                            m_bSicePos;
	int m_nXPos;
	int m_nYPos;
	CDC      *ParentDC;
	////CWnd							*m_pSinkWnd;						//֪ͨ����
	CGameFrameViewGDI							*m_pSinkWnd;						//֪ͨ����

	//��������
public:
	//���캯��
	CDrawSiceWnd();
	//��������
	virtual ~CDrawSiceWnd();

	//��������
public:
	//����ɫ����Ϣ
	void SetSiceInfo(CDC *dc,BYTE bMoveTime,WORD wSice1,WORD wSice2);
	//���û�׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos);
	//����֪ͨ����
	////void SetWindowSink(CWnd *pSinkWnd);
	void SetWindowSink(CGameFrameViewGDI *pSinkWnd);
	//ֹͣ����
	void StopSicing(bool bFinish=true);

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
