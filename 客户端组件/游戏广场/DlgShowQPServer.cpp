// DlgShowQPServer.cpp : 实现文件
//

#include "stdafx.h"
#include "GamePlaza.h"
#include "DlgShowQPServer.h"


#define GAME_SERVER_ITEM_PX			5									//房间间隔
#define GAME_SERVER_ITEM_PY			3									//房间间隔
#define GAME_SERVER_ITEM_CX			190									//房间宽度
#define GAME_SERVER_ITEM_CY			225									//房间高度
//图片位置
#define GAME_TITLE_CX				786									//标题宽度
#define GAME_TITLE_CY				148									//标题高度



// CDlgShowQPServer 对话框

IMPLEMENT_DYNAMIC(CDlgShowQPServer, CDialog)

CDlgShowQPServer::CDlgShowQPServer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShowQPServer::IDD, pParent)
{
	m_nSelServerID=0;
}

CDlgShowQPServer::~CDlgShowQPServer()
{
	m_font.DeleteObject();
}

void CDlgShowQPServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btQuit);
	DDX_Control(pDX, IDC_BUTTON1, m_btn_enter1);
	DDX_Control(pDX, IDC_BUTTON2, m_btn_enter2);
	DDX_Control(pDX, IDC_BUTTON3, m_btn_enter3);
	DDX_Control(pDX, IDC_BUTTON4, m_btn_enter4);
}


BEGIN_MESSAGE_MAP(CDlgShowQPServer, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CDlgShowQPServer::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgShowQPServer::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgShowQPServer::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgShowQPServer::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgShowQPServer::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgShowQPServer::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDlgShowQPServer 消息处理程序

BOOL CDlgShowQPServer::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_font.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,0,0,0,0,FF_SWISS,TEXT("微软雅黑"));
	//设置关闭按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	//XX
	m_btQuit.SetImage(CBmpUtil::GetExePath() + _T("skin\\bt_close.png"));
	//回到大厅
	m_btOk.SetImage(CBmpUtil::GetExePath() + _T("skin\\btn_back_plaza.png"));
	
	//进入游戏
	m_btn_enter1.SetImage(CBmpUtil::GetExePath() + _T("skin\\btn_enter_game.png"));
	m_btn_enter2.SetImage(CBmpUtil::GetExePath() + _T("skin\\btn_enter_game.png"));
	m_btn_enter3.SetImage(CBmpUtil::GetExePath() + _T("skin\\btn_enter_game.png"));
	m_btn_enter4.SetImage(CBmpUtil::GetExePath() + _T("skin\\btn_enter_game.png"));
	
	m_ImageGround.LoadImage(AfxGetInstanceHandle(),TEXT("PNG_PLAZA_QP_GAME"));

	CRect rc;
	GetWindowRect(&rc);
//	ClientToScreen(&rc);
	int nHeight = rc.Height();
	int nWidth = rc.Width();

	MoveWindow( rc.left, rc.top, m_ImageGround.GetWidth(), m_ImageGround.GetHeight());
	//MoveWindow( 0, 0, m_ImageGround.GetWidth(), m_ImageGround.GetHeight());

	//m_btOk
	return TRUE;
}
void CDlgShowQPServer::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDlgShowQPServer::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


//绘画背景
BOOL CDlgShowQPServer::OnEraseBkgnd(CDC * pDC)
{
		//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建设备
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	CFont* pOldFont = pBufferDC->SelectObject(&m_font);

	//绘画背景
	bool bQiPaiStyle=true;

	if(bQiPaiStyle)
	{
		m_ImageGround.DrawImage(pBufferDC,0,0);
	}
//	CPngImage ImageGround;
//	{
//		ImageGround.LoadImage(AfxGetInstanceHandle(),TEXT("PNG_PLAZA_QP_GAME"));
//		ImageGround.DrawImage(pBufferDC,0,0);
//	}

		//变量定义
		WORD wViewCount=m_nViewCount;
		WORD wStartIndex=m_nStartIndex;

		//绘画标题
		//m_ImageTitle.DrawImage(pBufferDC,(rcClient.Width()-GAME_TITLE_CX)/2,0,GAME_TITLE_CX,GAME_TITLE_CY,0,0);

		//绘画类型
		int nIndex=0;
		for (WORD i=0;i<wViewCount;i++)
		{
			//完成判断
			if ((wStartIndex+i)>=m_GameServerInfoActive->GetCount())
			{
				break;
			}

			//计算位置
			INT nXDrawPos=0+(i%m_wServerXCount)*(GAME_SERVER_ITEM_CX+GAME_SERVER_ITEM_PX);
			INT nYDrawPos=GAME_TITLE_CY+GAME_SERVER_ITEM_PY+(i/m_wServerXCount)*(GAME_SERVER_ITEM_CY+GAME_SERVER_ITEM_PY);

			//绘画对象
			tagGameServerInfo * pGameServerInfo= (*m_GameServerInfoActive)[wStartIndex+i];
			if (pGameServerInfo!=NULL) 
				DrawServerItem(pBufferDC,i,nXDrawPos,nYDrawPos,pGameServerInfo, nIndex++);
		}

		int nXPos = 30;		//m_wServerExcursion;
		int nYPos = GAME_TITLE_CY+GAME_SERVER_ITEM_PY;

		//棋牌界面里显示：用户名  分数   返点
		CRect rcUserName(nXPos+120, nYPos-50, nXPos+220, nYPos-30);
		CRect rcUserScore(nXPos+400, nYPos-50, nXPos+500, nYPos-30);
		CRect rcUserFandian(nXPos+715, nYPos-50, nXPos+780, nYPos-30);


		TCHAR szBuffer[256];
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"), theAccount.account);
		pBufferDC->DrawText(szBuffer,lstrlen(szBuffer),rcUserName,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		_sntprintf(szBuffer,CountArray(szBuffer),SCORE_STRING, pGlobalUserData->lScore);
		pBufferDC->DrawText(szBuffer,lstrlen(szBuffer),rcUserScore,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.0f %%"), theAccount.fandian * 1000*2);
		pBufferDC->DrawText(szBuffer,lstrlen(szBuffer),rcUserFandian,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);


	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);


	pBufferDC->SelectObject(pOldFont);

	return TRUE;
}

//大小尺寸
void CDlgShowQPServer::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL) 
	{
		return;
	}

	if(m_ImageGround.IsNull())
		return;


	int nWidth = m_ImageGround.GetWidth();
	HDWP hDwp=BeginDeferWindowPos(2);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;
	//两个滚动条的位置
//	DeferWindowPos(hDwp,this,NULL,nWidth-382,345,15,210,uFlags);
//	DeferWindowPos(hDwp,m_VerticalScrollBarQp,NULL,nWidth-32,345,15,210,uFlags);
	CRect rcButton;
	m_btQuit.GetWindowRect(&rcButton);

	int nButtonSpace = rcButton.Width();
	//int nWidth = 870;
	int nHeight= 588;
	int nYButtonPos = 5;

	m_btOk.SetWindowPos(NULL,nWidth/2 - 150,nYButtonPos + 420,m_btOk.Width()*2, m_btOk.Height(),SWP_NOZORDER|SWP_NOSIZE);


	//XX
	m_btQuit.SetWindowPos(NULL,nWidth - nButtonSpace-4, nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	//if(m_btOk.m_hWnd!=NULL)
	//{
	//	//获取大小
	//	CRect rcButton;
	//	m_btOk.GetWindowRect(&rcButton);
	//	int nButtonSpace = rcButton.Width();
	//	int nYButtonPos = 10;
	//	m_btOk.SetWindowPos(NULL,nWidth - nButtonSpace*2,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	//}
	EndDeferWindowPos(hDwp);

}


//绘画房间
VOID CDlgShowQPServer::DrawServerItem(CDC * pDC, WORD wViewIndex, INT nXPos, INT nYPos, tagGameServerInfo * pGameServerInfo, int nIndex)
{
	//效验参数
	ASSERT(pGameServerInfo!=NULL);
	if (pGameServerInfo==NULL) return;

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));


	//加载资源
	CPngImage ImageEnterServer;
	ImageEnterServer.LoadImage(AfxGetInstanceHandle(),TEXT("BT_ENTER_SERVER"));

	//获取大小
	CSize SizeEnterServer;
//	SizeEnterServer.SetSize(ImageEnterServer.GetWidth()/5L,ImageEnterServer.GetHeight());

	SizeEnterServer.SetSize(m_btn_enter1.Width(), m_btn_enter1.Height());
	//加载图片

	HINSTANCE hInstance=AfxGetInstanceHandle();
	if (m_ImageServer.IsNull())
		m_ImageServer.LoadImage(hInstance,TEXT("UNKNOWN_SERVER"));


	//绘画类型
	if (m_ImageServer.IsNull()==false)
	{
		CSize SizeImage;
		SizeImage.SetSize(m_ImageServer.GetWidth(),m_ImageServer.GetHeight());
		m_ImageServer.DrawImage(pDC,nXPos,nYPos,GAME_SERVER_ITEM_CX,GAME_SERVER_ITEM_CY,0,0,SizeImage.cx,SizeImage.cy);
	}

	//进入游戏
	INT nXEnterIndex=0;
	bool bEnterButtonDown=true;
	bool bEnterButtonHover=false;

	//进入游戏
	if ((bEnterButtonHover==true)&&(bEnterButtonDown==true)) nXEnterIndex=1;
	if ((bEnterButtonHover==true)&&(bEnterButtonDown==false)) nXEnterIndex=2;
	//进入游戏的大按钮  27, 228

//	ImageEnterServer.DrawImage(pDC,nXPos+7,nYPos+189,SizeEnterServer.cx,SizeEnterServer.cy,nXEnterIndex*SizeEnterServer.cx,0);
	//POINT ptn;
	//ptn.x=nXPos+7;
	//ptn.y=nYPos+189;
	//CRect* pRct = new CRect(ptn,SizeEnterServer);
	//m_mapServerIDRect[pGameServerInfo->wServerID] = pRct;


	if(nIndex == 0)
	{
		m_mapServerID[0] = pGameServerInfo->wServerID;
		m_btn_enter1.SetWindowPos(NULL, nXPos+10,nYPos+199,m_btn_enter1.Width(), m_btn_enter1.Height(),SWP_NOZORDER|SWP_NOSIZE);
		m_btn_enter1.ShowWindow(SW_SHOW);
	}
	if(nIndex == 1)
	{
		m_mapServerID[1] = pGameServerInfo->wServerID;
		m_btn_enter2.SetWindowPos(NULL, nXPos+10,nYPos+199,m_btn_enter1.Width(),m_btn_enter1.Height(),SWP_NOZORDER|SWP_NOSIZE);
		m_btn_enter2.ShowWindow(SW_SHOW);
	}
	if(nIndex == 2)
	{
		m_mapServerID[2] = pGameServerInfo->wServerID;
		m_btn_enter3.SetWindowPos(NULL, nXPos+10,nYPos+189,m_btn_enter1.Width(),m_btn_enter1.Height(),SWP_NOZORDER|SWP_NOSIZE);
		m_btn_enter3.ShowWindow(SW_SHOW);
	}
	if(nIndex == 3)
	{
		m_mapServerID[3] = pGameServerInfo->wServerID;
		m_btn_enter4.SetWindowPos(NULL, nXPos+10,nYPos+189,m_btn_enter1.Width(),m_btn_enter1.Height(),SWP_NOZORDER|SWP_NOSIZE);
		m_btn_enter4.ShowWindow(SW_SHOW);
	}

	//获取全局属性
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//负载信息：1 在线人数
	TCHAR szOnlineCount[32]=TEXT("");
	if(pParameterGlobal->m_bShowServerStatus)
	{
		if(pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount>=150) 
			_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("繁忙"));
		if(pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount>=80&&pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount<150) 
			_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("拥挤"));
		if(pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount>=20&&pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount<80) 
			_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("良好"));
		if(pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount<20)
			_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("空闲"));
	}
	else{
		//在线人数
		_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("%ld"),pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount);

	}


	//2：单元积分
	TCHAR szServerScore[32]=TEXT("");
	_sntprintf(szServerScore,CountArray(szServerScore),SCORE_STRING,pGameServerInfo->pGameServerItem->m_GameServer.lServerScore);

	//3：需要金币
	TCHAR szMinServerScore[32]=TEXT("");
	_sntprintf(szMinServerScore,CountArray(szMinServerScore),SCORE_STRING,pGameServerInfo->pGameServerItem->m_GameServer.lMinServerScore);

	//4：房间名字
	TCHAR szServerName[LEN_SERVER]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),pGameServerInfo->pGameServerItem->m_GameServer.szServerName);

	//设置环境
	pDC->SetBkColor(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画信息
	CRect rcOnlineCount(nXPos+78,	nYPos+65,nXPos+168,nYPos+85);		//在线人数
	CRect rcServerScore(nXPos+78,	nYPos+100,nXPos+168,nYPos+120);		//房间底注
	CRect rcMinServerScore(nXPos+78,nYPos+135,nXPos+168,nYPos+155);		//最小准入分数
	CRect rcServerName(nXPos+22,	nYPos+165,nXPos+168,nYPos+195);		//房间名字
	pDC->DrawText(szServerName,lstrlen(szServerName),rcServerName,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	pDC->DrawText(szOnlineCount,lstrlen(szOnlineCount),rcOnlineCount,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	pDC->DrawText(szServerScore,lstrlen(szServerScore),rcServerScore,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	pDC->DrawText(szMinServerScore,lstrlen(szMinServerScore),rcMinServerScore,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

	return;
}

void CDlgShowQPServer::SetDataPrt(CGameServerInfoArray& pGameServer, int nViewCount, int nStartIndex, int nServerXCount)
{
	m_GameServerInfoActive = &pGameServer;
	m_nViewCount=nViewCount;
	m_nStartIndex=nStartIndex;
	m_wServerXCount = nServerXCount;

	CServerListData * pServerListData=CServerListData::GetInstance();
	

}
void CDlgShowQPServer::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_mapServerID.size() >= 1)
		m_nSelServerID = m_mapServerID[0];
	OnOK();
}

void CDlgShowQPServer::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_mapServerID.size() >= 2)
		m_nSelServerID = m_mapServerID[1];
	OnOK();
}

void CDlgShowQPServer::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_mapServerID.size() >= 3)
		m_nSelServerID = m_mapServerID[2];
	OnOK();
}

void CDlgShowQPServer::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_mapServerID.size() >= 4)
		m_nSelServerID = m_mapServerID[3];
	OnOK();
}


DWORD	CDlgShowQPServer::GetSelServerID()
{
	return m_nSelServerID;
}


void CDlgShowQPServer::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}