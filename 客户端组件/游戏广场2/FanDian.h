#pragma once
#include "Stdafx.h"
#include "Resource.h"
static SCORE KeepOne(SCORE score)
{
	return ((LONGLONG)((score+0.000001)*100)/10)/10.000000;
};

static SCORE KeepTwo(SCORE score)
{
	return ((LONGLONG)((score+0.000001)*100))/100.000000;
};
class CFanDian
{
	//��������
protected:
	SCORE			m_sCellScore;				//��Ԫ����
	SCORE			m_sLimitMax;				//��ע����
	SCORE			m_sLimitMin;				//��ע����
	SCORE			m_sCheck;					//��ע���

	//ͼƬ��Դ
protected:
	CPngImageEx		m_PngBackView;			//����ͼƬ
	CPngImageEx		m_ImageDrag;			// �϶�ͼƬ
	CFont				m_font;
	COLORREF			m_Color;
	//��ʶ����
protected:
	BOOL			m_bShowView;			//��ʾ����

	//λ�ñ���
protected:
	CPoint			m_ptBasePoint;			//��ʼ����
	CRect			m_rcCheckScore;

	//�϶�����
protected:
	bool			m_bMouseDown;			// �����
	int				m_lDragSite;			// �϶�λ��
	CRect			m_rectDrag;				// �϶�����
	CSize			m_sizeDrag;				// �϶�ͼƬ��С

	//��ť�ؼ�
protected:
	CSkinButton		m_JettonButton[6];		//���밴ť

public:
	//���캯��
	CFanDian();
	//��������
	~CFanDian();


	//----------------------------------------------------------------------------------

	//��ʼ��
	BOOL	InitView(CWnd *pParent);
	//�����
	BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	// ��Ϣ����
	BOOL	PreTranslateMessage(MSG * pMsg);
	//	������Ϣ
	BOOL	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;
	//���ƽ���
	VOID	DrawView(CWnd *pWnd,CDC * pDC, INT nWidth, INT nHeight);
	//�����ؼ�
	VOID	RectifyControl(INT nWidth, INT nHeight);
	//��ʾ����
	VOID	ShowWindows(BOOL bshow);
	//----------------------------------------------------------------------------------

	//�������
	VOID	EmptyData();
	//������Ϣ
	VOID	SetSocreInfo(SCORE slimitmax,SCORE slimitmin);
	//��ע����
	VOID	AddScore(SCORE saddscore);
	//��ע����
	VOID	MinusScore(SCORE sminusscore);
	//��ԭ��С
	VOID	CleanScore();
	//��ʾ����
	CRect	GetRect();
	//���»���
	VOID	UpdataDrag();
	//----------------------------------------------------------------------------------

	//�϶�����
	VOID    SetDragScore();
	//������ע
	VOID	SetCheckScore(SCORE scheck);
	//��������
	VOID	SetLimitMaxScore(SCORE slimitmax);
	//��������
	VOID	SetLimitMinScore(SCORE slimitmin);
	//��Ԫ����
	VOID	SetCellScore(SCORE	sCellScore);
	//��ȡ��ע
	SCORE	GetCheckScore();
	//��ȡ����
	SCORE	GetLimitMaxScore(){return m_sLimitMax;}
	//��ȡ����
	SCORE	GetLimitMinScore(){return m_sLimitMin;}
	//��ȡ��ʾ
	BOOL	IsWindowVisible(){return m_bShowView;}


};