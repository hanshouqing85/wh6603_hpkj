#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
//����ʱ��
#define TIME_LESS					2									//����ʱ��
#define TIME_JETTON				    5									//��עʱ��
#define TIME_RAND				    8									//���ʱ��

//��Ϸʱ��
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			    // ��Ϸ��ʼID
#define IDI_PLACE_JETTON        (IDI_ANDROID_ITEM_SINK+1)               // ��ע�׶�

extern void OutputDebugMsg(const char * szMsg, ...);

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
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

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SH_Config.ini"),szPath);
	iTIME_RAND=8;iTIME_LESS=2;

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
	default: break;
	}
	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
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
				m_bCtrlWinner = true;
			}
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

			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//�������
			if ((NULL != pIServerUserItem) && (pIServerUserItem->GetUserStatus()!=US_READY))
			{
				UINT nElapse=rand()%iTIME_RAND+iTIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���ñ���
			m_lTurnMaxGold=pStatusPlay->lTurnMaxGold;
			m_lTurnBasicGold=pStatusPlay->lTurnBasicGold;
			m_lBasicGold = pStatusPlay->lBasicGold;
			CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
			m_wCurrentUser = pStatusPlay->wCurrentUser;
			m_lShowHandScore = pStatusPlay->lShowHandScore;
			m_bShowHand = pStatusPlay->bShowHand?true:false;
			m_byCardCount = pStatusPlay->bTableCardCount[m_pIAndroidUserItem->GetChairID()];
			m_lMeTableGold=pStatusPlay->lTableGold[m_pIAndroidUserItem->GetChairID()*2 + 1];	// �Լ����µ�����
			m_lMeTurnGold=pStatusPlay->lTableGold[m_pIAndroidUserItem->GetChairID()*2];			// �Լ�һ�����µ�����
			m_lMeScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore;
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

	iTIME_LESS=GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_LESS"),TIME_LESS,m_szConfigFileName);
	iTIME_RAND=GetPrivateProfileInt(TEXT("AndroidTime"),TEXT("TIME_RAND"),TIME_RAND,m_szConfigFileName);

	//��Ϸ����
	m_lTurnMaxGold=pGameStart->lTurnMaxGold;
	m_lTurnBasicGold=pGameStart->lTurnBasicGold;
	m_lBasicGold = pGameStart->lBasicGold;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lShowHandScore = pGameStart->lShowHandScore;
	m_bShowHand=false;
	m_byCardCount = 2;  // ���ŵ���
	m_lMeTableGold=pGameStart->lBasicGold;			// �Լ����µ�����
	m_lMeTurnGold=0L;				                // �Լ�һ�����µ�����
	m_bCtrlWinner = false;                          // �Լ��Ƿ�ΪӮ��

	if (NULL == m_pIAndroidUserItem)
	{
		return true;
	}

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_PLAYING);
	m_lMeScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore;

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

	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bFirstUser=true;                              // �Ƿ�Ϊ��һ����˵��
		// ��ע
		UINT uElapse = iTIME_LESS+rand()%iTIME_RAND;
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
		// ��ע
		UINT uElapse = iTIME_LESS+rand()%iTIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
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
		m_pIAndroidUserItem->SetGameStatus(GS_FREE);
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

	//���ñ���
	m_lTurnBasicGold=0L;
	m_lTurnMaxGold=pSendCard->lMaxGold;
	m_wCurrentUser = pSendCard->wCurrentUser;
	m_lMeTableGold += m_lMeTurnGold;
	m_lMeTurnGold = 0L;
	m_byCardCount ++;	
	if (m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bFirstUser=true;                              // �Ƿ�Ϊ��һ����˵��
		// ��ע
		UINT uElapse = iTIME_LESS+rand()%iTIME_RAND;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, uElapse);
	}
	else
	{
		m_bFirstUser = false;
	}
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_FREE);
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
	if (m_bShowHand)
	{
		// ֻ�������������
		bShowhand = true;
		bGiveUp = true;
		bFollow =false;
		int iRand = rand()%100;
		if (iRand > 80)
		{
			__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
			iGold = lShowHandScore-m_lMeTableGold;
		}
		else
		{
			// ���֪ͨ�Ǳ�Ӯ��ʱ���Զ������Ļ������
			if (m_bCtrlWinner && iRand > 8)
			{
				__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
				iGold = lShowHandScore-m_lMeTableGold;
			}
			else
			{
				iGold = -1;
			}
		}
	}
	else
	{
		// ��ͨ��ע��Ӧ���Ǹ�ע�Ƚ϶�
		__int64 lLeaveScore=m_lTurnMaxGold-m_lMeTableGold-m_lMeTurnGold;
		if ((m_byCardCount >= 3) && (lLeaveScore>0))
		{
			bShowhand = true;
		}
		lLeaveScore=m_lTurnBasicGold-m_lMeTurnGold;
		__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
		if ((0 == m_lTurnBasicGold) || ((lLeaveScore>0L) && (lShowHandScore>m_lTurnBasicGold+m_lMeTableGold)))
		{
			bFollow = true;
		}
		else
		{
			bFollow = false;
		}
		lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_lMeTableGold;
		bAdd1 = (lLeaveScore>=m_lBasicGold)?true:false;
		bAdd2 = (lLeaveScore>=(2*m_lBasicGold))?true:false;
		bAdd3 = (lLeaveScore>=(3*m_lBasicGold))?true:false;	
		// �м�עһ�������Ϊ��ע
		int iRand = rand()%100;
		if (bAdd1)
		{
			if ((iRand > 96) || (!m_bCtrlWinner && iRand>90 && m_byCardCount>3))
			{
				iGold = -1;
			}
			else if (((iRand>82) || (m_bCtrlWinner && iRand>75))  && bAdd3)
			{
				iGold= m_lTurnBasicGold+3*m_lBasicGold;
			}
			else if (((iRand>70) || (m_bCtrlWinner && iRand>67))  && bAdd2)
			{
				iGold = m_lTurnBasicGold+2*m_lBasicGold;
			}
			else if (iRand > 18 && bFollow)
			{
				if (0 == m_lMeTurnGold)
				{
					// ���Ǳ�Ӯ���Ը���
					if (!m_bCtrlWinner && iRand>48)
					{
						iGold=m_lMeTurnGold;
					}
					else
					{
						iGold = m_lTurnBasicGold+m_lBasicGold;
					}
				}
				else
				{
					//��ȡ�Ƹ�
					iGold=m_lMeTurnGold;
				}				
			}
			else if (iRand > 10 && bShowhand) // �л������
			{
				if (m_bFirstUser || (iRand > 14))
				{
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					iGold = lShowHandScore-m_lMeTableGold;
				}
				else
				{
					iGold = m_lTurnBasicGold+m_lBasicGold;
				}
			}
			else
			{
				iGold = m_lTurnBasicGold+m_lBasicGold;
			}

			if (iGold >= 0)
			{
				__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
				if (iGold+m_lMeTableGold < lShowHandScore)
				{
					iGold=__max(iGold,m_lTurnBasicGold);
				}
			}
		}
		else
		{
			if (iRand>95) // ����
			{
				iGold = -1;
			}
			else if (iRand >88)
			{
				if (!m_bShowHand)
				{
					iGold = -1;
				}
				else
				{
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					iGold = lShowHandScore-m_lMeTurnGold;
				}
			}
			else
			{
				if (bFollow)
				{
					//��ȡ�Ƹ�
					iGold=m_lMeTurnGold;
					__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
					if( iGold+m_lMeTableGold < lShowHandScore )
						iGold=__max(iGold,m_lTurnBasicGold);
				}
				else
				{
					iGold = -1;
				}
			}
		}
	}

	if (iGold < 0)
	{
		if (m_bFirstUser && !m_bShowHand)
		{
			iGold= m_lTurnBasicGold+m_lBasicGold;
			__int64 lShowHandScore = __min(m_lMeScore,m_lShowHandScore);
			if( iGold+m_lMeTableGold < lShowHandScore )
				iGold=__max(iGold,m_lTurnBasicGold);
		}
		else
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
			return true;
		}
	}
	//��������
	CMD_C_AddGold AddPoint;
	AddPoint.lGold=iGold;
	m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	return true;
}

//////////////////////////////////////////////////////////////////////////
