#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "ImgRioButton.h"
#include "TextButton.h"
// ȷ��Ͷע �Ի���

class CZhanNeiXinXX : public CDialog
{
	DECLARE_DYNAMIC(CZhanNeiXinXX)

public:
	CZhanNeiXinXX(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZhanNeiXinXX();

// �Ի�������
	enum { IDD = IDD_DLG_ZHANNEIXINXX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

public:
	CString m_gamekind;
	DOUBLE	m_fJine;
	DOUBLE	m_fQpJine;
	CString strTitle;
	CString strName;
	CString strContent;
	CString strTime;
	CRichEditCtrl m_richContent;

	DOUBLE m_fJiaoYiJine;
	int			m_nType;
	void SetContent(CString szTitle,CString szName,CString szContent,CString szTime,int nType);
private:
	Bitmap* m_bmpBk;
	Bitmap* m_bmpTile;
	CFont m_font;
	CFont m_btnfont;

	CImageButton m_btnOK;
	CImageButton m_btnCancel;



};
