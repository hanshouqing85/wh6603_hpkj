
#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							201								//游戏标识
#define GAME_PLAYER						2									//游戏人数
#define GAME_NAME						TEXT("欢乐五张")					//游戏名字
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)					//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)					//程序版本
#define	GAME_FOLDER						TEXT("ShowHandTwo")
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型

//游戏状态
#define GS_TK_FREE					GAME_STATUS_FREE                        //等待开始
#define GS_TK_CALL					GAME_STATUS_PLAY						//叫庄状态
#define GS_TK_SCORE					GAME_STATUS_PLAY+1						//下注状态
#define GS_TK_PLAYING				GAME_STATUS_PLAY+2						//游戏进行
//////////////////////////////////////////////////////////////////////////
//命令码定义

#define SUB_C_ADD_GOLD					1									//用户加注
#define SUB_C_GIVE_UP					2									//放弃跟注
#define SUB_C_USE_WINNER                3                                   //控制胜负
#define SUB_C_CHECK_SUPER				4									//超级用户判断
#define SUB_C_SET_CARD					5                                   //取牌命令
#define SUB_C_SHOW_CARD					6									//看牌消息
		
#define SUB_S_GAME_START				100									//游戏开始
#define SUB_S_ADD_GOLD					101									//加注结果
#define SUB_S_GIVE_UP					102									//放弃跟注
#define SUB_S_SEND_CARD					103									//发牌消息
#define SUB_S_GAME_END					104									//游戏结束
#define SUB_S_ANDROID_WINNER			105									// 通知机器人，本局其最大
#define SUB_S_SHOW_CARD					106									//看牌
#define	SUB_S_SEND_HUMAN_CARD			107									//发送真人的明牌

#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif
//////////////////////////////////////////////////////////////////////////
//消息结构体

//游戏状态
struct CMD_S_StatusFree
{
	__int64								dwBasicGold;						//基础金币
	bool								bPoor;								//是否财神以下
};

//游戏状态
struct CMD_S_StatusPlay
{
	WORD				 				wCurrentUser;						//当前玩家
	BYTE								bPlayStatus[GAME_PLAYER];			//游戏状态
	BYTE								bTableCardCount[GAME_PLAYER];		//扑克数目
	BYTE								bTableCardArray[GAME_PLAYER][5];	//扑克数组
	__int64								lBasicGold;							//基础金币
	__int64								lTurnMaxGold;						//最大下注
	__int64								lTurnBasicGold;						//最少下注
	__int64								lTableGold[2*GAME_PLAYER];			//桌面金币
	__int64								lShowHandScore;						//
	BYTE								bShowHand;							//是否梭哈
	bool								bPoor;								//是否财神以下
};

//游戏开始
struct CMD_S_GameStart
{
	BYTE								bFundusCard;						//底牌扑克
	WORD				 				wCurrentUser;						//当前玩家
	__int64								lTurnMaxGold;						//最大下注
	__int64								lTurnBasicGold;						//最少下注
	__int64								lBasicGold;							//单元下注
	BYTE								bCardData[GAME_PLAYER];				//用户扑克
	__int64								lShowHandScore;						//
	bool								bPoor;								//是否财神以下
};

//用户放弃
struct CMD_S_GiveUp
{
	WORD								wUserChairID;						//放弃用户
	__int64								lTurnMaxGold;						//
};

//用户加注
struct CMD_C_AddGold
{
	bool								bFllow;								//是否跟
	__int64								lGold;								//加注数目
};
//看牌
struct CMD_C_ShowCard
{
	WORD								wChairID;                           //用户椅子ID
};
//加注结果
struct CMD_S_AddGold
{
	WORD								wLastChairID;						//上一用户
	WORD								wCurrentUser;						//当前用户
	__int64								lLastAddGold;						//加注数目
	__int64								lCurrentLessGold;					//最少加注
	bool								bShowHand;							//是否梭哈
	bool								bFllow;								//是否跟
};

//发牌数据包
struct CMD_R_SendCard
{
	__int64								lMaxGold;							//最大下注
	WORD								wCurrentUser;						//当前用户
	WORD								wStartChairId;						//
	BYTE								cbSendCardCount;					//
	BYTE								bUserCard[GAME_PLAYER][2];			//用户扑克
	WORD								wWin;
};

//发真人的明牌给机器人
struct	CMD_S_ShowHumanCard
{
	WORD								wChairID;							//该明牌所在位置
	BYTE								cbUserCard[5];						//明牌信息
	BYTE								cbCardCount;						//明牌的数量
};

//游戏结束
struct CMD_S_GameEnd
{
	__int64								lTax;								//游戏税收
	__int64								lGameGold[GAME_PLAYER];				//游戏得分
	BYTE								bUserCard[GAME_PLAYER];				//用户扑克
};

struct CMD_S_AndroidWinner
{
	WORD              wWinner;  // 控制赢的玩家位置
};

struct CMD_C_UseWinner
{
	WORD              wWinner;  // 控制赢的玩家位置
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()

#endif
