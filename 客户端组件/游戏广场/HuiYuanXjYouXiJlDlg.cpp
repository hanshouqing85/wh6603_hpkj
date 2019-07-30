#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanXjYouXiJlDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
//��ʼʱ��
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

//��ֹʱ��
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

//�˺�
static const int tip_edit_act_x = 330;
static const int tip_edit_act_y = 8;
static const int tip_edit_act_width = 40;
static const int tip_edit_act_height = 20;
static CRect rc_edit_act(tip_edit_act_x, 
												 tip_edit_act_y, 
												 tip_edit_act_x+tip_edit_act_width, 
												 tip_edit_act_y+tip_edit_act_height);

static const int edit_act_x = 375;
static const int edit_act_y = 8;
static const int edit_act_width = 95;
static const int edit_act_height = 20;

//ID
static const int tip_edit_id_x = 410;
static const int tip_edit_id_y = 10;
static const int tip_edit_id_width = 40;
static const int tip_edit_id_height = 20;
static CRect rc_edit_id(tip_edit_id_x,
												tip_edit_id_y,
												tip_edit_id_x+tip_edit_id_width,
												tip_edit_id_y+tip_edit_id_height);

static const int edit_id_x = 455;
static const int edit_id_y = 8;
static const int edit_id_width = 95;
static const int edit_id_height = 20;

//ȷ����ť
static const int btn_ok_x = 640;
static const int btn_ok_y = 5;

//�б�
static const int list_log_x = 10;
static const int list_log_y = 38;
static const int list_log_width = 987;
static const int list_log_height = 448;

//��ҳ��
static const int tip_page_x = 20;
static const int tip_page_y = 490;
static const int tip_page_width = 400;
static const int tip_page_height = 20;
static CRect rc_tip_page(tip_page_x, 
						 tip_page_y, 
						 tip_page_x + tip_page_width, 
						 tip_page_y + tip_page_height);

//��һҳ
static const int btn_pre_page_x = 793;
static const int btn_pre_page_y = 490;

//��һҳ
static const int btn_next_page_x = 873;
static const int btn_next_page_y = 490;

static const int page_size = 19;

IMPLEMENT_DYNAMIC(CHuiYuanXjYouXiJlDlg, CDialog)

CHuiYuanXjYouXiJlDlg::CHuiYuanXjYouXiJlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanXjYouXiJlDlg::IDD, pParent)
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

CHuiYuanXjYouXiJlDlg::~CHuiYuanXjYouXiJlDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CHuiYuanXjYouXiJlDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CHuiYuanXjYouXiJlDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanXjYouXiJlDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CHuiYuanXjYouXiJlDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CHuiYuanXjYouXiJlDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_YKLOG_CHKXJ, &CHuiYuanXjYouXiJlDlg::OnYklogChkxj)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LOG, &CHuiYuanXjYouXiJlDlg::OnNMRClickListLog)
END_MESSAGE_MAP()


// CHuiYuanXjYouXiJlDlg ��Ϣ�������

void CHuiYuanXjYouXiJlDlg::OnPaint()
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
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

	cacheDC.DrawText(_T("��ʼʱ��"), rc_time_start, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("��"), rc_time_end, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//cacheDC.DrawText(_T("�˻�"), rc_edit_act, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("ID"), rc_edit_id, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

	//CString strTmp;
	//strTmp.Format(_T("��%dҳ/��%dҳ��ÿҳ%d����¼����%d����¼"), 
	//	m_page, m_maxPage, page_size, m_itemCount);
	//cacheDC.DrawText(strTmp, rc_tip_page, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CString strTmp;
 	if (!m_strAct.IsEmpty())
 	{
 		strTmp.Format(_T("�����ҵ�%d����¼"), m_listLog.GetItemCount());
 	}
 	else
	{
		strTmp.Format(_T("��%dҳ/��%dҳ��ÿҳ%d����¼����%d����¼"), 
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

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CHuiYuanXjYouXiJlDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanXjYouXiJlDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CHuiYuanXjYouXiJlDlg::OnShowWindow(BOOL bShow, UINT nStatus)
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
		m_user_id = 0;
		m_editID.SetWindowText(L"");
		m_strID.Empty();
	}
	else
	{
		m_listLog.DeleteAllItems();
		m_page = 1;
		m_editAct.SetWindowText(L"");
		m_editID.SetWindowText(L"");
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_dateStart.SetTime(&m_tStart);
		m_dateEnd.SetTime(&m_tEnd);
	}
}

void CHuiYuanXjYouXiJlDlg::OnBnClickedBtnOk()
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

	if(m_strID.IsEmpty())
	{
		MyMessageBox(_T("��������Ҫ��ѯ��ID"));
		return;
	}
	int xj_id = _ttoi(m_strID);
	if (xj_id <= 0) {
		MyMessageBox(_T("��������Ҫ��ѯ��ID"));
		return;
	}
	m_byType=1;
	m_user_id = xj_id;


	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("ѡ���ʱ�����䲻��"));
		return;
	}
// 	else if(m_tStart == m_tEnd)
// 		m_tEnd += CTimeSpan(0, 23, 59, 59);
	m_page = 1;

	m_byTime = TRUE;
	m_listLog.DeleteAllItems();
	SendToServer(1);
	SendToServer(2);


	InvalidateRect(rc_tip_page);
}

void CHuiYuanXjYouXiJlDlg::OnBnClickedBtnPrePage()
{
	if(m_page == 1)
	{
		MyMessageBox(_T("�Ѿ��ǵ�һҳ��"));
		return;
	}

	m_page--;
	m_listLog.DeleteAllItems();
	SendToServer(2);

	InvalidateRect(rc_tip_page);
}

void CHuiYuanXjYouXiJlDlg::OnBnClickedBtnNextPage()
{
	if(m_page == m_maxPage)
	{
		MyMessageBox(_T("�Ѿ������һҳ��"));
		return;
	}

	m_page++;
	m_listLog.DeleteAllItems();

	SendToServer(2);

	InvalidateRect(rc_tip_page);
}

void CHuiYuanXjYouXiJlDlg::OnYklogChkxj()
{
	return;
}

void CHuiYuanXjYouXiJlDlg::OnNMRClickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

BOOL CHuiYuanXjYouXiJlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg3.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt.png"));
	m_btnPrePage.SetImage(CBmpUtil::GetExePath() + _T("skin\\syy_bt.png"));
	m_btnNextPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\xyy_bt.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	m_byType=0;
	m_editAct.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(20,20,20));
	m_editID.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(20,20,20));

	InitListCtrl();
	m_dwTickCount=0;
	m_editAct.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CHuiYuanXjYouXiJlDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanXjYouXiJlDlg::OnOK()
{
	//CDialog::OnOK();
}

void CHuiYuanXjYouXiJlDlg::InitListCtrl()
{
	m_listLog.ModifyStyle(NULL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(95,82,81));

	m_listLog.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 100);
	m_listLog.InsertColumn(1, _T("�û���"), LVCFMT_CENTER, 120);
	m_listLog.InsertColumn(2, _T("������"), LVCFMT_CENTER, 100);
	m_listLog.InsertColumn(3, _T("��¼ID"), LVCFMT_CENTER, 100);
	m_listLog.InsertColumn(4, _T("����ʱ��"), LVCFMT_CENTER, 180);
	m_listLog.InsertColumn(5, _T("ӯ��"), LVCFMT_CENTER, 160);
	m_listLog.InsertColumn(6, _T("��ע"), LVCFMT_CENTER,223);
}

void CHuiYuanXjYouXiJlDlg::AdjustCtrls()
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
}

void CHuiYuanXjYouXiJlDlg::EnAbleCtrls(bool b)
{
	m_btnPrePage.EnableWindow(b);
	m_btnNextPage.EnableWindow(b);
}



//��ȡ�¼�
bool CHuiYuanXjYouXiJlDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_QP_YINGKUI_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetQiPaiYingkuiCountRet));
				if(wDataSize!=sizeof(CMD_GR_GetQiPaiYingkuiCountRet)) return false;

				CMD_GR_GetQiPaiYingkuiCountRet* pCountRet = (CMD_GR_GetQiPaiYingkuiCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
				m_maxPage = max(1,m_maxPage);

				break;
			}
		case SUB_GP_GET_QP_YINGKUI_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_GetQiPaiYingkuiRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetQiPaiYingkuiRet)!=0) return false;

				//m_fZongYingkui = SCORE_ZERO;
				m_listLog.DeleteAllItems();
				WORD wCount = wDataSize/sizeof(CMD_GR_GetQiPaiYingkuiRet);
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetQiPaiYingkuiRet *pYingkuiLogRet = ((CMD_GR_GetQiPaiYingkuiRet*)pData+i);



					DOUBLE f_t_yingkui = 0.0f;
					CString strTmp;
					strTmp.Format(_T("%d"), pYingkuiLogRet->n_t_userid);

					int item= m_listLog.InsertItem(m_listLog.GetItemCount(),strTmp);
					m_listLog.SetItemData(item,  pYingkuiLogRet->n_t_userid);



					m_listLog.SetItemText(item, 1,  pYingkuiLogRet->s_t_accounts);


					m_listLog.SetItemText(item, 2, pYingkuiLogRet->s_t_servername);

					strTmp.Format(_T("%d"), pYingkuiLogRet->n_t_drawid);
					m_listLog.SetItemText(item, 3, strTmp);

					strTmp.Format(_T("%d-%d-%d %d:%d:%d"), pYingkuiLogRet->n_t_playtime.wYear,pYingkuiLogRet->n_t_playtime.wMonth,pYingkuiLogRet->n_t_playtime.wDay,pYingkuiLogRet->n_t_playtime.wHour,pYingkuiLogRet->n_t_playtime.wMinute,pYingkuiLogRet->n_t_playtime.wSecond);
					m_listLog.SetItemText(item, 4,strTmp);

					strTmp.Format(_T("%.2lf"), pYingkuiLogRet->f_t_score);

//					m_fZongYingkui += pYingkuiLogRet->f_t_score;

					m_listLog.SetItemText(item, 5, strTmp);

					strTmp.Format(_T(""));

					m_listLog.SetItemText(item, 6, strTmp);


				}
				break;
			}

		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

	//�������
	ASSERT(FALSE);

	return true;
}

VOID CHuiYuanXjYouXiJlDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //��ע��¼����
	{
		m_bGetXjYkLogCount = true;
		if(m_bGetXjYkLogCount)
		{
			m_bGetXjYkLogCount = false;
			CMD_GP_GetQiPaiYingkuiCount GetQiPaiYingkuiCount;
			ZeroMemory(&GetQiPaiYingkuiCount,sizeof(GetQiPaiYingkuiCount));

			GetQiPaiYingkuiCount.dwUserID = m_user_id;
			GetQiPaiYingkuiCount.nTypeID = (m_byTime)?2:1;
			GetQiPaiYingkuiCount.nByTime = (m_byTime)?1:0;

			CopyMemory(GetQiPaiYingkuiCount.szTimeStart , m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetQiPaiYingkuiCount.szTimeStart));
			CopyMemory(GetQiPaiYingkuiCount.szTimeEnd ,m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetQiPaiYingkuiCount.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_QP_YINGKUI_COUNT,&GetQiPaiYingkuiCount,sizeof(GetQiPaiYingkuiCount));
			return;
		}


	}

	if(nSendType == 2)
	{
		m_bGetXjYkLog = true;

		if(m_bGetXjYkLog)
		{
			m_bGetXjYkLog = false;
			CMD_GP_GetQiPaiYingkui QiPaiYingkui;
			ZeroMemory(&QiPaiYingkui,sizeof(QiPaiYingkui));

			QiPaiYingkui.dwUserID = m_user_id;
			QiPaiYingkui.bByTime = m_byTime?1:0;
			QiPaiYingkui.nPage = m_page;
			QiPaiYingkui.nSize = page_size;

			CopyMemory(QiPaiYingkui.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(QiPaiYingkui.szTimeStart));
			CopyMemory(QiPaiYingkui.szTimeEnd  , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(QiPaiYingkui.szTimeEnd));


			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_QP_YINGKUI,&QiPaiYingkui,sizeof(QiPaiYingkui));
			return;

		}


	}

	if(nSendType == 3)
	{
		m_bGetXjYkLogByID = true;
		if(m_bGetXjYkLogByID)
		{
			m_bGetXjYkLogByID = false;
			CMD_GP_GetXJYKLogByID GetXJYKLog;
			ZeroMemory(&GetXJYKLog,sizeof(GetXJYKLog));

			GetXJYKLog.dwUserID = theAccount.user_id;
			int xj_id = _ttoi(m_strID);
			GetXJYKLog.nXiaJiID = xj_id;
			GetXJYKLog.nPage = m_page;
			GetXJYKLog.nSize = page_size;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJYK_LOG_BY_ID,&GetXJYKLog,sizeof(GetXJYKLog));
			return;

		}

	}

	if(nSendType == 4)
	{
		m_bGetXjYkLogByAct = true;
		if(m_bGetXjYkLogByAct)
		{
			m_bGetXjYkLogByAct=false;
			CMD_GP_GetXJYKLogByAct Log;
			ZeroMemory(&Log,sizeof(Log));

			Log.dwUserID = theAccount.user_id;
			lstrcpyn(Log.szAccount,m_strAct.GetBuffer(),sizeof(Log.szAccount));
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJYK_LOG_BY_ACT,&Log,sizeof(Log));
			return;

		}
	}

}
void CHuiYuanXjYouXiJlDlg::QueryChkXjYxYk(int n_t_id)
{
	m_strID.Format(L"%d",n_t_id);
	m_editID.SetWindowText(m_strID);
	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("ѡ���ʱ�����䲻��"));
		return;
	}
// 	else if(m_tStart == m_tEnd)
// 		m_tEnd += CTimeSpan(0, 23, 59, 59);
	m_page = 1;

	m_byTime = TRUE;
	m_listLog.DeleteAllItems();

	m_user_id = n_t_id;
	m_byType=1;
	SendToServer(1);
	SendToServer(2);
}
