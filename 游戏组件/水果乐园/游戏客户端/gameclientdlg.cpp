#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "ExchangeDlg.h"

CGameClientDlg*	g_pGameClientDlg=NULL;
//////////////////////////////////////////////////////////////////////////
//计时器标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间
#define IDI_SHOW_LOGO				302									//显示游戏LOGO
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGameClientDlg,CGameFrameEngine)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(IDM_APPLY_BANKER,OnApplyBanker)
	ON_MESSAGE(IDM_BIG_SMALL,OnBigSmall)
	ON_MESSAGE(IDM_XU_YA,OnXuYa)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg()
{
//AllocConsole();
//freopen("CONOUT$","w+t",stdout);
//freopen("CONIN$","r+t",stdin);
	//限制信息
	m_lMeMaxScore=0L;			
	m_lApplyBankerCondition=0L;	
	m_lMeMaxScoreAfterEx = 0;
	//用户兑换后的筹码数
	m_lUserChipCount = 0;
	//个人下注
	ZeroMemory(m_lMeAreaChipArray, sizeof(m_lMeAreaChipArray));
	ZeroMemory(m_lXuYaArray,sizeof(m_lXuYaArray));
	ZeroMemory(m_lCurrentChip,sizeof(m_lCurrentChip));
	//庄家信息
	m_lBankerChip=0L;
	m_wCurrentBanker=0L;
	//状态变量
	m_bMeApplyBanker=false;
	g_pGameClientDlg = this;
	m_bFirstEnter = true;
	m_lMyCurrentChip=0;
	m_dwChipRate=100;
	m_bAlreadyExchange=false;
	m_bXuYaContinue=false;
	m_cbXuYaContinue=0;
	return;
}

//初始函数
bool CGameClientDlg::OnInitGameEngine()
{
	//设置标题
	SetWindowText(TEXT("水果乐园游戏"));
	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	m_pGlobalUnits=CGlobalUnits::GetInstance();

	theApp.CloseControl();//隐藏侧边栏

	return true;
}

//重置框架
bool CGameClientDlg::OnResetGameEngine()
{
	//限制信息
	m_lMeMaxScore=0L;	
	m_lUserChipCount=0;
	m_lMeMaxScoreAfterEx=0;
	//个人下注
	ZeroMemory(m_lMeAreaChipArray, sizeof(m_lMeAreaChipArray));
	ZeroMemory(m_lXuYaArray,sizeof(m_lXuYaArray));
	//庄家信息
	m_lBankerChip=0L;
	m_wCurrentBanker=0L;
	//状态变量
	m_bMeApplyBanker=false;
	m_bFirstEnter = true;
	return true;
}

//时间消息
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	if (wClockID==IDI_PLACE_JETTON)
	{
		//庄家按钮
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
	}
	if (wClockID==IDI_PLACE_JETTON&&nElapse<10) 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
		if (nElapse<=0)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("STOP_JETTON"));
		}
	}
	return true;
}

//网络消息
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if(m_bFirstEnter)
	{
		IClientUserItem *pUserItem=GetTableUserItem(GetMeChairID());
		if(pUserItem)
		{
			m_lMeMaxScoreAfterEx = pUserItem->GetUserScore() - ChipToScore(m_lUserChipCount);
		}
		m_bFirstEnter = false;
	}

	if(m_lUserChipCount == 0)
	{
		//CExchangeDlg dlg;
		//dlg.DoModal();
	}

	switch (wSubCmdID)
	{
	case SUB_S_CHIP_UPDATE:		//更新筹码
		{
			return OnSubChipUpdate(pData,wDataSize);
		}
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			return OnSubGameFree(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pData,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnSubUserApplyBanker(pData, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//取消做庄
		{
			return OnSubUserCancelBanker(pData, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnSubChangeBanker(pData, wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//游戏记录
		{
			return OnSubGameRecord(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			return OnSubPlaceJettonFail(pData,wDataSize);
		}
	case SUB_S_BIG_SMALL:
		{
			return OnSubBigSmall(pData,wDataSize);
		}
	case SUB_S_ADMIN_COMMDN:		//设置
		{
			return OnSubAdminControl(pData,wDataSize);
		}	
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏场景
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) 
				return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			if( pStatusFree == NULL )
				return FALSE;
//printf("pStatusFree->dwChipRate=%d\n",pStatusFree->dwChipRate);

			m_lMyCurrentChip=0;

			CopyMemory(m_lCurrentChip,pStatusFree->lCurrentJetton,sizeof(m_lCurrentChip));

			////////////////////////////////////////////////////////////////////////////////
			//更新列表
			for (WORD wUserIndex=0; wUserIndex<MAX_CHAIR; wUserIndex++)
			{
				IClientUserItem * pUserItem=GetTableUserItem(wUserIndex);
				tagUserInfo * pUserData=NULL;
				if(pUserItem!=NULL)
					pUserData=pUserItem->GetUserInfo();

				if ( pUserData == NULL ) 
					continue;

				//////////
				tagPlayerRecord Record;
				ZeroMemory(&Record,sizeof(Record));
				Record.dwUserID=pUserData->dwUserID;
				lstrcpyn(Record.szUserName,pUserData->szNickName,sizeof(Record.szUserName));
			#ifndef FUNC_CHIP
				Record.lUserScore=ScoreToChip(pUserData->lScore);
			#else
				Record.lUserScore=ScoreToChip(m_lCurrentChip[pUserData->wChairID]);
			#endif
				m_GameClientView.m_PlayerList.UpdateUser(Record);
				m_GameClientView.m_ApplyList.UpdateUser(Record);
				//////////
			}
			////////////////////////////////////////////////////////////////////////////////

			m_dwChipRate=pStatusFree->dwChipRate;
			SetJettonRate(m_dwChipRate);

			//设置时间
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

			//显示游戏LOGO
			m_GameClientView.m_bShowSGZZLogo=true;
			SetTimer(IDI_SHOW_LOGO,10000,NULL);

            //玩家信息
			m_lMeMaxScore =  pStatusFree->lUserMaxScore;//当前玩家的金币数，由服务器传来处理，这里需要单独计算才好
 
			m_lUserChipCount = ScoreToChip(m_lMeMaxScore - m_lMeMaxScoreAfterEx);

			m_GameClientView.SetMeMaxChip(m_lUserChipCount);
			WORD wMeChairID=GetMeChairID();
		
			WORD wSwitchViewChairID=SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wSwitchViewChairID);
			
            //庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,ScoreToChip(pStatusFree->lBankerScore));
			m_GameClientView.SetBankerChip(pStatusFree->wBankerTime,ScoreToChip(pStatusFree->lBankerWinScore));
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //控制信息
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			__int64 aLimitChips[JETTON_AREA_COUNT]={0};
			for(int i=0;i<JETTON_AREA_COUNT;i++)
			{
				aLimitChips[i]= ScoreToChip(pStatusFree->lAreaLimitScore[i]);
			}
			m_GameClientView.SetAreaLimitChip(aLimitChips, JETTON_AREA_COUNT);

			//设置状态
			SetGameStatus(GAME_STATUS_FREE);
			
			//播放声音
			//StopSound();
			//PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			OnGameSound(1,0);

			//m_GameClientView.m_btAddStep1_H.ShowWindow(SW_SHOW);
			//m_GameClientView.m_btAddStep1_H.EnableWindow(TRUE);
			m_GameClientView.m_btAddStep1.ShowWindow(SW_HIDE);

			m_GameClientView.m_btAddStep10.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAddStep10.EnableWindow(TRUE);
			//m_GameClientView.m_btAddStep10_H.ShowWindow(SW_HIDE);

			m_GameClientView.m_btAddStep100.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAddStep100.EnableWindow(TRUE);
			//m_GameClientView.m_btAddStep100_H.ShowWindow(SW_HIDE);

			m_GameClientView.m_btBetLast.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetSmall.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetBig.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetLast2.ShowWindow(SW_HIDE);
			m_GameClientView.m_btBetLast.EnableWindow(TRUE);
			m_GameClientView.m_btBetSmall.EnableWindow(TRUE);
			m_GameClientView.m_btBetBig.EnableWindow(TRUE);
			m_GameClientView.m_btBetLast2.EnableWindow(TRUE);

			tagUserAttribute * UserAttribute=m_pIClientKernel->GetUserAttribute();
			ASSERT(UserAttribute!=NULL);
			if (UserAttribute->dwUserRight==1879048192)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}

			//更新控制
			UpdateButtonContron();
			m_GameClientView.InvalidGameView(0,0,0,0);
			m_GameClientView.m_ExchangeDlg.DoModal();

			return true;
		}
	case GAME_STATUS_PLAY:		//游戏状态
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) 
				return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
			if( pStatusPlay == NULL )
				return FALSE;
//printf("pStatusFree->dwChipRate=%d\n",pStatusPlay->dwChipRate);

			m_lMyCurrentChip=0;

			CopyMemory(m_lCurrentChip,pStatusPlay->lCurrentJetton,sizeof(m_lCurrentChip));
			m_dwChipRate=pStatusPlay->dwChipRate;
			SetJettonRate(m_dwChipRate);

			////////////////////////////////////////////////////////////////////////////////
			//更新列表
			for (WORD wUserIndex=0; wUserIndex<MAX_CHAIR; wUserIndex++)
			{
				IClientUserItem * pUserItem=GetTableUserItem(wUserIndex);
				tagUserInfo * pUserData=NULL;
				if(pUserItem!=NULL)
					pUserData=pUserItem->GetUserInfo();

				if ( pUserData == NULL ) 
					continue;

				//////////
				tagPlayerRecord Record;
				ZeroMemory(&Record,sizeof(Record));
				Record.dwUserID=pUserData->dwUserID;
				lstrcpyn(Record.szUserName,pUserData->szNickName,sizeof(Record.szUserName));
			#ifndef FUNC_CHIP
				Record.lUserScore=ScoreToChip(pUserData->lScore);
			#else
				Record.lUserScore=ScoreToChip(m_lCurrentChip[pUserData->wChairID]);
			#endif
				m_GameClientView.m_PlayerList.UpdateUser(Record);
				m_GameClientView.m_ApplyList.UpdateUser(Record);
				//////////
			}
			////////////////////////////////////////////////////////////////////////////////

			//StopSound();
			//PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			OnGameSound(1,0);

			//m_GameClientView.m_btAddStep1_H.ShowWindow(SW_SHOW);
			//m_GameClientView.m_btAddStep1_H.EnableWindow(TRUE);
			m_GameClientView.m_btAddStep1.ShowWindow(SW_HIDE);

			m_GameClientView.m_btAddStep10.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAddStep10.EnableWindow(TRUE);
			//m_GameClientView.m_btAddStep10_H.ShowWindow(SW_HIDE);

			m_GameClientView.m_btAddStep100.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAddStep100.EnableWindow(TRUE);
			//m_GameClientView.m_btAddStep100_H.ShowWindow(SW_HIDE);

			m_GameClientView.m_btBetLast.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetSmall.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetBig.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetLast2.ShowWindow(SW_HIDE);
			m_GameClientView.m_btBetLast.EnableWindow(TRUE);
			m_GameClientView.m_btBetSmall.EnableWindow(TRUE);
			m_GameClientView.m_btBetBig.EnableWindow(TRUE);
			m_GameClientView.m_btBetLast2.EnableWindow(TRUE);

			//全局下注
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				m_GameClientView.PlaceUserJetton(i, ScoreToChip(pStatusPlay->lTotalJettonScore[i]));

			//玩家下注
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				SetMePlaceJetton(i, ScoreToChip(pStatusPlay->lUserAreaScore[i]));

			//玩家积分
			m_lMeMaxScore =  pStatusPlay->lUserMaxScore;//当前玩家的金币数，由服务器传来处理，这里需要单独计算才好
			m_lUserChipCount = ScoreToChip(m_lMeMaxScore - m_lMeMaxScoreAfterEx);
 		
			m_GameClientView.SetMeMaxChip(m_lUserChipCount);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//控制信息
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			__int64 aLimitChips[JETTON_AREA_COUNT]={0};
			for(int i=0;i<JETTON_AREA_COUNT;i++)
			{
				aLimitChips[i]= ScoreToChip(pStatusPlay->lAreaScoreLimit[i]);
			}
			m_GameClientView.SetAreaLimitChip(aLimitChips, JETTON_AREA_COUNT);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//设置成绩
				m_GameClientView.SetCurGameChip(ScoreToChip(pStatusPlay->lEndUserScore),ScoreToChip(pStatusPlay->lEndUserReturnScore),ScoreToChip(pStatusPlay->lEndBankerScore),pStatusPlay->lEndRevenue);
			}
			else
			{
				//播放声音
				//StopSound();
				//PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			}

			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,ScoreToChip(pStatusPlay->lBankerScore));
			m_GameClientView.SetBankerChip(pStatusPlay->cbBankerTime,ScoreToChip(pStatusPlay->lBankerWinScore));
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);

			//设置时间
			SetGameClock(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			//显示游戏LOGO
			m_GameClientView.m_bShowSGZZLogo=true;
			SetTimer(IDI_SHOW_LOGO,10000,NULL);

			tagUserAttribute * UserAttribute=m_pIClientKernel->GetUserAttribute();
			ASSERT(UserAttribute!=NULL);
			if (UserAttribute->dwUserRight==1879048192)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}

			//更新按钮
			UpdateButtonContron();
			m_GameClientView.InvalidGameView(0,0,0,0);
			m_GameClientView.m_ExchangeDlg.DoModal();

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

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if( pGameStart == NULL )
		return FALSE;

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,ScoreToChip(pGameStart->lBankerScore));

	//玩家信息
	m_lMeMaxScore =  pGameStart->lUserMaxScore;
	m_lUserChipCount = ScoreToChip(m_lMeMaxScore - m_lMeMaxScoreAfterEx);
 
	m_GameClientView.SetMeMaxChip(m_lUserChipCount);

	//设置各下注区域初始可下分
	__int64 aLimitChips[JETTON_AREA_COUNT]={0};
	for(int i=0;i<JETTON_AREA_COUNT;i++)
	{
		aLimitChips[i]= ScoreToChip(pGameStart->lAreaLimitScore[i]);
	}
	m_GameClientView.SetAreaLimitChip(aLimitChips, JETTON_AREA_COUNT);

	//设置时间
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//////////
	m_GameClientView.CleanUserJetton();
	//////////

	//设置状态
	SetGameStatus(GS_PLACE_JETTON);

	//更新控制
	UpdateButtonContron();

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	if (m_GameClientView.m_AdminDlg.GetSafeHwnd())
	{
		m_GameClientView.m_AdminDlg.ResetUserBet();
	}

	//////////
	//连续续押
	if (m_bXuYaContinue)
	{
		if (m_cbXuYaContinue>0)
		{
			m_cbXuYaContinue--;
			for (int i=0; i<JETTON_AREA_COUNT; i++)
			{
				if (m_lXuYaArray[i]>m_lMeAreaChipArray[i]) OnPlaceJetton(i,m_lXuYaArray[i]-m_lMeAreaChipArray[i],true);
			}
		}
		else
		{
			m_bXuYaContinue=false;
			m_cbXuYaContinue=0;
			m_GameClientView.m_btBetLast.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetLast2.ShowWindow(SW_HIDE);
		}
	}
	//////////

	return true;
}

//游戏空闲
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) 
		return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	if( pGameFree == NULL )
		return false;

	m_GameClientView.SetBankerChip(pGameFree->iBankerTime,ScoreToChip(pGameFree->lBankerTotallScore));

	m_GameClientView.m_btBankButton.ShowWindow(SW_SHOW);

	//设置时间
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	tagGameRecord GameRecord[8];
	ZeroMemory(&GameRecord,sizeof(GameRecord));
//printf("pGameFree->cbGameRecordCount=%d\n",pGameFree->cbGameRecordCount);
	CopyMemory(GameRecord,pGameFree->cbGameRecord,sizeof(tagGameRecord)*pGameFree->cbGameRecordCount);
	m_GameClientView.m_GameRecord.FillGameRecord(GameRecord,pGameFree->cbGameRecordCount);
//for (int i=0; i<pGameFree->cbGameRecordCount; i++)
//{
//	printf("pGameFree->cbGameRecord[i].cbRecord=%d\n",pGameFree->cbGameRecord[i].cbRecord);
//	printf("pGameFree->cbGameRecord[i].dwGameTimes=%d\n",pGameFree->cbGameRecord[i].dwGameTimes);
//	printf("pGameFree->cbGameRecord[i].bGoodLuck=%d\n",pGameFree->cbGameRecord[i].bGoodLuck);
//}
	//StopSound();
	//PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
	OnGameSound(1,0);

	//设置状态
	SetGameStatus(GAME_STATUS_FREE);
	////////////////////////////////////////////////////////////////////////////////
	m_GameClientView.m_bBigSmallTime=false;
	m_GameClientView.m_bAlreadyGuess=false;
	m_GameClientView.m_cbGameEndTime=0;
	m_GameClientView.m_cbPaoHuoCheCount=0;
	m_GameClientView.m_cbPaoHuoCheIndex=0;
	m_GameClientView.m_cbGoodLuckType=0;
	memset(m_GameClientView.m_cbAreaLight,0,sizeof(m_GameClientView.m_cbAreaLight));
	memset(m_GameClientView.m_cbPaoHuoCheArea,0,sizeof(m_GameClientView.m_cbPaoHuoCheArea));
	////////////////////////////////////////////////////////////////////////////////

	//清理桌面
	m_GameClientView.SetWinnerSide(0xFF);
	for (int nAreaIndex=ID_BAR; nAreaIndex<=ID_APPLE; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);

	//更新列表
	for (WORD wUserIndex=0; wUserIndex<MAX_CHAIR; wUserIndex++)
	{
		IClientUserItem * pUserItem=GetTableUserItem(wUserIndex);
		tagUserInfo * pUserData=NULL;
		if(pUserItem!=NULL)
			pUserData=pUserItem->GetUserInfo();

		if ( pUserData == NULL ) 
			continue;

		//////////
		tagPlayerRecord Record;
		ZeroMemory(&Record,sizeof(Record));
		Record.dwUserID=pUserData->dwUserID;
		lstrcpyn(Record.szUserName,pUserData->szNickName,sizeof(Record.szUserName));
#ifndef FUNC_CHIP
		Record.lUserScore=ScoreToChip(pUserData->lScore);
#else
		Record.lUserScore=ScoreToChip(m_lCurrentChip[pUserData->wChairID]);
#endif
		m_GameClientView.m_PlayerList.UpdateUser(Record);
		m_GameClientView.m_ApplyList.UpdateUser(Record);
		//////////
	}

	//更新控件
	UpdateButtonContron();

	static bool AlreadyShow=false;
	if (m_lMyCurrentChip==0 && !AlreadyShow && m_bAlreadyExchange)
	{
		AlreadyShow=true;
		ShowInformation(TEXT("您的筹码已用完，请退出游戏增购筹码继续游戏！"));
		OnClose();//退出游戏
	}

	return true;
}

//用户加注
bool CGameClientDlg::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if( pPlaceJetton == NULL )
		return false;
	if( pPlaceJetton->cbJettonArea>ID_APPLE || pPlaceJetton->cbJettonArea<ID_BAR )
		return false;

	//加注界面
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,ScoreToChip(pPlaceJetton->lJettonScore));

	if (pPlaceJetton->cbAndroid==false && m_GameClientView.m_AdminDlg.GetSafeHwnd())
	{
		m_GameClientView.m_AdminDlg.SetUserBetScore(pPlaceJetton->cbJettonArea,ScoreToChip(pPlaceJetton->lJettonScore));
	}

	return true;
}

//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if( pGameEnd == NULL )
		return FALSE;

	//下注时间结束，则不再绘制按下的按钮
	m_GameClientView.m_bisLButtonDown=false;

	////////////////////////////////////////////////////////////////////////////////
	//续押数据
	bool bCopy=false;
	for (int ii=0; ii<JETTON_AREA_COUNT; ii++)
	{
		if (m_lMeAreaChipArray[ii]>0)
		{
			bCopy=true;
			break;
		}
	}
	if (bCopy)
	{
		CopyMemory(m_lXuYaArray,m_lMeAreaChipArray,sizeof(m_lXuYaArray));
	}
	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	//区分开中普通区域和GoodLuck区域
	if (pGameEnd->cbWinArea==9 || pGameEnd->cbWinArea==21)
	{
		if (pGameEnd->cbGoodLuckType==0)//吃分，无动画
		{
			m_GameClientView.SetGoodLuckCartoon(pGameEnd->cbPaoHuoCheArea,pGameEnd->cbPaoHuoCheCount,pGameEnd->cbWinArea,pGameEnd->cbGoodLuckType);
		}
		else//有动画的另外四种开奖
		{
			m_GameClientView.SetGoodLuckCartoon(pGameEnd->cbPaoHuoCheArea,pGameEnd->cbPaoHuoCheCount,pGameEnd->cbWinArea,pGameEnd->cbGoodLuckType);
		}
	}
	else//普通区域
	{
		m_GameClientView.UpdateCartoonTimeElapse(pGameEnd->cbWinArea);//构造卡通计时器间隔数组
	}
	////////////////////////////////////////////////////////////////////////////////

	//设置发牌计时器
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD,pGameEnd->cbTimeLeave);

	//庄家信息
	//m_GameClientView.SetBankerChip(pGameEnd->iBankerTime,SCORETOCHIP(pGameEnd->lBankerTotallScore));

	//成绩信息
	if (GetMeChairID()!=m_wCurrentBanker)//庄家显示成绩会有问题
		m_GameClientView.SetCurGameChip(ScoreToChip(pGameEnd->lUserScore),ScoreToChip(pGameEnd->lUserReturnScore),ScoreToChip(pGameEnd->lBankerScore),pGameEnd->lRevenue);

	//设置状态
	SetGameStatus(GS_GAME_END);

	//更新控件
	UpdateButtonContron();

	//停止声音

	return true;
}

//更新控制
void CGameClientDlg::UpdateButtonContron()
{
	//置能判断
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
		bEnablePlaceJetton=false;

	if (GetGameStatus()!=GS_PLACE_JETTON) 
		bEnablePlaceJetton=false;

	if (m_wCurrentBanker==GetMeChairID()) 
		bEnablePlaceJetton=false;

	if (IsLookonMode())
		bEnablePlaceJetton=false;

	//下注按钮
	if (bEnablePlaceJetton==true)
	{
		//计算积分
		__int64 lCurrentJetton=m_GameClientView.GetCurrentJetton();
		__int64 lLeaveChip= 0;
		__int64	iTemp = 0;
		for( int i=0; i<JETTON_AREA_COUNT; i++ )
			iTemp += m_lMeAreaChipArray[i];
		lLeaveChip = m_lMeMaxScore -  (iTemp);
	}
	//庄家按钮
	if (!IsLookonMode())
	{
		//获取信息
		IClientUserItem *pUserItem=GetTableUserItem(GetMeChairID());
		tagUserInfo *pMeUserData = NULL;
		if(pUserItem!=NULL)
			pMeUserData = pUserItem->GetUserInfo();

		//申请按钮
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) 
			bEnableApply=false;

		if (m_bMeApplyBanker) 
			bEnableApply=false;
#ifndef FUNC_CHIP
		if (pMeUserData && pMeUserData->lScore<m_lApplyBankerCondition)
			bEnableApply=false;
#else
		if (pMeUserData && ChipToScore(m_lMyCurrentChip)<m_lApplyBankerCondition)
			bEnableApply=false;
#endif
		if (GetGameStatus()!=GS_PLACE_JETTON)//修复，下注的一瞬间，“我要上庄”按钮会闪一下
			m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//取消按钮
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GAME_STATUS_FREE) 
			bEnableCancel=false;

		if (m_bMeApplyBanker==false) 
			bEnableCancel=false;

		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

		//显示判断
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);
		}
		else
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
	}

	//控制游戏记录按钮控件
	int nRecord = m_GameClientView.m_GameRecord.GetRecordCount();
	if( nRecord <= MAX_SHOW_HISTORY )
	{
		m_GameClientView.m_btChipMoveL.EnableWindow(FALSE);
		m_GameClientView.m_btChipMoveR.EnableWindow(FALSE);
	}
	else
	{
		int nReadPos = m_GameClientView.m_GameRecord.GetCurReadPos();
		int nWritePos = m_GameClientView.m_GameRecord.GetCurWritePos();
		m_GameClientView.m_btChipMoveL.EnableWindow(TRUE);
		if( nReadPos != nWritePos-1 )
			m_GameClientView.m_btChipMoveR.EnableWindow(TRUE);
	}
	return;
}

//加注消息
void CGameClientDlg::OnPlaceJetton(BYTE cbJettonArea, __int64 lJettonChip, bool bXuYa)
{


	CString str;
	str.Format(L"sg 加注区域%d",cbJettonArea);
	OutputDebugString(str);

	if( cbJettonArea > ID_APPLE || cbJettonArea < ID_BAR )
		return;

	//庄家不能下注
	if ( GetMeChairID() == m_wCurrentBanker )
		return;

	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
		return;

	//非下注状态，直接返回
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return;
	}

#ifdef FUNC_CHIP
	//当前筹码不足
	if (m_lMyCurrentChip<=0) return;
	__int64 lMyTotalJetton=0;
	for (int i=0; i<JETTON_AREA_COUNT; i++)
		lMyTotalJetton+=m_lMeAreaChipArray[i];
	if (m_lMyCurrentChip<(lMyTotalJetton+lJettonChip))
		return;
#endif

	//////////
	if ((m_lMeAreaChipArray[cbJettonArea]+lJettonChip)>999)
	{
		while ((m_lMeAreaChipArray[cbJettonArea]+lJettonChip)>999)
			lJettonChip=lJettonChip/10;
		if (lJettonChip<=0) return;
	}
	//////////

	//根据下注区域，设置本人下注筹码
	m_lMeAreaChipArray[cbJettonArea] += lJettonChip;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, m_lMeAreaChipArray[cbJettonArea]);

	//变量定义
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=ChipToScore(lJettonChip);

	//发送网络消息
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//更新按钮
	UpdateButtonContron();

	if (bXuYa) return;//续押不播放音效

	//////////
	switch (cbJettonArea)
	{
	case ID_APPLE:			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND1")); break;
	case ID_ORANGE:			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND2")); break;
	case ID_PAWPAW:			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND3")); break;
	case ID_BELL:			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND4")); break;
	case ID_WATERMELON:		PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND5")); break;
	case ID_STAR:			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND6")); break;
	case ID_DOUBLE_SEVEN:	PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND7")); break;
	case ID_BAR:			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND8")); break;
	}
	//////////

	return;
}

//申请消息
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
#ifndef FUNC_CHIP
	IClientUserItem *pUserItem=GetTableUserItem(GetMeChairID());
	tagUserInfo *pMeUserData=NULL;
	if (pUserItem!=NULL) pMeUserData=pUserItem->GetUserInfo();
	if (pMeUserData==NULL) return true;
	if (pMeUserData->lScore<m_lApplyBankerCondition) return true;
#else
	if (ChipToScore(m_lMyCurrentChip)<m_lApplyBankerCondition) return true;
#endif
	//旁观判断
	if (IsLookonMode()) return true;
	//转换变量
	bool bApplyBanker=wParam?true:false;
	//如果当前就是庄家，则不允许再次申请
	if (m_wCurrentBanker==GetMeChairID() && bApplyBanker) return true;

	if (bApplyBanker)
	{
		//发送申请上庄消息
		SendSocketData(SUB_C_APPLY_BANKER,NULL,0);
		m_bMeApplyBanker=true;
	}
	else
	{
		//发送取消上庄消息
		SendSocketData(SUB_C_CANCEL_BANKER,NULL,0);
		m_bMeApplyBanker=false;
	}
	//设置按钮
	UpdateButtonContron();
	return true;
}

//比大小
LRESULT CGameClientDlg::OnBigSmall(WPARAM wParam, LPARAM lParam)//0猜小，1猜大
{
	CMD_C_BigSmall BigSmall;
	memset(&BigSmall,0,sizeof(CMD_C_BigSmall));
	BigSmall.cbBigSmall=(BYTE)wParam;
	SendSocketData(SUB_C_BIG_SMALL,&BigSmall,sizeof(CMD_C_BigSmall));
	return 0;
}

//续押
LRESULT CGameClientDlg::OnXuYa(WPARAM wParam, LPARAM lParam)
{
	for (int i=0; i<JETTON_AREA_COUNT; i++)
	{
		if (m_lXuYaArray[i]>m_lMeAreaChipArray[i]) OnPlaceJetton(i,m_lXuYaArray[i]-m_lMeAreaChipArray[i],true);
	}
	return 0;
}

//时间消息
VOID CGameClientDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_SHOW_LOGO)
	{
		KillTimer(IDI_SHOW_LOGO);
		m_GameClientView.m_bShowSGZZLogo=false;
	}
	__super::OnTimer(nIDEvent);
}

//申请做庄
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;
	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;
	if (pApplyBanker==NULL) return false;
	//获取玩家
	IClientUserItem *pUserItem=GetTableUserItem(pApplyBanker->wApplyUser);
	tagUserInfo *pUserData=NULL;
	if (pUserItem!=NULL) pUserData=pUserItem->GetUserInfo();
	if (pUserData==NULL) return false;
	//插入玩家
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser)
	{
		//////////
		tagPlayerRecord Record;
		ZeroMemory(&Record,sizeof(Record));
		Record.dwUserID=pUserData->dwUserID;
		lstrcpyn(Record.szUserName,pUserData->szNickName,sizeof(Record.szUserName));
#ifndef FUNC_CHIP
		Record.lUserScore=ScoreToChip(pUserData->lScore);
#else
		Record.lUserScore=ScoreToChip(m_lCurrentChip[pUserData->wChairID]);
#endif
		m_GameClientView.m_ApplyList.InserUser(Record);
		//////////
	}
	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;
	//更新控件
	UpdateButtonContron();
	return true;
}

//取消做庄
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;
	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;
	if (pCancelBanker==NULL) return false;
	//删除玩家
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		IClientUserItem *pUserItem=GetTableUserItem(wUserIndex);
		tagUserInfo *pUserData = NULL;
		if(pUserItem!=NULL)
			pUserData = pUserItem->GetUserInfo();

		if ( pUserData == NULL ) 
			continue;
		if(!lstrcmpi(pUserData->szNickName, pCancelBanker->szCancelUser))
		{
			m_GameClientView.m_ApplyList.DeleteUser(pUserData->dwUserID);
			break;
		}
	}

	//自己判断
	IClientUserItem *pUserItem=GetTableUserItem(GetMeChairID());
	tagUserInfo *pMeUserData = NULL;
	if(pUserItem!=NULL)
		pMeUserData = pUserItem->GetUserInfo();
	if( pMeUserData == NULL )
		return FALSE;

	//本人取消做庄
	if (IsLookonMode()==false && !lstrcmp(pMeUserData->szNickName,pCancelBanker->szCancelUser)) 
		m_bMeApplyBanker=false;

	//更新控件
	UpdateButtonContron();

	return true;
}

//切换庄家
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) 
		return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;
	if( pChangeBanker == NULL )
		return FALSE;

	//显示图片
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//自己判断
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//庄家信
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerChip(0,0);

	//删除玩家
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IClientUserItem *pUserItem=GetTableUserItem(m_wCurrentBanker);
		tagUserInfo *pBankerUserData = NULL;
		if(pUserItem!=NULL)
			pBankerUserData = pUserItem->GetUserInfo();

		if (pBankerUserData != NULL)
		{
			m_GameClientView.m_ApplyList.DeleteUser(pBankerUserData->dwUserID);
		}
	}

	//更新界面
	UpdateButtonContron();
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//游戏记录
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) 
		return false;

	if( pBuffer == NULL )
		return FALSE;

	//结果变量
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//设置记录
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);
		if( pServerGameRecord == NULL )
			continue;

		GameRecord.dwGameTimes = pServerGameRecord->dwGameTimes;
		GameRecord.cbGameRecord = pServerGameRecord->cbRecord;
		GameRecord.bGoodLuck=pServerGameRecord->bGoodLuck;
		m_GameClientView.SetGameHistory(&GameRecord,1);
	}

	return true;
}

//下注失败
bool CGameClientDlg::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;
	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	if( pPlaceJettonFail == NULL ) return FALSE;

	//效验参数
	BYTE cbViewIndex=pPlaceJettonFail->cbJettonArea;	//取得下注区域
	ASSERT(cbViewIndex<=ID_APPLE);
	if (cbViewIndex>ID_APPLE) return false;

	//下注数量
	__int64 iJettonCount=ScoreToChip(pPlaceJettonFail->lJettonScore);
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
		{
			m_lMeAreaChipArray[i] -= iJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex, m_lMeAreaChipArray[i]);
		}
	}
	return true;
}

//猜大小结果
bool CGameClientDlg::OnSubBigSmall(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_BigSmall));
	if (wDataSize!=sizeof(CMD_S_BigSmall)) return false;
	//消息处理
	CMD_S_BigSmall * pBigSmall=(CMD_S_BigSmall *)pBuffer;
	if (pBigSmall==NULL) return false;

	if (pBigSmall->bWin)
	{
//printf("成功\n");
		m_GameClientView.m_bBigSmallWin=true;
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_DU_WIN"));
	}
	else
	{
//printf("失败\n");
		m_GameClientView.m_bBigSmallWin=false;
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_DU_LOSE"));
	}
	//庄家信息
	//m_GameClientView.SetBankerChip(pBigSmall->iBankerTime,SCORETOCHIP(pBigSmall->lBankerTotallScore));

	m_GameClientView.InsertBigSmallRecord(pBigSmall->cbBigSmall);

	//成绩信息
	if (GetMeChairID()!=m_wCurrentBanker)//庄家显示成绩会有问题
		m_GameClientView.SetCurGameChip(ScoreToChip(pBigSmall->lUserScore),0,ScoreToChip(pBigSmall->lBankerScore),0);

	m_GameClientView.m_cbGameEndTime=pBigSmall->cbBigSmall;

	return true;
}

//兑换筹码
bool CGameClientDlg::OnSubChipUpdate(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChipUpdate));
	if (wDataSize!=sizeof(CMD_S_ChipUpdate)) return false;
	//消息处理
	CMD_S_ChipUpdate * pChipUpdate=(CMD_S_ChipUpdate *)pBuffer;
	if (pChipUpdate==NULL) return false;

	if (GetMeChairID()==pChipUpdate->wChairID)
	{
		m_lMyCurrentChip=ScoreToChip(pChipUpdate->lTotalScore);
	}
	m_lCurrentChip[pChipUpdate->wChairID]=pChipUpdate->lTotalScore;
	m_GameClientView.InvalidGameView(0,0,0,0);
	//////////
	IClientUserItem * pIClientUserItem=GetTableUserItem(pChipUpdate->wChairID);
	tagPlayerRecord Record;
	ZeroMemory(&Record,sizeof(Record));
	Record.dwUserID=pIClientUserItem->GetUserID();
	lstrcpyn(Record.szUserName,pIClientUserItem->GetNickName(),sizeof(Record.szUserName));
#ifndef FUNC_CHIP
	Record.lUserScore=ScoreToChip(pIClientUserItem->GetUserScore());
#else
	Record.lUserScore=ScoreToChip(m_lCurrentChip[pIClientUserItem->GetChairID()]);
#endif
	m_GameClientView.m_PlayerList.UpdateUser(Record);
	m_GameClientView.m_ApplyList.UpdateUser(Record);
	//////////
	return true;
}

//设置庄家
void CGameClientDlg::SetBankerInfo(WORD wBanker,__int64 iChip)
{
	m_wCurrentBanker=wBanker;
	m_lBankerChip=iChip;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerChip);
}

//个人下注
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<= ID_APPLE);
	if (cbViewIndex>ID_APPLE) 
		return;

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
			m_lMeAreaChipArray[i] = iJettonCount;
	}

	//设置界面
	m_GameClientView.SetMePlaceJetton(cbViewIndex,iJettonCount);
}
//////////////////////////////////////////////////////////////////////////

//设置筹码比率
void CGameClientDlg::SetJettonRate(DWORD dwJettonRate)
{
	ASSERT(dwJettonRate!=0);
	m_dwChipRate=dwJettonRate;
	return;
}

//积分转换成筹码
__int64 CGameClientDlg::ScoreToChip(__int64 lScore)
{
	ASSERT(m_dwChipRate!=0);
	return (lScore/m_dwChipRate);
}

//筹码转换成积分
__int64 CGameClientDlg::ChipToScore(__int64 lChip)
{
	ASSERT(m_dwChipRate!=0);
	return (lChip*m_dwChipRate);
}

//兑换筹码
void CGameClientDlg::ExchangeChip(__int64 lChip)
{
	ASSERT(lChip>=0);
	if (lChip<=0) return;
	m_bAlreadyExchange=true;
	//////////
	CMD_C_Exchange Exchange;
	ZeroMemory(&Exchange,sizeof(Exchange));
	//Exchange.lTotalScore=ChipToScore(m_lMyCurrentChip+lChip);
	Exchange.lExchangeScore=ChipToScore(lChip);
	SendSocketData(SUB_C_EXCHANGE,&Exchange,sizeof(Exchange));
	//////////
	//m_lMyCurrentChip+=lChip;
	//m_GameClientView.InvalidGameView(0,0,0,0);
}

//用户进入
VOID CGameClientDlg::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	if (pIClientUserItem==NULL) return;
	//////////
	tagPlayerRecord Record;
	ZeroMemory(&Record,sizeof(Record));
	Record.dwUserID=pIClientUserItem->GetUserID();
	lstrcpyn(Record.szUserName,pIClientUserItem->GetNickName(),sizeof(Record.szUserName));
#ifndef FUNC_CHIP
	Record.lUserScore=ScoreToChip(pIClientUserItem->GetUserScore());
#else
	Record.lUserScore=ScoreToChip(m_lCurrentChip[pIClientUserItem->GetChairID()]);
#endif
	m_GameClientView.m_PlayerList.InserUser(Record);
	//////////
	return;
}

//用户离开
VOID CGameClientDlg::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	if (pIClientUserItem==NULL) return;
	m_GameClientView.m_PlayerList.DeleteUser(pIClientUserItem->GetUserID());
	return;
}

//背景音乐
bool CGameClientDlg::AllowBackGroundSound(bool bAllowSound)
{
	if (bAllowSound)
	{
		OnGameSound(1,0);
	}
	else
	{
		OnGameSound(0,0);
	}
	return true;
}

LRESULT CGameClientDlg::OnGameSound(WPARAM wParam, LPARAM lParam)
{
	if (!m_pGlobalUnits->IsAllowBackGroundSound()) wParam=0;
	//static bool bOpen=false;
	//if (!bOpen)
	//{
	//	bOpen=true;
	//	mciSendString(TEXT("open \"Sound\\mp3_BackGroud.mp3\" alias mysong1"),NULL,0,NULL);
	//	mciSendString(TEXT("open \"Sound\\mp3_Rand0.mp3\" alias mysong2"),NULL,0,NULL);
	//	mciSendString(TEXT("open \"Sound\\mp3_Rand1.mp3\" alias mysong3"),NULL,0,NULL);
	//	mciSendString(TEXT("open \"Sound\\mp3_Rand2.mp3\" alias mysong4"),NULL,0,NULL);
	//}
	if (wParam==0)
	{
		mciSendString(TEXT("close mysong1"),NULL,0,NULL);
		mciSendString(TEXT("close mysong2"),NULL,0,NULL);
		mciSendString(TEXT("close mysong3"),NULL,0,NULL);
		mciSendString(TEXT("close mysong4"),NULL,0,NULL);
	}
	if (wParam==1)
	{
		mciSendString(TEXT("close mysong1"),NULL,0,NULL);
		mciSendString(TEXT("close mysong2"),NULL,0,NULL);
		mciSendString(TEXT("close mysong3"),NULL,0,NULL);
		mciSendString(TEXT("close mysong4"),NULL,0,NULL);
		mciSendString(TEXT("open \"Sound\\mp3_BackGroud.mp3\" alias mysong1"),NULL,0,NULL);
		mciSendString(TEXT("play mysong1 repeat"),NULL,0,NULL);
	}
	if (wParam==2)
	{
		mciSendString(TEXT("close mysong1"),NULL,0,NULL);
		int nRand=rand()%3;
		switch (nRand)
		{
		case 0:
			{
				mciSendString(TEXT("open \"Sound\\mp3_Rand0.mp3\" alias mysong2"),NULL,0,NULL);
				mciSendString(TEXT("play mysong2"),NULL,0,NULL);
				break;
			}
		case 1:
			{
				mciSendString(TEXT("open \"Sound\\mp3_Rand1.mp3\" alias mysong3"),NULL,0,NULL);
				mciSendString(TEXT("play mysong3"),NULL,0,NULL);
				break;
			}
		case 2:
			{
				mciSendString(TEXT("open \"Sound\\mp3_Rand2.mp3\" alias mysong4"),NULL,0,NULL);
				mciSendString(TEXT("play mysong4"),NULL,0,NULL);
				break;
			}
		}
	}

	return 0;
}

//特殊账号控制
bool CGameClientDlg::OnSubAdminControl(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_ControlReturns));
	if (wDataSize!=sizeof(CMD_S_ControlReturns)) return false;
	if (m_GameClientView.m_AdminDlg.GetSafeHwnd())
	{
		CMD_S_ControlReturns * pResult=(CMD_S_ControlReturns *)pBuffer;
		m_GameClientView.m_AdminDlg.UpdateControl(pResult);
	}
	return true;
}

//特殊账号控制
LRESULT CGameClientDlg::OnAdminControl(WPARAM wParam, LPARAM lParam)
{
	SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));
	return 0;
}

void CGameClientDlg::OnClose()
{
	PostQuitMessage(0);
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	return;
}

//旁观状态
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//连续续押20局
void CGameClientDlg::OnXuYaContinue()
{
	m_GameClientView.m_btBetLast2.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBetLast.ShowWindow(SW_HIDE);
	m_bXuYaContinue=true;
	m_cbXuYaContinue=20;
}
