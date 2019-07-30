#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageButton.h"
#include "TextButton.h"
#include "MessageDlg.h"
// CHuiYuanXjtzhtjDlg 对话框
#define IDM_QUERY_XJ_TZH			101
#include "DlgStatus.h"

class CHuiYuanXjtzhtjDlg : public CDialog, public IStatusViewSink//,public CMissionItem
{
	DECLARE_DYNAMIC(CHuiYuanXjtzhtjDlg)

public:
	CHuiYuanXjtzhtjDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHuiYuanXjtzhtjDlg();
	CDlgStatus						m_DlgStatus;						//状态窗口
	//取消连接
	virtual VOID OnStatusCancel();

// 对话框数据
	enum { IDD = IDD_DLG_HUIYUAN_XJTZHTJ };
	void ResetContent();
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnNextPage();

	afx_msg void OnXjtzhtjChkxj();
	afx_msg void OnNMRClickListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMLclickListLog(NMHDR *pNMHDR, LRESULT *pResult);
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

	afx_msg void OnXjtzhtjChkxx();

	afx_msg void OnXjtzhtjChkmx();
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

private:
	Bitmap* m_bmpBk;
	DWORD m_dwTickCount;
	CDateTimeCtrl m_dateStart;
	CDateTimeCtrl m_dateEnd;
	CListCtrlCl m_listLog;
	CImageButton m_btnOK;
	CImageButton m_btnPrePage;
	CImageButton m_btnNextPage;

	CTextButton	m_btnChkxj[10];
	int m_nIndex;

	CFont m_font;
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

	//CMissionManager					m_MissionManager;					//任务管理
	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetXjtzhtjCount;						//获取日志数量
	bool							m_bGetXjtzhtj;							//获取日志

	bool							m_bGetXjtzhtjByID;						//获取日志通过ID
	bool							m_bGetXjtzhtjByAct;						//获取日志通过Act
protected:
	VOID SendToServer(int nSendType);

};
