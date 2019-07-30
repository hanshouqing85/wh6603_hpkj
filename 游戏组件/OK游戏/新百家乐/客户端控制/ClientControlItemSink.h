#pragma once
#include "Resource.h"
#include "../��Ϸ�ͻ���/ClientControl.h"


// CClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	BYTE m_cbWinArea;				//Ӯ������
	BYTE m_cbExcuteTimes;			//ִ�д���

public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientControlItemSinkDlg();

// �Ի�������
	enum { IDD = IDD_CLIENT_CONTROL_EX };
	LONGLONG  m_lAllUserBet[8];				//�û���ע
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//������ע
	virtual void ResetUserBet();
	//�����ע
	virtual void SetUserBetScore(BYTE cbArea,LONGLONG lScore);
	//������
	virtual bool ReqResult(const void * pBuffer);

	virtual BOOL OnInitDialog();
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();

protected:
	virtual void OnCancel();

public:
	afx_msg void OnTimer(UINT nIDEvent);
};
