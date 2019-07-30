#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// ȷ��Ͷע �Ի���

class CZhuanZhangDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhuanZhangDlg)

public:
	CZhuanZhangDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZhuanZhangDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HYXX_ZHUANZHANG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();
	CSkinComboBox m_cmbZzType;
	CFont m_cmbfont;

public:
	CString m_gamekind;
	DOUBLE	m_fJine;
	CSkinEdit m_editJine;
	CSkinEdit m_editPassWord;
	CString m_strName;
	int		m_n_t_userid;
	int		m_n_t_target_userid;
	DOUBLE m_fJiaoYiJine;
	int			m_nType;
	TCHAR		m_szPassWord[33];
	//ѡ��ı�
	VOID OnLvnItemChangedEdit();

	void SetZhuanZhangXinxi(int n_t_userid,CString strName,int n_t_target_userid,DOUBLE fJine);
private:
	Bitmap* m_bmpBk;
	CFont m_font;

	CImageButton m_btnOK;
	CImageButton m_btnCancel;
};
