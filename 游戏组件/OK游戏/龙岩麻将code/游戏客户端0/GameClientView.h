#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "DrawSiceWnd.h"
#include "DlgListen.h"
#include "GameScore.h"
#include "MoveCardItem.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_TRUSTEE_CONTROL			(WM_USER+102)						//�йܿ���
#define IDM_CARD_OPERATE			(WM_USER+106)						//��Ҳ�����
#define IDM_MOVE_CARD_FINISH		(WM_USER+107)						//�˿˶���������Ϣ

//////////////////////////////////////////////////////////////////////////

//��ͼλ��
#define MYSELF_VIEW_ID				2									//��ͼλ��

//////////////////////////////////////////////////////////////////////////

struct tagMoveCardItem
{
	CPoint			ptFrom;												//��ʼλ��
	CPoint			ptTo;												//����λ��
	WORD			wViewChairId;										//���ƻ�����ͼλ��
	BYTE			cbCardData;											//���ƻ�������
	BYTE			cbCardCount;										//����
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//��־����
protected:
	bool							m_bOutCard;							//���Ʊ�־
	bool							m_bWaitOther;						//�ȴ���־
	bool							m_bHuangZhuang;						//��ׯ��־
	bool							m_bTrustee[GAME_PLAYER];			//�Ƿ��й�
	bool							m_bListenStatus[GAME_PLAYER];		//���Ʊ�־

	//��Ϸ����
protected:
	int								m_lCellScore;						//��������
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbFengQuan;						//��Ȧ
	BYTE							m_cbLandCount;						//��ׯ����

	//��������
protected:
	bool							m_bBombEffect;						//����Ч��
	BYTE							m_cbBombFrameIndex;					//֡������

	//����Ч��
	WORD							m_wDiscUser;						//�����û�
	BYTE							m_cbDiscFrameIndex;					//֡������	

	//���ƻ���ƶ�������
protected:
	bool							m_bEnableAnimate;					//�Ƿ���ʾ���ƶ���
	bool							m_bCardMoving;						//�Ƿ��ڶ���
	int								m_nXStep;							//X����
	int								m_nYStep;							//Y����
	int								m_nStepCount;						//�ƶ�����
	enDirection						m_enMoveDirection;					//�ƶ�����
	CRect							m_rcCardMove;						//�滭����
	tagMoveCardItem					m_MoveCardItem;						//������Ŀ

	//�û�״̬
protected:
	BYTE							m_cbCardData;						//�����˿�
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbUserAction[GAME_PLAYER];		//�û�����

	//λ�ñ���
protected:
	CPoint							m_UserFlagPos[GAME_PLAYER];			//��־λ��
	CPoint							m_UserListenPos[GAME_PLAYER];		//��־λ��
	CPoint							m_PointTrustee[GAME_PLAYER];		//�й�λ��
	
	CPoint							m_ptWinOrder[GAME_PLAYER];			//
	CPoint							m_ptGangScore[GAME_PLAYER];			//
	//λͼ����
protected:
	CBitImage						m_ImageBack;						//����ͼ��
	CBitImage						m_ImageCenter;						//LOGOͼ
	CPngImage						m_ImageWait;						//�ȴ���ʾ
	CPngImage						m_ImageOutCard;						//������ʾ
	CPngImage						m_ImageUserFlag;					//�û���־
	CPngImage						m_ImageActionBack;					//��������
	CBitImage						m_ImageHuangZhuang;					//��ׯ��־
	CPngImage						m_ImageTrustee;						//�йܱ�־
	CPngImage						m_PngFengQuan;						//Ȧ���־
	CPngImage						m_PngListenFlag;					//���Ʊ�־
	CPngImage						m_ImageActionAni;					//���ƶ�����Դ
	CPngImage						m_ImageDisc;						//����Ч��
	CPngImage						m_ImageArrow;						//��ʱ����ͷ	

	//�˿˿ؼ�
public:
	CHeapCard						m_HeapCard[GAME_PLAYER];			//�����˿�
	CUserCard						m_UserCard[3];						//�û��˿�
	CTableCard						m_TableCard[GAME_PLAYER];					//�����˿�
	CWeaveCard						m_WeaveCard[GAME_PLAYER][MAX_WEAVE];		//����˿�
	CDiscardCard					m_DiscardCard[GAME_PLAYER];					//�����˿�
	CCardControl					m_HandCardControl;					//�����˿�

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���
	CControlWnd						m_ControlWnd;						//���ƴ���
	CDrawSiceWnd					m_DrawSiceWnd;						//ɫ�ӿؼ�
	CDlgListen						m_DlgListen;						//���ƶԻ���
	CGameScore						m_ScoreControl[GAME_PLAYER];						//�ɼ���

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyControl(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ܺ���
public:
	//��������
	void SetCellScore(LONGLONG lCellScore);
	//��ȡ����
	LONG GetCellScore(){return m_lCellScore;}
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//��ׯ����
	void SetHuangZhuang(bool bHuangZhuang);
	//״̬��־
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//���ö���
	bool SetBombEffect(bool bBombEffect);
	//�����û�
	void SetDiscUser(WORD wDiscUser);
	//��ʱ���
	void SetCurrentUser(WORD wCurrentUser);
	//�����й�
	void SetTrustee(WORD wTrusteeUser,bool bTrustee);
	//���Ʊ�־
	void SetUserListenStatus(WORD wViewChairID, bool bListenStatus);
	//���÷�Ȧ
	void SetFengQuan( BYTE cbFengQuan );
	//��ׯ����
	void SetLandCount( BYTE cbLandCount );
	//ֹͣ���ƻ��ƶ���
	bool StopMoveCard();
	//�Ƿ񶯻�������
	bool IsMovingCard() { return m_bCardMoving; }
	//������
	void EnableAnimate( bool bAnimate );
	//�Ƿ�������
	bool IsEnableAnimate() { return m_bEnableAnimate; }
	//�˿˶���
	void OnMoveCardItem( const tagMoveCardItem &MoveCardItem );

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭ʱ��
	void DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
	//�滭׼��
	void DrawUserReadyEx(CDC * pDC, int nXPos, int nYPos);
	//�滭�˿˶���
	void DrawMoveCardItem( CDC *pDC );

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//�Ϲܿ���
	afx_msg void OnStusteeControl();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
