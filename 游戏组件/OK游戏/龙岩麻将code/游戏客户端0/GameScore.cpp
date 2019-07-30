#include "Stdafx.h"
#include "GameScore.h"
#include "math.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////

//按钮标识
#define IDC_CLOSE_SCORE				100									//关闭成绩

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScore, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameScore::CGameScore() : CDialog(IDD_GAME_SCORE)
{
	//初始化变量
	ZeroMemory( &m_ScoreInfo,sizeof(m_ScoreInfo) );
	m_cbWeaveCount = 0;
	m_ChiHuRight.SetEmpty();

	HINSTANCE hInst = AfxGetInstanceHandle();
	m_ImageBack.LoadFromResource( hInst, IDB_GAME_SCORE );
	m_ImageFangPao.LoadImage( hInst,TEXT("FANG_PAO") );
	m_ImageZiMo.LoadImage( hInst,TEXT("ZI_MO") );
	m_ImageHuangZhuang.LoadFromResource(hInst,IDB_HUANG_ZHUANG);
}

//析构函数
CGameScore::~CGameScore()
{
}

//控件绑定
void CGameScore::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btCloseScore);
}

//初始化函数
BOOL CGameScore::OnInitDialog()
{
	__super::OnInitDialog();

	//删除窗口标题和获取属性
	ModifyStyle(WS_CAPTION,0,0);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);


	//创建区域
	CRgn RgnControl;
	m_ImageBack.CreateImageRegion(RgnControl,RGB(255,0,255));

	//设置窗口
	if (RgnControl.GetSafeHandle()!=NULL)
	{
		//移动窗口
		SetWindowRgn(RgnControl,TRUE);
		SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE);
	}

	m_btCloseScore.SetButtonImage(IDB_BT_SCORE_CLOSE,AfxGetInstanceHandle(),false,false);

	//调整按钮
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rcBtClose;
	m_btCloseScore.GetWindowRect(&rcBtClose);
	m_btCloseScore.SetWindowPos(NULL,(rcClient.Width()-rcBtClose.Width())/2,rcClient.Height()-rcBtClose.Height()-10,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//创建字体
	LOGFONT lg;
	ZeroMemory( &lg,sizeof(lg) );
	lg.lfCharSet = GB2312_CHARSET;
	lstrcpy( lg.lfFaceName,_T("黑体"));
	lg.lfHeight = -20;
//	if( !m_InfoFont.CreateFontIndirect( &lg ) )
	//	m_InfoFont.Attach(CSkinResourceManager::GetDefaultFont());

	//设置透明
	ModifyStyleEx(0,0x00080000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//取得SetLayeredWindowAttributes（）函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),0,220,2); 
		FreeLibrary(hInst); 
	}

	return TRUE;
}

//复位数据
void CGameScore::RestorationData()
{
	//设置变量
	m_cbWeaveCount=0;
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

	//隐藏窗口
	if (m_hWnd!=NULL) ShowWindow(SW_HIDE);

	return;
}

//设置积分
void CGameScore::SetScoreInfo(const tagScoreInfo & ScoreInfo, const tagWeaveInfo & WeaveInfo, const CChiHuRight &ChiHuRight)
{
	//设置变量
	m_ScoreInfo=ScoreInfo;
	m_cbWeaveCount=WeaveInfo.cbWeaveCount;
	m_ChiHuRight = ChiHuRight;

	//组合变量
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		bool bPublicWeave=(WeaveInfo.cbPublicWeave[i]==TRUE);
		m_WeaveCard[i].SetCardData(WeaveInfo.cbCardData[i],WeaveInfo.cbCardCount[i]);
		m_WeaveCard[i].SetDisplayItem(true);
	}

	//显示窗口
	ShowWindow(SW_SHOW);

	return;
}

//关闭按钮
void CGameScore::OnOK()
{
	//隐藏窗口
	RestorationData();

	__super::OnOK();
}

//重画函数
void CGameScore::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//设置 DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);
	DCBuffer.SelectObject(m_InfoFont);
	DCBuffer.SetTextColor(RGB(250,250,250));


	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(DCBuffer,0,0);

	//位置变量
	int nCardSpace=5;
	int nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
	int nTotalWidth=m_cbWeaveCount*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount+nCardSpace;

	//计算位置
	int nYCardPos=140-g_CardResource.m_ImageTableBottom.GetViewHeight();
	int nXCardPos=(rcClient.Width()-nTotalWidth)/2;
	TCHAR szBuffer[32] = TEXT("");

	//列表信息
	DCBuffer.SetTextColor( RGB(0,255,255) );
	INT nY=270;
	DCBuffer.TextOut(41+65,nY,TEXT("玩家"));
	//DCBuffer.TextOut(140,nY,TEXT("倍数"));
	//DCBuffer.TextOut(205,nY,TEXT("倍数"));
	//DCBuffer.TextOut(270,nY,TEXT("被跟"));
	DCBuffer.TextOut(345,nY,TEXT("总计"));
	//DCBuffer.TextOut(415,nY,TEXT("总计"));

	//胡牌信息
	if( m_ScoreInfo.wProvideUser != INVALID_CHAIR )
	{
		//绘画组合
		for (BYTE i=0;i<m_cbWeaveCount;i++)
		{
			//绘画扑克
			m_WeaveCard[i].DrawCardControl(&DCBuffer,nXCardPos,nYCardPos,false);

			//设置位置
			nXCardPos+=(nCardSpace+nItemWidth*3);
		}

		//绘画扑克
		for (BYTE i=0;i<m_ScoreInfo.cbCardCount;i++)
		{
			//绘画扑克
			g_CardResource.m_ImageTableBottom.DrawCardItem(&DCBuffer,m_ScoreInfo.cbCardData[i],nXCardPos,nYCardPos,false);

			//设置位置
			nXCardPos+=nItemWidth;
			if ((i+2)==m_ScoreInfo.cbCardCount) nXCardPos+=nCardSpace;
		}

		CRect rcDraw;
		//放炮
		DCBuffer.SetTextColor( RGB(252,255,255));
		if( m_ScoreInfo.wProvideUser != m_ScoreInfo.wChiHuUser )
		{
			rcDraw.SetRect(15,148,190,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageFangPao.DrawImage( &DCBuffer,205,150 );
			rcDraw.SetRect(295,148,482,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );
		}
		//自摸
		else
		{
			rcDraw.SetRect(15,148,190,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageZiMo.DrawImage( &DCBuffer,215,150 );
		}
	}

	//得分信息
	DCBuffer.SetTextColor( RGB(250,250,250) );
	for( WORD j = 0; j < GAME_PLAYER; j++ )
	{
		WORD i=(j+m_ScoreInfo.wMeChairID)%GAME_PLAYER;

		INT nY1=293+j*26;
		INT nY2=320+j*26;

		INT nX = 25+65;
		CRect rcDrawPlayer(nX,nY1,nX+175,nY2);
		//玩家
		DCBuffer.SetTextColor(( (i==m_ScoreInfo.wMeChairID)? RGB(252,255,0):RGB(252,255,0) ));
		DCBuffer.DrawText( m_ScoreInfo.szUserName[i],lstrlen(m_ScoreInfo.szUserName[i]),
			&rcDrawPlayer,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );

		////倍数
		//nX = 195/*130*/;
		//CRect rcDrawScore1(nX,nY1,nX+60,nY2);
		//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%ld"),m_ScoreInfo.lHuScore[i] );
		//DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore1,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		////犯枪
		//nX = 195;
		//CRect rcDrawScore2(nX,nY1,nX+60,nY2);
		//if(m_ScoreInfo.lFanHuScore[i]!=0)
		//	_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+ld"),m_ScoreInfo.lFanHuScore[i] );
		//else _sntprintf( szBuffer,CountArray(szBuffer),TEXT("0"));
		//DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore2,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		////被跟
		//nX = 260;
		//CRect rcDrawScore3(nX,nY1,nX+60,nY2);
		//if(m_ScoreInfo.lFollowScore[i]!=0)
		//	_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+ld"),m_ScoreInfo.lFollowScore[i] );
		//else _sntprintf( szBuffer,CountArray(szBuffer),TEXT("0"));
		//DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore3,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		////误打财神
		//nX = 335;
		//CRect rcDrawScore4(nX,nY1,nX+60,nY2);
		//if(m_ScoreInfo.lOutMagicScore[i]!=0)
		//	_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+ld"),m_ScoreInfo.lOutMagicScore[i] );
		//else _sntprintf( szBuffer,CountArray(szBuffer),TEXT("0"));
		//DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore4,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		//分数
		nX = /*405*/335;
		CRect rcDrawScore(nX,nY1,nX+60,nY2);
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+ld"),m_ScoreInfo.lGameScore[i] );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		//nX += 65;
		//DCBuffer.SetTextColor( RGB(250,250,250) );
		//CRect rcDrawFen(nX,nY1,nX+152,nY2);
		//DCBuffer.DrawText( CString(TEXT("分")),&rcDrawFen,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}

	//胡牌信息
	if( m_ScoreInfo.wProvideUser != INVALID_CHAIR )
	{
		//牌型信息
		LPCTSTR pszKind[] = {
			TEXT("七对"),TEXT("豪华七对"),TEXT("两豪华七对"),TEXT("三豪华七对"),
				TEXT("杠开"),TEXT("杠暴"),TEXT("暴头"),TEXT("财飘"),TEXT("杠飘"),TEXT("飘杠"),
		};
		//牌型权位
		DWORD pdwRight[] = {
			CHR_QI_DUI,CHR_QI_DUI1,CHR_QI_DUI2,CHR_QI_DUI3,
				CHR_GANG_KAI,CHR_GANG_KAI_MAGIC,CHR_MAGIC_HU,
				CHR_MAGIC_HU2,CHR_GANG_MAGIC,CHR_MAGIC_GANG,
		};

		//倍数
		BYTE pcbFanCount[] = {
			2,4,8,16,
			2,4,2,4,8,8,
		};

		//牌型
		CString csInfo,csCard;
		CRect rcFanXing(34,191,159,217);
		for( BYTE i = 0; i < CountArray(pcbFanCount); i++ ) 
		{
			if( !(m_ChiHuRight&pdwRight[i]).IsEmpty() )
			{
				CString strTemp=TEXT("");
				BYTE cbTimer=pcbFanCount[i];
				if(m_ScoreInfo.bSecondMagic && i>=(CountArray(pcbFanCount)-3))
				{
					cbTimer *= 2;
					strTemp=TEXT("(多飘花)");
				}
				csCard.Format(TEXT("%s%s   %d倍；  "),pszKind[i],strTemp,cbTimer);
				csInfo+=csCard;
			}
		}
		DCBuffer.SetTextColor( RGB(252,255,255) );
		DCBuffer.TextOut( 35,195,csInfo);
	}

	//庄家 总倍数:
	if( m_ScoreInfo.wLandUser <GAME_PLAYER )
	{
		TCHAR szInfo[512]=TEXT("");
		_sntprintf(szInfo,sizeof(szInfo),TEXT("庄家:%s   牢庄:%d次 [%d倍]   底注:%d"),m_ScoreInfo.szUserName[m_ScoreInfo.wLandUser],
			m_ScoreInfo.cbLandCount,LONG(powf(2,__min(m_ScoreInfo.cbLandCount,3))),m_ScoreInfo.lCellScore);
		DCBuffer.SetTextColor( RGB(252,255,255) );
		DCBuffer.TextOut( 35,225,szInfo);
	}

	//荒庄标志
	if (m_ScoreInfo.cbHuangZhuang)
	{
		//CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.AlphaDrawImage(&DCBuffer,185,23,RGB(255,0,255));
	}

	////总计
	//if( m_ScoreInfo.cbFanCount > 0 )
	//{
	//	DCBuffer.DrawText( TEXT("总番"),lstrlen(TEXT("总番")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	//	_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbFanCount );
	//	DCBuffer.SetTextColor( RGB(252,255,0) );
	//	DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
	//	DCBuffer.SetTextColor( RGB(250,250,250) );
	//	DCBuffer.DrawText( CString(TEXT("番")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	//}

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//清理资源
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//鼠标消息
void CGameScore::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//消息模拟
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//背景函数
BOOL CGameScore::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
