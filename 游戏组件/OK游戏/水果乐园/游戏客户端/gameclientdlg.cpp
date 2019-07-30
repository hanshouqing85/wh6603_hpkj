#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "ExchangeDlg.h"

CGameClientDlg*	g_pGameClientDlg=NULL;
//////////////////////////////////////////////////////////////////////////
//��ʱ����ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_SHOW_LOGO				302									//��ʾ��ϷLOGO
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGameClientDlg,CGameFrameEngine)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(IDM_APPLY_BANKER,OnApplyBanker)
	ON_MESSAGE(IDM_BIG_SMALL,OnBigSmall)
	ON_MESSAGE(IDM_XU_YA,OnXuYa)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg()
{
//AllocConsole();
//freopen("CONOUT$","w+t",stdout);
//freopen("CONIN$","r+t",stdin);
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lApplyBankerCondition=0L;	
	m_lMeMaxScoreAfterEx = 0;
	//�û��һ���ĳ�����
	m_lUserChipCount = 0;
	//������ע
	ZeroMemory(m_lMeAreaChipArray, sizeof(m_lMeAreaChipArray));
	ZeroMemory(m_lXuYaArray,sizeof(m_lXuYaArray));
	ZeroMemory(m_lCurrentChip,sizeof(m_lCurrentChip));
	//ׯ����Ϣ
	m_lBankerChip=0L;
	m_wCurrentBanker=0L;
	//״̬����
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

//��ʼ����
bool CGameClientDlg::OnInitGameEngine()
{
	//���ñ���
	SetWindowText(TEXT("ˮ����԰��Ϸ"));
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	m_pGlobalUnits=CGlobalUnits::GetInstance();

	theApp.CloseControl();//���ز����

	return true;
}

//���ÿ��
bool CGameClientDlg::OnResetGameEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;	
	m_lUserChipCount=0;
	m_lMeMaxScoreAfterEx=0;
	//������ע
	ZeroMemory(m_lMeAreaChipArray, sizeof(m_lMeAreaChipArray));
	ZeroMemory(m_lXuYaArray,sizeof(m_lXuYaArray));
	//ׯ����Ϣ
	m_lBankerChip=0L;
	m_wCurrentBanker=0L;
	//״̬����
	m_bMeApplyBanker=false;
	m_bFirstEnter = true;
	return true;
}

//ʱ����Ϣ
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	if (wClockID==IDI_PLACE_JETTON)
	{
		//ׯ�Ұ�ť
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

//������Ϣ
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
	case SUB_S_CHIP_UPDATE:		//���³���
		{
			return OnSubChipUpdate(pData,wDataSize);
		}
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pData,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pData, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pData, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pData, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pData,wDataSize);
		}
	case SUB_S_BIG_SMALL:
		{
			return OnSubBigSmall(pData,wDataSize);
		}
	case SUB_S_ADMIN_COMMDN:		//����
		{
			return OnSubAdminControl(pData,wDataSize);
		}	
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) 
				return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			if( pStatusFree == NULL )
				return FALSE;
//printf("pStatusFree->dwChipRate=%d\n",pStatusFree->dwChipRate);

			m_lMyCurrentChip=0;

			CopyMemory(m_lCurrentChip,pStatusFree->lCurrentJetton,sizeof(m_lCurrentChip));

			////////////////////////////////////////////////////////////////////////////////
			//�����б�
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

			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

			//��ʾ��ϷLOGO
			m_GameClientView.m_bShowSGZZLogo=true;
			SetTimer(IDI_SHOW_LOGO,10000,NULL);

            //�����Ϣ
			m_lMeMaxScore =  pStatusFree->lUserMaxScore;//��ǰ��ҵĽ�������ɷ�������������������Ҫ��������ź�
 
			m_lUserChipCount = ScoreToChip(m_lMeMaxScore - m_lMeMaxScoreAfterEx);

			m_GameClientView.SetMeMaxChip(m_lUserChipCount);
			WORD wMeChairID=GetMeChairID();
		
			WORD wSwitchViewChairID=SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wSwitchViewChairID);
			
            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,ScoreToChip(pStatusFree->lBankerScore));
			m_GameClientView.SetBankerChip(pStatusFree->wBankerTime,ScoreToChip(pStatusFree->lBankerWinScore));
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			__int64 aLimitChips[JETTON_AREA_COUNT]={0};
			for(int i=0;i<JETTON_AREA_COUNT;i++)
			{
				aLimitChips[i]= ScoreToChip(pStatusFree->lAreaLimitScore[i]);
			}
			m_GameClientView.SetAreaLimitChip(aLimitChips, JETTON_AREA_COUNT);

			//����״̬
			SetGameStatus(GAME_STATUS_FREE);
			
			//��������
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

			//���¿���
			UpdateButtonContron();
			m_GameClientView.InvalidGameView(0,0,0,0);
			m_GameClientView.m_ExchangeDlg.DoModal();

			return true;
		}
	case GAME_STATUS_PLAY:		//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) 
				return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
			if( pStatusPlay == NULL )
				return FALSE;
//printf("pStatusFree->dwChipRate=%d\n",pStatusPlay->dwChipRate);

			m_lMyCurrentChip=0;

			CopyMemory(m_lCurrentChip,pStatusPlay->lCurrentJetton,sizeof(m_lCurrentChip));
			m_dwChipRate=pStatusPlay->dwChipRate;
			SetJettonRate(m_dwChipRate);

			////////////////////////////////////////////////////////////////////////////////
			//�����б�
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

			//ȫ����ע
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				m_GameClientView.PlaceUserJetton(i, ScoreToChip(pStatusPlay->lTotalJettonScore[i]));

			//�����ע
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				SetMePlaceJetton(i, ScoreToChip(pStatusPlay->lUserAreaScore[i]));

			//��һ���
			m_lMeMaxScore =  pStatusPlay->lUserMaxScore;//��ǰ��ҵĽ�������ɷ�������������������Ҫ��������ź�
			m_lUserChipCount = ScoreToChip(m_lMeMaxScore - m_lMeMaxScoreAfterEx);
 		
			m_GameClientView.SetMeMaxChip(m_lUserChipCount);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			__int64 aLimitChips[JETTON_AREA_COUNT]={0};
			for(int i=0;i<JETTON_AREA_COUNT;i++)
			{
				aLimitChips[i]= ScoreToChip(pStatusPlay->lAreaScoreLimit[i]);
			}
			m_GameClientView.SetAreaLimitChip(aLimitChips, JETTON_AREA_COUNT);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//���óɼ�
				m_GameClientView.SetCurGameChip(ScoreToChip(pStatusPlay->lEndUserScore),ScoreToChip(pStatusPlay->lEndUserReturnScore),ScoreToChip(pStatusPlay->lEndBankerScore),pStatusPlay->lEndRevenue);
			}
			else
			{
				//��������
				//StopSound();
				//PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			}

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,ScoreToChip(pStatusPlay->lBankerScore));
			m_GameClientView.SetBankerChip(pStatusPlay->cbBankerTime,ScoreToChip(pStatusPlay->lBankerWinScore));
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			SetGameClock(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			//��ʾ��ϷLOGO
			m_GameClientView.m_bShowSGZZLogo=true;
			SetTimer(IDI_SHOW_LOGO,10000,NULL);

			tagUserAttribute * UserAttribute=m_pIClientKernel->GetUserAttribute();
			ASSERT(UserAttribute!=NULL);
			if (UserAttribute->dwUserRight==1879048192)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}

			//���°�ť
			UpdateButtonContron();
			m_GameClientView.InvalidGameView(0,0,0,0);
			m_GameClientView.m_ExchangeDlg.DoModal();

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if( pGameStart == NULL )
		return FALSE;

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,ScoreToChip(pGameStart->lBankerScore));

	//�����Ϣ
	m_lMeMaxScore =  pGameStart->lUserMaxScore;
	m_lUserChipCount = ScoreToChip(m_lMeMaxScore - m_lMeMaxScoreAfterEx);
 
	m_GameClientView.SetMeMaxChip(m_lUserChipCount);

	//���ø���ע�����ʼ���·�
	__int64 aLimitChips[JETTON_AREA_COUNT]={0};
	for(int i=0;i<JETTON_AREA_COUNT;i++)
	{
		aLimitChips[i]= ScoreToChip(pGameStart->lAreaLimitScore[i]);
	}
	m_GameClientView.SetAreaLimitChip(aLimitChips, JETTON_AREA_COUNT);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//////////
	m_GameClientView.CleanUserJetton();
	//////////

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);

	//���¿���
	UpdateButtonContron();

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	if (m_GameClientView.m_AdminDlg.GetSafeHwnd())
	{
		m_GameClientView.m_AdminDlg.ResetUserBet();
	}

	//////////
	//������Ѻ
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

//��Ϸ����
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) 
		return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	if( pGameFree == NULL )
		return false;

	m_GameClientView.SetBankerChip(pGameFree->iBankerTime,ScoreToChip(pGameFree->lBankerTotallScore));

	m_GameClientView.m_btBankButton.ShowWindow(SW_SHOW);

	//����ʱ��
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

	//����״̬
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

	//��������
	m_GameClientView.SetWinnerSide(0xFF);
	for (int nAreaIndex=ID_BAR; nAreaIndex<=ID_APPLE; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);

	//�����б�
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

	//���¿ؼ�
	UpdateButtonContron();

	static bool AlreadyShow=false;
	if (m_lMyCurrentChip==0 && !AlreadyShow && m_bAlreadyExchange)
	{
		AlreadyShow=true;
		ShowInformation(TEXT("���ĳ��������꣬���˳���Ϸ�������������Ϸ��"));
		OnClose();//�˳���Ϸ
	}

	return true;
}

//�û���ע
bool CGameClientDlg::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if( pPlaceJetton == NULL )
		return false;
	if( pPlaceJetton->cbJettonArea>ID_APPLE || pPlaceJetton->cbJettonArea<ID_BAR )
		return false;

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,ScoreToChip(pPlaceJetton->lJettonScore));

	if (pPlaceJetton->cbAndroid==false && m_GameClientView.m_AdminDlg.GetSafeHwnd())
	{
		m_GameClientView.m_AdminDlg.SetUserBetScore(pPlaceJetton->cbJettonArea,ScoreToChip(pPlaceJetton->lJettonScore));
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if( pGameEnd == NULL )
		return FALSE;

	//��עʱ����������ٻ��ư��µİ�ť
	m_GameClientView.m_bisLButtonDown=false;

	////////////////////////////////////////////////////////////////////////////////
	//��Ѻ����
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
	//���ֿ�����ͨ�����GoodLuck����
	if (pGameEnd->cbWinArea==9 || pGameEnd->cbWinArea==21)
	{
		if (pGameEnd->cbGoodLuckType==0)//�Է֣��޶���
		{
			m_GameClientView.SetGoodLuckCartoon(pGameEnd->cbPaoHuoCheArea,pGameEnd->cbPaoHuoCheCount,pGameEnd->cbWinArea,pGameEnd->cbGoodLuckType);
		}
		else//�ж������������ֿ���
		{
			m_GameClientView.SetGoodLuckCartoon(pGameEnd->cbPaoHuoCheArea,pGameEnd->cbPaoHuoCheCount,pGameEnd->cbWinArea,pGameEnd->cbGoodLuckType);
		}
	}
	else//��ͨ����
	{
		m_GameClientView.UpdateCartoonTimeElapse(pGameEnd->cbWinArea);//���쿨ͨ��ʱ���������
	}
	////////////////////////////////////////////////////////////////////////////////

	//���÷��Ƽ�ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD,pGameEnd->cbTimeLeave);

	//ׯ����Ϣ
	//m_GameClientView.SetBankerChip(pGameEnd->iBankerTime,SCORETOCHIP(pGameEnd->lBankerTotallScore));

	//�ɼ���Ϣ
	if (GetMeChairID()!=m_wCurrentBanker)//ׯ����ʾ�ɼ���������
		m_GameClientView.SetCurGameChip(ScoreToChip(pGameEnd->lUserScore),ScoreToChip(pGameEnd->lUserReturnScore),ScoreToChip(pGameEnd->lBankerScore),pGameEnd->lRevenue);

	//����״̬
	SetGameStatus(GS_GAME_END);

	//���¿ؼ�
	UpdateButtonContron();

	//ֹͣ����

	return true;
}

//���¿���
void CGameClientDlg::UpdateButtonContron()
{
	//�����ж�
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
		bEnablePlaceJetton=false;

	if (GetGameStatus()!=GS_PLACE_JETTON) 
		bEnablePlaceJetton=false;

	if (m_wCurrentBanker==GetMeChairID()) 
		bEnablePlaceJetton=false;

	if (IsLookonMode())
		bEnablePlaceJetton=false;

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		//�������
		__int64 lCurrentJetton=m_GameClientView.GetCurrentJetton();
		__int64 lLeaveChip= 0;
		__int64	iTemp = 0;
		for( int i=0; i<JETTON_AREA_COUNT; i++ )
			iTemp += m_lMeAreaChipArray[i];
		lLeaveChip = m_lMeMaxScore -  (iTemp);
	}
	//ׯ�Ұ�ť
	if (!IsLookonMode())
	{
		//��ȡ��Ϣ
		IClientUserItem *pUserItem=GetTableUserItem(GetMeChairID());
		tagUserInfo *pMeUserData = NULL;
		if(pUserItem!=NULL)
			pMeUserData = pUserItem->GetUserInfo();

		//���밴ť
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
		if (GetGameStatus()!=GS_PLACE_JETTON)//�޸�����ע��һ˲�䣬����Ҫ��ׯ����ť����һ��
			m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//ȡ����ť
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GAME_STATUS_FREE) 
			bEnableCancel=false;

		if (m_bMeApplyBanker==false) 
			bEnableCancel=false;

		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

		//��ʾ�ж�
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

	//������Ϸ��¼��ť�ؼ�
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

//��ע��Ϣ
void CGameClientDlg::OnPlaceJetton(BYTE cbJettonArea, __int64 lJettonChip, bool bXuYa)
{


	CString str;
	str.Format(L"sg ��ע����%d",cbJettonArea);
	OutputDebugString(str);

	if( cbJettonArea > ID_APPLE || cbJettonArea < ID_BAR )
		return;

	//ׯ�Ҳ�����ע
	if ( GetMeChairID() == m_wCurrentBanker )
		return;

	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
		return;

	//����ע״̬��ֱ�ӷ���
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return;
	}

#ifdef FUNC_CHIP
	//��ǰ���벻��
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

	//������ע�������ñ�����ע����
	m_lMeAreaChipArray[cbJettonArea] += lJettonChip;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, m_lMeAreaChipArray[cbJettonArea]);

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=ChipToScore(lJettonChip);

	//����������Ϣ
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonContron();

	if (bXuYa) return;//��Ѻ��������Ч

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

//������Ϣ
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
	//�Թ��ж�
	if (IsLookonMode()) return true;
	//ת������
	bool bApplyBanker=wParam?true:false;
	//�����ǰ����ׯ�ң��������ٴ�����
	if (m_wCurrentBanker==GetMeChairID() && bApplyBanker) return true;

	if (bApplyBanker)
	{
		//����������ׯ��Ϣ
		SendSocketData(SUB_C_APPLY_BANKER,NULL,0);
		m_bMeApplyBanker=true;
	}
	else
	{
		//����ȡ����ׯ��Ϣ
		SendSocketData(SUB_C_CANCEL_BANKER,NULL,0);
		m_bMeApplyBanker=false;
	}
	//���ð�ť
	UpdateButtonContron();
	return true;
}

//�ȴ�С
LRESULT CGameClientDlg::OnBigSmall(WPARAM wParam, LPARAM lParam)//0��С��1�´�
{
	CMD_C_BigSmall BigSmall;
	memset(&BigSmall,0,sizeof(CMD_C_BigSmall));
	BigSmall.cbBigSmall=(BYTE)wParam;
	SendSocketData(SUB_C_BIG_SMALL,&BigSmall,sizeof(CMD_C_BigSmall));
	return 0;
}

//��Ѻ
LRESULT CGameClientDlg::OnXuYa(WPARAM wParam, LPARAM lParam)
{
	for (int i=0; i<JETTON_AREA_COUNT; i++)
	{
		if (m_lXuYaArray[i]>m_lMeAreaChipArray[i]) OnPlaceJetton(i,m_lXuYaArray[i]-m_lMeAreaChipArray[i],true);
	}
	return 0;
}

//ʱ����Ϣ
VOID CGameClientDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_SHOW_LOGO)
	{
		KillTimer(IDI_SHOW_LOGO);
		m_GameClientView.m_bShowSGZZLogo=false;
	}
	__super::OnTimer(nIDEvent);
}

//������ׯ
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;
	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;
	if (pApplyBanker==NULL) return false;
	//��ȡ���
	IClientUserItem *pUserItem=GetTableUserItem(pApplyBanker->wApplyUser);
	tagUserInfo *pUserData=NULL;
	if (pUserItem!=NULL) pUserData=pUserItem->GetUserInfo();
	if (pUserData==NULL) return false;
	//�������
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
	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;
	//���¿ؼ�
	UpdateButtonContron();
	return true;
}

//ȡ����ׯ
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;
	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;
	if (pCancelBanker==NULL) return false;
	//ɾ�����
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

	//�Լ��ж�
	IClientUserItem *pUserItem=GetTableUserItem(GetMeChairID());
	tagUserInfo *pMeUserData = NULL;
	if(pUserItem!=NULL)
		pMeUserData = pUserItem->GetUserInfo();
	if( pMeUserData == NULL )
		return FALSE;

	//����ȡ����ׯ
	if (IsLookonMode()==false && !lstrcmp(pMeUserData->szNickName,pCancelBanker->szCancelUser)) 
		m_bMeApplyBanker=false;

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�л�ׯ��
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) 
		return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;
	if( pChangeBanker == NULL )
		return FALSE;

	//��ʾͼƬ
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//�Լ��ж�
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//ׯ����
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerChip(0,0);

	//ɾ�����
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

	//���½���
	UpdateButtonContron();
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//��Ϸ��¼
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) 
		return false;

	if( pBuffer == NULL )
		return FALSE;

	//�������
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//���ü�¼
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

//��עʧ��
bool CGameClientDlg::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;
	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	if( pPlaceJettonFail == NULL ) return FALSE;

	//Ч�����
	BYTE cbViewIndex=pPlaceJettonFail->cbJettonArea;	//ȡ����ע����
	ASSERT(cbViewIndex<=ID_APPLE);
	if (cbViewIndex>ID_APPLE) return false;

	//��ע����
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

//�´�С���
bool CGameClientDlg::OnSubBigSmall(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_BigSmall));
	if (wDataSize!=sizeof(CMD_S_BigSmall)) return false;
	//��Ϣ����
	CMD_S_BigSmall * pBigSmall=(CMD_S_BigSmall *)pBuffer;
	if (pBigSmall==NULL) return false;

	if (pBigSmall->bWin)
	{
//printf("�ɹ�\n");
		m_GameClientView.m_bBigSmallWin=true;
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_DU_WIN"));
	}
	else
	{
//printf("ʧ��\n");
		m_GameClientView.m_bBigSmallWin=false;
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_DU_LOSE"));
	}
	//ׯ����Ϣ
	//m_GameClientView.SetBankerChip(pBigSmall->iBankerTime,SCORETOCHIP(pBigSmall->lBankerTotallScore));

	m_GameClientView.InsertBigSmallRecord(pBigSmall->cbBigSmall);

	//�ɼ���Ϣ
	if (GetMeChairID()!=m_wCurrentBanker)//ׯ����ʾ�ɼ���������
		m_GameClientView.SetCurGameChip(ScoreToChip(pBigSmall->lUserScore),0,ScoreToChip(pBigSmall->lBankerScore),0);

	m_GameClientView.m_cbGameEndTime=pBigSmall->cbBigSmall;

	return true;
}

//�һ�����
bool CGameClientDlg::OnSubChipUpdate(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChipUpdate));
	if (wDataSize!=sizeof(CMD_S_ChipUpdate)) return false;
	//��Ϣ����
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

//����ׯ��
void CGameClientDlg::SetBankerInfo(WORD wBanker,__int64 iChip)
{
	m_wCurrentBanker=wBanker;
	m_lBankerChip=iChip;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerChip);
}

//������ע
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<= ID_APPLE);
	if (cbViewIndex>ID_APPLE) 
		return;

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
			m_lMeAreaChipArray[i] = iJettonCount;
	}

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,iJettonCount);
}
//////////////////////////////////////////////////////////////////////////

//���ó������
void CGameClientDlg::SetJettonRate(DWORD dwJettonRate)
{
	ASSERT(dwJettonRate!=0);
	m_dwChipRate=dwJettonRate;
	return;
}

//����ת���ɳ���
__int64 CGameClientDlg::ScoreToChip(__int64 lScore)
{
	ASSERT(m_dwChipRate!=0);
	return (lScore/m_dwChipRate);
}

//����ת���ɻ���
__int64 CGameClientDlg::ChipToScore(__int64 lChip)
{
	ASSERT(m_dwChipRate!=0);
	return (lChip*m_dwChipRate);
}

//�һ�����
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

//�û�����
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

//�û��뿪
VOID CGameClientDlg::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	if (pIClientUserItem==NULL) return;
	m_GameClientView.m_PlayerList.DeleteUser(pIClientUserItem->GetUserID());
	return;
}

//��������
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

//�����˺ſ���
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

//�����˺ſ���
LRESULT CGameClientDlg::OnAdminControl(WPARAM wParam, LPARAM lParam)
{
	SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));
	return 0;
}

void CGameClientDlg::OnClose()
{
	PostQuitMessage(0);
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	return;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//������Ѻ20��
void CGameClientDlg::OnXuYaContinue()
{
	m_GameClientView.m_btBetLast2.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBetLast.ShowWindow(SW_HIDE);
	m_bXuYaContinue=true;
	m_cbXuYaContinue=20;
}
