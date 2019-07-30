#include "Stdafx.h"
#include "AndroidUserItemSink.h"

////////////////////////////////////////////////////////////////////////////////
#define IDI_GAME_ACTION				105
////////////////////////////////////////////////////////////////////////////////

static const LONGLONG lScoreArray[]={100L,1000L,10000L};
int CAndroidUserItemSink::m_stlApplyBanker=0L;

//构造函数
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

//重置
bool CAndroidUserItemSink::RepositionSink()
{
	//m_bMeCurrentBanker=false;
	//m_bApplyingBanker=false;
	//m_bCancelingBanker=false;
	//ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	//ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	return true;
}
//重置接口
bool  CAndroidUserItemSink::RepositUserItemSink()
{
	m_bMeCurrentBanker=false;
	m_bApplyingBanker=false;
	m_bCancelingBanker=false;
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (m_pIAndroidUserItem->GetGameStatus())
	{
	case GAME_STATUS_FREE://用于申请上庄、或申请下庄的逻辑分支
		{
			if (m_bMeCurrentBanker==false && m_stlApplyBanker<m_nRobotApplyBanker)//不是庄家，且名额足够
			{
				if (m_bApplyingBanker==true)//已经申请，则不需再申请（已经申请的机器人，已经存在于上庄列表了）
				{
					return false;
				}
				if (m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()<m_lApplyBankerCondition)//分数不足
				{
					return false;
				}
				m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER,NULL,0);//发送上庄申请
				m_stlApplyBanker++;//占用一个名额
				m_bApplyingBanker=true;//已申请上庄
			}
			else if (m_bMeCurrentBanker==true)//我是庄家
			{
				if (m_bCancelingBanker==true) return false;
				m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER,NULL,0);//发送下庄申请
				m_bCancelingBanker=true;//已申请下庄
			}
			return true;
		}
	case GS_PLACE_JETTON://用于下注的逻辑分支
		{
			if (m_wJettonCount--==0) return false;//下注次数用完
			//限制索引
			//int nMaxIndex=0;
			//for (int i=0; i<CountArray(lScoreArray); i++)
			//	if (m_lMeMaxScore>lScoreArray[i]) nMaxIndex=i+1;
			//if (nMaxIndex>m_nMaxJettonRange) nMaxIndex=m_nMaxJettonRange;
			//筹码大小；在筹码数组里随机找合适的筹码
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
			//设置变量
			BYTE cbAreaCount=rand()%5+4;//每次下注4至8次
			for (int i=0; i<cbAreaCount; i++)
			{
				m_cbJettonArea=rand()%JETTON_AREA_COUNT;
				if (m_nJettonRange<1 || m_nJettonRange>999) m_nJettonRange=99;
				lJettonScore=(rand()%m_nJettonRange+1)*m_dwJettonRate;
				m_lUserJettonScore[m_cbJettonArea]+=lJettonScore;
				//发送消息
				CMD_C_PlaceJetton PlaceJetton;
				PlaceJetton.cbJettonArea=m_cbJettonArea;
				PlaceJetton.lJettonScore=lJettonScore;
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
			}
			//设置定时器
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

//游戏消息(OK)
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
//printf("wSubCmdID=%d\n",wSubCmdID);
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE://游戏空闲
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START://游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON://用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END://游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER://申请做庄
		{
			return OnSubUserApplyBanker(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER://切换庄家
		{
			return OnSubChangeBanker(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD://游戏记录
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL://下注失败
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER://取消做庄
		{
			return OnSubUserCancelBanker(pBuffer,wDataSize);
		}
	case SUB_S_BIG_SMALL://猜大小
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

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch(cbGameStatus)
	{
	case GAME_STATUS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
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
	case GS_PLACE_JETTON:			//游戏状态
	case GS_GAME_END:			//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
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

//游戏空闲(OK)；选择性的，申请上庄，或申请下庄
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;
	//读取配置文件
	ReadConfigInformation(false);

	m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_FREE);

	IServerUserItem *pServerUserItem=m_pIAndroidUserItem->GetMeUserItem();
	ASSERT(pServerUserItem!=NULL);
	if (m_wBankerUser==INVALID_CHAIR||m_wBankerUser!=pServerUserItem->GetChairID())
		m_bMeCurrentBanker=false;
	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	m_lApplyBankerCondition=pGameFree->lApplyBankerCondition;
	//很重要的申请数量同步
	if (m_stlApplyBanker!=pGameFree->wApplyUserCount)
	{
		m_stlApplyBanker=pGameFree->wApplyUserCount;
	}
	//
	if (pServerUserItem->GetUserStatus()!=US_LOOKON)
	{
		//上庄条件；未申请、我不是庄家、允许机器人上庄、分数足够
		if (m_bApplyingBanker==false && m_bMeCurrentBanker==false && m_bAllowApplyBanker==TRUE && pServerUserItem->GetUserScore()>m_lApplyBankerCondition)
		{
			m_wRandBankerTime=rand()%m_wMaxBankerTime+1;
			if(m_wRandBankerTime<3) m_wRandBankerTime=3;//最起码要坐庄3局以上

			UINT nElapse=rand()%3+1;
			m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION, nElapse);
		}
		else if (m_bMeCurrentBanker==true && m_bCancelingBanker==false)//下庄申请
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

//游戏开始(OK)，游戏开始，可以进行下注；产生下注区域，以及下注次数；然后去到CAndroidUserItemSink::OnEventTimer()的GS_PLACE_JETTON分支
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	//设置变量
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_wBankerUser=pGameStart->wBankerUser;
	m_lBankerScore=pGameStart->lBankerScore;
	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_PLACE_JETTON);
	//下注条件
	if (m_lMeMaxScore>2000L && m_wBankerUser!=m_pIAndroidUserItem->GetChairID() /*m_wBankerUser!=INVALID_CHAIR &&*/)
	{
		m_wJettonCount=rand()%10+3;
		//定时器
		UINT nElapse=rand()%2+1;
		m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION,nElapse);
	}
	return true;
}

//用户加注(OK)，得到用户下注的信息，存入“总注”
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;
	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	//校验数据
	ASSERT(pPlaceJetton->cbJettonArea<=ID_APPLE && pPlaceJetton->cbJettonArea>=ID_BAR);//区域校验
	ASSERT(pPlaceJetton->wChairID!=INVALID_CHAIR);//玩家校验
	//ASSERT(pPlaceJetton->lJettonScore>=lScoreArray[0] && pPlaceJetton->lJettonScore<=lScoreArray[3]);//分数校验
	//设置变量
	//m_lAllJettonScore[pPlaceJetton->cbJettonArea]+=pPlaceJetton->lJettonScore;
	CopyMemory(m_lAllJettonScore,pPlaceJetton->lTotalJettonScore,sizeof(m_lAllJettonScore));
	return true;
}

//下注失败(OK)，此消息是单发，不需判断是否是自己，取消已下注，删除定时器
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;
	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	//校验数据
	ASSERT(pPlaceJettonFail->cbJettonArea<=ID_APPLE && pPlaceJettonFail->cbJettonArea>=ID_BAR);
	//设置变量
	m_lUserJettonScore[pPlaceJettonFail->cbJettonArea]-=pPlaceJettonFail->lJettonScore;
	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_GAME_ACTION);
	return true;
}

//游戏结束(OK)
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	//设置变量
	m_lBankerScore=pGameEnd->lBankerScore;
	m_wBankerTime=(WORD)pGameEnd->iBankerTime;
	m_lApplyBankerCondition=pGameEnd->lApplyBankerCondition;
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	//游戏状态
	m_pIAndroidUserItem->SetGameStatus(GS_GAME_END);
	return true;
}

//申请做庄(OK)
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;
	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;
	return true;
}

//取消做庄(OK)，取消申请，设置“同步值”
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;
	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;
	//
	if (lstrcmp(m_pIAndroidUserItem->GetMeUserItem()->GetNickName(),pCancelBanker->szCancelUser)==0)
	{
		m_stlApplyBanker--;
		m_bApplyingBanker=false;
	}
	return true;
}

//切换庄家
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	if (m_wBankerUser==wMeChairID && pChangeBanker->wBankerUser!=wMeChairID)//机器人下庄
	{
		m_bMeCurrentBanker=false;
		m_bCancelingBanker=false;
		m_stlApplyBanker--;
		m_wRandBankerTime=0;
	}
	else if (pChangeBanker->wBankerUser==wMeChairID)//机器人上庄
	{
		m_bMeCurrentBanker=true;
		m_bApplyingBanker=false;
	}

	m_wBankerUser=pChangeBanker->wBankerUser;
	m_lBankerScore=pChangeBanker->lBankerScore;

	return true;
}

//最大下注
LONGLONG CAndroidUserItemSink::GetUserMaxJetton()
{
	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(JETTON_AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=JETTON_AREA_COUNT; ++nAreaIndex) 
		lNowJetton += m_lUserJettonScore[nAreaIndex];

	//庄家金币
	LONGLONG lBankerScore=2147483647;
	if (m_wBankerUser!=INVALID_CHAIR) 
		lBankerScore=m_lBankerScore;

	for (int nAreaIndex=1; nAreaIndex<=JETTON_AREA_COUNT; ++nAreaIndex)
		lBankerScore-=m_lAllJettonScore[nAreaIndex];

	//区域限制
	LONGLONG lMeMaxScore=min(m_lMeMaxScore-lNowJetton,m_lAreaLimitScore);

	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	//非零限制
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//读取配置文件
void CAndroidUserItemSink::ReadConfigInformation(bool bFirstRead)
{
	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	mysprintf(m_szConfigFileName,sizeof(m_szConfigFileName),_T("%s\\SGDZ.ini"),szPath);
	//筹码限制
	TCHAR OutBuf[255] = TEXT("");
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotMaxJetton"), _T("5000000"), OutBuf, 255, m_szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[1]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotMinJetton"), _T("100"), OutBuf, 255, m_szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[0]);
	//是否每局刷新
	if(!bFirstRead && GetPrivateProfileInt(m_szRoomName, TEXT("Refresh"),0, m_szConfigFileName)==0) return;
	//机器人下注范围
	m_nJettonRange=GetPrivateProfileInt(m_szRoomName,TEXT("JettonRange"),99,m_szConfigFileName);
	if (m_nJettonRange<1 || m_nJettonRange>999) m_nJettonRange=99;
	//机器人最大坐庄次数
	m_wMaxBankerTime=(WORD)GetPrivateProfileInt(m_szRoomName,TEXT("RobotBankerCount"),3,m_szConfigFileName);
	//机器人能否申请坐庄
	m_bAllowApplyBanker=GetPrivateProfileInt(m_szRoomName,TEXT("RobotBanker"),1,m_szConfigFileName);
	//机器人最大上庄个数
	m_nRobotApplyBanker=GetPrivateProfileInt(m_szRoomName, TEXT("RobotApplyBanker"),5,m_szConfigFileName);
}

////////////////////////////////////////////////////////////////////////////////
//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}
//初始接口
bool  CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;
	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//用户进入
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

////所有玩家都开始了
//void CAndroidUserItemSink::OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
//{
//	return ;
//}
////////////////////////////////////////////////////////////////////////////////
