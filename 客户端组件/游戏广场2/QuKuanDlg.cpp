#include "stdafx.h"
#include "GamePlaza.h"
#include "QukuanDlg.h"
//#include "LuckMeDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

//�Ӵ��ڱ���
static const int bk_qukuan_x = 220;
static const int bk_qukuan_y = 50;
static const int bk_qukuan_width = 725;
static const int bk_qukuan_height = 415;

//���ͼ��
static const int logo_qukuan_x = 0;
static const int logo_qukuan_y = 0;
static const int logo_qukuan_width = 220;
static const int logo_qukuan_height = 617;

//�˺�Edit
static const int edit_act_x = 254;
static const int edit_act_y = 368;
static const int edit_act_width = 255;
static const int edit_act_height = 24;
//����Edit
static const int edit_pwd_x = 254;
static const int edit_pwd_y = 498;
static const int edit_pwd_width = 255;
static const int edit_pwd_height = 24;
//������Edit
static const int edit_sxf_x = 254;
static const int edit_sxf_y = 399;
//���˽��Edit
static const int edit_dzje_x = 254;
static const int edit_dzje_y = 430;

//ȷ�ϰ�ť
static const int btn_ok_x = 341;
static const int btn_ok_y = 537;

static const int tip_width = 255;
static const int tip_height = 24;
static const int tip_x = 240;
static const int tip_youxizhanghu_y = 152;
static CRect rc_youxizhanghu(tip_x, tip_youxizhanghu_y,
tip_x+tip_width, tip_youxizhanghu_y+tip_height);
static const int tip_youxiid_y =238;
static CRect rc_youxiid(tip_x, tip_youxiid_y,
							 tip_x+tip_width, tip_youxiid_y+tip_height);
static const int tip_zhanghuyue_y = 338;
static CRect rc_zhanghuyue(tip_x, tip_zhanghuyue_y,
							 tip_x+tip_width, tip_zhanghuyue_y+tip_height);
static const int tip_qukuanzhanghu_y = 219;
static CRect rc_qukuanzhanghu(tip_x, tip_qukuanzhanghu_y,
							 tip_x+tip_width, tip_qukuanzhanghu_y+tip_height);
static const int tip_kaihuhang_y = 183;
static CRect rc_kaihuhang(tip_x, tip_kaihuhang_y,
							 tip_x+tip_width, tip_kaihuhang_y+tip_height);
static const int tip_kaihuren_y = 253;
static CRect rc_kaihuren(tip_x, tip_kaihuren_y,
							 tip_x+tip_width, tip_kaihuren_y+tip_height);

static const int need_xiaofei_x = 666;
static const int need_xiaofei_y = 333;


static const int tip_vip_x = 253;
static const int tip_vip_y = 121;
static const int tip_vip_width = 550;
static const int tip_vip_height = 18;

static CRect rc_vip(tip_vip_x, tip_vip_y,
						tip_vip_x+tip_vip_width, tip_vip_y+tip_vip_height);


static const int tip_remark_x = 166;
static const int tip_remark_y = 600;
static const int tip_remark_width = 800;
static const int tip_remark_height = 40;

static CRect rc_remark(tip_remark_x, tip_remark_y,
					   tip_remark_x+tip_remark_width, tip_remark_y+tip_remark_height);

IMPLEMENT_DYNAMIC(CQuKuanDlg, CDialog)

CQuKuanDlg::CQuKuanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuKuanDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_bmpQuKuanBk(NULL)
	, m_tikuane(0)
	, m_tikuan_pwd(_T(""))
{
	m_bGetTiKuan=false;
	m_bDoTiKuan=false;
	m_bGetLastYue=false;
	m_cbIsVip = 0;
	m_fNeedXiaoFei=0.00f;
	m_fminQkJine=0.00f;
	m_fmaxQkJine=0.00f;
	m_fMinSxf=0.00f;
	m_fMaxSxf=0.00f;
	m_fNowXiaofei=0.00f;
	m_fNeedXiaofei=0.00f;

}

CQuKuanDlg::~CQuKuanDlg()
{
	if (m_bmpBk != NULL)
	{
		SafeDelete(m_bmpBk);
	}

	if (m_bmpQuKuanBk != NULL)
	{
		SafeDelete(m_bmpQuKuanBk);
	}

}
void CQuKuanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_QUKUAN_ACT, m_editQkAct);
	DDX_Control(pDX, IDC_EDIT_QUKUAN_PWD, m_editQkPwd);
	DDX_Control(pDX, IDC_BTN_QUKUAN, m_btnQuKuan);
	DDX_Control(pDX, IDC_EDIT_SXF, m_editQkSxf);
	DDX_Control(pDX, IDC_EDIT_DZJE, m_editQkDzje);
	DDX_Text(pDX, IDC_EDIT_QUKUAN_ACT, m_tikuane);
	DDX_Text(pDX, IDC_EDIT_QUKUAN_PWD, m_tikuan_pwd);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
// 	DDX_Control(pDX, IDC_EDIT_QUKUAN_ACT, m_editTikuane);
// 	DDX_Control(pDX, IDC_EDIT_QUKUAN_PWD, m_editTkPwd);
}


BEGIN_MESSAGE_MAP(CQuKuanDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_QUKUAN, &CQuKuanDlg::OnBnClickedBtnQukuan)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CQuKuanDlg::OnBnClickedBtnClose)
	ON_EN_CHANGE(IDC_EDIT_QUKUAN_ACT,&CQuKuanDlg::OnEnchangeQkjine)
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()

END_MESSAGE_MAP()

void CQuKuanDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

// CQukuanDlg ��Ϣ�������

BOOL CQuKuanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk == NULL)
	{
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg.png"));
		m_bmpQuKuanBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\qkzx_bg.png"));


		m_font.CreateFont(22, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
		m_font1.CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
		m_ImgTitle.LoadImage(CBmpUtil::GetExePath() + _T("skin\\BTzhgl_04.png"));

	}
	m_btnQuKuan.SetImage(CBmpUtil::GetExePath() + _T("skin\\qdqk.png"));

	m_editQkAct.SetFont(&m_font);
	m_editQkPwd.SetFont(&m_font);
	m_editQkSxf.SetFont(&m_font);
	m_editQkDzje.SetFont(&m_font);

	m_nMfQkCishu = 1;
	m_editQkAct.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editQkPwd.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editQkSxf.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editQkDzje.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\return_bt.png"));
	SetWindowPos(NULL,0,0,m_bmpBk->GetWidth(),m_bmpBk->GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CQuKuanDlg::OnBnClickedBtnClose()
{
// 	CWnd* pParent = GetParent();
// 	if(pParent!=NULL)
// 	{
// 		pParent->PostMessage(IDM_RETURN_GAME,1,0);
// 	}
	CDialog::OnCancel();
	return;
}
void CQuKuanDlg::OnEnchangeQkjine()
{
	CString strTmp;
	CString strTikuan;
	m_editQkAct.GetWindowText(strTikuan);
	DOUBLE ftikuan = _wtof(strTikuan.GetBuffer(0));
	if(ftikuan>theAccount.yue)
	{
		strTikuan.Format(L"%.3lf",theAccount.yue);
		m_editQkAct.SetWindowText(strTikuan);
		return;
	}
	else if(ftikuan>m_fmaxQkJine)
	{
		strTikuan.Format(L"%.3lf",m_fmaxQkJine);
		m_editQkAct.SetWindowText(strTikuan);
		return;

	}
	DOUBLE fSxf = ftikuan*m_nQkSxfPercent/100;
	if(m_nMfQkCishu>0||ftikuan<0.000001)
	{
		fSxf = 0.00;
	}
	else if(fSxf<0.9999999)
	{
		fSxf = 1.000;
	}
	else if(fSxf>25)
	{
		fSxf = 25.00;
	}
	strTmp.Format(L"%.3lf",fSxf);
	m_editQkSxf.SetWindowText(strTmp);
	strTmp.Format(L"%.3lf",ftikuan-fSxf);

	m_editQkDzje.SetWindowText(strTmp);

	strTikuan.ReleaseBuffer();
	return;
}

void CQuKuanDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 
		0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);
	graphics.DrawImage(m_bmpQuKuanBk, Rect(0, 103, m_bmpQuKuanBk->GetWidth(), m_bmpQuKuanBk->GetHeight()), 
		0, 0, m_bmpQuKuanBk->GetWidth(), m_bmpQuKuanBk->GetHeight(), UnitPixel);
	int nX = (rect.Width()-m_ImgTitle.GetWidth())/2+5;
	m_ImgTitle.DrawImage(&cacheDC,nX,30);


	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(56,90,154));

	cacheDC.DrawText(theAccount.account, rc_youxizhanghu, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	CString strTmp;

	if(m_cbIsVip == 1)
	{
		COLORREF oldTextColor1 = cacheDC.SetTextColor(RGB(197, 49, 48));
		strTmp.Format(_T("������VIP��Ա���������������������Լ��������Ȩ��"));
		cacheDC.DrawText(strTmp, rc_vip, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		cacheDC.SetTextColor(oldTextColor1);
	}
// 	strTmp.Format(_T("%d"), theAccount.user_id);
// 	cacheDC.DrawText(strTmp, rc_youxiid, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.2lf"), theAccount.yue);
	cacheDC.DrawText(strTmp, rc_zhanghuyue, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_zhanghu, rc_qukuanzhanghu, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_yinhang, rc_kaihuhang, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_kaihuren, rc_kaihuren, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.SetTextColor(RGB(197, 49, 47));
	cacheDC.SelectObject(&m_font1);
	strTmp.Format(_T("%.2lfԪ"),m_fNeedXiaoFei);
	CRect rcNeed;
	rcNeed.CopyRect(rc_zhanghuyue);

	rcNeed.left = need_xiaofei_x;
	rcNeed.top = need_xiaofei_y;
	rcNeed.right = rcNeed.left+rc_zhanghuyue.Width();
	rcNeed.bottom = rcNeed.top+rc_zhanghuyue.Height();
	cacheDC.DrawText(strTmp, rcNeed, DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%d"),m_nMfQkCishu);
	rcNeed.left += 11;
	rcNeed.right = rcNeed.left +20;
	rcNeed.top+=97;
	rcNeed.bottom += 101;
	cacheDC.DrawText(strTmp, rcNeed, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%d"),m_nQkSxfPercent);
	rcNeed.left += 38;
	rcNeed.right = rcNeed.left +15;
	rcNeed.top-=34;
	rcNeed.bottom -= 34;
	cacheDC.DrawText(strTmp, rcNeed, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.0lf"),m_fMinSxf);
	rcNeed.left += 120;
	rcNeed.right = rcNeed.left +15;
	cacheDC.DrawText(strTmp, rcNeed, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.0lf"),m_fMaxSxf);
	rcNeed.left += 94;
	rcNeed.right = rcNeed.left +15;
	cacheDC.DrawText(strTmp, rcNeed, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.0lf"),m_fminQkJine);
	rcNeed.left -= 307;
	rcNeed.right = rcNeed.left +40;
	rcNeed.top-=31;
	rcNeed.bottom -= 31;
	cacheDC.DrawText(strTmp, rcNeed, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.0lf"),m_fmaxQkJine);
	rcNeed.left += 151;
	rcNeed.right = rcNeed.left +40;
	cacheDC.DrawText(strTmp, rcNeed, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.SetTextColor(RGB(255,0,0));
	cacheDC.DrawText(m_strRemark, rc_remark, DT_LEFT|DT_VCENTER);


	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CQuKuanDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CQuKuanDlg::OnSize(UINT nType, int cx, int cy)
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

	if(m_editQkAct.GetSafeHwnd() != NULL)
	{
		m_editQkAct.SetWindowPos(NULL,edit_act_x, edit_act_y, edit_act_width, edit_act_height, SWP_NOZORDER);
	}

	if(m_editQkPwd.GetSafeHwnd() != NULL)
	{
		m_editQkPwd.SetWindowPos(NULL, edit_pwd_x, edit_pwd_y, edit_pwd_width, edit_pwd_height, SWP_NOZORDER);
	}
	if(m_editQkSxf.GetSafeHwnd() != NULL)
	{
		m_editQkSxf.SetWindowPos(NULL, edit_sxf_x, edit_sxf_y, edit_pwd_width, edit_pwd_height, SWP_NOZORDER);
	}
	if(m_editQkDzje.GetSafeHwnd() != NULL)
	{
		m_editQkDzje.SetWindowPos(NULL, edit_dzje_x, edit_dzje_y, edit_pwd_width, edit_pwd_height, SWP_NOZORDER);
	}

	if(m_btnQuKuan.GetSafeHwnd() != NULL)
	{
		m_btnQuKuan.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnQuKuan.Width(), m_btnQuKuan.Height(), SWP_NOZORDER);
	}
}

void CQuKuanDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
		m_btnQuKuan.EnableWindow(FALSE);
		SendToServer(1);
		SendToServer(3);
		m_editQkAct.SetWindowText(L"");
		m_editQkPwd.SetWindowText(L"");
		m_yinhang.Empty();
		m_kaihuren.Empty();
		m_zhanghu.Empty();
	}
}

void CQuKuanDlg::OnBnClickedBtnQukuan()
{
	CTime t;
	theApp.GetTime(t);

	if(m_cbIsVip != 1 &&m_nLastCount<=0&&m_nAllCount!=0)
	{
		CString strLog;
		strLog.Format(L"��ܰ��ʾ','������VIP��Ա,ÿ�������ȡ��%d��!",m_nAllCount);
		MyMessageBox(strLog);
		return ;

	}
	int nStart = 0;
	int nEnd = 0;

	if(m_nStartHour < m_nEndHour)
	{
		if(t.GetHour() <m_nStartHour || t.GetHour() > m_nEndHour)
		{
			CString strLog;
			strLog.Format(L"��ܰ��ʾ:ȡ��ʱ��Ϊ%d�㵽%d��֮��!",m_nStartHour,m_nEndHour);
			MyMessageBox(strLog);
			return ;
		}

	}
	else if(m_nStartHour>m_nEndHour)
	{
		if(t.GetHour() <m_nStartHour && t.GetHour() > m_nEndHour)
		{
			CString strLog;
			strLog.Format(L"��ܰ��ʾ:ȡ��ʱ��Ϊ%d�㵽�ڶ���%d��֮��!",m_nStartHour,m_nEndHour);
			MyMessageBox(strLog);
			return ;
		}

	}
	if (theAccount.yue < 0.01)
	{
		MyMessageBox(_T("�������Ϊ��"));
		return;
	}

	if (m_yinhang.IsEmpty() || m_kaihuren.IsEmpty() || m_zhanghu.IsEmpty())
	{
		MyMessageBox(_T("��������˻���Ϣ���ò������������úú��ٳ������"));
		return;
	}

	if(m_fNowXiaofei<m_fNeedXiaoFei&&theAccount.fandian<0.028)
	{
		CString strLog;
		strLog.Format(L"QUKUAN xiaofei:%.3lf,need:%.3lf",m_fNowXiaofei,m_fNeedXiaoFei);
		OutputDebugString(strLog);
		MyMessageBox(_T("��ܰ��ʾ:�Բ�������������û�дﵽ��ֵ��30%!"));
		return ;

	}
	UpdateData();
	if (m_tikuane > theAccount.yue)
	{
		MyMessageBox(_T("���㣬���������������"));
		return;
	}

	if(m_tikuane < m_fminQkJine)
	{
		CString strLog;
		strLog.Format(L"��ܰ��ʾ:���������ܳ���%.0lfԪ�����%.0lfԪ",m_fmaxQkJine,m_fminQkJine);
		MyMessageBox(strLog);
		return;
	}

	m_tikuan_pwd = m_tikuan_pwd.Trim();
	if(m_tikuan_pwd.IsEmpty())
	{
		MyMessageBox(_T("������벻��Ϊ��"));
		return;
	}

	m_btnQuKuan.EnableWindow(FALSE);
	SendToServer(2);
}

void CQuKuanDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
}

void CQuKuanDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}

//��ȡ�¼�
bool CQuKuanDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_QU_KUAN_INFO_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetQukuanInfoRet));
				if(wDataSize!=sizeof(CMD_GP_GetQukuanInfoRet)) return false;

				CMD_GP_GetQukuanInfoRet* pCountRet = (CMD_GP_GetQukuanInfoRet*)pData;

				theAccount.yue = pCountRet->f_t_yue;
				CString strTmp;
				strTmp.Format(L"%s",pCountRet->s_t_yinhang);
				
				m_yinhang.Format(L"%s",strTmp);
				m_yinhang.Trim();
				strTmp.Format(L"%s",pCountRet->s_t_kaihuren);
				m_kaihuren.Format(L"%s**",strTmp.Left(1));
				strTmp.Format(L"%s",pCountRet->s_t_zhanghao);
				m_zhanghu.Format(L"%s**********%s",strTmp.Left(4),strTmp.Right(4));
				m_cbIsVip = pCountRet->n_t_isvip;
				if(m_zhanghu.IsEmpty()||m_kaihuren.IsEmpty()||m_yinhang.IsEmpty())
				{
					MyMessageBox(L"���ڡ��˺Ź�����������˺ţ�");
					return true;
				}

				m_btnQuKuan.EnableWindow(TRUE);
				Invalidate(FALSE);
				break;
			}
		case SUB_GP_DO_QU_KUAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_DoQukuanRet));
				if(wDataSize!=sizeof(CMD_GP_DoQukuanRet)) return false;

				CMD_GP_DoQukuanRet* pCountRet = (CMD_GP_DoQukuanRet*)pData;

				int res = pCountRet->nResult;
				if (res == 0)
				{
					MyMessageBox(pCountRet->szDesc);

					//theAccount.yue -= m_tikuane;
					m_tikuane = 0.0;
					m_tikuan_pwd = _T("");
					UpdateData(FALSE);
					m_editQkAct.SetWindowText(L"");
					m_btnQuKuan.EnableWindow(TRUE);
					m_editQkPwd.SetWindowText(L"");
					SendToServer(1);
					SendToServer(3);
					Invalidate(FALSE);
					break;
					//SendToServer(3);
				}
				else 
				{
					MyMessageBox(pCountRet->szDesc);
				}
// 				else if (res == 2)
// 				{
// 					MyMessageBox(_T("������߳�����������޶�"));
// 				}
// 				else if (res == 3)
// 				{
// 					CString strLog;
// 					strLog.Format(L"ȡ��������ܳ���%.0lfԪ, ���ܵ�����ͽ��%.0lfԪ",m_fmaxQkJine,m_fminQkJine);
// 					MyMessageBox(strLog);
// 				}
// 				else if (res == 4)
// 				{
// 					MyMessageBox(_T("�����������������"));
// 				}
// 				else if (res == 5)
// 				{
// 					MyMessageBox(_T("���������������ʧ�ܣ����Ժ�����"));
// 				}
// 				else if (res == 6)
// 				{
// 					MyMessageBox(_T("��ܰ��ʾ��ȡ��ʱ��Ϊ����10�㵽�賿2��֮��"));
// 				}
// 				else if (res == 7)
// 				{
// 					MyMessageBox(_T("�Բ���������ע���û�������3Сʱ����ȡ�"));
// 				}
// 				else if(res == 8)
// 				{
// 					MyMessageBox(_T("�Բ�����û��ȡ��Ȩ�ޣ�"));
// 				}
// 				else if(res == 9)
// 				{
// 					MyMessageBox(_T("�Բ�������ȡ��ܱ���ͣ��"));
// 				}
// 				else if (res == 10)
// 				{
// 					MyMessageBox(_T("�Բ�������������û�дﵽ��ֵ��30%"));
// 				}
				m_btnQuKuan.EnableWindow(TRUE);
				break;
			}
		case SUB_GP_GET_LAST_YUE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLastYueRet));
				if(wDataSize!=sizeof(CMD_GP_GetLastYueRet)) return false;

				CMD_GP_GetLastYueRet* pCountRet = (CMD_GP_GetLastYueRet*)pData;

				theAccount.yue = pCountRet->f_t_yue;
				theAccount.dongjie = pCountRet->f_t_dongjie;
				Invalidate(FALSE);
			}
		case SUB_GP_GET_QUKUAN_LIMIT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetQukuanLimitRet));
				if(wDataSize!=sizeof(CMD_GP_GetQukuanLimitRet)) return false;

				
				CMD_GP_GetQukuanLimitRet* pCountRet = (CMD_GP_GetQukuanLimitRet*)pData;
				m_fNeedXiaoFei = pCountRet->f_t_xiaofei;
				m_nQkSxfPercent = pCountRet->n_t_sxfpercent;
				m_nMfQkCishu = pCountRet->n_t_freelastcount;
				m_fMinSxf = pCountRet->f_t_lowsxf;
				m_fMaxSxf = pCountRet->f_t_highsxf;
				m_fminQkJine = pCountRet->f_t_lowjine;
				m_fmaxQkJine = pCountRet->f_t_highjine;
				m_strRemark.Format(L"%s",pCountRet->s_t_remark);
				m_nStartHour = pCountRet->t_starttime;
				m_nEndHour  = pCountRet->t_endtime;

				m_nLastCount = pCountRet->n_t_lastcount;
				m_nAllCount = pCountRet->n_t_allcount;
				m_fNeedXiaofei = pCountRet->f_t_xiaofei;
				m_fNowXiaofei = pCountRet->f_t_nowxiaofei;
				Invalidate(FALSE);

				m_editQkAct.SetFocus();
				return TRUE;
			}	
		}
	}
//	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	return true;
	//�������
	ASSERT(FALSE);

	return true;
}

VOID CQuKuanDlg::SendToServer(BYTE nSendType)
{
	if(nSendType == 1)
	{
		m_bGetTiKuan=true;
		if(m_bGetTiKuan)
		{
			m_bGetTiKuan = false;
			CMD_GP_GetQukuanInfo GetQukuanInfo;
			ZeroMemory(&GetQukuanInfo,sizeof(GetQukuanInfo));

			GetQukuanInfo.dwUserID = theAccount.user_id;

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_QU_KUAN_INFO,&GetQukuanInfo,sizeof(GetQukuanInfo));
		}
		return;

	}

	if(nSendType == 2)
	{
		m_bDoTiKuan=true;
		if(m_bDoTiKuan)
		{
			m_bDoTiKuan = false;
			CMD_GP_DoQukuan DoQukuan;
			ZeroMemory(&DoQukuan,sizeof(DoQukuan));

			DoQukuan.dwUserID = theAccount.user_id;
			DoQukuan.fJine = m_tikuane;

			TCHAR szPassword[33];
			memset(szPassword, 0, sizeof(szPassword));
			CMD5Encrypt::EncryptData(m_tikuan_pwd,szPassword);

			lstrcpyn(DoQukuan.szQukuanPass,szPassword,sizeof(DoQukuan.szQukuanPass));

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_DO_QU_KUAN,&DoQukuan,sizeof(DoQukuan));
		}
		return;

	}
	
	if(nSendType == 3)
	{
		CMD_GP_GetQukuanLimit GetQukuanLimit;
		ZeroMemory(&GetQukuanLimit,sizeof(GetQukuanLimit));

		GetQukuanLimit.n_t_userid = theAccount.user_id;

		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_QUKUAN_LIMIT,&GetQukuanLimit,sizeof(GetQukuanLimit));
		return;
	}

}