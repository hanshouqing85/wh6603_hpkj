#include "StdAfx.h"
#include "MissionManager.h"
#include "LogFile.h"
//////////////////////////////////////////////////////////////////////////////////

//�ر�����
#define IDI_CLOSE_LINK					100								//��������
#define IDI_CANCEL_LINK					101								//ȡ������

//////////////////////////////////////////////////////////////////////////////////
TCHAR							szCurrentServer[LEN_SERVER];		//��ǰ��ַ

BEGIN_MESSAGE_MAP(CMissionManager, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CMissionSocket::CMissionSocket()
{
	//״̬����
	m_bSwitchDns=false;
	m_bTryDefault=false;
	m_wSwitchIndex=0;

	//��ǰ����
	m_wCurrentPort=PORT_LOGON;
	m_dwCurrentServer=INADDR_NONE;

	//��ַ��Ϣ
	m_szCustomServer[0]=0;
	m_szCurrentServer[0]=0;

	//�ӿڱ���
	m_pIMissionSocketSink=NULL;

	
	ZeroMemory(szPlatformUrl,sizeof(szPlatformUrl));

	//��ȡ�����ļ�
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	//��ȡ����
	CWHIniData IniData;
	IniData.SetIniFilePath(szFileName);

	TCHAR szKey[20];
	ZeroMemory(&szKey,sizeof(szKey));
	for(int i = 0;i < 5;i++)
	{
		_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),i);
		//��ȡ����
		IniData.ReadEncryptString(TEXT("PlazaUrl"),szKey,NULL,szPlatformUrl[i],CountArray(szPlatformUrl[i]));
	}

	return;
}

//��������
CMissionSocket::~CMissionSocket()
{
}

//�ӿڲ�ѯ
VOID * CMissionSocket::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocketSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CMissionSocket::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
 	//�л�����
 	if ((nErrorCode==WSAETIMEDOUT)||(nErrorCode==WSAECONNREFUSED)||(nErrorCode==WSAEADDRNOTAVAIL))
 	{
		DWORD dwTickCount = GetTickCount();
		CString strLog;

		strLog.Format(L"PINGSECOND  MISSIONSEND CMissionSocket LinkError curtime: %d ",dwTickCount);
		OutputDebugString(strLog);

 		//�л�����
 		if (SwitchServerItem()==true)
 		{
 			//��������
 			if (m_TCPSocketModule->Connect(m_dwCurrentServer,m_wCurrentPort)==CONNECT_SUCCESS) return true;
 		}
 	}
	//����֪ͨ
	ASSERT(m_pIMissionSocketSink!=NULL);
	if (m_pIMissionSocketSink!=NULL) m_pIMissionSocketSink->OnEventMissionLink(nErrorCode, wSocketID);

	return true;
}

//�ر��¼�
bool CMissionSocket::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//�ر�֪ͨ
	ASSERT(m_pIMissionSocketSink!=NULL);
	if (m_pIMissionSocketSink!=NULL) m_pIMissionSocketSink->OnEventMissionShut(cbShutReason);

	return true;
}

//��ȡ�¼�
bool CMissionSocket::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//��ȡ֪ͨ
	ASSERT(m_pIMissionSocketSink!=NULL);
	if (m_pIMissionSocketSink!=NULL) m_pIMissionSocketSink->OnEventMissionRead(Command,pData,wDataSize);

	return true;
}

//����״̬
BYTE CMissionSocket::GetSocketStatus()
{
	//��������
	BYTE cbSocketStatus=SOCKET_STATUS_IDLE;

	//��ȡ״̬
	if (m_TCPSocketModule.GetInterface()!=NULL)
	{
		cbSocketStatus=m_TCPSocketModule->GetSocketStatus();
	}

	return cbSocketStatus;
}

//���õ�ַ
VOID CMissionSocket::SetCustomServer(LPCTSTR pszCustomServer)
{
	//���ñ���
	ASSERT((pszCustomServer!=NULL)&&(pszCustomServer[0]!=0));
	lstrcpyn(m_szCustomServer,pszCustomServer,CountArray(m_szCustomServer)); 
	lstrcpyn(szCurrentServer,m_szCustomServer,CountArray(szCurrentServer)); 

	return;
}

//�ж�����
bool CMissionSocket::PerformClose()
{
	//�ر�����
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);
	if (m_TCPSocketModule.GetInterface()!=NULL) m_TCPSocketModule->CloseSocket();

	return true;
}

//��������
bool CMissionSocket::PerformConnect(bool bContinue)
{
	//״̬Ч��
	ASSERT(GetSocketStatus()==SOCKET_STATUS_IDLE);
	if (GetSocketStatus()!=SOCKET_STATUS_IDLE) return false;

	//�������
	if (m_TCPSocketModule.GetInterface()==NULL)
	{
		if (m_TCPSocketModule.CreateInstance()==false) return false;
		if (m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx))==false) return false;
	}

	//���û���
	if (bContinue==false)
	{
		//���ñ���
		m_bSwitchDns=false;
		m_bTryDefault=false;
		m_wSwitchIndex=0;

		//���ô���
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer)==false)
		{
			ASSERT(FALSE);
			return false;
		}

  		//��ȡ��ַ
  		if (m_szCustomServer[0]==0)
  		{
			lstrcpyn(m_szCustomServer,szCurrentServer,CountArray(m_szCustomServer)); 
  		}


 		//���ñ���
 		m_wCurrentPort=PORT_LOGON;
 		lstrcpyn(m_szCurrentServer,m_szCustomServer,CountArray(m_szCurrentServer));
 
		CString strLog;
		strLog.Format(L"MISSIONSEND connectip:%s",m_szCurrentServer);
		OutputDebugString(strLog);
 		//Ĭ�ϵ�ַ
 		if ((GetServerInfo()==false)&&(SwitchServerItem()==false))
 		{
 			ASSERT(FALSE);
 			return false;
 		}
	}
	else
	{
		//�л�����
		if (SwitchServerItem()==false)
		{
			return false;
		}
	}

	//��������
	if (m_TCPSocketModule->Connect(m_dwCurrentServer,m_wCurrentPort)!=CONNECT_SUCCESS) return false;

	return true;
}

//���ͺ���
bool CMissionSocket::PerformSendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//״̬Ч��
	ASSERT(GetSocketStatus()==SOCKET_STATUS_CONNECT);
	if (GetSocketStatus()!=SOCKET_STATUS_CONNECT) return false;

	WORD wSendSize = 0;

	//��������
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);
	if (m_TCPSocketModule.GetInterface()!=NULL)
		wSendSize = m_TCPSocketModule->SendData(wMainCmdID,wSubCmdID);
	if(wSendSize>0)
		return true;
	else
		return false;

	return true;
}

//���ͺ���
bool CMissionSocket::PerformSendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(GetSocketStatus()==SOCKET_STATUS_CONNECT);
	if (GetSocketStatus()!=SOCKET_STATUS_CONNECT) return false;

	WORD wSendSize = 0;
	//��������
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);
	if (m_TCPSocketModule.GetInterface()!=NULL) 
		wSendSize = m_TCPSocketModule->SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
	if(wSendSize>0)
		return true;
	else
		return false;
	return true;
}

//��ַ��Ϣ
bool CMissionSocket::GetServerInfo()
{
	//Ч�����
	ASSERT(m_szCurrentServer[0]!=0);
	if (m_szCurrentServer[0]==0) return false;

	//��������
	tagServerItem ServerItem;
	ZeroMemory(&ServerItem,sizeof(ServerItem));

	//���ñ���
	m_wCurrentPort=PORT_LOGON;
	m_dwCurrentServer=INADDR_NONE;

	//��ȡ��Ϣ
 	CGlobalServer GlobalServer;
 	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
 	bool bSuccess=true;//GlobalServer.GetServerItemInfo(m_szCurrentServer,ServerItem);

// 	//ת����ַ
// 	if (bSuccess==true)
// 	{
// 		//ʹ������
// 		if (m_bSwitchDns==true)
// 		{
// 			LPHOSTENT lpHost=gethostbyname(CT2CA(ServerItem.szServerHost));
// 			if (lpHost!=NULL) ServerItem.dwServerAddr=((LPIN_ADDR)lpHost->h_addr)->s_addr;
// 		}
// 
// 		//ʹ�õ�ַ
// 		if ((m_bSwitchDns==false)&&(ServerItem.dwServerAddr==INADDR_NONE))
// 		{
// 			CT2CA strServerDomain(ServerItem.szServerHost);
// 			ServerItem.dwServerAddr=inet_addr(strServerDomain);
// 		}
// 	}
// 
// 	//���ַ���
// 	if (ServerItem.dwServerAddr==INADDR_NONE)
//	{

		USES_CONVERSION;
		int nValidHostName = inet_addr(W2A(m_szCustomServer));

		if(nValidHostName == -1)
		{

			CString strLog;
			strLog.Format(L"MISSIONSEND nValidHostName=-1  :%s",m_szCustomServer);
			OutputDebugString(strLog);

			DWORD dwServerAddr = 0;
			LPHOSTENT lpHost=gethostbyname(CT2CA(m_szCustomServer));
			if (lpHost!=NULL) m_dwCurrentServer=((LPIN_ADDR)lpHost->h_addr)->s_addr;

		}
		else
		{

			CString strLog;
			strLog.Format(L"MISSIONSEND nValidHostName=else :%s",m_szCurrentServer);
			OutputDebugString(strLog);

			//��ַת��
			CT2CA CurrentServer(m_szCurrentServer);
			m_dwCurrentServer=inet_addr(CurrentServer);

		}

		return true;
// 		//��������
// 		if (ServerItem.dwServerAddr==INADDR_NONE)
// 		{
// 			LPHOSTENT lpHost=gethostbyname(CurrentServer);
// 			if (lpHost!=NULL) ServerItem.dwServerAddr=((LPIN_ADDR)lpHost->h_addr)->s_addr;
// 		}
// 	}
// 
// 	//�������
// 	if (ServerItem.dwServerAddr!=INADDR_NONE)
// 	{
// 		//���ñ���
// 		m_wCurrentPort=ServerItem.wServerPort;
// 		m_dwCurrentServer=ServerItem.dwServerAddr;
// 
// 		return true;
// 	}

	return false;
}

//�л�����
bool CMissionSocket::SwitchServerItem()
{
	do
	{
		//��ȡ��Ϣ
// 		CGlobalServer GlobalServer;
// 		GlobalServer.GetServerItemName(++m_wSwitchIndex,m_szCurrentServer);
		if(m_wSwitchIndex>4)
		{
			m_wSwitchIndex=0;
			return false;
		}
		lstrcpyn(m_szCurrentServer,szPlatformUrl[m_wSwitchIndex],CountArray(m_szCurrentServer));
		CString strLog;

		strLog.Format(L"PINGSECOND  MISSIONSEND SwitchServerItem  SwitchIP:%s",m_szCurrentServer);
		OutputDebugString(strLog);
		LogFile::instance().LogText(strLog);

		m_wSwitchIndex = m_wSwitchIndex+1;
		//״̬�л�
		if ((m_szCurrentServer[0]==0)&&(m_bSwitchDns==false))
		{
			
			m_wSwitchIndex=INVALID_WORD;
			continue;
		}

		m_bSwitchDns=true;
		//����ж�
		if ((m_szCurrentServer[0]==0)&&(m_bSwitchDns==true)) break;

		//��ͬ�ж�
		if ((m_bSwitchDns==false)&&(lstrcmp(m_szCurrentServer,m_szCustomServer)==0)) continue;
		lstrcpyn(m_szCustomServer,m_szCurrentServer,CountArray(m_szCustomServer));

		//��ȡ��Ϣ
		if (GetServerInfo()==true) return true;

	} while (true);

	//����Ĭ��
	if (m_bTryDefault==false)
	{
		//���ñ���
		m_bTryDefault=true;


		lstrcpyn(m_szCurrentServer,szPlatformUrl[rand()%20],CountArray(m_szCurrentServer));

		//��ȡ��Ϣ
		if (GetServerInfo()==true) return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CMissionItem::CMissionItem()
{
	//���ñ���
	m_bActiveStatus=false;
	m_pMissionManager=NULL;

	return;
}

//��������
CMissionItem::~CMissionItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CMissionManager::CMissionManager()
{
	//�������
	m_MissionSocket.SetMissionSocketSink(this);

	return;
}

//��������
CMissionManager::~CMissionManager()
{
}

//�����¼�
bool CMissionManager::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{
	//����֪ͨ
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		//��ȡ����
		CMissionItem * pMissionItem=m_MissionItemArray[i];

		//�¼�����
		if (pMissionItem->m_bActiveStatus==true)
		{
			//����״̬
			if (nErrorCode!=0L)
			{
				pMissionItem->m_bActiveStatus=false;
			}

			pMissionItem->m_nSocketID = nSocketID;							//SAVE SOCKET ID

			//�¼�֪ͨ
			pMissionItem->OnEventMissionLink(nErrorCode, nSocketID);
		}
	}

	return true;
}

//�ر��¼�
bool CMissionManager::OnEventMissionShut(BYTE cbShutReason)
{
	int nCount = m_MissionItemArray.GetCount();
	CString strLog;
	strLog.Format(L"\nMissionArray:%d",nCount);
	OutputDebugString(strLog);
	//�ر�֪ͨ
	for (INT_PTR i=0;i<nCount;i++)
	{
		if(!AfxIsValidAddress((const void*)&m_MissionItemArray, sizeof(m_MissionItemArray)))
			return false;
		if(m_MissionItemArray.IsEmpty())
			return true;
		//��ȡ����
		try{

			CMissionItem * pMissionItem=m_MissionItemArray[i];
			if(pMissionItem == NULL)
			{
				continue;
			}
			//�¼�����
			if (pMissionItem->m_bActiveStatus==true)
			{
				pMissionItem->m_bActiveStatus=false;
				pMissionItem->OnEventMissionShut(cbShutReason);
			}
		}catch(...)
		{
			;
		}
	}

	return true;
}

//��ȡ�¼�
bool CMissionManager::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	int nCount = m_MissionItemArray.GetCount();
	//����֪ͨ
	for (INT_PTR i=0;i<nCount;i++)
	{
		CMissionItem * pMissionItem=m_MissionItemArray[i];
		if(pMissionItem->m_pMissionManager!=NULL)
			pMissionItem->OnEventMissionRead(Command,pData,wDataSize);
	}

	return true;
}

//���õ�ַ
VOID CMissionManager::SetCustomServer(LPCTSTR pszCustomServer)
{
	//���õ�ַ
	m_MissionSocket.SetCustomServer(pszCustomServer);

	return;
}

//��������
bool CMissionManager::InsertMissionItem(CMissionItem * pMissionItem)
{
	//��������
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		if (m_MissionItemArray[i]==pMissionItem)
		{
			ASSERT(FALSE);
			return true;
		}
	}

	//��������
	pMissionItem->m_bActiveStatus=false;
	pMissionItem->m_pMissionManager=this;

	//��������
	m_MissionItemArray.Add(pMissionItem);

	return true;
}
bool CMissionManager::ClearMissionItem()
{
	CMissionItem * pMissionItem = NULL;
	//ɾ������
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		pMissionItem = m_MissionItemArray[i];
		//ɾ������
		m_MissionItemArray.RemoveAt(i);

		//��������
		pMissionItem->m_bActiveStatus=false;
		pMissionItem->m_pMissionManager=NULL;

	}

	return true;
}
//ɾ������
bool CMissionManager::DeleteMissionItem(CMissionItem * pMissionItem)
{
	//��ֹ����
	if (pMissionItem->m_bActiveStatus==true)
	{
		ConcludeMissionItem(pMissionItem,false);
	}

	//ɾ������
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		if (m_MissionItemArray[i]==pMissionItem)
		{
			//ɾ������
			m_MissionItemArray.RemoveAt(i);

			//��������
			pMissionItem->m_bActiveStatus=false;
			pMissionItem->m_pMissionManager=NULL;

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��������
bool CMissionManager::AvtiveMissionItem(CMissionItem * pMissionItem, bool bContinue)
{
	if(this == NULL)
	{

		OutputDebugString(L"SOCKETSTATUS AvtiveMissionItem this == NULL");

		return false;
	}
	//Ч��״̬
	ASSERT(pMissionItem->m_pMissionManager==this);
	if (pMissionItem->m_pMissionManager!=this)
	{
		OutputDebugString(L"SOCKETSTATUS AvtiveMissionItem m_pMissionManager!=this");
		return false;
	}

	//�رմ���
	if (m_hWnd!=NULL) DestroyWindow();

	//��������
	pMissionItem->m_bActiveStatus=true;

	//���Ӵ���
	switch (m_MissionSocket.GetSocketStatus())
	{
	case SOCKET_STATUS_IDLE:	//����״̬
		{
			if(GetSafeHwnd())
				SetTimer(IDI_CANCEL_LINK,10*1000,NULL);
			//��������
			if (m_MissionSocket.PerformConnect(bContinue)==false)
			{
				KillTimer(IDI_CANCEL_LINK);
				//��������
				pMissionItem->m_bActiveStatus=false;

				OutputDebugString(L"SOCKETSTATUS AvtiveMissionItem PerformConnect==false");

				return false;
			}

			break;
		}
	case SOCKET_STATUS_CONNECT:	//����״̬
		{
			if(pMissionItem!=NULL)
			{
				//�¼�֪ͨ
				pMissionItem->OnEventMissionLink(0L, pMissionItem->m_nSocketID);

			}

			break;
		}
	}
	return true;
}

//��ֹ����
bool CMissionManager::ConcludeMissionItem(CMissionItem * pMissionItem, bool bDeferIntermit)
{
	if (pMissionItem->m_bActiveStatus==false) return false;
	//Ч��״̬
	ASSERT(pMissionItem->m_bActiveStatus==true);

	//��������
	pMissionItem->m_bActiveStatus=false;

	//��������
	UINT uActiveCount=0;
	BYTE cbSocketStatus=m_MissionSocket.GetSocketStatus();

	//��������
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		CMissionItem * pMissionItem=m_MissionItemArray[i];
		if (pMissionItem->m_bActiveStatus==true) uActiveCount++;
	}

	//��ֹ����
	if ((uActiveCount==0)&&(cbSocketStatus!=SOCKET_STATUS_IDLE))
	{
		//��������
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//�ر��ж�
		if ((bDeferIntermit==true)&&(pParameterGlobal->m_wIntermitTime>0))
		{
			//��������
			if (m_hWnd==NULL)
			{
				CRect rcCreate(0,0,0,0);
				Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
			}

			//����ʱ��
			SetTimer(IDI_CLOSE_LINK,pParameterGlobal->m_wIntermitTime*1000L,NULL);
		}
		else
		{
			//�ر�����
			m_MissionSocket.PerformClose();
		}
	}

	return false;
}

//���ͺ���
bool CMissionManager::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	bool bSuccess = true;
	//��������
	bSuccess = m_MissionSocket.PerformSendData(wMainCmdID,wSubCmdID);


	if(!bSuccess)
	{
		CString strLog;
		strLog.Format(L"MISSIONSEND wMainCmdID:%d,wSubCmdID:%d",wMainCmdID,wSubCmdID);
		OutputDebugString(strLog);

		m_MissionSocket.PerformClose();
		m_MissionSocket.PerformConnect(false);
	}
	return true;
}

//���ͺ���
bool CMissionManager::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)
{
	bool bSuccess = true;

	//��������
	bSuccess = m_MissionSocket.PerformSendData(wMainCmdID,wSubCmdID,pData,wDataSize);

	if(!bSuccess)
	{
		m_MissionSocket.PerformClose();
		bSuccess = m_MissionSocket.PerformConnect(false);

		CString strLog;
		strLog.Format(L"MISSIONSEND FAILE reconnect:%s wMainCmdID:%d,wSubCmdID:%d",bSuccess?L"success":L"faile",wMainCmdID,wSubCmdID);
		LogFile::instance().LogText(strLog);

	}
	return true;
}

//ʱ����Ϣ
VOID CMissionManager::OnTimer(UINT nIDEvent)
{
	//�ر�����
	if (nIDEvent==IDI_CLOSE_LINK)
	{
		//�رմ���
		DestroyWindow();

		//��ֹ����
		m_MissionSocket.PerformClose();

		return;
	}
	else if(nIDEvent == IDI_CANCEL_LINK)
	{
	//	ConcludeMissionItem()
		//�رմ���
		DestroyWindow();

		//��ֹ����
		m_MissionSocket.PerformClose();
		KillTimer(IDI_CANCEL_LINK);
		OnEventMissionLink(10049,0);
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////