#include "StdAfx.h"
#include "TableFrameSink.h"
#include "Config.h"
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����

//��ʱ����ʶ
#define IDI_DELAY_ENDGAME			10				//������ʶ
#define IDI_ACTION					11				//�ж���ʶ

//ʱ���ʶ
#define TIME_DELAY_ENDGAME			40
#define TIME_ACTION					30000
//////////////////////////////////////////////////////////////////////////

void TraceMessage(LPCTSTR pszMessage);

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//��ұ���
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	//m_wWinner = INVALID_CHAIR;

	//���״̬
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
    //���Ʊ�־
	ZeroMemory(m_cbGiveUpFlag,sizeof(m_cbGiveUpFlag));

	//�˿˱���
	m_cbSendCardCount = 0;
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע����
	m_lCellScore = 0L;
	m_lTurnLessScore = 0L;
	m_lAddLessScore = 0L;
	m_lTurnMaxScore = 0L;
	m_wOperaCount = 0;
	m_cbBalanceCount = 0;
	m_lBalanceScore =0L;

	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_cbShowHand,sizeof(m_cbShowHand));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));//by hxh

	//˰�ձ���
	//ZeroMemory(m_bUserTax,sizeof(m_bUserTax));
	//ZeroMemory(m_bLastTax,sizeof(m_bLastTax));

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);

	return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��ұ���
	m_wCurrentUser = INVALID_CHAIR;

	//���״̬
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
    //���Ʊ�־
	ZeroMemory(m_cbGiveUpFlag,sizeof(m_cbGiveUpFlag));

	//�˿˱���
	m_cbSendCardCount = 0;
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע����
	m_lCellScore = 0L;
	m_lTurnLessScore = 0L;
	m_lTurnMaxScore = 0L;
	m_lAddLessScore = 0L;
	m_wOperaCount = 0;
	m_cbBalanceCount = 0;
	m_lBalanceScore = 0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_cbShowHand,sizeof(m_cbShowHand));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));//by hxh

	//˰�ձ���
	//ZeroMemory(m_bUserTax,sizeof(m_bUserTax));	
	//ZeroMemory(m_bLastTax,sizeof(m_bLastTax));	

	return;
}

//�û����� 
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{	
	if(bLookonUser || pIServerUserItem->GetChairID()==INVALID_CHAIR)return true;
	WORD i=0;
	for ( i=0;i<m_wPlayerCount;i++)
	{
		if(i==pIServerUserItem->GetChairID())continue;
		if (m_pITableFrame->GetTableUserItem(i)!=NULL)
		{
			break;
		}
	}

	//ׯ������
	if(i==m_wPlayerCount)m_wDUser=INVALID_CHAIR;

	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);

	//��Ϸ����
	WORD wUserCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//��Ч�û�
		if (pIServerUserItem==NULL) continue;

		//��ȡ����
		ASSERT(pIServerUserItem->GetUserScore()>=m_pGameServiceOption->lCellScore);
		m_lUserMaxScore[i]=pIServerUserItem->GetUserScore();

		//����״̬
		m_cbPlayStatus[i]=TRUE;
		wUserCount++;
	}
	m_lCellScore=m_pGameServiceOption->lCellScore;

	DZPKConfig & cfg=DZPKConfig::Instance();
	if(cfg.m_Enable==1)
	{
		//�û��˿�
		WORD wCardCount=0;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i]==TRUE)
			{
				CopyMemory(&m_cbHandCardData[i][0],&cfg.m_cbHandCardData[i][0],sizeof(BYTE)*MAX_COUNT);
			}
		}

		//�����˿�
		CopyMemory(m_cbCenterCardData,cfg.m_cbCenterCardData,CountArray(m_cbCenterCardData));           
	}
	else
	{
		//�����˿�
		srand((unsigned int)time(NULL));
		BYTE cbRandCard[FULL_COUNT];
		ZeroMemory(cbRandCard,sizeof(cbRandCard));
		m_GameLogic.RandCardList(cbRandCard,wUserCount*MAX_COUNT+MAX_CENTERCOUNT);

		//�û��˿�
		WORD wCardCount=0;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i]==TRUE)
			{
				CopyMemory(&m_cbHandCardData[i],&cbRandCard[(wCardCount++)*MAX_COUNT],sizeof(BYTE)*MAX_COUNT);
			}
		}

		//�����˿�
		CopyMemory(m_cbCenterCardData,&cbRandCard[wUserCount*MAX_COUNT],CountArray(m_cbCenterCardData));
	}

	//�˿���Ŀ
	m_cbSendCardCount = 0;
	m_cbBalanceCount = 0;

	//�׼��ж�
	if(m_wDUser == INVALID_CHAIR)m_wDUser = 0;
	else m_wDUser =(m_wDUser+1)%GAME_PLAYER;

	//äע���
	WORD wPlayer[]={INVALID_CHAIR,INVALID_CHAIR,INVALID_CHAIR,INVALID_CHAIR},wPlayerCount = 0;
	WORD wNextUser = m_wDUser;
	do
	{
		if (m_cbPlayStatus[wNextUser]==TRUE) 
		{
			wPlayer[wPlayerCount++] = wNextUser;	
		}
		wNextUser =(wNextUser+1)%GAME_PLAYER;
	}while(wPlayerCount < 4);

	m_wDUser = wPlayer[0];
	m_wCurrentUser = wPlayer[3];

	//��ǰ��ע
	m_lTableScore[wPlayer[1]] = m_lCellScore;
	m_lTableScore[wPlayer[2]] = 2*m_lCellScore;
	m_lTotalScore[wPlayer[1]] = m_lCellScore;
	m_lTotalScore[wPlayer[2]] = 2*m_lCellScore;

	//���ñ���
	m_lBalanceScore = 2L*m_lCellScore;
	m_lTurnMaxScore = m_lUserMaxScore[m_wCurrentUser]-m_lTotalScore[m_wCurrentUser];
	m_lTurnLessScore = m_lBalanceScore - m_lTotalScore[m_wCurrentUser];
	m_lAddLessScore = 2L*m_lCellScore+m_lTurnLessScore;

	//�������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.wDUser = m_wDUser;
	GameStart.wMinChipInUser = wPlayer[1];
	GameStart.wMaxChipInUser = wPlayer[2];
	GameStart.wCurrentUser = m_wCurrentUser;
	GameStart.lCellScore = m_pGameServiceOption->lCellScore;
	GameStart.lAddLessScore = m_lAddLessScore;
	GameStart.lTurnLessScore = m_lTurnLessScore;
	GameStart.lTurnMaxScore = m_lTurnMaxScore;

    CopyMemory(GameStart.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));//�û���Ϸ״̬
    CopyMemory(GameStart.lUserMaxScore,m_lUserMaxScore,sizeof(m_lUserMaxScore));//�û������<by hxh>

	//����/©������
	//CopyMemory(GameStart.cbAllData,m_cbHandCardData,sizeof(m_cbHandCardData));

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		for (WORD j=0;j<MAX_COUNT;j++)
		{
			CString str;
			str.Format(L"hhh  server pai %d",m_cbHandCardData[i][j]);
			OutputDebugString(str);
		}

	}

	for (WORD i=0;i<5;i++)
	{
		CString strr;
		strr.Format(L"hhh  server CenterCard %d",m_cbCenterCardData[i]);
		OutputDebugString(strr);
	}

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			//��������
			CopyMemory(GameStart.cbCardData[i],m_cbHandCardData[i],MAX_COUNT);
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			ZeroMemory(GameStart.cbCardData[i],MAX_COUNT);
		}	
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	// �����ж���ʱ��
    m_pITableFrame->SetGameTimer(IDI_ACTION,TIME_ACTION,1,0L);

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	OutputDebugString(L"hhh ��Ϸ����");
	CString str;
	str.Format(L"hhh   cbReason==%d",cbReason);
	OutputDebugString(str);

	// ɱ���ж���ʱ��
	m_pITableFrame->KillGameTimer(IDI_ACTION);

	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.cbTotalEnd = 1;

			//�˿�����
			BYTE cbEndCardData[GAME_PLAYER][MAX_CENTERCOUNT];
			ZeroMemory(cbEndCardData,sizeof(cbEndCardData));

			try{
				//��ȡ�˿�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//�û�����
					if (m_cbPlayStatus[i]==FALSE) continue;

					//�������//BYTE CGameLogic::FiveFromSeven(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbCenterCardData[],BYTE cbCenterCardCount,BYTE cbLastCardData[],BYTE cbLastCardCount)
					GameEnd.cbLastCardKind[i] = m_GameLogic.FiveFromSeven(m_cbHandCardData[i],MAX_COUNT,m_cbCenterCardData,MAX_CENTERCOUNT,cbEndCardData[i],MAX_CENTERCOUNT);
					ASSERT(cbLastCardKind[i]!=FALSE);			
					CopyMemory(GameEnd.cbLastCenterCardData[i],cbEndCardData[i],sizeof(BYTE)*CountArray(cbEndCardData));
					

				}
			}catch(...)
			{
				TraceMessage(_TEXT("�û�����v�������"));
				ASSERT(FALSE);
			}
			
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (WORD j=0;j<MAX_COUNT;j++)
				{
					CString str;
					str.Format(L"hhh  jieshu pai %d",GameEnd.cbLastCenterCardData[i][j]);
					OutputDebugString(str);
				}
				
			}
		

			//����ע����
			DZPKSCORE lTotalScore[GAME_PLAYER];
			ZeroMemory(lTotalScore,sizeof(lTotalScore));
			CopyMemory(lTotalScore,m_lTotalScore,sizeof(m_lTotalScore));
			
			//ʤ���б�
			UserWinList WinnerList[GAME_PLAYER];
			ZeroMemory(WinnerList,sizeof(WinnerList));

			//��ʱ����
			BYTE bTempData[GAME_PLAYER][MAX_CENTERCOUNT];
			CopyMemory(bTempData,cbEndCardData,GAME_PLAYER*MAX_CENTERCOUNT);

			WORD wWinCount=0;
			try{
				//�û��÷�˳��
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//��������û�
					if(!m_GameLogic.SelectMaxUser(bTempData,WinnerList[i],lTotalScore))
					{
						wWinCount=i;
						break;
					}

					//ɾ��ʤ������
					for (WORD j=0;j<WinnerList[i].bSameCount;j++)
					{
						WORD wRemoveId=WinnerList[i].wWinerList[j];
						ASSERT(bTempData[wRemoveId][0]!=0);
						ZeroMemory(bTempData[wRemoveId],sizeof(BYTE)*MAX_CENTERCOUNT);
					}
				}
			}catch(...)
			{
				TraceMessage(_TEXT("�û��÷�˳��"));
				ASSERT(FALSE);
			}

			//ǿ���û�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE && lTotalScore[i]>0l)
				{					
					WinnerList[wWinCount].wWinerList[WinnerList[wWinCount].bSameCount++] = i;
				}
			}

			//�÷ֱ���
			DZPKSCORE lUserScore[GAME_PLAYER];
			ZeroMemory(lUserScore,sizeof(lUserScore));
			//CopyMemory(lTotalScore,m_lTotalScore,sizeof(m_lTotalScore));

			try
			{
				//�÷����
				for (int i=0;i<m_wPlayerCount-1;i++)
				{
					//ʤ������
					int iWinCount = (int)WinnerList[i].bSameCount;
					if(0 == iWinCount)break;

					//ʤ���û��÷����
					for(int j=0;j<iWinCount;j++)
					{
						if(0 == lTotalScore[WinnerList[i].wWinerList[j]])continue;

						if(j>0 && lTotalScore[WinnerList[i].wWinerList[j]] - 
							lTotalScore[WinnerList[i].wWinerList[j-1]] == 0)continue;

						//ʧ���û�ʧ�����
						for(int k=i+1;k<m_wPlayerCount;k++)
						{
							//ʧ������
							if(0 == WinnerList[k].bSameCount)break;

							for(int l=0;l<WinnerList[k].bSameCount;l++)
							{
								//�û������
								if(0 == lTotalScore[WinnerList[k].wWinerList[l]])continue;

								WORD wLostId=WinnerList[k].wWinerList[l];
								WORD wWinId=WinnerList[i].wWinerList[j];
								DZPKSCORE lMinScore = 0;

								//�ϼҵ÷���Ŀ
								DZPKSCORE lLastScore = ((j>0)?lTotalScore[WinnerList[i].wWinerList[j-1]]:0);
								if(j>0)ASSERT(lLastScore>0L);							
								lMinScore = min(lTotalScore[wWinId]-lLastScore,lTotalScore[wLostId]);

								



								for(int m=j;m<iWinCount;m++)
								{
									//�÷���Ŀ
									lUserScore[WinnerList[i].wWinerList[m]]+=lMinScore/(iWinCount-j);
								}

								//�⳥��Ŀ
								lUserScore[wLostId]-=lMinScore;
								lTotalScore[wLostId]-=lMinScore;
							}
						}
					}
				}
			}catch(...)
			{
				TraceMessage(_TEXT("�÷���Ŀ/�⳥��Ŀ"));
				ASSERT(FALSE);
			}

			//ͳ���û�����(˰��)
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.lGameScore[i]=lUserScore[i];
				ASSERT(lUserScore[i]+m_lTotalScore[i]>=0L);
				if(GameEnd.lGameScore[i]>0L)
				{
					GameEnd.lGameTax[i]=m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
					GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
				}
			}
		

			//��Ӯ����

			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));



			bool bDelayOverGame=false;
			IServerUserItem * pUserData;
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			//���ֱ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					BYTE nScoreKind=SCORE_TYPE_WIN;
					if(GameEnd.lGameScore[i]==0L)nScoreKind=SCORE_TYPE_DRAW;
					else nScoreKind=(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

					if(nScoreKind==SCORE_TYPE_LOSE)
					{
						pUserData=(IServerUserItem *)m_pITableFrame->GetTableUserItem(i);
						if(pUserData!=NULL)
						{
							if(bDelayOverGame==false && ((pUserData->GetUserScore()+GameEnd.lGameScore[i] )<m_pGameServiceOption->lMinTableScore)) 
	    						 bDelayOverGame=true;
						}
					}
					ScoreInfo[i].cbType = nScoreKind;
					ScoreInfo[i].lRevenue = GameEnd.lGameTax[i];
					ScoreInfo[i].lScore = GameEnd.lGameScore[i];

				}
			}
			
			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//������Ϸ
			if(bDelayOverGame==false)
			   m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,NULL,608);
			else
			   m_pITableFrame->SetGameTimer(IDI_DELAY_ENDGAME,40,1,0L);

			return true;
		}
	case GER_NO_PLAYER:		//û�����
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.cbTotalEnd = 0;

			//Ч����
			WORD wUserCount=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]!=FALSE)wUserCount++;
			}
			if(wUserCount!=1)
			{
				ASSERT(FALSE);
				TraceMessage(_TEXT("û�����//Ч��������"));
			}

			//ͳ�Ʒ���
			double lScore = 0,lRevenue = 0;
			BYTE nScoreKind ;
			WORD wWinner = INVALID_CHAIR;
			for (WORD i = 0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)
				{
					if(m_lTotalScore[i] > 0L)GameEnd.lGameScore[i]-=m_lTotalScore[i];
					continue;
				}

				wWinner = i;

				//����ע��Ŀ
				DZPKSCORE lAllScore = 0L;
				for (WORD j = 0;j<GAME_PLAYER;j++)
				{
					if(wWinner==j)continue;
					lAllScore += m_lTotalScore[j];
				}
				ASSERT(lAllScore>=0);
				GameEnd.lGameScore[i] = lAllScore;

				//ͳ��˰��
				if(GameEnd.lGameScore[i]>0L)
				{
					//��˰����
					//WORD cbRevenue=m_pGameServiceOption->wRevenueRatio;
					GameEnd.lGameTax[i]=m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
					GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
				}

				//�����˿�
				CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

				lScore=GameEnd.lGameScore[i];
				lRevenue= GameEnd.lGameTax[i];
				nScoreKind=(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
			}
			//������ҵ���������Ҳ���͹�ȥ
			BYTE cbEndCardData[GAME_PLAYER][MAX_CENTERCOUNT];
			ZeroMemory(cbEndCardData,sizeof(cbEndCardData));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				
				GameEnd.cbLastCardKind[i] = m_GameLogic.FiveFromSeven(m_cbHandCardData[i],MAX_COUNT,m_cbCenterCardData,MAX_CENTERCOUNT,cbEndCardData[i],MAX_CENTERCOUNT);
				ASSERT(cbLastCardKind[i]!=FALSE);			
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			//д�����
			if(wWinner<GAME_PLAYER)
			{
				ScoreInfo[wWinner].cbType = nScoreKind;
				ScoreInfo[wWinner].lRevenue = lRevenue;
				ScoreInfo[wWinner].lScore = lScore;
				m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			}
			else TraceMessage(_TEXT("//д�����ffff"));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,NULL,608);

			return true;
		}
	case GER_USER_LEAVE:
	case GER_NETWORK_ERROR:
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//ǿ�˴���
			OnUserGiveUp(wChairID,true);
			return true;
		}
	}
	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//��Ϸ����
			StatusFree.lCellMinScore=m_pGameServiceOption->lCellScore;
			StatusFree.lCellMaxScore=m_pGameServiceOption->lRestrictScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));	
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��־����
			//CopyMemory(StatusPlay.bShowHand,m_cbShowHand,sizeof(m_cbShowHand));

			//����ע��Ŀ
			DZPKSCORE lAllScore = 0L;
			for (WORD j = 0;j<GAME_PLAYER;j++)
			{
				lAllScore += m_lTotalScore[j];
				lAllScore -= m_lTableScore[j];
			}
			ASSERT(lAllScore>=0);

			//��ע��Ϣ
			StatusPlay.lAddLessScore = m_lAddLessScore;
			StatusPlay.lCellScore	 = m_lCellScore;
			StatusPlay.lTurnMaxScore = m_lTurnMaxScore;
			StatusPlay.lTurnLessScore= m_lTurnLessScore;
			StatusPlay.lCenterScore	 = lAllScore;
			StatusPlay.lCellMaxScore = m_pGameServiceOption->lRestrictScore;
			StatusPlay.cbBalanceCount= m_cbBalanceCount;

			//�˿���Ϣ
			CopyMemory(StatusPlay.cbHandCardData,m_cbHandCardData[wChairID],MAX_COUNT);
			if(m_cbBalanceCount>0)
			{
				CopyMemory(StatusPlay.cbCenterCardData,m_cbCenterCardData,sizeof(m_cbCenterCardData));
			}

			//���ñ���
			StatusPlay.wDUser = m_wDUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;//��ǰ���
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(m_lTableScore));//��ע��Ŀ
			CopyMemory(StatusPlay.lTotalScore,m_lTotalScore,sizeof(m_lTotalScore));//��ע��Ŀ
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));//�û���Ϸ״̬
            CopyMemory(StatusPlay.lUserMaxScore,m_lUserMaxScore,sizeof(m_lUserMaxScore));//�û������<by hxh>

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}
	//Ч����
	ASSERT(FALSE);
	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_DELAY_ENDGAME:		//������ʶ
		{
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,NULL,608);
		    m_pITableFrame->KillGameTimer(IDI_DELAY_ENDGAME);
			return true;
		}
	case IDI_ACTION:		//�ж���ʶ
		{
			//��ʱ���ƴ���
			OnUserGiveUp(m_wCurrentUser,false);
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch(wSubCmdID)
	{
	case SUB_C_GIVE_UP:		//�û�����
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			return OnUserGiveUp(pUserData->wChairID);
		}
	case SUB_C_ADD_SCORE:	//�û���ע	
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			return OnUserAddScore(pUserData->wChairID,pAddScore->lScore,false,pAddScore->cbJudgeAction);
		}
	case SUB_C_OPEN_CARD:	//����
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//״̬�ж�
			//ASSERT(m_cbPlayStatus[pUserData->wChairID]!=TRUE);
			//if (m_cbPlayStatus[pUserData->wChairID]!=FALSE) return false;

			//��������
			CMD_S_OpenCard OpenCard;
			OpenCard.wWinUser = pUserData->wChairID;			
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	

			return true;
		}
	//case SUB_C_SIT:			//����
	//	{
	//		tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
	//		CMD_S_SitDown sitUser;
	//		sitUser.wSitUser = pUserData->wChairID;
	//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SIT_DOWN,&sitUser,sizeof(sitUser));
	//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SIT_DOWN,&sitUser,sizeof(sitUser));
	//		
	//	}
	//case SUB_C_READY:			//׼��
	//	{
	//		tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
	//		CMD_S_Ready readyUser;
	//		readyUser.wReadyUser = pUserData->wChairID;
	//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SIT_DOWN,&readyUser,sizeof(readyUser));
	//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SIT_DOWN,&readyUser,sizeof(readyUser));

	//	}
	}
	return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID,bool bLeft)
{
    // add by hxh 20161104
	if (m_cbGiveUpFlag[wChairID]==TRUE) 
		return false;

    m_cbGiveUpFlag[wChairID]=TRUE;
	
	//����״̬
	m_cbPlayStatus[wChairID]=FALSE;
	m_cbShowHand[wChairID]=FALSE;

	//������Ϣ
	CMD_S_GiveUp GiveUp;
	GiveUp.wGiveUpUser=wChairID;
	GiveUp.lLost = -m_lTotalScore[wChairID];
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//д�����
	tagScoreInfo ScoreInfo[GAME_PLAYER];
	ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo[wChairID].lScore = -m_lTotalScore[wChairID];
	ScoreInfo[wChairID].cbType = (bLeft?SCORE_TYPE_FLEE:SCORE_TYPE_LOSE);
	m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

	//�����ע
	m_lTableScore[wChairID] = 0L;

	//����ͳ��
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//�жϽ���
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddScore(wChairID,0L,true,GAME_ACTION_GIVEUP);//������һ�������Ĳ���
	}
	else OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	return true;
}

//��ע�¼� 
bool CTableFrameSink::OnUserAddScore(WORD wChairID, DZPKSCORE lScore, bool bGiveUp,BYTE cbJudgeAction)
{
	//У���û�
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false; 

	//У����
	ASSERT((lScore +m_lTotalScore[wChairID])<= m_lUserMaxScore[wChairID]);	
	if ((lScore+m_lTotalScore[wChairID])>m_lUserMaxScore[wChairID]) return false;
	ASSERT(lScore>=0L);
	if ((lScore<0)) return false;

	//�ۼƽ��
	m_lTableScore[wChairID] += lScore;
	m_lTotalScore[wChairID] += lScore;

	//ƽ����ע
	if(m_lTableScore[wChairID] > m_lBalanceScore )
	{
		m_lBalanceScore = m_lTableScore[wChairID];
	}

	//����ж�
	if(m_lTotalScore[wChairID]==m_lUserMaxScore[wChairID])
	{
		m_cbShowHand[wChairID] = TRUE;
	}

	//�û��л�
	WORD wNextPlayer=INVALID_CHAIR;
	for (WORD i=1;i<m_wPlayerCount;i++)
	{
		//���ñ���
		m_wOperaCount++;
		wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

		//�����ж�
		if ((m_cbPlayStatus[wNextPlayer]==TRUE) &&(m_cbShowHand[wNextPlayer] == FALSE)) break;
	}
	ASSERT(wNextPlayer < m_wPlayerCount);

	//����ж�
	bool bFinishTurn=false;
	if (m_wOperaCount>=m_wPlayerCount)
	{
		WORD i=0;
		for ( i=0;i<m_wPlayerCount;i++)
		{
			//����δƽ�� ��δ����û�
			if ((m_cbPlayStatus[i]==TRUE)&&(m_lTableScore[i]<m_lBalanceScore)&&(m_cbShowHand[i]==FALSE)) 
				break;
		}
		if (i==m_wPlayerCount) 
			bFinishTurn=true;
	}

	//A��show190,B����,C��ѡ��?
	if(!bFinishTurn)
	{
		WORD wPlayCount = 0,wShowCount = 0;
		for (BYTE i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==TRUE)
			{
				if(m_cbShowHand[i]==TRUE)
				{
					wShowCount++;
				}
				wPlayCount++;
			}
		}
		if(wPlayCount-1==wShowCount && m_lTableScore[wNextPlayer]>=m_lBalanceScore) bFinishTurn=true;
	}

	//������ע
	if (!bFinishTurn)
	{
		//��ǰ�û�
		m_wCurrentUser=wNextPlayer;

		//��СֵΪƽ����ע -������ע  �� ʣ������ȡС �������
		m_lTurnLessScore = min(m_lBalanceScore - m_lTableScore[m_wCurrentUser],m_lUserMaxScore[m_wCurrentUser] - m_lTotalScore[m_wCurrentUser]);
		m_lTurnMaxScore = m_lUserMaxScore[m_wCurrentUser]-m_lTotalScore[m_wCurrentUser];
		if(m_lTotalScore[m_wCurrentUser]==m_lCellScore)
		{
			DZPKSCORE bTemp = (m_lBalanceScore==m_lCellScore*2)?(m_lCellScore*2):((m_lBalanceScore-m_lCellScore*2)*2);
			m_lAddLessScore = m_lCellScore+bTemp;
		}
		else m_lAddLessScore = (m_lBalanceScore==0)?(2*m_lCellScore):(__max((m_lBalanceScore-m_lTableScore[m_wCurrentUser])*2,2L*m_lCellScore));

		//��������
		CMD_S_AddScore AddScore;
		ZeroMemory(&AddScore,sizeof(AddScore));
		AddScore.lAddScoreCount=lScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.wCurrentUser=m_wCurrentUser;
		AddScore.lTurnLessScore=m_lTurnLessScore;
		AddScore.lTurnMaxScore = m_lTurnMaxScore;
		AddScore.lAddLessScore = m_lAddLessScore;
		AddScore.cbJudgeAction = cbJudgeAction;

		//��������
		SendAddScorePacket(&AddScore);

		return true;
	}

	//ƽ�����
	m_cbBalanceCount++;
	m_wOperaCount=0;

	//��1����עƽ���Ϳ�ʼ�������Ź���
	//��2����עƽ���Ϳ�ʼ�������Ź���
	//��3����עƽ���Ϳ�ʼ�������Ź���
	//��4����עƽ���ͽ�����Ϸ 

	//D����ע
	WORD wDUser=m_wDUser;
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		wDUser=(m_wDUser+i)%GAME_PLAYER;		
		if(m_cbPlayStatus[wDUser]==TRUE && m_cbShowHand[wDUser]==FALSE) break;
	}

	//��ֵ����
	m_lBalanceScore = 0L;
	m_lTurnLessScore = 0L;
	m_lTurnMaxScore = m_lUserMaxScore[wDUser]-m_lTotalScore[wDUser];
	m_lAddLessScore = 2*m_lCellScore;

	//��������
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore,sizeof(AddScore));
	AddScore.wAddScoreUser=wChairID;
	AddScore.wCurrentUser=INVALID_CHAIR;
	AddScore.lAddScoreCount=lScore;
	AddScore.lTurnLessScore=m_lTurnLessScore;
	AddScore.lTurnMaxScore = m_lTurnMaxScore;
	AddScore.lAddLessScore = m_lAddLessScore;
	AddScore.cbJudgeAction = cbJudgeAction; 

	//��������
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_lBalanceScore = 0L;

	//��������
	SendAddScorePacket(&AddScore);

	//�����ж�
	if (m_cbBalanceCount == 4) 
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
		return true;
	}

	//����û�ͳ��
	WORD wShowHandCount=0,wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbShowHand[i]==TRUE)	wShowHandCount++;
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//ֻʣһ���û�����ȫ��
	if((wShowHandCount >= wPlayerCount -1) && m_cbBalanceCount < 4)
	{
		//��������
		CMD_S_SendCard SendCard;
		ZeroMemory(&SendCard,sizeof(SendCard));
		SendCard.cbPublic = m_cbBalanceCount;
		SendCard.wCurrentUser = INVALID_CHAIR;
		SendCard.cbSendCardCount = MAX_CENTERCOUNT;
		CopyMemory(SendCard.cbCenterCardData,m_cbCenterCardData,sizeof(BYTE)*MAX_CENTERCOUNT);

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

		//������Ϸ
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

		return true;
	}

	//äע���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ʱ����
		BYTE cbNextUser =(m_wDUser+i)%GAME_PLAYER;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(cbNextUser);

		//��Ч�û� ����û�����
		if (pIServerUserItem==NULL||m_cbPlayStatus[cbNextUser] == FALSE||m_cbShowHand[cbNextUser] == 1) 
			continue;

		m_wCurrentUser = cbNextUser;
		break;
	}

	//��������
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard,sizeof(SendCard));
	SendCard.cbPublic = 0;
	SendCard.wCurrentUser = m_wCurrentUser;
	SendCard.cbSendCardCount = 3 +(m_cbBalanceCount-1);
	CopyMemory(SendCard.cbCenterCardData,m_cbCenterCardData,sizeof(BYTE)*(SendCard.cbSendCardCount));

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

void CTableFrameSink::SendAddScorePacket(CMD_S_AddScore *pAddScore)
{
	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,pAddScore,sizeof(CMD_S_AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,pAddScore,sizeof(CMD_S_AddScore));

	// ɱ���ж���ʱ��
	m_pITableFrame->KillGameTimer(IDI_ACTION);
	// �����ж���ʱ��
    m_pITableFrame->SetGameTimer(IDI_ACTION,TIME_ACTION,1,0L);
}

//�����Ϣ
void TraceMessage(LPCTSTR pszMessage)
{
	CFile File;
	if ((File.Open(TEXT("TraceData.txt"),CFile::modeWrite)==FALSE)&&
		(File.Open(TEXT("TraceData.txt"),CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	File.SeekToEnd();
	File.Write(pszMessage,lstrlen(pszMessage));
	File.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));

	File.Flush();
	File.Close();

	return;
}
//////////////////////////////////////////////////////////////////////////
