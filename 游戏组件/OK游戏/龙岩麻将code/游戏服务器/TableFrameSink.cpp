#include "StdAfx.h"
#include "TableFrameSink.h"
#include <cmath>
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
//const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_lSiceCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
	m_cbQuanFeng = 0;
	m_cbQuanCounter = 1;
	ZeroMemory( m_cbHuaCardCount,sizeof(m_cbHuaCardCount) );
	ZeroMemory(m_cbHuaCardData,sizeof(m_cbHuaCardData));
	m_lCellScore=0L;
	m_bMustWanTong=true;
	m_wReplaceUser = INVALID_CHAIR;
	ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));
	ZeroMemory(m_cbDone,sizeof(m_cbDone));
	m_cbHu=0;

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	ZeroMemory(m_cbEnjoinHuCard,sizeof(m_cbEnjoinHuCard));
	ZeroMemory(m_cbEnjoinPengCard,sizeof(m_cbEnjoinPengCard));
	ZeroMemory(m_cbEnjoinCardCount,sizeof(m_cbEnjoinCardCount));
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));


	//������Ϣ
	m_wHeapHand = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 18;

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory( m_dwChiHuKind,sizeof(m_dwChiHuKind) );

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
		return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);//add by hxh 20170321

	return true;
}

//��λ����
void  CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_lSiceCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
	ZeroMemory( m_cbHuaCardCount,sizeof(m_cbHuaCardCount) );
	ZeroMemory(m_cbHuaCardData,sizeof(m_cbHuaCardData));
	m_lCellScore=0L;

	m_wReplaceUser = INVALID_CHAIR;

	ZeroMemory(m_cbDone,sizeof(m_cbDone));

	m_cbHu=0;

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));


	ZeroMemory(m_cbEnjoinHuCard,sizeof(m_cbEnjoinHuCard));
	ZeroMemory(m_cbEnjoinPengCard,sizeof(m_cbEnjoinPengCard));
	ZeroMemory(m_cbEnjoinCardCount,sizeof(m_cbEnjoinCardCount));
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));


	//������Ϣ
	m_wHeapHand = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 18;

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(m_dwChiHuKind,sizeof(m_dwChiHuKind));

	return;
}

////��ʼģʽ
//enStartMode  CTableFrameSink::GetGameStartMode()
//{
//	return m_GameStartMode;
//}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//�����˿�
	if( m_wBankerUser == INVALID_CHAIR )
		m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),MAKEWORD(rand()%6+1,rand()%6+1));
	else
		m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),0);


	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));
	
	LONGLONG lMax = 0;
	bool bHaveSet = false;

	for (WORD i=0;i<m_wPlayerCount;i++)
	{

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//��ȡ���� 
		//const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();
        SCORE lScore=pIServerUserItem->GetUserScore();

		if (!bHaveSet)	
		{
			lMax = lScore;
			bHaveSet = true;
		}

		if (lScore<lMax) 
			lMax=lScore;
	}
	m_lCellScore=GetMemberLevelScore(lMax);


	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	if( m_wBankerUser == INVALID_CHAIR )
	{
		WORD wSice = (WORD)(m_lSiceCount>>16);
		m_wBankerUser = (HIBYTE(wSice)+LOBYTE(wSice))%GAME_PLAYER;
	}

	//֪ͨ��Ϣ
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("���ֻ�����Ϊ%I64d"),m_lCellScore);

	//for (WORD i=0;i<m_wPlayerCount;i++)
	//{
	//	IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
	//	if (pISendUserItem!=NULL) 
	//		m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
	//}

	WORD wIndex=0;
	//do
	//{
	//	IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
	//	if (pISendUserItem==NULL)
	//		break;
	//	m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
	//} while (true);

	//֪ͨ��Ϣ
	_sntprintf(szMessage,CountArray(szMessage),TEXT("��ʼ�µ�һ��,�����ǵ�%dȦ,ׯ���� [ %s ], %d��ׯ"),m_cbQuanFeng+1,m_pITableFrame->GetTableUserItem(m_wBankerUser)->GetNickName(),m_cbBankerContinuCount[m_wBankerUser]+1);

	for (WORD i=0;i<m_wPlayerCount;i++)
	/*{
		IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pISendUserItem!=NULL) 
			m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
	}*/

	wIndex=0;
	/*do
	{
		IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
		if (pISendUserItem==NULL)
			break;
		m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
	} while (true);*/
	
	//test
	//BYTE byTest[] = {
	//	0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
	//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
	//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
	//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
	//	0x21,0x22,0x23,0x24,0x25,0x26,0x22,0x27,
	//	
	//	0x29,0x31,0x31,0x11,

	//	0x11,0x12,0x13,0x14,0x15,0x16,0x21,0x22,0x23,
	//	0x24,0x25,0x26,0x31,

	//	0x28,0x29,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
	//	0x28,0x12,0x31,0x31,

	//	0x33,0x12,0x17,0x36,0x27,0x31,0x32,0x33,0x34,
	//	0x35,0x36,0x3f,0x3f,

	//	0x27,0x32,0x32,0x32,0x31,0x31,0x31,0x34,0x34,
	//	0x34,0x34,0x36,0x31,
	//};
	//CopyMemory( m_cbRepertoryCard,byTest,sizeof(byTest) );
	//m_wBankerUser = 0;
	//end test

	//�ַ��˿�


	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_cbProvideCard=m_cbSendCardData;
	m_wProvideUser=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//������Ϣ
	WORD wSice = WORD(m_lSiceCount&0xffff);
	BYTE cbSiceTakeCount= HIBYTE(wSice)+LOBYTE(wSice);
	
	//�ϱ�����
	if(cbSiceTakeCount%2==0)
		cbSiceTakeCount += 2;

	WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
	BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ŀ
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(min(HIBYTE(wSice),LOBYTE(wSice)))*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//��ȡ�˿�
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//����ж�
		if (cbTakeCount==0)
		{
			m_wHeapHand=wTakeChairID;
			m_wHeapTail=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
			break;
		}
		//�л�����
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//��������
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�û�����
		WORD wUserIndex=(m_wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

		for( BYTE j = MAX_INDEX-MAX_HUA_CARD; j < MAX_INDEX; j++ )
		{
			if( m_cbCardIndex[wUserIndex][j] > 0 )
			{
				m_wReplaceUser = wUserIndex;
				break;
			}
		}

		if( m_wReplaceUser != INVALID_CHAIR ) break;
	}

	//ׯ���ж�
	if ( m_wReplaceUser==INVALID_CHAIR )
	{
		//�����ж�
		tagGangCardResult GangCardResult;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wBankerUser],NULL,0,GangCardResult);

		//�����ж�
		CChiHuRight chr;
		BYTE cbHu=0;
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,m_cbSendCardData,chr,cbHu,true);
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
	}

	//��������
	CMD_S_GameStart GameStart;
	GameStart.lSiceCount=m_lSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wReplaceUser = m_wReplaceUser;
	GameStart.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
	GameStart.cbQuanFeng = m_cbQuanFeng;
	GameStart.wHeapHead/*wHeapHand*/ = m_wHeapHand;
	GameStart.wHeapTail = m_wHeapTail;
	//GameStart.bMustWanTong=m_bMustWanTong;
	CopyMemory(GameStart.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		GameStart.cbUserAction=m_cbUserAction[i];
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
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
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			WORD wWinner = INVALID_CHAIR;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.dwChiHuKind[i]=m_dwChiHuKind[i];
				if( m_dwChiHuKind[i] == WIK_CHI_HU )
					wWinner = i;
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//�����������
			if( wWinner != INVALID_CHAIR )
			{
				//Ȩλ����
				FiltrateRight( wWinner,m_ChiHuRight[wWinner] );
			}

			//ͳ�ƻ���
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//������Ϣ
				GameEnd.wProvideUser[0]=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;

				m_ChiHuRight[wWinner].GetRightData( GameEnd.dwChiHuRight1,MAX_RIGHT_COUNT );

				//GetNoWinScore( wWinner,GameEnd.lGameScore,GameEnd.lHu);
				GameEnd.cbHuaCardCount = m_cbHuaCardCount[wWinner];


				//����
				if(wWinner==m_wProvideUser)
				{
					//��ׯ��ʽ
					if(m_wProvideUser==m_wBankerUser)
					{
						m_wBankerUser=m_wBankerUser;
						m_cbBankerContinuCount[m_wBankerUser]++;
					}
					else
					{
						//��ׯ�������ׯ��Ŀ
						ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));
						m_wBankerUser=(m_wBankerUser-1+m_wPlayerCount)%m_wPlayerCount;
					}
				}
				else
				{
					//û����3��
					if((!m_GameLogic.AnalyseTingCard(m_cbCardIndex[m_wProvideUser],m_WeaveItemArray[m_wProvideUser],m_cbWeaveItemCount[m_wProvideUser]))&&(IsAfford(wWinner)>1))
					{
						if(!(m_ChiHuRight[wWinner]&(CHR_QING_YI_SE|CHR_HUN_YI_SE)).IsEmpty()) //��һɫ ��һɫ�Ű���
						{
							for (WORD j=0;j<m_wPlayerCount;j++)
							{
								//��ҹ���
								if (j==wWinner) 	continue;
								if(j==m_wProvideUser) continue;
								//�������
								GameEnd.lGameScore[m_wProvideUser]+=GameEnd.lGameScore[j];
								GameEnd.lGameScore[j]=0;
							}
						}

					}

					//��ׯ��ʽ
					if(wWinner==m_wBankerUser)
					{
						m_cbBankerContinuCount[m_wBankerUser]++;
						m_wBankerUser=m_wBankerUser;
					}
					else
					{
						//��ׯ�������ׯ��Ŀ
						ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));

						m_wBankerUser=(m_wBankerUser-1+m_wPlayerCount)%m_wPlayerCount;
					}
				}

			}
			else
			{
				//���ֽ���
				GameEnd.wProvideUser[0]=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;

				m_wBankerUser = (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount;

			}
			
			//ͳ�ƻ���
			LONGLONG lGameTax[GAME_PLAYER];
			ZeroMemory(lGameTax,sizeof(lGameTax));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//���û���
				if( GAME_GENRE_GOLD == m_pGameServiceOption->wServerType )
				{
					if (GameEnd.lGameScore[i]>0L)
					{
						lGameTax[i] = GameEnd.lGameScore[i]*m_pGameServiceOption->cbRevenueRatio/1000L;
						GameEnd.lGameScore[i] -= lGameTax[i];
					}
				}
			}

			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE ScoreKind;
				if( GameEnd.lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;
				//д�����
				///m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lGameTax[i],ScoreKind);

				//������Ϣ
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			if( ++m_cbQuanCounter == 5 )
			{
				m_cbQuanCounter = 1;
				if( ++m_cbQuanFeng == 4 )
				{
					m_cbQuanFeng = 0;
				}
			}

			//������Ϸ
			///m_pITableFrame->ConcludeGame();
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.wProvideUser[0]=INVALID_CHAIR;

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			///m_pITableFrame->ConcludeGame();
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	//case GER_USER_LEFT:		//�û�ǿ��
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.wProvideUser[0]=INVALID_CHAIR;

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//����
			GameEnd.lGameScore[wChairID] = -300L*m_lCellScore;
			
			//֪ͨ��Ϣ
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetNickName());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
				if (pISendUserItem!=NULL) 
					m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
			}

			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL)
					break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
			} while (true);

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//д�����
			///m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0,SCORE_TYPE_FLEE);

			//������Ϸ
			///m_pITableFrame->ConcludeGame();
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	}

	//�������
	ASSERT(FALSE);
	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.wReplaceUser = m_wReplaceUser;
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));
			StatusPlay.cbQuanFeng = m_cbQuanFeng;
			//StatusPlay.bMustWanTong=m_bMustWanTong;

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//����״̬
			CopyMemory( StatusPlay.cbHearStatus,m_cbListenStatus,sizeof(m_cbListenStatus) );

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));


			//������Ϣ
			//StatusPlay.cbEnjoinCardCount = m_cbEnjoinCardCount[m_wCurrentUser];
			//CopyMemory(StatusPlay.cbEnjoinCardData,m_cbEnjoinCardData[m_wCurrentUser],sizeof(StatusPlay.cbEnjoinCardData));

			//������Ϣ
			StatusPlay.wHeapHead/*wHeapHand*/ = m_wHeapHand;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

			//�˿�����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);
			if( m_wReplaceUser == wChiarID ) StatusPlay.cbSendCardData = m_cbSendCardData;
			else StatusPlay.cbSendCardData=(m_wProvideUser==wChiarID)?m_cbProvideCard:0x00;
			
			//CopyMemory(StatusPlay.cbHuaCardCount,m_cbHuaCardCount,sizeof(m_cbHuaCardCount));
			//CopyMemory(StatusPlay.cbHuaCardData,m_cbHuaCardData,sizeof(m_cbHuaCardData));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//�û�Ч��
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pIServerUserItem->GetChairID(),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_REPLACE_CARD:	//�û�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ReplaceCard));
			if (wDataSize!=sizeof(CMD_C_ReplaceCard)) return false;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��������
			CMD_C_ReplaceCard * pReplaceCard=(CMD_C_ReplaceCard *)pDataBuffer;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserReplaceCard(pIServerUserItem->GetChairID(),pReplaceCard->cbCardData);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			//�û�Ч��
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			m_bTrustee[pIServerUserItem->GetChairID()]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pIServerUserItem->GetChairID();
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

			return true;
		}
	case SUB_C_LISTEN:
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_Listen));
			if (wDataSize!=sizeof(CMD_C_Listen)) return false;

			//�û�Ч��
            tagUserInfo *pUserData = pIServerUserItem->GetUserInfo();
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			CMD_C_Listen *pListen = (CMD_C_Listen *)pDataBuffer;
			return OnUserListenCard(pIServerUserItem->GetChairID(),pListen->cbListen);
		}
	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if (bLookonUser==false)
	{
		m_wBankerUser = INVALID_CHAIR;

		m_cbQuanCounter = 0;
		m_cbQuanFeng=0;
		m_bMustWanTong=true;

		m_bTrustee[wChairID]=false;
		CMD_S_Trustee Trustee;
		Trustee.bTrustee=false;
		Trustee.wChairID = wChairID;

		ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));
		
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserListenCard(WORD wChairID,BYTE cbListen)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//Ч�����
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbListenStatus[wChairID]==0));
	if ((wChairID!=m_wCurrentUser)||(m_cbListenStatus[wChairID]!=0)) return false;

	//���ñ���
	m_cbListenStatus[wChairID] = cbListen;
	m_bEnjoinChiPeng[wChairID] = true;

	//��������
	CMD_S_Listen ListenCard;
	ListenCard.wChairId=wChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�������
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);
	if(m_bMustWanTong==true)
	{
		m_cbDone[wChairID]=1;

		BYTE cbValidteOutCardCount=0;
		for(WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbDone[i]==1)
				cbValidteOutCardCount++;
		}
		if(cbValidteOutCardCount==4)
			m_bMustWanTong=false;

		if(m_bMustWanTong==false)
		{
			//������Ϣ
			//m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_WANTONG);
			//m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_WANTONG);
		}
	}

	//Ч�����
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bSendStatus=true;
	m_bGangStatus = false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//�������
	m_cbEnjoinHuCard[wChairID]=0;
	m_cbEnjoinPengCard[wChairID]=0;

	//ȡ��
	m_cbEnjoinCardCount[wChairID]=0;
	ZeroMemory(m_cbEnjoinCardData[wChairID],sizeof(BYTE)*CountArray(m_cbEnjoinCardData));

	//���Ƽ�¼
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;

	//�û��л�
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//�ɷ��˿�
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3])
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY)
		return true;

	//Ч���û�
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return false;

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		ASSERT(m_bResponse[wChairID]==false);
		ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//Ч��״̬
		if (m_bResponse[wChairID]==true) 
			return false;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return true;

		//��������
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//���ñ���
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		if(cbOperateCard[0]==0)
			m_cbOperateCard[wChairID][0] = m_cbProvideCard;
		else CopyMemory( m_cbOperateCard[wChairID],cbOperateCard,sizeof(m_cbOperateCard[wChairID]) );

		//ִ���ж�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ����
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//���ȼ���
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//�����ж�
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) 
			return true;

		//�Ժ��ȴ�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
					return true;
			}
		}

		//��������
		if (cbTargetAction==WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//�����˿�
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser][0];

		//���Ʊ���
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//���Ʋ���
		if (cbTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;

			for (WORD i=(m_wProvideUser+m_wPlayerCount-1)%GAME_PLAYER;i!=m_wProvideUser;i = (i+m_wPlayerCount-1)%GAME_PLAYER)
			{
				//�����ж�
				if ((m_cbPerformAction[i]&WIK_CHI_HU)==0)
					continue;

				//�����ж�
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_dwChiHuKind[i] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i],m_cbHu);

				//�����˿�
				if (m_dwChiHuKind[i]!=WIK_NULL) 
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					wTargetUser = i;
					break;
				}
			}

			//������Ϸ
			ASSERT(m_dwChiHuKind[wTargetUser]!=WIK_NULL);
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//����˿�
		ASSERT(m_cbWeaveItemCount[wTargetUser]<MAX_WEAVE);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		m_WeaveItemArray[wTargetUser][wIndex].cbCardData[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = m_cbOperateCard[wTargetUser][1];
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = m_cbOperateCard[wTargetUser][2];
		}
		else
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = cbTargetCard;
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = cbTargetCard;
			if( cbTargetAction & WIK_GANG )
				m_WeaveItemArray[wTargetUser][wIndex].cbCardData[3] = cbTargetCard;
		}

		//ɾ���˿�
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//���Ʋ���
			{
				//ɾ���˿�
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}
				//��ֹ�������ݺ���Ŀ
				m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard;

				//����789
				if((cbTargetCard&MASK_VALUE)<7)
					m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard+3;


				break;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//ɾ���˿�
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				//��ֹ�������ݺ���Ŀ
				m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard;

				//����12 3
				if((cbTargetCard&MASK_VALUE)>3)
					m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard-3;


				break;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//ɾ���˿�
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				//��ֹ�������ݺ���Ŀ
				m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard;				


				break;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_GANG:		//���Ʋ���
			{
				//ɾ���˿�,��������ֻ���ڷŸ�
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard)) )
				{
					ASSERT( FALSE );
					return false;
				}
				//���ƴ���
				m_cbEndLeftCount += 2;


				break;
			}
		default:
			ASSERT( FALSE );
			return false;
		}

		//if(IsAfford(wTargetUser)>1)
		//{
		//	//֪ͨ��Ϣ
		//	TCHAR szMessage[128]=TEXT("");
		//	_sntprintf(szMessage,CountArray(szMessage),TEXT(" [ %s ] ��̯"),m_pITableFrame->GetTableUserItem(wTargetUser)->GetNickName());
		//	for (WORD i=0;i<m_wPlayerCount;i++)
		//	{
		//		IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
		//		if (pISendUserItem!=NULL) 
		//			m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
		//	}

		//	WORD wIndex=0;
		//	do
		//	{
		//		IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
		//		if (pISendUserItem==NULL)
		//			break;
		//		m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
		//	} while (true);

		//}

		//������
		CMD_S_OperateResult OperateResult;
		ZeroMemory( &OperateResult,sizeof(OperateResult) );
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		OperateResult.cbOperateCard[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			CopyMemory( &OperateResult.cbOperateCard[1],&m_cbOperateCard[wTargetUser][1],2*sizeof(BYTE) );
		else if( cbTargetAction&WIK_PENG )
		{
			OperateResult.cbOperateCard[1] = cbTargetCard;
			OperateResult.cbOperateCard[2] = cbTargetCard;
		}

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//�����û�
		m_wCurrentUser=wTargetUser;

		//���ƴ���
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus = true;
			DispatchCardData(wTargetUser,true);
		}

		return true;
	}

	//��������
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return true;

		//�˿�Ч��
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard[0])==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard[0])==false)) 
			return false;

		//���ñ���
		m_bSendStatus=true;
		m_cbUserAction[m_wCurrentUser]=WIK_NULL;
		m_cbPerformAction[m_wCurrentUser]=WIK_NULL;

		bool bPublic=false;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_GANG:			//���Ʋ���
			{
				//��������
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard[0]);

				//���ƴ���
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//Ѱ�����
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
						{
							bPublic=true;
							cbWeaveIndex=i;
							break;
						}
					}

					//Ч�鶯��
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//����˿�
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[3]=cbOperateCard[0];
				}
				else
				{
					//�˿�Ч��
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
						return false;

					//���ñ���
					bPublic=false;
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					for( BYTE j = 0; j < 4; j++ ) m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[j] = cbOperateCard[0];
				}

				//ɾ���˿�
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				m_bGangStatus = true;


				//���ƴ���
				m_cbEndLeftCount += 2;

				//if(IsAfford(wChairID)>1)
				//{
				//	//֪ͨ��Ϣ
				//	TCHAR szMessage[128]=TEXT("");
				//	_sntprintf(szMessage,CountArray(szMessage),TEXT(" [ %s ] ��̯"),m_pITableFrame->GetTableUserItem(wChairID)->GetNickName());
				//	for (WORD i=0;i<m_wPlayerCount;i++)
				//	{
				//		IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
				//		if (pISendUserItem!=NULL) 
				//			m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
				//	}

				//	WORD wIndex=0;
				//	do
				//	{
				//		IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				//		if (pISendUserItem==NULL)
				//			break;
				//		m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
				//	} while (true);

				//}

				//������
				CMD_S_OperateResult OperateResult;
				ZeroMemory( &OperateResult,sizeof(OperateResult) );
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard[0]=cbOperateCard[0];

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//Ч�鶯��
				bool bAroseAction=false;
				if (bPublic==true) bAroseAction=EstimateUserRespond(wChairID,cbOperateCard[0],EstimatKind_GangCard);

				//�����˿�
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID,true);
				}

				return true;
			}
		case WIK_CHI_HU:		//�Ժ�����
			{
				//����Ȩλ
				if (m_cbOutCardCount==0)
				{
					m_wProvideUser = m_wCurrentUser;
					m_cbProvideCard = m_cbSendCardData;
				}

				//��ͨ����
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],&m_cbProvideCard,1) )
				{
					ASSERT( FALSE );
					return false;
				}
				m_dwChiHuKind[wChairID] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,
					m_ChiHuRight[wChairID],m_cbHu,true);
				m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;

				//������Ϣ
				m_cbChiHuCard=m_cbProvideCard;

				//������Ϸ
				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//�û�����
bool CTableFrameSink::OnUserReplaceCard(WORD wChairID, BYTE cbCardData)
{
	//�������
	ASSERT(wChairID==m_wReplaceUser);
	ASSERT(m_GameLogic.IsHuaCard(cbCardData)==true);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
	if (wChairID!=m_wReplaceUser) return false;
	if (m_GameLogic.IsHuaCard(cbCardData)==false) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false)  return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//״̬����
	m_bSendStatus=true;
	m_cbSendCardData=0x00;

	//��������ͳ��
	m_cbHuaCardData[wChairID][m_cbHuaCardCount[wChairID]]=cbCardData;
	m_cbHuaCardCount[wChairID]++;


	//��������
	CMD_S_ReplaceCard ReplaceCard;
	ReplaceCard.wReplaceUser=wChairID;
	ReplaceCard.cbReplaceCard=cbCardData;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

	//�ɷ��˿�
	DispatchCardData(wChairID,true);

	return true;
}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
	//������ʾ
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//��������
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bTail)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;
	ASSERT( m_bSendStatus );
	if( m_bSendStatus == false ) return false;

	//�����˿�
	if ((m_wReplaceUser==INVALID_CHAIR)&&(m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//��ׯ����
	if (m_cbLeftCardCount==m_cbEndLeftCount)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//���ñ���
	if( m_wReplaceUser == INVALID_CHAIR )
	{
		m_cbOutCardData=0;
		m_wCurrentUser=wCurrentUser;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	//����
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//�����ж�
	if ((m_wReplaceUser!=INVALID_CHAIR)||(m_GameLogic.IsHuaCard(m_cbSendCardData)==true))
	{
		m_wReplaceUser=INVALID_CHAIR;

		//�û��ж�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//�û�����
			WORD wUserIndex=(wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

			//�˿��ж�
			for (BYTE j=MAX_INDEX-MAX_HUA_CARD;j<MAX_INDEX;j++)
			{
				if (m_cbCardIndex[wUserIndex][j]>0)
				{
					m_wReplaceUser=wUserIndex;
					break;
				}
			}

			if( m_wReplaceUser != INVALID_CHAIR ) break;
		}
	}

	if( m_wCurrentUser == wCurrentUser )
	{
		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;
	}

	if( m_wReplaceUser == INVALID_CHAIR )
	{
		if( !m_bEnjoinChiHu[m_wCurrentUser] )
		{
			//�����ж�
			CChiHuRight chr;
			BYTE cbHu=0;
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
			m_cbUserAction[m_wCurrentUser] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wCurrentUser],m_WeaveItemArray[m_wCurrentUser],
				m_cbWeaveItemCount[m_wCurrentUser],m_cbProvideCard,chr,cbHu,true);
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
		}

		//�����ж�
		if ((m_bEnjoinChiPeng[m_wCurrentUser]==false)&&(m_cbLeftCardCount>m_cbEndLeftCount))
		{
			tagGangCardResult GangCardResult;
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);
		}
	}

	//������Ϣ
	ASSERT( m_wHeapHand != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR );
	if( !bTail )
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapHand][0]++;
	}
	else
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}

	//��������
	CMD_S_SendCard SendCard;
	SendCard.cbSendCardUser/*wSendCardUser*/ = wCurrentUser;
	SendCard.wReplaceUser = m_wReplaceUser;
	SendCard.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
	SendCard.bTail = bTail;
	SendCard.cbActionMask=m_wReplaceUser==INVALID_CHAIR?m_cbUserAction[m_wCurrentUser]:WIK_NULL;
	SendCard.cbCardData=(m_wCurrentUser==wCurrentUser)?m_cbProvideCard:m_cbSendCardData;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY)
	{
		setChairTrustee(wChairID,true);
	}
	return true;
}
//�û�����
bool CTableFrameSink::OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY)
	{
		setChairTrustee(wChairID,false);
	}
	return true;
}
//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//��������
	bool bAroseAction=false;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//���ܳ�����
	if(m_bMustWanTong==true) return false;

	if(m_cbDone[wCenterUser]==1 && m_cbDiscardCount[wCenterUser]==0 && m_bMustWanTong==false) return false;

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�û�����
		if (wCenterUser==i) continue;

		//��������
		if (EstimatKind==EstimatKind_OutCard)
		{
			//�����ж�
			if (m_bEnjoinChiPeng[i]==false)
			{
				//�����ж�
				if(cbCenterCard != m_cbEnjoinPengCard[i])
				{
					m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);
				}
				//��ֹ����
				if(m_cbUserAction[i]&WIK_PENG)
				{
					m_cbEnjoinPengCard[i]=cbCenterCard;
				}	

				//�����ж�
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i)
					m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				if (m_cbLeftCardCount>m_cbEndLeftCount) 
				{
					m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				}
			}
		}

		//�����ж�
		if (m_bEnjoinChiHu[i]==false )
		{
			//�Ժ��ж�
			CChiHuRight chr;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			if(m_cbEnjoinHuCard[i] != cbCenterCard)
			{
				BYTE cbHu=0;
				BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr,cbHu);
				m_cbUserAction[i] |= cbAction;
			}
		}

		//��ֹ����
		if(m_cbUserAction[i]&WIK_CHI_HU)
		{
			m_cbEnjoinHuCard[i]=cbCenterCard;
		}

		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
	}

	//�������
	if (bAroseAction==true) 
	{
		//���ñ���
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//������ʾ
		SendOperateNotify();

		return true;
	}

	return false;
}

//��ϵͳ���ƻ�ȡ��������
BYTE CTableFrameSink::GetMagicIndex( BYTE cbFanCard )
{
	BYTE cbZPaiIndex = m_GameLogic.SwitchToCardIndex(cbFanCard);
	if( cbZPaiIndex < 27 )
	{
		if( (cbZPaiIndex+1)%9 == 0 )
			cbZPaiIndex -= 8;
		else
			cbZPaiIndex++;
	}
	else
	{
		if( cbZPaiIndex + 1 == MAX_INDEX )
			cbZPaiIndex = 27;
		else
			cbZPaiIndex++;
	}
	return cbZPaiIndex;
}

//��ȡ��������
BYTE CTableFrameSink::GetDiscardCount( BYTE cbCardData )
{
	BYTE cbCount = 0;
	//����������
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		for( BYTE j = 0; j < m_cbDiscardCount[i]; j++ )
		{
			if( m_cbDiscardCard[i][j] == cbCardData )
				if( ++cbCount == 4 ) break;
		}
	}
	if( cbCount < 4 )
	{
		//������
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			for( BYTE j = 0; j < m_cbWeaveItemCount[i]; j++ )
			{
				if( !m_WeaveItemArray[i][j].cbPublicCard ) continue;
				BYTE cbCardBuffer[4];
				BYTE cbCardCount = m_GameLogic.GetWeaveCard( m_WeaveItemArray[i][j].cbWeaveKind,
					m_WeaveItemArray[i][j].cbCenterCard,cbCardBuffer );
				for( BYTE k = 0; k < cbCardCount; k++ )
					if( cbCardBuffer[k] == cbCardData )
						if( ++cbCount == 4 ) break;
			}
		}
	}

	return cbCount;
}

//Ȩλ����
void CTableFrameSink::FiltrateRight( WORD wWinner, CChiHuRight &chr )
{
	/*	����Ȩλ	*/

	//����
	if( wWinner == m_wProvideUser )
		chr |= CHR_ZI_MO;

}
//���3�� ����
BYTE CTableFrameSink::IsAfford(WORD wUserAccept)
{
	//У������
	ASSERT(wUserAccept<4);
	ASSERT(wUserAccept>=0);

	if(m_cbWeaveItemCount[wUserAccept]<3) return 1;

	BYTE cbAfford=0;//1�����ǰ���ϵ 2������ϵ
	BYTE cbCount[4] = {0,0,0,0};
	BYTE bAllKe=true;
	//������
	for (BYTE cbTemp =0;cbTemp<m_cbWeaveItemCount[wUserAccept];cbTemp++)
	{
		if(m_WeaveItemArray[wUserAccept][cbTemp].cbWeaveKind&(WIK_LEFT|WIK_RIGHT|WIK_RIGHT))
		{
			bAllKe=false;
			break;
		}
		cbCount[(m_WeaveItemArray[wUserAccept][cbTemp].cbCenterCard&MASK_COLOR)>>4]++;
	}
	if(bAllKe==false) return 1;
	if((cbCount[0]+cbCount[3]>=3)||(cbCount[1]+cbCount[3]>=3) ||(cbCount[2]+cbCount[3]>=3))
		cbAfford+=2;

	return cbAfford>1?cbAfford:1;
}

//���
void CTableFrameSink::GetNoWinScore(WORD wWinner,LONGLONG lScore[GAME_PLAYER],LONGLONG lHu[GAME_PLAYER])
{
	ZeroMemory(lScore,sizeof(lScore)*GAME_PLAYER);
	ZeroMemory(lHu,sizeof(lHu)*GAME_PLAYER);

	LONGLONG cbHu[4];
	LONGLONG cbHua[4];
	LONGLONG lAllScore[4];

	ZeroMemory(cbHu,sizeof(cbHu));
	ZeroMemory(cbHua,sizeof(cbHua));
	ZeroMemory(lAllScore,sizeof(lAllScore));

	if(!(m_ChiHuRight[wWinner]&CHR_ZI_MO).IsEmpty())  //������
	{
		cbHu[wWinner] += 2;
	}

	if(!(m_ChiHuRight[wWinner]&CHR_KAN_ZHANG).IsEmpty()) //���к�
	{
		cbHu[wWinner] += 2;
	}
	cbHu[wWinner] += 30;
	cbHu[wWinner] += m_cbHu;
	cbHua[wWinner] +=m_GameLogic.GetChiHuActionRank(m_ChiHuRight[wWinner]);

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ͷ���
		for (BYTE j=0;j<m_cbWeaveItemCount[i];j++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
			if(cbWeaveKind==WIK_PENG)
			{
				if((m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR)==0x30)
				{
					cbHu[i] +=4;
				}
				else
				{
					if((m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x01 ||(m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x09)
					{
						cbHu[i] +=4;
					}
					else
						cbHu[i] +=2;
				}
			}
			if(cbWeaveKind==WIK_GANG)
			{
				if(m_WeaveItemArray[i][j].cbPublicCard==FALSE)
				{
					if((m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR)==0x30)
					{
						cbHu[i] +=32;
					}
					else
					{
						if((m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x01 ||(m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x09)
						{
							cbHu[i] +=32;
						}
						else
							cbHu[i] +=16;
					}
				}
				if(m_WeaveItemArray[i][j].cbPublicCard==TRUE)
				{
					if((m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR)==0x30)
					{
						cbHu[i] +=16;
					}
					else
					{
						if((m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x01 ||(m_WeaveItemArray[i][j].cbCenterCard&MASK_VALUE)==0x09)
						{
							cbHu[i] +=16;
						}
						else
							cbHu[i] +=8;
					}
				}
			}

			if(m_WeaveItemArray[i][j].cbCenterCard>=0x35 &&m_WeaveItemArray[i][j].cbCenterCard<=0x37)
			{
				cbHua[i]+=1;
			}
			if(i == m_wBankerUser && m_WeaveItemArray[i][j].cbCenterCard==0x31)
			{
				cbHua[i] += 1;
			}
			if((i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount) && m_WeaveItemArray[i][j].cbCenterCard==0x32)
			{
				cbHua[i] += 1;
			}
			if((i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount) && m_WeaveItemArray[i][j].cbCenterCard==0x33)
			{
				cbHua[i] += 1;
			}
			if((i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount) && m_WeaveItemArray[i][j].cbCenterCard==0x34)
			{
				cbHua[i] += 1;
			}
		}

		//����
		if(i == m_wBankerUser)//����λ
		{
			if(m_cbHuaCardCount[i] == 8)
			{
				cbHua[i] += 4;
				cbHu[i] += 32;
			}
			else
			{
				cbHu[i] += 4*m_cbHuaCardCount[i];
				m_GameLogic.SortCardList(m_cbHuaCardData[i],m_cbHuaCardCount[i]);
				//���� Ұ��
				for (INT bTemp = 0;bTemp<m_cbHuaCardCount[i];bTemp++)
				{
					//÷ �� ����
					if((m_cbHuaCardData[i][bTemp]==0x38)||(m_cbHuaCardData[i][bTemp]==0x3C))
					{
						cbHua[i] +=1;
						if(bTemp+3<m_cbHuaCardCount[i])
						{
							if(m_cbHuaCardData[i][bTemp]==0x38 &&m_cbHuaCardData[i][bTemp+3]==0x3B)
								cbHua[i]++;
							if(m_cbHuaCardData[i][bTemp]==0x3C &&m_cbHuaCardData[i][bTemp+3]==0x3F)
								cbHua[i]++;

						}
					}
				}
			}
		}

		if(i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//�Ϸ�λ
		{
			if(m_cbHuaCardCount[i] == 8)
			{
				cbHua[i] += 4;
				cbHu[i] += 32;
			}
			else
			{
				cbHu[i] += 4*m_cbHuaCardCount[i];
				m_GameLogic.SortCardList(m_cbHuaCardData[i],m_cbHuaCardCount[i]);
				for (INT bTemp = 0;bTemp<m_cbHuaCardCount[i];bTemp++)
				{
					//�� �� ����
					if((m_cbHuaCardData[i][bTemp]==0x39)||(m_cbHuaCardData[i][bTemp]==0x3D))
						cbHua[i]++;

					if(bTemp+2<m_cbHuaCardCount[i]&&bTemp>=1)
					{
						if(m_cbHuaCardData[i][bTemp]==0x39 &&m_cbHuaCardData[i][bTemp+2]==0x3B&&m_cbHuaCardData[i][bTemp-1]==0x38)
							cbHua[i]++;
						if(m_cbHuaCardData[i][bTemp]==0x3D &&m_cbHuaCardData[i][bTemp+2]==0x3F&&m_cbHuaCardData[i][bTemp-1]==0x3C)
							cbHua[i]++;

					}
				}


			}
		}
		if(i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//����λ
		{

			if(m_cbHuaCardCount[i] == 8)
			{
				cbHua[i] += 4;
				cbHu[i] += 32;
			}
			else
			{
				cbHu[i] += 4*m_cbHuaCardCount[i];
				m_GameLogic.SortCardList(m_cbHuaCardData[i],m_cbHuaCardCount[i]);
				//���� Ұ��
				for (INT bTemp = 0;bTemp<m_cbHuaCardCount[i];bTemp++)
				{

					//÷ �� ����
					if((m_cbHuaCardData[i][bTemp]==0x3A)||(m_cbHuaCardData[i][bTemp]==0x3E))
						cbHua[i] +=1;
					if(bTemp+1<m_cbHuaCardCount[i]&&bTemp>=2)
					{
						if(m_cbHuaCardData[i][bTemp]==0x3A &&m_cbHuaCardData[i][bTemp+1]==0x3B &&m_cbHuaCardData[i][bTemp-2]==0x38)
							cbHua[i]++;
						if(m_cbHuaCardData[i][bTemp]==0x3E &&m_cbHuaCardData[i][bTemp+1]==0x3F&&m_cbHuaCardData[i][bTemp-2]==0x3C)
							cbHua[i]++;

					}
				}
			}

		}
		if(i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//����λ
		{

			if(m_cbHuaCardCount[i] == 8)
			{
				cbHua[i] += 4;
				cbHu[i] += 32;
			}
			else
			{
				cbHu[i] += 4*m_cbHuaCardCount[i];
				m_GameLogic.SortCardList(m_cbHuaCardData[i],m_cbHuaCardCount[i]);
				//���� Ұ��
				for (INT bTemp = 0;bTemp<m_cbHuaCardCount[i];bTemp++)
				{

					//÷ �� ����
					if((m_cbHuaCardData[i][bTemp]==0x3B)||(m_cbHuaCardData[i][bTemp]==0x3F))
						cbHua[i] +=1;
					if(bTemp-3<m_cbHuaCardCount[i])
					{
						if(m_cbHuaCardData[i][bTemp]==0x3B &&m_cbHuaCardData[i][bTemp-3]==0x38)
							cbHua[i]++;
						if(m_cbHuaCardData[i][bTemp]==0x3F &&m_cbHuaCardData[i][bTemp-3]==0x3C)
							cbHua[i]++;

					}
				}


			}
		}

		
		if(i!=wWinner)
		{
			//�� �� Ͳ ��������
			for(BYTE j=0;j<MAX_INDEX-8-3;j++)
			{
				if(m_cbCardIndex[i][j]>=3)
				{
					if((j%9==8 || j%9==0)&&j<MAX_INDEX-15)
					{
						cbHu[i] += 8;
					}
					else if((j%9!=8 || j%9!=0)&&j<MAX_INDEX-15)
					{
						cbHu[i] += 4;
					}
					else
					{
						cbHu[i] += 8;
					}
				}

			}

		
			//�з���
			for(BYTE j=MAX_INDEX-3-8;j<MAX_INDEX-8;j++)
			{
				if(m_cbCardIndex[i][j]==2)
				{
					cbHu[i] += 2;

				}
				if(m_cbCardIndex[i][j]>=3)
				{
					cbHua[i] += 1;
					cbHu[i] += 8;
				}
			}
		}

		//ׯ���з���
		if(i==wWinner)
		{
			for(BYTE j=MAX_INDEX-3-8;j<MAX_INDEX-8;j++)
			{
				if(m_cbCardIndex[wWinner][j]==2)
				{
					cbHu[wWinner] += 2;

				}
				if(m_cbCardIndex[wWinner][j]>=3)
				{
					cbHua[wWinner] += 1;
				}
			}
		}
		//��
		if(i == m_wBankerUser)//����λ
		{
			if(m_cbCardIndex[i][27]==2)
				cbHu[i] += 2;

			if(m_cbCardIndex[i][27]>=3)
				cbHua[i] += 1;


		}

		if(i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//�Ϸ�λ
		{
			if(m_cbCardIndex[i][28]==2)
				cbHu[i] += 2;
			if(m_cbCardIndex[i][28]>=3)
				cbHua[i] += 1;


		}
		if(i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//����λ
		{
			if(m_cbCardIndex[i][29]==2)
				cbHu[i] += 2;
			if(m_cbCardIndex[i][29]>=3)
				cbHua[i] += 1;


		}
		if(i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//����λ
		{
			if(m_cbCardIndex[i][30]==2)
				cbHu[i] += 2;	
			if(m_cbCardIndex[i][30]>=3)
				cbHua[i] += 1;

		}

		LONGLONG  lAllTai=cbHu[i]*int(powf(2,int(cbHua[i])));
		if(lAllTai%10)
		{
			lAllTai -= lAllTai%10;
			lAllTai += 10;
		}

		if(lAllTai>300)
			lAllTai=300;

		lAllScore[i]=lAllTai;
		lHu[i]=lAllTai;
	}

		
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(i==wWinner) continue;
		for (WORD j=0;j<m_wPlayerCount;j++)
		{
			if(j==i || j==wWinner)  continue;

			if(i==m_wBankerUser||j==m_wBankerUser)
			{
				lScore[i] +=(lAllScore[i]-lAllScore[j]);
			}
			else
			{
				LONGLONG lTempScore=(lAllScore[i]-lAllScore[j])/2;
				if(lTempScore%10)
				{
					if(lTempScore>0)
					{
						lTempScore -= lTempScore%10;
						lTempScore += 10;
					}
					else
					{
						lTempScore -= lTempScore%10;
						lTempScore += -10;

					}
				}
				lScore[i] +=lTempScore;
			}

		}
	}

	if(!(m_ChiHuRight[wWinner]&CHR_ZI_MO).IsEmpty())
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(i==wWinner) continue;
			lScore[wWinner] += lAllScore[wWinner];
			lScore[i] -= lAllScore[wWinner];
		}
	}
	else
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(i==wWinner) continue;
			if(i==m_wProvideUser)
			{
				lScore[wWinner] += lAllScore[wWinner];
				lScore[i] -= lAllScore[wWinner];
			}
			else
			{
				LONGLONG  lTempScore=lAllScore[wWinner]/2;
				if(lTempScore%10)
				{
					if(lTempScore>0)
					{
						lTempScore -= lTempScore%10;
						lTempScore += 10;
					}
					else
					{
						lTempScore -= lTempScore%10;
						lTempScore += -10;

					}
				}
				lScore[wWinner] += lTempScore;
				lScore[i] -= lTempScore;
			}

		}
	}

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		lScore[i] *=m_lCellScore;
	}


}

//��ȡ�ȼ�
LONGLONG CTableFrameSink::GetMemberLevelScore(LONGLONG lMinScore)
{
	if(lMinScore<90)  return 1;
	else if(lMinScore<378) return 2;
	else if(lMinScore<3066) return 4;
	else if(lMinScore<24570) return 10;
	else if(lMinScore<200000) return 40;
	else if(lMinScore<400000) return 100;
	else if(lMinScore<1000000) return 200;
	else return 500;

}

bool CTableFrameSink::isUseTuoGuan()
{
	if (m_pITableFrame->GetGameServiceOption()->wServerType == GAME_GENRE_EDUCATE)
	{
		return false;
	}
	return true;
}

void CTableFrameSink::setChairTrustee(WORD iChairID,bool bTrustee)
{
	if (!isUseTuoGuan())
	{
		return;
	}
	if (iChairID >= GAME_PLAYER)
	{
		return;
	}
	m_bTrustee[iChairID]=bTrustee;

	CMD_S_Trustee Trustee;
	Trustee.bTrustee = bTrustee;
	Trustee.wChairID = iChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
}

//////////////////////////////////////////////////////////////////////////
////////////////
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0;
}
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0;
}
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	return false;
}
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}
void CTableFrameSink::SetGameBaseScore(LONG lBaseScore)
{
	return;
}
bool CTableFrameSink::OnActionUserOnReady(WORD,IServerUserItem *,void *,WORD)
{
	return false;
}
//////////////////////////////////////////////////////////////////////////
