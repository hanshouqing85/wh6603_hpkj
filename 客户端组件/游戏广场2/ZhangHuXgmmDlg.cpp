#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhangHuXgmmDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
#include "YanZhengMa.h"

static const int edit_width = 146;
static const int edit_height = 20;

//账户1
static const int dl_account_x = 301;
static const int dl_account_y = 92;
static CRect rc_dl_account(dl_account_x, dl_account_y, dl_account_x+edit_width, dl_account_y+edit_height);

//旧登陆密码
static const int dl_old_pwd_x = 299;
static const int dl_old_pwd_y = 130;
//新登陆密码
static const int dl_new_pwd_x = 299;
static const int dl_new_pwd_y = 171;
//确认新登陆密码
static const int dl_cfm_pwd_x = 299;
static const int dl_cfm_pwd_y = 210;
//确定按钮
static const int dl_ok_x = 316;
static const int dl_ok_y = 238;

//手机号编辑框
static const int dl_bd_sjh_x = 299;
static const int dl_bd_sjh_y = 316;

//账户1
static const int qk_account_x = 658;
static const int qk_account_y = 92;
static CRect rc_qk_account(qk_account_x, qk_account_y, qk_account_x+edit_width, qk_account_y+edit_height);
//旧取款密码
static const int qk_old_pwd_x = 654;
static const int qk_old_pwd_y = 130;
//新取款密码
static const int qk_new_pwd_x = 654;
static const int qk_new_pwd_y = 171;
//确认新取款密码
static const int qk_cfm_pwd_x = 654;
static const int qk_cfm_pwd_y = 210;
//确定按钮
static const int qk_ok_x = 679;
static const int qk_ok_y = 238;


IMPLEMENT_DYNAMIC(CZhangHuXgmmDlg, CDialog)

CZhangHuXgmmDlg::CZhangHuXgmmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZhangHuXgmmDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_old_pwd(_T(""))
	, m_new_pwd(_T(""))
	, m_confirm_pwd(_T(""))
{
	m_bAlterLogonPass=false;
	m_bAlterQukuanPass=false;
}

CZhangHuXgmmDlg::~CZhangHuXgmmDlg()
{
	if(m_bmpBk != NULL)
	{
		SafeDelete(m_bmpBk);
	}
}

void CZhangHuXgmmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLD_PWD, m_old_pwd);
	DDX_Text(pDX, IDC_EDIT_NEW_PWD, m_new_pwd);
	DDX_Text(pDX, IDC_EDIT_CONFIRM, m_confirm_pwd);
	DDX_Control(pDX, IDC_EDIT_OLD_PWD, m_editOldPwd);
	DDX_Control(pDX, IDC_EDIT_NEW_PWD, m_editNewPwd);
	DDX_Control(pDX, IDC_EDIT_CONFIRM, m_editConfirmPwd);
	DDX_Control(pDX, IDC_EDIT_BANGDING_PHONE, m_editPhone);
	DDX_Control(pDX, IDC_BTN_BD, m_btnBdPhone);
	DDX_Control(pDX, IDC_BTN_QRXG, m_btnQrXg);


	DDX_Control(pDX, IDC_BTN_OK, m_btnDlOK);
	DDX_Text(pDX, IDC_EDIT_OLD_PWD2, m_old_pwd_qk);
	DDX_Text(pDX, IDC_EDIT_NEW_PWD2, m_new_pwd_qk);
	DDX_Text(pDX, IDC_EDIT_CONFIRM2, m_confirm_pwd_qk);
	DDX_Control(pDX, IDC_EDIT_OLD_PWD2, m_editOldPwdQk);
	DDX_Control(pDX, IDC_EDIT_NEW_PWD2, m_editNewPwdQk);
	DDX_Control(pDX, IDC_EDIT_CONFIRM2, m_editConfirmPwdQk);
	DDX_Control(pDX, IDC_BTN_OK2, m_btnQkOK);
	DDX_Control(pDX, IDC_BTN_LOCKMACHINE, m_btnLockMachine);
}


BEGIN_MESSAGE_MAP(CZhangHuXgmmDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CZhangHuXgmmDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_OK2, &CZhangHuXgmmDlg::OnBnClickedBtnOk2)
	ON_BN_CLICKED(IDC_BTN_LOCKMACHINE, &CZhangHuXgmmDlg::OnBnClickedBtnLockMachine)
	ON_BN_CLICKED(IDC_BTN_BD, &CZhangHuXgmmDlg::OnBnClickedBtnBdPhone)
	ON_BN_CLICKED(IDC_BTN_QRXG, &CZhangHuXgmmDlg::OnBnClickedBtnQrxg)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CZhangHuXgmmDlg 消息处理程序
void CZhangHuXgmmDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.moormachine  == 0)
			m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\sdbj.png"));
		else if(theAccount.moormachine  == 1)
			m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\jsjq.png"));
		m_cbBangDing = 0;
		m_cbOperateType = 1;
		SendToServer(5);
	}
	else
	{
		m_old_pwd = _T("");
		m_new_pwd = _T("");
		m_confirm_pwd = _T("");
		UpdateData(FALSE);

	}
	return;
}

void CZhangHuXgmmDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(56,90,154));

	cacheDC.DrawText(theAccount.account, rc_dl_account, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(theAccount.account, rc_qk_account, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	m_btnLogonCheck.OnDrawControl(&cacheDC);
	m_btnTransferCheck.OnDrawControl(&cacheDC);
	m_btnStockCheck.OnDrawControl(&cacheDC);
	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);
	
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CZhangHuXgmmDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CZhangHuXgmmDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

BOOL CZhangHuXgmmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk!=NULL)
		return TRUE;
	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\xgmm_bg.png"));

	m_btnDlOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnQkOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\sdbj.png"));
	m_btnBdPhone.SetImage(CBmpUtil::GetExePath() + _T("skin\\bangding.png"));

	m_btnQrXg.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_editOldPwd.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editNewPwd.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editConfirmPwd.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editPhone.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));

	m_editPhone.ModifyStyle(NULL,ES_NUMBER);
	m_editOldPwdQk.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editNewPwdQk.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editConfirmPwdQk.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));

	m_btnLogonCheck.SetParentWndSink(this);
	m_btnLogonCheck.LoadCheckImage(_T("BT_LOGON_CHECK"));
	m_btnTransferCheck.SetParentWndSink(this);
	m_btnTransferCheck.LoadCheckImage(_T("BT_TRANSFER_CHECK"));
	m_btnStockCheck.SetParentWndSink(this);
	m_btnStockCheck.LoadCheckImage(_T("BT_STOCK_CHECK"));

	m_btnLogonCheck.SetControlBenchmark(dl_bd_sjh_x-4,dl_bd_sjh_y+34);
	
	m_btnTransferCheck.SetControlBenchmark(dl_bd_sjh_x-4+122,dl_bd_sjh_y+34);
	m_btnStockCheck.SetControlBenchmark(dl_bd_sjh_x-14+127*2,dl_bd_sjh_y+34);
	m_dwTickCount = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CZhangHuXgmmDlg::OnBnClickedBtnLockMachine()
{
	SendToServer(3);
	return;
}

void CZhangHuXgmmDlg::OnBnClickedBtnBdPhone()
{
	CString strPhoneNum;
	m_editPhone.GetWindowText(strPhoneNum);
	if(m_cbBangDing == 1 )
	{
		strPhoneNum = m_strPhoneNum;
	}
	if(strPhoneNum.IsEmpty()||strPhoneNum.GetLength()!=11)
	{
		MyMessageBox(L"请填入正确的手机号码！");
		return;
	}
	CYanZhengMaDlg YanZhengMa;
	if(m_cbBangDing == 0)
		YanZhengMa.SetYanZhengXinxi(strPhoneNum,0,0);
	else
		YanZhengMa.SetYanZhengXinxi(strPhoneNum,0,1);
	if(YanZhengMa.DoModal()==IDOK)
	{
		//MyMessageBox(L"验证成功");
		if(m_cbBangDing == 0)
			SendToServer(4);
		else
			SendToServer(6);
	}

	return;
}

void CZhangHuXgmmDlg::OnBnClickedBtnQrxg()
{
	m_cbOperateType = 2;
	SendToServer(5);
	return;
}
void CZhangHuXgmmDlg::OnBnClickedBtnOk()
{
	if(m_dwTickCount == 0)
	{
		m_dwTickCount = GetTickCount();
	}
	else if( GetTickCount()-m_dwTickCount <1000)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	UpdateData();

	if (m_old_pwd.IsEmpty()) 
	{
		MyMessageBox(_T("密码不能为空"));
		return;
	}

	if (m_old_pwd.GetLength() >= PASSWORD_LENGTH) 
	{
		CString msg;
		msg.Format(_T("密码限制长度在【%d】字符之内"), PASSWORD_LENGTH-1);
		MyMessageBox(msg);
		return;
	}

	if (m_new_pwd.IsEmpty()) 
	{
		MyMessageBox(_T("密码不能为空"));
		return;
	}

	if (m_new_pwd.GetLength() >= PASSWORD_LENGTH) 
	{
		CString msg;
		msg.Format(_T("密码限制长度在【%d】字符之内"), PASSWORD_LENGTH-1);
		MyMessageBox(msg);
		return;
	}

	if (m_new_pwd != m_confirm_pwd) 
	{
		MyMessageBox(_T("请确认新密码输入正确"));
		return;
	}

	SendToServer(1); //修改登录密码
	//数据库暂时屏蔽  Alter by a'long
// 	int res = theDBCmd.ModifyLoginPwd(theAccount.user_id, m_old_pwd, m_new_pwd);
// 	if (res == 0) 
// 	{
// 		CString msg;
// 		msg.Format(_T("您已经成功修改密码"));
// 		MyMessageBox(msg);
// 		
// 		m_old_pwd = _T("");
// 		m_new_pwd = _T("");
// 		m_confirm_pwd = _T("");
// 		UpdateData(FALSE);
// 	}
// 	else if (res == 1)
// 	{
// 		MyMessageBox(_T("旧密码不正确"));
// 		return;
// 	}
// 	else if(res == 2)
// 	{
// 		MyMessageBox(_T("修改密码失败，请重试"));
// 		return;
// 	}
}

void CZhangHuXgmmDlg::OnBnClickedBtnOk2()
{
	if(m_dwTickCount == 0)
	{
		m_dwTickCount = GetTickCount();
	}
	else if( GetTickCount()-m_dwTickCount <1000)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	UpdateData();

	if (m_old_pwd_qk.IsEmpty()) 
	{
		MyMessageBox(_T("密码不能为空"));
		return;
	}

	if (m_old_pwd_qk.GetLength() >= PASSWORD_LENGTH) 
	{
		CString msg;
		msg.Format(_T("密码限制长度在【%d】字符之内"), PASSWORD_LENGTH-1);
		MyMessageBox(msg);
		return;
	}

	if (m_new_pwd_qk.IsEmpty()) 
	{
		MyMessageBox(_T("密码不能为空"));
		return;
	}

	if (m_new_pwd_qk.GetLength() >= PASSWORD_LENGTH) 
	{
		CString msg;
		msg.Format(_T("密码限制长度在【%d】字符之内"), PASSWORD_LENGTH-1);
		MyMessageBox(msg);
		return;
	}

	if (m_new_pwd_qk != m_confirm_pwd_qk) 
	{
		MyMessageBox(_T("请确认新密码输入正确"));
		return;
	}

	SendToServer(2);
	//数据库暂时屏蔽  Alter by a'long
// 	int res = theDBCmd.ModifyQuKuanPwd(theAccount.user_id, m_old_pwd_qk, m_new_pwd_qk);
// 	if (res == 0) 
// 	{
// 		CString msg;
// 		msg.Format(_T("您已经成功修改密码"));
// 		MyMessageBox(msg);
// 
// 		m_old_pwd_qk = _T("");
// 		m_new_pwd_qk = _T("");
// 		m_confirm_pwd_qk = _T("");
// 		UpdateData(FALSE);
// 	}
// 	else if (res == 1)
// 	{
// 		MyMessageBox(_T("旧密码不正确"));
// 		return;
// 	}
// 	else if(res == 2)
// 	{
// 		MyMessageBox(_T("修改密码失败，请重试"));
// 		return;
// 	}
}

void CZhangHuXgmmDlg::AdjustCtrls()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_OLD_PWD);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, dl_old_pwd_x, dl_old_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_NEW_PWD);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, dl_new_pwd_x, dl_new_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_CONFIRM);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, dl_cfm_pwd_x, dl_cfm_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_BANGDING_PHONE);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, dl_bd_sjh_x, dl_bd_sjh_y, edit_width, edit_height, SWP_NOZORDER);
	//	pWnd->ShowWindow(SW_HIDE);
	}
	
	if(m_btnDlOK.GetSafeHwnd() != NULL)
	{
		m_btnDlOK.SetWindowPos(NULL, dl_ok_x-18, dl_ok_y, m_btnDlOK.Width(), m_btnDlOK.Height(), SWP_NOZORDER);
	}

	//--------------------------------------------------------------------------------
	pWnd = GetDlgItem(IDC_EDIT_OLD_PWD2);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, qk_old_pwd_x, qk_old_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_NEW_PWD2);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, qk_new_pwd_x, qk_new_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_CONFIRM2);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, qk_cfm_pwd_x, qk_cfm_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	if(m_btnQkOK.GetSafeHwnd() != NULL)
	{
		m_btnQkOK.SetWindowPos(NULL, qk_ok_x-23, qk_ok_y, m_btnQkOK.Width(), m_btnQkOK.Height(), SWP_NOZORDER);
	}
	if(m_btnLockMachine.GetSafeHwnd() != NULL)
	{
		m_btnLockMachine.SetWindowPos(NULL, qk_ok_x+ m_btnQkOK.Width()-10, qk_ok_y, m_btnLockMachine.Width(), m_btnLockMachine.Height(), SWP_NOZORDER);
	}
	if(m_btnBdPhone.GetSafeHwnd() != NULL)
	{
		m_btnBdPhone.SetWindowPos(NULL, dl_bd_sjh_x+ edit_width+10, dl_bd_sjh_y-5, m_btnBdPhone.Width(), m_btnBdPhone.Height(), SWP_NOZORDER);
		//m_btnBdPhone.ShowWindow(SW_HIDE);
	}
	if(m_btnQrXg.GetSafeHwnd() != NULL)
	{
		m_btnQrXg.SetWindowPos(NULL, dl_bd_sjh_x+ edit_width+214, dl_bd_sjh_y+31, m_btnQrXg.Width(), m_btnQrXg.Height(), SWP_NOZORDER);
		//m_btnQrXg.ShowWindow(SW_HIDE);
	}
}

void CZhangHuXgmmDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CZhangHuXgmmDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}


//读取事件
bool CZhangHuXgmmDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_UNBIND_PHONE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_UnBindPhone_RET));
				if(wDataSize!=sizeof(CMD_GR_UnBindPhone_RET))
					return false;
				m_cbOperateType = 1;
				SendToServer(5);

				CMD_GR_UnBindPhone_RET* pPhoneInfo = (CMD_GR_UnBindPhone_RET*)pData;
				MyMessageBox(pPhoneInfo->sDesc);
				return true;
			}
		case SUB_GR_BIND_PHONE_INFO_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_BindPhoneInfo_RET));
				if(wDataSize!=sizeof(CMD_GR_BindPhoneInfo_RET))
					return false;

				CMD_GR_BindPhoneInfo_RET* pPhoneInfo = (CMD_GR_BindPhoneInfo_RET*)pData;
				CString strBindPhone;
				strBindPhone.Format(L"%s",pPhoneInfo->sPhoneNum);
				m_strPhoneNum.Format(L"%s",pPhoneInfo->sPhoneNum);
				if(pPhoneInfo->nResult == 1&&m_cbOperateType==2)
				{
					MyMessageBox(L"设置失败，请重新设置！");
					return true;
				}
				if(!strBindPhone.IsEmpty()&&m_cbOperateType!=2)
				{
					
					m_cbBangDing = 1;
					CString strPhone;
					strPhone.Format(L"%s****%s",strBindPhone.Left(3),strBindPhone.Right(4));
					m_editPhone.SetWindowText(strPhone);
					m_editPhone.EnableWindow(FALSE);

					m_btnLogonCheck.SetButtonChecked((pPhoneInfo->cLoginverify==1)?true:false);
					m_btnTransferCheck.SetButtonChecked((pPhoneInfo->cTranferAccount==1)?true:false);
					m_btnStockCheck.SetButtonChecked((pPhoneInfo->cfenhong==1)?true:false);
					m_btnBdPhone.SetImage(CBmpUtil::GetExePath() + _T("skin\\jiebang.png"));
				}
				else if(m_cbOperateType==2)
				{
					if(pPhoneInfo->nResult == 0)
					{
						m_cbOperateType=1;
						SendToServer(5);
						MyMessageBox(L"设置成功！");
						return true;

					}
				}
				else
				{
					m_editPhone.SetWindowText(strBindPhone);
					if(strBindPhone.IsEmpty())
					{
						m_editPhone.EnableWindow(TRUE);
					}
					m_btnLogonCheck.SetButtonChecked((!strBindPhone.IsEmpty())?true:false);
					m_btnTransferCheck.SetButtonChecked((!strBindPhone.IsEmpty())?true:false);
					m_btnStockCheck.SetButtonChecked((!strBindPhone.IsEmpty())?true:false);

					m_btnBdPhone.SetImage(CBmpUtil::GetExePath() + _T("skin\\bangding.png"));
					//m_btnBdPhone.Invalidate();

					m_cbBangDing = 0;
				}
				return true;
			}
		case SUB_GR_BIND_PHONE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_BindPhone_RET));
				if(wDataSize!=sizeof(CMD_GR_BindPhone_RET))
					return false;

				CMD_GR_BindPhone_RET* pBindPhone = (CMD_GR_BindPhone_RET*)pData;
				m_cbOperateType = 1;
				SendToServer(5);
				if(pBindPhone->lResult == 1)
				{
					MyMessageBox(L"成功绑定手机！");
				}
				else if(pBindPhone->lResult == 2)
				{
					MyMessageBox(L"号码已绑定其他账号！");
				}
				else if(pBindPhone->lResult == 3)
				{
					MyMessageBox(L"账号已绑定其他手机号码！");
				}
				else
				{
					MyMessageBox(L"绑定手机号失败！");
				}


				return true;
			}
		case SUB_GR_LOCK_MACHINE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_LockMachine_RET));
				if(wDataSize!=sizeof(CMD_GR_LockMachine_RET))
					return false;

				CMD_GR_LockMachine_RET* pLockRet = (CMD_GR_LockMachine_RET*)pData;

				if(pLockRet->lResult == 0)
				{
					theAccount.moormachine = (theAccount.moormachine==1)?0:1;
					if(theAccount.moormachine  == 0)
					{
						m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\sdbj.png"));
						MyMessageBox(L"解锁成功！");
					}
					else if(theAccount.moormachine  == 1)
					{
						m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\jsjq.png"));

						MyMessageBox(L"锁定机器成功！");
					}
				}
				else 
				{
					MyMessageBox(L"锁定机器失败！");
				}
				return true;
				
			}
		case SUB_GR_XG_LOGIN_PASS_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_XG_Loginpass_RET));
				if(wDataSize!=sizeof(CMD_GR_XG_Loginpass_RET))
					return false;

				CMD_GR_XG_Loginpass_RET* pCountRet = (CMD_GR_XG_Loginpass_RET*)pData;

				int lResult = pCountRet->lResult;
				if(lResult == 0)
				{
					CString msg;
					msg.Format(_T("您已经成功修改密码"));
					MyMessageBox(msg);

					m_old_pwd = _T("");
					m_new_pwd = _T("");
					m_confirm_pwd = _T("");
					UpdateData(FALSE);

				}
				else if (lResult == 1)
				{
					MyMessageBox(_T("旧密码不正确"));
					return true;
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("修改密码失败，请重试"));
					return true;
				}

				return true;
			}
		case SUB_GR_XG_QUKUAN_PASS_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_XG_Qukuanpass_RET));
				if(wDataSize!=sizeof(CMD_GR_XG_Qukuanpass_RET))
					return false;

				CMD_GR_XG_Qukuanpass_RET* pXGqqRet = (CMD_GR_XG_Qukuanpass_RET*)pData;

				int lResult = pXGqqRet->lResult;
				if(lResult == 0)
				{
					CString msg;
					msg.Format(_T("您已经成功修改密码"));
					MyMessageBox(msg);

					m_old_pwd = _T("");
					m_new_pwd = _T("");
					m_confirm_pwd = _T("");
					UpdateData(FALSE);

				}
				else if (lResult == 1)
				{
					MyMessageBox(_T("旧密码不正确"));
					return true;
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("修改密码失败，请重试"));
					return true;
				}
				return true;
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CZhangHuXgmmDlg::SendToServer(int nSendType)
{

	if(nSendType == 1)   //获取用户信息
	{
		m_bAlterLogonPass = true;
		if(m_bAlterLogonPass)
		{
			m_bAlterLogonPass = false;


			CMD_GP_XG_Loginpass XGLoginPass;
			ZeroMemory(&XGLoginPass,sizeof(XGLoginPass));

			XGLoginPass.dwUserID = theAccount.user_id;
			TCHAR szPassword[33];
			TCHAR szPasswordOld[33];
			memset(szPassword, 0, sizeof(szPassword));
			memset(szPasswordOld, 0, sizeof(szPasswordOld));
			m_new_pwd.Trim();
			m_old_pwd.Trim();
			CMD5Encrypt::EncryptData(m_new_pwd,szPassword);
			CMD5Encrypt::EncryptData(m_old_pwd,szPasswordOld);

			lstrcpyn(XGLoginPass.szOldPass,szPasswordOld,sizeof(XGLoginPass.szOldPass));
			lstrcpyn(XGLoginPass.szNewPass,szPassword,sizeof(XGLoginPass.szNewPass));

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_XG_LOGIN_PASS,&XGLoginPass,sizeof(XGLoginPass));
			return;

		}


	}

	if(nSendType == 2)//修改QQ 
	{
		m_bAlterQukuanPass = true;
		if(m_bAlterQukuanPass)
		{
			m_bAlterQukuanPass = false;

			CMD_GP_XG_Qukuanpass XGQukuanpass;
			ZeroMemory(&XGQukuanpass,sizeof(XGQukuanpass));
			XGQukuanpass.dwUserID = theAccount.user_id;
			//变量定义
			TCHAR szPassword[33];
			TCHAR szPasswordOld[33];
			memset(szPassword, 0, sizeof(szPassword));
			memset(szPasswordOld, 0, sizeof(szPasswordOld));
			m_new_pwd_qk.Trim();
			m_old_pwd_qk.Trim();

			CMD5Encrypt::EncryptData(m_new_pwd_qk,szPassword);
			CMD5Encrypt::EncryptData(m_old_pwd_qk,szPasswordOld);

			lstrcpyn(XGQukuanpass.szOldPass,szPasswordOld,sizeof(XGQukuanpass.szOldPass));
			lstrcpyn(XGQukuanpass.szNewPass,szPassword,sizeof(XGQukuanpass.szNewPass));

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_XG_QUKUAN_PASS,&XGQukuanpass,sizeof(XGQukuanpass));

			return;
		}	
	}
	if(nSendType == 3)
	{
		CMD_GP_LockMachine LockMachine;
		ZeroMemory(&LockMachine,sizeof(LockMachine));
		LockMachine.dwUserID = theAccount.user_id;
		LockMachine.cbLock = (theAccount.moormachine == 0)?1:0;
		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_LOCK_MACHINE,&LockMachine,sizeof(LockMachine));

		return;

	}
	if(nSendType == 4)
	{
		CMD_GP_BindPhone BindPhone;
		ZeroMemory(&BindPhone,sizeof(BindPhone));

		BindPhone.nUserID = theAccount.user_id;
		CString strPhone;
		m_editPhone.GetWindowText(strPhone);

		_sntprintf(BindPhone.sPhoneNum,CountArray(BindPhone.sPhoneNum),TEXT("%s"),strPhone);
		BindPhone.cType=1;
		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_BIND_PHONE,&BindPhone,sizeof(BindPhone));
	}

	if(nSendType == 5)
	{
		CMD_GP_SetBindPhoneInfo SetBindPhoneInfo;
		ZeroMemory(&SetBindPhoneInfo,sizeof(SetBindPhoneInfo));

		SetBindPhoneInfo.nUserID = theAccount.user_id;
		BYTE cbCheck = 0;
		cbCheck = m_btnLogonCheck.GetButtonChecked();
		SetBindPhoneInfo.cLoginverify = (cbCheck==1)?1:2;
		cbCheck = m_btnTransferCheck.GetButtonChecked();
		SetBindPhoneInfo.cTranferAccount = (cbCheck==1)?1:2;
		cbCheck = m_btnStockCheck.GetButtonChecked();
		SetBindPhoneInfo.cfenhong = (cbCheck==1)?1:2;
		SetBindPhoneInfo.cType = m_cbOperateType;

		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_BIND_PHONE_INFO,&SetBindPhoneInfo,sizeof(SetBindPhoneInfo));
	}
	//解除绑定手机号
	if(nSendType == 6)
	{
		CMD_GP_UnBindPhone UnBindPhone;
		UnBindPhone.nUserID = theAccount.user_id;
		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_UNBIND_PHONE,&UnBindPhone,sizeof(UnBindPhone));
	}
}

void CZhangHuXgmmDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//选中事件
	m_btnLogonCheck.OnClickControl(point);
	m_btnTransferCheck.OnClickControl(point);
	m_btnStockCheck.OnClickControl(point);

	CDialog::OnLButtonUp(nFlags, point);
}

