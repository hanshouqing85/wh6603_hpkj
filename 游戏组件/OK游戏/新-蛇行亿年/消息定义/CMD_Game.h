/*****************************************************/
//本游戏开发者:哈皮科技,联系QQ:56248
/*****************************************************/

#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#pragma pack(1)
/////////////////////////////////////////////////////////////////////////////////
//服务定义
//游戏属性
#define KIND_ID						2013 								//游戏 I D
#define GAME_NAME					TEXT("蛇行亿年")					//游戏名字
#define	FISHCOUNT					32									//鱼总数

//组件属性
#define GAME_PLAYER					6									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_CALL				GAME_STATUS_PLAY					//叫分状态
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//游戏进行

//命令定义
enum enServerSubCmd
{
	enServerSubCmd_Begin=59,
	SUB_S_GAME_SCENE,									//场景消息
	SUB_S_TRACE_POINT,									//轨迹坐标
	SUB_S_USER_SHOOT,									//发射炮弹
	SUB_S_CAPTURE_FISH,									//捕获鱼群
	SUB_S_BULLET_COUNT,									//子弹数目
	SUB_S_COIN_COUNT,									//金币数目
	SUB_S_FISH_COUNT,									//鱼群数目
	SUB_S_CHANGE_SCENE,									//切换场景
	SUB_S_SCORE_INFO,									//分数信息
	SUB_S_SET_CELL_SCORE,								//单元积分
	SUB_S_BONUS_INFO,									//彩金信息
	SUB_S_GAME_MESSAGE,									//游戏消息
    SUB_S_LOCKFISH,
	SUB_S_ANDROID_CELL,									//机器人CellScore

	enServerSubCmd_End
};
//初始轨迹点和角度
struct FishTrace
{
     float x;    //鱼儿X坐标
	 float y;    //Y坐标
	 float rotation;//当前角度
	 float movetime;//移动时间保持这个轨迹运动的时间
	 float changetime;//改变时间，从之前的角度改变到现在角度规定在这个时间内完成
	 bool  m_isHave ;//坐标是否过期，默认为过期就是这个坐标默认不存在
	 int   m_fishid;//这个坐标对应的FISH的ID号
	 int   m_fishtype;//这个坐标对应的FISH的ID号
	 int   m_ptindex;
	 int   m_BuildTime;
	 int   m_shootCount;
	 int   m_mustShoot;
 	 int   m_fudaifishtype;//类型为21和22的鱼可以附带小鱼
	 int   m_speed;


};
//
struct FishPack
{
     int   m_fishid;        //鱼ID
	 int   m_fishtype;      //鱼类型
	 int   m_fishpathindex; //鱼轨迹
	 int   m_fudaifishtype; //鱼是否圈为-1表示不带
	 int   m_BuildTime;
	 int   m_CreatDelayTime;
	 int   m_SameFishID;
};

//消息数据包
struct CMD_S_GameMessage
{
	WORD								wMessageLength;					//消息长度
	TCHAR								szContent[1024];				//消息内容
};

//倍率信息
struct CMD_S_BonusInfo
{
	WORD							wChairID;								//设置玩家
	LONG							lBonus;									//彩金数目
};

//单元积分
struct CMD_S_SetCellScore
{
	WORD							wChairID;								//设置玩家
	LONG							lCellScore;								//单元积分
};

//切换场景
struct CMD_S_ChangeScene
{
	WORD     					    SceneIndex;						        //当前场景
	WORD                            RmoveID;                                //预留这里

};

//子弹数目
struct CMD_S_BulletCount
{
	WORD							wChairID;								//玩家椅子
	bool    				        isaddorremove;						    //子弹类型
	LONG							score;							//子弹数目
};
//子弹数目
struct CMD_S_AndroidCellScore
{
	LONG							score;							//机器人基本分
};

//鱼群数目
struct CMD_S_CptrFishCount
{
	int    						    FishKind;								//鱼群类型
	WORD							wFishCount;								//鱼群数目
};

//金币数目
struct CMD_S_CoinCount
{
	WORD							wChairID;								//玩家椅子
	LONG							lCoinCount;								//金币数目
};

//场景消息
struct CMD_S_GameScene
{
	LONG							lBulletCharge;	//子弹费用
	int     				   	    CureSeaSceneKind;						//当前场景
	LONG							m_lUserAllScore[6];			    //玩家分数
	LONG							lUserCellScore[6];			//单元炮的倍数字
	float                           powercount[6];
};

//游戏状态
struct CMD_S_FishTrace
{
	int							    bRegular;								//规则标识
	FishPack                        m_fishtrace;
};


//发射炮弹
struct CMD_S_UserShoot
{
	WORD							wChairID;								//玩家椅子
	float							fAngle;									//发射角度
	int 				            BulletCountKind;						//子弹类型
	bool							byShootCount;							//子弹数目	
	int                             dwUserScore;
	int                             dwZidanID;
	float                           powercount;
	bool                            superpao;
};
//单元积分
struct CMD_S_LockFishId
{
	LONG							lFishID;								//单元积分
	int                             iChairID;
};
//捕获鱼群
struct CMD_S_CaptureFish
{
	WORD							wChairID;								//玩家椅子
	DWORD							dwFishID;								//鱼群标识
	int    						    FishKindscore;								//鱼群种类
	LONG							lFishScore;								//鱼群得分
	bool                            m_canSuperPao;                          //超级炮
	int                             dwUserScore;
};
//////////////////////////////////////////////////////////////////////////
//命令定义

enum enClientSubCmd
{
	enClientSubCmd_Begin=60,
	SUB_C_BUY_BULLETSPEED,								//购买子弹
	SUB_C_BUY_BULLET,									//购买子弹
	SUB_C_USER_SHOOT,									//发射炮弹
	SUB_C_CAPTURE_FISH,									//捕获鱼群
	SUB_C_GET_SCORE_INFO,								//分数信息
	SUB_C_CALC_SCORE,									//计算分数
	SUB_C_SET_CELL_SCORE,								//单元积分
	SUB_C_HIT_FISH,										//捕获鱼群
	SUB_C_SET_PROPABILITY,								//设置概率
    SUB_C_SET_COMEINUSER,								//设置概率
	SUB_C_LOCKFISH,                                     //锁定捕鱼
	enClientSubCmd_End
};
//单元积分
struct CMD_C_LockFishId
{
	LONG							lFishID;								//单元积分
	LONG                            lChairID;
};
//单元积分
struct CMD_C_SetCellScore
{
	LONG							lCellScore;								//单元积分
};

//购买子弹
struct CMD_C_BuyBullet
{
	int  				           score;//上下分数						//数目种类
	bool                           addormove;//上或者是下
	bool						   allscore;
};

//发射炮弹
struct CMD_C_UserShoot
{
	int    				            BulletCountKind;						//子弹类型
	bool							byShootCount;							//射击数目
	float							fAngle;									//发射角度
	DWORD							dwBulletID;								//子弹标识

};

//击中鱼群
struct CMD_C_HitFish
{
	DWORD							dwFishID;								//鱼群标识
	DWORD							dwBulletID;								//子弹标识
	int                             bulletuserid;
	bool                            boolisandroid;

};
struct CMD_C_CoinCount
{
//	WORD							wChairID;								//玩家椅子
	LONG							lCoinCount;								//金币数目
};
//设置概率
struct CMD_C_SetProbability
{
	BYTE							byCptrProbability;	//捕获概率
};
//
struct CM_C_ComeInNow
{
    LONG                           lHaveCome  ;//
};

//////////////////////////////////////////////////////////////////////////

#endif