#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						104									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("����ţţ")					//��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//����״̬
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+2					//����״̬

#define VERSION_SERVER			    PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//��������
#define ID_TIAN_MEN					1									//��
#define ID_DI_MEN					2									//��
#define ID_XUAN_MEN					3									//��
#define ID_HUANG_MEN				4									//��

#define SCORE						DOUBLE								//��������
#define SCORE_STRING				TEXT("%0.2f")						//��������
#define SCORE_ZERO                  1.175494351e-38F                    //��������

//�������
#define BANKER_INDEX				0									//ׯ������
#define SHUN_MEN_INDEX				1									//˳������
#define DUI_MEN_INDEX				2									//��������
#define DAO_MEN_INDEX				3									//��������
#define HUAN_MEN_INDEX				4									//��������
#define MAX_INDEX					3									//�������

#define AREA_COUNT					4									//������Ŀ

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

//���ʶ���
#define RATE_TWO_PAIR				12									//��������
#define SERVER_LEN					32									//���䳤��

//��������Ϣ
struct tagRobotInfo
{
	int nChip[8];														//���붨��
	int nAreaChance[AREA_COUNT];										//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[8] = {1, 5, 10, 50, 100, 200, 500, 1000};
		int nTmpAreaChance[AREA_COUNT] = {1, 1, 1, 1};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("OxBattleConfig.ini");

		nMaxTime = 5;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	bool							bWinShunMen;						//˳��ʤ��
	bool							bWinDuiMen;							//����ʤ��
	bool							bWinDaoMen;							//����ʤ��
	bool							bWinHuang;							//����ʤ��
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������
#define SUB_S_SEND_ACCOUNT			109									//�����˺�
#define SUB_S_ADMIN_CHEAK			111									//��ѯ�˺�


#define SUB_S_AMDIN_COMMAND			110									//����Ա����

//����ظ�
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//�ظ�����
		#define ACK_SET_WIN_AREA  1
		#define ACK_PRINT_SYN     2
		#define ACK_RESET_CONTROL 3
	BYTE cbResult;
		#define CR_ACCEPT  2			//����
		#define CR_REFUSAL 3			//�ܾ�
	BYTE cbExtendData[20];			//��������
};
//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//��ע���
	BYTE							lJettonArea;						//��ע����
	LONGLONG						lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	DOUBLE							lScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	DOUBLE							lCurrentBankerScore;				//ׯ�ҷ���
};

//��ׯ�б����
struct tagBankerListPlayer
{
	tagBankerListPlayer(){memset(this, 0, sizeof(*this));}
    WORD wChairID;
	DWORD dwUserID;
    DOUBLE lScore;
	BYTE cbGender;
    TCHAR szNickName[32];
};

//��ׯ�б�
struct CMD_S_ApplyBankerList
{
	CMD_S_ApplyBankerList(){memset(this, 0, sizeof(*this));}
	BYTE cbCount;//	��ׯ�б���Ҹ���
	inline tagBankerListPlayer & operator [](unsigned int);
	int getLength()const{return sizeof(*this) + cbCount * sizeof(tagBankerListPlayer);}
};

struct CMD_S_ApplyBankerListMax:public CMD_S_ApplyBankerList
{
	tagBankerListPlayer wApplyList[GAME_PLAYER];
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	TCHAR							szCancelUser[32];					//ȡ�����
	WORD							wChairID;							//�û�����ID
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	TCHAR							szBankerNickName[32];				//	ׯ���ǳ�
	BYTE							cbBankerGender;						//	ׯ���Ա�
	LONGLONG						lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONGLONG						lUserMaxScore;						//��ҽ��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	TCHAR							szBankerNickName[32];				//	ׯ���ǳ�
	BYTE							cbBankerGender;						//	ׯ���Ա�
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ�ҳɼ�
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	LONGLONG						lAllJettonScore[AREA_COUNT+1];		//ȫ����ע

	//�����ע
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//������ע

	//��һ���
	LONGLONG						lUserMaxScore;						//�����ע							

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbTableCardArray[5][5];				//�����˿�

	bool							bResult[AREA_COUNT];				//	ʤ�����

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	TCHAR							szBankerNickName[32];				//	ׯ���ǳ�
	BYTE							cbBankerGender;						//	ׯ���Ա�
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lEndBankerScore;					//ׯ�ҳɼ�
	LONGLONG						lEndUserScore;						//��ҳɼ�
	LONGLONG						lEndUserReturnScore;				//���ػ���
	LONGLONG						lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	INT64                           nListUserCount;						//�б�����
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ��λ��
	LONGLONG						lBankerScore;						//ׯ�ҽ��
	LONGLONG						lUserMaxScore;						//�ҵĽ��
	BYTE							cbTimeLeave;						//ʣ��ʱ��	
	bool							bContiueCard;						//��������
	int								nChipRobotCount;					//�������� (��ע������)
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	bool							bIsAndroid;							//�Ƿ������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	BYTE							cbTableCardArray[5][5];				//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	bool							bResult[AREA_COUNT];				//	ʤ�����

	BYTE							bcFirstCard[2];
 
	//ׯ����Ϣ
	SCORE						lBankerScore;						//ׯ�ҳɼ�
	SCORE						lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	SCORE						lUserScore;							//��ҳɼ�
	SCORE						lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	SCORE						lRevenue;							//��Ϸ˰��
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_CONTINUE_CARD			4									//��������
#define SUB_C_AMDIN_COMMAND			5									//����Ա����
#define SUB_C_GET_ACCOUNT			6									//��ȡ�ǳ�
#define SUB_C_CHEAK_ACCOUNT			7									//��ȡ�ǳ�
#define SUB_C_GET_PLAYLIST			8									//��ׯ�б�

//�ͻ�����Ϣ
#define IDM_ADMIN_COMMDN			WM_USER+1000
#define IDM_GET_ACCOUNT				WM_USER+1001
#define IDM_CHEAK_ACCOUNT			WM_USER+1002

struct CMD_C_SendAccount
{
	DWORD							m_UserID[GAME_PLAYER];					//���ID
	DWORD							m_UserCount;							//�������
};
struct CMD_S_UserJettonScore
{
	//�����ע
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//������ע
};
//����������Ϣ
struct tagControlInfo
{
	BYTE cbControlArea[MAX_INDEX];			//��������
};

struct CMD_C_CheakAccount  //
{
	TCHAR							dwUserAccount[32];					//ȡ�����
};

struct tagAdminReq
{
	BYTE							m_cbExcuteTimes;					//ִ�д���
	BYTE							m_cbControlStyle;					//���Ʒ�ʽ
#define		CS_BANKER_LOSE    1
#define		CS_BANKER_WIN	  2
#define		CS_BET_AREA		  3
	bool							m_bWinArea[3];						//Ӯ������
};


struct CMD_C_AdminReq
{
	BYTE cbReqType;
#define RQ_SET_WIN_AREA		1
#define RQ_RESET_CONTROL	2
#define RQ_PRINT_SYN		3
	BYTE cbExtendData[20];			//��������
};
//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
