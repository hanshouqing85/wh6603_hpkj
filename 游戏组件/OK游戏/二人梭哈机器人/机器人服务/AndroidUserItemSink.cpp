#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
//����ʱ��
#define TIME_LESS					1									//����ʱ��
#define TIME_JETTON				    2									//��עʱ��
#define TIME_RAND				    2									//���ʱ��

#define IDI_ANDROID_ITEM_SINK			500									//��ʼ��ʶ
//��Ϸʱ��
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			    // ��Ϸ��ʼID
#define IDI_PLACE_JETTON        (IDI_ANDROID_ITEM_SINK+1)               // ��ע�׶�
#define IDI_SHOW_HAND_TIME      (IDI_ANDROID_ITEM_SINK+2)               // ����ӳ�
#define IDI_LOOK_CARD			(IDI_ANDROID_ITEM_SINK+3)               // �����˿���
#define IDI_GIVE_UP				(IDI_ANDROID_ITEM_SINK+4)               // ����
extern void OutputDebugMsg(const char * szMsg, ...);

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_bPoor = true;
	m_lTurnMaxGold=0L;						            // �����ע
	m_lTurnBasicGold=0L;					            // ��ע��Ŀ
	m_lBasicGold=0L;						            // ��Ԫ��Ŀ	
	m_wCurrentUser=INVALID_CHAIR;						// ��ǰ��ע��
	m_lShowHandScore=0L;					            // ������߷�
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));	// ��Ϸ״̬
	m_bShowHand = false;                                // �Ƿ����
	m_byCardCount = 0;
	m_lMeTableGold=0L;				                    // �Լ����µ�����
	m_lMeTurnGold=0L;				                    // �Լ�һ�����µ�����
	m_lMeScore=0L;                                      // �Լ�Я��������
	m_bCtrlWinner = false;                              // �Լ��Ƿ�ΪӮ��
	m_bFirstUser=false;                                 // �Ƿ�Ϊ��һ����˵��
	m_bWinner = false;

	//�����ļ���
// 	TCHAR szPath[MAX_PATH]=TEXT("");
// 	GetCurrentDirectory(sizeof(szPath),szPath);
// 	myprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SH_Config.ini"),szPath);
	iTIME_RAND=2;iTIME_LESS=1;

	memset(m_cbHumanCard, 0, sizeof(m_cbHumanCard));
	memset(m_cbOwnCard, 0, sizeof(m_cbOwnCard));
	
	m_bAddGold=false;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	m_lTurnMaxGold=0L;						            // �����ע
	m_lTurnBasicGold=0L;					            // ��ע��Ŀ
	m_lBasicGold=0L;						            // ��Ԫ��Ŀ	
	m_wCurrentUser=INVALID_CHAIR;						// ��ǰ��ע��
	m_lShowHandScore=0L;					            // ������߷�
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));	// ��Ϸ״̬
	m_bShowHand = false;                                // �Ƿ����
	m_byCardCount = 0;
	m_lMeTableGold=0L;				                    // �Լ����µ�����
	m_lMeTurnGold=0L;				                    // �Լ�һ�����µ�����
	m_lMeScore=0L;                                      // �Լ�Я��������
	m_bCtrlWinner = false;                              // �Լ��Ƿ�ΪӮ��
	m_bFirstUser=false;                                 // �Ƿ�Ϊ��һ����˵��
	m_bAddGold=false;

	memset(m_cbHumanCard, 0, sizeof(m_cbHumanCard));
	memset(m_cbOwnCard, 0, sizeof(m_cbOwnCard));
	
	return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:
		{
			//��ʼ�ж�
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_PLACE_JETTON:	// ��עʱ�䵽��
		{
			return OnPlaceJetton();
		}
	case IDI_LOOK_CARD:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_LOOK_CARD);
			m_pIAndroidUserItem->SendSocketData(SUB_C_SHOW_CARD);
			return true;
		}
	case IDI_SHOW_HAND_TIME:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_SHOW_HAND_TIME);
			__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
			__int64 lGold = lShowHandScore-m_lMeTableGold;
			//��������
			CMD_C_AddGold AddPoint;
			AddPoint.bFllow = false;
			AddPoint.lGold=lGold;
			if (lGold>0)
			{
				m_bAddGold=true;
			}
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));
			return true;
		}
	case IDI_GIVE_UP:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_GIVE_UP);
			m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
			return true;
		}
		
	default: break;
	}
	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	       // ��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_GOLD:	//�û���ע
		{
			return OnSubAddGold(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_ANDROID_WINNER:
		{
			if (NULL == m_pIAndroidUserItem)
			{
				return true;
			}
			//Ч������
			if (wDataSize!=sizeof(CMD_S_AndroidWinner)) return false;
			CMD_S_AndroidWinner * pAndroidWinner=(CMD_S_AndroidWinner *)pBuffer;
			if (pAndroidWinner->wWinner == m_pIAndroidUserItem->GetChairID())
			{
				//TCHAR str[44];
				//sprintf(str,"wWinne=%d,GetChairID=%d",pAndroidWinner->wWinner,m_pIAndroidUserItem->GetChairID());
					//AfxMessageBox(str);
				m_bCtrlWinner = true;
			}
			
			return true;
		}
	case SUB_S_SHOW_CARD:
		{
			return true;
		}
	case SUB_S_SEND_HUMAN_CARD:
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_ShowHumanCard)) return false;
			CMD_S_ShowHumanCard* pShowUserCard = (CMD_S_ShowHumanCard*) pBuffer;
			memcpy(m_cbHumanCard, pShowUserCard->cbUserCard, pShowUserCard->cbCardCount);

			return true;
		}
	default:break;
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_TK_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lTurnMaxGold=0L;						            // �����ע
			m_lTurnBasicGold=0L;					            // ��ע��Ŀ
			m_lBasicGold=0L;						            // ��Ԫ��Ŀ	
			m_wCurrentUser=INVALID_CHAIR;						// ��ǰ��ע��
			m_lShowHandScore=0L;					            // ������߷�
			ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));	// ��Ϸ״̬
			m_bShowHand = false;                                // �Ƿ����
			m_byCardCount = 0;
			m_lMeTableGold=0L;				                    // �Լ����µ�����
			m_lMeTurnGold=0L;				                    // �Լ�һ�����µ�����
			m_lMeScore=0L;                                      // �Լ�Я��������
			m_bCtrlWinner = false;                              // �Լ��Ƿ�ΪӮ��
			m_bFirstUser=false;                                 // �Ƿ�Ϊ��һ����˵��
			m_bPoor = pStatusFree->bPoor;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//�������
			if ((NULL != pIServerUserItem) && (pIServerUserItem->GetUserStatus()!=US_READY))
			{
				UINT nElapse=rand()%iTIME_RAND+iTIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���ñ���
			m_lTurnMaxGold=pStatusPlay->lTurnMaxGold;
			m_lTurnBasicGold=pStatusPlay->lTurnBasicGold;
			m_lBasicGold = pStatusPlay->lBasicGold;
			m_bPoor = pStatusPlay->bPoor;
			CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
			m_wCurrentUser = pStatusPlay->wCurrentUser;
			m_lShowHandScore = pStatusPlay->lShowHandScore;
			m_bShowHand = pStatusPlay->bShowHand?true:false;
			m_byCardCount = pStatusPlay->bTableCardCount[m_pIAndroidUserItem->GetChairID()];
			m_lMeTableGold=pStatusPlay->lTableGold[m_pIAndroidUserItem->GetChairID()*2 + 1];	// �Լ����µ�����
			m_lMeTurnGold=pStatusPlay->lTableGold[m_pIAndroidUserItem->GetChairID()*2];			// �Լ�һ�����µ�����
			m_lMeScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
			m_bCtrlWinner = false;                                                              // �Լ��Ƿ�ΪӮ��
			m_bFirstUser=false;                                                                 // �Ƿ�Ϊ��һ����˵��
			if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
			{
				// ��ע
				UINT uElapse = iTIME_LESS + (rand()%iTIME_RAND);
				m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
			}			
			return true;
		}
	}
	return true;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

// ������Ҷ���ʼ��
void CAndroidUserItemSink::OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	if (NULL != m_pIAndroidUserItem)
	{
		UINT nElapse=rand()%iTIME_RAND+iTIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
	}
	return ;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	iTIME_LESS=1;//GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_LESS"),TIME_LESS,m_szConfigFileName);
	iTIME_RAND=2;//GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_RAND"),TIME_RAND,m_szConfigFileName);

	//��Ϸ����
	m_lTurnMaxGold=pGameStart->lTurnMaxGold;
	m_lTurnBasicGold=pGameStart->lTurnBasicGold;
	m_lBasicGold = pGameStart->lBasicGold;
	m_bPoor = pGameStart->bPoor;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lShowHandScore = pGameStart->lShowHandScore;
	m_bShowHand=false;
	m_byCardCount = 2;  // ���ŵ���
	m_lMeTableGold=pGameStart->lBasicGold;			// �Լ����µ�����
	m_lMeTurnGold=0L;				                // �Լ�һ�����µ�����
	//m_bCtrlWinner = false;                          // �Լ��Ƿ�ΪӮ��
	m_bAddGold=false;

	if (NULL == m_pIAndroidUserItem)
	{
		return true;
	}

	m_cbOwnCard[0] = pGameStart->bFundusCard;				//�Լ�����
	m_cbOwnCard[1] = pGameStart->bCardData[m_wCurrentUser];	//�Լ�����
	m_cbHumanCard[1] = pGameStart->bCardData[(m_wCurrentUser+1) % GAME_PLAYER];	//�Է�����
	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_PLAY);
	m_lMeScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		const IServerUserItem *pServerUserItem=m_pIAndroidUserItem->GetTableUserItem(i);

		//���ý���
		if (NULL != pServerUserItem)
		{
			m_bPlayStatus[i]=TRUE;
		}
		else
		{
			m_bPlayStatus[i]=FALSE;
		}
	}

	//��Ϸ��ʼ�����˿���
	UINT uElapse = TIME_LESS+rand()%TIME_RAND;
	m_pIAndroidUserItem->SetGameTimer(IDI_LOOK_CARD, uElapse);

	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bFirstUser=true;                              // �Ƿ�Ϊ��һ����˵��
		// ��ע
		UINT uElapse = TIME_LESS+rand()%TIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
	}
	else
	{
		m_bFirstUser=false;                              // �Ƿ�Ϊ��һ����˵��
	}
	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddGold)) return false;
	CMD_S_AddGold * pAddGold=(CMD_S_AddGold *)pBuffer;

	//��������
	m_wCurrentUser = pAddGold->wCurrentUser;

	//��������
	m_lTurnBasicGold=pAddGold->lCurrentLessGold;
	m_bShowHand = pAddGold->bShowHand;
	if (pAddGold->wLastChairID == m_pIAndroidUserItem->GetChairID())
	{
		m_lMeTurnGold = pAddGold->lLastAddGold;
	}

	//��������
	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		// ��עTIME_RANDTIME_LESS
		UINT uElapse = TIME_LESS+rand()%TIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
		//m_bAddGold=true;
	}
	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_bPlayStatus[pGiveUp->wUserChairID]=FALSE;
	if (pGiveUp->wUserChairID==m_pIAndroidUserItem->GetChairID())
	{
		m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_FREE);
		m_bAddGold=false;
	}

	//��������
	if( m_lTurnMaxGold != pGiveUp->lTurnMaxGold )
	{
		//��������
		m_lTurnMaxGold = pGiveUp->lTurnMaxGold;
	}
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_R_SendCard)) return false;
	CMD_R_SendCard * pSendCard=(CMD_R_SendCard *)pBuffer;

	WORD wRand = 0;
	if(pSendCard->cbSendCardCount <= 1)
	{
		wRand = 99;
	}
	else if(pSendCard->cbSendCardCount == 2)
	{
		wRand =  rand()%100;
	}
	else if(pSendCard->cbSendCardCount == 3)
	{
		wRand = rand()%70;
	}
	else
		wRand = rand()%60;

	if(wRand >= 50)
	{
		UINT uElapse = TIME_LESS+rand()%TIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_LOOK_CARD, uElapse);

	}

	//���ñ���
	m_lTurnBasicGold=0L;
	m_lTurnMaxGold=pSendCard->lMaxGold;
	m_wCurrentUser = pSendCard->wCurrentUser;
	m_lMeTableGold += m_lMeTurnGold;
	m_lMeTurnGold = 0L;
	
	m_bAddGold=false;
	m_bWinner = (pSendCard->wWin == m_pIAndroidUserItem->GetChairID());
	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bFirstUser=true;                              // �Ƿ�Ϊ��һ����˵��
		// ��ע
		UINT uElapse = TIME_LESS+rand()%TIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
	}
	else
	{
		m_bFirstUser = false;
	}
	memcpy(m_cbOwnCard+m_byCardCount, pSendCard->bUserCard[m_pIAndroidUserItem->GetChairID()], pSendCard->cbSendCardCount);
	m_byCardCount ++;

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_FREE);
	m_pIAndroidUserItem->KillGameTimer(IDI_PLACE_JETTON);

	m_lTurnMaxGold=0L;						            // �����ע
	m_lTurnBasicGold=0L;					            // ��ע��Ŀ
	m_lBasicGold=0L;						            // ��Ԫ��Ŀ	
	m_wCurrentUser=INVALID_CHAIR;						// ��ǰ��ע��
	m_lShowHandScore=0L;					            // ������߷�
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));	// ��Ϸ״̬
	m_bShowHand = false;  
	m_lMeTableGold = 0L;
	m_lMeTurnGold = 0L;
	m_lMeScore = 0L;
	m_bAddGold=false;
	m_bCtrlWinner = false;                          // �Լ��Ƿ�ΪӮ��
	m_bWinner = false;
	// �Ƿ����
	// ��ע
	UINT uElapse = iTIME_LESS+rand()%iTIME_RAND;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, uElapse);
	return true;
}

// ��ע����
bool CAndroidUserItemSink::OnPlaceJetton(void)
{
	if (NULL == m_pIAndroidUserItem)
	{
		return true;
	}

	// �ж��Լ��Ƿ�Ϊ��ע���
	if ( m_pIAndroidUserItem->GetChairID() == m_wCurrentUser 
		&& (US_LOOKON == m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus())) 
	{
		return true;
	}

	bool bAdd1=false;  // �Ƿ���Լ�ע1
	bool bAdd2=false;  // �Ƿ���Լ�ע2
	bool bAdd3=false;  // �Ƿ���Լ�ע3
	bool bShowhand=false; // �Ƿ�������
	bool bGiveUp = true; // �Ƿ���Է���	
	bool bFollow=true;

	__int64 iGold = 0;
	__int64 lBasicGold = 0;
	if(!m_bPoor)
		lBasicGold = m_lBasicGold;
	else
		lBasicGold = m_lBasicGold/2;
	TCHAR strMessage[222]=_T("");
	//С��3����
	if (m_byCardCount<3)
	{
		//��Ӯ��
		if (m_bWinner)
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_lTurnBasicGold != 0)
				{
					if (iRand<20)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<30)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<40)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						iGold= m_lTurnBasicGold;
					}
				}else
				{
					if (iRand<20)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<40)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<60)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						iGold= m_lTurnBasicGold;
					}
				}
				
			}else
			{
				if (m_bAddGold)
				{
					iGold= m_lTurnBasicGold;
				}
				else
				{
					if (iRand<20)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<40)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<80)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						iGold= m_lTurnBasicGold;
					}
				}
			}				
		}else
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (iRand<40)
				{
					iGold= m_lTurnBasicGold;
				}else
				{
					iGold= -1;
				}

			}else
			{
				if (m_bAddGold)
				{
					if (iRand<50)
					{
						iGold= m_lTurnBasicGold;
					}else
					{
						iGold= -1;
					}
				}
				else
				{
					if (iRand<20)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<40)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<80)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						iGold= m_lTurnBasicGold;
					}
				}
			}		
		}
	}else if (m_byCardCount<4)
	{
		//��Ӯ��
		if (m_bWinner)
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					if (iRand<30)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
					
				}else
				{
					if (iRand<40)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<50)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<60)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<70)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					if (iRand<30)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}

				}else
				{
					if (m_bAddGold)
					{
						
						if (iRand<50)
						{
							iGold= m_lTurnBasicGold;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
					else
					{
						if (iRand<20)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<40)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else  if (iRand<50)
						{
							iGold= m_lTurnBasicGold+1*lBasicGold;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
				}				

				
			}				
		}else
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					iGold= -1;

				}else
				{
					if (iRand<50)
					{
						iGold= m_lTurnBasicGold;
					}else
					{
						iGold= -1;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					if (iRand<80)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}

				}else
				{
					if (m_bAddGold)
					{

						if (iRand<60)
						{
							iGold= m_lTurnBasicGold;
						}else
						{
							iGold= -1;
						}
					}
					else
					{
						if (iRand<20)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<40)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else
						{
							iGold= m_lTurnBasicGold;
						}
					}
				}				


			}		
		}
	}
	else if (m_byCardCount<5)
	{
		//��Ӯ��
		if (m_bWinner)
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					if (iRand<30)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}

				}else
				{
					if (iRand<30)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<50)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<60)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<70)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					iGold = lShowHandScore-m_lMeTableGold;

				}else
				{
					if (m_bAddGold)
					{

						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;

					}
					else
					{
						if (iRand<10)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<20)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else  if (iRand<30)
						{
							iGold= m_lTurnBasicGold+1*lBasicGold;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
				}				


			}				
		}else
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					iGold= -1;

				}else
				{
					if (iRand<40)
					{
						iGold= m_lTurnBasicGold;
					}else
					{
						iGold= -1;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					if (iRand<80)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}

				}else
				{
					if (m_bAddGold)
					{

						if (iRand<60)
						{
							iGold= m_lTurnBasicGold;
						}else
						{
							iGold= -1;
						}
					}
					else
					{
						if (iRand<40)
						{
							iGold= m_lTurnBasicGold;
						}else if (iRand<60)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<80)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else
						{
							iGold= m_lTurnBasicGold;
						}
					}
				}				


			}		
		}
	}
	else
	{
		myprintf(strMessage,sizeof(strMessage),_T("����4����===================================================="));
		WriteErrorRecord(strMessage);

		//��Ӯ��
		if (m_bWinner)
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					iGold = lShowHandScore-m_lMeTableGold;

				}else
				{
					if (iRand<30)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<50)
					{
						iGold= m_lTurnBasicGold+4*lBasicGold;
					}else if (iRand<60)
					{
						iGold= m_lTurnBasicGold+2*lBasicGold;
					}else  if (iRand<70)
					{
						iGold= m_lTurnBasicGold+1*lBasicGold;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					if (iRand<10)
					{
						iGold= -1;
					}else
					{
						__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
						iGold = lShowHandScore-m_lMeTableGold;
					}
				}else
				{
					if (m_bAddGold)
					{

						if (iRand<50)
						{
							iGold= -1;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
					else
					{
						if (iRand<20)
						{
							iGold= m_lTurnBasicGold+4*lBasicGold;
						}else if (iRand<30)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else  if (iRand<40)
						{
							iGold= m_lTurnBasicGold+1*lBasicGold;
						}else
						{
							__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
							iGold = lShowHandScore-m_lMeTableGold;
						}
					}
				}				


			}				
		}else
		{
			int iRand = rand()%100;
			if (!m_bFirstUser)
			{
				if (m_bShowHand)
				{
					iGold= -1;

				}else
				{
					if (iRand<40)
					{
						iGold= m_lTurnBasicGold;
					}else
					{
						iGold= -1;
					}
				}				

			}else
			{
				if (m_bShowHand)
				{
					iGold= -1;	
				}else
				{
					if (m_bAddGold)
					{

						if (iRand<40)
						{
							iGold= m_lTurnBasicGold;
						}else
						{
							iGold= -1;
						}
					}
					else
					{
						if (iRand<30)
						{
							iGold= m_lTurnBasicGold;
						}else if (iRand<40)
						{
							iGold= m_lTurnBasicGold+2*lBasicGold;
						}else if (iRand<60)
						{
							iGold= m_lTurnBasicGold+1*lBasicGold;
						}else
						{
							iGold= m_lTurnBasicGold;
						}
					}
				}				


			}		
		}
	}

	int nValue = m_GameLogic.CompareCard( m_cbOwnCard, m_byCardCount, m_cbHumanCard+1, m_byCardCount-1);
	if(nValue >= 3)
	{
		if(iGold <= 0)
			iGold += m_lTurnBasicGold;
	}
	else if(nValue < -1)
	{
		if(m_byCardCount == 3 && iGold > 0)
		{
			if(iGold > 0)
			{
				if(rand() % 100 < 50)
					iGold = 0;
			}
		}
		if(m_byCardCount == 4)
		{
			if(iGold > 0)
			{
				if(rand() % 100 < 80)
					iGold = -1;
				else
					iGold = 0;
			}
		}

	}
	
	if(m_byCardCount == 5)
		if( nValue < 0)
		{
			iGold= -100;
		}

	//�жϴ�С�����и�Ԥ
	/*
	int nValue = m_GameLogic.CompareCard(�Լ����˿�,m_byCardCount, �Է��˿ˣ��������ƣ�, �Է��˿˸���)) ;
	if((nValue>= 3)
	{
		//���ԱȶԷ�Ҫ��
		��Ҫ������������ע��
	}
	else if(nValue <= 3)
	{
		//����Ҫ�ȶԷ�С��
		m_byCardCount == 2 ʱ������Ԥ��
		m_byCardCount == 3 ʱ��50%������50%��ע��
		m_byCardCount == 4 ʱ��80%������20%��ע������Է��Ѿ���ע�������
		m_byCardCount == 5 ʱ��100%����
	}

	// ���5���ƶ��ȱ���4���ƻ�С�ˣ��ͷ�����
	if(m_byCardCount == 5 && nValue < 0)
		100%����
	*/
	if (iGold < 0)
	{
		BYTE byRand = rand()%5+1;
		m_pIAndroidUserItem->SetGameTimer(IDI_GIVE_UP,byRand);
		return true;
	}

	__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
	if(iGold == lShowHandScore-m_lMeTableGold)
	{
 		BYTE byRand = rand()%5+1;
 		m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_HAND_TIME,byRand);
	}
	else
	{
		//��������
		CMD_C_AddGold AddPoint;
		AddPoint.bFllow = m_bAddGold;
		AddPoint.lGold=iGold;
		if (iGold>0)
		{
			m_bAddGold=true;
		}
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	}

	return true;
}

//д���¼
void CAndroidUserItemSink::WriteErrorRecord(LPCTSTR pszErrorMsg)
{
	try
	{
		//�����ļ���
		TCHAR szPath[MAX_PATH]=TEXT("");
		TCHAR szFileName[MAX_PATH];
		GetCurrentDirectory(sizeof(szPath),szPath);
		myprintf(szFileName,sizeof(szFileName),TEXT("%s\\������.txt"),szPath);

		//���ļ�
		CFile FileRecord;
		if (FileRecord.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			FileRecord.SeekToEnd();
			FileRecord.Write(pszErrorMsg,mystrlen(pszErrorMsg));
			FileRecord.Write(TEXT("\r\n"),mystrlen(TEXT("\r\n")));

			FileRecord.Close();
		}
	}
	catch(...){}
}

//////////////////////////////////////////////////////////////////////////
