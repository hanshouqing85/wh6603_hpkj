
#ifndef CMD_FISH_H_
#define CMD_FISH_H_

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
// 服务定义

#define KIND_ID               715
#define GAME_NAME             TEXT("真·摇钱树")
#define GAME_PLAYER           8
#define VERSION_SERVER        101056515       // PROCESS_VERSION(1,0,0)
#define VERSION_CLIENT        101056515       // PROCESS_VERSION(1,0,0)


// 简单的版本检测
// 如果客户端有更新应该更改这个值.如果只修改EXE的版本而网络数据包没修改的话，更新客户端是没什么意义的，因为EXE的版本是可以直接被修改的
// 只要保存以前的EXE版本,更新完之后再换回去，如果有BUG的话等于还是没修复。
#define GAME_VERSION          30

#ifndef SCORE
#define SCORE LONGLONG
#endif

// add by hxh 20161227
#define FISHSCORE LONGLONG
//#define USE_TRACETIME
typedef DWORD TFishMoveSecs;//鱼轨迹时长，也就是鱼的最长存活时间
typedef DWORD TBuildTraceTime;//服务器端轨迹建立时间ms

const int kResolutionWidth = 1366;
const int kResolutionHeight = 768;

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#define M_PI_2  1.57079632679489661923f
#define M_PI_4  0.785398163397448309616f
#define M_1_PI  0.318309886183790671538f
#define M_2_PI  0.636619772367581343076f
#endif
const float kChairDefaultAngle[GAME_PLAYER] = { M_PI, M_PI, M_PI, -M_PI_2, 0, 0, 0, M_PI_2 };

enum TraceType {
  TRACE_LINEAR = 0,
  TRACE_BEZIER
};

struct FPoint {
  float x;
  float y;
};

struct FPointAngle {
  float x;
  float y;
  float angle;
};

//////////////////////////////////////////////////////////////////////////
// 游戏定义

/*
// 座位号
-------------
    0   1   2 
7               3                
    6   5   4
-------------
*/

enum SceneKind {
  SCENE_KIND_1 = 0,
  SCENE_KIND_2,
  SCENE_KIND_3,
  SCENE_KIND_4,
  SCENE_KIND_5,

  SCENE_KIND_COUNT
};

enum FishKind 
{
	//小鱼
	FISH_KIND_1 = 0,
	FISH_KIND_2,
	FISH_KIND_3,
	FISH_KIND_4,
	FISH_KIND_5,
	FISH_KIND_6,
	FISH_KIND_7,
	FISH_KIND_8,
	FISH_KIND_9,
	FISH_KIND_10,

	//中鱼
	FISH_KIND_11,
	FISH_KIND_12,
	FISH_KIND_13,
	FISH_KIND_14,
	FISH_KIND_15,
	FISH_KIND_16,
	FISH_KIND_17,

	FISH_KIND_18,		//黄金鲨鱼
	FISH_KIND_19,		//金龙
	FISH_KIND_20,		//金龟
	FISH_KIND_21,		//金刀;炸弹
	FISH_KIND_22,		//黑色龟；定海神针
	FISH_KIND_23,		//绿色龟；超级炸弹
	FISH_KIND_24,		//真摇钱树
	FISH_KIND_25,		//大三元1
	FISH_KIND_26,		//大三元2
	FISH_KIND_27,		//大三元3
	FISH_KIND_28,		//大四喜1
	FISH_KIND_29,		//大四喜2
	FISH_KIND_30,		//大四喜3

	//鱼王
	FISH_KIND_31,		//鱼王1
	FISH_KIND_32,		//鱼王2
	FISH_KIND_33,		//鱼王3
	FISH_KIND_34,		//鱼王4
	FISH_KIND_35,		//鱼王5
	FISH_KIND_36,		//鱼王6
	FISH_KIND_37,		//鱼王7
	FISH_KIND_38,		//鱼王8
	FISH_KIND_39,		//鱼王9
	FISH_KIND_40,		//鱼王10

	FISH_KIND_COUNT
};

enum BulletKind 
{
  BULLET_KIND_1_NORMAL = 0,
  BULLET_KIND_2_NORMAL,
  BULLET_KIND_3_NORMAL,
  BULLET_KIND_4_NORMAL,
  BULLET_KIND_1_ION,
  BULLET_KIND_2_ION,
  BULLET_KIND_3_ION,
  BULLET_KIND_4_ION,

  BULLET_KIND_COUNT
};

enum CreateFishKind
{
	CREATE_KIND_0,
	CREATE_KIND_1,
	CREATE_KIND_2,
	CREATE_KIND_3,
	CREATE_KIND_4,
	CREATE_KIND_5,
	CREATE_KIND_6,
	CREATE_KIND_7,
	CREATE_KIND_8,
	CREATE_KIND_9,
	CREATE_KIND_10,
	CREATE_KIND_11,

	CREATE_KIND_COUNT,
};

const DWORD kBulletIonTime = 20;
const DWORD kLockTime = 10;

const int kMaxCatchFishCount = 2;

// 限制客户端的鱼种类数量
//const int kFishKindCount[FISH_KIND_COUNT] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

//////////////////////////////////////////////////////////////////////////
// 服务端命令

#define SUB_S_GAME_CONFIG                   100
#define SUB_S_FISH_TRACE                    101
#define SUB_S_EXCHANGE_FISHSCORE            102
#define SUB_S_USER_FIRE                     103
#define SUB_S_CATCH_FISH                    104
#define SUB_S_BULLET_ION_TIMEOUT            105
#define SUB_S_LOCK_TIMEOUT                  106
#define SUB_S_CATCH_SWEEP_FISH              107
#define SUB_S_CATCH_SWEEP_FISH_RESULT       108
#define SUB_S_HIT_FISH_LK                    109
#define SUB_S_SWITCH_SCENE                  110
#define SUB_S_STOCK_OPERATE_RESULT          111
#define SUB_S_SCENE_END                     112
//发炮由服务器产生，而不是机器人
#define SUB_S_SHOT							113



struct CMD_S_GameStatus {
  DWORD game_version;
  FISHSCORE fish_score[GAME_PLAYER];
  FISHSCORE exchange_fish_score[GAME_PLAYER];
};

struct CMD_S_GameConfig {
  int exchange_ratio_userscore;
  int exchange_ratio_fishscore;
  int exchange_count;

  int min_bullet_multiple;
  int max_bullet_multiple;

  int bomb_range_width;
  int bomb_range_height;

  int fish_multiple[FISH_KIND_COUNT];
  int fish_speed[FISH_KIND_COUNT];
  int fish_bounding_box_width[FISH_KIND_COUNT];
  int fish_bounding_box_height[FISH_KIND_COUNT];
  int fish_hit_radius[FISH_KIND_COUNT];

  int bullet_speed[BULLET_KIND_COUNT];
  int net_radius[BULLET_KIND_COUNT];
};

struct CMD_S_FishTrace {
  FPoint init_pos[5];
  int init_count;
  FishKind fish_kind;
  int fish_id;
  TraceType trace_type;
#ifdef USE_TRACETIME
  TFishMoveSecs FishMoveSecs;
  TBuildTraceTime BuildTraceTime;
#endif
};

struct CMD_S_ExchangeFishScore {
  WORD chair_id;
  FISHSCORE swap_fish_score;
  FISHSCORE exchange_fish_score;
};

struct CMD_S_UserFire {
  BulletKind bullet_kind;
  int bullet_id;
  WORD chair_id;
  WORD android_chairid;
  float angle;
  int bullet_mulriple;
  int lock_fishid;
  FISHSCORE fish_score;
};

struct CMD_S_CatchFish {
  WORD chair_id;
  int fish_id;
  FishKind fish_kind;
  bool bullet_ion;
  FISHSCORE fish_score;
};

struct CMD_S_BulletIonTimeout {
  WORD chair_id;
};

struct CMD_S_CatchSweepFish {
  WORD chair_id;
  int fish_id;
};

struct CMD_S_CatchSweepFishResult {
  WORD chair_id;
  int fish_id;
  FISHSCORE fish_score;
  int catch_fish_count;
  int catch_fish_id[300];
};

struct CMD_S_HitFishLK {
  WORD chair_id;
  int fish_id;
  int fish_mulriple;
};

struct CMD_S_SwitchScene {
  SceneKind scene_kind;
  int fish_count;
  FishKind fish_kind[300];
  int fish_id[300];
};

struct CMD_S_StockOperateResult {
  unsigned char operate_code;
  FISHSCORE stock_score;
};

//////////////////////////////////////////////////////////////////////////
// 客户端命令

#define SUB_C_EXCHANGE_FISHSCORE            1
#define SUB_C_USER_FIRE                     2
#define SUB_C_CATCH_FISH                    3
#define SUB_C_CATCH_SWEEP_FISH              4
#define SUB_C_HIT_FISH_I                    5
#define SUB_C_STOCK_OPERATE                 6
#define SUB_C_USER_FILTER                   7
#define SUB_C_ANDROID_STAND_UP              8
#define SUB_C_FISH20_CONFIG                 9
#define SUB_C_ANDROID_BULLET_MUL            10
#define SUB_C_ANDROID_LEAVE					11		//机器人离开
#define SUB_C_TIME_STAMP					15		//时间戳<by hxh>

/* 
时间戳
单向传输时间T=(C2-C1+S1-S2)/2
时间差dT=C1-S1
例如，服务器端轨迹建立时间BuildTraceTime换算成客户端的时间戳是BuildTraceTime+dT
*/
struct CMD_C_TimeStamp
{
	CMD_C_TimeStamp()
	{
		memset(this, 0, sizeof(*this));
	}
     
	unsigned long long tClientSendTime;				//客户端发送时刻C1,客户端发送请求包时填写
	DWORD tServerRecvTime;							//服务器接收时刻S1
	DWORD tServerSendTime;							//服务器发送时刻S2
    unsigned long long tClientRecvTime;				//客户端接收时刻C2,客户端收到返回包时填写
	// 计算出来的常量
    long long tTransTime;//常量T 
    long long tClientServerTimeInterval;//常量dT
};

struct CMD_C_ExchangeFishScore {
  int increase;
};

struct CMD_C_UserFire {
  BulletKind bullet_kind;
  float angle;
  int bullet_mulriple;
  int lock_fishid;
};

struct CMD_C_CatchFish {
  WORD chair_id;
  int fish_id;
  BulletKind bullet_kind;
  int bullet_id;
  int bullet_mulriple;
};

struct CMD_C_CatchSweepFish {
  WORD chair_id;
  int fish_id;
  int catch_fish_count;
  int catch_fish_id[300];
};

struct CMD_C_HitFishLK {
  int fish_id;
};

struct CMD_C_StockOperate {
  unsigned char operate_code; // 0查询 1 清除 2 增加 3 查询抽水
};

struct CMD_C_UserFilter {
  DWORD game_id;
  unsigned char operate_code; // 0 黑名单 1 白名单 2 清除
};

struct CMD_C_Fish20Config {
  DWORD game_id;
  int catch_count;
  double catch_probability;
};

struct CMD_C_AndroidBulletMul {
  WORD chair_id;
  int bullet_id;
  int bullet_mulriple;
  BulletKind bullet_kind;
};

#pragma pack()

#endif // CMD_FISH_H_