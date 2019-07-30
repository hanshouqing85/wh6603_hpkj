#include "stdafx.h"
#include "YJAlgorithm.h"
#include "math.h"
#include <afx.h>
#include <algorithm>
using namespace std;

extern int  g_nUseHistory; // 历史记录关联
extern SCORE g_stock_score_;

double Approximate(double a);

void InitMonster(  
				 SMonster *monster_out,
				 int _MonID, 
				 int _MonMul, 
				 int _RT, 
				 int _FirstNT,
				 int _SecondNT, 
				 int _ThirdNt, 
				 double _Influence,
				 double _MLevel,
				 double _InfluenceSmall,
				 double _MLevelSmall,
				 double _level1, 
				 double _level2, 
				 double _level3, 
				 double _level4, 
				 double _level5 )
{
	monster_out->MonID = _MonID;
	monster_out->MonMul = _MonMul;
	monster_out->RT = _RT;
	monster_out->FirstNT = _FirstNT;
	monster_out->SecondNT = _SecondNT;
	monster_out->ThirdNT = _ThirdNt;
	monster_out->Influence = _Influence;
	monster_out->MLevel = _MLevel;
	monster_out->level1 = _level1;
	monster_out->level2 = _level2;
	monster_out->level3 = _level3;
	monster_out->level4 = _level4;
	monster_out->level5 = _level5;
	monster_out->InfluenceSmall=_InfluenceSmall;
	monster_out->MlevelSmall=_MLevelSmall;
}

SMonster* GetMonster( SMonster* monsterteam, int type)
{
	ASSERT((type-1)>=0);
	ASSERT((type-1)<MAX_FISH_TYPE);
	return &monsterteam[type-1];
}

void SPMonster(SMonster* monster, int MonMul_, const SRoomData* difficult,bool bAndrior)
{
	InitMonster(monster, 0, 0, 0, 0, 0, 0, difficult->factor, difficult->level,difficult->factorsmall,difficult->levelsmall, 0, 0.5, 1.0, 1.5, 2.0);
	monster->MonMul = MonMul_;
	double ratio = monster->Influence / (monster->MLevel * monster->MonMul);

	if(MonMul_<30)
	{
		ratio=monster->InfluenceSmall/(monster->MlevelSmall * monster->MonMul);
	}
	else if ((MonMul_>50) && (g_stock_score_<900000)) // 库存不足,50倍以上调整
	{
		ratio = ratio*0.7;
	}
	double level = Approximate(ratio);
	monster->RT = (int)level;
	double _RMproduct = pow(level, 3) * ratio;

	int RMproduct = (int)_RMproduct;
	int time = RMproduct;
	int Array[] ={1,1,1};
     
	if (Array[0] * Array[1] * Array[2] == RMproduct)
	{
		 monster->FirstNT = Array[0];
		 monster->SecondNT = Array[1];
         monster->ThirdNT = Array[2];
	}
	for (int i = 0; i < time - 1; i++)
	{
		Array[i % 3] += 1;
		if (Array[0] * Array[1] * Array[2] == RMproduct)
		{ 
			monster->FirstNT = Array[0];
			monster->SecondNT = Array[1];
			monster->ThirdNT = Array[2];
		}
		if (Array[0] * Array[1] * Array[2] > RMproduct)
        {
			int a = Array[0] * Array[1] * Array[2] - RMproduct;

			monster->FirstNT = Array[0];
			monster->SecondNT = Array[1];
			monster->ThirdNT = Array[2];

            Array[(i - 1) % 3] -= 1;
            int b = abs((Array[0] * Array[1] * Array[2] - RMproduct));
            if (b < a)
            {
                 monster->FirstNT = Array[0];
				 monster->SecondNT = Array[1];
				 monster->ThirdNT = Array[2];
                 break;
			}
		} 
	}
}

double Approximate(double a)
{
    for (double i = 3; i < AL_PRECISION; i++)
    {
        double b = i * i * i * a;
        if ((double)((int)b) == b)
        {
			return i;
		}
    }
    return AL_PRECISION;
 }
//计算玩家level
int PlayerLevel(const SPlayer* player,
				const SMonster* monster,
				int MaxGunScore,
				bool bAndrior)
{
	double result1 = 0;
	if (g_nUseHistory > 0)
	{
		__int64 playerScore = player->playerPutScore - (player->playerNowScore + player->playerGetScore);
		if (playerScore<0 && playerScore>=-1000000)
		{
			result1 = 0;
		}
		else if (playerScore<-1000000)
		{
			result1 = ((double)playerScore+1000000)/((double)(7000*MaxGunScore));
		}
		else
		{
			result1 = ((double)playerScore)/((double)(8000*MaxGunScore));
		}
		if (result1>0.85)
		{
			result1 = 0.85;
		}

		if (result1>0)
		{
			// 控制状态不调整其等级
			/*if (player->bControl)
			{
				result1 = 0;
			}
			else*/ if (bAndrior || (rand()%1000>900))
			{
				double ret =  0.4-(((double)(rand()%1000)) / 1000.0);
				result1 = min(result1, ret);
			}

			if (monster->MonMul>100 && result1>-0.5)
			{
				double ret =  0.5-(((double)(rand()%1000)) / 1000.0);
				result1 = min(result1, ret);
			}			

			// 库存不足
			if ((g_stock_score_<1000000) && result1>0)
			{
				result1 = -(((double)(rand()%4000)) / 1000.0);
			}
		}		
	}

	double result2 = ((double) ((rand()*rand())%2000)) / 1000.0;
	double result = result1 + result2;

	//double result = /*result1 + */result2;
	if(result < monster->level1)
		return 0;
	else if(result >= monster->level1 && result < monster->level2)
		return 1;
	else if(result >= monster->level2 && result < monster->level3)
		return 2;
	else if(result >= monster->level3 && result < monster->level4)
		return 3;
	else if(result >= monster->level4 && result < monster->level5)
		return 4;
	else if(result >= monster->level5)
		return 5;
	return 0;
}

//手动影响值
void PlayerRegulation(SPlayer* player, int value)
{
	 player->playerRegulation  = value;
}

#define ARRAY_MAX	2048
//static int arrayA [ARRAY_MAX]={0};
//static int arrayB [ARRAY_MAX]={0};
//static int arrayC [ARRAY_MAX]={0};

//计算结果
__int64 RMSystem(int bulletCost,
			 int MaxGunScore,//当前房间的最大允许每炮分数
			 int nPlayerRegulation, // 不使用SPlayer中的了,专门传递参数
			 bool bAndrior,
			 const SMonster* monster,
			 const SPlayer* player,
			 LpPlayerLevel playerLevelFunc
			 )
{
	int arrayA [ARRAY_MAX]={0};
	int arrayB [ARRAY_MAX]={0};
	int arrayC [ARRAY_MAX]={0};

	int playerRegulation = nPlayerRegulation;

	//if(playerRegulation == -4)return 0;
	if (playerRegulation <= -3 && monster->MonMul >= 100)
	{
		return 0;
	}

	int ArrayMax = monster->RT;
	if(ArrayMax < 0 || ArrayMax >= ARRAY_MAX)return 0;

	for (int i = 0; i < ArrayMax; i++)
	{
		ASSERT(i>=0&&i<ARRAY_MAX);
		arrayA[i]=0;
		arrayB[i]=0;
		arrayC[i]=0;
	}

	int arrayAT = monster->FirstNT;
	int arrayBT = monster->SecondNT;
	int arrayCT = monster->ThirdNT;

	while(playerRegulation < 0)
	{
		arrayAT --;
		if (arrayAT == 0)
			return 0;
		playerRegulation++;
		if(playerRegulation == 0)
			break;
		arrayCT --;
		if (arrayCT == 0)
			return 0;
		playerRegulation++;
		if(playerRegulation == 0)
			break;
		arrayBT --;
		if(arrayBT == 0)
			return 0;
		playerRegulation++;
	}

	while (playerRegulation > 0)
	{
		arrayAT ++;
		playerRegulation--;
		if (playerRegulation == 0)
			break;
		arrayCT++;
		playerRegulation--;
		if (playerRegulation == 0)
			break;
		arrayBT++;
		playerRegulation--;
	}

	if(arrayAT < 0 || arrayAT >= ARRAY_MAX)return 0;
	if(arrayBT < 0 || arrayBT >= ARRAY_MAX)return 0;
	if(arrayCT < 0 || arrayCT >= ARRAY_MAX)return 0;

	for (int i = 0; i < arrayAT; i++)
	{
		ASSERT(i>=0&&i<ARRAY_MAX);
		arrayA[i] = 1;
	}

	for (int i = 0; i < arrayBT; i++)
	{
		ASSERT(i>=0&&i<ARRAY_MAX);
		arrayB[i] = 1;
	}

	for (int i = 0; i < arrayCT; i++)
	{
		ASSERT(i>=0&&i<ARRAY_MAX);
		arrayC[i] = 1;
	}

	for (int i = 1; i < ArrayMax; i++)
	{
		int a = MTRandom(0, i);
		ASSERT(i>=0&&i<ARRAY_MAX);
		swap(arrayA[i], arrayA[a]);
	}
	for (int i = 1; i < ArrayMax; i++)
	{
		int a = MTRandom(0, i);
		ASSERT(i>=0&&i<ARRAY_MAX);
		swap(arrayB[i], arrayB[a]);
	}
	for (int i = 1; i < ArrayMax; i++)
	{
		int a = MTRandom(0, i);
		ASSERT(i>=0&&i<ARRAY_MAX);
		swap(arrayC[a], arrayC[i]);
	}

	int newarrayA [3]={0};
	int aa = MTRandom(0, ArrayMax - 1);
	for (int i = 0; i < 3; i++)
	{
		ASSERT(aa>=0&&aa<ARRAY_MAX);
		newarrayA[i] = arrayA[aa];
		aa++;
		if (aa >= ArrayMax)
		{
			aa = 0;
		}
	}
	int newarrayB [3]={0};
	int ab = MTRandom(0, ArrayMax - 1);
	for (int i = 0; i < 3; i++)
	{
		ASSERT(ab>=0&&ab<ARRAY_MAX);
		newarrayB[i] = arrayB[ab];
		ab++;
		if (ab >= ArrayMax)
		{
			ab = 0;
		}
	}
	int newarrayC [3]={0};
	int ac = MTRandom(0, ArrayMax - 1);
	for (int i = 0; i < 3; i++)
	{
		ASSERT(ac>=0&&ac<ARRAY_MAX);
		newarrayC[i] = arrayC[ac];
		ac++;
		if (ac >= ArrayMax)
		{
			ac = 0;
		}
	}


	//判断是否成功击杀
	bool killfish = false;
	int level = playerLevelFunc(player, monster, MaxGunScore, bAndrior);
	
	CString strMsg;
	strMsg.Format(TEXT("[lkpy] level: %d"), level);
	OutputDebugString(strMsg);

	switch (level)   // 越大越容易
	{
	case 0:
		break;
	case 1:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		break;
	case 2:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[0] && newarrayB[0] == newarrayC[0] && newarrayA[0] != 0)
			killfish = true;
		break;
	case 3:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[0] && newarrayB[0] == newarrayC[0] && newarrayA[0] != 0)
			killfish = true;
		if (newarrayA[2] == newarrayB[2] && newarrayB[2] == newarrayC[2] && newarrayA[2] != 0)
			killfish = true;
		break;
	case 4:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[0] && newarrayB[0] == newarrayC[0] && newarrayA[0] != 0)
			killfish = true;
		if (newarrayA[2] == newarrayB[2] && newarrayB[2] == newarrayC[2] && newarrayA[2] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[1] && newarrayB[1] == newarrayC[2] && newarrayA[0] != 0)
			killfish = true;
		break;
	case 5:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[0] && newarrayB[0] == newarrayC[0] && newarrayA[0] != 0)
			killfish = true;
		if (newarrayA[2] == newarrayB[2] && newarrayB[2] == newarrayC[2] && newarrayA[2] != 0)
			killfish = true;
		if (newarrayA[2] == newarrayB[1] && newarrayB[1] == newarrayC[0] && newarrayA[2] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[1] && newarrayB[1] == newarrayC[2] && newarrayA[0] != 0)
			killfish = true;
		break;
	}

	//给分
	if (killfish == true)
	{
		return bulletCost * monster->MonMul;
	}
	else
		return 0;
}
