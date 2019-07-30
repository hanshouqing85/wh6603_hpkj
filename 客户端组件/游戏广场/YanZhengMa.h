#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#define IDM_SEND_RANDNUM 10001
// 确认投注 对话框

class CYanZhengMaDlg : public CDialog
{
	DECLARE_DYNAMIC(CYanZhengMaDlg)

public:
	CYanZhengMaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CYanZhengMaDlg();

// 对话框数据
	enum { IDD = IDD_DLG_YANZHENGMA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnHqyzm();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();
	CSkinComboBox m_cmbZzType;
	CFont m_cmbfont;

public:
	CString m_gamekind;
	DOUBLE	m_fJine;
	CSkinEditEx m_editYanZhengma;
	CSkinEdit m_editPassWord;
	CString m_strPhoneNum;
	CString m_strInput;
	BYTE     m_cbNetWork;
	BYTE		m_cbOperateType;
	int		m_nRandNum;
	int		m_nEscape;
	DOUBLE m_fJiaoYiJine;
	BYTE			m_cbType;
	TCHAR		m_szPassWord[33];
	bool ChangeStringToA(CString strInput, string&  strOutput);
	CString GetYanZhengma();
	void SetYanZhengXinxi(CString strNum,BYTE IsNetwork,BYTE cType);
private:
	Bitmap* m_bmpBk;
	CFont m_font;
	CFont m_font1;
	CImageButton m_btnHqyzm;//获取验证码
	CImageButton m_btnOK;
	CImageButton m_btnCancel;
};
