#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// ȷ��Ͷע �Ի���

class CQrTzhDlg : public CDialog
{
	DECLARE_DYNAMIC(CQrTzhDlg)

public:
	CQrTzhDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQrTzhDlg();

// �Ի�������
	enum { IDD = IDD_DLG_QRTZH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

public:
	CString m_gamekind;
	int m_zhushu;
	int m_moshi;
	DOUBLE m_zongjine;
	CPngImage	m_ImageBack;
private:
	Bitmap* m_bmpBk;
	CFont m_font;

	CImageButton m_btnOK;
	CImageButton m_btnCancel;
};
