

//
#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include "CardExtractor.h"
//////////////////////////////////////////////////////////////////////////

//定时器标识
#define IDI_START_GAME					200								//开始定时器
#define IDI_GIVE_UP						201								//放弃定时器

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

//构造函数
CGameClientEngine::CGameClientEngine()
{
	//游戏变量
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	m_lBasicGold = 0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_lShowHandScore = 0L;
	ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );

	m_bPoor = true;
	//辅助变量
	m_lGoldShow=0L;
	m_bShowHand=false;
	memset(szName,0,sizeof(szName));
	ZeroMemory(&m_GameEnd,sizeof(m_GameEnd));

	m_bAddGold = false;

	return;
}

//析构函数
CGameClientEngine::~CGameClientEngine()
{
}

//初始函数
bool CGameClientEngine::OnInitGameEngine()
{
	//全局对象
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	ASSERT(m_pGlobalUnits!=NULL);

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	return true;
}

//重置框架
bool CGameClientEngine::OnResetGameEngine()
{
	//if(IsFreeze())
	//{
	//	OnCancel();
	//	return;
	//}

	//游戏变量
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	m_lBasicGold = 0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = INVALID_CHAIR;
	m_lShowHandScore = 0L;
	ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );

	//辅助变量
	m_lGoldShow=0L;
	m_bShowHand=false;
	memset(szName,0,sizeof(szName));
	ZeroMemory(&m_GameEnd,sizeof(m_GameEnd));

	m_bAddGold = false;

	return true;
}

//游戏设置
void CGameClientEngine::OnGameOptionSet()
{
	//构造数据
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	CGameOption GameOption;
	GameOption.m_bEnableSound=m_pGlobalUnits->m_bAllowSound;
	GameOption.m_bAllowLookon = IsAllowLookon();

	////配置数据
	if (GameOption.DoModal()==IDOK)
	{
		//设置控件
		m_pGlobalUnits->m_bAllowSound = GameOption.m_bEnableSound;
		m_pGlobalUnits->m_bAllowLookon = GameOption.m_bAllowLookon;
	}

	return;
}

//时间消息
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//继续游戏定时器
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
	case IDI_GIVE_UP:			//放弃定时器
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

//旁观状态
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	m_GameClientView.m_CardControl[MYSELF_VIEW_ID].AllowPositively((IsAllowLookon()==true));	return true;	
}

bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//网络消息
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_GOLD:	//用户下注
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubAddGold(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:		//用户放弃
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//发牌消息
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//游戏结束
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

//游戏场景
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
	case GAME_STATUS_FREE:		//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//辅助变量
			m_bShowHand=false;

			//设置界面
			m_lGoldShow=0L;
			m_lTurnBasicGold=0L;
			m_lBasicGold = pStatusFree->dwBasicGold;
			m_bPoor = pStatusFree->bPoor;
			m_GameClientView.SetGoldTitleInfo(0,pStatusFree->dwBasicGold,m_bPoor);

			//玩家设置
			if (IsLookonMode()==false)
			{
				//设置按钮
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();

				//设置时间
				SetGameClock(GetMeChairID(),IDI_START_GAME,30);
			}

			return true;
		}
	case GAME_STATUS_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//设置变量
			m_lTurnMaxGold=pStatusPlay->lTurnMaxGold;
			m_lTurnBasicGold=pStatusPlay->lTurnBasicGold;
			m_lBasicGold = pStatusPlay->lBasicGold;
			m_bPoor = pStatusPlay->bPoor;
			CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
			m_wCurrentUser = pStatusPlay->wCurrentUser;
			m_lShowHandScore = pStatusPlay->lShowHandScore;
			m_bShowHand = pStatusPlay->bShowHand?true:false;

			//设置界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewStation=SwitchViewChairID(i);
				IClientUserItem * pUserData=GetTableUserItem(i);
				if (pUserData!=NULL)
				{
					//只显示部分用户昵称
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

			//判断是否观看
			if ((IsLookonMode()==false)||(IsAllowLookon()==true) || m_GameClientView.m_bSuperUser) m_GameClientView.m_CardControl[MYSELF_VIEW_ID].AllowPositively(true);

			//判断是否自己下注
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

//游戏开始
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//游戏变量
	m_lTurnMaxGold=pGameStart->lTurnMaxGold;
	m_lTurnBasicGold=pGameStart->lTurnBasicGold;
	m_lBasicGold = pGameStart->lBasicGold;
	memset(szName,0,sizeof(szName));
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lShowHandScore = pGameStart->lShowHandScore;
	m_bPoor = pGameStart->bPoor;
	//辅助变量
	m_lGoldShow=0L;
	m_bShowHand=false;

	//变量定义
	WORD wMeChairID=GetMeChairID();
	bool bLookonMode=IsLookonMode();
	__int64 lBaseGold=m_lBasicGold;
	m_GameClientView.SetGoldTitleInfo(pGameStart->lTurnMaxGold,m_lBasicGold,m_bPoor);
	//m_GameClientView.enableScoreControls(TRUE);
	//设置状态
	SetGameStatus(GAME_STATUS_PLAY);

	//设置界面
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wViewChairID=SwitchViewChairID(i);
		IClientUserItem * pUserData=GetTableUserItem(i);

		//设置界面
		if (pUserData!=NULL)
		{
			//只显示部分用户昵称
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

		//设置控件
		m_GameClientView.m_CardControl[wViewChairID].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[wViewChairID].ShowFirstCard(false);
	}
	if (bLookonMode==false) 
	{
		ActiveGameFrame();
		m_GameClientView.m_CardControl[MYSELF_VIEW_ID].AllowPositively(true);
	}

	//派发扑克
	for (BYTE cbIndex=0;cbIndex<2;cbIndex++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_bPlayStatus[i]==TRUE)
			{
				//变量定义
				WORD wViewChairID=SwitchViewChairID(i);
				BYTE cbCardData[2]={0,pGameStart->bCardData[i]};

				//派发扑克
				cbCardData[0]=(GetMeChairID()==i)?pGameStart->bFundusCard:0;
				m_GameClientView.DispatchUserCard(wViewChairID,cbCardData[cbIndex]);
			}
		}
	}

	//设置
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//用户加注
bool CGameClientEngine::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_AddGold)) return false;
	CMD_S_AddGold * pAddGold=(CMD_S_AddGold *)pBuffer;

	//变量定义
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pAddGold->wLastChairID);
	m_wCurrentUser = pAddGold->wCurrentUser;

	bool bFllow = pAddGold->bFllow;
// 	if(bFllow)
// 	{
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("Game_lost"));
// 	}
	//处理数据
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

	//其他处理
	SetGameClock(pAddGold->wCurrentUser,IDI_GIVE_UP,30);





	return true;
}

//用户放弃
bool CGameClientEngine::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//设置变量
	m_bPlayStatus[pGiveUp->wUserChairID]=false;
	if (pGiveUp->wUserChairID==GetMeChairID()) SetGameStatus(GAME_STATUS_FREE);

	//设置界面
	BYTE bCard[5]={0,0,0,0,0};
	WORD wViewStation=SwitchViewChairID(pGiveUp->wUserChairID);
	WORD bCount=m_GameClientView.m_CardControl[wViewStation].GetCardCount();
	m_GameClientView.m_CardControl[wViewStation].AllowPositively(false);
	m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(false);
	m_GameClientView.m_CardControl[wViewStation].SetCardData(bCard,bCount);

	//更新数据
	if( m_lTurnMaxGold != pGiveUp->lTurnMaxGold )
	{
		//设置数据
		m_lTurnMaxGold = pGiveUp->lTurnMaxGold;

		//设置最大下注
		m_GameClientView.SetGoldTitleInfo( m_lTurnMaxGold,m_GameClientView.m_lBasicGold ,m_bPoor);

		if( !IsLookonMode() && m_wCurrentUser==GetMeChairID() && pGiveUp->wUserChairID != m_wCurrentUser )
			UpdateScoreControl();
	}

	//其他处理
	if (pGiveUp->wUserChairID==GetMeChairID()) KillGameClock(IDI_GIVE_UP);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	return true;
}

//发牌消息
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_R_SendCard)) return false;
	CMD_R_SendCard * pSendCard=(CMD_R_SendCard *)pBuffer;

	//设置变量
	m_lGoldShow=0L;
	m_lTurnBasicGold=0L;
	m_lTurnMaxGold=pSendCard->lMaxGold;
	m_wCurrentUser = pSendCard->wCurrentUser;
	m_bAddGold = false;

	//更新界面
	__int64 lUserTableGold=0L;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取位置
		WORD wViewChairID=SwitchViewChairID(i);

		//设置财富
		lUserTableGold=m_GameClientView.m_GoldView[wViewChairID*2].GetGold();
		lUserTableGold+=m_GameClientView.m_GoldView[wViewChairID*2+1].GetGold();
		m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
		m_GameClientView.SetUserGoldInfo(wViewChairID,true,lUserTableGold);
	}

	//派发扑克,从上次最大玩家开始发起
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

	//播放声音
	if( !m_bShowHand )
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	return true;
}

//游戏结束
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_bAddGold = false;

	//CloseMCISound(_T("gamesound"));
	//播放声音
// 	if( !IsLookonMode() )
// 	{
// 		if (pGameEnd->lGameGold[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
// 		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
// 	}
	/*else*/ PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//设置状态
	SetGameStatus(GAME_STATUS_FREE);

	KillGameClock(IDI_GIVE_UP);

	//复制数据
	CopyMemory( &m_GameEnd,pGameEnd,sizeof(m_GameEnd) );

	if( !m_GameClientView.IsDispatchCard() )
		PerformGameEnd();

	return true;
}

//开始按钮
LRESULT	CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	for(int i=0;i<GAME_PLAYER;i++)
		m_enableAddScore[i]=TRUE;
	//删除时间
	KillGameClock(IDI_START_GAME);

	//设置控件
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_GoldView[i*2].SetGold(0L);
		m_GameClientView.m_GoldView[i*2+1].SetGold(0L);
		m_GameClientView.m_GoldView[i*2].SetGoldIndex(1);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[i].ShowFirstCard(false);
	}

	//设置控件
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	HideScoreControl();

	//隐藏控件
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//更新界面
	m_GameClientView.RefreshGameView();

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}
//看牌
LRESULT	CGameClientEngine::OnShowCard(WPARAM wParam, LPARAM lParam)
{
	CMD_C_ShowCard	ShowCard;
	ZeroMemory(&ShowCard,sizeof(CMD_C_ShowCard));

	WORD wChairID = GetMeChairID();
	ShowCard.wChairID = wChairID;

	SendSocketData(SUB_C_SHOW_CARD);
	return true;

}
//放弃按钮
LRESULT CGameClientEngine::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//放弃游戏
	KillGameClock(IDI_GIVE_UP);
	//显示按钮
	HideScoreControl();

	m_GameClientView.SetUserGoldInfo(MYSELF_VIEW_ID,false,m_lGoldShow);

	//发送消息
	SendSocketData(SUB_C_GIVE_UP);

	return 0;
}

//跟注按钮
LRESULT CGameClientEngine::OnFollow(WPARAM wParam, LPARAM lParam)
{
	//获取财富
	__int64 lGold=m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold();
	
	__int64 lShowHandScore = __min(m_lUserScore[GetMeChairID()],m_lShowHandScore);

	if( lGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold() < lShowHandScore )
		lGold=__max(lGold,m_lTurnBasicGold);

	//设置变量
	if ( lShowHandScore == lGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold() ) 
	{
		if( !m_bShowHand )
			m_GameClientView.SetUserShowHand( true );
		m_bShowHand=true;
		m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].SetGoldIndex(2);
	}

	m_bAddGold = true;

	//显示按钮
	HideScoreControl();

	m_GameClientView.SetUserGoldInfo(MYSELF_VIEW_ID,false,lGold);

	//删除定时器
	KillGameClock(IDI_GIVE_UP);
	
	//发送数据
	CMD_C_AddGold AddPoint;
	if((LONG)wParam == 1)
		AddPoint.bFllow = true;
	else
		AddPoint.bFllow = false;
	AddPoint.lGold=lGold;
	
	SendSocketData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	return 0;
}

//梭哈按钮
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

//加注消息

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

	//设置加注
	m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].SetGold(lGold);

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CHOOSE_SCORE"));

	//
	OnFollow(0,0);

	return 0;
}

//发牌完成
LRESULT CGameClientEngine::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	if( m_bPlayStatus[GetMeChairID()] && GetGameStatus() == GAME_STATUS_FREE )
	{
		PerformGameEnd();
		return 0;
	}

	//下注判断
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		UpdateScoreControl();
	}

	//设置定时器
	SetGameClock(m_wCurrentUser,IDI_GIVE_UP,30);

	return 0;
}

//
void CGameClientEngine::UpdateScoreControl()
{
	ActiveGameFrame();

	//显示按钮
	m_GameClientView.ShowScoreControl( true );

	WORD wMeChairID = GetMeChairID();

	//设置数据
	m_lGoldShow=m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold();

	//变量定义
	WORD wCardCount=m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardData(NULL,0);
	__int64 lLeaveScore=m_lTurnMaxGold-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold()-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold();

	//禁止按钮
	m_GameClientView.m_btShowHand.EnableWindow((wCardCount>=3&&(lLeaveScore>0||m_bShowHand))?TRUE:FALSE);
	lLeaveScore=m_lTurnBasicGold-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold();
	__int64 lShowHandScore = __min(m_lUserScore[wMeChairID],m_lShowHandScore);
	if( !m_bShowHand && 
		( m_lTurnBasicGold == 0 || lLeaveScore>0L&&lShowHandScore>m_lTurnBasicGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold() )
		)
		m_GameClientView.m_btFollow.EnableWindow(TRUE);
	else m_GameClientView.m_btFollow.EnableWindow(FALSE);

	//加注按钮
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

	//成绩显示在即时聊天对话框
	TCHAR szBuffer[512]=TEXT("");
	myprintf(szBuffer,CountArray(szBuffer),TEXT("本次对局结果:"));
	m_pIStringMessage->InsertCustomString(szBuffer,RGB(255,1,255));
	myprintf(szBuffer,CountArray(szBuffer),TEXT("用户昵称\t成绩"));
	m_pIStringMessage->InsertCustomString(szBuffer,RGB(255,1,255));
	//设置积分
	CString strTemp ;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		IClientUserItem * pUserData=GetTableUserItem(i);
		if(pUserData!=NULL)
		{
			//只显示部分用户昵称
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
	//消息积分
	myprintf(szBuffer,CountArray(szBuffer),TEXT("本局游戏结束。\n--------------------"));
	m_pIStringMessage->InsertCustomString(szBuffer,RGB(255,1,255));

	//游戏正常结束
	BYTE bCardData[5];
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//设置信息
		if (pGameEnd->lGameGold[i]!=0L)
		{
			if (szName[i][0]==0) m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("已离开"),pGameEnd->lGameGold[i]);
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

	//设置控件
	m_GameClientView.SetUserShowHand( false );
	HideScoreControl();

	//游戏变量
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = INVALID_CHAIR;
	m_lShowHandScore = 0L;

	//辅助变量
	m_lGoldShow=0L;
	m_bShowHand=false;
	memset(szName,0,sizeof(szName));

	//设置界面
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btStart.SetFocus();
		SetGameClock(GetMeChairID(),IDI_START_GAME,30);
	}
}

// 胜负控制
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
	//		//ShowInformation(TEXT("对不起，您的帐户已被冻结，不能开始游戏！\n\n请与管理员联系。"),0,MB_ICONINFORMATION);
	//		//return true;
	//	}
	//}
	return false;
}

//void __cdecl CGameClientEngine::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
//{
//	////变量定义
//	if(US_READY==pUserData->cbUserStatus)
//	{
//		InsertGeneralString("【系统消息】玩家",RGB(255,1,255),false);
//		InsertGeneralString(pUserData->szName,RGB(0,0,255),false);
//		InsertGeneralString("已开始。",RGB(255,1,255),true);
//	}
//	return;
//}
