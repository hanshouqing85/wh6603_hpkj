//Download by http://www.NewXing.com
// GfxOutBarCtrl.h: interface for the CGfxOutBarCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GFXOUTBARCTRL_H__7568387A_09E6_44B6_9FB3_B6685F92B4B1__INCLUDED_)
#define AFX_GFXOUTBARCTRL_H__7568387A_09E6_44B6_9FB3_B6685F92B4B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxcoll.h>
#include "..\resource.h"
class CGfxOutBarCtrl : public CWnd  
{
public:
	class CBarFolder
	{
	public :
		CBarFolder(const char *szName,DWORD dwData);
		virtual ~CBarFolder();
		int GetItemCount();
		int InsertItem(int nIndex,const char *szFolderName,const int nImage,const DWORD dwFolderData);
		
	public :
		char *szName;//Name
		DWORD dwData;//
		CImageList *pLargeImageList;//Big Image
		CImageList *pSmallImageList;//Small Image
		CPtrArray arItems;
		CWnd *pChild;
	};

	public:
	class CBarItem
	{
	public :
		CBarItem(const char* szItemName,const int nItemImageIndex,DWORD dwItemData);
		virtual ~CBarItem();

		char*szName;
		int nImageIndex;
		DWORD dwData;

	};
public:
	CGfxOutBarCtrl();
	virtual ~CGfxOutBarCtrl();

public:
	enum { htNothing=-1,htFolder,htItem};
	enum {  fSmallIcon = 1 << 0, fLargeIcon = 1 << 1, fEditGroups = 1 << 2, fEditItems = 1 << 3,
		fRemoveGroups = 1 << 4, fRemoveItems = 1 << 5, fAddGroups = 1 << 6, 
		fDragItems = 1 << 7, fAnimation = 1 << 8, fSelHighlight = 1 << 9 };

public://member
	CPtrArray m_arrayFoldersCurrent;//Folder List
    COLORREF m_colBackground;		//BackColor
	COLORREF m_colFolderBorderColor;
	int m_nFolderHeight;			//FolderHeight
	int m_nSelectedFolder;			//Selected Folder;
	HCURSOR m_hHandleCursor;
	DWORD m_dwFlag;
	int m_nItemHeight;
	CImageList *m_pSmallImageList;
	CImageList *m_pLargeImageList;

public://member function
	void GetInsideRect(CRect *rect);
	
	BOOL SetImageList(CImageList* pImageList,DWORD dwFlag);
	int InsertItem(const int nIndexFolder,const int nIndexItem,const char* szItemName,const int nIndexImage,const DWORD dwData);
	BOOL IsSmallIconView();
	void DrawItem(CDC* pDC,int nIndexFolder,CRect rect,const int nIndex,const bool bOnlyImage);
	void GetItemRect(CRect* rect,int nIndexItem,int nIndexFolder=-1);
	void AnimateFolderScroll(const int iFrom, const int iTo);
	int HitTestEx(CPoint pt,int &index);
	
	void GetFolderRect(const int nPos,CRect* rect);
	void DrawFolder(CDC* pDC,const int nPos,CRect rect,const bool bSelect);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd * pParentWnd, UINT nID, const DWORD dwFlag);
	int AddFolder(const char* cFolderName,const DWORD exPos);

protected:
	//{{AFX_MSG(CGfxOutBarCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GFXOUTBARCTRL_H__7568387A_09E6_44B6_9FB3_B6685F92B4B1__INCLUDED_)
