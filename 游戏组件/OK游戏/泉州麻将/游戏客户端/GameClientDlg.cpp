#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
#include ".\gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////

//游戏定时器
#define IDI_START_GAME				200									//开始定时器
#define IDI_OPERATE_CARD			202									//操作定时器
#define IDT_AUTO					1									//代打定时
#define IDT_REPAIR_FLOWER			2									//补花定时器
#define IDI_OPEN_DOOR				203									//开门定时器
#define IDI_OPEN_GOLD				204									//开金定时器
#define IDI_REPAIR_FLOWER			205									//补花定时器

//游戏定时器
#define TIME_START_GAME				40									//开始定时器
#define TIME_HEAR_STATUS			1									//出牌定时器
#define TIME_OPERATE_CARD			150//15									//操作定时器
#define TIME_OPEN_DOOR				1//15									//开门限时
#define TIME_OPEN_GOLD				1//15									//开金限时

//windows定时器
#define TIME_AUTO					2000								//托管定时器

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

BEGIN_MESSAGE_MAP(CGameClientDlg,CGameFrameEngine)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_AUTO,OnAuto)
	ON_MESSAGE(IDM_OPEN_DOOR,OnOpenDoor)
	ON_MESSAGE(IDM_OPEN_GOLD,OnOpenGold)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameEngine()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//状态变量
	m_bAuto=false;
	m_wDoorDiceCount = 0;
	m_wGoldDiceCount = 0;
	m_cbSubtractTailCount = 0;
	m_cbSubtractHeadCount = 0;

	//堆立变量
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_cbGoldCard,sizeof(m_cbGoldCard));

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));
	//m_cMsg=NULL;


	//m_GameClientView.m_btAutoOn.EnableWindow(true);
	//m_GameClientView.m_btAutoOn.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btAutoOff.EnableWindow(false);
	//m_GameClientView.m_btAutoOff.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenDoor.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenGold.ShowWindow(SW_HIDE);
	m_GameClientView.SetGoldCardShow(false);
	m_GameClientView.SetGoldOutEnable(false);

	m_GameClientView.SetDoorDice(0);
	m_GameClientView.SetGoldDice(0);

	
	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
	
}

//初始函数
bool CGameClientDlg::OnInitGameEngine()
{
	//设置标题
	SetWindowText(TEXT("泉州麻将游戏"));


	//时间设置
	/* SYSTEMTIME SysTime;
	 GetSystemTime(&SysTime);
	 if(SysTime.wDay>10)
	 {
		MessageBox(TEXT("当前系统使用已经过期！"),TEXT("使用过期"),MB_OK|MB_ICONWARNING);
		m_GameClientView.ShowWindow(SW_HIDE);
		this->EndDialog(true);
		 return false;
	 }*/

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);//by hxh 20170308
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载资源
	g_CardResource.LoadResource();

	return true;
}

//重置框架
bool CGameClientDlg::OnResetGameEngine()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//状态变量
	m_bAuto=false;
	m_wDoorDiceCount = 0;
	m_wGoldDiceCount = 0;
	m_cbSubtractTailCount = 0;
	m_cbSubtractHeadCount = 0;

	//堆立变量
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_cbGoldCard,sizeof(m_cbGoldCard));



	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));

	//m_GameClientView.m_btAutoOn.EnableWindow(true);
	//m_GameClientView.m_btAutoOn.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btAutoOff.EnableWindow(false);
	//m_GameClientView.m_btAutoOff.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenDoor.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenGold.ShowWindow(SW_HIDE);
	m_GameClientView.SetGoldCardShow(false);
	m_GameClientView.SetGoldOutEnable(false);

	m_GameClientView.SetDoorDice(0);
	m_GameClientView.SetGoldDice(0);


	return true;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	////构造数据
	//CGameOption GameOption;
	//GameOption.m_bEnableSound=IsEnableSound();

	////配置数据
	//if (GameOption.DoModal()==IDOK)
	//{
	//	EnableSound(GameOption.m_bEnableSound);
	//}

	return;
}

//时间消息
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//开始游戏
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return false;
			}
			if (nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_OPERATE_CARD:		//操作定时器
		{
			//超时判断
			if ((nElapse==0)&&(IsLookonMode()==false))
			{
				//获取位置
				WORD wMeChairID=GetMeChairID();

				//动作处理
				if (wChairID==wMeChairID)
				{					
					if(m_wCurrentUser==wMeChairID)
					{
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
						OnOutCard(cbCardData,cbCardData);
					}
					else OnCardOperate(WIK_NULL,0);
				}

				return false;
			}

			//播放声音
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_OPEN_DOOR:
		{

			//超时判断
			if((nElapse==0)&&(wChairID==GetMeChairID()))
			{		
				OnOpenDoor(0,0);

				return false;
			}
			return true;
		}

	case IDI_OPEN_GOLD:
		{

			//超时判断
			if((nElapse==0)&&(wChairID==GetMeChairID()))
			{

				OnOpenGold(0,0);

				return false;
			}
			return true;
		}
	case IDI_REPAIR_FLOWER://补花状态
		{
			//超时判断
			if((nElapse==0)&&(wChairID==GetMeChairID()))
			{
				//删除定时器
				KillGameClock(IDI_REPAIR_FLOWER);
				

				//发送消息
				CMD_C_RepairFlower RepairFlower;
				RepairFlower.cbCurrentCard = m_cbOutCardData;
				RepairFlower.wChairID = GetMeChairID();

				SendSocketData(SUB_C_REPAIRFLOWER,&RepairFlower,sizeof(RepairFlower));	
				

			}
			return true;
		}
	
	}

	return false;
}

bool CGameClientDlg::OnEventGameClockKill(WORD t)
{
	return true;
}

//旁观状态
bool CGameClientDlg::OnEventLookonMode(void * pBuffer, WORD wDataSize)
{
	//扑克控制
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//网络消息
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//用户出牌
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//发牌消息
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//操作提示
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//操作结果
		{
			return OnSubOperateResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_OPENDOOR:		//开门状态
		{
			return OnSubOpenDoor(pBuffer,wDataSize);
		}
	case SUB_S_OPENGOLD:		//开金状态
		{
			return OnSubOpenGold(pBuffer,wDataSize);
		}
	case SUB_S_END_FLOWER:		//补花开始
		{
			return OnSubEndFlower(pBuffer,wDataSize);

		}
	case SUB_S_GOLD_SWIM:		//游牌状态
		{
			return OnSubGoldSwim(pBuffer,wDataSize);
		}
	}

	return true;
}

//游戏场景
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//设置数据
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);

			//设置界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{				
				m_cbHeapCardInfo[i][0]=0;				
				m_cbHeapCardInfo[i][1]=0;
				m_GameClientView.m_InsideHeapCard[SwitchViewChairID(i)].ResetMinusMiddleCount();
				m_GameClientView.m_InsideHeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],INSIDE_HEAP_FULL_COUNT,0);
				m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(i)].ResetMinusMiddleCount();
				m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],OUTSIDE_HEAP_FULL_COUNT,0);
				
			}

			//设置控件
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
			}

			//更新界面
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}	
	case GS_MJ_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//设置变量
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;

			//历史变量
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));

			//扑克变量
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//辅助变量
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//界面设置
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//组合扑克
			BYTE cbWeaveCard[5]={0,0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard);
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);

					if (cbWeaveKind&WIK_GANG)
					{
						if (m_WeaveItemArray[i][j].wProvideUser!=i) m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					}
				}
			}

			//用户扑克
			if (m_wCurrentUser==GetMeChairID())
			{
				BYTE cbCardCount=pStatusPlay->cbCardCount;
				m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,cbCardCount-1,pStatusPlay->cbCardData[cbCardCount-cbCardCount],m_cbGoldCard,4);
			}
			else m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,0,m_cbGoldCard,4); 

			//扑克设置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//用户扑克
				if (i!=GetMeChairID())
				{
					BYTE cbCardCount=17-m_cbWeaveCount[i]*3;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//丢弃扑克
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(m_cbDiscardCard[i],m_cbDiscardCount[i]);
			}

			//控制设置
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			}

			//堆立扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=0;
			}

			//分发扑克
			BYTE cbTakeCount=136-m_cbLeftCardCount;
			BYTE cbSiceFirst=HIBYTE(pStatusPlay->wSiceCount);
			BYTE cbSiceSecond=LOBYTE(pStatusPlay->wSiceCount);
			WORD wTakeChairID=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//计算数目				
				BYTE cbValidCount=INSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceSecond-1)*2:0);
				BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

				//提取扑克
				cbTakeCount-=cbRemoveCount;
				m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

				//完成判断
				if (cbTakeCount==0)
				{
					m_wHeapHand=wTakeChairID;
					m_wHeapTail=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
					break;
				}

				//切换索引
				wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
			}

			//堆立界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{				
				m_GameClientView.m_InsideHeapCard[wViewChairID[i]].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],INSIDE_HEAP_FULL_COUNT,0);
			}

			//历史扑克
			if (m_wOutCardUser!=INVALID_CHAIR)
			{
				WORD wOutChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wOutChairID,m_cbOutCardData);
			}

			//出牌设置
			if (cbGameStation==GS_MJ_PLAY)
			{
				//操作界面
				if ((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
				{
					//获取变量
					BYTE cbActionMask=pStatusPlay->cbActionMask;
					BYTE cbActionCard=pStatusPlay->cbActionCard;

					//变量定义
					tagGangCardResult GangCardResult;
					ZeroMemory(&GangCardResult,sizeof(GangCardResult));

					//杠牌判断

					if ((cbActionMask&WIK_GANG)!=0)
					{
						//桌面杆牌
						if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
						{
							GangCardResult.cbCardCount=1;
							GangCardResult.cbCardData[0]=cbActionCard;
						}

						//自己杆牌
						if ((m_wCurrentUser==GetMeChairID())||(cbActionCard==0))
						{
							WORD wMeChairID=GetMeChairID();
							m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
						}
					}	


					//设置界面					
					m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
					if (m_wCurrentUser==INVALID_CHAIR)
					{
						if(m_bAuto)
							SetTimer(IDT_AUTO,TIME_AUTO,NULL);
						SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					}
				}

				//设置时间
				if (m_wCurrentUser!=INVALID_CHAIR)
				{
					//计算时间
					WORD wTimeCount=TIME_OPERATE_CARD;					

					//设置时间
					SetGameClock(m_wCurrentUser,TIME_OPERATE_CARD,wTimeCount);
				}
			}

			//更新界面
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GS_MJ_BEFOREPLAY:
		{
			return true;
		}
	}

	return false;
}

//游戏开始
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

#ifdef USE_RS_PRINT
    CMD_S_GameStart GameStart;
	memcpy(&GameStart,pGameStart,sizeof(CMD_S_GameStart));
	SYSTEMTIME timeCreateFile={0};	// 建档的时间
	GetSystemTime(&timeCreateFile);	// 获得当前时间

	wstring  strLogFile=L"";
	strLogFile=s2ws(GetAppPath())+L"\\QZMJClient\\NetMsg.txt";
	wstring wstrDirName=s2ws(GetAppPath())+L"\\QZMJClient";
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

	CString strLog;
	strLog.Format(_T("CMD_S_GameStart 庄家用户wBankerUser=[%d],当前用户wCurrentUser=[%d]"),GameStart.wBankerUser,GameStart.wCurrentUser);
	outf<<CT2A(strLog)<<endl;
#endif

	//设置状态
	SetGameStatus(GS_MJ_BEFOREPLAY);

	//设置变量	
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1);

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//设置扑克
	BYTE cbCardCount = MAX_COUNT-1;
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//设置界面
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	m_bAuto=false;
	//m_GameClientView.m_btAutoOn.EnableWindow(true);
	//m_GameClientView.m_btAutoOn.ShowWindow (SW_SHOW);
	//m_GameClientView.m_btAutoOff.EnableWindow(true);
	//m_GameClientView.m_btAutoOff.ShowWindow(SW_HIDE);	

	//旁观界面
	if (bPlayerMode==false)
	{
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	}

	//堆立扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=0;
	}

	//分发扑克
	BYTE cbTakeCount=(MAX_COUNT-1)*GAME_PLAYER;

	//扑克设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wViewChairID=SwitchViewChairID(i);

		//组合界面
		m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][4].SetDisplayItem(true);
		m_GameClientView.m_InsideHeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],0,0);		
		m_GameClientView.m_OutSideHeapCard[wViewChairID].ResetMinusMiddleCount();
		m_GameClientView.m_OutSideHeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][0],OUTSIDE_HEAP_FULL_COUNT,0);

		//用户扑克
		if (wViewChairID!=2)
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			
			m_GameClientView.m_UserCard[wIndex].SetCardData(CountArray(pGameStart->cbCardData),0);
		}
		else
		{	
		
			m_GameClientView.m_HandCardControl.SetCardData(pGameStart->cbCardData,MAX_COUNT-1,0,m_cbGoldCard,4);
				
		}

		//旁观界面
		if (bPlayerMode==false)
		{
			m_GameClientView.m_TableCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][4].SetCardData(NULL,0);
		}
	}

	//出牌提示
	if ((bPlayerMode!=false)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		WORD wMeChairID=GetMeChairID();
		if (m_wCurrentUser==wMeChairID)
		{
			m_GameClientView.SetStatusFlag(!m_bAuto,false);

			//控件设置
			m_GameClientView.m_btOpenDoor.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOpenDoor.EnableWindow(TRUE);
		}
	}	

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//激活框架
	if (bPlayerMode!=false) ActiveGameFrame();

	//环境处理
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//设置时间
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		SetGameClock(m_wCurrentUser,IDI_OPEN_DOOR,TIME_OPEN_DOOR);
	}

	return true;
}

//用户出牌
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//消息处理
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//变量定义
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;

	//其他用户
	if ((IsLookonMode()!=false)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//环境设置
		KillGameClock(IDI_OPERATE_CARD); 
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

		//出牌界面
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

		//设置扑克
		if (wOutViewChairID==2)
		{
			//删除扑克
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);

			//设置扑克
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0,m_cbGoldCard,4);
		}
		else
		{
			WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}		
	}

	if(pOutCard->wOutCardUser==GetMeChairID())
	{
		if(GetGameStatus()==GS_MJ_BEFOREPLAY)
		{
			//删除扑克
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);

			//设置扑克
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0,m_cbGoldCard,4);

		}
	}

	return true;
}

//发牌消息
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//设置变量
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//状态设置
	if(pSendCard->bGameSatusPlay)SetGameStatus(GS_MJ_PLAY);

	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//丢弃扑克
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);

		//设置变量
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//花牌变量
	bool bFlowerCard = false;

	//发牌处理
	if (pSendCard->cbCardData!=0)
	{
		

		//取牌界面
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		if (wViewChairID!=2)
		{
			WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
		}
		else if((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
		{			

			//判断状态
			if(GetGameStatus()!=GS_MJ_PLAY)//不是玩家摸牌出牌时刻，所有玩家补花时间
			{
				
				//花牌数组
				//BYTE cbFlowerCard[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};

				//判断是否是花牌
				if(m_GameLogic.IsFlowerCard(pSendCard->cbCardData,m_cbFlowerCard,8))
				{	
					//设置变量
					m_wCurrentUser = pSendCard->wCurrentUser;
					m_cbOutCardData = pSendCard->cbCardData;

					//设置定时器
					SetTimer(IDT_REPAIR_FLOWER,100,NULL);
					bFlowerCard =  true;
				}

				if(bFlowerCard==false)//当前麻将不是花牌
				{
					m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;

					//手中有没有花牌
					if(m_GameLogic.IsHaveFlowerCard(m_cbCardIndex,m_cbFlowerCard,8))
					{
						//获取花牌
						BYTE cbCard = m_GameLogic.GetFlowerCard(m_cbCardIndex,m_cbFlowerCard,8);
						if(cbCard!=0)
						{
							//设置变量
							m_wCurrentUser = pSendCard->wCurrentUser;
							m_cbOutCardData = cbCard;

							//删除扑克
							m_GameLogic.RemoveCard(m_cbCardIndex,cbCard);

							//设置定时器
							SetTimer(IDT_REPAIR_FLOWER,100,NULL);
						}
					}
				}
			}
			else //游戏状态
			{
				//花牌数组
				//BYTE cbFlowerCard[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};

				//判断是否是花牌				
				if(m_GameLogic.IsFlowerCard(pSendCard->cbCardData,m_cbFlowerCard,8))
				{
					//设置变量
					m_wOutCardUser = pSendCard->wCurrentUser;
					m_cbOutCardData = pSendCard->cbCardData;

					//设置定时器
					SetTimer(IDT_REPAIR_FLOWER,100,NULL);
					bFlowerCard = true;
				}

				if(bFlowerCard==false)
				{
					m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;
				}			

			}

			tagCardItem CardItem;
			CardItem.cbCardData = pSendCard->cbCardData;
			CardItem.bShoot = false;
			CardItem.bGoldFlag = m_GameLogic.IsGoldCard(pSendCard->cbCardData,m_cbGoldCard,4);
			m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);

			//m_GameClientView.m_HandCardControl.SetCurrentCard(pSendCard->cbCardData);
			
			


			
		}
		
		
		//扣除扑克
		if(pSendCard->bRepairFlower)		
			DeductionTableCard(false);
		else
			DeductionTableCard(true);
	}
	BYTE mbHu=WIK_NULL;
	BYTE mbGang=WIK_NULL;

	//变量定义
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//当前用户
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//激活框架
		ActiveGameFrame();


		//获取变量
		BYTE cbActionCard=pSendCard->cbCardData;
		BYTE cbActionMask=pSendCard->cbActionMask;
		

		//动作处理
		if (pSendCard->cbActionMask!=WIK_NULL)
		{
			

			//杠牌判断
			if ((cbActionMask&WIK_GANG)!=0)
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}
			

		
			m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);

		}
		m_GameClientView.m_HandCardControl.SetPositively(true);
	}

	if((bFlowerCard==false)&&(GetGameStatus()==GS_MJ_PLAY))//状态判断
	{

		//出牌提示
		m_GameClientView.SetStatusFlag(!m_bAuto && (IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

	

		//计算时间
		WORD wTimeCount=TIME_OPERATE_CARD;

		//设置时间	
		if(m_bAuto && m_wCurrentUser==GetMeChairID())
			SetTimer(IDT_AUTO,TIME_AUTO,NULL);	
	
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);
	



	return true;
}

//操作提示
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//用户界面
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//获取变量
		BYTE cbActionMask=pOperateNotify->cbActionMask;
		BYTE cbActionCard=pOperateNotify->cbActionCard;

		//变量定义
		tagGangCardResult GangCardResult;
		ZeroMemory(&GangCardResult,sizeof(GangCardResult));

		//杠牌判断
		if ((cbActionMask&WIK_GANG)!=0)
		{
			//桌面杆牌
			if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
			{
				GangCardResult.cbCardCount=1;
				GangCardResult.cbCardData[0]=cbActionCard;
			}

			//自己杆牌
			if ((m_wCurrentUser==GetMeChairID())||(cbActionCard==0))
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}
		}		
		

		if(m_bAuto)
			SetTimer(IDT_AUTO,TIME_AUTO,NULL);
		

		//设置界面
		ActiveGameFrame();		
		SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		
		
		m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
	}

	return true;
}

//操作结果
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//变量定义
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);

	//出牌变量
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//设置组合
	if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)
	{
		//设置变量
		m_wCurrentUser=INVALID_CHAIR;

		//组合扑克
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
				break;
			}
		}

		//组合扑克
		if (cbWeaveIndex==0xFF)
		{
			//暗杠判断
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;
			
			//设置扑克
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		}

		//组合界面
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		bool bShow=(cbPublicCard==TRUE)?true:false;
		if((IsLookonMode()==false)&&(GetMeChairID()==pOperateResult->wProvideUser))bShow=true;
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem(bShow);
		

		//扑克设置
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard)]=0;
		}

		//设置扑克
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0,m_cbGoldCard,4);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
		}
	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		//设置变量
		m_wCurrentUser=pOperateResult->wOperateUser;

		//设置组合
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//组合界面
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);

		//删除扑克
		if (GetMeChairID()==wOperateUser)
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
			m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		}

		//设置扑克
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1],m_cbGoldCard,4);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}
	}

	//设置界面
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	m_GameClientView.SetStatusFlag(!m_bAuto && (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);



	//设置时间
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//计算时间
		WORD wTimeCount=TIME_OPERATE_CARD;


		//设置时间
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}

	return true;
}

//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	
	//杠描述
/*	char msg[60];
	memset(msg,0,sizeof(msg));
	char temp_buff[60];
	memset(temp_buff,0,sizeof(temp_buff));*/
	/*for(WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		for(WORD k=0;k<4;k++)
		{
			if(m_WeaveItemArray[i][k].cbWeaveKind==WIK_GANG)
			{
				if(i==m_WeaveItemArray[i][k].wProvideUser)
				{
					sprintf(temp_buff," %s杠牌 ",pUserData->szName);
					strcat(msg,temp_buff);
				}
				else
				{
					sprintf(temp_buff," %s明杠 ",pUserData->szName);
					strcat(msg,temp_buff);
				}
			}
			else continue;
		}
	}*/

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//设置状态
	SetGameStatus(GS_MJ_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	//删除定时器
	KillGameClock(IDI_OPERATE_CARD);

	//设置控件
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.m_btOpenDoor.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenGold.ShowWindow(SW_HIDE);

	//结束设置
	if (pGameEnd->cbChiHuCard!=0)
	{
		tagGameScoreInfo GameScoreInfo;
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.cbChiHuCard=pGameEnd->cbChiHuCard;
		GameScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		GameScoreInfo.lCellScore = pGameEnd->lCellScore;
		//strcat(pGameEnd->strEnd,msg);
		CopyMemory(&GameScoreInfo.m_cHu,pGameEnd->strEnd,sizeof(pGameEnd->strEnd) );
		GameScoreInfo.bGoOnBankerTime=pGameEnd->bGoOnBankerTime;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(i);
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
			GameScoreInfo.wCardType[i]=pGameEnd->wChiHuKind[i];
			GameScoreInfo.bFlowerCount[i]=pGameEnd->bFlowerCount[i];
			GameScoreInfo.bGoldCount[i]=pGameEnd->bGoldCount[i];
			GameScoreInfo.bAllPlantCard[i]=pGameEnd->bAllPlantCard[i];
			GameScoreInfo.bAllSeasonCard[i]=pGameEnd->bAllSeasonCard[i];
			GameScoreInfo.bAllSidesCard[i]=pGameEnd->bAllSidesCard[i];
			GameScoreInfo.bGangCount[i]=pGameEnd->bGangCount[i];
			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->GetNickName(),CountArray(GameScoreInfo.szUserName[i]));
		}
		m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);
		
	}
	else 
	{
		DeductionTableCard(true);
		//test
		tagGameScoreInfo GameScoreInfo;
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.lCellScore = pGameEnd->lCellScore;
		GameScoreInfo.cbChiHuCard=NULL;
		GameScoreInfo.wProvideUser=6;
		//strcat(pGameEnd->strEnd,msg);
		CopyMemory(&GameScoreInfo.m_cHu,pGameEnd->strEnd,sizeof(pGameEnd->strEnd) );
		GameScoreInfo.bGoOnBankerTime=pGameEnd->bGoOnBankerTime;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(i);
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
			GameScoreInfo.wCardType[i]=NULL;
			GameScoreInfo.bFlowerCount[i] = pGameEnd->bFlowerCount[i];
			GameScoreInfo.bGoldCount[i] = pGameEnd->bGoldCount[i];
			GameScoreInfo.bAllPlantCard[i]=pGameEnd->bAllPlantCard[i];
			GameScoreInfo.bAllSeasonCard[i]=pGameEnd->bAllSeasonCard[i];
			GameScoreInfo.bAllSidesCard[i]=pGameEnd->bAllSidesCard[i];
			GameScoreInfo.bGangCount[i]=pGameEnd->bGangCount[i];
			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->GetNickName(),CountArray(GameScoreInfo.szUserName[i]));
		}
		m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);

		m_GameClientView.SetHuangZhuang(true);
	}

	//用户扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->wChiHuKind[i]!=CHK_NULL) m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//设置扑克
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0,NULL,0);

	//播放声音
	LONG lScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//设置界面
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}
	m_bAuto=false;
	//m_GameClientView.m_btAutoOn.EnableWindow(false);
	//m_GameClientView.m_btAutoOff.EnableWindow(false);
	//m_GameClientView.m_btAutoOn.ShowWindow (SW_HIDE);
	//m_GameClientView.m_btAutoOff.ShowWindow (SW_HIDE);

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//扣除扑克
void CGameClientDlg::DeductionTableCard(bool bHeadCard)
{
	if (bHeadCard==true)
	{		

		//判断庄家堆立扑克
		if((m_wHeapHand==m_wBankerUser)&&((OUTSIDE_HEAP_FULL_COUNT-m_wDoorDiceCount+1)>=m_cbSubtractHeadCount))
		{
			//减少扑克
			m_cbLeftCardCount--;			

			//扑克空缺
			WORD wMinusMiddlePos =  OUTSIDE_HEAP_FULL_COUNT-m_wDoorDiceCount-m_cbSubtractHeadCount;

			//堆立扑克
			WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
			WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
			WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];

			m_GameClientView.m_OutSideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,OUTSIDE_HEAP_FULL_COUNT,wMinusMiddlePos);

			//设置变量
			m_cbSubtractHeadCount++;
		}
		else//非庄家堆立扑克
		{
			if(m_wHeapHand==m_wBankerUser)
			{
				//切换索引
				m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);

			}
			else
			{
				//切换索引		
				BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
				if (cbHeapCount==OUTSIDE_HEAP_FULL_COUNT) m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);
			}
			//if(SwitchViewChairID(m_wHeapHand)==0)		

			//减少扑克
			m_cbLeftCardCount--;			
			m_cbHeapCardInfo[m_wHeapHand][0]++;

			//堆立扑克
			WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
			WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
			WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];			
			m_GameClientView.m_OutSideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,OUTSIDE_HEAP_FULL_COUNT,0);
		}

		
		
	}
	else
	{

		//设置变量
		m_cbSubtractTailCount++;


		//跳过金牌麻将
		if(m_cbSubtractTailCount==m_wGoldDiceCount)m_cbSubtractTailCount++;

		//减少扑克
		m_cbLeftCardCount--;

		//判断庄家堆立扑克
		if((m_wHeapTail==m_wBankerUser)&&(m_cbSubtractTailCount<=m_wDoorDiceCount))
		{
			//扑克空缺
			WORD wMinusMiddlePos =  OUTSIDE_HEAP_FULL_COUNT-m_wDoorDiceCount+m_cbSubtractTailCount;

			//堆立扑克
			WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
			WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
			WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];

			m_GameClientView.m_OutSideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,OUTSIDE_HEAP_FULL_COUNT,wMinusMiddlePos);
		}
		else//非庄家堆立扑克
		{
			if(m_wHeapTail==m_wBankerUser)
			{
				//切换索引
				m_wHeapTail = (m_wBankerUser+3)%CountArray(m_cbHeapCardInfo);
			}
			else
			{
				//切换索引		
				BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
				if (cbHeapCount==OUTSIDE_HEAP_FULL_COUNT) m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);	
			}		
					
		
			//堆立扑克	
			m_cbHeapCardInfo[m_wHeapTail][1]++;
			if(m_cbSubtractTailCount==m_wDoorDiceCount)m_cbHeapCardInfo[m_wHeapTail][1]++;
			WORD wHeapViewID=SwitchViewChairID(m_wHeapTail);
			WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapTail][0];
			WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapTail][1];		
			//m_GameClientView.m_InsideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,INSIDE_HEAP_FULL_COUNT);
			m_GameClientView.m_OutSideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,OUTSIDE_HEAP_FULL_COUNT,0);
	
		}
	}

	return;
}

//显示控制
bool CGameClientDlg::ShowOperateControl(BYTE cbUserAction, BYTE cbActionCard)
{
	//变量定义
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//杠牌判断
	if ((cbUserAction&WIK_GANG)!=0)
	{
		//桌面杆牌
		if (cbActionCard!=0)
		{
			GangCardResult.cbCardCount=1;
			GangCardResult.cbCardData[0]=cbActionCard;
		}

		//自己杆牌
		if (cbActionCard==0)
		{
			WORD wMeChairID=GetMeChairID();
			m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
		}
	}

	
	//显示界面
	m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbUserAction,GangCardResult);

	return true;
}

//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//环境设置
	KillGameClock(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetGoldCardShow(false);

	m_GameClientView.SetDoorDice(0);
	m_GameClientView.SetGoldDice(0);	


	//设置界面
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);


	//扑克设置
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0,NULL,0);
	m_GameClientView.m_GoldCardControl.SetCardData(NULL,0,0,NULL,0);

	//扑克设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][0].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][1].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][2].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][3].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][4].SetCardData(NULL,0);
	}

	//堆立扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		
		BYTE bHeapFullCount=0;
		m_cbHeapCardInfo[i][0]=0;			
		m_cbHeapCardInfo[i][1]=0;
		m_GameClientView.m_InsideHeapCard[SwitchViewChairID(i)].ResetMinusMiddleCount();
		m_GameClientView.m_InsideHeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],INSIDE_HEAP_FULL_COUNT,0);
		m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(i)].ResetMinusMiddleCount();
		m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],OUTSIDE_HEAP_FULL_COUNT,0);
	}

	//状态变量
	m_wDoorDiceCount = 0;
	m_wGoldDiceCount = 0;
	m_cbSubtractTailCount = 0;
	m_cbSubtractHeadCount = 0;

	//游戏变量
	m_wCurrentUser=INVALID_CHAIR;

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_cbGoldCard,sizeof(m_cbGoldCard));

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//堆立扑克
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//出牌操作
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//出牌判断
	if ((IsLookonMode()!=false)||(m_wCurrentUser!=GetMeChairID())) return 0;
	

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	BYTE cbOutCardData=(BYTE)wParam;
	m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData);

	//设置扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0,m_cbGoldCard,4);

	//设置界面
	KillGameClock(IDI_OPERATE_CARD);
	m_GameClientView.InvalidGameView(0,0,0,0);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);	

	//发送数据
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	return 0;
}

//扑克操作
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard=(BYTE)(LOWORD(lParam));

	//状态判断
	if ((cbOperateCode==WIK_NULL)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
		return 0;
	}

	//环境设置
	KillGameClock(IDI_OPERATE_CARD);
	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//发送命令
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

LRESULT CGameClientDlg::OnAuto(WPARAM wParam, LPARAM lParam)
{
	if(wParam!=0)
	{
		m_bAuto=true;
		if(GetMeChairID()==m_wCurrentUser)
			OnEventGameClockInfo(GetMeChairID(), 0, IDI_OPERATE_CARD);
	}
	else
	{
		m_bAuto=false;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////

void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDT_AUTO:
		KillTimer(IDT_AUTO);
		OnEventGameClockInfo(GetMeChairID(), 0, IDI_OPERATE_CARD);
		CGameFrameEngine::OnTimer(nIDEvent);
		break;	

	case IDT_REPAIR_FLOWER:
		KillTimer(IDT_REPAIR_FLOWER);
		OnEventGameClockInfo(GetMeChairID(), 0, IDI_REPAIR_FLOWER);
		CGameFrameEngine::OnTimer(nIDEvent);
		break;
	
	}
	return;
}

//开门操作
LRESULT CGameClientDlg::OnOpenDoor(WPARAM wParam,LPARAM lParam)
{

	//控件设置
	m_GameClientView.m_btOpenDoor.ShowWindow(SW_HIDE);

	//删除定时器
	KillGameClock(IDI_OPEN_DOOR);

	SendSocketData(SUB_C_OPENDOOR);

	return 0;
}

//开金操作
LRESULT CGameClientDlg::OnOpenGold(WPARAM wParam,LPARAM lParam)
{

	//控件设置
	m_GameClientView.m_btOpenGold.ShowWindow(SW_HIDE);

	//删除定时器
	KillGameClock(IDI_OPEN_GOLD);

	SendSocketData(SUB_C_OPENGOLD);

	return 0;
}



// 开门状态
bool CGameClientDlg::OnSubOpenDoor(const void * pBuffer, WORD wDataSize)
{

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OpenDoor));
	if(wDataSize!=sizeof(CMD_S_OpenDoor))return false;

	//转换数据
	CMD_S_OpenDoor *pOpenDoor = (CMD_S_OpenDoor *)pBuffer;


	//变量设置
	m_wDoorDiceCount = pOpenDoor->wDiceCount;	
	

	//控件设置
	if(GetMeChairID()==pOpenDoor->wOpenGoldUser)
	{
		m_GameClientView.m_btOpenGold.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOpenGold.EnableWindow(TRUE);
	}


	//计算门牌位置
	BYTE cbDoorCardPos = OUTSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[m_wBankerUser][1]-m_wDoorDiceCount;

	//界面设置
	m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(m_wBankerUser)].SetCardData(m_cbHeapCardInfo[m_wBankerUser][0],m_cbHeapCardInfo[m_wBankerUser][1],OUTSIDE_HEAP_FULL_COUNT,cbDoorCardPos);
	m_GameClientView.SetDoorDice(m_wDoorDiceCount);

	//设置扑克
	WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
	if (wViewChairID!=2)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
	}
	else if((IsLookonMode()==false)&&(m_wBankerUser==GetMeChairID()))
	{			
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOpenDoor->cbDoorCard)]++;
		m_GameClientView.m_HandCardControl.SetCurrentCard(pOpenDoor->cbDoorCard);
		
	}

	//时间设置
	SetGameClock(pOpenDoor->wOpenGoldUser,IDI_OPEN_GOLD,TIME_OPEN_GOLD);
	return true;
}

// 开金状态
bool CGameClientDlg::OnSubOpenGold(const void * pBuffer, WORD wDataSize)
{

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OpenGold));
	if(wDataSize!=sizeof(CMD_S_OpenGold))return false;

	//转换数据
	CMD_S_OpenGold *pOpenGold = (CMD_S_OpenGold *)pBuffer;

	//设置变量
	CopyMemory(m_cbGoldCard,pOpenGold->cbGoldCard,sizeof(m_cbGoldCard));
	CopyMemory(m_cbFlowerCard,pOpenGold->cbFlowerCard,sizeof(m_cbFlowerCard));
	m_wGoldDiceCount = pOpenGold->wDiceCount;
	m_wHeapHand = m_wBankerUser;
	m_wHeapTail = m_wBankerUser;

	//控件设置
	BYTE cbGoldCard[2];
	ZeroMemory(cbGoldCard,sizeof(cbGoldCard));

	cbGoldCard[0] = pOpenGold->cbKeyGoldCard;
	m_GameClientView.m_GoldCardControl.SetCardData(cbGoldCard,1,0,NULL,0);
	m_GameClientView.m_GoldCardControl.SetDisplayItem(true);
	m_GameClientView.SetGoldCardShow(true);
	m_GameClientView.SetGoldDice(m_wGoldDiceCount);
	

	//金牌位置
	BYTE cbGoldCardPos;

	//计算金牌界面位置
	if(m_wGoldDiceCount<=m_wDoorDiceCount)//金牌出现在庄家方
	{
		cbGoldCardPos = OUTSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[m_wBankerUser][1]-m_wDoorDiceCount+m_wGoldDiceCount;

		//界面设置
		m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(m_wBankerUser)].SetCardData(m_cbHeapCardInfo[m_wBankerUser][0],m_cbHeapCardInfo[m_wBankerUser][1],OUTSIDE_HEAP_FULL_COUNT,cbGoldCardPos);
	}
	else//金牌不出现在庄家方
	{
		//堆立扑克位置
		WORD wChairID = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;

		//cbGoldCardPos = OUTSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[wChairID][1]-(m_wGoldDiceCount-m_wDoorDiceCount);
		cbGoldCardPos = (m_wGoldDiceCount-m_wDoorDiceCount)%OUTSIDE_HEAP_FULL_COUNT;

		//界面设置
		m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(wChairID)].SetCardData(m_cbHeapCardInfo[wChairID][0],m_cbHeapCardInfo[wChairID][1],OUTSIDE_HEAP_FULL_COUNT,cbGoldCardPos);
	}

	//减少扑克
	m_cbLeftCardCount--;

	return true;
}

// 补花结束
bool CGameClientDlg::OnSubEndFlower(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if(wDataSize!=sizeof(CMD_S_SendCard))return false;

	//设置状态
	SetGameStatus(GS_MJ_PLAY);

	//设置变量
	m_wCurrentUser = m_wBankerUser;

	//获取信息
	CMD_S_SendCard *pSendCard = (CMD_S_SendCard*)pBuffer;

	BYTE mbHu=WIK_NULL;
	BYTE mbGang=WIK_NULL;

	//变量定义
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//当前用户
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//激活框架
		ActiveGameFrame();


		//获取变量
		BYTE cbActionCard=pSendCard->cbCardData;
		BYTE cbActionMask=pSendCard->cbActionMask;
		

		//动作处理
		if (pSendCard->cbActionMask!=WIK_NULL)
		{
			//杠牌判断
			if ((cbActionMask&WIK_GANG)!=0)
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}
		
			m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);

		}	

		//设置显示
		//m_GameLogic.RemoveCard(m_cbCardIndex,pSendCard->cbCardData);
		//m_GameClientView.m_HandCardControl.SetCardData(m_cbCardIndex,MAX_COUNT-1,0,m_cbGoldCard,4);
		/*tagCardItem CardItem;
		CardItem.bGoldFlag=m_GameLogic.IsGoldCard(pSendCard->cbCardData,m_cbGoldCard,4);
		CardItem.bShoot=false;
		CardItem.cbCardData=pSendCard->cbCardData;
		m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);*/

		BYTE cbCardData[MAX_COUNT];
		BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
		m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1],m_cbGoldCard,4);
		

		
	}

	m_GameClientView.m_HandCardControl.SetPositively(true);
	//出牌提示
	m_GameClientView.SetStatusFlag(!m_bAuto && (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);

	

	//计算时间
	WORD wTimeCount=TIME_OPERATE_CARD;

	//设置时间	
	if(m_bAuto && m_wCurrentUser==GetMeChairID())
		SetTimer(IDT_AUTO,TIME_AUTO,NULL);	
	
	SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);


	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

// 游牌状态
bool CGameClientDlg::OnSubGoldSwim(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_GoldSwim));
	if(wDataSize!=sizeof(CMD_S_GoldSwim))return false;

	//转换数据
	CMD_S_GoldSwim *pGoldSwim = (CMD_S_GoldSwim *)pBuffer;

	//设置信息
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		//双游或三游
		if((GetMeChairID()==i)&&((pGoldSwim->GoldSwim[i]==GoldSwim_Double)||(pGoldSwim->GoldSwim[i]==GoldSwim_Three)))
		{
			//金牌使能
			m_GameClientView.SetGoldOutEnable(true);
		}
	}
	return true;
}
