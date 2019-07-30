// GfxOutBarCtrl.cpp: implementation of the CGfxOutBarCtrl class.
// Download by http://www.NewXing.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GfxOutBarCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

CGfxOutBarCtrl::CGfxOutBarCtrl()
{
	m_colBackground=RGB(56,81,143);//BackColor
	m_colFolderBorderColor=RGB(12,188,230);//Folder Border Color
	m_nFolderHeight=22;
//	m_arrayFoldersCurrent=NULL;
	m_nSelectedFolder=0;
	m_hHandleCursor=AfxGetApp()->LoadCursor(IDC_HANDLE);
}

CGfxOutBarCtrl::~CGfxOutBarCtrl()
{

}
//-------
CGfxOutBarCtrl::CBarFolder::CBarFolder(const char *szFolderName,DWORD dwFolderData)
{
	if(szFolderName)
	{
		szName=new char[strlen(szFolderName)+1];
		ASSERT(szName);
		strcpy(szName,szFolderName);
	}
	dwData=dwFolderData;
	pLargeImageList=NULL;
	pSmallImageList=NULL;
	pChild=NULL;

}
CGfxOutBarCtrl::CBarFolder::~CBarFolder()
{
	if(szName)
		delete[]szName;
}
int CGfxOutBarCtrl::CBarFolder::GetItemCount()
{
	return arItems.GetSize();
}
int CGfxOutBarCtrl::CBarFolder::InsertItem(int nIndex,const char *szText,const int nImage,const DWORD dwData)
{
	//---
	return 1;
}
//-------------


CGfxOutBarCtrl::CBarItem::CBarItem(const char* szItemName,const int nItemImageIndex,DWORD dwItemData)
{
	if(szItemName)
	{
		szName=new char[strlen(szItemName)+1];
		strcpy(szName,szItemName);
	}
	nImageIndex=nItemImageIndex;
	dwData=dwItemData;
}

CGfxOutBarCtrl::CBarItem::~CBarItem()
{

}

//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGfxOutBarCtrl,CWnd)
//{{AFX_MSG(CGfxOutBarCtrl)
ON_WM_PAINT()
ON_WM_ERASEBKGND()
ON_WM_SETCURSOR()
ON_WM_LBUTTONDOWN()
//}}AFX_MSG
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//Create
BOOL CGfxOutBarCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, const DWORD dwFlag)
{
	m_dwFlag=dwFlag;
	return CWnd::CreateEx(0,NULL,NULL,dwStyle|WS_CHILD,rect,pParentWnd,nID);
}

//Add Folder in OutBarCtrl
int CGfxOutBarCtrl::AddFolder(const char *szFolderName, const DWORD dwPos)
{ 
	CBarFolder *pBf=new CBarFolder(szFolderName,dwPos);
	if(pBf)
	{
		m_arrayFoldersCurrent.Add((void*)pBf);
	}
	return m_arrayFoldersCurrent.GetSize();
}

//DrawFolder
void CGfxOutBarCtrl::DrawFolder(CDC* pDC,const int nPos,CRect rect,const bool bSelect)
{
	CBarFolder *pBf=(CBarFolder*)m_arrayFoldersCurrent.GetAt(nPos);

	CPen pnFolderBorder,pnFolderName;
	pnFolderBorder.CreatePen(PS_SOLID,1,m_colFolderBorderColor);
	pnFolderName.CreatePen(PS_SOLID,1,RGB(129, 98,129));
	CPen* pnOld;
	if(!bSelect)//Selected not
	{
		pnOld=pDC->SelectObject(&pnFolderBorder);
	//	rect.DeflateRect(1,1);
		pDC->RoundRect(rect,CPoint(10,10));
		
		rect.DeflateRect(2,2);
		CBrush br[64];
		for(int i=0;i<64;i++)
		{
			br[i].CreateSolidBrush(/*RGB(225-i/4,244-i/5,255)*/RGB((221-i/2),(242-i/2),(255)));
		}
		
		CRect rcTmp;
		int i = 0;
		for(i=rect.top;i<rect.bottom+1;i++)
		{
			rcTmp.SetRect(rect.left,i,rect.right,i+1);
			pDC->FillRect(rcTmp,&br[(rect.bottom-i)*63/rect.Height()]);	
			
		}

		//Draw Folder Name In Folder
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(&pnFolderName);
		CFont *ftOld=pDC->SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
		CString strName(pBf->szName);
		pDC->DrawText(strName,rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		pDC->SelectObject(pnOld);
		pDC->SelectObject(ftOld);
	}
	else
	{

	}
	
}

//Get Folder Rect
void CGfxOutBarCtrl::GetFolderRect(const int nPos,CRect *rect)
{
	int nCurrentFolders=m_arrayFoldersCurrent.GetSize();
	if(nPos>=0&&nCurrentFolders)
	{
		CRect rc;
		GetClientRect(&rc);
		if(nPos>m_nSelectedFolder)
		rect->SetRect(rc.left,rc.bottom-1-(nCurrentFolders-(nPos+1)+1)*m_nFolderHeight,
						rc.right,rc.bottom-1-(nCurrentFolders-(nPos+1))*m_nFolderHeight);
		else
		rect->SetRect(rc.left,rc.top+1+(nPos-1+1)*m_nFolderHeight,
						rc.right,rc.top+1+(nPos+1)*m_nFolderHeight);
	}
rect->DeflateRect(CSize(1,1));//
}
//·ÀÖ¹ÉÁË¸
BOOL CGfxOutBarCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CGfxOutBarCtrl::OnPaint()
{
	CRect rc;
	GetClientRect(&rc);
	rc.top-=1;
	CPaintDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
 	CBitmap bmp;
 	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());

 	CBitmap * oldbmp = memDC.SelectObject(&bmp);//Select bmp
 	CDC* pDC=&memDC;
 	pDC->FillSolidRect(rc,m_colBackground);//draw the backGround
	
	int nFolders=m_arrayFoldersCurrent.GetSize();
	
	for(int i=0;i<nFolders;i++)
	{
		CRect rcFolder;
		GetFolderRect(i,&rcFolder);
		DrawFolder(pDC,i,&rcFolder,false);
	}
	
//	pDC->FillSolidRect(rc,m_colBackground);
// ASSERT(m_pSmallImageList);
// int yy=m_pSmallImageList->GetImageCount();
  	m_pSmallImageList->Draw(pDC,0,CPoint(24,24),ILD_NORMAL);
 //	m_pLargeImageList->Draw(pDC,1,CPoint(60,124),ILD_NORMAL);
	//
	CRect rcInside;
	CBarFolder*pBf=(CBarFolder*)m_arrayFoldersCurrent.GetAt(m_nSelectedFolder);
	for(int i=0;i<pBf->arItems.GetSize();i++)
	{
		GetItemRect(&rcInside,i,m_nSelectedFolder);
		DrawItem(pDC,m_nSelectedFolder,rcInside,i,false);
	}
	DrawItem(pDC,m_nSelectedFolder,rcInside,0,false);
	dc.BitBlt(rc.left,rc.top+1,rc.Width(),rc.Height(),&memDC,0,0,SRCCOPY);//Copy

}

BOOL CGfxOutBarCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{

	int nIndex;
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);
	if(HitTestEx(pt,nIndex)==htFolder)
	{
		SetCursor(m_hHandleCursor);
		return TRUE;
	}
	
	return CWnd::OnSetCursor(pWnd,nHitTest,message);
}

//
int CGfxOutBarCtrl::HitTestEx(CPoint pt,int &nIndex)
{
 	CRect rc;
//	ScreenToClient(&pt);
	for(int i=0;i<m_arrayFoldersCurrent.GetSize();i++)
	{
		GetFolderRect(i,&rc);
		if(rc.PtInRect(pt))
		{
			nIndex=i;
			return htFolder;
		}
	}
	return htNothing;
}

void CGfxOutBarCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nIndex;			
	if(HitTestEx(point,nIndex)==htFolder)
	{
		CClientDC dc(this);
		SetCapture();
		//m_nSelectedFolder=nIndex;
		while (TRUE)
		{
			MSG msg;
			::GetMessage(&msg,NULL,0,0);
			if(CWnd::GetCapture()!=this)break;
			switch (msg.message)
			{
			case WM_LBUTTONUP:
				{
					CPoint pt(/*GetMessagePos()*/msg.lParam);	
					if(htFolder==HitTestEx(pt,nIndex))
					{
						AnimateFolderScroll(m_nSelectedFolder,nIndex);
					}
					goto ExitLoop;
				}
				break;
			default:
				DispatchMessage(&msg);
				break;
			}

		}
// 		MSG msg;
// 		::GetMessage(&msg, NULL, 0, 0);
// 		for(int i=0;i<m_arrayFoldersCurrent.GetSize();i++)
// 		{
// 			CRect rc;
// 			GetFolderRect(i,&rc);
			
			//DrawFolder(&dc,i,rc,false);
		ExitLoop:
			m_nSelectedFolder=nIndex;
			Invalidate(TRUE);
			ReleaseCapture();
	//	}

	}
}

void CGfxOutBarCtrl::AnimateFolderScroll(const int nFrom, const int nTo)
{
	CRect rc,rc1,frc,frc1;
	GetInsideRect(&rc);
	rc1.SetRect(0,0,rc.Width(),rc.Height());
	GetFolderRect(nFrom,&frc);
	frc1.SetRect(0,0,frc.Width(),frc.Height());
	

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CDC* pDC=&memDC;

	CBitmap bmpFrom,bmpTo;CBitmap *bmpOld;
//	bmpFrom.CreateCompatibleBitmap(&dc,/*rc.Width()*/100,/*rc.Height()*/100);
	bmpFrom.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height()+m_nFolderHeight*2);
	bmpTo.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height()+m_nFolderHeight*2);

	if(nTo>nFrom)
	{
		bmpOld=pDC->SelectObject(&bmpFrom);
		rc1.SetRect(0,0,rc1.Width(),rc1.Height()+m_nFolderHeight*2);
		//GetClientRect(&rc1);
		pDC->FillSolidRect(rc1,m_colBackground/*RGB(255,255,0)*/);



		pDC->SelectObject(&bmpTo);
		pDC->FillSolidRect(rc1,m_colBackground/*RGB(255,0,0)*/);
		frc1.SetRect(1,1,frc1.Width(),frc.Height());
		DrawFolder(pDC,nTo,frc1,FALSE);
	}
	else
	{
		bmpOld=pDC->SelectObject(&bmpFrom);
		rc1.SetRect(0,0,rc.Width(),rc.Height()+m_nFolderHeight*2);
	//	pDC->FillSolidRect(rc1,/*m_colBackground*/RGB(255,0,0));
		pDC->FillSolidRect(rc1,m_colBackground);
		frc1.SetRect(1,1,frc1.Width(),frc.Height());
		DrawFolder(pDC,nFrom,frc1,FALSE);

		pDC->SelectObject(&bmpTo);
	//	pDC->FillSolidRect(rc1,/*m_colBackground*/RGB(255,255,0));
		pDC->FillSolidRect(rc1,m_colBackground);
		frc1.SetRect(1,1,frc1.Width(),frc.Height());
	//	DrawFolder(pDC,nTo,frc1,FALSE);
	}
	if(nTo>nFrom)
	{
		CRect rcFrom,rcTo;
		GetFolderRect(nFrom,&rcFrom);
		GetFolderRect(nTo,&rcTo);
     	for(int i=rcTo.top;i>=rcFrom.bottom;i-=m_nFolderHeight)
     	{
			pDC->SelectObject(&bmpFrom);
			dc.BitBlt(rcFrom.left,rcFrom.bottom+1,rcFrom.Width(),rc.top-1,pDC,0,m_nFolderHeight,SRCCOPY);
 			pDC->SelectObject(&bmpTo);
			dc.BitBlt(rc.left,i,rc.Width(),rc.bottom-i+m_nFolderHeight,pDC,0,0,SRCCOPY);
			Sleep(10);
		}
	}
	else
	{
		CRect rcFrom,rcTo;
		GetFolderRect(nFrom,&rcFrom);
		GetFolderRect(nTo,&rcTo);
		for(int i=rcFrom.bottom;i<=rc.bottom-m_nFolderHeight;i+=m_nFolderHeight)
		{
			pDC->SelectObject(&bmpFrom);
			dc.BitBlt(rc.left,i,rc.Width(),rc.bottom-i,pDC,0,0,SRCCOPY);
			pDC->SelectObject(&bmpTo);
			dc.BitBlt(rc.left,rc.top-m_nFolderHeight+1,rc.Width(),i-rc.top+m_nFolderHeight,pDC,0,0,SRCCOPY);
			Sleep(10);
		}
	}
	pDC->SelectObject(&bmpOld);
}
//
void CGfxOutBarCtrl::GetItemRect(CRect *rect,int nIndexItem,int nIndexFolder)
{
	if(nIndexFolder==-1)
		nIndexFolder=m_nSelectedFolder;

	GetInsideRect(rect);
	
	if(IsSmallIconView())
	{
		rect->SetRect(2,rect->top+1,2+16,2+16*nIndexItem);	
	}
	else
	{
		rect->SetRect(2,rect->top+1,2+32,2+32*nIndexItem);	
	}

}

void CGfxOutBarCtrl::DrawItem(CDC *pDC, int nIndexFolder, CRect rect, const int nIndex, const bool bOnlyImage)
{
	CBarFolder*pBf=(CBarFolder*)m_arrayFoldersCurrent.GetAt(nIndexFolder);
	CBarItem*pBi=(CBarItem*)pBf->arItems.GetAt(nIndex);
	if(IsSmallIconView())
	{
		m_pSmallImageList->Draw(pDC,pBi->nImageIndex,CPoint(rect.left,rect.top),ILD_NORMAL);
	}
	else
	{

	}

	CRect rcText;
	rcText.CopyRect(rect);
	rcText.left+=20;
	rcText.right+=100;
	rcText.top=(rect.bottom+50);
	rcText.bottom=(rcText.bottom + 30);
	CString str( pBi->szName);
	pDC->DrawText(str,&rcText,DT_SINGLELINE|DT_VCENTER|DT_LEFT);

}
BOOL CGfxOutBarCtrl::IsSmallIconView()
{
	return (m_dwFlag&fSmallIcon);
}

int CGfxOutBarCtrl::InsertItem(const int nIndexFolder, const int nIndexItem, const char *szItemName, const int nIndexImage, const DWORD dwData)
{
	CBarFolder *pBf=(CBarFolder*)m_arrayFoldersCurrent.GetAt(nIndexImage);
	if (pBf)
	{
		CBarItem* pBi=new CBarItem(szItemName,nIndexImage,dwData);
		pBf->arItems.Add((void*)pBi);
		
		return 1;
	}
	return 0;
}

BOOL CGfxOutBarCtrl::SetImageList(CImageList *pImageList, DWORD dwFlag)
{
	if(dwFlag&fLargeIcon)
	{
		m_pLargeImageList=pImageList;
		int yy=m_pLargeImageList->GetImageCount();
		return TRUE;
	}
	if(dwFlag&fSmallIcon)
	{
		m_pSmallImageList=pImageList;
		int yy=m_pSmallImageList->GetImageCount();
		return TRUE;
	}
	return FALSE;
}

void CGfxOutBarCtrl::GetInsideRect(CRect *rect)
{
	GetClientRect(rect);
	int nCurFolders=m_arrayFoldersCurrent.GetSize();
	if(nCurFolders>0)
	{
		rect->SetRect(0,1+(m_nSelectedFolder+1)*m_nFolderHeight,
			rect->Width(),rect->Height()-1-(nCurFolders-(m_nSelectedFolder+1))*m_nFolderHeight);
	}
}
