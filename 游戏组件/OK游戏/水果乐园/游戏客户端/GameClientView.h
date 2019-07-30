#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "RecordGameList.h"
#include "ExchangeDlg.h"
#include "PlayerList.h"
#include "AdminClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				9									//������Ŀ

//��Ϣ����
#define IDM_APPLY_BANKER			(WM_USER+201)						//������Ϣ
#define IDM_BIG_SMALL				(WM_USER+202)						//�ȴ�С
#define IDM_XU_YA					(WM_USER+203)						//��Ѻ

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//������
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

class CMySkinButton : public CSkinButton
{
public:
	int m_nTime;//��갴��ʱ��������ʱ��
	CMySkinButton();
	~CMySkinButton();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	__int64							m_lMeMaxChip;							//�����ע
	__int64							m_lMeAreaChip[JETTON_AREA_COUNT];		//�����ע					
	__int64							m_lAllAreaChip[JETTON_AREA_COUNT];		//ȫ����ע
	__int64							m_lAreaChipLimit[JETTON_AREA_COUNT];	//������ǰ���·�
	CRect							m_rcJettonArea[JETTON_AREA_COUNT];		//����ע����
	CRect							m_rcChipLimit[JETTON_AREA_COUNT];		//�����������·���д��
	CRect							m_rcApplayBankerNo[4];					//������ׯ����б�
	CRect							m_rcApplayBankerName[4];				//������ׯ�������
	CRect							m_rcApplayBankerChip[4];				//������ׯ��ҽ��
	CRect							m_rcHistoryRecord[MAX_SHOW_HISTORY];
	int								m_nCurCartoonIndex;						//��ǰӦ�û����ĸ���ͨͼ��
	int								m_CartoonTimeElapseArray[130];//96		//���ƿ�ͨ��ת��ʱ����
	int								m_CartoonElapsIndex;					//��ͨ��ʱ�������±�
	BYTE							m_cbWinArea;							//ÿ����תֹͣ��λ��
	BYTE							m_cbMyBigSmallResult[JETTON_AREA_COUNT];

	//λ����Ϣ
public:
	int								m_nChipHead;							//�ɼ�λ��
	bool							m_bisLButtonDown;						//�Ƿ�����������
	BYTE							m_cbEndCount;							//��Ϸ��������

	//��������
protected:
	bool							m_bBombEffect[JETTON_AREA_COUNT];		//��ըЧ��
	BYTE							m_cbBombFrameIndex[JETTON_AREA_COUNT];	//֡������

	//��ʷ��Ϣ
protected:
	__int64							m_lMeStatisticChip;					//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbWinnerSide;						//ʤ�����
	BYTE							m_cbAreaFlash;						//ʤ�����
	__int64							m_lCurrentJetton;					//��ǰ����
	CString							m_strDispatchCardTips;				//������ʾ
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	__int64							m_lBankerChip;						//ׯ�һ���
	__int64							m_lBankerWinChip;					//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ
	CFont							m_fontChipLimit;					//ר�Ż��ƿ��·ֵĻ���

	//���ֳɼ�
public:
	__int64							m_lMeCurGameChip;					//�ҵĳɼ�
	__int64							m_lMeCurGameReturnChip;			//�ҵĳɼ�(������ע��)
	__int64							m_lBankerCurGameChip;				//ׯ�ҳɼ�
	__int64							m_lGameRevenue;						//��Ϸ˰��
	BYTE							m_cbGameEndTime;					//��Ϸ�����׶���ʾ��ʱ�䣬��Dlg������

	bool							m_bBigSmallTime;					//�ȴ�Сʱ��
	bool							m_bAlreadyGuess;					//�Ѿ��¹���С��
	BYTE							m_cbPaoHuoCheCount;
	BYTE							m_cbPaoHuoCheIndex;
	BYTE							m_cbPaoHuoCheArea[7];
	BYTE							m_cbAreaLight[24];					//��������߿�
	bool							m_bBigSmallWin;
	BYTE							m_cbGoodLuckType;
	bool							m_bShowSGZZLogo;
	bool							m_bShowPlayerList;					//��ʾ����б�������ʾ��ׯ�б�

	//���ݱ���
protected:
	CPoint							m_PointJetton[JETTON_AREA_COUNT];					//����λ��
	CPoint							m_PointJettonNumber[JETTON_AREA_COUNT];				//����λ��
	CPoint							m_PtCardArray[24];					//��ת·���ϸ���������ĵ�
	bool							m_bGoodLuckTypeTip;					//����GoodLuck
	bool							m_bGoodLuckFlash;
	BYTE							m_cbFlashAndHitCount;
	BYTE							m_cbGoodLuckFlashCount;
	BYTE							m_cbGoodLuckTipCount;

	BYTE							m_cbPaoHuoCheWinIndex;			//���С��ܻ𳵡�ʱ����ʼλ��
	BYTE							m_cbPaoHuoCheWinCount;			//���С��ܻ𳵡�ʱ�Ĺ�������

	BYTE							m_cbActiveBorderFrameCount;
	CRect							m_rectActiveBorderUpdate;

	//�ؼ�����
public:
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��

	CSkinButton						m_btChipMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btChipMoveR;						//�ƶ��ɼ�
	CSkinButton						m_btBankButton;						//���а�ť

	CSkinButton						m_btAddStep1;						//1��Ͷע
	CSkinButton						m_btAddStep10;						//10��Ͷע
	CSkinButton						m_btAddStep100;						//100��Ͷע
	CSkinButton						m_btAddStep1_H;						//1��Ͷע
	CSkinButton						m_btAddStep10_H;					//10��Ͷע
	CSkinButton						m_btAddStep100_H;					//100��Ͷע

	CMySkinButton					m_btBetLast;						//��Ѻ��ť
	CSkinButton						m_btBetSmall;						//ѺС��ť
	CSkinButton						m_btBetBig;							//Ѻ��ť

	CSkinButton						m_btPlayerList;
	CSkinButton						m_btApplyList;
	CSkinButton						m_btPlayerList_H;
	CSkinButton						m_btApplyList_H;
	CSkinButton						m_btBetLast2;

	//�ؼ�����
public:
	CGameRecord						m_GameRecord;						//��¼�б�	
	CExchangeDlg					m_ExchangeDlg;
	CPlayerList						m_PlayerList;
	CPlayerList						m_ApplyList;
	CButton							m_btOpenAdmin;						//�����˺ſ���
	CAdminClientDlg					m_AdminDlg;							//�����˺ſ���
	CGameClientDlg					*m_pGameClientDlg;					//

	//�������
protected:
	CBitImage						m_ImageViewFill;					//����λͼ
	CBitImage						m_ImageViewBack;					//����λͼ
	CBitImage						m_ImageJettonView;					//������ͼ
	CBitImage						m_ImageChipNumber;					//������ͼ
	CBitImage						m_ImageHistoryRecord;				//��ʷ��¼ͼƬ
	CBitImage						m_ImageGoodLuckFlash[2];
	CBitImage						m_ImageActiveButton1;
	CBitImage						m_ImageActiveButton10;
	CBitImage						m_ImageActiveButton100;
	CBitImage						m_ImageBigSmallNumber;

	CPngImage						m_pngActiveBorder1;
	CPngImage						m_pngActiveBorder10;
	CPngImage						m_pngActiveBorder100;
	CPngImage						m_ImageCardCartoon;					//��ת����
	CPngImage						m_ImageCardCartoon2;
	CPngImage						m_pngRecordTip;

	CPngImage						m_ImageApple;						//ƻ������
	CPngImage						m_ImageApple2;						//ƻ��X2����
	CPngImage						m_ImageBar;							//BAR����
	CPngImage						m_ImageBar2;						//BARX2����
	CPngImage						m_ImageBell;						//��������
	CPngImage						m_ImageBell2;						//����X2����
	CPngImage						m_ImageSeven;						//SEVEN
	CPngImage						m_ImageSeven2;						//SEVENX2
	CPngImage						m_ImageStar;						//STAR
	CPngImage						m_ImageStar2;						//STARX2
	CPngImage						m_ImageOrange;						//ORANGE
	CPngImage						m_ImageOrange2;						//ORANGEX2
	CPngImage						m_ImagePaw;							//PAW
	CPngImage						m_ImagePaw2;						//PAWX2
	CPngImage						m_ImageWaterMelon;					//WATERMELON
	CPngImage						m_ImageWaterMelon2;					//WATERMELONX2
	CPngImage						m_pngGoodLuckTip[5];				//����GoodLuckʱ����ʾ��ͼƬ
	CPngImage						m_pngSongJiang;

	CPngImage						m_ImageBetNumber;					//������ͼ
	CPngImage						m_ImageBetBigNumber;				//ʱ����ͼ

	CPngImage						m_ImageFreeTime;					//����ʱ��
	CPngImage						m_ImageBitSmallTime;				//Ѻ��Сʱ��
	CPngImage						m_ImageJettonTime;					//��עʱ��

	CPngImage						m_pngJettonBack;
	CPngImage						m_pngJettonButton;
	CPngImage						m_pngMove;

	CPngImage						m_pngLine;
	CPngImage						m_pngBigSmall[2];
	CPngImage						m_pngSGZZLogo;

	HCURSOR							m_MyCursor[3];

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//���ú���
public:
	//������Ϣ
	void SetMeMaxChip(__int64 iMeMaxChip);
	//�ҵ�λ��
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID;}
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, __int64 lBankerChip);
	//ׯ�ҳɼ�
	void SetBankerChip(WORD wBankerTime, __int64 lWinChip) {m_wBankerTime=wBankerTime; m_lBankerWinChip=lWinChip;}
	//���ֳɼ�
	void SetCurGameChip(__int64 lMeCurGameChip, __int64 lMeCurGameReturnChip, __int64 lBankerCurGameChip, __int64 lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, __int64 lChipCount);
	//��������
	void SetAreaLimitChip(__int64 * pLimitArray, int ArraySize);	
	//���ó���
	//void SetCurrentJetton(__int64 iCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(tagGameRecord * pGameRecord, int nRecordCount);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameChip(__int64 lMeCurGameChip, __int64 lMeCurGameReturnChip, __int64 lBankerCurGameChip);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	void CreateApplayBankerListRect(int nBaseX, int nBaseY);
	void CreateHistoryRecordRect(int nWidth, int nHeight);
	void UpdateCartoonTimeElapse(BYTE cbStopStep);
	void UpdateCartoonTimeElapse2(BYTE cbWinArea);
	void UpdateCartoonTimeElapse3(BYTE cbWinArea);
	void CreateTurnPathAreaBasePoint(int nWidth, int nHeight);
	//����GoodLuckʱ���ŵĶ���
	void SetGoodLuckCartoon(BYTE cbPaoHuoCheArea[], BYTE cbPaoHuoCheCount, BYTE cbWinArea, BYTE cbGoodLuckType);

	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nFlags);

	//��������
public:	
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//�滭��ը
	void DrawBombEffect(CDC *pDC);
	//����������ׯ����б�
	void DrawApplyBankerUserList(CDC * pDC);
	//������ʷ��¼
	void DrawHistoryRecord(CDC * pDC);
	//����ʱ����ʾ
	void DrawTimeTip(CDC * pDC, int nWidth, int nHeight);
	//����������ע
	void DrawAreaLimitChip(CDC * pDC);
	//���Ƴ���
	//void DrawAreaJetton(CDC * pDC);
	//����ׯ����Ϣ
	void DrawBankerInfo(CDC * pDC, int nWidth, int nHeight);
	//�����м���Ϣ
	void DrawUserInfo(CDC * pDC, int nWidth, int nHeight);
	//����GoodLuck��˸
	void DrawGoodLuckFlash(CDC * pDC, int nWidth, int nHeight);
	//�л�ׯ��
	void DrawChangeBanker(CDC * pDC, int nWidth, int nHeight);
	//������ע��ť
	void DrawAddMoney(CDC * pDC, int nWidth, int nHeight);
	//����ʱ��
	virtual void DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea);

	//��������
protected:

	//��ȡ��ǰ��һ�����ע������
	__int64 GetUserCurrentLeaveChip();
	
	//�滭����
public:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);
	//��ʼ��ת
	void PlayingCartoonEffect();
	void PlayingCartoonEffect2();
	void PlayingCartoonEffect3();
	//������ת
	void EndCartoonEffect();


	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(BYTE cbWinnerSide);
	//�滭����
	void DrawMyJettonNumber(CDC *pDC);
	//��С��ʷ��¼
	void DrawMyBigSmallNumber(CDC *pDC);
	//������С��ʷ��¼
	void InsertBigSmallRecord(BYTE cbBigSmall);

	//��������
public:
	//��ǰ����
	inline __int64 GetCurrentJetton() { return m_lCurrentJetton; }	

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos, bool bYellow=false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, CRect rcRect, INT nFormat=-1);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//�����ҵķ���
	void DrawMyChip(CDC *pDC, int nWidth, int nHeight);
	//���Ƶ�ǰ�ɼ�
	void DrawCurrentChip(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//���ƶ���
	void DrawCardCartoon(CDC * pDC);
	//��������߿�
	void DrawAreaLight(CDC * pDC);
	//���ƽ�������
	void DrawEndCartoon(CDC * pDC,int nWidth,int nHeight,BYTE cbWinArea);
	//���ƽ�������
	void DrawGoodLuckTip(CDC * pDC,int nWidth,int nHeight,BYTE cbWinArea);

	//��ť��Ϣ
protected:
	//��Ͷ��ť
	afx_msg void OnAddStep1();
	afx_msg void OnAddStep10();
	afx_msg void OnAddStep100();
	afx_msg void OnAddStep1_H();
	afx_msg void OnAddStep10_H();
	afx_msg void OnAddStep100_H();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnChipMoveL();
	//�ƶ���ť
	afx_msg void OnChipMoveR();

	afx_msg void OnClickedBank();

	afx_msg void OnBetSmall();
	afx_msg void OnBetBig();
	afx_msg void OnXuYa();
	afx_msg void OnXuYa2();
	afx_msg void OnPlayerList();
	afx_msg void OnApplyList();
	//�����˺ſ���
	afx_msg void OpenAdminWnd();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
