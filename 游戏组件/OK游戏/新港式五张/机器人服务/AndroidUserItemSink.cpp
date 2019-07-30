#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					2									//最少时间
#define TIME_DISPATCH				2									//发牌时间

//游戏时间
#define TIME_OUT_CARD				2									//出牌时间
#define TIME_START_GAME				3									//开始时间
#define TIME_CHIP_OP                2                                   //下注时间
#define TIME_OUT                    8                      

//游戏时间
#define IDI_OUT_CARD				(0)			//出牌时间
#define IDI_START_GAME				(1)			//开始时间
#define IDI_CHIP_OP                 (2)         //下注时间
#define IDI_OUT_TIME                (3)         //超时

//银行定时
#define IDI_BANK_OPERATE			4									//银行定时
//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lCellScore = 0;
	m_bFirstChip = false;
	m_lTurnLessScore = 0;
	m_lDrawMaxScore = 0;
	m_nCurCardCount = 0;
	m_lTurnMaxScore = 0;
	m_bGiveUp = false;
	m_bShowHand = false;
	m_bTemp=false;
	m_wCurrentUser = INVALID_CHAIR;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
    ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCard,sizeof(m_cbHandCard));
	ZeroMemory(m_cbTableCardData,sizeof(m_cbTableCardData));
	m_wSpeciaChairID = 0;

	m_lBeforeUserScore = 0;

	//接口变量
	m_pIAndroidUserItem=NULL;;

	return;
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
	//游戏变量
	m_lCellScore = 0;
	m_bFirstChip = false;
	m_lTurnLessScore = 0;
	m_lDrawMaxScore = 0;
	m_lTurnMaxScore = 0;
	m_nCurCardCount = 0;
	m_bGiveUp = false;
	m_wCurrentUser = INVALID_CHAIR;
	m_lBeforeUserScore = 0;


	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCard,sizeof(m_cbHandCard));

	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME: //开始游戏
		{
			//开始判断
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			 m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);

			return true;
		}
	case IDI_CHIP_OP: //下注操作
		{
			if(m_bGiveUp == true) return true;

			//LONGLONG lAddChip = GetAddChip();
 			OnSubAddScoreEx();
			return true;
		}
	case IDI_OUT_TIME: //超时
		{
			if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
			{
               m_pIAndroidUserItem->KillGameTimer(IDI_CHIP_OP);
			   m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
			}

			return true;
		}
	case IDI_BANK_OPERATE:		//银行操作
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_BANK_OPERATE);  //删除时钟

			//变量定义
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();  //获取自已指针
			LONGLONG lRobotScore = pUserItem->GetUserScore();					//读取金币
			{

				//判断存取
				if (lRobotScore > m_lRobotScoreRange[1])   //金币大于最大值  存款
				{
					LONGLONG lSaveScore=0L;

					lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);  //计算百分比
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0)
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);			//存款

				}
				else if (lRobotScore < m_lRobotScoreRange[0])     //取款
				{

					//梦成网络修改
					//随机种子
					srand( (unsigned)time(NULL));
					
					SCORE lScore=m_lRobotBankGetScore+(rand()%(m_lRobotBankGetScoreBanker-m_lRobotBankGetScore));
					if (lScore > 0)
						m_pIAndroidUserItem->PerformTakeScore(lScore);
				}
			}
			return true;
		}
	
	}

	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START://游戏开始
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_OUT_TIME,TIME_OUT);
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_GIVE_UP: //放弃操作
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_OUT_TIME,TIME_OUT);
            return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//用户加注
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_OUT_TIME,TIME_OUT);
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//发送扑克
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_OUT_TIME,TIME_OUT);
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//游戏结束
		{
			return OnSubGameEnd(pData,wDataSize);
		}
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
	case GAME_SCENE_FREE:	//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//变量定义
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

		//	m_lCellScore = pStatusFree->lCellScore;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			//消息处理
			ReadConfigInformation();

			//玩家设置
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}

			return true;
		}
	case GAME_SCENE_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

	/*		m_lCellScore = pStatusPlay->lCellScore;

			m_lDrawMaxScore = pStatusPlay->lDrawMaxScore;
			m_lTurnLessScore = pStatusPlay->lTurnLessScore;
			m_lTurnMaxScore = pStatusPlay->lTurnMaxScore;*/

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));
			ReadConfigInformation();
		
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
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

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_GameStart1));
	if (wDataSize!=sizeof(CMD_S_GameStart1)) return false;

	//变量定义
	CMD_S_GameStart1 * pGameStart=(CMD_S_GameStart1 *)pData;

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_PLAY);

	//随机种子
	 srand(GetTickCount());

	//更新变量
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lCellScore = pGameStart->lCellScore;
	m_lTurnLessScore=pGameStart->lTurnLessScore;
	m_lDrawMaxScore=pGameStart->lDrawMaxScore;
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	CopyMemory(m_cbTableCardData,pGameStart->cbTableCardData,sizeof(m_cbTableCardData));
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbOtherCard,sizeof(m_cbOtherCard));
	m_bWillWin = pGameStart->bWillWin;
	

	//加载玩家的牌
	for(int i=0;i<GAME_PLAYER;i++)
	{
		//if(i==m_pIAndroidUserItem->GetChairID()) continue;


		m_cbOtherCard[i].bGiveUp = false;
        
		if(pGameStart->cbCardData[i]==0)
		{
			m_cbOtherCard[i].bStatue = false;
			continue;
		}

        m_cbOtherCard[i].cbCard[1] = pGameStart->cbCardData[i];

		m_cbOtherCard[i].bStatue = true;

		m_lTableScore[i]=pGameStart->lCellScore;
	}

	m_cbHandCard[0] = pGameStart->cbObscureCard;
	m_cbHandCard[1] = pGameStart->cbCardData[m_pIAndroidUserItem->GetChairID()];
	m_lTableScore[m_pIAndroidUserItem->GetChairID()]=pGameStart->lCellScore;
	m_nCurCardCount = 2;
    m_bFirstChip = true;
	m_bGiveUp = false;
	m_bAddChip = true;

	if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=rand()%TIME_CHIP_OP+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse+TIME_DISPATCH);
	}
	return true;
}

bool CAndroidUserItemSink::OnSubGiveUp(VOID * pData, WORD wDataSize)
{

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GiveUp));
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;

	//变量定义
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pData;

	//变量定义
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;

	m_cbOtherCard[wGiveUpUser].bGiveUp = true;

	//下注变量
	m_lDrawMaxScore=pGiveUp->lDrawMaxScore;
	m_lTurnMaxScore=pGiveUp->lTrunMaxScore;

	//状态变量
	if(wGiveUpUser==m_pIAndroidUserItem->GetChairID()) m_bGiveUp = true;

	//设置用户
	m_wCurrentUser=pGiveUp->wCurrentUser;
    
	if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID() && m_pIAndroidUserItem->GetChairID() != pGiveUp->wGiveUpUser)
	{
		m_pIAndroidUserItem->KillGameTimer(IDI_CHIP_OP);
		UINT nElapse=rand()%TIME_CHIP_OP+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse+TIME_DISPATCH);
	}

	return true;
}

//用户加注
bool CAndroidUserItemSink::OnSubAddScore(VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;

	//变量定义
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pData;

	//变量定义
	WORD wAddScoreUser=pAddScore->wAddScoreUser;


	//设置变量

	m_bFirstChip = false;
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;

	m_bShowHand=(abs(m_lTurnLessScore-m_lDrawMaxScore)<0.001);

	//m_bShowHand = pAddScore->bShowHand;
	SCORE tem = pAddScore->lUserScoreCount;
	if(pAddScore->lUserScoreCount==0) m_bAddChip = false;
	else m_bAddChip = true;

	SCORE lCurrentScore=pAddScore->lUserScoreCount;

	m_lBeforeUserScore = lCurrentScore;


	m_lUserScore[wAddScoreUser]=lCurrentScore;

	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
//		m_pIAndroidUserItem->SendChatMessage(m_pIAndroidUserItem->GetChairID(),TEXT("开始操作"),RGB(0,0,0));
		UINT nElapse=rand()%TIME_CHIP_OP+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse+TIME_DISPATCH);
	}
	return true;
}

//发送扑克
bool CAndroidUserItemSink::OnSubSendCard(VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pData;

	//设置变量

	m_wCurrentUser=pSendCard->wCurrentUser;
	m_lTurnMaxScore=pSendCard->lTurnMaxScore;

	//汇集金币
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//桌面筹码
		m_lTableScore[i]+=m_lUserScore[i];

		//用户筹码
		m_lUserScore[i]=0L;
	}
    
	//派发扑克
	int nCurSendCount = m_nCurCardCount;
	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		for (BYTE i=0;i<pSendCard->cbSendCardCount;i++)
		{
			m_cbHandCard[nCurSendCount++] = pSendCard->cbCardData[i][m_pIAndroidUserItem->GetChairID()];
		}
	}

	//其它玩家
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(i==m_pIAndroidUserItem->GetChairID()) continue;

		if(m_cbOtherCard[i].bStatue == false) continue;
        
		nCurSendCount = m_nCurCardCount;

		//派发扑克
		for (BYTE j=0;j<pSendCard->cbSendCardCount;j++)
		{
			m_cbOtherCard[i].cbCard[nCurSendCount++] = pSendCard->cbCardData[j][i];
		}
	}

	m_nCurCardCount += pSendCard->cbSendCardCount;

	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=rand()%TIME_CHIP_OP+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse+TIME_DISPATCH);
	}

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//变量定义
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//结束设置
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_FREE);
	m_bShowHand = false;

	//停止操作
	m_pIAndroidUserItem->KillGameTimer(IDI_CHIP_OP);

	//机器人加钱
	m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE,rand()%TIME_START_GAME+TIME_LESS);
	
	//开始设置
	UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}
void CAndroidUserItemSink::OnSubAddScoreEx()
{
	switch(m_nCurCardCount)
	{
	case 2:
		{

			if (WinIsAndroid(1,1)) 
			{
				if (GaiLv(50))	//一个底牌一个名牌如果牌面上的牌大于玩家的牌面有百分之55加注
				{
					AddScore();
					return;
				}
				else	//fllow
				{
					FllowScore(); //一个底牌一个名牌如果牌面上的牌大于玩家的牌面有百分之45跟注
					return;
				}
			}
			else
			{
				if(m_bWillWin)
				{
					if (GaiLv(80))	//一个底牌一个名牌如果牌面上的牌小于玩家的牌面有百分之70跟注
					{
						FllowScore();
						return;
					}
					else 
					{
						if (GaiLv(100))	//一个底牌一个名牌如果牌面上的牌小于玩家的牌面有百分之（100-70）×80=24%加注
						{
							AddScore();
							return;
						}
						
					}
				}
				else
				{
					if (GaiLv(70))	//一个底牌一个名牌如果牌面上的牌小于玩家的牌面有百分之70跟注
					{
						FllowScore();
						return;
					}
					else 
					{
						if (GaiLv(80))	//一个底牌一个名牌如果牌面上的牌小于玩家的牌面有百分之（100-70）×80=24%加注
						{
							AddScore();
							return;
						}
						else	//give up
						{
							GiveUpScore();//一个底牌一个名牌如果牌面上的牌小于玩家的牌面有百分之（100-70）×20=6%放弃
							return;
						}
					}
				}


			}
		}
	case 3:
		{
			if (WinIsAndroid(1,2))
			{
				if (GaiLv(50))	//一个底牌两张名牌如果牌面上的牌大于玩家的牌面有百分之50加注
				{
					AddScore();
					return;
				}
				else
				{
					int type=m_GameLogic.GetCardGenre(m_cbHandCard,3);

					if(type>CT_SINGLE)
						AddScore();//一个底牌两张名牌如果牌面上的牌大于玩家的牌面有百分之50跟注
					else
						FllowScore();

					return;
				}
			}
			else
			{
				
					if(m_bWillWin)
					{
						if (GaiLv(100))//一个底牌两张名牌如果牌面上的牌小于玩家的牌面有百分之50跟注
						{
							FllowScore();
							return;
						}
						else
						{
							if (GaiLv(100))//一个底牌两张名牌如果牌面上的牌小于玩家的牌面有百分之（100-50）×80=40%加注
							{
								AddScore();
								return;
							}
					
						}
					}
					else
					{
						if (GaiLv(50))//一个底牌两张名牌如果牌面上的牌小于玩家的牌面有百分之50跟注
						{
							FllowScore();
							return;
						}
						else
						{
							if (GaiLv(10))//一个底牌两张名牌如果牌面上的牌小于玩家的牌面有百分之（100-50）×80=40%加注
							{
								AddScore();
								return;
							}
							else
							{
								GiveUpScore();//一个底牌两张名牌如果牌面上的牌小于玩家的牌面有百分之（100-50）×20=10%放弃
								return;
							}
						}
					}

				
			}
		}
	case 4:
		{
			/*SendSuoHand();
			return;*/
			m_bTemp=true;
			if (m_bShowHand) //如果是梭哈标准  //修改 edit 
			{
				if(m_bWillWin)
				{
					if (WinIsAndroid(1,3))
					{
						int type=m_GameLogic.GetCardGenre(m_cbHandCard,4);
						if(type>CT_SINGLE)
						{
							if (GaiLv(100))
							{
								FllowScore();//如果牌面上的牌大于玩家牌面上的牌有90%的跟注
								return;
							}
						}
						else
						{
							GiveUpScore();//如果牌面上的牌大于玩家牌面上的牌有10%放弃
							return;
						}
						//else
						//{
						//	GiveUpScore();//如果牌面上的牌大于玩家牌面上的牌有10%放弃
						//	return;
						//}
						
					}
					else
					{
						int type=m_GameLogic.GetCardGenre(m_cbHandCard,4);
						if(type>CT_SINGLE)
						{
							if (GaiLv(100))
							{
								FllowScore();//如果牌面上的牌大于玩家牌面上的牌有90%的跟注
								return;
							}
						}
						else
						{
							GiveUpScore();//如果牌面上的牌大于玩家牌面上的牌有10%放弃
							return;
						}
						//else
						//{
						//	GiveUpScore();//如果牌面上的牌大于玩家牌面上的牌有10%放弃
						//	return;
						//}
					}
				
				}
				else
				{
					if (WinIsAndroid(1,3))
					{
						int type=m_GameLogic.GetCardGenre(m_cbHandCard,4);
						if(type>CT_ONE_DOUBLE)
						{
							FllowScore();//如果牌面上的牌大于玩家牌面上的牌有90%的跟注
							return;
						}
						else
						{

							if (GaiLv(5))
							{
								FllowScore();//如果牌面上的牌大于玩家牌面上的牌有90%的跟注
								return;
							}
							else
							{
								GiveUpScore();//如果牌面上的牌大于玩家牌面上的牌有10%放弃
								return;
							}
						}
						
					}
					else
					{
						GiveUpScore();//如果牌面上的牌小于玩家牌面上的牌就放弃
						return;
					}
				}
			}
			else
			{
				if(m_bWillWin)
				{
						if (WinIsAndroid(1,3))
						{
							if (GaiLv(10)) 
							{
					//			AddScore();		
								SendSuoHand();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有80%梭哈
								return;
							}
							else
							{
								if (GaiLv(50))
								{
									AddScore();		//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-80）×70=14%加注
									return;
								}
								else
								{
									int type=m_GameLogic.GetCardGenre(m_cbHandCard,4);
									if(type>CT_ONE_DOUBLE)
										AddScore();
									else
										FllowScore();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-80）×30=6%跟注			
									return;
								}
							}
						}
						else
						{
							if (GaiLv(100))//如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有30%跟注
							{
								FllowScore();
						
								return;
							}
							//else
							//{
							//	GiveUpScore();//如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有70%放弃
							//	return;
							//}
						}
				}
				else
				{
						if (WinIsAndroid(1,3))
						{
							if (GaiLv(5)) 
							{
								AddScore();		
					
								return;
							}
							else
							{
								if (GaiLv(40))
								{
				
									AddScore();		//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-80）×70=14%加注
									return;
								}
								else
								{
									FllowScore();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-80）×30=6%跟注			
									return;
								}
							}
						}
						else
						{
							if (GaiLv(20))//如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有30%跟注
							{
								FllowScore();
						
								return;
							}
							else
							{
								GiveUpScore();//如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有70%放弃
								return;
							}
						}
				}
			}
				
		}
		
	case 5:
		{
			m_bTemp=true;
			if (m_bShowHand)
			{
				if(m_bWillWin)
				{
						if (WinIsAndroid(1,4))
						{
							int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
							if(type>CT_ONE_DOUBLE)
							{
								if (GaiLv(100))
								{
									FllowScore();	//如果是梭哈标准，如果机器人的牌面大于玩家牌面就有60%的概率跟注		
									return;
								}
							}
							else
							{
								GiveUpScore();	//如果是梭哈标准，如果机器人的牌面大于玩家牌面就有40%的概率放弃
								return;
							}
						}
						else
						{
							//if (GaiLv(50))
							//{
							//	GiveUpScore();//如果是梭哈标准，如果机器人的牌面小于玩家牌面就有90%的概率放弃
							//	return;	
							//}
							//else
							{
								int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
								if(type>CT_ONE_DOUBLE)
									FllowScore();//如果是梭哈标准，如果机器人的牌面小于玩家牌面就有10%的概率跟注
								else
									GiveUpScore();

								return;
							}
						}

				}
				else
				{
						if (WinIsAndroid(1,4))
						{
							if (GaiLv(5))
							{
								FllowScore();	//如果是梭哈标准，如果机器人的牌面大于玩家牌面就有60%的概率跟注		
								return;
							}
							else
							{
								GiveUpScore();	//如果是梭哈标准，如果机器人的牌面大于玩家牌面就有40%的概率放弃
								return;
							}
						}
						else
						{
							if (GaiLv(100))
							{
								GiveUpScore();//如果是梭哈标准，如果机器人的牌面小于玩家牌面就有90%的概率放弃
								return;	
							}
							//else
							//{
							//	FllowScore();//如果是梭哈标准，如果机器人的牌面小于玩家牌面就有10%的概率跟注
							//	return;
							//}
						}
				}

			}
			else
			{
				if(m_bWillWin)
				{
					if (WinIsAndroid(1,4))
						{
							if (GaiLv(30))
							{
								int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
								if(type>CT_ONE_DOUBLE)
									SendSuoHand();
								else
									AddScore();		//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有30%的概率加注		
								
								return;
							}
							else
							{
								int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
								if(type>CT_ONE_DOUBLE)
								{
									if (GaiLv(100))
									{
										SendSuoHand();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-30）×90=63%的概率梭哈			
										return;
									}
									else
									{
										FllowScore();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-30）×10=7%的概率跟注				
										return;
									}
								}
								else
								{
									FllowScore();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-30）×10=7%的概率跟注				
									return;
								}
							}
						}
					else
						{

							int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
							if(type>CT_ONE_DOUBLE)
							{
								if (GaiLv(100))
								{
									int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
									if(type>CT_ONE_DOUBLE)
										SendSuoHand();
									else
										FllowScore();	//如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有50%的概率跟注	

									return;
								}
							}
							else
							{
								GiveUpScore();	//如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有50%的概率放弃
								return;
							}
						}
				}
				else
				{
					if (WinIsAndroid(1,4))
					{
						int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
						if(type>=CT_ONE_DOUBLE)
						{
							if (GaiLv(30))
							{
								AddScore();		//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有30%的概率加注		
								return;
							}
							else
							{
								if (GaiLv(0))
								{
									SendSuoHand();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-30）×90=63%的概率梭哈			
									return;
								}
								else
								{
									FllowScore();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-30）×10=7%的概率跟注				
									return;
								}
							}
						}
						else
						{
							GiveUpScore();
						}
					}
					else
					{
						if (GaiLv(10))
						{
							FllowScore();	//如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有50%的概率跟注	
							return;
						}
						else
						{
							GiveUpScore();	//如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有50%的概率放弃
							return;
						}
					}
				}
			}
		}
		ASSERT(false);
		
	}
}
void CAndroidUserItemSink::GiveUpScore()
{

	

	if (m_bTemp)
	{
		m_bTemp=false;
		m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
		return;
	}
	else
	{
		if (m_wCurrentUser ==m_wSpeciaChairID)
		{	
			FllowScore();
			return;
		}
		m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
		return;
	}
}
void CAndroidUserItemSink::AddScore()
{
		
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		SCORE lScoreLess = m_lTurnLessScore - ( m_lTableScore[wMeChairID] + m_lUserScore[wMeChairID] );

		int iTem = 0;

		if(m_nCurCardCount>3)
			iTem = m_lBeforeUserScore+rand()%5+1;
		else
			iTem = m_lBeforeUserScore+rand()%3+1;

		SCORE lXXParam=0;

		SCORE lAddChip = iTem*m_lCellScore;
	//	if((LONGLONG)lScoreLess>=4*m_lCellScore)
	//		lXXParam = lScoreLess;
	//	else
		  	lXXParam= lAddChip + lScoreLess;

	//	SCORE lXXParam = iTem*m_lCellScore + (LONGLONG)lScoreLess;

		//金币计算
		SCORE lDrawAddScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];
		//LONGLONG lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);
	//	LONGLONG lCurrentAddScore=(m_lTurnLessScore-lDrawAddScore);

	//	if(lCurrentAddScore>m_lTurnMaxScore) lCurrentAddScore=m_lTurnMaxScore-lDrawAddScore;

		SCORE lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);
		
		if(lCurrentAddScore>m_lTurnMaxScore) lCurrentAddScore=m_lTurnMaxScore-lDrawAddScore;



		//筹码设置
		m_lUserScore[wMeChairID]+=lCurrentAddScore;



		
	



		//发送消息
		CMD_C_AddScore AddScore;
		
	//	lCurrentAddScore = lCurrentAddScore;
		AddScore.lScore=lCurrentAddScore;
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
		return;
}
void CAndroidUserItemSink::FllowScore()
{


//	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
//
//	SCORE lScoreLess = m_lTurnLessScore - ( m_lTableScore[wMeChairID] + m_lUserScore[wMeChairID] );
//	//LONGLONG lXXParam = lAddChip + (LONGLONG)lScoreLess;
//	//金币计算
//	SCORE lDrawAddScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];
//	//LONGLONG lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);
//	SCORE lCurrentAddScore=(m_lTurnLessScore-lDrawAddScore);
//
//	if(lCurrentAddScore>m_lTurnMaxScore) lCurrentAddScore=m_lTurnMaxScore-lDrawAddScore;
//
////	SCORE lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lScoreLess);
//
//	//筹码设置
//	m_lUserScore[wMeChairID]+=lCurrentAddScore;
//	//发送消息
//	CMD_C_AddScore AddScore;
//	AddScore.lScore=lCurrentAddScore;
//	m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
	//KillTimer()
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	SCORE lScoreLess = m_lTurnLessScore - ( m_lTableScore[wMeChairID] + m_lUserScore[wMeChairID] );

	SCORE lXXParam = (LONGLONG)lScoreLess;
	//金币计算
	SCORE lDrawAddScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];
	SCORE lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);
	//LONGLONG lCurrentAddScore=(m_lTurnLessScore-lDrawAddScore);
	//if(lCurrentAddScore>m_lTurnMaxScore) lCurrentAddScore=m_lTurnMaxScore-lDrawAddScore;
	//筹码设置
	m_lUserScore[wMeChairID]+=lCurrentAddScore;
	//发送消息
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentAddScore;
	m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));



	return;


}
bool CAndroidUserItemSink::GaiLv(BYTE bNum)
{
	if (rand()%100<bNum)
	{
		return true;
	}
	return false;
}
bool CAndroidUserItemSink::WinIsAndroid(BYTE cbStartPos, BYTE cbConcludePos)
{
	WORD iWinner = EstimateWinner(cbStartPos,cbConcludePos);
//	IServerUserItem *pIServerUserItem = m_pIAndroidUserItem->GetTableUserItem(iWinner);
	if(iWinner==m_pIAndroidUserItem->GetChairID())
		return true;
/*	if (pIServerUserItem->IsAndroidUser())
	{
		return true;
	}*/
	return false;
}
WORD CAndroidUserItemSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbTableCardData,sizeof(cbUserCardData));

	//寻找玩家
	WORD wWinner=0;
	for ( wWinner=0;wWinner<GAME_PLAYER;wWinner++)
	{
		if (m_cbOtherCard[wWinner].bStatue==true) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//对比玩家
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//用户过滤
		if (m_cbOtherCard[i].bStatue == FALSE) continue;
		
		
	//	IServerUserItem *pIServerUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
		
	//	if(pIServerUserItem==NULL) continue;

	//	if (pIServerUserItem->IsAndroidUser() && i != m_wCurrentUser) continue;

		//排列扑克
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//对比扑克
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=i;
		}
	}
	
	return wWinner;

}
LONGLONG CAndroidUserItemSink::GetAddChip()
{

	//标志变量
	bool bEnableShowHand=(m_lDrawMaxScore==m_lTurnMaxScore);
	bool bFollow = m_lTurnMaxScore>=m_lTurnLessScore;
	bool bAddChip = m_lTurnMaxScore>=m_lTurnLessScore;
	bool bShowHand = ((bEnableShowHand) && (m_lTurnMaxScore<=m_lTurnLessScore+m_lTurnLessScore));
	WORD wWinner = m_pIAndroidUserItem->GetChairID();
	BYTE bOtherKind=0;

	//获取当前牌局最大家
	//寻找玩家
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(i == m_pIAndroidUserItem->GetChairID()) continue;
		if(m_cbOtherCard[i].bStatue==false||(m_cbOtherCard[i].bStatue==true&&m_cbOtherCard[i].bGiveUp==true)) continue;

		//排列扑克
		m_GameLogic.SortCardList(m_cbOtherCard[i].cbCard+1,m_nCurCardCount-1);

		//对比扑克
		if (m_GameLogic.CompareCard(m_cbOtherCard[i].cbCard+1,m_cbOtherCard[i].cbCard+1,m_nCurCardCount-1)==true || wWinner==m_pIAndroidUserItem->GetChairID()) 
		{
			wWinner=i;
		}
	}

	if (wWinner==m_pIAndroidUserItem->GetChairID())
	{
		return 0;
	}

	bOtherKind = m_GameLogic.GetCardGenre(m_cbOtherCard[wWinner].cbCard+1,m_nCurCardCount-1);
	m_GameLogic.SortCardList(m_cbHandCard+1,m_nCurCardCount-1);
	BYTE bCardKind = m_GameLogic.GetCardGenre(m_cbHandCard+1,m_nCurCardCount-1);

	bool bMaxCmp1 = m_GameLogic.CompareCard(m_cbHandCard+1,m_cbOtherCard[wWinner].cbCard+1,m_nCurCardCount-1);
	bool bMaxCmp0 = m_GameLogic.CompareCard(m_cbHandCard+1,m_cbOtherCard[wWinner].cbCard,m_nCurCardCount);

	int r = ((rand()<<1)+(rand()>>1)+time(NULL)) & 127;
	if (r>40 && (bShowHand || m_nCurCardCount>3) && (bMaxCmp0 || bMaxCmp1 || bOtherKind==CT_SINGLE || bCardKind>=bOtherKind) && m_bWillWin)
	{
//		return SendSuoHand();
	}else if (!bShowHand)
	{
		if (bMaxCmp0||bMaxCmp1||(m_bWillWin&&(bOtherKind==CT_SINGLE || bCardKind>=bOtherKind)))
		{
			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			LONGLONG lCurrentAddScore=m_lDrawMaxScore-m_lTableScore[wMeChairID]-m_lUserScore[wMeChairID];
			
			
		return __min(m_lCellScore*(rand()%(bAddChip?3:1)),lCurrentAddScore);//加注


		}else if (m_nCurCardCount<4)
		{
			return 0;
		}
	}
	else if(m_nCurCardCount<4)
	{
		/*if (r>40)*/ return 0;
	}
	//m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
	return 0; //放弃
}

void CAndroidUserItemSink::SendSuoHand()
{
	
	
		//金币计算
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		SCORE lCurrentAddScore=m_lDrawMaxScore-m_lTableScore[wMeChairID]-m_lUserScore[wMeChairID];



		//筹码设置
		m_lUserScore[wMeChairID]+=lCurrentAddScore;

		//发送消息
		CMD_C_AddScore AddScore;
		AddScore.lScore=lCurrentAddScore;
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
		return;
	
	
}
//////////////////////////////////////////////////////////////////////////
//读取配置
void CAndroidUserItemSink::ReadConfigInformation()
{
	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//读取配置
	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\HKFiveCardConfig.ini"),szPath);

	//分数限制
	m_lRobotScoreRange[0] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMin"), 50, szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMax"), 1000, szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//提款数额
	m_lRobotBankGetScore = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGet"), 100, szConfigFileName);

	//提款数额 
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGetBanker"), 500, szConfigFileName);

	//存款倍数
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 50, szConfigFileName);



}