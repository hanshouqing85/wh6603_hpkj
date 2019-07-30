#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameEngine
{
	//友元声明
	friend class CGameClientView;

	//限制信息
protected:
	__int64							m_lMeMaxScore;						//最大下注
	__int64							m_lApplyBankerCondition;			//庄家申请条件
	__int64							m_lUserChipCount;					//用户兑换后的筹码数
	__int64							m_lMeMaxScoreAfterEx;				//兑换后的金币数
	bool							m_bFirstEnter;						//控制兑换的初始值

	//个人下注
protected:
	__int64							m_lMeAreaChipArray[JETTON_AREA_COUNT];
	__int64							m_lXuYaArray[JETTON_AREA_COUNT];	//续押用的数组

	//庄家信息
protected:
	__int64							m_lBankerChip;						//庄家积分
	WORD							m_wCurrentBanker;					//当前庄家
	bool							m_bEnableSysBanker;					//系统做庄

	//状态变量
protected:
	bool							m_bMeApplyBanker;					//本是否申请了上庄

public:
	__int64							m_lCurrentChip[GAME_PLAYER];
	DWORD							m_dwChipRate;
	__int64							m_lMyCurrentChip;					//我当前的筹码
	CGlobalUnits					*m_pGlobalUnits;
	bool							m_bAlreadyExchange;
	bool							m_bXuYaContinue;					//连续续押
	BYTE							m_cbXuYaContinue;					//连续续押次数

	//控件变量
public:
	CGameClientView					m_GameClientView;					//游戏视图

	//函数定义
public:
	//构造函数
	CGameClientDlg();
	//析构函数
	virtual ~CGameClientDlg();

	//常规继承

	//设置筹码比率
	void SetJettonRate(DWORD dwJettonRate);
	//积分转换成筹码
	__int64 ScoreToChip(__int64 lScore);
	//筹码转换成积分
	__int64 ChipToScore(__int64 lChip);
	//兑换筹码
	void ExchangeChip(__int64 lChip);
	LRESULT OnGameSound(WPARAM wParam, LPARAM lParam);
	//特殊账号控制
	LRESULT OnAdminControl(WPARAM wParam, LPARAM lParam);
	//连续续押20局
	void OnXuYaContinue();

private:
	//初始函数
	virtual bool OnInitGameEngine();
	//重置框架
	virtual bool OnResetGameEngine();
	//游戏设置
	virtual void OnGameOptionSet();
	//时钟删除
	virtual bool OnEventGameClockKill(WORD wChairID) {return true;}
	//时钟信息
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);
	//旁观状态
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
	//旁观消息
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize) {return true;}
	//用户进入
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户离开
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//背景音乐
	virtual bool AllowBackGroundSound(bool bAllowSound);
	//消息处理
protected:
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//游戏空闲
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//游戏记录
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//下注失败
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//猜大小结果
	bool OnSubBigSmall(const void * pBuffer, WORD wDataSize);
	//兑换筹码
	bool OnSubChipUpdate(const void * pBuffer, WORD wDataSize);
	//特殊账号控制
	bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);

	//赋值函数
protected:
	//设置庄家
	void SetBankerInfo(WORD wBanker,__int64 lChip);
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount);

	//功能函数
protected:
	//更新控制
	void UpdateButtonContron();	

	//消息映射
protected:
	//加注消息
	void OnPlaceJetton(BYTE lJettonArea, __int64 lJettonNum, bool bXuYa);
	//申请消息
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//比大小
	LRESULT OnBigSmall(WPARAM wParam, LPARAM lParam);
	//续押
	LRESULT OnXuYa(WPARAM wParam, LPARAM lParam);

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};

extern CGameClientDlg*	g_pGameClientDlg;

//////////////////////////////////////////////////////////////////////////
