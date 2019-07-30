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

// CHuiYuanChkxjtzhDlg �Ի���

class CHuiYuanChkxjtzhDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuiYuanChkxjtzhDlg)

public:
	CHuiYuanChkxjtzhDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuiYuanChkxjtzhDlg();
	void ResetContent();
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

// �Ի�������
	enum { IDD = IDD_DLG_HUIYUAN_CHKXJTZH };
	CChqSSCRule m_chqRule;
	CJxSSCRule m_jxRule;
	CXJSSCRule m_xjRule;
	CFenFenCaiRule m_ffcRule;
	CErFenCaiRule m_efcRule;
	CTXFenFenCaiRule m_txffcRule;
	CQQFenFenCaiRule m_qqffcRule;
	CWuFenCaiRule m_wfcRule;
	CHgydwfcRule	m_hgRule;
	CCanadaRule	m_jndRule;
	CCQ11X5Rule m_chq11x5Rule;
	CGD11X5Rule m_gd11x5Rule;
	CJX11X5Rule m_jx11x5Rule;
	CSD11X5Rule m_sd11x5Rule;
	CBJPK10Rule m_pk10Rule;
	CHLJ11X5Rule m_hlj11x5Rule;
	CQiXingCaiRule m_qxcRule;	
	CKuaiLe8RUle	m_bj5fcRule;
	CPaiLie3 m_dlgPaiLie3;
	C3D	m_dlg3D;
	CFont m_cmbfont;

	CSkinComboBox m_cmbSort;
	CSkinComboBox m_cmbStatus;

	DWORD  m_dwTickCount;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnNextPage();
	
	afx_msg void OnNMDblclkListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChkxjtzhChkxj();
	afx_msg void OnNMRClickListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChkxjtzhChkxx();
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

	CDateTimeCtrl m_dateStart;
	CDateTimeCtrl m_dateEnd;
	CListCtrlCl m_listLog;
	CImageButton m_btnOK;
	CImageButton m_btnPrePage;
	CImageButton m_btnNextPage;

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
	//���غ���
public:
// 	//�����¼�
// 	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
// 	//�ر��¼�
// 	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	void QueryChkXjTzh(int n_t_id);
protected:
	bool							m_bGetXjtzhLogCount;						//��ȡ��־����
	bool							m_bGetXjtzhLog;							//��ȡ��־

	bool							m_bGetXjtzhtjByID;						//��ȡ��־ͨ��ID
	bool							m_bGetXjtzhtjByAct;						//��ȡ��־ͨ��Act
protected:
	VOID SendToServer(int nSendType);

};
