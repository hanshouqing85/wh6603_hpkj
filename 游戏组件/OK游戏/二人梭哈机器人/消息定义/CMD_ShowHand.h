
#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							201								//��Ϸ��ʶ
#define GAME_PLAYER						2									//��Ϸ����
#define GAME_NAME						TEXT("��������")					//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)					//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)					//����汾
#define	GAME_FOLDER						TEXT("ShowHandTwo")
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//��Ϸ״̬
#define GS_TK_FREE					GAME_STATUS_FREE                        //�ȴ���ʼ
#define GS_TK_CALL					GAME_STATUS_PLAY						//��ׯ״̬
#define GS_TK_SCORE					GAME_STATUS_PLAY+1						//��ע״̬
#define GS_TK_PLAYING				GAME_STATUS_PLAY+2						//��Ϸ����
//////////////////////////////////////////////////////////////////////////
//�����붨��

#define SUB_C_ADD_GOLD					1									//�û���ע
#define SUB_C_GIVE_UP					2									//������ע
#define SUB_C_USE_WINNER                3                                   //����ʤ��
#define SUB_C_CHECK_SUPER				4									//�����û��ж�
#define SUB_C_SET_CARD					5                                   //ȡ������
#define SUB_C_SHOW_CARD					6									//������Ϣ
		
#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_GOLD					101									//��ע���
#define SUB_S_GIVE_UP					102									//������ע
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����
#define SUB_S_ANDROID_WINNER			105									// ֪ͨ�����ˣ����������
#define SUB_S_SHOW_CARD					106									//����
#define	SUB_S_SEND_HUMAN_CARD			107									//�������˵�����

#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif
//////////////////////////////////////////////////////////////////////////
//��Ϣ�ṹ��

//��Ϸ״̬
struct CMD_S_StatusFree
{
	__int64								dwBasicGold;						//�������
	bool								bPoor;								//�Ƿ��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD				 				wCurrentUser;						//��ǰ���
	BYTE								bPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE								bTableCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE								bTableCardArray[GAME_PLAYER][5];	//�˿�����
	__int64								lBasicGold;							//�������
	__int64								lTurnMaxGold;						//�����ע
	__int64								lTurnBasicGold;						//������ע
	__int64								lTableGold[2*GAME_PLAYER];			//������
	__int64								lShowHandScore;						//
	BYTE								bShowHand;							//�Ƿ����
	bool								bPoor;								//�Ƿ��������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE								bFundusCard;						//�����˿�
	WORD				 				wCurrentUser;						//��ǰ���
	__int64								lTurnMaxGold;						//�����ע
	__int64								lTurnBasicGold;						//������ע
	__int64								lBasicGold;							//��Ԫ��ע
	BYTE								bCardData[GAME_PLAYER];				//�û��˿�
	__int64								lShowHandScore;						//
	bool								bPoor;								//�Ƿ��������
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wUserChairID;						//�����û�
	__int64								lTurnMaxGold;						//
};

//�û���ע
struct CMD_C_AddGold
{
	bool								bFllow;								//�Ƿ��
	__int64								lGold;								//��ע��Ŀ
};
//����
struct CMD_C_ShowCard
{
	WORD								wChairID;                           //�û�����ID
};
//��ע���
struct CMD_S_AddGold
{
	WORD								wLastChairID;						//��һ�û�
	WORD								wCurrentUser;						//��ǰ�û�
	__int64								lLastAddGold;						//��ע��Ŀ
	__int64								lCurrentLessGold;					//���ټ�ע
	bool								bShowHand;							//�Ƿ����
	bool								bFllow;								//�Ƿ��
};

//�������ݰ�
struct CMD_R_SendCard
{
	__int64								lMaxGold;							//�����ע
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wStartChairId;						//
	BYTE								cbSendCardCount;					//
	BYTE								bUserCard[GAME_PLAYER][2];			//�û��˿�
	WORD								wWin;
};

//�����˵����Ƹ�������
struct	CMD_S_ShowHumanCard
{
	WORD								wChairID;							//����������λ��
	BYTE								cbUserCard[5];						//������Ϣ
	BYTE								cbCardCount;						//���Ƶ�����
};

//��Ϸ����
struct CMD_S_GameEnd
{
	__int64								lTax;								//��Ϸ˰��
	__int64								lGameGold[GAME_PLAYER];				//��Ϸ�÷�
	BYTE								bUserCard[GAME_PLAYER];				//�û��˿�
};

struct CMD_S_AndroidWinner
{
	WORD              wWinner;  // ����Ӯ�����λ��
};

struct CMD_C_UseWinner
{
	WORD              wWinner;  // ����Ӯ�����λ��
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()

#endif
