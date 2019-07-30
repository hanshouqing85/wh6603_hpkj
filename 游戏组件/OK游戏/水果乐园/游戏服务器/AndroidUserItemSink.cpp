#include "Stdafx.h"
#include "AndroidUserItemSink.h"

////////////////////////////////////////////////////////////////////////////////
#define IDI_GAME_ACTION				105
////////////////////////////////////////////////////////////////////////////////

static const LONGLONG lScoreArray[]={100L,1000L,10000L};
int CAndroidUserItemSink::m_stlApplyBanker=0L;

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
//AllocConsole();
//freopen("CONOUT$","w+t",stdout);
//freopen("CONIN$","r+t",stdin);
	m_lAreaLimitScore=0L;
	m_lUserLimitScore=0L;
	m_lApplyBankerCondition=0L;
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	m_lMeMaxScore=0L;
	m_lBankerScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_bEnableSysBanker=false;
	m_cbJettonArea=0;
	m_bMeCurrentBanker=false;
	m_bApplyingBanker=false;
	m_bCancelingBanker=false;
	m_wRandBankerTime=0;
	m_dwJettonRate=100;
	m_nJettonRange=99;
	return;
}

//����
bool CAndroidUserItemSink::RepositionSink()
{
	//m_bMeCurrentBanker=false;
	//m_bApplyingBanker=false;
	//m_bCancelingBanker=false;
	//ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	//ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	return true;
}
//���ýӿ�
bool  CAndroidUserItemSink::RepositUserItemSink()
{
	m_bMeCurrentBanker=false;
	m_bApplyingBanker=false;
	m_bCancelingBanker=false;
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (m_pIAndroidUserItem->GetGameStatus())
	{
	case GAME_STATUS_FREE://����������ׯ����������ׯ���߼���֧
		{
			if (m_bMeCurrentBanker==false && m_stlApplyBanker<m_nRobotApplyBanker)//����ׯ�ң��������㹻
			{
				if (m_bApplyingBanker==true)//�Ѿ����룬���������루�Ѿ�����Ļ����ˣ��Ѿ���������ׯ�б��ˣ�
				{
					return false;
				}
				if (m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()<m_lApplyBankerCondition)//��������
				{
					return false;
				}
				m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER,NULL,0);//������ׯ����
				m_stlApplyBanker++;//ռ��һ������
				m_bApplyingBanker=true;//��������ׯ
			}
			else if (m_bMeCurrentBanker==true)//����ׯ��
			{
				if (m_bCancelingBanker==true) return false;
				m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER,NULL,0);//������ׯ����
				m_bCancelingBanker=true;//��������ׯ
			}
			return true;
		}
	case GS_PLACE_JETTON://������ע���߼���֧
		{
			if (m_wJettonCount--==0) return false;//��ע��������
			//��������
			//int nMaxIndex=0;
			//for (int i=0; i<CountArray(lScoreArray); i++)
			//	if (m_lMeMaxScore>lScoreArray[i]) nMaxIndex=i+1;
			//if (nMaxIndex>m_nMaxJettonRange) nMaxIndex=m_nMaxJettonRange;
			//�����С���ڳ�������������Һ��ʵĳ���
			//LONGLONG lJettonScore=lScoreArray[rand()%nMaxIndex]*(rand()%9+1);
			LONGLONG lJettonScore;
			//ASSERT(lJettonScore<=m_lMeMaxScore);

			//int iCount = 0;
			//while (lJettonScore>m_lRobotJettonLimit[1] || lJettonScore<m_lRobotJettonLimit[0])
			//{
			//	lJettonScore=lScoreArray[rand()%nMaxIndex];
			//	iCount++;
			//	if (iCount>100) return true;
			//}

			//if (lJettonScore>GetUserMaxJetton()) return false;
			//printf("m_wJettonCount=%d,m_cbJettonArea=%d,lJettonScore=%I64d\n",m_wJettonCount,m_cbJettonArea,lJettonScore);
			//���ñ���
			BYTE cbAreaCount=rand()%5+4;//ÿ����ע4��8��
			for (int i=0; i<cbAreaCount; i++)
			{
				m_cbJettonArea=rand()%JETTON_AREA_COUNT;
				if (m_nJettonRange<1 || m_nJettonRange>999) m_nJettonRange=99;
				lJettonScore=(rand()%m_nJettonRange+1)*m_dwJettonRate;
				m_lUserJettonScore[m_cbJettonArea]+=lJettonScore;
				//������Ϣ
				CMD_C_PlaceJetton PlaceJetton;
				PlaceJetton.cbJettonArea=m_cbJettonArea;
				PlaceJetton.lJettonScore=lJettonScore;
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
			}
			//���ö�ʱ��
			UINT nElapse=rand()%2+2;
			m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION,nElapse);
			return true;
		}
	case GS_GAME_END:
		{
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ(OK)
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
//printf("wSubCmdID=%d\n",wSubCmdID);
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE://��Ϸ����
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START://��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON://�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END://��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER://������ׯ
		{
			return OnSubUserApplyBanker(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER://�л�ׯ��
		{
			return OnSubChangeBanker(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD://��Ϸ��¼
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL://��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER://ȡ����ׯ
		{
			return OnSubUserCancelBanker(pBuffer,wDataSize);
		}
	case SUB_S_BIG_SMALL://�´�С
		{
			return true;
		}
	case SUB_S_CHIP_UPDATE:
		{
			return true;
		}
	}
	ASSERT(FALSE);
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch(cbGameStatus)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			////////////////////m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_wBankerUser=pStatusFree->wBankerUser;
			m_wBankerTime=pStatusFree->wBankerTime;
			m_lBankerScore=pStatusFree->lBankerScore;
			m_lBankerWinScore=pStatusFree->lBankerWinScore;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;

			if (bLookonOther==false)
			{
				m_lMeMaxScore=pStatusFree->lUserMaxScore;
			}
			memcpy(m_szRoomName,pStatusFree->szRoomName,sizeof(m_szRoomName));
			m_dwJettonRate=pStatusFree->dwChipRate;

			ReadConfigInformation(true);
			return true;

		}
	case GS_PLACE_JETTON:			//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			////////////////////m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wBankerTime=pStatusPlay->cbBankerTime;
			m_lBankerScore=pStatusPlay->lBankerScore;
			m_lBankerWinScore=pStatusPlay->lBankerWinScore;
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;

			CopyMemory(m_lAllJettonScore,pStatusPlay->lTotalJettonScore,sizeof(m_lAllJettonScore));

			if (bLookonOther==false)
			{
				m_lMeMaxScore=pStatusPlay->lUserMaxScore;
				CopyMemory(m_lUserJettonScore,pStatusPlay->lUserAreaScore,sizeof(m_lUserJettonScore));
			}

			memcpy(m_szRoomName,pStatusPlay->szRoomName,sizeof(m_szRoomName));
			m_dwJettonRate=pStatusPlay->dwChipRate;

			ReadConfigInformation(true);

			return true;
		}
	}
	return true;
}

//��Ϸ����(OK)��ѡ���Եģ�������ׯ����������ׯ
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;
	//��ȡ�����ļ�
	ReadConfigInformation(false);

	m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_FREE);

	IServerUserItem *pServerUserItem=m_pIAndroidUserItem->GetMeUserItem();
	ASSERT(pServerUserItem!=NULL);
	if (m_wBankerUser==INVALID_CHAIR||m_wBankerUser!=pServerUserItem->GetChairID())
		m_bMeCurrentBanker=false;
	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	m_lApplyBankerCondition=pGameFree->lApplyBankerCondition;
	//����Ҫ����������ͬ��
	if (m_stlApplyBanker!=pGameFree->wApplyUserCount)
	{
		m_stlApplyBanker=pGameFree->wApplyUserCount;
	}
	//
	if (pServerUserItem->GetUserStatus()!=US_LOOKON)
	{
		//��ׯ������δ���롢�Ҳ���ׯ�ҡ������������ׯ�������㹻
		if (m_bApplyingBanker==false && m_bMeCurrentBanker==false && m_bAllowApplyBanker==TRUE && pServerUserItem->GetUserScore()>m_lApplyBankerCondition)
		{
			m_wRandBankerTime=rand()%m_wMaxBankerTime+1;
			if(m_wRandBankerTime<3) m_wRandBankerTime=3;//������Ҫ��ׯ3������

			UINT nElapse=rand()%3+1;
			m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION, nElapse);
		}
		else if (m_bMeCurrentBanker==true && m_bCancelingBanker==false)//��ׯ����
		{
			if (m_wBankerTime>=m_wRandBankerTime)
			{
				UINT nElapse=rand()%3+1;
				m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION, nElapse);
			}
		}
	}
	return true;
}

//��Ϸ��ʼ(OK)����Ϸ��ʼ�����Խ�����ע��������ע�����Լ���ע������Ȼ��ȥ��CAndroidUserItemSink::OnEventTimer()��GS_PLACE_JETTON��֧
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	//���ñ���
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_wBankerUser=pGameStart->wBankerUser;
	m_lBankerScore=pGameStart->lBankerScore;
	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_PLACE_JETTON);
	//��ע����
	if (m_lMeMaxScore>2000L && m_wBankerUser!=m_pIAndroidUserItem->GetChairID() /*m_wBankerUser!=INVALID_CHAIR &&*/)
	{
		m_wJettonCount=rand()%10+3;
		//��ʱ��
		UINT nElapse=rand()%2+1;
		m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION,nElapse);
	}
	return true;
}

//�û���ע(OK)���õ��û���ע����Ϣ�����롰��ע��
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;
	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	//У������
	ASSERT(pPlaceJetton->cbJettonArea<=ID_APPLE && pPlaceJetton->cbJettonArea>=ID_BAR);//����У��
	ASSERT(pPlaceJetton->wChairID!=INVALID_CHAIR);//���У��
	//ASSERT(pPlaceJetton->lJettonScore>=lScoreArray[0] && pPlaceJetton->lJettonScore<=lScoreArray[3]);//����У��
	//���ñ���
	//m_lAllJettonScore[pPlaceJetton->cbJettonArea]+=pPlaceJetton->lJettonScore;
	CopyMemory(m_lAllJettonScore,pPlaceJetton->lTotalJettonScore,sizeof(m_lAllJettonScore));
	return true;
}

//��עʧ��(OK)������Ϣ�ǵ����������ж��Ƿ����Լ���ȡ������ע��ɾ����ʱ��
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;
	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	//У������
	ASSERT(pPlaceJettonFail->cbJettonArea<=ID_APPLE && pPlaceJettonFail->cbJettonArea>=ID_BAR);
	//���ñ���
	m_lUserJettonScore[pPlaceJettonFail->cbJettonArea]-=pPlaceJettonFail->lJettonScore;
	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_GAME_ACTION);
	return true;
}

//��Ϸ����(OK)
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	//���ñ���
	m_lBankerScore=pGameEnd->lBankerScore;
	m_wBankerTime=(WORD)pGameEnd->iBankerTime;
	m_lApplyBankerCondition=pGameEnd->lApplyBankerCondition;
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	//��Ϸ״̬
	m_pIAndroidUserItem->SetGameStatus(GS_GAME_END);
	return true;
}

//������ׯ(OK)
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;
	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;
	return true;
}

//ȡ����ׯ(OK)��ȡ�����룬���á�ͬ��ֵ��
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;
	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;
	//
	if (lstrcmp(m_pIAndroidUserItem->GetMeUserItem()->GetNickName(),pCancelBanker->szCancelUser)==0)
	{
		m_stlApplyBanker--;
		m_bApplyingBanker=false;
	}
	return true;
}

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	if (m_wBankerUser==wMeChairID && pChangeBanker->wBankerUser!=wMeChairID)//��������ׯ
	{
		m_bMeCurrentBanker=false;
		m_bCancelingBanker=false;
		m_stlApplyBanker--;
		m_wRandBankerTime=0;
	}
	else if (pChangeBanker->wBankerUser==wMeChairID)//��������ׯ
	{
		m_bMeCurrentBanker=true;
		m_bApplyingBanker=false;
	}

	m_wBankerUser=pChangeBanker->wBankerUser;
	m_lBankerScore=pChangeBanker->lBankerScore;

	return true;
}

//�����ע
LONGLONG CAndroidUserItemSink::GetUserMaxJetton()
{
	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(JETTON_AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=JETTON_AREA_COUNT; ++nAreaIndex) 
		lNowJetton += m_lUserJettonScore[nAreaIndex];

	//ׯ�ҽ��
	LONGLONG lBankerScore=2147483647;
	if (m_wBankerUser!=INVALID_CHAIR) 
		lBankerScore=m_lBankerScore;

	for (int nAreaIndex=1; nAreaIndex<=JETTON_AREA_COUNT; ++nAreaIndex)
		lBankerScore-=m_lAllJettonScore[nAreaIndex];

	//��������
	LONGLONG lMeMaxScore=min(m_lMeMaxScore-lNowJetton,m_lAreaLimitScore);

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	//��������
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//��ȡ�����ļ�
void CAndroidUserItemSink::ReadConfigInformation(bool bFirstRead)
{
	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	mysprintf(m_szConfigFileName,sizeof(m_szConfigFileName),_T("%s\\SGDZ.ini"),szPath);
	//��������
	TCHAR OutBuf[255] = TEXT("");
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotMaxJetton"), _T("5000000"), OutBuf, 255, m_szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[1]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotMinJetton"), _T("100"), OutBuf, 255, m_szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[0]);
	//�Ƿ�ÿ��ˢ��
	if(!bFirstRead && GetPrivateProfileInt(m_szRoomName, TEXT("Refresh"),0, m_szConfigFileName)==0) return;
	//��������ע��Χ
	m_nJettonRange=GetPrivateProfileInt(m_szRoomName,TEXT("JettonRange"),99,m_szConfigFileName);
	if (m_nJettonRange<1 || m_nJettonRange>999) m_nJettonRange=99;
	//�����������ׯ����
	m_wMaxBankerTime=(WORD)GetPrivateProfileInt(m_szRoomName,TEXT("RobotBankerCount"),3,m_szConfigFileName);
	//�������ܷ�������ׯ
	m_bAllowApplyBanker=GetPrivateProfileInt(m_szRoomName,TEXT("RobotBanker"),1,m_szConfigFileName);
	//�����������ׯ����
	m_nRobotApplyBanker=GetPrivateProfileInt(m_szRoomName, TEXT("RobotApplyBanker"),5,m_szConfigFileName);
}

////////////////////////////////////////////////////////////////////////////////
//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}
//��ʼ�ӿ�
bool  CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;
	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//�û�����
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

////������Ҷ���ʼ��
//void CAndroidUserItemSink::OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
//{
//	return ;
//}
////////////////////////////////////////////////////////////////////////////////
