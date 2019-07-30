#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			201									//������ʱ��
#define IDI_REPLACE_CARD			301									//���ƶ�ʱ��

//��Ϸ��ʱ��
#define TIME_START_GAME				30									//��ʼ��ʱ��
#define TIME_OPERATE_CARD			15									//������ʱ��		
#define TIME_REPLACE_CARD			1000								//���ƶ�ʱ��

#define MAX_TIME_OUT				3									//���ʱ����

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_SICE_TWO,OnSiceTwo)
	ON_MESSAGE(IDM_SICE_FINISH,OnSiceFinish)
	ON_MESSAGE(IDM_OUT_CARD_FINISH,OnOutCardFinish)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_USER_ACTION,OnUserAction)
	ON_MESSAGE(IDM_USER_MOVE,OnUserMove)
	ON_MESSAGE(IDM_SOUND,OnSound)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg()
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
	m_bMustWanTong=true;


	//������Ϣ
	m_cbEnjoinCardCount = 0;
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));


	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::OnInitGameEngine()
{
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);//by hxh 20170308
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	g_CardResource.LoadResource();

	//��������
	m_DirectSound[0].Create( TEXT("OUT_CARD") );
	m_DirectSound[1].Create( TEXT("SEND_CARD") );
	m_DirectSound[2].Create( TEXT("SELECT") );


	/*WORD wWinner=0;
	m_wBankerUser=1;
	LONGLONG lScore[GAME_PLAYER];
	ZeroMemory(lScore,sizeof(lScore));
	LONGLONG lAllScore[]={0,10,20,10};

	for (WORD i=0;i<4;i++)
	{
		if(i==wWinner) continue;
		for (WORD j=0;j<4;j++)
		{
			if(j==i || j==wWinner)  continue;

			if(i==m_wBankerUser||j==m_wBankerUser)
			{
				lScore[i] +=(lAllScore[i]-lAllScore[j]);
			}
			else
			{
				LONGLONG  lTempScore=(lAllScore[i]-lAllScore[j])/2;
				if(lTempScore%10)
				{
					if(lTempScore>0)
					{
						lTempScore -= lTempScore%10;
						lTempScore += 10;
					}
					else
					{
						lTempScore -= lTempScore%10;
						lTempScore += -10;

					}
				}
				lScore[i] +=lTempScore;
			}

		}
	}*/
	



	//test
	/*BYTE cbCardData[] = {
		0x11,0x11,0x11,0x17,0x17,0x22,0x02,0x31,0x31,
		0x31,0x27,0x28,0x29
	};
	BYTE cbCardCount = 13;
	BYTE cbCurrentCard = 0x17;
	
	m_GameLogic.SetMagicIndex( m_GameLogic.SwitchToCardIndex(0x02) );

	BYTE cbCardIndex[MAX_INDEX];
	m_GameLogic.SwitchToCardIndex( cbCardData,cbCardCount,cbCardIndex );

	CChiHuRight chr;
	DWORD dw1 = GetTickCount();
	BYTE cbHu=0;
	
	BYTE byResult = m_GameLogic.AnalyseChiHuCard( cbCardIndex,NULL,0,cbCurrentCard,chr,cbHu);
	*/
	//DWORD dw2 = GetTickCount();
	//DWORD dw = dw2-dw1;
	//TCHAR szBuffer[32] = TEXT("");
	//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),dw );
	//MessageBox( szBuffer );
	//end test

	/*BYTE byCard[] = {
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,					
			0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
			0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09
	};
	BYTE cbHuaCard[]={0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
	BYTE cbGangCard[]={0x31,0x31,0x31,0x31};
	BYTE byCardCount = 14;

		m_GameClientView.m_HandCardControl.SetDisplayItem(true);
		m_GameClientView.m_HandCardControl.SetPositively(true);
		m_GameClientView.m_HandCardControl.SetCardData(byCard,MAX_COUNT-1-3-3-3-3,byCard[MAX_COUNT-1]);
		for( BYTE i = 0; i < GAME_PLAYER; i++ )
		{
			m_GameClientView.m_HeapCard[i].SetHeapCardInfo(5,5);
			//m_TableCard[i].SetCardData(byCard,byCardCount-3-3-3-3);
			m_GameClientView.m_DiscardCard[i].SetCardData(byCard,15);

			m_GameClientView.m_WeaveCard[i][0].SetUserAction(WIK_GANG);
			m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(false);
			m_GameClientView.m_WeaveCard[i][0].SetCardData(cbGangCard,4);

			m_GameClientView.m_WeaveCard[i][1].SetUserAction(WIK_GANG);
			m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(false);
			m_GameClientView.m_WeaveCard[i][1].SetCardData(cbGangCard,4 );

			m_GameClientView.m_WeaveCard[i][2].SetUserAction( WIK_GANG );
			m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
			m_GameClientView.m_WeaveCard[i][2].SetCardData( cbGangCard,4 );

			m_GameClientView.m_WeaveCard[i][3].SetUserAction(WIK_GANG );
			m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
			m_GameClientView.m_WeaveCard[i][3].SetCardData( cbGangCard,4 );
			
			



			m_GameClientView.m_WindCard[i].SetCardData(cbHuaCard,8);
			if( i < 3 )
				m_GameClientView.m_TableCard[i].SetCardData(cbHuaCard,8);
		}*/
		


	return true;
}

//���ÿ��
bool CGameClientDlg::OnResetGameEngine()
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
	m_bMustWanTong=true;

	//������Ϣ
	m_cbEnjoinCardCount = 0;
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));

	
	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	return true;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	////��������
	//CGameOption GameOption;
	//GameOption.m_bEnableSound=IsEnableSound();
	//GameOption.m_bAllowLookon = IsAllowUserLookon();
	//GameOption.m_bEanbleAnimate = m_GameClientView.IsEnableAnimate();
	//
	////��������
	//if (GameOption.DoModal()==IDOK)
	//{
	//	EnableSound(GameOption.m_bEnableSound);
	//	AllowUserLookon(0,GameOption.m_bAllowLookon);
	//	m_GameClientView.EnableAnimate(GameOption.m_bEanbleAnimate);
	//	m_DirectSound[0].EnableSound(GameOption.m_bEnableSound);
	//	m_DirectSound[1].EnableSound(GameOption.m_bEnableSound);
	//	m_DirectSound[2].EnableSound(GameOption.m_bEnableSound);

	//}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
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
				PostMessage(WM_CLOSE);
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
				/*	if( m_cbListenStatus == 0 && m_bStustee==false && ++m_wTimeOutCount>=MAX_TIME_OUT )
					{
						m_wTimeOutCount = 0;
						OnStusteeControl(TRUE,0);
						m_pIStringMessage->InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
					}
					*/

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
                            BYTE i=0;
							for (i=0;i<MAX_INDEX;i++)
							{
								//����Ч��
								if (m_cbCardIndex[i]==0) continue;
								if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false) 
									continue;

								//���ñ���
								cbCardData=m_GameLogic.SwitchToCardData(i);
							}
							if(i==MAX_INDEX)
							{
								m_pIStringMessage->InsertSystemString(_T("�����ͷǷ���"));
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

bool CGameClientDlg::OnEventGameClockKill(WORD t)
{
	return true;
}

//�Թ�״̬
bool CGameClientDlg::OnEventLookonMode(void * pBuffer, WORD wDataSize)
{
	//�˿˿���
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//������Ϣ
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			m_GameClientView.StopMoveCard();
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
			m_GameClientView.StopMoveCard();
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
			m_GameClientView.StopMoveCard();
			return OnSubReplaceCard( pBuffer,wDataSize );
		}
	//case SUB_S_WANTONG:		//�Ƿ���ͬ	
	//	{
 //           return OnSubWanTong(pBuffer,wDataSize);
	//	}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			////�Ƿ���������
			//if( !IsEnableSound() )
			//{
			//	m_DirectSound[0].EnableSound(FALSE);
			//	m_DirectSound[1].EnableSound(FALSE);
			//	m_DirectSound[2].EnableSound(FALSE);
			//}

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			//�й�����
			for (int i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
			}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				//m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
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

			////�Ƿ���������
			//if( !IsEnableSound() )
			//{
			//	m_DirectSound[0].EnableSound(FALSE);
			//	m_DirectSound[1].EnableSound(FALSE);
			//	m_DirectSound[2].EnableSound(FALSE);
			//}

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
			//m_bMustWanTong=pStatusPlay->bMustWanTong;

			//�û�����
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				//const tagUserData * pUserData=GetUserData(i);
                IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(i);
				ASSERT( pUserData != NULL );
				if( pUserData == NULL ) 
				{
					m_szAccounts[i][0] = 0;
					continue;
				}
				lstrcpy( m_szAccounts[i],pUserData->GetNickName());
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
			/*if(pStatusPlay->bTrustee[wMeChairID])
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
				*/

			//����Ч��
			if(pStatusPlay->wOutCardUser != INVALID_CHAIR)
				m_GameClientView.SetDiscUser(SwitchViewChairID(pStatusPlay->wOutCardUser));

			//�˿˱���
			//m_cbEnjoinCardCount = pStatusPlay->cbEnjoinCardCount;
			//CopyMemory(m_cbEnjoinCardData,pStatusPlay->cbEnjoinCardData,sizeof(m_cbEnjoinCardData));
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//�����ƾ�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<4;j++)
				{
					if(m_GameClientView.m_WeaveCard[i][j].GetCardCount()==4)
					{
						for (BYTE t=j+1;t<4;t++)
						{
							m_GameClientView.m_WeaveCard[i][t].OffPoint(i==2?true:false);
						}
					}

				}
			}
			

			//����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wOperateViewID = SwitchViewChairID(i);
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					
					BYTE cbWeaveCard[4];
					CopyMemory(cbWeaveCard,m_WeaveItemArray[i][j].cbCardData,cbWeaveCardCount);

					m_GameLogic.SortCardList(cbWeaveCard,cbWeaveCardCount);

					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					if ((cbWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i)) 
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
					m_GameClientView.m_WeaveCard[wOperateViewID][j].SetUserAction(cbWeaveKind);

				}
			}

			//�û��˿�
			if (m_wCurrentUser==wMeChairID)
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
				//m_GameClientView.m_WindCard[wViewChairID[i]].SetCardData(pStatusPlay->cbHuaCardData[i],pStatusPlay->cbHuaCardCount[i]);

			}

			//��������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				//m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			}

			//������Ϣ
			m_GameClientView.SetHeapCardInfo( SwitchViewChairID(pStatusPlay->wHeapHead/*wHeapHand*/),SwitchViewChairID(pStatusPlay->wHeapTail) );

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
				SetGameClock(m_wCurrentUser,TIME_OPERATE_CARD,wTimeCount);
			}
			else if( m_wReplaceUser == wMeChairID && !IsLookonMode() )
				SetTimer( IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL );

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
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//�û�����
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		//const tagUserData * pUserData=GetUserData(i);
        IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(i);
		ASSERT( pUserData != NULL );
		if( pUserData == NULL ) 
		{
			m_szAccounts[i][0] = 0;
			continue;
		}
		lstrcpy( m_szAccounts[i],pUserData->GetNickName());
	}

	//����״̬
	SetGameStatus(GS_MJ_PLAY);

	//���ñ���
	CopyMemory( &m_GameStart,pGameStart,sizeof(m_GameStart) );
	m_wBankerUser = m_GameStart.wBankerUser;
	//m_bMustWanTong=m_GameStart.bMustWanTong;

	//�Թ۽���
	if (IsLookonMode()==true)
	{
		//���ñ���
		m_cbListenStatus = 0;
		m_bWillHearStatus = false;

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
			m_GameClientView.m_WindCard[i].SetCardData(NULL,0);

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

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//ҡɫ�Ӷ���
	m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,(WORD)(pGameStart->lSiceCount>>16),(WORD)pGameStart->lSiceCount);
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;

	m_GameClientView.SetCurrentUser( INVALID_CHAIR );
	if( IsLookonMode() || pOutCard->wOutCardUser != wMeChairID )
	{
		//��������
		KillGameClock(IDI_OPERATE_CARD); 

		//��������
		PlayCardSound(pOutCard->wOutCardUser,pOutCard->cbOutCardData);

		//���ƽ���
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);
		
		//�����˿�
		if (wOutViewChairID==MYSELF_VIEW_ID)
		{
			//ɾ���˿�
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData) )
			{
				ASSERT( FALSE );
				return false;
			}

			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}

		//������ƶ���
		tagMoveCardItem mci;
		mci.bReplace=false;
		mci.bOutCard = true;
		mci.cbCardData = pOutCard->cbOutCardData;
		mci.wChairId = pOutCard->wOutCardUser;
		mci.wViewChairId = SwitchViewChairID( pOutCard->wOutCardUser );
		m_GameClientView.OnUserMoveCard( mci );
		//��ʼ���ƶ���
		m_GameClientView.BeginMoveCard();
	}

	//�˿�����
	if (GetMeChairID()==pOutCard->wOutCardUser)
	{
		m_cbEnjoinCardCount=0;
		ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));

	}
	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CopyMemory( &m_SendCard,pBuffer,sizeof(m_SendCard) );

	//Ч�鷢������
	ASSERT( m_SendCard.cbCardData != 0 );
	if( m_SendCard.cbCardData == 0 ) return false;

	//���췢�ƶ���
	tagMoveCardItem mci;
	mci.bReplace=false;
	mci.bOutCard = false;
	mci.bSendTail = m_SendCard.bTail;
	mci.cbCardData = m_SendCard.cbCardData;
	mci.wChairId = m_SendCard.cbSendCardUser/*wSendCardUser*/;
	mci.wViewChairId = SwitchViewChairID(m_SendCard.cbSendCardUser/*wSendCardUser*/);
	m_GameClientView.OnUserMoveCard( mci );
	//��ʼ���ƶ���
	m_GameClientView.BeginMoveCard();

	//���ŷ�������
	m_DirectSound[1].Play();

	return true;
}

//������ʾ
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

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

	return true;
}

//�������
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

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

			m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetUserAction(WIK_GANG);
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


		for (BYTE j=0;j<4;j++)
		{
			if(m_GameClientView.m_WeaveCard[wOperateViewID][j].GetCardCount()==4 && m_GameClientView.m_WeaveCard[wOperateViewID][j].GetCardData(3)==cbOperateCard[0])
			{
				for (BYTE t=j+1;t<4;t++)
				{
					m_GameClientView.m_WeaveCard[wOperateViewID][t].OffPoint(wOperateViewID==2?true:false);
				}
			}

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
		m_GameLogic.SortCardList(cbWeaveCard,sizeof(cbWeaveCard));
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetUserAction(cbWeaveKind);

		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{
			VERIFY( m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard[0],1) );
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1) )
			{
				ASSERT( FALSE );
				return false;
			}
			//��ֹ����
			if(cbWeaveKind == WIK_LEFT)  //���
			{
				//��ֹ��������
				m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0];
				m_cbEnjoinCardCount++;

				if((cbOperateCard[0]&MASK_VALUE)<7)
				{
					m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0]+3;

					m_cbEnjoinCardCount++;
				}

				/*CString str;
				for (BYTE tt=0;tt<m_cbEnjoinCardCount;tt++)
				{
					str.Format("��ֹ���ƣ�%d\n",m_cbEnjoinCardData[tt]);
					m_pIStringMessage->InsertSystemString(LPCSTR(str));

				}*/
			}
			else if (cbWeaveKind == WIK_CENTER) //�г�
			{
				m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0];

				m_cbEnjoinCardCount++;

				/*CString str;
				for (BYTE tt=0;tt<m_cbEnjoinCardCount;tt++)
				{
					str.Format("��ֹ���ƣ�%d\n",m_cbEnjoinCardData[tt]);
					m_pIStringMessage->InsertSystemString(LPCSTR(str));

				}*/
			}
			else  if(cbWeaveKind == WIK_RIGHT)//�ҳ�
			{
				m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0];
				m_cbEnjoinCardCount++;
				if((cbOperateCard[0]&MASK_VALUE)>3)
				{
					m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0]-3;
					m_cbEnjoinCardCount++;
				}

				/*CString str;
				for (BYTE tt=0;tt<m_cbEnjoinCardCount;tt++)
				{
					str.Format("��ֹ���ƣ�%d\n",m_cbEnjoinCardData[tt]);
					m_pIStringMessage->InsertSystemString(LPCSTR(str));

				}*/
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

			//m_cbActionMask = m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId]);

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

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

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
	ScoreInfo.wProvideUser=pGameEnd->wProvideUser[0];///
	ScoreInfo.cbProvideCard=pGameEnd->cbProvideCard;

	//���û���
	CString strTemp ,strEnd = TEXT("���ֽ���,�ɼ�ͳ��:\r\n");
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		strTemp.Format(_T("%s: %I64d\n"),m_szAccounts[i],pGameEnd->lGameScore[i]);
		strEnd += strTemp;
	}

	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		///strTemp.Format(_T("%s: %I64d��\n"),m_szAccounts[i],pGameEnd->lHu[i]);
		strEnd += strTemp;
	}
	//��Ϣ����
	m_pIStringMessage->InsertSystemString((LPCTSTR)strEnd);

	CChiHuRight chr;
	chr.SetRightData( pGameEnd->dwChiHuRight1/*dwChiHuRight*/,CountArray(pGameEnd->dwChiHuRight1/*dwChiHuRight*/) );
	if( pGameEnd->wProvideUser[0] != INVALID_CHAIR )///
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
	m_GameClientView.UpdateWindow();

	return true;
}

//�û��й�
bool CGameClientDlg::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//��Ϣ����
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		//const tagUserData * pUserData=GetUserData(pTrustee->wChairID);
        IClientUserItem * pUserData = GetTableUserItem(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ѡ�����йܹ���."),pUserData->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ȡ�����йܹ���."),pUserData->GetNickName());
		//InsertSystemString(szBuffer);
        m_pIStringMessage->InsertSystemString(szBuffer);
	}

	return true;
}

//�û�����
bool CGameClientDlg::OnSubListen(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Listen));
	if (wDataSize!=sizeof(CMD_S_Listen)) return false;

	CMD_S_Listen *pListen = (CMD_S_Listen *)pBuffer;
	WORD wViewId = SwitchViewChairID( pListen->wChairId );

	//��������״̬
	if( IsLookonMode() || pListen->wChairId != GetMeChairID() )
	{
		m_GameClientView.SetUserListenStatus( wViewId,true );
	}

	//��������
	PlayActionSound( pListen->wChairId,WIK_LISTEN );

	m_GameClientView.SetOutCardInfo( INVALID_CHAIR,0 );
	m_GameClientView.SetUserAction( wViewId,WIK_LISTEN );

	//����״̬
	m_GameClientView.SetStatusFlag( (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),m_wCurrentUser!=GetMeChairID() );

	return true;
}
//�Ƿ���ͬ
bool CGameClientDlg::OnSubWanTong(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==0);
	m_bMustWanTong=false;

	return true;

}
//���ų�������
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

	//�ж��Ա�
	//const tagUserData* pUserData = GetUserData(wChairID);
    IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(wChairID);
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
			default:
				{
					strSoundName=_T("BU_HUA");
				}

			}
			break;
		}		
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
void CGameClientDlg::PlayActionSound(WORD wChairID,BYTE cbAction)
{
	//�ж��Ա�
	//const tagUserData* pUserData = GetUserData(wChairID);
	IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(wChairID);
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
bool CGameClientDlg::VerdictOutCard(BYTE cbCardData)
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
		for (i=0;i<MAX_INDEX;i++)
		{
			//���Ʒ���
			BYTE cbCurrentCard=m_GameLogic.SwitchToCardData(i);
			BYTE cbHu=0;
			BYTE cbHuCardKind=m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp,m_WeaveItemArray[wMeChairID],cbWeaveCount,cbCurrentCard,chr,cbHu);

			//����ж�
			if (cbHuCardKind!=WIK_NULL)
			{
				break;
			}
		}

		//�����ж�
		return (i!=MAX_INDEX);
	}
	if(cbCardData>=0x38 && cbCardData<=0x3f)
		return true;



	if(m_bMustWanTong==false)
	{
		if(m_cbEnjoinCardCount>0)
		{
			int nTemp=0;
			for(nTemp=0;nTemp<m_cbEnjoinCardCount;nTemp++)
			{
				if(cbCardData == m_cbEnjoinCardData[nTemp])
				{
					break;
				}
			}
			if(nTemp == m_cbEnjoinCardCount)
				return true;
			else
			{
				return false;
			}
		}
		else
			return true;
	}
	else
	{
		//�������Ͳ��ʲô�ƶ����Գ�
		BYTE cbNo=0;
		for(BYTE i=0;i<9;i++)
		{
			cbNo +=m_cbCardIndex[i];
		}
		for(BYTE i=18;i<27;i++)
		{
			cbNo +=m_cbCardIndex[i];

		}
		if(cbNo==0) return true;

		if((cbCardData&MASK_COLOR)==0x00 ||(cbCardData&MASK_COLOR)==0x20)
			return true;
		else
			return false;
	}
	return false;
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
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
		m_GameClientView.m_WindCard[i].SetCardData(NULL,0);

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

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))
		return 0;

	//�����ж�
	if (((m_cbListenStatus>0)||(m_bWillHearStatus==true))||(VerdictOutCard((BYTE)wParam)==false))
	{
		m_pIStringMessage->InsertSystemString(TEXT("�����Ʋ�������Ϸ����!"));
		return 0;
	}
	

	KillGameClock(IDI_OPERATE_CARD);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	BYTE cbOutCardData=(BYTE)wParam;
	if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData) )
	{
		ASSERT( FALSE );
		return 0;
	}

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	ASSERT( (cbCardCount-1)%3==0 );
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);

	//���ý���
	m_GameClientView.InvalidGameView(0,0,0,0);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	if( m_GameClientView.m_DlgListen.m_hWnd )
		m_GameClientView.m_DlgListen.EndDialog(IDCANCEL);

	//������ƶ���
	tagMoveCardItem mci;
	mci.bOutCard = true;
	mci.bReplace=false;
	mci.cbCardData = cbOutCardData;
	mci.wChairId = GetMeChairID();
	mci.wViewChairId = MYSELF_VIEW_ID;
	m_GameClientView.OnUserMoveCard( mci );
	//��ʼ���ƶ���
	m_GameClientView.BeginMoveCard();

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	//��������
	PlayCardSound(GetMeChairID(),cbOutCardData);

	return 0;
}

//�˿˲���
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
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
	cbOperateCode = si.cbActionMask;
	cbOperateCard[0] = si.cbActionCard;
	CopyMemory( &cbOperateCard[1],si.cbCardData,sizeof(BYTE)*2 );

	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//ȡ������ģʽ
	m_GameClientView.m_HandCardControl.SetSelectMode( false );

	//���������
	if( m_bWillHearStatus && (cbOperateCode&WIK_LISTEN) )
	{
		m_bWillHearStatus = false;
		return OnOutCard( cbOperateCard[0],0 );
	}

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
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wTimeOutCount=0;

	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		//m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if( FALSE == (BOOL)wParam ) 
			m_pIStringMessage->InsertSystemString(_T("��ȡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}
	else
	{
		m_bStustee=true;
		//m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if( FALSE == (BOOL)wParam )
			m_pIStringMessage->InsertSystemString(_T("��ѡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}

	return 0;
}

//�ڶ���ҡɫ����Ϣ
LRESULT CGameClientDlg::OnSiceTwo(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	return 0;
}

//ҡɫ�ӽ�����Ϣ
LRESULT CGameClientDlg::OnSiceFinish(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	bool bPlayerMode = (IsLookonMode()==false);

	//�����˿�
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(m_GameStart.cbCardData,cbCardCount,m_cbCardIndex);

	//���ñ���
	m_wCurrentUser=m_GameStart.wCurrentUser;
	m_wReplaceUser = m_GameStart.wReplaceUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	m_cbActionMask = m_GameStart.cbUserAction;
	m_lSiceCount = m_GameStart.lSiceCount;

	WORD wMeChairID=GetMeChairID();

	//���ö���ͷβ
	m_GameClientView.SetHeapCardInfo( SwitchViewChairID(m_GameStart.wHeapHead/*wHeapHand*/),SwitchViewChairID(m_GameStart.wHeapTail) );
	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//�����˿�
		m_GameClientView.m_HeapCard[wViewChairID].SetHeapCardInfo(m_GameStart.cbHeapCardInfo[i][0],m_GameStart.cbHeapCardInfo[i][1]);

		//�û��˿�
		if (wViewChairID!=MYSELF_VIEW_ID)
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			m_GameClientView.m_UserCard[wIndex].SetCardData(MAX_COUNT-1,(i==m_wBankerUser));
		}
		else
		{
			SetHandCardControl(m_cbCardIndex,0x00);
		}
	}
	
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);

	//���Ʋ���
	if ((bPlayerMode==true)&&(m_wReplaceUser==wMeChairID))
	{
		//����ʱ��
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
	}

	//������ʾ,��������
	if ((bPlayerMode==true)&&(m_wCurrentUser==wMeChairID))
	{
		m_GameClientView.SetStatusFlag(true,false);
		
		//m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,NULL,0);

		if( m_cbActionMask != WIK_NULL )
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
	}
	else if( m_wCurrentUser != wMeChairID && m_wCurrentUser != INVALID_CHAIR )
		m_GameClientView.SetStatusFlag(false,true);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//������
	if (bPlayerMode==true)
		ActiveGameFrame();

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//��0
	ZeroMemory( &m_GameStart,sizeof(m_GameStart) );

	return 0;
}

//������Ϣ
bool CGameClientDlg::OnSubReplaceCard(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_ReplaceCard));
	if (wDataSize!=sizeof(CMD_S_ReplaceCard)) return false;

	//��������
	CMD_S_ReplaceCard * pReplaceCard=(CMD_S_ReplaceCard *)pBuffer;

	//�����˿�
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pReplaceCard->wReplaceUser);

	//�����û�
	if ((IsLookonMode()==true)||(pReplaceCard->wReplaceUser!=wMeChairID))
	{
		//��������
		PlayActionSound(pReplaceCard->wReplaceUser,WIK_REPLACE);

		//���ö���
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction( wViewChairID,WIK_REPLACE );

		//�����˿�
		if (wViewChairID==MYSELF_VIEW_ID)
		{
			//ɾ���˿�
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,pReplaceCard->cbReplaceCard) )
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
			WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
			if( m_GameClientView.m_UserCard[wUserIndex].IsHaveCurrentCard() )
				m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
			else
			{
				WORD wCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount();
				m_GameClientView.m_UserCard[wUserIndex].SetCardData(wCardCount-1,false);
			}
		}

		//������ƶ���
		tagMoveCardItem mci;
		mci.bOutCard = true;
		mci.bReplace=true;
		mci.cbCardData = pReplaceCard->cbReplaceCard;
		mci.wChairId = pReplaceCard->wReplaceUser;
		mci.wViewChairId = wViewChairID;
		m_GameClientView.OnUserMoveCard( mci );
		//��ʼ���ƶ���
		m_GameClientView.BeginMoveCard();
	}

	return true;
}

//�����˿�
void CGameClientDlg::SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard)
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
VOID CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_REPLACE_CARD:	//�û�����
		{
			//ɾ��ʱ��
			KillTimer(IDI_REPLACE_CARD);

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
						return;
					}

					//�����˿�
					SetHandCardControl(m_cbCardIndex,0x00);

					//��������
					PlayActionSound(GetMeChairID(),WIK_REPLACE);

					//���ý���
					m_GameClientView.SetUserAction( MYSELF_VIEW_ID,WIK_REPLACE );

					//������ƶ���
					tagMoveCardItem mci;
					mci.bOutCard = true;
					mci.bReplace=true;
					mci.cbCardData = cbReplaceCard;
					mci.wChairId = GetMeChairID();
					mci.wViewChairId = MYSELF_VIEW_ID;
					m_GameClientView.OnUserMoveCard( mci );
					
					//��ʼ���ƶ���
					m_GameClientView.BeginMoveCard();

					//��������
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

//���ƶ������
LRESULT CGameClientDlg::OnOutCardFinish( WPARAM wParam, LPARAM lParam )
{
	WORD wViewId = (WORD)wParam;
	if( IsLookonMode() || wViewId != MYSELF_VIEW_ID )
		m_GameClientView.SetDiscUser( wViewId );

	//��������
	m_DirectSound[0].Play();

	return 0;
}

//���ƶ������
LRESULT CGameClientDlg::OnSendCardFinish( WPARAM wParam, LPARAM lParam )
{
	//���ƴ���
	WORD wViewChairID=(WORD)wParam;
	WORD wChairId = (WORD)(wParam>>16);

	//ȡ�ƽ���
	if (wViewChairID!=MYSELF_VIEW_ID)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		WORD wCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount();
		wCardCount += m_cbWeaveCount[wChairId]*3;
		if( wCardCount == MAX_COUNT-1 )
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
		else
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(wCardCount+1,false);
	}
	else
	{
		//�����˿�
		BYTE cbCardData = (BYTE)lParam;
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData)]++;

		//�����˿�
		SetHandCardControl(m_cbCardIndex,cbCardData);
	}

	//������ڷ���,����ȴ����ƽ���
	if( m_GameClientView.IsMovingCard() ) 
	{
		m_GameClientView.InvalidGameView(0,0,0,0);
		return 0;
	}

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=m_SendCard.wCurrentUser;

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		ActiveGameFrame();

		if( m_cbListenStatus == 0 )
		{
			//m_SendCard.cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID]);
		}

		m_cbActionMask = m_SendCard.cbActionMask;
		m_cbActionCard = m_SendCard.cbCardData;

		//��������
		if (m_SendCard.cbActionMask!=WIK_NULL)
		{
			//���ý���
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
		}
	}

	//���Ʋ���
	if ((IsLookonMode()==false)&&(m_SendCard.wReplaceUser==wMeChairID))
	{
		//����ʱ��
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
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

	return 0;
}

//��Ҳ����ؼ�
LRESULT CGameClientDlg::OnUserAction( WPARAM wParam, LPARAM lParam )
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
		m_GameClientView.m_HandCardControl.SetSelectMode(false);
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
			cbOperateCode = si.cbActionMask;
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
BYTE CGameClientDlg::GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] )
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
				BYTE cbHu=0;
				if( WIK_CHI_HU == m_GameLogic.AnalyseChiHuCard( cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],
					cbCurrentCard,chr,cbHu) )
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
			SelectInfo[cbSelectCount].cbActionMask = WIK_LISTEN;
			SelectInfo[cbSelectCount].cbCardCount = 1;
			SelectInfo[cbSelectCount++].cbCardData[0] = cbCardData[i];
		}
	}
	//����
	else if( wOperateCode&(WIK_LEFT) )
	{
		//Ч��
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		if( m_cbActionMask & WIK_LEFT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].cbActionMask = WIK_LEFT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard+1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+2;
		}
	}
	else if( wOperateCode&(WIK_CENTER) )
	{
		//Ч��
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		if( m_cbActionMask & WIK_CENTER )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].cbActionMask = WIK_CENTER;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+1;
		}
	}
	else if( wOperateCode&(WIK_RIGHT) )
	{
		//Ч��
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;

		if( m_cbActionMask & WIK_RIGHT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].cbActionMask = WIK_RIGHT;
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
		SelectInfo[cbSelectCount].cbActionMask = WIK_PENG;
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
				SelectInfo[cbSelectCount].cbActionMask = WIK_GANG;				
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
			SelectInfo[cbSelectCount].cbActionMask = WIK_GANG;
			SelectInfo[cbSelectCount].cbCardCount = 3;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
			SelectInfo[cbSelectCount].cbCardData[1] = m_cbActionCard;
			SelectInfo[cbSelectCount++].cbCardData[2] = m_cbActionCard;
		}
	}

	return cbSelectCount;
}

//��Ҳ����ؼ�
LRESULT CGameClientDlg::OnUserMove( WPARAM wParam, LPARAM lParam )
{
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);

	if(cbOperateCode==WIK_NULL)
	{
		m_GameClientView.m_HandCardControl.SetSelectMode( false );
		m_GameClientView.m_HandCardControl.SetAllCardGray( false );
		m_GameClientView.UpdateWindow();

		return 0;
	}

	//��ȡѡ�����
	tagSelectCardInfo sci[MAX_WEAVE];
	BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );

	//���ò����¼�
	m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );
	
	return 0;
}
//��������
LRESULT CGameClientDlg::OnSound(WPARAM wParam,LPARAM lParam)
{
	m_DirectSound[2].Play();
	return 0;
}
//////////////////////////////////////////////////////////////////////////
