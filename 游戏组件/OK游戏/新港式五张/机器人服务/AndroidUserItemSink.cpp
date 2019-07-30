#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					2									//����ʱ��
#define TIME_DISPATCH				2									//����ʱ��

//��Ϸʱ��
#define TIME_OUT_CARD				2									//����ʱ��
#define TIME_START_GAME				3									//��ʼʱ��
#define TIME_CHIP_OP                2                                   //��עʱ��
#define TIME_OUT                    8                      

//��Ϸʱ��
#define IDI_OUT_CARD				(0)			//����ʱ��
#define IDI_START_GAME				(1)			//��ʼʱ��
#define IDI_CHIP_OP                 (2)         //��עʱ��
#define IDI_OUT_TIME                (3)         //��ʱ

//���ж�ʱ
#define IDI_BANK_OPERATE			4									//���ж�ʱ
//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lCellScore = 0;
	m_bFirstChip = false;
	m_lTurnLessScore = 0;
	m_lDrawMaxScore = 0;
	m_nCurCardCount = 0;
	m_lTurnMaxScore = 0;
	m_bGiveUp = false;
	m_bShowHand = false;
	m_bTemp=false;
	m_wCurrentUser = INVALID_CHAIR;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
    ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCard,sizeof(m_cbHandCard));
	ZeroMemory(m_cbTableCardData,sizeof(m_cbTableCardData));
	m_wSpeciaChairID = 0;

	m_lBeforeUserScore = 0;

	//�ӿڱ���
	m_pIAndroidUserItem=NULL;;

	return;
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
	//��Ϸ����
	m_lCellScore = 0;
	m_bFirstChip = false;
	m_lTurnLessScore = 0;
	m_lDrawMaxScore = 0;
	m_lTurnMaxScore = 0;
	m_nCurCardCount = 0;
	m_bGiveUp = false;
	m_wCurrentUser = INVALID_CHAIR;
	m_lBeforeUserScore = 0;


	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCard,sizeof(m_cbHandCard));

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME: //��ʼ��Ϸ
		{
			//��ʼ�ж�
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			 m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);

			return true;
		}
	case IDI_CHIP_OP: //��ע����
		{
			if(m_bGiveUp == true) return true;

			//LONGLONG lAddChip = GetAddChip();
 			OnSubAddScoreEx();
			return true;
		}
	case IDI_OUT_TIME: //��ʱ
		{
			if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
			{
               m_pIAndroidUserItem->KillGameTimer(IDI_CHIP_OP);
			   m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
			}

			return true;
		}
	case IDI_BANK_OPERATE:		//���в���
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_BANK_OPERATE);  //ɾ��ʱ��

			//��������
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();  //��ȡ����ָ��
			LONGLONG lRobotScore = pUserItem->GetUserScore();					//��ȡ���
			{

				//�жϴ�ȡ
				if (lRobotScore > m_lRobotScoreRange[1])   //��Ҵ������ֵ  ���
				{
					LONGLONG lSaveScore=0L;

					lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);  //����ٷֱ�
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0)
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);			//���

				}
				else if (lRobotScore < m_lRobotScoreRange[0])     //ȡ��
				{

					//�γ������޸�
					//�������
					srand( (unsigned)time(NULL));
					
					SCORE lScore=m_lRobotBankGetScore+(rand()%(m_lRobotBankGetScoreBanker-m_lRobotBankGetScore));
					if (lScore > 0)
						m_pIAndroidUserItem->PerformTakeScore(lScore);
				}
			}
			return true;
		}
	
	}

	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START://��Ϸ��ʼ
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_OUT_TIME,TIME_OUT);
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_GIVE_UP: //��������
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_OUT_TIME,TIME_OUT);
            return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_OUT_TIME,TIME_OUT);
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//�����˿�
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_OUT_TIME,TIME_OUT);
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
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
	case GAME_SCENE_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

		//	m_lCellScore = pStatusFree->lCellScore;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			//��Ϣ����
			ReadConfigInformation();

			//�������
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}

			return true;
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

	/*		m_lCellScore = pStatusPlay->lCellScore;

			m_lDrawMaxScore = pStatusPlay->lDrawMaxScore;
			m_lTurnLessScore = pStatusPlay->lTurnLessScore;
			m_lTurnMaxScore = pStatusPlay->lTurnMaxScore;*/

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));
			ReadConfigInformation();
		
			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
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

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_GameStart1));
	if (wDataSize!=sizeof(CMD_S_GameStart1)) return false;

	//��������
	CMD_S_GameStart1 * pGameStart=(CMD_S_GameStart1 *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_PLAY);

	//�������
	 srand(GetTickCount());

	//���±���
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lCellScore = pGameStart->lCellScore;
	m_lTurnLessScore=pGameStart->lTurnLessScore;
	m_lDrawMaxScore=pGameStart->lDrawMaxScore;
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	CopyMemory(m_cbTableCardData,pGameStart->cbTableCardData,sizeof(m_cbTableCardData));
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbOtherCard,sizeof(m_cbOtherCard));
	m_bWillWin = pGameStart->bWillWin;
	

	//������ҵ���
	for(int i=0;i<GAME_PLAYER;i++)
	{
		//if(i==m_pIAndroidUserItem->GetChairID()) continue;


		m_cbOtherCard[i].bGiveUp = false;
        
		if(pGameStart->cbCardData[i]==0)
		{
			m_cbOtherCard[i].bStatue = false;
			continue;
		}

        m_cbOtherCard[i].cbCard[1] = pGameStart->cbCardData[i];

		m_cbOtherCard[i].bStatue = true;

		m_lTableScore[i]=pGameStart->lCellScore;
	}

	m_cbHandCard[0] = pGameStart->cbObscureCard;
	m_cbHandCard[1] = pGameStart->cbCardData[m_pIAndroidUserItem->GetChairID()];
	m_lTableScore[m_pIAndroidUserItem->GetChairID()]=pGameStart->lCellScore;
	m_nCurCardCount = 2;
    m_bFirstChip = true;
	m_bGiveUp = false;
	m_bAddChip = true;

	if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=rand()%TIME_CHIP_OP+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse+TIME_DISPATCH);
	}
	return true;
}

bool CAndroidUserItemSink::OnSubGiveUp(VOID * pData, WORD wDataSize)
{

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GiveUp));
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;

	//��������
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pData;

	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;

	m_cbOtherCard[wGiveUpUser].bGiveUp = true;

	//��ע����
	m_lDrawMaxScore=pGiveUp->lDrawMaxScore;
	m_lTurnMaxScore=pGiveUp->lTrunMaxScore;

	//״̬����
	if(wGiveUpUser==m_pIAndroidUserItem->GetChairID()) m_bGiveUp = true;

	//�����û�
	m_wCurrentUser=pGiveUp->wCurrentUser;
    
	if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID() && m_pIAndroidUserItem->GetChairID() != pGiveUp->wGiveUpUser)
	{
		m_pIAndroidUserItem->KillGameTimer(IDI_CHIP_OP);
		UINT nElapse=rand()%TIME_CHIP_OP+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse+TIME_DISPATCH);
	}

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;

	//��������
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pData;

	//��������
	WORD wAddScoreUser=pAddScore->wAddScoreUser;


	//���ñ���

	m_bFirstChip = false;
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;

	m_bShowHand=(abs(m_lTurnLessScore-m_lDrawMaxScore)<0.001);

	//m_bShowHand = pAddScore->bShowHand;
	SCORE tem = pAddScore->lUserScoreCount;
	if(pAddScore->lUserScoreCount==0) m_bAddChip = false;
	else m_bAddChip = true;

	SCORE lCurrentScore=pAddScore->lUserScoreCount;

	m_lBeforeUserScore = lCurrentScore;


	m_lUserScore[wAddScoreUser]=lCurrentScore;

	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
//		m_pIAndroidUserItem->SendChatMessage(m_pIAndroidUserItem->GetChairID(),TEXT("��ʼ����"),RGB(0,0,0));
		UINT nElapse=rand()%TIME_CHIP_OP+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse+TIME_DISPATCH);
	}
	return true;
}

//�����˿�
bool CAndroidUserItemSink::OnSubSendCard(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pData;

	//���ñ���

	m_wCurrentUser=pSendCard->wCurrentUser;
	m_lTurnMaxScore=pSendCard->lTurnMaxScore;

	//�㼯���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�������
		m_lTableScore[i]+=m_lUserScore[i];

		//�û�����
		m_lUserScore[i]=0L;
	}
    
	//�ɷ��˿�
	int nCurSendCount = m_nCurCardCount;
	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		for (BYTE i=0;i<pSendCard->cbSendCardCount;i++)
		{
			m_cbHandCard[nCurSendCount++] = pSendCard->cbCardData[i][m_pIAndroidUserItem->GetChairID()];
		}
	}

	//�������
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(i==m_pIAndroidUserItem->GetChairID()) continue;

		if(m_cbOtherCard[i].bStatue == false) continue;
        
		nCurSendCount = m_nCurCardCount;

		//�ɷ��˿�
		for (BYTE j=0;j<pSendCard->cbSendCardCount;j++)
		{
			m_cbOtherCard[i].cbCard[nCurSendCount++] = pSendCard->cbCardData[j][i];
		}
	}

	m_nCurCardCount += pSendCard->cbSendCardCount;

	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=rand()%TIME_CHIP_OP+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse+TIME_DISPATCH);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��������
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//��������
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_FREE);
	m_bShowHand = false;

	//ֹͣ����
	m_pIAndroidUserItem->KillGameTimer(IDI_CHIP_OP);

	//�����˼�Ǯ
	m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE,rand()%TIME_START_GAME+TIME_LESS);
	
	//��ʼ����
	UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}
void CAndroidUserItemSink::OnSubAddScoreEx()
{
	switch(m_nCurCardCount)
	{
	case 2:
		{

			if (WinIsAndroid(1,1)) 
			{
				if (GaiLv(50))	//һ������һ��������������ϵ��ƴ�����ҵ������аٷ�֮55��ע
				{
					AddScore();
					return;
				}
				else	//fllow
				{
					FllowScore(); //һ������һ��������������ϵ��ƴ�����ҵ������аٷ�֮45��ע
					return;
				}
			}
			else
			{
				if(m_bWillWin)
				{
					if (GaiLv(80))	//һ������һ��������������ϵ���С����ҵ������аٷ�֮70��ע
					{
						FllowScore();
						return;
					}
					else 
					{
						if (GaiLv(100))	//һ������һ��������������ϵ���С����ҵ������аٷ�֮��100-70����80=24%��ע
						{
							AddScore();
							return;
						}
						
					}
				}
				else
				{
					if (GaiLv(70))	//һ������һ��������������ϵ���С����ҵ������аٷ�֮70��ע
					{
						FllowScore();
						return;
					}
					else 
					{
						if (GaiLv(80))	//һ������һ��������������ϵ���С����ҵ������аٷ�֮��100-70����80=24%��ע
						{
							AddScore();
							return;
						}
						else	//give up
						{
							GiveUpScore();//һ������һ��������������ϵ���С����ҵ������аٷ�֮��100-70����20=6%����
							return;
						}
					}
				}


			}
		}
	case 3:
		{
			if (WinIsAndroid(1,2))
			{
				if (GaiLv(50))	//һ����������������������ϵ��ƴ�����ҵ������аٷ�֮50��ע
				{
					AddScore();
					return;
				}
				else
				{
					int type=m_GameLogic.GetCardGenre(m_cbHandCard,3);

					if(type>CT_SINGLE)
						AddScore();//һ����������������������ϵ��ƴ�����ҵ������аٷ�֮50��ע
					else
						FllowScore();

					return;
				}
			}
			else
			{
				
					if(m_bWillWin)
					{
						if (GaiLv(100))//һ����������������������ϵ���С����ҵ������аٷ�֮50��ע
						{
							FllowScore();
							return;
						}
						else
						{
							if (GaiLv(100))//һ����������������������ϵ���С����ҵ������аٷ�֮��100-50����80=40%��ע
							{
								AddScore();
								return;
							}
					
						}
					}
					else
					{
						if (GaiLv(50))//һ����������������������ϵ���С����ҵ������аٷ�֮50��ע
						{
							FllowScore();
							return;
						}
						else
						{
							if (GaiLv(10))//һ����������������������ϵ���С����ҵ������аٷ�֮��100-50����80=40%��ע
							{
								AddScore();
								return;
							}
							else
							{
								GiveUpScore();//һ����������������������ϵ���С����ҵ������аٷ�֮��100-50����20=10%����
								return;
							}
						}
					}

				
			}
		}
	case 4:
		{
			/*SendSuoHand();
			return;*/
			m_bTemp=true;
			if (m_bShowHand) //����������׼  //�޸� edit 
			{
				if(m_bWillWin)
				{
					if (WinIsAndroid(1,3))
					{
						int type=m_GameLogic.GetCardGenre(m_cbHandCard,4);
						if(type>CT_SINGLE)
						{
							if (GaiLv(100))
							{
								FllowScore();//��������ϵ��ƴ�����������ϵ�����90%�ĸ�ע
								return;
							}
						}
						else
						{
							GiveUpScore();//��������ϵ��ƴ�����������ϵ�����10%����
							return;
						}
						//else
						//{
						//	GiveUpScore();//��������ϵ��ƴ�����������ϵ�����10%����
						//	return;
						//}
						
					}
					else
					{
						int type=m_GameLogic.GetCardGenre(m_cbHandCard,4);
						if(type>CT_SINGLE)
						{
							if (GaiLv(100))
							{
								FllowScore();//��������ϵ��ƴ�����������ϵ�����90%�ĸ�ע
								return;
							}
						}
						else
						{
							GiveUpScore();//��������ϵ��ƴ�����������ϵ�����10%����
							return;
						}
						//else
						//{
						//	GiveUpScore();//��������ϵ��ƴ�����������ϵ�����10%����
						//	return;
						//}
					}
				
				}
				else
				{
					if (WinIsAndroid(1,3))
					{
						int type=m_GameLogic.GetCardGenre(m_cbHandCard,4);
						if(type>CT_ONE_DOUBLE)
						{
							FllowScore();//��������ϵ��ƴ�����������ϵ�����90%�ĸ�ע
							return;
						}
						else
						{

							if (GaiLv(5))
							{
								FllowScore();//��������ϵ��ƴ�����������ϵ�����90%�ĸ�ע
								return;
							}
							else
							{
								GiveUpScore();//��������ϵ��ƴ�����������ϵ�����10%����
								return;
							}
						}
						
					}
					else
					{
						GiveUpScore();//��������ϵ���С����������ϵ��ƾͷ���
						return;
					}
				}
			}
			else
			{
				if(m_bWillWin)
				{
						if (WinIsAndroid(1,3))
						{
							if (GaiLv(10)) 
							{
					//			AddScore();		
								SendSuoHand();	//������������׼����������˵������������������80%���
								return;
							}
							else
							{
								if (GaiLv(50))
								{
									AddScore();		//������������׼����������˵�����������������У�100-80����70=14%��ע
									return;
								}
								else
								{
									int type=m_GameLogic.GetCardGenre(m_cbHandCard,4);
									if(type>CT_ONE_DOUBLE)
										AddScore();
									else
										FllowScore();	//������������׼����������˵�����������������У�100-80����30=6%��ע			
									return;
								}
							}
						}
						else
						{
							if (GaiLv(100))//������������׼����������˵�����С������������30%��ע
							{
								FllowScore();
						
								return;
							}
							//else
							//{
							//	GiveUpScore();//������������׼����������˵�����С������������70%����
							//	return;
							//}
						}
				}
				else
				{
						if (WinIsAndroid(1,3))
						{
							if (GaiLv(5)) 
							{
								AddScore();		
					
								return;
							}
							else
							{
								if (GaiLv(40))
								{
				
									AddScore();		//������������׼����������˵�����������������У�100-80����70=14%��ע
									return;
								}
								else
								{
									FllowScore();	//������������׼����������˵�����������������У�100-80����30=6%��ע			
									return;
								}
							}
						}
						else
						{
							if (GaiLv(20))//������������׼����������˵�����С������������30%��ע
							{
								FllowScore();
						
								return;
							}
							else
							{
								GiveUpScore();//������������׼����������˵�����С������������70%����
								return;
							}
						}
				}
			}
				
		}
		
	case 5:
		{
			m_bTemp=true;
			if (m_bShowHand)
			{
				if(m_bWillWin)
				{
						if (WinIsAndroid(1,4))
						{
							int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
							if(type>CT_ONE_DOUBLE)
							{
								if (GaiLv(100))
								{
									FllowScore();	//����������׼����������˵������������������60%�ĸ��ʸ�ע		
									return;
								}
							}
							else
							{
								GiveUpScore();	//����������׼����������˵������������������40%�ĸ��ʷ���
								return;
							}
						}
						else
						{
							//if (GaiLv(50))
							//{
							//	GiveUpScore();//����������׼����������˵�����С������������90%�ĸ��ʷ���
							//	return;	
							//}
							//else
							{
								int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
								if(type>CT_ONE_DOUBLE)
									FllowScore();//����������׼����������˵�����С������������10%�ĸ��ʸ�ע
								else
									GiveUpScore();

								return;
							}
						}

				}
				else
				{
						if (WinIsAndroid(1,4))
						{
							if (GaiLv(5))
							{
								FllowScore();	//����������׼����������˵������������������60%�ĸ��ʸ�ע		
								return;
							}
							else
							{
								GiveUpScore();	//����������׼����������˵������������������40%�ĸ��ʷ���
								return;
							}
						}
						else
						{
							if (GaiLv(100))
							{
								GiveUpScore();//����������׼����������˵�����С������������90%�ĸ��ʷ���
								return;	
							}
							//else
							//{
							//	FllowScore();//����������׼����������˵�����С������������10%�ĸ��ʸ�ע
							//	return;
							//}
						}
				}

			}
			else
			{
				if(m_bWillWin)
				{
					if (WinIsAndroid(1,4))
						{
							if (GaiLv(30))
							{
								int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
								if(type>CT_ONE_DOUBLE)
									SendSuoHand();
								else
									AddScore();		//������������׼����������˵������������������30%�ĸ��ʼ�ע		
								
								return;
							}
							else
							{
								int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
								if(type>CT_ONE_DOUBLE)
								{
									if (GaiLv(100))
									{
										SendSuoHand();	//������������׼����������˵�����������������У�100-30����90=63%�ĸ������			
										return;
									}
									else
									{
										FllowScore();	//������������׼����������˵�����������������У�100-30����10=7%�ĸ��ʸ�ע				
										return;
									}
								}
								else
								{
									FllowScore();	//������������׼����������˵�����������������У�100-30����10=7%�ĸ��ʸ�ע				
									return;
								}
							}
						}
					else
						{

							int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
							if(type>CT_ONE_DOUBLE)
							{
								if (GaiLv(100))
								{
									int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
									if(type>CT_ONE_DOUBLE)
										SendSuoHand();
									else
										FllowScore();	//������������׼����������˵�����С������������50%�ĸ��ʸ�ע	

									return;
								}
							}
							else
							{
								GiveUpScore();	//������������׼����������˵�����С������������50%�ĸ��ʷ���
								return;
							}
						}
				}
				else
				{
					if (WinIsAndroid(1,4))
					{
						int type=m_GameLogic.GetCardGenre(m_cbHandCard,5);
						if(type>=CT_ONE_DOUBLE)
						{
							if (GaiLv(30))
							{
								AddScore();		//������������׼����������˵������������������30%�ĸ��ʼ�ע		
								return;
							}
							else
							{
								if (GaiLv(0))
								{
									SendSuoHand();	//������������׼����������˵�����������������У�100-30����90=63%�ĸ������			
									return;
								}
								else
								{
									FllowScore();	//������������׼����������˵�����������������У�100-30����10=7%�ĸ��ʸ�ע				
									return;
								}
							}
						}
						else
						{
							GiveUpScore();
						}
					}
					else
					{
						if (GaiLv(10))
						{
							FllowScore();	//������������׼����������˵�����С������������50%�ĸ��ʸ�ע	
							return;
						}
						else
						{
							GiveUpScore();	//������������׼����������˵�����С������������50%�ĸ��ʷ���
							return;
						}
					}
				}
			}
		}
		ASSERT(false);
		
	}
}
void CAndroidUserItemSink::GiveUpScore()
{

	

	if (m_bTemp)
	{
		m_bTemp=false;
		m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
		return;
	}
	else
	{
		if (m_wCurrentUser ==m_wSpeciaChairID)
		{	
			FllowScore();
			return;
		}
		m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
		return;
	}
}
void CAndroidUserItemSink::AddScore()
{
		
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		SCORE lScoreLess = m_lTurnLessScore - ( m_lTableScore[wMeChairID] + m_lUserScore[wMeChairID] );

		int iTem = 0;

		if(m_nCurCardCount>3)
			iTem = m_lBeforeUserScore+rand()%5+1;
		else
			iTem = m_lBeforeUserScore+rand()%3+1;

		SCORE lXXParam=0;

		SCORE lAddChip = iTem*m_lCellScore;
	//	if((LONGLONG)lScoreLess>=4*m_lCellScore)
	//		lXXParam = lScoreLess;
	//	else
		  	lXXParam= lAddChip + lScoreLess;

	//	SCORE lXXParam = iTem*m_lCellScore + (LONGLONG)lScoreLess;

		//��Ҽ���
		SCORE lDrawAddScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];
		//LONGLONG lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);
	//	LONGLONG lCurrentAddScore=(m_lTurnLessScore-lDrawAddScore);

	//	if(lCurrentAddScore>m_lTurnMaxScore) lCurrentAddScore=m_lTurnMaxScore-lDrawAddScore;

		SCORE lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);
		
		if(lCurrentAddScore>m_lTurnMaxScore) lCurrentAddScore=m_lTurnMaxScore-lDrawAddScore;



		//��������
		m_lUserScore[wMeChairID]+=lCurrentAddScore;



		
	



		//������Ϣ
		CMD_C_AddScore AddScore;
		
	//	lCurrentAddScore = lCurrentAddScore;
		AddScore.lScore=lCurrentAddScore;
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
		return;
}
void CAndroidUserItemSink::FllowScore()
{


//	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
//
//	SCORE lScoreLess = m_lTurnLessScore - ( m_lTableScore[wMeChairID] + m_lUserScore[wMeChairID] );
//	//LONGLONG lXXParam = lAddChip + (LONGLONG)lScoreLess;
//	//��Ҽ���
//	SCORE lDrawAddScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];
//	//LONGLONG lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);
//	SCORE lCurrentAddScore=(m_lTurnLessScore-lDrawAddScore);
//
//	if(lCurrentAddScore>m_lTurnMaxScore) lCurrentAddScore=m_lTurnMaxScore-lDrawAddScore;
//
////	SCORE lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lScoreLess);
//
//	//��������
//	m_lUserScore[wMeChairID]+=lCurrentAddScore;
//	//������Ϣ
//	CMD_C_AddScore AddScore;
//	AddScore.lScore=lCurrentAddScore;
//	m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
	//KillTimer()
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	SCORE lScoreLess = m_lTurnLessScore - ( m_lTableScore[wMeChairID] + m_lUserScore[wMeChairID] );

	SCORE lXXParam = (LONGLONG)lScoreLess;
	//��Ҽ���
	SCORE lDrawAddScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];
	SCORE lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);
	//LONGLONG lCurrentAddScore=(m_lTurnLessScore-lDrawAddScore);
	//if(lCurrentAddScore>m_lTurnMaxScore) lCurrentAddScore=m_lTurnMaxScore-lDrawAddScore;
	//��������
	m_lUserScore[wMeChairID]+=lCurrentAddScore;
	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentAddScore;
	m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));



	return;


}
bool CAndroidUserItemSink::GaiLv(BYTE bNum)
{
	if (rand()%100<bNum)
	{
		return true;
	}
	return false;
}
bool CAndroidUserItemSink::WinIsAndroid(BYTE cbStartPos, BYTE cbConcludePos)
{
	WORD iWinner = EstimateWinner(cbStartPos,cbConcludePos);
//	IServerUserItem *pIServerUserItem = m_pIAndroidUserItem->GetTableUserItem(iWinner);
	if(iWinner==m_pIAndroidUserItem->GetChairID())
		return true;
/*	if (pIServerUserItem->IsAndroidUser())
	{
		return true;
	}*/
	return false;
}
WORD CAndroidUserItemSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbTableCardData,sizeof(cbUserCardData));

	//Ѱ�����
	WORD wWinner=0;
	for ( wWinner=0;wWinner<GAME_PLAYER;wWinner++)
	{
		if (m_cbOtherCard[wWinner].bStatue==true) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_cbOtherCard[i].bStatue == FALSE) continue;
		
		
	//	IServerUserItem *pIServerUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
		
	//	if(pIServerUserItem==NULL) continue;

	//	if (pIServerUserItem->IsAndroidUser() && i != m_wCurrentUser) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=i;
		}
	}
	
	return wWinner;

}
LONGLONG CAndroidUserItemSink::GetAddChip()
{

	//��־����
	bool bEnableShowHand=(m_lDrawMaxScore==m_lTurnMaxScore);
	bool bFollow = m_lTurnMaxScore>=m_lTurnLessScore;
	bool bAddChip = m_lTurnMaxScore>=m_lTurnLessScore;
	bool bShowHand = ((bEnableShowHand) && (m_lTurnMaxScore<=m_lTurnLessScore+m_lTurnLessScore));
	WORD wWinner = m_pIAndroidUserItem->GetChairID();
	BYTE bOtherKind=0;

	//��ȡ��ǰ�ƾ�����
	//Ѱ�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(i == m_pIAndroidUserItem->GetChairID()) continue;
		if(m_cbOtherCard[i].bStatue==false||(m_cbOtherCard[i].bStatue==true&&m_cbOtherCard[i].bGiveUp==true)) continue;

		//�����˿�
		m_GameLogic.SortCardList(m_cbOtherCard[i].cbCard+1,m_nCurCardCount-1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(m_cbOtherCard[i].cbCard+1,m_cbOtherCard[i].cbCard+1,m_nCurCardCount-1)==true || wWinner==m_pIAndroidUserItem->GetChairID()) 
		{
			wWinner=i;
		}
	}

	if (wWinner==m_pIAndroidUserItem->GetChairID())
	{
		return 0;
	}

	bOtherKind = m_GameLogic.GetCardGenre(m_cbOtherCard[wWinner].cbCard+1,m_nCurCardCount-1);
	m_GameLogic.SortCardList(m_cbHandCard+1,m_nCurCardCount-1);
	BYTE bCardKind = m_GameLogic.GetCardGenre(m_cbHandCard+1,m_nCurCardCount-1);

	bool bMaxCmp1 = m_GameLogic.CompareCard(m_cbHandCard+1,m_cbOtherCard[wWinner].cbCard+1,m_nCurCardCount-1);
	bool bMaxCmp0 = m_GameLogic.CompareCard(m_cbHandCard+1,m_cbOtherCard[wWinner].cbCard,m_nCurCardCount);

	int r = ((rand()<<1)+(rand()>>1)+time(NULL)) & 127;
	if (r>40 && (bShowHand || m_nCurCardCount>3) && (bMaxCmp0 || bMaxCmp1 || bOtherKind==CT_SINGLE || bCardKind>=bOtherKind) && m_bWillWin)
	{
//		return SendSuoHand();
	}else if (!bShowHand)
	{
		if (bMaxCmp0||bMaxCmp1||(m_bWillWin&&(bOtherKind==CT_SINGLE || bCardKind>=bOtherKind)))
		{
			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			LONGLONG lCurrentAddScore=m_lDrawMaxScore-m_lTableScore[wMeChairID]-m_lUserScore[wMeChairID];
			
			
		return __min(m_lCellScore*(rand()%(bAddChip?3:1)),lCurrentAddScore);//��ע


		}else if (m_nCurCardCount<4)
		{
			return 0;
		}
	}
	else if(m_nCurCardCount<4)
	{
		/*if (r>40)*/ return 0;
	}
	//m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
	return 0; //����
}

void CAndroidUserItemSink::SendSuoHand()
{
	
	
		//��Ҽ���
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		SCORE lCurrentAddScore=m_lDrawMaxScore-m_lTableScore[wMeChairID]-m_lUserScore[wMeChairID];



		//��������
		m_lUserScore[wMeChairID]+=lCurrentAddScore;

		//������Ϣ
		CMD_C_AddScore AddScore;
		AddScore.lScore=lCurrentAddScore;
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
		return;
	
	
}
//////////////////////////////////////////////////////////////////////////
//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����
	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\HKFiveCardConfig.ini"),szPath);

	//��������
	m_lRobotScoreRange[0] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMin"), 50, szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMax"), 1000, szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������
	m_lRobotBankGetScore = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGet"), 100, szConfigFileName);

	//������� 
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGetBanker"), 500, szConfigFileName);

	//����
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 50, szConfigFileName);



}