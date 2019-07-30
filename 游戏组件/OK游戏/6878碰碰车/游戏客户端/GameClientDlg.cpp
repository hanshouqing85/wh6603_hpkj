 #include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
#include ".\gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_OPEN_CARD				302								    //����ʱ��

#define IDI_ANDROID_BET				1000	


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameEngine)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_CONTINUE_CARD, OnContinueCard)
	ON_MESSAGE(IDM_AUTO_OPEN_CARD, OnAutoOpenCard)	
	ON_MESSAGE(IDM_OPEN_CARD, OnOpenCard)	
	ON_MESSAGE(IDM_SHOW_RESULT,OnShowResult)
	ON_MESSAGE(IDM_SOUND,OnPlaySound)
	ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)
	ON_MESSAGE(IDM_CHEAK_ACCOUNT, OnCheckAccount)
	ON_MESSAGE(IDM_GET_ACCOUNT,OnGetAccount)
	ON_MESSAGE(IDM_CHANGE_MUSIC,OnChangeMusic)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//���캯��
CGameClientDlg::CGameClientDlg() 
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	
	m_GameEndTime = 0;

	m_blUsing = false;



	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//״̬����
	m_bMeApplyBanker=false;

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

//��ʼ����
bool CGameClientDlg::OnInitGameEngine()
{
	//���ñ���
	SetWindowText(TEXT("��������Ϸ "));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	//��������
 	VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
 	VERIFY(m_DITDXuanzhuan.Create(TEXT("KCYY")));
 	VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
 	VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
 	VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));
 	VERIFY(m_DTSDCheer[3].Create(TEXT("SPEEKSOUNDWAV")));

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

	CBassMusicManager::GetInstance()->Init(this->m_hWnd);
	CBassMusicManager::GetInstance()->LoadFromFile(0,("BumperCarBattle/bk1.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(1,("BumperCarBattle/bk2.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(2,("BumperCarBattle/bk3.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(3,("BumperCarBattle/bk4.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(4,("BumperCarBattle/bk5.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(5,("BumperCarBattle/bk6.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(6,("BumperCarBattle/bk7.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(7,("BumperCarBattle/bk8.mp3"),true);
	CBassMusicManager::GetInstance()->LoadFromFile(8,("BumperCarBattle/bk9.mp3"),true);
	CBassMusicManager::GetInstance()->SetVolumn(100);
	m_cbMusic = rand()%9;
	//CBassMusicManager::GetInstance()->Play(m_cbMusic,FALSE);
	return true;
}

//���ÿ��
bool CGameClientDlg::OnResetGameEngine()
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

	return true;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
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
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);


		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}
	if(IDI_DISPATCH_CARD == nTimerID)
	{
		m_GameClientView.StartHandle_Leave();
		KillTimer(IDI_DISPATCH_CARD);

		if ( m_GameEndTime <= 0 || m_GameEndTime > 100)
			m_GameEndTime = 2;
		
		SetGameClock(GetMeChairID(),IDI_OPEN_CARD ,m_GameEndTime-1);
		return false;
	}

//	if (IsEnableSound()) 
//	{
		if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
//	}

//	if (IsEnableSound()) 
	{
		if (nTimerID==IDI_PLACE_JETTON)
		{
	
			/*if(nElapse%3==0)
				m_DTSDCheer[3].Play();	*/
		}
	}

	return true;
}

//�Թ���Ϣ
bool CGameClientDlg::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
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
	case SUB_S_CHECK_IMAGE:
		{
			return OnSubCheckImageIndex(pBuffer,wDataSize);
		}
	case SUB_S_ADMIN_COMMDN:		//����
		{
			return OnSubAdminControl(pBuffer,wDataSize);
		}	
	case SUB_S_SCORE_RESULT:
		{
			return OnSubScoreResult(pBuffer,wDataSize);
		}
	case SUB_S_ACCOUNT_RESULT:
		{
			return OnSubAccountResult(pBuffer,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{

	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;

		
		
			m_GameClientView.m_ShowImageIndex = pStatusFree->CheckImage;

	//		CBassMusicManager::GetInstance()->Play(1,false);
            //�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			 IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
		
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_pClientControlDlg)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			//����״̬
			SetGameStatus(GAME_STATUS_FREE);

			//���¿���
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);
			
			//��������

			//PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KCYY"));


			return true;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			m_GameClientView.m_ShowImageIndex = pStatusPlay->CheckImage;

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

				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(NULL);
				m_blUsing = true;
			}

			//��������

			PlayGameSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KCYY"));


			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_pClientControlDlg)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			UINT nTimerID = pStatusPlay->cbGameStatus==GS_GAME_END ? IDI_OPEN_CARD : IDI_PLACE_JETTON;
			SetGameClock(GetMeChairID(), nTimerID, pStatusPlay->cbTimeLeave);

			m_GameClientView.StartRandShowSide();

			//���°�ť
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart( void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_GameClientView.SwithToNormalView();

	m_GameClientView.KillCardTime();

	//CBassMusicManager::GetInstance()->Play(1,false);
	
	m_blUsing = true;

//	PlayMCISound(CGlobal::instance()->GetGameSoundPath(GAME_IMAGE_NAME)+strTemp, _T("gamesound"), false);


	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	m_GameClientView.StartRandShowSide();

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);
	SetTimer(IDI_ANDROID_BET, 100, NULL);

	m_GameClientView.m_bShowBao = true;


	//���¿���
	UpdateButtonContron();

	//������ʾ
	m_GameClientView.SetDispatchCardTip(pGameStart->bContiueCard ? enDispatchCardTip_Continue : enDispatchCardTip_Dispatch);

	
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

	//��������
	//if (IsEnableSound()) 
// 	{
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
// 		
// 	}
	PlayGameSound(AfxGetInstanceHandle(),TEXT("KCYY"));

	if (m_GameClientView.m_pClientControlDlg->GetSafeHwnd())
	{
		m_GameClientView.m_pClientControlDlg->ResetUserBet();
		//m_GameClientView.m_pClientControlDlg->ResetUserNickName();
	}


	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameFree( void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	m_GameClientView.m_btLast.EnableWindow(FALSE);
	m_GameClientView.m_btNext.EnableWindow(FALSE);

	m_blUsing = true;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//m_GameClientView.StartMove();
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
// 	if(m_bAllowBackSound)
// 	{
// 		CBassMusicManager::GetInstance()->Stop(m_cbMusic);
// 		m_cbMusic = (m_cbMusic+1)%9;
// 		CBassMusicManager::GetInstance()->Play(m_cbMusic,false);
// 	}
	//����״̬
	SetGameStatus(GAME_STATUS_FREE);

	//����ʱ��
	KillTimer(IDI_ANDROID_BET);

	////��������
	bool blWin[AREA_COUNT];
	for (int i = 0;i<AREA_COUNT;i++){

			blWin[i]=false;
	}

	m_GameClientView.FinishDispatchCard();

	m_GameClientView.SetWinnerSide(blWin, false);



	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	m_GameClientView.CleanUserJetton();
	
	//���³ɼ�
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		IClientUserItem  *pUserData = GetTableUserItem(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;

		//������Ϣ
		CString strNick = pUserData->GetNickName();
		strNick = strNick.Left(4);
		strNick+=_T("****");

		ApplyUser.lUserScore = pUserData->GetUserScore();
		ApplyUser.strUserName = strNick;
		//m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
		m_GameClientView.UpdateApplyUser(&ApplyUser);
	

	}

	

	/*if(m_GameClientView.m_btApplyBanker.IsWindowVisible())

	{
		if(m_GameClientView.m_ApplyUser.GetItemCount()>=8)
		{
			HINSTANCE hResInstance=AfxGetInstanceHandle();
			m_GameClientView.m_btApplyBanker.SetButtonImage(BT_APPLY_BANKER_FULL,hResInstance,false);
			m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
			if(m_GameClientView.m_btApplyBanker.IsWindowVisible())
				;
			else
				m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);

		}else
		{
			HINSTANCE hResInstance=AfxGetInstanceHandle();
			m_GameClientView.m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false);
			m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);


		}
	}
	else
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);*/



	//��ɷ���




	m_GameClientView.SwitchToCheck();

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�û���ע
bool CGameClientDlg::OnSubPlaceJetton( void * pBuffer, WORD wDataSize,bool bGameMes)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	if (pPlaceJetton->cbAndroid==FALSE && (m_GameClientView.m_pClientControlDlg->GetSafeHwnd())&&bGameMes)
	{
		m_GameClientView.m_pClientControlDlg->SetAllUserBetScore(pPlaceJetton->cbJettonArea - 1,pPlaceJetton->lJettonScore);
	}


	if (GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
	{
		//��ע����
		if(pPlaceJetton->cbAndroid == TRUE)
		{
			//����
			static WORD wStFluc = 1;	//�������
			tagAndroidBet androidBet = {};
			androidBet.cbJettonArea = pPlaceJetton->cbJettonArea;
			androidBet.lJettonScore = pPlaceJetton->lJettonScore;
			androidBet.wChairID = pPlaceJetton->wChairID;
			androidBet.nLeftTime = ((rand()+androidBet.wChairID+wStFluc*3)%10+1)*100;
			wStFluc = wStFluc%3 + 1;

			m_ListAndroid.AddTail(androidBet);
		}
		else
		{
			m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

			//��������
		//	if (IsEnableSound()) 
		//	{
				if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
				{
				//	if (pPlaceJetton->lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
				//	else 
					PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));

					/*if(rand()%100 > 80 )
					{
						m_DTSDCheer[rand()%3].Play();
					}	*/
				}
		//	}
		}
	}
	UpdateButtonContron();
    
	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd( void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_GameClientView.m_blMoveFinish = false;
	CBassMusicManager::GetInstance()->Stop(m_cbMusic);

	m_GameClientView.m_btLast.EnableWindow(FALSE);
	m_GameClientView.m_btNext.EnableWindow(FALSE);

	m_GameEndTime = pGameEnd->cbTimeLeave;

	m_blUsing = true;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(pGameEnd->cbTableCardArray);
	m_GameClientView.ClearAreaFlash();

	//�O�Ó���Ƅ�
	m_GameClientView.StartRunCar(20);

	//����״̬
	SetGameStatus(GS_GAME_END);

	m_cbLeftCardCount=pGameEnd->cbLeftCardCount;

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);
	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	m_GameClientView.PerformAllBetAnimation();

	m_TempData.a = pGameEnd->lUserScore;
	m_TempData.b = pGameEnd->lUserReturnScore;
	m_TempData.c = pGameEnd->lBankerScore;
	m_TempData.d = pGameEnd->lRevenue;
	
	//���¿ؼ�
	UpdateButtonContron();

	//ֹͣ����
	for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientDlg::UpdateButtonContron()
{
	/*if(__TEST__)
	{
		return ;
	}*/
	//�����ж�
	bool bEnablePlaceJetton=true;

	if(m_wCurrentBanker==INVALID_CHAIR)
	{
		bEnablePlaceJetton = true;

	}
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		bEnablePlaceJetton=false;

	}
	if (m_wCurrentBanker==GetMeChairID()) 
	{
		bEnablePlaceJetton=false;
	}
	if (IsLookonMode())
	{
		bEnablePlaceJetton=false;
	}
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) 
	{
		bEnablePlaceJetton=false;
	}

	m_GameClientView.SetEnablePlaceJetton(bEnablePlaceJetton);

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		
		//�������
		SCORE lCurrentJetton=m_GameClientView.GetCurrentJetton();
		SCORE lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		//�����ע
		SCORE lUserMaxJetton = 0;

		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
		{
			if(lUserMaxJetton==0&&nAreaIndex == 1)
			{
				lUserMaxJetton = m_GameClientView.GetUserMaxJetton(nAreaIndex);

			}else
			{
				if(m_GameClientView.GetUserMaxJetton(nAreaIndex)>lUserMaxJetton)
				{
					lUserMaxJetton = m_GameClientView.GetUserMaxJetton(nAreaIndex);
				}
			}
		}
		lLeaveScore = min((lLeaveScore),lUserMaxJetton); //�û����·� �����ֱȽ� �������屶 

		CString strLog;
		strLog.Format(L"lLeaveScore = %0.2lf");
		OutputDebugString(strLog);
		//���ù��
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=1000L) m_GameClientView.SetCurrentJetton(1000L);
			else if (lLeaveScore>=500L) m_GameClientView.SetCurrentJetton(500L);
			else if (lLeaveScore>=100L) m_GameClientView.SetCurrentJetton(100L);
			else if (lLeaveScore>=10L) m_GameClientView.SetCurrentJetton(10L);
			else if (lLeaveScore>=5L) m_GameClientView.SetCurrentJetton(5L);
			else if (lLeaveScore>=1L) m_GameClientView.SetCurrentJetton(1L);
			else m_GameClientView.SetCurrentJetton(0L);
		}

		//���ư�ť
		int iTimer = 1;

		if(m_blUsing==false)
		{
			lLeaveScore = 0;
			lUserMaxJetton = 0;

		}

		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=1*iTimer && lUserMaxJetton>=1*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=5*iTimer && lUserMaxJetton>=5*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10*iTimer && lUserMaxJetton>=10*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100*iTimer && lUserMaxJetton>=100*iTimer)?TRUE:FALSE);
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
		//m_GameClientView.m_btJetton50000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);
	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
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
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			
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
	//��ȡ��Ϣ
	 IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	ReSetBankCtrol(GetGameStatus());
	return;
}
bool CGameClientDlg::OnSubCheckImageIndex( void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_C_CheckImage));
	if (wDataSize!=sizeof(CMD_C_CheckImage)) return false;

	CMD_C_CheckImage * pCheckImage=(CMD_C_CheckImage *)pBuffer;
	m_GameClientView.m_ShowImageIndex = pCheckImage->Index;

	return true;


}
//��ע��Ϣ
LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=(LONGLONG)(*((LONGLONG*)lParam));

	//�Ϸ��ж�
	ASSERT(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT);
	if (!(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT)) return 0;

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
	//if (IsEnableSound()) 
	{
	/*	if (lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));*/
	//	else 
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		if(rand()%100 > 80 )
		{
			m_DTSDCheer[rand()%3].Play();
		}	
	}

	return 0;

}
LRESULT  CGameClientDlg::OnPlaySound(WPARAM wParam, LPARAM lParam)
{
	//if (IsEnableSound()) 
	{
		if(lParam == 0)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("PLEASEJETTONWAV"));
		}
		else if(lParam==1)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("LEAVEHANDLEWAV"));
		}
		else if(lParam==3)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("IDC_SNDWAV"));
		}
		else if(lParam==4)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
		}
		else if(lParam==5)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
		}
		else if(lParam==6)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
		}
		else if(lParam==7)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		}
	}
	

	return 1;
		
}

LRESULT  CGameClientDlg::OnShowResult(WPARAM wParam, LPARAM lParam)
{

	TCHAR szBuffer[128] = TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT(" ���ַ��� %I64d ���ط��� %I64d ׯ�ҷ��� %I64d "),m_TempData.a,m_TempData.b,m_TempData.c);
//	m_pIStringMessage->InsertCustomString(szBuffer,RGB(255,0,255));

	return 1;

}
//�ֹ�����
LRESULT  CGameClientDlg::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
	//	m_pIStringMessage->InsertCustomString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"),RGB(255,0,255));
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = false;
	//m_pIStringMessage->InsertCustomString(TEXT("��ϵͳ��ʾ����ѡ�����ֹ�����ģʽ�����ƺ�������ʹ������϶��˿ˣ�"),RGB(255,0,255));
	//m_GameClientView.m_btOpenCard.EnableWindow(false);
	//m_GameClientView.m_btAutoOpenCard.EnableWindow(true);
	return 1;
}
//�Զ�����
LRESULT  CGameClientDlg::OnAutoOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
	//	m_pIStringMessage->InsertCustomString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"),RGB(255,0,255));
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = true;
//	m_pIStringMessage->InsertCustomString(TEXT("��ϵͳ��ʾ����ѡ�����Զ�����ģʽ�����ƺ�ϵͳ���Զ��������ҵ��ƣ�"),RGB(255,0,255));
	//m_GameClientView.m_btOpenCard.EnableWindow(true);
	//m_GameClientView.m_btAutoOpenCard.EnableWindow(false);
	return 1;
}
//��������
LRESULT CGameClientDlg::OnContinueCard(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	if (GetMeChairID()!=m_wCurrentBanker) return 0;
	if (GetGameStatus()!=GAME_STATUS_FREE) return 0;
	if (m_cbLeftCardCount < 8) return 0;
	if (IsLookonMode()) return 0;

	//������Ϣ
	//SendSocketData(SUB_C_CONTINUE_CARD);
    CMD_C_CheckImage CheckImage;
    CheckImage.Index =   m_GameClientView.m_CheckImagIndex;
	SendSocketData(SUB_C_CHECK_IMAGE,&CheckImage,sizeof(CheckImage));
	//���ð�ť

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	IClientUserItem  *pMeUserData = GetTableUserItem( GetMeChairID() );
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

		//m_bMeApplyBanker=true;
	}
	else
	{
		//������Ϣ
		SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);

		//m_bMeApplyBanker=false;
	}

	//���ð�ť
	UpdateButtonContron();

	return true;
}

//������ׯ
bool CGameClientDlg::OnSubUserApplyBanker( void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//��ȡ���
	IClientUserItem  *pUserData=GetTableUserItem(pApplyBanker->wApplyUser);

	//�������
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser)
	{
		tagApplyUser* ApplyUser = new tagApplyUser;
		CString strNick = pUserData->GetNickName();
		strNick = strNick.Left(4);
		strNick+=_T("****");

		ApplyUser->strUserName=strNick;
		ApplyUser->lUserScore=pUserData->GetUserScore();
		m_GameClientView.InsertApplyUser(ApplyUser);  // m_ApplyUser.GetCount();
	//	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	}

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//���¿ؼ�
	UpdateButtonContron();


	/*if(m_GameClientView.m_btApplyBanker.IsWindowVisible())

	{
		if(m_GameClientView.m_ApplyUser.GetItemCount()>=8)
		{
			HINSTANCE hResInstance=AfxGetInstanceHandle();
			m_GameClientView.m_btApplyBanker.SetButtonImage(BT_APPLY_BANKER_FULL,hResInstance,false);
			m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
			if(m_GameClientView.m_btApplyBanker.IsWindowVisible())
				;
			else
				m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);

		}else
		{
			HINSTANCE hResInstance=AfxGetInstanceHandle();
			m_GameClientView.m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false);
			m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);


		}
	}
	else
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);*/


	return true;
}

//ȡ����ׯ
bool CGameClientDlg::OnSubUserCancelBanker( void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	

	/*if(m_GameClientView.m_ApplyUser.GetItemCount()>4)
	{
		m_GameClientView.m_btUp.ShowWindow(SW_SHOW);
		m_GameClientView.m_btUp.EnableWindow(true);
		m_GameClientView.m_btDown.ShowWindow(SW_SHOW);
		m_GameClientView.m_btDown.EnableWindow(true);  

	}else
	{
		m_GameClientView.m_btUp.ShowWindow(SW_HIDE);
		m_GameClientView.m_btUp.EnableWindow(true);
		m_GameClientView.m_btDown.ShowWindow(SW_HIDE);
		m_GameClientView.m_btDown.EnableWindow(true); 

	}*/
	tagApplyUser ApplyUser;

	CString strNick;
	strNick.Format(L"%s",pCancelBanker->szCancelUser);
	strNick = strNick.Left(4);
	strNick+=L"****";
	ApplyUser.strUserName=strNick;
	ApplyUser.lUserScore=0;
	m_GameClientView.RemoveApplyUser(&ApplyUser);
	//m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
	//m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);

	//�Լ��ж�
	 IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	if (IsLookonMode()==false && lstrcmp(pMeUserData->GetNickName(),pCancelBanker->szCancelUser)==0) m_bMeApplyBanker=false;

	
	//���¿ؼ�
	UpdateButtonContron();

	//ɾ�����
	

	
	/*if(m_GameClientView.m_btApplyBanker.IsWindowVisible())

	{
		if(m_GameClientView.m_ApplyUser.GetItemCount()>=8)
		{
			HINSTANCE hResInstance=AfxGetInstanceHandle();
			m_GameClientView.m_btApplyBanker.SetButtonImage(BT_APPLY_BANKER_FULL,hResInstance,false);
			m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
			if(m_GameClientView.m_btApplyBanker.IsWindowVisible())
				;
			else
				m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);

		}else
		{
			HINSTANCE hResInstance=AfxGetInstanceHandle();
			m_GameClientView.m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false);
			m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);


		}
	}
	else
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);*/


	return true;
}

//�л�ׯ��
bool CGameClientDlg::OnSubChangeBanker( void * pBuffer, WORD wDataSize)
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
		IClientUserItem  *pBankerUserData=GetTableUserItem(m_wCurrentBanker);
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			ApplyUser.strUserName = pBankerUserData->GetNickName();
			m_GameClientView.RemoveApplyUser(&ApplyUser);
	/*		m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
			m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);*/
		}
	}

	//���½���
	UpdateButtonContron();
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��Ϸ��¼
bool CGameClientDlg::OnSubGameRecord( void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(pServerGameRecord->bWinMen);
	}

	return true;
}

//��עʧ��
bool CGameClientDlg::OnSubPlaceJettonFail( void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;

	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

	//Ч�����
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
	ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
	if (!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT)) return false;

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

	//�Լ��ж�
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		SCORE lJettonCount=pPlaceJettonFail->lPlaceScore;
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
void CGameClientDlg::SetBankerInfo(WORD wBanker,SCORE lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	IClientUserItem  *pUserData=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL==pUserData) ? 0 : pUserData->GetUserID();
	m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);

	//if (IsLookonMode()==false && GetMeChairID()==m_wCurrentBanker) m_bMeApplyBanker=true;
	//else m_bMeApplyBanker = false;
	//UpdateButtonContron();

}

//������ע
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount)
{
	//�Ϸ��ж�
	ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
	if (!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT)) return;

	//���ñ���
	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
//�л�����
LRESULT CGameClientDlg::OnChangeMusic( WPARAM wParam, LPARAM lParam )
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
//////////////////////////////////////////////////////////////////////////
//����
LRESULT CGameClientDlg::OnAdminControl( WPARAM wParam, LPARAM lParam )
{
	SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));
	return true;
}

LRESULT CGameClientDlg::OnGetAccount(WPARAM wParam, LPARAM lParam)
{
	SendSocketData(SUB_C_GET_ACCOUNT);
	return true;
}

LRESULT CGameClientDlg::OnCheckAccount(WPARAM wParam, LPARAM lParam)
{
	CString strNickName;
	m_GameClientView.m_pClientControlDlg->GetCheckNickName(strNickName);
	if(strNickName.IsEmpty()) return 0;

	CMD_C_CheakAccount CheakAccount;
	TRACE("szNickName=%s\n",strNickName);
	CopyMemory(CheakAccount.szUserAccount,strNickName.GetBuffer(),CountArray(CheakAccount.szUserAccount));
	strNickName.ReleaseBuffer();
	SendSocketData(SUB_C_CHECK_ACCOUNT,&CheakAccount,sizeof(CMD_C_CheakAccount));
	return true;
}

//����
bool CGameClientDlg::OnSubAdminControl(  void * pBuffer, WORD wDataSize )
{
	ASSERT(wDataSize==sizeof(CMD_S_ControlReturns));
	if(wDataSize!=sizeof(CMD_S_ControlReturns)) return false;

	if( NULL != m_GameClientView.m_pClientControlDlg && NULL != m_GameClientView.m_pClientControlDlg->GetSafeHwnd() )
	{
		CMD_S_ControlReturns* pResult = (CMD_S_ControlReturns*)pBuffer;
		m_GameClientView.m_pClientControlDlg->UpdateControl(pResult);
	}
	return true;
}

bool CGameClientDlg::OnSubScoreResult(void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_ScoreResult));
	if(wDataSize!=sizeof(CMD_S_ScoreResult)) return false;

	CMD_S_ScoreResult * pResult=(CMD_S_ScoreResult*)pBuffer;

	if (m_GameClientView.m_pClientControlDlg!=NULL && m_GameClientView.m_pClientControlDlg->GetSafeHwnd())
	{
		for(int i=0; i<=AREA_COUNT; i++) m_GameClientView.m_pClientControlDlg->SetUserBetScore(i,pResult->lUserJettonScore[i]);
	}

	return true;
}

bool CGameClientDlg::OnSubAccountResult( void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_AccountResult));
	if(wDataSize!=sizeof(CMD_S_AccountResult)) return false;

	CMD_S_AccountResult * pResult=(CMD_S_AccountResult*)pBuffer;

	if (m_GameClientView.m_pClientControlDlg!=NULL && m_GameClientView.m_pClientControlDlg->GetSafeHwnd())
	{
		m_GameClientView.m_pClientControlDlg->ResetUserNickName();
		//m_GameClientView.m_pClientControlDlg->ResetUserBet();

		for(int i=0; i<MAX_CHAIR; i++) 
		{
			if(pResult->szAccount[i][0]!=TEXT('\0')) m_GameClientView.m_pClientControlDlg->SetUserNickName(pResult->szAccount[i]);
		}
	}

	return true;
}




void CGameClientDlg::OnTimer(UINT nIDEvent)
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
					placeJetton.cbAndroid = FALSE;
					placeJetton.cbJettonArea = androidBet.cbJettonArea;
					placeJetton.lJettonScore = androidBet.lJettonScore;
					placeJetton.wChairID = androidBet.wChairID;

					OnSubPlaceJetton((void*)&placeJetton, sizeof(placeJetton),false);

					//ɾ��Ԫ��
					m_ListAndroid.RemoveAt(posTmp);
				}
			}
		}
	}
	__super::OnTimer(nIDEvent);
}


void CGameClientDlg::ReSetBankCtrol(int nGameState)
{

	if(!IsLookonMode())
	{
		switch (GetGameStatus())
		{
		case GAME_STATUS_FREE:
			{
				m_GameClientView.m_winflag = false;
				m_GameClientView.flagrrc = false;
				m_GameClientView.qisx = 0; 
			m_GameClientView.chsxx = 0;
			m_GameClientView.chsxy = 0;
				if(GetMeChairID()!=m_wCurrentBanker)
				{
					if(m_GameClientView.m_DlgBank.m_hWnd!=NULL)
					{
						m_GameClientView.m_DlgBank.AllowStorage(TRUE);
						m_GameClientView.m_DlgBank.AllowDraw(TRUE);
					}

// 					m_GameClientView.m_btBankDraw.EnableWindow(FALSE);
// 					m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}
				else
				{
					if(m_GameClientView.m_DlgBank.m_hWnd!=NULL)
					{
						m_GameClientView.m_DlgBank.AllowStorage(FALSE);
						m_GameClientView.m_DlgBank.AllowDraw(TRUE);
					}

// 					m_GameClientView.m_btBankDraw.EnableWindow(FALSE);
// 					m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}
				break;
			}
		case GS_GAME_END:
		case GS_PLACE_JETTON:
		case GS_MOVECARD_END:
			{
				if(GetMeChairID()!=m_wCurrentBanker)
				{
					if(m_GameClientView.m_DlgBank.m_hWnd)
					{
						m_GameClientView.m_DlgBank.AllowStorage(FALSE);
						m_GameClientView.m_DlgBank.AllowDraw(TRUE);
					}
				}
				else
				{
					if(m_GameClientView.m_DlgBank.m_hWnd)
					{
						m_GameClientView.m_DlgBank.AllowStorage(FALSE);
						m_GameClientView.m_DlgBank.AllowDraw(TRUE);
					}
				}
// 				m_GameClientView.m_btBankDraw.EnableWindow(FALSE);
// 				m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				break;
			}
		default:break;
		}
	}
// 	else
// 	{
// 		m_GameClientView.m_btBankDraw.EnableWindow(FALSE);
// 		m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
// 	}

}


bool CGameClientDlg::OnEventGameClockKill( WORD wChairID )
{
	return true;
}

bool CGameClientDlg::AllowBackGroundSound( bool bAllowSound )
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