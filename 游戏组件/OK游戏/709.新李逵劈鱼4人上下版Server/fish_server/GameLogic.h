#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "YJAlgorithm.h"

#define MAX_ERROR			5000//发送MAX_ERROR次错误则踢玩家下线

#define	MAX_ENERGY			10
#define ENERGY_TIME			20

#define DEFFALUT_ROOM_MAX_COST	1000
#define DEFFALUT_UP_SCORE	10000//一次上分数值

#define DEFFALUT_FACTOR		1.0f
#define DEFFALUT_LEVEL		3	

#ifdef _DEBUG
#define ONE_TABLE_DELAY		0.1f
#else
#define ONE_TABLE_DELAY		3.0f	//单个桌子更新时间 = MaxTable*ONE_TABLE_DELAY
#endif // _DEBUG


#define NAME_LEN	32

#define HIT_COUNT_DELAY		3

#define ENERGY_ADD_BY_HIT_STEP	5

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//数据库里需要保存的玩家信息
struct SDataBasePlayerData
{
	__int64 nowScore;//玩家目前分数
	__int64 nowPutScore;//玩家当次上分数
	__int64 putScore;//玩家总上分值
	__int64 getScore;//玩家总下分值
	int shootingTime;//总射击次数
	__int64 maxScore;//分数最大值
	int deadTime;//分数归零次数
	int gunScore;//玩家每炮分数
	__int64 lostScore;//玩家损失分数
	int playerRegulation;//手动操控影响值
	TCHAR name[32];//玩家名字
	DWORD IP;//玩家IP
	int maxHitCount;//玩家最大连击数
	bool bControl;
};

//数据库回调
interface IDataBaseSkin
{
	//获得用户数据
	virtual bool OnGetUserData(DWORD dwUserID, WORD wChairID, SDataBasePlayerData * pPlayer, bool bSuccess)=NULL;
	//更新用户数据
	virtual bool OnUpdateUserData(DWORD dwUserID, WORD wChairID, int nPlayerRegulation,bool bControl, bool bSuccess)=NULL;

	//更新用户数据
	virtual bool OnUpdateOnLineTime(DWORD dwUserID, WORD wChairID, int nAllStep, int nCurStep, int nTotalTime)=NULL;

	//更新用户金币
	virtual bool OnUpdateUserCoin(DWORD dwUserID, WORD wChairID, int nStep)=NULL;
};

//数据库接口
interface IGameManagerDataBase
{
	//读取房间数据
	virtual bool GetRoomData()=NULL;
	//确认领取奖励
	virtual bool ConfirmAward(DWORD dwUserID,  WORD wChairID, IDataBaseSkin* pSkin) = NULL;
	virtual bool GetOnLineTime(DWORD dwUserID, WORD wChairID, IDataBaseSkin* pSkin)=NULL;
	virtual bool RecordLoginTime(DWORD dwUserID, int nRecord)=NULL;
	//实物奖券写入数据库
	virtual bool WriteEntityDate(DWORD dwUserID, int nTicketCount, int nIpadCount, int nMp3Count, int nWhiteCount, int nZippoCount) = NULL;
	//保存用户数据
	virtual bool SaveUserData(DWORD dwUserID, const SDataBasePlayerData * pPlayer)=NULL;
	//读取用户数据
	virtual bool GetUserData(DWORD dwUserID, WORD wChairID, int nExchangeUserRatio, int nExchangeFishRatio, IDataBaseSkin* pSkin)=NULL;
	//更新用户数据
	virtual bool UpdateUserData(DWORD dwUserID, WORD wChairID, int nExchangeUserRatio, int nExchangeFishRatio, SDataBasePlayerData * pPlayer, IDataBaseSkin* pSkin)=NULL;
};
//////////////////////////////////////////////////////////////////////////
class TableFrameSink;

struct SGamePlayer
{
	SPlayer		alData;
	DWORD		uid;
	BYTE		cannonType;
	bool		serverAcceptEnergy;
	int			errorCount;
	float		energyTimer;
	bool		bActive;
	bool		isAI;
	__int64		lostScore;
	DWORD		IP;
	TCHAR		szName[NAME_LEN];
	SCORE		lscore;//总积分
	SCORE		lscoreLogin;//登陆时的总积分
	int			energy;
	int			hitCount;
	int			maxHitCount;
	float		lastHitTime;
	int			onLineTime;
	int         nGetStatus;
};

struct DBR_PlayerData
{
	DWORD   dwDataBaseSkin;
	DWORD	dwUserID;
	WORD	wTableID;
	WORD	wChairID;	
	int		nExchangeUserRatio;
	int		nExchangeFishRatio;
};

struct DBR_SavePlayer
{
	DWORD   dwDataBaseSkin;
	DWORD	dwUserID;
	WORD	wChairID;
	int		nExchangeUserRatio;
	int		nExchangeFishRatio;
	SDataBasePlayerData PlayerData;
};

struct DBR_OnLineTime
{
	DWORD   dwDataBaseSkin;
	DWORD	dwUserID;
	WORD	wChairID;
};

struct DBR_LoginTime
{
	DWORD	dwUserID;
	int 	nRecord;
};

struct DBR_ConfirmAward
{
	DWORD   dwDataBaseSkin;
	DWORD	dwUserID;
	WORD	wChairID;
};

struct DBR_EntityDate
{
	DWORD	dwUserID;
	int		nTicketCount;
	int		nIpadCount;
	int		nMp3Count;
	int		nWhiteCount;
	int		nZippoCount;
};

//游戏逻辑
class CGameLogic : public IDataBaseSkin
{
public:
	//static		char		dataBasePath[MAX_PATH];

private:
	static		IGameManagerDataBase* pIGameManager;
	static		SRoomData	roomData;
	static		char		roomName[256];
	//变量定义
public:
	SGamePlayer	player[GAME_PLAYER];
	bool bfresh_[GAME_PLAYER];
protected:
	float		saveDataTimer;
	float		saveTimeDelay;

	
	int			realUserCount;
	int			totalUserCount;

	int			m_iOnlinetime;

	TableFrameSink * pTable;

	//CsLock		csSaveLock;
	//CsLock		csReadLock;
	//CsLock		csTickLock;

	//SQLiteOperate sql_;

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	void SetTable(TableFrameSink* table);
public:
	static void ReadRoomData(const SRoomData* roomData_);
	static void SetManager(IGameManagerDataBase* p){pIGameManager = p;};

	bool FishGoldByStyle(int chairID, int wCost, int nFishStyleOrRatio, bool autoRatio,bool bAndrior);

	int RandomTeamNumber()const;

	void Tick();

	int GetRatioByTpye(int type);

	//数据库接口
public:
	//用户数据
	virtual bool OnGetUserData(DWORD dwUserID, WORD wChairID, SDataBasePlayerData * pPlayer, bool bSuccess);
	//更新用户数据
	virtual bool OnUpdateUserData(DWORD dwUserID, WORD wChairID,  int nPlayerRegulation,bool bControl, bool bSuccess);
	//更新用户数据
	virtual bool OnUpdateOnLineTime(DWORD dwUserID, WORD wChairID, int nAllStep, int nCurStep, int nTotalTime);
	//更新用户金币
	virtual bool OnUpdateUserCoin(DWORD dwUserID, WORD wChairID, int nStep);
private:
	bool _WriteAllDataToDataBase();
	bool _ReadPlayerDataFromDatabase(DWORD uid, int chairID);
	bool _SavePlayerDataToDatabase(DWORD uid, int chairID);
	bool _ReadPlayerDataFromDatabase(DWORD uid, int chairID, IGameManagerDataBase* psql, bool* bAdd = NULL);
	bool _SavePlayerDataToDatabase(DWORD uid, int chairID, IGameManagerDataBase* psql);
	void _AcountScore(int chairID);//结算分数
	bool _FishGoldByRmSystemEx(int chairID, int ratio, int bulletCost,bool bAndrior);
	void _AddScoreByAlgorithm(int chairID, __int64 score);
	int _GetMinGunScore();
	void _dberror(const char* msg);
public:
	SGamePlayer* GetData(int chairID){return &player[chairID];}
	SPlayer* GetAlData(int chairID){return &player[chairID].alData;}

	//确认领取奖励
	void ConfirmAward(DWORD dwUserID,  WORD wChairID);
	void GetOnLineTime(DWORD dwUserID,  WORD wChairID);
	//记录登录时间
	void RecordLoginTime(DWORD dwUserID, int nRecord);
	//实物奖券写入数据库
	void WriteEntityDate(DWORD dwUserID, int nTicketCount, int nIpadCount, int nMp3Count, int nWhiteCount, int nZippoCount);
	// 提高难度
	void SetplayerRegulation(int chairID,int playerRegulation, bool bIncreaseDiffic=true);

	void InitChair(DWORD uid, DWORD IP, const TCHAR* name, int chairID, bool bAI, SCORE lscore);
	bool AccountChair(DWORD uid, int chairID/*, SQLiteOperate& sql*/);
	bool ClearChair(DWORD uid, int chairID/*, SQLiteOperate& sql*/);

	int GetBulletCost(int chairID)const {return player[chairID].alData.playerGunScore;}
	__int64 GetNowScore(int chairID)const {return player[chairID].alData.playerNowScore;}
	int GetFireCount(int chairID)const{return player[chairID].alData.playerShootingTime;}
	BYTE GetCannonType(int chairID)const{return player[chairID].cannonType;}
	int GetRoomMaxCost()const{return roomData.roomMaxCost;};
	static void SetRoomMaxCost(int nMaxCost){roomData.roomMaxCost = nMaxCost;}
	static void SetRoomOnceUpScore(int OnceUpScore){roomData.onceUpScore = OnceUpScore;}
	DWORD GetUID(int chairID)const{return player[chairID].uid;}
	int GetRoomOnceUpScore()const{return roomData.onceUpScore;};
	int GetRealUserCount()const{return realUserCount;}
	int GetTotalUserCount()const{return totalUserCount;}
	int GetRegulation(int chairID)const{return player[chairID].alData.playerRegulation;}
	__int64 GetLostScore(int chairID)const{return player[chairID].lostScore;}
	SCORE GetlScore(int chairID)const{return player[chairID].lscore;}
	SCORE GetAccountScore(int chairID){return player[chairID].lscore-player[chairID].lscoreLogin;}
	WORD GetHitCount(int chairID)const{return (WORD)player[chairID].hitCount;}

	bool MaxError(int chairID)const{return player[chairID].errorCount >= MAX_ERROR;}
	void ClearError(int chairID){player[chairID].errorCount = 0;}

	void AddScore(int chairID, __int64 score, bool upscore = false);//用于上下分以及子弹发射
	void AddLostScore(int chairID, __int64 score){player[chairID].lostScore += score;}
	void AddPlayerShoot(int chairID){player[chairID].alData.playerShootingTime++;}
	void AddError(int chairID){player[chairID].errorCount++;}
	void AddEnergy(int chairID, BYTE number = 1);

	void AddlScore(int chairID, SCORE value){player[chairID].lscore += value;}

	void SetlScore(int chairID, SCORE lscore){player[chairID].lscore = player[chairID].lscoreLogin = lscore;}
	bool SetBulletCost(int chairID, int cost){
		if(cost > roomData.roomMaxCost)return false;
		player[chairID].alData.playerGunScore = cost;
		return true;
	}
	bool SetCannonTpye(int chairID, BYTE type);
	void SetCannonTypeByServer(int chairID, BYTE type){player[chairID].cannonType = type;}

	bool IsChairHasUser(int chairID)const{return player[chairID].bActive;}
	bool IsChairAI(int chairID)const{return player[chairID].isAI;}
	bool IsEnergyGun(int chairID)const{return player[chairID].cannonType == 1;}
	bool HasNoneAIUser()const{return realUserCount!=0;}//是否存在非机器人用户

	void HitCount(int chairID, int count);
	virtual void LogError( const char* msg ){_dberror(msg);}
	void _UpdateRoomData();
};

inline int CGameLogic::_GetMinGunScore()
{
	if (roomData.roomMaxCost <= 99)
	{
		return 1;
	}
	if (roomData.roomMaxCost <= 1000)
	{
		return 10;
	}
	if (roomData.roomMaxCost <= 9900)
	{
		return 100;
	}
	if (roomData.roomMaxCost <= 99000)
	{
		return 100;
	}
	return 10;
}

inline int CGameLogic::RandomTeamNumber() const
{
	int n = rand()%10;
	switch(n)
	{
	case 1:
	case 2:
		return 7;
	case 3:
	case 4:
	case 5:
	case 6:
		return 4;
	default:
		return 3;
	}
}

//////////////////////////////////////////////////////////////////////////
inline void DBPD_To_SP(const SDataBasePlayerData* in, SPlayer* out)
{
	ASSERT(in);
	ASSERT(out);
	//out->playerNowScore = in->nowScore;
	//out->playerNowPutScore = in->nowPutScore;
	out->playerPutScore = in->putScore;
	out->playerGetScore = in->getScore;
	out->playerShootingTime = in->shootingTime;
	out->playerDeadTime = in->deadTime;
	out->playerGunScore = in->gunScore;
	out->playerRegulation = in->playerRegulation;
	out->playerMaxScore = in->maxScore;
	out->bControl=in->bControl;
}

inline void SP_To_DBPD(const SPlayer* in,  SDataBasePlayerData* out)
{
	ASSERT(in);
	ASSERT(out);
	out->nowScore = in->playerNowScore;
	out->nowPutScore = in->playerNowPutScore;
	out->putScore = in->playerPutScore;
	out->getScore = in->playerGetScore;
	out->shootingTime = in->playerShootingTime;
	out->deadTime = in->playerDeadTime;
	out->gunScore = in->playerGunScore;
	out->playerRegulation = in->playerRegulation;
	out->maxScore = in->playerMaxScore;
}

inline void CheckPlayerData(SPlayer* in_out)
{
	if (in_out->playerNowPutScore<0)
	{
		in_out->playerNowPutScore = 0;
	}
	if (in_out->playerPutScore<0)
	{
		in_out->playerPutScore = 0;
	}
	if (in_out->playerGetScore<0)
	{
		in_out->playerGetScore = 0;
	}
	if (in_out->playerShootingTime<0)
	{
		in_out->playerShootingTime = 0;
	}
	if (in_out->playerMaxScore<0)
	{
		in_out->playerMaxScore = 0;
	}
	if (in_out->playerDeadTime<0)
	{
		in_out->playerDeadTime = 0;
	}
	if (in_out->playerGunScore<0)
	{
		in_out->playerGunScore = 1;
	}
}

inline void CheckAndModifyPlayerData(SPlayer* out)
{
	__int64 maxValue = max(out->playerPutScore, out->playerGetScore);
	if (maxValue > 1000000000)
	{
		__int64 minValue = min(out->playerPutScore, out->playerGetScore);
		out->playerPutScore -= minValue;
		out->playerGetScore -= minValue;

		out->playerPutScore = max(0, out->playerPutScore);
		out->playerGetScore = max(0, out->playerGetScore);
	}
}

#endif