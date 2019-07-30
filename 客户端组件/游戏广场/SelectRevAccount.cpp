#include "stdafx.h"
#include "GamePlaza.h"
#include "SelectRevAccount.h"
#include "MessageDlg.h"

static const int kj_small_haoma_x = 110;
static const int kj_small_haoma_y = 40;
static const int kj_small_haoma_col_span = 3;
static const int kj_small_haoma_row_span = 11;

static const int btn_x = 26;
static const int btn_y = 86;
static const int btn_span_cx = 219;
static const int btn_span_cy = 27;
static const int btn_cx = 152;
static const int btn_cy = 24;

static const int bt_close_x = 610;
static const int bt_close_y = 5;
static CRect rcClose(bt_close_x,bt_close_y,bt_close_x+29,bt_close_y+25);

static const int bt_queding_cx = 326;
static const int bt_queding_cy = 99;

static const int page_x = 282;
static const int page_y = 375;
static const int page_cx = 60;
static const int page_cy = 20;

static CRect rcPage(page_x,page_y,page_x+page_cx,page_y+page_cy);

IMPLEMENT_DYNAMIC(CSelectRevAccount, CDialog)
#define  IDC_XIAJI_USER			10000
CSelectRevAccount::CSelectRevAccount(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectRevAccount::IDD, pParent)
	, m_btClose(NULL)
	, m_nCloseIndex(0)
{
	m_font.CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	mapName.clear();
	mapIndex.clear();
	m_nPage = 1;
	m_nMaxPage = 1;
}

CSelectRevAccount::~CSelectRevAccount()
{
	if (m_btClose != NULL) 
	{
		delete m_btClose;
	}

}
void CSelectRevAccount::AdjustButton()
{

  	for (int i = 0;i < CountArray(m_btnImgRioUser);i++)
  	{
  		CWnd* pWnd = GetDlgItem(IDC_RIO_BUTTON1+i);
  		if (pWnd != NULL) 
  		{
  			int nX = i%3;
  			int nY = i/3;
  
  			CString strLog;
  			strLog.Format(L"SELECTACT x:%d,y:%d",btn_x+nX*btn_span_cx, btn_y+nY*btn_span_cy);
  			OutputDebugString(strLog);
  			pWnd->SetWindowPos(NULL,btn_x+nX*btn_span_cx, btn_y+nY*btn_span_cy, btn_cx, btn_cy, SWP_NOZORDER);
  		}
  	}

}
//鼠标消息
VOID CSelectRevAccount::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);
	if(rcClose.PtInRect(Point))
	{
		int nIndex = m_nCloseIndex;
		m_nCloseIndex = 3;
		if(nIndex == 0)
			InvalidateRect(rcClose);
	}
	else
	{
		int nIndex = m_nCloseIndex;
		m_nCloseIndex = 0;
		if(nIndex == 3)
			InvalidateRect(rcClose);
	}
	return ;
}
void CSelectRevAccount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_PRE_PAGE, m_btnPrePage);
	DDX_Control(pDX, IDC_BTN_NEXT_PAGE, m_btnNextPage);
	DDX_Control(pDX, IDC_BTN_SELALL, m_btnSelectAll);

	DDX_Control(pDX, IDC_RIO_BUTTON1, m_btnImgRioUser[0]);
	DDX_Control(pDX, IDC_RIO_BUTTON2, m_btnImgRioUser[1]);
	DDX_Control(pDX, IDC_RIO_BUTTON3, m_btnImgRioUser[2]);
	DDX_Control(pDX, IDC_RIO_BUTTON4, m_btnImgRioUser[3]);
	DDX_Control(pDX, IDC_RIO_BUTTON5, m_btnImgRioUser[4]);
	DDX_Control(pDX, IDC_RIO_BUTTON6, m_btnImgRioUser[5]);
	DDX_Control(pDX, IDC_RIO_BUTTON7, m_btnImgRioUser[6]);
	DDX_Control(pDX, IDC_RIO_BUTTON8, m_btnImgRioUser[7]);
	DDX_Control(pDX, IDC_RIO_BUTTON9, m_btnImgRioUser[8]);
	DDX_Control(pDX, IDC_RIO_BUTTON10, m_btnImgRioUser[9]);
	DDX_Control(pDX, IDC_RIO_BUTTON11, m_btnImgRioUser[10]);
	DDX_Control(pDX, IDC_RIO_BUTTON12, m_btnImgRioUser[11]);
	DDX_Control(pDX, IDC_RIO_BUTTON13, m_btnImgRioUser[12]);
	DDX_Control(pDX, IDC_RIO_BUTTON14, m_btnImgRioUser[13]);
	DDX_Control(pDX, IDC_RIO_BUTTON15, m_btnImgRioUser[14]);
	DDX_Control(pDX, IDC_RIO_BUTTON16, m_btnImgRioUser[15]);
	DDX_Control(pDX, IDC_RIO_BUTTON17, m_btnImgRioUser[16]);
	DDX_Control(pDX, IDC_RIO_BUTTON18, m_btnImgRioUser[17]);
	DDX_Control(pDX, IDC_RIO_BUTTON19, m_btnImgRioUser[18]);
	DDX_Control(pDX, IDC_RIO_BUTTON20, m_btnImgRioUser[19]);
	DDX_Control(pDX, IDC_RIO_BUTTON21, m_btnImgRioUser[20]);
	DDX_Control(pDX, IDC_RIO_BUTTON22, m_btnImgRioUser[21]);
	DDX_Control(pDX, IDC_RIO_BUTTON23, m_btnImgRioUser[22]);
	DDX_Control(pDX, IDC_RIO_BUTTON24, m_btnImgRioUser[23]);
	DDX_Control(pDX, IDC_RIO_BUTTON25, m_btnImgRioUser[24]);
	DDX_Control(pDX, IDC_RIO_BUTTON26, m_btnImgRioUser[25]);
	DDX_Control(pDX, IDC_RIO_BUTTON27, m_btnImgRioUser[26]);
	DDX_Control(pDX, IDC_RIO_BUTTON28, m_btnImgRioUser[27]);
	DDX_Control(pDX, IDC_RIO_BUTTON29, m_btnImgRioUser[28]);
	DDX_Control(pDX, IDC_RIO_BUTTON30, m_btnImgRioUser[29]);

	DDX_Control(pDX, IDC_REV_SEARCH, m_edSearch);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
}


BEGIN_MESSAGE_MAP(CSelectRevAccount, CDialog)
	ON_WM_PAINT()
	//ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDOK, &CSelectRevAccount::OnBnClickedOk)

	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CSelectRevAccount::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CSelectRevAccount::OnBnClickedBtnNextPage)
	ON_BN_CLICKED(IDC_BTN_SELALL, &CSelectRevAccount::OnBnClickedBtnSelAll)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CSelectRevAccount::OnBnClickedBtnSearch)
END_MESSAGE_MAP()


// CSelectRevAccount 消息处理程序
BOOL CSelectRevAccount::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	Graphics graphics(pDC->m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);
	pDC->SetBkMode(TRANSPARENT);
	COLORREF OldColor = pDC->SetTextColor(RGB(114,114,114));
	CString strPage;
	strPage.Format(L"%d/%d",m_nPage,m_nMaxPage);
	pDC->DrawText(strPage,strPage.GetLength(),rcPage,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	DrawCloseBtn(pDC,graphics);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	pDC->SetTextColor(OldColor);
	return TRUE;
}

void CSelectRevAccount::OnPaint()
{
	CPaintDC dc(this); 


	// 不为绘图消息调用 CDialog::OnPaint()
}

void CSelectRevAccount::DrawCloseBtn(CDC* pDC, Graphics& graphics)
{
	Rect rect(rcClose.left,rcClose.top,rcClose.Width(),rcClose.Height());
	graphics.DrawImage(m_btClose, rect, (m_nCloseIndex)*rcClose.Width(), 0, rcClose.Width(), rcClose.Height(), UnitPixel);

	return;
}

void CSelectRevAccount::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//使窗口可以拖动
	if(rcClose.PtInRect(point))
	{
		CWnd* pWnd = GetParent();
		if(pWnd!=NULL)
		{
			pWnd->SendMessage(IDM_SELECT_USER,32,0);
		}
		PostMessage(WM_CLOSE);
		return;
	}
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}
bool CSelectRevAccount::IsPushed(int nIndex)
{
	if(nIndex>29)
		return false;
	return m_btnImgRioUser[nIndex].GetPushed();
}
void CSelectRevAccount::SetPushed(int nIndex)
{
	if(nIndex>29)
		return;
	int nSize = mapIndex.size();
	mapIndex[nSize] = nIndex;

	m_btnImgRioUser[nIndex].SetPushed(true);
}
void CSelectRevAccount::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	if(m_btnOK.GetSafeHwnd()!=NULL)
	{

		int nY = 8;
		m_btnOK.SetWindowPos(NULL, (cx-bt_queding_cx)/2+370, btn_y+nY*btn_span_cy-5+68, bt_queding_cx, bt_queding_cy, SWP_NOZORDER);

	}
	if(m_btnCancel.GetSafeHwnd()!=NULL)
	{
		int nY = 13;
		m_btnCancel.SetWindowPos(NULL, (cx/2-bt_queding_cx)/2+cx/2, btn_y+nY*btn_span_cy-5, bt_queding_cx, bt_queding_cy, SWP_NOZORDER);
	}

	if(m_edSearch.GetSafeHwnd()!=NULL)
	{
		m_edSearch.SetWindowPos(NULL,27,57,141,20,SWP_NOZORDER);
	}

	if(m_btnSearch.GetSafeHwnd()!=NULL)
	{
		m_btnSearch.SetWindowPos(NULL,351-177,62-6,m_btnSearch.Width(),m_btnSearch.Height(),SWP_NOZORDER);
	}
	if(m_btnPrePage.GetSafeHwnd()!=NULL)
	{
		int nY = 8;
		m_btnPrePage.SetWindowPos(NULL,401-177, btn_y+nY*btn_span_cy+73,m_btnPrePage.Width(),m_btnPrePage.Height(),SWP_NOZORDER);
	}

	if(m_btnNextPage.GetSafeHwnd()!=NULL)
	{
		int nY = 8;
		m_btnNextPage.SetWindowPos(NULL,501-161, btn_y+nY*btn_span_cy+73,m_btnNextPage.Width(),m_btnNextPage.Height(),SWP_NOZORDER);
	}

	if(m_btnSelectAll.GetSafeHwnd()!=NULL)
	{
		int nY = 8;
		m_btnSelectAll.SetWindowPos(NULL,46, btn_y+nY*btn_span_cy+76,m_btnSelectAll.Width(),m_btnSelectAll.Height(),SWP_NOZORDER);
	}
	AdjustButton();
}

BOOL CSelectRevAccount::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);
	if(nCommandID>=IDC_RIO_BUTTON1 && nCommandID<=IDC_RIO_BUTTON30)
	{
		int nIndex  = nCommandID-IDC_RIO_BUTTON1;
		bool bPushed = m_btnImgRioUser[nIndex].GetPushed();
		m_btnImgRioUser[nIndex].SetPushed(!bPushed);

		CWnd* pWnd = GetParent();
		if(pWnd!=NULL)
		{
			pWnd->SendMessage(IDM_SELECT_USER,nIndex,!bPushed);
		}

	}
	return  __super::OnCommand(wParam,lParam);;
}

void CSelectRevAccount::SetButtonText(int nIndex,CString strName,int nPage,int nMaxPage)
{
 	if(nIndex>29)
 		return;
	mapName[nIndex] = strName;
	CString strLog;
	strLog.Format(L"SELECTACT index:%d,Name:%s",nIndex,strName);
	OutputDebugString(strLog);

	m_nPage = nPage;
	m_nMaxPage = nMaxPage;

	m_btnImgRioUser[nIndex].SetWindowText(strName);
	m_btnImgRioUser[nIndex].ShowWindow(SW_SHOW);

	InvalidateRect(&rcPage);
// 	m_btnImgRioUser[nIndex].SetWindowText(strName);
}
BOOL CSelectRevAccount::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\Message\\sxr_bj.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\quxiao_bt.png"));
	m_btnPrePage.SetImage(CBmpUtil::GetExePath() + _T("skin\\syy_bt.png"));
	m_btnNextPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\xyy_bt.png"));
	m_btnSelectAll.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\quanxuan.png"));

	//m_btnOK.ShowWindow(SW_HIDE);
	//m_btnCancel.ShowWindow(SW_HIDE);
	m_btClose = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\close_bt.png"));
	CRect rcBtn(0,0,0,0);
	for (int i = 0;i < CountArray(m_btnImgRioUser);i++)
	{
		//m_btnImgRioUser[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcBtn,this,IDC_XIAJI_USER+i);
		m_btnImgRioUser[i].SetBkImage(CBmpUtil::GetExePath() + _T("skin\\Message\\sxr_btn.png"));
		m_btnImgRioUser[i].ShowWindow(SW_HIDE);
		m_btnImgRioUser[i].SetTextColor(RGB(114,114,114));
	}

	m_btnSearch.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\sosuo.png"));
// 	int nSize = mapName.size();
// 	for (map<int,CString>::iterator pos = mapName.begin();pos != mapName.end();++pos)
// 	{
// 		CString strName = pos->second;
// 
// 		if(pos->first >29)
// 			break;
// 		m_btnImgRioUser[pos->first].SetWindowText(strName);
// 		m_btnImgRioUser[pos->first].ShowWindow(SW_SHOW);
// 	}

	m_edSearch.SetEnableColor(RGB(189,186,185),RGB(255,255,255),RGB(136,134,135));
	m_edSearch.SetFont(&m_font);
	for (map<int ,int>::iterator pos = mapIndex.begin();pos != mapIndex.end();++pos)
	{
		if(pos->second > 29)
			continue;
		m_btnImgRioUser[pos->second].SetPushed(TRUE);
	}
	SetWindowPos(NULL,0,0,m_bmpBk->GetWidth(),m_bmpBk->GetHeight(),SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE);
	AdjustButton();
	CenterWindow();
	return TRUE;  
}

void CSelectRevAccount::HideAllButton()
{	

	for (int i = 0;i < CountArray(m_btnImgRioUser);i++)
	{
		m_btnImgRioUser[i].ShowWindow(SW_HIDE);
		m_btnImgRioUser[i].SetPushed(false);
	}

}
void CSelectRevAccount::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();

	CDialog::OnOK();
}


void CSelectRevAccount::OnBnClickedOk()
{
	m_btnSelectAll.SetPushed(false);
	OnOK();
}
void CSelectRevAccount::OnBnClickedBtnSelAll()
{
	bool bSel = m_btnSelectAll.GetChecked();
	m_btnSelectAll.SetPushed(!bSel);
	for (int i =0 ;i < CountArray(m_btnImgRioUser);i++)
	{
		m_btnImgRioUser[i].SetPushed(!bSel);
	}
	CWnd *pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->SendMessage(IDM_SELECT_USER,31,!bSel);
	}
}
void CSelectRevAccount::OnBnClickedBtnNextPage()
{
	if(m_nPage>=m_nMaxPage)
	{
		MyMessageBox(L"已经是最后一页了！");
		return;
	}
	m_nPage++;
	HideAllButton();

	CWnd *pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->SendMessage(IDM_NEXT_PAGE,m_nPage,0);
	}
}
void CSelectRevAccount::OnBnClickedBtnSearch()
{
// 	CString strSearch;
// 	m_edSearch.GetWindowText(strSearch);
// 	if(strSearch.IsEmpty())
// 	{
// 		MyMessageBox(L"请输入要查询的用户名！");
// 		return;
// 	}
	m_btnSelectAll.SetPushed(false);
	HideAllButton();
	CWnd *pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->SendMessage(IDM_NEXT_PAGE,1,2);
	}

}
void CSelectRevAccount::OnBnClickedBtnPrePage()
{
	if(m_nPage<=1)
	{
		MyMessageBox(L"已经是第一页了！");
		return;
	}
	HideAllButton();
	m_nPage--;
	CWnd *pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->SendMessage(IDM_NEXT_PAGE,m_nPage,0);
	}
	
}


