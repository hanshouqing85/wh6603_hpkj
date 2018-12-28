#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//����ʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					10									//����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��
#define TIME_PLACE_JETTON			20									//��עʱ��

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��
#define TIME_GAME_END				20									//����ʱ��


//��Ϣ��ѯ
#define KEY_STOCK					0
#define KEY_IMMORTAL_COUNT			1
#define KEY_ROBOT_COUNT				2
#define KEY_IMMORTAL_BET			3	
#define KEY_ROBOT_BET				4
#define KEY_MAX						5

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ��¼
	m_GameRecord.Empty();

	//��ע��
	ZeroMemory(m_lAllBet,sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet,sizeof(m_lPlayBet));

	//����
	m_lBankerScore = 0l;
	ZeroMemory(m_lPlayScore,sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//״̬����
	m_dwBetTime=0L;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_wFaceID = rand()%2;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

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


	//�������
	m_hControlInst = NULL;
	m_pServerContro = NULL;
	m_hControlInst = LoadLibrary(TEXT("BaccaratNewServerControl.dll"));
	if ( m_hControlInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hControlInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}

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

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}
}

//�ͷŶ���
VOID CTableFrameSink::Release()
{
	if( m_pServerContro )
	{
		delete m_pServerContro;
		m_pServerContro = NULL;
	}

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}
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
	if (m_pITableFrame == NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//��ȡ����
	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,CountArray(m_szConfigFileName),TEXT("%s\\BaccaratNewConfig.ini"),szPath);

	ReadConfigInformation(true);
	return true;
}

//��λ����
void CTableFrameSink::RepositionSink()
{
	//��Ϸ��¼
	m_GameRecord.Empty();

	//��ע��
	ZeroMemory(m_lAllBet,sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet,sizeof(m_lPlayBet));

	//����
	m_lBankerScore = 0l;
	ZeroMemory(m_lPlayScore,sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�����˿���
	m_nChipRobotCount = 0;

	return;
}


//��Ϸ״̬
bool CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//��ȡׯ��
	IServerUserItem* pIBankerServerUserItem = NULL;
	if ( m_wCurrentBanker == INVALID_CHAIR )
	{
		m_lBankerScore = 10000;
		m_GameRecord.AppendFormat(TEXT("ϵͳׯ,%I64d#"),m_lBankerScore);
	}
	else
	{
		IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if ( pIBankerServerUserItem != NULL )
		{
			m_lBankerScore = (LONGLONG)pIBankerServerUserItem->GetUserScore();
		}
		m_GameRecord.AppendFormat(TEXT("ׯ:%s,%I64d#"),pIBankerServerUserItem->GetNickName(),m_lBankerScore);
	}

	//���ñ���
	GameStart.cbTimeLeave = m_cbBetTime;
	GameStart.wBankerUser = m_wCurrentBanker;
	GameStart.lBankerScore = m_lBankerScore;

	//��ע����������
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}
	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);
	nChipRobotCount = 0;
	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++) 
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray.GetAt(i));
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

//	if(nChipRobotCount > 0)
//		GameStart.nAndriodCount=nChipRobotCount-1;


	//�����˿���
	m_nChipRobotCount = 0;

    //�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		//���û���
		TRACE(TEXT("UserFreeScore :: %I64d"), pIServerUserItem->GetUserScore());
		GameStart.lPlayBetScore=min((LONGLONG)pIServerUserItem->GetUserScore() ,m_lUserLimitScore);
		GameStart.lPlayFreeSocre = (LONGLONG)pIServerUserItem->GetUserScore();

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
		{
			//�������
			LONGLONG lBankerWinScore = GameOver();

			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ׯ����Ϣ
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;

			//�˿���Ϣ
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			CopyMemory(GameEnd.cbCardCount,m_cbCardCount,sizeof(m_cbCardCount));

			//���ͻ���
			GameEnd.cbTimeLeave=m_cbEndTime;	
			for ( WORD i = 0; i < GAME_PLAYER; ++i )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if ( pIServerUserItem == NULL ) continue;

				//���óɼ�
				GameEnd.lPlayAllScore = m_lUserWinScore[i];
				memcpy( GameEnd.lPlayScore, m_lPlayScore[i], sizeof(GameEnd.lPlayScore));
				
				//����˰��
				GameEnd.lRevenue = (LONGLONG)m_lUserRevenue[i];

				//������Ϣ					
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			TCHAR szTemp[255]=TEXT("");
			swprintf(szTemp, sizeof(szTemp),TEXT("��Ϸ����,��ǰ��棺%I64d"),m_StorageStart);
			//m_DebugWindow.WriteString(szTemp);

			////////////////////////////////////////////////////////////

			//�����ļ���
			//��ȡĿ¼
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetCurrentDirectory(CountArray(szPath),szPath);
			//��ȡ����
			_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\BaccaratNewConfig.ini"),szPath);

			//TCHAR szPath[MAX_PATH] = TEXT("");
			//TCHAR szConfigFileName[MAX_PATH] = TEXT("");
			//TCHAR OutBuf[255] = TEXT("");
			//GetCurrentDirectory(sizeof(szPath), szPath);
			//_sntprintf(szConfigFileName, CountArray(szConfigFileName), TEXT("%s\\%s"), szPath, szFileName);

	

			//�ж��Ƿ������ʵ���
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//������ʵ�û�
					bUser = true;
					//����FOR
					break;

				}
			}
			if (bUser)
			{

				//CString strLog;
				//strLog.Format(TEXT("���ţ�%d,��ǰ��棺%0.2f,ϵͳ�÷֣�%0.2f,˰�գ�%0.2f,ʣ���棺%0.2f,���˥����%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				//CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ


				
				//m_lStockLimit+=lSystemScore;m_StorageStart

				CString szlStorageStart;
				szlStorageStart.Format(TEXT("%d"),m_StorageStart);
				//��¼����Ӯ��¼
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageStart"),szlStorageStart,m_szFileName);


				///////////////////////////////////


				////������ڿ����û��Ļ�//������Ϸ��Ϣ�����ƺ�
				//for(int i=0; i<GAME_PLAYER; i++)
				//{
				//	IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //��ȡ�û��ʺ�
				//	if(pUserItem==NULL) continue;	//����û�������
				//	if(pUserItem->IsAndroidUser()) continue;  //����ǻ�����
				//	if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //�ж��û�����Ȩ��
				//	m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //������Ϣ����
				//}
			}

			CTraceService::TraceString(szTemp,TraceLevel_Info);

			return true;
		}
	case GER_NETWORK_ERROR:		//�û�����
	case GER_USER_LEAVE:		//�û��뿪
		{
			if(cbReason ==GER_NETWORK_ERROR )
			{
				m_GameRecord.AppendFormat(TEXT("%s,����#"),pIServerUserItem->GetNickName());
			}
			else
			{
				m_GameRecord.AppendFormat(TEXT("%s,�뿪#"),pIServerUserItem->GetNickName());
			}
			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				//��������
				LONGLONG lRevenue=0;

				//д�����
				if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
				{
					for ( WORD wAreaIndex = AREA_XIAN; wAreaIndex <= AREA_ZHUANG_DUI; ++wAreaIndex )
					{
						if (m_lPlayBet[wChairID][wAreaIndex] != 0)
						{
							CMD_S_PlaceBetFail PlaceBetFail;
							ZeroMemory(&PlaceBetFail,sizeof(PlaceBetFail));
							PlaceBetFail.lBetArea = (BYTE)wAreaIndex;
							PlaceBetFail.lPlaceScore = m_lPlayBet[wChairID][wAreaIndex];
							PlaceBetFail.wPlaceUser = wChairID;

							//��Ϸ���
							for (WORD i=0; i<GAME_PLAYER; ++i)
							{
								IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
								if (pIServerUserItem == NULL) 
									continue;

								m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceBetFail,sizeof(PlaceBetFail));
							}

							m_lAllBet[wAreaIndex] -= m_lPlayBet[wChairID][wAreaIndex];
							m_lPlayBet[wChairID][wAreaIndex] = 0;
						}
					}
				}
				else
				{
					//д�����
					if (m_lUserWinScore[wChairID] >0.001||m_lUserWinScore[wChairID] <-0.001) 
					{
						tagScoreInfo ScoreInfoArray[GAME_PLAYER];
						ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

						if ( m_lUserWinScore[wChairID] > 0.0 )
							ScoreInfoArray[wChairID].cbType=SCORE_TYPE_WIN;
						else if ( m_lUserWinScore[wChairID] < 0.0 )
							ScoreInfoArray[wChairID].cbType=SCORE_TYPE_LOSE;
						else
							ScoreInfoArray[wChairID].cbType=SCORE_TYPE_DRAW;

						ScoreInfoArray[wChairID].lScore=m_lUserWinScore[wChairID];
						ScoreInfoArray[wChairID].lRevenue=m_lUserRevenue[wChairID];


						m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
						m_lUserWinScore[wChairID] = 0.00;

						m_GameRecord.AppendFormat(TEXT("%s�÷�:%0.2f,%0.2f#"),m_pITableFrame->GetTableUserItem(wChairID)->GetNickName(),ScoreInfoArray[wChairID].lScore, ScoreInfoArray[wChairID].lRevenue);
					}

					//�����ע
					for (WORD wAreaIndex = AREA_XIAN; wAreaIndex <= AREA_ZHUANG_DUI; ++wAreaIndex )
					{
						m_lPlayBet[wChairID][wAreaIndex] = 0;
					}
				}

				return true;
			}

			//״̬�ж�
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_END)
			{
				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��ǿ�ˣ���Ϸ��ǰ������"));


				for ( WORD i = 0; i < GAME_PLAYER; ++i )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
					if ( pIServerUserItem == NULL ) continue;

					//������Ϣ
					CMD_S_TipInfo TipInfo;
					TipInfo.cbTimeLeave = 10;
					_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("����ׯ��ǿ�ˣ���Ϸ��ǰ������"));
					m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
				}

				//����״̬
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				//����ʱ��
				m_dwBetTime=(DWORD)time(NULL);
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//�������
				GameOver();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (m_lBankerWinScore>0L) GameEnd.lBankerScore=0;

				//�˿���Ϣ
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				CopyMemory(GameEnd.cbCardCount,m_cbCardCount,sizeof(m_cbCardCount));

				//���ͻ���
				GameEnd.cbTimeLeave=m_cbEndTime;	
				for ( WORD i = 0; i < GAME_PLAYER; ++i )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
					if ( pIServerUserItem == NULL ) continue;

					//���óɼ�
					GameEnd.lPlayAllScore = m_lUserWinScore[i];
					memcpy( GameEnd.lPlayScore, m_lPlayScore[i], sizeof(GameEnd.lPlayScore));

					//����˰��
					GameEnd.lRevenue = (LONGLONG)m_lUserRevenue[i];

					//������Ϣ					
					m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//�۳�����
			if (m_lUserWinScore[m_wCurrentBanker] != 0.0)
			{

				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
				ScoreInfoArray[m_wCurrentBanker].cbType=SCORE_TYPE_DRAW;

				if ( m_lUserWinScore[m_wCurrentBanker] > 0.0 )
					ScoreInfoArray[m_wCurrentBanker].cbType=SCORE_TYPE_WIN;
				else if ( m_lUserWinScore[m_wCurrentBanker] < 0.0 )
					ScoreInfoArray[m_wCurrentBanker].cbType=SCORE_TYPE_LOSE;
				else
					ScoreInfoArray[m_wCurrentBanker].cbType=SCORE_TYPE_DRAW;

				ScoreInfoArray[m_wCurrentBanker].lScore=m_lUserWinScore[m_wCurrentBanker];
				ScoreInfoArray[m_wCurrentBanker].lRevenue=m_lUserRevenue[m_wCurrentBanker];
				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
				m_lUserWinScore[m_wCurrentBanker] = 0.0;
			}

			//�л�ׯ��
			ChangeBanker(true);

			TCHAR szTemp[255]=TEXT("");
			swprintf(szTemp, sizeof(szTemp),TEXT("��Ϸ����,��ǰ��棺%I64d"),m_StorageStart);
			//m_DebugWindow.WriteString(szTemp);

			////////////////////////////////////////////////////////////

			//�����ļ���
			//��ȡĿ¼
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetCurrentDirectory(CountArray(szPath),szPath);
			//��ȡ����
			_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\BaccaratNewConfig.ini"),szPath);

			//TCHAR szPath[MAX_PATH] = TEXT("");
			//TCHAR szConfigFileName[MAX_PATH] = TEXT("");
			//TCHAR OutBuf[255] = TEXT("");
			//GetCurrentDirectory(sizeof(szPath), szPath);
			//_sntprintf(szConfigFileName, CountArray(szConfigFileName), TEXT("%s\\%s"), szPath, szFileName);



			//�ж��Ƿ������ʵ���
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//������ʵ�û�
					bUser = true;
					//����FOR
					break;

				}
			}
			if (bUser)
			{

				//CString strLog;
				//strLog.Format(TEXT("���ţ�%d,��ǰ��棺%0.2f,ϵͳ�÷֣�%0.2f,˰�գ�%0.2f,ʣ���棺%0.2f,���˥����%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				//CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ



				//m_lStockLimit+=lSystemScore;m_StorageStart

				CString szlStorageStart;
				szlStorageStart.Format(TEXT("%d"),m_StorageStart);
				//��¼����Ӯ��¼
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageStart"),szlStorageStart,m_szFileName);


				///////////////////////////////////


				////������ڿ����û��Ļ�//������Ϸ��Ϣ�����ƺ�
				//for(int i=0; i<GAME_PLAYER; i++)
				//{
				//	IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //��ȡ�û��ʺ�
				//	if(pUserItem==NULL) continue;	//����û�������
				//	if(pUserItem->IsAndroidUser()) continue;  //����ǻ�����
				//	if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //�ж��û�����Ȩ��
				//	m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //������Ϣ����
				//}
			}


			return true;
		}
	}

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//ȫ����Ϣ
			DWORD dwPassTime = (DWORD)time(NULL)-m_dwBetTime;
			StatusFree.cbTimeLeave = (BYTE)(m_cbFreeTime - __min(dwPassTime, (DWORD)m_cbFreeTime));

			//�����Ϣ
			StatusFree.lPlayFreeSocre = (LONGLONG)pIServerUserItem->GetUserScore();
			//ׯ����Ϣ
			StatusFree.wBankerUser = m_wCurrentBanker;	
			StatusFree.wBankerTime = m_wBankerTime;
			StatusFree.lBankerWinScore = m_lBankerWinScore;
			StatusFree.lBankerScore = m_lBankerScore;
			StatusFree.wFaceID = m_wFaceID;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				wcscpy(StatusFree.szBankerNickName,pIServerUserItem->GetNickName());
			}


			//�Ƿ�ϵͳ��ׯ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_szGameRoomName, sizeof(StatusFree.szGameRoomName));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n��������ׯ����Ϊ��%s\n��������Ϊ��%s\n�������Ϊ��%s"), AddComma(m_lApplyBankerCondition), AddComma(m_lAreaLimitScore), AddComma(m_lUserLimitScore));
			
			//������Ϣ
			CMD_S_TipInfo TipInfo;
			TipInfo.cbTimeLeave = 10;
			_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("%s"), szTipMsg);
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
						
			//����������
			SendApplyUser(pIServerUserItem);
 
			return bSuccess;
		}
	case GAME_SCENE_BET:		//��Ϸ״̬
	case GAME_SCENE_END:		//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL) - m_dwBetTime;
			int	nTotalTime = ( (cbGameStatus == GAME_SCENE_BET) ? m_cbBetTime : m_cbEndTime);
			StatusPlay.cbTimeLeave = (BYTE)(nTotalTime - __min(dwPassTime, (DWORD)nTotalTime));
			StatusPlay.cbGameStatus = m_pITableFrame->GetGameStatus();		

			//ȫ����ע
			memcpy(StatusPlay.lAllBet, m_lAllBet, sizeof(StatusPlay.lAllBet));
			StatusPlay.lPlayFreeSocre = (LONGLONG)pIServerUserItem->GetUserScore();

			//�����ע
			if (pIServerUserItem->GetUserStatus() != US_LOOKON && bSendSecret)
			{
				memcpy(StatusPlay.lPlayBet, m_lPlayBet[wChiarID], sizeof(StatusPlay.lPlayBet));
				memcpy(StatusPlay.lPlayScore, m_lPlayScore[wChiarID], sizeof(StatusPlay.lPlayScore));

				//�����ע
				StatusPlay.lPlayBetScore = min((LONGLONG)pIServerUserItem->GetUserScore() , m_lUserLimitScore);
			}

			//ׯ����Ϣ
			StatusPlay.wBankerUser = m_wCurrentBanker;			
			StatusPlay.wBankerTime = m_wBankerTime;
			StatusPlay.lBankerWinScore = m_lBankerWinScore;	
			StatusPlay.lBankerScore = m_lBankerScore;
			StatusPlay.wFaceID = m_wFaceID;

			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				wcscpy(StatusPlay.szBankerNickName,pIServerUserItem->GetNickName());
			}

			//�Ƿ�ϵͳ��ׯ
			StatusPlay.bEnableSysBanker = m_bEnableSysBanker;

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//�����ж�
			if (cbGameStatus == GAME_SCENE_END)
			{
				//�˿���Ϣ
				CopyMemory(StatusPlay.cbCardCount,m_cbCardCount,sizeof(m_cbCardCount));
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

				//��������
				StatusPlay.lPlayAllScore = m_lUserWinScore[wChiarID];
			}

			//��������
			CopyMemory(StatusPlay.szGameRoomName, m_szGameRoomName, sizeof(StatusPlay.szGameRoomName));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n��������ׯ����Ϊ��%s\n��������Ϊ��%s\n�������Ϊ��%s"), AddComma(m_lApplyBankerCondition), AddComma(m_lAreaLimitScore), AddComma(m_lUserLimitScore));

			//������Ϣ
			CMD_S_TipInfo TipInfo;
			TipInfo.cbTimeLeave = 10;
			_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("%s"), szTipMsg);
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));

			
			//����������
			SendApplyUser( pIServerUserItem );

			return bSuccess;
		}
	}

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{
			//��ʼ��Ϸ
			m_pITableFrame->StartGame();

			//����ʱ��
			m_dwBetTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GAME_SCENE_BET);

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_END)
			{

				//����״̬
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);			

				//������Ϸ
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_dwBetTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			m_GameRecord+=TEXT("END:#");
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			for(WORD i = 0;i<2;i++)
			{
				if(i == INDEX_PLAYER)
					m_GameRecord+=TEXT("��,Card:");
				else
					m_GameRecord+=TEXT("ׯ,Card:");
				
				for(WORD j = 0;j<m_cbCardCount[i];j++)
					m_GameRecord.AppendFormat(TEXT("%s,"),TransformCardInfo(m_cbTableCardArray[i][j]));

				m_GameRecord+=TEXT("#");
			}
			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;


				//д�����
				if (m_lUserWinScore[wUserChairID] >0.001||m_lUserWinScore[wUserChairID]<-0.001) 
				{
					ScoreInfoArray[wUserChairID].lRevenue=m_lUserRevenue[wUserChairID];
					ScoreInfoArray[wUserChairID].lScore=m_lUserWinScore[wUserChairID];

					if ( m_lUserWinScore[wUserChairID] > 0.0 )
						ScoreInfoArray[wUserChairID].cbType=SCORE_TYPE_WIN;
					else if ( m_lUserWinScore[wUserChairID] < 0.0 )
						ScoreInfoArray[wUserChairID].cbType=SCORE_TYPE_LOSE;
					else
						ScoreInfoArray[wUserChairID].cbType=SCORE_TYPE_DRAW;
					//��Ϸ��¼
					m_GameRecord.AppendFormat(TEXT("%s�÷�:%0.2f,%0.2f#"),m_pITableFrame->GetTableUserItem(wUserChairID)->GetNickName(),ScoreInfoArray[wUserChairID].lScore, ScoreInfoArray[wUserChairID].lRevenue);
				}
				if(!pIServerUserItem->IsAndroidUser())
				{
					CString strLog;
					strLog.Format(L"OXBATTLE lScore:%.2lf,revenue:%.2lf",ScoreInfoArray[wUserChairID].lScore,ScoreInfoArray[wUserChairID].lRevenue);
					CTraceService::TraceString(strLog,TraceLevel_Warning);

				}
			}


			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//������Ϸ
			//m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,m_GameRecord,608);
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);

			//��ȡ����
			if (m_bRefreshCfg)
				ReadConfigInformation(false);

			//�л�ׯ��
			ChangeBanker(false);

			//����ʱ��
			m_dwBetTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_cbFreeTime;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize == sizeof(CMD_C_PlaceBet));
			if (wDataSize!=sizeof(CMD_C_PlaceBet)) return false;

			//�û�Ч��
			if ( pIServerUserItem->GetUserStatus() != US_PLAYING ) return true;

			//��Ϣ����
			CMD_C_PlaceBet * pPlaceBet = (CMD_C_PlaceBet *)pData;
			return OnUserPlayBet(pIServerUserItem->GetChairID(), pPlaceBet->cbBetArea, pPlaceBet->lBetScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//�û�Ч��
			if ( pIServerUserItem->GetUserStatus() == US_LOOKON ) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			if ( pIServerUserItem->GetUserStatus() == US_LOOKON ) return true;
			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize == sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;

			if ( m_pServerContro == NULL)
				return false;

			return m_pServerContro->ServerControl(wSubCmdID, pData, wDataSize, pIServerUserItem, m_pITableFrame);
		}
	case SUB_C_CANCEL_JETTON:
		{
			//�û�Ч��
			if ( pIServerUserItem->GetUserStatus() != US_PLAYING ) return true;

			return OnUserCancelBet(pIServerUserItem->GetChairID());
		}
	}

	return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	// ������Ϣ
	//if (wSubCmdID == SUB_GF_USER_CHAT && CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
	//{
	//	//��������
	//	CMD_GF_C_UserChat * pUserChat=(CMD_GF_C_UserChat *)pData;

	//	//Ч�����
	//	ASSERT(wDataSize<=sizeof(CMD_GF_C_UserChat));
	//	ASSERT(wDataSize>=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)));
	//	ASSERT(wDataSize==(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//	//Ч�����
	//	if (wDataSize>sizeof(CMD_GF_C_UserChat)) return false;
	//	if (wDataSize<(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString))) return false;
	//	if (wDataSize!=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//	bool bKeyProcess = false;
	//	CString strChatString(pUserChat->szChatString);
	//	CString strKey[KEY_MAX] = { TEXT("/stock"), TEXT("/immortal count"), TEXT("/robot count"), TEXT("/immortal bet"), TEXT("/robot bet") };
	//	CString strName[KEY_MAX] = { TEXT("���"), TEXT("��������"), TEXT("����������"), TEXT("������ע"), TEXT("��������ע") };
	//	if ( strChatString == TEXT("/help") )
	//	{
	//		bKeyProcess = true;
	//		CString strMsg;
	//		for ( int i = 0 ; i < KEY_MAX; ++i)
	//		{
	//			strMsg += TEXT("\n");
	//			strMsg += strKey[i];
	//			strMsg += TEXT(" �鿴");
	//			strMsg += strName[i];
	//		}
	//		m_pITableFrame->SendGameMessage(pIServerUserItem, strMsg, SMT_EJECT);	
	//	}
	//	else 
	//	{
	//		CString strMsg;
	//		for ( int i = 0 ; i < KEY_MAX; ++i)
	//		{
	//			if ( strChatString == strKey[i] )
	//			{
	//				bKeyProcess = true;
	//				switch(i)
	//				{
	//				case KEY_STOCK:
	//					{
	//						strMsg.Format(TEXT("���ʣ������%I64d"), m_StorageStart);
	//					}
	//					break;
	//				case KEY_IMMORTAL_COUNT:
	//					{
	//						int nImmortal = 0;
	//						for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	//						{
	//							//��ȡ�û�
	//							IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	//							if (pIServerUserItem == NULL) continue;

	//							if ( !pIServerUserItem->IsAndroidUser() )
	//								nImmortal += 1;
	//						}
	//						strMsg.Format(TEXT("����������%d"), nImmortal);
	//					}
	//					break;
	//				case KEY_ROBOT_COUNT:
	//					{
	//						int nRobot = 0;
	//						for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	//						{
	//							//��ȡ�û�
	//							IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	//							if (pIServerUserItem == NULL) continue;

	//							if ( pIServerUserItem->IsAndroidUser() )
	//								nRobot += 1;
	//						}
	//						strMsg.Format(TEXT("������������%d"), nRobot);
	//					}
	//					break;
	//				case KEY_IMMORTAL_BET:
	//					{
	//						LONGLONG lBet[AREA_MAX] = {0};
	//						for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	//						{
	//							//��ȡ�û�
	//							IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	//							if (pIServerUserItem == NULL) continue;

	//							if ( !pIServerUserItem->IsAndroidUser() )
	//							{	
	//								for ( int nArea = 0; nArea < AREA_MAX; ++nArea )
	//								{
	//									lBet[nArea] += m_lPlayBet[wChairID][nArea];
	//								}
	//							}
	//						}

	//						strMsg.Format(TEXT("�����ע��\n �У�%I64d \n ƽ��%I64d \n ׯ��%I64d \n ��������%I64d \n ׯ������%I64d \n ͬ��ƽ��%I64d \n �ж��ӣ�%I64d \n ׯ���ӣ�%I64d"), 
	//							lBet[AREA_XIAN], lBet[AREA_PING], lBet[AREA_ZHUANG], lBet[AREA_XIAN_TIAN], lBet[AREA_ZHUANG_TIAN], lBet[AREA_TONG_DUI], lBet[AREA_XIAN_DUI], lBet[AREA_ZHUANG_DUI] );
	//					}
	//					break;
	//				case KEY_ROBOT_BET:
	//					{
	//						LONGLONG lBet[AREA_MAX] = {0};
	//						for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	//						{
	//							//��ȡ�û�
	//							IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	//							if (pIServerUserItem == NULL) continue;

	//							if ( pIServerUserItem->IsAndroidUser() )
	//							{	
	//								for ( int nArea = 0; nArea < AREA_MAX; ++nArea )
	//								{
	//									lBet[nArea] += m_lPlayBet[wChairID][nArea];
	//								}
	//							}
	//						}

	//						strMsg.Format(TEXT("��������ע��\n �У�%I64d \n ƽ��%I64d \n ׯ��%I64d \n ��������%I64d \n ׯ������%I64d \n ͬ��ƽ��%I64d \n �ж��ӣ�%I64d \n ׯ���ӣ�%I64d"), 
	//							lBet[AREA_XIAN], lBet[AREA_PING], lBet[AREA_ZHUANG], lBet[AREA_XIAN_TIAN], lBet[AREA_ZHUANG_TIAN], lBet[AREA_TONG_DUI], lBet[AREA_XIAN_DUI], lBet[AREA_ZHUANG_DUI] );
	//					}
	//					break;
	//				}
	//				m_pITableFrame->SendGameMessage(pIServerUserItem, strMsg, SMT_EJECT);	
	//				break;
	//			}
	//		}
	//	}
	//	return bKeyProcess;
	//}
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//����ʱ��
	if ((bLookonUser == false)&&(m_dwBetTime == 0L))
	{
		m_dwBetTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser == false)
	{
		//�л�ׯ��
		if (wChairID == m_wCurrentBanker) ChangeBanker(true);

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//ȡ����ע
bool CTableFrameSink::OnUserCancelBet(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_BET)
	{
		return true;
	}
	
	LONGLONG lBetCount = 0;
	for(WORD i = 0; i < AREA_MAX; ++i )
	{
		lBetCount += m_lPlayBet[wChairID][i];
	}

	if(lBetCount == 0) return true;

	CMD_S_CancelBet CancelBet;
	ZeroMemory(&CancelBet,sizeof(CancelBet));
	
	CancelBet.wChairID = wChairID;
	CopyMemory(CancelBet.lPlayBet,m_lPlayBet[wChairID],sizeof(m_lPlayBet[wChairID]));
	ZeroMemory(m_lPlayBet[wChairID],sizeof(m_lPlayBet[wChairID]));

		//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CANCEL_JETTON,&CancelBet,sizeof(CancelBet));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CANCEL_JETTON,&CancelBet,sizeof(CancelBet));

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlayBet(WORD wChairID, BYTE cbBetArea, LONGLONG lBetScore)
{
	//Ч�����
	ASSERT((cbBetArea<=AREA_ZHUANG_DUI)&&(lBetScore>0L));
	if ((cbBetArea>AREA_ZHUANG_DUI)||(lBetScore<=0L)) return false;

	//Ч��״̬
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_BET)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}

	//ׯ���ж�
	if (m_wCurrentBanker == wChairID)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}
	if (m_bEnableSysBanker == false && m_wCurrentBanker == INVALID_CHAIR)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	LONGLONG lUserScore = (LONGLONG)pIServerUserItem->GetUserScore();
	LONGLONG lBetCount = 0;
	for ( int i = 0; i < AREA_MAX; ++i )
	{
		lBetCount += m_lPlayBet[wChairID][i];
	}

	

	//�ɹ���ʶ
	bool bPlaceBetSuccess=true;

	//�Ϸ�У��
	if (lUserScore < lBetCount + lBetScore)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}
	else
	{
		//��������֤
		if(pIServerUserItem->IsAndroidUser())
		{
			//��Ŀ����
			bool bHaveChip = false;
			bHaveChip = (lBetCount>0);

			if (!bHaveChip)
			{
				if (m_nChipRobotCount+1 > m_nMaxChipRobot)
				{
					SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
					return true;
				}
				else
					m_nChipRobotCount++;
			}
		}
	}

	if (m_lUserLimitScore < lBetCount + lBetScore)
	{
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
		return true;
	}


	//�Ϸ���֤
	if ( GetMaxPlayerScore(cbBetArea, wChairID) >= lBetScore )
	{
		m_lAllBet[cbBetArea] += lBetScore;
		m_lPlayBet[wChairID][cbBetArea]  += lBetScore;
	}
	else
	{
		bPlaceBetSuccess = false;
	}

	if (bPlaceBetSuccess)
	{
		//��������
		CMD_S_PlaceBet PlaceBet;
		ZeroMemory(&PlaceBet,sizeof(PlaceBet));

		//�������
		PlaceBet.wChairID=wChairID;
		PlaceBet.cbBetArea=cbBetArea;
		PlaceBet.lBetScore=lBetScore;
		PlaceBet.cbAndroidUser=pIServerUserItem->IsAndroidUser()?TRUE:FALSE;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceBet,sizeof(PlaceBet));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceBet,sizeof(PlaceBet));

		CString strArea[8]={L"��",L"ƽ",L"ׯ",L"������",L"ׯ����",L"ͬ��ƽ",L"�ж���",L"ׯ����"};
		for(int i=0; i<MAX_CHAIR; i++)
		{
			IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i);
			if(pUserItem==NULL) continue;
			if(pUserItem->IsAndroidUser()) continue;
			if(pIServerUserItem->IsAndroidUser()) continue;
			if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;
			TCHAR strMessage[256]=TEXT("");
			_sntprintf(strMessage, sizeof(strMessage), _T("%s:[%s]��ע%I64d"), pIServerUserItem->GetUserInfo()->szNickName,strArea[cbBetArea],lBetScore);
			m_pITableFrame->SendGameMessage(pUserItem,strMessage,SMT_CHAT);

		}

	}
	else
	{
		//������Ϣ
		SendPlaceBetFail(wChairID,cbBetArea,lBetScore);
	}

	return true;
}

//������Ϣ
void CTableFrameSink::SendPlaceBetFail(WORD wChairID, BYTE cbBetArea, LONGLONG lBetScore)
{
	CMD_S_PlaceBetFail PlaceBetFail;
	ZeroMemory(&PlaceBetFail,sizeof(PlaceBetFail));
	PlaceBetFail.lBetArea=cbBetArea;
	PlaceBetFail.lPlaceScore=lBetScore;
	PlaceBetFail.wPlaceUser=wChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceBetFail,sizeof(PlaceBetFail));
}

//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	//����˿�
	m_GameLogic.RandCardList(m_cbTableCardArray[0],sizeof(m_cbTableCardArray)/sizeof(m_cbTableCardArray[0][0]));

	//�״η���
	m_cbCardCount[INDEX_PLAYER] = 2;
	m_cbCardCount[INDEX_BANKER] = 2;
	
	//�������
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);
	BYTE cbPlayerTwoCardCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);

	//�мҲ���
	BYTE cbPlayerThirdCardValue = 0 ;	//�������Ƶ���
	if(cbPlayerTwoCardCount<=5 && cbBankerCount<8)
	{
		//�������
		m_cbCardCount[INDEX_PLAYER]++;
		cbPlayerThirdCardValue = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_PLAYER][2]);
	}

	//ׯ�Ҳ���
	if(cbPlayerTwoCardCount<8 && cbBankerCount<8)
	{
		switch(cbBankerCount)
		{
		case 0:
		case 1:
		case 2:
			m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 3:
			if((m_cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue!=8) || m_cbCardCount[INDEX_PLAYER] == 2) m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 4:
			if((m_cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 && cbPlayerThirdCardValue!=0) || m_cbCardCount[INDEX_PLAYER] == 2) m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 5:
			if((m_cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=2 && cbPlayerThirdCardValue!=3  && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 &&  cbPlayerThirdCardValue!=0) || m_cbCardCount[INDEX_PLAYER] == 2) m_cbCardCount[INDEX_BANKER]++ ;
			break;

		case 6:
			if(m_cbCardCount[INDEX_PLAYER] == 3 && (cbPlayerThirdCardValue == 6 || cbPlayerThirdCardValue == 7)) m_cbCardCount[INDEX_BANKER]++ ;
			break;

			//���벹��
		case 7:
		case 8:
		case 9:
			break;
		default:
			break;
		}
	}

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	LONGLONG lUserScore = (LONGLONG)pIApplyServerUserItem->GetUserScore();
	if ( lUserScore < m_lApplyBankerCondition )
	{
		CMD_S_TipInfo TipInfo;
		TipInfo.cbTimeLeave = 10;
		_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"));
		m_pITableFrame->SendUserItemData(pIApplyServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
		return true;
	}

	//�����ж�
	WORD wApplyUserChairID = pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID = m_ApplyUserArray[nUserIdx];
		if (wChairID == wApplyUserChairID)
		{
			CMD_S_TipInfo TipInfo;
			TipInfo.cbTimeLeave = 10;
			_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"));
			m_pITableFrame->SendUserItemData(pIApplyServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
			return true;
		}
	}

	//������Ϣ 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//�������
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//���ñ���
	ApplyBanker.wApplyUser = wApplyUserChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//�л��ж�
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE && m_ApplyUserArray.GetCount() == 1)
	{
		ChangeBanker(false);
	}

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
 	if (pICancelServerUserItem->GetChairID() == m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_SCENE_FREE)
	{
		//������Ϣ
		CMD_S_TipInfo TipInfo;
		TipInfo.cbTimeLeave = 10;
		_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"));
		m_pITableFrame->SendUserItemData(pICancelServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));

		return true;
	}

	//�����ж�
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//��������
		if (pIServerUserItem == NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker == wChairID)
		{
			//�л�ׯ�� 
			//m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return true;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	bool bSystemBanker = (m_wCurrentBanker == INVALID_CHAIR);
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
		if(pIServerUserItem)
		{
			LONGLONG lBankerScore=(LONGLONG)pIServerUserItem->GetUserScore();

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
						LONGLONG lScore = (LONGLONG)pIServerUserItem->GetUserScore();

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
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����������(%I64d)��ǿ�л�ׯ!"),m_lApplyBankerCondition);
				else
            		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ�����ﵽ(%d)��ǿ�л�ׯ!"), m_lPlayerBankerMAX);
				
				//������Ϣ
				CMD_S_TipInfo TipInfo;
				TipInfo.cbTimeLeave = 10;
				_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("%s"), szTipMsg);
				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));

			}
		}
	}
	//ϵͳ��ׯ
	else if (m_wCurrentBanker == INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
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
		if(bSystemBanker == false && m_wCurrentBanker == INVALID_CHAIR)
		{
			m_wFaceID = (m_wFaceID+1)%2;
		}
		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker stChangeBanker;
		ZeroMemory(&stChangeBanker,sizeof(stChangeBanker));
		stChangeBanker.wBankerUser = m_wCurrentBanker;
		stChangeBanker.wFaceID=m_wFaceID;
		if (m_wCurrentBanker != INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			stChangeBanker.lBankerScore = (LONGLONG)pIServerUserItem->GetUserScore();
			wcscpy(stChangeBanker.szBankerNickName,pIServerUserItem->GetNickName());
		}
		else
		{
			stChangeBanker.lBankerScore = 100000000;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&stChangeBanker,sizeof(CMD_S_ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&stChangeBanker,sizeof(CMD_S_ChangeBanker));

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

				//������Ϣ
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,strNick);
				//SendGameMessage(INVALID_CHAIR,szMessage2);
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
		if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
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

					//���ñ���
					CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

					//������Ϣ
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

					//��ʾ��Ϣ
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ������%I64d��������ׯ����������%I64d��,���޷���ׯ��"),(LONGLONG)pIServerUserItem->GetUserScore(), m_lApplyBankerCondition);

					//������Ϣ
					CMD_S_TipInfo TipInfo;
					TipInfo.cbTimeLeave = 10;
					_sntprintf(TipInfo.szTipInfo, CountArray(TipInfo.szTipInfo), TEXT("%s"), szTipMsg);
					m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_TIP_INFO,&TipInfo, sizeof(TipInfo));
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
		if (pServerUserItem->GetChairID() == m_wCurrentBanker) continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine( WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
	if (wChairID == m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxPlayerScore( BYTE cbBetArea, WORD wChairID )
{
	//��ȡ���
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	if ( NULL == pIMeServerUserItem ) 
		return 0L;

	if ( cbBetArea >= AREA_MAX )
		return 0L;

	//����ע��
	LONGLONG lNowBet = 0l;
	for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex ) 
		lNowBet += m_lPlayBet[wChairID][nAreaIndex];

	//ׯ�ҽ��
	LONGLONG lBankerScore = m_lBankerScore;

	//������
	BYTE cbMultiple[AREA_MAX] = {MULTIPLE_XIAN, MULTIPLE_PING, MULTIPLE_ZHUANG, 
									MULTIPLE_XIAN_TIAN, MULTIPLE_ZHUANG_TIAN, MULTIPLE_TONG_DIAN, 
									MULTIPLE_XIAN_PING, MULTIPLE_ZHUANG_PING};

	//������Ӯ
	BYTE cbArae[4][4] = {	{ AREA_XIAN_DUI,	255,			AREA_MAX,			AREA_MAX }, 
							{ AREA_ZHUANG_DUI,	255,			AREA_MAX,			AREA_MAX }, 
							{ AREA_XIAN,		AREA_PING,		AREA_ZHUANG,		AREA_MAX },  
							{ AREA_XIAN_TIAN,	AREA_TONG_DUI,	AREA_ZHUANG_TIAN,	255 }};

	//�����趨
	for ( int nTopL = 0; nTopL < 4; ++nTopL )
	{
		if( cbArae[0][nTopL] == AREA_MAX )
			continue;

		for ( int nTopR = 0; nTopR < 4; ++nTopR )
		{
			if( cbArae[1][nTopR] == AREA_MAX )
				continue;

			for ( int nCentral = 0; nCentral < 4; ++nCentral )
			{
				if( cbArae[2][nCentral] == AREA_MAX )
					continue;

				for ( int nBottom = 0; nBottom < 4; ++nBottom )
				{
					if( cbArae[3][nBottom] == AREA_MAX )
						continue;

					BYTE cbWinArea[AREA_MAX] = {FALSE};

					//ָ����ʤ����
					if ( cbArae[0][nTopL] != 255 )
						cbWinArea[cbArae[0][nTopL]] = TRUE;

					if ( cbArae[1][nTopR] != 255 )
						cbWinArea[cbArae[1][nTopR]] = TRUE;

					if ( cbArae[2][nCentral] != 255 )
						cbWinArea[cbArae[2][nCentral]] = TRUE;

					if ( cbArae[3][nBottom] != 255 )
						cbWinArea[cbArae[3][nBottom]] = TRUE;

					//ѡ������Ϊ���ʤ����ͬ�ȼ�������������Ϊ����䡣�Եó������עֵ
					for ( int i = 0; i < 4; i++ )
					{
						for ( int j = 0; j < 4; j++ )
						{
							if ( cbArae[i][j] == cbBetArea )
							{
								for ( int n = 0; n < 4; ++n )
								{
									if ( cbArae[i][n] != 255 && cbArae[i][n] != AREA_MAX )
									{
										cbWinArea[cbArae[i][n]] = FALSE;
									}
								}
								cbWinArea[cbArae[i][j]] = TRUE;
							}
						}
					}

					LONGLONG lScore = m_lBankerScore;
					for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex ) 
					{
						if ( cbWinArea[nAreaIndex] == TRUE )
						{
							lScore -= m_lAllBet[nAreaIndex]*(cbMultiple[nAreaIndex] - 1);
						}
						else if ( cbWinArea[AREA_PING] == TRUE && ( nAreaIndex == AREA_XIAN || nAreaIndex == AREA_ZHUANG ) )
						{

						}
						else
						{
							lScore += m_lAllBet[nAreaIndex];
						}
					}
					if ( lBankerScore == -1 )
						lBankerScore = lScore;
					else
						lBankerScore = min(lBankerScore, lScore);
				}
			}
		}
	}

	//�����ע
	LONGLONG lMaxBet = 0L;

	//�����ע
	lMaxBet = min((LONGLONG)pIMeServerUserItem->GetUserScore() - lNowBet, m_lUserLimitScore - m_lPlayBet[wChairID][cbBetArea]);

	lMaxBet = min( lMaxBet, m_lAreaLimitScore - m_lAllBet[cbBetArea]);

	lMaxBet = min( lMaxBet, lBankerScore / (cbMultiple[cbBetArea] - 1));

	//��������
	ASSERT(lMaxBet >= 0);
	lMaxBet = max(lMaxBet, 0);

	//ׯ�жԵ�
	if ( cbBetArea == AREA_XIAN && (m_lAllBet[AREA_ZHUANG] - m_lAllBet[AREA_XIAN]) && lMaxBet < (m_lAllBet[AREA_ZHUANG] - m_lAllBet[AREA_XIAN]) )
		lMaxBet = m_lAllBet[AREA_ZHUANG] - m_lAllBet[AREA_XIAN];
	else if( cbBetArea == AREA_ZHUANG && (m_lAllBet[AREA_XIAN] - m_lAllBet[AREA_ZHUANG]) && lMaxBet < (m_lAllBet[AREA_XIAN] - m_lAllBet[AREA_ZHUANG]) )
		lMaxBet = m_lAllBet[AREA_XIAN] - m_lAllBet[AREA_ZHUANG];

	return lMaxBet;
}


//����÷�
bool CTableFrameSink::CalculateScore( OUT LONGLONG& lBankerWinScore, OUT tagServerGameRecord& GameRecord )
{
	bool bControl = false;
	if ( m_pServerContro && m_pServerContro->NeedControl() && m_pServerContro->ControlResult(m_cbTableCardArray[0], m_cbCardCount))
	{
		bControl = true;
	}

	//�����Ƶ�
	BYTE cbPlayerCount = m_GameLogic.GetCardListPip( m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER] );
	BYTE cbBankerCount = m_GameLogic.GetCardListPip( m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER] );

	//ϵͳ��Ӯ
	LONGLONG lSystemScore = 0l;

	//�ƶ����
	BYTE cbWinArea[AREA_MAX] = {FALSE};
	DeduceWinner(cbWinArea);

	//��Ϸ��¼
	GameRecord.cbBankerCount = cbBankerCount;
	GameRecord.cbPlayerCount = cbPlayerCount;
	GameRecord.bPlayerTwoPair = cbWinArea[AREA_XIAN_DUI] == TRUE;
	GameRecord.bBankerTwoPair = cbWinArea[AREA_ZHUANG_DUI] == TRUE;
	if ( cbWinArea[AREA_TONG_DUI] == TRUE )
		GameRecord.cbKingWinner = AREA_TONG_DUI;
	else if ( cbWinArea[AREA_XIAN_TIAN] == TRUE )
		GameRecord.cbKingWinner = AREA_XIAN_TIAN;
	else if ( cbWinArea[AREA_ZHUANG_TIAN] == TRUE )
		GameRecord.cbKingWinner = AREA_ZHUANG_TIAN;


	//��ҳɼ�
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//������
	BYTE cbMultiple[AREA_MAX] = {MULTIPLE_XIAN, MULTIPLE_PING, MULTIPLE_ZHUANG, 
									MULTIPLE_XIAN_TIAN, MULTIPLE_ZHUANG_TIAN, MULTIPLE_TONG_DIAN, 
									MULTIPLE_XIAN_PING, MULTIPLE_ZHUANG_PING };

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
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if ( m_wCurrentBanker == wChairID ) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for ( WORD wAreaIndex = 0; wAreaIndex < AREA_MAX; ++wAreaIndex )
		{
			if ( cbWinArea[wAreaIndex] == TRUE )
			{
				m_lUserWinScore[wChairID] += (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
				m_lPlayScore[wChairID][wAreaIndex] += (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
				lBankerWinScore -= (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));

				//ϵͳ�÷�
				if(bIsAndroidUser)
					lSystemScore += (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));

				//ׯ�ҵ÷�
				if ( m_wCurrentBanker != INVALID_CHAIR && m_wCurrentBanker != wChairID )
					m_lPlayScore[m_wCurrentBanker][wAreaIndex] -= (m_lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
			}
			else if ( cbWinArea[AREA_PING] == TRUE && ( wAreaIndex == AREA_XIAN || wAreaIndex == AREA_ZHUANG ) )
			{

			}
			else
			{
				lUserLostScore[wChairID] -= m_lPlayBet[wChairID][wAreaIndex];
				m_lPlayScore[wChairID][wAreaIndex] -= m_lPlayBet[wChairID][wAreaIndex];
				lBankerWinScore += m_lPlayBet[wChairID][wAreaIndex];

				//ϵͳ�÷�
				if(bIsAndroidUser)
					lSystemScore -= m_lPlayBet[wChairID][wAreaIndex];
				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore += m_lPlayBet[wChairID][wAreaIndex];

				//ׯ�ҵ÷�
				if ( m_wCurrentBanker != INVALID_CHAIR && m_wCurrentBanker != wChairID )
					m_lPlayScore[m_wCurrentBanker][wAreaIndex] += m_lPlayBet[wChairID][wAreaIndex];
			}

		}

		//����˰��
		if ( m_lUserWinScore[wChairID] > 0.0 )
		{
			m_lUserRevenue[wChairID]  = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}

		//�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker != INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = (SCORE)lBankerWinScore;

		//����˰��
		if (0.0 < m_lUserWinScore[m_wCurrentBanker])
		{
			m_lUserRevenue[m_wCurrentBanker]  = m_pITableFrame->CalculateRevenue(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker]);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = (LONGLONG)m_lUserWinScore[m_wCurrentBanker];
		}		
	}

	// �����в������ڿ����
	if ( bControl )
	{
		return true; 
	}



	//ϵͳ��ֵ����
	if ((lSystemScore + m_StorageStart) < 0l)
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
		m_StorageStart += lSystemScore;
		m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
		return true;
	}
}

//��Ϸ��������
LONGLONG CTableFrameSink::GameOver()
{
	//�������
	LONGLONG lBankerWinScore = 0l;
	bool bSuccess = false;

	CString szTemp;
	bool bRecord;
	LONGLONG lAllScore;
	for(WORD i = 0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL) continue;

		bRecord = false;
		szTemp.Empty();
		lAllScore = 0;
		
		szTemp.AppendFormat(TEXT("P%d,"),i);
		
		for(BYTE j =0;j<AREA_MAX;j++)
		{
			if(m_lPlayBet[i][j]>0)
			{
				bRecord=true;
				szTemp.AppendFormat(TEXT("%s,%I64d,"),TransformAreaInfo(j),m_lPlayBet[i][j]);
				lAllScore+=m_lPlayBet[i][j];
			}
		}

		if(bRecord)
		{
			szTemp.AppendFormat(TEXT("(��:%I64d)"),lAllScore);
			szTemp+=TEXT("#");
			m_GameRecord+=szTemp;
		}
	}
	//��Ϸ��¼
	tagServerGameRecord& GameRecord = m_GameRecordArrary[m_nRecordLast];
	
	//�������
	do 
	{
		//�ɷ��˿�
		DispatchTableCard();

		//��̽���ж�
		lBankerWinScore = 0l;
		bSuccess = CalculateScore( lBankerWinScore, GameRecord );

	} while (!bSuccess);

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore = lBankerWinScore;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	return lBankerWinScore;
}

//�ƶ�Ӯ��
void CTableFrameSink::DeduceWinner(BYTE* pWinArea)
{
	//�����Ƶ�
	BYTE cbPlayerCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//ʤ������--------------------------
	//ƽ
	if( cbPlayerCount == cbBankerCount )
	{
		pWinArea[AREA_PING] = TRUE;

		// ͬƽ��
		if ( m_cbCardCount[INDEX_PLAYER] == m_cbCardCount[INDEX_BANKER] )
		{
			WORD wCardIndex =0;
			for (wCardIndex = 0; wCardIndex < m_cbCardCount[INDEX_PLAYER]; ++wCardIndex )
			{
				BYTE cbBankerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][wCardIndex]);
				BYTE cbPlayerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][wCardIndex]);
				if ( cbBankerValue != cbPlayerValue ) break;
			}

			if ( wCardIndex == m_cbCardCount[INDEX_PLAYER] )
			{
				pWinArea[AREA_TONG_DUI] = TRUE;
			}
		}
	}
	// ׯ
	else if ( cbPlayerCount < cbBankerCount)  
	{
		pWinArea[AREA_ZHUANG] = TRUE;

		//�����ж�
		if ( cbBankerCount == 8 || cbBankerCount == 9 )
		{
			pWinArea[AREA_ZHUANG_TIAN] = TRUE;
		}
	}
	// ��
	else 
	{
		pWinArea[AREA_XIAN] = TRUE;

		//�����ж�
		if ( cbPlayerCount == 8 || cbPlayerCount == 9 )
		{
			pWinArea[AREA_XIAN_TIAN] = TRUE;
		}
	}
	

	//�����ж�
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][0]) == m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][1]))
	{
		pWinArea[AREA_XIAN_DUI] = TRUE;
	}
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][0]) == m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][1]))
	{
		pWinArea[AREA_ZHUANG_DUI] = TRUE;
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

//��ȡ����
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	//��ȡ�Զ�������
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomConfig);

	TCHAR szMaxScore[255] = {0};

	//ÿ��ˢ��
	if (bReadFresh)
	{
		//ÿ��ˢ��
		BYTE cbRefreshCfg = GetPrivateProfileInt(m_szGameRoomName, TEXT("Refresh"), 0, m_szConfigFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;
	}


	m_lApplyBankerCondition = pCustomConfig->lApplyBankerCondition;
	m_lAreaLimitScore = pCustomConfig->lAreaLimitScore;
	m_lUserLimitScore = pCustomConfig->lUserLimitScore;
	m_bEnableSysBanker = (pCustomConfig->nEnableSysBanker == TRUE);

	m_StorageStart = pCustomConfig->StorageStart;

	
	m_StorageDeduct = pCustomConfig->StorageDeduct;

	m_lBankerMAX = pCustomConfig->lBankerMAX;
	m_lBankerAdd = pCustomConfig->lBankerAdd;
	m_lBankerScoreMAX = pCustomConfig->lBankerScoreMAX;
	m_lBankerScoreAdd = pCustomConfig->lBankerScoreAdd;

	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	m_nWinMaxPercent = GetPrivateProfileInt(m_szGameRoomName, TEXT("WinMaxPercent"), 80, m_szConfigFileName);
	m_nWinMinPercent = GetPrivateProfileInt(m_szGameRoomName, TEXT("WinMinPercent"), 20, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;

	if ( m_lBankerScoreMAX <= m_lApplyBankerCondition)
		m_lBankerScoreMAX = 0l;

	m_lPlayerBankerMAX = m_lBankerMAX;

	//ʱ�����
	m_cbFreeTime = pCustomConfig->cbFreeTime;
	m_cbBetTime = pCustomConfig->cbBetTime;
	m_cbEndTime = pCustomConfig->cbEndTime;

	if( m_cbEndTime < TIME_GAME_END )
		m_cbEndTime = TIME_GAME_END;
}

// ��Ӷ���
CString CTableFrameSink::AddComma( LONGLONG lScore )
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%3) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}

	if ( lScore < 0 )
		strReturn.Insert(0, '-');

	return strReturn;
}
//////////////////////////////////////////////////////////////////////////

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
	////��������й���ԱȨ�� �򷵻ش���
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
		return false;
	//
	return true;
}

//��ѯ���
bool CTableFrameSink::OnEventQueryChargeable( IServerUserItem *pIServerUserItem, bool bLookonUser )
{
	//if ( bLookonUser )
	//	return false;

	return false;
}

//��ѯ�����
LONGLONG CTableFrameSink::OnEventQueryCharge( IServerUserItem *pIServerUserItem )
{

	return 0;
}	

//��ת����
CString CTableFrameSink::TransformCardInfo( BYTE cbCardData ) 
{
	CString str = TEXT("");

	BYTE cbCardValue=cbCardData&LOGIC_MASK_VALUE;
	switch(cbCardValue)
	{
		case 0x01:
		{
			str += TEXT("A");
			break;
		}
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
			{
				str.Format( TEXT("%d"),cbCardValue);
				break;
			}
		case 0x0A:
			{
				str += TEXT("10");
				break;
			}
		case 0x0B:
			{
				str += TEXT("J");
				break;
			}
		case 0x0C:
			{
				str += TEXT("Q");
				break;
			}
		case 0x0D:
			{
				str += TEXT("K");
				break;
			}
		default:
			ASSERT(FALSE);
	}
	
	BYTE cbCardColor = (cbCardData&LOGIC_MASK_COLOR);
	switch( cbCardColor )
	{
	case 0x00:
		str += TEXT("����");
		break;
	case 0x10:
		str += TEXT("÷��");
		break;
	case 0x20:
		str += TEXT("����");
		break;
	case 0x30:
		str += TEXT("����");
		break;
	default:
		ASSERT(FALSE);
	}

	return str;
}

//��������
CString CTableFrameSink::TransformAreaInfo( BYTE cbArea )
{
	CString str = TEXT("");

	switch(cbArea)
	{
		case AREA_XIAN:
			str+= TEXT("��");
			break;
		case AREA_PING:
			str+= TEXT("ƽ");
			break;
		case AREA_ZHUANG:
			str+= TEXT("ׯ");
			break;
		case AREA_XIAN_TIAN:
			str+= TEXT("������");
			break;
		case AREA_ZHUANG_TIAN:
			str+= TEXT("ׯ����");
			break;
		case AREA_TONG_DUI:
			str+= TEXT("ͬ��ƽ");
			break;
		case AREA_XIAN_DUI:
			str+= TEXT("�ж���");
			break;
		case AREA_ZHUANG_DUI:
			str+= TEXT("ׯ����");
			break;
		default:
			ASSERT(FALSE);
	}

	return str;
}