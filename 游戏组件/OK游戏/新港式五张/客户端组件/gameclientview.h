#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "HistoryRecord.h"
#include "GameLogic.h"
#include "PlayOperate.h"
#include "MoveDraw.h"
#include "GameLogic.h"
#include "AdminControlDlg.h"
//////////////////////////////////////////////////////////////////////////
//宏定义

//视图位置
#define MYSELF_VIEW_ID				2									//视图位置

//动作标识
#define AC_NO_ADD					0									//不加动作
#define AC_FOLLOW					1									//跟注动作
#define AC_ADD_SCORE				2									//加注动作
#define AC_SHOW_HAND				3									//梭哈动作
#define AC_GIVE_UP					4									//放弃动作

//控制消息
#define IDM_START					(WM_USER+200)						//开始消息


//通知消息
#define IDM_SEND_CARD_FINISH		(WM_USER+300)						//发牌完成

//////////////////////////////////////////////////////////////////////////
//结构定义

//发牌子项
struct tagSendCardItem
{
	WORD							wChairID;							//发牌用户
	BYTE							cbCardData;							//发牌数据
};

//数组说明
typedef CWHArray<tagSendCardItem,tagSendCardItem &>	CSendCardItemArray;
typedef CWHArray<stuMoveChip*>						CChipArray;


//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//状态变量
protected:
	SCORE							m_lCellScore;						//游戏底注
	SCORE							m_lDrawMaxScore;					//最大下
	SCORE							m_lUserScore[GAME_PLAYER];			//用户积分
	SCORE							m_lUserAddScore[GAME_PLAYER];		//用户结束增加值
	SCORE							m_lUserAddAllScore[GAME_PLAYER];	//用户结束增加值
	SCORE							m_lUserSaveScore[GAME_PLAYER];		//用户结束积分
	SCORE							m_lTableScore[GAME_PLAYER];			//下注数目
	SCORE							m_lOverScore[GAME_PLAYER];			//结束结算
	SCORE							m_lOverSaveScore[GAME_PLAYER];		//结束保存结算

	//发牌动画变量
protected:
	CPoint							m_SendCardPos;						//发牌位置
	CPoint							m_SendCardCurPos;					//发牌当前位置
	INT								m_nStepCount;						//步数
	INT								m_nXStep;							//步长
	INT								m_nYStep;							//步长
	CSendCardItemArray				m_SendCardItemArray;				//发牌数组

	BYTE							m_cbUserCard[GAME_PLAYER][MAX_COUNT];
	BYTE							m_cbUserCardCount[GAME_PLAYER];

	//动作变量
protected:
	WORD							m_wActionUser;						//动作用户
	BYTE							m_cbUserAction;						//动作标识

	//控制按钮
public:
	CSkinButton						m_btStart;							//开始按钮
	CPlayOperate					m_PlayOperate;						//玩家操作

	//游戏控件
public:
	CGameLogic						m_GameLogic;
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件

//控制变量
public:
	CAdminControlDlg				m_AdminDlg;							//管理员控制
	CButton							m_btOpenAdmin;
	//扑克筹码
public:
	CCardControl					m_CardControl[GAME_PLAYER];			//扑克控件
	SCORE							m_lPalyBetChip[GAME_PLAYER];		//玩家下注
	SCORE							m_lALLBetChip;						//桌面下注
	CChipArray						m_ArrayPlayChip;					//玩家下注数组
	CChipArray						m_ArrayBetChip;						//下注数组
	CChipArray						m_ArrayOverChip;					//回收数组

	CRect							m_rectBetChip;						//下注区域
	CSize							m_sizeWin;							//窗口大小


	//位置变量
protected:

	CPoint							m_ptUserAction[GAME_PLAYER];		//动作位置
	CPoint							m_ptPalyBetChip[GAME_PLAYER];		//玩家下注
	CPoint							m_ptPalyPos[GAME_PLAYER];			//玩家下注
	CPoint							m_ptOverScore[GAME_PLAYER];			//结束积分

	CPoint							m_ptUserInfo[GAME_PLAYER];			//玩家信息
	CPoint							m_ptLevel[GAME_PLAYER];
	CPoint							m_ptScore[GAME_PLAYER];

	//资源大小
protected:
	CSize							m_SizeHistory;						//积分大小
	CSize							m_SizeGameCard;						//扑克大小

	//资源变量
protected:
	CPngImage						m_ImageReady;						// 准备

	CPngImage						m_ImageGameCard;					// 扑克资源
	CBitImage						m_ImageViewFill;					// 背景资源
	CPngImage						m_ImageViewBack;					// 背景资源

	CPngImage						m_ImageActionBack;					// 操作背景
	CPngImage						m_ImageActionFont;					// 操作
	CPngImage						m_ImageShowHand;					// 梭哈

	CPngImage						m_ImageChip;						// 筹码位图

	CPngImage						m_ImageCellScore;					// 低分背景
	CPngImage						m_ImageTipInfo;						// 小贴士

	CPngImage						m_ImageCellNumber;					// 低分数字
	CPngImage						m_ImgaeScoreTotalNumber;			// 总注数字

	CPngImage						m_ImgaeScoreBackR;					// 结束背景
	CPngImage						m_ImgaeScoreBackL;					// 结束背景
	CPngImage						m_ImgaeScoreBackM;					// 结束背景

	//CPngImage						m_ImgaeScoreSum;					// 结束符号
	CPngImage						m_ImgaeScoreWin;					// 结束分数
	CPngImage						m_ImgaeScoreLose;					// 结束分数
	CPngImage						m_ImageCradType;					// 牌型

	CPngImage						m_ImageUserInfo;
	CPngImage						m_ImageClock;

	//字体变量
	CDFontEx						m_Font;

	//12-7-31
	CPngImage						m_PngOffLine;
	CPoint							m_ptOffLine[GAME_PLAYER];

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//重载函数
public:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//控制接口
public:
	//重置界面
	virtual VOID ResetGameView();

	//继承函数
protected:
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7支持
	virtual bool RealizeWIN7() { return true; }

	//发牌函数
public:
	//是否发牌
	bool IsDispatchStatus();
	//停止发牌
	VOID ConcludeDispatch();
	//发送扑克
	VOID DispatchUserCard(WORD wChairID, BYTE cbCardData);

	VOID SetUserCard(WORD wViewID,BYTE cbCardData[MAX_COUNT],BYTE cbCount);
	//功能函数
public:
	//游戏底注
	bool SetCellScore(SCORE lCellScore);
	//最大下注
	bool SetDrawMaxScore(SCORE lDrawMaxScore);
	//设置积分
	bool SetUserScore(WORD wChairID, SCORE lUserScore);
	//设置积分
	bool SetUserSaveScore(WORD wChairID, SCORE lUserScore);
	//设置结束积分
	bool SetOverScore(WORD wChairID, SCORE lUserScore);
	//设置动作
	bool SetUserAction(WORD wActionUser, BYTE cbUserAction);
	//设置下注
	void SetUserTableScore(WORD wChairID, SCORE lTableScore);
	// 添加桌面下注
	void AddDeskChip( SCORE lScore, WORD wChairID );
	// 添加玩家下注
	void AddPlayChip( SCORE lScore, WORD wChairID );
	// 添加玩家加注
	void AddPlayBetChip( SCORE lScore, WORD wChairID );
	// 筹码回收
	void ChioRecovery( WORD wChairID );

	
	//内部函数
private:
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	// 绘画字符
	bool DrawTextStringEx(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, INT nDrawFormat);
	// 绘画结束分数
	void DrawOverScore(CDC * pDC, SCORE lOutNum, INT nXPos, INT nYPos );
	// 添加逗号
	CString AddComma( LONGLONG lScore );
	//时间绘制
	void DrawUserClockEx(CDC *pDC,INT nXPos, INT nYPos,WORD wChairID);
	//按钮消息
public:
	//开始按钮
	VOID OnBnClickedStart();
	//管理员控制
	VOID OpenAdminWnd();

	//消息函数
public:
	//时间消息
	VOID OnTimer(UINT nIDEvent);
	//创建函数
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	// 消息函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//按键消息
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif