// HuodongZhuanzhuan.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GamePlaza.h"
#include "HuodongZhuanzhuan.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
// HuodongZhuanzhuan �Ի���
#define IDC_WEB_PUBLICIZE  100001

IMPLEMENT_DYNAMIC(HuodongZhuanzhuan, CDialog)

HuodongZhuanzhuan::HuodongZhuanzhuan(CWnd* pParent /*=NULL*/)
	: CDialog(HuodongZhuanzhuan::IDD, pParent)
{

	m_bPrized=false;
	m_bCanUserLuckyzhuanZhuan=false;
	m_bGetUserLuckyZhuan=false;
}

HuodongZhuanzhuan::~HuodongZhuanzhuan()
{
	if (m_bmpBk != NULL)
	{
//		SafeDelete( m_bmpBk);
	}
	m_font.DeleteObject();
}

void HuodongZhuanzhuan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_LUCKY, m_btnChouJiang);
}


BEGIN_MESSAGE_MAP(HuodongZhuanzhuan, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_LUCKY, &HuodongZhuanzhuan::OnBnClickedBtChouJiang)
	
END_MESSAGE_MAP()


// HuodongZhuanzhuan ��Ϣ�������

BOOL HuodongZhuanzhuan::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bmpBk == NULL)
	{
		m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\hd_zhuanzhuan.png"));
		CRect rcCreate(0,0,0,0);
		CRect rect;
		GetClientRect(&rect);
		m_PublicizeWeb.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);
		m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	}
	m_btnChouJiang.SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_zhuanzhuan.png"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void HuodongZhuanzhuan::OnPaint()
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


	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void HuodongZhuanzhuan::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}
	int nX = (cx-m_btnChouJiang.Width())/2+59;
	int nY = (cy-m_btnChouJiang.Height())/2-19;

	if (m_btnChouJiang.GetSafeHwnd() != NULL)
	{
		m_btnChouJiang.SetWindowPos(NULL, nX, nY, m_btnChouJiang.Width(), m_btnChouJiang.Height(), SWP_NOZORDER);
	}

	if(m_PublicizeWeb.GetSafeHwnd() != NULL)
	{
		m_PublicizeWeb.SetWindowPos(NULL,0,0,780,576,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	}

}

void HuodongZhuanzhuan::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;


		m_btnChouJiang.EnableWindow(FALSE);

 		TCHAR szWeb[126]=TEXT("");
 		_sntprintf(szWeb,CountArray(szWeb),TEXT("http://zhuanpan.google021.com/Roulette/Default.aspx?user_id=%d"),theAccount.user_id);
 
 		m_PublicizeWeb.Navigate(szWeb);

		//��ѯȨ��
		//SendToServer(3);
	}
}

void HuodongZhuanzhuan::OnBnClickedBtChouJiang()
{
	if(!m_bPrized)
	{
		SendToServer(1);
		m_bPrized=true;
	}
 	else
 	{
 		m_bPrized=true;
 		MyMessageBox(L"�������Ѿ�������˰ɣ�����������", L"��ܰ��ʾ");
 	}
}


void HuodongZhuanzhuan::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}

void HuodongZhuanzhuan::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
}

//��ȡ�¼�
bool HuodongZhuanzhuan::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_LUCKY_ZHUAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLuckyZhuanRet));
				if(wDataSize!=sizeof(CMD_GP_GetLuckyZhuanRet)) return false;

				CMD_GP_GetLuckyZhuanRet* pCountRet = (CMD_GP_GetLuckyZhuanRet*)pData;

				int res = pCountRet->n_t_got;

				CString strPath;  //�ļ�·��
				GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
				strPath.ReleaseBuffer(); 
				int pos = strPath.ReverseFind('\\'); 
				strPath = strPath.Left(pos); 
				CString strWeb;
				strWeb.Format(_T("Roulette\\Default.aspx?user_id=%d"),theAccount.user_id);

				strPath+=strWeb;
				// 	m_plogo = new CWebBrowserLogo;
				// 	m_plogo->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,20,NULL);
				m_PublicizeWeb.Navigate(strPath);

// 				TCHAR szWeb[126]=TEXT("");
// 				_sntprintf(szWeb,CountArray(szWeb),TEXT("http://pay.szhuis.cn/Roulette/Default.aspx?user_id=%d"),theAccount.user_id);

			//	m_PublicizeWeb.Navigate(szWeb);

				// 				if(res == 0)
// 				{
// 					DOUBLE dPrize = pCountRet->f_t_jine;
// 					CString strMsg;
// 					strMsg.Format(L"��ϲ�����н��ˣ���ý���%.2f", dPrize);
// 					const double EPSINON = 0.00001;
// 					if((dPrize >= - EPSINON) && (dPrize <= EPSINON))
// 					{
// 						strMsg.Format(L"лл�ݹˣ�", dPrize);
// 					}
// 
// 					m_btnChouJiang.EnableWindow(false);
// 					MyMessageBox(strMsg, L"��ϲ����");
// 
// 				}
// 				else
// 				{
// 					m_bPrized=true;
// 					MyMessageBox(L"�������Ѿ�������˰ɣ�����������", L"��ܰ��ʾ");
// 				}

				break;
			}
		case SUB_GP_GET_LAST_YUE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLastYueRet));
				if(wDataSize!=sizeof(CMD_GP_GetLastYueRet)) return false;

				CMD_GP_GetLastYueRet* pCountRet = (CMD_GP_GetLastYueRet*)pData;

				theAccount.yue = pCountRet->f_t_yue;
				theAccount.dongjie = pCountRet->f_t_dongjie;
				// 				InvalidateRect(rc_jinrixiaofei);
				// 				InvalidateRect(rc_zhanghuyue);
				break;
			}

		case SUB_GP_CAN_LUCKY_ZHUAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_CanLuckyZhuanRet));
				if(wDataSize!=sizeof(CMD_GP_CanLuckyZhuanRet)) return false;

				CMD_GP_CanLuckyZhuanRet* pCountRet = (CMD_GP_CanLuckyZhuanRet*)pData;

				
				DOUBLE dXiaofei = pCountRet->f_t_xiaofei;

// 				if(dXiaofei < 888)
// 					m_btnChouJiang.EnableWindow(FALSE);
// 				else
					m_btnChouJiang.EnableWindow(TRUE);


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

VOID HuodongZhuanzhuan::SendToServer(int nSendType)
{

	if(nSendType == 1)   //��ע��¼����
	{
		m_bGetUserLuckyZhuan = true;
		if(m_bGetUserLuckyZhuan)
		{
			m_bGetUserLuckyZhuan = false;
			CMD_GP_GetLuckyZhuan GetLuckyZhuan;
			ZeroMemory(&GetLuckyZhuan,sizeof(GetLuckyZhuan));

			GetLuckyZhuan.dwUserID = theAccount.user_id;
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_LUCKY_ZHUAN,&GetLuckyZhuan,sizeof(GetLuckyZhuan));
			return;
		}



	}

	if(nSendType == 2)
	{
		return;
	}

	if(nSendType == 3)
	{
		m_bCanUserLuckyzhuanZhuan = true;
		if(m_bCanUserLuckyzhuanZhuan)
		{
			m_bCanUserLuckyzhuanZhuan = false;
			CMD_GP_CanLuckyZhuan GetCanLuckyZhuan;
			ZeroMemory(&GetCanLuckyZhuan,sizeof(GetCanLuckyZhuan));

			GetCanLuckyZhuan.dwUserID = theAccount.user_id;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CAN_LUCKY_ZHUAN,&GetCanLuckyZhuan,sizeof(GetCanLuckyZhuan));
			return;

		}
	}
}