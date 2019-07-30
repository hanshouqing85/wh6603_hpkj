#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE


//////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						25									//��Ϸ I D
#define GAME_NAME					TEXT("�°�����")						//��Ϸ����



#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#define myatoi      _wtoi
#define myatoi64	_wtoi64
#endif

//�������
#define GAME_PLAYER					5									//��Ϸ����

#ifdef DEBUG
	#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
	#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾
#else
	#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
	#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾
#endif
//////////////////////////////////////////////////////////////////////////

//����ԭ��
#define GER_NO_PLAYER				0x10								//û�����

//״̬����
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//��Ϸ����


//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

//���ư�ť
#define IDC_START						100								//��ʼ��ť
#define IDC_ADD_SCORE_ONE				101
#define IDC_ADD_SCORE_TWO				102
#define IDC_ADD_SCORE_THREE				103
#define IDC_ADD_SCORE_FOUR				104
#define IDC_ADD_SCORE					105
#define IDC_CONTROL_FOLLOW				106
#define IDC_CONTROL_GIVEUP				107 
#define IDC_CONTROL_SHOWHAND			108	
#define IDC_CONTROL_ADD					109	
#define IDC_CONTROL_SUB     			110
//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE				101									//�û���ע
#define SUB_S_GIVE_UP				102									//�û�����
#define SUB_S_SEND_CARD				103									//�����˿�
#define SUB_S_GAME_END				104									//��Ϸ����
#define SUB_S_GET_WINNER			105									//��ȡ��Ϣ
#define SUB_S_ANDROID_GET_CARD		112									//������ר������:��ȡ�˿�

struct CMD_S_GetAllCard
{
	BYTE	cbCardData[GAME_PLAYER][5];
};
//����״̬
struct CMD_S_StatusFree
{
	//��Ϸ����
	SCORE							lCellScore;							//��������

	//��ʷ����
	SCORE							lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE							lCollectScore[GAME_PLAYER];			//������Ϣ
	
		//������Ϣ
	TCHAR							szGameRoomName[32];				//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	SCORE							lCellScore;							//��������

	//��ע��Ϣ
	SCORE							lDrawMaxScore;						//�����ע
	SCORE							lTurnMaxScore;						//�����ע
	SCORE							lTurnLessScore;						//��С��ע
	SCORE							lUserScore[GAME_PLAYER];			//�û���ע
	SCORE							lTableScore[GAME_PLAYER];			//������ע

	//״̬��Ϣ
	BYTE							cbShowHand;							//�����־
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbHandCardData[GAME_PLAYER][5];		//�����˿�

	//��ʷ����
	SCORE							lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE							lCollectScore[GAME_PLAYER];			//������Ϣ
	
		//������Ϣ
	TCHAR							szGameRoomName[32];				//��������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��Ϸ����
	SCORE							lCellScore;							//��Ԫ��ע

	//��ע��Ϣ
	SCORE							lDrawMaxScore;						//�����ע
	SCORE							lTurnMaxScore;						//�����ע
	SCORE							lTurnLessScore;						//��С��ע

	//�û���Ϣ
	WORD				 			wCurrentUser;						//��ǰ���

	//�˿�����
	BYTE							cbObscureCard;						//�����˿�
	BYTE							cbCardData[GAME_PLAYER];			//�û��˿�


	BYTE							cbPlayStatus[GAME_PLAYER];
};

//��Ϸ��ʼ
struct CMD_S_GameStart1
{
	//��Ϸ����
	SCORE							lCellScore;							//��Ԫ��ע

	//��ע��Ϣ
	SCORE							lDrawMaxScore;						//�����ע
	SCORE							lTurnMaxScore;						//�����ע
	SCORE							lTurnLessScore;						//��С��ע

	//�û���Ϣ
	WORD				 			wCurrentUser;						//��ǰ���

	//�˿�����
	BYTE							cbObscureCard;						//�����˿�
	BYTE							cbCardData[GAME_PLAYER];			//�û��˿�


	BYTE							cbPlayStatus[GAME_PLAYER];
	
	BYTE							cbTableCardData[GAME_PLAYER][5];	//�����˿�
	bool							bWillWin;							//���ƽ��

};


//�û�����
struct CMD_S_GiveUp
{
	WORD							wGiveUpUser;						//�����û�
	WORD							wCurrentUser;						//��ǰ�û�
	SCORE							lDrawMaxScore;						//�����ע
	SCORE							lTrunMaxScore;						//�����ע
};

//�û���ע
struct CMD_S_AddScore
{
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wAddScoreUser;						//��ע�û�
	SCORE							lTurnLessScore;							//���ټ�ע
	SCORE							lUserScoreCount;						//��ע��Ŀ
};

//�����˿�
struct CMD_S_SendCard
{
	//��Ϸ��Ϣ
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wStartChairID;						//��ʼ�û�
	SCORE							lTurnMaxScore;						//�����ע

	//�˿���Ϣ
	BYTE							cbSendCardCount;					//������Ŀ
	BYTE							cbCardData[2][GAME_PLAYER];			//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbCardData[GAME_PLAYER];			//�û��˿�
	SCORE							lGameScore[GAME_PLAYER];			//��Ϸ����
};


//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_C_GIVE_UP				1									//�û�����
#define SUB_C_ADD_SCORE				2									//�û���ע
#define SUB_C_GET_WINNER			3									//��ȡ��Ϣ

//�û���ע
struct CMD_C_AddScore
{
	SCORE						lScore;								//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

static SCORE KeepOne(SCORE score)
{
	return ((LONGLONG)((score+0.000001)*100)/10)/10.000;
};


//////////////////////////////////////////////////////////////////////////
//���ƶ���
#define SUB_C_AMDIN_COMMAND			5									//����Ա����

#define RQ_OPTION_CANCLE		1		//ȡ��
#define RQ_OPTION_QUERYING		2		//��ѯ
#define RQ_OPTION_SETING		3		//����

#define CHEAT_TYPE_LOST		0		//��
#define CHEAT_TYPE_WIN		1		//Ӯ

struct CMD_C_AdminReq
{
	BYTE cbReqType;							//��������
	BYTE cbCheatCount;						//���ƴ���
	BYTE cbCheatType;						//��������
	DWORD dwGameID;							//��ұ�ʶ
};
//////////////////////////////////////////////////////////////////////////
#endif