#include "StdAfx.h"
#include "Resource.h"
#include "PlazaViewItem.h"
#include "GlobalUnits.h"
#include "WndTopNews.h"
#include "MessageDlg.h"
//////////////////////////////////////////////////////////////////////////////////

//��������
#define MIN_TYPE_ID					1									//��С����
#define MAX_TYPE_ID					6									//�������
#define IDC_FRAME_ADD 200
static int nTimer_Fresh=				100;
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndTopNews, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()

END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWndTopNews::CWndTopNews()
{
	ImageItemBack.LoadFromResource(AfxGetInstanceHandle(),IDB_ITEM_BACK);
	//tagEncircleResource	EncircleItemFrame;
	//EncircleItemFrame.pszImageTL=MAKEINTRESOURCE(IDB_ITEM_NEWS_TL);
	//EncircleItemFrame.pszImageTM=MAKEINTRESOURCE(IDB_ITEM_NEWS_TM);
	//EncircleItemFrame.pszImageTR=MAKEINTRESOURCE(IDB_ITEM_NEWS_TR);
	//m_ItemFrameEncircle.InitEncircleResource(EncircleItemFrame,AfxGetInstanceHandle());
	m_nNewsLength = 0;
	m_nFreshLength = 0;
	m_nNewsX = 0;
	return;
}
//λ����Ϣ
VOID CWndTopNews::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	m_webbrowser.SetWindowPos(NULL,0,0,cx,cy+10,SWP_NOZORDER);
	m_webbrowser.EnableWindow(FALSE);
	m_webbrowser.ShowWindow(SW_HIDE);
	return;
}
//������Ϣ
INT CWndTopNews::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;


	//������ť
	CRect rcCreate(0,0,0,0);

	m_font.CreateFont(17, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	m_webbrowser.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_FRAME_ADD);
	m_webbrowser.ShowScrollBar(SB_HORZ, FALSE);

	TCHAR szTodayURL[126]={0};
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	//��ȡ����
	CWHIniData IniData;
	IniData.SetIniFilePath(szFileName);

	IniData.ReadEncryptString(TEXT("PlazaUrl"),TEXT("PlazaTopLot"),TEXT("http://192.168.0.105/WinFromBanner/winlottery.aspx"),szTodayURL,CountArray(szTodayURL));

	m_webbrowser.Navigate(szTodayURL,NULL,NULL,NULL,NULL);
	return 0;
}
//��������
CWndTopNews::~CWndTopNews()
{

}

void CWndTopNews::SetTopNews(CString strNews)
{
	m_strNews = strNews;
	
	m_nNewsLength = m_strNews.GetLength();
	m_nFreshLength = m_nNewsLength;
	SetTimer(nTimer_Fresh,100,NULL);
	Invalidate(TRUE);
	return;
}
void CWndTopNews::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == nTimer_Fresh)
	{
		m_nNewsX -= 2;
		int nAllNewsLength = (m_nNewsLength*5);
		if((m_nNewsX + nAllNewsLength)<=0)
		{
			//��ȡλ��
			CRect rcClient;
			GetClientRect(&rcClient);

			m_nNewsX = rcClient.Width();
		}
		Invalidate(TRUE);
	}
	__super::OnTimer(nIDEvent);

}
//�滭����
BOOL CWndTopNews::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//�����豸
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	pBufferDC->SetBkMode(TRANSPARENT);

	ImageItemBack.DrawImageTile(pBufferDC,30,0,990,26);
	//m_ItemFrameEncircle.DrawEncircleFrame(pBufferDC,rcClient);

	CRect rcNews;
	rcNews.left = rcClient.left + m_nNewsX;
	rcNews.top = rcClient.top;
	rcNews.right = rcClient.right+m_nNewsLength*20+ m_nNewsX;
	rcNews.bottom = rcClient.bottom;
	CFont* pOldFont = pBufferDC->SelectObject(&m_font);

	COLORREF oldTextClr = pBufferDC->SetTextColor(RGB(242, 223, 196));
	pBufferDC->DrawText( m_strNews, &rcNews, DT_LEFT|DT_WORDBREAK|DT_SINGLELINE|DT_VCENTER);
	CPngImage imgTop;
	imgTop.LoadImage(AfxGetInstanceHandle(),TEXT("IMG_TOP10"));
	imgTop.DrawImage(pBufferDC,-8,0);
	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);


	return TRUE;
}
//��Ϣ����
BOOL CWndTopNews::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}
void CWndTopNews::DrawMultLineText(CDC *pDC, CRect rect, int nRowDis, UINT nFromat, CString strText)
{

	if( strText.GetLength() <= 0 )
		return;

	WCHAR* pText = strText.GetBuffer(strText.GetLength());
	int nCount = strText.GetLength();
	CRect rtChar;
	CSize size = pDC->GetTextExtent(pText + 0, 1);
	int nRowHeight = size.cy + nRowDis;
	rtChar.top = rect.top;
	rtChar.left = rect.left;
	rtChar.bottom  = rtChar.top + nRowDis + size.cy;
	rtChar.right  = rtChar.left + size.cx; 
	CString strChar;
	for (int nCharIndex = 0; nCharIndex < nCount; nCharIndex++)
	{
		if( rtChar.right > rect.right )
		{
			rtChar.top = rtChar.bottom;
			rtChar.bottom += nRowHeight;
			size = pDC->GetTextExtent(pText + nCharIndex, 1);
			rtChar.left = rect.left;
			rtChar.right = rtChar.left + size.cx;
			if( rtChar.bottom > rect.bottom )
				break;
		}
		strChar = pText[nCharIndex];
		pDC->DrawText(strChar, rtChar, nFromat);
		size = pDC->GetTextExtent(pText + nCharIndex + 1, 1);
		rtChar.left = rtChar.right;
		rtChar.right += size.cx;
	}
}

//////////////////////////////////////////////////////////////////////////////////
