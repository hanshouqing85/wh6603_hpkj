#include "Stdafx.h"
#include "GameScore.h"
#include "math.h"
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
	m_ImageBack.LoadFromResource( hInst, IDB_GAME_SCORE );
	m_ImageFangPao.LoadImage( hInst,TEXT("FANG_PAO") );
	m_ImageZiMo.LoadImage( hInst,TEXT("ZI_MO") );
	m_ImageHuangZhuang.LoadFromResource(hInst,IDB_HUANG_ZHUANG);
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
//	if( !m_InfoFont.CreateFontIndirect( &lg ) )
	//	m_InfoFont.Attach(CSkinResourceManager::GetDefaultFont());

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


	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(DCBuffer,0,0);

	//λ�ñ���
	int nCardSpace=5;
	int nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
	int nTotalWidth=m_cbWeaveCount*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount+nCardSpace;

	//����λ��
	int nYCardPos=140-g_CardResource.m_ImageTableBottom.GetViewHeight();
	int nXCardPos=(rcClient.Width()-nTotalWidth)/2;
	TCHAR szBuffer[32] = TEXT("");

	//�б���Ϣ
	DCBuffer.SetTextColor( RGB(0,255,255) );
	INT nY=270;
	DCBuffer.TextOut(41+65,nY,TEXT("���"));
	//DCBuffer.TextOut(140,nY,TEXT("����"));
	//DCBuffer.TextOut(205,nY,TEXT("����"));
	//DCBuffer.TextOut(270,nY,TEXT("����"));
	DCBuffer.TextOut(345,nY,TEXT("�ܼ�"));
	//DCBuffer.TextOut(415,nY,TEXT("�ܼ�"));

	//������Ϣ
	if( m_ScoreInfo.wProvideUser != INVALID_CHAIR )
	{
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

		CRect rcDraw;
		//����
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
	for( WORD j = 0; j < GAME_PLAYER; j++ )
	{
		WORD i=(j+m_ScoreInfo.wMeChairID)%GAME_PLAYER;

		INT nY1=293+j*26;
		INT nY2=320+j*26;

		INT nX = 25+65;
		CRect rcDrawPlayer(nX,nY1,nX+175,nY2);
		//���
		DCBuffer.SetTextColor(( (i==m_ScoreInfo.wMeChairID)? RGB(252,255,0):RGB(252,255,0) ));
		DCBuffer.DrawText( m_ScoreInfo.szUserName[i],lstrlen(m_ScoreInfo.szUserName[i]),
			&rcDrawPlayer,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );

		////����
		//nX = 195/*130*/;
		//CRect rcDrawScore1(nX,nY1,nX+60,nY2);
		//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%ld"),m_ScoreInfo.lHuScore[i] );
		//DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore1,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		////��ǹ
		//nX = 195;
		//CRect rcDrawScore2(nX,nY1,nX+60,nY2);
		//if(m_ScoreInfo.lFanHuScore[i]!=0)
		//	_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+ld"),m_ScoreInfo.lFanHuScore[i] );
		//else _sntprintf( szBuffer,CountArray(szBuffer),TEXT("0"));
		//DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore2,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		////����
		//nX = 260;
		//CRect rcDrawScore3(nX,nY1,nX+60,nY2);
		//if(m_ScoreInfo.lFollowScore[i]!=0)
		//	_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+ld"),m_ScoreInfo.lFollowScore[i] );
		//else _sntprintf( szBuffer,CountArray(szBuffer),TEXT("0"));
		//DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore3,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		////������
		//nX = 335;
		//CRect rcDrawScore4(nX,nY1,nX+60,nY2);
		//if(m_ScoreInfo.lOutMagicScore[i]!=0)
		//	_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+ld"),m_ScoreInfo.lOutMagicScore[i] );
		//else _sntprintf( szBuffer,CountArray(szBuffer),TEXT("0"));
		//DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore4,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		//����
		nX = /*405*/335;
		CRect rcDrawScore(nX,nY1,nX+60,nY2);
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+ld"),m_ScoreInfo.lGameScore[i] );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore,DT_SINGLELINE|DT_BOTTOM|DT_CENTER );

		//nX += 65;
		//DCBuffer.SetTextColor( RGB(250,250,250) );
		//CRect rcDrawFen(nX,nY1,nX+152,nY2);
		//DCBuffer.DrawText( CString(TEXT("��")),&rcDrawFen,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}

	//������Ϣ
	if( m_ScoreInfo.wProvideUser != INVALID_CHAIR )
	{
		//������Ϣ
		LPCTSTR pszKind[] = {
			TEXT("�߶�"),TEXT("�����߶�"),TEXT("�������߶�"),TEXT("�������߶�"),
				TEXT("�ܿ�"),TEXT("�ܱ�"),TEXT("��ͷ"),TEXT("��Ʈ"),TEXT("��Ʈ"),TEXT("Ʈ��"),
		};
		//����Ȩλ
		DWORD pdwRight[] = {
			CHR_QI_DUI,CHR_QI_DUI1,CHR_QI_DUI2,CHR_QI_DUI3,
				CHR_GANG_KAI,CHR_GANG_KAI_MAGIC,CHR_MAGIC_HU,
				CHR_MAGIC_HU2,CHR_GANG_MAGIC,CHR_MAGIC_GANG,
		};

		//����
		BYTE pcbFanCount[] = {
			2,4,8,16,
			2,4,2,4,8,8,
		};

		//����
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
					strTemp=TEXT("(��Ʈ��)");
				}
				csCard.Format(TEXT("%s%s   %d����  "),pszKind[i],strTemp,cbTimer);
				csInfo+=csCard;
			}
		}
		DCBuffer.SetTextColor( RGB(252,255,255) );
		DCBuffer.TextOut( 35,195,csInfo);
	}

	//ׯ�� �ܱ���:
	if( m_ScoreInfo.wLandUser <GAME_PLAYER )
	{
		TCHAR szInfo[512]=TEXT("");
		_sntprintf(szInfo,sizeof(szInfo),TEXT("ׯ��:%s   ��ׯ:%d�� [%d��]   ��ע:%d"),m_ScoreInfo.szUserName[m_ScoreInfo.wLandUser],
			m_ScoreInfo.cbLandCount,LONG(powf(2,__min(m_ScoreInfo.cbLandCount,3))),m_ScoreInfo.lCellScore);
		DCBuffer.SetTextColor( RGB(252,255,255) );
		DCBuffer.TextOut( 35,225,szInfo);
	}

	//��ׯ��־
	if (m_ScoreInfo.cbHuangZhuang)
	{
		//CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.AlphaDrawImage(&DCBuffer,185,23,RGB(255,0,255));
	}

	////�ܼ�
	//if( m_ScoreInfo.cbFanCount > 0 )
	//{
	//	DCBuffer.DrawText( TEXT("�ܷ�"),lstrlen(TEXT("�ܷ�")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	//	_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbFanCount );
	//	DCBuffer.SetTextColor( RGB(252,255,0) );
	//	DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
	//	DCBuffer.SetTextColor( RGB(250,250,250) );
	//	DCBuffer.DrawText( CString(TEXT("��")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	//}

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
