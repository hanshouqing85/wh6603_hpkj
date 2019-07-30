#include "StdAfx.h"
#include "TableFrameSink.h"
#include <map>
//////////////////////////////////////////////////////////////////////////

#define IDI_GAME_END				1										//结束标识
#define IDI_PASS_CARD				2										//放弃标识

#define TIME_PASS_CARD				50000									//放弃定时

#define	MAX_SHOW_HAND_SCORE			0x7FFFFFFF								//最大梭哈分数

//////////////////////////////////////////////////////////////////////////
void OutputDebugMsg(const char * szMsg, ...)
{
	//char szData[1024]={0};
	//va_list args;
	//va_start(args, szMsg);
	//_vsntprintf_s(szData, sizeof(szData) - 1, szMsg, args);
	//va_end(args);
	//OutputDebugString(szData);
}

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
//const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_cbCardCount,0,sizeof(m_cbCardCount));
	memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
	m_cbGameEndReason = GER_NORMAL;
	m_bGameEnd = false;
	m_bShowHand = false;
	m_bPoor = true;
	//财富信息
	m_lMaxGold=0;
	m_lTurnMaxGold = 0;
	m_lTurnBasicGold=0;
	m_lBasicGold = 0;
	memset(m_lTableGold,0,sizeof(m_lTableGold));
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	m_SingleUserArray.RemoveAll();
	
	m_lUserLevel[0]=12800000;
	m_lUserLevel[1]=26000000;
    m_lUserLevel[2]=1000000000;
	m_tagSingleUserCtrl = NULL;
	m_dwSingleCtrlUser = 0;
	m_bCtrlWin = false;
	m_UserLevel.clear();
	//辅助变量
	memset(m_cbPlayStatus,0,sizeof(m_cbPlayStatus));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_wCtrlWinner = INVALID_CHAIR;  // 控制赢的玩家
	m_arPensonInfo.RemoveAll();
	m_lMaxShowhandGold=0L;                     // 梭哈时最大金币数

// 	if(!CINIFile::IsFileExist(m_szConfigFileName))
// 		myprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SH_Config.ini"),szPath);
	m_nAllTrunsGold = 0;
	m_nAIWinTrunGold = 0;
	m_fAIWinPercent = (float)0.00001;
	m_fAILostPercent = 1.0f;
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
	//删除数据
	for (INT_PTR i=0;i<m_SingleUserArray.GetCount();i++)
	{
		SafeDelete(m_SingleUserArray[i]);
	}
	if(	m_tagSingleUserCtrl != NULL)
	{
		SafeDelete(	m_tagSingleUserCtrl);
	}
	return;
}


//开始模式
// enStartMode  CTableFrameSink::GetGameStartMode()
// {
// 	return m_GameStartMode;
// }


//接口查询--检测相关信息版本
VOID * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
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
    
	//m_pITableFrame->SetStartMode(START_MODE_ALL_READY);
	//游戏配置
//	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	myprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SH_Config_%d.ini"),szPath,m_pGameServiceOption->wServerID);

	CFileFind FileFinder;
	if(!FileFinder.FindFile(m_szConfigFileName))	
		myprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SH_Config.ini"),szPath);

	//////////////////////////////////////////////////////////////////////////
	// 加载控制变量
	m_arPensonInfo.RemoveAll();
	//读取配置
//	TCHAR szPath[MAX_PATH]=TEXT("");
//	GetCurrentDirectory(sizeof(szPath),szPath);
	//读取配置
//	TCHAR szFileName[MAX_PATH];
//	myprintf(szFileName,sizeof(szFileName),TEXT("%s\\UserList.ini"),szPath);
//	int iPersonNum = GetPrivateProfileInt( TEXT("Winner"), TEXT("PersonCount"), 0, szFileName );
//	if (iPersonNum>50)  // 最多控制50人
//	{
//		iPersonNum = 50;
//	}

	// 从配置文件文件中读取玩家名字
//	for (int i=0; i<iPersonNum; ++i)
//	{
//		TCHAR szKeyName[_MAX_PATH]={0};
//		S_PERSONINFO sPensonInfo;
//		ZeroMemory(&sPensonInfo, sizeof(sPensonInfo));
//		myprintf(szKeyName, sizeof(szKeyName), _T("UserID%d"),(i+1));
		// 读取玩家名称
//		sPensonInfo.dwUserID = GetPrivateProfileInt( TEXT("Winner"), szKeyName, 0, szFileName );
//		if (0 == sPensonInfo.dwUserID)  // 为空时不设置
//		{
//			continue;
//		}
		// 控制胜率的读取
//		myprintf(szKeyName, sizeof(szKeyName), TEXT("Percent%d"),(i+1));
//		sPensonInfo.iPercent = GetPrivateProfileInt( TEXT("Winner"), szKeyName, 0, szFileName );
///		// 初始化座位号
//		m_arPensonInfo.Add(sPensonInfo);
//	}
	return true;
}

//复位桌子
VOID  CTableFrameSink::RepositionSink()
{
	//状态变量
	m_wOperaCount=0;
	m_bCtrlWin = false;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_cbPlayStatus,0,sizeof(m_cbPlayStatus));
	memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
	memset(m_cbCardCount,0,sizeof(m_cbCardCount));
	m_cbGameEndReason = GER_NORMAL;
	m_bGameEnd = false;
	m_bShowHand = false;
	m_SingleUserArray.RemoveAll();

	//财富变量
	m_lMaxGold=0L;
	m_lTurnMaxGold = 0;
	m_lTurnBasicGold=0L;
	m_lBasicGold = 0;
	memset(m_lTableGold,0,sizeof(m_lTableGold));
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	m_dwSingleCtrlUser = 0;
	
	m_wCtrlWinner = INVALID_CHAIR;  // 控制赢的玩家
	m_lMaxShowhandGold=0L;                     // 梭哈时最大金币数
	return;
}

//游戏状态
bool CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	ASSERT(wChairID<m_wPlayerCount && m_cbPlayStatus[wChairID]==TRUE);
	if(wChairID<m_wPlayerCount && m_cbPlayStatus[wChairID]==TRUE)return true;
	return false;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	//状态变量
	m_bCtrlWin = false;
	m_wOperaCount=0;
	m_bSendCardCount=0;
	memset(m_cbPlayStatus,0,sizeof(m_cbPlayStatus));
	memset(m_cbCardCount,0,sizeof(m_cbCardCount));
	m_dwSingleCtrlUser = 0;
	//财富变量
	m_lMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_lTableGold,0,sizeof(m_lTableGold));

//////////////////////////////////////////////////////////////////////////
	//加载控制变量
	m_arPensonInfo.RemoveAll();
	//读取配置
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	//读取配置
	TCHAR szFileName[MAX_PATH];
	myprintf(szFileName,sizeof(szFileName),TEXT("%s\\Conf\\UserList.ini"),szPath);
	int iPersonNum = GetPrivateProfileInt( TEXT("Winner"), TEXT("PersonCount"), 0, szFileName );
	if (iPersonNum>50)  // 最多控制50人
	{
		iPersonNum = 50;
	}

	// 从配置文件文件中读取玩家名字
	for (int i=0; i<iPersonNum; ++i)
	{
		TCHAR szKeyName[_MAX_PATH]={0};
		S_PERSONINFO sPensonInfo;
		ZeroMemory(&sPensonInfo, sizeof(sPensonInfo));
		myprintf(szKeyName, sizeof(szKeyName), TEXT("UserID%d"),(i+1));
		// 读取玩家名称
		sPensonInfo.dwUserID = GetPrivateProfileInt( TEXT("Winner"), szKeyName, 0, szFileName );
		if (0 == sPensonInfo.dwUserID)  // 为空时不设置
		{
			continue;
		}
		// 控制胜率的读取
		_sntprintf(szKeyName, sizeof(szKeyName), TEXT("Percent%d"),(i+1));
		sPensonInfo.iPercent = GetPrivateProfileInt( TEXT("Winner"), szKeyName, 0, szFileName );
		// 初始化座位号
		m_arPensonInfo.Add(sPensonInfo);
	}

	//设置状态
	m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取用户
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);

		if(pIServerUser==NULL)
		{
			continue;
		}

		if(!pIServerUser->IsAndroidUser())
		{
			DWORD dwUserID = pIServerUser->GetUserID();
			IsControlUser(dwUserID);

		}
	}


	//变量定义
	//const tagUserScore * pUserScore=NULL;
	IServerUserItem * pIServerUserItem=NULL;

	m_lSessionID=GetTickCount();
	//计算下注
	bool bCalculateGold=false;
	__int64 lMaxShowHandScore = m_pGameServiceOption->lRestrictScore!=0L?m_pGameServiceOption->lRestrictScore:MAX_SHOW_HAND_SCORE;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{ 
			m_cbPlayStatus[i]=TRUE;
			//pUserScore=pIServerUserItem->GetUserScore();
			m_lUserScore[i] = __min(pIServerUserItem->GetUserScore(),lMaxShowHandScore);
			lMaxShowHandScore = __min(lMaxShowHandScore,pIServerUserItem->GetUserScore());
			if (bCalculateGold==false)
			{
				bCalculateGold=true;
				m_lMaxGold=m_lUserScore[i];
			}
			else 
			{
				m_lMaxGold=__min(m_lMaxGold,m_lUserScore[i]);
			}
		}
	}
	CString strLevel;

	WORD wCount = GetPrivateProfileInt(TEXT("USER_SCORE_LEVEL"),TEXT("ITEM_COUNT"), 13, m_szConfigFileName);
	int myint=0;
	TCHAR RankName[64];

	for(myint=0;myint<wCount;myint++)
	{
		strLevel.Format(L"USER_LEVEL%d",myint);
	  m_UserLevel[myint]=GetPrivateProfileInt(TEXT("USER_SCORE_LEVEL"),strLevel, 0, m_szConfigFileName);
	}
	for(myint=0;myint<wCount;myint++)
	{
		strLevel.Format(L"RANK_NAME%d",myint);
		GetPrivateProfileString(TEXT("USER_SCORE_LEVEL"),strLevel, TEXT(""),RankName,CountArray(RankName) ,m_szConfigFileName);
	//	GetPrivateProfileString(TEXT("AndroidGold"),TEXT("AllGold"),TEXT("1000000"),szBuf,CountArray(szBuf),m_szConfigFileName);
		if(!wcscmp(RankName,_T("无敌")))
		{
			 m_lUserLevel[USER_LEVEL_ZERO]=m_UserLevel[myint];
		//	 break;
		}
		if(!wcscmp(RankName,L"财神"))
		{
			m_lUserLevel[USER_LEVEL_FIRST]=m_UserLevel[myint];
		//	break;
		}
		if(!wcscmp(RankName,L"首富"))
		{
			m_lUserLevel[USER_LEVEL_SECOND]=m_UserLevel[myint];
		//	break;
		}
	}
	m_lTurnMaxGold = m_lMaxGold/2;
	//m_lBasicGold=__max(m_lMaxGold/50L,m_pGameServiceOption->lCellScore);
	if(m_lMaxGold<=m_lUserLevel[USER_LEVEL_FIRST])
	{
		BYTE byLevel = 0;
		for(byLevel = 0;byLevel < 13;byLevel++)
		{
			if(m_lMaxGold >= m_UserLevel[byLevel] && m_lMaxGold < m_UserLevel[byLevel+1])
				break;
		}

		m_bPoor = true;
		m_lBasicGold=m_UserLevel[byLevel]/25L;
		//m_lBasicGold=10000;
		
	}
	else if(m_lMaxGold>m_lUserLevel[USER_LEVEL_FIRST]&&m_lMaxGold<m_lUserLevel[USER_LEVEL_SECOND])
	{
		m_bPoor = false; 
		m_lBasicGold=1000000;
	}
	else if(m_lMaxGold>=m_lUserLevel[USER_LEVEL_SECOND])
	{
		m_bPoor = false; 
		m_lBasicGold=2000000;
	}
	for( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		if( m_cbPlayStatus[i] )
		{
			m_lTableGold[i*2+1]=m_lBasicGold;
		}
	}

	m_nCardIndex = 0;
	ZeroMemory(m_cbCardArray,sizeof(m_cbCardArray));
	// 分发扑克
	m_bSendCardCount=2;
	m_GameLogic.RandCardList(m_cbCardArray,sizeof(m_cbCardArray));

	for ( WORD i=0; i<m_wPlayerCount; i++)
	{
		if (m_cbPlayStatus[i])
		{
			CopyMemory(m_cbHandCardData[i],&m_cbCardArray[m_nCardIndex],5*sizeof(BYTE));
			m_nCardIndex+=5;
		}
	}

	int nTemp = GetPrivateProfileInt(TEXT("Android"),TEXT("AIWinPercent"), 500, m_szConfigFileName);
	if( nTemp>1000 )
		nTemp = 500;
	m_fAIWinPercent = (float)nTemp/1000;

	int nTemp1 = GetPrivateProfileInt(TEXT("Android"),TEXT("AILostPercent"), 1000, m_szConfigFileName);
	if( nTemp1>1000 )
		nTemp1 = 1000;
	m_fAILostPercent = (float)nTemp1/1000;
	
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;

	WORD wOneAI=0;
	WORD wOneUser=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			ASSERT(m_bPlayStatus[i]==TRUE);
			if(pIServerUserItem->IsAndroidUser()) 
			{
				wAiCount++ ;
				wOneAI =i;
			}
			else
			{
				wOneUser=i;
			}
			wPlayerCount++; 
		}
	}

	bool bHaveSingleCtrl = false;
	tagSingleUserCtrl* pSingleCtrlUser = NULL;
	WORD wCtrlChair = INVALID_CHAIR;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
		if(pUserItem == NULL) continue;
		for(WORD j = 0;j < m_SingleUserArray.GetCount();j++)
		{
			tagSingleUserCtrl* pSingle = m_SingleUserArray[j];
			if(pSingle->dwUserID == pUserItem->GetUserID())
			{
				wCtrlChair = i;
				bHaveSingleCtrl = true;
				pSingleCtrlUser=m_SingleUserArray[j];
			}
		}
	}
	if(bHaveSingleCtrl)
	{
		m_dwSingleCtrlUser = pSingleCtrlUser->dwUserID;
		m_bCtrlWin = pSingleCtrlUser->cbLoseWin;
		WORD wMaxUser= EstimateWinner(0,4);
		srand((unsigned)time(NULL));

		BYTE cbRand = rand()%100;
		if(cbRand < pSingleCtrlUser->cbWinLosePercent)
		{
			if(!pSingleCtrlUser->cbLoseWin)
			{
				wMaxUser = EstimateLoser(0,4);

			}

			BYTE byCardTemp[MAX_COUNT]={0};
			memcpy(byCardTemp, m_cbHandCardData[wCtrlChair],MAX_COUNT);
			memcpy(m_cbHandCardData[wCtrlChair], m_cbHandCardData[wMaxUser],MAX_COUNT);
			memcpy(m_cbHandCardData[wMaxUser], byCardTemp,MAX_COUNT);
		}

	}
	////在不做控制的情况下，如果全部是机器或没有机器人，则不做处理
	if(wPlayerCount != wAiCount && wAiCount!=0)
	{
		TCHAR szBuf[64] = {0};
		GetPrivateProfileString(TEXT("AndroidGold"),TEXT("AllGold"),TEXT("0"),szBuf,CountArray(szBuf),m_szConfigFileName);
		//m_nAllTrunsGold = _atoi64(szBuf);
		myscanf(szBuf,mystrlen(szBuf),_T("%I64d"),&m_nAllTrunsGold);

		GetPrivateProfileString(TEXT("AndroidGold"),TEXT("AiWinGold"),TEXT("0"),szBuf,CountArray(szBuf),m_szConfigFileName);
		//m_nAIWinTrunGold = _atoi64(szBuf);
		myscanf(szBuf,mystrlen(szBuf),_T("%I64d"),&m_nAIWinTrunGold);


		double fPercent = 0.00001;
		if( m_nAllTrunsGold > 0)
			fPercent= (float)m_nAIWinTrunGold/m_nAllTrunsGold;

		if( fPercent<m_fAIWinPercent )
		{
			WORD wMaxUser= EstimateWinner(0,4);
		CMD_S_AndroidWinner S_AndroidUser;
			if (m_pITableFrame->GetTableUserItem(wMaxUser)->IsAndroidUser())
			{
				S_AndroidUser.wWinner = wMaxUser;
			}
			else
			{
				BYTE byCardTemp[MAX_COUNT]={0};
				memcpy(byCardTemp, m_cbHandCardData[wOneAI],MAX_COUNT);
				memcpy(m_cbHandCardData[wOneAI], m_cbHandCardData[wMaxUser],MAX_COUNT);
				memcpy(m_cbHandCardData[wMaxUser], byCardTemp,MAX_COUNT);
				
				S_AndroidUser.wWinner = wOneAI;
			}

			m_pITableFrame->SendTableData(S_AndroidUser.wWinner,SUB_S_ANDROID_WINNER,&S_AndroidUser,sizeof(S_AndroidUser));

			for (WORD j=0;j<m_wPlayerCount;j++) 
			{
				if (m_cbPlayStatus[j]==TRUE)
				{
#ifdef	_DEBUG
					m_pITableFrame->SendGameMessage(m_pITableFrame->GetServerUserItem(j),_T("机器人赢"),/*SMT_CHAT*/0);
#endif
				}
			}
		}
		else if (fPercent> m_fAILostPercent)
		{
			WORD wMaxUser= EstimateWinner(0,4);
			if (m_pITableFrame->GetTableUserItem(wMaxUser)->IsAndroidUser())
			{
				BYTE byCardTemp[MAX_COUNT]={0};
				memcpy(byCardTemp, m_cbHandCardData[wOneUser],MAX_COUNT);
				memcpy(m_cbHandCardData[wOneUser], m_cbHandCardData[wMaxUser],MAX_COUNT);
				memcpy(m_cbHandCardData[wMaxUser], byCardTemp,MAX_COUNT);			

			}
			else
			{
			}
		}
		for (WORD j=0;j<m_wPlayerCount;j++) 
		{
			if (m_cbPlayStatus[j]==TRUE)
			{
				m_pITableFrame->SendGameMessage(m_pITableFrame->GetTableUserItem(j),_T("======="),SMT_CHAT);
			}
		}

	}

	if(wPlayerCount != wAiCount && wAiCount!=0)
	{
		CMD_S_AndroidWinner S_AndroidUser;

		WORD wMaxUser= EstimateWinner(0,4);
		//if (m_pITableFrame->GetTableUserItem(wMaxUser)->IsAndroidUser())
		//{
		//	S_AndroidUser.wWinner = wMaxUser;
		//}
		//else
		if (!m_pITableFrame->GetTableUserItem(wMaxUser)->IsAndroidUser()/* && rand()%80 > 0*/)
		{
			/*for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					if (m_pITableFrame->GetTableUserItem(i)->IsAndroidUser()&& rand()%80 > 0)
					{
						BYTE byCardTemp[MAX_COUNT]={0};
						memcpy(byCardTemp, m_cbHandCardData[i],MAX_COUNT);
						memcpy(m_cbHandCardData[i], m_cbHandCardData[wMaxUser],MAX_COUNT);
						memcpy(m_cbHandCardData[wMaxUser], byCardTemp,MAX_COUNT);	
						S_AndroidUser.wWinner = i;

						for (WORD j=0;j<m_wPlayerCount;j++) 
						{
							if (m_cbPlayStatus[j]==TRUE)
							{
								m_pITableFrame->SendGameMessage(m_pITableFrame->GetTableUserItem(j),_T("机器人赢1"),SMT_CHAT);
							}
						}
					}
				}
			}*/
		}
		else
		{
			for (WORD j=0;j<m_wPlayerCount;j++) 
			{
				if (m_cbPlayStatus[j]==TRUE)
				{
					for (int n=0; n<m_arPensonInfo.GetCount(); ++n)
					{
						if(m_pITableFrame->GetTableUserItem(j)->GetUserID() == m_arPensonInfo[n].dwUserID)
							m_pITableFrame->SendGameMessage(m_pITableFrame->GetTableUserItem(j),TEXT("机器人赢"),SMT_CHAT);
					}
				}
			}
		}
		S_AndroidUser.wWinner = EstimateWinner(0,4);
		m_pITableFrame->SendTableData(S_AndroidUser.wWinner,SUB_S_ANDROID_WINNER,&S_AndroidUser,sizeof(S_AndroidUser));

	}


	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			m_cbCardCount[i]=m_bSendCardCount;
		}
	}

	//设置用户
	m_wCurrentUser=EstimateWinner(1,1);

	//发送开始
	CMD_S_GameStart GameStart;
	memset(&GameStart,0,sizeof(GameStart));
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.lTurnMaxGold=m_lTurnMaxGold;
	GameStart.lTurnBasicGold=m_lTurnBasicGold;
	GameStart.bPoor = m_bPoor;

	GameStart.lBasicGold = m_lBasicGold ; //test
	GameStart.lShowHandScore = lMaxShowHandScore;
	m_lMaxShowhandGold=lMaxShowHandScore;                     // 梭哈时最大金币数
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		if( m_cbPlayStatus[i] )
			GameStart.bCardData[i]=m_cbHandCardData[i][1];
	}
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart.bFundusCard=m_cbHandCardData[i][0];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			//GameStart.bFundusCard=0;
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
	}

	m_pITableFrame->SetGameTimer( IDI_PASS_CARD,TIME_PASS_CARD,1,m_wCurrentUser );

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//游戏解散
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//构造数据
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->KillGameTimer(IDI_GAME_END);

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,TEXT(""),608);
			return true;
		}
	case GER_NORMAL:		//常规结束
	case GER_NO_PLAYER:		//没有玩家
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//统计信息
			WORD wWinerUser=EstimateWinner(0,4);
			ASSERT( wWinerUser != INVALID_CHAIR );
			__int64 lMaxWinScore = m_lUserScore[wWinerUser];
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//底牌信息
				if ((cbReason==GER_NO_PLAYER)||(m_cbPlayStatus[i]==FALSE)) GameEnd.bUserCard[i]=0;
				else GameEnd.bUserCard[i]=m_cbHandCardData[i][0];

				//积分信息
				if (i!=wWinerUser)
				{
					__int64 lScore = __min(__min(m_lTableGold[i*2]+m_lTableGold[i*2+1],m_lUserScore[i]),lMaxWinScore);
					GameEnd.lGameGold[i] = -lScore;
					GameEnd.lGameGold[wWinerUser] += lScore;
				}
			}

			//积分税收
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				__int64 lRevenue=0;
				//enScoreKind ScoreKind;
				//胜利类型
				ScoreInfoArray[i].cbType=(GameEnd.lGameGold[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				//计算税收
				if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					//扣税变量
					__int64 cbRevenue=m_pGameServiceOption->cbRevenueRatio;

					if (GameEnd.lGameGold[i]>=100L)
					{
						//计算税收
						lRevenue=(__int64)((__int64)GameEnd.lGameGold[i]*cbRevenue/1000L);

						//成绩变量
						GameEnd.lTax+=lRevenue;
						GameEnd.lGameGold[i]=GameEnd.lGameGold[i]-lRevenue;
					}
				}
				ScoreInfoArray[i].lRevenue = lRevenue;
				ScoreInfoArray[i].lScore=GameEnd.lGameGold[i];
			}

			//库存统计
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserIte==NULL) continue;

				//库存累计
				if(!pIServerUserIte->IsAndroidUser())
				{
					if(m_dwSingleCtrlUser == pIServerUserIte->GetUserID())
						WriteSingleUserScore(m_dwSingleCtrlUser,ScoreInfoArray[i].lScore,m_bCtrlWin);
				}

			}

			CountAIWinTruns(GameEnd);
			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->KillGameTimer(IDI_GAME_END);
			
			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,TEXT(""),608);
			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			////变量定义
			//DOUBLE lScore=0;
			//DOUBLE lRevenue=0;
			////enScoreKind ScoreKind;

			//////设置变量
			////ScoreKind=enScoreKind_Lost;
			//lScore=-m_lTableGold[wChairID*2]-m_lTableGold[wChairID*2+1];

			//DOUBLE lUserScore = pIServerUserItem->GetUserScore();

			//tagScoreInfo ScoreInfo;
			//ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			//ScoreInfo.cbType=SCORE_TYPE_FLEE;
			//ScoreInfo.lScore = __max(lScore,lUserScore*(-1));
			//ScoreInfo.lRevenue = 0;

			////用户写分
			//m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

			////判断结束
			//WORD wPlayerCount=0;
			//for (WORD i=0;i<m_wPlayerCount;i++)
			//{
			//	if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
			//}
			//if (wPlayerCount>=2)
			//{
			//	if (m_wCurrentUser==wChairID) OnUserAddGold(wChairID,0L,true,false);
			//}
			//else OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);

			//改为强退和放弃一样

			//强退处理
			OnUserGiveUp(wChairID);

			return true;
		}
	}

	return false;
}

//发送场景
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));
			StatusFree.dwBasicGold=0;
			StatusFree.bPoor = m_bPoor;
			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_STATUS_PLAY:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			//tagUserScore    *StatusUserScore;
			__int64   lUserLevel=0;
			memset(&StatusPlay,0,sizeof(StatusPlay));
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lTurnBasicGold=m_lTurnBasicGold;
             lUserLevel=pIServerUserItem->GetUserScore();      //获取积分情况
		     
			if(lUserLevel<=m_lUserLevel[USER_LEVEL_FIRST])
			{
				BYTE byLevel = 0;
				for(byLevel = 0;byLevel < 13;byLevel++)
				{
					if(lUserLevel >= m_UserLevel[byLevel] && lUserLevel < m_UserLevel[byLevel+1])
						break;
				}
				m_bPoor = true;
				m_lBasicGold=m_UserLevel[byLevel]/25L;

			}
			else if(lUserLevel>m_lUserLevel[USER_LEVEL_FIRST]&&lUserLevel<m_lUserLevel[USER_LEVEL_SECOND])
			{
				m_bPoor = false; 
				m_lBasicGold=1000000;
			}
			else if(lUserLevel>=m_lUserLevel[USER_LEVEL_SECOND])
			{
				m_bPoor = false;
				m_lBasicGold=2000000;
			}
			StatusPlay.lBasicGold=m_lBasicGold;
			CopyMemory(StatusPlay.bPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));
			CopyMemory(StatusPlay.lTableGold,m_lTableGold,sizeof(m_lTableGold));
			StatusPlay.lShowHandScore = m_pGameServiceOption->lRestrictScore!=0L?m_pGameServiceOption->lRestrictScore:MAX_SHOW_HAND_SCORE;
			StatusPlay.bShowHand = m_bShowHand?TRUE:FALSE;
			StatusPlay.bPoor = m_bPoor;
			//设置下注
			StatusPlay.lTurnMaxGold=m_lTurnMaxGold;

			//设置扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					ASSERT(m_pITableFrame->GetTableUserItem(i)!=NULL);
					StatusPlay.bTableCardCount[i]=m_cbCardCount[i];
					if (i==wChiarID/*&&pIServerUserItem->GetUserStatus()!=US_LOOKON*/) StatusPlay.bTableCardArray[i][0]=m_cbHandCardData[i][0];
					CopyMemory(&StatusPlay.bTableCardArray[i][1],&m_cbHandCardData[i][1],(m_cbCardCount[i]-1)*sizeof(BYTE));
				}
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//效验结果
	ASSERT(FALSE);

	return false;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_GAME_END:	//游戏结束
		{
			if (m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY)
			{
				OnEventGameConclude(INVALID_CHAIR,NULL,m_cbGameEndReason);
				return true;
			}
		}
	case IDI_PASS_CARD:
		{
			if ((wBindParam==m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				//获取用户
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentUser);
				
				//用户判断
				if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserStatus()!=US_OFFLINE))
				{
					OnUserGiveUp(m_wCurrentUser);
				}
				else m_pITableFrame->SetGameTimer(IDI_PASS_CARD,TIME_PASS_CARD,1,m_wCurrentUser);

				return true;
			}
		}
	}
	return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_ADD_GOLD:	//用户加注
		{
			//效验数据
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return true;

			//消息处理
			CMD_C_AddGold * pAddGold=(CMD_C_AddGold *)pDataBuffer;
			OnUserAddGold(pUserData->wChairID,pAddGold->lGold,false,pAddGold->bFllow);

			return true;
		}
	case SUB_C_GIVE_UP:		//用户放弃
		{
			//效验数据
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return true;

			//消息处理
			OnUserGiveUp(pUserData->wChairID);
			return true;
		}
	case SUB_C_USE_WINNER:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_UseWinner));
			if (wDataSize!=sizeof(CMD_C_UseWinner)) return false;

			// 非下注状态不处理
			if (GAME_STATUS_FREE != m_pITableFrame->GetGameStatus())
			{
				return true;
			}
			//消息处理
			CMD_C_UseWinner * pUseWinner=(CMD_C_UseWinner *)pDataBuffer;
			if (pUseWinner->wWinner < GAME_PLAYER)
			{
				// 当前账号不是控制账号则不处理
				for (int i=0; i<m_arPensonInfo.GetCount(); ++i)
				{
					if (pIServerUserItem->GetUserID() == m_arPensonInfo[i].dwUserID)
					{						
						// 控制胜负
						m_wCtrlWinner = pUseWinner->wWinner;
						break;
					}
				}				
			}
			return true;
		}
	case SUB_C_SHOW_CARD:
		{
			//tagUserData * pUserData=pIServerUserItem->GetUserData();
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return true;

			CMD_C_ShowCard ShowCard;
			ZeroMemory(&ShowCard,sizeof(ShowCard));
			ShowCard.wChairID = pUserData->wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(CMD_C_ShowCard));
			return true;
		}
	case SUB_C_CHECK_SUPER:
		{
			//当前账号不是控制账号则不处理
			for (int i=0; i<m_arPensonInfo.GetCount(); ++i)
			{
				if (pIServerUserItem->GetUserID() == m_arPensonInfo[i].dwUserID)
				{

					// 控制胜负
					m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_C_CHECK_SUPER);
					break;
				}
			}
			return true;
		}
	case SUB_C_SET_CARD:
		{
			bool bExists=false;
			//当前账号不是控制账号则不处理
			for (int i=0; i<m_arPensonInfo.GetCount(); ++i)
			{
				if (pIServerUserItem->GetUserID() == m_arPensonInfo[i].dwUserID)
				{
					bExists=true;
					break;
				}
			}
			if(!bExists) 
				return true;

			BYTE *pCardValue =(BYTE *)pDataBuffer;
			if(wDataSize != sizeof(BYTE)) return false;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;
		

			bool bFind(false);
			for (int i=m_nCardIndex; i<28;i++)
			{
				if (m_cbCardArray[i] == *pCardValue)
				{
					BYTE cbTempCard = m_cbCardArray[i];
					m_cbCardArray[i] = m_cbHandCardData[pIServerUserItem->GetChairID()][m_bSendCardCount];
					m_cbHandCardData[pIServerUserItem->GetChairID()][m_bSendCardCount] = cbTempCard;

					bFind = true;
				}
			}

			TCHAR szBuf[128]={0};
			myprintf(szBuf,sizeof(szBuf),TEXT("申请换牌（牌面：0x%02x）%s"),
				*pCardValue,bFind?TEXT("成功，下一张将发放。"):TEXT("失败，牌已发完。"));
			m_pITableFrame->SendGameMessage(pIServerUserItem,szBuf,SMT_CHAT);

			return true;
		}

	}

	return false;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//if(wSubCmdID==SUB_GF_USER_CHAT)
	//{
	//	//CString strCmd=				
	//	CMD_GF_UserChat * pUserChat=(CMD_GF_UserChat *)pDataBuffer;
	//	CString strCmd=pUserChat->szChatMessage;
	//	if(strCmd.Left(5)=="gc://")
	//	{
	//		if(pIServerUserItem->GetUserID()>10)
	//			return true;
	//		TCHAR szTipMsg[256]={0};
	//		if(strCmd=="gc://getinfo")
	//		{
	//			int nAIWinPercent = GetPrivateProfileInt(TEXT("Android"),TEXT("AIWinPercent"), 500, m_szConfigFileName);
	//			int nAILostPercent = GetPrivateProfileInt(TEXT("Android"),TEXT("AILostPercent"), 1000, m_szConfigFileName);
	//				
	//			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("AIWinPer:[%d],AILostPer[%d],AIAllWinGold:[%I64d],AIWinGold:[%I64d]"),
	//				nAIWinPercent,nAILostPercent,m_nAllTrunsGold,m_nAIWinTrunGold);
	//			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_PROMPT);
	//		}
	//		if(strCmd.Left(26)==TEXT("gc://setandroidwinpercent="))
	//		{			
	//			strCmd=TEXT("0")+strCmd.Mid(26).Trim();
	//			//找出第一个时间
	//			int i1=0,i2=0;
	//			i1=atoi(strCmd);
	//			int p=strCmd.Find(TEXT(","));
	//			if(p>0)
	//			{
	//				strCmd=TEXT("0")+strCmd.Mid(p+1).Trim();
	//				i2=atoi(strCmd);
	//			}

	//			if(i1>0)
	//			{

	//				m_fAIWinPercent=(float)i1/1000;
	//				strCmd.Format(TEXT("%d"),i1);
	//				WritePrivateProfileString(TEXT("Android"),TEXT("AIWinPercent"),strCmd,m_szConfigFileName);
	//			}
	//			if(i2>0)
	//			{
	//				m_fAILostPercent=(float)i2/1000;
	//				strCmd.Format("%d",i2);
	//				WritePrivateProfileString(TEXT("Android"),TEXT("AILostPercent"),strCmd,m_szConfigFileName);
	//			}
	//		}
	//		if(strCmd.Left(18)=="gc://setplacetime=")
	//		{
	//			int iTIME_LESS=GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_LESS"),1,m_szConfigFileName);
	//			int iTIME_RAND=GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_RAND"),2,m_szConfigFileName);

	//			strCmd="0"+strCmd.Mid(18).Trim();
	//			//找出第一个时间
	//			int i1=0,i2=0;
	//			i1=atoi(strCmd);
	//			int p=strCmd.Find(",");
	//			if(p>0)
	//			{
	//				strCmd="0"+strCmd.Mid(p+1).Trim();
	//				i2=atoi(strCmd);
	//			}
	//			if(i1>0)
	//			{
	//				iTIME_LESS=i1;
	//				strCmd.Format("%d",i1);
	//				WritePrivateProfileString("AndroidTime","TIME_LESS",strCmd,m_szConfigFileName);
	//			}
	//			if(i2>0)
	//			{
	//				iTIME_RAND=i2;
	//				strCmd.Format("%d",i2);
	//				WritePrivateProfileString("AndroidTime","TIME_RAND",strCmd,m_szConfigFileName);
	//			}
	//			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("机器人下注间隔时间:%d(秒) 随机时间:%d(秒)，注：下局生效"),iTIME_LESS,iTIME_RAND);
	//			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_PROMPT);
	//		}
	//		return true;
	//	}
	//}
	return false;
}

//放弃事件
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	//状态判断
	ASSERT(m_cbPlayStatus[wChairID]==TRUE);
	if (m_cbPlayStatus[wChairID]==FALSE) return false;

	//效验
	if( m_bGameEnd ) 
	{
		m_pITableFrame->KillGameTimer(IDI_GAME_END);
		return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	//设置数据
	m_cbPlayStatus[wChairID]=FALSE;
	m_cbCardCount[wChairID]=0;

	WORD i=0;
	//重新获取最大下注
	if( m_lUserScore[wChairID] == m_lMaxGold )
	{
		for( i = 0; i < GAME_PLAYER; i ++ )
		{
			if( !m_cbPlayStatus[i] ) continue;
			m_lMaxGold = m_lUserScore[i];
			break;
		}
		for( ; i < GAME_PLAYER; i++ )
		{
			if( m_cbPlayStatus[i] )
				m_lMaxGold = __min(m_lMaxGold,m_lUserScore[i]);
		}
	}
	m_lTurnMaxGold = m_bSendCardCount>=3?m_lMaxGold:m_lMaxGold/2L;

	//发送放弃消息
	CMD_S_GiveUp GiveUp;
	GiveUp.wUserChairID=wChairID;
	GiveUp.lTurnMaxGold = m_lTurnMaxGold;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//变量定义
	__int64 lScore=0;
	__int64 lRevenue=0;
	//enScoreKind ScoreKind;

	////设置变量
	//ScoreKind=enScoreKind_Lost;
	lScore=-m_lTableGold[wChairID*2]-m_lTableGold[wChairID*2+1];

	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo.cbType=SCORE_TYPE_LOSE;
	ScoreInfo.lScore = lScore;
	ScoreInfo.lRevenue = 0;

	//用户写分
	m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

	//判断结束
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddGold(wChairID,0L,true,false);
	}
	else OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	return true;
}

//加注事件
bool CTableFrameSink::OnUserAddGold(WORD wChairID, __int64 lGold, bool bCancel,bool bFllow)
{
	//效验
	if( m_bGameEnd ) 
	{
		m_pITableFrame->KillGameTimer(IDI_GAME_END);
		return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	//设置数据
	if (bCancel==false)
	{
		//效验数据
		ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;

		ASSERT( !m_bShowHand && lGold >= m_lTurnBasicGold && lGold <= m_lTurnMaxGold
			|| lGold+m_lTableGold[wChairID*2+1]==m_lMaxShowhandGold );
		if( (m_bShowHand || lGold < m_lTurnBasicGold || lGold > m_lTurnMaxGold)
			&& lGold+m_lTableGold[wChairID*2+1]!=m_lMaxShowhandGold ) 
			return false;

		m_lTableGold[wChairID*2]=lGold;
		__int64 iUserScore = m_lTableGold[wChairID*2]+m_lTableGold[wChairID*2+1];

		if( !m_bShowHand && (iUserScore == m_lMaxShowhandGold)) 
		{
			m_bShowHand = true;
		}
	}
	WORD i=0;
	//操作次数
	bool bFinishTurn=false;
	WORD wNextUser=INVALID_CHAIR;
	for (i=0;i<m_wPlayerCount-1;i++)
	{
		m_wOperaCount++;
		wNextUser=(m_wCurrentUser+i+1)%m_wPlayerCount;
		if (m_cbPlayStatus[wNextUser]==TRUE) break;
	}
	
	i=0;
	//判断下注
	if (m_wOperaCount>=m_wPlayerCount)
	{
		__int64 lTableGold=0L;
		for (i=0;i<m_wPlayerCount;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				lTableGold=m_lTableGold[i*2];
				break;
			}
		}
		while (i<m_wPlayerCount)
		{
			if (m_cbPlayStatus[i]==TRUE) 
			{
				if( !m_bShowHand&&m_lTableGold[i*2]!=lTableGold ||
					m_bShowHand&&m_lTableGold[i*2]+m_lTableGold[i*2+1]<m_lMaxShowhandGold)
					break;
			}
			i++;
		}
		if (i==m_wPlayerCount) bFinishTurn=true;
	}

	//一轮判断
	if (bFinishTurn==true)
	{
		//发送数据
		CMD_S_AddGold GoldResult;
		GoldResult.lCurrentLessGold=0L;
		GoldResult.wLastChairID=wChairID;
		GoldResult.wCurrentUser=INVALID_CHAIR;
		GoldResult.lLastAddGold=m_lTableGold[wChairID*2];
		GoldResult.bShowHand = m_bShowHand;
		GoldResult.bFllow=bFllow;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));

		//累计财富
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_lTableGold[i*2+1]+=m_lTableGold[i*2];
			m_lTableGold[i*2]=0L;
		}

		//开始一轮
		if (m_bSendCardCount<5) 
		{
			//设置变量
			m_wOperaCount=0;
			m_lTurnBasicGold=0L;
			WORD wLastMostUser = EstimateWinner(1,m_bSendCardCount-1);

			BYTE bySourceCount = m_bSendCardCount;
			if( m_bShowHand ) m_bSendCardCount = MAX_COUNT;
			else m_bSendCardCount++;

			//发送扑克
			CMD_R_SendCard SendCard;
			ZeroMemory( &SendCard,sizeof(SendCard) );
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if (m_cbPlayStatus[i]==FALSE) continue;

				m_cbCardCount[i]=m_bSendCardCount;
				IServerUserItem * pServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if (pServerUserItem!=NULL)
				{
					WORD wWinner=EstimateWinner(0,m_bSendCardCount-1);
					if (pServerUserItem->IsAndroidUser())
					{
						SendCard.wWin = wWinner;
					}else
					{
						SendCard.wWin = 0;
					}
				}

				//派发扑克
				for (BYTE j=0;j<(m_bSendCardCount-bySourceCount);j++)
				{
					ASSERT(j<CountArray(SendCard.bUserCard[i]));
					SendCard.bUserCard[i][j]=m_cbHandCardData[i][bySourceCount+j];
				}

				//发送真人明牌，给机器人
				if(pServerUserItem->IsAndroidUser())
				{
					WORD wNextChair = (i + 1) % GAME_PLAYER;
					IServerUserItem* pUserItemNext = m_pITableFrame->GetTableUserItem(wNextChair);
					if(!pUserItemNext->IsAndroidUser())
					{
						CMD_S_ShowHumanCard	ShowCard;
						memset(&ShowCard, 0, sizeof(CMD_S_ShowHumanCard));
						ShowCard.wChairID = wNextChair;
						ShowCard.cbCardCount = m_bSendCardCount;
						memcpy(ShowCard.cbUserCard, m_cbHandCardData[wNextChair], sizeof(ShowCard.cbUserCard));
						m_pITableFrame->SendUserItemData(pServerUserItem, SUB_S_SEND_HUMAN_CARD, &ShowCard, sizeof(CMD_S_ShowHumanCard));
					}
				}
			}

			m_wCurrentUser=EstimateWinner(1,m_bSendCardCount-1);
			m_lTurnMaxGold = m_bSendCardCount>=3?m_lMaxGold:m_lMaxGold/2L;

			//发送消息
			SendCard.wCurrentUser=(m_bShowHand?INVALID_CHAIR:m_wCurrentUser);
			SendCard.wStartChairId = wLastMostUser;
			SendCard.cbSendCardCount = m_bSendCardCount-bySourceCount;
			SendCard.lMaxGold=m_lTurnMaxGold;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

			//结束游戏
			if ( m_bSendCardCount == 5 && m_lMaxGold <= m_lTableGold[m_wCurrentUser*2+1] ) 
			{
				m_bGameEnd = true;
				m_cbGameEndReason = GER_NORMAL;
			//	m_pITableFrame->SetGameTimer(IDI_GAME_END,500,1,0);
				if (m_pITableFrame->GetGameStatus()== GAME_STATUS_PLAY)
				{
					OnEventGameConclude(INVALID_CHAIR,NULL,m_cbGameEndReason);
					return true;
				}

			}
		}
		else 
		{
			m_bGameEnd = true;
			m_cbGameEndReason = GER_NORMAL;
		//	m_pITableFrame->SetGameTimer(IDI_GAME_END,500,1,0);
						if (m_pITableFrame->GetGameStatus()== GAME_STATUS_PLAY)
			{
				OnEventGameConclude(INVALID_CHAIR,NULL,m_cbGameEndReason);
				return true;
			}

		}
	}
	else
	{
		//设置变量
		m_wCurrentUser=wNextUser;
		m_lTurnBasicGold=__max(m_lTableGold[wChairID*2],m_lTurnBasicGold);

		//发送数据
		CMD_S_AddGold AddGold;
		AddGold.bFllow=bFllow;
		AddGold.wLastChairID=wChairID;
		AddGold.wCurrentUser=m_wCurrentUser;
		AddGold.lCurrentLessGold=m_lTurnBasicGold;
		AddGold.lLastAddGold=m_lTableGold[wChairID*2];
		AddGold.bShowHand = m_bShowHand;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
	}

	if( m_wCurrentUser != INVALID_CHAIR )
		m_pITableFrame->SetGameTimer( IDI_PASS_CARD,TIME_PASS_CARD,1,m_wCurrentUser );

	return true;
}

//推断胜者
WORD CTableFrameSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	//保存扑克
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	WORD wWinner=0;
	//寻找玩家
	for (wWinner=0;wWinner<GAME_PLAYER;wWinner++)
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
WORD CTableFrameSink::EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos)
{
	//保存扑克
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	WORD wLoser=0;
	//寻找玩家
	for (wLoser=0;wLoser<GAME_PLAYER;wLoser++)
	{
		if (m_cbPlayStatus[wLoser]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//对比玩家
	WORD wId = wLoser;
	for (WORD i=0;i<GAME_PLAYER-1;i++)
	{
		wId = (wId+1)%GAME_PLAYER;

		//用户过滤
		if (m_cbPlayStatus[wId]==FALSE) continue;

		//排列扑克
		m_GameLogic.SortCardList(cbUserCardData[wId]+cbStartPos,cbConcludePos-cbStartPos+1);

		//对比扑克
		if (m_GameLogic.CompareCard(cbUserCardData[wId]+cbStartPos,cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1)==FALSE) 
		{
			wLoser=wId;
		}
	}

	return wLoser;
}
//////////////////////////////////////////////////////////////////////////
void CTableFrameSink::CountAIWinTruns(const CMD_S_GameEnd &GameEnd)
{
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			ASSERT(m_bPlayStatus[i]==TRUE);
			if(pIServerUserItem->IsAndroidUser()) 
			{
				wAiCount++;
			}
			wPlayerCount++;
		}
	}

	//如果本局全部是机器人或者全部是真人，则不做统计
	if(wPlayerCount == wAiCount || wAiCount==0)
		return;

	__int64 nAIGameScore = 0;	//本局所有机器人总的输赢
	for( int i=0; i<m_wPlayerCount; i++ )
	{
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			ASSERT(m_bPlayStatus[i]==TRUE);
			if(pIServerUserItem->IsAndroidUser()) 
				nAIGameScore += GameEnd.lGameGold[i];
		}
	}

	TCHAR szBuf[64] = {0};
	GetPrivateProfileString(TEXT("AndroidGold"),TEXT("AllGold"),TEXT("0"),szBuf,CountArray(szBuf),m_szConfigFileName);
	myscanf(szBuf,mystrlen(szBuf),_T("%I64d"),&m_nAllTrunsGold);
	//m_nAllTrunsGold = _atoi64(szBuf);

	GetPrivateProfileString(TEXT("AndroidGold"),TEXT("AiWinGold"),TEXT("0"),szBuf,CountArray(szBuf),m_szConfigFileName);
	myscanf(szBuf,mystrlen(szBuf),_T("%I64d"),&m_nAIWinTrunGold);
	//m_nAIWinTrunGold = _atoi64(szBuf);

	if( nAIGameScore > 0 )	
	{
		m_nAIWinTrunGold += nAIGameScore;
	}

	m_nAllTrunsGold += abs(long(nAIGameScore));

	CString strTemp;
	strTemp.Format(TEXT("%I64d"),m_nAllTrunsGold);
	WritePrivateProfileString(TEXT("AndroidGold"),TEXT("AllGold"),strTemp,m_szConfigFileName);
	strTemp.Format(TEXT("%I64d"),m_nAIWinTrunGold);
	WritePrivateProfileString(TEXT("AndroidGold"),TEXT("AiWinGold"),strTemp,m_szConfigFileName);
}

bool CTableFrameSink::IsControlUser(DWORD dwUserID)
{
	LONGLONG lScore=0;
	LONGLONG lMostScore=0;
	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\User_%d.ini"), szPath,m_pGameServiceOption->wServerID);

	CTraceService::TraceString(szConfigFileName,TraceLevel_Warning);
	CString strLog;

	DWORD dwCtrlUserID = 0;

	for(BYTE i = 1;i <= MAX_CTRL_USER_COUNT;i++)
	{
		TCHAR szSesion[10] = _T("");
		_sntprintf(szSesion,sizeof(szSesion),_T("%d"),i);
		bool bCheatUser = false;
		bCheatUser = GetPrivateProfileInt(szSesion, _T("IsCheatUser"), 0, szConfigFileName);

		strLog.Format(L"bCheatUser = %d",bCheatUser);
		CTraceService::TraceString(strLog,TraceLevel_Warning);

		if(!bCheatUser) continue;
		dwCtrlUserID = GetPrivateProfileInt(szSesion, _T("UserID"), -1, szConfigFileName);

		strLog.Format(L"dwCtrlUserID = %d",dwCtrlUserID);
		CTraceService::TraceString(strLog,TraceLevel_Warning);
		if(dwCtrlUserID == -1) continue;

		if(dwCtrlUserID == dwUserID)
		{
			BYTE cbLoseWin = 0;
			cbLoseWin = GetPrivateProfileInt(szSesion, _T("LoseWin"), -1, szConfigFileName);
			strLog.Format(L"cbLoseWin = %d",cbLoseWin);
			CTraceService::TraceString(strLog,TraceLevel_Warning);
			if(cbLoseWin == -1) continue;
			if(cbLoseWin == 1)
			{
				ZeroMemory(OutBuf, sizeof(OutBuf));
				GetPrivateProfileString(szSesion, TEXT("WinMaxGold"), _T("1000000"), OutBuf, 255, szConfigFileName);
				myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &lMostScore);

			}
			else
			{
				ZeroMemory(OutBuf, sizeof(OutBuf));
				GetPrivateProfileString(szSesion, TEXT("LoseMaxGold"), _T("1000000"), OutBuf, 255, szConfigFileName);
				myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &lMostScore);

			}

			strLog.Format(L"lMostScore = %ld",lMostScore);
			CTraceService::TraceString(strLog,TraceLevel_Warning);
			if(lMostScore <= 0 && cbLoseWin)
				return false;
			else if(lMostScore >= 0 &&!cbLoseWin)
				return false;
			ZeroMemory(OutBuf, sizeof(OutBuf));
			GetPrivateProfileString(szSesion, TEXT("UserGold"), _T("0"), OutBuf, 255, szConfigFileName);
			myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &lScore);

			IServerUserItem *pUserItem = m_pITableFrame->SearchUserItem(dwUserID);
			if(pUserItem == NULL) return false;

			WORD wChairID = pUserItem->GetChairID();


			BYTE cbLoseWinPer = 0;
			cbLoseWinPer = GetPrivateProfileInt(szSesion, _T("UserLoseWinPercent"),0, szConfigFileName);
			if(cbLoseWinPer == 0)
				return false;
			bool bExist = false;
			for(INT j = 0;j < m_SingleUserArray.GetCount();j++)
			{
				tagSingleUserCtrl* pSingleUser = m_SingleUserArray[i];
				if(pSingleUser->dwUserID == dwUserID)
				{
					bExist=true;
					m_SingleUserArray.RemoveAt(j);

					m_tagSingleUserCtrl = new tagSingleUserCtrl;

					m_tagSingleUserCtrl->cbCheatUser = bCheatUser;
					m_tagSingleUserCtrl->cbLoseWin = cbLoseWin;
					m_tagSingleUserCtrl->cbWinLosePercent = cbLoseWinPer;
					m_tagSingleUserCtrl->dwUserID = dwUserID;
					m_tagSingleUserCtrl->lCtrlScore = lMostScore-lScore;
					m_tagSingleUserCtrl->wChairID = wChairID;
					m_SingleUserArray.Add(m_tagSingleUserCtrl);

				}
			}

			if(!bExist)
			{
				m_tagSingleUserCtrl = new tagSingleUserCtrl;
				m_tagSingleUserCtrl->cbCheatUser = bCheatUser;
				m_tagSingleUserCtrl->cbLoseWin = cbLoseWin;
				m_tagSingleUserCtrl->cbWinLosePercent = cbLoseWinPer;
				m_tagSingleUserCtrl->dwUserID = dwUserID;
				m_tagSingleUserCtrl->lCtrlScore = lMostScore-lScore;
				m_SingleUserArray.Add(m_tagSingleUserCtrl);
			}
			return true;
		}
	}
	return false;
}
bool CTableFrameSink::WriteSingleUserScore(DWORD dwUserID,LONGLONG lWinScore,bool bWin)
{
	//获取路径
	TCHAR szWorkDir[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szWorkDir),szWorkDir);

	//构造路径
	TCHAR szIniFile[MAX_PATH]=TEXT("");

	_sntprintf(szIniFile,CountArray(szIniFile),TEXT("%s\\User_%d.ini"),szWorkDir,m_pGameServiceOption->wServerID);

	TCHAR	szSession[32]=TEXT("");

	for(WORD i = 1;i <= MAX_CTRL_USER_COUNT;i++)
	{
		_sntprintf(szSession,CountArray(szSession),TEXT("%d"),i);
		DWORD dwCtrlID = GetPrivateProfileInt(szSession,TEXT("UserID"),0,szIniFile);
		if(dwCtrlID == dwUserID) break;
	}

	__int64 lUserWinScore = 0;
	TCHAR szWinScore[33] = TEXT("");

	GetPrivateProfileString(szSession,TEXT("UserGold"),TEXT(""),szWinScore,sizeof(szWinScore),szIniFile);

	_snwscanf(szWinScore,wcslen(szWinScore),TEXT("%I64d"),&lUserWinScore);


	lUserWinScore+=lWinScore;


	CString strWinScore;
	strWinScore.Format(L"%I64d",lUserWinScore);

	WritePrivateProfileString(szSession,TEXT("UserGold"),strWinScore,szIniFile);
	bool bConclude = false;
	LONGLONG lMostScore = 0;
	if(bWin)
	{
		GetPrivateProfileString(szSession,TEXT("WinMaxGold"),TEXT(""),szWinScore,sizeof(szWinScore),szIniFile);

		_snwscanf(szWinScore,wcslen(szWinScore),TEXT("%I64d"),&lMostScore);

		lMostScore-=100000;
		if(lUserWinScore>=lMostScore)
			bConclude = true;
	}
	else
	{
		GetPrivateProfileString(szSession,TEXT("LoseMaxGold"),TEXT(""),szWinScore,sizeof(szWinScore),szIniFile);

		_snwscanf(szWinScore,wcslen(szWinScore),TEXT("%I64d"),&lMostScore);

		lMostScore+=100000;
		if(lUserWinScore<=lMostScore)
			bConclude = true;

	}
	if(bConclude)
	{
		strWinScore.Format(L"%d",0);
		WritePrivateProfileString(szSession,TEXT("IsCheatUser"),strWinScore,szIniFile);
	}

	return true;
}

