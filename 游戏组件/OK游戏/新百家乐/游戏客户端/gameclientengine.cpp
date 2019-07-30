#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include "GameOption.h"


//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_PLACE_JETTON_BUFFER		302									//�������								

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_PALY_BET,OnPlayBet)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_ADMIN_COMMDN,OnAdminCommand)
	ON_MESSAGE(IDM_PLAY_SOUND,OnPlaySound)
	ON_MESSAGE(IDM_CHANGE_MUSIC,OnChangeMusic)
	ON_MESSAGE(IDM_CANCEL_BET,OnCancelBet)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine() : CGameFrameEngine()
{
	//������Ϣ
	m_lPlayBetScore = 0L;			
	m_lPlayFreeSocre = 0L;		

	//��ע��
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	//ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	//ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));

	//ׯ����Ϣ
	m_wBankerUser = INVALID_CHAIR;
	m_lBankerScore = 0l;
	m_lBankerWinScore = 0l;
	m_wBankerTime = 0;

	//ϵͳ��ׯ
	m_bEnableSysBanker = false;

	//״̬����
	m_bMeApplyBanker = false;

	//������Ϣ
	m_lAreaLimitScore = 0L;	
	m_lApplyBankerCondition = 0l;

	ZeroMemory(m_bUserType,sizeof(m_bUserType));

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//���ñ���
	SetWindowText(TEXT("�ټ�����Ϸ  --  Ver��6.6.1.0"));

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
	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	if (m_pIClientKernel!=NULL) m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	//����ͼ��
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	CBassMusicManager::GetInstance()->Init(this->m_hWnd);
	CBassMusicManager::GetInstance()->LoadFromFile(0,("BaccaratNew/bk1.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(1,("BaccaratNew/bk2.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(2,("BaccaratNew/bk3.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(3,("BaccaratNew/bk4.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(4,("BaccaratNew/bk5.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(5,("BaccaratNew/bk6.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(6,("BaccaratNew/bk7.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(7,("BaccaratNew/bk8.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(8,("BaccaratNew/bk9.mp3"),true);
	CBassMusicManager::GetInstance()->SetVolumn(100);
 	m_cbMusic = rand()%9;
// 	CBassMusicManager::GetInstance()->Play(m_cbMusic,FALSE);

	//��������
	//m_DTSDBackground.InitDirectSound(this);
	//m_dwBackID = m_DTSDBackground.Create(TEXT("BACK_GROUND"));

	m_PlaceBetArray.RemoveAll();

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
		if (GetGameStatus() == GAME_SCENE_BET)
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
	m_lPlayBetScore = 0L;			
	m_lPlayFreeSocre = 0L;		

	//��ע��
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	//ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	//ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));

	//ׯ����Ϣ
	m_wBankerUser = INVALID_CHAIR;
	m_lBankerScore = 0l;
	m_lBankerWinScore = 0l;
	m_wBankerTime = 0;

	//ϵͳ��ׯ
	m_bEnableSysBanker = false;

	//״̬����
	m_bMeApplyBanker = false;

	//������Ϣ
	m_lAreaLimitScore = 0L;	
	m_lApplyBankerCondition = 0l;

	ZeroMemory(m_bUserType,sizeof(m_bUserType));

	return true;
}
//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	if ((wClockID == IDI_PLACE_JETTON)&&(nElapse == 0))
	{
		//ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));
		//m_GameClientView.SetCurrentBet(INVALID_CHAIR,0);

		//m_GameClientView.m_btBetComfirm.EnableWindow(FALSE);
		m_GameClientView.m_btBetCancel.EnableWindow(FALSE);

		//���ù��
		m_GameClientView.SetCurrentBet(0L);

		//��ֹ��ť
		m_GameClientView.m_btBet1.EnableWindow(FALSE);		
		m_GameClientView.m_btBet3.EnableWindow(FALSE);		
		m_GameClientView.m_btBet5.EnableWindow(FALSE);	
		m_GameClientView.m_btBet10.EnableWindow(FALSE);
		m_GameClientView.m_btBet50.EnableWindow(FALSE);
		m_GameClientView.m_btBet100.EnableWindow(FALSE);
		m_GameClientView.m_btBet1000.EnableWindow(FALSE);

		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );

		return false;
	}

	if ( CGlobalUnits::GetInstance()->m_bAllowSound && wClockID == IDI_PLACE_JETTON && nElapse<=5 ) 
	{
		//PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
	}

	if((wClockID == IDI_DISPATCH_CARD) && (nElapse==0) ) 
	{
		return false;
	}

	return true;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
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
			return OnSubPlaceBet(pData,wDataSize);
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
			//��ռ�ע
			while( m_PlaceBetArray.GetCount() > 0 )
			{
				CMD_S_PlaceBet &PlaceBet = m_PlaceBetArray[0];
				PlaceBet.cbAndroidUser = FALSE;
				OnSubPlaceBet(&PlaceBet,sizeof(PlaceBet));
				m_PlaceBetArray.RemoveAt(0);
			}
			//KillTimer(IDI_PLACE_JETTON_BUFFER);
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceBetFail(pData,wDataSize);
		}	
	case SUB_S_CANCEL_JETTON://ȡ����ע
		{
			return OnSubCancelBet(pData,wDataSize);
		}
	case SUB_S_AMDIN_COMMAND:
		{
			return OnSubReqResult(pData,wDataSize);
		}
		//��ʾ��Ϣ
	case SUB_S_TIP_INFO:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_TipInfo));
			if (wDataSize!=sizeof(CMD_S_TipInfo)) return false;

			//��Ϣ����
			CMD_S_TipInfo * pTipInfo = (CMD_S_TipInfo *)pData;

			//��ʾ��Ϣ
		//	CInformation Information(this);
		//	Information.ShowMessageBox(pTipInfo->szTipInfo, (UINT)MB_OK, (UINT)pTipInfo->cbTimeLeave);

			return true;
		}

	}

	//�������
	ASSERT(FALSE);

	return true;
}
//�л�����
LRESULT CGameClientEngine::OnChangeMusic( WPARAM wParam, LPARAM lParam )
{
	if(GetGameStatus() != GAME_SCENE_BET)
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

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//����״̬
			SetGameStatus(GAME_SCENE_FREE);

			//����ʱ��
			SetGameClock(GetMeChairID(), IDI_FREE, pStatusFree->cbTimeLeave);

            //������Ϣ
			m_lPlayFreeSocre = pStatusFree->lPlayFreeSocre;
		
			//ׯ����Ϣ
			m_wBankerUser = pStatusFree->wBankerUser;
			m_lBankerScore = pStatusFree->lBankerScore;
			m_lBankerWinScore = pStatusFree->lBankerWinScore;
			m_wBankerTime = pStatusFree->wBankerTime;
			m_GameClientView.SetSystemFaceID(pStatusFree->wFaceID);

			// ϵͳ��ׯ
			m_bEnableSysBanker = pStatusFree->bEnableSysBanker;

			// ���Ʊ���
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			m_lApplyBankerCondition = pStatusFree->lApplyBankerCondition;

			// ���ý���
			m_GameClientView.SetGameStatus(GAME_SCENE_FREE);
			m_GameClientView.SetMeChairID(SwitchViewChairID(GetMeChairID()));
			m_GameClientView.SetPlayBetScore(0);
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);
			m_GameClientView.SetBankerInfo(SwitchViewChairID(m_wBankerUser), m_lBankerScore, m_lBankerWinScore, m_wBankerTime);
			m_GameClientView.SetEnableSysBanker(m_bEnableSysBanker);

		
			// ���ư�ť
			if( CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hControlInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			// ���ý���
			m_GameClientView.FlexAnimation(enFlexNULL,false);

			//���¿���
			UpdateButtonContron();

			return true;
		}
	case GAME_SCENE_PLAY:		//��Ϸ״̬
	case GAME_SCENE_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			if( pStatusPlay->cbGameStatus == GAME_SCENE_END )
				SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD,pStatusPlay->cbTimeLeave);
			else
				SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			//��ע��Ϣ
			memcpy(m_lAllBet, pStatusPlay->lAllBet, sizeof(m_lAllBet));
			memcpy(m_lPlayBet, pStatusPlay->lPlayBet, sizeof(m_lPlayBet));
			//memcpy(m_lPlayScore, pStatusPlay->lPlayScore, sizeof(m_lPlayScore));
			
			//������Ϣ
			m_lPlayBetScore = pStatusPlay->lPlayBetScore;
			m_lPlayFreeSocre = pStatusPlay->lPlayFreeSocre;

			//ׯ����Ϣ
			m_wBankerUser = pStatusPlay->wBankerUser;
			m_lBankerScore = pStatusPlay->lBankerScore;
			m_lBankerWinScore = pStatusPlay->lBankerWinScore;
			m_wBankerTime = pStatusPlay->wBankerTime;
			m_GameClientView.SetSystemFaceID(pStatusPlay->wFaceID);
			// ϵͳ��ׯ
			m_bEnableSysBanker = pStatusPlay->bEnableSysBanker;

			// ���Ʊ���
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			m_lApplyBankerCondition = pStatusPlay->lApplyBankerCondition;

			// ���ý���
			m_GameClientView.SetGameStatus(pStatusPlay->cbGameStatus);
			m_GameClientView.SetMeChairID(SwitchViewChairID(GetMeChairID()));
			m_GameClientView.SetPlayBetScore(m_lPlayBetScore);
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);
			m_GameClientView.SetBankerInfo(SwitchViewChairID(m_wBankerUser), m_lBankerScore, m_lBankerWinScore, m_wBankerTime);
			m_GameClientView.SetEnableSysBanker(m_bEnableSysBanker);
			m_GameClientView.SetCardInfo(pStatusPlay->cbCardCount, pStatusPlay->cbTableCardArray);

			// ������ע����
			for( int i = 0 ; i < AREA_MAX; ++i )
			{
				m_GameClientView.SetPlayBet(i, m_lPlayBet[i]);
				m_GameClientView.SetAllBet(i, m_lAllBet[i]);
				m_GameClientView.AddChip(i, m_lAllBet[i]);
			}

			// ���ý�����Ϣ
			if ( pStatusPlay->cbGameStatus == GAME_SCENE_END && !IsLookonMode() )
			{
				//�ɼ���Ϣ
				m_GameClientView.SetCurGameScore(pStatusPlay->lPlayScore,pStatusPlay->lPlayAllScore);
			}

			// ���ý���
			if ( pStatusPlay->cbGameStatus == GAME_SCENE_END )
			{
				
				//�����˿�
				m_GameClientView.m_CardControl[INDEX_PLAYER].SetCardData(pStatusPlay->cbTableCardArray[INDEX_PLAYER], pStatusPlay->cbCardCount[INDEX_PLAYER]);
				m_GameClientView.m_CardControl[INDEX_BANKER].SetCardData(pStatusPlay->cbTableCardArray[INDEX_BANKER], pStatusPlay->cbCardCount[INDEX_BANKER]);

				// ���ý���
				m_GameClientView.FlexAnimation(enFlexDealCrad, true, false);

				//����
				m_GameClientView.FlashAnimation(true);
			}
			else
			{
				// ���ý���
				m_GameClientView.FlexAnimation(enFlexBetTip, true, false);
			}
	
			//���ư�ť
			if( CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hControlInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);


			//���°�ť
			UpdateButtonContron();

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

	//�����Ϣ
	m_lPlayBetScore = pGameStart->lPlayBetScore;
	m_lPlayFreeSocre = pGameStart->lPlayFreeSocre;
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

	//ׯ����Ϣ
	m_wBankerUser = pGameStart->wBankerUser;
	m_lBankerScore = pGameStart->lBankerScore;

	//���ý���
	m_GameClientView.SetPlayBetScore(m_lPlayBetScore);
	m_GameClientView.SetBankerInfo(SwitchViewChairID(m_wBankerUser), m_lBankerScore );

	//����ʱ��
	WORD wMeChair = GetMeChairID();
	SetGameClock(wMeChair,IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//����״̬
	SetGameStatus(GAME_SCENE_BET);
	m_GameClientView.SetGameStatus(GAME_SCENE_BET);

	//���¿���
	UpdateButtonContron();

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

// 	//��������
// 	if ( CGlobalUnits::GetInstance()->m_bAllowSound ) 
// 	{
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
// 		//m_DTSDBackground.Play(m_dwBackID,true);
// 	}

	  if (m_GameClientView.m_pClientControlDlg->GetSafeHwnd())
	  {
		m_GameClientView.m_pClientControlDlg->ResetUserBet();
	  }

	m_PlaceBetArray.RemoveAll();

	ZeroMemory(m_bUserType,sizeof(m_bUserType));

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


	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//����״̬
	SetGameStatus(GAME_SCENE_FREE);
	m_GameClientView.SetGameStatus(GAME_SCENE_FREE);

	//�����ע
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	//ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));

	//��ɷ���
	m_GameClientView.FinishDispatchCard();

	//��������
	m_GameClientView.CleanUserBet();

	//����
	m_GameClientView.FlashAnimation(false);

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubPlaceBet(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceBet));
	if (wDataSize!=sizeof(CMD_S_PlaceBet)) return false;

	//��Ϣ����
	CMD_S_PlaceBet * pPlaceBet=(CMD_S_PlaceBet *)pBuffer;

	//�����ж�
	if (pPlaceBet->cbAndroidUser)
	{
		//�γ�����,����ʾ��������ע��Ϣ
		if(!CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
		{
		m_bUserType[pPlaceBet->wChairID] = 1;
		m_PlaceBetArray.Add(*pPlaceBet);
		if (m_PlaceBetArray.GetCount()==1) SetTimer(IDI_PLACE_JETTON_BUFFER,70,NULL);
		}
		return true;
	}

	 if (m_bUserType[pPlaceBet->wChairID]==0 && (m_GameClientView.m_pClientControlDlg->GetSafeHwnd()))
	{
		m_GameClientView.m_pClientControlDlg->SetUserBetScore(pPlaceBet->cbBetArea,pPlaceBet->lBetScore);
	}



	if (GetMeChairID() != pPlaceBet->wChairID || IsLookonMode())
	{

		//�γ��������,����ʾ��������ע��Ϣ
		if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&pPlaceBet->cbAndroidUser)
		{
		//��������ע��Ϣ,����ʾ


		}else
		{
		//��ע����
		m_lAllBet[pPlaceBet->cbBetArea] += pPlaceBet->lBetScore;
		m_GameClientView.SetAllBet( pPlaceBet->cbBetArea, m_lAllBet[pPlaceBet->cbBetArea]);
		m_GameClientView.AddChip( pPlaceBet->cbBetArea,pPlaceBet->lBetScore );
		}

		//��������
		if (CGlobalUnits::GetInstance()->m_bAllowSound) 
		{
			if (pPlaceBet->wChairID!=GetMeChairID() || IsLookonMode())
			{
				if (pPlaceBet->lBetScore == 1000) 
					PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
				else 
					PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
			}
		}
	}
    
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
	CBassMusicManager::GetInstance()->Stop(m_cbMusic);
	m_GameClientView.m_btLast.EnableWindow(FALSE);
	m_GameClientView.m_btNext.EnableWindow(FALSE);


	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//����ʱ��
	KillTimer(IDI_PLACE_JETTON_BUFFER);

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(pGameEnd->cbCardCount,pGameEnd->cbTableCardArray);

	//ׯ����Ϣ
	m_GameClientView.SetBankerOverInfo( pGameEnd->lBankerTotallScore, pGameEnd->nBankerTime);

	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lPlayScore,pGameEnd->lPlayAllScore);

	//����״̬
	SetGameStatus(GAME_SCENE_END);
	m_GameClientView.SetGameStatus(GAME_SCENE_END);

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//���¿���
void CGameClientEngine::UpdateButtonContron()
{
	//�����ж�
	bool bEnablePlaceBet = true;
	if ( (m_bEnableSysBanker == false && m_wBankerUser == INVALID_CHAIR )
		|| (GetGameStatus() != GAME_SCENE_BET)
		|| (m_wBankerUser == GetMeChairID())
		|| (IsLookonMode()) )
	{
		bEnablePlaceBet = false;
	}

	//m_GameClientView.m_btBetComfirm.EnableWindow(FALSE);
	m_GameClientView.m_btBetCancel.EnableWindow(FALSE);
	//��ע��ť
	if ( bEnablePlaceBet )
	{
		//�������
		LONGLONG lCurrentBet = m_GameClientView.GetCurrentBet();


		LONGLONG lLeaveScore = 0;
		for ( int i = 0; i < AREA_MAX; ++i )
		{
			lLeaveScore = max( lLeaveScore, m_GameClientView.GetMaxPlayerScore(i) );
		}

		//���ù��
		if (lCurrentBet>lLeaveScore)
		{
			if (lLeaveScore>=1000) m_GameClientView.SetCurrentBet(1000);
			else if (lLeaveScore>=100) m_GameClientView.SetCurrentBet(100);
			else if (lLeaveScore>=50) m_GameClientView.SetCurrentBet(50);
			else if (lLeaveScore>=10) m_GameClientView.SetCurrentBet(10);
			else if (lLeaveScore>=5) m_GameClientView.SetCurrentBet(5);
			else if (lLeaveScore>=3) m_GameClientView.SetCurrentBet(3);
			else if (lLeaveScore>=1) m_GameClientView.SetCurrentBet(1);
			else m_GameClientView.SetCurrentBet(0L);
		}

		//���ư�ť
		m_GameClientView.m_btBet1.EnableWindow((lLeaveScore>=1)?TRUE:FALSE);
		m_GameClientView.m_btBet3.EnableWindow((lLeaveScore>=3)?TRUE:FALSE);
		m_GameClientView.m_btBet5.EnableWindow((lLeaveScore>=5)?TRUE:FALSE);
		m_GameClientView.m_btBet10.EnableWindow((lLeaveScore>=10)?TRUE:FALSE);
		m_GameClientView.m_btBet50.EnableWindow((lLeaveScore>=50)?TRUE:FALSE);		
		m_GameClientView.m_btBet100.EnableWindow((lLeaveScore>=100)?TRUE:FALSE);
		m_GameClientView.m_btBet1000.EnableWindow((lLeaveScore>=1000)?TRUE:FALSE);
		//for(WORD i = 0;i<AREA_MAX;i++)
		//{
		//	if(m_lCurrentScore[i]>0)
		//	{
		//		m_GameClientView.m_btBetComfirm.EnableWindow(TRUE);
		//		m_GameClientView.m_btBetCancel.EnableWindow(TRUE);
		//		break;
		//	}
		//}
		m_GameClientView.m_btBetCancel.EnableWindow(m_GameClientView.IsHasBet());
	}
	else
	{
		//���ù��
		m_GameClientView.SetCurrentBet(0L);

		//��ֹ��ť
		m_GameClientView.m_btBet1.EnableWindow(FALSE);		
		m_GameClientView.m_btBet3.EnableWindow(FALSE);		
		m_GameClientView.m_btBet5.EnableWindow(FALSE);	
		m_GameClientView.m_btBet10.EnableWindow(FALSE);
		m_GameClientView.m_btBet50.EnableWindow(FALSE);
		m_GameClientView.m_btBet100.EnableWindow(FALSE);
		m_GameClientView.m_btBet1000.EnableWindow(FALSE);

		//ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));
		//m_GameClientView.SetCurrentBet(INVALID_CHAIR,0);
	}

	//ׯ�Ұ�ť
	if ( !IsLookonMode() )
	{
		//��ȡ��Ϣ
		IClientUserItem* pMeUserItem = GetMeUserItem();

		//���밴ť
		bool bEnableApply = true;
		if (m_wBankerUser == GetMeChairID() || m_bMeApplyBanker || m_lPlayFreeSocre < m_lApplyBankerCondition || pMeUserItem->GetUserScore() < m_lApplyBankerCondition) 
			bEnableApply = false;

		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//ȡ����ť
		bool bEnableCancel = true;
		if ( m_wBankerUser == GetMeChairID() && GetGameStatus() != GAME_SCENE_FREE ) bEnableCancel=false;
		if ( m_bMeApplyBanker == false ) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wBankerUser==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

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

	m_GameClientView.InvalidGameView(0,0,0,0);

	return;
}

LRESULT CGameClientEngine::OnCancelBet(WPARAM wParam, LPARAM lParam)
{
	//ׯ���ж�
	if ( GetMeChairID() == m_wBankerUser ) return true;

		//״̬�ж�
	if (GetGameStatus() != GAME_SCENE_BET)
	{
		UpdateButtonContron();
		return true;
	}

	m_GameClientView.m_btBetCancel.EnableWindow(FALSE);

	//������Ϣ
	SendSocketData(SUB_C_CANCEL_JETTON);

	//ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));
	//m_GameClientView.SetCurrentBet(INVALID_CHAIR,0);

	//���°�ť
	//UpdateButtonContron();

	return 0;
}
//LRESULT CGameClientEngine::OnConfirmBet(WPARAM wParam, LPARAM lParam)
//{
//	//ׯ���ж�
//	if ( GetMeChairID() == m_wBankerUser ) return true;
//
//		//״̬�ж�
//	if (GetGameStatus() != GAME_SCENE_BET)
//	{
//		UpdateButtonContron();
//		return true;
//	}
//
//	CMD_C_PlaceBet PlaceBet;
//
//	for(BYTE i = 0;i<AREA_MAX;i++)
//	{
//		LONGLONG lBetScore = m_lCurrentScore[i];
//		if(lBetScore>0)
//		{
//			m_lAllBet[i] += lBetScore;
//			m_lPlayBet[i] += lBetScore;
//
//			//���ý���
//			m_GameClientView.SetPlayBet(i, m_lPlayBet[i]);
//			m_GameClientView.SetAllBet(i, m_lAllBet[i]);
//			m_GameClientView.AddChip(i, lBetScore);
//
//			ZeroMemory(&PlaceBet,sizeof(PlaceBet));
//
//			//�������
//			PlaceBet.cbBetArea = i;
//			PlaceBet.lBetScore = lBetScore;
//
//			//������Ϣ
//			SendSocketData(SUB_C_PLACE_JETTON,&PlaceBet,sizeof(PlaceBet));
//		}
//	}
//
//	//ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));
//	//m_GameClientView.SetCurrentBet(INVALID_CHAIR,0);
//
//	//���°�ť
//	UpdateButtonContron();
//	return 0;
//}
//��ע��Ϣ
LRESULT CGameClientEngine::OnPlayBet(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbBetArea=(BYTE)wParam;
	LONGLONG lBetScore= m_GameClientView.GetCurrentBet();

	//ׯ���ж�
	if ( GetMeChairID() == m_wBankerUser ) return true;

	//״̬�ж�
	if (GetGameStatus() != GAME_SCENE_BET)
	{
		UpdateButtonContron();
		return true;
	}

	//���ñ���
	//m_lCurrentScore[cbBetArea]+=lBetScore;
	m_lAllBet[cbBetArea] += lBetScore;
	m_lPlayBet[cbBetArea] += lBetScore;

	//���ý���
	m_GameClientView.SetPlayBet(cbBetArea, m_lPlayBet[cbBetArea]);
	m_GameClientView.SetAllBet(cbBetArea, m_lAllBet[cbBetArea]);
	m_GameClientView.AddChip(cbBetArea, lBetScore);
	//m_GameClientView.SetCurrentBet(cbBetArea,m_lCurrentScore[cbBetArea]);

	//��������
	CMD_C_PlaceBet PlaceBet;
	ZeroMemory(&PlaceBet,sizeof(PlaceBet));

	//�������
	PlaceBet.cbBetArea = cbBetArea;
	PlaceBet.lBetScore = lBetScore;

	//������Ϣ
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceBet,sizeof(PlaceBet));

	//���°�ť
	UpdateButtonContron();

	//��������
	if (CGlobalUnits::GetInstance()->m_bAllowSound) 
	{
		if (lBetScore == 1000) 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	}

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	IClientUserItem* pMeUserItem = GetMeUserItem();
	if (pMeUserItem->GetUserScore() < m_lApplyBankerCondition) return true;
	if (m_lPlayFreeSocre < m_lApplyBankerCondition) return true;

	//�Թ��ж�
	if (IsLookonMode()) return true;

	//ת������
	bool bApplyBanker = (wParam!=0) ? true:false;

	//��ǰ�ж�
	if (m_wBankerUser == GetMeChairID() && bApplyBanker) return true;

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

//������Ϣ
LRESULT CGameClientEngine::OnPlaySound(WPARAM wParam, LPARAM lParam)
{
	if( !CGlobalUnits::GetInstance()->m_bAllowSound )
		return 0;

	PlayGameSound(AfxGetInstanceHandle(), (TCHAR*)wParam);
	return 0;
}

//������ׯ
bool CGameClientEngine::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker = (CMD_S_ApplyBanker *)pBuffer;

	//��ȡ���
	IClientUserItem* pClientUserItem = GetTableUserItem(pApplyBanker->wApplyUser);

	if ( pClientUserItem == NULL )
		return true;

	//�������
	if (m_wBankerUser != pApplyBanker->wApplyUser)
	{
		m_GameClientView.m_ValleysList.Add( SwitchViewChairID(pApplyBanker->wApplyUser) );
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
	for( int i = 0; i < m_GameClientView.m_ValleysList.GetCount(); ++i )
	{
		if ( SwitchViewChairID(pCancelBanker->wCancelUser) == m_GameClientView.m_ValleysList[i] )
		{
			m_GameClientView.m_ValleysList.RemoveAt(i);
			break;
		}
	}

	//�Լ��ж�
	if ( IsLookonMode() == false && pCancelBanker->wCancelUser == GetMeChairID() ) 
		m_bMeApplyBanker = false;

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

	m_GameClientView.SetSystemFaceID(pChangeBanker->wFaceID);
	//��ʾͼƬ
	m_GameClientView.ShowChangeBanker(m_wBankerUser!=pChangeBanker->wBankerUser);

	//�Լ��ж�
	if (m_wBankerUser==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//ׯ����
	m_wBankerUser = pChangeBanker->wBankerUser;
	m_lBankerScore = pChangeBanker->lBankerScore;
	m_lBankerWinScore = 0;
	m_wBankerTime = 0;
	m_GameClientView.SetBankerInfo(SwitchViewChairID(m_wBankerUser), m_lBankerScore, m_lBankerWinScore, m_wBankerTime);

	//ɾ�����
	if (m_wBankerUser != INVALID_CHAIR)
	{
		for(int i = 0; i < m_GameClientView.m_ValleysList.GetCount(); ++i)
		{
			if( SwitchViewChairID(m_wBankerUser) == m_GameClientView.m_ValleysList[i] )
			{
				m_GameClientView.m_ValleysList.RemoveAt(i);
				break;
			}
		}
	}

	//���½���
	UpdateButtonContron();

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

		m_GameClientView.SetGameHistory(enOperateResult_NULL, pServerGameRecord->cbPlayerCount, pServerGameRecord->cbBankerCount,
			pServerGameRecord->cbKingWinner,pServerGameRecord->bPlayerTwoPair,pServerGameRecord->bBankerTwoPair);
	}

	return true;
}

//ȡ����ע
bool CGameClientEngine::OnSubCancelBet(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBet));
	if (wDataSize!=sizeof(CMD_S_CancelBet)) return false;

	CMD_S_CancelBet *pCancelBet = (CMD_S_CancelBet*)pBuffer;
	
	for(BYTE i = 0 ;i<AREA_MAX;i++)
	{
		if(pCancelBet->lPlayBet[i]==0) continue;
			
		if(pCancelBet->wChairID == GetMeChairID()&& !IsLookonMode() )
		{
			m_lPlayBet[i] -= pCancelBet->lPlayBet[i];
			m_GameClientView.SetPlayBet(i, m_lPlayBet[i]);
		}
		
		m_lAllBet[i] -= pCancelBet->lPlayBet[i];
		m_GameClientView.SetAllBet(i, m_lAllBet[i]);

		m_GameClientView.CleanAreaChip(i);

		if (m_GameClientView.m_pClientControlDlg->GetSafeHwnd())
		{
			m_GameClientView.m_pClientControlDlg->SetUserBetScore(i,-pCancelBet->lPlayBet[i]);
		}

		if(m_lAllBet[i]>0)
		{
			m_GameClientView.AddChip(i, m_lAllBet[i]);
		}	
	}

	UpdateButtonContron();
	return true;
}

//��עʧ��
bool CGameClientEngine::OnSubPlaceBetFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceBetFail));
	if (wDataSize!=sizeof(CMD_S_PlaceBetFail)) return false;

	//��Ϣ����
	CMD_S_PlaceBetFail * pPlaceBetFail=(CMD_S_PlaceBetFail *)pBuffer;

	//Ч�����
	BYTE cbViewIndex = pPlaceBetFail->lBetArea;
	ASSERT(cbViewIndex < AREA_MAX);
	if (cbViewIndex >= AREA_MAX) return false;

	 if ((m_GameClientView.m_pClientControlDlg->GetSafeHwnd()))
	{
		m_GameClientView.m_pClientControlDlg->SetUserBetScore(pPlaceBetFail->lBetArea,-pPlaceBetFail->lPlaceScore);
	}

	//�Լ��ж�
	if ( GetMeChairID() == pPlaceBetFail->wPlaceUser && !IsLookonMode() )
	{
		//���ñ���
		m_lPlayBet[cbViewIndex] -= pPlaceBetFail->lPlaceScore;
		m_lAllBet[cbViewIndex] -= pPlaceBetFail->lPlaceScore;
		
		//���ý���
		m_GameClientView.SetPlayBet(cbViewIndex, m_lPlayBet[cbViewIndex]);
		m_GameClientView.SetAllBet(cbViewIndex, m_lAllBet[cbViewIndex]);
		m_GameClientView.AddChip(pPlaceBetFail->lBetArea, -pPlaceBetFail->lPlaceScore);
	}
	UpdateButtonContron();

	return true;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CGameClientEngine::OnAdminCommand(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_AMDIN_COMMAND,(CMD_C_AdminReq*)wParam,sizeof(CMD_C_AdminReq));
	return true;
}

bool CGameClientEngine::OnSubReqResult(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(CMD_S_CommandResult));
	if(wDataSize!=sizeof(CMD_S_CommandResult)) return false;

	m_GameClientView.m_pClientControlDlg->ReqResult(pBuffer);
	return true;
}

void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	if (IDI_PLACE_JETTON_BUFFER == nIDEvent)
	{
		if (m_PlaceBetArray.GetCount()>0)
		{
			CMD_S_PlaceBet &PlaceBet=m_PlaceBetArray[0];
			PlaceBet.cbAndroidUser=FALSE;
			OnSubPlaceBet(&PlaceBet,sizeof(PlaceBet));
			m_PlaceBetArray.RemoveAt(0);

			if(m_PlaceBetArray.GetCount()>30)
			{
				CMD_S_PlaceBet &PlaceBet=m_PlaceBetArray[0];
				PlaceBet.cbAndroidUser=FALSE;
				OnSubPlaceBet(&PlaceBet,sizeof(PlaceBet));
				m_PlaceBetArray.RemoveAt(0);
			}
		}

		if (m_PlaceBetArray.GetCount() == 0) KillTimer(IDI_PLACE_JETTON_BUFFER);
		return;
	}

	CGameFrameEngine::OnTimer(nIDEvent);
}

