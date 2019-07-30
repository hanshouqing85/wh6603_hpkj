#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhangHuShZhTkZhhDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
static const int edit_width = 199;
static const int edit_height = 21;

//��Ϸ�˺�
static const int account_x = 448;
static const int account_y = 78;
static CRect rc_account(account_x, account_y, account_x+edit_width, account_y+edit_height);

//ID
static const int game_id_x = 448;
static const int game_id_y = 117;
static CRect rc_game_id(game_id_x, game_id_y, game_id_x+edit_width, game_id_y+edit_height);

//��������
static const int edit_khyh_x = 445 ;
static const int edit_khyh_y = 155;

//������
static const int edit_khr_x = 448;
static const int edit_khr_y = 195;

//�����˺�
static const int edit_yhzhh_x = 448;
static const int edit_yhzhh_y = 235;

//ȡ������
static const int edit_qkmm_x = 448;
static const int edit_qkmm_y = 275;

//ȷ����ť
static const int btn_ok_x = 446;
static const int btn_ok_y = 314;

IMPLEMENT_DYNAMIC(CZhangHuShZhTkZhhDlg, CDialog)

CZhangHuShZhTkZhhDlg::CZhangHuShZhTkZhhDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZhangHuShZhTkZhhDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_khyh(_T(""))
	, m_khr(_T(""))
	, m_yhzhh(_T(""))
	, m_qkmm(_T(""))
{
	m_bSetQuKuanZhanghao=false;
	m_bQueryYinHang = false;
	m_bQueryMyYinHang=false;
}

CZhangHuShZhTkZhhDlg::~CZhangHuShZhTkZhhDlg()
{
	if (m_bmpBk != NULL)
	{
		SafeDelete(m_bmpBk);
	}
}

void CZhangHuShZhTkZhhDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_KHYH, m_khyh);
	DDX_Text(pDX, IDC_COMBO_KHYH, m_khyh);
	DDX_Text(pDX, IDC_EDIT_KHR, m_khr);
	DDX_Text(pDX, IDC_EDIT_YHZHH, m_yhzhh);
	DDX_Text(pDX, IDC_EDIT_QKMM, m_qkmm);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_COMBO_KHYH, m_cmbKhyh);

	DDX_Control(pDX, IDC_EDIT_KHR, m_editKhr);
	DDX_Control(pDX, IDC_EDIT_YHZHH, m_editYhzhh);
	DDX_Control(pDX, IDC_EDIT_QKMM, m_editQkmm);
}


BEGIN_MESSAGE_MAP(CZhangHuShZhTkZhhDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CZhangHuShZhTkZhhDlg::OnBnClickedBtnOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CZhangHuShZhTkZhhDlg ��Ϣ�������

void CZhangHuShZhTkZhhDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(56,90,154));

	cacheDC.DrawText(theAccount.account, rc_account, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	CString strTmp;
	strTmp.Format(_T("%d"), theAccount.user_id);
	cacheDC.DrawText(strTmp, rc_game_id, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CZhangHuShZhTkZhhDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CZhangHuShZhTkZhhDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CZhangHuShZhTkZhhDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_btnOK.EnableWindow(FALSE);
		SendToServer(2);
		SendToServer(3);
		m_qkmm = _T("");
		m_cmbKhyh.EnableWindow(FALSE);
		SetYHCmbSel(m_khyh);
		UpdateData(FALSE);
	}

}

void CZhangHuShZhTkZhhDlg::OnBnClickedBtnOk()
{
	if(m_dwTickCount == 0)
	{
		m_dwTickCount = GetTickCount();
	}
	else if( GetTickCount()-m_dwTickCount <1000)
	{
		MyMessageBox(L"���������Ƶ�������Ե�Ƭ�̣�");
		return;
	}
	m_dwTickCount = GetTickCount();

	UpdateData();

	//m_khyh.Trim();
	//if (m_khyh.IsEmpty())
	//{
	//	MyMessageBox(_T("�������в���Ϊ��"));
	//	return;
	//}
	//if (m_khyh.GetLength() > 40)
	//{
	//	MyMessageBox(_T("�����������ֹ���"));
	//	return;
	//}
	
	m_khr.Trim();
	if (m_khr.IsEmpty())
	{
		MyMessageBox(_T("�����˲���Ϊ��"));
		return;
	}
	if (m_khr.GetLength() > 40)
	{
		MyMessageBox(_T("���������ֹ���"));
		return;
	}
	
	m_yhzhh.Trim();
	if (m_yhzhh.IsEmpty())
	{
		MyMessageBox(_T("�����˻�����Ϊ��"));
		return;
	}
	if (m_yhzhh.GetLength() > 40)
	{
		MyMessageBox(_T("�����˻�����"));
		return;
	}

	m_qkmm.Trim();
	if (m_qkmm.IsEmpty())
	{
		MyMessageBox(_T("ȡ�����벻��Ϊ��"));
		return;
	}
	if (m_qkmm.GetLength() > PASSWORD_LENGTH - 1)
	{
		MyMessageBox(_T("ȡ���������"));
		return;
	}

	//���ݿ���ʱ����  Alter by a'long
	SendToServer(1);
// 	int res = theDBCmd.SetTiKuanZhangHao(
// 		theAccount.user_id, m_khyh, m_khr, m_yhzhh, m_qkmm);
// 
// 	if (res == 0)
// 	{
// 		MyMessageBox(_T("��������˺ųɹ�"));
// 		EnableCtrls(FALSE);
// 	}
// 	else if (res == 1)
// 	{
// 		MyMessageBox(_T("����������˺�"));
// 	}
// 	else if (res == 2)
// 	{
// 		MyMessageBox(_T("������벻��ȷ"));
// 	}
// 	else if (res == 3)
// 	{
// 		MyMessageBox(_T("����ʧ��"));
// 	}
}

BOOL CZhangHuShZhTkZhhDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(m_bmpBk!=NULL)
		return TRUE;

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\tkzh_bg.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));

	m_font.CreateFont(19, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 

	m_cmbKhyh.InsertString(0, _T("֧����"));
	m_cmbKhyh.InsertString(1, _T("�Ƹ�ͨ"));
	m_cmbKhyh.InsertString(2, _T("�й���������"));
	m_cmbKhyh.InsertString(3, _T("�й���������"));
	m_cmbKhyh.InsertString(4, _T("�й�ũҵ����"));
	m_cmbKhyh.InsertString(5, _T("�й���������"));
	m_cmbKhyh.InsertString(6, _T("�й��������"));
	m_cmbKhyh.InsertString(7, _T("�й�����"));
	m_cmbKhyh.InsertString(8, _T("��������"));
	m_cmbKhyh.InsertString(9, _T("��ͨ����"));
	m_cmbKhyh.InsertString(10, _T("��������"));
	m_cmbKhyh.InsertString(11, _T("��������"));
	m_cmbKhyh.InsertString(12, _T("ƽ������"));
	m_cmbKhyh.InsertString(13, _T("��������"));
	m_cmbKhyh.InsertString(14, _T("�㷢����"));
	m_cmbKhyh.InsertString(15, _T("�������"));
	m_cmbKhyh.InsertString(16, _T("��������"));
	m_cmbKhyh.InsertString(17, _T("ũ�����ú�����"));
	m_cmbKhyh.InsertString(18, _T("������ҵ����"));
	m_cmbKhyh.InsertString(19, _T("�Ϻ��ֶ���չ����"));
	m_cmbKhyh.SetCurSel(0);

	m_cmbKhyh.SetFont(&m_font);
	m_editKhr.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editYhzhh.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editQkmm.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));

	m_dwTickCount = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CZhangHuShZhTkZhhDlg::AdjustCtrls()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_KHYH);
	//if (pWnd != NULL)
	//{
	//	pWnd->SetWindowPos(NULL, edit_khyh_x, edit_khyh_y, edit_width, edit_height, SWP_NOZORDER);
	//}
	if (m_cmbKhyh.GetSafeHwnd() != NULL)
	{
		m_cmbKhyh.SetWindowPos(NULL, edit_khyh_x, edit_khyh_y-3, edit_width+3, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_KHR);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, edit_khr_x, edit_khr_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_YHZHH);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, edit_yhzhh_x, edit_yhzhh_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_QKMM);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, edit_qkmm_x, edit_qkmm_y, edit_width, edit_height, SWP_NOZORDER);
	}

	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}
}

void CZhangHuShZhTkZhhDlg::EnableCtrls(BOOL b)
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_KHYH);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(b);
	}
	pWnd = GetDlgItem(IDC_EDIT_KHR);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(b);
	}
	pWnd = GetDlgItem(IDC_EDIT_YHZHH);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(b);
	}
	pWnd = GetDlgItem(IDC_EDIT_QKMM);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(b);
	}

	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.EnableWindow(b);
	}

	if (m_cmbKhyh.GetSafeHwnd() != NULL)
	{
		m_cmbKhyh.EnableWindow(b);
	}
}

void CZhangHuShZhTkZhhDlg::SetYHCmbSel(const CString& khyh) {

	if (khyh == _T("֧����")) {
		m_cmbKhyh.SetCurSel(0);
	} 
	else if(khyh == _T("�Ƹ�ͨ")) {
		m_cmbKhyh.SetCurSel(1);
	}
	else if(khyh == _T("�й���������")) {
		m_cmbKhyh.SetCurSel(2);
	}
	else if(khyh == _T("�й���������")) {
		m_cmbKhyh.SetCurSel(3);
	}
	else if(khyh == _T("�й�ũҵ����")) {
		m_cmbKhyh.SetCurSel(4);
	}
	else {
		m_cmbKhyh.SetCurSel(0);
	}
}

void CZhangHuShZhTkZhhDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}

void CZhangHuShZhTkZhhDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
}

//��ȡ�¼�
bool CZhangHuShZhTkZhhDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_QUERY_YINHANG_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_QueryYinhang_RET)==0);
				if(wDataSize%sizeof(CMD_GR_QueryYinhang_RET)!=0)
					return false;

				WORD wCount = wDataSize/sizeof(CMD_GR_QueryYinhang_RET);
				

				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_QueryYinhang_RET* pQueryLotResult  =  ((CMD_GR_QueryYinhang_RET*)pData+i);

					int nItem = m_cmbKhyh.GetCount();
					m_cmbKhyh.InsertString(nItem,pQueryLotResult->szYinHangName);
					
				}


				m_cmbKhyh.SetCurSel(0);
				return true;
			}
		case SUB_GR_QUERY_MY_YINHANG_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_QueryMyYinHang_RET));
				if(wDataSize!=sizeof(CMD_GR_QueryMyYinHang_RET))
					return false;
				CMD_GR_QueryMyYinHang_RET* pQueryMyYinHang = (CMD_GR_QueryMyYinHang_RET*)pData;
				
				if(pQueryMyYinHang->lResult == 0)
				{
					BYTE cbLock = pQueryMyYinHang->n_t_lock;
					int nSel = m_cmbKhyh.FindString(0,pQueryMyYinHang->szKaihuYinghang);
					m_cmbKhyh.SetCurSel(nSel);

					//if(cbLock == 1)
					{
						m_btnOK.EnableWindow((cbLock==0)?TRUE:FALSE);
						m_cmbKhyh.EnableWindow((cbLock==0)?TRUE:FALSE);
					}
					CWnd *pWnd = GetDlgItem(IDC_EDIT_KHR);
					if(pWnd != NULL)
					{
						pWnd->EnableWindow(FALSE);
						CString strKaihR,strText;
						strKaihR.Format(L"%s",pQueryMyYinHang->szKaihuRen);

						if(cbLock == 1)
							strText.Format(L"%s*",strKaihR.Left(2));
						else
							strText=strKaihR;
						pWnd->SetWindowText(strText);
					}
					pWnd = GetDlgItem(IDC_EDIT_YHZHH);
					if(pWnd != NULL)
					{
						
						pWnd->EnableWindow((cbLock==0)?TRUE:FALSE);
						CString strKaihR,strText;
						strKaihR.Format(L"%s",pQueryMyYinHang->szYinhangZhanghu);

						if(cbLock == 1)
							strText.Format(L"%s**********%s",strKaihR.Left(4),strKaihR.Right(4));
						else
							strText=strKaihR;
						pWnd->SetWindowText(strText);

					//	pWnd->SetWindowText(pQueryMyYinHang->szYinhangZhanghu);
					}
				}
				else
				{
					m_btnOK.EnableWindow(TRUE);
					m_cmbKhyh.EnableWindow(TRUE);
					EnableCtrls(TRUE);
				}

				return true;
			}
		case SUB_GR_SET_QUKUAN_ZHANGHAO_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_SetQukuanZhanghao_RET));
				if(wDataSize!=sizeof(CMD_GR_SetQukuanZhanghao_RET))
					return false;

				CMD_GR_SetQukuanZhanghao_RET* pCountRet = (CMD_GR_SetQukuanZhanghao_RET*)pData;

				int res = pCountRet->lResult;
				if (res == 0)
				{
					MyMessageBox(_T("��������˺ųɹ�"));
					EnableCtrls(FALSE);
				}
				else if (res == 1)
				{
					MyMessageBox(_T("����������˺�"));
				}
				else if (res == 2)
				{
					MyMessageBox(_T("������벻��ȷ"));
				}
				else if (res == 3)
				{
					MyMessageBox(_T("����ʧ��"));
				}

				return true;
			}

		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

VOID CZhangHuShZhTkZhhDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //��ȡ�û���Ϣ
	{
		m_bSetQuKuanZhanghao = true;
		if(m_bSetQuKuanZhanghao)
		{
			m_bSetQuKuanZhanghao = false;


			CMD_GP_SetQukuanZhanghao SetQukuanZhanghao;
			ZeroMemory(&SetQukuanZhanghao,sizeof(SetQukuanZhanghao));

			SetQukuanZhanghao.dwUserID = theAccount.user_id;
			lstrcpyn(SetQukuanZhanghao.szKaihuYinghang,m_khyh.GetBuffer(),sizeof(SetQukuanZhanghao.szKaihuYinghang));
			lstrcpyn(SetQukuanZhanghao.szKaihuRen,m_khr.GetBuffer(),sizeof(SetQukuanZhanghao.szKaihuRen));
			lstrcpyn(SetQukuanZhanghao.szYinhangZhanghu,m_yhzhh.GetBuffer(),sizeof(SetQukuanZhanghao.szYinhangZhanghu));
			TCHAR szPassword[33];
			memset(szPassword, 0, sizeof(szPassword));
			CMD5Encrypt::EncryptData(m_qkmm,szPassword);

			lstrcpyn(SetQukuanZhanghao.szQukuanPass,szPassword,sizeof(SetQukuanZhanghao.szQukuanPass));
			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_QUKUAN_ZHANGHAO,&SetQukuanZhanghao,sizeof(SetQukuanZhanghao));
			return;

		}


	}
	else if(nSendType == 2)
	{
		m_bQueryYinHang = true;
		if(m_bQueryYinHang)
		{
			m_bQueryYinHang = false;	
			m_cmbKhyh.ResetContent();

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_YINHANG);
			return;

		}

	}
	else if(nSendType == 3)
	{
		m_bQueryMyYinHang = true;
		if(m_bQueryMyYinHang)
		{
			m_bQueryMyYinHang = false;
			CMD_GP_QueryMyYinHang QueryMyYinhang;
			ZeroMemory(&QueryMyYinhang,sizeof(QueryMyYinhang));

			QueryMyYinhang.dwUserID = theAccount.user_id;

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_MY_YINHANG,&QueryMyYinhang,sizeof(QueryMyYinhang));
			return;
		}
	}

}