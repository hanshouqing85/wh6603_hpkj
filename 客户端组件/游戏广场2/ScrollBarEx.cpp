// ScrollBarEx.cpp: implementation of the CScrollBarEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScrollBarEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)		// From WINUSER.H
#endif

#define BACK_COLOR                          RGB(44,61,117)
#define ARROW_COLOR                         RGB(44,61,117)
#define THUMB_COLOR                         RGB(92, 104, 166)
#define THUMB_HOVER_COLOR                   RGB(92, 104, 166)
#define FRAME_COLOR                         RGB(44,61,117)	// dark gray


#define TIMER_LEFTUP_LBUTTON_PRESSED		2	
#define TIMER_RIGHTDN_LBUTTON_PRESSED		3	
#define TIMER_PAGEUP_LBUTTON_PRESSED		4	
#define TIMER_PAGEDN_LBUTTON_PRESSED		5

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScrollBarEx, CLSASS_BASE)
ON_WM_PAINT()
ON_WM_DESTROY()
ON_WM_ERASEBKGND()
ON_WM_SIZE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
ON_WM_TIMER()
ON_WM_LBUTTONDBLCLK()
ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


CScrollBarEx::CScrollBarEx()
{
    m_pParent              = NULL;
    m_hCursor              = NULL;
    m_bHorizontal          = FALSE;
    m_rtBarClient          = CRect(-1,-1,-1,-1);
    m_rtLeftUpArrow        = CRect(-1,-1,-1,-1);
    m_rtRightDownArrow     = CRect(-1,-1,-1,-1);
    m_rtThumbClient        = CRect(-1,-1,-1,-1);
    m_clrArrowL            = ARROW_COLOR;
    m_clrArrowR            = ARROW_COLOR;
    m_clrThumb             = THUMB_COLOR;
    m_iCounter             = 0;
    m_bDragging            = FALSE;
    m_ptDragOffset         = 0;
    m_bLastHand            = FALSE;
    m_bHover               = FALSE;

    DWORD si[] = {sizeof(SCROLLINFO), SIF_ALL, 0, 100, 10, 0, 0};
    memcpy(&m_si, si, sizeof(SCROLLINFO));
}

CScrollBarEx::~CScrollBarEx()
{
}

BOOL CScrollBarEx::CreateFromWindow(DWORD dwStyle, CWnd* pParentWnd, HWND hWnd, UINT nId)
{
    ASSERT(pParentWnd);
    ASSERT(IsWindow(pParentWnd->m_hWnd));
    ASSERT(::IsWindow(hWnd));
    
    CRect rect;
    ::GetWindowRect(hWnd, &rect);
    pParentWnd->ScreenToClient(&rect);
    
    ::ShowWindow(hWnd, SW_HIDE);
    
	return CreateFromRect(dwStyle, pParentWnd, rect, nId);
}

#define ARROWSIZE 0
BOOL CScrollBarEx::CreateFromRect(DWORD dwStyle, CWnd* pParentWnd, CRect& rect, UINT nId)
{
    if (pParentWnd == NULL || !IsWindow(pParentWnd->m_hWnd)) _asm int 3;
    
    m_pParent = pParentWnd;
    m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
    m_bHorizontal = (dwStyle & SBS_VERT) ? FALSE : TRUE;
    
    return CLSASS_BASE::Create(AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW | CS_OWNDC, 0, 0, 0),
        _T(""), dwStyle, rect, pParentWnd, nId);
}

BOOL CScrollBarEx::OnEraseBkgnd(CDC* pDC) 
{
    return TRUE;	
}

void CScrollBarEx::OnDestroy() 
{
    CLSASS_BASE::OnDestroy();
    if (m_hCursor)
    {
        DestroyCursor(m_hCursor);
        m_hCursor = NULL;
    }
}

void CScrollBarEx::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    CMemDC memDc(&dc, &m_rtBarClient);

    // background
    CBrush bshBk(HS_BDIAGONAL, FRAME_COLOR);
    memDc->SetBkColor(BACK_COLOR);
    memDc->FillRect(m_rtBarClient, &bshBk);
//	memDc->RoundRect(m_rtThumb,CPoint(4,4));

    // arrow
    CBrush brshArrowL(m_clrArrowL);
    CBrush brshArrowR(m_clrArrowR);
    CPen  penFrame(PS_SOLID, 1, FRAME_COLOR);
    memDc->SelectObject(penFrame);
    memDc->SelectObject(brshArrowL);
    memDc->Rectangle(m_rtLeftUpArrow);
    memDc->SelectObject(brshArrowR);
    memDc->Rectangle(m_rtRightDownArrow);

	//m_clrThumb = RGB(255,255,255);
    // thumb
    CBrush brshThumb(m_clrThumb);
    m_rtThumb = m_rtThumbClient;
	memDc->RoundRect(m_rtThumb,CPoint(10,10));

	m_rtThumb.DeflateRect(2,2);

    memDc->SelectObject(brshThumb);
    if (m_bHorizontal)
    {
        int iPos = (m_si.nPos-m_si.nMin) * (m_rtBarClient.Width() - ARROWSIZE*2 - m_rtThumb.Width()) / (m_si.nMax - m_si.nMin);
        m_rtThumb.OffsetRect(iPos, 0);
    }
    else
    {
		if(m_si.nMax - m_si.nMin<=0)
			return;
        int iPos = (m_si.nPos-m_si.nMin) * (m_rtBarClient.Height() - ARROWSIZE*2 - m_rtThumb.Height()) / (m_si.nMax - m_si.nMin);
        m_rtThumb.OffsetRect(0, iPos);
    }
    memDc->Rectangle(m_rtThumb);

//     // gripper
//     if (m_rtThumb.Height() > 0 && m_rtThumb.Width() > 0)
//     {
//         if (m_bHorizontal)
//         {
//             memDc->MoveTo(m_rtThumb.CenterPoint().x - 2, 3);
//             memDc->LineTo(m_rtThumb.CenterPoint().x - 1, 3);
//             memDc->MoveTo(m_rtThumb.CenterPoint().x, 2);
//             memDc->LineTo(m_rtThumb.CenterPoint().x+1, 2);
//             memDc->MoveTo(m_rtThumb.CenterPoint().x + 2, 3);
//             memDc->LineTo(m_rtThumb.CenterPoint().x + 3,  3);
//         }
//         else
//         {
//             memDc->MoveTo(3, m_rtThumb.CenterPoint().y - 2);
//             memDc->LineTo(3, 4);
//             memDc->MoveTo(2, m_rtThumb.CenterPoint().y);
//             memDc->LineTo(2, 3);
//             memDc->MoveTo(3, m_rtThumb.CenterPoint().y + 2);
//             memDc->LineTo(3, 4);
//         }
//     }

    // frame, triangle
    CBrush brushFrame(FRAME_COLOR);
    memDc->FrameRect(m_rtBarClient, &brushFrame);
    memDc->FillRgn(&m_triLeftUpArrow, &brushFrame);
    memDc->FillRgn(&m_triRightDownArrow, &brushFrame);
}

void CScrollBarEx::OnSize(UINT nType, int cx, int cy) 
{
    CLSASS_BASE::OnSize(nType, cx, cy);
    if (m_si.nMax-m_si.nMin <= (int)m_si.nPage)
    {
        ShowWindow(SW_HIDE);
		return;
    }
    else
    {
        ShowWindow(SW_SHOW);
    }
    m_rtBarClient = CRect(0, 0, cx, cy);
    m_triLeftUpArrow.DeleteObject();
    m_triRightDownArrow.DeleteObject();
    POINT pt[3];

    m_rtLeftUpArrow = m_rtRightDownArrow = m_rtThumbClient = m_rtBarClient;
    if (m_bHorizontal)
    {
        m_rtLeftUpArrow.right = m_rtLeftUpArrow.left + ARROWSIZE;
        m_rtRightDownArrow.left = m_rtRightDownArrow.right - ARROWSIZE;
        pt[0].x = m_rtLeftUpArrow.CenterPoint().x - ARROWSIZE / 5;
        pt[0].y = m_rtLeftUpArrow.CenterPoint().y;
        pt[1].x = pt[0].x + 2 * ARROWSIZE / 5;
        pt[1].y = pt[0].y - m_rtLeftUpArrow.Height() / 3;
        pt[2].x = pt[1].x;
        pt[2].y = pt[1].y + 2 * m_rtLeftUpArrow.Height() / 3;
        m_triLeftUpArrow.CreatePolygonRgn(pt, 3, POLYFILL_LAST);
        pt[0].x = m_rtRightDownArrow.CenterPoint().x + ARROWSIZE / 5;
        pt[1].x = pt[0].x - 2 * ARROWSIZE / 5;
        pt[2].x = pt[1].x;
        m_triRightDownArrow.CreatePolygonRgn(pt, 3, POLYFILL_LAST);

        int iRange = m_si.nMax - m_si.nMin;
        int iThumbSize = m_si.nPage * (m_rtBarClient.Width() - 2*ARROWSIZE) / iRange;
        if (m_rtBarClient.Width() > ARROWSIZE*2 + 15)
        {
            iThumbSize = max(iThumbSize, 10);
        }
        else
        {
            iThumbSize = 0;
        }
        m_rtThumbClient.left = m_rtLeftUpArrow.Width();
        m_rtThumbClient.right = m_rtThumbClient.left + iThumbSize;
    }
    else
    {
        m_rtLeftUpArrow.bottom = m_rtLeftUpArrow.top + ARROWSIZE;
        m_rtRightDownArrow.top = m_rtRightDownArrow.bottom - ARROWSIZE;
        pt[0].x = m_rtLeftUpArrow.CenterPoint().x;
        pt[0].y = m_rtLeftUpArrow.CenterPoint().y - ARROWSIZE / 5;
        pt[1].x = pt[0].x + m_rtLeftUpArrow.Width() / 3;
        pt[1].y = pt[0].y + 2 * ARROWSIZE / 5;
        pt[2].x = pt[1].x - 2 * m_rtLeftUpArrow.Width() / 3;
        pt[2].y = pt[1].y;
        m_triLeftUpArrow.CreatePolygonRgn(pt, 3, POLYFILL_LAST);
        pt[0].y = m_rtRightDownArrow.CenterPoint().y + ARROWSIZE / 5;
        pt[1].y = pt[0].y - 2 * ARROWSIZE / 5;
        pt[2].y = pt[1].y;
        m_triRightDownArrow.CreatePolygonRgn(pt, 3, POLYFILL_LAST);

        int iRange = m_si.nMax - m_si.nMin;
        int iThumbSize = m_si.nPage * (m_rtBarClient.Height() - 2*ARROWSIZE) / iRange;
        if (m_rtBarClient.Height() > ARROWSIZE*2 + 15)
        {
            iThumbSize = max(iThumbSize, 10);
        }
        else
        {
            iThumbSize = 0;
        }
        m_rtThumbClient.top = m_rtLeftUpArrow.Height();
        m_rtThumbClient.bottom = m_rtThumbClient.top +  iThumbSize;
    }

    InvalidateRect(NULL, FALSE);
}

void CScrollBarEx::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    OnLButtonDown(nFlags, point);
}

void CScrollBarEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
    if (m_rtRightDownArrow.PtInRect(point))
    {
        ScrollToEnd();
    }
    else if (m_rtLeftUpArrow.PtInRect(point))
    {
        ScrollToHome();
    }
    else
    {
        CRect rtPageUp, rtPageDn;
        GetPageRect(rtPageUp, rtPageDn);
        if (rtPageUp.PtInRect(point) || rtPageDn.PtInRect(point))
        {
            ScrollTo(point);
        }
    }
}

void CScrollBarEx::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetCapture();
    if (m_rtThumb.PtInRect(point))
    {
        m_ptDragOffset = point - m_rtThumb.CenterPoint();
        m_bDragging = TRUE;
        m_clrThumb = ~THUMB_HOVER_COLOR & 0xffffff;
    }

    CRect rtPageUp, rtPageDn;
    GetPageRect(rtPageUp, rtPageDn);

    if (m_rtLeftUpArrow.PtInRect(point))
    {
        m_clrArrowL = (~m_clrArrowL) & 0xffffff;
        ScrollLeft();
        SetTimer(TIMER_LEFTUP_LBUTTON_PRESSED, 100, NULL);
        m_iCounter = 0;
    }
    else if (m_rtRightDownArrow.PtInRect(point))
    {
        m_clrArrowR = (~m_clrArrowR) & 0xffffff;
        ScrollRight();
        SetTimer(TIMER_RIGHTDN_LBUTTON_PRESSED, 100, NULL);
        m_iCounter = 0;
    }
    else if (rtPageUp.PtInRect(point))
    {
        ScrollPageUp();
        SetTimer(TIMER_PAGEUP_LBUTTON_PRESSED, 100, NULL);
        m_iCounter = 0;
    }
    else if (rtPageDn.PtInRect(point))
    {
        ScrollPageDn();
        SetTimer(TIMER_PAGEDN_LBUTTON_PRESSED, 100, NULL);
        m_iCounter = 0;
    }

    InvalidateRect(NULL, FALSE);

    CLSASS_BASE::OnLButtonDown(nFlags, point);
}

void CScrollBarEx::OnLButtonUp(UINT nFlags, CPoint point)
{
    ReleaseCapture();
    BOOL bDrag = m_bDragging;
    ResetButton();
    if (bDrag)
    {
        point.Offset(-m_ptDragOffset.x, -m_ptDragOffset.y);
        ScrollTo(point);
    }
    InvalidateRect(NULL, FALSE);

    CLSASS_BASE::OnLButtonUp(nFlags,point);
}

void CScrollBarEx::OnMouseMove(UINT nFlags, CPoint point)
{
    // track mouse event
    TRACKMOUSEEVENT tme;   
    tme.cbSize=sizeof(TRACKMOUSEEVENT);
    tme.dwFlags=/*TME_HOVER |*/ TME_LEAVE;   
    tme.hwndTrack = m_hWnd;  
    tme.dwHoverTime = 1;
    _TrackMouseEvent(&tme); 
    
    // set cursor
    if (m_rtLeftUpArrow.PtInRect(point) || m_rtRightDownArrow.PtInRect(point))
    {
        if (m_bLastHand == FALSE)
        {
            SetCursor(LoadCursor(NULL, IDC_HAND));
            m_bLastHand = TRUE;
        }
    }
    else
    {
        if (m_bLastHand)
        {
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            m_bLastHand = FALSE;
        }
    }

    // drag thumb
    if (m_bDragging)
    {
        point.Offset(-m_ptDragOffset.x, -m_ptDragOffset.y);
        ScrollTo(point);
        InvalidateRect(NULL, FALSE);
    }
    else
    {
        if (m_rtThumb.PtInRect(point))
        {
            if (m_bHover == FALSE)
            {
                m_clrThumb = THUMB_HOVER_COLOR;
                m_bHover = TRUE;
                InvalidateRect(m_rtThumb, FALSE);
            }
        }
        else
        {
            if (m_bHover == TRUE)
            {
                m_clrThumb = THUMB_COLOR;
                m_bHover = FALSE;
                InvalidateRect(m_rtThumb, FALSE);
            }
        }
    }
}

LRESULT CScrollBarEx::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
    return TRUE;
}

LRESULT CScrollBarEx::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    ResetButton();
    InvalidateRect(NULL, FALSE);
    return TRUE;
}

BOOL CScrollBarEx::GetScrollInfo(LPSCROLLINFO lpScrollInfo)
{
    *lpScrollInfo = m_si;
    return TRUE;
}

BOOL CScrollBarEx::SetScrollInfo(LPSCROLLINFO lpScrollInfo)
{
    m_si = *lpScrollInfo;
    CRect rt;
    GetClientRect(rt);
    PostMessage(WM_SIZE, SIZE_RESTORED, MAKELPARAM(rt.Width(), rt.Height()));
    return TRUE;
}

void CScrollBarEx::ScrollLeft()
{
    if (m_si.nPos > m_si.nMin)
    {
        m_si.nPos--;
        m_pParent->PostMessage(m_bHorizontal ? WM_HSCROLL : WM_VSCROLL, MAKELONG(SB_LINELEFT,0), (LPARAM)m_hWnd);
    }
}

void CScrollBarEx::ScrollRight()
{
    if (m_si.nPos < m_si.nMax)
    {
        m_si.nPos++;
        m_pParent->PostMessage(m_bHorizontal ? WM_HSCROLL : WM_VSCROLL, MAKELONG(SB_LINERIGHT,0), (LPARAM)m_hWnd);
    }
}

void CScrollBarEx::ScrollPageUp()
{
    if (m_si.nPos > m_si.nMin)
    {
        m_si.nPos -= m_si.nPage - 1;
        m_si.nPos = max(m_si.nPos, m_si.nMin);
        m_pParent->PostMessage(m_bHorizontal ? WM_HSCROLL : WM_VSCROLL, MAKELONG(SB_PAGEUP,0), (LPARAM)m_hWnd);
    }
}

void CScrollBarEx::ScrollPageDn()
{
    if (m_si.nPos < m_si.nMax)
    {
        m_si.nPos += m_si.nPage - 1;
        m_si.nPos = min(m_si.nPos, m_si.nMax);
        m_pParent->PostMessage(m_bHorizontal ? WM_HSCROLL : WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0), (LPARAM)m_hWnd);
    }
}

void CScrollBarEx::ScrollToEnd()
{
    m_si.nPos = m_si.nMax;
    m_pParent->PostMessage(m_bHorizontal ? WM_HSCROLL : WM_VSCROLL, MAKELONG(SB_BOTTOM,0), (LPARAM)m_hWnd);
    InvalidateRect(NULL, FALSE);
}

void CScrollBarEx::ScrollToHome()
{
    m_si.nPos = m_si.nMin;
    m_pParent->PostMessage(m_bHorizontal ? WM_HSCROLL : WM_VSCROLL, MAKELONG(SB_TOP,0), (LPARAM)m_hWnd);
    InvalidateRect(NULL, FALSE);
}

void CScrollBarEx::ScrollTo(CPoint point)
{
    point.Offset(-ARROWSIZE-m_rtThumb.Width()/2, -ARROWSIZE-m_rtThumb.Height()/2);

    if (m_bHorizontal)
    {
        m_si.nPos = point.x * (m_si.nMax-m_si.nMin) / (m_rtBarClient.Width()-ARROWSIZE*2-m_rtThumb.Width()) + m_si.nMin;
    }
    else
    {
        m_si.nPos = point.y * (m_si.nMax-m_si.nMin) / (m_rtBarClient.Height()-ARROWSIZE*2-m_rtThumb.Height()) + m_si.nMin;
    }
    m_si.nPos = max(m_si.nMin, m_si.nPos);
    m_si.nPos = min(m_si.nMax, m_si.nPos);
    if (m_bDragging)
    {
        m_pParent->PostMessage(m_bHorizontal ? WM_HSCROLL : WM_VSCROLL, MAKELONG(SB_THUMBTRACK,0), (LPARAM)m_hWnd);
    }
    else
    {
        m_pParent->PostMessage(m_bHorizontal ? WM_HSCROLL : WM_VSCROLL, MAKELONG(SB_THUMBPOSITION,0), (LPARAM)m_hWnd);
    }

    InvalidateRect(NULL, FALSE);
}

void CScrollBarEx::OnTimer(UINT nIDEvent)
{
    switch (nIDEvent)
    {
    case TIMER_LEFTUP_LBUTTON_PRESSED:
        m_iCounter++;
        if (m_iCounter > 5)
        {
            ScrollLeft();
        }
        break;
    case TIMER_RIGHTDN_LBUTTON_PRESSED:
        m_iCounter++;
        if (m_iCounter > 5)
        {
            ScrollRight();
        }
        break;
    case TIMER_PAGEUP_LBUTTON_PRESSED:
        m_iCounter++;
        if (m_iCounter > 5)
        {
            ScrollPageUp();
        }
        break;
    case TIMER_PAGEDN_LBUTTON_PRESSED:
        m_iCounter++;
        if (m_iCounter > 5)
        {
            ScrollPageDn();
        }
        break;
    }
    InvalidateRect(NULL, FALSE);
}

void CScrollBarEx::GetPageRect( CRect &rtPageUp, CRect &rtPageDn )
{
    if (m_bHorizontal)
    {
        rtPageUp.left       = m_rtLeftUpArrow.right;
        rtPageUp.top        = m_rtThumb.top;
        rtPageUp.right      = m_rtThumb.left;
        rtPageUp.bottom     = m_rtThumb.bottom;
        
        rtPageDn.left       = m_rtThumb.right;
        rtPageDn.top        = m_rtThumb.top;
        rtPageDn.right      = m_rtRightDownArrow.left;
        rtPageDn.bottom     = m_rtThumb.bottom;
    }
    else
    {
        rtPageUp.left       = m_rtThumb.left;
        rtPageUp.top        = m_rtLeftUpArrow.bottom;
        rtPageUp.right      = m_rtThumb.right;
        rtPageUp.bottom     = m_rtThumb.top;
        
        rtPageDn.left       = m_rtThumb.left;
        rtPageDn.top        = m_rtThumb.bottom;
        rtPageDn.right      = m_rtThumb.right;
        rtPageDn.bottom     = m_rtRightDownArrow.top;
    }
}

void CScrollBarEx::ResetButton()
{
    m_clrArrowL = m_clrArrowR = ARROW_COLOR;
    m_clrThumb = THUMB_COLOR;
    m_bDragging = FALSE;
    m_bLastHand = FALSE;
    m_bHover = FALSE;

    KillTimer(TIMER_LEFTUP_LBUTTON_PRESSED);
    KillTimer(TIMER_RIGHTDN_LBUTTON_PRESSED);
    KillTimer(TIMER_PAGEUP_LBUTTON_PRESSED);
    KillTimer(TIMER_PAGEDN_LBUTTON_PRESSED);
}

void CScrollBarEx::SetScrollPos(int iPos)
{
    iPos = min(m_si.nMax, iPos);
    iPos = max(m_si.nMin, iPos);
    m_si.nPos = iPos;
    InvalidateRect(NULL, FALSE);
}
