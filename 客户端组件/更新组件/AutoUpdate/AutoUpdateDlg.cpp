// AutoUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include "AutoUpdateDlg.h"
#include "TransparentHelper.h"
#include "..\MD5Checksum\MD5Checksum.h"

#import <msxml3.dll>
using namespace MSXML2;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HTTP_PROTOCOL	_T("http://")

// CAutoUpdateDlg dialog


CAutoUpdateDlg::CAutoUpdateDlg(CWnd* pParent): CDialog(IDD_AUTOUPDATE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_ulTotalLength = 0L;
	m_ulCurentLenght = 0L;
    m_status=0;
	m_pMemDC = NULL;
	m_pOldBitmap = NULL;
	m_pBitmapMem = NULL;
	m_bmBack.LoadBitmap(IDB_DIALOG_BACK);
}

CAutoUpdateDlg::~CAutoUpdateDlg()
{
	if (m_pMemDC!=NULL)
	{
		m_pMemDC->SelectObject(m_pOldBitmap);
		delete m_pMemDC;
		delete m_pBitmapMem;
	}
}

void CAutoUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CURRENT_PROGRESS, m_Progress1);
	DDX_Control(pDX, IDC_TOTAL_PROGRESS, m_Progress2);
	DDX_Control(pDX, IDC_PERCENT_TEXT, m_PercentText);
	DDX_Control(pDX, IDC_STATUS_TEXT, m_StatusText);
	DDX_Control(pDX, IDC_COUNT_TEXT, m_CountText);
    DDX_Control(pDX, IDC_COUNT_TEXT2, m_CountText2);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_RETRY, m_btRetry);
	DDX_Control(pDX, IDC_BY_SELF, m_btBySelf);
	
}

BEGIN_MESSAGE_MAP(CAutoUpdateDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_MESSAGE( WM_TRANSPARENT_BK, &CAutoUpdateDlg::OnTransaprentBk)
	ON_BN_CLICKED(IDCANCEL, &CAutoUpdateDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RETRY, &CAutoUpdateDlg::OnBnClickedRetry)
	ON_BN_CLICKED(IDC_BY_SELF, &CAutoUpdateDlg::OnBnClickedSelf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CAutoUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

 //   // ����PNGͼƬ
	//LoadPngImage();
	m_font.CreateFont(-36, // nHeight 
	 0, // nWidth 
	 0, // nEscapement 
	 0, // nOrientation 
	 FW_NORMAL, // nWeight 
	 FALSE, // bItalic 
	 FALSE, // bUnderline 
	 0, // cStrikeOut 
	DEFAULT_CHARSET, // nCharSet 
	 OUT_DEFAULT_PRECIS, // nOutPrecision 
	 CLIP_DEFAULT_PRECIS, // nClipPrecision 
	 DEFAULT_QUALITY, // nQuality 
	 DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
	 _T("΢���ź�"));

	m_font2.CreateFont(-14, // nHeight 
	 0, // nWidth 
	 0, // nEscapement 
	 0, // nOrientation 
	 FW_NORMAL, // nWeight 
	 FALSE, // bItalic 
	 FALSE, // bUnderline 
	 0, // cStrikeOut 
	DEFAULT_CHARSET, // nCharSet 
	 OUT_DEFAULT_PRECIS, // nOutPrecision 
	 CLIP_DEFAULT_PRECIS, // nClipPrecision 
	 DEFAULT_QUALITY, // nQuality 
	 DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
	 _T("΢���ź�"));

    m_PercentText.SetFont( &m_font );
    m_StatusText.SetFont( &m_font2 );
    m_CountText.SetFont( &m_font2 );
    m_CountText2.SetFont( &m_font2 );
    
	COLORREF color=RGB(216,205,131);
	COLORREF color2=RGB(255,250,228);
	m_PercentText.SetTextColor(color);
	m_StatusText.SetTextColor(color2);
    m_StatusText.ShowWindow(SW_HIDE);
	m_CountText.SetTextColor(color2);
    m_CountText2.SetTextColor(color2);

	//m_
	HWND hNotifyWindow = NULL;
	TCHAR szModuleFile[MAX_PATH]={0};
	CString strSiteURL;
	CString strTempDir;

	m_btRetry.SetButtonImage(IDB_RETRY);
	m_btRetry.EnableWindow(FALSE);
	m_btBySelf.SetButtonImage(IDB_SELF);
	//���Ҵ���

	hNotifyWindow = ::FindWindow(szPlazaClass, NULL);
	//�رմ���
	if (hNotifyWindow!=NULL)
	{
		::SendMessage(hNotifyWindow,WM_CLOSE,0,0);
	}


	GetModuleFileName(AfxGetInstanceHandle(), szModuleFile, MAX_PATH);
	PathRemoveFileSpec(szModuleFile);
	PathAddBackslash(szModuleFile);
	m_strCurrentDir = szModuleFile;

	//�����ַ
	TCHAR szNavigation[256]=TEXT("");
	//	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("http://ala.zoossoft.com/LR/Chatpre.aspx?id=ALA22728236&lng=cn"),szPlatformLink);


	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Update\\Update.ini"),szModuleFile);

	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	//	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%sLogonLink4.aspx"),szPlatformLink);
	GetPrivateProfileString(TEXT("SERVER"),TEXT("SERVER1"),TEXT(""),szNavigation,CountArray(szNavigation),szFileName);


	//��ȡ���ò���
	strSiteURL = AfxGetApp()->GetProfileString(_T("SERVER"), _T("SERVER1"), szNavigation);
	strTempDir = AfxGetApp()->GetProfileString(_T("DIR"), _T("TEMPDIR"), _T("Update"));
	TCHAR szXmlUrl[MAX_PATH]=TEXT("");
	// add by hxh 20160603
	GetPrivateProfileString(TEXT("SERVER"),TEXT("XMLURL"),TEXT(""),szXmlUrl,CountArray(szXmlUrl),szFileName);
	CString strXmlUrl = AfxGetApp()->GetProfileString(_T("SERVER"), _T("XMLURL"),szXmlUrl);

	//����URL·��
	if( strSiteURL.Left(7) != HTTP_PROTOCOL )
		strSiteURL = HTTP_PROTOCOL + strSiteURL;

	strSiteURL += _T("/update/");

	CString strLog;
	strLog.Format(_T("\nUPDATEFAILED strSiteURL:%s"),strSiteURL);
	OutputDebugString(strLog);
	m_DownloadMgr.Initialize((IDownloadSink*)this, strSiteURL, m_strCurrentDir+strTempDir, m_strCurrentDir);

	BITMAP bm;
	m_bmBack.GetBitmap(&bm);
	SetWindowPos(NULL,0,0,bm.bmWidth,bm.bmHeight,SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOMOVE|SWP_NOZORDER);

	//���汾
	if (!CheckVersionUpdate(strXmlUrl))
	{

			//EWIN���粻����ֱ������
			//ASSERT(hNotifyWindow!=NULL);
			//::SendMessage(hNotifyWindow, WM_USER+102, 0, 0);
		
			CHAR szModuleName[MAX_PATH]={0};
			GetModuleFileNameA(AfxGetInstanceHandle(), szModuleName, MAX_PATH);
			PathRemoveFileSpecA(szModuleName);
			strcat(szModuleName, ("\\GamePlaza.exe UPDATED"));
			WinExec(szModuleName, SW_SHOWDEFAULT);
			//ShellExecute(NULL,TEXT("open"),szModuleName,NULL,NULL,true);
			SendMessage(WM_CLOSE);

		return TRUE;
	}

	// ����������PNGͼƬ
	m_Progress1.LoadPngImage();
    m_Progress2.LoadPngImage();

	//���÷�Χ
	m_Progress2.SetRange32(0, m_ulTotalLength/1024);

	// Send WM_NCCALCSIZE messages to the window
	m_Progress1.SetWindowPos(NULL, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	m_Progress2.SetWindowPos(NULL, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	CRect rcRetry;
	m_btRetry.GetWindowRect(&rcRetry);
	m_btRetry.SetWindowPos(NULL,44,107,rcRetry.Width(),rcRetry.Height(),SWP_NOMOVE|SWP_NOZORDER);

#if 0
    // ���Խ�����
	m_Progress1.SetRange32(0,20);
	m_Progress1.SetPos(10);
#endif

	ShowWindow(SW_SHOW);
	//���㴰�ڼ���
	BringWindowToTop();
	UpdateWindow();

	m_btCancel.EnableWindow(TRUE);
	//return TRUE;

	//��������
	m_DownloadMgr.Start();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rtClient;
		GetClientRect(&rtClient);

		if ( m_pMemDC )
		{
			dc.BitBlt( 0,0,rtClient.Width(), rtClient.Height(), m_pMemDC, 0,0, SRCCOPY);
		}

		// ����PNGͼƬ
		LoadPngImage();
		CDC * pDC=this->GetDC();
		m_BackImage.DrawImage(pDC,0,0);
		//for(int i=0;i<3 && i!=m_status;i++){

		//}
		m_StatusImage[m_status].DrawImage(pDC,50,80);
	}
}

VOID  CAutoUpdateDlg::LoadPngImage()
{
	m_BackImage.LoadImage(AfxGetInstanceHandle(),_T("IMG_DIALOG_BACK"));
	for(int i=0;i<3;i++){
		CString strStatus;
		strStatus.Format(_T("IMG_PROGRESS_STATUS_%d"),i);
		m_StatusImage[i].LoadImage(AfxGetInstanceHandle(),strStatus.GetString());
	}
}

BOOL CAutoUpdateDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CAutoUpdateDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	BuildBkDC();
	Invalidate(FALSE);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutoUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAutoUpdateDlg::OnStatusCallback(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCTSTR pszStatusText)
{
	switch( ulStatusCode )
	{
	case DMS_SENDREQUEST:
		{
			break;
		}
	case DMS_BEGINDOWNLOAD:
		{
			m_ulCurentLenght = ulProgressMax;
			m_Progress1.SetRange32(0, ulProgressMax/1024);
			m_Progress1.SetPos(0);
			if(m_status!=0){
				m_status=0;
				Invalidate(FALSE);  
			}
			break;
		}
	case DMS_DOWNLOADDATA:
		{
			CString strText;
			strText.Format(TEXT("�������� %s "), pszStatusText);
            m_StatusText.SetWindowText(strText);

			CString strCountText,strCountText2;
			strCountText.Format(TEXT("%d KB"), m_ulCurentLenght/1024);
            strCountText2.Format(TEXT("%d KB"), ulProgressMax/1024);
			m_CountText.SetWindowText(strCountText);
            m_CountText2.SetWindowText(strCountText2);

			m_Progress1.SetPos(ulProgress/1024);
			m_Progress2.SetPos(ulProgressMax/1024);

            int nLower=0,nUpper=0;
            m_Progress2.GetRange(nLower,nUpper);
			if(nUpper>nLower){
				CString strText3;
				strText3.Format(TEXT("%d%%"), 100*ulProgressMax/(1024*(nUpper-nLower)));
				m_PercentText.SetWindowText(strText3);
			}
			if(m_status!=1){
				m_status=1;
				Invalidate(FALSE);  
			}
			break;
		}
	case DMS_ENDDOWNLOADDATA:
		{
			m_ulCurentLenght = 0;
			//m_Progress1.SetPos(0);
			if(m_status!=2){
				m_status=2;
				Invalidate(FALSE);  
			}
			break;
		}
	case DMS_FINISHDOWNLOAD:
		{
			m_btCancel.EnableWindow(FALSE);
			break;
		}
	case DMS_CANCELDOWNLOAD:
		{
			SendMessage(WM_CLOSE);
			break;
		}
	case DMS_ENDSETUP:
		{
			//��������
			CHAR szModuleName[MAX_PATH]={0};
			GetModuleFileNameA(AfxGetInstanceHandle(), szModuleName, MAX_PATH);
			PathRemoveFileSpecA(szModuleName);
			strcat(szModuleName, ("\\GamePlaza.exe UPDATED"));
			WinExec(szModuleName, SW_SHOWDEFAULT);

			SendMessage(WM_CLOSE);
			break;
		}
	case DMS_ERROR:
		{
			CString strError = AfxGetApp()->GetProfileString(_T("messages"), _T("ErrorMsg"), _T("����ʧ�ܣ��������Ի��ߵ��ֶ����ص��ٷ��������°汾"));
			MessageBox(strError, _T("��ʾ"), MB_OK|MB_ICONINFORMATION);
			m_btRetry.EnableWindow(TRUE);
			//SendMessage(WM_CLOSE);
			break;
		}
	}
}

BOOL CAutoUpdateDlg::CheckVersionUpdate(CString& strUpdateURL)
{
	BOOL bHasUpdate = FALSE;
	//return TRUE;

	HRESULT hr = S_OK;
	IXMLHTTPRequestPtr pHttpRequest;
	IDispatchPtr pDispatch;
	MSXML2::IXMLDOMDocumentPtr pXmlDoc;
	MSXML2::IXMLDOMNodeListPtr pList;
	MSXML2::IXMLDOMElementPtr pChild;

	UINT nFileSize;
	CString strFileName, strFileVer, strMD5String,strFilePath;
	LONG lElementCount = 0L;
	CString strLog;
	strLog.Format(_T("\nUPDATEFAILED  CheckVersionUpdate"));
	OutputDebugString(strLog);

	try
	{
		hr = pHttpRequest.CreateInstance(TEXT("Msxml2.XMLHTTP.3.0"));
		if( FAILED(hr) )
			_com_issue_error(hr);
		DeleteUrlCacheEntry(strUpdateURL);
		hr = pHttpRequest->open(TEXT("GET"), (_bstr_t)strUpdateURL, false);
		strLog.Format(_T("\nUPDATEFAILED  GET ��strUpdateURL��%s"),strUpdateURL);
		OutputDebugString(strLog);

		if( FAILED(hr) ){
			CString strError = AfxGetApp()->GetProfileString(_T("messages"), _T("ErrorMsg"), _T("����ʧ�ܣ��뵽�ٷ��������°汾"));
			MessageBox(strError, _T("��ʾ"), MB_OK|MB_ICONINFORMATION);
            _com_issue_error(hr);
		}

		hr = pHttpRequest->send();
		strLog.Format(_T("\nUPDATEFAILED  send()"));
		OutputDebugString(strLog);

		if( FAILED(hr) )
			_com_issue_error(hr);

		if (pHttpRequest->Getstatus() != 200)
		{
			strLog.Format(_T("\nUPDATEFAILED  pHttpRequest->Getstatus():%d"),pHttpRequest->Getstatus());
			OutputDebugString(strLog);

			throw (0);
		}
		pDispatch = pHttpRequest->GetresponseXML();
		hr = pDispatch->QueryInterface(pXmlDoc.GetIID(), (void**)&pXmlDoc);
		strLog.Format(_T("\nUPDATEFAILED  QueryInterface"));
		OutputDebugString(strLog);

		if( FAILED(hr) )
			_com_issue_error(hr);

		pList = pXmlDoc->selectNodes("/manifest/filelist/file");
		lElementCount = pList->Getlength();

	
		strLog.Format(_T("\nUPDATEFAILED  �ļ���Ŀ:%d"),lElementCount);
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
			CString strRemoteFile=m_DownloadMgr.GetDownloadURL()+strFilePath+strFileName;
			CString strPathFileName=m_strCurrentDir+strFilePath+strFileName;
            BOOL iRet=PathFileExists(strPathFileName);
			CString strLog;
			strLog.Format(_T("\nUPDATEFAILED %s %s,%s"),(iRet?_T("�����ļ�"):_T("�������ļ�")),strPathFileName,strRemoteFile);
			OutputDebugString(strLog);
			if(iRet)
			{
				//Ч���ļ�,���MD5�벻��ͬ����������
				CString strLocalMD5String =CMD5Checksum::GetMD5(strPathFileName);
				CString strLog;
				strLog.Format(_T("\nUPDATEFAILED  ����MD5:%s Զ��MD5:%s %s"),strLocalMD5String,strMD5String,(strLocalMD5String==strMD5String?_T("��ͬ"):_T("����ͬ")));
				OutputDebugString(strLog);
				if ( strLocalMD5String!= strMD5String)
				{
					m_ulTotalLength += nFileSize;
					m_DownloadMgr.AddTask(strRemoteFile,strPathFileName);
					bHasUpdate = TRUE;
				}	
			}
			else
			{
				m_ulTotalLength += nFileSize;
				m_DownloadMgr.AddTask(strRemoteFile,strPathFileName);
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

void CAutoUpdateDlg::OnBnClickedCancel()
{
	//DestroyWindow();
    OnStatusCallback(0, 0, DMS_ENDSETUP, NULL);
	return;
	if ( m_DownloadMgr.GetStatus() == DMS_FINISHDOWNLOAD || m_DownloadMgr.GetStatus() == DMS_ERROR )
	{
		DestroyWindow();
	}
	else
	{
		m_DownloadMgr.CancelTask();
		m_btCancel.EnableWindow(FALSE);
	}
}
void CAutoUpdateDlg::OnBnClickedRetry()
{
	//��������
	m_DownloadMgr.Start();

}
void CAutoUpdateDlg::OnBnClickedSelf()
{
	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Update\\Update.ini"),m_strCurrentDir);

	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	//	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%sLogonLink4.aspx"),szPlatformLink);
	GetPrivateProfileString(TEXT("SERVER"),TEXT("BYSELF"),TEXT(""),szLogonLink,CountArray(szLogonLink),szFileName);

	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);
}
void CAutoUpdateDlg::OnClose()
{
	DestroyWindow();
}

LRESULT CAutoUpdateDlg::OnTransaprentBk( WPARAM wParam, LPARAM lParam )
{
	HDC hdc = ( HDC)wParam;
	HWND hwnd = ( HWND)lParam;
	CTransparentHelper::OnTranparentControl( m_pMemDC->GetSafeHdc(), (WPARAM)hdc, (LPARAM)hwnd);
	return TRUE;
}

void CAutoUpdateDlg::BuildBkDC()
{
	CDC* pDC;
	CDC MemDC;
	CBitmap* pOldBitmap;
	CRect rtClient;

	pDC = GetDC();
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject( &m_bmBack );

	GetClientRect(&rtClient);

	if ( m_pMemDC )
	{
		m_pMemDC->SelectObject( m_pOldBitmap);
		delete m_pBitmapMem;
		m_pBitmapMem = NULL;
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if ( m_pMemDC == NULL )
	{
		m_pMemDC = new CDC;
		m_pMemDC->CreateCompatibleDC(pDC);

		m_pBitmapMem = new CBitmap;
		m_pBitmapMem->CreateCompatibleBitmap(pDC, rtClient.Width(), rtClient.Height());
		m_pOldBitmap = (CBitmap*)m_pMemDC->SelectObject( m_pBitmapMem);
	}

	m_pMemDC->BitBlt( 0,0, rtClient.Width(), rtClient.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject( pOldBitmap );
	ReleaseDC(pDC);

	//when the parent dialog's background is rebuild, notify the child which has an transparent tag.
	CTransparentHelper::NotifyTransparentChild( GetSafeHwnd());
}