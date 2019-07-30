#pragma once
#include "Resource.h"

// CAdminControlDlg �Ի���
#define IDM_ADMIN_COMMDN WM_USER+1000
#define IDM_ADMIN_QUERYUSER WM_USER+1001

class CAdminControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminControlDlg)

	//����ؼ�
protected:
	DWORD		 m_dwGameID;	

	DWORD		m_wUserGameID[GAME_PLAYER];
	TCHAR		m_szUserAccounts[GAME_PLAYER][LEN_ACCOUNTS];	//�������
public:
	CAdminControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdminControlDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_ADMIN };

public:
	//������
	bool ReqResult(const void * pBuffer);
	bool OnSubAdminCommand(const void * pBuffer, WORD wDataSize);
	void SetUserJetton(LONGLONG lJetton,BYTE cbArea);
	void EmptyUserJetton();

	void SetUserInfo(TCHAR m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS],DWORD m_wGameID[GAME_PLAYER]);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	virtual BOOL OnInitDialog();
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();
	afx_msg void  OnQueryUser();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};
