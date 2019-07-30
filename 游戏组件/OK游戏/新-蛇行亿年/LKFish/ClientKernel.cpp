#include "stdafx.h"
#include "ClientKernel.h"
#include <tchar.h>


//////////////////////////////////////////////////////////////////////////
CClientKernel *CClientKernel::m_ClientKernel = 0;

std::string g_WideCharToMultiByte(WCHAR* wstr);
//////////////////////////////////////////////////////////////////////////

//构造函数
CClientKernel::CClientKernel()
{
    m_dwUserID=0L;
    m_wTableID=INVALID_TABLE;
    m_wChairID=INVALID_CHAIR;

    //房间信息
    memset(&m_ServerAttribute,0,sizeof(m_ServerAttribute));

    //游戏信息
    m_pMeUserItem=NULL;
	m_bGameStatus=GAME_STATUS_FREE;
    for (int i=0; i<MAX_CHAIR; i++)
        m_pUserItem[i] = NULL;

    //通讯变量
    m_hShareMemory=NULL;
    //m_pShareMemory=NULL;

    m_hWndGameFrame = NULL;
    m_pIClientKernelSink = NULL;

    m_hWndChannel = NULL;
    m_hWndGameServer = NULL;

    m_ClientKernel = this;

	m_IPCBuffers.clear();

    return;
}

//析构函数
CClientKernel::~CClientKernel(void)
{
    DestroyClientKernel();

    return;
}

//设置接口
bool  CClientKernel::DestroyClientKernel()
{
    //SendData(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_CLIENT_CLOSE);

    //关闭窗口
    if (m_hWndChannel!=NULL)
    {
        DestroyWindow(m_hWndChannel);
        m_hWndChannel = NULL;
    }

    //设置接口
    m_ClientKernel = NULL;
    m_pIClientKernelSink=NULL;

    std::deque<IPC_Buffer *>::iterator i=m_IPCBuffers.begin();
    for (; i!=m_IPCBuffers.end(); ++i)
    {
        delete (*i);
    }
    m_IPCBuffers.clear();

    for (int i=0; i<MAX_CHAIR; i++)
    {
        if (m_pUserItem[i] != NULL)
        {
            delete m_pUserItem[i];
            m_pUserItem[i] = NULL;
        }
    }

   if (m_hShareMemory!=NULL)
    {
        CloseHandle(m_hShareMemory);
        m_hShareMemory=NULL;
    }

    return true;
}

//查询命令
bool CClientKernel::SearchCommandItem(LPCWSTR pszCommandLine, LPCWSTR pszCommand, std::wstring &szParameter)
{
	//效验参数
	ASSERT((pszCommandLine!=NULL)&&(pszCommand!=NULL));
	if ((pszCommandLine==NULL)||(pszCommand==NULL)) return false;

	//参数处理
	if (pszCommandLine[0]!=0)
	{
		//变量定义
		UINT nCommandLen=_tcslen(pszCommand);
		LPCWSTR lpszBeginString=pszCommandLine;
		
		//提取参数
		while (true)
		{
			//参数分析
			LPCWSTR lpszEndString=_tcschr(lpszBeginString,' ');
			UINT nStringLength=(lpszEndString==NULL)?_tcslen(lpszBeginString):(UINT)(lpszEndString-lpszBeginString);

			//命令分析
			if (nStringLength>=nCommandLen)
			{
				//提取参数
				std::wstring strLine = lpszBeginString;
				szParameter = strLine.substr(nCommandLen,nStringLength-nCommandLen);

				return true;
			}

			//设置变量
			if (lpszEndString==NULL) break;
			lpszBeginString=(lpszEndString+1);
		}
	}

	return false;
}

//内核配置
bool  CClientKernel::InitClientKernel(HWND hWnd, LPCWSTR CmdLine, CClientKernelSink* pIUnknownEx)
{
	// 设置游戏属性
	_sntprintf((wchar_t *)m_GameAttribute.szGameName,sizeof(m_GameAttribute.szGameName),L"%s",TEXT("金蟾捕鱼"));
	m_GameAttribute.wChairCount=6;
	m_GameAttribute.wKindID=200;
	m_GameAttribute.dwClientVersion=PROCESS_VERSION(6,0,3);

    if (_tcslen(CmdLine)==0) return false;

    //获取框架
    m_hWndGameFrame=hWnd;
    m_pIClientKernelSink=pIUnknownEx;

	
    //创建窗口
    if (m_hWndChannel==NULL) 
    {
		TCHAR lpClassName[32];
		_sntprintf((wchar_t *)lpClassName,sizeof(lpClassName),_T("%s"),_T("ChannelWND"));
        WNDCLASS wc = { CS_HREDRAW | CS_VREDRAW, 
            CClientKernel::ChannelWndProc, 0, 0, GetModuleHandle(0),
            LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
            (HBRUSH)GetStockObject(BLACK_BRUSH), 0, lpClassName};
        RegisterClass(&wc);

        //创建窗口
        m_hWndChannel = CreateWindowEx(0, lpClassName,lpClassName,WS_CHILD,0,0,0,0,GetDesktopWindow(),0,GetModuleHandle(0),0);
    }

    //命令行处理
	if (_tcslen(CmdLine)!=0)
    {
		//变量定义
		std::wstring szTransmittal;

		if(SearchCommandItem(CmdLine,L"/Transmittal:",szTransmittal)==true)
		{
			//设置参数
			m_hWndGameServer=(HWND)(_wtoi64(szTransmittal.c_str()));
          
			//效验状态
			ASSERT(IsWindow(m_hWndGameServer));
			if (IsWindow(m_hWndGameServer)==FALSE) m_hWndGameServer=NULL;

			//发送消息
			SendData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLIENT_READY);
		}
    }

    //更新标题
    UpdateGameTitle();
   
    return true;
}

//获取玩家
tagUserInfo *  CClientKernel::GetUserInfo(WORD wChairID)
{
    if (wChairID>=MAX_CHAIR) return NULL;
    return m_pUserItem[wChairID];
}

bool CClientKernel::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	if(IsWindow(m_hWndGameServer)==FALSE) return false;

	//构造数据
	IPC_Head IPCHead;
	IPCHead.wVersion=IPC_VER;
	IPCHead.wSubCmdID=wSubCmdID;
	IPCHead.wMainCmdID=wMainCmdID;
	IPCHead.wPacketSize=sizeof(IPC_Head);

	//发送数据
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCHead;
	CopyDataStruct.dwData=IDT_ASYNCHRONISM;
	CopyDataStruct.cbData=IPCHead.wPacketSize;
	LRESULT lResult = ::SendMessage(m_hWndGameServer,WM_COPYDATA,(LPARAM)m_hWndChannel,(WPARAM)&CopyDataStruct);

    return true;
}

//发送数据
bool CClientKernel::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (IsWindow(m_hWndGameServer)==FALSE) return false;

	//构造数据
	IPC_Buffer IPCBuffer;
	IPCBuffer.Head.wVersion=IPC_VER;
	IPCBuffer.Head.wSubCmdID=wSubCmdID;
	IPCBuffer.Head.wMainCmdID=wMainCmdID;
	IPCBuffer.Head.wPacketSize=sizeof(IPCBuffer.Head);

	//附加数据
	if (pBuffer!=NULL)
	{
		//效验长度
		ASSERT(wDataSize<=sizeof(IPCBuffer.cbBuffer));
		if (wDataSize>sizeof(IPCBuffer.cbBuffer)) return false;

		//拷贝数据
		IPCBuffer.Head.wPacketSize+=wDataSize;
		CopyMemory(IPCBuffer.cbBuffer,pBuffer,wDataSize);
	}

	//发送数据
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCBuffer;
	CopyDataStruct.dwData=IDT_ASYNCHRONISM;
	CopyDataStruct.cbData=IPCBuffer.Head.wPacketSize;
	LRESULT lResult = ::SendMessage(m_hWndGameServer,WM_COPYDATA,(LPARAM)m_hWndChannel,(WPARAM)&CopyDataStruct);
    return true;
}

//发送函数
bool CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID)
{
	//效验状态
	if (m_hWndGameServer==NULL) return false;

	//变量定义
	IPC_GF_SocketSend SocketSend;
	ZeroMemory(&SocketSend,sizeof(SocketSend));

	//构造数据
	SocketSend.CommandInfo.wSubCmdID=wSubCmdID;
	SocketSend.CommandInfo.wMainCmdID=wMainCmdID;

	//发送数据
	WORD wSendSize=sizeof(SocketSend)-sizeof(SocketSend.cbBuffer);
	return SendData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_SEND,&SocketSend,wSendSize);
 }

//发送函数
bool  CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;

	//效验状态
	if (m_hWndGameServer==NULL) return false;

	//变量定义
	IPC_GF_SocketSend SocketSend;
	ZeroMemory(&SocketSend,sizeof(SocketSend));

	//构造数据
	SocketSend.CommandInfo.wSubCmdID=wSubCmdID;
	SocketSend.CommandInfo.wMainCmdID=wMainCmdID;

	//叠加数据
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketSend.cbBuffer,pData,wDataSize);
	}

	//发送数据
	WORD wSendSize=sizeof(SocketSend)-sizeof(SocketSend.cbBuffer)+wDataSize;
	SendData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_SEND,&SocketSend,wSendSize);

	return true;
}

//发送准备
bool  CClientKernel::SendUserReady(void * pBuffer, WORD wDataSize)
{
    return SendSocketData(MDM_GF_FRAME,SUB_GF_USER_READY,pBuffer,wDataSize);
}

////发送聊天
bool  CClientKernel::SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)
{
    //效验状态
    if (m_pMeUserItem==NULL) return false;

    //构造数据
    CMD_GF_C_UserChat UserChat;
    UserChat.dwChatColor=crFontColor;
    UserChat.dwTargetUserID=dwTargetUserID;
    //UserChat.dwSendUserID=m_pMeUserItem->dwUserID;
    lstrcpyn(UserChat.szChatString,pszChatMessage,LEN_USER_CHAT);
    UserChat.wChatLength=lstrlen(UserChat.szChatString);

    //发送命令
    WORD wSendSize=sizeof(UserChat)-sizeof(UserChat.szChatString)+UserChat.wChatLength;
    SendSocketData(MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);

    return true;
}

//信道数据处理
bool  CClientKernel::OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	
    //特殊处理
    if (m_pIClientKernelSink==NULL) return true;

    //默认处理
    switch (pHead->wMainCmdID)
    {
    case IPC_CMD_GF_SOCKET:			//网络数据
        {

            bool bSuccess=OnIPCSocket(pHead,pIPCBuffer,wDataSize,hWndSend);
            if (bSuccess==false)
            {

                if (m_pIClientKernelSink)
                    m_pIClientKernelSink->CloseGameFrame();
            }
            return true;
        }
    case IPC_CMD_GF_CONFIG:			//配置信息
        {
            return OnIPCConfig(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_CMD_GF_USER_INFO:		//用户信息
        {

            return OnIPCUser(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_CMD_GF_CONTROL:		//控制消息
        {
            return OnIPCControl(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    }

    return false;
}

//网络消息
bool CClientKernel::OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{

    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_GF_SOCKET_RECV:	//数据接收
        {
			//效验数据
			ASSERT(wDataSize>=sizeof(TCP_Command));
			if (wDataSize<sizeof(TCP_Command)) return false;

            //提取数据
            WORD wPacketSize=wDataSize-sizeof(TCP_Command);
            IPC_GF_SocketRecv * pSocketRecv=(IPC_GF_SocketRecv *)pIPCBuffer;

            //特殊处理
            if (m_pIClientKernelSink==NULL) return true;

            bool bHandle=m_pIClientKernelSink->OnEventSocket(pSocketRecv->CommandInfo,pSocketRecv->cbBuffer,wPacketSize);

            //框架消息
            if ((bHandle==false)&&(pSocketRecv->CommandInfo.wMainCmdID==MDM_GF_FRAME))
            {
                switch (pSocketRecv->CommandInfo.wSubCmdID)
                {
				case SUB_GF_USER_CHAT:			//用户聊天
					{
						return true;
					}
				case SUB_GF_USER_EXPRESSION:	//用户表情
					{
						return true;
					}
				case SUB_GF_GAME_STATUS:		//游戏状态
					{
						//效验参数
						ASSERT(wPacketSize==sizeof(CMD_GF_GameStatus));
						if (wPacketSize!=sizeof(CMD_GF_GameStatus)) return false;

						//消息处理
						CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)pSocketRecv->cbBuffer;

						//设置变量
						m_bGameStatus=pGameStatus->cbGameStatus;

						return true;
					}
				case SUB_GF_GAME_SCENE:			//游戏场景
					{
						
						return m_pIClientKernelSink->OnEventGameScene(m_bGameStatus,false,pSocketRecv->cbBuffer,wPacketSize);
					}
                case SUB_GF_LOOKON_STATUS:		//旁观状态
                    {
                        return true;
                    }
                case SUB_GF_ACTION_MESSAGE:		//聊天信息
                    {
                        return true;
                    }
                case SUB_GF_SYSTEM_MESSAGE:		//系统消息
                    {
                        //效验参数
                        CMD_CM_SystemMessage * pMessage=(CMD_CM_SystemMessage *)pSocketRecv->cbBuffer;
                        if (wDataSize<=(sizeof(CMD_CM_SystemMessage)-sizeof(pMessage->szString))) return false;

                        //关闭房间
                        if (pMessage->wType&SMT_CLOSE_GAME)
                        {
                            m_pIClientKernelSink->CloseGameFrame();
                        }
						else
						{
							//AfxMessageBox(pMessage->szString);
							//TraceString(pMessage->szString);
							//return m_pIClientKernelSink->OnEventGameScene(m_bGameStatus,false,pSocketRecv->cbBuffer,wPacketSize);
							//m_pIClientKernelSink->OnEventSocket(pSocketRecv->CommandInfo,pSocketRecv->cbBuffer,wPacketSize);
						}
                        return true;
                    }
				case SUB_GF_USER_READY:			//用户准备
					{
						if(m_pMeUserItem==NULL || m_pMeUserItem->cbUserStatus>=US_READY)
							return true;

						SendUserReady(NULL,0);
						return true;
					}
                }
            }

            return true;
        }     
    }

    return true;
}

//配置信息
bool CClientKernel::OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{

    switch (pHead->wSubCmdID)
    {
	case IPC_SUB_GF_LEVEL_INFO:		//等级信息
		{
			return true;
		}
	case IPC_SUB_GF_COLUMN_INFO:	//列表信息
		{
			return true;
		}
    case IPC_SUB_GF_SERVER_INFO:	//房间信息
        {
			////效验参数
			//ASSERT(wDataSize==sizeof(IPC_GF_ServerInfo));
			//if (wDataSize!=sizeof(IPC_GF_ServerInfo)) return false;

			//变量定义
			IPC_GF_ServerInfo * pServerInfo=(IPC_GF_ServerInfo *)pIPCBuffer;

            m_wTableID=pServerInfo->wTableID;
            m_wChairID=pServerInfo->wChairID;
            m_dwUserID=pServerInfo->dwUserID;

            m_ServerAttribute.wKindID=pServerInfo->wKindID;
            m_ServerAttribute.wServerID=pServerInfo->wServerID;
            m_ServerAttribute.wServerType=pServerInfo->wServerType;
			m_ServerAttribute.dwServerRule=pServerInfo->dwServerRule;
// 			m_ServerAttribute.dwAVServerAddr=pServerInfo->dwAVServerAddr;
// 			m_ServerAttribute.wAVServerPort=pServerInfo->wAVServerPort;
 
			ZeroMemory(m_ServerAttribute.szServerName,sizeof(m_ServerAttribute.szServerName));
            lstrcpyn(m_ServerAttribute.szServerName,pServerInfo->szServerName,CountArray(m_ServerAttribute.szServerName));

            //更新标题
            UpdateGameTitle();

            return true;
        }
// 	case IPC_SUB_GF_PROPERTY_INFO:	//道具信息
// 		{
// 			return true;
// 		}
	case IPC_SUB_GF_CONFIG_FINISH:	//配置完成
		{
			//变量定义
			CMD_GF_GameOption GameOption;
			ZeroMemory(&GameOption,sizeof(GameOption));

			//构造数据
			GameOption.dwFrameVersion=VERSION_FRAME;
			GameOption.cbAllowLookon=false;
			GameOption.dwClientVersion=m_GameAttribute.dwClientVersion;

			//发送数据
			SendSocketData(MDM_GF_FRAME,SUB_GF_GAME_OPTION,&GameOption,sizeof(GameOption));

			return true;
		}
// 	case IPC_SUB_GF_USER_RIGHT:		//玩家权限
// 		{
// 			return true;
// 		}
    }
    return true;
}

//用户通知
bool CClientKernel::OnIPCUser(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_GF_USER_ENTER:		//用户消息
        {
			//效验参数
			ASSERT(wDataSize>=sizeof(IPC_GF_UserInfo));
			if (wDataSize<sizeof(IPC_GF_UserInfo)) return false;

            //读取基本信息
			tagUserInfo *pUserData = new tagUserInfo;

			ZeroMemory(pUserData,sizeof(tagUserInfo));

			//消息处理
			IPC_GF_UserInfo * pIPCUserInfo=(IPC_GF_UserInfo *)pIPCBuffer;
			tagUserInfoHead * pUserInfoHead=&pIPCUserInfo->UserInfoHead;

			//变量定义
			TCHAR szUserNote[LEN_USERNOTE]=TEXT("");

			//变量定义
// 			tagCustomFaceInfo CustomFaceInfo;
// 			ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

			//用户属性
			pUserData->wFaceID=pUserInfoHead->wFaceID;
//			pUserData->dwGameID=pUserInfoHead->dwGameID;
			pUserData->dwUserID=pUserInfoHead->dwUserID;
//			pUserData->dwGroupID=pUserInfoHead->dwGroupID;
///			pUserData->dwCustomID=pUserInfoHead->dwCustomID;

			//用户属性
			pUserData->cbGender=pUserInfoHead->cbGender;
			pUserData->cbMemberOrder=pUserInfoHead->cbMemberOrder;
			pUserData->cbMasterOrder=pUserInfoHead->cbMasterOrder;

			//用户状态
			pUserData->wTableID=pUserInfoHead->wTableID;
			pUserData->wChairID=pUserInfoHead->wChairID;
			pUserData->cbUserStatus=pUserInfoHead->cbUserStatus;

			//用户积分
			pUserData->lScore=pUserInfoHead->lScore;
//			pUserData->lGrade=pUserInfoHead->lGrade;
			pUserData->lInsure=pUserInfoHead->lInsure;
			pUserData->dwWinCount=pUserInfoHead->dwWinCount;
			pUserData->dwLostCount=pUserInfoHead->dwLostCount;
			pUserData->dwDrawCount=pUserInfoHead->dwDrawCount;
			pUserData->dwFleeCount=pUserInfoHead->dwFleeCount;
//			pUserData->dwUserMedal=pUserInfoHead->dwUserMedal;
			pUserData->dwExperience=pUserInfoHead->dwExperience;
//			pUserData->lLoveLiness=pUserInfoHead->lLoveLiness;

			//变量定义
			VOID * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pIPCUserInfo+1,wDataSize-sizeof(IPC_GF_UserInfo));

			//扩展信息
			while (true)
			{
				//提取数据
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;

				//设置数据
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_GR_NICK_NAME:		//用户昵称
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(pUserData->szNickName));
						if (DataDescribe.wDataSize<=sizeof(pUserData->szNickName))
						{
							CopyMemory(&pUserData->szNickName,pDataBuffer,DataDescribe.wDataSize);
							pUserData->szNickName[CountArray(pUserData->szNickName)-1]=0;
						}
						break;
					}
// 				case DTP_GR_GROUP_NAME:		//用户社团
// 					{
// 						ASSERT(pDataBuffer!=NULL);
// 						ASSERT(DataDescribe.wDataSize<=sizeof(pUserData->szGroupName));
// 						if (DataDescribe.wDataSize<=sizeof(pUserData->szGroupName))
// 						{
// 							CopyMemory(&pUserData->szGroupName,pDataBuffer,DataDescribe.wDataSize);
// 							pUserData->szGroupName[CountArray(pUserData->szGroupName)-1]=0;
// 						}
// 						break;
// 					}
// 				case DTP_GR_UNDER_WRITE:	//个性签名
// 					{
// 						ASSERT(pDataBuffer!=NULL);
// 						ASSERT(DataDescribe.wDataSize<=sizeof(pUserData->szUnderWrite));
// 						if (DataDescribe.wDataSize<=sizeof(pUserData->szUnderWrite))
// 						{
// 							CopyMemory(pUserData->szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
// 							pUserData->szUnderWrite[CountArray(pUserData->szUnderWrite)-1]=0;
// 						}
// 						break;
// 					}
				case DTP_GR_USER_NOTE:			//用户备注
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(szUserNote));
						if (DataDescribe.wDataSize<=sizeof(szUserNote))
						{
							CopyMemory(szUserNote,pDataBuffer,DataDescribe.wDataSize);
							szUserNote[CountArray(szUserNote)-1]=0;
						}
						break;
					}
// 				case DTP_GR_CUSTOM_FACE:		//自定头像
// 					{
// 						ASSERT(pDataBuffer!=NULL);
// 						ASSERT(DataDescribe.wDataSize==sizeof(tagCustomFaceInfo));
// 						if (DataDescribe.wDataSize==sizeof(tagCustomFaceInfo))
// 						{
// 							CopyMemory(&CustomFaceInfo,pDataBuffer,DataDescribe.wDataSize);
// 						}
// 						break;
// 					}
 				}
			}

            //增加用户
            ActiveUserItem(pUserData);

            return true;
        }
	case IPC_SUB_GF_USER_SCORE:		//用户积分
		{
			//效验参数
			ASSERT(wDataSize==sizeof(IPC_GF_UserScore));
			if (wDataSize!=sizeof(IPC_GF_UserScore)) return false;

			//寻找用户
			IPC_GF_UserScore * pUserScore=(IPC_GF_UserScore *)pIPCBuffer;

			UpdateUserItem(pUserScore->dwUserID,&pUserScore->UserScore);

			return true;
		}
    case IPC_SUB_GF_USER_STATUS:	//用户状态
        {
			//效验参数
			ASSERT(wDataSize==sizeof(IPC_GF_UserStatus));
			if (wDataSize!=sizeof(IPC_GF_UserStatus)) return false;

			//寻找用户
			IPC_GF_UserStatus * pUserStatus=(IPC_GF_UserStatus *)pIPCBuffer;

			if (pUserStatus->UserStatus.cbUserStatus==US_FREE) 
            {
                if (pUserStatus->dwUserID==m_dwUserID)
                {
                    ResetClientKernel();
                    return true;
                }
                else           
				{
					DeleteUserItem(pUserStatus->dwUserID);
				}
            }
            else 
			{
				UpdateUserItem(pUserStatus->dwUserID,&pUserStatus->UserStatus,0);
			}

            return true;
        }
// 	case IPC_SUB_GF_USER_ATTRIB:	//用户属性
// 		{
// 			//效验参数
// 			ASSERT(wDataSize==sizeof(IPC_GF_UserAttrib));
// 			if (wDataSize!=sizeof(IPC_GF_UserAttrib)) return false;
// 
// 			//寻找用户
// 			IPC_GF_UserAttrib * pUserAttrib=(IPC_GF_UserAttrib *)pIPCBuffer;
// 
// 			return true;
// 		}
// 	case IPC_SUB_GF_CUSTOM_FACE:
// 		{
// 			//效验参数
// 			ASSERT(wDataSize==sizeof(IPC_GF_CustomFace));
// 			if (wDataSize!=sizeof(IPC_GF_CustomFace)) return false;
// 
// 			//寻找用户
// 			IPC_GF_CustomFace * pCustomFace=(IPC_GF_CustomFace *)pIPCBuffer;
// 
// 			return true;
// 		}
    }

    return false;
}

//控制通知
bool CClientKernel::OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_GF_ACTIVE_PROCESS:		//启动完成
        {
			if(m_hWndGameFrame==NULL) return false;

			ShowWindow(m_hWndGameFrame,SW_SHOW);
			BringWindowToTop(m_hWndGameFrame);
			SetForegroundWindow(m_hWndGameFrame);

            return true;
        }
    case IPC_SUB_GF_CLOSE_PROCESS:		//关闭框架
        {
            if (m_pIClientKernelSink!=NULL)
                m_pIClientKernelSink->CloseGameFrame();

            return true;
        }
    }

    return false;
}

tagUserInfo *  CClientKernel::SearchUserItem(DWORD dwUserID)
{
    for (int i=0; i<MAX_CHAIR; i++)
    {

        if (m_pUserItem[i] && m_pUserItem[i]->dwUserID == dwUserID)
            return m_pUserItem[i];
    }

    return NULL;
}

//增加用户
bool CClientKernel::ActiveUserItem(tagUserInfo * pUserData)
{
    //通知改变
    m_pUserItem[pUserData->wChairID] = pUserData;

    if (m_dwUserID==pUserData->dwUserID)
    {
        m_pMeUserItem=m_pUserItem[pUserData->wChairID];
        m_wTableID=m_pMeUserItem->wTableID;
        m_wChairID=m_pMeUserItem->wChairID;
    }	

    if (m_pIClientKernelSink!=NULL)
        m_pIClientKernelSink->OnEventUserEnter(m_pUserItem[pUserData->wChairID], pUserData->wChairID, false);

    return true;
}

//删除用户
bool CClientKernel::DeleteUserItem(DWORD dwUserID)
{
    //游戏用户
	tagUserInfo * pUserData=SearchUserItem(dwUserID);
    if(pUserData==NULL)return true;
    if (m_pIClientKernelSink!=NULL) 
        m_pIClientKernelSink->OnEventUserLeave(pUserData,pUserData->wChairID,false);


    int wChairID = pUserData->wChairID ;
    delete m_pUserItem[wChairID];
    m_pUserItem[wChairID] = NULL;

    return true;
}

//更新用户
bool CClientKernel::UpdateUserItem(DWORD dwUserID, const tagUserScore * pUserScore)
{
    //寻找用户
	tagUserInfo * pUserInfo=SearchUserItem(dwUserID);
    if (pUserInfo==NULL) return false;

	//设置数据
	pUserInfo->lScore=pUserScore->lScore;
////	pUserInfo->lGrade=pUserScore->lGrade;
	pUserInfo->lInsure=pUserScore->lInsure;
	pUserInfo->dwWinCount=pUserScore->dwWinCount;
	pUserInfo->dwLostCount=pUserScore->dwLostCount;
	pUserInfo->dwDrawCount=pUserScore->dwDrawCount;
	pUserInfo->dwFleeCount=pUserScore->dwFleeCount;
//	pUserInfo->dwUserMedal=pUserScore->dwUserMedal;
	pUserInfo->dwExperience=pUserScore->dwExperience;
//	pUserInfo->lLoveLiness=pUserScore->lLoveLiness;

    //通知改变
    if (m_pIClientKernelSink!=NULL)
		m_pIClientKernelSink->OnEventUserScore(pUserInfo,pUserInfo->wChairID,false);

    return true;
}

//更新用户
bool CClientKernel::UpdateUserItem(DWORD dwUserID, tagUserStatus * pUserStatus, WORD wNetDelay)
{
    //寻找用户
	tagUserInfo * pUserInfo=SearchUserItem(dwUserID);
    if (pUserInfo==NULL) return false;

	//设置数据
	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

    //通知改变
	bool bLookonUser=(pUserStatus->cbUserStatus==US_LOOKON);

    if (m_pIClientKernelSink!=NULL) 
        m_pIClientKernelSink->OnEventUserStatus(pUserInfo,pUserInfo->wChairID,bLookonUser);

    return false;
}

//更新标题
void CClientKernel::UpdateGameTitle()
{
//     //变量定义
//     wchar_t szTitle[256];
// 
//     //获取文件
//     wchar_t szProcessName[MAX_PATH];
// 
// 	_sntprintf(szProcessName,CountArray(szProcessName),_T("%s"),_T("深海捕鱼"));

//     //构造标题
//     if ((m_ServerAttribute.szServerName[0]!=0)&&(m_wTableID!=INVALID_TABLE))
//     {
//         _sntprintf(szTitle,CountArray(szTitle),_T("%s ［ %s - 第 %d 桌 ］"),szProcessName,m_ServerAttribute.szServerName,m_wTableID+1);
//     }
//     else
//     {
//         _sntprintf(szTitle,CountArray(szTitle),_T("%s"),szProcessName);
//     }

//#ifndef _DEBUG
//	SetWindowText(Root::instance()->render_window()->window_handle(),g_WideCharToMultiByte(szTitle).c_str());
//#else
	//SetWindowText(Root::instance()->render_window()->window_handle(),szTitle);
//#endif

    return;
}

//内核重置
void CClientKernel::ResetClientKernel()
{
    //设置变量
    m_pMeUserItem=NULL;
    m_bGameStatus=GAME_STATUS_FREE;
    m_wTableID=INVALID_TABLE;
    m_wChairID=INVALID_CHAIR;

    //清理界面
    if (m_pIClientKernelSink!=NULL)
        m_pIClientKernelSink->ResetGameFrame();

    return;
}

LRESULT CClientKernel::ChannelWndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_COPYDATA:
        if (CClientKernel::m_ClientKernel)
		{
			CClientKernel::m_ClientKernel->OnCopyData(HWND(wParam),(COPYDATASTRUCT *) lParam);
		}
        return 0;
    case WM_IPC_MESSAGE:
        if (CClientKernel::m_ClientKernel)
		{
			CClientKernel::m_ClientKernel->OnIPCMessage(wParam,lParam);
		}
        return 0;
	case WM_CLOSE:
		ASSERT(0);
        if (CClientKernel::m_ClientKernel)
		{
			CClientKernel::m_ClientKernel->SendSocketData(MDM_GF_GAME,216);
		}
		return 0;
	case WM_NCLBUTTONDOWN:
		return 0;
    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

bool CClientKernel::OnCopyData(HWND hWnd, COPYDATASTRUCT * pCopyDataStruct)
{
    //效验数据
    if (pCopyDataStruct->cbData<sizeof(IPC_Head)) return false;

    //提取数据
    IPC_Buffer *pIPCBuffer = new IPC_Buffer;
    CopyMemory(pIPCBuffer,pCopyDataStruct->lpData,pCopyDataStruct->cbData);

    if (pIPCBuffer->Head.wVersion!=IPC_VER) return false;
    if (pIPCBuffer->Head.wPacketSize!=pCopyDataStruct->cbData) return false;

    pIPCBuffer->Head.wPacketSize-=sizeof(IPC_Head);

    m_IPCBuffers.push_front(pIPCBuffer);

	PostMessage(m_hWndChannel,WM_IPC_MESSAGE,0,0);

    return true;
}

bool CClientKernel::OnIPCMessage(WPARAM wParam, LPARAM lParam)
{
    IPC_Buffer *pIPCBuffer = m_IPCBuffers.back();

    WORD wDataSize=pIPCBuffer->Head.wPacketSize;

    OnChannelMessage(&pIPCBuffer->Head,pIPCBuffer->cbBuffer,wDataSize,NULL);

    delete pIPCBuffer;
    m_IPCBuffers.pop_back();

    return true;
}

//////////////////////////////////////////////////////////////////////////
