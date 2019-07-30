#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "ImgRioButton.h"

// CHuiYuanTjxjDlg �Ի���

class CHuiYuanTjxjDlg : public CDialog//,public CMissionItem
{
	DECLARE_DYNAMIC(CHuiYuanTjxjDlg)

public:
	CHuiYuanTjxjDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuiYuanTjxjDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HUIYUAN_TJXJ };
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnBnClickedRioDaili();
	afx_msg void OnBnClickedRioHuiyuan();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedSetWebFandian();
	afx_msg void OnBnClickedSetMorenMima();
	afx_msg void OnBnClickedBtnCpRegurl();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void AdjustCtrls();
	void EnableCtrls(BOOL b=TRUE);
	void InitFanDianCmb();

private:
	Bitmap* m_bmpBk;
	CString m_strPeie;
	int m_type;	//�¼����
	double m_fandian;
	CString m_zhanghu;
	CString m_password;
	CString m_crmpwd;
	CString m_qkpwd;
	CString m_crmqkpwd;

	CSkinEditEx m_editZhanghu;
	CSkinEditEx	m_editPassword;
	CSkinEditEx m_editCrmPwd;
	CSkinEditEx	m_editQkPwd;
	CSkinEditEx m_editCrmQkPwd;

	CImageButton m_btnOK;

	CImgRioButton m_rioDaili;
	CImgRioButton m_rioHuiYuan;

	CComboBox m_cmbFandian;
	CString m_strFandian;
	CComboBox m_cmbWebFandian;
	CString m_strWebFandian;
	CComboBox m_cmbWebRegUrl;

	CImageButton m_btnRegUrl;
	CImageButton m_btnSetWebFandian;
	CImageButton m_btnSetMorenMima;
	CString m_strRegUrl;
	CString m_strWebFandian1;
	CFont m_font;
	CSkinEditEx m_editQQ;
	CString m_strQQ;

	//CMissionManager					m_MissionManager;					//�������
 	//���غ���
 public:
//  	//�����¼�
//  	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
//  	//�ر��¼�
//  	virtual bool OnEventMissionShut(BYTE cbShutReason);
 	//��ȡ�¼�
 	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
 protected:
 	bool							m_bGetRegURL;						//��ȡע������
 	bool							m_AddHuiYuan;							//����¼�
	bool							m_bSetWebFandian;//������ҳע�᷵��
 protected:
 	VOID SendToServer(int nSendType);

};
