#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanTjxjDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"

static const int rio_width = 71;
static const int rio_height = 23;
static const int rio_daili_x = 380;
static const int rio_daili_y = 140;

static const int rio_huiyuan_x = 472;
static const int rio_huiyuan_y = 140;

static const int edit_width = 198;
static const int edit_height = 21;

static const int edit_x = 382;
static const int edit_fandian_y = 175;
static const int edit_zhanghu_y = 208;
static const int edit_dl_pwd_y = 240;
static const int edit_crm_dl_pwd_y = 272;
static const int edit_qk_pwd_y = 304;
static const int edit_crm_qk_pwd_y = 338;
static const int edit_qq_y = 370;

static const int btn_ok_x = 433;
static const int btn_ok_y = 427;

static const int btn_regurl_x = 749;
static const int btn_regurl_y = 66;

static CRect rcRegUrl(btn_regurl_x+80, btn_regurl_y, btn_regurl_x+600, btn_regurl_y+30);


static CRect rcText(edit_x-72, edit_qq_y + 30, edit_x+400-72, edit_qq_y + 30+20);

static const int web_fandian_x = 229;
static const int web_fandian_y = 106;

static const int set_fandian_x = 587;
static const int set_fandian_y = 105;

static const int set_mima_x = 587;
static const int set_mima_y = 199;
IMPLEMENT_DYNAMIC(CHuiYuanTjxjDlg, CDialog)

CHuiYuanTjxjDlg::CHuiYuanTjxjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanTjxjDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_type(0)
	, m_fandian(0)
	, m_zhanghu(_T(""))
	, m_password(_T(""))
	, m_crmpwd(_T(""))
	, m_qkpwd(_T(""))
	, m_crmqkpwd(_T(""))
	, m_strFandian(_T(""))
	, m_strWebFandian(_T(""))
	, m_strQQ(_T(""))
{
	m_bGetRegURL=false;
	m_AddHuiYuan=false;
	m_bSetWebFandian=false;
}

CHuiYuanTjxjDlg::~CHuiYuanTjxjDlg()
{
	if (m_bmpBk != NULL)
	{
		SafeDelete(m_bmpBk);
	}
}

void CHuiYuanTjxjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FANDIAN, m_fandian);
	DDX_Text(pDX, IDC_EDIT_ZHANGHAO, m_zhanghu);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDX_Text(pDX, IDC_EDIT_CRM_PWD, m_crmpwd);
	DDX_Text(pDX, IDC_EDIT_QK_PWD, m_qkpwd);
	DDX_Text(pDX, IDC_EDIT_CRM_QK_PWD, m_crmqkpwd);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_RIO_DAILI, m_rioDaili);
	DDX_Control(pDX, IDC_RIO_HUIYUAN, m_rioHuiYuan);
	DDX_Control(pDX, IDC_COMBO_FANDIAN, m_cmbFandian);
	DDX_CBString(pDX, IDC_COMBO_FANDIAN, m_strFandian);
	DDX_Control(pDX, IDC_COMBO_WEB_FANDIAN, m_cmbWebFandian);
	DDX_CBString(pDX, IDC_COMBO_WEB_FANDIAN, m_strWebFandian);
	DDX_Control(pDX, IDC_BTN_CP_REGURL, m_btnRegUrl);
	DDX_Control(pDX, IDC_BTN_SET_WEB_FANDIAN, m_btnSetWebFandian);
	DDX_Control(pDX, IDC_BTN_SET_MOREN_MIMA, m_btnSetMorenMima);
	DDX_Control(pDX, IDC_EDIT_QQ, m_editQQ);
	DDX_Control(pDX, IDC_COMBO_WEB_REG_URL, m_cmbWebRegUrl);

	DDX_Control(pDX, IDC_EDIT_ZHANGHAO, m_editZhanghu);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_CRM_PWD, m_editCrmPwd);
	DDX_Control(pDX, IDC_EDIT_QK_PWD, m_editQkPwd);
	DDX_Control(pDX, IDC_EDIT_CRM_QK_PWD, m_editCrmQkPwd);

	DDX_Text(pDX, IDC_EDIT_QQ, m_strQQ);
}


BEGIN_MESSAGE_MAP(CHuiYuanTjxjDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RIO_DAILI, &CHuiYuanTjxjDlg::OnBnClickedRioDaili)
	ON_BN_CLICKED(IDC_RIO_HUIYUAN, &CHuiYuanTjxjDlg::OnBnClickedRioHuiyuan)
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanTjxjDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_SET_WEB_FANDIAN, &CHuiYuanTjxjDlg::OnBnClickedSetWebFandian)
	ON_BN_CLICKED(IDC_BTN_SET_MOREN_MIMA, &CHuiYuanTjxjDlg::OnBnClickedSetMorenMima)
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CP_REGURL, &CHuiYuanTjxjDlg::OnBnClickedBtnCpRegurl)
END_MESSAGE_MAP()


// CHuiYuanTjxjDlg ��Ϣ�������

void CHuiYuanTjxjDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0,m_bmpBk->GetWidth(), m_bmpBk-> GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);

 	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
// 	CFont* pOldFont = cacheDC.SelectObject(&m_font);
// 	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));
	
// 	CString str;
// 	if (theAccount.type==1)
// 	{
// 		str = _T("�Ƽ�ע�᣺");
// 		str += m_strRegUrl;
// 	}
// 	else
// 	{
// 		str = _T("�����Ϊ�����Ա�󣬼��ɵõ��Լ����ƹ�����");
// 	}
// 	cacheDC.DrawText(str, rcRegUrl, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

// 	cacheDC.SetTextColor(oldTextColor);
// 	cacheDC.SelectObject(pOldFont);
// 	cacheDC.SetBkMode(oldBkMode);

	if (theAccount.fandian*100>2.799999)
	{
		COLORREF oldTextColor = cacheDC.SetTextColor(RGB(197, 49, 48));
		cacheDC.DrawText(m_strPeie,rcText,DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
		cacheDC.SetTextColor(oldTextColor);
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CHuiYuanTjxjDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanTjxjDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL) 
	{
		return;
	}

	AdjustCtrls();
}

void CHuiYuanTjxjDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		if(theAccount.user_id <= 0)
			return;


		EnableCtrls(theAccount.type == 1);
		InitFanDianCmb();


		int nSel = m_cmbWebFandian.FindString(0,m_strWebFandian1);
		if(nSel == -1)
		{
			m_cmbWebFandian.AddString(_T("0.0"));
			nSel = m_cmbWebFandian.FindString(0,m_strWebFandian1);
			m_cmbWebFandian.SetCurSel(nSel);
		}
		else
		{
			m_cmbWebFandian.SetCurSel(nSel);
		}

		SendToServer(1);
		SendToServer(4);
	}

}

void CHuiYuanTjxjDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(theAccount.type == 0)
	{
		MyMessageBox(_T("ֻ�д����ܹ�����¼�����������ϵ�ͷ��������Ϊ����"));
	}

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CHuiYuanTjxjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk == NULL)
	{
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\tjxj_bg.png"));
		m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
		m_btnRegUrl.SetImage(CBmpUtil::GetExePath() + _T("skin\\fz_bt.png"));
		m_btnSetWebFandian.SetImage(CBmpUtil::GetExePath() + _T("skin\\shezhi.png"));
		m_btnSetMorenMima.SetImage(CBmpUtil::GetExePath() + _T("skin\\shezhi.png"));
		m_font.CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 


	}
	

	((CEdit*)GetDlgItem(IDC_EDIT_ZHANGHAO))->LimitText(12);
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->LimitText(12);
	((CEdit*)GetDlgItem(IDC_EDIT_QK_PWD))->LimitText(12);
	((CEdit*)GetDlgItem(IDC_EDIT_QQ))->LimitText(12);
	m_editZhanghu.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editPassword.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editCrmPwd.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editQkPwd.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editCrmQkPwd.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editQQ.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));

	m_cmbWebFandian.SetFont(&m_font);
	m_cmbWebRegUrl.SetFont(&m_font);
	m_cmbFandian.SetFont(&m_font);
// 	m_rioDaili.SetPaintTextColor(RGB(88,78,77));
// 	m_rioHuiYuan.SetPaintTextColor(RGB(88,78,77));

	//m_ImgRioMan.SetPaintTextColor(RGB(88,78,77));

	//CButton* pWnd = (CButton*)GetDlgItem(IDC_RIO_HUIYUAN);
	//if (pWnd != NULL) 
	//{
	//	pWnd->SetCheck(BST_CHECKED);
	//}
	OnBnClickedRioDaili();

	m_strWebFandian1.Format(L"0.0");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CHuiYuanTjxjDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanTjxjDlg::OnOK()
{
	//CDialog::OnOK();
}

void CHuiYuanTjxjDlg::OnBnClickedRioDaili()
{
	m_type = 1;

	m_rioDaili.SetPushed(true);
	m_rioHuiYuan.SetPushed(false);
}

void CHuiYuanTjxjDlg::OnBnClickedRioHuiyuan()
{
	m_type = 0;

	m_rioDaili.SetPushed(false);
	m_rioHuiYuan.SetPushed(true);
}
void CHuiYuanTjxjDlg::OnBnClickedSetWebFandian()
{
	SendToServer(3);
	return;
}
void CHuiYuanTjxjDlg::OnBnClickedSetMorenMima()
{
	MyMessageBox(L"Ĭ�ϵ�½����Ϊ��a123456\r\nȡ������Ϊ��123456");
	CWnd *pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if(pWnd!=NULL)
	{
		pWnd->SetWindowText(L"a123456");
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_PWD);
	if(pWnd!=NULL)
	{
		pWnd->SetWindowText(L"a123456");
	}
	pWnd = GetDlgItem(IDC_EDIT_QK_PWD);
	if(pWnd!=NULL)
	{
		pWnd->SetWindowText(L"123456");
	}
	pWnd = GetDlgItem(IDC_EDIT_CRM_QK_PWD);
	if(pWnd!=NULL)
	{
		pWnd->SetWindowText(L"123456");
	}
	return;
}
void CHuiYuanTjxjDlg::OnBnClickedBtnOk()
{
	UpdateData();

	//if (m_fandian/100.0 < 0.0) 
	//{
	//	MyMessageBox(_T("���㲻��С����"));
	//	return;
	//}

	//if (m_fandian/100.0 > theAccount.fandian) 
	//{
	//	MyMessageBox(_T("���㲻�ܴ����Լ��ķ���"));
	//	return;
	//}

	m_zhanghu = m_zhanghu.Trim();
	if (m_zhanghu.IsEmpty()) 
	{
		MyMessageBox(_T("�û�������Ϊ��"));
		return;
	}

	if (m_zhanghu.GetLength() > 12) 
	{
		MyMessageBox(_T("�û���Ϊ6-12λ"));
		return;
	}

	if (m_zhanghu.GetLength() < 6) 
	{
		MyMessageBox(_T("�û���Ϊ6-12λ"));
		return;
	}

	m_password = m_password.Trim();
	if (m_password.IsEmpty()) 
	{
		MyMessageBox(_T("��¼���벻��Ϊ��"));
		return;
	}
	if (m_password.GetLength() > 12) 
	{
		MyMessageBox(_T("��¼����Ϊ6-12λ"));
		return;
	}
	if (m_password.GetLength() < 6) 
	{
		MyMessageBox(_T("��¼����Ϊ6-12λ"));
		return;
	}

	m_crmpwd = m_crmpwd.Trim();
	if(m_crmpwd != m_password)
	{
		MyMessageBox(_T("���������¼���벻һ��"));
		return;
	}

	if(m_qkpwd == m_password)
	{
		MyMessageBox(_T("ȡ�����벻�ܵ�¼����һ��"));
		return;

	}
	m_qkpwd = m_qkpwd.Trim();
	if (m_qkpwd.IsEmpty()) 
	{
		MyMessageBox(_T("ȡ�����벻��Ϊ��"));
		return;
	}
	if (m_qkpwd.GetLength() > 12) 
	{
		MyMessageBox(_T("ȡ������Ϊ6-12λ"));
		return;
	}
	if (m_qkpwd.GetLength() < 6) 
	{
		MyMessageBox(_T("ȡ������Ϊ6-12λ"));
		return;
	}

	m_crmqkpwd = m_crmqkpwd.Trim();
	if(m_crmqkpwd != m_qkpwd)
	{
		MyMessageBox(_T("��������ȡ�����벻һ��"));
		return;
	}

	m_strQQ = m_strQQ.Trim();
 	if (!m_strQQ.IsEmpty())
 	{
		if(m_strQQ.GetLength()<5)
		{
 			MyMessageBox(_T("QQ���벻��С��5λ"));
 			return;
		}
 	}

	double fandian = _tstof(m_strFandian);
	SendToServer(2);
}

void CHuiYuanTjxjDlg::OnBnClickedBtnCpRegurl()
{
	const int bufLen = 1024;
	TCHAR text[bufLen] = _T("");

	int nSel = m_cmbWebRegUrl.GetCurSel();

	m_cmbWebRegUrl.GetLBText(nSel,m_strRegUrl);

	
	_tcscpy_s(text, bufLen, m_strRegUrl);
	if (::OpenClipboard(m_hWnd)) 
	{
		::EmptyClipboard();

		HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, (_tcslen(text) + 1) * sizeof(TCHAR));
		LPTSTR pData = (LPTSTR)::GlobalLock(hData);
		_tcscpy_s(pData, _tcslen(text) + 1, text);
		::GlobalUnlock(hData);

		SetClipboardData(CF_UNICODETEXT, hData);
		::CloseClipboard();

		CString strTip;
		strTip.Format(_T("�Ѹ������ӵ�������"), text);
		MyMessageBox(strTip);
	}
	else
	{
		MyMessageBox(_T("δ�ܸ��ƣ�ֱ��ʹ�ü���"));
	}
}

void CHuiYuanTjxjDlg::AdjustCtrls()
{
	if (m_rioDaili.GetSafeHwnd() != NULL) 
	{
		m_rioDaili.SetWindowPos(NULL, rio_daili_x, rio_daili_y, rio_width, rio_height, SWP_NOZORDER);
	}

	if (m_rioHuiYuan.GetSafeHwnd() != NULL) 
	{
		m_rioHuiYuan.SetWindowPos(NULL, rio_huiyuan_x, rio_huiyuan_y, rio_width, rio_height, SWP_NOZORDER);
	}

	//CWnd* pWnd = GetDlgItem(IDC_EDIT_FANDIAN);
	//if (pWnd != NULL) 
	//{
	//	pWnd->SetWindowPos(NULL, edit_x, edit_fandian_y, edit_width, edit_height, SWP_NOZORDER);
	//}

	if (m_cmbFandian.GetSafeHwnd() != NULL) 
	{
		m_cmbFandian.SetWindowPos(NULL, edit_x-2, edit_fandian_y-4, edit_width+3, edit_height+20, SWP_NOZORDER);
	}

	if (m_cmbWebFandian.GetSafeHwnd() != NULL) 
	{
		m_cmbWebFandian.SetWindowPos(NULL,  edit_x-2, web_fandian_y-2, edit_width+3, edit_height+20, SWP_NOZORDER);
	}
	if (m_cmbWebRegUrl.GetSafeHwnd() != NULL) 
	{
		m_cmbWebRegUrl.SetWindowPos(NULL,  edit_x-2, web_fandian_y-36, edit_width+153, edit_height+20, SWP_NOZORDER);
	}

	CWnd* pWnd = GetDlgItem(IDC_EDIT_ZHANGHAO);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_zhanghu_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_dl_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_PWD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_crm_dl_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_dl_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_QK_PWD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_qk_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_QK_PWD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_crm_qk_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_QQ);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_qq_y, edit_width, edit_height-2, SWP_NOZORDER);
	}

	if (m_btnOK.GetSafeHwnd() != NULL) 
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}

	if(m_btnRegUrl.GetSafeHwnd() != NULL) 
	{
		m_btnRegUrl.SetWindowPos(NULL,btn_regurl_x , btn_regurl_y-2, m_btnRegUrl.Width(), m_btnRegUrl.Height(), SWP_NOZORDER);
	}
	if(m_btnSetWebFandian.GetSafeHwnd() != NULL) 
	{
		m_btnSetWebFandian.SetWindowPos(NULL, set_fandian_x, set_fandian_y-2, m_btnSetWebFandian.Width(), m_btnSetWebFandian.Height(), SWP_NOZORDER);
	}
	if(m_btnSetMorenMima.GetSafeHwnd() != NULL) 
	{
		m_btnSetMorenMima.SetWindowPos(NULL, set_mima_x, set_mima_y+5, m_btnSetWebFandian.Width(), m_btnSetWebFandian.Height(), SWP_NOZORDER);
	}
}

void CHuiYuanTjxjDlg::EnableCtrls(BOOL b)
{
	if (m_rioDaili.GetSafeHwnd() != NULL) 
	{
		m_rioDaili.EnableWindow(b);
	}

	if (m_rioHuiYuan.GetSafeHwnd() != NULL) 
	{
		m_rioHuiYuan.EnableWindow(b);
	}

	CWnd* pWnd = GetDlgItem(IDC_EDIT_FANDIAN);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_ZHANGHAO);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_PWD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_QK_PWD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_QK_PWD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	if (m_btnOK.GetSafeHwnd() != NULL) 
	{
		m_btnOK.EnableWindow(b);
	}

	if (m_btnRegUrl.GetSafeHwnd() != NULL)
	{
		m_btnRegUrl.EnableWindow(b);
	}

	if (m_cmbFandian.GetSafeHwnd() != NULL) 
	{
		m_cmbFandian.EnableWindow(b);
	}
	if (m_cmbWebFandian.GetSafeHwnd() != NULL) 
	{
		m_cmbWebFandian.EnableWindow(b);
	}
}

void CHuiYuanTjxjDlg::InitFanDianCmb()
{
	m_cmbFandian.ResetContent();
	if (theAccount.fandian*100-0.1 < 0.0001)
	{
		m_cmbFandian.AddString(_T("0.0"));
		m_cmbFandian.SetCurSel(0);
		return;
	}


	if (theAccount.type == 1) {
		if(theAccount.fandian*100 > 2.799999 && theAccount.fandian*100<2.8000001)
		{
			CString str;
			str.Format(_T("2.8"));
			m_cmbFandian.AddString(str);

		}
		//int i=0;
		for(double f=theAccount.fandian*100-0.1; f>=-0.0000000001; f-=0.1/*, i++*/) 
		{
// 			if(f>2.89999 && f<2.9000001) //�ų� 2.9
// 				continue;
			CString str;
			str.Format(_T("%.1lf"), fabs(f));
			m_cmbFandian.AddString(str);
			m_cmbFandian.SetCurSel(0);
		}
	}
	m_cmbWebFandian.ResetContent();
	if (theAccount.fandian*100-0.1 < 0.0001)
	{
		m_cmbWebFandian.AddString(_T("0.0"));
		m_cmbWebFandian.SetCurSel(0);
		return;
	}

	if (theAccount.type == 1) {
		if(theAccount.fandian*100 > 2.799999 && theAccount.fandian*100<2.8000001)
		{
			CString str;
			str.Format(_T("2.8"));
			m_cmbWebFandian.AddString(str);

		}

		//int i=0;
		for(double f=theAccount.fandian*100-0.1; f>=-0.0000000001; f-=0.1/*, i++*/) 
		{
// 			if(f>2.89999 && f<2.9000001) //�ų� 2.9
//  				continue;
			CString str;
			str.Format(_T("%.1lf"), fabs(f));
			m_cmbWebFandian.AddString(str);
			m_cmbWebFandian.SetCurSel(0);
		}
	}
}





bool CHuiYuanTjxjDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_Peie_RET:
			{
				//Ч�����
				ASSERT(wDataSize%sizeof(CMD_GetPeieRet)==0);
				if (wDataSize%sizeof(CMD_GetPeieRet)!=0) return false;

				CMD_GetPeieRet* pCountRet = (CMD_GetPeieRet*)pData;
				int n_peie_1 = pCountRet->n_t_peie_1;
				int n_peie_2 = pCountRet->n_t_peie_2;
				int n_peie_3 = pCountRet->n_t_peie_3;
				int n_peie_4 = pCountRet->n_t_peie_4;
				int n_peie_5 = pCountRet->n_t_peie_5;
				int n_peie_s_1 = pCountRet->n_t_peie_s_1;
				int n_peie_s_2 = pCountRet->n_t_peie_s_2;
				int n_peie_s_3 = pCountRet->n_t_peie_s_3;
				int n_peie_s_4 = pCountRet->n_t_peie_s_4;
				int n_peie_s_5 = pCountRet->n_t_peie_s_5;
				m_strPeie.Empty();
				if(theAccount.fandian*100 > 3.099999999999 && theAccount.fandian < 3.100000000000001)
				{
					m_strPeie.Format(L"ʣ������3.0��%d����2.9��%d�� ��2.8��%d�� ��2.7��%d�� ��2.6��%d��",n_peie_1-n_peie_s_1,n_peie_2-n_peie_s_2,n_peie_3-n_peie_s_3,n_peie_4-n_peie_s_4,n_peie_5-n_peie_s_5);
				}
				else if(theAccount.fandian*100 > 2.999999999999 && theAccount.fandian*100 < 3.00000001)
				{
					m_strPeie.Format(L"ʣ������2.9��%d����2.8��%d�� ��2.7��%d�� ��2.6��%d��",n_peie_2-n_peie_s_2,n_peie_3-n_peie_s_3,n_peie_4-n_peie_s_4,n_peie_5-n_peie_s_5);
				}
				else if(theAccount.fandian*100 > 2.899999999999 && theAccount.fandian*100 < 2.90000001)
				{
					m_strPeie.Format(L"ʣ������2.8��%d�� ��2.7��%d�� ��2.6��%d��",n_peie_3-n_peie_s_3,n_peie_4-n_peie_s_4,n_peie_5-n_peie_s_5);
				}
				else if(theAccount.fandian*100 > 2.799999999999 && theAccount.fandian*100 < 2.80000001)
				{
					m_strPeie.Format(L"ʣ������2.8��%d�� ��2.7��%d�� ��2.6��%d��",n_peie_3-n_peie_s_3,n_peie_4-n_peie_s_4,n_peie_5-n_peie_s_5);
				}
				else 
				{
					return true;
				}

				RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_ERASE);
				return true;

			}
		case SUB_GR_GET_REG_URL_RET:
			{
				//Ч�����
				ASSERT(wDataSize%sizeof(CMD_GetRegUrl)==0);
				if (wDataSize%sizeof(CMD_GetRegUrl)!=0) return false;

				int nCount = wDataSize/sizeof(CMD_GetRegUrl);

				m_cmbWebRegUrl.ResetContent();
				for(int i =0;i < nCount;i++)
				{
					CMD_GetRegUrl* pCountRet = (CMD_GetRegUrl*)pData+i;

					m_strRegUrl.Format(_T("%s%d"), pCountRet->s_reg_url, theAccount.user_id);
					m_cmbWebRegUrl.AddString(m_strRegUrl);

					m_strWebFandian1.Format(L"%.1lf",pCountRet->f_t_fandian*100);
				}

				int nSelReg = m_cmbWebRegUrl.FindString(0,m_strRegUrl);
				if(nSelReg == -1)
				{
					m_cmbWebRegUrl.AddString(_T("�����Ϊ�����Ա�󣬼��ɵõ��Լ����ƹ�����"));
					nSelReg = m_cmbWebRegUrl.FindString(0,m_strRegUrl);
					m_cmbWebRegUrl.SetCurSel(nSelReg);

				}
				else
					m_cmbWebRegUrl.SetCurSel(nSelReg);

				CRect rcRect;
				rcRect.left = 0;
				rcRect.top = 0;
				rcRect.right = 750;
				rcRect.bottom = 300;


				//CString strFandian;

				int nSel = m_cmbWebFandian.FindString(0,m_strWebFandian1);
				if(nSel == -1)
				{
					m_cmbWebFandian.AddString(_T("0.0"));
					 nSel = m_cmbWebFandian.FindString(0,m_strWebFandian1);
					m_cmbWebFandian.SetCurSel(nSel);
				}
				else
				{
					m_cmbWebFandian.SetCurSel(nSel);
				}

				RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_ERASE);
				return true;
			}
		case SUB_GR_ADD_HY_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_AddHuiYuanRet));
				if(wDataSize!=sizeof(CMD_GR_AddHuiYuanRet)) return false;

				CMD_GR_AddHuiYuanRet* pAddHuiyuan = (CMD_GR_AddHuiYuanRet*)pData;
	
				int res =pAddHuiyuan->n_t_res;
				if(res == 0)
				{
					m_fandian = 0.0;
					m_zhanghu = _T("");
					m_password = _T("");
					m_crmpwd = _T("");
					m_qkpwd = _T("");
					m_crmqkpwd = _T("");
					m_strQQ = _T("");
					UpdateData(FALSE);
				}
// 				else if(res == 1)
// 				{
// 					MyMessageBox(m_zhanghu + _T("-�Ѵ��ڣ��볢�������û���"));
// 				}
// 				else if(res == 2)
// 				{
// 					MyMessageBox(_T("����¼�ʧ��"));
// 				}
				MyMessageBox(pAddHuiyuan->s_t_Desc);
				return true;
			}
		case SUB_GR_SET_WEB_FANDIAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_SetWebFandianRet));
				if(wDataSize!=sizeof(CMD_GR_SetWebFandianRet)) return false;

				CMD_GR_SetWebFandianRet* pSetWebFandianRet = (CMD_GR_SetWebFandianRet*)pData;

				int res =pSetWebFandianRet->n_t_res;
				if(res == 0)
				{
					MyMessageBox(_T("������ҳ����ɹ�"));
				}
				else if(res == 2)
				{
					CString strLog;
					strLog.Format(L"��Ǹ�����÷��㳬�����ƣ�����趨%.2lf",pSetWebFandianRet->f_t_fandian);
					MyMessageBox(strLog);
				}
				else
				{
					MyMessageBox( _T("������ҳ����ʧ��"));
				}
				return true;
			}

		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

VOID CHuiYuanTjxjDlg::SendToServer(int nSendType)
{

	if(nSendType == 1)   //��ע��¼����
	{
		m_bGetRegURL = true;
		if(m_bGetRegURL)
		{
			m_bGetRegURL = false;

			CMD_GetUrlFandian GetUrlFandian;
			ZeroMemory(&GetUrlFandian,sizeof(GetUrlFandian));

			GetUrlFandian.n_t_userid = theAccount.user_id;
			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_REG_URL,&GetUrlFandian,sizeof(GetUrlFandian));
			return;
		}

	}

	if(nSendType == 2)//��ע��¼ 
	{
		m_AddHuiYuan = true;
		if(m_AddHuiYuan)
		{
			m_AddHuiYuan=false;
			CMD_GP_AddHuiYuan AddHuiYuan;
			ZeroMemory(&AddHuiYuan,sizeof(AddHuiYuan));

			AddHuiYuan.n_t_daili_id = theAccount.user_id;
			AddHuiYuan.n_type = m_type;

			DWORD wSel = m_cmbFandian.GetCurSel();
			m_cmbFandian.GetLBText(wSel,m_strFandian);

			DOUBLE fandian = _tstof(m_strFandian);
			AddHuiYuan.f_t_fandian = fandian/100;

			CString dlPwd;
			CString qkPwd;

			TCHAR szDlPwd[33]=TEXT("");
			TCHAR szQkPwd[33]=TEXT("");

			m_password.Trim();
			m_qkpwd.Trim();

			m_zhanghu.Trim();
			CMD5Encrypt::EncryptData(m_password,szDlPwd);
			CMD5Encrypt::EncryptData(m_qkpwd,szQkPwd);

			lstrcpyn(AddHuiYuan.s_t_account,m_zhanghu.GetBuffer(),sizeof(AddHuiYuan.s_t_account));
			lstrcpyn(AddHuiYuan.s_t_login_pass,szDlPwd,sizeof(AddHuiYuan.s_t_login_pass));
			lstrcpyn(AddHuiYuan.s_t_qukuan_pass,szQkPwd,sizeof(AddHuiYuan.s_t_qukuan_pass));
			lstrcpyn(AddHuiYuan.s_t_qq,m_strQQ.GetBuffer(),sizeof(AddHuiYuan.s_t_qq));

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_ADD_HY,&AddHuiYuan,sizeof(AddHuiYuan));
			return;
		}
	}

	if(nSendType == 3)
	{
		m_bSetWebFandian = true;
		if(m_bSetWebFandian)
		{
			m_bSetWebFandian=false;

			CMD_GP_SetWebFandian SetWebFandian;
			ZeroMemory(&SetWebFandian,sizeof(SetWebFandian));

			SetWebFandian.n_t_userid = theAccount.user_id;

			DWORD wSel = m_cmbWebFandian.GetCurSel();
			m_cmbWebFandian.GetLBText(wSel,m_strWebFandian);
			m_strWebFandian.Trim();
			DOUBLE fandian = _tstof(m_strWebFandian);
			SetWebFandian.f_t_fandian = fandian/100;

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_WEB_FANDIAN,&SetWebFandian,sizeof(SetWebFandian));
			return;
		}

	}
	if(nSendType == 4)   //��ע��¼����
	{
		m_bGetRegURL = true;
		if(m_bGetRegURL)
		{
			m_bGetRegURL = false;

			CMD_GetPeie GetPeie;
			ZeroMemory(&GetPeie,sizeof(GetPeie));

			GetPeie.n_t_userid = theAccount.user_id;
			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_Peie,&GetPeie,sizeof(GetPeie));
			return;
		}

	}


}