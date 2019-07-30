#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanHyshjDlg.h"
#include "PlatformFrame.h"
static const int tip_width = 200;
static const int tip_height = 20;
static const int tip_x = 185;
static const int time_start_x = 256;
static const int time_start_y = 26;
static const int time_start_width = 100;
static const int time_start_height = 20;
static const int time_end_x = 383;
static const int time_end_y = 26;
static const int time_end_width = 100;
static const int time_end_height = 20;
//ȷ����ť
static const int btn_ok_x = 601;
static const int btn_ok_y = 22;

static const int tip_span = 211;
//�¼�����
static const int xiajirenshu_y = 102; 
static CRect rc_xiajirenshu(tip_x+ tip_span, 
																	xiajirenshu_y, 
																	tip_x + tip_span+ tip_width,
																	xiajirenshu_y + tip_height); 
//��������
static const int zaixianrenshu_y = 145;
static CRect rc_zaixianrenshu(tip_x + 25 , 
																	zaixianrenshu_y, 
																	tip_x   + tip_width,
																	zaixianrenshu_y + tip_height); 
//��Ա���
static const int huiyuanyue_y = 102;
static CRect rc_huiyuanyue(tip_x , 
																	huiyuanyue_y, 
																	tip_x  + tip_span + tip_width,
																	huiyuanyue_y + tip_height); 
//����ע��
static const int jinrizhuce_y = 102;
static CRect rc_jinrizhuce(tip_x + tip_span*2+10, 
																	jinrizhuce_y, 
																	tip_x  + tip_span*2+10 + tip_width,
																	jinrizhuce_y + tip_height); 
//�����³�ֵ
static const int jinrixinchongzhi_y = 102;
static CRect rc_jinrixinchongzhi(tip_x + tip_span*3+20, 
																	jinrixinchongzhi_y, 
																	tip_x  + tip_span*3+20 + tip_width,
																	jinrixinchongzhi_y + tip_height); 
//���շ���
static const int jinrifandian_y = 306;
static CRect rc_jinrifandian(tip_x + 25 , 
																	jinrifandian_y, 
																	tip_x  + 25 + tip_width,
																	jinrifandian_y + tip_height); 
//�����ܶ�
static const int chedanzonge_y = 262;
static CRect rc_chedanzonge(tip_x + tip_span*2+25, 
																	chedanzonge_y, 
																	tip_x + tip_span*2+25 + tip_width,
																	chedanzonge_y + tip_height); 
//���ճ�ֵ
static const int jinrichongzhi_y = 145;
static CRect rc_jinrichongzhi(tip_x + tip_span, 
																		jinrichongzhi_y, 
																		tip_x  + tip_span + tip_width,
																		jinrichongzhi_y + tip_height); 
//���ճ�ֵ������
static const int jinrichongzhisxf_y = 189;
static CRect rc_jinrichongzhisxf(tip_x + tip_span + 10, 
								 jinrichongzhisxf_y, 
								 tip_x  + tip_span + 10 + tip_width,
								 jinrichongzhisxf_y + tip_height); 

//�ۺ�Ӷ��
static const int zongheyongjin_y = 189;
static CRect rc_zongheyongjin(tip_x , 
								 zongheyongjin_y, 
								 tip_x  + tip_width,
								 zongheyongjin_y + tip_height); 
//����ȡ��
static const int jinriqukuan_y = 145;
static CRect rc_jinriqukuan(tip_x+ tip_span*2, 
																	jinriqukuan_y, 
																	tip_x+ tip_span*2 + tip_width,
																	jinriqukuan_y + tip_height); 
//����ȡ��
static const int zongheyingkui_y = 145;
static CRect rc_zongheyingkui(tip_x+ tip_span*3+25, 
																	zongheyingkui_y, 
																	tip_x+ tip_span*3+25 + tip_width,
																	zongheyingkui_y + tip_height); 
//����Ͷע
static const int jinritouzhu_y = 262;
static CRect rc_jinritouzhu(tip_x + 25, 
																	jinritouzhu_y, 
																	tip_x + 25+ tip_width,
																	jinritouzhu_y + tip_height); 
//�н��ܶ�
static const int zhongjiangzonge_y = 262;
static CRect rc_zhongjiangzonge(tip_x+ tip_span+25, 
																	zhongjiangzonge_y, 
																	tip_x+ tip_span+25 + tip_width,
																	zhongjiangzonge_y + tip_height); 
//�ҵķ��� - ��Ʊ
static const int wodefandian_y = 306;
static CRect rc_wodefandian(tip_x + 25, 
																	wodefandian_y, 
																	tip_x+ 25 + tip_width,
																	wodefandian_y + tip_height); 
//��Ʊӯ��
static const int caipiaoyingkui_y = 306;
static CRect rc_caipiaoyingkui(tip_x+ tip_span*2+25, 
																	caipiaoyingkui_y, 
																	tip_x + tip_span*2+25+ tip_width,
																	caipiaoyingkui_y + tip_height); 
//ͶעӶ��
static const int touzhuyongjin_y = 266;
static CRect rc_touzhuyongjin(tip_x+ tip_span, 
																	touzhuyongjin_y, 
																	tip_x + tip_span+ tip_width,
																	touzhuyongjin_y + tip_height); 

//����Ӷ��
static const int kuisunyongjin_y = 266;
static CRect rc_kuisunyongjin(tip_x+ tip_span*2, 
																	kuisunyongjin_y, 
																	tip_x + tip_span*2+ tip_width,
																	kuisunyongjin_y + tip_height); 

//���ջ
static const int jinrihuodong = 306;
static CRect rc_jinrihuodong(tip_x+ tip_span+25, 
																	jinrihuodong, 
																	tip_x+ tip_span+25+ tip_width,
																	jinrihuodong + tip_height); 
//����
static const int xianmaliang_y = 392;
static CRect rc_xianmaliang(tip_x + 37, 
																	xianmaliang_y, 
																	tip_x + tip_width,
																	xianmaliang_y + tip_height); 
//���Ʒ���
static const int qipaifandian_y = 392;
static CRect rc_qipaifandian(tip_x+tip_span*2 + 37, 
																	qipaifandian_y, 
																	tip_x+tip_span*2 + 37+ tip_width,
																	qipaifandian_y + tip_height); 
//����ӯ��
static const int qipaiyingkui_y = 392;
static CRect rc_qipaiyingkui(tip_x+tip_span + 37, 
																	qipaiyingkui_y, 
																	tip_x+tip_span  + 37+ tip_width,
																	qipaiyingkui_y + tip_height); 
//���Ʒ���
static const int myqipaifandian_y = 392;
static CRect rc_myqipaifandian(tip_x+tip_span*3 + 37, 
																	myqipaifandian_y, 
																	tip_x+tip_span*3  + 37+ tip_width,
																	myqipaifandian_y + tip_height); 
IMPLEMENT_DYNAMIC(CHuiYuanHyshjDlg, CDialog)

CHuiYuanHyshjDlg::CHuiYuanHyshjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanHyshjDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_tStart(CTime::GetCurrentTime())
	, m_tEnd(CTime::GetCurrentTime())
{
	m_bGetHuiyuanShj = false;
}

CHuiYuanHyshjDlg::~CHuiYuanHyshjDlg()
{
	if (m_bmpBk != NULL)
	{
		SafeDelete(m_bmpBk);
	}
}

void CHuiYuanHyshjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DATE_START, m_dateStart);
	DDX_Control(pDX, IDC_DATE_END, m_dateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_tStart);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_tEnd);

	DDX_Control(pDX, IDC_RIO_CHK_TODAY, m_rioChkToday);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
}

BEGIN_MESSAGE_MAP(CHuiYuanHyshjDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(IDC_RIO_CHK_TODAY, &CHuiYuanHyshjDlg::OnBnClickedBtnChkToday)
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanHyshjDlg::OnBnClickedBtnOk)
END_MESSAGE_MAP()

void CHuiYuanHyshjDlg::OnBnClickedBtnChkToday()
{
	m_bChkToday = !m_bChkToday;
	if(m_bChkToday)
	{
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();
		m_dateStart.SetTime(&m_tStart);
		m_dateEnd.SetTime(&m_tEnd);
		UpdateData();

		m_rioChkToday.SetPushed(true);
	}
	else
		m_rioChkToday.SetPushed(false);
	return;
}
void CHuiYuanHyshjDlg::OnBnClickedBtnOk()
{
	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("ѡ���ʱ�����䲻��"));
		return;
	}
	UpdateData();

	SendToServer(1);
	return;
}
void CHuiYuanHyshjDlg::AdjustCtrls()
{
	if (m_dateStart.GetSafeHwnd() != NULL)
	{
		m_dateStart.SetWindowPos(NULL, time_start_x, time_start_y, time_start_width, time_start_height, SWP_NOZORDER);
	}
	if (m_dateEnd.GetSafeHwnd() != NULL)
	{
		m_dateEnd.SetWindowPos(NULL, time_end_x, time_end_y, time_end_width, time_end_height, SWP_NOZORDER);
	}
	if (m_rioChkToday.GetSafeHwnd() != NULL)
	{
		m_rioChkToday.SetWindowPos(NULL, time_end_x+110, time_end_y, 71, 23, SWP_NOZORDER);
	}
	if (m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}

}
// CHuiYuanHyshjDlg ��Ϣ�������

void CHuiYuanHyshjDlg::OnPaint()
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

	cacheDC.DrawText(m_xiajirenshu, &rc_xiajirenshu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_zaixianrenshu, &rc_zaixianrenshu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_huiyuanyue, &rc_huiyuanyue, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinrizhuce, &rc_jinrizhuce, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinrifandian, &rc_jinrifandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_chedanzonge, &rc_chedanzonge, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinrichongzhi, &rc_jinrichongzhi, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinrichongzhisxf, &rc_jinrichongzhisxf, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_zongheyongjin, &rc_zongheyongjin, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinriqukuan, &rc_jinriqukuan, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinritouzhu, &rc_jinritouzhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinrihuodong, &rc_jinrihuodong, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_zhongjiangzonge, &rc_zhongjiangzonge, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_xinchongzhi, &rc_jinrixinchongzhi, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_zongheyingkui, &rc_zongheyingkui, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	//cacheDC.DrawText(m_wodefandian, &rc_wodefandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_caipiaoyingkui, &rc_caipiaoyingkui, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 	cacheDC.DrawText(m_touzhuyongjin, &rc_touzhuyongjin, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 	cacheDC.DrawText(m_kuisunyongjin, &rc_kuisunyongjin, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_xianmaliang, &rc_xianmaliang, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_qipaifandian, &rc_qipaifandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_qipaiyingkui, &rc_qipaiyingkui, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_myqipaifandian, &rc_myqipaifandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CHuiYuanHyshjDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanHyshjDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		SendToServer(1);
	}
	else
	{
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_dateStart.SetTime(&m_tStart);
		m_dateEnd.SetTime(&m_tEnd);

	}
}

BOOL CHuiYuanHyshjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk == NULL)
	{
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hysj_bg.png"));

		m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt.png"));

		m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	}
	m_rioChkToday.SetPushed(TRUE);
	m_bChkToday=true;

//	m_rioChkToday.SetPaintTextColor(RGB(98,88,84));
	AdjustCtrls();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CHuiYuanHyshjDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanHyshjDlg::OnOK()
{
	//CDialog::OnOK();
}


//��ȡ�¼�
bool CHuiYuanHyshjDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_HYSHJ_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetHyShjRet));
				if(wDataSize!=sizeof(CMD_GR_GetHyShjRet)) return false;

				CMD_GR_GetHyShjRet* pCountRet = (CMD_GR_GetHyShjRet*)pData;
				
				m_xiajirenshu.Format(_T("%d��"), pCountRet->n_t_xjrs);
				m_zaixianrenshu.Format(_T("%d��"), pCountRet->n_t_xjzx);
				m_huiyuanyue.Format(_T("%.3lf"), pCountRet->f_t_hyye);
				m_jinrifandian.Format(_T("%.3lf"), pCountRet->f_t_jrfd);
				m_jinrichongzhi.Format(_T("%.3lf"), pCountRet->f_t_jrchz);
				m_jinrichongzhisxf.Format(_T("%.3lf"), pCountRet->f_t_jrchz*pCountRet->n_t_qianfenlv/1000);
				m_jinriqukuan.Format(_T("%.3lf"), fabs(pCountRet->f_t_jrqk));
				m_jinritouzhu.Format(_T("%.3lf"), fabs(pCountRet->f_t_jrtzh));
				m_jinrihuodong.Format(_T("%.3lf"), fabs(pCountRet->f_wodefandian)/*fabs(pCountRet->f_t_jrhd)*/);
				m_zongheyingkui.Format(_T("%.3lf"),pCountRet->f_caipiao_yingkui+pCountRet->f_qp_yingkui+pCountRet->n_t_qianfenlv*pCountRet->f_t_jrchz/1000);
				m_xinchongzhi.Format(_T("%d��"),pCountRet->n_t_newchongzhi);
				m_jinrizhuce.Format(_T("%d��"),pCountRet->n_t_jinrizhuce);
				m_chedanzonge.Format(_T("%.3lf"), fabs(pCountRet->f_chedan_zonge));
				m_zhongjiangzonge.Format(_T("%.3lf"), fabs(pCountRet->f_zhongjiang_zonge));
				m_wodefandian.Format(_T("%.3lf"), fabs(pCountRet->f_wodefandian));
				m_caipiaoyingkui.Format(_T("%.3lf"),pCountRet->f_caipiao_yingkui+pCountRet->n_t_qianfenlv*pCountRet->f_t_jrchz/1000);
				m_xianmaliang.Format(_T("%.3lf"), fabs(pCountRet->f_t_xianma));
				m_qipaifandian.Format(_T("%.3lf"), fabs(pCountRet->f_qp_fandian));
				m_qipaiyingkui.Format(_T("%.3lf"), pCountRet->f_qp_yingkui);
				m_myqipaifandian.Format(_T("%.3lf"), fabs(pCountRet->f_my_qp_fandian));
				m_touzhuyongjin.Format(_T("%.3lf"), fabs(pCountRet->f_touzhuyongjin));
				m_kuisunyongjin.Format(_T("%.3lf"), fabs(pCountRet->f_kuisunyongjin));
				m_zongheyongjin.Format(_T("%.3lf"),pCountRet->f_touzhuyongjin+pCountRet->f_kuisunyongjin);
			}

		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	return true;
	//�������
	ASSERT(FALSE);

	return true;
}

VOID CHuiYuanHyshjDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //��ע��¼����
	{
		m_bGetHuiyuanShj = true;
		if(m_bGetHuiyuanShj)
		{
			m_bGetHuiyuanShj = false;
			CMD_GP_GetHyShj GetHyShj;
			ZeroMemory(&GetHyShj,sizeof(GetHyShj));

			GetHyShj.dwUserID = theAccount.user_id;
			GetHyShj.cbOnlyToday = (m_bChkToday)?1:0;

			CopyMemory(GetHyShj.szTimeStart,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetHyShj.szTimeStart));
			CopyMemory(GetHyShj.szTimeEnd,m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetHyShj.szTimeEnd));

			if(m_MissionManager!=NULL)
				m_MissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_HYSHJ,&GetHyShj,sizeof(GetHyShj));
			return;
		}

	}


}