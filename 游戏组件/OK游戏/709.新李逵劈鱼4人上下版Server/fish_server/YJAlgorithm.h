#pragma once

#define MAX_FISH_TYPE		20
#define AL_PRECISION	100

struct SPlayer
{
	DWORD dwPlayerID;//���ID
	__int64 playerNowScore;//���Ŀǰ����
	__int64 playerNowPutScore;//��ҵ����Ϸ���
	__int64 playerPutScore;//������Ϸ�ֵ
	__int64 playerGetScore;//������·�ֵ
	int playerShootingTime;//���������
	__int64 playerMaxScore;//�������ֵ
	int playerDeadTime;//�����������
	int playerGunScore;//���ÿ�ڷ���
	int playerRegulation;//�ֶ��ٿ�Ӱ��ֵ(ȡֵ��Χ-4��2��ȡֵԽ��Խ���״���)
	bool bControl;
};




struct SMonster
{
	int MonID;//����ID
	int MonMul;//���ﱶ��
	int RT;//ÿ�����ָ�����������
	int FirstNT;//�������ָ��������ʳ�������
	int SecondNT;//�������ָ��������ʳ�������
	int ThirdNT;//ĩ�����ָ��������ʳ�������
	double Influence;//����ֵ
	double MLevel;//����ȼ���1��5��
	double InfluenceSmall;
	double MlevelSmall;
	//����������
	double level1;
	double level2;
	double level3;
	double level4;
	double level5;
};

//��������
struct SRoomData
{
	int		roomMaxCost;
	int		onceUpScore;
	double	factor;//�����Ѷ�ϵ����ȡֵ��Χ 0.1��2.0��ֵԽ��Խ���״�
	int		level;//�����Ѷȱ�׼������ȡֵ��Χ 1��5��ֵԽ��Խ�Ѵ�

	double	factorsmall;
	int		levelsmall;
};

//һ�����ӵ�����
struct TableData
{
	__int64 totalUpScore;//���Ϸ�
	__int64 totalDownScore;//���·�
	__int64 totalNowScore;//��ӯ��
	__int64 totalPlayUseScore;//���������
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
				 bool bAndrior);//�������level

typedef int (*LpPlayerLevel)(const SPlayer* player,
							 const SMonster* monster,
							 int MaxGunScore,
							 bool bAndrior);

//������
__int64 RMSystem(int bulletCost,
			 int MaxGunScore,//��ǰ������������ÿ�ڷ���
			 int nPlayerRegulation, // ��ʹ��SPlayer�е���,ר�Ŵ��ݲ���
			  bool bAndrior,
			 const SMonster* monster,
			 const SPlayer* player,
			 LpPlayerLevel playerLevel
			 );

void PlayerRegulation(SPlayer* player, 
					  int value);//�ֶ�Ӱ��ֵ


//////////////////////////////////////////////////////////////////////////



