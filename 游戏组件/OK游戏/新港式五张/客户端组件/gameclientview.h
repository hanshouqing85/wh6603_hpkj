#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "HistoryRecord.h"
#include "GameLogic.h"
#include "PlayOperate.h"
#include "MoveDraw.h"
#include "GameLogic.h"
#include "AdminControlDlg.h"
//////////////////////////////////////////////////////////////////////////
//�궨��

//��ͼλ��
#define MYSELF_VIEW_ID				2									//��ͼλ��

//������ʶ
#define AC_NO_ADD					0									//���Ӷ���
#define AC_FOLLOW					1									//��ע����
#define AC_ADD_SCORE				2									//��ע����
#define AC_SHOW_HAND				3									//�������
#define AC_GIVE_UP					4									//��������

//������Ϣ
#define IDM_START					(WM_USER+200)						//��ʼ��Ϣ


//֪ͨ��Ϣ
#define IDM_SEND_CARD_FINISH		(WM_USER+300)						//�������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &>	CSendCardItemArray;
typedef CWHArray<stuMoveChip*>						CChipArray;


//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//״̬����
protected:
	SCORE							m_lCellScore;						//��Ϸ��ע
	SCORE							m_lDrawMaxScore;					//�����
	SCORE							m_lUserScore[GAME_PLAYER];			//�û�����
	SCORE							m_lUserAddScore[GAME_PLAYER];		//�û���������ֵ
	SCORE							m_lUserAddAllScore[GAME_PLAYER];	//�û���������ֵ
	SCORE							m_lUserSaveScore[GAME_PLAYER];		//�û���������
	SCORE							m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	SCORE							m_lOverScore[GAME_PLAYER];			//��������
	SCORE							m_lOverSaveScore[GAME_PLAYER];		//�����������

	//���ƶ�������
protected:
	CPoint							m_SendCardPos;						//����λ��
	CPoint							m_SendCardCurPos;					//���Ƶ�ǰλ��
	INT								m_nStepCount;						//����
	INT								m_nXStep;							//����
	INT								m_nYStep;							//����
	CSendCardItemArray				m_SendCardItemArray;				//��������

	BYTE							m_cbUserCard[GAME_PLAYER][MAX_COUNT];
	BYTE							m_cbUserCardCount[GAME_PLAYER];

	//��������
protected:
	WORD							m_wActionUser;						//�����û�
	BYTE							m_cbUserAction;						//������ʶ

	//���ư�ť
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CPlayOperate					m_PlayOperate;						//��Ҳ���

	//��Ϸ�ؼ�
public:
	CGameLogic						m_GameLogic;
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�

//���Ʊ���
public:
	CAdminControlDlg				m_AdminDlg;							//����Ա����
	CButton							m_btOpenAdmin;
	//�˿˳���
public:
	CCardControl					m_CardControl[GAME_PLAYER];			//�˿˿ؼ�
	SCORE							m_lPalyBetChip[GAME_PLAYER];		//�����ע
	SCORE							m_lALLBetChip;						//������ע
	CChipArray						m_ArrayPlayChip;					//�����ע����
	CChipArray						m_ArrayBetChip;						//��ע����
	CChipArray						m_ArrayOverChip;					//��������

	CRect							m_rectBetChip;						//��ע����
	CSize							m_sizeWin;							//���ڴ�С


	//λ�ñ���
protected:

	CPoint							m_ptUserAction[GAME_PLAYER];		//����λ��
	CPoint							m_ptPalyBetChip[GAME_PLAYER];		//�����ע
	CPoint							m_ptPalyPos[GAME_PLAYER];			//�����ע
	CPoint							m_ptOverScore[GAME_PLAYER];			//��������

	CPoint							m_ptUserInfo[GAME_PLAYER];			//�����Ϣ
	CPoint							m_ptLevel[GAME_PLAYER];
	CPoint							m_ptScore[GAME_PLAYER];

	//��Դ��С
protected:
	CSize							m_SizeHistory;						//���ִ�С
	CSize							m_SizeGameCard;						//�˿˴�С

	//��Դ����
protected:
	CPngImage						m_ImageReady;						// ׼��

	CPngImage						m_ImageGameCard;					// �˿���Դ
	CBitImage						m_ImageViewFill;					// ������Դ
	CPngImage						m_ImageViewBack;					// ������Դ

	CPngImage						m_ImageActionBack;					// ��������
	CPngImage						m_ImageActionFont;					// ����
	CPngImage						m_ImageShowHand;					// ���

	CPngImage						m_ImageChip;						// ����λͼ

	CPngImage						m_ImageCellScore;					// �ͷֱ���
	CPngImage						m_ImageTipInfo;						// С��ʿ

	CPngImage						m_ImageCellNumber;					// �ͷ�����
	CPngImage						m_ImgaeScoreTotalNumber;			// ��ע����

	CPngImage						m_ImgaeScoreBackR;					// ��������
	CPngImage						m_ImgaeScoreBackL;					// ��������
	CPngImage						m_ImgaeScoreBackM;					// ��������

	//CPngImage						m_ImgaeScoreSum;					// ��������
	CPngImage						m_ImgaeScoreWin;					// ��������
	CPngImage						m_ImgaeScoreLose;					// ��������
	CPngImage						m_ImageCradType;					// ����

	CPngImage						m_ImageUserInfo;
	CPngImage						m_ImageClock;

	//�������
	CDFontEx						m_Font;

	//12-7-31
	CPngImage						m_PngOffLine;
	CPoint							m_ptOffLine[GAME_PLAYER];

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���ƽӿ�
public:
	//���ý���
	virtual VOID ResetGameView();

	//�̳к���
protected:
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }

	//���ƺ���
public:
	//�Ƿ���
	bool IsDispatchStatus();
	//ֹͣ����
	VOID ConcludeDispatch();
	//�����˿�
	VOID DispatchUserCard(WORD wChairID, BYTE cbCardData);

	VOID SetUserCard(WORD wViewID,BYTE cbCardData[MAX_COUNT],BYTE cbCount);
	//���ܺ���
public:
	//��Ϸ��ע
	bool SetCellScore(SCORE lCellScore);
	//�����ע
	bool SetDrawMaxScore(SCORE lDrawMaxScore);
	//���û���
	bool SetUserScore(WORD wChairID, SCORE lUserScore);
	//���û���
	bool SetUserSaveScore(WORD wChairID, SCORE lUserScore);
	//���ý�������
	bool SetOverScore(WORD wChairID, SCORE lUserScore);
	//���ö���
	bool SetUserAction(WORD wActionUser, BYTE cbUserAction);
	//������ע
	void SetUserTableScore(WORD wChairID, SCORE lTableScore);
	// ���������ע
	void AddDeskChip( SCORE lScore, WORD wChairID );
	// ��������ע
	void AddPlayChip( SCORE lScore, WORD wChairID );
	// �����Ҽ�ע
	void AddPlayBetChip( SCORE lScore, WORD wChairID );
	// �������
	void ChioRecovery( WORD wChairID );

	
	//�ڲ�����
private:
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	// �滭�ַ�
	bool DrawTextStringEx(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, INT nDrawFormat);
	// �滭��������
	void DrawOverScore(CDC * pDC, SCORE lOutNum, INT nXPos, INT nYPos );
	// ��Ӷ���
	CString AddComma( LONGLONG lScore );
	//ʱ�����
	void DrawUserClockEx(CDC *pDC,INT nXPos, INT nYPos,WORD wChairID);
	//��ť��Ϣ
public:
	//��ʼ��ť
	VOID OnBnClickedStart();
	//����Ա����
	VOID OpenAdminWnd();

	//��Ϣ����
public:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	// ��Ϣ����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif