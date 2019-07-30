#pragma once
#include "afxwin.h"
#include "ImageButton.h"

//class CLuckMeDlg;

//ȡ������
class CQuKuanDlg : public CDialog
{
	DECLARE_DYNAMIC(CQuKuanDlg)

public:
	CQuKuanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQuKuanDlg();

// �Ի�������
	enum { IDD = IDD_DLG_QUKUAN };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnEnchangeQkjine();

	afx_msg void OnBnClickedBtnQukuan();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

private:
	Bitmap* m_bmpBk;
	Bitmap* m_bmpQuKuanBk;
	CPngImage m_ImgTitle;

	CSkinEditEx m_editQkAct;
	CSkinEditEx m_editQkPwd;
	CSkinEditEx m_editQkSxf;	//������
	CSkinEditEx m_editQkDzje;	//���˽��
	CImageButton m_btnQuKuan;
	CImageButton m_btnClose;

	CString m_yinhang;
	CString m_kaihuren;
	CString m_zhanghu;

	CFont m_font;
	CFont	m_font1;
	double m_tikuane;
	CString m_tikuan_pwd;

	BYTE m_cbIsVip;

	DOUBLE    m_fNeedXiaoFei;
	DOUBLE		m_fminQkJine;
	DOUBLE		m_fmaxQkJine;
	int			m_nQkSxfPercent;
	int			m_nMfQkCishu;
	DOUBLE		m_fMinSxf;
	DOUBLE		m_fMaxSxf;
	CString		m_strRemark;
	int			m_nStartHour;
	int			m_nEndHour;
	int			m_nLastCount;
	int			m_nAllCount;
	DOUBLE		m_fNowXiaofei;
	DOUBLE		m_fNeedXiaofei;
	//���غ���	
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetTiKuan;						//��ȡ�����Ϣ
	bool							m_bDoTiKuan;
	bool							m_bGetLastYue;
protected:
	VOID SendToServer(BYTE nSendType);
};
