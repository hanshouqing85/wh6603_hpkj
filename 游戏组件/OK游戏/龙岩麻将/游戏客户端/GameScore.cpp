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
	////CImageHandle ImageHandle(&m_ImageBack);

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

	//////��������
	////LOGFONT lg;
	////ZeroMemory( &lg,sizeof(lg) );
	////lg.lfCharSet = GB2312_CHARSET;
	////strcpy( lg.lfFaceName,TEXT("����") );
	////lg.lfHeight = -20;
	////if( !m_InfoFont.CreateFontIndirect( &lg ) )
	////	m_InfoFont.Attach(CSkinResourceManager::GetDefaultFont());

	//����͸��
	ModifyStyleEx(0,0x00080000);
	HINSTANCE hInst = LoadLibrary(TEXT("User32.DLL")); 
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
	////CImageHandle ImageHandle(&m_ImageBack);
	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(DCBuffer,0,0);

	//λ�ñ���
	int nCardSpace=5;
	int nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
	int nTotalWidth=m_cbWeaveCount*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount+nCardSpace;

	//����λ��
	int nYCardPos=140-g_CardResource.m_ImageTableBottom.GetViewHeight();
	int nXCardPos=(rcClient.Width()-nTotalWidth)/2;

	//�滭���
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		//�滭�˿�
		m_WeaveCard[i].DrawCardControl(&DCBuffer,nXCardPos,nYCardPos,false);

		//����λ��
		nXCardPos+=(nCardSpace+nItemWidth*3);
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
			rcDraw.SetRect(15,148,190,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageFangPao.DrawImage( &DCBuffer,205,150 );
			rcDraw.SetRect(295,148,482,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );
		}
		//����
		else
		{
			rcDraw.SetRect(15,148,190,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageZiMo.DrawImage( &DCBuffer,215,150 );
		}
	}

	//�÷���Ϣ
	DCBuffer.SetTextColor( RGB(250,250,250) );
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		CRect rcDrawPlayer(100,183+i*26,275,210+i*26);
		//���
		DCBuffer.DrawText( m_ScoreInfo.szUserName[i],lstrlen(m_ScoreInfo.szUserName[i]),
			&rcDrawPlayer,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );

		//����
		CRect rcDrawScore(265,183+i*26,355,210+i*26);
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+I64d"),m_ScoreInfo.lGameScore[i] );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore,DT_SINGLELINE|DT_BOTTOM|DT_RIGHT|DT_END_ELLIPSIS );

		DCBuffer.SetTextColor( RGB(250,250,250) );
		CRect rcDrawFen(360,183+i*26,512,210+i*26);
		DCBuffer.DrawText( CString(TEXT("��")),&rcDrawFen,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}

	//������Ϣ
	LPCTSTR pszKind[] = {
		TEXT("����"),TEXT("������"),TEXT("����"),TEXT("����"),TEXT("����"),TEXT("ȱһ��"),TEXT("����"),TEXT("�۾ſ�"),TEXT("���ٸ�"),
		TEXT("����"),TEXT("ϲ���"),TEXT("һ���"),TEXT("����"),TEXT("����"),TEXT("˫����"),TEXT("˫ͬ��"),
		TEXT("�Ĺ�һ"),TEXT("ƽ��"),TEXT("��ǰ��"),TEXT("�ŷ��"),TEXT("Ȧ���"),TEXT("����"),TEXT("����"),TEXT("˫����"),TEXT("������"),
		TEXT("ȫ����"),TEXT("˫����"),TEXT("˫����"),TEXT("ȫ����"),TEXT("������"),TEXT("��ɫ������"),TEXT("��һɫ"),TEXT("������"),TEXT("���ܺ�"),
		TEXT("���Ͽ���"),TEXT("��������"),TEXT("���ֻش�"),TEXT("�޷���"),TEXT("��ɫ���ڸ�"),TEXT("��ɫ��ͬ˳"),TEXT("�Ʋ���"),TEXT("����"),TEXT("�����"),
		TEXT("С����"),TEXT("������"),TEXT("�����"),TEXT("ȫ����"),TEXT("������"),TEXT("��ͬ��"),TEXT("ȫ����"),TEXT("һɫ������"),TEXT("��ɫ˫����"),
		TEXT("����"),TEXT("ȫС"),TEXT("ȫ��"),TEXT("ȫ��"),TEXT("һɫ���ڸ�"),TEXT("һɫ��ͬ˳"),TEXT("��һɫ"),TEXT("ȫ˫"),TEXT("���ǲ���"),
		TEXT("�߶�"),TEXT("���۾�"),TEXT("����"),TEXT("һɫ�Ĳ���"),TEXT("һɫ�Ľڸ�"),TEXT("һɫ��ͬ˳"),TEXT("һɫ˫����"),TEXT("�İ���"),TEXT("��һɫ"),
		TEXT("С��Ԫ"),TEXT("С��ϲ"),TEXT("���۾�"),TEXT("ʮ����"),TEXT("���߶�"),TEXT("�ĸ�"),TEXT("��������"),TEXT("��һɫ"),TEXT("����Ԫ"),
		TEXT("����ϲ")
	};
	//����
	BYTE pcbFanCount[] = {
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,
		2,2,2,2,2,2,2,2,2,
		4,6,6,6,6,6,6,6,8,
		8,8,8,8,8,8,8,8,12,
		12,12,12,12,16,16,16,16,16,
		16,24,24,24,24,24,24,24,24,
		24,32,32,32,48,48,64,64,64,
		64,64,64,88,88,88,88,88,88,
		88
	};
	//����Ȩλ
	DWORD pdwRight[] = {
		CHR_ZI_MO,CHR_DAN_DIAO_JIANG,CHR_KAN_ZHANG,CHR_BIAN_ZHANG,CHR_WU_ZI,CHR_QUE_YI_MEN,CHR_MING_GANG,CHR_YAO_JIU,CHR_LAO_SHAO_JIANG,CHR_LIAN_LIU,CHR_XI_XIANG_FENG,
		CHR_YI_BAN_GAO,CHR_DUAN_YAO,CHR_AN_GANG,CHR_SHUANG_AN_KE,CHR_SHUANG_TONG_KE,CHR_SHI_GUI_YI,CHR_PING_HU,CHR_MEN_QI_QING,CHR_MEN_FENG_KE,CHR_QUAN_FENG_KE,CHR_JIAN_KE,
		CHR_JUE_ZHANG,CHR_SHUANG_MING_GANG,CHR_BU_QIU_REN,CHR_QUAN_DAI_YAO,CHR_SHUANG_JIAN_KE,CHR_SHUANG_AN_GANG,CHR_QUAN_QIU_REN,CHR_WU_MEN_QI,CHR_SAN_SE_BU,
		CHR_HUN_YI_SE,CHR_PENG_PENG,CHR_QIANG_GANG,CHR_GANG_KAI,CHR_HAI_DI,CHR_MIAO_SHOU,CHR_WU_FAN,CHR_SAN_SE_JIE,CHR_SAN_SE_TONG,CHR_TUI_BU_DAO,CHR_HUA_LONG,
		CHR_SAN_FENG_KE,CHR_XIAO_WU,CHR_DA_WU,CHR_ZU_HE_LONG,CHR_QUAN_BU_KAO,CHR_SAN_AN_KE,CHR_SAN_TONG_KE,CHR_QUAN_DAI_WU,CHR_YI_SE_BU,CHR_SAN_SE_SHUANG,CHR_QING_LONG,
		CHR_QUAN_XIAO,CHR_QUAN_ZHONG,CHR_QUAN_DA,CHR_YI_SE_JIE,CHR_YI_SE_TONG,CHR_QING_YI_SE,CHR_QUAN_SHUANG,CHR_QI_XING_BU_KAO,CHR_QI_DUI,CHR_HUN_YAO,CHR_SAN_GANG,
		CHR_YI_SHI_BU,CHR_YI_SHI_JIE,CHR_YI_SHI_TONG,CHR_YI_SE_SHUANG,CHR_SI_AN_KE,CHR_ZI_YI_SE,CHR_XIAO_SAN_YUAN,CHR_XIAO_SHI_XI,CHR_QING_YAO_JIU,CHR_SHI_SAN_YAO,CHR_LIAN_QI_DUI,
		CHR_SI_GANG,CHR_JIU_LIAN_DENG,CHR_LU_YI_SE,CHR_DA_SAN_YUAN,CHR_DA_SHI_XI
	};
	//��������
	CRect rcFanXing(34,301,159,327);
	//��������
	CRect rcFanShu(160,301,190,327);
	//������
	CRect rcFan(195,301,215,327);
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
	if( m_ScoreInfo.cbHuaCardCount > 0 )
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
	}
	//�ܼ�
	if( m_ScoreInfo.cbFanCount > 0 )
	{
		DCBuffer.DrawText( TEXT("�ܼ�"),lstrlen(TEXT("�ܼ�")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbFanCount );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
		DCBuffer.SetTextColor( RGB(250,250,250) );
		DCBuffer.DrawText( CString(TEXT("��")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}

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
