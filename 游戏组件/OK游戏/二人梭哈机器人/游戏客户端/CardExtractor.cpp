// CardExtractor.cpp : 实现文件
//

#include "stdafx.h"
#include "CardExtractor.h"


// CCardExtractor 对话框

IMPLEMENT_DYNAMIC(CCardExtractor, CDialog)

CCardExtractor::CCardExtractor(CWnd* pParent /*=NULL*/)
	: CDialog(CCardExtractor::IDD, pParent)
{
	m_pParentDlg = static_cast<CGameClientEngine *>(pParent);

	for (int i=0; i<4;i++)
	{
		m_CardCtrl[i].ShowFirstCard(true);
		m_CardCtrl[i].SetXDistance(85);
		m_CardCtrl[i].AllowPositively(true);
	}

	m_cbHoverCard = 0x00;
	//加载资源
	m_ImageCard.LoadFromResource(AfxGetInstanceHandle(),IDB_CARD);

}

CCardExtractor::~CCardExtractor()
{
}

void CCardExtractor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCardExtractor, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDOK, &CCardExtractor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCardExtractor::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCardExtractor 消息处理程序

void CCardExtractor::OnBnClickedOk()
{
	if (m_cbHoverCard != 0x00 && m_pParentDlg !=NULL)
	{
		m_pParentDlg->SendSocketData(SUB_C_SET_CARD, &m_cbHoverCard, sizeof(m_cbHoverCard));
	}
	m_cbHoverCard = 0x00;
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0; i<4;i++)
	{
		m_CardCtrl[i].SetIsExtractor(false);
	}
	OnOK();
}
void CCardExtractor::OnBnClickedCancel()
{
	for (int i=0; i<4;i++)
	{
		m_CardCtrl[i].SetIsExtractor(false);
	}
	OnCancel();
}

void CCardExtractor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	for (int i=0; i<4;i++)
	{
		m_CardCtrl[i].SetIsExtractor(true);
		m_CardCtrl[i].DrawCardControl(&dc);
	}

	if (m_cbHoverCard != 0x00)
	{
		//CImageHandle HandleCard(&m_ImageCard);
		
		INT nXImagePos=((m_cbHoverCard&CARD_MASK_VALUE)-1)*m_ImageCard.GetWidth()/13;
		INT nYImagePos=((m_cbHoverCard&CARD_MASK_COLOR)>>4)*m_ImageCard.GetHeight()/5;
	
		m_ImageCard.TransDrawImage(&dc,30,50,m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5,nXImagePos,nYImagePos,RGB(255,0,255));
	}
}

BOOL CCardExtractor::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetClientRect(rect);
	INT nWidth = rect.Width();
	INT nHeight = 120;// rect.Height();

	for (int i=0; i<4; i++)
	{
		m_CardCtrl[i].SetBasicStation(CPoint(nWidth/2+80,nHeight*(i+1) + 5),enXCenter,enYBottom);
	}

	BYTE cbCardData[4][7]={
		{0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x01},									//方块 8 - A
		{0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x11},									//梅花 8 - A
		{0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x21},									//红桃 8 - A
		{0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x31}									//黑桃 8 - A
	};

	for (int i=0; i<4;i++)
	{
		m_CardCtrl[i].SetCardData(cbCardData[i],7);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CCardExtractor::OnLButtonDown(UINT nFlags, CPoint point)
{
	BYTE cbTempCard(0);
	for (int i=0; i<4; i++)
	{
		cbTempCard = m_CardCtrl[i].GetLeftMouseDownCard(point);
		if (cbTempCard != 0x00 && m_cbHoverCard != cbTempCard)
		{
			m_cbHoverCard = cbTempCard;

			CRect rtTemp(CPoint(30,50),CSize(80,105));
			InvalidateRect(&rtTemp);
			break;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CCardExtractor::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//光标控制
	__super::OnSetCursor(pWnd,nHitTest,message);
	return true;
}
