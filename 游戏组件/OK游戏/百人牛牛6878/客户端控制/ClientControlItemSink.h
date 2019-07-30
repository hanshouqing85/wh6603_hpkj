#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"

#define CONTROL_AREA		4
// CClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	bool m_bWinArea[CONTROL_AREA];
	BYTE m_cbControlStyle;
	BYTE m_cbExcuteTimes;
protected:		
	LONGLONG  m_lAllUserBet[AREA_COUNT];				//�û���ע
public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientControlItemSinkDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//���¿���
	virtual void __cdecl OnAllowControl(bool bEnable);
	//������
	virtual bool __cdecl ReqResult(const void * pBuffer);
	//������ע
	virtual void __cdecl ResetUserBet();
	//�����ע
	virtual void __cdecl SetUserBetScore(BYTE cbArea,LONGLONG lScore);
	//�����ע
	virtual void __cdecl SetUserGameScore(BYTE cbArea,LONGLONG lScore);

	//���1
	virtual void __cdecl SetUserNickName(CString strNickName);
	virtual void __cdecl ResetUserNickName();

public:
	//���ý���
	void ReSetAdminWnd();

public:
	//��ʾ״̬
	void PrintCurStatus();

public:
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();
	afx_msg void  OnRadioClick();
	afx_msg void OnBnClickedBtGet();
	afx_msg void OnBnClickedBtCheak();
protected:
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
};
//
