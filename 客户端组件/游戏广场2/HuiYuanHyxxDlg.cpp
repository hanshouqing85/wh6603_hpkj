#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanHyxxDlg.h"
#include "XgfdDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
#include <map>
#include "YanZhengMa.h"
//////////////////////////////////////////////////////////////////////////////////
map<int ,DWORD> m_UserID;

//////////////////////////////////////////////////////////////////////////////////
//ÆðÊ¼Ê±¼ä
static const int tip_time_start_x = 20;
static const int tip_time_start_y = 49;
static const int tip_time_start_width = 70;
static const int tip_time_start_height = 20;
static CRect rc_time_start(tip_time_start_x, 
						   tip_time_start_y, 
						   tip_time_start_x + tip_time_start_width, 
						   tip_time_start_y + tip_time_start_height);

static const int time_start_x = 90;
static const int time_start_y = 49;
static const int time_start_width = 100;
static const int time_start_height = 20;


//½ØÖ¹Ê±¼ä
static const int tip_time_end_x = 190;
static const int tip_time_end_y = 49;
static const int tip_time_end_width = 20;
static const int tip_time_end_height = 20;
static CRect rc_time_end(tip_time_end_x, 
						 tip_time_end_y, 
						 tip_time_end_x + tip_time_end_width, 
						 tip_time_end_y + tip_time_end_height);

static const int time_end_x = 210;
static const int time_end_y = 49;
static const int time_end_width = 100;
static const int time_end_height = 20;

//账号
static const int tip_edit_act_x = 20;
static const int tip_edit_act_y = 49;
static const int tip_edit_act_width = 40;
static const int tip_edit_act_height = 20;
static CRect rc_edit_act(tip_edit_act_x, 
						 tip_edit_act_y, 
						 tip_edit_act_x+tip_edit_act_width, 
						 tip_edit_act_y+tip_edit_act_height);

static const int edit_act_x = 70;
static const int edit_act_y = 49;
static const int edit_act_width = 95;
static const int edit_act_height = 20;

//ID
static const int tip_edit_id_x = 150;
static const int tip_edit_id_y = 49;
static const int tip_edit_id_width = 40;
static const int tip_edit_id_height = 20;
static CRect rc_edit_id(tip_edit_id_x,
						tip_edit_id_y,
						tip_edit_id_x+tip_edit_id_width,
						tip_edit_id_y+tip_edit_id_height);

static const int edit_id_x = 200;
static const int edit_id_y = 49;
static const int edit_id_width = 95;
static const int edit_id_height = 20;

static const int btn_chkxj_x = 2;
static const int btn_chkxj_y = 2;
static const int btn_chkxj_width = 114;
static const int btn_chkxj_height = 25;

//排序
static const int cmb_sort_x = 320;
static const int cmb_sort_y = 46;
static const int cmb_sort__width = 120;
static const int cmb_sort__height = 12;

//确定按钮
static const int btn_ok_x = 680;
static const int btn_ok_y = 43;

//列表
static const int list_log_x = 10;
static const int list_log_y = 77;
static const int list_log_width = 987;
static const int list_log_height = 347;

//总页数
static const int tip_page_x = 20;
static const int tip_page_y = 430;
static const int tip_page_width = 400;
static const int tip_page_height = 20;
static CRect rc_tip_page(tip_page_x, 
						 tip_page_y, 
						 tip_page_x + tip_page_width, 
						 tip_page_y + tip_page_height);

//上一页
static const int btn_pre_page_x = 793;
static const int btn_pre_page_y = 430;

//下一页
static const int btn_next_page_x = 873;
static const int btn_next_page_y = 430;


static const int page_size = 19;

IMPLEMENT_DYNAMIC(CHuiYuanHyxxDlg, CDialog)

CHuiYuanHyxxDlg::CHuiYuanHyxxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanHyxxDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_tStart(CTime::GetCurrentTime())
	, m_tEnd(CTime::GetCurrentTime())
	, m_page(1)
	, m_itemCount(0)
	, m_maxPage(1)
	, m_byTime(false)
	, m_user_id(0)
	, m_strAct(_T(""))
	, m_strID(_T(""))
{
/*	m_tStart = CTime::GetCurrentTime();
	CString starttime ;

	starttime.Format(L"%d-%02d-%02d",m_tStart.GetYear(),m_tStart.GetMonth(),1);


	COleDateTime tm;
	tm.ParseDateTime(starttime);
	SYSTEMTIME st;
	tm.GetAsSystemTime(st);
	CTime t(st);
	m_tStart = t;
*/
	m_fXGFandian = 0.0f;
	m_bGetHyxxLogCount = false;
	m_bGetHYXXLog = false;
	m_bGetHYXXByIDLog=false;
	m_bGetHYXXByActLog = false;
	m_bXgHyFandian= false;
	m_bSetUserAgent=false;
	m_nIndex = 0;
}

CHuiYuanHyxxDlg::~CHuiYuanHyxxDlg()
{
	if (m_bmpBk != NULL)
	{
		SafeDelete(m_bmpBk);
	}
}
//Êó±êµ¯Æð
void CHuiYuanHyxxDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//Ñ¡ÖÐÊÂ¼þ
	m_CheckOnlineBtn.OnClickControl(point);
	m_CheckUserYueBtn.OnClickControl(point);

	CDialog::OnLButtonUp(nFlags, point);
}

void CHuiYuanHyxxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_START, m_dateStart);
	DDX_Control(pDX, IDC_DATE_END, m_dateEnd);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_PRE_PAGE, m_btnPrePage);
	DDX_Control(pDX, IDC_BTN_NEXT_PAGE, m_btnNextPage);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_tStart);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_tEnd);
	DDX_Control(pDX, IDC_EDIT_ACT, m_editAct);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Text(pDX, IDC_EDIT_ACT, m_strAct);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Control(pDX, IDC_COMBO_SORT, m_cmbSort);

	DDX_Control(pDX, IDC_BTN_CHKXJ_1, m_btnChkxj[0]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_2, m_btnChkxj[1]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_3, m_btnChkxj[2]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_4, m_btnChkxj[3]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_5, m_btnChkxj[4]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_6, m_btnChkxj[5]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_7, m_btnChkxj[6]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_8, m_btnChkxj[7]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_9, m_btnChkxj[8]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_10, m_btnChkxj[9]);
	
}

BEGIN_MESSAGE_MAP(CHuiYuanHyxxDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()

	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanHyxxDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CHuiYuanHyxxDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CHuiYuanHyxxDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_HYXX_CHKXJ, &CHuiYuanHyxxDlg::OnHyxxChkxj)
	ON_COMMAND(ID_HYXX_XGFD, &CHuiYuanHyxxDlg::OnHyxxXgfd)
	ON_COMMAND(ID_HYXX_SHEWEIDAILI, &CHuiYuanHyxxDlg::OnHyxxSwdl)
	ON_COMMAND(ID_HYXX_ZHUANZHANG, &CHuiYuanHyxxDlg::OnHyxxZhuanZ)
	ON_COMMAND(ID_HYXX_FEIPEIPE, &CHuiYuanHyxxDlg::OnHyxxFenPeipe)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LOG, &CHuiYuanHyxxDlg::OnNMRclickListLog)

	ON_BN_CLICKED(IDC_BTN_CHKXJ_1, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj1)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_2, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj2)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_3, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj3)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_4, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj4)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_5, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj5)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_6, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj6)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_7, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj7)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_8, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj8)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_9, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj9)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_10, &CHuiYuanHyxxDlg::OnBnClickedBtnChkxj10)
END_MESSAGE_MAP()


// CHuiYuanHyxxDlg ÏûÏ¢´¦Àí³ÌÐò

void CHuiYuanHyxxDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ÔÚ´Ë´¦Ìí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂë
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
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(255,255,255));

// 	cacheDC.DrawText(_T("ÆðÊ¼Ê±¼ä"), rc_time_start, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
// 	cacheDC.DrawText(_T("ÖÁ"), rc_time_end, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("账户"), rc_edit_act, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("ID"), rc_edit_id, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

	CString strTmp;
// 	if (_ttoi(m_strID) != 0 || (!m_strAct.IsEmpty()))
// 	{
// 		strTmp.Format(_T("¹²²éÕÒµ½%dÌõ¼ÇÂ¼"), m_listLog.GetItemCount());
// 	}
// 	else
	{
		strTmp.Format(_T("第%d页/共%d页，每页%d条记录，共%d条记录"), 
			m_page, m_maxPage, page_size, m_itemCount);
	}
	cacheDC.DrawText(strTmp, rc_tip_page, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	m_CheckOnlineBtn.OnDrawControl(&cacheDC);
	m_CheckUserYueBtn.OnDrawControl(&cacheDC);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);
	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);


	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// ²»Îª»æÍ¼ÏûÏ¢µ÷ÓÃ CDialog::OnPaint()
}

BOOL CHuiYuanHyxxDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanHyxxDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}
void CHuiYuanHyxxDlg::ResetContent()
{
	m_tStart = CTime::GetCurrentTime();
	m_tEnd = CTime::GetCurrentTime();

	for(int i = 1;i < CountArray(m_btnChkxj);i++)
	{
		m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_UserID.clear();
	m_listLog.DeleteAllItems();
	m_page = 1;
	m_editAct.SetWindowText(L"");
	m_editID.SetWindowText(L"");
	m_user_id = 0;
	m_nIndex = 0;
}

void CHuiYuanHyxxDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//m_listLog.DeleteAllItems();
	
	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
//		m_MissionManager.InsertMissionItem(this);

		if(m_UserID.size() <= 0)
		{
			m_tStart = CTime::GetCurrentTime();
			m_tEnd = CTime::GetCurrentTime();

			for(int i = 1;i < CountArray(m_btnChkxj);i++)
			{
				m_btnChkxj[i].ShowWindow(SW_HIDE);
			}

			m_btnChkxj[m_nIndex].ShowWindow(SW_SHOW);
			m_btnChkxj[m_nIndex].SetWindowText(theAccount.account);
			m_user_id = 0;

			m_byType=0;
			m_page = 1;
			m_byTime = false;
			//if(m_user_id != 0)
			{
				//SendToServer(1);  //查询记录数量
				SendToServer(2);	//查询记录
			}
			EnAbleCtrls(true);
		}
	}

}

void CHuiYuanHyxxDlg::OnBnClickedBtnOk()
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

 	m_user_id = theAccount.user_id;
 
 	if (!m_strID.IsEmpty())
 	{
 		int xj_id = _ttoi(m_strID);
 		if (xj_id <= 0) {
 			MyMessageBox(_T("您输入的ID不对"));
 			return;
 		}
		m_user_id = xj_id;
 		m_byType=1;
		m_editAct.SetWindowText(L"");
 	}
 	else if (!m_strAct.IsEmpty())
 	{
 
 		m_byType=2;
 
 	}
 	else 
 	{
 		m_byType=0;
		m_nIndex = 0;
		CString strText;

		for(int i = m_nIndex;i < CountArray(m_btnChkxj);i++)
		{
			if(i<=m_nIndex)
				m_btnChkxj[i].ShowWindow(SW_SHOW);
			else
				m_btnChkxj[i].ShowWindow(SW_HIDE);
		}
		
	}
	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("选择的时间区间不对"));
		return;
	}
// 	else if(m_tStart == m_tEnd)
// 		m_tEnd += CTimeSpan(1L, 0, 0, 0);
	m_byTime = false;
	//m_listLog.DeleteAllItems();
	m_page = 1;
	SendToServer(10);

	//SendToServer(1);
	SendToServer(2);


	InvalidateRect(rc_tip_page);
}

void CHuiYuanHyxxDlg::OnBnClickedBtnPrePage()
{
	if(m_page == 1)
	{
		MyMessageBox(_T("已经是第一页了"));
		return;
	}

	m_page--;
	m_listLog.DeleteAllItems();

	SendToServer(2);
	InvalidateRect(rc_tip_page);
}

//·­Ò³
void CHuiYuanHyxxDlg::OnBnClickedBtnNextPage()
{
	if(m_page == m_maxPage)
	{
		MyMessageBox(_T("已经是最后一页了"));
		return;
	}

	m_page++;
	m_listLog.DeleteAllItems();
	SendToServer(2);
	InvalidateRect(rc_tip_page);
}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj1()
{
	m_nIndex = 0;
	CString strText;

	for(int i = m_nIndex;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_page = 1;
	//m_btnChkxj[m_nIndex].ShowWindow(SW_HIDE);
// 	m_btnChkxj[m_nIndex].GetWindowText(strText);
// 
// 	if(strText.IsEmpty())
// 		return;
	m_editAct.SetWindowText(L"");
	m_editID.SetWindowText(L"");
	OnBnClickedBtnOk();
}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj2()
{
	m_nIndex = 1;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	//m_btnChkxj[m_nIndex].ShowWindow(SW_HIDE);
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID[m_nIndex]);
	m_user_id = m_UserID[m_nIndex];
	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;

	m_byType=0;
	//OnBnClickedBtnOk();
	//SendToServer(1);
	SendToServer(2);
}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj3()
{
	m_nIndex = 2;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID[m_nIndex]);
	m_user_id = m_UserID[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

//	OnBnClickedBtnOk();
	//SendToServer(1);
	SendToServer(2);

}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj4()
{
	m_nIndex = 3;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID[m_nIndex]);
	m_user_id = m_UserID[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	//SendToServer(1);
	SendToServer(2);

}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj5()
{
	m_nIndex = 4;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID[m_nIndex]);
	m_user_id = m_UserID[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	//SendToServer(1);
	SendToServer(2);

}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj6()
{
	m_nIndex = 5;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	//m_btnChkxj[m_nIndex].ShowWindow(SW_HIDE);
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID[m_nIndex]);
	m_user_id = m_UserID[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	//SendToServer(1);
	SendToServer(2);
}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj7()
{
	m_nIndex = 6;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID[m_nIndex]);
	m_user_id = m_UserID[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	//SendToServer(1);
	SendToServer(2);
}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj8()
{
	m_nIndex = 7;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID[m_nIndex]);
	m_user_id = m_UserID[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	//SendToServer(1);
	SendToServer(2);
}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj9()
{
	m_nIndex = 8;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID[m_nIndex]);
	m_user_id = m_UserID[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	//SendToServer(1);
	SendToServer(2);
}
void CHuiYuanHyxxDlg::OnBnClickedBtnChkxj10()
{
	m_nIndex = 9;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	m_editID.SetWindowText(L"");
	OnBnClickedBtnOk();
}
//²éÑ¯ÏÂ¼¶
void CHuiYuanHyxxDlg::OnHyxxChkxj()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		CString strType = m_listLog.GetItemText(nItem, 3);
		if (strType == _T("会员"))
		{
			MyMessageBox(_T("此人不是代理，无下级"));
		}
		else
		{
			m_byType=0;
			m_user_id = _ttoi(m_listLog.GetItemText(nItem,0));
			CString  strAccount = m_listLog.GetItemText(nItem,1);
			if(m_nIndex<7&&m_user_id!=m_UserID[m_nIndex])
			{
				m_nIndex++;
				m_btnChkxj[m_nIndex].ShowWindow(SW_SHOW);
				m_btnChkxj[m_nIndex].SetWindowText(strAccount);
				m_UserID[m_nIndex] = m_user_id;

			}
			m_editAct.SetWindowText(L"");
			m_strID.Format(L"%d",m_user_id);
			m_editID.SetWindowText(m_strID);
			//SendToServer(1);
			SendToServer(2);
		}
	}
}

///ÐÞ¸ÄÏÂ¼¶·µµã
void CHuiYuanHyxxDlg::OnHyxxXgfd()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}

	int nItem = m_listLog.GetNextSelectedItem(pos);
	CXgfdDlg dlg;
	dlg.m_user_id = _ttoi(m_listLog.GetItemText(nItem,0));
	dlg.m_account = m_listLog.GetItemText(nItem, 1);
	CString sFandian = m_listLog.GetItemText(nItem, 4);
	sFandian = sFandian.Left(sFandian.GetLength() - 1);
	dlg.m_old_fandian = _tstof(sFandian);
	dlg.m_fandian = dlg.m_old_fandian;


	if(dlg.DoModal() == IDOK)
	{
		m_fXGFandian = dlg.m_fandian;
		SendToServer(5);
	}
}

//ÉèÎª´úÀí
void CHuiYuanHyxxDlg::OnHyxxSwdl()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}
	int nItem = m_listLog.GetNextSelectedItem(pos);

	m_user_id = _ttoi(m_listLog.GetItemText(nItem,0));
	SendToServer(6);
}
#include "FenPeiPeie.h"
void CHuiYuanHyxxDlg::OnHyxxFenPeipe()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}
	int nItem = m_listLog.GetNextSelectedItem(pos);

	SendToServer(8);
// 	CFenPeiPeie FenPeiPeie;
// 	FenPeiPeie.m_nUserID = m_listLog.GetItemData(nItem);
// 	FenPeiPeie.m_fFandian = _tstof(sFandian);
// 	if (FenPeiPeie.DoModal() == IDOK)
// 	{
// 
// 	}
	return;
}
#include "ZhuanZhangDlg.h"
//×ªÕË
void CHuiYuanHyxxDlg::OnHyxxZhuanZ()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	int nItem = m_listLog.GetNextSelectedItem(pos);


	int n_target_id = _ttoi(m_listLog.GetItemText(nItem,0));
	CString strName= m_listLog .GetItemText(nItem,1);
	CZhuanZhangDlg ZhuanZhangDlg;

	ZhuanZhangDlg.SetZhuanZhangXinxi(theAccount.user_id,strName,n_target_id,theAccount.yue);
	if (ZhuanZhangDlg.DoModal() == IDOK)
	{
		m_nTargetUserID = n_target_id;
		m_fZhuanZhangJine = ZhuanZhangDlg.m_fJiaoYiJine;
		m_nZhuanZType = ZhuanZhangDlg.m_nType;
		CWHEncrypt::MD5Encrypt(ZhuanZhangDlg.m_szPassWord,m_szPassWord);

		//CopyMemory(m_szPassWord,ZhuanZhangDlg.m_szPassWord,CountArray(m_szPassWord));
		m_strName = strName;
		SendToServer(11);
	}
}

void CHuiYuanHyxxDlg::OnNMRclickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}

	POINT pt = {0, 0};
	GetCursorPos(&pt);
	m_byType=0;
	CMenu menu;
	menu.LoadMenu(IDR_MENU_HYXX);
	int nItem = m_listLog.GetNextSelectedItem(pos);

	int nUserID = _ttoi(m_listLog.GetItemText(nItem,0));
	BYTE cbIfxj = m_listLog.GetItemData(nItem);
	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		if(nUserID==theAccount.user_id)
		{
			pMenu->EnableMenuItem(ID_HYXX_CHKXJ, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			pMenu->EnableMenuItem(ID_HYXX_XGFD, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			pMenu->EnableMenuItem(ID_HYXX_SHEWEIDAILI, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);


		}
		else
		{
			CString sFandian = m_listLog.GetItemText(nItem, 4);
			sFandian = sFandian.Left(sFandian.GetLength() - 1);
			DOUBLE fFandian =  _tstof(sFandian);

			if(fFandian<2.8)
			{
				pMenu->EnableMenuItem(ID_HYXX_FEIPEIPE,MF_BYCOMMAND| MF_DISABLED | MF_GRAYED);
			}

			if(theAccount.fandian < 0.027)
			{
				pMenu->EnableMenuItem(ID_HYXX_ZHUANZHANG,MF_BYCOMMAND| MF_DISABLED | MF_GRAYED);

			}
// 			if(m_nIndex != 0)
// 			{
// 				pMenu->EnableMenuItem(ID_HYXX_SHEWEIDAILI,MF_BYCOMMAND| MF_DISABLED | MF_GRAYED);
// 				pMenu->EnableMenuItem(ID_HYXX_ZHUANZHANG,MF_BYCOMMAND| MF_DISABLED | MF_GRAYED);
// 
// 				pMenu->EnableMenuItem(ID_HYXX_XGFD, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
// 				pMenu->EnableMenuItem(ID_HYXX_FEIPEIPE,MF_BYCOMMAND| MF_DISABLED | MF_GRAYED);
// 			}

			if(cbIfxj==0)
			{
 				pMenu->EnableMenuItem(ID_HYXX_SHEWEIDAILI,MF_BYCOMMAND| MF_DISABLED | MF_GRAYED);
 			//	pMenu->EnableMenuItem(ID_HYXX_ZHUANZHANG,MF_BYCOMMAND| MF_DISABLED | MF_GRAYED);
 
 				pMenu->EnableMenuItem(ID_HYXX_XGFD, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
 				pMenu->EnableMenuItem(ID_HYXX_FEIPEIPE,MF_BYCOMMAND| MF_DISABLED | MF_GRAYED);
				
			}
		}
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
	*pResult = 0;
}

BOOL CHuiYuanHyxxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk == NULL)
	{
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg3.png"));

		m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt.png"));
		m_btnPrePage.SetImage(CBmpUtil::GetExePath() + _T("skin\\syy_bt.png"));
		m_btnNextPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\xyy_bt.png"));

		for(int i = 0;i < CountArray(m_btnChkxj);i++)
		{
			m_btnChkxj[i].SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_xj.png"));
		}

		m_font.CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
		m_cmbfont.CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
		m_cmbSort.SetButtonImage(AfxGetInstanceHandle(),IDB_BT_COMBOBOX_ACT,CSize(16,18));
		m_CheckOnlineBtn.SetParentWndSink(this);
		m_CheckOnlineBtn.LoadCheckImage(_T("BT_ONLINE"));
		m_CheckUserYueBtn.SetParentWndSink(this);
		m_CheckUserYueBtn.LoadCheckImage(_T("BT_USER_YUE"));

	}
	InitListCtrl();
	m_byType=0;
	m_dwTickCount=0;

	m_editAct.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editID.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}

void CHuiYuanHyxxDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanHyxxDlg::OnOK()
{
	//CDialog::OnOK();
}


void CHuiYuanHyxxDlg::InitListCtrl()
{

	m_listLog.ModifyStyle(NULL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(56,90,154));

	m_listLog.InsertColumn(0, _T("用户ID"), LVCFMT_CENTER, 73);
	m_listLog.InsertColumn(1, _T("用户名"), LVCFMT_CENTER, 140);
	m_listLog.InsertColumn(2, _T("注册时间"), LVCFMT_CENTER, 150);
	m_listLog.InsertColumn(3, _T("类型"), LVCFMT_CENTER, 60);
	m_listLog.InsertColumn(4, _T("返点%"), LVCFMT_CENTER,60);
	m_listLog.InsertColumn(5, _T("余额"), LVCFMT_CENTER, 140);
	m_listLog.InsertColumn(6, _T("最后登录时间"), LVCFMT_CENTER, 150);
	//m_listLog.InsertColumn(7, _T("状态"), LVCFMT_LEFT, 40);
	m_listLog.InsertColumn(7, _T("下级在线/人数"), LVCFMT_CENTER, 100);
	m_listLog.InsertColumn(8, _T("联系QQ"), LVCFMT_CENTER, 110);

	for (int i = 0 ;i < 19;i++)
	{
		
		int nColumnCount = 8;
		for(int j = 0;j < nColumnCount;j++)
		{
			COLORREF color;

			//color = GetBkColor();
			if(i%2)
			{
				color = RGB(201,198,197);
				m_listLog.SetItemColor(j,i,color);
			}
		}
	}

	m_cmbSort.SetFont(&m_cmbfont);
	m_cmbSort.ResetContent();

	m_cmbSort.AddString(TEXT("注册时间 ↓"));
	m_cmbSort.AddString(TEXT("注册时间 ↑"));
	m_cmbSort.AddString(TEXT("登录时间 ↓"));
	m_cmbSort.AddString(TEXT("登录时间 ↑"));
	m_cmbSort.SetCurSel(0);
}

void CHuiYuanHyxxDlg::AdjustCtrls()
{
	m_CheckOnlineBtn.SetControlBenchmark(cmb_sort_x+130,cmb_sort_y);
	m_CheckUserYueBtn.SetControlBenchmark(cmb_sort_x+232,cmb_sort_y);

	if (m_cmbSort.GetSafeHwnd() != NULL)
	{
		m_cmbSort.SetWindowPos(NULL, cmb_sort_x, cmb_sort_y, cmb_sort__width,cmb_sort__height, SWP_NOZORDER);
	}

	if (m_dateStart.GetSafeHwnd() != NULL)
	{
		m_dateStart.SetWindowPos(NULL, time_start_x, time_start_y, time_start_width, time_start_height, SWP_NOZORDER);
		m_dateStart.ShowWindow(SW_HIDE);
	}
	if (m_dateEnd.GetSafeHwnd() != NULL)
	{
		m_dateEnd.SetWindowPos(NULL, time_end_x, time_end_y, time_end_width, time_end_height, SWP_NOZORDER);
		m_dateEnd.ShowWindow(SW_HIDE);
	}
	if (m_listLog.GetSafeHwnd() != NULL)
	{
		m_listLog.SetWindowPos(NULL, list_log_x, list_log_y, list_log_width, list_log_height, SWP_NOZORDER);
	}
	if (m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}
	if (m_btnPrePage.GetSafeHwnd() != NULL)
	{
		m_btnPrePage.SetWindowPos(NULL, btn_pre_page_x, btn_pre_page_y, m_btnPrePage.Width(), m_btnPrePage.Height(), SWP_NOZORDER);
	}
	if (m_btnNextPage.GetSafeHwnd() != NULL)
	{
		m_btnNextPage.SetWindowPos(NULL, btn_next_page_x, btn_next_page_y, m_btnNextPage.Width(), m_btnNextPage.Height(), SWP_NOZORDER);
	}
	if (m_editAct.GetSafeHwnd() != NULL)
	{
		m_editAct.SetWindowPos(NULL, edit_act_x, edit_act_y, edit_act_width, edit_act_height, SWP_NOZORDER);
	}
	if (m_editID.GetSafeHwnd() != NULL)
	{
		m_editID.SetWindowPos(NULL, edit_id_x, edit_id_y, edit_id_width, edit_id_height, SWP_NOZORDER);
	}

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(m_btnChkxj[i].GetSafeHwnd() == NULL)
			 continue;
		m_btnChkxj[i].SetWindowPos(NULL,btn_chkxj_x+i*(btn_chkxj_width),btn_chkxj_y,btn_chkxj_width,btn_chkxj_height,SWP_NOZORDER);
		if(i>7)
		{
			m_btnChkxj[i].ShowWindow(SW_HIDE);
		}
	}
}

void CHuiYuanHyxxDlg::EnAbleCtrls(bool b)
{
	m_btnPrePage.EnableWindow(b);
	m_btnNextPage.EnableWindow(b);
}


bool CHuiYuanHyxxDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_PARENT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GetParentRet));
				if(wDataSize!=sizeof(CMD_GetParentRet)) return false;

				CMD_GetParentRet* pGetParentRet = (CMD_GetParentRet*)pData;

				m_UserID.clear();
				for(int i = 1;i < CountArray(m_btnChkxj);i++)
				{
						m_btnChkxj[i].ShowWindow(SW_HIDE);
				}

				int nCount = pGetParentRet->n_t_result;

				CString strLog;
				for(int i = 0;i < nCount;i++)
				{
					m_btnChkxj[i].ShowWindow(SW_SHOW);
					strLog.Format(L"%s",pGetParentRet->s_t_desc[nCount-i-1]);
					m_btnChkxj[i].SetWindowText(strLog);
					m_UserID[i] = pGetParentRet->n_t_userid[nCount-i-1];
				}
				break;
			}
		case SUB_GR_GET_HYXX_LOG_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetHYXXLogCountRet));
				if(wDataSize!=sizeof(CMD_GR_GetHYXXLogCountRet)) return false;

				CMD_GR_GetHYXXLogCountRet* pCountRet = (CMD_GR_GetHYXXLogCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
				m_maxPage = max(1,m_maxPage);

				InvalidateRect(&rc_tip_page);
				return true;
			}
		case SUB_GP_GET_HYXX_LOG_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_GetHYXXLogRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetHYXXLogRet)!=0) return false;
				m_listLog.DeleteAllItems();
				SendToServer(1);
				WORD wCount = wDataSize/sizeof(CMD_GR_GetHYXXLogRet);
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetHYXXLogRet *pHYXXLogRet = ((CMD_GR_GetHYXXLogRet*)pData+i);

					CString strTmp;
					strTmp.Format(_T("%d"), pHYXXLogRet->n_t_id);
					bool bThisID = false;
					CString strAccount;
					strAccount.Format(L"%s",pHYXXLogRet->s_t_account);

					int item = m_listLog.InsertItem(m_listLog.GetItemCount(), strTmp);
					m_listLog.SetItemData(item, pHYXXLogRet->cb_ifxj);
					m_cbIfXj = pHYXXLogRet->cb_ifxj;
					m_listLog.SetItemText(item, 1, pHYXXLogRet->s_t_account);

					CTime ct(pHYXXLogRet->n_t_time);
					m_listLog.SetItemText(item, 2, ct.Format(_T("%Y%m%d %H:%M:%S")));


					int t_type = pHYXXLogRet->n_t_type;
					switch (t_type)
					{
					case 0: strTmp = _T("会员"); break;
					case 1: strTmp = _T("代理"); break;
					default: strTmp = _T(""); break;
					}
					m_listLog.SetItemText(item, 3, strTmp);

					double fandian = (double)pHYXXLogRet->f_t_fandian;
					strTmp.Format(_T("%.2lf"), fandian * 100);

					m_listLog.SetItemText(item, 4, strTmp);

					strTmp.Format(_T("%.3lf"), pHYXXLogRet->f_t_yue);
					m_listLog.SetItemText(item, 5, strTmp);

					CTime ct1(pHYXXLogRet->n_t_lasttime);
					m_listLog.SetItemText(item, 6, ct1.Format(_T("%Y%m%d %H:%M:%S")));

					//int t_online = pHYXXLogRet->n_t_online;
					//strTmp = t_online == 0? _T("否"): _T("是");
					m_listLog.SetItemText(item, 7, pHYXXLogRet->s_t_online);
					if(m_user_id == theAccount.user_id)
						m_listLog.SetItemText(item, 8, pHYXXLogRet->s_t_qq);
				}
				return 1;
			}
		case SUB_GR_XG_HY_FANDIAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_XGHYFandian_RET));
				if(wDataSize!=sizeof(CMD_GP_XGHYFandian_RET)) return false;

				CMD_GP_XGHYFandian_RET* pXGHYFandian = (CMD_GP_XGHYFandian_RET*)pData;

				LONG lResult  = pXGHYFandian->lResult;
				if(lResult == DB_SUCCESS)
				{
					CString str;
					str.Format(_T("已成功修改 %s 的返点为 %.2lf"), pXGHYFandian->s_Account, pXGHYFandian->f_Fandian*100);
					MyMessageBox(str + _T("%"));

					str.Format(_T("%.2lf"), pXGHYFandian->f_Fandian*100);
					m_listLog.SetItemText(m_nItem, 4, str );

				}
				else if(lResult == 1)
				{
					MyMessageBox(L"返点修改失败！");
				}	
				else
				{
					CString str;
					str.Format(_T("您不是 %s 的直属代理，或者超过限额，不能修改TA的返点！"), pXGHYFandian->s_Account);
					MyMessageBox(str);
				}

				return true;
			}
		case SUB_GR_SW_DAILI_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_SWDaili_RET));
				if(wDataSize!=sizeof(CMD_GP_SWDaili_RET)) return false;

				CMD_GP_SWDaili_RET* pXGHYFandian = (CMD_GP_SWDaili_RET*)pData;

				LONG lResult  = pXGHYFandian->lResult;
				if(lResult == DB_SUCCESS)
				{
					CString strLog;
					strLog.Format(L"已经成功修改%d为代理", m_user_id);
					MyMessageBox(strLog);

					m_listLog.SetItemText(m_nItem, 3, L"代理" );


				}
				else
				{
					MyMessageBox(_T("修改失败"));

				}
				return true;
			}
		case SUB_GR_HYXX_ZHUANZHANG_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_HYXX_ZhuanZhang_RET));
				if(wDataSize!=sizeof(CMD_GP_HYXX_ZhuanZhang_RET)) return false;

				CMD_GP_HYXX_ZhuanZhang_RET* pZhuanzhang = (CMD_GP_HYXX_ZhuanZhang_RET*)pData;
				OnBnClickedBtnOk();
				MyMessageBox(pZhuanzhang->s_t_desc);

				return true;
			}
		case SUB_GR_GET_XJ_Peie_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GetXjPeieRet));
				if(wDataSize!=sizeof(CMD_GetXjPeieRet)) return false;

				CMD_GetXjPeieRet* pGetXjPeieRet = (CMD_GetXjPeieRet*)pData;
				int nPeie1Sy = pGetXjPeieRet->n_t_peie_1-pGetXjPeieRet->n_t_peie_s_1;
				int nPeie2Sy = pGetXjPeieRet->n_t_peie_2-pGetXjPeieRet->n_t_peie_s_2;
				int nPeie3Sy = pGetXjPeieRet->n_t_peie_3 - pGetXjPeieRet->n_t_peie_s_3;
				int nPeie4Sy = pGetXjPeieRet->n_t_peie_4-pGetXjPeieRet->n_t_peie_s_4;
				int nPeie5Sy = pGetXjPeieRet->n_t_peie_5 - pGetXjPeieRet->n_t_peie_s_5;

				CFenPeiPeie FenPeiPeie;
				FenPeiPeie.m_nPeieSy1 = nPeie1Sy;
				FenPeiPeie.m_nPeieSy2 = nPeie2Sy;
				FenPeiPeie.m_nPeieSy3 = nPeie3Sy;
				FenPeiPeie.m_nPeieSy4 = nPeie4Sy;
				FenPeiPeie.m_nPeieSy5 = nPeie5Sy;

				FenPeiPeie.m_fFandian = theAccount.fandian;
				if(FenPeiPeie.DoModal() == IDOK)
				{
					m_nSetPeie4 = FenPeiPeie.m_nSetPeie4;
					m_nSetPeie5 = FenPeiPeie.m_nSetPeie5;

					if(m_nSetPeie1 > nPeie1Sy ||m_nSetPeie2 > nPeie2Sy ||m_nSetPeie3 > nPeie3Sy ||m_nSetPeie4 > nPeie4Sy ||m_nSetPeie5 > nPeie5Sy||m_nSetPeie1 < 0 ||m_nSetPeie2 < 0||m_nSetPeie3 < 0||m_nSetPeie4 < 0||m_nSetPeie5 < 0)
					{
						MyMessageBox(L"分配配额不合法，请重新设置！");
						return true;
					}
					if(m_nSetPeie1 <= 0 && m_nSetPeie2 <= 0&&m_nSetPeie3 <= 0&& m_nSetPeie4 <= 0&&m_nSetPeie5 <= 0)
					{

						return true;

					}

					SendToServer(9);
				}
				return true;
			}
		case SUB_GR_SET_XJ_Peie_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_SetXjPeieRet));
				if(wDataSize!=sizeof(CMD_SetXjPeieRet)) return false;

				CMD_SetXjPeieRet* SetXjPeieRet = (CMD_SetXjPeieRet*)pData;


				MyMessageBox(SetXjPeieRet->s_t_desc);
				return true;
			}
		case SUB_GP_SEND_CHECKYANZHENGMA_TRANS_RET:	//验证码
			{
				//效验数据
				ASSERT(wDataSize>=sizeof(CMD_GR_SendYanZhengmaRet));
				if (wDataSize<sizeof(CMD_GR_SendYanZhengmaRet)) return false;

				CMD_GR_SendYanZhengmaRet* pSendYanZheng = (CMD_GR_SendYanZhengmaRet*)pData;
				CString strDesc;
				strDesc.Format(L"%s",pSendYanZheng->sDesc);
				if(pSendYanZheng->nResult == 0)
				{
					SendToServer(7);
				}
				else
				{
					MyMessageBox(pSendYanZheng->sDesc);

					CYanZhengMaDlg YanZhengMaDlg;
					YanZhengMaDlg.SetYanZhengXinxi(m_strPhoneNum,2,3);

					if(YanZhengMaDlg.DoModal()==IDOK)
					{
						CMD_SendYanZhengma SendYanZhengma;
						ZeroMemory(&SendYanZhengma,sizeof(SendYanZhengma));
						SendYanZhengma.n_t_userid = theAccount.user_id;
						CString strTemp;
						strTemp = YanZhengMaDlg.GetYanZhengma();
						_sntprintf(SendYanZhengma.s_t_code,CountArray(SendYanZhengma.s_t_code),TEXT("%s"),strTemp);
						SendYanZhengma.c_t_type = 3;
						SendYanZhengma.c_t_stype = 1;
						CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
						if(pPlatformFrame!=NULL)
							pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SEND_CHECKYANZHENGMA_TRANS,&SendYanZhengma,sizeof(SendYanZhengma));
					}	

				}
				return true;
			}
 		case SUB_GP_GET_PHONE_TRASFER_FENHONG_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetPhoneTransferFenhongRet));
				if(wDataSize!=sizeof(CMD_GR_GetPhoneTransferFenhongRet)) return false;
				
				CMD_GR_GetPhoneTransferFenhongRet* pGetPhoneInfo = (CMD_GR_GetPhoneTransferFenhongRet*)pData;
				if(pGetPhoneInfo->n_t_state == 1)
				{
					m_strPhoneNum.Format(L"%s",pGetPhoneInfo->sPhoneNum);
					CYanZhengMaDlg YanZhengMaDlg;
					YanZhengMaDlg.SetYanZhengXinxi(m_strPhoneNum,2,3);

					if(YanZhengMaDlg.DoModal()==IDOK)
					{
						CMD_SendYanZhengma SendYanZhengma;
						ZeroMemory(&SendYanZhengma,sizeof(SendYanZhengma));
						SendYanZhengma.n_t_userid = theAccount.user_id;
						CString strTemp;
						strTemp = YanZhengMaDlg.GetYanZhengma();
						_sntprintf(SendYanZhengma.s_t_code,CountArray(SendYanZhengma.s_t_code),TEXT("%s"),strTemp);
						SendYanZhengma.c_t_type = 3;
						SendYanZhengma.c_t_stype = 1;
						CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
						if(pPlatformFrame!=NULL)
							pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SEND_CHECKYANZHENGMA_TRANS,&SendYanZhengma,sizeof(SendYanZhengma));
					}	
				}
				else
				{
					SendToServer(7);
				}

				return true;
			}

		}
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
		return true;
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CHuiYuanHyxxDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //会员记录数量
	{
		m_bGetHyxxLogCount = true;
		if(m_bGetHyxxLogCount)
		{
			m_bGetHyxxLogCount = false;
			CMD_GP_GetHYXXLogCount GetHYXXLogCount;
			ZeroMemory(&GetHYXXLogCount,sizeof(GetHYXXLogCount));

			GetHYXXLogCount.dwUserID = theAccount.user_id;
			GetHYXXLogCount.n_t_type = m_byType;

			int xj_id = _ttoi(m_strID);
			GetHYXXLogCount.n_t_user_id = m_user_id;
			lstrcpyn(GetHYXXLogCount.s_t_account,m_strAct.GetBuffer(),sizeof(GetHYXXLogCount.s_t_account));
			GetHYXXLogCount.bTime = m_byTime;
			GetHYXXLogCount.nOnline = m_CheckOnlineBtn.GetButtonChecked();
			GetHYXXLogCount.nYue = m_CheckUserYueBtn.GetButtonChecked();
			CopyMemory(GetHYXXLogCount.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetHYXXLogCount.szTimeStart));
			CopyMemory(GetHYXXLogCount.szTimeEnd , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetHYXXLogCount.szTimeEnd));

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_HYXX_LOG_COUNT,&GetHYXXLogCount,sizeof(GetHYXXLogCount));

			return ;
		}


	}

	if(nSendType == 2)//会员记录 
	{
		m_bGetHYXXLog = true;
		if(m_bGetHYXXLog)
		{
			m_bGetHYXXLog = false;

			m_listLog.DeleteAllItems();
			CMD_GP_GetHYXXLog HYXXLog;
			ZeroMemory(&HYXXLog,sizeof(HYXXLog));

			HYXXLog.dwUserID = theAccount.user_id;
			HYXXLog.n_t_type = m_byType;

			int xj_id = _ttoi(m_strID);
			HYXXLog.n_t_user_id = m_user_id;
			lstrcpyn(HYXXLog.s_t_account,m_strAct.GetBuffer(),CountArray(HYXXLog.s_t_account));
			HYXXLog.n_t_sorttype = m_cmbSort.GetCurSel();
			HYXXLog.nPage = m_page;
			HYXXLog.nSize = page_size;
			HYXXLog.bByTime = m_byTime;
			HYXXLog.nOnline = m_CheckOnlineBtn.GetButtonChecked();
			HYXXLog.nYue = m_CheckUserYueBtn.GetButtonChecked();
			CopyMemory(HYXXLog.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(HYXXLog.szTimeStart));
			CopyMemory(HYXXLog.szTimeEnd , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(HYXXLog.szTimeEnd));


			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_HYXX_LOG,&HYXXLog,sizeof(HYXXLog));
			return ;
		}

	}


	if(nSendType == 5)
	{
		m_bXgHyFandian = true;
 		if(m_bXgHyFandian)
 		{
 			m_bXgHyFandian=false;
 			POSITION pos = m_listLog.GetFirstSelectedItemPosition();
 			if (pos == NULL) 
 			{
 				return ;
 			}
 			m_nItem = m_listLog.GetNextSelectedItem(pos);
 			CXgfdDlg dlg;
 			dlg.m_user_id = _ttoi(m_listLog.GetItemText(m_nItem,0));
 			dlg.m_account = m_listLog.GetItemText(m_nItem, 1);
 			CString sFandian = m_listLog.GetItemText(m_nItem, 4);
 			sFandian = sFandian.Left(sFandian.GetLength() - 1);
 
 			CMD_GP_XGHYFandian XGHYFandian;
 			ZeroMemory(&XGHYFandian,sizeof(XGHYFandian));
 
 			XGHYFandian.dwUserID = theAccount.user_id;
 			XGHYFandian.n_xiaji_id = _ttoi(m_listLog.GetItemText(m_nItem,0));
 			XGHYFandian.f_Fandian = m_fXGFandian/100;
 
			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_XG_HY_FANDIAN,&XGHYFandian,sizeof(XGHYFandian));
			return;
 		}

	}

	if(nSendType == 6)
	{
		m_bSetUserAgent = true;
 		if(m_bSetUserAgent)
 		{
 			m_bSetUserAgent=false;
 			POSITION pos = m_listLog.GetFirstSelectedItemPosition();
 			if (pos == NULL) 
 			{
 				return ;
 			}
 			m_nItem = m_listLog.GetNextSelectedItem(pos);
 
 			CMD_GP_SWDaili SWDaili;
 			SWDaili.dwUserID = _ttoi(m_listLog.GetItemText(m_nItem,0));
 			SWDaili.nAgentID = theAccount.user_id;
 
 			SWDaili.nDaili = 1;
			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_SW_DAILI,&SWDaili,sizeof(SWDaili));
 
			return;
 		}

	}
	//转账
	if(nSendType == 7)
	{
		POSITION pos = m_listLog.GetFirstSelectedItemPosition();
		if (pos == NULL) 
		{
			return ;
		}
		m_nItem = m_listLog.GetNextSelectedItem(pos);

		CMD_GP_HYXX_ZhuanZhang ZhuanZhang;
		ZhuanZhang.n_t_user_id = theAccount.user_id;
		ZhuanZhang.n_t_target_id = m_nTargetUserID;
		ZhuanZhang.n_t_type = m_nZhuanZType;
		ZhuanZhang.f_t_jine = m_fZhuanZhangJine;
		CopyMemory(ZhuanZhang.s_t_password,m_szPassWord,CountArray(ZhuanZhang.s_t_password));

		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_HYXX_ZHUANZHANG,&ZhuanZhang,sizeof(ZhuanZhang));

		return;

	}

	//配额
	if(nSendType == 8)
	{
		POSITION pos = m_listLog.GetFirstSelectedItemPosition();
		if (pos == NULL) 
		{
			return ;
		}
		m_nItem = m_listLog.GetNextSelectedItem(pos);

		CMD_GetXjPeie GetXjPeie;
		GetXjPeie.n_t_userid = theAccount.user_id;

		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJ_Peie,&GetXjPeie,sizeof(GetXjPeie));

		return;

	}
	if(nSendType == 9)
	{
		POSITION pos = m_listLog.GetFirstSelectedItemPosition();
		if (pos == NULL) 
		{
			return ;
		}
		m_nItem = m_listLog.GetNextSelectedItem(pos);

		CMD_SetXjPeie SetXjPeie;
		SetXjPeie.n_t_userid = theAccount.user_id;
		SetXjPeie.n_t_xj_id = _ttoi(m_listLog.GetItemText(m_nItem,0));
		SetXjPeie.n_t_peie1 = m_nSetPeie1;
		SetXjPeie.n_t_peie2 = m_nSetPeie2;
		SetXjPeie.n_t_peie3 = m_nSetPeie3;
		SetXjPeie.n_t_peie4 = m_nSetPeie4;
		SetXjPeie.n_t_peie5 = m_nSetPeie5;

		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_XJ_Peie,&SetXjPeie,sizeof(SetXjPeie));

		return;

	}
	if(nSendType == 10)
	{
		if(m_user_id == theAccount.user_id)
			return;
		CMD_GetParent SetXjPeie;
		SetXjPeie.n_t_userid = theAccount.user_id;
		SetXjPeie.n_t_xj_id = m_user_id;

		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_PARENT,&SetXjPeie,sizeof(SetXjPeie));

		return;

	}
	if(nSendType == 11)
	{
		CMD_GP_GetPhoneTransferFenhong GetPhoneTransferFenhong;
		ZeroMemory(&GetPhoneTransferFenhong,sizeof(GetPhoneTransferFenhong));

		GetPhoneTransferFenhong.n_t_userid = theAccount.user_id;
		GetPhoneTransferFenhong.n_t_type = 1;

		if(m_MissionManager!=NULL)
			m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_PHONE_TRASFER_FENHONG,&GetPhoneTransferFenhong,sizeof(GetPhoneTransferFenhong));
		return;
	}
}