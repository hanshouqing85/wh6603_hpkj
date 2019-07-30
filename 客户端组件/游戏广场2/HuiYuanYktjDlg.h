#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageButton.h"
#include "MessageDlg.h"
#include "TextButton.h"
#include "DlgStatus.h"
#define IDM_QUERY_XJ_YKMX			100
// CHuiYuanYktjDlg �Ի���

class CHuiYuanYktjDlg : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CHuiYuanYktjDlg)

public:
	CHuiYuanYktjDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuiYuanYktjDlg();
	CDlgStatus						m_DlgStatus;						//״̬����
	//ȡ������
	virtual VOID OnStatusCancel();
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

// �Ի�������
	enum { IDD = IDD_DLG_HUIYUAN_YKTJ };
	void ResetContent();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnNextPage();
	afx_msg void OnXjyktjChkxx();
	afx_msg void OnBnClickedBtnChkxj1();
	afx_msg void OnBnClickedBtnChkxj2();
	afx_msg void OnBnClickedBtnChkxj3();
	afx_msg void OnBnClickedBtnChkxj4();
	afx_msg void OnBnClickedBtnChkxj5();
	afx_msg void OnBnClickedBtnChkxj6();
	afx_msg void OnBnClickedBtnChkxj7();
	afx_msg void OnBnClickedBtnChkxj8();
	afx_msg void OnBnClickedBtnChkxj9();
	afx_msg void OnBnClickedBtnChkxj10();
	int m_nIndex;

	afx_msg void OnXjyktjChkmx();
	afx_msg void OnNMRClickListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMLclickListLog(NMHDR *pNMHDR, LRESULT *pResult);
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
	void EnAbleCtrls(bool b);

	CTextButton	m_btnChkxj[10];

private:
	Bitmap* m_bmpBk;
	DWORD m_dwTickCount;
	CDateTimeCtrl m_dateStart;
	CDateTimeCtrl m_dateEnd;
	CListCtrlCl m_listLog;
	CImageButton m_btnOK;
	CImageButton m_btnPrePage;
	CImageButton m_btnNextPage;

	CFont m_font;
	CFont m_cmbfont;
	CTime m_tStart;
	CTime m_tEnd;
	
	int m_page;
	int m_itemCount;
	int m_maxPage;
	BOOL m_byTime;

	int m_user_id;
	int m_byType;
	CSkinEditEx m_editAct;
	CSkinEditEx m_editID;
	CString m_strAct;
	CString m_strID;

	CSkinComboBox m_cmbSort;

	int m_nTestCount;
	//���غ���
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetXjYkTjCount;						//��ȡ��־����
	bool							m_bGetXjYkTj;						//��ȡ��־����
	bool							m_bGetXjYkTjByID;						//��ȡ��־����ͨ��ID
	bool							m_bGetXjYkTjByAct;						//��ȡ��־����ͨ���˻�
protected:
	VOID SendToServer(int nSendType);
};
