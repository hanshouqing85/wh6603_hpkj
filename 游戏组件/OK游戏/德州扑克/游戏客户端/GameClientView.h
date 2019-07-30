#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "GoldControl.h"
#include "GoldView.h"
#include "CardControl.h"
#include "SmallCardControl.h"
#include "Prompt.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_START					WM_USER+100							//��ʼ��Ϣ
#define IDM_EXIT					WM_USER+101							//�뿪��Ϣ
#define IDM_GIVEUP					WM_USER+102							//������Ϣ
#define IDM_ADD_SCORE				WM_USER+103							//��ע��Ϣ	
#define IDM_PASS_CARD				WM_USER+104							//������Ϣ	
#define IDM_FOLLOW					WM_USER+105							//��ע��Ϣ
#define IDM_SHOWHAND				WM_USER+106							//�����Ϣ
#define IDM_START_TIMES				WM_USER+107							//ʱ����Ϣ
#define IDM_AUTO_START				WM_USER+108							//��ʼ��Ϣ
#define IDM_SIT_DOWN				WM_USER+109							//������Ϣ
#define IDM_GAME_OVER				WM_USER+111							//������Ϣ
#define IDM_SEND_FINISH				WM_USER+112							//���ƽ���
#define IDM_OPEN_CARD				WM_USER+113							//������Ϣ

//ʱ�䶨ʱ��
#define IDI_GAME_OVER					30								//������ʱ��
#define IDI_NO_SCORE					31								//������ʱ��
#define IDI_CLOSE_GAME                  32                              //�˳�����ʱ 

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//����״̬
struct tagJettonStatus
{
	//������Ϣ
	WORD							wMoveCount;							//�ƶ�����
	WORD							wMoveIndex;							//�ƶ�����

	//������Ϣ
	CPoint							ptFrom;								//����λ��
	CPoint							ptDest;								//Ŀ��λ��
	CPoint							ptCourse;							//����λ��
	LONGLONG						lGold;								//������Ŀ

	//�ƶ���ʽ
	INT								iMoveType;							//�ƶ���ʽ
};

//�˿�״̬
struct tagCardStatus
{
	//������Ϣ
	WORD							wMoveCount;							//�ƶ�����
	WORD							wMoveIndex;							//�ƶ�����

	//������Ϣ
	CPoint							ptFrom;								//����λ��
	CPoint							ptDest;								//Ŀ��λ��
	CPoint							ptCourse;							//����λ��
	BYTE							bCard;								//�˿�����

	//�ƶ���ʽ
	INT								iMoveType;							//�ƶ���ʽ
};
//////////////////////////////////////////////////////////////////////////

//��������
#define TO_USERCARD						1								//�û��˿�
#define TO_GIVEUP_CARD					2								//�����˿�
#define TO_CENTER_CARD					3								//�����˿�

typedef CWHArray<tagCardStatus,tagCardStatus&> CCardStatusArray;
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//״̬����
protected:
	bool							m_bReset;							//��λ��־
	WORD							m_wMeChairID;						//����λ��
	WORD							m_wDUser;							//D��ʾ
	WORD							m_wSmallBlindUser;					//Сäע��ʶ
	WORD							m_wBigBlindUser;					//��äע��ʶ

	WORD							m_wMyIsLookOn;						//�Թ��û�
	WORD							m_wUserGender[GAME_PLAYER];			//�û��ձ�
	WORD							m_wUserPost[GAME_PLAYER];			//�û�λ��

	//���ݱ���
protected:
	BYTE							m_bShowUser;						//��ʾ��־
	LONGLONG						m_lTableScore[GAME_PLAYER];			//������ע
	LONGLONG						m_lTotalScore[GAME_PLAYER];			//����ע
	LONGLONG						m_lEachScore[GAME_PLAYER];			//ÿ����ע
	LONGLONG						m_lCenterScore;						//���ĳ���
	LONGLONG						m_lTurnLessScore;					//��С��ע
	LONGLONG						m_lTurnMaxScore;					//�����ע

	//�������
protected:
	CBitImageEx						m_ImageViewCenter;					//����λͼ
	CBitImageEx						m_ImageViewFill;
	CBitImageEx						m_ImageHuman;						//����ͼ��
	CBitImageEx						m_ImageTitleInfo;					//��ע��Ϣ
	CBitImageEx						m_ImageUserFrame;					//�û����

	CPngImageEx						m_ImageCardMask;					//�˿���ͼ
	CPngImageEx						m_ImageD;							//Dλͼ
	CPngImageEx						m_ImageBigBlind;					//��äעλͼ
	CPngImageEx						m_ImageSmallBlind;					//Сäעλͼ
	CPngImageEx						m_ImageArrowhead;					//ʱ���ͷ
	CPngImageEx						m_ImageCard;						//�˿�λͼ
	CPngImageEx						m_ImageSmallCard;					//С��λͼ
	CPngImageEx						m_ImagePass;                        //passλͼ
	CPngImageEx						m_ImageAdd;							//addλͼ
	CPngImageEx						m_ImageFollow;                      //followλͼ
	CPngImageEx						m_ImageAllIn;                       //allinλͼ

	//��������
public:
	//�ƶ�����
	enum ANTE_ANIME_ENUM
	{
		AA_BASEFROM_TO_BASEDEST=0,										// ��ע������ע
		AA_BASEDEST_TO_CENTER,											// ��ע���������м�
		AA_CENTER_TO_BASEFROM,											// �мӳ���������ע
	};
	CGoldView						m_MoveGoldView[GAME_PLAYER];		//����ؼ�
	bool							m_bJettonAction;					//������־
	bool							m_bCardAction;						//������־
	tagJettonStatus					m_JettonStatus[GAME_PLAYER];		//������Ϣ
	CCardStatusArray				m_CardStatus[GAME_PLAYER+1];		//�˿���Ϣ
	BYTE							m_bCenterCount;						//���Ĵ���

	//λ�ñ���
protected:
	CPoint							m_ptHuman[GAME_PLAYER];				//����λ��
	CPoint							m_ptWomen[GAME_PLAYER];				//����λ��
	CPoint							m_ptSmallCard[GAME_PLAYER];			//С��λ��
	CPoint							m_ptCard[GAME_PLAYER];				//�˿�λ��
	CPoint							m_ptJetton[GAME_PLAYER];			//����λ��
	CPoint							m_ptCenterCard;						//�����˿�λ��
	CPoint							m_ptCenterJetton;					//���ĳ���λ��
	CPoint							m_ptD[GAME_PLAYER];					//Dλ��
	CPoint							m_ptSmallBlind[GAME_PLAYER];		//Сäע��λ��
	CPoint							m_ptBigBlind[GAME_PLAYER];			//��äע��λ��
	CPoint							m_SendEndingPos[GAME_PLAYER];		//���ƽ���λ��
	CPoint							m_ptSitDown[GAME_PLAYER];			//���°�ťλ��
	CPoint                          m_ptViewCenter;                     //��ͼ���� 

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btExit;							//�뿪��ť
	CSkinButton						m_btPassCard;						//������Ŧ
	CSkinButton						m_btShowHand;						//�����ť
	CSkinButton						m_btGiveUp;							//������ť	
	CSkinButton						m_btAdd;							//��ע��ť	
	CSkinButton						m_btFollow;							//��ע��ť	
	CSkinButton						m_btAutoStart;						//�Զ���ʼ
	CSkinButton						m_btOpenCard;						//���ư�ť
	CSkinButton						m_btSitDown[GAME_PLAYER];			//���°�ť

	//��������
protected:
	bool							m_bGameEnd;							//��Ϸ����
	CPoint							m_SendCardPos;						//����λ��

	//�ؼ�����
public:
	CPrompt							m_Prompt;							//��ʾ����
	CScoreView						m_ScoreView;						//�ɼ�����
	CGoldControl					m_GoldControl;						//��ע��ͼ
	CGoldView						m_GoldView[GAME_PLAYER];			//����ؼ�
	CGoldView						m_CenterGoldView;					//���ĳ���
	CCardControl					m_CardControl[GAME_PLAYER];			//�˿���ͼ
	CCardControl					m_CenterCardControl;				//������ͼ
	CSmallCardControl				m_SmallCardControl[GAME_PLAYER];	//С�˿���ͼ

	//�������
public:
	CFont                           m_ViewFont;                         //��ͼ���� 
	CDFontEx                        m_ViewFontEx;                       //��ͼ����

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
	//�Ƿ��������
	virtual bool RealizeWIN7() { return true;; }

	//���ܺ���
public:
	//��ʾ�û�
	void SetShowUserView(BYTE bShow);
	//����λ��
	void SetMePost(WORD wMeChairID){m_wMeChairID = wMeChairID;}
	//�Թ�״̬
	void SetMyLookOn(WORD wIsLookOnId);
	//�Թ�״̬
	WORD GetMyLookOnId(){return m_wMyIsLookOn;}
	//���ñ�ʾ
	void SetDFlag(WORD wDUser);
	//���ô�äע��ʶ
	void SetBigBlindFlag(WORD wBigBlindUser);
	//����Сäע��ʶ
	void SetSmallBlindFlag(WORD wSmallBlindUser);
	//������ע
	void SetUserTableScore(WORD wChairID, LONGLONG lTableScore);
	//���ĳ���
	void SetCenterScore(LONGLONG lCenterScore);
	//�����ܳ���
	void SetTotalScore(WORD wChairID, LONGLONG lTotalScore);
	//�����û����μ�ע��������
	void SetUserEachScore(WORD wChairID,LONGLONG LEachSore);
	//������Ϣ
	void SetTitleInfo(LONGLONG lTurnLessScore, LONGLONG lTurnMaxScore);
	//���ý���
	void SetGameEndStart();
	//���ý���
	void SetGameEndEnd();
	//���ý���
	void SetCenterCount(){m_bCenterCount=0;}
	//�ƶ�����
	void DrawMoveAnte( WORD wViewChairID, int iMoveType, LONGLONG lTableScore );
	//�ƶ��˿�
	void DrawMoveCard( WORD wViewChairID, int iMoveType, BYTE bCard );
	//�ƶ�����
	bool MoveJetton();
	//�ƶ�����
	bool MoveCard();
	//���Ա��ַ�
	LPCTSTR GetGlodString(LONGLONG lGold, TCHAR szString[]);
	//����λ��
	void SetUserPost(WORD wPhysicsPost,WORD wViewPost);
	//������־
	bool IsMoveing(){return (m_bJettonAction || m_bCardAction);}
	//����ʱ��
	void SetOverTimer(){SetTimer(IDI_GAME_OVER,50,NULL);}
	//��λ��־
	void SetResetTag(bool bReset=false){m_bReset = bReset;}
	//��λ��־
	void SetUserGender(WORD wViewChairID,WORD wGender){m_wUserGender[wViewChairID] = wGender;}
	//������ʾ��
	void SetNoScoreDlg(){SetTimer(IDI_NO_SCORE,50,NULL);}
	//������ͼ
	void RefreshGameView();

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
