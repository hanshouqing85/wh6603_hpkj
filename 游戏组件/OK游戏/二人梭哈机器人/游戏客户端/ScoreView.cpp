#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//��������
	m_lTax=0L;
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//������Դ
	m_ImageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_GAME_END);
	//������Դ������
	m_pSkinResourceManager= (CSkinResourceManager *)CSkinResourceManager::GetInstance();
	ASSERT(m_pSkinResourceManager!=NULL);
}

//��������
CScoreView::~CScoreView(void)
{
}

//������Ϣ
int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	m_lTax=0L;
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextAlign(TA_CENTER);
	dc.SetTextColor(RGB(250,250,250));
	dc.SelectObject(m_pSkinResourceManager->GetDefaultFont());

	//�滭����
	m_ImageBack.BitBlt(dc,0,0);

	//��ʾ����
	WORD wStation=0;
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		if (m_szUserName[i][0]!=0)
		{
			dc.TextOut(106,(int)(wStation*25+85),m_szUserName[i]);
			myprintf(szBuffer,sizeof(szBuffer),_T("%I64d"),m_lScore[i]);
			dc.TextOut(232,(int)(wStation*25+85),szBuffer,lstrlen(szBuffer));
			wStation++;
		}
	}

// 	//��ʾ˰��
// 	if (wStation>0)
// 	{
// 		LPCTSTR pszTax=TEXT("����˰");
// 		dc.TextOut(106,(int)(wStation*25+85),pszTax,lstrlen(pszTax));
// 		myprintf(szBuffer,sizeof(szBuffer),_T("%I64d"),m_lTax);
// 		dc.TextOut(232,(int)(wStation*25+85),szBuffer,lstrlen(szBuffer));
// 	}

	return;
}

//���û���
void CScoreView::ResetScore()
{
	//��������
	m_lTax=0L;
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//�滭��ͼ
	Invalidate(NULL);
	
	return;
}

//����˰��
void CScoreView::SetTax(__int64 lTax)
{
	if (m_lTax!=lTax)
	{
		m_lTax=lTax;
		Invalidate(NULL);
	}
	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, __int64 lScore)
{
	if (wChairID<CountArray(m_lScore))
	{
		m_lScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}
	return;
}


//�����Ϣ
void CScoreView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );

}
//////////////////////////////////////////////////////////////////////////
