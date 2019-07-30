#include "stdafx.h"
#include "GamePlaza.h"
#include "YanZhengMa.h"
#include "Bmputil.h"
#include "MessageDlg.h"
#include "MissionLogon.h"

static const int wnd_width = 332;
static const int wnd_height = 264;

static const int btn_ok_x = 100;
static const int btn_ok_y = 40;

static const int btn_cancel_x = 233;
static const int btn_cancel_y = 240;

//玩法
static const int tip_wanfa_x = 130;
static const int tip_wanfa_y = 47;
static const int tip_wanfa_width = 70;
static const int tip_wanfa_height = 20;
static CRect rc_wanfa(tip_wanfa_x, 
						   tip_wanfa_y, 
						   tip_wanfa_x + tip_wanfa_width, 
						   tip_wanfa_y + tip_wanfa_height);

//注数
static const int tip_zhushu_x = 130;
static const int tip_zhushu_y = 87;
static const int tip_zhushu_width = 70;
static const int tip_zhushu_height = 20;
static CRect rc_zhushu(tip_zhushu_x, 
						   tip_zhushu_y, 
						   tip_zhushu_x + tip_zhushu_width, 
						   tip_zhushu_y + tip_zhushu_height);

//用户ID
static const int tip_danzhujine_x = 80;
static const int tip_danzhujine_y = 50;
static const int tip_danzhujine_width = 250;
static const int tip_danzhujine_height = 20;
static CRect rc_zongjine(tip_danzhujine_x, 
						   tip_danzhujine_y, 
						   tip_danzhujine_x + tip_danzhujine_width, 
						   tip_danzhujine_y + tip_danzhujine_height);
//验证码提示文字
static const int tip_yanzheng_x = 20;
static const int tip_yanzheng_y = 45;
static const int tip_yanzheng_width = 288;
static const int tip_yanzheng_height = 20;

static CRect rc_tip(tip_yanzheng_x, 
						 tip_yanzheng_y, 
						 tip_yanzheng_x + tip_yanzheng_width, 
						 tip_yanzheng_y + tip_yanzheng_height);
//总金额
static const int tip_zongjine_x = 80;
static const int tip_zongjine_y = 111;
static const int tip_zongjine_width = 230;
static const int tip_zongjine_height = 20;
static CRect rc_danzhujine(tip_zongjine_x, 
						   tip_zongjine_y, 
						   tip_zongjine_x + tip_zongjine_width, 
						   tip_zongjine_y + tip_zongjine_height);

// CYanZhengMaDlg 对话框
static const int timer_yanzhengma = 100;
static const int timer_daojishi = 101;
IMPLEMENT_DYNAMIC(CYanZhengMaDlg, CDialog)

CYanZhengMaDlg::CYanZhengMaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYanZhengMaDlg::IDD, pParent)
	, m_bmpBk(NULL)
	,m_fJine(0.0)
	,m_fJiaoYiJine(0.0)
{
	m_nEscape = 120;
	m_cbType=0;
	m_nRandNum = 0;
	m_strPhoneNum.Empty();
	ZeroMemory(m_szPassWord,sizeof(m_szPassWord));
}

CYanZhengMaDlg::~CYanZhengMaDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CYanZhengMaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_editYanZhengma);
	DDX_Control(pDX, ID_BTN_HQYZM, m_btnHqyzm);


}


BEGIN_MESSAGE_MAP(CYanZhengMaDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_BTN_HQYZM, &CYanZhengMaDlg::OnBnClickedBtnHqyzm)
		ON_WM_TIMER()
END_MESSAGE_MAP()

void CYanZhengMaDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == timer_yanzhengma)
	{
	//	if(m_nEscape<=0)
		{
			KillTimer(timer_yanzhengma);
			KillTimer(timer_daojishi);
			m_cbType = 2;
			m_nEscape = 120;
			m_btnHqyzm.EnableWindow(TRUE);
			InvalidateRect(&rc_tip);
			return;
		}
	}
	if(nIDEvent == timer_daojishi)
	{
		m_nEscape--;
		if(m_nEscape<=0)
		{
			m_cbType = 2;
			KillTimer(timer_daojishi);
		}
		InvalidateRect(&rc_tip);
	}
	CDialog::OnTimer(nIDEvent);
}
int request(char* hostname, char* api, char* parameters)
{
	WSADATA WsaData;
	WSAStartup(0x0101, &WsaData);


	//初始化socket
	struct hostent* host_addr = gethostbyname(hostname);
	if (host_addr == NULL)
	{
		//		cout<<"Unable to locate host"<<endl;
		return -103;
	}


	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons((unsigned short)80);
	sin.sin_addr.s_addr = *((int*)*host_addr->h_addr_list);


	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		return -100;
	}


	//建立连接
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sockaddr_in) ) == -1)
	{
		//		cout<<"connect failed"<<endl;
		return -101;
	}


	//初始化发送信息
	char send_str[2048] = {0};


	//头信息
	strcat(send_str, "POST ");
	strcat(send_str, api);
	strcat(send_str, " HTTP/1.1\r\n");
	strcat(send_str, "Host: ");
	strcat(send_str, hostname);
	strcat(send_str, "\r\n");
	strcat(send_str, "Connection: keep-alive\r\n");

	int iLength = 0;  
	TCHAR szContentHeader[100];
	char content_header[100];
	_sntprintf(szContentHeader,CountArray(szContentHeader),L"Content-Length: %d\r\n", strlen(parameters));
	iLength = WideCharToMultiByte(CP_ACP, 0, szContentHeader, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, szContentHeader, -1, content_header, iLength, NULL, NULL);   
	strcat(send_str, content_header);
	strcat(send_str, "Content-Type: application/x-www-form-urlencoded\r\n");


	//内容信息
	strcat(send_str, "\r\n");
	strcat(send_str, parameters);


	if (send(sock, send_str, strlen(send_str),0) == -1)
	{
		//		cout<<"send failed"<<endl;
		return -101;
	}


	//获取返回信息
	char recv_str[4096] = {0};
	if (recv(sock, recv_str, sizeof(recv_str), 0) == -1)
	{
		//	cout<<"recv failed"<<endl;
		return -101;
	}


	//cout<<recv_str<<endl;


	WSACleanup( );


	return 0;
}
char* CreateUTF8TextInitWithString(CString strValue)  
{  
	char *buffer = NULL;  
	int  length;  

#ifdef _UNICODE  
	length = WideCharToMultiByte(CP_UTF8, 0, strValue, -1, NULL, 0, NULL, NULL);  
#else  
	return NULL;  
#endif  
	if (length <= 0)  
	{  
		return NULL;  
	}  

	buffer = new char[length];  
	if (buffer == NULL)  
	{  
		return NULL;  
	}  

	ZeroMemory(buffer, length);  

#ifdef _UNICODE  
	WideCharToMultiByte(CP_UTF8, 0, strValue, -1, buffer, length, NULL, NULL);  
#else  
	strcpy_s(buffer, length, strValue);  
#endif  

	return buffer;  
}
bool	CYanZhengMaDlg::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}
#include "PlatformFrame.h"
CString CYanZhengMaDlg::GetYanZhengma()
{
// 	CString strRandNum;
// 	m_editYanZhengma.GetWindowText(strRandNum);
	return m_strInput;
}
void CYanZhengMaDlg::OnBnClickedBtnHqyzm()
{
	srand( (unsigned)time( NULL ) );
	CString strTempRand;
	int nRand = 0;// (rand()%1000000+100000);

	int nIndex = 1;
	int nRandWei = 100000;
	while (nIndex++<7)
	{
		int nNum = rand()%(1,10);
		if(nNum<=0)
			nNum++;
		nRand += nNum*nRandWei;
		nRandWei/=10;
	}
	m_nRandNum = nRand;

	if(m_cbNetWork == 1)
	{
		CString strYanzhengma;
		strYanzhengma.Format(L"%d",nRand);
		CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
		if (pMissionLogon!=NULL) pMissionLogon->PerformSendYanZhengma(strYanzhengma);
	}
	else if(m_cbNetWork == 2)
	{
		CString strYanzhengma;
		strYanzhengma.Format(L"%d",nRand);
		CMD_SendYanZhengma SendYanZhengma;
		ZeroMemory(&SendYanZhengma,sizeof(SendYanZhengma));

		SendYanZhengma.n_t_userid = theAccount.user_id;
		_sntprintf(SendYanZhengma.s_t_code,CountArray(SendYanZhengma.s_t_code),TEXT("%s"),strYanzhengma);
		SendYanZhengma.c_t_type = 1;
		SendYanZhengma.c_t_stype = 1;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SEND_YANZHENGMA_TRANS,&SendYanZhengma,sizeof(SendYanZhengma));

	}

	if(m_cbOperateType == 0)
		strTempRand.Format(L"您正在绑定手机号码,验证码为：%d,如非本人操作,请忽略.",nRand);
	else if(m_cbOperateType == 1)
		strTempRand.Format(L"您正在解绑手机号码,验证码为：%d,如非本人操作,请忽略.",nRand);
	else if(m_cbOperateType == 2)
		strTempRand.Format(L"您正在登录JIUYI,验证码为：%d,如非本人操作,请忽略.",nRand);
	else if(m_cbOperateType == 3)
		strTempRand.Format(L"您正在JIUYI给下级转账,验证码为：%d,如非本人操作,请忽略.",nRand);
	CString strContent;
	strContent.Format(L"http://web.wasun.cn/asmx/smsservice.aspx?name=caifu000111222&pwd=DBA9C1688034B26F772E5BAFBBA5&content=%s&mobile=%s&stime=&sign=JIUYI&type=pt&extno=",strTempRand,m_strPhoneNum);

	string stContent;
	ChangeStringToA(strContent,stContent);

	char cContent[500];
	strcpy(cContent,stContent.c_str());

	string sRandNum;
	ChangeStringToA(strTempRand,sRandNum);
	char cRand[200] ;
	strcpy(cRand,sRandNum.c_str());
	char* pContent = CreateUTF8TextInitWithString(strContent);
	int nRet = request("web.wasun.cn",pContent,cRand);
	if(nRet == 0)
	{
		CInformation information(this);
		information.ShowMessageBox(L"获取验证码",L"验证码获取成功！",MB_OK,5);
		m_btnHqyzm.EnableWindow(FALSE);
		m_cbType = 1;
		SetTimer(timer_daojishi,1000L,NULL);
		SetTimer(timer_yanzhengma,120*1000L,NULL);
	}
	else
	{
		CInformation information(this);
		information.ShowMessageBox(L"获取验证码",L"验证码获取失败，请点击按钮重新获取！",MB_OK,5);
		m_cbType = 2;
	}
	InvalidateRect(&rc_tip);
}
// CYanZhengMaDlg 消息处理程序

void CYanZhengMaDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

	if(m_cbType == 1)
	{
		CString strNotice;
		strNotice.Format(L"验证码已发送至您的手机，%d秒后重新获取验证码",m_nEscape);
		cacheDC.DrawText(strNotice, rc_tip, DT_CENTER|DT_VCENTER|DT_WORD_ELLIPSIS|DT_WORDBREAK|DT_EDITCONTROL);
	}
	else 	if(m_cbType == 2)
	{
		CString strNotice;
		strNotice.Format(L"请重新获取验证码");
		cacheDC.DrawText(strNotice, rc_tip,  DT_CENTER|DT_VCENTER|DT_WORD_ELLIPSIS|DT_WORDBREAK|DT_EDITCONTROL);

	}

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CYanZhengMaDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
//cType 0:绑定手机,1:解除绑定,2:登陆验证
void CYanZhengMaDlg::SetYanZhengXinxi(CString strNum,BYTE IsNetwork ,BYTE cType)
{
	m_strPhoneNum = strNum;
	m_cbNetWork = IsNetwork;
	m_cbOperateType = cType;
	return;
}

VOID CYanZhengMaDlg::OnOK()
{
	//CString strRandNum;
	m_editYanZhengma.GetWindowText(m_strInput);
	if(m_strInput.IsEmpty())
	{
		MyMessageBox(L"请输入验证码！");
		return;
	}
	if(m_nRandNum < 100000)
	{
		MyMessageBox(L"请先获取验证码！");
		return;
	}
	int nRandNum = _ttoi(m_strInput);
	if(nRandNum != m_nRandNum)
	{
		MyMessageBox(L"验证码错误，请重新输入!");
		return;
	}
	return CDialog::OnOK();
}
VOID CYanZhengMaDlg::OnCancel()
{
	return CDialog::OnCancel();
}
BOOL CYanZhengMaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\sjyz.png"));

	m_font.CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	m_font1.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\quxiao_bt.png"));
	m_btnHqyzm.SetImage(CBmpUtil::GetExePath() + _T("skin\\hqyzm.png"));
// 	SetTimer(timer_daojishi,1000L,NULL);
// 
// SetTimer(timer_yanzhengma,60*1000L,NULL);
	m_editYanZhengma.SetEnableColor(RGB(56,90,154),RGB(253,255,255),RGB(253,255,255));
	m_editYanZhengma.SetFont(&m_font1);
	SetWindowPos(NULL, 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CYanZhengMaDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	if(m_btnHqyzm.GetSafeHwnd() != NULL)
	{
		m_btnHqyzm.SetWindowPos(NULL, btn_ok_x+14+88, btn_ok_y+40, m_btnHqyzm.Width(), m_btnHqyzm.Height(), SWP_NOZORDER);
	}
	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x+14+48-99, btn_ok_y+100, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}

	if(m_btnCancel.GetSafeHwnd() != NULL)
	{
		m_btnCancel.SetWindowPos(NULL, btn_cancel_x-52, btn_cancel_y-101, m_btnCancel.Width(), m_btnCancel.Height(), SWP_NOZORDER);
	//	m_btnCancel.ShowWindow(SW_HIDE);
	}

	if(m_editYanZhengma.GetSafeHwnd() != NULL)
	{
		m_editYanZhengma.SetWindowPos(NULL, btn_cancel_x-134, btn_cancel_y-158,60, 28, SWP_NOSIZE);
	}

}

