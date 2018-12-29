#pragma once
#include "Stdafx.h"

class CAdminClientDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminClientDlg)

protected:		
	LONGLONG				m_lAllUserBet[JETTON_AREA_COUNT];				//�û���ע

public:
	CAdminClientDlg();   // ��׼���캯��
	virtual ~CAdminClientDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	//���¿���
	virtual void __cdecl UpdateControl(CMD_S_ControlReturns* pControlReturns);
	//������ע
	void ResetUserBet();
	//�����ע
	void SetUserBetScore(BYTE cbArea,LONGLONG lScore);

protected:
	//��Ϣ����
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);

public:
	//��ʼ��
	virtual BOOL OnInitDialog();
	//������ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//ȡ������
	afx_msg void OnBnClickedButtonReset();
	//������Ϣ
	afx_msg void OnBnClickedButtonSyn();
	//��������
	afx_msg void OnBnClickedButtonOk();
	//ȡ���ر�
	afx_msg void OnBnClickedButtonCancel();

	DECLARE_MESSAGE_MAP()
};
