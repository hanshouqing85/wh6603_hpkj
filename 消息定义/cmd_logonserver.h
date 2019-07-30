#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GP_LOGON				1									//�㳡��¼

//��¼ģʽ
#define SUB_GP_LOGON_ACCOUNTS		1									//�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS	2									//ע���ʺ�

#define SUB_GP_GET_CP_USERINFO		103									//��ѯ��Ʊ�û���Ϣ
#define SUB_GP_GET_CP_USERINFO_RET		104									//��ѯ��Ʊ�û���Ϣ

//��¼���
#define SUB_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GP_LOGON_FINISH			102									//��¼���

#define SUB_GP_KILL_SOCKET			644									//ǿ������
//������ʾ
#define SUB_GP_UPDATE_NOTIFY		200									//������ʾ
// #define SUB_GP_GET_CP_USERINFO		103									//��ȡ��Ʊ�û���Ϣ

//////////////////////////////////////////////////////////////////////////////////

//�ʺŵ�¼
struct CMD_GP_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	//�γ��������
	TCHAR							szQQ[LEN_QQ];		//QQ����
	TCHAR							szEmain[LEN_EMAIL];	//E-Mail
	TCHAR							szIPhone[LEN_MOBILE_PHONE];	//�ֻ�����

};

//�ʺŵ�¼
struct CMD_GP_GetCpUserInfo
{
	int								n_userid;
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwExperience;						//������ֵ

	//�û��ɼ�
	SCORE							lScore;								//�û�����
	SCORE							lInsure;							//�û�����
	SCORE                           lUserMedal;                         //�û����� 

	//�û���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //��ַ����

	int								n_type;
	DOUBLE							f_fandian;
	DOUBLE							f_dongjie;
	DOUBLE							f_yue;

	//�γ�����
	BYTE							cbShowServerStatus;					//�Ƿ���ʾ��������
};

//��¼ʧ��
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��½���
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//�ж�ʱ��
	WORD							wOnLineCountTime;					//����ʱ��
};

//������ʾ
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};


struct CMD_GP_KillSocket
{
	BYTE							cbResult;
	TCHAR							szMessage[256];
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//������Ϣ
#define DTP_GP_MEMBER_INFO			2									//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3									//����ǩ��
#define DTP_GP_STATION_URL			4									//��ҳ��Ϣ

//������Ϣ
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��Ա��Ϣ
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_GP_SERVER_LIST			2									//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1									//��ȡ�б�
#define SUB_GP_GET_ONLINE			2									//��ȡ����

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100									//�����б�
#define SUB_GP_LIST_KIND			101									//�����б�
#define SUB_GP_LIST_SERVER			102									//�����б�
#define SUB_GP_LIST_FINISH			103									//�������

//������Ϣ
#define SUB_GR_KINE_ONLINE			300									//��������
#define SUB_GR_SERVER_ONLINE		301									//��������

//////////////////////////////////////////////////////////////////////////////////

//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GP_USER_SERVICE			3									//�û�����

//�˺ŷ���
#define SUB_GP_MODIFY_MACHINE		100									//�޸Ļ���
#define SUB_GP_MODIFY_LOGON_PASS	101									//�޸�����
#define SUB_GP_MODIFY_INSURE_PASS	102									//�޸�����

// #define SUB_GP_GET_CP_USERINFO		103									//��ѯ��Ʊ�û���Ϣ
// #define SUB_GP_GET_CP_USERINFO_RET		104									//��ѯ��Ʊ�û���Ϣ

//�޸�ͷ��
#define SUB_GP_USER_FACE_INFO		200									//ͷ����Ϣ
#define SUB_GP_SYSTEM_FACE_INFO		201									//ϵͳͷ��

//��������
#define SUB_GP_USER_INDIVIDUAL		301									//��������
#define	SUB_GP_QUERY_INDIVIDUAL		302									//��ѯ��Ϣ
#define SUB_GP_MODIFY_INDIVIDUAL	303									//�޸�����

//���з���
#define SUB_GP_USER_SAVE_SCORE		400									//������
#define SUB_GP_USER_TAKE_SCORE		401									//ȡ�����
#define SUB_GP_USER_INSURE_INFO		402									//��������
#define SUB_GR_USER_TRANSFER_SCORE  403 								//ȡ�����
#define SUB_GP_QUERY_INSURE_INFO	404									//��ѯ����
#define SUB_GP_USER_INSURE_SUCCESS	405									//���гɹ�
#define SUB_GP_USER_INSURE_FAILURE	406									//����ʧ��
//��Ϸ������ѯ
#define SUB_GP_QUERY_GAME_RESULT	500									//��ѯ�������
#define SUB_GP_QUERY_GAME_RET		501									//��ѯ�������
//��ѯϵͳʱ��
#define	SUB_GP_QUERY_TIME			502									//��ѯʱ��
#define	SUB_GP_QUERY_TIME_RET		503									//��ѯʱ��
//����ʱʱ����ע
#define	SUB_GP_TOUZHU_CQSSC			504									//��ѯʱ��
#define	SUB_GP_TOUZHU_CQSSC_RET		505									//��ѯʱ��
#define	SUB_GP_TOUZHU_CQSSC_DAN		908									//��ʽ��ע
#define	SUB_GP_TOUZHU_CONTINUE_RET		407									//��ѯʱ��
#define	SUB_GP_TOUZHU_CQSSC_ZHUIHAO		408									//��ʽ��ע
#define	SUB_MB_TOUZHU_CQSSC_ZHUIHAO		636									//�ֻ�׷��

//��ѯ����
#define	SUB_GP_QUERY_YINHANG			909									//��ѯ����
#define	SUB_GP_QUERY_YINHANG_RET		910									//��ѯ����
//�������
#define SUB_GP_OPERATE_SUCCESS		900									//�����ɹ�
#define SUB_GP_OPERATE_FAILURE		901									//����ʧ��

//��Ϸ������ѯ
#define SUB_GP_GET_CP_USER			902									//��ȡ��Ʊ�����Ϣ
#define SUB_GP_GET_CP_USER_RET		903									//��ȡ��Ʊ�����Ϣ���

//��ѯ����
#define SUB_GP_GET_USER_FANDIAN			904									//��ȡ��Ʊ��ҷ���
#define SUB_GP_GET_USER_FANDIAN_RET		905									//��ȡ��Ʊ��ҷ�����

//���÷���
#define SUB_GP_SET_USER_BONUS			906									//���ò�Ʊ��ҷ���
#define SUB_GP_SET_USER_BONUS_RET		907									//���ò�Ʊ��ҷ�����

//��ȡMAPBONUS
#define SUB_GP_GET_MAP_BONUS			598									//��ȡMAPBONUS
#define SUB_GP_GET_MAP_BONUS_RET		599									//��ȡMAPBONUS���
struct CMD_GP_GetMyMapBonus
{
	int								n_t_userid;
};
#define SUB_GP_GET_CHONGZHI_TISHI	508									//��ֵ��ʾ
#define SUB_GP_GET_CHONGZHI_TISHI_RET		509							//��ֵ��ʾ����

#define SUB_GP_GET_TIXIAN_TISHI		512									//������ʾ
#define SUB_GP_GET_TIXIAN_TISHI_RET		513							//������ʾ����

#define SUB_GP_GET_TOUZHU_TISHI		516									//Ͷע��ʾ
#define SUB_GP_GET_TOUZHU_TISHI_RET		517							//Ͷע��ʾ����

//�����˻�
#define SUB_GP_GET_TOUZHU_LOG_COUNT			506								//��ѯͶע��־��
#define SUB_GR_GET_TOUZHU_LOG_COUNT_RET		507							//��ѯͶע��־������

//��ѯͶע��־��
struct CMD_GP_GetTouzhuLogCount
{
	DWORD							dwUserID;							//�û�ID
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetTouzhuLogCountRet
{
	DWORD							dwCount;							//����
};
#define SUB_GP_GET_TOUZHU_LOG			514								//��ѯͶע��־
#define SUB_GP_GET_TOUZHU_LOG_RET		515								//����

struct	CMD_GP_GetTouzhuLog
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetTouzhuLogRet
{
	int								n_t_id;
	int								n_t_type;
	int								n_t_kind;
	TCHAR							szQishu[30];
	int								n_t_zhushu;
	int								n_t_moshi;
	int								n_t_beishu;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	int								n_t_winzhushu;
	DOUBLE							f_t_yingkui;
	int								n_t_zhuihao;
};

#define SUB_GP_CANCEL_TOUZHU			510								//����
#define SUB_GR_CANCEL_TOUZHU_RET		511								//��������
struct CMD_GP_CancelTouzhu
{
	int								n_t_userid;							//�û�ID
	int								n_t_id;								//ͶעID
	int								n_t_kindid;
	TCHAR							s_t_qihao[30];
};
struct CMD_GR_CancelTouzhuRet
{
	int								nResult;
};
//////////////////////////////////////////////////////////////������־////////////////
//�����˻�
#define SUB_GP_GET_TIXIAN_LOG_COUNT			520								//��ѯ������־��
#define SUB_GR_GET_TIXIAN_LOG_COUNT_RET		521							//��ѯ������־������

//��ѯͶע��־��
struct CMD_GP_GetTixianLogCount
{
	DWORD							dwUserID;							//�û�ID
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetTixianLogCountRet
{
	DWORD							dwCount;							//����
};

//�����˻�
#define SUB_GP_GET_CHONGZHI_LOG_COUNT			522								//��ѯ������־��
#define SUB_GR_GET_CHONGZHI_LOG_COUNT_RET		523							//��ѯ������־������

//��ѯͶע��־��
struct CMD_GP_GetChongzhiLogCount
{
	DWORD							dwUserID;							//�û�ID
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetChongzhiLogCountRet
{
	DWORD							dwCount;							//����
};

//�����˻�
#define SUB_GP_GET_YINGKUI_LOG_COUNT			528								//��ѯӯ����־��
#define SUB_GR_GET_YINGKUI_LOG_COUNT_RET		529							//��ѯӯ����־������

//��ѯͶע��־��
struct CMD_GP_GetYingkuiLogCount
{
	DWORD							dwUserID;							//�û�ID
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetYingkuiLogCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_TIXIAN_LOG			518								//��ѯͶע��־
#define SUB_GP_GET_TIXIAN_LOG_RET		519								//����

struct	CMD_GP_GetTixianLog
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetTixianLogRet
{
	int								n_t_id;
	SYSTEMTIME						n_t_time;
	TCHAR							n_t_yinhang[30];
	DOUBLE							f_t_jine;
	TCHAR							s_t_kaihuren[30];
	int								n_t_state;
	TCHAR							s_t_note[30];
};
///////////////////////ӯ����ϸ///////////////////////////////////////////////////////
//�����˻�
#define SUB_GP_GET_YINGKUI_MX_COUNT			626								//��ѯӯ����־��
#define SUB_GR_GET_YINGKUI_MX_COUNT_RET		627							//��ѯӯ����־������

//��ѯӯ����ϸ
struct CMD_GP_GetYingkuiMxCount
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetYingkuiMxCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_YINGKUI_MX			628								//��ѯӯ����ϸ
#define SUB_GP_GET_YINGKUI_MX_RET		629								//����

struct	CMD_GP_GetYingkuiMx
{
	int							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	int							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetYingkuiMxRet
{
	int								n_t_dingdan;
	DOUBLE							f_t_yingkui;
	DOUBLE							f_t_yue;
	SYSTEMTIME						t_t_yingkuitime;
	TCHAR							s_t_type[30];
	TCHAR							s_t_zhaiyao[126];
};

///////////////////////����ӯ��///////////////////////////////////////////////////////
//�����˻�
#define SUB_GP_GET_QP_YINGKUI_COUNT			411								//��ѯӯ����־��
#define SUB_GR_GET_QP_YINGKUI_COUNT_RET		412							//��ѯӯ����־������

//��ѯӯ����ϸ
struct CMD_GP_GetQiPaiYingkuiCount
{
	DWORD							dwUserID;							//�û�ID
	int								nTypeID;					
	int								nByTime;					
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetQiPaiYingkuiCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_QP_YINGKUI			413								//��ѯӯ����ϸ
#define SUB_GP_GET_QP_YINGKUI_RET		414								//����

struct	CMD_GP_GetQiPaiYingkui
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetQiPaiYingkuiRet
{
	int								n_t_userid;
	int								n_t_drawid;
	DOUBLE							f_t_score;
	int								n_t_playcount;
	DOUBLE							f_t_revenue;
	SYSTEMTIME							n_t_playtime;
	TCHAR							s_t_servername[33];
	TCHAR							s_t_accounts[33];
};

//////////////////////////////////////////////////////////////////////////////////////
#define SUB_GP_GET_CHONGZHI_LOG			524								//��ѯͶע��־
#define SUB_GP_GET_CHONGZHI_LOG_RET		525								//����

struct	CMD_GP_GetChongzhiLog
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetChongzhiLogRet
{
	int								n_t_id;
	DOUBLE							f_t_jine;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	TCHAR							s_t_pingtai[30];
};

#define SUB_GP_GET_YINGKUI_LOG			526								//��ѯӯ����־
#define SUB_GP_GET_YINGKUI_LOG_RET		527								//����

struct	CMD_GP_GetYingkuiLog
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetYingkuiLogRet
{
	DOUBLE							f_t_chongzhi;
	DOUBLE							f_t_qukuan;
	DOUBLE							f_t_touzhu;
	DOUBLE							f_t_fandian;
	DOUBLE							f_t_zhongjiang;
	DOUBLE							f_t_huodong;
	DOUBLE							f_t_chedan;
};

#define SUB_GP_GET_USER_INFO			530								//��ѯӯ����־
#define SUB_GR_GET_USER_INFO_RET		531								//����

struct	CMD_GP_GetUserInfo
{
	DWORD							dwUserID;							//�û�ID
};

struct CMD_GR_GetUserInfoRet
{
	TCHAR							s_t_qq[12];
	SYSTEMTIME						t_reg_time;						// ע��ʱ��
	TCHAR							s_t_sjqq[12];
	DOUBLE							f_t_yongjin;					//Ӷ��
	DOUBLE							f_t_yongjinze;					//Ӷ���ܶ�
	DOUBLE							f_t_fenhong;					//�ֺ�
	DOUBLE							f_t_fenhongze;					//�ֺ��ܶ�
	DOUBLE							f_t_qipaiye;					//�������
	DOUBLE							f_t_kuisunyongjin;					//����Ӷ��
	DOUBLE							f_t_kuisunyongjinze;					//����Ӷ���ܶ�

	SYSTEMTIME						t_ksyj_time;						// ����Ӷ��ʱ��

	SYSTEMTIME						t_fh_time;						//	�ֺ�ʱ�� 
	SYSTEMTIME						t_yj_time;						// Ӷ��ʱ��

};
#define SUB_GP_XG_QQ			532								//�޸�QQ
#define SUB_GR_XG_QQ_RET		533								//����

struct	CMD_GP_XGqq
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szQQ[11];							//��QQ 
};
struct CMD_GR_XG_QQ_RET
{
	LONG							lResult;
};

#define SUB_GP_XG_LOGIN_PASS			534								//�޸ĵ�¼����
#define SUB_GR_XG_LOGIN_PASS_RET		535								//����

struct	CMD_GP_XG_Loginpass
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szOldPass[33];						//������
	TCHAR							szNewPass[33];						//������
};
struct CMD_GR_XG_Loginpass_RET
{
	int							lResult;
};
#define SUB_GP_LOCK_MACHINE			919								//��������
#define SUB_GR_LOCK_MACHINE_RET		920								//����

struct	CMD_GP_LockMachine
{
	DWORD							dwUserID;							//�û�ID
	BYTE						cbLock;									//����
};
struct CMD_GR_LockMachine_RET
{
	int							lResult;
};

#define SUB_GP_XG_QUKUAN_PASS			536								//�޸ĵ�¼����
#define SUB_GR_XG_QUKUAN_PASS_RET		537								//����

struct	CMD_GP_XG_Qukuanpass
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szOldPass[33];						//������
	TCHAR							szNewPass[33];						//������
};
struct CMD_GR_XG_Qukuanpass_RET
{
	int							lResult;
};
#define SUB_GP_SET_QUKUAN_PROTECT			538								//����ȡ���
#define SUB_GR_SET_QUKUAN_PROTECT_RET		539								//����

struct	CMD_GP_SetQukuanProtect
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szQukuanPass[33];						//����
	TCHAR							szQukuanAsk[33];						//����
	TCHAR							szQukuanAnswer[33];						//��
};
struct CMD_GR_SetQukuanProtect_RET
{
	int							lResult;
};

#define SUB_GP_SET_QUKUAN_ZHANGHAO			540								//����ȡ���
#define SUB_GR_SET_QUKUAN_ZHANGHAO_RET		541								//����

struct	CMD_GP_SetQukuanZhanghao
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szKaihuYinghang[33];						//����
	TCHAR							szKaihuRen[33];						//����
	TCHAR							szYinhangZhanghu[33];						//��
	TCHAR							szQukuanPass[33];						//��
};
struct CMD_GR_SetQukuanZhanghao_RET
{
	int							lResult;
};

struct CMD_GR_QueryYinhang_RET
{
	LONG							lResultCode;						//��������
	TCHAR							szYinHangName[128];					//�ɹ���Ϣ
};

#define SUB_GP_QUERY_MY_YINHANG			415								//��ѯ�Լ�������˺�����
#define SUB_GR_QUERY_MY_YINHANG_RET		416								//����

struct	CMD_GP_QueryMyYinHang
{
	DWORD							dwUserID;							//�û�ID
};
struct CMD_GR_QueryMyYinHang_RET
{
	int							lResult;
	TCHAR						szKaihuYinghang[33];
	TCHAR						szKaihuRen[33];
	TCHAR						szYinhangZhanghu[33];				
};

#define SUB_GP_QUERY_MY_PROTECT			423								//��ѯ�Լ�������
#define SUB_GR_QUERY_MY_PROTECT_RET		424								//����

struct	CMD_GP_QueryMyProTect
{
	DWORD							dwUserID;							//�û�ID
};
struct CMD_GR_QueryMyProTect_RET
{
	int							lResult;
	TCHAR						szQuestion[33];
	TCHAR						szAnswer[33];
};

/////////////////��Ա//////////////////////////////////////////////////////////////////
//�����˻�
#define SUB_GP_GET_HYXX_LOG_COUNT			542								//��ѯ��Ա��Ϣ��־��
#define SUB_GR_GET_HYXX_LOG_COUNT_RET		543							//��ѯ��Ա��Ϣ��־������

//��ѯͶע��־��
struct CMD_GP_GetHYXXLogCount
{
	int								dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	bool							bTime;								//ͨ��ʱ���ѯ
 	TCHAR							szTimeStart[30];					//��ʼʱ��
 	TCHAR							szTimeEnd[30];						//����ʱ��
	int								nOnline;							//�������
	int								nYue;								//�ж����
};
//����
struct CMD_GR_GetHYXXLogCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_HYXX_LOG			544								//��ѯӯ����־
#define SUB_GP_GET_HYXX_LOG_RET		545								//����


#define SUB_GP_GET_HYXX_LIST			431								//��ѯ��Ա��Ϣ
#define SUB_GP_GET_HYXX_LIST_RET		432								//����

struct	CMD_GP_GetHYXXLog
{
	int								dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	int								n_t_sorttype;						//����
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	int								bByTime;							//ͨ��ʱ��
	int								nOnline;							//�������
	int								nYue;								//�ж����
	TCHAR						szTimeStart[30];					//��ʼʱ��
	TCHAR						szTimeEnd[30];						//����ʱ��

};

struct CMD_GR_GetHYXXLogRet
{
	int								n_t_id;
	TCHAR							s_t_account[32];
	SYSTEMTIME						n_t_time;
	int								n_t_type;
	DOUBLE							f_t_fandian;
	DOUBLE							f_t_yue;
	int								n_t_online;
	TCHAR							s_t_qq[11];

};
#define SUB_GP_GET_HYXX_LOG_BY_ID			546								//��ѯ��Ա��Ϣ��־
// #define SUB_GP_GET_HYXX_LOG_BY_ID_RET		547								//����

struct	CMD_GP_GetHYXXLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
};
#define SUB_GP_GET_HYXX_LOG_BY_ACT			547								//��ѯ��Ա��Ϣ��־

struct	CMD_GP_GetHYXXLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};


#define SUB_GP_XG_HY_FANDIAN			548								//�޸�QQ
#define SUB_GR_XG_HY_FANDIAN_RET		549								//����

struct	CMD_GP_XGHYFandian
{
	int								dwUserID;							//�û�ID
	int								n_xiaji_id;							//�¼�ID
	DOUBLE							f_Fandian;							//����
};
struct CMD_GP_XGHYFandian_RET
{
	LONG							lResult;
	DOUBLE							f_Fandian;
	TCHAR							s_Account[33];
};


#define SUB_GP_SW_DAILI				911								//��Ϊ����
#define SUB_GR_SW_DAILI_RET			912								//����

struct	CMD_GP_SWDaili
{
	DWORD							dwUserID;							//�û�ID
	DWORD							nAgentID;							//����ID
	int 							nDaili;							//�Ƿ����
};
struct CMD_GP_SWDaili_RET
{
	LONG							lResult;
};
///////////////////////��Աת��///////////////////////////////////////////////////////////
#define SUB_GP_HYXX_ZHUANZHANG				913								//��Ϊ����
#define SUB_GR_HYXX_ZHUANZHANG_RET			914								//����

struct	CMD_GP_HYXX_ZhuanZhang
{
	int								n_t_user_id;							//�Լ�ID
	int								n_t_target_id;							//Ŀ��ID
	DOUBLE 							f_t_jine;								//���
	TCHAR							s_t_password[126];							//����
};
struct CMD_GP_HYXX_ZhuanZhang_RET
{
	int							lResult;
};

#define SUB_GP_GET_Peie			915								//��ȡ���
#define SUB_GR_GET_Peie_RET		916								//����

struct CMD_GetPeie
{
	int								n_t_userid;						//ID
};
struct CMD_GetPeieRet
{
	int								n_t_peie_1;					//3.0�����
	int								n_t_peie_2;					//2.8�����
	int								n_t_peie_3;					//2.7�����
	int								n_t_peie_s_1;				//3.0�������
	int								n_t_peie_s_2;				//2.8�������
	int								n_t_peie_s_3;				//2.7�������
};
#define SUB_GP_GET_LHC_QIHAO			917								//��ȡ���ϲ��ں�
#define SUB_GR_GET_LHC_QIHAO_RET		918								//����

// struct CMD_GetPeie
// {
// 	int								n_t_userid;						//ID
// };
struct CMD_GetLhcQihaoRet
{
	int								n_t_qihao;					//3.0�����
	SYSTEMTIME						n_t_fdshijian;							//ϵͳʱ��
};

#define SUB_GP_GET_XJ_Peie				921								//��ȡ�¼����
#define SUB_GR_GET_XJ_Peie_RET			922								//����

 struct CMD_GetXjPeie
 {
 	int								n_t_userid;						//ID
 };
struct CMD_GetXjPeieRet
{
	int								n_t_peie_1;					//3.0�����
	int								n_t_peie_2;					//2.8�����
	int								n_t_peie_3;					//2.7�����
	int								n_t_peie_s_1;				//3.0�������
	int								n_t_peie_s_2;				//2.8�������
	int								n_t_peie_s_3;				//2.7�������
};
#define SUB_GP_SET_XJ_Peie				923								//�����¼����
#define SUB_GR_SET_XJ_Peie_RET			924								//����

 struct CMD_SetXjPeie
 {
 	int								n_t_userid;						//ID
	int								n_t_xj_id;						//�¼�ID
	int								n_t_peie1;
	int								n_t_peie2;
	int								n_t_peie3;
 };
struct CMD_SetXjPeieRet
{
	int								n_t_result;					//����
	TCHAR							s_t_desc[126];				//����
};


#define SUB_GP_KEFU_URL				925								//�����¼����
#define SUB_GR_KEFU_URL_RET			926								//����
struct CMD_GetKefuUrlRet
{
	int								n_t_result;					//����
	TCHAR							s_t_desc[256];				//����
};

//////////////////////////////////////////////////////////////////////////////////
#define SUB_GP_GET_REG_URL			550								//��ȡע������
#define SUB_GR_GET_REG_URL_RET		551								//����

struct CMD_GetUrlFandian
{
	int								n_t_userid;						//ID
};
struct CMD_GetRegUrl
{
	TCHAR							s_reg_url[126];					//ע������
	DOUBLE							f_t_fandian;
};

#define SUB_GP_ADD_HY				552								//����¼�
#define SUB_GR_ADD_HY_RET			553								//����

struct CMD_GP_AddHuiYuan
{
	int								n_t_daili_id;
	int								n_type;
	DOUBLE							f_t_fandian;
	TCHAR							s_t_account[33];
	TCHAR							s_t_login_pass[33];
	TCHAR							s_t_qukuan_pass[33];
	TCHAR							s_t_qq[33];
};
struct CMD_GR_AddHuiYuanRet
{
	int								n_t_res;
	TCHAR							s_t_Desc[126];
};

#define SUB_GP_SET_WEB_FANDIAN				630								//������ҳע�᷵��
#define SUB_GR_SET_WEB_FANDIAN_RET			631								//����

struct CMD_GP_SetWebFandian
{
	int								n_t_userid;
	DOUBLE							f_t_fandian;
};
struct CMD_GR_SetWebFandianRet
{
	int								n_t_res;
	DOUBLE							f_t_fandian;
};

//�����˻�
#define SUB_GP_GET_XJTZH_LOG_COUNT			554								//��ѯ�¼�Ͷע��־��
#define SUB_GR_GET_XJTZH_LOG_COUNT_RET		555							//��ѯ�¼�Ͷע��־������

//��ѯͶע��־��
struct CMD_GP_GetXJTZHLogCount
{
	int							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetXJTZHLogCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_XJTZH_LOG			556								//��ѯ�¼�Ͷע
#define SUB_GP_GET_XJTZH_LOG_RET		557								//����

struct	CMD_GP_GetXJTZHLog
{
	int							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetXJTZHLogRet
{
	int								n_t_id;
	TCHAR							s_t_account[32];
	int								n_t_type;
	DOUBLE							f_t_touzhu_ze;
	DOUBLE							f_t_yingkui_ze;
};

#define SUB_GP_GET_XJTZH_LOG_BY_ID			558								//��ѯ��Ա��Ϣ��־

struct	CMD_GP_GetXJTZHLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
};
#define SUB_GP_GET_XJTZH_LOG_BY_ACT			559								//��ѯ��Ա��Ϣ��־

struct	CMD_GP_GetXJTZHLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};


//�����˻�
#define SUB_GP_CHK_XJTZH_LOG_COUNT			560								//��ѯ�¼�Ͷע��־��
#define SUB_GR_CHK_XJTZH_LOG_COUNT_RET		561							//��ѯ�¼�Ͷע��־������

//��ѯͶע��־��
struct CMD_GP_CHKXJTZHLogCount
{
	DWORD							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	int								n_xj_id;							//�¼�ID 
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_CHKXJTZHLogCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_CHK_XJTZH_LOG			562								//��ѯ�¼�Ͷע
#define SUB_GP_CHK_XJTZH_LOG_RET		563								//����

struct	CMD_GP_CHKXJTZHLog
{
	DWORD							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_CHKXJTZHLogRet
{
	int								n_t_id;
	TCHAR							s_t_account[33];
	int								n_t_type;
	int								n_t_kind;
	TCHAR							s_t_qishu[30];
	int								n_t_zhushu;
	int								n_t_moshi;
	int								n_t_beishu;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	int								n_t_winzhushu;
	DOUBLE							f_t_yingkui;
	int								n_t_zhuihao;
};

#define SUB_GP_CHK_XJTZH_LOG_BY_ID			564								//��ѯ��Ա��Ϣ��־

struct	CMD_GP_CHKXJTZHLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С

};
#define SUB_GP_CHK_XJTZH_LOG_BY_ACT			565								//��ѯ��Ա��Ϣ��־

struct	CMD_GP_CHKXJTZHLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};

//�����˻�
#define SUB_GP_GET_HYSHJ			566								//��ѯ��Ա����
#define SUB_GR_GET_HYSHJ_RET		567							//��ѯ��Ա���ݷ���

//��ѯͶע��־��
struct CMD_GP_GetHyShj
{
	int							dwUserID;							//�û�ID
	BYTE						cbOnlyToday;						//ֻ������   0 ����    1 ��
	TCHAR						szTimeStart[30];					//��ʼʱ��
	TCHAR						szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetHyShjRet
{
	int								n_t_xjrs;						//�¼�����
	int								n_t_xjzx;						//�¼�����
	DOUBLE							f_t_hyye;						//��Ա���
	DOUBLE							f_t_jrfd;						//���շ���
	DOUBLE							f_t_jrchz;						//���ճ�ֵ
	DOUBLE							f_t_jrqk;						//����ȡ��
	DOUBLE							f_t_jrtzh;						//����Ͷע
	DOUBLE							f_t_jrhd;						//���ջ
	int								n_t_jinrizhuce;					//����ע��
	DOUBLE							f_chedan_zonge;					//�����ܶ�
	DOUBLE							f_zhongjiang_zonge;				//�н��ܶ�
	DOUBLE							f_wodefandian;					//�ҵķ���-��Ʊ
	DOUBLE							f_caipiao_yingkui;				//��Ʊӯ��
	DOUBLE							f_t_xianma;						//����
	DOUBLE							f_qp_fandian;					//���Ʒ���
	DOUBLE							f_qp_yingkui;					//����ӯ��
	DOUBLE							f_my_qp_fandian;				//�ҵ����Ʒ���
	DOUBLE							f_touzhuyongjin;				//ͶעӶ��
	DOUBLE							f_kuisunyongjin;				//����Ӷ��
};

//�����˻�
#define SUB_GP_GET_XJYK_LOG_COUNT			568								//��ѯ�¼�ӯ����Ϣ��־��
#define SUB_GR_GET_XJYK_LOG_COUNT_RET		569							//��ѯ�¼�ӯ����Ϣ��־������

//��ѯ�¼�ӯ����־��
struct CMD_GP_GetXJYKLogCount
{
	DWORD							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	bool							bTime;								//ͨ��ʱ���ѯ
	int								nXiaJiID;							//�¼�ID
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
	int								nType;								//0:Ͷע  1 ����  2�¼�����  3 �н� 4 ��ֵ 5ȡ�� 6����ӯ��
};
//����
struct CMD_GR_GetXJYKLogCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_XJYK_LOG			570								//��ѯ�¼�ӯ��
#define SUB_GP_GET_XJYK_LOG_RET		571								//����

struct	CMD_GP_GetXJYKLog
{
	DWORD							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	int								n_sort_type;						//����    0:�Ŷӳ�ֵ(��)  1 �Ŷӳ�ֵ(��)  2�Ŷ�ȡ��(��)  3 �Ŷ�ȡ��(��) 4 �Ŷ�Ͷע(��) 5�Ŷ�Ͷע(��) 6�Ŷ��н�(��) 7�Ŷ��н�(��)  8�Ŷӷ���(��)  9�Ŷӷ���(��)
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetXJYKLogRet
{
	int								n_t_id;
	int								n_t_userid;
	TCHAR							s_t_account[30];
	DOUBLE							f_t_yingkui;
	DOUBLE							f_t_at_yue;

	SYSTEMTIME						n_t_time;
	int								n_t_type;
	TCHAR							s_t_comment[126];

};

#define SUB_GP_GET_XJYK_LOG_BY_ID			572								//��ѯ�¼�ӯ����Ϣ��־

struct	CMD_GP_GetXJYKLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
		int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С

};
#define SUB_GP_GET_XJYK_LOG_BY_ACT			573								//��ѯ�¼�ӯ����Ϣ��־

struct	CMD_GP_GetXJYKLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};

//////////////////////////�¼�ӯ��ͳ��////////////////
//�����˻�
#define SUB_GP_GET_XJYK_TJ_COUNT			574								//��ѯ�¼�ӯ����Ϣ��־��
#define SUB_GR_GET_XJYK_TJ_COUNT_RET		575							//��ѯ�¼�ӯ����Ϣ��־������

//��ѯ�¼�ӯ����־��
struct CMD_GP_GetXJYKTjCount
{
	int							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetXJYKTjCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_XJYK_TJ			576							//��ѯ�¼�ӯ��
#define SUB_GP_GET_XJYK_TJ_RET		577								//����

struct	CMD_GP_GetXJYKTj
{
	int							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	int								n_sort_type;						//����    0:Ͷע  1 ��ֵ  2ȡ��  3 �н� 4 ӯ��
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetXJYKTjRet
{
	int								n_t_id;
	TCHAR							s_t_account[30];
	int								n_t_type;
	DOUBLE							f_t_chongzhi_ze;
	DOUBLE							f_t_qukuan_ze;
	DOUBLE							f_t_touzhu_ze;
	DOUBLE							f_t_zhongjiang_ze;
	DOUBLE							f_t_fandian_ze;
	DOUBLE							f_t_xjfandian_ze;
	DOUBLE							f_t_huodong_ze;
	DOUBLE							f_t_tuandui_ye;
//	DOUBLE							f_t_qipai_ze;
};
//////////////////////////�¼���Ϸӯ��ͳ��////////////////
//�����˻�
#define SUB_GP_GET_XJYX_TJ_COUNT			419								//��ѯ�¼���Ϸӯ����Ϣ��־��
#define SUB_GR_GET_XJYX_TJ_COUNT_RET		420							//��ѯ�¼���Ϸӯ����Ϣ��־������

//��ѯ�¼�ӯ����־��
struct CMD_GP_GetXJYXTjCount
{
	int							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetXJYXTjCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_XJYX_TJ			421							//��ѯ�¼�ӯ��
#define SUB_GP_GET_XJYX_TJ_RET		422								//����

struct	CMD_GP_GetXJYXTj
{
	int							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetXJYXTjRet
{
	int								n_t_id;
	TCHAR							s_t_account[30];
	int								n_t_type;
	int								n_t_daili_id;

	DOUBLE							f_t_fandian_ze;
	DOUBLE							f_t_qipai_ze;
	DOUBLE							f_t_tuandui_ye;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SUB_GP_GET_XJYK_TJ_BY_ID			578								//��ѯ�¼�ӯ����Ϣ��־

struct	CMD_GP_GetXJYKTjByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
};
#define SUB_GP_GET_XJYK_TJ_BY_ACT			579								//��ѯ�¼�ӯ����Ϣ��־

struct	CMD_GP_GetXJYKTjByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};

//////////////////////////�¼���ֵ��־////////////////
//�����˻�
#define SUB_GP_GET_XJCHZH_LOG_COUNT			580								//��ѯ�¼���ֵ��־��
#define SUB_GR_GET_XJCHZH_LOG_COUNT_RET		581							//��ѯ�¼���ֵ��־������

//��ѯ�¼�ӯ����־��
struct CMD_GP_GetXjChzhLogCount
{
	DWORD							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetXjChzhLogCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_XJCHZH_LOG			582							//��ѯ�¼���ֵ
#define SUB_GP_GET_XJCHZH_LOG_RET		583								//����

struct	CMD_GP_GetXJCHZHLog
{
	DWORD							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetXJCHZHLogRet
{
	int								n_t_id;
	int								n_t_user_id;
	TCHAR							s_t_account[30];
	DOUBLE							f_t_jine;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	TCHAR							s_t_pingtai[30];
};

#define SUB_GP_GET_XJCHZH_LOG_BY_ID			584								//��ѯ�¼�ӯ����Ϣ��־

struct	CMD_GP_GetXJCHZHLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
};
#define SUB_GP_GET_XJCHZH_LOG_BY_ACT			585								//��ѯ�¼�ӯ����Ϣ��־

struct	CMD_GP_GetXJCHZHLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};
//////////////////////////�¼�������־////////////////
//�����˻�
#define SUB_GP_GET_XJTX_LOG_COUNT			586								//��ѯ�¼���ֵ��־��
#define SUB_GR_GET_XJTX_LOG_COUNT_RET		587							//��ѯ�¼���ֵ��־������

//��ѯ�¼�������־��
struct CMD_GP_GetXjTxLogCount
{
	DWORD							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����
struct CMD_GR_GetXjTxLogCountRet
{
	DWORD							dwCount;							//����
};

#define SUB_GP_GET_XJTX_LOG			588							//��ѯ�¼���ֵ
#define SUB_GP_GET_XJTX_LOG_RET		589								//����

struct	CMD_GP_GetXJTxLog
{
	DWORD							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct CMD_GR_GetXJTxLogRet
{
	int								n_t_id;
	int								n_t_user_id;
	TCHAR							s_t_account[30];
	SYSTEMTIME						n_t_time;
	TCHAR							s_t_yinhang[30];
	DOUBLE							f_t_jine;
	TCHAR							s_t_kaihuiren[30];
	int								n_t_state;
	TCHAR							s_t_note[30];
};

#define SUB_GP_GET_XJTX_LOG_BY_ID			590								//��ѯ�¼�ӯ����Ϣ��־

struct	CMD_GP_GetXJTxLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
};
#define SUB_GP_GET_XJTX_LOG_BY_ACT			591								//��ѯ�¼�ӯ����Ϣ��־

struct	CMD_GP_GetXJTxLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};

#define SUB_GP_CHONG_ZHI_TYPE					592					//��ȡ��ֵ��Ϣ
#define SUB_GP_CHONG_ZHI_TYPE_RET				593					//��ȡ��ֵ��Ϣ����

struct CMD_GP_GetChongzhiType
{
	int								nUserID;					//���ID
	BYTE							cbType;						//����
};

struct CMD_GP_GetChongzhiTypeRet
{
	TCHAR							s_t_yinhang[60];				//����
	TCHAR							s_t_kaihuren[60];				//������
	TCHAR							s_t_zhanghao[60];				//�˺�
	TCHAR							s_t_web[60];					//��ҳ
};
#define SUB_GP_GET_ALL_YINHANG_NAME					417					//��ȡ����
#define SUB_GP_GET_ALL_YINHANG_NAME_RET				418					//��ȡ���з���

// struct CMD_GP_GetAllYinHangName
// {
// 	int								nUserID;					//���ID
// 	BYTE							cbType;						//����
// };

struct CMD_GP_GetAllYinHangNameRet
{
	TCHAR							s_t_yinhang[60];				//����
	int								n_QiYong;						//�Ƿ�����
};

#define SUB_GP_QU_KUAN_INFO					594					//��ȡȡ����Ϣ
#define SUB_GP_QU_KUAN_INFO_RET				595					//��ȡȡ����Ϣ����

struct CMD_GP_GetQukuanInfo
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_GetQukuanInfoRet
{
	DOUBLE							f_t_yue;						//���
	TCHAR							s_t_yinhang[60];				//����
	TCHAR							s_t_kaihuren[60];				//������
	TCHAR							s_t_zhanghao[60];				//�˺�
	BYTE							n_t_isvip;						//�Ƿ񳬼�VIP
};

#define SUB_GP_DO_QU_KUAN					600					//ȡ����Ϣ
#define SUB_GP_DO_QU_KUAN_RET				601					//ȡ����Ϣ����

struct CMD_GP_DoQukuan
{
	DWORD							dwUserID;						//�û�ID
	DOUBLE							fJine;							//���
	TCHAR							szQukuanPass[33];				//ȡ������
};

struct CMD_GP_DoQukuanRet
{
	int								nResult;						//����ֵ
	TCHAR							szDesc[126];					//ȡ����ʾ
};

#define SUB_GP_GET_LAST_YUE					596					//��ȡ���������Ϣ
#define SUB_GP_GET_LAST_YUE_RET				597					//��ȡ���������Ϣ����

struct CMD_GP_GetLastYue
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_GetLastYueRet
{
	DOUBLE							f_t_yue;						//���
	DOUBLE							f_t_dongjie;					//����
	DOUBLE							f_t_score;						//��Ϸ��
};

#define SUB_GP_QUIT_GAME					409					//��ȡ���������Ϣ
#define SUB_GP_QUIT_GAME_RET				410					//��ȡ���������Ϣ����

struct CMD_GP_QuitGame
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_QuitGameRet
{
	int								nResult;
};

///////////////////////////////�����/////////////////////////////////////////////
#define SUB_GP_GET_DA_LI_BAO				602					//��ȡ����� ��Ϣ
#define SUB_GP_GET_DA_LI_BAO_RET			603					//��ȡ����� ��Ϣ����

struct CMD_GP_GetDaLiBao
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_GetDaLiBaoRet
{
	int							f_t_jine;						//���
};
#define SUB_GP_CAN_DA_LI_BAO				604					//��ȡ����� ��Ϣ
#define SUB_GP_CAN_DA_LI_BAO_RET			605					//��ȡ����� ��Ϣ����

struct CMD_GP_CanDaLiBao
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_CanDaLiBaoRet
{
	DOUBLE						f_t_xiaofei;
	DOUBLE						f_t_chongzhi;
	DOUBLE						f_t_yue;
	int							n_t_got;						//���
};
//////////////////////////////////////�ҽ�////////////////////////////////////////
#define SUB_GP_CAN_ZA_JIN_DAN				606					//��ȡ����� ��Ϣ
#define SUB_GP_CAN_ZA_JIN_DAN_RET			607					//��ȡ����� ��Ϣ����

struct CMD_GP_CanZaJinDan
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_CanZaJinDanRet
{
	DOUBLE						f_t_xiaofei;
	int							n_t_got;						//���
};

#define SUB_GP_GET_ZA_JIN_DAN				608					//��ȡ����� ��Ϣ
#define SUB_GP_GET_ZA_JIN_DAN_RET			609					//��ȡ����� ��Ϣ����

struct CMD_GP_GetZaJinDan
{
	DWORD							dwUserID;						//�û�ID
	int								nType;						//��type
};

struct CMD_GP_GetZaJinDanRet
{
	int							f_t_jine;						//���
};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////����תת////////////////////////////////////////
#define SUB_GP_CAN_LUCKY_ZHUAN				610					//��ȡ����תת ��Ϣ
#define SUB_GP_CAN_LUCKY_ZHUAN_RET			611					//��ȡ����תת ��Ϣ����

struct CMD_GP_CanLuckyZhuan
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_CanLuckyZhuanRet
{
	DOUBLE						f_t_xiaofei;
};

#define SUB_GP_GET_LUCKY_ZHUAN				612					//��ȡ����תת ��Ϣ
#define SUB_GP_GET_LUCKY_ZHUAN_RET			613					//��ȡ����תת��Ϣ����

struct CMD_GP_GetLuckyZhuan
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_GetLuckyZhuanRet
{
	int							n_t_got;
	DOUBLE							f_t_jine;						//���
};
//////////////////////////////////////////////////////////////////////////////////
#define SUB_GP_GET_USER_QIANDAO				614					//ǩ����Ϣ
#define SUB_GP_GET_USER_QIANDAO_RET			615					//ǩ������

struct CMD_GP_UserQiandao
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_UserQiandaoRet
{
	int							n_t_res;
};
#define SUB_GP_GET_USER_HUANLESONG				616					//��������Ϣ
#define SUB_GP_GET_USER_HUANLESONG_RET			617					//�����ͷ���

struct CMD_GP_GetUserHuanlesong
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_GetUserHuanlesongRet
{
	int							nRes;
	DOUBLE						f_t_xiaofei;
	DOUBLE						f_t_prize;
	int							n_t_count;
	int							n_t_XXcount;
};

#define SUB_GP_GET_LING_JIANG				618					//��ȡ�콱��Ϣ
#define SUB_GP_GET_LING_JIANG_RET			619					//��ȡ�콱��Ϣ����

struct CMD_GP_GetLingJiang
{
	DWORD							dwUserID;						//�û�ID
};

struct CMD_GP_GetLingJiangRet
{
	int							n_t_got;
	DOUBLE							f_t_jine;						//���
};

#define SUB_GP_GET_TOU_ZHU_XX				620					//��ȡͶע��Ϣ
#define SUB_GP_GET_TOU_ZHU_XX_RET			621					//��ȡͶע��Ϣ����

struct CMD_GP_GetTouzhu
{
	int							n_t_id;						//ID
};

struct CMD_GP_GetTouzhuRet
{
	TCHAR						s_t_wanjiazhanghao[30];				//�˻�
	int							n_t_dingdanhao;						//������
	TCHAR						s_t_qishu[20];						//����
	int							n_t_gametype;						//��Ϸ���
	int							n_t_gamekind;						//�淨
	char						s_t_touzhuhaoma[13312];				//Ͷע����
	TCHAR						s_t_kaijianghaoma[45];				//��������
	SYSTEMTIME					n_t_time;
	int							n_t_state;
	int							n_t_zhushu;
	int							n_t_moshi;
	int							n_t_beishu;
	int							n_t_winzhushu;
	DOUBLE						f_t_yingkui;
	DOUBLE						f_t_danzhujine;
	DOUBLE						f_t_fandian;
	int							n_t_End;
	int							nHaoMaLen;
	int							n_Index;
};

#define SUB_GP_GET_NEWS				622					//��ȡ����
#define SUB_GP_GET_NEWS_RET			623					//��ȡ���ŷ���

struct CMD_GP_GetNews
{
	int							n_t_id;						//ID
	int							n_t_ccount;			//����
};

struct CMD_GP_GetNewsRet
{
	TCHAR						s_t_news[512];				//����

};

#define SUB_GP_ZHUANHUAN				624					//ת��
#define SUB_GP_ZHUANHUAN_RET			625					//ת������

struct CMD_GP_ZhuanHuan
{
	int							n_t_userid;					//�û�ID
	int							n_t_type;					//����   1 ת���   2 ת�ʽ�
	DOUBLE						f_t_jine;					//���
};

struct CMD_GP_ZhuanHuanRet
{
	int							nResult;
	DOUBLE						f_t_yue;
	DOUBLE						f_t_score;
};
///////////////////////////////////////////////////////////////////////////////////
//��ȡ���࿪�����
#define SUB_GP_GET_MORE_RECORD				632					//
#define SUB_GP_GET_MORE_RECORD_RET			633					//����

struct CMD_GP_GetMoreRecord
{
	int							n_t_type;					//����   1 ת���   2 ת�ʽ�
};

struct CMD_GP_GetMoreRecordRet
{
	WORD							wKindID;							//��������
	TCHAR							wPeriod[30];							//��
	TCHAR							szLotNum[45];						//��������
	TCHAR							szShijian[30];							//ʱ��
};

#define SUB_MB_GET_TOU_ZHU_XX				634					//��ȡͶע��Ϣ
#define SUB_MB_GET_TOU_ZHU_XX_RET			635					//��ȡͶע��Ϣ����

struct CMD_MB_GetTouzhu
{
	int							n_t_id;						//ID
};

struct CMD_MB_GetTouzhuRet
{
	TCHAR						s_t_wanjiazhanghao[30];				//�˻�
	int							n_t_dingdanhao;						//������
	TCHAR						s_t_qishu[20];						//����
	int							n_t_gametype;						//��Ϸ���
	int							n_t_gamekind;						//�淨
	char						s_t_touzhuhaoma[13312];				//Ͷע����
	TCHAR						s_t_kaijianghaoma[45];				//��������
	SYSTEMTIME					n_t_time;
	int							n_t_state;
	int							n_t_zhushu;
	int							n_t_moshi;
	int							n_t_beishu;
	int							n_t_winzhushu;
	DOUBLE						f_t_yingkui;
	DOUBLE						f_t_danzhujine;
	DOUBLE						f_t_fandian;
	int							n_t_End;
	int							nHaoMaLen;
	int							n_Index;
};
///////////////////////////////////////////////////////////////////////////////////
//��ȡ�������
#define SUB_GP_GET_DAILI_HUIKUI				638					//
#define SUB_GP_GET_DAILI_HUIKUI_RET			639					//����

struct CMD_GP_GetDailiHuikui
{
	int							n_t_userid;					//�û�ID
};

struct CMD_GP_GetDailiHuikuiRet
{
	BYTE						cbNewRegUser;					//����
	DOUBLE						f_t_touzhu;						//Ͷע
	DOUBLE						f_t_chongzhi;					//��ֵ
};

//�����콱
#define SUB_GP_DAILI_LJ						640					//
#define SUB_GP_DAILI_LJ_RET					641					//����

struct CMD_GP_DailiLingjiang
{
	int							n_t_userid;					//�û�ID
};

struct CMD_GP_DailiLingjiangRet
{
	BYTE						n_t_state;					//����״̬ 0 δ  1����
	DOUBLE						f_t_jine;						//���
};
///////////////////////////////////////////////////////////////////////////
//�޸��Ա�
#define SUB_GP_ALTER_GENDER					642					//
#define SUB_GP_ALTER_GENDER_RET				643					//����

struct CMD_GP_AlterGender
{
	int								n_t_userid;					//�û�ID
	BYTE 							cbGender;					//�Ա�
};

struct CMD_GP_AlterGenderRet
{
	BYTE						n_t_return;					//����״̬ 0 δ  1����
};

///////////////////////////////////////////////////////////////////////////////////
//�޸�����
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//////////////////////////////////////////////////////////////////////////////////
struct CMD_GP_GetCpUser
{
	int								n_t_userid;
};
//����
struct CMD_GR_GetCpUserInfoRet //��ȡ��Ʊ�����Ϣ
{
	int								n_t_userid;
	TCHAR							s_t_account[30];
	int								n_t_type;
	DOUBLE							f_t_fandian;
	TCHAR							s_t_qq[11];
	int								n_t_daili_id;
	int								n_t_online;
	DOUBLE							f_t_yue;
	DOUBLE							f_t_dongjie;
	int								n_t_ban;

};

struct CMD_GP_GetUserFandian
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
};
//����
struct CMD_GR_GetUserFandianRet //��ȡ��Ʊ��ҷ���
{
	int								n_t_kindid;
	DOUBLE							f_t_bonus;
	DOUBLE							f_t_bonusPercent;
	DOUBLE							f_t_fandian;
};
struct CMD_GR_GetMapBonusRet
{
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
};

struct CMD_GP_SetUserBonus
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
	DOUBLE							f_bonuspercent;
};
//����
struct CMD_GR_SetUserBonusRet //��ȡ��Ʊ��ҷ���
{
	int								n_t_result;
};

//��ѯ��Ϸ�������
struct CMD_GP_QueryGameResult
{
	int							wKindID;							//��Ҫ��ѯ����ϷID
};
//���ز�ѯϵͳʱ��
struct CMD_GR_GetSystemTime
{
	SYSTEMTIME						ServerTime;							//ϵͳʱ��
};
//��ѯ��ֵ��ʾ
struct CMD_GP_GetChonzhiTishi
{
	DWORD							dwUserID;							//�û�ID 
};
//��ֵ��ʾ����
struct CMD_GR_ChongzhiTishiRet
{
	DOUBLE							fJine;
	int								nState;
};
//��ѯ������ʾ
struct CMD_GP_GetTixianTishi
{
	DWORD							dwUserID;							//�û�ID 
};
//������ʾ����
struct CMD_GR_TixianTishiRet
{
	DOUBLE							fJine;
	int								nState;
};

//Ͷע��ʾ
struct CMD_GP_GetTouzhuTishi
{
	DWORD							dwUserID;							//�û�ID 
};
//������ʾ����
struct CMD_GR_TouzhuTishiRet
{
	DOUBLE								nWinZhushu;
	int								nState;
	TCHAR							szQishu[30];
	int								nType;
	DOUBLE							f_yingkui;
	DOUBLE								nZhushu;
};
/////////////////////////////////////////////////////////////////////////////////
//�û�ͷ��
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
};

//�޸�ͷ��
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};
/////////////////////////////////////////////////////////////////////////////////////
//����ʱʱ��
//��ע
struct CMD_GP_TouZhuCQSSC
{
	DWORD							dwUserID;							//�û�ID
	char							strQishu[20];						//����
	int								nGameType;							//��Ϸ���
	int								nGameKind;							//�淨����
	char							strHaoma[15360];					//��ע����
	int								nHaoMaLen;							//���볤��
	int								nZhushu;							//ע��
	int								nBeitou;							//����
	int								nMoshi;								//ģʽ  Ԫ�Ƿ�
	int								nSign;								//Ͷע��־
	int								bZhuihao;							//�Ƿ�׷��
	WORD							nIndex;								//index
	WORD							cbPlatform;							//ƽ̨
};
struct CMD_GP_TouZhuCQSSC_Dan
{
	DWORD							dwUserID;							//�û�ID
	char							strQishu[20];						//����
	int								nGameType;							//��Ϸ���
	int								nGameKind;							//�淨����
	char							strHaoma[15360];					//��ע����
	int								nHaoMaLen;							//���볤��
	int								nZhushu;							//ע��
	int								nBeitou;							//����
	int								nMoshi;								//ģʽ  Ԫ�Ƿ�
	int								nSign;								//Ͷע��־
	int								nEnd;								//�Ƿ��ͽ���
	int								bZhuihao;							//�Ƿ�׷��
	int								nIndex;								//index
};
struct CMD_GP_TouZhuCQSSC_Zhuihao
{
	DWORD							dwUserID;							//�û�ID
	char							strQishu[50][20];						//����
	int								nGameType;							//��Ϸ���
	int								nGameKind;							//�淨����
	char							strHaoma[14336];					//��ע����
	int								nHaoMaLen;							//���볤��
	int								nZhushu;							//ע��
	int								nBeitou[50];							//����
	int								nMoshi;								//ģʽ  Ԫ�Ƿ�
	int								nSign;								//Ͷע��־
	int								nEnd;								//�Ƿ��ͽ���
	int								bZhuihao;							//�Ƿ�׷��
	int								nIndex;								//index
};
//////////////////////////////////////////////////////////////////////////////////

//�󶨻���
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ��Ϣ
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//�޸�����
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			2									//�û�˵��
#define DTP_GP_UI_QQ				4									//Q Q ����
#define DTP_GP_UI_EMAIL				5									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		6									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		7									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		8									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	9									//��ϵ��ַ

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserInsureInfo
{
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//������
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ���
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת�˽��
struct CMD_GP_UserTransferScore
{
	DWORD                           dwUserID;                           //�û� I D
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
	TCHAR                           szLogonPass[LEN_PASSWORD];          //��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//���гɹ�
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ȡ���
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//��ѯ����
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
};

//////////////////////////////////////////////////////////////////////////////////

//����ʧ��
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���

#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//���ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//���ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_GP_C_SearchCorrespond
{
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//�û���Ŀ
	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_ACCOUNTS		1									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	2									//ע���ʺ�

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��

//������ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//��¼�ɹ�
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwExperience;						//������ֵ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//��¼ʧ��
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};
//������ѯ���
struct CMD_GP_QueryLotResult
{
	WORD							wKindID;							//��������
	TCHAR							wPeriod[30];							//��
	TCHAR							szLotNum[45];						//��������
	TCHAR							szShijian[30];							//ʱ��
};
struct CMD_GR_TouzhuRes
{
	LONG							lResult;							//����ֵ
	TCHAR							szDesc[126];						//��������
	int								nSign;								//��־

};
//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_MB_SERVER_LIST			101									//�б���Ϣ

//�б���Ϣ
#define SUB_MB_LIST_KIND			100									//�����б�
#define SUB_MB_LIST_SERVER			101									//�����б�
#define SUB_MB_LIST_FINISH			200									//�б����

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif