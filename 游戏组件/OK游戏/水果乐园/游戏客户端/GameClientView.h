#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "RecordGameList.h"
#include "ExchangeDlg.h"
#include "PlayerList.h"
#include "AdminClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//筹码定义
#define JETTON_COUNT				9									//筹码数目

//消息定义
#define IDM_APPLY_BANKER			(WM_USER+201)						//申请信息
#define IDM_BIG_SMALL				(WM_USER+202)						//比大小
#define IDM_XU_YA					(WM_USER+203)						//续押

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//////////////////////////////////////////////////////////////////////////
//类声明
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

class CMySkinButton : public CSkinButton
{
public:
	int m_nTime;//鼠标按下时，持续的时间
	CMySkinButton();
	~CMySkinButton();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//限制信息
protected:
	__int64							m_lMeMaxChip;							//最大下注
	__int64							m_lMeAreaChip[JETTON_AREA_COUNT];		//玩家下注					
	__int64							m_lAllAreaChip[JETTON_AREA_COUNT];		//全体下注
	__int64							m_lAreaChipLimit[JETTON_AREA_COUNT];	//各区域当前可下分
	CRect							m_rcJettonArea[JETTON_AREA_COUNT];		//各下注区域
	CRect							m_rcChipLimit[JETTON_AREA_COUNT];		//各区域最大可下分书写点
	CRect							m_rcApplayBankerNo[4];					//申请上庄玩家列表
	CRect							m_rcApplayBankerName[4];				//申请上庄玩家名字
	CRect							m_rcApplayBankerChip[4];				//申请上庄玩家金币
	CRect							m_rcHistoryRecord[MAX_SHOW_HISTORY];
	int								m_nCurCartoonIndex;						//当前应该绘制哪个卡通图案
	int								m_CartoonTimeElapseArray[130];//96		//控制卡通旋转的时间间隔
	int								m_CartoonElapsIndex;					//卡通定时器数组下标
	BYTE							m_cbWinArea;							//每次旋转停止的位置
	BYTE							m_cbMyBigSmallResult[JETTON_AREA_COUNT];

	//位置信息
public:
	int								m_nChipHead;							//成绩位置
	bool							m_bisLButtonDown;						//是否鼠标左键按下
	BYTE							m_cbEndCount;							//游戏结束记数

	//动画变量
protected:
	bool							m_bBombEffect[JETTON_AREA_COUNT];		//爆炸效果
	BYTE							m_cbBombFrameIndex[JETTON_AREA_COUNT];	//帧数索引

	//历史信息
protected:
	__int64							m_lMeStatisticChip;					//游戏成绩
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//游戏记录
	int								m_nRecordFirst;							//开始记录
	int								m_nRecordLast;							//最后记录

	//状态变量
protected:
	WORD							m_wMeChairID;						//我的位置
	BYTE							m_cbWinnerSide;						//胜利玩家
	BYTE							m_cbAreaFlash;						//胜利玩家
	__int64							m_lCurrentJetton;					//当前筹码
	CString							m_strDispatchCardTips;				//发牌提示
	bool							m_bShowChangeBanker;				//轮换庄家
	bool							m_bNeedSetGameRecord;				//完成设置

	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	__int64							m_lBankerChip;						//庄家积分
	__int64							m_lBankerWinChip;					//庄家成绩	
	bool							m_bEnableSysBanker;					//系统做庄
	CFont							m_fontChipLimit;					//专门绘制可下分的画笔

	//当局成绩
public:
	__int64							m_lMeCurGameChip;					//我的成绩
	__int64							m_lMeCurGameReturnChip;			//我的成绩(返还下注分)
	__int64							m_lBankerCurGameChip;				//庄家成绩
	__int64							m_lGameRevenue;						//游戏税收
	BYTE							m_cbGameEndTime;					//游戏结束阶段显示的时间，由Dlg来定义

	bool							m_bBigSmallTime;					//比大小时间
	bool							m_bAlreadyGuess;					//已经猜过大小了
	BYTE							m_cbPaoHuoCheCount;
	BYTE							m_cbPaoHuoCheIndex;
	BYTE							m_cbPaoHuoCheArea[7];
	BYTE							m_cbAreaLight[24];					//绘制区域边框
	bool							m_bBigSmallWin;
	BYTE							m_cbGoodLuckType;
	bool							m_bShowSGZZLogo;
	bool							m_bShowPlayerList;					//显示玩家列表，还是显示上庄列表

	//数据变量
protected:
	CPoint							m_PointJetton[JETTON_AREA_COUNT];					//筹码位置
	CPoint							m_PointJettonNumber[JETTON_AREA_COUNT];				//数字位置
	CPoint							m_PtCardArray[24];					//旋转路径上各区域的中心点
	bool							m_bGoodLuckTypeTip;					//开中GoodLuck
	bool							m_bGoodLuckFlash;
	BYTE							m_cbFlashAndHitCount;
	BYTE							m_cbGoodLuckFlashCount;
	BYTE							m_cbGoodLuckTipCount;

	BYTE							m_cbPaoHuoCheWinIndex;			//开中“跑火车”时的起始位置
	BYTE							m_cbPaoHuoCheWinCount;			//开中“跑火车”时的滚动次数

	BYTE							m_cbActiveBorderFrameCount;
	CRect							m_rectActiveBorderUpdate;

	//控件变量
public:
	CSkinButton						m_btApplyBanker;					//申请庄家
	CSkinButton						m_btCancelBanker;					//取消庄家

	CSkinButton						m_btChipMoveL;						//移动成绩
	CSkinButton						m_btChipMoveR;						//移动成绩
	CSkinButton						m_btBankButton;						//银行按钮

	CSkinButton						m_btAddStep1;						//1倍投注
	CSkinButton						m_btAddStep10;						//10倍投注
	CSkinButton						m_btAddStep100;						//100倍投注
	CSkinButton						m_btAddStep1_H;						//1倍投注
	CSkinButton						m_btAddStep10_H;					//10倍投注
	CSkinButton						m_btAddStep100_H;					//100倍投注

	CMySkinButton					m_btBetLast;						//续押按钮
	CSkinButton						m_btBetSmall;						//押小按钮
	CSkinButton						m_btBetBig;							//押大按钮

	CSkinButton						m_btPlayerList;
	CSkinButton						m_btApplyList;
	CSkinButton						m_btPlayerList_H;
	CSkinButton						m_btApplyList_H;
	CSkinButton						m_btBetLast2;

	//控件变量
public:
	CGameRecord						m_GameRecord;						//记录列表	
	CExchangeDlg					m_ExchangeDlg;
	CPlayerList						m_PlayerList;
	CPlayerList						m_ApplyList;
	CButton							m_btOpenAdmin;						//特殊账号控制
	CAdminClientDlg					m_AdminDlg;							//特殊账号控制
	CGameClientDlg					*m_pGameClientDlg;					//

	//界面变量
protected:
	CBitImage						m_ImageViewFill;					//背景位图
	CBitImage						m_ImageViewBack;					//背景位图
	CBitImage						m_ImageJettonView;					//筹码视图
	CBitImage						m_ImageChipNumber;					//数字视图
	CBitImage						m_ImageHistoryRecord;				//历史记录图片
	CBitImage						m_ImageGoodLuckFlash[2];
	CBitImage						m_ImageActiveButton1;
	CBitImage						m_ImageActiveButton10;
	CBitImage						m_ImageActiveButton100;
	CBitImage						m_ImageBigSmallNumber;

	CPngImage						m_pngActiveBorder1;
	CPngImage						m_pngActiveBorder10;
	CPngImage						m_pngActiveBorder100;
	CPngImage						m_ImageCardCartoon;					//旋转动画
	CPngImage						m_ImageCardCartoon2;
	CPngImage						m_pngRecordTip;

	CPngImage						m_ImageApple;						//苹果动画
	CPngImage						m_ImageApple2;						//苹果X2动画
	CPngImage						m_ImageBar;							//BAR动画
	CPngImage						m_ImageBar2;						//BARX2动画
	CPngImage						m_ImageBell;						//铃铛动画
	CPngImage						m_ImageBell2;						//铃铛X2动画
	CPngImage						m_ImageSeven;						//SEVEN
	CPngImage						m_ImageSeven2;						//SEVENX2
	CPngImage						m_ImageStar;						//STAR
	CPngImage						m_ImageStar2;						//STARX2
	CPngImage						m_ImageOrange;						//ORANGE
	CPngImage						m_ImageOrange2;						//ORANGEX2
	CPngImage						m_ImagePaw;							//PAW
	CPngImage						m_ImagePaw2;						//PAWX2
	CPngImage						m_ImageWaterMelon;					//WATERMELON
	CPngImage						m_ImageWaterMelon2;					//WATERMELONX2
	CPngImage						m_pngGoodLuckTip[5];				//开中GoodLuck时，显示的图片
	CPngImage						m_pngSongJiang;

	CPngImage						m_ImageBetNumber;					//数字视图
	CPngImage						m_ImageBetBigNumber;				//时间视图

	CPngImage						m_ImageFreeTime;					//空闲时间
	CPngImage						m_ImageBitSmallTime;				//押大小时间
	CPngImage						m_ImageJettonTime;					//下注时间

	CPngImage						m_pngJettonBack;
	CPngImage						m_pngJettonButton;
	CPngImage						m_pngMove;

	CPngImage						m_pngLine;
	CPngImage						m_pngBigSmall[2];
	CPngImage						m_pngSGZZLogo;

	HCURSOR							m_MyCursor[3];

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//继承函数
private:
	//重置界面
	virtual void ResetGameView();
	//调整控件
	virtual void RectifyControl(int nWidth, int nHeight);
	//绘画界面
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//设置函数
public:
	//设置信息
	void SetMeMaxChip(__int64 iMeMaxChip);
	//我的位置
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID;}
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount);
	//庄家信息
	void SetBankerInfo(WORD wBankerUser, __int64 lBankerChip);
	//庄家成绩
	void SetBankerChip(WORD wBankerTime, __int64 lWinChip) {m_wBankerTime=wBankerTime; m_lBankerWinChip=lWinChip;}
	//当局成绩
	void SetCurGameChip(__int64 lMeCurGameChip, __int64 lMeCurGameReturnChip, __int64 lBankerCurGameChip, __int64 lGameRevenue);
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, __int64 lChipCount);
	//区域限制
	void SetAreaLimitChip(__int64 * pLimitArray, int ArraySize);	
	//设置筹码
	//void SetCurrentJetton(__int64 iCurrentJetton);
	//历史记录
	void SetGameHistory(tagGameRecord * pGameRecord, int nRecordCount);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//成绩设置
	void SetGameChip(__int64 lMeCurGameChip, __int64 lMeCurGameReturnChip, __int64 lBankerCurGameChip);
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	void CreateApplayBankerListRect(int nBaseX, int nBaseY);
	void CreateHistoryRecordRect(int nWidth, int nHeight);
	void UpdateCartoonTimeElapse(BYTE cbStopStep);
	void UpdateCartoonTimeElapse2(BYTE cbWinArea);
	void UpdateCartoonTimeElapse3(BYTE cbWinArea);
	void CreateTurnPathAreaBasePoint(int nWidth, int nHeight);
	//开中GoodLuck时播放的动画
	void SetGoodLuckCartoon(BYTE cbPaoHuoCheArea[], BYTE cbPaoHuoCheCount, BYTE cbWinArea, BYTE cbGoodLuckType);

	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nFlags);

	//动画函数
public:	
	//设置爆炸
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//绘画爆炸
	void DrawBombEffect(CDC *pDC);
	//绘制申请上庄玩家列表
	void DrawApplyBankerUserList(CDC * pDC);
	//绘制历史记录
	void DrawHistoryRecord(CDC * pDC);
	//绘制时间提示
	void DrawTimeTip(CDC * pDC, int nWidth, int nHeight);
	//绘制区域限注
	void DrawAreaLimitChip(CDC * pDC);
	//绘制筹码
	//void DrawAreaJetton(CDC * pDC);
	//绘制庄家信息
	void DrawBankerInfo(CDC * pDC, int nWidth, int nHeight);
	//绘制闲家信息
	void DrawUserInfo(CDC * pDC, int nWidth, int nHeight);
	//绘制GoodLuck闪烁
	void DrawGoodLuckFlash(CDC * pDC, int nWidth, int nHeight);
	//切换庄家
	void DrawChangeBanker(CDC * pDC, int nWidth, int nHeight);
	//绘制下注按钮
	void DrawAddMoney(CDC * pDC, int nWidth, int nHeight);
	//绘制时间
	virtual void DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea);

	//计算限制
protected:

	//获取当前玩家还能下注的数额
	__int64 GetUserCurrentLeaveChip();
	
	//绘画函数
public:
	//闪烁边框
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);
	//开始旋转
	void PlayingCartoonEffect();
	void PlayingCartoonEffect2();
	void PlayingCartoonEffect3();
	//结束旋转
	void EndCartoonEffect();


	//界面函数
public:
	//清理筹码
	void CleanUserJetton();
	//设置胜方
	void SetWinnerSide(BYTE cbWinnerSide);
	//绘画数字
	void DrawMyJettonNumber(CDC *pDC);
	//大小历史记录
	void DrawMyBigSmallNumber(CDC *pDC);
	//新增大小历史记录
	void InsertBigSmallRecord(BYTE cbBigSmall);

	//内联函数
public:
	//当前筹码
	inline __int64 GetCurrentJetton() { return m_lCurrentJetton; }	

	//内部函数
private:
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//绘画数字
	void DrawNumberString(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos, bool bYellow=false);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, CRect rcRect, INT nFormat=-1);
	//显示结果
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//绘制我的分数
	void DrawMyChip(CDC *pDC, int nWidth, int nHeight);
	//绘制当前成绩
	void DrawCurrentChip(CDC *pDC, int nWidth, int nHeight);
	//透明绘画
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//绘制动画
	void DrawCardCartoon(CDC * pDC);
	//绘制区域边框
	void DrawAreaLight(CDC * pDC);
	//绘制结束动画
	void DrawEndCartoon(CDC * pDC,int nWidth,int nHeight,BYTE cbWinArea);
	//绘制结束动画
	void DrawGoodLuckTip(CDC * pDC,int nWidth,int nHeight,BYTE cbWinArea);

	//按钮消息
protected:
	//倍投按钮
	afx_msg void OnAddStep1();
	afx_msg void OnAddStep10();
	afx_msg void OnAddStep100();
	afx_msg void OnAddStep1_H();
	afx_msg void OnAddStep10_H();
	afx_msg void OnAddStep100_H();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//移动按钮
	afx_msg void OnChipMoveL();
	//移动按钮
	afx_msg void OnChipMoveR();

	afx_msg void OnClickedBank();

	afx_msg void OnBetSmall();
	afx_msg void OnBetBig();
	afx_msg void OnXuYa();
	afx_msg void OnXuYa2();
	afx_msg void OnPlayerList();
	afx_msg void OnApplyList();
	//特殊账号控制
	afx_msg void OpenAdminWnd();

	//消息映射
protected:
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
