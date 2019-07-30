#ifndef DRAW_WND_HEAD_FILE
#define DRAW_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
#define IDM_SICE_FINISH			(WM_USER+401)						//�˿˲���
#define IDM_CARD_MOVE			(WM_USER+402)						//�ƶ�����
#define IDM_SICE_TWO			(WM_USER+403)						//�ƶ�����
#define IDM_CAI_SHEN_SICE_FINISH (WM_USER+404)						//��������
#define IDM_LAND_SICE_FINISH	 (WM_USER+405)						//ׯ������

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
	WORD							m_wSice1;
	WORD							m_wSice2;
	WORD							m_wSice3;
	BYTE                            m_bSicePos;
	int m_nXPos;
	int m_nYPos;
	CDC      *ParentDC;
	CWnd							*m_pSinkWnd;						//֪ͨ����
	//��������
public:
	//���캯��
	CDrawSiceWnd();
	//��������
	virtual ~CDrawSiceWnd();

	//��������
public:
	//����ɫ����Ϣ
	void SetSiceInfo(CDC *dc,BYTE bMoveTime,WORD wSice1,WORD wSice2,WORD wSice3=0);
	//���û�׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos);
	//����֪ͨ����
	void SetWindowSink(CWnd *pSinkWnd);
	//ֹͣ����
	void StopSicing();

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
