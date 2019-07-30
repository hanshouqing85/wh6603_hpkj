#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "HistoryScore.h"

//////////////////////////////////////////////////////////////////////////

//用户追踪
struct tagUserTracing
{
	SCORE							lWinScore;								//胜利积分
	SCORE							lVarietyScore;							//变化积分
	SYSTEMTIME						SystemTimeTrace;						//记录时间
};

//////////////////////////////////////////////////////////////////////////

//游戏桌子
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//游戏记录
protected:
	CString							m_GameRecord;
	TCHAR							m_szNickName[GAME_PLAYER][LEN_NICKNAME];
	//游戏变量
protected:
	bool							m_bShowHand;						//梭哈标志
	WORD							m_wCurrentUser;						//当前用户
	CString							m_strPalyName[GAME_PLAYER];			//玩家名字

	//下注信息
protected:
	SCORE							m_lDrawMaxScore;					//最大下注
	SCORE							m_lTurnMaxScore;					//最大下注
	SCORE							m_lTurnLessScore;					//最小下注
	SCORE							m_lDrawCellScore;					//底注积分

	//用户状态
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//游戏状态
	BYTE							m_cbOperaScore[GAME_PLAYER];		//操作标志

	//金币信息
protected:
	SCORE							m_lUserScore[GAME_PLAYER];			//用户下注
	SCORE							m_lTableScore[GAME_PLAYER];			//桌面下注
	SCORE							m_lUserMaxScore[GAME_PLAYER];		//最大下注

	//扑克变量
protected:
	BYTE							m_cbSendCardCount;					//发牌数目
	BYTE							m_cbCardCount[GAME_PLAYER];			//扑克数目
	BYTE							m_cbHandCardData[GAME_PLAYER][5];	//桌面扑克

	//组件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CHistoryScore					m_HistoryScore;						//历史成绩

	//组件接口
protected:
	ITableFrame	*					m_pITableFrame;						//框架接口
	tagGameServiceOption *			m_pGameServiceOption;				//服务配置

//AI变量
protected:
//	LONGLONG						m_lUserScore[GAME_PLAYER];				//用户分数///////
	bool							m_bNeedWin[GAME_PLAYER];				//需要赢分///////
	bool							m_bAndroid[GAME_PLAYER];				//是否机器/////////
	LONGLONG						m_lScoreLimit[GAME_PLAYER];				//////////////////
	//AI变量
protected:
	///////
	bool							m_bRefreshCfg;							//每盘刷新
	bool							m_bRefreshStorageCfg;					//是否刷新库存
	//////
	static SCORE					m_lStockScore;							//总输赢分
	LONGLONG						m_lStorageDeduct;						//回扣变量 千分比

	LONGLONG						m_lStockLimit;							//总输赢分

	static LONGLONG					m_lStorageOff;							//库存减少值

	//梦成网络添加
	LONGLONG						m_lStorageMin;							//库存最小值
	LONGLONG						m_lStorageMax;							//库存最大值
	BYTE							StorageRatio;							//库存初值输赢比例百分之N		
	BYTE							StorageMinRatio;						//库存最小值输赢比例百分之N		杀分模式
	BYTE							StorageMaxRatio;						//库存最大值输赢比例百分之N		送分模式
	LONGLONG						StorageEating;							//吃分百分比,系统到达最大值后一次性吃掉这个值
	//梦成网络添加
	
	
	TCHAR							m_szFileName[MAX_PATH];
protected:
	//控制玩家输赢
	BYTE							m_WinCount;								//控制玩家个数
	bool							m_bWinnerCtrl;							//是否控制玩家赢


	DWORD							m_dwCheatGameID;						//作弊帐号
	DWORD							m_dwCheatCount;							//作弊次数
	BYTE							m_cbCheatType;							//作弊类型
	BYTE							m_WinningRatio;							//控制玩家机率

	DWORD							m_dwCheatGameID1;						//管理帐号

	//////玩家输赢比率
	bool							m_bAndroidWinningCtrl;					//是否控制机器人输赢比率
	LONGLONG				        m_AndroidWinningRatio;					//机器人赢的比率
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
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//配置桌子
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}

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
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//游戏事件
protected:
	//用户放弃
	bool OnUserGiveUp(WORD wChairID);
	//用户加注
	bool OnUserAddScore(WORD wChairID, SCORE lScore);
		//获取胜者
	bool OnUserGetWinner(IServerUserItem * pIServerUserItem);
	//管理员命令
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);

	//辅助函数
	//机器人作弊
	bool AndroidCheck();


	//辅助函数
protected:
	//调整下注
	VOID RectifyMaxScore();
	//发送扑克
	VOID DispatchUserCard();
		//读取配置
	VOID ReadConfigInformation(bool bReadFresh);
	//获取椅子号
	WORD GetChairIDFromGameID(DWORD dwGameID);

	//逻辑辅助
protected:
	//推断输者
	WORD EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos);
	//推断胜者
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
		//牌转文字
	CString TransformCardInfo( BYTE cbCardData );
	WORD EstimateWinnerEx(BYTE cbStartPos, BYTE cbConcludePos);

	WORD GetChairIdByGameId(DWORD dwGameId);

	void AndroidSendUserCard();

	void ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType);
};

//////////////////////////////////////////////////////////////////////////

#endif