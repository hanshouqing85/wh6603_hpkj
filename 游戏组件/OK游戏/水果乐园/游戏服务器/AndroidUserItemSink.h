#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"


////////////////////////////////////////////////////////////////////////////////

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
protected:
	LONGLONG							m_lAreaLimitScore;					//区域限制
	LONGLONG							m_lUserLimitScore;					//区域限制
	LONGLONG							m_lApplyBankerCondition;			//申请条件

	LONGLONG							m_lUserJettonScore[JETTON_AREA_COUNT];	//个人总注
	LONGLONG							m_lAllJettonScore[JETTON_AREA_COUNT];	//全体总注

	LONGLONG							m_lMeMaxScore;						//最大下注
	//庄家信息
protected:
	LONGLONG							m_lBankerScore;						//庄家积分
	LONGLONG							m_lBankerWinScore;					//庄家成绩
	WORD								m_wBankerUser;						//当前庄家
	WORD								m_wBankerTime;						//做庄次数

	bool								m_bEnableSysBanker;					//系统做庄

protected:
	BYTE								m_cbJettonArea;
	WORD								m_wJettonCount;

	bool								m_bMeCurrentBanker;
	bool								m_bApplyingBanker;
	bool								m_bCancelingBanker;
	WORD								m_wRandBankerTime;
	static int							m_stlApplyBanker;					//申请数
	DWORD								m_dwJettonRate;						//筹码比率

	//配置文件
protected:
	LONGLONG						    m_lRobotJettonLimit[2];				//筹码限制	
	BOOL								m_bAllowApplyBanker;				//机器人是否可以申请坐庄
	WORD								m_wMaxBankerTime;					//机器人坐庄次数
	int									m_nJettonRange;						//机器人下注范围
	int									m_nRobotApplyBanker;				//上庄个数

	TCHAR								m_szConfigFileName[MAX_PATH];		//配置文件
	TCHAR								m_szRoomName[32];					//配置房间

	IAndroidUserItem *					m_pIAndroidUserItem;				//用户接口

	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CAndroidUserItemSink))?true:false; }
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool RepositionSink();

public:
	//初始接口
	virtual bool  InitUserItemSink(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool  RepositUserItemSink();

	//游戏事件
public:
	//时间消息
	virtual bool OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//游戏消息
	virtual bool OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual void OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual void OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual void OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual void OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户段位
	virtual void OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//所有玩家都开始了
	//virtual void OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//消息处理
public:
	//游戏空闲
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//下注失败
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);

	//最大下注
	LONGLONG GetUserMaxJetton();
	//读取配置文件
	void ReadConfigInformation(bool bFirstRead);
};

////////////////////////////////////////////////////////////////////////////////
#endif
