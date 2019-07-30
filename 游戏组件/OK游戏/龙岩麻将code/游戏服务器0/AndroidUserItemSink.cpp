#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#ifdef _DEBUG
	#define TIME_LESS				1									//最少时间
#else
	#define TIME_LESS				1									//最少时间
#endif
//游戏时间
#define TIME_OUT_CARD				3									//出牌时间
#define TIME_START_GAME				3									//开始时间
#define TIME_OPERATE_CARD			3									//操作时间
#define TIME_HEAR_STATUS			3									//听牌时间按

//游戏时间
#define IDI_OUT_CARD				(IDI_REBOT_MODULE_START+0)			//出牌时间
#define IDI_START_GAME				(IDI_REBOT_MODULE_START+1)			//开始时间
#define IDI_OPERATE_CARD			(IDI_REBOT_MODULE_START+2)			//操作时加

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//状态变量
	m_bHearStatus=false;
	m_bWillHearStatus=false;
	m_bActionMask = WIK_NULL;
	m_cbActionCard=0;
	m_dwGameRuleIdex=0;

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	m_cbSendCardData = 0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	
	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{

}

//接口查询
void *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;
	
	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_dwGameRuleIdex=0;
	//状态变量
	m_bHearStatus=false;
	m_bWillHearStatus=false;
	m_bActionMask = WIK_NULL;
	m_cbActionCard=0;

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	m_cbSendCardData = 0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	return true;
}

//时间消息
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//开始游戏
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
			//开始判断
			if( m_pIAndroidUserItem->GetGameStatus()==GS_MJ_FREE )
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_OPERATE_CARD:		//操作定时器
		{
			//首先处理操作消息（吃碰杠胡）
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
					////如果是因为上听 这里的，就不用回音了
					//OnOperateCard(m_bActionMask, m_cbActionCard);
					//return true;
				}
			}
			//下面是正常打牌/上听了
			if(m_wCurrentUser==wMeChair)
			{
#ifdef _DEBUG
				//开始抓什么牌就打什么牌的节奏
				if(m_cbSendCardData!=0 && !m_GameLogic.IsMagicCard(m_cbSendCardData))
				{
					OnOutCard(m_cbSendCardData);
					return true;
				}
#endif

				//已经停牌了：而且是发牌状态；自动停吃停碰
				if((m_bHearStatus==true)&&(m_cbSendCardData!=0))
				{
					BYTE cbCardData=m_cbSendCardData;
					//出牌效验
					if (VerdictOutCard(cbCardData)==false)
					{
						for (BYTE i=0;i<MAX_INDEX;i++)
						{
							//出牌效验
							if (m_cbCardIndex[wMeChair][i]==0) continue;
							if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false)
								continue;

							if (m_GameLogic.IsMagicCard(cbCardData)) 
								continue;

							//设置变量
							cbCardData=m_GameLogic.SwitchToCardData(i);
						}
					}
					//出牌动作
					OnOutCard(cbCardData);
					return true;

				}

				//智能出牌:还没有停牌呢
				BYTE cbCardData=GetIsolatedCard();
				if((cbCardData!=0xFF)&&(m_bHearStatus==false))
				{
					//未停牌，不能把财神牌打出去
					if(!m_GameLogic.IsMagicCard(cbCardData))
					{
						OnOutCard(cbCardData);
						return true;
					}
				}

				//异常处理：
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
				
				//开始抓什么牌就打什么牌的节奏
				if(m_cbSendCardData!=0 && !m_GameLogic.IsMagicCard(m_cbSendCardData))
				{
					OnOutCard(m_cbSendCardData);
					return true;
				}

				//出牌效验
				if (VerdictOutCard(cbCardData)==false)
				{
					for (BYTE i=0;i<MAX_INDEX;i++)
					{
						//出牌效验
						if (m_cbCardIndex[wMeChair][i]==0) continue;
						if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false)continue;

						//设置变量
						cbCardData=m_GameLogic.SwitchToCardData(i);
					}
				}
				//出牌动作
				OnOutCard(cbCardData);
				return true;

				ASSERT(FALSE);

			}
			return true;
		}
	}

	return false;
}

//游戏消息
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
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//用户出牌
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:		//发牌消息
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//操作提示
		{
			return OnSubOperateNotify(pData,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//操作结果
		{
			return OnSubOperateResult(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_TRUSTEE:			//用户托管
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

//游戏消息
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//设置数据
			m_wBankerUser=pStatusFree->wBankerUser;

			if( pIServerUserItem->GetUserStatus() != US_READY )
			{
				UINT nElapse = rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	case GS_MJ_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//设置变量
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;

			//听牌状态
			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			m_bHearStatus=(pStatusPlay->cbHearStatus[wMeChairID]==TRUE)?true:false;

			//历史变量
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));

			//扑克变量
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()]);

			//出牌设置
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

//用户进入
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void  CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//庄家信息
bool CAndroidUserItemSink::OnSubGameStart(const void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_MJ_PLAY);

	//设置变量
	m_bHearStatus=false;
	m_bWillHearStatus=false;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	m_bActionMask = WIK_NULL;
	m_cbActionCard = 0;

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	//m_cbSendCardData = 0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//设置扑克
	WORD wMeChairId = m_pIAndroidUserItem->GetChairID();
	BYTE cbCardCount = (wMeChairId==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex[wMeChairId]);

	
	//动作处理
	if ((pGameStart->cbUserAction!=WIK_NULL) || pGameStart->wCurrentUser==m_pIAndroidUserItem->GetChairID() )
	{
		m_bActionMask = pGameStart->cbUserAction;
		UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,3+nElapse);
	}

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_MJ_FREE);

	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//设置
	UINT nElapse = rand()%TIME_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}



//操作提示
bool CAndroidUserItemSink::OnSubOperateNotify( const void *pBuffer,WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//用户界面
	if ((pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//获取变量
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		m_bActionMask = pOperateNotify->cbActionMask;
		m_cbActionCard=pOperateNotify->cbActionCard;
		//处理杠牌
		if(pOperateNotify->cbActionMask & WIK_GANG > 0)
		{
			TRACE("GANG:user%ld, operationCard:%x, GangCard:%x\n", pOperateNotify->wResumeUser, pOperateNotify->cbActionCard, pOperateNotify->cbGangCardData);
			m_cbActionCard = pOperateNotify->cbGangCardData[0];
		}

		//设置时间
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
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//变量定义
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard[0];

	//出牌变量
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//设置数据
	m_bActionMask = WIK_NULL;
	m_cbActionCard =0;
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//设置组合
	if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)
	{
		//设置变量
		m_wCurrentUser=INVALID_CHAIR;

		//组合扑克
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

		//组合扑克
		if (cbWeaveIndex==0xFF)
		{
			//暗杠判断
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

			//设置扑克
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		}

		//扑克设置
		if(wOperateUser==m_pIAndroidUserItem->GetChairID())
			m_cbCardIndex[wOperateUser][m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard[0])]=0;

	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL&&pOperateResult->cbOperateCode!=WIK_CHI_HU)
	{
		//设置变量
		m_wCurrentUser=pOperateResult->wOperateUser;

		//设置组合
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//组合界面
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);


		//删除扑克
		if(wOperateUser==m_pIAndroidUserItem->GetChairID())
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);

			m_GameLogic.RemoveCard(m_cbCardIndex[wOperateUser],cbWeaveCard,cbWeaveCardCount-1);
		}
	}

	//设置时间
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
			TRACE("AAA玩家%ld 停牌\n", m_wCurrentUser);
			m_bHearStatus=true;

		}
		//计算时间
		UINT nElapse=rand()%TIME_OPERATE_CARD+TIME_LESS;
		if ((m_bHearStatus==true)) 
			nElapse=rand()%TIME_HEAR_STATUS+TIME_LESS;
		//ASSERT(FALSE);

		//设置时间
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
	}

	return true;
}

//
bool CAndroidUserItemSink::OnSubOutCard( const void *pBuffer,WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//消息处理
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//变量定义
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	ASSERT( pOutCard->cbOutCardData != 0 );
	m_cbOutCardData=pOutCard->cbOutCardData;


	return true;
}

//
bool CAndroidUserItemSink::OnSubSendCard( const void *pBuffer,WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	m_cbActionCard = pSendCard->cbCardData;

	//设置变量
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//丢弃扑克
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]++] = m_cbOutCardData;
		//设置变量
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//发牌处理
	if ((pSendCard->cbCardData!=0)&&((m_wCurrentUser==wMeChairID)))
	{
		m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;	
		//扣除扑克
		m_cbLeftCardCount--;
	}

	//当前用户
	if ((m_wCurrentUser==wMeChairID))
	{
		m_cbSendCardData = pSendCard->cbCardData;
		if((m_bActionMask & WIK_GANG) > 0)
		{
			m_cbActionCard = pSendCard->cbGangCardIndeed;		//真正的杠牌
		}
		//听牌判断
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

	//设置时间
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
			TRACE("AAA玩家%ld 停牌\n", m_wCurrentUser);
			m_bHearStatus=true;

		}
		//计算时间
		UINT nElapse=rand()%TIME_OPERATE_CARD+TIME_LESS;
		if ((m_bHearStatus==true)&&(m_wCurrentUser==wMeChairID)) 
			nElapse=rand()%TIME_HEAR_STATUS+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OPERATE_CARD,nElapse);
	}

	return true;
}
//用户听牌
bool CAndroidUserItemSink::OnSubListenCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_C_Listen));
	if (wDataSize!=sizeof(CMD_C_Listen)) return false;

	//变量定义
	CMD_C_Listen * pListen=(CMD_C_Listen *)pBuffer;
	if(pListen->cbListen==m_pIAndroidUserItem->GetChairID())
	{
		m_bHearStatus=true;
	}
	return true;

}

//用户出牌
void CAndroidUserItemSink::OnOutCard( BYTE cbOutCard )
{
	//听牌设置
	if (m_bWillHearStatus==true)
	{
		m_bHearStatus=true;
		m_bWillHearStatus=false;
	}

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	WORD wMeChairId = m_pIAndroidUserItem->GetChairID();
	m_GameLogic.RemoveCard(m_cbCardIndex[wMeChairId],cbOutCard);

	//设置界面
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);
	m_cbSendCardData=0;

	//发送数据
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCard;
//	ASSERT(!m_GameLogic.IsMagicCard(cbOutCard));
	if( !m_pIAndroidUserItem->SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)) )
	{
		ASSERT( FALSE );
		return ;
	}

	//重置变量
	m_bActionMask = WIK_NULL;
	
	return ;
}

//
void CAndroidUserItemSink::OnOperateCard( BYTE cbOperateCode,BYTE cbOperateCard )
{
	//删除时间
	m_pIAndroidUserItem->KillGameTimer(IDI_OPERATE_CARD);

	//变量定义
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));


	//提取杆牌
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
	//发送命令
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;

	BYTE cbValue=cbOperateCard&MASK_VALUE;
	
	// 以下的 OperateCode需要时单选命令
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

	//重置数据
	m_bActionMask = WIK_NULL;
	m_cbActionCard=0;
	return ;
}
//智能打牌
BYTE CAndroidUserItemSink::GetIsolatedCard()
{

	//定义变量
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	int i=0;
	
	//有风打风
	for( i=27; i<27+7; i++)
	{
		if(m_cbCardIndex[wMeChairID][i]==1 && m_GameLogic.GetMagicIndex()!=i)
		{
			return m_GameLogic.SwitchToCardData(i);
		}
	}
	//有孤牌打孤牌
	//打19孤牌，不要打财神牌
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
	//打普通孤牌
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

	//打卡张
	//打19卡张,不要打财神牌
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

	//打普通卡张
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
//孤牌数目
BYTE CAndroidUserItemSink::GetIsolatedCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//临时数据
	BYTE cbTempCardIndex[MAX_INDEX];
	CopyMemory(cbTempCardIndex,cbCardIndex,sizeof(cbTempCardIndex));

	//定义变量
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	int i=0;
	int iIsolateCardCount=0;

	//有风打风
	for( i=27; i<27+7; i++)
	{
		if(cbTempCardIndex[i]==1)
		{
			iIsolateCardCount++;
		}
	}
	//有孤牌打孤牌
	//打19孤牌
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
	//打普通孤牌
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

	//打卡张
	//打19卡张
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

	//打普通卡张
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
	//定义变量
	BYTE cbPreIsolateCardCount=GetIsolatedCardCount(m_cbCardIndex[m_pIAndroidUserItem->GetChairID()]);
	BYTE cbPengIsolateCardCount=0XFF;
	BYTE cbLeftIsolateCardCount=0XFF;
	BYTE cbCenterIsolateCradCount=0XFF;
	BYTE cbRightIsolateCardCount=0XFF;

	//碰牌操作
	if(cbOperateCode&WIK_PENG)
	{
		BYTE cbTempCardIndex[MAX_INDEX];
		CopyMemory(cbTempCardIndex,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],sizeof(cbTempCardIndex));

		//组合界面
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=WIK_PENG;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
		m_GameLogic.RemoveCard(cbTempCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		cbPengIsolateCardCount=GetIsolatedCardCount(cbTempCardIndex);
	}
	//左吃操作
	if(cbOperateCode&WIK_LEFT)
	{
		BYTE cbTempCardIndex[MAX_INDEX];
		CopyMemory(cbTempCardIndex,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],sizeof(cbTempCardIndex));

		//组合界面
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=WIK_LEFT;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
		m_GameLogic.RemoveCard(cbTempCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		cbLeftIsolateCardCount=GetIsolatedCardCount(cbTempCardIndex);
	}
	//中吃操作
	if(cbOperateCode&WIK_CENTER)
	{
		BYTE cbTempCardIndex[MAX_INDEX];
		CopyMemory(cbTempCardIndex,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],sizeof(cbTempCardIndex));

		//组合界面
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=WIK_CENTER;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
		m_GameLogic.RemoveCard(cbTempCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		cbCenterIsolateCradCount=GetIsolatedCardCount(cbTempCardIndex);
	}
	//右吃操作
	if(cbOperateCode&WIK_RIGHT)
	{
		BYTE cbTempCardIndex[MAX_INDEX];
		CopyMemory(cbTempCardIndex,m_cbCardIndex[m_pIAndroidUserItem->GetChairID()],sizeof(cbTempCardIndex));

		//组合界面
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=WIK_RIGHT;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
		m_GameLogic.RemoveCard(cbTempCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		cbRightIsolateCardCount=GetIsolatedCardCount(cbTempCardIndex);
	}
	//有碰就碰
	if(cbPengIsolateCardCount==0)
		return WIK_PENG;
	if(cbLeftIsolateCardCount==0)
		return WIK_LEFT;
	if(cbCenterIsolateCradCount==0)
		return WIK_CENTER;
	if(cbRightIsolateCardCount==0)
		return WIK_RIGHT;
	
	//调整权位
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

	//最优权位
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
//出牌判断
bool CAndroidUserItemSink::VerdictOutCard(BYTE cbCardData)
{
	//听牌判断
	if ((m_bHearStatus==true))
	{
		//变量定义
		CChiHuRight ChiHuRight;
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		BYTE cbWeaveCount=m_cbWeaveCount[wMeChairID];

		//构造扑克
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,m_cbCardIndex[wMeChairID],sizeof(cbCardIndexTemp));

		//删除扑克
		m_GameLogic.RemoveCard(cbCardIndexTemp,cbCardData);

		//听牌判断
		BYTE i=0;
		for ( i=0;i<MAX_INDEX;i++)
		{
			//胡牌分析
			BYTE cbCurrentCard=m_GameLogic.SwitchToCardData(i);
			BYTE cbHuCardKind=m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp,m_WeaveItemArray[wMeChairID],cbWeaveCount,cbCurrentCard,ChiHuRight);

			//结果判断
			if (cbHuCardKind!=CHR_NULL) 
				break;
		}

		//听牌判断
		return (i!=MAX_INDEX);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
