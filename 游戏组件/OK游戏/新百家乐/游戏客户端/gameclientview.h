#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "GameLogic.h"
#include "DlgViewChart.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////

//���붨��
#define BET_COUNT					7									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����confirm

#define IDM_PALY_BET				WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define IDM_PLAY_SOUND				WM_USER+202							//������Ϣ
#define IDM_CONFIRM_BET				WM_USER+203							//������Ϣ
#define IDM_CANCEL_BET				WM_USER+204							//������Ϣ
#define IDM_CHANGE_MUSIC			WM_USER+205							//�л�����

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagBetInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbBetIndex;							//��������
};

//������ʾ
struct tagLimitTip
{
	int								nXPos;								//λ��
	int								nYPos;								//λ��
	int								nAlpha;								//͸��
	BYTE							cbAreaIndex;						//����
};

//ѡȡ������Ϣ
struct tagFlashInfo
{
	BYTE							cbFlashArea;					//��ʾ����
	int								nFlashAreaAlpha;				//��ʾ͸����
	bool							bFlashAreaAlpha;				//�Ƿ���ʾ
};

enum enFlexMode
{
	enFlexNULL,
	enFlexBetTip,
	enFlexDealCrad,
	enFlexGameEnd,
};

//��������
typedef CWHArray<tagBetInfo,tagBetInfo&> CBetInfoArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
    LONGLONG						m_lAreaLimitScore;					//��������


	//��ע��
protected:
	LONGLONG						m_lAllBet[AREA_MAX];				//����ע
	LONGLONG						m_lPlayBet[AREA_MAX];				//�����ע
	//LONGLONG						m_lCurrentScore[AREA_MAX];			//��ǰ��ע

	//λ����Ϣ
protected:
	CWHArray<CRect>					m_ArrayBetArea[AREA_MAX];			//��ע����
	CPoint							m_ptBetFrame[AREA_MAX];				//��ע��ʾλ��
	CSize							m_sizeWin;							//���ڴ�С

	//�˿���Ϣ
protected:	
	BYTE							m_cbCardCount[2];					//�˿���Ŀ
    BYTE							m_cbTableCardArray[2][3];			//�����˿�

	//��������
protected:
	int								m_nDealMove;						//���Ʋ���
	int								m_nDealIndex;						//��������
	CPoint							m_ptDispatchCard;					//����λ��
	CCardControl					m_DispatchCard;						//�����˿�

	//��������
protected:
	enFlexMode						m_enFlexMode;						//��������
	int								m_nFlexMove;						//��������
	bool							m_bFlexShow;						//��ʾ
	CPoint							m_ptFlexBeing;						//�ƶ�λ��
	CPoint							m_ptFlexMove;						//�ƶ�λ��
	CPngImage*						m_pImageFlex;						//�ƶ�ͼƬ

	//��ʷ��Ϣ
protected:
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼
	int								m_nScoreHead;						//�ɼ�λ��

	//״̬����
protected:
	LONG							m_nWinCount;						//Ӯ����
	LONG							m_nLoseCount;						//������
	BYTE							m_cbGameStatus;						//��Ϸ״̬
	WORD							m_wMeChairID;						//�ҵ�λ��
	LONGLONG						m_lCurrentBet;						//��ǰ����
	CString							m_strDispatchCardTips;				//������ʾ
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������

	//������
protected:
	CWHArray<BYTE>					m_ArrayFlashArea;					//ʤ�����
	int								m_nFlashAreaAlpha;					//ʤ����ʾ
	bool							m_bFlashAreaAlpha;					//ʤ����ʾ
	CWHArray<tagFlashInfo>			m_ArrayCurrentFlash;				//ѡ������

	//������ʾ
protected:
	CWHArray<tagLimitTip>			m_ArrayLimitTip;					//������ʾ

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	LONGLONG						m_lBankerWinScore;					//ׯ�ҳɼ�	
	LONGLONG						m_lBankerWinTempScore;				//ׯ�ҳɼ�[��ʱ����]
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//���ֳɼ�
public:
	LONGLONG						m_lPlayScore[AREA_MAX];				//�����Ӯ
	SCORE							m_lPlayAllScore;					//�������Ӯ

	//��������
public:
	int								m_nWinShowArea;						//Ӯ��������
	int								m_nWinShowIndex[3];					//Ӯ��������

	//���ݱ���
protected:
	CPoint							m_ptBetNumber[AREA_MAX];			//����λ��
	CPoint							m_ptAllBetNumber[AREA_MAX];			//����λ��
	CBetInfoArray					m_BetInfoArray[AREA_MAX];			//��������

protected:
	WORD							m_wSystemFaceID;

	//�ؼ�����
public:
	CSkinButton						m_btBet1;							//���밴ť
	CSkinButton						m_btBet3;							//���밴ť
	CSkinButton						m_btBet5;							//���밴ť
	CSkinButton						m_btBet10;							//���밴ť
	CSkinButton						m_btBet50;							//���밴ť
	CSkinButton						m_btBet100;							//���밴ť
	CSkinButton						m_btBet1000;						//���밴ť

	//CSkinButton						m_btBetComfirm;						//����ȷ��
	CSkinButton						m_btBetCancel;						//����ȡ��

	CSkinButton						m_btViewChart;						//�鿴·��
	
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��

	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�

	CSkinButton						m_btValleysUp;						//��ť
	CSkinButton						m_btValleysDown;					//��ť
	CSkinButton						m_btLast;								//��ť
	CSkinButton						m_btNext;							//ȡ�ť

	CButton							m_btOpenAdmin;


	//������Դ
public:
	CDFontEx*						m_FontCurrent;						//��ǰ����
	CDFontEx						m_FontNormal;						//��������
	CDFontEx						m_FontValleysListOne;				//��ׯ�б�����һ
	CDFontEx						m_FontValleysListTwo;				//��ׯ�б������

	//�ؼ�����
public:
	INT								m_nShowValleyIndex;					//��ʾ��ׯ�б�
	CWHArray<WORD>					m_ValleysList;						//��ׯ�б�			
	CCardControl					m_CardControl[2];					//�˿˿ؼ�	
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CDlgViewChart					m_DlgViewChart;						//�鿴·��

	//�������
protected:
	CBitImage						m_ImageViewFill;					//����λͼ
	CPngImage						m_ImageViewBack;					//����λͼ
	CPngImage						m_ImageViewBackJB;					//����λͼ
	CPngImage						m_ImageWinFlags;					//��־λͼ
	CPngImage						m_ImageBetView;						//������ͼ
	CPngImage						m_ImageScoreBack;					//������ͼ
	CPngImage						m_ImageScoreNumber;					//������ͼ
	CPngImage						m_ImageMeScoreBack;					//������ͼ
	CPngImage						m_ImageMeScoreNumber;				//������ͼ


	//�߿���Դ
protected:
	CPngImage						m_ImageFrame[AREA_MAX];				//�߿�ͼƬ

	CPngImage						m_ImageMeBanker;					//�л�ׯ��
	CPngImage						m_ImageChangeBanker;				//�л�ׯ��
	CPngImage						m_ImageNoBanker;					//�л�ׯ��
	CPngImage						m_ImageNodeBanker;					//�л�ׯ��
	

	//��������
protected:
	CPngImage						m_ImageBetTip;						//��ע��ʾ
	CPngImage						m_ImageBankerTip;					//ׯ����ʾ
	CPngImage						m_ImageGameEnd;						//�ɼ�����
	CPngImage						m_ImageDealBack;					//���Ʊ���

	//������Դ
protected:
	CPngImage						m_ImageGamePoint;					//����ͼƬ
	CPngImage						m_ImageGameEndMyScore;				//��ҽ�������
	CPngImage						m_ImageGameEndAllScore;				//��ҽ�������
	CPngImage						m_ImageGameEndPoint;				//�����������

	CPngImage						m_ImageWinType;						//��Ӯ����
	CPngImage						m_ImageWinXian;						//����
	CPngImage						m_ImageWinZhuang;					//ׯ��


	//�������
protected:
	CPngImage						m_ImageBrandBoxRight;				//�ƺ���

	CPngImage						m_ImageTimeNumber;					//ʱ������
	CPngImage						m_ImageTimeBack;					//ʱ�䱳��
	CPngImage						m_ImageTimeType;					//ʱ���ʶ
	CPngImage						m_ImageTimeFlag;					//ʱ���ʶ

	CPngImage						m_ImagePlayLeft;					//�����Ϣ��
	CPngImage						m_ImagePlayMiddle;					//�����Ϣ��
	CPngImage						m_ImagePlayRight;					//�����Ϣ��

	CPngImage						m_ImageScoreInfo;					//������Ϣ
	CPngImage						m_ImageBankerInfo;					//ׯ����Ϣ
	CPngImage						m_ImageWaitValleys;					//�ȴ���ׯ
	CPngImage						m_ImageWaitFirst;					//��һ�ȴ�

	CPngImage						m_ImageLimitTip;					//������ʾ

	//����
public:
	HINSTANCE						m_hControlInst;
	IClientControlDlg*				m_pClientControlDlg;


	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���ƽӿ�
public:
	//���ý���
	virtual VOID ResetGameView();

	//�̳к���
private:
	//�����ؼ�
	virtual VOID RectifyControl(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }


	//���ú���
public:
	//����״̬
	VOID SetGameStatus(BYTE cbGameStatus);
	//�ҵ�λ��
	void SetMeChairID(WORD wMeChairID);
	//���������ע
	void SetPlayBetScore(LONGLONG lPlayBetScore);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);
	//ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore);
	//ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore, LONGLONG lBankerWinScore, WORD wBankerTime);
	//ׯ����Ϣ
	void SetBankerOverInfo(LONGLONG lBankerWinScore, WORD wBankerTime);
	//����ϵͳ�Ƿ���ׯ
	void SetEnableSysBanker(bool bEnableSysBanker);
	//������ע
	void SetPlayBet(BYTE cbViewIndex, LONGLONG lBetCount);
	//ȫ����ע
	void SetAllBet(BYTE cbViewIndex, LONGLONG lBetCount);
	//���ó���
	void AddChip(BYTE cbViewIndex, LONGLONG lScoreCount);
	//���ֳɼ�
	void SetCurGameScore(LONGLONG lPlayScore[AREA_MAX], SCORE lPlayAllScore);
	//�����˿�
	void SetCardInfo(BYTE cbCardCount[2], BYTE cbTableCardArray[2][3]);
	//���ó���
	void SetCurrentBet(LONGLONG lCurrentBet);
	//��ʷ��¼
	void SetGameHistory(enOperateResult OperateResult, BYTE cbPlayerCount, BYTE cbBankerCount, BYTE cbKingWinner, bool bPlayerTwoPair, bool bBankerTwoPair);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�Ƿ���ע
	BOOL IsHasBet();
	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard(bool bScene = false);
	//��������
	void FlexAnimation(enFlexMode nFlexMode, bool bShow, bool bMove = true);
	//��˸����
	void FlashAnimation( bool bBegin );


	//�滭����
protected:
	// �滭������Ϣ
	void DrawTopInfo(CDC *pDC, int nWidth, int nHeight);
	// �滭�ײ���Ϣ
	void DrawBottomInfo(CDC *pDC, int nWidth, int nHeight);
	//��ʾ��Ӯ
	void DrawWinType(CDC *pDC, int nWidth, int nHeight, int nBeginX, int nBeginY );
	//��ʾ���
	void DrawGameOver(CDC *pDC, int nWidth, int nHeight, int nBeginX, int nBeginY );
	// ��˸�߿�
	void DrawFlashFrame(CDC *pDC, int nWidth, int nHeight);

	//��������
public:
	//�����ע
	LONGLONG GetMaxPlayerScore(BYTE cbBetArea);

	//���溯��
public:
	//�������
	void CleanUserBet();
	//������ʾ
	void SetDispatchCardTips();
	//ϵͳͷ��
	void SetSystemFaceID(WORD wFaceId){m_wSystemFaceID =wFaceId;}
	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentBet() { return m_lCurrentBet; }	
	//��ǰ����
	//inline LONGLONG GetCurrentBet(BYTE cbArea){return m_lCurrentScore[cbArea];}
	//���õ�ǰ����
	//void SetCurrentBet(WORD cbArea,LONGLONG lscore);

	void CleanAreaChip(BYTE index);
	//�ڲ�����
private:
	//��ȡ����
	BYTE GetBetArea(CPoint MousePoint);
	//�ƶ�Ӯ��
	void DeduceWinner(BYTE* pWinArea);
	//�ƶ�Ӯ��
	void DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair);
	// ��Ӷ���
	CString AddComma( LONGLONG lScore , bool bPlus = false);
	// ��Ӷ���
	CString AddCommaDOUBLE( DOUBLE lScore , bool bPlus = false);
	// ɾ������
	LONGLONG DeleteComma( CString strScore );

	//���õ�ǰ����
	void SetCurrentFont( CDFontEx* pFontCurrent ) { m_FontCurrent = pFontCurrent; }

	//�滭���亯��
private:
	//�滭ͷ��
	bool DrawUserFace(CDC * pDC, INT nXPos, INT nYPos,INT nWidth,INT nHeight, IClientUserItem * pIClientUserItem);
	//��������
	void DrawTextStringE(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭�ַ�
	void DrawTextStringE(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	// �滭ʱ��
	void DrawTime(CDC * pDC, WORD wUserTime,INT nXPos, INT nYPos);

	//��ť��Ϣ
protected:
	afx_msg void OnLastMusic();
	afx_msg void OnNextMusic();

	//���밴ť
	afx_msg void OnBetButton1();
	//���밴ť
	afx_msg void OnBetButton3();
	//���밴ť
	afx_msg void OnBetButton5();
	//���밴ť
	afx_msg void OnBetButton10();
	//���밴ť
	afx_msg void OnBetButton50();
	//���밴ť
	afx_msg void OnBetButton100();
	//���밴ť
	afx_msg void OnBetButton1000();
	//�鿴·��
	afx_msg void OnViewChart();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();
	//�Ϸ�ҳ
	afx_msg void OnValleysUp();
	//�·�ҳ
	afx_msg void OnValleysDown();
	//����Ա����
	afx_msg void OpenAdminWnd();
	//ȷ����ע
	//afx_msg void OnComfirmBet();
	//ȡ����ע
	afx_msg void OnCancelBet();
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
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
