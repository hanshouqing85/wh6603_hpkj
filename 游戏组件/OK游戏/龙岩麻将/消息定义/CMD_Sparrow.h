#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define GAME_PLAYER					4									//��Ϸ����
#ifdef USE_GB
#define KIND_ID						512									//��Ϸ I D
#define GAME_NAME					TEXT("�����齫")					//��Ϸ����
#else
#define KIND_ID						311									//��Ϸ I D
#define GAME_NAME					TEXT("�����齫")					//��Ϸ����
#endif
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾///////////
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾//////////

//��Ϸ״̬
////#define GS_MJ_FREE					GS_FREE								//����״̬
#define GS_MJ_FREE					GAME_STATUS_FREE								//����״̬
////#define GS_MJ_PLAY					(GS_PLAYING+1)						//��Ϸ״̬
#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)						//��Ϸ״̬

//��������
#define MAX_WEAVE					4									//������
#define MAX_INDEX					42									//�������
#define MAX_COUNT					14									//�����Ŀ
#define MAX_REPERTORY				144									//�����
#define MAX_HUA_CARD				8									//���Ƹ���
// by hxh
#define SPRING_INDEX			    34									//��������
#define SPRING_DATA					0x41								//��������

//�˿˶���
#define HEAP_FULL_COUNT				36									//����ȫ��

#define MAX_RIGHT_COUNT				3									//���ȨλDWORD����			

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbCardData[4];						//�������
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_LISTEN				103									//�û�����
#define SUB_S_REPLACE_CARD			108									//�û�����

//////////////////////////////////////////////////////////////////////////////////
//״̬����/////////////////

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONGLONG						lCellScore;									//��Ԫ����
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	WORD							wReplaceUser;								//�����滻�û�
	BYTE							cbQuanFeng;									//Ȧ��

	//״̬����
	BYTE							cbActionCard;								//�����˿�
	BYTE							cbActionMask;								//��������
	BYTE							cbHearStatus[GAME_PLAYER];					//����״̬
	BYTE							cbLeftCardCount;							//ʣ����Ŀ
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�

	//������Ϣ
	WORD							wOutCardUser;								//�����û�
	BYTE							cbOutCardData;								//�����˿�
	BYTE							cbDiscardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//������¼

	//�˿�����
	BYTE							cbCardCount;								//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbSendCardData;								//�����˿�

	//����˿�
	BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	//������Ϣ
	WORD							wHeapHead;									//����ͷ��
	WORD							wHeapTail;									//����β��
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	LONG							lSiceCount;									//���ӵ���
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	WORD							wReplaceUser;								//�����û�
	BYTE							cbHuaCardData;						        //�����齫
	BYTE							cbUserAction;								//�û�����
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];			//�˿��б�
	BYTE							cbQuanFeng;									//Ȧ��
	WORD							wHeapHead;									//������ͷ
	WORD							wHeapTail;									//������β
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wReplaceUser;						//�����û�
	BYTE							cbHuaCardData;						//�����齫
	WORD							wSendCardUser;						//�����û�
	bool							bTail;								//ĩβ����
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard[3];					//�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;							//��Ϸ˰��
	//������Ϣ
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbProvideCard;						//��Ӧ�˿�
	DWORD							dwChiHuKind[GAME_PLAYER];			//��������
	DWORD							dwChiHuRight[MAX_RIGHT_COUNT];		//��������
	BYTE							cbHuaCardCount;						//���Ƹ���
	BYTE							cbFanCount;							//�ܷ���

	//������Ϣ
	LONGLONG						lGameScore[GAME_PLAYER];			//��Ϸ����

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//�˿�����
};

//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};

//�û�����
struct CMD_S_Listen
{
	WORD							wChairId;							//�����û�
};

//��������
struct CMD_S_ReplaceCard
{
	WORD							wReplaceUser;						//�����û�
	BYTE							cbReplaceCard;						//�����˿�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_LISTEN				2									//�û�����
#define SUB_C_REPLACE_CARD			5									//�û�����

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard[3];					//�����˿�
};

//�û�����
struct CMD_C_Listen
{
	BYTE							cbListen;							//�����û�
};

//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};

//��������
struct CMD_C_ReplaceCard
{
	BYTE							cbCardData;							//�˿�����
};

//////////////////////////////////////////////////////////////////////////

#endif
