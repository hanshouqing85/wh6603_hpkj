#include "StdAfx.h"
#include "TableFrameSink.h"

SCORE			CTableFrameSink::m_lStockScore = 0L;
LONGLONG		CTableFrameSink::m_lStorageOff = 0L;
//////////////////////////////////////////////////////////////////////////

void TraceMessage(CString szinfo,WORD id);
//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
		
	m_GameRecord.Empty();
	ZeroMemory(m_szNickName,sizeof(m_szNickName));
	//游戏变量
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;

	//下注信息
	m_lDrawMaxScore=SCORE_ZERO;
	m_lTurnMaxScore=SCORE_ZERO;
	m_lTurnLessScore=SCORE_ZERO;
	m_lDrawCellScore=SCORE_ZERO;
	//控制信息
	m_dwCheatCount=0;
	m_dwCheatGameID=0;
	m_cbCheatType=CHEAT_TYPE_WIN;

	//用户状态
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));

	//金币信息
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//扑克变量
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	m_bWinnerCtrl=false;
	srand( (unsigned)time(NULL));
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink()
{
#ifdef DEBUG
	if(!m_GameRecord.IsEmpty())
		TraceMessage(m_GameRecord,m_pITableFrame->GetTableID());
#endif
}

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//配置桌子
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//查询配置
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);


	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//读取配置
	//TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\HKFiveCardConfig.ini"),szPath);
	//开始存储数目
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);
	//读取库存
	m_lStockScore=GetPrivateProfileInt(szServerName,TEXT("StorageStart"),0,m_szFileName);
	//读取衰减值
	m_lStorageDeduct=GetPrivateProfileInt(szServerName,TEXT("StorageDeduct"),10,m_szFileName);


	//设置最大衰减值
	if( m_lStorageDeduct < 0 || m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 0;
	if ( m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 1000;



	//梦成网络添加
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
		if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);
	//梦成网络添加

	//AI变量
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));///////////
	ZeroMemory(m_lScoreLimit,sizeof(m_lScoreLimit));///////////

	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_bNeedWin[i]=false;////////////
		m_bAndroid[i]=false;//////////
	}

	//m_bCtrlThisTime=false;
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl = false;
	
	
	//读取配置
	ReadConfigInformation(true);


	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{	
	m_GameRecord.Empty();
	//游戏变量
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;

	//下注信息
	m_lDrawMaxScore=SCORE_ZERO;
	m_lTurnMaxScore=SCORE_ZERO;
	m_lTurnLessScore=SCORE_ZERO;
	m_lDrawCellScore=SCORE_ZERO;

	//用户状态
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));

	//金币信息
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//扑克变量
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	m_bWinnerCtrl=false;
	return;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{

	ReadConfigInformation(true);

	//设置状态
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	ZeroMemory(m_szNickName,sizeof(m_szNickName));

	//派发扑克
	m_cbSendCardCount=2;
	m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem * pUser=m_pITableFrame->GetTableUserItem(i);
		if(pUser==NULL)continue;

		m_cbPlayStatus[i]=TRUE;
	}

	

	//查找特殊用户
	WORD wChairIDCtrl=INVALID_CHAIR;
	//最大用户
	WORD wWinChairID = EstimateWinnerEx(0,4);
	//最小用户
	WORD wLoseChairID=EstimateLoser(0,4);

	
	//控制次数大于0
	if(m_dwCheatCount>0)
	{
		m_bWinnerCtrl = true;

		if(m_dwCheatGameID==0)
		{
			m_bWinnerCtrl=false;
		}
		else
		{
			wChairIDCtrl = GetChairIdByGameId(m_dwCheatGameID);

			//指定玩家不在
			if( wChairIDCtrl==INVALID_CHAIR)
			{
				m_bWinnerCtrl=false;
			}
		}
	}

	if (m_bWinnerCtrl)
	{
		if(m_cbCheatType==1)
		{
			ProbeJudge(wChairIDCtrl,m_cbCheatType);
		}
		else
		{
			ProbeJudge(wChairIDCtrl,m_cbCheatType);
		
		}
		
		//m_bCtrlThisTime = true;
	}
	else
	{
		//m_bCtrlThisTime = false;
		
		//机器人作弊
		AndroidCheck();
	}
	//最低积分
	m_lDrawCellScore=(m_pGameServiceOption->lCellScore);


	//变量设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//设置变量
		if (pIServerUserItem!=NULL)
		{
			//状态设置
			m_cbPlayStatus[i]=TRUE;
			m_lTableScore[i]=m_lDrawCellScore;
			m_lUserMaxScore[i] = KeepOne(pIServerUserItem->GetUserScore());

			//扑克设置
			m_cbCardCount[i]=m_cbSendCardCount;

			_sntprintf(m_szNickName[i],CountArray(m_szNickName[i]),TEXT("%s"),pIServerUserItem->GetNickName());
		}
	}

	//下注计算
	RectifyMaxScore();

	//最大用户
	wWinChairID = EstimateWinnerEx(0,4);

	//设置变量
	m_wCurrentUser=EstimateWinner(1,1);
	m_lTurnLessScore=m_lUserScore[m_wCurrentUser]+m_lTableScore[m_wCurrentUser];


	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//设置变量
	
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.lDrawMaxScore=m_lDrawMaxScore;
	GameStart.lTurnMaxScore=m_lTurnMaxScore;
	GameStart.lTurnLessScore=m_lTurnLessScore;
	GameStart.lCellScore=m_lDrawCellScore;
	
	CopyMemory(GameStart.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

	CMD_S_GameStart1 GameStart1;
	ZeroMemory(&GameStart1,sizeof(GameStart1));

	//设置变量
	GameStart1.wCurrentUser=m_wCurrentUser;
	GameStart1.lDrawMaxScore=m_lDrawMaxScore;
	GameStart1.lTurnMaxScore=m_lTurnMaxScore;
	GameStart1.lTurnLessScore=m_lTurnLessScore;
	GameStart1.lCellScore=m_lDrawCellScore;

	CopyMemory(GameStart1.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));



	bool bHaveUser = false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;
		if (!pIServerUserItem->IsAndroidUser())
		{
			bHaveUser = true;
			break;
		}
	}
	//设置扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i]==FALSE) GameStart.cbCardData[i]=0;
		else GameStart.cbCardData[i]=m_cbHandCardData[i][1];
	}

	//设置扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i]==FALSE) GameStart1.cbCardData[i]=0;
		else GameStart1.cbCardData[i]=m_cbHandCardData[i][1];
	}
	//发送数据
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//游戏数据

	//	GameStart.bWillWin=false;
		if (m_cbPlayStatus[i]==TRUE)
		{
		
			m_GameRecord.AppendFormat( TEXT("%s-Card:"),m_szNickName[i] );
			m_GameRecord.AppendFormat( TEXT("%s,"),	TransformCardInfo(m_cbHandCardData[i][0]) );
			m_GameRecord.AppendFormat( TEXT("%s#"),	TransformCardInfo(m_cbHandCardData[i][1]) );

			GameStart.cbObscureCard=m_cbHandCardData[i][0];
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			
			if (pIServerUserItem!=NULL && !pIServerUserItem->IsAndroidUser())
				m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}

		//旁观数据
		GameStart.cbObscureCard=(m_cbPlayStatus[i]==TRUE)?0xFF:0x00;
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	
		//发送数据
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//游戏数据
		//游戏数据
		GameStart1.bWillWin=false;

		CopyMemory(GameStart1.cbTableCardData,m_cbHandCardData,sizeof(GameStart1.cbTableCardData));
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart1.cbObscureCard=m_cbHandCardData[i][0];
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem!=NULL && pIServerUserItem->IsAndroidUser())
			{
				GameStart1.bWillWin = (wWinChairID==i);
				m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart1,sizeof(GameStart1));
			}
		}
		
	}
	
	
	//保存名字
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//设置变量
		if (pIServerUserItem!=NULL)
		{
			m_strPalyName[i].Format(TEXT("%s"), pIServerUserItem->GetNickName() );
		}
	}
	//向机器人发送用户牌
//	AndroidSendUserCard();
	return true;
}

void CTableFrameSink::AndroidSendUserCard()
{
	// 发送扑克
	CMD_S_GetAllCard gac={0};
	for(int i = 0 ; i < GAME_PLAYER ; i ++)
	{
		for(int j = 0 ; j < MAX_COUNT ; j ++)
		{
			gac.cbCardData[i][j]=(m_cbPlayStatus[i]==TRUE)?m_cbHandCardData[i][j]:0;
		}
	}

	for(WORD k=0;k<GAME_PLAYER;k++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(k);

		if(pIServerUserItem==NULL)continue;
		// 判断机器人否
		if((CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))==true)
			m_pITableFrame->SendTableData(k,SUB_S_ANDROID_GET_CARD,&gac,sizeof(CMD_S_GetAllCard));
	}
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
	case GER_NO_PLAYER:		//没有玩家
		{
			
			if(cbReason==GER_NORMAL)
				m_GameRecord+=TEXT("END_NORMAL:#");
			else
				m_GameRecord+=TEXT("END_NO_PLAYER:#");
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));



			//变量定义
			WORD wWinerUser=EstimateWinner(0,MAX_COUNT-1);
			//计算总注
			SCORE lDrawScore=SCORE_ZERO;
			for (WORD i=0;i<CountArray(m_lTableScore);i++) 
			{
				if(i == wWinerUser) continue;
					lDrawScore+=Double_Round(m_lTableScore[i],2,1);
			}
			//积分变量
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			// 统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if ( m_cbPlayStatus[i] == TRUE ) 
				{
					//成绩计算
					SCORE lUserScore = (i == wWinerUser)?lDrawScore: -Double_Round(m_lTableScore[i],2,1);
					
					// 收税
					if((m_pGameServiceOption->wServerType&GAME_GENRE_SCORE)==0 && lUserScore>0.001)
					{
						ScoreInfoArray[i].lRevenue = m_pITableFrame->CalculateRevenue(i, lUserScore);					
						lUserScore-=ScoreInfoArray[i].lRevenue;
					}
					//设置积分
					ScoreInfoArray[i].lScore = Double_Round(lUserScore,2,1);
					ScoreInfoArray[i].cbType = (ScoreInfoArray[i].lScore >0.001) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;
					GameEnd.lGameScore[i] = ScoreInfoArray[i].lScore;

					//扑克信息
					GameEnd.cbCardData[i]=(cbReason!=GER_NO_PLAYER)?m_cbHandCardData[i][0]:0;
					
					m_GameRecord.AppendFormat(TEXT("%s得分:%0.2f,%0.2f#"),m_szNickName[i],ScoreInfoArray[i].lScore, ScoreInfoArray[i].lRevenue);
				
				}
				else
				{
					//设置扑克
					GameEnd.cbCardData[i] = 0;

					//设置成绩
					GameEnd.lGameScore[i] = -m_lTableScore[i];
				}

				//历史积分
				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}

			


			
			//统计结束写分操作结束
			/////////////////////////////////////////////////////////
			////统计库存信息 只有存在机器人并且存在真实用户的情况下才会计算
			bool  bUserAI = false;  //是否存在机器人
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (pIServerUserItemTemp->IsAndroidUser()){
					//存在机器人
					bUserAI = true;
					//跳出FOR
					break;

				}
			}
			//统计信息与计算库存
			SCORE lSystemScore=0.0;    //玩家得分
			SCORE lCurrentStorgeNum = m_lStockScore; //当前库存值
			SCORE RevenueScore = 0.0;	 //税收
			SCORE lStorageDeduct=0.0;	//库存衰减
			//库存统计 
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//如果用户不存在
				if (pIServerUserIte==NULL)continue;					
				//存在机器人才执行
				if(bUserAI){
					//如果用户不是机器人
					if(!pIServerUserIte->IsAndroidUser())
					{
						//计算税收
						if(ScoreInfoArray[i].lRevenue>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//计算库存衰减
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//系统库存值
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore+RevenueScore,2,1)+lStorageDeduct); 
						//系统得分
						lSystemScore-=ScoreInfoArray[i].lScore;
					}
				}
			}

			////////////////////////////////////////////////////////////

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

			//判断是否存在真实玩家
			if (bUser)
			{
				
				CString strLog;
				strLog.Format(TEXT("桌号：%d,当前库存：%0.2f,系统得分：%0.2f,税收：%0.2f,剩余库存：%0.2f,库存衰减：%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息

				//梦成网络添加
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%I64d"),m_lStockLimit);
				//记录总输赢记录
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//如果存在控制用户的话//发送游戏信息给控制号
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue;	//如果用户不存在
					if(pUserItem->IsAndroidUser()) continue;  //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //发送消息窗口
				}
			}

			//////////////////////////////////////////////////////
			//扣除库存
			m_lStorageOff=GetPrivateProfileInt(m_pGameServiceOption->szServerName,_T("StorageOff"),0,m_szFileName);
			//如果扣减数不等于0
			if (m_lStorageOff != 0)
			{
				m_lStockScore -= m_lStorageOff;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("人为操作库存减少%I64d，剩余库存：%0.2f"),m_lStorageOff, m_lStockScore);
				//输出信息
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				//设置完之后把该值变成0
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageOff"),TEXT("0"),m_szFileName);

				//发送控制号窗口提示
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue; //如果用户不存在
					if(pUserItem->IsAndroidUser()) continue; //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue; //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_EJECT);  //发送消息窗口

				}
			}

			//库存数据处理结束

			/////////////////////////////////////////////////



			//发送数据
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			TraceMessage(m_GameRecord,m_pITableFrame->GetTableID());

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);

			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			TraceMessage(m_GameRecord,m_pITableFrame->GetTableID());

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);

			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:
		{
			if(m_cbPlayStatus[wChairID]==TRUE)
			{
				//放弃处理
				OnUserGiveUp(wChairID);
			}

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//设置变量
			StatusFree.lCellScore=(m_pGameServiceOption->lCellScore);

			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}
			
				//房间名称
				CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));


			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_PLAY:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//设置变量
			StatusPlay.lCellScore=(m_pGameServiceOption->lCellScore);
			
			//加注信息
			StatusPlay.lDrawMaxScore=m_lDrawMaxScore;
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore;
			StatusPlay.lTurnLessScore=m_lTurnLessScore;
			CopyMemory(StatusPlay.lUserScore,m_lUserScore,sizeof(StatusPlay.lUserScore));
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(StatusPlay.lTableScore));

			//状态信息
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.cbShowHand=(m_bShowHand==true)?TRUE:FALSE;
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(StatusPlay.cbPlayStatus));

			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//设置扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//设置数目
				StatusPlay.cbCardCount[i]=m_cbCardCount[i];

				//设置扑克
				if (m_cbPlayStatus[i]==TRUE)
				{
					if ((i==wChairID)&&(bSendSecret==true)&&pIServerUserItem->GetUserStatus()!=US_LOOKON) 
						StatusPlay.cbHandCardData[i][0]=m_cbHandCardData[i][0];
					CopyMemory(&StatusPlay.cbHandCardData[i][1],&m_cbHandCardData[i][1],(m_cbCardCount[i]-1)*sizeof(BYTE));
				}
			}	//房间名称
				CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			
			

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_GIVE_UP:			//用户放弃
		{
			//状态效验
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//用户效验
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			return OnUserGiveUp(pIServerUserItem->GetChairID());
		}
	case SUB_C_ADD_SCORE:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//状态效验
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//用户效验
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//变量定义
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserAddScore(wChairID,pAddScore->lScore);
		}
		
		case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
			return OnSubAmdinCommand(pIServerUserItem,pData);
		}
	}

	return false;
}

//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//历史积分
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(pIServerUserItem->GetChairID());

	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//历史积分
	if (bLookonUser==false) m_HistoryScore.OnEventUserLeave(pIServerUserItem->GetChairID());

	return true;
}

//用户放弃
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	if(m_cbPlayStatus[wChairID]==FALSE)
		return true;
	//设置变量
	m_cbPlayStatus[wChairID]=FALSE;

	// 获取玩家
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

	// 扣除分数
	SCORE lDeficiencyPoint = -(m_lUserScore[wChairID] + m_lTableScore[wChairID] );

	// 扣除服务费
	if ( lDeficiencyPoint < 0.000 &&  pIServerUserItem->GetUserScore()+lDeficiencyPoint<0.001 )
	{
		lDeficiencyPoint =  -pIServerUserItem->GetUserScore();
	}

	//写入积分
	tagScoreInfo  ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo.lScore=lDeficiencyPoint;
	ScoreInfo.cbType=SCORE_TYPE_LOSE;
	m_pITableFrame->WriteUserScore(wChairID, ScoreInfo);

	//游戏记录
	m_GameRecord.AppendFormat(TEXT("%s,弃,得分:%0.2f,%0.2f#"),m_szNickName[wChairID],ScoreInfo.lScore, 0);

	//人数计算
	WORD wPlayerCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//继续判断
	if (wPlayerCount>=2)
	{
		//下注调整
		RectifyMaxScore();

		//切换用户
		if (m_wCurrentUser==wChairID)
		{
			//设置用户
			m_wCurrentUser=INVALID_CHAIR;

			//用户搜索
			for (WORD i=1;i<GAME_PLAYER;i++)
			{
				//变量定义
				WORD wCurrentUser=(wChairID+i)%GAME_PLAYER;
				SCORE lDrawAddScroe=m_lUserScore[wCurrentUser]+m_lTableScore[wCurrentUser];

				//状态判断
				if (m_cbPlayStatus[wCurrentUser]==FALSE) continue;

				//用户切换
				if ((m_cbOperaScore[wCurrentUser]==FALSE)||(lDrawAddScroe<m_lTurnLessScore))
				{
					m_wCurrentUser=wCurrentUser;
					break;
				}
			}
		}
	}
	else 
	{
		//汇集金币
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			m_lTableScore[i]+=m_lUserScore[i];
			m_lUserScore[i]=SCORE_ZERO;
		}

		//设置用户
		m_wCurrentUser=INVALID_CHAIR;
	}

	//变量定义
	CMD_S_GiveUp GiveUp;
	ZeroMemory(&GiveUp,sizeof(GiveUp));

	//设置变量
	GiveUp.wGiveUpUser=wChairID;
	GiveUp.wCurrentUser=m_wCurrentUser;
	GiveUp.lDrawMaxScore=m_lDrawMaxScore;
	GiveUp.lTrunMaxScore=m_lTurnMaxScore;
		
	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//结束游戏
	if (wPlayerCount<=1)
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);
		return true;
	}

	//发送扑克
	if (m_wCurrentUser==INVALID_CHAIR) DispatchUserCard();

	return true;
}

//用户加注
bool CTableFrameSink::OnUserAddScore(WORD wChairID, SCORE lScore)
{
	//状态效验
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false;

	SCORE lAddScore = (lScore);
	//加注效验
	ASSERT((lAddScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])<=m_lTurnMaxScore);
	ASSERT((lAddScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])>=m_lTurnLessScore);
	ASSERT(lAddScore>-0.000001);

	//加注效验
	if ((lAddScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])>m_lTurnMaxScore) 
	{
		lAddScore = Double_Round(m_lTurnMaxScore-m_lUserScore[wChairID]-m_lTableScore[wChairID],2,1);
	}
	if (lAddScore<-0.001||((lAddScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])<m_lTurnLessScore))
	{
		lAddScore = Double_Round(m_lTurnLessScore-m_lTableScore[wChairID]-m_lUserScore[wChairID],2,1);
	}

	//设置变量
	m_cbOperaScore[wChairID]=TRUE;
	m_lUserScore[wChairID]+=lAddScore;
	m_lTurnLessScore=m_lUserScore[wChairID]+m_lTableScore[wChairID];

	//游戏记录
	m_GameRecord.AppendFormat(TEXT("%s加,%0.2f,已下注:%0.2f,桌面:%0.2f#"),m_szNickName[wChairID],lAddScore,m_lUserScore[wChairID],m_lTableScore[wChairID]);

	//状态变量
	m_wCurrentUser=INVALID_CHAIR;
	m_bShowHand=(abs(m_lTurnLessScore-m_lDrawMaxScore)<0.001);

	//用户搜索
	for (WORD i=1;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wCurrentUser=(wChairID+i)%GAME_PLAYER;
		//状态判断
		if (m_cbPlayStatus[wCurrentUser]==FALSE) continue;

		SCORE lDrawAddScroe=m_lUserScore[wCurrentUser]+m_lTableScore[wCurrentUser];

		//用户切换
		if ((m_cbOperaScore[wCurrentUser]==FALSE)||(m_lTurnLessScore-lDrawAddScroe>0.001))
		{
			m_wCurrentUser=wCurrentUser;
			break;
		}
	}

	//变量定义
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore,sizeof(AddScore));

	//设置变量
	AddScore.wAddScoreUser=wChairID;
	AddScore.wCurrentUser=m_wCurrentUser;
	AddScore.lTurnLessScore=m_lTurnLessScore;
	AddScore.lUserScoreCount=m_lUserScore[wChairID];


	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));

	//发送扑克
	if (m_wCurrentUser==INVALID_CHAIR) DispatchUserCard();

	return true;
}

//调整下注
VOID CTableFrameSink::RectifyMaxScore()
{
	//设置变量
	m_lDrawMaxScore=SCORE_ZERO;
	m_lTurnMaxScore=SCORE_ZERO;

	//最大下注
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//用户判断
		if (m_cbPlayStatus[i]==FALSE) continue;

		//变量设置
		if ((m_lDrawMaxScore<=0.001&&m_lDrawMaxScore>=-0.001)||(m_lDrawMaxScore-m_lUserMaxScore[i]>0.001)) m_lDrawMaxScore=m_lUserMaxScore[i];
	}

	//当前下注
	if (m_cbSendCardCount<=2) m_lTurnMaxScore=KeepOne(m_lDrawMaxScore/4);
	else if (m_cbSendCardCount==3) m_lTurnMaxScore=KeepOne(m_lDrawMaxScore/2);
	else m_lTurnMaxScore=KeepOne(m_lDrawMaxScore);

	return;
}

//发送扑克
VOID CTableFrameSink::DispatchUserCard()
{
	//汇集金币
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_lTableScore[i]+=m_lUserScore[i];
		m_lUserScore[i]=SCORE_ZERO;
	}

	//结束判断
	if (m_cbSendCardCount==MAX_COUNT)
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
		return;
	}

	//派发扑克
	BYTE cbSourceCount=m_cbSendCardCount;
	m_cbSendCardCount=(m_bShowHand==false)?(m_cbSendCardCount+1):MAX_COUNT;

	//当前用户
	if (m_bShowHand==false)
	{
		//状态变量
		ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));

		//设置用户
		m_wCurrentUser=EstimateWinner(1,m_cbSendCardCount-1);

		//下注设置
		m_lTurnMaxScore=((m_cbSendCardCount>=4)?m_lDrawMaxScore:m_lDrawMaxScore/2.000);
		m_lTurnLessScore=(m_lUserScore[m_wCurrentUser]+m_lTableScore[m_wCurrentUser]);
	}
	else
	{
		//设置变量
		m_wCurrentUser=INVALID_CHAIR;
		m_lTurnMaxScore=m_lDrawMaxScore;
		m_lTurnLessScore=m_lDrawMaxScore;
	}

	//构造数据
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard,sizeof(SendCard));

	//设置变量
	SendCard.wCurrentUser=m_wCurrentUser;
	SendCard.lTurnMaxScore=m_lTurnMaxScore;
	SendCard.wStartChairID=EstimateWinner(1,cbSourceCount-1);
	SendCard.cbSendCardCount=m_cbSendCardCount-cbSourceCount;

	//发送扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//状态判断
		if (m_cbPlayStatus[i]==FALSE) continue;

		m_GameRecord.AppendFormat( TEXT("%s-发牌:"),m_szNickName[i] );

		//设置数目
		m_cbCardCount[i]=m_cbSendCardCount;

		//派发扑克
		for (BYTE j=0;j<(m_cbSendCardCount-cbSourceCount);j++)
		{
			ASSERT(j<CountArray(SendCard.cbCardData[i]));
			SendCard.cbCardData[j][i]=m_cbHandCardData[i][cbSourceCount+j];
			m_GameRecord.AppendFormat( TEXT("%s,"),	TransformCardInfo(SendCard.cbCardData[j][i]) );
		}
		m_GameRecord+=TEXT("#");
	}

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	//结束处理
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

	return;
}

//推断输者
WORD CTableFrameSink::EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos)
{
	//保存扑克
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//寻找玩家
	WORD  wLoser=0;
	for ( wLoser=0;wLoser<GAME_PLAYER;wLoser++)
	{
		if (m_cbPlayStatus[wLoser]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//对比玩家
	WORD i=0;
	for ( i=(wLoser+1);i<GAME_PLAYER;i++)
	{
		//用户过滤
		if (m_cbPlayStatus[i]==FALSE) continue;

		//排列扑克
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//对比扑克
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1)==false) 
		{
			wLoser=i;
		}
	}

	return wLoser;
}

//推断胜者
WORD CTableFrameSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	//保存扑克
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//寻找玩家
	WORD wWinner=0;
	for ( wWinner=0;wWinner<GAME_PLAYER;wWinner++)
	{
		if (m_cbPlayStatus[wWinner]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}
	//对比玩家
	WORD wId = wWinner;
	for (WORD i=0;i<GAME_PLAYER-1;i++)
	{
		wId = (wId+1)%GAME_PLAYER;

		//用户过滤
		if (m_cbPlayStatus[wId]==FALSE) continue;

		//排列扑克
		m_GameLogic.SortCardList(cbUserCardData[wId]+cbStartPos,cbConcludePos-cbStartPos+1);

		//对比扑克
		if (m_GameLogic.CompareCard(cbUserCardData[wId]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=wId;
		}
	}

	return wWinner;
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

//输出信息
void TraceMessage(CString szinfo,WORD id)
{
	if(!szinfo.IsEmpty())
		szinfo.Replace(TEXT("#"),TEXT("\r\n"));
	
	COleDateTime timeNow, dateNow;
	timeNow = COleDateTime::GetCurrentTime();     // 获取当前日期时间
	dateNow = COleDateTime::GetCurrentTime();     // 同样获取当前日期时间
	CString sTime = timeNow.Format(VAR_TIMEVALUEONLY);     // 获取当前时间
	CString sDate = dateNow.Format(VAR_DATEVALUEONLY);     // 获取当前日期

	TCHAR tchBuffer[MAX_PATH ]; 
	LPTSTR lpszCurDir;
	lpszCurDir = tchBuffer; 
	GetCurrentDirectory(MAX_PATH , lpszCurDir);//得到当前程序路径

	CString Filep = lpszCurDir;
	Filep.AppendFormat(TEXT("\\%s"),GAME_NAME);

	//创建文件夹	游戏分类
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ创建文件夹失败！")+Filep,TraceLevel_Exception);
		 }
	}
	Filep.AppendFormat(TEXT("\\TableID_%d"),id);

	//创建文件夹	桌子分类
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ创建文件夹失败！")+Filep,TraceLevel_Exception);
		 }
	}
	Filep.AppendFormat(TEXT("\\%s"),sDate);
	
	//创建文件夹	日期分类
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ创建文件夹失败！")+Filep,TraceLevel_Exception);
		 }
	}

	
	CString FileName=TEXT("");     // 文件名 = 日期_时间.txt
	FileName.AppendFormat(TEXT("%s\\%s.txt"),Filep,sTime);

	TCHAR pFilePath[MAX_PATH];
	LPCTSTR filepath = FileName;
	int FilePathLength = FileName.GetLength();     // 获得文件名长度，CFile不能用CString，只能用string做参数，要做转换
	int i=0;
	for( i=0; i < FilePathLength ; i++)
	{
		pFilePath[i] = FileName.GetAt(i);     // CString -> string
		if( (pFilePath[i] == TEXT(':')) && (i != 1))     // 剔除':'等不能作为文件名的符号,并保留根目录后的冒号
			pFilePath[i] = '-';
	}
	pFilePath[i] = '\0';     // 文件名结束

	CFile File;
	if ((File.Open(pFilePath,CFile::modeWrite)==FALSE)&&
		(File.Open(pFilePath,CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	File.SeekToEnd();
	File.Write(szinfo.GetBuffer(),szinfo.GetLength()*2);

	szinfo.ReleaseBuffer();
	File.Flush();
	File.Close();

	return;
}

//推断胜者
WORD CTableFrameSink::EstimateWinnerEx(BYTE cbStartPos, BYTE cbConcludePos)
{
	//保存扑克
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//寻找玩家
	WORD wWinner=0;
	for ( wWinner=0;wWinner<GAME_PLAYER;wWinner++)
	{
		if (m_cbPlayStatus[wWinner]==TRUE) 
		{
			//排列扑克
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//对比玩家
	WORD wId = wWinner;
	for (WORD i=0;i<GAME_PLAYER-1;i++)
	{
		wId = (wId+1)%GAME_PLAYER;

		//用户过滤
		if (m_cbPlayStatus[wId]==FALSE) continue;

		//排列扑克
		m_GameLogic.SortCardList(cbUserCardData[wId]+cbStartPos,cbConcludePos-cbStartPos+1);

		//对比扑克
		if (m_GameLogic.CompareCard(cbUserCardData[wId]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=wId;
		}
	}

	return wWinner;
}

//////////////////////////////////////////////////////////////////////////
//读取配置
VOID CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	
	TCHAR OutBuf[255];
	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);
	//读取配置
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\HKFiveCardConfig.ini"),szPath);
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

	//是否读取每盘刷新配置变量
	if (bReadFresh)
	{
		//每盘刷新
		BYTE cbRefreshCfg = GetPrivateProfileInt(szServerName, TEXT("Refresh"), 0, szFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;

		//是否刷新库存
		BYTE cbRefreshStorageCfg = GetPrivateProfileInt(szServerName, TEXT("RefreshStorage"), 0, szFileName);
		m_bRefreshStorageCfg = cbRefreshStorageCfg?true:false;

	}

	//库存设置
	if(m_bRefreshStorageCfg)
	{
		//重置库存
		memset(OutBuf,0,255);
		GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,szFileName);
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%0.2f"),&m_lStockScore);
	}


	//重置衰减值
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageDeduct);

	//库存最大值
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageMax"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageMax);

		//梦成网络添加
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
		if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	//m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);


	//系统总输赢分
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageLimit"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStockLimit);

	

		//梦成网络添加   //最新前台控制

	////2012-08-28
	//控制玩家输赢
	//控制玩家个数
	m_WinCount = GetPrivateProfileInt(szServerName, TEXT("WinCount"), 0, szFileName);	
	//是否控制
	BYTE cbWinnerCtrl = GetPrivateProfileInt(szServerName, TEXT("WinnerCtrl"), 0, szFileName);
	m_bWinnerCtrl = cbWinnerCtrl?true:false;
	//需要胜出的玩家UserID
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("WinnerUserID"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%lu"),&m_dwCheatGameID);
	//读取胜利类型
	m_cbCheatType = GetPrivateProfileInt(szServerName, TEXT("CheatType"), 1, szFileName);
	//读取控制次数
	m_dwCheatCount = GetPrivateProfileInt(szServerName, TEXT("CheatCount"), 0, szFileName);
	//玩家输赢百分比
	m_WinningRatio = GetPrivateProfileInt(szServerName, TEXT("WinningRatio"), 0, szFileName);


	/////////////////



	//////获取机器人输赢
	BYTE cbAndroidWinningCtrl = GetPrivateProfileInt(szServerName, TEXT("AndroidWinningCtrl"), 0, szFileName);
	m_bAndroidWinningCtrl = cbAndroidWinningCtrl?true:false;

	//////获取机器人输赢比率
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("AndroidWinningRatio"),TEXT("0"),OutBuf,60,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_AndroidWinningRatio);
	if (m_AndroidWinningRatio<0 || m_AndroidWinningRatio >100)
	{
		m_AndroidWinningRatio = 60;
	}




}

//试探性判断, 确保Chair的用户可以赢
void CTableFrameSink::ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType)
{	
	//获取用户

	IServerUserItem * pIServerCtrlUserItem=m_pITableFrame->GetTableUserItem(wChairIDCtrl);
	//待用户不存在, 不需要控制

	if( NULL==pIServerCtrlUserItem ) return;
	if( pIServerCtrlUserItem->GetUserID()!=m_dwCheatGameID ) return;
	
	//最大牌值玩家索引
	int iMaxIndex= EstimateWinnerEx(0,4);
	int iLostIndex=EstimateLoser(0,MAX_COUNT-1);

	//临时变量
	BYTE cbTemp[MAX_COUNT]={0};
	//交换牌
	WORD wWinerUser=iMaxIndex;

	if(cbCheatType==CHEAT_TYPE_LOST) wWinerUser=iLostIndex;

	if (wWinerUser!=wChairIDCtrl)
	{

		CopyMemory(cbTemp,m_cbHandCardData[wWinerUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wWinerUser],m_cbHandCardData[wChairIDCtrl],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wChairIDCtrl],cbTemp,MAX_COUNT);

	}

				//减少控制次数
	if(m_dwCheatCount>0) m_dwCheatCount--;
	if(m_dwCheatCount==0) {m_dwCheatGameID=0;}
	m_bWinnerCtrl = false;

			//写入控制总数
			CString TempValue;
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
	
	return;
}

//获取椅子号
WORD CTableFrameSink::GetChairIDFromGameID(DWORD dwGameID)
{
	for(int i=0; i<GAME_PLAYER; i++)
	{
		IServerUserItem * pUser=m_pITableFrame->GetTableUserItem(i);
		if(pUser==NULL)continue;
		if(pUser->GetUserID()==dwGameID)
		{
			return i;
		}
	}
	return INVALID_CHAIR;
}

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
	//如果不具有管理员权限 则返回错误
	if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);
	switch(AdminReq->cbReqType)
	{
	case RQ_OPTION_CANCLE:	//取消
		{
			if (m_dwCheatGameID!=0)
			{
				//限制提示
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("已经成功将GameID为:%d的作弊权限取消"),m_dwCheatGameID);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
				m_dwCheatGameID=0;
			}
			else
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("当前没有设置作弊用户!"),SMT_CHAT);
			}
			break;
		}
	case RQ_OPTION_QUERYING:			//查询
		{
			if (m_dwCheatGameID!=0)
			{
				TCHAR szType[][32]={TEXT("输"),TEXT("赢")};

				if(m_cbCheatType!=0 && m_cbCheatType!=1) break;

				//限制提示
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("当前作弊GameID：%d，剩余控制次数：%d局，控制类型：%s\n"),
					m_dwCheatGameID,m_dwCheatCount,szType[m_cbCheatType]);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			}
			else
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("当前没有设置作弊用户!"),SMT_CHAT);
			}
			break;
		}
	case RQ_OPTION_SETING:		//设置
		{
			m_dwCheatGameID=AdminReq->dwGameID;
			m_dwCheatCount=AdminReq->cbCheatCount;
			m_cbCheatType=AdminReq->cbCheatType;



						//梦成网络添加
			//写入配置文件
			CString TempValue;
			//写入控制类型
			TempValue.Format(TEXT("%d"),m_cbCheatType);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatType"),TempValue,m_szFileName);
			//写入控制总数
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
			//写入控制用户ID
			TempValue.Format(TEXT("%d"),m_dwCheatGameID);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("WinnerUserID"),TempValue,m_szFileName);
			//梦成网络添加


			m_dwCheatGameID1 = pIServerUserItem->GetUserID();
			TCHAR szType[][32]={TEXT("输"),TEXT("赢")};

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("成功为GameID:%d设置作弊权限,控制：%d次，类型：%s"),m_dwCheatGameID,m_dwCheatCount,szType[m_cbCheatType]);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			break;
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

//根据用户Id获取其所坐的椅子Id
WORD CTableFrameSink::GetChairIdByGameId(DWORD dwGameId)
{
	WORD wChairID;
	for (wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;

		if( pIServerUserItem->GetUserID() == dwGameId )
		{
			return wChairID;
		}
	}

	return INVALID_CHAIR;
}

//库存控制
bool CTableFrameSink::AndroidCheck()
{



		//机器人个数
	int iAndroidCount=0;
	//真人个数
	int iPlayerCount=0;

	//机器人换牌目标索引
	int iPostionAndroid[GAME_PLAYER]={0};
	//真人换牌目标索引
	int iPostionPlayer[GAME_PLAYER]={0};

	//最大牌值索引
	WORD iMaxIndex=INVALID_CHAIR;	

	//初始化指针
	IServerUserItem *pIServerUserItem=NULL;

	//获取本桌机器人个数、玩家个数
	for(WORD k=0;k<GAME_PLAYER;k++)
	{
		pIServerUserItem=m_pITableFrame->GetTableUserItem(k);
		if (pIServerUserItem!=NULL)
		{
			if (pIServerUserItem->IsAndroidUser())
			{
				//记录索引
				iPostionAndroid[iAndroidCount]=k;
				iAndroidCount++;
			}
			else
			{
				//记录索引
				iPostionPlayer[iPlayerCount]=k;
				iPlayerCount++;
			}
		}
	}

	//没有机器人返回
	if(iAndroidCount==0) return false;
	//没有真人返回
	if (iPlayerCount==0) return false;
	//库存大于0 返回
	if(m_lStockScore>=m_lStorageMax) return false;

	


	//换牌变量
	int iTarget=0;//目标索引
	bool bChange = false;//是否要换牌

	

	//iMaxindexitem获取
//	pIServerUserItem=m_pITableFrame->GetTableUserItem(iMaxIndex);
//	if(pIServerUserItem == NULL) return false; 

	
//	if(pIServerUserItem->IsAndroidUser()==FALSE)//最大牌不在机器人手上
	{
		iTarget=iPostionAndroid[rand()%iAndroidCount];
//		bChange = true;
	}

	



	iMaxIndex =  EstimateWinnerEx(0,4);

	//换牌
//	if (bChange)
	{
		//临时变量
		BYTE cbTemp[MAX_COUNT]={0};

		//交换牌
		CopyMemory(cbTemp,m_cbHandCardData[iMaxIndex],MAX_COUNT);
		CopyMemory(m_cbHandCardData[iMaxIndex],m_cbHandCardData[iTarget],MAX_COUNT);
		CopyMemory(m_cbHandCardData[iTarget],cbTemp,MAX_COUNT);

	

	}

return true;

}
