#ifndef CMD_DZSHOWHAND_HEAD_FILE
#define CMD_DZSHOWHAND_HEAD_FILE
#pragma pack(1)//<by hxh>
//////////////////////////////////////////////////////////////////////////
#if 0
typedef DOUBLE DZPKSCORE;
#else
typedef LONGLONG DZPKSCORE;
#endif

//�����궨��

#define KIND_ID							308									//��Ϸ I D
#define GAME_PLAYER						8									//��Ϸ����
#define GAME_NAME						TEXT("�����˿�")					//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//�汾��Ϣ
#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//����汾

//����ԭ��
#define GER_NO_PLAYER					0x10								//û�����



//��Ŀ����
#define FULL_COUNT					    52									//ȫ����Ŀ
#define MAX_COUNT						2									//�����Ŀ
#define MAX_CENTERCOUNT					5									//�����Ŀ

//��ֵ����
#define	CARD_MASK_COLOR				0xF0									//��ɫ����
#define	CARD_MASK_VALUE				0x0F									//��ֵ����

#define  SMALL_CARD_WIDTH			25
#define  SMALL_CARD_HEIGHT			33

//X ���з�ʽ
enum enXCollocateMode 
{
	enXLeft,						//�����
	enXCenter,						//�ж���
	enXRight,						//�Ҷ���
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,							//�϶���
	enYCenter,						//�ж���
	enYBottom,						//�¶���
};

//�˿˽ṹ
struct tagCardItem
{
	bool							bEffect;							//��Ч��־
	bool							bMy;								//�Լ���־
	bool							bShoot;								//�����־
	BYTE							cbCardData;							//�˿�����
};



//���Զ���
#define MAX_CARD_COUNT				5									//�˿���Ŀ
#define SPACE_CARD_DATA				255									//����˿�


//��ඨ��
#define DEF_X_DISTANCE				19									//Ĭ�ϼ��
#define DEF_Y_DISTANCE				17									//Ĭ�ϼ��
#define DEF_SHOOT_DISTANCE			20									//Ĭ�ϼ��

//��ע���Ͷ��� "1��","2��ע","3��ע","4��ע","5����","6ȫ��
#define GAME_ACTION_PASS			1
#define GAME_ACTION_FOLLOW			2
#define GAME_ACTION_ADD				3
#define GAME_ACTION_BET				4
#define GAME_ACTION_GIVEUP			5
#define GAME_ACTION_ALLIN			6



//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE					101									//��ע���
#define SUB_S_GIVE_UP					102									//������ע	
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����
#define SUB_S_SIT_DOWN					105									//�û�����
#define SUB_S_OPEN_CARD					106									//�û�����
#define SUB_S_SP_USER					107									//�����û�

//��Ϸ״̬
struct CMD_S_StatusFree
{
	DZPKSCORE							lCellMinScore;						//��С��ע
	DZPKSCORE							lCellMaxScore;						//�����ע
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��ע��Ϣ
	DZPKSCORE							lCellScore;							//��Ԫ��ע
	DZPKSCORE							lTurnMaxScore;						//�����ע
	DZPKSCORE							lTurnLessScore;						//��С��ע
	DZPKSCORE							lCellMaxScore;						//�����ע
	DZPKSCORE							lAddLessScore;						//����Сע
	DZPKSCORE							lTableScore[GAME_PLAYER];			//��ע��Ŀ
	DZPKSCORE							lTotalScore[GAME_PLAYER];			//�ۼ���ע
	DZPKSCORE							lCenterScore;						//���ĳ���

	//״̬��Ϣ
	WORD								wDUser;								//D���
	WORD				 				wCurrentUser;						//��ǰ���
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	DZPKSCORE							lUserMaxScore[GAME_PLAYER];			//�û������<by hxh>
	BYTE							    cbBalanceCount;						//ƽ�����

	//�˿���Ϣ
	BYTE								cbCenterCardData[MAX_CENTERCOUNT];	//�˿���Ŀ
	BYTE								cbHandCardData[MAX_COUNT];			//�����˿�
};

//�����˿�
struct CMD_S_GameStart
{
	WORD				 				wCurrentUser;						//��ǰ���
	WORD								wDUser;								//D���
	WORD								wMinChipInUser;						//Сäע���
	WORD								wMaxChipInUser;						//��äע���	
	DZPKSCORE							lCellScore;							//��Ԫ��ע
	DZPKSCORE							lTurnMaxScore;						//�����ע
	DZPKSCORE							lTurnLessScore;						//��С��ע
	DZPKSCORE							lAddLessScore;						//����Сע
	DZPKSCORE							lUserMaxScore[GAME_PLAYER];			//�û������<by hxh>
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//�û��˿�
};

//�����˿�
struct CMD_S_SpUser
{
	BYTE								cbAllData[GAME_PLAYER][MAX_COUNT];	//�û��˿�
	BYTE								cbCenterCardData[MAX_CENTERCOUNT];	//�����˿�
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wAddScoreUser;						//��ע�û�
	DZPKSCORE							lAddScoreCount;						//��ע��Ŀ
	DZPKSCORE							lTurnLessScore;						//���ټ�ע
	DZPKSCORE							lTurnMaxScore;						//�����ע
	DZPKSCORE							lAddLessScore;						//����Сע
	//BYTE								cbShowHand[GAME_PLAYER];			//����û�
	BYTE								cbJudgeAction;                      //�ж���ע����   "1��","2��ע","3��ע","4��ע","5����","6ȫ��
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wGiveUpUser;						//�����û�
	DZPKSCORE							lLost;								//������	
};

//�������ݰ�
struct CMD_S_SendCard
{
	BYTE								cbPublic;							//�Ƿ���
	WORD								wCurrentUser;						//��ǰ�û�
	BYTE								cbSendCardCount;					//������Ŀ
	BYTE								cbCenterCardData[MAX_CENTERCOUNT];	//�����˿�	
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE								cbTotalEnd;							//ǿ�˱�־
	double 								lGameTax[GAME_PLAYER];				//��Ϸ˰��
	double								lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//�û��˿�//zrh
	BYTE								cbLastCenterCardData[GAME_PLAYER][MAX_CENTERCOUNT];//����˿�
	BYTE								cbLastCardKind[GAME_PLAYER];           //��������
};

//��Ϸ����
struct CMD_S_OpenCard
{
	WORD								wWinUser;							//�û�����
};
//�û�����
struct CMD_S_SitDown
{
	WORD								wSitUser;							//�����û�
};

//�û�׼��
struct CMD_S_Ready
{
	WORD								wReadyUser;							//����׼��

};



//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_ADD_SCORE					1									//�û���ע
#define SUB_C_GIVE_UP					2									//������Ϣ
#define SUB_C_OPEN_CARD					3									//������Ϣ
//#define SUB_C_SIT						4									//������Ϣ
//#define SUB_C_READY					5									//�û�׼����Ϣ



//�û���ע
struct CMD_C_AddScore
{
	DZPKSCORE							lScore;								//��ע��Ŀ
	BYTE								cbJudgeAction;                      //�ж���ע����   "1��","2��ע","3��ע","4��ע","5����","6ȫ��

};


//////////////////////////////////////////////////////////////////////////
#pragma pack()//<by hxh>
#endif
