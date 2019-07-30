#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					10									//����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��
#define TIME_PLACE_JETTON			20									//��עʱ��

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��
#define TIME_GAME_END				20									//����ʱ��
//ׯ��ȡǮ
#define IDI_SENT_GET_MONEY			4									//ׯ��ȡǮ
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����

//////////////////////////////////////////////////////////////////////////
int  Findstr(char *buf,char findchar,int len)
{
	int i = 0;
	while (i<len)
	{
		if(buf[i++]==findchar)
			return (i-1);
	}
	return -1;
}
char* strGetbuffer(char *buf,int ipos,char *OutBuf,int outLen)
{
	if(ipos<outLen)
	memcpy(OutBuf,buf,ipos);
	else
	{
		memcpy(OutBuf,buf,outLen);
	}
	return  OutBuf;

}
void MakeStirngToStirng(char *buf,int len,char *outbuf,int &Outlen)
{
	
	size_t iPos = 0;
	Outlen =0;
	while (1)
	{
		iPos = Findstr(buf,',',(INT)strlen(buf));
		if(iPos==-1)
		{
			char tmp[256]={0};
			char *strtmp = buf;
			outbuf[Outlen++] = atoi(strtmp);
			break;
		}
		char tmp[256]={0};
		char *strtmp = strGetbuffer(buf,(INT)iPos,tmp,256);
		outbuf[Outlen++] = atoi(strtmp);
		buf = buf+iPos+1;
	}


}
//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	m_CheckImage = 0;

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//״̬����
	m_dwJettonTime=0L;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//���Ʊ���
	m_StorageStart = 0l;
	m_StorageDeduct = 0l;

	//�����˿���					
	m_lRobotAreaLimit = 0l;
	m_lRobotBetCount = 0l;										

	//ׯ������
	m_lBankerMAX = 0l;
	m_lBankerAdd = 0l;							
	m_lBankerScoreMAX = 0l;
	m_lBankerScoreAdd = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;

	//ϵͳ��Ӯ����
	m_nWinMaxPercent=80;
	m_nWinMinPercent=20;

	//ʱ�����
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//�������
	m_hInst = NULL;
	m_pServerContro = NULL;
	m_hInst = LoadLibrary(TEXT("BumperCarBattleServerControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}

	m_bControl = false;
	m_nRobotApplyBanker = 0;

	m_lAllScore = 0;
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	if( m_pServerContro )
	{
		delete m_pServerContro;
		m_pServerContro = NULL;
	}

	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
}

//�ӿڲ�ѯ
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
    QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//���ƽӿ�
//	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
//	if (m_pITableFrameControl==NULL) return false;

	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);
	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));
	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\BumperCarBattle.ini"),szPath);


	ReadConfigInformation(true);

	return true;
}

//��λ����
VOID  CTableFrameSink::RepositionSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//������
	m_nRobotApplyBanker = GetPrivateProfileInt(m_szGameRoomName, _T("RobotApplyBanker"), 3, m_szConfigFileName);
	TCHAR OutBuf[255];
	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageOff"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%0.2f"),&m_StorageOff);
	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageCritical"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%0.2f"),&m_StorageCritical);
	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageOffEx"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%0.2f"),&m_StorageOffEx);
	m_bControl = false;
	//��ע��������Ŀ
	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;

	m_lAllScore = 0;
	return;
}



//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		return 0L;
	}
	else
	{
		return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinEnterScore-10L, 0L);
	}
}

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;

		if(wChairID==m_wCurrentBanker) return true;

		if(wChairID==i)
		{
			//������ע
			for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
			{

				if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
				{
					return true;
				}
			}
			break;
		}
	}
	return false;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
	{
		pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		m_lBankerScore=pIBankerServerUserItem->GetUserScore();
	}

	//���ñ���
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	GameStart.lBankerScore = 0x7fffffffffffffff;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore();

	GameStart.bContiueCard=m_bContiueCard;
	
	//��ע����������
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	//�����˿���
	m_nChipRobotCount = 0;

	//�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
		{
			GameStart.lBankerScore = 1;
		}

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
		{
			//����
			if ( m_pServerContro != NULL && m_pServerContro->NeedControl() )
			{
				tagControlInfo ControlInfo;
				m_pServerContro->ReturnControlArea(ControlInfo);
				m_cbTableCardArray[0][0] = ControlInfo.cbControlArea;
				m_pServerContro->CompleteControl();
				m_bControl = true;

				//�����ע
				LONGLONG *pUserScore[AREA_COUNT+1];
				pUserScore[0] = NULL;
				for (int i = 1;i<AREA_COUNT+1;i++)
				{
					pUserScore[i] = m_lUserJettonScore[i];
				}
				for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
				{
					//ׯ���ж�
					if (m_wCurrentBanker == wChairID) continue;

					//��ȡ�û�
					IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
					if (pIServerUserItem==NULL) 
						continue;
					if (pIServerUserItem->IsAndroidUser()) 
						continue;
					for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
					{
						m_lAllScore += pUserScore[wAreaIndex][wChairID];
					}
				}
			}
			else
			{
				while(true)
				{
					//�ɷ��˿�
					DispatchTableCard();
					//��̽���ж�
					if (ProbeJudge())
					{
						break;
					}
				}
			}
			
			//�������
			SCORE lBankerWinScore=CalculateScore();

			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ׯ����Ϣ
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;
			GameEnd.bcFirstCard = m_bcFirstPostCard;
			
			//�˿���Ϣ
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			GameEnd.cbLeftCardCount=m_cbLeftCardCount;

			//���ͻ���
			GameEnd.cbTimeLeave=m_cbEndTime;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//��������
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//����˰��
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEAVE:		//�û��뿪
		{
			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				//��������
				SCORE lScore=0;
				SCORE lRevenue=0;
			

				//ͳ�Ƴɼ�
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lScore -= m_lUserJettonScore[nAreaIndex][wChairID];

				//д�����
				if (m_pITableFrame->GetGameStatus() != GS_GAME_END)
				{
					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					{
						if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
						{
							CMD_S_PlaceJettonFail PlaceJettonFail;
							ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
							PlaceJettonFail.lJettonArea=nAreaIndex;
							PlaceJettonFail.lPlaceScore=m_lUserJettonScore[nAreaIndex][wChairID];
							PlaceJettonFail.wPlaceUser=wChairID;

							//��Ϸ���
							for (WORD i=0; i<GAME_PLAYER; ++i)
							{
								IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
								if (pIServerUserItem==NULL) 
									continue;

								m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
							}

							m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[nAreaIndex][wChairID];
							m_lUserJettonScore[nAreaIndex][wChairID] = 0;
						}
					}
				}
				else
				{

					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					{
						if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
						{
							m_lUserJettonScore[nAreaIndex][wChairID] = 0;
						}
					}

					
					//д�����
					if (m_lUserWinScore[wChairID]!=0L) 
					{
						tagScoreInfo ScoreInfo[GAME_PLAYER];
						ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
						ScoreInfo[wChairID].cbType=m_lUserWinScore[wChairID]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;;
						ScoreInfo[wChairID].lRevenue=m_lUserRevenue[wChairID];
						ScoreInfo[wChairID].lScore=m_lUserWinScore[wChairID];
						m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
						//m_pITableFrame->WriteUserScore(pIServerUserItem,m_lUserWinScore[wChairID], static_cast<long>(m_lUserRevenue[wChairID]), ScoreKind);
						m_lUserWinScore[wChairID] = 0;
					}
				}
				return true;
			}

			//״̬�ж�
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
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//����
				if ( m_pServerContro != NULL && m_pServerContro->NeedControl() )
				{
					tagControlInfo ControlInfo;
					m_pServerContro->ReturnControlArea(ControlInfo);
					m_cbTableCardArray[0][0] = ControlInfo.cbControlArea;
					m_pServerContro->CompleteControl();
					m_bControl = true;
					//�����ע
					LONGLONG *pUserScore[AREA_COUNT+1];
					pUserScore[0] = NULL;
					for (int i = 1;i<AREA_COUNT+1;i++)
					{
						pUserScore[i] = m_lUserJettonScore[i];
					}
					for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
					{
						//ׯ���ж�
						if (m_wCurrentBanker == wChairID) continue;

						//��ȡ�û�
						IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
						if (pIServerUserItem==NULL) 
							continue;
						if (pIServerUserItem->IsAndroidUser()) 
							continue;
						for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
						{
							m_lAllScore += pUserScore[wAreaIndex][wChairID];
						}
					}
				}
				else
				{
					while(true)
					{
						//�ɷ��˿�
						DispatchTableCard();

						//��̽���ж�
						if (ProbeJudge())
						{
							break;
						}
					}
				}

				//�������
				CalculateScore();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (m_lBankerWinScore>0) GameEnd.lBankerScore=0;

				//�˿���Ϣ
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				GameEnd.cbLeftCardCount=m_cbLeftCardCount;

				//���ͻ���
				GameEnd.cbTimeLeave=m_cbEndTime;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//���óɼ�
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//��������
					GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

					//����˰��
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//�۳�����
			if (m_lUserWinScore[m_wCurrentBanker] != 0l)
			{
				tagScoreInfo ScoreInfo[GAME_PLAYER];
				ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
				ScoreInfo[wChairID].cbType=m_lUserWinScore[m_wCurrentBanker]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;;
				ScoreInfo[wChairID].lRevenue=m_lUserRevenue[m_wCurrentBanker];
				ScoreInfo[wChairID].lScore=m_lUserWinScore[m_wCurrentBanker];
				m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

				//m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
				m_lUserWinScore[m_wCurrentBanker] = 0;
			}

			//�л�ׯ��
			ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			StatusFree.CheckImage = m_CheckImage;

			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			memset(StatusFree.szBankerNickName, 0, sizeof(StatusFree.szBankerNickName));
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			StatusFree.lBankerScore = 0x7fffffffffffffff;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				wcscpy(StatusFree.szBankerNickName,pIServerUserItem->GetNickName());
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore();
			}

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore*4); 
			}

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));


			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
						
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
			CMD_S_StatusPlay StatusPlay={0};

			//ȫ����ע
//			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));
			for (int i = 0; i < (AREA_COUNT+1); i++)
			{
				StatusPlay.lAllJettonScore[i] = m_lAllJettonScore[i];
			}

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];

				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			memset(StatusPlay.szBankerNickName, 0, sizeof(StatusPlay.szBankerNickName));
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			StatusPlay.CheckImage = m_CheckImage;
			StatusPlay.lBankerScore = 0x7fffffffffffffff;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				StatusPlay.lBankerScore=m_lBankerScore;
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				wcscpy(StatusPlay.szBankerNickName,pIServerUserItem->GetNickName());
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			int	nTotalTime = (cbGameStatus==GS_PLACE_JETTON?m_cbBetTime:m_cbEndTime);
			StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,(DWORD)nTotalTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();						

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				StatusPlay.cbTimeLeave=(BYTE)(m_cbEndTime-__min(dwPassTime,m_cbEndTime));

				//���óɼ�
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//��������
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

				//����˰��
				if (m_lUserRevenue[wChiarID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//ׯ�ҳɼ�
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				//�˿���Ϣ
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			}

			//��������
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			
			//����������
			SendApplyUser( pIServerUserItem );

			return bSuccess;
		}
	}

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{
			//��ʼ��Ϸ
			m_pITableFrame->StartGame();

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//������Ϸ
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);			
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pIServerUserItem != NULL)
				{
					if (pIServerUserItem->IsAndroidUser())
					{
						m_pITableFrame->SetGameTimer(IDI_SENT_GET_MONEY,(m_cbEndTime-1)*1000,1,0L);	
					}
				}
			}

			return true;
		}
	case IDI_SENT_GET_MONEY:
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			if (pIServerUserItem != NULL) m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TO_ANDROID);
			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));

			LONGLONG lSystemScore = 0;
			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				
				ScoreInfo[wUserChairID].cbType=m_lUserWinScore[wUserChairID]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo[wUserChairID].lRevenue=m_lUserRevenue[wUserChairID];
				ScoreInfo[wUserChairID].lScore=m_lUserWinScore[wUserChairID];
				//д�����
			//	if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);
			//�����
			if (!pIServerUserItem->IsAndroidUser())
				lSystemScore += m_lUserWinScore[wUserChairID];
			}
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			m_StorageStart -= lSystemScore;
			bool bCritical = false;
			if (m_lAllScore < m_StorageCritical)
			{
				m_StorageStartEx -= lSystemScore;
				bCritical = true;
			}
			//���˥��
			if (m_StorageStart > 0)
				m_StorageStart = m_StorageStart - m_StorageStart*m_StorageDeduct/1000;

			TCHAR szStorage[1024]=TEXT("");
			_sntprintf(szStorage,CountArray(szStorage),TEXT("ԭ��棺%I64d,���Ӯ��%I64d,���ʣ�ࣺ%I64d,�ٽ�ֵ��%I64d,С��棺%I64d"),m_StorageStart +lSystemScore,lSystemScore,m_StorageStart,m_StorageCritical,m_StorageStartEx);
			CTraceService::TraceString(szStorage,TraceLevel_Info);

			for(int i=0; i<MAX_CHAIR; i++)
			{
				IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i);
				if(pUserItem==NULL) continue;
				if(pUserItem->IsAndroidUser()) continue;
				if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;

				TCHAR strMessage[256]=TEXT("");
				_sntprintf(strMessage, sizeof(strMessage), _T("���ֽ���:%s,%s,%s"),szStorage,m_bControl?TEXT("�����п���"):TEXT(""),bCritical?TEXT("�ٽ����"):TEXT(""));
				m_pITableFrame->SendGameMessage(pUserItem,strMessage,SMT_CHAT);
				m_bControl = false;

			}

			if (m_StorageOff != 0)
			{
				m_StorageStart -= m_StorageOff;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("��̨����������%I64d�����ʣ�ࣺ%I64d"),m_StorageOff, m_StorageStart);
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				WritePrivateProfileString(m_szGameRoomName,TEXT("StorageOff"),TEXT("0"),m_szConfigFileName);

				for(int i=0; i<MAX_CHAIR; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i);
					if(pUserItem==NULL) continue;
					if(pUserItem->IsAndroidUser()) continue;
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_CHAT);

				}
			}

			if (m_StorageOffEx != 0)
			{
				m_StorageStartEx -= m_StorageOffEx;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("��̨����������%I64d��С���ʣ�ࣺ%I64d"),m_StorageOffEx, m_StorageStartEx);
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				WritePrivateProfileString(m_szGameRoomName,TEXT("StorageOffEx"),TEXT("0"),m_szConfigFileName);

				for(int i=0; i<MAX_CHAIR; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i);
					if(pUserItem==NULL) continue;
					if(pUserItem->IsAndroidUser()) continue;
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_CHAT);

				}
			}

			CString strGameRecord ;
			strGameRecord.Format(L"��Ϸ����");
			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,strGameRecord,608);

			//��ȡ����
			if (m_bRefreshCfg)
				ReadConfigInformation(false);

			for(int i =0;i < m_ApplyUserArray.GetCount();i++)
			{
				WORD wApplyChairID = m_ApplyUserArray.GetAt(i);
				IServerUserItem *pApplyUser = m_pITableFrame->GetTableUserItem(wApplyChairID);
				if(pApplyUser->GetUserScore()<m_lApplyBankerCondition)
				{
					m_ApplyUserArray.RemoveAt(i);

					//��ʾ��Ϣ
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ������%lf��������ׯ����������%I64d��,�ѳ���������ׯ��"),
						pApplyUser->GetUserScore(), m_lApplyBankerCondition);
					SendGameMessage(wApplyChairID,szTipMsg);

					//�������
					CMD_S_CancelBanker CancelBanker;
					ZeroMemory(&CancelBanker,sizeof(CancelBanker));

					//���ñ���
					lstrcpyn(CancelBanker.szCancelUser,pApplyUser->GetNickName(),CountArray(CancelBanker.szCancelUser));
					CancelBanker.wChairID=wApplyChairID;

					//������Ϣ
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				}
			}

			//�л�ׯ��
			ChangeBanker(false);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));

			GameFree.cbTimeLeave = m_cbFreeTime;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//�û�Ч��
			
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pIServerUserItem->GetChairID(),pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()==US_LOOKON) return true;
			OnUserApplyBanker(pIServerUserItem);
			return 	true;
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()==US_LOOKON) return true;
			OnUserCancelBanker(pIServerUserItem);
			return true;	
		}
	case SUB_C_CONTINUE_CARD:		//��������
		{
			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()==US_LOOKON) return true;
			if (pIServerUserItem->GetChairID()!=m_wCurrentBanker) return true;
			if (m_cbLeftCardCount < 8) return true;

			//���ñ���
			m_bContiueCard=true;

			//������Ϣ
			SendGameMessage(pIServerUserItem->GetChairID(),TEXT("���óɹ�����һ�ֽ��������ƣ�"));

			return true;
		}
	case SUB_C_CHECK_IMAGE:		//�û�ѡ�񱳾�ͼ
		{
			ASSERT(wDataSize==sizeof(CMD_C_CheckImage));
			if (wDataSize!=sizeof(CMD_C_CheckImage)) return false;

			CMD_C_CheckImage * pCheckImage=(CMD_C_CheckImage *)pDataBuffer;
			this->m_CheckImage = pCheckImage->Index;
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHECK_IMAGE, pCheckImage, sizeof(CMD_C_CheckImage));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CHECK_IMAGE, pCheckImage, sizeof(CMD_C_CheckImage));
			return true;

		}
	case SUB_C_GET_ACCOUNT:
		{
// 			if (!IsAdmin(pIServerUserItem))
// 			{
// 				return false;
// 			}
			CMD_S_AccountResult Result;
			ZeroMemory(&Result,sizeof(Result));
			INT iCount=0;
			for(int i=0; i<MAX_CHAIR; i++)
			{
				IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i);
				if(pUserItem==NULL) continue;
				if(pUserItem->IsAndroidUser()) continue;

				CopyMemory(Result.szAccount[iCount],pUserItem->GetNickName(),sizeof(Result.szAccount[iCount]));

				iCount++;
			}

			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ACCOUNT_RESULT,&Result,sizeof(Result));

			return true;
		}
	case SUB_C_CHECK_ACCOUNT:
		{
// 			if (!IsAdmin(pIServerUserItem))
// 			{
// 				return false;
// 			}
			ASSERT(wDataSize==sizeof(CMD_C_CheakAccount));
			if (wDataSize!=sizeof(CMD_C_CheakAccount)) return false;

			CMD_C_CheakAccount * pCheck=(CMD_C_CheakAccount*)pDataBuffer;
			for(int i=0; i<MAX_CHAIR; i++)
			{
				IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i);
				if(pUserItem==NULL) continue;
				TRACE("pUserItem->GetAccounts()=%s,szUserAccount=%s\n",pUserItem->GetNickName(),pCheck->szUserAccount);
				if(_tcscmp(pUserItem->GetNickName(),pCheck->szUserAccount)==0)
				{
					CMD_S_ScoreResult Score;
					ZeroMemory(&Score,sizeof(Score));
					for(int j=0; j<AREA_COUNT+1; j++)Score.lUserJettonScore[j]=m_lUserJettonScore[j][i];

					m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SCORE_RESULT,&Score,sizeof(Score));
					break;
				}
			}


			return true;
		}
	case SUB_C_ADMIN_COMMDN:
		{
// 			if (!IsAdmin(pIServerUserItem))
// 			{
// 				return false;
// 			}
			ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
			if(wDataSize!=sizeof(CMD_C_ControlApplication)) return false;
			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;
			if ( m_pServerContro == NULL) return true;
			return m_pServerContro->ServerControl(wSubCmdID, pDataBuffer, wDataSize, pIServerUserItem, m_pITableFrame);
		}
	case SUB_C_GET_PLAYLIST:
		{
			return SendApplyBankerList(pIServerUserItem);
		}
}


	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
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
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));
			CancelBanker.wChairID=i;

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=AREA_COUNT && cbJettonArea>=1)&&(lJettonScore>0L));
	if ((cbJettonArea>AREA_COUNT)||(lJettonScore<=0L) || cbJettonArea<1)
	{
		return false;
	}

	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
		return true;
	}

	//ׯ���ж�
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	SCORE lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];

	//��һ���
	SCORE lUserScore = pIServerUserItem->GetUserScore();

	//�Ϸ�У��
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if (GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
	{
		//��������֤
		if(pIServerUserItem->IsAndroidUser())
		{
			//��������
			if (m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
				return true;

			//��Ŀ����
			bool bHaveChip = false;
			for (int i = 0; i < AREA_COUNT; i++)
			{
				if (m_lUserJettonScore[i+1][wChairID] != 0)
					bHaveChip = true;
			}

			if (!bHaveChip)
			{
				if (m_nChipRobotCount+1 > m_nMaxChipRobot)
				{
					bPlaceJettonSuccess = false;
				}
				else
					m_nChipRobotCount++;
			}

			//ͳ�Ʒ���
			if (bPlaceJettonSuccess)
				m_lRobotAreaScore[cbJettonArea] += lJettonScore;
		}

		if (bPlaceJettonSuccess)
		{
			//������ע
			m_lAllJettonScore[cbJettonArea] += lJettonScore;
			m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;			
		}	
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if (bPlaceJettonSuccess)
	{
		//��������
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//�������
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem != NULL) 
			PlaceJetton.cbAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		CString strArea[8]={L"��ʱ��",L"����",L"�󱼳�",L"�����",L"��ʱ��",L"����",L"����",L"����"};
		for(int i=0; i<MAX_CHAIR; i++)
		{
			IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i);
			if(pUserItem==NULL) continue;
			if(pUserItem->IsAndroidUser()) continue;
			if(pIServerUserItem->IsAndroidUser()) continue;
			if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;
			TCHAR strMessage[256]=TEXT("");
			myprintf(strMessage, sizeof(strMessage), _T("%s:[%s]��ע%.2lf"), pIServerUserItem->GetUserInfo()->szNickName,strArea[cbJettonArea-1],lJettonScore);
			m_pITableFrame->SendGameMessage(pUserItem,strMessage,SMT_CHAT);

		}
	}
	else
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}
void CTableFrameSink::RandList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{

	//����׼��
	BYTE *cbCardData = new BYTE[cbBufferCount];
	CopyMemory(cbCardData,cbCardBuffer,cbBufferCount);

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	delete []cbCardData;
	cbCardData = NULL;

	return;
}


//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	INT cbControlArea[32] = { 1, 9, 17, 25,   3, 11, 19, 27,   5, 13, 21, 29,   7, 15, 23, 31,   2, 10, 18, 26,   4, 12, 20, 28,   6, 14, 22, 30,   8, 16, 24, 32 };
	INT cbnChance[32]	  = { 1, 1,  1,  1,   2,  2,  2,  2,   2,  2,  2,  2,   3,  3,  3,  3,   10,10, 10, 10,  10, 10, 10, 10,  10, 10, 10, 10,  10, 10, 10, 10 };

	m_GameLogic.ChaosArray(cbControlArea, CountArray(cbControlArea), cbnChance, CountArray(cbnChance));

	//�������
	DWORD wTick = GetTickCount();

	//���ʺ�ֵ
	INT nChanceAndValue = 0;
	for ( int n = 0; n < CountArray(cbnChance); ++n )
	{
		nChanceAndValue += cbnChance[n];
	}

	int nMuIndex = 0;
	int nRandNum = 0;					//�������
	static int nStFluc = 1;				
	nRandNum = (rand() + wTick + nStFluc*3 ) % nChanceAndValue;
	for (int j = 0; j < CountArray(cbnChance); j++)
	{
		nRandNum -= cbnChance[j];
		if (nRandNum < 0)
		{
			nMuIndex = j;
			break;
		}
	}
	nStFluc = nStFluc%3 + 1;

	m_cbTableCardArray[0][0] = cbControlArea[nMuIndex];
	m_cbCardCount[0] = 1;

	//���Ʊ�־
	m_bContiueCard = false;

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	SCORE lUserScore=pIApplyServerUserItem->GetUserScore();
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"),SMT_CHAT|SMT_EJECT);
		return true;
	}

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"),SMT_CHAT|SMT_EJECT);
			return true;
		}
	}

	if (pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>rand()%m_nRobotApplyBanker)//��Ϊ��Χ�ڵ����ֵ
	{
		return true;
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
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_CHAT|SMT_EJECT);
		return true;
	}

	//�����ж�
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//��������
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			CancelBanker.wChairID=wChairID;

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

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

	return false;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;

	//ȡ����ǰ
	if (bCancelCurrentBanker)
	{
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
		TakeTurns();

		//���ñ���
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}
	//��ׯ�ж�
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

		if(pIServerUserItem!= NULL)
		{
			SCORE lBankerScore=pIServerUserItem->GetUserScore();

			//�����ж�
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker && lBankerScore>=m_lApplyBankerCondition)
				{
					//��ׯ�������ã���ׯ�������趨�ľ���֮��(m_lBankerMAX)��
					//�������ֵ��������������ׯ���б�����������ҽ��ʱ��
					//�����ټ���ׯm_lBankerAdd�֣���ׯ���������á�

					//��ҳ���m_lBankerScoreMAX֮��
					//������������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_lBankerScoreAdd�֡�
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//��ȡ���
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						SCORE lScore = pIUserItem->GetUserScore();

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l) )
					{
						if ( bScoreMAX )
						{
							m_lPlayerBankerMAX += m_lBankerAdd;
						}
						if ( lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l )
						{
							m_lPlayerBankerMAX += m_lBankerScoreAdd;
						}
						return true;
					}
				}

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
				TakeTurns();

				bChangeBanker=true;
				m_bExchangeBanker = true;

				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),m_lPlayerBankerMAX);

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	
			}
		}
		else
		{
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
		}

	}
	//ϵͳ��ׯ
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//�ֻ��ж�
		TakeTurns();

		bChangeBanker=true;
		m_bExchangeBanker = true;
	}

	//�л��ж�
	if (bChangeBanker)
	{
		//�����ׯ��
		m_lPlayerBankerMAX = m_lBankerMAX;

		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker sChangeBanker;
		ZeroMemory(&sChangeBanker,sizeof(sChangeBanker));
		memset(sChangeBanker.szBankerNickName, 0, sizeof(sChangeBanker.szBankerNickName));
		sChangeBanker.wBankerUser=m_wCurrentBanker;
		sChangeBanker.lBankerScore = 0x7fffffffffffffff;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			wcscpy(sChangeBanker.szBankerNickName,pIServerUserItem->GetNickName());
			sChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//��ȡ��Ϣ
			LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//��ȡ����
				LONGLONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
				GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//��ȡ���
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

				//������Ϣ
				CString strNick;
				strNick = pIServerUserItem->GetNickName();
				strNick = strNick.Left(4);
				strNick+=_T("****");
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,strNick);
				SendGameMessage(INVALID_CHAIR,szMessage2);
			}
		}
	}

	return bChangeBanker;
}

//�ֻ��ж�
void CTableFrameSink::TakeTurns()
{
	//��������
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GAME_STATUS_FREE)
		{
			//��ȡ����
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker=m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//������Ϣ
					CMD_S_CancelBanker CancelBanker = {};

					CancelBanker.wChairID=m_ApplyUserArray[i];

					//���ñ���
					lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

					//������Ϣ
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

					//��ʾ��Ϣ
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ������%lf��������ׯ����������%I64d��,���޷���ׯ��"),
						pIServerUserItem->GetUserScore(), m_lApplyBankerCondition);
					SendGameMessage(m_ApplyUserArray[i],szTipMsg);
				}
			}
		}
	}

	//ɾ�����
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
}

//����ׯ��
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (!pServerUserItem) continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//�û�����
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		CancelBanker.wChairID=i;

		//���ñ���
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}
void   CTableFrameSink::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
	if (InArea==0xFF)
	{
		return ;
	}
	ZeroMemory(bcWinArea,bcAreaCount);


	SCORE lMaxSocre = 0;

	for (int i = 0;i<32;i++)
	{
		BYTE bcOutCadDataWin[AREA_COUNT];
		BYTE bcData[1];
		bcData[0]=i+1;
		m_GameLogic.GetCardType(bcData,1,bcOutCadDataWin);
		for (int j= 0;j<AREA_COUNT;j++)
		{

			if(bcOutCadDataWin[j]>1&&j==InArea-1)
			{
				SCORE Score = 0; 
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
				{
					if(bcOutCadDataWin[nAreaIndex-1]>1)
					{
						Score += m_lAllJettonScore[nAreaIndex]*(bcOutCadDataWin[nAreaIndex-1]);
					}
				}
				if(Score>=lMaxSocre)
				{
					lMaxSocre = Score;
					CopyMemory(bcWinArea,bcOutCadDataWin,bcAreaCount);

				}
				break;
			}
		}
	}
}
//�����ע
SCORE CTableFrameSink::GetUserMaxJetton(WORD wChairID,BYTE Area)
{
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;


	//����ע��
	SCORE lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//ׯ�ҽ��
	SCORE lBankerScore=0x7fffffffffffffff;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (NULL!=pIUserItemBanker) lBankerScore=pIUserItemBanker->GetUserScore();
	}

	BYTE bcWinArea[AREA_COUNT];
	SCORE LosScore = 0;
	SCORE WinScore = 0;

	GetAllWinArea(bcWinArea,AREA_COUNT,Area);

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if(bcWinArea[nAreaIndex-1]>1)
		{
			LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
		}else
		{
			if(bcWinArea[nAreaIndex-1]==0)
			{
				WinScore+=m_lAllJettonScore[nAreaIndex];

			}
		}
	}
	lBankerScore = lBankerScore + WinScore - LosScore;

	if ( lBankerScore < 0 )
	{
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			if (NULL!=pIUserItemBanker) 
				lBankerScore=pIUserItemBanker->GetUserScore();
		}
		else
		{
			lBankerScore = 0x7fffffffffffffff;
		}
	}

	//��������
	SCORE lMeMaxScore = min((pIMeServerUserItem->GetUserScore()-lNowJetton), m_lUserLimitScore);

	//��������
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	BYTE diMultiple[AREA_COUNT];

	for (int i = 0;i<32;i++)
	{
		   BYTE bcData[1];
		   bcData[0]= i+1;
		   BYTE  bcOutCadDataWin[AREA_COUNT];
		   m_GameLogic.GetCardType(bcData,1,bcOutCadDataWin);
		   for (int j = 0;j<AREA_COUNT;j++)
		   {
			   if(bcOutCadDataWin[j]>1)
			   {
				   diMultiple[j] = bcOutCadDataWin[j];

			   }
		   }
	}
	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore/(diMultiple[Area-1]));

	//��������
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return (SCORE)(lMeMaxScore);
}
//����÷�
SCORE CTableFrameSink::CalculateScore()
{
	//��������
	SCORE static cbRevenue=m_pGameServiceOption->cbRevenueRatio;

	//�ƶ����
	bool static bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang;
	BYTE TianMultiple,diMultiple,TianXuanltiple,HuangMultiple;
	TianMultiple  = 1;
	diMultiple = 1 ;
	TianXuanltiple = 1;
	HuangMultiple = 1;

    BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);
	m_GameLogic.GetCardType(&m_cbTableCardArray[0][0],1,bcResulteOut);

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	BYTE  cbMultiple[AREA_COUNT]={1};

	for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
	{

		if(bcResulteOut[wAreaIndex-1]>0)
		{
			GameRecord.bWinMen[wAreaIndex-1] = 4;
		}
		else
		{
			GameRecord.bWinMen[wAreaIndex-1] = 0;
		}
	}

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//ׯ������
	SCORE lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	SCORE lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//�����ע
	LONGLONG *pUserScore[AREA_COUNT+1];
	pUserScore[0]=NULL;
	for (int i = 1;i<AREA_COUNT+1;i++)
	{
		pUserScore[i]=m_lUserJettonScore[i];
	}

	//�������
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{

			if (bcResulteOut[wAreaIndex-1]>0) 
			{
				m_lUserWinScore[wChairID] += ( pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1])) ;
				m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][wChairID] * (bcResulteOut[wAreaIndex-1]) ) ;
			}
			else
			{
				if (bcResulteOut[wAreaIndex-1]==0)
				{
					lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID];
					lBankerWinScore += pUserScore[wAreaIndex][wChairID];
				}
				else
				{
					//���Ϊ1���ٷ�
					m_lUserWinScore[wChairID] += 0;
					m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
				}
			}
		}

		//����˰��
		if (0 < m_lUserWinScore[wChairID])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000000);
			m_lUserRevenue[wChairID]  = SCORE(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}

		//�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000000);
			m_lUserRevenue[m_wCurrentBanker]  = SCORE(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}	
	}

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}

//��̽���ж�
bool CTableFrameSink::ProbeJudge()
{
	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);
	m_GameLogic.GetCardType(&m_cbTableCardArray[0][0],1,bcResulteOut);

	//ϵͳ��Ӯ
	SCORE lSystemScore = 0l;

	//�����ע
	LONGLONG *pUserScore[AREA_COUNT+1];
	pUserScore[0] = NULL;
	for (int i = 1;i<AREA_COUNT+1;i++)
	{
		pUserScore[i] = m_lUserJettonScore[i];
	}

	//ׯ���ǲ��ǻ�����
	bool bIsBankerAndroidUser = false;
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem != NULL) 
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//�������
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker == wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{

			if (bcResulteOut[wAreaIndex-1]>0) 
			{
				if ( bIsAndroidUser )
					lSystemScore += (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]));

				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]));
			}
			else
			{
				if (bcResulteOut[wAreaIndex-1]==0)
				{
					if ( bIsAndroidUser )
						lSystemScore -= pUserScore[wAreaIndex][wChairID];

					if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
						lSystemScore += pUserScore[wAreaIndex][wChairID];
				}
			}
		}
	}

	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker == wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;
		if (pIServerUserItem->IsAndroidUser()) 
			continue;
		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{
			m_lAllScore += pUserScore[wAreaIndex][wChairID];
		}
	}

	if (m_lAllScore < m_StorageCritical)
	{
		if ((lSystemScore + m_StorageStartEx) < 0l && lSystemScore<0)
		{
			return false;
		}
	}



	//ϵͳ��ֵ����
	if ((lSystemScore + m_StorageStart) < 0l && lSystemScore<0)
	{
		return false;
	}
	else
	{
		//ϵͳ��Ӯ����
		if (lSystemScore + m_StorageStart<m_StorageStart*m_nWinMinPercent/100&&lSystemScore + m_StorageStart>m_StorageStart*m_nWinMaxPercent/100)
		{
			return false;
		}

		//m_StorageStart += lSystemScore;
		//m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
		return true;
	}
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
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
	if (wBufferSize>0) m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
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
        sList.wApplyList[j].lScore = pApplyUser->GetUserScore();
        sList.wApplyList[j].cbGender = pApplyUser->GetGender();
        wcscpy(sList.wApplyList[j].szNickName,pApplyUser->GetNickName());
		j++;
	}
	sList.cbCount=j;
	m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_C_GET_PLAYLIST, &sList,sList.getLength());

	return true;
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
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT);
		}

		//�Թ����
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_CHAT);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT|SMT_EJECT);
	}
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR OutBuf[255];

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("Score"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_lApplyBankerCondition);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("AreaLimitScore"),TEXT("10000"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_lAreaLimitScore);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("UserLimitScore"),TEXT("100000000"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_lUserLimitScore);

	CString strLog;
	strLog.Format(L"m_lUserLimitScore: %I64d,m_lAreaLimitScore:%I64d,m_szGameRoomName:%s,OutBuf:%s",m_lUserLimitScore,m_lAreaLimitScore,m_szGameRoomName,OutBuf);
	CTraceService::TraceString(strLog,TraceLevel_Warning);

	BYTE cbEnableSysBanker=GetPrivateProfileInt(m_szGameRoomName, TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker = cbEnableSysBanker != 0 ? true : false;

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageStart"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_StorageStart);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_StorageDeduct);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("Time"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_lBankerMAX);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("TimeAdd"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_lBankerAdd);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("BankerScore"),TEXT("100000000000"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_lBankerScoreMAX);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("BankerAdd"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_lBankerScoreAdd);

	//��ע��������Ŀ
	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;

	//��������ע����
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szGameRoomName, TEXT("RobotAreaLimit"), _T("50000000"), OutBuf, 255, m_szConfigFileName);
	_sntscanf(OutBuf, _tcslen(OutBuf), _T("%I64d"), &m_lRobotAreaLimit);


	//ʱ�����
	m_cbFreeTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("FreeTime"), TIME_FREE, m_szConfigFileName);

	m_cbBetTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetTime"), TIME_PLACE_JETTON, m_szConfigFileName);

	m_cbEndTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("EndTime"), TIME_GAME_END, m_szConfigFileName);

	m_nWinMaxPercent = GetPrivateProfileInt(m_szGameRoomName, TEXT("WinMaxPercent"), 80, m_szConfigFileName);
	m_nWinMinPercent = GetPrivateProfileInt(m_szGameRoomName, TEXT("WinMinPercent"), 20, m_szConfigFileName);

	if (m_cbFreeTime <= 0 || m_cbFreeTime > 99)			
		m_cbFreeTime = TIME_FREE;
	if (m_cbBetTime <= 0 || m_cbBetTime > 99)		
		m_cbBetTime = TIME_PLACE_JETTON;
	if (m_cbEndTime <= 0 || m_cbEndTime > 99)			
		m_cbEndTime = TIME_GAME_END;

	if ( m_lBankerScoreMAX <= m_lApplyBankerCondition)
		m_lBankerScoreMAX = 0l;

	m_lPlayerBankerMAX = m_lBankerMAX;

	//������
	m_nRobotApplyBanker = GetPrivateProfileInt(m_szGameRoomName, _T("RobotApplyBanker"), 3, m_szConfigFileName);
	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageOff"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_StorageOff);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageCritical"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_StorageCritical);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageStartEx"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_StorageStartEx);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageOffEx"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_StorageOffEx);

	m_bControl = false;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//���в���
#ifdef __SPECIAL___
bool  CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}
bool  CTableFrameSink::IsAllowBankAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if(wChairID == m_wCurrentBanker)
		return false;
	else 
	return true;

}
BOOL CTableFrameSink::IsAdmin( IServerUserItem * pIServerUserItem )
{
	CTraceService::TraceString(CString("���������")+pIServerUserItem->GetMachineID(),TraceLevel_Info);
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	TCHAR adminFile[MAX_PATH];			//�����ļ�
	_stprintf(adminFile,/*sizeof(adminFile),*/TEXT("%s\\%s"),szPath,TEXT("admin.txt"));
	FILE *fp = _tfopen(adminFile,_T("r"));
	if (fp)
	{
		while(!feof(fp))
		{
			TCHAR buf[256];
			memset(buf,0,sizeof(buf));
			_fgetts(buf,256,fp);
			CString ss = buf;
			if (ss.Find(pIServerUserItem->GetMachineID()) == 0)
			{
				CTraceService::TraceString(CString("ͨ��������+") + ss,TraceLevel_Info);
				fclose(fp);
				return TRUE;
			}
			else
			{
				CTraceService::TraceString(CString("���û�����") + ss,TraceLevel_Info);
			}
		}
		fclose(fp);
	}
	CTraceService::TraceString(CString("ʧ�ܻ�����")+pIServerUserItem->GetMachineID(),TraceLevel_Info);
	return FALSE;
}
#endif
//////////////////////////////////////////////////////////////////////////
