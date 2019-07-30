#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ApplyUserList.h"
#ifdef  __BANKER___
#include "DlgBankEx.h"
#endif
#include "..\游戏服务器\GameLogic.h"
#include "ClientControl.h"
#include "RgnButton.h"
//////////////////////////////////////////////////////////////////////////

//筹码定义
#define JETTON_COUNT				6									//筹码数目
#define JETTON_RADII				68									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息
#define IDM_CONTINUE_CARD			WM_USER+202							//继续发牌
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//按钮标识
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_SHOW_RESULT				WM_USER+205		
#define IDM_SOUND					WM_USER+206		
#define IDM_CHANGE_MUSIC			WM_USER+207							//切换音乐


//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//////////////////////////////////////////////////////////////////////////
//结构定义

//历史记录
#define MAX_SCORE_HISTORY			65									//历史个数
#define MAX_FALG_COUNT				9									//标识个数
#define COUNT_DRAWINFO              18

//筹码信息
struct tagJettonInfo
{
	int								nXPos;								//筹码位置
	int								nYPos;								//筹码位置
	BYTE							cbJettonIndex;						//筹码索引
};

//操作结果
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//记录信息
struct tagClientGameRecord
{
	enOperateResult					enOperateMen[AREA_COUNT+1];					//操作标识
	bool							bWinMen[AREA_COUNT+1];						//顺门胜利

};

//发牌提示
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//继续发牌
	enDispatchCardTip_Dispatch											//重新洗牌
};

struct  sT_ShowInfo
{
	CRect RcRect[AREA_COUNT+1];
	CPoint point[AREA_COUNT+1];
	bool  blShow[AREA_COUNT+1];
};
struct  sT_ShowJetton
{
	bool  blShow[6];
	sT_ShowJetton()
	{
		for (int i = 0;i<6;i++){
			blShow[i]=true;
		}
	}
};
//筹码数组
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//累声明
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

struct tagAndroidBet
{
	BYTE							cbJettonArea;						//筹码区域
	SCORE						lJettonScore;						//加注数目
	WORD							wChairID;							//玩家位置
	int								nLeftTime;							//剩余时间 (100ms为单位)
};

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//限制信息
protected:
	LONGLONG						m_lMeMaxScore;						//最大下注
    LONGLONG						m_lAreaLimitScore;					//区域限制
	LONGLONG						m_lRobotMaxJetton;					//机器人最大筹码

	//下注信息
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//个人总注
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//全体总注
	LONGLONG						m_JettonQue[COUNT_DRAWINFO];
	BYTE							m_JettonQueArea[COUNT_DRAWINFO];
	BYTE							m_JettonQueIndex;

	//位置信息
protected:
	int								m_nWinFlagsExcursionX;				//偏移位置
	int								m_nWinFlagsExcursionY;				//偏移位置
	int								m_nScoreHead;						//成绩位置
	CRect							m_rcTianMen;						//闲家区域
	CRect							m_rcDimen;							//闲天王区域
	CRect							m_rcQiao;							//对子区域
	CRect							m_rcHuangMen;							//平区域
	CRect							m_rcXuanMen;							//同点平区域
	CRect							m_rcHuang;							//同点平区域
	CRect							m_rcJiaoR;							//庄家区域
	int								m_OpenCardIndex;					//開牌順序
	int								m_PostCardIndex;					//發牌順序
	int								m_PostStartIndex;					//发牌起始位置
	CPoint							m_CardTypePoint[AREA_COUNT+1];
   

	//扑克信息
public:	
    BYTE							m_cbTableCardArray[1][1];			//桌面扑克
	BYTE							m_cbTableSortCardArray[1][1];			//桌面扑克
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//手自动
	SCORE						m_lUserCardType[AREA_COUNT+1];	//个人总注
	CRect							m_JettonAreaRect[AREA_COUNT];
	CRect m_cTmpRect[64];
	sT_ShowJetton                   m_T_ShowJetton;

	//历史信息
protected:
	SCORE						m_lMeStatisticScore;				//游戏成绩
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//游戏记录
	int								m_nRecordFirst;						//开始记录
	int								m_nRecordLast;						//最后记录
	int                             m_szpm;                             //上庄排名
	float                             dbsj,dbsjdt;                               //开奖记录数据
	float                             xbsj,xbsjdt;
	float                             xbm,xbmdt;
	float                             dbm,dbmdt;
	float                             xbc,xbcdt;
	float                             dbc,dbcdt;
	float                             xdz,xdzdt;
	float                             ddz,ddzdt;
	//状态变量
protected:
	WORD							m_wMeChairID;						//我的位置
	BYTE							m_cbAreaFlash;						//胜利玩家
	LONGLONG						m_lCurrentJetton;					//当前筹码
	bool							m_bShowChangeBanker;				//轮换庄家
	bool							m_bNeedSetGameRecord;				//完成设置
	bool							m_bWinTianMen;						//胜利标识
	bool							m_bWinHuangMen;						//胜利标识
	bool                            m_bWinDiMen;
	bool							m_bWinXuanMen;						//胜利标识
	bool							m_bFlashResult;						//显示结果
	bool							m_bShowGameResult;					//显示结果
	enDispatchCardTip				m_enDispatchCardTip;				//发牌提示
	CPngImage						m_ViewBackPng;
	CPngImage                       m_ImageZuoshang;
	CPngImage                      m_ImageYoushang;
	CPngImage                       m_ImageZuoxia;
	CPngImage                       m_ImageYouxia;
	

	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	SCORE						m_lBankerScore;						//庄家积分
	SCORE						m_lBankerWinScore;					//庄家成绩	
	SCORE						m_lTmpBankerWinScore;				//庄家成绩	
	bool							m_bEnableSysBanker;					//系统做庄

	//当局成绩
public:
	SCORE						m_lMeCurGameScore;					//我的成绩
	SCORE						m_lMeCurGameReturnScore;			//我的成绩
	SCORE						m_lBankerCurGameScore;				//庄家成绩
	SCORE						m_lGameRevenue;						//游戏税收

	//数据变量
protected:
	CPoint							m_PointJetton[AREA_COUNT];			//筹码位置
	CPoint							m_PointJettonNumber[AREA_COUNT];	//数字位置
	CJettonInfoArray				m_JettonInfoArray[AREA_COUNT];		//筹码数组
	bool							m_bWinFlag[AREA_COUNT];						//胜利标识
	int								m_TopHeight;
	int								m_LifeWidth;
	int								m_CurArea;
	int                             m_nApplyPageCount;                  //申请列表页数
	int iP;     //角度
    int iP1;
	//下注区域 2012-9-12
	CRect							m_RectArea[8];
	CPngImage						m_ImageCh[8];                   
	//机器人下注
	CWHArray<tagAndroidBet>	m_ArrayAndroid;

	//控件变量
public:

	
	CRgnButton						m_btJetton100;						//筹码按钮
	CRgnButton						m_btJetton1000;						//筹码按钮
	CRgnButton						m_btJetton10000;					//筹码按钮
	//CRgnButton						m_btJetton50000;					//筹码按钮
	CRgnButton						m_btJetton100000;					//筹码按钮
	CRgnButton						m_btJetton1000000;					//筹码按钮
	CRgnButton						m_btJetton5000000;					//筹码按钮
	CSkinButton						m_btApplyBanker;					//申请庄家
	CSkinButton						m_btCancelBanker;					//取消庄家
	CSkinButton						m_btScoreMoveL;						//移动成绩
	CSkinButton						m_btScoreMoveR;						//移动成绩

// 	CSkinButton						m_btBankStorage;					//存款按钮
// 	CSkinButton						m_btBankDraw;						//取款按钮
	CSkinButton						m_btUp;								//存款按钮
	CSkinButton						m_btDown;							//取款按钮
	CButton							m_btOpenAdmin;						//系统控制
	CSkinButton						m_btLast;								//存款按钮
	CSkinButton						m_btNext;							//取款按钮

	int								m_ShowImageIndex;
	int								m_CheckImagIndex;
	//控件变量
public:
//	CApplyUser						m_ApplyUser;						//申请列表
	CWHArray<tagApplyUser*>          m_ApplyUser;
	
	CGameClientDlg					*m_pGameClientDlg;					//父类指针
	CGameLogic						m_GameLogic;						//游戏逻辑
#ifdef  __BANKER___
	CDlgBank						m_DlgBank;							//银行控件
#endif
	CRect							m_MeInfoRect;						//
	int								m_Out_Bao_y;
	bool						    m_bShowBao;
	CRect							m_CarRect[36];	
	int								m_carX;
	int								m_carY;
	int								m_carX1;
	int								m_carY1;
	int                             m_carX2;
	int                             m_carY2;
	int								m_carX3;
	int								m_carY3;
	CPngImage						m_idb_selPng;
	int								m_CarIndex ;
	int  m_CarIndexod;
	int                             m_js;
	//控制
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;	

	//银行
public:
	bool							m_blCanStore;                       //是否能保存
	bool                               m_winflag;							//绘画中奖汽车标志
	int  qisx; 
	int  chsxx;
	int  chsxy;

	void                            InsertApplyUser(tagApplyUser* pApplyUser);
	void                            UpdateApplyUser(tagApplyUser* pApplyUser);
	void                            RemoveApplyUser(tagApplyUser* pApplyUser);
	//界面变量
protected:
	CBitImage						m_ImageViewFill;					//背景位图
	//CBitImage						m_ImageViewcentre;					//背景位图
	CPngImage						m_ImageWinFlags;					//标志位图
	CPngImage						m_ImageJettonView;					//筹码视图
	CPngImage                       m_ImageXdz;                         //小大众
	CPngImage                       m_ImageDbc;                         //大奔驰
	CPngImage                       m_ImageDbm;                         //大宝马
	CPngImage                       m_ImageDbsj;                        //大保时捷
	CPngImage                       m_ImageDdz;                         //大大众
	CPngImage                       m_ImageXbc;                         //小奔驰
	CPngImage                       m_ImageXbm;                         //小宝马
	CPngImage                       m_ImageXbsj;                        //小保时捷
	CBitImage						m_ImageScoreNumber;					//数字视图
	CBitImage						m_ImageMeScoreNumber;				//数字视图

	CPngImage                       m_ImageCar;                         //小汽车
	CPngImage                       m_ImageChdzqz;                     //大众旗子
	CPngImage                       m_ImageChbcqz;                     //奔驰旗子
	CPngImage                       m_ImageChbmqz;                     //宝马旗子
	CPngImage                       m_ImageChbsjqz;                     //大众旗子
	CPngImage                       m_ImageChdbcdh;                     //中大奔驰
	CPngImage                       m_ImageChddzdh;                     //中大大众
	CPngImage                       m_ImageChdbmdh;                     //中大宝马
	CPngImage                       m_ImageChdbsdh;                     //中大保时捷
	CPngImage                       m_ImageChxbci;                     //中小奔驰
	CPngImage                       m_ImageChxdzo;                     //中小大众
	CPngImage                       m_ImageChxbma;                     //中小宝马
	CPngImage                       m_ImageChxbsj;                     //中小保时捷
	CPngImage                        m_Imagehkjbj;                     //中奖背景
	
	CPngImage                       m_ImageRedcar;                    //红色汽车
	CPngImage						m_ImageTimeFlagPng;					//时间标识
	CBitImage						m_ImageDispatchCardTip;				//发牌提示
	CBitImage						m_ImageCardType[AREA_COUNT+1];		//数字视图
	//CBitImage						m_ImageBao;							//发牌提示
	CRect							m_BaoPosion;

	//边框资源
protected:

	CBitImage						m_ImageFrameTianMen;				//边框图片
	CBitImage						m_ImageFrameDiMen;					//边框图片
	CBitImage						m_ImageFrameHuangMen;				//边框图片
	CBitImage						m_ImageFrameXuanMen;				//边框图片
	CBitImage						m_ImageFrameQiao;					//边框图片
	CBitImage						m_ImageFrameJiaoR;					//边框图片
	CBitImage						m_ImageMeBanker;					//切换庄家
	CBitImage						m_ImageChangeBanker;				//切换庄家
	CBitImage						m_ImageNoBanker;					//切换庄家
	bool							m_blMoveShowInfo;
	Image							*m_pImage;
	Image							*m_pImage1;

protected:

	CPngImage                          m_PngPushBox[7];
	CPngImage                          m_PngShowJetton[4];
	bool							   m_bShowJettonAn;
	int                                m_bShowJettonIndex;
	CPngImage                          m_PngShowLeaveHandle[4];
	bool							   m_bShowLeaveHandleAn;
	int                                m_bShowLeaveHandleIndex;
	bool							   m_bOPenBoxAn;
	int                                m_bOPenBoxIndex;
	bool                               m_blShowLastResult;

	CPngImage                          m_PngResult[4];
	CPngImage                          m_TimerCount_png;
	CPngImage						   m_ImageMeScoreNumberPng;				//数字视图
	CPngImage						   m_ImageScoreNumberPng;

	bool							   m_bShowResult;
	bool                               m_blShowResultIndex;
	SCORE						   m_lLastJetton;


	//结束资源
protected:
	CPngImage						m_pngGameEnd;
	bool							m_DrawBack;
	bool							m_bEnablePlaceJetton;

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//继承函数
private:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	virtual bool RealizeWIN7() { return true; }

	//设置函数
public:
	//取款操作
	void OnBankDraw();
	//存款操作
	void OnBankStorage();
	//设置信息
	void SetMeMaxScore(SCORE lMeMaxScore);
	//我的位置
	void SetMeChairID(DWORD dwMeUserID);
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount);
	//庄家信息
	void SetBankerInfo(DWORD dwBankerUserID, SCORE lBankerScore);
	//庄家成绩
	void SetBankerScore(WORD wBankerTime, SCORE lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//当局成绩
	void SetCurGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore, SCORE lGameRevenue);
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, SCORE lScoreCount);
	//机器人下注
	void AndroidBet(BYTE cbViewIndex, SCORE lScoreCount);
	//区域限制
	void SetAreaLimitScore(SCORE lAreaLimitScore);	
	//设置扑克
	void SetCardInfo(BYTE cbTableCardArray[1][1]);
	//设置筹码
	void SetCurrentJetton(SCORE lCurrentJetton);
	//历史记录
	void SetGameHistory(BYTE *bcResulte);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//成绩设置
	void SetGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore);
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//设置提示
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);
	//执行剩余所有的缓冲动画
	void PerformAllBetAnimation();
	//设置机器人最大筹码
	void SetRobotMaxJetton(SCORE lRobotMaxJetton){ m_lRobotMaxJetton = lRobotMaxJetton; };

	void SetFirstShowCard(BYTE bcCard);
	//取消闪动
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }

	void SetEnablePlaceJetton(bool bEnablePlaceJetton);

	void UpdataJettonButton();

	void StartRunCar(int iTimerStep);

	void RuningCar(int iTimerStep);
	void RuningRedCar();

	void FlashWinOpenSide();

	void StartRandShowSide();

	CString NumberStringWithSpace(LONGLONG lNumber);

	int ifirstTimer;

	bool m_bFlashrandShow;

	bool m_blRungingCar;

	bool flagrrc;

	int iTimerStep;
	int iTimerStep1;

	int mX;
	int mX1;
	int mY1;
	int mY2;
	int mX3;
	int mY3;
	int mX2;
	int mxx;
	int tx1;
	int ty1;
	int tx2;
	int ty2;
	int m_x1;
	int m_y1;
	int m_x2;
	int m_y2;
	int  mxx2;
	int sp0;
	int sp1;
	int sp2;
	int sp3;
	int m_CarIndexold;

	int iOpenSide;

	int iRunIndex;

	int iTotoalRun;

	
	
	void UpdateGameView( CRect *pRect );
	//动画函数
public:	
	//开始发牌
	void DispatchCard();
	//结束发牌
	void FinishDispatchCard( bool bRecord = true );
	//设置爆炸
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);

	//计算限制
public:
	//最大下注
	LONGLONG GetUserMaxJetton(BYTE cbJettonArea = 0);

	void  GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);

	//绘画函数
protected:
	//闪烁边框
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//界面函数
public:
	//清理筹码
	void CleanUserJetton();
	//设置胜方
	void SetWinnerSide(bool blWin[],bool bSet);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//绘画数字
	void DrawMeJettonNumber(CDC *pDC);
	//绘画庄家
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//绘画玩家
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);
	//绘画历史成绩数据
	void DrawMemoryDate(CDC *pDC,int nWidth, int nHeight);

	//内联函数
public:
	//当前筹码
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	

	void KillCardTime();

	void SwitchToCheck();

	void SwithToNormalView();

	void SetMoveCardTimer();

	void StartMove();

	void StartJetton_AniMationN();

	void StartOPenBox();

	void StartHandle_Leave();

	void StarShowResult();

	void OnCancelMyApply();

	void MyDrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea);

	//内部函数
private:
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//绘画数字
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool blTimer= false,bool bMeScore = false);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, SCORE lNumber, CRect rcRect, INT nFormat=-1);
	CString DrawNumberStringWithDot(DOUBLE lScore);
	//绘画标识
	void DrawWinFlags(CDC * pDC);
	//显示结果
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//透明绘画
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//推断赢家
	void DeduceWinner(bool bWinMen[]);

	void DrawMoveInfo(CDC* pDC, CRect rcRect);

	void SetJettonHide(int ID);


	//按钮消息
protected:
	bool  m_bApplyBanker;
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//移动按钮
	afx_msg void OnScoreMoveL();
	//移动按钮
	afx_msg void OnScoreMoveR();
	//手工搓牌
	afx_msg void OnOpenCard();
	//自动搓牌
	afx_msg void OnAutoOpenCard();
	afx_msg void OnUp();
	afx_msg void OnLastMusic();
	afx_msg void OnNextMusic();

	afx_msg void OnDown();


	//消息映射
protected:
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//鼠标消息
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
    //鼠标消息
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//控件命令
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    //鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//其他控件鼠标UP消息
	LRESULT OnViLBtUp(WPARAM wParam, LPARAM lParam);
	//管理员控制
	afx_msg void OpenAdminWnd();

	DECLARE_MESSAGE_MAP()	

};

//////////////////////////////////////////////////////////////////////////

#endif
