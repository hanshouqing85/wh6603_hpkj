#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include <map>
//////////////////////////////////////////////////////////////////////////

//结束原因
#define GER_NO_PLAYER						0x10						//没有玩家
#define USER_LEVEL                          3                            //游戏等级
#define USER_LEVEL_ZERO                     0                            //无敌等级
#define USER_LEVEL_FIRST                    1                           //财神等级
#define USER_LEVEL_SECOND                   2                           //首富等级
//聊天结构
struct CMD_GF_UserChat
{
	WORD								wChatLength;					//信息长度
	COLORREF							crFontColor;					//信息颜色
	DWORD								dwSendUserID;					//发送用户
	DWORD								dwTargetUserID;					//目标用户
	TCHAR								szChatMessage[1024];	//聊天信息
};


typedef struct tagPersonInfo
{
	DWORD dwUserID;           // 玩家ID
	int iPercent;             // 胜率
}S_PERSONINFO;
typedef CWHArray<S_PERSONINFO,S_PERSONINFO&> CPensonInfoArray;

struct tagSingleUserCtrl
{
	BYTE							cbCheatUser;
	DWORD							dwUserID;
	WORD							wChairID;
	BYTE							cbWinLosePercent;
	BYTE							cbLoseWin;
	LONGLONG						lCtrlScore;
};
#define MAX_CTRL_USER_COUNT 10

typedef CWHArray<tagSingleUserCtrl*> CSingleUserCtrlArray;
//////////////////////////////////////////////////////////////////////////

struct tagChageInfo 
{
	WORD wChairID;
	BYTE cbCardData;
};
//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//游戏变量
protected:
	std::map<DWORD,__int64>			m_UserLevel;

	WORD							m_wOperaCount;							//操作次数
	WORD							m_wCurrentUser;							//当前用户
	BYTE							m_bSendCardCount;						//发牌数目
	BYTE							m_cbCardCount[GAME_PLAYER];				//扑克数目
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];		//桌面扑克
	BYTE							m_cbGameEndReason;						//游戏结束理由
	bool							m_bGameEnd;								//
	bool							m_bShowHand;							//
	LONG							m_lSessionID;

	bool							m_bPoor;								//是否无敌以下级别
	BYTE							m_cbCardArray[28];							//扑克列表
	INT								m_nCardIndex;							//扑克索引
	//金币信息
protected:
	DOUBLE							m_lMaxGold;								//最大下注
	DOUBLE							m_lTurnMaxGold;							//
	DOUBLE							m_lBasicGold;							//单元下注
	DOUBLE                         m_lMaxShowhandGold;                     // 梭哈时最大金币数
	DOUBLE							m_lTurnBasicGold;						//最少下注
	DOUBLE							m_lTableGold[2*GAME_PLAYER];			//下注金币
	DOUBLE							m_lUserScore[GAME_PLAYER];				//
    DOUBLE                         m_lUserLevel[USER_LEVEL];                         //玩家级别分数

	//辅助变量
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//游戏状态
	WORD                            m_wCtrlWinner;                          // 赢的玩家
	CPensonInfoArray                m_arPensonInfo;
	CSingleUserCtrlArray			m_SingleUserArray;						//单个玩家控制
	tagSingleUserCtrl*				m_tagSingleUserCtrl;					//单玩家控制
	DWORD							m_dwSingleCtrlUser;						//受控的单个玩家
	bool							m_bCtrlWin;								//受控的单个玩家

	//控制变量
protected:
	TCHAR							m_szConfigFileName[MAX_PATH];			//配置文件

	__int64							m_nAllTrunsGold;						//总金币数
	__int64							m_nAIWinTrunGold;						//机器人赢局总金币数
	float							m_fAIWinPercent;					//参考失败率
	float							m_fAILostPercent;						//参考败率

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	tagGameServiceOption *			m_pGameServiceOption;					//游戏配置
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//游戏属性

	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数
//	static const enStartMode        m_GameStartMode;						//开始模式
	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	////开始模式
	//enStartMode  CTableFrameSink::GetGameStartMode();
	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//数据事件
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize){return true;}

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser){return true;}
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser){return true;}
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//查询服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//事件接口
public:
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//设置积分
	virtual void SetGameBaseScore(LONG lBaseScore) { }


	bool OnSubSupperApplay(const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//游戏事件
protected:
	//放弃事件
	bool OnUserGiveUp(WORD wChairID);
	//加注事件
	bool OnUserAddGold(WORD wChairID, __int64 lGold, bool bCancel,bool bFllow);
	//游戏函数
protected:
	//推断胜者
	//游戏状态
	bool IsUserPlaying(WORD wChairID);
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
	WORD EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos);

	//统计机器人赢局
	void CountAIWinTruns(const CMD_S_GameEnd &GameEnd);

	bool IsControlUser(DWORD dwUserID);
	bool WriteSingleUserScore(DWORD dwUserID,LONGLONG lWinScore,bool bWin);

};

//////////////////////////////////////////////////////////////////////////

#endif

