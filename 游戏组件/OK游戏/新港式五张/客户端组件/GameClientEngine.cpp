#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸʱ��
#define IDI_ADD_SCORE				200									//��ע��ʱ��
#define IDI_START_GAME				201									//��ʼ��ʱ��

//��Ϸʱ��
#define TIME_ADD_SCORE				30									//��ע��ʱ��
#define TIME_START_GAME				15									//��ʼ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_START,OnMessageStart)
	ON_MESSAGE(IDM_FOLLOW,OnMessageFollow)
	ON_MESSAGE(IDM_GIVE_UP,OnMessageGiveUp)
	ON_MESSAGE(IDM_ADD_SCORE,OnMessageAddScore)
	ON_MESSAGE(IDM_SHOW_HAND,OnMessageShowHand)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnMessageSendCardFinish)

	ON_MESSAGE(IDM_ADMIN_COMMDN,OnAdminCommand)
	ON_MESSAGE(IDM_ADMIN_QUERYUSER,OnAdminQueryUser)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��ע��Ϣ
	m_lDrawMaxScore=SCORE_ZERO;
	m_lTurnMaxScore=SCORE_ZERO;
	m_lTurnLessScore=SCORE_ZERO;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//��������
	ZeroMemory(&m_GameEndPacket,sizeof(m_GameEndPacket));

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	if (m_pIClientKernel!=NULL) m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	return true;
}

//��������
bool CGameClientEngine::OnResetGameEngine()
{
	//��ע��Ϣ
	m_lDrawMaxScore=SCORE_ZERO;
	m_lTurnMaxScore=SCORE_ZERO;
	m_lTurnLessScore=SCORE_ZERO;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//��������
	ZeroMemory(&m_GameEndPacket,sizeof(m_GameEndPacket));

	return true;
}


//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_START_GAME:	//��Ϸ��ʼ
		{
			//�ر���Ϸ
			if ((nElapse==0)&&(IsLookonMode()==false)&&(wChairID==GetMeChairID()))
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_ADD_SCORE:		//�û���ע
		{
			//�Զ�����
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnMessageGiveUp(0,0);
				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}

	return false;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//�����˿�
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubGameEnd(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//���ý���
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);

				//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
			{

				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
			//�������
			IClientUserItem * pIClientUserItem=GetMeUserItem();
			if ((IsLookonMode()==false)&&(pIClientUserItem->GetUserStatus()!=US_READY))
			{
				PerformAutoStart();
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.EnableWindow();
			}

			return true;
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��������
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//��ע��Ϣ
			m_lCellScore = pStatusPlay->lCellScore;
			m_lDrawMaxScore=pStatusPlay->lDrawMaxScore;
			m_lTurnMaxScore=pStatusPlay->lTurnMaxScore;
			m_lTurnLessScore=pStatusPlay->lTurnLessScore;
			CopyMemory(m_lUserScore,pStatusPlay->lUserScore,sizeof(m_lUserScore));
			CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(m_lTableScore));

			//״̬��Ϣ
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_bShowHand=(pStatusPlay->cbShowHand==TRUE)?true:false;
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(m_cbPlayStatus));

			//�ʺ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IClientUserItem * pIClientUserItem=GetTableUserItem(i);
				if (pIClientUserItem!=NULL) lstrcpyn(m_szAccounts[i],pIClientUserItem->GetNickName(),CountArray(m_szAccounts[i]));
			}

			//�Թۿ���
			if ((IsLookonMode()==false)||(IsAllowLookon()==true))
			{
				m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(true);
			}

			//���ý���
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetDrawMaxScore(pStatusPlay->lDrawMaxScore);
				//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
			{

				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(i);
				IClientUserItem * pIClientUserItem=GetTableUserItem(i);

				//���û���
				if (pIClientUserItem!=NULL)
				{
					if (m_cbPlayStatus[i]==TRUE)
					{
						SCORE lUserScore=pIClientUserItem->GetUserScore();
						m_GameClientView.SetUserScore(wViewChairID,lUserScore - m_lUserScore[i] - m_lTableScore[i]);
					}
					else
					{
						m_GameClientView.SetUserScore(wViewChairID,pIClientUserItem->GetUserScore());
					}
				}

				//�����˿�
				if (m_lTableScore[i]>0.001f||m_lTableScore[i]<-0.001f)
				{
					if (m_cbPlayStatus[i]==FALSE)
					{
						BYTE cbCardData[MAX_COUNT];
						ZeroMemory(cbCardData,sizeof(cbCardData));
						m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,pStatusPlay->cbCardCount[i]);
						m_GameClientView.SetUserCard(wViewChairID,NULL,0);
					}
					else
					{
						BYTE cbCardCount=pStatusPlay->cbCardCount[i];
						m_GameClientView.m_CardControl[wViewChairID].SetCardData(pStatusPlay->cbHandCardData[i],cbCardCount);
						m_GameClientView.SetUserCard(wViewChairID,pStatusPlay->cbHandCardData[i],cbCardCount);
					}
				}

				//��������
				m_GameClientView.m_lPalyBetChip[wViewChairID] = m_lUserScore[i];
				m_GameClientView.m_lALLBetChip += m_lTableScore[i];
				m_GameClientView.SetUserTableScore(wViewChairID,m_lUserScore[i]+m_lTableScore[i]);
			}

			m_GameClientView.AddDeskChip( m_GameClientView.m_lALLBetChip, GAME_PLAYER );

			//��ǰ�û�
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID())) UpdateScoreControl();

			//����ʱ��
			SetGameClock(m_wCurrentUser,IDI_ADD_SCORE,TIME_ADD_SCORE);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//����״̬
	SetGameStatus(GAME_SCENE_PLAY);

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=pGameStart->wCurrentUser;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//���ñ���
	m_lDrawMaxScore=pGameStart->lDrawMaxScore;
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	m_lTurnLessScore=pGameStart->lTurnLessScore;


	//��������
	BYTE cbCardData[2][GAME_PLAYER];
	WORD wStartChairID=INVALID_CHAIR;
	ZeroMemory(cbCardData,sizeof(cbCardData));

	//ɾ������
	for ( int i = 0 ; i < m_GameClientView.m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayPlayChip[i]);
	m_GameClientView.m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_GameClientView.m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayBetChip[i]);
	m_GameClientView.m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_GameClientView.m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayOverChip[i]);
	m_GameClientView.m_ArrayOverChip.RemoveAll();

	//���ý���
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetDrawMaxScore(m_lDrawMaxScore);
	m_GameClientView.SetCellScore(pGameStart->lCellScore);
	m_lCellScore = pGameStart->lCellScore;

	//���ý���
	for (WORD i = 0;i < GAME_PLAYER; i++)
	{
		m_GameClientView.SetOverScore(i, 0);
	}

	//���ý���
	m_GameClientView.m_lALLBetChip = SCORE_ZERO;

	CopyMemory(m_cbPlayStatus,pGameStart->cbPlayStatus,sizeof(m_cbPlayStatus));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		IClientUserItem * pIClientUserItem=GetTableUserItem(i);

		//�����˿�
		if (pGameStart->cbCardData[i]!=0x00 && m_cbPlayStatus[i]==TRUE)
		{
			cbCardData[1][wViewChairID]=pGameStart->cbCardData[i];
			cbCardData[0][wViewChairID]=(wViewChairID==MYSELF_VIEW_ID)?pGameStart->cbObscureCard:0xFF;
		}

		//�������
		if (pGameStart->cbCardData[i]!=0x00&& m_cbPlayStatus[i]==TRUE)
		{
			//�����û�
			if (wStartChairID==INVALID_CHAIR) wStartChairID=wViewChairID;

			//״̬����
			m_lTableScore[i]=pGameStart->lCellScore;
			lstrcpyn(m_szAccounts[i],pIClientUserItem->GetNickName(),CountArray(m_szAccounts[i]));

			//���ó���
			m_GameClientView.m_lPalyBetChip[wViewChairID] = SCORE_ZERO;
			m_GameClientView.m_lALLBetChip += pGameStart->lCellScore;
			m_GameClientView.SetUserTableScore(wViewChairID,pGameStart->lCellScore);

			//�����������
			m_GameClientView.AddPlayChip( pGameStart->lCellScore, wViewChairID );

			//���û���
			ASSERT(pIClientUserItem!=NULL);
			SCORE lUserScore= pIClientUserItem->GetUserScore();
			m_GameClientView.SetUserScore(wViewChairID,lUserScore - m_lTableScore[i] - m_lUserScore[1] );
		}
		else
		{
			//���û���
			m_GameClientView.SetUserScore(wViewChairID,SCORE_ZERO);

			//���ó���
			m_GameClientView.m_lPalyBetChip[wViewChairID] = SCORE_ZERO;
		}

		//�����˿�
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[i].SetDisplayHead(false);
	}

	//�ɷ��˿�
	for (BYTE cbIndex=0;cbIndex<2;cbIndex++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(i);
				BYTE cbCardData[2]={0,pGameStart->cbCardData[i]};

				//�ɷ��˿�
				cbCardData[0]=(GetMeChairID()==i)?pGameStart->cbObscureCard:0;
				m_GameClientView.DispatchUserCard(wViewChairID,cbCardData[cbIndex]);
			}
		}
	}

	//�������
	if (IsLookonMode()==false)
	{
		ActiveGameFrame();
		m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(true);
	}

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);
#ifndef DEBUG
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
#endif

	return true;
}

//�û�����
bool CGameClientEngine::OnSubGiveUp(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GiveUp));
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;

	//��������
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pData;

	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;
	WORD wViewChairID=SwitchViewChairID(wGiveUpUser);

	//��ɷ���
	m_GameClientView.ConcludeDispatch();

	//��������
	if ((IsLookonMode()==true)||(wViewChairID!=MYSELF_VIEW_ID))
	{
		//���ö���
		m_GameClientView.SetUserAction(wViewChairID,AC_GIVE_UP);

		//��������
		BYTE cbCardData[MAX_COUNT];
		WORD wCardCount=m_GameClientView.m_CardControl[wViewChairID].GetCardCount();

		//�����˿�
		ZeroMemory(cbCardData,sizeof(cbCardData));
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(false);
		m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,wCardCount);

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	//��ע����
	m_lDrawMaxScore=pGiveUp->lDrawMaxScore;
	m_lTurnMaxScore=pGiveUp->lTrunMaxScore;

	//״̬����
	m_cbPlayStatus[wGiveUpUser]=FALSE;

	//���ý���
	m_GameClientView.SetDrawMaxScore(m_lDrawMaxScore);

	//�����û�
	m_wCurrentUser=pGiveUp->wCurrentUser;

	//��ע����
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID())) 
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	if (GetClockChairID()!=m_wCurrentUser) SetGameClock(m_wCurrentUser,IDI_ADD_SCORE,TIME_ADD_SCORE);

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubAddScore(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;

	//��������
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pData;

	//��������
	WORD wAddScoreUser=pAddScore->wAddScoreUser;
	WORD wViewChairID=SwitchViewChairID(wAddScoreUser);

	//��ɷ���
	m_GameClientView.ConcludeDispatch();

	//��ע����
	if ((IsLookonMode()==true)||(wViewChairID!=MYSELF_VIEW_ID))
	{
		//��������
		SCORE lCurrentScore=pAddScore->lUserScoreCount;
		SCORE lDrawAddScore=m_lTableScore[wAddScoreUser];
		IClientUserItem * pIClientUserItem=GetTableUserItem(wAddScoreUser);

		//��������
		m_lUserScore[wAddScoreUser]=lCurrentScore;
		m_GameClientView.AddPlayBetChip(m_lUserScore[wAddScoreUser], wViewChairID);
		m_GameClientView.SetUserTableScore(wViewChairID,m_lUserScore[wAddScoreUser]+m_lTableScore[wAddScoreUser]);

		//���û���
		if (pIClientUserItem!=NULL)
		{
			SCORE lUserDrawScore=m_lUserScore[wAddScoreUser] + m_lTableScore[wAddScoreUser] ;
			m_GameClientView.SetUserScore(wViewChairID, pIClientUserItem->GetUserScore()-lUserDrawScore);
		}

		//����ж�
		if ((lCurrentScore+lDrawAddScore)==m_lDrawMaxScore)
		{
			//���ö���
			m_bShowHand=true;
			m_GameClientView.SetUserAction(wViewChairID,AC_SHOW_HAND);

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
		}
		else
		{
			if (lCurrentScore<=0.001f&&lCurrentScore>=-0.001f)
			{
				//���Ӷ���
				PlayGameSound(AfxGetInstanceHandle(),TEXT("NO_ADD"));
				m_GameClientView.SetUserAction(wViewChairID,AC_NO_ADD);
			}
			else if ((lCurrentScore+lDrawAddScore)==m_lTurnLessScore)
			{
				//��ע����
				PlayGameSound(AfxGetInstanceHandle(),TEXT("FOLLOW"));
				m_GameClientView.SetUserAction(wViewChairID,AC_FOLLOW);
			}
			else
			{
				//��ע����
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
				m_GameClientView.SetUserAction(wViewChairID,AC_ADD_SCORE);
			}
		}

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	//���ñ���
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	SetGameClock(m_wCurrentUser,IDI_ADD_SCORE,TIME_ADD_SCORE);

	return true;
}

//�����˿�
bool CGameClientEngine::OnSubSendCard(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pData;

	//���ñ���
	m_wCurrentUser=pSendCard->wCurrentUser;
	m_lTurnMaxScore=pSendCard->lTurnMaxScore;

	//�㼯���
	CollectionGold();

	//�ɷ��˿�
	for (BYTE i=0;i<pSendCard->cbSendCardCount;i++)
	{
		for (WORD j=0;j<GAME_PLAYER;j++)
		{
			WORD wChairId = (pSendCard->wStartChairID+j)%GAME_PLAYER;
			if (m_cbPlayStatus[wChairId]==TRUE)
			{
				WORD wViewChairID=SwitchViewChairID(wChairId);
				m_GameClientView.DispatchUserCard(wViewChairID,pSendCard->cbCardData[i][wChairId]);
			}
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��������
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//��������
	KillGameClock(IDI_ADD_SCORE);
	SetGameStatus(GAME_SCENE_FREE);

	//�㼯���
	CollectionGold();

	//��������
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,SCORE_ZERO);

	//��������
	CopyMemory(&m_GameEndPacket,pGameEnd,sizeof(CMD_S_GameEnd));

	//������Ϸ
	PerformGameConclude();

	return true;
}

//�Զ���ʼ
bool CGameClientEngine::PerformAutoStart()
{
	//��ʼ����
	WORD wMeChairID=GetMeChairID();
	SetGameClock(wMeChairID,IDI_START_GAME,TIME_START_GAME);

	return true;
}

//���ؿ���
VOID CGameClientEngine::HideScoreControl()
{
	//���ؿؼ�
	m_GameClientView.m_PlayOperate.ShowOperate(false);

	//���ÿؼ�
	m_GameClientView.m_PlayOperate.m_btFollow.EnableWindow(FALSE);
	m_GameClientView.m_PlayOperate.m_btAddScore.EnableWindow(FALSE);
	m_GameClientView.m_PlayOperate.m_btShowHand.EnableWindow(FALSE);

	m_GameClientView.InvalidGameView(0,0,0,0);

	return;
}

//���¿���
VOID CGameClientEngine::UpdateScoreControl()
{
	//��������
	WORD wMeChairID=GetMeChairID();
	SCORE lDrawUserScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];

	//��ǰ�û�
	if ((m_wCurrentUser==wMeChairID)&&(m_cbPlayStatus[wMeChairID]==TRUE))
	{
		// ��С��ע
		SCORE lMinBet = (abs(m_lTurnLessScore-lDrawUserScore)<0.001f)?m_lCellScore:SCORE_ZERO;

		//��ʾ��ť
		m_GameClientView.m_PlayOperate.ShowOperate(true);
		//����?
		m_GameClientView.m_PlayOperate.SetBetCount( m_lTurnMaxScore - m_lTurnLessScore, lMinBet );

		m_GameClientView.m_PlayOperate.m_btFollow.ShowWindow(SW_SHOW);
		m_GameClientView.m_PlayOperate.m_btGiveUp.ShowWindow(SW_SHOW);
		m_GameClientView.m_PlayOperate.m_btAddScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_PlayOperate.m_btShowHand.ShowWindow(SW_SHOW);

		//��־����
		bool bEnableShowHand=(abs(m_lDrawMaxScore-m_lTurnMaxScore)<0.001f);
		UINT uBtImageID=(abs(m_lTurnLessScore-lDrawUserScore)<0.001f)?IDB_BT_CONTROL_NOADD:IDB_BT_CONTROL_FOLLOW;

		//���ư�ť//����?(������Ϊ0.075)
		m_GameClientView.m_PlayOperate.m_btFollow.EnableWindow((m_lTurnMaxScore-m_lTurnLessScore>=-0.08f)?TRUE:FALSE);
		m_GameClientView.m_PlayOperate.m_btShowHand.EnableWindow((bEnableShowHand==true)?TRUE:FALSE);
		m_GameClientView.m_PlayOperate.m_btAddScore.EnableWindow((m_lTurnMaxScore-m_lTurnLessScore>=-0.08f)?TRUE:FALSE);
		m_GameClientView.m_PlayOperate.m_btFollow.SetButtonImage(uBtImageID,AfxGetInstanceHandle(),false,false);
	}
	else
	{
		//���ؿؼ�
		m_GameClientView.m_PlayOperate.ShowOperate(false);

		//���ÿؼ�
		m_GameClientView.m_PlayOperate.m_btFollow.EnableWindow(FALSE);
		m_GameClientView.m_PlayOperate.m_btAddScore.EnableWindow(FALSE);
		m_GameClientView.m_PlayOperate.m_btShowHand.EnableWindow(FALSE);
	}

	m_GameClientView.InvalidGameView(0,0,0,0);

	return;
}

//ִ�н���
VOID CGameClientEngine::PerformGameConclude()
{
	//���ؿؼ�
	HideScoreControl();

	//�ɼ�����
	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		m_GameClientView.SetOverScore(SwitchViewChairID(i),  m_GameEndPacket.lGameScore[i]);
	}
	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		IClientUserItem * pIClientUserItem=GetTableUserItem(i);
		m_GameClientView.SetUserSaveScore(SwitchViewChairID(i),pIClientUserItem==NULL?0: pIClientUserItem->GetUserScore());
	}
	// ���ն���
	SCORE lMaxScore = SCORE_ZERO;
	WORD     wWinSite = INVALID_CHAIR;
	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		if (  m_GameEndPacket.lGameScore[i] > lMaxScore )
		{
			lMaxScore =  m_GameEndPacket.lGameScore[i];
			wWinSite = SwitchViewChairID(i);
		}
	}

	if( wWinSite != INVALID_CHAIR )
	{
		m_GameClientView.ChioRecovery(wWinSite);
	}
	

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		if (m_GameEndPacket.lGameScore[i]>0.001f||m_GameEndPacket.lGameScore[i]<-0.001f)
		{
			//�����ж�
			if (m_GameEndPacket.cbCardData[i]==0x00) continue;

			//��ȡ�˿�
			BYTE cbCardData[MAX_CHAIR];
			WORD wViewChairID=SwitchViewChairID(i);
			WORD wCardCount=m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

			//�����˿�
			cbCardData[0]=m_GameEndPacket.cbCardData[i];
			m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(true);
			m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,wCardCount);
		}
	}

	//���ÿؼ�
	m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(false);

	//��������
	SCORE lMeScore= m_GameEndPacket.lGameScore[GetMeChairID()];
	if (lMeScore>0.001f) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<-0.001f) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOSE"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//��ʼ����
	if ((IsLookonMode()==false)&&(GetMeChairID()!=INVALID_CHAIR))
	{
		PerformAutoStart();
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	}

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return;
}

//�㼯���
VOID CGameClientEngine::CollectionGold()
{
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//�������
		m_lTableScore[i]+=m_lUserScore[i];
		m_GameClientView.m_lALLBetChip += m_lUserScore[i];
		m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[i]);

		// �������
		m_GameClientView.AddDeskChip(m_lUserScore[i], wViewChairID);

		//�û�����
		m_lUserScore[i]=SCORE_ZERO;
		m_GameClientView.AddPlayBetChip(m_lUserScore[i], wViewChairID);
	}
}

//��ʼ��Ϣ
LRESULT CGameClientEngine::OnMessageStart(WPARAM wParam, LPARAM lParam)
{
	//��ע��Ϣ
	m_lDrawMaxScore=SCORE_ZERO;
	m_lTurnMaxScore=SCORE_ZERO;
	m_lTurnLessScore=SCORE_ZERO;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//ɾ��ʱ��
	KillGameClock(IDI_START_GAME);

	//��ɷ���
	m_GameClientView.ConcludeDispatch();

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_lPalyBetChip[i] = SCORE_ZERO;
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.SetOverScore(i, 0);
	}
	m_GameClientView.m_lALLBetChip = SCORE_ZERO;
	
	//ɾ������
	for ( int i = 0 ; i < m_GameClientView.m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayPlayChip[i]);
	m_GameClientView.m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_GameClientView.m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayBetChip[i]);
	m_GameClientView.m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_GameClientView.m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayOverChip[i]);
	m_GameClientView.m_ArrayOverChip.RemoveAll();

	//��������
	m_GameClientView.SetDrawMaxScore(SCORE_ZERO);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,SCORE_ZERO);
	m_GameClientView.SetUserCard(INVALID_CHAIR,NULL,0);
	//���ÿؼ�
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//������Ϣ
	m_pIClientKernel->SendUserReady(NULL,0);

	return 0;
}

//��ע��Ϣ
LRESULT CGameClientEngine::OnMessageFollow(WPARAM wParam, LPARAM lParam)
{
	//���ؿؼ�
	HideScoreControl();

	//ɾ��ʱ��
	KillGameClock(IDI_ADD_SCORE);

	//���ÿؼ�
	WORD wMeChairID=GetMeChairID();
	SCORE lScoreLess = m_lTurnLessScore - ( m_lTableScore[wMeChairID] + m_lUserScore[wMeChairID] );
	SCORE lXXParam = lScoreLess;
	if( wParam != 0 )
	{
		lXXParam = *((SCORE*)wParam) + lScoreLess;
	}

	//��Ҽ���
	SCORE lDrawAddScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];
	SCORE lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);

	//��������
	m_lUserScore[wMeChairID]+=lCurrentAddScore;
	m_GameClientView.AddPlayBetChip(m_lUserScore[wMeChairID], MYSELF_VIEW_ID);
	m_GameClientView.SetUserTableScore(MYSELF_VIEW_ID,m_lUserScore[wMeChairID]+m_lTableScore[wMeChairID]);

	//���û���
	IClientUserItem * pIClientUserItem=GetMeUserItem();
	SCORE lUserItemScore= pIClientUserItem->GetUserScore();
	m_GameClientView.SetUserScore(MYSELF_VIEW_ID,lUserItemScore-m_lUserScore[wMeChairID]-m_lTableScore[wMeChairID] );

	//����ж�
	if ((m_bShowHand==false)&&((lCurrentAddScore+lDrawAddScore)==m_lDrawMaxScore))
	{
		//���ö���
		m_bShowHand=true;
		m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_SHOW_HAND);

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
	}
	else
	{
		if (lCurrentAddScore<0.001f&&lCurrentAddScore>-0.001f)
		{
			//���Ӷ���
			PlayGameSound(AfxGetInstanceHandle(),TEXT("NO_ADD"));
			m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_NO_ADD);
		}
		else if (abs(lCurrentAddScore+lDrawAddScore-m_lTurnLessScore)<0.001f)
		{
			//��ע����
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FOLLOW"));
			m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_FOLLOW);
		}
		else
		{
			//��ע����
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
			m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_ADD_SCORE);
		}
	}

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentAddScore;

	SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0;
}

//������Ϣ		 
LRESULT CGameClientEngine::OnMessageGiveUp(WPARAM wParam, LPARAM lParam)
{
	//���ؿؼ�
	HideScoreControl();

	//��������
	KillGameClock(IDI_ADD_SCORE);

	//���ö���
	m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_GIVE_UP);

	//��������
	BYTE cbCardData[MAX_COUNT];
	WORD wCardCount=m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardCount();

	//�����˿�
	ZeroMemory(cbCardData,sizeof(cbCardData));
	m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetCardData(cbCardData,wCardCount);

	//���ÿؼ�
	m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(false);
	m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	//������Ϣ
	SendSocketData(SUB_C_GIVE_UP);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0;
}

//��ע��Ϣ		 
LRESULT CGameClientEngine::OnMessageAddScore(WPARAM wParam, LPARAM lParam)
{
	//���ð�ť
	m_GameClientView.m_PlayOperate.m_btFollow.EnableWindow(FALSE);
	m_GameClientView.m_PlayOperate.m_btAddScore.EnableWindow(FALSE);
	m_GameClientView.m_PlayOperate.m_btShowHand.EnableWindow(FALSE);

	return 0;
}

//�����Ϣ
LRESULT CGameClientEngine::OnMessageShowHand(WPARAM wParam, LPARAM lParam)
{
	//���ؿؼ�
	HideScoreControl();

	//��������
	KillGameClock(IDI_ADD_SCORE);

	//���ö���
	m_bShowHand=true;
	m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_SHOW_HAND);

	//��Ҽ���
	WORD wMeChairID=GetMeChairID();
	SCORE lCurrentAddScore=m_lDrawMaxScore-m_lTableScore[wMeChairID]-m_lUserScore[wMeChairID];

	//��������
	m_lUserScore[wMeChairID]+=lCurrentAddScore;
	m_GameClientView.AddPlayBetChip(m_lUserScore[wMeChairID], MYSELF_VIEW_ID);
	m_GameClientView.SetUserTableScore(MYSELF_VIEW_ID,m_lUserScore[wMeChairID]+m_lTableScore[wMeChairID]);

	//���û���
	IClientUserItem * pIClientUserItem=GetMeUserItem();
	SCORE lUserItemScore= pIClientUserItem->GetUserScore();
	m_GameClientView.SetUserScore(MYSELF_VIEW_ID,lUserItemScore-m_lUserScore[wMeChairID]-m_lTableScore[wMeChairID]);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentAddScore;
	SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0;
}

//�������
LRESULT CGameClientEngine::OnMessageSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_wCurrentUser==INVALID_CHAIR) return 0;

	//���ý���
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);

	//���ÿ���
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	SetGameClock(m_wCurrentUser,IDI_ADD_SCORE,TIME_ADD_SCORE);

	return 0;
}

LRESULT CGameClientEngine::OnAdminCommand(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_AMDIN_COMMAND,(CMD_C_AdminReq*)wParam,sizeof(CMD_C_AdminReq));
	return true;
}
LRESULT CGameClientEngine::OnAdminQueryUser(WPARAM wParam,LPARAM lParam)
{
	DWORD	m_wGameID[GAME_PLAYER];
	TCHAR	m_szAccounts1[GAME_PLAYER][LEN_ACCOUNTS];	//�������

	memset(m_wGameID,0,sizeof(m_wGameID));
	ZeroMemory(m_szAccounts1,sizeof(m_szAccounts1));

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pClientUserItem=GetTableUserItem(i);
		IClientUserItem * pClientUserItem1=GetTableUserItem(GetMeChairID());

		if (pClientUserItem==NULL) continue;

		//�û���Ϣ
	//	lstrcpyn(m_szAccounts1[i],pClientUserItem->GetNickName(),CountArray(m_szAccounts1[i]));
		if(pClientUserItem->GetUserID()!=pClientUserItem1->GetUserID())
			_sntprintf(m_szAccounts1[i],CountArray(m_szAccounts1[i]),TEXT("%d"),pClientUserItem->GetUserID());
		else
			lstrcpyn(m_szAccounts1[i],pClientUserItem->GetNickName(),CountArray(m_szAccounts1[i]));


		m_wGameID[i] = pClientUserItem->GetUserID();
	}
	m_GameClientView.m_AdminDlg.SetUserInfo(m_szAccounts1,m_wGameID);

	return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
