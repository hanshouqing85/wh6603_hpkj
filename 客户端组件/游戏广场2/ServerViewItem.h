#ifndef SERVER_VIEW_ITEM_HEAD_FILE
#define SERVER_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgStatus.h"
#include "ServerListData.h"
#include "ProcessManager.h"
#include "TableViewFrame.h"
#include "DlgServerManager.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//����״̬
enum enServiceStatus
{
	ServiceStatus_Unknow,			//δ֪״̬
	ServiceStatus_Entering,			//����״̬
	ServiceStatus_Validate,			//��֤״̬
	ServiceStatus_RecvInfo,			//��ȡ״̬
	ServiceStatus_ServiceIng,		//����״̬
	ServiceStatus_NetworkDown,		//�ж�״̬
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CServerViewItem : public CDialog, public IServerViewItem, public ITCPSocketSink,
	public IUserManagerSink, public IProcessManagerSink, public IStatusViewSink,
	public ITableViewFrameSink
{
	//�������
protected:
	bool							m_bCreateFlag;						//������־

	//�û�����
protected:
	DWORD							m_dwUserRight;						//�û�Ȩ��
	DWORD							m_dwMasterRight;					//����Ȩ��
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��

	//��������
protected:
	BOOL                            m_bAllowDistribute;                 //�������
	WORD							m_wServerType;						//��������
	DWORD							m_dwServerRule;						//�������
	enServiceStatus					m_ServiceStatus;					//����״̬

	//��������
protected:
	tagGameKind						m_GameKind;							//������Ϣ
	tagGameServer					m_GameServer;						//������Ϣ

	//������Ϣ
protected:
	BYTE							m_cbColumnCount;					//�б���Ŀ
	tagColumnItem					m_ColumnItem[MAX_COLUMN];			//�б�����

	//�������
protected:
	CDlgStatus						m_DlgStatus;						//״̬����
	CProcessManager					m_ProcessManager;					//���̹���
	CTableViewFrame					m_TableViewFrame;					//���ӿ��

	//�ؼ�����
protected:
	CDlgInsureServer *				m_pDlgInsureServer;					//���ж���
	CDlgServerManager *				m_pDlgServerManager;				//�������
	CPlatformPublicize				m_PlatformPublicize;				//����ؼ�

	//�������
protected:
	CTCPSocketHelper				m_TCPSocketModule;					//��������
	CGameLevelParserHelper			m_GameLevelParserModule;			//��Ϸ�ȼ�
	CPlazaUserManagerHelper			m_PlazaUserManagerModule;			//�û�����


	//��������
public:
	//���캯��
	CServerViewItem();
	//��������
	virtual ~CServerViewItem();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ӽӿ�
public:
	//���Ӱ�ť
	virtual VOID OnHitTableButton(BYTE cbButton);
	//���˫��
	virtual VOID OnDButtonHitTable(WORD wTableID);
	//�������
	virtual VOID OnLButtonHitTable(WORD wTableID, WORD wChairID);
	//�Ҽ�����
	virtual VOID OnRButtonHitTable(WORD wTableID, WORD wChairID);

	//����ӿ�
protected:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//���̽ӿ�
protected:
	//���̴���
	virtual bool OnGameProcessError();
	//���̹ر�
	virtual bool OnGameProcessClose(DWORD dwExitCode);
	//��������
	virtual bool OnGameProcessCreate(bool bAlreadyExist);
	//��������
	virtual bool OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�û��ӿ�
public:
	//�û�����
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem);
	//�û�ɾ��
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem);
	//�û�����
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserSpreadInfo & LastSpreadInfo);

	//�¼��ӿ�
protected:
	//ȡ������
	virtual VOID OnStatusCancel();

	//��Ϣ����
public:
	//�����ʶ
	virtual WORD GetServerID() { return m_GameServer.wServerID; }
	//��������
	virtual LPCTSTR GetServerName() { return m_GameServer.szServerName; }
	//����״̬
	virtual enServiceStatus GetServiceStatus() { return m_ServiceStatus; }

	//�������
public:
	//�������
	ITCPSocket * GetTCPSocket() { return m_TCPSocketModule.GetInterface(); }
	//�û����
	IPlazaUserManager * GetPlazaUserManager() { return m_PlazaUserManagerModule.GetInterface(); }

	//���ܺ���
public:
	//��ʾ����
	VOID ShowInsureView();

	//�����¼�
protected:
	//�û�����
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��¼����
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//���ô���
	bool OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//���д���
	bool OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������
	bool OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//ϵͳ����
	bool OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//�û�����
	bool OnSocketSubUserEnter(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSocketSubUserScore(VOID * pData, WORD wDataSize);
	//�û�״̬
	bool OnSocketSubUserStatus(VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnSocketSubUserSpreadInfo(VOID * pData, WORD wDataSize);
	//��¼���
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//��¼�ɹ�
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//������ʾ
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnSocketSubRequestFailure(VOID * pData, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnSocketSubActionMessage(VOID * pData, WORD wDataSize);

	//���溯��
public:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//���÷���
	VOID InitServerViewItem(CGameServerItem * pGameServerItem);
	//��ԴĿ¼
	VOID GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount);

	//��������
public:
	BOOL GetHostIP(TCHAR * inData) ;
	//���͵�¼
	bool SendLogonPacket();
	//�����Թ�
	bool SendLookonPacket(WORD wTableID, WORD wChairID);
	//��������
	bool SendSitDownPacket(WORD wTableID, WORD wChairID);
	//��������
	bool SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave);

	//��������
public:
	//ִ���Թ�
	bool PerformLookonAction(WORD wTableID, WORD wChairID);
	//ִ������
	bool PerformStandUpAction(WORD wTableID, WORD wChairID);
	//ִ������
	bool PerformSitDownAction(WORD wTableID, WORD wChairID);

	//���ܺ���
public:
	//Ȩ�޼��
	bool CheckUserRight();

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnNcDestroy();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif