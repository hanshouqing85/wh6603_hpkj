#pragma once
#include "afxwin.h"
#include "ImageButton.h"

#include "MessageDlg.h"
// CZhangHuZhhbhDlg �Ի���

class CZhangHuZhhbhDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuZhhbhDlg)

public:
	CZhangHuZhhbhDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZhangHuZhhbhDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ZHANGHU_ZHHBH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

private:
	void AdjustCtrls();
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

private:
	Bitmap* m_bmpBk;
	CImageButton m_btnOK;
	CFont m_font;
	DWORD m_dwTickCount;
	CString m_pwd_qk;
	CString m_mb_ask;
	CString m_mb_answer;

	CSkinEditEx  m_editPwdQk;
	CSkinEditEx	m_editMbAsk;
	CSkinEditEx	m_editMbAnswer;
protected:
	virtual void OnCancel();
	virtual void OnOK();
	//���غ���
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bSetQuKuanProtect;						//����ȡ���
	bool							m_bQueryMyProtect;						//����ȡ���
	
protected:
	VOID SendToServer(int nSendType);

};
