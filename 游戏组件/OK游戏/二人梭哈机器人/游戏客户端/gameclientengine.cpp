

//
#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include "CardExtractor.h"
//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_START_GAME					200								//��ʼ��ʱ��
#define IDI_GIVE_UP						201								//������ʱ��

void OutputDebugMsg(const char * szMsg, ...)
{
	char szData[1024]={0};
	va_list args;
	va_start(args, szMsg);
	//_vsntprintf_s(szData, sizeof(szData) - 1, szMsg, args);
	va_end(args);
	//OutputDebugString(szData);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine,CGameFrameEngine)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_FOLLOW,OnFollow)
	ON_MESSAGE(IDM_GIVE_UP,OnGiveUp)
	ON_MESSAGE(IDM_SHOWHAND,OnShowHand)
	ON_MESSAGE(IDM_ADD_GOLD,OnAddGold)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_USE_WINNER, OnUseWinner)
	ON_MESSAGE(IDM_SHOWCARD,OnShowCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	m_lBasicGold = 0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_lShowHandScore = 0L;
	ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );

	m_bPoor = true;
	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	memset(szName,0,sizeof(szName));
	ZeroMemory(&m_GameEnd,sizeof(m_GameEnd));

	m_bAddGold = false;

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//ȫ�ֶ���
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	ASSERT(m_pGlobalUnits!=NULL);

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//if(IsFreeze())
	//{
	//	OnCancel();
	//	return;
	//}

	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	m_lBasicGold = 0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = INVALID_CHAIR;
	m_lShowHandScore = 0L;
	ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );

	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	memset(szName,0,sizeof(szName));
	ZeroMemory(&m_GameEnd,sizeof(m_GameEnd));

	m_bAddGold = false;

	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
	//��������
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	CGameOption GameOption;
	GameOption.m_bEnableSound=m_pGlobalUnits->m_bAllowSound;
	GameOption.m_bAllowLookon = IsAllowLookon();

	////��������
	if (GameOption.DoModal()==IDOK)
	{
		//���ÿؼ�
		m_pGlobalUnits->m_bAllowSound = GameOption.m_bEnableSound;
		m_pGlobalUnits->m_bAllowLookon = GameOption.m_bAllowLookon;
	}

	return;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//������Ϸ��ʱ��
		{
			if (nElapse==0)
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
				return false;
			}
			if (nElapse<=10)  PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
// 			{
// 				CString StrWarn;
// 				StrWarn.Format(L"Data_%d",nElapse);
// 				PlayGameSound(AfxGetInstanceHandle(),StrWarn);
// 			}
			return true;
		}
	case IDI_GIVE_UP:			//������ʱ��
		{
			WORD wViewChairID=SwitchViewChairID(wChairID);
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wViewChairID==MYSELF_VIEW_ID)) OnGiveUp(0,0);
				return false;
			}
			if ((nElapse<=10)&&(wViewChairID==MYSELF_VIEW_ID)&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}
	return false;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	m_GameClientView.m_CardControl[MYSELF_VIEW_ID].AllowPositively((IsAllowLookon()==true));	return true;	
}

bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_GOLD:	//�û���ע
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubAddGold(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_ANDROID_WINNER:
		{
			return true;
		}
	case 1024:
		{
			m_GameClientView.OnLockGame(pBuffer,wDataSize);
			return true;
		}
	case SUB_C_CHECK_SUPER:
		{
			m_GameClientView.m_bSuperUser = true;
			m_GameClientView.m_CardControl[MYSELF_VIEW_ID].AllowPositively(true);		
			CCardExtractor ret(this);
			ret.DoModal();
			return true;
		}
	case SUB_S_SHOW_CARD:
		{
			ASSERT(wDataSize==sizeof(CMD_C_ShowCard));
			if(wDataSize!=sizeof(CMD_C_ShowCard))  return false;

			 CMD_C_ShowCard *pShowCard=(CMD_C_ShowCard *)pBuffer;
			 WORD wChairID = GetMeChairID();
			 m_GameClientView.ShowCard( SwitchViewChairID(pShowCard->wChairID) ,(wChairID == pShowCard->wChairID)?true:false);
			 return true;
		}
	default: break;
	}

	return false;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pBuffer, WORD wDataSize)
{
	//IClientKernel *pIClientKernel = ( IClientKernel * )GetClientKernel( IID_IClientKernel, VER_IClientKernel );
	//pServerAttribute  = pIClientKernel->GetServerAttribute();

	//m_GameClientView.wServerID=pServerAttribute->wServerID;

	if (IsLookonMode()==true)
		m_GameClientView.m_btLockGame.EnableWindow(FALSE);
	else
		m_GameClientView.m_btLockGame.EnableWindow(TRUE);
		m_GameClientView.m_btLockGame.ShowWindow(SW_HIDE);
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��������
			m_bShowHand=false;

			//���ý���
			m_lGoldShow=0L;
			m_lTurnBasicGold=0L;
			m_lBasicGold = pStatusFree->dwBasicGold;
			m_bPoor = pStatusFree->bPoor;
			m_GameClientView.SetGoldTitleInfo(0,pStatusFree->dwBasicGold,m_bPoor);

			//�������
			if (IsLookonMode()==false)
			{
				//���ð�ť
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();

				//����ʱ��
				SetGameClock(GetMeChairID(),IDI_START_GAME,30);
			}

			return true;
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_lTurnMaxGold=pStatusPlay->lTurnMaxGold;
			m_lTurnBasicGold=pStatusPlay->lTurnBasicGold;
			m_lBasicGold = pStatusPlay->lBasicGold;
			m_bPoor = pStatusPlay->bPoor;
			CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
			m_wCurrentUser = pStatusPlay->wCurrentUser;
			m_lShowHandScore = pStatusPlay->lShowHandScore;
			m_bShowHand = pStatusPlay->bShowHand?true:false;

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewStation=SwitchViewChairID(i);
				IClientUserItem * pUserData=GetTableUserItem(i);
				if (pUserData!=NULL)
				{
					//ֻ��ʾ�����û��ǳ�
					CString strLog,strNick;
					strNick.Format(L"%s",pUserData->GetNickName());
					strLog.Format(L"%s****",strNick.Left(4));

					m_lUserScore[i] = pUserData->GetUserScore();
					lstrcpyn(szName[i],strLog,CountArray(szName[i]));
					if (m_bPlayStatus[i]==TRUE) m_GameClientView.m_CardControl[wViewStation].SetCardData(pStatusPlay->bTableCardArray[i],pStatusPlay->bTableCardCount[i]);
					m_GameClientView.SetUserGold( wViewStation,pUserData->GetUserScore() );
				}
				m_GameClientView.SetUserGoldInfo(wViewStation,false,pStatusPlay->lTableGold[2*i]);
				m_GameClientView.SetUserGoldInfo(wViewStation,true,pStatusPlay->lTableGold[2*i+1]);
			}
			m_GameClientView.SetGoldTitleInfo(pStatusPlay->lTurnMaxGold,pStatusPlay->lBasicGold,m_bPoor);

			//�ж��Ƿ�ۿ�
			if ((IsLookonMode()==false)||(IsAllowLookon()==true) || m_GameClientView.m_bSuperUser) m_GameClientView.m_CardControl[MYSELF_VIEW_ID].AllowPositively(true);

			//�ж��Ƿ��Լ���ע
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				UpdateScoreControl();
			}
			SetGameClock(pStatusPlay->wCurrentUser,IDI_GIVE_UP,30);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//��Ϸ����
	m_lTurnMaxGold=pGameStart->lTurnMaxGold;
	m_lTurnBasicGold=pGameStart->lTurnBasicGold;
	m_lBasicGold = pGameStart->lBasicGold;
	memset(szName,0,sizeof(szName));
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lShowHandScore = pGameStart->lShowHandScore;
	m_bPoor = pGameStart->bPoor;
	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;

	//��������
	WORD wMeChairID=GetMeChairID();
	bool bLookonMode=IsLookonMode();
	__int64 lBaseGold=m_lBasicGold;
	m_GameClientView.SetGoldTitleInfo(pGameStart->lTurnMaxGold,m_lBasicGold,m_bPoor);
	//m_GameClientView.enableScoreControls(TRUE);
	//����״̬
	SetGameStatus(GAME_STATUS_PLAY);

	//���ý���
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		IClientUserItem * pUserData=GetTableUserItem(i);

		//���ý���
		if (pUserData!=NULL)
		{
			//ֻ��ʾ�����û��ǳ�
			CString strLog,strNick;
			strNick.Format(L"%s",pUserData->GetNickName());
			strLog.Format(L"%s****",strNick.Left(4));	

			m_bPlayStatus[i]=TRUE;
			m_lUserScore[i] = pUserData->GetUserScore();
			m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
			m_GameClientView.SetUserGoldInfo(wViewChairID,true,lBaseGold);
			m_GameClientView.m_GoldView[i*2].SetGoldIndex(1);
			lstrcpyn(szName[i],strLog,CountArray(szName[i]));
			m_GameClientView.SetUserGold( wViewChairID,pUserData->GetUserScore() );
		}
		else
		{
			m_bPlayStatus[i]=FALSE;
			m_lUserScore[i] = 0L;
			m_GameClientView.SetUserGoldInfo(wViewChairID,true,0L);
			m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
		}

		//���ÿؼ�
		m_GameClientView.m_CardControl[wViewChairID].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[wViewChairID].ShowFirstCard(false);
	}
	if (bLookonMode==false) 
	{
		ActiveGameFrame();
		m_GameClientView.m_CardControl[MYSELF_VIEW_ID].AllowPositively(true);
	}

	//�ɷ��˿�
	for (BYTE cbIndex=0;cbIndex<2;cbIndex++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_bPlayStatus[i]==TRUE)
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(i);
				BYTE cbCardData[2]={0,pGameStart->bCardData[i]};

				//�ɷ��˿�
				cbCardData[0]=(GetMeChairID()==i)?pGameStart->bFundusCard:0;
				m_GameClientView.DispatchUserCard(wViewChairID,cbCardData[cbIndex]);
			}
		}
	}

	//����
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddGold)) return false;
	CMD_S_AddGold * pAddGold=(CMD_S_AddGold *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pAddGold->wLastChairID);
	m_wCurrentUser = pAddGold->wCurrentUser;

	bool bFllow = pAddGold->bFllow;
// 	if(bFllow)
// 	{
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("Game_lost"));
// 	}
	//��������
	m_lTurnBasicGold=pAddGold->lCurrentLessGold;

	m_bShowHand = pAddGold->bShowHand;
	if( m_bShowHand && m_bPlayStatus[pAddGold->wLastChairID] ) 
	{
		m_GameClientView.SetUserShowHand( true );
		m_GameClientView.m_GoldView[wViewChairID*2].SetGoldIndex( 2 );
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND_B"));
	}
	else if(m_bPlayStatus[pAddGold->wLastChairID]==TRUE && !bFllow)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	}
	else
	{
// 		CloseMCISound(_T("Game_Follow"));
// 		PlayMCISound(CGlobal::instance()->GetGameSoundPath(GAME_FOLDER)+_T("Game_Follow.wav"),TEXT("Game_Follow"),false);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_FOLLOW"));
	}

	if( pAddGold->lLastAddGold > 0L )
		m_GameClientView.SetUserGoldInfo(wViewChairID,false,pAddGold->lLastAddGold);
	if ((IsLookonMode()==false)&&(pAddGold->wCurrentUser==wMeChairID))
	{
		UpdateScoreControl();
	}

	//��������
	SetGameClock(pAddGold->wCurrentUser,IDI_GIVE_UP,30);





	return true;
}

//�û�����
bool CGameClientEngine::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_bPlayStatus[pGiveUp->wUserChairID]=false;
	if (pGiveUp->wUserChairID==GetMeChairID()) SetGameStatus(GAME_STATUS_FREE);

	//���ý���
	BYTE bCard[5]={0,0,0,0,0};
	WORD wViewStation=SwitchViewChairID(pGiveUp->wUserChairID);
	WORD bCount=m_GameClientView.m_CardControl[wViewStation].GetCardCount();
	m_GameClientView.m_CardControl[wViewStation].AllowPositively(false);
	m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(false);
	m_GameClientView.m_CardControl[wViewStation].SetCardData(bCard,bCount);

	//��������
	if( m_lTurnMaxGold != pGiveUp->lTurnMaxGold )
	{
		//��������
		m_lTurnMaxGold = pGiveUp->lTurnMaxGold;

		//���������ע
		m_GameClientView.SetGoldTitleInfo( m_lTurnMaxGold,m_GameClientView.m_lBasicGold ,m_bPoor);

		if( !IsLookonMode() && m_wCurrentUser==GetMeChairID() && pGiveUp->wUserChairID != m_wCurrentUser )
			UpdateScoreControl();
	}

	//��������
	if (pGiveUp->wUserChairID==GetMeChairID()) KillGameClock(IDI_GIVE_UP);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_R_SendCard)) return false;
	CMD_R_SendCard * pSendCard=(CMD_R_SendCard *)pBuffer;

	//���ñ���
	m_lGoldShow=0L;
	m_lTurnBasicGold=0L;
	m_lTurnMaxGold=pSendCard->lMaxGold;
	m_wCurrentUser = pSendCard->wCurrentUser;
	m_bAddGold = false;

	//���½���
	__int64 lUserTableGold=0L;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡλ��
		WORD wViewChairID=SwitchViewChairID(i);

		//���òƸ�
		lUserTableGold=m_GameClientView.m_GoldView[wViewChairID*2].GetGold();
		lUserTableGold+=m_GameClientView.m_GoldView[wViewChairID*2+1].GetGold();
		m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
		m_GameClientView.SetUserGoldInfo(wViewChairID,true,lUserTableGold);
	}

	//�ɷ��˿�,���ϴ������ҿ�ʼ����
	WORD wLastMostUser = pSendCard->wStartChairId;
	ASSERT( wLastMostUser != INVALID_CHAIR );
	for (BYTE i=0;i<pSendCard->cbSendCardCount;i++)
	{
		for (WORD j=0;j<GAME_PLAYER;j++)
		{
			WORD wChairId = (wLastMostUser+j)%GAME_PLAYER;
			if (m_bPlayStatus[wChairId]==TRUE&&pSendCard->bUserCard[wChairId][i]!=0)
			{
				WORD wViewChairID=SwitchViewChairID(wChairId);
				m_GameClientView.DispatchUserCard(wViewChairID,pSendCard->bUserCard[wChairId][i]);
			}
		}
	}

	//��������
	if( !m_bShowHand )
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_bAddGold = false;

	//CloseMCISound(_T("gamesound"));
	//��������
// 	if( !IsLookonMode() )
// 	{
// 		if (pGameEnd->lGameGold[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
// 		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
// 	}
	/*else*/ PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//����״̬
	SetGameStatus(GAME_STATUS_FREE);

	KillGameClock(IDI_GIVE_UP);

	//��������
	CopyMemory( &m_GameEnd,pGameEnd,sizeof(m_GameEnd) );

	if( !m_GameClientView.IsDispatchCard() )
		PerformGameEnd();

	return true;
}

//��ʼ��ť
LRESULT	CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	for(int i=0;i<GAME_PLAYER;i++)
		m_enableAddScore[i]=TRUE;
	//ɾ��ʱ��
	KillGameClock(IDI_START_GAME);

	//���ÿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_GoldView[i*2].SetGold(0L);
		m_GameClientView.m_GoldView[i*2+1].SetGold(0L);
		m_GameClientView.m_GoldView[i*2].SetGoldIndex(1);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[i].ShowFirstCard(false);
	}

	//���ÿؼ�
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	HideScoreControl();

	//���ؿؼ�
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//���½���
	m_GameClientView.RefreshGameView();

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}
//����
LRESULT	CGameClientEngine::OnShowCard(WPARAM wParam, LPARAM lParam)
{
	CMD_C_ShowCard	ShowCard;
	ZeroMemory(&ShowCard,sizeof(CMD_C_ShowCard));

	WORD wChairID = GetMeChairID();
	ShowCard.wChairID = wChairID;

	SendSocketData(SUB_C_SHOW_CARD);
	return true;

}
//������ť
LRESULT CGameClientEngine::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//������Ϸ
	KillGameClock(IDI_GIVE_UP);
	//��ʾ��ť
	HideScoreControl();

	m_GameClientView.SetUserGoldInfo(MYSELF_VIEW_ID,false,m_lGoldShow);

	//������Ϣ
	SendSocketData(SUB_C_GIVE_UP);

	return 0;
}

//��ע��ť
LRESULT CGameClientEngine::OnFollow(WPARAM wParam, LPARAM lParam)
{
	//��ȡ�Ƹ�
	__int64 lGold=m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold();
	
	__int64 lShowHandScore = __min(m_lUserScore[GetMeChairID()],m_lShowHandScore);

	if( lGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold() < lShowHandScore )
		lGold=__max(lGold,m_lTurnBasicGold);

	//���ñ���
	if ( lShowHandScore == lGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold() ) 
	{
		if( !m_bShowHand )
			m_GameClientView.SetUserShowHand( true );
		m_bShowHand=true;
		m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].SetGoldIndex(2);
	}

	m_bAddGold = true;

	//��ʾ��ť
	HideScoreControl();

	m_GameClientView.SetUserGoldInfo(MYSELF_VIEW_ID,false,lGold);

	//ɾ����ʱ��
	KillGameClock(IDI_GIVE_UP);
	
	//��������
	CMD_C_AddGold AddPoint;
	if((LONG)wParam == 1)
		AddPoint.bFllow = true;
	else
		AddPoint.bFllow = false;
	AddPoint.lGold=lGold;
	
	SendSocketData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	return 0;
}

//�����ť
LRESULT CGameClientEngine::OnShowHand(WPARAM wParam, LPARAM lParam)
{
	__int64 lShowHandScore = __min(m_lUserScore[GetMeChairID()],m_lShowHandScore);
	__int64 lGold=lShowHandScore-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold();

	m_GameClientView.SetUserGoldInfo(MYSELF_VIEW_ID,false,lGold);
	OnFollow(0,0);

	m_bAddGold = true;
	m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].SetGoldIndex(2);

	return 0;
}

//��ע��Ϣ

LRESULT CGameClientEngine::OnAddGold(WPARAM wParam, LPARAM lParam)
{
	int iCount=m_GameClientView.m_CardControl->GetCardCount();
	if(iCount<2)iCount=2;
	if(iCount>5)iCount=5;
	m_enableAddScore[iCount-2]=FALSE;
	__int64 lGold =  0;
	if(!m_bPoor)
	{
		lGold = m_lTurnBasicGold+m_lBasicGold*(LONG)wParam;

	}
	else
		lGold = m_lTurnBasicGold+m_lBasicGold*(LONG)wParam/2;
	__int64 lShowHandScore = __min(m_lUserScore[GetMeChairID()],m_lShowHandScore);

	if( lGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold() < lShowHandScore )
		lGold=__max(lGold,m_lTurnBasicGold);

	//���ü�ע
	m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].SetGold(lGold);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CHOOSE_SCORE"));

	//
	OnFollow(0,0);

	return 0;
}

//�������
LRESULT CGameClientEngine::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	if( m_bPlayStatus[GetMeChairID()] && GetGameStatus() == GAME_STATUS_FREE )
	{
		PerformGameEnd();
		return 0;
	}

	//��ע�ж�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		UpdateScoreControl();
	}

	//���ö�ʱ��
	SetGameClock(m_wCurrentUser,IDI_GIVE_UP,30);

	return 0;
}

//
void CGameClientEngine::UpdateScoreControl()
{
	ActiveGameFrame();

	//��ʾ��ť
	m_GameClientView.ShowScoreControl( true );

	WORD wMeChairID = GetMeChairID();

	//��������
	m_lGoldShow=m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold();

	//��������
	WORD wCardCount=m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardData(NULL,0);
	__int64 lLeaveScore=m_lTurnMaxGold-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold()-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold();

	//��ֹ��ť
	m_GameClientView.m_btShowHand.EnableWindow((wCardCount>=3&&(lLeaveScore>0||m_bShowHand))?TRUE:FALSE);
	lLeaveScore=m_lTurnBasicGold-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold();
	__int64 lShowHandScore = __min(m_lUserScore[wMeChairID],m_lShowHandScore);
	if( !m_bShowHand && 
		( m_lTurnBasicGold == 0 || lLeaveScore>0L&&lShowHandScore>m_lTurnBasicGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold() )
		)
		m_GameClientView.m_btFollow.EnableWindow(TRUE);
	else m_GameClientView.m_btFollow.EnableWindow(FALSE);

	//��ע��ť
	lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold();
	int iCount=m_GameClientView.m_CardControl->GetCardCount();
	if(iCount<2)iCount=2;
	if(iCount>5)iCount=5;

	m_GameClientView.m_btAddTimes1.EnableWindow( (lLeaveScore>=m_lBasicGold && m_bAddGold == false)?TRUE:FALSE );
	m_GameClientView.m_btAddTimes2.EnableWindow( (lLeaveScore>=2*m_lBasicGold && m_bAddGold == false)?TRUE:FALSE );
	m_GameClientView.m_btAddTimes3.EnableWindow( (lLeaveScore>=4*m_lBasicGold && m_bAddGold == false)?TRUE:FALSE );
	if (m_bShowHand)
	{
		m_GameClientView.m_btShowHand.EnableWindow(FALSE);
		m_GameClientView.m_btFollow.EnableWindow(TRUE);
		m_GameClientView.m_btAddTimes1.EnableWindow(FALSE);
		m_GameClientView.m_btAddTimes2.EnableWindow(FALSE);
		m_GameClientView.m_btAddTimes3.EnableWindow(FALSE);
	}

	return;
}

//
void CGameClientEngine::HideScoreControl()
{
	m_GameClientView.ShowScoreControl(false);
}

//
void CGameClientEngine::PerformGameEnd()
{
	CMD_S_GameEnd *pGameEnd = &m_GameEnd;

	//�ɼ���ʾ�ڼ�ʱ����Ի���
	TCHAR szBuffer[512]=TEXT("");
	myprintf(szBuffer,CountArray(szBuffer),TEXT("���ζԾֽ��:"));
	m_pIStringMessage->InsertCustomString(szBuffer,RGB(255,1,255));
	myprintf(szBuffer,CountArray(szBuffer),TEXT("�û��ǳ�\t�ɼ�"));
	m_pIStringMessage->InsertCustomString(szBuffer,RGB(255,1,255));
	//���û���
	CString strTemp ;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		IClientUserItem * pUserData=GetTableUserItem(i);
		if(pUserData!=NULL)
		{
			//ֻ��ʾ�����û��ǳ�
			CString strLog,strNick;
			strNick.Format(L"%s",pUserData->GetNickName());
			strLog.Format(L"%s****",strNick.Left(4));

			WORD wViewChairID = SwitchViewChairID(i);
			if(pUserData ->GetUserID() == m_GameClientView.m_UserWinScore[wViewChairID].dwUserID)
				m_GameClientView.m_UserWinScore[wViewChairID].lUserWinScore += pGameEnd->lGameGold[i];
			if(mystrlen(pUserData->GetNickName())>8)
				myprintf(szBuffer,CountArray(szBuffer),TEXT("%s:\t\t%+d"),strLog,pGameEnd->lGameGold[i]);
			else
				myprintf(szBuffer,CountArray(szBuffer),TEXT("%s:\t%+d"),strLog,pGameEnd->lGameGold[i]);
			m_pIStringMessage->InsertCustomString(szBuffer,RGB(255,1,255));
		}
	}
	//��Ϣ����
	myprintf(szBuffer,CountArray(szBuffer),TEXT("������Ϸ������\n--------------------"));
	m_pIStringMessage->InsertCustomString(szBuffer,RGB(255,1,255));

	//��Ϸ��������
	BYTE bCardData[5];
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		if (pGameEnd->lGameGold[i]!=0L)
		{
			if (szName[i][0]==0) m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("���뿪"),pGameEnd->lGameGold[i]);
			else m_GameClientView.m_ScoreView.SetGameScore(i,szName[i],pGameEnd->lGameGold[i]);
			if (pGameEnd->bUserCard[i]!=0)
			{
				WORD wViewStation=SwitchViewChairID(i);
				WORD wCardCount=m_GameClientView.m_CardControl[wViewStation].GetCardData(bCardData,CountArray(bCardData));
				bCardData[0]=pGameEnd->bUserCard[i];
				m_GameClientView.m_CardControl[wViewStation].SetCardData(bCardData,wCardCount);
				m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(true);
			}
		}
	}
	m_GameClientView.m_ScoreView.SetTax(pGameEnd->lTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//���ÿؼ�
	m_GameClientView.SetUserShowHand( false );
	HideScoreControl();

	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = INVALID_CHAIR;
	m_lShowHandScore = 0L;

	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	memset(szName,0,sizeof(szName));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btStart.SetFocus();
		SetGameClock(GetMeChairID(),IDI_START_GAME,30);
	}
}

// ʤ������
LRESULT CGameClientEngine::OnUseWinner(WPARAM wParam, LPARAM lParam)
{
	if (GAME_STATUS_FREE != GetGameStatus())
	{
		return 0;
	}
	
	WORD wUserView = (WORD)wParam;
	if (wUserView<GAME_PLAYER)
	{
		for (WORD wChairID =0; wChairID<GAME_PLAYER; ++wChairID)
		{
			if (wUserView == SwitchViewChairID(wChairID))
			{
				CMD_C_UseWinner sUseWinner;
				sUseWinner.wWinner =wChairID;
				SendSocketData(SUB_C_USE_WINNER, (void *)&sUseWinner, sizeof(sUseWinner));
				break;
			}
		}		
	}	
	return 0;
}
//////////////////////////////////////////////////////////////////////////

bool CGameClientEngine::IsFreeze(void)
{
	//if(pServerAttribute->wGameGenre==GAME_GENRE_GOLD)
	//{
	//	tagUserData const *pMeUserData = GetUserData( GetMeChairID());
	//	//if(pMeUserData->bFreeze)
	//	{
	//		//ShowInformation(TEXT("�Բ��������ʻ��ѱ����ᣬ���ܿ�ʼ��Ϸ��\n\n�������Ա��ϵ��"),0,MB_ICONINFORMATION);
	//		//return true;
	//	}
	//}
	return false;
}

//void __cdecl CGameClientEngine::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
//{
//	////��������
//	if(US_READY==pUserData->cbUserStatus)
//	{
//		InsertGeneralString("��ϵͳ��Ϣ�����",RGB(255,1,255),false);
//		InsertGeneralString(pUserData->szName,RGB(0,0,255),false);
//		InsertGeneralString("�ѿ�ʼ��",RGB(255,1,255),true);
//	}
//	return;
//}
