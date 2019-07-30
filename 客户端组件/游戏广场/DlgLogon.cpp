#include "Stdafx.h"
#include "DlgLogon.h"
#include "GamePlaza.h"
#include "MissionLogon.h"
#include ".\dlglogon.h"
#include "Ping.h"
#include "MD5Checksum\MD5Checksum.h"
#import <msxml3.dll>
using namespace MSXML2;

#define IDC_LOGON_LOGO 1112615
//////////////////////////////////////////////////////////////////////////////////
int CDlgLogon::m_nLoadServer = 0;
//Բ�Ǵ�С
#define ROUND_CX					7									//Բ�ǿ��
#define ROUND_CY					7									//Բ�Ǹ߶�

//��Ļλ��
#define LAYERED_SIZE				5									//�ֲ��С
#define CAPTION_SIZE				135									//�����С

//��ť��Ŀ
#define BUTTON_COUNT_CX				5									//������Ŀ
#define BUTTON_COUNT_CY				4									//������Ŀ
#define BUTTON_ITEM_UNDER			0									//��ť�ײ�
#define BUTTON_ITEM_START			8									//��ť��ʼ
#define BUTTON_ITEM_SPACE			30									//��ť���

//�ؼ���ʶ
#define IDC_WEB_PUBLICIZE			300									//����ؼ�
#define IDC_WND_LOGON_BUTTON		301									//��ť�ؼ�

//��ť��ʶ
#define INDEX_REGISTER				0									//ע�ᰴť
#define INDEX_USER_HELP				1									//���ְ���
#define INDEX_BUY_SCORE				2									//������ֵ
#define INDEX_HOME_PAGE				3									//�ٷ���ҳ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgLogon, CDialog)

	//ϵͳ��Ϣ
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_PAINT()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_REGISTER, OnBnClickedRegister)
	//ON_CBN_EDITCHANGE(IDC_ACCOUNTS, OnCbnEditchangeAccounts)
	ON_BN_CLICKED(IDC_FORGET_PASS, OnBnClickedForget)
	ON_BN_CLICKED(IDC_KEFU, OnBnClickedKeFu)
	ON_BN_CLICKED(IDC_RUBBISH, OnBnClickedDelete)
	ON_CBN_SELCHANGE(IDC_ACCOUNTS, OnSelchangeAccounts)
	ON_CBN_SELCHANGE(IDC_LOGON_SERVER, OnSelchangeServer)

	//ON_EN_CHANGE(IDC_ACCOUNTS, OnEnChangeAccounts)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// BEGIN_MESSAGE_MAP(CWndLogonButton, CWnd)
// 	ON_WM_PAINT()
// 	ON_WM_CREATE()
// 	ON_WM_SETCURSOR()
// 	ON_WM_MOUSEMOVE()
// 	ON_WM_LBUTTONUP()
// 	ON_WM_LBUTTONDOWN()
// 	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
// END_MESSAGE_MAP()
// 
//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////

//���캯��
CControlCheckButton::CControlCheckButton()
{
	//���ñ���
	m_cbChecked=FALSE;
	m_pParentSink=NULL;

	//������Դ
	//m_ImageBtnBack.LoadFromResource(AfxGetResourceHandle(),IDB_CHECK_BUTTON_BACK);
	//m_ImageBtnBack.LoadImage(AfxGetResourceHandle(),TEXT("BT_CHECK_BUTTON_BACK"));
}

//��������
CControlCheckButton::~CControlCheckButton()
{
}

//�ؼ�����
CRect CControlCheckButton::GetControlRect()
{
	CRect rcClient(m_ptControlBenchmark.x,m_ptControlBenchmark.y,m_ptControlBenchmark.x+m_ImageBtnBack.GetWidth()/2,m_ptControlBenchmark.y+m_ImageBtnBack.GetHeight());

	return rcClient;
}

//����״̬
VOID  CControlCheckButton::SetButtonChecked(BYTE cbChecked) 
{ 
	//���ñ���
	m_cbChecked=cbChecked;

	//�������
	CRect rcClient = GetControlRect();

	//�ػ����
	if(m_pParentSink!=NULL) m_pParentSink->InvalidateRect(&rcClient);
};
VOID  CControlCheckButton::LoadCheckImage(UINT nIDResource) 
{
	//m_ImageBtnBack.LoadImage(AfxGetResourceHandle(),nIDResource);
}
VOID  CControlCheckButton::LoadCheckImage(LPCTSTR pszResourceName) 
{
	m_ImageBtnBack.LoadImage(AfxGetResourceHandle(),pszResourceName);
}
//���ƿؼ�
VOID  CControlCheckButton::OnDrawControl(CDC * pDC)
{
	//����У��
	if(pDC==NULL) return;

	//�������
	INT nImageWidth = m_ImageBtnBack.GetWidth()/2;
	INT nImageHeight = m_ImageBtnBack.GetHeight();
    
    //���Ʊ���
	//m_ImageBtnBack.BitBlt(*pDC,m_ptControlBenchmark.x,m_ptControlBenchmark.y,nImageWidth,nImageHeight,m_cbChecked==TRUE?nImageWidth:0,0);
	m_ImageBtnBack.DrawImage(pDC,m_ptControlBenchmark.x,m_ptControlBenchmark.y,nImageWidth,nImageHeight,m_cbChecked==TRUE?nImageWidth:0,0);
}

//����¼�
VOID  CControlCheckButton::OnClickControl(CPoint Point)
{
	//�������
	CRect rcClient = GetControlRect();

// 	CString strLog;
// 	strLog.Format(L"x = %d,y = %d,cx = %d,cy = %d,px = %d,py = %d",rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height(),Point.x,Point.y);
// 	AfxMessageBox(strLog);
	//ѡ���ж�
	if(rcClient.PtInRect(Point))
	{
		//���ñ���
		m_cbChecked=m_cbChecked==FALSE?TRUE:FALSE;

		//�ػ����
		if(m_pParentSink!=NULL)	m_pParentSink->InvalidateRect(&rcClient);
	}
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgLogon::CDlgLogon() : CDialog(IDD_DLG_LOGON)
{
	//��¼��Ϣ
	m_szAccounts[0]=0;
	m_szPassword[0]=0;
	m_szLogonServer[0]=0;
	m_bCreatePlazaview = true;
	m_nLoadServer = 0;
	//������ˢ
	m_brBrush.CreateSolidBrush(RGB(215,223,228));

	return;
}

//��������
CDlgLogon::~CDlgLogon()
{
	m_Destroyed=true;		////	�Ѿ����ٵı�ʶ

	//������Դ
	if (m_brBrush.GetSafeHandle()!=NULL)
	{
		m_brBrush.DeleteObject();
	}
	//ɾ������
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		SafeDelete(m_AccountsInfoArray[i]);
	}

	return;
}

//�ؼ���
VOID CDlgLogon::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//�󶨿ؼ�
	DDX_Control(pDX, IDOK, m_btEnter);
	DDX_Control(pDX, IDCANCEL, m_btQuit);

	//�󶨿ؼ�
//	DDX_Control(pDX, IDC_ACCOUNTS, m_edAccounts);
	DDX_Control(pDX, IDC_ACCOUNTS, m_AccountsControl);

	DDX_Control(pDX, IDC_PASSWORD, m_PasswordControl);
	DDX_Control(pDX, IDC_LOGON_SERVER, m_ServerControl);
}
VOID CDlgLogon::OnSelchangeServer()
{
	if(m_ServerControl.GetSafeHwnd())
		m_nLoadServer = m_ServerControl.GetCurSel();
	else
		return;


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

	TCHAR szTodayURL[124] = L"";
	TCHAR szKeyBoard[124]=L"";
	_sntprintf(szKeyBoard,CountArray(szKeyBoard),_T("LogonServer%d"),m_nLoadServer);
	IniData.ReadEncryptString(TEXT("PlazaUrl"),szKeyBoard,TEXT(""),szTodayURL,CountArray(szTodayURL));
	CString strLog;

	strLog.Format(L"PINGSECOND  CurSel: %s ",szTodayURL);
	OutputDebugString(strLog);

	_sntprintf(m_szLogonServer,CountArray(m_szLogonServer),_T("%s"),szTodayURL);
}

//ѡ��ı�
VOID CDlgLogon::OnSelchangeAccounts()
{
	//��ȡѡ��
	INT nCurrentSel=m_AccountsControl.GetCurSel();
	tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)m_AccountsControl.GetItemDataPtr(nCurrentSel);

	//���ÿؼ�
	if ((nCurrentSel!=LB_ERR)&&(pAccountsInfo!=NULL))
	{
		//�û��ʺ�
		m_AccountsControl.SetCurSel(nCurrentSel);


		//��������
		m_PasswordControl.SetUserPassword(pAccountsInfo->szPassword);

		//��ס����
		bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
		//((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);
		m_RemPwdControl.SetButtonChecked((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);
	}
	else
	{
		//��������
		m_PasswordControl.SetUserPassword(TEXT(""));

		//��ס����
		//((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_UNCHECKED);
		m_RemPwdControl.SetButtonChecked(BST_UNCHECKED);
	}

	return;
}

//��������
BOOL CDlgLogon::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�û���¼"));	

	m_Destroyed=false;

//ע����¼һ�µ�ǰSERVER
// 	CWHRegKey RegServerAddr;
// 	if(RegServerAddr.OpenRegKey(TEXT("CurrentLogonServer"), false))
// 	{
// 		TCHAR szLogonServer[255];
// 		memset(szLogonServer, 0, sizeof(szLogonServer));
// 		RegServerAddr.GetString(TEXT("CurrentIpAddress"), szLogonServer, CountArray(szLogonServer));
// 		if(_tcslen(szLogonServer) != 0)
// 		{
// 			m_ServerControl.AddString(szLogonServer);
// 		}
// 	}

	m_Font.CreateFont(20,0,0,0,0,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("Arial"));
	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_LOGON_BACK"));

	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	CRect rcBtn(0,0,0,0);
	m_btForgetPass.Create(NULL,WS_CHILD|WS_VISIBLE,rcBtn,this,IDC_FORGET_PASS);
//	m_btKeFu.Create(NULL,WS_CHILD|WS_VISIBLE,rcBtn,this,IDC_KEFU);
	m_btDeleteRecord.Create(NULL,WS_CHILD|WS_VISIBLE,rcBtn,this,IDC_RUBBISH);
	//�߿�����
	m_ServerControl.SetDrawBorad(false);
	m_ServerControl.SetRenderImage(false);
	m_PasswordControl.SetDrawBorad(true);
	m_AccountsControl.SetDrawBorad(false);
	m_AccountsControl.SetRenderImage(false);

	m_PasswordControl.SetEnableColor(RGB(254,254,254),RGB(41,51,97),RGB(41,51,97));
	if(m_edAccounts.GetSafeHwnd())
	{
		m_edAccounts.SetWindowPos(NULL,320,100,124,20,SWP_NOZORDER|SWP_NOREDRAW);
		m_edAccounts.SetFont(&m_Font);

		m_edAccounts.SetEnableColor(RGB(0,0,0),RGB(255,255,0),RGB(255,255,255));
		m_edAccounts.ShowWindow(SW_HIDE);
	}

	m_PasswordControl.SetFont(&m_Font);
	m_AccountsControl.SetBackColor(RGB(41,51,97));
	m_AccountsControl.SetFont(&m_Font);
	m_ServerControl.SetBackColor(RGB(41,51,97));
	m_ServerControl.SetFont(&m_Font);
	
	
	//m_ServerControl:��¼��ַ��IP
	if(m_ServerControl.GetSafeHwnd())
		m_ServerControl.SetWindowPos(NULL, 88+20,122,180,28 +200,SWP_NOZORDER|SWP_NOREDRAW);
	//��¼�˺�
	if(m_AccountsControl.GetSafeHwnd())
		m_AccountsControl.SetWindowPos(NULL, 88+16,168,180,28 + 200,SWP_NOZORDER|SWP_NOREDRAW);

	//��¼�����
	if(m_PasswordControl.GetSafeHwnd())
		m_PasswordControl.SetWindowPos(NULL,88,212,188,28,SWP_NOZORDER|SWP_NOREDRAW);


	TCHAR szPlatformUrl1[124] = L"";

	//��ȡ�����ļ�
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	for(int i = 0;i < 10;i++)
	{
		TCHAR szKey[33] = TEXT("");

		_sntprintf(szKey,CountArray(szKey),TEXT("ServerDesc%d"),i);
		TCHAR szDesc[64] ;
		GetPrivateProfileString(TEXT("PlazaUrlDesc"),szKey,TEXT(""),szDesc,CountArray(szDesc),szFileName);
		if(szDesc[0] == NULL)
			break;
		m_ServerControl.AddString(szDesc);

	}

	TCHAR szPlatformUrl[124] = L"";
	//TCHAR szPlatformUrl1[124] = L"";


	//��ȡ����
	CWHIniData IniData;
	IniData.SetIniFilePath(szFileName);

	// 			CTime time = CTime::GetCurrentTime();
	TCHAR szTodayURL[124] = L"";
	int nUrlID = 0;//GetPrivateProfileInt(time.Format(L"%Y-%m-%d"),TEXT("TodayURLID"),0,szFileName);
	// 			int nUrlCanUse = GetPrivateProfileInt(time.Format(L"%Y-%m-%d"),TEXT("TodayCanUse"),0,szFileName);
	int nCur = 0;
	srand((DWORD)time(NULL));
	nCur = rand()%5;
	m_nLoadServer = nCur;
	m_ServerControl.SetCurSel(nCur);

	TCHAR szKeyBoard[124]=L"";
	_sntprintf(szKeyBoard,CountArray(szKeyBoard),_T("LogonServer%d"),nCur);
	IniData.ReadEncryptString(TEXT("PlazaUrl"),szKeyBoard,TEXT(""),szTodayURL,CountArray(szTodayURL));
	_sntprintf(m_szLogonServer,CountArray(m_szLogonServer),_T("%s"),szTodayURL);

	//���ùرհ�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btQuit.SetButtonImage(IDB_BT_LOGON_QUIT,hInstance,true,false);
	m_btQuit.GetClientRect(rcBtn);
	m_btQuit.SetWindowPos(NULL,500,11,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);

	//��¼��ť
	m_btEnter.SetButtonImage(IDB_BT_LOGON,hInstance,false,false);
	m_btEnter.SetWindowPos(NULL,59,306,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);

	//��ס����
	m_RemPwdControl.LoadCheckImage(TEXT("BT_CHECK_BUTTON_BACK"));
	m_RemPwdControl.SetControlBenchmark(20,257);
	m_RemPwdControl.SetParentWndSink(this);

	//��������
	m_btForgetPass.SetButtonImage(IDB_BTN_FORGET_PASS,hInstance,true,false);
	m_btForgetPass.GetClientRect(rcBtn);
	m_btForgetPass.SetWindowPos(NULL,265,257,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);


	//�ͷ���ť
//	m_btKeFu.SetButtonImage(IDB_BTN_KEFU,hInstance,true,false);
//	m_btKeFu.GetClientRect(rcBtn);
//	m_btKeFu.SetWindowPos(NULL,444,193,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);

	//ɾ���˻���ʷ��¼
	m_btDeleteRecord.SetButtonImage(IDB_BT_RUBBISH,hInstance,true,false);
	m_btDeleteRecord.GetClientRect(rcBtn);
	m_btDeleteRecord.SetWindowPos(NULL,300,170,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);
	//CRect rcCreate(0,0,0,0);

	//Loadweb();
	//�����ؼ�
	//m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//EWIN����Ƽ�
	//�����ؼ�
// 	CRect rcWndButton(15,304,440,300);
// 	m_WndLogonButton.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcWndButton,this,IDC_WND_LOGON_BUTTON);


	//��������
	//CGlobalUnits * pGlobalUnits = (CGlobalUnits *)CGlobalUnits::GetInstance();
	//ASSERT(pGlobalUnits!=NULL);


	//����Ŀ¼
	//TCHAR szDirectory[MAX_PATH]=TEXT("");
	//CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));


	//������Ϣ
	LoadAccountsInfo();
	//LoadLogonServerInfo();
	//���ý���
	if (m_AccountsControl.GetCurSel()!=LB_ERR)
	{
		//��Ϣ����
		OnSelchangeAccounts();

		//���ý���
		m_PasswordControl.SetFocus();
	}
	else
	{
		//���ý���
		m_AccountsControl.SetFocus();
	}

	//�ͷ���ͼ
	if(m_png_Kefu.IsNull())
	{
		bool bLoaded = m_png_Kefu.LoadImage(AfxGetInstanceHandle(),TEXT("IDB_PNG_BG_KEFU"));
	}

	//���д���
	CenterWindow(this);

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//��������
	CRgn RgnWindow;
	//RgnWindow.CreateRoundRectRgn(20,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+30,SizeWindow.cy-LAYERED_SIZE+50,ROUND_CX,ROUND_CY);
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);
	CRect rcCreate(0,0,0,0);
//	m_logo.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_LOGON_LOGO);

//	m_logo.SetWindowPos(NULL,5,3,149,187,SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);
	m_DlgStatus.SetStatusViewSink(this);
	return FALSE;
}
void CDlgLogon::Loadweb()
{

	CString strPath;  //�ļ�·��
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	int pos = strPath.ReverseFind('\\'); 
	strPath = strPath.Left(pos); 

//	m_logo.Navigate(strPath +_T("\\logon.html"));
//	m_logo.EnableWindow(FALSE);
}

//��Ϣ����
BOOL CDlgLogon::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//ȷ������
VOID CDlgLogon::OnOK()
{
	//��ȡ��Ϣ
	if (GetInformation()==false) return;

	//���ش���
	ShowWindow(SW_HIDE);
// 
// 	CString strSiteURL;
// 	//����Ŀ¼
// 	TCHAR szDirectory[MAX_PATH]=TEXT("");
// 	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));
// 	m_strCurrentDir = szDirectory;
// 	m_strCurrentDir += L"\\";
// 
// 	//����·��
// 	TCHAR szFileName[MAX_PATH]=TEXT("");
// 	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Update\\update.ini"),szDirectory);
// 	//�����ַ
// 	TCHAR szNavigation[256]=TEXT("");
// 
// 	GetPrivateProfileString(TEXT("SERVER"),TEXT("SERVER1"),TEXT(""),szNavigation,CountArray(szNavigation),szFileName);
// 	strSiteURL = szNavigation;
// 	m_strSiteUrl = szNavigation;
// 	strSiteURL+=_T("/update/update.xml");
// 	m_strSiteUrl+=_T("\\");
// 	//EWIN���� �Ƽ�
// 	if(lstrcmp(AfxGetApp()->m_lpCmdLine,TEXT("UPDATED"))!=0 && CheckVersionUpdate(strSiteURL))
// 	{
// 		//��������
// 		CHAR szModuleName[MAX_PATH]={0};
// 		GetModuleFileNameA(AfxGetInstanceHandle(), szModuleName, MAX_PATH);
// 		PathRemoveFileSpecA(szModuleName);
// 		strcat(szModuleName, "\\Update.exe");
// 		WinExec(szModuleName, SW_SHOWDEFAULT);
// 
// 
// 		return ;
// 
// 	}

	//ִ�е�¼
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_RemPwdControl.GetButtonChecked()==TRUE);
	return;
}
BOOL CDlgLogon::CheckVersionUpdate(CString& strUpdateURL)
{
	BOOL bHasUpdate = FALSE;

	HRESULT hr = S_OK;
	IXMLHTTPRequestPtr pHttpRequest;
	IDispatchPtr pDispatch;
	MSXML2::IXMLDOMDocumentPtr pXmlDoc;
	MSXML2::IXMLDOMNodeListPtr pList;
	MSXML2::IXMLDOMElementPtr pChild;

	UINT nFileSize;
	CString strFileName, strFileVer, strMD5String,strFilePath;
	LONG lElementCount = 0L;

	try
	{
		hr = pHttpRequest.CreateInstance(TEXT("Msxml2.XMLHTTP.3.0"));
		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = pHttpRequest->open(TEXT("GET"), (_bstr_t)strUpdateURL, false);
		if( FAILED(hr) )
		{
			CString strLog;
			strLog.Format(_T("\nUPDATE Get %s  failed"),strUpdateURL);
			OutputDebugString(strLog);

			_com_issue_error(hr);
		}

		hr = pHttpRequest->send();
		if( FAILED(hr) )
		{
			CString strLog;
			strLog.Format(_T("\nUPDATE Send %s  failed"),strUpdateURL);
			OutputDebugString(strLog);


			_com_issue_error(hr);
		}

		if (pHttpRequest->Getstatus() != 200)
		{
			CString strLog;
			strLog.Format(_T("\nUPDATE Send %s  != 200"),strUpdateURL);
			OutputDebugString(strLog);

			throw (0);
		}

		pDispatch = pHttpRequest->GetresponseXML();
		hr = pDispatch->QueryInterface(pXmlDoc.GetIID(), (void**)&pXmlDoc);
		if( FAILED(hr) )
		{
			CString strLog;
			strLog.Format(_T("\nUPDATE _com_issue_error  error"));
			OutputDebugString(strLog);

			_com_issue_error(hr);
		}

		pList = pXmlDoc->selectNodes("/manifest/filelist/file");
		lElementCount = pList->Getlength();
		CString strLog;
		strLog.Format(_T("\nUPDATE lElementCount :%d"),lElementCount);
		OutputDebugString(strLog);


		for( LONG i = 0; i < lElementCount; i++ )
		{
			pChild = pList->Getitem(i);
			 strFilePath = pChild->getAttribute("filepath");
			strFileName = pChild->getAttribute("filename");
			nFileSize = pChild->getAttribute("filesize");
			strFileVer = pChild->getAttribute("fileversion");
			strMD5String = pChild->getAttribute("md5");
			//��������ļ�������Ч���ļ��������ھ���������ļ�
			if(strFilePath!=""){
				strFilePath=strFilePath.TrimLeft(_T("\\"));
				strFilePath+=_T("\\");
			}
			CString strRemoteFile=m_strSiteUrl+strFilePath+strFileName;
			CString strPathFileName=m_strCurrentDir+strFilePath+strFileName;
			BOOL iRet=PathFileExists(strPathFileName);
			CString strLog;
			strLog.Format(_T("\nUPDATE %s %s,%s"),(iRet?_T("�����ļ�"):_T("�������ļ�")),strPathFileName,strRemoteFile);
			OutputDebugString(strLog);
			if(iRet)
			{
				//Ч���ļ�,���MD5�벻��ͬ����������
				CString strLocalMD5String =CMD5Checksum::GetMD5(strPathFileName);
				CString strLog;
				strLog.Format(_T("\nUPDATE  ����MD5:%s Զ��MD5:%s %s"),strLocalMD5String,strMD5String,(strLocalMD5String==strMD5String?_T("��ͬ"):_T("����ͬ")));
				OutputDebugString(strLog);
				if ( strLocalMD5String!= strMD5String)
				{
					bHasUpdate = TRUE;
				}	
			}
			else
			{
				bHasUpdate = TRUE;
			}
		}

		return bHasUpdate;
	}
#ifdef _DEBUG
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Error code:%d\nMessage:%s\nDescrption:%s"), (int)e.WCode(), e.ErrorMessage(), (TCHAR*)e.Description());
		MessageBox(strError, _T("��ʾ"), MB_OK|MB_ICONWARNING);
		return FALSE;
	}
#endif
	catch(...)
	{
		return FALSE;
	}
}


//ȡ����Ϣ
VOID CDlgLogon::OnCancel()
{
	m_Destroyed=true;
	//�رմ���2018.9.1  changed to hide windows
	ShowWindow(SW_HIDE);
//	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	return;
}
int StrToBin(TCHAR* inWord, BYTE* OutBin, int source_len)
{
	int t;
	int t2;
	int count = 0;
	BYTE temBin[2];
	temBin[0] = 1;
	temBin[1] = 1;
	if (source_len < 1)
		return 0;
	for(t = 0 ;t < source_len; t ++)
	{   
		t2 = inWord[t];
		if( t2 > 127 )
		{
			temBin[0] =  t2 >> 8 ;/// 256;
			temBin[1] =  t2;
			OutBin[count] = temBin[0];
			count += 1;
			OutBin[count] = temBin[1];
			count += 1;
		}
		else
		{
			OutBin[count] = t2;
			count += 1;
		}
	}
	return count;
}
//��������
WORD CDlgLogon::ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize)
{
	//��������
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)cbBuffer;

	//������Ϣ
	CWHService::GetMachineID(pLogonAccounts->szMachineID);
	CWHEncrypt::MD5Encrypt(m_szPassword,pLogonAccounts->szPassword);

	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	//��ȡ����
	CWHIniData IniData;
	IniData.SetIniFilePath(szFileName);

	TCHAR szMain[12] = L"";
	IniData.ReadEncryptString(TEXT("MainVersion"),TEXT("MAIN"),TEXT("28"),szMain,CountArray(szMain));
	
	int nMain = _ttoi(szMain);
	IniData.ReadEncryptString(TEXT("MainVersion"),TEXT("SUB"),TEXT("1"),szMain,CountArray(szMain));
	int nSub = _ttoi(szMain);
	IniData.ReadEncryptString(TEXT("MainVersion"),TEXT("PRO"),TEXT("17"),szMain,CountArray(szMain));
	int nPro = _ttoi(szMain);

// 	BYTE cbMain = 0;
// 	StrToBin(szMain,&cbMain,24);

	//��¼��Ϣ
	pLogonAccounts->dwPlazaVersion=PROCESS_VERSION(nMain,nSub,nPro);//(VERSION_PLAZA;
	BYTE cbBuild = GetBuildVer(pLogonAccounts->dwPlazaVersion);
	BYTE cbMain = GetMainVer(pLogonAccounts->dwPlazaVersion);
	BYTE cbSub = GetSubVer(pLogonAccounts->dwPlazaVersion);
	BYTE cbPro = GetProductVer(pLogonAccounts->dwPlazaVersion);
	lstrcpyn(pLogonAccounts->szAccounts,m_szAccounts,CountArray(pLogonAccounts->szAccounts));

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	lstrcpyn(pGlobalUserData->szPassword,pLogonAccounts->szPassword,CountArray(pGlobalUserData->szPassword));

	return sizeof(CMD_GP_LogonAccounts);
}

//�����˺�
VOID CDlgLogon::LoadAccountsInfo()
{
	//��ȡ��Ϣ
	CWHRegKey RegUserInfo;
	if (RegUserInfo.OpenRegKey(REG_USER_INFO,false)==false) return;

	//��������
	DWORD dwRegIndex=0L;
	DWORD dwLastUserID=RegUserInfo.GetValue(TEXT("LastUserID"),0L);

	//��ȡ��Ϣ
	do
	{
		//��������
		tagAccountsInfo AccountsInfo;
		ZeroMemory(&AccountsInfo,sizeof(AccountsInfo));

		//��ȡ����
		TCHAR szKeyItemName[16]=TEXT("");
		if (RegUserInfo.EnumItemString(dwRegIndex++,szKeyItemName,CountArray(szKeyItemName))==false) break;

		//���Ӽ�
		CWHRegKey RegUserItem;
		if (RegUserItem.Attach(RegUserInfo.OpenItemKey(szKeyItemName))==NULL) continue;

		//�ʻ���Ϣ
		AccountsInfo.dwUserID=_tstol(szKeyItemName);
		RegUserItem.GetString(TEXT("GameID"),AccountsInfo.szGameID,CountArray(AccountsInfo.szGameID));
		RegUserItem.GetString(TEXT("UserAccount"),AccountsInfo.szAccounts,CountArray(AccountsInfo.szAccounts));

		//��ȡ����
		TCHAR szPassword[MAX_ENCRYPT_LEN]=TEXT("");
		RegUserItem.GetString(TEXT("UserPassword"),szPassword,CountArray(szPassword));

		//�⿪����
		if (szPassword[0]!=0)
		{
			CWHEncrypt::XorCrevasse(szPassword,AccountsInfo.szPassword,CountArray(AccountsInfo.szPassword));
		}

		//��������
		tagAccountsInfo * pAccountsInfo=new tagAccountsInfo;
		CopyMemory(pAccountsInfo,&AccountsInfo,sizeof(AccountsInfo));

		//��������
		m_AccountsInfoArray.InsertAt(m_AccountsInfoArray.GetCount(),pAccountsInfo);

	} while (true);

	//������Ϣ
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		//������Ϣ
		INT nGameIDItem=CB_ERR;
		INT nAccountsItem=CB_ERR;
		tagAccountsInfo * pAccountsInfo=m_AccountsInfoArray[i];

		//������Ϣ
		if (pAccountsInfo->szAccounts[0]!=0) nAccountsItem=m_AccountsControl.AddString(pAccountsInfo->szAccounts);

		//��������
		if (nAccountsItem!=CB_ERR) m_AccountsControl.SetItemDataPtr(nAccountsItem,pAccountsInfo);

		//����ѡ��
		if ((dwLastUserID!=0L)&&(pAccountsInfo->dwUserID==dwLastUserID))
		{
			if (nAccountsItem!=CB_ERR) m_AccountsControl.SetCurSel(nAccountsItem);
		}
	}

	//����ѡ��
	if ((m_AccountsControl.GetCount()>0)&&(m_AccountsControl.GetCurSel()==LB_ERR)) m_AccountsControl.SetCurSel(0);

	return;
}
UINT CDlgLogon::GetBestUrl(LPVOID lpParam)
{
	CDlgLogon* pLogon = (CDlgLogon*)lpParam;
	if(pLogon->GetSafeHwnd() == NULL)
		return 0;
	int nLoadServer = 0;
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
			TCHAR szKeyBoard[124]=L"";
			TCHAR szTodayURL[124] = L"";

			int nTTL = 10000;
			CPing ping;
			PingReply reply;

			for(int i = 0;i < 5;i++)
			{
				memset(&reply, 0, sizeof(PingReply));
				_sntprintf(szKeyBoard,CountArray(szKeyBoard),_T("LogonServer%d"),i);
				IniData.ReadEncryptString(TEXT("PlazaUrl"),szKeyBoard,TEXT(""),szTodayURL,CountArray(szTodayURL));

				USES_CONVERSION;
				int nValidHostName = inet_addr(W2A(szTodayURL));

				if(nValidHostName == -1)
				{
					DWORD dwServerAddr = 0;
					LPHOSTENT lpHost=gethostbyname(CT2CA(szTodayURL));
					if (lpHost!=NULL) 
						dwServerAddr=((LPIN_ADDR)lpHost->h_addr)->s_addr;
					in_addr t1;
					t1.S_un.S_addr = dwServerAddr;
					char* pTrueAddr = inet_ntoa(t1);

					ping.Ping(dwServerAddr,&reply);

				}
				else
				{
					char szUrl[124] = "";
					int iLength ;  
					//��ȡ�ֽڳ���   
					iLength = WideCharToMultiByte(CP_ACP, 0, szTodayURL, -1, NULL, 0, NULL, NULL);  
					//��tcharֵ����_char    
					WideCharToMultiByte(CP_ACP, 0, szTodayURL, -1, szUrl, iLength, NULL, NULL);   
					ping.Ping(szUrl,&reply);
				}
				CString strLog;
				
				strLog.Format(L"PINGSECOND  MISSIONSEND  %s   %d",szTodayURL,reply.m_dwRoundTripTime);
				OutputDebugString(strLog);

				if (nTTL > reply.m_dwRoundTripTime&&reply.m_dwRoundTripTime!=0)
				{
					nTTL = reply.m_dwRoundTripTime;
					nLoadServer = i;

				}

			}
			pLogon->SetCurServer(nLoadServer);

	return nLoadServer;
}
void CDlgLogon::SetCurServer(int nLoad)
{
	if(m_Destroyed)
		return;

	if(!IsWindow(this->m_hWnd ))
		return;
	if(!IsWindow(m_ServerControl.GetSafeHwnd()))
		return;
	if(m_ServerControl.GetSafeHwnd())
		m_ServerControl.SetCurSel(nLoad);
	m_nLoadServer = nLoad;
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

	TCHAR szTodayURL[124] = L"";
	TCHAR szKeyBoard[124]=L"";
	_sntprintf(szKeyBoard,CountArray(szKeyBoard),_T("LogonServer%d"),nLoad);
	IniData.ReadEncryptString(TEXT("PlazaUrl"),szKeyBoard,TEXT(""),szTodayURL,CountArray(szTodayURL));
	CString strLog;

	strLog.Format(L"PINGSECOND  MISSIONSEND CurSel: %s ",szTodayURL);
	OutputDebugString(strLog);

	_sntprintf(m_szLogonServer,CountArray(m_szLogonServer),_T("%s"),szTodayURL);
}
//��ַ��Ϣ
VOID CDlgLogon::LoadLogonServerInfo()
{

	return;

	CWinThread *pThread =  AfxBeginThread(GetBestUrl, this);


	return;
//	if (m_ServerControl.GetCurSel()==LB_ERR)
	{
	//	if (m_ServerControl.GetCount()==0L)
		{
			//m_DlgStatus.ShowStatusWindow(L"��������������վ...");
			
			TCHAR szPlatformUrl[124] = L"";
			TCHAR szPlatformUrl1[124] = L"";

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

// 			CTime time = CTime::GetCurrentTime();
 			TCHAR szTodayURL[124] = L"";
 			int nUrlID = 0;//GetPrivateProfileInt(time.Format(L"%Y-%m-%d"),TEXT("TodayURLID"),0,szFileName);
// 			int nUrlCanUse = GetPrivateProfileInt(time.Format(L"%Y-%m-%d"),TEXT("TodayCanUse"),0,szFileName);
			TCHAR szKeyBoard[124]=L"";
			_sntprintf(szKeyBoard,CountArray(szKeyBoard),_T("LogonServer%d"),m_nLoadServer);
			IniData.ReadEncryptString(TEXT("PlazaUrl"),szKeyBoard,TEXT(""),szTodayURL,CountArray(szTodayURL));

// 			CString strUrl;
// 			strUrl.Format(L"%s",szTodayURL);
// 
// 
// 			int nUrl=0;
// 			TCHAR szKey[32];
// 
// 			if(strUrl.IsEmpty())
// 			{
// 				int nTTL = 10000;
// 				CPing ping;
// 				PingReply reply;
// 				for(int i = 0;i < 4;i++)
// 				{
// // 					if(!strUrl.IsEmpty()&&(i == nUrlID))
// // 					{
// // 						continue;
// // 					}
// 					memset(&reply, 0, sizeof(PingReply));
// 			//		TCHAR szKey[32];
// 					ZeroMemory(&szKey,sizeof(szKey));
// 					_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),i);
// 					//��ȡ����
// 					IniData.ReadEncryptString(TEXT("PlazaUrl"),szKey,NULL,szPlatformUrl1,CountArray(szPlatformUrl1));
// 
// 					USES_CONVERSION;
// 					int nValidHostName = inet_addr(W2A(szPlatformUrl1));
// 
// 					if(nValidHostName == -1)
// 					{
// 						DWORD dwServerAddr = 0;
// 						LPHOSTENT lpHost=gethostbyname(CT2CA(szPlatformUrl1));
// 						if (lpHost!=NULL) 
// 							dwServerAddr=((LPIN_ADDR)lpHost->h_addr)->s_addr;
// 						in_addr t1;
// 						t1.S_un.S_addr = dwServerAddr;
// 						char* pTrueAddr = inet_ntoa(t1);
// 						OutputDebugStringA(pTrueAddr);
// 
// 						ping.Ping(dwServerAddr,&reply);
// 
// 					}
// 					else
// 					{
// 						char szUrl[124] = "";
// 						int iLength ;  
// 						//��ȡ�ֽڳ���   
// 						iLength = WideCharToMultiByte(CP_ACP, 0, szPlatformUrl1, -1, NULL, 0, NULL, NULL);  
// 						//��tcharֵ����_char    
// 						WideCharToMultiByte(CP_ACP, 0, szPlatformUrl1, -1, szUrl, iLength, NULL, NULL);   
// 						ping.Ping(szUrl,&reply);
// 					}
// 
// 
// 
// 					//CString strTmp;
// 					if((reply.m_dwRoundTripTime < nTTL) || lstrlen(szPlatformUrl) == 0)
// 					{
// 						lstrcpyn(szPlatformUrl,szPlatformUrl1,CountArray(szPlatformUrl));
// 						nUrl = i;
// //						if(reply.m_dwRoundTripTime > 0 && reply.m_dwRoundTripTime < 10000)
// //						{
// // 							//���PINGͨ�ˣ�ע����¼һ�µ�ǰSERVER
// // 							CWHRegKey RegServerAddr;
// // 							RegServerAddr.OpenRegKey(TEXT("CurrentLogonServer"),true);
// // 							RegServerAddr.WriteString(TEXT("CurrentIpAddress"),szPlatformUrl);
// //						}
// 						nTTL = reply.m_dwRoundTripTime;
// 					}
// 				}
// 
// 			}
// 			else if(nUrlCanUse==0)
// 			{
// 				nUrl = (nUrlID+1)%5;
// 				ZeroMemory(&szKey,sizeof(szKey));
// 				_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),nUrl);
// 				//��ȡ����
// 				IniData.ReadEncryptString(TEXT("PlazaUrl"),szKey,NULL,szPlatformUrl,CountArray(szPlatformUrl));
// 
// 			}
// 			else
//			{
				lstrcpyn(szPlatformUrl,szTodayURL,CountArray(szPlatformUrl));
//			}

// 			ZeroMemory(&szKey,sizeof(szKey));
// 			_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),nUrl);
// 
// 			GetPrivateProfileString(TEXT("PlazaUrl"),szKey,NULL,szTodayURL,CountArray(szTodayURL),szFileName);
// 
// 			WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayURL"),szTodayURL,szFileName);
// 			_sntprintf(szKey,CountArray(szKey),_T("%d"),nUrl);
// 			WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayURLID"),szKey,szFileName);
// 			_sntprintf(szKey,CountArray(szKey),_T("%d"),1);
// 			WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayCanUse"),szKey,szFileName);
// 		

			_sntprintf(m_szLogonServer,CountArray(m_szLogonServer),_T("%s"),szPlatformUrl);
			m_ServerControl.SetCurSel(nUrlID);
			m_DlgStatus.HideStatusWindow();
		}
// 		else
// 		{
// 			m_ServerControl.SetCurSel(0);
// 		}
	}

	return;
}
//ȡ������
VOID CDlgLogon::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//��ȡ��Ϣ
bool CDlgLogon::GetInformation()
{
	//��ȡ����
	m_PasswordControl.GetUserPassword(m_szPassword);

	//��ȡ��Ϣ
	GetDlgItemText(IDC_ACCOUNTS,m_szAccounts,CountArray(m_szAccounts));
	//GetDlgItemText(IDC_LOGON_SERVER,m_szLogonServer,CountArray(m_szLogonServer));

	//��ַ�ж�
	if (m_szLogonServer[0]==0L)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��ѡ�񡰵�¼�����������ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		m_ServerControl.SetFocus();

		return false;
	}

	//�ʺ��ж�
	if (m_szAccounts[0]==0L)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���������ʺź��ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		m_edAccounts.SetFocus();

		return false;
	}

	//�����ж�
	if (m_szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������������ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		m_PasswordControl.SetFocus();

		return false;
	}

	return true;
}

//ע���ʺ�
VOID CDlgLogon::OnBnClickedRegister()
{

	return;
}
#include "MessageDlg.h"
//�һ�����
VOID CDlgLogon::OnBnClickedForget()
{

	MyMessageBox(L"����ϵ�ͷ��һ����룡");
	return;
	//��ȡ�����ļ�
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	//	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%sLogonLink4.aspx"),szPlatformLink);
	GetPrivateProfileString(TEXT("register"),TEXT("Link"),TEXT(""),szLogonLink,CountArray(szLogonLink),szFileName);

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}
VOID CDlgLogon::OnBnClickedDelete()
{
	//��������
	CAccountsCtrl * pAccountsCtrl=NULL;
	tagAccountsInfo * pAccountsInfo=NULL;

	pAccountsCtrl=&m_AccountsControl;

	//��ȡ��Ϣ
	INT nCurrentSel=pAccountsCtrl->GetCurSel();

	//ɾ������
	if (nCurrentSel!=LB_ERR)
	{
		//��ȡ����
		ASSERT(pAccountsCtrl->GetItemDataPtr(nCurrentSel)!=NULL);
		pAccountsInfo=(tagAccountsInfo *)pAccountsCtrl->GetItemDataPtr(nCurrentSel);

		//�����ʶ
		CString strUserID;
		strUserID.Format(TEXT("%ld"),pAccountsInfo->dwUserID);

		//�û�����
		CWHRegKey RegUserInfo;
		RegUserInfo.OpenRegKey(REG_USER_INFO,false);

		//ɾ������
		RegUserInfo.RecurseDeleteKey(strUserID);


		//ɾ���б�
		for (INT i=0;i<m_AccountsControl.GetCount();i++)
		{
			if (m_AccountsControl.GetItemDataPtr(i)==pAccountsInfo)
			{
				//ɾ���ַ�
				m_AccountsControl.DeleteString(i);
				m_AccountsControl.SetWindowText(TEXT(""));

				//����ѡ��
				if ((m_AccountsControl.GetCurSel()==LB_ERR)&&(m_AccountsControl.GetCount()>0))
				{
					m_AccountsControl.SetCurSel(0);
				}

				break;
			}
		}
	}

	//���ý���
	pAccountsCtrl->SetFocus();

	//��ȡ����
	nCurrentSel=pAccountsCtrl->GetCurSel();
	pAccountsInfo=(nCurrentSel!=LB_ERR)?(tagAccountsInfo *)pAccountsCtrl->GetItemDataPtr(nCurrentSel):NULL;

	//��������
	m_PasswordControl.SetUserPassword((pAccountsInfo!=NULL)?pAccountsInfo->szPassword:TEXT(""));

	//��ס����
	bool bRemPassword=(pAccountsInfo!=NULL)?(pAccountsInfo->szPassword[0]!=0):false;
	//((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);
	m_RemPwdControl.SetButtonChecked((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);

	return;
}
//�ͷ�
VOID CDlgLogon::OnBnClickedKeFu()
{
// 	//�����ַ
// 	TCHAR szNavigation[256]=TEXT("");
// //	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("http://ala.zoossoft.com/LR/Chatpre.aspx?id=ALA22728236&lng=cn"));
// 
// 	//��ȡ�����ļ�
// 	//����Ŀ¼
// 	TCHAR szDirectory[MAX_PATH]=TEXT("");
// 	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));
// 
// 	//����·��
// 	TCHAR szFileName[MAX_PATH]=TEXT("");
// 	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
// 
// 	//�����ַ
// 	TCHAR szLogonLink[256]=TEXT("");
// 	//	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%sLogonLink4.aspx"),szPlatformLink);
// 	GetPrivateProfileString(TEXT("kefu"),TEXT("Link"),TEXT(""),szNavigation,CountArray(szNavigation),szFileName);
// 
// 	//���ӵ�ַ
// 	ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
	//ִ�е�¼
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformKefu();

	return;
}

//�༭�ı�
VOID CDlgLogon::OnCbnEditchangeAccounts()
{
	//��ȫ����
	((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_UNCHECKED);

	//��������
	if (m_PasswordControl.IsFalsityPassword()==true) m_PasswordControl.SetUserPassword(NULL);
	m_edAccounts.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );

	return;
}

//�滭����
BOOL CDlgLogon::OnEraseBkgnd(CDC * pDC)
{
// 	//��ȡλ��
// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 
// 	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
// 
// 	Graphics graphics( pDC->m_hDC );
// 
// 	Image image(CBmpUtil::GetExePath() + _T("skin\\DLG_LOGON_BACK.png"));
// 
// 	UINT width = image.GetWidth();
// 
// 	UINT height = image.GetHeight();
// 
// 	// ����������
// 	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
// 	graphics.DrawImage( &image,0,0);
//	m_RemPwdControl.OnDrawControl(pDC);

	return false;
}
//�ػ���Ϣ
VOID CDlgLogon::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int Height = rect.bottom - rect.top;

	CDC *pDC = this->GetDC();
//  	Image image(CBmpUtil::GetExePath() + _T("skin\\DLG_LOGON_BACK.png"));
//  
//  	UINT width = image.GetWidth();
//  
//  	UINT height = image.GetHeight();
// 	CDC MenmDC;
// 	CBitmap MemBitmap;
// 	MenmDC.CreateCompatibleDC(NULL);
// 	MemBitmap.CreateCompatibleBitmap(pDC, width, height);
// 	CBitmap *pOldBit = MenmDC.SelectObject(&MemBitmap);
// 	MenmDC.SetBkMode(TRANSPARENT);
// 
// 
// 	using namespace Gdiplus;
// 	Graphics graphics(MenmDC.m_hDC);
// 	graphics.DrawImage(&image, Rect(0, 0,width,height), 0, 0, width,height, UnitPixel);
	/*********************************************************/
	CPngImage BackImage;
	BackImage.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_LOGON_BACK"));
	BackImage.DrawImage(pDC,0,0);

	//pDC->BitBlt(0, 0, width, height, &MenmDC, 0, 0, SRCCOPY);
	m_RemPwdControl.OnDrawControl(pDC);

	if(!m_png_Kefu.IsNull())
		m_png_Kefu.DrawImage(pDC,300, 200);

// 	MenmDC.SelectObject(pOldBit);
// 	MemBitmap.DeleteObject();
// 	MenmDC.DeleteDC();
// 	ReleaseDC(pDC);
	return;
}

//��ʾ��Ϣ
VOID CDlgLogon::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//��ʾ�ֲ�
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
		if(bShow)
		{
			LoadLogonServerInfo();
			Loadweb();
		}
	}

	return;
}

//�����Ϣ
VOID CDlgLogon::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�����
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//λ�øı�
VOID CDlgLogon::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//�ƶ��ֲ�
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//�ؼ���ɫ
HBRUSH CDlgLogon::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	if(pWnd->GetDlgCtrlID() == IDC_ACCOUNTS)
	{
	//	pDC->SetBkMode(TRANSPARENT);
		m_edAccounts.SetFont(&m_Font,false);
		pDC->SetBkColor(RGB(255,255,255));
		HBRUSH B = CreateSolidBrush(RGB(255,255,255)); 
		return (HBRUSH) B;

	}
	if(pWnd->GetDlgCtrlID() == IDC_PASSWORD)
	{
		pDC->SetBkMode(TRANSPARENT);
		m_PasswordControl.SetFont(&m_Font,false);
		pDC->SetBkColor(RGB(255,0,0));
		HBRUSH B = CreateSolidBrush(RGB(234,179,106)); 
		//return (HBRUSH) B;

	}

	switch (nCtlColor)
	{
	//case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(10,10,10));
			return m_brBrush;
		}
	case IDC_ACCOUNTS:
		{
			pDC->SetBkMode(TRANSPARENT);
			m_edAccounts.SetFont(&m_Font,false);
			pDC->SetBkColor(RGB(255,0,0));
			HBRUSH B = CreateSolidBrush(RGB(255,0,0)); 
			return (HBRUSH) B;

			//pDC->SetFont(&m_Font);
			//return m_brBrush;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor); ;
}

//�ؼ��ı�
void CDlgLogon::OnEnChangeAccounts()
{
//	m_edAccounts.SetFont(&m_Font,false);

	//�������
	if(m_PasswordControl.m_hWnd!=NULL && m_PasswordControl.IsFalsityPassword()==true)
	{
		//��������
		m_PasswordControl.SetUserPassword(TEXT(""));

        //��ȫ����
		m_RemPwdControl.SetButtonChecked(FALSE);
	}
	m_edAccounts.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );

}

//��굯��
void CDlgLogon::OnLButtonUp(UINT nFlags, CPoint point)
{
	//ѡ���¼�
	m_RemPwdControl.OnClickControl(point);

	CDialog::OnLButtonUp(nFlags, point);
}



//////////////////////////////////////////////////////////////////////////////////



