#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
struct OtherInfo
{
	BYTE    cbCard[5];
	bool    bGiveUp;
	bool    bStatue;
};

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//游戏变量
protected:
	SCORE							m_lCellScore;						//游戏底注
	SCORE						m_lUserScore[GAME_PLAYER];			//用户下注
	SCORE						m_lTableScore[GAME_PLAYER];			//桌面下注
    WORD							m_wCurrentUser;						//当前用户
	bool                            m_bFirstChip;                       //初步下注
	SCORE						m_lTurnLessScore;					//最小下注
	SCORE						m_lDrawMaxScore;					//最大下注
	SCORE						m_lTurnMaxScore;					//最大下注
	int                             m_nCurCardCount;                    //当前牌数
	bool                            m_bGiveUp;                          //是否放弃
	bool							m_bWillWin;							//发牌结果
	BYTE							m_cbTableCardData[GAME_PLAYER][5];	//桌面扑克
	bool							m_bShowHand;						//梭哈标志
	WORD							m_wSpeciaChairID;					//特殊用户		

	SCORE						m_lBeforeUserScore;					//上一个用户加注
	//扑克变量
protected:
	
	//手上扑克
protected:
	BYTE                           m_cbHandCard[5];                     //手上扑克
	OtherInfo                      m_cbOtherCard[5];                    //其它家信息
	bool                           m_bAddChip; 
	bool						   m_bTemp;								//更改

	//历史扑克
protected:

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口

	//银行操作
	LONGLONG						m_lRobotBankGetScore;					//取款最小数额(取款数是该值以上的一个随机数)
	LONGLONG						m_lRobotBankGetScoreBanker;			//取款最大数额(此数值一定要大于RobotBankGet)
	int								m_nRobotBankStorageMul;				//存款倍数
	LONGLONG						m_lBankerLostScore;							//庄家输分 (当机器人为庄)
	LONGLONG						m_lRobotScoreRange[2];					//最大范围

	TCHAR							m_szRoomName[32];					//配置房间
	

	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool RepositionSink();

	//游戏事件
public:
	//时间消息
	virtual bool OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏消息
	virtual bool OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual VOID OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual VOID OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual VOID OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual VOID OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户段位
	virtual VOID OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//消息处理
protected:
	//游戏开始
	bool OnSubGameStart(VOID * pData, WORD wDataSize);
	//用户放弃
	bool OnSubGiveUp(VOID * pData, WORD wDataSize);
	//用户加注
	bool OnSubAddScore(VOID * pData, WORD wDataSize);
	//发送扑克
	bool OnSubSendCard(VOID * pData, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(VOID * pData, WORD wDataSize);
	//梭哈
	void SendSuoHand();

	//功能函数
protected:
	//获取下注金额
	LONGLONG GetAddChip();
	//推算赢家
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
	//赢家是否为机器人
	bool WinIsAndroid(BYTE cbStartPos, BYTE cbConcludePos);
	//概率
	bool GaiLv(BYTE bNum);
	//跟注
	void FllowScore();
	//加注
	void AddScore();
	//下注
	void OnSubAddScoreEx();
	//放弃
	void GiveUpScore();

	//读取配置
	void ReadConfigInformation();

};

//////////////////////////////////////////////////////////////////////////

#endif