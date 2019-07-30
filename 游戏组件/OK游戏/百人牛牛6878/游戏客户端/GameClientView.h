#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "ApplyUserList.h"
#include "RecordGameList.h"
#include "SiceControl.h"
#include "ZhuangControl.h"
#include "ClientControl.h"
#include "..\��Ϸ������\GameLogic.h"
#ifdef __BANKER___
//#include "DlgBank.h"
#endif

//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				8									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define IDM_CONTINUE_CARD			WM_USER+202							//��������
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//��ť��ʶ
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_CHANGE_MUSIC			WM_USER+205		

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

#define MAX_APPLY_DISPLAY			6									//���������ʾ����
//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
#define MAX_SCORE_HISTORY			65									//��ʷ����
#define MAX_FALG_COUNT				15									//��ʶ����

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
};

//�������
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//��¼��Ϣ
struct tagClientGameRecord
{
	enOperateResult					enOperateTianMen;					//������ʶ
	enOperateResult					enOperateDiMen;						//������ʶ
	enOperateResult					enOperateXuanMen;					//������ʶ
	enOperateResult					enOperateHuangMen;					//������ʶ
	bool							bWinTianMen;						//˳��ʤ��
	bool							bWinDiMen;							//����ʤ��
	bool							bWinXuanMen;						//����ʤ��
	bool							bWinHuangMen;						//����ʤ��
};

//������ʾ
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//��������
	enDispatchCardTip_Dispatch											//����ϴ��
};

//�ͻ��˻������·�
struct tagAndroidBet
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	WORD							wChairID;							//���λ��
	int								nLeftTime;							//ʣ��ʱ�� (100msΪ��λ)
};

//��������
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
    LONGLONG						m_lAreaLimitScore;					//��������

	//��ע��Ϣ
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//������ע
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//ȫ����ע
	
	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcTianMen;						//�м�����
	CRect							m_rcDimen;							//����������
	CRect							m_rcQiao;							//��������
	CRect							m_rcHuangMen;						//ƽ����
	CRect							m_rcXuanMen;						//ͬ��ƽ����
	CRect							m_rcHuang;							//ͬ��ƽ����
	CRect							m_rcJiaoR;							//ׯ������
	int								m_OpenCardIndex;					//�_�����
	int								m_PostCardIndex;					//�l�����
	int								m_PostStartIndex;					//������ʼλ��
	CPoint							m_CardTypePoint[AREA_COUNT+1];

	//�˿���Ϣ
public:	
    BYTE							m_cbTableCardArray[5][5];			//�����˿�
	BYTE							m_cbTableSortCardArray[5][5];		//�����˿�
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard[2];
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//���Զ�
	int								m_lUserCardType[AREA_COUNT+1];		//������ע
	BYTE							m_cbUserCardType[AREA_COUNT+1];		//����

	//��ʷ��Ϣ
protected:
	SCORE						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	//״̬����
public:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������
	bool							m_bWinTianMen;						//ʤ����ʶ
	bool							m_bWinHuangMen;						//ʤ����ʶ
	bool                            m_bWinDiMen;
	bool							m_bWinXuanMen;						//ʤ����ʶ
	bool							m_bFlashResult;						//��ʾ���
	bool							m_bShowGameResult;					//��ʾ���
	enDispatchCardTip				m_enDispatchCardTip;				//������ʾ

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	LONGLONG						m_lBankerWinScore;					//ׯ�ҳɼ�	
	LONGLONG						m_lTmpBankerWinScore;				//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//���ֳɼ�
public:
	SCORE						m_lMeCurGameScore;					//�ҵĳɼ�
	SCORE						m_lMeCurGameReturnScore;			//�ҵĳɼ�
	SCORE						m_lBankerCurGameScore;				//ׯ�ҳɼ�
	SCORE						m_lGameRevenue;						//��Ϸ˰��

	//���ݱ���
protected:
	CPoint							m_PointJetton[AREA_COUNT];			//����λ��
	CPoint							m_PointJettonNumber[AREA_COUNT];	//����λ��
	CJettonInfoArray				m_JettonInfoArray[JETTON_COUNT];	//��������

	//�ؼ�����
public:
	CSkinButton						m_btJetton100;						//���밴ť
	CSkinButton						m_btJetton1000;						//���밴ť
	CSkinButton						m_btJetton10000;					//���밴ť
	CSkinButton						m_btJetton50000;					//���밴ť
	CSkinButton						m_btJetton100000;					//���밴ť
	CSkinButton						m_btJetton500000;					//���밴ť
	CSkinButton						m_btJetton1000000;					//���밴ť
	CSkinButton						m_btJetton5000000;					//���밴ť
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��
	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�
	CSkinButton						m_btAutoOpenCard;					//�Զ�����
	CSkinButton						m_btOpenCard;						//�ֶ�����
	CSkinButton						m_btBank;							//���а�ť
	CSkinButton						m_btContinueCard;					//��������
	CSkinButton						m_btUp;								//��ť
	CSkinButton						m_btDown;							//ȡ�ť
	CSkinButton						m_btLast;							//��һ��
	CSkinButton						m_btNext;							//��һ��
public:
	CButton							m_btOpenAdmin;						//��������
	//�ؼ�����
public:
#ifdef __BANKER___
	//CDlgBank						m_DlgBank;							//���пؼ�
// 	CSkinButton						m_btBankerStorage;					//��ť
// 	CSkinButton						m_btBankerDraw;						//ȡ�ť
#endif
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�
	CCardControl					m_CardControl[6];					//�˿˿ؼ�	
	CGameClientEngine					*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CRect							m_MeInfoRect;

	//����
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;				

	//�������
protected:
	CBitImageEx							m_ImageViewFill;					//����λͼ
	//CBitImageEx					m_ImageViewBack;					//����λͼ
	CBitmap							m_ImageViewBack;					//����λͼ
	CBitmap							m_ImageWinFlags;					//��־λͼ
	//CBitImageEx						m_ImageJettonView;					//������ͼ
	CBitmap							m_ImageJettonView;
	CBitImageEx						m_ImageScoreNumber;					//������ͼ
	//CBitImageEx						m_ImageMeScoreNumber;				//������ͼ
	CBitmap							m_ImageMeScoreNumber;					//������ͼ
	CBitmap							m_ImageTimeFlag;					//ʱ���ʶ
	CBitmap							m_ImageDispatchCardTip;				//������ʾ
	CBitmap							m_ImageCardType[AREA_COUNT+1];		//������ͼ

	//�߿���Դ
protected:
	CBitmap						m_ImageFrameTianMen;				//�߿�ͼƬ
	CBitmap						m_ImageFrameDiMen;					//�߿�ͼƬ
	CBitmap						m_ImageFrameHuangMen;				//�߿�ͼƬ
	CBitmap						m_ImageFrameXuanMen;				//�߿�ͼƬ

	CBitImageEx						m_ImageMeBanker;					//�л�ׯ��
	CBitImageEx						m_ImageChangeBanker;				//�л�ׯ��
	CBitImageEx						m_ImageNoBanker;					//�л�ׯ��

	CSiceControl					m_Sice[2];							///����
	CZhuangControl					m_Zhuang;							///ׯ��


	//������Դ
protected:
	CBitmap						m_ImageGameEnd;						//�ɼ�ͼƬ

	//������Դ
public:
	CDFontEx						m_FontValleysListOne;				//��ׯ�б�����һ
	CDFontEx						m_FontValleysListTwo;				//��ׯ�б������

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7֧��
	virtual bool RealizeWIN7() { return false; }

	//���ú���
public:
	//������Ϣ
	void SetMeMaxScore(LONGLONG lMeMaxScore);
	//�ҵ�λ��
	void SetMeChairID(DWORD dwMeUserID);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore, SCORE lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbTableCardArray[5][5]);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(bool bWinTian, bool bWinDi, bool bWinXuan,bool bWinHuang);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//������ʾ
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);
	///��Ϸ״̬�ı�
	void OnStateChange(BYTE state);

	void SetFirstShowCard(BYTE *bcCard);
	//ȡ������
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }

	//������ͼ
	void RefreshGameView();

	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard(bool bNotScene);
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//��ֹ����
	void StopAnimal();

	//��������
public:
	//�����ע
	LONGLONG GetUserMaxJetton();

	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(bool bWinTian, bool bWinDi, bool bWinXuan,bool bWinHuang, bool bSet);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//�滭ׯ��
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//�滭���
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);

	//��������
public:
	//��ǰ����
	LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }

	void SetMoveCardTimer();
	void KillCardTime();

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat=-1);
	void	DrawNumberStringWithDot(CDC *pDC,DOUBLE lScore,CRect rcRect,INT nFormat = -1);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);

	//
	void DrawType(CDC* pDC,WORD wChairID);
	//�ƶ�Ӯ��
	void DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple );


	//��ť��Ϣ
protected:
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();
	//�ֹ�����
	afx_msg void OnOpenCard();
	//�Զ�����
	afx_msg void OnAutoOpenCard();
	//���д��
	afx_msg void OnBankStorage();
	//����ȡ��
	afx_msg void OnBankDraw();
	//��ҳ��ť
	afx_msg void OnUp();
	//��ҳ��ť
	afx_msg void OnDown();

	//���д��
	afx_msg void OnLastMusic();
	//����ȡ��
	afx_msg void OnNextMusic();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
    //�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�ؼ�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    //����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//�����ؼ����UP��Ϣ
	LRESULT OnViLBtUp(WPARAM wParam, LPARAM lParam);
	//����Ա����
	afx_msg void OpenAdminWnd();

	DECLARE_MESSAGE_MAP()	

};

//////////////////////////////////////////////////////////////////////////

#endif
