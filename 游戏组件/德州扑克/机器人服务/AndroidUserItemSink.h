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
	DZPKSCORE							m_lCellScore;						//单元下注
	DZPKSCORE							m_lTurnLessScore;					//最小下注
	DZPKSCORE							m_lTurnMaxScore;					//最大下注
	DZPKSCORE							m_lAddLessScore;					//加最小注
	DZPKSCORE							m_lTableScore[GAME_PLAYER];			//下注数目
	DZPKSCORE							m_lTotalScore[GAME_PLAYER];			//累计下注
	DZPKSCORE							m_lCenterScore;						//中心筹码
	DZPKSCORE							m_lBalanceScore;					//平衡筹码

	DZPKSCORE							m_lBeforeScore;						//用户押注
	
	int                             m_nCurCardCount;                    //当前牌数
	bool                            m_bGiveUp;                          //是否放弃

	bool							m_bWillWin;							//发牌结果  //机器人胜利标志
	bool							m_bShowHand;						//梭哈标志

	//状态变量
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//游戏状态
	BOOL							m_bAutoStart;							//自动开始
	DZPKSCORE							m_dEndScore[GAME_PLAYER];				//结束分数

	//扑克信息
protected:
	BYTE							m_cbCenterCardData[MAX_CENTERCOUNT];	//中心扑克
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//手上扑克
	BYTE							m_cbOverCardData[GAME_PLAYER][MAX_CENTERCOUNT];//结束扑克

	//控件变量
protected:
	ITableFrame						* m_pITableFrame;					//框架接口
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口
	wstring                         m_strLogFile;                       //日志文件
//银行操作
	LONGLONG						m_lRobotBankGetScore;					//取款最小数额(取款数是该值以上的一个随机数)
	LONGLONG						m_lRobotBankGetScoreBanker;			//取款最大数额(此数值一定要大于RobotBankGet)
	int								m_nRobotBankStorageMul;				//存款倍数
	LONGLONG						m_lBankerLostScore;							//庄家输分 (当机器人为庄)
	LONGLONG						m_lRobotScoreRange[2];					//最大范围

	TCHAR							m_szRoomName[32];			

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

	//功能函数
private:
	//生成日志
	void createLogFile(tagAndroidUserParameter * pAndroidUserParameter);
	//打印日志
	void printLog(char *szBuff,...);
	void TraceString(LPCTSTR pszString, enTraceLevel TraceLevel);

//	游戏消息
private:
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

	//功能函数
protected:

	//推算赢家
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
	//赢家是否为机器人
	bool WinIsAndroid(BYTE cbStartPos, BYTE cbConcludePos);
	//概率
	bool GaiLv(BYTE bNum);
	//跟注
	void FollowScore();
	//加注
	void AddScore();
	//梭哈
	void SendSuoHand();
	//下注
	void OnSubAddScoreEx();
	//放弃
	void GiveUpScore();

	//读取配置
	void ReadConfigInformation();

};

//////////////////////////////////////////////////////////////////////////

#endif
