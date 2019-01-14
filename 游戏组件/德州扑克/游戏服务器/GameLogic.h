#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"
//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
#define CT_SINGLE					1									//单牌类型
#define CT_ONE_LONG					2									//对子类型
#define CT_TWO_LONG					3									//两对类型
#define CT_THREE_TIAO				4									//三条类型
#define	CT_SHUN_ZI					5									//顺子类型
#define CT_TONG_HUA					6									//同花类型
#define CT_HU_LU					7									//葫芦类型
#define CT_TIE_ZHI					8									//铁支类型
#define CT_TONG_HUA_SHUN			9									//同花顺型
#define CT_KING_TONG_HUA_SHUN		10									//皇家同花顺

//////////////////////////////////////////////////////////////////////////

//胜利信息结构
struct UserWinList
{
	BYTE bSameCount;
	WORD wWinerList[GAME_PLAYER];
};

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//四张数目
	BYTE 							cbThreeCount;						//三张数目
	BYTE 							cbLONGCount;						//两张数目
	BYTE							cbSignedCount;						//单张数目
	BYTE 							cbFourLogicVolue[1];				//四张列表
	BYTE 							cbThreeLogicVolue[1];				//三张列表
	BYTE 							cbLONGLogicVolue[2];				//两张列表
	BYTE 							cbSignedLogicVolue[5];				//单张列表
	BYTE							cbFourCardData[MAX_CENTERCOUNT];	//四张列表
	BYTE							cbThreeCardData[MAX_CENTERCOUNT];	//三张列表
	BYTE							cbLONGCardData[MAX_CENTERCOUNT];	//两张列表
	BYTE							cbSignedCardData[MAX_CENTERCOUNT];	//单张数目
};
//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class CGameLogic
{
	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//变量定义
private:
	static BYTE						m_cbCardData[FULL_COUNT];				//扑克定义

	//控制函数
public:
	//排列扑克
	static void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	static VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//类型函数
public:
	//获取类型
	static BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取数值
	static BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	static BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//功能函数
public:
	//逻辑数值
	static BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	static BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
	//分析扑克
	static void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//7返5
	static BYTE FiveFromSeven(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbCenterCardData[],BYTE cbCenterCardCount,BYTE cbLastCardData[],BYTE cbLastCardCount);
	//查找最大
	static bool SelectMaxUser(BYTE bCardData[GAME_PLAYER][MAX_CENTERCOUNT],UserWinList &EndResult,const DZPKSCORE lAddScore[]);
};

//定义函数对象用来排序
class sort_tagMadeHandsOrder
{
public:
	sort_tagMadeHandsOrder(){}
	bool operator()(tagMadeHandsOrder& X,tagMadeHandsOrder& Y)const
	{
		BYTE iRet=CGameLogic::CompareCard(X.cbLastCenterCardData,Y.cbLastCenterCardData,MAX_CENTERCOUNT);
		// 在德克萨斯扑克里，四种花色不分大小
		if(iRet==0)
		{
		   // 得到一种稳定的排序关系
		   BYTE XLogic[5];
		   XLogic[0]=CGameLogic::GetCardLogicValue(X.cbCardData[0]);
		   XLogic[1]=CGameLogic::GetCardLogicValue(X.cbCardData[1]);
		   XLogic[2]=CARD_COLOR(X.cbCardData[0]);
		   XLogic[3]=CARD_COLOR(X.cbCardData[1]);
		   XLogic[4]=(XLogic[0]+XLogic[1]+XLogic[2]+XLogic[3]);
		   BYTE YLogic[5];
		   YLogic[0]=CGameLogic::GetCardLogicValue(Y.cbCardData[0]);
		   YLogic[1]=CGameLogic::GetCardLogicValue(Y.cbCardData[1]);
		   YLogic[2]=CARD_COLOR(Y.cbCardData[0]);
		   YLogic[3]=CARD_COLOR(Y.cbCardData[1]);
		   YLogic[4]=(YLogic[0]+YLogic[1]+YLogic[2]+YLogic[3]);
		   return XLogic[4]>YLogic[4];
		}
		return iRet>1;
	}
};

//////////////////////////////////////////////////////////////////////////

#endif
