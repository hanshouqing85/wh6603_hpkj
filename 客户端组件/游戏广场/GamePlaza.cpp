#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"
#include "MD5Checksum\MD5Checksum.h"
//////////////////////////////////////////////////////////////////////////////////
#import <msxml3.dll>
using namespace MSXML2;

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePlazaApp::CGamePlazaApp()
{
	//���ñ���
	m_pIDispatch=NULL;
	m_pCustomOccManager=NULL;

	return;
}

//��ʼ����
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();

#ifndef _DEBUG
#ifndef RELEASE_MANAGER

	//�����ж�
	bool bPlazaExist=false;
	CMutex Mutex(FALSE,szPlazaClass,NULL);
	if (Mutex.Lock(0)==FALSE) bPlazaExist=true;

	LoadLibraryA("RICHED20.DLL");
	//��������
	CWnd * pWndGamePlaza=CWnd::FindWindow(szPlazaClass,NULL);
	if (pWndGamePlaza!=NULL) 
	{
		//���ñ���
		bPlazaExist=true;

		//��ԭ����
		if (pWndGamePlaza->IsIconic()) 
		{
			pWndGamePlaza->ShowWindow(SW_RESTORE);
		}

		//�����
		pWndGamePlaza->SetActiveWindow();
		pWndGamePlaza->BringWindowToTop();
		pWndGamePlaza->SetForegroundWindow();
	}

	//�������
	if (bPlazaExist==true) return FALSE;

#endif
#endif

	//��������
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//�������
	srand((DWORD)time(NULL));

	//���ñ���
	SetRegistryKey(szProductKey);


	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//��װĿ¼
	CWHRegKey RegInstallPath;
	RegInstallPath.OpenRegKey(TEXT("HpKjGameDev"),true);
	RegInstallPath.WriteString(TEXT("InstallPath"),szDirectory);

	//��������
	m_ParameterGlobal.LoadParameter();
	m_UserInformation.LoadInformation();

	//����Ŀ¼
	SetCurrentDirectory(szDirectory);
	m_SkinResourceManager.SetSkinResource(m_ParameterGlobal.m_SkinRenderInfo);

	m_strCurrentDir = szDirectory;
	m_strCurrentDir += L"\\";
	//��������
	m_pIDispatch=new CImpIDispatch;
	AfxEnableControlContainer(new CCustomOccManager); 

	//�������
	if (m_FaceItemControlModule.CreateInstance()==false) return false;
	if (m_UserOrderParserModule.CreateInstance()==false) return false;

	//�û�Ԫ��
	m_UserItemElement.SetUserOrderParser(m_UserOrderParserModule.GetInterface());
	m_UserItemElement.SetFaceItemControl(m_FaceItemControlModule.GetInterface());

	theApp.InitCaiZhong();

//  	//������
//  #ifndef DEBUG 
//  #ifndef RELEASE_MANAGER
//  
// //  	CString strSiteURL;
// //  
// //  	//����·��
// //  	TCHAR szFileName[MAX_PATH]=TEXT("");
// //  	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\update.ini"),szDirectory);
// //  	//�����ַ
// //  	TCHAR szNavigation[256]=TEXT("");
// //  
// //  	GetPrivateProfileString(TEXT("SERVER"),TEXT("SERVER1"),TEXT(""),szNavigation,CountArray(szNavigation),szFileName);
// //  	strSiteURL = szNavigation;
// //  
// //  	strSiteURL+=_T("/update/update.xml");
// //  	//EWIN���� �Ƽ�
// //  	if(lstrcmp(AfxGetApp()->m_lpCmdLine,TEXT("UPDATED"))!=0 && CheckVersionUpdate(strSiteURL))
// //  	{
// //  		//��������
// //  		CHAR szModuleName[MAX_PATH]={0};
// //  		GetModuleFileNameA(AfxGetInstanceHandle(), szModuleName, MAX_PATH);
// //  		PathRemoveFileSpecA(szModuleName);
// //  		strcat(szModuleName, "\\Update.exe");
// //  		WinExec(szModuleName, SW_SHOWDEFAULT);
// //  
// //  
// //  		return true;
// //  
// //  	}
//  #endif
//  #endif
	theBeginCount = ::GetTickCount();
	//��������
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//ע�ᴰ��
	WndClasss.style=CS_DBLCLKS;
	WndClasss.hIcon=LoadIcon(IDR_MAINFRAME);
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=szPlazaClass;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	//��������
	CPlatformFrame * pPlatformFrame=new CPlatformFrame();
	pPlatformFrame->Create(szPlazaClass,szProduct,WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));
	//���ñ���
	m_pMainWnd=pPlatformFrame;


	return TRUE;
}
bool	CGamePlazaApp::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}
//��ȡϵͳʱ��
bool CGamePlazaApp::GetSystemT()
{
//	SYSTEMTIME sysT;
// 	memset(&sysT, 0, sizeof(SYSTEMTIME));
// 	GetSystemTime(sysT);
// 	if(!theDBCmd.GetSysTime(sysT))
// 	{
// 		return false;
// 	}

// 	theTimeCS.Lock();
// 	theTime = CTime::GetCurrentTime()/*(sysT)*/;
// 	theTimeCS.Unlock();

	return true;
}
BOOL CGamePlazaApp::CheckVersionUpdate(CString& strUpdateURL)
{
	BOOL bHasUpdate = FALSE;

	HRESULT hr = S_OK;
	IXMLHTTPRequestPtr pHttpRequest;
	IDispatchPtr pDispatch;
	MSXML2::IXMLDOMDocumentPtr pXmlDoc;
	MSXML2::IXMLDOMNodeListPtr pList;
	MSXML2::IXMLDOMElementPtr pChild;

	UINT nFileSize;
	CString strFileName, strFileVer, strMD5String;
	LONG lElementCount = 0L;

	try
	{
		hr = pHttpRequest.CreateInstance(TEXT("Msxml2.XMLHTTP.3.0"));
		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = pHttpRequest->open(TEXT("GET"), (_bstr_t)strUpdateURL, false);
		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = pHttpRequest->send();
		if( FAILED(hr) )
			_com_issue_error(hr);

		if (pHttpRequest->Getstatus() != 200)
			throw (0);

		pDispatch = pHttpRequest->GetresponseXML();
		hr = pDispatch->QueryInterface(pXmlDoc.GetIID(), (void**)&pXmlDoc);
		if( FAILED(hr) )
			_com_issue_error(hr);

		pList = pXmlDoc->selectNodes("/manifest/filelist/file");
		lElementCount = pList->Getlength();
		for( LONG i = 0; i < lElementCount; i++ )
		{
			pChild = pList->Getitem(i);
			strFileName = pChild->getAttribute("filename");
			nFileSize = pChild->getAttribute("filesize");
			strFileVer = pChild->getAttribute("fileversion");
			strMD5String = pChild->getAttribute("md5");

			//��������ļ�������Ч���ļ��������ھ���������ļ�
			if (PathFileExists(m_strCurrentDir+strFileName))
			{
				//��������
				TCHAR szInsurePass[LEN_MD5];
				CWHEncrypt::MD5Encrypt(m_strCurrentDir+strFileName,szInsurePass);

				CHAR szModuleName[MAX_PATH]={0};
				GetModuleFileNameA(AfxGetInstanceHandle(), szModuleName, MAX_PATH);

		
				CString strFName = (m_strCurrentDir+strFileName);

				CString strMD5File = CMD5Checksum::GetMD5(strFName) ;
				//Ч���ļ�,���MD5�벻��ͬ����������
				if (strMD5File != strMD5String)
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


//��ȡ
void CGamePlazaApp::GetTime(CTime& time)
{
	theTimeCS.Lock();
	time = theTime;
	theTimeCS.Unlock();
}

//��ʼ��������Ϣ
void CGamePlazaApp::InitCaiZhong()
{
	theGameType[CZJiangXiSSC] = _T("����ʱʱ��");
	theGameType[CZ_TianJinSSC] = _T("���ʱʱ��");

	theGameType[CZChongQingSSC] = _T("����ʱʱ��");
	theGameType[CZ_FENFEN_CAI] = _T("�����ֲַ�");
	theGameType[CZ_WUFEN_CAI] = _T("������ֲ�");
	theGameType[CZXinJiangSSC] = _T("�½�ʱʱ��");
	theGameType[CZ_HGYDWFC] = _T("����1.5�ֲ�");
	theGameType[CZ3D] = _T("3D��");
	theGameType[CZGD11Xuan5]=_T("�㶫11ѡ5");
	theGameType[CZCQ11Xuan5]=_T("����11ѡ5");
	theGameType[CZJX11Xuan5]=_T("����11ѡ5");
	theGameType[CZSD11Xuan5]=_T("ɽ��11ѡ5");
	theGameType[CZ_LIUHECAI] = _T("���ϲ�");
	theGameType[CZHLJ11Xuan5] = _T("������11ѡ5");
	theGameType[CZ_QiXingCai] = _T("���ǲ�");
	theGameType[CZKUAILE8] = _T("������8");
	theGameType[CZPaiLie3] = _T("������");
	theGameType[CZ_PK10] = _T("����PK10");
	theGameType[CZXingYun28] = _T("����28");

}

//�˳�����
INT CGamePlazaApp::ExitInstance()
{
	//��������
	m_ParameterGlobal.SaveParameter();

	//ɾ������
	SafeDelete(m_pIDispatch);
	SafeDelete(m_pCustomOccManager);

	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
CGamePlazaApp theApp;

CMap<unsigned int, unsigned int, double, double> theBonus;
//������Ϣ-����ʱʱ��
CMap<int, int, CString, CString&> theGameType;
//�淨ģʽ-����������ʽ
CMap<int, int, CString, CString&> theGameKind;
//����ʱ����ϵͳʱ��Ĳ�ֵ
//long theTimeDiff = 0L;
//ϵͳʱ��
CTime theTime;
BaseUser theAccount;
//////////////////////////////////////////////////////////////////////////////////
