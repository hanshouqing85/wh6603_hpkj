//
//??:????6602?6603,???6602??????6603???
//??:???
//??:2012.02 
//Q Q :349126394
//QQ?:101494119
//??:shaojiang216@163.com
//??:http://shaojiang216.blog.163.com/ 
#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
typedef CWHArray<DWORD,DWORD&> CApplyBankerArray;

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	TCHAR							m_szConfigFileName[MAX_PATH];			//配置文件
	int								iTIME_RAND;
	int								iTIME_LESS;

	//游戏变量
protected:
	__int64							m_lTurnMaxGold;						//最大下注
	__int64							m_lTurnBasicGold;					//跟注数目
	__int64							m_lBasicGold;						//单元数目
	BYTE							m_bPlayStatus[GAME_PLAYER];			//游戏状态
	WORD							m_wCurrentUser;						// 当前下注者
	__int64							m_lShowHandScore;					//限制最高分
	bool							m_bShowHand;						//是否梭哈
	BYTE m_byCardCount;                                                 // 每个人手中发了几张牌
	__int64                         m_lMeTableGold;				        // 自己所下的总数
	__int64                         m_lMeTurnGold;				        // 自己一轮所下的总数
	__int64                         m_lMeScore;                         // 自己携带的总数
	bool                            m_bCtrlWinner;                      // 自己是否为赢家
	bool                            m_bFirstUser;                       // 是否为第一个人说话                     

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口

	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//????
	virtual VOID Release() { delete this; }
	//????
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//????
public:
	//????
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//????
	virtual bool RepositionSink();

	//????
public:
	//????
	virtual bool OnEventTimer(UINT nTimerID);
	//????
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//????
	virtual bool OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//????
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//????
public:
	//????
	virtual VOID OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//????
	virtual VOID OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//????
	virtual VOID OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//????
	virtual VOID OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//????
	virtual VOID OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	// ????????
	virtual void OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubAddGold(const void * pBuffer, WORD wDataSize);
	//用户放弃
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//发牌消息
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	// 下注处理
	bool OnPlaceJetton(void);
};

//////////////////////////////////////////////////////////////////////////

#endif
