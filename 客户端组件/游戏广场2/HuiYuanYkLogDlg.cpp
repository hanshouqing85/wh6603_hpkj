#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanYkLogDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
//起始时间
static const int tip_time_start_x = 18;
static const int tip_time_start_y = 10;
static const int tip_time_start_width = 70;
static const int tip_time_start_height = 20;
static CRect rc_time_start(tip_time_start_x, 
						   tip_time_start_y, 
						   tip_time_start_x + tip_time_start_width, 
						   tip_time_start_y + tip_time_start_height);

static const int time_start_x = 90;
static const int time_start_y = 8;
static const int time_start_width = 100;
static const int time_start_height = 20;

//截止时间
static const int tip_time_end_x = 190;
static const int tip_time_end_y = 10;
static const int tip_time_end_width = 20;
static const int tip_time_end_height = 20;
static CRect rc_time_end(tip_time_end_x, 
						 tip_time_end_y, 
						 tip_time_end_x + tip_time_end_width, 
						 tip_time_end_y + tip_time_end_height);

static const int time_end_x = 210;
static const int time_end_y = 8;
static const int time_end_width = 100;
static const int time_end_height = 20;

//账号
static const int tip_edit_act_x = 310;
static const int tip_edit_act_y = 8;
static const int tip_edit_act_width = 40;
static const int tip_edit_act_height = 20;
static CRect rc_edit_act(tip_edit_act_x, 
												 tip_edit_act_y, 
												 tip_edit_act_x+tip_edit_act_width, 
												 tip_edit_act_y+tip_edit_act_height);

static const int edit_act_x = 355;
static const int edit_act_y = 8;
static const int edit_act_width = 95;
static const int edit_act_height = 20;


//ID
static const int tip_edit_id_x = 430;
static const int tip_edit_id_y = 8;
static const int tip_edit_id_width = 40;
static const int tip_edit_id_height = 20;
static CRect rc_edit_id(tip_edit_id_x,
												tip_edit_id_y,
												tip_edit_id_x+tip_edit_id_width,
												tip_edit_id_y+tip_edit_id_height);

static const int edit_id_x = 475;
static const int edit_id_y = 8;
static const int edit_id_width = 95;
static const int edit_id_height = 20;

//确定按钮
static const int btn_ok_x = 680;
static const int btn_ok_y = 3;
//排序
static const int cmb_sort_x = 575;
static const int cmb_sort_y = 7;
static const int cmb_sort__width = 95;
static const int cmb_sort__height = 14;


//列表
static const int list_log_x = 10;
static const int list_log_y = 38;
static const int list_log_width = 960;
static const int list_log_height = 388;

//总页数
static const int tip_page_x = 20;
static const int tip_page_y = 433;
static const int tip_page_width = 400;
static const int tip_page_height = 20;
static CRect rc_tip_page(tip_page_x, 
						 tip_page_y, 
						 tip_page_x + tip_page_width, 
						 tip_page_y + tip_page_height);

//上一页
static const int btn_pre_page_x = 793;
static const int btn_pre_page_y = 433;

//下一页
static const int btn_next_page_x = 873;
static const int btn_next_page_y = 433;


static const int page_size = 19;

IMPLEMENT_DYNAMIC(CHuiYuanYkLogDlg, CDialog)

CHuiYuanYkLogDlg::CHuiYuanYkLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanYkLogDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_tStart(CTime::GetCurrentTime())
	, m_tEnd(CTime::GetCurrentTime())
	, m_page(1)
	, m_itemCount(0)
	, m_maxPage(1)
	, m_byTime(TRUE)
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
	m_bGetXjYkLogCount=false;
	m_bGetXjYkLog=false;
	m_bGetXjYkLogByID=false;
	m_bGetXjYkLogByAct=false;
}

CHuiYuanYkLogDlg::~CHuiYuanYkLogDlg()
{
	if (m_bmpBk != NULL)
	{
		SafeDelete(m_bmpBk);
	}
}

void CHuiYuanYkLogDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CHuiYuanYkLogDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanYkLogDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CHuiYuanYkLogDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CHuiYuanYkLogDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_YKLOG_CHKXJ, &CHuiYuanYkLogDlg::OnYklogChkxj)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LOG, &CHuiYuanYkLogDlg::OnNMRClickListLog)
END_MESSAGE_MAP()


// CHuiYuanYkLogDlg 消息处理程序

void CHuiYuanYkLogDlg::OnPaint()
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
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(255,255,255));

	cacheDC.DrawText(_T("起始时间"), rc_time_start, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("至"), rc_time_end, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("账户"), rc_edit_act, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("ID"), rc_edit_id, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

	//CString strTmp;
	//strTmp.Format(_T("第%d页/共%d页，每页%d条记录，共%d条记录"), 
	//	m_page, m_maxPage, page_size, m_itemCount);
	//cacheDC.DrawText(strTmp, rc_tip_page, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CString strTmp;
 	if (!m_strAct.IsEmpty())
 	{
 		strTmp.Format(_T("共查找到%d条记录"), m_listLog.GetItemCount());
 	}
 	else
	{
		strTmp.Format(_T("第%d页/共%d页，每页%d条记录，共%d条记录"), 
			m_page, m_maxPage, page_size, m_itemCount);
	}
	cacheDC.DrawText(strTmp, rc_tip_page, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CHuiYuanYkLogDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanYkLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CHuiYuanYkLogDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_byType=0;
		m_page = 1;
		m_user_id = theAccount.user_id;
	}
	else
	{
		m_listLog.DeleteAllItems();

		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_dateStart.SetTime(&m_tStart);
		m_dateEnd.SetTime(&m_tEnd);
		m_page = 1;
		m_editAct.SetWindowText(L"");
		m_editID.SetWindowText(L"");

	}
}

void CHuiYuanYkLogDlg::OnBnClickedBtnOk()
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
	if ((!m_strID.IsEmpty()))
	{
		int xj_id = _ttoi(m_strID);
		if (xj_id <= 0) {
			MyMessageBox(_T("您输入的ID不对"));
			return;
		}
		m_byType=1;

	}
	else if (!m_strAct.IsEmpty())
	{
		m_byType=2;
	}
	else 
	{
		m_byType=0;

	}
	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("选择的时间区间不对"));
		return;
	}
// 	else if(m_tStart == m_tEnd)
// 		m_tEnd += CTimeSpan(1L, 0, 0, 0);
	m_page = 1;

	m_byTime = TRUE;
	m_listLog.DeleteAllItems();
	//SendToServer(1);
	SendToServer(2);


	InvalidateRect(rc_tip_page);
}

void CHuiYuanYkLogDlg::OnBnClickedBtnPrePage()
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

void CHuiYuanYkLogDlg::OnBnClickedBtnNextPage()
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

void CHuiYuanYkLogDlg::OnYklogChkxj()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		/*CString strType = m_listLog.GetItemText(nItem, 2);
		if (strType == _T("会员"))
		{
			MyMessageBox(_T("此人不是代理，无下级"));
		}
		else
		{*/
			m_user_id = m_listLog.GetItemData(nItem);
			//SendToServer(1);
			SendToServer(2);

	}
}

void CHuiYuanYkLogDlg::OnNMRClickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}

	POINT pt = {0, 0};
	GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_XJYKTJ);

	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
	*pResult = 0;
}

BOOL CHuiYuanYkLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk == NULL)
	{
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg3.png"));

		m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt.png"));
		m_btnPrePage.SetImage(CBmpUtil::GetExePath() + _T("skin\\syy_bt.png"));
		m_btnNextPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\xyy_bt.png"));

		m_font.CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

		m_cmbSort.SetButtonImage(AfxGetInstanceHandle(),IDB_BT_COMBOBOX_ACT,CSize(16,18));
	}
	InitListCtrl();
	m_byType=0;
	m_dwTickCount=0;
	m_editAct.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_editID.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHuiYuanYkLogDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanYkLogDlg::OnOK()
{
	//CDialog::OnOK();
}

void CHuiYuanYkLogDlg::InitListCtrl()
{
	m_listLog.ModifyStyle(NULL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(56,90,154));

	m_listLog.InsertColumn(0, _T("订单ID"), LVCFMT_CENTER, 90);
	m_listLog.InsertColumn(1, _T("用户名"), LVCFMT_CENTER, 120);
	m_listLog.InsertColumn(2, _T("盈亏"), LVCFMT_CENTER, 130);
	m_listLog.InsertColumn(3, _T("彩票余额"), LVCFMT_CENTER, 140);
	m_listLog.InsertColumn(4, _T("盈亏时间"), LVCFMT_CENTER, 140);
	m_listLog.InsertColumn(5, _T("类型"), LVCFMT_CENTER, 133);
	m_listLog.InsertColumn(6, _T("盈亏摘要"), LVCFMT_CENTER, 230);

	m_cmbSort.SetFont(&m_font);
	m_cmbSort.ResetContent();
	m_cmbSort.AddString(TEXT("投注"));
	m_cmbSort.AddString(TEXT("返点"));
	m_cmbSort.AddString(TEXT("下级返点"));
	m_cmbSort.AddString(TEXT("中奖"));
	m_cmbSort.AddString(TEXT("充值"));
	m_cmbSort.AddString(TEXT("取款"));
	m_cmbSort.AddString(TEXT("撤单"));
	m_cmbSort.AddString(TEXT("余额转换"));
	m_cmbSort.AddString(TEXT("活动"));
	m_cmbSort.AddString(TEXT("佣金"));
	m_cmbSort.AddString(TEXT("日工资"));
	m_cmbSort.AddString(TEXT("转账"));
	m_cmbSort.SetCurSel(0);

}

void CHuiYuanYkLogDlg::AdjustCtrls()
{
	if (m_dateStart.GetSafeHwnd() != NULL)
	{
		m_dateStart.SetWindowPos(NULL, time_start_x, time_start_y, time_start_width, time_start_height, SWP_NOZORDER);
	}
	if (m_dateEnd.GetSafeHwnd() != NULL)
	{
		m_dateEnd.SetWindowPos(NULL, time_end_x, time_end_y, time_end_width, time_end_height, SWP_NOZORDER);
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

	if (m_cmbSort.GetSafeHwnd() != NULL)
	{
		m_cmbSort.SetWindowPos(NULL, cmb_sort_x, cmb_sort_y, cmb_sort__width,cmb_sort__height, SWP_NOZORDER);
	}

}

void CHuiYuanYkLogDlg::EnAbleCtrls(bool b)
{
	m_btnPrePage.EnableWindow(b);
	m_btnNextPage.EnableWindow(b);
}


//读取事件
bool CHuiYuanYkLogDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_XJYK_LOG_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetXJYKLogCountRet));
				if(wDataSize!=sizeof(CMD_GR_GetXJYKLogCountRet)) return false;

				CMD_GR_GetXJYKLogCountRet* pCountRet = (CMD_GR_GetXJYKLogCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
				m_maxPage = max(1,m_maxPage);

				
				break;
			}
		case SUB_GP_GET_XJYK_LOG_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_GetXJYKLogRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetXJYKLogRet)!=0) return false;

				WORD wCount = wDataSize/sizeof(CMD_GR_GetXJYKLogRet);

				m_listLog.DeleteAllItems();
				SendToServer(1);

				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetXJYKLogRet *pXJYKLogRet = ((CMD_GR_GetXJYKLogRet*)pData+i);

					CString strTmp;
					strTmp.Format(_T("%d"), pXJYKLogRet->n_t_id);
					int item = m_listLog.InsertItem(m_listLog.GetItemCount(), strTmp);
					m_listLog.SetItemData(item, pXJYKLogRet->n_t_id);

					m_listLog.SetItemText(item, 1,pXJYKLogRet->s_t_account);

					DOUBLE t_yingkui = pXJYKLogRet->f_t_yingkui;
					strTmp.Format(_T("%.3lf"), t_yingkui);
					m_listLog.SetItemText(item, 2, strTmp);

					DOUBLE t_at_yue = pXJYKLogRet->f_t_at_yue;
					strTmp.Format(_T("%.3lf"), t_at_yue);
					m_listLog.SetItemText(item, 3, strTmp);


					CTime ct(pXJYKLogRet->n_t_time);
					m_listLog.SetItemText(item, 4, ct.Format(_T("%Y%m%d %H:%M:%S")));

					int t_type = pXJYKLogRet->n_t_type;
					switch (t_type)
					{
					case 0: strTmp = _T("投注"); break;
					case 1: strTmp = _T("返点"); break;
					case 2: strTmp = _T("下级返点"); break;
					case 3: strTmp = _T("中奖"); break;
					case 4: strTmp = _T("充值"); break;
					case 5: strTmp = _T("取款"); break;
					case 6: strTmp = _T("活动"); break;
					case 7: strTmp = _T("签到"); break;
					case 8: strTmp = _T("撤单"); break;
					case 9:
						{
							if(pXJYKLogRet->n_t_id == 104)
							{
								strTmp = _T("彩票分红");
							}
							else if(pXJYKLogRet->n_t_id == 105)
							{
								strTmp = _T("棋牌分红");
							}
							break;
						}
					case 10: strTmp = _T("消费佣金"); break;
					case 11: strTmp = _T("亏损佣金"); break;
					case 12: strTmp = _T("余额转换"); break;
					case 13: strTmp = _T("棋牌返点"); break;
					case 14:
						{
							if(pXJYKLogRet->n_t_id == 107)
							{
								strTmp = _T("普通转账");
							}
							else if(pXJYKLogRet->n_t_id == 108)
							{
								strTmp = _T("分红转账");
							}
							else if(pXJYKLogRet->n_t_id == 109)
							{
								strTmp = _T("工资转账");
							}
							else 
							{
								strTmp = _T("转账");
							}
							break;
						}
					case 15:
						{
							if(pXJYKLogRet->n_t_id == 102)
							{
								strTmp = _T("投注日工资");
							}
							else if(pXJYKLogRet->n_t_id == 103)
							{
								strTmp = _T("亏损日工资");
							}
							else if(pXJYKLogRet->n_t_id == 110)
							{
								strTmp = _T("日工资转账");
							}

							break;
						}
					default: strTmp = _T("其他"); break;
					}
					m_listLog.SetItemText(item, 5, strTmp);

					CString strZhaiyao;
					strZhaiyao.Format(L"%s",pXJYKLogRet->s_t_comment);
					if((t_type >= 0 && t_type <=3)||(t_type == 8))
					{
						CString strTemp;
						int nLength = strZhaiyao.Find(L"|");
						strTemp = strZhaiyao.Right(strZhaiyao.GetLength()-nLength-1);
						int nLength1 = strTemp.Find(L"|");
						CString strQihao  = strZhaiyao.Left(nLength);
						CString strType = strZhaiyao.Mid(nLength+1,nLength1);
						CString wanfa = strZhaiyao.Right(strZhaiyao.GetLength()-nLength-2-nLength1);

						if(strQihao.IsEmpty())
							break;
						int nType = _ttoi(strType);

						strTmp = theGameType[nType];
						CString str1;
						str1.Format(L"[%s%s期]",strQihao.Left(8),strQihao.Right(strQihao.GetLength()-8-1));
						strTmp += str1;
						int nWanfa = _ttoi(wanfa);

						strTmp += theGameKind[(nType << 16) | (unsigned short)nWanfa];
					}
					else if(t_type == 12)
					{
						strTmp = _T("余额兑换");
					}
					else
					{
						strTmp.Format(L"%s",pXJYKLogRet->s_t_comment); 
					}
					m_listLog.SetItemText(item, 6,strTmp);

				}
				return true;
			}

		}
	}
	//RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	InvalidateRect(&rc_tip_page);


	return true;
}

VOID CHuiYuanYkLogDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //下注记录数量
	{
		m_bGetXjYkLogCount = true;
		if(m_bGetXjYkLogCount)
		{
			DWORD dwTickCount = GetTickCount();
			CString strLog;
			strLog.Format(L"XIHYLOG tick4:%d",dwTickCount);
			OutputDebugString(strLog);
			m_bGetXjYkLogCount = false;
			CMD_GP_GetXJYKLogCount GetXJYKLogCount;
			ZeroMemory(&GetXJYKLogCount,sizeof(GetXJYKLogCount));

			GetXJYKLogCount.dwUserID = theAccount.user_id;
			GetXJYKLogCount.n_t_type = m_byType;

			int xj_id = _ttoi(m_strID);
			GetXJYKLogCount.n_t_user_id = xj_id;
			lstrcpyn(GetXJYKLogCount.s_t_account,m_strAct.GetBuffer(),sizeof(GetXJYKLogCount.s_t_account));
			GetXJYKLogCount.bTime = m_byTime;

			CopyMemory(GetXJYKLogCount.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetXJYKLogCount.szTimeStart));
			CopyMemory(GetXJYKLogCount.szTimeEnd , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetXJYKLogCount.szTimeEnd));
			GetXJYKLogCount.nType = m_cmbSort.GetCurSel();
			dwTickCount = GetTickCount();

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJYK_LOG_COUNT,&GetXJYKLogCount,sizeof(GetXJYKLogCount));
			return;
		}

	}

	if(nSendType == 2)
	{
		m_bGetXjYkLog = true;
		if(m_bGetXjYkLog)
		{

			m_bGetXjYkLog = false;
			CMD_GP_GetXJYKLog GetXJYKLogLog;
			ZeroMemory(&GetXJYKLogLog,sizeof(GetXJYKLogLog));

			GetXJYKLogLog.dwUserID = theAccount.user_id;
			GetXJYKLogLog.n_t_type = m_byType;

			int xj_id = _ttoi(m_strID);
			GetXJYKLogLog.n_t_user_id = xj_id;
			lstrcpyn(GetXJYKLogLog.s_t_account,m_strAct.GetBuffer(),sizeof(GetXJYKLogLog.s_t_account));
			GetXJYKLogLog.nPage = m_page;
			GetXJYKLogLog.nSize = page_size;
			GetXJYKLogLog.bByTime = m_byTime;
			GetXJYKLogLog.n_sort_type = m_cmbSort.GetCurSel();

			CopyMemory(GetXJYKLogLog.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetXJYKLogLog.szTimeStart));
			CopyMemory(GetXJYKLogLog.szTimeEnd  , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetXJYKLogLog.szTimeEnd));

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJYK_LOG,&GetXJYKLogLog,sizeof(GetXJYKLogLog));

			return;
		}

	}


}
void CHuiYuanYkLogDlg::QueryXJYKMX(int n_t_id)
{
	m_strID.Format(L"%d",n_t_id);
	m_editID.SetWindowText(m_strID);
	m_byType=1;
	//SendToServer(1);
	SendToServer(2);
}
