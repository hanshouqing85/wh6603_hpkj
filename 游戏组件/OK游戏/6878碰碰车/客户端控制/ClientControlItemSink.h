#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"


// CClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientControlItemSinkDlg();

// �Ի�������
	enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:		
	SCORE  m_lAllUserBet[AREA_COUNT];				//�û���ע

public:
	//���ý���
	virtual void __cdecl ResetUserBet();
	//����ǳ�
	virtual void __cdecl ResetUserNickName();
	//��ȡ�ǳ�
	virtual void __cdecl GetCheckNickName(CString &strNickName);
	//����ǳ�
	virtual void __cdecl SetUserNickName(LPCTSTR lpszNickName);
	//��ע��Ϣ
	virtual void __cdecl SetUserBetScore(BYTE cbArea,SCORE lScore);
	//��ע��Ϣ
	virtual void __cdecl SetAllUserBetScore(BYTE cbArea,SCORE lScore);
	//���¿���
	virtual void __cdecl UpdateControl(CMD_S_ControlReturns* pControlReturns);

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
	//���ֿ���
	afx_msg void OnBnClickedButtonSyn();
	//��������
	afx_msg void OnBnClickedButtonOk();
	//ȡ���ر�
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedBtGet();
	afx_msg void OnBnClickedBtCheak();


};
