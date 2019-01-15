#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//常量定义
#define IDI_FREE					1									//空闲计时器
#define IDI_PLACE_JETTON			2									//下注计时器
#define IDI_GAME_END				3									//结束计时器
//////////////////////////////////////////////////////////////////////////
//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式
//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink() : m_iMaxAreaScore(2100000000)
{
//AllocConsole();
//freopen("CONOUT$","w+t",stdout);
//freopen("CONIN$","r+t",stdin);
//system("cls");
	//总下注数
	ZeroMemory(m_lTotalJettonScore, sizeof(m_lTotalJettonScore));
	//各区域可下分
	ZeroMemory(m_lAreaScoreLimit, sizeof(m_lAreaScoreLimit));
	//控制变量
	m_cbWinArea = -1;
	m_dwGameTimes = 0;
	//个人下注
	ZeroMemory(m_lUserAreaScore, sizeof(m_lUserAreaScore));
	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserJetton,sizeof(m_lUserJetton));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(m_cbPaoHuoCheArea,sizeof(m_cbPaoHuoCheArea));
	ZeroMemory(m_lCurrentJetton,sizeof(m_lCurrentJetton));
	ZeroMemory(m_GameRecordMultiple,sizeof(m_GameRecordMultiple));

	m_CtrlAccountArray.RemoveAll();

	//状态变量
	m_dwJettonTime=0L;
	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_nHasWinTimes = 0;
	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;

	//时间控制
	m_cbFreeTime = 10;
	m_cbBetTime = 20;
	m_cbEndTime = 20;
	m_cbBigSmall=0;
	m_cbGoodLuckType=0;
	m_cbPaoHuoCheCount=0;

	//库存值
	m_StorageStart=0;
	m_StorageDeduct=5;

	m_dwJettonRate=100;
	m_cbGameRecordMultipleCount=0;

	m_cbControlArea=0xFF;
	m_cbControlTimes=0;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
	m_ApplyUserArray.RemoveAll();
	m_CtrlAccountArray.RemoveAll();
}

//接口查询
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	ReadConfigInformation(true);

	return true;
}

//复位桌子
void CTableFrameSink::RepositionSink()
{
	//总下注数
	ZeroMemory(m_lTotalJettonScore, sizeof(m_lTotalJettonScore));
	//各区域可下分
	ZeroMemory(m_lAreaScoreLimit, sizeof(m_lAreaScoreLimit));
	//个人下注
	ZeroMemory(m_lUserAreaScore, sizeof(m_lUserAreaScore));
	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserJetton,sizeof(m_lUserJetton));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(m_cbPaoHuoCheArea,sizeof(m_cbPaoHuoCheArea));
	m_cbGoodLuckType=0;
	m_cbPaoHuoCheCount=0;
	m_dwJettonRate=100;

	return;
}

//开始模式
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//游戏状态
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	m_dwGameTimes++;

	//获取庄家
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
		pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

	//设置变量
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore();

	//每局游戏开始时刻，计算各区域初始可下分值
	CalculateAreaInitLimite();

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		GameStart.lAreaLimitScore[i] = m_lAreaScoreLimit[i];

    //发送给旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//发送给游戏玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;

		//设置积分
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//每局游戏开始时刻，计算各区域初始可下分值
bool __cdecl CTableFrameSink::CalculateAreaInitLimite()
{
	//初始化变量
	__int64 lBankerScore = 0;
	IServerUserItem * pIBankerServerUserItem = NULL;

	//有人做庄，获得庄家信息
	if( INVALID_CHAIR != m_wCurrentBanker )
	{
		pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if( pIBankerServerUserItem == NULL )
			return FALSE;

#ifndef FUNC_CHIP
		//获得庄家当前的积分(金币)
		lBankerScore=pIBankerServerUserItem->GetUserScore();
#else
		lBankerScore=m_lCurrentJetton[pIBankerServerUserItem->GetChairID()];
#endif

		//根据庄家金币数量，计算各区域的初始化可下分
		//for( int i=0; i<JETTON_AREA_COUNT; i++ )
		//	m_lAreaScoreLimit[i] = __min(lBankerScore/(m_iOddsArray[i]+1), m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_APPLE]			=__min(lBankerScore/23,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_ORANGE]		=__min(lBankerScore/31,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_PAWPAW]		=__min(lBankerScore/31,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_BELL]			=__min(lBankerScore/31,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_WATERMELON]	=__min(lBankerScore/61,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_STAR]			=__min(lBankerScore/61,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_DOUBLE_SEVEN]	=__min(lBankerScore/61,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_BAR]			=__min(lBankerScore/76,m_iMaxAreaScore);
	}
	else //如果无人做庄
	{
		if( m_bEnableSysBanker ) //如果允许系统做庄
		{
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
			{
				//m_lAreaScoreLimit[i] = __min( m_lSysLimitScore/(m_iOddsArray[i]/m_iOddsArray[i+1]), m_iMaxAreaScore);
				//m_lAreaScoreLimit[i] = m_lSysLimitScore;	//如果是低倍率区域，则该区域的限注为配置文件指定的限额
				m_lAreaScoreLimit[i]=999*10000;
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

//	发送上庄列表
bool CTableFrameSink::SendApplyBankerList(IServerUserItem *pIServerUserItem)
{
	CMD_S_ApplyBankerListMax sList;
	int nCount = m_ApplyUserArray.GetCount();
	int j=0;
	for (int i = 0; i < nCount; i++)
	{
		WORD wChairID = m_ApplyUserArray.GetAt(i);
		IServerUserItem *pApplyUser = m_pITableFrame->GetTableUserItem(wChairID);
		if (m_wCurrentBanker == wChairID||pApplyUser==NULL)
		{
			continue;
		}
		sList.wApplyList[j].wChairID = wChairID;
		sList.wApplyList[j].dwUserID = pApplyUser->GetUserID();
		sList.wApplyList[j].lScore = m_lCurrentJetton[wChairID];
		sList.wApplyList[j].cbGender = pApplyUser->GetGender();
		wcscpy(sList.wApplyList[j].szNickName,pApplyUser->GetNickName());
		j++;
	}
	sList.cbCount=j;
	m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_C_GET_PLAYLIST, &sList,sList.getLength());

	return true;
}


//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//派发扑克
			do
			{
				DispatchTableCard();
			} while (!ProbeJudge());
			//特殊账号控制
			DispatchTableCardControl();
//printf("m_cbBigSmall=%d\n",m_cbBigSmall);
//printf("m_cbPaoHuoCheCount=%d\n",m_cbPaoHuoCheCount);
//for (int ii=0; ii<m_cbPaoHuoCheCount; ii++)
//{
//	printf("m_cbPaoHuoCheArea[ii]=%d\n",m_cbPaoHuoCheArea[ii]);
//}

			//计算分数
			__int64 iBankerWinScore=CalculateScore();

			//递增次数
			m_wBankerTime++;

			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//庄家信息
			GameEnd.iBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=iBankerWinScore;
			GameEnd.lApplyBankerCondition=m_lApplyBankerCondition;

			GameEnd.cbWinArea=m_cbWinArea;
			GameEnd.cbGoodLuckType=m_cbGoodLuckType;
			GameEnd.cbPaoHuoCheCount=m_cbPaoHuoCheCount;
			CopyMemory(GameEnd.cbPaoHuoCheArea,m_cbPaoHuoCheArea,sizeof(GameEnd.cbPaoHuoCheArea));

			//发送时间
			m_cbEndTime=20;
			if (m_cbGoodLuckType==0)
			{
				if (m_cbEndTime<20) m_cbEndTime=20;
			}
			else if (m_cbGoodLuckType>=1 && m_cbGoodLuckType<=3)
			{
				if (m_cbEndTime<27+5) m_cbEndTime=27+5;
			}
			else
			{
				if (m_cbEndTime<36) m_cbEndTime=36;
			}
			GameEnd.cbTimeLeave=m_cbEndTime;

			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) 
					continue;

				//设置成绩
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//返还积分
				GameEnd.lUserReturnScore=m_lUserJetton[wUserIndex];

				//设置税收
				if (m_lUserRevenue[wUserIndex]>0) 
					GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) 
					GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else 
					GameEnd.lRevenue=0;

				//发送消息					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEAVE:		//用户离开
		{
			//闲家离开
			if (m_wCurrentBanker!=wChairID)
			{
				if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
				{
//printf("下注时间离开\n");
					//清理数据
					for (int i=0; i<JETTON_AREA_COUNT; i++) m_lUserAreaScore[wChairID][i]=0;
					m_lCurrentJetton[wChairID]=0;
					m_lUserWinScore[wChairID]=0;
					m_lUserJetton[wChairID]=0;
				}
				else
				{
//printf("出结果时间离开\n");
					////////////////////////////////////////////////////////////////////////////////
					//修正库存值
					//if (!pIServerUserItem->IsAndroidUser())
					//	m_StorageStart-=(m_lUserWinScore[wChairID]+m_lUserJetton[wChairID]);
					////////////////////////////////////////////////////////////////////////////////
					//计算税收
					if (0<(m_lUserWinScore[wChairID]+m_lUserJetton[wChairID]))
					{
						m_lUserRevenue[wChairID]=((m_lUserWinScore[wChairID]+m_lUserJetton[wChairID])/1000)*m_pGameServiceOption->cbRevenueRatio;
						m_lUserJetton[wChairID]-=m_lUserRevenue[wChairID];
					}
					////////////////////////////////////////////////////////////////////////////////

					//////////
					tagScoreInfo ScoreInfoArray[GAME_PLAYER];
					memset(ScoreInfoArray,0,sizeof(ScoreInfoArray));
					ScoreInfoArray[wChairID].lScore=m_lUserWinScore[wChairID]+m_lUserJetton[wChairID];
					ScoreInfoArray[wChairID].cbType=(m_lUserWinScore[wChairID]+m_lUserJetton[wChairID]>0)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					//////////

					//写入积分
					if ((m_lUserWinScore[wChairID]+m_lUserJetton[wChairID])!=0L) 
						m_pITableFrame->WriteTableScore(ScoreInfoArray,GAME_PLAYER);
//printf("得分：%I64d\n",m_lUserWinScore[wChairID]+m_lUserJetton[wChairID]);
					//清理数据
					for (int i=0; i<JETTON_AREA_COUNT; i++) m_lUserAreaScore[wChairID][i]=0;
					m_lCurrentJetton[wChairID]=0;
					m_lUserWinScore[wChairID]=0;
					m_lUserJetton[wChairID]=0;
				}

				return true;
			}

			//庄家离开，状态判断
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//提示消息
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于庄家[ %s ]强退，游戏提前结束！"),pIServerUserItem->GetNickName());

				//发送消息
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//设置时间
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//派发扑克
				do
				{
					DispatchTableCard();
				} while (!ProbeJudge());
				//特殊账号控制
				DispatchTableCardControl();

				//计算分数
				CalculateScore();

				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//庄家信息
				GameEnd.iBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				GameEnd.lApplyBankerCondition=m_lApplyBankerCondition;
				if (m_lBankerWinScore>0) 
					GameEnd.lBankerScore=0;

				//发送时间
				m_cbEndTime=20;
				if (m_cbGoodLuckType==0)
				{
					if (m_cbEndTime<20) m_cbEndTime=20;
				}
				else if (m_cbGoodLuckType>=1 && m_cbGoodLuckType<=3)
				{
					if (m_cbEndTime<27+5) m_cbEndTime=27+5;
				}
				else
				{
					if (m_cbEndTime<36) m_cbEndTime=36;
				}
				GameEnd.cbTimeLeave=m_cbEndTime;

				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) 
						continue;

					//设置成绩
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//返还积分
					GameEnd.lUserReturnScore=m_lUserJetton[wUserIndex];

					//设置税收
					if (m_lUserRevenue[wUserIndex]>0) 
						GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) 
						GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else 
						GameEnd.lRevenue=0;

					//发送消息					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//扣除分数
			if ((m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker])!=0)
			{
				////////////////////////////////////////////////////////////////////////////////
				//修正库存值
				//if (!pIServerUserItem->IsAndroidUser())
				//	m_StorageStart-=(m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker]);
				////////////////////////////////////////////////////////////////////////////////
				//计算税收
				if (0<(m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker]))
				{
					m_lUserRevenue[m_wCurrentBanker]=((m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker])/1000)*m_pGameServiceOption->cbRevenueRatio;
					m_lUserJetton[m_wCurrentBanker]-=m_lUserRevenue[m_wCurrentBanker];
				}
				////////////////////////////////////////////////////////////////////////////////

				//////////
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				memset(ScoreInfoArray,0,sizeof(ScoreInfoArray));
				ScoreInfoArray[m_wCurrentBanker].lScore=m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker];
				ScoreInfoArray[m_wCurrentBanker].cbType=(m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker]>0)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				//////////

				//写入积分
				if ((m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker])!=0L) 
					m_pITableFrame->WriteTableScore(ScoreInfoArray,GAME_PLAYER);
//printf("庄家强退，得分：%I64d\n",m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker]);
				//清理数据
				for (int i=0; i<JETTON_AREA_COUNT; i++) m_lUserAreaScore[m_wCurrentBanker][i]=0;
				m_lCurrentJetton[m_wCurrentBanker]=0;
				m_lUserWinScore[m_wCurrentBanker]=0;
				m_lUserJetton[m_wCurrentBanker]=0;
			}

			//切换庄家
			ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
//printf("wChiarID=%d\n",wChiarID);
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:			//空闲状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));	

			//重入的玩家，筹码数量清0
			//m_lCurrentJetton[wChiarID]=0;
			//机器人分数全部自动兑换成筹码
			if (pIServerUserItem->IsAndroidUser()) m_lCurrentJetton[wChiarID]=pIServerUserItem->GetUserInfo()->lScore;

			CopyMemory(StatusFree.lCurrentJetton,m_lCurrentJetton,sizeof(StatusFree.lCurrentJetton));
			StatusFree.dwChipRate=m_dwJettonRate;

			//控制信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				StatusFree.lAreaLimitScore[i] = m_lAreaScoreLimit[i];

			//庄家信息
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.wBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore();
				wcscpy(StatusFree.szBankerNickName,pIServerUserItem->GetNickName());
			}

			//玩家信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore); 
			}

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			memcpy(StatusFree.szRoomName,m_szGameRoomName,sizeof(StatusFree.szRoomName));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d, 玩家限制为：%I64d"),m_lApplyBankerCondition,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_PROMPT);
						
			//发送申请者
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//游戏状态
	case GS_GAME_END:			//结束状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);		

			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//重入的玩家，筹码数量清0
			//m_lCurrentJetton[wChiarID]=0;
			//机器人分数全部自动兑换成筹码
			if (pIServerUserItem->IsAndroidUser()) m_lCurrentJetton[wChiarID]=pIServerUserItem->GetUserInfo()->lScore;

			CopyMemory(StatusPlay.lCurrentJetton,m_lCurrentJetton,sizeof(StatusPlay.lCurrentJetton));
			StatusPlay.dwChipRate=m_dwJettonRate;

			//全局下注
			for (int i=0; i<JETTON_AREA_COUNT; i++ )
			{
				StatusPlay.lTotalJettonScore[i] = m_lTotalJettonScore[i];
				StatusPlay.lAreaScoreLimit[i] = m_lAreaScoreLimit[i];
			}

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for(int i=0; i<JETTON_AREA_COUNT; i++)
					StatusPlay.lUserAreaScore[i] = m_lUserAreaScore[wChiarID][i];

				//最大下注
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
			}

			//控制信息
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;

			//庄家信息
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	

			StatusPlay.cbWinArea=m_cbWinArea;
			StatusPlay.cbGoodLuckType=m_cbGoodLuckType;
			StatusPlay.cbPaoHuoCheCount=m_cbPaoHuoCheCount;
			CopyMemory(StatusPlay.cbPaoHuoCheArea,m_cbPaoHuoCheArea,sizeof(StatusPlay.cbPaoHuoCheArea));

			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIServerUserItem->GetUserScore();
				wcscpy(StatusPlay.szBankerNickName,pIServerUserItem->GetNickName());
			}	

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(m_cbBetTime-__min(dwPassTime,m_cbBetTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();

			//结束判断
			if (cbGameStatus==GS_GAME_END)
			{
				StatusPlay.cbTimeLeave=(BYTE)(m_cbEndTime-__min(dwPassTime,m_cbEndTime));
				//设置成绩
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//返还积分
				StatusPlay.lEndUserReturnScore=m_lUserJetton[wChiarID];

				//设置税收
				if (m_lUserRevenue[wChiarID]>0) 
					StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) 
					StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else 
					StatusPlay.lEndRevenue=0;

				//庄家成绩
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;
			}

			//发送申请者
			SendApplyUser( pIServerUserItem );

			memcpy(StatusPlay.szRoomName,m_szGameRoomName,sizeof(StatusPlay.szRoomName));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_FREE:		//空闲时间结束
		{
			//开始游戏
			m_pITableFrame->StartGame();

			//启动下注计时器
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//设置游状态为下注阶段
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间结束
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//结束游戏
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);
			}

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) 
					continue;

				////////////////////////////////////////////////////////////////////////////////
				//修正库存值
				//if (!pIServerUserItem->IsAndroidUser())
				//	m_StorageStart-=(m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID]);
				//else
				//	printf("修正库存值时，有机器人\n");
				////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////
				//计算税收
				if (0<(m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID]))
				{
					m_lUserRevenue[wUserChairID]=((m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID])/1000)*m_pGameServiceOption->cbRevenueRatio;
					m_lUserJetton[wUserChairID]-=m_lUserRevenue[wUserChairID];
//printf("抽水：m_lUserRevenue[wUserChairID]=%I64d\n",m_lUserRevenue[wUserChairID]);
				}
				////////////////////////////////////////////////////////////////////////////////

				//////////
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				memset(ScoreInfoArray,0,sizeof(ScoreInfoArray));
				ScoreInfoArray[wUserChairID].lScore=m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID];
				ScoreInfoArray[wUserChairID].cbType=(m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID]>0)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				//////////

				//写入积分
				if ((m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID])!=0L) 
					m_pITableFrame->WriteTableScore(ScoreInfoArray,GAME_PLAYER);
				////////////////////////////////////////////////////////////////////////////////
				//更新当前筹码
				m_lCurrentJetton[wUserChairID]+=m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID];
				CMD_S_ChipUpdate ChipUpdate;
				ZeroMemory(&ChipUpdate,sizeof(ChipUpdate));
				ChipUpdate.wChairID=wUserChairID;
				ChipUpdate.lTotalScore=m_lCurrentJetton[wUserChairID];
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHIP_UPDATE,&ChipUpdate,sizeof(ChipUpdate));//游戏结束定时器时的更新
				////////////////////////////////////////////////////////////////////////////////

				//坐庄条件判断。一轮游戏结束，玩家有输有赢。所以需要在申请上庄队列里把输了钱不在满足做庄的玩家清除掉
				__int64 lCurrentJetton=m_lCurrentJetton[wUserChairID];
				if (wUserChairID!=m_wCurrentBanker && lCurrentJetton<m_lApplyBankerCondition)
				{
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_ApplyUserArray[i]) continue;

						//从庄家队列删除不符合上庄的玩家
						m_ApplyUserArray.RemoveAt(i);

						//发送消息
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						//设置变量
						lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));
						CancelBanker.wChairID = pIServerUserItem->GetChairID();
						//发送消息
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//提示消息
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数少于坐庄必须金币数（%I64d）,你自动下庄！"),m_lApplyBankerCondition);
						SendGameMessage(wUserChairID,szTipMsg);

						break;
					}
				}
			}
			////////////////////////////////////////////////////////////////////////////////
			//机器人自动存取分
			RobotAddMoney(m_lRobotMoneyLine,m_lRobotAddScore,m_lRoboMaxMoney,m_nSetlower,m_bAddMoney);
			////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////
			//修正库存值
			//m_StorageStart=m_StorageStart-(m_StorageStart*m_StorageDeduct/1000);
			////////////////////////////////////////////////////////////////////////////////
//printf("m_StorageStart=%I64d\n",m_StorageStart/100);
//printf("m_StorageDeduct=%I64d\n",m_StorageDeduct);
			//结束游戏
			m_pITableFrame->ConcludeGame(GER_NORMAL,NULL,608);//注意参数

			//切换庄家
			ChangeBanker(false);

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//发送消息
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_cbFreeTime;
			GameFree.cbGameRecordCount=1;
			GameFree.cbGameRecord[0].cbRecord=m_cbWinArea;
			GameFree.cbGameRecord[0].dwGameTimes=m_dwGameTimes;
			GameFree.cbGameRecord[0].bGoodLuck=false;
			//////////
			//if ((m_cbWinArea==9 || m_cbWinArea==21) && m_cbGoodLuckType!=0)
			if (m_cbGameRecordMultipleCount>0)
			{
				//for (int i=0; i<m_cbPaoHuoCheCount; i++)
				for (int i=0; i<m_cbGameRecordMultipleCount; i++)
				{
					GameFree.cbGameRecordCount++;
					GameFree.cbGameRecord[i+1].cbRecord=m_GameRecordMultiple[i].cbRecord;
					GameFree.cbGameRecord[i+1].dwGameTimes=m_GameRecordMultiple[i].dwGameTimes;
					GameFree.cbGameRecord[i+1].bGoodLuck=m_GameRecordMultiple[i].bGoodLuck;
//printf("GameFree.cbGameRecord[i+1].cbRecord=%d\n",GameFree.cbGameRecord[i+1].cbRecord);
//printf("GameFree.cbGameRecord[i+1].dwGameTimes=%d\n",GameFree.cbGameRecord[i+1].dwGameTimes);
					//////////
					tagServerGameRecord &GameRecord=m_GameRecordArrary[m_nRecordLast];
					GameRecord.cbRecord=m_GameRecordMultiple[i].cbRecord;
					GameRecord.dwGameTimes=m_GameRecordMultiple[i].dwGameTimes;
					GameRecord.bGoodLuck=m_GameRecordMultiple[i].bGoodLuck;
					//移动下标
					m_nRecordLast=(m_nRecordLast+1)%MAX_SCORE_HISTORY;
					if (m_nRecordLast==m_nRecordFirst) m_nRecordFirst=(m_nRecordFirst+1)%MAX_SCORE_HISTORY;
					//////////
				}
				GameFree.cbGameRecord[0].dwGameTimes=GameFree.cbGameRecord[1].dwGameTimes-1;
				m_cbGameRecordMultipleCount=0;
//printf("GameFree.cbGameRecordCount=%d\n",GameFree.cbGameRecordCount);
			}
			//////////
			//////////
			DWORD AndroidCount=0;
			for (int i=0; i<m_ApplyUserArray.GetCount(); i++)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
				if (pIServerUserItem==NULL) continue;
				if (!pIServerUserItem->IsAndroidUser()) continue;
				AndroidCount++;
			}
			GameFree.wApplyUserCount=AndroidCount;
			//////////
			GameFree.lApplyBankerCondition=m_lApplyBankerCondition;
			GameFree.lBankerTotallScore=m_lBankerWinScore;
			GameFree.iBankerTime=m_wBankerTime;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			ReadConfigInformation(false);

			return true;
		}
	}

	return false;
}

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_EXCHANGE:
		{
			CMD_C_Exchange * pExchange=(CMD_C_Exchange *)pData;
			if (pExchange->lExchangeScore>0)// && m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE)//空闲时间才能兑换
			{
				tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
				if (pUserData==NULL) return false;
				if (pUserData->lScore>=(m_lCurrentJetton[pUserData->wChairID]+pExchange->lExchangeScore))
				{
					CMD_S_ChipUpdate ChipUpdate;
					ZeroMemory(&ChipUpdate,sizeof(ChipUpdate));
					ChipUpdate.wChairID=pUserData->wChairID;
					ChipUpdate.lTotalScore=m_lCurrentJetton[pUserData->wChairID]+pExchange->lExchangeScore;
					m_lCurrentJetton[ChipUpdate.wChairID]=ChipUpdate.lTotalScore;//保存
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHIP_UPDATE,&ChipUpdate,sizeof(ChipUpdate));//玩家兑换时的更新
				}
			}
			return true;
		}
	case SUB_C_PLACE_JETTON:		//用户加注
		{
			//CTime t1(2012,10,30,0,0,0);
			//time_t ct;  
			//time( &ct) ;/// 当前日期 
			//CTime t2(ct);
			//if(t2>t1)
			//{
			//	return false;
			//}
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) 
				return false;

			if( pIServerUserItem == NULL )
				return true;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if( pUserData == NULL )
				return true;

			if (pUserData->cbUserStatus!=US_PLAYING) 
				return true;

			//消息处理
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pData;
			if( pPlaceJetton == NULL )
				return FALSE;

			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//申请做庄
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if( pUserData == NULL )
				return FALSE;

			if (pUserData->cbUserStatus==US_LOOKON)
				return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if( pUserData == NULL )
				return FALSE;
			if (pUserData->cbUserStatus==US_LOOKON)
				return true;

			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_BIG_SMALL:
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData==NULL) return false;
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			if (pUserData->wChairID==m_wCurrentBanker) return true;//庄家不能比大小

			CMD_C_BigSmall * pBigSmall=(CMD_C_BigSmall *)pData;

			CMD_S_BigSmall BigSmall;
			memset(&BigSmall,0,sizeof(CMD_S_BigSmall));

			if (m_lUserWinScore[pUserData->wChairID]<=0) return true;

			do
			{
				m_cbBigSmall=rand()%14+1;
			} while (!ProbeJudge2(pBigSmall->cbBigSmall,pUserData->wChairID));

			BYTE cbWin=2;
			if (m_cbBigSmall>0 && m_cbBigSmall<8) cbWin=0;
			if (m_cbBigSmall>7 && m_cbBigSmall<15) cbWin=1;
			ASSERT(cbWin!=2);

			__int64 lBankerWinScore=0;

//printf("m_lUserJetton[pUserData->wChairID]=%I64d\n",m_lUserJetton[pUserData->wChairID]);
//printf("m_lUserWinScore[pUserData->wChairID]=%I64d\n",m_lUserWinScore[pUserData->wChairID]);
			if (pBigSmall->cbBigSmall==cbWin)//猜对大小
			{
				//if (m_wCurrentBanker!=INVALID_CHAIR) m_lUserWinScore[m_wCurrentBanker]-=m_lUserWinScore[pUserData->wChairID];//庄家的正分减少闲家成绩的1倍
				lBankerWinScore-=m_lUserWinScore[pUserData->wChairID];//庄家的正分减少闲家成绩的1倍
				m_lUserWinScore[pUserData->wChairID]+=m_lUserWinScore[pUserData->wChairID];//闲家的成绩增加1倍
				BigSmall.bWin=true;
			}
			else
			{
				//if (m_wCurrentBanker!=INVALID_CHAIR) m_lUserWinScore[m_wCurrentBanker]+=m_lUserWinScore[pUserData->wChairID];//庄家的正分增加闲家的成绩
				lBankerWinScore+=m_lUserWinScore[pUserData->wChairID];//庄家的正分增加闲家的成绩
				//m_lUserJetton[pUserData->wChairID]-=m_lUserWinScore[pUserData->wChairID];
				m_lUserWinScore[pUserData->wChairID]=0;//闲家的成绩减少为0
				BigSmall.bWin=false;
			}
			m_lBankerWinScore+=lBankerWinScore;
			//////////
			if (m_wCurrentBanker!=INVALID_CHAIR)
				m_lUserWinScore[m_wCurrentBanker]+=lBankerWinScore;
			//////////
//printf("m_lUserJetton[pUserData->wChairID]=%I64d\n",m_lUserJetton[pUserData->wChairID]);
//printf("m_lUserWinScore[pUserData->wChairID]=%I64d\n",m_lUserWinScore[pUserData->wChairID]);
			BigSmall.cbBigSmall=m_cbBigSmall;
			BigSmall.lUserScore=m_lUserWinScore[pUserData->wChairID];
			BigSmall.lBankerScore=0;
			m_pITableFrame->SendTableData(pUserData->wChairID,SUB_S_BIG_SMALL,&BigSmall,sizeof(BigSmall));

			return true;
		}
	case SUB_C_ADMIN_COMMDN:
		{
			ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
			if(wDataSize!=sizeof(CMD_C_ControlApplication)) return false;

			if (pIServerUserItem->GetUserRight()!=1879048192)
				return false;

			//消息处理
			CMD_C_ControlApplication * pControlApplication=(CMD_C_ControlApplication *)pData;
			return OnAdminControl(pControlApplication,pIServerUserItem);
		}
	case SUB_C_GET_PLAYLIST:
		{
			return SendApplyBankerList(pIServerUserItem);
		}

	}

	//return false;
	return true;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//设置时间
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
	}
	//限制提示
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("此房间上庄条件为：%I64d, 玩家限制为：%I64d"),m_lApplyBankerCondition,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_PROMPT);
	return true;
}

//用户起来
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser==false)
	{
		//切换庄家
		if (wChairID==m_wCurrentBanker) ChangeBanker(true);

		//取消申请
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) 
				continue;
			//删除玩家
			m_ApplyUserArray.RemoveAt(i);
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));
			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));
			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			break;
		}
		m_lCurrentJetton[wChairID]=0;
		m_lUserWinScore[wChairID]=0;
		m_lUserJetton[wChairID]=0;

		return true;
	}
	return true;
}

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<=ID_APPLE)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_APPLE)||(lJettonScore<=0L)) 
		return true;//return false;

	//效验状态
	//ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) 
		return true;

	//庄家判断，如果是庄家，则不应该下注
	if (m_wCurrentBanker==wChairID) 
		return true;

	//如果当前不允许系统做庄，且没有庄家，则返回
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
		return true;

	if( cbJettonArea > ID_APPLE || cbJettonArea < ID_BAR )
		return true;

	//成功标识
	bool bPlaceJettonSuccess=true;

	//计算该玩家在各个区域所下注数额的总和
	__int64 lJettonCount = 0;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		lJettonCount += m_lUserAreaScore[wChairID][i];

#ifndef FUNC_CHIP
	//取得玩家资料
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if( pIServerUserItem == NULL )
		return false;
	//该玩家当前总共具有多少积分（金币）
	__int64 lUserScore = pIServerUserItem->GetUserScore();
	//合法校验，如果已下注额和即将下注数额相加大于玩家具备的积分，则返回
	if (lUserScore < lJettonCount + lJettonScore) 
		//return true;
		bPlaceJettonSuccess=false;
#else
	if (m_lCurrentJetton[wChairID]<lJettonCount+lJettonScore)
		//return true;
		bPlaceJettonSuccess=false;
#endif

	//如果已下注额和即将下注数额相加大于玩家的限注，则返回
	if (m_lUserLimitScore < lJettonCount + lJettonScore) 
		//return true;
		bPlaceJettonSuccess=false;
//printf("[%d]m_lUserLimitScore=%I64d\n",a,m_lUserLimitScore);

	//如果某区域已下总注加上即将下注数额小于等于该区域当前的下注上限，则进行操作，否则返回
	//if (m_lTotalJettonScore[cbJettonArea]+lJettonScore<=m_lAreaScoreLimit[cbJettonArea])//错误的
	if (bPlaceJettonSuccess && lJettonScore<=m_lAreaScoreLimit[cbJettonArea])
	{
		m_lTotalJettonScore[cbJettonArea] += lJettonScore;
		m_lUserAreaScore[wChairID][cbJettonArea] += lJettonScore;
		m_lAreaScoreLimit[cbJettonArea] -= lJettonScore;
		//bPlaceJettonSuccess = true;
//#ifdef FUNC_CHIP
//		m_lCurrentJetton[wChairID]-=lJettonScore;
//#endif
	}
	else
	{
		bPlaceJettonSuccess=false;
	}
	//else
	//{
	//	return true;
	//}
//printf("lJettonScore=%I64d\n",lJettonScore);
//printf("m_lTotalJettonScore[cbJettonArea]=%I64d\n",m_lTotalJettonScore[cbJettonArea]);
//printf("m_lAreaScoreLimit[cbJettonArea]=%I64d\n",m_lAreaScoreLimit[cbJettonArea]);
//printf("bPlaceJettonSuccess=%d\n",bPlaceJettonSuccess);
	//加注成功
	if (bPlaceJettonSuccess)
	{
		//变量定义
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//构造变量
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL)
			PlaceJetton.cbAndroid=pIServerUserItem->IsAndroidUser();
		CopyMemory(PlaceJetton.lTotalJettonScore,m_lTotalJettonScore,sizeof(PlaceJetton.lTotalJettonScore));

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	}
	else	//加注失败
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.cbJettonArea=cbJettonArea;
		PlaceJettonFail.lJettonScore=lJettonScore;

		//发送消息
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}

//特殊账号控制
bool CTableFrameSink::DispatchTableCardControl()
{
	if (m_cbControlArea==AREA_9 || m_cbControlArea==AREA_21)
		return false;
	if (m_cbControlArea!=0xff && m_cbControlTimes>0)
	{
		if (m_cbControlArea<PATH_STEP_NUMBER)
		{
			m_cbWinArea=m_cbControlArea;
			m_cbControlTimes--;
		}
		if (m_cbControlTimes==0)
		{
			m_cbControlArea=0xff;
		}
	}
	return true;
}

//发送扑克
bool CTableFrameSink::DispatchTableCard()
{
	//srand((unsigned)time(NULL));
	DWORD dwRateGoodLuck=rand()%1000;
	if (dwRateGoodLuck<m_dwRateGoodLuck)
	{
		m_cbWinArea=(rand()%2==0)?AREA_9:AREA_21;
	}
	else
	{
		do
		{
			m_cbWinArea=rand()%PATH_STEP_NUMBER;//随机24个区域
		} while (m_cbWinArea==AREA_9 || m_cbWinArea==AREA_21);
	}
	//m_cbWinArea=9;
	//m_cbWinArea=(rand()%2==0)?9:21;
	if (m_cbWinArea==AREA_9 || m_cbWinArea==AREA_21)//开中GoodLuck
	{
		DWORD dwRateType=rand()%(m_dwRateChiFen+m_dwRateCaiJin+m_dwRateDaQiang);
		if (dwRateType<m_dwRateChiFen) m_cbGoodLuckType=0;
		else if (dwRateType<(m_dwRateChiFen+m_dwRateCaiJin)) m_cbGoodLuckType=rand()%4+1;
		else m_cbGoodLuckType=5;
		//m_cbGoodLuckType=rand()%6;//0：吃分、1：大三元、2：大四喜、3：小三元、4：跑火车
		//m_cbGoodLuckType=5;
		//static temp=0;
		//m_cbGoodLuckType=temp%5;
		//temp++;
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_0)//吃分
		{
			m_cbPaoHuoCheCount=77;
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_1)//大三元
		{
			m_cbPaoHuoCheCount=3;
			if (m_cbWinArea==AREA_9)
			{
				m_cbPaoHuoCheArea[0]=7;
				m_cbPaoHuoCheArea[1]=19;
				m_cbPaoHuoCheArea[2]=15;
			}
			if (m_cbWinArea==AREA_21)
			{
				m_cbPaoHuoCheArea[0]=19;
				m_cbPaoHuoCheArea[1]=15;
				m_cbPaoHuoCheArea[2]=7;
			}
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_2)//大四喜
		{
			m_cbPaoHuoCheCount=4;
			if (m_cbWinArea==AREA_9)
			{
				m_cbPaoHuoCheArea[0]=4;
				m_cbPaoHuoCheArea[1]=22;
				m_cbPaoHuoCheArea[2]=16;
				m_cbPaoHuoCheArea[3]=10;
			}
			if (m_cbWinArea==AREA_21)
			{
				m_cbPaoHuoCheArea[0]=16;
				m_cbPaoHuoCheArea[1]=10;
				m_cbPaoHuoCheArea[2]=4;
				m_cbPaoHuoCheArea[3]=22;
			}
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_3)//小三元
		{
			m_cbPaoHuoCheCount=3;
			if (rand()%2==0)
			{
				if (m_cbWinArea==AREA_9)
				{
					m_cbPaoHuoCheArea[0]=1;
					m_cbPaoHuoCheArea[1]=18;
					m_cbPaoHuoCheArea[2]=12;
				}
				if (m_cbWinArea==AREA_21)
				{
					m_cbPaoHuoCheArea[0]=18;
					m_cbPaoHuoCheArea[1]=12;
					m_cbPaoHuoCheArea[2]=1;
				}
			}
			else
			{
				if (m_cbWinArea==AREA_9)
				{
					m_cbPaoHuoCheArea[0]=6;
					m_cbPaoHuoCheArea[1]=0;
					m_cbPaoHuoCheArea[2]=13;
				}
				if (m_cbWinArea==AREA_21)
				{
					m_cbPaoHuoCheArea[0]=13;
					m_cbPaoHuoCheArea[1]=6;
					m_cbPaoHuoCheArea[2]=0;
				}
			}
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_4)//跑火车
		{
			m_cbPaoHuoCheCount=rand()%6+2;//随机2至7个区域
			//新规定，跑火车为连续的区域
			//新的算法
			//10、11、12、13、14、15、16、17、18、19、20
			//22、23、0、1、2、3、4、5、6、7、8
			BYTE cbArea1[11]={10,11,12,13,14,15,16,17,18,19,20};
			BYTE cbArea2[11]={22,23,0,1,2,3,4,5,6,7,8};
			BYTE cbBegin=rand()%(12-m_cbPaoHuoCheCount);
			BYTE *cbArea=(rand()%2==0)?cbArea1:cbArea2;
			for (int i=0; i<m_cbPaoHuoCheCount; i++)
			{
				m_cbPaoHuoCheArea[i]=cbArea[cbBegin+i];
				ASSERT(m_cbPaoHuoCheArea[i]!=9 && m_cbPaoHuoCheArea[i]!=21);
			}
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_5)//随机打枪
		{
			m_cbPaoHuoCheCount=rand()%7+1;//随机1至7个区域
			BYTE cbArea1[22]={8,7,6,5,4,3,2,1,0,23,22,20,19,18,17,16,15,14,13,12,11,10};
			BYTE cbArea2[22]={20,19,18,17,16,15,14,13,12,11,10,8,7,6,5,4,3,2,1,0,23,22};
			BYTE cbIndex=0;
			if (m_cbWinArea==AREA_9)
			{
				for (int i=0; i<m_cbPaoHuoCheCount; )
				{
					cbIndex+=rand()%3+3;
					BYTE &cbTemp=cbArea1[cbIndex%22];
					if (cbTemp!=255)
					{
						m_cbPaoHuoCheArea[i]=cbTemp;
						cbTemp=255;//排除重复区域
						i++;
					}
				}
			}
			if (m_cbWinArea==AREA_21)
			{
				for (int i=0; i<m_cbPaoHuoCheCount; )
				{
					cbIndex+=rand()%3+3;
					BYTE &cbTemp=cbArea2[cbIndex%22];
					if (cbTemp!=255)
					{
						m_cbPaoHuoCheArea[i]=cbTemp;
						cbTemp=255;//排除重复区域
						i++;
					}
				}
			}
		}
	}

	return true;
}

//对于比大小的库存值控制
bool CTableFrameSink::ProbeJudge2(BYTE cbGuess, WORD wChairID)
{
	//系统输赢
	LONGLONG lSystemScore=0l;

	BYTE cbWin=2;
	if (m_cbBigSmall>0 && m_cbBigSmall<8) cbWin=0;
	if (m_cbBigSmall>7 && m_cbBigSmall<15) cbWin=1;
	ASSERT(cbWin!=2);

	bool bIsBankerAndroidUser=false;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem * pIBankerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem!=NULL)
		{
			bIsBankerAndroidUser=pIBankerUserItem->IsAndroidUser();
		}
	}

	//是否是机器人
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	bool bIsAndroidUser=pIServerUserItem->IsAndroidUser();

	if (cbGuess==cbWin)//猜对大小
	{
		if (bIsAndroidUser) lSystemScore+=m_lUserWinScore[wChairID];
		if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser) lSystemScore-=m_lUserWinScore[wChairID];
	}
	else
	{
		if (bIsAndroidUser) lSystemScore-=m_lUserWinScore[wChairID];
		if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser) lSystemScore+=m_lUserWinScore[wChairID];
	}

//printf("m_cbBigSmall=%d\n",m_cbBigSmall);
	if ((lSystemScore+m_StorageStart)<0l)
	{
		return false;
	}
	else
	{
		m_StorageStart+=lSystemScore;
//printf("lSystemScore=%I64d\n",lSystemScore/m_dwJettonRate);
//printf("m_StorageStart=%I64d\n",m_StorageStart/m_dwJettonRate);
		for (int i=0; i<GAME_PLAYER; i++)
		{
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;
			if (pIServerUserItem->GetUserRight()==1879048192)
			{
				CString szInfo;
				szInfo.Format(TEXT("押大小，库存值[%I64d]筹码"),m_StorageStart/m_dwJettonRate);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szInfo,SMT_CHAT);
			}
		}
		return true;
	}
}

//试探性判断
bool CTableFrameSink::ProbeJudge()
{
//printf(__FUNCTION__"\n\n");
	//是否有真实玩家下注
	bool bRealPlayerJetton=false;
	//系统输赢
	LONGLONG lSystemScore=0l;
	//庄家是不是机器人
	bool bIsBankerAndroidUser=false;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem * pIBankerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem!=NULL)
		{
			bIsBankerAndroidUser=pIBankerUserItem->IsAndroidUser();
//printf("机器人坐庄\n");
		}
	}
	//中奖区域、赔率
	BYTE cbHotArea=8;
	BYTE cbWinAreaMultiple=0;
	CalAreaMultiple(m_cbWinArea,cbHotArea,cbWinAreaMultiple);
	//计算积分
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		//庄家判断
		if (m_wCurrentBanker==i) continue;
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;
		//是否是机器人
		bool bIsAndroidUser=pIServerUserItem->IsAndroidUser();
		//普通区域
		for (int wAreaIndex=0; wAreaIndex<JETTON_AREA_COUNT; wAreaIndex++)
		{
			//////////
			//判断是否有真实玩家下注
			if (!bIsAndroidUser && m_lUserAreaScore[i][wAreaIndex]>0) bRealPlayerJetton=true;
			//////////
			if (cbHotArea==wAreaIndex)
			{
				if (bIsAndroidUser)
					lSystemScore+=m_lUserAreaScore[i][wAreaIndex]*cbWinAreaMultiple;
				if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore-=m_lUserAreaScore[i][wAreaIndex]*cbWinAreaMultiple;
			}
			//下分即扣除，不管这个区域赢没赢，因为此游戏是没有“返还下注”这个说法的
			if (bIsAndroidUser)
				lSystemScore-=m_lUserAreaScore[i][wAreaIndex];
			if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
				lSystemScore+=m_lUserAreaScore[i][wAreaIndex];
		}
		//开中GoodLuck
		if (cbHotArea==8)
		{
			switch (m_cbGoodLuckType)
			{
			case 0://吃分
				{
					//for (int jj=0; jj<JETTON_AREA_COUNT; jj++)
					//{
					//	if (bIsAndroidUser)
					//		lSystemScore-=m_lUserAreaScore[i][jj];
					//	if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
					//		lSystemScore+=m_lUserAreaScore[i][jj];
					//}
					break;
				}
			case 1://大三元
			case 2://大四喜
			case 3://小三元
			case 4://跑火车
			case 5://随机打枪
				{
					for (int jj=0; jj<m_cbPaoHuoCheCount; jj++)
					{
						BYTE cbHotArea2=8;
						BYTE cbWinAreaMultiple2=0;
						CalAreaMultiple(m_cbPaoHuoCheArea[jj],cbHotArea2,cbWinAreaMultiple2);
						ASSERT(cbHotArea2<8);
//printf("1:=%I64d\n",m_lUserAreaScore[i][cbHotArea2]*cbWinAreaMultiple2);
//printf("cbHotArea2=%d\n",cbHotArea2);
//printf("cbWinAreaMultiple2=%d\n",cbWinAreaMultiple2);
//printf("m_wCurrentBanker=%d\n",m_wCurrentBanker);
//printf("bIsAndroidUser=%d\n",bIsAndroidUser);
//printf("bIsBankerAndroidUser=%d\n",bIsBankerAndroidUser);
						if (bIsAndroidUser)
						{
							lSystemScore+=m_lUserAreaScore[i][cbHotArea2]*cbWinAreaMultiple2;
						}
						if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
						{
							lSystemScore-=m_lUserAreaScore[i][cbHotArea2]*cbWinAreaMultiple2;
						}
					}
				}
			}
		}
	}
	//系统分值计算
	if ((lSystemScore+m_StorageStart)<0l)
	{
		return false;
	}
	else
	{
		m_StorageStart+=lSystemScore;
//printf("m_StorageStart=%I64d\n",m_StorageStart/m_dwJettonRate);
		//有真实玩家下注，才衰减库存值
		if (bRealPlayerJetton)
			m_StorageStart=m_StorageStart-(m_StorageStart*m_StorageDeduct/1000);
//printf("lSystemScore=%I64d\n",lSystemScore/m_dwJettonRate);
//printf("m_StorageStart=%I64d\n",m_StorageStart/m_dwJettonRate);
		for (int i=0; i<GAME_PLAYER; i++)
		{
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;
			if (pIServerUserItem->GetUserRight()==1879048192)
			{
				CString szInfo;
				szInfo.Format(TEXT("库存值[%I64d]筹码"),m_StorageStart/m_dwJettonRate);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szInfo,SMT_CHAT);
			}
		}
		return true;
	}
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	if( pIApplyServerUserItem == NULL )
		return true;
#ifndef FUNC_CHIP
	//合法判断
	__int64 lUserScore=pIApplyServerUserItem->GetUserScore();
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你的金币不足以申请庄家，申请失败！"),SMT_PROMPT|SMT_EJECT);
		return true;
	}
#else
	//合法判断
	if (m_lCurrentJetton[pIApplyServerUserItem->GetUserInfo()->wChairID]<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你的筹码不足以申请庄家，申请失败！"),SMT_PROMPT|SMT_EJECT);
		return true;
	}
#endif

	//存在判断
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你已经申请了庄家，不需要再次申请！"),SMT_PROMPT|SMT_EJECT);
			return true;
		}
	}

	//保存信息 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//构造变量
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//设置变量
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
	{
		//发送消息
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("游戏已经开始，不可以取消当庄！"),SMT_PROMPT|SMT_EJECT);
		return true;
	}

	//存在判断
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//条件过滤
		if (pIServerUserItem==NULL) 
			continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) 
			continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));
			CancelBanker.wChairID = pICancelServerUserItem->GetChairID();

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//切换庄家 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return true;
}

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//切换标识
	bool bChangeBanker=false;

	//取消当前庄家做庄资格
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//获取玩家
			WORD wChairID=m_ApplyUserArray[i];

			//条件过滤
			if (wChairID!=m_wCurrentBanker) 
				continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//设置庄家
		m_wCurrentBanker=INVALID_CHAIR;

		//轮换判断
		if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()!=0)
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//设置变量
		bChangeBanker=true;
	}
	//轮庄判断。当前庄家暂时还有机会做庄
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if( pIServerUserItem == NULL )
			return FALSE;

		__int64 iBankerScore=pIServerUserItem->GetUserScore();

		//次数判断。如果超过了做庄的次数或者所剩积分不再满足做庄条件，则强制替换掉
		if (m_nMaxBankerTimes<=m_wBankerTime || iBankerScore<m_lApplyBankerCondition)
		{
			//撤销玩家
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//获取玩家
				WORD wChairID=m_ApplyUserArray[i];

				//条件过滤
				if (wChairID!=m_wCurrentBanker) continue;

				//删除玩家
				m_ApplyUserArray.RemoveAt(i);

				break;
			}

			//设置庄家
			m_wCurrentBanker=INVALID_CHAIR;

			//轮换判断
			if (m_ApplyUserArray.GetCount()!=0)
			{
				m_wCurrentBanker=m_ApplyUserArray[0];
			}

			bChangeBanker=true;

			//提示消息
			TCHAR szTipMsg[128];
			if (iBankerScore<m_lApplyBankerCondition)
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
			else
            	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetNickName(),m_nMaxBankerTimes);

			//发送消息
			SendGameMessage(INVALID_CHAIR,szTipMsg);	
		}
	}
	//如果当前是系统做庄，则可以换一个真人玩家来做庄
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		m_wCurrentBanker=m_ApplyUserArray[0];
		bChangeBanker=true;
	}

	//切换判断
	if (bChangeBanker)
	{
		//设置变量
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//发送消息
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			if( pIServerUserItem == NULL )
				return FALSE;
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
			wcscpy(ChangeBanker.szBankerNickName,pIServerUserItem->GetNickName());
			ChangeBanker.lBankerChip = m_lCurrentJetton[m_wCurrentBanker];
		}
		else
		{
			//如果if的条件不满足，应该如何处理？
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//读取消息
			LONG lMessageCount=GetPrivateProfileInt(TEXT("Message"),TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//读取配置
				LONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%ld"),lIndex);
				GetPrivateProfileString(TEXT("Message"),szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//获取玩家
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

				//发送消息
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetNickName());
				SendGameMessage(INVALID_CHAIR,szMessage2);
			}
		}
	}

	return bChangeBanker;
}

//发送申请上庄的玩家列表
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pServerUserItem == NULL) 
			continue;

		//庄家判断
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) 
			continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));
		ApplyBanker.wApplyUser=wChairID;

		//发送消息
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//切换庄家
	if (wChairID==m_wCurrentBanker) 
		ChangeBanker(true);

	//取消申请
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) 
			continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//设置变量
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//区域转换，以及计算倍率
void CTableFrameSink::CalAreaMultiple(const BYTE cbWinArea, BYTE &cbOutArea, BYTE &cbMultiple)
{
	switch(cbWinArea)
	{
	case 5://2倍区，苹果
		{
			cbOutArea=7;
			cbMultiple=2;
			break;
		}
	case 8://2倍区，西瓜
		{
			cbOutArea=3;
			cbMultiple=2;
			break;
		}
	case 11://2倍区，桔子
		{
			cbOutArea=6;
			cbMultiple=2;
			break;
		}
	case 14://2倍区，双七
		{
			cbOutArea=1;
			cbMultiple=2;
			break;
		}
	case 17://2倍区，木瓜
		{
			cbOutArea=5;
			cbMultiple=2;
			break;
		}
	case 20://2倍区，星星
		{
			cbOutArea=2;
			cbMultiple=2;
			break;
		}
	case 23://2倍区，铃铛
		{
			cbOutArea=4;
			cbMultiple=2;
			break;
		}
	case 0:
	case 12://桔子
		{
			cbOutArea = 6;
			cbMultiple=10;
			break;
		}
	case 1://铃铛
	case 13:
		{
			cbOutArea = 4;
			cbMultiple=10;
			break;
		}
	case 2://BAR
		{
			cbOutArea=0;
			cbMultiple=25;
			break;
		}
	case 3://BAR
		{
			cbOutArea = 0;
			cbMultiple=50;
			break;
		}
	case 4://苹果
	case 10:
	case 16:
	case 22:
		{
			cbOutArea = 7;
			cbMultiple=5;
			break;
		}
	case 6://木瓜
	case 18:
		{
			cbOutArea = 5;
			cbMultiple=10;
			break;
		}
	case 7://西瓜
		{
			cbOutArea = 3;
			cbMultiple=20;
			break;
		}
	case 9://好运
	case 21:
		{
			cbOutArea = 8;
			cbMultiple=0;
			break;
		}
	case 15://双七
		{
			cbOutArea = 1;
			cbMultiple=20;
			break;
		}
	case 19://星星
		{
			cbOutArea = 2;
			cbMultiple=20;
			break;
		}
	}
}

//计算得分
__int64 CTableFrameSink::CalculateScore()
{
	////////////////////////////////////////////////////////////////////////////////
	//游戏记录
	tagServerGameRecord &GameRecord=m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbRecord=m_cbWinArea;
	GameRecord.dwGameTimes=m_dwGameTimes;
	GameRecord.bGoodLuck=false;//修正，此处不赋值，将会导致1000条记录以后，LUCKY图标可能出现在非GoodLuck开奖上
	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//////////
	//开中GoodLuck时，保存历史记录
	if (m_cbWinArea==9 || m_cbWinArea==21)
	{
		if (m_cbGoodLuckType!=0)
		{
			m_cbGameRecordMultipleCount=m_cbPaoHuoCheCount;
			for (int i=0; i<m_cbPaoHuoCheCount; i++)
			{
				m_dwGameTimes++;
				//总记录不能在这里保存，因为会导致游戏未结束，而游戏中进入的玩家，看到游戏结果
				//tagServerGameRecord &GameRecord=m_GameRecordArrary[m_nRecordLast];
				//GameRecord.cbRecord=m_cbPaoHuoCheArea[i];
				//GameRecord.dwGameTimes=m_lGameTimes;
				//GameRecord.bGoodLuck=true;
				////移动下标
				//m_nRecordLast=(m_nRecordLast+1)%MAX_SCORE_HISTORY;
				//if (m_nRecordLast==m_nRecordFirst) m_nRecordFirst=(m_nRecordFirst+1)%MAX_SCORE_HISTORY;
				m_GameRecordMultiple[i].cbRecord=m_cbPaoHuoCheArea[i];
				m_GameRecordMultiple[i].dwGameTimes=m_dwGameTimes;
				m_GameRecordMultiple[i].bGoodLuck=true;
//printf("m_GameRecordMultiple[i].cbRecord=%d\n",m_GameRecordMultiple[i].cbRecord);
//printf("m_GameRecordMultiple[i].lGameTimes=dwGameTimes=%d\n",m_GameRecordMultiple[i].lGameTimes=dwGameTimes);
			}
		}
	}
	//////////
	////////////////////////////////////////////////////////////////////////////////
	//庄家本局成绩
	__int64 lBankerWinScore=0;
	//玩家成绩
	__int64 lUserLostScore[GAME_PLAYER];
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore,sizeof(lUserLostScore));
	//中奖区域、赔率
	BYTE cbHotArea=8;
	BYTE cbWinAreaMultiple=0;
	CalAreaMultiple(m_cbWinArea,cbHotArea,cbWinAreaMultiple);
//printf("cbHotArea=%d\n",cbHotArea);
//printf("cbWinAreaMultiple=%d\n",cbWinAreaMultiple);
	//计算积分
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		//庄家判断
		if (m_wCurrentBanker==i) continue;
		//获取用户
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;
		//普通区域
		for(int iArea=0; iArea<JETTON_AREA_COUNT; iArea++)
		{
			if (cbHotArea==iArea)
			{
				m_lUserWinScore[i]+=m_lUserAreaScore[i][iArea]*cbWinAreaMultiple;
				lBankerWinScore-=m_lUserAreaScore[i][iArea]*cbWinAreaMultiple;
				//if (m_wCurrentBanker!=INVALID_CHAIR) m_lUserJetton[m_wCurrentBanker]-=m_lUserAreaScore[i][iArea]*cbWinAreaMultiple;
			}
			//lUserLostScore[i]-=m_lUserAreaScore[i][iArea];
			m_lUserJetton[i]-=m_lUserAreaScore[i][iArea];//玩家所下注的筹码
			lBankerWinScore+=m_lUserAreaScore[i][iArea];//庄家先临时得到这些筹码
		}

		//开中GoodLuck
		if (cbHotArea==8)
		{
//printf("开中GoodLuck\n");
//printf("m_cbGoodLuckTyp=%d\n",m_cbGoodLuckType);
			switch (m_cbGoodLuckType)
			{
			case 0://吃分
				{
					m_lUserWinScore[i]=0;
					lBankerWinScore=0;//庄家不输不赢
					break;
				}
			case 1://大三元
			case 2://大四喜
			case 3://小三元
			case 4://跑火车
			case 5://随机打枪
				{
					ASSERT(m_lUserWinScore[i]==0);
					for (int jj=0; jj<m_cbPaoHuoCheCount; jj++)
					{
						BYTE cbHotArea2=8;
						BYTE cbWinAreaMultiple2=0;
						CalAreaMultiple(m_cbPaoHuoCheArea[jj],cbHotArea2,cbWinAreaMultiple2);
						ASSERT(cbHotArea2<8);
//printf("m_cbPaoHuoCheArea[jj]=%d\n",m_cbPaoHuoCheArea[jj]);
//printf("cbHotArea2=%d\n",cbHotArea2);
//printf("cbWinAreaMultiple2=%d\n",cbWinAreaMultiple2);
						m_lUserWinScore[i]+=m_lUserAreaScore[i][cbHotArea2]*cbWinAreaMultiple2;
					}
					break;
				}
			}
			lBankerWinScore-=m_lUserWinScore[i];
		}
		//抽水在写分前
		//总的分数
		//m_lUserWinScore[i] += lUserLostScore[i];
	}

	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker]=lBankerWinScore;
		//抽水在写分前
	}

	//累计积分
	//m_lBankerWinScore+=lBankerWinScore+((m_wCurrentBanker==INVALID_CHAIR)?0:m_lUserJetton[m_wCurrentBanker]);
	m_lBankerWinScore+=lBankerWinScore;
	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;
	return lBankerWinScore;
}

//发送记录
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	//BYTE cbBuffer[SOCKET_BUFFER];
	BYTE cbBuffer[4086];
	int nIndex = m_nRecordFirst;
	while ( nIndex != m_nRecordLast )
	{
		if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
			wBufferSize=0;
		}
		CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
		wBufferSize+=sizeof(tagServerGameRecord);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}
	if (wBufferSize>0) 
		m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//发送消息
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//游戏玩家
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_PROMPT);
		}

		//旁观玩家
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_PROMPT);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_PROMPT|SMT_EJECT);
	}
}

//特殊账号控制
bool CTableFrameSink::OnAdminControl(CMD_C_ControlApplication * pData, IServerUserItem * pIServerUserItem)
{
//printf("申请类型：[%d]\n",pData->cbControlAppType);
//printf("控制区域：[%d]\n",pData->cbControlArea);
//printf("控制次数：[%d]\n",pData->cbControlTimes);
	switch (pData->cbControlAppType)
	{
	case C_CA_UPDATE://更新
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType=S_CR_UPDATE_SUCCES;
			ControlReturns.cbControlArea=m_cbControlArea;
			ControlReturns.cbControlTimes=m_cbControlTimes;
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_SET://设置
		{
			if (pData->cbControlArea==0xff || pData->cbControlArea>PATH_STEP_NUMBER)
			{
				//设置失败
				CMD_S_ControlReturns ControlReturns;
				ZeroMemory(&ControlReturns,sizeof(ControlReturns));
				ControlReturns.cbReturnsType=S_CR_FAILURE;
				ControlReturns.cbControlArea=0xff;
				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
				return true;
			}
			m_cbControlArea=pData->cbControlArea;
			m_cbControlTimes=pData->cbControlTimes;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType=S_CR_SET_SUCCESS;
			ControlReturns.cbControlArea=m_cbControlArea;
			ControlReturns.cbControlTimes=m_cbControlTimes;
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_CANCELS://取消
		{
			m_cbControlArea=0xff;
			m_cbControlTimes=0;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType=S_CR_CANCEL_SUCCESS;
			ControlReturns.cbControlArea=0xff;
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	}
	//设置失败
	CMD_S_ControlReturns ControlReturns;
	ZeroMemory(&ControlReturns,sizeof(ControlReturns));
	ControlReturns.cbReturnsType=S_CR_FAILURE;
	ControlReturns.cbControlArea=0xff;
	m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
	return true;
}

//读取配置
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SGDZ.ini"),szPath);
	//mystrcpy(m_szGameRoomName,TEXT("水果乐园"));
	mystrcpy(m_szGameRoomName,m_pGameServiceOption->szServerName);//房间名称

	//每盘刷新
	if (!bReadFresh && GetPrivateProfileInt(m_szGameRoomName,TEXT("Refresh"),0,m_szConfigFileName)==0) return;

	//////////
	//机器人自动存取分
	m_lRobotMoneyLine=GetPrivateProfileInt(m_szGameRoomName,TEXT("RobotMoneyLine"),100000,m_szConfigFileName);
	m_lRobotAddScore=GetPrivateProfileInt(m_szGameRoomName,TEXT("RobotAddScore"),500000,m_szConfigFileName);
	m_lRoboMaxMoney=GetPrivateProfileInt(m_szGameRoomName,TEXT("RoboMaxMoney"),1000000,m_szConfigFileName);
	m_nSetlower=GetPrivateProfileInt(m_szGameRoomName,TEXT("Setlower"),10,m_szConfigFileName);
	BYTE cbAddMoney=GetPrivateProfileInt(m_szGameRoomName,TEXT("IsAdd"),0,m_szConfigFileName);
	m_bAddMoney=(cbAddMoney!=0)?true:false;
	//////////
//printf("m_lRobotMoneyLine=%I64d\n",m_lRobotMoneyLine);
//printf("m_lRobotAddScore=%I64d\n",m_lRobotAddScore);
//printf("m_lRoboMaxMoney=%I64d\n",m_lRoboMaxMoney);
	//时间控制
	m_cbFreeTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("FreeTime"), 10, m_szConfigFileName);
	m_cbBetTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetTime"), 20, m_szConfigFileName);
	m_cbEndTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("EndTime"), 20, m_szConfigFileName);

	if (m_cbFreeTime <= 0 || m_cbFreeTime > 99)
		m_cbFreeTime = 10;
	if (m_cbBetTime <= 0 || m_cbBetTime > 99)
		m_cbBetTime = 20;
	if (m_cbEndTime <= 0 || m_cbEndTime > 99)
		m_cbEndTime = 20;

	//几率控制
	m_dwRateGoodLuck=GetPrivateProfileInt(m_szGameRoomName,TEXT("GoodLuck"),5,m_szConfigFileName);
	m_dwRateChiFen=GetPrivateProfileInt(m_szGameRoomName,TEXT("ChiFen"),10,m_szConfigFileName);
	m_dwRateCaiJin=GetPrivateProfileInt(m_szGameRoomName,TEXT("CaiJin"),20,m_szConfigFileName);
	m_dwRateDaQiang=GetPrivateProfileInt(m_szGameRoomName,TEXT("DaQiang"),70,m_szConfigFileName);
	if (m_dwRateGoodLuck<0 || m_dwRateGoodLuck>1000) m_dwRateGoodLuck=5;
	if (m_dwRateChiFen<1 || m_dwRateChiFen>100) m_dwRateChiFen=100;
	if (m_dwRateCaiJin<1 || m_dwRateCaiJin>100) m_dwRateCaiJin=100;
	if (m_dwRateDaQiang<1 || m_dwRateDaQiang>100) m_dwRateDaQiang=100;
//printf("m_dwRateGoodLuck=%d\n",m_dwRateGoodLuck);
//printf("m_dwRateChiFen=%d\n",m_dwRateChiFen);
//printf("m_dwRateCaiJin=%d\n",m_dwRateCaiJin);
//printf("m_dwRateDaQiang=%d\n",m_dwRateDaQiang);

	//控制变量
	__int64 lMaxScore=10000;
	CString str;

	//筹码比率
	m_dwJettonRate=GetPrivateProfileInt(m_szGameRoomName,TEXT("CellScore"),100,m_szConfigFileName);
	if (m_dwJettonRate<=0) m_dwJettonRate=100;

	//配置信息
	TCHAR szBuffer[MAX_PATH];
	GetPrivateProfileString(m_szGameRoomName,TEXT("Score"),TEXT("50000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	myscanf(szBuffer,mystrlen(szBuffer),TEXT("%I64d"),&m_lApplyBankerCondition);

	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("SysLimitScore"),TEXT("10000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	myscanf(szBuffer,mystrlen(szBuffer),TEXT("%I64d"),&m_lSysLimitScore);

	GetPrivateProfileString(m_szGameRoomName,TEXT("UserLimitScore"),TEXT("800000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	myscanf(szBuffer,mystrlen(szBuffer),TEXT("%I64d"),&m_lUserLimitScore);

	BYTE cbEnableSysBanker=GetPrivateProfileInt(m_szGameRoomName, TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	BYTE cbCtrlWin = GetPrivateProfileInt(TEXT("ControlWin"), TEXT("Win"), 0, m_szConfigFileName);
	m_bControlWin = cbCtrlWin>0 ? true : false;

	//庄家一次做庄多少局
	m_nMaxBankerTimes = GetPrivateProfileInt(m_szGameRoomName, TEXT("Time"), 10, m_szConfigFileName);

	if (bReadFresh)
	{
		m_StorageStart=GetPrivateProfileInt(m_szGameRoomName,TEXT("StorageStart"),0,m_szConfigFileName);
	}

	m_StorageDeduct=GetPrivateProfileInt(m_szGameRoomName,TEXT("StorageDeduct"),5,m_szConfigFileName);
//printf("m_StorageStart=%I64d\n",m_StorageStart);
//printf("m_StorageDeduct=%I64d\n",m_StorageDeduct);
	//读取控制账号信息
	if( m_bControlWin)
	{
		int nAccountNum = GetPrivateProfileInt(TEXT("ControlWin"), TEXT("CtrlAccountNum"), 0, m_szConfigFileName);
		if( nAccountNum > 0 )
		{
			for( int i=0; i<nAccountNum; i++)
			{
				//TCHAR szTemp[64]={0};
				TCHAR szTemp[128]=_TEXT("");
				ControlAccount CtrlAccount;
				ZeroMemory(&CtrlAccount, sizeof(CtrlAccount));
				//sprintf(szTemp, "Account_%d", i);
				_sntprintf(szTemp,sizeof(szTemp),TEXT("Account_%d"), i);
				CtrlAccount.dwUserID = GetPrivateProfileInt(TEXT("ControlWin"), szTemp, -1, m_szConfigFileName);
				if( CtrlAccount.dwUserID >=0 )
				{
					ZeroMemory(szTemp, sizeof(szTemp));
					//sprintf(szTemp, "WinOdds_%d", i);
					_sntprintf(szTemp,sizeof(szTemp),TEXT("WinOdds_%d"), i);
					CtrlAccount.nOdds = GetPrivateProfileInt(TEXT("ControlWin"), szTemp, 0, m_szConfigFileName);
					m_CtrlAccountArray.Add(CtrlAccount);
				}
			}
		}
	}
}

//机器人自动存取分
void CTableFrameSink::RobotAddMoney(__int64 lRobotMoneyLine, __int64 lRobotAddScore, __int64 lRoboMaxMoney, int nSetlower, bool bAddMoney)
{
	LONGLONG lMyscore,lBakScore;
	CString strtmp;
	if (bAddMoney)
	{
		for (WORD i=0; i<GAME_PLAYER; i++)
		{
			lBakScore=0;
			strtmp="不变";
			IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pServerUserItem!=NULL && pServerUserItem->IsAndroidUser())
			{
				lMyscore=(LONGLONG)pServerUserItem->GetUserScore();//m_pITableFrame->GetTableUserItem(i)->GetUserScore()->lScore;
				//printf("lMyscore=%I64d\n",lMyscore);
				//printf("存款：%I64d\n",pServerUserItem->GetUserInsure());
				if (lMyscore<m_lRobotMoneyLine)
				{
					/*if ((LONGLONG)pServerUserItem->GetUserScore()<m_lRobotAddScore)
					{
						lBakScore=(LONGLONG)pServerUserItem->GetUserScore();
					}
					else*/
					{
						lBakScore=lMyscore + m_lRobotAddScore;
					}
					strtmp="底限自动加";
					pServerUserItem->ModifyUserInsure(lBakScore,(-1)*lBakScore,0);
				}
				if (lMyscore>m_lRoboMaxMoney && m_wCurrentBanker!=i)
				{
					lBakScore=lMyscore*m_nSetlower/100;
					lMyscore=lMyscore-lBakScore;
					strtmp="超限自动减";
					pServerUserItem->ModifyUserInsure((-1)*lBakScore,lBakScore,0);
				}
				m_lCurrentJetton[i]=pServerUserItem->GetUserInfo()->lScore;
				CMD_S_ChipUpdate ChipUpdate;
				ZeroMemory(&ChipUpdate,sizeof(ChipUpdate));
				ChipUpdate.wChairID=i;
				ChipUpdate.lTotalScore=m_lCurrentJetton[i];
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHIP_UPDATE,&ChipUpdate,sizeof(ChipUpdate));//机器人存取分时的更新
//printf("[%s]：%s[%I64d]，筹码[%I64d]\n",pServerUserItem->GetNickName(),strtmp,lBakScore,m_lCurrentJetton[i]);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
