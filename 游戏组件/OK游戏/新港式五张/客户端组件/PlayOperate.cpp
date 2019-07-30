#include "StdAfx.h"
#include "resource.h"
#include ".\playoperate.h"


BEGIN_MESSAGE_MAP(CEditOperate, CEdit)
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CEditOperate::CEditOperate()
{
	m_lMaxScore = 0;
	m_lMinScore = 0;
	m_Brush.CreateSolidBrush(RGB(0,0,0));
}

CEditOperate::~CEditOperate()
{
	m_Brush.DeleteObject();
}


HBRUSH CEditOperate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	return m_Brush;
}

HBRUSH CEditOperate::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	return m_Brush;
}

void CEditOperate::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	TCHAR ch = TCHAR(nChar);

	// 对当前输入字符进行判断
	if ( ch != '\b' && ch != '0' && ch != '1' && ch != '2' && ch != '3' && ch != '4' && ch != '5' && ch != '6' && ch != '7' && ch != '8' && ch != '9' )
		return;

	CEdit::OnChar(nChar, nRepCnt, nFlags);

	CString str;
	GetWindowText(str); 
	CString strScore;
	if( !str.IsEmpty() )
	{
		while( str.GetAt(0) == '0' )
		{
			str.Delete(0);
		}

		SCORE lScore = _tcstod(str,NULL);
		if( lScore < 0 )
		{
			SetWindowText(TEXT("0"));
		}
		else if ( lScore < m_lMinScore )
		{
			strScore.Format(TEXT("")SCORE_STRING,m_lMinScore);
			SetWindowText(strScore);
		}
		else if ( lScore > m_lMaxScore )
		{
			strScore.Format(TEXT("")SCORE_STRING,m_lMaxScore);
			SetWindowText(strScore);
		}
		else
		{
			strScore.Format(TEXT("")SCORE_STRING,lScore);
			SetWindowText(strScore);
		}
	}
	m_pPlayOperate->SetDragScore();
}

void CEditOperate::OnRButtonDown(UINT nFlags, CPoint point)
{
}


void CEditOperate::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( nChar == VK_DELETE )
		return;
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////
// 玩家操作
CPlayOperate::CPlayOperate(void)
{
	m_bShow = false;
	m_bMouseDown = false;
	m_lCellScore = 0;
	m_ptPos.SetPoint( 0, 0 );
	m_sizeBakc.SetSize( 0, 0 );
	m_fontScoreInput.CreateFont(12,0,0,0,800,0,0,0,0,0,0,0,0,TEXT("宋体"));
	m_edieScoreInput.m_pPlayOperate = this;
}

CPlayOperate::~CPlayOperate(void)
{
	m_fontScoreInput.DeleteObject();
}

// 初始化
void CPlayOperate::Initialization( CWnd* pParent )
{
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageBack.LoadImage( hResInstance, TEXT("USER_CONTROL"));
	m_ImageDrag.LoadImage( hResInstance, TEXT("ARROW_CONTROL"));
	
	m_sizeBakc.SetSize( m_ImageBack.GetWidth(), m_ImageBack.GetHeight() );
	m_sizeDrag.SetSize( m_ImageDrag.GetWidth(), m_ImageDrag.GetHeight() );

	CRect rcCreate(0,0,0,0);
	m_btOneScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE_ONE);
	m_btTwoScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE_TWO);
	m_btThreeScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE_THREE);
	m_btFourScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE_FOUR);
	m_btAddScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE);
	m_btFollow.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_CONTROL_FOLLOW);
	m_btGiveUp.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_CONTROL_GIVEUP);
	m_btShowHand.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_CONTROL_SHOWHAND);
	m_btADD.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_CONTROL_ADD);
	m_btSUB.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_CONTROL_SUB);

	m_btOneScore.SetButtonImage(IDB_BT_DEF1,hResInstance,false,false);
	m_btTwoScore.SetButtonImage(IDB_BT_DEF2,hResInstance,false,false);
	m_btThreeScore.SetButtonImage(IDB_BT_DEF3,hResInstance,false,false);
	m_btFourScore.SetButtonImage(IDB_BT_DEF4,hResInstance,false,false);
	
	m_btAddScore.SetButtonImage(IDB_BT_CONTROL_CENTER,hResInstance,false,false);
	m_btFollow.SetButtonImage(IDB_BT_CONTROL_FOLLOW,hResInstance,false,false);
	m_btGiveUp.SetButtonImage(IDB_BT_CONTROL_GIVEUP,hResInstance,false,false);
	m_btShowHand.SetButtonImage(IDB_BT_CONTROL_SHOWHAND,hResInstance,false,false);
	m_btADD.SetButtonImage(IDB_BT_CONTROL_ADD,hResInstance,false,false);
	m_btSUB.SetButtonImage(IDB_BT_CONTROL_SUB,hResInstance,false,false);
	
	m_btOneScore.SetButtonColor(RGB(250,250,0));
	m_btTwoScore.SetButtonColor(RGB(250,250,0));
	m_btThreeScore.SetButtonColor(RGB(250,250,0));
	m_btFourScore.SetButtonColor(RGB(250,250,0));

	m_btAddScore.SetFont(&m_fontScoreInput);
	m_btOneScore.SetFont(&m_fontScoreInput);
	m_btTwoScore.SetFont(&m_fontScoreInput);
	m_btThreeScore.SetFont(&m_fontScoreInput);
	m_btFourScore.SetFont(&m_fontScoreInput);

	// 输入框
	m_edieScoreInput.Create( WS_CHILD|ES_AUTOHSCROLL, CRect(0,0,0,0), pParent, 2021 );
	m_edieScoreInput.SetFont(&m_fontScoreInput);
}



// 绘画函数
void CPlayOperate::DrawPlayOperate( CWnd* pWnd,CDC * pDC, INT nWidth, INT nHeight )
{
	if ( !m_bShow )
		return;

	m_ImageBack.DrawImage( pDC, m_ptPos.x, m_ptPos.y );

	int nDragX = m_lDragSite;
	int nOffSet = m_sizeDrag.cx / 2;
	if ( nDragX < m_rectDrag.left + nOffSet )
		nDragX = m_rectDrag.left + nOffSet;
	else if ( nDragX > m_rectDrag.right - nOffSet )
		nDragX = m_rectDrag.right - nOffSet;
	m_ImageDrag.DrawImage( pDC, nDragX - nOffSet, m_rectDrag.top );

	SCORE lCurrentScore=GetBetCount();
	TCHAR szbuff[128] = TEXT("");
	_sntprintf(szbuff,CountArray(szbuff),TEXT("")SCORE_STRING,lCurrentScore);
	//UINT old=pDC->SetTextAlign(TA_LEFT);
	//COLORREF cl=pDC->SetTextColor(RGB(255,255,0));
	//CFont *oldfont = pDC->SelectObject(&m_fontScoreInput);
	//pDC->TextOut( m_ptPos.x+20, m_ptPos.y+121,szbuff);
	CRect rc (m_ptPos.x+20, m_ptPos.y+121,m_ptPos.x+120, m_ptPos.y+137);
	CDFontEx::DrawText(pWnd, pDC, 12, 500,szbuff,&rc, RGB(255,255,0), DT_LEFT|DT_END_ELLIPSIS);


	//pDC->SetTextAlign(old);
	//pDC->SetTextColor(cl);
	//pDC->SelectObject(oldfont);
}

// 设置位置
void CPlayOperate::SetOperatePos( int nPosX, int nPosY, UINT nFlags )
{
	if ( PYOE_RIGHT&nFlags )
		m_ptPos.x = nPosX - m_sizeBakc.cx;
	else if ( PYOE_CENTER&nFlags )
		m_ptPos.x = nPosX - m_sizeBakc.cx/2;
	else
		m_ptPos.x = nPosX;


	if ( PYOE_BOTTOM&nFlags )
		m_ptPos.y = nPosY - m_sizeBakc.cy;
	else if ( PYOE_VCENTER&nFlags )
		m_ptPos.y = nPosY - m_sizeBakc.cy/2;
	else
		m_ptPos.y = nPosY;

	//按钮控件
	CRect rcButton;
	m_btOneScore.GetWindowRect(&rcButton);

	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	DeferWindowPos( hDwp, m_btOneScore,  NULL, nPosX + 10, nPosY + 11, 0, 0, uFlags );
	DeferWindowPos( hDwp, m_btTwoScore,  NULL, nPosX + 10, nPosY + 13 + rcButton.Height(), 0, 0, uFlags );
	DeferWindowPos( hDwp, m_btThreeScore,NULL, nPosX + 10, nPosY + 15 + rcButton.Height() * 2, 0, 0, uFlags );
	DeferWindowPos( hDwp, m_btFourScore, NULL, nPosX + 10, nPosY + 17 + rcButton.Height() * 3, 0, 0, uFlags );

	DeferWindowPos( hDwp, m_btAddScore, NULL, nPosX + m_sizeBakc.cx-55, nPosY+m_sizeBakc.cy-65, 0, 0, uFlags );

	m_btFollow.GetWindowRect(&rcButton);
	DeferWindowPos( hDwp, m_btFollow,   NULL, nPosX + 98, nPosY + 10 , 0, 0, uFlags );
	DeferWindowPos( hDwp, m_btShowHand, NULL, nPosX + 98, nPosY + 10 +rcButton.Height()+2, 0, 0, uFlags );
	DeferWindowPos( hDwp, m_btGiveUp,   NULL, nPosX + 98, nPosY + 10 +rcButton.Height()*2+4, 0, 0, uFlags );
	


	DeferWindowPos( hDwp, m_btADD, NULL, nPosX + 110, nPosY + 143 , 0, 0, uFlags );
	DeferWindowPos( hDwp, m_btSUB, NULL, nPosX + 10, nPosY + 143 , 0, 0, uFlags );

	//结束移动
	EndDeferWindowPos(hDwp);


	CRect rcButtonOver;
	m_btOneScore.GetWindowRect(&rcButtonOver);
	m_lDragSite += rcButtonOver.left - rcButton.left;

	m_edieScoreInput.MoveWindow( nPosX + 32, nPosY + 136, 118, 12);

	m_rectDrag.SetRect( nPosX + 25, nPosY + 138, nPosX + 110, nPosY + 158 );
}

// 设置信息
void CPlayOperate::SetCellScore( SCORE lCellScore )
{
	m_lCellScore = lCellScore;

	CString str;
	str.Format(TEXT("")SCORE_STRING, lCellScore);
	m_btOneScore.SetWindowText(str);

	str.Format(TEXT("")SCORE_STRING, lCellScore * 2);
	m_btTwoScore.SetWindowText(str);

	str.Format(TEXT("")SCORE_STRING, lCellScore * 3);
	m_btThreeScore.SetWindowText(str);

	str.Format(TEXT("")SCORE_STRING, lCellScore * 4);
	m_btFourScore.SetWindowText(str);
}

// 设置最大数
void CPlayOperate::SetBetCount(  SCORE lMaxBetCount, SCORE lMinBetCount  )
{
	m_edieScoreInput.m_lMaxScore = lMaxBetCount;
	m_edieScoreInput.m_lMinScore = lMinBetCount;

	if ( m_edieScoreInput.m_lMaxScore < SCORE_ZERO )
		m_edieScoreInput.m_lMaxScore = SCORE_ZERO;

	if ( m_edieScoreInput.m_lMinScore < SCORE_ZERO )
		m_edieScoreInput.m_lMinScore = SCORE_ZERO;

	if ( m_edieScoreInput.m_lMinScore > m_edieScoreInput.m_lMaxScore )
		m_edieScoreInput.m_lMinScore = m_edieScoreInput.m_lMaxScore;

	if ( m_lCellScore > lMaxBetCount )
	{
		m_btOneScore.EnableWindow(FALSE);
	}
	else
	{
		CString str;
		str.Format(TEXT("")SCORE_STRING, m_lCellScore);
		m_btOneScore.SetWindowText(str);
		m_btOneScore.EnableWindow(TRUE);
	}
	

	if ( m_lCellScore * 2 > lMaxBetCount )
	{
		m_btTwoScore.EnableWindow(FALSE);
	}
	else
	{
		CString str;
		str.Format(TEXT("")SCORE_STRING, m_lCellScore * 2);
		m_btTwoScore.SetWindowText(str);
		m_btTwoScore.EnableWindow(TRUE);
	}

	if ( m_lCellScore * 5 > lMaxBetCount )
	{
		m_btThreeScore.EnableWindow(FALSE);
	}
	else
	{
		CString str;
		str.Format(TEXT("")SCORE_STRING, m_lCellScore * 5);
		m_btThreeScore.SetWindowText(str);
		m_btThreeScore.EnableWindow(TRUE);
	}

	if ( m_lCellScore * 10 > lMaxBetCount )
	{
		m_btFourScore.EnableWindow(FALSE);
	}
	else
	{
		CString str;
		str.Format(TEXT("")SCORE_STRING, m_lCellScore * 10);
		m_btFourScore.SetWindowText(str);
		m_btFourScore.EnableWindow(TRUE);
	}

	if ( m_edieScoreInput.m_lMinScore >0.000001|| m_edieScoreInput.m_lMinScore <-0.000001)
	{
		CString str;
		str.Format(TEXT("")SCORE_STRING, m_edieScoreInput.m_lMinScore);
		m_edieScoreInput.SetWindowText(str);
	}
}

// 获取下载数量
SCORE CPlayOperate::GetBetCount()
{
	CString str;
	m_edieScoreInput.GetWindowText(str);
	SCORE lScore = (_tcstod(str,NULL));

	if ( lScore < m_edieScoreInput.m_lMinScore )
	{
		lScore = m_edieScoreInput.m_lMinScore;
	}	
	else if( lScore> m_edieScoreInput.m_lMaxScore)
	{
		lScore = m_edieScoreInput.m_lMaxScore;
	}
	return lScore;
}

// 获取位置
CRect CPlayOperate::GetOperateRect()
{
	return CRect( m_ptPos.x, m_ptPos.y, m_ptPos.x + m_sizeBakc.cx, m_ptPos.y + m_sizeBakc.cy);
}


// 显示函数
void CPlayOperate::ShowOperate( bool bShow )
{
	m_edieScoreInput.SetWindowText(TEXT(""));
	m_bShow = bShow;

	if ( !m_bShow )
	{
		m_btOneScore.ShowWindow(SW_HIDE);
		m_btTwoScore.ShowWindow(SW_HIDE);
		m_btThreeScore.ShowWindow(SW_HIDE);
		m_btFourScore.ShowWindow(SW_HIDE);
		m_btAddScore.ShowWindow(SW_HIDE);
		m_btFollow.ShowWindow(SW_HIDE);
		m_btGiveUp.ShowWindow(SW_HIDE);
		m_btShowHand.ShowWindow(SW_HIDE);

		m_btADD.ShowWindow(SW_HIDE);
		m_btSUB.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btADD.ShowWindow(SW_SHOW);
		m_btSUB.ShowWindow(SW_SHOW);
		m_btOneScore.ShowWindow(SW_SHOW);
		m_btTwoScore.ShowWindow(SW_SHOW);
		m_btThreeScore.ShowWindow(SW_SHOW);
		m_btFourScore.ShowWindow(SW_SHOW);
		m_lDragSite = m_rectDrag.left;
	}
	m_edieScoreInput.ShowWindow(SW_HIDE);
	m_edieScoreInput.EnableWindow(FALSE);
}

BOOL CPlayOperate::PreTranslateMessage( MSG * pMsg )
{
	if ( pMsg->message == WM_LBUTTONDOWN )
	{
		CPoint ptMouse(LOWORD(pMsg->lParam) , HIWORD(pMsg->lParam));
		if( m_rectDrag.PtInRect(ptMouse) )
		{
			m_bMouseDown = true;
			m_lDragSite = ptMouse.x;
			SetEditScore();
			return TRUE;
		}
	}
	else if ( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint ptMouse(LOWORD(pMsg->lParam) , HIWORD(pMsg->lParam));
		if ( m_bMouseDown && m_rectDrag.PtInRect(ptMouse) )
		{
			m_lDragSite = ptMouse.x;
			SetEditScore();
			return TRUE;
		}
	}
	else if ( pMsg->message == WM_LBUTTONUP || pMsg->message == WM_KILLFOCUS )
	{
		m_bMouseDown = false;
	}
	
	return FALSE;
}


//	按键消息
BOOL	CPlayOperate::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch( nChar ) {


		case VK_LEFT :
		case VK_UP :

			ScoreSUB( m_lCellScore);
			return TRUE;


		case VK_RIGHT :
		case VK_DOWN :

			ScoreADD( m_lCellScore );
			return TRUE;
		}
	return FALSE;
}

BOOL CPlayOperate::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CONTROL_ADD:
		{
			ScoreADD(m_lCellScore);
			return TRUE;
		}
	case IDC_CONTROL_SUB:
		{
			ScoreSUB(m_lCellScore);
			return TRUE;
		}
	case IDC_ADD_SCORE:
		{
			SCORE sFollowCount = GetBetCount();
			CGameFrameView::GetInstance()->SendEngineMessage(IDM_FOLLOW, (WPARAM)&sFollowCount, 0);
			return TRUE;
		}
	case IDC_CONTROL_FOLLOW:
		{
			CGameFrameView::GetInstance()->SendEngineMessage(IDM_FOLLOW, 0, 0);
			return TRUE;
		}
	case IDC_CONTROL_GIVEUP:
		{
			CGameFrameView::GetInstance()->SendEngineMessage(IDM_GIVE_UP,0,0);
			return TRUE;
		}
	case IDC_CONTROL_SHOWHAND:
		{
			CGameFrameView::GetInstance()->SendEngineMessage(IDM_SHOW_HAND,0,0);
			return TRUE;
		}
	case IDC_ADD_SCORE_ONE:
		{
			if( m_lCellScore > 0.000001 )
			{
				SCORE sFollowCount = m_lCellScore/* + 0.001f*/;
				CGameFrameView::GetInstance()->SendEngineMessage(IDM_FOLLOW, (WPARAM)&sFollowCount, 0);
			}
			return TRUE;
		}
	case IDC_ADD_SCORE_TWO:
		{
			if( m_lCellScore > 0.000001 )
			{
				SCORE sFollowCount = m_lCellScore*2/* + 0.001f*/;
				CGameFrameView::GetInstance()->SendEngineMessage(IDM_FOLLOW, (WPARAM)&sFollowCount, 0);
			}
			return TRUE;
		}
	case IDC_ADD_SCORE_THREE:
		{
			if( m_lCellScore > 0.000001 )
			{
				SCORE sFollowCount = m_lCellScore*5/* + 0.001f*/;
				CGameFrameView::GetInstance()->SendEngineMessage(IDM_FOLLOW, (WPARAM)&sFollowCount, 0);
			}
			return TRUE;
		}
	case IDC_ADD_SCORE_FOUR:
		{
			if( m_lCellScore > 0.000001 )
			{
				SCORE sFollowCount = m_lCellScore*10/* + 0.001f*/;
				CGameFrameView::GetInstance()->SendEngineMessage(IDM_FOLLOW, (WPARAM)&sFollowCount, 0);
			}
			return TRUE;
		}
	}
	return FALSE;
}
// 设置积分
void CPlayOperate::SetEditScore()
{
	int nWidth = m_rectDrag.Width() - m_sizeDrag.cx;
	int nOffSet = m_sizeDrag.cx / 2;
	int nDragX = m_lDragSite;
	if ( nDragX < m_rectDrag.left + nOffSet )
		nDragX = m_rectDrag.left + nOffSet;
	else if ( nDragX > m_rectDrag.right - nOffSet )
		nDragX = m_rectDrag.right - nOffSet;

	nDragX -= m_rectDrag.left + nOffSet;

	SCORE lCurrentScore = SCORE_ZERO;
	lCurrentScore = KeepOne(((m_edieScoreInput.m_lMaxScore - m_edieScoreInput.m_lMinScore) * ( (DOUBLE)nDragX/(DOUBLE)nWidth )) + m_edieScoreInput.m_lMinScore);
	
	if ( lCurrentScore < m_edieScoreInput.m_lMinScore )
	{
		lCurrentScore = m_edieScoreInput.m_lMinScore;
	}	
	else if( lCurrentScore> m_edieScoreInput.m_lMaxScore)
	{
		lCurrentScore = m_edieScoreInput.m_lMaxScore;
	}
	CString str;
	str.Format(TEXT("")SCORE_STRING,lCurrentScore);
	m_edieScoreInput.SetWindowText(str);

}

// 设置积分
void CPlayOperate::SetDragScore()
{
	CString str;
	m_edieScoreInput.GetWindowText(str);
	SCORE lScore = _tcstod(str,NULL);

	if ( lScore < m_edieScoreInput.m_lMinScore )
	{
		lScore = m_edieScoreInput.m_lMinScore;
	}	
	else if( lScore> m_edieScoreInput.m_lMaxScore)
	{
		lScore = m_edieScoreInput.m_lMaxScore;
	}

	str.Format(TEXT("")SCORE_STRING,lScore);
	m_edieScoreInput.SetWindowText(str);

	lScore -= m_edieScoreInput.m_lMinScore;
	int nWidth = m_rectDrag.Width() - m_sizeDrag.cx;
	int nDragX = (int)( ((DOUBLE)lScore / (DOUBLE)(m_edieScoreInput.m_lMaxScore - m_edieScoreInput.m_lMinScore)) * nWidth);
	
	nDragX += m_rectDrag.left + m_sizeDrag.cx / 2;
	m_lDragSite = nDragX;
}

// 设置积分
void CPlayOperate::ScoreADD()
{
	CString str;
	m_edieScoreInput.GetWindowText(str);
	SCORE lScore = _tcstod(str,NULL);

	if( lScore+m_lCellScore < m_edieScoreInput.m_lMaxScore )
		lScore+=m_lCellScore;
	
	str.Format(TEXT("")SCORE_STRING,lScore);

	m_edieScoreInput.SetWindowText(str);

	SetDragScore();
}
// 设置积分
void CPlayOperate::ScoreADD(SCORE score)
{
	CString str;
	m_edieScoreInput.GetWindowText(str);
	SCORE lScore = _tcstod(str,NULL);

	if( lScore+score <= m_edieScoreInput.m_lMaxScore )
		lScore+=score;
	else
		lScore = m_edieScoreInput.m_lMaxScore;

	str.Format(TEXT("")SCORE_STRING,lScore);

	m_edieScoreInput.SetWindowText(str);

	SetDragScore();
}

// 设置积分
void CPlayOperate::ScoreSUB()
{
	CString str;
	m_edieScoreInput.GetWindowText(str);
	SCORE lScore = _tcstod(str,NULL);

	if( lScore > 0.000001 )
		lScore--;
	
	str.Format(TEXT("")SCORE_STRING,lScore);
	m_edieScoreInput.SetWindowText(str);

	SetDragScore();
}
void CPlayOperate::ScoreSUB(SCORE score)
{
	CString str;
	m_edieScoreInput.GetWindowText(str);
	SCORE lScore = _tcstod(str,NULL);

	if( lScore-score > 0.000001 )
		lScore-=score;
	else
		lScore = m_edieScoreInput.m_lMinScore;

	str.Format(TEXT("")SCORE_STRING,lScore);
	m_edieScoreInput.SetWindowText(str);

	SetDragScore();
}
// 添加逗号
CString CPlayOperate::AddComma( LONGLONG lScore )
{
	CString strScore;
	CString strReturn;
	strScore.Format(TEXT("%I64d"), lScore);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%4) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}
	return strReturn;
}

// 添加逗号
CString CPlayOperate::AddComma( CString strScore )
{
	CString strReturn;
	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%4) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}
	return strReturn;
}

// 删除逗号
LONGLONG CPlayOperate::DeleteComma( CString strScore )
{
	LONGLONG lScore = 0l;
	strScore.Remove(',');
	if ( !strScore.IsEmpty() )
		lScore = _ttoi64(strScore);

	return lScore;
}





