#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//游戏时间
//#define IDI_OUT_CARD				200									//出牌定时器
//#define IDI_START_GAME				201									//开始定时器
//#define IDI_CALL_SCORE				202									//叫分定时器

#define IDI_OUT_CARD				200									//出牌定时器
#define IDI_START_GAME				201									//开始定时器
#define IDI_CALL_BANKER				202									//叫地主时器
#define IDI_ROD_BANKER				203									//抢地主时器
//#define IDI_ADD_DUBLE				204									//加倍时器


//游戏时间
#define IDI_MOST_CARD				300									//最大定时器
#define IDI_LAST_TURN				301									//上轮定时器

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)

	//系统消息
	ON_WM_TIMER()

	//游戏消息
	ON_MESSAGE(IDM_START,OnMessageStart)
	ON_MESSAGE(IDM_OUT_CARD,OnMessageOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnMessagePassCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnMessageOutPrompt)
	//ON_MESSAGE(IDM_CALL_SCORE,OnMessageCallScore)

	//叫地主和抢地主消息

	ON_MESSAGE(IDM_CALL_BANKER,OnMessageCallBanker)
	ON_MESSAGE(IDM_ROD_BANKER,OnMessageRodBanker)



	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnMessageLeftHitCard)
	ON_MESSAGE(IDM_SORT_HAND_CARD,OnMessageSortHandCard)
	ON_MESSAGE(IDM_LAST_TURN_CARD,OnMessageLastTurnCard)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnMessageTrusteeControl)
	ON_MESSAGE(IDM_SEARCH_CARD,OnMessageSearchCard)

	//辅助消息
	ON_MESSAGE(WM_ENABLE_HISTORY,OnMessageEnableHistory)
	ON_MESSAGE(IDM_OUT_CARD_FINISH,OnMessageOutCardFinish)
	ON_MESSAGE(IDM_DISPATCH_FINISH,OnMessageDispatchFinish)
	ON_MESSAGE(IDM_REVERSAL_FINISH,OnMessageReversalFinish)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientEngine::CGameClientEngine()
{
	//辅助变量
	m_bTrusteeCount = 0;
	m_bTrustee=false;
	m_bLastTurn=false;
	m_cbSortType=ST_ORDER;
	m_bBackGroundSound = false;
	m_bCheatRight = false;

	//时间定义
	m_cbTimeCallBanker=0;
	m_cbTimeRodBanker=0;
	m_cbTimeOutCard=0;
	//m_cbTimeCallScore=0;
	m_cbTimeStartGame=0;
	m_cbTimeHeadOutCard=0;

	//游戏变量
	//m_cbBombCount=0;
	//m_wBankerUser=INVALID_CHAIR;
	//m_cbBankerScore = 0;
	m_wCurrentUser=INVALID_CHAIR;
	m_wMostCardUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	//游戏变量
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wFirstValidUser=INVALID_CHAIR;
	m_wCurrentTime=1;
	m_wStartTime=1;
	m_wRodBankerTime=1;
	m_wBackCardTime=1;
	m_wBombTime=1;
	m_wChunTianbTime=1;
	m_wCardTypeTime=1;


	m_wCurrentUser=INVALID_CHAIR;
	m_wMostCardUser=INVALID_CHAIR;
	
	ZeroMemory(m_cbCallBankerInfo,sizeof(m_cbCallBankerInfo));
	ZeroMemory(m_cbRodBankerInfo,sizeof(m_cbRodBankerInfo));
	ZeroMemory(m_cbAddDoubleInfo,sizeof(m_cbAddDoubleInfo));

	//搜索变量
	m_cbSearchResultIndex = 0;
	ZeroMemory( &m_SearchCardResult,sizeof(m_SearchCardResult) );
	m_nCurSearchType = -1;
	m_cbEachSearchIndex = 0;
	ZeroMemory( &m_EachSearchResult,sizeof(m_EachSearchResult) );

	//出牌变量
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	return;
}

//析构函数
CGameClientEngine::~CGameClientEngine()
{
}

//创建引擎
bool CGameClientEngine::OnInitGameEngine()
{
	//设置属性
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	//播放声音
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	if ( pGlobalUnits->m_bAllowBackGroundSound && !pGlobalUnits->m_bMuteStatuts)
	{
		m_bBackGroundSound = true;
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_MUSIC"));
	}

	return true;
}

//重置函数
bool CGameClientEngine::OnResetGameEngine()
{
	//删除时间
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_LAST_TURN);

	//辅助变量
	m_bTrusteeCount = 0;
	m_bTrustee=false;
	m_bLastTurn=false;
	m_cbSortType=ST_ORDER;
	m_bCheatRight = false;

	//时间定义
	m_cbTimeCallBanker=0;
	m_cbTimeRodBanker=0;
	m_cbTimeOutCard=0;
	//m_cbTimeCallScore=0;
	m_cbTimeStartGame=0;
	m_cbTimeHeadOutCard=0;

	//游戏变量
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	//m_cbBankerScore = 0;
	m_wCurrentUser=INVALID_CHAIR;
	m_wMostCardUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	//搜索变量
	m_cbSearchResultIndex = 0;
	ZeroMemory( &m_SearchCardResult,sizeof(m_SearchCardResult) );
	m_nCurSearchType = -1;
	m_cbEachSearchIndex = 0;
	ZeroMemory( &m_EachSearchResult,sizeof(m_EachSearchResult) );


	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//出牌变量
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	return true;
}

//时钟删除
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//时钟信息
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_OUT_CARD:		//用户出牌
		{
			if (IsLookonMode() ) return true;

			//自动处理
			if ( wChairID == GetMeChairID() &&
				( nElapse <= 0 || ( m_bTrustee && (int)nElapse < (int)m_cbTimeOutCard - 1 ) ) )
			{

				//自动托管
				if ( !m_bTrustee && nElapse == 0 && m_bTrusteeCount < 3 )
				{
					m_bTrusteeCount++;
					if ( m_bTrusteeCount == 3 )
					{
						OnMessageTrusteeControl(0,0);
					}
				}

				//自动出牌
				if( m_bTrustee || m_cbTurnCardCount == 0 )
					AutomatismOutCard();
				else 
					OnMessagePassCard(0,0);

				return true;
			}

			//超时警告
			if ((nElapse<=5)&&(wChairID==GetMeChairID())) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_START_GAME:	//游戏开始
		{
			//关闭游戏
			if ( nElapse <= 0 && !IsLookonMode() && wChairID == GetMeChairID() )
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return true;
			}

			//超时警告
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_CALL_BANKER:	//用户叫地主
		{
			//更新整个界面
			//m_GameClientView.InvalidGameView(0,0,0,0);

			//自动处理
			if (( nElapse <= 0 ) || ( m_bTrustee && (int)nElapse < (int)m_cbTimeCallBanker - 1 ))
			{
				if ( !IsLookonMode() && wChairID==GetMeChairID() ) 
					OnMessageCallBanker(CB_NO_CALL_BENKER, 0xFF);
				return true;
			}

			//超时警告
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_ROD_BANKER	:	//抢地主时器
		{
			//更新整个界面
			//m_GameClientView.InvalidGameView(0,0,0,0);

			//自动处理
			if (( nElapse <= 0 ) || ( m_bTrustee && (int)nElapse < (int)m_cbTimeRodBanker - 1 ))
			{
				if ( !IsLookonMode() && wChairID==GetMeChairID() ) 
					OnMessageRodBanker(CB_NO_ROD_BANKER, 0xFF);
				return true;
			}

			//超时警告
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	//case IDI_CALL_SCORE:	//用户叫分
	//	{
	//		////更新整个界面
	//		////m_GameClientView.InvalidGameView(0,0,0,0);

	//		//自动处理
	//		if (( nElapse <= 0 ) || ( m_bTrustee && (int)nElapse < (int)m_cbTimeOutCard - 1 ))
	//		{
	//			if ( !IsLookonMode() && wChairID==GetMeChairID() ) 
	//				OnMessageCallScore(0,0);
	//			return true;
	//		}

	//		//超时警告
	//		if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
	//		{
	//			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
	//		}

	//		return true;
	//	}
	}
	return false;
}

//旁观消息
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//旁观控制
	bool bAllowLookon=IsAllowLookon();

	//扑克控制
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetDisplayItem(bAllowLookon);

	m_GameClientView.m_btSortCard.EnableWindow(bAllowLookon?TRUE:FALSE);
	if( bAllowLookon )
		UpdateButtonControl();
	else
	{
		m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);
	}

	if ( IsLookonMode() )
	{
		m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

		m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
		m_GameClientView.m_btTrustee.EnableWindow(FALSE);
		m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
		m_GameClientView.m_btSortCard.EnableWindow(FALSE);
	}

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);


	return true;
}

//游戏消息
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_CALL_BANKER:		//用户叫地主
		{
			return OnSubCallBanker(pData,wDataSize);
		}
	case SUB_S_ROD_BANKER:		//用户抢地主
		{
			return OnSubRodBanker(pData,wDataSize);
		}
	//case SUB_S_CALL_SCORE:		//用户叫分
	//	{
	//		return OnSubCallScore(pData,wDataSize);
	//	}
	case SUB_S_BANKER_INFO:		//庄家信息
		{
			return OnSubBankerInfo(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//用户出牌
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:		//用户放弃
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_OUT_START_START:		//出牌开始
		{
			return OnSubOutCardStart(pData,wDataSize);
		}
	case SUB_S_GAME_CONCLUDE:	//游戏结束
		{
			return OnSubGameConclude(pData,wDataSize);
		}
	case SUB_S_SET_BASESCORE:	//设置基数
		{
			//效验数据
			ASSERT(wDataSize==sizeof(LONG));
			if(wDataSize!=sizeof(LONG)) return false;

			LONG lCellScore=*(LONG*)pData;
			m_GameClientView.SetCellScore(lCellScore);
			return true;
		}
	case SUB_S_CHEAT_CARD:		//作弊数据	
		{
			return OnSubCheatCard(pData,wDataSize);
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//场景消息
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//空闲状态
		{
			OutputDebugStringA("zrh client 有接受到空闲状态消息，设置按钮状态");
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//变量定义
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//设置界面
			m_GameClientView.m_btScore.ShowWindow(FALSE);
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);

			//计算倍数
			SetCurrentTime();

			//时间定义
			m_cbTimeCallBanker=pStatusFree->cbTimeCallBanker;
			m_cbTimeRodBanker=pStatusFree->cbTimeRodBanker;
			m_cbTimeOutCard=pStatusFree->cbTimeOutCard;
			//m_cbTimeCallScore=pStatusFree->cbTimeCallScore;
			m_cbTimeStartGame=pStatusFree->cbTimeStartGame;
			m_cbTimeHeadOutCard=pStatusFree->cbTimeHeadOutCard;
			//玩家设置
			if (IsLookonMode()==false)
			{
				//托管按钮
				m_GameClientView.m_btTrustee.ShowWindow(true);
				m_GameClientView.m_btCancelTrustee.ShowWindow(false);
				m_GameClientView.m_btTrustee.EnableWindow(true);

				//开始设置
				if (GetMeUserItem()->GetUserStatus()!=US_READY)
				{
					m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
					
					PerformAutoStart();
				}
			}

			//历史成绩
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//获取变量
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置积分
				pHistoryScore->lTurnScore=pStatusFree->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusFree->lCollectScore[i];

				//绑定设置
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			//旁观设置界面
			if ( IsLookonMode() )
			{
				m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

				m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
				m_GameClientView.m_btSortCard.EnableWindow(FALSE);
			}

			return true;
		}
	case GAME_SCENE_CALL:	//叫地主状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusCall));
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;

			//变量定义
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

			//时间定义
			m_cbTimeCallBanker=pStatusCall->cbTimeCallBanker;
			m_cbTimeRodBanker=pStatusCall->cbTimeRodBanker;
			//m_cbTimeAddDouble=pStatusCall->cbTimeAddDouble;
			m_cbTimeStartGame=pStatusCall->cbTimeStartGame;
			m_cbTimeHeadOutCard=pStatusCall->cbTimeHeadOutCard;
			m_cbTimeOutCard=pStatusCall->cbTimeOutCard;

			//设置变量
			m_wCurrentUser=pStatusCall->wCurrentUser;			
			//m_wFirstValidUser = pStatusCall->wFirstValidUser;

			m_wStartTime=pStatusCall->wStartTime;
			//m_wValidCardTime=pStatusCall->wValidCardTime;

			CopyMemory(m_cbCallBankerInfo, pStatusCall->cbCallBankerInfo, sizeof(pStatusCall->cbCallBankerInfo));
		//	CopyMemory(m_bValidCardInfo, pStatusCall->bValidCardInfo, sizeof(pStatusCall->bValidCardInfo));

	

			//扑克数据
			//for (WORD i=0;i<GAME_PLAYER;i++)
			//{
			//	m_cbHandCardCount[i]=NORMAL_COUNT;

			//	CopyMemory(m_cbHandCardData[i],pStatusCall->cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

			//	OutputDebugStringA("ddz client牌数据");
			//	

			//	//设置扑克
			//	WORD wViewChairID=SwitchViewChairID(i);	
			//	/*if (m_bValidCardInfo[i])
			//	{
			//		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
			//		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			//		m_GameClientView.UserValidCard(wViewChairID, m_cbHandCardData[i],m_cbHandCardCount[i]);

			//	}*/
			//	 if (wViewChairID==MYSELF_VIEW_ID)
			//	{
			//		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
			//		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[i],m_cbHandCardCount[i]);
			//	}
			//	else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			//}
				/*WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);	
				CString str;
				str.Format(L"ddz callbank阶段的wViewChairID==%d",wViewChairID);
				OutputDebugString(str);*/
			//WORD wMeChairID=GetMeChairID();
			//CopyMemory(m_cbHandCardData[wMeChairID],pStatusCall->cbHandCardData,NORMAL_COUNT*sizeof(BYTE));		
			//扑克数据
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHandCardCount[i]=NORMAL_COUNT;

				CopyMemory(m_cbHandCardData[i],pStatusCall->cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//设置扑克
				WORD wViewChairID=SwitchViewChairID(i);	
				
				/*if (m_bValidCardInfo[i])
				{
					m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
					m_GameClientView.UserValidCard(wViewChairID, m_cbHandCardData[i],m_cbHandCardCount[i]);

				}
				else */if (wViewChairID==MYSELF_VIEW_ID)
				{
					m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[i],m_cbHandCardCount[i]);
				}
				else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			}


			//for (WORD i=0;i<GAME_PLAYER;i++)
			//{
			//	WORD wViewChairID=SwitchViewChairID(i);	
			//	if (wViewChairID==1)
			//	{
			//		for (WORD j=0;j<MAX_COUNT;j++)
			//		{
			//			CString str;
			//			str.Format(L"ddz client callbank牌数据%d",pStatusCall->cbHandCardData[i][j]);
			//			OutputDebugString(str);
			//		}
			//	}
			//

			//}



			// 设置底牌
			m_GameClientView.m_BackCardControl.SetCardData(3);

			//设置界面
			m_GameClientView.m_btScore.ShowWindow(FALSE);
			m_GameClientView.SetCellScore(pStatusCall->lCellScore); 

			CString str;
			str.Format(L"zzz 初始倍数是m_wStartTime==%d",m_wStartTime);
			OutputDebugString(str);

			//计算倍数
			SetCurrentTime();

			//叫地主界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (pStatusCall->cbCallBankerInfo[i]!=CB_NOT_CALL)
				{
					m_GameClientView.SetUserCallBanker(SwitchViewChairID(i),pStatusCall->cbCallBankerInfo[i]);
				}
			}

			//叫地主设置
			WORD wMeChairID=GetMeChairID();
			if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID) && CB_NOT_CALL == m_cbCallBankerInfo[wMeChairID])
			{
				//显示按钮
				m_GameClientView.m_btCallBanker.ShowWindow(true);
				m_GameClientView.m_btNoCallBanker.ShowWindow(true);				
			}

			//玩家控制
			if (IsLookonMode()==false)	
			{
				m_GameClientView.m_btTrustee.ShowWindow(true);
				m_GameClientView.m_btCancelTrustee.ShowWindow(false);
				m_GameClientView.m_btTrustee.EnableWindow(true);
				m_GameClientView.m_btSortCard.EnableWindow(true);
				UpdateButtonControl();
			}

			//显示设置
			bool bDisplayItem=((IsLookonMode()==false)||(IsAllowLookon()==true) /*|| m_bValidCardInfo[wMeChairID]*/);
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetDisplayItem(bDisplayItem);

			//历史成绩
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//获取变量
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置积分
				pHistoryScore->lTurnScore=pStatusCall->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusCall->lCollectScore[i];

				//绑定设置
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			//设置时间
			SetGameClock(m_wCurrentUser,IDI_CALL_BANKER,m_cbTimeCallBanker);

			//清楚当前操作
			m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			m_GameClientView.SetUserCallBanker(SwitchViewChairID(m_wCurrentUser), CB_NOT_CALL);

			//旁观设置界面
			if ( IsLookonMode() )
			{
				m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

				m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
				m_GameClientView.m_btSortCard.EnableWindow(FALSE);

				m_GameClientView.m_btCallBanker.ShowWindow(false);
				m_GameClientView.m_btNoCallBanker.ShowWindow(false);
				m_GameClientView.m_btRodBanker.ShowWindow(false);
				m_GameClientView.m_btNoRodBanker.ShowWindow(false);
				/*m_GameClientView.m_btAddDouble.ShowWindow(false);
				m_GameClientView.m_btNoAddDouble.ShowWindow(false);
				m_GameClientView.m_btValidCard.ShowWindow(false);*/
			}


			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GAME_SCENE_ROD:	//抢地主状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusRod));
			if (wDataSize!=sizeof(CMD_S_StatusRod)) return false;

			//变量定义
			CMD_S_StatusRod * pStatusRod=(CMD_S_StatusRod *)pData;

			//时间定义
			m_cbTimeCallBanker=pStatusRod->cbTimeCallBanker;
			m_cbTimeRodBanker=pStatusRod->cbTimeRodBanker;
			//m_cbTimeAddDouble=pStatusRod->cbTimeAddDouble;
			m_cbTimeStartGame=pStatusRod->cbTimeStartGame;
			m_cbTimeHeadOutCard=pStatusRod->cbTimeHeadOutCard;
			m_cbTimeOutCard=pStatusRod->cbTimeOutCard;

			//设置变量
			m_wCurrentUser=pStatusRod->wCurrentUser;
			//m_wFirstValidUser = pStatusRod->wFirstValidUser;

			m_wStartTime = pStatusRod->wStartTime;
			//m_wValidCardTime = pStatusRod->wValidCardTime;
			m_wRodBankerTime = pStatusRod->wRodBankerTime;

			//游戏信息
//			CopyMemory(m_bValidCardInfo, pStatusRod->bValidCardInfo, sizeof(pStatusRod->bValidCardInfo));
			CopyMemory(m_cbCallBankerInfo, pStatusRod->cbCallBankerInfo, sizeof(pStatusRod->cbCallBankerInfo));			
			CopyMemory(m_cbRodBankerInfo,pStatusRod->cbRodBankerInfo,sizeof(pStatusRod->cbRodBankerInfo));

			//扑克数据
				WORD wMeChairID=GetMeChairID();
			CopyMemory(m_cbHandCardData[wMeChairID],pStatusRod->cbHandCardData,NORMAL_COUNT*sizeof(BYTE));		

		
			//CopyMemory(m_cbHandCardCount,NORMAL_COUNT,sizeof(m_cbHandCardCount));

			//for (WORD i=0;i<GAME_PLAYER;i++)
			//{
			//	m_cbHandCardCount[i]=NORMAL_COUNT;
			//	CopyMemory(m_cbHandCardData[i],pStatusRod->cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

			//	//获取位置
			//	WORD wViewChairID=SwitchViewChairID(i);

			//	//设置扑克
			//	/*if (m_bValidCardInfo[i])
			//	{
			//		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
			//		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			//		m_GameClientView.UserValidCard(wViewChairID, m_cbHandCardData[i],m_cbHandCardCount[i]);

			//	}*/
			//	 if (wViewChairID==MYSELF_VIEW_ID)
			//	{
			//		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
			//		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[i],m_cbHandCardCount[i]);
			//	}
			//	else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			//}
			//扑克数据

		
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHandCardCount[i]=NORMAL_COUNT;
				CopyMemory(m_cbHandCardData[i],pStatusRod->cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//获取位置
				WORD wViewChairID=SwitchViewChairID(i);

				//设置扑克
				/*if (m_bValidCardInfo[i])
				{
					m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
					m_GameClientView.UserValidCard(wViewChairID, m_cbHandCardData[i],m_cbHandCardCount[i]);

				}
				else*/ if (wViewChairID==MYSELF_VIEW_ID)
				{
					m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[i],m_cbHandCardCount[i]);
				}
				else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			}

		
			//设置底牌
			m_GameClientView.m_BackCardControl.SetCardData(3);

			//设置界面
			m_GameClientView.m_btScore.ShowWindow(FALSE);
			m_GameClientView.SetCellScore(pStatusRod->lCellScore);

			//计算倍数
			SetCurrentTime();

			//地主界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (pStatusRod->cbRodBankerInfo[i]!=CB_NOT_ROD)
				{
					m_GameClientView.SetUserRodBanker(SwitchViewChairID(i),pStatusRod->cbRodBankerInfo[i]);
				}
				else
				{
					m_GameClientView.SetUserCallBanker(SwitchViewChairID(i),pStatusRod->cbCallBankerInfo[i]);
				}
			}

			//抢地主设置

			if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID)&&CB_NOT_ROD == m_cbRodBankerInfo[wMeChairID])
			{
				//显示按钮
				m_GameClientView.m_btRodBanker.ShowWindow(true);
				m_GameClientView.m_btNoRodBanker.ShowWindow(true);				
			}

			//玩家控制
			if (IsLookonMode()==false)	
			{
				m_GameClientView.m_btTrustee.ShowWindow(true);
				m_GameClientView.m_btCancelTrustee.ShowWindow(false);
				m_GameClientView.m_btTrustee.EnableWindow(true);
				m_GameClientView.m_btSortCard.EnableWindow(true);
				UpdateButtonControl();
			}

			//显示设置
			bool bDisplayItem=((IsLookonMode()==false)||(IsAllowLookon()==true)/* || m_bValidCardInfo[wMeChairID]*/);
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetDisplayItem(bDisplayItem);

			//剩余提醒
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_cbHandCardCount[i]<=2)
				{
					m_GameClientView.SetUserCountWarn(SwitchViewChairID(i),true);
				}
			}

			//历史成绩
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//获取变量
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置积分
				pHistoryScore->lTurnScore=pStatusRod->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusRod->lCollectScore[i];

				//绑定设置
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			//设置时间
			SetGameClock(m_wCurrentUser,IDI_ROD_BANKER,m_cbTimeRodBanker);

			//清楚当前操作
			m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			m_GameClientView.SetUserCallBanker(SwitchViewChairID(m_wCurrentUser), CB_NOT_CALL);

			//旁观设置界面
			if ( IsLookonMode() )
			{
				m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

				m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
				m_GameClientView.m_btSortCard.EnableWindow(FALSE);

				m_GameClientView.m_btCallBanker.ShowWindow(false);
				m_GameClientView.m_btNoCallBanker.ShowWindow(false);
				m_GameClientView.m_btRodBanker.ShowWindow(false);
				m_GameClientView.m_btNoRodBanker.ShowWindow(false);
				//m_GameClientView.m_btAddDouble.ShowWindow(false);
				//m_GameClientView.m_btNoAddDouble.ShowWindow(false);
				//m_GameClientView.m_btValidCard.ShowWindow(false);
			}

			//设置界面
			m_GameClientView.m_btScore.ShowWindow(FALSE);

			return true;
		}


	//case GAME_SCENE_CALL:	//叫分状态
	//	{
	//		//效验数据
	//		ASSERT(wDataSize==sizeof(CMD_S_StatusCall));
	//		if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;

	//		//变量定义
	//		CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

	//		//设置变量
	//		m_wCurrentUser=pStatusCall->wCurrentUser;

	//		//时间定义
	//		m_cbTimeOutCard=pStatusCall->cbTimeOutCard;
	//		m_cbTimeCallScore=pStatusCall->cbTimeCallScore;
	//		m_cbTimeStartGame=pStatusCall->cbTimeStartGame;
	//		m_cbTimeHeadOutCard=pStatusCall->cbTimeHeadOutCard;

	//		//扑克数据
	//		WORD wMeChairID=GetMeChairID();
	//		for (WORD i=0;i<GAME_PLAYER;i++) m_cbHandCardCount[i]=NORMAL_COUNT;
	//		CopyMemory(m_cbHandCardData[wMeChairID],pStatusCall->cbHandCardData,m_cbHandCardCount[wMeChairID]*sizeof(BYTE));

	//		//叫分状态
	//		m_GameClientView.SetWaitCallScore(true);
	//		m_GameClientView.m_BackCardControl.SetCardData(3);

	//		//设置界面
	//		m_GameClientView.m_btScore.ShowWindow(true);
	//		m_GameClientView.SetCellScore(pStatusCall->lCellScore);

	//		//叫分界面
	//		for (WORD i=0;i<GAME_PLAYER;i++)
	//		{
	//			if (pStatusCall->cbScoreInfo[i]!=0)
	//			{
	//				BYTE cbCallScore=pStatusCall->cbScoreInfo[i];
	//				m_GameClientView.SetUserCallScore(SwitchViewChairID(i),pStatusCall->cbScoreInfo[i]);
	//			}
	//		}

	//		//设置扑克
	//		for (WORD i=0;i<GAME_PLAYER;i++)
	//		{
	//			//获取位置
	//			WORD wViewChairID=SwitchViewChairID(i);

	//			//设置扑克
	//			if (wViewChairID==MYSELF_VIEW_ID)
	//			{
	//				m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
	//				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[i],m_cbHandCardCount[i]);
	//			}
	//			else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
	//		}

	//		//叫分设置
	//		if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	//		{
	//			//显示按钮
	//		/*	m_GameClientView.m_btCallScore1.ShowWindow(true);
	//			m_GameClientView.m_btCallScore2.ShowWindow(true);
	//			m_GameClientView.m_btCallScore3.ShowWindow(true);*/
	//			m_GameClientView.m_btCallScoreNone.ShowWindow(true);
	//			m_GameClientView.m_btCallBanker.ShowWindow(true);

	//			//禁用按钮
	//			/*m_GameClientView.m_btCallScore1.EnableWindow((pStatusCall->cbBankerScore==0)?true:false);
	//			m_GameClientView.m_btCallScore2.EnableWindow((pStatusCall->cbBankerScore<=1)?true:false);
	//			m_GameClientView.m_btCallScore3.EnableWindow((pStatusCall->cbBankerScore<=2)?true:false);*/

	//			UpdateButtonControl();
	//		}

	//		//玩家控制
	//		if (IsLookonMode()==false)	
	//		{
	//			m_GameClientView.m_btTrustee.ShowWindow(true);
	//			m_GameClientView.m_btCancelTrustee.ShowWindow(false);
	//			m_GameClientView.m_btTrustee.EnableWindow(true);
	//			m_GameClientView.m_btSortCard.EnableWindow(true);
	//		}

	//		//显示设置
	//		bool bDisplayItem=((IsLookonMode()==false)||(IsAllowLookon()==true));
	//		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetDisplayItem(bDisplayItem);

	//		//剩余提醒
	//		for (WORD i=0;i<GAME_PLAYER;i++)
	//		{
	//			if (m_cbHandCardCount[i]<=2)
	//			{
	//				m_GameClientView.SetUserCountWarn(SwitchViewChairID(i),true);
	//			}
	//		}

	//		//历史成绩
	//		for (BYTE i=0;i<GAME_PLAYER;i++)
	//		{
	//			//获取变量
	//			tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

	//			//设置积分
	//			pHistoryScore->lTurnScore=pStatusCall->lTurnScore[i];
	//			pHistoryScore->lCollectScore=pStatusCall->lCollectScore[i];

	//			//绑定设置
	//			WORD wViewChairID=SwitchViewChairID(i);
	//			m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
	//		}

	//		//设置时间
	//		SetGameClock(m_wCurrentUser,IDI_CALL_SCORE,m_cbTimeCallScore);

	//		//清楚当前操作
	//		m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
	//		m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
	//		m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

	//		//旁观设置界面
	//		if ( IsLookonMode() )
	//		{
	//			m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
	//			m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
	//			m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
	//			m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
	//			m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
	//			m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

	//			m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
	//			m_GameClientView.m_btTrustee.EnableWindow(FALSE);
	//			m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
	//			m_GameClientView.m_btSortCard.EnableWindow(FALSE);
	//		}

	//		m_GameClientView.InvalidGameView(0,0,0,0);

	//		return true;
	//	}
	case GAME_SCENE_PLAY:	//游戏状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//变量定义
			WORD wMeChairID=GetMeChairID();
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//设置变量
			m_cbBombCount=pStatusPlay->cbBombCount;
			m_wBankerUser=pStatusPlay->wBankerUser;
			//m_cbBankerScore = pStatusPlay->cbBankerScore;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_wStartTime = pStatusPlay->wStartTime;
			m_wRodBankerTime = pStatusPlay->wRodBankerTime;
			m_wBombTime = pStatusPlay->wBombTime;

			//时间定义
			m_cbTimeCallBanker=pStatusPlay->cbTimeCallBanker;
			m_cbTimeRodBanker=pStatusPlay->cbTimeRodBanker;
			m_cbTimeOutCard=pStatusPlay->cbTimeOutCard;
			//m_cbTimeCallScore=pStatusPlay->cbTimeCallScore;
			m_cbTimeStartGame=pStatusPlay->cbTimeStartGame;
			m_cbTimeHeadOutCard=pStatusPlay->cbTimeHeadOutCard;

			//出牌变量
			m_cbTurnCardCount=pStatusPlay->cbTurnCardCount;
			CopyMemory(m_cbTurnCardData,pStatusPlay->cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//扑克数据
			CopyMemory(m_cbHandCardCount,pStatusPlay->cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(m_cbHandCardData[wMeChairID],pStatusPlay->cbHandCardData,m_cbHandCardCount[wMeChairID]*sizeof(BYTE));

			//设置牌面
			WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
			m_GameClientView.m_HandCardControl[wViewChairID].SetBackGround(CARD_LAND);

			//设置界面
			m_GameClientView.SetBombCount(m_cbBombCount);
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);


			//扑克数据
			CopyMemory(m_cbHandCardCount,pStatusPlay->cbHandCardCount, sizeof(BYTE) * sizeof(m_cbHandCardCount));
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				CopyMemory(m_cbHandCardData[i],pStatusPlay->cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//获取位置
				WORD wViewChairID=SwitchViewChairID(i);
				//设置扑克
				/*if (m_bValidCardInfo[i])
				{
					m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
					m_GameClientView.UserValidCard(wViewChairID, m_cbHandCardData[i],m_cbHandCardCount[i]);

				}
				else*/ if (wViewChairID==MYSELF_VIEW_ID)
				{
					m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[i],m_cbHandCardCount[i]);
				}
				else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			}
			//设置扑克
			//for (WORD i=0;i<GAME_PLAYER;i++)
			//{
			//	//获取位置
			//	WORD wViewChairID=SwitchViewChairID(i);

			//	//设置扑克
			//	if (wViewChairID==MYSELF_VIEW_ID)
			//	{
			//		m_GameLogic.SortCardList(m_cbHandCardData[wMeChairID],m_cbHandCardCount[i],m_cbSortType);
			//		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[wMeChairID],m_cbHandCardCount[i]);
			//	}
			//	else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			//}

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (WORD j=0;j<MAX_COUNT;j++)
				{
					CString str;
					str.Format(L"ddz client game牌数据%d",pStatusPlay->cbHandCardData[i][j]);
					OutputDebugString(str);
				}
				
			}

			//计算倍数
			SetCurrentTime();

			//庄家设置
			m_GameClientView.SetBankerUser(wViewChairID);
			//m_GameClientView.SetBankerScore(pStatusPlay->cbBankerScore);

			//控制界面
			m_GameClientView.m_btScore.ShowWindow(FALSE);
			m_GameClientView.m_BackCardControl.SetCardData(pStatusPlay->cbBankerCard,3);

			//出牌界面
			if (pStatusPlay->wTurnWiner!=INVALID_CHAIR)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wTurnWiner);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbTurnCardData,m_cbTurnCardCount);
			}

			//玩家控制
			if (IsLookonMode()==false)	
			{
				m_GameClientView.m_btTrustee.ShowWindow(true);
				m_GameClientView.m_btCancelTrustee.ShowWindow(false);
				m_GameClientView.m_btTrustee.EnableWindow(true);
				m_GameClientView.m_btSortCard.EnableWindow(true);

				UpdateButtonControl();
			}

			//控制设置
			bool bPositively=(IsLookonMode()==false);
			bool bDisplayItem=((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(bPositively);
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetDisplayItem(bDisplayItem);

			//当前玩家
			if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
			{
				//显示按钮
				m_GameClientView.m_btOutCard.ShowWindow(true);
				m_GameClientView.m_btPassCard.ShowWindow(true);
				m_GameClientView.m_btOutPrompt.ShowWindow(true);

				//
				m_GameClientView.m_btCallBanker.ShowWindow(false);
				//m_GameClientView.m_btCallScoreNone.ShowWindow(false);



				//启用按钮
				m_GameClientView.m_btPassCard.EnableWindow((m_cbTurnCardCount>0)?true:false);
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);

				//搜索出牌
				if( pStatusPlay->wTurnWiner == wMeChairID )
				{
					m_GameLogic.SearchOutCard( m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID],NULL,0,&m_SearchCardResult );
				}
				else
				{
					m_GameLogic.SearchOutCard( m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID],m_cbTurnCardData,
						m_cbTurnCardCount,&m_SearchCardResult );
				}
			}
			//else if ( m_wCurrentUser != INVALID_CHAIR )
			//{
			//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			//}

			//历史成绩
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//获取变量
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置积分
				pHistoryScore->lTurnScore=pStatusPlay->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusPlay->lCollectScore[i];

				//绑定设置
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			//设置时间
			SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeOutCard);

			//清楚当前操作
			m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			m_GameClientView.SetUserCallBanker(SwitchViewChairID(m_wCurrentUser), CB_NOT_CALL);
			m_GameClientView.SetUserRodBanker(SwitchViewChairID(m_wCurrentUser), CB_NOT_ROD);
//			m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

			//旁观设置界面
			if ( IsLookonMode() )
			{
				m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

				m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
				m_GameClientView.m_btSortCard.EnableWindow(FALSE);
			}

			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//游戏开始
bool CGameClientEngine::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//设置状态
	SetGameStatus(GAME_SCENE_CALL);

	//搜索变量
	m_cbSearchResultIndex = 0;
	ZeroMemory( &m_SearchCardResult,sizeof(m_SearchCardResult) );
	m_nCurSearchType = -1;
	m_cbEachSearchIndex = 0;
	ZeroMemory( &m_EachSearchResult,sizeof(m_EachSearchResult) );

	//游戏变量
	m_bTrusteeCount = 0;
	m_wRodBankerTime=1;
	m_cbBombCount=0;

	m_wBombTime=1;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));
	//m_cbBankerScore = 0;
	m_wCurrentUser=pGameStart->wCurrentUser;

	m_wStartTime=pGameStart->wStartTime;
	//出牌变量
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//计算倍数
	SetCurrentTime();

	//历史记录
	m_HistoryCard.ResetData();

	//界面庄家
	//m_GameClientView.SetBankerScore(0);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);

	//成绩窗口
	m_GameClientView.m_ScoreControl.CloseControl();

	//状态设置
	m_GameClientView.SetBombCount(0);
	m_GameClientView.SetWaitCallScore(false);
	//m_GameClientView.SetUserCallScore(INVALID_CHAIR,0);
	m_GameClientView.SetUserCallBanker(INVALID_CHAIR,CB_NOT_CALL);
	m_GameClientView.SetUserPassState(INVALID_CHAIR,false);
	m_GameClientView.SetUserCountWarn(INVALID_CHAIR,false);

	//玩家扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取位置
		WORD wViewChairID=SwitchViewChairID(i);

		//扑克背面
		m_GameClientView.m_HandCardControl[i].SetBackGround(CARD_BOOR);
		m_GameClientView.m_UserCardControl[i].SetBackGround(CARD_BOOR);

		//设置扑克
		if (wViewChairID==MYSELF_VIEW_ID)
		{
			bool bShowCard=((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetPositively(false);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(bShowCard);
		}
		else
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(false);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardDistance(DEF_X_DISTANCE,11,0);
		}
	}

	//底牌扑克
	m_GameClientView.m_BackCardControl.SetCardData(3);
	m_GameClientView.m_BackCardControl.SetBackGround(CARD_BOOR);

	//设置扑克
	WORD wMeChairID=GetMeChairID();
	for (WORD i=0;i<GAME_PLAYER;i++) m_cbHandCardCount[i]=CountArray(pGameStart->cbCardData);
	CopyMemory(m_cbHandCardData[wMeChairID],pGameStart->cbCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//发牌动画
	WORD wViewStartUser=SwitchViewChairID(pGameStart->wStartUser);
	m_GameClientView.ShowDispatchCard(m_cbHandCardData[wMeChairID],wViewStartUser);

	//环境设置
	if ((IsLookonMode()==false)&&(m_bTrustee==false)) ActiveGameFrame();

	//隐藏控件
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//更新界面
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	m_GameClientView.m_btStart.ShowWindow(false);//zrh


	return true;
}
//用户叫地主
bool CGameClientEngine::OnSubCallBanker(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_CallBanker));
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;

	//停止发牌
	m_GameClientView.StopDispatchCard();

	//变量定义
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pData;

	//叫地主界面
	WORD wViewChairID=SwitchViewChairID(pCallBanker->wLastUser);
	m_cbCallBankerInfo[wViewChairID] = pCallBanker->cbCallInfo;
	m_GameClientView.SetUserCallBanker(wViewChairID, pCallBanker->cbCallInfo);

	//排列扑克
	m_GameLogic.SortCardList(m_cbHandCardData[GetMeChairID()],m_cbHandCardCount[GetMeChairID()],ST_ORDER);

	//设置扑克
	m_GameClientView.m_HandCardControl[SwitchViewChairID(GetMeChairID())].SetCardData(m_cbHandCardData[GetMeChairID()],m_cbHandCardCount[GetMeChairID()]);
	OutputDebugStringA("ddz  111叫地主阶段的定时器！！！");

	if (pCallBanker->cbCallInfo != CB_CALL_BENKER)
	{
		//叫地主设置
		if (/*(IsLookonMode()==false)&&*/(pCallBanker->wCurrentUser==GetMeChairID()))
		{
			//激活框架
			if (m_bTrustee==false) ActiveGameFrame();

			//显示按钮
			m_GameClientView.m_btCallBanker.ShowWindow(true);
			m_GameClientView.m_btNoCallBanker.ShowWindow(true);
			//设置时间
			SetGameClock(pCallBanker->wCurrentUser,IDI_CALL_BANKER,m_cbTimeCallBanker);

			OutputDebugStringA("ddz  222叫地主阶段的定时器！！！");
			CString str;
			str.Format(L"ddz 555叫地主阶段的定时器时间是%d",m_cbTimeCallBanker);
			OutputDebugString(str);
		}
	}

	//获取用户
	IClientUserItem * pIClientUserItem=GetTableUserItem(pCallBanker->wLastUser);
	BYTE cbGender=pIClientUserItem->GetGender();

	if (CB_CALL_BENKER == pCallBanker->cbCallInfo)
	{
		m_wBankerUser = pCallBanker->wLastUser;
		SetGameStatus(GAME_SCENE_ROD);		

		//播放声音
		PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("CALL_BANKER_GIRL"):TEXT("CALL_BANKER_BOY"));
	}
	else
	{
		//播放声音
		PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("NO_CALL_BANKER_GIRL"):TEXT("NO_CALL_BANKER_BOY"));
	}

	if (pCallBanker->wCurrentUser!=INVALID_CHAIR)
	{
		if (pCallBanker->wCurrentUser != GetMeChairID())
		{
			//设置时间
			SetGameClock(pCallBanker->wCurrentUser,IDI_CALL_BANKER,m_cbTimeCallBanker);
		}
		//清楚当前操作
		m_GameClientView.m_UserCardControl[SwitchViewChairID(pCallBanker->wCurrentUser)].SetCardData(0);
		m_GameClientView.SetUserPassState(SwitchViewChairID(pCallBanker->wCurrentUser), false);
		m_GameClientView.SetUserCallBanker(SwitchViewChairID(pCallBanker->wCurrentUser), CB_NOT_CALL);
		m_GameClientView.SetUserRodBanker(SwitchViewChairID(pCallBanker->wCurrentUser), CB_NOT_ROD);
//		m_GameClientView.SetUserAddDouble(SwitchViewChairID(pCallBanker->wCurrentUser), CB_NOT_ADD_DOUBLE);

		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return true;
}

//用户抢地主
bool CGameClientEngine::OnSubRodBanker(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_RodBanker));
	if (wDataSize!=sizeof(CMD_S_RodBanker)) return false;

	//变量定义
	CMD_S_RodBanker * pRodBanker=(CMD_S_RodBanker *)pData;

	m_wBankerUser = pRodBanker->wBankerUser;
	m_wRodBankerTime = pRodBanker->wRodBankerTime;

	//计算倍数
	SetCurrentTime();

	//停止发牌
	m_GameClientView.StopDispatchCard();

	if (pRodBanker->wLastUser != INVALID_CHAIR)
	{	
		//抢地主界面
		WORD wViewChairID=SwitchViewChairID(pRodBanker->wLastUser);
		m_GameClientView.SetUserRodBanker(wViewChairID, pRodBanker->cbRodlInfo);

		//获取用户
		IClientUserItem * pIClientUserItem=GetTableUserItem(pRodBanker->wLastUser);
		BYTE cbGender=pIClientUserItem->GetGender();

		if (CB_ROD_BANKER == pRodBanker->cbRodlInfo)
		{
			m_wBankerUser = pRodBanker->wLastUser;
//			m_GameClientView.ShowCartoonAddDouble(true, 2);

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("ROD_BANKER_GIRL"):TEXT("ROD_BANKER_BOY"));
		}
		else
		{	
			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("NO_ROD_BANKER_GIRL"):TEXT("NO_ROD_BANKER_BOY"));
		}	
	}

	//抢地主设置
	if ((IsLookonMode()==false)&&(pRodBanker->wCurrentUser==GetMeChairID()))
	{
		//激活框架
		if (m_bTrustee==false) ActiveGameFrame();

		if (( m_wBankerUser != INVALID_CHAIR && pRodBanker->wCurrentUser != m_wBankerUser) 
			|| (m_wBankerUser == INVALID_CHAIR && INVALID_CHAIR != m_wFirstValidUser) && pRodBanker->wCurrentUser != m_wFirstValidUser)
		{
			//显示按钮
			m_GameClientView.m_btRodBanker.ShowWindow(true);
			m_GameClientView.m_btNoRodBanker.ShowWindow(true);

			//设置时间
			SetGameClock(pRodBanker->wCurrentUser,IDI_ROD_BANKER,m_cbTimeRodBanker);
		}		
	}

	//设置时间
	if (pRodBanker->wCurrentUser!=INVALID_CHAIR)
	{
		if (pRodBanker->wCurrentUser != GetMeChairID())
		{
			//设置时间
			SetGameClock(pRodBanker->wCurrentUser,IDI_ROD_BANKER,m_cbTimeRodBanker);
		}

		//清楚当前操作
		m_GameClientView.m_UserCardControl[SwitchViewChairID(pRodBanker->wCurrentUser)].SetCardData(0);
		m_GameClientView.SetUserPassState(SwitchViewChairID(pRodBanker->wCurrentUser), false);
		m_GameClientView.SetUserCallBanker(SwitchViewChairID(pRodBanker->wCurrentUser), CB_NOT_CALL);
		m_GameClientView.SetUserRodBanker(SwitchViewChairID(pRodBanker->wCurrentUser), CB_NOT_ROD);
//		m_GameClientView.SetUserAddDouble(SwitchViewChairID(pRodBanker->wCurrentUser), CB_NOT_ADD_DOUBLE);

		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return true;
}


////用户叫分
//bool CGameClientEngine::OnSubCallScore(VOID * pData, WORD wDataSize)
//{
//	//效验参数
//	ASSERT(wDataSize==sizeof(CMD_S_CallScore));
//	if (wDataSize!=sizeof(CMD_S_CallScore)) return false;
//
//	//变量定义
//	CMD_S_CallScore * pCallScore=(CMD_S_CallScore *)pData;
//
//	//停止发牌
//	m_GameClientView.StopDispatchCard();
//
//	//叫分界面
//	WORD wViewChairID=SwitchViewChairID(pCallScore->wCallScoreUser);
//	m_GameClientView.SetUserCallScore(wViewChairID,pCallScore->cbUserCallScore);
//
//	if ( pCallScore->cbUserCallScore != 255 )
//	{
//		m_cbBankerScore = max(m_cbBankerScore, pCallScore->cbUserCallScore);
//	}
//
//	WORD wMeChairID=GetMeChairID();
//
//	//排列扑克
//	m_GameLogic.SortCardList(m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID],ST_ORDER);
//
//	//设置扑克
//	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID]);
//	
//	//叫地主设置
//	if ((IsLookonMode()==false)&&(pCallScore->wCurrentUser==GetMeChairID())&&(pCallScore->cbUserCallScore==0)&&pCallScore->cbCurrentScore!=1)
//	{
//		//激活框架
//		if (m_bTrustee==false) ActiveGameFrame();
//
//		//显示按钮
//		/*m_GameClientView.m_btCallScore1.ShowWindow(true);
//		m_GameClientView.m_btCallScore2.ShowWindow(true);
//		m_GameClientView.m_btCallScore3.ShowWindow(true);*/
//
//		m_GameClientView.m_btCallScoreNone.ShowWindow(true);
//		m_GameClientView.m_btCallBanker.ShowWindow(true);
//		m_GameClientView.m_btRodBanker.ShowWindow(false);
//		m_GameClientView.m_btNoRodBanker.ShowWindow(false);
//
//		////禁用按钮
//		//m_GameClientView.m_btCallScore1.EnableWindow((pCallScore->cbCurrentScore==0)?true:false);
//		//m_GameClientView.m_btCallScore2.EnableWindow((pCallScore->cbCurrentScore<=1)?true:false);
//		//m_GameClientView.m_btCallScore3.EnableWindow((pCallScore->cbCurrentScore<=2)?true:false);
//	}
//	//	抢地主设置
//	if ((IsLookonMode()==false)&&(pCallScore->wCurrentUser==GetMeChairID())&&pCallScore->cbUserCallScore!=0)
//	{
//		m_GameClientView.m_btCallScoreNone.ShowWindow(false);
//		m_GameClientView.m_btCallBanker.ShowWindow(false);
//		m_GameClientView.m_btRodBanker.ShowWindow(TRUE);
//		m_GameClientView.m_btNoRodBanker.ShowWindow(TRUE);
//
//	}
//
//	//获取用户
//	WORD wCallScoreUser=pCallScore->wCallScoreUser;
//	IClientUserItem * pIClientUserItem=GetTableUserItem(wCallScoreUser);
//
//	////播放声音
//	//switch (pCallScore->cbUserCallScore)
//	//{
//	//case 1:
//	//	{
//	//		BYTE cbGender=pIClientUserItem->GetGender();
//	//		PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("SCORE_1_GIRL"):TEXT("SCORE_1_BOY"));
//	//		break;
//	//	}
//	//case 2:
//	//	{
//	//		BYTE cbGender=pIClientUserItem->GetGender();
//	//		PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("SCORE_2_GIRL"):TEXT("SCORE_2_BOY"));
//	//		break;
//	//	}
//	//case 3:
//	//	{
//	//		BYTE cbGender=pIClientUserItem->GetGender();
//	//		PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("SCORE_3_GIRL"):TEXT("SCORE_3_BOY"));
//	//		break;
//	//	}
//	//case 0xFF:
//	//	{
//	//		BYTE cbGender=pIClientUserItem->GetGender();
//	//		PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("SCORE_NONE_GIRL"):TEXT("SCORE_NONE_BOY"));
//	//		break;
//	//	}
//	//}
//
//	//设置时间
//	if (pCallScore->wCurrentUser!=INVALID_CHAIR)
//	{
//		SetGameClock(pCallScore->wCurrentUser,IDI_CALL_SCORE,m_cbTimeCallScore);
//
//		//清楚当前操作
//		m_GameClientView.m_UserCardControl[SwitchViewChairID(pCallScore->wCurrentUser)].SetCardData(0);
//		m_GameClientView.SetUserPassState(SwitchViewChairID(pCallScore->wCurrentUser), false);
//		m_GameClientView.SetUserCallScore(SwitchViewChairID(pCallScore->wCurrentUser), 0);
//
//		m_GameClientView.InvalidGameView(0,0,0,0);
//	}
//
//	return true;
//}

//庄家信息
bool CGameClientEngine::OnSubBankerInfo(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_BankerInfo));
	if (wDataSize!=sizeof(CMD_S_BankerInfo)) return false;

	//变量定义
	CMD_S_BankerInfo * pBankerInfo=(CMD_S_BankerInfo *)pData;

	//设置状态
	SetGameStatus(GAME_SCENE_PLAY);

	//设置变量
	m_wBankerUser = pBankerInfo->wBankerUser;
	m_wCurrentUser = pBankerInfo->wCurrentUser;
	//m_cbBankerScore = pBankerInfo->cbBankerScore;
//
//	//设置界面
//	m_GameClientView.SetWaitCallScore(false);
//	m_GameClientView.ShowBankerCard(pBankerInfo->cbBankerCard);
//	m_GameClientView.SetBankerScore(m_cbBankerScore);
//
//	//设置叫地主为false
//	m_GameClientView.m_btCallBanker.ShowWindow(false);
//	//m_GameClientView.m_btCallScoreNone.ShowWindow(false);

	//设置界面
	//m_GameClientView.ShowBankerCard(pBankerInfo->cbBankerCard);
	//m_GameClientView.m_btScore.ShowWindow(true);
	//m_GameClientView.SetSpecialBackCard(true, pBankerInfo->cbBackCardType, m_wBackCardTime);

	//设置界面
	m_GameClientView.SetWaitCallScore(false);
	m_GameClientView.ShowBankerCard(pBankerInfo->cbBankerCard);





	//计算倍数
	SetCurrentTime();

	//庄家设置
	//m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//m_cbHandCardCount[m_wBankerUser]+=3;
	//if (/*m_bValidCardInfo[m_wBankerUser] ||*/ (GetMeChairID() == m_wBankerUser && !IsLookonMode()))
	//{		
	//	CopyMemory(&m_cbHandCardData[m_wBankerUser][NORMAL_COUNT],pBankerInfo->cbBankerCard,sizeof(BYTE) * 3);

	//	//排列扑克
	//	m_GameLogic.SortCardList(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser],ST_ORDER);
	//	m_GameClientView.m_HandCardControl[SwitchViewChairID(m_wBankerUser)].SetCardData(m_cbHandCardData[m_wBankerUser], 20);
	//	if (GetMeChairID() == m_wBankerUser)
	//	{
	//		//弹起扑克
	//		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(pBankerInfo->cbBankerCard,CountArray(pBankerInfo->cbBankerCard));
	//	}

	//}
	//else
	//{
	//	m_GameClientView.m_HandCardControl[SwitchViewChairID(m_wBankerUser)].SetCardData(20);
	//}

	////ZeroMemory(m_cbAddDoubleInfo, sizeof(m_cbAddDoubleInfo));
	///*for (int i = 0; i < GAME_PLAYER; i++)
	//{
	//	if (!pBankerInfo->bDoubleInfo[i])
	//	{
	//		m_cbAddDoubleInfo[i] = CB_NO_ADD_DOUBLE;
	//	}
	//}*/

	return true;
}

//用户出牌
bool CGameClientEngine::OnSubOutCard(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pData;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->cbCardData);

	//效验数据
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE)))) return false;

	//变量定义
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	BYTE cbTurnCardType=m_GameLogic.GetCardType(pOutCard->cbCardData,pOutCard->cbCardCount);

	//删除时间
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);

	//历史清理
	if (m_cbTurnCardCount==0)
	{
		//用户扑克
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//保存数据
			m_HistoryCard.OnEventSaveData(i);

			//桌面清理
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);
			}
		}

		//放弃标志
		m_GameClientView.SetUserPassState(INVALID_CHAIR,false);
	}

	//出牌动作
	if ((IsLookonMode()==true)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//当前扑克
		SwitchToCurrentCard();

		//播放声音
		BYTE cbCardType=m_GameLogic.GetCardType(pOutCard->cbCardData,pOutCard->cbCardCount);
		if ( m_cbTurnCardCount != 0 
			&& !((cbCardType == CT_THREE_TAKE_ONE && pOutCard->cbCardCount > 4) || (cbCardType == CT_THREE_TAKE_TWO && pOutCard->cbCardCount > 5))
			&& !( cbCardType == CT_MISSILE_CARD )
			&& !( cbCardType == CT_BOMB_CARD ))
		{
			//变量定义
			LPCTSTR pszSoundBoy=TEXT("OUT_CARD_BOY");
			LPCTSTR pszSoundGirl=TEXT("OUT_CARD_GIRL");
			IClientUserItem * pIClientUserItem=GetTableUserItem(pOutCard->wOutCardUser);

			//播放声音
			BYTE cbGender=pIClientUserItem->GetGender();
			PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?pszSoundGirl:pszSoundBoy);
		}
		else
		{
			//播放声音
			PlayOutCardSound(pOutCard->wOutCardUser,pOutCard->cbCardData,pOutCard->cbCardCount);
		}

		//启动动画
		if ( (cbCardType == CT_THREE_TAKE_ONE && pOutCard->cbCardCount > 4)
			|| (cbCardType == CT_THREE_TAKE_TWO && pOutCard->cbCardCount > 5) )
		{
			m_GameClientView.ShowCartoonPlan(true);
			PlayGameSound(AfxGetInstanceHandle(),TEXT("PLANE"));	
		}
		else if ( cbCardType == CT_MISSILE_CARD )
		{
			m_GameClientView.ShowCartoonRocket(true);
			PlayGameSound(AfxGetInstanceHandle(),TEXT("PLANE"));
			m_cbBombCount++;
			m_wBombTime *= 2;
			//计算倍数
			SetCurrentTime();
		}
		else if ( cbCardType == CT_BOMB_CARD )
		{
			m_GameClientView.ShowCartoonBomb(true);
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB"));	
			m_cbBombCount++;
			m_wBombTime *= 2;

			//计算倍数
			SetCurrentTime();
		}

		//出牌历史
		m_HistoryCard.OnEventUserOutCard(pOutCard->wOutCardUser,pOutCard->cbCardData,pOutCard->cbCardCount);

		//显示出牌
		WORD wViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
		m_GameClientView.m_UserCardControl[wViewChairID].ShowOutCard(pOutCard->cbCardData,pOutCard->cbCardCount);
		
		//删除扑克
		if (pOutCard->wOutCardUser==wMeChairID || m_bUserCheat[pOutCard->wOutCardUser])
		{
			//删除扑克
			BYTE cbSourceCount=m_cbHandCardCount[pOutCard->wOutCardUser];
			m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
			m_GameLogic.RemoveCardList(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData[pOutCard->wOutCardUser],cbSourceCount);

			//排列扑克
			m_GameLogic.SortCardList(m_cbHandCardData[pOutCard->wOutCardUser],m_cbHandCardCount[pOutCard->wOutCardUser],m_cbSortType);

			//设置扑克
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[pOutCard->wOutCardUser],m_cbHandCardCount[pOutCard->wOutCardUser]);
		}
		else
		{
			//设置扑克
			m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[pOutCard->wOutCardUser]);
		}
	}

	//出牌变量
	m_wCurrentUser=pOutCard->wCurrentUser;
	m_cbTurnCardCount=pOutCard->cbCardCount;
	CopyMemory(m_cbTurnCardData,pOutCard->cbCardData,pOutCard->cbCardCount*sizeof(BYTE));

	//报牌警告
	if ( m_cbHandCardCount[pOutCard->wOutCardUser] <= 2 && m_cbHandCardCount[pOutCard->wOutCardUser] > 0 )
	{
		//设置报警
		m_GameClientView.SetUserCountWarn(wViewChairID,true);

		//播放报警
		PlayGameSound(AfxGetInstanceHandle(),TEXT("COUNT_WARN"));
	}

	//最大判断
	if (m_wCurrentUser==pOutCard->wOutCardUser)
	{
		//设置用户
		m_wCurrentUser=INVALID_CHAIR;
		m_wMostCardUser=pOutCard->wCurrentUser;

		//出牌变量
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

		//放弃动作
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//用户过虑
			if (pOutCard->wOutCardUser==i) continue;

			//放弃记录
			m_HistoryCard.OnEventSaveData(i);
			m_HistoryCard.OnEventUserPass(i);

			//放弃界面
			WORD wViewChairID=SwitchViewChairID(i);
			m_GameClientView.SetUserPassState(wViewChairID,true);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);
		}

		//设置时间
		SetTimer(IDI_MOST_CARD,3000,NULL);
	}

	//更新界面
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return true;
}

//用户放弃
bool CGameClientEngine::OnSubPassCard(VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PassCard));
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;

	//变量定义
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pData;

	//设置变量
	m_wCurrentUser=pPassCard->wCurrentUser;

	//放弃设置
	if ((IsLookonMode()==true)||(pPassCard->wPassCardUser!=GetMeChairID()))
	{
		//当前扑克
		SwitchToCurrentCard();

		//删除时间
		KillGameClock(IDI_OUT_CARD);

		//历史记录
		m_HistoryCard.OnEventUserPass(pPassCard->wPassCardUser);

		//放弃设置
		m_GameClientView.SetUserPassState(SwitchViewChairID(pPassCard->wPassCardUser),true);
		m_GameClientView.m_UserCardControl[SwitchViewChairID(pPassCard->wPassCardUser)].SetCardData(0);

		//播放声音
		IClientUserItem * pClientUserItem=GetTableUserItem(pPassCard->wPassCardUser);
		PlayGameSound(AfxGetInstanceHandle(),(pClientUserItem->GetGender()==GENDER_FEMALE)?TEXT("PASS_CARD_GIRL"):TEXT("PASS_CARD_BOY"));
	}

	//玩家设置
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//保存数据
		m_HistoryCard.OnEventSaveData(m_wCurrentUser);

		//变量定义
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);

		//门前清理
		m_GameClientView.SetUserPassState(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);
	}

	//一轮判断
	if (pPassCard->cbTurnOver==TRUE)
	{
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	}

	//玩家设置
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//激活框架
		if (m_bTrustee==false) ActiveGameFrame();

		//显示按钮
		m_GameClientView.m_btOutCard.ShowWindow(true);
		m_GameClientView.m_btPassCard.ShowWindow(true);
		m_GameClientView.m_btOutPrompt.ShowWindow(true);

		//m_GameClientView.m_btCallBanker.ShowWindow(false);
		//m_GameClientView.m_btCallScoreNone.ShowWindow(false);

		m_GameClientView.m_btPassCard.EnableWindow((m_cbTurnCardCount>0)?true:false);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);

		//搜索提示
		try
		{
			m_GameLogic.SearchOutCard(m_cbHandCardData[GetMeChairID()],m_cbHandCardCount[m_wCurrentUser],m_cbTurnCardData,m_cbTurnCardCount,
				&m_SearchCardResult);
		}catch(...)
		{
			ASSERT(FALSE);
			m_SearchCardResult.cbSearchCount = 0;
		}
	}
	//else if ( m_wCurrentUser != INVALID_CHAIR )
	//{
	//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
	//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
	//}


	//设置时间
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeOutCard);

		//清楚当前操作
		m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
		m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
//		m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return true;
}

//出牌开始
bool CGameClientEngine::OnSubOutCardStart(VOID * pData, WORD wDataSize)
{

	m_GameClientView.SetUserCallBanker(INVALID_CHAIR,CB_NOT_CALL);
	m_GameClientView.SetUserRodBanker(INVALID_CHAIR,CB_NOT_ROD);
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_Game_Start));
	if (wDataSize!=sizeof(CMD_S_Game_Start)) return false;

	//变量定义
	CMD_S_Game_Start * pGameStart=(CMD_S_Game_Start *)pData;

	//设置状态
	SetGameStatus(GAME_SCENE_PLAY);

	//删除时间
	KillGameClock(IDI_ROD_BANKER);

	//设置变量
	m_wBankerUser = pGameStart->wBankerUser;
	m_wCurrentUser = pGameStart->wCurrentUser;

	//m_GameClientView.m_btValidCard.ShowWindow(false);

	//庄家设置
	m_GameClientView.SetBankerUser(SwitchViewChairID(pGameStart->wBankerUser));

	//状态效验
	ASSERT(GetGameStatus()==GAME_SCENE_PLAY);
	if (GetGameStatus()!=GAME_SCENE_PLAY) return 0L;

	//设置界面
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//设置背面
	WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
	m_GameClientView.m_HandCardControl[wViewChairID].SetBackGround(CARD_LAND);

	////设置底牌
	//if (m_wBankerUser==GetMeChairID())
	//{
	//	//拷贝扑克
	//	m_cbHandCardCount[m_wBankerUser]=MAX_COUNT;
	//	CopyMemory(&m_cbHandCardData[m_wBankerUser],pGameStart->cbCardData,sizeof(BYTE) * MAX_COUNT);

	//	//排列扑克
	//	m_GameLogic.SortCardList(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser],m_cbSortType);

	//	//设置扑克
	//	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser]);

	//	//弹起扑克
	//	BYTE cbBankerCard[3];
	//	m_GameClientView.m_BackCardControl.GetCardData(cbBankerCard,CountArray(cbBankerCard));
	//	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(cbBankerCard,CountArray(cbBankerCard));
	//}
	//else
	//{
	//	//设置扑克
	//	m_cbHandCardCount[m_wBankerUser]=MAX_COUNT;

	//	////其他玩家
	//	//if (!m_bValidCardInfo[m_wBankerUser])
	//	//{
	//	//	WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
	//	//	m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(MAX_COUNT);
	//	//}		
	//}

	//控制设置
	if (IsLookonMode()==false)
	{
		//激活框架
		if (m_bTrustee==false) ActiveGameFrame();

		//出牌按钮
		if (m_wCurrentUser==GetMeChairID())
		{
			//启用按钮
			m_GameClientView.m_btOutCard.EnableWindow(false);
			m_GameClientView.m_btPassCard.EnableWindow(false);

			//显示按钮
			m_GameClientView.m_btOutCard.ShowWindow(true);
			m_GameClientView.m_btPassCard.ShowWindow(true);
			m_GameClientView.m_btOutPrompt.ShowWindow(true);

			//搜索提示
			try
			{
				m_GameLogic.SearchOutCard(m_cbHandCardData[m_wCurrentUser],m_cbHandCardCount[m_wCurrentUser],m_cbTurnCardData,m_cbTurnCardCount,
					&m_SearchCardResult);
			}catch(...)
			{
				ASSERT(FALSE);
				m_SearchCardResult.cbSearchCount = 0;
			}
		}
		//else if ( m_wCurrentUser != INVALID_CHAIR )
		//{
		//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
		//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
		//}


		//控制设置
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(true);
	}

	//更新按钮
	UpdateButtonControl();

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("BANKER_INFO"));

	//清楚当前操作
	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
	m_GameClientView.SetUserCallBanker(SwitchViewChairID(m_wCurrentUser), CB_NOT_CALL);
	m_GameClientView.SetUserRodBanker(SwitchViewChairID(m_wCurrentUser), CB_NOT_ROD);
	/*for (int i = 0; i < GAME_PLAYER; i++)
	{
		m_GameClientView.SetUserAddDouble(i, CB_NOT_ADD_DOUBLE);
	}	*/

	//设置时间
	SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeHeadOutCard);

	//玩家控制
	if (IsLookonMode()==false)	
	{
		m_GameClientView.m_btSortCard.EnableWindow(true);
	}

	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//游戏结束
bool CGameClientEngine::OnSubGameConclude(VOID * pData, WORD wDataSize)
{

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameConclude));
	if (wDataSize!=sizeof(CMD_S_GameConclude)) return false;

	//变量定义
	CMD_S_GameConclude * pGameConclude=(CMD_S_GameConclude *)pData;

	//当前状态
	SwitchToCurrentCard();

	//设置状态
	SetGameStatus(GAME_SCENE_FREE);

	//停止发牌
	m_GameClientView.StopDispatchCard();

	//删除时间
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);
//	KillGameClock(IDI_CALL_SCORE);
	KillGameClock(IDI_CALL_BANKER);


	//控制按钮
	m_GameClientView.m_btOutCard.ShowWindow(false);
	m_GameClientView.m_btPassCard.ShowWindow(false);
	m_GameClientView.m_btOutPrompt.ShowWindow(false);

	//叫分按钮
	/*m_GameClientView.m_btCallScore1.ShowWindow(false);
	m_GameClientView.m_btCallScore2.ShowWindow(false);
	m_GameClientView.m_btCallScore3.ShowWindow(false);*/
	m_GameClientView.m_btNoCallBanker.ShowWindow(false);
	m_GameClientView.m_btRodBanker.ShowWindow(false);
	m_GameClientView.m_btNoRodBanker.ShowWindow(false);

	//叫地主按钮
	m_GameClientView.m_btCallBanker.ShowWindow(false);

	//禁用控件
	m_GameClientView.m_btOutCard.EnableWindow(false);
	m_GameClientView.m_btPassCard.EnableWindow(false);
	m_GameClientView.m_btSortCard.EnableWindow(false);
	m_GameClientView.m_btLastTurn.EnableWindow(false);

	//变量定义
	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

	//成绩变量
	
	ScoreInfo.wMeChairID=GetMeChairID();
	ScoreInfo.wBankerUser=m_wBankerUser;
	ScoreInfo.lCellScore=pGameConclude->lCellScore;
	ScoreInfo.cbBombCount=pGameConclude->cbBombCount;
	//ScoreInfo.cbBankerScore=pGameConclude->cbBankerScore;
	ScoreInfo.bChunTian=pGameConclude->bChunTian;
	ScoreInfo.bFanChunTian=pGameConclude->bFanChunTian;



	CString strOverInfo = TEXT("结束信息 \n");
	//用户成绩
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		IClientUserItem * pIClientUserItem=GetTableUserItem(i);
		tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

		//炸弹信息
		ScoreInfo.cbEachBombCount[i]=pGameConclude->cbEachBombCount[i];

		SetCurrentTime();

		//设置成绩
		ScoreInfo.lGameScore[i]=pGameConclude->lGameScore[i];
		ScoreInfo.lCollectScore[i]=pHistoryScore->lCollectScore+pGameConclude->lGameScore[i];

		//用户信息
		ScoreInfo.wFaceID[i]=pIClientUserItem->GetFaceID();
		ScoreInfo.dwUserID[i]=pIClientUserItem->GetUserID();
		lstrcpyn(ScoreInfo.szNickName[i],pIClientUserItem->GetNickName(),CountArray(ScoreInfo.szNickName[i]));

		//历史成绩
		m_HistoryScore.OnEventUserScore(i,pGameConclude->lGameScore[i]);

		CString str;
		str.Format(TEXT("%s : %0.2f \n"), ScoreInfo.szNickName[i], ScoreInfo.lGameScore[i]);
		strOverInfo += str;
	}

	//if ( m_pIStringMessage )
	//	m_pIStringMessage->InsertSystemString(strOverInfo);

	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//设置信息
		if (pGameConclude->lGameScore[i]!=0L)
		{
			if (ScoreInfo.szNickName[i]==0) m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("已离开"),pGameConclude->lGameScore[i]);
			else m_GameClientView.m_ScoreView.SetGameScore(i,ScoreInfo.szNickName[i],pGameConclude->lGameScore[i]);
			
		}
	}

	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);


	ScoreInfo.wStartTime = m_wStartTime;
//	ScoreInfo.wValidCardTime = m_wValidCardTime;
	ScoreInfo.wRodBankerTime = m_wRodBankerTime;
//	ScoreInfo.wBackCardTime = m_wBackCardTime;
	ScoreInfo.wChunTianbTime = m_wChunTianbTime;
	ScoreInfo.wCardTypeTime = m_wCardTypeTime;
	ScoreInfo.wBombTime = m_wBombTime;
	ScoreInfo.wTotalTime = m_wCurrentTime;

	ScoreInfo.wStartTime = m_wStartTime;

	
	//显示成绩
	//m_GameClientView.ShowGameScoreInfo(ScoreInfo);

	//设置扑克
	BYTE cbCardIndex=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//设置扑克
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(true);
		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(&pGameConclude->cbHandCardData[cbCardIndex],pGameConclude->cbCardCount[i]);

		//设置索引
		cbCardIndex+=pGameConclude->cbCardCount[i];
	}

	//设置界面
	m_GameClientView.SetWaitCallScore(false);
	m_GameClientView.SetUserPassState(INVALID_CHAIR,false);
//	m_GameClientView.SetUserCallScore(INVALID_CHAIR,false);
	m_GameClientView.SetUserCountWarn(INVALID_CHAIR,false);
	m_GameClientView.SetUserCallBanker(INVALID_CHAIR, CB_NOT_CALL);
	m_GameClientView.SetUserRodBanker(INVALID_CHAIR, CB_NOT_ROD);

	//播放声音
	SCORE lMeScore=pGameConclude->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOSE"));
	if (lMeScore==0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_CONCLUDE"));

	//取消托管
	if(m_bTrustee)
		OnMessageTrusteeControl(0,0);

	//排序
	if ( m_cbSortType == ST_CUSTOM )
		OnMessageSortHandCard(ST_ORDER,0);

	//开始设置
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(true);
		PerformAutoStart();
	}

	//更新界面
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return true;
}

//作弊数据
bool CGameClientEngine::OnSubCheatCard(VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CheatCard));
	if (wDataSize!=sizeof(CMD_S_CheatCard)) return false;

	//变量定义
	CMD_S_CheatCard * pCheatCard=(CMD_S_CheatCard *)pData;

	ASSERT(pCheatCard->cbUserCount <= GAME_PLAYER);
	if(pCheatCard->cbUserCount > GAME_PLAYER) return false;
	
	m_bCheatRight = true;
	for(WORD i = 0; i < pCheatCard->cbUserCount ; i++)
	{
		//显示扑克
		m_GameClientView.UserValidCard(SwitchViewChairID(pCheatCard->wCardUser[i]), pCheatCard->cbCardData[i], pCheatCard->cbCardCount[i]);
		m_bUserCheat[pCheatCard->wCardUser[i]] = true;
		CopyMemory(m_cbHandCardData[pCheatCard->wCardUser[i]], pCheatCard->cbCardData[i], sizeof(BYTE) * pCheatCard->cbCardCount[i]);

	}
	//更新界面
	CRect rcClient;
	GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	return true;
}
//计算倍数
bool CGameClientEngine::SetCurrentTime()
{
	m_wCurrentTime = 1;

	tagDoubleInfo DoubleInfo;
	ZeroMemory(&DoubleInfo, sizeof(DoubleInfo));

	//计算倍数
	m_wCurrentTime *= (m_wStartTime != 0 ? m_wStartTime : 1);
	//m_wCurrentTime *= (m_wValidCardTime != 0 ? m_wValidCardTime : 1);
	m_wCurrentTime *= (m_wRodBankerTime != 0 ? m_wRodBankerTime : 1);
	m_wCurrentTime *= (m_wBackCardTime != 0 ? m_wBackCardTime : 1);
	m_wCurrentTime *= (m_wBombTime != 0 ? m_wBombTime : 1);

	////用户加倍
	//for(int i = 0; i < GAME_PLAYER; i++)
	//{
	//	DoubleInfo.wUserDouble[i] = 1;
	//	if (CB_ADD_DOUBLE == m_cbAddDoubleInfo[i])
	//	{
	//		DoubleInfo.wUserDouble[i] = 2;
	//		m_wCurrentTime *= 2;
	//	}		
	//}

	//m_wCurrentTime *= (m_wChunTianbTime != 0 ? m_wChunTianbTime : 1);
	//m_wCurrentTime *= (m_wCardTypeTime != 0 ? m_wCardTypeTime : 1);

	CString str;
	str.Format(L"zzz 当前倍数是m_wCurrentTime==%d ",m_wCurrentTime);
	OutputDebugString(str);

	m_GameClientView.SetCurrentTime(m_wCurrentTime);


	DoubleInfo.wStartTime =  m_wStartTime;
	//DoubleInfo.wValidCardTime = m_wValidCardTime == 1 ? 0 : m_wValidCardTime;
	DoubleInfo.wRodBankerTime = m_wRodBankerTime == 1 ? 0 : m_wRodBankerTime;
	DoubleInfo.wBackCardTime = m_wBackCardTime == 1 ? 0 : m_wBackCardTime;
	DoubleInfo.wCardTypeTime =m_wCardTypeTime == 1 ? 0 : m_wCardTypeTime;
	DoubleInfo.wBombTime = (m_wBombTime == 1 ? 0 : m_wBombTime);
	DoubleInfo.wChunTianbTime = m_wChunTianbTime == 1 ? 0 : m_wChunTianbTime;

	DoubleInfo.wMeChairID = GetMeChairID();
	DoubleInfo.wBankerUser = m_wBankerUser;

	m_GameClientView.m_DoubleControl.SetDoubleInfo(DoubleInfo);

	return true;
}

//出牌判断
bool CGameClientEngine::VerdictOutCard()
{
	//状态判断
	if (m_wCurrentUser!=GetMeChairID()) return false;
	if (GetGameStatus()!=GAME_SCENE_PLAY) return false;

	//获取扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbCardData,CountArray(cbCardData));

	//出牌判断
	if (cbShootCount>0L)
	{
		//类型判断
		m_GameLogic.SortCardList(cbCardData,cbShootCount,ST_ORDER);
		if (m_GameLogic.GetCardType(cbCardData,cbShootCount)==CT_ERROR) return false;

		//跟牌判断
		if (m_cbTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(m_cbTurnCardData,cbCardData,m_cbTurnCardCount,cbShootCount);
	}

	return false;
}

//自动开始
bool CGameClientEngine::PerformAutoStart()
{
	//变量定义
	WORD wTableUserCount=0,wReadyUserCount=0;
	IClientUserItem * pIMySelfUserItem=GetMeUserItem();

	//状态判断
	if ((pIMySelfUserItem==NULL)||(pIMySelfUserItem->GetUserStatus()==US_READY)) return false;
	if ((IsLookonMode()==true)||(m_pIClientKernel->GetClockChairID()==IDI_START_GAME)) return false;

	//统计用户
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取用户
		IClientUserItem * pIClientUserItem=GetTableUserItem(i);

		//数目统计
		if (pIClientUserItem!=NULL) 
		{
			wTableUserCount++;
			if (pIClientUserItem->GetUserStatus()==US_READY) wReadyUserCount++;
		}
	}

	//开始判断
	if ((wTableUserCount==GAME_PLAYER)&&((wReadyUserCount+1)==wTableUserCount))
	{
		//开始设置
		WORD wMeChairID=GetMeChairID();
		SetGameClock(wMeChairID,IDI_START_GAME,m_cbTimeStartGame);
		return true;
	}

	return true;
}

//当前扑克
bool CGameClientEngine::SwitchToCurrentCard()
{
	//处理判断
	if (m_bLastTurn==false) return false;

	//设置变量
	m_bLastTurn=false;

	//删除时间
	KillTimer(IDI_LAST_TURN);

	//设置界面
	m_GameClientView.SetLastTurnState(false);

	//设置扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wViewChairID=SwitchViewChairID(i);
		BYTE cbEventFlag=m_HistoryCard.GetCurrentEvent(i);

		//获取扑克
		BYTE cbCardData[MAX_COUNT];
		BYTE cbCardCount=m_HistoryCard.GetCurrentCard(i,cbCardData,CountArray(cbCardData));

		//设置界面
		m_GameClientView.SetUserPassState(wViewChairID,cbEventFlag == AF_PASS);
		if( wViewChairID,cbEventFlag == AF_PASS || wViewChairID,cbEventFlag == AF_NULL )
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		}
		else
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//更新界面
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return true;
}

//自动出牌
bool CGameClientEngine::AutomatismOutCard()
{
	//状态判断
	if (GetGameStatus()!=GAME_SCENE_PLAY) return false;
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return false;

	//当前弹起
	CSkinButton & btOutCard=m_GameClientView.m_btOutCard;
	if ((btOutCard.IsWindowEnabled())&&(m_wCurrentUser==GetMeChairID()))
	{
		OnMessageOutCard(0,0);
		return true;
	}

	//出牌处理
	if ((m_cbTurnCardCount==0)||(m_bTrustee==true))
	{
		//设置界面
		if (m_SearchCardResult.cbSearchCount>0)
		{
			//设置界面
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard( m_SearchCardResult.cbResultCard[0],
				m_SearchCardResult.cbCardCount[0] );

			//设置控件
			bool bOutCard=VerdictOutCard();
			ASSERT(bOutCard);
			m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

			//出牌动作
			OnMessageOutCard(0,0);

			return true;
		}
	}

	//放弃出牌
	if (m_cbTurnCardCount>0) OnMessagePassCard(0,0);

	return true;
}

//更新按钮
VOID CGameClientEngine::UpdateButtonControl()
{
	//状态判断
	if (GetMeUserItem()==NULL) return;
	if ((IsLookonMode()==true)&&(IsAllowLookon()==false)) return;

	//变量定义
	WORD wMeChairId=GetMeChairID();
	if (m_cbHandCardCount[wMeChairId]==0) return;

	//变量定义
	BYTE cbHandCardCount=m_cbHandCardCount[wMeChairId];
	BYTE cbHandCardData[MAX_COUNT] = {0};
	CopyMemory(cbHandCardData,m_cbHandCardData[wMeChairId],sizeof(BYTE)*cbHandCardCount);

	//排序
	m_GameLogic.SortCardList(cbHandCardData,cbHandCardCount,ST_ORDER);


	//显示功能按钮
	m_GameClientView.m_btCardPrompt1.ShowWindow(true);
	m_GameClientView.m_btCardPrompt2.ShowWindow(true);
	m_GameClientView.m_btCardPrompt3.ShowWindow(true);
	m_GameClientView.m_btCardPrompt4.ShowWindow(true);
	m_GameClientView.m_btCardPrompt5.ShowWindow(true);
	m_GameClientView.m_btCardPrompt6.ShowWindow(true);

	bool bEnableButton = false;
	//搜索火箭
	if( cbHandCardCount > 1 && cbHandCardData[0] == 0x4f && cbHandCardData[1] == 0x4e )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt1.EnableWindow(bEnableButton);

	//搜索炸弹
	if( m_GameLogic.SearchSameCard( cbHandCardData,cbHandCardCount,0,4,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt2.EnableWindow(bEnableButton);

	//搜索飞机
	if( m_GameLogic.SearchThreeTwoLine( cbHandCardData,cbHandCardCount,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt3.EnableWindow(bEnableButton);

	//搜索双顺
	if( m_GameLogic.SearchLineCardType( cbHandCardData,cbHandCardCount,0,2,0,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt4.EnableWindow(bEnableButton);

	//搜索单顺
	if( m_GameLogic.SearchLineCardType( cbHandCardData,cbHandCardCount,0,1,0,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt5.EnableWindow(bEnableButton);

	//搜索三带N
	if( m_GameLogic.SearchTakeCardType( cbHandCardData,cbHandCardCount,0,3,1,NULL ) ||
		m_GameLogic.SearchTakeCardType( cbHandCardData,cbHandCardCount,0,3,2,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt6.EnableWindow(bEnableButton);

	//清空搜索
	m_nCurSearchType = -1;
	m_cbEachSearchIndex = 0;
	ZeroMemory( &m_EachSearchResult,sizeof(m_EachSearchResult) );

	return;
}

//播放声音
VOID CGameClientEngine::PlayOutCardSound(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//效验参数
	ASSERT((wChairID<GAME_PLAYER)&&(cbCardCount>0));
	if ((wChairID>=GAME_PLAYER)||(cbCardCount==0)) return;
	
	//获取类型
	BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);

	//获取用户
	IClientUserItem * pIClientUserItem=GetTableUserItem(wChairID);

	//播放声音
	switch (cbCardType)
	{
	case CT_SINGLE:					//单牌类型
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();
			BYTE cbCardValue=m_GameLogic.GetCardValue(cbCardData[0]);

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("SINGLE_%d_%s"),cbCardValue,(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_DOUBLE:					//对牌类型
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("DOUBLE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_THREE:					//三条类型
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_SINGLE_LINE:			//单连类型
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("SINGLE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_DOUBLE_LINE:			//对连类型
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("DOUBLE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_THREE_LINE:				//三连类型
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_THREE_TAKE_ONE:			//三带一单
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");

			if ( cbCardCount > 4 )
				_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_ONE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));
			else
				_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_TAKE_ONE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_THREE_TAKE_TWO:			//三带一对
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			if ( cbCardCount > 5 )
				_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_ONE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));
			else
				_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_TAKE_TWO_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_FOUR_TAKE_ONE:			//四带两单
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("FOUR_TAKE_ONE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_FOUR_TAKE_TWO:			//四带两对
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("FOUR_TAKE_TWO_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_BOMB_CARD:				//炸弹类型
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("BOMB_CARD_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_MISSILE_CARD:			//火箭类型
		{
			//变量定义
			BYTE cbGender=pIClientUserItem->GetGender();

			//构造声音
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("MISSILE_CARD_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	}

	return;
}


//用户状态
VOID CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//变量定义
	IClientUserItem * pIMySelfUserItem=GetMeUserItem();
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

	//自动开始
	if ((pIClientUserItem!=pIMySelfUserItem)&&(cbUserStatus==US_READY)) PerformAutoStart();

	//停止开始
	if ((pIClientUserItem==pIMySelfUserItem)&&(cbUserStatus>=US_READY)) 
	{
		KillGameClock(IDI_START_GAME);
		m_GameClientView.m_btStart.ShowWindow(false);//zrh
	}
}

//时间消息
VOID CGameClientEngine::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_MOST_CARD:			//最大扑克
		{
			//用户效验
			ASSERT(m_wMostCardUser!=INVALID_CHAIR);
			if (m_wMostCardUser==INVALID_CHAIR) return;

			//动画判断
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_GameClientView.m_UserCardControl[i].IsOutCard()==true)
				{
					return;
				}
			}

			//删除时间
			KillTimer(IDI_MOST_CARD);

			//设置变量
			m_wCurrentUser=m_wMostCardUser;
			m_wMostCardUser=INVALID_CHAIR;

			//上轮设置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_HistoryCard.OnEventSaveData(i);
			}

			//设置界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetUserPassState(i,false);
				m_GameClientView.m_UserCardControl[i].SetCardData(0);
			}

			//玩家设置
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				//激活框架
				if (m_bTrustee==false) ActiveGameFrame();

				//设置按钮
				m_GameClientView.m_btOutCard.ShowWindow(true);
				m_GameClientView.m_btPassCard.ShowWindow(true);
				m_GameClientView.m_btPassCard.EnableWindow(false);
				m_GameClientView.m_btOutPrompt.ShowWindow(true);
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);

				//搜索提示
				try
				{
					m_GameLogic.SearchOutCard(m_cbHandCardData[GetMeChairID()],m_cbHandCardCount[m_wCurrentUser],m_cbTurnCardData,m_cbTurnCardCount,
						&m_SearchCardResult);
				}catch(...)
				{
					ASSERT(FALSE);
					m_SearchCardResult.cbSearchCount = 0;
				}
			}
			//else if ( m_wCurrentUser != INVALID_CHAIR )
			//{
			//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			//}


			//设置时间
			SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeOutCard);

			//清楚当前操作
			m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
//			m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

			m_GameClientView.InvalidGameView(0,0,0,0);

			return;
		}
	case IDI_LAST_TURN:			//上轮扑克
		{
			//还原界面
			SwitchToCurrentCard();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//开始消息
LRESULT CGameClientEngine::OnMessageStart(WPARAM wParam, LPARAM lParam)
{
	//删除时间
	KillGameClock(IDI_START_GAME);
	KillGameClock(IDI_OUT_CARD);
	KillGameClock(IDI_CALL_BANKER);
	KillGameClock(IDI_ROD_BANKER);

	

	//用户扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetDisplayItem(false);
	}

	//扑克控件
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(false);

	//界面庄家
	//m_cbBankerScore = 0;
//	m_GameClientView.SetBankerScore(0);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);

	//状态设置
	m_GameClientView.SetBombCount(0);
	m_GameClientView.SetWaitCallScore(false);
//	m_GameClientView.SetUserCallScore(INVALID_CHAIR,0);
	m_GameClientView.SetUserCallBanker(INVALID_CHAIR, CB_NOT_CALL);
	m_GameClientView.SetUserRodBanker(INVALID_CHAIR, CB_NOT_ROD);
	m_GameClientView.SetUserPassState(INVALID_CHAIR,false);
	m_GameClientView.SetUserCountWarn(INVALID_CHAIR,false);


	//设置界面
	m_GameClientView.m_btStart.ShowWindow(false);//zrh
	m_GameClientView.m_ScoreControl.CloseControl();

	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//更新界面
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());



	//发送消息
	SendUserReady(NULL,0);

	return 0L;
}

//出牌消息
LRESULT CGameClientEngine::OnMessageOutCard(WPARAM wParam, LPARAM lParam)
{
	//状态判断
	//if (m_GameClientView.m_btOutCard.IsWindowEnable()==false) return 0;
	if (m_GameClientView.m_btOutCard.IsWindowEnabled()==false) return 0;
	
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==false) return 0;

	//当前扑克
	SwitchToCurrentCard();

	//删除时间
	KillGameClock(IDI_OUT_CARD);

	//设置变量
	if(1 == wParam)
	{
		m_bTrusteeCount = 0;
	}
	m_wCurrentUser=INVALID_CHAIR;
	m_cbSearchResultIndex = 0;
	m_SearchCardResult.cbSearchCount = 0;

	//设置界面
	m_GameClientView.m_btOutCard.ShowWindow(false);
	m_GameClientView.m_btPassCard.ShowWindow(false);
	m_GameClientView.m_btOutPrompt.ShowWindow(false);
	m_GameClientView.m_btOutCard.EnableWindow(false);
	m_GameClientView.m_btPassCard.EnableWindow(false);

	//获取扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbCardData,CountArray(cbCardData));

	//排列扑克
	m_GameLogic.SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//出牌设置
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].RemoveShootItem();
	m_GameClientView.m_UserCardControl[MYSELF_VIEW_ID].ShowOutCard(cbCardData,cbCardCount);

	//删除扑克
	WORD wMeChairID=GetMeChairID();
	m_cbHandCardCount[wMeChairID]-=cbCardCount;
	m_GameLogic.RemoveCardList(cbCardData,cbCardCount,m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID]+cbCardCount);

	//按钮设置
	UpdateButtonControl();

	//播放声音
	BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);
	if ( m_cbTurnCardCount != 0 
		&& !((cbCardType == CT_THREE_TAKE_ONE && cbCardCount > 4) || (cbCardType == CT_THREE_TAKE_TWO && cbCardCount > 5))
		&& !( cbCardType == CT_MISSILE_CARD )
		&& !( cbCardType == CT_BOMB_CARD ))
	{
		//变量定义
		LPCTSTR pszSoundBoy=TEXT("OUT_CARD_BOY");
		LPCTSTR pszSoundGirl=TEXT("OUT_CARD_GIRL");
		IClientUserItem * pIClientUserItem=GetTableUserItem(wMeChairID);
		
		//播放声音
		BYTE cbGender=pIClientUserItem->GetGender();
		PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?pszSoundGirl:pszSoundBoy);
	}
	else
	{
		//播放声音
		PlayOutCardSound(wMeChairID,cbCardData,cbCardCount);
	}

	//启动动画
	if ( (cbCardType == CT_THREE_TAKE_ONE && cbCardCount > 4)
		|| (cbCardType == CT_THREE_TAKE_TWO && cbCardCount > 5) )
	{
		m_GameClientView.ShowCartoonPlan(true);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PLANE"));
	}
	else if ( cbCardType == CT_MISSILE_CARD )
	{
		m_GameClientView.ShowCartoonRocket(true);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PLANE"));
		m_cbBombCount++;
		m_wBombTime *= 2;

		//计算倍数
		SetCurrentTime();
	}
	else if ( cbCardType == CT_BOMB_CARD )
	{
		m_GameClientView.ShowCartoonBomb(true);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB"));	
		m_cbBombCount++;
		m_wBombTime *= 2;

		//计算倍数
		SetCurrentTime();
	}

	//出牌历史
	m_HistoryCard.OnEventUserOutCard(wMeChairID,cbCardData,cbCardCount);

	//构造数据
	CMD_C_OutCard OutCard;
	OutCard.cbCardCount=cbCardCount;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount*sizeof(BYTE));

	//发送数据
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE));

	return 0;
}

//PASS消息
LRESULT CGameClientEngine::OnMessagePassCard(WPARAM wParam, LPARAM lParam)
{
	//状态判断
	//if (m_GameClientView.m_btPassCard.IsWindowEnable()==false) return 0;
	//if (m_GameClientView.m_btPassCard.IsWindowVisible()==false) return 0;

	//当前扑克
	SwitchToCurrentCard();

	//删除时间
	KillGameClock(IDI_OUT_CARD);

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	m_cbSearchResultIndex = 0;
	m_SearchCardResult.cbSearchCount = 0;

	//设置界面
	m_GameClientView.m_btOutCard.ShowWindow(false);
	m_GameClientView.m_btPassCard.ShowWindow(false);
	m_GameClientView.m_btOutPrompt.ShowWindow(false);
	m_GameClientView.m_btOutCard.EnableWindow(false);
	m_GameClientView.m_btPassCard.EnableWindow(false);

	//历史记录
	m_HistoryCard.OnEventUserPass(GetMeChairID());

	//设置放弃
	m_GameClientView.SetUserPassState(MYSELF_VIEW_ID,true);

	//设置扑克
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

	//更新界面
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	//播放声音
	IClientUserItem * pClientUserItem=GetMeUserItem();
	PlayGameSound(AfxGetInstanceHandle(),(pClientUserItem->GetGender()==GENDER_FEMALE)?TEXT("PASS_CARD_GIRL"):TEXT("PASS_CARD_BOY"));

	//发送数据
	SendSocketData(SUB_C_PASS_CARD);

	return 0L;
}

//提示消息
LRESULT CGameClientEngine::OnMessageOutPrompt(WPARAM wParam, LPARAM lParam)
{
	WORD wMeChairID = GetMeChairID();

	//有大过牌
	if( m_SearchCardResult.cbSearchCount > 0 )
	{
		//设置界面
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(m_SearchCardResult.cbResultCard[m_cbSearchResultIndex],
			m_SearchCardResult.cbCardCount[m_cbSearchResultIndex]);

		//设置控件
		bool bOutCard=VerdictOutCard();
		ASSERT(bOutCard);
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

		//更新界面
		m_GameClientView.InvalidGameView(0,0,0,0);

		//设置变量
		m_cbSearchResultIndex = (m_cbSearchResultIndex+1)%m_SearchCardResult.cbSearchCount;

		return 0;
	}

	//放弃出牌
	OnMessagePassCard(0,0);
	return 0L;
}
//叫地主消息
LRESULT CGameClientEngine::OnMessageCallBanker(WPARAM wParam, LPARAM lParam)
{
	//删除时间
	KillGameClock(IDI_CALL_BANKER);

	//设置界面
	m_GameClientView.m_btCallBanker.ShowWindow(false);
	m_GameClientView.m_btNoCallBanker.ShowWindow(false);

	//发送数据
	CMD_C_CallBanker CallBanker;
	CallBanker.cbCallInfo=(BYTE)wParam;
	SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));

	return 0L;
}

//抢地主消息
LRESULT CGameClientEngine::OnMessageRodBanker(WPARAM wParam, LPARAM lParam)
{
	//删除时间
	KillGameClock(IDI_ROD_BANKER);

	//设置界面
	m_GameClientView.m_btRodBanker.ShowWindow(false);
	m_GameClientView.m_btNoRodBanker.ShowWindow(false);

	//发送数据
	CMD_C_RodBanker rodBanker;
	rodBanker.cbRodInfo=(BYTE)wParam;
	SendSocketData(SUB_C_ROD_BANKER,&rodBanker,sizeof(rodBanker));

	return 0L;
}
////叫分消息
//LRESULT CGameClientEngine::OnMessageCallScore(WPARAM wParam, LPARAM lParam)
//{
//	//删除时间
//	KillGameClock(IDI_CALL_SCORE);
//
//	//设置界面
//	/*m_GameClientView.m_btCallScore1.ShowWindow(false);
//	m_GameClientView.m_btCallScore2.ShowWindow(false);
//	m_GameClientView.m_btCallScore3.ShowWindow(false);*/
//	//设置按钮隐藏
//	m_GameClientView.m_btCallScoreNone.ShowWindow(false);
//	m_GameClientView.m_btCallBanker.ShowWindow(false);
//	m_GameClientView.m_btRodBanker.ShowWindow(false);
//	m_GameClientView.m_btNoRodBanker.ShowWindow(false);
//
//	//发送数据
//	CMD_C_CallScore CallScore;
//	CallScore.cbCallScore=(BYTE)wParam;
//	SendSocketData(SUB_C_CALL_SCORE,&CallScore,sizeof(CallScore));
//
//	return 0L;
//}

//右键扑克
LRESULT CGameClientEngine::OnMessageLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//设置控件
	bool bOutCard=VerdictOutCard();
	m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

	return 0L;
}

//排列扑克
LRESULT CGameClientEngine::OnMessageSortHandCard(WPARAM wParam, LPARAM lParam)
{
	//自定过滤
	if ((BYTE)wParam==ST_CUSTOM)
	{
		//设置状态
		if (m_cbSortType!=ST_CUSTOM)
		{
			//设置变量
			m_cbSortType=ST_CUSTOM;

			//设置按钮
			HINSTANCE hInstance=AfxGetInstanceHandle();
			//m_GameClientView.m_btSortCard.SetButtonImage(&m_GameClientView.m_D3DDevice,TEXT("BT_SORT_CARD_ORDER"),TEXT("PNG"),hInstance);
			m_GameClientView.m_btSortCard.SetButtonImage(BT_SORT_CARD_ORDER,hInstance,true,true);

		}

		return 0L;
	}

	//设置变量
	m_cbSortType=(m_cbSortType!=ST_ORDER)?ST_ORDER:ST_COUNT;

	//排列扑克
	WORD wMeChairID=GetMeChairID();
	m_GameLogic.SortCardList(m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID],m_cbSortType);

	//获取扑克
	BYTE cbShootCard[MAX_COUNT];
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbShootCard,CountArray(cbShootCard));

	//设置扑克
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID]);

	//提起扑克
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(cbShootCard,cbShootCount);

	//设置按钮
	//LPCTSTR pszButtonImage=(m_cbSortType==ST_ORDER)?TEXT("BT_SORT_CARD_COUNT"):TEXT("BT_SORT_CARD_ORDER");
	//m_GameClientView.m_btSortCard.SetButtonImage(NULL,&m_GameClientView.m_D3DDevice,pszButtonImage,TEXT("PNG"),AfxGetInstanceHandle());
	m_GameClientView.m_btSortCard.SetButtonImage((m_cbSortType==ST_ORDER)?TEXT("BT_SORT_CARD_COUNT"):TEXT("BT_SORT_CARD_ORDER"),AfxGetInstanceHandle(),(m_cbSortType==ST_ORDER),(m_cbSortType==ST_ORDER));

	//更新界面
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return 0;
}

//查看上轮
LRESULT CGameClientEngine::OnMessageLastTurnCard(WPARAM wParam, LPARAM lParam)
{
	if (m_bLastTurn==false)
	{
		//设置扑克
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//变量定义
			WORD wViewChairID=SwitchViewChairID(i);
			BYTE cbEventFlag=m_HistoryCard.GetHistoryEvent(i);

			//获取扑克
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_HistoryCard.GetHistoryCard(i,cbCardData,CountArray(cbCardData));

			//设置界面
			m_GameClientView.SetUserPassState(wViewChairID,cbEventFlag==AF_PASS);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
		}

		//设置时间
		SetTimer(IDI_LAST_TURN,3000,NULL);

		//环境设置
		m_bLastTurn=true;
		m_GameClientView.SetLastTurnState(true);

		//更新界面
		CRect rcClient;
		m_GameClientView.GetClientRect(&rcClient);
		m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}
	else
	{
		//当前扑克
		SwitchToCurrentCard();
	}

	return 0;
}

//拖管控制
LRESULT CGameClientEngine::OnMessageTrusteeControl(WPARAM wParam, LPARAM lParam)
{
	m_bTrusteeCount = 0;
	m_bTrustee = !m_bTrustee;
	if ( !m_bTrustee )
	{
		m_GameClientView.m_btTrustee.ShowWindow(true);
		m_GameClientView.m_btTrustee.EnableWindow(true);
		m_GameClientView.m_btCancelTrustee.ShowWindow(false);
	}
	else
	{
		m_GameClientView.m_btCancelTrustee.EnableWindow(true);
		m_GameClientView.m_btCancelTrustee.ShowWindow(true);
		m_GameClientView.m_btTrustee.ShowWindow(false);
	}

	//更新界面
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return 0L;
}

//搜索牌型
LRESULT CGameClientEngine::OnMessageSearchCard(WPARAM wParam, LPARAM lParam)
{
	WORD wMeChairId = GetMeChairID();	
	if( m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetCardCount() != m_cbHandCardCount[wMeChairId] )
		return 0;

	//变量定义
	BYTE cbHandCardCount = m_cbHandCardCount[wMeChairId];
	BYTE cbHandCardData[MAX_COUNT] = {0};
	CopyMemory( cbHandCardData,m_cbHandCardData[wMeChairId],sizeof(BYTE)*cbHandCardCount );

	//排序
	m_GameLogic.SortCardList( cbHandCardData,cbHandCardCount,ST_ORDER );

	//设置变量
	m_cbSearchResultIndex = 0;

	switch( wParam )
	{
	case SEARCH_MISSILE:			//搜索火箭
		{
			if( m_nCurSearchType == wParam ) break;

			//设置变量
			m_cbEachSearchIndex = 0;

			if( cbHandCardCount > 1 && cbHandCardData[0] == 0x4f && cbHandCardData[1] == 0x4e )
			{
				m_EachSearchResult.cbCardCount[0] = 2;
				m_EachSearchResult.cbResultCard[0][0] = cbHandCardData[0];
				m_EachSearchResult.cbResultCard[0][1] = cbHandCardData[1];

				m_EachSearchResult.cbSearchCount = 1;
			}
			m_nCurSearchType = SEARCH_MISSILE;
			break;
		}
	case SEARCH_BOMB:				//搜索炸弹
		{
			if( m_nCurSearchType == wParam ) break;

			//设置变量
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchSameCard( cbHandCardData,cbHandCardCount,0,4,&m_EachSearchResult );
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_BOMB;
			break;
		}
	case SEARCH_THREE_TOW_LINE:		//搜索飞机
		{
			if( m_nCurSearchType == wParam ) break;

			//设置变量
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchThreeTwoLine( cbHandCardData,cbHandCardCount,&m_EachSearchResult );
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_THREE_TOW_LINE;
			break;
		}
	case SEARCH_DOUBLE_LINE:		//搜索双顺
		{
			if( m_nCurSearchType == wParam ) break;

			//设置变量
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchLineCardType( cbHandCardData,cbHandCardCount,0,2,0,&m_EachSearchResult );
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_DOUBLE_LINE;
			break;
		}
	case SEARCH_SINGLE_LINE:		//搜索单顺
		{
			if( m_nCurSearchType == wParam ) break;

			//设置变量
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchLineCardType( cbHandCardData,cbHandCardCount,0,1,0,&m_EachSearchResult );
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_SINGLE_LINE;
			break;
		}
	case SEARCH_THREE_TWO_ONE:		//搜索三带N
		{
			if( m_nCurSearchType == wParam ) break;

			//设置变量
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchTakeCardType( cbHandCardData,cbHandCardCount,0,3,1,&m_EachSearchResult );
				tagSearchCardResult tmpSearchResult = {0};
				m_GameLogic.SearchTakeCardType( cbHandCardData,cbHandCardCount,0,3,2,&tmpSearchResult );
				if( tmpSearchResult.cbSearchCount > 0 )
				{
					//复制牌型
					for( BYTE i = 0; i < tmpSearchResult.cbSearchCount; i++ )
					{
						BYTE cbResultIndex = m_EachSearchResult.cbSearchCount++;
						m_EachSearchResult.cbCardCount[cbResultIndex] = tmpSearchResult.cbCardCount[i];
						CopyMemory( m_EachSearchResult.cbResultCard[cbResultIndex],tmpSearchResult.cbResultCard[i],
							sizeof(BYTE)*tmpSearchResult.cbCardCount[i] );
					}
				}
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_THREE_TWO_ONE;
			break;
		}
	}

	//弹起扑克
	if( m_EachSearchResult.cbSearchCount > 0 )
	{
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard( m_EachSearchResult.cbResultCard[m_cbEachSearchIndex],
			m_EachSearchResult.cbCardCount[m_cbEachSearchIndex] );

		m_cbEachSearchIndex = (m_cbEachSearchIndex+1)%m_EachSearchResult.cbSearchCount;

		//玩家设置
		if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairId))
		{
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		}
	}
	else 
	{
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

		//玩家设置
		if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairId))
		{
			m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		}
	}

	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0L;
}

//启用上轮
LRESULT CGameClientEngine::OnMessageEnableHistory(WPARAM wParam, LPARAM lParam)
{
	//启用上轮
	m_GameClientView.m_btLastTurn.EnableWindow(true);

	return 0;
}

//出牌完成
LRESULT CGameClientEngine::OnMessageOutCardFinish(WPARAM wParam, LPARAM lParam)
{
	//动画判断
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_GameClientView.m_UserCardControl[i].IsOutCard()==true)
		{
			return 0L;
		}
	}

	//出牌设置
	if ((GetGameStatus()==GAME_SCENE_PLAY)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		//变量定义
		WORD wMeChairID=GetMeChairID();

		//清理扑克
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);

		//出牌按钮
		if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
		{
			//激活框架
			if (m_bTrustee==false) ActiveGameFrame();

			//显示按钮
			m_GameClientView.m_btOutCard.ShowWindow(true);
			m_GameClientView.m_btPassCard.ShowWindow(true);
			m_GameClientView.m_btOutPrompt.ShowWindow(true);

			//启用按钮
			m_GameClientView.m_btPassCard.EnableWindow((m_cbTurnCardCount>0)?true:false);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);

			//搜索提示
			try
			{
				m_GameLogic.SearchOutCard(m_cbHandCardData[wMeChairID],m_cbHandCardCount[m_wCurrentUser],m_cbTurnCardData,m_cbTurnCardCount,
					&m_SearchCardResult);
			}catch(...)
			{
				ASSERT(FALSE);
				m_SearchCardResult.cbSearchCount = 0;
			}
		}
		//else if ( m_wCurrentUser != INVALID_CHAIR )
		//{
		//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
		//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
		//}


		//设置时间
		SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeOutCard);

		//清楚当前操作
		m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
		m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
		m_GameClientView.SetUserCallBanker(SwitchViewChairID(m_wCurrentUser), CB_NOT_CALL);
		m_GameClientView.SetUserRodBanker(SwitchViewChairID(wMeChairID), CB_NOT_ROD);
//		m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	//结束设置
	if ((GetGameStatus()==GAME_SCENE_FREE)&&(m_wCurrentUser==INVALID_CHAIR))
	{
		m_GameClientView.m_btCardPrompt1.EnableWindow(false);
		m_GameClientView.m_btCardPrompt2.EnableWindow(false);
		m_GameClientView.m_btCardPrompt3.EnableWindow(false);
		m_GameClientView.m_btCardPrompt4.EnableWindow(false);
		m_GameClientView.m_btCardPrompt5.EnableWindow(false);
		m_GameClientView.m_btCardPrompt6.EnableWindow(false);
	}
	else
	{
		//更新按钮
		UpdateButtonControl();
	}

	return 0L;
}

//发牌完成
LRESULT CGameClientEngine::OnMessageDispatchFinish(WPARAM wParam, LPARAM lParam)
{
	//状态效验
	ASSERT(GetGameStatus()==GAME_SCENE_CALL);
	if (GetGameStatus()!=GAME_SCENE_CALL) return 0L;

	//设置界面
	//m_GameClientView.SetWaitCallScore(true);
	m_GameClientView.m_btSortCard.EnableWindow(!IsLookonMode());

	WORD wMeChairID=GetMeChairID();
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		if(i == wMeChairID || m_bUserCheat[i])
		{
			//排列扑克
			m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],ST_ORDER);
			//设置扑克
			m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(m_cbHandCardData[i],m_cbHandCardCount[i]);
		}
	}

	//显示按钮
	if ((IsLookonMode()==false)&&(wMeChairID==m_wCurrentUser))
	{
		////控制按钮
		//m_GameClientView.m_btCallScore1.EnableWindow(true);
		//m_GameClientView.m_btCallScore2.EnableWindow(true);
		//m_GameClientView.m_btCallScore3.EnableWindow(true);
		//m_GameClientView.m_btCallScoreNone.EnableWindow(true);
		//m_GameClientView.m_btCallBanker.EnableWindow(true);

		////显示按钮
		//m_GameClientView.m_btCallScore1.ShowWindow(true);
		//m_GameClientView.m_btCallScore2.ShowWindow(true);
		//m_GameClientView.m_btCallScore3.ShowWindow(true);*/
		//m_GameClientView.m_btCallScoreNone.ShowWindow(true);
		//m_GameClientView.m_btCallBanker.ShowWindow(true);

		//控制按钮
		m_GameClientView.m_btCallBanker.EnableWindow(true);
		m_GameClientView.m_btNoCallBanker.EnableWindow(true);

		//显示按钮
		m_GameClientView.m_btCallBanker.ShowWindow(true);
		m_GameClientView.m_btNoCallBanker.ShowWindow(true);
	}

	//玩家控制
	if (IsLookonMode()==false)	
	{
		m_GameClientView.m_btSortCard.EnableWindow(true);
	}

	//更新按钮
	UpdateButtonControl();

	//设置时间
	//SetGameClock(m_wCurrentUser,IDI_CALL_SCORE,m_cbTimeCallScore);
	SetGameClock(m_wCurrentUser,IDI_CALL_BANKER,m_cbTimeCallBanker);

	CString str;
	str.Format(L"ddz 6666设置的叫地主时间是=%d",m_cbTimeCallBanker);
	OutputDebugString(str);


	//清楚当前操作
	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
	//m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);
	m_GameClientView.SetUserCallBanker(SwitchViewChairID(m_wCurrentUser), CB_NOT_CALL);

	m_GameClientView.InvalidGameView(0,0,0,0);

	//激活框架
	if ((IsLookonMode()==false)&&(m_bTrustee==false)) ActiveGameFrame();

	return 0L;
}

//翻牌完成
LRESULT CGameClientEngine::OnMessageReversalFinish(WPARAM wParam, LPARAM lParam)
{
	//状态效验
	ASSERT(GetGameStatus()==GAME_SCENE_PLAY);
	if (GetGameStatus()!=GAME_SCENE_PLAY) return 0L;

	//设置界面
	//m_GameClientView.SetUserCallScore(INVALID_CHAIR,0);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//设置背面
	WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
	m_GameClientView.m_HandCardControl[wViewChairID].SetBackGround(CARD_LAND);
//	m_GameClientView.SetBankerScore(m_cbBankerScore);

	WORD wMeChairID=GetMeChairID();

	//设置底牌
	if (m_wBankerUser==wMeChairID || m_bCheatRight)
	{
		//获取扑克
		BYTE cbBankerCard[3];
		m_GameClientView.m_BackCardControl.GetCardData(cbBankerCard,CountArray(cbBankerCard));

		//拷贝扑克
		m_cbHandCardCount[m_wBankerUser]=MAX_COUNT;
		CopyMemory(&m_cbHandCardData[m_wBankerUser][NORMAL_COUNT],cbBankerCard,CountArray(cbBankerCard));

		//排列扑克
		m_GameLogic.SortCardList(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser],m_cbSortType);

		//设置扑克
		m_GameClientView.m_HandCardControl[SwitchViewChairID(m_wBankerUser)].SetCardData(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser]);
		if(m_wBankerUser==wMeChairID)
		{
			//弹起扑克
			m_GameClientView.m_HandCardControl[SwitchViewChairID(m_wBankerUser)].SetShootCard(cbBankerCard,CountArray(cbBankerCard));
		}
	}
	else
	{
		//设置扑克
		m_cbHandCardCount[m_wBankerUser]=MAX_COUNT;

		//其他玩家
		WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(MAX_COUNT);
	}

	//控制设置
	if (IsLookonMode()==false)
	{
		//激活框架
		if (m_bTrustee==false) ActiveGameFrame();

		//出牌按钮
		if (m_wCurrentUser==wMeChairID)
		{
			//启用按钮
			m_GameClientView.m_btOutCard.EnableWindow(false);
			m_GameClientView.m_btPassCard.EnableWindow(false);

			//显示按钮
			m_GameClientView.m_btOutCard.ShowWindow(true);
			m_GameClientView.m_btPassCard.ShowWindow(true);
			m_GameClientView.m_btOutPrompt.ShowWindow(true);

			//搜索提示
			try
			{
				m_GameLogic.SearchOutCard(m_cbHandCardData[wMeChairID],m_cbHandCardCount[m_wCurrentUser],m_cbTurnCardData,m_cbTurnCardCount,
					&m_SearchCardResult);
			}catch(...)
			{
				ASSERT(FALSE);
				m_SearchCardResult.cbSearchCount = 0;
			}
		}
		//else if ( m_wCurrentUser != INVALID_CHAIR )
		//{
		//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
		//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
		//}


		//控制设置
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(true);
	}

	//更新按钮
	UpdateButtonControl();

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("BANKER_INFO"));

	//清楚当前操作
	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
	m_GameClientView.SetUserCallBanker(SwitchViewChairID(wMeChairID), CB_NOT_CALL);
	m_GameClientView.SetUserRodBanker(SwitchViewChairID(wMeChairID), CB_NOT_ROD);
	//m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

	//设置时间
	SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeHeadOutCard);

	//玩家控制
	if (IsLookonMode()==false)	
	{
		m_GameClientView.m_btSortCard.EnableWindow(true);
	}

	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0L;
}

//声音控制
bool CGameClientEngine::AllowBackGroundSound(bool bAllowSound )
{
	/*CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	if(bAllowSound && !m_bBackGroundSound && !pGlobalUnits->m_bMuteStatuts)
	{
		m_bBackGroundSound = true;
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_MUSIC"));
	}
	else if(!bAllowSound || pGlobalUnits->m_bMuteStatuts)
	{
		StopSound();
		m_bBackGroundSound = false;
	}*/
	if(bAllowSound && !m_bBackGroundSound )
	{
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_MUSIC"));
	}
	else if ( !bAllowSound )
	{
		StopSound();
		m_bBackGroundSound = false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
