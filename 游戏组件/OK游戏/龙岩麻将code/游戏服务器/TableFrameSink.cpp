#include "StdAfx.h"
#include "TableFrameSink.h"
#include <cmath>
//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
//const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
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

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	ZeroMemory(m_cbEnjoinHuCard,sizeof(m_cbEnjoinHuCard));
	ZeroMemory(m_cbEnjoinPengCard,sizeof(m_cbEnjoinPengCard));
	ZeroMemory(m_cbEnjoinCardCount,sizeof(m_cbEnjoinCardCount));
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));


	//堆立信息
	m_wHeapHand = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 18;

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory( m_dwChiHuKind,sizeof(m_dwChiHuKind) );

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
		return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);//add by hxh 20170321

	return true;
}

//复位桌子
void  CTableFrameSink::RepositionSink()
{
	//游戏变量
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

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));


	ZeroMemory(m_cbEnjoinHuCard,sizeof(m_cbEnjoinHuCard));
	ZeroMemory(m_cbEnjoinPengCard,sizeof(m_cbEnjoinPengCard));
	ZeroMemory(m_cbEnjoinCardCount,sizeof(m_cbEnjoinCardCount));
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));


	//堆立信息
	m_wHeapHand = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 18;

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory(m_dwChiHuKind,sizeof(m_dwChiHuKind));

	return;
}

////开始模式
//enStartMode  CTableFrameSink::GetGameStartMode()
//{
//	return m_GameStartMode;
//}

//游戏状态
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//混乱扑克
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

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//获取积分 
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

	//通知消息
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("本局基础分为%I64d"),m_lCellScore);

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

	//通知消息
	_sntprintf(szMessage,CountArray(szMessage),TEXT("开始新的一局,现在是第%d圈,庄家是 [ %s ], %d连庄"),m_cbQuanFeng+1,m_pITableFrame->GetTableUserItem(m_wBankerUser)->GetNickName(),m_cbBankerContinuCount[m_wBankerUser]+1);

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

	//分发扑克


	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//设置变量
	m_cbProvideCard=m_cbSendCardData;
	m_wProvideUser=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//堆立信息
	WORD wSice = WORD(m_lSiceCount&0xffff);
	BYTE cbSiceTakeCount= HIBYTE(wSice)+LOBYTE(wSice);
	
	//南北调换
	if(cbSiceTakeCount%2==0)
		cbSiceTakeCount += 2;

	WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
	BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//计算数目
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(min(HIBYTE(wSice),LOBYTE(wSice)))*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//提取扑克
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//完成判断
		if (cbTakeCount==0)
		{
			m_wHeapHand=wTakeChairID;
			m_wHeapTail=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
			break;
		}
		//切换索引
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//动作分析
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//用户索引
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

	//庄家判断
	if ( m_wReplaceUser==INVALID_CHAIR )
	{
		//杠牌判断
		tagGangCardResult GangCardResult;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wBankerUser],NULL,0,GangCardResult);

		//胡牌判断
		CChiHuRight chr;
		BYTE cbHu=0;
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,m_cbSendCardData,chr,cbHu,true);
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
	}

	//构造数据
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

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//设置变量
		GameStart.cbUserAction=m_cbUserAction[i];
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//结束信息
			WORD wWinner = INVALID_CHAIR;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.dwChiHuKind[i]=m_dwChiHuKind[i];
				if( m_dwChiHuKind[i] == WIK_CHI_HU )
					wWinner = i;
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//如果不是流局
			if( wWinner != INVALID_CHAIR )
			{
				//权位过滤
				FiltrateRight( wWinner,m_ChiHuRight[wWinner] );
			}

			//统计积分
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//结束信息
				GameEnd.wProvideUser[0]=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;

				m_ChiHuRight[wWinner].GetRightData( GameEnd.dwChiHuRight1,MAX_RIGHT_COUNT );

				//GetNoWinScore( wWinner,GameEnd.lGameScore,GameEnd.lHu);
				GameEnd.cbHuaCardCount = m_cbHuaCardCount[wWinner];


				//自摸
				if(wWinner==m_wProvideUser)
				{
					//轮庄方式
					if(m_wProvideUser==m_wBankerUser)
					{
						m_wBankerUser=m_wBankerUser;
						m_cbBankerContinuCount[m_wBankerUser]++;
					}
					else
					{
						//换庄，清空连庄数目
						ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));
						m_wBankerUser=(m_wBankerUser-1+m_wPlayerCount)%m_wPlayerCount;
					}
				}
				else
				{
					//没听且3包
					if((!m_GameLogic.AnalyseTingCard(m_cbCardIndex[m_wProvideUser],m_WeaveItemArray[m_wProvideUser],m_cbWeaveItemCount[m_wProvideUser]))&&(IsAfford(wWinner)>1))
					{
						if(!(m_ChiHuRight[wWinner]&(CHR_QING_YI_SE|CHR_HUN_YI_SE)).IsEmpty()) //混一色 清一色才包牌
						{
							for (WORD j=0;j<m_wPlayerCount;j++)
							{
								//输家过滤
								if (j==wWinner) 	continue;
								if(j==m_wProvideUser) continue;
								//放炮算分
								GameEnd.lGameScore[m_wProvideUser]+=GameEnd.lGameScore[j];
								GameEnd.lGameScore[j]=0;
							}
						}

					}

					//轮庄方式
					if(wWinner==m_wBankerUser)
					{
						m_cbBankerContinuCount[m_wBankerUser]++;
						m_wBankerUser=m_wBankerUser;
					}
					else
					{
						//换庄，清空连庄数目
						ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));

						m_wBankerUser=(m_wBankerUser-1+m_wPlayerCount)%m_wPlayerCount;
					}
				}

			}
			else
			{
				//流局结束
				GameEnd.wProvideUser[0]=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;

				m_wBankerUser = (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount;

			}
			
			//统计积分
			LONGLONG lGameTax[GAME_PLAYER];
			ZeroMemory(lGameTax,sizeof(lGameTax));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//设置积分
				if( GAME_GENRE_GOLD == m_pGameServiceOption->wServerType )
				{
					if (GameEnd.lGameScore[i]>0L)
					{
						lGameTax[i] = GameEnd.lGameScore[i]*m_pGameServiceOption->cbRevenueRatio/1000L;
						GameEnd.lGameScore[i] -= lGameTax[i];
					}
				}
			}

			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE ScoreKind;
				if( GameEnd.lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;
				//写入积分
				///m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lGameTax[i],ScoreKind);

				//发送信息
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

			//结束游戏
			///m_pITableFrame->ConcludeGame();
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//设置变量
			GameEnd.wProvideUser[0]=INVALID_CHAIR;

			//拷贝扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			///m_pITableFrame->ConcludeGame();
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	//case GER_USER_LEFT:		//用户强退
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//设置变量
			GameEnd.wProvideUser[0]=INVALID_CHAIR;

			//拷贝扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//积分
			GameEnd.lGameScore[wChairID] = -300L*m_lCellScore;
			
			//通知消息
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("由于 [ %s ] 离开游戏，游戏结束"),pIServerUserItem->GetNickName());
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

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//写入积分
			///m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0,SCORE_TYPE_FLEE);

			//结束游戏
			///m_pITableFrame->ConcludeGame();
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);
	return false;
}

//发送场景
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//构造数据
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//游戏状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.wReplaceUser = m_wReplaceUser;
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));
			StatusPlay.cbQuanFeng = m_cbQuanFeng;
			//StatusPlay.bMustWanTong=m_bMustWanTong;

			//状态变量
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//听牌状态
			CopyMemory( StatusPlay.cbHearStatus,m_cbListenStatus,sizeof(m_cbListenStatus) );

			//历史记录
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));


			//禁牌信息
			//StatusPlay.cbEnjoinCardCount = m_cbEnjoinCardCount[m_wCurrentUser];
			//CopyMemory(StatusPlay.cbEnjoinCardData,m_cbEnjoinCardData[m_wCurrentUser],sizeof(StatusPlay.cbEnjoinCardData));

			//堆立信息
			StatusPlay.wHeapHead/*wHeapHand*/ = m_wHeapHand;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

			//扑克数据
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);
			if( m_wReplaceUser == wChiarID ) StatusPlay.cbSendCardData = m_cbSendCardData;
			else StatusPlay.cbSendCardData=(m_wProvideUser==wChiarID)?m_cbProvideCard:0x00;
			
			//CopyMemory(StatusPlay.cbHuaCardCount,m_cbHuaCardCount,sizeof(m_cbHuaCardCount));
			//CopyMemory(StatusPlay.cbHuaCardData,m_cbHuaCardData,sizeof(m_cbHuaCardData));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//定时器事件
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//出牌消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//用户效验
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pIServerUserItem->GetChairID(),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_REPLACE_CARD:	//用户补牌
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_ReplaceCard));
			if (wDataSize!=sizeof(CMD_C_ReplaceCard)) return false;

			//用户效验
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//变量定义
			CMD_C_ReplaceCard * pReplaceCard=(CMD_C_ReplaceCard *)pDataBuffer;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserReplaceCard(pIServerUserItem->GetChairID(),pReplaceCard->cbCardData);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			//用户效验
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
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_Listen));
			if (wDataSize!=sizeof(CMD_C_Listen)) return false;

			//用户效验
            tagUserInfo *pUserData = pIServerUserItem->GetUserInfo();
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			CMD_C_Listen *pListen = (CMD_C_Listen *)pDataBuffer;
			return OnUserListenCard(pIServerUserItem->GetChairID(),pListen->cbListen);
		}
	}

	return false;
}

//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户起来
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
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

//用户听牌
bool CTableFrameSink::OnUserListenCard(WORD wChairID,BYTE cbListen)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//效验参数
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbListenStatus[wChairID]==0));
	if ((wChairID!=m_wCurrentUser)||(m_cbListenStatus[wChairID]!=0)) return false;

	//设置变量
	m_cbListenStatus[wChairID] = cbListen;
	m_bEnjoinChiPeng[wChairID] = true;

	//构造数据
	CMD_S_Listen ListenCard;
	ListenCard.wChairId=wChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//错误断言
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
			//发送消息
			//m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_WANTONG);
			//m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_WANTONG);
		}
	}

	//效验参数
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置变量
	m_bSendStatus=true;
	m_bGangStatus = false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//允许胡碰
	m_cbEnjoinHuCard[wChairID]=0;
	m_cbEnjoinPengCard[wChairID]=0;

	//取消
	m_cbEnjoinCardCount[wChairID]=0;
	ZeroMemory(m_cbEnjoinCardData[wChairID],sizeof(BYTE)*CountArray(m_cbEnjoinCardData));

	//出牌记录
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;

	//用户切换
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//响应判断
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//派发扑克
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3])
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY)
		return true;

	//效验用户
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return false;

	//被动动作
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//效验状态
		ASSERT(m_bResponse[wChairID]==false);
		ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//效验状态
		if (m_bResponse[wChairID]==true) 
			return false;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return true;

		//变量定义
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//设置变量
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		if(cbOperateCard[0]==0)
			m_cbOperateCard[wChairID][0] = m_cbProvideCard;
		else CopyMemory( m_cbOperateCard[wChairID],cbOperateCard,sizeof(m_cbOperateCard[wChairID]) );

		//执行判断
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//获取动作
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//优先级别
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//动作判断
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) 
			return true;

		//吃胡等待
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
					return true;
			}
		}

		//放弃操作
		if (cbTargetAction==WIK_NULL)
		{
			//用户状态
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//发送扑克
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//变量定义
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser][0];

		//出牌变量
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//胡牌操作
		if (cbTargetAction==WIK_CHI_HU)
		{
			//结束信息
			m_cbChiHuCard=cbTargetCard;

			for (WORD i=(m_wProvideUser+m_wPlayerCount-1)%GAME_PLAYER;i!=m_wProvideUser;i = (i+m_wPlayerCount-1)%GAME_PLAYER)
			{
				//过虑判断
				if ((m_cbPerformAction[i]&WIK_CHI_HU)==0)
					continue;

				//胡牌判断
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_dwChiHuKind[i] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i],m_cbHu);

				//插入扑克
				if (m_dwChiHuKind[i]!=WIK_NULL) 
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					wTargetUser = i;
					break;
				}
			}

			//结束游戏
			ASSERT(m_dwChiHuKind[wTargetUser]!=WIK_NULL);
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//组合扑克
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

		//删除扑克
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}
				//禁止出牌数据和数目
				m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard;

				//过滤789
				if((cbTargetCard&MASK_VALUE)<7)
					m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard+3;


				break;
			}
		case WIK_RIGHT:		//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				//禁止出牌数据和数目
				m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard;

				//过滤12 3
				if((cbTargetCard&MASK_VALUE)>3)
					m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard-3;


				break;
			}
		case WIK_CENTER:	//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				//禁止出牌数据和数目
				m_cbEnjoinCardData[wTargetUser][m_cbEnjoinCardCount[wTargetUser]++]=cbTargetCard;				


				break;
			}
		case WIK_PENG:		//碰牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_GANG:		//杠牌操作
			{
				//删除扑克,被动动作只存在放杠
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard)) )
				{
					ASSERT( FALSE );
					return false;
				}
				//杠牌次数
				m_cbEndLeftCount += 2;


				break;
			}
		default:
			ASSERT( FALSE );
			return false;
		}

		//if(IsAfford(wTargetUser)>1)
		//{
		//	//通知消息
		//	TCHAR szMessage[128]=TEXT("");
		//	_sntprintf(szMessage,CountArray(szMessage),TEXT(" [ %s ] 三摊"),m_pITableFrame->GetTableUserItem(wTargetUser)->GetNickName());
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

		//构造结果
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

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//用户状态
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//设置用户
		m_wCurrentUser=wTargetUser;

		//杠牌处理
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus = true;
			DispatchCardData(wTargetUser,true);
		}

		return true;
	}

	//主动动作
	if (m_wCurrentUser==wChairID)
	{
		//效验操作
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return true;

		//扑克效验
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard[0])==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard[0])==false)) 
			return false;

		//设置变量
		m_bSendStatus=true;
		m_cbUserAction[m_wCurrentUser]=WIK_NULL;
		m_cbPerformAction[m_wCurrentUser]=WIK_NULL;

		bool bPublic=false;

		//执行动作
		switch (cbOperateCode)
		{
		case WIK_GANG:			//杠牌操作
			{
				//变量定义
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard[0]);

				//杠牌处理
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//寻找组合
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

					//效验动作
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//组合扑克
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[3]=cbOperateCard[0];
				}
				else
				{
					//扑克效验
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
						return false;

					//设置变量
					bPublic=false;
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					for( BYTE j = 0; j < 4; j++ ) m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[j] = cbOperateCard[0];
				}

				//删除扑克
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				m_bGangStatus = true;


				//杠牌次数
				m_cbEndLeftCount += 2;

				//if(IsAfford(wChairID)>1)
				//{
				//	//通知消息
				//	TCHAR szMessage[128]=TEXT("");
				//	_sntprintf(szMessage,CountArray(szMessage),TEXT(" [ %s ] 三摊"),m_pITableFrame->GetTableUserItem(wChairID)->GetNickName());
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

				//构造结果
				CMD_S_OperateResult OperateResult;
				ZeroMemory( &OperateResult,sizeof(OperateResult) );
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard[0]=cbOperateCard[0];

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//效验动作
				bool bAroseAction=false;
				if (bPublic==true) bAroseAction=EstimateUserRespond(wChairID,cbOperateCard[0],EstimatKind_GangCard);

				//发送扑克
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID,true);
				}

				return true;
			}
		case WIK_CHI_HU:		//吃胡操作
			{
				//吃牌权位
				if (m_cbOutCardCount==0)
				{
					m_wProvideUser = m_wCurrentUser;
					m_cbProvideCard = m_cbSendCardData;
				}

				//普通胡牌
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

				//结束信息
				m_cbChiHuCard=m_cbProvideCard;

				//结束游戏
				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//用户补牌
bool CTableFrameSink::OnUserReplaceCard(WORD wChairID, BYTE cbCardData)
{
	//错误断言
	ASSERT(wChairID==m_wReplaceUser);
	ASSERT(m_GameLogic.IsHuaCard(cbCardData)==true);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wReplaceUser) return false;
	if (m_GameLogic.IsHuaCard(cbCardData)==false) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false)  return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//状态变量
	m_bSendStatus=true;
	m_cbSendCardData=0x00;

	//花牌数量统计
	m_cbHuaCardData[wChairID][m_cbHuaCardCount[wChairID]]=cbCardData;
	m_cbHuaCardCount[wChairID]++;


	//构造数据
	CMD_S_ReplaceCard ReplaceCard;
	ReplaceCard.wReplaceUser=wChairID;
	ReplaceCard.cbReplaceCard=cbCardData;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

	//派发扑克
	DispatchCardData(wChairID,true);

	return true;
}

//发送操作
bool CTableFrameSink::SendOperateNotify()
{
	//发送提示
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//构造数据
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//发送数据
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bTail)
{
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;
	ASSERT( m_bSendStatus );
	if( m_bSendStatus == false ) return false;

	//丢弃扑克
	if ((m_wReplaceUser==INVALID_CHAIR)&&(m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//荒庄结束
	if (m_cbLeftCardCount==m_cbEndLeftCount)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//设置变量
	if( m_wReplaceUser == INVALID_CHAIR )
	{
		m_cbOutCardData=0;
		m_wCurrentUser=wCurrentUser;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	//加牌
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//补花判断
	if ((m_wReplaceUser!=INVALID_CHAIR)||(m_GameLogic.IsHuaCard(m_cbSendCardData)==true))
	{
		m_wReplaceUser=INVALID_CHAIR;

		//用户判断
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//用户索引
			WORD wUserIndex=(wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

			//扑克判断
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
		//设置变量
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;
	}

	if( m_wReplaceUser == INVALID_CHAIR )
	{
		if( !m_bEnjoinChiHu[m_wCurrentUser] )
		{
			//胡牌判断
			CChiHuRight chr;
			BYTE cbHu=0;
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
			m_cbUserAction[m_wCurrentUser] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wCurrentUser],m_WeaveItemArray[m_wCurrentUser],
				m_cbWeaveItemCount[m_wCurrentUser],m_cbProvideCard,chr,cbHu,true);
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
		}

		//杠牌判断
		if ((m_bEnjoinChiPeng[m_wCurrentUser]==false)&&(m_cbLeftCardCount>m_cbEndLeftCount))
		{
			tagGangCardResult GangCardResult;
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);
		}
	}

	//堆立信息
	ASSERT( m_wHeapHand != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR );
	if( !bTail )
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapHand][0]++;
	}
	else
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}

	//构造数据
	CMD_S_SendCard SendCard;
	SendCard.cbSendCardUser/*wSendCardUser*/ = wCurrentUser;
	SendCard.wReplaceUser = m_wReplaceUser;
	SendCard.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
	SendCard.bTail = bTail;
	SendCard.cbActionMask=m_wReplaceUser==INVALID_CHAIR?m_cbUserAction[m_wCurrentUser]:WIK_NULL;
	SendCard.cbCardData=(m_wCurrentUser==wCurrentUser)?m_cbProvideCard:m_cbSendCardData;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY)
	{
		setChairTrustee(wChairID,true);
	}
	return true;
}
//用户重入
bool CTableFrameSink::OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY)
	{
		setChairTrustee(wChairID,false);
	}
	return true;
}
//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//变量定义
	bool bAroseAction=false;

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//不能吃碰胡
	if(m_bMustWanTong==true) return false;

	if(m_cbDone[wCenterUser]==1 && m_cbDiscardCount[wCenterUser]==0 && m_bMustWanTong==false) return false;

	//动作判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//用户过滤
		if (wCenterUser==i) continue;

		//出牌类型
		if (EstimatKind==EstimatKind_OutCard)
		{
			//吃碰判断
			if (m_bEnjoinChiPeng[i]==false)
			{
				//碰牌判断
				if(cbCenterCard != m_cbEnjoinPengCard[i])
				{
					m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);
				}
				//禁止碰牌
				if(m_cbUserAction[i]&WIK_PENG)
				{
					m_cbEnjoinPengCard[i]=cbCenterCard;
				}	

				//吃牌判断
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i)
					m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);

				//杠牌判断
				if (m_cbLeftCardCount>m_cbEndLeftCount) 
				{
					m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				}
			}
		}

		//胡牌判断
		if (m_bEnjoinChiHu[i]==false )
		{
			//吃胡判断
			CChiHuRight chr;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			if(m_cbEnjoinHuCard[i] != cbCenterCard)
			{
				BYTE cbHu=0;
				BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr,cbHu);
				m_cbUserAction[i] |= cbAction;
			}
		}

		//禁止胡牌
		if(m_cbUserAction[i]&WIK_CHI_HU)
		{
			m_cbEnjoinHuCard[i]=cbCenterCard;
		}

		//结果判断
		if (m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
	}

	//结果处理
	if (bAroseAction==true) 
	{
		//设置变量
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//发送提示
		SendOperateNotify();

		return true;
	}

	return false;
}

//从系统翻牌获取钻牌索引
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

//获取桌面牌数
BYTE CTableFrameSink::GetDiscardCount( BYTE cbCardData )
{
	BYTE cbCount = 0;
	//遍历丢弃牌
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
		//检查杠牌
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

//权位过滤
void CTableFrameSink::FiltrateRight( WORD wWinner, CChiHuRight &chr )
{
	/*	增加权位	*/

	//自摸
	if( wWinner == m_wProvideUser )
		chr |= CHR_ZI_MO;

}
//检查3包 互包
BYTE CTableFrameSink::IsAfford(WORD wUserAccept)
{
	//校验数据
	ASSERT(wUserAccept<4);
	ASSERT(wUserAccept>=0);

	if(m_cbWeaveItemCount[wUserAccept]<3) return 1;

	BYTE cbAfford=0;//1：不是包关系 2：包关系
	BYTE cbCount[4] = {0,0,0,0};
	BYTE bAllKe=true;
	//供三包
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

//算分
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

	if(!(m_ChiHuRight[wWinner]&CHR_ZI_MO).IsEmpty())  //自摸胡
	{
		cbHu[wWinner] += 2;
	}

	if(!(m_ChiHuRight[wWinner]&CHR_KAN_ZHANG).IsEmpty()) //夹中胡
	{
		cbHu[wWinner] += 2;
	}
	cbHu[wWinner] += 30;
	cbHu[wWinner] += m_cbHu;
	cbHua[wWinner] +=m_GameLogic.GetChiHuActionRank(m_ChiHuRight[wWinner]);

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//牌型分析
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

		//花牌
		if(i == m_wBankerUser)//东风位
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
				//正花 野花
				for (INT bTemp = 0;bTemp<m_cbHuaCardCount[i];bTemp++)
				{
					//梅 春 正花
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

		if(i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//南风位
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
					//兰 夏 正花
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
		if(i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//西风位
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
				//正花 野花
				for (INT bTemp = 0;bTemp<m_cbHuaCardCount[i];bTemp++)
				{

					//梅 春 正花
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
		if(i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//北风位
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
				//正花 野花
				for (INT bTemp = 0;bTemp<m_cbHuaCardCount[i];bTemp++)
				{

					//梅 春 正花
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
			//万 索 筒 东南西北
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

		
			//中发白
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

		//庄家中发白
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
		//花
		if(i == m_wBankerUser)//东风位
		{
			if(m_cbCardIndex[i][27]==2)
				cbHu[i] += 2;

			if(m_cbCardIndex[i][27]>=3)
				cbHua[i] += 1;


		}

		if(i == (m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount)//南风位
		{
			if(m_cbCardIndex[i][28]==2)
				cbHu[i] += 2;
			if(m_cbCardIndex[i][28]>=3)
				cbHua[i] += 1;


		}
		if(i == (m_wBankerUser+m_wPlayerCount-2)%m_wPlayerCount)//西风位
		{
			if(m_cbCardIndex[i][29]==2)
				cbHu[i] += 2;
			if(m_cbCardIndex[i][29]>=3)
				cbHua[i] += 1;


		}
		if(i == (m_wBankerUser+m_wPlayerCount-3)%m_wPlayerCount)//北风位
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

//获取等级
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
