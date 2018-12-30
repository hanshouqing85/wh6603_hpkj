#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//服务定义

//游戏属性
#define KIND_ID						200									//游戏 I D
#define GAME_NAME					TEXT("斗地主")						//游戏名字

//组件属性
#define GAME_PLAYER					3									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

//////////////////////////////////////////////////////////////////////////////////

//数目定义
#define MAX_COUNT					20									//最大数目
#define FULL_COUNT					54									//全牌数目

//逻辑数目
#define NORMAL_COUNT				17									//常规数目
#define DISPATCH_COUNT				51									//派发数目
#define GOOD_CARD_COUTN				38									//好牌数目

//数值掩码
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//逻辑类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对牌类型
#define CT_THREE					3									//三条类型
#define CT_SINGLE_LINE				4									//单连类型
#define CT_DOUBLE_LINE				5									//对连类型
#define CT_THREE_LINE				6									//三连类型
#define CT_THREE_TAKE_ONE			7									//三带一单
#define CT_THREE_TAKE_TWO			8									//三带一对
#define CT_FOUR_TAKE_ONE			9									//四带两单
#define CT_FOUR_TAKE_TWO			10									//四带两对
#define CT_BOMB_CARD				11									//炸弹类型
#define CT_MISSILE_CARD				12									//火箭类型

//////////////////////////////////////////////////////////////////////////////////
//状态定义

//#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
//#define GAME_SCENE_CALL				GAME_STATUS_PLAY					//叫分状态
//#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//游戏进行

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_CALL				GAME_STATUS_PLAY					//叫地主状态
#define GAME_SCENE_ROD				GAME_STATUS_PLAY+1					//抢地主状态
//#define GAME_SCENE_ADD				GAME_STATUS_PLAY+2					//加倍状态
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+2					//游戏进行


//叫地主
#define CB_NOT_CALL					0									//没叫地主
#define CB_CALL_BENKER				1									//叫地主
#define CB_NO_CALL_BENKER			2									//不叫地主

//抢地主
#define CB_NOT_ROD					0									//没抢地主
#define CB_ROD_BANKER				1									//抢地主
#define CB_NO_ROD_BANKER			2									//不抢地主
#define CB_CAN_NO_ROD				3									//不能抢

//加倍信息
#define CB_NOT_ADD_DOUBLE			0									//没加倍
#define CB_ADD_DOUBLE				1									//加倍标志
#define CB_NO_ADD_DOUBLE			2									//不加倍




//空闲状态
struct CMD_S_StatusFree
{
	//游戏属性
	LONG							lCellScore;							//基础积分

	//时间信息
	BYTE							cbTimeCallBanker;					//叫地主时间
	BYTE							cbTimeRodBanker;					//抢地主时间
	BYTE							cbTimeOutCard;						//出牌时间
	//BYTE							cbTimeCallScore;					//叫分时间
	BYTE							cbTimeStartGame;					//开始时间
	BYTE							cbTimeHeadOutCard;					//首出时间

	//历史积分
	SCORE							lTurnScore[GAME_PLAYER];			//积分信息
	SCORE							lCollectScore[GAME_PLAYER];			//积分信息
};

////叫分状态
//struct CMD_S_StatusCall
//{
//	//时间信息
//	BYTE							cbTimeOutCard;						//出牌时间
//	BYTE							cbTimeCallScore;					//叫分时间
//	BYTE							cbTimeStartGame;					//开始时间
//	BYTE							cbTimeHeadOutCard;					//首出时间
//
//	//游戏信息
//	LONG							lCellScore;							//单元积分
//	WORD							wCurrentUser;						//当前玩家
//	BYTE							cbBankerScore;						//庄家叫分
//	BYTE							cbScoreInfo[GAME_PLAYER];			//叫分信息
//	BYTE							cbHandCardData[NORMAL_COUNT];		//手上扑克
//
//	//历史积分
//	SCORE							lTurnScore[GAME_PLAYER];			//积分信息
//	SCORE							lCollectScore[GAME_PLAYER];			//积分信息
//};

//叫地主状态
struct CMD_S_StatusCall
{
	//时间信息
	BYTE							cbTimeCallBanker;					//叫地主时间
	BYTE							cbTimeRodBanker;					//抢地主时间
	//BYTE							cbTimeAddDouble;					//加倍时间
	BYTE							cbTimeStartGame;					//开始时间
	BYTE							cbTimeHeadOutCard;					//首出时间
	BYTE							cbTimeOutCard;						//出牌时间

	//游戏信息
	LONG							lCellScore;							//单元积分
	WORD							wFirstUser;							//首叫用户
	//WORD							wFirstValidUser;					//首叫明牌
	WORD							wCurrentUser;						//当前玩家
	WORD							wStartTime;							//初始倍数
	//WORD							wValidCardTime;						//明牌倍数
	//bool							bValidCardInfo[GAME_PLAYER];		//明牌信息
	BYTE							cbCallBankerInfo[GAME_PLAYER];		//叫地主信息	
	BYTE							cbHandCardData[GAME_PLAYER][NORMAL_COUNT];		//手上扑克

	//历史积分
	SCORE							lTurnScore[GAME_PLAYER];			//积分信息
	SCORE							lCollectScore[GAME_PLAYER];			//积分信息
};

//抢地主状态
struct CMD_S_StatusRod
{
	//时间信息
	BYTE							cbTimeCallBanker;					//叫地主时间
	BYTE							cbTimeRodBanker;					//抢地主时间
	//BYTE							cbTimeAddDouble;					//加倍时间
	BYTE							cbTimeStartGame;					//开始时间
	BYTE							cbTimeHeadOutCard;					//首出时间
	BYTE							cbTimeOutCard;						//出牌时间

	//游戏信息
	LONG							lCellScore;							//单元积分
	WORD							wFirstUser;							//首叫用户
	//WORD							wFirstValidUser;					//首叫明牌
	WORD							wBankerUser;						//庄家用户
	WORD							wCurrentUser;						//当前用户

	WORD							wStartTime;							//初始倍数
	//WORD							wValidCardTime;						//明牌倍数
	WORD							wRodBankerTime;						//抢地主倍数

	//bool							bValidCardInfo[GAME_PLAYER];		//明牌信息
	BYTE							cbCallBankerInfo[GAME_PLAYER];		//叫地主信息	
	BYTE							cbRodBankerInfo[GAME_PLAYER];		//抢地主信息
	//BYTE							cbValidCardInfo[GAME_PLAYER];		//明牌信息
	BYTE							cbHandCardData[GAME_PLAYER][NORMAL_COUNT];//手上扑克

	//历史积分
	SCORE							lTurnScore[GAME_PLAYER];			//积分信息
	SCORE							lCollectScore[GAME_PLAYER];			//积分信息
};

////游戏状态
//struct CMD_S_StatusPlay
//{
//	//时间信息
//	BYTE							cbTimeOutCard;						//出牌时间
//	BYTE							cbTimeCallScore;					//叫分时间
//	BYTE							cbTimeStartGame;					//开始时间
//	BYTE							cbTimeHeadOutCard;					//首出时间
//
//	//游戏变量
//	LONG							lCellScore;							//单元积分
//	BYTE							cbBombCount;						//炸弹次数
//	WORD							wBankerUser;						//庄家用户
//	WORD							wCurrentUser;						//当前玩家
//	BYTE							cbBankerScore;						//庄家叫分
//
//	//出牌信息
//	WORD							wTurnWiner;							//胜利玩家
//	BYTE							cbTurnCardCount;					//出牌数目
//	BYTE							cbTurnCardData[MAX_COUNT];			//出牌数据
//
//	//扑克信息
//	BYTE							cbBankerCard[3];					//游戏底牌
//	BYTE							cbHandCardData[MAX_COUNT];			//手上扑克
//	BYTE							cbHandCardCount[GAME_PLAYER];		//扑克数目
//
//	//历史积分
//	SCORE							lTurnScore[GAME_PLAYER];			//积分信息
//	SCORE							lCollectScore[GAME_PLAYER];			//积分信息
//};

//游戏状态
struct CMD_S_StatusPlay
{
	//时间信息
	BYTE							cbTimeCallBanker;					//叫地主时间
	BYTE							cbTimeRodBanker;					//抢地主时间
	BYTE							cbTimeAddDouble;					//加倍时间
	BYTE							cbTimeStartGame;					//开始时间
	BYTE							cbTimeHeadOutCard;					//首出时间
	BYTE							cbTimeOutCard;						//出牌时间

	//游戏变量
	LONG							lCellScore;							//单元积分
	WORD							wBankerUser;						//庄家用户
	WORD							wCurrentUser;						//当前玩家

	WORD							wStartTime;							//初始倍数
	//WORD							wValidCardTime;						//明牌倍数
	WORD							wRodBankerTime;						//抢地主倍数
	WORD							wBackCardTime;						//底牌倍数
	WORD							wBombTime;							//炸弹倍数
	WORD							wCardTypeTime;						//牌型倍数

	BYTE							cbBombCount;						//炸弹次数
	BYTE							cbCallBankerInfo[GAME_PLAYER];		//叫地主信息	
	BYTE							cbRodBankerInfo[GAME_PLAYER];		//抢地主信息
	BYTE							cbAddDoubleInfo[GAME_PLAYER];		//加倍信息	
	//bool							bValidCardInfo[GAME_PLAYER];		//明牌信息

	//出牌信息
	WORD							wTurnWiner;							//胜利玩家
	BYTE							cbTurnCardCount;					//出牌数目
	BYTE							cbTurnCardData[MAX_COUNT];			//出牌数据

	//扑克信息
	BYTE							cbBankerCard[3];					//游戏底牌
	BYTE							cbHandCardCount[GAME_PLAYER];		//扑克数目
	BYTE							cbHandCardData[GAME_PLAYER][MAX_COUNT];		//手上扑克

	//历史积分
	SCORE							lTurnScore[GAME_PLAYER];			//积分信息
	SCORE							lCollectScore[GAME_PLAYER];			//积分信息
};


//////////////////////////////////////////////////////////////////////////////////
//命令定义

//#define SUB_S_GAME_START			100									//游戏开始
//#define SUB_S_CALL_SCORE			101									//用户叫分
//#define SUB_S_BANKER_INFO			102									//庄家信息
//#define SUB_S_OUT_CARD			103									//用户出牌
//#define SUB_S_PASS_CARD			104									//用户放弃
//#define SUB_S_GAME_CONCLUDE		105									//游戏结束
//#define SUB_S_SET_BASESCORE		106									//设置基数
#define SUB_S_CHEAT_CARD			111									//作弊扑克

#define SUB_S_GAME_START			100									//游戏开始
//#define SUB_S_CALL_SCORE			101									//用户叫分
#define SUB_S_CALL_BANKER			101									//叫地主
#define SUB_S_BANKER_INFO			102									//庄家信息
#define SUB_S_ROD_BANKER			103									//抢地主
//#define SUB_S_DOUBLE				104									//加倍信息
//#define SUB_S_VALID_CARD			105									//用户明牌
#define SUB_S_OUT_CARD				106									//用户出牌
#define SUB_S_PASS_CARD				107									//用户放弃
#define SUB_S_OUT_START_START		108									//开始出牌
#define SUB_S_GAME_CONCLUDE			109									//游戏结束
#define SUB_S_SET_BASESCORE			110									//设置基数

//发送扑克
struct CMD_S_GameStart
{
	WORD							wStartUser;							//开始玩家
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wStartTime;							//初始倍数
	//bool							bValidCard;							//是否明牌
	//BYTE							cbValidCardData;					//明牌扑克
	//BYTE							cbValidCardIndex;					//明牌位置
	BYTE							cbCardData[NORMAL_COUNT];			//扑克列表
};

//机器人扑克
struct CMD_S_AndroidCard
{
	BYTE							cbHandCard[GAME_PLAYER][NORMAL_COUNT];//手上扑克
	WORD							wCurrentUser ;						//当前玩家
};

//作弊扑克
struct CMD_S_CheatCard
{
	WORD							wCardUser[GAME_PLAYER ];				//作弊玩家
	BYTE							cbUserCount;							//作弊数量
	BYTE							cbCardData[GAME_PLAYER ][MAX_COUNT];	//扑克列表
	BYTE							cbCardCount[GAME_PLAYER ];				//扑克数量

};
////用户叫分
//struct CMD_S_CallScore
//{
//	WORD				 			wCurrentUser;						//当前玩家
//	WORD							wCallScoreUser;						//叫分玩家
//	BYTE							cbCurrentScore;						//当前叫分
//	BYTE							cbUserCallScore;					//上次叫分
//
//
//};
//用户叫地主
struct CMD_S_CallBanker
{
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wLastUser;							//叫地主玩家
	BYTE							cbCallInfo;							//叫地主
};

//用户抢地主
struct CMD_S_RodBanker
{
	WORD							wRodBankerTime;						//抢地主倍数

	WORD				 			wBankerUser;						//庄家玩家
	WORD				 			wCurrentUser;						//当前玩家	
	WORD							wLastUser;							//抢地主玩家
	BYTE							cbRodlInfo;							//抢地主
};
////加倍信息
//struct CMD_S_Double
//{
//	WORD							wCurrentUser;						//当前用户
//	BYTE							cbDouble;							//是否加倍
//};

////庄家信息
//struct CMD_S_BankerInfo
//{
//	WORD				 			wBankerUser;						//庄家玩家
//	WORD				 			wCurrentUser;						//当前玩家
//	BYTE							cbBankerScore;						//庄家叫分
//	BYTE							cbBankerCard[3];					//庄家扑克
//};

//庄家信息
struct CMD_S_BankerInfo
{
	WORD				 			wBankerUser;						//庄家玩家
	WORD				 			wCurrentUser;						//当前玩家
	//WORD							wBackCardTime;						//底牌倍数
	BYTE							cbBankerCard[3];					//庄家扑克
	//BYTE							cbBackCardType;						//底牌类型
	//bool							bDoubleInfo[3];						//能否加倍
};

//开始信息
struct CMD_S_Game_Start
{
	WORD				 			wBankerUser;						//庄家玩家
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							cbCardData[MAX_COUNT];				//扑克数据

};

//用户出牌
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wOutCardUser;						//出牌玩家
	BYTE							cbCardData[MAX_COUNT];				//扑克列表
};

//放弃出牌
struct CMD_S_PassCard
{
	BYTE							cbTurnOver;							//一轮结束
	WORD				 			wCurrentUser;						//当前玩家
	WORD				 			wPassCardUser;						//放弃玩家
};

//游戏结束
struct CMD_S_GameConclude
{
	//积分变量
	LONG							lCellScore;							//单元积分
	SCORE							lGameScore[GAME_PLAYER];			//游戏积分

	//春天标志
	BYTE							bChunTian;							//春天标志
	BYTE							bFanChunTian;						//春天标志

	//炸弹信息
	BYTE							cbBombCount;						//炸弹个数
	BYTE							cbEachBombCount[GAME_PLAYER];		//炸弹个数

	//游戏信息
	//BYTE							cbBankerScore;						//叫分数目
	BYTE							cbCardCount[GAME_PLAYER];			//扑克数目
	BYTE							cbHandCardData[FULL_COUNT];			//扑克列表
};

//托管
//struct CMD_S_TRUSTEE
//{
//	WORD							wTrusteeUser;						//托管玩家
//	BYTE							bTrustee;							//托管标志
//};

//////////////////////////////////////////////////////////////////////////////////
//命令定义
//
//#define SUB_C_CALL_SCORE			1									//用户叫分
//#define SUB_C_OUT_CARD			2									//用户出牌
//#define SUB_C_PASS_CARD			3									//用户放弃


//#define SUB_C_VALID_CARD			1									//用户明牌
//#define SUB_C_CALL_SCORE			2									//用户叫分
#define SUB_C_CALL_BANKER			2									//用户叫地主
#define SUB_C_ROD_BANKER			3									//用户抢地主
#define SUB_C_DOUBLE				4									//用户加倍
#define SUB_C_OUT_CARD				5									//用户出牌
#define SUB_C_PASS_CARD				6									//用户放弃
#define SUB_C_TRUSTEE				7									//用户托管

////用户叫分
//struct CMD_C_CallScore
//{
//	BYTE							cbCallScore;						//叫分数目
//};

//用户叫地主
struct CMD_C_CallBanker
{				
	BYTE							cbCallInfo;							//叫地主
};

//用户抢地主
struct CMD_C_RodBanker
{				
	BYTE							cbRodInfo;							//抢地主
};
//用户出牌
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	BYTE							cbCardData[MAX_COUNT];				//扑克数据
};
////托管
//struct CMD_C_TRUSTEE
//{
//	BYTE							bTrustee;							//托管标志
//};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif