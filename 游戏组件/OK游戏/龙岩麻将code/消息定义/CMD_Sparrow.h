#ifndef CMD_SPARROW_LONGYAN_MJ_HEAD_FILE
#define CMD_SPARROW_LONGYAN_MJ_HEAD_FILE
#include <vector>

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#pragma pack(1)

#define GAME_PLAYER					4									//游戏人数
#ifdef USE_GB
#define KIND_ID						512									//游戏 I D
#define GAME_NAME					TEXT("大众麻将")					//游戏名字
#else
#define KIND_ID						311									//游戏 I D
#define GAME_NAME					TEXT("龙岩麻将")					//游戏名字
#endif
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//游戏类型
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本///////////
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本//////////

//游戏状态
#define GS_MJ_FREE					GAME_STATUS_FREE								//空闲状态
#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)						//游戏状态
#define GS_MJ_XIAOHU				(GAME_STATUS_PLAY+2)							//小胡状态
#define GS_MJ_PIAO					(GAME_STATUS_PLAY+3)							//飘状态
	

//常量定义
#define MAX_WEAVE					4									//最大组合
#define MAX_INDEX					42									//最大索引
#define MAX_COUNT					14									//最大数目
#define MAX_REPERTORY				144									//最大库存
#define MAX_HUA_CARD				8									//花牌个数

//扑克定义
#define HEAP_FULL_COUNT				(136/4)									//堆立全牌

#define MAX_RIGHT_COUNT				3									//最大权位DWORD个数			

#define	CAI_SHEN_CARD						MAX_INDEX
//////////////////////////////////////////////////////////////////////////

//游戏规则
#define GAME_TYPE_ZZ_ZIMOHU			1		//只能自模胡
#define GAME_TYPE_ZZ_QIDUI			2		//可胡七对
#define GAME_TYPE_ZZ_QIANGGANGHU	3		//可抢杠胡
#define GAME_TYPE_ZZ_ZHANIAO2		4		//扎鸟2个
#define GAME_TYPE_ZZ_ZHANIAO4		5		//扎鸟4个
#define GAME_TYPE_ZZ_ZHANIAO6		6		//扎鸟6个
#define GAME_TYPE_ZZ_HONGZHONG		7		//红中癞子
#define GAME_TYPE_CS_ZHUANGXIANFEN	8		//庄闲分
#define GAME_TYPE_ZZ_ZHANIAO3		9		//扎鸟3个
#define GAME_TYPE_ZZ_ZHANIAO5		10		//扎鸟5个
#define GAME_TYPE_ZZ_258			11		//258做将 长沙麻将
#define GAME_TYPE_ZZ_HONGZHONG_GZ	12		//红中玩法 癞子算红中2分 不可接炮

#define	GAME_TYPE_MJ_MAX_WANFA		2		//最大玩法个数

#define	GAME_TYPE_LESS_HU_ONE		1		//一片起胡
#define	GAME_TYPE_LESS_HU_TW0		2		//两片起胡


#define GAME_TYPE_ZZ_LIUJU_4CARD	13		//剩余4张黄庄
#define GAME_TYPE_ZZ_ZHANIAO1		14		//扎鸟1个
#define GAME_TYPE_ZZ_ZHANIAO_DOUBLE	15		//扎鸟翻倍
#define GAME_TYPE_ZZ_ZHANIAO_DIFF	16		//扎鸟分开算
#define GAME_TYPE_ZZ_ZHANIAO_2FEN	17		//扎鸟+2分
#define GAME_TYPE_ZZ_PIAO			18		//飘
#define GAME_TYPE_BU_NIAO			19		//牌不够补鸟





//组合子项
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户
	BYTE							cbCardData[4];						//组合数据
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_OUT_CARD				101									//出牌命令
#define SUB_S_SEND_CARD				102									//发送扑克
#define SUB_S_LISTEN				103									//用户听牌
#define SUB_S_OPERATE_NOTIFY		104									//操作提示
#define SUB_S_OPERATE_RESULT		105									//操作命令
#define SUB_S_GAME_END				106									//游戏结束
#define SUB_S_TRUSTEE				107									//用户托管
#define SUB_S_REPLACE_CARD			108									//用户补牌
#define SUB_S_CHI_HU				109								//
#define SUB_S_GANG_SCORE			110									//
#define	SUB_S_GAME_END_LAST_CARD	111
#define SUB_S_MASTER_HANDCARD		120									//
#define SUB_S_MASTER_LEFTCARD		121									//剩余牌堆


//胡牌规则：
//胡牌
#define CHK_NULL					0x00										//非胡类型
#define CHK_CHI_HU				0x01										//胡类型

// 大胡
#define CHR_PENGPENG_HU						0x00000001									//碰碰胡
#define CHR_JIANGJIANG_HU					0x00000002									//将将胡
#define CHR_QING_YI_SE						0x00000004									//清一色
#define CHR_HAI_DI_LAO						0x00000008									//海底捞
#define CHR_HAI_DI_PAO						0x00000010									//海底捞
#define CHR_QI_XIAO_DUI						0x00000020									//七小对
#define CHR_HAOHUA_QI_XIAO_DUI				0x00000040									//豪华七小对
//#define CHR_GANG_KAI						0x00000080									//杠上开花
#define CHR_QIANG_GANG_HU					0x00000100									//抢杠胡
#define CHR_GANG_SHANG_PAO					0x00000200									//杠上跑
#define CHR_QUAN_QIU_REN					0x00000400									//全求人
#define CHR_SHUANG_HAOHUA_QI_XIAO_DUI		0x00000800									//双豪华七小对
#define CHR_HONGZHONG_WU					0x00001000									//无红中多加一鸟
#define CHR_GANG_SHUANG_KAI					0x00002000									//长沙双杠开
#define WIK_BU_ZHANG						0x00004000									//补张
//小胡
#define CHR_XIAO_DA_SI_XI					0x00004000									//大四喜
#define CHR_XIAO_BAN_BAN_HU					0x00008000									//板板胡
#define CHR_XIAO_QUE_YI_SE					0x00010000									//缺一色
#define CHR_XIAO_LIU_LIU_SHUN				0x00020000									//六六顺


#define CHR_ZI_MO							0x01000000									//自摸
#define	WIK_ZI_MO							CHR_ZI_MO
#define CHR_SHU_FAN							0x02000000									//素翻
#define CHR_TIAN_HU							0x04000000									//天胡

#define SUB_S_ANDROID_RULE         200                                 //发送规则给机器人

//游戏状态
struct CMD_S_StatusFree
{
	DWORD							lCellScore;							//基础金币
	WORD							wBankerUser;						//庄家用户
	bool							bTrustee[GAME_PLAYER];				//是否托管
};

//游戏状态
struct CMD_S_StatusPlay
{
	//游戏变量
	DWORD							lCellScore;									//单元积分
	WORD							wBankerUser;								//庄家用户
	WORD							wCurrentUser;								//当前用户
	WORD							wReplaceUser;								//花牌替换用户
	BYTE							cbQuanFeng;									//圈风
	BYTE							cbChangeCard;								//视图财神
	BYTE							cbLandCount;								//庄家次数
	WORD							wCaiShenIndex;								//财神位置
	WORD							wCaiShenChairID;							//财神位置
	BYTE							cbLeftCardCount;							//剩余数目
	//状态变量
	BYTE							cbActionCard;								//动作扑克
	BYTE							cbActionMask;								//动作掩码
	BYTE							cbHearStatus[GAME_PLAYER];					//听牌状态
	bool							bTrustee[GAME_PLAYER];						//是否托管
	WORD							wWinOrder[GAME_PLAYER];						//

	//出牌信息
	WORD							wOutCardUser;								//出牌用户
	BYTE							cbOutCardData;								//出牌扑克
	BYTE							cbDiscardCount[GAME_PLAYER];				//丢弃数目
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//丢弃记录

	//扑克数据
	BYTE							cbCardCount;								//扑克数目
	BYTE							cbCardData[MAX_COUNT];						//扑克列表
	BYTE							cbSendCardData;								//发送扑克

	//组合扑克
	BYTE							cbWeaveCount[GAME_PLAYER];					//组合数目
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//组合扑克

	//堆立信息
	WORD							wHeapHead;									//堆立头部
	WORD							wHeapTail;									//堆立尾部
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//堆牌信息
};

//游戏开始
struct CMD_S_GameStart
{
	DWORD							lSiceCount;									//骰子点数
	WORD							wFirstUser;									//首骰用户
	WORD							wLandSice;									//庄家骰子
	WORD							wCaiShenSice;								//财神点数
	WORD							wCaiShenIndex;								//财神位置
	WORD							wCaiShenChairID;							//财神位置
	WORD							wBankerUser;								//庄家用户
	WORD							wCurrentUser;								//当前用户
	WORD							wReplaceUser;								//补牌用户
	BYTE							cbUserAction;								//用户动作
	BYTE							cbCardData[MAX_COUNT];						//扑克列表
	BYTE							cbQuanFeng;									//圈风
	WORD							wHeapHead;									//堆立牌头
	WORD							wHeapTail;									//堆立牌尾
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//堆立信息
	BYTE							cbChangeCard;								//视图财神
	BYTE							cbLandCount;								//庄家次数
	BYTE							cbLeftCardCount;							//剩余数目、、
	BYTE							cbLessHu;									//几片起胡（1片OR两片）
};

//出牌命令
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
};

//发送扑克
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//扑克数据
	BYTE							cbActionMask;						//动作掩码
	WORD							wCurrentUser;						//当前用户
	WORD							wReplaceUser;						//补牌用户
	BYTE							cbSendCardUser;						//发牌用户
	BYTE							cbGangCardIndeed;					//实际杠牌
	bool							bTail;								//末尾发牌
};

//杠分
struct CMD_S_GangScore
{
	WORD							wChairId;							//
	BYTE							cbXiaYu;							//
	DWORD						lGangScore[GAME_PLAYER];			//
};


//操作提示
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//还原用户
	BYTE							cbActionMask;						//动作掩码
	BYTE							cbActionCard;						//动作扑克
	BYTE							cbGangCardData[4];
	BYTE							cbGangCardCount;
	BYTE cbCardEyeCount;	//游牌等级			0没有1单游2双游3三游
};

//操作命令
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//操作用户
	WORD							wProvideUser;						//供应用户
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard[3];					//操作扑克
};

//游戏结束
struct CMD_S_GameEnd
{
	DWORD							lGameTax;							//游戏税收
	//结束信息
	WORD							wProvideUser[GAME_PLAYER];						//供应用户
	BYTE							cbProvideCard;						//供应扑克
	DWORD							dwChiHuKind[GAME_PLAYER];			//胡牌类型
//	DWORD							dwChiHuRight[MAX_RIGHT_COUNT];		//胡牌类型
	DWORD							dwChiHuRight1[MAX_RIGHT_COUNT];		//胡
	DWORD							dwChiHuRight2[MAX_RIGHT_COUNT];		//胡
	BYTE							cbHuaCardCount;						//花牌个数
	BYTE							cbFanCount;							//总番数

	//罚张信息
	BYTE							cbFollowCard[GAME_PLAYER][20];		//被跟扑克
	BYTE							cbFollowCount[GAME_PLAYER];			//被跟数目
	DWORD							lOutMagicScore[GAME_PLAYER];		//误打积分
	DWORD							lFollowScore[GAME_PLAYER];			//被跟积分
	DWORD							lHuScore[GAME_PLAYER];				//胡牌积分
	DWORD							lFanHuScore[GAME_PLAYER];			//放胡积分

	//积分信息
	bool							bSecondMagic;
	int								lGameScore[GAME_PLAYER];			//游戏积分
	
	DWORD							dwStartHuRight[GAME_PLAYER];		//XX起胡
	
	//扑克信息
	BYTE							cbCardCount[GAME_PLAYER];			//扑克数目
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//扑克数据
	BYTE							cbChiHuCardData[GAME_PLAYER][MAX_WEAVE];

	bool                            bHasCSGang[GAME_PLAYER];
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//组合扑克
	BYTE							cbWeaveCount[GAME_PLAYER];
	DWORD							lStartHuScore[GAME_PLAYER];
	int								lGangScore[GAME_PLAYER];
	BYTE							cbWinerMakeEye;					//赢家游金
};

//用户托管
struct CMD_S_Trustee
{
	bool							bTrustee;							//是否托管
	WORD							wChairID;							//托管用户
};

//用户听牌
struct CMD_S_Listen
{
	WORD							wChairId;							//听牌用户
};

//补牌命令
struct CMD_S_ReplaceCard
{
	WORD							wReplaceUser;						//补牌用户
	BYTE							cbReplaceCard;						//补牌扑克
};
//用户托管

//
struct CMD_S_ChiHu
{
	WORD							wChiHuUser;							//
	WORD							wProviderUser;						//
	BYTE							cbChiHuCard;						//
	BYTE							cbCardCount;						//
	DWORD						lGameScore;							//
	BYTE							cbWinOrder;							//
};

struct CMD_S_AndroidRule
{
	BYTE							cbGameTypeIdex;	//游戏类型
	DWORD							dwGameRuleIdex;	//游戏规则
	BYTE							cbMagicIndex;	//财神牌
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CARD				1									//出牌命令
#define SUB_C_OPERATE_CARD			3									//操作扑克
#define SUB_C_TRUSTEE				4									//用户托管
#define SUB_C_LISTEN				2									//用户听牌
#define SUB_C_REPLACE_CARD			5									//用户补牌
#define SUB_C_MASTER_LEFTCARD		6									//剩余牌堆
#define SUB_C_MASTER_CHEAKCARD		7									//选择的牌
#define SUB_C_MAKE_CARDEYE			122									//游牌
//出牌命令
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//扑克数据
};

//操作命令
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard[3];					//操作扑克
};

//用户听牌
struct CMD_C_Listen
{
	BYTE							cbListen;							//听牌用户
};

//用户托管
struct CMD_C_Trustee
{
	bool							bTrustee;							//是否托管	
};

//补牌命令
struct CMD_C_ReplaceCard
{
	BYTE							cbCardData;							//扑克数据
};

struct MaterCheckCard
{
	BYTE							cbCheakCard;						//操作代码
};

struct MaterNiaoCout
{
	BYTE							cbNiaoCout;							//操作代码
};

struct MasterLeftCard
{
	BYTE      kMasterLeftIndex[MAX_INDEX];
	BYTE      kMasterCheakCard;
};
#pragma pack()
#endif
