#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
//����ʱ��
#define TIME_LESS					2									//����ʱ��
//��ʱ����ʶ
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_USER_ADD_SCORE			201									//��ע��ʱ��

//ʱ���ʶ
#define TIME_START_GAME				10	/*20*/								//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			10	/*20*/								//������ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	ZeroMemory(m_bUserName,sizeof(m_bUserName));
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_bOpenCard = false;
	m_bExitTag	= false;

	//��ע��Ϣ
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//�˿˱���
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbOverCardData,sizeof(m_cbOverCardData));

	//״̬����
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	m_bAutoStart = FALSE;
	m_bReset =true;
	//�ӿڱ���
	m_pIAndroidUserItem=NULL;

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}
  //��ʼ�ӿ�
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}
//���ýӿ�
bool  CAndroidUserItemSink::RepositionSink()
{
	ZeroMemory(m_bUserName,sizeof(m_bUserName));
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_bOpenCard = false;
	m_bExitTag	= false;

	//��ע��Ϣ
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//�˿˱���
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbOverCardData,sizeof(m_cbOverCardData));

	//״̬����
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	m_bAutoStart = FALSE;
	m_bReset =true;
	return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:
		{
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_USER_ADD_SCORE:
		{
			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.lScore=m_lTurnLessScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;
		}
	default:
		{
			break;
		}
	}

	return false;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//�û���ע
		{
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return true;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{

	return true;
}

//������Ϣ
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//�������
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	}
	return false;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void  CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//���ñ���
	m_wDUser = pGameStart->wDUser;
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lAddLessScore = pGameStart->lAddLessScore;
	m_lTurnLessScore = pGameStart->lTurnLessScore;
	m_lTurnMaxScore = pGameStart->lTurnMaxScore;
	m_lCellScore = pGameStart->lCellScore;

	//��ע��Ϣ
	m_lTableScore[pGameStart->wDUser] += m_lCellScore;
	m_lTableScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;
	m_lTotalScore[pGameStart->wDUser] =  m_lCellScore;
	m_lTotalScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;

	//���Ͱ���
	CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(m_cbHandCardData));
	m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,rand()%TIME_USER_ADD_SCORE);

	return true;
}

//��ע��Ϣ
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//��������
	//WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;

	//���ñ���
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;
	m_lTurnMaxScore = pAddScore->lTurnMaxScore;
	m_lAddLessScore = pAddScore->lAddLessScore;

	m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,rand()%TIME_USER_ADD_SCORE+5);
	
	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;

	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;

	if(m_lTableScore[wGiveUpUser]!=0L)
	{
		m_lTableScore[wGiveUpUser] = 0L;
	}

	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//У������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//��ǰ���
	m_wCurrentUser = pSendCard->wCurrentUser;
	CopyMemory(m_cbCenterCardData,pSendCard->cbCenterCardData,sizeof(BYTE)*(pSendCard->cbSendCardCount));

	if((pSendCard->cbSendCardCount < 3))
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,rand()%TIME_START_GAME+5);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,rand()%TIME_START_GAME+5);
	
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////
