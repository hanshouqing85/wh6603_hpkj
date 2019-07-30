#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include <map>
#include "LogFile.h"
#include "GameRule.h"
//��Ʊ����
typedef enum {
	CZChongQingSSC=1,		//����ʱʱ��
	CZJiangXiSSC=2,			//����ʱʱ��
	CZ3D=3,					//����3D
	CZXinJiangSSC = 4,		//�½�ʱʱ��
	CZPaiLie3,			//����3
	CZGD11Xuan5,		//�㶫11ѡ5
	CZCQ11Xuan5,		//����11ѡ5
	CZSD11Xuan5,		//ɽ��11ѡ5
	CZJX11Xuan5,		//����11ѡ5
	CZKUAILE8,				//��������8
	CZ_FENFEN_CAI,			//�ֲַʣ�5���ӿ�����24Сʱ��ͣ��ÿ��288��
	CZ_WUFEN_CAI,			//5�ֲ�
	CZ_BJKUAICHE,			//�����쳵
	CZ_PK10=13,		//PK10
	CZ_LIUHECAI = 14,
	CZ_QiXingCai = 15,			//���ǲ�--lly
	CZHLJ11Xuan5=16,		//������11ѡ5
	CZXingYun28,
	CZ_HGYDWFC=20,
	CZ_TianJinSSC = 23,	//���ʱʱ��
	CZCount

}CaiZhong;

//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_LOAD_GAME_LIST			1									//�����б�
#define IDI_CONNECT_CORRESPOND		2									//������ʶ
#define IDI_COLLECT_ONLINE_INFO		3									//ͳ������
#define IDI_LOAD_VIRTUAL_COUNT      4                                   //�������� 
#define IDI_GET_SQL_TIME		    5                                   //��ȡ���ݿ�ʱ��
#define IDI_GET_NEWS			    6                                   //��ȡ���Ź���
#define IDI_GET_ZHONGJIANG		    7                                   //��ȡ�н�USERID

//////////////////////////////////////////////////////////////////////////////////


struct tagUserBonusInfo 
{
	DBO_GR_GetUserFandianRet	UserFandian[2000];
};
map<int, tagUserBonusInfo*> mapUserBonusInfo;

map<int ,tagLuckNum*>			m_AllLuckNum;
map<int ,DWORD>	m_UserSocketID;
map<CString ,DWORD> m_UserLogonTickCount;
map<CString ,WORD> m_UserLogonBlackList;

map<int ,DWORD> m_UserYue;

map<int ,CString> m_mapNews;

struct tagMapBonus
{
	DBO_GR_GetMapBonusRet MapBonus;
};

map<int ,tagMapBonus* > mapMapBonus;
//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_bNeekCorrespond=true;
	m_bShowServerStatus=false;
	//״̬����
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;
	ZeroMemory(&m_MyLuckNum,sizeof(m_MyLuckNum));


	for(int i = 0;i < 13;i++)
		m_dwLuckyNumTickCount[i] = GetTickCount();
	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

	mapMapBonus.clear();
	//����ʱ��
	ASSERT(m_pITimerEngine!=NULL);
	//ͳ�ƴ�������������û���õ�����ʱȥ��
	//m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,m_pInitParameter->m_wCollectTime*1000L,TIMES_INFINITY,0);

	//����ʱ�� ��������
	m_pITimerEngine->SetTimer(IDI_LOAD_VIRTUAL_COUNT,m_pInitParameter->m_wLoadVCountTime*1000L,TIMES_INFINITY,0);

	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	CString szFileName;
	GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	szFileName=szPath;
	int nIndex = szFileName.ReverseFind(TEXT('\\'));
	szFileName = szFileName.Left(nIndex);
	szFileName += TEXT("\\ServerConfig.ini");

	//��ȡ����
	m_bShowServerStatus = (GetPrivateProfileInt(TEXT("ServerStatus"),TEXT("ShowServerStatus"),0,szFileName) != 0);
	
	m_pITimerEngine->SetTimer(IDI_GET_SQL_TIME,300*1000L,TIMES_INFINITY,0);
	m_pITimerEngine->SetTimer(IDI_GET_ZHONGJIANG,30*1000L,TIMES_INFINITY,0);
	m_pITimerEngine->SetTimer(IDI_GET_NEWS,30*1000L,TIMES_INFINITY,0);

	for( map<CString ,DWORD>::iterator pos = m_UserLogonTickCount.begin(); pos!=m_UserLogonTickCount.end(); ++pos)
	{
		pos->second = 0;
	}				


	return true;
}

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//״̬����
	m_bNeekCorrespond=true;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�б����
	m_ServerListManager.ResetServerList();

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_LOAD_DB_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case CT_CONNECT_CORRESPOND:		//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
//��ȡϵͳʱ��
bool CAttemperEngineSink::OnGetSqlserverTimer()
{
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_SYS_TIME,0,NULL,NULL);
	return true;

}
//��ȡϵͳʱ��
bool CAttemperEngineSink::OnGetNews()
{
	DBR_GP_GetNews News;
	ZeroMemory(&News,sizeof(News));
	News.n_t_id = 10000;
	News.n_t_ccount = 5;
	
	return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_NEWS,0,&News,sizeof(News));

}
bool CAttemperEngineSink::OnGetMapBonusRet()
{
	return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_MAP_BONUS,0,NULL,NULL);
}
//��ȡϵͳʱ��
bool CAttemperEngineSink::OnGetWinUserID()
{
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_WIN_USER,0,NULL,NULL);

	return true;
}
//ʱ���¼�
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_GET_SQL_TIME:
		{
			OnGetSqlserverTimer();
			return true;
		}
	case IDI_GET_ZHONGJIANG:
		{
			return OnGetWinUserID();
		}
	case IDI_GET_NEWS:
		{
		//	return true;
			return OnGetNews();

		}
	case IDI_LOAD_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case IDI_LOAD_VIRTUAL_COUNT:    //��������
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_VIRTUAL_COUNT,0,NULL,0);

			return true;
		}
	case IDI_CONNECT_CORRESPOND:	//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case IDI_COLLECT_ONLINE_INFO:	//ͳ������
		{
			//��������
			DBR_GP_OnLineCountInfo OnLineCountInfo;
			ZeroMemory(&OnLineCountInfo,sizeof(OnLineCountInfo));

			//��ȡ����
			OnLineCountInfo.dwOnLineCountSum=m_ServerListManager.CollectOnlineInfo();

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=OnLineCountInfo.wKindCount++;
					OnLineCountInfo.OnLineCountKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//����ж�
				if (OnLineCountInfo.wKindCount>=CountArray(OnLineCountInfo.OnLineCountKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��������
			WORD wHeadSize=sizeof(OnLineCountInfo)-sizeof(OnLineCountInfo.OnLineCountKind);
			WORD wSendSize=wHeadSize+OnLineCountInfo.wKindCount*sizeof(OnLineCountInfo.OnLineCountKind[0]);
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ONLINE_COUNT_INFO,0,&OnLineCountInfo,wSendSize);

			return true;
		}
	}

	return false;
}

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));


	for (map<int,DWORD>::iterator it=m_UserSocketID.begin();it!=m_UserSocketID.end();++it)
	{
		if(it->second == dwSocketID)
		{
			int nUserID = it->first;

			DBR_GP_QuitGame LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = nUserID;

			CString strLog;
			strLog.Format(L"[%d]=[%d]",nUserID,dwSocketID);
			CTraceService::TraceString(strLog,TraceLevel_Normal);

			m_UserSocketID[nUserID]=0;
			 m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUIT_GAME,0,&LogCount,sizeof(LogCount));

			return true;

		}
	}
	//m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:			//��¼����
		{
			return OnTCPNetworkMainPCLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainPCServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_USER_SERVICE:	//��������
		{
			return OnTCPNetworkMainPCUserService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_REMOTE_SERVICE:	//Զ�̷���
		{
			return OnTCPNetworkMainPCRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_LOGON:			//��¼����
		{
			return OnTCPNetworkMainMBLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainMBServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GP_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBPCLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_CP_USER_INFO_RET:
		{
			return OnGetCpUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_MY_MAP_BONUS_RET:
		{
			return OnGetMyUserMapBonus(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_USER_FANDIAN_RET:
		{
			return OnGetUserFandianRet(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SET_USER_BONUS_RET:
		{
			return OnSetUserBonusRet(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_SYS_TIME_RET:
		{
			return OnGetSysTimeRet(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_WIN_USER_RET:
		{
			return OnGetWinUserIDRet(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_MAP_BONUS_RET:
		{
			return OnGetMapBonusRet(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_KEFU_URL_RET:
		{
			return OnGetKefuUrlRet(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBPCLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_FACE_INFO:			//�û�ͷ��
		{
			return OnDBPCUserFaceInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INDIVIDUAL:		//�û���Ϣ
		{
			return OnDBPCUserIndividual(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_INFO:		//��������
		{
			return OnDBPCUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_SUCCESS:	//���гɹ�
		{
			return OnDBPCUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_FAILURE:	//����ʧ��
		{
			return OnDBPCUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_SUCCESS:		//�����ɹ�
		{
			return OnDBPCOperateSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_FAILURE:		//����ʧ��
		{
			return OnDBPCOperateFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_TYPE_ITEM:			//��������
		{
			return OnDBPCGameTypeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_KIND_ITEM:			//��������
		{
			return OnDBPCGameKindItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_VIRTUAL_COUNT:          //��������
		{
			return OnDBPCLoadVirtualCount(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_LIST_RESULT:		//���ؽ��
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBMBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBMBLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_RESULT:			//��ѯ�����������
		{
			return OnDBQueryOpenResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_YINHANG_RET:			//��ѯ�����������
		{
			return OnDBQueryYinHangResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TOUZHU_CQSSC_RET:
		{
			return OnDBQueryTouzhuResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TOUZHU_CONTINUE_RET:
		{	
			return 	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CONTINUE_RET);
		}
	case DBO_GP_TOUZHU_LOG_COUNT_RET:
		{
			return OnDBQueryTouzhuLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_REG_URL_RET:
		{
			return OnDBQueryRegUrlResult(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GET_Peie_RET:
		{
			return OnDBQueryPeieResult(dwContextID,pData,wDataSize);

		}
	case DBO_GP_HYXX_GET_XJPEIE_RET:
		{
			return OnDBQueryXjPeieResult(dwContextID,pData,wDataSize);

		}
	case DBO_GP_HYXX_SET_XJPEIE_RET:
		{
			return OnDBQuerySetXjPeieResult(dwContextID,pData,wDataSize);

		}
	case DBO_GP_TIXIAN_LOG_COUNT_RET:
		{
			return OnDBQueryTixianLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHONGZHI_LOG_COUNT_RET:
		{
			return OnDBQueryChongzhiLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_HYXX_LOG_COUNT_RET:
		{
			return OnDBQueryHYXXLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_YINGKUI_LOG_COUNT_RET:
		{
			return OnDBQueryYingkuiLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_YINGKUI_MX_COUNT_RET:
		{
			return OnDBQueryYingkuiMxCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QP_YINGKUI_COUNT_RET:
		{
			return OnDBQueryQiPaiYingkuiCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XJTZH_LOG_COUNT_RET:
		{
			return OnDBQueryXITZHLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHKXJTZH_LOG_COUNT_RET:
		{
			return OnDBCHKXITZHLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XJYK_LOG_COUNT_RET:
		{
			return OnDBXJYKLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XJYK_TJ_COUNT_RET:
		{
			return OnDBXJYKTjCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XJYX_TJ_COUNT_RET:
		{
			return OnDBXJYXTjCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XJCHZH_LOG_COUNT_RET:
		{
			return OnDBXJCHZHLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XJTX_LOG_COUNT_RET:
		{
			return OnDBXJTxLogCountResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_CHONGZHI_TYPE_RET:
		{
			return OnDBGetChongzhiXinxiResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_QUKUAN_INFO_RET:
		{
			return OnDBGetQukuanInfoResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_YUE_INFO_RET:
		{
			return OnDBGetYueInfoResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_MORE_RECORD_RET:
		{
			return OnDBGetMoreRecordResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_DAILI_HUIKUI_RET:
		{
			return OnDBGetDailiHuikuiResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_DAILI_LJ_RET:
		{
			return OnDBDailiLingjiangResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUIT_GAME_RET:
		{
			return OnDBQuitGameResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_QIANDAO_RET:
		{
			return OnDBUserQiandaoResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_HUANLE_SONG_RET:
		{
			return OnDBUserHuanlesongResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_LING_JIANG_RET:
		{
			return OnDBUserLingjiangResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_DO_QUKUAN_RET:
		{
			return OnDBDoQukuanResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TOUZHU_LOG_RET:
		{
			return OnDBGetTouzhuLogResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_NEWS_RET:
		{
			return OnDBGetNewsResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ZHUAN_HUAN_RET:
		{
			return OnDBZhuanhuanResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJCHZH_LOG_RET:
		{
			return OnDBGetXJCHZHLogResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJTX_LOG_RET:
		{
			return OnDBGetXJTxLogResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJYK_LOG_RET:
		{
			return OnDBGetXJYKLogResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJYK_TJ_RET:
		{
			return OnDBGetXJYKTjResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJYX_TJ_RET:
		{
			return OnDBGetXJYXTjResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XJTZH_LOG_RET:
		{
			return OnDBGetXJTZHLogResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHKXJTZH_LOG_RET:
		{
			return OnDBCHKXJTZHLogResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TIXIAN_LOG_RET:
		{
			return OnDBGetTixianLogResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_HYXX_LOG_RET:
		{
			return OnDBGetHYXXLogResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHONGZHI_LOG_RET:
		{
			return OnDBGetChongzhiLogResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_YINGKUI_LOG_RET:
		{
			return OnDBGetYingkuiLogResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_YINGKUI_MX_RET:
		{
			return OnDBGetYingkuiMxResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QP_YINGKUI_RET:
		{
			return OnDBGetQiPaiYingkuiResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CANCEL_TOUZHU_RET:
		{
			return OnDBCancelTouzhuResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ADD_HY_RET:
		{
			return OnDBAddHuiyuanResult(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GET_LHC_QIHAO_RET:
		{
			return OnDBGetLhcQihaoResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SET_WEB_FANDIAN_RESULT:
		{
			return OnDBSetWebFandianResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_USER_INFO_RET:
		{
			return OnDBGetUserInfoResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHONGZHI_TISHI_RET:
		{
			return OnDBChongzhiTishiResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TIXIAN_TISHI_RET:
		{
			return OnDBTixianTishiResult(dwContextID,pData,wDataSize);
		}
	case  DBO_GP_TOUZHU_TISHI_RET:
		{
			return OnDBTouzhuTishiResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XG_QQ_RET:
		{
			return OnDBXGqqResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ALTER_GENDER_RET:
		{
			return OnDBXGGenderResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XGHY_FANDIAN_RET:
		{
			return OnDBXGHYFandianResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SW_DAILI_RET:
		{
			return OnDBSzhidailiResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_HYXX_ZHUANZHANG_RET:
		{
			return OnDBHyxxZhuanZhangResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XG_LOGIN_PASS_RET:
		{
			return OnDBXGLoginPassResult(dwContextID,pData,wDataSize);
		}
	case DBO_GR_LOCK_MACHINE_RET:
		{
			return OnDBLockMachineResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_XG_QUKUAN_PASS_RET:
		{
			return OnDBXGQukuanPassResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SET_QUKUAN_PROTECT_RET:
		{
			return OnDBSetQukuanProtectResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_MY_YINHANG_RET:
		{
			return OnDBQueryMyYinhangResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_MY_PROTECT_RET:
		{
			return OnDBQueryMyProtectResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SET_QUKUAN_ZHANGHAO_RET:
		{
			return OnDBSetQukuanZhanghaoResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GET_TOUZHU_XX_RET:
		{
			return OnDBGetTouzhuXXResult(dwContextID,pData,wDataSize);
		}
	case DBO_MB_GET_TOUZHU_XX_RET:
		{
			return OnDBMBGetTouzhuXXResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_HYSHJ_RET:
		{
			return OnDBGetHyShjResult(dwContextID,pData,wDataSize);
		}
 	case DBR_GP_GET_DA_LI_BAO_RET:
 		{
 			return OnDBGetDaLiBaoRet(dwContextID,pData,wDataSize);
 		}
	case DBR_GP_GET_ZA_JIN_DAN_RET:
		{
			return OnDBGetZaJinDanRet(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_LUCKY_ZHUAN_RET:
		{
			return OnDBGetLuckyZhuanRet(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CAN_DA_LI_BAO_RET:
		{
			return OnDBCanDaLiBaoRet(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CAN_ZA_JIN_DAN_RET:
		{
			return OnDBCanZaJinDanRet(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CAN_LUCKY_ZHUAN_RET:
		{
			return OnDBCanLuckyZhuanRet(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (m_bNeekCorrespond==true)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Э�������������ӹر��ˣ�%ld �����������"),m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (nErrorCode!=0)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("Э������������ʧ�� [ %ld ]��%ld �����������"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//��ʾ��Ϣ
		CTraceService::TraceString(TEXT("����ע����Ϸ��¼������..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//���ñ���
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		return true;
	}

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//ע�����
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//������Ϣ
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//Զ�̷���
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//ע���¼�
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_CHECK_USER_ONLINE_RET:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_CS_C_CheckUserOnlineRet));
			if (wDataSize!=sizeof(CMD_CS_C_CheckUserOnlineRet)) return false;

			CMD_CS_C_CheckUserOnlineRet* pCheckUserOnline = (CMD_CS_C_CheckUserOnlineRet*)pData;

			DWORD dwUserID = pCheckUserOnline->dwUserID;
			if(m_UserSocketID[dwUserID]!=0)
			{
				CMD_GP_KillSocket MyKillSocket;
				ZeroMemory(&MyKillSocket,sizeof(MyKillSocket));
				MyKillSocket.cbResult = 1;
				_sntprintf(MyKillSocket.szMessage,CountArray(MyKillSocket.szMessage),TEXT("�����˺����ڱ𴦵�¼������ǿ�����ߣ�"));
				m_pITCPNetworkEngine->SendData(m_UserSocketID[dwUserID],MDM_GP_USER_SERVICE,SUB_GP_KILL_SOCKET,&MyKillSocket,sizeof(MyKillSocket));
				m_pITCPNetworkEngine->ShutDownSocket(m_UserSocketID[dwUserID]);

			}

			return true;

		}
	case SUB_CS_S_REGISTER_FAILURE:		//ע��ʧ��
		{
			//��������
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//Ч�����
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//�رմ���
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//��ʾ��Ϣ
			LPCTSTR pszDescribeString=pRegisterFailure->szDescribeString;
			if (lstrlen(pszDescribeString)>0){

				CTraceService::TraceString(pszDescribeString,TraceLevel_Exception);
				CTraceService::TraceString(_T("TSSCVhis ok?"),TraceLevel_Exception);
			}

			//�¼�֪ͨ
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�б��¼�
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//������Ϣ
		{
			//�����б�
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//��������
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);

			//��������
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;
			}

			//Ŀ¼����
			CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
			if (pGameKindItem!=NULL)
			{
				tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
				pGameKindItem->m_GameKind.dwOnLineCount+=pGameServer->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//�������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//�����޸�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//��������
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//���ҷ���
			ASSERT(m_ServerListManager.SearchGameServer(pServerModify->wServerID));
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//���÷���
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//����ɾ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//��������
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//��������
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//�������
		{
			//�����б�
			m_ServerListManager.CleanServerItem();

			//�¼�����
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//Զ�̷���
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SEARCH_CORRESPOND:	//Э������
		{
			//��������
			CMD_CS_S_SearchCorrespond * pSearchCorrespond=(CMD_CS_S_SearchCorrespond *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_CS_S_SearchCorrespond));
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
			ASSERT(wDataSize==(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

			//Ч�����
			if (wDataSize>sizeof(CMD_CS_S_SearchCorrespond)) return false;
			if (wDataSize<(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
			if (wDataSize!=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

			//�ж�����
			ASSERT(LOWORD(pSearchCorrespond->dwSocketID)<m_pInitParameter->m_wMaxConnect);
			if ((m_pBindParameter+LOWORD(pSearchCorrespond->dwSocketID))->dwSocketID!=pSearchCorrespond->dwSocketID) return true;

			//��������
			CMD_GP_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ñ���
			for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
			{
				//����Ч��
				ASSERT(SearchCorrespond.wUserCount<CountArray(SearchCorrespond.UserRemoteInfo));
				if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

				//���ñ���
				WORD wIndex=SearchCorrespond.wUserCount++;
				CopyMemory(&SearchCorrespond.UserRemoteInfo[wIndex],&pSearchCorrespond->UserRemoteInfo[i],sizeof(SearchCorrespond.UserRemoteInfo[wIndex]));
			}

			//��������
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(pSearchCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	}

	return true;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubPCLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubPCRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_LIST:			//��ȡ�б�
		{
			//�����б�
			SendGameTypeInfo(dwSocketID);
			SendGameKindInfo(dwSocketID);
			SendGameServerInfo(dwSocketID);

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

			return true;
		}
	case SUB_GP_GET_ONLINE:			//��ȡ����
		{
			//��������
			CMD_GP_GetOnline * pGetOnline=(CMD_GP_GetOnline *)pData;
			WORD wHeadSize=(sizeof(CMD_GP_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

			//��������
			CMD_GP_KindOnline KindOnline;
			CMD_GP_ServerOnline ServerOnline;
			ZeroMemory(&KindOnline,sizeof(KindOnline));
			ZeroMemory(&ServerOnline,sizeof(ServerOnline));

			//��ȡ����
			/*POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=KindOnline.wKindCount++;
					KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//����ж�
				if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);*/

			//��ȡ����
			for (WORD i=0;i<pGetOnline->wServerCount;i++)
			{
				//��ȡ����
				WORD wServerID=pGetOnline->wOnLineServerID[i];
				CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

				//���ñ���
				if (pGameServerItem!=NULL)
				{
					WORD wServerIndex=ServerOnline.wServerCount++;
					ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount+=pGameServerItem->m_dwVirtualCount;

				}

				//����ж�
				if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
				{
					ASSERT(FALSE);
					break;
				}
			}

			//��������
			if (KindOnline.wKindCount>0)
			{
				WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
				WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_KINE_ONLINE,&KindOnline,wSendSize);
			}

			//��������
			if (ServerOnline.wServerCount>0)
			{
				WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
				WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_SERVER_ONLINE,&ServerOnline,wSendSize);
			}

			return true;
		}
	}

	return false;
}

//������
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_MODIFY_MACHINE:		//�󶨻���
		{

			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyMachine));
			if (wDataSize!=sizeof(CMD_GP_ModifyMachine)) return false;

			//������Ϣ
			CMD_GP_ModifyMachine * pModifyMachine=(CMD_GP_ModifyMachine *)pData;
			pModifyMachine->szPassword[CountArray(pModifyMachine->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyMachine ModifyMachine;
			ZeroMemory(&ModifyMachine,sizeof(ModifyMachine));

			//��������
			ModifyMachine.cbBind=pModifyMachine->cbBind;
			ModifyMachine.dwUserID=pModifyMachine->dwUserID;
			ModifyMachine.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyMachine.szPassword,pModifyMachine->szPassword,CountArray(ModifyMachine.szPassword));
			lstrcpyn(ModifyMachine.szMachineID,pModifyMachine->szMachineID,CountArray(ModifyMachine.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_MACHINE,dwSocketID,&ModifyMachine,sizeof(ModifyMachine));

			return true;
		}
	case SUB_GP_MODIFY_LOGON_PASS:	//�޸�����
		{

			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyLogonPass));
			if (wDataSize!=sizeof(CMD_GP_ModifyLogonPass)) return false;

			//������Ϣ
			CMD_GP_ModifyLogonPass * pModifyLogonPass=(CMD_GP_ModifyLogonPass *)pData;
			pModifyLogonPass->szDesPassword[CountArray(pModifyLogonPass->szDesPassword)-1]=0;
			pModifyLogonPass->szScrPassword[CountArray(pModifyLogonPass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyLogonPass ModifyLogonPass;
			ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

			//��������
			ModifyLogonPass.dwUserID=pModifyLogonPass->dwUserID;
			ModifyLogonPass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyLogonPass.szDesPassword,pModifyLogonPass->szDesPassword,CountArray(ModifyLogonPass.szDesPassword));
			lstrcpyn(ModifyLogonPass.szScrPassword,pModifyLogonPass->szScrPassword,CountArray(ModifyLogonPass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_LOGON_PASS,dwSocketID,&ModifyLogonPass,sizeof(ModifyLogonPass));

			return true;
		}
	case SUB_GP_MODIFY_INSURE_PASS:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyInsurePass));
			if (wDataSize!=sizeof(CMD_GP_ModifyInsurePass)) return false;

			//������Ϣ
			CMD_GP_ModifyInsurePass * pModifyInsurePass=(CMD_GP_ModifyInsurePass *)pData;
			pModifyInsurePass->szDesPassword[CountArray(pModifyInsurePass->szDesPassword)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyInsurePass ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//��������
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesPassword,pModifyInsurePass->szDesPassword,CountArray(ModifyInsurePass.szDesPassword));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INSURE_PASS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_SYSTEM_FACE_INFO:	//�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_SystemFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_SystemFaceInfo)) return false;

			//������Ϣ
			CMD_GP_SystemFaceInfo * pSystemFaceInfo=(CMD_GP_SystemFaceInfo *)pData;

			//��������
			DBR_GP_ModifySystemFace ModifySystemFace;
			ZeroMemory(&ModifySystemFace,sizeof(ModifySystemFace));

			//��������
			ModifySystemFace.wFaceID=pSystemFaceInfo->wFaceID;
			ModifySystemFace.dwUserID=pSystemFaceInfo->dwUserID;
			ModifySystemFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifySystemFace.szPassword,pSystemFaceInfo->szPassword,CountArray(ModifySystemFace.szPassword));
			lstrcpyn(ModifySystemFace.szMachineID,pSystemFaceInfo->szMachineID,CountArray(ModifySystemFace.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_SYSTEM_FACE,dwSocketID,&ModifySystemFace,sizeof(ModifySystemFace));

			return true;
		}
	case SUB_GP_QUERY_INDIVIDUAL:	//��ѯ��Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryIndividual));
			if (wDataSize!=sizeof(CMD_GP_QueryIndividual)) return false;

			//������Ϣ
			CMD_GP_QueryIndividual * pQueryIndividual=(CMD_GP_QueryIndividual *)pData;

			//��������
			DBR_GP_QueryIndividual QueryIndividual;
			ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

			//��������
			QueryIndividual.dwUserID=pQueryIndividual->dwUserID;
			QueryIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INDIVIDUAL,dwSocketID,&QueryIndividual,sizeof(QueryIndividual));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_ModifyIndividual));
			if (wDataSize<sizeof(CMD_GP_ModifyIndividual)) return false;

			//������Ϣ
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyIndividual ModifyIndividual;
			ZeroMemory(&ModifyIndividual,sizeof(ModifyIndividual));

			//���ñ���
			ModifyIndividual.dwUserID=pModifyIndividual->dwUserID;
			ModifyIndividual.cbGender=pModifyIndividual->cbGender;
			ModifyIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyIndividual.szPassword,pModifyIndividual->szPassword,CountArray(ModifyIndividual.szPassword));

			//��������
			VOID * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pModifyIndividual+1,wDataSize-sizeof(CMD_GP_ModifyIndividual));

			//��չ��Ϣ
			while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_GP_UI_NICKNAME:		//�û��ǳ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName))
						{
							CopyMemory(&ModifyIndividual.szNickName,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szNickName[CountArray(ModifyIndividual.szNickName)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_USER_NOTE:		//�û���ע
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote))
						{
							CopyMemory(&ModifyIndividual.szUserNote,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUserNote[CountArray(ModifyIndividual.szUserNote)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_COMPELLATION:	//��ʵ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation))
						{
							CopyMemory(&ModifyIndividual.szCompellation,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szCompellation[CountArray(ModifyIndividual.szCompellation)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_SEAT_PHONE:		//�̶��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone))
						{
							CopyMemory(ModifyIndividual.szSeatPhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szSeatPhone[CountArray(ModifyIndividual.szSeatPhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_MOBILE_PHONE:	//�ƶ��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone))
						{
							CopyMemory(ModifyIndividual.szMobilePhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szMobilePhone[CountArray(ModifyIndividual.szMobilePhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_QQ:				//Q Q ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ))
						{
							CopyMemory(ModifyIndividual.szQQ,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szQQ[CountArray(ModifyIndividual.szQQ)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_EMAIL:			//�����ʼ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail))
						{
							CopyMemory(ModifyIndividual.szEMail,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szEMail[CountArray(ModifyIndividual.szEMail)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_DWELLING_PLACE:	//��ϵ��ַ
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace))
						{
							CopyMemory(ModifyIndividual.szDwellingPlace,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szDwellingPlace[CountArray(ModifyIndividual.szDwellingPlace)-1]=0;
						}
						break;
					}
				}
			}

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL,dwSocketID,&ModifyIndividual,sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_USER_SAVE_SCORE:	//������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserSaveScore));
			if (wDataSize!=sizeof(CMD_GP_UserSaveScore)) return false;

			//������Ϣ
			CMD_GP_UserSaveScore * pUserSaveScore=(CMD_GP_UserSaveScore *)pData;
			pUserSaveScore->szMachineID[CountArray(pUserSaveScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserSaveScore UserSaveScore;
			ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

			//��������
			UserSaveScore.dwUserID=pUserSaveScore->dwUserID;
			UserSaveScore.lSaveScore=pUserSaveScore->lSaveScore;
			UserSaveScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserSaveScore.szMachineID,pUserSaveScore->szMachineID,CountArray(UserSaveScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

			return true;
		}
	case SUB_GP_USER_TAKE_SCORE:	//ȡ�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTakeScore));
			if (wDataSize!=sizeof(CMD_GP_UserTakeScore)) return false;

			//������Ϣ
			CMD_GP_UserTakeScore * pUserTakeScore=(CMD_GP_UserTakeScore *)pData;
			pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTakeScore UserTakeScore;
			ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

			//��������
			UserTakeScore.dwUserID=pUserTakeScore->dwUserID;
			UserTakeScore.lTakeScore=pUserTakeScore->lTakeScore;
			UserTakeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTakeScore.szPassword,pUserTakeScore->szPassword,CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID,pUserTakeScore->szMachineID,CountArray(UserTakeScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	case SUB_GP_QUERY_INSURE_INFO:	//��ѯ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryInsureInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryInsureInfo)) return false;

			//������Ϣ
			CMD_GP_QueryInsureInfo * pQueryInsureInfo=(CMD_GP_QueryInsureInfo *)pData;

			//��������
			DBR_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//��������
			QueryInsureInfo.dwUserID=pQueryInsureInfo->dwUserID;
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	case SUB_GR_USER_TRANSFER_SCORE:  //����ת��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTransferScore));
			if (wDataSize!=sizeof(CMD_GP_UserTransferScore)) return false;

            //������Ϣ
			CMD_GP_UserTransferScore * pUserTransferScore=(CMD_GP_UserTransferScore *)pData;
			pUserTransferScore->szAccounts[CountArray(pUserTransferScore->szAccounts)-1]=0;
			pUserTransferScore->szLogonPass[CountArray(pUserTransferScore->szLogonPass)-1]=0;
			pUserTransferScore->szInsurePass[CountArray(pUserTransferScore->szInsurePass)-1]=0;
			pUserTransferScore->szMachineID[CountArray(pUserTransferScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTransferScore UserTransferScore;
			UserTransferScore.dwUserID = pUserTransferScore->dwUserID;
			UserTransferScore.lTransferScore = pUserTransferScore->lTransferScore;
			UserTransferScore.dwClientAddr = (m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			CopyMemory(UserTransferScore.szAccounts,pUserTransferScore->szAccounts,sizeof(UserTransferScore.szAccounts));
			CopyMemory(UserTransferScore.szLogonPass,pUserTransferScore->szLogonPass,sizeof(UserTransferScore.szLogonPass));
			CopyMemory(UserTransferScore.szInsurePass,pUserTransferScore->szInsurePass,sizeof(UserTransferScore.szInsurePass));
			CopyMemory(UserTransferScore.szMachineID,pUserTransferScore->szMachineID,sizeof(UserTransferScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TRANS_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

			return true;
		}
	case SUB_GP_GET_LHC_QIHAO:
		{
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_LHC_QIHAO,dwSocketID,NULL,NULL);
		}
	case SUB_GP_QUERY_TIME:
		{
			SYSTEMTIME	nowsystime;	
			::GetLocalTime(&nowsystime);

			CTime OldNowTime(nowsystime);

			CTime NowTime(nowsystime);
			NowTime += m_timespan;

			CMD_GR_GetSystemTime GetSystemTime1;
			NowTime.GetAsSystemTime(GetSystemTime1.ServerTime);

			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_TIME_RET,&GetSystemTime1, sizeof(GetSystemTime1));
			return true;
		}
	case SUB_GP_QUERY_GAME_RESULT:
		{
			//Ч�����

			ASSERT(wDataSize==sizeof(CMD_GP_QueryGameResult));
			if (wDataSize!=sizeof(CMD_GP_QueryGameResult)) return false;

			CMD_GP_QueryGameResult* pQueryGameResult = (CMD_GP_QueryGameResult*)pData;
			int nTypeID = pQueryGameResult->wKindID;
			if((GetTickCount() - m_dwLuckyNumTickCount[nTypeID])>=3000)
			{	
				m_dwLuckyNumTickCount[nTypeID] = GetTickCount();
				DBR_GP_QueryGameResult QueryGameResult;
				ZeroMemory(&QueryGameResult,sizeof(QueryGameResult));
				QueryGameResult.wKindID = pQueryGameResult->wKindID;


				m_nLuckNumID = pQueryGameResult->wKindID;
				//Ͷ������
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_RESULT,dwSocketID,&QueryGameResult,sizeof(QueryGameResult));
			}
			else
			{
				int nIndex = 0;
				DBO_GP_QueryLotResult QuseryLotResult[70];

				int nSize = m_AllLuckNum.size();
				for( map<int ,tagLuckNum*>::iterator pos = m_AllLuckNum.begin(); pos!=m_AllLuckNum.end(); ++pos)
				{
					for(int kk=0; kk<5; kk++)
					{
						if(nIndex>=70)
						{	
							m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_GAME_RET,&QuseryLotResult,sizeof(DBO_GP_QueryLotResult)* (nIndex-1));
							ZeroMemory(&QuseryLotResult,sizeof(QuseryLotResult));
							nIndex = 0;
						}
						QuseryLotResult[nIndex].wKindID = pos->first;
						lstrcpyn(QuseryLotResult[nIndex].wPeriod, pos->second->LuckNum[kk].qihao,CountArray(QuseryLotResult[nIndex].wPeriod));
						lstrcpyn(QuseryLotResult[nIndex].szLotNum,pos->second->LuckNum[kk].haoma,CountArray(QuseryLotResult[nIndex].szLotNum));
						lstrcpyn(QuseryLotResult[nIndex].szShijian,pos->second->LuckNum[kk].shijian,CountArray(QuseryLotResult[nIndex].szShijian));
						nIndex++;
					}
				}				
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_GAME_RET,&QuseryLotResult,sizeof(DBO_GP_QueryLotResult)* (nIndex-1));

			}
			return true;
		}
	case SUB_GP_GET_MAP_BONUS:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_GetMyMapBonus));
			if (wDataSize!=sizeof(CMD_GP_GetMyMapBonus)) return false;
			CMD_GP_GetMyMapBonus* pGetMyMapBonus = (CMD_GP_GetMyMapBonus*)pData;


			DBR_GP_GetMyMapBonus GetMyMapBonus;
			ZeroMemory(&GetMyMapBonus,sizeof(GetMyMapBonus));
			GetMyMapBonus.n_t_userid = pGetMyMapBonus->n_t_userid;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_MY_MAP_BONUS,dwSocketID,&GetMyMapBonus,sizeof(GetMyMapBonus));
			return true;
		}
	case SUB_GP_GET_CP_USER:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_GetCpUser));
			if (wDataSize!=sizeof(CMD_GP_GetCpUser)) return false;

			CMD_GP_GetCpUser* pUser = (CMD_GP_GetCpUser*)pData;

			DBR_GP_GetCpUser User;
			ZeroMemory(&User,sizeof(User));
			User.n_t_userid = pUser->n_t_userid;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_CP_USER_INFO,dwSocketID,&User,sizeof(User));
			return true;
		}
	case SUB_GP_GET_USER_FANDIAN:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_GetUserFandian));
			if (wDataSize!=sizeof(CMD_GP_GetUserFandian)) return false;

			CMD_GP_GetUserFandian* pUser = (CMD_GP_GetUserFandian*)pData;


			DBR_GP_GetUserFandian User;
			ZeroMemory(&User,sizeof(User));
			User.n_t_userid = pUser->n_t_userid;
			User.n_t_type_id = pUser->n_t_type_id;
			User.n_t_kind_id = pUser->n_t_kind_id;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_USER_FANDIAN,dwSocketID,&User,sizeof(User));
			return true;
		}
	case SUB_GP_SET_USER_BONUS:
		{

			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_SetUserBonus));
			if (wDataSize!=sizeof(CMD_GP_SetUserBonus)) return false;

			CMD_GP_SetUserBonus* pUser = (CMD_GP_SetUserBonus*)pData;

			DBR_GP_SetUserBonus User;
			ZeroMemory(&User,sizeof(User));
			User.n_t_userid = pUser->n_t_userid;
			User.n_t_type_id = pUser->n_t_type_id;
			User.n_t_kind_id = pUser->n_t_kind_id;
			User.f_t_bonus = pUser->f_t_bonus;
			User.f_bonuspercent = pUser->f_bonuspercent;


			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SET_USER_BONUS,dwSocketID,&User,sizeof(User));
			return true;
		}
	case SUB_GP_GET_CHONGZHI_TISHI:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_GetChonzhiTishi));
			if (wDataSize!=sizeof(CMD_GP_GetChonzhiTishi)) return false;

			CMD_GP_GetChonzhiTishi* pGetChonzhiTishi = (CMD_GP_GetChonzhiTishi*)pData;

			DBR_GP_GetChonzhiTishi GetChonzhiTishi;
			ZeroMemory(&GetChonzhiTishi,sizeof(GetChonzhiTishi));
			GetChonzhiTishi.dwUserID = pGetChonzhiTishi->dwUserID;
			//Ͷ������
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHONGZHI_TISHI,dwSocketID,&GetChonzhiTishi,sizeof(GetChonzhiTishi));

		}
	case SUB_GP_GET_TIXIAN_TISHI: //������ʾ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_GetTixianTishi));
			if (wDataSize!=sizeof(CMD_GP_GetTixianTishi)) return false;

			CMD_GP_GetTixianTishi* pGetTixianTishi = (CMD_GP_GetTixianTishi*)pData;

			DBR_GP_GetTixianTishi GetTixianTishi;
			ZeroMemory(&GetTixianTishi,sizeof(GetTixianTishi));
			GetTixianTishi.dwUserID = pGetTixianTishi->dwUserID;
			//Ͷ������
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TIXIAN_TISHI,dwSocketID,&GetTixianTishi,sizeof(GetTixianTishi));

		}
	case SUB_GP_GET_TOUZHU_TISHI: //Ͷע��ʾ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_GetTouzhuTishi));
			if (wDataSize!=sizeof(CMD_GP_GetTouzhuTishi)) return false;

			CMD_GP_GetTouzhuTishi* pGetTouzhuTishi = (CMD_GP_GetTouzhuTishi*)pData;

			DBR_GP_GetTouzhuTishi GetTouzhuTishi;
			ZeroMemory(&GetTouzhuTishi,sizeof(GetTouzhuTishi));
			GetTouzhuTishi.dwUserID = pGetTouzhuTishi->dwUserID;
			//Ͷ������
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TOUZHU_TISHI,dwSocketID,&GetTouzhuTishi,sizeof(GetTouzhuTishi));

		}
	case SUB_GP_TOUZHU_CQSSC:
		{

			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_TouZhuCQSSC));
			if (wDataSize!=sizeof(CMD_GP_TouZhuCQSSC)) 
				return false;

			CMD_GP_TouZhuCQSSC* pTouzhu = (CMD_GP_TouZhuCQSSC*)pData;

			DBR_GP_TouZhuCQSSC TouzhuSSC;
			ZeroMemory(&TouzhuSSC,sizeof(TouzhuSSC));

			TouzhuSSC.dwUserID = pTouzhu->dwUserID;
			TouzhuSSC.nBeitou = pTouzhu->nBeitou;
			TouzhuSSC.nGameKind = pTouzhu->nGameKind;
			TouzhuSSC.nGameType = pTouzhu->nGameType;
			TouzhuSSC.nMoshi = pTouzhu->nMoshi;
			TouzhuSSC.nZhushu = pTouzhu->nZhushu;
// 			TouzhuSSC.strHaoma.Format(TEXT("%s"), pTouzhu->strHaoma);
// 			TouzhuSSC.strQishu .Format(TEXT("%s"), pTouzhu->strQishu);
			TouzhuSSC.nSign =pTouzhu->nSign;
			TouzhuSSC.bZhuihao = pTouzhu->bZhuihao;
			strcpy(TouzhuSSC.strHaoma, pTouzhu->strHaoma);
///////////////////////////////////////��������//////////////////////////////////////////////////////////////////////////////////////////////////////////
			bool bCheDan = true;
			int nGameKind = pTouzhu->nGameType;
			CString sQihao = ChangeStringToT(pTouzhu->strQishu);
			CString strNowQihao;
			if(nGameKind == CZChongQingSSC)
			{
				CChqSSCRule rule;
				rule.SetTimeSpan(m_timespan);
				strNowQihao = rule.GetNextExpect(-1);
			}
			else if(nGameKind == CZ_TianJinSSC)
			{
				CJxSSCRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZXinJiangSSC)
			{
				CXJSSCRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZ_FENFEN_CAI)
			{
				CFenFenCaiRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZ_HGYDWFC)
			{
				CHgydwfcRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZ_WUFEN_CAI)
			{
				CWuFenCaiRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZGD11Xuan5)
			{
				CGD11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZSD11Xuan5)
			{
				CSD11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZJX11Xuan5)
			{
				CJX11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZHLJ11Xuan5)
			{
				CHLJ11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZ_PK10)
			{
				CBJPK10Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
//			CTraceService::TraceString(strNowQihao,TraceLevel_Exception);

// 			if (!bCheDan)
// 			{
// 				CMD_GR_CancelTouzhuRet CancelTouzhu;
// 				ZeroMemory(&CancelTouzhu,sizeof(CancelTouzhu));
// 
// 				CancelTouzhu.nResult = 2;
// 
// 				return m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GR_CANCEL_TOUZHU_RET,&CancelTouzhu, sizeof(CancelTouzhu));
// 			}
// 

///////////////////////////////////////��������//////////////////////////////////////////////////////////////////////////////////////////////////////////
			memcpy(TouzhuSSC.strQishu ,pTouzhu->strQishu,sizeof(TouzhuSSC.strQishu));
			TouzhuSSC.cbPlatform = pTouzhu->cbPlatform;

			TouzhuSSC.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
		//	m_DataBaseEngineSink[1]->OnTouzhuCQSSC(dwSocketID,&TouzhuSSC,wDataSize);
			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TOUZHU_CQSSC,dwSocketID,&TouzhuSSC,sizeof(TouzhuSSC));
			return true;

		}
	case SUB_GP_KEFU_URL:
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_KEFU_URL,dwSocketID,NULL,0);
			return true;
		}
	case SUB_GP_TOUZHU_CQSSC_DAN:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_TouZhuCQSSC_Dan));
			if (wDataSize!=sizeof(CMD_GP_TouZhuCQSSC_Dan))
			{
				return false;
			}

			CMD_GP_TouZhuCQSSC_Dan* pTouzhu = (CMD_GP_TouZhuCQSSC_Dan*)pData;

			DBR_GP_TouZhuCQSSC_Dan TouzhuSSC;
			ZeroMemory(&TouzhuSSC,sizeof(TouzhuSSC));

			TouzhuSSC.dwUserID = pTouzhu->dwUserID;
			TouzhuSSC.nBeitou = pTouzhu->nBeitou;
			TouzhuSSC.nGameKind = pTouzhu->nGameKind;
			TouzhuSSC.nGameType = pTouzhu->nGameType;
			TouzhuSSC.nMoshi = pTouzhu->nMoshi;
			TouzhuSSC.nZhushu = pTouzhu->nZhushu;
			TouzhuSSC.nHaoMaLen = pTouzhu->nHaoMaLen;
			// 			TouzhuSSC.strHaoma.Format(TEXT("%s"), pTouzhu->strHaoma);
			// 			TouzhuSSC.strQishu .Format(TEXT("%s"), pTouzhu->strQishu);
			TouzhuSSC.nSign =pTouzhu->nSign;
			TouzhuSSC.bZhuihao = pTouzhu->bZhuihao;
			//memcpy(TouzhuSSC.strHaoma, pTouzhu->strHaoma,sizeof(TouzhuSSC.strHaoma));
			memset(TouzhuSSC.strHaoma, 0, sizeof(TouzhuSSC.strHaoma));
			memcpy(TouzhuSSC.strHaoma,	pTouzhu->strHaoma,pTouzhu->nHaoMaLen);
			///////////////////////////////////////��������//////////////////////////////////////////////////////////////////////////////////////////////////////////
			bool bCheDan = true;
			int nGameKind = pTouzhu->nGameType;
			CString sQihao = ChangeStringToT(pTouzhu->strQishu);
			CString strNowQihao;
			if(nGameKind == CZChongQingSSC)
			{
				CChqSSCRule rule;
				rule.SetTimeSpan(m_timespan);

				long secDiff = rule.GetKjShjDiff(); 
				strNowQihao = rule.GetNextExpect();

				bCheDan = (strNowQihao==sQihao);
				if(!bCheDan)
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO now:%s,client:%s",strNowQihao,sQihao);
					OutputDebugString(strLog);
				}

				if (secDiff <= 43 )
				{
					bCheDan = false;
				}

			}
			else if(nGameKind == CZ_TianJinSSC)
			{
				CJxSSCRule rule;
				rule.SetTimeSpan(m_timespan);
				strNowQihao = rule.GetNextExpect();
				bCheDan = (strNowQihao==sQihao);
				if(!bCheDan)
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO now:%s,client:%s",strNowQihao,sQihao);
					OutputDebugString(strLog);
				}
				long secDiff = rule.GetKjShjDiff(); 

				if (secDiff <= 198 )
				{
					bCheDan = false;
				}
			}
			else if(nGameKind == CZXinJiangSSC)
			{
				CXJSSCRule rule;
				rule.SetTimeSpan(m_timespan);
				strNowQihao = rule.GetNextExpect();
				bCheDan = (strNowQihao==sQihao);
				long secDiff = rule.GetKjShjDiff(); 
				if(!bCheDan)
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO now:%s,client:%s,secDiff:%d",strNowQihao,sQihao,secDiff);
					OutputDebugString(strLog);
				}

				if (secDiff < 58 )
				{
					bCheDan = false;
				}
			}
 			else if(nGameKind == CZ_FENFEN_CAI)
 			{
 				CFenFenCaiRule rule;
 				rule.SetTimeSpan(m_timespan);
				long secDiff = rule.GetKjShjDiff(); 
				CString strLog;
				strLog.Format(L"secDiff:%d,span:%d",secDiff-3,rule.GetQiSpan());
				CTraceService::TraceString(strLog,TraceLevel_Exception);
				if((secDiff)>3)
				{
					if((secDiff)>rule.GetQiSpan())
						strNowQihao = rule.GetNextExpect(-1);
					else
						strNowQihao = rule.GetNextExpect();

					bCheDan = (strNowQihao==sQihao);
				}
				else
				{
					bCheDan = false;
				}


 			}
			else if(nGameKind == CZ_HGYDWFC)
			{
				CHgydwfcRule rule;
				rule.SetTimeSpan(m_timespan);
				long secDiff = rule.GetKjShjDiff(); 
				if((secDiff)>3)
				{
					if((secDiff)>rule.GetQiSpan())
						strNowQihao = rule.GetNextExpect(-1);
					else
						strNowQihao = rule.GetNextExpect();

					CString strLog;
					strLog.Format(L"secDiff:%d,span:%d,strNowQihao:%s,sQihao:%s",secDiff-3,rule.GetQiSpan(),strNowQihao,sQihao);
					CTraceService::TraceString(strLog,TraceLevel_Exception);
					bCheDan = (strNowQihao==sQihao);
				}
				else
				{
					bCheDan = false;
				}


			}
			else if(nGameKind == CZ_WUFEN_CAI)
			{
				CWuFenCaiRule rule;
				rule.SetTimeSpan(m_timespan);
				strNowQihao = rule.GetNextExpect();
				bCheDan = (strNowQihao==sQihao);
				if(!bCheDan)
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO now:%s,client:%s",strNowQihao,sQihao);
					OutputDebugString(strLog);
				}
				long secDiff = rule.GetKjShjDiff(); 

				if (secDiff <= 43 )
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO secDiff:%d",secDiff);
					OutputDebugString(strLog);
					bCheDan = false;
				}
			}
 			else if(nGameKind == CZGD11Xuan5)
 			{
 				CGD11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				strNowQihao = rule.GetNextExpect();
				bCheDan = (strNowQihao==sQihao);
				long secDiff = rule.GetKjShjDiff(); 
			//	if(!bCheDan)
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO now:%s,client:%s,secDiff:%d",strNowQihao,sQihao,secDiff);
					OutputDebugString(strLog);
				}

				if (secDiff <= 58 )
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO secDiff:%d",secDiff);
					OutputDebugString(strLog);
					bCheDan = false;
				}
 			}
 			else if(nGameKind == CZSD11Xuan5)
 			{
 				CSD11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				strNowQihao = rule.GetNextExpect();
				bCheDan = (strNowQihao==sQihao);
				long secDiff = rule.GetKjShjDiff(); 
				//	if(!bCheDan)
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO now:%s,client:%s,secDiff:%d",strNowQihao,sQihao,secDiff);
					OutputDebugString(strLog);
				}

				if (secDiff < 58 )
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO secDiff:%d",secDiff);
					OutputDebugString(strLog);
					bCheDan = false;
				}
 			}
 			else if(nGameKind == CZJX11Xuan5)
 			{
 				CJX11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				strNowQihao = rule.GetNextExpect();
				bCheDan = (strNowQihao==sQihao);
				long secDiff = rule.GetKjShjDiff(); 
				//	if(!bCheDan)
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO now:%s,client:%s,secDiff:%d",strNowQihao,sQihao,secDiff);
					OutputDebugString(strLog);
				}

				if (secDiff < 58 )
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO secDiff:%d",secDiff);
					OutputDebugString(strLog);
					bCheDan = false;
				}
 			}
 			else if(nGameKind == CZHLJ11Xuan5)
 			{
 				CHLJ11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				strNowQihao = rule.GetNextExpect();
				bCheDan = (strNowQihao==sQihao);
				long secDiff = rule.GetKjShjDiff(); 
				//	if(!bCheDan)
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO now:%s,client:%s,secDiff:%d",strNowQihao,sQihao,secDiff);
					OutputDebugString(strLog);
				}

				if (secDiff < 58 )
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO secDiff:%d",secDiff);
					OutputDebugString(strLog);
					bCheDan = false;
				}
 			}
 			else if(nGameKind == CZ_PK10)
 			{
 				CBJPK10Rule rule;
				rule.SetTimeSpan(m_timespan);
				strNowQihao = rule.GetNextExpect();
				bCheDan = (strNowQihao==sQihao);
				long secDiff = rule.GetKjShjDiff(); 
				//	if(!bCheDan)
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO now:%s,client:%s,secDiff:%d",strNowQihao,sQihao,secDiff);
					OutputDebugString(strLog);
				}

				if (secDiff < 30 )
				{
					CString strLog;
					strLog.Format(L"WRONGQIHAO secDiff:%d",secDiff);
					OutputDebugString(strLog);
					bCheDan = false;
				}
 			}

 			if (!bCheDan)
 			{
				CMD_GR_TouzhuRes TouzhuRes;
				ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
				TouzhuRes.lResult = 2;
				lstrcpyn(TouzhuRes.szDesc,_T(""),sizeof(TouzhuRes.szDesc));
				TouzhuRes.nSign = pTouzhu->nSign;


				return m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_RET,&TouzhuRes,sizeof(TouzhuRes));
 			}
 

			///////////////////////////////////////��������//////////////////////////////////////////////////////////////////////////////////////////////////////////
			memcpy(TouzhuSSC.strQishu ,pTouzhu->strQishu,sizeof(TouzhuSSC.strQishu));
			TouzhuSSC.nEnd = pTouzhu->nEnd;
			TouzhuSSC.nIndex = pTouzhu->nIndex;
			TouzhuSSC.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			m_DataBaseEngineSink[1]->OnTouzhuCQSSCDan(dwSocketID,&TouzhuSSC,sizeof(TouzhuSSC));

			//Ͷ������
		//	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TOUZHU_CQSSC_DAN,dwSocketID,pData,wDataSize);


			return true;

		}
	case SUB_MB_TOUZHU_CQSSC_ZHUIHAO:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
			if (wDataSize!=sizeof(CMD_GP_TouZhuCQSSC_Zhuihao))
			{
				return false;
			}
			CMD_GP_TouZhuCQSSC_Zhuihao* pTouzhu = (CMD_GP_TouZhuCQSSC_Zhuihao*)pData;

			DBR_GP_TouZhuCQSSC_Zhuihao TouzhuSSC;
			ZeroMemory(&TouzhuSSC,sizeof(TouzhuSSC));

			TouzhuSSC.dwUserID = pTouzhu->dwUserID;
			for(int i = 0;i< 50;i++)
			{
				TouzhuSSC.nBeitou[i] = pTouzhu->nBeitou[i];

			}
			TouzhuSSC.nGameKind = pTouzhu->nGameKind;
			TouzhuSSC.nGameType = pTouzhu->nGameType;
			TouzhuSSC.nMoshi = pTouzhu->nMoshi;
			TouzhuSSC.nZhushu = pTouzhu->nZhushu;
			TouzhuSSC.nHaoMaLen = pTouzhu->nHaoMaLen;
			// 			TouzhuSSC.strHaoma.Format(TEXT("%s"), pTouzhu->strHaoma);
			// 			TouzhuSSC.strQishu .Format(TEXT("%s"), pTouzhu->strQishu);
			TouzhuSSC.nSign =pTouzhu->nSign;
			TouzhuSSC.bZhuihao = pTouzhu->bZhuihao;
			//memcpy(TouzhuSSC.strHaoma, pTouzhu->strHaoma,sizeof(TouzhuSSC.strHaoma));
			memset(TouzhuSSC.strHaoma, 0, sizeof(TouzhuSSC.strHaoma));
			memcpy(TouzhuSSC.strHaoma,	pTouzhu->strHaoma,pTouzhu->nHaoMaLen);
			memcpy(TouzhuSSC.strQishu ,pTouzhu->strQishu,sizeof(TouzhuSSC.strQishu));
			TouzhuSSC.nEnd = pTouzhu->nEnd;
			TouzhuSSC.nIndex = pTouzhu->nIndex;
			TouzhuSSC.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;


			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_TOUZHU_CQSSC_ZHUIHAO,dwSocketID,&TouzhuSSC,sizeof(TouzhuSSC));
			return true;
		}
	case SUB_GP_TOUZHU_CQSSC_ZHUIHAO:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
			if (wDataSize!=sizeof(CMD_GP_TouZhuCQSSC_Zhuihao))
			{
				return false;
			}

			CMD_GP_TouZhuCQSSC_Zhuihao* pTouzhu = (CMD_GP_TouZhuCQSSC_Zhuihao*)pData;

			DBR_GP_TouZhuCQSSC_Zhuihao TouzhuSSC;
			ZeroMemory(&TouzhuSSC,sizeof(TouzhuSSC));

			TouzhuSSC.dwUserID = pTouzhu->dwUserID;
			for(int i = 0;i< 50;i++)
			{
				TouzhuSSC.nBeitou[i] = pTouzhu->nBeitou[i];

			}
			TouzhuSSC.nGameKind = pTouzhu->nGameKind;
			TouzhuSSC.nGameType = pTouzhu->nGameType;
			TouzhuSSC.nMoshi = pTouzhu->nMoshi;
			TouzhuSSC.nZhushu = pTouzhu->nZhushu;
			TouzhuSSC.nHaoMaLen = pTouzhu->nHaoMaLen;
			// 			TouzhuSSC.strHaoma.Format(TEXT("%s"), pTouzhu->strHaoma);
			// 			TouzhuSSC.strQishu .Format(TEXT("%s"), pTouzhu->strQishu);
			TouzhuSSC.nSign =pTouzhu->nSign;
			TouzhuSSC.bZhuihao = pTouzhu->bZhuihao;
			//memcpy(TouzhuSSC.strHaoma, pTouzhu->strHaoma,sizeof(TouzhuSSC.strHaoma));
			memset(TouzhuSSC.strHaoma, 0, sizeof(TouzhuSSC.strHaoma));
			memcpy(TouzhuSSC.strHaoma,	pTouzhu->strHaoma,pTouzhu->nHaoMaLen);
			memcpy(TouzhuSSC.strQishu ,pTouzhu->strQishu,sizeof(TouzhuSSC.strQishu));
			TouzhuSSC.nEnd = pTouzhu->nEnd;
			TouzhuSSC.nIndex = pTouzhu->nIndex;
			TouzhuSSC.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;



		//	m_DataBaseEngineSink[1]->OnTouzhuCQSSCZhuihao(dwSocketID,pData,wDataSize);

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TOUZHU_CQSSC_ZHUIHAO,dwSocketID,&TouzhuSSC,sizeof(TouzhuSSC));


			return true;

		}
	case SUB_GP_GET_TOUZHU_LOG_COUNT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetTouzhuLogCount));
			if(wDataSize!= sizeof(CMD_GP_GetTouzhuLogCount)) return false;

			CMD_GP_GetTouzhuLogCount* pLogCount = (CMD_GP_GetTouzhuLogCount*)pData;

			DBR_GP_GetTouzhuLogCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			LogCount.bTime = pLogCount->bTime;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TOUZHU_LOG_COUNT,dwSocketID,&LogCount,sizeof(LogCount));
		}
	case SUB_GP_GET_TIXIAN_LOG_COUNT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetTixianLogCount));
			if(wDataSize!= sizeof(CMD_GP_GetTixianLogCount)) return false;

			CMD_GP_GetTixianLogCount* pLogCount = (CMD_GP_GetTixianLogCount*)pData;

			DBR_GP_GetTixianLogCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			LogCount.bTime = pLogCount->bTime;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TIXIAN_LOG_COUNT,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_CHONGZHI_LOG_COUNT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetChongzhiLogCount));
			if(wDataSize!= sizeof(CMD_GP_GetChongzhiLogCount)) return false;

			CMD_GP_GetChongzhiLogCount* pLogCount = (CMD_GP_GetChongzhiLogCount*)pData;

			DBR_GP_GetChongzhiLogCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			LogCount.bTime = pLogCount->bTime;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHONGZHI_LOG_COUNT,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_HYXX_LOG_COUNT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetHYXXLogCount));
			if(wDataSize!= sizeof(CMD_GP_GetHYXXLogCount)) return false;

			CMD_GP_GetHYXXLogCount* pLogCount = (CMD_GP_GetHYXXLogCount*)pData;

			DBR_GP_GetHYXXLogCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			LogCount.n_t_type = pLogCount->n_t_type;
			LogCount.n_t_user_id = pLogCount->n_t_user_id;
			lstrcpyn(LogCount.s_t_account,pLogCount->s_t_account,sizeof(LogCount.s_t_account));

			LogCount.bTime = pLogCount->bTime;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));

			LogCount.nOnline = pLogCount->nOnline;
			LogCount.nYue = pLogCount->nYue;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_HYXX_LOG_COUNT,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_YINGKUI_LOG_COUNT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetYingkuiLogCount));
			if(wDataSize!= sizeof(CMD_GP_GetYingkuiLogCount)) return false;

			CMD_GP_GetYingkuiLogCount* pLogCount = (CMD_GP_GetYingkuiLogCount*)pData;

			DBR_GP_GetYingkuiLogCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			LogCount.bTime = pLogCount->bTime;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_YINGKUI_LOG_COUNT,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_YINGKUI_MX_COUNT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetYingkuiMxCount));
			if(wDataSize!= sizeof(CMD_GP_GetYingkuiMxCount)) return false;

			CMD_GP_GetYingkuiMxCount* pLogCount = (CMD_GP_GetYingkuiMxCount*)pData;

			DBR_GP_GetYingkuiMxCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_YINGKUI_MX_COUNT,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_QP_YINGKUI_COUNT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetQiPaiYingkuiCount));
			if(wDataSize!= sizeof(CMD_GP_GetQiPaiYingkuiCount)) return false;


			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QP_YINGKUI_COUNT,dwSocketID,pData,wDataSize);

		}
	case SUB_GP_GET_XJTZH_LOG_COUNT:    //�¼�Ͷע��־��
		{

			ASSERT(wDataSize == sizeof(CMD_GP_GetXJTZHLogCount));
			if(wDataSize!= sizeof(CMD_GP_GetXJTZHLogCount)) return false;


			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJTZH_LOG_COUNT,dwSocketID,pData,wDataSize);

		}
	case SUB_GP_CHK_XJTZH_LOG_COUNT:    //�¼�Ͷע��־��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_CHKXJTZHLogCount));
			if(wDataSize!= sizeof(CMD_GP_CHKXJTZHLogCount)) return false;

			CMD_GP_CHKXJTZHLogCount* pLogCount = (CMD_GP_CHKXJTZHLogCount*)pData;

			DBR_GP_CHKXJTZHLogCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;

			LogCount.n_t_type = pLogCount->n_t_type;
			LogCount.n_t_user_id = pLogCount->n_t_user_id;

			lstrcpyn(LogCount.s_t_account,pLogCount ->s_t_account,sizeof(LogCount.s_t_account));
			LogCount.n_xj_id = pLogCount->n_xj_id;
			LogCount.bTime = pLogCount->bTime;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHKXJTZH_LOG_COUNT,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_XJYK_LOG_COUNT:    //�¼�ӯ����־��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYKLogCount));
			if(wDataSize!= sizeof(CMD_GP_GetXJYKLogCount)) return false;

			CMD_GP_GetXJYKLogCount* pLogCount = (CMD_GP_GetXJYKLogCount*)pData;

			DBR_GP_GetXJYKLogCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			LogCount.n_t_type = pLogCount->n_t_type;
			LogCount.n_t_user_id = pLogCount->n_t_user_id;

			lstrcpyn(LogCount.s_t_account,pLogCount->s_t_account,sizeof(LogCount.s_t_account));
			LogCount.bTime = pLogCount->bTime;
			LogCount.nXiaJiaID = pLogCount->nXiaJiID;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));
			LogCount.nType = pLogCount->nType;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJYK_LOG_COUNT,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_XJYK_TJ_COUNT:    //�¼�ӯ����־��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYKTjCount));
			if(wDataSize!= sizeof(CMD_GP_GetXJYKTjCount)) return false;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJYK_TJ_COUNT,dwSocketID,pData,wDataSize);

		}
	case SUB_GP_GET_XJYX_TJ_COUNT:    //�¼���Ϸӯ����־��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYXTjCount));
			if(wDataSize!= sizeof(CMD_GP_GetXJYXTjCount)) return false;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJYX_TJ_COUNT,dwSocketID,pData,wDataSize);

		}
	case SUB_GP_GET_XJCHZH_LOG_COUNT:    //�¼���ֵ��־��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXjChzhLogCount));
			if(wDataSize!= sizeof(CMD_GP_GetXjChzhLogCount)) return false;

			CMD_GP_GetXjChzhLogCount* pLogCount = (CMD_GP_GetXjChzhLogCount*)pData;

			DBR_GP_GetXjChzhLogCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			LogCount.n_t_type = pLogCount->n_t_type;
			LogCount.n_t_user_id = pLogCount->n_t_user_id;

			lstrcpyn(LogCount.s_t_account,pLogCount->s_t_account,sizeof(LogCount.s_t_account));
			LogCount.bTime = pLogCount->bTime;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJCHZH_LOG_COUNT,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_XJTX_LOG_COUNT:    //�¼�������־��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXjTxLogCount));
			if(wDataSize!= sizeof(CMD_GP_GetXjTxLogCount)) return false;

			CMD_GP_GetXjTxLogCount* pLogCount = (CMD_GP_GetXjTxLogCount*)pData;

			DBR_GP_GetXjTxLogCount LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			LogCount.n_t_type = pLogCount->n_t_type;
			LogCount.n_t_user_id = pLogCount->n_t_user_id;

			lstrcpyn(LogCount.s_t_account,pLogCount->s_t_account,sizeof(LogCount.s_t_account));
			LogCount.bTime = pLogCount->bTime;
			lstrcpyn(LogCount.szTimeStart,pLogCount->szTimeStart,sizeof(LogCount.szTimeStart));
			lstrcpyn(LogCount.szTimeEnd,pLogCount->szTimeEnd,sizeof(LogCount.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJTX_LOG_COUNT,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_CHONG_ZHI_TYPE:    //��ȡ��ֵ��Ϣ
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetChongzhiType));
			if(wDataSize!= sizeof(CMD_GP_GetChongzhiType)) return false;

			CMD_GP_GetChongzhiType* pLogCount = (CMD_GP_GetChongzhiType*)pData;

			DBR_GP_GetChongzhiType LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.nUserID=pLogCount->nUserID;
			LogCount.cbType = pLogCount->cbType;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_CHONGZHI_TYPE,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_ALL_YINHANG_NAME:
		{
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_YINHANG,dwSocketID,NULL,NULL);
		}
	case SUB_GP_QU_KUAN_INFO:  //��ȡȡ����Ϣ
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetQukuanInfo));
			if(wDataSize!= sizeof(CMD_GP_GetQukuanInfo)) return false;

			CMD_GP_GetQukuanInfo* pLogCount = (CMD_GP_GetQukuanInfo*)pData;

			DBR_GP_GetQukuanInfo LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_QUKUAN_INFO,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_LAST_YUE:  //��ȡ�����Ϣ
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetLastYue));
			if(wDataSize!= sizeof(CMD_GP_GetLastYue)) return false;

			CMD_GP_GetLastYue* pLogCount = (CMD_GP_GetLastYue*)pData;
			DWORD dwTickCount = GetTickCount();
			if((dwTickCount-m_UserYue[pLogCount->dwUserID]) < 30000)
			{
				CString strLogFile;
				strLogFile.Format(L"����%d SUB_GP_GET_LAST_YUE",pLogCount->dwUserID);
				LogFile::instance().LogText(strLogFile);

				return true;
			}

			m_UserYue[pLogCount->dwUserID] = dwTickCount;

// 			DBR_GP_GetLastYue LogCount;
// 			ZeroMemory(&LogCount,sizeof(LogCount));
// 
// 			LogCount.dwUserID = pLogCount->dwUserID;
			return GetUserYueInfo(pLogCount->dwUserID,dwSocketID);//m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_YUE_INFO,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_DAILI_HUIKUI:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetDailiHuikui));
			if(wDataSize!= sizeof(CMD_GP_GetDailiHuikui)) return false;

			CMD_GP_GetDailiHuikui* pGetDailiHuikui = (CMD_GP_GetDailiHuikui*)pData;

			DBR_GP_GetDailiHuikui GetDailiHuikui;
			ZeroMemory(&GetDailiHuikui,sizeof(GetDailiHuikui));

			GetDailiHuikui.n_t_userid = pGetDailiHuikui->n_t_userid;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_DAILI_HUIKUI,dwSocketID,&GetDailiHuikui,sizeof(GetDailiHuikui));


		}
	case SUB_GP_DAILI_LJ:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_DailiLingjiang));
			if(wDataSize!= sizeof(CMD_GP_DailiLingjiang)) return false;

			CMD_GP_DailiLingjiang* pDailiLingjiang = (CMD_GP_DailiLingjiang*)pData;

			DBR_GP_DailiLingjiang DailiLingjiang;
			ZeroMemory(&DailiLingjiang,sizeof(DailiLingjiang));

			DailiLingjiang.n_t_userid = pDailiLingjiang->n_t_userid;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_DAILI_LJ,dwSocketID,&DailiLingjiang,sizeof(DailiLingjiang));
		}
	case SUB_GP_GET_MORE_RECORD:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetMoreRecord));
			if(wDataSize!= sizeof(CMD_GP_GetMoreRecord)) return false;

			CMD_GP_GetMoreRecord* pLog = (CMD_GP_GetMoreRecord*)pData;

			DBR_GP_GetMoreRecord LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.n_t_type = pLog->n_t_type;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_MORE_RECORD,dwSocketID,&LogCount,sizeof(LogCount));


		}
	case SUB_GP_QUIT_GAME:  //�˳���Ϸ
		{
			ASSERT(wDataSize == sizeof(CMD_GP_QuitGame));
			if(wDataSize!= sizeof(CMD_GP_QuitGame)) return false;

			CMD_GP_QuitGame* pLogCount = (CMD_GP_QuitGame*)pData;

			DBR_GP_QuitGame LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

// 			if(mapUserBonusInfo[pLogCount->dwUserID]!=NULL)
// 			{
// 				delete mapUserBonusInfo[pLogCount->dwUserID];
// 			}
			
			LogCount.dwUserID = pLogCount->dwUserID;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUIT_GAME,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_USER_QIANDAO:  //ǩ��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_UserQiandao));
			if(wDataSize!= sizeof(CMD_GP_UserQiandao)) return false;

			CMD_GP_UserQiandao* pLogCount = (CMD_GP_UserQiandao*)pData;

			DBR_GP_UserQiandao LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_QIANDAO,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_USER_HUANLESONG:  //������
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetUserHuanlesong));
			if(wDataSize!= sizeof(CMD_GP_GetUserHuanlesong)) return false;

			CMD_GP_GetUserHuanlesong* pLogCount = (CMD_GP_GetUserHuanlesong*)pData;

			DBR_GP_GetUserHuanlesong LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_HUANLE_SONG,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_LING_JIANG:  //�콱
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetLingJiang));
			if(wDataSize!= sizeof(CMD_GP_GetLingJiang)) return false;

			CMD_GP_GetLingJiang* pLogCount = (CMD_GP_GetLingJiang*)pData;

			DBR_GP_GetLingJiang LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_LING_JIANG,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_DO_QU_KUAN:  //ȡ����Ϣ
		{
			ASSERT(wDataSize == sizeof(CMD_GP_DoQukuan));
			if(wDataSize!= sizeof(CMD_GP_DoQukuan)) return false;

			CMD_GP_DoQukuan* pLogCount = (CMD_GP_DoQukuan*)pData;

			DBR_GP_DoQukuan LogCount;
			ZeroMemory(&LogCount,sizeof(LogCount));

			LogCount.dwUserID = pLogCount->dwUserID;
			LogCount.fJine = pLogCount->fJine;
			lstrcpyn(LogCount.szQukuanPass,pLogCount->szQukuanPass,sizeof(LogCount.szQukuanPass));
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_DO_QUKUAN,dwSocketID,&LogCount,sizeof(LogCount));

		}
	case SUB_GP_GET_TOUZHU_LOG:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetTouzhuLog));
			if(wDataSize!= sizeof(CMD_GP_GetTouzhuLog)) return false;

			CMD_GP_GetTouzhuLog* pLog = (CMD_GP_GetTouzhuLog*)pData;

			DBR_GP_GetTouzhuLog TouzhuLog;
			TouzhuLog.dwUserID = pLog->dwUserID;
			TouzhuLog.nPage = pLog->nPage;
			TouzhuLog.nSize = pLog->nSize;
			TouzhuLog.bByTime = pLog->bByTime;
			lstrcpyn(TouzhuLog.szTimeStart,pLog->szTimeStart,sizeof(TouzhuLog.szTimeStart));
			lstrcpyn(TouzhuLog.szTimeEnd,pLog->szTimeEnd,sizeof(TouzhuLog.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TOUZHU_LOG,dwSocketID,&TouzhuLog,sizeof(TouzhuLog));
		}
	case SUB_GP_GET_XJCHZH_LOG:    //�¼���ֵ��־
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJCHZHLog));
			if(wDataSize!= sizeof(CMD_GP_GetXJCHZHLog)) return false;

			CMD_GP_GetXJCHZHLog* pLog = (CMD_GP_GetXJCHZHLog*)pData;

			DBR_GP_GetXJCHZHLog Log;
			Log.dwUserID = pLog->dwUserID;
			Log.n_t_type = pLog->n_t_type;
			Log.n_t_user_id = pLog->n_t_user_id;

			lstrcpyn(Log.s_t_account,pLog->s_t_account,sizeof(Log.s_t_account));
			Log.nPage = pLog->nPage;
			Log.nSize = pLog->nSize;
			Log.bByTime = pLog->bByTime;
			lstrcpyn(Log.szTimeStart,pLog->szTimeStart,sizeof(Log.szTimeStart));
			lstrcpyn(Log.szTimeEnd,pLog->szTimeEnd,sizeof(Log.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_XJCHZH_LOG,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJTX_LOG:    //�¼�������־
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJTxLog));
			if(wDataSize!= sizeof(CMD_GP_GetXJTxLog)) return false;

			CMD_GP_GetXJTxLog* pLog = (CMD_GP_GetXJTxLog*)pData;

			DBR_GP_GetXJTxLog Log;
			Log.dwUserID = pLog->dwUserID;
			Log.n_t_type = pLog->n_t_type;
			Log.n_t_user_id = pLog->n_t_user_id;

			lstrcpyn(Log.s_t_account,pLog->s_t_account,sizeof(Log.s_t_account));
			Log.nPage = pLog->nPage;
			Log.nSize = pLog->nSize;
			Log.bByTime = pLog->bByTime;
			lstrcpyn(Log.szTimeStart,pLog->szTimeStart,sizeof(Log.szTimeStart));
			lstrcpyn(Log.szTimeEnd,pLog->szTimeEnd,sizeof(Log.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_XJTX_LOG,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJYK_LOG:    //��ȡ�¼�ӯ����־
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYKLog));
			if(wDataSize!= sizeof(CMD_GP_GetXJYKLog)) return false;

			CMD_GP_GetXJYKLog* pLog = (CMD_GP_GetXJYKLog*)pData;

			DBR_GP_GetXJYKLog GetXJYKLog;
			GetXJYKLog.dwUserID = pLog->dwUserID;
			GetXJYKLog.n_t_type = pLog->n_t_type;
			GetXJYKLog.n_t_user_id = pLog->n_t_user_id;

			lstrcpyn(GetXJYKLog.s_t_account,pLog->s_t_account,sizeof(GetXJYKLog.s_t_account));
			GetXJYKLog.n_sort_type = pLog->n_sort_type;
			GetXJYKLog.nPage = pLog->nPage;
			GetXJYKLog.nSize = pLog->nSize;
			GetXJYKLog.bByTime = pLog->bByTime;
			lstrcpyn(GetXJYKLog.szTimeStart,pLog->szTimeStart,sizeof(GetXJYKLog.szTimeStart));
			lstrcpyn(GetXJYKLog.szTimeEnd,pLog->szTimeEnd,sizeof(GetXJYKLog.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_XJYK_LOG,dwSocketID,&GetXJYKLog,sizeof(GetXJYKLog));
		}
	case SUB_GP_GET_XJYK_TJ:    //��ȡ�¼�ӯ����־
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYKTj));
			if(wDataSize!= sizeof(CMD_GP_GetXJYKTj)) return false;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_XJYK_TJ,dwSocketID,pData,wDataSize);
		}
	case SUB_GP_GET_XJYX_TJ:    //��ȡ�¼���Ϸӯ����־
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYXTj));
			if(wDataSize!= sizeof(CMD_GP_GetXJYXTj)) return false;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_XJYX_TJ,dwSocketID,pData,wDataSize);
		}
	case SUB_GP_GET_DA_LI_BAO:    //��ȡ�����
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetDaLiBao));
			if(wDataSize!= sizeof(CMD_GP_GetDaLiBao)) return false;

			CMD_GP_GetDaLiBao* pLog = (CMD_GP_GetDaLiBao*)pData;

			DBR_GP_GetDaLiBao GetDLiBao;
			GetDLiBao.dwUserID = pLog->dwUserID;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_DA_LI_BAO,dwSocketID,&GetDLiBao,sizeof(GetDLiBao));
		}
	case SUB_GP_GET_ZA_JIN_DAN:    //��ȡ�ҽ�
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetZaJinDan));
			if(wDataSize!= sizeof(CMD_GP_GetZaJinDan)) return false;

			CMD_GP_GetZaJinDan* pLog = (CMD_GP_GetZaJinDan*)pData;

			DBR_GP_GetZaJinDan GetZaJinDan;
			GetZaJinDan.dwUserID = pLog->dwUserID;
			GetZaJinDan.nType = pLog->nType;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_ZA_JIN_DAN,dwSocketID,&GetZaJinDan,sizeof(GetZaJinDan));
		}
	case SUB_GP_GET_LUCKY_ZHUAN:    //��ȡ����ת��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetLuckyZhuan));
			if(wDataSize!= sizeof(CMD_GP_GetLuckyZhuan)) return false;

			CMD_GP_GetLuckyZhuan* pLog = (CMD_GP_GetLuckyZhuan*)pData;

			DBR_GP_GetLuckyZhuan GetLuckyZhuan;
			GetLuckyZhuan.dwUserID = pLog->dwUserID;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_LUCKY_ZHUAN,dwSocketID,&GetLuckyZhuan,sizeof(GetLuckyZhuan));
		}
	case SUB_GP_CAN_DA_LI_BAO:    //�жϴ����
		{
			ASSERT(wDataSize == sizeof(CMD_GP_CanDaLiBao));
			if(wDataSize!= sizeof(CMD_GP_CanDaLiBao)) return false;

			CMD_GP_CanDaLiBao* pLog = (CMD_GP_CanDaLiBao*)pData;

			DBR_GP_CanDaLiBao GetDLiBao;
			GetDLiBao.dwUserID = pLog->dwUserID;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CAN_DA_LI_BAO,dwSocketID,&GetDLiBao,sizeof(GetDLiBao));
		}
	case SUB_GP_CAN_ZA_JIN_DAN:    //�ж��ҽ�
		{
			ASSERT(wDataSize == sizeof(CMD_GP_CanZaJinDan));
			if(wDataSize!= sizeof(CMD_GP_CanZaJinDan)) return false;

			CMD_GP_CanZaJinDan* pLog = (CMD_GP_CanZaJinDan*)pData;

			DBR_GP_CanZaJinDan GetZaJinDan;
			GetZaJinDan.dwUserID = pLog->dwUserID;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CAN_ZA_JIN_DAN,dwSocketID,&GetZaJinDan,sizeof(GetZaJinDan));
		}
	case SUB_GP_CAN_LUCKY_ZHUAN:    //�ж�����ת��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_CanLuckyZhuan));
			if(wDataSize!= sizeof(CMD_GP_CanLuckyZhuan)) return false;

			CMD_GP_CanLuckyZhuan* pLog = (CMD_GP_CanLuckyZhuan*)pData;

			DBR_GP_CanLuckyZhuan GetCanLuckyZhuan;
			GetCanLuckyZhuan.dwUserID = pLog->dwUserID;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CAN_LUCKY_ZHUAN,dwSocketID,&GetCanLuckyZhuan,sizeof(GetCanLuckyZhuan));
		}
	case SUB_GP_GET_XJTZH_LOG:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJTZHLog));
			if(wDataSize!= sizeof(CMD_GP_GetXJTZHLog)) return false;


			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJTZH_LOG,dwSocketID,pData,wDataSize);
		}
	case SUB_GP_CHK_XJTZH_LOG:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_CHKXJTZHLog));
			if(wDataSize!= sizeof(CMD_GP_CHKXJTZHLog)) return false;

			CMD_GP_CHKXJTZHLog* pLog = (CMD_GP_CHKXJTZHLog*)pData;

			DBR_GP_CHKXJTZHLog Log;
			Log.dwUserID = pLog->dwUserID;
			Log.n_t_type = pLog->n_t_type;
			Log.n_t_user_id = pLog->n_t_user_id;

			lstrcpyn(Log.s_t_account,pLog->s_t_account,sizeof(Log.s_t_account));
			Log.nPage = pLog->nPage;
			Log.nSize = pLog->nSize;
			Log.bByTime = pLog->bByTime;
			lstrcpyn(Log.szTimeStart,pLog->szTimeStart,sizeof(Log.szTimeStart));
			lstrcpyn(Log.szTimeEnd,pLog->szTimeEnd,sizeof(Log.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHKXJTZH_LOG,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJTZH_LOG_BY_ID:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJTZHLogByID));
			if(wDataSize!= sizeof(CMD_GP_GetXJTZHLogByID)) return false;

			CMD_GP_GetXJTZHLogByID* pLog = (CMD_GP_GetXJTZHLogByID*)pData;

			DBR_GP_GetXJTZHLogByID Log;
			Log.dwUserID = pLog->dwUserID;
			Log.nXiaJiID = pLog->nXiaJiID;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJTZH_LOG_BY_ID,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJTX_LOG_BY_ID:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJTxLogByID));
			if(wDataSize!= sizeof(CMD_GP_GetXJTxLogByID)) return false;

			CMD_GP_GetXJTxLogByID* pLog = (CMD_GP_GetXJTxLogByID*)pData;

			DBR_GP_GetXJTxLogByID Log;
			Log.dwUserID = pLog->dwUserID;
			Log.nXiaJiID = pLog->nXiaJiID;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJTX_LOG_BY_ID,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJTX_LOG_BY_ACT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJTxLogByAct));
			if(wDataSize!= sizeof(CMD_GP_GetXJTxLogByAct)) return false;

			CMD_GP_GetXJTxLogByAct* pLog = (CMD_GP_GetXJTxLogByAct*)pData;

			DBR_GP_GetXJTxLogByAct Log;
			Log.dwUserID = pLog->dwUserID;
			lstrcpyn(Log.szAccount,pLog->szAccount,sizeof(Log.szAccount));
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJTX_LOG_BY_ACT,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJYK_LOG_BY_ID:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYKLogByID));
			if(wDataSize!= sizeof(CMD_GP_GetXJYKLogByID)) return false;

			CMD_GP_GetXJYKLogByID* pLog = (CMD_GP_GetXJYKLogByID*)pData;

			DBR_GP_GetXJYKLogByID Log;
			Log.dwUserID = pLog->dwUserID;
			Log.nXiaJiID = pLog->nXiaJiID;
			Log.nSize = pLog->nSize;
			Log.nPage = pLog->nPage;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJYK_LOG_BY_ID,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJTZH_LOG_BY_ACT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJTZHLogByAct));
			if(wDataSize!= sizeof(CMD_GP_GetXJTZHLogByAct)) return false;

			CMD_GP_GetXJTZHLogByAct* pLog = (CMD_GP_GetXJTZHLogByAct*)pData;

			DBR_GP_GetXJTZHLogByAct Log;
			Log.dwUserID = pLog->dwUserID;
			lstrcpyn(Log.szAccount,pLog->szAccount,sizeof(Log.szAccount));
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJTZH_LOG_BY_ACT,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJCHZH_LOG_BY_ID:    //ͨ��ID��ѯ�¼���ֵ��־
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJCHZHLogByID));
			if(wDataSize!= sizeof(CMD_GP_GetXJCHZHLogByID)) return false;

			CMD_GP_GetXJCHZHLogByID* pLog = (CMD_GP_GetXJCHZHLogByID*)pData;

			DBR_GP_GetXJCHZHLogByID Log;
			Log.dwUserID = pLog->dwUserID;
			Log.nXiaJiID = pLog->nXiaJiID;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJCHZH_LOG_BY_ID,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJCHZH_LOG_BY_ACT:	//ͨ���˺Ų�ѯ�¼���ֵ��־
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJCHZHLogByAct));
			if(wDataSize!= sizeof(CMD_GP_GetXJCHZHLogByAct)) return false;

			CMD_GP_GetXJCHZHLogByAct* pLog = (CMD_GP_GetXJCHZHLogByAct*)pData;

			DBR_GP_GetXJCHZHLogByAct Log;
			Log.dwUserID = pLog->dwUserID;
			lstrcpyn(Log.szAccount,pLog->szAccount,sizeof(Log.szAccount));
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJCHZH_LOG_BY_ACT,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJYK_TJ_BY_ID:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYKTjByID));
			if(wDataSize!= sizeof(CMD_GP_GetXJYKTjByID)) return false;

			CMD_GP_GetXJYKTjByID* pLog = (CMD_GP_GetXJYKTjByID*)pData;

			DBR_GP_GetXJYKTjByID Log;
			Log.dwUserID = pLog->dwUserID;
			Log.nXiaJiID = pLog->nXiaJiID;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJYK_TJ_BY_ID,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJYK_TJ_BY_ACT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYKTjByAct));
			if(wDataSize!= sizeof(CMD_GP_GetXJYKTjByAct)) return false;

			CMD_GP_GetXJYKTjByAct* pLog = (CMD_GP_GetXJYKTjByAct*)pData;

			DBR_GP_GetXJYKTjByAct Log;
			Log.dwUserID = pLog->dwUserID;
			lstrcpyn(Log.szAccount,pLog->szAccount,sizeof(Log.szAccount));
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJYK_TJ_BY_ACT,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJYK_LOG_BY_ACT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetXJYKLogByAct));
			if(wDataSize!= sizeof(CMD_GP_GetXJYKLogByAct)) return false;

			CMD_GP_GetXJYKLogByAct* pLog = (CMD_GP_GetXJYKLogByAct*)pData;

			DBR_GP_GetXJYKLogByAct Log;
			Log.dwUserID = pLog->dwUserID;
			lstrcpyn(Log.szAccount,pLog->szAccount,sizeof(Log.szAccount));
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XJYK_LOG_BY_ACT,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_CHK_XJTZH_LOG_BY_ID:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_CHKXJTZHLogByID));
			if(wDataSize!= sizeof(CMD_GP_CHKXJTZHLogByID)) return false;

			CMD_GP_CHKXJTZHLogByID* pLog = (CMD_GP_CHKXJTZHLogByID*)pData;

			DBR_GP_CHKXJTZHLogByID Log;
			Log.dwUserID = pLog->dwUserID;
			Log.nXiaJiID = pLog->nXiaJiID;
			Log.nPage = pLog->nPage;
			Log.nSize = pLog->nSize;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHKXJTZH_LOG_BY_ID,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_HYSHJ: //��ȡ��Ա����
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetHyShj));
			if(wDataSize!= sizeof(CMD_GP_GetHyShj)) return false;

			CMD_GP_GetHyShj* pLog = (CMD_GP_GetHyShj*)pData;

			DBR_GP_GetHyShj Log;
			Log.dwUserID = pLog->dwUserID;
			Log.cbOnlyToday = pLog->cbOnlyToday;

			lstrcpyn(Log.szTimeStart,pLog->szTimeStart,sizeof(Log.szTimeStart));
			lstrcpyn(Log.szTimeEnd,pLog->szTimeEnd,sizeof(Log.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_HYSHJ,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_CHK_XJTZH_LOG_BY_ACT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_CHKXJTZHLogByAct));
			if(wDataSize!= sizeof(CMD_GP_CHKXJTZHLogByAct)) return false;

			CMD_GP_CHKXJTZHLogByAct* pLog = (CMD_GP_CHKXJTZHLogByAct*)pData;

			DBR_GP_CHKXJTZHLogByAct Log;
			Log.dwUserID = pLog->dwUserID;
			lstrcpyn(Log.szAccount,pLog->szAccount,sizeof(Log.szAccount));
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHKXJTZH_LOG_BY_ACT,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_HYXX_LIST:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetHYXXLog));
			if(wDataSize!= sizeof(CMD_GP_GetHYXXLog)) return false;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_HYXX_LIST,dwSocketID,pData,wDataSize);
		}
	case SUB_GP_GET_HYXX_LOG:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetHYXXLog));
			if(wDataSize!= sizeof(CMD_GP_GetHYXXLog)) return false;

			CMD_GP_GetHYXXLog* pLog = (CMD_GP_GetHYXXLog*)pData;

			DBR_GP_GetHYXXLog Log;
			ZeroMemory(&Log,sizeof(Log));
			Log.dwUserID = pLog->dwUserID;
			Log.n_t_type = pLog->n_t_type;
			Log.n_t_user_id = pLog->n_t_user_id;

			lstrcpyn(Log.s_t_account,pLog->s_t_account,CountArray(Log.s_t_account));
			Log.n_t_sorttype = pLog->n_t_sorttype;
			Log.nPage = pLog->nPage;
			Log.nSize = pLog->nSize;
			Log.bByTime = pLog->bByTime;

			Log.nOnline = pLog->nOnline;
			Log.nYue = pLog->nYue;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_HYXX_LOG,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_HYXX_LOG_BY_ID:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetHYXXLogByID));
			if(wDataSize!= sizeof(CMD_GP_GetHYXXLogByID)) return false;

			CMD_GP_GetHYXXLogByID* pLog = (CMD_GP_GetHYXXLogByID*)pData;

			DBR_GP_GetHYXXLogByID Log;
			Log.dwUserID = pLog->dwUserID;
			Log.nXiaJiID = pLog->nXiaJiID;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_HYXX_LOG_BY_ID,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_HYXX_LOG_BY_ACT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetHYXXLogByAct));
			if(wDataSize!= sizeof(CMD_GP_GetHYXXLogByAct)) return false;

			CMD_GP_GetHYXXLogByAct* pLog = (CMD_GP_GetHYXXLogByAct*)pData;

			DBR_GP_GetHYXXLogByAct Log;
			Log.dwUserID = pLog->dwUserID;
			lstrcpyn(Log.szAccount,pLog->szAccount,sizeof(Log.szAccount));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_HYXX_LOG_BY_ACT,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_XG_HY_FANDIAN:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_XGHYFandian));
			if(wDataSize!= sizeof(CMD_GP_XGHYFandian)) return false;

			CMD_GP_XGHYFandian* pLog = (CMD_GP_XGHYFandian*)pData;

			DBR_GP_XGHYFandian Log;
			Log.dwUserID = pLog->dwUserID;
			Log.n_xiaji_id = pLog->n_xiaji_id;
			Log.f_Fandian = pLog->f_Fandian;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XGHY_FANDIAN,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_SW_DAILI:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_SWDaili));
			if(wDataSize!= sizeof(CMD_GP_SWDaili)) return false;

			CMD_GP_SWDaili* pLog = (CMD_GP_SWDaili*)pData;

			DBR_GP_SWDaili Log;
			Log.dwUserID = pLog->dwUserID;
			Log.nAgentID = pLog->nAgentID;
			Log.nDaili = pLog->nDaili;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SW_DAILI,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_HYXX_ZHUANZHANG:			//��Ա��Ϣת��
		{
			ASSERT(wDataSize == sizeof(CMD_GP_HYXX_ZhuanZhang));
			if(wDataSize!= sizeof(CMD_GP_HYXX_ZhuanZhang)) return false;

			CMD_GP_HYXX_ZhuanZhang* pLog = (CMD_GP_HYXX_ZhuanZhang*)pData;

			DBR_GP_HYXX_ZhuanZhang Log;
			Log.n_t_user_id = pLog->n_t_user_id;
			Log.n_t_target_id = pLog->n_t_target_id;
			Log.f_t_jine = pLog->f_t_jine;
			CopyMemory(Log.s_t_password,pLog->s_t_password,CountArray(Log.s_t_password));
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_HYXX_ZHUANZHANG,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_XJ_Peie:			//��ȡ�¼����
		{
			ASSERT(wDataSize == sizeof(CMD_GetXjPeie));
			if(wDataSize!= sizeof(CMD_GetXjPeie)) return false;

			CMD_GetXjPeie* pLog = (CMD_GetXjPeie*)pData;

			DBR_GetXjPeie Log;
			Log.n_t_userid = pLog->n_t_userid;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_HYXX_GET_XJPEIE,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_SET_XJ_Peie:			//�����¼����
		{
			ASSERT(wDataSize == sizeof(CMD_SetXjPeie));
			if(wDataSize!= sizeof(CMD_SetXjPeie)) return false;

			CMD_SetXjPeie* pLog = (CMD_SetXjPeie*)pData;

			DBR_SetXjPeie Log;
			Log.n_t_userid = pLog->n_t_userid;
			Log.n_t_xj_id = pLog->n_t_xj_id;
			Log.n_t_peie1 = pLog->n_t_peie1;
			Log.n_t_peie2 = pLog->n_t_peie2;
			Log.n_t_peie3 = pLog->n_t_peie3;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_HYXX_SET_XJPEIE,dwSocketID,&Log,sizeof(Log));
		}
	case SUB_GP_GET_TIXIAN_LOG:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetTixianLog));
			if(wDataSize!= sizeof(CMD_GP_GetTixianLog)) return false;

			CMD_GP_GetTixianLog* pLog = (CMD_GP_GetTixianLog*)pData;

			DBR_GP_GetTixianLog TixianLog;
			TixianLog.dwUserID = pLog->dwUserID;
			TixianLog.nPage = pLog->nPage;
			TixianLog.nSize = pLog->nSize;
			TixianLog.bByTime = pLog->bByTime;
			lstrcpyn(TixianLog.szTimeStart,pLog->szTimeStart,sizeof(TixianLog.szTimeStart));
			lstrcpyn(TixianLog.szTimeEnd,pLog->szTimeEnd,sizeof(TixianLog.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TIXIAN_LOG,dwSocketID,&TixianLog,sizeof(TixianLog));
		}
	case SUB_GP_GET_CHONGZHI_LOG:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetChongzhiLog));
			if(wDataSize!= sizeof(CMD_GP_GetChongzhiLog)) return false;

			CMD_GP_GetChongzhiLog* pLog = (CMD_GP_GetChongzhiLog*)pData;

			DBR_GP_GetChongzhiLog ChongzhiLog;
			ChongzhiLog.dwUserID = pLog->dwUserID;
			ChongzhiLog.nPage = pLog->nPage;
			ChongzhiLog.nSize = pLog->nSize;
			ChongzhiLog.bByTime = pLog->bByTime;
			lstrcpyn(ChongzhiLog.szTimeStart,pLog->szTimeStart,sizeof(ChongzhiLog.szTimeStart));
			lstrcpyn(ChongzhiLog.szTimeEnd,pLog->szTimeEnd,sizeof(ChongzhiLog.szTimeEnd));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHONGZHI_LOG,dwSocketID,&ChongzhiLog,sizeof(ChongzhiLog));
		}
	case SUB_GP_GET_YINGKUI_LOG:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetYingkuiLog));
			if(wDataSize!= sizeof(CMD_GP_GetYingkuiLog)) return false;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_YINGKUI_LOG,dwSocketID,pData,wDataSize);
		}
	case SUB_GP_GET_YINGKUI_MX:				//��ѯӯ����ϸ
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetYingkuiMx));
			if(wDataSize!= sizeof(CMD_GP_GetYingkuiMx)) return false;

			CMD_GP_GetYingkuiMx* pLog = (CMD_GP_GetYingkuiMx*)pData;

			DBR_GP_GetYingkuiMx YingkuiMx;
			YingkuiMx.dwUserID = pLog->dwUserID;
			YingkuiMx.bByTime = pLog->bByTime;
			YingkuiMx.nPage = pLog->nPage;
			YingkuiMx.nSize = pLog->nSize;

			lstrcpyn(YingkuiMx.szTimeStart,pLog->szTimeStart,sizeof(YingkuiMx.szTimeStart));
			lstrcpyn(YingkuiMx.szTimeEnd,pLog->szTimeEnd,sizeof(YingkuiMx.szTimeEnd));
			
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_YINGKUI_MX,dwSocketID,&YingkuiMx,sizeof(YingkuiMx));
		}
	case SUB_GP_GET_QP_YINGKUI:				//��ѯӯ����ϸ
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetQiPaiYingkui));
			if(wDataSize!= sizeof(CMD_GP_GetQiPaiYingkui)) return false;

			CMD_GP_GetQiPaiYingkui* pLog = (CMD_GP_GetQiPaiYingkui*)pData;

			DBR_GP_GetQiPaiYingkui YingkuiMx;
			YingkuiMx.dwUserID = pLog->dwUserID;
			YingkuiMx.bByTime = pLog->bByTime;
			YingkuiMx.nPage = pLog->nPage;
			YingkuiMx.nSize = pLog->nSize;

			lstrcpyn(YingkuiMx.szTimeStart,pLog->szTimeStart,sizeof(YingkuiMx.szTimeStart));
			lstrcpyn(YingkuiMx.szTimeEnd,pLog->szTimeEnd,sizeof(YingkuiMx.szTimeEnd));
			
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QP_YINGKUI,dwSocketID,&YingkuiMx,sizeof(YingkuiMx));
		}
	case SUB_GP_GET_REG_URL:
		{
			ASSERT(wDataSize == sizeof(CMD_GetUrlFandian));
			if(wDataSize!= sizeof(CMD_GetUrlFandian)) return false;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_REG_URL,dwSocketID,pData,wDataSize);
		}
	case SUB_GP_GET_Peie:
		{
			ASSERT(wDataSize == sizeof(CMD_GetPeie));
			if(wDataSize!= sizeof(CMD_GetPeie)) return false;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_Peie,dwSocketID,pData,wDataSize);

		}
	case SUB_GP_CANCEL_TOUZHU:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_CancelTouzhu));
			if(wDataSize!= sizeof(CMD_GP_CancelTouzhu)) return false;

			CMD_GP_CancelTouzhu* pCancel = (CMD_GP_CancelTouzhu*)pData;

		//	CTime OldNowTime(nowsystime);
			bool bCheDan = true;
			int nGameKind = pCancel->n_t_kindid;
			CString sQihao = pCancel->s_t_qihao;
			if(nGameKind == CZChongQingSSC)
			{
				CChqSSCRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZ_TianJinSSC)
			{
				CJxSSCRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZXinJiangSSC)
			{
				CXJSSCRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZ_FENFEN_CAI)
			{
				CFenFenCaiRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZ_HGYDWFC)
			{
				CHgydwfcRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZ_WUFEN_CAI)
			{
				CWuFenCaiRule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZGD11Xuan5)
			{
				CGD11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZSD11Xuan5)
			{
				CSD11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZJX11Xuan5)
			{
				CJX11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZHLJ11Xuan5)
			{
				CHLJ11X5Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}
			else if(nGameKind == CZ_PK10)
			{
				CBJPK10Rule rule;
				rule.SetTimeSpan(m_timespan);
				bCheDan = rule.IsCanCancel(sQihao);
			}

			if (!bCheDan)
			{
				CString strLog;
				strLog.Format(L"CGD11X5Rule ����ʧ�� bCheDan��%d",bCheDan);
				OutputDebugString(strLog);

				CMD_GR_CancelTouzhuRet CancelTouzhu;
				ZeroMemory(&CancelTouzhu,sizeof(CancelTouzhu));

				CancelTouzhu.nResult = 2;

				return m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GR_CANCEL_TOUZHU_RET,&CancelTouzhu, sizeof(CancelTouzhu));
			}

			//bool bCan = false;
			bCheDan = true;
			SYSTEMTIME	nowtime;	
			::GetLocalTime(&nowtime);

			CTime NowTime(nowtime);
			NowTime += m_timespan;

			CString strLog;
			strLog.Format(L"CANCEL kindid:%d",pCancel->n_t_kindid);
			OutputDebugString(strLog);
			if(pCancel->n_t_kindid == CZ3D||pCancel->n_t_kindid == CZPaiLie3||pCancel->n_t_kindid ==CZ_QiXingCai )
			{
				strLog.Format(L"CANCEL hour:%d",NowTime.GetHour());
				OutputDebugString(strLog);
				if(NowTime.GetHour() == 20 && (NowTime.GetMinute()>=0 && NowTime.GetMinute()<36))
				{
					if(pCancel->n_t_kindid == CZ_QiXingCai)
					{
						//���
						int weekDay = NowTime.GetDayOfWeek();
						strLog.Format(L"CANCEL weekDay:%d",weekDay);
						OutputDebugString(strLog);
						if(weekDay == 0 || weekDay == 3 || weekDay == 6)
						{
							bCheDan = false;

						}
					}
					else
					{
						bCheDan = false;
					}
				}
				if (!bCheDan)
				{
					CMD_GR_CancelTouzhuRet CancelTouzhu;
					ZeroMemory(&CancelTouzhu,sizeof(CancelTouzhu));

					CancelTouzhu.nResult = 2;

					return m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GR_CANCEL_TOUZHU_RET,&CancelTouzhu, sizeof(CancelTouzhu));
				}

			}

			DBR_GP_CancelTouzhu CancelTouzhu;
			ZeroMemory(&CancelTouzhu,sizeof(CancelTouzhu));
			CancelTouzhu.n_t_id = pCancel->n_t_id;
			CancelTouzhu.n_t_userid = pCancel->n_t_userid;
				
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CANCEL_TOUZHU,dwSocketID,&CancelTouzhu,sizeof(CancelTouzhu));
		}
	case SUB_GP_ADD_HY:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_AddHuiYuan));
			if(wDataSize!= sizeof(CMD_GP_AddHuiYuan)) return false;

			CMD_GP_AddHuiYuan* pAddHuiYuan = (CMD_GP_AddHuiYuan*)pData;
			DBR_GP_AddHuiYuan AddHuiYuan;
			ZeroMemory(&AddHuiYuan,sizeof(AddHuiYuan));
			AddHuiYuan.n_t_daili_id = pAddHuiYuan->n_t_daili_id;
			AddHuiYuan.n_type = pAddHuiYuan->n_type;
			AddHuiYuan.f_t_fandian = pAddHuiYuan->f_t_fandian;
			lstrcpyn(AddHuiYuan.s_t_account,pAddHuiYuan->s_t_account,sizeof(AddHuiYuan.s_t_account));
			lstrcpyn(AddHuiYuan.s_t_login_pass,pAddHuiYuan->s_t_login_pass,sizeof(AddHuiYuan.s_t_login_pass));
			lstrcpyn(AddHuiYuan.s_t_qukuan_pass,pAddHuiYuan->s_t_qukuan_pass,sizeof(AddHuiYuan.s_t_qukuan_pass));
			lstrcpyn(AddHuiYuan.s_t_qq,pAddHuiYuan->s_t_qq,sizeof(AddHuiYuan.s_t_qq));
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ADD_HY,dwSocketID,&AddHuiYuan,sizeof(AddHuiYuan));
		}
	case SUB_GP_SET_WEB_FANDIAN:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_SetWebFandian));
			if(wDataSize!= sizeof(CMD_GP_SetWebFandian)) return false;

			CMD_GP_SetWebFandian* pSetWebFandian = (CMD_GP_SetWebFandian*)pData;
			DBR_GP_SetWebFandian SetWebFandian;
			ZeroMemory(&SetWebFandian,sizeof(SetWebFandian));
			SetWebFandian.n_t_userid = pSetWebFandian->n_t_userid;
			SetWebFandian.f_t_fandian = pSetWebFandian->f_t_fandian;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SET_WEB_FANDIAN,dwSocketID,&SetWebFandian,sizeof(SetWebFandian));
		}
	case SUB_GP_GET_USER_INFO:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetUserInfo));
			if(wDataSize!= sizeof(CMD_GP_GetUserInfo)) return false;

			CMD_GP_GetUserInfo* pUserInfo = (CMD_GP_GetUserInfo*)pData;
			DBR_GP_GetUserInfo UserInfo;
			ZeroMemory(&UserInfo,sizeof(UserInfo));
			UserInfo.dwUserID = pUserInfo->dwUserID;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_USER_INFO,dwSocketID,&UserInfo,sizeof(UserInfo));
		}
	case SUB_GP_GET_NEWS:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetNews));
			if(wDataSize!= sizeof(CMD_GP_GetNews)) return false;

 			CMD_GP_GetNewsRet GetNewsRet[5];
 			ZeroMemory(GetNewsRet,sizeof(GetNewsRet));
 
 			CMD_GP_GetNews* pNews = (CMD_GP_GetNews*)pData;
 			int i = 0;
 			for ( i = 0; i<5;i++)
 			{
 				if(m_mapNews[i].IsEmpty())
 					break;
 				CString strLog;
 				strLog.Format(L"GETNEWS1 SUB_GP_GET_NEWS %s",m_mapNews[i]);
 				OutputDebugString(strLog);
 
 				lstrcpyn(GetNewsRet[i].s_t_news,m_mapNews[i].GetBuffer(),CountArray(GetNewsRet[i].s_t_news));
 
 			}
 			return m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_GET_NEWS_RET,&GetNewsRet,sizeof(CMD_GP_GetNewsRet)*i);

			//return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_NEWS,dwSocketID,pData,wDataSize);

		}
	case SUB_GP_ZHUANHUAN:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_ZhuanHuan));
			if(wDataSize!= sizeof(CMD_GP_ZhuanHuan)) return false;

			CMD_GP_ZhuanHuan* pZhuanhuan = (CMD_GP_ZhuanHuan*)pData;
			DBR_GP_ZhuanHuan Zhuanhuan;
			ZeroMemory(&Zhuanhuan,sizeof(Zhuanhuan));
			Zhuanhuan.n_t_userid = pZhuanhuan->n_t_userid;
			Zhuanhuan.n_t_type = pZhuanhuan->n_t_type;
			Zhuanhuan.f_t_jine = pZhuanhuan->f_t_jine;
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ZHUAN_HUAN,dwSocketID,&Zhuanhuan,sizeof(Zhuanhuan));
		}
	case SUB_GP_XG_QQ:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_XGqq));
			if(wDataSize!= sizeof(CMD_GP_XGqq)) return false;

			CMD_GP_XGqq* pXGqq = (CMD_GP_XGqq*)pData;
			DBR_GP_XGqq XGqq;
			ZeroMemory(&XGqq,sizeof(XGqq));
			XGqq.dwUserID = pXGqq->dwUserID;
			lstrcpyn(XGqq.szQQ,pXGqq->szQQ,sizeof(XGqq.szQQ));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XG_QQ,dwSocketID,&XGqq,sizeof(XGqq));
		}
	case SUB_GP_ALTER_GENDER:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_AlterGender));
			if(wDataSize!= sizeof(CMD_GP_AlterGender)) return false;

			CMD_GP_AlterGender* pXGqq = (CMD_GP_AlterGender*)pData;
			DBR_GP_AlterGender AlterGender;
			ZeroMemory(&AlterGender,sizeof(AlterGender));
			AlterGender.n_t_userid= pXGqq->n_t_userid;
			AlterGender.cbGender = pXGqq->cbGender;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ALTER_GENDER,dwSocketID,&AlterGender,sizeof(AlterGender));
		}
	case SUB_GP_XG_LOGIN_PASS:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_XG_Loginpass));
			if(wDataSize!= sizeof(CMD_GP_XG_Loginpass)) return false;

			CMD_GP_XG_Loginpass* pXGLoginPass = (CMD_GP_XG_Loginpass*)pData;
			DBR_GP_XG_Loginpass XGLoginpass;
			ZeroMemory(&XGLoginpass,sizeof(XGLoginpass));
			XGLoginpass.dwUserID = pXGLoginPass->dwUserID;

			lstrcpyn(XGLoginpass.szOldPass,pXGLoginPass->szOldPass,sizeof(XGLoginpass.szOldPass));
			lstrcpyn(XGLoginpass.szNewPass,pXGLoginPass->szNewPass,sizeof(XGLoginpass.szNewPass));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XG_LOGIN_PASS,dwSocketID,&XGLoginpass,sizeof(XGLoginpass));
		}
	case SUB_GP_XG_QUKUAN_PASS:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_XG_Qukuanpass));
			if(wDataSize!= sizeof(CMD_GP_XG_Qukuanpass)) return false;

			CMD_GP_XG_Qukuanpass* pXGQukuanpass = (CMD_GP_XG_Qukuanpass*)pData;
			DBR_GP_XG_Qukuanpass XGQukuanpass;
			ZeroMemory(&XGQukuanpass,sizeof(XGQukuanpass));
			XGQukuanpass.dwUserID = pXGQukuanpass->dwUserID;

			lstrcpyn(XGQukuanpass.szOldPass,pXGQukuanpass->szOldPass,sizeof(XGQukuanpass.szOldPass));
			lstrcpyn(XGQukuanpass.szNewPass,pXGQukuanpass->szNewPass,sizeof(XGQukuanpass.szNewPass));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_XG_QUKUAN_PASS,dwSocketID,&XGQukuanpass,sizeof(XGQukuanpass));
		}
	case SUB_GP_LOCK_MACHINE:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_LockMachine));
			if(wDataSize!= sizeof(CMD_GP_LockMachine)) return false;

			CMD_GP_LockMachine* pLockMachine = (CMD_GP_LockMachine*)pData;

			DBR_GP_LockMachine LockMachine;
			ZeroMemory(&LockMachine,sizeof(LockMachine));
			LockMachine.dwUserID = pLockMachine->dwUserID;
			LockMachine.cbLock = pLockMachine->cbLock;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOCK_MACHINE,dwSocketID,&LockMachine,sizeof(LockMachine));

		}
	case SUB_GP_SET_QUKUAN_PROTECT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_SetQukuanProtect));
			if(wDataSize!= sizeof(CMD_GP_SetQukuanProtect)) return false;

			CMD_GP_SetQukuanProtect* pSetQukuanProtect = (CMD_GP_SetQukuanProtect*)pData;
			DBR_GP_SetQukuanProtect SetQukuanProtect;
			ZeroMemory(&SetQukuanProtect,sizeof(SetQukuanProtect));
			SetQukuanProtect.dwUserID = pSetQukuanProtect->dwUserID;

			lstrcpyn(SetQukuanProtect.szQukuanPass,pSetQukuanProtect->szQukuanPass,sizeof(SetQukuanProtect.szQukuanPass));
			lstrcpyn(SetQukuanProtect.szQukuanAsk,pSetQukuanProtect->szQukuanAsk,sizeof(SetQukuanProtect.szQukuanAsk));
			lstrcpyn(SetQukuanProtect.szQukuanAnswer,pSetQukuanProtect->szQukuanAnswer,sizeof(SetQukuanProtect.szQukuanAnswer));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SET_QUKUAN_PROTECT,dwSocketID,&SetQukuanProtect,sizeof(SetQukuanProtect));
		}
	case SUB_GP_SET_QUKUAN_ZHANGHAO:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_SetQukuanZhanghao));
			if(wDataSize!= sizeof(CMD_GP_SetQukuanZhanghao)) return false;

			CMD_GP_SetQukuanZhanghao* pSetQukuanHaoma = (CMD_GP_SetQukuanZhanghao*)pData;
			DBR_GP_SetQukuanZhanghao SetQukuanHaoma;
			ZeroMemory(&SetQukuanHaoma,sizeof(SetQukuanHaoma));
			SetQukuanHaoma.dwUserID = pSetQukuanHaoma->dwUserID;

			lstrcpyn(SetQukuanHaoma.szKaihuYinghang,pSetQukuanHaoma->szKaihuYinghang,sizeof(SetQukuanHaoma.szKaihuYinghang));
			lstrcpyn(SetQukuanHaoma.szKaihuRen,pSetQukuanHaoma->szKaihuRen,sizeof(SetQukuanHaoma.szKaihuRen));
			lstrcpyn(SetQukuanHaoma.szYinhangZhanghu,pSetQukuanHaoma->szYinhangZhanghu,sizeof(SetQukuanHaoma.szYinhangZhanghu));
			lstrcpyn(SetQukuanHaoma.szQukuanPass,pSetQukuanHaoma->szQukuanPass,sizeof(SetQukuanHaoma.szQukuanPass));

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SET_QUKUAN_ZHANGHAO,dwSocketID,&SetQukuanHaoma,sizeof(SetQukuanHaoma));
		}
	case SUB_GP_QUERY_MY_YINHANG:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_QueryMyYinHang));
			if(wDataSize!= sizeof(CMD_GP_QueryMyYinHang)) return false;

			CMD_GP_QueryMyYinHang* pQueryMyYinHang = (CMD_GP_QueryMyYinHang*)pData;
			DBO_GP_QueryMyYinHang QueryMyYinHang;
			ZeroMemory(&QueryMyYinHang,sizeof(QueryMyYinHang));
			QueryMyYinHang.dwUserID = pQueryMyYinHang->dwUserID;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_MY_YINHANG,dwSocketID,&QueryMyYinHang,sizeof(QueryMyYinHang));
		}
	case SUB_GP_QUERY_MY_PROTECT:
		{
			ASSERT(wDataSize == sizeof(CMD_GP_QueryMyProTect));
			if(wDataSize!= sizeof(CMD_GP_QueryMyProTect)) return false;

			CMD_GP_QueryMyProTect* pQueryMyProTect = (CMD_GP_QueryMyProTect*)pData;
			DBR_GP_QueryMyProTect QueryMyProTect;
			ZeroMemory(&QueryMyProTect,sizeof(QueryMyProTect));
			QueryMyProTect.dwUserID = pQueryMyProTect->dwUserID;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_MY_PROTECT,dwSocketID,&QueryMyProTect,sizeof(QueryMyProTect));
		}
	case SUB_GP_QUERY_YINHANG:
		{
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_YINHANG,dwSocketID,NULL,NULL);
		}
	case SUB_GP_GET_CP_USERINFO_RET:
		{
			return OnTCPNetworkSubPCGetCpUserInfo(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_GET_TOU_ZHU_XX:			//PC��ȡͶע��Ϣ
		{
			ASSERT(wDataSize == sizeof(CMD_GP_GetTouzhu));
			if(wDataSize!= sizeof(CMD_GP_GetTouzhu)) return false;

			CMD_GP_GetTouzhu* pGetTouzhuXX= (CMD_GP_GetTouzhu*)pData;
			DBR_GP_GetTouzhu GetTouzhu;
			ZeroMemory(&GetTouzhu,sizeof(GetTouzhu));
			GetTouzhu.n_t_id = pGetTouzhuXX->n_t_id;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_TOUZHU_XX,dwSocketID,&GetTouzhu,sizeof(GetTouzhu));
		}
	case SUB_MB_GET_TOU_ZHU_XX:
		{
			ASSERT(wDataSize == sizeof(CMD_MB_GetTouzhu));
			if(wDataSize!= sizeof(CMD_MB_GetTouzhu)) return false;

			CMD_MB_GetTouzhu* pGetTouzhuXX= (CMD_MB_GetTouzhu*)pData;
			DBR_MB_GetTouzhu GetTouzhu;
			ZeroMemory(&GetTouzhu,sizeof(GetTouzhu));
			GetTouzhu.n_t_id = pGetTouzhuXX->n_t_id;

			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_GET_TOUZHU_XX,dwSocketID,&GetTouzhu,sizeof(GetTouzhu));
		}



	}

	return false;
}

//Զ�̴���
bool CAttemperEngineSink::OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_C_SEARCH_CORRESPOND:	//Э������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_GP_C_SearchCorrespond)) return false;

			//������Ϣ
			CMD_GP_C_SearchCorrespond * pSearchCorrespond=(CMD_GP_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//��������
			CMD_CS_C_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ñ���
			SearchCorrespond.dwSocketID=dwSocketID;
			SearchCorrespond.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(SearchCorrespond.szNickName,pSearchCorrespond->szNickName,CountArray(SearchCorrespond.szNickName));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

			return true;
		}
	}

	return false;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubMBLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubMBRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_GET_CP_USERINFO:
		{
			return OnTCPNetworkSubPCGetCpUserInfo(pData,wDataSize,dwSocketID);
		}

	}

	return false;
}
bool CAttemperEngineSink::GetUserYueInfo(int nUserID,DWORD dwSocketID)
{
	DBR_GP_GetLastYue LogCount;
	ZeroMemory(&LogCount,sizeof(LogCount));

	LogCount.dwUserID = nUserID;
	return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_YUE_INFO,dwSocketID,&LogCount,sizeof(LogCount));

}
//�б���
bool CAttemperEngineSink::OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return false;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonAccounts));
	if (wDataSize<sizeof(CMD_GP_LogonAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	CString strAccount;
	strAccount.Format(L"%s",pLogonAccounts->szAccounts);

	DWORD dwTickCount = GetTickCount();
	if(m_UserLogonBlackList[strAccount] > 5)
	{
		//һСʱ����������
		if((dwTickCount - m_UserLogonTickCount[strAccount])>5*1000)
		{
			m_UserLogonBlackList[strAccount] = 0;
			CString strLog;
			strLog.Format(L"[%s] �ѽ��������,����������½",pLogonAccounts->szAccounts);
			LogFile::instance().LogText(strLog);

		}
	}


	if(m_UserLogonBlackList[strAccount] > 5)
	{
		CString strLog;
		strLog.Format(L"[%s] �Ѽ��������,�ر�Socket",pLogonAccounts->szAccounts);
		LogFile::instance().LogText(strLog);
		//��������
		CMD_GP_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));

		//��������
		LogonFailure.lResultCode=0;
		lstrcpyn(LogonFailure.szDescribeString,_T("�������¼����Ƶ����ϵͳ����ʱֹͣ��5���ӵĵ�¼Ȩ��"),CountArray(LogonFailure.szDescribeString));

		//��������
		WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
		bool bSuccess = m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);
		//	if(bSuccess)
		{
			//�ر�����
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
		}

		//m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
		return true;
	}
	if((dwTickCount - m_UserLogonTickCount[strAccount])<3000)
	{
		m_UserLogonTickCount[strAccount] = dwTickCount;
		m_UserLogonBlackList[strAccount] ++;
		CString strLog;
		strLog.Format(L"[%s] ����%d�������ڵ�½,�ر�Socket",pLogonAccounts->szAccounts,m_UserLogonBlackList[strAccount]);
		LogFile::instance().LogText(strLog);
		//��������
		CMD_GP_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));

		//��������
		LogonFailure.lResultCode=0;
		lstrcpyn(LogonFailure.szDescribeString,_T("�������¼����Ƶ����ϵͳ����ʱֹͣ��ı��ε�¼��"),CountArray(LogonFailure.szDescribeString));

		//��������
		WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
		bool bSuccess = m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);
		//	if(bSuccess)
		{
			//�ر�����
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
		}

		return true;
	}
	m_UserLogonTickCount[strAccount] = dwTickCount;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_GP_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));

	CString strLog;
	strLog.Format(L"PC [%s]���ڵ�½���汾��:[%d,%d,%d],��ǰ�������汾[%d,%d,%d],dwSocketID:%d",pLogonAccounts->szAccounts,GetMainVer(pLogonAccounts->dwPlazaVersion),GetSubVer(pLogonAccounts->dwPlazaVersion),GetProductVer(pLogonAccounts->dwPlazaVersion),m_pInitParameter->m_dwMainVer,m_pInitParameter->m_dwSubVer,m_pInitParameter->m_dwProductVer,dwSocketID);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));
	//m_DataBaseEngineSink[0]->OnRequestLogonAccounts(dwSocketID,&LogonAccounts,sizeof(LogonAccounts));
	return true;
}
//��ȡ��Ʊ�û���Ϣ
bool CAttemperEngineSink::OnTCPNetworkSubPCGetCpUserInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_GetCpUserInfo));
	if (wDataSize<sizeof(CMD_GP_GetCpUserInfo)) return false;

	CMD_GP_GetCpUserInfo* pGetCpUserInfo = (CMD_GP_GetCpUserInfo*)pData;
	DBR_GP_GetCpUserInfo GetCpUserInfo;
	ZeroMemory(&GetCpUserInfo,sizeof(GetCpUserInfo));

	GetCpUserInfo.nUserID = pGetCpUserInfo->n_userid;

	//Ͷ������
	return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_CP_USER_INFO,dwSocketID,&GetCpUserInfo,sizeof(GetCpUserInfo));

}
//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_RegisterAccounts));
	if (wDataSize<sizeof(CMD_GP_RegisterAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szPassPortID[CountArray(pRegisterAccounts->szPassPortID)-1]=0;
	pRegisterAccounts->szCompellation[CountArray(pRegisterAccounts->szCompellation)-1]=0;

	//EWIN�������
	pRegisterAccounts->szQQ[CountArray(pRegisterAccounts->szQQ)-1]=0;
	pRegisterAccounts->szEmain[CountArray(pRegisterAccounts->szEmain)-1]=0;
	pRegisterAccounts->szIPhone[CountArray(pRegisterAccounts->szIPhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szPassPortID,pRegisterAccounts->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	lstrcpyn(RegisterAccounts.szCompellation,pRegisterAccounts->szCompellation,CountArray(RegisterAccounts.szCompellation));

	//EWIN�������
	lstrcpyn(RegisterAccounts.szQQ,pRegisterAccounts->szQQ,CountArray(RegisterAccounts.szQQ));
	lstrcpyn(RegisterAccounts.szEmain,pRegisterAccounts->szEmain,CountArray(RegisterAccounts.szEmain));
	lstrcpyn(RegisterAccounts.szIPhone,pRegisterAccounts->szIPhone,CountArray(RegisterAccounts.szIPhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	if (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone,pLogonAccounts->szMobilePhone,CountArray(LogonAccounts.szMobilePhone));

	CString strLog;
	strLog.Format(L"MB [%s]���ڵ�½���汾��:[%d,%d,%d],��ǰ�������汾[%d,%d,%d]",pLogonAccounts->szAccounts,GetMainVer(pLogonAccounts->dwPlazaVersion),GetSubVer(pLogonAccounts->dwPlazaVersion),GetProductVer(pLogonAccounts->dwPlazaVersion),m_pInitParameter->m_dwMainVer,m_pInitParameter->m_dwSubVer,m_pInitParameter->m_dwProductVer);
	CTraceService::TraceString(strLog,TraceLevel_Normal);

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_RegisterAccounts));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_RegisterAccounts * pRegisterAccounts=(CMD_MB_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pRegisterAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}
//��ȡ��Ʊ�û���Ϣ
bool CAttemperEngineSink::OnGetCpUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GR_GetCpUserInfoRet));
	if (wDataSize!=sizeof(DBO_GR_GetCpUserInfoRet)) return false;

	DBO_GR_GetCpUserInfoRet* pGetCPUserInfoRet = (DBO_GR_GetCpUserInfoRet*)pData;

	CMD_GR_GetCpUserInfoRet GetCpUserInfo;
	ZeroMemory(&GetCpUserInfo,sizeof(GetCpUserInfo));

	GetCpUserInfo.n_t_userid = pGetCPUserInfoRet->n_t_userid;
	lstrcpyn(GetCpUserInfo.s_t_account,pGetCPUserInfoRet->s_t_account,sizeof(GetCpUserInfo.s_t_account));
	GetCpUserInfo.n_t_type = pGetCPUserInfoRet->n_t_type;
	GetCpUserInfo.f_t_fandian = pGetCPUserInfoRet->f_t_fandian;
	lstrcpyn(GetCpUserInfo.s_t_qq,pGetCPUserInfoRet->s_t_qq,sizeof(GetCpUserInfo.s_t_qq));
	GetCpUserInfo.n_t_daili_id = pGetCPUserInfoRet->n_t_daili_id;
	GetCpUserInfo.n_t_online = pGetCPUserInfoRet->n_t_online;
	GetCpUserInfo.f_t_yue = pGetCPUserInfoRet->f_t_yue;
	GetCpUserInfo.f_t_dongjie = pGetCPUserInfoRet->f_t_dongjie;
	GetCpUserInfo.n_t_ban = pGetCPUserInfoRet->n_t_ban;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_CP_USER_RET,&GetCpUserInfo,sizeof(GetCpUserInfo));

	return true;
}
//��ȡ��Ʊ�û�BONUS
bool CAttemperEngineSink::OnGetMyUserMapBonus(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GP_GetMyMapBonusRet));
	if (wDataSize!=sizeof(DBO_GP_GetMyMapBonusRet)) return false;

	DBO_GP_GetMyMapBonusRet* pGetMyMapBonusRet = (DBO_GP_GetMyMapBonusRet*)pData;
	int nType = pGetMyMapBonusRet->n_t_type;

	CMD_GR_GetMapBonusRet GetMapBonusRet[500];
	ZeroMemory(GetMapBonusRet,sizeof(GetMapBonusRet));

	CString strLog;

	int nCount = 0;
	int i = 0;

	for(map<int ,tagMapBonus* >::iterator pos = mapMapBonus.begin();pos!=mapMapBonus.end();++pos)
	{
		if(nType == pos->second->MapBonus.n_t_bonustype)
		{
			if(nCount >= 500)
			{
				m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_MAP_BONUS_RET,&GetMapBonusRet, sizeof(CMD_GR_GetMapBonusRet)*nCount);
				ZeroMemory(GetMapBonusRet,sizeof(GetMapBonusRet));
				nCount = 0;
				i=0;
			}
			GetMapBonusRet[i].f_t_bonus = pos->second->MapBonus.f_t_bonus ;
			GetMapBonusRet[i].n_t_kind_id = pos->second->MapBonus.n_t_kind_id ;
			GetMapBonusRet[i].n_t_type_id = pos->second->MapBonus.n_t_type_id  ;
			if(GetMapBonusRet[i].n_t_type_id == 1)
			{
				CString strLog;
				strLog.Format(L"GETMYMAPBONUS11 thebonus[%d]:%.2lf",i,GetMapBonusRet[i].f_t_bonus);
				OutputDebugString(strLog);

			}
			i++;
			nCount++;
		}
	}
	strLog.Format(L"GETMYMAPBONUS nCount:%d",nCount);
	OutputDebugString(strLog);

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_MAP_BONUS_RET,&GetMapBonusRet, sizeof(CMD_GR_GetMapBonusRet)*nCount);


	return true;
}


//��ȡ��Ʊ�û�������Ϣ
bool CAttemperEngineSink::OnGetUserFandianRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GR_GetUserFandianRet)==0);
	if ((wDataSize%sizeof(DBO_GR_GetUserFandianRet))!=0) return false;

// 	int nCount = (wDataSize/sizeof(DBO_GR_GetUserFandianRet));
// 
// 	CMD_GR_GetUserFandianRet GetUserFandianRet[200];
// 	ZeroMemory(GetUserFandianRet,sizeof(CMD_GR_GetUserFandianRet)*200);
// 	for(int i = 0;i < nCount;i++)
// 	{
// 		DBO_GR_GetUserFandianRet* pUserData = ((DBO_GR_GetUserFandianRet*)pData+i);
// 
// 		GetUserFandianRet[i].n_t_kindid = pUserData->n_t_kindid;
// 		GetUserFandianRet[i].f_t_bonus = pUserData->f_t_bonus;
// 		GetUserFandianRet[i].f_t_bonusPercent = pUserData->f_t_bonusPercent;
// 		GetUserFandianRet[i].f_t_fandian = pUserData->f_t_fandian;
// 
// 		if(mapUserBonusInfo.find(pUserData->n_t_userid) == mapUserBonusInfo.end())
// 		{
// 			tagUserBonusInfo* pUserBonusInfo = new tagUserBonusInfo();
// 			ZeroMemory(pUserBonusInfo->UserFandian,sizeof(DBO_GR_GetUserFandianRet)*2000);
// 
// 			pUserBonusInfo->UserFandian[0].n_t_userid = pUserData->n_t_userid;
// 			pUserBonusInfo->UserFandian[0].n_t_typeid = pUserData->n_t_typeid;
// 			pUserBonusInfo->UserFandian[0].n_t_kindid = pUserData->n_t_kindid;
// 			pUserBonusInfo->UserFandian[0].f_t_bonus = pUserData->f_t_bonus;
// 			pUserBonusInfo->UserFandian[0].f_t_bonusPercent = pUserData->f_t_bonusPercent;
// 			pUserBonusInfo->UserFandian[0].f_t_fandian = pUserData->f_t_fandian;
// 
// 			mapUserBonusInfo[pUserData->n_t_userid] = pUserBonusInfo;
// 		}
// 		else
// 		{
//  			int n=0;
//   			for( n=0; n<sizeof(mapUserBonusInfo[pUserData->n_t_userid]->UserFandian)/sizeof(DBO_GR_GetUserFandianRet); n++)
//   			{
//   				int ntypeid = mapUserBonusInfo[pUserData->n_t_userid]->UserFandian[n].n_t_typeid;
//   				if(ntypeid == 0)
//   					break;
//   			}
// 
// 			mapUserBonusInfo[pUserData->n_t_userid]->UserFandian[n].n_t_userid = pUserData->n_t_userid;
// 			mapUserBonusInfo[pUserData->n_t_userid]->UserFandian[n].n_t_typeid = pUserData->n_t_typeid;
// 			mapUserBonusInfo[pUserData->n_t_userid]->UserFandian[n].n_t_kindid = pUserData->n_t_kindid;
// 			mapUserBonusInfo[pUserData->n_t_userid]->UserFandian[n].f_t_bonus = pUserData->f_t_bonus;
// 			mapUserBonusInfo[pUserData->n_t_userid]->UserFandian[n].f_t_bonusPercent = pUserData->f_t_bonusPercent;
// 			mapUserBonusInfo[pUserData->n_t_userid]->UserFandian[n].f_t_fandian = pUserData->f_t_fandian;
// 
// 		}
// 	}

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_USER_FANDIAN_RET,pData,wDataSize);
	return true;
}
//���ò�Ʊ�û�������Ϣ
bool CAttemperEngineSink::OnSetUserBonusRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GR_SetUserBonusRet));
	if (wDataSize!=sizeof(DBO_GR_SetUserBonusRet)) return false;

	DBO_GR_SetUserBonusRet* pSetUserFandianRet = (DBO_GR_SetUserBonusRet*)pData;

	CMD_GR_SetUserBonusRet SetUserFandian;
	ZeroMemory(&SetUserFandian,sizeof(SetUserFandian));

	SetUserFandian.n_t_result = pSetUserFandianRet->n_t_result;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_SET_USER_BONUS_RET,&SetUserFandian,sizeof(SetUserFandian));
	return true;
}
//��¼�ɹ�
bool CAttemperEngineSink::OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{

	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_LogonSuccess * pDBOLogonSuccess=(DBO_GP_LogonSuccess *)pData;
	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbDataBuffer;


	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_GP_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//���ñ���
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_GP_LogonSuccess));

	//��������
	pCMDLogonSuccess->lScore=pDBOLogonSuccess->lScore;
	pCMDLogonSuccess->lInsure=pDBOLogonSuccess->lInsure;
	pCMDLogonSuccess->lUserMedal=pDBOLogonSuccess->lUserMedal;
	pCMDLogonSuccess->wFaceID=pDBOLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBOLogonSuccess->cbGender;
	pCMDLogonSuccess->dwUserID=pDBOLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwExperience=pDBOLogonSuccess->dwExperience;
	pCMDLogonSuccess->cbMoorMachine=pDBOLogonSuccess->cbMoorMachine;
	pCMDLogonSuccess->n_type = pDBOLogonSuccess->n_type;
	pCMDLogonSuccess->f_fandian = pDBOLogonSuccess->f_fandian;
	pCMDLogonSuccess->f_yue = pDBOLogonSuccess->f_yue;
	pCMDLogonSuccess->f_dongjie = pDBOLogonSuccess->f_dongjie;
	lstrcpyn(pCMDLogonSuccess->szAccounts,pDBOLogonSuccess->szAccounts,CountArray(pCMDLogonSuccess->szAccounts));
	lstrcpyn(pCMDLogonSuccess->szNickName,pDBOLogonSuccess->szNickName,CountArray(pCMDLogonSuccess->szNickName));
	lstrcpyn(pCMDLogonSuccess->szAddrDescribe,pDBOLogonSuccess->szAddrDescribe,CountArray(pCMDLogonSuccess->szAddrDescribe));
	WORD wBindIndex=LOWORD(dwContextID);

	//������Ϣ
	pCMDLogonSuccess->cbShowServerStatus=m_bShowServerStatus?1:0;
	//��Ա��Ϣ
	if (pDBOLogonSuccess->cbMemberOrder!=0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo,sizeof(MemberInfo));
		MemberInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate=pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo,sizeof(MemberInfo),DTP_GP_MEMBER_INFO);
	}

	//��¼�ɹ�

	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_LogonSuccess);
	bool bSuccess = m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbDataBuffer,wSendSize);


	//�����б�

	SendGameTypeInfo(dwContextID);
	SendGameKindInfo(dwContextID);
	SendGameServerInfo(dwContextID);
	bSuccess = m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

 	//��¼���
 	CMD_GP_LogonFinish LogonFinish;
 	ZeroMemory(&LogonFinish,sizeof(LogonFinish));
 	LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
 	LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
 	bSuccess = m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));



	if(m_UserSocketID[pCMDLogonSuccess->dwUserID]!=0)
	{
		CMD_GP_KillSocket MyKillSocket;
		ZeroMemory(&MyKillSocket,sizeof(MyKillSocket));
		MyKillSocket.cbResult = 1;
		_sntprintf(MyKillSocket.szMessage,CountArray(MyKillSocket.szMessage),TEXT("�����˺����ڱ𴦵�¼������ǿ�����ߣ�"));
		m_pITCPNetworkEngine->SendData(m_UserSocketID[pCMDLogonSuccess->dwUserID],MDM_GP_USER_SERVICE,SUB_GP_KILL_SOCKET,&MyKillSocket,sizeof(MyKillSocket));
		m_pITCPNetworkEngine->ShutDownSocket(m_UserSocketID[pCMDLogonSuccess->dwUserID]);

	}
	else
	{
		//��������
		CMD_CS_C_CheckUserOnline CheckUserOnline;
		ZeroMemory(&CheckUserOnline,sizeof(CheckUserOnline));

		CheckUserOnline.dwUserID = pCMDLogonSuccess->dwUserID;
		//��������
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_CHECK_USER_ONLINE,&CheckUserOnline,sizeof(CheckUserOnline));

	}

	m_UserSocketID[pCMDLogonSuccess->dwUserID] = dwContextID;
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	CString strLog;
	strLog.Format(L"��½�ɹ���Nick:%s��m_UserSocketID��%d,dwContextID:%d", pCMDLogonSuccess->szNickName,m_UserSocketID[pCMDLogonSuccess->dwUserID],dwContextID);
	CTraceService::TraceString(strLog,TraceLevel_Normal);

	return true;
}


//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_LogonFailure * pLogonFailure=(DBO_GP_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	bool bSuccess = m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);
//	if(bSuccess)
	{
		//�ر�����
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}
	return true;
}

//�û�ͷ��
bool CAttemperEngineSink::OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserFaceInfo UserFaceInfo;
	ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));
	DBO_GP_UserFaceInfo * pUserFaceInfo=(DBO_GP_UserFaceInfo *)pData;

	//���ñ���
	UserFaceInfo.wFaceID=pUserFaceInfo->wFaceID;

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_FACE_INFO,&UserFaceInfo,sizeof(UserFaceInfo));

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserIndividual * pDBOUserIndividual=(DBO_GP_UserIndividual *)pData;
	CMD_GP_UserIndividual * pCMDUserIndividual=(CMD_GP_UserIndividual *)cbDataBuffer;
	CSendPacketHelper SendPacket(cbDataBuffer+sizeof(CMD_GP_UserIndividual),sizeof(cbDataBuffer)-sizeof(CMD_GP_UserIndividual));

	//���ñ���
	ZeroMemory(pCMDUserIndividual,sizeof(CMD_GP_UserIndividual));

	//��������
	pCMDUserIndividual->dwUserID=pDBOUserIndividual->dwUserID;

	//�û���Ϣ
	if (pDBOUserIndividual->szUserNote[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szUserNote);
		SendPacket.AddPacket(pDBOUserIndividual->szUserNote,wBufferSize,DTP_GP_UI_USER_NOTE);
	}

	//��ʵ����
	if (pDBOUserIndividual->szCompellation[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szCompellation);
		SendPacket.AddPacket(pDBOUserIndividual->szCompellation,wBufferSize,DTP_GP_UI_COMPELLATION);
	}

	//�绰����
	if (pDBOUserIndividual->szSeatPhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szSeatPhone);
		SendPacket.AddPacket(pDBOUserIndividual->szSeatPhone,wBufferSize,DTP_GP_UI_SEAT_PHONE);
	}

	//�ƶ��绰
	if (pDBOUserIndividual->szMobilePhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szMobilePhone);
		SendPacket.AddPacket(pDBOUserIndividual->szMobilePhone,wBufferSize,DTP_GP_UI_MOBILE_PHONE);
	}

	//��ϵ����
	if (pDBOUserIndividual->szQQ[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szQQ);
		SendPacket.AddPacket(pDBOUserIndividual->szQQ,wBufferSize,DTP_GP_UI_QQ);
	}

	//�����ʼ�
	if (pDBOUserIndividual->szEMail[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szEMail);
		SendPacket.AddPacket(pDBOUserIndividual->szEMail,wBufferSize,DTP_GP_UI_EMAIL);
	}

	//��ϵ��ַ
	if (pDBOUserIndividual->szDwellingPlace[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szDwellingPlace);
		SendPacket.AddPacket(pDBOUserIndividual->szDwellingPlace,wBufferSize,DTP_GP_UI_DWELLING_PLACE);
	}

	//������Ϣ
	WORD wSendSize=sizeof(CMD_GP_UserIndividual)+SendPacket.GetDataSize();
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INDIVIDUAL,cbDataBuffer,wSendSize);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureInfo * pUserInsureInfo=(DBO_GP_UserInsureInfo *)pData;

	//��������
	CMD_GP_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//��������
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//���гɹ�
bool CAttemperEngineSink::OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//��������
	CMD_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//���ñ���
	UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	UserInsureSuccess.lUserScore=pUserInsureSuccess->lSourceScore+pUserInsureSuccess->lVariationScore;
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//������Ϣ
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	DBO_GP_UserInsureFailure * pUserInsureFailure=(DBO_GP_UserInsureFailure *)pData;

	//��������
	UserInsureFailure.lResultCode=pUserInsureFailure->lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pUserInsureFailure->szDescribeString,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�����ɹ�
bool CAttemperEngineSink::OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//��������
	DBO_GP_OperateSuccess * pOperateSuccess=(DBO_GP_OperateSuccess *)pData;

	//��������
	OperateSuccess.lResultCode=pOperateSuccess->lResultCode;
	lstrcpyn(OperateSuccess.szDescribeString,pOperateSuccess->szDescribeString,CountArray(OperateSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//��������
	DBO_GP_OperateFailure * pOperateFailure=(DBO_GP_OperateFailure *)pData;

	//��������
	OperateFailure.lResultCode=pOperateFailure->lResultCode;
	lstrcpyn(OperateFailure.szDescribeString,pOperateFailure->szDescribeString,CountArray(OperateFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ( (m_pBindParameter+LOWORD(dwContextID))->dwSocketID != dwContextID) 
		return true;

	//��������
	DBO_MB_LogonSuccess * pDBOLogonSuccess=(DBO_MB_LogonSuccess *)pData;

	//��������
	CMD_MB_LogonSuccess LogonSuccess;
	ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

	//��������
	LogonSuccess.wFaceID=pDBOLogonSuccess->wFaceID;
	LogonSuccess.cbGender=pDBOLogonSuccess->cbGender;
	LogonSuccess.dwUserID=pDBOLogonSuccess->dwUserID;
	LogonSuccess.dwExperience=pDBOLogonSuccess->dwExperience;
	lstrcpyn(LogonSuccess.szNickName,pDBOLogonSuccess->szNickName,CountArray(LogonSuccess.szNickName));

	//��¼�ɹ�
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

	//���ͷ���
	WORD wIndex=LOWORD(dwContextID);
	SendMobileKindInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	SendMobileServerInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_SERVER_LIST,SUB_MB_LIST_FINISH);
	//��ʾ��Ϣ
	CString strLog;
	strLog.Format(L"�ֻ���½�ɹ���Nick:%s, Desc:%s", pDBOLogonSuccess->szNickName, pDBOLogonSuccess->szDescribeString);
	CTraceService::TraceString(strLog,TraceLevel_Normal);

	return true;
}
//��ѯ��Ϸ�������
bool CAttemperEngineSink::OnDBQueryOpenResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_QueryLotResult)==0);
	if (wDataSize%sizeof(DBO_GP_QueryLotResult)!=0) return false;

	DBO_GP_QueryLotResult* pQueryLotResult0 = ((DBO_GP_QueryLotResult*)pData);
	int nCount = wDataSize/sizeof(DBO_GP_QueryLotResult);
	int nFirstTypeID = pQueryLotResult0->wKindID;
	int nSecondTypeID = 0;

	int nIndex = 0;
	for (int i = 0;i < nCount;i++)
	{
		DBO_GP_QueryLotResult* pQueryLotResult = ((DBO_GP_QueryLotResult*)pData+i);

		nSecondTypeID = pQueryLotResult->wKindID;
		if(nFirstTypeID != nSecondTypeID)
		{
			int nSize = m_AllLuckNum.size();
			m_AllLuckNum.insert(make_pair(nFirstTypeID,&m_MyLuckNum[nFirstTypeID]));
			nFirstTypeID = nSecondTypeID;
			nIndex = 0;
		}
		m_MyLuckNum[nFirstTypeID].LuckNum[nIndex].wKindID = pQueryLotResult->wKindID;
		lstrcpyn(m_MyLuckNum[nFirstTypeID].LuckNum[nIndex].qihao,pQueryLotResult->wPeriod,CountArray(m_MyLuckNum[nFirstTypeID].LuckNum[nIndex].qihao));
		lstrcpyn(m_MyLuckNum[nFirstTypeID].LuckNum[nIndex].haoma,pQueryLotResult->szLotNum,CountArray(m_MyLuckNum[nFirstTypeID].LuckNum[nIndex].haoma));
		lstrcpyn(m_MyLuckNum[nFirstTypeID].LuckNum[nIndex].shijian,pQueryLotResult->szShijian,CountArray(m_MyLuckNum[nFirstTypeID].LuckNum[nIndex].shijian));

		nIndex++;
		if (nIndex >= 5)
		{
			nIndex = 0;
		}
	}
	int nSize = m_AllLuckNum.size();
	m_AllLuckNum.insert(make_pair(nFirstTypeID,&m_MyLuckNum[nFirstTypeID]));

//	m_AllLuckNum.insert(make_pair(nTypeID,&MyLuckNum[nTypeID]));
//	if(nTypeID == m_nLuckNumID)

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_GAME_RET,pData,wDataSize);


	return true;
}
//��ѯ��Ϸ�������
bool CAttemperEngineSink::OnDBQueryYinHangResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GetBankName)==0);
	if (wDataSize%sizeof(DBO_GP_GetBankName)!=0) return false;


	int nCount = wDataSize/sizeof(DBO_GP_GetBankName);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_YINHANG_RET,pData,wDataSize);


	return true;
}
//��ѯ��ע��־����
bool CAttemperEngineSink::OnDBQueryTouzhuLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetTouzhuLogCountRet));
	if(wDataSize != sizeof(DBO_GR_GetTouzhuLogCountRet)) return false;
	DBO_GR_GetTouzhuLogCountRet *pLogRet = (DBO_GR_GetTouzhuLogCountRet*)pData;

	CMD_GR_GetTouzhuLogCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwTouzhuLogCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_TOUZHU_LOG_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//ע�����ӷ���
bool CAttemperEngineSink::OnDBQueryRegUrlResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GetRegUrl)==0);
	if (wDataSize%sizeof(DBO_GetRegUrl)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_REG_URL_RET,pData,wDataSize);
	return true;
}
//��ȡ����
bool CAttemperEngineSink::OnDBQueryPeieResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize== sizeof(DBR_GetPeieRet));
	if (wDataSize!=sizeof(DBR_GetPeieRet)) return false;
	DBR_GetPeieRet *pLogRet = (DBR_GetPeieRet*)pData;

	CMD_GetPeieRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.n_t_peie_1 = pLogRet->n_t_peie_1;
	LogRet.n_t_peie_2 = pLogRet->n_t_peie_2;
	LogRet.n_t_peie_3 = pLogRet->n_t_peie_3;
	LogRet.n_t_peie_s_1 = pLogRet->n_t_peie_s_1;
	LogRet.n_t_peie_s_2 = pLogRet->n_t_peie_s_2;
	LogRet.n_t_peie_s_3 = pLogRet->n_t_peie_s_3;


	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_Peie_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ȡ�¼�����
bool CAttemperEngineSink::OnDBQueryXjPeieResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize== sizeof(DBO_GetXjPeieRet));
	if (wDataSize!=sizeof(DBO_GetXjPeieRet)) return false;
	DBO_GetXjPeieRet *pLogRet = (DBO_GetXjPeieRet*)pData;

	CMD_GetXjPeieRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.n_t_peie_1 = pLogRet->n_t_peie_1;
	LogRet.n_t_peie_2 = pLogRet->n_t_peie_2;
	LogRet.n_t_peie_3 = pLogRet->n_t_peie_3;
	LogRet.n_t_peie_s_1 = pLogRet->n_t_peie_s_1;
	LogRet.n_t_peie_s_2 = pLogRet->n_t_peie_s_2;
	LogRet.n_t_peie_s_3 = pLogRet->n_t_peie_s_3;


	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_XJ_Peie_RET,&LogRet,sizeof(LogRet));
	return true;
}
//�����¼�����
bool CAttemperEngineSink::OnDBQuerySetXjPeieResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize== sizeof(DBO_SetXjPeieRet));
	if (wDataSize!=sizeof(DBO_SetXjPeieRet)) return false;
	DBO_SetXjPeieRet *pLogRet = (DBO_SetXjPeieRet*)pData;

	CMD_SetXjPeieRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));

	LogRet.n_t_result = pLogRet->n_t_result;
	CopyMemory(LogRet.s_t_desc,pLogRet->s_t_desc,CountArray(LogRet.s_t_desc));

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_SET_XJ_Peie_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ������־����
bool CAttemperEngineSink::OnDBQueryTixianLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetTixianLogCountRet));
	if(wDataSize != sizeof(DBO_GR_GetTixianLogCountRet)) return false;
	DBO_GR_GetTixianLogCountRet *pLogRet = (DBO_GR_GetTixianLogCountRet*)pData;

	CMD_GR_GetTixianLogCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwTixianLogCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_TIXIAN_LOG_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ��ֵ��־����
bool CAttemperEngineSink::OnDBQueryChongzhiLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetChongzhiLogCountRet));
	if(wDataSize != sizeof(DBO_GR_GetChongzhiLogCountRet)) return false ;
	DBO_GR_GetChongzhiLogCountRet *pLogRet = (DBO_GR_GetChongzhiLogCountRet*)pData;

	CMD_GR_GetChongzhiLogCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwChongzhiLogCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_CHONGZHI_LOG_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ��Ա��Ϣ��־����
bool CAttemperEngineSink::OnDBQueryHYXXLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetHYXXLogCountRet));
	if(wDataSize != sizeof(DBO_GR_GetHYXXLogCountRet)) return false ;
	DBO_GR_GetHYXXLogCountRet *pLogRet = (DBO_GR_GetHYXXLogCountRet*)pData;

	CMD_GR_GetHYXXLogCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwHYXXLogCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_HYXX_LOG_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯӯ����־����
bool CAttemperEngineSink::OnDBQueryYingkuiLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetYingkuiLogCountRet));
	if(wDataSize != sizeof(DBO_GR_GetYingkuiLogCountRet)) return false;
	DBO_GR_GetYingkuiLogCountRet *pLogRet = (DBO_GR_GetYingkuiLogCountRet*)pData;

	CMD_GR_GetYingkuiLogCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwYingkuiLogCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_YINGKUI_LOG_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯӯ����ϸ����
bool CAttemperEngineSink::OnDBQueryYingkuiMxCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetYingkuiMxCountRet));
	if(wDataSize != sizeof(DBO_GR_GetYingkuiMxCountRet)) return false;
	DBO_GR_GetYingkuiMxCountRet *pLogRet = (DBO_GR_GetYingkuiMxCountRet*)pData;

	CMD_GR_GetYingkuiMxCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwYingkuiMxCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_YINGKUI_MX_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ����ӯ����ϸ����
bool CAttemperEngineSink::OnDBQueryQiPaiYingkuiCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetQiPaiYingkuiCountRet));
	if(wDataSize != sizeof(DBO_GR_GetQiPaiYingkuiCountRet)) return false;
	DBO_GR_GetQiPaiYingkuiCountRet *pLogRet = (DBO_GR_GetQiPaiYingkuiCountRet*)pData;

	CMD_GR_GetQiPaiYingkuiCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwYingkuiMxCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_QP_YINGKUI_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ�¼�Ͷע��־����
bool CAttemperEngineSink::OnDBQueryXITZHLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetXJTZHLogCountRet));
	if(wDataSize != sizeof(DBO_GR_GetXJTZHLogCountRet)) return false;
// 	DBO_GR_GetXJTZHLogCountRet *pLogRet = (DBO_GR_GetXJTZHLogCountRet*)pData;
// 
// 	CMD_GR_GetXJTZHLogCountRet LogRet;
// 	ZeroMemory(&LogRet,sizeof(LogRet));
// 	LogRet.dwCount = pLogRet->dwCount;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_XJTZH_LOG_COUNT_RET,pData,wDataSize);
	return true;
}
//��ѯ�¼�Ͷע��־����
bool CAttemperEngineSink::OnDBCHKXITZHLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_CHKXJTZHLogCountRet));
	if(wDataSize != sizeof(DBO_GR_CHKXJTZHLogCountRet)) return false;
	DBO_GR_CHKXJTZHLogCountRet *pLogRet = (DBO_GR_CHKXJTZHLogCountRet*)pData;

	CMD_GR_CHKXJTZHLogCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_CHK_XJTZH_LOG_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ�¼�ӯ����־����
bool CAttemperEngineSink::OnDBXJYKLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_CHKXJTZHLogCountRet));
	if(wDataSize != sizeof(DBO_GR_CHKXJTZHLogCountRet)) return false;
	DBO_GR_CHKXJTZHLogCountRet *pLogRet = (DBO_GR_CHKXJTZHLogCountRet*)pData;

	CMD_GR_CHKXJTZHLogCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_XJYK_LOG_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ�¼�ӯ��ͳ������
bool CAttemperEngineSink::OnDBXJYKTjCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetXJYKTjCountRet));
	if(wDataSize != sizeof(DBO_GR_GetXJYKTjCountRet)) return false;
	DBO_GR_GetXJYKTjCountRet *pLogRet = (DBO_GR_GetXJYKTjCountRet*)pData;

	CMD_GR_GetXJYKTjCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_XJYK_TJ_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ�¼���Ϸӯ��ͳ������
bool CAttemperEngineSink::OnDBXJYXTjCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetXJYXTjCountRet));
	if(wDataSize != sizeof(DBO_GR_GetXJYXTjCountRet)) return false;
	DBO_GR_GetXJYXTjCountRet *pLogRet = (DBO_GR_GetXJYXTjCountRet*)pData;

	CMD_GR_GetXJYXTjCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_XJYX_TJ_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ�¼���ֵ��־����
bool CAttemperEngineSink::OnDBXJCHZHLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetXjChzhLogCountRet));
	if(wDataSize != sizeof(DBO_GR_GetXjChzhLogCountRet)) return false;
	DBO_GR_GetXjChzhLogCountRet *pLogRet = (DBO_GR_GetXjChzhLogCountRet*)pData;

	CMD_GR_GetXjChzhLogCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_XJCHZH_LOG_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ�¼�������־����
bool CAttemperEngineSink::OnDBXJTxLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GR_GetXjTxLogCountRet));
	if(wDataSize != sizeof(DBO_GR_GetXjTxLogCountRet)) return false;
	DBO_GR_GetXjTxLogCountRet *pLogRet = (DBO_GR_GetXjTxLogCountRet*)pData;

	CMD_GR_GetXjTxLogCountRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));
	LogRet.dwCount = pLogRet->dwCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_XJTX_LOG_COUNT_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ֵ��Ϣ����
bool CAttemperEngineSink::OnDBGetChongzhiXinxiResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GP_GetChongzhiTypeRet)==0);
	if(wDataSize % sizeof(DBO_GP_GetChongzhiTypeRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_CHONG_ZHI_TYPE_RET,pData,wDataSize);
	return true;
}
//���࿪����¼����
bool CAttemperEngineSink::OnDBGetMoreRecordResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GP_GetMoreRecordRet)==0);
	if(wDataSize % sizeof(DBO_GP_GetMoreRecordRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_MORE_RECORD_RET,pData,wDataSize);
	return true;
}
//�����������
bool CAttemperEngineSink::OnDBGetDailiHuikuiResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_GetDailiHuikuiRet));
	if(wDataSize != sizeof(DBO_GP_GetDailiHuikuiRet)) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_DAILI_HUIKUI_RET,pData,wDataSize);
	return true;
}
//�����콱����
bool CAttemperEngineSink::OnDBDailiLingjiangResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_DailiLingjiangRet));
	if(wDataSize != sizeof(DBO_GP_DailiLingjiangRet)) return false;

	DBO_GP_DailiLingjiangRet* pDaili = (DBO_GP_DailiLingjiangRet*)pData;

	CMD_GP_DailiLingjiangRet DailiLingjiangRet;
	ZeroMemory(&DailiLingjiangRet,sizeof(DailiLingjiangRet));

	DailiLingjiangRet.n_t_state = pDaili->n_t_state;
	DailiLingjiangRet.f_t_jine = pDaili->f_t_jine;
	if(pDaili->n_t_state == 0)
	{
		GetUserYueInfo(pDaili->n_t_userid,dwContextID);
	}

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_DAILI_LJ_RET,&DailiLingjiangRet,sizeof(DailiLingjiangRet));
	return true;
}
//ȡ����Ϣ����
bool CAttemperEngineSink::OnDBGetQukuanInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_GetQukuanInfoRet));
	if(wDataSize != sizeof(DBO_GP_GetQukuanInfoRet)) return false;
	DBO_GP_GetQukuanInfoRet *pLogRet = (DBO_GP_GetQukuanInfoRet*)pData;

	CMD_GP_GetQukuanInfoRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));

	LogRet.f_t_yue = pLogRet->f_t_yue;
	lstrcpyn(LogRet.s_t_yinhang,pLogRet->s_t_yinhang,sizeof(LogRet.s_t_yinhang));
	lstrcpyn(LogRet.s_t_kaihuren,pLogRet->s_t_kaihuren,sizeof(LogRet.s_t_kaihuren));
	lstrcpyn(LogRet.s_t_zhanghao,pLogRet->s_t_zhanghao,sizeof(LogRet.s_t_zhanghao));
	LogRet.n_t_isvip = pLogRet->n_t_isvip;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QU_KUAN_INFO_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ�����Ϣ����
bool CAttemperEngineSink::OnDBGetYueInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_GetLastYueRet));
	if(wDataSize != sizeof(DBO_GP_GetLastYueRet)) return false;
	DBO_GP_GetLastYueRet *pLogRet = (DBO_GP_GetLastYueRet*)pData;

	CMD_GP_GetLastYueRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));

	LogRet.f_t_yue = pLogRet->f_t_yue;

	LogRet.f_t_dongjie = pLogRet->f_t_dongjie;
	LogRet.f_t_score = pLogRet->f_t_score;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_LAST_YUE_RET,&LogRet,sizeof(LogRet));
	return true;
}
//�˳���Ϸ����
bool CAttemperEngineSink::OnDBQuitGameResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_QuitGameRet));
	if(wDataSize != sizeof(DBO_GP_QuitGameRet)) return false;


	DBO_GP_QuitGameRet *pLogRet = (DBO_GP_QuitGameRet*)pData;
	CMD_GP_QuitGameRet QuitGameRet;

	QuitGameRet.nResult = pLogRet->nResult;


	CString strLog;
	strLog.Format(L"[%d]�˳���½",pLogRet->nUserID);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUIT_GAME_RET,&QuitGameRet,sizeof(QuitGameRet));
	m_UserSocketID[pLogRet->nUserID]=0;

	return true;
}
//ǩ����Ϣ����
bool CAttemperEngineSink::OnDBUserQiandaoResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{

	ASSERT(wDataSize == sizeof(DBO_GP_UserQiandaoRet));
	if(wDataSize != sizeof(DBO_GP_UserQiandaoRet)) return false;
	DBO_GP_UserQiandaoRet *pLogRet = (DBO_GP_UserQiandaoRet*)pData;

	CMD_GP_UserQiandaoRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));


	LogRet.n_t_res = pLogRet->n_t_res;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_USER_QIANDAO_RET,&LogRet,sizeof(LogRet));
	return true;
}
//�����ͷ���
bool CAttemperEngineSink::OnDBUserHuanlesongResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_GetUserHuanlesongRet));
	if(wDataSize != sizeof(DBO_GP_GetUserHuanlesongRet)) return false;
	DBO_GP_GetUserHuanlesongRet *pLogRet = (DBO_GP_GetUserHuanlesongRet*)pData;

	CMD_GP_GetUserHuanlesongRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));

	LogRet.nRes = pLogRet->nRes;
	LogRet.f_t_xiaofei = pLogRet->f_t_xiaofei;
	LogRet.f_t_prize = pLogRet->f_t_prize;
	LogRet.n_t_count = pLogRet->n_t_count;
	LogRet.n_t_XXcount = pLogRet->n_t_XXcount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_USER_HUANLESONG_RET,&LogRet,sizeof(LogRet));
	return true;
}
//�콱����
bool CAttemperEngineSink::OnDBUserLingjiangResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_GetLingJiangRet));
	if(wDataSize != sizeof(DBO_GP_GetLingJiangRet)) return false;
	DBO_GP_GetLingJiangRet *pLogRet = (DBO_GP_GetLingJiangRet*)pData;

	CMD_GP_GetLingJiangRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));

	LogRet.n_t_got = pLogRet->n_t_got;
	LogRet.f_t_jine = pLogRet->f_t_jine;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_LING_JIANG_RET,&LogRet,sizeof(LogRet));
	return true;
}
//ȡ���
bool CAttemperEngineSink::OnDBDoQukuanResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_DoQukuanRet));
	if(wDataSize != sizeof(DBO_GP_DoQukuanRet)) return false;
	DBO_GP_DoQukuanRet *pLogRet = (DBO_GP_DoQukuanRet*)pData;

	CMD_GP_DoQukuanRet LogRet;
	ZeroMemory(&LogRet,sizeof(LogRet));

	lstrcpyn(LogRet.szDesc,pLogRet->szDesc,sizeof(LogRet.szDesc));
	LogRet.nResult = pLogRet->nResult;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_DO_QU_KUAN_RET,&LogRet,sizeof(LogRet));
	return true;
}
//��ѯ��ע��־
bool CAttemperEngineSink::OnDBGetTouzhuLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetTouzhuLogRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetTouzhuLogRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_TOUZHU_LOG_RET,pData, wDataSize);

	return true;
}
//��ѯ����
bool CAttemperEngineSink::OnDBGetNewsResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GP_GetNewsRet)==0);
	if(wDataSize % sizeof(DBO_GP_GetNewsRet)!=0) return false;
 	CString strLog;
 

	CMD_GP_GetNewsRet GetNewsRet[5];
	ZeroMemory(&GetNewsRet,sizeof(GetNewsRet));
	CString str ;

	int i = 0;
 	int nCount = (wDataSize / sizeof(DBO_GP_GetNewsRet));
 	for( i = 0;i < nCount;i++)
 	{
 		DBO_GP_GetNewsRet* pGetNews = ((DBO_GP_GetNewsRet*)pData+i);

		lstrcpyn(GetNewsRet[i].s_t_news,pGetNews->s_t_news,sizeof(GetNewsRet[i].s_t_news));

 		CString strNews;
 		strNews.Format(L"%s",pGetNews->s_t_news);
 		m_mapNews[i] = strNews;
 
		str.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), pGetNews->s_t_time.wYear,pGetNews->s_t_time.wMonth,
			pGetNews->s_t_time.wDay, pGetNews->s_t_time.wHour, pGetNews->s_t_time.wMinute, pGetNews->s_t_time.wSecond);


 	//	CString strLog;
 	}

	bool bSendToUser = false;
	if (m_strNewsTime.IsEmpty())
	{
		m_strNewsTime = str;
		bSendToUser = true;
	}
	else if(m_strNewsTime != str)
	{
		bSendToUser = true;
		m_strNewsTime = str;
	}
	strLog.Format(L"GETNEWS RESULT %s,bSendToUser:%d,m_strNewsTime:%s,str:%s",m_mapNews[i],bSendToUser,m_strNewsTime,str);
	OutputDebugString(strLog);

	if(bSendToUser)
	{
		map<int,DWORD>::iterator it;
		for(it=m_UserSocketID.begin();it!=m_UserSocketID.end();++it)
		{
			CMD_GP_GetNewsRet GetNewsRet[20];
			ZeroMemory(GetNewsRet,sizeof(GetNewsRet));

			for( i = 0;i < nCount;i++)
			{
				lstrcpyn(GetNewsRet[i].s_t_news,m_mapNews[i],sizeof(GetNewsRet[i].s_t_news));
				strLog.Format(L"GETNEWS Send %d %s ,user:%d",i,GetNewsRet[i].s_t_news,it->first);
				OutputDebugString(strLog);

			}

			m_pITCPNetworkEngine->SendData(it->second,MDM_GP_USER_SERVICE,SUB_GP_GET_NEWS_RET,&GetNewsRet, sizeof(CMD_GP_GetNewsRet)*i);
		}
	}
 
	//m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_NEWS_RET,&GetNewsRet, sizeof(CMD_GP_GetNewsRet)*i);

	return true;
}
//ת������
bool CAttemperEngineSink::OnDBZhuanhuanResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_ZhuanHuanRet));
	if(wDataSize != sizeof(DBO_GP_ZhuanHuanRet)) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_ZHUANHUAN_RET,pData, wDataSize);

	return true;
}
//�ͷ�URL
bool CAttemperEngineSink::OnGetKefuUrlRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBO_GP_GetKefuUrlRet));
	if(wDataSize != sizeof(DBO_GP_GetKefuUrlRet)) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_KEFU_URL_RET,pData, wDataSize);

	return true;
}
//��ȡMAPBONUS
bool CAttemperEngineSink::OnGetMapBonusRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetMapBonusRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetMapBonusRet)!=0) return false;


	int nCount = (wDataSize/sizeof(DBO_GR_GetMapBonusRet));
	CString strLog;
	int nMap = 0;
	for(map<int ,tagMapBonus* >::iterator pos = mapMapBonus.begin();pos!=mapMapBonus.end();++pos)
	{
		nMap++;
	}

	for(int i = 0;i < nCount;i++)
	{
		DBO_GR_GetMapBonusRet* pGetMapBonusRet = ((DBO_GR_GetMapBonusRet*)pData+i);

		if(mapMapBonus.find(i) == mapMapBonus.end())
		{
 			tagMapBonus *pMapBonus = new tagMapBonus();
 
 			pMapBonus->MapBonus.f_t_bonus = pGetMapBonusRet->f_t_bonus;
 			pMapBonus->MapBonus.n_t_kind_id = pGetMapBonusRet->n_t_kind_id;
 			pMapBonus->MapBonus.n_t_type_id = pGetMapBonusRet->n_t_type_id;
 
			pMapBonus->MapBonus.n_t_bonustype = pGetMapBonusRet->n_t_bonustype;
			mapMapBonus[i] = pMapBonus;
		}
		else
		{
			tagMapBonus *pMapBonus = new tagMapBonus();

			pMapBonus->MapBonus.f_t_bonus = pGetMapBonusRet->f_t_bonus;
			pMapBonus->MapBonus.n_t_kind_id = pGetMapBonusRet->n_t_kind_id;
			pMapBonus->MapBonus.n_t_type_id = pGetMapBonusRet->n_t_type_id;

			pMapBonus->MapBonus.n_t_bonustype = pGetMapBonusRet->n_t_bonustype;
			mapMapBonus[nMap+i] = pMapBonus;

// 		//	int nMap = mapMapBonus.size();
// 			mapMapBonus[nMap]->MapBonus.f_t_bonus = pGetMapBonusRet->f_t_bonus;
// 			mapMapBonus[nMap]->MapBonus.n_t_kind_id = pGetMapBonusRet->n_t_kind_id;
// 			mapMapBonus[nMap]->MapBonus.n_t_type_id = pGetMapBonusRet->n_t_type_id;
// 			mapMapBonus[nMap++]->MapBonus.n_t_bonustype = pGetMapBonusRet->n_t_bonustype;

		}

		if(mapMapBonus[nMap+i]->MapBonus.n_t_kind_id == 66 && mapMapBonus[nMap+i]->MapBonus.n_t_type_id == 1)
		{
			strLog.Format(L"\nGETMAPBONUS f_t_bonus:%0.2lf,n_t_kind_id:%d,n_t_type_id:%d",mapMapBonus[nMap+i]->MapBonus.f_t_bonus,mapMapBonus[nMap+i]->MapBonus.n_t_kind_id,mapMapBonus[nMap+i]->MapBonus.n_t_type_id);
			OutputDebugString(strLog);

		}
	}

//	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_MAP_BONUS_RET,pData, wDataSize);

	return true;
}
//��ȡ�н��û�ID
bool CAttemperEngineSink::OnGetWinUserIDRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetWinUserID)==0);
	if(wDataSize % sizeof(DBO_GR_GetWinUserID)!=0) return false;

	int nCount = (wDataSize / sizeof(DBO_GR_GetWinUserID));

	for(int i = 0;i < nCount;i++)
	{
		DBO_GR_GetWinUserID* pGetWinUserID = ((DBO_GR_GetWinUserID*)pData+i);
		int nUserID = pGetWinUserID->nUserID;
		CString strLog;
		strLog.Format(L"SUB_GP_GET_TOUZHU_TISHI_RET nUserID:%d ",nUserID);
		OutputDebugString(strLog);
		if(m_UserSocketID.find(nUserID) != m_UserSocketID.end())
		{

			if(m_UserSocketID[nUserID]!=0)
			{
				GetUserYueInfo(nUserID,m_UserSocketID[nUserID]);

			 	CMD_GR_TouzhuTishiRet TouzhuTishiRet;
			 	ZeroMemory(&TouzhuTishiRet,sizeof(TouzhuTishiRet));
			 	TouzhuTishiRet.nWinZhushu = pGetWinUserID->nWinZhushu;
			 	TouzhuTishiRet.nState = pGetWinUserID->nState;
			 	lstrcpyn(TouzhuTishiRet.szQishu,pGetWinUserID->szQishu,sizeof(TouzhuTishiRet.szQishu));
			 	TouzhuTishiRet.nType = pGetWinUserID->nType;
				TouzhuTishiRet.f_yingkui = pGetWinUserID->f_yingkui;
				TouzhuTishiRet.nZhushu = pGetWinUserID->nZhushu;
				bool bSuccess = m_pITCPNetworkEngine->SendData(m_UserSocketID[nUserID],MDM_GP_USER_SERVICE,SUB_GP_GET_TOUZHU_TISHI_RET,&TouzhuTishiRet, sizeof(TouzhuTishiRet));

			}	
		}
	}
	return true;
}
//��ȡϵͳʱ��
bool CAttemperEngineSink::OnGetSysTimeRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetSystemTime)==0);
	if(wDataSize % sizeof(DBO_GR_GetSystemTime)!=0) return false;
	SYSTEMTIME	ServerTime;							//ϵͳʱ��

	DBO_GR_GetSystemTime* pGetSysTime = (DBO_GR_GetSystemTime*)pData;


	ServerTime = pGetSysTime->ServerTime;


	CTime sqltime(ServerTime);
	SYSTEMTIME	NowTime;	
	::GetLocalTime(&NowTime);
	CTime nowtime(NowTime);

// 	CString strLog;
// 	strLog.Format(L"NowTime = %s | %d-%d-%d %d:%d:%d",nowtime.Format(L"%Y-%m-%d %H:%M:%S"),ServerTime.wYear,ServerTime.wMonth,ServerTime.wDay,ServerTime.wHour,ServerTime.wMinute,ServerTime.wSecond);
// 	CTraceService::TraceString(strLog,TraceLevel_Normal);
	m_timespan = sqltime-nowtime;
	CString strLog;
	strLog.Format(L"���µ�¼�����������ݿ��ʱ���:%ld(s)",(long)m_timespan.GetTotalSeconds());
	CTraceService::TraceString(strLog,TraceLevel_Normal);
// 
// 
// 	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_TIME_RET,pData, wDataSize);

	return true;
}
//��ѯ�¼���ֵ��־
bool CAttemperEngineSink::OnDBGetXJCHZHLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetXJCHZHLogRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetXJCHZHLogRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_XJCHZH_LOG_RET,pData, wDataSize);

	return true;
}
//��ѯ�¼�������־
bool CAttemperEngineSink::OnDBGetXJTxLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetXJTxLogRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetXJTxLogRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_XJTX_LOG_RET,pData, wDataSize);

	return true;
}
//��ѯ�¼�ӯ����־
bool CAttemperEngineSink::OnDBGetXJYKLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetXJYKLogRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetXJYKLogRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_XJYK_LOG_RET,pData, wDataSize);

	return true;
}
//��ѯ�¼�ӯ����־
bool CAttemperEngineSink::OnDBGetXJYKTjResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetXJYKTjRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetXJYKTjRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_XJYK_TJ_RET,pData, wDataSize);

	return true;
}
//��ѯ�¼���Ϸӯ����־
bool CAttemperEngineSink::OnDBGetXJYXTjResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetXJYXTjRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetXJYXTjRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_XJYX_TJ_RET,pData, wDataSize);

	return true;
}
//��ѯ�¼���ע��־
bool CAttemperEngineSink::OnDBGetXJTZHLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetXJTZHLogRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetXJTZHLogRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_XJTZH_LOG_RET,pData, wDataSize);

	return true;
}
//��ѯ�¼���ע��־
bool CAttemperEngineSink::OnDBCHKXJTZHLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_CHKXJTZHLogRet)==0);
	if(wDataSize % sizeof(DBO_GR_CHKXJTZHLogRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_CHK_XJTZH_LOG_RET,pData, wDataSize);

	return true;
}
//��ѯ��ע��־
bool CAttemperEngineSink::OnDBGetChongzhiLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetChongzhiRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetChongzhiRet)!=0) return false; 

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_CHONGZHI_LOG_RET,pData, wDataSize);

	return true;
}
//��ѯ��ע��־
bool CAttemperEngineSink::OnDBGetYingkuiLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetYingkuiLogRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetYingkuiLogRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_YINGKUI_LOG_RET,pData, wDataSize);
	return true;
}
//��ѯӯ����ϸ
bool CAttemperEngineSink::OnDBGetYingkuiMxResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetYingkuiMxRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetYingkuiMxRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_YINGKUI_MX_RET,pData, wDataSize);

	return true;
}
//��ѯ����ӯ����ϸ
bool CAttemperEngineSink::OnDBGetQiPaiYingkuiResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetQiPaiYingkuiRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetQiPaiYingkuiRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_QP_YINGKUI_RET,pData, wDataSize);

	return true;
}
//��ѯ������־
bool CAttemperEngineSink::OnDBGetTixianLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetTixianLogRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetTixianLogRet)!=0) return false;


	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_TIXIAN_LOG_RET,pData, wDataSize);

	return true;
}
//��ѯ������־
bool CAttemperEngineSink::OnDBGetHYXXLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize % sizeof(DBO_GR_GetHYXXLogRet)==0);
	if(wDataSize % sizeof(DBO_GR_GetHYXXLogRet)!=0) return false;

	CString strLog;
	strLog.Format(L"HYXX SERVER wDataSize = %d,size = %d",wDataSize,sizeof(DBO_GR_GetHYXXLogRet));
	OutputDebugString(strLog);

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_HYXX_LOG_RET,pData, wDataSize);

	return true;
}
//��ѯ��ע��־
bool CAttemperEngineSink::OnDBCancelTouzhuResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_CancelTouzhuRet)==0);
	if (wDataSize%sizeof(DBO_GR_CancelTouzhuRet)!=0) return false;

	DBO_GR_CancelTouzhuRet* pCancelRet = (DBO_GR_CancelTouzhuRet*)pData;
	CMD_GR_CancelTouzhuRet CancelTouzhu;
	ZeroMemory(&CancelTouzhu,sizeof(CancelTouzhu));

	CancelTouzhu.nResult = pCancelRet->nResult;
 	if(CancelTouzhu.nResult == 0)
 	{
 		GetUserYueInfo(pCancelRet->n_t_userid,dwContextID);
 	}

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_CANCEL_TOUZHU_RET,&CancelTouzhu, sizeof(CancelTouzhu));

	return true;
}
//��ӻ�Ա����
bool CAttemperEngineSink::OnDBAddHuiyuanResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_AddHuiYuanRet)==0);
	if (wDataSize%sizeof(DBO_GR_AddHuiYuanRet)!=0) return false;

	DBO_GR_AddHuiYuanRet* pAddHuiYuan = (DBO_GR_AddHuiYuanRet*)pData;
	CMD_GR_AddHuiYuanRet AddHuiYuan;
	ZeroMemory(&AddHuiYuan,sizeof(AddHuiYuan));

	AddHuiYuan.n_t_res = pAddHuiYuan->n_t_res;
	memcpy(AddHuiYuan.s_t_Desc,pAddHuiYuan->s_t_Desc,sizeof(AddHuiYuan.s_t_Desc));

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_ADD_HY_RET,pData, wDataSize);

	return true;
}
//��ȡ���ϲ��ںŷ���
bool CAttemperEngineSink::OnDBGetLhcQihaoResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GetLhcQihaoRet)==0);
	if (wDataSize%sizeof(DBO_GetLhcQihaoRet)!=0) return false;


	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_LHC_QIHAO_RET,pData, wDataSize);

	return true;
}
//������ҳע�᷵�㷵��
bool CAttemperEngineSink::OnDBSetWebFandianResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_SetWebFandianRet)==0);
	if (wDataSize%sizeof(DBO_GR_SetWebFandianRet)!=0) return false;

	DBO_GR_SetWebFandianRet* pSetWebFandian = (DBO_GR_SetWebFandianRet*)pData;
	CMD_GR_SetWebFandianRet SetWebFandian;
	ZeroMemory(&SetWebFandian,sizeof(SetWebFandian));

	SetWebFandian.n_t_res = pSetWebFandian->n_t_res;
	SetWebFandian.f_t_fandian = pSetWebFandian->f_t_fandian;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_SET_WEB_FANDIAN_RET,pData, wDataSize);

	return true;
}
//��ѯ�����Ϣ����
bool CAttemperEngineSink::OnDBGetUserInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_GetUserInfoRet)==0);
	if (wDataSize%sizeof(DBO_GR_GetUserInfoRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_USER_INFO_RET,pData, wDataSize);

	return true;
}
//�޸�QQ����
bool CAttemperEngineSink::OnDBXGqqResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_XG_QQ_RET)==0);
	if (wDataSize%sizeof(DBO_GR_XG_QQ_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_XG_QQ_RET,pData, wDataSize);

	return true;
}
//�޸��Ա𷵻�
bool CAttemperEngineSink::OnDBXGGenderResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_AlterGenderRet)==0);
	if (wDataSize%sizeof(DBO_GP_AlterGenderRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_ALTER_GENDER_RET,pData, wDataSize);

	return true;
}
//�޸�QQ����
bool CAttemperEngineSink::OnDBXGHYFandianResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_XGHYFandian_RET)==0);
	if (wDataSize%sizeof(DBO_GP_XGHYFandian_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_XG_HY_FANDIAN_RET,pData, wDataSize);

	return true;
}
//��Ϊ������
bool CAttemperEngineSink::OnDBSzhidailiResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_SWDaili_RET)==0);
	if (wDataSize%sizeof(DBO_GP_SWDaili_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_SW_DAILI_RET,pData, wDataSize);

	return true;
}
//��Ա��Ϣת�˷���
bool CAttemperEngineSink::OnDBHyxxZhuanZhangResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_HYXX_ZhuanZhang_RET)==0);
	if (wDataSize%sizeof(DBO_GP_HYXX_ZhuanZhang_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_HYXX_ZHUANZHANG_RET,pData, wDataSize);

	return true;
}
//�޸ĵ�¼���뷵��
bool CAttemperEngineSink::OnDBXGLoginPassResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_XG_Loginpass_RET)==0);
	if (wDataSize%sizeof(DBO_GR_XG_Loginpass_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_XG_LOGIN_PASS_RET,pData, wDataSize);

	return true;
}
//������������
bool CAttemperEngineSink::OnDBLockMachineResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_LockMachine_RET)==0);
	if (wDataSize%sizeof(DBO_GR_LockMachine_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_LOCK_MACHINE_RET,pData, wDataSize);

	return true;
}
//�޸�ȡ�����뷵��
bool CAttemperEngineSink::OnDBXGQukuanPassResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_XG_Qukuanpass_RET)==0);
	if (wDataSize%sizeof(DBO_GR_XG_Qukuanpass_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_XG_QUKUAN_PASS_RET,pData, wDataSize);

	return true;
}
//����ȡ�������
bool CAttemperEngineSink::OnDBSetQukuanProtectResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_SetQukuanProtect_RET)==0);
	if (wDataSize%sizeof(DBO_GR_SetQukuanProtect_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_SET_QUKUAN_PROTECT_RET,pData, wDataSize);

	return true;
}
//��ȡ�ҵ������˻�����
bool CAttemperEngineSink::OnDBQueryMyYinhangResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBR_GR_QueryMyYinHang_RET)==0);
	if (wDataSize%sizeof(DBR_GR_QueryMyYinHang_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_QUERY_MY_YINHANG_RET,pData, wDataSize);

	return true;
}
//��ȡ�ҵ��˻���������
bool CAttemperEngineSink::OnDBQueryMyProtectResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_QueryMyProTect_RET)==0);
	if (wDataSize%sizeof(DBO_GR_QueryMyProTect_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_QUERY_MY_PROTECT_RET,pData, wDataSize);

	return true;
}
//��ȡ��Ա���ݷ���
bool CAttemperEngineSink::OnDBGetHyShjResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_GetHyShjRet)==0);
	if (wDataSize%sizeof(DBO_GR_GetHyShjRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_HYSHJ_RET,pData, wDataSize);

	return true;
}
//��ȡ���������
bool CAttemperEngineSink::OnDBGetDaLiBaoRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_GetDaLiBaoRet)==0);
	if (wDataSize%sizeof(DBO_GP_GetDaLiBaoRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_DA_LI_BAO_RET,pData, wDataSize);

	return true;
}
//��ȡ�ҽ𵰷���
bool CAttemperEngineSink::OnDBGetZaJinDanRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_GetZaJinDanRet)==0);
	if (wDataSize%sizeof(DBO_GP_GetZaJinDanRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_ZA_JIN_DAN_RET,pData, wDataSize);

	return true;
}
//��ȡ����ת�̷���
bool CAttemperEngineSink::OnDBGetLuckyZhuanRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_GetLuckyZhuanRet)==0);
	if (wDataSize%sizeof(DBO_GP_GetLuckyZhuanRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_LUCKY_ZHUAN_RET,pData, wDataSize);

	return true;
}
//��ȡ������ʸ񷵻�
bool CAttemperEngineSink::OnDBCanDaLiBaoRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_CanDaLiBaoRet)==0);
	if (wDataSize%sizeof(DBO_GP_CanDaLiBaoRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_CAN_DA_LI_BAO_RET,pData, wDataSize);

	return true;
}
//��ȡ�ҽ��ʸ񷵻�
bool CAttemperEngineSink::OnDBCanZaJinDanRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_CanZaJinDanRet)==0);
	if (wDataSize%sizeof(DBO_GP_CanZaJinDanRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_CAN_ZA_JIN_DAN_RET,pData, wDataSize);

	return true;
}
//��ȡ����ת���ʸ񷵻�
bool CAttemperEngineSink::OnDBCanLuckyZhuanRet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_CanLuckyZhuanRet)==0);
	if (wDataSize%sizeof(DBO_GP_CanLuckyZhuanRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_CAN_LUCKY_ZHUAN_RET,pData, wDataSize);

	return true;
}
//����ȡ���˺ŷ���
bool CAttemperEngineSink::OnDBSetQukuanZhanghaoResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GR_SetQukuanZhanghao_RET)==0);
	if (wDataSize%sizeof(DBO_GR_SetQukuanZhanghao_RET)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_SET_QUKUAN_ZHANGHAO_RET,pData, wDataSize);

	return true;
}
//��ȡͶע��ϸ��Ϣ
bool CAttemperEngineSink::OnDBGetTouzhuXXResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_GP_GetTouzhuRet)==0);
	if (wDataSize%sizeof(DBO_GP_GetTouzhuRet)!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_TOU_ZHU_XX_RET,pData, wDataSize);

	return true;
}
//��ȡͶע��ϸ��Ϣ
bool CAttemperEngineSink::OnDBMBGetTouzhuXXResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize%sizeof(DBO_MB_GetTouzhuRet)==0);
	if (wDataSize%sizeof(DBO_MB_GetTouzhuRet)!=0) return false;


	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_MB_GET_TOU_ZHU_XX_RET,pData, wDataSize);

	return true;
}
//��ֵ��ʾ����
bool CAttemperEngineSink::OnDBChongzhiTishiResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT((wDataSize% sizeof(DBO_GR_ChongzhiTishiRet))==0);
	if(( wDataSize% sizeof(DBO_GR_ChongzhiTishiRet))!=0) return false;


	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_CHONGZHI_TISHI_RET,pData, wDataSize);

	return true;
}
//������ʾ����
bool CAttemperEngineSink::OnDBTixianTishiResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT((wDataSize% sizeof(DBO_GR_TixianTishiRet))==0);
	if(( wDataSize% sizeof(DBO_GR_TixianTishiRet))!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_TIXIAN_TISHI_RET,pData, wDataSize);
	return true;

}
//Ͷע��ʾ����
bool CAttemperEngineSink::OnDBTouzhuTishiResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT((wDataSize% sizeof(DBO_GR_TouzhuTishiRet))==0);
	if(( wDataSize% sizeof(DBO_GR_TouzhuTishiRet))!=0) return false;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GET_TOUZHU_TISHI_RET,pData, wDataSize);
	return true;

}
//��ע����
bool CAttemperEngineSink::OnDBQueryTouzhuResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����

	ASSERT(wDataSize%sizeof(DBO_GR_TouzhuResult)==0);
	if (wDataSize%sizeof(DBO_GR_TouzhuResult)!=0) return false;


	DBO_GR_TouzhuResult *pTouzhuRes = (DBO_GR_TouzhuResult*)pData;

	CMD_GR_TouzhuRes TouzhuRes;
	ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
	TouzhuRes.lResult = pTouzhuRes->lResultCode;
	lstrcpyn(TouzhuRes.szDesc,pTouzhuRes->szDesc,sizeof(TouzhuRes.szDesc));
	TouzhuRes.nSign = pTouzhuRes->nSign;

	if(TouzhuRes.lResult == 0)
	{
		GetUserYueInfo(pTouzhuRes->nUserID,dwContextID);
	}
	CString strLog;
	strLog.Format(L"Ͷע���� UserID ��%d,lResult:%d", pTouzhuRes->nUserID, TouzhuRes.lResult);
	CTraceService::TraceString(strLog,TraceLevel_Exception);

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_RET,&TouzhuRes,sizeof(TouzhuRes));

	return true;
}
//��¼ʧ��
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_MB_LogonFailure * pLogonFailure=(DBO_MB_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameType)==0);
	if (wDataSize%sizeof(DBO_GP_GameType)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameType);
	DBO_GP_GameType * pGameType=(DBO_GP_GameType *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameType GameType;
		ZeroMemory(&GameType,sizeof(GameType));

		//��������
		GameType.wTypeID=(pGameType+i)->wTypeID;
		GameType.wSortID=(pGameType+i)->wSortID;
		lstrcpyn(GameType.szTypeName,(pGameType+i)->szTypeName,CountArray(GameType.szTypeName));

		//�����б�
		m_ServerListManager.InsertGameType(&GameType);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameKind)==0);
	if (wDataSize%sizeof(DBO_GP_GameKind)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameKind);
	DBO_GP_GameKind * pGameKind=(DBO_GP_GameKind *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameKind GameKind;
		ZeroMemory(&GameKind,sizeof(GameKind));

		//��������
		GameKind.wTypeID=(pGameKind+i)->wTypeID;
		GameKind.wSortID=(pGameKind+i)->wSortID;
		GameKind.wKindID=(pGameKind+i)->wKindID;
		GameKind.wGameID=(pGameKind+i)->wGameID;
		GameKind.dwOnLineCount=m_ServerListManager.CollectOnlineInfo((pGameKind+i)->wKindID);
		lstrcpyn(GameKind.szKindName,(pGameKind+i)->szKindName,CountArray(GameKind.szKindName));
		lstrcpyn(GameKind.szProcessName,(pGameKind+i)->szProcessName,CountArray(GameKind.szProcessName));

		//�����б�
		m_ServerListManager.InsertGameKind(&GameKind);
	}

	return true;
}


//��������
bool CAttemperEngineSink::OnDBPCLoadVirtualCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_VirtualCount)==0);
	if (wDataSize%sizeof(DBO_GP_VirtualCount)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_VirtualCount);
	DBO_GP_VirtualCount * pVirtualCount=(DBO_GP_VirtualCount *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		WORD wServerID=(pVirtualCount+i)->wServerID;
		CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

		//���ñ���
		if (pGameServerItem!=NULL)
		{
			pGameServerItem->m_dwVirtualCount=(pVirtualCount+i)->dwVirtualCount;
		}
	}

	return true;
}

//��Ϸ�б�
bool CAttemperEngineSink::OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GP_GameListResult));
	if (wDataSize!=sizeof(DBO_GP_GameListResult)) return false;

	//��������
	DBO_GP_GameListResult * pGameListResult=(DBO_GP_GameListResult *)pData;

	//��Ϣ����
	if (pGameListResult->cbSuccess==TRUE)
	{
		//�����б�
		m_ServerListManager.CleanKernelItem();

		//�¼�֪ͨ
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_LOAD_DB_LIST_RESULT,&ControlResult,sizeof(ControlResult));

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wLoadListTime*1000L,1,0);
	//	m_pITimerEngine->SetTimer(IDI_GET_SQL_TIME,3600*1000L,1,0);
	}
	else
	{
		//������ʾ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�������б����ʧ�ܣ�%ld ������¼���"),m_pInitParameter->m_wReLoadListTime);

		//��ʾ��Ϣ
		CTraceService::TraceString(szDescribe,TraceLevel_Warning);

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wReLoadListTime*1000L,1,0);
	}

	return true;
}

//�汾���
bool CAttemperEngineSink::CheckPlazaVersion(DWORD dwPlazaVersion, DWORD dwSocketID)
{
	//��������
	bool bMustUpdate=false;
	bool bAdviceUpdate=false;

	//�汾�ж�
	if (GetSubVer(dwPlazaVersion)<m_pInitParameter->m_dwSubVer) bAdviceUpdate=true;
	if (GetMainVer(dwPlazaVersion)!=m_pInitParameter->m_dwMainVer) bMustUpdate=true;
	if (GetProductVer(dwPlazaVersion)!=m_pInitParameter->m_dwProductVer) bMustUpdate=true;

	CString strLog;
	strLog.Format(L"BANBENHAO GetSubVer(dwPlazaVersion) = %d,GetSubVer(VERSION_PLAZA) = %d",GetSubVer(dwPlazaVersion),m_pInitParameter->m_dwSubVer);
	OutputDebugString(strLog);
	strLog.Format(L"BANBENHAO GetMainVer(dwPlazaVersion) = %d,GetMainVer(VERSION_PLAZA) = %d",GetMainVer(dwPlazaVersion),m_pInitParameter->m_dwMainVer);
	OutputDebugString(strLog);
	strLog.Format(L"BANBENHAO GetProductVer(dwPlazaVersion) = %d,GetProductVer(VERSION_PLAZA) = %d",GetProductVer(dwPlazaVersion),m_pInitParameter->m_dwProductVer);
	OutputDebugString(strLog);
	strLog.Format(L"BANBENHAO bAdviceUpdate = %d,bMustUpdate = %d",bAdviceUpdate,bMustUpdate);
	OutputDebugString(strLog);
	//�����ж�
	if ((bMustUpdate==true)||(bAdviceUpdate==true))
	{
		//��������
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//��������
		UpdateNotify.cbMustUpdate=bMustUpdate;
		UpdateNotify.cbAdviceUpdate=bAdviceUpdate;
		UpdateNotify.dwCurrentVersion=VERSION_PLAZA;

		//������Ϣ
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//�ж��ж�
		if (bMustUpdate==true) 
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return true;
		}
	}

	return true;
}

//��������
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//��������
VOID CAttemperEngineSink::SendGameTypeInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameTypeItem * pGameTypeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameTypeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameTypeItem=m_ServerListManager.EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameTypeItem->m_GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);

	return;
}

//��������
VOID CAttemperEngineSink::SendGameKindInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendGameServerInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
		wSendSize+=sizeof(tagGameServer);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);

	return;
}

//��������
VOID CAttemperEngineSink::SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID)
{
	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//��������
		if (pGameServerItem->m_GameServer.wKindID==wModuleID)
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);

	return;
}


CString	CAttemperEngineSink::ChangeStringToT(CStringA strInput)
{
	CString  strOutput;
	USES_CONVERSION;
	strOutput = A2T(strInput);

	return strOutput;
}

//////////////////////////////////////////////////////////////////////////////////
