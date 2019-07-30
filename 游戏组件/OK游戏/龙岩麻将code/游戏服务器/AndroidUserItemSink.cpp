#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#ifdef _DEBUG
	#define TIME_LESS				1									//����ʱ��
#else
	#define TIME_LESS				1									//����ʱ��
#endif
//��Ϸʱ��
#define TIME_OUT_CARD				3									//����ʱ��
#define TIME_START_GAME				3									//��ʼʱ��
#define TIME_OPERATE_CARD			3									//����ʱ��
#define TIME_HEAR_STATUS			3									//����ʱ�䰴

//��Ϸʱ��
#define IDI_OUT_CARD				(IDI_REBOT_MODULE_START+0)			//����ʱ��
#define IDI_START_GAME				(IDI_REBOT_MODULE_START+1)			//��ʼʱ��
#define IDI_OPERATE_CARD			(IDI_REBOT_MODULE_START+2)			//����ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//״̬����
	m_bHearStatus=false;
	m_bWillHearStatus=false;
	m_bActionMask = WIK_NULL;
	m_cbActionCard=0;
	m_dwGameRuleIdex=0;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	m_cbSendCardData = 0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	
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
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_dwGameRuleIdex=0;
	//״̬����
	m_bHearStatus=false;
	m_bWillHearStatus=false;
	m_bActionMask = WIK_NULL;
	m_cbActionCard=0;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	m_cbSendCardData = 0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
			//��ʼ�ж�
			if( m_pIAndroidUserItem->GetGameStatus()==GS_MJ_FREE )
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//���ȴ��������Ϣ�������ܺ���
			WORD wMeChair=m_pIAndroidUserItem->GetChairID();
			if((m_bActionMask!=WIK_NULL)  /*&&(m_wCurrentUser!=m_pIAndroidUserItem->GetChairID())*/)
			{
				if(m_bActionMask&WIK_CHI_HU )
				{
#ifdef DEBUG
					//OnOperateCard(WIK_NULL,0);
					OnOutCard(m_cbActionCard);
#else
					OnOperateCard(WIK_CHI_HU,m_cbActionCard);
#endif // DEBUG
					return true;
				}
				if(m_bActionMask&WIK_GANG)
				{
					OnOperateCard(WIK_GANG,m_cbActionCard);
					return true;
				}
	
				m_bActionMask &=~WIK_LISTEN;
				if((m_bActionMask!=WIK_NULL))		//&&(m_bHearStatus==false))
				{
					BYTE cbOperateCode=GetTheBestOperate(m_bActionMask,m_cbActionCard);
					ASSERT( m_cbActionCard!=0xFF);
					ASSERT(m_cbActionCard!=0);
					if(cbOperateCode==WIK_NULL)
						m_cbActionCard=0;
					OnOperateCard(cbOperateCode,m_cbActionCard);
				}
				else
				{
					////�������Ϊ���� ����ģ��Ͳ��û�����
					//OnOperateCard(m_bActionMask, m_cbActionCard);
					//return true;
				}
			}
			//��������������/������
			if(m_wCurrentUser==wMeChair)
			{
#ifdef _DEBUG
				//��ʼץʲô�ƾʹ�ʲô�ƵĽ���
				if(m_cbSendCardData!=0 && !m_GameLogic.IsMagicCard(m_cbSendCardData))
				{
					OnOutCard(m_cbSendCardData);
					return true;
				}
#endif

				//�Ѿ�ͣ���ˣ������Ƿ���״̬���Զ�ͣ��ͣ��
				if((m_bHearStatus==true)&&(m_cbSendCardData!=0))
				{
					BYTE cbCardData=m_cbSendCardData;
					//����Ч��
					if (VerdictOutCard(cbCardData)==false)
					{
						for (BYTE i=0;i<MAX_INDEX;i++)
						{
							//����Ч��
							if (m_cbCardIndex[wMeChair][i]==0) continue;
							if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false)
								continue;

							if (m_GameLogic.IsMagicCard(cbCardData)) 
								continue;

							//���ñ���
							cbCardData=m_GameLogic.SwitchToCardData(i);
						}
					}
					//���ƶ���
					OnOutCard(cbCardData);
					return true;

				}

				//���ܳ���:��û��ͣ����
				BYTE cbCardData=GetIsolatedCard();
				if((cbCardData!=0xFF)&&(m_bHearStatus==false))
				{
					//δͣ�ƣ����ܰѲ����ƴ��ȥ
					if(!m_GameLogic.IsMagicCard(cbCardData))
					{
						OnOutCard(cbCardData);
						return true;
					}
				}

				//�쳣����
				for (INT i=MAX_INDEX-1;i>0;i--)
				{
					if((m_cbCardIndex[wMeChair][i]>0))
					{
						BYTE cbIndex=i;
						cbCardData=m_GameLogic.SwitchToCardData(i);
						if(m_bHearStatus==false && !m_GameLogic.IsMagicCard(cbCardData))
						{
							OnOutCard(cbCardData);
							return true;
						}
					}
				}
				
				//��ʼץʲô�ƾʹ�ʲô�ƵĽ���
				if(m_cbSendCardData!=0 && !m_GameLogic.IsMagicCard(m_cbSendCardData))
				{
					OnOutCard(m_cbSendCardData);
					return true;
				}

				//����Ч��
				if (VerdictOutCard(cbCardData)==false)
				{
					for (BYTE i=0;i<MAX_INDEX;i++)
					{
						//����Ч��
						if (m_cbCardIndex[wMeChair][i]==0) continue;
						if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false)continue;

						//���ñ���
						cbCardData=m_GameLogic.SwitchToCardData(i);
					}
				}
				//���ƶ���
				OnOutCard(cbCardData);
				return true;

				ASSERT(FALSE);

			}
			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_ANDROID_RULE:
		{
			ASSERT(wDataSize == sizeof(CMD_S_AndroidRule));
			if (wDataSize != sizeof(CMD_S_AndroidRule))return false;

			CMD_S_AndroidRule* pInfo =(CMD_S_AndroidRule*)pData;
//			m_cbGameTypeIdex = pInfo->cbGameTypeIdex;
			m_dwGameRuleIdex = pInfo->dwGameRuleIdex;
			m_GameLogic.SetMagicIndex(pInfo->cbMagicIndex);
			//if (m_GameLogic.hasRule(GAME_TYPE_ZZ_HONGZHONG))
			//{
			//	m_GameLogic.SetMagicIndex(m_GameLogic.SwitchToCardIndex(0x35));
			//}
			return true;
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			return OnSubOperateNotify(pData,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			return OnSubOperateResult(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return true;
		}
	case SUB_S_CHI_HU:
		{
			return true;
		}
	case SUB_S_GANG_SCORE:
		{
			return true;
		}
	case SUB_S_MASTER_HANDCARD:
		{
			return true;
		}
	//case SUB_S_OUT_CARD_CSGANG:
	//	{
	//		return true;
	//	}
	//case SUB_S_XIAO_HU:
	//	{
	//		return true;
	//	}
	case SUB_S_GAME_END_LAST_CARD:
		{
			return true;
		}
	//case SUB_S_USE_HAIDI_CARD:
	//	{
	//		m_pIAndroidUserItem->SetGameTimer(IDI_HAI_DI_CARD,2);
	//		return true;
	//	}
	//case SUB_S_PIAO_NOTICE:
	//	{
	//		return true;
	//	}
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
	case GS_MJ_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;

			if( pIServerUserItem->GetUserStatus() != US_READY )
			{
				UINT nElapse = rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;

			//����״̬
			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			m_bHearStatus=(pStatusPlay->cbHearStatus[wMeChairID]==TRUE)?true:false;

			//��ʷ����
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));

			//�˿˱���
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()]);

			//��������
			if ((pStatusPlay->cbActionMask!=WIK_NULL))
			{
				UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
			}
			if (m_wCurrentUser==m_pIAndroidUserItem->GetChairID()) 
			{
				{
					UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
					m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
				}

			}

			return true;
		}
	}

	return true;
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

//ׯ����Ϣ
bool CAndroidUserItemSink::OnSubGameStart(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_MJ_PLAY);

	//���ñ���
	m_bHearStatus=false;
	m_bWillHearStatus=false;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	m_bActionMask = WIK_NULL;
	m_cbActionCard = 0;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	//m_cbSendCardData = 0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����˿�
	WORD wMeChairId = m_pIAndroidUserItem->GetChairID();
	BYTE cbCardCount = (wMeChairId==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex[wMeChairId]);

	
	//��������
	if ((pGameStart->cbUserAction!=WIK_NULL) || pGameStart->wCurrentUser==m_pIAndroidUserItem->GetChairID() )
	{
		m_bActionMask = pGameStart->cbUserAction;
		UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,3+nElapse);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_MJ_FREE);

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//����
	UINT nElapse = rand()%TIME_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}



//������ʾ
bool CAndroidUserItemSink::OnSubOperateNotify( const void *pBuffer,WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//�û�����
	if ((pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//��ȡ����
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		m_bActionMask = pOperateNotify->cbActionMask;
		m_cbActionCard=pOperateNotify->cbActionCard;
		//�������
		if(pOperateNotify->cbActionMask & WIK_GANG > 0)
		{
			TRACE("GANG:user%ld, operationCard:%x, GangCard:%x\n", pOperateNotify->wResumeUser, pOperateNotify->cbActionCard, pOperateNotify->cbGangCardData);
			m_cbActionCard = pOperateNotify->cbGangCardData[0];
		}

		//����ʱ��
		UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
	}
	else
		ASSERT(0);

	return true;
}

//
bool CAndroidUserItemSink::OnSubOperateResult( const void *pBuffer,WORD wDataSize )
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard[0];

	//���Ʊ���
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//��������
	m_bActionMask = WIK_NULL;
	m_cbActionCard =0;
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//�������
	if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)
	{
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;

		//����˿�
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
				break;
			}
		}

		//����˿�
		if (cbWeaveIndex==0xFF)
		{
			//�����ж�
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

			//�����˿�
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		}

		//�˿�����
		if(wOperateUser==m_pIAndroidUserItem->GetChairID())
			m_cbCardIndex[wOperateUser][m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard[0])]=0;

	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL&&pOperateResult->cbOperateCode!=WIK_CHI_HU)
	{
		//���ñ���
		m_wCurrentUser=pOperateResult->wOperateUser;

		//�������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);


		//ɾ���˿�
		if(wOperateUser==m_pIAndroidUserItem->GetChairID())
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);

			m_GameLogic.RemoveCard(m_cbCardIndex[wOperateUser],cbWeaveCard,cbWeaveCardCount-1);
		}
	}

	//����ʱ��
	if (m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		BYTE cbChiHuRight=0;
		BYTE cbActionMask=m_GameLogic.AnalyseTingCard(m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],m_WeaveItemArray[m_pIAndroidUserItem->GetChairID()],m_cbWeaveCount[m_pIAndroidUserItem->GetChairID()]);
		if((cbActionMask==WIK_LISTEN)&&(m_bHearStatus==false))
		{
			CMD_C_Listen Listen;
			Listen.cbListen=m_wCurrentUser;
			
			if( !m_pIAndroidUserItem->SendSocketData(SUB_C_LISTEN,&Listen,sizeof(Listen)))
			{
				ASSERT( FALSE );
				return false;
			}
			TRACE("AAA���%ld ͣ��\n", m_wCurrentUser);
			m_bHearStatus=true;

		}
		//����ʱ��
		UINT nElapse=rand()%TIME_OPERATE_CARD+TIME_LESS;
		if ((m_bHearStatus==true)) 
			nElapse=rand()%TIME_HEAR_STATUS+TIME_LESS;
		//ASSERT(FALSE);

		//����ʱ��
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
	}

	return true;
}

//
bool CAndroidUserItemSink::OnSubOutCard( const void *pBuffer,WORD wDataSize )
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	ASSERT( pOutCard->cbOutCardData != 0 );
	m_cbOutCardData=pOutCard->cbOutCardData;


	return true;
}

//
bool CAndroidUserItemSink::OnSubSendCard( const void *pBuffer,WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	m_cbActionCard = pSendCard->cbCardData;

	//���ñ���
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]++] = m_cbOutCardData;
		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//���ƴ���
	if ((pSendCard->cbCardData!=0)&&((m_wCurrentUser==wMeChairID)))
	{
		m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;	
		//�۳��˿�
		m_cbLeftCardCount--;
	}

	//��ǰ�û�
	if ((m_wCurrentUser==wMeChairID))
	{
		m_cbSendCardData = pSendCard->cbCardData;
		if((m_bActionMask & WIK_GANG) > 0)
		{
			m_cbActionCard = pSendCard->cbGangCardIndeed;		//�����ĸ���
		}
		//�����ж�
		if (m_bHearStatus==false)
		{
			BYTE cbChiHuRight=0;
			BYTE cbWeaveCount=m_cbWeaveCount[wMeChairID];
			if( WIK_LISTEN==m_GameLogic.AnalyseTingCard(m_cbCardIndex[wMeChairID],m_WeaveItemArray[wMeChairID],cbWeaveCount) )
			{
				m_bWillHearStatus = true;
			}
			else 
				m_bWillHearStatus = false;
		}
	}

	//����ʱ��
	if( wMeChairID == m_wCurrentUser )
	{
		m_bActionMask = pSendCard->cbActionMask;
		if( (m_bWillHearStatus == true)&&(m_bHearStatus==false) )
			m_bActionMask |= WIK_LISTEN;

		if((m_bActionMask&WIK_LISTEN)&&(m_bHearStatus==false))
		{

			CMD_C_Listen Listen;
			Listen.cbListen=m_wCurrentUser;
			 
			if( !m_pIAndroidUserItem->SendSocketData(SUB_C_LISTEN,&Listen,sizeof(Listen)))
			{
				ASSERT( FALSE );
				return false;
			}
			TRACE("AAA���%ld ͣ��\n", m_wCurrentUser);
			m_bHearStatus=true;

		}
		//����ʱ��
		UINT nElapse=rand()%TIME_OPERATE_CARD+TIME_LESS;
		if ((m_bHearStatus==true)&&(m_wCurrentUser==wMeChairID)) 
			nElapse=rand()%TIME_HEAR_STATUS+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
	}

	return true;
}
//�û�����
bool CAndroidUserItemSink::OnSubListenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_C_Listen));
	if (wDataSize!=sizeof(CMD_C_Listen)) return false;

	//��������
	CMD_C_Listen * pListen=(CMD_C_Listen *)pBuffer;
	if(pListen->cbListen==m_pIAndroidUserItem->GetChairID())
	{
		m_bHearStatus=true;
	}
	return true;

}

//�û�����
void CAndroidUserItemSink::OnOutCard( BYTE cbOutCard )
{
	//��������
	if (m_bWillHearStatus==true)
	{
		m_bHearStatus=true;
		m_bWillHearStatus=false;
	}

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	WORD wMeChairId = m_pIAndroidUserItem->GetChairID();
	m_GameLogic.RemoveCard(m_cbCardIndex[wMeChairId],cbOutCard);

	//���ý���
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);
	m_cbSendCardData=0;

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCard;
//	ASSERT(!m_GameLogic.IsMagicCard(cbOutCard));
	if( !m_pIAndroidUserItem->SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)) )
	{
		ASSERT( FALSE );
		return ;
	}

	//���ñ���
	m_bActionMask = WIK_NULL;
	
	return ;
}

//
void CAndroidUserItemSink::OnOperateCard( BYTE cbOperateCode,BYTE cbOperateCard )
{
	//ɾ��ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//��������
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));


	//��ȡ����
	if (m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		BYTE cbWeaveCount=m_cbWeaveCount[wMeChairID];
		m_GameLogic.AnalyseGangCard(m_cbCardIndex[wMeChairID],m_WeaveItemArray[wMeChairID],cbWeaveCount,GangCardResult);
	}
	else
	{
		GangCardResult.cbCardCount=1;
		GangCardResult.cbCardData[0]=m_cbActionCard;

	}
	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;

	BYTE cbValue=cbOperateCard&MASK_VALUE;
	
	// ���µ� OperateCode��Ҫʱ��ѡ����
	if((cbOperateCard!=0)&&(cbOperateCode&WIK_GANG))
	{
		OperateCard.cbOperateCode=WIK_GANG;
		OperateCard.cbOperateCard[0]=cbOperateCard;
		OperateCard.cbOperateCard[1]=cbOperateCard;
		OperateCard.cbOperateCard[2]=cbOperateCard;
	}
	else if((cbOperateCard!=0)&&(cbOperateCode&WIK_PENG))
	{
		OperateCard.cbOperateCode=WIK_PENG;
		OperateCard.cbOperateCard[0]=cbOperateCard;
		OperateCard.cbOperateCard[1]=cbOperateCard;
		OperateCard.cbOperateCard[2]=cbOperateCard;
	}
	else if(cbOperateCode&WIK_LEFT )
	{
		OperateCard.cbOperateCode=WIK_LEFT;
		OperateCard.cbOperateCard[0]=cbOperateCard;
		OperateCard.cbOperateCard[1]=cbOperateCard+1;
		OperateCard.cbOperateCard[2]=cbOperateCard+2;		
	}
	else if(cbOperateCode&WIK_CENTER )
	{
		OperateCard.cbOperateCode=WIK_CENTER;
		OperateCard.cbOperateCard[0]=cbOperateCard;
		OperateCard.cbOperateCard[1]=cbOperateCard-1;
		OperateCard.cbOperateCard[2]=cbOperateCard+1;	
	}
	else if(cbOperateCode&WIK_RIGHT )
	{
		OperateCard.cbOperateCode=WIK_RIGHT;
		OperateCard.cbOperateCard[0]=cbOperateCard;
		OperateCard.cbOperateCard[1]=cbOperateCard-1;
		OperateCard.cbOperateCard[2]=cbOperateCard-2;		
	}
	else
	{
		OperateCard.cbOperateCard[0]=cbOperateCard;
		OperateCard.cbOperateCard[1]=cbOperateCard;
		OperateCard.cbOperateCard[2]=cbOperateCard;
	}

	if( !m_pIAndroidUserItem->SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard)) )
	{
		ASSERT( FALSE );
		return ;
	}

	//��������
	m_bActionMask = WIK_NULL;
	m_cbActionCard=0;
	return ;
}
//���ܴ���
BYTE CAndroidUserItemSink::GetIsolatedCard()
{

	//�������
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	int i=0;
	
	//�з���
	for( i=27; i<27+7; i++)
	{
		if(m_cbCardIndex[wMeChairID][i]==1 && m_GameLogic.GetMagicIndex()!=i)
		{
			return m_GameLogic.SwitchToCardData(i);
		}
	}
	//�й��ƴ����
	//��19���ƣ���Ҫ�������
	for(i=0; i<3; i++)
	{
		if(m_cbCardIndex[wMeChairID][i*9]==1 && m_GameLogic.GetMagicIndex()!=i*9)
		{
			if((m_cbCardIndex[wMeChairID][i*9+1]==0)
				&&(m_cbCardIndex[wMeChairID][i*9+2]==0))
			{

				return m_GameLogic.SwitchToCardData(i*9);
			}
		}
		if(m_cbCardIndex[wMeChairID][i*9+8]==1 && m_GameLogic.GetMagicIndex()!=(i*9+8))
		{
			if((m_cbCardIndex[wMeChairID][i*9+7]==0)
				&&(m_cbCardIndex[wMeChairID][i*9+6]==0))
			{
				return m_GameLogic.SwitchToCardData(i*9+8);
			}
		}
	}
	//����ͨ����
	for( i=0; i<27; i++)
	{
		if(m_cbCardIndex[wMeChairID][i]==1 && m_GameLogic.GetMagicIndex()!=(i))
		{
			BYTE cbTmp = i%9;
			switch(cbTmp)
			{
			case 0:
				if((m_cbCardIndex[wMeChairID][i+1]==0)
					&& (m_cbCardIndex[wMeChairID][i+2]==0))
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			case 1:
				if((m_cbCardIndex[wMeChairID][i+1]==0)
					&&(m_cbCardIndex[wMeChairID][i+2]==0)
					&&(m_cbCardIndex[wMeChairID][i-1]==0))
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				if((m_cbCardIndex[wMeChairID][i+1]==0)
					&&(m_cbCardIndex[wMeChairID][i+2]==0)
					&&(m_cbCardIndex[wMeChairID][i-1]==0)
					&& (m_cbCardIndex[wMeChairID][i-2]==0))
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			case 7:
				if((m_cbCardIndex[wMeChairID][i-1]==0)
					&&(m_cbCardIndex[m_pIAndroidUserItem->GetChairID()][i-2]==0)
					&&(m_cbCardIndex[m_pIAndroidUserItem->GetChairID()][i+1]==0))
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			case 8:
				if((m_cbCardIndex[wMeChairID][i-1]==0)
					&&(m_cbCardIndex[m_pIAndroidUserItem->GetChairID()][i-2]==0))
				{
					return m_GameLogic.SwitchToCardData(i);
				}
				break;
			}
			if(cbTmp < 3)
			{
				i += 5;
			}
			else
			{
				i = (i/9+1)*9 - 1;
			}
		}	
	}

	//����
	//��19����,��Ҫ�������
	for(i=0; i<3; i++)
	{
		if(m_cbCardIndex[wMeChairID][i*9] == 1 && m_GameLogic.GetMagicIndex()!=(i*9))
		{
			if(m_cbCardIndex[wMeChairID][i*9+1] == 0
				|| m_cbCardIndex[wMeChairID][i*9+2] == 0)
			{
				return m_GameLogic.SwitchToCardData(i*9);
			}
		}
		if(m_cbCardIndex[wMeChairID][i*9+8] == 1 && m_GameLogic.GetMagicIndex()!=(i*9+8))
		{
			if(m_cbCardIndex[wMeChairID][i*9+7] == 0
				|| m_cbCardIndex[wMeChairID][i*9+6] == 0)
			{
				return m_GameLogic.SwitchToCardData(i*9+8);
			}
		}
	}

	//����ͨ����
	for(i=0; i<27; i++)
	{
		if(m_cbCardIndex[wMeChairID][i] == 1 && m_GameLogic.GetMagicIndex()!=i)
		{
			if((i%9) == 0)
			{
				i += 3;
				continue;
			}
			if((i+1)%9 == 0)
			{
				continue;
			}
			if(m_cbCardIndex[wMeChairID][i+1] == 0
				&& m_cbCardIndex[wMeChairID][i-1] == 0)
			{
				return m_GameLogic.SwitchToCardData(i);
			}
			i += 1;
		}	
	}


	return  0XFF;
}
//������Ŀ
BYTE CAndroidUserItemSink::GetIsolatedCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//��ʱ����
	BYTE cbTempCardIndex[MAX_INDEX];
	CopyMemory(cbTempCardIndex,cbCardIndex,sizeof(cbTempCardIndex));

	//�������
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	int i=0;
	int iIsolateCardCount=0;

	//�з���
	for( i=27; i<27+7; i++)
	{
		if(cbTempCardIndex[i]==1)
		{
			iIsolateCardCount++;
		}
	}
	//�й��ƴ����
	//��19����
	for(i=0; i<3; i++)
	{
		if(cbTempCardIndex[i*9]==1)
		{
			if((cbTempCardIndex[i*9+1]==0)
				&&(cbTempCardIndex[i*9+2]==0))
			{
				iIsolateCardCount++;
			}
		}
		if(cbTempCardIndex[i*9+8]==1)
		{
			if((cbTempCardIndex[i*9+7]==0)
				&&(cbTempCardIndex[i*9+6]==0))
			{
				iIsolateCardCount++;
			}
		}
	}
	//����ͨ����
	for( i=0; i<27; i++)
	{
		if(cbTempCardIndex[i]==1)
		{
			BYTE cbTmp = i%9;
			switch(cbTmp)
			{
			case 0:
				if((cbTempCardIndex[i+1]==0)
					&& (cbTempCardIndex[i+2]==0))
				{
					iIsolateCardCount++;
				}
				break;
			case 1:
				if((cbTempCardIndex[i+1]==0)
					&&(cbTempCardIndex[i+2]==0)
					&&(cbTempCardIndex[i-1]==0))
				{
					iIsolateCardCount++;
				}
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				if((cbTempCardIndex[i+1]==0)
					&&(cbTempCardIndex[i+2]==0)
					&&(cbTempCardIndex[i-1]==0)
					&& (cbTempCardIndex[i-2]==0))
				{
					iIsolateCardCount++;
				}
				break;
			case 7:
				if((cbTempCardIndex[i-1]==0)
					&&(cbTempCardIndex[i-2]==0)
					&&(cbTempCardIndex[i+1]==0))
				{
					iIsolateCardCount++;
				}
				break;
			case 8:
				if((cbTempCardIndex[i-1]==0)
					&&(cbTempCardIndex[i-2]==0))
				{
					iIsolateCardCount++;
				}
				break;
			}
			if(cbTmp < 3)
			{
				i += 5;
			}
			else
			{
				i = (i/9+1)*9 - 1;
			}
		}	
	}

	//����
	//��19����
	for(i=0; i<3; i++)
	{
		if(cbTempCardIndex[i*9] == 1)
		{
			if(cbTempCardIndex[i*9+1] == 0
				|| cbTempCardIndex[i*9+2] == 0)
			{
				iIsolateCardCount++;
				
			}
		}
		if(cbTempCardIndex[i*9+8] == 1)
		{
			if(cbTempCardIndex[i*9+7] == 0
				|| cbTempCardIndex[i*9+6] == 0)
			{
				iIsolateCardCount++;
			}
		}
	}

	//����ͨ����
	for(i=0; i<27; i++)
	{
		if(cbTempCardIndex[i] == 1)
		{
			if((i%9) == 0)
			{
				i += 3;
				continue;
			}
			if((i+1)%9 == 0)
			{
				continue;
			}
			if(cbTempCardIndex[i+1] == 0
				&& cbTempCardIndex[i-1] == 0)
			{
				iIsolateCardCount++;
			}
			i += 1;
		}	
	}

	return  iIsolateCardCount;

}
BYTE CAndroidUserItemSink::GetTheBestOperate(BYTE cbOperateCode,BYTE cbOperateCard)
{
	//�������
	BYTE cbPreIsolateCardCount=GetIsolatedCardCount(m_cbCardIndex[m_pIAndroidUserItem->GetChairID()]);
	BYTE cbPengIsolateCardCount=0XFF;
	BYTE cbLeftIsolateCardCount=0XFF;
	BYTE cbCenterIsolateCradCount=0XFF;
	BYTE cbRightIsolateCardCount=0XFF;

	//���Ʋ���
	if(cbOperateCode&WIK_PENG)
	{
		BYTE cbTempCardIndex[MAX_INDEX];
		CopyMemory(cbTempCardIndex,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],sizeof(cbTempCardIndex));

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=WIK_PENG;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
		m_GameLogic.RemoveCard(cbTempCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		cbPengIsolateCardCount=GetIsolatedCardCount(cbTempCardIndex);
	}
	//��Բ���
	if(cbOperateCode&WIK_LEFT)
	{
		BYTE cbTempCardIndex[MAX_INDEX];
		CopyMemory(cbTempCardIndex,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],sizeof(cbTempCardIndex));

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=WIK_LEFT;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
		m_GameLogic.RemoveCard(cbTempCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		cbLeftIsolateCardCount=GetIsolatedCardCount(cbTempCardIndex);
	}
	//�гԲ���
	if(cbOperateCode&WIK_CENTER)
	{
		BYTE cbTempCardIndex[MAX_INDEX];
		CopyMemory(cbTempCardIndex,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],sizeof(cbTempCardIndex));

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=WIK_CENTER;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
		m_GameLogic.RemoveCard(cbTempCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		cbCenterIsolateCradCount=GetIsolatedCardCount(cbTempCardIndex);
	}
	//�ҳԲ���
	if(cbOperateCode&WIK_RIGHT)
	{
		BYTE cbTempCardIndex[MAX_INDEX];
		CopyMemory(cbTempCardIndex,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],sizeof(cbTempCardIndex));

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=WIK_RIGHT;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
		m_GameLogic.RemoveCard(cbTempCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		cbRightIsolateCardCount=GetIsolatedCardCount(cbTempCardIndex);
	}
	//��������
	if(cbPengIsolateCardCount==0)
		return WIK_PENG;
	if(cbLeftIsolateCardCount==0)
		return WIK_LEFT;
	if(cbCenterIsolateCradCount==0)
		return WIK_CENTER;
	if(cbRightIsolateCardCount==0)
		return WIK_RIGHT;
	
	//����Ȩλ
	BYTE cbMin=0xFF;
	if(cbMin>cbPengIsolateCardCount)
		cbMin=cbPengIsolateCardCount;
	if(cbMin>cbLeftIsolateCardCount)
		cbMin=cbLeftIsolateCardCount;
	if(cbMin>cbCenterIsolateCradCount)
		cbMin=cbCenterIsolateCradCount;
	if(cbMin>cbRightIsolateCardCount)
		cbMin=cbRightIsolateCardCount;
	ASSERT(cbMin!=0xFF);
	if(cbMin>cbPreIsolateCardCount)
		return WIK_NULL;
	if(cbMin==cbPengIsolateCardCount)
		return WIK_PENG;
	if(cbMin==cbLeftIsolateCardCount)
		return WIK_LEFT;
	if(cbMin==cbCenterIsolateCradCount)
		return WIK_CENTER;
	if(cbMin==cbRightIsolateCardCount)
		return WIK_RIGHT;

	//����Ȩλ
	if((cbOperateCode&WIK_PENG)&&(cbPreIsolateCardCount-1==cbPengIsolateCardCount))
		return WIK_PENG;
	if((cbOperateCode&WIK_LEFT)&&(cbPreIsolateCardCount-1==cbLeftIsolateCardCount))
		return WIK_LEFT;
	if((cbOperateCode&WIK_CENTER)&&(cbPreIsolateCardCount-1==cbCenterIsolateCradCount))
		return WIK_CENTER;
	if((cbOperateCode&WIK_RIGHT)&&(cbPreIsolateCardCount-1==cbRightIsolateCardCount))
		return WIK_RIGHT;
	return WIK_NULL;
}
//�����ж�
bool CAndroidUserItemSink::VerdictOutCard(BYTE cbCardData)
{
	//�����ж�
	if ((m_bHearStatus==true))
	{
		//��������
		CChiHuRight ChiHuRight;
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		BYTE cbWeaveCount=m_cbWeaveCount[wMeChairID];

		//�����˿�
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,m_cbCardIndex[wMeChairID],sizeof(cbCardIndexTemp));

		//ɾ���˿�
		m_GameLogic.RemoveCard(cbCardIndexTemp,cbCardData);

		//�����ж�
		BYTE i=0;
		for ( i=0;i<MAX_INDEX;i++)
		{
			//���Ʒ���
			BYTE cbCurrentCard=m_GameLogic.SwitchToCardData(i);
			BYTE cbHuCardKind=m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp,m_WeaveItemArray[wMeChairID],cbWeaveCount,cbCurrentCard,ChiHuRight);

			//����ж�
			if (cbHuCardKind!=CHR_NULL) 
				break;
		}

		//�����ж�
		return (i!=MAX_INDEX);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
