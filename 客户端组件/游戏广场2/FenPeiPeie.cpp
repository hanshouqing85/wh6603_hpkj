#include "stdafx.h"
#include "GamePlaza.h"
#include "FenPeiPeie.h"
#include "Bmputil.h"


static const int wnd_width = 332;
static const int wnd_height = 264;

static const int btn_ok_x = 105;
static const int btn_ok_y = 250;

static const int btn_cancel_x = 311;
static const int btn_cancel_y = 250;

static const int edit_peie1_x = 192;
static const int edit_peie1_y = 52;
static const int edit_peie2_x = 192;
static const int edit_peie2_y = 92;
static const int edit_peie3_x = 192;
static const int edit_peie3_y = 132;

static const int edit_peie4_x = 192;
static const int edit_peie4_y = 172;
static const int edit_peie5_x = 192;
static const int edit_peie5_y = 212;

static const int edit_width = 60;
static const int edit_height = 20;

//玩法
static const int tip_wanfa_x = 102;
static const int tip_wanfa_y = 52;
static const int tip_wanfa_width = 170;
static const int tip_wanfa_height = 20;
static CRect rc_wanfa(tip_wanfa_x, 
						   tip_wanfa_y, 
						   tip_wanfa_x + tip_wanfa_width, 
						   tip_wanfa_y + tip_wanfa_height);

//注数
static const int tip_zhushu_x = 102;
static const int tip_zhushu_y = 92;
static const int tip_zhushu_width = 170;
static const int tip_zhushu_height = 20;
static CRect rc_zhushu(tip_zhushu_x, 
						   tip_zhushu_y, 
						   tip_zhushu_x + tip_zhushu_width, 
						   tip_zhushu_y + tip_zhushu_height);

//单注金额
static const int tip_danzhujine_x = 102;
static const int tip_danzhujine_y = 132;
static const int tip_danzhujine_width = 170;
static const int tip_danzhujine_height = 20;
static CRect rc_danzhujine(tip_danzhujine_x, 
						   tip_danzhujine_y, 
						   tip_danzhujine_x + tip_danzhujine_width, 
						   tip_danzhujine_y + tip_danzhujine_height);

//总金额
static const int tip_zongjine_x = 102;
static const int tip_zongjine_y = 172;
static const int tip_zongjine_width = 170;
static const int tip_zongjine_height = 20;
static CRect rc_zongjine(tip_zongjine_x, 
						   tip_zongjine_y, 
						   tip_zongjine_x + tip_zongjine_width, 
						   tip_zongjine_y + tip_zongjine_height);
//总金额
static const int tip_lastzongjine_x = 102;
static const int tip_lastzongjine_y = 212;
static const int tip_lastzongjine_width = 170;
static const int tip_lastzongjine_height = 20;
static CRect rc_lastzongjine(tip_lastzongjine_x, 
						   tip_lastzongjine_y, 
						   tip_lastzongjine_x + tip_zongjine_width, 
						   tip_lastzongjine_y + tip_zongjine_height);

// CFenPeiPeie 对话框

IMPLEMENT_DYNAMIC(CFenPeiPeie, CDialog)

CFenPeiPeie::CFenPeiPeie(CWnd* pParent /*=NULL*/)
	: CDialog(CFenPeiPeie::IDD, pParent)
	, m_bmpBk(NULL)
	, m_zhushu(0)
	, m_gamekind(_T("重庆彩"))
	, m_moshi(MoShi_Yuan)
{

}

CFenPeiPeie::~CFenPeiPeie()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CFenPeiPeie::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_PEIE_1, m_editPeie1);
	DDX_Control(pDX, IDC_EDIT_PEIE_2, m_editPeie2);
	DDX_Control(pDX, IDC_EDIT_PEIE_3, m_editPeie3);
	DDX_Control(pDX, IDC_EDIT_PEIE_4, m_editPeie4);
	DDX_Control(pDX, IDC_EDIT_PEIE_5, m_editPeie5);
}


BEGIN_MESSAGE_MAP(CFenPeiPeie, CDialog)
	ON_WM_LBUTTONDOWN()

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFenPeiPeie 消息处理程序
void CFenPeiPeie::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//使窗口可以拖动
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}
VOID CFenPeiPeie::OnOK()
{
	CString strPeie1;
	m_editPeie1.GetWindowText(strPeie1);

	m_nSetPeie1 = _ttoi(strPeie1);
	CString strPeie2;
	m_editPeie2.GetWindowText(strPeie2);
	m_nSetPeie2 = _ttoi(strPeie2);
	CString strPeie3;
	m_editPeie3.GetWindowText(strPeie3);
	m_nSetPeie3 = _ttoi(strPeie3);
	CString strPeie4;
	m_editPeie4.GetWindowText(strPeie4);
	m_nSetPeie4 = _ttoi(strPeie4);
	CString strPeie5;
	m_editPeie5.GetWindowText(strPeie5);
	m_nSetPeie5 = _ttoi(strPeie5);
	CDialog::OnOK();
}
void CFenPeiPeie::OnPaint()
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

	//Graphics graphics(cacheDC.m_hDC);

	//graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	m_ImageBack.DrawImage(&cacheDC,0,0);
	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

 	CString strTmp;
	CRect rcFenpei;
	if(m_fFandian > 0.030000009)
	{
		strTmp.Format(L"3.0配额分配");
		cacheDC.DrawText(strTmp, rc_wanfa, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		rcFenpei.CopyRect(rc_wanfa);

		rcFenpei.left += 158;
		rcFenpei.right += 158;
		strTmp.Format(L"您的分配剩余配额%d",m_nPeieSy1);
		cacheDC.DrawText(strTmp, rcFenpei, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	}
	if(m_fFandian > 0.029999999)
	{
		strTmp.Format(L"2.9配额分配");
		cacheDC.DrawText(strTmp, rc_zhushu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		rcFenpei.CopyRect(rc_zhushu);

		rcFenpei.left += 158;
		rcFenpei.right += 158;
		strTmp.Format(L"您的分配剩余配额%d",m_nPeieSy2);
		cacheDC.DrawText(strTmp, rcFenpei, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	}
	if(m_fFandian > 0.0279999)
	{
		strTmp.Format(L"2.8配额分配");
		cacheDC.DrawText(strTmp, rc_danzhujine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		rcFenpei.CopyRect(rc_danzhujine);

		rcFenpei.left += 158;
		rcFenpei.right += 158;
		strTmp.Format(L"您的分配剩余配额%d",m_nPeieSy3);
		cacheDC.DrawText(strTmp, rcFenpei, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	}
	if(m_fFandian > 0.0279999)
	{
		strTmp.Format(L"2.7配额分配");
		cacheDC.DrawText(strTmp, rc_zongjine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		rcFenpei.CopyRect(rc_zongjine);

		rcFenpei.left += 158;
		rcFenpei.right += 158;
		strTmp.Format(L"您的分配剩余配额%d",m_nPeieSy4);
		cacheDC.DrawText(strTmp, rcFenpei, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	}
 	if(m_fFandian > 0.0279999)
 	{
 		strTmp.Format(L"2.6配额分配");
 		cacheDC.DrawText(strTmp, rc_lastzongjine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
 		rcFenpei.CopyRect(rc_lastzongjine);
 
 		rcFenpei.left += 158;
 		rcFenpei.right += 158;
 		strTmp.Format(L"您的分配剩余配额%d",m_nPeieSy5);
 		cacheDC.DrawText(strTmp, rcFenpei, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
 
 	}

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CFenPeiPeie::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CFenPeiPeie::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\qrtz.png"));

	m_ImageBack.LoadImage(CBmpUtil::GetExePath() + _T("skin\\fppe.png"));
	m_font.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\quxiao_bt.png"));

	SetWindowPos(NULL, 0, 0, m_ImageBack.GetWidth(), m_ImageBack.GetHeight(), SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow();

	m_editPeie1.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editPeie2.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editPeie3.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editPeie4.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editPeie5.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));

	m_editPeie1.SetFont(&m_font);
	m_editPeie2.SetFont(&m_font);
	m_editPeie3.SetFont(&m_font);
	m_editPeie4.SetFont(&m_font);
	m_editPeie5.SetFont(&m_font);

	if(m_fFandian < 0.031)
	{
		m_editPeie1.ShowWindow(SW_HIDE);
	}
	if(m_fFandian < 0.02999999999)
	{
		m_editPeie2.ShowWindow(SW_HIDE);
	}
// 	if(m_fFandian < 0.02899999999)
// 	{
// 		m_editPeie3.ShowWindow(SW_HIDE);
// 	}
	if(m_fFandian < 0.02799999999)
	{
		m_editPeie3.ShowWindow(SW_HIDE);
		m_editPeie4.ShowWindow(SW_HIDE);
		m_editPeie5.ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFenPeiPeie::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}

	if(m_btnCancel.GetSafeHwnd() != NULL)
	{
		m_btnCancel.SetWindowPos(NULL, btn_cancel_x, btn_cancel_y, m_btnCancel.Width(), m_btnCancel.Height(), SWP_NOZORDER);
	}

	if(m_editPeie1.GetSafeHwnd() != NULL)
	{
		m_editPeie1.SetWindowPos(NULL,edit_peie1_x,edit_peie1_y,edit_width,edit_height,SWP_NOZORDER);
	}
	if(m_editPeie2.GetSafeHwnd() != NULL)
	{
		m_editPeie2.SetWindowPos(NULL,edit_peie2_x,edit_peie2_y,edit_width,edit_height,SWP_NOZORDER);
	}
	if(m_editPeie3.GetSafeHwnd() != NULL)
	{
		m_editPeie3.SetWindowPos(NULL,edit_peie3_x,edit_peie3_y,edit_width,edit_height,SWP_NOZORDER);
	}
	if(m_editPeie4.GetSafeHwnd() != NULL)
	{
		m_editPeie4.SetWindowPos(NULL,edit_peie4_x,edit_peie4_y,edit_width,edit_height,SWP_NOZORDER);
	}
	if(m_editPeie5.GetSafeHwnd() != NULL)
	{
		m_editPeie5.SetWindowPos(NULL,edit_peie5_x,edit_peie5_y,edit_width,edit_height,SWP_NOZORDER);
	}
}

