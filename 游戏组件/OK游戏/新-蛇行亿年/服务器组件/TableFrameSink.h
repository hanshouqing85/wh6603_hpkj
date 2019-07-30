/*****************************************************/
//本游戏开发者:哈皮科技,联系QQ:56248
/*****************************************************/

#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE
#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "HistoryScore.h"
#include "DlgCustomRule.h"
#include "factory.h"

//子弹信息
struct BulletInfo
{
	long							beilv;							        //子弹倍率
	bool 							ishave;						            //子弹存在
	int                             zidanId;
};
//////////////////////////////////////////////////////////////////////////////////

//游戏桌子
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//游戏变量
protected:
	int								m_MuserPower;

	LONG							m_lUserCellScore[GAME_PLAYER];			//单元积分
	LONG							m_lUserAllScore[GAME_PLAYER];			//单元积分
	int                             dintRand;								//击中概率
	int                             dintRand1;								//击中概率
	FishTrace                       m_fishtrace[250][5];//对应鱼儿的坐标路线，5个坐标和角度
	int                             m_bgindex;//背景索引
	int                             m_regfishcount;    //规则鱼个数
	static const int	            m_fishscore[FISHCOUNT]; //鱼分值	
	int                             m_fishShoot[FISHCOUNT][3];//击杀枪数
	int                             m_Percent;
	bool                            m_userisSuperPao[GAME_PLAYER];
	int                             m_TimeToTimeIn;  //时间内进入分
	int                             m_TimeToTimeOut; //时间内出去分
	int                             m_UerResultScore[GAME_PLAYER];//时间内玩家总的分输赢
	int                             m_UserPercent[GAME_PLAYER];
	float                           m_UserP;
	int                             m_UserShootCount[GAME_PLAYER];
	BulletInfo                      m_UserShoot[GAME_PLAYER][200];
	int                             m_UserOutTime[GAME_PLAYER];//玩家发射子弹
	int  							m_dwFishID;								//鱼群标识
	TCHAR							m_szIniFileName[MAX_PATH];				//配置文件
	int  							m_dwZidanID;								//子弹标识
	int                             m_BiliDuihuan;//兑换比例
	int                             m_AndroidPrec;
	int                             m_SendTime[GAME_PLAYER];
	int                             m_KuCun;//全局库存
	int								m_nAllChoushui;							//历史所有抽水
	int                             m_FuckUserID[10];//全局库存
	int                             m_MaxScore;
	int                             m_MinScore;
	int                             m_Cellscore;
	int                             m_GoodLuckFish;
	int                             m_jisuan;
	int                             m_Kucun[10];
	int                             m_CurrCuCun;
	int                             m_EveryUpScore;
	CFactoryTemplate<FishPack>	    m_FishFactory;						//鱼群工厂
	int                             m_BomFishStorg;                     //大鱼库存
	int                             m_ChoushuiScore;
	int                             m_SitTime[GAME_PLAYER];
	int                             m_CanWinScore[GAME_PLAYER];
	int                             m_UserGameTime[GAME_PLAYER];		//玩家进入时间
	int                             m_IsAndroidHaveSit[GAME_PLAYER];
	bool                            m_isChangeScreen;
	bool                            m_ischoujiang[6];
	int                             m_userPower[6];
	int                             m_suodingindex[GAME_PLAYER];					//锁定序号
	int                             m_suodingcount[GAME_PLAYER];					//锁定数
	int                             m_AndroidSuoyuPer;
	int								m_FishCountMax;						//当前桌子最大鱼儿个数
	int								m_BigFishCount;						//大鱼个数


	//炸弹信息
protected:
	BYTE							m_cbBombCount;						//炸弹个数
	BYTE							m_cbEachBombCount[GAME_PLAYER];		//炸弹个数

	//叫分信息
protected:
	BYTE							m_cbBankerScore;					//庄家叫分
	BYTE							m_cbScoreInfo[GAME_PLAYER];			//叫分信息

	//出牌信息
protected:
	WORD							m_wTurnWiner;						//胜利玩家
	BYTE							m_cbTurnCardCount;					//出牌数目


	//扑克信息
protected:
	BYTE							m_cbBankerCard[3];					//游戏底牌
	BYTE							m_cbHandCardCount[GAME_PLAYER];		//扑克数目
	BYTE							m_cbHandCardData[GAME_PLAYER][20];	//手上扑克

	//组件变量
protected:

	CHistoryScore					m_HistoryScore;						//历史成绩

	//组件接口
protected:
	ITableFrame	*					m_pITableFrame;						//框架接口
	tagCustomRule *					m_pGameCustomRule;					//自定规则
	tagGameServiceOption *			m_pGameServiceOption;				//游戏配置
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//游戏属性

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//复位桌子
	virtual VOID RepositionSink();
	//配置桌子
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:
	//时间事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//网络接口
public:
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore);

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//游戏事件
protected:
	//用户放弃
	bool OnUserPassCard(WORD wChairID);
	//用户叫分
	bool OnUserCallScore(WORD wChairID, BYTE cbCallScore);
	//用户出牌
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount);
	//规则鱼形一
	void RegFishone();
	//
	void RegFishtwo();
	//
	void RegFishthree();
	//
	void CreadFish();
	//
	void CreatSmalFish();
	//
	void CreatRegSmalFish();
	//
	int GetNewFishID();
	//
	int GetNewZiDanID();
	//发送消息
	void SendDataExcludeSourceUser(IServerUserItem * pIServerUserItemSource, WORD wSubCmdID, void * pData, WORD wDataSize);
	//
	bool OnSubHitFish(IServerUserItem * pIServerUserItem, DWORD dwFishID, DWORD dwBulletID,int SendUser,bool IsAndroid);
	//

	void ReadConfig();
	//
	//
	double random(double start, double end);
	//
		//IP限制
	bool IsReg();
	//
	bool IsRegTwo();
	// 解密函数
	CString  Decrypt(CString S, WORD Key);
	//获取CPU号
	CString  GetCPUID();
};

//////////////////////////////////////////////////////////////////////////////////

#endif