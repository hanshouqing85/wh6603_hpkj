#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientEngine : public CGameFrameEngine
{
	BOOL							m_enableAddScore[4];
	//游戏变量
protected:
	__int64							m_lTurnMaxGold;						//最大下注
	__int64							m_lTurnBasicGold;					//跟注数目
	__int64							m_lBasicGold;						//单元数目
	BYTE							m_bPlayStatus[GAME_PLAYER];			//游戏状态
	WORD							m_wCurrentUser;						//
	__int64							m_lShowHandScore;					//限制最高分
	__int64							m_lUserScore[GAME_PLAYER];			//

	//辅助变量
protected:
	bool							m_bShowHand;						//是否梭哈
	__int64							m_lGoldShow;						//加注筹码
	TCHAR							szName[GAME_PLAYER][LEN_ACCOUNTS];	//玩家名字
	CMD_S_GameEnd					m_GameEnd;							//
	bool							m_bAddGold;							//是否已下注
	bool							m_bPoor			;					//是否财神以下
	

	//控件变量
public:
	CGameClientView					m_GameClientView;					//游戏视图
	//const tagServerAttribute		*pServerAttribute ;
	
	//函数定义
public:
	//构造函数
	CGameClientEngine();
	//析构函数
	virtual ~CGameClientEngine();

	//常规继承
private:
	//初始函数
	virtual bool OnInitGameEngine();
	//游戏设置
	virtual void OnGameOptionSet();

	//重置框架
	virtual bool OnResetGameEngine();
	//时间消息
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID);
	//删除定时器
	virtual bool OnEventGameClockKill(WORD wChairID);
	//旁观消息
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//网络消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏场景
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
	virtual void OnLookonViewChange(bool bLookon){return;};
	//virtual VOID __cdecl OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser);

	//消息处理
protected:
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

public:
	//声音控制
	virtual bool AllowBackGroundSound(bool bAllowSound){return true;}
	//消息映射
protected:
	//开始按钮
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	LRESULT	OnShowCard(WPARAM wParam, LPARAM lParam);       //看牌
	//放弃按钮
	LRESULT OnGiveUp(WPARAM wParam, LPARAM lParam);
	//跟注按钮
	LRESULT OnFollow(WPARAM wParam, LPARAM lParam);
	//梭哈按钮
	LRESULT OnShowHand(WPARAM wParam, LPARAM lParam);
	//加注消息
	LRESULT OnAddGold(WPARAM wParam, LPARAM lParam);
	//发牌完成
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
	// 胜负控制
	LRESULT OnUseWinner(WPARAM wParam, LPARAM lParam);
private:
	//
	void UpdateScoreControl();
	//
	void HideScoreControl();
	//
	void PerformGameEnd();

	DECLARE_MESSAGE_MAP()
	bool IsFreeze(void);
};

//////////////////////////////////////////////////////////////////////////
