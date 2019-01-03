#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include <math.h>
#include <algorithm>
using namespace std;

#define		ANDROID_BANKER_TEST
//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_REQUEST_BANKER			101									//申请定时
#define IDI_GIVEUP_BANKER			102									//下庄定时
#define IDI_PLACE_JETTON1			103									//下注定时
#define IDI_PLACE_JETTON2			104									//下注定时
#define IDI_PLACE_JETTON3			105									//下注定时
#define IDI_PLACE_JETTON4			106									//下注定时
#define IDI_PLACE_JETTON5			107									//下注定时
#define IDI_CHECK_BANKER			108									//检查上庄
#define IDI_BANK_OPERATE			109									//银行定时
#define IDI_PLACE_JETTON			110									//下注定义 (预留110-160)

//////////////////////////////////////////////////////////////////////////

int CAndroidUserItemSink::m_stlApplyBanker = 0L;
void RecordMessage(CString szinfo,DWORD id);

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
	m_lRecordScore = SCORE_ZERO;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	ZeroMemory(m_lAreaChip1, sizeof(m_lAreaChip1));

	//上庄变量
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	// 读取变量初始化
	m_lRobotBetLimit[1] = 1000;
	m_lRobotBetLimit[0] = 1;
	if (m_lRobotBetLimit[1] > 1000)					m_lRobotBetLimit[1] = 1000;
	if (m_lRobotBetLimit[0] < 1)						m_lRobotBetLimit[0] = 1;
	if (m_lRobotBetLimit[1] < m_lRobotBetLimit[0])	m_lRobotBetLimit[1] = m_lRobotBetLimit[0];

	//次数限制
	m_nRobotBetTimeLimit[0] = 4;
	m_nRobotBetTimeLimit[1] = 8;

	if (m_nRobotBetTimeLimit[0] < 0)							m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0])		m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//是否坐庄
	m_bRobotBanker = 0;

	//坐庄次数
	m_nRobotBankerCount = 3;

	//空盘重申
	m_nRobotWaitBanker = 3;

	//最多个数
	m_nRobotApplyBanker = 3;
	m_nRobotListMinCount = 2;

	//降低限制
	m_bReduceBetLimit = 0;

	//区域概率
	m_RobotInfo.nAreaChance[0] = 5;
	m_RobotInfo.nAreaChance[1] = 0;
	m_RobotInfo.nAreaChance[2] = 5;
	m_RobotInfo.nAreaChance[3] = 1;
	m_RobotInfo.nAreaChance[4] = 1;
	m_RobotInfo.nAreaChance[5] = 0;
	m_RobotInfo.nAreaChance[6] = 0;
	m_RobotInfo.nAreaChance[7] = 0;


	return;
}

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
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	ZeroMemory(m_lAreaChip1, sizeof(m_lAreaChip1));

	//上庄变量
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_BANK_OPERATE:
		{
			//变量定义
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			LONGLONG lRobotScore = pUserItem->GetUserScore();			
			//判断存取
			if (lRobotScore >m_lRobotScoreRange[1] )
			{
				LONGLONG lSaveScore=0L;

				lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
				if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

				if (lSaveScore > 0 && m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
				{
					m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
#ifdef ANDROID_BANKER_TEST
					CString szinfo=TEXT("");
					szinfo.Format(TEXT("%s,存款：%I64d,积分：%0.2f"),pUserItem->GetNickName(),lSaveScore,pUserItem->GetUserScore());
					RecordMessage(szinfo,pUserItem->GetUserID());
#endif
				}
			}
			else if (lRobotScore < m_lRobotScoreRange[0])
			{
				LONGLONG lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
				if (lScore > 0)
				{
					m_pIAndroidUserItem->PerformTakeScore(lScore);
#ifdef ANDROID_BANKER_TEST
					CString szinfo=TEXT("");
					szinfo.Format(TEXT("%s,取款：%I64d,积分：%0.2f"),pUserItem->GetNickName(),lScore,pUserItem->GetUserScore());
					RecordMessage(szinfo,pUserItem->GetUserID());
#endif
				}
			}
			
			return false;
		}
	case IDI_CHECK_BANKER:		//检查上庄
		{

			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
				return false;
			
			int nMinCount = m_nRobotApplyBanker;
			if ( m_nRobotApplyBanker > m_nRobotListMinCount)
				nMinCount = (rand()%(m_nRobotApplyBanker - m_nRobotListMinCount)) + m_nRobotListMinCount;
			
		if (m_wCurrentBanker == INVALID_CHAIR)
			{
				//空庄
				m_nWaitBanker++;

				//MyDebug(TEXT("机器人上庄(End) %d [%d %d] [%d %d]"), m_pIAndroidUserItem->GetChairID(), m_nWaitBanker, 
				//	m_nRobotWaitBanker, m_stlApplyBanker, m_nRobotApplyBanker);

				//机器人上庄
				if ( m_bRobotBanker
				&& !m_bMeApplyBanker 
				 && m_nWaitBanker >= m_nRobotWaitBanker
				  && m_stlApplyBanker < m_nRobotApplyBanker
				  && m_stlApplyBanker < nMinCount )
				{
					//合法判断
					IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
					if (pIUserItemBanker->GetUserScore() > m_lBankerCondition) 
					{
				
						//机器人上庄
						m_nBankerCount = 0;
						m_stlApplyBanker++;
						m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % m_cbTimeLeave) + 1);
					}
				}
			}
			else if ( m_wCurrentBanker != INVALID_CHAIR )
			{
				//其他人坐庄
				m_nWaitBanker = 0;
			}

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
	default:
		{
			if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON+MAX_CHIP_TIME)
			{
				srand(GetTickCount());

				//变量定义
				int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
				LONGLONG lMaxChipLmt = __min(m_lMaxChipBanker, m_lMaxChipUser);			//最大可下注值

				CString strLog;
// 				strLog.Format(L"ANDROIDJETTON m_lMaxChipBanker:%I64d,m_lMaxChipUser:%I64d",m_lMaxChipBanker,m_lMaxChipUser);
// 				OutputDebugString(strLog);
				WORD wMyID = m_pIAndroidUserItem->GetChairID();
				for (int i = 0; i < AREA_COUNT; i++)
					nACTotal += m_RobotInfo.nAreaChance[i];

				//统计次数
				m_nChipTimeCount++;

				//检测退出
				if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])
				{
					strLog.Format(L"ANDROIDJETTON 1 lMaxChipLmt:%I64d,nChip:%d, m_lMaxChipBanker:%I64d,m_lMaxChipUser:%I64d",lMaxChipLmt,m_RobotInfo.nChip[m_nChipLimit[0]],m_lMaxChipBanker,m_lMaxChipUser);
					OutputDebugString(strLog);
					return false;
				}
				for (int i = 0; i < AREA_COUNT; i++)
				{
					if (m_lAreaChip[i] >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
					if (i == AREA_COUNT-1)
					{
						strLog.Format(L"ANDROIDJETTON 2 i:%d,area:%d",i,AREA_COUNT-1);
						OutputDebugString(strLog);

						return false;
					}
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
				LONGLONG lLeaveScore = 0;
				lLeaveScore = (m_lMaxChipBanker-m_lAreaChip1[nChipArea]);

				lMaxChipLmt = __min(lLeaveScore, m_lMaxChipUser);
				if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])
				{
					strLog.Format(L"ANDROIDJETTON 3 lMaxChipLmt:%I64d,nChip:%d, m_lMaxChipBanker:%I64d,m_lMaxChipUser:%I64d",lMaxChipLmt,m_RobotInfo.nChip[m_nChipLimit[0]],m_lMaxChipBanker,m_lMaxChipUser);
					OutputDebugString(strLog);
					return false;
				}
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
					//多下控制 (当庄家金币较少时会尽量保证下足次数)
					if (m_nChipTimeCount < m_nChipTime)
					{
						LONGLONG lLeftBet = LONGLONG( (lMaxChipLmt-m_RobotInfo.nChip[nCurChip])/(m_nChipTime-m_nChipTimeCount) );

						//不够次数 (即全用最小限制筹码下注也少了)
						if (lLeftBet < m_RobotInfo.nChip[m_nChipLimit[0]] && nCurChip > m_nChipLimit[0])
							nCurChip--;
					}
				}

				/*ASSERT( MyDebug(TEXT("机器人下注 %d 下注次数 [%d/%d] 下注 [%d %d] 范围 [%d %d] 限制 [%I64d %I64d %I64d]"), wMyID, nTimerID-IDI_PLACE_JETTON, m_nChipTime, 
					nChipArea, m_RobotInfo.nChip[nCurChip], m_nChipLimit[0], m_nChipLimit[1], m_lMaxChipBanker, m_lMaxChipUser, lMaxChipLmt) );*/

				//变量定义
				CMD_C_PlaceBet PlaceBet = {};
				//构造变量
				PlaceBet.cbBetArea = nChipArea;		//区域宏从1开始
				PlaceBet.lBetScore = m_RobotInfo.nChip[nCurChip];

				//发送消息
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceBet, sizeof(PlaceBet));
			}

			m_pIAndroidUserItem->KillGameTimer(nTimerID);
			return false;
		}
	}
	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
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
			return OnSubPlaceBet(pBuffer, wDataSize);
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
	}

	//错误断言
//	ASSERT(FALSE);

	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lUserLimitScore = pStatusFree->lPlayFreeSocre;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			m_lBankerCondition = pStatusFree->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);

			//MyDebug(TEXT("机器人上庄(Free) %d [%d %d] [%d %d]"), m_pIAndroidUserItem->GetChairID(), m_nWaitBanker, 
			//	m_nRobotWaitBanker, m_stlApplyBanker, m_nRobotApplyBanker);

			//上庄处理
			if (pStatusFree->wBankerUser == INVALID_CHAIR)
			{
				if (m_bRobotBanker && m_nRobotWaitBanker == 0  && m_stlApplyBanker < m_nRobotApplyBanker)
				{
					//合法判断
					IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
					if (pIUserItemBanker->GetUserScore() > m_lBankerCondition) 
					{
						//机器人上庄
						m_nBankerCount = 0;
						m_stlApplyBanker++;
						
						BYTE cbTime = (pStatusFree->cbTimeLeave>0?(rand()%pStatusFree->cbTimeLeave+1):2);
						if (cbTime == 0) cbTime = 2;

						m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, cbTime);
					}
				}
			}

			return true;
		}
	case GAME_SCENE_PLAY:		//游戏状态
	case GAME_SCENE_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//庄家信息
			m_wCurrentBanker = pStatusPlay->wBankerUser;
			m_lUserLimitScore = pStatusPlay->lPlayBetScore;
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			m_lBankerCondition = pStatusPlay->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);

			return true;
		}
	}

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



//游戏空闲
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//读取配置
	if (m_bRefreshCfg)
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
			//机器人做庄
			m_nBankerCount = 0;
			m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

			bMeGiveUp = true;
		}
	}
	
	
	//检查上庄
	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() || bMeGiveUp)
	{
		
		m_cbTimeLeave = pGameFree->cbTimeLeave - 3;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER, 3);
	}

		//银行操作
	if (pGameFree->cbTimeLeave > 2)
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, 2);
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

	srand(GetTickCount());

	//自己当庄或无下注机器人
	if (pGameStart->wBankerUser == m_pIAndroidUserItem->GetChairID() || pGameStart->nChipRobotCount <= 0)
		return true;

	//设置变量
	m_lMaxChipBanker = pGameStart->lBankerScore/m_RobotInfo.nMaxTime;
	m_lMaxChipUser = pGameStart->lPlayBetScore/m_RobotInfo.nMaxTime;
	m_wCurrentBanker = pGameStart->wBankerUser;
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	ZeroMemory(m_lAreaChip1, sizeof(m_lAreaChip1));
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

	CString strLog;
	strLog.Format(L"ANDROIDJETTON m_lMaxChipBanker:%I64d,m_lMaxChipUser:%I64d,lBankerScore:%I64d,lPlayBetScore:%I64d,nMaxTime:%d",m_lMaxChipBanker,m_lMaxChipUser,pGameStart->lBankerScore,pGameStart->lPlayBetScore,m_RobotInfo.nMaxTime);
	OutputDebugString(strLog);


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
	if (!CalcBetRange(__min(m_lMaxChipBanker, m_lMaxChipUser), m_lRobotBetLimit, m_nChipTime, m_nChipLimit))
		return true;

	//设置时间
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

	//ASSERT( MyDebug(TEXT("机器人 %d 下注次数 %d 范围 [%d %d] 总人数 %d 限制 [%I64d %I64d] 上庄 [%d %d]"), wMyID, m_nChipTime, m_nChipLimit[0], m_nChipLimit[1], 
	//	pGameStart->nChipRobotCount, m_lMaxChipBanker, m_lMaxChipUser, m_stlApplyBanker, m_nRobotApplyBanker) );

	return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubPlaceBet(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceBet));
	if (wDataSize!=sizeof(CMD_S_PlaceBet)) return false;

	//消息处理
	CMD_S_PlaceBet * pPlaceBet=(CMD_S_PlaceBet *)pBuffer;

	//设置变量
//	m_lMaxChipBanker -= pPlaceBet->lBetScore;


	m_lAreaChip1[pPlaceBet->cbBetArea-1] += pPlaceBet->lBetScore;
	if (pPlaceBet->wChairID == m_pIAndroidUserItem->GetChairID())
		m_lMaxChipUser -= pPlaceBet->lBetScore;

	return true;
}

//下注失败
bool CAndroidUserItemSink::OnSubPlaceBetFail(const void * pBuffer, WORD wDataSize)
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
	if ( m_pIAndroidUserItem->GetMeUserItem()->GetChairID() == pCancelBanker->wCancelUser ) 
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
		m_stlApplyBanker--;
		m_bMeApplyBanker = false;
	}
	m_wCurrentBanker = pChangeBanker->wBankerUser;
	m_nWaitBanker = 0;

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
	_sntprintf(szConfigFileName, CountArray(szConfigFileName), TEXT("%s\\%s"), szPath, szFileName);

	//每盘刷新
	if (bReadFresh)
	{
		//每盘刷新
		BYTE cbRefreshCfg = GetPrivateProfileInt(szRoomName, TEXT("Refresh"), 0, szConfigFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;
	}

	//筹码限制
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMaxBet"), TEXT("1000"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lRobotBetLimit[1]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMinBet"), TEXT("1"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lRobotBetLimit[0]);

	if (m_lRobotBetLimit[1] > 1000)					m_lRobotBetLimit[1] = 1000;
	if (m_lRobotBetLimit[0] < 1)						m_lRobotBetLimit[0] = 1;
	if (m_lRobotBetLimit[1] < m_lRobotBetLimit[0])	m_lRobotBetLimit[1] = m_lRobotBetLimit[0];

	//次数限制
	m_nRobotBetTimeLimit[0] = GetPrivateProfileInt(szRoomName, TEXT("RobotMinBetTime"), 1, szConfigFileName);;
	m_nRobotBetTimeLimit[1] = GetPrivateProfileInt(szRoomName, TEXT("RobotMaxBetTime"), 3, szConfigFileName);;

	if (m_nRobotBetTimeLimit[0] < 0)							m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0])		m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//是否坐庄
	m_bRobotBanker = (GetPrivateProfileInt(szRoomName, TEXT("RobotBanker"), 0, szConfigFileName) == 1);

	//坐庄次数
	m_nRobotBankerCount = GetPrivateProfileInt(szRoomName, TEXT("RobotBankerCount"), 3, szConfigFileName);

	//空盘重申
	m_nRobotWaitBanker = GetPrivateProfileInt(szRoomName, TEXT("RobotWaitBanker"), 3, szConfigFileName);

	//最多个数
	m_nRobotApplyBanker = GetPrivateProfileInt(szRoomName, TEXT("RobotApplyBanker"), 3, szConfigFileName);

	//最少个数
	m_nRobotListMinCount = GetPrivateProfileInt(szRoomName, TEXT("RobotListMinCount"), 2, szConfigFileName);
	//降低限制
	m_bReduceBetLimit = (GetPrivateProfileInt(szRoomName, TEXT("RobotReduceLimit"), 0, szConfigFileName)!=0);

	//分数限制
	m_lRobotScoreRange[0] = GetPrivateProfileInt(szRoomName, _T("RobotScoreMin"), 100, szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt(szRoomName, _T("RobotScoreMax"), 500, szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//提款数额
	m_lRobotBankGetScore = GetPrivateProfileInt(szRoomName, _T("RobotBankGet"), 300, szConfigFileName);

	//提款数额 (庄家)
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt(szRoomName, _T("RobotBankGetBanker"), 500, szConfigFileName);

	//存款倍数
	m_nRobotBankStorageMul = GetPrivateProfileInt(szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

	//MyDebug(TEXT("机器人 读取配置 [%I64d %I64d %d %d] %d [%d %d %d] 下注 %d 下降 %d"), m_lRobotBetLimit[0], m_lRobotBetLimit[1],
	//	m_nRobotBetTimeLimit, m_nRobotBetTimeLimit, m_bRobotBanker, m_nRobotBankerCount, m_nRobotWaitBanker, m_nRobotApplyBanker, m_bReduceBetLimit);

	//区域概率
	m_RobotInfo.nAreaChance[0] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance1"), 3, szConfigFileName);
	m_RobotInfo.nAreaChance[1] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance2"), 0, szConfigFileName);
	m_RobotInfo.nAreaChance[2] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance3"), 3, szConfigFileName);
	m_RobotInfo.nAreaChance[3] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance4"), 1, szConfigFileName);
	m_RobotInfo.nAreaChance[4] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance5"), 1, szConfigFileName);
	m_RobotInfo.nAreaChance[5] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance6"), 0, szConfigFileName);
	m_RobotInfo.nAreaChance[6] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance7"), 0, szConfigFileName);
	m_RobotInfo.nAreaChance[7] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance8"), 0, szConfigFileName);
}

//计算范围	(返回值表示是否可以通过下降下限达到下注)
bool CAndroidUserItemSink::CalcBetRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[])
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
	if (m_bReduceBetLimit)
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

//输出信息
void RecordMessage(CString szinfo,DWORD id)
{
	if(!szinfo.IsEmpty())
		szinfo.Replace(TEXT("#"),TEXT("\r\n"));
	
	COleDateTime timeNow, dateNow;
	timeNow = COleDateTime::GetCurrentTime();     // 获取当前日期时间
	dateNow = COleDateTime::GetCurrentTime();     // 同样获取当前日期时间
	CString sTime = timeNow.Format(VAR_TIMEVALUEONLY);     // 获取当前时间
	CString sDate = dateNow.Format(VAR_DATEVALUEONLY);     // 获取当前日期

	TCHAR tchBuffer[MAX_PATH ]; 
	LPTSTR lpszCurDir;
	lpszCurDir = tchBuffer; 
	GetCurrentDirectory(MAX_PATH , lpszCurDir);//得到当前程序路径

	CString Filep = lpszCurDir;
	Filep.AppendFormat(TEXT("\\%s"),GAME_NAME);

	//创建文件夹	游戏分类
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			return;
		 }
	}
	Filep.AppendFormat(TEXT("\\USERID_%ld"),id);

	//创建文件夹	桌子分类
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			return;
		 }
	}
	Filep.AppendFormat(TEXT("\\%s"),sDate);
	
	//创建文件夹	日期分类
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			return;
		 }
	}

	
	CString FileName=TEXT("");     // 文件名 = 日期_时间.txt
	FileName.AppendFormat(TEXT("%s\\%s.txt"),Filep,sTime);

	TCHAR pFilePath[MAX_PATH];
	LPCTSTR filepath = FileName;
	int FilePathLength = FileName.GetLength();     // 获得文件名长度，CFile不能用CString，只能用string做参数，要做转换
	int i=0;
	for( i=0; i < FilePathLength ; i++)
	{
		pFilePath[i] = FileName.GetAt(i);     // CString -> string
		if( (pFilePath[i] == TEXT(':')) && (i != 1))     // 剔除':'等不能作为文件名的符号,并保留根目录后的冒号
			pFilePath[i] = '-';
	}
	pFilePath[i] = '\0';     // 文件名结束

	CFile File;
	if ((File.Open(pFilePath,CFile::modeWrite)==FALSE)&&
		(File.Open(pFilePath,CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	//梦成网络
	ULONGLONG dwFileLen = File.GetLength();
	if (0 == dwFileLen) // 文件为空时写入UNICODE字节序标记
	{
		const unsigned char LeadBytes[]  = {0xff, 0xfe};
		File.Write(LeadBytes, sizeof(LeadBytes));
	}

	File.SeekToEnd();
	File.Write(szinfo.GetBuffer(),szinfo.GetLength()*2);

	szinfo.ReleaseBuffer();
	File.Flush();
	File.Close();

	return;
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

//////////////////////////////////////////////////////////////////////////
