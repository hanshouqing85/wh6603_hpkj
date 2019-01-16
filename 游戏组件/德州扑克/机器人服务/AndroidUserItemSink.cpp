#include "Stdafx.h"
#include "AndroidUserItemSink.h"

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

//定时器标识
#define IDI_CHIP_OP                 (1)           //行动定时器
#define IDI_START_GAME				(2)									
#define IDI_BANK_OPERATE			(3)                   
//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	ZeroMemory(m_bUserName,sizeof(m_bUserName));
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_bOpenCard = false;
	m_bExitTag	= false;

	//加注信息
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;
	m_bGiveUp = false;
	m_bShowHand = false;
	m_bWillWin = false;
	m_lBeforeScore = 0;
	
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//扑克变量
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbOverCardData,sizeof(m_cbOverCardData));

	//状态变量
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	m_bAutoStart = FALSE;
	m_bReset =true;
	//接口变量
	m_pIAndroidUserItem=NULL;

	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}
  //初始接口
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}
//重置接口
bool  CAndroidUserItemSink::RepositionSink()
{
	ZeroMemory(m_bUserName,sizeof(m_bUserName));
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_bOpenCard = false;
	m_bExitTag	= false;
	m_bGiveUp = false;

	//加注信息
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;

	m_lBeforeScore = 0;

	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//扑克变量
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbOverCardData,sizeof(m_cbOverCardData));

	//状态变量
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	m_bAutoStart = FALSE;
	m_bReset =true;
	return true;
}

//时间消息
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	m_pIAndroidUserItem->KillGameTimer(nTimerID);
	switch (nTimerID)
	{
	case IDI_START_GAME:
		{
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_CHIP_OP:
		{
			if(m_bGiveUp == true) return true;  //如果用户已经放弃的话退出

 			OnSubAddScoreEx();   

			return true;
		}
	case IDI_BANK_OPERATE:		//银行操作
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			//变量定义
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			LONGLONG lRobotScore = pUserItem->GetUserScore();			
			{

				//判断存取
				if (lRobotScore > m_lRobotScoreRange[1])
				{
					LONGLONG lSaveScore=0L;

					lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0)
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

				}
				else if (lRobotScore < m_lRobotScoreRange[0])
				{
					DZPKSCORE lScore=m_lRobotBankGetScore+m_lRobotBankGetScoreBanker/(rand()%3+3);
					if (lScore > 0)
						m_pIAndroidUserItem->PerformTakeScore(lScore);
				}
			}
			return false;
		}
	default:
		{
			break;
		}
	}

	return false;
}

//游戏消息
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADMIN_GET_CARD:		//所有玩家的牌型名次数据
		{
			return OnSubGetAllCard(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//用户加注
		{
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//发送扑克
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:			//用户放弃
		{
            return OnSubGiveUp(pBuffer,wDataSize);
		}
	}

	return true;
}

bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wCmdID, void * pData, WORD wDataSize)
{
	BYTE mid=LOBYTE(wCmdID);
	BYTE sid=HIBYTE(wCmdID);

	//登录完成
	if ((mid==MDM_GR_LOGON)&&(sid==SUB_GR_LOGON_FINISH))
	{
		//变量定义
		tagAndroidUserParameter * pAndroidUserParameter=(tagAndroidUserParameter *)pData;
		createLogFile(pAndroidUserParameter);
		IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
		if(pMeUserItem)
		{
			WORD chairId0=m_pIAndroidUserItem->GetChairID();
			WORD uid0=m_pIAndroidUserItem->GetUserID();//uid0的值是错的
			WORD tableId0=m_pIAndroidUserItem->GetTableID();
			WORD chairId=pMeUserItem->GetChairID();
			DWORD uid=pMeUserItem->GetUserID();//uid的值是对的
			WORD tableId=pMeUserItem->GetTableID();
			printLog("登录完成chairId0=%d,uid0=%d,tableId0=%d,chairId=%d,uid=%d,tableId=%d",chairId0,uid0,tableId0,chairId,uid,tableId);
		}
		return true;
	}


	return true;
}

//场景消息
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//变量定义
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			//消息处理
			ReadConfigInformation();
			//玩家设置
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=CGameLogic::GetRand(1,3);
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	case GAME_STATUS_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

	/*		m_lCellScore = pStatusPlay->lCellScore;

			m_lDrawMaxScore = pStatusPlay->lDrawMaxScore;
			m_lTurnLessScore = pStatusPlay->lTurnLessScore;
			m_lTurnMaxScore = pStatusPlay->lTurnMaxScore;*/

			//memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));
			ReadConfigInformation();
		
			return true;
		}
	
	}
	return false;
}

//用户进入
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void  CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//设置变量
	m_wDUser = pGameStart->wDUser;
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lAddLessScore = pGameStart->lAddLessScore;
	m_lTurnLessScore = pGameStart->lTurnLessScore;
	m_lTurnMaxScore = pGameStart->lTurnMaxScore;
	m_lCellScore = pGameStart->lCellScore;  //单元倍数

	//加注信息
	m_lTableScore[pGameStart->wDUser] += m_lCellScore;
	m_lTableScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;
	m_lTotalScore[pGameStart->wDUser] =  m_lCellScore;
	m_lTotalScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;

	m_bShowHand = false;
	m_bGiveUp = false;
	m_nCurCardCount = 2;

	//复制暗牌
	CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(m_cbHandCardData));
	
	return true;
}

//所有玩家的牌型名次数据
bool CAndroidUserItemSink::OnSubGetAllCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize<=sizeof(CMD_S_GetAllCard)) return false;
	CMD_S_GetAllCard * pAllCard=(CMD_S_GetAllCard *)pBuffer;

	m_vAllCard.clear();

	std::stringstream ss;
	IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pMeUserItem)
	{
		WORD chairId0=m_pIAndroidUserItem->GetChairID();
		WORD uid0=m_pIAndroidUserItem->GetUserID();//uid0的值是错的
		WORD tableId0=m_pIAndroidUserItem->GetTableID();
		WORD chairId=pMeUserItem->GetChairID();
		DWORD uid=pMeUserItem->GetUserID();//uid的值是对的
		WORD tableId=pMeUserItem->GetTableID();

		char sz1[1024]={0};
		sprintf(sz1,"OnSubGetAllCard所有玩家的牌型名次数据 chairId0=%d,uid0=%d,tableId0=%d,chairId=%d,uid=%d,tableId=%d",chairId0,uid0,tableId0,chairId,uid,tableId);
		ss<<sz1<<"\n";

		for(int i=0;i<pAllCard->cbCount;i++)
		{
		   tagMadeHandsOrder obj=(*pAllCard)[i];
		   m_vAllCard.push_back(obj);
		   if(i==0)
		   {
		      m_bWillWin=(obj.wChairID==chairId);//是否当前机器人赢
		   }
	       string strName;
		   IServerUserItem * pItem=m_pIAndroidUserItem->GetTableUserItem(obj.wChairID);
		   if(pItem)
		   {
			   TCHAR szNickName[LEN_NICKNAME]={0};
			   _sntprintf(szNickName,CountArray(szNickName),TEXT("%s"),pItem->GetNickName());
			   strName=ws2s(szNickName);
			   strName=Trim(strName);
		   }
		   ss<<"第"<<i<<"个玩家"<<strName.c_str()<<"(chairId="<<obj.wChairID<<",uid="<<obj.dwUserID<<")的牌型名次数据：order="<<obj.order<<",cbLastCardKind="<<(int)obj.cbLastCardKind<<"\n";
		}
		string s = ss.str();
		ss.str("");
		printLog((char *)s.c_str());
	}

	if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=CGameLogic::GetRand(5,7);
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse);
	}

	return true;
}

//加注消息
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//变量定义
	WORD wAddScoreUser=pAddScore->wAddScoreUser;

	//设置变量
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;
	m_lTurnMaxScore = pAddScore->lTurnMaxScore;
	m_lAddLessScore = pAddScore->lAddLessScore;

#if _MSC_VER == 1400
	if(abs((double)pAddScore->lAddScoreCount-m_lTurnMaxScore)<0.001)
		m_bShowHand = true;
#else
	if(abs(pAddScore->lAddScoreCount-m_lTurnMaxScore)<0.001)
		m_bShowHand = true;
#endif

	m_lBeforeScore = pAddScore->lAddScoreCount;

	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=CGameLogic::GetRand(3,5);
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse);
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
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;

	//变量定义
	WORD chairId0=m_pIAndroidUserItem->GetChairID();
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;
	if(m_lTableScore[wGiveUpUser]!=0L)
	{
		m_lTableScore[wGiveUpUser] = 0L;
	}

	//状态变量
	if(wGiveUpUser==chairId0) 
		m_bGiveUp = true;

	for(vector<tagMadeHandsOrder>::iterator it=m_vAllCard.begin(); it!=m_vAllCard.end();)
	{
		if(it->wChairID == wGiveUpUser)
		{
			it=m_vAllCard.erase(it);
		}
		else
		{
			++it;
		}
	}

	int iCount=m_vAllCard.size();
	for(int i=0;i<iCount;i++)
	{
	    m_vAllCard[i].order=i;
		if(i==0)
		{
			m_bWillWin=(chairId0==m_vAllCard[i].wChairID);//是否当前机器人赢
		}
	}

	return true;
}

//发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//校验数据
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//当前玩家
	m_wCurrentUser = pSendCard->wCurrentUser;
	CopyMemory(m_cbCenterCardData,pSendCard->cbCenterCardData,sizeof(BYTE)*(pSendCard->cbSendCardCount));

/*	if((pSendCard->cbSendCardCount < 3))
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,rand()%TIME_START_GAME+2);
	}*/

	m_nCurCardCount = pSendCard->cbSendCardCount;
	
	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=CGameLogic::GetRand(1,2);
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse);
	}

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	UINT nElapse=CGameLogic::GetRand(5,7);
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
	
	return true;
}

//开牌消息
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////
/////所有操作
void CAndroidUserItemSink::OnSubAddScoreEx()
{
	//梦成网络修改
	srand((unsigned)time(NULL));//设置随机数种子
	//获取当前机器人用户
	WORD MeChairID=m_pIAndroidUserItem->GetChairID();


	//如果用户梭哈的话,怎么办?
	if(m_bWillWin)
	{
		TraceString(_T("赢"),TraceLevel_Info);  //输出信息
	}else{
	
		TraceString(_T("输"),TraceLevel_Info);  //输出信息
	}
	
	switch(m_nCurCardCount)   //当前牌数
	{
	case 2:     //刚发牌   //第一张牌不应该放弃
		{
				TraceString(_T("没有发牌"),TraceLevel_Info);  //输出信息
				if(m_bWillWin)     //机器人胜利
				{
					if (GaiLv(50))	//一个底牌一个名牌如果牌面上的牌小于玩家的牌面有百分之70跟注
					{
						FollowScore();
						return;
					}
					else 
					{
						//加注
						AddScore();
						return;
					}
				}
				else  //机器人输
				{
					if (GaiLv(80))	//百分之80跟注
					{
						FollowScore();
						return;
					}
					else 
					{
						//这里是放弃或加注
						if (GaiLv(80))  //机器人输,百分之80加注
						{
							AddScore();
							return;
						}
						else	//give up
						{
							TraceString(_T("机器人放弃1"),TraceLevel_Info);  //输出信息
							GiveUpScore();//机器人输机器人弃牌
							return;
						}
					}
				}


			
		}

	case 3:  //放送第3张牌
		{
			
					TraceString(_T("第一次发牌"),TraceLevel_Info);  //输出信息
					if(m_bWillWin)  //机器人胜利
					{
						if (GaiLv(50)) 
						{
							FollowScore();
							return;
						}
						else
						{

							//加注
							AddScore();
							return;

						}
					}
					else   //机器人输   
					{
						if (GaiLv(80))    //百分之80 跟注
						{
							FollowScore();  
							return;
						}
						else
						{
							if (GaiLv(80))      //百分之80加注
							{
								AddScore();
								return;
							}
							else
							{
							    TraceString(_T("机器人放弃2"),TraceLevel_Info);  //输出信息
								GiveUpScore();  //放弃
								return;
							}
						}
					}

				
			
		}
	case 4:  //放送第4张牌
		{
			TraceString(_T("第四次发牌"),TraceLevel_Info);  //输出信息
			if (m_bShowHand) //如果有用户梭哈
			{
				if(m_bWillWin)			//可器人胜利
				{


						FollowScore();//如果牌面上的牌大于玩家牌面上的牌有90%的跟注
						return;

				}
				else    //机器人输
				{
					if (GaiLv(5))      //百分之5可能会跟
					{
						FollowScore();//如果牌面上的牌大于玩家牌面上的牌有90%的跟注
						return;
					}
					else            
					{
						TraceString(_T("机器人放弃3"),TraceLevel_Info);  //输出信息
						GiveUpScore();//如果牌面上的牌大于玩家牌面上的牌有10%放弃
						return;
					}
				}
			}
			else        //没有用户梭哈
			{
				if(m_bWillWin)     //用户胜利
				{
						if (GaiLv(10))     //百分之10梭哈
						{
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
									FollowScore();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-80）×30=6%跟注			
									return;
								}
						}
						
					
				}
				else    //机器人输
				{
					if (GaiLv(10)) //如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有30%跟注
					{
						FollowScore();
						return;
					}
					else
					{
						TraceString(_T("机器人放弃4"),TraceLevel_Info);  //输出信息
						GiveUpScore();//如果不是梭哈标准，如果机器人的牌面小于玩家牌面就有70%放弃
						return;
					}
						
				}
			}
				
		}
		
	case 5:    //一共有五张牌
		{
		
			TraceString(_T("第三次发牌"),TraceLevel_Info);  //输出信息
			if (m_bShowHand)    //有用户梭哈
			{
				if(m_bWillWin)      //机器人胜利
				{
						FollowScore();	//如果是梭哈标准，如果机器人的牌面大于玩家牌面就有60%的概率跟注		
						return;
				}
				else
				{
						TraceString(_T("机器人放弃5"),TraceLevel_Info);  //输出信息
						GiveUpScore();//如果是梭哈标准，如果机器人的牌面小于玩家牌面就有90%的概率放弃
						return;	

				}

			}
			else   //没有用户梭哈
			{
				if(m_bWillWin)     //机器人胜利
				{
				
						if (GaiLv(20))
						{
							SendSuoHand();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-30）×90=63%的概率梭哈			
							return;
						}
						else
						{
							if (GaiLv(65))
								AddScore();	
							else
								FollowScore();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-30）×10=7%的概率跟注				
								
							return;
						}

				}
				else      //机器人输
				{

							if (GaiLv(20))
							{
									FollowScore();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-30）×90=63%的概率梭哈			
									return;
							}
							else
							{
									TraceString(_T("机器人放弃6"),TraceLevel_Info);  //输出信息
									GiveUpScore();	//如果不是梭哈标准，如果机器人的牌面大于玩家牌面就有（100-30）×10=7%的概率跟注				
									return;
							}

				
				}
			}
		}
		ASSERT(false);
		
	}
}
void CAndroidUserItemSink::GiveUpScore()
{
		m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
		return;
}

void CAndroidUserItemSink::FollowScore()
{
	if(m_lTurnLessScore<=0.001)
	{
		CMD_C_AddScore AddScore;
		AddScore.lScore=SCORE_ZERO;
		AddScore.cbJudgeAction=GAME_ACTION_PASS;
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
	}
	else
	{
		CMD_C_AddScore AddScore;
		AddScore.lScore=m_lTurnLessScore;
		AddScore.cbJudgeAction=GAME_ACTION_FOLLOW;
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));	
	}
	return;
}

void CAndroidUserItemSink::AddScore()
{
	DZPKSCORE lCurrentAddScore = m_lBeforeScore+(rand()%3+1)*m_lCellScore;
	if(lCurrentAddScore<m_lAddLessScore)
		lCurrentAddScore = m_lAddLessScore;

	//发送消息
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentAddScore;
	AddScore.cbJudgeAction=GAME_ACTION_ADD;
	m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
	return;
}

void CAndroidUserItemSink::SendSuoHand()
{
		DZPKSCORE lCurrentScore = m_lTurnMaxScore;
		//发送消息
		CMD_C_AddScore AddScore;
		AddScore.lScore=lCurrentScore;
	    AddScore.cbJudgeAction=GAME_ACTION_ALLIN;
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
	if(iWinner==m_pIAndroidUserItem->GetChairID())
		return true;
	return false;
}
WORD CAndroidUserItemSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	return 1;
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
	_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\DZShowHandConfig.ini"),szPath);

	//分数限制
	m_lRobotScoreRange[0] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMin"), 100, szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMax"), 100000, szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//提款数额
	m_lRobotBankGetScore = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGet"), 20000000, szConfigFileName);

	//提款数额 
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGetBanker"), 30000000, szConfigFileName);

	//存款倍数
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

}

void CAndroidUserItemSink::createLogFile(tagAndroidUserParameter * pAndroidUserParameter)
{
	tagAndroidService *pAndroidService = m_pIAndroidUserItem->GetAndroidService();
	tagAndroidParameter *pAndroidParameter = m_pIAndroidUserItem->GetAndroidParameter();
	IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pMeUserItem && pAndroidUserParameter)
	{
		WORD chairId=pMeUserItem->GetChairID();
		DWORD uid=pMeUserItem->GetUserID();//uid的值是对的
		WORD tableId=pMeUserItem->GetTableID();
		wstring wstr=pMeUserItem->GetNickName();
		wstr=Trim(wstr);
		string str=ws2s(wstr);
		WORD wKindID=pAndroidUserParameter->pGameServiceAttrib->wKindID;
		WORD wServerKind=pAndroidUserParameter->pGameServiceAttrib->wServerKind;
		WORD wServerID=pAndroidUserParameter->pGameServiceOption->wServerID;
		wstring szServerName=pAndroidUserParameter->pGameServiceOption->szServerName;
#ifdef USE_RS_PRINT
		wchar_t g_wcDirName[256]={0};
		wchar_t g_wcFileName[256]={0};
		wstring wstrPath=s2ws(GetAppPath());
		//wsprintfW(g_wcDirName,L"%s\\%s_%d_第%d桌", 
		//	wstrPath.c_str(),
		//	szServerName.c_str(),
		//	wServerID,
		//	tableId
		//	);
		wsprintfW(g_wcDirName,L"%s\\%s_%d", 
			wstrPath.c_str(),
			szServerName.c_str(),
			wServerID
			);
		wsprintfW(g_wcFileName,L"\\%s_%d.txt", 
			wstr.c_str(),
			uid
			);
		m_strLogFile=g_wcDirName;
		m_strLogFile+=g_wcFileName;
		int ret=create_dir(g_wcDirName,wcslen(g_wcDirName));
#endif
	}
}

void CAndroidUserItemSink::printLog(char *szBuff,...)
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

		OutputDebugString(CA2T(buf));
		ofstream outf(m_strLogFile.c_str(),ios::app); 
		outf<<timeFormat<<buf<<endl;
	}
#endif
}

void CAndroidUserItemSink::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	CString str=pszString;
	printLog((LPSTR)CT2A(str));
	CTraceService::TraceString(pszString,TraceLevel);
}