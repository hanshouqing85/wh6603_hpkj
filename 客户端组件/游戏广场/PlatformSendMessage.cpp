#include "stdafx.h"
#include "GamePlaza.h"
#include "PlatformSendMessage.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"

#define IDC_QIANDAO_DAY	100
// CPlatformSendMessage 对话框

static int rev_x = 202;
static int rev_y = 91;
static int rev_cx = 532;
static int rev_cy = 26;

static int shangji_x = 210;
static int shangji_y = 54;

static int rev_content_cy = 222;
IMPLEMENT_DYNAMIC(CPlatformSendMessage, CDialog)
map<int,CString> mapXiaji;
CPlatformSendMessage::CPlatformSendMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CPlatformSendMessage::IDD, pParent)
	, m_bmpBk(NULL)
{
	m_font.CreateFont(22, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	m_dJinriXiafei=0;			//今日消费
	m_dWinPrize=0;				//中奖金额		
	m_nSignedCount=0;			//签到次数
	m_XXCount=0;				//剩余签到次数
	m_nSign = 0;
	m_bUserQianDao=false;
	m_bGetUserHuanlesongInfo=false;
	m_bUserHuanlesongLingjiang=false;
	mapRevUserID.clear();
	mapIndex.clear();
}

CPlatformSendMessage::~CPlatformSendMessage()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
	m_font.DeleteObject();
}

void CPlatformSendMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REV_ACT, m_edRevAct);
	DDX_Control(pDX, IDC_REV_TITLE, m_edRevTitle);
	DDX_Control(pDX, IDC_RICHEDIT_CONTENT, m_richContent);
	DDX_Control(pDX, IDC_BTN_SENDMESSAGE, m_btnSendMessage);
	DDX_Control(pDX, IDC_BTN_SHANGJI, m_btnShangji);
	DDX_Control(pDX, IDC_BTN_XIAJI, m_btnXiaji);
	DDX_Control(pDX, IDC_BTN_KEFU, m_btnKefu);
}


BEGIN_MESSAGE_MAP(CPlatformSendMessage, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SHANGJI, &CPlatformSendMessage::OnBnClickedBtnSj)
	ON_BN_CLICKED(IDC_BTN_XIAJI, &CPlatformSendMessage::OnBnClickedBtnXj)
	ON_BN_CLICKED(IDC_BTN_KEFU, &CPlatformSendMessage::OnBnClickedBtnKeFu)
	ON_EN_CHANGE(IDC_REV_TITLE, &CPlatformSendMessage::OnEnChangeEditTitle)
	ON_BN_CLICKED(IDC_BTN_SENDMESSAGE, &CPlatformSendMessage::OnBnClickedBtnSendMessage)

	ON_MESSAGE(IDM_NEXT_PAGE,OnTurnPage)
END_MESSAGE_MAP()


// CPlatformSendMessage 消息处理程序

BOOL CPlatformSendMessage::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_nPage = 1;
	m_nMaxPage = 1;
	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\Message\\xinxinxi_bj.png"));


	m_edRevAct.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_edRevTitle.SetEnableColor(RGB(56,90,154),RGB(241,233,255),RGB(241,233,255));
	m_richContent.SetBackgroundColor(false,RGB(241,233,255));
	m_nSearchType = 0;
	m_dlgSelectRevAccount.Create(CSelectRevAccount::IDD,this);
	m_dlgSelectRevAccount.ShowWindow(SW_HIDE);
	m_edRevTitle.LimitText(50);
	m_edRevAct.SetReadOnly(true);
//	m_edRevAct.EnableWindow(false);
	m_edRevAct.SetFont(&m_font);
	m_edRevTitle.SetFont(&m_font);
	m_richContent.SetFont(&m_font);
	m_btnSendMessage.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\fasong_bt.png"));
	m_btnShangji.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\shangji_bt.png"));
	m_btnXiaji.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\xiaji_bt.png"));
	m_btnKefu.SetImage(CBmpUtil::GetExePath() + _T("skin\\Message\\kefu_bt.png"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
// BOOL CPlatformSendMessage::OnCommand(WPARAM wParam, LPARAM lParam)
// {
// 	//变量定义
// 	UINT nCommandID=LOWORD(wParam);
// 	if(nCommandID==IDC_REV_ACT&&m_cbSendType==2)
// 	{
// 		OnBnClickedBtnXj();
// 	}
// 	return  __super::OnCommand(wParam,lParam);;
// }
BOOL CPlatformSendMessage::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if (pMsg->hwnd == m_edRevAct.m_hWnd)
	{
		if(pMsg->message==WM_LBUTTONDBLCLK)
		{
			OnBnClickedBtnXj();
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);

}
void CPlatformSendMessage::OnPaint()
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

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

void CPlatformSendMessage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	if(m_edRevAct.GetSafeHwnd()!=NULL)
	{
		m_edRevAct.SetWindowPos(NULL,rev_x,rev_y,rev_cx,rev_cy, SWP_NOZORDER);
	}
	if(m_edRevTitle.GetSafeHwnd()!=NULL)
	{
		m_edRevTitle.SetWindowPos(NULL,rev_x,rev_y+36,rev_cx,rev_cy, SWP_NOZORDER);
	}
	if(m_richContent.GetSafeHwnd()!=NULL)
	{
		m_richContent.SetWindowPos(NULL,rev_x,rev_y+72,rev_cx,rev_content_cy, SWP_NOZORDER);
	}
	if(m_btnSendMessage.GetSafeHwnd()!=NULL)
	{
		m_btnSendMessage.SetWindowPos(NULL,rev_x+430,rev_y+rev_content_cy+72+8,m_btnSendMessage.Width(),m_btnSendMessage.Height(), SWP_NOZORDER);
	}
	if(m_btnShangji.GetSafeHwnd()!=NULL)
	{
		m_btnShangji.SetWindowPos(NULL,shangji_x,shangji_y,m_btnShangji.Width(),m_btnShangji.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaji.GetSafeHwnd()!=NULL)
	{
		m_btnXiaji.SetWindowPos(NULL,shangji_x+70,shangji_y,m_btnXiaji.Width(),m_btnXiaji.Height(), SWP_NOZORDER);
	}
	if(m_btnKefu.GetSafeHwnd()!=NULL)
	{
		m_btnKefu.SetWindowPos(NULL,shangji_x+140,shangji_y,m_btnKefu.Width(),m_btnKefu.Height(), SWP_NOZORDER);
	}
}
void CPlatformSendMessage::SearchUser()
{
	m_nSearchType = 1;
	SendToServer(1);
}

void CPlatformSendMessage::SetSendUser(CString strName,int nUserID,CString strTitle)
{
	mapRevUserID.clear();
	mapRevUserID[nUserID] = strName;
	m_strRevName = strName;
	m_nRevUserID = nUserID;

	CString strTemp;
	if(strName == _T("上级"))
		strTemp.Format(L"%s;",strName);
	else
		strTemp.Format(L"%s(%d);",strName,nUserID);
	m_edRevAct.SetWindowText(strTemp);

	strTemp.Format(L"Re:%s",strTitle);
	m_edRevTitle.SetWindowText(strTemp);
}
//上级
void CPlatformSendMessage::OnBnClickedBtnSj()
{
	mapRevUserID.clear();
	m_cbSendType = 1;
	SendToServer(1);
}
//下级
void CPlatformSendMessage::OnBnClickedBtnXj()
{
	if(m_cbSendType!=2)
	{
		mapRevUserID.clear();
		m_edRevAct.SetWindowText(L"");

	}
	m_cbSendType = 2;
	m_dlgSelectRevAccount.ShowWindow(SW_SHOW);
	m_nPage = 1;
	m_dlgSelectRevAccount.HideAllButton();

	SendToServer(1);
}
//客服
void CPlatformSendMessage::OnBnClickedBtnKeFu()
{
	mapRevUserID.clear();
	m_edRevAct.SetWindowText(L"客服");
	CString strName;
	strName.Format(L"客服");

	mapRevUserID[1] = strName;
 	m_cbSendType = 3;
// 	SendToServer(1);
}
LRESULT CPlatformSendMessage::OnTurnPage(WPARAM wParam, LPARAM lParam)
{
	m_dlgSelectRevAccount.HideAllButton();
	m_nPage = (int)wParam;
	SendToServer(1);
	return 1;
}
//下级
void CPlatformSendMessage::OnBnClickedBtnSendMessage()
{
	CString strTmp;
	m_edRevAct.GetWindowText(strTmp);
	if(strTmp.IsEmpty())
	{
		MyMessageBox(L"请添加收件人！");
		return;
	}
	m_edRevTitle.GetWindowText(strTmp);
	if(strTmp.IsEmpty())
	{
		MyMessageBox(L"请添加主题！");
		return;
	}
	m_richContent.GetWindowText(strTmp);
	if(strTmp.IsEmpty())
	{
		MyMessageBox(L"请添加内容！");
		return;
	}
	SendToServer(2);
}
void CPlatformSendMessage::OnEnChangeEditTitle()
{
	return;
}

void CPlatformSendMessage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	m_edRevAct.SetWindowText(L"");
	m_edRevTitle.SetWindowText(L"");
	m_richContent.SetWindowText(L"");
	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

	}
}

void CPlatformSendMessage::OnCancel()
{
	//CDialog::OnCancel();
}

void CPlatformSendMessage::OnOK()
{
	//CDialog::OnOK();
}

//读取事件
bool CPlatformSendMessage::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_SEND_MESSAGE_RET:
			{
				ASSERT((wDataSize == sizeof(CMD_GP_SendMessageRet)));
				if((wDataSize != sizeof(CMD_GP_SendMessageRet)) )return false;

				CMD_GP_SendMessageRet* pSendMessageRet = (CMD_GP_SendMessageRet*)pData;
				mapRevUserID.clear();
				if(m_nSign == pSendMessageRet->n_t_sign)
				{
					m_nSign++;

					CString strMessage;
					if(pSendMessageRet->n_t_retid==1)
					{
						strMessage.Format(L"发送成功 ！");
						MyMessageBox(strMessage);

						CWnd* pParent = GetParent();
						if(pParent!=NULL)
							pParent->SendMessage(IDM_GO_OUTBOX,0,0);
					}
					else if(pSendMessageRet->n_t_retid==2)
					{
						strMessage.Format(L"30分钟内不能重复发送相同内容 ！");
						MyMessageBox(strMessage);
					}
					else
					{
						strMessage.Format(L"发送失败 ！");
						MyMessageBox(strMessage);
					}
				}


				return true;
			}
		case SUB_GP_GET_ALL_USERINFO_RET:
			{
				ASSERT((wDataSize % sizeof(CMD_GP_GetUserInfoRet))==0);
				if((wDataSize % sizeof(CMD_GP_GetUserInfoRet))!=0 )return false;

				int nCount = wDataSize/sizeof(CMD_GP_GetUserInfoRet);
				
				mapXiaji.clear();
				for (int i = 0;i < nCount;i++)
				{
					CMD_GP_GetUserInfoRet* pLogRet = ((CMD_GP_GetUserInfoRet*)pData+i);
					
					m_nMaxPage = pLogRet->n_t_count/30+1;
					if(m_cbSendType == 1 && pLogRet->n_t_type == 2)
					{
						CString strAct;
						strAct.Format(L"上级;");
						m_edRevAct.SetWindowText(strAct);

						mapRevUserID[pLogRet->n_t_userid] = strAct;
					}
					else if(m_cbSendType == 2 && pLogRet->n_t_type == 1)
					{
						CString strName;
						strName.Format(L"%s",pLogRet->s_t_name);
						mapXiaji[pLogRet->n_t_userid] = strName;
					}
				}

				if(m_cbSendType == 2)
				{
				//	CSelectRevAccount dlgSelectRevAccount;
					int nCount = mapXiaji.size();

					map<int ,CString>::iterator pos = mapXiaji.begin();
					nCount = min(nCount,30);
					for (int i = 0;i < nCount && pos != mapXiaji.end();i++)
					{
						CString strName;
						CString strAct;
						strAct = pos->second;
						strName.Format(L"%s(%d)",pos->second,pos->first);
						for (map<int ,CString>::iterator posR = mapRevUserID.begin();posR!=mapRevUserID.end();++posR)
						{
							if(posR->first == pos->first )
							{
								m_dlgSelectRevAccount.SetPushed(i);
							}
						}
						
						m_dlgSelectRevAccount.SetButtonText(i,strName,m_nPage,m_nMaxPage);
						++pos;
					}

// 					for (map<int,int>::iterator PosIndex = mapIndex.begin();PosIndex!=mapIndex.end();PosIndex++)
// 					{
// 						m_dlgSelectRevAccount.SetPushed(PosIndex->second);
// 					}

// 					if(dlgSelectRevAccount.DoModal() == IDOK)
// 					{
// 						mapRevUserID.clear();
// 						int nIndex = 0 ;
// 						pos = mapXiaji.begin();
// 						for (int i = 0;i < nCount && pos != mapXiaji.end();i++)
// 						{
// 							bool bPushed = dlgSelectRevAccount.IsPushed(i);
// 							if(bPushed)
// 							{
// 								mapIndex[nIndex] = i;
// 								mapRevUserID[nIndex++] = pos->first;
// 								CString strName;
// 								strName.Format(L"%s(%d);",pos->second,pos->first);
// 								
// 								CString strSel;
// 								m_edRevAct.GetWindowText(strSel);
// 
// 								strSel += strName;
// 								m_edRevAct.SetWindowText(strSel);
// 							}
// 							++pos;
// 						}
// 					}
 				}


			}

		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	return true;

	//错误断言
	ASSERT(FALSE);

	return true;
}
void CPlatformSendMessage::SelectUser(int nUser,int nSel)
{
	if(nUser == 32)
	{
		m_edRevAct.SetWindowText(L"");
		mapRevUserID.clear();
		return;
	}
	else if(nUser == 31)
	{
		for(map<int ,CString >::iterator pos = mapXiaji.begin();pos!=mapXiaji.end();++pos)
		{
			if(nSel == 1)
			{
				mapRevUserID[pos->first] = pos->second;
			}
			else
			{
				for(map<int ,CString >::iterator posR = mapRevUserID.begin();posR!=mapRevUserID.end();++posR)
				{
					if(posR->first == pos->first)
					{
						mapRevUserID.erase(posR);
					}
				}
			}
			
		}
	}
	else 
	{
		if(nSel == 0)
		{
			int nCount = mapXiaji.size();
			int nUserID = 0;
			map<int ,CString>::iterator pos = mapXiaji.begin();
			for(int i = 0;i < nCount&& pos != mapXiaji.end();i++)
			{
				if(i == nUser)
				{
					nUserID = pos->first;
				}
				++pos;
			}
			for(map<int ,CString>::iterator posR = mapRevUserID.begin();posR!=mapRevUserID.end();++posR)
			{
				if(posR->first == nUserID)
				{
					mapRevUserID.erase(posR);
					break;
				}
			}
			
		}
		else
		{
			int nCount = mapXiaji.size();

			map<int ,CString>::iterator pos = mapXiaji.begin();
			nCount = min(nCount,30);

			//	mapRevUserID.clear();

				int nIndex = 0 ;
				pos = mapXiaji.begin();
				m_edRevAct.SetWindowText(L"");
				for (int i = 0;i < nCount && pos != mapXiaji.end();i++)
				{
					bool bPushed = m_dlgSelectRevAccount.IsPushed(i);
					if(bPushed)
					{
						mapIndex[nIndex] = i;
						bool bAdd = true;
						int nRevSize = mapRevUserID.size();
						for (map<int,CString>::iterator posR = mapRevUserID.begin();posR!=mapRevUserID.end();++posR)
						{
							if(posR->first == pos->first)
							{
								bAdd = false;
							}
						}
						if(bAdd)
							mapRevUserID[pos->first] = pos->second;
	// 					CString strName;
	// 					strName.Format(L"%s(%d);",pos->second,pos->first);
	// 					
	// 					CString strSel;
	// 					m_edRevAct.GetWindowText(strSel);
	// 
	// 					strSel += strName;
	// 					m_edRevAct.SetWindowText(strSel);
					}
					++pos;
				}
		}
	}

	m_edRevAct.SetWindowText(L"");
	int nIndex = 0;
	for (map<int,CString>::iterator posR = mapRevUserID.begin();posR!=mapRevUserID.end();++posR)
	{
		CString strName;
		
		strName.Format(L"%s(%d);",posR->second,posR->first);

		CString strSel;
		m_edRevAct.GetWindowText(strSel);

		strSel += strName;
		m_edRevAct.SetWindowText(strSel);

		CString strLog;
		strLog.Format(L"MAPREV id:(%d)%s,count:%d",posR->first,posR->second,mapRevUserID.size());
		OutputDebugString(strLog);
	}

}
VOID CPlatformSendMessage::SendToServer(int nSendType)
{

	if(nSendType == 1)   //下注记录数量
	{
		CMD_GP_GetAllUserInfo GetUserInfo;
		ZeroMemory(&GetUserInfo,sizeof(GetUserInfo));

		GetUserInfo.n_t_typeid= 0;
		CString strSearch;
		if(m_nSearchType == 1)
		{
			m_dlgSelectRevAccount.m_edSearch.GetWindowText(strSearch);
			CopyMemory(GetUserInfo.s_t_search,strSearch.GetBuffer(),CountArray(GetUserInfo.s_t_search));
		}
		else
		{
			ZeroMemory(GetUserInfo.s_t_search,sizeof(GetUserInfo.s_t_search));
		}
		GetUserInfo.n_t_userid = theAccount.user_id;
		GetUserInfo.n_t_edittype = 1;
		GetUserInfo.n_t_size = m_nPage;
		GetUserInfo.n_t_count = 27;
		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_ALL_USERINFO,&GetUserInfo,sizeof(GetUserInfo));
		m_dlgSelectRevAccount.HideAllButton();
		strSearch.ReleaseBuffer();
		m_nSearchType = 0;
		return;
	}

	if(nSendType == 2)
	{
		CMD_GP_SendMessage SendMessage;
		ZeroMemory(&SendMessage,sizeof(SendMessage));

		CString strContent;
		m_richContent.GetWindowText(strContent);
		CopyMemory(SendMessage.s_t_content,strContent.GetBuffer(),sizeof(SendMessage.s_t_content));
		
		CString strTitle;
		m_edRevTitle.GetWindowText(strTitle);
		CopyMemory(SendMessage.s_t_title,strTitle.GetBuffer(),sizeof(SendMessage.s_t_title));
		SendMessage.n_t_userid = theAccount.user_id;
		SendMessage.n_t_type = m_cbSendType;
		SendMessage.n_t_sign = m_nSign;
		for (map<int,CString>::iterator pos = mapRevUserID.begin();pos!=mapRevUserID.end();++pos)
		{
			SendMessage.n_t_revid = pos->first;
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SEND_MESSAGE,&SendMessage,sizeof(SendMessage));
		}
		
		return;
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
	if(nSendType == 4)
	{
		CMD_GP_GetLingJiang GetLingjiang;
		ZeroMemory(&GetLingjiang,sizeof(GetLingjiang));

		GetLingjiang.dwUserID = theAccount.user_id;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_LING_JIANG,&GetLingjiang,sizeof(GetLingjiang));
		return;

	}


}