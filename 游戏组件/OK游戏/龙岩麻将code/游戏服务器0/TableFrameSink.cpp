#include "StdAfx.h"
#include "math.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include "DlgCustomRule.h"
#include "HapiDump.h"
//////////////////////////////////////////////////////////////////////////

//静态变量
//const WORD			CTableFrameSink::GAME_PLAYER=GAME_PLAYER;			//游戏人数
//const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式
#define IDI_TIMER_PLAYER_ACTION		3 //出牌
#define TIME_PLAYER_ACTION			30
#define TIME_PLAYER_ACTION_TRUSTEE	2
//////////////////////////////////////////////////////////////////////////
wchar_t g_wcTmp[256]={0};

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

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//生成DUMP文件。
	DeclareDumpFile();

	srand(time(NULL));

	CGameLogic	m_Logic;
	BYTE cbCardData[14] = {0x01, \
			0x15, 0x15, 0x15, \
			0x11, 0x12, 0x13, \
			0x25, 0x26, 0x27,
			0x00, 0x00, 0x00, 0x00};
	BYTE cbCardIndex[MAX_INDEX];
	BYTE cbNewCard=0x22;
	tagWeaveItem WeaveInfo[4];
	memset(&WeaveInfo, 0, sizeof(WeaveInfo));
	WeaveInfo[0].cbCenterCard=0x18;
	WeaveInfo[0].cbCardData[0]=0x18;
	WeaveInfo[0].cbCardData[1]=0x18;
	WeaveInfo[0].cbCardData[2]=0x18;
	WeaveInfo[0].cbWeaveKind=WIK_PENG;
	WeaveInfo[0].cbPublicCard=true;
	CChiHuRight	myRight;

	m_Logic.SetMagicIndex(m_Logic.SwitchToCardIndex(0x01));
	m_Logic.SwitchToCardIndex(cbCardData, 10, cbCardIndex);
	//BYTE cbResult = m_Logic.AnalyseChiHuCard(cbCardIndex, WeaveInfo, 1,cbNewCard, myRight);
	//
	m_bGangStatus=false;
	m_wProvideUser=0;
	m_bOutMagicCard[0]=1;
	m_bOutGangHuCard[0]=0;
	m_cbDiscardCount[0]=0;
	m_cbCaiShenOutCnt[0]=2;
	//FiltrateRight(0, myRight);





	//获取目录
	GetCurrentDirectory(CountArray(m_szPath),m_szPath);
	m_dwGameRuleIdex = 0;
//	m_cbGameTypeIdex = 0;
	m_cbHuangZhuang=false;
	m_lStartHuScore=1;		//默认1片起胡
	m_cbMagicCardUser=0xFF;

	//游戏变量
	m_lSiceCount=0;
	m_cbLandCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
	ZeroMemory(m_bOutMagicCard,sizeof(m_bOutMagicCard));
	ZeroMemory(m_bOutGangHuCard,sizeof(m_bOutGangHuCard));
	m_cbQuanFeng = 0;
	m_cbQuanCounter = 1;
	ZeroMemory( m_cbHuaCardCount,sizeof(m_cbHuaCardCount) );
	m_wReplaceUser = INVALID_CHAIR;
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	ZeroMemory( m_GangScore,sizeof(m_GangScore) );

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_ChiHuRight, sizeof(m_ChiHuRight));
	ZeroMemory(m_cbCaiShenOutCnt, sizeof(m_cbCaiShenOutCnt));

	//堆立信息
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 0;

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
//	memset(&m_bEnjoinChiHu, 1, sizeof(m_bEnjoinChiHu));			//普通情况下禁止吃胡；也就是只能自摸

	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
	ZeroMemory( m_cbMasterCheckCard,sizeof(m_cbMasterCheckCard) );

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory( m_dwChiHuKind,sizeof(m_dwChiHuKind) );

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	m_wLandSice=0;
	m_wCaiShenIndex=0;
	m_wCaiShenChairID=0;
	m_cbChangeCard=0;
	m_wCaiShenSice=0;
	m_wFirstUser=INVALID_CHAIR;
	ZeroMemory( m_cbEnjoinHuCard,sizeof(m_cbEnjoinHuCard) );
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
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
	if (m_pITableFrame==NULL)
		return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	for(int i=0; i<GAME_TYPE_MJ_MAX_WANFA; i++)
	{
		if (pCustomConfig->cbGameRule[i])
		{
			FvMask::Add(m_dwGameRuleIdex,_MASK_(i+1));
		}
	}
	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//游戏变量
	m_lSiceCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
	ZeroMemory( m_cbHuaCardCount,sizeof(m_cbHuaCardCount) );
	ZeroMemory(m_bOutMagicCard,sizeof(m_bOutMagicCard));
	ZeroMemory(m_bOutGangHuCard,sizeof(m_bOutGangHuCard));
	m_wReplaceUser = INVALID_CHAIR;
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	ZeroMemory( m_GangScore,sizeof(m_GangScore) );
	ZeroMemory(m_ChiHuRight, sizeof(m_ChiHuRight));
	m_cbMagicCardUser=0xFF;

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory( m_cbMasterCheckCard,sizeof(m_cbMasterCheckCard) );
	ZeroMemory(m_cbCaiShenOutCnt, sizeof(m_cbCaiShenOutCnt));
	//堆立信息
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//运行变量
	clearUserActionStates();
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
//	memset(&m_bEnjoinChiHu, 1, sizeof(m_bEnjoinChiHu));			//普通情况下禁止吃胡；也就是只能自摸
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory(m_dwChiHuKind,sizeof(m_dwChiHuKind));

	m_wLandSice=0;
	m_wCaiShenIndex=0;
	m_wCaiShenChairID=0;
	m_cbChangeCard=0;
	m_wCaiShenSice=0;
	m_wFirstUser=INVALID_CHAIR;
	ZeroMemory( m_cbEnjoinHuCard,sizeof(m_cbEnjoinHuCard) );

	if (m_pITableFrame)
	{
		m_pITableFrame->KillGameTimer(IDI_TIMER_PLAYER_ACTION);
	}

	return;
}

//
////游戏状态
//bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
//{
//	return true;
//}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	//设置状态
	m_cbLandCount++;
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//混乱扑克
	if(!GetPrivateProfile()) 
	{
		//随机洗牌
		m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));
	}
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
 
	//视图财神
	m_cbChangeCard = m_GameLogic.SwitchToCardData(rand()%MAX_INDEX);
	m_GameLogic.SetMagicIndex(m_GameLogic.SwitchToCardIndex(m_cbChangeCard));
	//翻出来一张财神，剩下3张财神牌
	RemoveOneMagicCard(m_cbChangeCard );
	TRACE(_T("AAA财神牌：%X\r\n"),m_cbChangeCard);
	TRACE(_T("AAA") + m_GameLogic.GetPaiString(m_cbChangeCard));
	//test
#ifdef _DEBUG
//	m_cbChangeCard = 0x23;			//财神牌

	BYTE byTest[MAX_REPERTORY] = {
					//万子
	0x02,0x05,0x06,0x37,0x19,0x01,0x02,0x04,0x05,0x06,		//万子

	0x12,0x13,0x14,0x15,0x16,0x16,0x17,						//索子
	0x15,0x18,0x19,0x11,0x11,0x21,0x22,						//索子
															//同子
	0x37,0x37,0x37,0x37,
	0x18,0x19,0x27,0x31,
	0x24,0x25,0x36,

	0x22,0x23,0x24,0x25,0x26,					//同子
	0x11,0x12,0x13,0x14,0x15,
	0x16,0x17,0x18,

	0x01,0x04,0x09,0x11,0x14,
	0x32,0x33,0x34,0x35,0x23,

	0x21,0x22,0x24,0x25,0x26,					//同子
	0x37,0x37,0x37,0x37,0x37,0x37,
0x34,
					//番子
	0x26,0x26,0x12,0x14,0x16,0x16,
	0x03,0x36,0x32,0x35,0x36,0x36,0x36,0x11,
	
	0x24,0x08,0x23,0x31,0x32,0x33,				//同子
	0x28,0x29,0x28,0x29,0x27,0x27,0x09,	

	0x34,0x35,0x36,0x19,0x23,0x26,					//番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x23,


	0x31,0x32,0x34,0x11,0x12,0x13,
	0x21,0x22,0x24,
	0x31,0x32,0x34,0x37,

	0x31,0x32,0x33,0x11,0x12,0x13,
	0x11,0x12,0x13,
	0x31,0x32,0x33,0x37,


	};
	CopyMemory( m_cbRepertoryCard,byTest,sizeof(byTest) ); 

	m_wBankerUser = 0;
#endif
	//end test

	//分发扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	//定出来庄家判断
	if( m_wBankerUser == INVALID_CHAIR )
	{
		m_wFirstUser = rand()%GAME_PLAYER;
		m_wLandSice = MAKEWORD(rand()%6+1,rand()%6+1);
		m_wBankerUser = (m_wFirstUser+HIBYTE(m_wLandSice)+LOBYTE(m_wLandSice)-1)%GAME_PLAYER;
	}

	//if( m_wBankerUser == INVALID_CHAIR )m_wBankerUser = rand()%GAME_PLAYER;

	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//设置变量
	m_cbProvideCard=m_cbSendCardData;
	m_wProvideUser=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//堆立信息
//#ifdef _DEBUG
//	WORD wTakeSice = MAKEWORD(rand()%6+1,rand()%6+1);
//	WORD wTakeCount = MAKEWORD(rand()%6+1,rand()%6+1);
//	//WORD wTakeSice = MAKEWORD(6,6);
//	//WORD wTakeCount = MAKEWORD(4,1);
//#else 
	WORD wTakeSice = MAKEWORD(rand()%6+1,rand()%6+1);		//1次色字1
	WORD wTakeCount = MAKEWORD(rand()%6+1,rand()%6+1);		//2次色字2
//#endif

	m_lSiceCount = MAKELONG(wTakeCount,wTakeSice);
	BYTE cbSiceTakeCount= HIBYTE(wTakeSice)+LOBYTE(wTakeSice);

	WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;			//谁先拿牌
	m_wHeapTail=wTakeChairID;

	//cbSiceTakeCount+= (HIBYTE(wTakeCount)+LOBYTE(wTakeCount));
	//if(cbSiceTakeCount>=HEAP_FULL_COUNT/2)
	//{
	//	wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	//	if(cbSiceTakeCount!=HEAP_FULL_COUNT/2)m_wHeapTail=(m_wHeapTail+1)%GAME_PLAYER;
	//	cbSiceTakeCount-=(HEAP_FULL_COUNT/2);
	//}

	BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//计算数目
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceTakeCount)*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//提取扑克
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//完成判断
		if (cbTakeCount==0)
		{
			m_wHeapHead=wTakeChairID;
			break;
		}
		//切换索引
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//财神点数
//#ifdef _DEBUG
//	m_wCaiShenSice = MAKEWORD(rand()%4+3,rand()%4+3);
//#else 
	m_wCaiShenSice = MAKEWORD(rand()%6+1,rand()%6+1);
//#endif
	//黄庄剩余牌数。
	m_cbEndLeftCount = 2 * max(HIBYTE(m_wCaiShenSice), LOBYTE(m_wCaiShenSice));
	CString strCaishen;
	strCaishen.Format(_T("\nAAA色子：%ld vs %ld\n"), HIBYTE(m_wCaiShenSice), LOBYTE(m_wCaiShenSice));
	TRACE(_T("AAA") + strCaishen);

	//财神位置
	m_wCaiShenIndex =HIBYTE(m_wCaiShenSice)+LOBYTE(m_wCaiShenSice);
	if((HEAP_FULL_COUNT-m_cbHeapCardInfo[m_wHeapTail][1])/2 >= m_wCaiShenIndex)
	{
		m_wCaiShenIndex+=m_cbHeapCardInfo[m_wHeapTail][1]/2;
		m_wCaiShenChairID =m_wHeapTail;
	}
	else
	{
		m_wCaiShenIndex = m_wCaiShenIndex-(HEAP_FULL_COUNT-m_cbHeapCardInfo[m_wHeapTail][1])/2;
		m_wCaiShenChairID =(m_wHeapTail+GAME_PLAYER-1)%GAME_PLAYER;
	}
	m_wCaiShenIndex-=1;

	//动作分析
	bool bAroseAction=false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//用户索引
		WORD wUserIndex=(m_wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

		for( BYTE j = MAX_INDEX-MAX_HUA_CARD; j < MAX_INDEX; j++ )
		{
			if( m_cbCardIndex[wUserIndex][j] > 0 )
			{
				m_wReplaceUser = wUserIndex;
				break;
			}
		}

		if( m_wReplaceUser != INVALID_CHAIR ) break;
	}

	//庄家判断
	if ( m_wReplaceUser==INVALID_CHAIR )
	{
		//杠牌判断
		tagGangCardResult GangCardResult;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wBankerUser],NULL,0,GangCardResult);

		//胡牌判断
		CChiHuRight chr;
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,m_cbSendCardData,chr);
		if(!(chr&CHR_MAGIC_EYE).IsEmpty())
					{
						m_cbUserAction[m_wBankerUser] |= WIK_MAKE_EYE;
					}
		if(!(chr&CHR_THREE_MAGIC).IsEmpty())
		{
			m_cbUserAction[m_wBankerUser] |= WIK_MAKE_EYE_THREE;
		}
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
	}

	//N片起胡
	BYTE	 cbLessHu=0;
	if(hasRule(GAME_TYPE_LESS_HU_ONE))
		cbLessHu=1;
	if(hasRule(GAME_TYPE_LESS_HU_TW0))
		cbLessHu=2;
	if(cbLessHu==0)
		cbLessHu=1;
	m_lStartHuScore=cbLessHu;

	//构造数据
	CMD_S_GameStart GameStart;
	memset(&GameStart, 0, sizeof(CMD_S_GameStart));
	GameStart.lSiceCount=m_lSiceCount;					//庄家色字，无用；
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wReplaceUser = m_wReplaceUser;
	GameStart.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
	GameStart.cbQuanFeng = m_cbQuanFeng;
	GameStart.wHeapHead = m_wHeapHead;
	GameStart.wHeapTail = m_wHeapTail;
	GameStart.cbChangeCard=m_cbChangeCard;				//财神牌，万能牌（替换财神原先位置的牌？）
	GameStart.wCaiShenSice=m_wCaiShenSice;				//色字点数
	GameStart.wCaiShenIndex=m_wCaiShenIndex;			//财神位置
	GameStart.wCaiShenChairID=m_wCaiShenChairID;
	GameStart.wLandSice = m_wLandSice;
	GameStart.wFirstUser = m_wFirstUser;
	GameStart.cbLandCount = m_cbLandCount;				//坐庄次数
	GameStart.cbLeftCardCount = m_cbLeftCardCount;		//剩余扑克
	GameStart.cbLessHu = cbLessHu;								//N片起胡
	CopyMemory(GameStart.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

#ifdef USE_RS_PRINT
	SYSTEMTIME timeCreateFile={0};	// 建档的时间
	GetSystemTime(&timeCreateFile);	// 获得当前时间

	wstring  strLogFile=L"";
	strLogFile=s2ws(GetAppPath())+L"\\HZMJServer\\NetMsg.txt";
	wstring wstrDirName=s2ws(GetAppPath())+L"\\HZMJServer";
	int ret=create_dir((wchar_t *)wstrDirName.c_str(),wstrDirName.length());

	wsprintfW(g_wcTmp,L"%s%04d_%02d_%02d【%02d：%02d：%02d】.txt", 
		strLogFile.c_str(),
		timeCreateFile.wYear,
		timeCreateFile.wMonth,
		timeCreateFile.wDay,
		timeCreateFile.wHour+8,	// 加8是对齐北京时间
		timeCreateFile.wMinute,
		timeCreateFile.wSecond
		);
	ofstream outf(g_wcTmp,ios::app);
#ifdef USE_FIXCARD
	int iNum=sizeof(byTest)/sizeof(BYTE);
	CString strLog0;
	strLog0.Format(_T("sizeof(byTest)/sizeof(BYTE)=[%d]"),iNum);
	for(int i=0;i<iNum;i++)
	{
		CString strLog;
		strLog.Format(_T("[%d,%x]"),i,byTest[i]);
		strLog0+=strLog;
	}
	CTraceService::TraceString(strLog0,TraceLevel_Normal);
    outf<<CT2A(strLog0)<<endl;
#endif
	CString strLog;
	strLog.Format(_T("CMD_S_GameStart 骰子点数lSiceCount=[%d],庄家用户wBankerUser=[%d],当前用户wCurrentUser=[%d],补牌用户wReplaceUser=[%d],圈风cbQuanFeng=[%d],堆立牌头wHeapHead=[%d],堆立牌尾wHeapTail=[%d]"),GameStart.lSiceCount,GameStart.wBankerUser,GameStart.wCurrentUser,GameStart.wReplaceUser,GameStart.cbQuanFeng,GameStart.wHeapHead,GameStart.wHeapTail);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
	outf<<CT2A(strLog)<<endl;
#endif

	//发送数据
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//设置变量
		GameStart.cbUserAction=m_cbUserAction[i];
		memset(GameStart.cbCardData, 0, sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);
		CString strLog;
		for(int k=0; k<14; k++)
		{
			strLog += m_GameLogic.GetPaiString(GameStart.cbCardData[k]);
			strLog += L" ";
		}
		TRACE(_T("AAA第%ld个玩家数据：\n"), i);
		TRACE(_T("AAA")+strLog);
		TRACE(_T("\n"));

		//FOR ANDROID
		if( m_pITableFrame->GetTableUserItem(i)->IsAndroidUser() )
		{
			CMD_S_AndroidRule kAndroidRule;
			memset(&kAndroidRule, 0, sizeof(CMD_S_AndroidRule));
//			kAndroidRule.cbGameTypeIdex = m_cbGameTypeIdex;
			kAndroidRule.dwGameRuleIdex = m_dwGameRuleIdex;
			kAndroidRule.cbMagicIndex = m_GameLogic.SwitchToCardIndex(m_cbChangeCard);

			m_pITableFrame->SendTableData(i,SUB_S_ANDROID_RULE,&kAndroidRule,sizeof(kAndroidRule));
			m_pITableFrame->SendLookonData(i,SUB_S_ANDROID_RULE,&kAndroidRule,sizeof(kAndroidRule));

		}
		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

//	m_bSendStatus=TRUE;
	starGameRecord();
	sendMasterHandCard();

	return true;
}
//后台配置
bool CTableFrameSink::GetPrivateProfile()
{
	try
	{
	//读取配置
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\MaJiang.ini"),m_szPath);

	//是否配置
	bool bReadFile=(GetPrivateProfileInt(TEXT("Read"),TEXT("IsRead"),0,szFileName)!=0);

	//庄家用户
	WORD wLandUser=GetPrivateProfileInt(TEXT("Land"),TEXT("LandUser"),GAME_PLAYER,szFileName);
	if(wLandUser<GAME_PLAYER)m_wBankerUser=wLandUser;

	//读取消息
	BYTE cbAllCardCount=0;
	BYTE cbCardIndex=CountArray(m_cbRepertoryCard)-1;
	if (bReadFile)
	{
		//读取配置
		bool bFind=true;
		TCHAR szKeyName[32],szMessage1[1024];
		for(LONG i=0;i<16;i++)
		{
			_sntprintf(szKeyName,CountArray(szKeyName),TEXT("List%ld"),i+1);
			GetPrivateProfileString(TEXT("MA_JIANG_BAI"),szKeyName,TEXT(""),szMessage1,CountArray(szMessage1),szFileName);

			LONG lCount=CountArray(szMessage1);
			for(LONG j=0;j<lCount;j++)
			{
				//结束查找
				if(szMessage1[j]=='\0')break;
				if((szMessage1[j]=='x' || szMessage1[j]=='X') && (j+2 < lCount))
				{
					//颜色数字
					m_cbRepertoryCard[cbCardIndex]=(szMessage1[j+1]-'0')*16;

					//牌值数据
					if(szMessage1[j+2]>='a' && szMessage1[j+2]<='f')
						m_cbRepertoryCard[cbCardIndex] += (szMessage1[j+2]-'a'+10);
					else if(szMessage1[j+2]>='A' && szMessage1[j+2]<='F')
						m_cbRepertoryCard[cbCardIndex] += (szMessage1[j+2]-'A'+10);
					else m_cbRepertoryCard[cbCardIndex]+= (szMessage1[j+2]-'0');

					//判断是否有效
					if(!m_GameLogic.IsValidCard(m_cbRepertoryCard[cbCardIndex])) bFind=false;

					//数目判断
					if(cbCardIndex>0)--cbCardIndex;
					if(++cbAllCardCount==MAX_REPERTORY)break;
					j+=2;
				}

				if(cbAllCardCount==MAX_REPERTORY)break;
				if(!bFind)break;
			}
			if(cbAllCardCount==MAX_REPERTORY)break;
			if(!bFind)break;
		}
		return (cbAllCardCount==MAX_REPERTORY && bFind);
	}
	}
	catch(...)
	{
		ASSERT(FALSE);
		return false;
	}

	return false;
}


//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{


			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
//			GameEnd.wLeftUser = INVALID_CHAIR;

			//结束信息：定赢家
			WORD wWinner = INVALID_CHAIR;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.dwChiHuKind[i]=m_dwChiHuKind[i];
				if( m_dwChiHuKind[i] == WIK_CHI_HU ) 
					wWinner = i;
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}


			//如果不是流局
			if( wWinner != INVALID_CHAIR )
			{
				//权位过滤
				FiltrateRight( wWinner,m_ChiHuRight[wWinner] );
				
				//记住，上面需要根据黄庄来判断横翻规则，因此需要判断以后再修改黄庄状态
				m_cbHuangZhuang=false;
			}
			else
				m_cbHuangZhuang=true;

			if(!m_cbHuangZhuang)
			{
				//发送胡牌消息
				CMD_S_ChiHu ChiHu;
				memset(&ChiHu, 0, sizeof(CMD_S_ChiHu));
				ChiHu.wChiHuUser = wWinner;
				ChiHu.wProviderUser = m_wProvideUser;
				ChiHu.lGameScore = m_lGameScore[wChairID];
				ChiHu.cbCardCount = m_GameLogic.GetCardCount(m_cbCardIndex[wChairID]);
				ChiHu.cbChiHuCard = m_cbProvideCard;
				addGameRecordAction(ChiHu);

				m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&ChiHu,sizeof(ChiHu) );
				m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&ChiHu,sizeof(ChiHu) );
			}

			LONG lGangScore[GAME_PLAYER];
			ZeroMemory(&lGangScore,sizeof(lGangScore));
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				for( BYTE j = 0; j < m_GangScore[i].cbGangCount; j++ )
				{

					if (m_GangScore[i].bMingGang)
					{
						///m_pITableFrame->AddPrivateAction(i,4);
					}
					else
					{
						///m_pITableFrame->AddPrivateAction(i,3);
					}
					for( WORD k = 0; k < GAME_PLAYER; k++ )
						lGangScore[k] += m_GangScore[i].lScore[j][k];
				}
			}


			//统计积分 输家
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//结束信息
				GameEnd.wProvideUser[0]=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;
				GameEnd.cbChiHuCardData[m_wProvideUser][0] = m_cbProvideCard;

				m_ChiHuRight[wWinner].GetRightData( GameEnd.dwChiHuRight1,MAX_RIGHT_COUNT );

				GameEnd.cbFanCount = CalScore( wWinner, m_lGameScore );
				//GameEnd.cbFanCount =m_GameLogic.GetChiHuActionRank(m_ChiHuRight[wWinner],  \
				//	m_GameLogic.IsMagicCard(m_cbDiscardCard[wWinner][m_cbDiscardCount[wWinner]-2]) );

				//打出来2个财神，多飘花
				if(m_cbCaiShenOutCnt[wWinner]>=2)
					GameEnd.bSecondMagic=true;
				GameEnd.cbHuaCardCount = m_cbHuaCardCount[wWinner];
			}
			else
			{
				//流局结束
				GameEnd.wProvideUser[0]=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;
			}
			GameEnd.cbWinerMakeEye = m_cbUserMakeEye[wWinner];
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				m_lGameScore[i] += lGangScore[i];
				//m_lGameScore[i] += m_lStartHuScore[i];
			}
			//统计积分
			LONG lGameTax[GAME_PLAYER];
			ZeroMemory(lGameTax,sizeof(lGameTax));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//累加积分
				GameEnd.lGameScore[i] = m_pGameServiceOption->lCellScore * m_lGameScore[i];
				//设置积分
				if( GAME_GENRE_SCORE != m_pGameServiceOption->wServerType )
				{
					if (GameEnd.lGameScore[i]>0L)
					{
						///lGameTax[i] = (LONG)(GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenueRatio /1000L);
						GameEnd.lGameScore[i] -= lGameTax[i];
					}
				}
			}

			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE ScoreKind;
				if( GameEnd.lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;
				//写入积分
				//m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lGameTax[i],ScoreKind);
				ScoreInfoArray[i].lScore   = GameEnd.lGameScore[i];		//已经扣税的金额
				ScoreInfoArray[i].lRevenue = lGameTax[i];
				ScoreInfoArray[i].cbType   = ScoreKind;
				//发送信息
				//m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				//m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//写入积分
			datastream kDataStream;
			m_kGameRecord.StreamValue(kDataStream,true);
			//写入积分
			///m_pITableFrame->WriteTableScore(ScoreInfoArray ,CountArray(GameEnd.lGameScore),kDataStream);

			CopyMemory( GameEnd.lGameScore,m_lGameScore,sizeof(GameEnd.lGameScore) );
			CopyMemory( GameEnd.lGangScore,lGangScore,sizeof(GameEnd.lGangScore) );
			//CopyMemory( GameEnd.wProvideUser,m_wProvider,sizeof(GameEnd.wProvideUser) );
			//CopyMemory( GameEnd.lGameTax,lGameTax,sizeof(GameEnd.lGameTax));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//CopyMemory( GameEnd.wLostFanShu[i],m_wLostFanShu[i],sizeof(m_wLostFanShu[i]) );
				CopyMemory(GameEnd.WeaveItemArray[i],m_WeaveItemArray[i],sizeof(m_WeaveItemArray[i]));
			}
			//CopyMemory( GameEnd.lStartHuScore, &m_lStartHuScore,sizeof(GameEnd.lStartHuScore) );

			

			//组合扑克
			CopyMemory(GameEnd.cbWeaveCount,m_cbWeaveItemCount,sizeof(GameEnd.cbWeaveCount));
			//发送结束信息
			m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd) );
			//设置变量
			if(wWinner!=m_wBankerUser && wWinner!=INVALID_CHAIR)
			{
				m_cbLandCount=0;
				m_wBankerUser = wWinner;
			}

			//结束游戏
			///m_pITableFrame->ConcludeGame(GS_MJ_FREE);
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//设置变量
			GameEnd.wProvideUser[0]=INVALID_CHAIR;

			//拷贝扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			///m_pITableFrame->ConcludeGame(GS_MJ_FREE);
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:
		{
			///m_pITableFrame->ConcludeGame(GS_MJ_FREE);
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;

			/*
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//设置变量
			GameEnd.wProvideUser=INVALID_CHAIR;

			//拷贝扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//积分
			WORD i=0;
			for ( i=0;i<GAME_PLAYER;i++) 
				if(m_bOutMagicCard[i])break;
			GameEnd.lGameScore[wChairID] = (i<GAME_PLAYER?-32L:-8L)*m_pGameServiceOption->lCellScore;
			
			if (i<GAME_PLAYER)
			{
				for (WORD j=0;j<GAME_PLAYER;j++)
				{
					if(j==wChairID)continue;
					WORD wUserID=(m_wBankerUser+GAME_PLAYER-i)%GAME_PLAYER;
					if(WIK_LISTEN!=m_GameLogic.AnalyseTingCard(m_cbCardIndex[wUserID],m_WeaveItemArray[wUserID],
						m_cbWeaveItemCount[wUserID]))continue;
					GameEnd.lGameScore[wUserID] = -GameEnd.lGameScore[wChairID];
					break;
				}
			}

			//通知消息
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("由于 [ %s ] 离开游戏，游戏结束"),pIServerUserItem->GetNickName());
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
				if (pISendUserItem!=NULL) 
					m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
			}

			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL)
					break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
			} while (true);

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//写入积分
			m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0,SCORE_TYPE_FLEE);

			//结束游戏
			m_pITableFrame->ConcludeGame(GS_MJ_FREE);

			return true;
			*/
		}
	}

	//错误断言
	ASSERT(FALSE);
	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//构造数据
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//游戏状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.wReplaceUser = m_wReplaceUser; 
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));
			StatusPlay.cbQuanFeng = m_cbQuanFeng;
			StatusPlay.cbChangeCard = m_cbChangeCard;
			StatusPlay.cbLandCount=m_cbLandCount;
			StatusPlay.wCaiShenIndex=m_lStartHuScore;		//起胡
			StatusPlay.wCaiShenChairID=m_wCaiShenSice;		//财神色子

			//状态变量
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//听牌状态
			CopyMemory( StatusPlay.cbHearStatus,m_cbListenStatus,sizeof(m_cbListenStatus) );

			//历史记录
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//堆立信息
			StatusPlay.wHeapHead = m_wHeapHead;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

			//扑克数据
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);
			if( m_wReplaceUser == wChiarID ) StatusPlay.cbSendCardData = m_cbSendCardData;
			else StatusPlay.cbSendCardData=(m_wProvideUser==wChiarID)?m_cbProvideCard:0x00;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

void CTableFrameSink::setActionTimeStart(bool bStart,bool Trustee)
{
	if (!isUseTuoGuan())
	{
		return;
	}
	int iTime = TIME_PLAYER_ACTION;
	if (Trustee)
	{
		iTime = TIME_PLAYER_ACTION_TRUSTEE;
	}
	if (bStart)
	{
		m_pITableFrame->SetGameTimer(IDI_TIMER_PLAYER_ACTION,iTime*1000,1,0);
	}
	else
	{
		m_pITableFrame->KillGameTimer(IDI_TIMER_PLAYER_ACTION);
	}
}
//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_TIMER_PLAYER_ACTION:  //玩家一轮
		{
//			m_pITableFrame->KillGameTimer(IDI_TIMER_XIAO_HU);
//			if (m_wHaiDiUser != INVALID_CHAIR)
//			{
//				OnUseHaiDiCard(m_wHaiDiUser,0);
//				return true;
//			}
			if (m_wCurrentUser == INVALID_CHAIR)
			{
				for (WORD i = 0;i<GAME_PLAYER;i++)
				{
					if (m_kUserActionStates1.m_cbUserAction[i] == WIK_NULL)
					{
						continue;
					}
					setChairTrustee(i,true);
					OnUserOperateCard(i,WIK_NULL,0);
				}
				for (WORD i = 0;i<GAME_PLAYER;i++)
				{
					if (m_kUserActionStates2.m_cbUserAction[i] == WIK_NULL)
					{
						continue;
					}
					setChairTrustee(i,true);
					OnUserOperateCard(i,WIK_NULL,0);
				}
			}
			else
			{
				setChairTrustee(m_wCurrentUser,true);
				std::vector<BYTE> kCard;
				BYTE cbActSendCard = INVALID_BYTE;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[m_wCurrentUser],kCard);
				if (kCard.size() == 0)
				{
					OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
					return true;
				}
				for (int i = 0;i<(int)kCard.size();i++)
				{
					if (kCard[i] == m_kUserActionStates1.m_cbSendCardData)
					{
						cbActSendCard = m_kUserActionStates1.m_cbSendCardData;
					}
				}
				if (cbActSendCard == INVALID_BYTE)
				{
					cbActSendCard = kCard[0];
				}
				OnUserOutCard(m_wCurrentUser,cbActSendCard);
			}

			return true;
		}
	}
	return false;
}

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID* pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//出牌消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;
			//消息处理
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			TRACE("AAA玩家位置：%ld 出牌 %x -%s \n", pIServerUserItem->GetChairID(), pOutCard->cbCardData, m_GameLogic.GetPaiString(pOutCard->cbCardData));
			return OnUserOutCard(pIServerUserItem->GetChairID(), pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pIServerUserItem->GetChairID(),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_REPLACE_CARD:	//用户补牌；补花（这里无用）
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_ReplaceCard));
			if (wDataSize!=sizeof(CMD_C_ReplaceCard)) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//变量定义
			CMD_C_ReplaceCard * pReplaceCard=(CMD_C_ReplaceCard *)pDataBuffer;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserReplaceCard(pIServerUserItem->GetChairID(),pReplaceCard->cbCardData);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			m_bTrustee[pUserData->wChairID]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			memset(&Trustee, 0, sizeof(CMD_S_Trustee));
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pUserData->wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

			return true;
		}
	case SUB_C_LISTEN:
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_Listen));
			if (wDataSize!=sizeof(CMD_C_Listen)) return false;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			CMD_C_Listen *pListen = (CMD_C_Listen *)pDataBuffer;
			return OnUserListenCard(pUserData->wChairID,pListen->cbListen);
		}
	case SUB_C_MASTER_LEFTCARD:
		{
			WORD wChairID = pIServerUserItem->GetChairID();
			sendMasterLeftCard(wChairID);
			return true;
		}
	case SUB_C_MASTER_CHEAKCARD:
		{

			MaterCheckCard *pMaterCheckCard =(MaterCheckCard *)pDataBuffer;
			if(wDataSize != sizeof(MaterCheckCard)) return false;

			WORD wChairID = pIServerUserItem->GetChairID();
			m_cbMasterCheckCard[wChairID] = pMaterCheckCard->cbCheakCard;

			BYTE cbRequestCard = getSendCardData(wChairID);
			sendMasterHandCard();
			return true;
		}
	case SUB_C_MAKE_CARDEYE:
		{
			WORD wChairID = pIServerUserItem->GetChairID();
			m_cbUserMakeEye[wChairID]++;
			return true;
		}
	}

	return false;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户起来
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if (bLookonUser==false)
	{
		m_cbLandCount=0;
		m_wBankerUser = INVALID_CHAIR;

		m_cbQuanCounter = 1;

		m_bTrustee[wChairID]=false;
		CMD_S_Trustee Trustee;
		memset(&Trustee, 0, sizeof(CMD_S_Trustee));
		Trustee.bTrustee=false;
		Trustee.wChairID = wChairID;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	}

	return true;
}

//用户听牌
bool CTableFrameSink::OnUserListenCard(WORD wChairID,BYTE cbListen)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//效验参数
//	ASSERT((wChairID==m_wCurrentUser)&&(m_cbListenStatus[wChairID]==0));
	if ((wChairID!=m_wCurrentUser)||(m_cbListenStatus[wChairID]!=0)) 
	{
		//设置变量
		m_cbListenStatus[wChairID] = cbListen;
		m_bEnjoinChiPeng[wChairID] = true;			//禁止吃碰，可以杠胡
		return true;
	}

	//构造数据
	CMD_S_Listen ListenCard;
	memset(&ListenCard, 0, sizeof(CMD_S_Listen));
	ListenCard.wChairId=wChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//错误断言
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wCurrentUser) 
		return true;
	if (m_GameLogic.IsValidCard(cbCardData)==false) 
		return true;	

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return true;
	}

	//判断财神
	if(m_bOutMagicCard[wChairID])
		m_bOutMagicCard[wChairID]=false;
	if(m_GameLogic.IsMagicCard(cbCardData)) 
	{
		m_cbMagicCardUser=wChairID;
		m_bOutMagicCard[wChairID]=true;
	}
	//打的是财神牌，本轮其他玩家禁止吃碰，但是可以胡，杠;下一局自动解除
	if(m_bEnjoinChiPeng[wChairID])
		m_bEnjoinChiPeng[wChairID]=false;
	if(m_bOutMagicCard[wChairID])							//飘花的时候，所有人禁止吃碰；除了杠，胡
	{
		memset(m_bEnjoinChiPeng, 1, sizeof(m_bEnjoinChiPeng));
	}
	
	if(!m_bEnjoinChiHu[wChairID])
		m_bEnjoinChiHu[wChairID]=true;
	//无花大吊可以接炮==吃胡
	////是否大吊：只剩下两张手牌
	bool bDaDiao = m_GameLogic.IsDaDiao(m_cbCardIndex[wChairID]);
	if(bDaDiao && m_GameLogic.getMagicCardCount(m_cbCardIndex[wChairID]) == 0)
	{
		m_bEnjoinChiHu[wChairID]=false;		//可以吃胡
	}
	//爆头只能自摸，不能吃碰，接炮

	//判断杠开 
	if(m_bOutGangHuCard[wChairID])
		m_bOutGangHuCard[wChairID]=false;
	if(m_bGangStatus)
	{
			for(int n=0; n<GAME_PLAYER; n++)
				m_bEnjoinChiHu[n]=false;		//杠了，别人可以吃胡

		CChiHuRight chr;
		BYTE cbWeaveCount=m_cbWeaveItemCount[wChairID];
		if(WIK_CHI_HU==m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],m_WeaveItemArray[wChairID],cbWeaveCount,cbCardData,chr))
			m_bOutGangHuCard[wChairID] = true;
	}

	//设置变量
	m_bSendStatus=true;
	m_bGangStatus = false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//出牌记录
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//记录打过的财神牌数量
	if(m_bOutMagicCard[wChairID])
	{
		m_cbCaiShenOutCnt[m_wOutCardUser]++;		//财神打出去 ++
	}
	//构造数据
	CMD_S_OutCard OutCard;
	memset(&OutCard, 0, sizeof(CMD_S_OutCard));
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;
	addGameRecordAction(OutCard);

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;

	//用户切换
	m_wCurrentUser=(wChairID+GAME_PLAYER-1)%GAME_PLAYER;

	//响应判断:如果返回TRUE,那么久等待其他玩家的响应了
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//派发扑克
	if (bAroseAction==false) 
		DispatchCardData(m_wCurrentUser);

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3])
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()!=GS_MJ_FREE);
	if (m_pITableFrame->GetGameStatus()==GS_MJ_FREE)
		return true;
	
		//JUST FOR TEST
		for(int nn=0; nn<GAME_PLAYER; nn++)
		{
			BYTE	cbCardData[14];
			m_GameLogic.SwitchToCardData(m_cbCardIndex[nn],cbCardData);
			CString strLog;
			for(int k=0; k<14; k++)
			{
					strLog += m_GameLogic.GetPaiString(cbCardData[k]);
					strLog += L" ";
			}
			//TRACE("操作前，第%ld个玩家数据：\n", nn);
			//TRACE(strLog);
			//TRACE("\n");
		}
		CString strLog;
		CString strCmd;
			
		if(cbOperateCode & WIK_LEFT)													//左吃类型
			strCmd=_T("左吃 ");
		if(cbOperateCode & WIK_CENTER)												//中吃类型
			strCmd=_T("中吃 ");
		if(cbOperateCode & WIK_RIGHT)													//右吃类型
			strCmd=_T("右吃 ");
		if(cbOperateCode & WIK_PENG)													//碰牌类型
			strCmd=_T("碰牌 ");
		if(cbOperateCode & WIK_GANG)												//杠牌类型
			strCmd=_T("杠牌 ");
		if(cbOperateCode & WIK_LISTEN)												//听牌类型
			strCmd=_T("听牌 ");
		if(cbOperateCode & WIK_CHI_HU)												//胡牌
			strCmd=_T("胡牌 ");
		m_GameLogic.GetPaiString(cbOperateCard[0]);
		strLog.Format(_T("玩家位置：%ld, 目标牌:%s,%s,%s,操作指令："), wChairID, m_GameLogic.GetPaiString(cbOperateCard[0]), m_GameLogic.GetPaiString(cbOperateCard[1]), m_GameLogic.GetPaiString(cbOperateCard[2]));
		strLog += strCmd;
		strLog += _T("\r\n");
		TRACE(_T("AAA")+strLog);

	//效验用户 注意：当有机器人时有可能会发生此断言
	//ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return true;

	//被动动作
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//效验状态
		//ASSERT(m_bResponse[wChairID]==false);
		//ASSERT( m_cbUserAction[wChairID] != WIK_NULL );
		//ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//效验状态
		if (m_bResponse[wChairID]==true) return true;
		if( m_cbUserAction[wChairID] == WIK_NULL ) return true;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return true;

		//变量定义
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//设置变量
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		if(cbOperateCard[0]==0)		//补救措施，不要问我为什么
		{
			m_cbOperateCard[wChairID][0] = m_cbProvideCard;
			if(cbOperateCode & WIK_LEFT)													//左吃类型
			{
				m_cbOperateCard[wChairID][1] = m_cbProvideCard+1;
				m_cbOperateCard[wChairID][2] = m_cbProvideCard+2;
			}
			if(cbOperateCode & WIK_CENTER)												//中吃类型
			{
				m_cbOperateCard[wChairID][1] = m_cbProvideCard-1;
				m_cbOperateCard[wChairID][2] = m_cbProvideCard+1;
			}
			if(cbOperateCode & WIK_RIGHT)	
			{
				m_cbOperateCard[wChairID][1] = m_cbProvideCard-1;
				m_cbOperateCard[wChairID][2] = m_cbProvideCard-2;
			}
		}
		else 
			CopyMemory( m_cbOperateCard[wChairID],cbOperateCard,sizeof(m_cbOperateCard[wChairID]) );

		//执行判断
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//获取动作
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//优先级别
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//动作判断
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) 
			return true;

		//吃胡等待
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
					return true;
			}
		}

		//放弃操作
		if (cbTargetAction==WIK_NULL)
		{
			//用户状态
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//发送扑克
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//变量定义
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser][0];

		//出牌变量
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//胡牌操作
		if (cbTargetAction==WIK_CHI_HU)
		{
			//结束信息
			m_cbChiHuCard=cbTargetCard;

			for (WORD i=(m_wProvideUser+GAME_PLAYER-1)%GAME_PLAYER;i!=m_wProvideUser;i = (i+GAME_PLAYER-1)%GAME_PLAYER)
			{
				//过虑判断
				if ((m_cbPerformAction[i]&WIK_CHI_HU)==0)
					continue;

				//胡牌判断
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_dwChiHuKind[i] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i]);

				//插入扑克
				if (m_dwChiHuKind[i]!=WIK_NULL) 
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					wTargetUser = i;
					break;
				}
			}


			//结束游戏
			ASSERT(m_dwChiHuKind[wTargetUser]!=WIK_NULL);
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//组合扑克
		ASSERT(m_cbWeaveItemCount[wTargetUser]<=MAX_WEAVE);
		WORD wIndex=0;
		bool bPengGang=false;
		if(cbTargetAction == WIK_GANG)
		{
			int kk=0;		//是否碰了立马就杠的那个组合牌
			for(kk=0; kk<m_cbWeaveItemCount[wTargetUser]; kk++)
			{
				if(m_WeaveItemArray[wTargetUser][kk].cbWeaveKind == WIK_PENG && m_WeaveItemArray[wTargetUser][kk].cbCenterCard == cbTargetCard)
				{
					bPengGang=true;
					wIndex = kk;
					break;
				}
			}
			if(kk==m_cbWeaveItemCount[wTargetUser])
				wIndex = m_cbWeaveItemCount[wTargetUser]++;
		}
		else
			wIndex = m_cbWeaveItemCount[wTargetUser]++;

		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		m_WeaveItemArray[wTargetUser][wIndex].cbCardData[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = m_cbOperateCard[wTargetUser][1];
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = m_cbOperateCard[wTargetUser][2];
		}
		else
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = cbTargetCard;
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = cbTargetCard;
			if( cbTargetAction & WIK_GANG )
				m_WeaveItemArray[wTargetUser][wIndex].cbCardData[3] = cbTargetCard;
		}

		//删除扑克
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_RIGHT:		//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_CENTER:	//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_PENG:		//碰牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_GANG:		//杠牌操作
			{
				//删除扑克,被动动作只存在放杠
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
				int nRemoveCnt=CountArray(cbRemoveCard);
				if(bPengGang)
					nRemoveCnt=1;
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard, nRemoveCnt ) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		default:
			ASSERT( FALSE );
			return false;
		}

		//构造结果
		CMD_S_OperateResult OperateResult;
		ZeroMemory( &OperateResult,sizeof(OperateResult) );
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		OperateResult.cbOperateCard[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			CopyMemory( &OperateResult.cbOperateCard[1],&m_cbOperateCard[wTargetUser][1],2*sizeof(BYTE) );
		else if( cbTargetAction&WIK_PENG )
		{
			OperateResult.cbOperateCard[1] = cbTargetCard;
			OperateResult.cbOperateCard[2] = cbTargetCard;
		}

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//用户状态
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//设置用户
		m_wCurrentUser=wTargetUser;

		//杠牌处理
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus = true;
			//发送杠牌信息，计分
			//杠牌得分
			bool bPublic=true;
			int lScore = bPublic?m_pGameServiceOption->lCellScore :m_pGameServiceOption->lCellScore * 2;
			BYTE cbGangIndex = m_GangScore[wChairID].cbGangCount++;
			m_GangScore[wChairID].bMingGang = bPublic;
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if(  i == wChairID ) continue;

				m_GangScore[wChairID].lScore[cbGangIndex][i] = -lScore;
				m_GangScore[wChairID].lScore[cbGangIndex][wChairID] += lScore;
			}
			CMD_S_GangScore gs;
			memset(&gs, 0, sizeof(gs));
			gs.wChairId = wChairID;
			ZeroMemory( &gs,sizeof(gs) );
			gs.cbXiaYu = bPublic ?FALSE:TRUE;
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( i == wChairID ) continue;

				gs.lGangScore[i] = -lScore;
				gs.lGangScore[wChairID] += lScore;
			}
			m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );

			DispatchCardData(wTargetUser,true);
		}
		if(cbTargetAction == WIK_PENG)
		{
			//杠牌判断；
			if (m_cbLeftCardCount>m_cbEndLeftCount)
			{
				tagGangCardResult GangCardResult;
				memset(&GangCardResult, 0, sizeof(tagGangCardResult));
				m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
					m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);
			
				m_kUserActionStates2.m_cbUserAction[m_wCurrentUser] = m_cbUserAction[m_wCurrentUser];
				m_kUserActionStates2.m_cbOperateCard[m_wCurrentUser] = GangCardResult.cbCardData[0];
				if(m_cbUserAction[m_wCurrentUser] & WIK_GANG)
				{
				//结果处理	//设置变量
					m_wProvideUser=m_wCurrentUser;
					m_cbProvideCard=GangCardResult.cbCardData[0];
					m_wResumeUser=m_wCurrentUser;
					m_wCurrentUser=INVALID_CHAIR;
					//发送提示
					SendOperateNotify();

				}
			}	
		}
		return true;
	}

	//主动动作
	if (m_wCurrentUser==wChairID)
	{
		//效验操作
		//ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0))
		{
			TRACE("弃牌");
		}

		//扑克效验
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard[0])==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard[0])==false)) 
			return false;

		//设置变量
		m_bSendStatus=true;
		m_cbUserAction[m_wCurrentUser]=WIK_NULL;
		m_cbPerformAction[m_wCurrentUser]=WIK_NULL;
		if(WIK_CHI_HU!=cbOperateCode)m_bOutGangHuCard[m_wCurrentUser]=false;
		ZeroMemory(m_cbEnjoinHuCard[m_wCurrentUser],sizeof(m_cbEnjoinHuCard[m_wCurrentUser]));
		bool bPublic=false;

		//执行动作
		switch (cbOperateCode)
		{
		case WIK_GANG:			//杠牌操作
			{
				//变量定义
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard[0]);

				//杠牌处理
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//寻找组合
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
						{
							bPublic=true;
							cbWeaveIndex=i;
							break;
						}
					}

					//效验动作
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) 
						return false;

					//组合扑克
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[0]=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[1]=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[2]=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[3]=cbOperateCard[0];
				}
				else
				{
					//扑克效验
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
						return false;

					//设置变量
					bPublic=false;
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					for( BYTE j = 0; j < 4; j++ )
						m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[j] = cbOperateCard[0];
				}

				//删除扑克
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				m_bGangStatus = true;
				//杠牌得分
				int lScore = bPublic?m_pGameServiceOption->lCellScore :m_pGameServiceOption->lCellScore * 2;
				BYTE cbGangIndex = m_GangScore[wChairID].cbGangCount++;
				m_GangScore[wChairID].bMingGang = bPublic;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if(  i == wChairID ) continue;

					m_GangScore[wChairID].lScore[cbGangIndex][i] = -lScore;
					m_GangScore[wChairID].lScore[cbGangIndex][wChairID] += lScore;
				}

				//发送信息
				CMD_S_GangScore gs;
				memset(&gs, 0, sizeof(gs));
				gs.wChairId = wChairID;
				ZeroMemory( &gs,sizeof(gs) );
				gs.cbXiaYu = bPublic?FALSE:TRUE;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( i == wChairID ) continue;

					gs.lGangScore[i] = -lScore;
					gs.lGangScore[wChairID] += lScore;
				}
				m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );
				m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );

				//构造结果
				CMD_S_OperateResult OperateResult;
				ZeroMemory( &OperateResult,sizeof(OperateResult) );
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard[0]=cbOperateCard[0];
				OperateResult.cbOperateCard[1]=cbOperateCard[1];
				OperateResult.cbOperateCard[2]=cbOperateCard[2];
				addGameRecordAction(OperateResult);

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//效验动作
				bool bAroseAction=false;
				if (bPublic==true) bAroseAction=EstimateUserRespond(wChairID,cbOperateCard[0],EstimatKind_GangCard);

				//发送扑克
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID,true);
				}

				return true;
			}
		case WIK_CHI_HU:		//吃胡操作
			{
				//普通胡牌
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],&m_cbProvideCard,1) )
				{
					ASSERT( FALSE );
					return false;
				}
				m_dwChiHuKind[wChairID] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,
					m_ChiHuRight[wChairID]);
				m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;

				//结束信息
				m_cbChiHuCard=m_cbProvideCard;

				//结束游戏
				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

				//JUST FOR TEST
				for(int nn=0; nn<GAME_PLAYER; nn++)
				{
					BYTE	cbCardData[14];
					m_GameLogic.SwitchToCardData(m_cbCardIndex[nn],cbCardData);
					CString strLog;
					for(int k=0; k<14; k++)
					{
							strLog += m_GameLogic.GetPaiString(cbCardData[k]);
							strLog += L" ";
					}
					TRACE(_T("AAA操作后，第%ld个玩家数据：\n"), nn);
					TRACE(strLog);
					TRACE(_T("\n"));
				}
				return true;
			}
		}
		//JUST FOR TEST
		for(int nn=0; nn<GAME_PLAYER; nn++)
		{
			BYTE	cbCardData[14];
			m_GameLogic.SwitchToCardData(m_cbCardIndex[nn],cbCardData);
			CString strLog;
			for(int k=0; k<14; k++)
			{
					strLog += m_GameLogic.GetPaiString(cbCardData[k]);
					strLog += L" ";
			}
			TRACE(_T("AAA操作后，第%ld个玩家数据：\n"), nn);
			TRACE(_T("AAA")+strLog);
			TRACE(_T("\n"));
		}
		return true;
	}

	return false;
}

//用户补牌
bool CTableFrameSink::OnUserReplaceCard(WORD wChairID, BYTE cbCardData)
{
	//错误断言
	ASSERT(wChairID==m_wReplaceUser);
	ASSERT(m_GameLogic.IsHuaCard(cbCardData)==true);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wReplaceUser) return false;
	if (m_GameLogic.IsHuaCard(cbCardData)==false) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false)  return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//状态变量
	m_bSendStatus=true;
	m_cbSendCardData=0x00;

	//花牌数量统计
	m_cbHuaCardCount[wChairID]++;
	if(m_cbHuaCardCount[wChairID] >= 4){
		int lScore =wChairID == m_wBankerUser? m_pGameServiceOption->lCellScore * 2: m_pGameServiceOption->lCellScore;
				lScore*=((m_cbHuaCardCount[wChairID]-4)==0)?1:(m_cbHuaCardCount[wChairID]-4)*2;
				BYTE cbGangIndex = m_GangScore[wChairID].cbGangCount++;
				m_GangScore[wChairID].bMingGang = false;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if(  i == wChairID ) continue;

					m_GangScore[wChairID].lScore[cbGangIndex][i] -= (wChairID == m_wBankerUser)?lScore*2:lScore;
					m_GangScore[wChairID].lScore[cbGangIndex][wChairID] += (wChairID == m_wBankerUser)?lScore*2:lScore;
				}

				//发送信息
				CMD_S_GangScore gs;
				gs.wChairId = wChairID;
				ZeroMemory( &gs,sizeof(gs) );
				gs.cbXiaYu = FALSE;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( i == wChairID ) continue;

					gs.lGangScore[i] -= (wChairID == m_wBankerUser)?lScore*2:lScore;
					gs.lGangScore[wChairID] += (wChairID == m_wBankerUser)?lScore*2:lScore;
				}
				m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );
				m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );
	}	

	//丢弃扑克
	m_cbDiscardCount[wChairID]++;
	m_cbDiscardCard[wChairID][m_cbDiscardCount[wChairID]-1]=cbCardData;

	//构造数据
	CMD_S_ReplaceCard ReplaceCard;
	ReplaceCard.wReplaceUser=wChairID;
	ReplaceCard.cbReplaceCard=cbCardData;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

	//派发扑克
	DispatchCardData(wChairID);

	return true;
}

//发送操作
bool CTableFrameSink::SendOperateNotify()
{
	//发送提示
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//构造数据
			CMD_S_OperateNotify OperateNotify;
			ZeroMemory(&OperateNotify, sizeof(CMD_S_OperateNotify));
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard= (m_cbProvideCard == 0)? m_cbOperateCard[i][0] : m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//能杠，给1个牌就够了
			CopyMemory(OperateNotify.cbGangCardData,  &OperateNotify.cbActionCard, 1);
			//CopyMemory(OperateNotify.cbGangCardData+1, m_cbOperateCard[i], 1);
			//CopyMemory(OperateNotify.cbGangCardData+2, m_cbOperateCard[i], 1);
			//CopyMemory(OperateNotify.cbGangCardData+3, m_cbOperateCard[i], 1);
			OperateNotify.cbGangCardCount=1;
			TRACE("AAA 等待玩家%ld 响应指令：%ld , 目标用户%ld的牌%s\n",m_wResumeUser, OperateNotify.cbActionMask, m_wProvideUser, m_GameLogic.GetPaiString(m_cbProvideCard ));

			//发送数据
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bTail)
{
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;
	ASSERT( m_bSendStatus );
	if( m_bSendStatus == false ) return false;

	//丢弃扑克
	if ((m_wReplaceUser==INVALID_CHAIR)&&(m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//荒庄结束
	if (m_cbLeftCardCount==m_cbEndLeftCount)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//设置变量
	if( m_wReplaceUser == INVALID_CHAIR )
	{
		m_cbOutCardData=0;
		m_wCurrentUser=wCurrentUser;
		m_wOutCardUser=INVALID_CHAIR;
		ZeroMemory(m_cbEnjoinHuCard[wCurrentUser],sizeof(m_cbEnjoinHuCard[wCurrentUser]));
	}

	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	//加牌
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//补花判断
	if ((m_wReplaceUser!=INVALID_CHAIR)||(m_GameLogic.IsHuaCard(m_cbSendCardData)==true))
	{
		m_wReplaceUser=INVALID_CHAIR;

		//用户判断
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//用户索引
			WORD wUserIndex=(wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

			//扑克判断
			for (BYTE j=MAX_INDEX-MAX_HUA_CARD;j<MAX_INDEX;j++)
			{
				if (m_cbCardIndex[wUserIndex][j]>0)
				{
					m_wReplaceUser=wUserIndex;
					break;
				}
			}

			if( m_wReplaceUser != INVALID_CHAIR ) break;
		}
	}

	if( m_wCurrentUser == wCurrentUser )
	{
		//设置变量
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;
	}

	if( m_wReplaceUser == INVALID_CHAIR )
	{
		//禁止吃胡，只能自摸
		//if( m_bEnjoinChiHu[m_wCurrentUser] )
		if( m_wCurrentUser == wCurrentUser )
		{
			//胡牌判断
			CChiHuRight chr;
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
			m_cbUserAction[m_wCurrentUser] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wCurrentUser],m_WeaveItemArray[m_wCurrentUser],
				m_cbWeaveItemCount[m_wCurrentUser],m_cbProvideCard,chr);
			if(!(chr&CHR_MAGIC_EYE).IsEmpty())
					{
						m_cbUserAction[m_wBankerUser] |= WIK_MAKE_EYE;
					}
			if(!(chr&CHR_THREE_MAGIC).IsEmpty())
		{
			m_cbUserAction[m_wBankerUser] |= WIK_MAKE_EYE_THREE;
		}
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
		}

		//杠牌判断；
		if ((m_bEnjoinChiPeng[m_wCurrentUser]==false)&&(m_cbLeftCardCount>m_cbEndLeftCount))
		{
			tagGangCardResult GangCardResult;
			memset(&GangCardResult, 0, sizeof(tagGangCardResult));
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);
			
			m_kUserActionStates2.m_cbUserAction[m_wCurrentUser] = m_cbUserAction[m_wCurrentUser];
			m_kUserActionStates2.m_cbOperateCard[m_wCurrentUser] = GangCardResult.cbCardData[0];
		}
	}

	//堆立信息
	ASSERT( m_wHeapHead != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR );
	if( !bTail )
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHead][0]+m_cbHeapCardInfo[m_wHeapHead][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHead=(m_wHeapHead+1)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapHead][0]++;
	}
	else
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}

	//构造数据
	CMD_S_SendCard SendCard;
	SendCard.cbSendCardUser = wCurrentUser;
	SendCard.wReplaceUser = m_wReplaceUser;
	SendCard.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
	SendCard.bTail = bTail;
	SendCard.cbActionMask=m_wReplaceUser==INVALID_CHAIR?m_cbUserAction[m_wCurrentUser]:WIK_NULL;
	SendCard.cbCardData=(m_wCurrentUser==wCurrentUser)?m_cbProvideCard:m_cbSendCardData;
	SendCard.cbGangCardIndeed = m_kUserActionStates2.m_cbOperateCard[m_wCurrentUser];		//特别说明：碰牌后又遇到这个牌，可以杠但是没有杠，以后每次发牌都提示杠牌（但是杠的不是发的这张牌）
	TRACE("AAA玩家：%ld 派发牌:%x %s", wCurrentUser, SendCard.cbCardData, m_GameLogic.GetPaiString(SendCard.cbCardData));
	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	m_kUserActionStates2.m_cbOperateCard[m_wCurrentUser]=0;
	return true;
}


//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY)
	{
		setChairTrustee(wChairID,true);
	}
	return true;
}
//用户重入
bool CTableFrameSink::OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY)
	{
		setChairTrustee(wChairID,false);
	}
	return true;
}

//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//变量定义
	bool bAroseAction=false;

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//禁止操作
	WORD i=0;
	for ( i=0;i<GAME_PLAYER;i++) 
		if(m_bOutMagicCard[i])break;
	bool bEnjoinOutMagic = (i<GAME_PLAYER);

	//动作判断:没有打财神牌，才可以进来；打了财神牌，就不进入
	if(!bEnjoinOutMagic)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//用户过滤
			if (wCenterUser==i) continue;

			//出牌类型
			if (EstimatKind==EstimatKind_OutCard)
			{
				//吃碰判断
				if (m_bEnjoinChiPeng[i]==false)
				{
					//碰牌判断
					m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

					//吃牌判断
					WORD wEatUser=(wCenterUser+GAME_PLAYER-1)%GAME_PLAYER;
					if (wEatUser==i)
						m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);

				}
				//杠牌判断(2016.12.31修改：禁止吃碰，不禁止杠牌)
				if (m_cbLeftCardCount>m_cbEndLeftCount) 
				{
					m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				}
			}
			if ( EstimatKind==EstimatKind_PengCard)
			{

			}
			////是否无花大吊：只剩下两张手牌；只有无花大吊才可以接炮
			bool bDaDiao = m_GameLogic.IsDaDiao(m_cbCardIndex[i]);
			if(bDaDiao && (m_GameLogic.getMagicCardCount(m_cbCardIndex[i]) == 0))
				bDaDiao=true;
			else
				bDaDiao=false;

			//胡牌判断
			//if (m_bEnjoinChiHu[i]==false && m_cbLandCount>3 && (wCenterUser==m_wBankerUser || i==m_wBankerUser))
			//只能自摸；仅有大吊可以接炮（吃胡），仅有杠可以接炮
			if ( !m_bEnjoinChiHu[i] &&( bDaDiao || m_bGangStatus))
			{
				//胡牌判断;这个for循环 允许一炮多响
				BYTE j=0;
				for( j=0;j<(MAX_COUNT-1)/2;j++)
				{
					if(m_cbEnjoinHuCard[i][j]==cbCenterCard)break;
				}
				if(j==((MAX_COUNT-1)/2))
				{
					//吃胡判断
					CChiHuRight chr;
					BYTE cbWeaveCount=m_cbWeaveItemCount[i];
					BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr);
					//排除爆头，因为暴头不能抢杠
					if(m_bGangStatus && !(chr & CHR_MAGIC_HU).IsEmpty())
						;
					else
						m_cbUserAction[i] |= cbAction;
					if(!(chr&CHR_MAGIC_EYE).IsEmpty())
					{
						m_cbUserAction[i] |= WIK_MAKE_EYE;
					}
					if(!(chr&CHR_THREE_MAGIC).IsEmpty())
		{
			m_cbUserAction[m_wBankerUser] |= WIK_MAKE_EYE_THREE;
		}
				}

				//记录胡牌
				if((m_cbUserAction[i]&WIK_CHI_HU)!=0)
				{
					BYTE j=0;
					for(;j<(MAX_COUNT-1)/2;j++)
					{
						if(m_cbEnjoinHuCard[i][j]==0)break;
					}
					ASSERT(j<(MAX_COUNT-1)/2);
					if((j<(MAX_COUNT-1)/2))
						m_cbEnjoinHuCard[i][j] = cbCenterCard;		//此牌禁止胡？
				}
			}

			//结果判断
			if (m_cbUserAction[i]!=WIK_NULL) 
				bAroseAction=true;
		}
	}
	if (bAroseAction==true) 
	{
	//结果处理	//设置变量
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;
		//发送提示
		SendOperateNotify();

		//等待玩家响应操作
		return true;
	}

	//继续发牌
	return false;
}

//算分
BYTE CTableFrameSink::CalScore( WORD wWinner, int lScore[GAME_PLAYER] )
{
	//初始化
	ZeroMemory( lScore,sizeof(LONG)*GAME_PLAYER );

	//这个应该是多飘花
	bool bSecondMagic=false;
	////打出来2个财神，多飘花
	if(m_cbCaiShenOutCnt[wWinner]>=2)
		bSecondMagic=true;
	WORD wFanShuo = m_GameLogic.GetChiHuActionRank( m_ChiHuRight[wWinner] ,bSecondMagic );

	//花牌个数
	ASSERT( wFanShuo > 0 );
	if( wFanShuo == 0 ) return 0;

	//判断包牌
	WORD wPaymentUser=INVALID_CHAIR;
	if(m_cbWeaveItemCount[wWinner]>=3)
	{
		//供牌次数
		WORD wProvideCount[GAME_PLAYER]={};
		for( WORD j = 0; j < m_cbWeaveItemCount[wWinner]; j++ )
		{
			wProvideCount[m_WeaveItemArray[wWinner][j].wProvideUser]++;
		}

		////包牌用户；送吃三次要包陪
		//for( WORD i = 0; i < GAME_PLAYER; i++ )
		//{
		//	if(wProvideCount[i]<3 || wWinner==i)continue;
		//		wPaymentUser=i;
		//	break;
		//}
	}
	if(m_wProvideUser != wWinner)
		wPaymentUser=m_wProvideUser;
	// 最大256 = 16 *8 *2  
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( i == wWinner )continue;
		int lTimes=1;	//((wWinner==m_wBankerUser || i==m_wBankerUser)?INT(pow(2,__min(m_cbLandCount,3))):1);
		switch (m_cbUserMakeEye[wWinner])
		{
		case 1:
			{
				lTimes=4;
			}
			case 2:
			{
				lTimes=8;
			}
			case 3:
			{
				lTimes=16;
			}
		default:
			break;
		}
		int lGameScore = wFanShuo;
		lScore[((wPaymentUser==INVALID_CHAIR)?i:wPaymentUser)] -= lGameScore*lTimes;
		lScore[wWinner] += lGameScore*lTimes;
	}

	return BYTE(wFanShuo);
}

//从系统翻牌获取钻牌索引
BYTE CTableFrameSink::GetMagicIndex( BYTE cbFanCard )
{
	BYTE cbZPaiIndex = m_GameLogic.SwitchToCardIndex(cbFanCard);
	if( cbZPaiIndex < 27 )
	{
		if( (cbZPaiIndex+1)%9 == 0 )
			cbZPaiIndex -= 8;
		else
			cbZPaiIndex++;
	}
	else
	{
		if( cbZPaiIndex + 1 == MAX_INDEX )
			cbZPaiIndex = 27;
		else
			cbZPaiIndex++;
	}
	return cbZPaiIndex;
}


void CTableFrameSink::SetCreateUserID(DWORD dwUserID)
{
	IServerUserItem* pUserItem = m_pITableFrame->SearchUserItem(dwUserID);

	if (pUserItem)
	{
		m_wBankerUser = pUserItem->GetChairID();
	}
}

void CTableFrameSink::SetPrivateInfo(BYTE bGameTypeIdex,DWORD	bGameRuleIdex)
{
//	m_cbGameTypeIdex = bGameTypeIdex;
	m_dwGameRuleIdex = bGameRuleIdex;
	//FvMask::Add(m_dwGameRuleIdex, _MASK_(GAME_TYPE_ZZ_QIDUI));
}


bool CTableFrameSink::isUseTuoGuan()
{
	if (m_pITableFrame->GetGameServiceOption()->wServerType == GAME_GENRE_EDUCATE)
	{
		return false;
	}
	return true;
}

void CTableFrameSink::setChairTrustee(WORD iChairID,bool bTrustee)
{
	if (!isUseTuoGuan())
	{
		return;
	}
	if (iChairID >= GAME_PLAYER)
	{
		return;
	}
	m_bTrustee[iChairID]=bTrustee;

	CMD_S_Trustee Trustee;
	Trustee.bTrustee = bTrustee;
	Trustee.wChairID = iChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
}

void CTableFrameSink::clearUserActionStates(WORD cbChairID)
{
	if (cbChairID == INVALID_CHAIR)
	{
		m_kUserActionStates1.m_cbProvideCard = 0;
		ZeroMemory(m_kUserActionStates1.m_bResponse,sizeof(m_kUserActionStates1.m_bResponse));
		ZeroMemory(m_kUserActionStates1.m_cbUserAction,sizeof(m_kUserActionStates1.m_cbUserAction));
		ZeroMemory(m_kUserActionStates1.m_cbPerformAction,sizeof(m_kUserActionStates1.m_cbPerformAction));

		m_kUserActionStates2.m_cbProvideCard = 0;
		ZeroMemory(m_kUserActionStates2.m_bResponse,sizeof(m_kUserActionStates2.m_bResponse));
		ZeroMemory(m_kUserActionStates2.m_cbUserAction,sizeof(m_kUserActionStates2.m_cbUserAction));
		ZeroMemory(m_kUserActionStates2.m_cbPerformAction,sizeof(m_kUserActionStates2.m_cbPerformAction));
	}
	else
	{
		if (cbChairID >= GAME_PLAYER)
		{
			return;
		}
		m_kUserActionStates1.m_bResponse[cbChairID] = false;
		m_kUserActionStates1.m_cbUserAction[cbChairID] = 0;
		m_kUserActionStates1.m_cbPerformAction[cbChairID] = 0;

		m_kUserActionStates2.m_bResponse[cbChairID] = false;
		m_kUserActionStates2.m_cbUserAction[cbChairID] = 0;
		m_kUserActionStates2.m_cbPerformAction[cbChairID] = 0;
	}
}

//权位过滤//*
void CTableFrameSink::FiltrateRight( WORD wWinner, CChiHuRight &chr )
{
	/*	增加权位	*/
	if(wWinner == m_wProvideUser) 
	{
		//飘杠
		if(m_bOutMagicCard[wWinner] && m_bGangStatus && !(chr&CHR_MAGIC_EYE).IsEmpty())
			chr |= CHR_MAGIC_GANG;
		//杠飘 飘花 杠上开花
		else if(m_bOutMagicCard[wWinner] && m_bOutGangHuCard[wWinner] && !(chr&CHR_MAGIC_EYE).IsEmpty())
			chr |= CHR_GANG_MAGIC;
		//杠暴 杠上开花 + 暴头
		else if(m_bOutGangHuCard[wWinner] && !(chr&CHR_MAGIC_HU).IsEmpty())		//CHR_MAGIC_EYE
			chr |= CHR_GANG_KAI_MAGIC;
		//暴头+飘花=多飘花；多飘花必须连续进行
		else if(!(chr&CHR_MAGIC_HU).IsEmpty() && m_cbCaiShenOutCnt[wWinner]>1)	//暴头才能飘花
		{
			chr |= CHR_MAGIC_HU3;
		}
		//1飘花
		else if(m_bOutMagicCard[wWinner] && !(chr&CHR_MAGIC_HU).IsEmpty())		//暴头才能飘花
		{
			chr |= CHR_MAGIC_HU2;
		}
		//杠开
		else if(m_bGangStatus)
			chr |= CHR_GANG_KAI;
		//手上的花 + 打出去的花数量
		if(m_cbCaiShenOutCnt[wWinner] + m_GameLogic.getMagicCardCount(m_cbCardIndex[wWinner]) > 2 )
			chr |= CHR_THREE_MAGIC;
	}
	//如果还有其他胡法，就去掉平胡
	if(chr != CHR_WU_FAN)
		chr  &=~CHR_WU_FAN;
	else						//无番，只能是自摸（不能接炮，又没有特殊玩法）
		chr |= CHR_HU_ZIMO;

	//计算横翻: 色子成对，色子之和大于等于10点，上局是黄庄，下局翻倍。
	//但是这三种情况加起来也只能翻一倍。
	BYTE cbSice1 = HIBYTE(m_wCaiShenSice);
	BYTE cbSice2 = LOBYTE(m_wCaiShenSice);
	if((cbSice1 == cbSice2) || (cbSice1+cbSice2)>=10 || m_cbHuangZhuang)
	{
		//2片起胡才有横翻
		if(m_lStartHuScore == 2)
			chr |= CHR_HENG_FAN;
	}
	DWORD	dwRight[3];
	if(!chr.IsEmpty())
		chr.GetRightData(dwRight, 3);
	CString strLog=_T("AAA");
	if(dwRight[0] & CHR_GANG_KAI )			strLog +=   _T("杠开 ");
	if(dwRight[0] &CHR_GANG_KAI_MAGIC )		strLog += 	_T("杠暴 ");
	if(dwRight[0] &CHR_MAGIC_HU)			strLog += 	_T("暴头 ");
	if(dwRight[0] &CHR_MAGIC_HU2)			strLog += 	_T("飘花 ");
	if(dwRight[0] &CHR_MAGIC_HU3 )
											strLog +=   _T("多飘花 ");
	if(dwRight[0] &CHR_GANG_MAGIC)			strLog += 	_T("杠飘 ");
	if(dwRight[0] &CHR_MAGIC_GANG)			strLog += 	_T("飘杠 ");
	if(dwRight[0] &CHR_MAGIC_EYE)			strLog += 	_T("财神吊牌 ");
	if(dwRight[0] &CHR_NO_MAGIC_DADIAO)		strLog += 	_T("无花大调 ");
	if(dwRight[0] &CHR_HU_ZIMO)				strLog += 	_T("自摸 ");
	if(dwRight[0] &CHR_THREE_MAGIC)			strLog += 	_T("三花 ");
	if(dwRight[0] &CHR_HENG_FAN)			strLog += 	_T("横翻 ");
	if(dwRight[0] &CHR_GANG_FAN)			strLog += 	_T("杠分 ");
	if(dwRight[0] &CHR_WU_FAN)				strLog += 	_T("平胡 ");
		
	OutputDebugString(strLog);
		OutputDebugString(_T("\n"));
	/*	调整权位	*/
	////边张	不计坎张
	//if( !(chr&CHR_BIAN_ZHANG).IsEmpty() )
	//	chr &= ~CHR_KAN_ZHANG;
}

int CTableFrameSink::getLeftCardNum()
{
	int nLeftCardNum = 0;
	if (hasRule(GAME_TYPE_ZZ_LIUJU_4CARD))
	{
		nLeftCardNum = m_cbLeftCardCount-4;
	}
	else
	{
		nLeftCardNum = m_cbLeftCardCount;
	}
	return nLeftCardNum;
}


//去掉一个财神牌
int CTableFrameSink::RemoveOneMagicCard(BYTE cbMagicCard)
{
#define  swapCard(a,b)\
	BYTE nTemp = a;\
	a = b;\
	b = nTemp;\

	bool bValue = false;
	for (int i=m_cbLeftCardCount-1;i>=0;i--)
	{
		if (m_cbRepertoryCard[i] == cbMagicCard)
		{
			bValue = true;
			swapCard(m_cbRepertoryCard[i],m_cbRepertoryCard[m_cbLeftCardCount-1]);
			m_cbLeftCardCount--;
			break;
		}
	}
	return bValue;
}
//////////////////////////////////////////////////////////////////////////
