#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						200									//��Ϸ I D
#define GAME_NAME					TEXT("������")						//��Ϸ����

//�������
#define GAME_PLAYER					3									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//////////////////////////////////////////////////////////////////////////////////

//��Ŀ����
#define MAX_COUNT					20									//�����Ŀ
#define FULL_COUNT					54									//ȫ����Ŀ

//�߼���Ŀ
#define NORMAL_COUNT				17									//������Ŀ
#define DISPATCH_COUNT				51									//�ɷ���Ŀ
#define GOOD_CARD_COUTN				38									//������Ŀ

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//�߼�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
#define CT_SINGLE_LINE				4									//��������
#define CT_DOUBLE_LINE				5									//��������
#define CT_THREE_LINE				6									//��������
#define CT_THREE_TAKE_ONE			7									//����һ��
#define CT_THREE_TAKE_TWO			8									//����һ��
#define CT_FOUR_TAKE_ONE			9									//�Ĵ�����
#define CT_FOUR_TAKE_TWO			10									//�Ĵ�����
#define CT_BOMB_CARD				11									//ը������
#define CT_MISSILE_CARD				12									//�������

//////////////////////////////////////////////////////////////////////////////////
//״̬����

//#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
//#define GAME_SCENE_CALL				GAME_STATUS_PLAY					//�з�״̬
//#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//��Ϸ����

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_CALL				GAME_STATUS_PLAY					//�е���״̬
#define GAME_SCENE_ROD				GAME_STATUS_PLAY+1					//������״̬
//#define GAME_SCENE_ADD				GAME_STATUS_PLAY+2					//�ӱ�״̬
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+2					//��Ϸ����


//�е���
#define CB_NOT_CALL					0									//û�е���
#define CB_CALL_BENKER				1									//�е���
#define CB_NO_CALL_BENKER			2									//���е���

//������
#define CB_NOT_ROD					0									//û������
#define CB_ROD_BANKER				1									//������
#define CB_NO_ROD_BANKER			2									//��������
#define CB_CAN_NO_ROD				3									//������

//�ӱ���Ϣ
#define CB_NOT_ADD_DOUBLE			0									//û�ӱ�
#define CB_ADD_DOUBLE				1									//�ӱ���־
#define CB_NO_ADD_DOUBLE			2									//���ӱ�




//����״̬
struct CMD_S_StatusFree
{
	//��Ϸ����
	LONG							lCellScore;							//��������

	//ʱ����Ϣ
	BYTE							cbTimeCallBanker;					//�е���ʱ��
	BYTE							cbTimeRodBanker;					//������ʱ��
	BYTE							cbTimeOutCard;						//����ʱ��
	//BYTE							cbTimeCallScore;					//�з�ʱ��
	BYTE							cbTimeStartGame;					//��ʼʱ��
	BYTE							cbTimeHeadOutCard;					//�׳�ʱ��

	//��ʷ����
	SCORE							lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE							lCollectScore[GAME_PLAYER];			//������Ϣ
};

////�з�״̬
//struct CMD_S_StatusCall
//{
//	//ʱ����Ϣ
//	BYTE							cbTimeOutCard;						//����ʱ��
//	BYTE							cbTimeCallScore;					//�з�ʱ��
//	BYTE							cbTimeStartGame;					//��ʼʱ��
//	BYTE							cbTimeHeadOutCard;					//�׳�ʱ��
//
//	//��Ϸ��Ϣ
//	LONG							lCellScore;							//��Ԫ����
//	WORD							wCurrentUser;						//��ǰ���
//	BYTE							cbBankerScore;						//ׯ�ҽз�
//	BYTE							cbScoreInfo[GAME_PLAYER];			//�з���Ϣ
//	BYTE							cbHandCardData[NORMAL_COUNT];		//�����˿�
//
//	//��ʷ����
//	SCORE							lTurnScore[GAME_PLAYER];			//������Ϣ
//	SCORE							lCollectScore[GAME_PLAYER];			//������Ϣ
//};

//�е���״̬
struct CMD_S_StatusCall
{
	//ʱ����Ϣ
	BYTE							cbTimeCallBanker;					//�е���ʱ��
	BYTE							cbTimeRodBanker;					//������ʱ��
	//BYTE							cbTimeAddDouble;					//�ӱ�ʱ��
	BYTE							cbTimeStartGame;					//��ʼʱ��
	BYTE							cbTimeHeadOutCard;					//�׳�ʱ��
	BYTE							cbTimeOutCard;						//����ʱ��

	//��Ϸ��Ϣ
	LONG							lCellScore;							//��Ԫ����
	WORD							wFirstUser;							//�׽��û�
	//WORD							wFirstValidUser;					//�׽�����
	WORD							wCurrentUser;						//��ǰ���
	WORD							wStartTime;							//��ʼ����
	//WORD							wValidCardTime;						//���Ʊ���
	//bool							bValidCardInfo[GAME_PLAYER];		//������Ϣ
	BYTE							cbCallBankerInfo[GAME_PLAYER];		//�е�����Ϣ	
	BYTE							cbHandCardData[GAME_PLAYER][NORMAL_COUNT];		//�����˿�

	//��ʷ����
	SCORE							lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE							lCollectScore[GAME_PLAYER];			//������Ϣ
};

//������״̬
struct CMD_S_StatusRod
{
	//ʱ����Ϣ
	BYTE							cbTimeCallBanker;					//�е���ʱ��
	BYTE							cbTimeRodBanker;					//������ʱ��
	//BYTE							cbTimeAddDouble;					//�ӱ�ʱ��
	BYTE							cbTimeStartGame;					//��ʼʱ��
	BYTE							cbTimeHeadOutCard;					//�׳�ʱ��
	BYTE							cbTimeOutCard;						//����ʱ��

	//��Ϸ��Ϣ
	LONG							lCellScore;							//��Ԫ����
	WORD							wFirstUser;							//�׽��û�
	//WORD							wFirstValidUser;					//�׽�����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	WORD							wStartTime;							//��ʼ����
	//WORD							wValidCardTime;						//���Ʊ���
	WORD							wRodBankerTime;						//����������

	//bool							bValidCardInfo[GAME_PLAYER];		//������Ϣ
	BYTE							cbCallBankerInfo[GAME_PLAYER];		//�е�����Ϣ	
	BYTE							cbRodBankerInfo[GAME_PLAYER];		//��������Ϣ
	//BYTE							cbValidCardInfo[GAME_PLAYER];		//������Ϣ
	BYTE							cbHandCardData[GAME_PLAYER][NORMAL_COUNT];//�����˿�

	//��ʷ����
	SCORE							lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE							lCollectScore[GAME_PLAYER];			//������Ϣ
};

////��Ϸ״̬
//struct CMD_S_StatusPlay
//{
//	//ʱ����Ϣ
//	BYTE							cbTimeOutCard;						//����ʱ��
//	BYTE							cbTimeCallScore;					//�з�ʱ��
//	BYTE							cbTimeStartGame;					//��ʼʱ��
//	BYTE							cbTimeHeadOutCard;					//�׳�ʱ��
//
//	//��Ϸ����
//	LONG							lCellScore;							//��Ԫ����
//	BYTE							cbBombCount;						//ը������
//	WORD							wBankerUser;						//ׯ���û�
//	WORD							wCurrentUser;						//��ǰ���
//	BYTE							cbBankerScore;						//ׯ�ҽз�
//
//	//������Ϣ
//	WORD							wTurnWiner;							//ʤ�����
//	BYTE							cbTurnCardCount;					//������Ŀ
//	BYTE							cbTurnCardData[MAX_COUNT];			//��������
//
//	//�˿���Ϣ
//	BYTE							cbBankerCard[3];					//��Ϸ����
//	BYTE							cbHandCardData[MAX_COUNT];			//�����˿�
//	BYTE							cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ
//
//	//��ʷ����
//	SCORE							lTurnScore[GAME_PLAYER];			//������Ϣ
//	SCORE							lCollectScore[GAME_PLAYER];			//������Ϣ
//};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ʱ����Ϣ
	BYTE							cbTimeCallBanker;					//�е���ʱ��
	BYTE							cbTimeRodBanker;					//������ʱ��
	BYTE							cbTimeAddDouble;					//�ӱ�ʱ��
	BYTE							cbTimeStartGame;					//��ʼʱ��
	BYTE							cbTimeHeadOutCard;					//�׳�ʱ��
	BYTE							cbTimeOutCard;						//����ʱ��

	//��Ϸ����
	LONG							lCellScore;							//��Ԫ����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ���

	WORD							wStartTime;							//��ʼ����
	//WORD							wValidCardTime;						//���Ʊ���
	WORD							wRodBankerTime;						//����������
	WORD							wBackCardTime;						//���Ʊ���
	WORD							wBombTime;							//ը������
	WORD							wCardTypeTime;						//���ͱ���

	BYTE							cbBombCount;						//ը������
	BYTE							cbCallBankerInfo[GAME_PLAYER];		//�е�����Ϣ	
	BYTE							cbRodBankerInfo[GAME_PLAYER];		//��������Ϣ
	BYTE							cbAddDoubleInfo[GAME_PLAYER];		//�ӱ���Ϣ	
	//bool							bValidCardInfo[GAME_PLAYER];		//������Ϣ

	//������Ϣ
	WORD							wTurnWiner;							//ʤ�����
	BYTE							cbTurnCardCount;					//������Ŀ
	BYTE							cbTurnCardData[MAX_COUNT];			//��������

	//�˿���Ϣ
	BYTE							cbBankerCard[3];					//��Ϸ����
	BYTE							cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE							cbHandCardData[GAME_PLAYER][MAX_COUNT];		//�����˿�

	//��ʷ����
	SCORE							lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE							lCollectScore[GAME_PLAYER];			//������Ϣ
};


//////////////////////////////////////////////////////////////////////////////////
//�����

//#define SUB_S_GAME_START			100									//��Ϸ��ʼ
//#define SUB_S_CALL_SCORE			101									//�û��з�
//#define SUB_S_BANKER_INFO			102									//ׯ����Ϣ
//#define SUB_S_OUT_CARD			103									//�û�����
//#define SUB_S_PASS_CARD			104									//�û�����
//#define SUB_S_GAME_CONCLUDE		105									//��Ϸ����
//#define SUB_S_SET_BASESCORE		106									//���û���
#define SUB_S_CHEAT_CARD			111									//�����˿�

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
//#define SUB_S_CALL_SCORE			101									//�û��з�
#define SUB_S_CALL_BANKER			101									//�е���
#define SUB_S_BANKER_INFO			102									//ׯ����Ϣ
#define SUB_S_ROD_BANKER			103									//������
//#define SUB_S_DOUBLE				104									//�ӱ���Ϣ
//#define SUB_S_VALID_CARD			105									//�û�����
#define SUB_S_OUT_CARD				106									//�û�����
#define SUB_S_PASS_CARD				107									//�û�����
#define SUB_S_OUT_START_START		108									//��ʼ����
#define SUB_S_GAME_CONCLUDE			109									//��Ϸ����
#define SUB_S_SET_BASESCORE			110									//���û���

//�����˿�
struct CMD_S_GameStart
{
	WORD							wStartUser;							//��ʼ���
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wStartTime;							//��ʼ����
	//bool							bValidCard;							//�Ƿ�����
	//BYTE							cbValidCardData;					//�����˿�
	//BYTE							cbValidCardIndex;					//����λ��
	BYTE							cbCardData[NORMAL_COUNT];			//�˿��б�
};

//�������˿�
struct CMD_S_AndroidCard
{
	BYTE							cbHandCard[GAME_PLAYER][NORMAL_COUNT];//�����˿�
	WORD							wCurrentUser ;						//��ǰ���
};

//�����˿�
struct CMD_S_CheatCard
{
	WORD							wCardUser[GAME_PLAYER ];				//�������
	BYTE							cbUserCount;							//��������
	BYTE							cbCardData[GAME_PLAYER ][MAX_COUNT];	//�˿��б�
	BYTE							cbCardCount[GAME_PLAYER ];				//�˿�����

};
////�û��з�
//struct CMD_S_CallScore
//{
//	WORD				 			wCurrentUser;						//��ǰ���
//	WORD							wCallScoreUser;						//�з����
//	BYTE							cbCurrentScore;						//��ǰ�з�
//	BYTE							cbUserCallScore;					//�ϴνз�
//
//
//};
//�û��е���
struct CMD_S_CallBanker
{
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wLastUser;							//�е������
	BYTE							cbCallInfo;							//�е���
};

//�û�������
struct CMD_S_RodBanker
{
	WORD							wRodBankerTime;						//����������

	WORD				 			wBankerUser;						//ׯ�����
	WORD				 			wCurrentUser;						//��ǰ���	
	WORD							wLastUser;							//���������
	BYTE							cbRodlInfo;							//������
};
////�ӱ���Ϣ
//struct CMD_S_Double
//{
//	WORD							wCurrentUser;						//��ǰ�û�
//	BYTE							cbDouble;							//�Ƿ�ӱ�
//};

////ׯ����Ϣ
//struct CMD_S_BankerInfo
//{
//	WORD				 			wBankerUser;						//ׯ�����
//	WORD				 			wCurrentUser;						//��ǰ���
//	BYTE							cbBankerScore;						//ׯ�ҽз�
//	BYTE							cbBankerCard[3];					//ׯ���˿�
//};

//ׯ����Ϣ
struct CMD_S_BankerInfo
{
	WORD				 			wBankerUser;						//ׯ�����
	WORD				 			wCurrentUser;						//��ǰ���
	//WORD							wBackCardTime;						//���Ʊ���
	BYTE							cbBankerCard[3];					//ׯ���˿�
	//BYTE							cbBackCardType;						//��������
	//bool							bDoubleInfo[3];						//�ܷ�ӱ�
};

//��ʼ��Ϣ
struct CMD_S_Game_Start
{
	WORD				 			wBankerUser;						//ׯ�����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[MAX_COUNT];				//�˿�����

};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							cbTurnOver;							//һ�ֽ���
	WORD				 			wCurrentUser;						//��ǰ���
	WORD				 			wPassCardUser;						//�������
};

//��Ϸ����
struct CMD_S_GameConclude
{
	//���ֱ���
	LONG							lCellScore;							//��Ԫ����
	SCORE							lGameScore[GAME_PLAYER];			//��Ϸ����

	//�����־
	BYTE							bChunTian;							//�����־
	BYTE							bFanChunTian;						//�����־

	//ը����Ϣ
	BYTE							cbBombCount;						//ը������
	BYTE							cbEachBombCount[GAME_PLAYER];		//ը������

	//��Ϸ��Ϣ
	//BYTE							cbBankerScore;						//�з���Ŀ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbHandCardData[FULL_COUNT];			//�˿��б�
};

//�й�
//struct CMD_S_TRUSTEE
//{
//	WORD							wTrusteeUser;						//�й����
//	BYTE							bTrustee;							//�йܱ�־
//};

//////////////////////////////////////////////////////////////////////////////////
//�����
//
//#define SUB_C_CALL_SCORE			1									//�û��з�
//#define SUB_C_OUT_CARD			2									//�û�����
//#define SUB_C_PASS_CARD			3									//�û�����


//#define SUB_C_VALID_CARD			1									//�û�����
//#define SUB_C_CALL_SCORE			2									//�û��з�
#define SUB_C_CALL_BANKER			2									//�û��е���
#define SUB_C_ROD_BANKER			3									//�û�������
#define SUB_C_DOUBLE				4									//�û��ӱ�
#define SUB_C_OUT_CARD				5									//�û�����
#define SUB_C_PASS_CARD				6									//�û�����
#define SUB_C_TRUSTEE				7									//�û��й�

////�û��з�
//struct CMD_C_CallScore
//{
//	BYTE							cbCallScore;						//�з���Ŀ
//};

//�û��е���
struct CMD_C_CallBanker
{				
	BYTE							cbCallInfo;							//�е���
};

//�û�������
struct CMD_C_RodBanker
{				
	BYTE							cbRodInfo;							//������
};
//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿�����
};
////�й�
//struct CMD_C_TRUSTEE
//{
//	BYTE							bTrustee;							//�йܱ�־
//};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif