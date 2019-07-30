#pragma once
#include "afxwin.h"

// CExchangeDlg 对话框

class CExchangeDlg : public CDialog
{
	__int64					m_lCurrentScore;//当前的分数
	__int64					m_lExchangeChip;//将要兑换的筹码数
	CString							m_strChipNotice;
	CString							m_strChipState;

	// 对话框数据
	enum { IDD = IDD_EXCHANGEDLG };

	CSkinLayered m_SkinLayered;						//分层窗口
	CSkinButton m_BtnClose;
	CSkinButton m_BtnEx100;
	CSkinButton m_BtnEx1000;
	CSkinButton m_BtnEx10000;
	CSkinButton m_BtnExMax;
	CSkinButton m_BtnExClear;
	CSkinButton m_BtnOK;
	CSkinButton m_BtnCancel;

public:
	CExchangeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExchangeDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnChipclose();
	void RefreshChipState();
	afx_msg void OnBnClickedBtnEx100();
	afx_msg void OnBnClickedBtnEx1000();
	afx_msg void OnBnClickedBtnEx10000();
	afx_msg void OnBnClickedBtnExmax();
	afx_msg void OnBnClickedBtnExclear();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};
