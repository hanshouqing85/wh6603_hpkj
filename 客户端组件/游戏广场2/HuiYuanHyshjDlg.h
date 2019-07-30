#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageButton.h"
#include "MessageDlg.h"

#include "ImgRioButton.h"

// CHuiYuanHyshjDlg �Ի���

class CHuiYuanHyshjDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuiYuanHyshjDlg)

public:
	CHuiYuanHyshjDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuiYuanHyshjDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HUIYUAN_HYSHJ };
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnChkToday();
	afx_msg void OnBnClickedBtnOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	Bitmap* m_bmpBk;
	CFont m_font;
	CImageButton m_btnOK;
	bool	m_bChkToday;
	CString m_xiajirenshu;
	CString m_zaixianrenshu;
	CString m_huiyuanyue;
	CString m_jinrifandian;
	CString m_jinrichongzhi;
	CString m_jinrichongzhisxf;
	CString m_jinriqukuan;
	CString m_jinritouzhu;
	CString m_jinrihuodong;
	CString m_jinrixinchongzhi;
	CString m_zongheyingkui;
	CString m_xinchongzhi;
	CString m_zongheyongjin;
	CString m_jinrizhuce;
	CString m_chedanzonge;
	CString m_zhongjiangzonge;
	CString m_wodefandian;
	CString m_caipiaoyingkui;
	CString m_xianmaliang;
	CString m_qipaifandian;
	CString m_qipaiyingkui;
	CString m_myqipaifandian;
	CString m_touzhuyongjin;
	CString m_kuisunyongjin;
	CDateTimeCtrl m_dateStart;
	CDateTimeCtrl m_dateEnd;
	CTime m_tStart;
	CTime m_tEnd;

	CImgRioButton m_rioChkToday;
	void AdjustCtrls();
	//���غ���
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetHuiyuanShj;						//��ȡ��־����
protected:
	VOID SendToServer(int nSendType);

};
