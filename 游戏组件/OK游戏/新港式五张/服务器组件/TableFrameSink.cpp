#include "StdAfx.h"
#include "TableFrameSink.h"

SCORE			CTableFrameSink::m_lStockScore = 0L;
LONGLONG		CTableFrameSink::m_lStorageOff = 0L;
//////////////////////////////////////////////////////////////////////////

void TraceMessage(CString szinfo,WORD id);
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
		
	m_GameRecord.Empty();
	ZeroMemory(m_szNickName,sizeof(m_szNickName));
	//��Ϸ����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;

	//��ע��Ϣ
	m_lDrawMaxScore=SCORE_ZERO;
	m_lTurnMaxScore=SCORE_ZERO;
	m_lTurnLessScore=SCORE_ZERO;
	m_lDrawCellScore=SCORE_ZERO;
	//������Ϣ
	m_dwCheatCount=0;
	m_dwCheatGameID=0;
	m_cbCheatType=CHEAT_TYPE_WIN;

	//�û�״̬
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));

	//�����Ϣ
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//�˿˱���
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	m_bWinnerCtrl=false;
	srand( (unsigned)time(NULL));
	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
#ifdef DEBUG
	if(!m_GameRecord.IsEmpty())
		TraceMessage(m_GameRecord,m_pITableFrame->GetTableID());
#endif
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ѯ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);


	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����
	//TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\HKFiveCardConfig.ini"),szPath);
	//��ʼ�洢��Ŀ
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);
	//��ȡ���
	m_lStockScore=GetPrivateProfileInt(szServerName,TEXT("StorageStart"),0,m_szFileName);
	//��ȡ˥��ֵ
	m_lStorageDeduct=GetPrivateProfileInt(szServerName,TEXT("StorageDeduct"),10,m_szFileName);


	//�������˥��ֵ
	if( m_lStorageDeduct < 0 || m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 0;
	if ( m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 1000;



	//�γ��������
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
		if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);
	//�γ��������

	//AI����
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));///////////
	ZeroMemory(m_lScoreLimit,sizeof(m_lScoreLimit));///////////

	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_bNeedWin[i]=false;////////////
		m_bAndroid[i]=false;//////////
	}

	//m_bCtrlThisTime=false;
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl = false;
	
	
	//��ȡ����
	ReadConfigInformation(true);


	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{	
	m_GameRecord.Empty();
	//��Ϸ����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;

	//��ע��Ϣ
	m_lDrawMaxScore=SCORE_ZERO;
	m_lTurnMaxScore=SCORE_ZERO;
	m_lTurnLessScore=SCORE_ZERO;
	m_lDrawCellScore=SCORE_ZERO;

	//�û�״̬
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));

	//�����Ϣ
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//�˿˱���
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	m_bWinnerCtrl=false;
	return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{

	ReadConfigInformation(true);

	//����״̬
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	ZeroMemory(m_szNickName,sizeof(m_szNickName));

	//�ɷ��˿�
	m_cbSendCardCount=2;
	m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem * pUser=m_pITableFrame->GetTableUserItem(i);
		if(pUser==NULL)continue;

		m_cbPlayStatus[i]=TRUE;
	}

	

	//���������û�
	WORD wChairIDCtrl=INVALID_CHAIR;
	//����û�
	WORD wWinChairID = EstimateWinnerEx(0,4);
	//��С�û�
	WORD wLoseChairID=EstimateLoser(0,4);

	
	//���ƴ�������0
	if(m_dwCheatCount>0)
	{
		m_bWinnerCtrl = true;

		if(m_dwCheatGameID==0)
		{
			m_bWinnerCtrl=false;
		}
		else
		{
			wChairIDCtrl = GetChairIdByGameId(m_dwCheatGameID);

			//ָ����Ҳ���
			if( wChairIDCtrl==INVALID_CHAIR)
			{
				m_bWinnerCtrl=false;
			}
		}
	}

	if (m_bWinnerCtrl)
	{
		if(m_cbCheatType==1)
		{
			ProbeJudge(wChairIDCtrl,m_cbCheatType);
		}
		else
		{
			ProbeJudge(wChairIDCtrl,m_cbCheatType);
		
		}
		
		//m_bCtrlThisTime = true;
	}
	else
	{
		//m_bCtrlThisTime = false;
		
		//����������
		AndroidCheck();
	}
	//��ͻ���
	m_lDrawCellScore=(m_pGameServiceOption->lCellScore);


	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//���ñ���
		if (pIServerUserItem!=NULL)
		{
			//״̬����
			m_cbPlayStatus[i]=TRUE;
			m_lTableScore[i]=m_lDrawCellScore;
			m_lUserMaxScore[i] = KeepOne(pIServerUserItem->GetUserScore());

			//�˿�����
			m_cbCardCount[i]=m_cbSendCardCount;

			_sntprintf(m_szNickName[i],CountArray(m_szNickName[i]),TEXT("%s"),pIServerUserItem->GetNickName());
		}
	}

	//��ע����
	RectifyMaxScore();

	//����û�
	wWinChairID = EstimateWinnerEx(0,4);

	//���ñ���
	m_wCurrentUser=EstimateWinner(1,1);
	m_lTurnLessScore=m_lUserScore[m_wCurrentUser]+m_lTableScore[m_wCurrentUser];


	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//���ñ���
	
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.lDrawMaxScore=m_lDrawMaxScore;
	GameStart.lTurnMaxScore=m_lTurnMaxScore;
	GameStart.lTurnLessScore=m_lTurnLessScore;
	GameStart.lCellScore=m_lDrawCellScore;
	
	CopyMemory(GameStart.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

	CMD_S_GameStart1 GameStart1;
	ZeroMemory(&GameStart1,sizeof(GameStart1));

	//���ñ���
	GameStart1.wCurrentUser=m_wCurrentUser;
	GameStart1.lDrawMaxScore=m_lDrawMaxScore;
	GameStart1.lTurnMaxScore=m_lTurnMaxScore;
	GameStart1.lTurnLessScore=m_lTurnLessScore;
	GameStart1.lCellScore=m_lDrawCellScore;

	CopyMemory(GameStart1.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));



	bool bHaveUser = false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;
		if (!pIServerUserItem->IsAndroidUser())
		{
			bHaveUser = true;
			break;
		}
	}
	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i]==FALSE) GameStart.cbCardData[i]=0;
		else GameStart.cbCardData[i]=m_cbHandCardData[i][1];
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i]==FALSE) GameStart1.cbCardData[i]=0;
		else GameStart1.cbCardData[i]=m_cbHandCardData[i][1];
	}
	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��Ϸ����

	//	GameStart.bWillWin=false;
		if (m_cbPlayStatus[i]==TRUE)
		{
		
			m_GameRecord.AppendFormat( TEXT("%s-Card:"),m_szNickName[i] );
			m_GameRecord.AppendFormat( TEXT("%s,"),	TransformCardInfo(m_cbHandCardData[i][0]) );
			m_GameRecord.AppendFormat( TEXT("%s#"),	TransformCardInfo(m_cbHandCardData[i][1]) );

			GameStart.cbObscureCard=m_cbHandCardData[i][0];
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			
			if (pIServerUserItem!=NULL && !pIServerUserItem->IsAndroidUser())
				m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}

		//�Թ�����
		GameStart.cbObscureCard=(m_cbPlayStatus[i]==TRUE)?0xFF:0x00;
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	
		//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��Ϸ����
		//��Ϸ����
		GameStart1.bWillWin=false;

		CopyMemory(GameStart1.cbTableCardData,m_cbHandCardData,sizeof(GameStart1.cbTableCardData));
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart1.cbObscureCard=m_cbHandCardData[i][0];
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem!=NULL && pIServerUserItem->IsAndroidUser())
			{
				GameStart1.bWillWin = (wWinChairID==i);
				m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart1,sizeof(GameStart1));
			}
		}
		
	}
	
	
	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//���ñ���
		if (pIServerUserItem!=NULL)
		{
			m_strPalyName[i].Format(TEXT("%s"), pIServerUserItem->GetNickName() );
		}
	}
	//������˷����û���
//	AndroidSendUserCard();
	return true;
}

void CTableFrameSink::AndroidSendUserCard()
{
	// �����˿�
	CMD_S_GetAllCard gac={0};
	for(int i = 0 ; i < GAME_PLAYER ; i ++)
	{
		for(int j = 0 ; j < MAX_COUNT ; j ++)
		{
			gac.cbCardData[i][j]=(m_cbPlayStatus[i]==TRUE)?m_cbHandCardData[i][j]:0;
		}
	}

	for(WORD k=0;k<GAME_PLAYER;k++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(k);

		if(pIServerUserItem==NULL)continue;
		// �жϻ����˷�
		if((CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))==true)
			m_pITableFrame->SendTableData(k,SUB_S_ANDROID_GET_CARD,&gac,sizeof(CMD_S_GetAllCard));
	}
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
	case GER_NO_PLAYER:		//û�����
		{
			
			if(cbReason==GER_NORMAL)
				m_GameRecord+=TEXT("END_NORMAL:#");
			else
				m_GameRecord+=TEXT("END_NO_PLAYER:#");
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));



			//��������
			WORD wWinerUser=EstimateWinner(0,MAX_COUNT-1);
			//������ע
			SCORE lDrawScore=SCORE_ZERO;
			for (WORD i=0;i<CountArray(m_lTableScore);i++) 
			{
				if(i == wWinerUser) continue;
					lDrawScore+=Double_Round(m_lTableScore[i],2,1);
			}
			//���ֱ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			// ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if ( m_cbPlayStatus[i] == TRUE ) 
				{
					//�ɼ�����
					SCORE lUserScore = (i == wWinerUser)?lDrawScore: -Double_Round(m_lTableScore[i],2,1);
					
					// ��˰
					if((m_pGameServiceOption->wServerType&GAME_GENRE_SCORE)==0 && lUserScore>0.001)
					{
						ScoreInfoArray[i].lRevenue = m_pITableFrame->CalculateRevenue(i, lUserScore);					
						lUserScore-=ScoreInfoArray[i].lRevenue;
					}
					//���û���
					ScoreInfoArray[i].lScore = Double_Round(lUserScore,2,1);
					ScoreInfoArray[i].cbType = (ScoreInfoArray[i].lScore >0.001) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;
					GameEnd.lGameScore[i] = ScoreInfoArray[i].lScore;

					//�˿���Ϣ
					GameEnd.cbCardData[i]=(cbReason!=GER_NO_PLAYER)?m_cbHandCardData[i][0]:0;
					
					m_GameRecord.AppendFormat(TEXT("%s�÷�:%0.2f,%0.2f#"),m_szNickName[i],ScoreInfoArray[i].lScore, ScoreInfoArray[i].lRevenue);
				
				}
				else
				{
					//�����˿�
					GameEnd.cbCardData[i] = 0;

					//���óɼ�
					GameEnd.lGameScore[i] = -m_lTableScore[i];
				}

				//��ʷ����
				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}

			


			
			//ͳ�ƽ���д�ֲ�������
			/////////////////////////////////////////////////////////
			////ͳ�ƿ����Ϣ ֻ�д��ڻ����˲��Ҵ�����ʵ�û�������²Ż����
			bool  bUserAI = false;  //�Ƿ���ڻ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (pIServerUserItemTemp->IsAndroidUser()){
					//���ڻ�����
					bUserAI = true;
					//����FOR
					break;

				}
			}
			//ͳ����Ϣ�������
			SCORE lSystemScore=0.0;    //��ҵ÷�
			SCORE lCurrentStorgeNum = m_lStockScore; //��ǰ���ֵ
			SCORE RevenueScore = 0.0;	 //˰��
			SCORE lStorageDeduct=0.0;	//���˥��
			//���ͳ�� 
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//����û�������
				if (pIServerUserIte==NULL)continue;					
				//���ڻ����˲�ִ��
				if(bUserAI){
					//����û����ǻ�����
					if(!pIServerUserIte->IsAndroidUser())
					{
						//����˰��
						if(ScoreInfoArray[i].lRevenue>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//������˥��
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//ϵͳ���ֵ
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore+RevenueScore,2,1)+lStorageDeduct); 
						//ϵͳ�÷�
						lSystemScore-=ScoreInfoArray[i].lScore;
					}
				}
			}

			////////////////////////////////////////////////////////////

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

			//�ж��Ƿ������ʵ���
			if (bUser)
			{
				
				CString strLog;
				strLog.Format(TEXT("���ţ�%d,��ǰ��棺%0.2f,ϵͳ�÷֣�%0.2f,˰�գ�%0.2f,ʣ���棺%0.2f,���˥����%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ

				//�γ��������
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%I64d"),m_lStockLimit);
				//��¼����Ӯ��¼
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//������ڿ����û��Ļ�//������Ϸ��Ϣ�����ƺ�
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //��ȡ�û��ʺ�
					if(pUserItem==NULL) continue;	//����û�������
					if(pUserItem->IsAndroidUser()) continue;  //����ǻ�����
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //�ж��û�����Ȩ��
					m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //������Ϣ����
				}
			}

			//////////////////////////////////////////////////////
			//�۳����
			m_lStorageOff=GetPrivateProfileInt(m_pGameServiceOption->szServerName,_T("StorageOff"),0,m_szFileName);
			//����ۼ���������0
			if (m_lStorageOff != 0)
			{
				m_lStockScore -= m_lStorageOff;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("��Ϊ����������%I64d��ʣ���棺%0.2f"),m_lStorageOff, m_lStockScore);
				//�����Ϣ
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				//������֮��Ѹ�ֵ���0
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageOff"),TEXT("0"),m_szFileName);

				//���Ϳ��ƺŴ�����ʾ
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //��ȡ�û��ʺ�
					if(pUserItem==NULL) continue; //����û�������
					if(pUserItem->IsAndroidUser()) continue; //����ǻ�����
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue; //�ж��û�����Ȩ��
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_EJECT);  //������Ϣ����

				}
			}

			//������ݴ������

			/////////////////////////////////////////////////



			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			TraceMessage(m_GameRecord,m_pITableFrame->GetTableID());

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);

			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			TraceMessage(m_GameRecord,m_pITableFrame->GetTableID());

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		{
			if(m_cbPlayStatus[wChairID]==TRUE)
			{
				//��������
				OnUserGiveUp(wChairID);
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=(m_pGameServiceOption->lCellScore);

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}
			
				//��������
				CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.lCellScore=(m_pGameServiceOption->lCellScore);
			
			//��ע��Ϣ
			StatusPlay.lDrawMaxScore=m_lDrawMaxScore;
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore;
			StatusPlay.lTurnLessScore=m_lTurnLessScore;
			CopyMemory(StatusPlay.lUserScore,m_lUserScore,sizeof(StatusPlay.lUserScore));
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(StatusPlay.lTableScore));

			//״̬��Ϣ
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.cbShowHand=(m_bShowHand==true)?TRUE:FALSE;
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(StatusPlay.cbPlayStatus));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//������Ŀ
				StatusPlay.cbCardCount[i]=m_cbCardCount[i];

				//�����˿�
				if (m_cbPlayStatus[i]==TRUE)
				{
					if ((i==wChairID)&&(bSendSecret==true)&&pIServerUserItem->GetUserStatus()!=US_LOOKON) 
						StatusPlay.cbHandCardData[i][0]=m_cbHandCardData[i][0];
					CopyMemory(&StatusPlay.cbHandCardData[i][1],&m_cbHandCardData[i][1],(m_cbCardCount[i]-1)*sizeof(BYTE));
				}
			}	//��������
				CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			
			

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_GIVE_UP:			//�û�����
		{
			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserGiveUp(pIServerUserItem->GetChairID());
		}
	case SUB_C_ADD_SCORE:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserAddScore(wChairID,pAddScore->lScore);
		}
		
		case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
			return OnSubAmdinCommand(pIServerUserItem,pData);
		}
	}

	return false;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(pIServerUserItem->GetChairID());

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserLeave(pIServerUserItem->GetChairID());

	return true;
}

//�û�����
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	if(m_cbPlayStatus[wChairID]==FALSE)
		return true;
	//���ñ���
	m_cbPlayStatus[wChairID]=FALSE;

	// ��ȡ���
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

	// �۳�����
	SCORE lDeficiencyPoint = -(m_lUserScore[wChairID] + m_lTableScore[wChairID] );

	// �۳������
	if ( lDeficiencyPoint < 0.000 &&  pIServerUserItem->GetUserScore()+lDeficiencyPoint<0.001 )
	{
		lDeficiencyPoint =  -pIServerUserItem->GetUserScore();
	}

	//д�����
	tagScoreInfo  ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo.lScore=lDeficiencyPoint;
	ScoreInfo.cbType=SCORE_TYPE_LOSE;
	m_pITableFrame->WriteUserScore(wChairID, ScoreInfo);

	//��Ϸ��¼
	m_GameRecord.AppendFormat(TEXT("%s,��,�÷�:%0.2f,%0.2f#"),m_szNickName[wChairID],ScoreInfo.lScore, 0);

	//��������
	WORD wPlayerCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//�����ж�
	if (wPlayerCount>=2)
	{
		//��ע����
		RectifyMaxScore();

		//�л��û�
		if (m_wCurrentUser==wChairID)
		{
			//�����û�
			m_wCurrentUser=INVALID_CHAIR;

			//�û�����
			for (WORD i=1;i<GAME_PLAYER;i++)
			{
				//��������
				WORD wCurrentUser=(wChairID+i)%GAME_PLAYER;
				SCORE lDrawAddScroe=m_lUserScore[wCurrentUser]+m_lTableScore[wCurrentUser];

				//״̬�ж�
				if (m_cbPlayStatus[wCurrentUser]==FALSE) continue;

				//�û��л�
				if ((m_cbOperaScore[wCurrentUser]==FALSE)||(lDrawAddScroe<m_lTurnLessScore))
				{
					m_wCurrentUser=wCurrentUser;
					break;
				}
			}
		}
	}
	else 
	{
		//�㼯���
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			m_lTableScore[i]+=m_lUserScore[i];
			m_lUserScore[i]=SCORE_ZERO;
		}

		//�����û�
		m_wCurrentUser=INVALID_CHAIR;
	}

	//��������
	CMD_S_GiveUp GiveUp;
	ZeroMemory(&GiveUp,sizeof(GiveUp));

	//���ñ���
	GiveUp.wGiveUpUser=wChairID;
	GiveUp.wCurrentUser=m_wCurrentUser;
	GiveUp.lDrawMaxScore=m_lDrawMaxScore;
	GiveUp.lTrunMaxScore=m_lTurnMaxScore;
		
	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//������Ϸ
	if (wPlayerCount<=1)
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);
		return true;
	}

	//�����˿�
	if (m_wCurrentUser==INVALID_CHAIR) DispatchUserCard();

	return true;
}

//�û���ע
bool CTableFrameSink::OnUserAddScore(WORD wChairID, SCORE lScore)
{
	//״̬Ч��
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false;

	SCORE lAddScore = (lScore);
	//��עЧ��
	ASSERT((lAddScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])<=m_lTurnMaxScore);
	ASSERT((lAddScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])>=m_lTurnLessScore);
	ASSERT(lAddScore>-0.000001);

	//��עЧ��
	if ((lAddScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])>m_lTurnMaxScore) 
	{
		lAddScore = Double_Round(m_lTurnMaxScore-m_lUserScore[wChairID]-m_lTableScore[wChairID],2,1);
	}
	if (lAddScore<-0.001||((lAddScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])<m_lTurnLessScore))
	{
		lAddScore = Double_Round(m_lTurnLessScore-m_lTableScore[wChairID]-m_lUserScore[wChairID],2,1);
	}

	//���ñ���
	m_cbOperaScore[wChairID]=TRUE;
	m_lUserScore[wChairID]+=lAddScore;
	m_lTurnLessScore=m_lUserScore[wChairID]+m_lTableScore[wChairID];

	//��Ϸ��¼
	m_GameRecord.AppendFormat(TEXT("%s��,%0.2f,����ע:%0.2f,����:%0.2f#"),m_szNickName[wChairID],lAddScore,m_lUserScore[wChairID],m_lTableScore[wChairID]);

	//״̬����
	m_wCurrentUser=INVALID_CHAIR;
	m_bShowHand=(abs(m_lTurnLessScore-m_lDrawMaxScore)<0.001);

	//�û�����
	for (WORD i=1;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wCurrentUser=(wChairID+i)%GAME_PLAYER;
		//״̬�ж�
		if (m_cbPlayStatus[wCurrentUser]==FALSE) continue;

		SCORE lDrawAddScroe=m_lUserScore[wCurrentUser]+m_lTableScore[wCurrentUser];

		//�û��л�
		if ((m_cbOperaScore[wCurrentUser]==FALSE)||(m_lTurnLessScore-lDrawAddScroe>0.001))
		{
			m_wCurrentUser=wCurrentUser;
			break;
		}
	}

	//��������
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore,sizeof(AddScore));

	//���ñ���
	AddScore.wAddScoreUser=wChairID;
	AddScore.wCurrentUser=m_wCurrentUser;
	AddScore.lTurnLessScore=m_lTurnLessScore;
	AddScore.lUserScoreCount=m_lUserScore[wChairID];


	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));

	//�����˿�
	if (m_wCurrentUser==INVALID_CHAIR) DispatchUserCard();

	return true;
}

//������ע
VOID CTableFrameSink::RectifyMaxScore()
{
	//���ñ���
	m_lDrawMaxScore=SCORE_ZERO;
	m_lTurnMaxScore=SCORE_ZERO;

	//�����ע
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�û��ж�
		if (m_cbPlayStatus[i]==FALSE) continue;

		//��������
		if ((m_lDrawMaxScore<=0.001&&m_lDrawMaxScore>=-0.001)||(m_lDrawMaxScore-m_lUserMaxScore[i]>0.001)) m_lDrawMaxScore=m_lUserMaxScore[i];
	}

	//��ǰ��ע
	if (m_cbSendCardCount<=2) m_lTurnMaxScore=KeepOne(m_lDrawMaxScore/4);
	else if (m_cbSendCardCount==3) m_lTurnMaxScore=KeepOne(m_lDrawMaxScore/2);
	else m_lTurnMaxScore=KeepOne(m_lDrawMaxScore);

	return;
}

//�����˿�
VOID CTableFrameSink::DispatchUserCard()
{
	//�㼯���
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_lTableScore[i]+=m_lUserScore[i];
		m_lUserScore[i]=SCORE_ZERO;
	}

	//�����ж�
	if (m_cbSendCardCount==MAX_COUNT)
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
		return;
	}

	//�ɷ��˿�
	BYTE cbSourceCount=m_cbSendCardCount;
	m_cbSendCardCount=(m_bShowHand==false)?(m_cbSendCardCount+1):MAX_COUNT;

	//��ǰ�û�
	if (m_bShowHand==false)
	{
		//״̬����
		ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));

		//�����û�
		m_wCurrentUser=EstimateWinner(1,m_cbSendCardCount-1);

		//��ע����
		m_lTurnMaxScore=((m_cbSendCardCount>=4)?m_lDrawMaxScore:m_lDrawMaxScore/2.000);
		m_lTurnLessScore=(m_lUserScore[m_wCurrentUser]+m_lTableScore[m_wCurrentUser]);
	}
	else
	{
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;
		m_lTurnMaxScore=m_lDrawMaxScore;
		m_lTurnLessScore=m_lDrawMaxScore;
	}

	//��������
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard,sizeof(SendCard));

	//���ñ���
	SendCard.wCurrentUser=m_wCurrentUser;
	SendCard.lTurnMaxScore=m_lTurnMaxScore;
	SendCard.wStartChairID=EstimateWinner(1,cbSourceCount-1);
	SendCard.cbSendCardCount=m_cbSendCardCount-cbSourceCount;

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//״̬�ж�
		if (m_cbPlayStatus[i]==FALSE) continue;

		m_GameRecord.AppendFormat( TEXT("%s-����:"),m_szNickName[i] );

		//������Ŀ
		m_cbCardCount[i]=m_cbSendCardCount;

		//�ɷ��˿�
		for (BYTE j=0;j<(m_cbSendCardCount-cbSourceCount);j++)
		{
			ASSERT(j<CountArray(SendCard.cbCardData[i]));
			SendCard.cbCardData[j][i]=m_cbHandCardData[i][cbSourceCount+j];
			m_GameRecord.AppendFormat( TEXT("%s,"),	TransformCardInfo(SendCard.cbCardData[j][i]) );
		}
		m_GameRecord+=TEXT("#");
	}

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	//��������
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

	return;
}

//�ƶ�����
WORD CTableFrameSink::EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	WORD  wLoser=0;
	for ( wLoser=0;wLoser<GAME_PLAYER;wLoser++)
	{
		if (m_cbPlayStatus[wLoser]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	WORD i=0;
	for ( i=(wLoser+1);i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1)==false) 
		{
			wLoser=i;
		}
	}

	return wLoser;
}

//�ƶ�ʤ��
WORD CTableFrameSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	WORD wWinner=0;
	for ( wWinner=0;wWinner<GAME_PLAYER;wWinner++)
	{
		if (m_cbPlayStatus[wWinner]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}
	//�Ա����
	WORD wId = wWinner;
	for (WORD i=0;i<GAME_PLAYER-1;i++)
	{
		wId = (wId+1)%GAME_PLAYER;

		//�û�����
		if (m_cbPlayStatus[wId]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[wId]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[wId]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=wId;
		}
	}

	return wWinner;
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

//�����Ϣ
void TraceMessage(CString szinfo,WORD id)
{
	if(!szinfo.IsEmpty())
		szinfo.Replace(TEXT("#"),TEXT("\r\n"));
	
	COleDateTime timeNow, dateNow;
	timeNow = COleDateTime::GetCurrentTime();     // ��ȡ��ǰ����ʱ��
	dateNow = COleDateTime::GetCurrentTime();     // ͬ����ȡ��ǰ����ʱ��
	CString sTime = timeNow.Format(VAR_TIMEVALUEONLY);     // ��ȡ��ǰʱ��
	CString sDate = dateNow.Format(VAR_DATEVALUEONLY);     // ��ȡ��ǰ����

	TCHAR tchBuffer[MAX_PATH ]; 
	LPTSTR lpszCurDir;
	lpszCurDir = tchBuffer; 
	GetCurrentDirectory(MAX_PATH , lpszCurDir);//�õ���ǰ����·��

	CString Filep = lpszCurDir;
	Filep.AppendFormat(TEXT("\\%s"),GAME_NAME);

	//�����ļ���	��Ϸ����
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ�����ļ���ʧ�ܣ�")+Filep,TraceLevel_Exception);
		 }
	}
	Filep.AppendFormat(TEXT("\\TableID_%d"),id);

	//�����ļ���	���ӷ���
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ�����ļ���ʧ�ܣ�")+Filep,TraceLevel_Exception);
		 }
	}
	Filep.AppendFormat(TEXT("\\%s"),sDate);
	
	//�����ļ���	���ڷ���
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ�����ļ���ʧ�ܣ�")+Filep,TraceLevel_Exception);
		 }
	}

	
	CString FileName=TEXT("");     // �ļ��� = ����_ʱ��.txt
	FileName.AppendFormat(TEXT("%s\\%s.txt"),Filep,sTime);

	TCHAR pFilePath[MAX_PATH];
	LPCTSTR filepath = FileName;
	int FilePathLength = FileName.GetLength();     // ����ļ������ȣ�CFile������CString��ֻ����string��������Ҫ��ת��
	int i=0;
	for( i=0; i < FilePathLength ; i++)
	{
		pFilePath[i] = FileName.GetAt(i);     // CString -> string
		if( (pFilePath[i] == TEXT(':')) && (i != 1))     // �޳�':'�Ȳ�����Ϊ�ļ����ķ���,��������Ŀ¼���ð��
			pFilePath[i] = '-';
	}
	pFilePath[i] = '\0';     // �ļ�������

	CFile File;
	if ((File.Open(pFilePath,CFile::modeWrite)==FALSE)&&
		(File.Open(pFilePath,CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	File.SeekToEnd();
	File.Write(szinfo.GetBuffer(),szinfo.GetLength()*2);

	szinfo.ReleaseBuffer();
	File.Flush();
	File.Close();

	return;
}

//�ƶ�ʤ��
WORD CTableFrameSink::EstimateWinnerEx(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	WORD wWinner=0;
	for ( wWinner=0;wWinner<GAME_PLAYER;wWinner++)
	{
		if (m_cbPlayStatus[wWinner]==TRUE) 
		{
			//�����˿�
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	WORD wId = wWinner;
	for (WORD i=0;i<GAME_PLAYER-1;i++)
	{
		wId = (wId+1)%GAME_PLAYER;

		//�û�����
		if (m_cbPlayStatus[wId]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[wId]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[wId]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=wId;
		}
	}

	return wWinner;
}

//////////////////////////////////////////////////////////////////////////
//��ȡ����
VOID CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	
	TCHAR OutBuf[255];
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);
	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\HKFiveCardConfig.ini"),szPath);
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

	//�Ƿ��ȡÿ��ˢ�����ñ���
	if (bReadFresh)
	{
		//ÿ��ˢ��
		BYTE cbRefreshCfg = GetPrivateProfileInt(szServerName, TEXT("Refresh"), 0, szFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;

		//�Ƿ�ˢ�¿��
		BYTE cbRefreshStorageCfg = GetPrivateProfileInt(szServerName, TEXT("RefreshStorage"), 0, szFileName);
		m_bRefreshStorageCfg = cbRefreshStorageCfg?true:false;

	}

	//�������
	if(m_bRefreshStorageCfg)
	{
		//���ÿ��
		memset(OutBuf,0,255);
		GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,szFileName);
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%0.2f"),&m_lStockScore);
	}


	//����˥��ֵ
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageDeduct);

	//������ֵ
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageMax"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageMax);

		//�γ��������
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
		if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	//m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);


	//ϵͳ����Ӯ��
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageLimit"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStockLimit);

	

		//�γ��������   //����ǰ̨����

	////2012-08-28
	//���������Ӯ
	//������Ҹ���
	m_WinCount = GetPrivateProfileInt(szServerName, TEXT("WinCount"), 0, szFileName);	
	//�Ƿ����
	BYTE cbWinnerCtrl = GetPrivateProfileInt(szServerName, TEXT("WinnerCtrl"), 0, szFileName);
	m_bWinnerCtrl = cbWinnerCtrl?true:false;
	//��Ҫʤ�������UserID
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("WinnerUserID"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%lu"),&m_dwCheatGameID);
	//��ȡʤ������
	m_cbCheatType = GetPrivateProfileInt(szServerName, TEXT("CheatType"), 1, szFileName);
	//��ȡ���ƴ���
	m_dwCheatCount = GetPrivateProfileInt(szServerName, TEXT("CheatCount"), 0, szFileName);
	//�����Ӯ�ٷֱ�
	m_WinningRatio = GetPrivateProfileInt(szServerName, TEXT("WinningRatio"), 0, szFileName);


	/////////////////



	//////��ȡ��������Ӯ
	BYTE cbAndroidWinningCtrl = GetPrivateProfileInt(szServerName, TEXT("AndroidWinningCtrl"), 0, szFileName);
	m_bAndroidWinningCtrl = cbAndroidWinningCtrl?true:false;

	//////��ȡ��������Ӯ����
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("AndroidWinningRatio"),TEXT("0"),OutBuf,60,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_AndroidWinningRatio);
	if (m_AndroidWinningRatio<0 || m_AndroidWinningRatio >100)
	{
		m_AndroidWinningRatio = 60;
	}




}

//��̽���ж�, ȷ��Chair���û�����Ӯ
void CTableFrameSink::ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType)
{	
	//��ȡ�û�

	IServerUserItem * pIServerCtrlUserItem=m_pITableFrame->GetTableUserItem(wChairIDCtrl);
	//���û�������, ����Ҫ����

	if( NULL==pIServerCtrlUserItem ) return;
	if( pIServerCtrlUserItem->GetUserID()!=m_dwCheatGameID ) return;
	
	//�����ֵ�������
	int iMaxIndex= EstimateWinnerEx(0,4);
	int iLostIndex=EstimateLoser(0,MAX_COUNT-1);

	//��ʱ����
	BYTE cbTemp[MAX_COUNT]={0};
	//������
	WORD wWinerUser=iMaxIndex;

	if(cbCheatType==CHEAT_TYPE_LOST) wWinerUser=iLostIndex;

	if (wWinerUser!=wChairIDCtrl)
	{

		CopyMemory(cbTemp,m_cbHandCardData[wWinerUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wWinerUser],m_cbHandCardData[wChairIDCtrl],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wChairIDCtrl],cbTemp,MAX_COUNT);

	}

				//���ٿ��ƴ���
	if(m_dwCheatCount>0) m_dwCheatCount--;
	if(m_dwCheatCount==0) {m_dwCheatGameID=0;}
	m_bWinnerCtrl = false;

			//д���������
			CString TempValue;
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
	
	return;
}

//��ȡ���Ӻ�
WORD CTableFrameSink::GetChairIDFromGameID(DWORD dwGameID)
{
	for(int i=0; i<GAME_PLAYER; i++)
	{
		IServerUserItem * pUser=m_pITableFrame->GetTableUserItem(i);
		if(pUser==NULL)continue;
		if(pUser->GetUserID()==dwGameID)
		{
			return i;
		}
	}
	return INVALID_CHAIR;
}

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
	//��������й���ԱȨ�� �򷵻ش���
	if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);
	switch(AdminReq->cbReqType)
	{
	case RQ_OPTION_CANCLE:	//ȡ��
		{
			if (m_dwCheatGameID!=0)
			{
				//������ʾ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("�Ѿ��ɹ���GameIDΪ:%d������Ȩ��ȡ��"),m_dwCheatGameID);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
				m_dwCheatGameID=0;
			}
			else
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("��ǰû�����������û�!"),SMT_CHAT);
			}
			break;
		}
	case RQ_OPTION_QUERYING:			//��ѯ
		{
			if (m_dwCheatGameID!=0)
			{
				TCHAR szType[][32]={TEXT("��"),TEXT("Ӯ")};

				if(m_cbCheatType!=0 && m_cbCheatType!=1) break;

				//������ʾ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��ǰ����GameID��%d��ʣ����ƴ�����%d�֣��������ͣ�%s\n"),
					m_dwCheatGameID,m_dwCheatCount,szType[m_cbCheatType]);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			}
			else
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("��ǰû�����������û�!"),SMT_CHAT);
			}
			break;
		}
	case RQ_OPTION_SETING:		//����
		{
			m_dwCheatGameID=AdminReq->dwGameID;
			m_dwCheatCount=AdminReq->cbCheatCount;
			m_cbCheatType=AdminReq->cbCheatType;



						//�γ��������
			//д�������ļ�
			CString TempValue;
			//д���������
			TempValue.Format(TEXT("%d"),m_cbCheatType);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatType"),TempValue,m_szFileName);
			//д���������
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
			//д������û�ID
			TempValue.Format(TEXT("%d"),m_dwCheatGameID);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("WinnerUserID"),TempValue,m_szFileName);
			//�γ��������


			m_dwCheatGameID1 = pIServerUserItem->GetUserID();
			TCHAR szType[][32]={TEXT("��"),TEXT("Ӯ")};

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("�ɹ�ΪGameID:%d��������Ȩ��,���ƣ�%d�Σ����ͣ�%s"),m_dwCheatGameID,m_dwCheatCount,szType[m_cbCheatType]);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			break;
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

//�����û�Id��ȡ������������Id
WORD CTableFrameSink::GetChairIdByGameId(DWORD dwGameId)
{
	WORD wChairID;
	for (wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;

		if( pIServerUserItem->GetUserID() == dwGameId )
		{
			return wChairID;
		}
	}

	return INVALID_CHAIR;
}

//������
bool CTableFrameSink::AndroidCheck()
{



		//�����˸���
	int iAndroidCount=0;
	//���˸���
	int iPlayerCount=0;

	//�����˻���Ŀ������
	int iPostionAndroid[GAME_PLAYER]={0};
	//���˻���Ŀ������
	int iPostionPlayer[GAME_PLAYER]={0};

	//�����ֵ����
	WORD iMaxIndex=INVALID_CHAIR;	

	//��ʼ��ָ��
	IServerUserItem *pIServerUserItem=NULL;

	//��ȡ���������˸�������Ҹ���
	for(WORD k=0;k<GAME_PLAYER;k++)
	{
		pIServerUserItem=m_pITableFrame->GetTableUserItem(k);
		if (pIServerUserItem!=NULL)
		{
			if (pIServerUserItem->IsAndroidUser())
			{
				//��¼����
				iPostionAndroid[iAndroidCount]=k;
				iAndroidCount++;
			}
			else
			{
				//��¼����
				iPostionPlayer[iPlayerCount]=k;
				iPlayerCount++;
			}
		}
	}

	//û�л����˷���
	if(iAndroidCount==0) return false;
	//û�����˷���
	if (iPlayerCount==0) return false;
	//������0 ����
	if(m_lStockScore>=m_lStorageMax) return false;

	


	//���Ʊ���
	int iTarget=0;//Ŀ������
	bool bChange = false;//�Ƿ�Ҫ����

	

	//iMaxindexitem��ȡ
//	pIServerUserItem=m_pITableFrame->GetTableUserItem(iMaxIndex);
//	if(pIServerUserItem == NULL) return false; 

	
//	if(pIServerUserItem->IsAndroidUser()==FALSE)//����Ʋ��ڻ���������
	{
		iTarget=iPostionAndroid[rand()%iAndroidCount];
//		bChange = true;
	}

	



	iMaxIndex =  EstimateWinnerEx(0,4);

	//����
//	if (bChange)
	{
		//��ʱ����
		BYTE cbTemp[MAX_COUNT]={0};

		//������
		CopyMemory(cbTemp,m_cbHandCardData[iMaxIndex],MAX_COUNT);
		CopyMemory(m_cbHandCardData[iMaxIndex],m_cbHandCardData[iTarget],MAX_COUNT);
		CopyMemory(m_cbHandCardData[iTarget],cbTemp,MAX_COUNT);

	

	}

return true;

}
