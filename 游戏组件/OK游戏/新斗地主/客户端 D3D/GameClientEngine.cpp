#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸʱ��
#define IDI_OUT_CARD				200									//���ƶ�ʱ��
#define IDI_START_GAME				201									//��ʼ��ʱ��
#define IDI_CALL_SCORE				202									//�зֶ�ʱ��

//��Ϸʱ��
#define IDI_MOST_CARD				300									//���ʱ��
#define IDI_LAST_TURN				301									//���ֶ�ʱ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)

	//ϵͳ��Ϣ
	ON_WM_TIMER()

	//��Ϸ��Ϣ
	ON_MESSAGE(IDM_START,OnMessageStart)
	ON_MESSAGE(IDM_OUT_CARD,OnMessageOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnMessagePassCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnMessageOutPrompt)
	ON_MESSAGE(IDM_CALL_SCORE,OnMessageCallScore)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnMessageLeftHitCard)
	ON_MESSAGE(IDM_SORT_HAND_CARD,OnMessageSortHandCard)
	ON_MESSAGE(IDM_LAST_TURN_CARD,OnMessageLastTurnCard)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnMessageTrusteeControl)
	ON_MESSAGE(IDM_SEARCH_CARD,OnMessageSearchCard)


	//������Ϣ
	ON_MESSAGE(WM_ENABLE_HISTORY,OnMessageEnableHistory)
	ON_MESSAGE(IDM_OUT_CARD_FINISH,OnMessageOutCardFinish)
	ON_MESSAGE(IDM_DISPATCH_FINISH,OnMessageDispatchFinish)
	ON_MESSAGE(IDM_REVERSAL_FINISH,OnMessageReversalFinish)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��������
	m_bTrusteeCount = 0;
	m_bTrustee=false;
	m_bLastTurn=false;
	m_cbSortType=ST_ORDER;
	m_bBackGroundSound = false;
	m_bCheatRight = false;

	//ʱ�䶨��
	m_cbTimeOutCard=0;
	m_cbTimeCallScore=0;
	m_cbTimeStartGame=0;
	m_cbTimeHeadOutCard=0;

	//��Ϸ����
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_cbBankerScore = 0;
	m_wCurrentUser=INVALID_CHAIR;
	m_wMostCardUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	//��������
	m_cbSearchResultIndex = 0;
	ZeroMemory( &m_SearchCardResult,sizeof(m_SearchCardResult) );
	m_nCurSearchType = -1;
	m_cbEachSearchIndex = 0;
	ZeroMemory( &m_EachSearchResult,sizeof(m_EachSearchResult) );

	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��������
bool CGameClientEngine::OnInitGameEngine()
{
	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	//��������
/*	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	if ( pGlobalUnits->m_bAllowBackGroundSound && !pGlobalUnits->m_bMuteStatuts)
	{
		m_bBackGroundSound = true;
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_MUSIC"));
	}
*/
	return true;
}

//���ú���
bool CGameClientEngine::OnResetGameEngine()
{
	//ɾ��ʱ��
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_LAST_TURN);

	//��������
	m_bTrusteeCount = 0;
	m_bTrustee=false;
	m_bLastTurn=false;
	m_cbSortType=ST_ORDER;
	m_bCheatRight = false;

	//ʱ�䶨��
	m_cbTimeOutCard=0;
	m_cbTimeCallScore=0;
	m_cbTimeStartGame=0;
	m_cbTimeHeadOutCard=0;

	//��Ϸ����
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_cbBankerScore = 0;
	m_wCurrentUser=INVALID_CHAIR;
	m_wMostCardUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	//��������
	m_cbSearchResultIndex = 0;
	ZeroMemory( &m_SearchCardResult,sizeof(m_SearchCardResult) );
	m_nCurSearchType = -1;
	m_cbEachSearchIndex = 0;
	ZeroMemory( &m_EachSearchResult,sizeof(m_EachSearchResult) );


	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

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
	switch (wClockID)
	{
	case IDI_OUT_CARD:		//�û�����
		{
			if (IsLookonMode() ) return true;

			//�Զ�����
			if ( wChairID == GetMeChairID() &&
				( nElapse <= 0 || ( m_bTrustee && (int)nElapse < (int)m_cbTimeOutCard - 1 ) ) )
			{

				//�Զ��й�
				if ( !m_bTrustee && nElapse == 0 && m_bTrusteeCount < 3 )
				{
					m_bTrusteeCount++;
					if ( m_bTrusteeCount == 3 )
					{
						OnMessageTrusteeControl(0,0);
					}
				}

				//�Զ�����
				if( m_bTrustee || m_cbTurnCardCount == 0 )
					AutomatismOutCard();
				else 
					OnMessagePassCard(0,0);

				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_START_GAME:	//��Ϸ��ʼ
		{
			//�ر���Ϸ
			if ( nElapse <= 0 && !IsLookonMode() && wChairID == GetMeChairID() )
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_CALL_SCORE:	//�û��з�
		{
			////������������
			////m_GameClientView.InvalidGameView(0,0,0,0);

			//�Զ�����
			if (( nElapse <= 0 ) || ( m_bTrustee && (int)nElapse < (int)m_cbTimeOutCard - 1 ))
			{
				if ( !IsLookonMode() && wChairID==GetMeChairID() ) 
					OnMessageCallScore(255,255);
				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}
	return false;
}

//�Թ���Ϣ
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//�Թۿ���
	bool bAllowLookon=IsAllowLookon();

	//�˿˿���
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetDisplayItem(bAllowLookon);

	m_GameClientView.m_btSortCard.EnableWindow(bAllowLookon?TRUE:FALSE);
	if( bAllowLookon )
		UpdateButtonControl();
	else
	{
		m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);
	}

	if ( IsLookonMode() )
	{
		m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
		m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

		m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
		m_GameClientView.m_btTrustee.EnableWindow(FALSE);
		m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
		m_GameClientView.m_btSortCard.EnableWindow(FALSE);
	}

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);


	return true;
}

//��Ϸ��Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_CALL_SCORE:		//�û��з�
		{
			return OnSubCallScore(pData,wDataSize);
		}
	case SUB_S_BANKER_INFO:		//ׯ����Ϣ
		{
			return OnSubBankerInfo(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:		//�û�����
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_GAME_CONCLUDE:	//��Ϸ����
		{
			return OnSubGameConclude(pData,wDataSize);
		}
	case SUB_S_SET_BASESCORE:	//���û���
		{
			//Ч������
			ASSERT(wDataSize==sizeof(SCORE));
			if(wDataSize!=sizeof(SCORE)) return false;

			SCORE lCellScore=*(SCORE*)pData;
			m_GameClientView.SetCellScore(lCellScore);
			return true;
		}
	case SUB_S_CHEAT_CARD:		//��������	
		{
			return OnSubCheatCard(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//���ý���
			m_GameClientView.m_btScore.ShowWindow(!IsDistributeMode());
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);


			//ʱ�䶨��
			m_cbTimeOutCard=pStatusFree->cbTimeOutCard;
			m_cbTimeCallScore=pStatusFree->cbTimeCallScore;
			m_cbTimeStartGame=pStatusFree->cbTimeStartGame;
			m_cbTimeHeadOutCard=pStatusFree->cbTimeHeadOutCard;

			//�������
			if (IsLookonMode()==false)
			{
				//�йܰ�ť
				m_GameClientView.m_btTrustee.ShowWindow(true);
				m_GameClientView.m_btCancelTrustee.ShowWindow(false);
				m_GameClientView.m_btTrustee.EnableWindow(true);

				//��ʼ����
				if (GetMeUserItem()->GetUserStatus()!=US_READY)
				{
					m_GameClientView.m_btStart.ShowWindow(TRUE);
					PerformAutoStart();
				}
			}

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusFree->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusFree->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			//�Թ����ý���
			if ( IsLookonMode() )
			{
				m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

				m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
				m_GameClientView.m_btSortCard.EnableWindow(FALSE);
			}

			return true;
		}
	case GAME_SCENE_CALL:	//�з�״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusCall));
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;

			//ֹͣ����   //�γ���������,�������BUG ��֪Ϊʲô�з�״̬һ���뷿��ͷ��͹�����!
			m_GameClientView.StopDispatchCard();

			//��������
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

			//���ñ���
			m_wCurrentUser=pStatusCall->wCurrentUser;

			//ʱ�䶨��
			m_cbTimeOutCard=pStatusCall->cbTimeOutCard;
			m_cbTimeCallScore=pStatusCall->cbTimeCallScore;
			m_cbTimeStartGame=pStatusCall->cbTimeStartGame;
			m_cbTimeHeadOutCard=pStatusCall->cbTimeHeadOutCard;

			//�˿�����
			WORD wMeChairID=GetMeChairID();
			for (WORD i=0;i<GAME_PLAYER;i++) m_cbHandCardCount[i]=NORMAL_COUNT;
			CopyMemory(m_cbHandCardData[wMeChairID],pStatusCall->cbHandCardData,m_cbHandCardCount[wMeChairID]*sizeof(BYTE));

			//�з�״̬
			m_GameClientView.SetWaitCallScore(true);
			m_GameClientView.m_BackCardControl.SetCardData(3);

			//���ý���
			m_GameClientView.m_btScore.ShowWindow(!IsDistributeMode());
			m_GameClientView.SetCellScore(pStatusCall->lCellScore);

	
			//�зֽ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (pStatusCall->cbScoreInfo[i]!=0)
				{
					BYTE cbCallScore=pStatusCall->cbScoreInfo[i];
					m_GameClientView.SetUserCallScore(SwitchViewChairID(i),pStatusCall->cbScoreInfo[i]);
				}
			}

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ȡλ��
				WORD wViewChairID=SwitchViewChairID(i);

				//�����˿�
				if (wViewChairID==MYSELF_VIEW_ID)
				{
					m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],m_cbSortType);
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[i],m_cbHandCardCount[i]);
				}
				else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			}

			//�з�����
			if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
			{
				//��ʾ��ť
				m_GameClientView.m_btCallScore1.ShowWindow(true);
				m_GameClientView.m_btCallScore2.ShowWindow(true);
				m_GameClientView.m_btCallScore3.ShowWindow(true);
				m_GameClientView.m_btCallScoreNone.ShowWindow(true);

				//���ð�ť
				m_GameClientView.m_btCallScore1.EnableWindow((pStatusCall->cbBankerScore==0)?true:false);
				m_GameClientView.m_btCallScore2.EnableWindow((pStatusCall->cbBankerScore<=1)?true:false);
				m_GameClientView.m_btCallScore3.EnableWindow((pStatusCall->cbBankerScore<=2)?true:false);

				UpdateButtonControl();
			}

			//��ҿ���
			if (IsLookonMode()==false)	
			{
				m_GameClientView.m_btTrustee.ShowWindow(true);
				m_GameClientView.m_btCancelTrustee.ShowWindow(false);
				m_GameClientView.m_btTrustee.EnableWindow(true);
				m_GameClientView.m_btSortCard.EnableWindow(true);
			}

			//��ʾ����
			bool bDisplayItem=((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetDisplayItem(bDisplayItem);

			//ʣ������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_cbHandCardCount[i]<=2)
				{
					m_GameClientView.SetUserCountWarn(SwitchViewChairID(i),true);
				}
			}

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusCall->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusCall->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			//����ʱ��
			SetGameClock(m_wCurrentUser,IDI_CALL_SCORE,m_cbTimeCallScore);

			//�����ǰ����
			m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

			//�Թ����ý���
			if ( IsLookonMode() )
			{
				m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

				m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
				m_GameClientView.m_btSortCard.EnableWindow(FALSE);
			}

			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��������
			WORD wMeChairID=GetMeChairID();
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���ñ���
			m_cbBombCount=pStatusPlay->cbBombCount;
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_cbBankerScore = pStatusPlay->cbBankerScore;
			m_wCurrentUser=pStatusPlay->wCurrentUser;

			//ʱ�䶨��
			m_cbTimeOutCard=pStatusPlay->cbTimeOutCard;
			m_cbTimeCallScore=pStatusPlay->cbTimeCallScore;
			m_cbTimeStartGame=pStatusPlay->cbTimeStartGame;
			m_cbTimeHeadOutCard=pStatusPlay->cbTimeHeadOutCard;

			//���Ʊ���
			m_cbTurnCardCount=pStatusPlay->cbTurnCardCount;
			CopyMemory(m_cbTurnCardData,pStatusPlay->cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿�����
			CopyMemory(m_cbHandCardCount,pStatusPlay->cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(m_cbHandCardData[wMeChairID],pStatusPlay->cbHandCardData,m_cbHandCardCount[wMeChairID]*sizeof(BYTE));

			//��������
			WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
			m_GameClientView.m_HandCardControl[wViewChairID].SetBackGround(CARD_LAND);

			//���ý���
			m_GameClientView.SetBombCount(m_cbBombCount);
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);

		

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ȡλ��
				WORD wViewChairID=SwitchViewChairID(i);

				//�����˿�
				if (wViewChairID==MYSELF_VIEW_ID)
				{
					m_GameLogic.SortCardList(m_cbHandCardData[wMeChairID],m_cbHandCardCount[i],m_cbSortType);
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[wMeChairID],m_cbHandCardCount[i]);
				}
				else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
			}

			//ׯ������
			m_GameClientView.SetBankerUser(wViewChairID);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerScore);

			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(!IsDistributeMode());
			m_GameClientView.m_BackCardControl.SetCardData(pStatusPlay->cbBankerCard,3);

			//���ƽ���
			if (pStatusPlay->wTurnWiner!=INVALID_CHAIR)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wTurnWiner);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbTurnCardData,m_cbTurnCardCount);
			}

			//��ҿ���
			if (IsLookonMode()==false)	
			{
				m_GameClientView.m_btTrustee.ShowWindow(true);
				m_GameClientView.m_btCancelTrustee.ShowWindow(false);
				m_GameClientView.m_btTrustee.EnableWindow(true);
				m_GameClientView.m_btSortCard.EnableWindow(true);

				UpdateButtonControl();
			}

			//��������
			bool bPositively=(IsLookonMode()==false);
			bool bDisplayItem=((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(bPositively);
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetDisplayItem(bDisplayItem);

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
			{
				//��ʾ��ť
				m_GameClientView.m_btOutCard.ShowWindow(true);
				m_GameClientView.m_btPassCard.ShowWindow(true);
				m_GameClientView.m_btOutPrompt.ShowWindow(true);

				//���ð�ť
				m_GameClientView.m_btPassCard.EnableWindow((m_cbTurnCardCount>0)?true:false);
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);

				//��������
				if( pStatusPlay->wTurnWiner == wMeChairID )
				{
					m_GameLogic.SearchOutCard( m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID],NULL,0,&m_SearchCardResult );
				}
				else
				{
					m_GameLogic.SearchOutCard( m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID],m_cbTurnCardData,
						m_cbTurnCardCount,&m_SearchCardResult );
				}
			}
			//else if ( m_wCurrentUser != INVALID_CHAIR )
			//{
			//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			//}

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusPlay->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusPlay->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			//����ʱ��
			SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeOutCard);

			//�����ǰ����
			m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

			//�Թ����ý���
			if ( IsLookonMode() )
			{
				m_GameClientView.m_btCardPrompt1.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt2.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt3.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt4.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt5.EnableWindow(FALSE);
				m_GameClientView.m_btCardPrompt6.EnableWindow(FALSE);

				m_GameClientView.m_btCancelTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
				m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
				m_GameClientView.m_btSortCard.EnableWindow(FALSE);
			}

			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//����״̬
	SetGameStatus(GAME_SCENE_CALL);

	//��������
	m_cbSearchResultIndex = 0;
	ZeroMemory( &m_SearchCardResult,sizeof(m_SearchCardResult) );
	m_nCurSearchType = -1;
	m_cbEachSearchIndex = 0;
	ZeroMemory( &m_EachSearchResult,sizeof(m_EachSearchResult) );

	//��Ϸ����
	m_bTrusteeCount = 0;
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));
	m_cbBankerScore = 0;
	m_wCurrentUser=pGameStart->wCurrentUser;

	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//��ʷ��¼
	m_HistoryCard.ResetData();

	//����ׯ��
	m_GameClientView.SetBankerScore(0);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);

	//�ɼ�����
	m_GameClientView.m_ScoreControl.CloseControl();

	//������
	m_GameClientView.ShowNoteCard(false);	
	m_GameClientView.m_NoteCard.SetCardInfo();

	//״̬����
	m_GameClientView.SetBombCount(0);
	m_GameClientView.SetWaitCallScore(false);
	m_GameClientView.SetUserCallScore(INVALID_CHAIR,0);
	m_GameClientView.SetUserPassState(INVALID_CHAIR,false);
	m_GameClientView.SetUserCountWarn(INVALID_CHAIR,false);

	//����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡλ��
		WORD wViewChairID=SwitchViewChairID(i);

		//�˿˱���
		m_GameClientView.m_HandCardControl[i].SetBackGround(CARD_BOOR);
		m_GameClientView.m_UserCardControl[i].SetBackGround(CARD_BOOR);

		//�����˿�
		if (wViewChairID==MYSELF_VIEW_ID)
		{
			bool bShowCard=((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetPositively(false);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(bShowCard);
		}
		else
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(false);
			////�����������û����,��һ�°�?!
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardDistance(DEF_X_DISTANCE,11,0);
		}
	}

	//�����˿�
	m_GameClientView.m_BackCardControl.SetCardData(3);
	m_GameClientView.m_BackCardControl.SetBackGround(CARD_BOOR);

	//�����˿�
	WORD wMeChairID=GetMeChairID();
	for (WORD i=0;i<GAME_PLAYER;i++) m_cbHandCardCount[i]=CountArray(pGameStart->cbCardData);
	CopyMemory(m_cbHandCardData[wMeChairID],pGameStart->cbCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//���ƶ���
	WORD wViewStartUser=SwitchViewChairID(pGameStart->wStartUser);
	m_GameClientView.ShowDispatchCard(m_cbHandCardData[wMeChairID],wViewStartUser);

	//��������
	if ((IsLookonMode()==false)&&(m_bTrustee==false)) ActiveGameFrame();

	//���½���
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	m_GameClientView.m_btStart.ShowWindow(false);
	

	return true;
}

//�û��з�
bool CGameClientEngine::OnSubCallScore(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_CallScore));
	if (wDataSize!=sizeof(CMD_S_CallScore)) return false;

	//��������
	CMD_S_CallScore * pCallScore=(CMD_S_CallScore *)pData;

	//ֹͣ����
	m_GameClientView.StopDispatchCard();

	//�зֽ���
	WORD wViewChairID=SwitchViewChairID(pCallScore->wCallScoreUser);
	m_GameClientView.SetUserCallScore(wViewChairID,pCallScore->cbUserCallScore);

	if ( pCallScore->cbUserCallScore != 255 )
	{
		m_cbBankerScore = max(m_cbBankerScore, pCallScore->cbUserCallScore);
	}

	WORD wMeChairID=GetMeChairID();

	//�����˿�
	m_GameLogic.SortCardList(m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID],ST_ORDER);

	//�����˿�
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID]);
	
	//�з�����
	if ((IsLookonMode()==false)&&(pCallScore->wCurrentUser==GetMeChairID()))
	{
		//������
		if (m_bTrustee==false) ActiveGameFrame();

		//��ʾ��ť
		m_GameClientView.m_btCallScore1.ShowWindow(true);
		m_GameClientView.m_btCallScore2.ShowWindow(true);
		m_GameClientView.m_btCallScore3.ShowWindow(true);
		m_GameClientView.m_btCallScoreNone.ShowWindow(true);

		//���ð�ť
		m_GameClientView.m_btCallScore1.EnableWindow((pCallScore->cbCurrentScore==0)?true:false);
		m_GameClientView.m_btCallScore2.EnableWindow((pCallScore->cbCurrentScore<=1)?true:false);
		m_GameClientView.m_btCallScore3.EnableWindow((pCallScore->cbCurrentScore<=2)?true:false);
	}

	//��ȡ�û�
	WORD wCallScoreUser=pCallScore->wCallScoreUser;
	IClientUserItem * pIClientUserItem=GetTableUserItem(wCallScoreUser);

	//��������
	switch (pCallScore->cbUserCallScore)
	{
	case 1:
		{
			BYTE cbGender=pIClientUserItem->GetGender();
			PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("SCORE_1_GIRL"):TEXT("SCORE_1_BOY"));
			break;
		}
	case 2:
		{
			BYTE cbGender=pIClientUserItem->GetGender();
			PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("SCORE_2_GIRL"):TEXT("SCORE_2_BOY"));
			break;
		}
	case 3:
		{
			BYTE cbGender=pIClientUserItem->GetGender();
			PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("SCORE_3_GIRL"):TEXT("SCORE_3_BOY"));
			break;
		}
	case 0xFF:
		{
			BYTE cbGender=pIClientUserItem->GetGender();
			PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?TEXT("SCORE_NONE_GIRL"):TEXT("SCORE_NONE_BOY"));
			break;
		}
	}

	//����ʱ��
	if (pCallScore->wCurrentUser!=INVALID_CHAIR)
	{
		SetGameClock(pCallScore->wCurrentUser,IDI_CALL_SCORE,m_cbTimeCallScore);

		//�����ǰ����
		m_GameClientView.m_UserCardControl[SwitchViewChairID(pCallScore->wCurrentUser)].SetCardData(0);
		m_GameClientView.SetUserPassState(SwitchViewChairID(pCallScore->wCurrentUser), false);
		m_GameClientView.SetUserCallScore(SwitchViewChairID(pCallScore->wCurrentUser), 0);

		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return true;
}

//ׯ����Ϣ
bool CGameClientEngine::OnSubBankerInfo(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_BankerInfo));
	if (wDataSize!=sizeof(CMD_S_BankerInfo)) return false;

	//��������
	CMD_S_BankerInfo * pBankerInfo=(CMD_S_BankerInfo *)pData;

	//����״̬
	SetGameStatus(GAME_SCENE_PLAY);

	//���ñ���
	m_wBankerUser = pBankerInfo->wBankerUser;
	m_wCurrentUser = pBankerInfo->wCurrentUser;
	m_cbBankerScore = pBankerInfo->cbBankerScore;

	//���ý���
	m_GameClientView.SetWaitCallScore(false);
	m_GameClientView.ShowBankerCard(pBankerInfo->cbBankerCard);
	m_GameClientView.SetBankerScore(m_cbBankerScore);

	m_GameClientView.ShowNoteCard(true);

	return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pData;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->cbCardData);

	//Ч������
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE)))) return false;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	BYTE cbTurnCardType=m_GameLogic.GetCardType(pOutCard->cbCardData,pOutCard->cbCardCount);

		//���¼���
	m_GameClientView.m_NoteCard.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount);
	//ɾ��ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);

	//��ʷ����
	if (m_cbTurnCardCount==0)
	{
		//�û��˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			m_HistoryCard.OnEventSaveData(i);

			//��������
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);
			}
		}

		//������־
		m_GameClientView.SetUserPassState(INVALID_CHAIR,false);
	}

	//���ƶ���
	if ((IsLookonMode()==true)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//��ǰ�˿�
		SwitchToCurrentCard();

		//��������
		BYTE cbCardType=m_GameLogic.GetCardType(pOutCard->cbCardData,pOutCard->cbCardCount);
		if ( m_cbTurnCardCount != 0 
			&& !((cbCardType == CT_THREE_TAKE_ONE && pOutCard->cbCardCount > 4) || (cbCardType == CT_THREE_TAKE_TWO && pOutCard->cbCardCount > 5))
			&& !( cbCardType == CT_MISSILE_CARD )
			&& !( cbCardType == CT_BOMB_CARD ))
		{
			//��������
			LPCTSTR pszSoundBoy=TEXT("OUT_CARD_BOY");
			LPCTSTR pszSoundGirl=TEXT("OUT_CARD_GIRL");
			IClientUserItem * pIClientUserItem=GetTableUserItem(pOutCard->wOutCardUser);

			//��������
			BYTE cbGender=pIClientUserItem->GetGender();
			PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?pszSoundGirl:pszSoundBoy);
		}
		else
		{
			//��������
			PlayOutCardSound(pOutCard->wOutCardUser,pOutCard->cbCardData,pOutCard->cbCardCount);
		}

		//��������
		if ( (cbCardType == CT_THREE_TAKE_ONE && pOutCard->cbCardCount > 4)
			|| (cbCardType == CT_THREE_TAKE_TWO && pOutCard->cbCardCount > 5) )
		{
			m_GameClientView.ShowCartoonPlan(true);
			PlayGameSound(AfxGetInstanceHandle(),TEXT("PLANE"));	
		}
		else if ( cbCardType == CT_MISSILE_CARD )
		{
			m_GameClientView.ShowCartoonRocket(true);
			PlayGameSound(AfxGetInstanceHandle(),TEXT("PLANE"));	
		}
		else if ( cbCardType == CT_BOMB_CARD )
		{
			m_GameClientView.ShowCartoonBomb(true);
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB"));	
		}

		//������ʷ
		m_HistoryCard.OnEventUserOutCard(pOutCard->wOutCardUser,pOutCard->cbCardData,pOutCard->cbCardCount);

		//��ʾ����
		WORD wViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
		m_GameClientView.m_UserCardControl[wViewChairID].ShowOutCard(pOutCard->cbCardData,pOutCard->cbCardCount);
		
		//ɾ���˿�
		if (pOutCard->wOutCardUser==wMeChairID || m_bUserCheat[pOutCard->wOutCardUser])
		{
			//ɾ���˿�
			BYTE cbSourceCount=m_cbHandCardCount[pOutCard->wOutCardUser];
			m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
			m_GameLogic.RemoveCardList(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData[pOutCard->wOutCardUser],cbSourceCount);

			//�����˿�
			m_GameLogic.SortCardList(m_cbHandCardData[pOutCard->wOutCardUser],m_cbHandCardCount[pOutCard->wOutCardUser],m_cbSortType);

			//�����˿�
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData[pOutCard->wOutCardUser],m_cbHandCardCount[pOutCard->wOutCardUser]);
		}
		else
		{
			//�����˿�
			m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[pOutCard->wOutCardUser]);
		}
	}

	//���Ʊ���
	m_wCurrentUser=pOutCard->wCurrentUser;
	m_cbTurnCardCount=pOutCard->cbCardCount;
	CopyMemory(m_cbTurnCardData,pOutCard->cbCardData,pOutCard->cbCardCount*sizeof(BYTE));

	//���ƾ���
	if ( m_cbHandCardCount[pOutCard->wOutCardUser] <= 2 && m_cbHandCardCount[pOutCard->wOutCardUser] > 0 )
	{
		//���ñ���
		m_GameClientView.SetUserCountWarn(wViewChairID,true);

		//���ű���
		PlayGameSound(AfxGetInstanceHandle(),TEXT("COUNT_WARN"));
	}

	//����ж�
	if (m_wCurrentUser==pOutCard->wOutCardUser)
	{
		//�����û�
		m_wCurrentUser=INVALID_CHAIR;
		m_wMostCardUser=pOutCard->wCurrentUser;

		//���Ʊ���
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

		//��������
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//�û�����
			if (pOutCard->wOutCardUser==i) continue;

			//������¼
			m_HistoryCard.OnEventSaveData(i);
			m_HistoryCard.OnEventUserPass(i);

			//��������
			WORD wViewChairID=SwitchViewChairID(i);
			m_GameClientView.SetUserPassState(wViewChairID,true);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);
		}

		//����ʱ��
		SetTimer(IDI_MOST_CARD,3000,NULL);
	}

	//���½���
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return true;
}

//�û�����
bool CGameClientEngine::OnSubPassCard(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PassCard));
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;

	//��������
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pData;

	//���ñ���
	m_wCurrentUser=pPassCard->wCurrentUser;

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassCardUser!=GetMeChairID()))
	{
		//��ǰ�˿�
		SwitchToCurrentCard();

		//ɾ��ʱ��
		KillGameClock(IDI_OUT_CARD);

		//��ʷ��¼
		m_HistoryCard.OnEventUserPass(pPassCard->wPassCardUser);

		//��������
		m_GameClientView.SetUserPassState(SwitchViewChairID(pPassCard->wPassCardUser),true);
		m_GameClientView.m_UserCardControl[SwitchViewChairID(pPassCard->wPassCardUser)].SetCardData(0);

		//��������
		IClientUserItem * pClientUserItem=GetTableUserItem(pPassCard->wPassCardUser);
		PlayGameSound(AfxGetInstanceHandle(),(pClientUserItem->GetGender()==GENDER_FEMALE)?TEXT("PASS_CARD_GIRL"):TEXT("PASS_CARD_BOY"));
	}

	//�������
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//��������
		m_HistoryCard.OnEventSaveData(m_wCurrentUser);

		//��������
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);

		//��ǰ����
		m_GameClientView.SetUserPassState(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);
	}

	//һ���ж�
	if (pPassCard->cbTurnOver==TRUE)
	{
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//������
		if (m_bTrustee==false) ActiveGameFrame();

		//��ʾ��ť
		m_GameClientView.m_btOutCard.ShowWindow(true);
		m_GameClientView.m_btPassCard.ShowWindow(true);
		m_GameClientView.m_btOutPrompt.ShowWindow(true);
		m_GameClientView.m_btPassCard.EnableWindow((m_cbTurnCardCount>0)?true:false);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);

		//������ʾ
		try
		{
			m_GameLogic.SearchOutCard(m_cbHandCardData[GetMeChairID()],m_cbHandCardCount[m_wCurrentUser],m_cbTurnCardData,m_cbTurnCardCount,
				&m_SearchCardResult);
		}catch(...)
		{
			ASSERT(FALSE);
			m_SearchCardResult.cbSearchCount = 0;
		}
	}
	//else if ( m_wCurrentUser != INVALID_CHAIR )
	//{
	//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
	//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
	//}


	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeOutCard);

		//�����ǰ����
		m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
		m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
		m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameConclude(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameConclude));
	if (wDataSize!=sizeof(CMD_S_GameConclude)) return false;

	//��������
	CMD_S_GameConclude * pGameConclude=(CMD_S_GameConclude *)pData;

	//��ǰ״̬
	SwitchToCurrentCard();

	//����״̬
	SetGameStatus(GAME_SCENE_FREE);

	//ֹͣ����
	m_GameClientView.StopDispatchCard();

	//ɾ��ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);
	KillGameClock(IDI_CALL_SCORE);

	//���ư�ť
	m_GameClientView.m_btOutCard.ShowWindow(false);
	m_GameClientView.m_btPassCard.ShowWindow(false);
	m_GameClientView.m_btOutPrompt.ShowWindow(false);

	//�зְ�ť
	m_GameClientView.m_btCallScore1.ShowWindow(false);
	m_GameClientView.m_btCallScore2.ShowWindow(false);
	m_GameClientView.m_btCallScore3.ShowWindow(false);
	m_GameClientView.m_btCallScoreNone.ShowWindow(false);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(false);
	m_GameClientView.m_btPassCard.EnableWindow(false);
	m_GameClientView.m_btSortCard.EnableWindow(false);
	m_GameClientView.m_btLastTurn.EnableWindow(false);

	//��������
	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

	//�ɼ�����
	ScoreInfo.wMeChairID=GetMeChairID();
	ScoreInfo.wBankerUser=m_wBankerUser;
	ScoreInfo.lCellScore=pGameConclude->lCellScore;
	ScoreInfo.cbBombCount=pGameConclude->cbBombCount;
	ScoreInfo.cbBankerScore=pGameConclude->cbBankerScore;
	ScoreInfo.bChunTian=pGameConclude->bChunTian;
	ScoreInfo.bFanChunTian=pGameConclude->bFanChunTian;



	//�û��ɼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		IClientUserItem * pIClientUserItem=GetTableUserItem(i);
		tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

		//ը����Ϣ
		ScoreInfo.cbEachBombCount[i]=pGameConclude->cbEachBombCount[i];

		//���óɼ�
		ScoreInfo.lGameScore[i]=pGameConclude->lGameScore[i];
		ScoreInfo.lCollectScore[i]=pHistoryScore->lCollectScore+pGameConclude->lGameScore[i];

		//�û���Ϣ
		ScoreInfo.wFaceID[i]=pIClientUserItem->GetFaceID();
		ScoreInfo.dwUserID[i]=pIClientUserItem->GetUserID();
		lstrcpyn(ScoreInfo.szNickName[i],pIClientUserItem->GetNickName(),CountArray(ScoreInfo.szNickName[i]));

		//��ʷ�ɼ�
		m_HistoryScore.OnEventUserScore(i,pGameConclude->lGameScore[i]);

	}

	//��ʾ�ɼ�
	m_GameClientView.ShowGameScoreInfo(ScoreInfo);

	//�����˿�
	BYTE cbCardIndex=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�����˿�
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(true);
		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(&pGameConclude->cbHandCardData[cbCardIndex],pGameConclude->cbCardCount[i]);

		//��������
		cbCardIndex+=pGameConclude->cbCardCount[i];
	}

	//���ý���
	m_GameClientView.SetWaitCallScore(false);
	m_GameClientView.SetUserPassState(INVALID_CHAIR,false);
	m_GameClientView.SetUserCallScore(INVALID_CHAIR,false);
	m_GameClientView.SetUserCountWarn(INVALID_CHAIR,false);

	//��������
	SCORE lMeScore=pGameConclude->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOSE"));
	if (lMeScore==0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_CONCLUDE"));

	//ȡ���й�
	if(m_bTrustee)
		OnMessageTrusteeControl(0,0);

	//����
	if ( m_cbSortType == ST_CUSTOM )
		OnMessageSortHandCard(ST_ORDER,0);

	//��ʼ����
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(true);
		PerformAutoStart();
	}

	//���½���
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return true;
}

//��������
bool CGameClientEngine::OnSubCheatCard(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CheatCard));
	if (wDataSize!=sizeof(CMD_S_CheatCard)) return false;

	//��������
	CMD_S_CheatCard * pCheatCard=(CMD_S_CheatCard *)pData;

	ASSERT(pCheatCard->cbUserCount <= GAME_PLAYER);
	if(pCheatCard->cbUserCount > GAME_PLAYER) return false;
	
	m_bCheatRight = true;
	for(WORD i = 0; i < pCheatCard->cbUserCount ; i++)
	{
		//��ʾ�˿�
		m_GameClientView.UserValidCard(SwitchViewChairID(pCheatCard->wCardUser[i]), pCheatCard->cbCardData[i], pCheatCard->cbCardCount[i]);
		m_bUserCheat[pCheatCard->wCardUser[i]] = true;
		CopyMemory(m_cbHandCardData[pCheatCard->wCardUser[i]], pCheatCard->cbCardData[i], sizeof(BYTE) * pCheatCard->cbCardCount[i]);

	}
	//���½���
	CRect rcClient;
	GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	return true;
}

//�����ж�
bool CGameClientEngine::VerdictOutCard()
{
	//״̬�ж�
	if (m_wCurrentUser!=GetMeChairID()) return false;
	if (GetGameStatus()!=GAME_SCENE_PLAY) return false;

	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbCardData,CountArray(cbCardData));

	//�����ж�
	if (cbShootCount>0L)
	{
		//�����ж�
		m_GameLogic.SortCardList(cbCardData,cbShootCount,ST_ORDER);
		if (m_GameLogic.GetCardType(cbCardData,cbShootCount)==CT_ERROR) return false;

		//�����ж�
		if (m_cbTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(m_cbTurnCardData,cbCardData,m_cbTurnCardCount,cbShootCount);
	}

	return false;
}

//�Զ���ʼ
bool CGameClientEngine::PerformAutoStart()
{
	//��������
	WORD wTableUserCount=0,wReadyUserCount=0;
	IClientUserItem * pIMySelfUserItem=GetMeUserItem();

	//״̬�ж�
	if ((pIMySelfUserItem==NULL)||(pIMySelfUserItem->GetUserStatus()==US_READY)) return false;
	if ((IsLookonMode()==true)||(m_pIClientKernel->GetClockChairID()==IDI_START_GAME)) return false;

	//ͳ���û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetTableUserItem(i);

		//��Ŀͳ��
		if (pIClientUserItem!=NULL) 
		{
			wTableUserCount++;
			if (pIClientUserItem->GetUserStatus()==US_READY) wReadyUserCount++;
		}
	}

	//��ʼ�ж�
	if ((wTableUserCount==GAME_PLAYER)&&((wReadyUserCount+1)==wTableUserCount))
	{
		//��ʼ����
		WORD wMeChairID=GetMeChairID();
		SetGameClock(wMeChairID,IDI_START_GAME,m_cbTimeStartGame);
		return true;
	}

	return true;
}

//��ǰ�˿�
bool CGameClientEngine::SwitchToCurrentCard()
{
	//�����ж�
	if (m_bLastTurn==false) return false;

	//���ñ���
	m_bLastTurn=false;

	//ɾ��ʱ��
	KillTimer(IDI_LAST_TURN);

	//���ý���
	m_GameClientView.SetLastTurnState(false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		BYTE cbEventFlag=m_HistoryCard.GetCurrentEvent(i);

		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		BYTE cbCardCount=m_HistoryCard.GetCurrentCard(i,cbCardData,CountArray(cbCardData));

		//���ý���
		m_GameClientView.SetUserPassState(wViewChairID,cbEventFlag == AF_PASS);
		if( wViewChairID,cbEventFlag == AF_PASS || wViewChairID,cbEventFlag == AF_NULL )
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		}
		else
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//���½���
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return true;
}

//�Զ�����
bool CGameClientEngine::AutomatismOutCard()
{
	//״̬�ж�
	if (GetGameStatus()!=GAME_SCENE_PLAY) return false;
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return false;

	//��ǰ����
	CVirtualButton & btOutCard=m_GameClientView.m_btOutCard;
	if ((btOutCard.IsWindowEnable())&&(m_wCurrentUser==GetMeChairID()))
	{
		OnMessageOutCard(0,0);
		return true;
	}

	//���ƴ���
	if ((m_cbTurnCardCount==0)||(m_bTrustee==true))
	{
		//���ý���
		if (m_SearchCardResult.cbSearchCount>0)
		{
			//���ý���
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard( m_SearchCardResult.cbResultCard[0],
				m_SearchCardResult.cbCardCount[0] );

			//���ÿؼ�
			bool bOutCard=VerdictOutCard();
			ASSERT(bOutCard);
			m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

			//���ƶ���
			OnMessageOutCard(0,0);

			return true;
		}
	}

	//��������
	if (m_cbTurnCardCount>0) OnMessagePassCard(0,0);

	return true;
}

//���°�ť
VOID CGameClientEngine::UpdateButtonControl()
{
	//״̬�ж�
	if (GetMeUserItem()==NULL) return;
	if ((IsLookonMode()==true)&&(IsAllowLookon()==false)) return;

	//��������
	WORD wMeChairId=GetMeChairID();
	if (m_cbHandCardCount[wMeChairId]==0) return;

	//��������
	BYTE cbHandCardCount=m_cbHandCardCount[wMeChairId];
	BYTE cbHandCardData[MAX_COUNT] = {0};
	CopyMemory(cbHandCardData,m_cbHandCardData[wMeChairId],sizeof(BYTE)*cbHandCardCount);

	//����
	m_GameLogic.SortCardList(cbHandCardData,cbHandCardCount,ST_ORDER);

	bool bEnableButton = false;
	//�������
	if( cbHandCardCount > 1 && cbHandCardData[0] == 0x4f && cbHandCardData[1] == 0x4e )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt1.EnableWindow(bEnableButton);

	//����ը��
	if( m_GameLogic.SearchSameCard( cbHandCardData,cbHandCardCount,0,4,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt2.EnableWindow(bEnableButton);

	//�����ɻ�
	if( m_GameLogic.SearchThreeTwoLine( cbHandCardData,cbHandCardCount,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt3.EnableWindow(bEnableButton);

	//����˫˳
	if( m_GameLogic.SearchLineCardType( cbHandCardData,cbHandCardCount,0,2,0,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt4.EnableWindow(bEnableButton);

	//������˳
	if( m_GameLogic.SearchLineCardType( cbHandCardData,cbHandCardCount,0,1,0,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt5.EnableWindow(bEnableButton);

	//��������N
	if( m_GameLogic.SearchTakeCardType( cbHandCardData,cbHandCardCount,0,3,1,NULL ) ||
		m_GameLogic.SearchTakeCardType( cbHandCardData,cbHandCardCount,0,3,2,NULL ) )
		bEnableButton = true;
	else bEnableButton = false;
	m_GameClientView.m_btCardPrompt6.EnableWindow(bEnableButton);

	//�������
	m_nCurSearchType = -1;
	m_cbEachSearchIndex = 0;
	ZeroMemory( &m_EachSearchResult,sizeof(m_EachSearchResult) );

	return;
}

//��������
VOID CGameClientEngine::PlayOutCardSound(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч�����
	ASSERT((wChairID<GAME_PLAYER)&&(cbCardCount>0));
	if ((wChairID>=GAME_PLAYER)||(cbCardCount==0)) return;
	
	//��ȡ����
	BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);

	//��ȡ�û�
	IClientUserItem * pIClientUserItem=GetTableUserItem(wChairID);

	//��������
	switch (cbCardType)
	{
	case CT_SINGLE:					//��������
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();
			BYTE cbCardValue=m_GameLogic.GetCardValue(cbCardData[0]);

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("SINGLE_%d_%s"),cbCardValue,(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_DOUBLE:					//��������
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("DOUBLE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_THREE:					//��������
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_SINGLE_LINE:			//��������
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("SINGLE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_DOUBLE_LINE:			//��������
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("DOUBLE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_THREE_LINE:				//��������
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_THREE_TAKE_ONE:			//����һ��
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");

			if ( cbCardCount > 4 )
				_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_ONE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));
			else
				_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_TAKE_ONE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_THREE_TAKE_TWO:			//����һ��
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			if ( cbCardCount > 5 )
				_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_ONE_LINE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));
			else
				_sntprintf(szSoundName,CountArray(szSoundName),TEXT("THREE_TAKE_TWO_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_FOUR_TAKE_ONE:			//�Ĵ�����
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("FOUR_TAKE_ONE_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_FOUR_TAKE_TWO:			//�Ĵ�����
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("FOUR_TAKE_TWO_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_BOMB_CARD:				//ը������
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("BOMB_CARD_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	case CT_MISSILE_CARD:			//�������
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();

			//��������
			TCHAR szSoundName[32]=TEXT("");
			_sntprintf(szSoundName,CountArray(szSoundName),TEXT("MISSILE_CARD_%s"),(cbGender==GENDER_FEMALE)?TEXT("GIRL"):TEXT("BOY"));

			//��������
			PlayGameSound(AfxGetInstanceHandle(),szSoundName);

			return;
		}
	}

	return;
}


//�û�״̬
VOID CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//��������
	IClientUserItem * pIMySelfUserItem=GetMeUserItem();
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

	//�Զ���ʼ
	if ((pIClientUserItem!=pIMySelfUserItem)&&(cbUserStatus==US_READY)) PerformAutoStart();

	//ֹͣ��ʼ
	if ((pIClientUserItem==pIMySelfUserItem)&&(cbUserStatus>=US_READY)) 
	{
		KillGameClock(IDI_START_GAME);
		m_GameClientView.m_btStart.ShowWindow(false);
	}
}

//ʱ����Ϣ
VOID CGameClientEngine::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_MOST_CARD:			//����˿�
		{
			//�û�Ч��
			ASSERT(m_wMostCardUser!=INVALID_CHAIR);
			if (m_wMostCardUser==INVALID_CHAIR) return;

			//�����ж�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_GameClientView.m_UserCardControl[i].IsOutCard()==true)
				{
					return;
				}
			}

			//ɾ��ʱ��
			KillTimer(IDI_MOST_CARD);

			//���ñ���
			m_wCurrentUser=m_wMostCardUser;
			m_wMostCardUser=INVALID_CHAIR;

			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_HistoryCard.OnEventSaveData(i);
			}

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetUserPassState(i,false);
				m_GameClientView.m_UserCardControl[i].SetCardData(0);
			}

			//�������
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				//������
				if (m_bTrustee==false) ActiveGameFrame();

				//���ð�ť
				m_GameClientView.m_btOutCard.ShowWindow(true);
				m_GameClientView.m_btPassCard.ShowWindow(true);
				m_GameClientView.m_btPassCard.EnableWindow(false);
				m_GameClientView.m_btOutPrompt.ShowWindow(true);
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);

				//������ʾ
				try
				{
					m_GameLogic.SearchOutCard(m_cbHandCardData[GetMeChairID()],m_cbHandCardCount[m_wCurrentUser],m_cbTurnCardData,m_cbTurnCardCount,
						&m_SearchCardResult);
				}catch(...)
				{
					ASSERT(FALSE);
					m_SearchCardResult.cbSearchCount = 0;
				}
			}
			//else if ( m_wCurrentUser != INVALID_CHAIR )
			//{
			//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			//}


			//����ʱ��
			SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeOutCard);

			//�����ǰ����
			m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
			m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
			m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

			m_GameClientView.InvalidGameView(0,0,0,0);

			return;
		}
	case IDI_LAST_TURN:			//�����˿�
		{
			//��ԭ����
			SwitchToCurrentCard();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��Ϣ
LRESULT CGameClientEngine::OnMessageStart(WPARAM wParam, LPARAM lParam)
{
	//ɾ��ʱ��
	KillGameClock(IDI_START_GAME);

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetDisplayItem(false);
	}

	//�˿˿ؼ�
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(false);

	//����ׯ��
	m_cbBankerScore = 0;
	m_GameClientView.SetBankerScore(0);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);

	//״̬����
	m_GameClientView.SetBombCount(0);
	m_GameClientView.SetWaitCallScore(false);
	m_GameClientView.SetUserCallScore(INVALID_CHAIR,0);
	m_GameClientView.SetUserPassState(INVALID_CHAIR,false);
	m_GameClientView.SetUserCountWarn(INVALID_CHAIR,false);

	//���ý���
	m_GameClientView.m_btStart.ShowWindow(false);
	m_GameClientView.m_ScoreControl.CloseControl();
	m_GameClientView.ShowNoteCard(false);

	//���½���
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	//������Ϣ
	SendUserReady(NULL,0);

	return 0L;
}

//������Ϣ
LRESULT CGameClientEngine::OnMessageOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowEnable()==false) return 0;
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==false) return 0;

	//��ǰ�˿�
	SwitchToCurrentCard();

	//ɾ��ʱ��
	KillGameClock(IDI_OUT_CARD);

	//���ñ���
	if(1 == wParam)
	{
		m_bTrusteeCount = 0;
	}
	m_wCurrentUser=INVALID_CHAIR;
	m_cbSearchResultIndex = 0;
	m_SearchCardResult.cbSearchCount = 0;

	//���ý���
	m_GameClientView.m_btOutCard.ShowWindow(false);
	m_GameClientView.m_btPassCard.ShowWindow(false);
	m_GameClientView.m_btOutPrompt.ShowWindow(false);
	m_GameClientView.m_btOutCard.EnableWindow(false);
	m_GameClientView.m_btPassCard.EnableWindow(false);

	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbCardData,CountArray(cbCardData));

	//�����˿�
	m_GameLogic.SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//��������
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].RemoveShootItem();
	m_GameClientView.m_UserCardControl[MYSELF_VIEW_ID].ShowOutCard(cbCardData,cbCardCount);

	//ɾ���˿�
	WORD wMeChairID=GetMeChairID();
	m_cbHandCardCount[wMeChairID]-=cbCardCount;
	m_GameLogic.RemoveCardList(cbCardData,cbCardCount,m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID]+cbCardCount);

	//��ť����
	UpdateButtonControl();

	//��������
	BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);
	if ( m_cbTurnCardCount != 0 
		&& !((cbCardType == CT_THREE_TAKE_ONE && cbCardCount > 4) || (cbCardType == CT_THREE_TAKE_TWO && cbCardCount > 5))
		&& !( cbCardType == CT_MISSILE_CARD )
		&& !( cbCardType == CT_BOMB_CARD ))
	{
		//��������
		LPCTSTR pszSoundBoy=TEXT("OUT_CARD_BOY");
		LPCTSTR pszSoundGirl=TEXT("OUT_CARD_GIRL");
		IClientUserItem * pIClientUserItem=GetTableUserItem(wMeChairID);
		
		//��������
		BYTE cbGender=pIClientUserItem->GetGender();
		PlayGameSound(AfxGetInstanceHandle(),(cbGender==GENDER_FEMALE)?pszSoundGirl:pszSoundBoy);
	}
	else
	{
		//��������
		PlayOutCardSound(wMeChairID,cbCardData,cbCardCount);
	}

	//��������
	if ( (cbCardType == CT_THREE_TAKE_ONE && cbCardCount > 4)
		|| (cbCardType == CT_THREE_TAKE_TWO && cbCardCount > 5) )
	{
		m_GameClientView.ShowCartoonPlan(true);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PLANE"));
	}
	else if ( cbCardType == CT_MISSILE_CARD )
	{
		m_GameClientView.ShowCartoonRocket(true);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PLANE"));
	}
	else if ( cbCardType == CT_BOMB_CARD )
	{
		m_GameClientView.ShowCartoonBomb(true);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB"));	
	}

	//������ʷ
	m_HistoryCard.OnEventUserOutCard(wMeChairID,cbCardData,cbCardCount);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardCount=cbCardCount;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount*sizeof(BYTE));

	//��������
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE));

	return 0;
}

//PASS��Ϣ
LRESULT CGameClientEngine::OnMessagePassCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowEnable()==false) return 0;
	if (m_GameClientView.m_btPassCard.IsWindowVisible()==false) return 0;

	//��ǰ�˿�
	SwitchToCurrentCard();

	//ɾ��ʱ��
	KillGameClock(IDI_OUT_CARD);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_cbSearchResultIndex = 0;
	m_SearchCardResult.cbSearchCount = 0;

	//���ý���
	m_GameClientView.m_btOutCard.ShowWindow(false);
	m_GameClientView.m_btPassCard.ShowWindow(false);
	m_GameClientView.m_btOutPrompt.ShowWindow(false);
	m_GameClientView.m_btOutCard.EnableWindow(false);
	m_GameClientView.m_btPassCard.EnableWindow(false);

	//��ʷ��¼
	m_HistoryCard.OnEventUserPass(GetMeChairID());

	//���÷���
	m_GameClientView.SetUserPassState(MYSELF_VIEW_ID,true);

	//�����˿�
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

	//���½���
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	//��������
	IClientUserItem * pClientUserItem=GetMeUserItem();
	PlayGameSound(AfxGetInstanceHandle(),(pClientUserItem->GetGender()==GENDER_FEMALE)?TEXT("PASS_CARD_GIRL"):TEXT("PASS_CARD_BOY"));

	//��������
	SendSocketData(SUB_C_PASS_CARD);

	return 0L;
}

//��ʾ��Ϣ
LRESULT CGameClientEngine::OnMessageOutPrompt(WPARAM wParam, LPARAM lParam)
{
	WORD wMeChairID = GetMeChairID();

	//�д����
	if( m_SearchCardResult.cbSearchCount > 0 )
	{
		//���ý���
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(m_SearchCardResult.cbResultCard[m_cbSearchResultIndex],
			m_SearchCardResult.cbCardCount[m_cbSearchResultIndex]);

		//���ÿؼ�
		bool bOutCard=VerdictOutCard();
		ASSERT(bOutCard);
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);

		//���ñ���
		m_cbSearchResultIndex = (m_cbSearchResultIndex+1)%m_SearchCardResult.cbSearchCount;

		return 0;
	}

	//��������
	OnMessagePassCard(0,0);
	return 0L;
}

//�з���Ϣ
LRESULT CGameClientEngine::OnMessageCallScore(WPARAM wParam, LPARAM lParam)
{
	//ɾ��ʱ��
	KillGameClock(IDI_CALL_SCORE);

	//���ý���
	m_GameClientView.m_btCallScore1.ShowWindow(false);
	m_GameClientView.m_btCallScore2.ShowWindow(false);
	m_GameClientView.m_btCallScore3.ShowWindow(false);
	m_GameClientView.m_btCallScoreNone.ShowWindow(false);

	//��������
	CMD_C_CallScore CallScore;
	CallScore.cbCallScore=(BYTE)wParam;
	SendSocketData(SUB_C_CALL_SCORE,&CallScore,sizeof(CallScore));

	return 0L;
}

//�Ҽ��˿�
LRESULT CGameClientEngine::OnMessageLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//���ÿؼ�
	bool bOutCard=VerdictOutCard();
	m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

	return 0L;
}

//�����˿�
LRESULT CGameClientEngine::OnMessageSortHandCard(WPARAM wParam, LPARAM lParam)
{
	//�Զ�����
	if ((BYTE)wParam==ST_CUSTOM)
	{
		//����״̬
		if (m_cbSortType!=ST_CUSTOM)
		{
			//���ñ���
			m_cbSortType=ST_CUSTOM;

			//���ð�ť
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_GameClientView.m_btSortCard.SetButtonImage(&m_GameClientView.m_D3DDevice,TEXT("BT_SORT_CARD_ORDER"),TEXT("PNG"),hInstance);
		}

		return 0L;
	}

	//���ñ���
	m_cbSortType=(m_cbSortType!=ST_ORDER)?ST_ORDER:ST_COUNT;

	//�����˿�
	WORD wMeChairID=GetMeChairID();
	m_GameLogic.SortCardList(m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID],m_cbSortType);

	//��ȡ�˿�
	BYTE cbShootCard[MAX_COUNT];
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbShootCard,CountArray(cbShootCard));

	//�����˿�
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbHandCardData[wMeChairID],m_cbHandCardCount[wMeChairID]);

	//�����˿�
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(cbShootCard,cbShootCount);

	//���ð�ť
	LPCTSTR pszButtonImage=(m_cbSortType==ST_ORDER)?TEXT("BT_SORT_CARD_COUNT"):TEXT("BT_SORT_CARD_ORDER");
	m_GameClientView.m_btSortCard.SetButtonImage(&m_GameClientView.m_D3DDevice,pszButtonImage,TEXT("PNG"),AfxGetInstanceHandle());

	//���½���
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return 0;
}

//�鿴����
LRESULT CGameClientEngine::OnMessageLastTurnCard(WPARAM wParam, LPARAM lParam)
{
	if (m_bLastTurn==false)
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			WORD wViewChairID=SwitchViewChairID(i);
			BYTE cbEventFlag=m_HistoryCard.GetHistoryEvent(i);

			//��ȡ�˿�
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_HistoryCard.GetHistoryCard(i,cbCardData,CountArray(cbCardData));

			//���ý���
			m_GameClientView.SetUserPassState(wViewChairID,cbEventFlag==AF_PASS);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
		}

		//����ʱ��
		SetTimer(IDI_LAST_TURN,3000,NULL);

		//��������
		m_bLastTurn=true;
		m_GameClientView.SetLastTurnState(true);

		//���½���
		CRect rcClient;
		m_GameClientView.GetClientRect(&rcClient);
		m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}
	else
	{
		//��ǰ�˿�
		SwitchToCurrentCard();
	}

	return 0;
}

//�Ϲܿ���
LRESULT CGameClientEngine::OnMessageTrusteeControl(WPARAM wParam, LPARAM lParam)
{
	m_bTrusteeCount = 0;
	m_bTrustee = !m_bTrustee;
	if ( !m_bTrustee )
	{
		m_GameClientView.m_btTrustee.ShowWindow(true);
		m_GameClientView.m_btTrustee.EnableWindow(true);
		m_GameClientView.m_btCancelTrustee.ShowWindow(false);
	}
	else
	{
		m_GameClientView.m_btCancelTrustee.EnableWindow(true);
		m_GameClientView.m_btCancelTrustee.ShowWindow(true);
		m_GameClientView.m_btTrustee.ShowWindow(false);
	}

	//���½���
	CRect rcClient;
	m_GameClientView.GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return 0L;
}

//��������
LRESULT CGameClientEngine::OnMessageSearchCard(WPARAM wParam, LPARAM lParam)
{
	WORD wMeChairId = GetMeChairID();	
	if( m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetCardCount() != m_cbHandCardCount[wMeChairId] )
		return 0;

	//��������
	BYTE cbHandCardCount = m_cbHandCardCount[wMeChairId];
	BYTE cbHandCardData[MAX_COUNT] = {0};
	CopyMemory( cbHandCardData,m_cbHandCardData[wMeChairId],sizeof(BYTE)*cbHandCardCount );

	//����
	m_GameLogic.SortCardList( cbHandCardData,cbHandCardCount,ST_ORDER );

	//���ñ���
	m_cbSearchResultIndex = 0;

	switch( wParam )
	{
	case SEARCH_MISSILE:			//�������
		{
			if( m_nCurSearchType == wParam ) break;

			//���ñ���
			m_cbEachSearchIndex = 0;

			if( cbHandCardCount > 1 && cbHandCardData[0] == 0x4f && cbHandCardData[1] == 0x4e )
			{
				m_EachSearchResult.cbCardCount[0] = 2;
				m_EachSearchResult.cbResultCard[0][0] = cbHandCardData[0];
				m_EachSearchResult.cbResultCard[0][1] = cbHandCardData[1];

				m_EachSearchResult.cbSearchCount = 1;
			}
			m_nCurSearchType = SEARCH_MISSILE;
			break;
		}
	case SEARCH_BOMB:				//����ը��
		{
			if( m_nCurSearchType == wParam ) break;

			//���ñ���
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchSameCard( cbHandCardData,cbHandCardCount,0,4,&m_EachSearchResult );
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_BOMB;
			break;
		}
	case SEARCH_THREE_TOW_LINE:		//�����ɻ�
		{
			if( m_nCurSearchType == wParam ) break;

			//���ñ���
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchThreeTwoLine( cbHandCardData,cbHandCardCount,&m_EachSearchResult );
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_THREE_TOW_LINE;
			break;
		}
	case SEARCH_DOUBLE_LINE:		//����˫˳
		{
			if( m_nCurSearchType == wParam ) break;

			//���ñ���
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchLineCardType( cbHandCardData,cbHandCardCount,0,2,0,&m_EachSearchResult );
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_DOUBLE_LINE;
			break;
		}
	case SEARCH_SINGLE_LINE:		//������˳
		{
			if( m_nCurSearchType == wParam ) break;

			//���ñ���
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchLineCardType( cbHandCardData,cbHandCardCount,0,1,0,&m_EachSearchResult );
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_SINGLE_LINE;
			break;
		}
	case SEARCH_THREE_TWO_ONE:		//��������N
		{
			if( m_nCurSearchType == wParam ) break;

			//���ñ���
			m_cbEachSearchIndex = 0;

			try
			{
				m_GameLogic.SearchTakeCardType( cbHandCardData,cbHandCardCount,0,3,1,&m_EachSearchResult );
				tagSearchCardResult tmpSearchResult = {0};
				m_GameLogic.SearchTakeCardType( cbHandCardData,cbHandCardCount,0,3,2,&tmpSearchResult );
				if( tmpSearchResult.cbSearchCount > 0 )
				{
					//��������
					for( BYTE i = 0; i < tmpSearchResult.cbSearchCount; i++ )
					{
						BYTE cbResultIndex = m_EachSearchResult.cbSearchCount++;
						m_EachSearchResult.cbCardCount[cbResultIndex] = tmpSearchResult.cbCardCount[i];
						CopyMemory( m_EachSearchResult.cbResultCard[cbResultIndex],tmpSearchResult.cbResultCard[i],
							sizeof(BYTE)*tmpSearchResult.cbCardCount[i] );
					}
				}
			}catch(...)
			{
				ASSERT(FALSE);
				m_EachSearchResult.cbSearchCount = 0;
			}
			m_nCurSearchType = SEARCH_THREE_TWO_ONE;
			break;
		}
	}

	//�����˿�
	if( m_EachSearchResult.cbSearchCount > 0 )
	{
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard( m_EachSearchResult.cbResultCard[m_cbEachSearchIndex],
			m_EachSearchResult.cbCardCount[m_cbEachSearchIndex] );

		m_cbEachSearchIndex = (m_cbEachSearchIndex+1)%m_EachSearchResult.cbSearchCount;

		//�������
		if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairId))
		{
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		}
	}
	else 
	{
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(NULL,0);

		//�������
		if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairId))
		{
			m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		}
	}

	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0L;
}

//��������
LRESULT CGameClientEngine::OnMessageEnableHistory(WPARAM wParam, LPARAM lParam)
{
	//��������
	m_GameClientView.m_btLastTurn.EnableWindow(true);

	return 0;
}

//�������
LRESULT CGameClientEngine::OnMessageOutCardFinish(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_GameClientView.m_UserCardControl[i].IsOutCard()==true)
		{
			return 0L;
		}
	}

	//��������
	if ((GetGameStatus()==GAME_SCENE_PLAY)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		//��������
		WORD wMeChairID=GetMeChairID();

		//�����˿�
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);

		//���ư�ť
		if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
		{
			//������
			if (m_bTrustee==false) ActiveGameFrame();

			//��ʾ��ť
			m_GameClientView.m_btOutCard.ShowWindow(true);
			m_GameClientView.m_btPassCard.ShowWindow(true);
			m_GameClientView.m_btOutPrompt.ShowWindow(true);

			//���ð�ť
			m_GameClientView.m_btPassCard.EnableWindow((m_cbTurnCardCount>0)?true:false);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);

			//������ʾ
			try
			{
				m_GameLogic.SearchOutCard(m_cbHandCardData[wMeChairID],m_cbHandCardCount[m_wCurrentUser],m_cbTurnCardData,m_cbTurnCardCount,
					&m_SearchCardResult);
			}catch(...)
			{
				ASSERT(FALSE);
				m_SearchCardResult.cbSearchCount = 0;
			}
		}
		//else if ( m_wCurrentUser != INVALID_CHAIR )
		//{
		//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
		//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
		//}


		//����ʱ��
		SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeOutCard);

		//�����ǰ����
		m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
		m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
		m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	//��������
	if ((GetGameStatus()==GAME_SCENE_FREE)&&(m_wCurrentUser==INVALID_CHAIR))
	{
		m_GameClientView.m_btCardPrompt1.EnableWindow(false);
		m_GameClientView.m_btCardPrompt2.EnableWindow(false);
		m_GameClientView.m_btCardPrompt3.EnableWindow(false);
		m_GameClientView.m_btCardPrompt4.EnableWindow(false);
		m_GameClientView.m_btCardPrompt5.EnableWindow(false);
		m_GameClientView.m_btCardPrompt6.EnableWindow(false);
	}
	else
	{
		//���°�ť
		UpdateButtonControl();
	}

	return 0L;
}

//�������
LRESULT CGameClientEngine::OnMessageDispatchFinish(WPARAM wParam, LPARAM lParam)
{
	//״̬Ч��
	ASSERT(GetGameStatus()==GAME_SCENE_CALL);
	if (GetGameStatus()!=GAME_SCENE_CALL) return 0L;

	//���ý���
	m_GameClientView.SetWaitCallScore(true);
	m_GameClientView.m_btSortCard.EnableWindow(!IsLookonMode());

	WORD wMeChairID=GetMeChairID();
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		if(i == wMeChairID || m_bUserCheat[i])
		{
			//�����˿�
			m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],ST_ORDER);
			//�����˿�
			m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(m_cbHandCardData[i],m_cbHandCardCount[i]);
		}
	}

	//��ʾ��ť
	if ((IsLookonMode()==false)&&(wMeChairID==m_wCurrentUser))
	{
		//���ư�ť
		m_GameClientView.m_btCallScore1.EnableWindow(true);
		m_GameClientView.m_btCallScore2.EnableWindow(true);
		m_GameClientView.m_btCallScore3.EnableWindow(true);
		m_GameClientView.m_btCallScoreNone.EnableWindow(true);

		//��ʾ��ť
		m_GameClientView.m_btCallScore1.ShowWindow(true);
		m_GameClientView.m_btCallScore2.ShowWindow(true);
		m_GameClientView.m_btCallScore3.ShowWindow(true);
		m_GameClientView.m_btCallScoreNone.ShowWindow(true);
	}

	//��ҿ���
	if (IsLookonMode()==false)	
	{
		m_GameClientView.m_btSortCard.EnableWindow(true);
	}

	//���°�ť
	UpdateButtonControl();

	//����ʱ��
	SetGameClock(m_wCurrentUser,IDI_CALL_SCORE,m_cbTimeCallScore);

	//�����ǰ����
	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
	m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

	m_GameClientView.InvalidGameView(0,0,0,0);

	//������
	if ((IsLookonMode()==false)&&(m_bTrustee==false)) ActiveGameFrame();

	return 0L;
}

//�������
LRESULT CGameClientEngine::OnMessageReversalFinish(WPARAM wParam, LPARAM lParam)
{
	//״̬Ч��
	ASSERT(GetGameStatus()==GAME_SCENE_PLAY);
	if (GetGameStatus()!=GAME_SCENE_PLAY) return 0L;

	//���ý���
	m_GameClientView.SetUserCallScore(INVALID_CHAIR,0);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//���ñ���
	WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
	m_GameClientView.m_HandCardControl[wViewChairID].SetBackGround(CARD_LAND);
	m_GameClientView.SetBankerScore(m_cbBankerScore);

	WORD wMeChairID=GetMeChairID();

	//���õ���
	if (m_wBankerUser==wMeChairID || m_bCheatRight)
	{
		//��ȡ�˿�
		BYTE cbBankerCard[3];
		m_GameClientView.m_BackCardControl.GetCardData(cbBankerCard,CountArray(cbBankerCard));

		//�����˿�
		m_cbHandCardCount[m_wBankerUser]=MAX_COUNT;
		CopyMemory(&m_cbHandCardData[m_wBankerUser][NORMAL_COUNT],cbBankerCard,CountArray(cbBankerCard));

		//�����˿�
		m_GameLogic.SortCardList(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser],m_cbSortType);

		//�����˿�
		m_GameClientView.m_HandCardControl[SwitchViewChairID(m_wBankerUser)].SetCardData(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser]);
		if(m_wBankerUser==wMeChairID)
		{
			//�����˿�
			m_GameClientView.m_HandCardControl[SwitchViewChairID(m_wBankerUser)].SetShootCard(cbBankerCard,CountArray(cbBankerCard));
		}
	}
	else
	{
		//�����˿�
		m_cbHandCardCount[m_wBankerUser]=MAX_COUNT;

		//�������
		WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(MAX_COUNT);
	}

	//��������
	if (IsLookonMode()==false)
	{
		//������
		if (m_bTrustee==false) ActiveGameFrame();

		//���ư�ť
		if (m_wCurrentUser==wMeChairID)
		{
			//���ð�ť
			m_GameClientView.m_btOutCard.EnableWindow(false);
			m_GameClientView.m_btPassCard.EnableWindow(false);

			//��ʾ��ť
			m_GameClientView.m_btOutCard.ShowWindow(true);
			m_GameClientView.m_btPassCard.ShowWindow(true);
			m_GameClientView.m_btOutPrompt.ShowWindow(true);

			//������ʾ
			try
			{
				m_GameLogic.SearchOutCard(m_cbHandCardData[wMeChairID],m_cbHandCardCount[m_wCurrentUser],m_cbTurnCardData,m_cbTurnCardCount,
					&m_SearchCardResult);
			}catch(...)
			{
				ASSERT(FALSE);
				m_SearchCardResult.cbSearchCount = 0;
			}
		}
		//else if ( m_wCurrentUser != INVALID_CHAIR )
		//{
		//	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
		//	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
		//}


		//��������
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(true);
	}

	//���°�ť
	UpdateButtonControl();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("BANKER_INFO"));

	//�����ǰ����
	m_GameClientView.m_UserCardControl[SwitchViewChairID(m_wCurrentUser)].SetCardData(0);
	m_GameClientView.SetUserPassState(SwitchViewChairID(m_wCurrentUser), false);
	m_GameClientView.SetUserCallScore(SwitchViewChairID(m_wCurrentUser), 0);

	//����ʱ��
	SetGameClock(m_wCurrentUser,IDI_OUT_CARD,m_cbTimeHeadOutCard);

	//��ҿ���
	if (IsLookonMode()==false)	
	{
		m_GameClientView.m_btSortCard.EnableWindow(true);
	}

	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0L;
}

//��������
bool CGameClientEngine::AllowBackGroundSound( bool bAllowSound )
{
/*	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	if(bAllowSound && !m_bBackGroundSound && !pGlobalUnits->m_bMuteStatuts)
	{
		m_bBackGroundSound = true;
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_MUSIC"));
	}
	else if(!bAllowSound || pGlobalUnits->m_bMuteStatuts)
	{
		StopSound();
		m_bBackGroundSound = false;
	}
	*/
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
