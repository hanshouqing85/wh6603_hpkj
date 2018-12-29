#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ApplyUserList.h"
#ifdef  __BANKER___
#include "DlgBankEx.h"
#endif
#include "..\��Ϸ������\GameLogic.h"
#include "ClientControl.h"
#include "RgnButton.h"
//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define IDM_CONTINUE_CARD			WM_USER+202							//��������
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//��ť��ʶ
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_SHOW_RESULT				WM_USER+205		
#define IDM_SOUND					WM_USER+206		
#define IDM_CHANGE_MUSIC			WM_USER+207							//�л�����


//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
#define MAX_SCORE_HISTORY			65									//��ʷ����
#define MAX_FALG_COUNT				9									//��ʶ����
#define COUNT_DRAWINFO              18

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
	enOperateResult					enOperateMen[AREA_COUNT+1];					//������ʶ
	bool							bWinMen[AREA_COUNT+1];						//˳��ʤ��

};

//������ʾ
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//��������
	enDispatchCardTip_Dispatch											//����ϴ��
};

struct  sT_ShowInfo
{
	CRect RcRect[AREA_COUNT+1];
	CPoint point[AREA_COUNT+1];
	bool  blShow[AREA_COUNT+1];
};
struct  sT_ShowJetton
{
	bool  blShow[6];
	sT_ShowJetton()
	{
		for (int i = 0;i<6;i++){
			blShow[i]=true;
		}
	}
};
//��������
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

struct tagAndroidBet
{
	BYTE							cbJettonArea;						//��������
	SCORE						lJettonScore;						//��ע��Ŀ
	WORD							wChairID;							//���λ��
	int								nLeftTime;							//ʣ��ʱ�� (100msΪ��λ)
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
    LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lRobotMaxJetton;					//������������

	//��ע��Ϣ
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//������ע
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//ȫ����ע
	LONGLONG						m_JettonQue[COUNT_DRAWINFO];
	BYTE							m_JettonQueArea[COUNT_DRAWINFO];
	BYTE							m_JettonQueIndex;

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcTianMen;						//�м�����
	CRect							m_rcDimen;							//����������
	CRect							m_rcQiao;							//��������
	CRect							m_rcHuangMen;							//ƽ����
	CRect							m_rcXuanMen;							//ͬ��ƽ����
	CRect							m_rcHuang;							//ͬ��ƽ����
	CRect							m_rcJiaoR;							//ׯ������
	int								m_OpenCardIndex;					//�_�����
	int								m_PostCardIndex;					//�l�����
	int								m_PostStartIndex;					//������ʼλ��
	CPoint							m_CardTypePoint[AREA_COUNT+1];
   

	//�˿���Ϣ
public:	
    BYTE							m_cbTableCardArray[1][1];			//�����˿�
	BYTE							m_cbTableSortCardArray[1][1];			//�����˿�
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//���Զ�
	SCORE						m_lUserCardType[AREA_COUNT+1];	//������ע
	CRect							m_JettonAreaRect[AREA_COUNT];
	CRect m_cTmpRect[64];
	sT_ShowJetton                   m_T_ShowJetton;

	//��ʷ��Ϣ
protected:
	SCORE						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼
	int                             m_szpm;                             //��ׯ����
	float                             dbsj,dbsjdt;                               //������¼����
	float                             xbsj,xbsjdt;
	float                             xbm,xbmdt;
	float                             dbm,dbmdt;
	float                             xbc,xbcdt;
	float                             dbc,dbcdt;
	float                             xdz,xdzdt;
	float                             ddz,ddzdt;
	//״̬����
protected:
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
	CPngImage						m_ViewBackPng;
	CPngImage                       m_ImageZuoshang;
	CPngImage                      m_ImageYoushang;
	CPngImage                       m_ImageZuoxia;
	CPngImage                       m_ImageYouxia;
	

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	SCORE						m_lBankerScore;						//ׯ�һ���
	SCORE						m_lBankerWinScore;					//ׯ�ҳɼ�	
	SCORE						m_lTmpBankerWinScore;				//ׯ�ҳɼ�	
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
	CJettonInfoArray				m_JettonInfoArray[AREA_COUNT];		//��������
	bool							m_bWinFlag[AREA_COUNT];						//ʤ����ʶ
	int								m_TopHeight;
	int								m_LifeWidth;
	int								m_CurArea;
	int                             m_nApplyPageCount;                  //�����б�ҳ��
	int iP;     //�Ƕ�
    int iP1;
	//��ע���� 2012-9-12
	CRect							m_RectArea[8];
	CPngImage						m_ImageCh[8];                   
	//��������ע
	CWHArray<tagAndroidBet>	m_ArrayAndroid;

	//�ؼ�����
public:

	
	CRgnButton						m_btJetton100;						//���밴ť
	CRgnButton						m_btJetton1000;						//���밴ť
	CRgnButton						m_btJetton10000;					//���밴ť
	//CRgnButton						m_btJetton50000;					//���밴ť
	CRgnButton						m_btJetton100000;					//���밴ť
	CRgnButton						m_btJetton1000000;					//���밴ť
	CRgnButton						m_btJetton5000000;					//���밴ť
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��
	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�

// 	CSkinButton						m_btBankStorage;					//��ť
// 	CSkinButton						m_btBankDraw;						//ȡ�ť
	CSkinButton						m_btUp;								//��ť
	CSkinButton						m_btDown;							//ȡ�ť
	CButton							m_btOpenAdmin;						//ϵͳ����
	CSkinButton						m_btLast;								//��ť
	CSkinButton						m_btNext;							//ȡ�ť

	int								m_ShowImageIndex;
	int								m_CheckImagIndex;
	//�ؼ�����
public:
//	CApplyUser						m_ApplyUser;						//�����б�
	CWHArray<tagApplyUser*>          m_ApplyUser;
	
	CGameClientDlg					*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
#ifdef  __BANKER___
	CDlgBank						m_DlgBank;							//���пؼ�
#endif
	CRect							m_MeInfoRect;						//
	int								m_Out_Bao_y;
	bool						    m_bShowBao;
	CRect							m_CarRect[36];	
	int								m_carX;
	int								m_carY;
	int								m_carX1;
	int								m_carY1;
	int                             m_carX2;
	int                             m_carY2;
	int								m_carX3;
	int								m_carY3;
	CPngImage						m_idb_selPng;
	int								m_CarIndex ;
	int  m_CarIndexod;
	int                             m_js;
	//����
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;	

	//����
public:
	bool							m_blCanStore;                       //�Ƿ��ܱ���
	bool                               m_winflag;							//�滭�н�������־
	int  qisx; 
	int  chsxx;
	int  chsxy;

	void                            InsertApplyUser(tagApplyUser* pApplyUser);
	void                            UpdateApplyUser(tagApplyUser* pApplyUser);
	void                            RemoveApplyUser(tagApplyUser* pApplyUser);
	//�������
protected:
	CBitImage						m_ImageViewFill;					//����λͼ
	//CBitImage						m_ImageViewcentre;					//����λͼ
	CPngImage						m_ImageWinFlags;					//��־λͼ
	CPngImage						m_ImageJettonView;					//������ͼ
	CPngImage                       m_ImageXdz;                         //С����
	CPngImage                       m_ImageDbc;                         //�󱼳�
	CPngImage                       m_ImageDbm;                         //����
	CPngImage                       m_ImageDbsj;                        //��ʱ��
	CPngImage                       m_ImageDdz;                         //�����
	CPngImage                       m_ImageXbc;                         //С����
	CPngImage                       m_ImageXbm;                         //С����
	CPngImage                       m_ImageXbsj;                        //С��ʱ��
	CBitImage						m_ImageScoreNumber;					//������ͼ
	CBitImage						m_ImageMeScoreNumber;				//������ͼ

	CPngImage                       m_ImageCar;                         //С����
	CPngImage                       m_ImageChdzqz;                     //��������
	CPngImage                       m_ImageChbcqz;                     //��������
	CPngImage                       m_ImageChbmqz;                     //��������
	CPngImage                       m_ImageChbsjqz;                     //��������
	CPngImage                       m_ImageChdbcdh;                     //�д󱼳�
	CPngImage                       m_ImageChddzdh;                     //�д����
	CPngImage                       m_ImageChdbmdh;                     //�д���
	CPngImage                       m_ImageChdbsdh;                     //�д�ʱ��
	CPngImage                       m_ImageChxbci;                     //��С����
	CPngImage                       m_ImageChxdzo;                     //��С����
	CPngImage                       m_ImageChxbma;                     //��С����
	CPngImage                       m_ImageChxbsj;                     //��С��ʱ��
	CPngImage                        m_Imagehkjbj;                     //�н�����
	
	CPngImage                       m_ImageRedcar;                    //��ɫ����
	CPngImage						m_ImageTimeFlagPng;					//ʱ���ʶ
	CBitImage						m_ImageDispatchCardTip;				//������ʾ
	CBitImage						m_ImageCardType[AREA_COUNT+1];		//������ͼ
	//CBitImage						m_ImageBao;							//������ʾ
	CRect							m_BaoPosion;

	//�߿���Դ
protected:

	CBitImage						m_ImageFrameTianMen;				//�߿�ͼƬ
	CBitImage						m_ImageFrameDiMen;					//�߿�ͼƬ
	CBitImage						m_ImageFrameHuangMen;				//�߿�ͼƬ
	CBitImage						m_ImageFrameXuanMen;				//�߿�ͼƬ
	CBitImage						m_ImageFrameQiao;					//�߿�ͼƬ
	CBitImage						m_ImageFrameJiaoR;					//�߿�ͼƬ
	CBitImage						m_ImageMeBanker;					//�л�ׯ��
	CBitImage						m_ImageChangeBanker;				//�л�ׯ��
	CBitImage						m_ImageNoBanker;					//�л�ׯ��
	bool							m_blMoveShowInfo;
	Image							*m_pImage;
	Image							*m_pImage1;

protected:

	CPngImage                          m_PngPushBox[7];
	CPngImage                          m_PngShowJetton[4];
	bool							   m_bShowJettonAn;
	int                                m_bShowJettonIndex;
	CPngImage                          m_PngShowLeaveHandle[4];
	bool							   m_bShowLeaveHandleAn;
	int                                m_bShowLeaveHandleIndex;
	bool							   m_bOPenBoxAn;
	int                                m_bOPenBoxIndex;
	bool                               m_blShowLastResult;

	CPngImage                          m_PngResult[4];
	CPngImage                          m_TimerCount_png;
	CPngImage						   m_ImageMeScoreNumberPng;				//������ͼ
	CPngImage						   m_ImageScoreNumberPng;

	bool							   m_bShowResult;
	bool                               m_blShowResultIndex;
	SCORE						   m_lLastJetton;


	//������Դ
protected:
	CPngImage						m_pngGameEnd;
	bool							m_DrawBack;
	bool							m_bEnablePlaceJetton;

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
	virtual bool RealizeWIN7() { return true; }

	//���ú���
public:
	//ȡ�����
	void OnBankDraw();
	//������
	void OnBankStorage();
	//������Ϣ
	void SetMeMaxScore(SCORE lMeMaxScore);
	//�ҵ�λ��
	void SetMeChairID(DWORD dwMeUserID);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(DWORD dwBankerUserID, SCORE lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, SCORE lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore, SCORE lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, SCORE lScoreCount);
	//��������ע
	void AndroidBet(BYTE cbViewIndex, SCORE lScoreCount);
	//��������
	void SetAreaLimitScore(SCORE lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbTableCardArray[1][1]);
	//���ó���
	void SetCurrentJetton(SCORE lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(BYTE *bcResulte);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//������ʾ
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);
	//ִ��ʣ�����еĻ��嶯��
	void PerformAllBetAnimation();
	//���û�����������
	void SetRobotMaxJetton(SCORE lRobotMaxJetton){ m_lRobotMaxJetton = lRobotMaxJetton; };

	void SetFirstShowCard(BYTE bcCard);
	//ȡ������
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }

	void SetEnablePlaceJetton(bool bEnablePlaceJetton);

	void UpdataJettonButton();

	void StartRunCar(int iTimerStep);

	void RuningCar(int iTimerStep);
	void RuningRedCar();

	void FlashWinOpenSide();

	void StartRandShowSide();

	CString NumberStringWithSpace(LONGLONG lNumber);

	int ifirstTimer;

	bool m_bFlashrandShow;

	bool m_blRungingCar;

	bool flagrrc;

	int iTimerStep;
	int iTimerStep1;

	int mX;
	int mX1;
	int mY1;
	int mY2;
	int mX3;
	int mY3;
	int mX2;
	int mxx;
	int tx1;
	int ty1;
	int tx2;
	int ty2;
	int m_x1;
	int m_y1;
	int m_x2;
	int m_y2;
	int  mxx2;
	int sp0;
	int sp1;
	int sp2;
	int sp3;
	int m_CarIndexold;

	int iOpenSide;

	int iRunIndex;

	int iTotoalRun;

	
	
	void UpdateGameView( CRect *pRect );
	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard( bool bRecord = true );
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);

	//��������
public:
	//�����ע
	LONGLONG GetUserMaxJetton(BYTE cbJettonArea = 0);

	void  GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);

	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(bool blWin[],bool bSet);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//�滭ׯ��
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//�滭���
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);
	//�滭��ʷ�ɼ�����
	void DrawMemoryDate(CDC *pDC,int nWidth, int nHeight);

	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	

	void KillCardTime();

	void SwitchToCheck();

	void SwithToNormalView();

	void SetMoveCardTimer();

	void StartMove();

	void StartJetton_AniMationN();

	void StartOPenBox();

	void StartHandle_Leave();

	void StarShowResult();

	void OnCancelMyApply();

	void MyDrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea);

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool blTimer= false,bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, SCORE lNumber, CRect rcRect, INT nFormat=-1);
	CString DrawNumberStringWithDot(DOUBLE lScore);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//�ƶ�Ӯ��
	void DeduceWinner(bool bWinMen[]);

	void DrawMoveInfo(CDC* pDC, CRect rcRect);

	void SetJettonHide(int ID);


	//��ť��Ϣ
protected:
	bool  m_bApplyBanker;
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
	afx_msg void OnUp();
	afx_msg void OnLastMusic();
	afx_msg void OnNextMusic();

	afx_msg void OnDown();


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
