#include "StdAfx.h"
#include "MissionManager.h"
#include "LogFile.h"
//////////////////////////////////////////////////////////////////////////////////

//关闭连接
#define IDI_CLOSE_LINK					100								//激活任务
#define IDI_CANCEL_LINK					101								//取消链接

//////////////////////////////////////////////////////////////////////////////////
TCHAR							szCurrentServer[LEN_SERVER];		//当前地址

BEGIN_MESSAGE_MAP(CMissionManager, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CMissionSocket::CMissionSocket()
{
	//状态变量
	m_bSwitchDns=false;
	m_bTryDefault=false;
	m_wSwitchIndex=0;

	//当前连接
	m_wCurrentPort=PORT_LOGON;
	m_dwCurrentServer=INADDR_NONE;

	//地址信息
	m_szCustomServer[0]=0;
	m_szCurrentServer[0]=0;

	//接口变量
	m_pIMissionSocketSink=NULL;

	
	ZeroMemory(szPlatformUrl,sizeof(szPlatformUrl));

	//读取配置文件
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	//读取配置
	CWHIniData IniData;
	IniData.SetIniFilePath(szFileName);

	TCHAR szKey[20];
	ZeroMemory(&szKey,sizeof(szKey));
	for(int i = 0;i < 5;i++)
	{
		_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),i);
		//读取配置
		IniData.ReadEncryptString(TEXT("PlazaUrl"),szKey,NULL,szPlatformUrl[i],CountArray(szPlatformUrl[i]));
	}

	return;
}

//析构函数
CMissionSocket::~CMissionSocket()
{
}

//接口查询
VOID * CMissionSocket::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocketSink,Guid,dwQueryVer);
	return NULL;
}

//连接事件
bool CMissionSocket::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
 	//切换连接
 	if ((nErrorCode==WSAETIMEDOUT)||(nErrorCode==WSAECONNREFUSED)||(nErrorCode==WSAEADDRNOTAVAIL))
 	{
		DWORD dwTickCount = GetTickCount();
		CString strLog;

		strLog.Format(L"PINGSECOND  MISSIONSEND CMissionSocket LinkError curtime: %d ",dwTickCount);
		OutputDebugString(strLog);

 		//切换连接
 		if (SwitchServerItem()==true)
 		{
 			//发起连接
 			if (m_TCPSocketModule->Connect(m_dwCurrentServer,m_wCurrentPort)==CONNECT_SUCCESS) return true;
 		}
 	}
	//连接通知
	ASSERT(m_pIMissionSocketSink!=NULL);
	if (m_pIMissionSocketSink!=NULL) m_pIMissionSocketSink->OnEventMissionLink(nErrorCode, wSocketID);

	return true;
}

//关闭事件
bool CMissionSocket::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//关闭通知
	ASSERT(m_pIMissionSocketSink!=NULL);
	if (m_pIMissionSocketSink!=NULL) m_pIMissionSocketSink->OnEventMissionShut(cbShutReason);

	return true;
}

//读取事件
bool CMissionSocket::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//读取通知
	ASSERT(m_pIMissionSocketSink!=NULL);
	if (m_pIMissionSocketSink!=NULL) m_pIMissionSocketSink->OnEventMissionRead(Command,pData,wDataSize);

	return true;
}

//网络状态
BYTE CMissionSocket::GetSocketStatus()
{
	//变量定义
	BYTE cbSocketStatus=SOCKET_STATUS_IDLE;

	//获取状态
	if (m_TCPSocketModule.GetInterface()!=NULL)
	{
		cbSocketStatus=m_TCPSocketModule->GetSocketStatus();
	}

	return cbSocketStatus;
}

//设置地址
VOID CMissionSocket::SetCustomServer(LPCTSTR pszCustomServer)
{
	//设置变量
	ASSERT((pszCustomServer!=NULL)&&(pszCustomServer[0]!=0));
	lstrcpyn(m_szCustomServer,pszCustomServer,CountArray(m_szCustomServer)); 
	lstrcpyn(szCurrentServer,m_szCustomServer,CountArray(szCurrentServer)); 

	return;
}

//中断连接
bool CMissionSocket::PerformClose()
{
	//关闭连接
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);
	if (m_TCPSocketModule.GetInterface()!=NULL) m_TCPSocketModule->CloseSocket();

	return true;
}

//发起连接
bool CMissionSocket::PerformConnect(bool bContinue)
{
	//状态效验
	ASSERT(GetSocketStatus()==SOCKET_STATUS_IDLE);
	if (GetSocketStatus()!=SOCKET_STATUS_IDLE) return false;

	//创建组件
	if (m_TCPSocketModule.GetInterface()==NULL)
	{
		if (m_TCPSocketModule.CreateInstance()==false) return false;
		if (m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx))==false) return false;
	}

	//设置环境
	if (bContinue==false)
	{
		//设置变量
		m_bSwitchDns=false;
		m_bTryDefault=false;
		m_wSwitchIndex=0;

		//设置代理
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer)==false)
		{
			ASSERT(FALSE);
			return false;
		}

  		//读取地址
  		if (m_szCustomServer[0]==0)
  		{
			lstrcpyn(m_szCustomServer,szCurrentServer,CountArray(m_szCustomServer)); 
  		}


 		//设置变量
 		m_wCurrentPort=PORT_LOGON;
 		lstrcpyn(m_szCurrentServer,m_szCustomServer,CountArray(m_szCurrentServer));
 
		CString strLog;
		strLog.Format(L"MISSIONSEND connectip:%s",m_szCurrentServer);
		OutputDebugString(strLog);
 		//默认地址
 		if ((GetServerInfo()==false)&&(SwitchServerItem()==false))
 		{
 			ASSERT(FALSE);
 			return false;
 		}
	}
	else
	{
		//切换连接
		if (SwitchServerItem()==false)
		{
			return false;
		}
	}

	//发起连接
	if (m_TCPSocketModule->Connect(m_dwCurrentServer,m_wCurrentPort)!=CONNECT_SUCCESS) return false;

	return true;
}

//发送函数
bool CMissionSocket::PerformSendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//状态效验
	ASSERT(GetSocketStatus()==SOCKET_STATUS_CONNECT);
	if (GetSocketStatus()!=SOCKET_STATUS_CONNECT) return false;

	WORD wSendSize = 0;

	//发送数据
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);
	if (m_TCPSocketModule.GetInterface()!=NULL)
		wSendSize = m_TCPSocketModule->SendData(wMainCmdID,wSubCmdID);
	if(wSendSize>0)
		return true;
	else
		return false;

	return true;
}

//发送函数
bool CMissionSocket::PerformSendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)
{
	//状态效验
	ASSERT(GetSocketStatus()==SOCKET_STATUS_CONNECT);
	if (GetSocketStatus()!=SOCKET_STATUS_CONNECT) return false;

	WORD wSendSize = 0;
	//发送数据
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);
	if (m_TCPSocketModule.GetInterface()!=NULL) 
		wSendSize = m_TCPSocketModule->SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
	if(wSendSize>0)
		return true;
	else
		return false;
	return true;
}

//地址信息
bool CMissionSocket::GetServerInfo()
{
	//效验参数
	ASSERT(m_szCurrentServer[0]!=0);
	if (m_szCurrentServer[0]==0) return false;

	//变量定义
	tagServerItem ServerItem;
	ZeroMemory(&ServerItem,sizeof(ServerItem));

	//设置变量
	m_wCurrentPort=PORT_LOGON;
	m_dwCurrentServer=INADDR_NONE;

	//获取信息
 	CGlobalServer GlobalServer;
 	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
 	bool bSuccess=true;//GlobalServer.GetServerItemInfo(m_szCurrentServer,ServerItem);

// 	//转换地址
// 	if (bSuccess==true)
// 	{
// 		//使用域名
// 		if (m_bSwitchDns==true)
// 		{
// 			LPHOSTENT lpHost=gethostbyname(CT2CA(ServerItem.szServerHost));
// 			if (lpHost!=NULL) ServerItem.dwServerAddr=((LPIN_ADDR)lpHost->h_addr)->s_addr;
// 		}
// 
// 		//使用地址
// 		if ((m_bSwitchDns==false)&&(ServerItem.dwServerAddr==INADDR_NONE))
// 		{
// 			CT2CA strServerDomain(ServerItem.szServerHost);
// 			ServerItem.dwServerAddr=inet_addr(strServerDomain);
// 		}
// 	}
// 
// 	//名字翻译
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

			//地址转换
			CT2CA CurrentServer(m_szCurrentServer);
			m_dwCurrentServer=inet_addr(CurrentServer);

		}

		return true;
// 		//域名解释
// 		if (ServerItem.dwServerAddr==INADDR_NONE)
// 		{
// 			LPHOSTENT lpHost=gethostbyname(CurrentServer);
// 			if (lpHost!=NULL) ServerItem.dwServerAddr=((LPIN_ADDR)lpHost->h_addr)->s_addr;
// 		}
// 	}
// 
// 	//结果处理
// 	if (ServerItem.dwServerAddr!=INADDR_NONE)
// 	{
// 		//设置变量
// 		m_wCurrentPort=ServerItem.wServerPort;
// 		m_dwCurrentServer=ServerItem.dwServerAddr;
// 
// 		return true;
// 	}

	return false;
}

//切换连接
bool CMissionSocket::SwitchServerItem()
{
	do
	{
		//读取信息
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
		//状态切换
		if ((m_szCurrentServer[0]==0)&&(m_bSwitchDns==false))
		{
			
			m_wSwitchIndex=INVALID_WORD;
			continue;
		}

		m_bSwitchDns=true;
		//结果判断
		if ((m_szCurrentServer[0]==0)&&(m_bSwitchDns==true)) break;

		//相同判断
		if ((m_bSwitchDns==false)&&(lstrcmp(m_szCurrentServer,m_szCustomServer)==0)) continue;
		lstrcpyn(m_szCustomServer,m_szCurrentServer,CountArray(m_szCustomServer));

		//获取信息
		if (GetServerInfo()==true) return true;

	} while (true);

	//尝试默认
	if (m_bTryDefault==false)
	{
		//设置变量
		m_bTryDefault=true;


		lstrcpyn(m_szCurrentServer,szPlatformUrl[rand()%20],CountArray(m_szCurrentServer));

		//获取信息
		if (GetServerInfo()==true) return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CMissionItem::CMissionItem()
{
	//设置变量
	m_bActiveStatus=false;
	m_pMissionManager=NULL;

	return;
}

//析构函数
CMissionItem::~CMissionItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CMissionManager::CMissionManager()
{
	//设置组件
	m_MissionSocket.SetMissionSocketSink(this);

	return;
}

//析构函数
CMissionManager::~CMissionManager()
{
}

//连接事件
bool CMissionManager::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{
	//连接通知
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		//获取子项
		CMissionItem * pMissionItem=m_MissionItemArray[i];

		//事件处理
		if (pMissionItem->m_bActiveStatus==true)
		{
			//激活状态
			if (nErrorCode!=0L)
			{
				pMissionItem->m_bActiveStatus=false;
			}

			pMissionItem->m_nSocketID = nSocketID;							//SAVE SOCKET ID

			//事件通知
			pMissionItem->OnEventMissionLink(nErrorCode, nSocketID);
		}
	}

	return true;
}

//关闭事件
bool CMissionManager::OnEventMissionShut(BYTE cbShutReason)
{
	int nCount = m_MissionItemArray.GetCount();
	CString strLog;
	strLog.Format(L"\nMissionArray:%d",nCount);
	OutputDebugString(strLog);
	//关闭通知
	for (INT_PTR i=0;i<nCount;i++)
	{
		if(!AfxIsValidAddress((const void*)&m_MissionItemArray, sizeof(m_MissionItemArray)))
			return false;
		if(m_MissionItemArray.IsEmpty())
			return true;
		//获取子项
		try{

			CMissionItem * pMissionItem=m_MissionItemArray[i];
			if(pMissionItem == NULL)
			{
				continue;
			}
			//事件处理
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

//读取事件
bool CMissionManager::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	int nCount = m_MissionItemArray.GetCount();
	//数据通知
	for (INT_PTR i=0;i<nCount;i++)
	{
		CMissionItem * pMissionItem=m_MissionItemArray[i];
		if(pMissionItem->m_pMissionManager!=NULL)
			pMissionItem->OnEventMissionRead(Command,pData,wDataSize);
	}

	return true;
}

//设置地址
VOID CMissionManager::SetCustomServer(LPCTSTR pszCustomServer)
{
	//设置地址
	m_MissionSocket.SetCustomServer(pszCustomServer);

	return;
}

//加入任务
bool CMissionManager::InsertMissionItem(CMissionItem * pMissionItem)
{
	//查找现有
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		if (m_MissionItemArray[i]==pMissionItem)
		{
			ASSERT(FALSE);
			return true;
		}
	}

	//设置子项
	pMissionItem->m_bActiveStatus=false;
	pMissionItem->m_pMissionManager=this;

	//加入任务
	m_MissionItemArray.Add(pMissionItem);

	return true;
}
bool CMissionManager::ClearMissionItem()
{
	CMissionItem * pMissionItem = NULL;
	//删除任务
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		pMissionItem = m_MissionItemArray[i];
		//删除数组
		m_MissionItemArray.RemoveAt(i);

		//设置子项
		pMissionItem->m_bActiveStatus=false;
		pMissionItem->m_pMissionManager=NULL;

	}

	return true;
}
//删除任务
bool CMissionManager::DeleteMissionItem(CMissionItem * pMissionItem)
{
	//终止任务
	if (pMissionItem->m_bActiveStatus==true)
	{
		ConcludeMissionItem(pMissionItem,false);
	}

	//删除任务
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		if (m_MissionItemArray[i]==pMissionItem)
		{
			//删除数组
			m_MissionItemArray.RemoveAt(i);

			//设置子项
			pMissionItem->m_bActiveStatus=false;
			pMissionItem->m_pMissionManager=NULL;

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//激活任务
bool CMissionManager::AvtiveMissionItem(CMissionItem * pMissionItem, bool bContinue)
{
	if(this == NULL)
	{

		OutputDebugString(L"SOCKETSTATUS AvtiveMissionItem this == NULL");

		return false;
	}
	//效验状态
	ASSERT(pMissionItem->m_pMissionManager==this);
	if (pMissionItem->m_pMissionManager!=this)
	{
		OutputDebugString(L"SOCKETSTATUS AvtiveMissionItem m_pMissionManager!=this");
		return false;
	}

	//关闭窗口
	if (m_hWnd!=NULL) DestroyWindow();

	//设置任务
	pMissionItem->m_bActiveStatus=true;

	//连接处理
	switch (m_MissionSocket.GetSocketStatus())
	{
	case SOCKET_STATUS_IDLE:	//空闲状态
		{
			if(GetSafeHwnd())
				SetTimer(IDI_CANCEL_LINK,10*1000,NULL);
			//发起连接
			if (m_MissionSocket.PerformConnect(bContinue)==false)
			{
				KillTimer(IDI_CANCEL_LINK);
				//设置任务
				pMissionItem->m_bActiveStatus=false;

				OutputDebugString(L"SOCKETSTATUS AvtiveMissionItem PerformConnect==false");

				return false;
			}

			break;
		}
	case SOCKET_STATUS_CONNECT:	//连接状态
		{
			if(pMissionItem!=NULL)
			{
				//事件通知
				pMissionItem->OnEventMissionLink(0L, pMissionItem->m_nSocketID);

			}

			break;
		}
	}
	return true;
}

//终止任务
bool CMissionManager::ConcludeMissionItem(CMissionItem * pMissionItem, bool bDeferIntermit)
{
	if (pMissionItem->m_bActiveStatus==false) return false;
	//效验状态
	ASSERT(pMissionItem->m_bActiveStatus==true);

	//设置任务
	pMissionItem->m_bActiveStatus=false;

	//变量定义
	UINT uActiveCount=0;
	BYTE cbSocketStatus=m_MissionSocket.GetSocketStatus();

	//搜索子项
	for (INT_PTR i=0;i<m_MissionItemArray.GetCount();i++)
	{
		CMissionItem * pMissionItem=m_MissionItemArray[i];
		if (pMissionItem->m_bActiveStatus==true) uActiveCount++;
	}

	//终止连接
	if ((uActiveCount==0)&&(cbSocketStatus!=SOCKET_STATUS_IDLE))
	{
		//变量定义
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//关闭判断
		if ((bDeferIntermit==true)&&(pParameterGlobal->m_wIntermitTime>0))
		{
			//创建窗口
			if (m_hWnd==NULL)
			{
				CRect rcCreate(0,0,0,0);
				Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
			}

			//设置时间
			SetTimer(IDI_CLOSE_LINK,pParameterGlobal->m_wIntermitTime*1000L,NULL);
		}
		else
		{
			//关闭连接
			m_MissionSocket.PerformClose();
		}
	}

	return false;
}

//发送函数
bool CMissionManager::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	bool bSuccess = true;
	//发送数据
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

//发送函数
bool CMissionManager::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)
{
	bool bSuccess = true;

	//发送数据
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

//时间消息
VOID CMissionManager::OnTimer(UINT nIDEvent)
{
	//关闭连接
	if (nIDEvent==IDI_CLOSE_LINK)
	{
		//关闭窗口
		DestroyWindow();

		//终止连接
		m_MissionSocket.PerformClose();

		return;
	}
	else if(nIDEvent == IDI_CANCEL_LINK)
	{
	//	ConcludeMissionItem()
		//关闭窗口
		DestroyWindow();

		//终止连接
		m_MissionSocket.PerformClose();
		KillTimer(IDI_CANCEL_LINK);
		OnEventMissionLink(10049,0);
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////