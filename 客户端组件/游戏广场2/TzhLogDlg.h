#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageButton.h"
#include "GameRule.h"
#include "comm.h"
#include "MessageDlg.h"
#include "PaiLie3.h"
#include "3D.h"

// Ͷע��¼ �Ի���

class CTzhLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CTzhLogDlg)

public:
	CTzhLogDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTzhLogDlg();

// �Ի�������
	enum { IDD = IDD_DLG_TZHLOG };

	CChqSSCRule m_chqRule;
	CJxSSCRule m_jxRule;
	CXJSSCRule m_xjRule;
	CFenFenCaiRule m_ffcRule;
	CErFenCaiRule m_efcRule;
	CWuFenCaiRule m_wfcRule;
	CHgydwfcRule	m_hgRule;
	CCQ11X5Rule m_chq11x5Rule;
	CGD11X5Rule m_gd11x5Rule;
	CJX11X5Rule m_jx11x5Rule;
	CSD11X5Rule m_sd11x5Rule;
	CHLJ11X5Rule m_hlj11x5Rule;
	CQiXingCaiRule m_qxcRule;
	CPaiLie3 m_dlgPaiLie3;
	CCanadaRule	m_jndRule;
	CKuaiLe8RUle	m_bj5fcRule;
	CFont m_cmbfont;
	CTXFenFenCaiRule m_txffcRule;
	CQQFenFenCaiRule m_qqffcRule;

	C3D	m_dlg3D;	
	CBJPK10Rule m_pk10Rule;

	DWORD m_dwTickCount;
	CImageButton m_btnClose;

	CSkinComboBox m_cmbSort;
	CSkinComboBox m_cmbStatus;
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}
	CPngImage m_ImgTitle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnNextPage();
	afx_msg void OnBnClickedBtnClose();

	afx_msg void OnNMDblclkListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMLclickListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTouzhuLogChakanxx();
	afx_msg void OnTouzhuLogChexiao();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void InitListCtrl();
	void AdjustCtrls();

private:
	Bitmap* m_bmpBk;

	CDateTimeCtrl m_dateStart;
	CDateTimeCtrl m_dateEnd;
	CEdit m_editQihao;
	CListCtrlCl m_listLog;
	CImageButton m_btnOK;
	CImageButton m_btnPrePage;
	CImageButton m_btnNextPage;

	CFont m_font;
	CTime m_tStart;
	CTime m_tEnd;
	UINT m_qihao;

	int m_page;
	int m_itemCount;
	int m_maxPage;
	BOOL m_byTime;

	int m_nCancelID;
	int m_nCancelKind;
	CString m_strQihao;
	//���غ���
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetTouzhuLogCount;						//��ȡ��־����
	bool							m_bGetTouzhuLog;							//��ȡ��־
	bool							m_bCancelTouzhu;							//����
protected:
	VOID SendToServer(int nSendType);


};
