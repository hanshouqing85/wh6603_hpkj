#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhangHuYkLogDlg.h"
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

static const int edit_qihao_x = 430;
static const int edit_qihao_y = 1;
static const int edit_qihao_width = 60;
static const int edit_qihao_height = 20;

//ȷ����ť
static const int btn_ok_x = 520;
static const int btn_ok_y = 3;

//�б�
static const int list_log_x = 10;
static const int list_log_y = 38;
static const int list_log_width = 960;
static const int list_log_height = 388;

//��ҳ��
static const int tip_page_x = 20;
static const int tip_page_y = 433;
static const int tip_page_width = 400;
static const int tip_page_height = 20;
static CRect rc_tip_page(tip_page_x, 
						 tip_page_y, 
						 tip_page_x + tip_page_width, 
						 tip_page_y + tip_page_height);

//��һҳ
static const int btn_pre_page_x = 793;
static const int btn_pre_page_y = 433;

//��һҳ
static const int btn_next_page_x = 873;
static const int btn_next_page_y = 433;

static const int page_size = 19;
// CZhangHuYkLogDlg �Ի���

IMPLEMENT_DYNAMIC(CZhangHuYkLogDlg, CDialog)

CZhangHuYkLogDlg::CZhangHuYkLogDlg(CWnd* pParent /*=NULL*/)
: CDialog(CZhangHuYkLogDlg::IDD, pParent)
, m_bmpBk(NULL)
, m_tStart(CTime::GetCurrentTime())
, m_tEnd(CTime::GetCurrentTime())
, m_page(1)
, m_itemCount(0)
, m_maxPage(1)
, m_byTime(TRUE)
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
	m_bGetYingkuiLogCount = false;
	m_bGetYingkuiLog = false;
}

CZhangHuYkLogDlg::~CZhangHuYkLogDlg()
{
	if (m_bmpBk != NULL)
	{
		SafeDelete(m_bmpBk);

	}
}

void CZhangHuYkLogDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CZhangHuYkLogDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CZhangHuYkLogDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CZhangHuYkLogDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CZhangHuYkLogDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CZhangHuYkLogDlg ��Ϣ�������

void CZhangHuYkLogDlg::OnPaint()
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
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(255,255,255));

	cacheDC.DrawText(_T("��ʼʱ��"), rc_time_start, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("��"), rc_time_end, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	CString strTmp;
	strTmp.Format(_T("��%dҳ/��%dҳ��ÿҳ%d����¼����%d����¼"), 
		m_page, m_maxPage, page_size, m_itemCount);
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

BOOL CZhangHuYkLogDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CZhangHuYkLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CZhangHuYkLogDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_page = 1;
		SendToServer(1);
		SendToServer(2);
	}
	else
	{
		m_listLog.DeleteAllItems();
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_dateStart.SetTime(&m_tStart);
		m_dateEnd.SetTime(&m_tEnd);
		m_page = 1;

	}
}

void CZhangHuYkLogDlg::OnBnClickedBtnOk()
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

	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("ѡ���ʱ�����䲻��"));
		return;
	}

	m_byTime = TRUE;
	m_listLog.DeleteAllItems();
//	m_tEnd += CTimeSpan(1L, 0, 0, 0);
	m_page = 1;
	SendToServer(1);
	SendToServer(2);

	InvalidateRect(rc_tip_page);
}

void CZhangHuYkLogDlg::OnBnClickedBtnPrePage()
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

void CZhangHuYkLogDlg::OnBnClickedBtnNextPage()
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

BOOL CZhangHuYkLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(m_bmpBk!=NULL)
		return TRUE;

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg3.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt.png"));
	m_btnPrePage.SetImage(CBmpUtil::GetExePath() + _T("skin\\syy_bt.png"));
	m_btnNextPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\xyy_bt.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	InitListCtrl();
	m_dwTickCount = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CZhangHuYkLogDlg::InitListCtrl()
{
	m_listLog.ModifyStyle(NULL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(56,90,154));

	m_listLog.InsertColumn(0, _T("�û���"), LVCFMT_CENTER, 120);
	m_listLog.InsertColumn(1, _T("����"), LVCFMT_CENTER, 100);
	m_listLog.InsertColumn(2, _T("��ֵ���"), LVCFMT_CENTER, 96);
	m_listLog.InsertColumn(3, _T("ȡ����"), LVCFMT_CENTER, 96);
	m_listLog.InsertColumn(4, _T("Ͷע���"), LVCFMT_CENTER, 96);
	m_listLog.InsertColumn(5, _T("������"), LVCFMT_CENTER, 96);
	m_listLog.InsertColumn(6, _T("�н����"), LVCFMT_CENTER, 96);
	m_listLog.InsertColumn(7, _T("�"), LVCFMT_CENTER, 96);
	m_listLog.InsertColumn(8, _T("ӯ��"), LVCFMT_CENTER, 94);
	m_listLog.InsertColumn(9, _T("���"), LVCFMT_CENTER, 93);
}

void CZhangHuYkLogDlg::AdjustCtrls()
{
	if (m_dateStart.GetSafeHwnd() != NULL)
	{
		m_dateStart.SetWindowPos(NULL, time_start_x, time_start_y, time_start_width, time_start_height, SWP_NOZORDER);
	}
	if (m_dateEnd.GetSafeHwnd() != NULL)
	{
		m_dateEnd.SetWindowPos(NULL, time_end_x, time_end_y, time_end_width, time_end_height, SWP_NOZORDER);
	}
	//if (m_editQihao.GetSafeHwnd() != NULL)
	//{
	//	m_editQihao.SetWindowPos(NULL, edit_qihao_x, edit_qihao_y, edit_qihao_width, edit_qihao_height, SWP_NOZORDER);
	//}
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
}

void CZhangHuYkLogDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
}

void CZhangHuYkLogDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}


//��ȡ�¼�
bool CZhangHuYkLogDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_YINGKUI_LOG_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetYingkuiLogCountRet));
				if(wDataSize!=sizeof(CMD_GR_GetYingkuiLogCountRet)) return false;

				CMD_GR_GetYingkuiLogCountRet* pCountRet = (CMD_GR_GetYingkuiLogCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
				m_maxPage = max(1,m_maxPage);

				break;
			}
		case SUB_GP_GET_YINGKUI_LOG_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_GetYingkuiLogRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetYingkuiLogRet)!=0) return false;
				m_listLog.DeleteAllItems();

				WORD wCount = wDataSize/sizeof(CMD_GR_GetYingkuiLogRet);
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetYingkuiLogRet *pYingkuiLogRet = ((CMD_GR_GetYingkuiLogRet*)pData+i);

					DOUBLE f_t_yingkui = 0.0f;
					CString strTmp;

					int item= m_listLog.InsertItem(m_listLog.GetItemCount(), theAccount.account);
					m_listLog.SetItemData(item, theAccount.user_id);
					if(theAccount.type==1)
						m_listLog.SetItemText(item, 1, L"����");
					else
						m_listLog.SetItemText(item, 1, L"����");

					strTmp.Format(_T("%.3lf"), pYingkuiLogRet->f_t_chongzhi);
					m_listLog.SetItemText(item, 2, strTmp);

					strTmp.Format(_T("%.3lf"), pYingkuiLogRet->f_t_qukuan);
					m_listLog.SetItemText(item, 3, strTmp);

					strTmp.Format(_T("%.3lf"), pYingkuiLogRet->f_t_touzhu);
					m_listLog.SetItemText(item, 4, strTmp);
					f_t_yingkui = pYingkuiLogRet->f_t_touzhu;

					strTmp.Format(_T("%.3lf"), pYingkuiLogRet->f_t_fandian);
					m_listLog.SetItemText(item, 5, strTmp);
					f_t_yingkui+=pYingkuiLogRet->f_t_fandian;

					strTmp.Format(_T("%.3lf"), pYingkuiLogRet->f_t_zhongjiang);
					m_listLog.SetItemText(item, 6, strTmp);
					f_t_yingkui+=pYingkuiLogRet->f_t_zhongjiang;

					strTmp.Format(_T("%.3lf"), pYingkuiLogRet->f_t_huodong);
					m_listLog.SetItemText(item, 7, strTmp);
					f_t_yingkui+=pYingkuiLogRet->f_t_huodong;

					strTmp.Format(_T("%.3lf"), f_t_yingkui);
					m_listLog.SetItemText(item, 8, strTmp);

					strTmp.Format(_T("%.3lf"), pYingkuiLogRet->f_t_chedan);
					m_listLog.SetItemText(item, 9, strTmp);

				}
				break;
			}
		}
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
		return true;
	}

	//�������
	ASSERT(FALSE);

	return true;
}

VOID CZhangHuYkLogDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //��ע��¼����
	{
		m_bGetYingkuiLogCount = true;
		if(m_bGetYingkuiLogCount)
		{
			m_bGetYingkuiLogCount = false;
			CMD_GP_GetYingkuiLogCount GetYingkuiLogCount;
			ZeroMemory(&GetYingkuiLogCount,sizeof(GetYingkuiLogCount));

			GetYingkuiLogCount.dwUserID = theAccount.user_id;
			GetYingkuiLogCount.bTime = m_byTime;
// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}
// 			m_tEnd += CTimeSpan(0, 23, 59, 59);

			CopyMemory(GetYingkuiLogCount.szTimeStart , m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetYingkuiLogCount.szTimeStart));
			CopyMemory(GetYingkuiLogCount.szTimeEnd ,m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetYingkuiLogCount.szTimeEnd));

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_YINGKUI_LOG_COUNT,&GetYingkuiLogCount,sizeof(GetYingkuiLogCount));
			return;
		}


	}

	if(nSendType == 2)//��ע��¼ 
	{
		m_bGetYingkuiLog = true;
		if(m_bGetYingkuiLog)
		{
			m_bGetYingkuiLog = false;

			CMD_GP_GetYingkuiLog YingkuiLog;
			ZeroMemory(&YingkuiLog,sizeof(YingkuiLog));

			m_listLog.DeleteAllItems();
			YingkuiLog.dwUserID = theAccount.user_id;
			YingkuiLog.nPage = m_page;
			YingkuiLog.nSize = page_size;
			YingkuiLog.bByTime = m_byTime;
// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}
			CopyMemory(YingkuiLog.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(YingkuiLog.szTimeStart));
			CopyMemory(YingkuiLog.szTimeEnd  , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(YingkuiLog.szTimeEnd));
			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_YINGKUI_LOG,&YingkuiLog,sizeof(YingkuiLog));
			return;
		}
	}


}