#include "stdafx.h"
#include "GamePlaza.h"
#include "PlatformMessage.h"
#include "BmpUtil.h"

const static int dalibao_x = 33;
const static int dalibao_y = 90;

const static int zajindan_x = 49;
const static int zajindan_y = 141;

//子窗口数据
static const int bk_zhanghu_x = 25;
static const int bk_zhanghu_y = 192;
static const int bk_zhanghu_width = 966;
static const int bk_zhanghu_height = 462;
IMPLEMENT_DYNAMIC(CPlatformMessage, CDialog)

CPlatformMessage::CPlatformMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CPlatformMessage::IDD, pParent)
	, m_bmpBk(NULL)
	//, m_bmpTip(NULL)
{
	
}

CPlatformMessage::~CPlatformMessage()

{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
		m_bmpBk = NULL;
	}
// 	if (m_bmpTip != NULL)
// 	{
// 		delete m_bmpTip;
// 		m_bmpTip = NULL;
// 	}
}
//判断是否能够参加活动
void CPlatformMessage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)
	{
		OnBnClickedBtnInbox();

// 		if (theAccount.fandian < 0.029000001 && theAccount.fandian > 0.02799999)
// 		{
// 			m_btnDaili.ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			m_btnDaili.ShowWindow(SW_HIDE);
// 		}
		
	}
	else
	{
		m_dlgInbox.ShowWindow(SW_HIDE);
		m_dlgOutbox.ShowWindow(SW_HIDE);
		m_dlgSendMessage.ShowWindow(SW_HIDE);
	}
	return;
}
void CPlatformMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_DALIBAO, m_btnInbox);
	DDX_Control(pDX, IDC_BTN_ZAJINDAN, m_btnOutbox);
	DDX_Control(pDX, IDC_BTN_HUANLESONG, m_btnSendMessage);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);

}


BEGIN_MESSAGE_MAP(CPlatformMessage, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_DALIBAO, &CPlatformMessage::OnBnClickedBtnInbox)
	ON_BN_CLICKED(IDC_BTN_ZAJINDAN, &CPlatformMessage::OnBnClickedBtnOutbox)
	ON_BN_CLICKED(IDC_BTN_HUANLESONG, &CPlatformMessage::OnBnClickedBtnSendMessage)
	
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CPlatformMessage::OnBnClickedBtnClose)
	ON_MESSAGE(IDM_REPLY,&CPlatformMessage::OnMessageReply)
	ON_MESSAGE(IDM_GO_OUTBOX,&CPlatformMessage::OnGoOutbox)
	ON_WM_LBUTTONDOWN()

END_MESSAGE_MAP()

void CPlatformMessage::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}


// CHuodongDlg 消息处理程序

BOOL CPlatformMessage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(m_bmpBk == NULL)
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg.png"));
	//m_bmpTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hdzx.png"));

	m_btnInbox.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\shouxinxiang.png"));
	m_btnOutbox.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\faxinxiang.png"));
	m_btnSendMessage.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\xinxinxi.png"));

	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\return_bt.png"));

	m_dlgInbox.Create(CPlatformInbox::IDD, this);
	m_dlgOutbox.Create(CPlatformOutbox::IDD, this);
	m_dlgSendMessage.Create(CPlatformSendMessage::IDD, this);
	m_dlgInbox.SetTypeID(1);
	m_dlgOutbox.SetTypeID(2);
	SetWindowPos(NULL,0,0,m_bmpBk->GetWidth(),m_bmpBk->GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);


	return TRUE; 
}
// CZhanghuDlg 消息处理程序
void CPlatformMessage::OnBnClickedBtnClose()
{
// 	CWnd* pParent = GetParent();
// 	if(pParent!=NULL)
// 	{
// 		pParent->PostMessage(IDM_RETURN_GAME,1,0);
// 	}
	CDialog::OnCancel();
	return;
}
LRESULT CPlatformMessage::OnGoOutbox(WPARAM wParam, LPARAM lParam)
{
		OnBnClickedBtnOutbox();
		return 1;
}
LRESULT CPlatformMessage::OnMessageReply(WPARAM wParam, LPARAM lParam)
{
	OnBnClickedBtnSendMessage();
	CString strName = m_dlgInbox.m_strName;
	int nUserID = m_dlgInbox.m_nUserID;
	CString strTiltle = m_dlgInbox.m_strTitle;
	m_dlgSendMessage.SetSendUser(strName,nUserID,strTiltle);
	return 1;
}
void CPlatformMessage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);

	Graphics graphics(dc.m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);
	//graphics.DrawImage(m_bmpTip, Rect(16, 5, m_bmpTip->GetWidth(), m_bmpTip->GetHeight()), 0, 0, m_bmpTip->GetWidth(), m_bmpTip->GetHeight(), UnitPixel);

	//dc.TextOut(300, 300, _T("活动专区"));
	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CPlatformMessage::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CPlatformMessage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(GetSafeHwnd() == NULL)
	{
		return;
	}
	if(m_btnClose.GetSafeHwnd() != NULL)
	{
		m_btnClose.SetWindowPos(NULL, cx - m_btnClose.Width() - 7, -1, m_btnClose.Width(), m_btnClose.Height(), SWP_NOZORDER);
	}

	AdjustCtrls();
}

//大礼包
void CPlatformMessage::OnBnClickedBtnInbox()
{
	m_btnInbox.SetPushed(true);
	m_btnOutbox.SetPushed(false);
	m_btnSendMessage.SetPushed(false);

	m_dlgInbox.ShowWindow(SW_SHOW);
	m_dlgOutbox.ShowWindow(SW_HIDE);

	m_dlgSendMessage.ShowWindow(SW_HIDE);
}

//砸金蛋
void CPlatformMessage::OnBnClickedBtnOutbox()
{
	m_btnInbox.SetPushed(false);
	m_btnOutbox.SetPushed(true);
	m_btnSendMessage.SetPushed(false);

	m_dlgInbox.ShowWindow(SW_HIDE);

	m_dlgSendMessage.ShowWindow(SW_HIDE);
	m_dlgOutbox.ShowWindow(SW_SHOW);
}

//欢乐颂
void CPlatformMessage::OnBnClickedBtnSendMessage()
{
	m_btnInbox.SetPushed(false);
	m_btnOutbox.SetPushed(false);
	m_btnSendMessage.SetPushed(true);

	m_dlgInbox.ShowWindow(SW_HIDE);

	m_dlgOutbox.ShowWindow(SW_HIDE);
	m_dlgSendMessage.ShowWindow(SW_SHOW);
}


void CPlatformMessage::OnCancel()
{
	//CDialog::OnCancel();
}

void CPlatformMessage::OnOK()
{
	//CDialog::OnOK();
}

void CPlatformMessage::AdjustCtrls()
{
	// 4个按钮
	if (m_btnInbox.GetSafeHwnd())
	{
		m_btnInbox.SetWindowPos(NULL, dalibao_x, dalibao_y, m_btnInbox.Width(), m_btnInbox.Height(), SWP_NOZORDER);
	}

	if (m_btnOutbox.GetSafeHwnd())
	{
		m_btnOutbox.SetWindowPos(NULL, dalibao_x+90, dalibao_y, m_btnOutbox.Width(), m_btnOutbox.Height(), SWP_NOZORDER);
	}

	if(m_btnSendMessage.GetSafeHwnd())
		m_btnSendMessage.SetWindowPos(NULL, dalibao_x+180, dalibao_y, m_btnSendMessage.Width(), m_btnSendMessage.Height(), SWP_NOZORDER);

	//4个对话框
	if (m_dlgInbox.GetSafeHwnd())
	{
		m_dlgInbox.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}
	//4个对话框
	if (m_dlgOutbox.GetSafeHwnd())
	{
		m_dlgOutbox.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}

	//4个对话框
	if (m_dlgSendMessage.GetSafeHwnd())
	{
		m_dlgSendMessage.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}

	
}
