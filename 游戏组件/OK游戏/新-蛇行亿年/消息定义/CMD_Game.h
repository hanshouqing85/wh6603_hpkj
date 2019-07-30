/*****************************************************/
//����Ϸ������:��Ƥ�Ƽ�,��ϵQQ:56248
/*****************************************************/

#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#pragma pack(1)
/////////////////////////////////////////////////////////////////////////////////
//������
//��Ϸ����
#define KIND_ID						2013 								//��Ϸ I D
#define GAME_NAME					TEXT("��������")					//��Ϸ����
#define	FISHCOUNT					32									//������

//�������
#define GAME_PLAYER					6									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_CALL				GAME_STATUS_PLAY					//�з�״̬
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//��Ϸ����

//�����
enum enServerSubCmd
{
	enServerSubCmd_Begin=59,
	SUB_S_GAME_SCENE,									//������Ϣ
	SUB_S_TRACE_POINT,									//�켣����
	SUB_S_USER_SHOOT,									//�����ڵ�
	SUB_S_CAPTURE_FISH,									//������Ⱥ
	SUB_S_BULLET_COUNT,									//�ӵ���Ŀ
	SUB_S_COIN_COUNT,									//�����Ŀ
	SUB_S_FISH_COUNT,									//��Ⱥ��Ŀ
	SUB_S_CHANGE_SCENE,									//�л�����
	SUB_S_SCORE_INFO,									//������Ϣ
	SUB_S_SET_CELL_SCORE,								//��Ԫ����
	SUB_S_BONUS_INFO,									//�ʽ���Ϣ
	SUB_S_GAME_MESSAGE,									//��Ϸ��Ϣ
    SUB_S_LOCKFISH,
	SUB_S_ANDROID_CELL,									//������CellScore

	enServerSubCmd_End
};
//��ʼ�켣��ͽǶ�
struct FishTrace
{
     float x;    //���X����
	 float y;    //Y����
	 float rotation;//��ǰ�Ƕ�
	 float movetime;//�ƶ�ʱ�䱣������켣�˶���ʱ��
	 float changetime;//�ı�ʱ�䣬��֮ǰ�ĽǶȸı䵽���ڽǶȹ涨�����ʱ�������
	 bool  m_isHave ;//�����Ƿ���ڣ�Ĭ��Ϊ���ھ����������Ĭ�ϲ�����
	 int   m_fishid;//��������Ӧ��FISH��ID��
	 int   m_fishtype;//��������Ӧ��FISH��ID��
	 int   m_ptindex;
	 int   m_BuildTime;
	 int   m_shootCount;
	 int   m_mustShoot;
 	 int   m_fudaifishtype;//����Ϊ21��22������Ը���С��
	 int   m_speed;


};
//
struct FishPack
{
     int   m_fishid;        //��ID
	 int   m_fishtype;      //������
	 int   m_fishpathindex; //��켣
	 int   m_fudaifishtype; //���Ƿ�ȦΪ-1��ʾ����
	 int   m_BuildTime;
	 int   m_CreatDelayTime;
	 int   m_SameFishID;
};

//��Ϣ���ݰ�
struct CMD_S_GameMessage
{
	WORD								wMessageLength;					//��Ϣ����
	TCHAR								szContent[1024];				//��Ϣ����
};

//������Ϣ
struct CMD_S_BonusInfo
{
	WORD							wChairID;								//�������
	LONG							lBonus;									//�ʽ���Ŀ
};

//��Ԫ����
struct CMD_S_SetCellScore
{
	WORD							wChairID;								//�������
	LONG							lCellScore;								//��Ԫ����
};

//�л�����
struct CMD_S_ChangeScene
{
	WORD     					    SceneIndex;						        //��ǰ����
	WORD                            RmoveID;                                //Ԥ������

};

//�ӵ���Ŀ
struct CMD_S_BulletCount
{
	WORD							wChairID;								//�������
	bool    				        isaddorremove;						    //�ӵ�����
	LONG							score;							//�ӵ���Ŀ
};
//�ӵ���Ŀ
struct CMD_S_AndroidCellScore
{
	LONG							score;							//�����˻�����
};

//��Ⱥ��Ŀ
struct CMD_S_CptrFishCount
{
	int    						    FishKind;								//��Ⱥ����
	WORD							wFishCount;								//��Ⱥ��Ŀ
};

//�����Ŀ
struct CMD_S_CoinCount
{
	WORD							wChairID;								//�������
	LONG							lCoinCount;								//�����Ŀ
};

//������Ϣ
struct CMD_S_GameScene
{
	LONG							lBulletCharge;	//�ӵ�����
	int     				   	    CureSeaSceneKind;						//��ǰ����
	LONG							m_lUserAllScore[6];			    //��ҷ���
	LONG							lUserCellScore[6];			//��Ԫ�ڵı�����
	float                           powercount[6];
};

//��Ϸ״̬
struct CMD_S_FishTrace
{
	int							    bRegular;								//�����ʶ
	FishPack                        m_fishtrace;
};


//�����ڵ�
struct CMD_S_UserShoot
{
	WORD							wChairID;								//�������
	float							fAngle;									//����Ƕ�
	int 				            BulletCountKind;						//�ӵ�����
	bool							byShootCount;							//�ӵ���Ŀ	
	int                             dwUserScore;
	int                             dwZidanID;
	float                           powercount;
	bool                            superpao;
};
//��Ԫ����
struct CMD_S_LockFishId
{
	LONG							lFishID;								//��Ԫ����
	int                             iChairID;
};
//������Ⱥ
struct CMD_S_CaptureFish
{
	WORD							wChairID;								//�������
	DWORD							dwFishID;								//��Ⱥ��ʶ
	int    						    FishKindscore;								//��Ⱥ����
	LONG							lFishScore;								//��Ⱥ�÷�
	bool                            m_canSuperPao;                          //������
	int                             dwUserScore;
};
//////////////////////////////////////////////////////////////////////////
//�����

enum enClientSubCmd
{
	enClientSubCmd_Begin=60,
	SUB_C_BUY_BULLETSPEED,								//�����ӵ�
	SUB_C_BUY_BULLET,									//�����ӵ�
	SUB_C_USER_SHOOT,									//�����ڵ�
	SUB_C_CAPTURE_FISH,									//������Ⱥ
	SUB_C_GET_SCORE_INFO,								//������Ϣ
	SUB_C_CALC_SCORE,									//�������
	SUB_C_SET_CELL_SCORE,								//��Ԫ����
	SUB_C_HIT_FISH,										//������Ⱥ
	SUB_C_SET_PROPABILITY,								//���ø���
    SUB_C_SET_COMEINUSER,								//���ø���
	SUB_C_LOCKFISH,                                     //��������
	enClientSubCmd_End
};
//��Ԫ����
struct CMD_C_LockFishId
{
	LONG							lFishID;								//��Ԫ����
	LONG                            lChairID;
};
//��Ԫ����
struct CMD_C_SetCellScore
{
	LONG							lCellScore;								//��Ԫ����
};

//�����ӵ�
struct CMD_C_BuyBullet
{
	int  				           score;//���·���						//��Ŀ����
	bool                           addormove;//�ϻ�������
	bool						   allscore;
};

//�����ڵ�
struct CMD_C_UserShoot
{
	int    				            BulletCountKind;						//�ӵ�����
	bool							byShootCount;							//�����Ŀ
	float							fAngle;									//����Ƕ�
	DWORD							dwBulletID;								//�ӵ���ʶ

};

//������Ⱥ
struct CMD_C_HitFish
{
	DWORD							dwFishID;								//��Ⱥ��ʶ
	DWORD							dwBulletID;								//�ӵ���ʶ
	int                             bulletuserid;
	bool                            boolisandroid;

};
struct CMD_C_CoinCount
{
//	WORD							wChairID;								//�������
	LONG							lCoinCount;								//�����Ŀ
};
//���ø���
struct CMD_C_SetProbability
{
	BYTE							byCptrProbability;	//�������
};
//
struct CM_C_ComeInNow
{
    LONG                           lHaveCome  ;//
};

//////////////////////////////////////////////////////////////////////////

#endif