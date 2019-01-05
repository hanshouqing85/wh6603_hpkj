#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////
//宏定义

//最大下注次数
#define MAX_CHIP_TIME								50
//////////////////////////////////////////////////////////////////////////

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//游戏变量
protected:
	LONGLONG						m_lMaxChipBanker;					//最大下注 (庄家)
	LONGLONG						m_lMaxChipUser;						//最大下注 (个人)
	LONGLONG						m_lAreaChip[AREA_COUNT];			//每个区域的可下注数量(0~配置值) 
	WORD							m_wCurrentBanker;					//庄家位置
	BYTE							m_cbTimeLeave;						//剩余时间

	int								m_nChipLimit[2];					//下注筹码面值索引范围(0~CHIP_COUNT-1)
	int								m_nChipTime;						//下注次数 (本局)
	int								m_nChipTimeCount;					//已下次数 (本局)
	
	//上庄变量
protected:
	bool							m_bMeApplyBanker;					//申请标识
	int								m_nBankerCount;						//本机器人的坐庄次数
	int								m_nWaitBanker;						//空几盘
	static int						m_stlApplyBanker;					//申请数

	//配置变量  (全局配置)
protected:
	tagRobotInfo					m_RobotInfo;						//全局配置
	TCHAR							m_szRoomName[32];					//配置房间

	//配置变量	(游戏配置)
protected:
	bool							m_bRefreshCfg;						//每盘刷新
	LONGLONG						m_lAreaLimitScore;					//区域限制(每个注区的最高封顶数量)
	LONGLONG						m_lUserLimitScore;					//下注限制
	LONGLONG						m_lBankerCondition;					//上庄条件		

	//配置变量  (机器人配置)
protected:
	vector<int>                     m_vRobotChipChance;                 //机器人下注筹码面值从小到大的概率权重
	LONGLONG						m_lRobotJettonLimit[2];				//筹码限制(注意是面值，不是面值索引)	
	int								m_nRobotBetTimeLimit[2];			//次数限制	
	bool							m_bRobotBanker;						//是否坐庄
	int								m_nRobotBankerCount;				//坐庄次数
	int								m_nRobotWaitBanker;					//空盘重申
	int								m_nRobotApplyBanker;				//上庄个数
	bool							m_bReduceJettonLimit;				//降低限制
	int								m_nRobotListMinCount;				//最少上庄个数
	INT64                           m_nListUserCount;					//列表人数


	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口
	wstring                         m_strLogFile;                       //日志文件

	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//控制接口
public:
	//初始接口
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool RepositionSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { }
	//接口查询
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool  InitUserItemSink(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool  RepositUserItemSink();

	//游戏事件
public:
	//时间消息
	virtual bool  OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool  OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	/*
		框架消息
		wCmdID=低字节mid+高字节sid
		考虑兼容性，复用此虚方法
	*/
	virtual bool  OnEventFrameMessage(WORD wCmdID, void * pData, WORD wDataSize);
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

	//功能函数
public:
	//读取配置
	void ReadConfigInformation(TCHAR szFileName[], TCHAR szRoomName[], bool bReadFresh);

	/* --------------------------------------------------------------------------
	函数说明：计算下注筹码面值索引范围(0~CHIP_COUNT-1)
	传入参数：
		lMaxScore	最大下注，其值等于庄家赔付限制和玩家赔付限制的较小者
		lChipLmt[0]	机器人筹码范围(下限)，也即下注筹码最小的面值
		lChipLmt[1]	机器人筹码范围(上限)，也即下注筹码最大的面值
	传入传出参数：
		nChipTime	下注次数(本局)
	传出参数：
		lJetLmt[0]，考虑了赔付限制的下注筹码最小的面值索引
		lJetLmt[1]，考虑了赔付限制的下注筹码最大的面值索引
	返回值：
		成功则返回true，表示可以下注
	--------------------------------------------------------------------------*/
	bool CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[]);

	//生成日志
	void createLogFile(tagAndroidUserParameter * pAndroidUserParameter);
	//打印日志
	void printLog(char *szBuff,...);
	void printLog(CString& str);
};

//////////////////////////////////////////////////////////////////////////

#endif
