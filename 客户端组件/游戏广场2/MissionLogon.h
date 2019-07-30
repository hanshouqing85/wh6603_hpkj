#ifndef MISSION_LOGON_HEAD_FILE
#define MISSION_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgLogon.h"
#include "DlgStatus.h"

//////////////////////////////////////////////////////////////////////////////////

//��¼����
class CMissionLogon : public CDlgStatus, public CMissionItem, public IStatusViewSink
{
	//��Ԫ����
	friend class CDlgLogon;
	friend class CDlgRegister;

	//��������
protected:
	int								m_nAutoLogon;						//�Զ���¼����
	bool							m_bRegister;						//ע���־
	bool							m_bRemPassword;						//��ס����
	bool							m_bGetCPUserInfo;
	bool							m_bKefu;								//�ͷ�
	//�ؼ�ָ��
protected:
	CDlgLogon *						m_pDlgLogon;						//��¼����
	//CDlgRegister *					m_pDlgRegister;						//ע�ᴰ��

	//��̬����
protected:
	static CMissionLogon *			m_pMissionLogon;					//����ָ��

	BYTE										m_cbIsYanzheng;
	CString									m_strPhoneNum;
	//��������
public:
	//���캯��
	CMissionLogon();
	//��������
	virtual ~CMissionLogon();
	bool							m_bKillSocket;

	//״̬�ӿ�
public:
	//ȡ������
	virtual VOID OnStatusCancel();

	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
public:
	//��ʾ��¼
	VOID ShowLogon();
	bool PerformSendYanZhengma(CString strYanzhengma);
	//��������
protected:
	//�����û�
	VOID UpdateUserInfo();
	//ִ�е�¼
	bool PerformLogonMission(bool bRemPassword);
	bool CreatePlazaview();
	bool PerformKefu();
	//�����¼�
protected:
	//��¼�ɹ�
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//��¼���
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//������ʾ
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);
	//��֤��
	bool OnSocketSubCheckYzmRet(VOID * pData, WORD wDataSize);

	//��̬����
public:
	//��ȡ����
	static CMissionLogon * GetInstance() { return m_pMissionLogon; }
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnMessageSendRandNum(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif