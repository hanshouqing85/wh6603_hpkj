#include "StdAfx.h"
#include "TableFrameSink.h"
#include "Config.h"

//////////////////////////////////////////////////////////////////////////
//得到当前程序所在的路径 最后一个字符无 '\'
string GetAppPath()
{
	HINSTANCE hInst=NULL;
	hInst=(HINSTANCE)GetModuleHandleA(NULL);

	CHAR path_buffer[_MAX_PATH];
	GetModuleFileNameA(hInst,path_buffer,sizeof(path_buffer));//得到exe文件的全路径 
	string strPath;

	strPath=path_buffer;

	//只提出文件的路径，不要文件名
	int pos=strPath.find_last_of("\\");

	strPath=strPath.substr(0,pos);


	return strPath;
}

// 创建目录
int create_dir(wchar_t* pszDirName, int iDirNameLen)
{
	if(!pszDirName || 0==iDirNameLen)
		return 10;	// 目录名是空值或者长度不正确

	int iResult = 0;
	int iFlag = 0;
	DWORD dwError = 0;

	// iMode 值的含义：
	//	00 Existence only
	//	02  Write-only
	//	04 Read-only
	//	06 Read and write
	int iMode = 4;

	// 判断目录存在否
	iFlag = _waccess(pszDirName, iMode);

	if(0==iFlag)	// 存在该目录
	{
		return 0;
	}	
	else	// 没有该目录
	{
		iFlag = _wmkdir(pszDirName);
		if(0==iFlag)	// 建好了
		{
		}
		else	// 创建失败
		{
			dwError = GetLastError();
			iResult = 30;	// 创建失败
		}
	}

	return 0;
}

string ws2s(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

	setlocale(LC_ALL, "chs"); 

	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	string result = _Dest;
	delete []_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

wstring s2ws(const string& s)
{
	setlocale(LC_ALL, "chs"); 

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	wstring result = _Dest;
	delete []_Dest;

	setlocale(LC_ALL, "C");

	return result;
}

#if 0//另外一种写法
template<class T>  
T Trim(const T& str)  
{  
	int length = str.size();  
	int i = 0,j = length -1;  
	// vc的isspace实现> 256就崩溃
	while(i < length && isspace(str[i] & 0xFF)){i++;}  
	while(j >= 0 && isspace(str[j] & 0xFF)){j--;}  
	if(j<i) return T();  
	return str.substr(i,j-i+1);  
}  
#else
template<typename E,typename TR,typename AL>
inline std::basic_string<E, TR, AL> Trim(const std::basic_string<E, TR, AL>&theString) 
{
	int aStartPos=0;
	while(aStartPos<(int)theString.length() && isspace(theString[aStartPos]))
		aStartPos++;
	int anEndPos=theString.length()-1;
	while(anEndPos>=0 && isspace(theString[anEndPos]))
		anEndPos--;
	return theString.substr(aStartPos,anEndPos-aStartPos+1);
}
#endif

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数

//定时器标识
#define IDI_DELAY_ENDGAME			10				//动作标识
#define IDI_ACTION					11				//行动标识

//时间标识
#define TIME_DELAY_ENDGAME			40
#define TIME_ACTION					30000
//////////////////////////////////////////////////////////////////////////

void TraceMessage(LPCTSTR pszMessage);

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//玩家变量
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	//m_wWinner = INVALID_CHAIR;

	//玩家状态
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
    //弃牌标志
	ZeroMemory(m_cbGiveUpFlag,sizeof(m_cbGiveUpFlag));

	//扑克变量
	m_cbSendCardCount = 0;
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//加注变量
	m_lCellScore = 0L;
	m_lTurnLessScore = 0L;
	m_lAddLessScore = 0L;
	m_lTurnMaxScore = 0L;
	m_wOperaCount = 0;
	m_cbBalanceCount = 0;
	m_lBalanceScore =0L;

	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_cbShowHand,sizeof(m_cbShowHand));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));//by hxh

	//税收变量
	//ZeroMemory(m_bUserTax,sizeof(m_bUserTax));
	//ZeroMemory(m_bLastTax,sizeof(m_bLastTax));

	m_Jushu=0;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
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

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//玩家变量
	m_wCurrentUser = INVALID_CHAIR;

	//玩家状态
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
    //弃牌标志
	ZeroMemory(m_cbGiveUpFlag,sizeof(m_cbGiveUpFlag));

	//扑克变量
	m_cbSendCardCount = 0;
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//加注变量
	m_lCellScore = 0L;
	m_lTurnLessScore = 0L;
	m_lTurnMaxScore = 0L;
	m_lAddLessScore = 0L;
	m_wOperaCount = 0;
	m_cbBalanceCount = 0;
	m_lBalanceScore = 0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_cbShowHand,sizeof(m_cbShowHand));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));//by hxh

	//税收变量
	//ZeroMemory(m_bUserTax,sizeof(m_bUserTax));	
	//ZeroMemory(m_bLastTax,sizeof(m_bLastTax));	

	return;
}

//用户坐下 
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户起来
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{	
	if(bLookonUser || pIServerUserItem->GetChairID()==INVALID_CHAIR)return true;
	WORD i=0;
	for ( i=0;i<m_wPlayerCount;i++)
	{
		if(i==pIServerUserItem->GetChairID())continue;
		if (m_pITableFrame->GetTableUserItem(i)!=NULL)
		{
			break;
		}
	}

	//庄家设置
	if(i==m_wPlayerCount)m_wDUser=INVALID_CHAIR;

	return true;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	m_Jushu++;
	if(m_Jushu<0)m_Jushu=0;
	createLogFile();
	
	//设置状态
	m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);

	//游戏变量
	WORD wUserCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//无效用户
		if (pIServerUserItem==NULL) continue;

		//获取积分
		ASSERT(pIServerUserItem->GetUserScore()>=m_pGameServiceOption->lCellScore);
		m_lUserMaxScore[i]=pIServerUserItem->GetUserScore();

		//设置状态
		m_cbPlayStatus[i]=TRUE;
		wUserCount++;
	}
	m_lCellScore=m_pGameServiceOption->lCellScore;

	DZPKConfig & cfg=DZPKConfig::Instance();
	if(cfg.m_Enable==1)
	{
		//用户扑克
		WORD wCardCount=0;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i]==TRUE)
			{
				CopyMemory(&m_cbHandCardData[i][0],&cfg.m_cbHandCardData[i][0],sizeof(BYTE)*MAX_COUNT);
			}
		}

		//中心扑克
		CopyMemory(m_cbCenterCardData,cfg.m_cbCenterCardData,CountArray(m_cbCenterCardData));           
	}
	else
	{
		//混乱扑克
		srand((unsigned int)time(NULL));
		BYTE cbRandCard[FULL_COUNT];
		ZeroMemory(cbRandCard,sizeof(cbRandCard));
		m_GameLogic.RandCardList(cbRandCard,wUserCount*MAX_COUNT+MAX_CENTERCOUNT);

		//用户扑克
		WORD wCardCount=0;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i]==TRUE)
			{
				CopyMemory(&m_cbHandCardData[i],&cbRandCard[(wCardCount++)*MAX_COUNT],sizeof(BYTE)*MAX_COUNT);
			}
		}

		//中心扑克
		CopyMemory(m_cbCenterCardData,&cbRandCard[wUserCount*MAX_COUNT],CountArray(m_cbCenterCardData));
	}

	//扑克数目
	m_cbSendCardCount = 0;
	m_cbBalanceCount = 0;

	//首家判断
	if(m_wDUser == INVALID_CHAIR)m_wDUser = 0;
	else m_wDUser =(m_wDUser+1)%GAME_PLAYER;

	//盲注玩家
	WORD wPlayer[]={INVALID_CHAIR,INVALID_CHAIR,INVALID_CHAIR,INVALID_CHAIR},wPlayerCount = 0;
	WORD wNextUser = m_wDUser;
	do
	{
		if (m_cbPlayStatus[wNextUser]==TRUE) 
		{
			wPlayer[wPlayerCount++] = wNextUser;	
		}
		wNextUser =(wNextUser+1)%GAME_PLAYER;
	}while(wPlayerCount < 4);

	m_wDUser = wPlayer[0];
	m_wCurrentUser = wPlayer[3];

	//当前下注
	m_lTableScore[wPlayer[1]] = m_lCellScore;
	m_lTableScore[wPlayer[2]] = 2*m_lCellScore;
	m_lTotalScore[wPlayer[1]] = m_lCellScore;
	m_lTotalScore[wPlayer[2]] = 2*m_lCellScore;

	//设置变量
	m_lBalanceScore = 2L*m_lCellScore;
	m_lTurnMaxScore = m_lUserMaxScore[m_wCurrentUser]-m_lTotalScore[m_wCurrentUser];
	m_lTurnLessScore = m_lBalanceScore - m_lTotalScore[m_wCurrentUser];
	m_lAddLessScore = 2L*m_lCellScore+m_lTurnLessScore;

	//构造变量
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.wDUser = m_wDUser;
	GameStart.wMinChipInUser = wPlayer[1];
	GameStart.wMaxChipInUser = wPlayer[2];
	GameStart.wCurrentUser = m_wCurrentUser;
	GameStart.lCellScore = m_pGameServiceOption->lCellScore;
	GameStart.lAddLessScore = m_lAddLessScore;
	GameStart.lTurnLessScore = m_lTurnLessScore;
	GameStart.lTurnMaxScore = m_lTurnMaxScore;

    CopyMemory(GameStart.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));
    CopyMemory(GameStart.lUserMaxScore,m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//作弊/漏洞数据
	//CopyMemory(GameStart.cbAllData,m_cbHandCardData,sizeof(m_cbHandCardData));
	std::stringstream ss;
	char sz1[1024]={0};
	sprintf(sz1,"游戏开始 庄家wDUser=%d,小盲注wMinChipInUser=%d,大盲注wMaxChipInUser=%d,当前玩家wCurrentUser=%d,单元下注lCellScore=%lld,加最小注lAddLessScore=%lld,最小下注lTurnLessScore=%lld,最大下注lTurnMaxScore=%lld", \
		GameStart.wDUser, \
		GameStart.wMinChipInUser, \
		GameStart.wMaxChipInUser, \
		GameStart.wCurrentUser, \
		GameStart.lCellScore, \
		GameStart.lAddLessScore, \
		GameStart.lTurnLessScore, \
		GameStart.lTurnMaxScore \
		);
	ss<<sz1<<"\n";

	char sz2[1024]={0};
	sprintf(sz2,"公共牌 %x(%s) %x(%s) %x(%s) %x(%s) %x(%s)", \
		m_cbCenterCardData[0], \
		GetCardName(m_cbCenterCardData[0]), \
		m_cbCenterCardData[1], \
		GetCardName(m_cbCenterCardData[1]), \
		m_cbCenterCardData[2], \
		GetCardName(m_cbCenterCardData[2]), \
		m_cbCenterCardData[3], \
		GetCardName(m_cbCenterCardData[3]), \
		m_cbCenterCardData[4], \
		GetCardName(m_cbCenterCardData[4]) \
		);
	ss<<sz2<<"\n";

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
	    ss<<"第"<<i<<"个玩家的手牌:";
		for (WORD j=0;j<MAX_COUNT;j++)
		{
			ss<<(int)m_cbHandCardData[i][j];
			ss<<"("<<GetCardName(m_cbHandCardData[i][j])<<"),";
		    //if(j < MAX_COUNT)
			   //ss << ",";
		}
		ss<<"用户游戏状态:"<<(int)GameStart.cbPlayStatus[i]<<","<<"用户金币数:"<<GameStart.lUserMaxScore[i]<<"\n";
	}
	string s = ss.str();
    ss.str("");
	printLog((char *)s.c_str());
	
	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			//发送数据
			CopyMemory(GameStart.cbCardData[i],m_cbHandCardData[i],MAX_COUNT);
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			ZeroMemory(GameStart.cbCardData[i],MAX_COUNT);
		}	
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	// 开启行动定时器
    m_pITableFrame->SetGameTimer(IDI_ACTION,TIME_ACTION,1,0L);

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	OutputDebugString(L"hhh 游戏结束");
	CString str;
	str.Format(L"hhh   cbReason==%d",cbReason);
	OutputDebugString(str);

	// 杀掉行动定时器
	m_pITableFrame->KillGameTimer(IDI_ACTION);

	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.cbTotalEnd = 1;

			//扑克数据
			BYTE cbEndCardData[GAME_PLAYER][MAX_CENTERCOUNT];
			ZeroMemory(cbEndCardData,sizeof(cbEndCardData));

			try{
				//获取扑克
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//用户过滤
					if (m_cbPlayStatus[i]==FALSE) continue;

					//最大牌型//BYTE CGameLogic::FiveFromSeven(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbCenterCardData[],BYTE cbCenterCardCount,BYTE cbLastCardData[],BYTE cbLastCardCount)
					GameEnd.cbLastCardKind[i] = m_GameLogic.FiveFromSeven(m_cbHandCardData[i],MAX_COUNT,m_cbCenterCardData,MAX_CENTERCOUNT,cbEndCardData[i],MAX_CENTERCOUNT);
					ASSERT(GameEnd.cbLastCardKind[i]!=FALSE);			
					CopyMemory(GameEnd.cbLastCenterCardData[i],cbEndCardData[i],sizeof(BYTE)*CountArray(cbEndCardData));
					

				}
			}catch(...)
			{
				TraceMessage(_TEXT("用户过滤v最大牌型"));
				ASSERT(FALSE);
			}
			
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (WORD j=0;j<MAX_COUNT;j++)
				{
					CString str;
					str.Format(L"hhh  jieshu pai %d",GameEnd.cbLastCenterCardData[i][j]);
					OutputDebugString(str);
				}
				
			}
		

			//总下注备份
			DZPKSCORE lTotalScore[GAME_PLAYER];
			ZeroMemory(lTotalScore,sizeof(lTotalScore));
			CopyMemory(lTotalScore,m_lTotalScore,sizeof(m_lTotalScore));
			
			//胜利列表
			UserWinList WinnerList[GAME_PLAYER];
			ZeroMemory(WinnerList,sizeof(WinnerList));

			//临时数据
			BYTE bTempData[GAME_PLAYER][MAX_CENTERCOUNT];
			CopyMemory(bTempData,cbEndCardData,GAME_PLAYER*MAX_CENTERCOUNT);

			WORD wWinCount=0;
			try{
				//用户得分顺序
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//查找最大用户
					if(!m_GameLogic.SelectMaxUser(bTempData,WinnerList[i],lTotalScore))
					{
						wWinCount=i;
						break;
					}

					//删除胜利数据
					for (WORD j=0;j<WinnerList[i].bSameCount;j++)
					{
						WORD wRemoveId=WinnerList[i].wWinerList[j];
						ASSERT(bTempData[wRemoveId][0]!=0);
						ZeroMemory(bTempData[wRemoveId],sizeof(BYTE)*MAX_CENTERCOUNT);
					}
				}
			}catch(...)
			{
				TraceMessage(_TEXT("用户得分顺序"));
				ASSERT(FALSE);
			}

			//强退用户
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE && lTotalScore[i]>0l)
				{					
					WinnerList[wWinCount].wWinerList[WinnerList[wWinCount].bSameCount++] = i;
				}
			}

			//得分变量
			DZPKSCORE lUserScore[GAME_PLAYER];
			ZeroMemory(lUserScore,sizeof(lUserScore));
			//CopyMemory(lTotalScore,m_lTotalScore,sizeof(m_lTotalScore));

			try
			{
				//得分情况
				for (int i=0;i<m_wPlayerCount-1;i++)
				{
					//胜利人数
					int iWinCount = (int)WinnerList[i].bSameCount;
					if(0 == iWinCount)break;

					//胜利用户得分情况
					for(int j=0;j<iWinCount;j++)
					{
						if(0 == lTotalScore[WinnerList[i].wWinerList[j]])continue;

						if(j>0 && lTotalScore[WinnerList[i].wWinerList[j]] - 
							lTotalScore[WinnerList[i].wWinerList[j-1]] == 0)continue;

						//失败用户失分情况
						for(int k=i+1;k<m_wPlayerCount;k++)
						{
							//失败人数
							if(0 == WinnerList[k].bSameCount)break;

							for(int l=0;l<WinnerList[k].bSameCount;l++)
							{
								//用户已赔空
								if(0 == lTotalScore[WinnerList[k].wWinerList[l]])continue;

								WORD wLostId=WinnerList[k].wWinerList[l];
								WORD wWinId=WinnerList[i].wWinerList[j];
								DZPKSCORE lMinScore = 0;

								//上家得分数目
								DZPKSCORE lLastScore = ((j>0)?lTotalScore[WinnerList[i].wWinerList[j-1]]:0);
								if(j>0)ASSERT(lLastScore>0L);							
								lMinScore = min(lTotalScore[wWinId]-lLastScore,lTotalScore[wLostId]);

								



								for(int m=j;m<iWinCount;m++)
								{
									//得分数目
									lUserScore[WinnerList[i].wWinerList[m]]+=lMinScore/(iWinCount-j);
								}

								//赔偿数目
								lUserScore[wLostId]-=lMinScore;
								lTotalScore[wLostId]-=lMinScore;
							}
						}
					}
				}
			}catch(...)
			{
				TraceMessage(_TEXT("得分数目/赔偿数目"));
				ASSERT(FALSE);
			}

			//统计用户分数(税收)
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.lGameScore[i]=lUserScore[i];
				ASSERT(lUserScore[i]+m_lTotalScore[i]>=0L);
				if(GameEnd.lGameScore[i]>0L)
				{
					GameEnd.lGameTax[i]=m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
					GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
				}
			}
		

			//输赢限制

			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));



			bool bDelayOverGame=false;
			IServerUserItem * pUserData;
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			//积分变量
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					BYTE nScoreKind=SCORE_TYPE_WIN;
					if(GameEnd.lGameScore[i]==0L)nScoreKind=SCORE_TYPE_DRAW;
					else nScoreKind=(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

					if(nScoreKind==SCORE_TYPE_LOSE)
					{
						pUserData=(IServerUserItem *)m_pITableFrame->GetTableUserItem(i);
						if(pUserData!=NULL)
						{
							if(bDelayOverGame==false && ((pUserData->GetUserScore()+GameEnd.lGameScore[i] )<m_pGameServiceOption->lMinTableScore)) 
	    						 bDelayOverGame=true;
						}
					}
					ScoreInfo[i].cbType = nScoreKind;
					ScoreInfo[i].lRevenue = GameEnd.lGameTax[i];
					ScoreInfo[i].lScore = GameEnd.lGameScore[i];

				}
			}
			
			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//结束游戏
			if(bDelayOverGame==false)
			   m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,NULL,608);
			else
			   m_pITableFrame->SetGameTimer(IDI_DELAY_ENDGAME,40,1,0L);

			return true;
		}
	case GER_NO_PLAYER:		//没有玩家
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.cbTotalEnd = 0;

			//效验结果
			WORD wUserCount=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]!=FALSE)wUserCount++;
			}
			if(wUserCount!=1)
			{
				ASSERT(FALSE);
				TraceMessage(_TEXT("没有玩家//效验结果出错"));
			}

			//统计分数
			double lScore = 0,lRevenue = 0;
			BYTE nScoreKind ;
			WORD wWinner = INVALID_CHAIR;
			for (WORD i = 0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)
				{
					if(m_lTotalScore[i] > 0L)GameEnd.lGameScore[i]-=m_lTotalScore[i];
					continue;
				}

				wWinner = i;

				//总下注数目
				DZPKSCORE lAllScore = 0L;
				for (WORD j = 0;j<GAME_PLAYER;j++)
				{
					if(wWinner==j)continue;
					lAllScore += m_lTotalScore[j];
				}
				ASSERT(lAllScore>=0);
				GameEnd.lGameScore[i] = lAllScore;

				//统计税收
				if(GameEnd.lGameScore[i]>0L)
				{
					//扣税变量
					//WORD cbRevenue=m_pGameServiceOption->wRevenueRatio;
					GameEnd.lGameTax[i]=m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
					GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
				}

				//构造扑克
				CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

				lScore=GameEnd.lGameScore[i];
				lRevenue= GameEnd.lGameTax[i];
				nScoreKind=(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
			}
			//各个玩家的最终牌型也发送过去
			BYTE cbEndCardData[GAME_PLAYER][MAX_CENTERCOUNT];
			ZeroMemory(cbEndCardData,sizeof(cbEndCardData));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				
				GameEnd.cbLastCardKind[i] = m_GameLogic.FiveFromSeven(m_cbHandCardData[i],MAX_COUNT,m_cbCenterCardData,MAX_CENTERCOUNT,cbEndCardData[i],MAX_CENTERCOUNT);
				ASSERT(GameEnd.cbLastCardKind[i]!=FALSE);			
			}

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			//写入积分
			if(wWinner<GAME_PLAYER)
			{
				ScoreInfo[wWinner].cbType = nScoreKind;
				ScoreInfo[wWinner].lRevenue = lRevenue;
				ScoreInfo[wWinner].lScore = lScore;
				m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			}
			else TraceMessage(_TEXT("//写入积分ffff"));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,NULL,608);

			return true;
		}
	case GER_USER_LEAVE:
	case GER_NETWORK_ERROR:
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//强退处理
			OnUserGiveUp(wChairID,true);
			return true;
		}
	}
	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//游戏变量
			StatusFree.lCellMinScore=m_pGameServiceOption->lCellScore;
			StatusFree.lCellMaxScore=m_pGameServiceOption->lRestrictScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));	
		}
	case GAME_STATUS_PLAY:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//标志变量
			//CopyMemory(StatusPlay.bShowHand,m_cbShowHand,sizeof(m_cbShowHand));

			//总下注数目
			DZPKSCORE lAllScore = 0L;
			for (WORD j = 0;j<GAME_PLAYER;j++)
			{
				lAllScore += m_lTotalScore[j];
				lAllScore -= m_lTableScore[j];
			}
			ASSERT(lAllScore>=0);

			//加注信息
			StatusPlay.lAddLessScore = m_lAddLessScore;
			StatusPlay.lCellScore	 = m_lCellScore;
			StatusPlay.lTurnMaxScore = m_lTurnMaxScore;
			StatusPlay.lTurnLessScore= m_lTurnLessScore;
			StatusPlay.lCenterScore	 = lAllScore;
			StatusPlay.lCellMaxScore = m_pGameServiceOption->lRestrictScore;
			StatusPlay.cbBalanceCount= m_cbBalanceCount;

			//扑克信息
			CopyMemory(StatusPlay.cbHandCardData,m_cbHandCardData[wChairID],MAX_COUNT);
			if(m_cbBalanceCount>0)
			{
				CopyMemory(StatusPlay.cbCenterCardData,m_cbCenterCardData,sizeof(m_cbCenterCardData));
			}

			//设置变量
			StatusPlay.wDUser = m_wDUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;//当前玩家
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(m_lTableScore));//下注数目
			CopyMemory(StatusPlay.lTotalScore,m_lTotalScore,sizeof(m_lTotalScore));//下注数目
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));//用户游戏状态
            CopyMemory(StatusPlay.lUserMaxScore,m_lUserMaxScore,sizeof(m_lUserMaxScore));//用户金币数<by hxh>

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
	case IDI_DELAY_ENDGAME:		//动作标识
		{
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,NULL,608);
		    m_pITableFrame->KillGameTimer(IDI_DELAY_ENDGAME);
			return true;
		}
	case IDI_ACTION:		//行动标识
		{
			//超时弃牌处理
			OnUserGiveUp(m_wCurrentUser,false);
			return true;
		}
	}
	return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch(wSubCmdID)
	{
	case SUB_C_GIVE_UP:		//用户放弃
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//状态判断
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//消息处理
			return OnUserGiveUp(pUserData->wChairID);
		}
	case SUB_C_ADD_SCORE:	//用户加注	
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//变量定义
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//状态判断
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//消息处理
			return OnUserAddScore(pUserData->wChairID,pAddScore->lScore,false,pAddScore->cbJudgeAction);
		}
	case SUB_C_OPEN_CARD:	//开牌
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//状态判断
			//ASSERT(m_cbPlayStatus[pUserData->wChairID]!=TRUE);
			//if (m_cbPlayStatus[pUserData->wChairID]!=FALSE) return false;

			//发送数据
			CMD_S_OpenCard OpenCard;
			OpenCard.wWinUser = pUserData->wChairID;			
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	

			return true;
		}
	//case SUB_C_SIT:			//坐下
	//	{
	//		tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
	//		CMD_S_SitDown sitUser;
	//		sitUser.wSitUser = pUserData->wChairID;
	//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SIT_DOWN,&sitUser,sizeof(sitUser));
	//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SIT_DOWN,&sitUser,sizeof(sitUser));
	//		
	//	}
	//case SUB_C_READY:			//准备
	//	{
	//		tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
	//		CMD_S_Ready readyUser;
	//		readyUser.wReadyUser = pUserData->wChairID;
	//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SIT_DOWN,&readyUser,sizeof(readyUser));
	//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SIT_DOWN,&readyUser,sizeof(readyUser));

	//	}
	}
	return false;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//数据事件
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//放弃事件
bool CTableFrameSink::OnUserGiveUp(WORD wChairID,bool bLeft)
{
    // add by hxh 20161104
	if (m_cbGiveUpFlag[wChairID]==TRUE) 
		return false;

    m_cbGiveUpFlag[wChairID]=TRUE;
	
	//重置状态
	m_cbPlayStatus[wChairID]=FALSE;
	m_cbShowHand[wChairID]=FALSE;

	//发送消息
	CMD_S_GiveUp GiveUp;
	GiveUp.wGiveUpUser=wChairID;
	GiveUp.lLost = -m_lTotalScore[wChairID];
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//写入积分
	tagScoreInfo ScoreInfo[GAME_PLAYER];
	ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo[wChairID].lScore = -m_lTotalScore[wChairID];
	ScoreInfo[wChairID].cbType = (bLeft?SCORE_TYPE_FLEE:SCORE_TYPE_LOSE);
	m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

	//清空下注
	m_lTableScore[wChairID] = 0L;

	//人数统计
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//判断结束
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddScore(wChairID,0L,true,GAME_ACTION_GIVEUP);//添加最后一个放弃的参数
	}
	else OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	return true;
}

//加注事件 
bool CTableFrameSink::OnUserAddScore(WORD wChairID, DZPKSCORE lScore, bool bGiveUp,BYTE cbJudgeAction)
{
	//校验用户
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false; 

	//校验金币
	ASSERT((lScore +m_lTotalScore[wChairID])<= m_lUserMaxScore[wChairID]);	
	if ((lScore+m_lTotalScore[wChairID])>m_lUserMaxScore[wChairID]) return false;
	ASSERT(lScore>=0L);
	if ((lScore<0)) return false;

	//累计金币
	m_lTableScore[wChairID] += lScore;
	m_lTotalScore[wChairID] += lScore;

	//平衡下注
	if(m_lTableScore[wChairID] > m_lBalanceScore )
	{
		m_lBalanceScore = m_lTableScore[wChairID];
	}

	//梭哈判断
	if(m_lTotalScore[wChairID]==m_lUserMaxScore[wChairID])
	{
		m_cbShowHand[wChairID] = TRUE;
	}

	//用户切换
	WORD wNextPlayer=INVALID_CHAIR;
	for (WORD i=1;i<m_wPlayerCount;i++)
	{
		//设置变量
		m_wOperaCount++;
		wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

		//继续判断
		if ((m_cbPlayStatus[wNextPlayer]==TRUE) &&(m_cbShowHand[wNextPlayer] == FALSE)) break;
	}
	ASSERT(wNextPlayer < m_wPlayerCount);

	//完成判断
	bool bFinishTurn=false;
	if (m_wOperaCount>=m_wPlayerCount)
	{
		WORD i=0;
		for ( i=0;i<m_wPlayerCount;i++)
		{
			//过滤未平衡 和未梭哈用户
			if ((m_cbPlayStatus[i]==TRUE)&&(m_lTableScore[i]<m_lBalanceScore)&&(m_cbShowHand[i]==FALSE)) 
				break;
		}
		if (i==m_wPlayerCount) 
			bFinishTurn=true;
	}

	//A家show190,B放弃,C还选择?
	if(!bFinishTurn)
	{
		WORD wPlayCount = 0,wShowCount = 0;
		for (BYTE i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==TRUE)
			{
				if(m_cbShowHand[i]==TRUE)
				{
					wShowCount++;
				}
				wPlayCount++;
			}
		}
		if(wPlayCount-1==wShowCount && m_lTableScore[wNextPlayer]>=m_lBalanceScore) bFinishTurn=true;
	}

	//继续加注
	if (!bFinishTurn)
	{
		//当前用户
		m_wCurrentUser=wNextPlayer;

		//最小值为平衡下注 -桌面下注  和 剩余金币中取小 可能梭哈
		m_lTurnLessScore = min(m_lBalanceScore - m_lTableScore[m_wCurrentUser],m_lUserMaxScore[m_wCurrentUser] - m_lTotalScore[m_wCurrentUser]);
		m_lTurnMaxScore = m_lUserMaxScore[m_wCurrentUser]-m_lTotalScore[m_wCurrentUser];
		if(m_lTotalScore[m_wCurrentUser]==m_lCellScore)
		{
			DZPKSCORE bTemp = (m_lBalanceScore==m_lCellScore*2)?(m_lCellScore*2):((m_lBalanceScore-m_lCellScore*2)*2);
			m_lAddLessScore = m_lCellScore+bTemp;
		}
		else m_lAddLessScore = (m_lBalanceScore==0)?(2*m_lCellScore):(__max((m_lBalanceScore-m_lTableScore[m_wCurrentUser])*2,2L*m_lCellScore));

		//构造数据
		CMD_S_AddScore AddScore;
		ZeroMemory(&AddScore,sizeof(AddScore));
		AddScore.lAddScoreCount=lScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.wCurrentUser=m_wCurrentUser;
		AddScore.lTurnLessScore=m_lTurnLessScore;
		AddScore.lTurnMaxScore = m_lTurnMaxScore;
		AddScore.lAddLessScore = m_lAddLessScore;
		AddScore.cbJudgeAction = cbJudgeAction;

		//发送数据
		SendAddScorePacket(&AddScore);

		return true;
	}

	//平衡次数
	m_cbBalanceCount++;
	m_wOperaCount=0;

	//第1次下注平衡后就开始发给三张公牌
	//第2次下注平衡后就开始发第四张公牌
	//第3次下注平衡后就开始发第五张公牌
	//第4次下注平衡后就结束游戏 

	//D家下注
	WORD wDUser=m_wDUser;
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		wDUser=(m_wDUser+i)%GAME_PLAYER;		
		if(m_cbPlayStatus[wDUser]==TRUE && m_cbShowHand[wDUser]==FALSE) break;
	}

	//重值变量
	m_lBalanceScore = 0L;
	m_lTurnLessScore = 0L;
	m_lTurnMaxScore = m_lUserMaxScore[wDUser]-m_lTotalScore[wDUser];
	m_lAddLessScore = 2*m_lCellScore;

	//构造数据
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore,sizeof(AddScore));
	AddScore.wAddScoreUser=wChairID;
	AddScore.wCurrentUser=INVALID_CHAIR;
	AddScore.lAddScoreCount=lScore;
	AddScore.lTurnLessScore=m_lTurnLessScore;
	AddScore.lTurnMaxScore = m_lTurnMaxScore;
	AddScore.lAddLessScore = m_lAddLessScore;
	AddScore.cbJudgeAction = cbJudgeAction; 

	//清理数据
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_lBalanceScore = 0L;

	//发送数据
	SendAddScorePacket(&AddScore);

	//结束判断
	if (m_cbBalanceCount == 4) 
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
		return true;
	}

	//梭哈用户统计
	WORD wShowHandCount=0,wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbShowHand[i]==TRUE)	wShowHandCount++;
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//只剩一玩家没梭或者全梭
	if((wShowHandCount >= wPlayerCount -1) && m_cbBalanceCount < 4)
	{
		//构造数据
		CMD_S_SendCard SendCard;
		ZeroMemory(&SendCard,sizeof(SendCard));
		SendCard.cbPublic = m_cbBalanceCount;
		SendCard.wCurrentUser = INVALID_CHAIR;
		SendCard.cbSendCardCount = MAX_CENTERCOUNT;
		CopyMemory(SendCard.cbCenterCardData,m_cbCenterCardData,sizeof(BYTE)*MAX_CENTERCOUNT);

		//发送数据
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

		//结束游戏
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

		return true;
	}

	//盲注玩家
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//临时变量
		BYTE cbNextUser =(m_wDUser+i)%GAME_PLAYER;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(cbNextUser);

		//无效用户 梭哈用户过滤
		if (pIServerUserItem==NULL||m_cbPlayStatus[cbNextUser] == FALSE||m_cbShowHand[cbNextUser] == 1) 
			continue;

		m_wCurrentUser = cbNextUser;
		break;
	}

	//构造数据
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard,sizeof(SendCard));
	SendCard.cbPublic = 0;
	SendCard.wCurrentUser = m_wCurrentUser;
	SendCard.cbSendCardCount = 3 +(m_cbBalanceCount-1);
	CopyMemory(SendCard.cbCenterCardData,m_cbCenterCardData,sizeof(BYTE)*(SendCard.cbSendCardCount));

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

void CTableFrameSink::SendAddScorePacket(CMD_S_AddScore *pAddScore)
{
	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,pAddScore,sizeof(CMD_S_AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,pAddScore,sizeof(CMD_S_AddScore));

	// 杀掉行动定时器
	m_pITableFrame->KillGameTimer(IDI_ACTION);
	// 开启行动定时器
    m_pITableFrame->SetGameTimer(IDI_ACTION,TIME_ACTION,1,0L);
}

//输出信息
void TraceMessage(LPCTSTR pszMessage)
{
	CFile File;
	if ((File.Open(TEXT("TraceData.txt"),CFile::modeWrite)==FALSE)&&
		(File.Open(TEXT("TraceData.txt"),CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	File.SeekToEnd();
	File.Write(pszMessage,lstrlen(pszMessage));
	File.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));

	File.Flush();
	File.Close();

	return;
}

void CTableFrameSink::createLogFile()
{
	WORD tableId=m_pITableFrame->GetTableID();
	WORD wKindID=m_pGameServiceOption->wKindID;
	WORD wServerKind=m_pGameServiceOption->wServerKind;
	WORD wServerID=m_pGameServiceOption->wServerID;
	wstring szServerName=m_pGameServiceOption->szServerName;
#ifdef USE_RS_PRINT
	wchar_t g_wcDirName[256]={0};
	wchar_t g_wcFileName[256]={0};
	wstring wstrPath=s2ws(GetAppPath());
	wsprintfW(g_wcDirName,L"%s\\%s_%d\\第%d桌", 
		wstrPath.c_str(),
		szServerName.c_str(),
		wServerID,
		tableId
		);
	wsprintfW(g_wcFileName,L"\\第%d局.txt", 
		m_Jushu
		);
	m_strLogFile=g_wcDirName;
	m_strLogFile+=g_wcFileName;
	int ret=create_dir(g_wcDirName,wcslen(g_wcDirName));
#endif
}

void CTableFrameSink::printLog(char *szBuff,...)
{
#ifdef USE_RS_PRINT
	if(m_strLogFile!=L"")
	{
		SYSTEMTIME st={0};
		GetSystemTime(&st);	// 获得当前时间
		char timeFormat[256]={0};
		sprintf(timeFormat,"【%04d-%02d-%02d %02d:%02d:%02d %03d】", 
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour+8,	// 加8是对齐北京时间
			st.wMinute,
			st.wSecond,
			st.wMilliseconds
			);

		char buf[1024]={0};
		va_list ap;
		va_start(ap,szBuff);
		vsprintf(buf,szBuff,ap);
		va_end(ap);

		//CTraceService::TraceString(CA2T(buf),TraceLevel_Normal);
		OutputDebugString(CA2T(buf));
		ofstream outf(m_strLogFile.c_str(),ios::app); 
		outf<<timeFormat<<buf<<endl;
	}
#endif
}

void CTableFrameSink::printLog(CString& str)
{
	printLog((LPSTR)CT2A(str));
}
//////////////////////////////////////////////////////////////////////////
