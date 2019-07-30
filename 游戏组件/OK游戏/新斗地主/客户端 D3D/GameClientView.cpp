#include "StdAfx.h"
#include "GameLogic.h"
#include "GameClient.h"
#include "CardControl.h"
#include "GameClientView.h"
#include "GameClientEngine.h"
//////////////////////////////////////////////////////////////////////////////////

//��������
#define BOMB_CARTOON_COUNT			6									//��ը��Ŀ
#define PLAN_CARTOON_COUNT			3									//�ɻ���Ŀ
#define CARD_WARNING_COUNT			5									//������Ŀ
#define BANKER_MOVIE_COUNT			4									//������Ŀ
#define REVERSAL_CARD_COUNT			4									//������Ŀ

//ϴ�ƶ���
#define RIFFLE_CARD_COUNT_X			4									//ϴ����Ŀ
#define RIFFLE_CARD_COUNT_Y			6									//ϴ����Ŀ
#define RIFFLE_CARD_COUNT_ALL		RIFFLE_CARD_COUNT_X*RIFFLE_CARD_COUNT_Y

//���ƶ���
#define DISPATCH_CARD_TIMES			9									//���ƴ���
#define DISPATCH_CARD_SPACE			2									//���Ƽ��
#define DISPATCH_HEAP_DISTANCE		6									//�ƶѼ��

//////////////////////////////////////////////////////////////////////////////////

//��ʾ��ť
#define IDC_BT_START				100									//��ʼ��ť
#define IDC_BT_SCORE				101									//��ְ�ť

//�зְ�ť
#define IDC_CALL_SCORE_1			200									//�зְ�ť
#define IDC_CALL_SCORE_2			201									//�зְ�ť
#define IDC_CALL_SCORE_3			202									//�зְ�ť
#define IDC_CALL_SCORE_NONE			203									//�зְ�ť

//��Ϸ��ť
#define IDC_BT_OUT_CARD				300									//���ư�ť
#define IDC_BT_PASS_CARD			301									//PASS��ť
#define IDC_BT_OUT_PROMPT           302                             	//��ʾ��ť
#define IDC_BT_NOTE_CARD			303                             	//���ư�ť

//���ܰ�ť
#define IDC_BT_TRUSTEE				400									//�йܿ���
#define IDC_BT_CANCEL_TRUSTEE		401									//�йܿ���
#define IDC_BT_SORT_CARD			402									//�˿�����
#define IDC_BT_LAST_TURN			403									//�����˿�

//��ʾ��ť
#define IDC_CARD_PROMPT_1			500									//��ʾ��ť
#define IDC_CARD_PROMPT_2			501									//��ʾ��ť
#define IDC_CARD_PROMPT_3			502									//��ʾ��ť
#define IDC_CARD_PROMPT_4			503									//��ʾ��ť
#define IDC_CARD_PROMPT_5			504									//��ʾ��ť
#define IDC_CARD_PROMPT_6			505									//��ʾ��ť

//�ɼ�����
#define IDC_GAME_SCORE				600									//�ɼ�����
//���ƴ���
#define IDC_NOTE_CARD				6011									//���ƴ���
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewD3D)

	//ϵͳ��Ϣ
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	//���ư�ť
	ON_BN_CLICKED(IDC_BT_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_BT_SCORE, OnBnClickedScore)
	ON_BN_CLICKED(IDC_BT_SORT_CARD, OnBnClickedSortCard)
	ON_BN_CLICKED(IDC_BT_LAST_TURN, OnBnClickedLastTurn)
	ON_BN_CLICKED(IDC_BT_TRUSTEE, OnBnClickedTrustee)
	ON_BN_CLICKED(IDC_BT_CANCEL_TRUSTEE, OnBnClickedCancelTrustee)

	//��Ϸ��ť
	ON_BN_CLICKED(IDC_BT_OUT_CARD, OnBnClickedOutCard)
	ON_BN_CLICKED(IDC_BT_PASS_CARD, OnBnClickedPassCard)
	ON_BN_CLICKED(IDC_BT_OUT_PROMPT, OnBnClickedOutPrompt)
	ON_BN_CLICKED(IDC_BT_NOTE_CARD, OnBnClickedNoteCard)
	//�зְ�ť
	ON_BN_CLICKED(IDC_CALL_SCORE_1, OnBnClickedCallScore1)
	ON_BN_CLICKED(IDC_CALL_SCORE_2, OnBnClickedCallScore2)
	ON_BN_CLICKED(IDC_CALL_SCORE_3, OnBnClickedCallScore3)
	ON_BN_CLICKED(IDC_CALL_SCORE_NONE, OnBnClickedCallScoreNone)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��Ϸ����
	m_cbBankerScore=0;
	m_wBankerUser=INVALID_CHAIR;
	
	//��ը����
	m_bCartoonBomb=false;
	m_wCartoonBombIndex=0L;

	//�ɻ�����
	m_bCartoonPlan=false;
	m_wCartoonPlanIndex=0L;

	//�������
	m_bCartoonRocket=false;
	m_wCartoonRocketIndex=0l;

	//ׯ�Ҷ���
	m_wBankerMovieIndex=0L;

	//���ƶ���
	m_bReversalCard=false;
	m_wReversalCardIndex=0L;

	//���ƶ���
	m_bRiffleCard=false;
	m_bDispatchCard=false;
	m_wDispatchCardIndex=0L;

	//�ƶ�����
	m_bMoveMouse=false;
	m_bSelectCard=false;
	m_bSwitchCard=false;
	m_cbHoverCardItem=INVALID_ITEM;
	m_cbMouseDownItem=INVALID_ITEM;
	m_cbSwitchCardItem=INVALID_ITEM;

	//��Ϸ����
	m_lCellScore=0.0;
	m_cbBombCount=0L;
	ZeroMemory(m_cbCallScore,sizeof(m_cbCallScore));
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	//��������
	m_wStartUser=INVALID_CHAIR;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//λ�ñ���
	ZeroMemory(&m_ptPass,sizeof(m_ptPass));
	ZeroMemory(&m_ptBanker,sizeof(m_ptBanker));
	ZeroMemory(&m_ptCountWarn,sizeof(m_ptCountWarn));
	ZeroMemory(&m_ptCallScore,sizeof(m_ptCallScore));
	ZeroMemory(&m_ptOrderFlag,sizeof(m_ptOrderFlag));

	//���ñ���
	m_bShowScore=false;
	m_bLastTurnState=false;
	m_bWaitCallScore=false;
	m_bShowNoteCard=false;
	ZeroMemory(&m_bUserPass,sizeof(m_bUserPass));
	ZeroMemory(&m_pHistoryScore,sizeof(m_pHistoryScore));

	//��������
	ZeroMemory(&m_bCountWarn,sizeof(m_bCountWarn));
	ZeroMemory(&m_wCountWarnIndex,sizeof(m_wCountWarnIndex));

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_cbBankerScore=0;
	m_wBankerUser=INVALID_CHAIR;

	//��ը����
	m_bCartoonBomb=false;
	m_wCartoonBombIndex=0L;

	//�ɻ�����
	m_bCartoonPlan=false;
	m_wCartoonPlanIndex=0L;

	//�������
	m_bCartoonRocket=false;
	m_wCartoonRocketIndex=0l;

	//ׯ�Ҷ���
	m_wBankerMovieIndex=0L;

	//���ƶ���
	m_bReversalCard=false;
	m_wReversalCardIndex=0L;

	//���ƶ���
	m_bRiffleCard=false;
	m_bDispatchCard=false;
	m_wDispatchCardIndex=0L;

	//�ƶ�����
	m_bMoveMouse=false;
	m_bSelectCard=false;
	m_bSwitchCard=false;
	m_cbHoverCardItem=INVALID_ITEM;
	m_cbMouseDownItem=INVALID_ITEM;
	m_cbSwitchCardItem=INVALID_ITEM;

	//��Ϸ����
	m_lCellScore=0.0;
	m_cbBombCount=0L;
	ZeroMemory(m_cbCallScore,sizeof(m_cbCallScore));
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	//��������
	m_wStartUser=INVALID_CHAIR;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//���ñ���
	m_bShowScore=false;
	m_bLastTurnState=false;
	m_bWaitCallScore=false;
	m_bShowNoteCard=false;
	ZeroMemory(&m_bUserPass,sizeof(m_bUserPass));
	ZeroMemory(&m_pHistoryScore,sizeof(m_pHistoryScore));

	//��������
	ZeroMemory(&m_bCountWarn,sizeof(m_bCountWarn));
	ZeroMemory(&m_wCountWarnIndex,sizeof(m_wCountWarnIndex));

	//���ý���
	m_btScore.ShowWindow(false);
	m_btStart.ShowWindow(false);
	m_btOutCard.ShowWindow(false);
	m_btPassCard.ShowWindow(false);
	m_btOutPrompt.ShowWindow(false);

	//��Ϸ�ؼ�
	m_btTrustee.ShowWindow(true);
	m_btCancelTrustee.ShowWindow(false);
	m_btTrustee.EnableWindow(false);
	m_btLastTurn.EnableWindow(false);
	m_btSortCard.EnableWindow(false);

	//�зְ�ť
	m_btCallScore1.ShowWindow(false);
	m_btCallScore2.ShowWindow(false);
	m_btCallScore3.ShowWindow(false);
	m_btCallScoreNone.ShowWindow(false);

	//��ʾ��ť
	m_btCardPrompt1.EnableWindow(false);
	m_btCardPrompt2.EnableWindow(false);
	m_btCardPrompt3.EnableWindow(false);
	m_btCardPrompt4.EnableWindow(false);
	m_btCardPrompt5.EnableWindow(false);
	m_btCardPrompt6.EnableWindow(false);

	//��Ϸ�ؼ�
	m_ScoreControl.CloseControl();
	m_BackCardControl.SetCardData(NULL,0);
	m_HandCardControl[MYSELF_VIEW_ID].SetPositively(false);

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HandCardControl[i].SetCardData(NULL,0);
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetDisplayItem(false);
	}

	//������Դ
	if (m_TextureUserPass.IsNull()==false) m_TextureUserPass.Destory();
	if (m_TextureLastTurn.IsNull()==false) m_TextureLastTurn.Destory();
	if (m_TextureCallScore.IsNull()==false) m_TextureCallScore.Destory();
	if (m_TextureCountWarn.IsNull()==false) m_TextureCountWarn.Destory();
	if (m_TextureCartoonBomb.IsNull()==false) m_TextureCartoonBomb.Destory();
	if (m_TextureDispatchCard.IsNull()==false) m_TextureDispatchCard.Destory();
	if (m_TextureHistoryScore.IsNull()==false) m_TextureHistoryScore.Destory();
	if (m_TextureReversalCard.IsNull()==false) m_TextureReversalCard.Destory();
	if (m_TextureWaitCallScore.IsNull()==false) m_TextureWaitCallScore.Destory();

	//��ťͼƬ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage(&m_D3DDevice,TEXT("BT_SHOW_SCORE"),TEXT("PNG"),hResInstance);
	m_btSortCard.SetButtonImage(&m_D3DDevice,TEXT("BT_SORT_CARD_ORDER"),TEXT("PNG"),hResInstance);

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//ͷ��λ��
	m_ptAvatar[0].SetPoint(10,nHeight/2-155);
	m_ptAvatar[1].SetPoint(nWidth/2-332,nHeight-54);
	m_ptAvatar[2].SetPoint(nWidth-79,nHeight/2-155);

	//�ʺ�λ��
	m_ptNickName[0].SetPoint(10,m_ptAvatar[0].y+53);
	m_ptNickName[1].SetPoint(m_ptAvatar[1].x+54,m_ptAvatar[1].y+1);
	m_ptNickName[2].SetPoint(nWidth-78,m_ptAvatar[2].y+53);

	//ʱ��λ��
	m_ptClock[0].SetPoint(nWidth*21/80,nHeight/2-100);
	m_ptClock[1].SetPoint(nWidth/2,nHeight-335);
	m_ptClock[2].SetPoint(nWidth*59/80,nHeight/2-100);

	//׼��λ��
	m_ptReady[0].SetPoint(nWidth*24/80,nHeight/2-90);
	m_ptReady[1].SetPoint(nWidth/2,nHeight-250);
	m_ptReady[2].SetPoint(nWidth*56/80,nHeight/2-90);

	//����λ��
	m_ptPass[0].SetPoint(nWidth*24/80,nHeight/2-90);
	m_ptPass[1].SetPoint(nWidth/2,nHeight-315);
	m_ptPass[2].SetPoint(nWidth*56/80,nHeight/2-90);

	//�з�λ��
	m_ptCallScore[0].SetPoint(nWidth*24/80,nHeight/2-90);
	m_ptCallScore[1].SetPoint(nWidth/2,nHeight-315);
	m_ptCallScore[2].SetPoint(nWidth*56/80,nHeight/2-90);

	//ׯ�ұ�־
	m_ptBanker[0].SetPoint(8,m_ptAvatar[0].y-75);
	m_ptBanker[1].SetPoint(nWidth/2+275,nHeight-105);
	m_ptBanker[2].SetPoint(nWidth-85,m_ptAvatar[2].y-75);

	//����λ��
	m_ptCountWarn[0].SetPoint(25,m_ptAvatar[0].y+150);
	m_ptCountWarn[1].SetPoint(nWidth/2-322,nHeight-118);
	m_ptCountWarn[2].SetPoint(nWidth-60,m_ptAvatar[2].y+150);

	//�ȼ�λ��
	m_ptOrderFlag[0].SetPoint(61,nHeight/2-160);
	m_ptOrderFlag[1].SetPoint(nWidth/2-359,nHeight-58);
	m_ptOrderFlag[2].SetPoint(nWidth-27,nHeight/2-160);

	//����λ��
	m_UserCardControl[0].SetDispatchPos(115,nHeight/2-95);
	m_UserCardControl[1].SetDispatchPos(nWidth/2,nHeight-100);
	m_UserCardControl[2].SetDispatchPos(nWidth-115,nHeight/2-95);

	//�����˿�
	m_BackCardControl.SetBenchmarkPos(nWidth/2,14,enXCenter,enYTop);

	//�û��˿�
	m_HandCardControl[0].SetBenchmarkPos(95,nHeight/2-95,enXLeft,enYCenter);
	m_HandCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-70,enXCenter,enYBottom);
	m_HandCardControl[2].SetBenchmarkPos(nWidth-95,nHeight/2-95,enXRight,enYCenter);

	//�����˿�
	m_UserCardControl[0].SetBenchmarkPos(nWidth*17/80,nHeight/2-95,enXLeft,enYCenter);
	m_UserCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-320,enXCenter,enYCenter);
	m_UserCardControl[2].SetBenchmarkPos(nWidth*63/80,nHeight/2-95,enXRight,enYCenter);

	//���ⰴť
	if (m_bInitD3D==true)
	{
		//��ְ�ť
		CRect rcScore;
		m_btScore.GetClientRect(rcScore);
		m_btScore.SetWindowPos(nWidth-rcScore.Width()-5,8,0,0,SWP_NOSIZE);

		//��ʼ��ť
		CRect rcStart;
		m_btStart.GetClientRect(rcStart);
		m_btStart.SetWindowPos((nWidth-rcStart.Width())/2,nHeight-rcStart.Height()-160,0,0,SWP_NOSIZE);

		//��Ϸ��ť
		CRect rcGameButton;
		m_btOutCard.GetClientRect(rcGameButton);
		m_btPassCard.SetWindowPos(nWidth/2-rcGameButton.Width()*3/2-20,nHeight-rcGameButton.Height()-210,0,0,SWP_NOSIZE);
		m_btOutPrompt.SetWindowPos(nWidth/2-rcGameButton.Width()/2,nHeight-rcGameButton.Height()-210,0,0,SWP_NOSIZE);
		m_btOutCard.SetWindowPos(nWidth/2+rcGameButton.Width()/2+20,nHeight-rcGameButton.Height()-210,0,0,SWP_NOSIZE);

		//�зְ�ť
		CRect rcCallButton;
		m_btCallScore1.GetClientRect(rcCallButton);
		m_btCallScore1.SetWindowPos(nWidth/2-rcCallButton.Width()*2-30,nHeight-rcCallButton.Height()-210,0,0,SWP_NOSIZE);
		m_btCallScore2.SetWindowPos(nWidth/2-rcCallButton.Width()-10,nHeight-rcCallButton.Height()-210,0,0,SWP_NOSIZE);
		m_btCallScore3.SetWindowPos(nWidth/2+10,nHeight-rcCallButton.Height()-210,0,0,SWP_NOSIZE);
		m_btCallScoreNone.SetWindowPos(nWidth/2+rcCallButton.Width()+30,nHeight-rcCallButton.Height()-210,0,0,SWP_NOSIZE);

		//���ܰ�ť
		CRect rcTrustee;
		m_btTrustee.GetClientRect(rcTrustee);
		m_btSortCard.SetWindowPos(nWidth/2+113+rcTrustee.Width()*1,nHeight-rcTrustee.Height()-4,0,0,SWP_NOSIZE);
		m_btLastTurn.SetWindowPos(nWidth/2+113+rcTrustee.Width()*2,nHeight-rcTrustee.Height()-4,0,0,SWP_NOSIZE);
		m_btTrustee.SetWindowPos(nWidth/2+113+rcTrustee.Width()*3,nHeight-rcTrustee.Height()-4,0,0,SWP_NOSIZE);
		m_btCancelTrustee.SetWindowPos(nWidth/2+113+rcTrustee.Width()*3,nHeight-rcTrustee.Height()-4,0,0,SWP_NOSIZE);

		//��ʾ��ť
		CRect rcPromptButton;
		m_btCardPrompt1.GetClientRect(rcPromptButton);
		m_btCardPrompt1.SetWindowPos(nWidth/2-200,nHeight-rcPromptButton.Height()-5,0,0,SWP_NOSIZE);
		m_btCardPrompt2.SetWindowPos(nWidth/2-200+rcPromptButton.Width()*1,nHeight-rcPromptButton.Height()-4,0,0,SWP_NOSIZE);
		m_btCardPrompt3.SetWindowPos(nWidth/2-200+rcPromptButton.Width()*2,nHeight-rcPromptButton.Height()-4,0,0,SWP_NOSIZE);
		m_btCardPrompt4.SetWindowPos(nWidth/2-200+rcPromptButton.Width()*3,nHeight-rcPromptButton.Height()-4,0,0,SWP_NOSIZE);
		m_btCardPrompt5.SetWindowPos(nWidth/2-200+rcPromptButton.Width()*4,nHeight-rcPromptButton.Height()-4,0,0,SWP_NOSIZE);
		m_btCardPrompt6.SetWindowPos(nWidth/2-200+rcPromptButton.Width()*5,nHeight-rcPromptButton.Height()-4,0,0,SWP_NOSIZE);
		
		m_btNoteCard.SetWindowPos(nWidth - 230, nHeight-140,0,0,SWP_NOSIZE);

		//����
		if (m_NoteCard.IsWindowActive()==true)
		{
			CRect rcBtNoteCard;
			CRect rcNoteCard;
			m_btNoteCard.GetWindowRect(rcBtNoteCard);
			m_NoteCard.GetWindowRect(rcNoteCard);
			m_NoteCard.SetWindowPos(rcBtNoteCard.left - 35, rcBtNoteCard.top - rcNoteCard.Height() - 5,0,0,SWP_NOSIZE);
		}
		//�ɼ�����
		if (m_ScoreControl.IsWindowActive()==true)
		{
			CRect rcScoreControl;
			m_ScoreControl.GetWindowRect(rcScoreControl);
			m_ScoreControl.SetWindowPos((nWidth-rcScoreControl.Width())/2,(nHeight-rcScoreControl.Height())*2/5,0,0,SWP_NOSIZE);
		}
	}

	return;
}

//��������
VOID CGameClientView::CartoonMovie()
{
	//�˿˶���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HandCardControl[i].CartoonMovie();
		m_UserCardControl[i].CartoonMovie();
	}

	//�ɻ�����
	if ((m_bCartoonPlan==true)&&(m_CartoonPlanLapseCount.GetLapseCount(10)>0L))
	{
		//���ñ���
		m_wCartoonPlanIndex++;

		//����λ��	
		m_ptCartoonPlan.x -= 30;

		//ֹͣ�ж�
		if (m_ptCartoonPlan.x < -200 )
		{
			//���ñ���
			m_bCartoonPlan=false;
			m_wCartoonPlanIndex=0L;

			m_ptCartoonPlan.SetPoint( 0, 0 );

			//ж����Դ
			ASSERT(m_TextureCartoonPlan.IsNull()==false);
			if (m_TextureCartoonPlan.IsNull()==false) m_TextureCartoonPlan.Destory();
		}
	}

	//�������
	if ((m_bCartoonRocket==true)&&(m_CartoonRocketLapseCount.GetLapseCount(10)>0L))
	{
		//���ñ���
		m_wCartoonRocketIndex++;

		//����λ��	
		m_ptCartoonRocket.y -= 30;

		//ֹͣ�ж�
		if (m_ptCartoonRocket.y < -200 )
		{
			//���ñ���
			m_bCartoonRocket=false;
			m_wCartoonRocketIndex=0L;

			m_ptCartoonRocket.SetPoint( 0, 0 );

			//ж����Դ
			ASSERT(m_TextureCartoonRocket.IsNull()==false);
			if (m_TextureCartoonRocket.IsNull()==false) m_TextureCartoonRocket.Destory();
		}
	}

	//��ը����
	if ((m_bCartoonBomb==true)&&(m_CartoonBombLapseCount.GetLapseCount(180)>0L))
	{
		//���ñ���
		m_wCartoonBombIndex++;

		//ֹͣ�ж�
		if (m_wCartoonBombIndex>=BOMB_CARTOON_COUNT)
		{
			//���ñ���
			m_bCartoonBomb=false;
			m_wCartoonBombIndex=0L;

			//ж����Դ
			ASSERT(m_TextureCartoonBomb.IsNull()==false);
			if (m_TextureCartoonBomb.IsNull()==false) m_TextureCartoonBomb.Destory();
		}
	}

	//���ƶ���
	if ((m_bReversalCard==true)&&(m_ReversalCardLapseCount.GetLapseCount(180)>0L))
	{
		//���ñ���
		m_wReversalCardIndex++;

		//ֹͣ�ж�
		if (m_wReversalCardIndex>=REVERSAL_CARD_COUNT)
		{
			//���ñ���
			m_bReversalCard=false;
			m_wReversalCardIndex=0L;

			//�����˿�
			m_BackCardControl.SetShowCount(MAX_CARD_COUNT);

			//ж����Դ
			ASSERT(m_TextureReversalCard.IsNull()==false);
			if (m_TextureReversalCard.IsNull()==false) m_TextureReversalCard.Destory();

			//������Ϣ
			SendEngineMessage(IDM_REVERSAL_FINISH,0L,0L);
		}
	}

	//ׯ�Ҷ���
	if ((m_wBankerUser!=INVALID_TABLE)&&(m_BankerMovieLapseCount.GetLapseCount(500)>0L))
	{
		//���ñ���
		m_wBankerMovieIndex=(m_wBankerMovieIndex+1)%BANKER_MOVIE_COUNT;
	}

	//ϴ�ƶ���
	if ((m_bRiffleCard==true)&&(m_DispatchCardLapseCount.GetLapseCount(100)>0L))
	{
		//���ñ���
		m_wDispatchCardIndex++;

		//ֹͣ�ж�
		if (m_wDispatchCardIndex>=(RIFFLE_CARD_COUNT_ALL+6))
		{
			//���ñ���
			m_bRiffleCard=false;
			m_bDispatchCard=true;
			m_wDispatchCardIndex=0L;

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
		}
	}

	//���ƶ���
	if ((m_bDispatchCard==true)&&(m_DispatchCardLapseCount.GetLapseCount(10)>0L))
	{
		//���ñ���
		m_wDispatchCardIndex++;

		//��������
		WORD wBaseIndex=DISPATCH_CARD_SPACE+DISPATCH_CARD_TIMES;

		//�����˿�
		if ((m_wDispatchCardIndex>=wBaseIndex)&&((m_wDispatchCardIndex-wBaseIndex)%DISPATCH_CARD_SPACE==0))
		{
			//��������
			WORD wCardIndex=m_wDispatchCardIndex-wBaseIndex;
			WORD wUserIndex=(m_wStartUser+wCardIndex/DISPATCH_CARD_SPACE)%GAME_PLAYER;

			//�����˿�
			if (wUserIndex==MYSELF_VIEW_ID || m_bUserCheat[wUserIndex])
			{
				//��ȡ�˿�
				BYTE cbCardData[MAX_COUNT];
				BYTE cbCardCount=m_HandCardControl[wUserIndex].GetCardData(cbCardData,CountArray(cbCardData));

				//�����˿�
				cbCardData[cbCardCount]=m_cbHandCardData[wUserIndex][cbCardCount++];
				m_GameLogic.SortCardList(cbCardData,cbCardCount,ST_ORDER);

				//�����˿�
				m_HandCardControl[wUserIndex].SetCardData(cbCardData,cbCardCount);
			}
			else
			{
				//����˿�
				m_HandCardControl[wUserIndex].SetCardData(m_HandCardControl[wUserIndex].GetCardCount()+1);
			}
		}

		//ֹͣ�ж�
		if (m_wDispatchCardIndex>=DISPATCH_COUNT*DISPATCH_CARD_SPACE+DISPATCH_CARD_TIMES)
		{
			//���ñ���
			m_bDispatchCard=false;
			m_wDispatchCardIndex=0L;

			//ж����Դ
			ASSERT(m_TextureDispatchCard.IsNull()==false);
			if (m_TextureDispatchCard.IsNull()==false) m_TextureDispatchCard.Destory();

			//������Ϣ
			SendEngineMessage(IDM_DISPATCH_FINISH,0L,0L);
		}
	}

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_bCountWarn[i]==true)&&(m_CountWarnLapseCount[i].GetLapseCount(180)>0L))
		{
			//���ñ���
			m_wCountWarnIndex[i]=(m_wCountWarnIndex[i]+1)%CARD_WARNING_COUNT;
		}
	}

	return;
}

//�����豸
VOID CGameClientView::InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	//��Ϸ��ť
	m_btStart.ActiveWindow(rcCreate,0,IDC_BT_START,&m_VirtualEngine,NULL);
	m_btScore.ActiveWindow(rcCreate,0,IDC_BT_SCORE,&m_VirtualEngine,NULL);
	m_btOutCard.ActiveWindow(rcCreate,0,IDC_BT_OUT_CARD,&m_VirtualEngine,NULL);
	m_btPassCard.ActiveWindow(rcCreate,0,IDC_BT_PASS_CARD,&m_VirtualEngine,NULL);
	m_btOutPrompt.ActiveWindow(rcCreate,0,IDC_BT_OUT_PROMPT,&m_VirtualEngine,NULL);
	m_btNoteCard.ActiveWindow(rcCreate,0,IDC_BT_NOTE_CARD,&m_VirtualEngine,NULL);

	//�зְ�ť
	m_btCallScore1.ActiveWindow(rcCreate,0,IDC_CALL_SCORE_1,&m_VirtualEngine,NULL);
	m_btCallScore2.ActiveWindow(rcCreate,0,IDC_CALL_SCORE_2,&m_VirtualEngine,NULL);
	m_btCallScore3.ActiveWindow(rcCreate,0,IDC_CALL_SCORE_3,&m_VirtualEngine,NULL);
	m_btCallScoreNone.ActiveWindow(rcCreate,0,IDC_CALL_SCORE_NONE,&m_VirtualEngine,NULL);

	//���ܰ�ť
	m_btCancelTrustee.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_BT_CANCEL_TRUSTEE,&m_VirtualEngine,NULL);
	m_btTrustee.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_BT_TRUSTEE,&m_VirtualEngine,NULL);
	m_btLastTurn.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_BT_LAST_TURN,&m_VirtualEngine,NULL);
	m_btSortCard.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_BT_SORT_CARD,&m_VirtualEngine,NULL);
	m_btCancelTrustee.ShowWindow(false);

	//��ʾ��ť
	m_btCardPrompt1.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_CARD_PROMPT_1,&m_VirtualEngine,NULL);
	m_btCardPrompt2.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_CARD_PROMPT_2,&m_VirtualEngine,NULL);
	m_btCardPrompt3.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_CARD_PROMPT_3,&m_VirtualEngine,NULL);
	m_btCardPrompt4.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_CARD_PROMPT_4,&m_VirtualEngine,NULL);
	m_btCardPrompt5.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_CARD_PROMPT_5,&m_VirtualEngine,NULL);
	m_btCardPrompt6.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_CARD_PROMPT_6,&m_VirtualEngine,NULL);

	//��Ϸ��ť
	m_btStart.SetButtonImage(pD3DDevice,TEXT("BT_START"),TEXT("PNG"),hResInstance);
	m_btScore.SetButtonImage(pD3DDevice,TEXT("BT_SHOW_SCORE"),TEXT("PNG"),hResInstance);
	m_btOutCard.SetButtonImage(pD3DDevice,TEXT("BT_OUT_CARD"),TEXT("PNG"),hResInstance);
	m_btPassCard.SetButtonImage(pD3DDevice,TEXT("BT_PASS_CARD"),TEXT("PNG"),hResInstance);
	m_btOutPrompt.SetButtonImage(pD3DDevice,TEXT("BT_OUT_PROMPT"),TEXT("PNG"),hResInstance);
	m_btNoteCard.SetButtonImage(pD3DDevice,TEXT("BT_NOTE_CARD"),TEXT("PNG"),hResInstance);

	//���ܰ�ť
	m_btLastTurn.SetButtonImage(pD3DDevice,TEXT("BT_LAST_TURN"),TEXT("PNG"),hResInstance);
	m_btTrustee.SetButtonImage(pD3DDevice,TEXT("BT_START_TRUSTEE"),TEXT("PNG"),hResInstance);
	m_btCancelTrustee.SetButtonImage(pD3DDevice,TEXT("BT_STOP_TRUSTEE"),TEXT("PNG"),hResInstance);
	m_btSortCard.SetButtonImage(pD3DDevice,TEXT("BT_SORT_CARD_COUNT"),TEXT("PNG"),hResInstance);

	//�зְ�ť
	m_btCallScore1.SetButtonImage(pD3DDevice,TEXT("BT_CALL_SCORE_1"),TEXT("PNG"),hResInstance);
	m_btCallScore2.SetButtonImage(pD3DDevice,TEXT("BT_CALL_SCORE_2"),TEXT("PNG"),hResInstance);
	m_btCallScore3.SetButtonImage(pD3DDevice,TEXT("BT_CALL_SCORE_3"),TEXT("PNG"),hResInstance);
	m_btCallScoreNone.SetButtonImage(pD3DDevice,TEXT("BT_CALL_SCORE_NONE"),TEXT("PNG"),hResInstance);

	//��ʾ��ť
	m_btCardPrompt1.SetButtonImage(pD3DDevice,TEXT("BT_CARD_PROMPT_1"),TEXT("PNG"),hResInstance);
	m_btCardPrompt2.SetButtonImage(pD3DDevice,TEXT("BT_CARD_PROMPT_2"),TEXT("PNG"),hResInstance);
	m_btCardPrompt3.SetButtonImage(pD3DDevice,TEXT("BT_CARD_PROMPT_3"),TEXT("PNG"),hResInstance);
	m_btCardPrompt4.SetButtonImage(pD3DDevice,TEXT("BT_CARD_PROMPT_4"),TEXT("PNG"),hResInstance);
	m_btCardPrompt5.SetButtonImage(pD3DDevice,TEXT("BT_CARD_PROMPT_5"),TEXT("PNG"),hResInstance);
	m_btCardPrompt6.SetButtonImage(pD3DDevice,TEXT("BT_CARD_PROMPT_6"),TEXT("PNG"),hResInstance);

	//�˿���Դ
	CCardControl::LoadCardResource(pD3DDevice);

	//������Դ
	m_TextureBack.LoadImage(pD3DDevice,hResInstance,TEXT("VIEW_BACK"),TEXT("PNG"));
	m_TextureFill.LoadImage(pD3DDevice,hResInstance,TEXT("VIEW_FILL"),TEXT("PNG"));
	m_TextureUserPlayer.LoadImage(pD3DDevice,hResInstance,TEXT("USER_FRAME_PLAYER"),TEXT("PNG"));
	m_TextureUserMySelf.LoadImage(pD3DDevice,hResInstance,TEXT("USER_FRAME_MYSELF"),TEXT("PNG"));
	m_TextureMemberOrder.LoadImage(pD3DDevice,hResInstance,TEXT("MEMBER_FLAG"),TEXT("PNG"));

	//��������
	m_TextureCellNumber.LoadImage(pD3DDevice,hResInstance,TEXT("CELL_NUMBER"),TEXT("PNG"));
	m_TextureBankerFlag.LoadImage(pD3DDevice,hResInstance,TEXT("BANKER_FLAG"),TEXT("PNG"));
	m_TextureCellScoreBack.LoadImage(pD3DDevice,hResInstance,TEXT("CELL_SCORE_BACK"),TEXT("PNG"));
	m_TextureBankerCardBack.LoadImage(pD3DDevice,hResInstance,TEXT("BANKER_CARD_BACK"),TEXT("PNG"));
	m_TextureRemnantCardNumber.LoadImage(pD3DDevice,hResInstance,TEXT("REMNANT_CARD_NUMBER"),TEXT("PNG"));

	m_TextureFenWei.LoadImage(pD3DDevice,hResInstance,TEXT("FEN_WEI"),TEXT("PNG"));
	m_TextureFenTou.LoadImage(pD3DDevice,hResInstance,TEXT("FEN_TOU"),TEXT("PNG"));

	//�����˿�
	m_BackCardControl.SetSmallMode(true);
	m_BackCardControl.SetDisplayItem(true);
	m_BackCardControl.SetBackGround(CARD_LAND);
	m_BackCardControl.SetCardDistance(70L,0L,0L);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//����˿�
		if ((i==0)||(i==2))
		{
			m_HandCardControl[i].SetSmallMode(true);
			m_HandCardControl[i].SetDirection(false);
			m_HandCardControl[i].SetCardDistance(DEF_X_DISTANCE_SMALL,15,0);
		}

		//�û��˿�
		if (i==MYSELF_VIEW_ID)
		{
			m_HandCardControl[i].SetDirection(true);
			m_HandCardControl[i].SetSmallMode(false);
			m_HandCardControl[i].SetCardDistance(DEF_X_DISTANCE,DEF_Y_DISTANCE,DEF_SHOOT_DISTANCE);
		}

		//�����˿�
		m_UserCardControl[i].SetDisplayItem(true);
		m_UserCardControl[i].SetCardDistance(DEF_X_DISTANCE,0,0);
	}

	//������Ⱦ
	StartRenderThread();

	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//��ȡ��С
	CSize SizeBack;
	CSize SizeUserUserMySelf;
	SizeBack.SetSize(m_TextureBack.GetWidth(),m_TextureBack.GetHeight());
	SizeUserUserMySelf.SetSize(m_TextureUserMySelf.GetWidth(),m_TextureUserMySelf.GetHeight());

	//�滭����
	DrawViewImage(pD3DDevice,m_TextureFill,DRAW_MODE_SPREAD);
	m_TextureBack.DrawImage(pD3DDevice,(nWidth-SizeBack.cx)/2,(nHeight-SizeBack.cy)*2/5,SizeBack.cx,SizeBack.cy,0,0);

	//���Ʊ���
	m_TextureBankerCardBack.DrawImage(pD3DDevice,(nWidth-m_TextureBankerCardBack.GetWidth())/2,5);

	//�з�
	TCHAR szOutNumT[128] = {0};
	m_TextureCellScoreBack.DrawImage(pD3DDevice,(nWidth)/2,111);
	if ( m_cbBankerScore != 0 && m_cbBankerScore != 255 )
	{
		_sntprintf(szOutNumT,CountArray(szOutNumT),SCORE_STRING,(SCORE)m_cbBankerScore);
		DrawNumber(pD3DDevice,&m_TextureCellNumber, TEXT(".0123456789"),szOutNumT,(nWidth)/2 + 60 ,113, DT_CENTER);
	}

	//�滭��λ����
	_sntprintf(szOutNumT,CountArray(szOutNumT),SCORE_STRING,m_lCellScore);
	int nOutNumber = lstrlen(szOutNumT);
	if ( nOutNumber > 3 )
	{
		m_TextureFenWei.DrawImage(pD3DDevice,(nWidth)/2 - 20,114);
		DrawNumber(pD3DDevice,&m_TextureCellNumber, TEXT(".0123456789"),szOutNumT,(nWidth)/2 - 25 ,113, DT_RIGHT);
		m_TextureFenTou.DrawImage(pD3DDevice,(nWidth)/2 - 60 - nOutNumber * (m_TextureCellNumber.GetWidth()/10),113);
	}
	else
	{
		m_TextureFenWei.DrawImage(pD3DDevice,(nWidth)/2 - 15,114);
		DrawNumber(pD3DDevice,&m_TextureCellNumber, TEXT(".0123456789"),szOutNumT,(nWidth)/2 - 37 ,113, DT_CENTER);
		m_TextureFenTou.DrawImage(pD3DDevice,(nWidth)/2 - 55 - 3 * (m_TextureCellNumber.GetWidth()/10),113);
	}

	//��ҿ��
	m_TextureUserPlayer.DrawImage(pD3DDevice,m_ptAvatar[0].x-9,m_ptAvatar[0].y-10);
	m_TextureUserPlayer.DrawImage(pD3DDevice,m_ptAvatar[2].x-9,m_ptAvatar[2].y-10);
	m_TextureUserMySelf.DrawImage(pD3DDevice,(nWidth-SizeUserUserMySelf.cx)/2,nHeight-SizeUserUserMySelf.cy);

	//�����˿�
	m_BackCardControl.DrawCardControl(pD3DDevice);
	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		if (pIClientUserItem==NULL) continue;

		//�û�����
		DrawUserClock(pD3DDevice,m_ptClock[i].x,m_ptClock[i].y,GetUserClock(i));
		DrawUserAvatar(pD3DDevice,m_ptAvatar[i].x,m_ptAvatar[i].y,pIClientUserItem);

		//λ�ö���
		CRect rcNickName(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+72,m_ptNickName[i].y+12);
		CRect rcUserScore(m_ptNickName[i].x,m_ptNickName[i].y+17,m_ptNickName[i].x+72,m_ptNickName[i].y+29);
		CRect rcUserLevel(m_ptNickName[i].x,m_ptNickName[i].y+34,m_ptNickName[i].x+72,m_ptNickName[i].y+46);
		CRect rcUserUserID(m_ptNickName[i].x,m_ptNickName[i].y+51,m_ptNickName[i].x+72,m_ptNickName[i].y+70);

		//�滭�û�
		LPCTSTR pszNickName=pIClientUserItem->GetNickName();
		DrawTextString(pD3DDevice,pszNickName,rcNickName,DT_LEFT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(153,255,0),D3DCOLOR_XRGB(0,50,0));

		//�û�����
		TCHAR szUserScore[64]=TEXT("");
		_sntprintf(szUserScore,CountArray(szUserScore),SCORE_STRING,pIClientUserItem->GetUserScore());
		DrawTextString(pD3DDevice,szUserScore,rcUserScore,DT_LEFT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255,255,0),D3DCOLOR_XRGB(0,50,0));

		//��Ϸ�ȼ�
		LPCTSTR pszUserLevel=GetLevelDescribe(pIClientUserItem);
		DrawTextString(pD3DDevice,/*pszUserLevel*/pIClientUserItem->GetAddrDescribe(),rcUserLevel,DT_LEFT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255,255,255),D3DCOLOR_XRGB(0,50,0));

				CGameClientEngine *pGameClientDlg=(CGameClientEngine*)CGameFrameEngine::GetInstance();
				IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;
		if(CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight))
		{
			TCHAR szUserUserID[64]=TEXT("");
			_sntprintf(szUserUserID,CountArray(szUserUserID),TEXT("%d"),pIClientUserItem->GetUserID());
			DrawTextString(pD3DDevice,szUserUserID,rcUserUserID,DT_LEFT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255,255,255),D3DCOLOR_XRGB(0,50,0));
		}

		//ׯ�ұ�־
		if (m_wBankerUser!=INVALID_CHAIR)
		{
			//��ȡ��С
			CSize SizeBankerFlag;
			SizeBankerFlag.SetSize(m_TextureBankerFlag.GetWidth()/(BANKER_MOVIE_COUNT*2),m_TextureBankerFlag.GetHeight());

			//�滭��־
			if ((GetUserClock(i)>0))
			{
				m_TextureBankerFlag.DrawImage(pD3DDevice,m_ptBanker[i].x,m_ptBanker[i].y,SizeBankerFlag.cx,SizeBankerFlag.cy,
					SizeBankerFlag.cx*((i==m_wBankerUser)?m_wBankerMovieIndex:BANKER_MOVIE_COUNT+m_wBankerMovieIndex),0);
			}
			else
			{
				m_TextureBankerFlag.DrawImage(pD3DDevice,m_ptBanker[i].x,m_ptBanker[i].y,SizeBankerFlag.cx,SizeBankerFlag.cy,
					SizeBankerFlag.cx*((i==m_wBankerUser)?0:BANKER_MOVIE_COUNT),0);
			}
		}

		//��Ա�ȼ�
		if (pIClientUserItem->GetMemberOrder()!=0)
		{
			//��������
			BYTE cbMemberOrder=pIClientUserItem->GetMemberOrder();

			//��ȡ��С
			CSize SizeMember;
			SizeMember.SetSize(m_TextureMemberOrder.GetWidth()/5,m_TextureMemberOrder.GetHeight());

			//�滭��־
			m_TextureMemberOrder.DrawImage(pD3DDevice,m_ptOrderFlag[i].x,m_ptOrderFlag[i].y,SizeMember.cx,SizeMember.cy,SizeMember.cx*(cbMemberOrder - 1),0);
			
		}
	}

	//�滭�˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HandCardControl[i].DrawCardControl(pD3DDevice);
		m_UserCardControl[i].DrawCardControl(pD3DDevice);
	}


	//ʣ���˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (i!=MYSELF_VIEW_ID)
		{
			//��������
			WORD wCardCount=m_HandCardControl[i].GetCardCount();

			//�滭����
			if (wCardCount>0)
			{
				//��ȡλ��
				CSize ControlSize;
				CPoint OriginPoint;
				m_HandCardControl[i].GetControlSize(ControlSize);
				m_HandCardControl[i].GetOriginPoint(OriginPoint);

				//�滭����
				INT nXPos=OriginPoint.x+ControlSize.cx;
				INT nYPos=OriginPoint.y+ControlSize.cy;
				DrawNumberString(pD3DDevice,m_TextureRemnantCardNumber,wCardCount,nXPos-26,nYPos+18);
				DrawTextString(pD3DDevice,TEXT("ʣ��      ��"),nXPos-67,nYPos+12,DT_LEFT,D3DCOLOR_XRGB(255,255,255),D3DCOLOR_XRGB(0,30,0));
			}
		}
	}

	//�û���־
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		if (pIClientUserItem==NULL) continue;

		//ͬ���־
		BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
		if (cbUserStatus==US_READY) DrawUserReady(pD3DDevice,m_ptReady[i].x,m_ptReady[i].y);
	}

	//����״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bUserPass[i]==true)
		{
			CSize SizeUserPass;
			SizeUserPass.SetSize(m_TextureUserPass.GetWidth(),m_TextureUserPass.GetHeight());
			m_TextureUserPass.DrawImage(pD3DDevice,m_ptPass[i].x-SizeUserPass.cx/2,m_ptPass[i].y-SizeUserPass.cy/2);
		}
	}

	//�û��з�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbCallScore[i]!=0)
		{
			//��ȡ��С
			CSize SizeCallScore;
			SizeCallScore.SetSize(m_TextureCallScore.GetWidth()/4,m_TextureCallScore.GetHeight());

			//�滭��־
			INT nImageIndex=(m_cbCallScore[i]==0xFF)?3:(m_cbCallScore[i]-1);
			m_TextureCallScore.DrawImage(pD3DDevice,m_ptCallScore[i].x-SizeCallScore.cx/2,m_ptCallScore[i].y-SizeCallScore.cy/2,
				SizeCallScore.cx,SizeCallScore.cy,nImageIndex*SizeCallScore.cx,0);
		}
	}

	//���ֱ�־
	if (m_bLastTurnState==true)
	{
		CSize SizeLastTurn;
		SizeLastTurn.SetSize(m_TextureLastTurn.GetWidth(),m_TextureLastTurn.GetHeight());
		m_TextureLastTurn.DrawImage(pD3DDevice,(nWidth-SizeLastTurn.cx)/2,(nHeight-SizeLastTurn.cy)/2-100);
	}

	//�ȴ��з�
	if (m_bWaitCallScore==true)
	{
		CSize SizeWaitCallScore;
		SizeWaitCallScore.SetSize(m_TextureWaitCallScore.GetWidth(),m_TextureWaitCallScore.GetHeight());
		m_TextureWaitCallScore.DrawImage(pD3DDevice,(nWidth-SizeWaitCallScore.cx)/2,(nHeight-SizeWaitCallScore.cy)/2-135);
	}

	//�ɻ�����
	if (m_bCartoonPlan==true)
	{
		//��ȡ��С
		CSize SizeCartoonPlan;
		SizeCartoonPlan.SetSize(m_TextureCartoonPlan.GetWidth()/PLAN_CARTOON_COUNT,m_TextureCartoonPlan.GetHeight());

		//�滭Ч��
		m_TextureCartoonPlan.DrawImage(pD3DDevice,m_ptCartoonPlan.x,m_ptCartoonPlan.y - SizeCartoonPlan.cy/2,SizeCartoonPlan.cx,SizeCartoonPlan.cy,
			SizeCartoonPlan.cx*(m_wCartoonPlanIndex%PLAN_CARTOON_COUNT),0);
	}

	//�������
	if (m_bCartoonRocket==true)
	{
		//��ȡ��С
		CSize SizeCartoonRocket;
		SizeCartoonRocket.SetSize(m_TextureCartoonRocket.GetWidth()/3,m_TextureCartoonRocket.GetHeight());

		//�滭Ч��
		m_TextureCartoonRocket.DrawImage(pD3DDevice,m_ptCartoonRocket.x - SizeCartoonRocket.cx/2,m_ptCartoonRocket.y,SizeCartoonRocket.cx,SizeCartoonRocket.cy,
			SizeCartoonRocket.cx*(m_wCartoonRocketIndex%3),0);
	}

	//��ը����
	if (m_bCartoonBomb==true)
	{
		//��ȡ��С
		CSize SizeCartoonBomb;
		SizeCartoonBomb.SetSize(m_TextureCartoonBomb.GetWidth()/BOMB_CARTOON_COUNT,m_TextureCartoonBomb.GetHeight());

		//�滭Ч��
		INT nYPosScreen=__max(nHeight-SizeCartoonBomb.cy-250,0);
		m_TextureCartoonBomb.DrawImage(pD3DDevice,(nWidth-SizeCartoonBomb.cx)/2,nYPosScreen,SizeCartoonBomb.cx,SizeCartoonBomb.cy,
			SizeCartoonBomb.cx*(m_wCartoonBombIndex%BOMB_CARTOON_COUNT),0);
	}

	//���ƶ���
	if (m_bReversalCard==true)
	{
		//��ȡ��С
		CSize SizeReversalCard;
		SizeReversalCard.SetSize(m_TextureReversalCard.GetWidth()/REVERSAL_CARD_COUNT,m_TextureReversalCard.GetHeight());

		//�滭Ч��
		for (BYTE i=0;i<3;i++)
		{
			m_TextureReversalCard.DrawImage(pD3DDevice,nWidth/2+i*70-102,13,SizeReversalCard.cx,SizeReversalCard.cy,
				SizeReversalCard.cx*(m_wReversalCardIndex%REVERSAL_CARD_COUNT),0);
		}
	}

	//ϴ�ƶ���
	if (m_bRiffleCard==true)
	{
		if (m_wDispatchCardIndex>=RIFFLE_CARD_COUNT_ALL)
		{
			//��ȡ��С
			CSize SizeCardItem;
			CCardControl::m_CardResource.GetCardSize(true,SizeCardItem);

			//��ȡ����
			CD3DSprite * pD3DTexture=NULL;
			CCardControl::m_CardResource.GetCardTexture(true,&pD3DTexture);

			//����λ��
			INT nDistance=__min(DISPATCH_HEAP_DISTANCE-2,m_wDispatchCardIndex-RIFFLE_CARD_COUNT_ALL)+2;
			INT nXSourcePos=(nWidth-SizeCardItem.cx-(DISPATCH_COUNT-1)*nDistance)/2;

			//��ȡ����
			for (BYTE i=0;i<DISPATCH_COUNT;i++)
			{
				INT nXDrawPos=nXSourcePos+i*nDistance;
				pD3DTexture->DrawImage(pD3DDevice,nXDrawPos,nHeight/2-200,SizeCardItem.cx,SizeCardItem.cy,SizeCardItem.cx*2,SizeCardItem.cy*4);
			}
		}
		else
		{
			//��ȡ��С
			CSize SizeDispatchCard;
			SizeDispatchCard.SetSize(m_TextureDispatchCard.GetWidth()/RIFFLE_CARD_COUNT_X,m_TextureDispatchCard.GetHeight()/RIFFLE_CARD_COUNT_Y);

			//�滭Ч��
			m_TextureDispatchCard.DrawImage(pD3DDevice,(nWidth-SizeDispatchCard.cx)/2,nHeight/2-220,SizeDispatchCard.cx,SizeDispatchCard.cy,
				SizeDispatchCard.cx*(m_wDispatchCardIndex%RIFFLE_CARD_COUNT_X),SizeDispatchCard.cy*(m_wDispatchCardIndex/RIFFLE_CARD_COUNT_X));
		}
	}

	//���ƶ���
	if (m_bDispatchCard==true)
	{
		//��������
		CSize SizeControl[GAME_PLAYER];
		CPoint PointControl[GAME_PLAYER];

		//��ȡ��С
		CSize SizeCardItem;
		CCardControl::m_CardResource.GetCardSize(true,SizeCardItem);

		//��ȡ����
		CD3DSprite * pD3DTexture=NULL;
		CCardControl::m_CardResource.GetCardTexture(true,&pD3DTexture);

		//����λ��
		INT nYHeapPos=nHeight/2-200;
		INT nXHeapPos=(nWidth-SizeCardItem.cx-(DISPATCH_COUNT-1)*DISPATCH_HEAP_DISTANCE)/2;

		//��ȡλ��
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_HandCardControl[i].GetControlSize(SizeControl[i]);
			m_HandCardControl[i].GetOriginPoint(PointControl[i]);
		}

		//�滭�ƶ�
		for (BYTE i=0;i<DISPATCH_COUNT;i++)
		{
			if ((DISPATCH_COUNT-i)*DISPATCH_CARD_SPACE>m_wDispatchCardIndex)
			{
				INT nXDrawPos=nXHeapPos+i*DISPATCH_HEAP_DISTANCE;
				pD3DTexture->DrawImage(pD3DDevice,nXDrawPos,nHeight/2-200,SizeCardItem.cx,SizeCardItem.cy,SizeCardItem.cx*2,SizeCardItem.cy*4);
			}
		}

		//�滭����
		for (BYTE i=0;i<DISPATCH_COUNT;i++)
		{
			if ((m_wDispatchCardIndex>=(i+1)*DISPATCH_CARD_SPACE)&&(m_wDispatchCardIndex<(i+1)*DISPATCH_CARD_SPACE+DISPATCH_CARD_TIMES))
			{
				//��������
				WORD wSendUser=(m_wStartUser+i)%GAME_PLAYER;
				WORD wSendIndex=m_wDispatchCardIndex-(i+1)*DISPATCH_CARD_SPACE;

				//Ŀ��λ��
				INT nXTargetPos=PointControl[wSendUser].x+SizeControl[wSendUser].cx/2;
				INT nYTargetPos=PointControl[wSendUser].y+SizeControl[wSendUser].cy/2;

				//���λ��
				INT nYSourcePos=nYHeapPos+SizeCardItem.cy/2;
				INT nXSourcePos=nXHeapPos+(DISPATCH_COUNT-i-1)*DISPATCH_HEAP_DISTANCE+SizeCardItem.cx/2;

				//�滭�˿�
				INT nXDrawPos=nXSourcePos+wSendIndex*(nXTargetPos-nXSourcePos)/DISPATCH_CARD_TIMES-SizeCardItem.cx/2;
				INT nYDrawPos=nYSourcePos+wSendIndex*(nYTargetPos-nYSourcePos)/DISPATCH_CARD_TIMES-SizeCardItem.cy/2;

				INT nXImagePos=2;
				INT nYImagePos=4;
				if (m_bUserCheat[wSendUser])
				{
					BYTE cbCardData = m_cbHandCardData[wSendUser][i / GAME_PLAYER];
					if ((cbCardData==0x4E)||(cbCardData==0x4F))
					{
						nXImagePos=((cbCardData&CARD_MASK_VALUE)%14);
						nYImagePos=((cbCardData&CARD_MASK_COLOR)>>4);
					}
					else
					{
						nXImagePos=((cbCardData&CARD_MASK_VALUE)-1);
						nYImagePos=((cbCardData&CARD_MASK_COLOR)>>4);
					}
				}
				pD3DTexture->DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeCardItem.cx,SizeCardItem.cy,SizeCardItem.cx*nXImagePos,SizeCardItem.cy*nYImagePos);
			}
		}
	}

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bCountWarn[i]==true)
		{
			//��ȡ��С
			CSize SizeCountWarn;
			SizeCountWarn.SetSize(m_TextureCountWarn.GetWidth()/CARD_WARNING_COUNT,m_TextureCountWarn.GetHeight());

			//�滭Ч��
			m_TextureCountWarn.DrawImage(pD3DDevice,m_ptCountWarn[i].x,m_ptCountWarn[i].y,SizeCountWarn.cx,SizeCountWarn.cy,
				SizeCountWarn.cx*m_wCountWarnIndex[i],0);
		}
	}


	//��ʷ����
	if (m_bShowScore==true)
	{
		//��ȡ��С
		CSize SizeHistoryScore;
		SizeHistoryScore.SetSize(m_TextureHistoryScore.GetWidth(),m_TextureHistoryScore.GetHeight());

		//���ֱ���
		m_TextureHistoryScore.DrawImage(pD3DDevice,nWidth-SizeHistoryScore.cx-40,5);

		//����λ��
		INT nYBenchmark=5;
		INT nXBenchmark=nWidth-SizeHistoryScore.cx-40;

		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{			
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=GetClientUserItem(i);
			if ((pIClientUserItem==NULL)||(m_pHistoryScore[i]==NULL)) continue;

			//λ�ü���
			CRect rcAccounts(nXBenchmark+13,nYBenchmark+31+i*23,nXBenchmark+83,nYBenchmark+43+i*23);
			CRect rcTurnScore(nXBenchmark+85,nYBenchmark+31+i*23,nXBenchmark+135,nYBenchmark+43+i*23);
			CRect rcCollectScore(nXBenchmark+139,nYBenchmark+31+i*23,nXBenchmark+185,nYBenchmark+43+i*23);

			//������Ϣ
			TCHAR szTurnScore[16]=TEXT("");
			TCHAR szCollectScore[16]=TEXT("");
			_sntprintf(szTurnScore,CountArray(szTurnScore),SCORE_STRING,m_pHistoryScore[i]->lTurnScore);
			_sntprintf(szCollectScore,CountArray(szCollectScore),SCORE_STRING,m_pHistoryScore[i]->lCollectScore);

			//�滭��Ϣ
			D3DCOLOR crColor1=D3DCOLOR_XRGB(255,153,0);
			D3DCOLOR crColor2=D3DCOLOR_XRGB(0,255,153);
			LPCTSTR pszNickName=pIClientUserItem->GetNickName();
			DrawTextString(pD3DDevice,pszNickName,&rcAccounts,DT_END_ELLIPSIS|DT_LEFT,D3DCOLOR_XRGB(255,255,255));
			DrawTextString(pD3DDevice,szTurnScore,&rcTurnScore,DT_END_ELLIPSIS|DT_CENTER,(m_pHistoryScore[i]->lTurnScore>=0)?crColor1:crColor2);
			DrawTextString(pD3DDevice,szCollectScore,&rcCollectScore,DT_END_ELLIPSIS|DT_CENTER,(m_pHistoryScore[i]->lCollectScore>=0)?crColor1:crColor2);
		}
	}

	return;
}

//��Ԫ����
VOID CGameClientView::SetCellScore(SCORE lCellScore)
{
	if (m_lCellScore!=lCellScore)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	return;
}

//ը������
VOID CGameClientView::SetBombCount(BYTE cbBombCount)
{
	if (m_cbBombCount!=cbBombCount)
	{
		//���ñ���
		m_cbBombCount=cbBombCount;

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	return;
}

//����ׯ��
VOID CGameClientView::SetBankerUser(WORD wBankerUser)
{
	if (m_wBankerUser!=wBankerUser)
	{
		//���ñ���
		m_wBankerUser=wBankerUser;

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	return;
}

//���ñ���
VOID CGameClientView::SetBankerScore(BYTE cbBankerScore)
{
	if (m_cbBankerScore!=cbBankerScore)
	{
		//���ñ���
		m_cbBankerScore=cbBankerScore;

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	return;
}

//��ʾ����
VOID CGameClientView::ShowBankerCard(BYTE cbCardData[3])
{
	//���ñ���
	m_bReversalCard=true;
	m_wReversalCardIndex=0;

	//����ʱ��
	m_ReversalCardLapseCount.Initialization();

	//���ÿؼ�
	m_BackCardControl.SetShowCount(0);
	m_BackCardControl.SetCardData(cbCardData,3);

	//������Դ
	ASSERT(m_TextureReversalCard.IsNull()==true);
	if (m_TextureReversalCard.IsNull()==true) m_TextureReversalCard.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("REVERSAL_CARD"),TEXT("PNG"));

	//���½���
	CRect rcClient;
	GetClientRect(&rcClient);
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//���ý���
VOID CGameClientView::ShowGameScoreInfo(tagScoreInfo & ScoreInfo)
{
	//������Ϣ
	m_ScoreControl.SetScoreInfo(ScoreInfo);

	//�����ؼ�
	if (m_ScoreControl.IsWindowActive()==false)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//��������
		CRect rcCreate(0,0,0,0);
		m_ScoreControl.ActiveWindow(rcCreate,WS_VISIBLE,IDC_GAME_SCORE,&m_VirtualEngine,NULL);

		//�ƶ�λ��
		CRect rcScoreControl;
		m_ScoreControl.GetWindowRect(rcScoreControl);
		m_ScoreControl.SetWindowPos((rcClient.Width()-rcScoreControl.Width())/2,(rcClient.Height()-rcScoreControl.Height())*2/5,0,0,SWP_NOSIZE);
	}

	return;
}



// �滭����
void CGameClientView::DrawNumber( CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),SCORE_STRING,lOutNum);
	DrawNumber(pD3DDevice, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CGameClientView::DrawNumber( CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNumT[128] = {0};
	_sntprintf(szOutNumT,CountArray(szOutNumT),TEXT("%s"),szOutNum);
	DrawNumber(pD3DDevice, ImageNumber, szImageNum, szOutNumT, nXPos, nYPos, uFormat);
}


// �滭����
void CGameClientView::DrawNumber( CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_LEFT*/)
{
	// ������Դ
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pD3DDevice, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}

//�ɻ�����
VOID CGameClientView::ShowCartoonPlan(bool bCartoon)
{
	if (bCartoon==true)
	{
		//���ñ���
		m_bCartoonPlan=true;
		m_wCartoonPlanIndex=0L;

		//����ʱ��
		m_CartoonPlanLapseCount.Initialization();

		//������Դ
		if (m_TextureCartoonPlan.IsNull()==true)
		{
			m_TextureCartoonPlan.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("CARTOON_PLAN"),TEXT("PNG"));
		}

		//���ڴ�С
		CRect rcClient;
		GetClientRect(&rcClient);

		//����λ��
		m_ptCartoonPlan.SetPoint( rcClient.Width() , rcClient.Height()/2 );

		//���½���
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}
	else
	{
		//ֹͣ����
		if (m_bCartoonPlan==true)
		{
			//���ñ���
			m_bCartoonPlan=false;
			m_wCartoonPlanIndex=0L;

			//����λ��
			m_ptCartoonPlan.SetPoint( 0, 0 );

			//ж����Դ
			ASSERT(m_TextureCartoonPlan.IsNull()==false);
			if (m_TextureCartoonPlan.IsNull()==false) m_TextureCartoonPlan.Destory();

			//���½���
			CRect rcClient;
			GetClientRect(&rcClient);
			InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
		}
	}

	return;
}

//��ը����
VOID CGameClientView::ShowCartoonBomb(bool bCartoon)
{
	if (bCartoon==true)
	{
		//���ñ���
		m_bCartoonBomb=true;
		m_wCartoonBombIndex=0L;

		//����ʱ��
		m_CartoonBombLapseCount.Initialization();

		//������Դ
		if (m_TextureCartoonBomb.IsNull()==true)
		{
			m_TextureCartoonBomb.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("CARTOON_BOMB"),TEXT("PNG"));
		}

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}
	else
	{
		//ֹͣ����
		if (m_bCartoonBomb==true)
		{
			//���ñ���
			m_bCartoonBomb=false;
			m_wCartoonBombIndex=0L;

			//ж����Դ
			ASSERT(m_TextureCartoonBomb.IsNull()==false);
			if (m_TextureCartoonBomb.IsNull()==false) m_TextureCartoonBomb.Destory();

			//���½���
			CRect rcClient;
			GetClientRect(&rcClient);
			InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
		}
	}

	return;
}

//�������
VOID CGameClientView::ShowCartoonRocket(bool bCartoon)
{
	if (bCartoon==true)
	{
		//���ñ���
		m_bCartoonRocket=true;
		m_wCartoonRocketIndex=0L;

		//����ʱ��
		m_CartoonRocketLapseCount.Initialization();

		//������Դ
		if (m_TextureCartoonRocket.IsNull()==true)
		{
			m_TextureCartoonRocket.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("ROCKET"),TEXT("PNG"));
		}

		//���ڴ�С
		CRect rcClient;
		GetClientRect(&rcClient);

		//����λ��
		m_ptCartoonRocket.SetPoint( rcClient.Width() /2 , rcClient.Height() );

		//���½���
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}
	else
	{
		//ֹͣ����
		if (m_bCartoonRocket==true)
		{
			//���ñ���
			m_bCartoonRocket=false;
			m_wCartoonRocketIndex=0L;

			//����λ��
			m_ptCartoonRocket.SetPoint( 0, 0 );

			//ж����Դ
			ASSERT(m_TextureCartoonRocket.IsNull()==false);
			if (m_TextureCartoonRocket.IsNull()==false) m_TextureCartoonRocket.Destory();

			//���½���
			CRect rcClient;
			GetClientRect(&rcClient);
			InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
		}
	}

	return;
}

//��ʾ����
VOID CGameClientView::ShowNoteCard(bool bShow)
{
	if(!bShow)
	{
		if(m_NoteCard.IsWindowActive())
		{
			m_NoteCard.ShowWindow(false);
		}
		m_bShowNoteCard = false;
	}
	m_btNoteCard.ShowWindow(bShow);
}

//��������
VOID CGameClientView::SetLastTurnState(bool bLastTurnState)
{
	if (m_bLastTurnState!=bLastTurnState)
	{
		//���ñ���
		m_bLastTurnState=bLastTurnState;

		//��Դ����
		if (m_bLastTurnState==true)
		{
			ASSERT(m_TextureLastTurn.IsNull()==true);
			if (m_TextureLastTurn.IsNull()==true) m_TextureLastTurn.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("LAST_TURN"),TEXT("PNG"));
		}
		else
		{
			ASSERT(m_TextureLastTurn.IsNull()==false);
			if (m_TextureLastTurn.IsNull()==false) m_TextureLastTurn.Destory();
		}

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	return;
}

//�ȴ��з�
VOID CGameClientView::SetWaitCallScore(bool bWaitCallScore)
{
	if (m_bWaitCallScore!=bWaitCallScore)
	{
		//���ñ���
		m_bWaitCallScore=bWaitCallScore;

		//��Դ����
		if (m_bWaitCallScore==true)
		{
			ASSERT(m_TextureWaitCallScore.IsNull()==true);
			if (m_TextureWaitCallScore.IsNull()==true) m_TextureWaitCallScore.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("WAIT_CALL_SCORE"),TEXT("PNG"));
		}
		else
		{
			ASSERT(m_TextureWaitCallScore.IsNull()==false);
			if (m_TextureWaitCallScore.IsNull()==false) m_TextureWaitCallScore.Destory();
		}

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	return;
}

//���÷���
VOID CGameClientView::SetUserPassState(WORD wChairID, bool bUserPass)
{
	//��������
	bool bLoadImage=false;

	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		//���ñ���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_bUserPass[i]=bUserPass;
		}
	}
	else
	{
		//���ñ���
		ASSERT(wChairID<GAME_PLAYER);
		m_bUserPass[wChairID]=bUserPass;
	}

	//״̬���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bUserPass[i]==true)
		{
			bLoadImage=true;
			break;
		}
	}

	//��Դ����
	if (bLoadImage==true)
	{
		if (m_TextureUserPass.IsNull()==true)
		{
			m_TextureUserPass.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("USER_PASS"),TEXT("PNG"));
		}
	}
	else
	{
		if (m_TextureUserPass.IsNull()==false)
		{
			m_TextureUserPass.Destory();
		}
	}

	//���½���
	CRect rcClient;
	GetClientRect(&rcClient);
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//���þ���
VOID CGameClientView::SetUserCountWarn(WORD wChairID, bool bCountWarn)
{
	//��������
	bool bLoadImage=false;

	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		//���ñ���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_bCountWarn[i]=bCountWarn;
		}
	}
	else
	{
		//���ñ���
		ASSERT(wChairID<GAME_PLAYER);
		m_bCountWarn[wChairID]=bCountWarn;
	}

	//״̬���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bCountWarn[i]==true)
		{
			bLoadImage=true;
			break;
		}
	}

	//��Դ����
	if (bLoadImage==true)
	{
		if (m_TextureCountWarn.IsNull()==true)
		{
			m_TextureCountWarn.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("COUNT_WARN"),TEXT("PNG"));
		}
	}
	else
	{
		if (m_TextureCountWarn.IsNull()==false)
		{
			m_TextureCountWarn.Destory();
		}
	}

	//���½���
	CRect rcClient;
	GetClientRect(&rcClient);
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//���ýз�
VOID CGameClientView::SetUserCallScore(WORD wChairID, BYTE cbCallScore)
{
	//��������
	bool bLoadImage=false;

	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		//���ñ���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_cbCallScore[i]=cbCallScore;
		}
	}
	else
	{
		//���ñ���
		ASSERT(wChairID<GAME_PLAYER);
		m_cbCallScore[wChairID]=cbCallScore;
	}

	//״̬���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbCallScore[i]!=0)
		{
			bLoadImage=true;
			break;
		}
	}

	//��Դ����
	if (bLoadImage==true)
	{
		if (m_TextureCallScore.IsNull()==true)
		{
			m_TextureCallScore.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("CALL_SCORE"),TEXT("PNG"));
		}
	}
	else
	{
		if (m_TextureCallScore.IsNull()==false)
		{
			m_TextureCallScore.Destory();
		}
	}

	//���½���
	CRect rcClient;
	GetClientRect(&rcClient);
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//���û���
VOID CGameClientView::SetHistoryScore(WORD wChairID, tagHistoryScore * pHistoryScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	m_pHistoryScore[wChairID]=pHistoryScore;

	//���½���
	CRect rcClient;
	GetClientRect(&rcClient);
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//���ƶ���
VOID CGameClientView::StopDispatchCard()
{
	//ֹͣ����
	if ((m_bRiffleCard==true)||(m_bDispatchCard==true))
	{
		//���ñ���
		m_bRiffleCard=false;
		m_bDispatchCard=false;
		m_wDispatchCardIndex=0L;

		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i==MYSELF_VIEW_ID || m_bUserCheat[i])
			{
				m_HandCardControl[i].SetCardData(m_cbHandCardData[i],CountArray(m_cbHandCardData[i]));
			}
			else
			{
				m_HandCardControl[i].SetCardData(NORMAL_COUNT);
			}
		}

		//ж����Դ
		ASSERT(m_TextureDispatchCard.IsNull()==false);
		if (m_TextureDispatchCard.IsNull()==false) m_TextureDispatchCard.Destory();

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	return;
}

//���ƶ���
VOID CGameClientView::ShowDispatchCard(BYTE cbCardData[NORMAL_COUNT], WORD wStartUser)
{
	//���ñ���
	m_bRiffleCard=true;
	m_bDispatchCard=false;
	m_wDispatchCardIndex=0L;

	//���ñ���
	m_wStartUser=wStartUser;
	CopyMemory(m_cbHandCardData[MYSELF_VIEW_ID],cbCardData,sizeof(BYTE)*NORMAL_COUNT);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HandCardControl[i].SetCardData(0);
	}

	//����ʱ��
	m_DispatchCardLapseCount.Initialization();

	//������Դ
	ASSERT(m_TextureDispatchCard.IsNull()==true);
	if (m_TextureDispatchCard.IsNull()==true) m_TextureDispatchCard.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"),TEXT("PNG"));

	//���½���
	CRect rcClient;
	GetClientRect(&rcClient);
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("REVERSAL_CARD"));

	return;
}

//��ʼ��ť
VOID CGameClientView::OnBnClickedStart()
{
	//������Ϣ
	SendEngineMessage(IDM_START,0,0);

	return;
}

//���ְ�ť
VOID CGameClientView::OnBnClickedScore()
{
	//���ñ���
	m_bShowScore=!m_bShowScore;

	//������Դ
	if (m_bShowScore==true)
	{
		ASSERT(m_TextureHistoryScore.IsNull()==true);
		m_TextureHistoryScore.LoadImage(&m_D3DDevice,AfxGetInstanceHandle(),TEXT("HISTORY_SCORE"),TEXT("PNG"));
	}
	else
	{
		ASSERT(m_TextureHistoryScore.IsNull()==false);
		if (m_TextureHistoryScore.IsNull()==false) m_TextureHistoryScore.Destory();
	}

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage(&m_D3DDevice,(m_bShowScore==true)?TEXT("BT_CLOSE_SCORE"):TEXT("BT_SHOW_SCORE"),TEXT("PNG"),hResInstance);

	return;
}

//���ư�ť
VOID CGameClientView::OnBnClickedOutCard()
{
	//������Ϣ
	SendEngineMessage(IDM_OUT_CARD,1,0);

	return;
}

//����ť
VOID CGameClientView::OnBnClickedSortCard()
{
	//������Ϣ
	SendEngineMessage(IDM_SORT_HAND_CARD,0,0);

	return;
}

//���ְ�ť
VOID CGameClientView::OnBnClickedLastTurn()
{
	//������Ϣ
	SendEngineMessage(IDM_LAST_TURN_CARD,0,0);

	return;
}

//�йܰ�ť
VOID CGameClientView::OnBnClickedTrustee()
{
	//������Ϣ
	SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);

	return;
}

//�йܰ�ť
VOID CGameClientView::OnBnClickedCancelTrustee()
{
	//������Ϣ
	SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);

	return;
}

//PASS��ť
VOID CGameClientView::OnBnClickedPassCard()
{
	//������Ϣ
	SendEngineMessage(IDM_PASS_CARD,0,0);

	return;
}

//��ʾ��ť
VOID CGameClientView::OnBnClickedOutPrompt()
{
	//������Ϣ
	SendEngineMessage(IDM_OUT_PROMPT,0,0);

	return;
}

//���ư�ť
VOID CGameClientView::OnBnClickedNoteCard()
{
	
	//�����ؼ�
	if (m_NoteCard.IsWindowActive()==false)
	{
		
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//��������
		CRect rcCreate(0,0,0,0);
		m_NoteCard.ActiveWindow(rcCreate,WS_VISIBLE,IDC_NOTE_CARD,&m_VirtualEngine,NULL);

		//�ƶ�λ��
		CRect rcBtNoteCard;
		CRect rcNoteCard;
		m_btNoteCard.GetWindowRect(rcBtNoteCard);
		m_NoteCard.GetWindowRect(rcNoteCard);
		m_NoteCard.SetWindowPos(rcBtNoteCard.left - 35, rcBtNoteCard.top - rcNoteCard.Height() - 6,0,0,SWP_NOSIZE);
	}

	m_bShowNoteCard = !m_bShowNoteCard;
	m_NoteCard.ShowWindow(m_bShowNoteCard);
}

//һ�ְ�ť
VOID CGameClientView::OnBnClickedCallScore1()
{
	//������Ϣ
	SendEngineMessage(IDM_CALL_SCORE,1,1);

	return;
}

//���ְ�ť
VOID  CGameClientView::OnBnClickedCallScore2()
{
	//������Ϣ
	SendEngineMessage(IDM_CALL_SCORE,2,2);

	return;
}

//���ְ�ť
VOID  CGameClientView::OnBnClickedCallScore3()
{
	//������Ϣ
	SendEngineMessage(IDM_CALL_SCORE,3,3);

	return;
}

//������ť
VOID  CGameClientView::OnBnClickedCallScoreNone()
{
	//������Ϣ
	SendEngineMessage(IDM_CALL_SCORE,0xFF,0xFF);

	return;
}

//�����Ϣ
VOID CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//���ͳ���
	if ( m_HandCardControl[MYSELF_VIEW_ID].GetPositively() )
	{
		SendEngineMessage(IDM_OUT_CARD,1,1);
	}

	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//��ȡ���������˿�
	BYTE cbOldShootCard[MAX_CARD_COUNT];
	BYTE cbOldShootCrad = m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbOldShootCard, MAX_CARD_COUNT);

	//Ĭ�ϴ���
	if (m_bMoveMouse==false)
	{
		//��ȡ�˿�
		tagCardItem * pCardItem=NULL;
		WORD wMouseDownItem=m_HandCardControl[MYSELF_VIEW_ID].SwitchCardPoint(Point);
		if (wMouseDownItem==m_cbMouseDownItem) pCardItem=m_HandCardControl[MYSELF_VIEW_ID].GetCardFromPoint(Point);

		//�����˿�
		if (pCardItem!=NULL)
		{
			//�����˿�
			pCardItem->bShoot=!pCardItem->bShoot;

			//������Ϣ
			SendEngineMessage(IDM_LEFT_HIT_CARD,0,0);

			//���½���
			CRect rcClient;
			GetClientRect(&rcClient);
			InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
		}
	}

	//ѡ����
	if ((m_bSelectCard==true)&&(m_bMoveMouse==true))
	{
		//�����˿�
		if (m_cbHoverCardItem>m_cbMouseDownItem)
		{
			m_HandCardControl[MYSELF_VIEW_ID].SetShootArea(m_cbMouseDownItem,m_cbHoverCardItem);
		}
		else
		{
			m_HandCardControl[MYSELF_VIEW_ID].SetShootArea(m_cbHoverCardItem,m_cbMouseDownItem);
		}

		//������Ϣ
		SendEngineMessage(IDM_LEFT_HIT_CARD,0,0);

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	//��������
	if ((m_bSwitchCard==true)&&(m_bMoveMouse==true))
	{
		//�����˿�
		m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

		//������Ϣ
		SendEngineMessage(IDM_LEFT_HIT_CARD,0,0);
		SendEngineMessage(IDM_SORT_HAND_CARD,ST_CUSTOM,ST_CUSTOM);

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	//��ȡ�˿�
	BYTE cbShootCard[MAX_CARD_COUNT];
	ZeroMemory(cbShootCard, sizeof(cbShootCard));
	BYTE cbShootCrad = m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbShootCard, MAX_CARD_COUNT);

	//����˳��
	if ( cbOldShootCrad == 1 && cbShootCrad == 2 && !m_bSwitchCard && !m_bMoveMouse &&m_HandCardControl[MYSELF_VIEW_ID].SetShootOrderCard(cbShootCard[0], cbShootCard[1]) )
	{
		//������Ϣ
		SendEngineMessage(IDM_LEFT_HIT_CARD,0,0);

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	//�ͷ����
	ReleaseCapture();


	//���ñ���
	m_bMoveMouse=false;
	m_bSwitchCard=false;
	m_bSelectCard=false;

	//��������
	m_cbMouseDownItem=INVALID_ITEM;
	m_cbHoverCardItem=INVALID_ITEM;

	return;
}

//�����Ϣ
VOID CGameClientView::OnMouseMove(UINT nFlags, CPoint MousePoint)
{
	__super::OnMouseMove(nFlags, MousePoint);

	//��������
	BYTE cbHoverCardItem=INVALID_ITEM;

	//�ƶ��ж�
	if (m_cbHoverCardItem!=INVALID_ITEM)
	{
		//�˿˴�С
		CSize ControlSize;
		m_HandCardControl[MYSELF_VIEW_ID].GetControlSize(ControlSize);

		//�˿�λ��
		CPoint OriginPoint;
		m_HandCardControl[MYSELF_VIEW_ID].GetOriginPoint(OriginPoint);

		//���е���
		if (MousePoint.x<OriginPoint.x) MousePoint.x=OriginPoint.x;
		if (MousePoint.x>(OriginPoint.x+ControlSize.cx)) MousePoint.x=(OriginPoint.x+ControlSize.cx);

		//��ȡ����
		MousePoint.y=OriginPoint.y+DEF_SHOOT_DISTANCE;
		cbHoverCardItem=m_HandCardControl[MYSELF_VIEW_ID].SwitchCardPoint(MousePoint);

		//�ƶ�����
		if (cbHoverCardItem!=m_cbHoverCardItem) m_bMoveMouse=true;
	}

	////�����˿�
	//if ((m_bSwitchCard==true)&&(cbHoverCardItem!=m_cbHoverCardItem))
	//{
	//	//Ŀ���ж�
	//	BYTE cbTargetCardItem=INVALID_ITEM;
	//	cbTargetCardItem=(m_cbSwitchCardItem>cbHoverCardItem)?0:cbHoverCardItem-m_cbSwitchCardItem;

	//	//�����˿�
	//	m_HandCardControl[MYSELF_VIEW_ID].MoveCardItem(cbTargetCardItem);

	//	//���½���
	//	CRect rcClient;
	//	GetClientRect(&rcClient);
	//	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	//}

	//ѡ���˿�
	if ((m_bSelectCard==true)&&(cbHoverCardItem!=m_cbHoverCardItem))
	{
		//�����˿�
		if (cbHoverCardItem>m_cbMouseDownItem)
		{
			m_HandCardControl[MYSELF_VIEW_ID].SetSelectIndex(m_cbMouseDownItem,cbHoverCardItem);
		}
		else
		{
			m_HandCardControl[MYSELF_VIEW_ID].SetSelectIndex(cbHoverCardItem,m_cbMouseDownItem);
		}

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	//���ñ���
	m_cbHoverCardItem=cbHoverCardItem;

	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);


	//״̬�ж�
	if (m_HandCardControl[MYSELF_VIEW_ID].GetPositively()==true)
	{
		//��ȡ�˿�
		m_cbMouseDownItem=m_HandCardControl[MYSELF_VIEW_ID].SwitchCardPoint(Point);

		//�����˿�
		if (m_cbMouseDownItem!=INVALID_ITEM)
		{
			//��ȡ�˿�
			tagCardItem * pCardItem=m_HandCardControl[MYSELF_VIEW_ID].GetCardFromIndex(m_cbMouseDownItem);

			//���ñ���
			m_bMoveMouse=false;
			m_cbHoverCardItem=m_cbMouseDownItem;

			//��������
			if (pCardItem->bShoot==true)
			{
				//���ñ���
				m_bSwitchCard=true;
				m_cbSwitchCardItem=0;

				//����״̬
				for (BYTE i=0;i<m_cbMouseDownItem;i++)
				{
					tagCardItem * pCardItemTemp=m_HandCardControl[MYSELF_VIEW_ID].GetCardFromIndex(i);
					if ((pCardItemTemp!=NULL)&&(pCardItemTemp->bShoot==true)) m_cbSwitchCardItem++;
				}
			}
			else
			{
				m_bSelectCard=true;
			}

			//�������
			SetCapture();
		}
	}

	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags, Point);

	//��ȡ�˿�
	tagCardItem * pCardItem = m_HandCardControl[MYSELF_VIEW_ID].GetCardFromPoint(Point);

	//�����˿�
	if ( pCardItem == NULL )
	{
		//��ֹ��ť
		m_btOutCard.EnableWindow(FALSE);

		//�����˿�
		m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}
	else if ( m_HandCardControl[MYSELF_VIEW_ID].GetPositively()  )
	{
		//�����˿�
		m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

		//����ͬ��
		m_HandCardControl[MYSELF_VIEW_ID].SetShootSameCard(pCardItem->cbCardData);

		//������Ϣ
		SendEngineMessage(IDM_LEFT_HIT_CARD,0,0);

		//���½���
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˲���
	if (m_HandCardControl[MYSELF_VIEW_ID].OnEventSetCursor(MousePoint)==true)
	{
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//��Ϣ����
BOOL CGameClientView::OnCommand( WPARAM wParam, LPARAM lParam )
{
	switch( LOWORD(wParam) )
	{
	case IDC_CARD_PROMPT_1:
	case IDC_CARD_PROMPT_2:
	case IDC_CARD_PROMPT_3:	
	case IDC_CARD_PROMPT_4:
	case IDC_CARD_PROMPT_5:	
	case IDC_CARD_PROMPT_6:
		{
			//������Ϣ
			SendEngineMessage(IDM_SEARCH_CARD,LOWORD(wParam)-IDC_CARD_PROMPT_1,0);

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}



//�û�����
VOID CGameClientView::UserValidCard(WORD wChairID, BYTE cbCardData[MAX_COUNT], BYTE cbCardCount/* = 17*/)
{
	ASSERT(wChairID >=0 && wChairID < GAME_PLAYER);

	m_HandCardControl[wChairID].SetDisplayItem(true);
	CopyMemory(m_cbHandCardData[wChairID], cbCardData, sizeof(BYTE)*cbCardCount);
	m_bUserCheat[wChairID] = true;
	if (m_HandCardControl[wChairID].GetCardCount() > 0)
	{
		m_HandCardControl[wChairID].SetCardData(cbCardData, m_HandCardControl[wChairID].GetCardCount());
	}
	else
	{
		m_HandCardControl[wChairID].SetCardData(0);
	}
}

//////////////////////////////////////////////////////////////////////////////////
