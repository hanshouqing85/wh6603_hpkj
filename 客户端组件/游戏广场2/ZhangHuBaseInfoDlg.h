#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "MessageDlg.h"
#include "ImgRioButton.h"
#include "DlgLogon.h"
#define  RELEASE_FACE  WM_USER+11123
//�ҵ��˻�-������Ϣ
class CZhangHuBaseInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuBaseInfoDlg)

public:
	CZhangHuBaseInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZhangHuBaseInfoDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ZHANGHU_BASEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

	CControlCheckButton m_ImgRioMan;
	CControlCheckButton m_ImgRioWoman;
public:
	WDZHUserInfo m_userinfo;	
	DWORD m_dwTickCount;
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}
private:
	Bitmap* m_bmpBk;
	CFont m_font;
	BYTE m_cbGender;
	CSkinEditEx m_editQQ;
	CImageButton m_btnXgqq;
	afx_msg void OnBnClickedBtnXgqq();
	afx_msg void OnBnClickedBtnXgGender();
	afx_msg void OnBnClickedRioBtnMan();
	afx_msg void OnBnClickedRioBtnWoman();
	CImageButton m_btnXggender;

	//���غ���
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetUserInfo;						//��ȡ��־����
	bool							m_bXgUserQQ;							//��ȡ��־
protected:
	VOID SendToServer(int nSendType);

};
