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
#include <map>
// Ͷע��¼ �Ի���
#define IDM_REPLY				1000    //�ظ�
class CPlatformInbox : public CDialog
{
	DECLARE_DYNAMIC(CPlatformInbox)

public:
	CPlatformInbox(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlatformInbox();

// �Ի�������
	enum { IDD = IDD_DLG_PLATFORM_INBOX };
	map<int,CString> mapContent;
	map<int ,int> mapSendUser;
	DWORD m_dwTickCount;
	CImageButton m_btnClose;
	CString m_strName;
	CString m_strTitle;
	int			m_nUserID;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	afx_msg void OnPlatformInboxChk();
	afx_msg void OnPlatformInboxShc();
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

	int			m_nChkID;

	CFont m_font;
	CTime m_tStart;
	CTime m_tEnd;
	UINT m_qihao;
	int m_nTypeID;
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
	void SetTypeID(int nTypeID)
	{
		m_nTypeID = nTypeID;
	}

protected:
	bool							m_bGetTouzhuLogCount;						//��ȡ��־����
	bool							m_bGetTouzhuLog;							//��ȡ��־
	bool							m_bCancelTouzhu;							//����
protected:
	VOID SendToServer(int nSendType);


};
