#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhanNeiXinXX.h"
#include "Bmputil.h"
#include "MessageDlg.h"

static const int wnd_width = 332;
static const int wnd_height = 264;

static const int btn_ok_x = 550;
static const int btn_ok_y = 373;

static const int btn_cancel_x = 591;
static const int btn_cancel_y = 5;

//玩法
static const int tip_wanfa_x = 12;
static const int tip_wanfa_y = 42;
static const int tip_wanfa_width = 670;
static const int tip_wanfa_height = 20;
static CRect rc_title(tip_wanfa_x, 
						   tip_wanfa_y, 
						   tip_wanfa_x + tip_wanfa_width, 
						   tip_wanfa_y + tip_wanfa_height);

//注数
static const int tip_zhushu_x = 130;
static const int tip_zhushu_y = 87;
static const int tip_zhushu_width = 70;
static const int tip_zhushu_height = 20;
static CRect rc_zhushu(tip_zhushu_x, 
						   tip_zhushu_y, 
						   tip_zhushu_x + tip_zhushu_width, 
						   tip_zhushu_y + tip_zhushu_height);
//注数
static const int tip_jine_x = 110;
static const int tip_jine_y = 246;
static const int tip_jine_width = 470;
static const int tip_jine_height = 20;
static CRect rc_jine(tip_jine_x, 
						   tip_jine_y, 
						   tip_jine_x + tip_jine_width, 
						   tip_jine_y + tip_jine_height);

//彩票金额
static const int tip_danzhujine_x = 110;
static const int tip_danzhujine_y = 43;
static const int tip_danzhujine_width = 170;
static const int tip_danzhujine_height = 20;
static CRect rc_danzhujine(tip_danzhujine_x, 
						   tip_danzhujine_y, 
						   tip_danzhujine_x + tip_danzhujine_width, 
						   tip_danzhujine_y + tip_danzhujine_height);
//棋牌金额
static const int tip_qipaijine_x = 110;
static const int tip_qipaijine_y = 71;
static const int tip_qipaijine_width = 170;
static const int tip_qipaijine_height = 20;
static CRect rc_qipaijine(tip_qipaijine_x, 
						   tip_qipaijine_y, 
						   tip_qipaijine_x + tip_danzhujine_width, 
						   tip_qipaijine_y + tip_danzhujine_height);

//总金额
static const int tip_zongjine_x = 315;
static const int tip_zongjine_y = 304;
static const int tip_zongjine_width = 170;
static const int tip_zongjine_height = 30;
static CRect rc_zongjine(tip_zongjine_x, 
						   tip_zongjine_y, 
						   tip_zongjine_x + tip_zongjine_width, 
						   tip_zongjine_y + tip_zongjine_height);

static const int rich_content_x = 12;
static const int rich_content_y = 114;
static const int rich_content_span_x = 617;
static const int rich_content_span_y = 239;
// CZhanNeiXinXX 对话框

IMPLEMENT_DYNAMIC(CZhanNeiXinXX, CDialog)

CZhanNeiXinXX::CZhanNeiXinXX(CWnd* pParent /*=NULL*/)
	: CDialog(CZhanNeiXinXX::IDD, pParent)
	, m_bmpBk(NULL)
	,m_fJine(0.0)
	,m_fJiaoYiJine(0.0)
{
	m_nType=0;
}

CZhanNeiXinXX::~CZhanNeiXinXX()
{
	if (m_bmpBk != NULL)
	{
		SafeDelete( m_bmpBk);
	}
	if (m_bmpTile!= NULL)
	{
		SafeDelete( m_bmpTile);
	}
}

void CZhanNeiXinXX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);

	DDX_Control(pDX, IDC_RICHEDIT_CONTENT, m_richContent);
}


BEGIN_MESSAGE_MAP(CZhanNeiXinXX, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	

	ON_WM_LBUTTONDOWN()

END_MESSAGE_MAP()

void CZhanNeiXinXX::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//使窗口可以拖动
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}
BOOL CZhanNeiXinXX::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);
	switch(nCommandID)
	{
	default:
		{
			return __super::OnCommand(wParam,lParam);
		}
	}


	return  __super::OnCommand(wParam,lParam);;
}

void CZhanNeiXinXX::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);

	CDC cacheDC;
	cacheDC.CreateCompatibleDC(&dc);
	CBitmap cacheBmp;
	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

	Graphics graphics(cacheDC.m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);
	graphics.DrawImage(m_bmpTile, Rect((rect.Width()-m_bmpTile->GetWidth())/2, 5, m_bmpTile->GetWidth(), m_bmpTile->GetHeight()), 0, 0, m_bmpTile->GetWidth(), m_bmpTile->GetHeight(), UnitPixel);

	cacheDC.SetBkMode(TRANSPARENT);
	CFont *pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

	CString strTmp;
	strTmp.Format(L"%s",strTitle);
	if(strTmp.GetLength()>60)
	{
		strTmp.Format(L"%s...",strTitle.Left(60));

	}
	cacheDC.DrawText(strTmp,rc_title,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CRect rcSend;
	rcSend.CopyRect(rc_title);

	rcSend.top+=30;
	rcSend.bottom+=30;
	if(m_nType == 1)
		strTmp.Format(L"发件人:%s",strName);
	else
		strTmp.Format(L"收件人:%s",strName);
	cacheDC.DrawText(strTmp,rcSend,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	rcSend.top+=20;
	rcSend.bottom+=20;
	strTmp.Format(L"时间:%s",strTime);
	cacheDC.DrawText(strTmp,rcSend,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CZhanNeiXinXX::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CZhanNeiXinXX::SetContent(CString szTitle,CString szName,CString szContent,CString szTime,int nType)
{
	strTitle.Format(L"%s",szTitle);
	strName.Format(L"%s",szName);
	strContent.Format(L"%s",szContent);
	strTime.Format(L"%s",szTime);
	m_nType = nType;
	return;
}

VOID CZhanNeiXinXX::OnOK()
{

	return CDialog::OnOK();
}
VOID CZhanNeiXinXX::OnCancel()
{
	return CDialog::OnCancel();
}
BOOL CZhanNeiXinXX::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk == NULL)
	{
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\Message\\shouxinxiangbk.png"));

		if(m_nType == 1)
		{
			m_bmpTile = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\Message\\shouxinneirong_1.png"));
		}
		else
		{
			m_bmpTile = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\Message\\shouxinneirong_2.png"));

		}
		m_font.CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

		m_btnfont.CreateFont(20,0,0,0,FW_NORMAL,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("Arial"));

	}
	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\huifu.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\close_bt.png"));
	m_richContent.SetBackgroundColor(false,RGB(217,215,213));

	//m_richContent.LimitText(-1);
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(88, 78, 77);
	m_richContent.SetDefaultCharFormat(cf);
	m_richContent.SetWindowText(strContent);
	m_richContent.EnableWindow(FALSE);
	m_richContent.SetFont(&m_font);
	SetWindowPos(NULL,0,0,m_bmpBk->GetWidth(),m_bmpBk->GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CZhanNeiXinXX::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}
	if(m_richContent.GetSafeHwnd()!=NULL)
	{
		m_richContent.SetWindowPos(NULL, rich_content_x, rich_content_y, rich_content_span_x, rich_content_span_y, SWP_NOZORDER);
	}

	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
		if(m_nType == 2 ||strName == _T("系统消息"))
		{
			m_btnOK.ShowWindow(SW_HIDE);
		}

	}

	if(m_btnCancel.GetSafeHwnd() != NULL)
	{
		m_btnCancel.SetWindowPos(NULL, btn_cancel_x, btn_cancel_y, m_btnCancel.Width(), m_btnCancel.Height(), SWP_NOZORDER);
	}


}

