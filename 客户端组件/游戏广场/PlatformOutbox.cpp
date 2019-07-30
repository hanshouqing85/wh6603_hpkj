#include "stdafx.h"
#include "GamePlaza.h"
#include "PlatformOutbox.h"
#include "ZhanNeiXinXX.h"
#include "GameRule.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
#include <map>
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


//�ں�
static const int tip_edit_qihao_x = 480;
static const int tip_edit_qihao_y = 36;
static const int tip_edit_qihao_width = 70;
static const int tip_edit_qihao_height = 20;
static CRect rc_edit_qihao(tip_edit_qihao_x, 
						   tip_edit_qihao_y, 
						   tip_edit_qihao_x + tip_edit_qihao_width, 
						   tip_edit_qihao_y + tip_edit_qihao_height);

static const int edit_qihao_x = 550;
static const int edit_qihao_y = 36;
static const int edit_qihao_width = 60;
static const int edit_qihao_height = 20;

//ȷ����ť
static const int btn_ok_x = 760;
static const int btn_ok_y = 8;

//�б�
static const int list_log_x = 10;
static const int list_log_y = 38;
static const int list_log_width = 987;
static const int list_log_height = 400;

//��ҳ��
static const int tip_page_x = 20;
static const int tip_page_y = 443;
static const int tip_page_width = 400;
static const int tip_page_height = 20;
static CRect rc_tip_page(tip_page_x, 
						 tip_page_y, 
						 tip_page_x + tip_page_width, 
						 tip_page_y + tip_page_height);

//��һҳ
static const int btn_pre_page_x = 793;
static const int btn_pre_page_y = 443;

//��һҳ
static const int btn_next_page_x = 873;
static const int btn_next_page_y = 443;

static const int page_size = 19;

IMPLEMENT_DYNAMIC(CPlatformOutbox, CDialog)
//map<int,CString> mapContent;
CPlatformOutbox::CPlatformOutbox(CWnd* pParent /*=NULL*/)
: CDialog(CPlatformOutbox::IDD, pParent)
, m_bmpBk(NULL)
, m_tStart(CTime::GetCurrentTime())
, m_tEnd(CTime::GetCurrentTime())
, m_qihao(0)
, m_page(1)
, m_itemCount(0)
, m_maxPage(1)
, m_byTime(TRUE)
{
	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	m_nTypeID = 2;
	m_bGetTouzhuLog = false;
	m_bGetTouzhuLogCount = false;
	m_bCancelTouzhu = false;
	m_nCancelID = 0;
	m_nCancelKind = 0;
	m_strQihao.Empty();
}

CPlatformOutbox::~CPlatformOutbox()
{
	if (m_bmpBk != NULL)
	{
		SafeDelete(m_bmpBk);
	}
}

void CPlatformOutbox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_START, m_dateStart);
	DDX_Control(pDX, IDC_DATE_END, m_dateEnd);
	DDX_Control(pDX, IDC_EDIT_QIHAO, m_editQihao);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_PRE_PAGE, m_btnPrePage);
	DDX_Control(pDX, IDC_BTN_NEXT_PAGE, m_btnNextPage);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_tStart);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_tEnd);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Text(pDX, IDC_EDIT_QIHAO, m_qihao);
}


BEGIN_MESSAGE_MAP(CPlatformOutbox, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CPlatformOutbox::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CPlatformOutbox::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CPlatformOutbox::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOG, &CPlatformOutbox::OnNMDblclkListLog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LOG, &CPlatformOutbox::OnNMRclickListLog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LOG, &CPlatformOutbox::OnNMLclickListLog)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CPlatformOutbox::OnBnClickedBtnClose)
	ON_COMMAND(ID_ZNX_CHAKAN, &CPlatformOutbox::OnPlatformInboxChk)
	ON_COMMAND(ID_ZNX_SHANCHU, &CPlatformOutbox::OnPlatformInboxShc)
END_MESSAGE_MAP()


// CPlatformOutbox ��Ϣ�������

void CPlatformOutbox::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0,  rect.Width(), rect.Height()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(255,255,255));

// 	cacheDC.DrawText(_T("��ʼʱ��"), rc_time_start, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
// 	cacheDC.DrawText(_T("��"), rc_time_end, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//cacheDC.DrawText(_T("Ͷע�ں�"), rc_edit_qihao, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

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

BOOL CPlatformOutbox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
// CZhanghuDlg ��Ϣ�������
void CPlatformOutbox::OnBnClickedBtnClose()
{
	CWnd* pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->PostMessage(IDM_RETURN_GAME,1,0);
	}

	return;
}

void CPlatformOutbox::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}
	if(m_btnClose.GetSafeHwnd() != NULL)
	{
		m_btnClose.SetWindowPos(NULL, cx - m_btnClose.Width() - 8, -1, m_btnClose.Width(), m_btnClose.Height(), SWP_NOZORDER);
		m_btnClose.ShowWindow(SW_HIDE);
	}

	AdjustCtrls();
}

void CPlatformOutbox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;


		m_page = 1;
	
	
		SendToServer(1); //��ѯ��־����
	
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

void CPlatformOutbox::OnBnClickedBtnOk()
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
	m_page = 1;

	SendToServer(1);
	SendToServer(2);
	InvalidateRect(rc_tip_page);
}

void CPlatformOutbox::OnBnClickedBtnPrePage()
{
	if(m_page == 1)
	{
		MyMessageBox(_T("�Ѿ��ǵ�һҳ��"));
		return;
	}

	//���ݿ���ʱ���� Alter by a'long

	m_page--;
	m_listLog.DeleteAllItems();
	SendToServer(1);
	InvalidateRect(rc_tip_page);
}

void CPlatformOutbox::OnBnClickedBtnNextPage()
{
	if(m_page == m_maxPage)
	{
		MyMessageBox(_T("�Ѿ������һҳ��"));
		return;
	}


	m_page++;
	m_listLog.DeleteAllItems();
	SendToServer(1);
	InvalidateRect(rc_tip_page);
}

void CPlatformOutbox::OnNMDblclkListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		int t_id = m_listLog.GetItemData(nItem);

		CString sTitle = m_listLog.GetItemText(nItem, 0);
		CString sName = m_listLog.GetItemText(nItem, 1);
		CString sTime = m_listLog.GetItemText(nItem, 2);
// 		CZhanNeiXinXX dlg;
// 		dlg.SetContent(sTitle,sName,mapContent[nItem],sTime);


//		dlg.DoModal();
	}
	*pResult = 0;
}

void CPlatformOutbox::OnNMRclickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}

	int nItem = m_listLog.GetNextSelectedItem(pos);
	CString sState = m_listLog.GetItemText(nItem, 6);

	POINT pt = {0, 0};
	GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_ZNX);

	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
	*pResult = 0;
}
void CPlatformOutbox::OnNMLclickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}

	int nItem = m_listLog.GetNextSelectedItem(pos);
	CString sState = m_listLog.GetItemText(nItem, 6);

	POINT pt = {0, 0};
	GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_ZNX);

	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
	*pResult = 0;
}

void CPlatformOutbox::OnPlatformInboxChk()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		int t_id = m_listLog.GetItemData(nItem);

		m_nChkID = t_id;
		CString sTitle = m_listLog.GetItemText(nItem, 0);
		CString sName = m_listLog.GetItemText(nItem, 1);
		CString sTime = m_listLog.GetItemText(nItem, 2);
		//m_listLog.SetItemText(nItem,3,L"�Ѳ鿴");
		CString sContent = mapContent[t_id];
	//	SendToServer(2);
		CZhanNeiXinXX dlg;
		dlg.SetContent(sTitle,sName,sContent,sTime,m_nTypeID);
		

		dlg.DoModal();
	}
}

void CPlatformOutbox::OnPlatformInboxShc()
{
	SendToServer(3);
}

BOOL CPlatformOutbox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg3.png"));
	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\return_bt.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\bt_shuaxin.png"));
	m_btnPrePage.SetImage(CBmpUtil::GetExePath() + _T("skin\\syy_bt.png"));
	m_btnNextPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\xyy_bt.png"));



	InitListCtrl();
	m_dwTickCount = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPlatformOutbox::OnCancel()
{
	//CDialog::OnCancel();
}

void CPlatformOutbox::OnOK()
{
	//CDialog::OnOK();
}

void CPlatformOutbox::InitListCtrl()
{
	m_listLog.ModifyStyle(WS_HSCROLL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_UNDERLINEHOT);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(56,90,154));
	//m_listLog.SetHeaderTextColor(RGB(210,210,210));

	m_listLog.InsertColumn(0, _T("����"), LVCFMT_LEFT,200);
	if(m_nTypeID ==1)
		m_listLog.InsertColumn(1, _T("������"), LVCFMT_LEFT,200);
	else
		m_listLog.InsertColumn(1, _T("�ռ���"), LVCFMT_LEFT,200);
	m_listLog.InsertColumn(2, _T("ʱ��"), LVCFMT_LEFT,200);
	m_listLog.InsertColumn(3, _T("״̬"), LVCFMT_LEFT,200);
	m_listLog.InsertColumn(4, _T("����"), LVCFMT_LEFT,200);
}

void CPlatformOutbox::AdjustCtrls()
{
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
	if (m_editQihao.GetSafeHwnd() != NULL)
	{
		m_editQihao.SetWindowPos(NULL, edit_qihao_x, edit_qihao_y, edit_qihao_width, edit_qihao_height, SWP_NOZORDER);
		m_editQihao.ShowWindow(SW_HIDE);
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
}

//��ȡ�¼�
bool CPlatformOutbox::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_DEL_MESSAGE_RET:
			{
				ASSERT((wDataSize==sizeof(CMD_GP_DelMessageRet)));
				if((wDataSize!=sizeof(CMD_GP_DelMessageRet)))
					return false;

				SendToServer(1);
				CMD_GP_DelMessageRet* pDelMessage = (CMD_GP_DelMessageRet*)pData;
				return true;
			}

		case SUB_GP_GET_INBOX_MESSAGE_RET:
			{
				ASSERT((wDataSize%sizeof(CMD_GP_GetInboxMessageRet))==0);
				if((wDataSize%sizeof(CMD_GP_GetInboxMessageRet))!=0)
					return false;

				int nCount = wDataSize/sizeof(CMD_GP_GetInboxMessageRet);
				for(int i = 0;i < nCount;i++)
				{
					CMD_GP_GetInboxMessageRet* pLog = ((CMD_GP_GetInboxMessageRet*)pData+i);

					int item = m_listLog.InsertItem(m_listLog.GetItemCount(), pLog->s_t_title);
					m_listLog.SetItemData(item, pLog->n_t_id);
					mapContent[pLog->n_t_id].Format(L"%s",pLog->s_t_content);
 					if(pLog->n_t_ifSj == 1)
 						m_listLog.SetItemText(item, 1, L"�ϼ�");
					else if(pLog->n_t_ifSj == 2)
						m_listLog.SetItemText(item, 1, L"�ͷ�");

					else
						m_listLog.SetItemText(item, 1, pLog->s_t_username);
					CTime t(pLog->n_t_time);
					m_listLog.SetItemText(item, 2, t.Format(L"%Y%m%d %H:%M:%S"));
// 					if(m_nTypeID ==1)
// 						m_listLog.SetItemText(item, 3, (pLog->c_t_ifread==0)?L"����Ϣ":L"�Ѳ鿴");
// 					else
						m_listLog.SetItemText(item, 3,L"�ѷ���");
					m_listLog.SetItemText(item, 4, L"�Ҽ�ѡ�����");

					//mapContent[i].Format(L"%s",pLog->s_t_content);
					m_itemCount = pLog->n_t_count;
					m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
					m_maxPage = max(1,m_maxPage);

				}
			}
		}
		InvalidateRect(&rc_tip_page);
		return true;
	}

	//�������
	ASSERT(FALSE);

	return true;
}

VOID CPlatformOutbox::SendToServer(int nSendType)
{
	if(nSendType == 1)   //��ȡվ����
	{
		mapContent.clear();
		m_listLog.DeleteAllItems();

		CMD_GP_GetInboxMessage GetInboxMessage;
		ZeroMemory(&GetInboxMessage,sizeof(GetInboxMessage));

		
		GetInboxMessage.n_t_userid = theAccount.user_id;
		GetInboxMessage.n_t_typeid = m_nTypeID;
		GetInboxMessage.n_t_page = m_page;
		GetInboxMessage.n_t_size = page_size;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_INBOX_MESSAGE,&GetInboxMessage,sizeof(GetInboxMessage));
		return;
	}

	if(nSendType == 2)
	{
		CMD_GP_ChkInboxMessage ChkInboxMessage;
		ZeroMemory(&ChkInboxMessage,sizeof(ChkInboxMessage));

		ChkInboxMessage.n_t_userid = theAccount.user_id;
		ChkInboxMessage.n_t_id = m_nChkID;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CHK_INBOX_MESSAGE,&ChkInboxMessage,sizeof(ChkInboxMessage));
		return;

	}
	if(nSendType == 3)
	{

		POSITION pos = m_listLog.GetFirstSelectedItemPosition();
		if (pos != NULL) 
		{
			int nItem = m_listLog.GetNextSelectedItem(pos);
			int t_id = m_listLog.GetItemData(nItem);

			CMD_GP_DelMessage DelMessage;
			ZeroMemory(&DelMessage,sizeof(DelMessage));
			DelMessage.n_t_type = 1;
			DelMessage.n_t_id = t_id;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_DEL_MESSAGE,&DelMessage,sizeof(DelMessage));
		}
		return;

	}

}