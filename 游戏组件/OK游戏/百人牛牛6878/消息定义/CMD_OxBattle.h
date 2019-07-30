#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						104									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("百人牛牛")					//游戏名字

//状态定义
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//结束状态
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+2					//结束状态

#define VERSION_SERVER			    PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

//区域索引
#define ID_TIAN_MEN					1									//天
#define ID_DI_MEN					2									//地
#define ID_XUAN_MEN					3									//玄
#define ID_HUANG_MEN				4									//黄

#define SCORE						DOUBLE								//积分类型
#define SCORE_STRING				TEXT("%0.2f")						//积分类型
#define SCORE_ZERO                  1.175494351e-38F                    //积分类型

//玩家索引
#define BANKER_INDEX				0									//庄家索引
#define SHUN_MEN_INDEX				1									//顺门索引
#define DUI_MEN_INDEX				2									//对门索引
#define DAO_MEN_INDEX				3									//倒门索引
#define HUAN_MEN_INDEX				4									//倒门索引
#define MAX_INDEX					3									//最大索引

#define AREA_COUNT					4									//区域数目

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

//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率
#define SERVER_LEN					32									//房间长度

//机器人信息
struct tagRobotInfo
{
	int nChip[8];														//筹码定义
	int nAreaChance[AREA_COUNT];										//区域几率
	TCHAR szCfgFileName[MAX_PATH];										//配置文件
	int	nMaxTime;														//最大赔率

	tagRobotInfo()
	{
		int nTmpChip[8] = {1, 5, 10, 50, 100, 200, 500, 1000};
		int nTmpAreaChance[AREA_COUNT] = {1, 1, 1, 1};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("OxBattleConfig.ini");

		nMaxTime = 5;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//记录信息
struct tagServerGameRecord
{
	bool							bWinShunMen;						//顺门胜利
	bool							bWinDuiMen;							//对门胜利
	bool							bWinDaoMen;							//倒门胜利
	bool							bWinHuang;							//倒门胜利
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE				99									//游戏空闲
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_PLACE_JETTON			101									//用户下注
#define SUB_S_GAME_END				102									//游戏结束
#define SUB_S_APPLY_BANKER			103									//申请庄家
#define SUB_S_CHANGE_BANKER			104									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		105									//更新积分
#define SUB_S_SEND_RECORD			106									//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		107									//下注失败
#define SUB_S_CANCEL_BANKER			108									//取消申请
#define SUB_S_SEND_ACCOUNT			109									//发送账号
#define SUB_S_ADMIN_CHEAK			111									//查询账号


#define SUB_S_AMDIN_COMMAND			110									//管理员命令

//请求回复
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//回复类型
		#define ACK_SET_WIN_AREA  1
		#define ACK_PRINT_SYN     2
		#define ACK_RESET_CONTROL 3
	BYTE cbResult;
		#define CR_ACCEPT  2			//接受
		#define CR_REFUSAL 3			//拒绝
	BYTE cbExtendData[20];			//附加数据
};
//失败结构
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//下注玩家
	BYTE							lJettonArea;						//下注区域
	LONGLONG						lPlaceScore;						//当前下注
};

//更新积分
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//椅子号码
	DOUBLE							lScore;								//玩家积分

	//庄家信息
	WORD							wCurrentBankerChairID;				//当前庄家
	BYTE							cbBankerTime;						//庄家局数
	DOUBLE							lCurrentBankerScore;				//庄家分数
};

//上庄列表玩家
struct tagBankerListPlayer
{
	tagBankerListPlayer(){memset(this, 0, sizeof(*this));}
    WORD wChairID;
	DWORD dwUserID;
    DOUBLE lScore;
	BYTE cbGender;
    TCHAR szNickName[32];
};

//上庄列表
struct CMD_S_ApplyBankerList
{
	CMD_S_ApplyBankerList(){memset(this, 0, sizeof(*this));}
	BYTE cbCount;//	上庄列表玩家个数
	inline tagBankerListPlayer & operator [](unsigned int);
	int getLength()const{return sizeof(*this) + cbCount * sizeof(tagBankerListPlayer);}
};

struct CMD_S_ApplyBankerListMax:public CMD_S_ApplyBankerList
{
	tagBankerListPlayer wApplyList[GAME_PLAYER];
};

//申请庄家
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//申请玩家
};

//取消申请
struct CMD_S_CancelBanker
{
	TCHAR							szCancelUser[32];					//取消玩家
	WORD							wChairID;							//用户椅子ID
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	TCHAR							szBankerNickName[32];				//	庄家昵称
	BYTE							cbBankerGender;						//	庄家性别
	LONGLONG						lBankerScore;						//庄家金币
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//玩家信息
	LONGLONG						lUserMaxScore;						//玩家金币

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	TCHAR							szBankerNickName[32];				//	庄家昵称
	BYTE							cbBankerGender;						//	庄家性别
	WORD							cbBankerTime;						//庄家局数
	LONGLONG						lBankerWinScore;					//庄家成绩
	LONGLONG						lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//房间信息
	TCHAR							szGameRoomName[SERVER_LEN];			//房间名称
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局下注
	LONGLONG						lAllJettonScore[AREA_COUNT+1];		//全体总注

	//玩家下注
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//个人总注

	//玩家积分
	LONGLONG						lUserMaxScore;						//最大下注							

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//扑克信息
	BYTE							cbTableCardArray[5][5];				//桌面扑克

	bool							bResult[AREA_COUNT];				//	胜负结果

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	TCHAR							szBankerNickName[32];				//	庄家昵称
	BYTE							cbBankerGender;						//	庄家性别
	WORD							cbBankerTime;						//庄家局数
	LONGLONG						lBankerWinScore;					//庄家赢分
	LONGLONG						lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//结束信息
	LONGLONG						lEndBankerScore;					//庄家成绩
	LONGLONG						lEndUserScore;						//玩家成绩
	LONGLONG						lEndUserReturnScore;				//返回积分
	LONGLONG						lEndRevenue;						//游戏税收

	//全局信息
	BYTE							cbTimeLeave;						//剩余时间
	BYTE							cbGameStatus;						//游戏状态

	//房间信息
	TCHAR							szGameRoomName[SERVER_LEN];			//房间名称
};

//游戏空闲
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//剩余时间
	INT64                           nListUserCount;						//列表人数
};

//游戏开始
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//庄家位置
	LONGLONG						lBankerScore;						//庄家金币
	LONGLONG						lUserMaxScore;						//我的金币
	BYTE							cbTimeLeave;						//剩余时间	
	bool							bContiueCard;						//继续发牌
	int								nChipRobotCount;					//人数上限 (下注机器人)
};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
	bool							bIsAndroid;							//是否机器人
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
	BYTE							cbTableCardArray[5][5];				//桌面扑克
	BYTE							cbLeftCardCount;					//扑克数目

	bool							bResult[AREA_COUNT];				//	胜负结果

	BYTE							bcFirstCard[2];
 
	//庄家信息
	SCORE						lBankerScore;						//庄家成绩
	SCORE						lBankerTotallScore;					//庄家成绩
	INT								nBankerTime;						//做庄次数

	//玩家成绩
	SCORE						lUserScore;							//玩家成绩
	SCORE						lUserReturnScore;					//返回积分

	//全局信息
	SCORE						lRevenue;							//游戏税收
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_CONTINUE_CARD			4									//继续发牌
#define SUB_C_AMDIN_COMMAND			5									//管理员命令
#define SUB_C_GET_ACCOUNT			6									//获取昵称
#define SUB_C_CHEAK_ACCOUNT			7									//获取昵称
#define SUB_C_GET_PLAYLIST			8									//上庄列表

//客户端消息
#define IDM_ADMIN_COMMDN			WM_USER+1000
#define IDM_GET_ACCOUNT				WM_USER+1001
#define IDM_CHEAK_ACCOUNT			WM_USER+1002

struct CMD_C_SendAccount
{
	DWORD							m_UserID[GAME_PLAYER];					//玩家ID
	DWORD							m_UserCount;							//玩家数量
};
struct CMD_S_UserJettonScore
{
	//玩家下注
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//个人总注
};
//控制区域信息
struct tagControlInfo
{
	BYTE cbControlArea[MAX_INDEX];			//控制区域
};

struct CMD_C_CheakAccount  //
{
	TCHAR							dwUserAccount[32];					//取消玩家
};

struct tagAdminReq
{
	BYTE							m_cbExcuteTimes;					//执行次数
	BYTE							m_cbControlStyle;					//控制方式
#define		CS_BANKER_LOSE    1
#define		CS_BANKER_WIN	  2
#define		CS_BET_AREA		  3
	bool							m_bWinArea[3];						//赢家区域
};


struct CMD_C_AdminReq
{
	BYTE cbReqType;
#define RQ_SET_WIN_AREA		1
#define RQ_RESET_CONTROL	2
#define RQ_PRINT_SYN		3
	BYTE cbExtendData[20];			//附加数据
};
//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
