#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PlatformPublicize.h"
#include "Resource.h"
#include "DlgStatus.h"
#include "AccountsControl.h"
#define IDC_FORGET_PASS				120
#define IDC_KEFU					121
#define IDC_RUBBISH					122

//�˺���Ϣ
struct tagAccountsInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szGameID[16];						//��Ϸ��ʶ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�˺�
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//���鶨��
typedef CWHArray<tagAccountsInfo *>	CAccountsInfoArray;					//�˺�����

//////////////////////////////////////////////////////////////////////////////////

//��ѡ�ؼ�
class  CControlCheckButton
{
	//״̬����
protected:
	BYTE                           m_cbChecked;                        //��ס����
	CWnd *                         m_pParentSink;                      //����ָ��

	//λ�ñ���
protected:
	CPoint                         m_ptControlBenchmark;               //��׼λ��

	//��Դ����
protected:
	CPngImage                      m_ImageBtnBack;                     //������Դ

	//��������
public:
	//���캯��
	CControlCheckButton();
	//��������
	virtual ~CControlCheckButton();

	//��������
public:
	//�ؼ�����
	CRect GetControlRect();

	//��������
public:
	//��׼λ��
	inline VOID  SetControlBenchmark(INT nXPos,INT nYPos) { m_ptControlBenchmark.SetPoint(nXPos,nYPos); };
	//��ȡ״̬
	inline BYTE  GetButtonChecked() { return m_cbChecked; };
	//����״̬
	inline VOID  SetButtonChecked(BYTE cbChecked);
	//���ø���
	inline VOID  SetParentWndSink(CWnd * pParentSink) { m_pParentSink=pParentSink; };
	//���ø���
	VOID  LoadCheckImage(UINT nIDResource) ;
	//���ø���
	VOID  LoadCheckImage(LPCTSTR pszResourceName) ;

	//�¼�����
public:
	//���ƿؼ�
	VOID  OnDrawControl(CDC * pDC);
	//����¼�
	VOID  OnClickControl(CPoint Point);

};

//////////////////////////////////////////////////////////////////////////////////

//��¼����
class CDlgLogon : public CDialog, public IStatusViewSink
{
	//���ñ���
public:
	BYTE							m_cbRemPassword;					//��ס����
	static int									m_nLoadServer;
	//��¼��Ϣ
public:
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//��Ϸ�ʺ�
	TCHAR							m_szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							m_szLogonServer[LEN_SERVER];		//��¼��ַ
	CPlatformPublicize			m_logo;			//LOGO IE�ؼ�

	//�ؼ�����
protected:
	CSkinButton						m_btQuit;							//�˳���ť
	CSkinButton						m_btEnter;							//��¼��ť
	CSkinButton						m_btForgetPass;						//��������
	CSkinButton						m_btKeFu;						//��������

	CSkinButton						m_btDeleteRecord;					//ɾ����ť

	static	UINT	GetBestUrl(LPVOID lpParam);

	//����ؼ�
protected:
	CSkinEditEx						m_edAccounts;						//��¼�ʺ�
	CAccountsCtrl					m_ServerControl;					//��¼��ַ
	CPasswordControl				m_PasswordControl;					//�û�����
	CControlCheckButton             m_RemPwdControl;                    //��ס����
	CAccountsCtrl					m_AccountsControl;					//��¼�ʺ�

	CFont							m_Font;
	//����ؼ�
protected:
	CBrush							m_brBrush;							//������ˢ
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��
	//CWndLogonButton					m_WndLogonButton;					//��ť�ؼ�
//	CPlatformPublicize				m_PlatformPublicize;				//����ؼ�
	CDlgStatus						m_DlgStatus;	

	bool							m_bCreatePlazaview;
	//��������
protected:
	CAccountsInfoArray				m_AccountsInfoArray;				//�˺���Ϣ
	CString					m_strCurrentDir;		//��ǰĿ¼
	CString						m_strSiteUrl;
	bool							m_Destroyed;
	//ȡ������
	virtual VOID OnStatusCancel();
//״̬����
	//��������
public:
	//���캯��
	CDlgLogon();
	//��������
	virtual ~CDlgLogon();


	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();
	//�ػ���Ϣ
	VOID OnPaint();
	//ѡ��ı�
	VOID OnSelchangeAccounts();
	VOID OnSelchangeServer();
	void Loadweb();
	//���ܺ���
public:
	//��������
	WORD ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize);
	//��ַ��Ϣ
	VOID LoadLogonServerInfo();
	BOOL CheckVersionUpdate(CString& strUpdateURL);

	//��������
private:
	//��ȡ��Ϣ
	bool GetInformation();
	//�����˺�
	VOID LoadAccountsInfo();
	//��ť��Ϣ
protected:
	//ע���ʺ�
	VOID OnBnClickedRegister();
	//�༭�ı�
	VOID OnCbnEditchangeAccounts();
	//�һ�����
	VOID OnBnClickedForget();
	//�ͷ�
	VOID OnBnClickedKeFu();
	//ɾ����¼
	VOID OnBnClickedDelete();
	void SetCurServer(int nLoad);
	//ϵͳ��Ϣ
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);

	//�ؼ���ɫ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	//��Ϣӳ��
public:
	afx_msg void OnEnChangeAccounts();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////////////

#endif