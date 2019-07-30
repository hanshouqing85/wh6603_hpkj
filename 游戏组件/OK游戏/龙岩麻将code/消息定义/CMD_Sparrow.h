#ifndef CMD_SPARROW_LONGYAN_MJ_HEAD_FILE
#define CMD_SPARROW_LONGYAN_MJ_HEAD_FILE
#include <vector>

//////////////////////////////////////////////////////////////////////////
//�����궨��
#pragma pack(1)

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
#define GS_MJ_FREE					GAME_STATUS_FREE								//����״̬
#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)						//��Ϸ״̬
#define GS_MJ_XIAOHU				(GAME_STATUS_PLAY+2)							//С��״̬
#define GS_MJ_PIAO					(GAME_STATUS_PLAY+3)							//Ʈ״̬
	

//��������
#define MAX_WEAVE					4									//������
#define MAX_INDEX					42									//�������
#define MAX_COUNT					14									//�����Ŀ
#define MAX_REPERTORY				144									//�����
#define MAX_HUA_CARD				8									//���Ƹ���

//�˿˶���
#define HEAP_FULL_COUNT				(136/4)									//����ȫ��

#define MAX_RIGHT_COUNT				3									//���ȨλDWORD����			

#define	CAI_SHEN_CARD						MAX_INDEX
//////////////////////////////////////////////////////////////////////////

//��Ϸ����
#define GAME_TYPE_ZZ_ZIMOHU			1		//ֻ����ģ��
#define GAME_TYPE_ZZ_QIDUI			2		//�ɺ��߶�
#define GAME_TYPE_ZZ_QIANGGANGHU	3		//�����ܺ�
#define GAME_TYPE_ZZ_ZHANIAO2		4		//����2��
#define GAME_TYPE_ZZ_ZHANIAO4		5		//����4��
#define GAME_TYPE_ZZ_ZHANIAO6		6		//����6��
#define GAME_TYPE_ZZ_HONGZHONG		7		//�������
#define GAME_TYPE_CS_ZHUANGXIANFEN	8		//ׯ�з�
#define GAME_TYPE_ZZ_ZHANIAO3		9		//����3��
#define GAME_TYPE_ZZ_ZHANIAO5		10		//����5��
#define GAME_TYPE_ZZ_258			11		//258���� ��ɳ�齫
#define GAME_TYPE_ZZ_HONGZHONG_GZ	12		//�����淨 ��������2�� ���ɽ���

#define	GAME_TYPE_MJ_MAX_WANFA		2		//����淨����

#define	GAME_TYPE_LESS_HU_ONE		1		//һƬ���
#define	GAME_TYPE_LESS_HU_TW0		2		//��Ƭ���


#define GAME_TYPE_ZZ_LIUJU_4CARD	13		//ʣ��4�Ż�ׯ
#define GAME_TYPE_ZZ_ZHANIAO1		14		//����1��
#define GAME_TYPE_ZZ_ZHANIAO_DOUBLE	15		//���񷭱�
#define GAME_TYPE_ZZ_ZHANIAO_DIFF	16		//����ֿ���
#define GAME_TYPE_ZZ_ZHANIAO_2FEN	17		//����+2��
#define GAME_TYPE_ZZ_PIAO			18		//Ʈ
#define GAME_TYPE_BU_NIAO			19		//�Ʋ�������





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
#define SUB_S_LISTEN				103									//�û�����
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_REPLACE_CARD			108									//�û�����
#define SUB_S_CHI_HU				109								//
#define SUB_S_GANG_SCORE			110									//
#define	SUB_S_GAME_END_LAST_CARD	111
#define SUB_S_MASTER_HANDCARD		120									//
#define SUB_S_MASTER_LEFTCARD		121									//ʣ���ƶ�


//���ƹ���
//����
#define CHK_NULL					0x00										//�Ǻ�����
#define CHK_CHI_HU				0x01										//������

// ���
#define CHR_PENGPENG_HU						0x00000001									//������
#define CHR_JIANGJIANG_HU					0x00000002									//������
#define CHR_QING_YI_SE						0x00000004									//��һɫ
#define CHR_HAI_DI_LAO						0x00000008									//������
#define CHR_HAI_DI_PAO						0x00000010									//������
#define CHR_QI_XIAO_DUI						0x00000020									//��С��
#define CHR_HAOHUA_QI_XIAO_DUI				0x00000040									//������С��
//#define CHR_GANG_KAI						0x00000080									//���Ͽ���
#define CHR_QIANG_GANG_HU					0x00000100									//���ܺ�
#define CHR_GANG_SHANG_PAO					0x00000200									//������
#define CHR_QUAN_QIU_REN					0x00000400									//ȫ����
#define CHR_SHUANG_HAOHUA_QI_XIAO_DUI		0x00000800									//˫������С��
#define CHR_HONGZHONG_WU					0x00001000									//�޺��ж��һ��
#define CHR_GANG_SHUANG_KAI					0x00002000									//��ɳ˫�ܿ�
#define WIK_BU_ZHANG						0x00004000									//����
//С��
#define CHR_XIAO_DA_SI_XI					0x00004000									//����ϲ
#define CHR_XIAO_BAN_BAN_HU					0x00008000									//����
#define CHR_XIAO_QUE_YI_SE					0x00010000									//ȱһɫ
#define CHR_XIAO_LIU_LIU_SHUN				0x00020000									//����˳


#define CHR_ZI_MO							0x01000000									//����
#define	WIK_ZI_MO							CHR_ZI_MO
#define CHR_SHU_FAN							0x02000000									//�ط�
#define CHR_TIAN_HU							0x04000000									//���

#define SUB_S_ANDROID_RULE         200                                 //���͹����������

//��Ϸ״̬
struct CMD_S_StatusFree
{
	DWORD							lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	DWORD							lCellScore;									//��Ԫ����
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	WORD							wReplaceUser;								//�����滻�û�
	BYTE							cbQuanFeng;									//Ȧ��
	BYTE							cbChangeCard;								//��ͼ����
	BYTE							cbLandCount;								//ׯ�Ҵ���
	WORD							wCaiShenIndex;								//����λ��
	WORD							wCaiShenChairID;							//����λ��
	BYTE							cbLeftCardCount;							//ʣ����Ŀ
	//״̬����
	BYTE							cbActionCard;								//�����˿�
	BYTE							cbActionMask;								//��������
	BYTE							cbHearStatus[GAME_PLAYER];					//����״̬
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
	WORD							wWinOrder[GAME_PLAYER];						//

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
	DWORD							lSiceCount;									//���ӵ���
	WORD							wFirstUser;									//�����û�
	WORD							wLandSice;									//ׯ������
	WORD							wCaiShenSice;								//�������
	WORD							wCaiShenIndex;								//����λ��
	WORD							wCaiShenChairID;							//����λ��
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	WORD							wReplaceUser;								//�����û�
	BYTE							cbUserAction;								//�û�����
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbQuanFeng;									//Ȧ��
	WORD							wHeapHead;									//������ͷ
	WORD							wHeapTail;									//������β
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ
	BYTE							cbChangeCard;								//��ͼ����
	BYTE							cbLandCount;								//ׯ�Ҵ���
	BYTE							cbLeftCardCount;							//ʣ����Ŀ����
	BYTE							cbLessHu;									//��Ƭ�����1ƬOR��Ƭ��
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
	BYTE							cbSendCardUser;						//�����û�
	BYTE							cbGangCardIndeed;					//ʵ�ʸ���
	bool							bTail;								//ĩβ����
};

//�ܷ�
struct CMD_S_GangScore
{
	WORD							wChairId;							//
	BYTE							cbXiaYu;							//
	DWORD						lGangScore[GAME_PLAYER];			//
};


//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
	BYTE							cbGangCardData[4];
	BYTE							cbGangCardCount;
	BYTE cbCardEyeCount;	//���Ƶȼ�			0û��1����2˫��3����
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
	DWORD							lGameTax;							//��Ϸ˰��
	//������Ϣ
	WORD							wProvideUser[GAME_PLAYER];						//��Ӧ�û�
	BYTE							cbProvideCard;						//��Ӧ�˿�
	DWORD							dwChiHuKind[GAME_PLAYER];			//��������
//	DWORD							dwChiHuRight[MAX_RIGHT_COUNT];		//��������
	DWORD							dwChiHuRight1[MAX_RIGHT_COUNT];		//��
	DWORD							dwChiHuRight2[MAX_RIGHT_COUNT];		//��
	BYTE							cbHuaCardCount;						//���Ƹ���
	BYTE							cbFanCount;							//�ܷ���

	//������Ϣ
	BYTE							cbFollowCard[GAME_PLAYER][20];		//�����˿�
	BYTE							cbFollowCount[GAME_PLAYER];			//������Ŀ
	DWORD							lOutMagicScore[GAME_PLAYER];		//������
	DWORD							lFollowScore[GAME_PLAYER];			//��������
	DWORD							lHuScore[GAME_PLAYER];				//���ƻ���
	DWORD							lFanHuScore[GAME_PLAYER];			//�ź�����

	//������Ϣ
	bool							bSecondMagic;
	int								lGameScore[GAME_PLAYER];			//��Ϸ����
	
	DWORD							dwStartHuRight[GAME_PLAYER];		//XX���
	
	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//�˿�����
	BYTE							cbChiHuCardData[GAME_PLAYER][MAX_WEAVE];

	bool                            bHasCSGang[GAME_PLAYER];
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�
	BYTE							cbWeaveCount[GAME_PLAYER];
	DWORD							lStartHuScore[GAME_PLAYER];
	int								lGangScore[GAME_PLAYER];
	BYTE							cbWinerMakeEye;					//Ӯ���ν�
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
//�û��й�

//
struct CMD_S_ChiHu
{
	WORD							wChiHuUser;							//
	WORD							wProviderUser;						//
	BYTE							cbChiHuCard;						//
	BYTE							cbCardCount;						//
	DWORD						lGameScore;							//
	BYTE							cbWinOrder;							//
};

struct CMD_S_AndroidRule
{
	BYTE							cbGameTypeIdex;	//��Ϸ����
	DWORD							dwGameRuleIdex;	//��Ϸ����
	BYTE							cbMagicIndex;	//������
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_LISTEN				2									//�û�����
#define SUB_C_REPLACE_CARD			5									//�û�����
#define SUB_C_MASTER_LEFTCARD		6									//ʣ���ƶ�
#define SUB_C_MASTER_CHEAKCARD		7									//ѡ�����
#define SUB_C_MAKE_CARDEYE			122									//����
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

struct MaterCheckCard
{
	BYTE							cbCheakCard;						//��������
};

struct MaterNiaoCout
{
	BYTE							cbNiaoCout;							//��������
};

struct MasterLeftCard
{
	BYTE      kMasterLeftIndex[MAX_INDEX];
	BYTE      kMasterCheakCard;
};
#pragma pack()
#endif
