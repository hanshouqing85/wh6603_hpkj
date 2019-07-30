#ifndef MY_SKIN_SCROLL_BAR_HEAD_FILE
#define MY_SKIN_SCROLL_BAR_HEAD_FILE

#pragma once

#include "StdAfx.h"
/////////////////////////////////////////////////////////////////////////////
// CSkinScrollBar window
class CMySkinScrollBar : public CScrollBar
{
// Construction
public:
	CMySkinScrollBar();
	HBITMAP	m_hBmp;
	int		m_nWid;
	int		m_nFrmHei;
	int		m_nHei;

	SCROLLINFO	m_si;
	BOOL		m_bDrag;
	CPoint		m_ptDrag;
	int			m_nDragPos;

	UINT		m_uClicked;
	BOOL		m_bNotify;
	UINT		m_uHtPrev;
	BOOL		m_bPause;
	BOOL		m_bTrace;
// Attributes
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Implementation
public:
	void DrawArrow(UINT uArrow,int nState);
	void SetBitmap(HBITMAP hBmp);
	BOOL IsVertical();
	virtual ~CMySkinScrollBar();

	// Generated message map functions
protected:
	void DrawThumb(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC, RECT *pSourRect);
	void TileBlt(CDC *pDestDC,RECT *pDestRect,CDC *pSourDC,RECT *pSourRect);
	RECT GetRect(UINT uSBCode);
	RECT GetImageRect(UINT uSBCode,int nState=0);
	UINT HitTest(CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
