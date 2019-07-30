#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//游戏定时器
#define IDI_START_GAME				200									//开始定时器
#define IDI_OPERATE_CARD			201									//操作定时器
#define IDI_REPLACE_CARD			301									//补牌定时器

//游戏定时器
#ifdef _DEBUG
#define TIME_START_GAME				300									//开始定时器
#define TIME_OPERATE_CARD			300									//操作定时器
#define TIME_REPLACE_CARD			500									//补牌定时器
#else
#define TIME_START_GAME				30									//开始定时器
#define TIME_OPERATE_CARD			20									//操作定时器
#define TIME_REPLACE_CARD			1000								//补牌定时器
#endif

#define MAX_TIME_OUT				3									//最大超时次数

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

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_SICE_TWO,OnSiceTwo)
	ON_MESSAGE(IDM_SICE_FINISH,OnSiceFinish)
	ON_MESSAGE(IDM_CAI_SHEN_SICE_FINISH,OnCaiShenSiceFinish)
	ON_MESSAGE(IDM_LAND_SICE_FINISH,OnLandSiceFinish)
	ON_MESSAGE(IDM_USER_ACTION,OnUserAction)
	ON_MESSAGE(IDM_MOVE_CARD_FINISH,OnMoveCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg()
{
	//游戏变量
	//g_cbChangeCard=0;
	g_CardResource.m_cbChangeCard=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbListenStatus = 0;
	m_bWillHearStatus=false;
	m_lSiceCount = 0;
	m_wCaiShenSice=0;				
	m_wCaiShenIndex=0;				
	m_wCaiShenChairID=INVALID_CHAIR;
	m_wFirstUser=INVALID_CHAIR;		
	m_wLandSice=0;					
	m_cbLandCount=1;					

	//托管变量
	m_bStustee=false;
	m_wTimeOutCount =0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory( m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );

	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::OnInitGameEngine()
{
	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载资源
	g_CardResource.LoadResource();

	//加载声音
	m_DirectSound[0].Create( TEXT("OUT_CARD") );
	m_DirectSound[1].Create( TEXT("SEND_CARD") );
	m_DirectSound[2].Create( TEXT("GAME_START") );

	//test
	//BYTE cbCardData[] = {
	//	0x33,0x33,
	//	0x31,0x31,
	//	0x31,0x31,
	//	0x34,0x37,
	//	0x35,0x37,
	//	0x11,0x11,0x37,
	//		//0x13,0x12,0x16,0x15,0x22,0x23,0x25,0x26,0x05,
	//		//0x03,0x03,0x03,0x03
	//};
	//BYTE cbCardIndex[MAX_INDEX]={};
	//m_GameLogic.SwitchToCardIndex(cbCardData,sizeof(cbCardData),cbCardIndex);
	//BYTE cbValue=m_GameLogic.IsQiXiaoDui(cbCardIndex,NULL,0,0x11);
	//if(cbValue)
	//{
	//	CString str;
	//	str.Format(TEXT("%ld"),cbValue);
	//	MessageBox(str);
	//	PostMessage(WM_CLOSE,0,0);
	//}


	//BYTE cbCardCount = 13;
	//BYTE cbCurrentCard = 0x06;


	//BYTE cbCardIndex[MAX_INDEX];
	//m_GameLogic.SwitchToCardIndex( cbCardData,cbCardCount,cbCardIndex );

	//CChiHuRight chr;
	//DWORD dw1 = GetTickCount();
	//BYTE byResult = m_GameLogic.AnalyseChiHuCard( cbCardIndex,NULL,0,cbCurrentCard,chr );
	//DWORD dw2 = GetTickCount();
	//DWORD dw = dw2-dw1;
	//TCHAR szBuffer[32] = TEXT("");
	//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),dw );
	//MessageBox( szBuffer );
	//end test

	return true;
}

//重置框架
bool CGameClientDlg::OnResetGameEngine()
{
	//游戏变量
	//g_cbChangeCard=0;
	g_CardResource.m_cbChangeCard=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbListenStatus = 0;
	m_bWillHearStatus=false;
	m_lSiceCount = 0;
	m_wCaiShenSice=0;				
	m_wCaiShenIndex=0;				
	m_wCaiShenChairID=INVALID_CHAIR;
	m_wFirstUser=INVALID_CHAIR;		
	m_wLandSice=0;					
	m_cbLandCount=1;	
	
	//托管变量
	m_bStustee=false;
	m_wTimeOutCount =0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	return true;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	//构造数据
	CGameOption GameOption;
//	GameOption.m_bEnableSound=IsEnableSound();
	//GameOption.m_bAllowLookon = IsAllowUserLookon();
	GameOption.m_bEanbleAnimate = m_GameClientView.IsEnableAnimate();
	
	//配置数据
	if (GameOption.DoModal()==IDOK)
	{
//		EnableSound(GameOption.m_bEnableSound);
	//	AllowUserLookon(0,GameOption.m_bAllowLookon);
		m_GameClientView.EnableAnimate(GameOption.m_bEanbleAnimate);
		m_DirectSound[0].EnableSound(GameOption.m_bEnableSound);
		m_DirectSound[1].EnableSound(GameOption.m_bEnableSound);
	}

	return;
}

//时间消息
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_START_GAME:		//开始游戏
		{
			if( m_bStustee && nElapse < TIME_START_GAME )
			{
				OnStart(0,0);
				return true;
			}
			if (nElapse==0)
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return true;
			}
			if (nElapse<=5)
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_OPERATE_CARD:		//操作定时器
		{
			//自动出牌
			bool bAutoOutCard=false;
			if ((bAutoOutCard==true)&&(m_GameClientView.m_ControlWnd.IsWindowVisible()))
				bAutoOutCard=false;
			if((bAutoOutCard==false)&&(m_bStustee==true))
			{
				bAutoOutCard=true;
			}

			//超时判断
			if ( (IsLookonMode()==false)&&
				( nElapse==0 || bAutoOutCard || (m_cbListenStatus&&!m_bWillHearStatus&&nElapse<TIME_OPERATE_CARD) ) )
			{
				//获取位置
				WORD wMeChairID=GetMeChairID();

				//动作处理
				if (wChairID==wMeChairID)
				{
					//玩家未听牌,且未托管,则累加超时次数
					if( m_cbListenStatus == 0 && m_bStustee==false && ++m_wTimeOutCount>=MAX_TIME_OUT )
					{
						m_wTimeOutCount = 0;
						OnStusteeControl(TRUE,0);
//						m_pIStringMessage->InsertSystemString(TEXT("由于您多次超时，切换为“系统托管”模式."));
					}

					if (m_wCurrentUser==wMeChairID)
					{
						//获取扑克
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();

						//听牌状态
						if( m_cbListenStatus >= 2 )
						{
							if( m_cbActionMask & WIK_CHI_HU )
							{
								OnUserAction( WIK_CHI_HU,0 );
								return true;
							}
						}
						//如果在操作模式下
						if( m_GameClientView.m_HandCardControl.GetSelectMode() )
						{
							OnCardOperate(0,0);
							return true;
						}
						//出牌效验
						if (VerdictOutCard(cbCardData)==false)
						{
							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								//出牌效验
								if (m_cbCardIndex[i]==0) continue;
								if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false) 
									continue;

								//设置变量
								cbCardData=m_GameLogic.SwitchToCardData(i);
							}
						}

						//出牌动作
						ASSERT( m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData)] > 0 );
						OnOutCard(cbCardData,cbCardData);
					}
					else 
					{
						//听牌状态,点炮或自摸和点炮
						if( m_cbListenStatus == 1 || m_cbListenStatus == 3 )
						{
							if( m_cbActionMask & WIK_CHI_HU )
							{
								OnUserAction( WIK_CHI_HU,0 );
								return true;
							}
						}
						//如果在操作模式下
						if( m_GameClientView.m_HandCardControl.GetSelectMode() )
						{
							OnCardOperate(0,0);
							return true;
						}
						OnUserAction(WIK_NULL,0);
					}
				}

				return true;
			}

			//播放声音
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}

	return true;
}

//旁观状态
bool CGameClientDlg::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//扑克控制
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//网络消息
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	//停止色子动画
	if(SUB_S_GAME_END==wSubCmdID)SetGameStatus(GS_MJ_FREE);
	m_GameClientView.m_DrawSiceWnd.StopSicing();
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
			StopMoveCard();
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//操作结果
		{
			StopMoveCard();
			return OnSubOperateResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			StopMoveCard();
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_TRUSTEE:			//用户托管
		{
			return OnSubTrustee(pBuffer,wDataSize);
		}
	case SUB_S_LISTEN:			//玩家听牌
		{
			return OnSubListen( pBuffer,wDataSize );
		}
	case SUB_S_REPLACE_CARD:	//补花
		{
			return OnSubReplaceCard( pBuffer,wDataSize );
		}
	}

	return true;
}

//游戏场景
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//是否启用声音
//			if( !IsEnableSound() )
			{
				m_DirectSound[0].EnableSound(FALSE);
				m_DirectSound[1].EnableSound(FALSE);
				m_DirectSound[2].EnableSound(FALSE);
			}

			//设置数据
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			//托管设置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
			}

			//设置控件
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}

			//丢弃效果
			m_GameClientView.SetDiscUser(INVALID_CHAIR);

			//更新界面
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GS_MJ_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//是否启用声音
//			if( !IsEnableSound() )
			{
				m_DirectSound[0].EnableSound(FALSE);
				m_DirectSound[1].EnableSound(FALSE);
				m_DirectSound[2].EnableSound(FALSE);
			}

			//辅助变量
			WORD wMeChairID=GetMeChairID();
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//设置变量
			//g_cbChangeCard=pStatusPlay->cbChangeCard;
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount = pStatusPlay->cbLeftCardCount;
			m_cbListenStatus = pStatusPlay->cbHearStatus[wMeChairID];
			m_wReplaceUser = pStatusPlay->wReplaceUser;
			m_wHeapHead = pStatusPlay->wHeapHead;
			m_wHeapTail = pStatusPlay->wHeapTail;
			m_cbLandCount = pStatusPlay->cbLandCount ;
			g_CardResource.m_cbChangeCard = pStatusPlay->cbChangeCard;
			m_GameClientView.SetLandCount(m_cbLandCount);

			//财神位置
			m_wCaiShenIndex = pStatusPlay->wCaiShenIndex;
			m_wCaiShenChairID = pStatusPlay->wCaiShenChairID;
			m_GameClientView.m_HeapCard[SwitchViewChairID(m_wCaiShenChairID)].SetCardData(m_wCaiShenIndex*2,g_CardResource.m_cbChangeCard);

			//用户名称
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				
				IClientUserItem * pUserData = GetTableUserItem(i);
				ASSERT( pUserData != NULL );
				if( pUserData == NULL ) 
				{
					m_szAccounts[i][0] = 0;
					continue;
				}
				lstrcpy( m_szAccounts[i],pUserData->GetNickName() );
			}

			//旁观
			if( IsLookonMode()==true )
				m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());

			//托管设置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusPlay->bTrustee[i]);
			}
			
			if( pStatusPlay->bTrustee[wMeChairID] )
			{
				OnStusteeControl(0,0);
			}

			m_wTimeOutCount=0;
			if(pStatusPlay->bTrustee[wMeChairID])
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);

			//扑克变量
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//界面设置
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//组合扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wOperateViewID = SwitchViewChairID(i);
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(m_WeaveItemArray[i][j].cbCardData,cbWeaveCardCount);
					if ((cbWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i)) 
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
					m_GameClientView.m_WeaveCard[wOperateViewID][j].SetDirectionCardPos((BYTE)wProviderViewID);

				}
			}

			//用户扑克
			if (m_wCurrentUser==wMeChairID)
			{
				SetHandCardControl(m_cbCardIndex,pStatusPlay->cbSendCardData);
			}
			else
			{
				SetHandCardControl(m_cbCardIndex,0x00);
			}

			//扑克设置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//用户扑克
				if (i!=wMeChairID)
				{
					BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[i]*3-1;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//丢弃扑克
				m_GameClientView.m_DiscardCard[wViewChairID[i]].SetCardData(pStatusPlay->cbDiscardCard[i],pStatusPlay->cbDiscardCount[i]);
			}

			//丢弃效果
			if(pStatusPlay->wOutCardUser != INVALID_CHAIR)
			{
				m_GameClientView.SetDiscUser(SwitchViewChairID(pStatusPlay->wOutCardUser));
				m_GameClientView.SetOutCardInfo( wViewChairID[pStatusPlay->wOutCardUser],pStatusPlay->cbOutCardData );
				m_GameClientView.m_DiscardCard[wViewChairID[pStatusPlay->wOutCardUser]].AddCardItem( pStatusPlay->cbOutCardData );
			}

			//控制设置
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			}

			//堆立界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_HeapCard[wViewChairID[i]].SetHeapCardInfo(pStatusPlay->cbHeapCardInfo[i][0],pStatusPlay->cbHeapCardInfo[i][1]);
			}

			//听牌状态
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( pStatusPlay->cbHearStatus[i] > 0 )
					m_GameClientView.SetUserListenStatus( wViewChairID[i],true );
			}

			//操作界面
			if ((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
			{
				//获取变量
				m_cbActionMask=pStatusPlay->cbActionMask;
				m_cbActionCard=pStatusPlay->cbActionCard;

				//设置界面
				if (m_wCurrentUser==INVALID_CHAIR)
					SetGameClock(wMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				if (IsLookonMode()==false) 
					m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
			}

			//设置时间
			if (m_wCurrentUser!=INVALID_CHAIR)
			{
				//计算时间
				WORD wTimeCount=TIME_OPERATE_CARD;

				//设置时间
				SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
			}
			else if( m_wReplaceUser == wMeChairID && !IsLookonMode() )
				SetTimer( IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL );

			//设置圈风
			m_GameClientView.SetFengQuan( pStatusPlay->cbQuanFeng );

			//更新界面
			m_GameClientView.InvalidGameView(0,0,0,0);

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
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

#ifdef USE_RS_PRINT
    CMD_S_GameStart GameStart;
	memcpy(&GameStart,pGameStart,sizeof(CMD_S_GameStart));
	SYSTEMTIME timeCreateFile={0};	// 建档的时间
	GetSystemTime(&timeCreateFile);	// 获得当前时间

	wstring  strLogFile=L"";
	strLogFile=s2ws(GetAppPath())+L"\\HZMJClient\\NetMsg.txt";
	wstring wstrDirName=s2ws(GetAppPath())+L"\\HZMJClient";
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
	strLog.Format(_T("CMD_S_GameStart 骰子点数lSiceCount=[%d],庄家用户wBankerUser=[%d],当前用户wCurrentUser=[%d],补牌用户wReplaceUser=[%d],圈风cbQuanFeng=[%d],堆立牌头wHeapHead=[%d],堆立牌尾wHeapTail=[%d]"),GameStart.lSiceCount,GameStart.wBankerUser,GameStart.wCurrentUser,GameStart.wReplaceUser,GameStart.cbQuanFeng,GameStart.wHeapHead,GameStart.wHeapTail);
	outf<<CT2A(strLog)<<endl;
#endif

	//用户名称
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		IClientUserItem * pUserData = GetTableUserItem( i );
		ASSERT( pUserData != NULL );
		if( pUserData == NULL ) 
		{
			m_szAccounts[i][0] = 0;
			continue;
		}
		lstrcpy( m_szAccounts[i],pUserData->GetNickName() );
	}

	//设置状态
	SetGameStatus(GS_MJ_PLAY);

	//设置变量
	m_wBankerUser = pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_wReplaceUser = pGameStart->wReplaceUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	m_cbActionMask = pGameStart->cbUserAction;
	m_lSiceCount = pGameStart->lSiceCount;
	m_wCaiShenSice = pGameStart->wCaiShenSice;
	m_wCaiShenIndex = pGameStart->wCaiShenIndex;
	m_wCaiShenChairID = pGameStart->wCaiShenChairID;
	m_wFirstUser = pGameStart->wFirstUser ;
	m_wLandSice = pGameStart->wLandSice ;
	g_CardResource.m_cbChangeCard = pGameStart->cbChangeCard ;
	m_cbLandCount = pGameStart->cbLandCount ;

	//设置扑克
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//设置堆牌头尾
	m_wHeapHead = pGameStart->wHeapHead;
	m_wHeapTail = pGameStart->wHeapTail;
	CopyMemory( m_cbHeapCardInfo,pGameStart->cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );

	//旁观界面
	if (IsLookonMode()==true)
	{
		//设置变量
		m_cbListenStatus = 0;
		m_bWillHearStatus = false;

		//组合扑克
		ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
		ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

		m_GameClientView.m_ScoreControl[0].RestorationData();
		m_GameClientView.m_ScoreControl[1].RestorationData();
		m_GameClientView.m_ScoreControl[2].RestorationData();
		m_GameClientView.m_ScoreControl[3].RestorationData();

		m_GameClientView.SetCurrentUser(INVALID_CHAIR);

		//设置界面
		m_GameClientView.SetDiscUser(INVALID_CHAIR);
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetBankerUser(INVALID_CHAIR);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);

		//扑克设置
		m_GameClientView.m_UserCard[0].SetCardData(0,false);
		m_GameClientView.m_UserCard[1].SetCardData(0,false);
		m_GameClientView.m_UserCard[2].SetCardData(0,false);
		m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

		//扑克设置
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
			for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
				m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
		}

		//堆立扑克
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[i].ResetCardData();
			m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
		}
	}

	//设置界面
	m_GameClientView.SetFengQuan( pGameStart->cbQuanFeng );
	if( m_wFirstUser == INVALID_CHAIR )
		m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//播放开始声音
	m_DirectSound[2].Play();

	//环境处理
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//摇色子动画
	if( m_wFirstUser != INVALID_CHAIR )
	{
		m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,0,(WORD)(m_lSiceCount>>16)/*m_wLandSice*/,1);
		m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);
		//m_GameClientView.m_DrawSiceWnd.StopSicing();
	}
	else
	{
		m_GameClientView.SetLandCount(m_cbLandCount);

		//摇色子动画
		m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,0,(WORD)(m_lSiceCount>>16)/*m_wLandSice*/,1);
		//m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,(WORD)(m_lSiceCount>>16),(WORD)m_lSiceCount);
		m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);
		//m_GameClientView.m_DrawSiceWnd.StopSicing();
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

	if( IsLookonMode() || pOutCard->wOutCardUser != GetMeChairID() )
	{
		COutCardItem *pOutCardItem = NULL;
		try
		{
			//创建动画项目
			pOutCardItem = new COutCardItem(pOutCard);
		}
		catch(...)
		{
			ASSERT(FALSE);
			return false;
		}

		//添加动画
		m_MoveCardItemArray.Add(pOutCardItem);

		//播放动画
		BeginMoveCard();
	}

	return true;
}

//发牌消息
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//消息处理
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	CSendCardItem *pSendCardItem = NULL;
	try
	{
		//创建动画项目
		pSendCardItem = new CSendCardItem(pSendCard);
	}
	catch(...)
	{
		ASSERT(FALSE);
		return false;
	}

	//添加动画
	m_MoveCardItemArray.Add(pSendCardItem);

	//播放动画
	BeginMoveCard();

	return true;
}

//补牌消息
bool CGameClientDlg::OnSubReplaceCard(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_ReplaceCard));
	if (wDataSize!=sizeof(CMD_S_ReplaceCard)) return false;

	//变量定义
	CMD_S_ReplaceCard * pReplaceCard=(CMD_S_ReplaceCard *)pBuffer;

	if( IsLookonMode() || pReplaceCard->wReplaceUser != GetMeChairID() )
	{
		CReplaceCardItem *pReplaceCardItem = NULL;
		try
		{
			//创建动画项目
			pReplaceCardItem = new CReplaceCardItem(pReplaceCard);
		}
		catch(...)
		{
			ASSERT(FALSE);
			return false;
		}

		//添加动画
		m_MoveCardItemArray.Add(pReplaceCardItem);

		//播放动画
		BeginMoveCard();
	}

	return true;
}

//操作提示
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return false;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//用户界面
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//获取变量
		WORD wMeChairID=GetMeChairID();
		m_cbActionMask = pOperateNotify->cbActionMask;
		m_cbActionCard = pOperateNotify->cbActionCard;

		//如果手上牌属于操作模式,则取消
		if( m_GameClientView.m_HandCardControl.GetSelectMode() )
			m_GameClientView.m_HandCardControl.SetSelectMode(false);

		//设置界面
		ActiveGameFrame();
		m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);

		//设置时间
		m_GameClientView.SetCurrentUser(INVALID_CHAIR);
		SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	return true;
}

//操作结果
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return false;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//变量定义
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard[3];
	CopyMemory( cbOperateCard,pOperateResult->cbOperateCard,sizeof(cbOperateCard) );
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);
	WORD wProviderViewID = SwitchViewChairID(pOperateResult->wProvideUser);

	//设置变量
	m_wCurrentUser=pOperateResult->wOperateUser;

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
			if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[3] = cbCenterCard;
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
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
			for( BYTE i = 0; i < 4; i++ ) m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[i] = cbOperateCard[0];

			m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);
		}

		//组合界面
		BYTE cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=4;
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,
			cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicCard==TRUE)?true:false);

		//扑克设置
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbOperateCard[0])]=0;
		}

		//设置扑克
		if (GetMeChairID()==wOperateUser)
		{
			SetHandCardControl( m_cbCardIndex,0x00 );
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
		//设置组合
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		CopyMemory( m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,cbOperateCard,sizeof(cbOperateCard) );

		//组合界面
		BYTE cbWeaveCard[3],cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=3;
		CopyMemory( cbWeaveCard,cbOperateCard,sizeof(cbWeaveCard) );
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);

		//删除扑克
		if (GetMeChairID()==wOperateUser)
		{
			VERIFY( m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard[0],1) );
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1) )
			{
				ASSERT( FALSE );
				return false;
			}
		}

		//设置扑克
		if (GetMeChairID()==wOperateUser)
		{
			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}
	}

	//设置界面
	//如果提供玩家不是自己,则去除提供玩家的已出牌
	if( pOperateResult->wProvideUser != wOperateUser )
	{
		if( m_GameClientView.m_DiscardCard[wProviderViewID].GetCardCount() > 0 )
			m_GameClientView.m_DiscardCard[wProviderViewID].RemoveCard(1);
	}
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);
	//如果手上牌属于操作模式,则取消
	if( m_GameClientView.m_HandCardControl.GetSelectMode() )
		m_GameClientView.m_HandCardControl.SetSelectMode(false);

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//环境设置
	PlayActionSound(wOperateUser,pOperateResult->cbOperateCode);

	//设置时间
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//听牌判断
		if( !IsLookonMode() && m_cbListenStatus == 0 && m_wCurrentUser==GetMeChairID() )
		{
			WORD wMeChairId = GetMeChairID();

			if(m_cbCardIndex[m_GameLogic.SwitchToCardIndex(CAI_SHEN_CARD)]==0)
			{
				//m_cbActionMask = m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId]);
			}

			if( m_cbActionMask != WIK_NULL )
			{
				m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
			}
		}

		//计算时间
		WORD wTimeCount=TIME_OPERATE_CARD;

		//设置时间
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
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
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	//设置扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
	}

	//变量定义
	tagScoreInfo ScoreInfo;
	tagWeaveInfo WeaveInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));

	//成绩变量
	ScoreInfo.wMeChairID = GetMeChairID();
	ScoreInfo.wLandUser = m_wBankerUser;
	ScoreInfo.lCellScore = m_GameClientView.GetCellScore();
	ScoreInfo.cbLandCount = m_cbLandCount;
	ScoreInfo.wProvideUser=pGameEnd->wProvideUser[0];
	ScoreInfo.cbProvideCard=pGameEnd->cbProvideCard;
	ScoreInfo.bSecondMagic=pGameEnd->bSecondMagic;
	CopyMemory(ScoreInfo.cbFollowCard,pGameEnd->cbFollowCard,sizeof(ScoreInfo.cbFollowCard));
	CopyMemory(ScoreInfo.cbFollowCount,pGameEnd->cbFollowCount,sizeof(ScoreInfo.cbFollowCount));
	CopyMemory(ScoreInfo.lOutMagicScore,pGameEnd->lOutMagicScore,sizeof(ScoreInfo.lOutMagicScore));
	CopyMemory(ScoreInfo.lFollowScore,pGameEnd->lFollowScore,sizeof(ScoreInfo.lFollowScore));
	CopyMemory(ScoreInfo.lHuScore,pGameEnd->lHuScore,sizeof(ScoreInfo.lHuScore));
	CopyMemory(ScoreInfo.lFanHuScore,pGameEnd->lFanHuScore,sizeof(ScoreInfo.lFanHuScore));

	//设置积分
	CString strTemp ,strEnd = TEXT("本局结束,成绩统计:\r\n");
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		strTemp.Format(_T("%s: %+ld\n"),m_szAccounts[i],pGameEnd->lGameScore[i]);
		strEnd += strTemp;
	}
	//消息积分
	m_pIStringMessage->InsertSystemString((LPCTSTR)strEnd);

	CChiHuRight chr;
	chr.SetRightData( pGameEnd->dwChiHuRight1,CountArray(pGameEnd->dwChiHuRight1) );
	if( pGameEnd->wProvideUser[0] != INVALID_CHAIR )
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//胡牌类型
			ScoreInfo.dwChiHuKind[i]=pGameEnd->dwChiHuKind[i];

			//设置成绩
			ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(ScoreInfo.szUserName[i],m_szAccounts[i],CountArray(ScoreInfo.szUserName[i]));

			//胡牌扑克
			if ((ScoreInfo.cbCardCount==0)&&(pGameEnd->dwChiHuKind[i]!=WIK_NULL))
			{
				ScoreInfo.wChiHuUser = i;
				//组合扑克
				WeaveInfo.cbWeaveCount=m_cbWeaveCount[i];
				for (BYTE j=0;j<WeaveInfo.cbWeaveCount;j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					WeaveInfo.cbPublicWeave[j]=m_WeaveItemArray[i][j].cbPublicCard;
					WeaveInfo.cbCardCount[j]=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					CopyMemory( WeaveInfo.cbCardData[j],m_WeaveItemArray[i][j].cbCardData,sizeof(m_WeaveItemArray[i][j].cbCardData) );
				}

				//设置扑克
				ScoreInfo.cbCardCount=pGameEnd->cbCardCount[i];
				CopyMemory(ScoreInfo.cbCardData,&pGameEnd->cbCardData[i],ScoreInfo.cbCardCount*sizeof(BYTE));

				//提取胡牌
				for (BYTE j=0;j<ScoreInfo.cbCardCount;j++)
				{
					if (ScoreInfo.cbCardData[j]==pGameEnd->cbProvideCard)
					{
						MoveMemory(&ScoreInfo.cbCardData[j],&ScoreInfo.cbCardData[j+1],(ScoreInfo.cbCardCount-j-1)*sizeof(BYTE));
						ScoreInfo.cbCardData[ScoreInfo.cbCardCount-1]=pGameEnd->cbProvideCard;
						break;
					}
				}
			}
			
			PlayActionSound( i,WIK_CHI_HU );
		}

		ScoreInfo.cbFanCount = pGameEnd->cbFanCount;
		ScoreInfo.cbHuaCardCount = pGameEnd->cbHuaCardCount;
		//成绩界面
		m_GameClientView.m_ScoreControl[pGameEnd->wProvideUser[0]].SetScoreInfo(ScoreInfo,WeaveInfo,chr);

	}
	else 
	{
		WORD i=0;
		for ( i=0;i<GAME_PLAYER;i++)
		{
			if(ScoreInfo.lFollowScore[i]!=0)break;
			if(ScoreInfo.lOutMagicScore[i]!=0)break;
		}

		if(i==GAME_PLAYER)
		{
			m_GameClientView.SetHuangZhuang(true);
		}
		//成绩界面
		else 
		{
			//设置成绩
			CChiHuRight chr;
			ScoreInfo.cbHuangZhuang=TRUE;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
				lstrcpyn(ScoreInfo.szUserName[i],m_szAccounts[i],CountArray(ScoreInfo.szUserName[i]));
				IClientUserItem * pUserData = GetTableUserItem(i);
				bool bBoy = (pUserData->GetGender() == 2 ? false : true);
				if(bBoy)
					PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_ZI_MO"));
				else
					PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_ZI_MO"));
	
			m_GameClientView.m_ScoreControl[i].SetScoreInfo(ScoreInfo,WeaveInfo,chr);
			}
		}

	}

	//用户扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->dwChiHuKind[i]!=WIK_NULL) m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//设置扑克
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//播放声音
	if( !chr.IsEmpty() )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( pGameEnd->dwChiHuKind[i] != WIK_NULL )
			{
				//辅助变量
				CString strSoundName=_T("");
				CChiHuRight ChiHuRight;
				ChiHuRight.SetRightData( pGameEnd->dwChiHuRight1,CountArray(pGameEnd->dwChiHuRight1) );

				////天胡
				//if( !(ChiHuRight&CHR_TIAN_HU).IsEmpty() ) 
				//	strSoundName = _T("CHR_TIAN_HU");
				////字一色牌
				//else if( !(ChiHuRight&CHR_ZI_YI_SE).IsEmpty() ) 
				//	strSoundName = _T("CHR_ZI_YI_SE");
				////清一色牌
				//else if( !(ChiHuRight&CHR_QING_YI_SE).IsEmpty() ) 
				//	strSoundName = _T("CHR_QING_YI_SE");
				////抢杠和
				//else if( !(ChiHuRight&CHR_QIANG_GANG).IsEmpty() ) 
				//	strSoundName = _T("CHR_QIANG_GANG");
				////七小对牌
				//else if( !(ChiHuRight&CHR_QI_DUI).IsEmpty() ) 
				//	strSoundName = _T("CHR_QI_DUI");
				////碰碰和
				//else if( !(ChiHuRight&CHR_PENG_PENG).IsEmpty() ) 
				//	strSoundName = _T("CHR_PENG_PENG");
				////全不靠
				//else if( !(ChiHuRight&CHR_QUAN_BU_KAO).IsEmpty() ) 
				//	strSoundName = _T("CHR_QUAN_BU_KAO");
				////地胡
				//else if( !(ChiHuRight&CHR_DI_HU).IsEmpty() ) 
				//	strSoundName = _T("CHR_DI_HU");
				////杠上开花
				//else if( !(ChiHuRight&CHR_GANG_KAI).IsEmpty() ) 
				//	strSoundName = _T("CHR_GANG_KAI");
				////海底捞月
				//else if( !(ChiHuRight&CHR_HAI_DI).IsEmpty() ) 
				//	strSoundName = _T("CHR_HAI_DI");
				////财神会
				//else if( !(ChiHuRight&CHR_CAI_SHEN_HUI).IsEmpty() ) 
				//	strSoundName = _T("CHR_CAI_SHEN_HUI");
				////自摸
				//else if( !(ChiHuRight&CHR_ZI_MO).IsEmpty() ) 
				//	strSoundName = _T("CHR_ZI_MO");
				////平胡
				//else PlayActionSound( i,WIK_CHI_HU );

				if(strSoundName!=_T(""))
				{
					//判断性别
					IClientUserItem * pUserData = GetTableUserItem(i);
					if(pUserData == 0)break;
					bool bBoy = (pUserData->GetGender() == 2 ? false : true);

					if(bBoy)
					{
						strSoundName = _T("BOY_") +strSoundName;
					}
					else
					{
						strSoundName = _T("GIRL_") + strSoundName;
					}
					PlayGameSound(AfxGetInstanceHandle(), strSoundName);
				}

				break;
			}
		}
	}
	else 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));
	}
		

	//设置界面
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	//取消托管
	if(m_bStustee)
		OnStusteeControl(0,0);

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//用户托管
bool CGameClientDlg::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//消息处理
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		IClientUserItem * pUserData = GetTableUserItem(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("玩家[%s]选择了托管功能."),pUserData->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("玩家[%s]取消了托管功能."),pUserData->GetNickName());
		m_pIStringMessage->InsertSystemString(szBuffer);
	}

	return true;
}

//用户听牌
bool CGameClientDlg::OnSubListen(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Listen));
	if (wDataSize!=sizeof(CMD_S_Listen)) return false;

	CMD_S_Listen *pListen = (CMD_S_Listen *)pBuffer;
	WORD wViewId = SwitchViewChairID( pListen->wChairId );

	//设置听牌状态
	if( IsLookonMode() || pListen->wChairId != GetMeChairID() )
	{
		m_GameClientView.SetUserListenStatus( wViewId,true );
	}

	//播放声音
	PlayActionSound( pListen->wChairId,WIK_LISTEN );

	m_GameClientView.SetOutCardInfo( INVALID_CHAIR,0 );
	m_GameClientView.SetUserAction( wViewId,WIK_LISTEN );

	//设置状态
	m_GameClientView.SetStatusFlag( (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),m_wCurrentUser!=GetMeChairID() );

	return true;
}

//转换信息
void CGameClientDlg::ChangeInfo(BYTE bCard,CString &csInfo)
{
	switch(bCard)
	{
	case 0x01:csInfo=TEXT("一万");break;
	case 0x02:csInfo=TEXT("二万");break;
	case 0x03:csInfo=TEXT("三万");break;
	case 0x04:csInfo=TEXT("四万");break;
	case 0x05:csInfo=TEXT("五万");break;
	case 0x06:csInfo=TEXT("六万");break;
	case 0x07:csInfo=TEXT("七万");break;
	case 0x08:csInfo=TEXT("八万");break;
	case 0x09:csInfo=TEXT("九万");break;

	case 0x11:csInfo=TEXT("一条");break;
	case 0x12:csInfo=TEXT("二条");break;
	case 0x13:csInfo=TEXT("三条");break;
	case 0x14:csInfo=TEXT("四条");break;
	case 0x15:csInfo=TEXT("五条");break;
	case 0x16:csInfo=TEXT("六条");break;
	case 0x17:csInfo=TEXT("七条");break;
	case 0x18:csInfo=TEXT("八条");break;
	case 0x19:csInfo=TEXT("九条");break;

	case 0x21:csInfo=TEXT("一筒");break;
	case 0x22:csInfo=TEXT("二筒");break;
	case 0x23:csInfo=TEXT("三筒");break;
	case 0x24:csInfo=TEXT("四筒");break;
	case 0x25:csInfo=TEXT("五筒");break;
	case 0x26:csInfo=TEXT("六筒");break;
	case 0x27:csInfo=TEXT("七筒");break;
	case 0x28:csInfo=TEXT("八筒");break;
	case 0x29:csInfo=TEXT("九筒");break;

	case 0x31:csInfo=TEXT("东风");break;
	case 0x32:csInfo=TEXT("南风");break;
	case 0x33:csInfo=TEXT("西风");break;
	case 0x34:csInfo=TEXT("北风");break;
	case 0x35:csInfo=TEXT("红中");break;
	case 0x36:csInfo=TEXT("发财");break;
	case 0x37:csInfo=TEXT("白板");break;

	//case 0x38:csInfo=TEXT("春夏秋冬");break;
	//case 0x39:csInfo=TEXT("春夏秋冬");break;
	//case 0x3a:csInfo=TEXT("春夏秋冬");break;
	//case 0x3b:csInfo=TEXT("春夏秋冬");break;
	//case 0x3c:csInfo=TEXT("梅兰菊竹");break;
	//case 0x3d:csInfo=TEXT("梅兰菊竹");break;
	//case 0x3e:csInfo=TEXT("梅兰菊竹");break;
	//case 0x3f:csInfo=TEXT("梅兰菊竹");break;

	}
	return ;
}

//播放出牌声音
void CGameClientDlg::PlayCardSound(WORD wChairID, BYTE cbCardData)
{
	if(m_GameLogic.IsValidCard(cbCardData) == false)
	{
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}

	//判断性别
	IClientUserItem * pUserData = GetTableUserItem(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "得不到玩家信息");
		return;
	}

	//转换扑克
	if(g_CardResource.m_cbChangeCard==cbCardData)
	{
		cbCardData=CAI_SHEN_CARD;
	}
	else if(cbCardData==CAI_SHEN_CARD)
	{
		cbCardData=g_CardResource.m_cbChangeCard;
	}

	bool bBoy = (pUserData->GetGender() == 2 ? false : true);
	BYTE cbType= (cbCardData & MASK_COLOR);
	BYTE cbValue= (cbCardData & MASK_VALUE);
	CString strSoundName;

	switch(cbType)
	{
	case 0X30:	//风
		{
			switch(cbValue) 
			{
			case 1:
				{
					strSoundName = _T("F_1_");
					break;
				}
			case 2:
				{
					strSoundName = _T("F_2_");
					break;
				}
			case 3:
				{
					strSoundName = _T("F_3_");
					break;
				}
			case 4:
				{
					strSoundName = _T("F_4_");
					break;
				}
			case 5:
				{
					strSoundName = _T("F_5_");
					break;
				}
			case 6:
				{
					strSoundName = _T("F_6_");
					break;
				}
			case 7:
				{
					strSoundName = _T("F_7_");
					break;
				}
			default:
				{
					strSoundName=_T("BU_HUA");
				}

			}
			break;
		}		
	case 0X20:	//筒
		{
			strSoundName.Format(_T("T_%d_"), cbValue);
			break;
		}

	case 0X10:	//索
		{
			strSoundName.Format(_T("S_%d_"), cbValue);
			break;
		}
	case 0X00:	//万
		{
			strSoundName.Format(_T("W_%d_"), cbValue);
			break;
		}
	}	

	if(bBoy)
	{
		strSoundName = _T("BOY_") +strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL_") + strSoundName;
	}



	for(int i=0;i<100;i++)
	{
		WORD WaveKind=rand()%4+1;
		CString strWaveKind;
		strWaveKind.Format(_T("%d"),WaveKind);
		HINSTANCE hInstance = AfxGetInstanceHandle();
		strSoundName=strSoundName+strWaveKind;
		HRSRC hResour=FindResource(hInstance,strSoundName,TEXT("WAVE"));
		if(hResour==NULL) continue;
		if (hResour!=NULL)
		{
			HGLOBAL hGlobal=LoadResource(hInstance,hResour);
			if (hGlobal!=NULL) 
			{
				//播放声音
				LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
				PlayGameSound(hInstance, strSoundName);

				//清理资源
				UnlockResource(hGlobal);
			}
			FreeResource(hGlobal);
		}
	}

	return;
}

//播放声音
void CGameClientDlg::PlayActionSound(WORD wChairID,BYTE cbAction)
{
	//判断性别
	IClientUserItem * pUserData = GetTableUserItem(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "得不到玩家信息");
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}
	bool bBoy = (pUserData->GetGender() == 2 ? false : true);

	switch (cbAction)
	{
	case WIK_LEFT:
	case WIK_CENTER:
	case WIK_RIGHT:		//上牌
		{
			WORD WaveKind=rand()%3+1;
			CString	strSoundName;

			if(bBoy)
			{
				strSoundName.Format(_T("BOY_CHI_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);
			}
			else
			{
				strSoundName.Format(_T("GIRL_CHI_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);		
			}
			break;
		}
	case WIK_PENG:		//碰牌
		{
			WORD WaveKind=rand()%3+1;
			CString	strSoundName;

			if(bBoy)
			{
				strSoundName.Format(_T("BOY_PENG_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);
			}
			else
			{
				strSoundName.Format(_T("GIRL_PENG_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);	
			}
			break;
		}
	case WIK_GANG:		//杠牌
		{
			WORD WaveKind=rand()%2+1;
			CString	strSoundName;

			if(bBoy)
			{
				strSoundName.Format(_T("BOY_GANG_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);
			}
			else
			{
				strSoundName.Format(_T("GIRL_GANG_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);	
			}
			break;
		}
	case WIK_CHI_HU:	//吃胡
		{

			WORD WaveKind=rand()%2+1;
			CString	strSoundName;

			if(bBoy)
			{
				strSoundName.Format(_T("BOY_CHI_HU_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);
			}
			else
			{
				strSoundName.Format(_T("GIRL_CHI_HU_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);		
			}
			break;
		}
	case WIK_REPLACE:	//替换花牌
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_REPLACE"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_REPLACE"));		
			break;
		}
	case WIK_LISTEN:	//听牌
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_TING"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_TING"));		
			break;
		}
	//case WIK_CHI_HU:
	//	{
	//		if(bBoy)
	//			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHR_ZI_MO"));
	//		else
	//			PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHR_ZI_MO"));		
	//		break;	
	//	}
	}

	return;
}

//出牌判断
bool CGameClientDlg::VerdictOutCard(BYTE cbCardData)
{
	//听牌判断
	if ((m_cbListenStatus>0)||(m_bWillHearStatus==true))
	{
		//变量定义
		CChiHuRight chr;
		WORD wMeChairID=GetMeChairID();
		BYTE cbWeaveCount=m_cbWeaveCount[wMeChairID];

		//构造扑克
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,m_cbCardIndex,sizeof(cbCardIndexTemp));

		//删除扑克
		if( !m_GameLogic.RemoveCard(cbCardIndexTemp,cbCardData) )
		{
			ASSERT( FALSE );
			return false;
		}

		//听牌判断
		BYTE i=0;
		for ( i=0;i<MAX_INDEX;i++)
		{
			//胡牌分析
			BYTE cbCurrentCard=m_GameLogic.SwitchToCardData(i);
			BYTE cbHuCardKind=m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp,m_WeaveItemArray[wMeChairID],cbWeaveCount,cbCurrentCard,chr);

			//结果判断
			if (cbHuCardKind!=WIK_NULL)
			{
				break;
			}
		}

		//听牌判断
		return (i!=MAX_INDEX);
	}

	return true;
}

//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//环境设置
	KillGameClock(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreControl[0].RestorationData();
	m_GameClientView.m_ScoreControl[1].RestorationData();
	m_GameClientView.m_ScoreControl[2].RestorationData();
	m_GameClientView.m_ScoreControl[3].RestorationData();

	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//设置界面
	m_GameClientView.SetDiscUser(INVALID_CHAIR);
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);
	
	//扑克设置
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//扑克设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
	}

	//堆立扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HeapCard[i].ResetCardData();
		m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
	}

	//游戏变量
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbListenStatus = 0;
	m_bWillHearStatus = false;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	//g_cbChangeCard = 0;
	g_CardResource.m_cbChangeCard=0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//出牌操作
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//出牌判断
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))
		return 0;

	//听牌判断
	if (((m_cbListenStatus>0)||(m_bWillHearStatus==true))&&(VerdictOutCard((BYTE)wParam)==false))
	{
		m_pIStringMessage->InsertSystemString(TEXT("出此牌不符合游戏规则!"));
		return 0;
	}

	KillGameClock(IDI_OPERATE_CARD);

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	BYTE cbOutCardData=(BYTE)wParam;
	if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData) )
	{
		ASSERT( FALSE );
		return 0;
	}

	//设置扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	ASSERT( (cbCardCount-1)%3==0 );
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);

	//设置界面
	m_GameClientView.SetCurrentUser( INVALID_CHAIR );
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	if( m_GameClientView.m_DlgListen.m_hWnd )
		m_GameClientView.m_DlgListen.EndDialog(IDCANCEL);

	//构造出牌动画
	COutCardItem *pOutCardItem = NULL;
	try { pOutCardItem = new COutCardItem(); }
	catch(...) { ASSERT(FALSE); return 0; }

	pOutCardItem->m_cbOutCardData = cbOutCardData;
	pOutCardItem->m_wOutCardUser = GetMeChairID();
	m_MoveCardItemArray.Add( pOutCardItem );

	tagMoveCardItem mci;
	mci.cbCardCount = 1;
	mci.cbCardData = cbOutCardData;
	mci.ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	mci.ptTo = m_GameClientView.m_DiscardCard[MYSELF_VIEW_ID].GetDispatchCardPos();
	mci.wViewChairId = MYSELF_VIEW_ID;
	m_GameClientView.OnMoveCardItem( mci );

	//发送数据
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	//播放声音
	PlayCardSound(GetMeChairID(),cbOutCardData);

	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0;
}

//扑克操作
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbOperateCode;
	BYTE cbOperateCard[3] = {0,0,0};

	//删除时间
	KillGameClock(IDI_OPERATE_CARD);

	//提取选择信息
	tagSelectCardInfo si;
	m_GameClientView.m_HandCardControl.GetUserSelectResult( si );
	
	//构造操作信息
	cbOperateCode = (BYTE)si.wActionMask;
	cbOperateCard[0] = si.cbActionCard;
	CopyMemory( &cbOperateCard[1],si.cbCardData,sizeof(BYTE)*2 );

	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//取消操作模式
	m_GameClientView.m_HandCardControl.SetSelectMode( false );

	//如果是听牌
	if( m_bWillHearStatus && (cbOperateCode&WIK_LISTEN) )
	{
		m_bWillHearStatus = false;
		return OnOutCard( cbOperateCard[0],0 );
	}

	//环境设置
	m_GameClientView.SetStatusFlag(false,true);
	
	//发送命令
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

//拖管控制
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	if(IsLookonMode())return 0;

	//设置变量
	m_wTimeOutCount=0;

	//设置状态
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if( FALSE == (BOOL)wParam ) 
			m_pIStringMessage->InsertSystemString(_T("您取消了托管功能."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if( FALSE == (BOOL)wParam )
			m_pIStringMessage->InsertSystemString(_T("您选择了托管功能."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}

	return 0;
}

//第二次摇色子消息
LRESULT CGameClientDlg::OnSiceTwo(WPARAM wParam, LPARAM lParam)
{
	//环境处理
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//提示信息
	CString csInfo;
	csInfo.Format(TEXT("\n庄家摇出骰子是:%d,%d"),HIBYTE((WORD)(m_lSiceCount>>16)),LOBYTE((WORD)(m_lSiceCount>>16)));
	m_pIStringMessage->InsertSystemString(csInfo);

	return 0;
}

//摇色子结束消息
LRESULT CGameClientDlg::OnSiceFinish(WPARAM wParam, LPARAM lParam)
{
	//设置界面
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	//提示信息
	WORD wChairID = HIBYTE((WORD)(m_lSiceCount>>16))+LOBYTE((WORD)(m_lSiceCount>>16))-1;
	wChairID = (wChairID+m_wBankerUser)%GAME_PLAYER;
	CString csInfo;
	csInfo.Format(TEXT("\n[%s]摇出骰子是:%d,%d"),m_szAccounts[wChairID],HIBYTE((WORD)m_lSiceCount),LOBYTE((WORD)m_lSiceCount));
	//InsertGeneralString(csInfo,RGB(0,0,128),true);

	WORD wMeChairID = GetMeChairID();
	bool bPlayerMode = (IsLookonMode()==false);

	//构造发牌动画
	BYTE cbHandCardData[MAX_COUNT];
	m_GameLogic.SwitchToCardData( m_cbCardIndex,cbHandCardData );
	//混乱扑克
	m_GameLogic.RandCardData(cbHandCardData,(m_wBankerUser==wMeChairID)?MAX_COUNT:MAX_COUNT-1,cbHandCardData);
	//确定堆牌删除位置
	WORD wHeapId = m_wHeapTail;
	WORD wHeapCardIndex = m_cbHeapCardInfo[wHeapId][1]-1;
	if(m_cbHeapCardInfo[m_wHeapTail][1]==0)
	{
		wHeapId = (wHeapId+1)%GAME_PLAYER;
		wHeapCardIndex = HEAP_FULL_COUNT-1;
	}
	//发4圈，最后1圈发一张
	for( BYTE i = 0; i < 4; i++ )
	{
		//从庄家开始
		WORD wId = m_wBankerUser;
		for( WORD j = 0; j < GAME_PLAYER; j++ )
		{
			WORD wChairId = (wId+GAME_PLAYER-j)%GAME_PLAYER;
			CStartCardItem *pStartCardItem = NULL;
			//创建动画项目
			try{ pStartCardItem = new CStartCardItem(); }
			catch(...) { ASSERT(FALSE); return 0; }
			//发牌数
			pStartCardItem->m_cbCardCount = i<3?4:1;
			//发牌值
			if( wChairId == wMeChairID )
				CopyMemory( pStartCardItem->m_cbCardData,&cbHandCardData[i*4],sizeof(BYTE)*(i<3?4:1) );
			//发牌玩家
			pStartCardItem->m_wChairId = wChairId;
			//发牌位置
			pStartCardItem->m_wHeapId = wHeapId;
			//堆牌位置
			pStartCardItem->m_wHeapCardIndex = wHeapCardIndex;
			//加入动画数组
			m_MoveCardItemArray.Add(pStartCardItem);

			//推进
			if( i < 3 )
			{
				if( wHeapCardIndex <= 3 )
				{
					wHeapId = (wHeapId+1)%GAME_PLAYER;
					wHeapCardIndex = HEAP_FULL_COUNT-(3-wHeapCardIndex)-1;
				}
				else wHeapCardIndex -= 4;
			}
			else
			{
				if( wHeapCardIndex == 0 )
				{
					wHeapId = (wHeapId+1)%GAME_PLAYER;
					wHeapCardIndex = HEAP_FULL_COUNT-1;
				}
				else wHeapCardIndex--;
			}
		}
	}
	//最后一张，发给庄家
	CStartCardItem *pStartCardItem = NULL;
	//创建动画项目
	try{ pStartCardItem = new CStartCardItem(); }
	catch(...) { ASSERT(FALSE); return 0; }
	//发牌数
	pStartCardItem->m_cbCardCount = 1;
	//发牌值
	if( m_wBankerUser == wMeChairID )
		pStartCardItem->m_cbCardData[0] = cbHandCardData[MAX_COUNT-1];
	//发牌玩家
	pStartCardItem->m_wChairId = m_wBankerUser;
	//发牌位置
	pStartCardItem->m_wHeapId = wHeapId;
	//堆牌位置
	pStartCardItem->m_wHeapCardIndex = wHeapCardIndex;
	//最后一张
	pStartCardItem->m_bLastItem = true;
	//加入动画数组
	m_MoveCardItemArray.Add(pStartCardItem);

	//开始动画
	BeginMoveCard();

	return 0;
}

//庄家骰子
LRESULT CGameClientDlg::OnLandSiceFinish(WPARAM wParam, LPARAM lParam)
{
	//提示信息
	//CString csInfo;
	//csInfo.Format(TEXT("\n[%s]摇出骰子是:%d,%d \n庄家是[%s]"),m_szAccounts[m_wFirstUser],
	//	HIBYTE(m_wLandSice),LOBYTE(m_wLandSice),m_szAccounts[m_wBankerUser]);
	//InsertGeneralString(csInfo,RGB(0,0,128),true);

	//设置庄家
	m_GameClientView.SetLandCount(m_cbLandCount);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//判断结束
	if(GetGameStatus()==GS_MJ_FREE)
	{
		//堆立扑克
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetHeapCardInfo(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1]);
		}
		return 0;
	}
	else
	{
		//环境处理
		PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

		//摇色子动画
		m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,(WORD)(m_lSiceCount>>16),(WORD)m_lSiceCount);
		m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);
		m_GameClientView.m_DrawSiceWnd.StopSicing();
	}

	return 0;
}

//财神骰子
LRESULT CGameClientDlg::OnCaiShenSiceFinish(WPARAM wParam, LPARAM lParam)
{
	//设置界面
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	//提示信息
	CString csInfo,csCard;
	ChangeInfo(g_CardResource.m_cbChangeCard,csCard);
	csInfo.Format(TEXT("\n庄家摇出骰子是:%d,%d  财神:%s"),HIBYTE(m_wCaiShenSice),LOBYTE(m_wCaiShenSice),csCard);
	m_pIStringMessage->InsertSystemString(csInfo);

	//财神骰子
	//g_cbChangeCard = g_CardResource.m_cbChangeCard ;
	m_GameClientView.m_HeapCard[SwitchViewChairID(m_wCaiShenChairID)].SetCardData(m_wCaiShenIndex*2,g_CardResource.m_cbChangeCard);

	if(GetGameStatus()==GS_MJ_FREE)return 0;
 
	//激活框架
	if (!IsLookonMode())
		ActiveGameFrame();

	//手上扑克
	SetHandCardControl(m_cbCardIndex,0x00);

	m_GameClientView.m_HandCardControl.SetPositively(!IsLookonMode());

	//补牌操作
	WORD wMeChairId = GetMeChairID();
	if (!IsLookonMode()&&(m_wReplaceUser==wMeChairId))
	{
		//设置时间
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
	}

	//出牌提示,动作处理
	if (!IsLookonMode()&&(m_wCurrentUser==wMeChairId))
	{
		m_GameClientView.SetStatusFlag(true,false);

		if(m_cbCardIndex[m_GameLogic.SwitchToCardIndex(CAI_SHEN_CARD)]==0)
		{
			//m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,NULL,0);
		}

		if( m_cbActionMask != WIK_NULL )
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
	}
	else if( m_wCurrentUser != wMeChairId && m_wCurrentUser != INVALID_CHAIR )
		m_GameClientView.SetStatusFlag(false,true);

	//设置时间
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	return 0;
}

//设置扑克
void CGameClientDlg::SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard)
{
	//组合数目
	WORD wMeChairID=GetMeChairID();
	BYTE cbWeaveCardCount=m_cbWeaveCount[wMeChairID]*3;

	//转换扑克
	BYTE cbHandCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(cbCardIndex,cbHandCardData);

	//调整扑克
	if ( (cbWeaveCardCount+cbCardCount)==MAX_COUNT )
	{
		if( cbAdvanceCard != 0x00 )
		{
			//删除扑克
			BYTE cbRemoveCard[]={cbAdvanceCard};
			VERIFY( m_GameLogic.RemoveCard(cbHandCardData,cbCardCount,cbRemoveCard,1) );

			//设置扑克
			cbHandCardData[cbCardCount-1]=cbAdvanceCard;
		}
		
		m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount-1,cbHandCardData[cbCardCount-1]);
	}
	else m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount,0x00);

	return;
}

//定时器消息
VOID CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_REPLACE_CARD:	//用户补牌
		{
			//删除时间
			KillTimer(IDI_REPLACE_CARD);

			//设置界面
			m_GameClientView.SetUserAction( INVALID_CHAIR,0 );
			//寻找扑克
			for (BYTE i=MAX_INDEX-MAX_HUA_CARD;i<MAX_INDEX;i++)
			{
				if (m_cbCardIndex[i]>0)
				{
					//变量定义
					BYTE cbReplaceCard=m_GameLogic.SwitchToCardData(i);
					if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard) )
					{
						ASSERT( FALSE );
						return;
					}

					//设置扑克
					SetHandCardControl(m_cbCardIndex,0x00);

					//播放声音
					PlayActionSound(GetMeChairID(),WIK_REPLACE);

					//设置界面
					m_GameClientView.SetUserAction( MYSELF_VIEW_ID,WIK_REPLACE );

					CReplaceCardItem *pReplaceCardItem = NULL;
					try{ pReplaceCardItem = new CReplaceCardItem(); }
					catch(...) { ASSERT(FALSE); return; }
					pReplaceCardItem->m_cbReplaceCard = cbReplaceCard;
					pReplaceCardItem->m_wReplaceUser = GetMeChairID();
					m_MoveCardItemArray.Add( pReplaceCardItem );

					//构造出牌动画
					tagMoveCardItem mci;
					mci.cbCardCount = 1;
					mci.cbCardData = cbReplaceCard;
					mci.wViewChairId = MYSELF_VIEW_ID;
					mci.ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
					mci.ptTo = m_GameClientView.m_DiscardCard[MYSELF_VIEW_ID].GetDispatchCardPos();
					m_GameClientView.OnMoveCardItem( mci );

					//发送数据
					CMD_C_ReplaceCard ReplaceCard;
					ReplaceCard.cbCardData=cbReplaceCard;
					SendSocketData(SUB_C_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

					break;
				}
			}
		
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//玩家操作控件
LRESULT CGameClientDlg::OnUserAction( WPARAM wParam, LPARAM lParam )
{
	//变量定义
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard[3] = {0,0,0};

	//隐藏操作控件
	m_GameClientView.m_ControlWnd.ShowWindow( SW_HIDE );

	//状态判断
	if (cbOperateCode==WIK_NULL)
	{
		//设置变量
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;
		if( m_wCurrentUser == INVALID_CHAIR )
		{
			//删除定时器
			KillGameClock( IDI_OPERATE_CARD );
			//发送消息
			CMD_C_OperateCard OperateCard;
			OperateCard.cbOperateCode = WIK_NULL;
			ZeroMemory( OperateCard.cbOperateCard,sizeof(OperateCard.cbOperateCard) );
			SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
		}
		return 0;
	}

	if( cbOperateCode & WIK_LISTEN )
	{
		ASSERT( m_cbListenStatus == 0 );
		if( m_cbListenStatus != 0 ) return 0;

		//胡牌选项
		if( m_GameClientView.m_DlgListen.DoModal() == IDOK )
		{
			//选择自摸和抓炮
			if( m_GameClientView.m_DlgListen.m_bZhuaPao && m_GameClientView.m_DlgListen.m_bZiMo )
				m_cbListenStatus = 3;
			//只自摸
			else if( m_GameClientView.m_DlgListen.m_bZiMo )
				m_cbListenStatus = 2;
			//只抓炮
			else if( m_GameClientView.m_DlgListen.m_bZhuaPao )
				m_cbListenStatus = 1;
			//如果选择了,则发送听牌消息
			if( m_cbListenStatus > 0 )
			{
				//设置即将听牌标志
				m_bWillHearStatus = true;

				//设置变量
				m_cbActionMask = WIK_NULL;
				m_cbActionCard = 0;

				//设置听牌状态
				m_GameClientView.SetUserListenStatus( 2,true );

				//发送听牌命令
				CMD_C_Listen Listen;
				Listen.cbListen = m_cbListenStatus;
				SendSocketData( SUB_C_LISTEN,&Listen,sizeof(Listen) );

				//判断财神
				if(m_cbCardIndex[m_GameLogic.SwitchToCardIndex(CAI_SHEN_CARD)]==0)
				{
					//获取选择组合
					tagSelectCardInfo sci[MAX_WEAVE];
					BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );

					//如果只听一张牌
					if( cbInfoCount == 1 )
					{
						m_bWillHearStatus = false;
						OnOutCard( sci[0].cbActionCard,0 );
					}
					//否则由玩家选择
					else
					{
						//设置操作事件
						m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );
						//提示消息
						m_pIStringMessage->InsertSystemString(TEXT("请选择胡口牌"));
					}
				}
				else m_bWillHearStatus = false;
			}
		}

		return 0;
	}

	bool bDone = false;
	//胡牌
	if( cbOperateCode & WIK_CHI_HU )
		bDone = true;
	else
	{
		//获取选择组合
		tagSelectCardInfo sci[MAX_WEAVE];
		BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );

		//设置操作事件
		bDone = m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );

		//如果完成操作
		if( bDone )
		{
			//设置操作结果
			tagSelectCardInfo si;
			m_GameClientView.m_HandCardControl.GetUserSelectResult( si );
			cbOperateCode = (BYTE)si.wActionMask;
			cbOperateCard[0] = si.cbActionCard;
			CopyMemory( &cbOperateCard[1],si.cbCardData,2*sizeof(BYTE) );
			m_GameClientView.m_HandCardControl.SetSelectMode( false );
		}
		//否则，设置等待选择
		else m_GameClientView.SetStatusFlag( true,false );
	}

	//如果操作完成，直接发送操作命令
	if( bDone )
	{
		//删除定时器
		KillGameClock( IDI_OPERATE_CARD );

		//设置变量
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;

		CMD_C_OperateCard OperateCard;
		OperateCard.cbOperateCode = cbOperateCode;
		CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );


		BYTE cbValue0=cbOperateCard[0]&MASK_VALUE;
		BYTE cbValue1=cbOperateCard[1]&MASK_VALUE;
		BYTE cbValue2=cbOperateCard[2]&MASK_VALUE;

		SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
	}

	return 0;
}

//获取操作信息
BYTE CGameClientDlg::GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] )
{
	//初始化
	BYTE cbSelectCount = 0;

	if( wOperateCode == WIK_NULL ) return 0;

	//听牌
	if( wOperateCode & WIK_LISTEN )
	{
		//提取可丢弃牌
		BYTE cbCardData[MAX_COUNT],cbCardCount=0;
		BYTE cbCardIndex[MAX_INDEX];
		CopyMemory( cbCardIndex,m_cbCardIndex,sizeof(cbCardIndex) );
		WORD wMeChairId = GetMeChairID();
		CChiHuRight chr;
		for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
		{
			if( cbCardIndex[i] == 0 ) continue;

			cbCardIndex[i]--;
			for( BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++ )
			{
				BYTE cbCurrentCard = m_GameLogic.SwitchToCardData(j);
				if( WIK_CHI_HU == m_GameLogic.AnalyseChiHuCard( cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],
					cbCurrentCard,chr) )
				{
					cbCardData[cbCardCount++] = m_GameLogic.SwitchToCardData(i);
					break;
				}
			}
			cbCardIndex[i]++;
		}
		for( BYTE i = 0; i < cbCardCount; i++ )
		{
			SelectInfo[cbSelectCount].cbActionCard = cbCardData[i];
			SelectInfo[cbSelectCount].wActionMask = WIK_LISTEN;
			SelectInfo[cbSelectCount].cbCardCount = 1;
			SelectInfo[cbSelectCount++].cbCardData[0] = cbCardData[i];
		}
	}
	//吃牌
	else if( wOperateCode&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
	{
		//效验
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		if( m_cbActionMask & WIK_LEFT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_LEFT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard+1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+2;
		}
		if( m_cbActionMask & WIK_CENTER )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_CENTER;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+1;
		}
		if( m_cbActionMask & WIK_RIGHT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_RIGHT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-2;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard-1;
		}
	}
	//碰牌
	else if( wOperateCode & WIK_PENG )
	{
		//效验
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
		SelectInfo[cbSelectCount].wActionMask = WIK_PENG;
		SelectInfo[cbSelectCount].cbCardCount = 2;
		SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
		SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard;
	}
	//杠牌
	else if( wOperateCode & WIK_GANG )
	{
		//如果是自己杠牌
		if( m_wCurrentUser == GetMeChairID() )
		{
			//寻找是否有多个杠牌
			WORD wMeChairId = GetMeChairID();
			tagGangCardResult gcr;
			ZeroMemory( &gcr,sizeof(gcr) );
			m_GameLogic.AnalyseGangCard( m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],gcr );
			ASSERT( gcr.cbCardCount > 0 );
			for( BYTE i = 0; i < gcr.cbCardCount; i++ )
			{
				SelectInfo[cbSelectCount].cbActionCard = gcr.cbCardData[i];
				SelectInfo[cbSelectCount].wActionMask = WIK_GANG;				
				if( m_cbCardIndex[m_GameLogic.SwitchToCardIndex(gcr.cbCardData[i])] == 1 )
				{
					SelectInfo[cbSelectCount].cbCardCount = 1;
					SelectInfo[cbSelectCount].cbCardData[0] = gcr.cbCardData[i];
				}
				else
				{
					SelectInfo[cbSelectCount].cbCardCount = m_GameLogic.GetWeaveCard(WIK_GANG,gcr.cbCardData[i],
						SelectInfo[cbSelectCount].cbCardData);
				}
				cbSelectCount++;
			}
		}
		else
		{
			ASSERT( m_cbActionCard != 0 );
			if( m_cbActionCard == 0 ) return 0;
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_GANG;
			SelectInfo[cbSelectCount].cbCardCount = 3;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
			SelectInfo[cbSelectCount].cbCardData[1] = m_cbActionCard;
			SelectInfo[cbSelectCount++].cbCardData[2] = m_cbActionCard;
		}
	}

	return cbSelectCount;
}

//扑克动画
bool CGameClientDlg::BeginMoveCard()
{
	if( m_MoveCardItemArray.GetCount() == 0 ) return false;
	if( m_GameClientView.IsMovingCard() ) return true;

	MoveKind enMoveKind = m_MoveCardItemArray[0]->GetMoveKind();

	//开局发牌
	if( enMoveKind == MK_StartCard )
		return BeginMoveStartCard( m_MoveCardItemArray[0] );
	//出牌
	else if( enMoveKind == MK_OutCard )
		return BeginMoveOutCard( m_MoveCardItemArray[0] );
	//发牌
	else if( enMoveKind == MK_SendCard )
		return BeginMoveSendCard( m_MoveCardItemArray[0] );
	//补花
	else if( enMoveKind == MK_ReplaceCard )
		return BeginMoveReplaceCard( m_MoveCardItemArray[0] );

	return false;
}

//停止动画
void CGameClientDlg::StopMoveCard()
{
	if( m_GameClientView.IsMovingCard() )
		m_GameClientView.StopMoveCard();

	while( BeginMoveCard() ) 
		m_GameClientView.StopMoveCard();
}

//出牌动画
bool CGameClientDlg::BeginMoveOutCard( const CMoveCardItem *pMoveCardItem )
{
	const COutCardItem *pOutCardItem = (const COutCardItem *)pMoveCardItem;

	//定义变量
	WORD wMeChairId = GetMeChairID();
	WORD wOutCardUser = pOutCardItem->m_wOutCardUser;
	WORD wViewOutCardUser = SwitchViewChairID(wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->m_cbOutCardData;
	
	if( IsLookonMode() || wOutCardUser != wMeChairId )
	{
		//环境设置
		KillGameClock(IDI_OPERATE_CARD); 
		
		//设置变量
		m_wCurrentUser=INVALID_CHAIR;

		//计算起始,结束点
		CPoint ptFrom,ptTo;
		if( wOutCardUser == wMeChairId )
			ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
		else
		{
			WORD wViewId = wOutCardUser>=3?2:wOutCardUser;
			ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
		}
		ptTo = m_GameClientView.m_DiscardCard[wViewOutCardUser].GetDispatchCardPos();

		//播放声音
		PlayCardSound(wOutCardUser,cbOutCardData);

		//设置界面
		m_GameClientView.SetCurrentUser( INVALID_CHAIR );
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wViewOutCardUser,cbOutCardData);
		
		//设置扑克
		if (wViewOutCardUser==MYSELF_VIEW_ID)
		{
			//删除扑克
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData) )
			{
				ASSERT( FALSE );
				return false;
			}

			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wViewOutCardUser>2)?2:wViewOutCardUser;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}

		tagMoveCardItem mci;
		mci.ptFrom = ptFrom;
		mci.ptTo = ptTo;
		mci.cbCardCount = 1;
		mci.cbCardData = cbOutCardData;
		mci.wViewChairId = wViewOutCardUser;
		m_GameClientView.OnMoveCardItem( mci );
	}

	return true;
}

//发牌动画
bool CGameClientDlg::BeginMoveSendCard( const CMoveCardItem *pMoveCardItem )
{
	const CSendCardItem *pSendCardItem = (const CSendCardItem *)pMoveCardItem;

	WORD wSendUserViewId = SwitchViewChairID(pSendCardItem->m_wSendCardUser);

	//计算起始,结束点
	CPoint ptFrom,ptTo;
	if( !pSendCardItem->m_bTail )
	{
		ASSERT( m_wHeapHead != INVALID_CHAIR );
		ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].GetDispatchCardPos(false);
		//删除牌头
		if( !m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].DeductionHeapCard(false) )
		{
			m_wHeapHead = (m_wHeapHead+1)%GAME_PLAYER;
			ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].GetDispatchCardPos(false);
			VERIFY( m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].DeductionHeapCard(false) );
		}
	}
	else
	{
		ASSERT( m_wHeapTail != INVALID_CHAIR );
		ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].GetDispatchCardPos(true);
		//删除牌尾
		if( !m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].DeductionHeapCard(true) )
		{
			m_wHeapTail = (m_wHeapTail+GAME_PLAYER-1)%GAME_PLAYER;
			ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].GetDispatchCardPos(true);
			VERIFY( m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].DeductionHeapCard(true) );
		}
	}
	if( wSendUserViewId == MYSELF_VIEW_ID )
		ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wViewId = wSendUserViewId>=3?2:wSendUserViewId;
		ptTo = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
	}

	//播放发牌声音
	m_DirectSound[1].Play();

	//构造动画
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = 1;
	mci.cbCardData = 0;
	mci.wViewChairId = SwitchViewChairID(pSendCardItem->m_wSendCardUser);
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//补花动画
bool CGameClientDlg::BeginMoveReplaceCard( const CMoveCardItem *pMoveCardItem )
{
	const CReplaceCardItem *pReplaceCardItem = (const CReplaceCardItem *)pMoveCardItem;

	//定义变量
	WORD wMeChairId = GetMeChairID();
	WORD wReplaceUser = pReplaceCardItem->m_wReplaceUser;
	WORD wReplaceUserViewId = SwitchViewChairID(wReplaceUser);
	BYTE cbReplaceCard = pReplaceCardItem->m_cbReplaceCard;
	
	if( IsLookonMode() || wReplaceUser != wMeChairId )
	{
		//播放声音
		PlayActionSound(wReplaceUser,WIK_REPLACE);

		//计算起始,结束点
		CPoint ptFrom,ptTo;
		if( wReplaceUser == wMeChairId )
			ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
		else
		{
			WORD wViewId = wReplaceUserViewId>=3?2:wReplaceUserViewId;
			ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
		}
		ptTo = m_GameClientView.m_DiscardCard[wReplaceUserViewId].GetDispatchCardPos();

		//设置动作
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction( wReplaceUserViewId,WIK_REPLACE );

		//设置扑克
		if (wReplaceUserViewId==MYSELF_VIEW_ID)
		{
			//删除扑克
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard) )
			{
				ASSERT( FALSE );
				return false;
			}

			//设置扑克
			SetHandCardControl(m_cbCardIndex,0x00);
		}
		else
		{
			//设置扑克
			WORD wUserIndex=(wReplaceUserViewId>2)?2:wReplaceUserViewId;
			if( m_GameClientView.m_UserCard[wUserIndex].IsHaveCurrentCard() )
				m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
			else
			{
				WORD wCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount();
				m_GameClientView.m_UserCard[wUserIndex].SetCardData(wCardCount-1,false);
			}
		}

		tagMoveCardItem mci;
		mci.ptFrom = ptFrom;
		mci.ptTo = ptTo;
		mci.cbCardCount = 1;
		mci.cbCardData = cbReplaceCard;
		mci.wViewChairId = wReplaceUserViewId;
		m_GameClientView.OnMoveCardItem( mci );
	}

	return true;
}

//开局发牌动画
bool CGameClientDlg::BeginMoveStartCard( const CMoveCardItem *pMoveCardItem )
{
	const CStartCardItem *pStartCardItem = (const CStartCardItem *)pMoveCardItem;

	//起始,结束点
	CPoint ptFrom,ptTo;

	//发牌位置
	WORD wHeapId = pStartCardItem->m_wHeapId;
	WORD wHeapViewId = SwitchViewChairID(wHeapId);
	//堆牌位置
	WORD wHeapCardIndex = pStartCardItem->m_wHeapCardIndex;
	if( pStartCardItem->m_cbCardCount > wHeapCardIndex+1 )
	{
		ASSERT( pStartCardItem->m_cbCardCount == 4 );
		//删除前面的堆牌
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,wHeapCardIndex+1 );

		wHeapId = (wHeapId+1)%GAME_PLAYER;
		wHeapViewId = SwitchViewChairID(wHeapId);
		wHeapCardIndex = HEAP_FULL_COUNT-(4-(wHeapCardIndex+1))-1;

		//获取起始点
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(WORD(wHeapCardIndex-1));

		//删除堆牌
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( HEAP_FULL_COUNT-1,
			pStartCardItem->m_cbCardCount-pStartCardItem->m_wHeapCardIndex-1 );
	}
	else
	{
		WORD wHeapPos = wHeapCardIndex;
		if( pStartCardItem->m_cbCardCount == 4 )
			wHeapPos -= 3;
		else
		{
			ASSERT( pStartCardItem->m_cbCardCount == 1 );
			if( wHeapPos%2 != 0 ) wHeapPos--;
			else wHeapPos++;
		}
		//获取起始点
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(wHeapPos);

		//删除堆牌
		if( pStartCardItem->m_cbCardCount == 4 )
			m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,4 );
		else
		{
			ASSERT( pStartCardItem->m_cbCardCount == 1 );
			if( wHeapCardIndex%2 != 0 )
				m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex-1,pStartCardItem->m_cbCardCount );
			else m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex+1,pStartCardItem->m_cbCardCount );
		}
	}

	//获取结束点
	WORD wViewChairId = SwitchViewChairID(pStartCardItem->m_wChairId);
	if( wViewChairId == MYSELF_VIEW_ID )
		ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		ptTo = m_GameClientView.m_UserCard[wUserIndex].GetDispatchCardPos();
	}
	
	//播放发牌声音
	m_DirectSound[1].Play();

	//构造动画
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = pStartCardItem->m_cbCardCount;
	mci.cbCardData = 0;
	mci.wViewChairId = wViewChairId;
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//动画完成消息
LRESULT CGameClientDlg::OnMoveCardFinish( WPARAM wParam, LPARAM lParam )
{
	//校验
	ASSERT( m_MoveCardItemArray.GetCount() > 0 );
	if( m_MoveCardItemArray.GetCount() == 0 ) return 0;

	MoveKind enMoveKind = m_MoveCardItemArray[0]->GetMoveKind();

	//派送消息
	if( enMoveKind == MK_StartCard )
	{
		CStartCardItem StartCardItem = *((CStartCardItem *)m_MoveCardItemArray[0]);

		//删除动画项目
		SafeDelete(m_MoveCardItemArray[0]);
		m_MoveCardItemArray.RemoveAt(0);

		VERIFY( OnMoveStartCardFinish(&StartCardItem) );
	}
	else if( enMoveKind == MK_OutCard )
	{
		COutCardItem OutCardItem = *((COutCardItem *)m_MoveCardItemArray[0]);

		//删除动画项目
		SafeDelete(m_MoveCardItemArray[0]);
		m_MoveCardItemArray.RemoveAt(0);

		VERIFY( OnMoveOutCardFinish(&OutCardItem) );
	}
	else if( enMoveKind == MK_SendCard )
	{
		CSendCardItem SendCardItem = *((CSendCardItem *)m_MoveCardItemArray[0]);

		//删除动画项目
		SafeDelete(m_MoveCardItemArray[0]);
		m_MoveCardItemArray.RemoveAt(0);

		VERIFY( OnMoveSendCardFinish(&SendCardItem) );
	}
	else if( enMoveKind == MK_ReplaceCard )
	{
		CReplaceCardItem ReplaceCardItem = *((CReplaceCardItem *)m_MoveCardItemArray[0]);

		//删除动画项目
		SafeDelete(m_MoveCardItemArray[0]);
		m_MoveCardItemArray.RemoveAt(0);

		VERIFY( OnMoveReplaceCardFinish(&ReplaceCardItem) );
	}
	else 
	{
		ASSERT(FALSE);
		return 0;
	}

	//播放下一个动画
	BeginMoveCard();

	return 0;
}

//出牌动画完成
bool CGameClientDlg::OnMoveOutCardFinish( const COutCardItem *pOutCardItem )
{
	WORD wOutUserViewId = SwitchViewChairID(pOutCardItem->m_wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->m_cbOutCardData;

	//添加到已丢弃牌
	m_GameClientView.m_DiscardCard[wOutUserViewId].AddCardItem( cbOutCardData );

	//设置出牌指示
	m_GameClientView.SetDiscUser( wOutUserViewId );

	//播放声音
	m_DirectSound[0].Play();

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//发牌动画完成
bool CGameClientDlg::OnMoveSendCardFinish( const CSendCardItem *pSendCardItem )
{
	//设置变量
	WORD wMeChairID = GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pSendCardItem->m_wSendCardUser);
	WORD wChairId = pSendCardItem->m_wSendCardUser;
	BYTE cbSendCardData = pSendCardItem->m_cbCardData;
	m_wCurrentUser = pSendCardItem->m_wCurrentUser;

	//取牌界面
	if (wViewChairID!=MYSELF_VIEW_ID)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
	}
	else
	{
		//插入扑克
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbSendCardData)]++;

		//设置扑克
		SetHandCardControl(m_cbCardIndex,cbSendCardData);
	}

	//当前用户
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//激活框架
		ActiveGameFrame();

		m_cbActionMask = pSendCardItem->m_cbActionMask;
		if( m_cbListenStatus == 0 && m_cbCardIndex[m_GameLogic.SwitchToCardIndex(CAI_SHEN_CARD)]==0 )
		{
			//m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID]);
		}
		m_cbActionCard = pSendCardItem->m_cbCardData;

		//动作处理
		if (m_cbActionMask!=WIK_NULL)
		{
			//设置界面
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
		}
	}

	//补牌操作
	if ((IsLookonMode()==false)&&(pSendCardItem->m_wReplaceUser==wMeChairID))
	{
		//设置时间
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
	}

	//出牌提示
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//计算时间
	WORD wTimeCount=TIME_OPERATE_CARD;

	//设置时间
	if( m_wCurrentUser != INVALID_CHAIR )
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}
	else m_GameClientView.SetCurrentUser( INVALID_CHAIR );

	return true;
}

//补花动画完成
bool CGameClientDlg::OnMoveReplaceCardFinish( const CReplaceCardItem *pReplaceCardItem )
{
	WORD wReplaceUserViewId = SwitchViewChairID(pReplaceCardItem->m_wReplaceUser);
	BYTE cbReplaceCardData = pReplaceCardItem->m_cbReplaceCard;

	//添加到已丢弃牌
	m_GameClientView.m_DiscardCard[wReplaceUserViewId].AddCardItem( cbReplaceCardData );

	//设置出牌指示
	m_GameClientView.SetDiscUser( wReplaceUserViewId );

	//播放声音
	m_DirectSound[0].Play();

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//开局动画完成
bool CGameClientDlg::OnMoveStartCardFinish( const CStartCardItem *pStartCardItem )
{
	//设置变量
	WORD wMeChairId = GetMeChairID();
	WORD wChairId = pStartCardItem->m_wChairId;
	WORD wViewChairId = SwitchViewChairID(wChairId);

	//添加扑克
	if( wViewChairId == MYSELF_VIEW_ID )
		m_GameClientView.m_HandCardControl.AddCardData( pStartCardItem->m_cbCardData,pStartCardItem->m_cbCardCount );
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		m_GameClientView.m_UserCard[wUserIndex].AddCardData( pStartCardItem->m_cbCardCount );
	}

	//播放声音
	m_DirectSound[1].Play();

	//判断是否最后一张牌
	if( pStartCardItem->m_bLastItem )
	{
		//堆立扑克
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetHeapCardInfo(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1]);
		}

		//环境处理
		PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

		//摇色子动画
		m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,0,m_wCaiShenSice);
		m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);
		//m_GameClientView.m_DrawSiceWnd.StopSicing();

	}

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//////////////////////////////////////////////////////////////////////////
