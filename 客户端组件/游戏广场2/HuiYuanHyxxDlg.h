#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageButton.h"
#include "DlgLogon.h"
#include "MessageDlg.h"
#include "TextButton.h"
// CHuiYuanHyxxDlg �Ի���
//��ѡ�ؼ�
// class  CControlCheckButton
// {
// 	//״̬����
// protected:
// 	BYTE                           m_cbChecked;                        //��ס����
// 	CWnd *                         m_pParentSink;                      //����ָ��
// 
// 	//λ�ñ���
// protected:
// 	CPoint                         m_ptControlBenchmark;               //��׼λ��
// 
// 	//��Դ����
// protected:
// 	CBitImage                      m_ImageBtnBack;                     //������Դ
// 
// 	//��������
// public:
// 	//���캯��
// 	CControlCheckButton();
// 	//��������
// 	virtual ~CControlCheckButton();
// 
// 	//��������
// public:
// 	//�ؼ�����
// 	CRect GetControlRect();
// 
// 	//��������
// public:
// 	//��׼λ��
// 	inline VOID  SetControlBenchmark(INT nXPos,INT nYPos) { m_ptControlBenchmark.SetPoint(nXPos,nYPos); };
// 	//��ȡ״̬
// 	inline BYTE  GetButtonChecked() { return m_cbChecked; };
// 	//����״̬
// 	inline VOID  SetButtonChecked(BYTE cbChecked);
// 	//���ø���
// 	inline VOID  SetParentWndSink(CWnd * pParentSink) { m_pParentSink=pParentSink; };
// 
// 	//�¼�����
// public:
// 	//���ƿؼ�
// 	VOID  OnDrawControl(CDC * pDC);
// 	//����¼�
// 	VOID  OnClickControl(CPoint Point);
// 
// };

class CHuiYuanHyxxDlg : public CDialog//,public CMissionItem
{
	DECLARE_DYNAMIC(CHuiYuanHyxxDlg)

public:
	CHuiYuanHyxxDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuiYuanHyxxDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HUIYUAN_HYXX };
	void ResetContent();
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

	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnNextPage();

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

	afx_msg void OnHyxxChkxj();
	afx_msg void OnHyxxXgfd();
	afx_msg void OnHyxxSwdl();
	afx_msg void OnHyxxZhuanZ(); //ת��
	afx_msg void OnHyxxFenPeipe(); //ת��
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnNMRclickListLog(NMHDR *pNMHDR, LRESULT *pResult);
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

	CControlCheckButton m_CheckOnlineBtn;
	CControlCheckButton m_CheckUserYueBtn;
	Bitmap* m_bmpBk;
	DWORD m_dwTickCount;
	CDateTimeCtrl m_dateStart;
	CDateTimeCtrl m_dateEnd;
	CListCtrlCl m_listLog;
	CImageButton m_btnOK;
	CImageButton m_btnPrePage;
	CImageButton m_btnNextPage;
	CSkinComboBox m_cmbSort;
	CFont m_font;
	CTime m_tStart;
	CTime m_tEnd;
	CFont m_cmbfont;
	int m_nIndex;
	int m_nTargetUserID;
	DOUBLE m_fZhuanZhangJine;
	TCHAR		m_szPassWord[126];
	int		m_nZhuanZType;
	CString m_strName;
	int m_page;
	int m_itemCount;
	int m_maxPage;
	BOOL m_byTime;
	CString m_strPhoneNum;

	DOUBLE m_fXGFandian;
	int m_user_id;
	int m_byType;
	CSkinEditEx m_editAct;
	CSkinEditEx m_editID;
	CString m_strAct;
	CString m_strID;

	CTextButton	m_btnChkxj[10];
	int	m_nSetPeie1;
	int	m_nSetPeie2;
	int	m_nSetPeie3;
	int	m_nSetPeie4;
	int	m_nSetPeie5;

	BYTE m_cbIfXj;
	//���غ���
public:
// 	//�����¼�
// 	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
// 	//�ر��¼�
// 	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetHyxxLogCount;						//��ȡ��־����
	bool							m_bGetHYXXLog;							//��ȡ��־
	bool							m_bGetHYXXByIDLog;						//��ȡ��־ͨ��ID
	bool							m_bGetHYXXByActLog;						//��ȡ��־ͨ��Act
	bool							m_bXgHyFandian;							//�޸Ļ�Ա����
	bool							m_bSetUserAgent;						//���ô���
	int								m_nItem;				
protected:
	VOID SendToServer(int nSendType);

};
