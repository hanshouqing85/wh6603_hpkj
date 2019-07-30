#include "StdAfx.h"
#include "TableFrameSink.h"


/////////////////��̬����

SCORE				CTableFrameSink::m_lStockScore = 0.0;
LONGLONG			CTableFrameSink::m_lStorageOff = 0L;
BYTE				CTableFrameSink::StorageStyle=1;
//////////////////////////////////////////////////////////////////////////////////
#define IDI_CHECK_TABLE				1					//�������
//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_pITableFrame=NULL;
	m_pGameCustomRule=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;

	//ը������
	m_wFirstUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//��Ϸ����
	m_cbBombCount=0;
	ZeroMemory(m_cbEachBombCount,sizeof(m_cbEachBombCount));

	//�з���Ϣ
	m_cbBankerScore=0;
	ZeroMemory(m_cbScoreInfo,sizeof(m_cbScoreInfo));

	//������Ϣ
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�˿���Ϣ
	ZeroMemory(m_cbBankerCard,sizeof(m_cbBankerCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));


	//�������
	m_hControlInst = NULL;
	m_pServerControl = NULL;
	m_hControlInst = LoadLibrary(TEXT("LandServerControl.dll"));
	if ( m_hControlInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hControlInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerControl = static_cast<IServerControl*>(ServerControl());
		}
	}

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
	if( m_pServerControl )
	{
		delete m_pServerControl;
		m_pServerControl = NULL;
	}

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
	ZeroMemory(m_cbEachBombCount,sizeof(m_cbEachBombCount));

	//�з���Ϣ
	m_cbBankerScore=0;
	ZeroMemory(m_cbScoreInfo,sizeof(m_cbScoreInfo));

	//������Ϣ
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�˿���Ϣ
	ZeroMemory(m_cbBankerCard,sizeof(m_cbBankerCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	return;
}

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//�����ж�
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("��Ϸ���� CTableFrameSink ��ѯ ITableFrame �ӿ�ʧ��"),TraceLevel_Exception);
		return false;
	}

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//�Զ�����
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();


	
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����
	_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\LandConfig.ini"),szPath);
	//��ʼ�洢��Ŀ
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

		//��ȡ���
		//m_lStockScore=GetPrivateProfileInt(szServerName,TEXT("StorageStart"),0,m_szFileName);
		///////////////////��ȡС��ת�� �γ���������
		TCHAR OutBuf[255];
		memset(OutBuf,0,255);
		GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,m_szFileName);
			
		CString TempValue;
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
		m_lStockScore=Mywtof(TempValue);
		///////////////////////////////


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

	ReadConfigInformation(true);

	return true;
}

//��������
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//���ٻ���
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//��ȡ���
	ReadConfigInformation(true);

	//�γ��������
	//��ճ�����Ϣ
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	/////

	//����״̬
	m_pITableFrame->SetGameStatus(GAME_SCENE_CALL);

	//�����˿�
	BYTE cbRandCard[FULL_COUNT];
	m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

		//�������������һ������з�
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0 )
	{
		//AfxMessageBox(TEXT("���������׽����"));
		m_wFirstUser = INVALID_CHAIR;
	}

	//��������ׯ�û�
	WORD wStartUser=rand()%GAME_PLAYER;
	WORD wCurrentUser=wStartUser;

	//�·����ǿ�����ص�
	////////////////////////////////////////////////////////
	//�ж��Ƿ����ָ�����
	WORD wChairIDCtrl = INVALID_CHAIR;
	bool m_bWinnerCtrl = false;

	//����Ա
	WORD IsAdmin = INVALID_CHAIR;
	//�����û�ID 
	WORD WinCharID = INVALID_CHAIR;
	//���ƻ�����
	WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
	//���ƻ��ʱ���
	bool ReturnValue=false;

	///////////////////////////////
	///////////////////////////////////////////ָ����ҿ��ƿ���
	if(m_dwCheatCount>0)  //���ƴ���
	{
		m_bWinnerCtrl = true;  //������ƴ�������0��ָ�������Ӯ���ƿ���

		if(m_dwCheatGameID==0)  //������ID����0 ָ�������Ӯ���ƹر�
		{
			m_bWinnerCtrl=false;  
		}
		else   //���ID����0
		{
			//����ID
			wChairIDCtrl = GetChairIdByGameId(m_dwCheatGameID);  //��ȡ������Ӻ�

			//ָ����Ҳ���
			if( wChairIDCtrl==INVALID_CHAIR)
			{
				m_bWinnerCtrl=false;
			}
		}
	}else{
		//���ƴ�������0
		m_bWinnerCtrl=false;
	}

	//����Ա�Ƿ����
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		//��ȡ�û�
		IServerUserItem * pServerUserItem = m_pITableFrame->GetTableUserItem( wChairID ) ;
		if (pServerUserItem==NULL) continue;
		//�������Ա,��¼ID
		if(CUserRight::IsGameCheatUser(pServerUserItem->GetUserRight()))
		{
			IsAdmin=wChairID;
		}
	}

	//������ƺŲ����ڻ��߹���Ա������,������
	//�����Ƴ�ʼ������
	if(m_bWinnerCtrl==false&&IsAdmin==INVALID_CHAIR){
	
	///�γ�������� ���ӷ��ͺ���
	///////////////////////////////////////////////////////////////////////
	//������
	//�������
	srand( (unsigned)time(NULL));
	//��ȡ�����
	WORD Ratio = rand()%(100); //����0~100����

		//�γ��������
	WORD WinRatio = 100;         //���嵱ǰ��Ӯ���� //Ĭ�ϱ���
	if(StorageStyle==1) WinRatio = StorageRatio;            //���嵱ǰ��Ӯ���� //Ĭ�ϱ���
	if(StorageStyle==2) WinRatio = StorageMinRatio;         //���嵱ǰ��Ӯ���� //ɱ��
	if(StorageStyle==3) WinRatio = StorageMaxRatio;         //���嵱ǰ��Ӯ���� //�ͷ�

	//������ɱ�ֻ����ͷ�
	//�Ե����
	if(m_lStockScore>=m_lStorageMax && StorageStyle!=3){
		
		SCORE tmp_lStockScore = m_lStockScore;							 //ԭ�����
		SCORE m_StorageEating=m_lStockScore*(StorageEating/100.00);		 //ϵͳ�Է�
			
		//��ȥ�Ե��ķ�
		m_lStockScore-=m_StorageEating;
		WinRatio=StorageMaxRatio;  //�ͷ�ģʽ
	
		//�ͷ�ģʽ
		StorageStyle=3;
		//������ֵ
		CString strLog;
		strLog.Format(TEXT("���ţ�%d,��ǰ��棺%0.2f,ϵͳ�Է֣�%0.2f,ʣ���棺%0.2f"),m_pITableFrame->GetTableID()+1,tmp_lStockScore,m_StorageEating,m_lStockScore);
		CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ
	
	}
	
	//���������Сֵ,ɱ��ģʽ
	if(m_lStockScore<=m_lStorageMin && StorageStyle!=2)
	{
		
		WinRatio=StorageMinRatio;
		StorageStyle=2;//ɱ��ģʽ
	}

	ReturnValue=(WinRatio>Ratio)?true:false; //�������

	 //�����ǰģʽ
	CString strLog;
	strLog.Format(TEXT("���ţ�%d,��Ϸģʽ��%d,��ǰ��Ӯ���ʣ�%d (��Ϸģʽ1,����,2,ɱ��,3,�ͷ�)"),m_pITableFrame->GetTableID()+1,StorageStyle,WinRatio);
	CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ

	//�γ�����
	//���ͺ��ƿ�ʼ
	//����������
	BYTE cbAndroidUserCount = 0 ;
	//���������
	WORD wAndroidUser[GAME_PLAYER] = {0};  
		
	//�����˸���  
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		//��ȡ�û�
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem( wChairID ) ;
		if (pServerUserItem==NULL) continue;
		//����ǻ�����
		if ( pServerUserItem->IsAndroidUser())
		{
			//��¼������ID
			wAndroidUser[ cbAndroidUserCount ] = ( wChairID ) ;
			//������������һ
			cbAndroidUserCount++;
		}
	}
	
	//�����ȡһ�������˷��ͺ���
	wHaveGoodCardAndroidUser = INVALID_CHAIR ;
	//�����������������1 ������һ�������˽��պ���
	if (  cbAndroidUserCount >0 ) wHaveGoodCardAndroidUser = wAndroidUser[ rand() % cbAndroidUserCount ] ;
	
	}


	//����Ա�����Ż�
	if(IsAdmin!=INVALID_CHAIR){
	
		
		//�����˿�
		BYTE cbRandCard[FULL_COUNT];
		m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

		//��ȡ�����˿�����
		BYTE cbGoodCard[ NORMAL_COUNT ] ;
		//��ȡ����
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//cbGoodCard��������Ѿ��Ǻ���������,������������Ա����
		//�ַ��˿�
		m_cbHandCardCount[ IsAdmin ] = NORMAL_COUNT;
		CopyMemory(&m_cbHandCardData[ IsAdmin ], cbGoodCard, NORMAL_COUNT );
		//ɾ����������
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, cbRandCard, FULL_COUNT ) ;
		
		//�����û��˿�����
		for ( WORD i = 0, j = 0; i < GAME_PLAYER; i++ )
		{
			//�����ǰ�û����ǹ���Ա�����˿����ݸ���
			if ( i != IsAdmin )
			{
				//�����û��˿����ݳ���
				m_cbHandCardCount[ i ] = NORMAL_COUNT;
				//�����˿����ݸ��û�
				CopyMemory( &m_cbHandCardData[ i ],&cbRandCard[j*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
				++j ;
			}
		}

			//��Ϊ�����û��˿��Ѿ�ɾ��,�˿�������ֻ��37����2*NORMAL_COUNT=34 ȡ��ǰ����,3���ַ�
			//���õ���
			CopyMemory(m_cbBankerCard,&cbRandCard[2*NORMAL_COUNT],sizeof(m_cbBankerCard));
			//���е���
			m_GameLogic.SortCardList(m_cbBankerCard,sizeof(m_cbBankerCard),ST_ORDER);
	

	}
	else if(m_bWinnerCtrl){
		//ָ�������Ӯ����
		
		//�����˿�
		BYTE cbRandCard[FULL_COUNT];
		m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

		//��ȡ�����˿�����
		BYTE cbGoodCard[ NORMAL_COUNT ] ;
		//��ȡ����
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//cbGoodCard��������Ѿ��Ǻ���������,������������Ա����
		//�ַ��˿�
		m_cbHandCardCount[ wChairIDCtrl ] = NORMAL_COUNT;
		CopyMemory(&m_cbHandCardData[ wChairIDCtrl ], cbGoodCard, NORMAL_COUNT ) ;
		
		//ɾ����������
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, cbRandCard, FULL_COUNT) ;
		
		//�����û��˿�����
		for ( WORD i = 0, j = 0; i < GAME_PLAYER; i++ )
		{
			//�����ǰ�û����ǹ���Ա�����˿����ݸ���
			if ( i != wChairIDCtrl )
			{
				//�����û��˿����ݳ���
				m_cbHandCardCount[ i ] = NORMAL_COUNT;
				//�����˿����ݸ��û�
				CopyMemory( &m_cbHandCardData[ i ],&cbRandCard[j*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
				++j ;
			}
		}

			//��Ϊ�����û��˿��Ѿ�ɾ��,�˿�������ֻ��37����2*NORMAL_COUNT=34 ȡ��ǰ����,3���ַ�
			//���õ���
			CopyMemory(m_cbBankerCard,&cbRandCard[2*NORMAL_COUNT],sizeof(m_cbBankerCard));
			//���е���
			m_GameLogic.SortCardList(m_cbBankerCard,sizeof(m_cbBankerCard),ST_ORDER);

			////////////////////////////
			////������Ϣ��ʼ��
			if(m_dwCheatCount>0) m_dwCheatCount--;
			if(m_dwCheatCount==0) {m_dwCheatGameID=0;}
			m_bWinnerCtrl = false;
			///////////////////////////

			//�γ��������
			//д�������ļ�
			CString TempValue;
			
			//д���������
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
		
			//�γ��������


	
	}
	//���ͺ��� //������
	else if ( ReturnValue && wHaveGoodCardAndroidUser != INVALID_CHAIR )
	{
	
		
		//�����˿�
		BYTE cbRandCard[FULL_COUNT];
		m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

		//��ȡ�����˿�����
		BYTE cbGoodCard[ NORMAL_COUNT ] ;
		//��ȡ����
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//cbGoodCard��������Ѿ��Ǻ���������,�������������˼���
		//�ַ��˿�
		m_cbHandCardCount[ wHaveGoodCardAndroidUser ] = NORMAL_COUNT;
		CopyMemory(&m_cbHandCardData[ wHaveGoodCardAndroidUser ], cbGoodCard, NORMAL_COUNT ) ;
		

		//ɾ����������
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, cbRandCard, FULL_COUNT ) ;
		
		//�����û��˿�����
		for ( WORD i = 0, j = 0; i < GAME_PLAYER; i++ )
		{
			//�����ǰ�û����Ǻ��ƻ����˼����˿����ݸ���
			if ( i != wHaveGoodCardAndroidUser )
			{
				//�����û��˿����ݳ���
				m_cbHandCardCount[ i ] = NORMAL_COUNT;
				//�����˿����ݸ��û�
				CopyMemory( &m_cbHandCardData[ i ],&cbRandCard[j*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
				++j ;
			}
		}

			//��Ϊ�����û��˿��Ѿ�ɾ��,�˿�������ֻ��37����2*NORMAL_COUNT=34 ȡ��ǰ����,3���ַ�
			//���õ���
			CopyMemory(m_cbBankerCard,&cbRandCard[2*NORMAL_COUNT],sizeof(m_cbBankerCard));
			//���е���
			m_GameLogic.SortCardList(m_cbBankerCard,sizeof(m_cbBankerCard),ST_ORDER);

			//���ñ���
			WinCharID=wHaveGoodCardAndroidUser;

	}else{
		
			////////////////////////////////������!����Ӧ�����ͷָ����˵ĲŶԵ�
			////�γ�����
			////�����û��˿�����
			//�û��˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHandCardCount[i]=NORMAL_COUNT;
				CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);

			}
			//���õ���
			CopyMemory(m_cbBankerCard,&cbRandCard[DISPATCH_COUNT],sizeof(m_cbBankerCard));
			//���е���
			m_GameLogic.SortCardList(m_cbBankerCard,sizeof(m_cbBankerCard),ST_ORDER);
			/////���������˿�
	}

	
	///�γ�������� //���ͺ��ƽ���
	///////////////////////////////////////////////////////////////////////
	////////////��������

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],ST_ORDER);
	}
	//����������
	CMD_S_AndroidCard AndroidCard ;
	ZeroMemory(&AndroidCard, sizeof(AndroidCard)) ;
	//���ͻ����˵�������
	CopyMemory(AndroidCard.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));
	//���ͻ������û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHandCardCount[i]=NORMAL_COUNT;
		//����������
		CopyMemory(&AndroidCard.cbHandCard[i], &m_cbHandCardData[i], sizeof(BYTE)*m_cbHandCardCount[i]) ;
		//�����û�ID
		AndroidCard.wAndroidUserID=WinCharID;
	}
	

	//�����û�
	m_wFirstUser=wCurrentUser;
	m_wCurrentUser=wCurrentUser;

	//�������
	CMD_S_GameStart GameStart;
	GameStart.wStartUser=wStartUser;
	GameStart.wCurrentUser=wCurrentUser;
	AndroidCard.wCurrentUser = m_wCurrentUser;

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�����˿�
		ASSERT(CountArray(GameStart.cbCardData)>=m_cbHandCardCount[i]);
		CopyMemory(GameStart.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//��������
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i) ;
		if(!pServerUserItem->IsAndroidUser())
		{
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		else 
		{
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&AndroidCard,sizeof(AndroidCard));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
	}

	for(WORD i = 0; i < GAME_PLAYER ;i++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;
		
		if(pIServerUserItem)
		{
			//�����û�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			//if(!pIServerUserItem->IsAndroidUser())
			{
				OnCheatCard(i);
			}
		}
	}

	if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
	{
		m_pITableFrame->SetGameTimer(IDI_CHECK_TABLE,30000,-1,NULL);
		m_cbWaitTime=0;
	}

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
		m_pITableFrame->KillGameTimer(IDI_CHECK_TABLE);
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//���ñ���
			GameConclude.cbBankerScore=m_cbBankerScore;
			GameConclude.lCellScore=m_pITableFrame->GetCellScore();

			//ը����Ϣ
			GameConclude.cbBombCount=m_cbBombCount;
			CopyMemory(GameConclude.cbEachBombCount,m_cbEachBombCount,sizeof(GameConclude.cbEachBombCount));

			//�û��˿�
			BYTE cbCardIndex=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�����˿�
				GameConclude.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameConclude.cbHandCardData[cbCardIndex],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//��������
				cbCardIndex+=m_cbHandCardCount[i];
			}

			//ը��ͳ��
			LONG lScoreTimes=1L * m_cbBankerScore;
			for (BYTE i=0;i<m_cbBombCount;i++) lScoreTimes*=2L;

			//�����ж�
			if (wChairID==m_wBankerUser)
			{
				//�û�����
				WORD wUser1=(m_wBankerUser+1)%GAME_PLAYER;
				WORD wUser2=(m_wBankerUser+2)%GAME_PLAYER;

				//�û��ж�
				if ((m_cbOutCardCount[wUser1]==0)&&(m_cbOutCardCount[wUser2]==0)) 
				{
					lScoreTimes*=2L;
					GameConclude.bChunTian=TRUE;
				}
			}

			//�����ж�
			if (wChairID!=m_wBankerUser)
			{
				if (m_cbOutCardCount[m_wBankerUser]==1)
				{
					lScoreTimes*=2L;
					GameConclude.bFanChunTian=TRUE;
				}
			}

			//��������
			lScoreTimes=__min(m_pGameCustomRule->wMaxScoreTimes,lScoreTimes);

			//���ֱ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				SCORE lUserScore=0L;
				SCORE lCellScore=m_pITableFrame->GetCellScore();

				//���ֻ���
				if (i==m_wBankerUser)
				{
					lUserScore=(m_cbHandCardCount[m_wBankerUser]==0)?2L:-2L;
				}
				else
				{
					lUserScore=(m_cbHandCardCount[m_wBankerUser]==0)?-1L:1L;
				}

				//�������
				ScoreInfoArray[i].lScore=lUserScore*lCellScore*lScoreTimes;
				ScoreInfoArray[i].cbType=(ScoreInfoArray[i].lScore>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				//����˰��
				if ( ScoreInfoArray[i].lScore > 0.0 )
				{
				//����˰��
				ScoreInfoArray[i].lRevenue=m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
				if (ScoreInfoArray[i].lRevenue>0L) ScoreInfoArray[i].lScore-=ScoreInfoArray[i].lRevenue;
				}
				//���û���
				GameConclude.lGameScore[i]=ScoreInfoArray[i].lScore;

				//��ʷ����
				m_HistoryScore.OnEventUserScore(i,GameConclude.lGameScore[i]);
			}

			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//�л��û�
			m_wFirstUser=wChairID;

			


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
						if(ScoreInfoArray[i].lScore>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//������˥��
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//ϵͳ���ֵ
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore,2,1)+lStorageDeduct); 
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
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
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

			
			//������
			ReadConfigInformation(true);

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);


			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//�û��˿�
			BYTE cbCardIndex=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�����˿�
				GameConclude.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameConclude.cbHandCardData[cbCardIndex],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//��������
				cbCardIndex+=m_cbHandCardCount[i];
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);

			return true;
		}
	case GER_USER_LEAVE:	//�û�ǿ��
	case GER_NETWORK_ERROR:	//�����ж�
		{
			//��������
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//���ñ���
			GameConclude.cbBankerScore=m_cbBankerScore;
			GameConclude.lCellScore=m_pITableFrame->GetCellScore();

			//ը����Ϣ
			GameConclude.cbBombCount=m_cbBombCount;
			CopyMemory(GameConclude.cbEachBombCount,m_cbEachBombCount,sizeof(GameConclude.cbEachBombCount));

			//�û��˿�
			BYTE cbCardIndex=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�����˿�
				GameConclude.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameConclude.cbHandCardData[cbCardIndex],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//��������
				cbCardIndex+=m_cbHandCardCount[i];
			}

			//ը��ͳ��
			WORD lScoreTimes=1;
			for (WORD i=0;i<m_cbBombCount;i++) lScoreTimes*=2L;

			//��������
			lScoreTimes=__min(m_pGameCustomRule->wMaxScoreTimes,lScoreTimes);

			//���ֱ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			//��������
			SCORE lCellScore=m_pITableFrame->GetCellScore();
			SCORE lUserScore=lCellScore*__max(lScoreTimes,m_pGameCustomRule->wFleeScoreTimes);

			//���ƽ��
			if ((m_pGameServiceOption->wServerType&SCORE_GENRE_POSITIVE)!=0 || (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0)
			{
				lUserScore=__min(pIServerUserItem->GetUserScore(),lUserScore);
			}

			//���ܷ���
			ScoreInfoArray[wChairID].lScore=-lUserScore;
			ScoreInfoArray[wChairID].cbType=SCORE_TYPE_FLEE;

			//������
			if (m_pGameCustomRule->cbFleeScorePatch==TRUE)
			{
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//����
					if ( i == wChairID )
						continue;

					//���û���
					ScoreInfoArray[i].lScore=lUserScore/2L;
					ScoreInfoArray[i].cbType=SCORE_TYPE_WIN;

					//����˰��
					ScoreInfoArray[i].lRevenue=m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
					if (ScoreInfoArray[i].lRevenue>0L) ScoreInfoArray[i].lScore-=ScoreInfoArray[i].lRevenue;
				}
			}

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//���óɼ�
				GameConclude.lGameScore[i]=ScoreInfoArray[i].lScore;

				//��ʷ�ɼ�
				m_HistoryScore.OnEventUserScore(i,GameConclude.lGameScore[i]);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));



			


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
						if(ScoreInfoArray[i].lScore>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//������˥��
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//ϵͳ���ֵ
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore,2,1)+lStorageDeduct); 
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
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
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
									//������
			ReadConfigInformation(true);

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);




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
			StatusFree.lCellScore=m_pITableFrame->GetCellScore();

			//�Զ�����
			StatusFree.cbTimeOutCard=m_pGameCustomRule->cbTimeOutCard;		
			StatusFree.cbTimeCallScore=m_pGameCustomRule->cbTimeCallScore;
			StatusFree.cbTimeStartGame=m_pGameCustomRule->cbTimeStartGame;
			StatusFree.cbTimeHeadOutCard=m_pGameCustomRule->cbTimeHeadOutCard;

						//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���ñ���
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_CALL:	//�з�״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//��Ԫ����
			StatusCall.lCellScore=m_pITableFrame->GetCellScore();

			//�Զ�����
			StatusCall.cbTimeOutCard=m_pGameCustomRule->cbTimeOutCard;		
			StatusCall.cbTimeCallScore=m_pGameCustomRule->cbTimeCallScore;
			StatusCall.cbTimeStartGame=m_pGameCustomRule->cbTimeStartGame;
			StatusCall.cbTimeHeadOutCard=m_pGameCustomRule->cbTimeHeadOutCard;

			//��Ϸ��Ϣ
			StatusCall.wCurrentUser=m_wCurrentUser;
			StatusCall.cbBankerScore=m_cbBankerScore;
			CopyMemory(StatusCall.cbScoreInfo,m_cbScoreInfo,sizeof(m_cbScoreInfo));
			CopyMemory(StatusCall.cbHandCardData,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID]*sizeof(BYTE));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���ñ���
				StatusCall.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusCall.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			bool bSendResult = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));

			if(pIServerUserItem)
			{
				//�����û�
				if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
				{
					OnCheatCard(wChairID);
				}
			}
			return bSendResult;
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��Ԫ����
			StatusPlay.lCellScore=m_pITableFrame->GetCellScore();

			//��Ϸ����
			StatusPlay.cbBombCount=m_cbBombCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.cbBankerScore=m_cbBankerScore;

			//�Զ�����
			StatusPlay.cbTimeOutCard=m_pGameCustomRule->cbTimeOutCard;		
			StatusPlay.cbTimeCallScore=m_pGameCustomRule->cbTimeCallScore;
			StatusPlay.cbTimeStartGame=m_pGameCustomRule->cbTimeStartGame;
			StatusPlay.cbTimeHeadOutCard=m_pGameCustomRule->cbTimeHeadOutCard;

			//������Ϣ
			StatusPlay.wTurnWiner=m_wTurnWiner;
			StatusPlay.cbTurnCardCount=m_cbTurnCardCount;
			CopyMemory(StatusPlay.cbTurnCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿���Ϣ
			CopyMemory(StatusPlay.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));
			CopyMemory(StatusPlay.cbHandCardCount,m_cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(StatusPlay.cbHandCardData,m_cbHandCardData[wChairID],sizeof(BYTE)*m_cbHandCardCount[wChairID]);

									//��������
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���ñ���
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			bool bSendResult = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			if(pIServerUserItem)
			{
				//�����û�
				if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
				{
					OnCheatCard(wChairID);
				}
			}
			return bSendResult;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
	{
		if(wTimerID==IDI_CHECK_TABLE)
		{
			m_cbWaitTime++;
			if(m_cbWaitTime>=3)
			{
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS);
			}
			else if(m_cbWaitTime>=2)
			{
				for(int i=0;i<GAME_PLAYER;i++)
				{
					IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(i);
					if(pServerUserItem!=NULL && pServerUserItem->GetUserStatus()==US_OFFLINE)return true;				
				}
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS);
			}
			return true;
		}
	}
	return false;
}

//�����¼�
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	m_cbWaitTime=0;
	switch (wSubCmdID)
	{
	case SUB_C_CALL_SCORE:	//�û��з�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallScore));
			if (wDataSize!=sizeof(CMD_C_CallScore)) return false;

			//״̬Ч��
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_CALL);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_CALL) return true;

			//�û�Ч��
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��������
			CMD_C_CallScore * pCallScore=(CMD_C_CallScore *)pData;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserCallScore(wChairID,pCallScore->cbCallScore);
		}
	case SUB_C_OUT_CARD:	//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE)))) return false;

			//״̬Ч��
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserOutCard(wChairID,pOutCard->cbCardData,pOutCard->cbCardCount);
		}
	case SUB_C_PASS_CARD:	//�û�����
		{
			//״̬Ч��
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserPassCard(pIServerUserItem->GetChairID());
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
	if (bLookonUser==false)
	{
		ASSERT(wChairID!=INVALID_CHAIR);
		m_HistoryScore.OnEventUserEnter(wChairID);
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false)
	{
		ASSERT(wChairID!=INVALID_CHAIR);
		m_HistoryScore.OnEventUserLeave(wChairID);
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbTurnCardCount!=0));
	if ((wChairID!=m_wCurrentUser)||(m_cbTurnCardCount==0)) return false;

	//���ñ���
	m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
	if (m_wCurrentUser==m_wTurnWiner) m_cbTurnCardCount=0;

	//������Ϣ
	CMD_S_PassCard PassCard;
	PassCard.wPassCardUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.cbTurnOver=(m_cbTurnCardCount==0)?TRUE:FALSE;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//�û��з�
bool CTableFrameSink::OnUserCallScore(WORD wChairID, BYTE cbCallScore)
{
	//Ч��״̬
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return false;

	//Ч�����
	//ASSERT(((cbCallScore>=1)&&(cbCallScore<=3)&&(cbCallScore>m_cbBankerScore))||(cbCallScore==255));
	if (((cbCallScore<1)||(cbCallScore>3)||(cbCallScore<=m_cbBankerScore))&&(cbCallScore!=255))
		cbCallScore = 255;

	//����״̬
	if (cbCallScore!=0xFF)
	{
		m_cbBankerScore=cbCallScore;
		m_wBankerUser=m_wCurrentUser;
	}

	//���ýз�
	m_cbScoreInfo[wChairID]=cbCallScore;

	//�����û�
	if ((m_cbBankerScore==3)||(m_wFirstUser==(wChairID+1)%GAME_PLAYER))
	{
		m_wCurrentUser=INVALID_CHAIR;
	}
	else
	{
		m_wCurrentUser=(wChairID+1)%GAME_PLAYER;
	}

	//�������
	CMD_S_CallScore CallScore;
	CallScore.wCallScoreUser=wChairID;
	CallScore.wCurrentUser=m_wCurrentUser;
	CallScore.cbUserCallScore=cbCallScore;
	CallScore.cbCurrentScore=m_cbBankerScore;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_SCORE,&CallScore,sizeof(CallScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_SCORE,&CallScore,sizeof(CallScore));

	//��ʼ�ж�
	if ((m_cbBankerScore==3)||(m_wFirstUser==(wChairID+1)%GAME_PLAYER))
	{
		//���˽з�
		if ( m_cbBankerScore == 0 )
		{
			return OnEventGameStart();
			//m_wBankerUser=m_wFirstUser;
			//m_cbBankerScore=1;
		}

		//����״̬
		m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

		//���ñ���
		if (m_cbBankerScore==0) m_cbBankerScore=1;
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

		//���͵���
		m_cbHandCardCount[m_wBankerUser]+=CountArray(m_cbBankerCard);
		CopyMemory(&m_cbHandCardData[m_wBankerUser][NORMAL_COUNT],m_cbBankerCard,sizeof(m_cbBankerCard));

		//�����˿�
		m_GameLogic.SortCardList(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser],ST_ORDER);


		//�����û�
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=m_wBankerUser;

		//������Ϣ
		CMD_S_BankerInfo BankerInfo;
		BankerInfo.wBankerUser=m_wBankerUser;
		BankerInfo.wCurrentUser=m_wCurrentUser;
		BankerInfo.cbBankerScore=m_cbBankerScore;
		CopyMemory(BankerInfo.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BANKER_INFO,&BankerInfo,sizeof(BankerInfo));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BANKER_INFO,&BankerInfo,sizeof(BankerInfo));

		return true;

	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч��״̬
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return false;

	//��ȡ����
	BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);

	//�����ж�
	if (cbCardType==CT_ERROR) 
	{
		ASSERT(FALSE);
		return false;
	}

	//�����ж�
	if (m_cbTurnCardCount!=0)
	{
		//�Ա��˿�
		if (m_GameLogic.CompareCard(m_cbTurnCardData,cbCardData,m_cbTurnCardCount,cbCardCount)==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCardList(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���Ʊ���
	m_cbOutCardCount[wChairID]++;

	//���ñ���
	m_cbTurnCardCount=cbCardCount;
	m_cbHandCardCount[wChairID]-=cbCardCount;
	CopyMemory(m_cbTurnCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//ը���ж�
	if ((cbCardType==CT_BOMB_CARD)||(cbCardType==CT_MISSILE_CARD)) 
	{
		m_cbBombCount++;
		m_cbEachBombCount[wChairID]++;
	}
	
	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_cbHandCardCount[wChairID]!=0)
	{
		if (cbCardType!=CT_MISSILE_CARD)
		{
			m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
		}
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

	//��������
	WORD wHeadSize=sizeof(OutCard)-sizeof(OutCard.cbCardData);
	WORD wSendSize=wHeadSize+OutCard.cbCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�������
	if (cbCardType==CT_MISSILE_CARD) m_cbTurnCardCount=0;

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(wChairID,NULL,GER_NORMAL);

	return true;
}

//�����û�
bool CTableFrameSink::OnCheatCard(WORD wChairID)
{
	//if(m_pServerControl)
	{
		CMD_S_CheatCard CheatCard ;
		ZeroMemory(&CheatCard, sizeof(CheatCard));

		for(WORD i = 0; i < GAME_PLAYER; i++)
		{
			CheatCard.wCardUser[CheatCard.cbUserCount] = i;
			CheatCard.cbCardCount[CheatCard.cbUserCount] = m_cbHandCardCount[i];
			CopyMemory(CheatCard.cbCardData[CheatCard.cbUserCount++],m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);
		}
		//��������
		//WB�� ����������������BUG
		//m_pITableFrame->SendTableData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
		//m_pITableFrame->SendLookonData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
		//m_pServerControl->ServerControl(&CheatCard, m_pITableFrame, pIServerUserItem);

			//��������
		m_pITableFrame->SendTableData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
		m_pITableFrame->SendLookonData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
	}
	
	return true;
}


//����������Ϣ
//void CTableFrameSink::SendCheatCard(WORD wChairID)
//{
	//if(m_pServerControl)
	//{
//		CMD_S_CheatCard CheatCard ;
//		ZeroMemory(&CheatCard, sizeof(CheatCard));
//
//		for(WORD i = 0; i < GAME_PLAYER; i++)
//		{
//			CheatCard.wCardUser[CheatCard.cbUserCount] = i;
//			CheatCard.cbCardCount[CheatCard.cbUserCount] = m_cbHandCardCount[i];
//			CopyMemory(CheatCard.cbCardData[CheatCard.cbUserCount++],m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);
//		}
		//m_pServerControl->ServerControl(&CheatCard, m_pITableFrame);
			//��������
//		m_pITableFrame->SendTableData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
//		m_pITableFrame->SendLookonData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
	//}
//}


//���û���
void CTableFrameSink::SetGameBaseScore(SCORE lBaseScore)
{
	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SET_BASESCORE,&lBaseScore,sizeof(lBaseScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SET_BASESCORE,&lBaseScore,sizeof(lBaseScore));
}
//////////////////////////////////////////////////////////////////////////////////


//�����û�Id��ȡ������������Id
WORD CTableFrameSink::GetChairIdByGameId(DWORD dwGameId)
{
	WORD wChairID;
	for (wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue; //�û�������

		if( pIServerUserItem->GetUserID() == dwGameId)
		{
			return wChairID;
		}
	}

	return INVALID_CHAIR;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//��ȡ����
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR OutBuf[255];
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);
	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\LandConfig.ini"),szPath);
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
		CString TempValue;
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
		m_lStockScore=Mywtof(TempValue);
	}


	//����˥��ֵ
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageDeduct);


		//�γ��������
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
		if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);


	//ϵͳ����Ӯ��
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageLimit"),TEXT("0.0"),OutBuf,255,szFileName);
	
	//////////////////////////////////
	//�γ������ȡС������ֵ
	CString TempValue;
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
	m_lStockLimit=Mywtof(TempValue);
	/////////////////////////////////

	//�γ�������� //����ǰ̨����
	//��Ҫʤ�������UserID
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("WinnerUserID"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%lu"),&m_dwCheatGameID);
	//��ȡ���ƴ���
	m_dwCheatCount = GetPrivateProfileInt(szServerName, TEXT("CheatCount"), 0, szFileName);
	/////////////////


}