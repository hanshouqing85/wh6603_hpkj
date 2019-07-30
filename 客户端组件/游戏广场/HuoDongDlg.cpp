#include "stdafx.h"
#include "GamePlaza.h"
#include "HuodongDlg.h"
#include "BmpUtil.h"

const static int dalibao_x = 25;
const static int dalibao_y = 100;

const static int zajindan_x = 30;
const static int zajindan_y = 160;

//�Ӵ�������
static const int bk_zhanghu_x = 0;
static const int bk_zhanghu_y = 194;

static const int bk_zhanghu_width = 1003;
static const int bk_zhanghu_height = 515;
IMPLEMENT_DYNAMIC(CHuoDongDlg, CDialog)

CHuoDongDlg::CHuoDongDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuoDongDlg::IDD, pParent)
	, m_bmpBk(NULL)
	//, m_bmpTip(NULL)
{
	
}

CHuoDongDlg::~CHuoDongDlg()

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
//�ж��Ƿ��ܹ��μӻ
void CHuoDongDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)
	{
		OnBnClickedBtnHuanlesong();

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
		m_daili.ShowWindow(SW_HIDE);
		m_dalibao.ShowWindow(SW_HIDE);
		m_zajindan.ShowWindow(SW_HIDE);
		m_huanlesong.ShowWindow(SW_HIDE);
		m_zhuanzhuan.ShowWindow(SW_HIDE);
	}
	return;
}
void CHuoDongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_DALIBAO, m_btnDalibao);
	DDX_Control(pDX, IDC_BTN_ZAJINDAN, m_btnZaJinDan);
	DDX_Control(pDX, IDC_BTN_HUANLESONG, m_btnHuanlesong);
	DDX_Control(pDX, IDC_BTN_ZHUANZHUAN, m_btnZhuanZhuan);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_DAILI, m_btnDaili);
}


BEGIN_MESSAGE_MAP(CHuoDongDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_DALIBAO, &CHuoDongDlg::OnBnClickedBtnDalibao)
	ON_BN_CLICKED(IDC_BTN_ZAJINDAN, &CHuoDongDlg::OnBnClickedBtnZajindan)
	ON_BN_CLICKED(IDC_BTN_HUANLESONG, &CHuoDongDlg::OnBnClickedBtnHuanlesong)
	ON_BN_CLICKED(IDC_BTN_ZHUANZHUAN, &CHuoDongDlg::OnBnClickedBtnZhuanzhuan)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CHuoDongDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_DAILI, &CHuoDongDlg::OnBnClickedBtnDaili)
	ON_WM_LBUTTONDOWN()

END_MESSAGE_MAP()

void CHuoDongDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}


// CHuodongDlg ��Ϣ�������

BOOL CHuoDongDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk == NULL)
	{
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg.png"));
		m_ImgTitle.LoadImage(CBmpUtil::GetExePath() + _T("skin\\BTzhgl_06.png"));

	}
	//m_bmpTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hdzx.png"));

	m_btnDalibao.SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_hd_1.png"));
	m_btnZaJinDan.SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_hd_2.png"));
	m_btnHuanlesong.SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_hd_3.png"));
	m_btnZhuanZhuan.SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_hd_4.png"));
	m_btnDaili.SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_hd_4.png"));

	m_huanlesong.Create(CHuoDongHuanLeSongDlg::IDD, this);
	m_dalibao.Create(CHuoDongDaliBaoDlg::IDD, this);
	m_zajindan.Create(CHuoDongZaJinDanDlg::IDD, this);
	m_zhuanzhuan.Create(HuodongZhuanzhuan::IDD, this);
	m_daili.Create(CHuoDongDaili::IDD, this);
	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\return_bt.png"));

// 	m_btnDalibao.ShowWindow(SW_HIDE);
// 	m_btnZaJinDan.ShowWindow(SW_HIDE);
	SetWindowPos(NULL,0,0,m_bmpBk->GetWidth(),m_bmpBk->GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
// CZhanghuDlg ��Ϣ�������
void CHuoDongDlg::OnBnClickedBtnClose()
{
// 	CWnd* pParent = GetParent();
// 	if(pParent!=NULL)
// 	{
// 		pParent->PostMessage(IDM_RETURN_GAME,1,0);
// 	}
	CDialog::OnCancel();
	return;
}

void CHuoDongDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(&rect);

	Graphics graphics(dc.m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);
	//graphics.DrawImage(m_bmpTip, Rect(16, 5, m_bmpTip->GetWidth(), m_bmpTip->GetHeight()), 0, 0, m_bmpTip->GetWidth(), m_bmpTip->GetHeight(), UnitPixel);
	int nX = (rect.Width()-m_ImgTitle.GetWidth())/2+5;
	m_ImgTitle.DrawImage(&dc,nX,30);

	//dc.TextOut(300, 300, _T("�ר��"));
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CHuoDongDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuoDongDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(GetSafeHwnd() == NULL)
	{
		return;
	}
	if(m_btnClose.GetSafeHwnd() != NULL)
	{
		m_btnClose.SetWindowPos(NULL, cx - m_btnClose.Width() - 9-15, 24, m_btnClose.Width(), m_btnClose.Height(), SWP_NOZORDER);
	}

	AdjustCtrls();
}

//�����
void CHuoDongDlg::OnBnClickedBtnDalibao()
{
	m_btnDalibao.SetPushed(true);
	m_btnZaJinDan.SetPushed(false);
	m_btnHuanlesong.SetPushed(false);
	m_btnZhuanZhuan.SetPushed(false);	
	m_btnDaili.SetPushed(false);	

	m_dalibao.ShowWindow(SW_SHOW);
	m_zajindan.ShowWindow(SW_HIDE);
	m_huanlesong.ShowWindow(SW_HIDE);
	m_zhuanzhuan.ShowWindow(SW_HIDE);
	m_daili.ShowWindow(SW_HIDE);
}

//�ҽ�
void CHuoDongDlg::OnBnClickedBtnZajindan()
{
	m_btnDalibao.SetPushed(false);
	m_btnZaJinDan.SetPushed(true);
	m_btnHuanlesong.SetPushed(false);
	m_btnZhuanZhuan.SetPushed(false);
	m_btnDaili.SetPushed(false);	

	m_dalibao.ShowWindow(SW_HIDE);
	m_zajindan.ShowWindow(SW_SHOW);
	m_huanlesong.ShowWindow(SW_HIDE);
	m_zhuanzhuan.ShowWindow(SW_HIDE);
	m_daili.ShowWindow(SW_HIDE);
}

//������
void CHuoDongDlg::OnBnClickedBtnHuanlesong()
{
	m_btnDalibao.SetPushed(false);
	m_btnZaJinDan.SetPushed(false);
	m_btnHuanlesong.SetPushed(true);
	m_btnZhuanZhuan.SetPushed(false);
	m_btnDaili.SetPushed(false);	

	m_dalibao.ShowWindow(SW_HIDE);
	m_zajindan.ShowWindow(SW_HIDE);
	m_huanlesong.ShowWindow(SW_SHOW);
	m_zhuanzhuan.ShowWindow(SW_HIDE);
	m_daili.ShowWindow(SW_HIDE);
}

//תת
void CHuoDongDlg::OnBnClickedBtnZhuanzhuan()
{
	m_btnDalibao.SetPushed(false);
	m_btnZaJinDan.SetPushed(false);
	m_btnHuanlesong.SetPushed(false);
	m_btnZhuanZhuan.SetPushed(true);
	m_btnDaili.SetPushed(false);	

	m_dalibao.ShowWindow(SW_HIDE);
	m_zajindan.ShowWindow(SW_HIDE);
	m_huanlesong.ShowWindow(SW_HIDE);
	m_zhuanzhuan.ShowWindow(SW_SHOW);
	m_daili.ShowWindow(SW_HIDE);
}

//�������
void CHuoDongDlg::OnBnClickedBtnDaili()
{
	m_btnDalibao.SetPushed(false);
	m_btnZaJinDan.SetPushed(false);
	m_btnHuanlesong.SetPushed(false);
	m_btnZhuanZhuan.SetPushed(false);
	m_btnDaili.SetPushed(true);	

	m_dalibao.ShowWindow(SW_HIDE);
	m_zajindan.ShowWindow(SW_HIDE);
	m_huanlesong.ShowWindow(SW_HIDE);
	m_zhuanzhuan.ShowWindow(SW_HIDE);
	m_daili.ShowWindow(SW_SHOW);
}


void CHuoDongDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuoDongDlg::OnOK()
{
	//CDialog::OnOK();
}

void CHuoDongDlg::AdjustCtrls()
{

	//4���Ի���
	if (m_dalibao.GetSafeHwnd())
	{
		m_dalibao.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}

	if (m_zajindan.GetSafeHwnd())
	{
		m_zajindan.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}
	if (m_huanlesong.GetSafeHwnd())
	{
		m_huanlesong.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}	
	if (m_zhuanzhuan.GetSafeHwnd())
	{
		m_zhuanzhuan.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}
	if (m_daili.GetSafeHwnd())
	{
		m_daili.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}
	


		// 4����ť
	if (m_btnDalibao.GetSafeHwnd())
	{
		m_btnDalibao.SetWindowPos(NULL, dalibao_x, dalibao_y, m_btnDalibao.Width(), m_btnDalibao.Height(), SWP_NOZORDER);
	}

	if (m_btnZaJinDan.GetSafeHwnd())
	{
		m_btnZaJinDan.SetWindowPos(NULL, dalibao_x+90, dalibao_y, m_btnZaJinDan.Width(), m_btnZaJinDan.Height(), SWP_NOZORDER);
	}

	if(m_btnHuanlesong.GetSafeHwnd())
		m_btnHuanlesong.SetWindowPos(NULL, dalibao_x+180, dalibao_y, m_btnHuanlesong.Width(), m_btnHuanlesong.Height(), SWP_NOZORDER);

	if(m_btnZhuanZhuan.GetSafeHwnd())
	{
		m_btnZhuanZhuan.SetWindowPos(NULL, dalibao_x+270, dalibao_y, m_btnZhuanZhuan.Width(), m_btnZhuanZhuan.Height(), SWP_NOZORDER);
		m_btnZhuanZhuan.ShowWindow(SW_HIDE);
	}
	if(m_btnDaili.GetSafeHwnd())
		m_btnDaili.SetWindowPos(NULL, dalibao_x+270, dalibao_y, m_btnDaili.Width(), m_btnDaili.Height(), SWP_NOZORDER);

}
