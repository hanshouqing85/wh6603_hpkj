#include "stdafx.h"
#include "GamePlaza.h"
#include "HuoDongHuanLeLingDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"

const static int xiaofeie_x = 266;
const static int xiaofeie_y = 328;
const static int xiaofeie_width = 140;
const static int xiaofeie_height = 20;
CRect rc_xiaofeie(xiaofeie_x,
				  xiaofeie_y,
				  xiaofeie_x + xiaofeie_width,
				  xiaofeie_y + xiaofeie_height);

const static int zuidijiangli_x = 505;
const static int zuidijiangli_y = 174;
const static int zuidijiangli_width = 140;
const static int zuidijiangli_height = 20;
CRect rc_zuidijiangli(zuidijiangli_x,
				  zuidijiangli_y,
				  zuidijiangli_x + zuidijiangli_width,
				  zuidijiangli_y + zuidijiangli_height);

const static int cishu_x = 485;
const static int cishu_y = 304;
const static int cishu_width = 20;
const static int cishu_height = 20;
CRect rc_cishu(cishu_x,
			  cishu_y,
			  cishu_x + cishu_width,
			  cishu_y + cishu_height);


const static int jilu_x = 225;
const static int jilu_y = 304;
const static int jilu_width = 200;
const static int jilu_height = 20;
CRect rc_jilu(jilu_x,
			   jilu_y,
			   jilu_x + jilu_width,
			   jilu_y + jilu_height);

const static int mubiao_x = 510;
const static int mubiao_y = 306;
const static int mubiao_width = 46;
const static int mubiao_height = 20;
CRect rc_mubiao(mubiao_x,
			  mubiao_y,
			  mubiao_x + mubiao_width,
			  mubiao_y + mubiao_height);

//ǩ�����콱��ť
const static int btn_lingjiang_x = 59;
const static int btn_lingjiang_y = 273;

#define IDC_QIANDAO_DAY	100
// CHuoDongHuanLeSongDlg �Ի���

IMPLEMENT_DYNAMIC(CHuoDongHuanLeSongDlg, CDialog)

CHuoDongHuanLeSongDlg::CHuoDongHuanLeSongDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuoDongHuanLeSongDlg::IDD, pParent)
	, m_bmpBk(NULL)
{
	m_dJinriXiafei=0;			//��������
	m_dWinPrize=0;				//�н����		
	m_nSignedCount=0;			//ǩ������
	m_XXCount=0;				//ʣ��ǩ������

	m_bUserQianDao=false;
	m_bGetUserHuanlesongInfo=false;
	m_bUserHuanlesongLingjiang=false;
}

CHuoDongHuanLeSongDlg::~CHuoDongHuanLeSongDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
	m_font.DeleteObject();
}

void CHuoDongHuanLeSongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_QIANDAO,	m_btnQiandao);
	DDX_Control(pDX, IDC_BTN_LINGJIANG, m_btnLingJiang);
	
}


BEGIN_MESSAGE_MAP(CHuoDongHuanLeSongDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_QIANDAO, &CHuoDongHuanLeSongDlg::OnBnClickedBtnQiandao)
	ON_BN_CLICKED(IDC_BTN_LINGJIANG, &CHuoDongHuanLeSongDlg::OnBnClickedBtnLingjiang)


	ON_BN_CLICKED(IDC_QIANDAO_DAY, &CHuoDongHuanLeSongDlg::OnBnClickedBtnQiandao)
	ON_BN_CLICKED(IDC_QIANDAO_DAY+1, &CHuoDongHuanLeSongDlg::OnBnClickedBtnQiandao)
	ON_BN_CLICKED(IDC_QIANDAO_DAY+2, &CHuoDongHuanLeSongDlg::OnBnClickedBtnQiandao)
	ON_BN_CLICKED(IDC_QIANDAO_DAY+3, &CHuoDongHuanLeSongDlg::OnBnClickedBtnQiandao)
	ON_BN_CLICKED(IDC_QIANDAO_DAY+4, &CHuoDongHuanLeSongDlg::OnBnClickedBtnQiandao)
	ON_BN_CLICKED(IDC_QIANDAO_DAY+5, &CHuoDongHuanLeSongDlg::OnBnClickedBtnQiandao)
	ON_BN_CLICKED(IDC_QIANDAO_DAY+6, &CHuoDongHuanLeSongDlg::OnBnClickedBtnQiandao)
END_MESSAGE_MAP()


// CHuoDongHuanLeSongDlg ��Ϣ�������

BOOL CHuoDongHuanLeSongDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk == NULL)
	{
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\hd_huanlesong.png"));

		m_font.CreateFont(22, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	}
	CRect rcCreate(0,0,0,0);
	for(int i = 0;i < CountArray(m_btnQiandaoDay);i++)
	{
		m_btnQiandaoDay[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_QIANDAO_DAY+i);
		m_btnQiandaoDay[i].SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_lingjiang4.png"));
		m_btnQiandaoDay[i].EnableWindow(false);
	}
	m_btnQiandao.SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_lingjiang5.png"));
	m_btnLingJiang.SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_lingjiang.png"));

	AdjustLingjiang();
	m_btnQiandao.EnableWindow(FALSE);
	m_btnLingJiang.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CHuoDongHuanLeSongDlg::AdjustLingjiang()
{
	int n_space_x = 0;
	int n_space_y = 0;

	n_space_x =  133;
	n_space_y =  0;
	int nWidth = 83;
	int nHeight = 20;
	if(m_btnQiandaoDay[0].GetSafeHwnd() != NULL)
	{
		m_btnQiandaoDay[0].SetWindowPos(NULL, btn_lingjiang_x, btn_lingjiang_y, nWidth, nHeight, SWP_NOZORDER);
	}
	if(m_btnQiandaoDay[1].GetSafeHwnd() != NULL)
	{
		m_btnQiandaoDay[1].SetWindowPos(NULL, btn_lingjiang_x+n_space_x, btn_lingjiang_y,  nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQiandaoDay[2].GetSafeHwnd() != NULL)
	{
		m_btnQiandaoDay[2].SetWindowPos(NULL, btn_lingjiang_x+n_space_x*2, btn_lingjiang_y,  nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQiandaoDay[3].GetSafeHwnd() != NULL)
	{
		m_btnQiandaoDay[3].SetWindowPos(NULL, btn_lingjiang_x+n_space_x*3, btn_lingjiang_y,  nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQiandaoDay[4].GetSafeHwnd() != NULL)
	{
		m_btnQiandaoDay[4].SetWindowPos(NULL, btn_lingjiang_x+n_space_x*4, btn_lingjiang_y+n_space_y, nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQiandaoDay[5].GetSafeHwnd() != NULL)
	{
		m_btnQiandaoDay[5].SetWindowPos(NULL, btn_lingjiang_x+n_space_x*5, btn_lingjiang_y+n_space_y, nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQiandaoDay[6].GetSafeHwnd() != NULL)
	{
		m_btnQiandaoDay[6].SetWindowPos(NULL, btn_lingjiang_x+n_space_x*6, btn_lingjiang_y+n_space_y,nWidth,nHeight, SWP_NOZORDER);
	}

}
void CHuoDongHuanLeSongDlg::OnPaint()
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

//cacheDC.Rectangle(&rc_xiaofeie);
//cacheDC.Rectangle(&rc_zuidijiangli);
//cacheDC.Rectangle(&rc_cishu);
//cacheDC.Rectangle(&rc_jilu);
	//cacheDC.Rectangle(&rc_mubiao);


	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(160, 8, 8));

	CString strText;
	strText.Format(L"%.2f", m_dJinriXiafei);
	cacheDC.DrawText(strText, rc_xiaofeie, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CRect rcZongQiandao;

	rcZongQiandao.CopyRect(rc_xiaofeie);

	rcZongQiandao.left += 381;
	rcZongQiandao.right += 381;
	strText.Format(L"%.2f", m_dWinPrize);
	cacheDC.DrawText(strText, rcZongQiandao, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

// 	strText.Format(L"%.2f", m_dWinPrize);
// 	cacheDC.DrawText(strText, rc_zuidijiangli, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 	
// 	strText.Format(L"%d", m_nSignedCount);
// 	cacheDC.DrawText(strText, rc_jilu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 	strText.Format(L"%.d", m_XXCount);
// 	cacheDC.DrawText(strText, rc_mubiao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
// 
	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CHuoDongHuanLeSongDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_btnQiandao.GetSafeHwnd() != NULL)
	{
		m_btnQiandao.SetWindowPos(NULL, btn_lingjiang_x, btn_lingjiang_y, m_btnQiandao.Width(), m_btnQiandao.Height(), SWP_NOZORDER);
		m_btnQiandao.ShowWindow(SW_HIDE);
	}

	if (m_btnLingJiang.GetSafeHwnd() != NULL)
	{
		m_btnLingJiang.SetWindowPos(NULL, btn_lingjiang_x + 200, btn_lingjiang_y, m_btnLingJiang.Width(), m_btnLingJiang.Height(), SWP_NOZORDER);
		m_btnLingJiang.ShowWindow(SW_HIDE);
	}
}

//ǩ��
void CHuoDongHuanLeSongDlg::OnBnClickedBtnQiandao()
{
	SendToServer(1);
}

//�콱
void CHuoDongHuanLeSongDlg::OnBnClickedBtnLingjiang()
{
	double nPrize=0;
	SendToServer(3);
	//a'long ��ʱ����
// 	if(theDBCmd.UserHuanlesongLingjiang(theAccount.user_id, nPrize))
// 	{
// 		CString strMsg;
// 		strMsg.Format(L"��ϲ�����н��ˣ���ý���%.2f", nPrize);
// 		MyMessageBox(strMsg, L"��ϲ����");
// 	}
// 	else
// 	{
// 		CString strMsg;
// 		strMsg.Format(L"����û�������콱�ʸ����´�����");
// 		MyMessageBox(strMsg, L"��ϲ����");
// 	}
}

void	CHuoDongHuanLeSongDlg::GetUserHuanlesongInfo()
{
	SendToServer(2);

}

void CHuoDongHuanLeSongDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	for(int i = 0;i < 7;i++)
	{
		m_btnQiandaoDay[i].SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_lingjiang4.png"));
		m_btnQiandaoDay[i].EnableWindow(FALSE);
	}
	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		//��ѯ��������Ϣ
		GetUserHuanlesongInfo();
	}
}

void CHuoDongHuanLeSongDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuoDongHuanLeSongDlg::OnOK()
{
	//CDialog::OnOK();
}

//��ȡ�¼�
bool CHuoDongHuanLeSongDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_USER_QIANDAO_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_UserQiandaoRet));
				if(wDataSize!=sizeof(CMD_GP_UserQiandaoRet)) return false;

				CMD_GP_UserQiandaoRet* pCountRet = (CMD_GP_UserQiandaoRet*)pData;

				int nRes = pCountRet->n_t_res;

				if(nRes == 0)
				{
					GetUserHuanlesongInfo();
					Invalidate(false);
					CString strMsg;
					strMsg.Format(L"������ǩ���ɹ���лл����");
					m_btnQiandao.EnableWindow(FALSE);
					MyMessageBox(strMsg, L"ף������");
				}
				else if(nRes == 1)
				{
					CString strMsg;
					strMsg.Format(L"�������Ѿ�ǩ���ˣ������ظ�ǩ��");
					MyMessageBox(strMsg, L"ף������");
				}
				else if(nRes == 2)
				{
					CString strMsg;
					strMsg.Format(L"����û�дﵽǩ���ʸ������Ͷע�ɣ�");
					MyMessageBox(strMsg, L"ף������");

				}
				else if(nRes == 3 )
				{
					GetUserHuanlesongInfo();
					Invalidate(false);
					CString strMsg;
					strMsg.Format(L"��������ǩ��7�죬ϵͳ�Զ����Ž�������˻�������飡");
					//m_btnQiandao.EnableWindow(FALSE);
					MyMessageBox(strMsg, L"ף������");

				}


				break;
			}
		case SUB_GP_GET_USER_HUANLESONG_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetUserHuanlesongRet));
				if(wDataSize!=sizeof(CMD_GP_GetUserHuanlesongRet)) return false;

				CMD_GP_GetUserHuanlesongRet* pCountRet = (CMD_GP_GetUserHuanlesongRet*)pData;


				m_dJinriXiafei = pCountRet->f_t_xiaofei;
				m_dWinPrize = pCountRet->f_t_prize;
				m_nSignedCount = pCountRet->n_t_count;
				m_XXCount = pCountRet->n_t_XXcount;

// 				if(m_nSignedCount>6)
// 					m_nSignedCount=0;
				int nQiandaoCount = 0;
				nQiandaoCount = min(m_nSignedCount,7);
				for(int i = 0;i < nQiandaoCount;i++)
				{
					m_btnQiandaoDay[i].SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_lingjiang5.png"));
					m_btnQiandaoDay[i].EnableWindow(FALSE);
				}

				if(pCountRet->nRes == 0 && nQiandaoCount<7)
				{
					m_btnQiandaoDay[nQiandaoCount].SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_lingjiang5.png"));
					m_btnQiandaoDay[nQiandaoCount].EnableWindow(TRUE);
				}


//  				if(m_dJinriXiafei < m_dWinPrize)
//  					m_btnQiandao.EnableWindow(false);
// 				else 
// 					m_btnQiandao.EnableWindow(true);
// 
// 				if(m_nSignedCount < 10 && m_XXCount > 0)
// 					m_btnLingJiang.EnableWindow(FALSE);
// 				else
// 					m_btnLingJiang.EnableWindow(TRUE);


				break;
			}

		case SUB_GP_GET_LING_JIANG_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLingJiangRet));
				if(wDataSize!=sizeof(CMD_GP_GetLingJiangRet)) return false;

				CMD_GP_GetLingJiangRet* pCountRet = (CMD_GP_GetLingJiangRet*)pData;

				int nRes = pCountRet->n_t_got;

				if(nRes == 0)
				{
					CString strMsg;
					strMsg.Format(L"��ϲ�����н��ˣ���ý���%.2f", pCountRet->f_t_jine);
					MyMessageBox(strMsg, L"��ϲ����");
				}
				else
				{
					CString strMsg;
					strMsg.Format(L"����û�������콱�ʸ����´�����");
					MyMessageBox(strMsg, L"��ϲ����");
				}

				break;

			}

		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	return true;

	//�������
	ASSERT(FALSE);

	return true;
}

VOID CHuoDongHuanLeSongDlg::SendToServer(int nSendType)
{

	if(nSendType == 1)   //��ע��¼����
	{
		m_bUserQianDao = true;
		if(m_bUserQianDao)
		{
			m_bUserQianDao = false;
			CMD_GP_UserQiandao UserQiandao;
			ZeroMemory(&UserQiandao,sizeof(UserQiandao));

			UserQiandao.dwUserID = theAccount.user_id;
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_QIANDAO,&UserQiandao,sizeof(UserQiandao));
			return;
		}


	}

	if(nSendType == 2)
	{
		m_bGetUserHuanlesongInfo = true;

		if(m_bGetUserHuanlesongInfo)
		{
			m_bGetUserHuanlesongInfo = true;
			CMD_GP_GetUserHuanlesong GetHuanlesong;
			ZeroMemory(&GetHuanlesong,sizeof(GetHuanlesong));

			GetHuanlesong.dwUserID = theAccount.user_id;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_HUANLESONG,&GetHuanlesong,sizeof(GetHuanlesong));
			return;
		}

	}

	if(nSendType == 3)
	{
		m_bUserHuanlesongLingjiang = true;
		if(m_bUserHuanlesongLingjiang)
		{
			m_bUserHuanlesongLingjiang = false;
			CMD_GP_GetLingJiang GetLingjiang;
			ZeroMemory(&GetLingjiang,sizeof(GetLingjiang));

			GetLingjiang.dwUserID = theAccount.user_id;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_LING_JIANG,&GetLingjiang,sizeof(GetLingjiang));
			return;

		}

	}



}