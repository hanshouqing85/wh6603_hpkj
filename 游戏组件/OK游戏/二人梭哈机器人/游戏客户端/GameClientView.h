#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//???￠?¨ò?
#define IDM_START						WM_USER+100						//?aê????￠
#define IDM_ADD_GOLD					WM_USER+101						//?ó×￠???￠
#define IDM_FOLLOW						WM_USER+102						//?ú×￠???￠
#define IDM_GIVE_UP						WM_USER+103						//·??ú???￠
#define IDM_SHOWHAND					WM_USER+104						//?ó1t???￠
#define IDM_SEND_CARD_FINISH			WM_USER+106						//·￠??íê3é
#define IDM_USE_WINNER                 (WM_USER + 204)                  // í??ò????
#define IDM_LOCK_GAME					WM_USER+107
#define IDM_SHOWCARD                    WM_USER+108                      //看牌

#define MYSELF_VIEW_ID					1								//×??o×ù??

//////////////////////////////////////////////////////////////////////////
//?á11?¨ò?

//·￠??×ó??
struct tagSendCardItem
{
	WORD							wChairID;							//·￠??ó??§
	BYTE							cbCardData;							//·￠??êy?Y
};

struct	tagUserWinScore
{
	WORD	wChairID;
	DWORD	dwUserID;
	TCHAR	tszUserName[128];
	__int64	lUserWinScore;
};

//êy×é?μ?÷
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//ó??·êóí?
class CGameClientView : public CGameFrameViewGDI
{
	//ó??a?¨ò?
	friend class CGameClientEngine;

	//êóí?±?á?
protected:
	__int64							m_lUserGold[GAME_PLAYER];			//
	__int64							m_lMaxGold;							//×?????×￠
	__int64							m_lBasicGold;						//×?éù·?êy
	CPoint							m_GoldStation[GAME_PLAYER*2];		//3???????
	bool							m_bShowScoreControl;				//
	bool							m_bUserShowHand;					//
	bool							m_bShowCard;						//是否看牌
	WORD							m_wShowCardChairID;					//看牌用户椅子号
public:
	//WORD							wServerID;
	tagUserWinScore					m_UserWinScore[GAME_PLAYER];		//玩家输赢统计

	bool							m_bSuperUser;
	//?ˉ?-±?á?
protected:
	CPoint							m_SendCardPos;						//·￠??????
	CPoint							m_SendCardCurPos;					//·￠??μ±?°????
	INT								m_nStepCount;						//2?êy
	INT								m_nXStep;							//2?3¤
	INT								m_nYStep;							//2?3¤
	CSendCardItemArray				m_SendCardItemArray;				//·￠??êy×é
	CRect                           m_rcCtrlWinner[GAME_PLAYER];        // ????ê?ó?

	//????±?á?
protected:
	CBitImage						m_ImageCardBack;					//????×ê?′
//	CBitImageEx						m_ImageTitle;						//±êìa??í?
	CBitImage						m_ImageViewFill;					//±3?°??í?
	CBitImageEx						m_ImageUserReady;					//UserBack
	CBitImageEx						m_ImageTimeFrame;
	CBitImageEx						m_ImageViewCenter;					//±3?°??í?
	//CBitImageEx						m_ImageScoreControlBack;			//
	CBitImageEx						m_ImageScoreTable;					//
	CPngImage						m_PngShowHand;						//
	CSkinButton						m_btLockGame;
	CPngImage						m_ImageTableScore;					//游戏输赢分数
	//???t±?á?
public:
	CSkinButton						m_btStart;							//?aê?°′?￥
	CSkinButton						m_btFollow;							//?ú×￠°′?￥
	CSkinButton						m_btGiveUp;							//·??ú°′?￥
	CSkinButton						m_btShowHand;						//?ó1t°′?￥
	CSkinButton						m_btAddTimes3;						//
	CSkinButton						m_btAddTimes2;						//
	CSkinButton						m_btAddTimes1;						//
	CGoldView						m_GoldView[8];						//ó??§3???
	CGoldView						m_TotalGoldView;					//
	CScoreView						m_ScoreView;						//?y·?êóí?
	CCardControl					m_CardControl[GAME_PLAYER];			//???????t
	CGameClientEngine					*m_pGameClientDlg;					//??àà????

	CFont							ViewFont;
	CFont							ViewFont1;
	//???t????
protected:
	CPoint							m_ptScoreControl;					//

	//o¡¥¨ºy?¡§¨°?
public:
	//11?ìoˉêy
	CGameClientView();
	//??11oˉêy
	virtual ~CGameClientView();

	//?ì3Doˉêy
private:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7支持
	virtual bool RealizeWIN7() { return true; }
	void DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime);
	//1|?üoˉêy
public:
	//?e±ò±êìa
	void SetGoldTitleInfo(__int64 lMaxGold, __int64 lBasicGold,bool bPoor);
	//ó??§3???
	void SetUserGoldInfo(WORD wViewChairID, bool bTableGold, __int64 dwGold);
	//看牌
	void ShowCard(WORD wChairID,bool bShowFirstCard); 
	//?ˉ?-?D??
	bool IsDispatchCard();
	//íê3é·￠??
	void FinishDispatchCard();
	//·￠?í????
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//
	VOID ShowScoreControl( bool bShow );
	//
	void SetUserShowHand( bool bShowHand );
	//
	void SetUserGold( WORD wChairId, __int64 lGold );
	void OnLockGame(const void * pBuffer, WORD wDataSize);
	//???￠ó3é?
protected:
	//?aê?°′?￥
	afx_msg void OnStart();
	//?ú×￠°′?￥
	afx_msg void OnFollow();
	//·??ú°′?￥
	afx_msg void OnGiveUp();
	//?ó×￠°′?￥
	afx_msg void OnAddTimes1();
	//?ó×￠°′?￥
	afx_msg void OnAddTimes2();
	//?ó×￠°′?￥
	afx_msg void OnAddTimes3();
	afx_msg void OnBnClickedLockGame();
	//?ó1t°′?￥
	afx_msg void OnShowHand();
	//?¨á￠oˉêy
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//?¨ê±?÷???￠
	afx_msg void OnTimer(UINT nIDEvent);
	//1a±ê???￠
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//êó±ê???￠
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//?üD?êóí?
	void RefreshGameView();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

};

//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
