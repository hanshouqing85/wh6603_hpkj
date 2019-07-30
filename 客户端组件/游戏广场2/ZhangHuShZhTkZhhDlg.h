#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// ��������˻� �Ի���

class CZhangHuShZhTkZhhDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuShZhTkZhhDlg)

public:
	CZhangHuShZhTkZhhDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZhangHuShZhTkZhhDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ZHANGHU_SHZTKZHH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedBtnOk();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

private:
	void AdjustCtrls();
	void EnableCtrls(BOOL b=TRUE);
	void SetYHCmbSel(const CString& khyh);

private:
	Bitmap* m_bmpBk;
	DWORD m_dwTickCount;
	CComboBox m_cmbKhyh;
	CString m_khyh;	//��������
	CString m_khr;	//������
	CString m_yhzhh;//�����˺�
	CString m_qkmm;	//ȡ������

	
	CSkinEditEx	m_editKhr;
	CSkinEditEx	m_editYhzhh;
	CSkinEditEx	m_editQkmm;
	CImageButton m_btnOK;

	CFont m_font;
protected:
	virtual void OnOK();
	virtual void OnCancel();
	//���غ���
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bSetQuKuanZhanghao;						//����ȡ���
	bool							m_bQueryYinHang;						//��ѯ����
	bool							m_bQueryMyYinHang;						//��ѯ����
protected:
	VOID SendToServer(int nSendType);

};
