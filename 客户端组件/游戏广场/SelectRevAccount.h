#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "ImgRioButton.h"
#include "TextButton.h"
#include <map>
// CXgfdDlg 对话框

#define IDM_NEXT_PAGE  300
#define IDM_SELECT_USER  301
#define IDM_CANCEL_SEL  302
class CSelectRevAccount : public CDialog
{
	DECLARE_DYNAMIC(CSelectRevAccount)

public:
	CSelectRevAccount(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectRevAccount();
	Bitmap* m_btClose;
	int m_nCloseIndex;
	map<int,CString> mapName;
	map<int,int> mapIndex;
	CTextButton		m_btnImgRioUser[30];
	bool IsPushed(int nIndex);
	void SetPushed(int nIndex);
// 对话框数据
	enum { IDD = IDD_DLG_SELECT_ACT };
	CFont m_font;
	CImageButton m_btnPrePage;
	CImageButton m_btnNextPage;
	void HideAllButton();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void DrawCloseBtn(CDC* pDC, Graphics& graphics);
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	VOID OnMouseMove(UINT nFlags, CPoint Point);

public:
	void SetOpenRecord(CMD_GP_GetMoreRecordRet* pMoreRecord,int nIndex,bool b11xuan5 = false);
 	Bitmap* m_bmpBk;
	void SetButtonText(int nIndex,CString strName,int nPage,int nMaxPage);
	CImageButton m_btnOK;
	CImageButton m_btnCancel;
	CImgRioButton m_btnSelectAll;

	CImageButton m_btnSearch;
	CSkinEditEx m_edSearch;
	int   m_nPage;
	int	m_nMaxPage;
	void AdjustButton();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnNextPage();
	afx_msg void OnBnClickedBtnSelAll();
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnSearch();
	DECLARE_MESSAGE_MAP()

};
