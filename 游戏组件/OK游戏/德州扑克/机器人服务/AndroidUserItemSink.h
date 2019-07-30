#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//游戏变量
protected:
	WORD							m_wMeChairID;						//本身位置
	WORD							m_wDUser;							//D玩家
	WORD							m_wCurrentUser;						//当前玩家
	bool							m_bOpenCard;						//开牌
	bool							m_bExitTag;							//强退标志
	bool							m_bReset;							//复位标志
	TCHAR							m_bUserName[GAME_PLAYER][32];		//用户名字			

	//加注信息
protected:
	LONG							m_lCellScore;						//单元下注
	LONG							m_lTurnLessScore;					//最小下注
	LONG							m_lTurnMaxScore;					//最大下注
	LONG							m_lAddLessScore;					//加最小注
	LONG							m_lTableScore[GAME_PLAYER];			//下注数目
	LONG							m_lTotalScore[GAME_PLAYER];			//累计下注
	LONG							m_lCenterScore;						//中心筹码
	LONG							m_lBalanceScore;					//平衡筹码

	//状态变量
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//游戏状态
	BOOL							m_bAutoStart;							//自动开始
	LONG							m_dEndScore[GAME_PLAYER];				//结束分数

	//扑克信息
protected:
	BYTE							m_cbCenterCardData[MAX_CENTERCOUNT];	//中心扑克
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//手上扑克
	BYTE							m_cbOverCardData[GAME_PLAYER][MAX_CENTERCOUNT];//结束扑克
//	//限制信息
//protected:
//	LONG							m_lMeMaxScore;						//最大下注
//	LONG							m_lAreaLimitScore;					//区域限制
//	LONG							m_lApplyBankerCondition;			//申请条件
//
//	//个人下注
//protected:
//	LONG							m_lMeTieScore;						//买平总注
//	LONG							m_lMeBankerScore;					//买庄总注
//	LONG							m_lMePlayerScore;					//买闲总注
//	LONG							m_lMeTieSamePointScore;				//同点平注
//	LONG							m_lMePlayerKingScore;				//闲天王注
//	LONG							m_lMeBankerKingScore;				//庄天王注
//	LONG							m_lMePlayerTwoPair;					//对子下注
//	LONG							m_lMeBankerTwoPair;					//对子下注

//	//庄家信息
//protected:
//	LONG							m_lBankerScore;						//庄家积分
//	WORD							m_wCurrentBanker;					//当前庄家

//	//状态变量
//protected:
//	bool							m_bMeApplyBanker;					//申请标识

//	//配置路径
//protected:
//	TCHAR							m_szConfigFile[MAX_PATH];			//配置文件路径



	//控件变量
protected:
	ITableFrame						* m_pITableFrame;					//框架接口
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口
	//BYTE							m_cbWinSideControl;					//控制输赢

	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { }
	//是否有效
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CAndroidUserItemSink))?true:false; }
	//接口查询
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool  RepositionSink();

	//游戏事件
public:
	//时间消息
	virtual bool  OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool  OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//游戏消息
	virtual bool  OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//场景消息
	virtual bool  OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual void  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual void  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual void  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual void  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户段位
	virtual void  OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

//	//辅助函数
//private:
//	//个人下注
//	void SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount);
//	//设置庄家
//	void SetBankerInfo(WORD wBanker,LONG lScore);
//
//	//游戏消息
//private:
//	//游戏开始
//	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
//	//游戏空闲
//	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
//	//游戏结束
//	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);	
//	//切换庄家
//	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);

	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//用户放弃
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//发牌消息
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//开牌消息
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);

};

//////////////////////////////////////////////////////////////////////////

#endif
