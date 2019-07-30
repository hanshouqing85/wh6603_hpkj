/*
½éÉÜ£º¼òÒ×Æ¯ÁÁµÄQQ³éÌëÊ½²Ëµ¥

Author:		Mario
Data:		2009-12-30
Email:		yemingwy@sina.com
*/

#if !defined(AFX_QMTAB_H__8B1EF756_1090_41CD_81F6_B5B2F3567B4D__INCLUDED_)
#define AFX_QMTAB_H__8B1EF756_1090_41CD_81F6_B5B2F3567B4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QMTab.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CQMTab window
#define QMT_MESSAGE							0x00000001
#define QMT_CFG_ICON_SMALL					0X00000010
#define QMT_CFG_ICON_BIG					0X00000100

#define QMTAB_CFG_FRAMEBORDER_TOPLEFT		RGB(33,134,139)
#define QMTAB_CFG_FRAMEBORDER_BOTTOMRIGHT	RGB(0,0,0)
#define QMTAB_CFG_BACKGROUND				RGB(60,83,147)

#define QMTAB_CFG_ARROWUP					-99
#define QMTAB_CFG_ARROWDOWN					-98
#define QMTAB_CFG_ARROWWIDTH				15
#define QMTAB_CFG_ARROWHEIGHT				28
#define QMTAB_CFG_ARROW_MASKCOLOR			RGB(0,0,255)

#define QMTAB_CFG_BTNCOLOR					RGB(44,61,147)
#define QMTAB_CFG_BTNCOLOR_OVER				RGB(54,75,131)
#define QMTAB_CFG_BTNBORDER					RGB(0,0,0)
#define QMTAB_CFG_BTNBORDER_TOPLEFT			RGB(255,255,255)

#define QMTAB_CFG_TIMER_FIXPOS				1001
#define QMTAB_CFG_TIMER_INTERVAL			20
#define QMTAB_CFG_MOVE_STEP					0.5

#define QMTAB_CFG_ITEM_HEIGHT				3

#define QMTAB_CFG_DIR_UP					1
#define QMTAB_CFG_DIR_DOWN					2

#define QMTAB_CFG_DEFAULTTXTCOLOR			RGB(0,0,0)
#include "Resource.h"
struct _QMTAB_ITEM
{
	char lpsItemName[30];
	TCHAR lpsItemImg[30];
	int nImage;
	int nData;
	COLORREF txtColor;
	_QMTAB_ITEM *pNext;

	_QMTAB_ITEM()
	{
		pNext = NULL;
	}
};

struct _QMTAB_TAB
{
	CPngImage* img;
	char lpsTabName[30];
	char lpsTabImg[30];
	int nImgIndex;
	RECT rect;
	int nPageIdx;
	_QMTAB_ITEM *item;
};

class CQMTab : public CWnd
{
// Construction
public:
	CQMTab();
	CPngImage	m_ImageBack;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQMTab)
	//}}AFX_VIRTUAL

private:
	int GetMaxTabItemCount();
	void ArrowMoveDown();
	void ArrowMoveUp();
	void GetArrowPosition( LPRECT rcUp, LPRECT rcDown );
	void CheckItemFocus( LPPOINT lpPoint );
	void GetTabAvailableArea( int nTabID, LPRECT rcArea );
	int GetTabItemCount( int nTabID );
	void AdjustPos();
	void CQMTab::DeleteSubItem( _QMTAB_ITEM *pItem );
	void GetDataAreaRect( LPRECT rect, LPRECT rectArea = NULL );
	void ChangeCursorTable(int nItemID, int nType);
	int GetEventItem( LPPOINT point );
	void DrawTabText( LPRECT rcItem, LPCTSTR lpzTabText, CDC *pMemDc, BOOL bFocus );
	void DrawItems(CDC *pMemDc);
	void GetTabItemRect( int nItem, LPRECT rect );
	void DrawItem( CDC *pMemDc );
	void DrawTabs( CDC *pMemDc );
	void DrawFrame( CDC *pMemDc );
	COLORREF m_TextColor;
	CArray <_QMTAB_TAB *, _QMTAB_TAB *> m_arTabs;
	DWORD m_nTabHeight;
	CImageList m_arrowList;
	CBitmap m_bitmapArrow;
	CPngImage m_ImgGroup;
	CPngImage m_ImgTabBack;
	BOOL m_bMoving, m_bMovingUp;
	BOOL m_bMouseDown;
	int m_nSelectedTab, m_nFocusTab, m_nMouseDownTab;
	int m_iSelectedItem;
	CImageList *m_pImgList;
	CRect m_rcTab;
	CSize m_iconSize;
	CRect m_rcClient;
	CFont m_fntTab, m_fntTabSel;
	CWnd *m_pParent;
// Implementation
public:
	UINT m_uCallbackMessage;
	UINT m_nFlag;
	void SetTextColor( COLORREF color );
	BOOL AddItem( int nTabID, LPCTSTR lpzBtnName, int nImage,LPCTSTR lpzBtnImg, int nData = -1, COLORREF txtColor = QMTAB_CFG_DEFAULTTXTCOLOR );
	void SetImageList( CImageList *pImageList );
	void SetCurSorTab( int nTabID );
	BOOL SetTabHeight( DWORD nHeight );
	BOOL IsCorrectPos();
	BOOL CreateWnd( DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL );
	int AddTab( LPCTSTR sTabName ,LPCTSTR sTabImg,int nIndex);
	virtual ~CQMTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CQMTab)
	afx_msg   BOOL   OnMouseWheel(   UINT   nFlags,   short   zDelta,   CPoint   pt   );

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QMTAB_H__8B1EF756_1090_41CD_81F6_B5B2F3567B4D__INCLUDED_)
