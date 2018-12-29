#include "stdafx.h"
#include "GameClient.h"
#include "ExchangeDlg.h"
#include "GameClientDlg.h"

//Բ�Ǵ�С
#define ROUND_CX					7									//Բ�ǿ��
#define ROUND_CY					7									//Բ�Ǹ߶�

//��Ļλ��
#define LAYERED_SIZE				5									//�ֲ��С
#define CAPTION_SIZE				35									//�����С
// CExchangeDlg �Ի���

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

// CExchangeDlg ��Ϣ�������

BOOL CExchangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HINSTANCE hInstance=AfxGetInstanceHandle();

	//������Դ
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

	//���ô�С
	CRect rcDlg;
	g_pGameClientDlg->GetClientRect(&rcDlg);
	g_pGameClientDlg->ClientToScreen(&rcDlg);
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,rcDlg.left+199-110,rcDlg.top+330-120,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER);

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	//CRect rcUnLayered;
	//rcUnLayered.top=LAYERED_SIZE;
	//rcUnLayered.left=LAYERED_SIZE;
	//rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	//rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//��������
	//CRgn RgnWindow;
	//RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//�ֲ㴰��
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
	//���öһ����ʺ�Ŀǰ�һ�����
	m_strChipNotice.Format(TEXT("�һ����ʣ�1����=%d���"),g_pGameClientDlg->m_dwChipRate);
	m_strChipState.Format(TEXT("��ǰ�һ���%I64d����=%I64d��ң�ʣ��%I64d���"),
		m_lExchangeChip,
		g_pGameClientDlg->ChipToScore(m_lExchangeChip),
		m_lCurrentScore-g_pGameClientDlg->ChipToScore(m_lExchangeChip));

	g_pGameClientDlg->m_bAlreadyExchange=false;

	RefreshChipState();

	//return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
	return TRUE;
}

void CExchangeDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);

	if (cx == 0 || cy == 0) return;

	//��������
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE|SWP_NOCOPYBITS;

	LockWindowUpdate();

	//�ƶ��ؼ�
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

	//�������
	UnlockWindowUpdate();
}

BOOL CExchangeDlg::OnEraseBkgnd(CDC* pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);
	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);
	//���� DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);
	//���û���
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	//������Դ
	CBitImage ImageBack;
	ImageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_EXCHANGE_BACK);
	//�滭����
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
	newFont.CreateFont(-14,0,0,0,800,0,0,0,134,3,2,1,2,TEXT("����"));
	CFont *oldFont=pBufferDC->SelectObject(&newFont);

	rcDraw.left=31;
	rcDraw.top=165;
	rcDraw.right=rcDraw.left+400;
	rcDraw.bottom=rcDraw.top+20;
	//pBufferDC->FillRect(&rcDraw,&CBrush(RGB(255,255,255)));
	g_pGameClientDlg->m_GameClientView.DrawTextString(pBufferDC,m_strChipState,RGB(0,0,0),RGB(255,255,255),rcDraw,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	//pBufferDC->DrawText(m_strChipState,rcDraw,DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	pBufferDC->SelectObject(oldFont);

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);
	return TRUE;
}

void CExchangeDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	//��ʾ�ֲ�
	//if (m_SkinLayered.m_hWnd!=NULL)
	//{
	//	m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	//}
}

//void CExchangeDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
//{
//	CDialog::OnWindowPosChanged(lpwndpos);
//	//�ƶ��ֲ�
//	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpwndpos->cx>=0)&&(lpwndpos->cy>0))
//	{
//		m_SkinLayered.SetWindowPos(NULL,lpwndpos->x,lpwndpos->y,lpwndpos->cx,lpwndpos->cy,SWP_NOZORDER);
//	}
//}

BOOL CExchangeDlg::PreTranslateMessage(MSG* pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CExchangeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//ģ�����
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
	m_strChipState.Format(TEXT("��ǰ�һ���%I64d����=%I64d��ң�ʣ��%I64d���"),
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
