// QMTab.cpp : implementation file
//

#include "stdafx.h"
//#include "QQTab.h"
#include "QMTab.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQMTab

CQMTab::CQMTab()
{
	m_arTabs.RemoveAll();
	m_nTabHeight = 25;
	m_nSelectedTab = -1;
	m_nFocusTab = -1;
	m_nMouseDownTab = -1;
	m_bMouseDown = FALSE;
	m_TextColor = QMTAB_CFG_DEFAULTTXTCOLOR;
	m_iconSize.cx = m_iconSize.cy = 0;
	m_uCallbackMessage = 0;

	m_fntTab.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体"));

	m_fntTabSel.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,				   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体"));

	m_pImgList = NULL;

	m_bitmapArrow.LoadBitmap( IDB_BITMAP_ARROW );
	m_arrowList.Create( QMTAB_CFG_ARROWWIDTH, QMTAB_CFG_ARROWHEIGHT, ILC_COLOR32|ILC_MASK, 4, 1 );
	m_arrowList.Add( &m_bitmapArrow, QMTAB_CFG_ARROW_MASKCOLOR );
	m_ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("BTN_TAB"));
	m_ImgTabBack.LoadImage(CBmpUtil::GetExePath() + _T("ADImage\\GameKindSmall\\tabback.png"));
}

CQMTab::~CQMTab()
{
	m_fntTab.DeleteObject();
	m_fntTabSel.DeleteObject();

	for(int i=0;i<m_arTabs.GetSize();i++)
	{
		_QMTAB_TAB *pTab  = m_arTabs.GetAt(i);
		if ( NULL != pTab ) {
			
			if ( pTab->item != NULL ) {
				DeleteSubItem( pTab->item );
			}

			delete pTab;
		}
	}
	m_arTabs.RemoveAll();

	m_bitmapArrow.DeleteObject();
	m_arrowList.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CQMTab, CWnd)
	//{{AFX_MSG_MAP(CQMTab)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()

	ON_WM_MOUSEWHEEL()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CQMTab message handlers

void CQMTab::DeleteSubItem( _QMTAB_ITEM *pItem )
{
	if ( pItem->pNext != NULL ) {
		DeleteSubItem( pItem->pNext );
	}

	pItem->pNext = NULL;
	delete pItem;
}

int CQMTab::AddTab(LPCTSTR sTabName,LPCTSTR sTabImg,int nIndex)
{
	_QMTAB_TAB *tab = new _QMTAB_TAB;
	memset( tab, 0, sizeof(_QMTAB_TAB) );
	CRect rcItem;
	
	//strcpy( tab->lpsTabName, sTabName );
	sprintf(tab->lpsTabName,"%s", sTabName);
	sprintf(tab->lpsTabImg,"%s", sTabImg);
	GetTabItemRect( m_arTabs.GetSize(), &rcItem );
	tab->rect = rcItem;
	tab->nImgIndex = nIndex;
	//tab->img->LoadImage(sTabImg);
	int n = m_arTabs.GetSize();
	m_arTabs.Add( tab );

	return ((n == m_arTabs.GetSize()) ? -1 : (m_arTabs.GetSize()-1));
}
BOOL  CQMTab:: OnMouseWheel(   UINT   nFlags,   short   zDelta,   CPoint   pt   )
{

	CRect rcClient;
	GetWindowRect(rcClient);
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	pt.x -= rcClient.left;
	pt.y -= rcClient.top;

	pt.x -= 7;

	int nItemID = GetEventItem( &pt );
	if ( nItemID == -1 ) 
	{
		CRect rc1, rc2;;
		GetDataAreaRect( &rc1, &rc2 );
		if ( rc2.PtInRect( pt ) ) 
		{
			CheckItemFocus( &pt );

			if (zDelta == 120)  {
				ArrowMoveUp();
			}
			else if ( zDelta == -120) 
			{
				ArrowMoveDown();
			}
			Invalidate( FALSE );
		}
	}
	return CWnd::OnMouseWheel(nFlags,zDelta,pt);
}
void CQMTab::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if ( 0 == m_arTabs.GetSize() ) {
		return;
	}

	if ( m_pImgList == NULL || m_pImgList->GetImageCount() == 0 ) {
		return;
	}

	CRect rcClient;
	GetClientRect( &rcClient );

	CDC MemDc;
	CBitmap BitmapDc, *pOldBitmap;

	MemDc.CreateCompatibleDC( &dc );
	BitmapDc.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	pOldBitmap = (CBitmap *)MemDc.SelectObject( &BitmapDc );
	MemDc.SetBkMode( TRANSPARENT );
	MemDc.FillSolidRect( rcClient, QMTAB_CFG_BACKGROUND );

	DrawItem( &MemDc );

	dc.BitBlt( 0, 0, rcClient.Width(), rcClient.Height(), &MemDc, 0, 0, SRCCOPY );

	MemDc.SelectObject( pOldBitmap );
	BitmapDc.DeleteObject();
	MemDc.DeleteDC();
}

BOOL CQMTab::CreateWnd(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, LPVOID lpParam)
{
	m_rcTab = rect;
	m_pParent = pParentWnd;

	m_rcTab.top=1;
	m_rcTab.bottom=rect.bottom-rect.top-1;
	m_rcTab.left=1;
	m_rcTab.right=rect.right-rect.left-1;

	m_rcClient = rect;

	m_ImgGroup.LoadImage(AfxGetInstanceHandle(),TEXT("BTN_FRAME"));

	return CWnd::CreateEx( dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam );
}

void CQMTab::DrawItem(CDC *pMemDc)
{
	//DrawFrame( pMemDc );
	DrawTabs( pMemDc );
	DrawItems( pMemDc );
}

void CQMTab::DrawItems(CDC *pMemDc)
{
	if ( m_nSelectedTab == -1 ) {
		return;
	}
	
	if ( NULL == m_pImgList ) {
		return;
	}

	int nWidth, nHeight;
	int nGroupItemCount, nPageItems, nTop;
	int i, j, k, m;
	CPoint ptImage;
	CRect rcItemText;
	CFont *pOldFont;
	CSize szText;
	CRect rc1, rc2, rcTmp;
	_QMTAB_ITEM *pItemTmp;

	GetDataAreaRect( &rc1, &rc2 );

	for( i=0;i<m_arTabs.GetSize();i++ )
	{
		_QMTAB_TAB *qTab = m_arTabs.GetAt( i );
		if ( !qTab ) {
			continue;
		}

		CDC pDc;
		CPen pen1, pen2, *pOldPen;
		pDc.CreateCompatibleDC( pMemDc );
		pen1.CreatePen( PS_SOLID, 2, RGB(255,255,255) );
		pen2.CreatePen( PS_SOLID, 2, RGB(106,106,106) );
		CBitmap bitmap, *pOldBitmap;
		nWidth = rc2.right-rc2.left;
		nHeight = rc2.bottom-rc2.top;
		nGroupItemCount = GetMaxTabItemCount();
		bitmap.CreateCompatibleBitmap( pMemDc, nWidth, nHeight );
		pOldBitmap = ( CBitmap * )pDc.SelectObject( &bitmap );
		pOldFont = ( CFont * )pDc.SelectObject( &m_fntTab );
//		szText = pDc.GetTextExtent( "TEST" );

		pDc.FillSolidRect( 0, 0, nWidth, nHeight, QMTAB_CFG_BACKGROUND );

		m_ImgTabBack.DrawImage(&pDc,0,0);
		//////////////////////////////////////////////////////////////////////////
		//draw arrow
		if ( ((_QMTAB_TAB *)m_arTabs.GetAt( i ))->nPageIdx > 0 && GetTabItemCount(i)>nGroupItemCount ) 
		{
			if ( m_iSelectedItem == QMTAB_CFG_ARROWUP ) {
				if ( m_bMouseDown ) {
					m_arrowList.Draw( &pDc, 1, CPoint( rc2.Width()-20+1, 10+1 ), ILD_NORMAL );
				}
				else {
					m_arrowList.Draw( &pDc, 1, CPoint( rc2.Width()-20, 10 ), ILD_NORMAL );
				}
			}
			else {
				m_arrowList.Draw( &pDc, 3, CPoint( rc2.Width()-20, 10 ), ILD_NORMAL );
			}
		}

		if ( GetTabItemCount(m_nSelectedTab)>nGroupItemCount
			&&((_QMTAB_TAB *)m_arTabs.GetAt( i ))->nPageIdx<GetTabItemCount(m_nSelectedTab)-nGroupItemCount
			&&GetTabItemCount(i)>nGroupItemCount )
		{
			if ( m_iSelectedItem == QMTAB_CFG_ARROWDOWN ) {
				if ( m_bMouseDown ) {
					m_arrowList.Draw( &pDc, 0, CPoint( rc2.Width()-20+1, rc2.Height()-QMTAB_CFG_ARROWHEIGHT-10+1 ), ILD_TRANSPARENT );
				}
				else {
					m_arrowList.Draw( &pDc, 0, CPoint( rc2.Width()-20, rc2.Height()-QMTAB_CFG_ARROWHEIGHT-10 ), ILD_TRANSPARENT );
				}
			}
			else {
				m_arrowList.Draw( &pDc, 2, CPoint( rc2.Width()-20, rc2.Height()-QMTAB_CFG_ARROWHEIGHT-10 ), ILD_TRANSPARENT );
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//draw item to dc
		_QMTAB_ITEM *pItem = qTab->item;
		if ( NULL != pItem ) 
		{
			nPageItems = GetMaxTabItemCount();

			j=0;
			nTop = 10;
			int nX = 2;
			int nIndex = 10 ;
			int nXCount = 0;
			CRect rcClient;
			GetClientRect(rcClient);
			nXCount = rcClient.Width()/m_iconSize.cx;
			while( true )
			{
				k = qTab->nPageIdx+j;
				if ( k >= GetTabItemCount( i ) || j >= nPageItems ) {
					break;
				}

				m = 0;
				pItemTmp = pItem;
				while( m++ != k ) {
					pItemTmp = pItemTmp->pNext;
				}
				
				//////////////////////////////////////////////////////////////////////////
				//draw icon
				if(j%nXCount == 0)
					nIndex = 0;
				//nTop += m_iconSize.cy+QMTAB_CFG_ITEM_HEIGHT;

				ptImage.x = nX+nIndex*(m_iconSize.cx+1);
				ptImage.y = nTop;
				
				nIndex ++;
				//////////////////////////////////////////////////////////////////////////
				//if focus
				if ( i == m_nSelectedTab && m_iSelectedItem != -1 && m_iSelectedItem == j ) 
				{
					//pMemDc->TextOut( 0, 0, m_bMouseDown ? "1" : "0" );
					/*
					pOldPen = (CPen *)pDc.SelectObject( &pen1 );
					pDc.MoveTo( ptImage.x - 1, ptImage.y - 1 );
					pDc.LineTo( ptImage.x - 1, ptImage.y + m_iconSize.cy + 1 );

					pDc.MoveTo( ptImage.x - 1, ptImage.y - 1 );
					pDc.LineTo( ptImage.x + m_iconSize.cx + 1, ptImage.y - 1 );

					pDc.SelectObject( &pen2 );

					pDc.MoveTo( ptImage.x + m_iconSize.cx, ptImage.y - 1 );
					pDc.LineTo( ptImage.x + m_iconSize.cx, ptImage.y + m_iconSize.cy + 1 );

					pDc.MoveTo( ptImage.x - 1, ptImage.y + m_iconSize.cy );
					pDc.LineTo( ptImage.x + m_iconSize.cx, ptImage.y + m_iconSize.cy );
					*/

					if ( !m_bMouseDown )
					{
						m_ImgGroup.DrawImage(&pDc,ptImage.x-1,ptImage.y-1);
					//	m_pImgList->Draw( &pDc, pItemTmp->nImage, ptImage, ILD_NORMAL );

						CString strImg;
						strImg.Format(L"%s%s.png",CBmpUtil::GetExePath() + _T("ADImage\\GameKindSmall\\GameKind_"),pItemTmp->lpsItemImg);
						CPngImage img;
						img.LoadImage( strImg);
						img.DrawImage(&pDc,ptImage.x,ptImage.y);

// 						pOldPen = (CPen *)pDc.SelectObject( &pen1 );
// 						pDc.MoveTo( ptImage.x-1, ptImage.y-2 );
// 						pDc.LineTo( ptImage.x-1, ptImage.y+m_iconSize.cy+1 );
// 						
// 						pDc.MoveTo( ptImage.x-1, ptImage.y-1);
// 						pDc.LineTo( ptImage.x+m_iconSize.cx+1, ptImage.y-1 );
// 						
// 						pDc.SelectObject( &pen2 );
// 						
// 						pDc.MoveTo( ptImage.x+m_iconSize.cx+1, ptImage.y );
// 						pDc.LineTo( ptImage.x+m_iconSize.cx+1, ptImage.y+m_iconSize.cy+1 );
// 						
// 						pDc.MoveTo( ptImage.x-1, ptImage.y+m_iconSize.cy+1 );
// 						pDc.LineTo( ptImage.x+m_iconSize.cx-1, ptImage.y+m_iconSize.cy+1 );
						//pDc.SetPixel( ptImage.x+m_iconSize.cx+1, ptImage.y-1 , RGB(106,106,106) );

						pDc.SelectObject( pOldPen );
					}
					else
					{
						ptImage.x += 2;
						ptImage.y += 2;
				//		m_pImgList->Draw( &pDc, pItemTmp->nImage, ptImage, ILD_NORMAL );
						CString strImg;
						strImg.Format(L"%s%s.png",CBmpUtil::GetExePath() + _T("ADImage\\GameKindSmall\\GameKind_"),pItemTmp->lpsItemImg);
						CPngImage img;
						img.LoadImage( strImg);
						img.DrawImage(&pDc,ptImage.x,ptImage.y);

						ptImage.x -= 2;
						ptImage.y -= 2;

						pOldPen = (CPen *)pDc.SelectObject( &pen2 );
						pDc.MoveTo( ptImage.x+1, ptImage.y );
						pDc.LineTo( ptImage.x+1, ptImage.y+m_iconSize.cy+1 );
						
						pDc.MoveTo( ptImage.x+1, ptImage.y+1);
						pDc.LineTo( ptImage.x+m_iconSize.cx+1, ptImage.y+1 );
						
						pDc.SelectObject( &pen1 );
						
						pDc.MoveTo( ptImage.x+m_iconSize.cx+1, ptImage.y );
						pDc.LineTo( ptImage.x+m_iconSize.cx+1, ptImage.y+m_iconSize.cy+1 );
						
						pDc.MoveTo( ptImage.x, ptImage.y+m_iconSize.cy+1 );
						pDc.LineTo( ptImage.x+m_iconSize.cx-1, ptImage.y+m_iconSize.cy+1 );
						
						pDc.SelectObject( pOldPen );
					}
				}
				else {
//					m_pImgList->Draw( &pDc, pItemTmp->nImage, ptImage, ILD_NORMAL );
					CString strImg;
					strImg.Format(L"%s%s.png",CBmpUtil::GetExePath() + _T("ADImage\\GameKindSmall\\GameKind_"),pItemTmp->lpsItemImg);
					CPngImage img;
					img.LoadImage( strImg);
					img.DrawImage(&pDc,ptImage.x,ptImage.y);

				}

				//////////////////////////////////////////////////////////////////////////
				//draw text
// 				rcItemText.top = ptImage.y + m_iconSize.cy + 5;
// 				rcItemText.bottom = rcItemText.top + 50;
// 				rcItemText.left = ptImage.x;
// 				rcItemText.right = ptImage.x+m_iconSize.cx;
// 
// 				COLORREF cOldColor = pDc.SetTextColor( pItemTmp->txtColor );
// 			//	CString str(pItemTmp->lpsItemName);
// 				pDc.DrawText((LPCTSTR)pItemTmp->lpsItemName , &rcItemText, DT_CENTER );
// 				pDc.SetTextColor( cOldColor );

				j++;
				if(j%nXCount == 0)
					nTop += m_iconSize.cy+QMTAB_CFG_ITEM_HEIGHT;
			}

			//////////////////////////////////////////////////////////////////////////
			//draw to memdc
			//pMemDc->BitBlt( rc2.left, rc2.top, nWidth, nHeight, &pDc, 0, 0, SRCCOPY );
			//////////////////////////////////////////////////////////////////////////
			//要判断当前tab是否有要显示的区域，并得知从上还是从下
			GetTabAvailableArea( i, &rcTmp );
			if ( rcTmp.Height() > 0 )
			{
				if ( m_bMovingUp ) 
				{
					if ( m_nSelectedTab == i ) 
					{
						//from bottom
						pMemDc->BitBlt( rc2.left, rc2.top+(rc2.Height()-rcTmp.Height()), nWidth, rcTmp.Height(), &pDc, 0, nHeight-rcTmp.Height(), SRCCOPY );
					}
					else 
					{
						//from top
						pMemDc->BitBlt( rc2.left, i*m_nTabHeight+m_nTabHeight+2, nWidth, rcTmp.Height(), &pDc, 0, 0, SRCCOPY );
					}
				}
				else 
				{
					if ( m_nSelectedTab == i ) 
					{
						//from top
						pMemDc->BitBlt( rc2.left, i*m_nTabHeight+m_nTabHeight+2, nWidth, rcTmp.Height(), &pDc, 0, 0, SRCCOPY );
					}
					else 
					{
						//from bottom
						pMemDc->BitBlt( rc2.left, ((_QMTAB_TAB *)m_arTabs.GetAt(i))->rect.top+m_nTabHeight, nWidth, rcTmp.Height(), &pDc, 0, nHeight-rcTmp.Height(), SRCCOPY );
					}
				}
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		//get back objects
		pDc.SelectObject( pOldBitmap );
		pDc.SelectObject( pOldFont );

		pen1.DeleteObject();
		pen2.DeleteObject();

		bitmap.DeleteObject();
		pDc.DeleteDC();
	}

	/*
	CRect rc1, rc2;
	GetDataAreaRect( &rc1, &rc2 );

	_QMTAB_ITEM *pItem = ((_QMTAB_TAB *)m_arTabs.GetAt( m_nSelectedTab ))->item;
	if ( NULL == pItem ) {
		return;
	}

	CFont *pOldFont;
	pOldFont = ( CFont * )pMemDc->SelectObject( &m_fntTab );

	IMAGEINFO imgInfo;
	int i=0;
	int nTop = rc2.top + 10;
	CPoint pt1;

	CRect rcTmp = rc2;
	while( pItem )
	{
		memset( &imgInfo, 0, sizeof(IMAGEINFO) );
		m_pImgList->GetImageInfo( pItem->nImage, &imgInfo );

		pt1.x = (rc2.Width()-(imgInfo.rcImage.right-imgInfo.rcImage.left))/2;
		pt1.y = nTop;
		m_pImgList->Draw( pMemDc, pItem->nImage, pt1, ILD_NORMAL );

		rcTmp.top = pt1.y + (imgInfo.rcImage.bottom-imgInfo.rcImage.top) + 5;
		pMemDc->DrawText( pItem->lpsItemName, rcTmp, DT_CENTER );

		i++;
		nTop += (imgInfo.rcImage.bottom-imgInfo.rcImage.top) + 40;
		pItem = pItem->pNext;
	}
	
	pMemDc->SelectObject( pOldFont );
	*/


}

void CQMTab::DrawTabs(CDC *pMemDc)
{
	CRect rcItem, rcArea, rcDataArea, rcMOverItem;
	CPen lPen;
	CPen *pOldPen;
	
	GetDataAreaRect( rcDataArea );
	lPen.CreatePen( PS_SOLID, 1, QMTAB_CFG_BTNBORDER );
	pOldPen = ( CPen * )pMemDc->SelectObject( &lPen );
	
	//CBitmap 
	for(int i=0;i<m_arTabs.GetSize();i++) 
	{
		//GetTabItemRect( i, &rcItem );
		rcItem = ((_QMTAB_TAB *)m_arTabs.GetAt(i))->rect;
//		CPngImage img = ((_QMTAB_TAB *)m_arTabs.GetAt(i))->img;

	//	pMemDc->FillSolidRect( rcItem, QMTAB_CFG_BTNBORDER_TOPLEFT );
		
		rcItem.top +=1;
		rcItem.left += 1;

		if ( i == m_nFocusTab ) 
		{
			pMemDc->FillSolidRect( rcItem, QMTAB_CFG_BTNCOLOR_OVER );

// 			rcMOverItem = rcItem;
// 			rcMOverItem.left = rcMOverItem.right-1;
// 			pMemDc->FillSolidRect( rcMOverItem, RGB(0,0,0) );
// 			rcMOverItem = rcItem;
// 			rcMOverItem.top = rcMOverItem.bottom-2;
// 			pMemDc->FillSolidRect( rcMOverItem, RGB(0,0,0) );
		}
		else {
			pMemDc->FillSolidRect( rcItem, QMTAB_CFG_BTNCOLOR );
		}

// 		if ( i == m_nMouseDownTab )
// 		{
// 			pMemDc->FillSolidRect( rcItem, QMTAB_CFG_BTNCOLOR_OVER );
// 
// 			rcMOverItem = rcItem;
// 			rcMOverItem.top-=1;
// 			rcMOverItem.left-=1;
// 			rcMOverItem.right = rcMOverItem.left+1;
// 			pMemDc->FillSolidRect( rcMOverItem, RGB(106,106,106) );
// 			rcMOverItem.left+=1;
// 			rcMOverItem.right = rcMOverItem.left+1;
// 			pMemDc->FillSolidRect( rcMOverItem, RGB(0,0,0) );
// 
// 			rcMOverItem = rcItem;
// 			rcMOverItem.top-=1;
// 			rcMOverItem.bottom = rcMOverItem.top+1;
// 			pMemDc->FillSolidRect( rcMOverItem, RGB(106,106,106) );
// 			rcMOverItem.top+=1;
// 			rcMOverItem.bottom = rcMOverItem.top+1;
// 			pMemDc->FillSolidRect( rcMOverItem, RGB(0,0,0) );
// 		}
// 
// 		rcItem.top -=1;
// 		rcItem.left -=1;
// 		
// 		pMemDc->MoveTo( rcItem.left, rcItem.bottom-1 );
// 		pMemDc->LineTo( rcItem.right, rcItem.bottom-1 );

		if ( 0 != i )
		{
			//draw up line
			pMemDc->MoveTo( rcItem.left, rcItem.top-1 );
			pMemDc->LineTo( rcItem.right, rcItem.top-1 );
		}

		if ( i == m_nSelectedTab+1 ) 
		{
			//rcArea = rcItem;

			//pMemDc->MoveTo( rcArea.left, rcArea.top-1 );
			//pMemDc->LineTo( rcArea.right, rcArea.top-1 );
		}
		else if ( i == m_nSelectedTab )
		{
			//rcArea = rcItem;
			//rcArea.top += m_nTabHeight;
			//rcArea.bottom = rcArea.top + rcDataArea.bottom;
			
			//pMemDc->MoveTo( rcArea.left, rcArea.bottom );
			//pMemDc->LineTo( rcArea.right, rcArea.bottom );
		}
		CString strImg;
		strImg.Format(L"%stype_%02d.png",CBmpUtil::GetExePath() + _T("ADImage\\GameTitle\\"), ((_QMTAB_TAB *)m_arTabs.GetAt(i))->nImgIndex);
		CPngImage img;
		img.LoadImage( strImg);
		img.DrawImage(pMemDc,rcItem.left,rcItem.top);

		DrawTabText( &rcItem, (((LPCTSTR)(m_arTabs.GetAt(i))->lpsTabName)), pMemDc, ( i == m_nSelectedTab ) );
	}
	
	pMemDc->SelectObject( pOldPen );
	lPen.DeleteObject();
}
 
void CQMTab::DrawFrame(CDC *pMemDc)
{
	CPen lPen;
	CPen *pOldPen;
	lPen.CreatePen( PS_SOLID, 1, QMTAB_CFG_FRAMEBORDER_TOPLEFT );
	
	pOldPen = ( CPen * )pMemDc->SelectObject( &lPen );
  
	if ( m_rcClient.Width() > 0 && m_rcClient.Height() > 0 ) 
	{
		pMemDc->MoveTo( 0, 0 );
		pMemDc->LineTo( m_rcClient.Width()+1, 0 );

		pMemDc->MoveTo( 0, 0 );
		pMemDc->LineTo( 0, m_rcClient.Height()+1 );

		pMemDc->SelectObject( pOldPen );
		lPen.DeleteObject();
		lPen.CreatePen( PS_SOLID, 1, QMTAB_CFG_FRAMEBORDER_BOTTOMRIGHT );
		pMemDc->SelectObject( &lPen );

		pMemDc->MoveTo( m_rcClient.Width()-1, 1 );
		pMemDc->LineTo( m_rcClient.Width()-1, m_rcClient.Height() );

		pMemDc->MoveTo( 1, m_rcClient.Height()-1 );
		pMemDc->LineTo( m_rcClient.Width()+1, m_rcClient.Height()-1 );
	}

	pMemDc->SelectObject( pOldPen );
	lPen.DeleteObject();
}


BOOL CQMTab::SetTabHeight(DWORD nHeight)
{
	m_nTabHeight = nHeight;

	return TRUE;
}

void CQMTab::GetTabItemRect(int nItem, LPRECT rect)
{
	m_nTabHeight = m_ImageBack.GetHeight();
	rect->left = m_rcTab.left;
	rect->top = nItem*m_nTabHeight+m_rcTab.top;
	rect->right = rect->left+m_rcTab.Width();
	rect->bottom = rect->top+m_nTabHeight;

	if ( -1 != m_nSelectedTab && nItem > m_nSelectedTab ) 
	{
		CRect rcDataArea;
		GetDataAreaRect( &rcDataArea );

		rect->top += rcDataArea.bottom;
		rect->bottom += rcDataArea.bottom;
		CString strLog;
		strLog.Format(L"TABITEMRECT :nItem:%d    %d,%d,%d,%d",nItem,rect->left,rect->top,rect->right,rect->bottom);
		OutputDebugString(strLog);
	}
	else if(-1 != m_nSelectedTab && (nItem-1) == m_nSelectedTab &&nItem>0)
	{
		CRect rcDataArea;
		GetDataAreaRect( &rcDataArea );

		rect->top -= rcDataArea.bottom;
		rect->bottom -= rcDataArea.bottom;

	}
}

void CQMTab::DrawTabText(LPRECT rcItem, LPCTSTR lpzTabText, CDC *pMemDc, BOOL bFocus)
{
	CFont *pOldFont;

	if ( bFocus ) {
		pOldFont = (CFont *)pMemDc->SelectObject( &m_fntTabSel );
	}
	else {
		pOldFont = (CFont *)pMemDc->SelectObject( &m_fntTab );
	}

	CSize sizFont = pMemDc->GetTextExtent( lpzTabText );
	rcItem->top += ((rcItem->bottom-rcItem->top)-sizFont.cy)/2;
	pMemDc->DrawText( lpzTabText, rcItem, DT_CENTER );

	pMemDc->SelectObject( pOldFont );
}

void CQMTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int nItemID = GetEventItem( &point );
	m_nMouseDownTab = nItemID;
	
	m_bMouseDown = TRUE;

	SetCapture();

	Invalidate( FALSE );
	//if ( m_nMouseDownTab == nItemID && m_iSelectedItem == -1 ) {
	//	return;
	//}
}

void CQMTab::ChangeCursorTable(int nItemID, int nType) 
{
	if ( -1 == nItemID ) {
		Invalidate( FALSE );
		return;
	}
	else if ( m_nSelectedTab == nItemID )
	{
		m_nSelectedTab = -1;
		SetTimer( QMTAB_CFG_TIMER_FIXPOS, QMTAB_CFG_TIMER_INTERVAL, NULL );
		m_bMoving = TRUE;
		Invalidate( FALSE );
		return;
	}
	else
	{
		m_nSelectedTab = nItemID;
		
		if ( 0 == nType )
		{
			CRect rcTmp;
			for(int i=0;i<m_arTabs.GetSize();i++)
			{
				GetTabItemRect( i, rcTmp );
				((_QMTAB_TAB *)m_arTabs.GetAt(i))->rect = rcTmp;
			}
			
			Invalidate( FALSE );
		}
		else {
			SetTimer( QMTAB_CFG_TIMER_FIXPOS, QMTAB_CFG_TIMER_INTERVAL, NULL );
			m_bMoving = TRUE;
		}
	}
}

int CQMTab::GetEventItem(LPPOINT point)
{
	CRect rcItem;
	
	for(int i=0;i<m_arTabs.GetSize();i++)
	{
		GetTabItemRect( i, &rcItem );
		if ( rcItem.PtInRect( *point ) ) {
			return i;
		}
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////
//得到用于显示数据的区域大小
void CQMTab::GetDataAreaRect(LPRECT rect, LPRECT rectArea)
{
	rect->left = rect->top = 0;
	rect->right = m_rcTab.Width();
	rect->bottom = m_rcTab.Height()-m_nTabHeight*m_arTabs.GetSize()+1;

	if ( NULL != rectArea )
	{
		if ( -1 == m_nSelectedTab ) {
			rectArea = NULL;
		}
		else {
			rectArea->left = m_rcTab.left + 1;
			rectArea->top = m_nSelectedTab*m_nTabHeight+m_nTabHeight + 2;
			rectArea->right = m_rcTab.Width();
			rectArea->bottom = rectArea->top+rect->bottom-3;
		}

	}
}

BOOL CQMTab::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

BOOL CQMTab::IsCorrectPos()
{
	CRect rcItem, rcItemNow;
	for(int i=0;i<m_arTabs.GetSize();i++)
	{
		GetTabItemRect( i, rcItem );
		rcItemNow = ((_QMTAB_TAB *)m_arTabs.GetAt(i))->rect;
		
		if ( rcItem.top != rcItemNow.top ) {
			return FALSE;
		}
	}
	
	return TRUE;
}

void CQMTab::AdjustPos()
{
	_QMTAB_TAB *pTab;
	CRect rcItem;
	int nDiff, nStep;

	for(int i=0;i<m_arTabs.GetSize();i++)
	{
		pTab = m_arTabs.GetAt(i);
		ASSERT( pTab != NULL );

		GetTabItemRect( i, &rcItem );

		if ( pTab->rect.top != rcItem.top )
		{
			nDiff = rcItem.top - pTab->rect.top;
			if ( abs( nDiff ) > 10 )
			{
				nStep = (int)ceil((double)nDiff * (double)QMTAB_CFG_MOVE_STEP);
				pTab->rect.top += nStep;
				pTab->rect.bottom += nStep;

				if ( nStep < 0 ) {
					m_bMovingUp = TRUE;
				}
				else {
					m_bMovingUp = FALSE;
				}
			}  
			else
			{
				pTab->rect = rcItem;
			}
		}
	}
}

void CQMTab::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if ( QMTAB_CFG_TIMER_FIXPOS == nIDEvent )
	{
		if ( !IsCorrectPos() )
		{
			AdjustPos();
			Invalidate( FALSE );
		}
		else {
			m_bMoving = FALSE;
			KillTimer( nIDEvent );
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CQMTab::SetCurSorTab(int nTabID)
{
	if ( nTabID < m_arTabs.GetSize() ) {
		ChangeCursorTable( nTabID, 0 );
	}
}

void CQMTab::SetImageList(CImageList *pImageList)
{
	m_pImgList = pImageList;

	IMAGEINFO imgInfo;
	memset( &imgInfo, 0, sizeof( IMAGEINFO ) );
// 	if ( m_pImgList->GetImageCount() > 0 ) {
// 		m_pImgList->GetImageInfo( 0, &imgInfo );
// 
// 		m_iconSize.cx = imgInfo.rcImage.right-imgInfo.rcImage.left;
// 		m_iconSize.cy = imgInfo.rcImage.bottom-imgInfo.rcImage.top;
// 	}
}

BOOL CQMTab::AddItem(int nTabID, LPCTSTR lpzBtnName, int nImage,LPCTSTR lpzBtnImg, int nData, COLORREF txtColor)
{
	if ( nTabID >= m_arTabs.GetSize() ) {
		return FALSE;
	}

	_QMTAB_TAB *pTab = m_arTabs.GetAt( nTabID );
	if ( NULL == pTab ) {
		return FALSE;	
	}

	_QMTAB_ITEM *pItem;
	if ( !pTab->item ) {
		pTab->item = new _QMTAB_ITEM;
		pItem = pTab->item;
	}
	else {
		pItem = pTab->item;
		while ( pItem->pNext != NULL ) {
			pItem = pItem->pNext;
		}
		
		pItem->pNext = new _QMTAB_ITEM;
		pItem = pItem->pNext;
	}
	
	memset( pItem, 0, sizeof( _QMTAB_ITEM ) );
	pItem->nData = nData;
	pItem->nImage = nImage;

	if ( txtColor == QMTAB_CFG_DEFAULTTXTCOLOR ) {
		pItem->txtColor = m_TextColor;		
	}
	else {
		pItem->txtColor = txtColor;
	}

	//strcpy( pItem->lpsItemName, lpzBtnName );
	sprintf(pItem->lpsItemName,"%s", lpzBtnName);
	_sntprintf(pItem->lpsItemImg,CountArray(pItem->lpsItemImg),TEXT("%s"), lpzBtnImg);

	CString strImg;
	strImg.Format(L"%sGameKind_%s.png",CBmpUtil::GetExePath() + _T("ADImage\\GameKindSmall\\"),pItem->lpsItemImg);

	CPngImage img;
	img.LoadImage( strImg);

	m_iconSize.cx = img.GetWidth();
	m_iconSize.cy = img.GetHeight();

	return TRUE;
}

int CQMTab::GetTabItemCount(int nTabID)
{
	if ( nTabID >= m_arTabs.GetSize() ) {
		return 0;
	}

	_QMTAB_TAB *pTab = m_arTabs.GetAt( nTabID );
	if ( pTab == NULL ) {
		return 0;
	}

	_QMTAB_ITEM *pItem = pTab->item;
	if ( NULL == pItem ) {
		return 0;
	}

	int nCount = 0;
	while( NULL != pItem ) {
		nCount++;
		pItem = pItem->pNext;
	}

	return nCount;
}

void CQMTab::GetTabAvailableArea(int nTabID, LPRECT rcArea)
{
	memset( rcArea, 0, sizeof(RECT) );

	if ( nTabID >= m_arTabs.GetSize() ) {
		return;
	}
	
	_QMTAB_TAB *pTab = m_arTabs.GetAt( nTabID );
	if ( pTab == NULL ) {
		return;
	}

	int n1, n2;
	if ( nTabID != m_arTabs.GetSize()-1 )
	{
		//not last tab
		n1 = ((_QMTAB_TAB *)m_arTabs.GetAt( nTabID ))->rect.top + m_nTabHeight;
		n2 = ((_QMTAB_TAB *)m_arTabs.GetAt( nTabID + 1 ))->rect.top;

		if ( n2-n1>10 ) {
			rcArea->left = rcArea->top = rcArea->right = 0;
			rcArea->bottom = n2-n1-2;
		}
	}
	else 
	{
		rcArea->left = rcArea->top = rcArea->right = 0;
		rcArea->bottom = m_rcTab.Height()-( ((_QMTAB_TAB *)m_arTabs.GetAt( nTabID ))->rect.top + m_nTabHeight );
	}
}

void CQMTab::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ( m_bMouseDown ) {
		return;
	}
	SetFocus();
	int nItemID = GetEventItem( &point );

	m_nFocusTab = nItemID;

	CRect rc1, rc2;;
	GetDataAreaRect( &rc1, &rc2 );
	if ( rc2.PtInRect( point ) ) 
	{
		CheckItemFocus( &point );
	}

	if ( m_rcTab.PtInRect( point ) ) 
	{
		CString strLog;
		strLog.Format(L"TABCTRL nItemID:%d",nItemID);
		OutputDebugString(strLog);
		SetCapture();
	}
	else 
	{
		m_iSelectedItem = -1;
		m_nFocusTab = -1;
		
		ReleaseCapture();
	}

	//Invalidate( FALSE );

	//CWnd::OnMouseMove(nFlags, point);
}

void CQMTab::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();

	m_nMouseDownTab = -1;
	m_nFocusTab = -1;
	m_iSelectedItem = -1;
	m_bMouseDown = FALSE;
	
	int nItemID = GetEventItem( &point );
	if ( nItemID == -1 ) 
	{
		CRect rc1, rc2;;
		GetDataAreaRect( &rc1, &rc2 );
		if ( rc2.PtInRect( point ) ) 
		{
			CheckItemFocus( &point );

			if ( m_iSelectedItem == QMTAB_CFG_ARROWUP ) {
				ArrowMoveUp();
			}
			else if ( m_iSelectedItem == QMTAB_CFG_ARROWDOWN ) {
				ArrowMoveDown();
			}
			else if ( m_iSelectedItem != -1 ) 
			{
				//send message to parent wnd
				if ( m_nFlag & QMT_MESSAGE && m_uCallbackMessage ) 
				{
					_QMTAB_TAB *pTab = m_arTabs.GetAt( m_nSelectedTab );
					ASSERT( pTab );
					_QMTAB_ITEM *pItem = pTab->item;
					ASSERT( pItem );

					int i = 0;
					while( pItem ) 
					{
						if ( i == m_iSelectedItem+pTab->nPageIdx ) 
						{
							::PostMessage( m_pParent->m_hWnd, m_uCallbackMessage, MAKEWPARAM( m_nSelectedTab, m_iSelectedItem+((_QMTAB_TAB *)m_arTabs.GetAt(m_nSelectedTab))->nPageIdx ), MAKELPARAM( pItem->nData, 0 ) );
							break;
						}

						i++;
						pItem = pItem->pNext;
					}
				}
			}
		}
	}

	ChangeCursorTable( nItemID, 1 );

	//CWnd::OnLButtonUp(nFlags, point);
}

void CQMTab::SetTextColor(COLORREF color)
{
	m_TextColor = color;	
}

//////////////////////////////////////////////////////////////////////////
//检查哪个子项被用户选中
void CQMTab::CheckItemFocus( LPPOINT lpPoint )
{
	if ( m_nSelectedTab < 0 || m_nSelectedTab >= m_arTabs.GetSize() ) {
		m_iSelectedItem = -1;
	}

	CRect rc1, rc2, rcIcon;
	int nTop;
	GetDataAreaRect( &rc1, &rc2 );

	_QMTAB_TAB *pTab = m_arTabs.GetAt( m_nSelectedTab );
	if ( pTab == NULL || pTab->item == NULL ) {
		return;
	}

	int nAvailableHeight = rc2.Height()-10;
	int nItemHeight = m_iconSize.cy + QMTAB_CFG_ITEM_HEIGHT;
	int nPageItems = nAvailableHeight/nItemHeight;
	CRect rcClient;
	GetClientRect(rcClient);
	int nXCount = rcClient.Width()/m_iconSize.cx;
	nPageItems*=nXCount;
	nTop = rc2.top+10;
	_QMTAB_ITEM *pItem = pTab->item;
	int i = 0;
	int nX = 10;
	int nIndex = 0;


	while( pItem ) 
	{
		if(i%nXCount == 0)
			nIndex = 0;

		rcIcon.left = nX + nIndex*(m_iconSize.cx+10);//(rc2.Width()-m_iconSize.cx)/2+rc2.left;
		rcIcon.right = rcIcon.left+m_iconSize.cx;
		rcIcon.top = nTop;
		rcIcon.bottom = rcIcon.top+m_iconSize.cy;
		nIndex++;
		if ( rcIcon.PtInRect( *lpPoint ) )
		{
			m_iSelectedItem = i;
			return;
		}

		pItem = pItem->pNext;
		i++;
		if(i%nXCount == 0)
			nTop += QMTAB_CFG_ITEM_HEIGHT + m_iconSize.cy;

		if ( i >= nPageItems ) {
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//箭头的位置
	CRect rcArrowUp, rcArrowDown;
	GetArrowPosition( &rcArrowUp, &rcArrowDown );

	if ( rcArrowUp.PtInRect( *lpPoint ) ) {
		m_iSelectedItem = QMTAB_CFG_ARROWUP;
	}
	else if ( rcArrowDown.PtInRect( *lpPoint ) ) {
		m_iSelectedItem = QMTAB_CFG_ARROWDOWN;
	}
	else {
		m_iSelectedItem = -1;
	}
}

void CQMTab::GetArrowPosition(LPRECT rcUp, LPRECT rcDown)
{
	CRect rc1, rc2, rcIcon;
	GetDataAreaRect( &rc1, &rc2 );

	rcUp->top = rc2.top+10;
	rcUp->bottom = rcUp->top+QMTAB_CFG_ARROWHEIGHT;
	rcUp->left = rc2.Width()-20+1;
	rcUp->right = rcUp->left+QMTAB_CFG_ARROWWIDTH+1;
	
	rcDown->top = rc2.bottom-QMTAB_CFG_ARROWHEIGHT-10;
	rcDown->bottom = rcDown->top+QMTAB_CFG_ARROWHEIGHT;
	rcDown->left = rc2.Width()-20+1;
	rcDown->right = rcDown->left+QMTAB_CFG_ARROWWIDTH+1;
}

void CQMTab::ArrowMoveUp()
{
	if ( m_nSelectedTab >= m_arTabs.GetSize() ) {
		return;
	}
	_QMTAB_TAB *pTab = m_arTabs.GetAt( m_nSelectedTab );
	if ( !pTab ) {
		return;
	}

	if ( pTab->nPageIdx > 0 ) {
		pTab->nPageIdx--;
	}
}

void CQMTab::ArrowMoveDown()
{
	if ( m_nSelectedTab >= m_arTabs.GetSize() ) {
		return;
	}
	_QMTAB_TAB *pTab = m_arTabs.GetAt( m_nSelectedTab );
	if ( !pTab ) {
		return;
	}
	
	CRect rc1, rc2;
	GetDataAreaRect( &rc1, &rc2 );

	int nGroupItemCount = GetMaxTabItemCount();
	if ( GetTabItemCount(m_nSelectedTab)>nGroupItemCount
		&&pTab->nPageIdx<GetTabItemCount(m_nSelectedTab)-nGroupItemCount )
	{
		pTab->nPageIdx++;
	}
}

//////////////////////////////////////////////////////////////////////////
//得到当前一个组能放几个图标
int CQMTab::GetMaxTabItemCount()
{
	CRect rc1, rc2;
	GetDataAreaRect( &rc1, &rc2 );
	
	int nGroupItemCount = (rc2.Height()-20)/(m_iconSize.cy+QMTAB_CFG_ITEM_HEIGHT)*3;
	return nGroupItemCount;
}
