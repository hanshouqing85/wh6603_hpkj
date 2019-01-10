 #include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientEngine.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��

#define IDI_ANDROID_BET				1000	

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER,OnApplyBanker)
	ON_MESSAGE(IDM_ADMIN_COMMDN,OnAdminCommand)
	ON_MESSAGE(IDM_CONTINUE_CARD,OnContinueCard)
	ON_MESSAGE(IDM_AUTO_OPEN_CARD,OnAutoOpenCard)		
	ON_MESSAGE(IDM_OPEN_CARD, OnOpenCard)	
	ON_MESSAGE(IDM_GET_ACCOUNT, OnGetAccount)	
	ON_MESSAGE(IDM_CHEAK_ACCOUNT, OnCheakAccount)	
	ON_WM_TIMER()
	ON_MESSAGE(IDM_CHANGE_MUSIC,OnChangeMusic)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine() 
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//״̬����
	m_bMeApplyBanker=false;

	//��������
	m_ListAndroid.RemoveAll();

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	m_bAllowBackSound = true;
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	ASSERT(m_pGlobalUnits!=NULL);
	m_bAllowBackSound= CGlobalUnits::GetInstance()->m_bAllowSound;
	if(m_bAllowBackSound)
	{
		m_GameClientView.m_btLast.EnableWindow(TRUE);
		m_GameClientView.m_btNext.EnableWindow(TRUE);
	}
	else
	{
		m_GameClientView.m_btLast.EnableWindow(FALSE);
		m_GameClientView.m_btNext.EnableWindow(FALSE);

	}

	//���ñ���
	SetWindowText(TEXT("����ţţ��Ϸ  --  Ver��6.6.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	if(0)
	{
		
		BYTE cbCardData[5]={0x42,0x08,0x08,0x28,0x17};
		BYTE cbCardData1[5]={0x13,0x02,0x16,0x38,0x33};
		for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) 
		{
			BYTE bcTmp[5];
			int iType = 0;
			iType = m_GameClientView.m_GameLogic.GetCardType(cbCardData,5,bcTmp);
			m_GameClientView.m_CardControl[i].SetCardData(cbCardData,5);
			m_GameClientView.m_lUserCardType[i] = iType;
			if(iType==CT_POINT)
			{
				CopyMemory(m_GameClientView.m_cbTableSortCardArray[i],bcTmp,5);
			}else
			{
				CopyMemory(m_GameClientView.m_cbTableSortCardArray[i],bcTmp+3,2);
				CopyMemory(m_GameClientView.m_cbTableSortCardArray[i]+2,bcTmp,3);

			}
		}
		
	//	m_GameClientView.m_CardControl[1].SetCardData(cbCardData1,5);
		m_GameClientView.SetMoveCardTimer();
	}
	CBassMusicManager::GetInstance()->Init(this->m_hWnd);
	CBassMusicManager::GetInstance()->LoadFromFile(0,("OxBattle/bk1.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(1,("OxBattle/bk2.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(2,("OxBattle/bk3.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(3,("OxBattle/bk4.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(4,("OxBattle/bk5.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(5,("OxBattle/bk6.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(6,("OxBattle/bk7.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(7,("OxBattle/bk8.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(8,("OxBattle/bk9.mp3"),true);
	CBassMusicManager::GetInstance()->SetVolumn(100);
	m_cbMusic = rand()%9;
//	CBassMusicManager::GetInstance()->Play(m_cbMusic,FALSE);

	//��������
	//VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	//VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	//VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	//VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));

	return true;
}
bool CGameClientEngine::AllowBackGroundSound( bool bAllowSound )
{
	m_bAllowBackSound = bAllowSound;
	if(!bAllowSound)
	{
		CBassMusicManager::GetInstance()->Stop(m_cbMusic);
	}
	else
	{
		if (GetGameStatus() == GS_PLACE_JETTON)
		{
			m_cbMusic = (m_cbMusic+1)%9;
			CBassMusicManager::GetInstance()->Play(m_cbMusic,FALSE);

		}
	}
	if(m_bAllowBackSound)
	{
		m_GameClientView.m_btLast.EnableWindow(TRUE);
		m_GameClientView.m_btNext.EnableWindow(TRUE);
	}
	else
	{
		m_GameClientView.m_btLast.EnableWindow(FALSE);
		m_GameClientView.m_btNext.EnableWindow(FALSE);

	}
	return true;
}
//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;			

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//״̬����
	m_bMeApplyBanker=false;

	//��������
	m_ListAndroid.RemoveAll();

	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{

	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=m_pGlobalUnits->m_bAllowSound;
	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//���ÿؼ�
		//EnableSound(GameOption.m_bEnableSound);

	}

	return;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);


		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}

	if (IsEnableSound()) 
	{
		if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
	}

	if (IsEnableSound()) 
	{
		if (nTimerID==IDI_PLACE_JETTON)
		{
	
			switch (rand()%3)
			{
			case 0:
				PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER1"));
				break;
			case 1:
				PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER2"));
				break;
			case 2:
				PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER3"));
				break;
			}
		}
	}
	return true;
}

////�Թ�״̬
//void CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
//{
//}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize,true);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_AMDIN_COMMAND:
		{
			return OnSubReqResult(pBuffer,wDataSize);
		}
	case SUB_S_SEND_ACCOUNT:		//����
		{
			CMD_C_SendAccount* pResult = (CMD_C_SendAccount*)pBuffer;
			m_GameClientView.m_pClientControlDlg->ResetUserNickName();
			for (int i=0; i<pResult->m_UserCount; i++ )
			{
				IClientUserItem *pUserData=GetTableUserItem(pResult->m_UserID[i]);
				m_GameClientView.m_pClientControlDlg->SetUserNickName(pUserData->GetNickName());
			}

			return true;
		}	
	case SUB_S_ADMIN_CHEAK:
		{
			CMD_S_UserJettonScore* pResult = (CMD_S_UserJettonScore*)pBuffer;
			for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
				m_GameClientView.m_pClientControlDlg->SetUserGameScore(nAreaIndex-1,pResult->lUserJettonScore[nAreaIndex]);
			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
	//������ʾЧ��
	for (int i = 0;i<5;i++)
	{
		m_GameClientView.m_CardControl[i].m_blShowResult = false;
		m_GameClientView.m_CardControl[i].m_blShowLineResult = false;
	}
	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

            //�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//��������
			//if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			//else m_DTSDBackground.Stop();

			//����״̬
			SetGameStatus(GAME_STATUS_FREE);
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			//TCHAR str[222];
			//sprintf(str,"%s",CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)?"��Ȩ��":"ûȨ��");
			//AfxMessageBox(str);
			//���¿���
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

			PlayGameSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			return true;
		}
	case GS_PLACE_JETTON:	//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//��ע��Ϣ
			for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
			{
				m_GameClientView.PlaceUserJetton(nAreaIndex,pStatusPlay->lAllJettonScore[nAreaIndex]);
				SetMePlaceJetton(nAreaIndex,pStatusPlay->lUserJettonScore[nAreaIndex]);
			}

			//��һ���
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//�˿���Ϣ
				m_GameClientView.SetCardInfo(pStatusPlay->cbTableCardArray);
				m_GameClientView.FinishDispatchCard(false);

				m_GameClientView.m_bShowGameResult = true;
				m_GameClientView.m_blMoveFinish = true;
				for (int i = 0;i<5;i++)
				{
					m_GameClientView.m_CardControl[i].m_blGameEnd = true;
					m_GameClientView.m_CardControl[i].m_blhideOneCard = false;

					BYTE bcTmp[5];
					int iType = m_GameLogic.GetCardType(pStatusPlay->cbTableCardArray[i],5,bcTmp);

					m_GameClientView.m_lUserCardType[i] = iType;
					if(iType==CT_POINT||iType==CT_SPECIAL_BOMEBOME)
					{
						CopyMemory(m_GameClientView.m_cbTableSortCardArray[i],bcTmp,5);
						m_GameClientView.m_CardControl[i].m_blShowLineResult = true;
					}
					else
					{
						CopyMemory(m_GameClientView.m_cbTableSortCardArray[i],bcTmp+3,2);
						CopyMemory(m_GameClientView.m_cbTableSortCardArray[i]+2,bcTmp,3);
						m_GameClientView.m_CardControl[i].m_blShowLineResult = false;
					}

					m_GameClientView.m_CardControl[i].SetCardData(m_GameClientView.m_cbTableSortCardArray[i],5,false);
					m_GameClientView.m_CardControl[i].m_blShowResult = true;
				}

				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(NULL);

				for (int i = 0;i<5;i++)
				{
					m_GameClientView.m_CardControl[i].m_CardItemArray.SetSize(0);
				}

			}

			////��������
			//if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			//else m_DTSDBackground.Stop();

			PlayGameSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);
			m_GameClientView.OnStateChange(pStatusPlay->cbGameStatus);

			//����ʱ��
			SetGameClock(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);
			
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			//TCHAR str[222];
			//sprintf(str,"%s",CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)?"��Ȩ��":"ûȨ��");
			//AfxMessageBox(str);

			//���°�ť
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_GameClientView.KillCardTime();

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);
	SetTimer(IDI_ANDROID_BET, 100, NULL);

	//���¿���
	UpdateButtonContron();

	//������ʾ
	m_GameClientView.SetDispatchCardTip(pGameStart->bContiueCard ? enDispatchCardTip_Continue : enDispatchCardTip_Dispatch);

	//��������
	m_ListAndroid.RemoveAll();

	if(m_bAllowBackSound)
	{
		CBassMusicManager::GetInstance()->Stop(m_cbMusic);
		m_cbMusic = (m_cbMusic+1)%9;
		CBassMusicManager::GetInstance()->Play(m_cbMusic,false);
	}
	if(m_bAllowBackSound)
	{
		m_GameClientView.m_btLast.EnableWindow(TRUE);
		m_GameClientView.m_btNext.EnableWindow(TRUE);
	}
	else
	{
		m_GameClientView.m_btLast.EnableWindow(FALSE);
		m_GameClientView.m_btNext.EnableWindow(FALSE);

	}


// 
// 	//��������
// 	if (IsEnableSound()) 
// 	{
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
// 		//m_DTSDBackground.Play(0,true);
// 	}
// 	else 
// 	{
// 		//m_DTSDBackground.Stop();
// 	}

	if (m_GameClientView.m_pClientControlDlg->GetSafeHwnd())		 
	{
		m_GameClientView.m_pClientControlDlg->ResetUserBet();
	}

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;
	m_GameClientView.m_btLast.EnableWindow(FALSE);
	m_GameClientView.m_btNext.EnableWindow(FALSE);

	for(int i = 0;i < AREA_COUNT+1;i++)
	{
		m_GameClientView.m_cbUserCardType[i] = INVALID_CHAIR;
	}

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//����ʱ��
	KillTimer(IDI_ANDROID_BET);

	//����״̬
	SetGameStatus(GAME_STATUS_FREE);

	//��������
	m_GameClientView.StopAnimal();
	m_GameClientView.SetWinnerSide(false, false, false,false, false);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_TIAN_MEN; nAreaIndex<=ID_HUANG_MEN; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	//�����˿�
	for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) m_GameClientView.m_CardControl[i].SetCardData(NULL,0);

	//���¿ؼ�
	UpdateButtonContron();

	//��ɷ���
	m_GameClientView.FinishDispatchCard(true);

	//���³ɼ�
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		//tagUserData const *pUserData = GetUserData(wUserIndex);
		IClientUserItem *pUserData=GetTableUserItem(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;
		CString strNick = pUserData->GetNickName();
		strNick = strNick.Left(4);
		strNick+=_T("****");

		//������Ϣ
		ApplyUser.lUserScore = pUserData->GetUserScore();
		ApplyUser.strUserName = strNick;
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize,bool bGameMes)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	srand(GetTickCount());

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	if (pPlaceJetton->bIsAndroid==FALSE && (m_GameClientView.m_pClientControlDlg->GetSafeHwnd())&&bGameMes)
	{
		m_GameClientView.m_pClientControlDlg->SetUserBetScore(pPlaceJetton->cbJettonArea - 1,pPlaceJetton->lJettonScore);
	}

	if (GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
	{
		//�Ƿ������
		if (pPlaceJetton->bIsAndroid)
		{
			//����
			static WORD wStFluc = 1;	//�������
			tagAndroidBet androidBet = {};
			androidBet.cbJettonArea = pPlaceJetton->cbJettonArea;
			androidBet.lJettonScore = pPlaceJetton->lJettonScore;
			androidBet.wChairID = pPlaceJetton->wChairID;
			androidBet.nLeftTime = ((rand()+androidBet.wChairID+wStFluc*3)%10+1)*100;
			wStFluc = wStFluc%3 + 1;

			//MyDebug(_T("client::OnSubPlaceJetton ��� %d ���� %d ��ע %I64d ʱ�� %d]"), androidBet.wChairID,
			//	androidBet.cbJettonArea, androidBet.lJettonScore, androidBet.nLeftTime);

			m_ListAndroid.AddTail(androidBet);
		}
		else
		{
			//��ע����
			m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

			//��������
			if (IsEnableSound()) 
			{
				if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
				{
					if (pPlaceJetton->lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
					else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
					//m_DTSDCheer[rand()%3].Play();
				}
			}
		}
	}

	UpdateButtonContron();
    
	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_GameClientView.m_blMoveFinish = false;
CBassMusicManager::GetInstance()->Stop(m_cbMusic);
	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(pGameEnd->cbTableCardArray);

	m_GameClientView.SetFirstShowCard(pGameEnd->bcFirstCard);

	m_GameClientView.ClearAreaFlash();

	//�����˿�
	for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) m_GameClientView.m_CardControl[i].SetCardData(m_GameClientView.m_cbTableCardArray[i],5);

	for(int i = 0;i<5;i++) 
	{
		BYTE bcTmp[5];
		int iType = m_GameClientView.m_GameLogic.GetCardType(m_GameClientView.m_cbTableCardArray[i],5,bcTmp);
	
		m_GameClientView.m_lUserCardType[i] = iType;
		if(iType==CT_POINT||iType==CT_SPECIAL_BOMEBOME)
		{
			CopyMemory(m_GameClientView.m_cbTableSortCardArray[i],bcTmp,5);
		}else
		{
			CopyMemory(m_GameClientView.m_cbTableSortCardArray[i],bcTmp+3,2);
			CopyMemory(m_GameClientView.m_cbTableSortCardArray[i]+2,bcTmp,3);

		}
	}

	//�O�Ó���Ƅ�
	//m_GameClientView.SetMoveCardTimer();

	//����״̬
	SetGameStatus(GS_GAME_END);

	m_cbLeftCardCount=pGameEnd->cbLeftCardCount;

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);
	{
		m_TempData.a = pGameEnd->lUserScore;
		m_TempData.b = pGameEnd->lUserReturnScore;
		m_TempData.c = pGameEnd->lBankerScore;
		m_TempData.d = pGameEnd->lRevenue;
	}

	for (int i = 0;i<5;i++)
	{
		m_GameClientView.m_CardControl[i].m_blGameEnd = false;
	}

	//���¿ؼ�
	UpdateButtonContron();

	//ֹͣ����
	//for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientEngine::UpdateButtonContron()
{
	//�����ж�
	bool bEnablePlaceJetton=true;
	
	if(m_wCurrentBanker==INVALID_CHAIR)		bEnablePlaceJetton = true;
	if (GetGameStatus()!=GS_PLACE_JETTON)	bEnablePlaceJetton=false;
	if (m_wCurrentBanker==GetMeChairID())	bEnablePlaceJetton=false;
	if (IsLookonMode())						bEnablePlaceJetton=false;
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR)		bEnablePlaceJetton=false;
	if (GetTableUserItem(GetMeChairID())->GetUserStatus()!=US_PLAYING)				bEnablePlaceJetton=false;

	if(GetGameStatus()==GS_GAME_END)
	{
		m_GameClientView.m_btOpenCard.EnableWindow(false);
		m_GameClientView.m_btAutoOpenCard.EnableWindow(false);

	}else
	{
		m_GameClientView.m_btOpenCard.EnableWindow(true);
		m_GameClientView.m_btAutoOpenCard.EnableWindow(true);

	}

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		//�������
		LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		//�����ע
		LONGLONG lUserMaxJetton=m_GameClientView.GetUserMaxJetton();

		//���ù��
		lLeaveScore = min((lLeaveScore/5),lUserMaxJetton); //�û����·� �����ֱȽ� �������屶 
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=1000L) m_GameClientView.SetCurrentJetton(1000L);
			else if (lLeaveScore>=500L) m_GameClientView.SetCurrentJetton(500L);
			else if (lLeaveScore>=200L) m_GameClientView.SetCurrentJetton(200L);
			else if (lLeaveScore>=100L) m_GameClientView.SetCurrentJetton(100L);
			else if (lLeaveScore>=50L) m_GameClientView.SetCurrentJetton(50L);
			else if (lLeaveScore>=10L) m_GameClientView.SetCurrentJetton(10L);
			else if (lLeaveScore>=5L) m_GameClientView.SetCurrentJetton(5L);
			else if (lLeaveScore>=1L) m_GameClientView.SetCurrentJetton(1L);
			else m_GameClientView.SetCurrentJetton(0L);
		}

		//���ư�ť
		int iTimer = 1;
		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=1*iTimer && lUserMaxJetton>=1*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=5*iTimer && lUserMaxJetton>=5*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10*iTimer && lUserMaxJetton>=10*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow((lLeaveScore>=50*iTimer && lUserMaxJetton>=50*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100*iTimer && lUserMaxJetton>=100*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow((lLeaveScore>=200*iTimer && lUserMaxJetton>=200*iTimer)?TRUE:FALSE);		
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=500*iTimer && lUserMaxJetton>=500*iTimer)?TRUE:FALSE);		
		m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore>=1000*iTimer && lUserMaxJetton>=1000*iTimer)?TRUE:FALSE);
	}
	else
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow(FALSE);
	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
	}

	//ׯ�Ұ�ť
	if (!IsLookonMode())
	{
		//��ȡ��Ϣ
		IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

		//���밴ť
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableApply=false;
		if (m_bMeApplyBanker) bEnableApply=false;
		if (pMeUserData->GetUserScore()<m_lApplyBankerCondition) bEnableApply=false;
		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//ȡ����ť
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GAME_STATUS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
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

		//���ư�ť
		if (GetMeChairID()==m_wCurrentBanker)
		{
			BOOL bEnableButton=TRUE;
			if (GetGameStatus()!=GAME_STATUS_FREE) bEnableButton=FALSE;
			if (m_cbLeftCardCount < 8) bEnableButton=FALSE;

			m_GameClientView.m_btContinueCard.ShowWindow(SW_SHOW);			
			m_GameClientView.m_btContinueCard.EnableWindow(bEnableButton);
		}
		else
		{
			m_GameClientView.m_btContinueCard.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
		m_GameClientView.m_btContinueCard.ShowWindow(SW_HIDE);
	}

	ReSetGameCtr();
	return;
}

//��ע��Ϣ
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=m_GameClientView.GetCurrentJetton();

	//�Ϸ��ж�
	ASSERT(cbJettonArea>=ID_TIAN_MEN && cbJettonArea<=ID_HUANG_MEN);
	if (!(cbJettonArea>=ID_TIAN_MEN && cbJettonArea<=ID_HUANG_MEN)) return 0;

	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	//״̬�ж�
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return true;
	}
	
	//���ñ���
	m_lUserJettonScore[cbJettonArea] += lJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, m_lUserJettonScore[cbJettonArea]);

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//������Ϣ
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonContron();

	//Ԥ����ʾ
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//��������
	if (IsEnableSound()) 
	{
		if (lJettonScore==1000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		switch (rand()%3)
		{
		case 0:
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER1"));
			break;
		case 1:
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER2"));
			break;
		case 2:
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER3"));
			break;
		}
	}

	return 0;
}
//�ֹ�����
LRESULT  CGameClientEngine::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
	//	m_pIStringMessage->InsertCustomString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"),RGB(255,0,255),true);
//		InsertGeneralString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"),RGB(255,0,255),true);
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = false;
//	InsertGeneralString(TEXT("��ϵͳ��ʾ����ѡ�����ֹ�����ģʽ�����ƺ�������ʹ������϶��˿ˣ�"),RGB(255,0,255),true);
	//m_pIStringMessage->InsertCustomString(TEXT("��ϵͳ��ʾ����ѡ�����ֹ�����ģʽ�����ƺ�������ʹ������϶��˿ˣ�"),RGB(255,0,255),true);
	//m_GameClientView.m_btOpenCard.EnableWindow(false);
	//m_GameClientView.m_btAutoOpenCard.EnableWindow(true);
	return 1;
}
//�Զ�����
LRESULT  CGameClientEngine::OnAutoOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
//		InsertGeneralString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"),RGB(255,0,255),true);
		//m_pIStringMessage->InsertCustomString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"),RGB(255,0,255),true);
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = true;
//	InsertGeneralString(TEXT("��ϵͳ��ʾ����ѡ�����Զ�����ģʽ�����ƺ�ϵͳ���Զ��������ҵ��ƣ�"),RGB(255,0,255),true);
	//m_pIStringMessage->InsertCustomString(TEXT("��ϵͳ��ʾ����ѡ�����Զ�����ģʽ�����ƺ�ϵͳ���Զ��������ҵ��ƣ�"),RGB(255,0,255),true);
	//m_GameClientView.m_btOpenCard.EnableWindow(true);
	//m_GameClientView.m_btAutoOpenCard.EnableWindow(false);
	return 1;
}
//�л�����
LRESULT  CGameClientEngine::OnChangeMusic(WPARAM wParam, LPARAM lParam)
{
	if(GetGameStatus() != GS_PLACE_JETTON)
		return 1;

	int nChange = (int)wParam;
	if(nChange == 1)
	{
		CBassMusicManager::GetInstance()->Stop(m_cbMusic);
		m_cbMusic = (m_cbMusic+9-1)%9;
		CBassMusicManager::GetInstance()->Play(m_cbMusic,false);
	}
	else
	{
		CBassMusicManager::GetInstance()->Stop(m_cbMusic);
		m_cbMusic = (m_cbMusic+1)%9;
		CBassMusicManager::GetInstance()->Play(m_cbMusic,false);

	}
	return true;

}
//��������
LRESULT CGameClientEngine::OnContinueCard(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	if (GetMeChairID()!=m_wCurrentBanker) return 0;
	if (GetGameStatus()!=GAME_STATUS_FREE) return 0;
	if (m_cbLeftCardCount < 8) return 0;
	if (IsLookonMode()) return 0;

	//������Ϣ
	SendSocketData(SUB_C_CONTINUE_CARD);

	//���ð�ť
	m_GameClientView.m_btContinueCard.EnableWindow(FALSE);

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	if (pMeUserData->GetUserScore() < m_lApplyBankerCondition) return true;

	//�Թ��ж�
	if (IsLookonMode()) return true;

	//ת������
	bool bApplyBanker = wParam ? true:false;

	//��ǰ�ж�
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;

	if (bApplyBanker)
	{
		//������Ϣ
		SendSocketData(SUB_C_APPLY_BANKER, NULL, 0);

		m_bMeApplyBanker=true;
	}
	else
	{
		//������Ϣ
		SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);

		m_bMeApplyBanker=false;
	}

	//���ð�ť
	UpdateButtonContron();

	return true;
}

//������ׯ
bool CGameClientEngine::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//��ȡ���
	//tagUserData const *pUserData=GetUserData(pApplyBanker->wApplyUser);
	IClientUserItem *pUserData=GetTableUserItem(pApplyBanker->wApplyUser);

	//�������
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;

		CString strNick = pUserData->GetNickName();
		strNick = strNick.Left(4);
		strNick+=_T("****");


		ApplyUser.strUserName=strNick;
		ApplyUser.lUserScore=pUserData->GetUserScore();
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);

		if(m_GameClientView.m_ApplyUser.GetItemCount()>MAX_APPLY_DISPLAY) 
		{
			m_GameClientView.m_btUp.EnableWindow(true);
			m_GameClientView.m_btDown.EnableWindow(true);  

		}else
		{
			m_GameClientView.m_btUp.EnableWindow(false);
			m_GameClientView.m_btDown.EnableWindow(false); 
		}
	}

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//ȡ����ׯ
bool CGameClientEngine::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//ɾ�����
	tagApplyUser ApplyUser;
	CString strNick,strTemp;
	strTemp.Format(L"%s",pCancelBanker->szCancelUser);
	strNick.Format(L"%s****",strTemp.Left(4));
	ApplyUser.strUserName=strNick;
	ApplyUser.lUserScore=0;
	m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);

	if(m_GameClientView.m_ApplyUser.GetItemCount()>MAX_APPLY_DISPLAY)
	{
		m_GameClientView.m_btUp.EnableWindow(true);
		m_GameClientView.m_btDown.EnableWindow(true);  

	}else
	{
		m_GameClientView.m_btUp.EnableWindow(false);
		m_GameClientView.m_btDown.EnableWindow(false); 
	}

	//�Լ��ж�
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	if (IsLookonMode()==false && lstrcmp(pMeUserData->GetNickName(),pCancelBanker->szCancelUser)==0) m_bMeApplyBanker=false;

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�л�ׯ��
bool CGameClientEngine::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

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

	//ׯ����Ϣ
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerScore(0,0);

	//ɾ�����
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//tagUserData const *pBankerUserData=GetUserData(m_wCurrentBanker);
		IClientUserItem *pBankerUserData=GetTableUserItem(m_wCurrentBanker);
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			CString strNick = pBankerUserData->GetNickName();
			strNick = strNick.Left(4);
			strNick+=_T("****");

			ApplyUser.strUserName = strNick;
			m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
			m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
		}
	}

	//���½���
	UpdateButtonContron();
	m_GameClientView.RefreshGameView();

	return true;
}

//��Ϸ��¼
bool CGameClientEngine::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//�������
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(pServerGameRecord->bWinShunMen, pServerGameRecord->bWinDuiMen, pServerGameRecord->bWinDaoMen,pServerGameRecord->bWinHuang);
	}

	return true;
}

//��עʧ��
bool CGameClientEngine::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;

	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

	//Ч�����
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
	ASSERT(cbViewIndex<=ID_HUANG_MEN && cbViewIndex>=ID_TIAN_MEN);
	if (!(cbViewIndex<=ID_HUANG_MEN && cbViewIndex>=ID_TIAN_MEN)) return false;

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

	//�Լ��ж�
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
		//�Ϸ�У��
		ASSERT(m_lUserJettonScore[cbViewIndex]>=lJettonCount);
		if (lJettonCount>m_lUserJettonScore[cbViewIndex]) return false;

		//������ע
		m_lUserJettonScore[cbViewIndex]-=lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lUserJettonScore[cbViewIndex]);
	}

	return true;
}

//����ׯ��
void CGameClientEngine::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	IClientUserItem *pUserData=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL==pUserData) ? 0 : pUserData->GetUserID();
	m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);
}

//������ע
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//�Ϸ��ж�
	ASSERT(cbViewIndex>=ID_TIAN_MEN && cbViewIndex<=ID_HUANG_MEN);
	if (!(cbViewIndex>=ID_TIAN_MEN && cbViewIndex<=ID_HUANG_MEN)) return;

	//���ñ���
	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}

//��������
void CGameClientEngine::ReSetGameCtr()
{
#ifdef __BANKER___
	if(!IsLookonMode())
	{
		switch (GetGameStatus())
		{
		case GAME_STATUS_FREE:
			{
				if(GetMeChairID()!=m_wCurrentBanker)
				{
					//if(m_GameClientView.m_DlgBank.m_hWnd!=NULL)
					//{
					//	m_GameClientView.m_DlgBank.AllowStorage(TRUE);
					//	m_GameClientView.m_DlgBank.AllowDraw(TRUE);
					//}

// 					m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
// 					m_GameClientView.m_btBankerStorage.EnableWindow(TRUE);
				}
				else
				{
					//if(m_GameClientView.m_DlgBank.m_hWnd!=NULL)
					//{
					//	m_GameClientView.m_DlgBank.AllowStorage(FALSE);
					//	m_GameClientView.m_DlgBank.AllowDraw(TRUE);
					//}

// 					m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
// 					m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
				}
				break;
			}
		case GS_GAME_END:
		case GS_PLACE_JETTON:
			{
				if(GetMeChairID()!=m_wCurrentBanker)
				{
					//if(m_GameClientView.m_DlgBank.m_hWnd)
					//{
					//	m_GameClientView.m_DlgBank.AllowStorage(FALSE);
					//	m_GameClientView.m_DlgBank.AllowDraw(TRUE);
					//}
// 					m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
// 					m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
				}
				else
				{
					//if(m_GameClientView.m_DlgBank.m_hWnd)
					//{
					//	m_GameClientView.m_DlgBank.AllowStorage(FALSE);
					//	m_GameClientView.m_DlgBank.AllowDraw(TRUE);
					//}
// 					m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
// 					m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
				}
				break;
			}
		default:break;
		}
	}
	else
	{
// 		m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
// 		m_GameClientView.m_btBankerStorage.EnableWindow(TRUE);
	}
#endif
}

void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == IDI_ANDROID_BET)
	{
		if (m_ListAndroid.GetCount() != 0)
		{
			POSITION pos = m_ListAndroid.GetTailPosition();
			POSITION posTmp = pos;

			//������ע����
			while (true)
			{
				if (pos == 0)
					break;

				posTmp = pos;
				tagAndroidBet & androidBet = m_ListAndroid.GetPrev(pos);
				
				androidBet.nLeftTime -= 100;
				if (androidBet.nLeftTime <= 0)
				{		
					//ģ����Ϣ
					CMD_S_PlaceJetton placeJetton = {};
					placeJetton.bIsAndroid = false;
					placeJetton.cbJettonArea = androidBet.cbJettonArea;
					placeJetton.lJettonScore = androidBet.lJettonScore;
					placeJetton.wChairID = androidBet.wChairID;

					//MyDebug(_T("client::OnTimer ��� %d ���� %d ��ע %I64d"), androidBet.wChairID, androidBet.cbJettonArea, androidBet.lJettonScore);

					OnSubPlaceJetton((void*)&placeJetton, sizeof(placeJetton),false);

					//ɾ��Ԫ��
					m_ListAndroid.RemoveAt(posTmp);
				}
			}
		}
	}

	CGameFrameEngine::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
bool CGameClientEngine::OnSubReqResult(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_CommandResult));
	if(wDataSize!=sizeof(CMD_S_CommandResult)) return false;

	if( NULL != m_GameClientView.m_pClientControlDlg && NULL != m_GameClientView.m_pClientControlDlg->GetSafeHwnd() )
	{
		m_GameClientView.m_pClientControlDlg->ReqResult(pBuffer);
	}

	return true;
}

LRESULT CGameClientEngine::OnAdminCommand(WPARAM wParam,LPARAM lParam)
{
	CMD_C_AdminReq* pData = (CMD_C_AdminReq*)wParam;
	CMD_C_AdminReq Application;
	ZeroMemory(&Application,sizeof(Application));
	Application.cbReqType = pData->cbReqType;
	tagAdminReq*pAdminReq=(tagAdminReq*)Application.cbExtendData;
	const tagAdminReq*pAdminReq2=reinterpret_cast<const tagAdminReq*>(pData->cbExtendData);
	pAdminReq->m_cbExcuteTimes = pAdminReq2->m_cbExcuteTimes;
	pAdminReq->m_cbControlStyle = pAdminReq2->m_cbControlStyle;
	pAdminReq->m_bWinArea[0] = pAdminReq2->m_bWinArea[0];
	pAdminReq->m_bWinArea[1] = pAdminReq2->m_bWinArea[1];
	pAdminReq->m_bWinArea[2] = pAdminReq2->m_bWinArea[2];

	SendSocketData(SUB_C_AMDIN_COMMAND,&Application,sizeof(Application));
	return true;
}