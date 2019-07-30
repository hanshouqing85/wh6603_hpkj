#include "StdAfx.h"
#include "GamePlaza.h"
#include "ServerViewItem.h"

//���ݿؼ�
#include "GlobalUnits.h"
#include "PlatformEvent.h"

//����ؼ�
#include "PlatformFrame.h"
#include "DlgServerManager.h"
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_SERVER_TABLE			100									//���ӿؼ�
#define IDC_WEB_PUBLICIZE           101                                 //����ؼ�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerViewItem, CDialog)
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerViewItem::CServerViewItem()
{
	//�������
	m_bCreateFlag=false;

	//�û�����
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//��������
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_ServiceStatus=ServiceStatus_Unknow;

	//�ؼ�����
	m_pDlgInsureServer=NULL;
	m_pDlgServerManager=NULL;

	//�������
	m_DlgStatus.SetStatusViewSink(this);

	//������Ϣ
	m_cbColumnCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));

	//��������
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	return;
}

//��������
CServerViewItem::~CServerViewItem()
{
}

//�ӿڲ�ѯ
VOID * CServerViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableViewFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerViewItem,Guid,dwQueryVer);
	return NULL;
}

//��������
BOOL CServerViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	m_bCreateFlag=true;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_TableViewFrame.CreateTableFrame(this,IDC_SERVER_TABLE,QUERY_ME_INTERFACE(IUnknownEx));

	m_TableViewFrame.ShowWindow(SW_HIDE);
	//�������
	if (m_GameLevelParserModule.CreateInstance()==false) throw TEXT("�ȼ������������ʧ�ܣ�");
	if (m_PlazaUserManagerModule.CreateInstance()==false) throw TEXT("�û������������ʧ�ܣ�");

	//�������
	m_ProcessManager.SetProcessManagerSink(QUERY_ME_INTERFACE(IUnknownEx));

	//�������
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	if (m_PlazaUserManagerModule->SetUserManagerSink(pIUnknownEx)==false) throw TEXT("�û������������ʧ�ܣ�");
	if (m_PlazaUserManagerModule->SetUserInformation(pUserInformation)==false) throw TEXT("�û������������ʧ�ܣ�");

	return TRUE;
}

//��Ϣ����
BOOL CServerViewItem::PreTranslateMessage(MSG * pMsg)
{
	//������Ϣ
	if (pMsg->message==WM_KEYDOWN)
	{
		//ȡ����ť
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}

		//�س���ť
		if (pMsg->wParam==VK_RETURN)
		{
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CServerViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//��ť����
	switch (nCommandID)
	{
	case IDC_BT_QUIT_SERVER:		//�˳�����
		{
			//��������
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);

			return TRUE;
		}
	}

	//�˵�����
	switch (nCommandID)
	{
	case IDM_DELETE_SERVER_ITEM:	//�رշ���
		{
			//�ж�����
			if (m_TCPSocketModule.GetInterface()!=NULL)
			{
				m_TCPSocketModule->CloseSocket();
				m_ServiceStatus=ServiceStatus_NetworkDown;
			}

			//�رշ���
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->DeleteServerViewItem();

			return TRUE;
		}
	case IDM_MANAGER_SERVER:		//�������
		{
			//��������
			CDlgServerManager DlgServerManager;
			DlgServerManager.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

			//���ñ���
			m_pDlgServerManager=&DlgServerManager;

			//��ʾ����
			DlgServerManager.DoModal();

			//���ñ���
			m_pDlgServerManager=NULL;

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//���Ӱ�ť
VOID CServerViewItem::OnHitTableButton(BYTE cbButton)
{
	//��ť����
	switch (cbButton)
	{
	case BT_ENTER_GAME:		//������Ϸ
		{
			//�Զ�����
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				//Ȩ���ж�
				if(CheckUserRight()==false) break;
				
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				if(pGlobalUserData->lScore < m_GameServer.lMinServerScore)
				{
					CInformation Information(this);
					Information.ShowMessageBox(L"������Ϸ�Ҳ����������������ֵ");
					//�رշ���
					PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

					break;
				}
				//��������
				bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

				//������
				if (bSuccess==false)
				{
					//�ж�����
					m_TCPSocketModule->CloseSocket();
					m_ServiceStatus=ServiceStatus_NetworkDown;

					return;
				}

				//���ͷ���
				PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR);
			}

			break;
		}
	case BT_CLOSE_SERVER:	//�رշ���
		{
			//�˳��ж�
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetUserStatus()==US_PLAYING))
			{
				//��ʾ��Ϣ
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0)!=IDYES)
				{
					return;
				}
			}

			//�رշ���
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

			break;
		}
	}

	return;
}

//���˫��
VOID CServerViewItem::OnDButtonHitTable(WORD wTableID)
{
	return;
}

//�������
VOID CServerViewItem::OnLButtonHitTable(WORD wTableID, WORD wChairID)
{
	return;
}

//�Ҽ�����
VOID CServerViewItem::OnRButtonHitTable(WORD wTableID, WORD wChairID)
{
	return;
}

//�����¼�
bool CServerViewItem::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//������ʾ
	if (nErrorCode!=0L)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("�ܱ�Ǹ����Ϸ��������ʧ�ܣ����Ժ����Ի�������վ���棡"),MB_ICONSTOP);

		return true;
	}

	//���͵�¼
	SendLogonPacket();

	//����״̬
	m_ServiceStatus=ServiceStatus_Validate;

	return true;
}

//�ر��¼�
bool CServerViewItem::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//�ر�״̬
	m_DlgStatus.HideStatusWindow();

	//�ر���Ϸ
	m_ProcessManager.CloseGameProcess(false);

	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if (m_ServiceStatus==ServiceStatus_ServiceIng)
		{
			//�ر���ʾ
			CInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(m_GameServer.szServerName,TEXT("����Ϸ�������������Ѿ��жϣ��Ƿ������Ϸ��"),
				MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2,0L);

			//�رշ���
			if (nResult==IDYES)
			{
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM);
			}
		}
		else
		{
			//�ر���ʾ
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("���������ԭ�򣬷�������ʧ�ܣ����Ժ����³��ԣ�"),MB_ICONERROR);

			//�رշ���
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
		}
	}

	//����״̬
	m_ServiceStatus=ServiceStatus_NetworkDown;

	return true;
}

//��ȡ�¼�
bool CServerViewItem::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:		//��¼��Ϣ
		{
			return OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_CONFIG:		//������Ϣ
		{
			return OnSocketMainConfig(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_USER:		//�û���Ϣ
		{
			return OnSocketMainUser(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_INSURE:		//������Ϣ
		{
			return OnSocketMainInsure(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_MANAGE:		//������Ϣ
		{
			return OnSocketMainManager(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_CM_SYSTEM:		//ϵͳ��Ϣ
		{
			return OnSocketMainSystem(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GF_GAME:		//��Ϸ��Ϣ
	case MDM_GF_FRAME:		//�����Ϣ
		{
			return OnSocketMainGameFrame(Command.wMainCmdID,Command.wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//���̴���
bool CServerViewItem::OnGameProcessError()
{
	return true;
}

//���̹ر�
bool CServerViewItem::OnGameProcessClose(DWORD dwExitCode)
{
	//Ч��״̬
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//��������
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();
	BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

	//�˳���Ϸ
	if ((wTableID!=INVALID_TABLE)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	}

	return true;
}

//��������
bool CServerViewItem::OnGameProcessCreate(bool bAlreadyExist)
{
	//��������
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();

	//��������
	if (bAlreadyExist==false)
	{
		//��ȡ�ȼ�
		tagLevelItem LevelItem[64];
		BYTE cbLevelCount=(BYTE)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

		//��������
		m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
		m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
	}

	//������Ϣ
	m_ProcessManager.SendServerInfo(m_pIMySelfUserItem,m_dwUserRight,m_dwMasterRight,m_dwServerRule,m_wServerType,m_bAllowDistribute,m_GameServer);

	//�����û�
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//��������
		WORD wEnumIndex=0;
		IPlazaUserManager * pIPlazaUserManager=m_PlazaUserManagerModule.GetInterface();

		//�������
		while (true)
		{
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//�����û�
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};

		//�Թ��û�
		wEnumIndex=0;
		while (true)
		{
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//�����û�
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};
	}

	//�������
	m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_CONFIG_FINISH);

	return true;
}

//��������
bool CServerViewItem::OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//������Ϣ
	if ((wMainCmdID==IPC_CMD_GF_SOCKET)&&(wSubCmdID==IPC_SUB_GF_SOCKET_SEND))
	{
		//Ч������
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize<sizeof(TCP_Command)) return false;

		//��������
		IPC_GF_SocketSend * pSocketPackage=(IPC_GF_SocketSend *)pData;
		WORD wPacketSize=wDataSize-(sizeof(IPC_GF_SocketSend)-sizeof(pSocketPackage->cbBuffer));

		//��������
		if (wPacketSize==0)
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID);
		}
		else 
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wPacketSize);
		}

		return true;
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�û�����
VOID CServerViewItem::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//�ж��Լ�
	if (m_pIMySelfUserItem==NULL)
	{
		m_pIMySelfUserItem=pIClientUserItem;
	}

	//��������
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		WORD wTableID=pIClientUserItem->GetTableID();
		WORD wChairID=pIClientUserItem->GetChairID();
		m_TableViewFrame.SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	return;
}

//�û�ɾ��
VOID CServerViewItem::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//��������
	WORD wLastTableID=pIClientUserItem->GetTableID();
	WORD wLastChairID=pIClientUserItem->GetChairID();
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//��������
	ASSERT(CParameterGlobal::GetInstance());
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�뿪����
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//��ȡ�û�
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_TableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,NULL);

		//�뿪֪ͨ
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.cbUserStatus=US_NULL;
			UserStatus.wTableID=INVALID_TABLE;
			UserStatus.wChairID=INVALID_CHAIR;
			m_ProcessManager.SendUserStatus(pIClientUserItem->GetUserID(),UserStatus);
		}
	}

	return;
}

//�û�����
VOID CServerViewItem::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	return;
}

//�û�����
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//����֪ͨ
	if ((m_wServerType&GAME_GENRE_GOLD)&&(pIClientUserItem==m_pIMySelfUserItem))
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		pGlobalUserData->lScore=pIClientUserItem->GetUserScore();
		pGlobalUserData->lInsure=pIClientUserItem->GetUserInsure();

		//��������
		if(m_pDlgInsureServer!=NULL) 
			m_pDlgInsureServer->OnEventUpdateInsureServer(pGlobalUserData->lScore,pGlobalUserData->lInsure);

		//�¼�֪ͨ
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//��Ϸ֪ͨ
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//��������
		tagUserScore UserScore;
		ZeroMemory(&UserScore,sizeof(UserScore));

		//���ñ���
		UserScore.lScore=pUserInfo->lScore;
		UserScore.dwWinCount=pUserInfo->dwWinCount;
		UserScore.dwLostCount=pUserInfo->dwLostCount;
		UserScore.dwDrawCount=pUserInfo->dwDrawCount;
		UserScore.dwFleeCount=pUserInfo->dwFleeCount;
		UserScore.dwExperience=pUserInfo->dwExperience;

		//��������
		m_ProcessManager.SendUserScore(pUserInfo->dwUserID,UserScore);
	}

	return;
}

//�û�����
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserSpreadInfo & LastSpreadInfo)
{
	if(pIClientUserItem == NULL)
		return;
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	if(pUserInfo == NULL || pMeUserInfo == NULL)
		return ;
	//��Ϸ֪ͨ
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//��������
		tagUserSpreadInfo UserSpreadInfo;
		ZeroMemory(&UserSpreadInfo,sizeof(UserSpreadInfo));

		//���ñ���
		CopyMemory(UserSpreadInfo.szIpAddrDescribe,pIClientUserItem->GetAddrDescribe(),sizeof(UserSpreadInfo.szIpAddrDescribe));

		//��������
		m_ProcessManager.SendUserSpreadInfo(pUserInfo->dwUserID,UserSpreadInfo);
	}

	return;
}

//�û�����
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	WORD wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	WORD wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	BYTE cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;

	//�����뿪
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_TableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,NULL);
	}

	//���Ӽ���
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		m_TableViewFrame.SetClientUserItem(wNowTableID,wNowChairID,pIClientUserItem);
	}

	//�뿪֪ͨ
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��Ϸ֪ͨ
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.wTableID=wNowTableID;
			UserStatus.wChairID=wNowChairID;
			UserStatus.cbUserStatus=cbNowStatus;
			m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);
		}

		return;
	}

	//���봦��
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��Ϸ֪ͨ
		if ((m_pIMySelfUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_ProcessManager.SendUserItem(pIClientUserItem);
		}

		//��������
		if (m_pIMySelfUserItem==pIClientUserItem)
		{
			m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);
		}

		return;
	}

	//״̬�ı�
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//��Ϸ֪ͨ
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;
		m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);

		return;
	}

	return;
}

//ȡ������
VOID CServerViewItem::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//��ʾ����
VOID CServerViewItem::ShowInsureView()
{
	//Ч�����
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//��������
	if((m_wServerType&GAME_GENRE_GOLD)==0)
	{
		//��������
		CInformation Information;
		Information.ShowMessageBox(TEXT("��Ǹ,����ǰ���ڵķ��䲻�������в���,�����˳�����!"));
        
		return;
	}

	//��������
	CDlgInsureServer DlgInsureServer;
	DlgInsureServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);
	
	//���ñ���
	m_pDlgInsureServer=&DlgInsureServer;

	//��ʾ����
	DlgInsureServer.DoModal();

	//���ñ���
	m_pDlgInsureServer=NULL;

	return;
}

//�û�����
bool CServerViewItem::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER:			//�û�����
		{
			return OnSocketSubUserEnter(pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//�û�����
		{
			return OnSocketSubUserScore(pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//�û�״̬
		{
			return OnSocketSubUserStatus(pData,wDataSize);
		}
	case SUB_GR_USER_SPREADINFO:    //�û���Ϣ
		{
			return OnSocketSubUserSpreadInfo(pData,wDataSize);
		}
	case SUB_GR_REQUEST_FAILURE:	//����ʧ��
		{
			return OnSocketSubRequestFailure(pData,wDataSize);
		}
	}

	return true;
}

//��¼����
bool CServerViewItem::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:	//��¼�ɹ�
		{
			return OnSocketSubLogonSuccess(pData,wDataSize);
		}
	case SUB_GR_LOGON_FAILURE:	//��¼ʧ��
		{
			return OnSocketSubLogonFailure(pData,wDataSize);
		}
	case SUB_GR_LOGON_FINISH:	//��¼���
		{
			return OnSocketSubLogonFinish(pData,wDataSize);
		}
	case SUB_GR_UPDATE_NOTIFY:	//������ʾ
		{
			return OnSocketSubUpdateNotify(pData,wDataSize);
		}
	}

	return true;
}

//���ô���
bool CServerViewItem::OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:		//�б�����
		{
			//��������
			CMD_GR_ConfigColumn * pConfigColumn=(CMD_GR_ConfigColumn *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigColumn)-sizeof(pConfigColumn->ColumnItem);

			//Ч�����
			ASSERT((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))!=wDataSize) return false;

			//������Ϣ
			m_cbColumnCount=__min(pConfigColumn->cbColumnCount,CountArray(m_ColumnItem));
			CopyMemory(m_ColumnItem,pConfigColumn->ColumnItem,sizeof(tagColumnItem)*m_cbColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_SERVER:		//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_ConfigServer));
			if (wDataSize<sizeof(CMD_GR_ConfigServer)) return false;

			//��Ϣ����
			CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)pData;

			//��������
			m_wServerType=pConfigServer->wServerType;
			m_dwServerRule=pConfigServer->dwServerRule;
			m_bAllowDistribute=pConfigServer->bAllowDistribute;

			//�������
			WORD wTableCount=pConfigServer->wTableCount;
			WORD wChairCount=pConfigServer->wChairCount;
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);

			//��������
			m_TableViewFrame.ConfigTableFrame(wTableCount,wChairCount);

			return true;
		}
	case SUB_GR_CONFIG_FINISH:		//�������
		{
			//��ԴĿ¼
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//��Ϸ�ȼ�
			ASSERT(m_GameLevelParserModule.GetInterface()!=NULL);
			bool bSuccess=m_GameLevelParserModule->LoadGameLevelItem(m_GameKind.szKindName,szResDirectory,m_wServerType);

			//������
			if (bSuccess==false)
			{
				//�ر�����
				m_TCPSocketModule->CloseSocket();

				//��ʾ��Ϣ
				INT nResult=ShowInformation(TEXT("��Ϸ�ȼ����ö�ȡʧ�ܻ��߸�ʽ�����Ƿ����ڽ����޸���"),MB_ICONERROR|MB_YESNO);

				//������Ϸ
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//�رշ���
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	}

	return true;
}

//���д���
bool CServerViewItem::OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(m_pDlgInsureServer!=NULL);
	if (m_pDlgInsureServer==NULL) return true;

	//��Ϣ����
	if (m_pDlgInsureServer->OnServerInsureMessage(MDM_GR_INSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//������
bool CServerViewItem::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_OPTION_CURRENT:	//��ǰ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GR_OptionCurrent));
			if (wDataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			//��Ϣ����
			if (m_pDlgServerManager!=NULL)
			{
				CMD_GR_OptionCurrent * pOptionCurrent=(CMD_GR_OptionCurrent *)pData;
				m_pDlgServerManager->SetServerOptionInfo(pOptionCurrent->ServerOptionInfo,pOptionCurrent->dwRuleMask);
			}

			return true;
		}
	}

	return true;
}

//ϵͳ����
bool CServerViewItem::OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CM_SYSTEM_MESSAGE:	//ϵͳ��Ϣ
		{
			return OnSocketSubSystemMessage(pData,wDataSize);
		}
	case SUB_CM_ACTION_MESSAGE:	//������Ϣ
		{
			return OnSocketSubActionMessage(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CServerViewItem::OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;

	//��������
	IPC_GF_SocketRecv SocketRecv;
	SocketRecv.CommandInfo.wSubCmdID=wSubCmdID;
	SocketRecv.CommandInfo.wMainCmdID=wMainCmdID;

	//��������
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketRecv.cbBuffer,pData,wDataSize);
	}

	//��������
	WORD wSendSize=sizeof(SocketRecv)-sizeof(SocketRecv.cbBuffer)+wDataSize;
	m_ProcessManager.SendProcessData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_RECV,&SocketRecv,wSendSize);

	return true;
}

//�û�����
bool CServerViewItem::OnSocketSubUserEnter(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//��������
	tagUserInfo UserInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));

	//��Ϣ����
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//�û�����
	UserInfo.wFaceID=pUserInfoHead->wFaceID;
	UserInfo.dwUserID=pUserInfoHead->dwUserID;
	UserInfo.cbGender=pUserInfoHead->cbGender;
	UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

	//�û�״̬
	UserInfo.wTableID=pUserInfoHead->wTableID;
	UserInfo.wChairID=pUserInfoHead->wChairID;
	UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

	//�û�����
	UserInfo.lScore=pUserInfoHead->lScore;
	UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
	UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
	UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
	UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;
	UserInfo.dwExperience=pUserInfoHead->dwExperience;

	//��������
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

	//��չ��Ϣ
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GR_NICK_NAME:		//�û��ǳ�
			{
				ASSERT(pDataBuffer!=NULL);
				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
				if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
				{
					CopyMemory(&UserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
					UserInfo.szNickName[CountArray(UserInfo.szNickName)-1]=0;
				}
				break;
			}
		case DTP_GR_ADDR_DESCRIBE: //��ַ����
			{
				ASSERT(pDataBuffer!=NULL);
				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szAddrDescribe));
				if (DataDescribe.wDataSize<=sizeof(UserInfo.szAddrDescribe))
				{
					CopyMemory(&UserInfo.szAddrDescribe,pDataBuffer,DataDescribe.wDataSize);
					UserInfo.szAddrDescribe[CountArray(UserInfo.szAddrDescribe)-1]=0;
				}

				break;
			}
		}
	}

	//�����û�
	ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==NULL) pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo);

	return true;
}

//�û�����
bool CServerViewItem::OnSocketSubUserScore(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore)) return false;

	//Ѱ���û�
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//�����û�
	m_PlazaUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);

	return true;
}

//�û�״̬
bool CServerViewItem::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//Ѱ���û�
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//����״̬
	if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	{
		//ɾ���û�
		m_PlazaUserManagerModule->DeleteUserItem(pIClientUserItem);
	}
	else
	{
		//�����û�
		m_PlazaUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	}

	return true;
}

//�û���Ϣ
bool CServerViewItem::OnSocketSubUserSpreadInfo(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserSpreadInfo));
	if (wDataSize<sizeof(CMD_GR_UserSpreadInfo)) return false;

	//Ѱ���û�
	CMD_GR_UserSpreadInfo * pUserSpreadInfo=(CMD_GR_UserSpreadInfo *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserSpreadInfo->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//�����û�
	m_PlazaUserManagerModule->UpdateUserItemSpreadInfo(pIClientUserItem,&pUserSpreadInfo->UserSpreadInfo);

	return true;
}

//��¼���
bool CServerViewItem::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//�ر���ʾ
	m_DlgStatus.HideStatusWindow();

	//����״̬
	m_ServiceStatus=ServiceStatus_ServiceIng;

	//�����
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	if (pPlatformFrame!=NULL) pPlatformFrame->ActiveServerViewItem();

	//�����ж�
	if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()!=INVALID_TABLE))
	{
		//��������	
		bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

		//������
		if (bSuccess==false)
		{
			//�ж�����
			m_TCPSocketModule->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetworkDown;

			return false;
		}
	}
	OnHitTableButton(1);
	return true;
}

//��¼�ɹ�
bool CServerViewItem::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_LogonSuccess));
	if (wDataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	//��Ϣ����
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)pData;

	//����״̬
	m_ServiceStatus=ServiceStatus_RecvInfo;

	//�������
	m_dwUserRight=pLogonSuccess->dwUserRight;
	m_dwMasterRight=pLogonSuccess->dwMasterRight;

	//�ر���ʾ
	m_DlgStatus.ShowStatusWindow(TEXT("���ڶ�ȡ������Ϣ..."));

	return true;
}

//��¼ʧ��
bool CServerViewItem::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//Ч�����
	CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//�ر���ʾ
	m_DlgStatus.HideStatusWindow();

	//�رմ���
	m_TCPSocketModule->CloseSocket();

	//����״̬
	m_ServiceStatus=ServiceStatus_NetworkDown;

 	//��ʾ��Ϣ
 	LPCTSTR pszDescribeString=pLogonFailure->szDescribeString;
 	if (lstrlen(pszDescribeString)>0) ShowInformation(pszDescribeString,MB_ICONERROR,60);

	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//������ʾ
bool CServerViewItem::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	//��������
	CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)pData;

	//��������
	if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	{
		//�رմ���
		m_DlgStatus.HideStatusWindow();
		m_TCPSocketModule->CloseSocket();

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//���ش���
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("��Ϸ�㳡"),0,0);

		return true;
	}

	//��Ϸ����
	if ((pUpdateNotify->cbMustUpdateClient==TRUE)||(pUpdateNotify->cbAdviceUpdateClient==TRUE))
	{
		//�رմ���
		if (pUpdateNotify->cbMustUpdateClient==TRUE)
		{
			m_DlgStatus.HideStatusWindow();
			m_TCPSocketModule->CloseSocket();
		}

		//������ʾ
		if (pUpdateNotify->cbMustUpdateClient==FALSE)
		{
			//������ʾ
			TCHAR szDescribe[512]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��%s���Ѿ������ˣ�����ǿ�ҽ��������и��£����ڽ��и�����"),m_GameKind.szKindName);

			//��ʾ��Ϣ
			CInformation Information(this);
			if (Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES) return true;
		}

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//������ʾ
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
	}

	return true;
}

//����ʧ��
bool CServerViewItem::OnSocketSubRequestFailure(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)pData;

	//Ч�����
	ASSERT(wDataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	if (wDataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	//�رմ���
	if(pRequestFailure->cbCommandCode==REQUEST_FAILURE_CMD_CONCLUDE)
		m_ProcessManager.CloseGameProcess(false);

	//��ʾ��Ϣ
	if (pRequestFailure->szDescribeString[0]!=0)
	{
		//��ʾ��Ϣ
		CInformation Information;
		Information.ShowMessageBox(pRequestFailure->szDescribeString,MB_ICONINFORMATION,30L);
	}

	return true;
}

//ϵͳ��Ϣ
bool CServerViewItem::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	//�رմ���
	if ((pSystemMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رմ���
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//״̬����
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//������Ϣ
	if (pSystemMessage->wType&SMT_EJECT)
	{
		CInformation Information;
		Information.ShowMessageBox(pSystemMessage->szString,MB_ICONINFORMATION);
	}

	//�رշ���
	if (pSystemMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//������Ϣ
bool CServerViewItem::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(WCHAR)))) return false;

	//�رմ���
	if ((pActionMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رմ���
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//״̬����
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//������Ϣ
	CInformation Information;
	INT nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

	//��������
	WORD wExcursion=wHeadSize+pActionMessage->wLength*sizeof(TCHAR);

	//��ȡ����
	while (wExcursion<wDataSize)
	{
		//��������
		tagActionHead * pActionHead=(tagActionHead *)((BYTE *)pData+wExcursion);

		//Ч�����
		ASSERT((wExcursion+sizeof(tagActionHead))<=wDataSize);
		ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=wDataSize);

		//Ч�����
		if ((wExcursion+sizeof(tagActionHead))>wDataSize) return false;
		if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>wDataSize) return false;

		//��������
		if (nResultCode==pActionHead->uResponseID)
		{
			switch (pActionHead->cbActionType)
			{
			case ACT_BROWSE:	//�������
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionBrowse * pActionBrowse=(tagActionBrowse *)((BYTE *)pData+wDataPos);

					//I E ���
					if (pActionBrowse->cbBrowseType&BRT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			case ACT_DOWN_LOAD:	//���ض���
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((BYTE *)pData+wDataPos);

					//I E ����
					if (pActionDownLoad->cbDownLoadMode&DLT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//����ģ��
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//����ƫ��
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//�رշ���
	if (pActionMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//�����ؼ�
VOID CServerViewItem::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//���ƿؼ�
	m_TableViewFrame.SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOZORDER);

	//�ػ�����
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

//���÷���
VOID CServerViewItem::InitServerViewItem(CGameServerItem * pGameServerItem)
{
	//����״̬
	m_ServiceStatus=ServiceStatus_Entering;

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
	CopyMemory(&m_GameKind,&pGameKindItem->m_GameKind,sizeof(m_GameKind));
	CopyMemory(&m_GameServer,&pGameServerItem->m_GameServer,sizeof(m_GameServer));

	//�ر��ж�
	ASSERT(m_GameServer.wServerID!=0);
	if (m_GameServer.wServerID==0) throw TEXT("�ܱ�Ǹ������Ϸ�����Ѿ��ر��ˣ�������������룡");

	//�������
	if (m_TCPSocketModule.CreateInstance()==false) throw TEXT("�޷��������������������Ϸ�������ʧ�ܣ�");

	//��������
	m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer);

	CString strLog;
	strLog.Format(L"CONNECTIP %s",m_GameServer.szServerAddr);
	OutputDebugString(strLog);
	//��������
	if (m_TCPSocketModule->Connect(m_GameServer.szServerAddr,m_GameServer.wServerPort)!=CONNECT_SUCCESS)
	{
		throw TEXT("��Ϸ��������ʧ�ܣ�����ϵͳ�������ò����Ƿ���ȷ��");
		return;
	}

	//������ʾ
	m_DlgStatus.ShowStatusWindow(TEXT("���ڽ�����Ϸ���䣬���Ժ�..."));

	return;
}

//��ԴĿ¼
VOID CServerViewItem::GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount)
{
	//��������
	WORD wStringIndex=0;

	//����Ŀ¼
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,m_GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//�ַ���ֹ
	szResDirectory[wStringIndex]=0;

	return;
}
BOOL CServerViewItem::GetHostIP(TCHAR * inData)  
{  
	char name[255];  
	if( gethostname ( name, sizeof(name)) == 0)  
	{  
		hostent* pHostent = gethostbyname(name);  
		hostent& he = *pHostent;  
		sockaddr_in sa;  
		for (int nAdapter=0; he.h_addr_list[nAdapter]; nAdapter++)   
		{  
			memcpy ( &sa.sin_addr.s_addr, he.h_addr_list[nAdapter],he.h_length);  
			CopyMemory(inData,inet_ntoa(*(struct in_addr *)he.h_addr_list[nAdapter]),strlen(name));  

		}       
	}   
	return TRUE;  
}
//���͵�¼
bool CServerViewItem::SendLogonPacket()
{
	//��������
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//��������
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��Ϸ�汾
	LPCTSTR pszProcessName=m_GameKind.szProcessName;
	CWHService::GetModuleVersion(pszProcessName,LogonUserID.dwProcessVersion);

	//������Ϣ
	LogonUserID.dwPlazaVersion=pGlobalUnits->GetPlazaVersion();
	LogonUserID.dwFrameVersion=pGlobalUnits->GetFrameVersion();

	//��¼��Ϣ
	LogonUserID.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(LogonUserID.szPassword,pGlobalUserData->szPassword,CountArray(LogonUserID.szPassword));

	//�㳡����
#ifdef	RELEASE_MANAGER
	LogonUserID.cbPlazaType = PLAZE_TYPE_MULTI;
#else
	LogonUserID.cbPlazaType = PLAZA_TYPE_NORMAL;
#endif

	//��������
	m_TCPSocketModule->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//�����Թ�
bool CServerViewItem::SendLookonPacket(WORD wTableID, WORD wChairID)
{
	//��������
	CMD_GR_UserLookon UserLookon;
	ZeroMemory(&UserLookon,sizeof(UserLookon));

	//��������
	UserLookon.wTableID=wTableID;
	UserLookon.wChairID=wChairID;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON,&UserLookon,sizeof(UserLookon));

	return true;
}

//��������
bool CServerViewItem::SendSitDownPacket(WORD wTableID, WORD wChairID)
{
	//��������
	CMD_GR_UserSitDown UserSitDown;
	ZeroMemory(&UserSitDown,sizeof(UserSitDown));

	//��������
	UserSitDown.wTableID=wTableID;
	UserSitDown.wChairID=wChairID;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitDown,sizeof(UserSitDown));

	return true;
}

//��������
bool CServerViewItem::SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
	//��������
	CMD_GR_UserStandUp UserStandUp;
	ZeroMemory(&UserStandUp,sizeof(UserStandUp));

	//��������
	UserStandUp.wTableID=wTableID;
	UserStandUp.wChairID=wChairID;
	UserStandUp.cbForceLeave=cbForceLeave;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP,&UserStandUp,sizeof(UserStandUp));

	return true;
}

//ִ���Թ�
bool CServerViewItem::PerformLookonAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//Ȩ���ж�
	if (CUserRight::CanLookon(m_dwUserRight)==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��Ǹ������ʱû���Թ���Ϸ��Ȩ�ޣ�"));

		return false;
	}

	//��������
	SendLookonPacket(wTableID,wChairID);

	return true;
}

//ִ������
bool CServerViewItem::PerformStandUpAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//��������
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//ִ������
bool CServerViewItem::PerformSitDownAction(WORD wTableID, WORD wChairID)
{
	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//Ȩ���ж�
	if(CheckUserRight()==false) return false;
	

	//��������
	SendSitDownPacket(wTableID,wChairID);

	return true;
}

//Ȩ�޼��
bool CServerViewItem::CheckUserRight()
{
	//Ȩ���ж�
	if (CUserRight::CanPlay(m_dwUserRight)==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��Ǹ������ʱû�м�����Ϸ��Ȩ�ޣ�"));

		return false;
	}

	return true;
}

//������Ϣ
VOID CServerViewItem::OnNcDestroy()
{
	__super::OnNcDestroy();

	//�������
	m_bCreateFlag=false;

	//�û�����
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//��������
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_bAllowDistribute=FALSE;
	m_ServiceStatus=ServiceStatus_Unknow;

	//������Ϣ
	m_cbColumnCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));

	//��������
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	return;
}

//�滭����
BOOL CServerViewItem::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//λ����Ϣ
VOID CServerViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
