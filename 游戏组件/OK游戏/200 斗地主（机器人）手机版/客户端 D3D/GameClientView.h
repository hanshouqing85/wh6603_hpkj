#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "CardControl.h"
#include "ScoreControl.h"
#include "HistoryRecord.h"
#include "DoubleControl.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////////////

//��ͼλ��
#define MYSELF_VIEW_ID				1									//��ͼλ��

//������Ϣ
//#define IDM_START					(WM_USER+200)						//��ʼ��Ϣ
//#define IDM_OUT_CARD				(WM_USER+201)						//������Ϣ
//#define IDM_PASS_CARD				(WM_USER+202)						//PASS��Ϣ
//#define IDM_OUT_PROMPT				(WM_USER+203)						//��ʾ��Ϣ
//#define IDM_CALL_SCORE				(WM_USER+204)						//�з���Ϣ
//#define	IDM_LEFT_HIT_CARD			(WM_USER+205)						//����˿�
//#define IDM_SORT_HAND_CARD			(WM_USER+206)						//�����˿�
//#define IDM_LAST_TURN_CARD			(WM_USER+207)						//�����˿�
//#define IDM_TRUSTEE_CONTROL			(WM_USER+208)						//�йܿ���
//#define IDM_CALL_BANKER				(WM_USER+209)						//�е�����Ϣ
//#define IDM_ROD_BANKER				(WM_USER+210)						//��������Ϣ
#define IDM_START					(WM_USER+200)						//��ʼ��Ϣ
#define IDM_VALID_START				(WM_USER+201)						//���ƿ�ʼ
#define IDM_OUT_CARD				(WM_USER+202)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+203)						//PASS��Ϣ
#define IDM_OUT_PROMPT				(WM_USER+204)						//��ʾ��Ϣ
//#define IDM_VALID_CARD				(WM_USER+205)						//������Ϣ
#define IDM_CALL_BANKER				(WM_USER+206)						//�е�����Ϣ
#define IDM_ROD_BANKER				(WM_USER+207)						//��������Ϣ
#define IDM_DOUBLE					(WM_USER+208)						//�ӱ���Ϣ
#define	IDM_LEFT_HIT_CARD			(WM_USER+209)						//����˿�
#define IDM_SORT_HAND_CARD			(WM_USER+210)						//�����˿�
#define IDM_LAST_TURN_CARD			(WM_USER+211)						//�����˿�
#define IDM_TRUSTEE_CONTROL			(WM_USER+212)						//�йܿ���


//������Ϣ
#define IDM_DISPATCH_FINISH			(WM_USER+300)						//�������
#define IDM_REVERSAL_FINISH			(WM_USER+301)						//�������

#define IDM_SEARCH_CARD				(WM_USER+320)						//��������

#define SEARCH_MISSILE				0									//�������
#define SEARCH_BOMB					1									//����ը��
#define SEARCH_THREE_TOW_LINE		2									//�����ɻ�
#define SEARCH_DOUBLE_LINE			3									//����˫˳
#define SEARCH_SINGLE_LINE			4									//������˳
#define SEARCH_THREE_TWO_ONE		5									//��������N

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewD3D
{
	friend class CGameClientEngine;
	//״̬����
public:
	bool							m_bShowScore;						//����״̬
	bool							m_bLastTurnState;					//���ֱ�־
	bool							m_bWaitCallScore;					//�ȴ��з�
	bool							m_bUserPass[GAME_PLAYER];			//������־
	bool							m_bUserCheat[GAME_PLAYER];			//���ױ�־
	tagHistoryScore	*				m_pHistoryScore[GAME_PLAYER];		//������Ϣ

	//��Ϸ����
protected:
	//LONG							m_lCellScore;						//��Ԫ����
	//BYTE							m_cbBombCount;						//ը������
	//BYTE							m_cbCallScore[GAME_PLAYER];			//�з���Ϣ

	LONG							m_lCellScore;						//��Ԫ����
	WORD							m_wCurrentTime;						//��ǰ����
	WORD							m_wBackCardTime;					//���Ʊ���
	BYTE							m_cbBombCount;						//ը������
	BYTE							m_cbBackCardType;					//��������	
	BYTE							m_cbCallBankerInfo[GAME_PLAYER];	//�е�����Ϣ
	BYTE							m_cbRodBankerInfo[GAME_PLAYER];		//��������Ϣ
	//BYTE							m_cbAddDoubleInfo[GAME_PLAYER];		//�ӱ���Ϣ	
	CRect							m_rcTime;							//����λ��

	//ׯ�ұ���
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	BYTE							m_cbBankerScore;					//ׯ�ҽз�

	//�ƶ�����
protected:
	bool							m_bMoveMouse;						//�ƶ���־
	bool							m_bSwitchCard;						//������־
	bool							m_bSelectCard;						//ѡ���־
	BYTE							m_cbHoverCardItem;					//��������
	BYTE							m_cbMouseDownItem;					//�������
	BYTE							m_cbSwitchCardItem;					//��������

	//λ�ñ���
protected:
	CPoint							m_ptPass[GAME_PLAYER];				//����λ��
	CPoint							m_ptBanker[GAME_PLAYER];			//ׯ��λ��
	CPoint							m_ptCountWarn[GAME_PLAYER];			//����λ��
	CPoint							m_ptCallScore[GAME_PLAYER];			//�з�λ��
	CPoint							m_ptOrderFlag[GAME_PLAYER];			//�ȼ�λ��

	//��ը����
protected:
	bool							m_bCartoonBomb;						//������־
	WORD							m_wCartoonBombIndex;				//֡������
	CLapseCount						m_CartoonBombLapseCount;			//���ż���

	//�ɻ�����
protected:
	bool							m_bCartoonPlan;						//������־
	WORD							m_wCartoonPlanIndex;				//֡������
	CPoint							m_ptCartoonPlan;					//�ɻ�λ��
	CLapseCount						m_CartoonPlanLapseCount;			//���ż���

	//�������
protected:
	bool							m_bCartoonRocket;					//������־
	WORD							m_wCartoonRocketIndex;				//֡������
	CPoint							m_ptCartoonRocket;					//���λ��
	CLapseCount						m_CartoonRocketLapseCount;			//���ż���

	//���ƶ���
protected:
	bool							m_bReversalCard;					//������־
	WORD							m_wReversalCardIndex;				//֡������
	CLapseCount						m_ReversalCardLapseCount;			//���ż���

	//ׯ�Ҷ���
protected:
	WORD							m_wBankerMovieIndex;				//֡������
	CLapseCount						m_BankerMovieLapseCount;			//���ż���

	//���ƶ���
protected:
	bool							m_bRiffleCard;						//������־
	bool							m_bDispatchCard;					//������־
	WORD							m_wDispatchCardIndex;				//֡������
	CLapseCount						m_DispatchCardLapseCount;			//���ż���

	//��������
protected:
	WORD							m_wStartUser;						//��ʼ�û�
	BYTE							m_cbHandCardData[GAME_PLAYER][NORMAL_COUNT];	//��������

	//��������
protected:
	bool							m_bCountWarn[GAME_PLAYER];			//������־
	WORD							m_wCountWarnIndex[GAME_PLAYER];		//֡������
	CLapseCount						m_CountWarnLapseCount[GAME_PLAYER];	//���ż���

	//��Ϸ��ť
public:
	CSkinButton					m_btScore;							//���ְ�ť
	CSkinButton					m_btStart;							//��ʼ��ť

	CSkinButton					m_btOutCard;						//���ư�ť
	CSkinButton					m_btPassCard;						//PASS��ť
	CSkinButton					m_btOutPrompt;						//��ʾ��ť

	//���ܰ�ť
public:
	CSkinButton					m_btCancelTrustee;					//ȡ���йܰ�ť
	CSkinButton					m_btTrustee;						//�йܰ�ť
	CSkinButton					m_btLastTurn;						//���ְ�ť
	CSkinButton					m_btSortCard;						//����ť

	//�зְ�ť
public:
	//CSkinButton					m_btCallScore1;						//�зְ�ť
	//CSkinButton					m_btCallScore2;						//�зְ�ť
	//CSkinButton					m_btCallScore3;						//�зְ�ť
	//CSkinButton					m_btCallScoreNone;					//�зְ�ť
	//������ť
public:
	CSkinButton					m_btCallBanker;						//�е�����ť
	CSkinButton					m_btNoCallBanker;					//���а�ť
	CSkinButton					m_btRodBanker;						//��������ť
	CSkinButton					m_btNoRodBanker;					//������ť



	//��ʾ��ť
public:
	CSkinButton					m_btCardPrompt1;					//��ʾ��ť
	CSkinButton					m_btCardPrompt2;					//��ʾ��ť
	CSkinButton					m_btCardPrompt3;					//��ʾ��ť
	CSkinButton					m_btCardPrompt4;					//��ʾ��ť
	CSkinButton					m_btCardPrompt5;					//��ʾ��ť
	CSkinButton					m_btCardPrompt6;					//��ʾ��ť

	//������Դ
protected:
	CD3DTexture						m_TextureFill;						//������Դ
	CD3DTexture						m_TextureBack;						//������Դ
	CD3DTexture						m_TextureUserPlayer;				//�û����
	CD3DTexture						m_TextureUserMySelf;				//�û����

	CD3DTexture						m_TextureGameOver;					//������Դ

	//��Դ����
protected:
	CD3DSprite						m_TextureBankerFlag;				//ׯ�ұ�־
	CD3DTexture						m_TextureCellNumber;				//��Ϣ����
	CD3DTexture						m_TextureCellScoreBack;				//��Դ����
	CD3DTexture						m_TextureBankerCardBack;			//��Դ����
	CD3DTexture						m_TextureRemnantCardNumber;			//��Դ����

	CD3DTexture						m_TextureFenTou;					//��Դ����
	CD3DTexture						m_TextureFenWei;					//��Դ����

	//��̬��Դ
protected:
	CD3DTexture						m_TextureUserPass;					//������־
	CD3DTexture						m_TextureLastTurn;					//���ֱ�־
	CD3DTexture						m_TextureCallScore;					//�û��з�
	CD3DTexture						m_TextureCountWarn;					//�˿˾���
	CD3DTexture						m_TextureCartoonBomb;				//״̬��Ϣ
	CD3DTexture						m_TextureCartoonPlan;				//�ɻ�����
	CD3DTexture						m_TextureMemberOrder;				//�ɻ�����
	CD3DTexture						m_TextureCartoonRocket;				//�������
	CD3DTexture						m_TextureDispatchCard;				//��Դ����
	CD3DTexture						m_TextureHistoryScore;				//��Դ����
	CD3DTexture						m_TextureReversalCard;				//��Դ����
	CD3DTexture						m_TextureWaitCallScore;				//�ȴ��з�
	CD3DTexture						m_TextureCallBanker;				//�û��е���
	CD3DTexture						m_TextureRodBanker;					//�û�������


	//�ؼ�����
public:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CScoreControl					m_ScoreControl;						//���ֿؼ�
	CDoubleControl					m_DoubleControl;					//�����ؼ�
	CScoreView						m_ScoreView;	
	//�˿˿ؼ�
public:
	CCardControl					m_BackCardControl;					//�����˿�
	CCardControl					m_HandCardControl[GAME_PLAYER];		//�û��˿�
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�˿���ͼ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);

	//���溯��
protected:
	//��������
	virtual VOID CartoonMovie();
	//�����豸
	virtual VOID InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);

	//ׯ������
public:
	//����ׯ��
	VOID SetBankerUser(WORD wBankerUser);
	//���ñ���
	VOID SetCurrentTime(WORD wCurrentTime);
	//��ʾ����
	VOID ShowBankerCard(BYTE cbCardData[3]);

	//���ܺ���
public:
	//��Ԫ����
	VOID SetCellScore(LONG lCellScore);
	//ը������
	VOID SetBombCount(BYTE cbBombCount);
	//���ý���
	VOID ShowGameScoreInfo(tagScoreInfo & ScoreInfo);
	// �滭����
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);


	//���Ͷ���
public:
	//�ɻ�����
	VOID ShowCartoonPlan(bool bCartoon);
	//��ը����
	VOID ShowCartoonBomb(bool bCartoon);
	//�������
	VOID ShowCartoonRocket(bool bCartoon);

	//���ƺ���
public:
	//�û�����
	VOID UserValidCard(WORD wChairID, BYTE cbCardData[MAX_COUNT], BYTE cbCardCount = 17);

	//״̬����
public:
	//��������
	VOID SetLastTurnState(bool bLastTurnState);
	//�ȴ��з�
	VOID SetWaitCallScore(bool bWaitCallScore);
	//���÷���
	VOID SetUserPassState(WORD wChairID, bool bUserPass);
	//���þ���
	VOID SetUserCountWarn(WORD wChairID, bool bCountWarn);
	//���ýз�
	//VOID SetUserCallScore(WORD wChairID, BYTE cbCallScore);
	//���ýе���
	VOID SetUserCallBanker(WORD wChairID, BYTE cbCallInfo);
	//����������
	VOID SetUserRodBanker(WORD wChairID, BYTE cbRodInfo);
	//���û���
	VOID SetHistoryScore(WORD wChairID, tagHistoryScore * pHistoryScore);

	//���ƺ���
public:
	//���ƶ���
	VOID StopDispatchCard();
	//���ƶ���
	VOID ShowDispatchCard(BYTE cbCardData[NORMAL_COUNT], WORD wStartUser);

	//��ť��Ϣ
public:
	//��ʼ��ť
	VOID OnBnClickedStart();
	//���ְ�ť
	VOID OnBnClickedScore();
	//���ư�ť
	VOID OnBnClickedOutCard();
	//����ť
	VOID OnBnClickedSortCard();
	//���ְ�ť
	VOID OnBnClickedLastTurn();
	//�йܰ�ť
	VOID OnBnClickedTrustee();
	//�йܰ�ť
	VOID OnBnClickedCancelTrustee();
	//PASS��ť
	VOID OnBnClickedPassCard();
	//��ʾ��ť
	VOID OnBnClickedOutPrompt();
	//һ�ְ�ť
	VOID OnBnClickedCallScore1();
	//���ְ�ť
	VOID OnBnClickedCallScore2();
	//���ְ�ť
	VOID OnBnClickedCallScore3();
	//�е�����ť
	VOID OnBnClickedCallBanker();
	//������ť
	//VOID OnBnClickedCallScoreNone();

	VOID OnBnClickedNoCallBanker();

	//��������ť
	VOID OnBnClickedRodBanker();
	//������ť
	VOID OnBnClickedNoRodBanker();

	//���غ���
public:
	//��Ϣ����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//��Ϣ����
public:
	//�����Ϣ
	VOID OnRButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnMouseMove(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif