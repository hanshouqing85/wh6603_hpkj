#ifndef GOLD_CONTROL_HEAD_FILE
#define GOLD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#define IDM_MAX_SCORE				  WM_USER+120						//�����ע
#define IDM_MIN_SCORE				  WM_USER+121						//��С��ע	
#define IDM_OK_SCORE				  WM_USER+122						//ȷ����Ϣ
#define IDM_CANCEL_SCORE			  WM_USER+123						//ȡ����Ϣ	
//////////////////////////////////////////////////////////////////////////

//��ҿ���
class CGoldControl : public CWnd
{
	//��������
protected:
	LONGLONG							m_lMaxGold;						//�����
	LONGLONG							m_lMinGold;						//��С���
	LONGLONG							m_lGoldCell[7];					//��Ԫ���
	CPoint								m_BasicPoint;					//����λ��

	//λ�ñ���
protected:
	int									m_nWidth;						//�ؼ����
	int									m_nCellCount;					//��Ԫ��Ŀ

	//��Ŧ�ؼ�
public:
	CSkinButton							m_btMaxScore;					//���ť
	CSkinButton						    m_btMinScore;					//��С��ť
	CSkinButton						    m_btOK;							//ȷ����Ŧ
	CSkinButton						    m_btCancel;						//ȡ����Ŧ


	//��Դ����
protected:
	CBitImage							m_ImageLeft;					//������Դ
	CBitImage							m_ImageMid;						//������Դ
	CBitImage							m_ImageRight;					//������Դ
	CBitImage							m_ImageNumber;					//������Դ

	//��������
public:
	//���캯��
	CGoldControl();
	//��������
	virtual ~CGoldControl();

	//���ܺ���
public:
	//��ȡ���
	LONGLONG GetGold();
	//���ý��
	void SetGold(LONGLONG lGold);
	//������ע
	void SetMaxGold(LONGLONG lMaxGold);
	//������С
	void SetMinGold(LONGLONG lMinGold);
	//����λ��
	void SetBasicPoint(int nXPos, int nYPos);

	//�ڲ�����
private:
	//�����ؼ�
	void RectifyControl();
	//�滭���
	void DrawGoldCell(CDC * pDC, int nXPos, int nYPox, LONGLONG lGold);

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//���������Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�Ҽ�������Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//���ù��
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//��С��ע
	afx_msg void OnMinScore();
	//�����ע
	afx_msg void OnMaxScore();
	//ȷ����Ϣ
	afx_msg void OnOKScore();
	//ȡ����Ϣ
	afx_msg void OnCancelScore();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


//////////////////////////////////////////////////////////////////////////

#endif
