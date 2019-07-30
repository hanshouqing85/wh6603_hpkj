#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//��¼����
#define	DBR_GP_LOGON_ACCOUNTS		1									//�ʺŵ�¼
#define DBR_GP_REGISTER_ACCOUNTS	2									//ע���ʺ�

//�˺ŷ���
#define DBR_GP_MODIFY_MACHINE		10									//�޸Ļ���
#define DBR_GP_MODIFY_LOGON_PASS	11									//�޸�����
#define DBR_GP_MODIFY_INSURE_PASS	12									//�޸�����
#define DBR_GP_MODIFY_INDIVIDUAL	13									//�޸�����
#define DBR_GP_MODIFY_SYSTEM_FACE	14									//�޸�ͷ��

//��������
#define DBR_GP_USER_SAVE_SCORE		20									//������
#define DBR_GP_USER_TAKE_SCORE		21									//��ȡ���
#define DBR_GP_USER_TRANS_SCORE     22                                  //ת�˽�� 

//��ѯ����
#define DBR_GP_QUERY_INDIVIDUAL		30									//��ѯ����
#define DBR_GP_QUERY_INSURE_INFO	31									//��ѯ����

//ϵͳ����
#define DBR_GP_LOAD_GAME_LIST		40									//�����б�
#define DBR_GP_ONLINE_COUNT_INFO	41									//������Ϣ
#define DBR_GP_LOAD_VIRTUAL_COUNT	42									//��������

//������ѯ
#define DBR_GP_QUERY_RESULT			50									//������ѯ
#define DBO_GP_QUERY_RESULT			51									//������ѯ

//������ҳע�᷵��
#define DBR_GP_SET_WEB_FANDIAN			284									//������ҳ����
#define DBO_GP_SET_WEB_FANDIAN_RESULT	285									//������ҳ���㷵��
//////////////////////////////////////////////////////////////////////////////////
#define DBR_GP_TOUZHU_CQSSC			52									//����ʱʱ����ע
#define DBR_GP_TOUZHU_CQSSC_DAN		283									//����ʱʱ����ע��ʽ
#define DBO_GP_TOUZHU_CQSSC_RET		53									//��ע����
#define DBO_GP_TOUZHU_CONTINUE_RET		286									//��ע����
#define DBR_GP_TOUZHU_CQSSC_ZHUIHAO		287									//����ʱʱ����ע��ʽ

#define DBR_GP_TOUZHU_LOG_COUNT		54									//��ѯͶע��¼����
#define DBO_GP_TOUZHU_LOG_COUNT_RET	55									//��ѯͶע��¼��������

#define DBR_GP_TOUZHU_LOG			56									//��ѯͶע��¼
#define DBO_GP_TOUZHU_LOG_RET		57									//��ѯͶע��¼����

#define DBR_MB_TOUZHU_LOG			320									//��ѯͶע��¼
#define DBO_MB_TOUZHU_LOG_RET		321									//��ѯͶע��¼����

#define DBR_GP_CANCEL_TOUZHU			58									//����
#define DBO_GP_CANCEL_TOUZHU_RET		59									//��������

#define DBR_GP_CHONGZHI_TISHI 			60									//��ֵ��ʾ
#define DBO_GP_CHONGZHI_TISHI_RET		61									//��ֵ��ʾ����

#define DBR_GP_TIXIAN_TISHI 			62									//������ʾ
#define DBO_GP_TIXIAN_TISHI_RET			63									//������ʾ����

#define DBR_GP_TOUZHU_TISHI 			64									//Ͷע��ʾ
#define DBO_GP_TOUZHU_TISHI_RET			65									//Ͷע��ʾ����

#define DBR_GP_TIXIAN_LOG_COUNT			66									//��ѯ���ּ�¼����
#define DBO_GP_TIXIAN_LOG_COUNT_RET		67									//��ѯ���ּ�¼��������

#define DBR_GP_TIXIAN_LOG			68									//��ѯ���ּ�¼
#define DBO_GP_TIXIAN_LOG_RET		69									//��ѯ���ּ�¼����

#define DBR_GP_CHONGZHI_LOG_COUNT			70									//��ѯ��ֵ��¼����
#define DBO_GP_CHONGZHI_LOG_COUNT_RET		71									//��ѯ��ֵ��¼��������

#define DBR_GP_CHONGZHI_LOG			72									//��ѯ���ּ�¼
#define DBO_GP_CHONGZHI_LOG_RET		73									//��ѯ���ּ�¼����

#define DBR_GP_YINGKUI_LOG_COUNT			75									//��ѯ��ֵ��¼����
#define DBO_GP_YINGKUI_LOG_COUNT_RET		76									//��ѯ��ֵ��¼��������

#define DBR_GP_YINGKUI_LOG			77									//��ѯ���ּ�¼
#define DBO_GP_YINGKUI_LOG_RET		78									//��ѯ���ּ�¼����

#define DBR_GP_GET_USER_INFO			79									//��ѯ�û���Ϣ
#define DBO_GP_GET_USER_INFO_RET		80									//��ѯ�û���Ϣ����

#define DBR_GP_XG_QQ			81									//�޸�QQ
#define DBO_GP_XG_QQ_RET		82									//�޸�QQ

#define DBR_GP_XG_LOGIN_PASS			83									//�޸ĵ�¼����
#define DBO_GP_XG_LOGIN_PASS_RET		84									//�޸ĵ�¼���뷵��


#define DBR_GP_XG_QUKUAN_PASS			85									//�޸�ȡ������
#define DBO_GP_XG_QUKUAN_PASS_RET		86									//�޸�ȡ�����뷵��


#define DBR_GP_SET_QUKUAN_PROTECT			87									//����ȡ���
#define DBO_GP_SET_QUKUAN_PROTECT_RET		88									//����ȡ�������

#define DBR_GP_SET_QUKUAN_ZHANGHAO			89									//����ȡ���
#define DBO_GP_SET_QUKUAN_ZHANGHAO_RET		90									//����ȡ�������

#define DBR_GP_QUERY_YINHANG			294									//��ȡ����
#define DBO_GP_QUERY_YINHANG_RET		295									//��ȡ���з���

#define DBR_GP_YINGKUI_MX_COUNT			290									//��ѯӯ����ϸ����
#define DBO_GP_YINGKUI_MX_COUNT_RET		291									//��ѯӯ����ϸ��������

#define DBR_GP_QP_YINGKUI_COUNT			298									//��ѯ����ӯ������
#define DBO_GP_QP_YINGKUI_COUNT_RET		299									//��ѯ����ӯ����������

#define DBR_GP_YINGKUI_MX			292									//��ѯӯ����ϸ
#define DBO_GP_YINGKUI_MX_RET		293									//��ѯӯ����ϸ����

#define DBR_GP_QP_YINGKUI			300									//��ѯ����ӯ����ϸ
#define DBO_GP_QP_YINGKUI_RET		301									//��ѯ����ӯ����ϸ����

#define DBR_GP_QUERY_MY_YINHANG		302									//��ѯ�ҵ������˻�
#define DBO_GP_QUERY_MY_YINHANG_RET	303									//��ѯ�ҵ������˻�����

#define DBR_GP_QUERY_YINHANG		304									//��ѯ�ҵ������˻�
#define DBO_GP_QUERY_YINHANG_RET	305									//��ѯ�ҵ������˻�����

#define DBR_GP_QUERY_MY_PROTECT		310									//��ѯ�ҵ������˻�
#define DBO_GP_QUERY_MY_PROTECT_RET	311									//��ѯ�ҵ������˻�����

#define DBR_GP_HYXX_LIST			312									//��ѯ��Ա��Ϣ
#define DBO_GP_HYXX_LIST_RET		313									//��ѯ��Ա��Ϣ����
#define DBR_GP_ALTER_GENDER			330									//�޸��Ա�
#define DBO_GP_ALTER_GENDER_RET		331									//�޸��Ա𷵻�

////////////////////////��Ա//////////////////////////////////////////////////////////////////////////
#define DBR_GP_HYXX_LOG_COUNT			91									//��ѯ��Ա��Ϣ����
#define DBO_GP_HYXX_LOG_COUNT_RET		92									//��ѯ��Ա��¼��������

#define DBR_GP_HYXX_LOG			93									//��ѯ��Ա��Ϣ����
#define DBO_GP_HYXX_LOG_RET		94									//��ѯ��Ա��¼��������
#define DBR_GP_HYXX_LOG_BY_ID	95									//��ѯ��Ա��Ϣ
#define DBR_GP_HYXX_LOG_BY_ACT	96									//��ѯ��Ա��Ϣ

#define DBR_GP_XGHY_FANDIAN			97									//�޸Ļ�Ա����
#define DBO_GP_XGHY_FANDIAN_RET		98									//�޸Ļ�Ա���㷵��

#define DBR_GP_SW_DAILI			296									//��Ϊ����
#define DBO_GP_SW_DAILI_RET		297									//��Ϊ������

#define DBR_GP_HYXX_ZHUANZHANG			326									//��Ա��Ϣת��
#define DBO_GP_HYXX_ZHUANZHANG_RET		327									//��Ա��Ϣת�˷���

#define DBR_GP_HYXX_GET_XJPEIE			340									//��ȡ�¼����
#define DBO_GP_HYXX_GET_XJPEIE_RET		341									//��ȡ�¼�����

#define DBR_GP_HYXX_SET_XJPEIE			342									//�����¼����
#define DBO_GP_HYXX_SET_XJPEIE_RET		343									//�����¼�����

#define DBR_GP_GET_KEFU_URL				344
#define DBO_GP_GET_KEFU_URL_RET			345							//��ȡ�ͷ�����
#define DBR_GP_GET_MY_MAP_BONUS			328									//��ȡ����bonus
#define DBO_GP_GET_MY_MAP_BONUS_RET		329									//��ȡ����bonus����

#define DBR_GP_GET_Peie			334								//��ȡ���
#define DBO_GR_GET_Peie_RET		335								//����

#define DBR_GP_GET_LHC_QIHAO			336								//��ȡ���ϲ��ں�
#define DBO_GR_GET_LHC_QIHAO_RET		337								//����

#define DBR_GP_LOCK_MACHINE				338								//��������
#define DBO_GR_LOCK_MACHINE_RET			339								//����

#define DBR_GP_GET_REG_URL			99									//��ȡע������
#define DBO_GP_GET_REG_URL_RET		200									//��ȡע�����ӷ���
#define DBR_GP_ADD_HY			201									//����¼�
#define DBO_GP_ADD_HY_RET		202									//����¼�����

#define DBR_GP_XJTZH_LOG_COUNT			203									//��ѯ�¼�Ͷע��¼����
#define DBO_GP_XJTZH_LOG_COUNT_RET		204									//��ѯ�¼�Ͷע��������

#define DBR_GP_XJTZH_LOG			205									//��ѯ�¼�Ͷע��¼
#define DBO_GP_XJTZH_LOG_RET		206									//��ѯ�¼�Ͷע����
#define DBR_GP_XJTZH_LOG_BY_ID			207									//��ѯ�¼�Ͷע��¼ͨ��ID
#define DBR_GP_XJTZH_LOG_BY_ACT			208									//��ѯ�¼�Ͷע��¼ͨ���˻�

#define DBR_GP_CHKXJTZH_LOG_COUNT			209									//��ѯ�¼�Ͷע��¼����
#define DBO_GP_CHKXJTZH_LOG_COUNT_RET		210									//��ѯ�¼�Ͷע��������
#define DBR_GP_CHKXJTZH_LOG					211									//��ѯ�¼�Ͷע��¼
#define DBO_GP_CHKXJTZH_LOG_RET				212									//��ѯ�¼�Ͷע����
#define DBR_GP_CHKXJTZH_LOG_BY_ID			213									//��ѯ�¼�Ͷע��¼ͨ��ID
#define DBR_GP_CHKXJTZH_LOG_BY_ACT			214									//��ѯ�¼�Ͷע��¼ͨ���˻�

#define DBR_GP_GET_HYSHJ			215									//��ȡ��Ա����
#define DBR_GP_GET_HYSHJ_RET		216									//��ȡ��Ա���ݷ���

#define DBR_GP_XJYK_LOG_COUNT			217									//��ѯ�¼�Ͷע��¼����
#define DBO_GP_XJYK_LOG_COUNT_RET		218									//��ѯ�¼�Ͷע��������

#define DBR_GP_GET_XJYK_LOG			219									//��ȡ�¼�ӯ��
#define DBR_GP_GET_XJYK_LOG_RET		220									//��ȡ�¼�ӯ������
#define DBR_GP_XJYK_LOG_BY_ID			221									//��ѯ�¼�Ͷע��¼ͨ��ID
#define DBR_GP_XJYK_LOG_BY_ACT			222								//��ѯ�¼�Ͷע��¼ͨ���˻�

#define DBR_GP_XJYK_TJ_COUNT			223									//��ѯ�¼�ӯ��ͳ������
#define DBO_GP_XJYK_TJ_COUNT_RET		224									//��ѯ�¼�ӯ��ͳ����������

#define DBR_GP_GET_XJYK_TJ			225								//��ȡ�¼�ӯ��
#define DBR_GP_GET_XJYK_TJ_RET		226									//��ȡ�¼�ӯ������
#define DBR_GP_XJYK_TJ_BY_ID			227									//��ѯ�¼�Ͷע��¼ͨ��ID
#define DBR_GP_XJYK_TJ_BY_ACT			228								//��ѯ�¼�Ͷע��¼ͨ���˻�

#define DBR_GP_XJCHZH_LOG_COUNT			229									//��ѯ�¼���ֵ����
#define DBO_GP_XJCHZH_LOG_COUNT_RET		230									//��ѯ�¼���ֵ��������

#define DBR_GP_GET_XJCHZH_LOG			231									//��ȡ�¼ҳ�ֵ
#define DBR_GP_GET_XJCHZH_LOG_RET		232									//��ȡ�¼ҳ�ֵ����
#define DBR_GP_XJCHZH_LOG_BY_ID			233									//��ѯ�¼���ֵ��¼ͨ��ID
#define DBR_GP_XJCHZH_LOG_BY_ACT			234								//��ѯ�¼���ֵͨ���˻�

#define DBR_GP_XJTX_LOG_COUNT			235									//��ѯ�¼���������
#define DBO_GP_XJTX_LOG_COUNT_RET		236									//��ѯ�¼�������������

#define DBR_GP_GET_XJTX_LOG			237									//��ȡ�¼�����
#define DBR_GP_GET_XJTX_LOG_RET		238									//��ȡ�¼����ַ���
#define DBR_GP_XJTX_LOG_BY_ID			239									//��ѯ�¼����ּ�¼ͨ��ID
#define DBR_GP_XJTX_LOG_BY_ACT			240								//��ѯ�¼�����ͨ���˻�

#define DBR_GP_GET_CHONGZHI_TYPE			241									//��ȡ��ֵ��Ϣ
#define DBO_GP_GET_CHONGZHI_TYPE_RET		242									//��ȡ��ֵ��Ϣ����

#define DBR_GP_GET_QUKUAN_INFO				243									//��ȡȡ����Ϣ
#define DBO_GP_GET_QUKUAN_INFO_RET			244									//��ȡȡ����Ϣ����

#define DBR_GP_DO_QUKUAN				245									//ȡ����Ϣ
#define DBO_GP_DO_QUKUAN_RET			246									//ȡ����Ϣ����

#define DBR_GP_GET_YUE_INFO				247									//��ȡ�����Ϣ
#define DBO_GP_GET_YUE_INFO_RET			248									//��ȡ�����Ϣ����

#define DBR_GP_QUIT_GAME				288									//�˳���Ϸ
#define DBO_GP_QUIT_GAME_RET			289									//�˳���Ϸ����

#define DBR_GP_GET_CP_USER_INFO				249									//��ȡ�����Ϣ
#define DBO_GP_GET_CP_USER_INFO_RET			250									//��ȡ�����Ϣ����

#define DBR_GP_GET_USER_FANDIAN				251									//��ȡ��ҷ�����Ϣ
#define DBO_GP_GET_USER_FANDIAN_RET			252									//��ȡ��ҷ�����Ϣ����
#define DBR_GP_SET_USER_BONUS				253									//������ҷ�����Ϣ
#define DBO_GP_SET_USER_BONUS_RET			254									//������ҷ�����Ϣ����

#define DBR_GP_GET_MAP_BONUS				255									//��ȡMAPBONUS
#define DBR_GP_GET_MAP_BONUS_RET			256									//��ȡMAPBONUS����

#define DBR_GP_GET_DA_LI_BAO				257									//��ȡ�����
#define DBR_GP_GET_DA_LI_BAO_RET			258									//��ȡ���������

#define DBR_GP_CAN_DA_LI_BAO				259									//��ȡ������ʸ�
#define DBR_GP_CAN_DA_LI_BAO_RET			260									//��ȡ������ʸ񷵻�

#define DBR_GP_CAN_ZA_JIN_DAN				261									//��ȡ�ҽ��ʸ�
#define DBR_GP_CAN_ZA_JIN_DAN_RET			262									//��ȡ�ҽ��ʸ񷵻�

#define DBR_GP_GET_ZA_JIN_DAN				263									//��ȡ�ҽ�
#define DBR_GP_GET_ZA_JIN_DAN_RET			264									//��ȡ�ҽ𵰷���

#define DBR_GP_CAN_LUCKY_ZHUAN				265									//��ȡת���ʸ�
#define DBR_GP_CAN_LUCKY_ZHUAN_RET			266									//��ȡת���ʸ񷵻�

#define DBR_GP_GET_LUCKY_ZHUAN				267									//��ȡת��
#define DBR_GP_GET_LUCKY_ZHUAN_RET			268									//��ȡת�̷���

#define DBR_GP_USER_QIANDAO				269									//ǩ��
#define DBR_GP_USER_QIANDAO_RET			270									//ǩ������

#define DBR_GP_USER_HUANLE_SONG				271									//������
#define DBR_GP_USER_HUANLE_SONG_RET			272									//�����ͷ���

#define DBR_GP_USER_LING_JIANG				273									//�콱
#define DBR_GP_USER_LING_JIANG_RET			274									//�콱����

#define DBR_GP_GET_TOUZHU_XX			275									//Ͷע��ϸ��Ϣ
#define DBO_GP_GET_TOUZHU_XX_RET		276									//Ͷע��ϸ��Ϣ����

#define DBR_GP_GET_NEWS					277									//������Ϣ
#define DBO_GP_GET_NEWS_RET				278									//������Ϣ����

#define DBR_GP_ZHUAN_HUAN				279									//ת��
#define DBO_GP_ZHUAN_HUAN_RET			280									//ת������

#define DBR_GP_GET_SYS_TIME				281									//��ȡϵͳʱ��
#define DBO_GP_GET_SYS_TIME_RET			282									//��ȡϵͳʱ�䷵��

#define DBR_GP_XJYX_TJ_COUNT			306									//��ѯ�¼�ӯ��ͳ������
#define DBO_GP_XJYX_TJ_COUNT_RET		307									//��ѯ�¼�ӯ��ͳ����������

#define DBR_GP_GET_XJYX_TJ			308								//��ȡ�¼���Ϸӯ��
#define DBR_GP_GET_XJYX_TJ_RET		309									//��ȡ�¼���Ϸӯ������

#define DBR_GP_GET_MORE_RECORD			314								//��ȡ�����¼
#define DBR_GP_GET_MORE_RECORD_RET		315								//��ȡ�����¼����

#define DBR_GP_GET_WIN_USER				316									//��ȡ�н��û�ID
#define DBO_GP_GET_WIN_USER_RET			317									//��ȡ�н��û�ID����

#define DBR_MB_GET_TOUZHU_XX			318									//Ͷע��ϸ��Ϣ
#define DBO_MB_GET_TOUZHU_XX_RET		319									//Ͷע��ϸ��Ϣ����

#define DBR_MB_TOUZHU_CQSSC_ZHUIHAO		320									//�ֻ�׷��

#define DBR_GP_GET_DAILI_HUIKUI				322					//
#define DBO_GP_GET_DAILI_HUIKUI_RET			323					//����

//�����콱
#define DBR_GP_DAILI_LJ						324					//
#define DBO_GP_DAILI_LJ_RET					325					//����
struct DBO_GetLhcQihaoRet
{
	int								n_t_qihao;					//3.0�����
	SYSTEMTIME						n_t_fdshijian;							//ϵͳʱ��
};

//////////////////////////////////////////////////////////////////////////////////
//�ʺŵ�¼
struct DBR_GP_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//EWIN�������
	TCHAR							szQQ[LEN_QQ];		//֤������
	TCHAR							szEmain[LEN_EMAIL];	//��ʵ����
	TCHAR							szIPhone[LEN_MOBILE_PHONE];	//��ʵ����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�޸Ļ���
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ͷ��
struct DBR_GP_ModifySystemFace
{
	//�û���Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyIndividual
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	//�ʺ�����
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//�û���Ϣ
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��ѯ����
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//������
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ�����
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת�˽��
struct DBR_GP_UserTransferScore
{
	DWORD                           dwUserID;                           //�û� I D
	SCORE							lTransferScore;						//ת�˽��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
	TCHAR                           szLogonPass[LEN_PASSWORD];          //��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};
//��ѯ��Ϸ�������
struct DBR_GP_QueryGameResult
{
	int							wKindID;							//��Ҫ��ѯ����ϷID
};

struct DBR_GP_GetUserFandian
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
};
//����
struct DBO_GR_GetUserFandianRet //��ȡ��Ʊ��ҷ���
{
// 	int								n_t_userid;
// 	int								n_t_typeid;
	int								n_t_kindid;
	DOUBLE							f_t_bonus;
	DOUBLE							f_t_bonusPercent;
	DOUBLE							f_t_fandian;
};

struct DBR_GP_SetUserBonus
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
	DOUBLE							f_bonuspercent;
};
//����
struct DBO_GR_SetUserBonusRet //��ȡ��Ʊ��ҷ���
{
	int								n_t_result;
};

struct DBR_GP_GetCpUser
{
	int								n_t_userid;
};
//����
struct DBO_GR_GetCpUserInfoRet //��ȡ��Ʊ�����Ϣ
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



//��ѯ��ֵ��ʾ
struct DBR_GP_GetChonzhiTishi
{
	DWORD							dwUserID;							//�û�ID 
};

//��ѯ������ʾ
struct DBR_GP_GetTixianTishi
{
	DWORD							dwUserID;							//�û�ID 
};

//��ѯͶע��ʾ
struct DBR_GP_GetTouzhuTishi
{
	DWORD							dwUserID;							//�û�ID 
};

/////////////////////////////////////////////////////////////////////////////////////
//����ʱʱ��
//��ע
struct DBR_GP_TouZhuCQSSC
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
	int								nSign;								//��ע��־
	int								bZhuihao;							//�Ƿ�׷��
	WORD							nIndex;								//index
	WORD							cbPlatform;							//ƽ̨
	DWORD							dwClientAddr;						//���ӵ�ַ
};
struct DBR_GP_TouZhuCQSSC_Dan
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
	DWORD							dwClientAddr;						//���ӵ�ַ
};
struct DBR_GP_TouZhuCQSSC_Zhuihao
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
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//////////////////////////////////////////////////////////////////////////////////
struct DBO_GR_GetMapBonusRet
{
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
	int								n_t_bonustype;
};
//���ز�ѯϵͳʱ��
struct DBO_GR_GetSystemTime
{
	SYSTEMTIME						ServerTime;							//ϵͳʱ��
};

//���ز�ѯϵͳʱ��
struct DBO_GR_GetWinUserID
{
	int								nUserID;							//���ID
	DOUBLE							nWinZhushu;							//�н����
	DOUBLE							nZhushu;							//Ͷע���
	int								nState;
	TCHAR							szQishu[30];
	int								nType;
	DOUBLE							f_yingkui;
};

//��ѯ����
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//������Ϣ
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//������Ŀ
	DWORD							dwOnLineCountSum;					//��������
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//��������
};
///////////////////////////////////////////////////////////////////////////////////////
//Ͷע��־��
struct DBR_GP_GetTouzhuLogCount
{
	DWORD							dwUserID;							//�û�ID
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//������־��
struct DBR_GP_GetTixianLogCount
{
	DWORD							dwUserID;							//�û�ID
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//��ֵ��־��
struct DBR_GP_GetChongzhiLogCount
{
	DWORD							dwUserID;							//�û�ID
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//��ֵ��־��
struct DBR_GP_GetHYXXLogCount
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
//ӯ����־��
struct DBR_GP_GetYingkuiLogCount
{
	int								dwUserID;							//�û�ID
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//ӯ����ϸ��
struct DBR_GP_GetYingkuiMxCount
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//����ӯ����
struct DBR_GP_GetQiPaiYingkuiMxCount
{
	DWORD							dwUserID;							//�û�ID
	int								nTypeID;					
	int								nByTime;					
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

//��ѯͶע��־��
struct DBR_GP_GetXJTZHLogCount
{
	int							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	bool							bTime;								//ͨ��ʱ���ѯ
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//��ѯͶע��־��
struct DBR_GP_CHKXJTZHLogCount
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
struct DBO_GR_GetXJTZHLogCountRet
{
	DWORD							dwCount;							//����
};
//��ѯͶע��־��
struct DBR_GP_GetXJYKLogCount
{
	DWORD							dwUserID;							//�û�ID
	int								n_t_type;							//��ѯ״̬ 0 :��������   1 :����ID ��ѯ   2 �����˻���ѯ0
	int								n_t_user_id;						//��ѯID 
	TCHAR							s_t_account[30];					//��ѯ�˻�
	bool							bTime;								//ͨ��ʱ���ѯ
	int								nXiaJiaID;							//�¼�ID 
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
	int								nType;								//0:Ͷע  1 ����  2�¼�����  3 �н� 4 ��ֵ 5ȡ�� 6����ӯ��
};
//����
struct DBO_GR_GetXJYKLogCountRet
{
	DWORD							dwCount;							//����
};
//��ѯͶע��־��
struct DBR_GP_GetXJYKTjCount
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
struct DBO_GR_GetXJYKTjCountRet
{
	DWORD							dwCount;							//����
};
//��ѯͶע��־��
struct DBR_GP_GetXJYXTjCount
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
struct DBO_GR_GetXJYXTjCountRet
{
	DWORD							dwCount;							//����
};
//��ѯ�¼���ֵ��־��
struct DBR_GP_GetXjChzhLogCount
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
struct DBO_GR_GetXjChzhLogCountRet
{
	DWORD							dwCount;							//����
};

//��ѯ�¼�������־��
struct DBR_GP_GetXjTxLogCount
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
struct DBO_GR_GetXjTxLogCountRet
{
	DWORD							dwCount;							//����
};
// ����
// struct DBO_GR_GetCpUserInfoRet //��ȡ��Ʊ�����Ϣ
// {
// 	int								n_t_userid;
// 	TCHAR							s_t_account[30];
// 	int								n_t_type;
// 	DOUBLE							f_t_fandian;
// 	TCHAR							s_t_qq[11];
// 	int								n_t_daili_id;
// 	int								n_t_online;
// 	DOUBLE							f_t_yue;
// 	DOUBLE							f_t_dongjie;
// 	int								n_t_ban;
// 
// };

struct DBR_GP_GetChongzhiType
{
	int								nUserID;					//�û�ID
	BYTE							cbType;						//����
};

struct DBO_GP_GetChongzhiTypeRet
{
	TCHAR							s_t_yinhang[60];				//����
	TCHAR							s_t_kaihuren[60];				//������
	TCHAR							s_t_zhanghao[60];				//�˺�
	TCHAR							s_t_web[60];					//��ҳ
};
struct DBR_GP_GetQukuanInfo				//��ȡȡ����Ϣ
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_GetQukuanInfoRet
{
	DOUBLE							f_t_yue;						//���
	TCHAR							s_t_yinhang[60];				//����
	TCHAR							s_t_kaihuren[60];				//������
	TCHAR							s_t_zhanghao[60];				//�˺�
	BYTE							n_t_isvip;						//�Ƿ񳬼�VIP
};
//��ȡ�������ַ���
struct DBO_GP_GetAllYinHangNameRet
{
	TCHAR							s_t_yinhang[60];				//����
	int								n_QiYong;						//�Ƿ�����
};

struct DBR_GP_GetLastYue			//�����Ϣ
{
	DWORD							dwUserID;						//�û�ID
};
struct DBR_GP_GetMoreRecord
{
	int							n_t_type;					//����   1 ת���   2 ת�ʽ�
};

struct DBO_GP_GetMoreRecordRet
{
	WORD							wKindID;							//��������
	TCHAR							wPeriod[30];							//��
	TCHAR							szLotNum[45];						//��������
	TCHAR							szShijian[30];							//ʱ��
};
struct DBR_GP_GetDailiHuikui
{
	int							n_t_userid;					//�û�ID
};

struct DBO_GP_GetDailiHuikuiRet
{
	BYTE						cbNewRegUser;					//����
	DOUBLE						f_t_touzhu;						//Ͷע
	DOUBLE						f_t_chongzhi;					//��ֵ
};
struct DBR_GP_DailiLingjiang
{
	int							n_t_userid;					//�û�ID
};

struct DBO_GP_DailiLingjiangRet
{
	int							n_t_userid;					//�û�ID 
	BYTE						n_t_state;					//����״̬ 0 δ  1����
	DOUBLE						f_t_jine;						//���
};

struct DBO_GP_GetLastYueRet
{
	DOUBLE							f_t_yue;						//���
	DOUBLE							f_t_dongjie;					//����
	DOUBLE							f_t_score;						//��Ϸ��
};
struct DBR_GP_QuitGame
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_QuitGameRet
{
	int								nResult;
	int								nUserID;
};

struct DBR_GP_UserQiandao
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_UserQiandaoRet
{
	int							n_t_res;
};

struct DBR_GP_GetUserHuanlesong
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_GetUserHuanlesongRet
{
	int							nRes;
	DOUBLE						f_t_xiaofei;
	DOUBLE						f_t_prize;
	int							n_t_count;
	int							n_t_XXcount;
};

struct DBR_GP_GetLingJiang
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_GetLingJiangRet
{
	int							n_t_got;
	DOUBLE							f_t_jine;						//���
};


struct DBR_GP_DoQukuan				//ȡ��
{
	DWORD							dwUserID;						//�û�ID
	DOUBLE							fJine;							//���
	TCHAR							szQukuanPass[33];				//ȡ������
};

struct DBO_GP_DoQukuanRet
{
	int								nResult;						//����ֵ
	TCHAR							szDesc[126];					//ȡ����ʾ
};

struct	DBR_GP_GetXJCHZHLog									//�¼���ֵ��־
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

struct DBO_GR_GetXJCHZHLogRet
{
	int								n_t_id;
	int								n_t_user_id;
	TCHAR							s_t_account[30];
	DOUBLE							f_t_jine;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	TCHAR							s_t_pingtai[30];
};
struct	DBR_GP_GetXJTxLog    //�¼�����
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

struct DBO_GR_GetXJTxLogRet   // �¼����ַ���
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

//����
struct DBO_GR_CHKXJTZHLogCountRet
{
	DWORD							dwCount;							//����
};

struct	DBR_GP_GetXJYKLog
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

struct DBO_GR_GetXJYKLogRet
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
struct	DBR_GP_GetXJYKTj
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
struct	DBR_GP_GetXJYXTj
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

struct DBR_GP_GetDaLiBao				//��ȡ�����
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_GetDaLiBaoRet
{
	int							f_t_jine;						//���
};

struct DBR_GP_GetZaJinDan
{
	DWORD							dwUserID;						//�û�ID
	int								nType;						//��type;
};

struct DBO_GP_GetZaJinDanRet
{
	int							f_t_jine;						//���
};
struct DBR_GP_CanDaLiBao//�жϴ�����ʸ�
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_CanDaLiBaoRet
{
	DOUBLE						f_t_xiaofei;
	DOUBLE						f_t_chongzhi;
	DOUBLE						f_t_yue;
	int							n_t_got;						//���
};

struct DBR_GP_CanZaJinDan
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_CanZaJinDanRet
{
	DOUBLE						f_t_xiaofei;
	int							n_t_got;						//���
};

struct DBR_GP_CanLuckyZhuan							//����ת���ʸ�
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_CanLuckyZhuanRet
{
	DOUBLE						f_t_xiaofei;
};

struct DBR_GP_GetLuckyZhuan						//����ת��
{
	DWORD							dwUserID;						//�û�ID
};

struct DBO_GP_GetLuckyZhuanRet
{
	int							n_t_got;
	DOUBLE							f_t_jine;						//���
};

struct DBO_GR_GetXJYKTjRet
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
struct DBO_GR_GetXJYXTjRet
{
	int								n_t_id;
	TCHAR							s_t_account[30];
	int								n_t_type;
	int								n_t_daili_id;

	DOUBLE							f_t_fandian_ze;
	DOUBLE							f_t_qipai_ze;
	DOUBLE							f_t_tuandui_ye;
};

struct	DBR_GP_GetTouzhuLog
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};

struct	DBR_GP_GetXJTZHLog
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
struct	DBR_GP_CHKXJTZHLog
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

struct	DBR_GP_GetXJTZHLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
};

struct	DBR_GP_GetXJTxLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
};

struct	DBR_GP_GetXJTxLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};

struct	DBR_GP_GetXJTZHLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};
struct	DBR_GP_GetXJYKLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С

};
struct	DBR_GP_GetXJCHZHLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
};

struct	DBR_GP_GetXJCHZHLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};

struct	DBR_GP_GetXJYKLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};
struct	DBR_GP_GetXJYKTjByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
};

struct	DBR_GP_GetXJYKTjByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};
struct	DBR_GP_CHKXJTZHLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;							//�¼�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
};
struct	DBR_GP_GetHyShj //��ȡ��Ա����
{
	int							dwUserID;							//�û�ID
	BYTE						cbOnlyToday;						//ֻ������   0 ����    1 ��
	TCHAR						szTimeStart[30];					//��ʼʱ��
	TCHAR						szTimeEnd[30];						//����ʱ��
};
//����
struct DBO_GR_GetHyShjRet
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

struct	DBR_GP_CHKXJTZHLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};

struct DBO_GR_GetXJTZHLogRet
{
	int								n_t_id;
	TCHAR							s_t_account[32];
	int								n_t_type;
	DOUBLE							f_t_touzhu_ze;
	DOUBLE							f_t_yingkui_ze;
};
struct DBO_GR_CHKXJTZHLogRet
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


struct	DBR_GP_GetHYXXLog
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
struct	DBR_GP_GetHYXXLogByID
{
	DWORD							dwUserID;							//�û�ID
	int								nXiaJiID;								//�¼�ID 
};
struct	DBR_GP_GetHYXXLogByAct
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szAccount[33];							//�¼��˻�
};
struct	DBR_GP_SWDaili
{
	DWORD							dwUserID;							//�û�ID
	DWORD							nAgentID;							//����ID
	int 							nDaili;							//�Ƿ����
};
struct DBO_GP_SWDaili_RET
{
	LONG							lResult;
};
struct DBR_GetXjPeie
{
	int								n_t_userid;						//ID
};
struct DBO_GetXjPeieRet
{
	int								n_t_peie_1;					//3.0�����
	int								n_t_peie_2;					//2.8�����
	int								n_t_peie_3;					//2.7�����
	int								n_t_peie_s_1;				//3.0�������
	int								n_t_peie_s_2;				//2.8�������
	int								n_t_peie_s_3;				//2.7�������
};
struct DBR_SetXjPeie
{
	int								n_t_userid;						//ID
	int								n_t_xj_id;						//�¼�ID
	int								n_t_peie1;
	int								n_t_peie2;
	int								n_t_peie3;
};
struct DBO_SetXjPeieRet
{
	int								n_t_result;					//����
	TCHAR							s_t_desc[126];				//����
};

struct	DBR_GP_HYXX_ZhuanZhang
{
	int								n_t_user_id;							//�Լ�ID
	int								n_t_target_id;							//Ŀ��ID
	DOUBLE 							f_t_jine;								//���
	TCHAR							s_t_password[126];							//����
};
struct DBO_GP_HYXX_ZhuanZhang_RET
{
	int							lResult;
};


struct	DBR_GP_XGHYFandian
{
	int								dwUserID;							//�û�ID
	int								n_xiaji_id;							//�¼�ID
	DOUBLE							f_Fandian;							//����
};
struct DBO_GP_XGHYFandian_RET
{
	LONG							lResult;
	DOUBLE							f_Fandian;
	TCHAR							s_Account[33];
};


//������־
struct	DBR_GP_GetTixianLog
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//��ֵ��־
struct	DBR_GP_GetChongzhiLog
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//��ֵ��־
struct	DBR_GP_GetYingkuiLog
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//ӯ����ϸ
struct	DBR_GP_GetYingkuiMx
{
	int							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	int							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
//ӯ����ϸ
struct	DBR_GP_GetQiPaiYingkui
{
	DWORD							dwUserID;							//�û�ID
	int								nPage;								//ҳ
	int								nSize;								//ҳ�Ĵ�С
	bool							bByTime;							//ͨ��ʱ��
	TCHAR							szTimeStart[30];					//��ʼʱ��
	TCHAR							szTimeEnd[30];						//����ʱ��
};
struct DBR_GetUrlFandian
{
	int								n_t_userid;						//ID
};

struct DBR_GP_CancelTouzhu
{
	int								n_t_userid;							//�û�ID
	int								n_t_id;								//ͶעID
	int								n_t_kindid;
	TCHAR							s_t_qihao[30];
};
struct DBR_GetPeie
{
	int								n_t_userid;						//ID
};
struct DBR_GetPeieRet
{
	int								n_t_peie_1;
	int								n_t_peie_2;
	int								n_t_peie_3;
	int								n_t_peie_s_1;
	int								n_t_peie_s_2;
	int								n_t_peie_s_3;
};

struct DBR_GP_AddHuiYuan
{
	int								n_t_daili_id;
	int								n_type;
	DOUBLE							f_t_fandian;
	TCHAR							s_t_account[33];
	TCHAR							s_t_login_pass[33];
	TCHAR							s_t_qukuan_pass[33];
	TCHAR							s_t_qq[33];
};

struct DBR_GP_SetWebFandian
{
	int								n_t_userid;
	DOUBLE							f_t_fandian;
};
struct DBO_GR_SetWebFandianRet
{
	int								n_t_res;
	DOUBLE							f_t_fandian;

};

struct DBO_GR_AddHuiYuanRet
{
	int								n_t_res;
	TCHAR							s_t_Desc[126];
};

struct	DBR_GP_GetUserInfo
{
	DWORD							dwUserID;							//�û�ID
};

struct DBO_GR_GetUserInfoRet
{
	TCHAR							s_t_qq[12];	
	SYSTEMTIME						t_reg_time;						// ע��ʱ��
	TCHAR							s_t_sjqq[12];
	DOUBLE							f_t_yongjin;					//ͶעӶ��
	DOUBLE							f_t_yongjinze;					//ͶעӶ���ܶ�
	DOUBLE							f_t_fenhong;					//�ֺ�
	DOUBLE							f_t_fenhongze;					//�ֺ��ܶ�
	DOUBLE							f_t_qipaiye;					//�������
	DOUBLE							f_t_kuisunyongjin;					//����Ӷ��
	DOUBLE							f_t_kuisunyongjinze;					//����Ӷ���ܶ�
	
	SYSTEMTIME						t_ksyj_time;						// ����Ӷ��ʱ��
	SYSTEMTIME						t_fh_time;						//	�ֺ�ʱ�� 
	SYSTEMTIME						t_yj_time;						// Ӷ��ʱ��
};

struct DBR_GP_GetNews
{
	int							n_t_id;						//ID
	int							n_t_ccount;			//����
};

struct DBO_GP_GetNewsRet
{
	TCHAR						s_t_news[512];				//����
	SYSTEMTIME					s_t_time;					//����ʱ��
};

//ת��
struct DBR_GP_ZhuanHuan
{
	int							n_t_userid;					//�û�ID
	int							n_t_type;					//����   1 ת���   2 ת�ʽ�
	DOUBLE						f_t_jine;					//���
};

struct DBO_GP_ZhuanHuanRet
{
	int							nResult;
	DOUBLE						f_t_yue;
	DOUBLE						f_t_score;
};
struct DBR_GP_AlterGender
{
	int								n_t_userid;					//�û�ID
	BYTE 							cbGender;					//�Ա�
};

struct DBO_GP_AlterGenderRet
{
	BYTE						n_t_return;					//����״̬ 0 δ  1����
};

//�޸�QQ 
struct	DBR_GP_XGqq
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szQQ[11];							//��QQ 
};
struct DBO_GR_XG_QQ_RET
{
	LONG							lResult;
};

struct	DBR_GP_XG_Loginpass
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szOldPass[33];						//������
	TCHAR							szNewPass[33];						//������
};
struct DBO_GR_XG_Loginpass_RET
{
	int							lResult;
};

struct	DBR_GP_XG_Qukuanpass
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szOldPass[33];						//������
	TCHAR							szNewPass[33];						//������
};
struct DBO_GR_XG_Qukuanpass_RET
{
	int							lResult;
};

struct	DBR_GP_LockMachine
{
	DWORD							dwUserID;							//�û�ID
	BYTE						cbLock;									//����
};
struct DBO_GR_LockMachine_RET
{
	int							lResult;
};

struct	DBR_GP_SetQukuanProtect
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szQukuanPass[33];						//����
	TCHAR							szQukuanAsk[33];						//����
	TCHAR							szQukuanAnswer[33];						//��
};
struct DBO_GR_SetQukuanProtect_RET
{
	int							lResult;
};
struct	DBR_GP_QueryMyProTect
{
	DWORD							dwUserID;							//�û�ID
};
struct DBO_GR_QueryMyProTect_RET
{
	int							lResult;
	TCHAR						szQuestion[33];
	TCHAR						szAnswer[33];
};

struct	DBR_GP_SetQukuanZhanghao
{
	DWORD							dwUserID;							//�û�ID
	TCHAR							szKaihuYinghang[33];						//����
	TCHAR							szKaihuRen[33];						//����
	TCHAR							szYinhangZhanghu[33];						//��
	TCHAR							szQukuanPass[33];						//��
};

struct	DBO_GP_QueryMyYinHang
{
	DWORD							dwUserID;							//�û�ID
};
struct DBR_GR_QueryMyYinHang_RET
{
	int							lResult;
	TCHAR						szKaihuYinghang[33];
	TCHAR						szKaihuRen[33];
	TCHAR						szYinhangZhanghu[33];				
};

struct DBO_GR_SetQukuanZhanghao_RET
{
	int							lResult;
};

struct DBR_GP_GetTouzhu
{
	int							n_t_id;						//ID
};
struct DBR_MB_GetTouzhu
{
	int							n_t_id;						//ID
};

struct DBO_MB_GetTouzhuRet
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


struct DBO_GP_GetTouzhuRet
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

//���ּ�¼����
struct DBO_GR_GetTixianLogRet
{
	int								n_t_id;
	SYSTEMTIME						n_t_time;
	TCHAR							n_t_yinhang[30];
	DOUBLE							f_t_jine;
	TCHAR							s_t_kaihuren[30];
	int								n_t_state;
	TCHAR							s_t_note[30];
};
struct DBO_GR_GetHYXXLogRet
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


struct DBO_GR_GetTouzhuLogRet
{
	int								n_t_id;
	int								n_t_type;
	int								n_t_kind;
	TCHAR							szQishu[30];
	int								n_t_zhushu;
	int								n_t_moshi;
	int								n_t_beishu;
	SYSTEMTIME							n_t_time;
	int								n_t_state;
	int								n_t_winzhushu;
	DOUBLE							f_t_yingkui;
	int								n_t_zhuihao;
};

struct DBO_GR_GetChongzhiRet
{
	int								n_t_id;
	DOUBLE							f_t_jine;
	SYSTEMTIME							n_t_time;
	int								n_t_state;
	TCHAR							s_t_pingtai[30];

};
struct DBO_GR_GetYingkuiLogRet
{
	DOUBLE							f_t_chongzhi;
	DOUBLE							f_t_qukuan;
	DOUBLE							f_t_touzhu;
	DOUBLE							f_t_fandian;
	DOUBLE							f_t_zhongjiang;
	DOUBLE							f_t_huodong;
	DOUBLE							f_t_chedan;
};

//ӯ����ϸ
struct DBO_GR_GetYingkuiMxRet
{
	int								n_t_dingdan;
	DOUBLE							f_t_yingkui;
	DOUBLE							f_t_yue;
	SYSTEMTIME						t_t_yingkuitime;
	TCHAR							s_t_type[30];
	TCHAR							s_t_zhaiyao[126];
};
//����ӯ����ϸ
struct DBO_GR_GetQiPaiYingkuiRet
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

//��������
struct DBO_GR_CancelTouzhuRet
{
	int								nResult;
	int								n_t_userid;
};
//��ֵ��ʾ����
struct DBO_GR_ChongzhiTishiRet
{
	DOUBLE							fJine;
	int								nState;
};
//������ʾ����
struct DBO_GR_TixianTishiRet
{
	DOUBLE							fJine;
	int								nState;
};
//Ͷע��ʾ����
struct DBO_GR_TouzhuTishiRet
{
	int								nWinZhushu;
	int								nState;
	TCHAR							szQishu[30];
	int								nType;
};
struct DBR_GP_GetCpUserInfo
{
	int								nUserID;
};
//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//��¼���
#define DBO_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GP_LOGON_FAILURE		101									//��¼ʧ��

//�������
#define DBO_GP_USER_FACE_INFO		110									//�û�ͷ��
#define DBO_GP_USER_INDIVIDUAL		111									//�û�����

//��������
#define DBO_GP_USER_INSURE_INFO		120									//��������
#define DBO_GP_USER_INSURE_SUCCESS	121									//���гɹ�
#define DBO_GP_USER_INSURE_FAILURE	122									//����ʧ��

//�б���
#define DBO_GP_GAME_TYPE_ITEM		130									//������Ϣ
#define DBO_GP_GAME_KIND_ITEM		131									//������Ϣ
#define DBO_GP_VIRTUAL_COUNT        132                                 //�������� 
#define DBO_GP_GAME_LIST_RESULT		133									//���ؽ��

//������
#define DBO_GP_OPERATE_SUCCESS		500									//�����ɹ�
#define DBO_GP_OPERATE_FAILURE		501									//����ʧ��


//��¼�ɹ�
struct DBO_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwExperience;						//������ֵ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//�û��ɼ�
	SCORE							lScore;								//�û�����
	SCORE							lInsure;							//�û�����
	SCORE                           lUserMedal;                         //�û�����

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��

	//��չ��Ϣ
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //��ַ����

	int								n_type;
	DOUBLE							f_fandian;
	DOUBLE							f_dongjie;
	DOUBLE							f_yue;
	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ

};

//��¼ʧ��
struct DBO_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//ͷ����Ϣ
struct DBO_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
};

//��������
struct DBO_GP_UserIndividual
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��������
struct DBO_GP_UserInsureInfo
{
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//���гɹ�
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ�����
	SCORE							lSourceInsure;						//ԭ�����
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lVariationScore;					//��ұ仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct  DBO_GP_UserInsureFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct DBO_GP_OperateFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct DBO_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};
//��ȡ����
struct DBO_GP_GetBankName
{
	LONG							lResultCode;						//��������
	TCHAR							szYinHangName[128];					//�ɹ���Ϣ
};

//������ѯ���
struct DBO_GP_QueryLotResult
{
	WORD							wKindID;							//��������
// 	TCHAR							szCode[32];							//
// 	TCHAR							szPlayKey[32];						//��Ϸ��ƴ
// 	TCHAR							cbSerialID[32];							//�������� ��SerialData �����Period
// 	TCHAR							wSerialDate[32];						//����
	TCHAR							wPeriod[30];							//��
	TCHAR							szLotNum[45];						//��������
	TCHAR							szShijian[30];							//ʱ��
// 	SYSTEMTIME						LotTime;							//ʱ��
// 	BYTE							cbStatus;							//״̬
};
struct DBO_GP_GetKefuUrlRet
{
	int								n_t_result;					//����
	TCHAR							s_t_desc[256];				//����

};
//����ʱʱ��Ͷע
struct DBO_GR_TouzhuResult
{
	int								nUserID;							//�û�ID
	LONG							lResultCode;						//����ֵ
	TCHAR							szDesc[126];						//��������
	int								nSign;								//��־
};
//��ѯͶע��־����
struct DBO_GR_GetTouzhuLogCountRet
{
	DWORD							dwTouzhuLogCount;					//����
};
//��ѯ������־����
struct DBO_GR_GetTixianLogCountRet
{
	DWORD							dwTixianLogCount;					//����
};
//��ѯ��ֵ��־����
struct DBO_GR_GetChongzhiLogCountRet
{
	DWORD							dwChongzhiLogCount;					//����
};
//ӯ����ֵ��־����
struct DBO_GR_GetYingkuiLogCountRet
{
	DWORD							dwYingkuiLogCount;					//����
};
//ӯ����ϸ��������
struct DBO_GR_GetYingkuiMxCountRet
{
	DWORD							dwYingkuiMxCount;					//����
};
//ӯ����ϸ��������
struct DBO_GR_GetQiPaiYingkuiCountRet
{
	DWORD							dwYingkuiMxCount;					//����
};
//��ѯ��Ա��־����
struct DBO_GR_GetHYXXLogCountRet
{
	DWORD							dwHYXXLogCount;					//����
};

struct DBO_GetRegUrl
{
	TCHAR							s_reg_url[126];					//ע������
	DOUBLE							f_t_fandian;
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct DBO_GP_GameType
{
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//��������
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
};

//��������
struct DBO_GP_VirtualCount
{
	WORD                           wServerID;                           //�����ʶ
	DWORD                          dwVirtualCount;                       //��������  
};

//���ؽ��
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//�ɹ���־
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//�ֻ����ݰ�

//��¼����
#define	DBR_MB_LOGON_ACCOUNTS		500									//�ʺŵ�¼
#define DBR_MB_REGISTER_ACCOUNTS	501									//ע���ʺ�

//��¼���
#define DBO_MB_LOGON_SUCCESS		600									//��¼�ɹ�
#define DBO_MB_LOGON_FAILURE		601									//��¼ʧ��

//////////////////////////////////////////////////////////////////////////////////

//�ʺŵ�¼
struct DBR_MB_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_MB_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//��¼�ɹ�
struct DBO_MB_LogonSuccess
{
	//�û�����
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwExperience;						//������ֵ
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ

	//��չ��Ϣ
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //��ַ����
};


//��¼ʧ��
struct DBO_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

struct DBR_GP_GetMyMapBonus
{
	int								n_t_userid;
};
struct DBO_GP_GetMyMapBonusRet
{
	int								n_t_type;
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()
#endif