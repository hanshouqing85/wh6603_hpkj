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

//////////////////////////////////////////////////////////////////////////
//辅助时间
#define TIME_LESS					2									//最少时间
//定时器标识
#define IDI_START_GAME				200									//开始定时器
#define IDI_USER_ADD_SCORE			201									//加注定时器

//时间标识
#define TIME_START_GAME				10	/*20*/								//开始定时器
#define TIME_USER_ADD_SCORE			10	/*20*/								//放弃定时器

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

	//加注信息
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;
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
	switch (nTimerID)
	{
	case IDI_START_GAME:
		{
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_USER_ADD_SCORE:
		{
			//发送消息
			CMD_C_AddScore AddScore;
			AddScore.lScore=m_lTurnLessScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;
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

			//玩家设置
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
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
	m_lCellScore = pGameStart->lCellScore;

	//加注信息
	m_lTableScore[pGameStart->wDUser] += m_lCellScore;
	m_lTableScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;
	m_lTotalScore[pGameStart->wDUser] =  m_lCellScore;
	m_lTotalScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;

	//发送暗牌
	CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(m_cbHandCardData));
	m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,rand()%TIME_USER_ADD_SCORE);

	return true;
}

//加注消息
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//变量定义
	//WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;

	//设置变量
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;
	m_lTurnMaxScore = pAddScore->lTurnMaxScore;
	m_lAddLessScore = pAddScore->lAddLessScore;

	m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,rand()%TIME_USER_ADD_SCORE+5);
	
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
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;

	if(m_lTableScore[wGiveUpUser]!=0L)
	{
		m_lTableScore[wGiveUpUser] = 0L;
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

	if((pSendCard->cbSendCardCount < 3))
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,rand()%TIME_START_GAME+5);
	}

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,rand()%TIME_START_GAME+5);
	
	return true;
}

//开牌消息
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	return true;
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

		//CTraceService::TraceString(CA2T(buf),TraceLevel_Normal);
		OutputDebugString(CA2T(buf));
		ofstream outf(m_strLogFile.c_str(),ios::app); 
		outf<<timeFormat<<buf<<endl;
	}
#endif
}

void CAndroidUserItemSink::printLog(CString& str)
{
	printLog((LPSTR)CT2A(str));
}

//////////////////////////////////////////////////////////////////////////
