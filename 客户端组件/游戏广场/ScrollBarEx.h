// ScrollBarEx.h: interface for the CScrollBarEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCROLLBAREX_H__E6CB22A5_2E46_494A_829C_164889FE5C69__INCLUDED_)
#define AFX_SCROLLBAREX_H__E6CB22A5_2E46_494A_829C_164889FE5C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMemDC : public CDC 
{
private:
    CBitmap*	m_bitmap;
    CBitmap*	m_oldBitmap;
    CDC*		m_pDC;
    CRect		m_rcBounds;
public:
    CMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
    {
        CreateCompatibleDC(pDC);
        m_bitmap = new CBitmap;
        m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
        m_oldBitmap = SelectObject(m_bitmap);
        m_pDC = pDC;
        m_rcBounds = rcBounds;
    }
    ~CMemDC() 
    {
        m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
            this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
        SelectObject(m_oldBitmap);
        if (m_bitmap != NULL) 
            delete m_bitmap;
    }
    CMemDC* operator->() 
    {
        return this;
    }
};

#define CLSASS_BASE CWnd
class CScrollBarEx : public CLSASS_BASE  
{
public:
	CScrollBarEx();
	virtual ~CScrollBarEx();
    BOOL CreateFromWindow(DWORD dwStyle, CWnd* pParentWnd, HWND hWnd, UINT nId);
    BOOL CreateFromRect(DWORD dwStyle, CWnd* pParentWnd, CRect& rect, UINT nId);
    BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo);
    BOOL SetScrollInfo(LPSCROLLINFO lpScrollInfo);
    int  GetScrollPos() const {return m_si.nPos;} 
    void SetScrollPos(int iPos);

public:
    void ScrollLeft();
    void ScrollRight();
    void ScrollPageUp();
    void ScrollPageDn();
    void ScrollToHome();
    void ScrollToEnd();
    void ScrollTo(CPoint point);

protected:
    CWnd *		m_pParent;
    HCURSOR		m_hCursor;
    BOOL		m_bHorizontal;
    CRect		m_rtBarClient;
    CRect       m_rtLeftUpArrow;
    CRgn        m_triLeftUpArrow;
    CRect       m_rtRightDownArrow;
    CRgn        m_triRightDownArrow;
    CRect       m_rtThumbClient;
    CRect       m_rtThumb;
    SCROLLINFO  m_si;
    COLORREF    m_clrArrowL;
    COLORREF    m_clrArrowR;
    COLORREF    m_clrThumb;
    int         m_iCounter;
    BOOL        m_bDragging;
    CPoint      m_ptDragOffset;
    BOOL        m_bLastHand;
    BOOL        m_bHover;

protected:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);  
    afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);  
	afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()
       
    void ResetButton();
    void GetPageRect( CRect &rtPageUp, CRect &rtPageDn );
};

#endif // !defined(AFX_SCROLLBAREX_H__E6CB22A5_2E46_494A_829C_164889FE5C69__INCLUDED_)
