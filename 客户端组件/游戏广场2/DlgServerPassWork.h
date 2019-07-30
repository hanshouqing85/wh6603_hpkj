#ifndef DLG_SERVER_PASSWORK_HEAD_FILE
#define DLG_SERVER_PASSWORK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

////////////////////////////////////////////////////////////////////////////////

class CDlgServerPassWork : public CDialog
{


public:
	CDlgServerPassWork(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgServerPassWork();

// �Ի�������
	enum { IDD = IDD_DLG_SERVER_PASSWORK };

private:
	CString							m_StrPassWork;						//��������
		//�ؼ�����
protected:
	CSkinButton						m_btOk;								//ȷ�ϲ���
	CSkinButton						m_btCancel;							//ȡ������
	CSkinEditEx						m_ServerPassWork;					//��¼�ʺ�
	//������Դ
protected:
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//���ú���
	virtual BOOL OnInitDialog();
	//�û��Զ��庯��
public:
	void SetPassWork(CString StrPassWork);
	//��Ϣ����
public:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//��ť��Ϣ
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()
};




////////////////////////////////////////////////////////////////////////////////



#endif