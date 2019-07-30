#pragma once
#include "SHButton.h"


#define PYOE_LEFT                     0x00000000
#define PYOE_CENTER                   0x00000001
#define PYOE_RIGHT                    0x00000002
#define PYOE_TOP                      0x00000000
#define PYOE_VCENTER                  0x00000004
#define PYOE_BOTTOM                   0x00000008

#define IDM_FOLLOW					(WM_USER+201)						//��ע��Ϣ
#define IDM_GIVE_UP					(WM_USER+202)						//������Ϣ
#define IDM_ADD_SCORE				(WM_USER+203)						//��ע��Ϣ
#define IDM_SHOW_HAND				(WM_USER+204)						//�����Ϣ

class CPlayOperate;

//�������
class CEditOperate : public CEdit
{

public:
	SCORE						m_lMaxScore;		//�����ע
	SCORE						m_lMinScore;		//��С��ע
	CPlayOperate*				m_pPlayOperate;		//��Ҳ�����

protected:
	CBrush						m_Brush;			//������ɫ

public:
	CEditOperate();
	~CEditOperate();

	// ������ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ������ɫ
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	// �ж��ַ�
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	// �����Ҽ�
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// ���̵��
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	DECLARE_MESSAGE_MAP()
};

// ��Ҳ���
class CPlayOperate
{
	// ��������
protected:
	bool							m_bShow;				// ��ʾ��ʾ
	bool							m_bMouseDown;			// �����
	SCORE							m_lCellScore;			// ��Ϸ��ע
	int								m_lDragSite;			// �϶�λ��

	// ���Զ���
protected:
	CPoint							m_ptPos;				// λ����Ϣ
	CSize							m_sizeBakc;				// ��С��Ϣ
	CRect							m_rectDrag;				// �϶�����
	CSize							m_sizeDrag;				// �϶�ͼƬ��С

	// ��Դ����
public:
	CPngImage						m_ImageBack;			// ����ͼƬ
	CPngImage						m_ImageDrag;			// �϶�ͼƬ
	CSkinButton						m_btOneScore;			// 1����ť
	CSkinButton						m_btTwoScore;			// 2����ť
	CSkinButton						m_btThreeScore;			// 3����ť	
	CSkinButton						m_btFourScore;			// 4����ť
	CSkinButton						m_btAddScore;			// ��ע��ť
	CSkinButton						m_btFollow;				// ��ע��ť
	CSkinButton						m_btGiveUp;				// ������ť
	CSkinButton						m_btShowHand;			// �����ť
	CSHButton						m_btADD;				// �����ť
	CSHButton						m_btSUB;				// �����ť

	CFont							m_fontScoreInput;		// ��������
	CEditOperate					m_edieScoreInput;		// ��������

public:
	CPlayOperate(void);
	~CPlayOperate(void);

	// ���Ժ���
public:
	// ��ʼ��
	void Initialization(CWnd* pParent);

	// �滭����
public:
	// �滭����
	void DrawPlayOperate(CWnd* pWnd,CDC * pDC, INT nWidth, INT nHeight);

	// ���ú���
public:
	// ����λ��
	void SetOperatePos( int nPosX, int nPosY, UINT nFlags);
	// ������Ϣ
	void SetCellScore( SCORE lCellScore);
	// ������ע��Χ��
	void SetBetCount( SCORE lMaxBetCount, SCORE lMinBetCount );

	// ��ȡ����
public:
	// ��ȡ��������
	SCORE GetBetCount();
	// ��ȡλ��
	CRect GetOperateRect();
	// ��ȡ��ʾ״̬
	bool	 IsWindowVisible() { return m_bShow; }
	// ��ȡ��ע״̬
	bool	 IsAddShow() { return m_btAddScore.IsWindowEnabled()&&m_btAddScore.IsWindowVisible(); }


	// ���ܺ���
public:
	// ��ʾ����
	void ShowOperate( bool bShow );
	// ��Ϣ����
	BOOL PreTranslateMessage(MSG * pMsg);
	// ������Ϣ
	BOOL	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;
	// ��ť��Ӧ
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	// ���û���
	void SetEditScore();
	// ���û���
	void SetDragScore();
	// ���û���
	void ScoreADD();
	// ���û���
	void ScoreADD(SCORE score);
	// ���û���
	void ScoreSUB();
	// ���û���
	void ScoreSUB(SCORE score);
	// ��Ӷ���
	CString AddComma( LONGLONG lScore );
	// ��Ӷ���
	CString AddComma( CString strScore );
	// ɾ������
	LONGLONG DeleteComma( CString strScore );
};
