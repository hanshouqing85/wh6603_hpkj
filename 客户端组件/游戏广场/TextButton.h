#pragma once


//具有图片背景的文字按钮
class CTextButton : public CButton
{
	DECLARE_DYNAMIC(CTextButton)

public:
	CTextButton();
	virtual ~CTextButton();

public:
	void SetBkImage(LPCTSTR imgFile);
	void SetPushed(bool bPushed);
	void SetBtnSize(int width, int height) {m_width = width; m_height = height;}
	int Width() {return m_width;}
	int Height() {return m_height;}
	void SetTextColor(COLORREF color) {m_TextClr = color;};
	void SetTextFont(CFont* font) {m_font = font;};
	void EnableTextButton(bool bEnable);
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

private:
	void PaintText(CDC* pDC);

private:
	Bitmap* m_pIamge;

	bool	m_bHaveBitmaps;
	int		m_width;
	int		m_height;

	CDC memDC; 	bool m_bDCStored; 
	bool m_bIsHovering;
	bool m_bIsTracking;
	bool m_bIsDisabled;

	bool m_bPushed;

	COLORREF m_TextClr;
	CFont* m_font;
	CFont m_font1;
};



