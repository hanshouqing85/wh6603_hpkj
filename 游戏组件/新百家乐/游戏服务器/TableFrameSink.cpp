#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					300									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//空闲时间
#define IDI_FREE					1									//空闲时间
#define TIME_FREE					10									//空闲时间

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间
#define TIME_PLACE_JETTON			20									//下注时间

//结束时间
#define IDI_GAME_END				3									//结束时间
#define TIME_GAME_END				20									//结束时间


//信息查询
#define KEY_STOCK					0
#define KEY_IMMORTAL_COUNT			1
#define KEY_ROBOT_COUNT				2
#define KEY_IMMORTAL_BET			3	
#define KEY_ROBOT_BET				4
#define KEY_MAX						5

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏记录
	m_GameRecord.Empty();

	//下注数
	ZeroMemory(m_lAllBet,sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet,sizeof(m_lPlayBet));

	//分数
	m_lBankerScore = 0l;
	ZeroMemory(m_lPlayScore,sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//状态变量
	m_dwBetTime=0L;

	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_wFaceID = rand()%2;

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//庄家设置
	m_lBankerMAX = 0l;
	m_lBankerAdd = 0l;							
	m_lBankerScoreMAX = 0l;
	m_lBankerScoreAdd = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;

	//系统输赢区间
	m_nWinMaxPercent=80;
	m_nWinMinPercent=20;

	//时间控制
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	//机器人控制
	m_nChipRobotCount = 0;


	//服务控制
	m_hControlInst = NULL;
	m_pServerContro = NULL;
	m_hControlInst = LoadLibrary(TEXT("BaccaratNewServerControl.dll"));
	if ( m_hControlInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hControlInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
	if( m_pServerContro )
	{
		delete m_pServerContro;
		m_pServerContro = NULL;
	}

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}
}

//释放对象
VOID CTableFrameSink::Release()
{
	if( m_pServerContro )
	{
		delete m_pServerContro;
		m_pServerContro = NULL;
	}

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}
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
	if (m_pITableFrame == NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//读取配置
	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,CountArray(m_szConfigFileName),TEXT("%s\\BaccaratNewConfig.ini"),szPath);

	ReadConfigInformation(true);
	return true;
}

//复位桌子
void CTableFrameSink::RepositionSink()
{
	//游戏记录
	m_GameRecord.Empty();

	//下注数
	ZeroMemory(m_lAllBet,sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet,sizeof(m_lPlayBet));

	//分数
	m_lBankerScore = 0l;
	ZeroMemory(m_lPlayScore,sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//机器人控制
	m_nChipRobotCount = 0;

	return;
}


//游戏状态
bool CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//获取庄家
	IServerUserItem* pIBankerServerUserItem = NULL;
	if ( m_wCurrentBanker == INVALID_CHAIR )
	{
		m_lBankerScore = 10000;
		m_GameRecord.AppendFormat(TEXT("系统庄,%I64d#"),m_lBankerScore);
	}
	else
	{
		IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if ( pIBankerServerUserItem != NULL )
		{
			m_lBankerScore = (LONGLONG)pIBankerServerUserItem->GetUserScore();
		}
		m_GameRecord.AppendFormat(TEXT("庄:%s,%I64d#"),pIBankerServerUserItem->GetNickName(),m_lBankerScore);
	}

	//设置变量
	GameStart.cbTimeLeave = m_cbBetTime;
	GameStart.wBankerUser = m_wCurrentBanker;
	GameStart.lBankerScore = m_lBankerScore;

	//下注机器人数量
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}
	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);
	nChipRobotCount = 0;
	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++) 
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray.GetAt(i));
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

//	if(nChipRobotCount > 0)
//		GameStart.nAndriodCount=nChipRobotCount-1;


	//机器人控制
	m_nChipRobotCount = 0;

    //旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//游戏玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		//设置积分
		TRACE(TEXT("UserFreeScore :: %I64d"), pIServerUserItem->GetUserScore());
		GameStart.lPlayBetScore=min((LONGLONG)pIServerUserItem->GetUserScore() ,m_lUserLimitScore);
		GameStart.lPlayFreeSocre = (LONGLONG)pIServerUserItem->GetUserScore();

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束	
		{
			//计算分数
			LONGLONG lBankerWinScore = GameOver();

			//递增次数
			m_wBankerTime++;

			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//庄家信息
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;

			//扑克信息
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			CopyMemory(GameEnd.cbCardCount,m_cbCardCount,sizeof(m_cbCardCount));

			//发送积分
			GameEnd.cbTimeLeave=m_cbEndTime;	
			for ( WORD i = 0; i < GAME_PLAYER; ++i )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if ( pIServerUserItem == NULL ) continue;

				//设置成绩
				GameEnd.lPlayAllScore = m_lUserWinScore[i];
				memcpy( GameEnd.lPlayScore, m_lPlayScore[i], sizeof(GameEnd.lPlayScore));
				
				//设置税收
				GameEnd.lRevenue = (LONGLONG)m_lUserRevenue[i];

				//发送消息					
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			TCHAR szTemp[255]=TEXT("");
			swprintf(szTemp, sizeof(szTemp),TEXT("游戏结束,当前库存：%I64d"),m_StorageStart);
			//m_DebugWindow.WriteString(szTemp);

			////////////////////////////////////////////////////////////

			//设置文件名
			//获取目录
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetCurrentDirectory(CountArray(szPath),szPath);
			//读取配置
			_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\BaccaratNewConfig.ini"),szPath);

			//TCHAR szPath[MAX_PATH] = TEXT("");
			//TCHAR szConfigFileName[MAX_PATH] = TEXT("");
			//TCHAR OutBuf[255] = TEXT("");
			//GetCurrentDirectory(sizeof(szPath), szPath);
			//_sntprintf(szConfigFileName, CountArray(szConfigFileName), TEXT("%s\\%s"), szPath, szFileName);

	

			//判断是否存在真实玩家
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//存在真实用户
					bUser = true;
					//跳出FOR
					break;

				}
			}
			if (bUser)
			{

				//CString strLog;
				//strLog.Format(TEXT("桌号：%d,当前库存：%0.2f,系统得分：%0.2f,税收：%0.2f,剩余库存：%0.2f,库存衰减：%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				//CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息


				
				//m_lStockLimit+=lSystemScore;m_StorageStart

				CString szlStorageStart;
				szlStorageStart.Format(TEXT("%d"),m_StorageStart);
				//记录总输赢记录
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageStart"),szlStorageStart,m_szFileName);


				///////////////////////////////////


				////如果存在控制用户的话//发送游戏信息给控制号
				//for(int i=0; i<GAME_PLAYER; i++)
				//{
				//	IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
				//	if(pUserItem==NULL) continue;	//如果用户不存在
				//	if(pUserItem->IsAndroidUser()) continue;  //如果是机器人
				//	if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //判断用户控制权限
				//	m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //发送消息窗口
				//}
			}

			CTraceService::TraceString(szTemp,TraceLevel_Info);

			return true;
		}
	case GER_NETWORK_ERROR:		//用户断线
	case GER_USER_LEAVE:		//用户离开
		{
			if(cbReason ==GER_NETWORK_ERROR )
			{
				m_GameRecord.AppendFormat(TEXT("%s,断线#"),pIServerUserItem->GetNickName());
			}
			else
			{
				m_GameRecord.AppendFormat(TEXT("%s,离开#"),pIServerUserItem->GetNickName());
			}
			//闲家判断
			if (m_wCurrentBanker!=wChairID)
			{
				//变量定义
				LONGLONG lRevenue=0;

				//写入积分
				if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
				{
					for ( WORD wAreaIndex = AREA_XIAN; wAreaIndex <= AREA_ZHUANG_DUI; ++wAreaIndex )
					{
						if (m_lPlayBet[wChairID][wAreaIndex] != 0)
						{
							CMD_S_PlaceBetFail PlaceBetFail;
							ZeroMemory(&PlaceBetFail,sizeof(PlaceBetFail));
							PlaceBetFail.lBetArea = (BYTE)wAreaIndex;
							PlaceBetFail.lPlaceScore = m_lPlayBet[wChairID][wAreaIndex];
							PlaceBetFail.wPlaceUser = wChairID;

							//游戏玩家
							for (WORD i=0; i<GAME_PLAYER; ++i)
							{
								IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
								if (pIServerUserItem == NULL) 
									continue;

								m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceBetFail,sizeof(PlaceBetFail));
							}

							m_lAllBet[wAreaIndex] -= m_lPlayBet[wChairID][wAreaIndex];
							m_lPlayBet[wChairID][wAreaIndex] = 0;
						}
					}
				}
				else
				{
					//写入积分
					if (m_lUserWinScore[wChairID] >0.001||m_lUserWinScore[wChairID] <-0.001) 
					{
						tagScoreInfo ScoreInfoArray[GAME_PLAYER];
						ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

						if ( m_lUserWinScore[wChairID] > 0.0 )
							ScoreInfoArray[wChairID].cbType=SCORE_TYPE_WIN;
						else if ( m_lUserWinScore[wChairID] < 0.0 )
							ScoreInfoArray[wChairID].cbType=SCORE_TYPE_LOSE;
						else
							ScoreInfoArray[wChairID].cbType=SCORE_TYPE_DRAW;

						ScoreInfoArray[wChairID].lScore=m_lUserWinScore[wChairID];
						ScoreInfoArray[wChairID].lRevenue=m_lUserRevenue[wChairID];


						m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
						m_lUserWinScore[wChairID] = 0.00;

						m_GameRecord.AppendFormat(TEXT("%s得分:%0.2f,%0.2f#"),m_pITableFrame->GetTableUserItem(wChairID)->GetNickName(),ScoreInfoArray[wChairID].lScore, ScoreInfoArray[wChairID].lRevenue);
					}

					//清除下注
					for (WORD wAreaIndex = AREA_XIAN; wAreaIndex <= AREA_ZHUANG_DUI; ++wAreaIndex )
					{
						m_lPlayBet[wChairID][wAreaIndex] = 0;
					}
				}

				return true;
			}

			//状态判断
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_END)
			{
				//提示消息
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于庄家强退，游戏提前结束！"));


				for ( WORD i = 0; i < GAME_PLAYER; ++i )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
					if ( pIServerUserItem == NULL ) continue;

					//发送消息
					CMD_S_TipInfo TipInfo;
					TipInfo.cbTimeLeave = 10;
					_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("由于庄家强退，游戏提前结束！"));
					m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
				}

				//设置状态
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				//设置时间
				m_dwBetTime=(DWORD)time(NULL);
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//计算分数
				GameOver();

				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//庄家信息
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (m_lBankerWinScore>0L) GameEnd.lBankerScore=0;

				//扑克信息
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				CopyMemory(GameEnd.cbCardCount,m_cbCardCount,sizeof(m_cbCardCount));

				//发送积分
				GameEnd.cbTimeLeave=m_cbEndTime;	
				for ( WORD i = 0; i < GAME_PLAYER; ++i )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
					if ( pIServerUserItem == NULL ) continue;

					//设置成绩
					GameEnd.lPlayAllScore = m_lUserWinScore[i];
					memcpy( GameEnd.lPlayScore, m_lPlayScore[i], sizeof(GameEnd.lPlayScore));

					//设置税收
					GameEnd.lRevenue = (LONGLONG)m_lUserRevenue[i];

					//发送消息					
					m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//扣除分数
			if (m_lUserWinScore[m_wCurrentBanker] != 0.0)
			{

				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
				ScoreInfoArray[m_wCurrentBanker].cbType=SCORE_TYPE_DRAW;

				if ( m_lUserWinScore[m_wCurrentBanker] > 0.0 )
					ScoreInfoArray[m_wCurrentBanker].cbType=SCORE_TYPE_WIN;
				else if ( m_lUserWinScore[m_wCurrentBanker] < 0.0 )
					ScoreInfoArray[m_wCurrentBanker].cbType=SCORE_TYPE_LOSE;
				else
					ScoreInfoArray[m_wCurrentBanker].cbType=SCORE_TYPE_DRAW;

				ScoreInfoArray[m_wCurrentBanker].lScore=m_lUserWinScore[m_wCurrentBanker];
				ScoreInfoArray[m_wCurrentBanker].lRevenue=m_lUserRevenue[m_wCurrentBanker];
				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
				m_lUserWinScore[m_wCurrentBanker] = 0.0;
			}

			//切换庄家
			ChangeBanker(true);

			TCHAR szTemp[255]=TEXT("");
			swprintf(szTemp, sizeof(szTemp),TEXT("游戏结束,当前库存：%I64d"),m_StorageStart);
			//m_DebugWindow.WriteString(szTemp);

			////////////////////////////////////////////////////////////

			//设置文件名
			//获取目录
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetCurrentDirectory(CountArray(szPath),szPath);
			//读取配置
			_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\BaccaratNewConfig.ini"),szPath);

			//TCHAR szPath[MAX_PATH] = TEXT("");
			//TCHAR szConfigFileName[MAX_PATH] = TEXT("");
			//TCHAR OutBuf[255] = TEXT("");
			//GetCurrentDirectory(sizeof(szPath), szPath);
			//_sntprintf(szConfigFileName, CountArray(szConfigFileName), TEXT("%s\\%s"), szPath, szFileName);



			//判断是否存在真实玩家
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//存在真实用户
					bUser = true;
					//跳出FOR
					break;

				}
			}
			if (bUser)
			{

				//CString strLog;
				//strLog.Format(TEXT("桌号：%d,当前库存：%0.2f,系统得分：%0.2f,税收：%0.2f,剩余库存：%0.2f,库存衰减：%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				//CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息



				//m_lStockLimit+=lSystemScore;m_StorageStart

				CString szlStorageStart;
				szlStorageStart.Format(TEXT("%d"),m_StorageStart);
				//记录总输赢记录
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageStart"),szlStorageStart,m_szFileName);


				///////////////////////////////////


				////如果存在控制用户的话//发送游戏信息给控制号
				//for(int i=0; i<GAME_PLAYER; i++)
				//{
				//	IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
				//	if(pUserItem==NULL) continue;	//如果用户不存在
				//	if(pUserItem->IsAndroidUser()) continue;  //如果是机器人
				//	if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //判断用户控制权限
				//	m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //发送消息窗口
				//}
			}


			return true;
		}
	}

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:			//空闲状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//全局信息
			DWORD dwPassTime = (DWORD)time(NULL)-m_dwBetTime;
			StatusFree.cbTimeLeave = (BYTE)(m_cbFreeTime - __min(dwPassTime, (DWORD)m_cbFreeTime));

			//玩家信息
			StatusFree.lPlayFreeSocre = (LONGLONG)pIServerUserItem->GetUserScore();
			//庄家信息
			StatusFree.wBankerUser = m_wCurrentBanker;	
			StatusFree.wBankerTime = m_wBankerTime;
			StatusFree.lBankerWinScore = m_lBankerWinScore;
			StatusFree.lBankerScore = m_lBankerScore;
			StatusFree.wFaceID = m_wFaceID;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				wcscpy(StatusFree.szBankerNickName,pIServerUserItem->GetNickName());
			}


			//是否系统坐庄
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;

			//控制信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//房间名称
			CopyMemory(StatusFree.szGameRoomName, m_szGameRoomName, sizeof(StatusFree.szGameRoomName));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n本房间上庄条件为：%s\n区域限制为：%s\n玩家限制为：%s"), AddComma(m_lApplyBankerCondition), AddComma(m_lAreaLimitScore), AddComma(m_lUserLimitScore));
			
			//发送消息
			CMD_S_TipInfo TipInfo;
			TipInfo.cbTimeLeave = 10;
			_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("%s"), szTipMsg);
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
						
			//发送申请者
			SendApplyUser(pIServerUserItem);
 
			return bSuccess;
		}
	case GAME_SCENE_BET:		//游戏状态
	case GAME_SCENE_END:		//结束状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);		

			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL) - m_dwBetTime;
			int	nTotalTime = ( (cbGameStatus == GAME_SCENE_BET) ? m_cbBetTime : m_cbEndTime);
			StatusPlay.cbTimeLeave = (BYTE)(nTotalTime - __min(dwPassTime, (DWORD)nTotalTime));
			StatusPlay.cbGameStatus = m_pITableFrame->GetGameStatus();		

			//全局下注
			memcpy(StatusPlay.lAllBet, m_lAllBet, sizeof(StatusPlay.lAllBet));
			StatusPlay.lPlayFreeSocre = (LONGLONG)pIServerUserItem->GetUserScore();

			//玩家下注
			if (pIServerUserItem->GetUserStatus() != US_LOOKON && bSendSecret)
			{
				memcpy(StatusPlay.lPlayBet, m_lPlayBet[wChiarID], sizeof(StatusPlay.lPlayBet));
				memcpy(StatusPlay.lPlayScore, m_lPlayScore[wChiarID], sizeof(StatusPlay.lPlayScore));

				//最大下注
				StatusPlay.lPlayBetScore = min((LONGLONG)pIServerUserItem->GetUserScore() , m_lUserLimitScore);
			}

			//庄家信息
			StatusPlay.wBankerUser = m_wCurrentBanker;			
			StatusPlay.wBankerTime = m_wBankerTime;
			StatusPlay.lBankerWinScore = m_lBankerWinScore;	
			StatusPlay.lBankerScore = m_lBankerScore;
			StatusPlay.wFaceID = m_wFaceID;

			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				wcscpy(StatusPlay.szBankerNickName,pIServerUserItem->GetNickName());
			}

			//是否系统坐庄
			StatusPlay.bEnableSysBanker = m_bEnableSysBanker;

			//控制信息
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//结束判断
			if (cbGameStatus == GAME_SCENE_END)
			{
				//扑克信息
				CopyMemory(StatusPlay.cbCardCount,m_cbCardCount,sizeof(m_cbCardCount));
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

				//结束分数
				StatusPlay.lPlayAllScore = m_lUserWinScore[wChiarID];
			}

			//房间名称
			CopyMemory(StatusPlay.szGameRoomName, m_szGameRoomName, sizeof(StatusPlay.szGameRoomName));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n本房间上庄条件为：%s\n区域限制为：%s\n玩家限制为：%s"), AddComma(m_lApplyBankerCondition), AddComma(m_lAreaLimitScore), AddComma(m_lUserLimitScore));

			//发送消息
			CMD_S_TipInfo TipInfo;
			TipInfo.cbTimeLeave = 10;
			_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("%s"), szTipMsg);
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));

			
			//发送申请者
			SendApplyUser( pIServerUserItem );

			return bSuccess;
		}
	}

	return false;
}

//定时器事件
bool  CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_FREE:		//空闲时间
		{
			//开始游戏
			m_pITableFrame->StartGame();

			//设置时间
			m_dwBetTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//设置状态
			m_pITableFrame->SetGameStatus(GAME_SCENE_BET);

			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_END)
			{

				//设置状态
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);			

				//结束游戏
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_dwBetTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			m_GameRecord+=TEXT("END:#");
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			for(WORD i = 0;i<2;i++)
			{
				if(i == INDEX_PLAYER)
					m_GameRecord+=TEXT("闲,Card:");
				else
					m_GameRecord+=TEXT("庄,Card:");
				
				for(WORD j = 0;j<m_cbCardCount[i];j++)
					m_GameRecord.AppendFormat(TEXT("%s,"),TransformCardInfo(m_cbTableCardArray[i][j]));

				m_GameRecord+=TEXT("#");
			}
			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;


				//写入积分
				if (m_lUserWinScore[wUserChairID] >0.001||m_lUserWinScore[wUserChairID]<-0.001) 
				{
					ScoreInfoArray[wUserChairID].lRevenue=m_lUserRevenue[wUserChairID];
					ScoreInfoArray[wUserChairID].lScore=m_lUserWinScore[wUserChairID];

					if ( m_lUserWinScore[wUserChairID] > 0.0 )
						ScoreInfoArray[wUserChairID].cbType=SCORE_TYPE_WIN;
					else if ( m_lUserWinScore[wUserChairID] < 0.0 )
						ScoreInfoArray[wUserChairID].cbType=SCORE_TYPE_LOSE;
					else
						ScoreInfoArray[wUserChairID].cbType=SCORE_TYPE_DRAW;
					//游戏记录
					m_GameRecord.AppendFormat(TEXT("%s得分:%0.2f,%0.2f#"),m_pITableFrame->GetTableUserItem(wUserChairID)->GetNickName(),ScoreInfoArray[wUserChairID].lScore, ScoreInfoArray[wUserChairID].lRevenue);
				}
				if(!pIServerUserItem->IsAndroidUser())
				{
					CString strLog;
					strLog.Format(L"OXBATTLE lScore:%.2lf,revenue:%.2lf",ScoreInfoArray[wUserChairID].lScore,ScoreInfoArray[wUserChairID].lRevenue);
					CTraceService::TraceString(strLog,TraceLevel_Warning);

				}
			}


			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//结束游戏
			//m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,m_GameRecord,608);
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);

			//读取配置
			if (m_bRefreshCfg)
				ReadConfigInformation(false);

			//切换庄家
			ChangeBanker(false);

			//设置时间
			m_dwBetTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//发送消息
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_cbFreeTime;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			return true;
		}
	}

	return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//用户下注
		{
			//效验数据
			ASSERT(wDataSize == sizeof(CMD_C_PlaceBet));
			if (wDataSize!=sizeof(CMD_C_PlaceBet)) return false;

			//用户效验
			if ( pIServerUserItem->GetUserStatus() != US_PLAYING ) return true;

			//消息处理
			CMD_C_PlaceBet * pPlaceBet = (CMD_C_PlaceBet *)pData;
			return OnUserPlayBet(pIServerUserItem->GetChairID(), pPlaceBet->cbBetArea, pPlaceBet->lBetScore);
		}
	case SUB_C_APPLY_BANKER:		//申请做庄
		{
			//用户效验
			if ( pIServerUserItem->GetUserStatus() == US_LOOKON ) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			//用户效验
			if ( pIServerUserItem->GetUserStatus() == US_LOOKON ) return true;
			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize == sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;

			if ( m_pServerContro == NULL)
				return false;

			return m_pServerContro->ServerControl(wSubCmdID, pData, wDataSize, pIServerUserItem, m_pITableFrame);
		}
	case SUB_C_CANCEL_JETTON:
		{
			//用户效验
			if ( pIServerUserItem->GetUserStatus() != US_PLAYING ) return true;

			return OnUserCancelBet(pIServerUserItem->GetChairID());
		}
	}

	return false;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	// 聊天消息
	//if (wSubCmdID == SUB_GF_USER_CHAT && CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
	//{
	//	//变量定义
	//	CMD_GF_C_UserChat * pUserChat=(CMD_GF_C_UserChat *)pData;

	//	//效验参数
	//	ASSERT(wDataSize<=sizeof(CMD_GF_C_UserChat));
	//	ASSERT(wDataSize>=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)));
	//	ASSERT(wDataSize==(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//	//效验参数
	//	if (wDataSize>sizeof(CMD_GF_C_UserChat)) return false;
	//	if (wDataSize<(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString))) return false;
	//	if (wDataSize!=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//	bool bKeyProcess = false;
	//	CString strChatString(pUserChat->szChatString);
	//	CString strKey[KEY_MAX] = { TEXT("/stock"), TEXT("/immortal count"), TEXT("/robot count"), TEXT("/immortal bet"), TEXT("/robot bet") };
	//	CString strName[KEY_MAX] = { TEXT("库存"), TEXT("真人数量"), TEXT("机器人数量"), TEXT("真人下注"), TEXT("机器人下注") };
	//	if ( strChatString == TEXT("/help") )
	//	{
	//		bKeyProcess = true;
	//		CString strMsg;
	//		for ( int i = 0 ; i < KEY_MAX; ++i)
	//		{
	//			strMsg += TEXT("\n");
	//			strMsg += strKey[i];
	//			strMsg += TEXT(" 查看");
	//			strMsg += strName[i];
	//		}
	//		m_pITableFrame->SendGameMessage(pIServerUserItem, strMsg, SMT_EJECT);	
	//	}
	//	else 
	//	{
	//		CString strMsg;
	//		for ( int i = 0 ; i < KEY_MAX; ++i)
	//		{
	//			if ( strChatString == strKey[i] )
	//			{
	//				bKeyProcess = true;
	//				switch(i)
	//				{
	//				case KEY_STOCK:
	//					{
	//						strMsg.Format(TEXT("库存剩余量：%I64d"), m_StorageStart);
	//					}
	//					break;
	//				case KEY_IMMORTAL_COUNT:
	//					{
	//						int nImmortal = 0;
	//						for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	//						{
	//							//获取用户
	//							IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	//							if (pIServerUserItem == NULL) continue;

	//							if ( !pIServerUserItem->IsAndroidUser() )
	//								nImmortal += 1;
	//						}
	//						strMsg.Format(TEXT("真人数量：%d"), nImmortal);
	//					}
	//					break;
	//				case KEY_ROBOT_COUNT:
	//					{
	//						int nRobot = 0;
	//						for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	//						{
	//							//获取用户
	//							IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	//							if (pIServerUserItem == NULL) continue;

	//							if ( pIServerUserItem->IsAndroidUser() )
	//								nRobot += 1;
	//						}
	//						strMsg.Format(TEXT("机器人数量：%d"), nRobot);
	//					}
	//					break;
	//				case KEY_IMMORTAL_BET:
	//					{
	//						LONGLONG lBet[AREA_MAX] = {0};
	//						for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	//						{
	//							//获取用户
	//							IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	//							if (pIServerUserItem == NULL) continue;

	//							if ( !pIServerUserItem->IsAndroidUser() )
	//							{	
	//								for ( int nArea = 0; nArea < AREA_MAX; ++nArea )
	//								{
	//									lBet[nArea] += m_lPlayBet[wChairID][nArea];
	//								}
	//							}
	//						}

	//						strMsg.Format(TEXT("玩家下注：\n 闲：%I64d \n 平：%I64d \n 庄：%I64d \n 闲天王：%I64d \n 庄天王：%I64d \n 同点平：%I64d \n 闲对子：%I64d \n 庄对子：%I64d"), 
	//							lBet[AREA_XIAN], lBet[AREA_PING], lBet[AREA_ZHUANG], lBet[AREA_XIAN_TIAN], lBet[AREA_ZHUANG_TIAN], lBet[AREA_TONG_DUI], lBet[AREA_XIAN_DUI], lBet[AREA_ZHUANG_DUI] );
	//					}
	//					break;
	//				case KEY_ROBOT_BET:
	//					{
	//						LONGLONG lBet[AREA_MAX] = {0};
	//						for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	//						{
	//							//获取用户
	//							IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	//							if (pIServerUserItem == NULL) continue;

	//							if ( pIServerUserItem->IsAndroidUser() )
	//							{	
	//								for ( int nArea = 0; nArea < AREA_MAX; ++nArea )
	//								{
	//									lBet[nArea] += m_lPlayBet[wChairID][nArea];
	//								}
	//							}
	//						}

	//						strMsg.Format(TEXT("机器人下注：\n 闲：%I64d \n 平：%I64d \n 庄：%I64d \n 闲天王：%I64d \n 庄天王：%I64d \n 同点平：%I64d \n 闲对子：%I64d \n 庄对子：%I64d"), 
	//							lBet[AREA_XIAN], lBet[AREA_PING], lBet[AREA_ZHUANG], lBet[AREA_XIAN_TIAN], lBet[AREA_ZHUANG_TIAN], lBet[AREA_TONG_DUI], lBet[AREA_XIAN_DUI], lBet[AREA_ZHUANG_DUI] );
	//					}
	//					break;
	//				}
	//				m_pITableFrame->SendGameMessage(pIServerUserItem, strMsg, SMT_EJECT);	
	//				break;
	//			}
	//		}
	//	}
	//	return bKeyProcess;
	//}
	return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//设置时间
	if ((bLookonUser == false)&&(m_dwBetTime == 0L))
	{
		m_dwBetTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	}

	return true;
}

//用户起来
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser == false)
	{
		//切换庄家
		if (wChairID == m_wCurrentBanker) ChangeBanker(true);

		//取消申请
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//取消下注
bool CTableFrameSink::OnUserCancelBet(WORD wChairID)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_BET)
	{
		return true;
	}
	
	LONGLONG lBetCount = 0;
	for(WORD i = 0; i < AREA_MAX; ++i )
	{
		lBetCount += m_lPlayBet[wChairID][i];
	}

	if(lBetCount == 0) return true;

	CMD_S_CancelBet CancelBet;
	ZeroMemory(&CancelBet,sizeof(CancelBet));
	
	CancelBet.wChairID = wChairID;
	CopyMemory(CancelBet.lPlayBet,m_lPlayBet[wChairID],sizeof(m_lPlayBet[wChairID]));
	ZeroMemory(m_lPlayBet[wChairID],sizeof(m_lPlayBet[wChairID]));

		//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CANCEL_JETTON,&CancelBet,sizeof(CancelBet));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CANCEL_JETTON,&CancelBet,sizeof(CancelBet));

	return true;
}

//下注事件
bool CTableFrameSink::OnUserPlayBet(WORD wChairID, BYTE cbBetArea, LONGLONG lBetScore)
{
	//效验参数
	ASSERT((cbBetArea<=AREA_ZHUANG_DUI)&&(lBetScore>0L));
	if ((cbBetArea>AREA_ZHUANG_DUI)||(lBetScore<=0L)) return false;

	//效验状态
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_BET)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}

	//庄家判断
	if (m_wCurrentBanker == wChairID)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}
	if (m_bEnableSysBanker == false && m_wCurrentBanker == INVALID_CHAIR)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}

	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	LONGLONG lUserScore = (LONGLONG)pIServerUserItem->GetUserScore();
	LONGLONG lBetCount = 0;
	for ( int i = 0; i < AREA_MAX; ++i )
	{
		lBetCount += m_lPlayBet[wChairID][i];
	}

	

	//成功标识
	bool bPlaceBetSuccess=true;

	//合法校验
	if (lUserScore < lBetCount + lBetScore)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}
	else
	{
		//机器人验证
		if(pIServerUserItem->IsAndroidUser())
		{
			//数目限制
			bool bHaveChip = false;
			bHaveChip = (lBetCount>0);

			if (!bHaveChip)
			{
				if (m_nChipRobotCount+1 > m_nMaxChipRobot)
				{
					SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
					return true;
				}
				else
					m_nChipRobotCount++;
			}
		}
	}

	if (m_lUserLimitScore < lBetCount + lBetScore)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}


	//合法验证
	if ( GetMaxPlayerScore(cbBetArea, wChairID) >= lBetScore )
	{
		m_lAllBet[cbBetArea] += lBetScore;
		m_lPlayBet[wChairID][cbBetArea]  += lBetScore;
	}
	else
	{
		bPlaceBetSuccess = false;
	}

	if (bPlaceBetSuccess)
	{
		//变量定义
		CMD_S_PlaceBet PlaceBet;
		ZeroMemory(&PlaceBet,sizeof(PlaceBet));

		//构造变量
		PlaceBet.wChairID=wChairID;
		PlaceBet.cbBetArea=cbBetArea;
		PlaceBet.lBetScore=lBetScore;
		PlaceBet.cbAndroidUser=pIServerUserItem->IsAndroidUser()?TRUE:FALSE;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceBet,sizeof(PlaceBet));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceBet,sizeof(PlaceBet));

		CString strArea[8]={L"闲",L"平",L"庄",L"闲天王",L"庄天王",L"同点平",L"闲对子",L"庄对子"};
		for(int i=0; i<MAX_CHAIR; i++)
		{
			IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i);
			if(pUserItem==NULL) continue;
			if(pUserItem->IsAndroidUser()) continue;
			if(pIServerUserItem->IsAndroidUser()) continue;
			if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;
			TCHAR strMessage[256]=TEXT("");
			_sntprintf(strMessage, sizeof(strMessage), _T("%s:[%s]下注%I64d"), pIServerUserItem->GetUserInfo()->szNickName,strArea[cbBetArea],lBetScore);
			m_pITableFrame->SendGameMessage(pUserItem,strMessage,SMT_CHAT);

		}

	}
	else
	{
		//发送消息
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
	}

	return true;
}

//发送消息
void CTableFrameSink::SendPlaceBetFail(WORD wChairID, BYTE cbBetArea, LONGLONG lBetScore)
{
	CMD_S_PlaceBetFail PlaceBetFail;
	ZeroMemory(&PlaceBetFail,sizeof(PlaceBetFail));
	PlaceBetFail.lBetArea=cbBetArea;
	PlaceBetFail.lPlaceScore=lBetScore;
	PlaceBetFail.wPlaceUser=wChairID;

	//发送消息
	m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceBetFail,sizeof(PlaceBetFail));
}

//发送扑克
bool CTableFrameSink::DispatchTableCard()
{
	//随机扑克
	m_GameLogic.RandCardList(m_cbTableCardArray[0],sizeof(m_cbTableCardArray)/sizeof(m_cbTableCardArray[0][0]));

	//首次发牌
	m_cbCardCount[INDEX_PLAYER] = 2;
	m_cbCardCount[INDEX_BANKER] = 2;
	
	//计算点数
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);
	BYTE cbPlayerTwoCardCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);

	//闲家补牌
	BYTE cbPlayerThirdCardValue = 0 ;	//第三张牌点数
	if(cbPlayerTwoCardCount<=5 && cbBankerCount<8)
	{
		//计算点数
		m_cbCardCount[INDEX_PLAYER]++;
		cbPlayerThirdCardValue = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_PLAYER][2]);
	}

	//庄家补牌
	if(cbPlayerTwoCardCount<8 && cbBankerCount<8)
	{
		switch(cbBankerCount)
		{
		case 0:
		case 1:
		case 2:
			m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 3:
			if((m_cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue!=8) || m_cbCardCount[INDEX_PLAYER] == 2) m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 4:
			if((m_cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 && cbPlayerThirdCardValue!=0) || m_cbCardCount[INDEX_PLAYER] == 2) m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 5:
			if((m_cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=2 && cbPlayerThirdCardValue!=3  && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 &&  cbPlayerThirdCardValue!=0) || m_cbCardCount[INDEX_PLAYER] == 2) m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 6:
			if(m_cbCardCount[INDEX_PLAYER] == 3 && (cbPlayerThirdCardValue == 6 || cbPlayerThirdCardValue == 7)) m_cbCardCount[INDEX_BANKER]++ ;
			break;

			//不须补牌
		case 7:
		case 8:
		case 9:
			break;
		default:
			break;
		}
	}

	return true;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	LONGLONG lUserScore = (LONGLONG)pIApplyServerUserItem->GetUserScore();
	if ( lUserScore < m_lApplyBankerCondition )
	{
		CMD_S_TipInfo TipInfo;
		TipInfo.cbTimeLeave = 10;
		_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("你的金币不足以申请庄家，申请失败！"));
		m_pITableFrame->SendUserItemData(pIApplyServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
		return true;
	}

	//存在判断
	WORD wApplyUserChairID = pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID = m_ApplyUserArray[nUserIdx];
		if (wChairID == wApplyUserChairID)
		{
			CMD_S_TipInfo TipInfo;
			TipInfo.cbTimeLeave = 10;
			_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("你已经申请了庄家，不需要再次申请！"));
			m_pITableFrame->SendUserItemData(pIApplyServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
			return true;
		}
	}

	//保存信息 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//构造变量
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//设置变量
	ApplyBanker.wApplyUser = wApplyUserChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE && m_ApplyUserArray.GetCount() == 1)
	{
		ChangeBanker(false);
	}

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
 	if (pICancelServerUserItem->GetChairID() == m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_SCENE_FREE)
	{
		//发送消息
		CMD_S_TipInfo TipInfo;
		TipInfo.cbTimeLeave = 10;
		_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("游戏已经开始，不可以取消当庄！"));
		m_pITableFrame->SendUserItemData(pICancelServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));

		return true;
	}

	//存在判断
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//条件过滤
		if (pIServerUserItem == NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker == wChairID)
		{
			//切换庄家 
			//m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return true;
}

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	bool bSystemBanker = (m_wCurrentBanker == INVALID_CHAIR);
	//切换标识
	bool bChangeBanker=false;

	//取消当前
	if (bCancelCurrentBanker)
	{
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
		TakeTurns();

		//设置变量
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}
	//轮庄判断
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if(pIServerUserItem)
		{
			LONGLONG lBankerScore=(LONGLONG)pIServerUserItem->GetUserScore();

			//次数判断
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//庄家增加判断 同一个庄家情况下只判断一次
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker && lBankerScore>=m_lApplyBankerCondition)
				{
					//加庄局数设置：当庄家坐满设定的局数之后(m_lBankerMAX)，
					//所带金币值还超过下面申请庄家列表里面所有玩家金币时，
					//可以再加坐庄m_lBankerAdd局，加庄局数可设置。

					//金币超过m_lBankerScoreMAX之后，
					//就算是下面玩家的金币值大于他的金币值，他也可以再加庄m_lBankerScoreAdd局。
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//获取玩家
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						LONGLONG lScore = (LONGLONG)pIServerUserItem->GetUserScore();

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l) )
					{
						if ( bScoreMAX )
						{
							m_lPlayerBankerMAX += m_lBankerAdd;
						}
						if ( lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l )
						{
							m_lPlayerBankerMAX += m_lBankerScoreAdd;
						}
						return true;
					}
				}

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
				TakeTurns();

				bChangeBanker=true;
				m_bExchangeBanker = true;

				//提示消息
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("您分数少于(%I64d)，强行换庄!"),m_lApplyBankerCondition);
				else
            		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("您做庄次数达到(%d)，强行换庄!"), m_lPlayerBankerMAX);
				
				//发送消息
				CMD_S_TipInfo TipInfo;
				TipInfo.cbTimeLeave = 10;
				_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("%s"), szTipMsg);
				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));

			}
		}
	}
	//系统做庄
	else if (m_wCurrentBanker == INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//轮换判断
		TakeTurns();

		bChangeBanker=true;
		m_bExchangeBanker = true;
	}

	//切换判断
	if (bChangeBanker)
	{
		//最大坐庄数
		m_lPlayerBankerMAX = m_lBankerMAX;
		if(bSystemBanker == false && m_wCurrentBanker == INVALID_CHAIR)
		{
			m_wFaceID = (m_wFaceID+1)%2;
		}
		//设置变量
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//发送消息
		CMD_S_ChangeBanker stChangeBanker;
		ZeroMemory(&stChangeBanker,sizeof(stChangeBanker));
		stChangeBanker.wBankerUser = m_wCurrentBanker;
		stChangeBanker.wFaceID=m_wFaceID;
		if (m_wCurrentBanker != INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			stChangeBanker.lBankerScore = (LONGLONG)pIServerUserItem->GetUserScore();
			wcscpy(stChangeBanker.szBankerNickName,pIServerUserItem->GetNickName());
		}
		else
		{
			stChangeBanker.lBankerScore = 100000000;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&stChangeBanker,sizeof(CMD_S_ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&stChangeBanker,sizeof(CMD_S_ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//读取消息
			LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//读取配置
				LONGLONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
				GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//获取玩家
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				//发送消息
				CString strNick;
				strNick = pIServerUserItem->GetNickName();
				strNick = strNick.Left(4);
				strNick+=_T("****");

				//发送消息
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,strNick);
				//SendGameMessage(INVALID_CHAIR,szMessage2);
			}
		}
	}

	return bChangeBanker;
}

//轮换判断
void CTableFrameSink::TakeTurns()
{
	//变量定义
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
		{
			//获取分数
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker=m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//发送消息
					CMD_S_CancelBanker CancelBanker = {};

					//设置变量
					CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

					//发送消息
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

					//提示消息
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数（%I64d）少于坐庄必须金币数（%I64d）,你无法上庄！"),(LONGLONG)pIServerUserItem->GetUserScore(), m_lApplyBankerCondition);

					//发送消息
					CMD_S_TipInfo TipInfo;
					TipInfo.cbTimeLeave = 10;
					_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("%s"), szTipMsg);
					m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
				}
			}
		}
	}

	//删除玩家
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
}

//发送庄家
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (!pServerUserItem) continue;

		//庄家判断
		if (pServerUserItem->GetChairID() == m_wCurrentBanker) continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//发送消息
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine( WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//切换庄家
	if (wChairID == m_wCurrentBanker) ChangeBanker(true);

	//取消申请
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//设置变量
		CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//最大下注
LONGLONG CTableFrameSink::GetMaxPlayerScore( BYTE cbBetArea, WORD wChairID )
{
	//获取玩家
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	if ( NULL == pIMeServerUserItem ) 
		return 0L;

	if ( cbBetArea >= AREA_MAX )
		return 0L;

	//已下注额
	LONGLONG lNowBet = 0l;
	for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex ) 
		lNowBet += m_lPlayBet[wChairID][nAreaIndex];

	//庄家金币
	LONGLONG lBankerScore = m_lBankerScore;

	//区域倍率
	BYTE cbMultiple[AREA_MAX] = {MULTIPLE_XIAN, MULTIPLE_PING, MULTIPLE_ZHUANG, 
									MULTIPLE_XIAN_TIAN, MULTIPLE_ZHUANG_TIAN, MULTIPLE_TONG_DIAN, 
									MULTIPLE_XIAN_PING, MULTIPLE_ZHUANG_PING};

	//区域输赢
	BYTE cbArae[4][4] = {	{ AREA_XIAN_DUI,	255,			AREA_MAX,			AREA_MAX }, 
							{ AREA_ZHUANG_DUI,	255,			AREA_MAX,			AREA_MAX }, 
							{ AREA_XIAN,		AREA_PING,		AREA_ZHUANG,		AREA_MAX },  
							{ AREA_XIAN_TIAN,	AREA_TONG_DUI,	AREA_ZHUANG_TIAN,	255 }};

	//筹码设定
	for ( int nTopL = 0; nTopL < 4; ++nTopL )
	{
		if( cbArae[0][nTopL] == AREA_MAX )
			continue;

		for ( int nTopR = 0; nTopR < 4; ++nTopR )
		{
			if( cbArae[1][nTopR] == AREA_MAX )
				continue;

			for ( int nCentral = 0; nCentral < 4; ++nCentral )
			{
				if( cbArae[2][nCentral] == AREA_MAX )
					continue;

				for ( int nBottom = 0; nBottom < 4; ++nBottom )
				{
					if( cbArae[3][nBottom] == AREA_MAX )
						continue;

					BYTE cbWinArea[AREA_MAX] = {FALSE};

					//指定获胜区域
					if ( cbArae[0][nTopL] != 255 )
						cbWinArea[cbArae[0][nTopL]] = TRUE;

					if ( cbArae[1][nTopR] != 255 )
						cbWinArea[cbArae[1][nTopR]] = TRUE;

					if ( cbArae[2][nCentral] != 255 )
						cbWinArea[cbArae[2][nCentral]] = TRUE;

					if ( cbArae[3][nBottom] != 255 )
						cbWinArea[cbArae[3][nBottom]] = TRUE;

					//选择区域为玩家胜利，同等级的其他的区域为玩家输。以得出最大下注值
					for ( int i = 0; i < 4; i++ )
					{
						for ( int j = 0; j < 4; j++ )
						{
							if ( cbArae[i][j] == cbBetArea )
							{
								for ( int n = 0; n < 4; ++n )
								{
									if ( cbArae[i][n] != 255 && cbArae[i][n] != AREA_MAX )
									{
										cbWinArea[cbArae[i][n]] = FALSE;
									}
								}
								cbWinArea[cbArae[i][j]] = TRUE;
							}
						}
					}

					LONGLONG lScore = m_lBankerScore;
					for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex ) 
					{
						if ( cbWinArea[nAreaIndex] == TRUE )
						{
							lScore -= m_lAllBet[nAreaIndex]*(cbMultiple[nAreaIndex] - 1);
						}
						else if ( cbWinArea[AREA_PING] == TRUE && ( nAreaIndex == AREA_XIAN || nAreaIndex == AREA_ZHUANG ) )
						{

						}
						else
						{
							lScore += m_lAllBet[nAreaIndex];
						}
					}
					if ( lBankerScore == -1 )
						lBankerScore = lScore;
					else
						lBankerScore = min(lBankerScore, lScore);
				}
			}
		}
	}

	//最大下注
	LONGLONG lMaxBet = 0L;

	//最大下注
	lMaxBet = min((LONGLONG)pIMeServerUserItem->GetUserScore() - lNowBet, m_lUserLimitScore - m_lPlayBet[wChairID][cbBetArea]);

	lMaxBet = min( lMaxBet, m_lAreaLimitScore - m_lAllBet[cbBetArea]);

	lMaxBet = min( lMaxBet, lBankerScore / (cbMultiple[cbBetArea] - 1));

	//非零限制
	ASSERT(lMaxBet >= 0);
	lMaxBet = max(lMaxBet, 0);

	//庄闲对等
	if ( cbBetArea == AREA_XIAN && (m_lAllBet[AREA_ZHUANG] - m_lAllBet[AREA_XIAN]) && lMaxBet < (m_lAllBet[AREA_ZHUANG] - m_lAllBet[AREA_XIAN]) )
		lMaxBet = m_lAllBet[AREA_ZHUANG] - m_lAllBet[AREA_XIAN];
	else if( cbBetArea == AREA_ZHUANG && (m_lAllBet[AREA_XIAN] - m_lAllBet[AREA_ZHUANG]) && lMaxBet < (m_lAllBet[AREA_XIAN] - m_lAllBet[AREA_ZHUANG]) )
		lMaxBet = m_lAllBet[AREA_XIAN] - m_lAllBet[AREA_ZHUANG];

	return lMaxBet;
}


//计算得分
bool CTableFrameSink::CalculateScore( OUT LONGLONG& lBankerWinScore, OUT tagServerGameRecord& GameRecord )
{
	bool bControl = false;
	if ( m_pServerContro && m_pServerContro->NeedControl() && m_pServerContro->ControlResult(m_cbTableCardArray[0], m_cbCardCount))
	{
		bControl = true;
	}

	//计算牌点
	BYTE cbPlayerCount = m_GameLogic.GetCardListPip( m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER] );
	BYTE cbBankerCount = m_GameLogic.GetCardListPip( m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER] );

	//系统输赢
	LONGLONG lSystemScore = 0l;

	//推断玩家
	BYTE cbWinArea[AREA_MAX] = {FALSE};
	DeduceWinner(cbWinArea);

	//游戏记录
	GameRecord.cbBankerCount = cbBankerCount;
	GameRecord.cbPlayerCount = cbPlayerCount;
	GameRecord.bPlayerTwoPair = cbWinArea[AREA_XIAN_DUI] == TRUE;
	GameRecord.bBankerTwoPair = cbWinArea[AREA_ZHUANG_DUI] == TRUE;
	if ( cbWinArea[AREA_TONG_DUI] == TRUE )
		GameRecord.cbKingWinner = AREA_TONG_DUI;
	else if ( cbWinArea[AREA_XIAN_TIAN] == TRUE )
		GameRecord.cbKingWinner = AREA_XIAN_TIAN;
	else if ( cbWinArea[AREA_ZHUANG_TIAN] == TRUE )
		GameRecord.cbKingWinner = AREA_ZHUANG_TIAN;


	//玩家成绩
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//区域倍率
	BYTE cbMultiple[AREA_MAX] = {MULTIPLE_XIAN, MULTIPLE_PING, MULTIPLE_ZHUANG, 
									MULTIPLE_XIAN_TIAN, MULTIPLE_ZHUANG_TIAN, MULTIPLE_TONG_DIAN, 
									MULTIPLE_XIAN_PING, MULTIPLE_ZHUANG_PING };

	//庄家是不是机器人
	bool bIsBankerAndroidUser = false;
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem != NULL) 
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//计算积分
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if ( m_wCurrentBanker == wChairID ) continue;

		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for ( WORD wAreaIndex = 0; wAreaIndex < AREA_MAX; ++wAreaIndex )
		{
			if ( cbWinArea[wAreaIndex] == TRUE )
			{
				m_lUserWinScore[wChairID] += (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
				m_lPlayScore[wChairID][wAreaIndex] += (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
				lBankerWinScore -= (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));

				//系统得分
				if(bIsAndroidUser)
					lSystemScore += (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));

				//庄家得分
				if ( m_wCurrentBanker != INVALID_CHAIR && m_wCurrentBanker != wChairID )
					m_lPlayScore[m_wCurrentBanker][wAreaIndex] -= (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
			}
			else if ( cbWinArea[AREA_PING] == TRUE && ( wAreaIndex == AREA_XIAN || wAreaIndex == AREA_ZHUANG ) )
			{

			}
			else
			{
				lUserLostScore[wChairID] -= m_lPlayBet[wChairID][wAreaIndex];
				m_lPlayScore[wChairID][wAreaIndex] -= m_lPlayBet[wChairID][wAreaIndex];
				lBankerWinScore += m_lPlayBet[wChairID][wAreaIndex];

				//系统得分
				if(bIsAndroidUser)
					lSystemScore -= m_lPlayBet[wChairID][wAreaIndex];
				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore += m_lPlayBet[wChairID][wAreaIndex];

				//庄家得分
				if ( m_wCurrentBanker != INVALID_CHAIR && m_wCurrentBanker != wChairID )
					m_lPlayScore[m_wCurrentBanker][wAreaIndex] += m_lPlayBet[wChairID][wAreaIndex];
			}

		}

		//计算税收
		if ( m_lUserWinScore[wChairID] > 0.0 )
		{
			m_lUserRevenue[wChairID]  = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}

		//总的分数
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
	}

	//庄家成绩
	if (m_wCurrentBanker != INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = (SCORE)lBankerWinScore;

		//计算税收
		if (0.0 < m_lUserWinScore[m_wCurrentBanker])
		{
			m_lUserRevenue[m_wCurrentBanker]  = m_pITableFrame->CalculateRevenue(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker]);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = (LONGLONG)m_lUserWinScore[m_wCurrentBanker];
		}		
	}

	// 控制中不保存在库存里
	if ( bControl )
	{
		return true; 
	}



	//系统分值计算
	if ((lSystemScore + m_StorageStart) < 0l)
	{
		return false;
	}
	else
	{

		//系统输赢区间
		if (lSystemScore + m_StorageStart<m_StorageStart*m_nWinMinPercent/100&&lSystemScore + m_StorageStart>m_StorageStart*m_nWinMaxPercent/100)
		{
			return false;
		}
		m_StorageStart += lSystemScore;
		m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
		return true;
	}
}

//游戏结束计算
LONGLONG CTableFrameSink::GameOver()
{
	//定义变量
	LONGLONG lBankerWinScore = 0l;
	bool bSuccess = false;

	CString szTemp;
	bool bRecord;
	LONGLONG lAllScore;
	for(WORD i = 0;i<GAME_PLAYER;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL) continue;

		bRecord = false;
		szTemp.Empty();
		lAllScore = 0;
		
		szTemp.AppendFormat(TEXT("P%d,"),i);
		
		for(BYTE j =0;j<AREA_MAX;j++)
		{
			if(m_lPlayBet[i][j]>0)
			{
				bRecord=true;
				szTemp.AppendFormat(TEXT("%s,%I64d,"),TransformAreaInfo(j),m_lPlayBet[i][j]);
				lAllScore+=m_lPlayBet[i][j];
			}
		}

		if(bRecord)
		{
			szTemp.AppendFormat(TEXT("(总:%I64d)"),lAllScore);
			szTemp+=TEXT("#");
			m_GameRecord+=szTemp;
		}
	}
	//游戏记录
	tagServerGameRecord& GameRecord = m_GameRecordArrary[m_nRecordLast];
	
	//计算分数
	do 
	{
		//派发扑克
		DispatchTableCard();

		//试探性判断
		lBankerWinScore = 0l;
		bSuccess = CalculateScore( lBankerWinScore, GameRecord );

	} while (!bSuccess);

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore = lBankerWinScore;

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	return lBankerWinScore;
}

//推断赢家
void CTableFrameSink::DeduceWinner(BYTE* pWinArea)
{
	//计算牌点
	BYTE cbPlayerCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//胜利区域--------------------------
	//平
	if( cbPlayerCount == cbBankerCount )
	{
		pWinArea[AREA_PING] = TRUE;

		// 同平点
		if ( m_cbCardCount[INDEX_PLAYER] == m_cbCardCount[INDEX_BANKER] )
		{
			WORD wCardIndex =0;
			for (wCardIndex = 0; wCardIndex < m_cbCardCount[INDEX_PLAYER]; ++wCardIndex )
			{
				BYTE cbBankerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][wCardIndex]);
				BYTE cbPlayerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][wCardIndex]);
				if ( cbBankerValue != cbPlayerValue ) break;
			}

			if ( wCardIndex == m_cbCardCount[INDEX_PLAYER] )
			{
				pWinArea[AREA_TONG_DUI] = TRUE;
			}
		}
	}
	// 庄
	else if ( cbPlayerCount < cbBankerCount)  
	{
		pWinArea[AREA_ZHUANG] = TRUE;

		//天王判断
		if ( cbBankerCount == 8 || cbBankerCount == 9 )
		{
			pWinArea[AREA_ZHUANG_TIAN] = TRUE;
		}
	}
	// 闲
	else 
	{
		pWinArea[AREA_XIAN] = TRUE;

		//天王判断
		if ( cbPlayerCount == 8 || cbPlayerCount == 9 )
		{
			pWinArea[AREA_XIAN_TIAN] = TRUE;
		}
	}
	

	//对子判断
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][0]) == m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][1]))
	{
		pWinArea[AREA_XIAN_DUI] = TRUE;
	}
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][0]) == m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][1]))
	{
		pWinArea[AREA_ZHUANG_DUI] = TRUE;
	}
}


//发送记录
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
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
	if (wBufferSize>0) m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//读取配置
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	//获取自定义配置
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomConfig);

	TCHAR szMaxScore[255] = {0};

	//每盘刷新
	if (bReadFresh)
	{
		//每盘刷新
		BYTE cbRefreshCfg = GetPrivateProfileInt(m_szGameRoomName, TEXT("Refresh"), 0, m_szConfigFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;
	}


	m_lApplyBankerCondition = pCustomConfig->lApplyBankerCondition;
	m_lAreaLimitScore = pCustomConfig->lAreaLimitScore;
	m_lUserLimitScore = pCustomConfig->lUserLimitScore;
	m_bEnableSysBanker = (pCustomConfig->nEnableSysBanker == TRUE);

	m_StorageStart = pCustomConfig->StorageStart;

	
	m_StorageDeduct = pCustomConfig->StorageDeduct;

	m_lBankerMAX = pCustomConfig->lBankerMAX;
	m_lBankerAdd = pCustomConfig->lBankerAdd;
	m_lBankerScoreMAX = pCustomConfig->lBankerScoreMAX;
	m_lBankerScoreAdd = pCustomConfig->lBankerScoreAdd;

	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	m_nWinMaxPercent = GetPrivateProfileInt(m_szGameRoomName, TEXT("WinMaxPercent"), 80, m_szConfigFileName);
	m_nWinMinPercent = GetPrivateProfileInt(m_szGameRoomName, TEXT("WinMinPercent"), 20, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;

	if ( m_lBankerScoreMAX <= m_lApplyBankerCondition)
		m_lBankerScoreMAX = 0l;

	m_lPlayerBankerMAX = m_lBankerMAX;

	//时间控制
	m_cbFreeTime = pCustomConfig->cbFreeTime;
	m_cbBetTime = pCustomConfig->cbBetTime;
	m_cbEndTime = pCustomConfig->cbEndTime;

	if( m_cbEndTime < TIME_GAME_END )
		m_cbEndTime = TIME_GAME_END;
}

// 添加逗号
CString CTableFrameSink::AddComma( LONGLONG lScore )
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%3) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}

	if ( lScore < 0 )
		strReturn.Insert(0, '-');

	return strReturn;
}
//////////////////////////////////////////////////////////////////////////

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
	////如果不具有管理员权限 则返回错误
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
		return false;
	//
	return true;
}

//咨询玩家
bool CTableFrameSink::OnEventQueryChargeable( IServerUserItem *pIServerUserItem, bool bLookonUser )
{
	//if ( bLookonUser )
	//	return false;

	return false;
}

//咨询服务费
LONGLONG CTableFrameSink::OnEventQueryCharge( IServerUserItem *pIServerUserItem )
{

	return 0;
}	

//牌转文字
CString CTableFrameSink::TransformCardInfo( BYTE cbCardData ) 
{
	CString str = TEXT("");

	BYTE cbCardValue=cbCardData&LOGIC_MASK_VALUE;
	switch(cbCardValue)
	{
		case 0x01:
		{
			str += TEXT("A");
			break;
		}
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
			{
				str.Format( TEXT("%d"),cbCardValue);
				break;
			}
		case 0x0A:
			{
				str += TEXT("10");
				break;
			}
		case 0x0B:
			{
				str += TEXT("J");
				break;
			}
		case 0x0C:
			{
				str += TEXT("Q");
				break;
			}
		case 0x0D:
			{
				str += TEXT("K");
				break;
			}
		default:
			ASSERT(FALSE);
	}
	
	BYTE cbCardColor = (cbCardData&LOGIC_MASK_COLOR);
	switch( cbCardColor )
	{
	case 0x00:
		str += TEXT("方块");
		break;
	case 0x10:
		str += TEXT("梅花");
		break;
	case 0x20:
		str += TEXT("红桃");
		break;
	case 0x30:
		str += TEXT("黑桃");
		break;
	default:
		ASSERT(FALSE);
	}

	return str;
}

//区域文字
CString CTableFrameSink::TransformAreaInfo( BYTE cbArea )
{
	CString str = TEXT("");

	switch(cbArea)
	{
		case AREA_XIAN:
			str+= TEXT("闲");
			break;
		case AREA_PING:
			str+= TEXT("平");
			break;
		case AREA_ZHUANG:
			str+= TEXT("庄");
			break;
		case AREA_XIAN_TIAN:
			str+= TEXT("闲天王");
			break;
		case AREA_ZHUANG_TIAN:
			str+= TEXT("庄天王");
			break;
		case AREA_TONG_DUI:
			str+= TEXT("同点平");
			break;
		case AREA_XIAN_DUI:
			str+= TEXT("闲对子");
			break;
		case AREA_ZHUANG_DUI:
			str+= TEXT("庄对子");
			break;
		default:
			ASSERT(FALSE);
	}

	return str;
}