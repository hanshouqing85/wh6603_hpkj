#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include <math.h>
#include <algorithm>
using namespace std;

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_BANK_OPERATE			3									//银行定时
#define IDI_PLACE_JETTON1			103									//下注定时
#define IDI_PLACE_JETTON2			104									//下注定时
#define IDI_PLACE_JETTON3			105									//下注定时
#define IDI_PLACE_JETTON4			106									//下注定时
#define IDI_PLACE_JETTON5			107									//下注定时
#define IDI_CHECK_BANKER			108									//检查上庄
#define IDI_REQUEST_BANKER			101									//申请定时
#define IDI_GIVEUP_BANKER			102									//下庄定时
#define IDI_PLACE_JETTON			110									//下注定义 (预留110-160)
#define IDI_GET_MONEY				100									//下注定义 (预留110-160)

//////////////////////////////////////////////////////////////////////////

int CAndroidUserItemSink::m_stlApplyBanker = 0L;

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//游戏变量
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//上庄变量
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	m_lWinScore=0;
	m_lScoreOld=0;
	m_lBankerWinScore=0;
	ZeroMemory(m_nWinScore, sizeof(m_nWinScore));

	m_nRobotListMinCount = 2;//最少上庄人数

	//区域概率
	m_RobotInfo.nAreaChance[0] = 1;
	m_RobotInfo.nAreaChance[1] = 2;
	m_RobotInfo.nAreaChance[2] = 2;
	m_RobotInfo.nAreaChance[3] = 3;
	m_RobotInfo.nAreaChance[4] = 10;
	m_RobotInfo.nAreaChance[5] = 10;
	m_RobotInfo.nAreaChance[6] = 10;
	m_RobotInfo.nAreaChance[7] = 10;


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
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool  CAndroidUserItemSink::RepositionSink()
{
	//游戏变量
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//上庄变量
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	return true;
}

//时间消息
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_CHECK_BANKER:		//检查上庄
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
				return false;

			int nMinCount = m_nRobotApplyBanker;
			if ( m_nRobotApplyBanker > m_nRobotListMinCount)
				nMinCount = (rand()%(m_nRobotApplyBanker - m_nRobotListMinCount)) + m_nRobotListMinCount;

			//if (m_wCurrentBanker == INVALID_CHAIR)
			//{
				//空庄
				m_nWaitBanker++;

				//MyDebug(_T("机器人上庄(End) %d [%d %d] [%d %d]"), m_pIAndroidUserItem->GetChairID(), m_nWaitBanker, 
				//	m_nRobotWaitBanker, m_stlApplyBanker, m_nRobotApplyBanker);

				//机器人上庄
				if ( m_bRobotBanker&&m_stlApplyBanker < nMinCount/* && m_nWaitBanker >= m_nRobotWaitBanker && m_stlApplyBanker < m_nRobotApplyBanker*/)
				{
					//合法判断
					IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
					if (pIUserItemBanker->GetUserScore() > m_lBankerCondition) 
					{
						//机器人上庄
						m_nBankerCount = 0;
						//m_stlApplyBanker++;
						m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % m_cbTimeLeave) + 1);
					}
				}
			//}
			//else if ( m_wCurrentBanker != INVALID_CHAIR )
			//{
			//	//其他人坐庄
			//	m_nWaitBanker = 0;
			//}

			return false;
		}
	case IDI_REQUEST_BANKER:	//申请上庄
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER);

			return false;
		}
	case IDI_GIVEUP_BANKER:		//申请下庄
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER);

			return false;
		}
	case IDI_GET_MONEY:
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			SCORE lRobotScore = pUserItem->GetUserScore();
			SCORE lInsureScore = pUserItem->GetUserInfo()->lInsure;

			if (lRobotScore+m_lBankerWinScore < m_lBankerCondition/* && rand()%100>50*/)
			{
				SCORE lTakeScore=0L;

				lTakeScore = m_lBankerCondition * (rand()%3 +1);
				CString strLog;
				strLog.Format(L"lRobotScore = %0.2f,m_lBankerWinScore = %I64d,m_lBankerCondition = %I64d,lTakeScore = %0.2f",lRobotScore,m_lBankerWinScore,m_lBankerCondition,lTakeScore);
				CTraceService::TraceString(strLog,TraceLevel_Warning);

				//if (lTakeScore > lInsureScore)  lTakeScore = lInsureScore;

				if (lTakeScore > 0)
					m_pIAndroidUserItem->PerformTakeScore(lTakeScore);
			}

			return false;
		}
	case IDI_BANK_OPERATE:		//银行操作
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			////变量定义
			//IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			//SCORE lRobotScore = pUserItem->GetUserScore();			
			//{

			//	//判断存取
			//	if (lRobotScore > m_lRobotScoreRange[1])
			//	{
			//		SCORE lSaveScore=0L;

			//		lSaveScore = SCORE(lRobotScore*m_nRobotBankStorageMul/100);
			//		if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

			//		if (lSaveScore > 0 && m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
			//			m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

			//	}
			//	else if (lRobotScore < m_lRobotScoreRange[0])
			//	{
			//		SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
			//		if (lScore > 0)
			//			m_pIAndroidUserItem->PerformTakeScore(lScore);								
			//	}
			//}

			//取钱
			ReadConfigInfo();
			return false;
		}
	default:
		{
			if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON+MAX_CHIP_TIME)
			{
				//变量定义
				int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
				SCORE lMaxChipLmt = __min(m_lMaxChipBanker, m_lMaxChipUser);			//最大可下注值
				WORD wMyID = m_pIAndroidUserItem->GetChairID();
				for (int i = 0; i < AREA_COUNT; i++){
					nACTotal += m_RobotInfo.nAreaChance[i];
				}
					

				//统计次数
				m_nChipTimeCount++;

				//检测退出
				//if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])	return false;
				for (int i = 0; i < AREA_COUNT; i++)
				{
					if (m_lAreaChip[i] >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
					if (i == AREA_COUNT-1)	return false;
				}

				//下注区域
				ASSERT(nACTotal>0);
				static int nStFluc = 1;				//随机辅助
				if (nACTotal <= 0)	return false;
				do {
					nRandNum = (rand()+wMyID+nStFluc*3) % nACTotal;
					for (int i = 0; i < AREA_COUNT; i++)
					{
						nRandNum -= m_RobotInfo.nAreaChance[i];
						if (nRandNum < 0)
						{
							nChipArea = i;
							break;
						}
					}
				}
				while (m_lAreaChip[nChipArea] < m_RobotInfo.nChip[m_nChipLimit[0]]);
				nStFluc = nStFluc%3 + 1;

				//下注大小
				if (m_nChipLimit[0] == m_nChipLimit[1])
					nCurChip = m_nChipLimit[0];
				else
				{
					//设置变量
					lMaxChipLmt = __min(lMaxChipLmt, m_lAreaChip[nChipArea]);
					nCurJetLmt[0] = m_nChipLimit[0];
					nCurJetLmt[1] = m_nChipLimit[0];

					//计算当前最大筹码
					for (int i = m_nChipLimit[1]; i > m_nChipLimit[0]; i--)
					{
						if (lMaxChipLmt > m_RobotInfo.nChip[i]) 
						{
							nCurJetLmt[1] = i;
							break;
						}
					}

					//随机下注
					nRandNum = (rand()+wMyID) % (nCurJetLmt[1]-nCurJetLmt[0]+1);
					nCurChip = nCurJetLmt[0] + nRandNum;

					if (m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()>100000 && rand()%3>=1)
					{
						nCurChip = 6;
					}
					else if (m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()>50000 && rand()%3>=1)
					{
						nCurChip = 5;
					}
					else if (m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()>30000 && rand()%3>=1)
					{
						nCurChip = 4;
					}

					//多下控制 (当庄家金币较少时会尽量保证下足次数)
					if (m_nChipTimeCount < m_nChipTime)
					{
						SCORE lLeftJetton = SCORE( (lMaxChipLmt-m_RobotInfo.nChip[nCurChip])/(m_nChipTime-m_nChipTimeCount) );

						//不够次数 (即全用最小限制筹码下注也少了)
						if (lLeftJetton < m_RobotInfo.nChip[m_nChipLimit[0]] && nCurChip > m_nChipLimit[0])
							nCurChip--;
					}
				}

				/*ASSERT( MyDebug(_T("机器人下注 %d 下注次数 [%d/%d] 下注 [%d %d] 范围 [%d %d] 限制 [%I64d %I64d %I64d]"), wMyID, nTimerID-IDI_PLACE_JETTON, m_nChipTime, 
					nChipArea, m_RobotInfo.nChip[nCurChip], m_nChipLimit[0], m_nChipLimit[1], m_lMaxChipBanker, m_lMaxChipUser, lMaxChipLmt) );*/

				//变量定义
				CMD_C_PlaceJetton PlaceJetton = {};

				//构造变量
				PlaceJetton.cbJettonArea = nChipArea+1;		//区域宏从1开始
				PlaceJetton.lJettonScore = m_RobotInfo.nChip[nCurChip];

				if (PlaceJetton.lJettonScore == 100 && rand()%3>0)
				{
					PlaceJetton.lJettonScore = 1000;
				}

				//发送消息
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			}

			m_pIAndroidUserItem->KillGameTimer(nTimerID);
			return false;
		}
	}
	return false;
}

//游戏消息
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:			//游戏空闲 
		{
			return OnSubGameFree(pBuffer, wDataSize);
		}
	case SUB_S_GAME_START:			//游戏开始
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
	case SUB_S_PLACE_JETTON:		//用户加注
		{
			return OnSubPlaceJetton(pBuffer, wDataSize);
		}
	case SUB_S_APPLY_BANKER:		//申请做庄 
		{
			return OnSubUserApplyBanker(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER:		//取消做庄 
		{
			return OnSubUserCancelBanker(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER:		//切换庄家 
		{
			return OnSubChangeBanker(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:			//游戏结束 
		{
			return OnSubGameEnd(pBuffer, wDataSize);
		}
	case SUB_S_SEND_RECORD:			//游戏记录 (忽略)
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败 (忽略)
		{
			return true;
		}
	case SUB_S_TO_ANDROID:			//
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_GET_MONEY, 1);
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

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
	case GAME_STATUS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lUserLimitScore = pStatusFree->lUserMaxScore;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			m_lBankerCondition = pStatusFree->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);

			return true;
		}
	case GS_PLACE_JETTON:		//游戏状态
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//庄家信息
			m_wCurrentBanker = pStatusPlay->wBankerUser;

			m_lUserLimitScore = pStatusPlay->lUserMaxScore;
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			m_lBankerCondition = pStatusPlay->lApplyBankerCondition;

			CString strLog;
			strLog.Format(L"BankerCondition :%.2lf",m_lBankerCondition);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);

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

//游戏空闲
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//读取配置
	//if (m_bRefreshCfg)
		ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, false);

	//消息处理
	CMD_S_GameFree* pGameFree=(CMD_S_GameFree *)pBuffer;

	m_cbTimeLeave = pGameFree->cbTimeLeave;

	bool bMeGiveUp = false;
	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
	{
		m_nBankerCount++;
		if ( m_nBankerCount >= m_nRobotBankerCount )
		{
			//机器人走庄
			m_nBankerCount = 0;
			m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

			bMeGiveUp = true;
		}
	}

	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID() && !bMeGiveUp)
	{
		if (m_lWinScore>0)
		{
			if ( m_lWinScore*100/m_lScoreOld >= m_nWinScore[0])
			{
				//机器人走庄
				m_nBankerCount = 0;
				m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

				bMeGiveUp = true;
			}
		}else
		{
			if ( -(m_lWinScore*100/m_lScoreOld) >= m_nWinScore[1])
			{
				//机器人走庄
				m_nBankerCount = 0;
				m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

				bMeGiveUp = true;
			}
		}
	}

	//检查上庄
	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() || bMeGiveUp)
	{
		m_cbTimeLeave = pGameFree->cbTimeLeave - 3;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER, 3);

		m_lWinScore=0;
		m_lScoreOld=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
	}

	//银行操作
	if (pGameFree->cbTimeLeave > 2)
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, (rand() % (pGameFree->cbTimeLeave-1)) + 1);

	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{	
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//自己当庄或无下注机器人
	if (pGameStart->wBankerUser == m_pIAndroidUserItem->GetChairID() || pGameStart->nChipRobotCount <= 0)
		return true;

	//设置变量
	m_lMaxChipBanker = pGameStart->lBankerScore/m_RobotInfo.nMaxTime;
	m_lMaxChipUser = pGameStart->lUserMaxScore/m_RobotInfo.nMaxTime;
	m_wCurrentBanker = pGameStart->wBankerUser;
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	for (int i = 0; i < AREA_COUNT; i++)
		m_lAreaChip[i] = m_lAreaLimitScore;

	//系统当庄
	if (pGameStart->wBankerUser == INVALID_CHAIR)
	{
		m_stlApplyBanker = 0;
		m_lMaxChipBanker = 2147483647/m_RobotInfo.nMaxTime;
	}
	else
		m_lMaxChipUser = __min( m_lMaxChipUser, m_lMaxChipBanker );

	//计算下注次数
	int nElapse = 0;												
	WORD wMyID = m_pIAndroidUserItem->GetChairID();

	if (m_nRobotBetTimeLimit[0] == m_nRobotBetTimeLimit[1])
		m_nChipTime = m_nRobotBetTimeLimit[0];
	else
		m_nChipTime = (rand()+wMyID)%(m_nRobotBetTimeLimit[1]-m_nRobotBetTimeLimit[0]+1) + m_nRobotBetTimeLimit[0];
	ASSERT(m_nChipTime>=0);		
	if (m_nChipTime <= 0)	return false;								//的确,2个都带等于
	if (m_nChipTime > MAX_CHIP_TIME)	m_nChipTime = MAX_CHIP_TIME;	//限定MAX_CHIP次下注

	//计算范围
	if (!CalcJettonRange(__min(m_lMaxChipBanker, m_lMaxChipUser), m_lRobotJettonLimit, m_nChipTime, m_nChipLimit))
		return true;

	//设置时间
#if 1
	vector<int> vElapse(m_nChipTime);
	for (int i = 0; i < m_nChipTime; i++)
	{
		nElapse = GetRand(2,pGameStart->cbTimeLeave-1);
		vElapse[i]=nElapse;
	}
	std::sort(vElapse.begin(),vElapse.end());
	for (int i = 0; i < m_nChipTime; i++)
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON+i+1, vElapse[i]);
	}
#else
	// 当下注次数配置为1~2次时，机器人下注明显集中在前半段
	int nTimeGrid = int(pGameStart->cbTimeLeave-2)*800/m_nChipTime;		//时间格,前2秒不下注,所以-2,800表示机器人下注时间范围千分比
	for (int i = 0; i < m_nChipTime; i++)
	{
		int nRandRage = int( nTimeGrid * i / (1500*sqrt((double)m_nChipTime)) ) + 1;		//波动范围
		nElapse = 2 + (nTimeGrid*i)/1000 + ( (rand()+wMyID)%(nRandRage*2) - (nRandRage-1) );
		ASSERT(nElapse>=2&&nElapse<=pGameStart->cbTimeLeave);
		if (nElapse < 2 || nElapse > pGameStart->cbTimeLeave)	continue;

		//if (nElapse>3)
		//{
		//	nElapse = 3;//机器人之多在多少秒之内全部下完
		//}
		
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON+i+1, nElapse);
	}
#endif

	//ASSERT( MyDebug(_T("机器人 %d 下注次数 %d 范围 [%d %d] 总人数 %d 限制 [%I64d %I64d] 上庄 [%d %d]"), wMyID, m_nChipTime, m_nChipLimit[0], m_nChipLimit[1], 
	//	pGameStart->nChipRobotCount, m_lMaxChipBanker, m_lMaxChipUser, m_stlApplyBanker, m_nRobotApplyBanker) );

	return true;
}

//用户加注
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	//设置变量
	m_lMaxChipBanker -= pPlaceJetton->lJettonScore;
	m_lAreaChip[pPlaceJetton->cbJettonArea-1] -= pPlaceJetton->lJettonScore;
	if (pPlaceJetton->wChairID == m_pIAndroidUserItem->GetChairID())
		m_lMaxChipUser -= pPlaceJetton->lJettonScore;

	return true;
}

//下注失败
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if (m_pIAndroidUserItem->GetChairID()==m_wCurrentBanker) 
	{
		m_lWinScore=pGameEnd->lBankerTotallScore;
		//银行操作
		m_lBankerWinScore= pGameEnd->lBankerScore;
	}


	return true;
}

//申请做庄
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//自己判断
	if (m_pIAndroidUserItem->GetChairID()==pApplyBanker->wApplyUser) 
		m_bMeApplyBanker=true;

	return true;
}

//取消做庄
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//自己判断
	if (lstrcmp(m_pIAndroidUserItem->GetMeUserItem()->GetNickName(),pCancelBanker->szCancelUser)==0) 
		m_bMeApplyBanker=false;

	return true;
}

//切换庄家
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	if ( m_wCurrentBanker == m_pIAndroidUserItem->GetChairID() && m_wCurrentBanker != pChangeBanker->wBankerUser )
	{
		//m_stlApplyBanker--;
		m_nWaitBanker = 0;
		m_bMeApplyBanker = false;
	}

	m_lWinScore=0;
	m_lScoreOld=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
	m_wCurrentBanker = pChangeBanker->wBankerUser;

	return true;
}

//读取配置
void CAndroidUserItemSink::ReadConfigInformation(TCHAR szFileName[], TCHAR szRoomName[], bool bReadFresh)
{
	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\%s"), szPath, szFileName);

	//每盘刷新
	if (bReadFresh)
	{
		//每盘刷新
		BYTE cbRefreshCfg = GetPrivateProfileInt(szRoomName, TEXT("Refresh"), 0, szConfigFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;
	}

	//筹码限制
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMaxJetton"), _T("5000"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, _tcslen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[1]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMinJetton"), _T("1"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, _tcslen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[0]);

	if (m_lRobotJettonLimit[1] > 5000)					m_lRobotJettonLimit[1] = 5000;
	if (m_lRobotJettonLimit[0] < 1)						m_lRobotJettonLimit[0] = 1;
	if (m_lRobotJettonLimit[1] < m_lRobotJettonLimit[0])	m_lRobotJettonLimit[1] = m_lRobotJettonLimit[0];

	//次数限制
	m_nRobotBetTimeLimit[0] = GetPrivateProfileInt(szRoomName, _T("RobotMinBetTime"), 4, szConfigFileName);;
	m_nRobotBetTimeLimit[1] = GetPrivateProfileInt(szRoomName, _T("RobotMaxBetTime"), 8, szConfigFileName);;

	if (m_nRobotBetTimeLimit[0] < 0)							m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0])		m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//是否坐庄
	m_bRobotBanker = (GetPrivateProfileInt(szRoomName, _T("RobotBanker"), 0, szConfigFileName) == 1);

	//坐庄次数
	m_nRobotBankerCount = GetPrivateProfileInt(szRoomName, _T("RobotBankerCount"), 3, szConfigFileName);

	//空盘重申
	m_nRobotWaitBanker = GetPrivateProfileInt(szRoomName, _T("RobotWaitBanker"), 3, szConfigFileName);

	//最多个数
	m_nRobotApplyBanker = GetPrivateProfileInt(szRoomName, _T("RobotApplyBanker"), 3, szConfigFileName);

	//最少个数
	m_nRobotListMinCount = GetPrivateProfileInt(szRoomName, TEXT("RobotListMinCount"), 2, szConfigFileName);

	//降低限制
	m_bReduceJettonLimit = (GetPrivateProfileInt(szRoomName, TEXT("RobotReduceLimit"), 0, szConfigFileName)!=0);

	//MyDebug(_T("机器人 读取配置 [%I64d %I64d %d %d] %d [%d %d %d] 下注 %d 下降 %d"), m_lRobotJettonLimit[0], m_lRobotJettonLimit[1],
	//	m_nRobotBetTimeLimit, m_nRobotBetTimeLimit, m_bRobotBanker, m_nRobotBankerCount, m_nRobotWaitBanker, m_nRobotApplyBanker, m_bReduceJettonLimit);
	m_nWinScore[0]=GetPrivateProfileInt(szRoomName, _T("RobotWinCount"), 100, szConfigFileName);
	m_nWinScore[1]=GetPrivateProfileInt(szRoomName, _T("RobotLostCount"), 80, szConfigFileName);


	//区域概率
	m_RobotInfo.nAreaChance[0] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance1"), 1, szConfigFileName);
	m_RobotInfo.nAreaChance[1] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance2"), 2, szConfigFileName);
	m_RobotInfo.nAreaChance[2] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance3"), 2, szConfigFileName);
	m_RobotInfo.nAreaChance[3] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance4"), 5, szConfigFileName);
	m_RobotInfo.nAreaChance[4] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance5"), 10, szConfigFileName);
	m_RobotInfo.nAreaChance[5] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance6"), 10, szConfigFileName);
	m_RobotInfo.nAreaChance[6] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance7"), 10, szConfigFileName);
	m_RobotInfo.nAreaChance[7] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance8"), 10, szConfigFileName);



}

//计算范围	(返回值表示是否可以通过下降下限达到下注)
bool CAndroidUserItemSink::CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[])
{
	//定义变量
	bool bHaveSetMinChip = false;

	//不够一注
	if (lMaxScore < m_RobotInfo.nChip[0])	return false;

	//配置范围
	for (int i = 0; i < CountArray(m_RobotInfo.nChip); i++)
	{
		if (!bHaveSetMinChip && m_RobotInfo.nChip[i] >= lChipLmt[0])
		{ 
			lJetLmt[0] = i;
			bHaveSetMinChip = true;
		}
		if (m_RobotInfo.nChip[i] <= lChipLmt[1])
			lJetLmt[1] = i;
	}
	if (lJetLmt[0] > lJetLmt[1])	lJetLmt[0] = lJetLmt[1];

	//是否降低下限
	if (m_bReduceJettonLimit)
	{
		if (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
		{
			//是否降低下注次数
			if (nChipTime * m_RobotInfo.nChip[0] > lMaxScore)
			{
				nChipTime = int(lMaxScore/m_RobotInfo.nChip[0]);
				lJetLmt[0] = 0;
				lJetLmt[1] = 0;
			}
			else
			{
				//降低到合适下限
				while (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
				{
					lJetLmt[0]--;
					ASSERT(lJetLmt[0]>=0);
				}
			}
		}
	}

	return true;
}

//取钱
void CAndroidUserItemSink::ReadConfigInfo()
{
	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\%s"), szPath, m_RobotInfo.szCfgFileName);

	BYTE cbLevelCount = GetPrivateProfileInt(TEXT("RobotScore"), TEXT("LevelCount"), 0, szConfigFileName);

	SCORE lScore = 0.00f;
	for(int i=0; i<cbLevelCount; i++)
	{ 
		TCHAR szKey[MAX_PATH];
		_sntprintf(szKey,sizeof(szKey),TEXT("Chair%d"),i+1);
		BYTE cbChairID = GetPrivateProfileInt(TEXT("RobotScore"), szKey, (1+i)*10, szConfigFileName);
		if (cbChairID > m_pIAndroidUserItem->GetChairID())
		{
			_sntprintf(szKey,sizeof(szKey),TEXT("Level%d"),i+1);
		//	lScore = GetPrivateProfileInt(TEXT("RobotScore"), szKey, 10000000, szConfigFileName);

			ZeroMemory(OutBuf, sizeof(OutBuf));
			GetPrivateProfileString(TEXT("RobotScore"),szKey, _T("1"), OutBuf, 255, szConfigFileName);
			//_sntscanf(OutBuf, _tcslen(OutBuf), _T("%.2lf"), &lScore);
			lScore = _wtof(OutBuf);

			break;
		}
	}

	SCORE lGetScore = lScore * (rand()%10+1);
	if (lScore>=1000)
	{
		lGetScore = lScore * (rand()%3+1);
	}
	
	
	CString strLog;
	strLog.Format(L"%s myscore:%.2lf,lGetScore = %.2lf,lScore = %.2lf",m_pIAndroidUserItem->GetMeUserItem()->GetNickName(),m_pIAndroidUserItem->GetMeUserItem()->GetUserScore(),lGetScore,lScore);
	CTraceService::TraceString(strLog,TraceLevel_Warning);

	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() && m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()<lGetScore)
	{
		m_pIAndroidUserItem->PerformTakeScore(lGetScore);
	}

	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() && !m_bMeApplyBanker&&m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()>lScore*10)
	{
		m_pIAndroidUserItem->PerformSaveScore(m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()/2);		
	}

	//大于5亿存进去一半
	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() && m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()>5000)
	{
		m_pIAndroidUserItem->PerformSaveScore(m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()/2);
	}

}

//取[nMin,nMax]之间的随机整数
int	CAndroidUserItemSink::GetRand(const int nMin,const int nMax)
{
	if ( nMin > nMax)
		return nMin;
	return rand()%(nMax-nMin+1)+nMin;
}

//根据概率选择，返回true的概率为p
bool CAndroidUserItemSink::SelectBoolByProb(float p)
{
	if(p>=1)
		return true;
	if(p<=0)
		return false;
	int P=(int)(p*1000+0.5); 
	int randNum = GetRand(0,1000);
	if(randNum<P)
		return true;
	return false;
}

//根据概率选择
int CAndroidUserItemSink::SelectByProb(int arr[],int arrProb[],int count,int total/*=0*/)
{
	if(total==0)
	{
		for(int i=0;i<count;i++)
		{
			total+=arrProb[i];
		}
	}
	int kind=0;
	int iRand =rand()%total;
	for(int i=0;i<count;i++)
	{
		if (iRand < arrProb[i])
		{
			kind=arr[i];
			break;
		}
		else
		{
			iRand -= arrProb[i];
		}
	}
	return kind;
}

//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);
//////////////////////////////////////////////////////////////////////////
