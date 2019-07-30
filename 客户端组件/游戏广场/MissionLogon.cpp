#include "StdAfx.h"
#include "GlobalUnits.h"
#include "MissionLogon.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"
#include "Ping.h"
//////////////////////////////////////////////////////////////////////////////////

//��̬����
CMissionLogon * CMissionLogon::m_pMissionLogon=NULL;					//����ָ��

#define  IDI_LOGON_TIME	101
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionLogon, CDlgStatus)
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CMissionLogon::CMissionLogon()
{
	//���ñ���
	m_bRegister=false;
	m_bRemPassword=false;
	m_bKillSocket = false;
	//�ؼ�ָ��
	m_pDlgLogon=NULL;
	//m_pDlgRegister=NULL;
	m_bKefu = false;

	//�������
	SetStatusViewSink(this);
	m_nAutoLogon=0;
	//���ö���
	ASSERT(m_pMissionLogon==NULL);
	if (m_pMissionLogon==NULL) m_pMissionLogon=this;

	return;
}
//��������
CMissionLogon::~CMissionLogon()
{
	//ɾ������
	SafeDelete(m_pDlgLogon);
	//SafeDelete(m_pDlgRegister);

	//�ͷŶ���
	ASSERT(m_pMissionLogon==this);
	if (m_pMissionLogon==this) m_pMissionLogon=NULL;

	return;
}
void CMissionLogon::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == IDI_LOGON_TIME)
	{
		KillTimer(IDI_LOGON_TIME);
		OnStatusCancel();
	}
	CDlgStatus::OnTimer(nIDEvent);
}
//ȡ������
VOID CMissionLogon::OnStatusCancel()
{
	//���ؽ���
	HideStatusWindow();

	//��ֹ����
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,true);

	//��ȡ�����ļ�
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
	TCHAR szKey[32];
	CTime time = CTime::GetCurrentTime();

	_sntprintf(szKey,CountArray(szKey),_T("%d"),0);
	WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayCanUse"),szKey,szFileName);

	//���µ�¼
	if (m_bRegister==false)
	{
		ShowLogon();
	}
	else
	{
		ShowRegister();
	}

	return;
}

//�����¼�
bool CMissionLogon::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{
	//������
	if (nErrorCode!=0L)
	{
		//�ر���ʾ
		HideStatusWindow();


		//��ȡ�����ļ�
		//����Ŀ¼
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//����·��
		TCHAR szFileName[MAX_PATH]=TEXT("");
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
		TCHAR szKey[32];
		CTime time = CTime::GetCurrentTime();

		_sntprintf(szKey,CountArray(szKey),_T("%d"),0);
		WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayCanUse"),szKey,szFileName);

		//���µ�¼
		if (m_bRegister==false)
		{
			if (m_nAutoLogon>5)
			{
				//��ʾ��ʾ
				CInformation Information(CPlatformFrame::GetInstance());
				Information.ShowMessageBox(TEXT("���������еķ��������޷��ɹ����ӷ���������������վά�����棡"),MB_ICONERROR);
				m_nAutoLogon=0;
				ShowLogon();
			}
			else
			{
				m_nAutoLogon++;
				m_pDlgLogon->LoadLogonServerInfo();
				PerformLogonMission(m_bRemPassword);

			}
			
		}
		else
		{
			ShowRegister();
		}
	}
	else
	{
		if(m_bKefu)
		{
			m_bKefu = false;
			//��������
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_USER_SERVICE,SUB_GP_KEFU_URL);
			
		}
		else
		{
			//��������
			WORD wPacketSize,wSubCmdID;
			BYTE cbBuffer[SOCKET_TCP_PACKET];

			//��������
			if (m_bRegister==false)
			{
				wSubCmdID=SUB_GP_LOGON_ACCOUNTS;
				wPacketSize=m_pDlgLogon->ConstructLogonPacket(cbBuffer,sizeof(cbBuffer));
			}
	// 		else
	// 		{
	// 			wSubCmdID=SUB_GP_REGISTER_ACCOUNTS;
	// 			wPacketSize=m_pDlgRegister->ConstructRegisterPacket(cbBuffer,sizeof(cbBuffer));
	// 		}

			//��ʾ��Ϣ
			ShowStatusWindow(TEXT("������֤�û���¼��Ϣ..."));

			//��������
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_LOGON,wSubCmdID,cbBuffer,wPacketSize);

		}
		
	}

	return true;
}

//�ر��¼�
bool CMissionLogon::OnEventMissionShut(BYTE cbShutReason)
{
	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL && !m_bKillSocket)
	{

		TCHAR szPlatformUrl[150] = L"";
		TCHAR szPlatformUrl1[150] = L"";

		//��ȡ�����ļ�
		//����Ŀ¼
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//����·��
		TCHAR szFileName[MAX_PATH]=TEXT("");
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

		//��ȡ����
		CWHIniData IniData;
		IniData.SetIniFilePath(szFileName);


		int nTTL = 10000;
		CPing ping;
		PingReply reply;
		for(int i = 0;i < 5;i++)
		{
			memset(&reply, 0, sizeof(PingReply));
			TCHAR szKey[32];
			ZeroMemory(&szKey,sizeof(szKey));
			_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),i);
			//��ȡ����
			IniData.ReadEncryptString(TEXT("PlazaUrl"),szKey,NULL,szPlatformUrl1,CountArray(szPlatformUrl1));

			USES_CONVERSION;
			int nValidHostName = inet_addr(W2A(szPlatformUrl1));

			if(nValidHostName == -1)
			{
				DWORD dwServerAddr = 0;
				LPHOSTENT lpHost=gethostbyname(CT2CA(szPlatformUrl1));
				if (lpHost!=NULL) 
					dwServerAddr=((LPIN_ADDR)lpHost->h_addr)->s_addr;
				in_addr t1;
				t1.S_un.S_addr = dwServerAddr;
				char* pTrueAddr = inet_ntoa(t1);
				OutputDebugStringA(pTrueAddr);

				ping.Ping(dwServerAddr,&reply);

			}
			else
			{
				char szUrl[124] = "";
				int iLength ;  
				//��ȡ�ֽڳ���   
				iLength = WideCharToMultiByte(CP_ACP, 0, szPlatformUrl1, -1, NULL, 0, NULL, NULL);  
				//��tcharֵ����_char    
				WideCharToMultiByte(CP_ACP, 0, szPlatformUrl1, -1, szUrl, iLength, NULL, NULL);   
				ping.Ping(szUrl,&reply);
			}



			//CString strTmp;
			if(reply.m_dwRoundTripTime < nTTL || lstrlen(szPlatformUrl) == 0)
			{
				lstrcpyn(szPlatformUrl,szPlatformUrl1,CountArray(szPlatformUrl));
				if(reply.m_dwRoundTripTime > 0 && reply.m_dwRoundTripTime < 10000)
				{
					//���PINGͨ�ˣ�ע����¼һ�µ�ǰSERVER
					CWHRegKey RegServerAddr;
					RegServerAddr.OpenRegKey(TEXT("CurrentLogonServer"),true);
					RegServerAddr.WriteString(TEXT("CurrentIpAddress"),szPlatformUrl);
				}
				nTTL = reply.m_dwRoundTripTime;
			}
		}

		if (m_pDlgLogon!=NULL) lstrcpyn(m_pDlgLogon->m_szLogonServer,szPlatformUrl,CountArray(m_pDlgLogon->m_szLogonServer));

		PerformLogonMission(m_bRemPassword);

	}

	return true;
}

//��ȡ�¼�
bool CMissionLogon::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_LOGON)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_LOGON_SUCCESS:	//��¼�ɹ�
			{
				return OnSocketSubLogonSuccess(pData,wDataSize);
			}
		case SUB_GP_LOGON_FAILURE:	//��¼ʧ��
			{
				return OnSocketSubLogonFailure(pData,wDataSize);
			}
		case SUB_GP_LOGON_FINISH:	//��¼���
			{
				return OnSocketSubLogonFinish(pData,wDataSize);
			}
		case SUB_GP_UPDATE_NOTIFY:	//������ʾ
			{
				return OnSocketSubUpdateNotify(pData,wDataSize);
			}
		}
	}

	return false;
}

//��ʾ��¼
VOID CMissionLogon::ShowLogon()
{
	//Ч��״̬
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return;

	//���ñ���
	m_bRegister=false;

	//��������
	if (m_pDlgLogon==NULL)
	{
		m_pDlgLogon=new CDlgLogon;
	}

	//��������
	if (m_pDlgLogon->m_hWnd==NULL)
	{
		m_pDlgLogon->Create(IDD_DLG_LOGON,GetDesktopWindow());
	}

	//��ʾ����
	if (GetActiveStatus()==false)
	{
		m_pDlgLogon->ShowWindow(SW_SHOW);
		m_pDlgLogon->SetForegroundWindow();
	}

// 	//���ش���
// 	if ((m_pDlgRegister!=NULL)&&(m_pDlgRegister->m_hWnd!=NULL))
// 	{
// 		m_pDlgRegister->ShowWindow(SW_HIDE);
// 	}

	return;
}

//��ʾע��
VOID CMissionLogon::ShowRegister()
{
	//Ч��״̬
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return;

	//���ñ���
	m_bRegister=true;

// 	//��������
// 	if (m_pDlgRegister==NULL)
// 	{
// 		m_pDlgRegister=new CDlgRegister;
// 	}
// 	
// 	//��������
// 	if (m_pDlgRegister->m_hWnd==NULL)
// 	{
// 		m_pDlgRegister->Create(IDD_DLG_REGISTER,GetDesktopWindow());
// 	}
// 
// 	//��ʾ����
// 	m_pDlgRegister->ShowWindow(SW_SHOW);
// 	m_pDlgRegister->SetForegroundWindow();

	//���ش���
	if ((m_pDlgLogon!=NULL)&&(m_pDlgLogon->m_hWnd!=NULL))
	{
		m_pDlgLogon->ShowWindow(SW_HIDE);
	}

	return;
}

//��¼�û�
VOID CMissionLogon::UpdateUserInfo()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ñ�־
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if (pParameterGlobal!=NULL) pParameterGlobal->m_bRemberPassword=m_bRemPassword;

	//������Ϣ
	TCHAR szUserID[16];
	_sntprintf(szUserID,CountArray(szUserID),TEXT("%ld"),pGlobalUserData->dwUserID);

	//�ϴ��û�
	CWHRegKey RegUserInfo;
	RegUserInfo.OpenRegKey(REG_USER_INFO,true);
	RegUserInfo.WriteValue(TEXT("LastUserID"),pGlobalUserData->dwUserID);

	//��������
	TCHAR szGameID[16]=TEXT("");
	TCHAR szPassBuffer[MAX_ENCRYPT_LEN]=TEXT("");

	//�û�����
	CWHRegKey RegUserItem;
	RegUserItem.Attach(RegUserInfo.CreateItemKey(szUserID));

	//��������
	if (m_bRemPassword==true)
	{
		if (m_bRegister==false)
		{
			LPCTSTR pszPassword=m_pDlgLogon->m_szPassword;
			CWHEncrypt::XorEncrypt(pszPassword,szPassBuffer,CountArray(szPassBuffer));
		}
// 		else
// 		{
// 			LPCTSTR pszPassword=m_pDlgRegister->m_szLogonPass;
// 			CWHEncrypt::XorEncrypt(pszPassword,szPassBuffer,CountArray(szPassBuffer));
// 		}
	}

	//������Ϣ
	RegUserItem.WriteValue(TEXT("FaceID"),pGlobalUserData->wFaceID);
	RegUserItem.WriteValue(TEXT("AcitveTimeToken"),(DWORD)time(NULL));

	//�ʺ���Ϣ
	RegUserItem.WriteString(TEXT("GameID"),szGameID);
	RegUserItem.WriteString(TEXT("UserPassword"),szPassBuffer);
	RegUserItem.WriteString(TEXT("UserAccount"),pGlobalUserData->szAccounts);
	RegUserItem.WriteString(TEXT("UserPassToken"),pGlobalUserData->szPassword);

	return;
}
bool CMissionLogon::CreatePlazaview()
{
	CPlatformFrame* pPlatformFrame = CPlatformFrame::GetInstance();
	return pPlatformFrame->CreatePlazaview();

}
//ִ�е�¼
bool CMissionLogon::PerformKefu()
{
	//Ч��״̬
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true)
	{
		ASSERT(GetMissionManager()!=NULL);
		GetMissionManager()->SendData(MDM_GP_USER_SERVICE,SUB_GP_KEFU_URL);

		return false;
	}

	//��ȡ��ַ
	TCHAR szCustomServer[LEN_SERVER]=TEXT("");
	if (m_pDlgLogon!=NULL) lstrcpyn(szCustomServer,m_pDlgLogon->m_szLogonServer,CountArray(szCustomServer));

	m_bKefu = true;
	//���õ�ַ
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->SetCustomServer(szCustomServer);

	//��������
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{

		return false;
	}

	return true;
}

//ִ�е�¼
bool CMissionLogon::PerformLogonMission(bool bRemPassword)
{
	//Ч��״̬
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true)
	{
		//��������
		WORD wPacketSize,wSubCmdID;
		BYTE cbBuffer[SOCKET_TCP_PACKET];

		//��������
		if (m_bRegister==false)
		{
			wSubCmdID=SUB_GP_LOGON_ACCOUNTS;
			wPacketSize=m_pDlgLogon->ConstructLogonPacket(cbBuffer,sizeof(cbBuffer));
		}
		// 		else
		// 		{
		// 			wSubCmdID=SUB_GP_REGISTER_ACCOUNTS;
		// 			wPacketSize=m_pDlgRegister->ConstructRegisterPacket(cbBuffer,sizeof(cbBuffer));
		// 		}

		//��ʾ��Ϣ
		ShowStatusWindow(TEXT("������֤�û���¼��Ϣ..."));

		//��������
		ASSERT(GetMissionManager()!=NULL);
		GetMissionManager()->SendData(MDM_GP_LOGON,wSubCmdID,cbBuffer,wPacketSize);

		return false;
	}

	m_bKefu = false;
	//��ȡ��ַ
	TCHAR szCustomServer[LEN_SERVER]=TEXT("");
	if (m_pDlgLogon!=NULL) lstrcpyn(szCustomServer,m_pDlgLogon->m_szLogonServer,CountArray(szCustomServer));

	//���õ�ַ
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->SetCustomServer(szCustomServer);


	CPlatformFrame* pPlatformFrame = CPlatformFrame::GetInstance();
	pPlatformFrame->AvtiveMainMissionItem();

	SetTimer(IDI_LOGON_TIME,3*1000,NULL);
	//��������
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{
		//���µ�¼
		if (m_bRegister==false)
		{
			ShowLogon();
		}
		else
		{
			ShowRegister();
		}

		return false;
	}

	//���ñ���
	m_bRemPassword=bRemPassword;

	///EWIN����ɾ��
	//д���ַ
	//CGlobalServer GlobalServer;
	//GlobalServer.SetLastServerName(szCustomServer);

	//��ʾ����
	ShowStatusWindow(TEXT("�������ӷ��������������Ժ�Ƭ��..."));

	return true;
}

//��¼�ɹ�
bool CMissionLogon::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonSuccess));
	if (wDataSize<sizeof(CMD_GP_LogonSuccess)) return false;

	//��Ϣ����
	CMD_GP_LogonSuccess * pLogonSuccess=(CMD_GP_LogonSuccess *)pData;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//������Ϣ
	pGlobalUserData->wFaceID=pLogonSuccess->wFaceID;
	pGlobalUserData->cbGender=pLogonSuccess->cbGender;
	pGlobalUserData->dwUserID=pLogonSuccess->dwUserID;
	pGlobalUserData->dwExperience=pLogonSuccess->dwExperience;
	pGlobalUserData->cbMoorMachine=pLogonSuccess->cbMoorMachine;

	//�û�����
	pGlobalUserData->lScore=pLogonSuccess->lScore;
	pGlobalUserData->lInsure=pLogonSuccess->lInsure;
	pGlobalUserData->lUserMedal=pLogonSuccess->lUserMedal;


	//��ʾ����
	pParameterGlobal->m_bShowServerStatus=(pLogonSuccess->cbShowServerStatus!=0);

	//�ʺ���Ϣ
	lstrcpyn(pGlobalUserData->szAccounts,pLogonSuccess->szAccounts,CountArray(pGlobalUserData->szAccounts));
	lstrcpyn(pGlobalUserData->szNickName,pLogonSuccess->szNickName,CountArray(pGlobalUserData->szNickName));

	//��չ��Ϣ
	lstrcpyn(pGlobalUserData->szAddrDescribe,pLogonSuccess->szAddrDescribe,CountArray(pGlobalUserData->szAddrDescribe));


	theAccount.Scoretype = MoShi_Yuan;
	theAccount.fandian = pLogonSuccess->f_fandian;
 	theAccount.user_id = pLogonSuccess->dwUserID;
	theAccount.cbgender = pLogonSuccess->cbGender;
 	lstrcpyn(theAccount.account,pGlobalUserData->szAccounts,CountArray(theAccount.account));

 	theAccount.yue = pLogonSuccess->f_yue;
	theAccount.dongjie = pLogonSuccess->f_dongjie;
	theAccount.type = pLogonSuccess->n_type;
	theAccount.moormachine = pLogonSuccess->cbMoorMachine;

	//��չ��Ϣ
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pLogonSuccess+1,wDataSize-sizeof(CMD_GP_LogonSuccess));
	while (true)
	{
		//��ȡ����
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		//��ȡ����
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_MEMBER_INFO:	//��Ա��Ϣ
			{
				ASSERT(DataDescribe.wDataSize==sizeof(DTP_GP_MemberInfo));
				if (DataDescribe.wDataSize==sizeof(DTP_GP_MemberInfo))
				{
					DTP_GP_MemberInfo * pMemberInfo=(DTP_GP_MemberInfo *)pDataBuffer;
					pGlobalUserData->cbMemberOrder=pMemberInfo->cbMemberOrder;
					pGlobalUserData->MemberOverDate=pMemberInfo->MemberOverDate;
				}

				break;
			}
		}
	}

	//������ʾ
	ShowStatusWindow(TEXT("���ڻ�ȡ��Ϸ�б�..."));

	return true;
}

//��¼ʧ��
bool CMissionLogon::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GP_LogonFailure * pLogonFailure=(CMD_GP_LogonFailure *)pData;

	//Ч������
	ASSERT(wDataSize>=(sizeof(CMD_GP_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GP_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//�رմ���
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//���Ƚ���
	HideStatusWindow();

	//��ʾ��Ϣ
	LPCTSTR pszDescribeString(pLogonFailure->szDescribeString);
	if (lstrlen(pszDescribeString)>0) ShowInformation(pszDescribeString,MB_ICONERROR,60);

	//���µ�¼
	if (m_bRegister==false)
	{
		ShowLogon();
	}
	else
	{
		ShowRegister();
	}

	return true;
}
//��¼���
bool CMissionLogon::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GP_LogonFinish));
	if (wDataSize!=sizeof(CMD_GP_LogonFinish)) return false;

	//��������
	CMD_GP_LogonFinish * pLogonFinish=(CMD_GP_LogonFinish *)pData;

	//��������
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//ʱ������
	pParameterGlobal->m_wIntermitTime=pLogonFinish->wIntermitTime;
	pParameterGlobal->m_wOnLineCountTime=pLogonFinish->wOnLineCountTime;

	//������Ϣ
	UpdateUserInfo();

	//�رմ���
	HideStatusWindow();

	//ɾ���ؼ�
	SafeDelete(m_pDlgLogon);
	//SafeDelete(m_pDlgRegister);

	//��ֹ����
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL)
	{
		pMissionManager->ConcludeMissionItem(this,false);
		//pMissionManager->DeleteMissionItem(this);
	}
	//�����¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGON,0L);



	return true;
}

//������ʾ
bool CMissionLogon::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GP_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GP_UpdateNotify)) return false;

	//���ñ���
	CMD_GP_UpdateNotify * pUpdateNotify=(CMD_GP_UpdateNotify *)pData;

	//��ֹ����
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//���ش���
	HideStatusWindow();

	//��ʾ��Ϣ
	if ((pUpdateNotify->cbMustUpdate==TRUE)||(pUpdateNotify->cbAdviceUpdate==TRUE))
	{
		//������ʾ
		if (pUpdateNotify->cbMustUpdate==FALSE)
		{
			//������ʾ
			LPCTSTR pszString=TEXT("��Ϸ�����������������ڵİ汾���ܼ���ʹ�ã��������������ظ��£�");

			//�˳��ж�
			CInformation Information(CPlatformFrame::GetInstance());
			if (Information.ShowMessageBox(pszString,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES) return true;
		}
	}

	//���ش���
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("��Ϸ����"),0,0);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
