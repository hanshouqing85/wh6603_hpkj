#pragma once

#define MAX_FISH_TYPE		20
#define AL_PRECISION	100

struct SPlayer
{
	DWORD dwPlayerID;//玩家ID
	__int64 playerNowScore;//玩家目前分数
	__int64 playerNowPutScore;//玩家当次上分数
	__int64 playerPutScore;//玩家总上分值
	__int64 playerGetScore;//玩家总下分值
	int playerShootingTime;//总射击次数
	__int64 playerMaxScore;//分数最大值
	int playerDeadTime;//分数归零次数
	int playerGunScore;//玩家每炮分数
	int playerRegulation;//手动操控影响值(取值范围-4到2，取值越大，越容易打到鱼)
	bool bControl;
};




struct SMonster
{
	int MonID;//怪物ID
	int MonMul;//怪物倍率
	int RT;//每行数字个数（总数）
	int FirstNT;//首行数字个数（倍率出现数）
	int SecondNT;//次行数字个数（倍率出现数）
	int ThirdNT;//末行数字个数（倍率出现数）
	double Influence;//期望值
	double MLevel;//怪物等级（1到5）
	double InfluenceSmall;
	double MlevelSmall;
	//五项阶数跨度
	double level1;
	double level2;
	double level3;
	double level4;
	double level5;
};

//房间数据
struct SRoomData
{
	int		roomMaxCost;
	int		onceUpScore;
	double	factor;//总体难度系数（取值范围 0.1到2.0，值越大越容易打）
	int		level;//总体难度标准阶数（取值范围 1到5，值越大越难打）

	double	factorsmall;
	int		levelsmall;
};

//一个桌子的数据
struct TableData
{
	__int64 totalUpScore;//总上分
	__int64 totalDownScore;//总下分
	__int64 totalNowScore;//总盈利
	__int64 totalPlayUseScore;//总玩家消耗
};

#define MTRandom(min,max) ((((rand()+100000000)%12345678)%(max-min+1))+min)

SMonster* GetMonster( SMonster* monsterteam, 
					 int type);

void SPMonster(SMonster* monster_out, 
			   int ratio,
			   const SRoomData* difficult,
			   bool bAndrior);

int PlayerLevel(const SPlayer* player, 
				const SMonster* monster, 
				int MaxGunScore,
				 bool bAndrior);//计算玩家level

typedef int (*LpPlayerLevel)(const SPlayer* player,
							 const SMonster* monster,
							 int MaxGunScore,
							 bool bAndrior);

//计算结果
__int64 RMSystem(int bulletCost,
			 int MaxGunScore,//当前房间的最大允许每炮分数
			 int nPlayerRegulation, // 不使用SPlayer中的了,专门传递参数
			  bool bAndrior,
			 const SMonster* monster,
			 const SPlayer* player,
			 LpPlayerLevel playerLevel
			 );

void PlayerRegulation(SPlayer* player, 
					  int value);//手动影响值


//////////////////////////////////////////////////////////////////////////



