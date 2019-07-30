#include "StdAfx.h"
#include "GameLogic.h"

#include <assert.h>
#include <math.h>
#include <float.h>
#include <fstream>
#include <sstream>
#include "table_frame_sink.h"
#include <stdio.h>



#define Print(str)  
#define Warnning(str) 
#define LogError(str) 
//////////////////////////////////////////////////////////////////////////


extern double g_increase_factor[15];
extern double g_increase_factorSmall[15];
extern int   g_cur_increase; // 当前调整的级别 -1不调整


//////////////////////////////////////////////////////////////////////////
IGameManagerDataBase* CGameLogic::pIGameManager=NULL;
SRoomData CGameLogic::roomData;
char CGameLogic::roomName[256] = {0};

//构造函数
CGameLogic::CGameLogic():
pTable(NULL)
{
	memset(player,0,sizeof(player));
	for (int i=0;i<GAME_PLAYER;i++)
	{
		bfresh_[i]=true;
	}

	realUserCount = 0;
	totalUserCount = 0;
}

//析构函数
CGameLogic::~CGameLogic()
{
	
}

bool CGameLogic::FishGoldByStyle(int chairID, int wCost, int nFishStyleOrRatio, bool autoRatio,bool bAndrior)
{ 
	int ratio = nFishStyleOrRatio;
	if (IsEnergyGun(chairID))
	{
		ratio *= 2;
	}
	return _FishGoldByRmSystemEx(chairID, ratio, wCost,bAndrior);
}

bool CGameLogic::_FishGoldByRmSystemEx( int chairID, int ratio, int bulletCost ,bool bAndrior)
{
	SMonster monster;
	ZeroMemory(&monster, sizeof(monster));
	ratio = max(ratio, 2);
	ratio = min(ratio, 2000);
	int playerRegulation=player[chairID].alData.playerRegulation;


	//// 机器减低17%
	//if (bAndrior && (rand()%1000)<170)
	//{
	//	return false;
	//}

	//if(!player[chairID].alData.bControl)
	//{
	//	if(ratio<30)
	//	{
	//		int nrand=rand()%2;
	//		if(nrand==0)
	//			playerRegulation=-2;
	//		else
	//			playerRegulation=-1;
	//	}
	//}
	//if (!bfresh_[chairID])
	//{
	//	playerRegulation=-4;
	//}

	//int nCurIncrease = g_cur_increase;

 //  	if ((nCurIncrease < 0) || (nCurIncrease >= CountArray(g_increase_factor)))
	//{
	//	SPMonster(&monster, ratio,&roomData,bAndrior);
	//}
 //	else
 //	{
 //		SRoomData curRoomData;
 //		memcpy(&curRoomData, &roomData, sizeof(roomData));
	//	double adjust = g_increase_factor[nCurIncrease];
	//	double adjustSmal = g_increase_factorSmall[nCurIncrease];
	//	if (bAndrior)
	//	{
	//		// 调容易的时候
	//		if (adjust>0)
	//		{
	//			adjust = adjust*0.3;
	//		}
	//		if (adjustSmal>0)
	//		{
	//			adjustSmal = adjustSmal*0.3;
	//		}			
	//	}
 //		curRoomData.factor+=adjust;
 //		curRoomData.factorsmall+=adjustSmal;
 //		if (curRoomData.factor < 0.1f)
 //		{
 //			curRoomData.factor=0.1f;
 //		}
 //		else if (curRoomData.factor>2.0f)
 //		{
 //			curRoomData.factor=2.0f;
 //		}
 //
 //		if (curRoomData.factorsmall < 0.1f)
 //		{
 //			curRoomData.factorsmall=0.1f;
 //		}
 //		else if (curRoomData.factorsmall>2.0f)
 //		{
 //			curRoomData.factorsmall=2.0f;
 //		}
 //		SPMonster(&monster, ratio,&curRoomData,bAndrior);

 //	}

	//// PlayerRegulation, 不使用SPlayer中的了,专门传递参数, 这样不影响修改值
	//__int64 ret = RMSystem(bulletCost, roomData.roomMaxCost,playerRegulation, bAndrior, &monster, &player[chairID].alData, PlayerLevel);

	/*if(!bAndrior)
	{
		if(ratio<30)
		{
			player[chairID].alData.playerRegulation=0;
		}
	}
	else
	{
		if (ratio>=30)
		{
			player[chairID].alData.playerRegulation=-1;
		}
	}*/

	SPMonster(&monster, ratio ,&roomData,bAndrior);

	__int64 ret = RMSystem(bulletCost, roomData.roomMaxCost,player[chairID].alData.playerRegulation, bAndrior, &monster, &player[chairID].alData, PlayerLevel);

	CString strMsg;
	strMsg.Format(TEXT("[lkpy] playerRegulation: %d, ret: %I64d"), player[chairID].alData.playerRegulation, ret);
	OutputDebugString(strMsg);

	assert(ret>=0);
	_AddScoreByAlgorithm(chairID, ret);
	return ret>0?true:false;
}

void CGameLogic::Tick()
{
// 	float time = pTable->getTime();
// 	if (time - saveDataTimer > saveTimeDelay)
// 	{
		_WriteAllDataToDataBase();
// 		saveDataTimer = time;
// 	}
}

void CGameLogic::HitCount( int chairID, int count )
{
	float time = pTable->getTime();
	if (time - player[chairID].lastHitTime < HIT_COUNT_DELAY || count > 1)
	{
		int lasthit = player[chairID].hitCount;
		int remainder = lasthit%ENERGY_ADD_BY_HIT_STEP;
		int addEnergy = (count+remainder)/ENERGY_ADD_BY_HIT_STEP;
		player[chairID].hitCount += count;
		player[chairID].maxHitCount = max(player[chairID].maxHitCount, player[chairID].hitCount);
		if (addEnergy>0)
		{
			AddEnergy(chairID, addEnergy);
		}
	}
	else
	{
		player[chairID].hitCount = 0;
	}
	player[chairID].lastHitTime = time;
}


bool CGameLogic::_WriteAllDataToDataBase()
{
	for (int i=0;i<GAME_PLAYER;i++)
	{
		if(IsChairAI(i))
			continue;

		if (IsChairHasUser(i))
		{
			_SavePlayerDataToDatabase(GetUID(i), i);
		}
	}
	return true;
}

bool CGameLogic::_ReadPlayerDataFromDatabase( DWORD uid, int chairID )
{
	return _ReadPlayerDataFromDatabase(uid, chairID, pIGameManager);
}

bool CGameLogic::_ReadPlayerDataFromDatabase( DWORD uid, int chairID, IGameManagerDataBase* psql, bool* bAdd )
{
	int nUserRatio=1;
	int nFishRatio =1;

	pTable->GetExchangeRatio(nUserRatio, nFishRatio);
	return psql->GetUserData(uid, chairID, nUserRatio, nFishRatio, this);
}

bool CGameLogic::_SavePlayerDataToDatabase( DWORD uid, int chairID )
{
	return _SavePlayerDataToDatabase(uid, chairID, pIGameManager);
}

bool CGameLogic::_SavePlayerDataToDatabase( DWORD uid, int chairID, IGameManagerDataBase* psql )
{
	if (IsChairAI(chairID))
		return true;

	if (player[chairID].alData.playerRegulation > 2)
	{
		LogError("数据库数值异常 playerRegulation:"<<player[chairID].alData.playerRegulation<<" uid:"<<uid);
		player[chairID].alData.playerRegulation = -3;
	}
	if (player[chairID].alData.playerRegulation < -4)
	{
		LogError("数据库数值异常 playerRegulation:"<<player[chairID].alData.playerRegulation<<" uid:"<<uid);
		player[chairID].alData.playerRegulation = -4;
	}

	bool b = false;
	
	SDataBasePlayerData dbpd;
	memset(&dbpd,0,sizeof(dbpd));

	SP_To_DBPD(&player[chairID].alData, &dbpd);
	dbpd.lostScore = player[chairID].lostScore;
	dbpd.IP = player[chairID].IP;
	dbpd.maxHitCount = player[chairID].maxHitCount;

	lstrcpyn(dbpd.name,player[chairID].szName,32);

	int nUserRatio = 1;
	int nFishRatio = 1;
	pTable->GetExchangeRatio(nUserRatio, nFishRatio);
	b = psql->UpdateUserData(uid, chairID, nUserRatio, nFishRatio, &dbpd, this);
	return b;
}

bool CGameLogic::SetCannonTpye( int chairID, BYTE type )
{
	if(type>=1){
		if (player[chairID].energy < MAX_ENERGY)return false;
		player[chairID].energy = 0;
	}	
	player[chairID].cannonType = type;

	return true;
}

void CGameLogic::SetplayerRegulation(int chairID,int playerRegulation, bool bIncreaseDiffic)
{
	if(!player[chairID].alData.bControl)
	{
		if (bIncreaseDiffic)
		{
			if (player[chairID].alData.playerRegulation>playerRegulation)
			{
				player[chairID].alData.playerRegulation = playerRegulation;
			}
			else
			{
				player[chairID].alData.playerRegulation--;
			}
		}
		else
		{
			player[chairID].alData.playerRegulation=playerRegulation;
		}

		if (player[chairID].alData.playerRegulation<-4 || player[chairID].alData.playerRegulation>2)
		{
			player[chairID].alData.playerRegulation = -4;
		}
	}
}

void CGameLogic::InitChair( DWORD uid, DWORD IP, const TCHAR* name, int chairID, bool bAI,  SCORE lscore)
{
	totalUserCount++;
	bool bAdd = false;

	player[chairID].maxHitCount = 0;
	player[chairID].IP = IP;
	player[chairID].alData.playerNowPutScore=0;
	player[chairID].alData.playerNowScore=0;

	if(!bAI)
	{
		saveDataTimer -= 1;
		_ReadPlayerDataFromDatabase(uid, chairID, pIGameManager, &bAdd);
		realUserCount ++;
		ASSERT(realUserCount<=GAME_PLAYER);
	}
	else
	{//数据库里没有机器人信息
		memset(&player[chairID].alData,0,sizeof(SPlayer));
		player[chairID].alData.playerRegulation = -1;//最低难度
	}
	
	player[chairID].isAI=bAI;
	player[chairID].uid=uid;
	player[chairID].alData.dwPlayerID = uid;
	player[chairID].alData.playerGunScore = _GetMinGunScore();
	player[chairID].cannonType = 0;
	player[chairID].energy = 0;
	player[chairID].hitCount = 0;
	player[chairID].lastHitTime = 0;
	player[chairID].serverAcceptEnergy = 0;
	player[chairID].bActive = true;
	player[chairID].IP = IP;
	player[chairID].lscore = lscore;
	player[chairID].lscoreLogin = lscore;

	lstrcpyn(player[chairID].szName,name,32);
	player[chairID].szName[NAME_LEN-1] = 0;
}

bool CGameLogic::ClearChair( DWORD uid, int chairID )
{
	ASSERT(IsChairHasUser(chairID));
	totalUserCount--;
	if(!IsChairAI(chairID))
	{
		realUserCount--;
		ASSERT(realUserCount>=0);
		realUserCount = max(0,realUserCount);
	}

	bool ret = AccountChair(uid, chairID);
	if (ret==false)
	{
		for (int i=0;i<10;i++)
		{
			ret = AccountChair(uid, chairID);
			if(ret)break;
			Sleep(1);
		}
	}
	memset(&player[chairID].alData, 0, sizeof(SPlayer));
	player[chairID].uid = INVALID_DWORD;
	player[chairID].errorCount = 0;
	player[chairID].bActive = false;
	player[chairID].lostScore = 0;
	return ret;
}

void CGameLogic::_AcountScore( int chairID )
{
	player[chairID].alData.playerGetScore += player[chairID].alData.playerNowScore;

	player[chairID].alData.playerNowScore = 0;
	player[chairID].alData.playerNowPutScore = 0;
	CheckAndModifyPlayerData(&player[chairID].alData);
}

bool CGameLogic::AccountChair( DWORD uid, int chairID/*, SQLiteOperate& sql*/)
{
	_AcountScore(chairID);
	if(IsChairAI(chairID))return true;
	return _SavePlayerDataToDatabase(uid, chairID, pIGameManager);
}

void CGameLogic::AddScore( int chairID, __int64 score, bool upscore )
{
	player[chairID].alData.playerNowScore += score;

	if(upscore)
	{//上分
		player[chairID].alData.playerPutScore += score;
		player[chairID].alData.playerNowPutScore += score;
		CheckAndModifyPlayerData(&player[chairID].alData);
	}

	player[chairID].alData.playerMaxScore = max(player[chairID].alData.playerMaxScore, 
		player[chairID].alData.playerNowScore);
}

void CGameLogic::_AddScoreByAlgorithm( int chairID, __int64 score )
{
	player[chairID].alData.playerNowScore += score;

	if (player[chairID].alData.playerNowScore == 0 )
	{
		player[chairID].alData.playerDeadTime++;
		Print("玩家分数归零 uid:"<<GetUID(chairID));
	}
}

void CGameLogic::SetTable( TableFrameSink* table )
{
	pTable = table;
	
	saveTimeDelay = (float)pTable->GetMaxTable()*ONE_TABLE_DELAY;
	saveDataTimer = (float)pTable->GetID()*(saveTimeDelay/pTable->GetMaxTable());
}

void CGameLogic::ReadRoomData( const SRoomData* roomData_ )
{
	SRoomData lastData = roomData;
//	roomData.roomMaxCost = roomData_->roomMaxCost;
	roomData.onceUpScore = roomData_->onceUpScore;
	roomData.factor = roomData_->factor;
	roomData.level = roomData_->level;

	roomData.factorsmall = roomData_->factorsmall;
	roomData.levelsmall = roomData_->levelsmall;


	if (roomData.roomMaxCost <= 10)
	{
		roomData.roomMaxCost = DEFFALUT_ROOM_MAX_COST;
		LogError("roomName:"<<roomName<<" 读取失败，赋予默认值"<<DEFFALUT_ROOM_MAX_COST);
	}
	if (roomData.onceUpScore <= 10)
	{
		roomData.onceUpScore = DEFFALUT_UP_SCORE;
		LogError("单次上分数据有误! onceUpScore:"<<roomData.onceUpScore<<" 使用默认值"<<DEFFALUT_UP_SCORE);
	}

	if (roomData.level > 5 || roomData.level < 1)
	{
		roomData.level = DEFFALUT_LEVEL;
		roomData.levelsmall = DEFFALUT_LEVEL;
		LogError("roomData.level 有误");
	}

	if ((0.1 - roomData.factor) > LDBL_EPSILON || (roomData.factor - 2.0) > LDBL_EPSILON)
	{
		roomData.factor = DEFFALUT_FACTOR;		
		LogError("roomData.factor 有误");
	}

	if ((0.1 - roomData.factorsmall) > LDBL_EPSILON || (roomData.factorsmall - 2.0) > LDBL_EPSILON)
	{
		roomData.factorsmall =DEFFALUT_FACTOR;
	}

	if (lastData.onceUpScore != roomData.onceUpScore ||
		lastData.roomMaxCost != roomData.roomMaxCost ||
		lastData.factor != roomData.factor ||
		lastData.level != roomData.level)
	{
		printf("更新房间信息 onceUpScore:%d \nroomMaxCost:%d \nfactor:%f \nlevel:%d\nfactorsmall:%f \nlevelsamll:%d\n",roomData.onceUpScore,
			roomData.roomMaxCost,
			roomData.factor,
			roomData.level,
			roomData.factorsmall,
			roomData.levelsmall
			);
	}
}

//void CGameLogic::_ReadRoomData( const char* roomName_ )
//{
//	bool b = false;
//	if (roomName_ == NULL)
//		return;
//
//	if (pTable->GetID()!=1)
//		return;
//
//	strcpy(roomName, roomName_);
//
//	b = pIGameManager->GetRoomData();
//
//	if (!b)
//	{
//		MessageBox(NULL,TEXT("读取房间失败"),TEXT("错误"),0);
//		throw std::exception("读取房间失败");
//
//		LogError("读取房间失败 本房间名:"<<roomName);
//		roomData.roomMaxCost = DEFFALUT_ROOM_MAX_COST;
//		roomData.onceUpScore = DEFFALUT_UP_SCORE;
//
//		roomData.factor = DEFFALUT_FACTOR;
//		roomData.level = DEFFALUT_LEVEL;
//		return;
//	}
//
//	if (roomData.roomMaxCost <= 10)
//	{
//		roomData.roomMaxCost = DEFFALUT_ROOM_MAX_COST;
//		LogError("roomName:"<<roomName<<" 读取失败，赋予默认值"<<DEFFALUT_ROOM_MAX_COST);
//	}
//	if (roomData.onceUpScore <= 10)
//	{
//		roomData.onceUpScore = DEFFALUT_UP_SCORE;
//		LogError("单次上分数据有误! onceUpScore:"<<roomData.onceUpScore<<" 使用默认值"<<DEFFALUT_UP_SCORE);
//	}
//
//	if (roomData.level > 5 || roomData.level < 1)
//	{
//		roomData.level = DEFFALUT_LEVEL;
//		LogError("roomData.level 有误");
//	}
//
//	if (roomData.factor < 0.1f || roomData.factor > 2.f)
//	{
//		roomData.factor = DEFFALUT_FACTOR;
//		LogError("roomData.factor 有误");
//	}
//
//	if (b)
//	{
//		//_Print_("读取到的房间信息 onceUpScore:"<<roomData.onceUpScore<<
//		//	" roomMaxCost:"<<roomData.roomMaxCost<<
//		//	" factor:"<<roomData.factor<<
//		//	" level:"<<roomData.level
//		//	);
//	}
//}

void CGameLogic::_UpdateRoomData()
{
	bool b= false;
	SRoomData lastData = roomData;

	b = pIGameManager->GetRoomData();
	if (lastData.onceUpScore != roomData.onceUpScore ||
		lastData.roomMaxCost != roomData.roomMaxCost ||
		lastData.factor != roomData.factor ||
		lastData.level != roomData.level)
	{
		//_Print_("更新房间信息 onceUpScore:"<<roomData.onceUpScore<<
		//	" roomMaxCost:"<<roomData.roomMaxCost<<
		//	" factor:"<<roomData.factor<<
		//	" level:"<<roomData.level
		//	);
	}

	if (!b)
	{
		roomData = lastData;
		return;
	}

	if (roomData.roomMaxCost <= 0)
	{
		roomData.roomMaxCost = DEFFALUT_ROOM_MAX_COST;
		LogError("roomName:"<<roomName<<" 读取失败，赋予默认值"<<DEFFALUT_ROOM_MAX_COST);
	}
	if (roomData.onceUpScore <= 0)
	{
		roomData.onceUpScore = DEFFALUT_UP_SCORE;
		LogError("单次上分数据有误! onceUpScore:"<<roomData.onceUpScore<<" 使用默认值"<<DEFFALUT_UP_SCORE);
	}

	if (roomData.level > 5 || roomData.level < 0)
	{
		roomData.level = DEFFALUT_LEVEL;
		LogError("roomData.level 有误");
	}

	if ((0.1 - roomData.factor) > LDBL_EPSILON || (roomData.factor - 2.0) > LDBL_EPSILON)
	{
		roomData.factor = DEFFALUT_FACTOR;
		LogError("roomData.factor 有误");
	}
}

void CGameLogic::_dberror( const char* msg )
{
	LogError(msg);
}

int CGameLogic::GetRatioByTpye( int type )
{

	switch(type)
	{
	case 1:
	case 2:return 2;
	case 3:return 3;
	case 4:return 4;
	case 5:return 5;
	case 6:return 6;
	case 7:return 7;
	case 8:return 8;
	case 9:return 9;
	case 10:return 10;
	case 11:return 12;
	case 12:return 15;
	case 13:return 18;
	case 14:return 20;
	case 15:return 25;
	case 16:return 30;
	case 17:return 40;
	case 18:return 150;
	case 19:return 200;
	case 20:return 300;
	case 23:return 20;
	default:return 10;
	}

	if (type<1 || type>MAX_FISH_TYPE)
	{
		LogError("fish type error type:"<<type);
		return 10;
	}
}

void CGameLogic::AddEnergy( int chairID, BYTE number )
{
	/*if(player[chairID].energy < MAX_ENERGY && player[chairID].cannonType == 0)
	{
		player[chairID].energy += number;
		player[chairID].energy = min(player[chairID].energy , MAX_ENERGY);
		if(pTable)
		{
			pTable->SendChangeEnergy(chairID, player[chairID].energy);
		}
	}*/
}


bool CGameLogic::OnGetUserData( DWORD dwUserID, WORD wChairID, SDataBasePlayerData * pPlayer, bool bSuccess )
{
	if(!bSuccess)
	{
		SDataBasePlayerData dbpd;
		memset(&dbpd,0,sizeof(dbpd));
		SP_To_DBPD(&player[wChairID].alData, &dbpd);
		dbpd.IP = player[wChairID].IP;
		lstrcpyn(dbpd.name,player[wChairID].szName,32);
		pIGameManager->SaveUserData(dwUserID, &dbpd);

		dbpd.lostScore = player[wChairID].lostScore;

		return true;
	}
	SDataBasePlayerData dbpd;
	memset(&dbpd,0,sizeof(dbpd));


	DBPD_To_SP(pPlayer, &player[wChairID].alData);
	player[wChairID].lostScore = dbpd.lostScore;

	if (player[wChairID].alData.playerRegulation > 2)
	{
		LogError("数据库数值异常 playerRegulation:"<<player[wChairID].alData.playerRegulation<<" uid:"<<dwUserID);
		player[wChairID].alData.playerRegulation = -3;
	}
	if (player[wChairID].alData.playerRegulation < -4)
	{
		LogError("数据库数值异常 playerRegulation:"<<player[wChairID].alData.playerRegulation<<" uid:"<<dwUserID);
		player[wChairID].alData.playerRegulation = -4;
	}

	CheckPlayerData(&player[wChairID].alData);

	return true;
}

bool CGameLogic::OnUpdateUserData( DWORD dwUserID, WORD wChairID,  int nPlayerRegulation,bool bControl, bool bSuccess )
{
	player[wChairID].alData.bControl=bControl;
	if(!bSuccess)
	{
		SDataBasePlayerData dbpd;
		memset(&dbpd,0,sizeof(dbpd));
		SP_To_DBPD(&player[wChairID].alData, &dbpd);
		dbpd.IP = player[wChairID].IP;
		lstrcpyn(dbpd.name,player[wChairID].szName,32);
		pIGameManager->SaveUserData(dwUserID, &dbpd);
		return true;
	}
	else
	{
		if( (pTable->GetFreshPlayer() == 0) || pTable->IsLostPlayer(wChairID) || bControl)		//不刷新玩家难易因子或者秒杀的账号
			player[wChairID].alData.playerRegulation = nPlayerRegulation;
	}
	return true;
}

void CGameLogic::GetOnLineTime(DWORD dwUserID,  WORD wChairID)
{
	pIGameManager->GetOnLineTime(dwUserID, wChairID, this);
}

bool CGameLogic::OnUpdateOnLineTime(DWORD dwUserID, WORD wChairID, int nAllStep, int nCurStep, int nTotalTime)
{
	if(pTable)
	{
		pTable->GetOnLineTime(wChairID ,nAllStep, nCurStep, nTotalTime);
	}

// 	player[wChairID].onLineTime = nTime;
// 	player[wChairID].nGetStatus = nStatus;
	return true;
}

void CGameLogic::RecordLoginTime(DWORD dwUserID, int nRecord)
{
	pIGameManager->RecordLoginTime(dwUserID, nRecord);
}

void CGameLogic::ConfirmAward(DWORD dwUserID, WORD wChairID)
{
	pIGameManager->ConfirmAward(dwUserID, wChairID, this);
}

bool CGameLogic::OnUpdateUserCoin(DWORD dwUserID, WORD wChairID, int nStep)
{
	if (pTable)
	{
		pTable->OnUpdateUserCoin(dwUserID ,wChairID, nStep);
	}
	return true;
}

void CGameLogic::WriteEntityDate(DWORD dwUserID, int nTicketCount, int nIpadCount, int nMp3Count, int nWhiteCount, int nZippoCount)
{
	pIGameManager->WriteEntityDate(dwUserID, nTicketCount, nIpadCount, nMp3Count, nMp3Count, nZippoCount);
}




//////////////////////////////////////////////////////////////////////////




