#include "Stdafx.h"
#include "Resource.h"
#include "GameServerDlg.h"
//////////////////////////////////////////////////////////////////////////////////
#include "Hardwareinfo.h"

//��Ϣ����
#define WM_PROCESS_CMD_LINE			(WM_USER+100)						//��������

//////////////////////////////////////////////////////////////////////////////////

#define	WLOG	OutputDebugString



//���16�����ַ�
CString HexShow(BYTE* sStr, int iLen, int iFlag)
{
	CString strHex;
	if(iLen <= 0)
		return strHex;
	register int iCount;
	for (iCount = 0; iCount < iLen; iCount++)
	{
		if (iCount % 36 == 0) 
			WLOG(L"\n");
		if (iFlag && sStr[iCount] > 0x1f) 
			TRACE(L"%2C ", sStr[iCount]);
		else 
		{
			CString strTemp;
			strTemp.Format(L"%.2X", sStr[iCount]);
			strHex += strTemp;
			TRACE(L"%.2X ", sStr[iCount]);
		}
	}
	if ((iCount - 1) % 36) 
		WLOG(L"\n");
	WLOG(L"\n");
	return strHex;
}

//�򵥼���
void	SimpleDecode(byte* szIn, int nLen)
{
	for(int n=0; n<nLen; n++)
	{
		szIn[n] ^= 0xFF;
		szIn[n] ^= 0xAC;
	}
}

//���ɻ�����
CString	GetMachineCode()
{
	BOOL	bRet = FALSE;
	BYTE	szSystemInfo[1024];
	UINT	uLen=0;
	memset(szSystemInfo, 0x00, sizeof(szSystemInfo));
	bRet = GetMacAddr(szSystemInfo, uLen);
	
	//cpuid
	GetCpuID(szSystemInfo, 12);

#ifdef	_DEBUG
	WLOG(HexShow(szSystemInfo, 10, 0));
#endif
	SimpleDecode(szSystemInfo, 10);
	CString strLocalCode = HexShow(szSystemInfo, 10, 0);
	memset(szSystemInfo, 0x00, sizeof(szSystemInfo));
	bRet = GetHdiskSerial(szSystemInfo, uLen);


#ifdef	_DEBUG
	WLOG(HexShow(szSystemInfo, 32, 0));
#endif	
//	SimpleDecode(szSystemInfo, 32);
	strLocalCode += (char*)szSystemInfo;
	strLocalCode.Replace(L" ", L"");
	strLocalCode.Replace(L"-", L"");
#ifdef _DEBUG
	WLOG(strLocalCode);
#endif
	for(int n=0; n<strLocalCode.GetLength(); n++)
	{
		if(n%6 == 0 && n!= 0)
			strLocalCode.Insert(n, L"-");
	}
#ifdef _DEBUG
	WLOG(L"\n");
	WLOG(strLocalCode);
#endif
	strLocalCode = strLocalCode.Left(48);
	return strLocalCode;
}

//У��License.
//����ֵ��0	�ɹ�
//1 �ļ������ڣ�2 �ļ��ѹ��ڣ�3 �ļ���ʽ����ȷ��
int		checkLicense(CString strInputCode, CString& strExpireDate)
{
	CString strLicenseFile = CINIFile::GetAppPath()+L"License.dat";
	if(!CINIFile::IsFileExist(strLicenseFile))
		return 1;
	CINIFile	iniFile(strLicenseFile);
	strExpireDate = iniFile.GetKeyVal(L"LICENSE", L"EXPRIREDATE",L"20090101");
	CString strLicenseCode= iniFile.GetKeyVal(L"LICENSE", L"LicenseCode",L"0123456789");
	if(strExpireDate.IsEmpty() || strExpireDate.GetLength() < 8)
		return 3;
	CTime tmExpire = CTime(_wtoi(strExpireDate.Left(4)), _wtoi(strExpireDate.Mid(4, 2)), _wtoi(strExpireDate.Right(2)), \
		0, 0, 0);
	CTime tmNow = CTime::GetCurrentTime();

	bool bIsValid = (tmExpire - tmNow) >= 0;
	if( !bIsValid )
		return 2;

	USES_CONVERSION;
	CString strInputLicense = strInputCode.GetBuffer(1);
	strInputLicense.Replace(L"-", L"");
	strInputLicense.Insert(5,strExpireDate.Left(2));
	strInputLicense.Insert(10, strExpireDate.Mid(2, 2));
	strInputLicense.Insert(15, strExpireDate.Mid(4, 2));
	strInputLicense.Insert(20, strExpireDate.Right(2));

	TCHAR szMD5Result[33];
	memset(szMD5Result, 0x00, sizeof(szMD5Result));
	CMD5Encrypt::EncryptData((LPCTSTR)strInputLicense , szMD5Result);
	
	CString strUpperCode = HexShow((BYTE*)W2A(szMD5Result), _tcsclen(szMD5Result), 0);
	strUpperCode.MakeUpper();
	if(strLicenseCode.MakeUpper().Compare(strUpperCode) == 0)
		return 0;
	else
		return 2;
}

bool	CreateLicenseFile(TCHAR* szFileName, TCHAR* szMachineCode, TCHAR* szExpireCode)
{
	CFile licenseFile;
	if(licenseFile.Open(szFileName,CFile::modeReadWrite|CFile::modeCreate))
	{
		CString strLicenseCode = szMachineCode;
		strLicenseCode.Replace(L"-", L"");
		CString strExpireCode = szExpireCode;
		strLicenseCode.Insert(5,  strExpireCode.Left(2));
		strLicenseCode.Insert(10, strExpireCode.Mid(2, 2));
		strLicenseCode.Insert(15, strExpireCode.Mid(4, 2));
		strLicenseCode.Insert(20, strExpireCode.Right(2));

		USES_CONVERSION;
		TCHAR szMD5Result[33];
		memset(szMD5Result, 0x00, sizeof(szMD5Result));
		CMD5Encrypt::EncryptData((LPCTSTR)strLicenseCode, szMD5Result);
		licenseFile.Write("[LICENSE]\n", strlen("[LICENSE]\n"));
		licenseFile.Write("VERSION=2.0\nPROVIDER=Typingsoft\n", strlen("VERSION=2.0\nPROVIDER=Typingsoft\n"));
		licenseFile.Write("EXPRIREDATE=", strlen("EXPRIREDATE="));
		licenseFile.Write(W2A(szExpireCode), strlen(W2A(szExpireCode)));
		licenseFile.Write("\n", strlen("\n"));
		licenseFile.Write("LicenseCode=", strlen("LicenseCode="));

		CString strMd5Code = HexShow((BYTE*)W2A(szMD5Result), _tcsclen(szMD5Result), 0);
		strMd5Code.MakeUpper();
		licenseFile.Write(W2A(strMd5Code.GetBuffer(1)), strMd5Code.GetLength());
		licenseFile.Write("\n", strlen("\n"));
		licenseFile.Close();
	}
	return true;
}

const CString		GetLicenseDesc(int nCode)
{
	if(nCode == 1)
		return TEXT("�ļ������ڣ�������ΪLicense.dat���ļ�����ȷ�ϸ��ļ��ڷ����������·���У�\n������Ϊ��");
	else if(nCode == 2)
		return TEXT("License.dat�ļ��ѹ��ڣ�����ϵ�������ṩ���µ�License��������Ϊ��");
	else if(nCode == 3)
		return TEXT("�ļ���ʽ����ȷ������License.dat�ļ���������Ϊ��");
	else
		return TEXT("LicenseУ��ɹ���");
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



BEGIN_MESSAGE_MAP(CGameServerDlg, CDialog)

	//ϵͳ��Ϣ
	ON_WM_QUERYENDSESSION()
	ON_BN_CLICKED(IDC_OPEN_SERVER, OnBnClickedOpenServer)
	ON_BN_CLICKED(IDC_STOP_SERVICE, OnBnClickedStopService)
	ON_BN_CLICKED(IDC_START_SERVICE, OnBnClickedStartService)
	ON_BN_CLICKED(IDC_CREATE_SERVER, OnBnClickedCreateServer)
	ON_BN_CLICKED(IDC_OPTION_SERVER, OnBnClickedOptionServer)

	//�Զ���Ϣ
	ON_MESSAGE(WM_PROCESS_CMD_LINE,OnMessageProcessCmdLine)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServerDlg::CGameServerDlg() : CDialog(IDD_DLG_GAME_SERVER)
{
	//���ò���
	m_bAutoControl=false;
	m_bOptionSuccess=false;
	ZeroMemory(&m_ModuleInitParameter,sizeof(m_ModuleInitParameter));

	return;
}

//��������
CGameServerDlg::~CGameServerDlg()
{
}

//�ؼ���
VOID CGameServerDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACE_MESSAGE, m_TraceServiceControl);
}

//��ʼ������
BOOL CGameServerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ϸ������ -- [ ֹͣ ]"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//�������
	m_ServiceUnits.SetServiceUnitsSink(this);

	//�����
	LPCTSTR pszCmdLine=AfxGetApp()->m_lpCmdLine;
	if (pszCmdLine[0]!=0) PostMessage(WM_PROCESS_CMD_LINE,0,(LPARAM)pszCmdLine);

	return TRUE;
}

//ȷ����Ϣ
VOID CGameServerDlg::OnOK()
{
	return;
}

//ȡ������
VOID CGameServerDlg::OnCancel()
{
	//�ر�ѯ��
	if (m_ServiceUnits.GetServiceStatus()!=ServiceStatus_Stop)
	{
		LPCTSTR pszQuestion=TEXT("��Ϸ���������������У���ȷʵҪ�رշ�������");
		if (AfxMessageBox(pszQuestion,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION)!=IDYES) return;
	}

	//ֹͣ����
	m_ServiceUnits.ConcludeService();

	__super::OnCancel();
}

//��Ϣ����
BOOL CGameServerDlg::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//����״̬
VOID CGameServerDlg::OnServiceUnitsStatus(enServiceStatus ServiceStatus)
{
	//״̬����
	switch (ServiceStatus)
	{
	case ServiceStatus_Stop:	//ֹͣ״̬
		{
			//���±���
			UpdateServerTitle(ServiceStatus);

			//����ť
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(FALSE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);

			//���ð�ť
			GetDlgItem(IDC_OPEN_MATCH)->EnableWindow(TRUE);
			GetDlgItem(IDC_OPEN_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_CREATE_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_OPTION_SERVER)->EnableWindow(TRUE);

			//���а�ť
			GetDlgItem(IDC_RUN_PARAMETER)->EnableWindow(FALSE);
			GetDlgItem(IDC_SERVICE_CONTROL)->EnableWindow(FALSE);

			//��ʾ��Ϣ
			LPCTSTR pszDescribe=TEXT("����ֹͣ�ɹ�");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	case ServiceStatus_Config:	//����״̬
		{
			//���±���
			UpdateServerTitle(ServiceStatus);

			//���ð�ť
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);

			//���ð�ť
			GetDlgItem(IDC_OPEN_MATCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPEN_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_CREATE_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPTION_SERVER)->EnableWindow(FALSE);

			//���а�ť
			GetDlgItem(IDC_RUN_PARAMETER)->EnableWindow(FALSE);
			GetDlgItem(IDC_SERVICE_CONTROL)->EnableWindow(FALSE);

			//��ʾ��Ϣ
			LPCTSTR pszDescribe=TEXT("���ڳ�ʼ�����...");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	case ServiceStatus_Service:	//����״̬
		{
			//���±���
			UpdateServerTitle(ServiceStatus);

			//����ť
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);

			//���ð�ť
			GetDlgItem(IDC_OPEN_MATCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPEN_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_CREATE_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPTION_SERVER)->EnableWindow(FALSE);

			//���а�ť
			GetDlgItem(IDC_RUN_PARAMETER)->EnableWindow(TRUE);
			GetDlgItem(IDC_SERVICE_CONTROL)->EnableWindow(TRUE);

			//��ʾ��Ϣ
			LPCTSTR pszDescribe=TEXT("���������ɹ�");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	}

	return;
}

//����ͼ��
VOID CGameServerDlg::UpdateServerLogo(LPCTSTR pszServerDLL)
{
	//������Դ
	HINSTANCE hInstance=AfxLoadLibrary(pszServerDLL);

	//����ͼ��
	if (hInstance!=NULL)
	{
		//������Դ
		AfxSetResourceHandle(hInstance);

		//������Դ
		CStatic * pServerLogo=(CStatic *)GetDlgItem(IDC_SERVER_LOGO);
		pServerLogo->SetIcon(::LoadIcon(hInstance,TEXT("SERVER_ICON")));

		//�ͷ���Դ
		AfxFreeLibrary(hInstance);
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}

	return;
}

//���±���
VOID CGameServerDlg::UpdateServerTitle(enServiceStatus ServiceStatus)
{
	//��������
	LPCTSTR pszStatusName=NULL;
	LPCTSTR pszServerName=NULL;

	//״̬�ַ�
	switch (ServiceStatus)
	{
	case ServiceStatus_Stop:	//ֹͣ״̬
		{
			pszStatusName=TEXT("ֹͣ");
			break;
		}
	case ServiceStatus_Config:	//����״̬
		{
			pszStatusName=TEXT("��ʼ��");
			break;
		}
	case ServiceStatus_Service:	//����״̬
		{
			pszStatusName=TEXT("����");
			break;
		}
	}

	//��������
	if (m_bOptionSuccess==false) pszServerName=TEXT("��Ϸ������");
	else pszServerName=m_ModuleInitParameter.GameServiceOption.szServerName;

	//�������
	TCHAR szTitle[128]=TEXT("");
	_sntprintf(szTitle,CountArray(szTitle),TEXT("[ %s ] -- [ %s ]"),pszServerName,pszStatusName);

	//���ñ���
	SetWindowText(szTitle);

	return;
}

//����״̬
VOID CGameServerDlg::UpdateParameterStatus(tagModuleInitParameter & ModuleInitParameter)
{
	//���License
	CString strMcode = GetMachineCode();
	CString strExpireDate;
	int nCode = checkLicense(strMcode, strExpireDate);
	if ( nCode != 0)
	{
		CTraceService::TraceString(GetLicenseDesc(nCode), TraceLevel_Warning);
		CString strMCode = GetMachineCode();
		CTraceService::TraceString(strMCode, TraceLevel_Warning);
		return;
	}

	//���ñ���
	m_bOptionSuccess=true;
	m_ModuleInitParameter=ModuleInitParameter;

	//���±���
	UpdateServerTitle(ServiceStatus_Stop);
	UpdateServerLogo(ModuleInitParameter.GameServiceAttrib.szServerDLLName);

	//���ð�ť
	GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPTION_SERVER)->EnableWindow(TRUE);

	//���ÿؼ�
	SetDlgItemText(IDC_GAME_NAME,m_ModuleInitParameter.GameServiceAttrib.szGameName);
	SetDlgItemText(IDC_SERVER_NAME,m_ModuleInitParameter.GameServiceOption.szServerName);

	//�����˿�
	if (m_ModuleInitParameter.GameServiceOption.wServerPort==0)
	{
		SetDlgItemText(IDC_SERVER_PORT,TEXT("�Զ�����"));
	}
	else
	{
		SetDlgItemInt(IDC_SERVER_PORT,m_ModuleInitParameter.GameServiceOption.wServerPort);
	}

	//����ģ��
	LPCTSTR pszServerDLLName=m_ModuleInitParameter.GameServiceAttrib.szServerDLLName;
	m_ServiceUnits.CollocateService(pszServerDLLName,m_ModuleInitParameter.GameServiceOption);

	//������ʾ
	TCHAR szString[256]=TEXT("");
	LPCTSTR pszServerName=m_ModuleInitParameter.GameServiceOption.szServerName;
	_sntprintf(szString,CountArray(szString),TEXT("[ %s ] ����������سɹ�"),pszServerName);

	//�����Ϣ
	CTraceService::TraceString(szString,TraceLevel_Normal);

	return;
}

//��������
bool CGameServerDlg::StartServerService(WORD wServerID)
{
	//��������
	tagDataBaseParameter DataBaseParameter;
	ZeroMemory(&DataBaseParameter,sizeof(DataBaseParameter));

	//���ò���
	InitDataBaseParameter(DataBaseParameter);
	m_ModuleDBParameter.SetPlatformDBParameter(DataBaseParameter);

		//���License
	CString strMcode = GetMachineCode();
	CString strExpireDate;
	int nCode = checkLicense(strMcode, strExpireDate);
	if ( nCode != 0)
	{
		CTraceService::TraceString(GetLicenseDesc(nCode), TraceLevel_Warning);
		CString strMCode = GetMachineCode();
		CTraceService::TraceString(strMCode, TraceLevel_Warning);
		return false;
	}


	//��ȡ����
	CDlgServerItem DlgServerItem;
	if (DlgServerItem.OpenGameServer(wServerID)==false)
	{
		CTraceService::TraceString(TEXT("�������ò��������ڻ��߼���ʧ��"),TraceLevel_Exception);
		return false;
	}

	//����״̬
	UpdateParameterStatus(DlgServerItem.m_ModuleInitParameter);

	//��������
	try
	{
		m_ServiceUnits.StartService();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return true;
}

//��ȡ����
bool CGameServerDlg::InitDataBaseParameter(tagDataBaseParameter & DataBaseParameter)
{
	//��ȡ·��
	TCHAR szWorkDir[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));

	//����·��
	TCHAR szIniFile[MAX_PATH]=TEXT("");
	_sntprintf(szIniFile,CountArray(szIniFile),TEXT("%s\\ServerParameter.ini"),szWorkDir);

	//��ȡ����
	CWHIniData IniData;
	IniData.SetIniFilePath(szIniFile);
	//���License
	CString strMcode = GetMachineCode();
	CString strExpireDate;
	int nCode = checkLicense(strMcode, strExpireDate);
	if ( nCode != 0)
	{
		CTraceService::TraceString(GetLicenseDesc(nCode), TraceLevel_Warning);
		CString strMCode = GetMachineCode();
		CTraceService::TraceString(strMCode, TraceLevel_Warning);
		return false;
	}

	//������Ϣ
	DataBaseParameter.wDataBasePort=(WORD)IniData.ReadInt(TEXT("PlatformDB"),TEXT("DBPort"),1433);
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBAddr"),NULL,DataBaseParameter.szDataBaseAddr,CountArray(DataBaseParameter.szDataBaseAddr));
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBUser"),NULL,DataBaseParameter.szDataBaseUser,CountArray(DataBaseParameter.szDataBaseUser));
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBPass"),NULL,DataBaseParameter.szDataBasePass,CountArray(DataBaseParameter.szDataBasePass));
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBName"),szPlatformDB,DataBaseParameter.szDataBaseName,CountArray(DataBaseParameter.szDataBaseName));

	return true;
}

//��������
VOID CGameServerDlg::OnBnClickedStartService()
{
	//��������
	try
	{
		m_ServiceUnits.StartService();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//ֹͣ����
VOID CGameServerDlg::OnBnClickedStopService()
{
	//ֹͣ����
	try
	{
		m_ServiceUnits.ConcludeService();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//�򿪷���
VOID CGameServerDlg::OnBnClickedOpenServer()
{
	//��������
	tagDataBaseParameter DataBaseParameter;
	ZeroMemory(&DataBaseParameter,sizeof(DataBaseParameter));

	//���ò���
	InitDataBaseParameter(DataBaseParameter);
	m_ModuleDBParameter.SetPlatformDBParameter(DataBaseParameter);
	
	//���License
	CString strMcode = GetMachineCode();
	CString strExpireDate;
	int nCode = checkLicense(strMcode, strExpireDate);
	if ( nCode != 0)
	{
		CTraceService::TraceString(GetLicenseDesc(nCode), TraceLevel_Warning);
		CString strMCode = GetMachineCode();
		CTraceService::TraceString(strMCode, TraceLevel_Warning);
		return;
	}

	//���÷���
	CDlgServerItem DlgServerItem;
	if (DlgServerItem.OpenGameServer()==false) return;

	//����״̬
	UpdateParameterStatus(DlgServerItem.m_ModuleInitParameter);

	return;
}

//��������
VOID CGameServerDlg::OnBnClickedCreateServer()
{
	//��������
	tagDataBaseParameter DataBaseParameter;
	ZeroMemory(&DataBaseParameter,sizeof(DataBaseParameter));

	//���ò���
	InitDataBaseParameter(DataBaseParameter);
	m_ModuleDBParameter.SetPlatformDBParameter(DataBaseParameter);

	//��������
	CDlgServerWizard DlgServerWizard;
	if (DlgServerWizard.CreateGameServer()==false) return;

	//����״̬
	UpdateParameterStatus(DlgServerWizard.m_ModuleInitParameter);

	return;
}

//���÷���
VOID CGameServerDlg::OnBnClickedOptionServer()
{
	//��Ϸģ��
	CGameServiceManagerHelper GameServiceManager;
	GameServiceManager.SetModuleCreateInfo(m_ModuleInitParameter.GameServiceAttrib.szServerDLLName,GAME_SERVICE_CREATE_NAME);

	//����ģ��
	if (GameServiceManager.CreateInstance()==false)
	{
		AfxMessageBox(TEXT("������������ڻ��߼���ʧ�ܣ����÷���ʧ��"),MB_ICONERROR);
		return;
	}

	//���÷���
	CDlgServerWizard DlgServerWizard;
	DlgServerWizard.SetWizardParameter(GameServiceManager.GetInterface(),&m_ModuleInitParameter.GameServiceOption);

	//��������
	if (DlgServerWizard.CreateGameServer()==false) return;

	//����״̬
	UpdateParameterStatus(DlgServerWizard.m_ModuleInitParameter);

	return;
}

//�ر�ѯ��
BOOL CGameServerDlg::OnQueryEndSession()
{
	//��ʾ��Ϣ
	if (m_ServiceUnits.GetServiceStatus()!=ServiceStatus_Stop)
	{
		CTraceService::TraceString(TEXT("�������������У�ϵͳҪ��ע���ػ�����ʧ��"),TraceLevel_Warning);
		return FALSE;
	}

	return TRUE;
}

//�����
LRESULT CGameServerDlg::OnMessageProcessCmdLine(WPARAM wParam, LPARAM lParam)
{
	//��������
	CWHCommandLine CommandLine;
	LPCTSTR pszCommandLine=(LPCTSTR)(lParam);

	//�����ʶ
	TCHAR szSrverID[32]=TEXT("");
	if (CommandLine.SearchCommandItem(pszCommandLine,TEXT("/ServerID:"),szSrverID,CountArray(szSrverID))==true)
	{
		//��ȡ����
		WORD wServerID=(WORD)(_tstol(szSrverID));

		//��������
		if (wServerID!=0)
		{
			//���ñ���
			m_bAutoControl=true;

			//��������
			StartServerService(wServerID);
		}
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
