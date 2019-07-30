#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhuanZhangDlg.h"
#include "Bmputil.h"
#include "MessageDlg.h"

static const int wnd_width = 332;
static const int wnd_height = 264;

static const int btn_ok_x = 100;
static const int btn_ok_y = 240;

static const int btn_cancel_x = 233;
static const int btn_cancel_y = 240;

//�淨
static const int tip_wanfa_x = 130;
static const int tip_wanfa_y = 47;
static const int tip_wanfa_width = 70;
static const int tip_wanfa_height = 20;
static CRect rc_wanfa(tip_wanfa_x, 
						   tip_wanfa_y, 
						   tip_wanfa_x + tip_wanfa_width, 
						   tip_wanfa_y + tip_wanfa_height);

//ע��
static const int tip_zhushu_x = 130;
static const int tip_zhushu_y = 87;
static const int tip_zhushu_width = 70;
static const int tip_zhushu_height = 20;
static CRect rc_zhushu(tip_zhushu_x, 
						   tip_zhushu_y, 
						   tip_zhushu_x + tip_zhushu_width, 
						   tip_zhushu_y + tip_zhushu_height);

//�û�ID
static const int tip_danzhujine_x = 80;
static const int tip_danzhujine_y = 50;
static const int tip_danzhujine_width = 250;
static const int tip_danzhujine_height = 20;
static CRect rc_zongjine(tip_danzhujine_x, 
						   tip_danzhujine_y, 
						   tip_danzhujine_x + tip_danzhujine_width, 
						   tip_danzhujine_y + tip_danzhujine_height);

//�ܽ��
static const int tip_zongjine_x = 80;
static const int tip_zongjine_y = 111;
static const int tip_zongjine_width = 230;
static const int tip_zongjine_height = 20;
static CRect rc_danzhujine(tip_zongjine_x, 
						   tip_zongjine_y, 
						   tip_zongjine_x + tip_zongjine_width, 
						   tip_zongjine_y + tip_zongjine_height);

// CZhuanZhangDlg �Ի���

IMPLEMENT_DYNAMIC(CZhuanZhangDlg, CDialog)

CZhuanZhangDlg::CZhuanZhangDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZhuanZhangDlg::IDD, pParent)
	, m_bmpBk(NULL)
	,m_fJine(0.0)
	,m_fJiaoYiJine(0.0)
{
	m_nType=0;
	m_n_t_userid = 0;
	m_n_t_target_userid = 0;
	m_strName.Empty();
	ZeroMemory(m_szPassWord,sizeof(m_szPassWord));
}

CZhuanZhangDlg::~CZhuanZhangDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CZhuanZhangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_JINE, m_editJine);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassWord);

	DDX_Control(pDX, IDC_COMBO_SORT, m_cmbZzType);

}


BEGIN_MESSAGE_MAP(CZhuanZhangDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_JINE, OnLvnItemChangedEdit)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_EDIT_JINE, OnLvnItemChangedEdit)
END_MESSAGE_MAP()


// CZhuanZhangDlg ��Ϣ�������

void CZhuanZhangDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(&rect);

	CDC cacheDC;
	cacheDC.CreateCompatibleDC(&dc);
	CBitmap cacheBmp;
	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

	Graphics graphics(cacheDC.m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

	CString strTmp;
	strTmp.Format(_T("��ǰ��%.2lf"), m_fJine);
	cacheDC.DrawText(strTmp, rc_danzhujine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strTmp.Format(L"Ŀ���û���%s [%d]",m_strName,m_n_t_target_userid);
 
 	cacheDC.DrawText(strTmp, rc_zongjine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

 	strTmp.Format(L"ת�˽�");
 	CRect rcStatic;
 	rcStatic.CopyRect(rc_danzhujine);
 	rcStatic.top +=30;
 	rcStatic.bottom +=30;
 	cacheDC.DrawText(strTmp, rcStatic, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strTmp.Format(L"ȡ�����룺");
	rcStatic.top +=30;
	rcStatic.bottom +=30;
	cacheDC.DrawText(strTmp, rcStatic, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strTmp.Format(L"ת�����ͣ�");
	rcStatic.CopyRect(rc_zongjine);
	rcStatic.top +=30;
	rcStatic.bottom +=30;
	cacheDC.DrawText(strTmp, rcStatic, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CZhuanZhangDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CZhuanZhangDlg::SetZhuanZhangXinxi(int n_t_userid,CString strName,int n_t_target_userid,DOUBLE fJine)
{
	m_n_t_userid = n_t_userid;
	m_n_t_target_userid = n_t_target_userid;
	m_fJine = fJine;
	m_strName = strName;
	return;
}
VOID CZhuanZhangDlg::OnLvnItemChangedEdit()
{
	TCHAR szJiaoyi[30];
	GetDlgItemText(IDC_EDIT_JINE,szJiaoyi,sizeof(szJiaoyi));

	for(int i = 0;i < 30;i++)
	{
		CString str;
		str.Format(L"DQSZ %d",szJiaoyi[i]);
		if(szJiaoyi[i] == 0)
			break;
		if((szJiaoyi[i]!=8 )&&(szJiaoyi[i]!=46)&&(szJiaoyi[i]<48||szJiaoyi[i] >57))
		{
			GetDlgItem(IDC_EDIT_JINE)->SetWindowText(L"");
			return;
		}
	}
	m_fJiaoYiJine = _tstof(szJiaoyi);
	if(m_fJiaoYiJine > m_fJine)
	{
		MyMessageBox(L"���㣬��������д");
		m_editJine.SetSel(0,-1);
		return;
	}
	return;

}
VOID CZhuanZhangDlg::OnOK()
{
	TCHAR szJiaoyi[30];
	GetDlgItemText(IDC_EDIT_JINE,szJiaoyi,sizeof(szJiaoyi));

	m_nType = m_cmbZzType.GetCurSel();
	if(m_nType == 0)
	{
		MyMessageBox(L"��ѡ��ת�����ͣ�");
		m_cmbZzType.SetFocus();
		return;
	}
	GetDlgItemText(IDC_EDIT_PASSWORD,m_szPassWord,sizeof(m_szPassWord));
	m_fJiaoYiJine = _tstof(szJiaoyi);
	if(m_fJiaoYiJine > m_fJine)
	{
		MyMessageBox(L"���㣬��������д");
		m_editJine.SetSel(0,-1);
		return;
	}
	return CDialog::OnOK();
}
VOID CZhuanZhangDlg::OnCancel()
{
	return CDialog::OnCancel();
}
BOOL CZhuanZhangDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\zhuanzhang.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\quxiao_bt.png"));

	SetWindowPos(NULL, 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow();
	m_cmbfont.CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_editJine.SetFocus();

	m_cmbZzType.SetButtonImage(AfxGetInstanceHandle(),IDB_BT_COMBOBOX_ACT,CSize(16,18));
	m_cmbZzType.SetFont(&m_cmbfont);
	m_cmbZzType.ResetContent();

	m_cmbZzType.AddString(TEXT("--��ѡ��--"));
	m_cmbZzType.AddString(TEXT("��ͨת��"));
	m_cmbZzType.AddString(TEXT("�ֺ�ת��"));
	m_cmbZzType.AddString(TEXT("����ת��"));
	m_cmbZzType.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CZhuanZhangDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x+14, btn_ok_y-24, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}

	if(m_btnCancel.GetSafeHwnd() != NULL)
	{
		m_btnCancel.SetWindowPos(NULL, btn_cancel_x-12, btn_cancel_y-24, m_btnCancel.Width(), m_btnCancel.Height(), SWP_NOZORDER);
	}

	if(m_editJine.GetSafeHwnd() != NULL)
	{
		m_editJine.SetWindowPos(NULL, btn_cancel_x-17-51, btn_cancel_y-104,80, 18, SWP_NOSIZE);
	}
	if(m_editPassWord.GetSafeHwnd() != NULL)
	{
		m_editPassWord.SetWindowPos(NULL, btn_cancel_x-51-17, btn_cancel_y-75,80, 18, SWP_NOSIZE);
	}

	if (m_cmbZzType.GetSafeHwnd() != NULL)
	{
		m_cmbZzType.SetWindowPos(NULL, btn_cancel_x-17-51, btn_cancel_y-162, 104,18, SWP_NOZORDER);
	}

}

