#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "datastream.h"

//////////////////////////////////////////////////////////////////////////
//枚举定义

//效验类型
enum enEstimatKind
{
	EstimatKind_OutCard,			//出牌效验
	EstimatKind_GangCard,			//杠牌效验
	EstimatKind_PengCard,			//碰牌校验
};


enum enGangState
{
	STATE_NULL,
	STATE_GANG,
	STATE_BU

};

//////////////////////////////////////////////////////////////////////////
//杠牌得分
struct tagGangScore
{
	bool		bMingGang;							//杠个数
	BYTE		cbGangCount;							//杠个数
	int			lScore[MAX_WEAVE][GAME_PLAYER];			//每个杠得分
};
//////////////////////////////////////////////////////////////////////////

struct  HNMJGameRecordPlayer
{
	DWORD dwUserID;
	std::string kHead;
	std::string kNickName;
	std::vector<BYTE> cbCardData;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwUserID);
		Stream_VALUE(kHead);
		Stream_VALUE(kNickName);
		Stream_VECTOR(cbCardData);
	}
};

struct  HNMJGameRecordOperateResult
{
	enum Type
	{
		TYPE_NULL,
		TYPE_OperateResult,
		TYPE_SendCard,
		TYPE_OutCard,
		TYPE_ChiHu,
	};
	HNMJGameRecordOperateResult()
	{
		cbActionType = 0;
		wOperateUser = 0;
		wProvideUser = 0;
		cbOperateCode = 0;
		cbOperateCard = 0;
	}
	BYTE							cbActionType;
	WORD							wOperateUser;						//操作用户
	WORD							wProvideUser;						//供应用户
	DWORD							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbActionType);
		Stream_VALUE(wOperateUser);
		Stream_VALUE(wProvideUser);
		Stream_VALUE(cbOperateCode);
		Stream_VALUE(cbOperateCard);
	}
};
struct  HNMJGameRecord
{
	DWORD dwKindID;
	DWORD dwVersion;
	std::vector<HNMJGameRecordPlayer> kPlayers;
	std::vector<HNMJGameRecordOperateResult> kAction;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(HNMJGameRecordPlayer,kPlayers);
		StructVecotrMember(HNMJGameRecordOperateResult,kAction);
		Stream_VALUE(dwKindID);
		Stream_VALUE(dwVersion);
	}

};

struct MasterHandCardInfo
{
	int nChairId;
	std::vector<BYTE>    kMasterHandCard;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(nChairId);
		Stream_VECTOR(kMasterHandCard);
	}
};

struct MasterHandCard
{
	std::vector<MasterHandCardInfo>    kMasterHandCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(MasterHandCardInfo,kMasterHandCardList);
	}
};


//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//游戏变量
protected:
	HNMJGameRecord					m_kGameRecord;
	DWORD							m_dwGameRuleIdex;						//游戏规则
	BYTE                            m_cbMasterCheckCard[GAME_PLAYER];
	TCHAR							m_szPath[MAX_PATH];
	LONG							m_lSiceCount;							//骰子点数
	WORD							m_wBankerUser;							//庄家用户
	int								m_lGameScore[GAME_PLAYER];				//游戏得分
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//用户扑克
	bool							m_bTrustee[GAME_PLAYER];				//是否托管
	BYTE							m_cbListenStatus[GAME_PLAYER];			//听牌状态
	BYTE							m_cbQuanFeng;							//圈风
	BYTE							m_cbQuanCounter;						//圈风计数
	BYTE							m_cbHuaCardCount[GAME_PLAYER];			//花牌个数
	WORD							m_wReplaceUser;							//补牌用户

	WORD							m_lStartHuScore;						//起胡
	BYTE							m_cbChangeCard;							//财神牌CardIndex
	BYTE							m_cbLandCount;							//庄家次数
	WORD							m_wFirstUser;							//首骰用户
	WORD							m_wLandSice;							//庄家骰子
	WORD							m_wCaiShenIndex;						//财神位置，由色子点数之和确定
	WORD							m_wCaiShenChairID;						//财神在堆里牌中的位置
	WORD							m_wCaiShenSice;							//财神点数

	//出牌信息
protected:
	WORD							m_wOutCardUser;							//出牌用户
	BYTE							m_cbOutCardData;						//出牌扑克
	BYTE							m_cbOutCardCount;						//出牌数目
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//财神牌丢弃数目
	BYTE							m_cbDiscardCard[GAME_PLAYER][60];		//丢弃记录
	BYTE							m_cbCaiShenOutCnt[GAME_PLAYER];			//财神打出去的个数
	BYTE							m_cbHuangZhuang;						//黄庄
	//发牌信息
protected:
	BYTE							m_cbSendCardData;						//发牌扑克
	BYTE							m_cbSendCardCount;						//发牌数目
	BYTE							m_cbLeftCardCount;						//剩余数目
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//库存扑克
	BYTE							m_cbEndLeftCount;						//荒庄牌数

	//堆立变量
protected:
	WORD							m_wHeapHead;							//堆立头部
	WORD							m_wHeapTail;							//堆立尾部
	BYTE							m_cbHeapCardInfo[GAME_PLAYER][2];		//堆牌信息

	//运行变量
protected:
	WORD							m_wResumeUser;							//还原用户
	WORD							m_wCurrentUser;							//当前用户
	WORD							m_wProvideUser;							//供应用户
	BYTE							m_cbProvideCard;						//供应扑克

	//状态变量
protected:
	bool							m_bSendStatus;							//发牌状态
	bool							m_bGangStatus;							//抢杆状态
	bool							m_bEnjoinChiHu[GAME_PLAYER];			//禁止吃胡
	bool							m_bEnjoinChiPeng[GAME_PLAYER];			//禁止吃碰
	BYTE							m_cbUserMakeEye[GAME_PLAYER];
	bool							m_bOutMagicCard[GAME_PLAYER];			//打财神牌//*
	BYTE							m_cbMagicCardUser;						//打财神牌玩家

	bool							m_bOutGangHuCard[GAME_PLAYER];			//打杠胡牌//*
	BYTE							m_cbEnjoinHuCard[GAME_PLAYER][(MAX_COUNT-1)/2];//禁止胡牌

	//用户状态
public:
	bool							m_bResponse[GAME_PLAYER];				//响应标志
	BYTE							m_cbUserAction[GAME_PLAYER];			//用户动作
	BYTE							m_cbOperateCard[GAME_PLAYER][3];		//操作扑克
	BYTE							m_cbPerformAction[GAME_PLAYER];			//执行动作
	tagGangScore					m_GangScore[GAME_PLAYER];				//
	//组合扑克
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];			//组合数目
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//组合扑克

	//结束信息
protected:
	BYTE							m_cbChiHuCard;							//吃胡扑克
	DWORD							m_dwChiHuKind[GAME_PLAYER];				//吃胡结果
	CChiHuRight						m_ChiHuRight[GAME_PLAYER];				//吃胡权位

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数

	//属性变量
protected:

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { }
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	//信息接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){  return 0; };
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){ return 0; };
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:

	void setActionTimeStart(bool bStart,bool Trustee);
	//定时器事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize) { return false; }
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) { return false; }

	//网络接口
public:
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem);
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize) { return true; }

	virtual void SetPrivateInfo(BYTE bGameTypeIdex,DWORD bGameRuleIdex);
	virtual void SetCreateUserID(DWORD	dwUserID);
	//游戏事件
protected:
	//用户听牌
	bool OnUserListenCard(WORD wChairID,BYTE cbListen);
	//用户出牌
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	bool CanQiangGangHu();
	//用户操作
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3]);
	//用户补牌
	bool OnUserReplaceCard(WORD wChairID, BYTE cbCardData);

public:
	bool hasRule(BYTE cbRule);
	bool isHZGuiZe();
	DWORD AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
	DWORD AnalyseChiHuCardZZ(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSendCard);
	DWORD AnalyseChiHuCardCS(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSendCard);
	DWORD AnalyseChiHuCardCS_XIAOHU(const BYTE cbCardIndex[MAX_INDEX], CChiHuRight &ChiHuRight);

	//辅助函数
protected:
	//后台配置
	bool GetPrivateProfile();
	//发送操作
	bool SendOperateNotify();
	//派发扑克
	bool DispatchCardData(WORD wCurrentUser,bool bTail=false);

	//响应判断
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
	//算分
	BYTE CalScore( WORD wWinner, int lScore[GAME_PLAYER] );
	//从系统翻牌获取钻牌索引
	BYTE GetMagicIndex( BYTE cbFanCard );
	//权位过滤
	void FiltrateRight( WORD wWinner, CChiHuRight &chr );
	
	void clearUserActionStates(WORD cbChairID = INVALID_CHAIR);
	bool isUseTuoGuan();
	//发送托管
	void setChairTrustee(WORD iChairID,bool bTrustee);

	struct UserActionStates 
	{
		bool							m_bResponse[GAME_PLAYER];				//响应标志
		DWORD							m_cbUserAction[GAME_PLAYER];			//用户动作
		BYTE							m_cbOperateCard[GAME_PLAYER];			//操作扑克
		DWORD							m_cbPerformAction[GAME_PLAYER];			//执行动作
		CChiHuRight						m_ChiHuRight[GAME_PLAYER];				//
		BYTE							m_cbProvideCard;						//供应扑克
		BYTE							m_cbOutCardData;						//出牌扑克
		BYTE							m_cbSendCardData;						//发牌扑克
	};
	UserActionStates					m_kUserActionStates1;
	UserActionStates					m_kUserActionStates2;

	void starGameRecord();
	void addGameRecordAction(CMD_S_OperateResult kNetInfo);
	void addGameRecordAction(CMD_S_SendCard kNetInfo);
	void addGameRecordAction(CMD_S_OutCard kNetInfo);
	void addGameRecordAction(CMD_S_ChiHu kNetInfo);
public:
	void sendMasterHandCard();
	void sendMasterLeftCard(int nChairldID);
	BYTE getSendCardData(WORD dwSendUser);
//	void getXiaoHuCard(WORD nChairldID,CMD_S_XiaoHu& kXiaoHu);
	int  getLeftCardNum();
	// 去掉一个，财神牌
	int RemoveOneMagicCard(BYTE cbMagicCard);

};

//////////////////////////////////////////////////////////////////////////

#endif
