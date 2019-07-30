#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
//辅助时间
#define TIME_LESS					1									//最少时间
#define TIME_JETTON				    2									//下注时间
#define TIME_RAND				    2									//随机时间

#define IDI_ANDROID_ITEM_SINK			500									//起始标识
//游戏时间
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			    // 游戏开始ID
#define IDI_PLACE_JETTON        (IDI_ANDROID_ITEM_SINK+1)               // 下注阶段
#define IDI_SHOW_HAND_TIME      (IDI_ANDROID_ITEM_SINK+2)               // 梭哈延迟
#define IDI_LOOK_CARD			(IDI_ANDROID_ITEM_SINK+3)               // 机器人看牌
#define IDI_GIVE_UP				(IDI_ANDROID_ITEM_SINK+4)               // 放弃
extern void OutputDebugMsg(const char * szMsg, ...);

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_bPoor = true;
	m_lTurnMaxGold=0L;						            // 最大下注
	m_lTurnBasicGold=0L;					            // 跟注数目
	m_lBasicGold=0L;						            // 单元数目	
	m_wCurrentUser=INVALID_CHAIR;						// 当前下注者
	m_lShowHandScore=0L;					            // 限制最高分
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));	// 游戏状态
	m_bShowHand = false;                                // 是否梭哈
	m_byCardCount = 0;
	m_lMeTableGold=0L;				                    // 自己所下的总数
	m_lMeTurnGold=0L;				                    // 自己一轮所下的总数
	m_lMeScore=0L;                                      // 自己携带的总数
	m_bCtrlWinner = false;                              // 自己是否为赢家
	m_bFirstUser=false;                                 // 是否为第一个人说话
	m_bWinner = false;

	//设置文件名
// 	TCHAR szPath[MAX_PATH]=TEXT("");
// 	GetCurrentDirectory(sizeof(szPath),szPath);
// 	myprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SH_Config.ini"),szPath);
	iTIME_RAND=2;iTIME_LESS=1;

	memset(m_cbHumanCard, 0, sizeof(m_cbHumanCard));
	memset(m_cbOwnCard, 0, sizeof(m_cbOwnCard));
	
	m_bAddGold=false;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	m_lTurnMaxGold=0L;						            // 最大下注
	m_lTurnBasicGold=0L;					            // 跟注数目
	m_lBasicGold=0L;						            // 单元数目	
	m_wCurrentUser=INVALID_CHAIR;						// 当前下注者
	m_lShowHandScore=0L;					            // 限制最高分
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));	// 游戏状态
	m_bShowHand = false;                                // 是否梭哈
	m_byCardCount = 0;
	m_lMeTableGold=0L;				                    // 自己所下的总数
	m_lMeTurnGold=0L;				                    // 自己一轮所下的总数
	m_lMeScore=0L;                                      // 自己携带的总数
	m_bCtrlWinner = false;                              // 自己是否为赢家
	m_bFirstUser=false;                                 // 是否为第一个人说话
	m_bAddGold=false;

	memset(m_cbHumanCard, 0, sizeof(m_cbHumanCard));
	memset(m_cbOwnCard, 0, sizeof(m_cbOwnCard));
	
	return true;
}

//时间消息
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:
		{
			//开始判断
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_PLACE_JETTON:	// 下注时间到了
		{
			return OnPlaceJetton();
		}
	case IDI_LOOK_CARD:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_LOOK_CARD);
			m_pIAndroidUserItem->SendSocketData(SUB_C_SHOW_CARD);
			return true;
		}
	case IDI_SHOW_HAND_TIME:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_SHOW_HAND_TIME);
			__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
			__int64 lGold = lShowHandScore-m_lMeTableGold;
			//发送数据
			CMD_C_AddGold AddPoint;
			AddPoint.bFllow = false;
			AddPoint.lGold=lGold;
			if (lGold>0)
			{
				m_bAddGold=true;
			}
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));
			return true;
		}
	case IDI_GIVE_UP:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_GIVE_UP);
			m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
			return true;
		}
		
	default: break;
	}
	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	       // 游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_GOLD:	//用户下注
		{
			return OnSubAddGold(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:		//用户放弃
		{
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//发牌消息
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_ANDROID_WINNER:
		{
			if (NULL == m_pIAndroidUserItem)
			{
				return true;
			}
			//效验数据
			if (wDataSize!=sizeof(CMD_S_AndroidWinner)) return false;
			CMD_S_AndroidWinner * pAndroidWinner=(CMD_S_AndroidWinner *)pBuffer;
			if (pAndroidWinner->wWinner == m_pIAndroidUserItem->GetChairID())
			{
				//TCHAR str[44];
				//sprintf(str,"wWinne=%d,GetChairID=%d",pAndroidWinner->wWinner,m_pIAndroidUserItem->GetChairID());
					//AfxMessageBox(str);
				m_bCtrlWinner = true;
			}
			
			return true;
		}
	case SUB_S_SHOW_CARD:
		{
			return true;
		}
	case SUB_S_SEND_HUMAN_CARD:
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_ShowHumanCard)) return false;
			CMD_S_ShowHumanCard* pShowUserCard = (CMD_S_ShowHumanCard*) pBuffer;
			memcpy(m_cbHumanCard, pShowUserCard->cbUserCard, pShowUserCard->cbCardCount);

			return true;
		}
	default:break;
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_TK_FREE:		//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lTurnMaxGold=0L;						            // 最大下注
			m_lTurnBasicGold=0L;					            // 跟注数目
			m_lBasicGold=0L;						            // 单元数目	
			m_wCurrentUser=INVALID_CHAIR;						// 当前下注者
			m_lShowHandScore=0L;					            // 限制最高分
			ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));	// 游戏状态
			m_bShowHand = false;                                // 是否梭哈
			m_byCardCount = 0;
			m_lMeTableGold=0L;				                    // 自己所下的总数
			m_lMeTurnGold=0L;				                    // 自己一轮所下的总数
			m_lMeScore=0L;                                      // 自己携带的总数
			m_bCtrlWinner = false;                              // 自己是否为赢家
			m_bFirstUser=false;                                 // 是否为第一个人说话
			m_bPoor = pStatusFree->bPoor;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//玩家设置
			if ((NULL != pIServerUserItem) && (pIServerUserItem->GetUserStatus()!=US_READY))
			{
				UINT nElapse=rand()%iTIME_RAND+iTIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	case GAME_STATUS_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//设置变量
			m_lTurnMaxGold=pStatusPlay->lTurnMaxGold;
			m_lTurnBasicGold=pStatusPlay->lTurnBasicGold;
			m_lBasicGold = pStatusPlay->lBasicGold;
			m_bPoor = pStatusPlay->bPoor;
			CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
			m_wCurrentUser = pStatusPlay->wCurrentUser;
			m_lShowHandScore = pStatusPlay->lShowHandScore;
			m_bShowHand = pStatusPlay->bShowHand?true:false;
			m_byCardCount = pStatusPlay->bTableCardCount[m_pIAndroidUserItem->GetChairID()];
			m_lMeTableGold=pStatusPlay->lTableGold[m_pIAndroidUserItem->GetChairID()*2 + 1];	// 自己所下的总数
			m_lMeTurnGold=pStatusPlay->lTableGold[m_pIAndroidUserItem->GetChairID()*2];			// 自己一轮所下的总数
			m_lMeScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
			m_bCtrlWinner = false;                                                              // 自己是否为赢家
			m_bFirstUser=false;                                                                 // 是否为第一个人说话
			if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
			{
				// 下注
				UINT uElapse = iTIME_LESS + (rand()%iTIME_RAND);
				m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
			}			
			return true;
		}
	}
	return true;
}

//用户进入
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

// 所有玩家都开始了
void CAndroidUserItemSink::OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	if (NULL != m_pIAndroidUserItem)
	{
		UINT nElapse=rand()%iTIME_RAND+iTIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
	}
	return ;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	iTIME_LESS=1;//GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_LESS"),TIME_LESS,m_szConfigFileName);
	iTIME_RAND=2;//GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_RAND"),TIME_RAND,m_szConfigFileName);

	//游戏变量
	m_lTurnMaxGold=pGameStart->lTurnMaxGold;
	m_lTurnBasicGold=pGameStart->lTurnBasicGold;
	m_lBasicGold = pGameStart->lBasicGold;
	m_bPoor = pGameStart->bPoor;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lShowHandScore = pGameStart->lShowHandScore;
	m_bShowHand=false;
	m_byCardCount = 2;  // 两张底牌
	m_lMeTableGold=pGameStart->lBasicGold;			// 自己所下的总数
	m_lMeTurnGold=0L;				                // 自己一轮所下的总数
	//m_bCtrlWinner = false;                          // 自己是否为赢家
	m_bAddGold=false;

	if (NULL == m_pIAndroidUserItem)
	{
		return true;
	}

	m_cbOwnCard[0] = pGameStart->bFundusCard;				//自己底牌
	m_cbOwnCard[1] = pGameStart->bCardData[m_wCurrentUser];	//自己明牌
	m_cbHumanCard[1] = pGameStart->bCardData[(m_wCurrentUser+1) % GAME_PLAYER];	//对方明牌
	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_PLAY);
	m_lMeScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		const IServerUserItem *pServerUserItem=m_pIAndroidUserItem->GetTableUserItem(i);

		//设置界面
		if (NULL != pServerUserItem)
		{
			m_bPlayStatus[i]=TRUE;
		}
		else
		{
			m_bPlayStatus[i]=FALSE;
		}
	}

	//游戏开始机器人看牌
	UINT uElapse = TIME_LESS+rand()%TIME_RAND;
	m_pIAndroidUserItem->SetGameTimer(IDI_LOOK_CARD, uElapse);

	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bFirstUser=true;                              // 是否为第一个人说话
		// 下注
		UINT uElapse = TIME_LESS+rand()%TIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
	}
	else
	{
		m_bFirstUser=false;                              // 是否为第一个人说话
	}
	return true;
}

//用户加注
bool CAndroidUserItemSink::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_AddGold)) return false;
	CMD_S_AddGold * pAddGold=(CMD_S_AddGold *)pBuffer;

	//变量定义
	m_wCurrentUser = pAddGold->wCurrentUser;

	//处理数据
	m_lTurnBasicGold=pAddGold->lCurrentLessGold;
	m_bShowHand = pAddGold->bShowHand;
	if (pAddGold->wLastChairID == m_pIAndroidUserItem->GetChairID())
	{
		m_lMeTurnGold = pAddGold->lLastAddGold;
	}

	//其他处理
	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		// 下注TIME_RANDTIME_LESS
		UINT uElapse = TIME_LESS+rand()%TIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
		//m_bAddGold=true;
	}
	return true;
}

//用户放弃
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//设置变量
	m_bPlayStatus[pGiveUp->wUserChairID]=FALSE;
	if (pGiveUp->wUserChairID==m_pIAndroidUserItem->GetChairID())
	{
		m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_FREE);
		m_bAddGold=false;
	}

	//更新数据
	if( m_lTurnMaxGold != pGiveUp->lTurnMaxGold )
	{
		//设置数据
		m_lTurnMaxGold = pGiveUp->lTurnMaxGold;
	}
	return true;
}

//发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_R_SendCard)) return false;
	CMD_R_SendCard * pSendCard=(CMD_R_SendCard *)pBuffer;

	WORD wRand = 0;
	if(pSendCard->cbSendCardCount <= 1)
	{
		wRand = 99;
	}
	else if(pSendCard->cbSendCardCount == 2)
	{
		wRand =  rand()%100;
	}
	else if(pSendCard->cbSendCardCount == 3)
	{
		wRand = rand()%70;
	}
	else
		wRand = rand()%60;

	if(wRand >= 50)
	{
		UINT uElapse = TIME_LESS+rand()%TIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_LOOK_CARD, uElapse);

	}

	//设置变量
	m_lTurnBasicGold=0L;
	m_lTurnMaxGold=pSendCard->lMaxGold;
	m_wCurrentUser = pSendCard->wCurrentUser;
	m_lMeTableGold += m_lMeTurnGold;
	m_lMeTurnGold = 0L;
	
	m_bAddGold=false;
	m_bWinner = (pSendCard->wWin == m_pIAndroidUserItem->GetChairID());
	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bFirstUser=true;                              // 是否为第一个人说话
		// 下注
		UINT uElapse = TIME_LESS+rand()%TIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
	}
	else
	{
		m_bFirstUser = false;
	}
	memcpy(m_cbOwnCard+m_byCardCount, pSendCard->bUserCard[m_pIAndroidUserItem->GetChairID()], pSendCard->cbSendCardCount);
	m_byCardCount ++;

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_FREE);
	m_pIAndroidUserItem->KillGameTimer(IDI_PLACE_JETTON);

	m_lTurnMaxGold=0L;						            // 最大下注
	m_lTurnBasicGold=0L;					            // 跟注数目
	m_lBasicGold=0L;						            // 单元数目	
	m_wCurrentUser=INVALID_CHAIR;						// 当前下注者
	m_lShowHandScore=0L;					            // 限制最高分
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));	// 游戏状态
	m_bShowHand = false;  
	m_lMeTableGold = 0L;
	m_lMeTurnGold = 0L;
	m_lMeScore = 0L;
	m_bAddGold=false;
	m_bCtrlWinner = false;                          // 自己是否为赢家
	m_bWinner = false;
	// 是否梭哈
	// 下注
	UINT uElapse = iTIME_LESS+rand()%iTIME_RAND;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, uElapse);
	return true;
}

// 下注处理
bool CAndroidUserItemSink::OnPlaceJetton(void)
{
	if (NULL == m_pIAndroidUserItem)
	{
		return true;
	}

	// 判断自己是否为下注玩家
	if ( m_pIAndroidUserItem->GetChairID() == m_wCurrentUser 
		&& (US_LOOKON == m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus())) 
	{
		return true;
	}

	bool bAdd1=false;  // 是否可以加注1
	bool bAdd2=false;  // 是否可以加注2
	bool bAdd3=false;  // 是否可以加注3
	bool bShowhand=false; // 是否可以梭哈
	bool bGiveUp = true; // 是否可以放弃	
	bool bFollow=true;

	__int64 iGold = 0;
	__int64 lBasicGold = 0;
	if(!m_bPoor)
		lBasicGold = m_lBasicGold;
	else
		lBasicGold = m_lBasicGold/2;
	TCHAR strMessage[222]=_T("");
	//小于3张牌
	if (m_byCardCount<3)
	{
		//是赢家
		if (m_bWinner)
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_lTurnBasicGold != 0)
				{
					if (iRand<20)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<30)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<40)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						iGold= m_lTurnBasicGold;
					}
				}else
				{
					if (iRand<20)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<40)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<60)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						iGold= m_lTurnBasicGold;
					}
				}
				
			}else
			{
				if (m_bAddGold)
				{
					iGold= m_lTurnBasicGold;
				}
				else
				{
					if (iRand<20)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<40)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<80)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						iGold= m_lTurnBasicGold;
					}
				}
			}				
		}else
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (iRand<40)
				{
					iGold= m_lTurnBasicGold;
				}else
				{
					iGold= -1;
				}

			}else
			{
				if (m_bAddGold)
				{
					if (iRand<50)
					{
						iGold= m_lTurnBasicGold;
					}else
					{
						iGold= -1;
					}
				}
				else
				{
					if (iRand<20)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<40)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<80)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						iGold= m_lTurnBasicGold;
					}
				}
			}		
		}
	}else if (m_byCardCount<4)
	{
		//是赢家
		if (m_bWinner)
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					if (iRand<30)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
					
				}else
				{
					if (iRand<40)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<50)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<60)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<70)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					if (iRand<30)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}

				}else
				{
					if (m_bAddGold)
					{
						
						if (iRand<50)
						{
							iGold= m_lTurnBasicGold;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
					else
					{
						if (iRand<20)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<40)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else  if (iRand<50)
						{
							iGold= m_lTurnBasicGold+1*lBasicGold;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
				}				

				
			}				
		}else
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					iGold= -1;

				}else
				{
					if (iRand<50)
					{
						iGold= m_lTurnBasicGold;
					}else
					{
						iGold= -1;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					if (iRand<80)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}

				}else
				{
					if (m_bAddGold)
					{

						if (iRand<60)
						{
							iGold= m_lTurnBasicGold;
						}else
						{
							iGold= -1;
						}
					}
					else
					{
						if (iRand<20)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<40)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else
						{
							iGold= m_lTurnBasicGold;
						}
					}
				}				


			}		
		}
	}
	else if (m_byCardCount<5)
	{
		//是赢家
		if (m_bWinner)
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					if (iRand<30)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}

				}else
				{
					if (iRand<30)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<50)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<60)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<70)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					iGold = lShowHandScore-m_lMeTableGold;

				}else
				{
					if (m_bAddGold)
					{

						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;

					}
					else
					{
						if (iRand<10)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<20)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else  if (iRand<30)
						{
							iGold= m_lTurnBasicGold+1*lBasicGold;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
				}				


			}				
		}else
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					iGold= -1;

				}else
				{
					if (iRand<40)
					{
						iGold= m_lTurnBasicGold;
					}else
					{
						iGold= -1;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					if (iRand<80)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}

				}else
				{
					if (m_bAddGold)
					{

						if (iRand<60)
						{
							iGold= m_lTurnBasicGold;
						}else
						{
							iGold= -1;
						}
					}
					else
					{
						if (iRand<40)
						{
							iGold= m_lTurnBasicGold;
						}else if (iRand<60)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<80)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else
						{
							iGold= m_lTurnBasicGold;
						}
					}
				}				


			}		
		}
	}
	else
	{
		myprintf(strMessage,sizeof(strMessage),_T("大于4张牌===================================================="));
		WriteErrorRecord(strMessage);

		//是赢家
		if (m_bWinner)
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					iGold = lShowHandScore-m_lMeTableGold;

				}else
				{
					if (iRand<30)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<50)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<60)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<70)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					if (iRand<10)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
				}else
				{
					if (m_bAddGold)
					{

						if (iRand<50)
						{
							iGold= -1;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
					else
					{
						if (iRand<20)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<30)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else  if (iRand<40)
						{
							iGold= m_lTurnBasicGold+1*lBasicGold;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
				}				


			}				
		}else
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					iGold= -1;

				}else
				{
					if (iRand<40)
					{
						iGold= m_lTurnBasicGold;
					}else
					{
						iGold= -1;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					iGold= -1;	
				}else
				{
					if (m_bAddGold)
					{

						if (iRand<40)
						{
							iGold= m_lTurnBasicGold;
						}else
						{
							iGold= -1;
						}
					}
					else
					{
						if (iRand<30)
						{
							iGold= m_lTurnBasicGold;
						}else if (iRand<40)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else if (iRand<60)
						{
							iGold= m_lTurnBasicGold+1*lBasicGold;
						}else
						{
							iGold= m_lTurnBasicGold;
						}
					}
				}				


			}		
		}
	}

	int nValue = m_GameLogic.CompareCard( m_cbOwnCard, m_byCardCount, m_cbHumanCard+1, m_byCardCount-1);
	if(nValue >= 3)
	{
		if(iGold <= 0)
			iGold += m_lTurnBasicGold;
	}
	else if(nValue < -1)
	{
		if(m_byCardCount == 3 && iGold > 0)
		{
			if(iGold > 0)
			{
				if(rand() % 100 < 50)
					iGold = 0;
			}
		}
		if(m_byCardCount == 4)
		{
			if(iGold > 0)
			{
				if(rand() % 100 < 80)
					iGold = -1;
				else
					iGold = 0;
			}
		}

	}
	
	if(m_byCardCount == 5)
		if( nValue < 0)
		{
			iGold= -100;
		}

	//判断大小，进行干预
	/*
	int nValue = m_GameLogic.CompareCard(自己的扑克,m_byCardCount, 对方扑克（不含底牌）, 对方扑克个数)) ;
	if((nValue>= 3)
	{
		//明显比对方要大啊
		不要放弃；积极加注。
	}
	else if(nValue <= 3)
	{
		//明显要比对方小啊
		m_byCardCount == 2 时，不干预；
		m_byCardCount == 3 时，50%放弃，50%跟注；
		m_byCardCount == 4 时，80%放弃；20%跟注；如果对方已经加注，则放弃
		m_byCardCount == 5 时，100%放弃
	}

	// 如果5张牌都比别人4张牌还小了，就放弃吧
	if(m_byCardCount == 5 && nValue < 0)
		100%放弃
	*/
	if (iGold < 0)
	{
		BYTE byRand = rand()%5+1;
		m_pIAndroidUserItem->SetGameTimer(IDI_GIVE_UP,byRand);
		return true;
	}

	__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
	if(iGold == lShowHandScore-m_lMeTableGold)
	{
 		BYTE byRand = rand()%5+1;
 		m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_HAND_TIME,byRand);
	}
	else
	{
		//发送数据
		CMD_C_AddGold AddPoint;
		AddPoint.bFllow = m_bAddGold;
		AddPoint.lGold=iGold;
		if (iGold>0)
		{
			m_bAddGold=true;
		}
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	}

	return true;
}

//写入记录
void CAndroidUserItemSink::WriteErrorRecord(LPCTSTR pszErrorMsg)
{
	try
	{
		//设置文件名
		TCHAR szPath[MAX_PATH]=TEXT("");
		TCHAR szFileName[MAX_PATH];
		GetCurrentDirectory(sizeof(szPath),szPath);
		myprintf(szFileName,sizeof(szFileName),TEXT("%s\\机器人.txt"),szPath);

		//打开文件
		CFile FileRecord;
		if (FileRecord.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			FileRecord.SeekToEnd();
			FileRecord.Write(pszErrorMsg,mystrlen(pszErrorMsg));
			FileRecord.Write(TEXT("\r\n"),mystrlen(TEXT("\r\n")));

			FileRecord.Close();
		}
	}
	catch(...){}
}

//////////////////////////////////////////////////////////////////////////
