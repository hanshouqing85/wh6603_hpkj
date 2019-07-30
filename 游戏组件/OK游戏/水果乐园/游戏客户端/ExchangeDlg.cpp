#include "stdafx.h"
#include "GameClient.h"
#include "ExchangeDlg.h"
#include "GameClientDlg.h"

//圆角大小
#define ROUND_CX					7									//圆角宽度
#define ROUND_CY					7									//圆角高度

//屏幕位置
#define LAYERED_SIZE				5									//分层大小
#define CAPTION_SIZE				35									//标题大小
// CExchangeDlg 对话框

BEGIN_MESSAGE_MAP(CExchangeDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	//ON_WM_WINDOWPOSCHANGED()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CHIPCLOSE, OnBnClickedBtnChipclose)
	ON_BN_CLICKED(IDC_BTN_EX100, OnBnClickedBtnEx100)
	ON_BN_CLICKED(IDC_BTN_EX1000, OnBnClickedBtnEx1000)
	ON_BN_CLICKED(IDC_BTN_EX10000, OnBnClickedBtnEx10000)
	ON_BN_CLICKED(IDC_BTN_EXMAX, OnBnClickedBtnExmax)
	ON_BN_CLICKED(IDC_BTN_EXCLEAR, OnBnClickedBtnExclear)
	ON_BN_CLICKED(ID_CANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(ID_OK,OnBnClickedOK)
END_MESSAGE_MAP()

//IMPLEMENT_DYNAMIC(CExchangeDlg, CDialog)
CExchangeDlg::CExchangeDlg(CWnd* pParent) : CDialog(CExchangeDlg::IDD)
{
	m_lCurrentScore=0;
	m_lExchangeChip=0;
}

CExchangeDlg::~CExchangeDlg()
{
}

void CExchangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// CExchangeDlg 消息处理程序

BOOL CExchangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HINSTANCE hInstance=AfxGetInstanceHandle();

	//加载资源
	CBitImage ImageBack;
	ImageBack.LoadFromResource(hInstance,IDB_EXCHANGE_BACK);

	CRect rcCreate(0,0,0,0);
	m_BtnClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_CHIPCLOSE);
	m_BtnEx100.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_EX100);
	m_BtnEx1000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_EX1000);
	m_BtnEx10000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_EX10000);
	m_BtnExMax.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_EXMAX);
	m_BtnExClear.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_EXCLEAR);
	m_BtnOK.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,ID_OK);
	m_BtnCancel.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,ID_CANCEL);

	m_BtnClose.SetButtonImage(IDB_BT_CHIP_CLOSE_X,hInstance,false,false);
	m_BtnEx100.SetButtonImage(IDB_BT_CHIP_EX100,hInstance,false,false);
	m_BtnEx1000.SetButtonImage(IDB_BT_CHIP_EX1000,hInstance,false,false);
	m_BtnEx10000.SetButtonImage(IDB_BT_CHIP_EX10000,hInstance,false,false);
	m_BtnExMax.SetButtonImage(IDB_BT_CHIP_EXMAX,hInstance,false,false);
	m_BtnExClear.SetButtonImage(IDB_BT_CHIP_EXCLEAR,hInstance,false,false);
	m_BtnOK.SetButtonImage(IDB_BT_CHIP_EXOK,hInstance,false,false);
	m_BtnCancel.SetButtonImage(IDB_BT_CHIP_EXCANCEL,hInstance,false,false);

	//设置大小
	CRect rcDlg;
	g_pGameClientDlg->GetClientRect(&rcDlg);
	g_pGameClientDlg->ClientToScreen(&rcDlg);
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,rcDlg.left+199-110,rcDlg.top+330-120,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER);

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	//CRect rcUnLayered;
	//rcUnLayered.top=LAYERED_SIZE;
	//rcUnLayered.left=LAYERED_SIZE;
	//rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	//rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//设置区域
	//CRgn RgnWindow;
	//RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//分层窗口
	//m_SkinLayered.CreateLayered(this,rcWindow);
	//m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	m_lCurrentScore=0;
	m_lExchangeChip=0;

	IClientUserItem *pUserItem=g_pGameClientDlg->GetTableUserItem(g_pGameClientDlg->GetMeChairID());
	tagUserInfo *pMeUserData = NULL;
	if(pUserItem!=NULL)
		pMeUserData = pUserItem->GetUserInfo();
	if( pMeUserData == NULL )
		return true;
	m_lCurrentScore=pMeUserData->lScore;
	m_lCurrentScore-=g_pGameClientDlg->ChipToScore(g_pGameClientDlg->m_lMyCurrentChip);

	m_strChipNotice="";
	m_strChipState="";
	//设置兑换比率和目前兑换数额
	m_strChipNotice.Format(TEXT("兑换比率：1筹码=%d金币"),g_pGameClientDlg->m_dwChipRate);
	m_strChipState.Format(TEXT("当前兑换：%I64d筹码=%I64d金币，剩余%I64d金币"),
		m_lExchangeChip,
		g_pGameClientDlg->ChipToScore(m_lExchangeChip),
		m_lCurrentScore-g_pGameClientDlg->ChipToScore(m_lExchangeChip));

	g_pGameClientDlg->m_bAlreadyExchange=false;

	RefreshChipState();

	//return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
	return TRUE;
}

void CExchangeDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);

	if (cx == 0 || cy == 0) return;

	//变量定义
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE|SWP_NOCOPYBITS;

	LockWindowUpdate();

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(8);
	DeferWindowPos(hDwp,m_BtnClose,		NULL,440,6,0,0,uFlags);
	DeferWindowPos(hDwp,m_BtnEx100,		NULL,16,92,0,0,uFlags);
	DeferWindowPos(hDwp,m_BtnEx1000,	NULL,104,92,0,0,uFlags);
	DeferWindowPos(hDwp,m_BtnEx10000,	NULL,192,92,0,0,uFlags);
	DeferWindowPos(hDwp,m_BtnExMax,		NULL,280,92,0,0,uFlags);
	DeferWindowPos(hDwp,m_BtnExClear,	NULL,368,92,0,0,uFlags);
	DeferWindowPos(hDwp,m_BtnOK,		NULL,133,202,0,0,uFlags);
	DeferWindowPos(hDwp,m_BtnCancel,	NULL,251,202,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//解除锁定
	UnlockWindowUpdate();
}

BOOL CExchangeDlg::OnEraseBkgnd(CDC* pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);
	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);
	//创建 DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);
	//设置缓冲
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	//加载资源
	CBitImage ImageBack;
	ImageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_EXCHANGE_BACK);
	//绘画背景
	ImageBack.AlphaDrawImage(pBufferDC,0,0,255);

	CRect rcDraw;
	rcDraw.left=31;
	rcDraw.top=140;
	rcDraw.right=rcDraw.left+400;
	rcDraw.bottom=rcDraw.top+20;
	//pBufferDC->FillRect(&rcDraw,&CBrush(RGB(255,255,255)));
	g_pGameClientDlg->m_GameClientView.DrawTextString(pBufferDC,m_strChipNotice,RGB(0,0,0),RGB(255,255,255),rcDraw,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	//pBufferDC->DrawText(m_strChipNotice,rcDraw,DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	CFont newFont;
	newFont.CreateFont(-14,0,0,0,800,0,0,0,134,3,2,1,2,TEXT("宋体"));
	CFont *oldFont=pBufferDC->SelectObject(&newFont);

	rcDraw.left=31;
	rcDraw.top=165;
	rcDraw.right=rcDraw.left+400;
	rcDraw.bottom=rcDraw.top+20;
	//pBufferDC->FillRect(&rcDraw,&CBrush(RGB(255,255,255)));
	g_pGameClientDlg->m_GameClientView.DrawTextString(pBufferDC,m_strChipState,RGB(0,0,0),RGB(255,255,255),rcDraw,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	//pBufferDC->DrawText(m_strChipState,rcDraw,DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	pBufferDC->SelectObject(oldFont);

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);
	return TRUE;
}

void CExchangeDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	//显示分层
	//if (m_SkinLayered.m_hWnd!=NULL)
	//{
	//	m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	//}
}

//void CExchangeDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
//{
//	CDialog::OnWindowPosChanged(lpwndpos);
//	//移动分层
//	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpwndpos->cx>=0)&&(lpwndpos->cy>0))
//	{
//		m_SkinLayered.SetWindowPos(NULL,lpwndpos->x,lpwndpos->y,lpwndpos->cx,lpwndpos->cy,SWP_NOZORDER);
//	}
//}

BOOL CExchangeDlg::PreTranslateMessage(MSG* pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CExchangeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//模拟标题
	if (point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CExchangeDlg::OnBnClickedBtnChipclose()
{
	g_pGameClientDlg->m_bAlreadyExchange=true;
	if (g_pGameClientDlg->m_lMyCurrentChip<=0)
		g_pGameClientDlg->PostMessage(WM_CLOSE);
	OnCancel();
}

void CExchangeDlg::OnBnClickedCancel()
{
	
	
	/*g_pGameClientDlg->m_bAlreadyExchange=true;
	if (g_pGameClientDlg->m_lMyCurrentChip<=0)
		g_pGameClientDlg->PostMessage(WM_CLOSE);
	OnCancel();*/
	OnCancel();
}

void CExchangeDlg::OnBnClickedOK()
{
	if (m_lExchangeChip>0)
	{
		g_pGameClientDlg->ExchangeChip(m_lExchangeChip);
		m_lExchangeChip=0;
	}
	OnCancel();
}

void CExchangeDlg::RefreshChipState()
{
	m_strChipState.Format(TEXT("当前兑换：%I64d筹码=%I64d金币，剩余%I64d金币"),
		m_lExchangeChip,
		g_pGameClientDlg->ChipToScore(m_lExchangeChip),
		m_lCurrentScore-g_pGameClientDlg->ChipToScore(m_lExchangeChip));

	m_BtnEx10000.EnableWindow(((g_pGameClientDlg->ScoreToChip(m_lCurrentScore)-m_lExchangeChip)>=10000)?TRUE:FALSE);
	m_BtnEx1000.EnableWindow(((g_pGameClientDlg->ScoreToChip(m_lCurrentScore)-m_lExchangeChip)>=1000)?TRUE:FALSE);
	m_BtnEx100.EnableWindow(((g_pGameClientDlg->ScoreToChip(m_lCurrentScore)-m_lExchangeChip)>=100)?TRUE:FALSE);
	m_BtnOK.EnableWindow((m_lExchangeChip>0)?TRUE:FALSE);

	Invalidate();
}

void CExchangeDlg::OnBnClickedBtnEx100()
{
	if ((g_pGameClientDlg->ScoreToChip(m_lCurrentScore)-m_lExchangeChip)>=100)
	{
		m_lExchangeChip+=100;
		RefreshChipState();
	}
}

void CExchangeDlg::OnBnClickedBtnEx1000()
{
	if ((g_pGameClientDlg->ScoreToChip(m_lCurrentScore)-m_lExchangeChip)>=1000)
	{
		m_lExchangeChip+=1000;
		RefreshChipState();
	}
}

void CExchangeDlg::OnBnClickedBtnEx10000()
{
	if ((g_pGameClientDlg->ScoreToChip(m_lCurrentScore)-m_lExchangeChip)>=10000)
	{
		m_lExchangeChip+=10000;
		RefreshChipState();
	}
}

void CExchangeDlg::OnBnClickedBtnExmax()
{
	m_lExchangeChip=g_pGameClientDlg->ScoreToChip(m_lCurrentScore);
	RefreshChipState();
}

void CExchangeDlg::OnBnClickedBtnExclear()
{
	m_lExchangeChip=0;
	RefreshChipState();
}
