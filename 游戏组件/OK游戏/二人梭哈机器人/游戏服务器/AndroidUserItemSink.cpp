#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
//辅助时间
#define TIME_LESS					2									//最少时间
#define TIME_JETTON				    5									//下注时间
#define TIME_RAND				    8									//随机时间

//游戏时间
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			    // 游戏开始ID
#define IDI_PLACE_JETTON        (IDI_ANDROID_ITEM_SINK+1)               // 下注阶段

extern void OutputDebugMsg(const char * szMsg, ...);

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
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

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SH_Config.ini"),szPath);
	iTIME_RAND=8;iTIME_LESS=2;

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
	default: break;
	}
	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
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
				m_bCtrlWinner = true;
			}
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

			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//玩家设置
			if ((NULL != pIServerUserItem) && (pIServerUserItem->GetUserStatus()!=US_READY))
			{
				UINT nElapse=rand()%iTIME_RAND+iTIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	case GS_PLAYING:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//设置变量
			m_lTurnMaxGold=pStatusPlay->lTurnMaxGold;
			m_lTurnBasicGold=pStatusPlay->lTurnBasicGold;
			m_lBasicGold = pStatusPlay->lBasicGold;
			CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
			m_wCurrentUser = pStatusPlay->wCurrentUser;
			m_lShowHandScore = pStatusPlay->lShowHandScore;
			m_bShowHand = pStatusPlay->bShowHand?true:false;
			m_byCardCount = pStatusPlay->bTableCardCount[m_pIAndroidUserItem->GetChairID()];
			m_lMeTableGold=pStatusPlay->lTableGold[m_pIAndroidUserItem->GetChairID()*2 + 1];	// 自己所下的总数
			m_lMeTurnGold=pStatusPlay->lTableGold[m_pIAndroidUserItem->GetChairID()*2];			// 自己一轮所下的总数
			m_lMeScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore;
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

	iTIME_LESS=GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_LESS"),TIME_LESS,m_szConfigFileName);
	iTIME_RAND=GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_RAND"),TIME_RAND,m_szConfigFileName);

	//游戏变量
	m_lTurnMaxGold=pGameStart->lTurnMaxGold;
	m_lTurnBasicGold=pGameStart->lTurnBasicGold;
	m_lBasicGold = pGameStart->lBasicGold;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lShowHandScore = pGameStart->lShowHandScore;
	m_bShowHand=false;
	m_byCardCount = 2;  // 两张底牌
	m_lMeTableGold=pGameStart->lBasicGold;			// 自己所下的总数
	m_lMeTurnGold=0L;				                // 自己一轮所下的总数
	m_bCtrlWinner = false;                          // 自己是否为赢家

	if (NULL == m_pIAndroidUserItem)
	{
		return true;
	}

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_PLAYING);
	m_lMeScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore;

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

	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bFirstUser=true;                              // 是否为第一个人说话
		// 下注
		UINT uElapse = iTIME_LESS+rand()%iTIME_RAND;
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
		// 下注
		UINT uElapse = iTIME_LESS+rand()%iTIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
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
		m_pIAndroidUserItem->SetGameStatus(GS_FREE);
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

	//设置变量
	m_lTurnBasicGold=0L;
	m_lTurnMaxGold=pSendCard->lMaxGold;
	m_wCurrentUser = pSendCard->wCurrentUser;
	m_lMeTableGold += m_lMeTurnGold;
	m_lMeTurnGold = 0L;
	m_byCardCount ++;	
	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bFirstUser=true;                              // 是否为第一个人说话
		// 下注
		UINT uElapse = iTIME_LESS+rand()%iTIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
	}
	else
	{
		m_bFirstUser = false;
	}
	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_FREE);
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
	if (m_bShowHand)
	{
		// 只能梭哈，跟放弃
		bShowhand = true;
		bGiveUp = true;
		bFollow =false;
		int iRand = rand()%100;
		if (iRand > 80)
		{
			__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
			iGold = lShowHandScore-m_lMeTableGold;
		}
		else
		{
			// 如果通知是必赢的时候，自动放弃的机会很少
			if (m_bCtrlWinner && iRand > 8)
			{
				__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
				iGold = lShowHandScore-m_lMeTableGold;
			}
			else
			{
				iGold = -1;
			}
		}
	}
	else
	{
		// 普通下注，应该是跟注比较多
		__int64 lLeaveScore=m_lTurnMaxGold-m_lMeTableGold-m_lMeTurnGold;
		if ((m_byCardCount >= 3) && (lLeaveScore>0))
		{
			bShowhand = true;
		}
		lLeaveScore=m_lTurnBasicGold-m_lMeTurnGold;
		__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
		if ((0 == m_lTurnBasicGold) || ((lLeaveScore>0L) && (lShowHandScore>m_lTurnBasicGold+m_lMeTableGold)))
		{
			bFollow = true;
		}
		else
		{
			bFollow = false;
		}
		lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_lMeTableGold;
		bAdd1 = (lLeaveScore>=m_lBasicGold)?true:false;
		bAdd2 = (lLeaveScore>=(2*m_lBasicGold))?true:false;
		bAdd3 = (lLeaveScore>=(3*m_lBasicGold))?true:false;	
		// 有加注一般情况下为加注
		int iRand = rand()%100;
		if (bAdd1)
		{
			if ((iRand > 96) || (!m_bCtrlWinner && iRand>90 && m_byCardCount>3))
			{
				iGold = -1;
			}
			else if (((iRand>82) || (m_bCtrlWinner && iRand>75))  && bAdd3)
			{
				iGold= m_lTurnBasicGold+3*m_lBasicGold;
			}
			else if (((iRand>70) || (m_bCtrlWinner && iRand>67))  && bAdd2)
			{
				iGold = m_lTurnBasicGold+2*m_lBasicGold;
			}
			else if (iRand > 18 && bFollow)
			{
				if (0 == m_lMeTurnGold)
				{
					// 不是必赢可以跟零
					if (!m_bCtrlWinner && iRand>48)
					{
						iGold=m_lMeTurnGold;
					}
					else
					{
						iGold = m_lTurnBasicGold+m_lBasicGold;
					}
				}
				else
				{
					//获取财富
					iGold=m_lMeTurnGold;
				}				
			}
			else if (iRand > 10 && bShowhand) // 有机会梭哈
			{
				if (m_bFirstUser || (iRand > 14))
				{
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					iGold = lShowHandScore-m_lMeTableGold;
				}
				else
				{
					iGold = m_lTurnBasicGold+m_lBasicGold;
				}
			}
			else
			{
				iGold = m_lTurnBasicGold+m_lBasicGold;
			}

			if (iGold >= 0)
			{
				__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
				if (iGold+m_lMeTableGold < lShowHandScore)
				{
					iGold=__max(iGold,m_lTurnBasicGold);
				}
			}
		}
		else
		{
			if (iRand>95) // 放弃
			{
				iGold = -1;
			}
			else if (iRand >88)
			{
				if (!m_bShowHand)
				{
					iGold = -1;
				}
				else
				{
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					iGold = lShowHandScore-m_lMeTurnGold;
				}
			}
			else
			{
				if (bFollow)
				{
					//获取财富
					iGold=m_lMeTurnGold;
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					if( iGold+m_lMeTableGold < lShowHandScore )
						iGold=__max(iGold,m_lTurnBasicGold);
				}
				else
				{
					iGold = -1;
				}
			}
		}
	}

	if (iGold < 0)
	{
		if (m_bFirstUser && !m_bShowHand)
		{
			iGold= m_lTurnBasicGold+m_lBasicGold;
			__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
			if( iGold+m_lMeTableGold < lShowHandScore )
				iGold=__max(iGold,m_lTurnBasicGold);
		}
		else
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
			return true;
		}
	}
	//发送数据
	CMD_C_AddGold AddPoint;
	AddPoint.lGold=iGold;
	m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	return true;
}

//////////////////////////////////////////////////////////////////////////
