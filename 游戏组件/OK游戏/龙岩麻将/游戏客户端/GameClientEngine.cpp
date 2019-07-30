#include "Stdafx.h"
#include "GameClient.h"
////#include "GameOption.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			201									//������ʱ��
#define IDI_REPLACE_CARD			301									//���ƶ�ʱ��

//��Ϸ��ʱ��
#define TIME_START_GAME				30									//��ʼ��ʱ��
#define TIME_OPERATE_CARD			20									//������ʱ��		
#define TIME_REPLACE_CARD			1000								//���ƶ�ʱ��

#define MAX_TIME_OUT				3									//���ʱ����

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_SICE_TWO,OnSiceTwo)
	ON_MESSAGE(IDM_USER_ACTION,OnUserAction)
	ON_MESSAGE(IDM_USER_ACTION_FINISH,OnUserActionFinish)
	ON_MESSAGE(IDM_OPEN_GOLD,OnOpenGold)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine() : CGameFrameEngine()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbListenStatus = 0;
	m_bWillHearStatus=false;
	m_lSiceCount = 0;

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory( m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{


	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

//////////////
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);


	//////������Դ
	////g_CardResource.LoadResource();

	//��������
	m_DirectSound[0].Create( TEXT("OUT_CARD") );
	m_DirectSound[1].Create( TEXT("SEND_CARD") );
	m_DirectSound[2].Create( TEXT("GAME_START") );


	//test
	//BYTE cbCardData[] = {
	//	0x13,0x12,0x16,0x15,0x22,0x23,0x25,0x26,0x05,
	//	0x03,0x03,0x03,0x03
	//};
	//BYTE cbCardCount = 13;
	//BYTE cbCurrentCard = 0x06;
	//
	//m_GameLogic.SetMagicIndex( m_GameLogic.SwitchToCardIndex(0x03) );

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

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbListenStatus = 0;
	m_bWillHearStatus=false;
	m_lSiceCount = 0;
	
	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	return true;
}

//////��Ϸ����
////void CGameClientEngine::OnGameOptionSet()
////{
////	//��������
////	CGameOption GameOption;
////	GameOption.m_bEnableSound=IsEnableSound();
////	GameOption.m_bAllowLookon = IsAllowUserLookon();
////	GameOption.m_bEanbleAnimate = m_GameClientView.IsEnableAnimate();
////	
////	//��������
////	if (GameOption.DoModal()==IDOK)
////	{
////		EnableSound(GameOption.m_bEnableSound);
////		AllowUserLookon(0,GameOption.m_bAllowLookon);
////		m_GameClientView.EnableAnimate(GameOption.m_bEanbleAnimate);
////		for( BYTE i = 0; i < CountArray(m_DirectSound); i++ )
////			m_DirectSound[i].EnableSound(GameOption.m_bEnableSound);
////	}
////
////	return;
////}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if( m_bStustee && nElapse < TIME_START_GAME )
			{
				OnStart(0,0);
				return true;
			}
			if (nElapse==0)
			{
				////PostMessage(WM_CLOSE);
				AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
				return true;
			}
			if (nElapse<=5)
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//�Զ�����
			bool bAutoOutCard=false;
			if ((bAutoOutCard==true)&&(m_GameClientView.m_ControlWnd.IsWindowVisible()))
				bAutoOutCard=false;
			if((bAutoOutCard==false)&&(m_bStustee==true))
			{
				bAutoOutCard=true;
			}

			//��ʱ�ж�
			if ( (IsLookonMode()==false)&&
				( nElapse==0 || bAutoOutCard || (m_cbListenStatus&&!m_bWillHearStatus&&nElapse<TIME_OPERATE_CARD) ) )
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if (wChairID==wMeChairID)
				{
					//���δ����,��δ�й�,���ۼӳ�ʱ����
					if( m_cbListenStatus == 0 && m_bStustee==false && ++m_wTimeOutCount>=MAX_TIME_OUT )
					{
						m_wTimeOutCount = 0;
						OnStusteeControl(TRUE,0);
						m_pIStringMessage->InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
					}

					if (m_wCurrentUser==wMeChairID)
					{
						//��ȡ�˿�
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();

						//����״̬
						if( m_cbListenStatus >= 2 )
						{
							if( m_cbActionMask & WIK_CHI_HU )
							{
								OnUserAction( WIK_CHI_HU,0 );
								return true;
							}
						}
						//����ڲ���ģʽ��
						if( m_GameClientView.m_HandCardControl.GetSelectMode() )
						{
							OnCardOperate(0,0);
							return true;
						}
						//����Ч��
						if (VerdictOutCard(cbCardData)==false)
						{
							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								//����Ч��
								if (m_cbCardIndex[i]==0) continue;
								if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false) 
									continue;

								//���ñ���
								cbCardData=m_GameLogic.SwitchToCardData(i);
							}
						}

						//���ƶ���
						ASSERT( m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData)] > 0 );
						OnOutCard(cbCardData,cbCardData);
					}
					else 
					{
						//����״̬,���ڻ������͵���
						if( m_cbListenStatus == 1 || m_cbListenStatus == 3 )
						{
							if( m_cbActionMask & WIK_CHI_HU )
							{
								OnUserAction( WIK_CHI_HU,0 );
								return true;
							}
						}
						//����ڲ���ģʽ��
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

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}

	return true;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(void * pBuffer, WORD wDataSize)
{
	//�˿˿���
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			return OnSubOperateResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return OnSubTrustee(pBuffer,wDataSize);
		}
	case SUB_S_LISTEN:			//�������
		{
			return OnSubListen( pBuffer,wDataSize );
		}
	case SUB_S_REPLACE_CARD:	//����
		{
			return OnSubReplaceCard( pBuffer,wDataSize );
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//////�Ƿ���������
			////if( !IsEnableSound() )
			////{
			////	for( BYTE i = 0; i < CountArray(m_DirectSound); i++ )
			////		m_DirectSound[i].EnableSound(FALSE);
			////}

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
			}

			//���ÿؼ�
			////if (IsLookonMode()==false)
			if (IsLookonMode()==false && GetMeUserItem()->GetUserStatus()!=US_READY)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}

			//����Ч��
			m_GameClientView.SetDiscUser(INVALID_CHAIR);

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//////�Ƿ���������
			////if( !IsEnableSound() )
			////{
			////	for( BYTE i = 0; i < CountArray(m_DirectSound); i++ )
			////		m_DirectSound[i].EnableSound(FALSE);
			////}

			//��������
			WORD wMeChairID=GetMeChairID();
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;
			m_cbListenStatus = pStatusPlay->cbHearStatus[wMeChairID];
			m_wReplaceUser = pStatusPlay->wReplaceUser;
			m_wHeapHead = pStatusPlay->wHeapHead;
			m_wHeapTail = pStatusPlay->wHeapTail;

			//�û�����
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				IClientUserItem * pUserData=GetTableUserItem(i);
				ASSERT( pUserData != NULL );
				if( pUserData == NULL ) 
				{
					m_szAccounts[i][0] = 0;
					continue;
				}
				lstrcpy( m_szAccounts[i],pUserData->GetNickName() );
			}

			//�Թ�
			if( IsLookonMode()==true )
				m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());

			//�й�����
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
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false, false);
			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false, false);

			//�˿˱���
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//����˿�
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

			//�û��˿�
			if (m_wCurrentUser==wMeChairID&&m_wReplaceUser==INVALID_CHAIR&&
				m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pStatusPlay->cbSendCardData)] > 0 )
			{
				SetHandCardControl(m_cbCardIndex,pStatusPlay->cbSendCardData);
			}
			else
			{
				SetHandCardControl(m_cbCardIndex,0x00);
			}

			//�˿�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�û��˿�
				if (i!=wMeChairID)
				{
					BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[i]*3-1;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//�����˿�
				m_GameClientView.m_DiscardCard[wViewChairID[i]].SetCardData(pStatusPlay->cbDiscardCard[i],pStatusPlay->cbDiscardCount[i]);
			}

			//����Ч��
			if(pStatusPlay->wOutCardUser != INVALID_CHAIR)
			{
				m_GameClientView.SetDiscUser(SwitchViewChairID(pStatusPlay->wOutCardUser));
				m_GameClientView.SetOutCardInfo( wViewChairID[pStatusPlay->wOutCardUser],pStatusPlay->cbOutCardData );
				m_GameClientView.m_DiscardCard[wViewChairID[pStatusPlay->wOutCardUser]].AddCardItem( pStatusPlay->cbOutCardData );
			}

			//��������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			}

			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_HeapCard[wViewChairID[i]].SetHeapCardInfo(pStatusPlay->cbHeapCardInfo[i][0],pStatusPlay->cbHeapCardInfo[i][1]);
			}

			//����״̬
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( pStatusPlay->cbHearStatus[i] > 0 )
					m_GameClientView.SetUserListenStatus( wViewChairID[i],true );
			}

			//��������
			if ((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
			{
				//��ȡ����
				m_cbActionMask=pStatusPlay->cbActionMask;
				m_cbActionCard=pStatusPlay->cbActionCard;

				//���ý���
				if (m_wCurrentUser==INVALID_CHAIR)
					SetGameClock(wMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				if (IsLookonMode()==false) 
					m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
			}

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR)
			{
				//����ʱ��
				WORD wTimeCount=TIME_OPERATE_CARD;

				//����ʱ��
				SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
			}
			else if( m_wReplaceUser == wMeChairID && !IsLookonMode() )
			{
				tagActionReplaceCard *pActionReplaceCard = (tagActionReplaceCard *)ActiveUserAction(AK_ReplaceCard);

				pActionReplaceCard->wReplaceUser = m_wReplaceUser;

				m_UserActionArray.Add(pActionReplaceCard);

				BeginUserAction();
			}

			//����Ȧ��
			m_GameClientView.SetFengQuan( pStatusPlay->cbQuanFeng );

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);

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
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	if( IsLookonMode() )
		StopUserAction();

	//�û�����
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		IClientUserItem * pUserData=GetTableUserItem(i);
		ASSERT( pUserData != NULL );
		if( pUserData == NULL ) 
		{
			m_szAccounts[i][0] = 0;
			continue;
		}
		lstrcpy( m_szAccounts[i],pUserData->GetNickName() );
	}

	//����״̬
	SetGameStatus(GS_MJ_PLAY);

	//���ñ���
	m_wBankerUser = pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	m_cbActionMask = pGameStart->cbUserAction;
	m_lSiceCount = pGameStart->lSiceCount;

	//�����˿�
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//���ö���ͷβ
	m_wHeapHead = pGameStart->wHeapHead;
	m_wHeapTail = pGameStart->wHeapTail;
	CopyMemory( m_cbHeapCardInfo,pGameStart->cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );

	//�Թ۽���
	if (IsLookonMode()==true)
	{
		//���ñ���
		m_cbListenStatus = 0;
		m_bWillHearStatus = false;
		m_wReplaceUser = INVALID_CHAIR;

		//����˿�
		ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
		ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

		m_GameClientView.m_GameScore.RestorationData();
		m_GameClientView.SetCurrentUser(INVALID_CHAIR);

		//���ý���
		m_GameClientView.SetDiscUser(INVALID_CHAIR);
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetBankerUser(INVALID_CHAIR);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);

		//�˿�����
		m_GameClientView.m_UserCard[0].SetCardData(0,false);
		m_GameClientView.m_UserCard[1].SetCardData(0,false);
		m_GameClientView.m_UserCard[2].SetCardData(0,false);
		m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

		//�˿�����
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
			for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
				m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
		}

		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[i].ResetCardData();
			m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
		}
	}

	//���ý���
	m_GameClientView.SetFengQuan( pGameStart->cbQuanFeng );
	if( WORD(pGameStart->lSiceCount>>16) == 0 )
		m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//���ſ�ʼ����
	m_DirectSound[2].Play();

	if( pGameStart->lSiceCount != 0 )
	{
		//ҡɫ�Ӷ���
		tagActionSice *pActionSice = (tagActionSice *)ActiveUserAction(AK_Sice);
		ASSERT(pActionSice);
		if( !pActionSice ) return false;
		pActionSice->lSiceCount = pGameStart->lSiceCount;

		m_UserActionArray.Add(pActionSice);
	}

	//���ƶ���
	WORD wMeChairID = GetMeChairID();
	BYTE cbHandCardData[MAX_COUNT];
	m_GameLogic.SwitchToCardData( m_cbCardIndex,cbHandCardData );
	//�����˿�
	m_GameLogic.RandCardData(cbHandCardData,(m_wBankerUser==wMeChairID)?MAX_COUNT:MAX_COUNT-1,cbHandCardData);
	//ȷ������ɾ��λ��
	WORD wHeapId = m_wHeapTail;
	WORD wHeapCardIndex = m_cbHeapCardInfo[m_wHeapTail][1]-1;
	//��4Ȧ�����1Ȧ��һ��
	for( BYTE i = 0; i < 4; i++ )
	{
		//��ׯ�ҿ�ʼ
		WORD wId = m_wBankerUser;
		for( WORD j = 0; j < GAME_PLAYER; j++ )
		{
			WORD wChairId = (wId+GAME_PLAYER-j)%GAME_PLAYER;

			//����������Ŀ
			tagActionStartCard *pStartCardItem = (tagActionStartCard *)ActiveUserAction(AK_StartCard);
			ASSERT( pStartCardItem );
			if( !pStartCardItem ) return false;

			//������
			pStartCardItem->cbCardCount = i<3?4:1;
			//����ֵ
			if( wChairId == wMeChairID )
				CopyMemory( pStartCardItem->cbCardData,&cbHandCardData[i*4],sizeof(BYTE)*(i<3?4:1) );
			//�������
			pStartCardItem->wChairId = wChairId;
			//����λ��
			pStartCardItem->wHeapId = wHeapId;
			//����λ��
			pStartCardItem->wHeapCardIndex = wHeapCardIndex;

			pStartCardItem->bLastItem = false;
			//���붯������
			m_UserActionArray.Add(pStartCardItem);

			//�ƽ�
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
	//���һ�ţ�����ׯ��
	tagActionStartCard *pStartCardItem = (tagActionStartCard *)ActiveUserAction(AK_StartCard);
	ASSERT( pStartCardItem );
	if( !pStartCardItem ) return false;
	//������
	pStartCardItem->cbCardCount = 1;
	//����ֵ
	if( m_wBankerUser == wMeChairID )
		pStartCardItem->cbCardData[0] = cbHandCardData[MAX_COUNT-1];
	//�������
	pStartCardItem->wChairId = m_wBankerUser;
	//����λ��
	pStartCardItem->wHeapId = wHeapId;
	//����λ��
	pStartCardItem->wHeapCardIndex = wHeapCardIndex;
	//���һ��
	pStartCardItem->bLastItem = true;
	//���붯������
	m_UserActionArray.Add(pStartCardItem);

	//��������
	if( !IsLookonMode() && pGameStart->wReplaceUser == GetMeChairID() )
	{
		tagActionReplaceCard *pActionReplaceCard = (tagActionReplaceCard *)ActiveUserAction(AK_ReplaceCard);
		ASSERT(pActionReplaceCard);
		if( !pActionReplaceCard ) return false;

		pActionReplaceCard->wReplaceUser = pGameStart->wReplaceUser;

		m_UserActionArray.Add(pActionReplaceCard);
	}

	//��ʼ����
	BeginUserAction();

	return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	if( IsLookonMode() || pOutCard->wOutCardUser != GetMeChairID() )
	{
		tagActionOutCard *pActionOutCard = (tagActionOutCard *)ActiveUserAction(AK_OutCard);
		ASSERT(pActionOutCard);
		if( !pActionOutCard ) return false;

		pActionOutCard->cbOutCardData = pOutCard->cbOutCardData;
		pActionOutCard->wOutCardUser = pOutCard->wOutCardUser;

		//��Ӷ���
		m_UserActionArray.Add(pActionOutCard);

		//��ʼ����
		BeginUserAction();
	}

	return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��Ϣ����
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	tagActionSendCard *pActionSendCard = (tagActionSendCard *)ActiveUserAction(AK_SendCard);
	ASSERT(pActionSendCard);
	if( !pActionSendCard ) return false;

	pActionSendCard->bTail = pSendCard->bTail;
	pActionSendCard->cbActionMask = pSendCard->cbActionMask;
	pActionSendCard->cbCardData = pSendCard->cbCardData;
	pActionSendCard->wCurrentUser = pSendCard->wCurrentUser;
	pActionSendCard->wSendCardUser = pSendCard->wSendCardUser;

	//��Ӷ���
	m_UserActionArray.Add(pActionSendCard);

	if( !IsLookonMode() && pSendCard->wReplaceUser == GetMeChairID() )
	{
		tagActionReplaceCard *pActionReplaceCard = (tagActionReplaceCard *)ActiveUserAction(AK_ReplaceCard);
		ASSERT(pActionReplaceCard);
		if( !pActionReplaceCard ) return false;

		pActionReplaceCard->wReplaceUser = pSendCard->wReplaceUser;

		m_UserActionArray.Add(pActionReplaceCard);
	}

	//��ʼ����
	BeginUserAction();

	return true;
}

//������Ϣ
bool CGameClientEngine::OnSubReplaceCard(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_ReplaceCard));
	if (wDataSize!=sizeof(CMD_S_ReplaceCard)) return false;

	//��������
	CMD_S_ReplaceCard * pReplaceCard=(CMD_S_ReplaceCard *)pBuffer;

	if( IsLookonMode() || pReplaceCard->wReplaceUser != GetMeChairID() )
	{
		tagActionReplaceCard *pActionReplaceCard = (tagActionReplaceCard *)ActiveUserAction(AK_ReplaceCard);
		ASSERT(pActionReplaceCard);
		if( !pActionReplaceCard ) return false;

		pActionReplaceCard->cbReplaceCard = pReplaceCard->cbReplaceCard;
		pActionReplaceCard->wReplaceUser = pReplaceCard->wReplaceUser;

		m_UserActionArray.Add(pActionReplaceCard);

		BeginUserAction();
	}

	return true;
}

// ����״̬
bool CGameClientEngine::OnSubOpenGold(const void * pBuffer, WORD wDataSize)
{
	////Ч������
	//ASSERT(wDataSize==sizeof(QZCMD_S_OpenGold));
	//if(wDataSize!=sizeof(QZCMD_S_OpenGold))return false;

	////ת������
	//QZCMD_S_OpenGold *pOpenGold = (QZCMD_S_OpenGold *)pBuffer;

	////���ñ���
	//CopyMemory(m_cbGoldCard,pOpenGold->cbGoldCard,sizeof(m_cbGoldCard));
	//CopyMemory(m_cbFlowerCard,pOpenGold->cbFlowerCard,sizeof(m_cbFlowerCard));
	//m_wGoldDiceCount = pOpenGold->wDiceCount;
	//m_wHeapHand = m_wBankerUser;
	//m_wHeapTail = m_wBankerUser;

	//�ؼ�����
	BYTE cbGoldCard[2];
	ZeroMemory(cbGoldCard,sizeof(cbGoldCard));

	cbGoldCard[0] = 0x12;/*pOpenGold->cbKeyGoldCard*/;
	m_GameClientView.m_GoldCardControl.SetCardData(cbGoldCard,1,0/*,NULL,0*/);
	m_GameClientView.m_GoldCardControl.SetDisplayItem(true);
	//m_GameClientView.SetGoldCardShow(true);
	//m_GameClientView.SetGoldDice(m_wGoldDiceCount);
	
	//����λ��
	BYTE cbGoldCardPos;

	////������ƽ���λ��
	//if(m_wGoldDiceCount<=m_wDoorDiceCount)//���Ƴ�����ׯ�ҷ�
	//{
	//	cbGoldCardPos = OUTSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[m_wBankerUser][1]-m_wDoorDiceCount+m_wGoldDiceCount;

	//	//��������
	//	m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(m_wBankerUser)].SetCardData(m_cbHeapCardInfo[m_wBankerUser][0],m_cbHeapCardInfo[m_wBankerUser][1],OUTSIDE_HEAP_FULL_COUNT,cbGoldCardPos);
	//}
	//else//���Ʋ�������ׯ�ҷ�
	//{
	//	//�����˿�λ��
	//	WORD wChairID = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;

	//	//cbGoldCardPos = OUTSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[wChairID][1]-(m_wGoldDiceCount-m_wDoorDiceCount);
	//	cbGoldCardPos = (m_wGoldDiceCount-m_wDoorDiceCount)%OUTSIDE_HEAP_FULL_COUNT;

	//	//��������
	//	m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(wChairID)].SetCardData(m_cbHeapCardInfo[wChairID][0],m_cbHeapCardInfo[wChairID][1],OUTSIDE_HEAP_FULL_COUNT,cbGoldCardPos);
	//}

	////�����˿�
	//m_cbLeftCardCount--;

	return true;
}


//������ʾ
bool CGameClientEngine::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//��Ӷ���
	tagActionActionNotify *pActionActionNotify = (tagActionActionNotify *)ActiveUserAction(AK_ActionNotify);
	ASSERT( pActionActionNotify );
	if( !pActionActionNotify ) return false;

	pActionActionNotify->cbActionCard = pOperateNotify->cbActionCard;
	pActionActionNotify->cbActionMask = pOperateNotify->cbActionMask;

	m_UserActionArray.Add(pActionActionNotify);

	//��ʼ����
	BeginUserAction();

	return true;
}

//�������
bool CGameClientEngine::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//��Ӷ���
	tagActionActionResult *pActionActionResult = (tagActionActionResult *)ActiveUserAction(AK_ActionResult);
	ASSERT( pActionActionResult );
	if( !pActionActionResult ) return false;

	pActionActionResult->wOperateUser = pOperateResult->wOperateUser;
	pActionActionResult->wProvideUser = pOperateResult->wProvideUser;
	pActionActionResult->cbOperateCode = pOperateResult->cbOperateCode;
	CopyMemory( pActionActionResult->cbOperateCard,pOperateResult->cbOperateCard,
		sizeof(pOperateResult->cbOperateCard) );

	m_UserActionArray.Add(pActionActionResult);

	//��ʼ����
	BeginUserAction();

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

	//��������
	tagActionGameEnd *pActionGameEnd = (tagActionGameEnd *)ActiveUserAction(AK_GameEnd);
	CopyMemory( pActionGameEnd->cbCardCount,pGameEnd->cbCardCount,sizeof(pGameEnd->cbCardCount) );
	CopyMemory( pActionGameEnd->cbCardData,pGameEnd->cbCardData,sizeof(pGameEnd->cbCardData) );
	pActionGameEnd->cbProvideCard = pGameEnd->cbProvideCard;
	CopyMemory( pActionGameEnd->dwChiHuKind,pGameEnd->dwChiHuKind,sizeof(pGameEnd->dwChiHuKind) );
	CopyMemory( pActionGameEnd->dwChiHuRight,pGameEnd->dwChiHuRight,sizeof(pGameEnd->dwChiHuRight) );
	pActionGameEnd->cbFanCount = pGameEnd->cbFanCount;
	CopyMemory( pActionGameEnd->lGameScore,pGameEnd->lGameScore,sizeof(pGameEnd->lGameScore) );
	pActionGameEnd->lGameTax = pGameEnd->lGameTax;
	pActionGameEnd->wProvideUser = pGameEnd->wProvideUser;
	pActionGameEnd->cbHuaCardCount = pGameEnd->cbHuaCardCount;

	m_UserActionArray.Add(pActionGameEnd);

	BeginUserAction();

	return true;
}

//�û��й�
bool CGameClientEngine::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//��Ϣ����
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		IClientUserItem * pUserData=GetTableUserItem(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ѡ�����йܹ���."),pUserData->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ȡ�����йܹ���."),pUserData->GetNickName());
		m_pIStringMessage->InsertSystemString(szBuffer);
	}

	return true;
}

//�û�����
bool CGameClientEngine::OnSubListen(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Listen));
	if (wDataSize!=sizeof(CMD_S_Listen)) return false;

	CMD_S_Listen *pListen = (CMD_S_Listen *)pBuffer;

	//��Ӷ���
	tagActionListenCard *pActionListen = (tagActionListenCard *)ActiveUserAction(AK_ListenCard);
	ASSERT(pActionListen);
	if( !pActionListen ) return false;

	pActionListen->wChairId = pListen->wChairId;

	m_UserActionArray.Add(pActionListen);

	BeginUserAction();

	return true;
}

//���ų�������
void CGameClientEngine::PlayCardSound(WORD wChairID, BYTE cbCardData)
{
	if(m_GameLogic.IsValidCard(cbCardData) == false)
	{
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}

	//�ж��Ա�
	IClientUserItem * pUserData = GetTableUserItem(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "�ò��������Ϣ");
		return;
	}
	bool bBoy = (pUserData->GetGender() == 2 ? false : true);
	BYTE cbType= (cbCardData & MASK_COLOR);
	BYTE cbValue= (cbCardData & MASK_VALUE);
	CString strSoundName;
	switch(cbType)
	{
	case 0X30:	//��
		{
			switch(cbValue) 
			{
			case 1:
				{
					strSoundName = _T("F_1");
					break;
				}
			case 2:
				{
					strSoundName = _T("F_2");
					break;
				}
			case 3:
				{
					strSoundName = _T("F_3");
					break;
				}
			case 4:
				{
					strSoundName = _T("F_4");
					break;
				}
			case 5:
				{
					strSoundName = _T("F_5");
					break;
				}
			case 6:
				{
					strSoundName = _T("F_6");
					break;
				}
			case 7:
				{
					strSoundName = _T("F_7");
					break;
				}
#ifdef USE_OLD
			default:
				{
					strSoundName=_T("BU_HUA");
				}
#endif
			}
			break;
		}
#ifdef USE_OLD
	case 0X20:	//Ͳ
		{
			strSoundName.Format(_T("T_%d"), cbValue);
			break;
		}

	case 0X10:	//��
		{
			strSoundName.Format(_T("S_%d"), cbValue);
			break;
		}
	case 0X00:	//��
		{
			strSoundName.Format(_T("W_%d"), cbValue);
			break;
		}
#else
	case 0X00:	//Ͳ
		{
			strSoundName.Format(_T("T_%d"), cbValue);
			break;
		}

	case 0X20:	//��
		{
			strSoundName.Format(_T("S_%d"), cbValue);
			break;
		}
	case 0X10:	//��
		{
			strSoundName.Format(_T("W_%d"), cbValue);
			break;
		}
	case 0X40:	//����
		{
			strSoundName=_T("BU_HUA");
			break;
		}
#endif
	}	

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

//��������
void CGameClientEngine::PlayActionSound(WORD wChairID,BYTE cbAction)
{
	//�ж��Ա�
	IClientUserItem * pUserData = GetTableUserItem(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "�ò��������Ϣ");
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
	case WIK_RIGHT:		//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI"));		
			break;
		}
	case WIK_PENG:		//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_PENG"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_PENG"));	
			break;
		}
	case WIK_GANG:		//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_GANG"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_GANG"));		
			break;
		}
	case WIK_CHI_HU:	//�Ժ�
		{

			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI_HU"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI_HU"));		
			break;
		}
	case WIK_REPLACE:	//�滻����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_REPLACE"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_REPLACE"));		
			break;
		}
	case WIK_LISTEN:	//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_TING"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_TING"));		
			break;
		}
	}

	return;
}

//�����ж�
bool CGameClientEngine::VerdictOutCard(BYTE cbCardData)
{
	//�����ж�
	if ((m_cbListenStatus>0)||(m_bWillHearStatus==true))
	{
		//��������
		CChiHuRight chr;
		WORD wMeChairID=GetMeChairID();
		BYTE cbWeaveCount=m_cbWeaveCount[wMeChairID];

		//�����˿�
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,m_cbCardIndex,sizeof(cbCardIndexTemp));

		//ɾ���˿�
		if( !m_GameLogic.RemoveCard(cbCardIndexTemp,cbCardData) )
		{
			ASSERT( FALSE );
			return false;
		}

		//�����ж�
		BYTE i=0;
		for ( i=0;i<MAX_INDEX;i++)
		{
			//���Ʒ���
			BYTE cbCurrentCard=m_GameLogic.SwitchToCardData(i);
			BYTE cbHuCardKind=m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp,m_WeaveItemArray[wMeChairID],cbWeaveCount,cbCurrentCard,chr);

			//����ж�
			if (cbHuCardKind!=WIK_NULL)
			{
				break;
			}
		}

		//�����ж�
		return (i!=MAX_INDEX);
	}

	return true;
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	StopUserAction();

	//��������
	KillGameClock(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScore.RestorationData();
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//���ý���
	m_GameClientView.SetDiscUser(INVALID_CHAIR);
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);
	
	//�˿�����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HeapCard[i].ResetCardData();
		m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
	}

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbListenStatus = 0;
	m_bWillHearStatus = false;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//���Ʋ���
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))
		return 0;

	//�����ж�
	if (((m_cbListenStatus>0)||(m_bWillHearStatus==true))&&(VerdictOutCard((BYTE)wParam)==false))
	{
		m_pIStringMessage->InsertSystemString(TEXT("�����Ʋ�������Ϸ����!"));
		return 0;
	}

	KillGameClock(IDI_OPERATE_CARD);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	BYTE cbOutCardData=(BYTE)wParam;

	//���ý���
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	if( m_GameClientView.m_DlgListen.m_hWnd )
		m_GameClientView.m_DlgListen.EndDialog(IDCANCEL);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	//��Ӷ���
	tagActionOutCard *pActionOutCard = (tagActionOutCard *)ActiveUserAction(AK_OutCard);
	ASSERT( pActionOutCard );
	if( !pActionOutCard ) return 0;

	pActionOutCard->cbOutCardData = cbOutCardData;
	pActionOutCard->wOutCardUser = GetMeChairID();

	m_UserActionArray.Add(pActionOutCard);

	BeginUserAction();

	return 0;
}

//�˿˲���
LRESULT CGameClientEngine::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbOperateCode;
	BYTE cbOperateCard[3] = {0,0,0};

	//ɾ��ʱ��
	KillGameClock(IDI_OPERATE_CARD);

	//��ȡѡ����Ϣ
	tagSelectCardInfo si;
	m_GameClientView.m_HandCardControl.GetUserSelectResult( si );
	
	//���������Ϣ
	cbOperateCode = (BYTE)si.wActionMask;
	cbOperateCard[0] = si.cbActionCard;
	CopyMemory( &cbOperateCard[1],si.cbCardData,sizeof(BYTE)*2 );

	//ȡ������ģʽ
	m_GameClientView.m_HandCardControl.SetSelectMode( false );

	//���������
	if( m_bWillHearStatus && (cbOperateCode&WIK_LISTEN) )
	{
		m_bWillHearStatus = false;
		return OnOutCard( cbOperateCard[0],0 );
	}

	m_wCurrentUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//��������
	m_GameClientView.SetStatusFlag(false,true);
	
	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

//�Ϲܿ���
LRESULT CGameClientEngine::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wTimeOutCount=0;

	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false, false);
		if( FALSE == (BOOL)wParam ) 
			m_pIStringMessage->InsertSystemString(_T("��ȡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false, false);
		if( FALSE == (BOOL)wParam )
			m_pIStringMessage->InsertSystemString(_T("��ѡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}

	return 0;
}

//�ڶ���ҡɫ����Ϣ
LRESULT CGameClientEngine::OnSiceTwo(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	return 0;
}

//�����˿�
void CGameClientEngine::SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard)
{
	//�����Ŀ
	WORD wMeChairID=GetMeChairID();
	BYTE cbWeaveCardCount=m_cbWeaveCount[wMeChairID]*3;

	//ת���˿�
	BYTE cbHandCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(cbCardIndex,cbHandCardData);

	//�����˿�
	if ( (cbWeaveCardCount+cbCardCount)==MAX_COUNT )
	{
		if( cbAdvanceCard != 0x00 )
		{
			//ɾ���˿�
			BYTE cbRemoveCard[]={cbAdvanceCard};
			VERIFY( m_GameLogic.RemoveCard(cbHandCardData,cbCardCount,cbRemoveCard,1) );

			//�����˿�
			cbHandCardData[cbCardCount-1]=cbAdvanceCard;
		}
		
		m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount-1,cbHandCardData[cbCardCount-1]);
	}
	else m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount,0x00);

	return;
}

//��ʱ����Ϣ
VOID CGameClientEngine::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_REPLACE_CARD:	//�û�����
		{
			OnTimerReplaceCard();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//��Ҳ����ؼ�
LRESULT CGameClientEngine::OnUserAction( WPARAM wParam, LPARAM lParam )
{
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard[3] = {0,0,0};

	//���ز����ؼ�
	m_GameClientView.m_ControlWnd.ShowWindow( SW_HIDE );

	//״̬�ж�
	if (cbOperateCode==WIK_NULL)
	{
		//���ñ���
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;
		if( m_wCurrentUser == INVALID_CHAIR )
		{
			//ɾ����ʱ��
			KillGameClock( IDI_OPERATE_CARD );
			//������Ϣ
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

		//����ѡ��
		if( m_GameClientView.m_DlgListen.DoModal() == IDOK )
		{
			//ѡ��������ץ��
			if( m_GameClientView.m_DlgListen.m_bZhuaPao && m_GameClientView.m_DlgListen.m_bZiMo )
				m_cbListenStatus = 3;
			//ֻ����
			else if( m_GameClientView.m_DlgListen.m_bZiMo )
				m_cbListenStatus = 2;
			//ֻץ��
			else if( m_GameClientView.m_DlgListen.m_bZhuaPao )
				m_cbListenStatus = 1;
			//���ѡ����,����������Ϣ
			if( m_cbListenStatus > 0 )
			{
				//���ü������Ʊ�־
				m_bWillHearStatus = true;

				//���ñ���
				m_cbActionMask = WIK_NULL;
				m_cbActionCard = 0;

				//��������״̬
				m_GameClientView.SetUserListenStatus( 2,true );

				//������������
				CMD_C_Listen Listen;
				Listen.cbListen = m_cbListenStatus;
				SendSocketData( SUB_C_LISTEN,&Listen,sizeof(Listen) );

				//��ȡѡ�����
				tagSelectCardInfo sci[MAX_WEAVE];
				BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );
				
				//���ֻ��һ����
				if( cbInfoCount == 1 )
				{
					m_bWillHearStatus = false;
					OnOutCard( sci[0].cbActionCard,0 );
				}
				//���������ѡ��
				else
				{
					//���ò����¼�
					m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );
					//��ʾ��Ϣ
					m_pIStringMessage->InsertSystemString(TEXT("��ѡ�������"));
				}
			}
		}

		return 0;
	}

	bool bDone = false;
	//����
	if( cbOperateCode & WIK_CHI_HU )
		bDone = true;
	else
	{
		//��ȡѡ�����
		tagSelectCardInfo sci[MAX_WEAVE];
		BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );

		//���ò����¼�
		bDone = m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );

		//�����ɲ���
		if( bDone )
		{
			//���ò������
			tagSelectCardInfo si;
			m_GameClientView.m_HandCardControl.GetUserSelectResult( si );
			cbOperateCode = (BYTE)si.wActionMask;
			cbOperateCard[0] = si.cbActionCard;
			CopyMemory( &cbOperateCard[1],si.cbCardData,2*sizeof(BYTE) );

			m_GameClientView.m_HandCardControl.SetSelectMode( false );
		}
		//�������õȴ�ѡ��
		else m_GameClientView.SetStatusFlag( true,false );
	}

	//���������ɣ�ֱ�ӷ��Ͳ�������
	if( bDone )
	{
		//ɾ����ʱ��
		KillGameClock( IDI_OPERATE_CARD );

		//���ñ���
		m_wCurrentUser = INVALID_CHAIR;
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;

		CMD_C_OperateCard OperateCard;
		OperateCard.cbOperateCode = cbOperateCode;
		CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
		SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
	}

	return 0;
}

//��ȡ������Ϣ
BYTE CGameClientEngine::GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] )
{
	//��ʼ��
	BYTE cbSelectCount = 0;

	if( wOperateCode == WIK_NULL ) return 0;

	//����
	if( wOperateCode & WIK_LISTEN )
	{
		//��ȡ�ɶ�����
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
	//����
	else if( wOperateCode&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
	{
		//Ч��
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
	//����
	else if( wOperateCode & WIK_PENG )
	{
		//Ч��
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
		SelectInfo[cbSelectCount].wActionMask = WIK_PENG;
		SelectInfo[cbSelectCount].cbCardCount = 2;
		SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
		SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard;
	}
	//����
	else if( wOperateCode & WIK_GANG )
	{
		//������Լ�����
		if( m_wCurrentUser == GetMeChairID() )
		{
			//Ѱ���Ƿ��ж������
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


//��ʼ��Ҷ���
bool CGameClientEngine::BeginUserAction()
{
	if( m_UserActionArray.GetCount() == 0 )
		return false;

	//ö�ٶ���
	switch( m_UserActionArray[0]->enActionKind )
	{
	case AK_StartCard:				//����
		return BeginActionStartCard();
	case AK_OutCard:				//����
		return BeginActionOutCard();
	case AK_SendCard:				//ץ��
		return BeginActionSendCard();
	case AK_ReplaceCard:			//����
		return BeginActionReplaceCard();
	case AK_ActionNotify:			//��ʾ����
		return BeginActionActionNotify();
	case AK_ActionResult:			//�������
		return BeginActionActionResult();
	case AK_ListenCard:				//���Ʋ���
		return BeginActionListenCard();
	case AK_Sice:					//��ɫ��
		return BeginActionSice();	
	case AK_GameEnd:				//������Ϸ
		return BeginActionGameEnd();
	default:
		ASSERT(FALSE);
		return false;
	}

	return false;
}

//ֹͣ��Ҷ���
bool CGameClientEngine::StopUserAction()
{
	//ֹͣɫ�Ӷ���
	m_GameClientView.m_DrawSiceWnd.StopSicing();

	if( m_GameClientView.IsMovingCard() )
		m_GameClientView.StopMoveCard();

	while( BeginUserAction() )
	{
		//�ж�����
		ASSERT(m_UserActionArray.GetCount() > 0 );
		if( m_UserActionArray.GetCount() > 0 )
		{
			enmActionKind ActionKind = m_UserActionArray[0]->enActionKind;
			if( ActionKind == AK_ReplaceCard )
				OnTimerReplaceCard();
			else if( m_GameClientView.IsMovingCard() )
				m_GameClientView.StopMoveCard();
		}
	}

	return true;
}

//�Ƴ���ǰ����
bool CGameClientEngine::RemoveCurrentAction()
{
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//ɾ����ǰ����
	tagUserAction *pUserAction = m_UserActionArray[0];
	m_UserActionArray.RemoveAt(0);
	m_UserActionStorage.Add(pUserAction);
	pUserAction->ResetAction();

	return true;
}

//�����
tagUserAction *CGameClientEngine::ActiveUserAction(enmActionKind enActionKind)
{
	//��������
	tagUserAction *pUserAction = NULL;

	//Ѱ�ҿ��
	for( INT_PTR i = 0; i < m_UserActionStorage.GetCount(); i++ )
	{
		if( m_UserActionStorage[i]->enActionKind == enActionKind )
		{
			pUserAction = m_UserActionStorage[i];
			m_UserActionStorage.RemoveAt(i);
			return pUserAction;
		}
	}

	//�´���
	switch( enActionKind )
	{
	case AK_StartCard:
		{
			pUserAction = new tagActionStartCard;
			return pUserAction;
		}
	case AK_OutCard:
		{
			pUserAction = new tagActionOutCard;
			return pUserAction;
		}
	case AK_SendCard:
		{
			pUserAction = new tagActionSendCard;
			return pUserAction;
		}
	case AK_ReplaceCard:
		{
			pUserAction = new tagActionReplaceCard;
			return pUserAction;
		}
	case AK_ActionNotify:
		{
			pUserAction = new tagActionActionNotify;
			return pUserAction;
		}
	case AK_ActionResult:
		{
			pUserAction = new tagActionActionResult;
			return pUserAction;
		}
	case AK_ListenCard:
		{
			pUserAction = new tagActionListenCard;
			return pUserAction;
		}
	case AK_GameEnd:
		{
			pUserAction = new tagActionGameEnd;
			return pUserAction;
		}
	case AK_Sice:
		{
			pUserAction = new tagActionSice;
			return pUserAction;
		}
	default:
		{
			ASSERT(FALSE);
			break;
		}
	}

	return pUserAction;
}

//���ƶ���
bool CGameClientEngine::BeginActionStartCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionStartCard *pStartCardItem = (tagActionStartCard *)m_UserActionArray[0];

	pStartCardItem->bHandleStatus = TRUE;

	//��ʼ,������
	CPoint ptFrom,ptTo;

	//����λ��
	WORD wHeapId = pStartCardItem->wHeapId;
	WORD wHeapViewId = SwitchViewChairID(wHeapId);
	//����λ��
	WORD wHeapCardIndex = pStartCardItem->wHeapCardIndex;
	if( pStartCardItem->cbCardCount > wHeapCardIndex+1 )
	{
		ASSERT( pStartCardItem->cbCardCount == 4 );
		//ɾ��ǰ��Ķ���
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,wHeapCardIndex+1 );

		wHeapId = (wHeapId+1)%GAME_PLAYER;
		wHeapViewId = SwitchViewChairID(wHeapId);
		wHeapCardIndex = HEAP_FULL_COUNT-(4-(wHeapCardIndex+1))-1;

		//��ȡ��ʼ��
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(WORD(wHeapCardIndex-1));

		//ɾ������
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( HEAP_FULL_COUNT-1,
			pStartCardItem->cbCardCount-pStartCardItem->wHeapCardIndex-1 );
	}
	else
	{
		WORD wHeapPos = wHeapCardIndex;
		if( pStartCardItem->cbCardCount == 4 )
			wHeapPos -= 3;
		else
		{
			ASSERT( pStartCardItem->cbCardCount == 1 );
			if( wHeapPos%2 != 0 ) wHeapPos--;
			else wHeapPos++;
		}
		//��ȡ��ʼ��
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(wHeapPos);

		//ɾ������
		if( pStartCardItem->cbCardCount == 4 )
			m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,4 );
		else
		{
			ASSERT( pStartCardItem->cbCardCount == 1 );
			if( wHeapCardIndex%2 != 0 )
				m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex-1,pStartCardItem->cbCardCount );
			else m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex+1,pStartCardItem->cbCardCount );
		}
	}

	//��ȡ������
	WORD wViewChairId = SwitchViewChairID(pStartCardItem->wChairId);
	if( wViewChairId == MYSELF_VIEW_ID )
		ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		ptTo = m_GameClientView.m_UserCard[wUserIndex].GetDispatchCardPos();
	}

	//���ŷ�������
	m_DirectSound[1].Play();

	//���춯��
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = pStartCardItem->cbCardCount;
	mci.cbCardData = 0;
	mci.wViewChairId = wViewChairId;
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//���ƶ���
bool CGameClientEngine::BeginActionOutCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionOutCard *pOutCardItem = (tagActionOutCard *)m_UserActionArray[0];

	pOutCardItem->bHandleStatus = TRUE;

	//�������
	WORD wMeChairId = GetMeChairID();
	WORD wOutCardUser = pOutCardItem->wOutCardUser;
	WORD wViewOutCardUser = SwitchViewChairID(wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->cbOutCardData;

	//��������
	KillGameClock(IDI_OPERATE_CARD); 

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//������ʼ,������
	CPoint ptFrom,ptTo;
	if( wOutCardUser == wMeChairId )
		ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wViewId = wViewOutCardUser>=3?2:wViewOutCardUser;
		ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
	}
	ptTo = m_GameClientView.m_DiscardCard[wViewOutCardUser].GetDispatchCardPos();

	//��������
	PlayCardSound(wOutCardUser,cbOutCardData);

	//���ý���
	m_GameClientView.SetCurrentUser( INVALID_CHAIR );
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(wViewOutCardUser,cbOutCardData);
	m_GameClientView.SetStatusFlag(false,false);

	//�����˿�
	if (wViewOutCardUser==MYSELF_VIEW_ID)
	{
		//ɾ���˿�
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

	return true;
}

//���ƶ���
bool CGameClientEngine::BeginActionSendCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionSendCard *pSendCardItem = (tagActionSendCard *)m_UserActionArray[0];

	pSendCardItem->bHandleStatus = TRUE;

	WORD wSendUserViewId = SwitchViewChairID(pSendCardItem->wSendCardUser);

	//������ʼ,������
	CPoint ptFrom,ptTo;
	if( !pSendCardItem->bTail )
	{
		ASSERT( m_wHeapHead != INVALID_CHAIR );
		ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].GetDispatchCardPos(false);
		//ɾ����ͷ
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
		//ɾ����β
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

	//���ŷ�������
	m_DirectSound[1].Play();

	//���춯��
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = 1;
	mci.cbCardData = 0;
	mci.wViewChairId = SwitchViewChairID(pSendCardItem->wSendCardUser);
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//��������
bool CGameClientEngine::BeginActionReplaceCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionReplaceCard *pReplaceCardItem = (tagActionReplaceCard *)m_UserActionArray[0];

	pReplaceCardItem->bHandleStatus = TRUE;

	//���ñ���
	WORD wMeChairId = GetMeChairID();
	m_wReplaceUser = pReplaceCardItem->wReplaceUser;
	BYTE cbReplaceCard = pReplaceCardItem->cbReplaceCard;
	WORD wReplaceUserViewId = SwitchViewChairID(m_wReplaceUser);

	//���Ʋ���
	if ((IsLookonMode()==false)&&(m_wReplaceUser==wMeChairId))
	{
		//����ʱ��
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
	}

	if( IsLookonMode() || m_wReplaceUser != wMeChairId )
	{
		//��������
		PlayActionSound(m_wReplaceUser,WIK_REPLACE);

		//������ʼ,������
		CPoint ptFrom,ptTo;
		if( m_wReplaceUser == wMeChairId )
			ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
		else
		{
			WORD wViewId = wReplaceUserViewId>=3?2:wReplaceUserViewId;
			ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
		}
		ptTo = m_GameClientView.m_DiscardCard[wReplaceUserViewId].GetDispatchCardPos();

		//���ö���
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction( wReplaceUserViewId,WIK_REPLACE );

		//�����˿�
		if (wReplaceUserViewId==MYSELF_VIEW_ID)
		{
			//ɾ���˿�
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard) )
			{
				ASSERT( FALSE );
				return false;
			}

			//�����˿�
			SetHandCardControl(m_cbCardIndex,0x00);
		}
		else
		{
			//�����˿�
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

//��ʾ����
bool CGameClientEngine::BeginActionActionNotify()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionActionNotify *pOperateNotify = (tagActionActionNotify *)m_UserActionArray[0];

	pOperateNotify->bHandleStatus = TRUE;

	//�û�����
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//��ȡ����
		WORD wMeChairID=GetMeChairID();
		m_cbActionMask = pOperateNotify->cbActionMask;
		m_cbActionCard = pOperateNotify->cbActionCard;

		//������������ڲ���ģʽ,��ȡ��
		if( m_GameClientView.m_HandCardControl.GetSelectMode() )
			m_GameClientView.m_HandCardControl.SetSelectMode(false);

		//���ý���
		ActiveGameFrame();
		m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);

		//����ʱ��
		m_GameClientView.SetCurrentUser(INVALID_CHAIR);
		SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	return OnActionActionNotify();
}

//�������
bool CGameClientEngine::BeginActionActionResult()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionActionResult *pOperateResult = (tagActionActionResult *)m_UserActionArray[0];

	pOperateResult->bHandleStatus = TRUE;

	//ɾ����ʱ��
	KillGameClock(IDI_OPERATE_CARD);

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard[3];
	CopyMemory( cbOperateCard,pOperateResult->cbOperateCard,sizeof(cbOperateCard) );
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);
	WORD wProviderViewID = SwitchViewChairID(pOperateResult->wProvideUser);

	//���ñ���
	m_wCurrentUser=pOperateResult->wOperateUser;

	//�������
	if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)
	{
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;

		//����˿�
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

		//����˿�
		if (cbWeaveIndex==0xFF)
		{
			//�����ж�
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

			//�����˿�
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
			for( BYTE i = 0; i < 4; i++ ) m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[i] = cbOperateCard[0];

			m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);
		}

		//��Ͻ���
		BYTE cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=4;
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,
			cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicCard==TRUE)?true:false);

		//�˿�����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbOperateCard[0])]=0;
		}

		//�����˿�
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
		//�������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		CopyMemory( m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,cbOperateCard,sizeof(cbOperateCard) );

		//��Ͻ���
		BYTE cbWeaveCard[3],cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=3;
		CopyMemory( cbWeaveCard,cbOperateCard,sizeof(cbWeaveCard) );
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);

		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{
			VERIFY( m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard[0],1) );
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1) )
			{
				ASSERT( FALSE );
				return false;
			}
		}

		//�����˿�
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

	//���ý���
	//����ṩ��Ҳ����Լ�,��ȥ���ṩ��ҵ��ѳ���
	if( pOperateResult->wProvideUser != wOperateUser )
	{
		if( m_GameClientView.m_DiscardCard[wProviderViewID].GetCardCount() > 0 )
			m_GameClientView.m_DiscardCard[wProviderViewID].RemoveCard(1);
	}
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);
	//������������ڲ���ģʽ,��ȡ��
	if( m_GameClientView.m_HandCardControl.GetSelectMode() )
		m_GameClientView.m_HandCardControl.SetSelectMode(false);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//��������
	PlayActionSound(wOperateUser,pOperateResult->cbOperateCode);

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//�����ж�
		if( !IsLookonMode() && m_cbListenStatus == 0 && m_wCurrentUser==GetMeChairID() )
		{
			WORD wMeChairId = GetMeChairID();

			m_cbActionMask = m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId]);

			if( m_cbActionMask != WIK_NULL )
			{
				m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
			}
		}

		//����ʱ��
		WORD wTimeCount=TIME_OPERATE_CARD;

		//����ʱ��
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}

	return OnActionActionResult();
}

//���Ʋ���
bool CGameClientEngine::BeginActionListenCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionListenCard *pListen = (tagActionListenCard *)m_UserActionArray[0];

	pListen->bHandleStatus = TRUE;

	WORD wViewId = SwitchViewChairID( pListen->wChairId );

	//��������״̬
	m_GameClientView.SetUserListenStatus( wViewId,true );

	//��������
	PlayActionSound( pListen->wChairId,WIK_LISTEN );

	m_GameClientView.SetOutCardInfo( INVALID_CHAIR,0 );
	m_GameClientView.SetUserAction( wViewId,WIK_LISTEN );

	//����״̬
	m_GameClientView.SetStatusFlag( (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),m_wCurrentUser!=GetMeChairID() );

	return OnActionListenCard();
}

//��ɫ��
bool CGameClientEngine::BeginActionSice()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionSice *pActionSice = (tagActionSice *)m_UserActionArray[0];

	pActionSice->bHandleStatus = TRUE;

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//ҡɫ�Ӷ���
	m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,(WORD)(pActionSice->lSiceCount>>16),(WORD)pActionSice->lSiceCount);
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);

	return true;
}

//��Ϸ����
bool CGameClientEngine::BeginActionGameEnd()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionGameEnd *pGameEnd = (tagActionGameEnd *)m_UserActionArray[0];

	pGameEnd->bHandleStatus = TRUE;

	//����״̬
	SetGameStatus(GS_MJ_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	//ɾ����ʱ��
	KillGameClock(IDI_OPERATE_CARD);

	//���ÿؼ�
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
	}

	//��������
	tagScoreInfo ScoreInfo;
	tagWeaveInfo WeaveInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));

	//�ɼ�����
	ScoreInfo.wProvideUser=pGameEnd->wProvideUser;
	ScoreInfo.cbProvideCard=pGameEnd->cbProvideCard;

	//���û���
	CString strTemp ,strEnd = TEXT("���ֽ���,�ɼ�ͳ��:\r\n");
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		strTemp.Format(TEXT("%s: %+I64d\n"),m_szAccounts[i],pGameEnd->lGameScore[i]);
		strEnd += strTemp;
	}
	//��Ϣ����
	m_pIStringMessage->InsertSystemString((LPCTSTR)strEnd);

	CChiHuRight chr;
	chr.SetRightData( pGameEnd->dwChiHuRight,CountArray(pGameEnd->dwChiHuRight) );
	if( pGameEnd->wProvideUser != INVALID_CHAIR )
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			ScoreInfo.dwChiHuKind[i]=pGameEnd->dwChiHuKind[i];

			//���óɼ�
			ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(ScoreInfo.szUserName[i],m_szAccounts[i],CountArray(ScoreInfo.szUserName[i]));

			//�����˿�
			if ((ScoreInfo.cbCardCount==0)&&(pGameEnd->dwChiHuKind[i]!=WIK_NULL))
			{
				ScoreInfo.wChiHuUser = i;
				//����˿�
				WeaveInfo.cbWeaveCount=m_cbWeaveCount[i];
				for (BYTE j=0;j<WeaveInfo.cbWeaveCount;j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					WeaveInfo.cbPublicWeave[j]=m_WeaveItemArray[i][j].cbPublicCard;
					WeaveInfo.cbCardCount[j]=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					CopyMemory( WeaveInfo.cbCardData[j],m_WeaveItemArray[i][j].cbCardData,sizeof(m_WeaveItemArray[i][j].cbCardData) );
				}

				//�����˿�
				ScoreInfo.cbCardCount=pGameEnd->cbCardCount[i];
				CopyMemory(ScoreInfo.cbCardData,&pGameEnd->cbCardData[i],ScoreInfo.cbCardCount*sizeof(BYTE));

				//��ȡ����
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
		}

		ScoreInfo.cbFanCount = pGameEnd->cbFanCount;
		ScoreInfo.cbHuaCardCount = pGameEnd->cbHuaCardCount;
		//�ɼ�����
		m_GameClientView.m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo,chr);
	}
	else m_GameClientView.SetHuangZhuang(true);

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->dwChiHuKind[i]!=WIK_NULL) m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//��������
	if( !chr.IsEmpty() )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( pGameEnd->dwChiHuKind[i] != WIK_NULL )
			{
				PlayActionSound( i,WIK_CHI_HU );
				break;
			}
		}
	}
	else 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));
	}

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	//ȡ���й�
	if(m_bStustee)
		OnStusteeControl(0,0);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return OnActionGameEnd();
}

//�������
bool CGameClientEngine::OnActionStartCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//��������
	tagActionStartCard *pStartCardItem = (tagActionStartCard *)m_UserActionArray[0];

	//���ñ���
	WORD wMeChairId = GetMeChairID();
	WORD wChairId = pStartCardItem->wChairId;
	WORD wViewChairId = SwitchViewChairID(wChairId);

	//����˿�
	if( wViewChairId == MYSELF_VIEW_ID )
		m_GameClientView.m_HandCardControl.AddCardData( pStartCardItem->cbCardData,pStartCardItem->cbCardCount );
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		if( pStartCardItem->bLastItem )
			m_GameClientView.m_UserCard[wUserIndex].AddCardData(true);
		else
		{
			for( BYTE i = 0; i < pStartCardItem->cbCardCount; i++ )
				m_GameClientView.m_UserCard[wUserIndex].AddCardData(false);
		}
	}

	//��������
	m_DirectSound[1].Play();

	//�ж��Ƿ����һ����
	if( pStartCardItem->bLastItem )
	{
		//������
		if (!IsLookonMode())
			ActiveGameFrame();

		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetHeapCardInfo(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1]);
		}

		//�����˿�
		SetHandCardControl(m_cbCardIndex,0x00);

		m_GameClientView.m_HandCardControl.SetPositively(!IsLookonMode());

		//������ʾ,��������
		if (!IsLookonMode()&&(m_wCurrentUser==wMeChairId))
		{
			m_GameClientView.SetStatusFlag(true,false);

			m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,NULL,0);

			if( m_cbActionMask != WIK_NULL )
				m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
		}
		else if( m_wCurrentUser != wMeChairId && m_wCurrentUser != INVALID_CHAIR )
			m_GameClientView.SetStatusFlag(false,true);

		//����ʱ��
		if (m_wCurrentUser!=INVALID_CHAIR)
		{
			m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
			SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
	}

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//�������
bool CGameClientEngine::OnActionOutCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//��������
	tagActionOutCard *pOutCardItem = (tagActionOutCard *)m_UserActionArray[0];

	WORD wOutUserViewId = SwitchViewChairID(pOutCardItem->wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->cbOutCardData;

	//��ӵ��Ѷ�����
	m_GameClientView.m_DiscardCard[wOutUserViewId].AddCardItem( cbOutCardData );

	//���ó���ָʾ
	m_GameClientView.SetDiscUser( wOutUserViewId );

	//��������
	m_DirectSound[0].Play();

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//ץ�����
bool CGameClientEngine::OnActionSendCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//��������
	tagActionSendCard *pSendCardItem = (tagActionSendCard *)m_UserActionArray[0];

	//���ñ���
	WORD wMeChairID = GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pSendCardItem->wSendCardUser);
	WORD wChairId = pSendCardItem->wSendCardUser;
	BYTE cbSendCardData = pSendCardItem->cbCardData;
	m_wCurrentUser = pSendCardItem->wCurrentUser;

	//ȡ�ƽ���
	if (wViewChairID!=MYSELF_VIEW_ID)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		BYTE cbCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount()+m_cbWeaveCount[wChairId]*3;
		m_GameClientView.m_UserCard[wUserIndex].AddCardData(cbCardCount==MAX_COUNT-1?true:false);
	}
	else
	{
		//�����˿�
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbSendCardData)]++;

		//�����˿�
		SetHandCardControl(m_cbCardIndex,cbSendCardData);
	}

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		ActiveGameFrame();

		m_cbActionMask = pSendCardItem->cbActionMask;
		if( m_cbListenStatus == 0 )
		{
			m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID]);
		}
		m_cbActionCard = pSendCardItem->cbCardData;

		//��������
		if (m_cbActionMask!=WIK_NULL)
		{
			//���ý���
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
		}
	}

	//������ʾ
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//����ʱ��
	WORD wTimeCount=TIME_OPERATE_CARD;

	//����ʱ��
	if( m_wCurrentUser != INVALID_CHAIR )
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}
	else m_GameClientView.SetCurrentUser( INVALID_CHAIR );

	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//�������
bool CGameClientEngine::OnActionReplaceCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//��������
	tagActionReplaceCard *pReplaceCardItem = (tagActionReplaceCard *)m_UserActionArray[0];

	WORD wReplaceUserViewId = SwitchViewChairID(pReplaceCardItem->wReplaceUser);
	BYTE cbReplaceCardData = pReplaceCardItem->cbReplaceCard;

	//��ӵ��Ѷ�����
	m_GameClientView.m_DiscardCard[wReplaceUserViewId].AddCardItem( cbReplaceCardData );

	//���ó���ָʾ
	m_GameClientView.SetDiscUser( wReplaceUserViewId );

	//��������
	m_DirectSound[0].Play();

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//���ñ���
	m_wReplaceUser = INVALID_CHAIR;

	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//��ʾ���
bool CGameClientEngine::OnActionActionNotify()
{
	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//�������
bool CGameClientEngine::OnActionActionResult()
{
	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//���Ʋ���
bool CGameClientEngine::OnActionListenCard()
{
	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//��ɫ�����
bool CGameClientEngine::OnActionSice()
{
	//���ش���
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnActionGameEnd()
{
	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//�������
LRESULT CGameClientEngine::OnUserActionFinish(WPARAM wParam, LPARAM lParam)
{
	//У��
	ASSERT( m_UserActionArray.GetCount() > 0 );		
	if( m_UserActionArray.GetCount() == 0 ) return 0;

	//ö�ٶ���
	switch( m_UserActionArray[0]->enActionKind )
	{
	case AK_StartCard:				//�������
		OnActionStartCard();
		break;
	case AK_OutCard:				//�������
		OnActionOutCard();
		break;
	case AK_SendCard:				//ץ�����
		OnActionSendCard();
		break;
	case AK_ReplaceCard:			//�������
		OnActionReplaceCard();
		break;
	case AK_Sice:					//��ɫ�����
		OnActionSice();
		break;
	default:
		ASSERT(FALSE);
		return 0;
	}

	return 0;
}

//�������
LRESULT CGameClientEngine::OnOpenGold(WPARAM wParam,LPARAM lParam)
{
	switch( lParam )
	{
	case 0:
		{
			//by hxh
			m_GameClientView.m_GoldCardControl.SetPositively(false);
			m_GameClientView.m_GoldCardControl.SetDisplayItem(false);
			m_GameClientView.m_GoldCardControl.SetCardData(NULL,0,0/*,NULL,0*/);

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);
		}
		break;
	case 1:
		{
			OnSubOpenGold(0,0);
			//�ؼ�����
			//m_GameClientView.m_btOpenGold.ShowWindow(SW_HIDE);

			//ɾ����ʱ��
			//KillGameTimer(IDI_OPEN_GOLD);

			//SendData(QZSUB_C_OPENGOLD);

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);
		}
		break;
	case 2:
		{
			//by hxh
			BOOL is=m_GameClientView.m_btOpenGold.IsWindowVisible();
			m_GameClientView.m_btOpenGold.ShowWindow(is==1?SW_HIDE:SW_SHOW);

			////���½���
			//m_GameClientView.InvalidGameView(0,0,0,0);
		}
		break;
	default:
		ASSERT(FALSE);
		return 0;
	}



	return 0;
}

//�����¼�
bool CGameClientEngine::OnTimerReplaceCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//��������
	tagActionReplaceCard *pReplaceCardItem = (tagActionReplaceCard *)m_UserActionArray[0];

	//ɾ��ʱ��
	KillTimer(IDI_REPLACE_CARD);

	ASSERT( m_wReplaceUser == GetMeChairID() );
	if( m_wReplaceUser != GetMeChairID() ) return false;

	//���ý���
	m_GameClientView.SetUserAction( INVALID_CHAIR,0 );
	//Ѱ���˿�
	for (BYTE i=MAX_INDEX-MAX_HUA_CARD;i<MAX_INDEX;i++)
	{
		if (m_cbCardIndex[i]>0)
		{
			//��������
			BYTE cbReplaceCard=m_GameLogic.SwitchToCardData(i);
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard) )
			{
				ASSERT( FALSE );
				return false;
			}

			//��������
			CMD_C_ReplaceCard ReplaceCard;
			ReplaceCard.cbCardData=cbReplaceCard;
			SendSocketData(SUB_C_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

			//�����˿�
			SetHandCardControl(m_cbCardIndex,0x00);

			//��������
			PlayActionSound(GetMeChairID(),WIK_REPLACE);

			//���ý���
			m_GameClientView.SetUserAction( MYSELF_VIEW_ID,WIK_REPLACE );

			pReplaceCardItem->cbReplaceCard = cbReplaceCard;

			//������ƶ���
			tagMoveCardItem mci;
			mci.cbCardCount = 1;
			mci.cbCardData = cbReplaceCard;
			mci.wViewChairId = MYSELF_VIEW_ID;
			mci.ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
			mci.ptTo = m_GameClientView.m_DiscardCard[MYSELF_VIEW_ID].GetDispatchCardPos();
			m_GameClientView.OnMoveCardItem( mci );

			break;
		}
	}

	return true;
}


///////////
bool CGameClientEngine::OnEventGameClockKill(WORD t)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
