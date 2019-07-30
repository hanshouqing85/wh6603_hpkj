#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "MessageDlg.h"
#include "DlgLogon.h"
//�޸�����
class CZhangHuXgmmDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuXgmmDlg)

public:
	CZhangHuXgmmDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZhangHuXgmmDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ZHANGHU_XGMM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnOk2();
	afx_msg void OnBnClickedBtnLockMachine();
	afx_msg void OnBnClickedBtnBdPhone();
	afx_msg void OnBnClickedBtnQrxg();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

private:
	void AdjustCtrls();
private:
	Bitmap* m_bmpBk;
	DWORD m_dwTickCount;
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}


	CString m_old_pwd;
	CString m_new_pwd;
	CString m_confirm_pwd;
	CString m_strPhoneNum;
	CSkinEditEx m_editOldPwd;
	CSkinEditEx m_editNewPwd;
	CSkinEditEx m_editConfirmPwd;
	CSkinEditEx m_editPhone;
	CControlCheckButton m_btnLogonCheck;				//��½��֤ѡ��
	CControlCheckButton m_btnTransferCheck;			//ת����֤ѡ��
	CControlCheckButton m_btnStockCheck;			//ת����֤ѡ��
	CImageButton m_btnDlOK;
	CImageButton m_btnBdPhone;
	CImageButton m_btnQrXg;

	CString m_old_pwd_qk;
	CString m_new_pwd_qk;
	CString m_confirm_pwd_qk;
	CSkinEditEx m_editOldPwdQk;
	CSkinEditEx m_editNewPwdQk;
	CSkinEditEx m_editConfirmPwdQk;
	CImageButton m_btnQkOK;

	CImageButton m_btnLockMachine;
	CFont m_font;
	BYTE	m_cbOperateType;
	BYTE	m_cbBangDing;
protected:
	virtual void OnCancel();
	virtual void OnOK();
	//���غ���
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bAlterLogonPass;						//��ȡ��־����
	bool							m_bAlterQukuanPass;							//��ȡ��־
protected:
	VOID SendToServer(int nSendType);

};
