#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//��������
#define IDI_FREE					1									//���м�ʱ��
#define IDI_PLACE_JETTON			2									//��ע��ʱ��
#define IDI_GAME_END				3									//������ʱ��
//////////////////////////////////////////////////////////////////////////
//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink() : m_iMaxAreaScore(2100000000)
{
//AllocConsole();
//freopen("CONOUT$","w+t",stdout);
//freopen("CONIN$","r+t",stdin);
//system("cls");
	//����ע��
	ZeroMemory(m_lTotalJettonScore, sizeof(m_lTotalJettonScore));
	//��������·�
	ZeroMemory(m_lAreaScoreLimit, sizeof(m_lAreaScoreLimit));
	//���Ʊ���
	m_cbWinArea = -1;
	m_dwGameTimes = 0;
	//������ע
	ZeroMemory(m_lUserAreaScore, sizeof(m_lUserAreaScore));
	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserJetton,sizeof(m_lUserJetton));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(m_cbPaoHuoCheArea,sizeof(m_cbPaoHuoCheArea));
	ZeroMemory(m_lCurrentJetton,sizeof(m_lCurrentJetton));
	ZeroMemory(m_GameRecordMultiple,sizeof(m_GameRecordMultiple));

	m_CtrlAccountArray.RemoveAll();

	//״̬����
	m_dwJettonTime=0L;
	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_nHasWinTimes = 0;
	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;

	//ʱ�����
	m_cbFreeTime = 10;
	m_cbBetTime = 20;
	m_cbEndTime = 20;
	m_cbBigSmall=0;
	m_cbGoodLuckType=0;
	m_cbPaoHuoCheCount=0;

	//���ֵ
	m_StorageStart=0;
	m_StorageDeduct=5;

	m_dwJettonRate=100;
	m_cbGameRecordMultipleCount=0;

	m_cbControlArea=0xFF;
	m_cbControlTimes=0;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	m_ApplyUserArray.RemoveAll();
	m_CtrlAccountArray.RemoveAll();
}

//�ӿڲ�ѯ
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
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

	ReadConfigInformation(true);

	return true;
}

//��λ����
void CTableFrameSink::RepositionSink()
{
	//����ע��
	ZeroMemory(m_lTotalJettonScore, sizeof(m_lTotalJettonScore));
	//��������·�
	ZeroMemory(m_lAreaScoreLimit, sizeof(m_lAreaScoreLimit));
	//������ע
	ZeroMemory(m_lUserAreaScore, sizeof(m_lUserAreaScore));
	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserJetton,sizeof(m_lUserJetton));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(m_cbPaoHuoCheArea,sizeof(m_cbPaoHuoCheArea));
	m_cbGoodLuckType=0;
	m_cbPaoHuoCheCount=0;
	m_dwJettonRate=100;

	return;
}

//��ʼģʽ
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	m_dwGameTimes++;

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
		pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

	//���ñ���
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore();

	//ÿ����Ϸ��ʼʱ�̣�����������ʼ���·�ֵ
	CalculateAreaInitLimite();

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		GameStart.lAreaLimitScore[i] = m_lAreaScoreLimit[i];

    //���͸��Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//���͸���Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//ÿ����Ϸ��ʼʱ�̣�����������ʼ���·�ֵ
bool __cdecl CTableFrameSink::CalculateAreaInitLimite()
{
	//��ʼ������
	__int64 lBankerScore = 0;
	IServerUserItem * pIBankerServerUserItem = NULL;

	//������ׯ�����ׯ����Ϣ
	if( INVALID_CHAIR != m_wCurrentBanker )
	{
		pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if( pIBankerServerUserItem == NULL )
			return FALSE;

#ifndef FUNC_CHIP
		//���ׯ�ҵ�ǰ�Ļ���(���)
		lBankerScore=pIBankerServerUserItem->GetUserScore();
#else
		lBankerScore=m_lCurrentJetton[pIBankerServerUserItem->GetChairID()];
#endif

		//����ׯ�ҽ�����������������ĳ�ʼ�����·�
		//for( int i=0; i<JETTON_AREA_COUNT; i++ )
		//	m_lAreaScoreLimit[i] = __min(lBankerScore/(m_iOddsArray[i]+1), m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_APPLE]			=__min(lBankerScore/23,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_ORANGE]		=__min(lBankerScore/31,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_PAWPAW]		=__min(lBankerScore/31,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_BELL]			=__min(lBankerScore/31,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_WATERMELON]	=__min(lBankerScore/61,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_STAR]			=__min(lBankerScore/61,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_DOUBLE_SEVEN]	=__min(lBankerScore/61,m_iMaxAreaScore);
		m_lAreaScoreLimit[ID_BAR]			=__min(lBankerScore/76,m_iMaxAreaScore);
	}
	else //���������ׯ
	{
		if( m_bEnableSysBanker ) //�������ϵͳ��ׯ
		{
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
			{
				//m_lAreaScoreLimit[i] = __min( m_lSysLimitScore/(m_iOddsArray[i]/m_iOddsArray[i+1]), m_iMaxAreaScore);
				//m_lAreaScoreLimit[i] = m_lSysLimitScore;	//����ǵͱ�����������������עΪ�����ļ�ָ�����޶�
				m_lAreaScoreLimit[i]=999*10000;
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

//	������ׯ�б�
bool CTableFrameSink::SendApplyBankerList(IServerUserItem *pIServerUserItem)
{
	CMD_S_ApplyBankerListMax sList;
	int nCount = m_ApplyUserArray.GetCount();
	int j=0;
	for (int i = 0; i < nCount; i++)
	{
		WORD wChairID = m_ApplyUserArray.GetAt(i);
		IServerUserItem *pApplyUser = m_pITableFrame->GetTableUserItem(wChairID);
		if (m_wCurrentBanker == wChairID||pApplyUser==NULL)
		{
			continue;
		}
		sList.wApplyList[j].wChairID = wChairID;
		sList.wApplyList[j].dwUserID = pApplyUser->GetUserID();
		sList.wApplyList[j].lScore = m_lCurrentJetton[wChairID];
		sList.wApplyList[j].cbGender = pApplyUser->GetGender();
		wcscpy(sList.wApplyList[j].szNickName,pApplyUser->GetNickName());
		j++;
	}
	sList.cbCount=j;
	m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_C_GET_PLAYLIST, &sList,sList.getLength());

	return true;
}


//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//�ɷ��˿�
			do
			{
				DispatchTableCard();
			} while (!ProbeJudge());
			//�����˺ſ���
			DispatchTableCardControl();
//printf("m_cbBigSmall=%d\n",m_cbBigSmall);
//printf("m_cbPaoHuoCheCount=%d\n",m_cbPaoHuoCheCount);
//for (int ii=0; ii<m_cbPaoHuoCheCount; ii++)
//{
//	printf("m_cbPaoHuoCheArea[ii]=%d\n",m_cbPaoHuoCheArea[ii]);
//}

			//�������
			__int64 iBankerWinScore=CalculateScore();

			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ׯ����Ϣ
			GameEnd.iBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=iBankerWinScore;
			GameEnd.lApplyBankerCondition=m_lApplyBankerCondition;

			GameEnd.cbWinArea=m_cbWinArea;
			GameEnd.cbGoodLuckType=m_cbGoodLuckType;
			GameEnd.cbPaoHuoCheCount=m_cbPaoHuoCheCount;
			CopyMemory(GameEnd.cbPaoHuoCheArea,m_cbPaoHuoCheArea,sizeof(GameEnd.cbPaoHuoCheArea));

			//����ʱ��
			m_cbEndTime=20;
			if (m_cbGoodLuckType==0)
			{
				if (m_cbEndTime<20) m_cbEndTime=20;
			}
			else if (m_cbGoodLuckType>=1 && m_cbGoodLuckType<=3)
			{
				if (m_cbEndTime<27+5) m_cbEndTime=27+5;
			}
			else
			{
				if (m_cbEndTime<36) m_cbEndTime=36;
			}
			GameEnd.cbTimeLeave=m_cbEndTime;

			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) 
					continue;

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//��������
				GameEnd.lUserReturnScore=m_lUserJetton[wUserIndex];

				//����˰��
				if (m_lUserRevenue[wUserIndex]>0) 
					GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) 
					GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else 
					GameEnd.lRevenue=0;

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEAVE:		//�û��뿪
		{
			//�м��뿪
			if (m_wCurrentBanker!=wChairID)
			{
				if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
				{
//printf("��עʱ���뿪\n");
					//��������
					for (int i=0; i<JETTON_AREA_COUNT; i++) m_lUserAreaScore[wChairID][i]=0;
					m_lCurrentJetton[wChairID]=0;
					m_lUserWinScore[wChairID]=0;
					m_lUserJetton[wChairID]=0;
				}
				else
				{
//printf("�����ʱ���뿪\n");
					////////////////////////////////////////////////////////////////////////////////
					//�������ֵ
					//if (!pIServerUserItem->IsAndroidUser())
					//	m_StorageStart-=(m_lUserWinScore[wChairID]+m_lUserJetton[wChairID]);
					////////////////////////////////////////////////////////////////////////////////
					//����˰��
					if (0<(m_lUserWinScore[wChairID]+m_lUserJetton[wChairID]))
					{
						m_lUserRevenue[wChairID]=((m_lUserWinScore[wChairID]+m_lUserJetton[wChairID])/1000)*m_pGameServiceOption->cbRevenueRatio;
						m_lUserJetton[wChairID]-=m_lUserRevenue[wChairID];
					}
					////////////////////////////////////////////////////////////////////////////////

					//////////
					tagScoreInfo ScoreInfoArray[GAME_PLAYER];
					memset(ScoreInfoArray,0,sizeof(ScoreInfoArray));
					ScoreInfoArray[wChairID].lScore=m_lUserWinScore[wChairID]+m_lUserJetton[wChairID];
					ScoreInfoArray[wChairID].cbType=(m_lUserWinScore[wChairID]+m_lUserJetton[wChairID]>0)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					//////////

					//д�����
					if ((m_lUserWinScore[wChairID]+m_lUserJetton[wChairID])!=0L) 
						m_pITableFrame->WriteTableScore(ScoreInfoArray,GAME_PLAYER);
//printf("�÷֣�%I64d\n",m_lUserWinScore[wChairID]+m_lUserJetton[wChairID]);
					//��������
					for (int i=0; i<JETTON_AREA_COUNT; i++) m_lUserAreaScore[wChairID][i]=0;
					m_lCurrentJetton[wChairID]=0;
					m_lUserWinScore[wChairID]=0;
					m_lUserJetton[wChairID]=0;
				}

				return true;
			}

			//ׯ���뿪��״̬�ж�
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��[ %s ]ǿ�ˣ���Ϸ��ǰ������"),pIServerUserItem->GetNickName());

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//����ʱ��
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//�ɷ��˿�
				do
				{
					DispatchTableCard();
				} while (!ProbeJudge());
				//�����˺ſ���
				DispatchTableCardControl();

				//�������
				CalculateScore();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.iBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				GameEnd.lApplyBankerCondition=m_lApplyBankerCondition;
				if (m_lBankerWinScore>0) 
					GameEnd.lBankerScore=0;

				//����ʱ��
				m_cbEndTime=20;
				if (m_cbGoodLuckType==0)
				{
					if (m_cbEndTime<20) m_cbEndTime=20;
				}
				else if (m_cbGoodLuckType>=1 && m_cbGoodLuckType<=3)
				{
					if (m_cbEndTime<27+5) m_cbEndTime=27+5;
				}
				else
				{
					if (m_cbEndTime<36) m_cbEndTime=36;
				}
				GameEnd.cbTimeLeave=m_cbEndTime;

				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) 
						continue;

					//���óɼ�
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//��������
					GameEnd.lUserReturnScore=m_lUserJetton[wUserIndex];

					//����˰��
					if (m_lUserRevenue[wUserIndex]>0) 
						GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) 
						GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else 
						GameEnd.lRevenue=0;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//�۳�����
			if ((m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker])!=0)
			{
				////////////////////////////////////////////////////////////////////////////////
				//�������ֵ
				//if (!pIServerUserItem->IsAndroidUser())
				//	m_StorageStart-=(m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker]);
				////////////////////////////////////////////////////////////////////////////////
				//����˰��
				if (0<(m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker]))
				{
					m_lUserRevenue[m_wCurrentBanker]=((m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker])/1000)*m_pGameServiceOption->cbRevenueRatio;
					m_lUserJetton[m_wCurrentBanker]-=m_lUserRevenue[m_wCurrentBanker];
				}
				////////////////////////////////////////////////////////////////////////////////

				//////////
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				memset(ScoreInfoArray,0,sizeof(ScoreInfoArray));
				ScoreInfoArray[m_wCurrentBanker].lScore=m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker];
				ScoreInfoArray[m_wCurrentBanker].cbType=(m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker]>0)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				//////////

				//д�����
				if ((m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker])!=0L) 
					m_pITableFrame->WriteTableScore(ScoreInfoArray,GAME_PLAYER);
//printf("ׯ��ǿ�ˣ��÷֣�%I64d\n",m_lUserWinScore[m_wCurrentBanker]+m_lUserJetton[m_wCurrentBanker]);
				//��������
				for (int i=0; i<JETTON_AREA_COUNT; i++) m_lUserAreaScore[m_wCurrentBanker][i]=0;
				m_lCurrentJetton[m_wCurrentBanker]=0;
				m_lUserWinScore[m_wCurrentBanker]=0;
				m_lUserJetton[m_wCurrentBanker]=0;
			}

			//�л�ׯ��
			ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
//printf("wChiarID=%d\n",wChiarID);
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));	

			//�������ң�����������0
			//m_lCurrentJetton[wChiarID]=0;
			//�����˷���ȫ���Զ��һ��ɳ���
			if (pIServerUserItem->IsAndroidUser()) m_lCurrentJetton[wChiarID]=pIServerUserItem->GetUserInfo()->lScore;

			CopyMemory(StatusFree.lCurrentJetton,m_lCurrentJetton,sizeof(StatusFree.lCurrentJetton));
			StatusFree.dwChipRate=m_dwJettonRate;

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				StatusFree.lAreaLimitScore[i] = m_lAreaScoreLimit[i];

			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.wBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore();
				wcscpy(StatusFree.szBankerNickName,pIServerUserItem->GetNickName());
			}

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore); 
			}

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			memcpy(StatusFree.szRoomName,m_szGameRoomName,sizeof(StatusFree.szRoomName));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d, �������Ϊ��%I64d"),m_lApplyBankerCondition,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_PROMPT);
						
			//����������
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//�������ң�����������0
			//m_lCurrentJetton[wChiarID]=0;
			//�����˷���ȫ���Զ��һ��ɳ���
			if (pIServerUserItem->IsAndroidUser()) m_lCurrentJetton[wChiarID]=pIServerUserItem->GetUserInfo()->lScore;

			CopyMemory(StatusPlay.lCurrentJetton,m_lCurrentJetton,sizeof(StatusPlay.lCurrentJetton));
			StatusPlay.dwChipRate=m_dwJettonRate;

			//ȫ����ע
			for (int i=0; i<JETTON_AREA_COUNT; i++ )
			{
				StatusPlay.lTotalJettonScore[i] = m_lTotalJettonScore[i];
				StatusPlay.lAreaScoreLimit[i] = m_lAreaScoreLimit[i];
			}

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for(int i=0; i<JETTON_AREA_COUNT; i++)
					StatusPlay.lUserAreaScore[i] = m_lUserAreaScore[wChiarID][i];

				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;

			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	

			StatusPlay.cbWinArea=m_cbWinArea;
			StatusPlay.cbGoodLuckType=m_cbGoodLuckType;
			StatusPlay.cbPaoHuoCheCount=m_cbPaoHuoCheCount;
			CopyMemory(StatusPlay.cbPaoHuoCheArea,m_cbPaoHuoCheArea,sizeof(StatusPlay.cbPaoHuoCheArea));

			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIServerUserItem->GetUserScore();
				wcscpy(StatusPlay.szBankerNickName,pIServerUserItem->GetNickName());
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(m_cbBetTime-__min(dwPassTime,m_cbBetTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				StatusPlay.cbTimeLeave=(BYTE)(m_cbEndTime-__min(dwPassTime,m_cbEndTime));
				//���óɼ�
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//��������
				StatusPlay.lEndUserReturnScore=m_lUserJetton[wChiarID];

				//����˰��
				if (m_lUserRevenue[wChiarID]>0) 
					StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) 
					StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else 
					StatusPlay.lEndRevenue=0;

				//ׯ�ҳɼ�
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;
			}

			//����������
			SendApplyUser( pIServerUserItem );

			memcpy(StatusPlay.szRoomName,m_szGameRoomName,sizeof(StatusPlay.szRoomName));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_FREE:		//����ʱ�����
		{
			//��ʼ��Ϸ
			m_pITableFrame->StartGame();

			//������ע��ʱ��
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//������״̬Ϊ��ע�׶�
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ�����
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//������Ϸ
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) 
					continue;

				////////////////////////////////////////////////////////////////////////////////
				//�������ֵ
				//if (!pIServerUserItem->IsAndroidUser())
				//	m_StorageStart-=(m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID]);
				//else
				//	printf("�������ֵʱ���л�����\n");
				////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////
				//����˰��
				if (0<(m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID]))
				{
					m_lUserRevenue[wUserChairID]=((m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID])/1000)*m_pGameServiceOption->cbRevenueRatio;
					m_lUserJetton[wUserChairID]-=m_lUserRevenue[wUserChairID];
//printf("��ˮ��m_lUserRevenue[wUserChairID]=%I64d\n",m_lUserRevenue[wUserChairID]);
				}
				////////////////////////////////////////////////////////////////////////////////

				//////////
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				memset(ScoreInfoArray,0,sizeof(ScoreInfoArray));
				ScoreInfoArray[wUserChairID].lScore=m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID];
				ScoreInfoArray[wUserChairID].cbType=(m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID]>0)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				//////////

				//д�����
				if ((m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID])!=0L) 
					m_pITableFrame->WriteTableScore(ScoreInfoArray,GAME_PLAYER);
				////////////////////////////////////////////////////////////////////////////////
				//���µ�ǰ����
				m_lCurrentJetton[wUserChairID]+=m_lUserWinScore[wUserChairID]+m_lUserJetton[wUserChairID];
				CMD_S_ChipUpdate ChipUpdate;
				ZeroMemory(&ChipUpdate,sizeof(ChipUpdate));
				ChipUpdate.wChairID=wUserChairID;
				ChipUpdate.lTotalScore=m_lCurrentJetton[wUserChairID];
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHIP_UPDATE,&ChipUpdate,sizeof(ChipUpdate));//��Ϸ������ʱ��ʱ�ĸ���
				////////////////////////////////////////////////////////////////////////////////

				//��ׯ�����жϡ�һ����Ϸ���������������Ӯ��������Ҫ��������ׯ�����������Ǯ����������ׯ����������
				__int64 lCurrentJetton=m_lCurrentJetton[wUserChairID];
				if (wUserChairID!=m_wCurrentBanker && lCurrentJetton<m_lApplyBankerCondition)
				{
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_ApplyUserArray[i]) continue;

						//��ׯ�Ҷ���ɾ����������ׯ�����
						m_ApplyUserArray.RemoveAt(i);

						//������Ϣ
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						//���ñ���
						lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));
						CancelBanker.wChairID = pIServerUserItem->GetChairID();
						//������Ϣ
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//��ʾ��Ϣ
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_lApplyBankerCondition);
						SendGameMessage(wUserChairID,szTipMsg);

						break;
					}
				}
			}
			////////////////////////////////////////////////////////////////////////////////
			//�������Զ���ȡ��
			RobotAddMoney(m_lRobotMoneyLine,m_lRobotAddScore,m_lRoboMaxMoney,m_nSetlower,m_bAddMoney);
			////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////
			//�������ֵ
			//m_StorageStart=m_StorageStart-(m_StorageStart*m_StorageDeduct/1000);
			////////////////////////////////////////////////////////////////////////////////
//printf("m_StorageStart=%I64d\n",m_StorageStart/100);
//printf("m_StorageDeduct=%I64d\n",m_StorageDeduct);
			//������Ϸ
			m_pITableFrame->ConcludeGame(GER_NORMAL,NULL,608);//ע�����

			//�л�ׯ��
			ChangeBanker(false);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_cbFreeTime;
			GameFree.cbGameRecordCount=1;
			GameFree.cbGameRecord[0].cbRecord=m_cbWinArea;
			GameFree.cbGameRecord[0].dwGameTimes=m_dwGameTimes;
			GameFree.cbGameRecord[0].bGoodLuck=false;
			//////////
			//if ((m_cbWinArea==9 || m_cbWinArea==21) && m_cbGoodLuckType!=0)
			if (m_cbGameRecordMultipleCount>0)
			{
				//for (int i=0; i<m_cbPaoHuoCheCount; i++)
				for (int i=0; i<m_cbGameRecordMultipleCount; i++)
				{
					GameFree.cbGameRecordCount++;
					GameFree.cbGameRecord[i+1].cbRecord=m_GameRecordMultiple[i].cbRecord;
					GameFree.cbGameRecord[i+1].dwGameTimes=m_GameRecordMultiple[i].dwGameTimes;
					GameFree.cbGameRecord[i+1].bGoodLuck=m_GameRecordMultiple[i].bGoodLuck;
//printf("GameFree.cbGameRecord[i+1].cbRecord=%d\n",GameFree.cbGameRecord[i+1].cbRecord);
//printf("GameFree.cbGameRecord[i+1].dwGameTimes=%d\n",GameFree.cbGameRecord[i+1].dwGameTimes);
					//////////
					tagServerGameRecord &GameRecord=m_GameRecordArrary[m_nRecordLast];
					GameRecord.cbRecord=m_GameRecordMultiple[i].cbRecord;
					GameRecord.dwGameTimes=m_GameRecordMultiple[i].dwGameTimes;
					GameRecord.bGoodLuck=m_GameRecordMultiple[i].bGoodLuck;
					//�ƶ��±�
					m_nRecordLast=(m_nRecordLast+1)%MAX_SCORE_HISTORY;
					if (m_nRecordLast==m_nRecordFirst) m_nRecordFirst=(m_nRecordFirst+1)%MAX_SCORE_HISTORY;
					//////////
				}
				GameFree.cbGameRecord[0].dwGameTimes=GameFree.cbGameRecord[1].dwGameTimes-1;
				m_cbGameRecordMultipleCount=0;
//printf("GameFree.cbGameRecordCount=%d\n",GameFree.cbGameRecordCount);
			}
			//////////
			//////////
			DWORD AndroidCount=0;
			for (int i=0; i<m_ApplyUserArray.GetCount(); i++)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
				if (pIServerUserItem==NULL) continue;
				if (!pIServerUserItem->IsAndroidUser()) continue;
				AndroidCount++;
			}
			GameFree.wApplyUserCount=AndroidCount;
			//////////
			GameFree.lApplyBankerCondition=m_lApplyBankerCondition;
			GameFree.lBankerTotallScore=m_lBankerWinScore;
			GameFree.iBankerTime=m_wBankerTime;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			ReadConfigInformation(false);

			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_EXCHANGE:
		{
			CMD_C_Exchange * pExchange=(CMD_C_Exchange *)pData;
			if (pExchange->lExchangeScore>0)// && m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE)//����ʱ����ܶһ�
			{
				tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
				if (pUserData==NULL) return false;
				if (pUserData->lScore>=(m_lCurrentJetton[pUserData->wChairID]+pExchange->lExchangeScore))
				{
					CMD_S_ChipUpdate ChipUpdate;
					ZeroMemory(&ChipUpdate,sizeof(ChipUpdate));
					ChipUpdate.wChairID=pUserData->wChairID;
					ChipUpdate.lTotalScore=m_lCurrentJetton[pUserData->wChairID]+pExchange->lExchangeScore;
					m_lCurrentJetton[ChipUpdate.wChairID]=ChipUpdate.lTotalScore;//����
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHIP_UPDATE,&ChipUpdate,sizeof(ChipUpdate));//��Ҷһ�ʱ�ĸ���
				}
			}
			return true;
		}
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//CTime t1(2012,10,30,0,0,0);
			//time_t ct;  
			//time( &ct) ;/// ��ǰ���� 
			//CTime t2(ct);
			//if(t2>t1)
			//{
			//	return false;
			//}
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) 
				return false;

			if( pIServerUserItem == NULL )
				return true;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if( pUserData == NULL )
				return true;

			if (pUserData->cbUserStatus!=US_PLAYING) 
				return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pData;
			if( pPlaceJetton == NULL )
				return FALSE;

			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if( pUserData == NULL )
				return FALSE;

			if (pUserData->cbUserStatus==US_LOOKON)
				return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if( pUserData == NULL )
				return FALSE;
			if (pUserData->cbUserStatus==US_LOOKON)
				return true;

			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_BIG_SMALL:
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData==NULL) return false;
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			if (pUserData->wChairID==m_wCurrentBanker) return true;//ׯ�Ҳ��ܱȴ�С

			CMD_C_BigSmall * pBigSmall=(CMD_C_BigSmall *)pData;

			CMD_S_BigSmall BigSmall;
			memset(&BigSmall,0,sizeof(CMD_S_BigSmall));

			if (m_lUserWinScore[pUserData->wChairID]<=0) return true;

			do
			{
				m_cbBigSmall=rand()%14+1;
			} while (!ProbeJudge2(pBigSmall->cbBigSmall,pUserData->wChairID));

			BYTE cbWin=2;
			if (m_cbBigSmall>0 && m_cbBigSmall<8) cbWin=0;
			if (m_cbBigSmall>7 && m_cbBigSmall<15) cbWin=1;
			ASSERT(cbWin!=2);

			__int64 lBankerWinScore=0;

//printf("m_lUserJetton[pUserData->wChairID]=%I64d\n",m_lUserJetton[pUserData->wChairID]);
//printf("m_lUserWinScore[pUserData->wChairID]=%I64d\n",m_lUserWinScore[pUserData->wChairID]);
			if (pBigSmall->cbBigSmall==cbWin)//�¶Դ�С
			{
				//if (m_wCurrentBanker!=INVALID_CHAIR) m_lUserWinScore[m_wCurrentBanker]-=m_lUserWinScore[pUserData->wChairID];//ׯ�ҵ����ּ����мҳɼ���1��
				lBankerWinScore-=m_lUserWinScore[pUserData->wChairID];//ׯ�ҵ����ּ����мҳɼ���1��
				m_lUserWinScore[pUserData->wChairID]+=m_lUserWinScore[pUserData->wChairID];//�мҵĳɼ�����1��
				BigSmall.bWin=true;
			}
			else
			{
				//if (m_wCurrentBanker!=INVALID_CHAIR) m_lUserWinScore[m_wCurrentBanker]+=m_lUserWinScore[pUserData->wChairID];//ׯ�ҵ����������мҵĳɼ�
				lBankerWinScore+=m_lUserWinScore[pUserData->wChairID];//ׯ�ҵ����������мҵĳɼ�
				//m_lUserJetton[pUserData->wChairID]-=m_lUserWinScore[pUserData->wChairID];
				m_lUserWinScore[pUserData->wChairID]=0;//�мҵĳɼ�����Ϊ0
				BigSmall.bWin=false;
			}
			m_lBankerWinScore+=lBankerWinScore;
			//////////
			if (m_wCurrentBanker!=INVALID_CHAIR)
				m_lUserWinScore[m_wCurrentBanker]+=lBankerWinScore;
			//////////
//printf("m_lUserJetton[pUserData->wChairID]=%I64d\n",m_lUserJetton[pUserData->wChairID]);
//printf("m_lUserWinScore[pUserData->wChairID]=%I64d\n",m_lUserWinScore[pUserData->wChairID]);
			BigSmall.cbBigSmall=m_cbBigSmall;
			BigSmall.lUserScore=m_lUserWinScore[pUserData->wChairID];
			BigSmall.lBankerScore=0;
			m_pITableFrame->SendTableData(pUserData->wChairID,SUB_S_BIG_SMALL,&BigSmall,sizeof(BigSmall));

			return true;
		}
	case SUB_C_ADMIN_COMMDN:
		{
			ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
			if(wDataSize!=sizeof(CMD_C_ControlApplication)) return false;

			if (pIServerUserItem->GetUserRight()!=1879048192)
				return false;

			//��Ϣ����
			CMD_C_ControlApplication * pControlApplication=(CMD_C_ControlApplication *)pData;
			return OnAdminControl(pControlApplication,pIServerUserItem);
		}
	case SUB_C_GET_PLAYLIST:
		{
			return SendApplyBankerList(pIServerUserItem);
		}

	}

	//return false;
	return true;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//����ʱ��
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
	}
	//������ʾ
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("�˷�����ׯ����Ϊ��%I64d, �������Ϊ��%I64d"),m_lApplyBankerCondition,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_PROMPT);
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker) ChangeBanker(true);

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) 
				continue;
			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));
			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			break;
		}
		m_lCurrentJetton[wChairID]=0;
		m_lUserWinScore[wChairID]=0;
		m_lUserJetton[wChairID]=0;

		return true;
	}
	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=ID_APPLE)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_APPLE)||(lJettonScore<=0L)) 
		return true;//return false;

	//Ч��״̬
	//ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) 
		return true;

	//ׯ���жϣ������ׯ�ң���Ӧ����ע
	if (m_wCurrentBanker==wChairID) 
		return true;

	//�����ǰ������ϵͳ��ׯ����û��ׯ�ң��򷵻�
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
		return true;

	if( cbJettonArea > ID_APPLE || cbJettonArea < ID_BAR )
		return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//���������ڸ�����������ע������ܺ�
	__int64 lJettonCount = 0;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		lJettonCount += m_lUserAreaScore[wChairID][i];

#ifndef FUNC_CHIP
	//ȡ���������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if( pIServerUserItem == NULL )
		return false;
	//����ҵ�ǰ�ܹ����ж��ٻ��֣���ң�
	__int64 lUserScore = pIServerUserItem->GetUserScore();
	//�Ϸ�У�飬�������ע��ͼ�����ע������Ӵ�����Ҿ߱��Ļ��֣��򷵻�
	if (lUserScore < lJettonCount + lJettonScore) 
		//return true;
		bPlaceJettonSuccess=false;
#else
	if (m_lCurrentJetton[wChairID]<lJettonCount+lJettonScore)
		//return true;
		bPlaceJettonSuccess=false;
#endif

	//�������ע��ͼ�����ע������Ӵ�����ҵ���ע���򷵻�
	if (m_lUserLimitScore < lJettonCount + lJettonScore) 
		//return true;
		bPlaceJettonSuccess=false;
//printf("[%d]m_lUserLimitScore=%I64d\n",a,m_lUserLimitScore);

	//���ĳ����������ע���ϼ�����ע����С�ڵ��ڸ�����ǰ����ע���ޣ�����в��������򷵻�
	//if (m_lTotalJettonScore[cbJettonArea]+lJettonScore<=m_lAreaScoreLimit[cbJettonArea])//�����
	if (bPlaceJettonSuccess && lJettonScore<=m_lAreaScoreLimit[cbJettonArea])
	{
		m_lTotalJettonScore[cbJettonArea] += lJettonScore;
		m_lUserAreaScore[wChairID][cbJettonArea] += lJettonScore;
		m_lAreaScoreLimit[cbJettonArea] -= lJettonScore;
		//bPlaceJettonSuccess = true;
//#ifdef FUNC_CHIP
//		m_lCurrentJetton[wChairID]-=lJettonScore;
//#endif
	}
	else
	{
		bPlaceJettonSuccess=false;
	}
	//else
	//{
	//	return true;
	//}
//printf("lJettonScore=%I64d\n",lJettonScore);
//printf("m_lTotalJettonScore[cbJettonArea]=%I64d\n",m_lTotalJettonScore[cbJettonArea]);
//printf("m_lAreaScoreLimit[cbJettonArea]=%I64d\n",m_lAreaScoreLimit[cbJettonArea]);
//printf("bPlaceJettonSuccess=%d\n",bPlaceJettonSuccess);
	//��ע�ɹ�
	if (bPlaceJettonSuccess)
	{
		//��������
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//�������
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL)
			PlaceJetton.cbAndroid=pIServerUserItem->IsAndroidUser();
		CopyMemory(PlaceJetton.lTotalJettonScore,m_lTotalJettonScore,sizeof(PlaceJetton.lTotalJettonScore));

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	}
	else	//��עʧ��
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.cbJettonArea=cbJettonArea;
		PlaceJettonFail.lJettonScore=lJettonScore;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}

//�����˺ſ���
bool CTableFrameSink::DispatchTableCardControl()
{
	if (m_cbControlArea==AREA_9 || m_cbControlArea==AREA_21)
		return false;
	if (m_cbControlArea!=0xff && m_cbControlTimes>0)
	{
		if (m_cbControlArea<PATH_STEP_NUMBER)
		{
			m_cbWinArea=m_cbControlArea;
			m_cbControlTimes--;
		}
		if (m_cbControlTimes==0)
		{
			m_cbControlArea=0xff;
		}
	}
	return true;
}

//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	//srand((unsigned)time(NULL));
	DWORD dwRateGoodLuck=rand()%1000;
	if (dwRateGoodLuck<m_dwRateGoodLuck)
	{
		m_cbWinArea=(rand()%2==0)?AREA_9:AREA_21;
	}
	else
	{
		do
		{
			m_cbWinArea=rand()%PATH_STEP_NUMBER;//���24������
		} while (m_cbWinArea==AREA_9 || m_cbWinArea==AREA_21);
	}
	//m_cbWinArea=9;
	//m_cbWinArea=(rand()%2==0)?9:21;
	if (m_cbWinArea==AREA_9 || m_cbWinArea==AREA_21)//����GoodLuck
	{
		DWORD dwRateType=rand()%(m_dwRateChiFen+m_dwRateCaiJin+m_dwRateDaQiang);
		if (dwRateType<m_dwRateChiFen) m_cbGoodLuckType=0;
		else if (dwRateType<(m_dwRateChiFen+m_dwRateCaiJin)) m_cbGoodLuckType=rand()%4+1;
		else m_cbGoodLuckType=5;
		//m_cbGoodLuckType=rand()%6;//0���Է֡�1������Ԫ��2������ϲ��3��С��Ԫ��4���ܻ�
		//m_cbGoodLuckType=5;
		//static temp=0;
		//m_cbGoodLuckType=temp%5;
		//temp++;
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_0)//�Է�
		{
			m_cbPaoHuoCheCount=77;
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_1)//����Ԫ
		{
			m_cbPaoHuoCheCount=3;
			if (m_cbWinArea==AREA_9)
			{
				m_cbPaoHuoCheArea[0]=7;
				m_cbPaoHuoCheArea[1]=19;
				m_cbPaoHuoCheArea[2]=15;
			}
			if (m_cbWinArea==AREA_21)
			{
				m_cbPaoHuoCheArea[0]=19;
				m_cbPaoHuoCheArea[1]=15;
				m_cbPaoHuoCheArea[2]=7;
			}
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_2)//����ϲ
		{
			m_cbPaoHuoCheCount=4;
			if (m_cbWinArea==AREA_9)
			{
				m_cbPaoHuoCheArea[0]=4;
				m_cbPaoHuoCheArea[1]=22;
				m_cbPaoHuoCheArea[2]=16;
				m_cbPaoHuoCheArea[3]=10;
			}
			if (m_cbWinArea==AREA_21)
			{
				m_cbPaoHuoCheArea[0]=16;
				m_cbPaoHuoCheArea[1]=10;
				m_cbPaoHuoCheArea[2]=4;
				m_cbPaoHuoCheArea[3]=22;
			}
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_3)//С��Ԫ
		{
			m_cbPaoHuoCheCount=3;
			if (rand()%2==0)
			{
				if (m_cbWinArea==AREA_9)
				{
					m_cbPaoHuoCheArea[0]=1;
					m_cbPaoHuoCheArea[1]=18;
					m_cbPaoHuoCheArea[2]=12;
				}
				if (m_cbWinArea==AREA_21)
				{
					m_cbPaoHuoCheArea[0]=18;
					m_cbPaoHuoCheArea[1]=12;
					m_cbPaoHuoCheArea[2]=1;
				}
			}
			else
			{
				if (m_cbWinArea==AREA_9)
				{
					m_cbPaoHuoCheArea[0]=6;
					m_cbPaoHuoCheArea[1]=0;
					m_cbPaoHuoCheArea[2]=13;
				}
				if (m_cbWinArea==AREA_21)
				{
					m_cbPaoHuoCheArea[0]=13;
					m_cbPaoHuoCheArea[1]=6;
					m_cbPaoHuoCheArea[2]=0;
				}
			}
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_4)//�ܻ�
		{
			m_cbPaoHuoCheCount=rand()%6+2;//���2��7������
			//�¹涨���ܻ�Ϊ����������
			//�µ��㷨
			//10��11��12��13��14��15��16��17��18��19��20
			//22��23��0��1��2��3��4��5��6��7��8
			BYTE cbArea1[11]={10,11,12,13,14,15,16,17,18,19,20};
			BYTE cbArea2[11]={22,23,0,1,2,3,4,5,6,7,8};
			BYTE cbBegin=rand()%(12-m_cbPaoHuoCheCount);
			BYTE *cbArea=(rand()%2==0)?cbArea1:cbArea2;
			for (int i=0; i<m_cbPaoHuoCheCount; i++)
			{
				m_cbPaoHuoCheArea[i]=cbArea[cbBegin+i];
				ASSERT(m_cbPaoHuoCheArea[i]!=9 && m_cbPaoHuoCheArea[i]!=21);
			}
		}
		if (m_cbGoodLuckType==GOOD_LUCK_TYPE_5)//�����ǹ
		{
			m_cbPaoHuoCheCount=rand()%7+1;//���1��7������
			BYTE cbArea1[22]={8,7,6,5,4,3,2,1,0,23,22,20,19,18,17,16,15,14,13,12,11,10};
			BYTE cbArea2[22]={20,19,18,17,16,15,14,13,12,11,10,8,7,6,5,4,3,2,1,0,23,22};
			BYTE cbIndex=0;
			if (m_cbWinArea==AREA_9)
			{
				for (int i=0; i<m_cbPaoHuoCheCount; )
				{
					cbIndex+=rand()%3+3;
					BYTE &cbTemp=cbArea1[cbIndex%22];
					if (cbTemp!=255)
					{
						m_cbPaoHuoCheArea[i]=cbTemp;
						cbTemp=255;//�ų��ظ�����
						i++;
					}
				}
			}
			if (m_cbWinArea==AREA_21)
			{
				for (int i=0; i<m_cbPaoHuoCheCount; )
				{
					cbIndex+=rand()%3+3;
					BYTE &cbTemp=cbArea2[cbIndex%22];
					if (cbTemp!=255)
					{
						m_cbPaoHuoCheArea[i]=cbTemp;
						cbTemp=255;//�ų��ظ�����
						i++;
					}
				}
			}
		}
	}

	return true;
}

//���ڱȴ�С�Ŀ��ֵ����
bool CTableFrameSink::ProbeJudge2(BYTE cbGuess, WORD wChairID)
{
	//ϵͳ��Ӯ
	LONGLONG lSystemScore=0l;

	BYTE cbWin=2;
	if (m_cbBigSmall>0 && m_cbBigSmall<8) cbWin=0;
	if (m_cbBigSmall>7 && m_cbBigSmall<15) cbWin=1;
	ASSERT(cbWin!=2);

	bool bIsBankerAndroidUser=false;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem * pIBankerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem!=NULL)
		{
			bIsBankerAndroidUser=pIBankerUserItem->IsAndroidUser();
		}
	}

	//�Ƿ��ǻ�����
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	bool bIsAndroidUser=pIServerUserItem->IsAndroidUser();

	if (cbGuess==cbWin)//�¶Դ�С
	{
		if (bIsAndroidUser) lSystemScore+=m_lUserWinScore[wChairID];
		if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser) lSystemScore-=m_lUserWinScore[wChairID];
	}
	else
	{
		if (bIsAndroidUser) lSystemScore-=m_lUserWinScore[wChairID];
		if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser) lSystemScore+=m_lUserWinScore[wChairID];
	}

//printf("m_cbBigSmall=%d\n",m_cbBigSmall);
	if ((lSystemScore+m_StorageStart)<0l)
	{
		return false;
	}
	else
	{
		m_StorageStart+=lSystemScore;
//printf("lSystemScore=%I64d\n",lSystemScore/m_dwJettonRate);
//printf("m_StorageStart=%I64d\n",m_StorageStart/m_dwJettonRate);
		for (int i=0; i<GAME_PLAYER; i++)
		{
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;
			if (pIServerUserItem->GetUserRight()==1879048192)
			{
				CString szInfo;
				szInfo.Format(TEXT("Ѻ��С�����ֵ[%I64d]����"),m_StorageStart/m_dwJettonRate);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szInfo,SMT_CHAT);
			}
		}
		return true;
	}
}

//��̽���ж�
bool CTableFrameSink::ProbeJudge()
{
//printf(__FUNCTION__"\n\n");
	//�Ƿ�����ʵ�����ע
	bool bRealPlayerJetton=false;
	//ϵͳ��Ӯ
	LONGLONG lSystemScore=0l;
	//ׯ���ǲ��ǻ�����
	bool bIsBankerAndroidUser=false;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem * pIBankerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem!=NULL)
		{
			bIsBankerAndroidUser=pIBankerUserItem->IsAndroidUser();
//printf("��������ׯ\n");
		}
	}
	//�н���������
	BYTE cbHotArea=8;
	BYTE cbWinAreaMultiple=0;
	CalAreaMultiple(m_cbWinArea,cbHotArea,cbWinAreaMultiple);
	//�������
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) continue;
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;
		//�Ƿ��ǻ�����
		bool bIsAndroidUser=pIServerUserItem->IsAndroidUser();
		//��ͨ����
		for (int wAreaIndex=0; wAreaIndex<JETTON_AREA_COUNT; wAreaIndex++)
		{
			//////////
			//�ж��Ƿ�����ʵ�����ע
			if (!bIsAndroidUser && m_lUserAreaScore[i][wAreaIndex]>0) bRealPlayerJetton=true;
			//////////
			if (cbHotArea==wAreaIndex)
			{
				if (bIsAndroidUser)
					lSystemScore+=m_lUserAreaScore[i][wAreaIndex]*cbWinAreaMultiple;
				if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore-=m_lUserAreaScore[i][wAreaIndex]*cbWinAreaMultiple;
			}
			//�·ּ��۳��������������ӮûӮ����Ϊ����Ϸ��û�С�������ע�����˵����
			if (bIsAndroidUser)
				lSystemScore-=m_lUserAreaScore[i][wAreaIndex];
			if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
				lSystemScore+=m_lUserAreaScore[i][wAreaIndex];
		}
		//����GoodLuck
		if (cbHotArea==8)
		{
			switch (m_cbGoodLuckType)
			{
			case 0://�Է�
				{
					//for (int jj=0; jj<JETTON_AREA_COUNT; jj++)
					//{
					//	if (bIsAndroidUser)
					//		lSystemScore-=m_lUserAreaScore[i][jj];
					//	if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
					//		lSystemScore+=m_lUserAreaScore[i][jj];
					//}
					break;
				}
			case 1://����Ԫ
			case 2://����ϲ
			case 3://С��Ԫ
			case 4://�ܻ�
			case 5://�����ǹ
				{
					for (int jj=0; jj<m_cbPaoHuoCheCount; jj++)
					{
						BYTE cbHotArea2=8;
						BYTE cbWinAreaMultiple2=0;
						CalAreaMultiple(m_cbPaoHuoCheArea[jj],cbHotArea2,cbWinAreaMultiple2);
						ASSERT(cbHotArea2<8);
//printf("1:=%I64d\n",m_lUserAreaScore[i][cbHotArea2]*cbWinAreaMultiple2);
//printf("cbHotArea2=%d\n",cbHotArea2);
//printf("cbWinAreaMultiple2=%d\n",cbWinAreaMultiple2);
//printf("m_wCurrentBanker=%d\n",m_wCurrentBanker);
//printf("bIsAndroidUser=%d\n",bIsAndroidUser);
//printf("bIsBankerAndroidUser=%d\n",bIsBankerAndroidUser);
						if (bIsAndroidUser)
						{
							lSystemScore+=m_lUserAreaScore[i][cbHotArea2]*cbWinAreaMultiple2;
						}
						if (m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
						{
							lSystemScore-=m_lUserAreaScore[i][cbHotArea2]*cbWinAreaMultiple2;
						}
					}
				}
			}
		}
	}
	//ϵͳ��ֵ����
	if ((lSystemScore+m_StorageStart)<0l)
	{
		return false;
	}
	else
	{
		m_StorageStart+=lSystemScore;
//printf("m_StorageStart=%I64d\n",m_StorageStart/m_dwJettonRate);
		//����ʵ�����ע����˥�����ֵ
		if (bRealPlayerJetton)
			m_StorageStart=m_StorageStart-(m_StorageStart*m_StorageDeduct/1000);
//printf("lSystemScore=%I64d\n",lSystemScore/m_dwJettonRate);
//printf("m_StorageStart=%I64d\n",m_StorageStart/m_dwJettonRate);
		for (int i=0; i<GAME_PLAYER; i++)
		{
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;
			if (pIServerUserItem->GetUserRight()==1879048192)
			{
				CString szInfo;
				szInfo.Format(TEXT("���ֵ[%I64d]����"),m_StorageStart/m_dwJettonRate);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szInfo,SMT_CHAT);
			}
		}
		return true;
	}
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	if( pIApplyServerUserItem == NULL )
		return true;
#ifndef FUNC_CHIP
	//�Ϸ��ж�
	__int64 lUserScore=pIApplyServerUserItem->GetUserScore();
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"),SMT_PROMPT|SMT_EJECT);
		return true;
	}
#else
	//�Ϸ��ж�
	if (m_lCurrentJetton[pIApplyServerUserItem->GetUserInfo()->wChairID]<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("��ĳ��벻��������ׯ�ң�����ʧ�ܣ�"),SMT_PROMPT|SMT_EJECT);
		return true;
	}
#endif

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"),SMT_PROMPT|SMT_EJECT);
			return true;
		}
	}

	//������Ϣ 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//�������
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//���ñ���
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//�л��ж�
	if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
	{
		//������Ϣ
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_PROMPT|SMT_EJECT);
		return true;
	}

	//�����ж�
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//��������
		if (pIServerUserItem==NULL) 
			continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) 
			continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));
			CancelBanker.wChairID = pICancelServerUserItem->GetChairID();

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//�л�ׯ�� 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return true;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;

	//ȡ����ǰׯ����ׯ�ʸ�
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//��ȡ���
			WORD wChairID=m_ApplyUserArray[i];

			//��������
			if (wChairID!=m_wCurrentBanker) 
				continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//����ׯ��
		m_wCurrentBanker=INVALID_CHAIR;

		//�ֻ��ж�
		if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()!=0)
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//���ñ���
		bChangeBanker=true;
	}
	//��ׯ�жϡ���ǰׯ����ʱ���л�����ׯ
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if( pIServerUserItem == NULL )
			return FALSE;

		__int64 iBankerScore=pIServerUserItem->GetUserScore();

		//�����жϡ������������ׯ�Ĵ���������ʣ���ֲ���������ׯ��������ǿ���滻��
		if (m_nMaxBankerTimes<=m_wBankerTime || iBankerScore<m_lApplyBankerCondition)
		{
			//�������
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//��ȡ���
				WORD wChairID=m_ApplyUserArray[i];

				//��������
				if (wChairID!=m_wCurrentBanker) continue;

				//ɾ�����
				m_ApplyUserArray.RemoveAt(i);

				break;
			}

			//����ׯ��
			m_wCurrentBanker=INVALID_CHAIR;

			//�ֻ��ж�
			if (m_ApplyUserArray.GetCount()!=0)
			{
				m_wCurrentBanker=m_ApplyUserArray[0];
			}

			bChangeBanker=true;

			//��ʾ��Ϣ
			TCHAR szTipMsg[128];
			if (iBankerScore<m_lApplyBankerCondition)
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
			else
            	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),m_nMaxBankerTimes);

			//������Ϣ
			SendGameMessage(INVALID_CHAIR,szTipMsg);	
		}
	}
	//�����ǰ��ϵͳ��ׯ������Ի�һ�������������ׯ
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		m_wCurrentBanker=m_ApplyUserArray[0];
		bChangeBanker=true;
	}

	//�л��ж�
	if (bChangeBanker)
	{
		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			if( pIServerUserItem == NULL )
				return FALSE;
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
			wcscpy(ChangeBanker.szBankerNickName,pIServerUserItem->GetNickName());
			ChangeBanker.lBankerChip = m_lCurrentJetton[m_wCurrentBanker];
		}
		else
		{
			//���if�����������㣬Ӧ����δ���
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//��ȡ��Ϣ
			LONG lMessageCount=GetPrivateProfileInt(TEXT("Message"),TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//��ȡ����
				LONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%ld"),lIndex);
				GetPrivateProfileString(TEXT("Message"),szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//��ȡ���
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

				//������Ϣ
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetNickName());
				SendGameMessage(INVALID_CHAIR,szMessage2);
			}
		}
	}

	return bChangeBanker;
}

//����������ׯ������б�
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pServerUserItem == NULL) 
			continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) 
			continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));
		ApplyBanker.wApplyUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
	if (wChairID==m_wCurrentBanker) 
		ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) 
			continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//����ת�����Լ����㱶��
void CTableFrameSink::CalAreaMultiple(const BYTE cbWinArea, BYTE &cbOutArea, BYTE &cbMultiple)
{
	switch(cbWinArea)
	{
	case 5://2������ƻ��
		{
			cbOutArea=7;
			cbMultiple=2;
			break;
		}
	case 8://2����������
		{
			cbOutArea=3;
			cbMultiple=2;
			break;
		}
	case 11://2����������
		{
			cbOutArea=6;
			cbMultiple=2;
			break;
		}
	case 14://2������˫��
		{
			cbOutArea=1;
			cbMultiple=2;
			break;
		}
	case 17://2������ľ��
		{
			cbOutArea=5;
			cbMultiple=2;
			break;
		}
	case 20://2����������
		{
			cbOutArea=2;
			cbMultiple=2;
			break;
		}
	case 23://2����������
		{
			cbOutArea=4;
			cbMultiple=2;
			break;
		}
	case 0:
	case 12://����
		{
			cbOutArea = 6;
			cbMultiple=10;
			break;
		}
	case 1://����
	case 13:
		{
			cbOutArea = 4;
			cbMultiple=10;
			break;
		}
	case 2://BAR
		{
			cbOutArea=0;
			cbMultiple=25;
			break;
		}
	case 3://BAR
		{
			cbOutArea = 0;
			cbMultiple=50;
			break;
		}
	case 4://ƻ��
	case 10:
	case 16:
	case 22:
		{
			cbOutArea = 7;
			cbMultiple=5;
			break;
		}
	case 6://ľ��
	case 18:
		{
			cbOutArea = 5;
			cbMultiple=10;
			break;
		}
	case 7://����
		{
			cbOutArea = 3;
			cbMultiple=20;
			break;
		}
	case 9://����
	case 21:
		{
			cbOutArea = 8;
			cbMultiple=0;
			break;
		}
	case 15://˫��
		{
			cbOutArea = 1;
			cbMultiple=20;
			break;
		}
	case 19://����
		{
			cbOutArea = 2;
			cbMultiple=20;
			break;
		}
	}
}

//����÷�
__int64 CTableFrameSink::CalculateScore()
{
	////////////////////////////////////////////////////////////////////////////////
	//��Ϸ��¼
	tagServerGameRecord &GameRecord=m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbRecord=m_cbWinArea;
	GameRecord.dwGameTimes=m_dwGameTimes;
	GameRecord.bGoodLuck=false;//�������˴�����ֵ�����ᵼ��1000����¼�Ժ�LUCKYͼ����ܳ����ڷ�GoodLuck������
	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//////////
	//����GoodLuckʱ��������ʷ��¼
	if (m_cbWinArea==9 || m_cbWinArea==21)
	{
		if (m_cbGoodLuckType!=0)
		{
			m_cbGameRecordMultipleCount=m_cbPaoHuoCheCount;
			for (int i=0; i<m_cbPaoHuoCheCount; i++)
			{
				m_dwGameTimes++;
				//�ܼ�¼���������ﱣ�棬��Ϊ�ᵼ����Ϸδ����������Ϸ�н������ң�������Ϸ���
				//tagServerGameRecord &GameRecord=m_GameRecordArrary[m_nRecordLast];
				//GameRecord.cbRecord=m_cbPaoHuoCheArea[i];
				//GameRecord.dwGameTimes=m_lGameTimes;
				//GameRecord.bGoodLuck=true;
				////�ƶ��±�
				//m_nRecordLast=(m_nRecordLast+1)%MAX_SCORE_HISTORY;
				//if (m_nRecordLast==m_nRecordFirst) m_nRecordFirst=(m_nRecordFirst+1)%MAX_SCORE_HISTORY;
				m_GameRecordMultiple[i].cbRecord=m_cbPaoHuoCheArea[i];
				m_GameRecordMultiple[i].dwGameTimes=m_dwGameTimes;
				m_GameRecordMultiple[i].bGoodLuck=true;
//printf("m_GameRecordMultiple[i].cbRecord=%d\n",m_GameRecordMultiple[i].cbRecord);
//printf("m_GameRecordMultiple[i].lGameTimes=dwGameTimes=%d\n",m_GameRecordMultiple[i].lGameTimes=dwGameTimes);
			}
		}
	}
	//////////
	////////////////////////////////////////////////////////////////////////////////
	//ׯ�ұ��ֳɼ�
	__int64 lBankerWinScore=0;
	//��ҳɼ�
	__int64 lUserLostScore[GAME_PLAYER];
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore,sizeof(lUserLostScore));
	//�н���������
	BYTE cbHotArea=8;
	BYTE cbWinAreaMultiple=0;
	CalAreaMultiple(m_cbWinArea,cbHotArea,cbWinAreaMultiple);
//printf("cbHotArea=%d\n",cbHotArea);
//printf("cbWinAreaMultiple=%d\n",cbWinAreaMultiple);
	//�������
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) continue;
		//��ȡ�û�
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;
		//��ͨ����
		for(int iArea=0; iArea<JETTON_AREA_COUNT; iArea++)
		{
			if (cbHotArea==iArea)
			{
				m_lUserWinScore[i]+=m_lUserAreaScore[i][iArea]*cbWinAreaMultiple;
				lBankerWinScore-=m_lUserAreaScore[i][iArea]*cbWinAreaMultiple;
				//if (m_wCurrentBanker!=INVALID_CHAIR) m_lUserJetton[m_wCurrentBanker]-=m_lUserAreaScore[i][iArea]*cbWinAreaMultiple;
			}
			//lUserLostScore[i]-=m_lUserAreaScore[i][iArea];
			m_lUserJetton[i]-=m_lUserAreaScore[i][iArea];//�������ע�ĳ���
			lBankerWinScore+=m_lUserAreaScore[i][iArea];//ׯ������ʱ�õ���Щ����
		}

		//����GoodLuck
		if (cbHotArea==8)
		{
//printf("����GoodLuck\n");
//printf("m_cbGoodLuckTyp=%d\n",m_cbGoodLuckType);
			switch (m_cbGoodLuckType)
			{
			case 0://�Է�
				{
					m_lUserWinScore[i]=0;
					lBankerWinScore=0;//ׯ�Ҳ��䲻Ӯ
					break;
				}
			case 1://����Ԫ
			case 2://����ϲ
			case 3://С��Ԫ
			case 4://�ܻ�
			case 5://�����ǹ
				{
					ASSERT(m_lUserWinScore[i]==0);
					for (int jj=0; jj<m_cbPaoHuoCheCount; jj++)
					{
						BYTE cbHotArea2=8;
						BYTE cbWinAreaMultiple2=0;
						CalAreaMultiple(m_cbPaoHuoCheArea[jj],cbHotArea2,cbWinAreaMultiple2);
						ASSERT(cbHotArea2<8);
//printf("m_cbPaoHuoCheArea[jj]=%d\n",m_cbPaoHuoCheArea[jj]);
//printf("cbHotArea2=%d\n",cbHotArea2);
//printf("cbWinAreaMultiple2=%d\n",cbWinAreaMultiple2);
						m_lUserWinScore[i]+=m_lUserAreaScore[i][cbHotArea2]*cbWinAreaMultiple2;
					}
					break;
				}
			}
			lBankerWinScore-=m_lUserWinScore[i];
		}
		//��ˮ��д��ǰ
		//�ܵķ���
		//m_lUserWinScore[i] += lUserLostScore[i];
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker]=lBankerWinScore;
		//��ˮ��д��ǰ
	}

	//�ۼƻ���
	//m_lBankerWinScore+=lBankerWinScore+((m_wCurrentBanker==INVALID_CHAIR)?0:m_lUserJetton[m_wCurrentBanker]);
	m_lBankerWinScore+=lBankerWinScore;
	//��ǰ����
	m_lBankerCurGameScore=lBankerWinScore;
	return lBankerWinScore;
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	//BYTE cbBuffer[SOCKET_BUFFER];
	BYTE cbBuffer[4086];
	int nIndex = m_nRecordFirst;
	while ( nIndex != m_nRecordLast )
	{
		if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
			wBufferSize=0;
		}
		CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
		wBufferSize+=sizeof(tagServerGameRecord);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}
	if (wBufferSize>0) 
		m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//������Ϣ
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//��Ϸ���
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_PROMPT);
		}

		//�Թ����
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_PROMPT);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_PROMPT|SMT_EJECT);
	}
}

//�����˺ſ���
bool CTableFrameSink::OnAdminControl(CMD_C_ControlApplication * pData, IServerUserItem * pIServerUserItem)
{
//printf("�������ͣ�[%d]\n",pData->cbControlAppType);
//printf("��������[%d]\n",pData->cbControlArea);
//printf("���ƴ�����[%d]\n",pData->cbControlTimes);
	switch (pData->cbControlAppType)
	{
	case C_CA_UPDATE://����
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType=S_CR_UPDATE_SUCCES;
			ControlReturns.cbControlArea=m_cbControlArea;
			ControlReturns.cbControlTimes=m_cbControlTimes;
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_SET://����
		{
			if (pData->cbControlArea==0xff || pData->cbControlArea>PATH_STEP_NUMBER)
			{
				//����ʧ��
				CMD_S_ControlReturns ControlReturns;
				ZeroMemory(&ControlReturns,sizeof(ControlReturns));
				ControlReturns.cbReturnsType=S_CR_FAILURE;
				ControlReturns.cbControlArea=0xff;
				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
				return true;
			}
			m_cbControlArea=pData->cbControlArea;
			m_cbControlTimes=pData->cbControlTimes;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType=S_CR_SET_SUCCESS;
			ControlReturns.cbControlArea=m_cbControlArea;
			ControlReturns.cbControlTimes=m_cbControlTimes;
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_CANCELS://ȡ��
		{
			m_cbControlArea=0xff;
			m_cbControlTimes=0;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType=S_CR_CANCEL_SUCCESS;
			ControlReturns.cbControlArea=0xff;
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	}
	//����ʧ��
	CMD_S_ControlReturns ControlReturns;
	ZeroMemory(&ControlReturns,sizeof(ControlReturns));
	ControlReturns.cbReturnsType=S_CR_FAILURE;
	ControlReturns.cbControlArea=0xff;
	m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
	return true;
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SGDZ.ini"),szPath);
	//mystrcpy(m_szGameRoomName,TEXT("ˮ����԰"));
	mystrcpy(m_szGameRoomName,m_pGameServiceOption->szServerName);//��������

	//ÿ��ˢ��
	if (!bReadFresh && GetPrivateProfileInt(m_szGameRoomName,TEXT("Refresh"),0,m_szConfigFileName)==0) return;

	//////////
	//�������Զ���ȡ��
	m_lRobotMoneyLine=GetPrivateProfileInt(m_szGameRoomName,TEXT("RobotMoneyLine"),100000,m_szConfigFileName);
	m_lRobotAddScore=GetPrivateProfileInt(m_szGameRoomName,TEXT("RobotAddScore"),500000,m_szConfigFileName);
	m_lRoboMaxMoney=GetPrivateProfileInt(m_szGameRoomName,TEXT("RoboMaxMoney"),1000000,m_szConfigFileName);
	m_nSetlower=GetPrivateProfileInt(m_szGameRoomName,TEXT("Setlower"),10,m_szConfigFileName);
	BYTE cbAddMoney=GetPrivateProfileInt(m_szGameRoomName,TEXT("IsAdd"),0,m_szConfigFileName);
	m_bAddMoney=(cbAddMoney!=0)?true:false;
	//////////
//printf("m_lRobotMoneyLine=%I64d\n",m_lRobotMoneyLine);
//printf("m_lRobotAddScore=%I64d\n",m_lRobotAddScore);
//printf("m_lRoboMaxMoney=%I64d\n",m_lRoboMaxMoney);
	//ʱ�����
	m_cbFreeTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("FreeTime"), 10, m_szConfigFileName);
	m_cbBetTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetTime"), 20, m_szConfigFileName);
	m_cbEndTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("EndTime"), 20, m_szConfigFileName);

	if (m_cbFreeTime <= 0 || m_cbFreeTime > 99)
		m_cbFreeTime = 10;
	if (m_cbBetTime <= 0 || m_cbBetTime > 99)
		m_cbBetTime = 20;
	if (m_cbEndTime <= 0 || m_cbEndTime > 99)
		m_cbEndTime = 20;

	//���ʿ���
	m_dwRateGoodLuck=GetPrivateProfileInt(m_szGameRoomName,TEXT("GoodLuck"),5,m_szConfigFileName);
	m_dwRateChiFen=GetPrivateProfileInt(m_szGameRoomName,TEXT("ChiFen"),10,m_szConfigFileName);
	m_dwRateCaiJin=GetPrivateProfileInt(m_szGameRoomName,TEXT("CaiJin"),20,m_szConfigFileName);
	m_dwRateDaQiang=GetPrivateProfileInt(m_szGameRoomName,TEXT("DaQiang"),70,m_szConfigFileName);
	if (m_dwRateGoodLuck<0 || m_dwRateGoodLuck>1000) m_dwRateGoodLuck=5;
	if (m_dwRateChiFen<1 || m_dwRateChiFen>100) m_dwRateChiFen=100;
	if (m_dwRateCaiJin<1 || m_dwRateCaiJin>100) m_dwRateCaiJin=100;
	if (m_dwRateDaQiang<1 || m_dwRateDaQiang>100) m_dwRateDaQiang=100;
//printf("m_dwRateGoodLuck=%d\n",m_dwRateGoodLuck);
//printf("m_dwRateChiFen=%d\n",m_dwRateChiFen);
//printf("m_dwRateCaiJin=%d\n",m_dwRateCaiJin);
//printf("m_dwRateDaQiang=%d\n",m_dwRateDaQiang);

	//���Ʊ���
	__int64 lMaxScore=10000;
	CString str;

	//�������
	m_dwJettonRate=GetPrivateProfileInt(m_szGameRoomName,TEXT("CellScore"),100,m_szConfigFileName);
	if (m_dwJettonRate<=0) m_dwJettonRate=100;

	//������Ϣ
	TCHAR szBuffer[MAX_PATH];
	GetPrivateProfileString(m_szGameRoomName,TEXT("Score"),TEXT("50000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	myscanf(szBuffer,mystrlen(szBuffer),TEXT("%I64d"),&m_lApplyBankerCondition);

	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("SysLimitScore"),TEXT("10000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	myscanf(szBuffer,mystrlen(szBuffer),TEXT("%I64d"),&m_lSysLimitScore);

	GetPrivateProfileString(m_szGameRoomName,TEXT("UserLimitScore"),TEXT("800000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	myscanf(szBuffer,mystrlen(szBuffer),TEXT("%I64d"),&m_lUserLimitScore);

	BYTE cbEnableSysBanker=GetPrivateProfileInt(m_szGameRoomName, TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	BYTE cbCtrlWin = GetPrivateProfileInt(TEXT("ControlWin"), TEXT("Win"), 0, m_szConfigFileName);
	m_bControlWin = cbCtrlWin>0 ? true : false;

	//ׯ��һ����ׯ���پ�
	m_nMaxBankerTimes = GetPrivateProfileInt(m_szGameRoomName, TEXT("Time"), 10, m_szConfigFileName);

	if (bReadFresh)
	{
		m_StorageStart=GetPrivateProfileInt(m_szGameRoomName,TEXT("StorageStart"),0,m_szConfigFileName);
	}

	m_StorageDeduct=GetPrivateProfileInt(m_szGameRoomName,TEXT("StorageDeduct"),5,m_szConfigFileName);
//printf("m_StorageStart=%I64d\n",m_StorageStart);
//printf("m_StorageDeduct=%I64d\n",m_StorageDeduct);
	//��ȡ�����˺���Ϣ
	if( m_bControlWin)
	{
		int nAccountNum = GetPrivateProfileInt(TEXT("ControlWin"), TEXT("CtrlAccountNum"), 0, m_szConfigFileName);
		if( nAccountNum > 0 )
		{
			for( int i=0; i<nAccountNum; i++)
			{
				//TCHAR szTemp[64]={0};
				TCHAR szTemp[128]=_TEXT("");
				ControlAccount CtrlAccount;
				ZeroMemory(&CtrlAccount, sizeof(CtrlAccount));
				//sprintf(szTemp, "Account_%d", i);
				_sntprintf(szTemp,sizeof(szTemp),TEXT("Account_%d"), i);
				CtrlAccount.dwUserID = GetPrivateProfileInt(TEXT("ControlWin"), szTemp, -1, m_szConfigFileName);
				if( CtrlAccount.dwUserID >=0 )
				{
					ZeroMemory(szTemp, sizeof(szTemp));
					//sprintf(szTemp, "WinOdds_%d", i);
					_sntprintf(szTemp,sizeof(szTemp),TEXT("WinOdds_%d"), i);
					CtrlAccount.nOdds = GetPrivateProfileInt(TEXT("ControlWin"), szTemp, 0, m_szConfigFileName);
					m_CtrlAccountArray.Add(CtrlAccount);
				}
			}
		}
	}
}

//�������Զ���ȡ��
void CTableFrameSink::RobotAddMoney(__int64 lRobotMoneyLine, __int64 lRobotAddScore, __int64 lRoboMaxMoney, int nSetlower, bool bAddMoney)
{
	LONGLONG lMyscore,lBakScore;
	CString strtmp;
	if (bAddMoney)
	{
		for (WORD i=0; i<GAME_PLAYER; i++)
		{
			lBakScore=0;
			strtmp="����";
			IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pServerUserItem!=NULL && pServerUserItem->IsAndroidUser())
			{
				lMyscore=(LONGLONG)pServerUserItem->GetUserScore();//m_pITableFrame->GetTableUserItem(i)->GetUserScore()->lScore;
				//printf("lMyscore=%I64d\n",lMyscore);
				//printf("��%I64d\n",pServerUserItem->GetUserInsure());
				if (lMyscore<m_lRobotMoneyLine)
				{
					/*if ((LONGLONG)pServerUserItem->GetUserScore()<m_lRobotAddScore)
					{
						lBakScore=(LONGLONG)pServerUserItem->GetUserScore();
					}
					else*/
					{
						lBakScore=lMyscore + m_lRobotAddScore;
					}
					strtmp="�����Զ���";
					pServerUserItem->ModifyUserInsure(lBakScore,(-1)*lBakScore,0);
				}
				if (lMyscore>m_lRoboMaxMoney && m_wCurrentBanker!=i)
				{
					lBakScore=lMyscore*m_nSetlower/100;
					lMyscore=lMyscore-lBakScore;
					strtmp="�����Զ���";
					pServerUserItem->ModifyUserInsure((-1)*lBakScore,lBakScore,0);
				}
				m_lCurrentJetton[i]=pServerUserItem->GetUserInfo()->lScore;
				CMD_S_ChipUpdate ChipUpdate;
				ZeroMemory(&ChipUpdate,sizeof(ChipUpdate));
				ChipUpdate.wChairID=i;
				ChipUpdate.lTotalScore=m_lCurrentJetton[i];
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHIP_UPDATE,&ChipUpdate,sizeof(ChipUpdate));//�����˴�ȡ��ʱ�ĸ���
//printf("[%s]��%s[%I64d]������[%I64d]\n",pServerUserItem->GetNickName(),strtmp,lBakScore,m_lCurrentJetton[i]);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
