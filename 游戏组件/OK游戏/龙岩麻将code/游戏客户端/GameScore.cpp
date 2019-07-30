#include "Stdafx.h"
#include "GameScore.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CLOSE_SCORE				100									//�رճɼ�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScore, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScore::CGameScore() : CDialog(IDD_GAME_SCORE)
{
	//��ʼ������
	ZeroMemory( &m_ScoreInfo,sizeof(m_ScoreInfo) );
	m_cbWeaveCount = 0;
	m_ChiHuRight.SetEmpty();

	HINSTANCE hInst = AfxGetInstanceHandle();
	m_ImageBack.LoadFromResource( hInst,IDB_GAME_SCORE );
	m_ImageFangPao.LoadImage( hInst,TEXT("FANG_PAO") );
	m_ImageZiMo.LoadImage( hInst,TEXT("ZI_MO") );
}

//��������
CGameScore::~CGameScore()
{
}

//�ؼ���
void CGameScore::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btCloseScore);
}

//��ʼ������
BOOL CGameScore::OnInitDialog()
{
	__super::OnInitDialog();

	//ɾ�����ڱ���ͻ�ȡ����
	ModifyStyle(WS_CAPTION,0,0);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//������Դ
	//CImageHandle ImageHandle(&m_ImageBack);

	//��������
	CRgn RgnControl;
	m_ImageBack.CreateImageRegion(RgnControl,RGB(255,0,255));

	//���ô���
	if (RgnControl.GetSafeHandle()!=NULL)
	{
		//�ƶ�����
		SetWindowRgn(RgnControl,TRUE);
		SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE);
	}

	m_btCloseScore.SetButtonImage(IDB_BT_SCORE_CLOSE,AfxGetInstanceHandle(),false,false);

	//������ť
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rcBtClose;
	m_btCloseScore.GetWindowRect(&rcBtClose);
	m_btCloseScore.SetWindowPos(NULL,(rcClient.Width()-rcBtClose.Width())/2,rcClient.Height()-rcBtClose.Height()-10,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//��������
	LOGFONT lg;
	ZeroMemory( &lg,sizeof(lg) );
	lg.lfCharSet = GB2312_CHARSET;
	lstrcpy( lg.lfFaceName,_T("����"));
	lg.lfHeight = -20;
	if( !m_InfoFont.CreateFontIndirect( &lg ) )
		m_InfoFont.Attach(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//����͸��
	ModifyStyleEx(0,0x00080000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//ȡ��SetLayeredWindowAttributes��������ָ�� 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),0,220,2); 
		FreeLibrary(hInst); 
	}

	return TRUE;
}

//��λ����
void CGameScore::RestorationData()
{
	//���ñ���
	m_cbWeaveCount=0;
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

	//��ϱ���
	for (BYTE i=0;i<4;i++)
	{
		m_WeaveCard[i].SetCardData(NULL,0);
	}

	//���ش���
	if (m_hWnd!=NULL) ShowWindow(SW_HIDE);

	return;
}

//���û���
void CGameScore::SetScoreInfo(const tagScoreInfo & ScoreInfo, const tagWeaveInfo & WeaveInfo, const CChiHuRight &ChiHuRight)
{
	//���ñ���
	m_ScoreInfo=ScoreInfo;
	m_cbWeaveCount=WeaveInfo.cbWeaveCount;
	m_ChiHuRight = ChiHuRight;

	//��ϱ���
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		bool bPublicWeave=(WeaveInfo.cbPublicWeave[i]==TRUE);
		m_WeaveCard[i].SetCardData(WeaveInfo.cbCardData[i],WeaveInfo.cbCardCount[i]);
		m_WeaveCard[i].SetDisplayItem(true);
	}

	for (BYTE j=0;j<m_cbWeaveCount;j++)
	{
		if(m_WeaveCard[j].GetCardCount()==4)
		{
			for (BYTE t=j+1;t<m_cbWeaveCount;t++)
			{
				m_WeaveCard[t].OffPoint();
			}
		}

	}
	//��ʾ����
	ShowWindow(SW_SHOW);

	return;
}

//�رհ�ť
void CGameScore::OnOK()
{
	//���ش���
	RestorationData();

	__super::OnOK();
}

//�ػ�����
void CGameScore::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);
	DCBuffer.SelectObject(m_InfoFont);
	DCBuffer.SetTextColor(RGB(250,250,250));

	//�滭����
	//CImageHandle ImageHandle(&m_ImageBack);
	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(DCBuffer,0,0);

	//λ�ñ���
	int nCardSpace=5;
	int nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
	int nTotalWidth=m_cbWeaveCount*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount+nCardSpace;

	//����λ��
	int nYCardPos=120-g_CardResource.m_ImageTableBottom.GetViewHeight();
	int nXCardPos=(rcClient.Width()-nTotalWidth)/2;

	//�滭���
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		//�滭�˿�
		m_WeaveCard[i].DrawCardControl(&DCBuffer,nXCardPos,nYCardPos,false);

		//����λ��
		nXCardPos+=(nCardSpace+nItemWidth*m_WeaveCard[i].GetCardCount());
	}

	//�滭�˿�
	for (BYTE i=0;i<m_ScoreInfo.cbCardCount;i++)
	{
		//�滭�˿�
		g_CardResource.m_ImageTableBottom.DrawCardItem(&DCBuffer,m_ScoreInfo.cbCardData[i],nXCardPos,nYCardPos,false);

		//����λ��
		nXCardPos+=nItemWidth;
		if ((i+2)==m_ScoreInfo.cbCardCount) nXCardPos+=nCardSpace;
	}

	TCHAR szBuffer[32] = TEXT("");
	//������Ϣ
	if( m_ScoreInfo.wProvideUser != INVALID_CHAIR )
	{
		CRect rcDraw;
		//����
		if( m_ScoreInfo.wProvideUser != m_ScoreInfo.wChiHuUser )
		{
			rcDraw.SetRect(15,128,190,155);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageFangPao.DrawImage( &DCBuffer,205,130 );
			rcDraw.SetRect(315,128,482,155);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );
		}
		//����
		else
		{
			rcDraw.SetRect(15,128,190,155);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageZiMo.DrawImage( &DCBuffer,215,130 );
		}
	}

	//�÷���Ϣ
	DCBuffer.SetTextColor( RGB(250,250,250) );
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		CRect rcDrawPlayer(100,163+i*26,275,190+i*26);
		//���
		DCBuffer.DrawText( m_ScoreInfo.szUserName[i],lstrlen(m_ScoreInfo.szUserName[i]),
			&rcDrawPlayer,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );

		//����
		CRect rcDrawScore(295,163+i*26,380,190+i*26);
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%I64d"),m_ScoreInfo.lGameScore[i] );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore,DT_SINGLELINE|DT_BOTTOM|DT_RIGHT );

		DCBuffer.SetTextColor( RGB(250,250,250) );
		CRect rcDrawFen(385,163+i*26,512,190+i*26);
		DCBuffer.DrawText( CString(TEXT("��")),&rcDrawFen,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}

	//������Ϣ
	/*LPCTSTR pszKind[] = {
		TEXT("����")
	};
	//����
	BYTE pcbFanCount[] = {1
	};
	//����Ȩλ
	DWORD pdwRight[] = {CHR_ZI_MO
	};
	//��������
	CRect rcFanXing(34,271,159,297);
	//��������
	CRect rcFanShu(160,271,190,297);
	//������
	CRect rcFan(195,271,215,297);
	BYTE cbDrawCount = 0;
	for( BYTE i = 0; i < CountArray(pcbFanCount); i++ ) 
	{
		if( !(m_ChiHuRight&pdwRight[i]).IsEmpty() )
		{
			DCBuffer.DrawText( pszKind[i],lstrlen(pszKind[i]),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
			_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),pcbFanCount[i] );
			DCBuffer.SetTextColor( RGB(252,255,0) );
			DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
			DCBuffer.SetTextColor( RGB(250,250,250) );
			DCBuffer.DrawText( CString(TEXT("��")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );

			//�ƶ��滭����
			if( ++cbDrawCount%2 == 0 )
			{
				rcFanXing.OffsetRect( -245,26 );
				rcFanShu.OffsetRect( -245,26 );
				rcFan.OffsetRect( -245,26 );
			}
			else
			{
				rcFanXing.OffsetRect( 245,0 );
				rcFanShu.OffsetRect( 245,0 );
				rcFan.OffsetRect( 245,0 );
			}
		}
	}
	*/
	/*if( m_ScoreInfo.cbHuaCardCount > 0 )
	{
		DCBuffer.DrawText( TEXT("��"),lstrlen(TEXT("��")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbHuaCardCount );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
		DCBuffer.SetTextColor( RGB(250,250,250) );
		DCBuffer.DrawText( CString(TEXT("��")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
		//�ƶ��滭����
		if( ++cbDrawCount%2 == 0 )
		{
			rcFanXing.OffsetRect( -245,26 );
			rcFanShu.OffsetRect( -245,26 );
			rcFan.OffsetRect( -245,26 );
		}
		else
		{
			rcFanXing.OffsetRect( 245,0 );
			rcFanShu.OffsetRect( 245,0 );
			rcFan.OffsetRect( 245,0 );
		}
	}*/
	//�ܼ�
	/*if( m_ScoreInfo.cbFanCount > 0 )
	{
		DCBuffer.DrawText( TEXT("�ܼ�"),lstrlen(TEXT("�ܼ�")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbFanCount );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
		DCBuffer.SetTextColor( RGB(250,250,250) );
		DCBuffer.DrawText( CString(TEXT("��")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}*/

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//�����Ϣ
void CGameScore::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//��������
BOOL CGameScore::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
