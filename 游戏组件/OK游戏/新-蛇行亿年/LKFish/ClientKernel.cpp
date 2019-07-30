#include "stdafx.h"
#include "ClientKernel.h"
#include <tchar.h>


//////////////////////////////////////////////////////////////////////////
CClientKernel *CClientKernel::m_ClientKernel = 0;

std::string g_WideCharToMultiByte(WCHAR* wstr);
//////////////////////////////////////////////////////////////////////////

//���캯��
CClientKernel::CClientKernel()
{
    m_dwUserID=0L;
    m_wTableID=INVALID_TABLE;
    m_wChairID=INVALID_CHAIR;

    //������Ϣ
    memset(&m_ServerAttribute,0,sizeof(m_ServerAttribute));

    //��Ϸ��Ϣ
    m_pMeUserItem=NULL;
	m_bGameStatus=GAME_STATUS_FREE;
    for (int i=0; i<MAX_CHAIR; i++)
        m_pUserItem[i] = NULL;

    //ͨѶ����
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

//��������
CClientKernel::~CClientKernel(void)
{
    DestroyClientKernel();

    return;
}

//���ýӿ�
bool  CClientKernel::DestroyClientKernel()
{
    //SendData(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_CLIENT_CLOSE);

    //�رմ���
    if (m_hWndChannel!=NULL)
    {
        DestroyWindow(m_hWndChannel);
        m_hWndChannel = NULL;
    }

    //���ýӿ�
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

//��ѯ����
bool CClientKernel::SearchCommandItem(LPCWSTR pszCommandLine, LPCWSTR pszCommand, std::wstring &szParameter)
{
	//Ч�����
	ASSERT((pszCommandLine!=NULL)&&(pszCommand!=NULL));
	if ((pszCommandLine==NULL)||(pszCommand==NULL)) return false;

	//��������
	if (pszCommandLine[0]!=0)
	{
		//��������
		UINT nCommandLen=_tcslen(pszCommand);
		LPCWSTR lpszBeginString=pszCommandLine;
		
		//��ȡ����
		while (true)
		{
			//��������
			LPCWSTR lpszEndString=_tcschr(lpszBeginString,' ');
			UINT nStringLength=(lpszEndString==NULL)?_tcslen(lpszBeginString):(UINT)(lpszEndString-lpszBeginString);

			//�������
			if (nStringLength>=nCommandLen)
			{
				//��ȡ����
				std::wstring strLine = lpszBeginString;
				szParameter = strLine.substr(nCommandLen,nStringLength-nCommandLen);

				return true;
			}

			//���ñ���
			if (lpszEndString==NULL) break;
			lpszBeginString=(lpszEndString+1);
		}
	}

	return false;
}

//�ں�����
bool  CClientKernel::InitClientKernel(HWND hWnd, LPCWSTR CmdLine, CClientKernelSink* pIUnknownEx)
{
	// ������Ϸ����
	_sntprintf((wchar_t *)m_GameAttribute.szGameName,sizeof(m_GameAttribute.szGameName),L"%s",TEXT("��󸲶��"));
	m_GameAttribute.wChairCount=6;
	m_GameAttribute.wKindID=200;
	m_GameAttribute.dwClientVersion=PROCESS_VERSION(6,0,3);

    if (_tcslen(CmdLine)==0) return false;

    //��ȡ���
    m_hWndGameFrame=hWnd;
    m_pIClientKernelSink=pIUnknownEx;

	
    //��������
    if (m_hWndChannel==NULL) 
    {
		TCHAR lpClassName[32];
		_sntprintf((wchar_t *)lpClassName,sizeof(lpClassName),_T("%s"),_T("ChannelWND"));
        WNDCLASS wc = { CS_HREDRAW | CS_VREDRAW, 
            CClientKernel::ChannelWndProc, 0, 0, GetModuleHandle(0),
            LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
            (HBRUSH)GetStockObject(BLACK_BRUSH), 0, lpClassName};
        RegisterClass(&wc);

        //��������
        m_hWndChannel = CreateWindowEx(0, lpClassName,lpClassName,WS_CHILD,0,0,0,0,GetDesktopWindow(),0,GetModuleHandle(0),0);
    }

    //�����д���
	if (_tcslen(CmdLine)!=0)
    {
		//��������
		std::wstring szTransmittal;

		if(SearchCommandItem(CmdLine,L"/Transmittal:",szTransmittal)==true)
		{
			//���ò���
			m_hWndGameServer=(HWND)(_wtoi64(szTransmittal.c_str()));
          
			//Ч��״̬
			ASSERT(IsWindow(m_hWndGameServer));
			if (IsWindow(m_hWndGameServer)==FALSE) m_hWndGameServer=NULL;

			//������Ϣ
			SendData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLIENT_READY);
		}
    }

    //���±���
    UpdateGameTitle();
   
    return true;
}

//��ȡ���
tagUserInfo *  CClientKernel::GetUserInfo(WORD wChairID)
{
    if (wChairID>=MAX_CHAIR) return NULL;
    return m_pUserItem[wChairID];
}

bool CClientKernel::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	if(IsWindow(m_hWndGameServer)==FALSE) return false;

	//��������
	IPC_Head IPCHead;
	IPCHead.wVersion=IPC_VER;
	IPCHead.wSubCmdID=wSubCmdID;
	IPCHead.wMainCmdID=wMainCmdID;
	IPCHead.wPacketSize=sizeof(IPC_Head);

	//��������
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCHead;
	CopyDataStruct.dwData=IDT_ASYNCHRONISM;
	CopyDataStruct.cbData=IPCHead.wPacketSize;
	LRESULT lResult = ::SendMessage(m_hWndGameServer,WM_COPYDATA,(LPARAM)m_hWndChannel,(WPARAM)&CopyDataStruct);

    return true;
}

//��������
bool CClientKernel::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (IsWindow(m_hWndGameServer)==FALSE) return false;

	//��������
	IPC_Buffer IPCBuffer;
	IPCBuffer.Head.wVersion=IPC_VER;
	IPCBuffer.Head.wSubCmdID=wSubCmdID;
	IPCBuffer.Head.wMainCmdID=wMainCmdID;
	IPCBuffer.Head.wPacketSize=sizeof(IPCBuffer.Head);

	//��������
	if (pBuffer!=NULL)
	{
		//Ч�鳤��
		ASSERT(wDataSize<=sizeof(IPCBuffer.cbBuffer));
		if (wDataSize>sizeof(IPCBuffer.cbBuffer)) return false;

		//��������
		IPCBuffer.Head.wPacketSize+=wDataSize;
		CopyMemory(IPCBuffer.cbBuffer,pBuffer,wDataSize);
	}

	//��������
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCBuffer;
	CopyDataStruct.dwData=IDT_ASYNCHRONISM;
	CopyDataStruct.cbData=IPCBuffer.Head.wPacketSize;
	LRESULT lResult = ::SendMessage(m_hWndGameServer,WM_COPYDATA,(LPARAM)m_hWndChannel,(WPARAM)&CopyDataStruct);
    return true;
}

//���ͺ���
bool CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч��״̬
	if (m_hWndGameServer==NULL) return false;

	//��������
	IPC_GF_SocketSend SocketSend;
	ZeroMemory(&SocketSend,sizeof(SocketSend));

	//��������
	SocketSend.CommandInfo.wSubCmdID=wSubCmdID;
	SocketSend.CommandInfo.wMainCmdID=wMainCmdID;

	//��������
	WORD wSendSize=sizeof(SocketSend)-sizeof(SocketSend.cbBuffer);
	return SendData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_SEND,&SocketSend,wSendSize);
 }

//���ͺ���
bool  CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;

	//Ч��״̬
	if (m_hWndGameServer==NULL) return false;

	//��������
	IPC_GF_SocketSend SocketSend;
	ZeroMemory(&SocketSend,sizeof(SocketSend));

	//��������
	SocketSend.CommandInfo.wSubCmdID=wSubCmdID;
	SocketSend.CommandInfo.wMainCmdID=wMainCmdID;

	//��������
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketSend.cbBuffer,pData,wDataSize);
	}

	//��������
	WORD wSendSize=sizeof(SocketSend)-sizeof(SocketSend.cbBuffer)+wDataSize;
	SendData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_SEND,&SocketSend,wSendSize);

	return true;
}

//����׼��
bool  CClientKernel::SendUserReady(void * pBuffer, WORD wDataSize)
{
    return SendSocketData(MDM_GF_FRAME,SUB_GF_USER_READY,pBuffer,wDataSize);
}

////��������
bool  CClientKernel::SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)
{
    //Ч��״̬
    if (m_pMeUserItem==NULL) return false;

    //��������
    CMD_GF_C_UserChat UserChat;
    UserChat.dwChatColor=crFontColor;
    UserChat.dwTargetUserID=dwTargetUserID;
    //UserChat.dwSendUserID=m_pMeUserItem->dwUserID;
    lstrcpyn(UserChat.szChatString,pszChatMessage,LEN_USER_CHAT);
    UserChat.wChatLength=lstrlen(UserChat.szChatString);

    //��������
    WORD wSendSize=sizeof(UserChat)-sizeof(UserChat.szChatString)+UserChat.wChatLength;
    SendSocketData(MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);

    return true;
}

//�ŵ����ݴ���
bool  CClientKernel::OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	
    //���⴦��
    if (m_pIClientKernelSink==NULL) return true;

    //Ĭ�ϴ���
    switch (pHead->wMainCmdID)
    {
    case IPC_CMD_GF_SOCKET:			//��������
        {

            bool bSuccess=OnIPCSocket(pHead,pIPCBuffer,wDataSize,hWndSend);
            if (bSuccess==false)
            {

                if (m_pIClientKernelSink)
                    m_pIClientKernelSink->CloseGameFrame();
            }
            return true;
        }
    case IPC_CMD_GF_CONFIG:			//������Ϣ
        {
            return OnIPCConfig(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_CMD_GF_USER_INFO:		//�û���Ϣ
        {

            return OnIPCUser(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_CMD_GF_CONTROL:		//������Ϣ
        {
            return OnIPCControl(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    }

    return false;
}

//������Ϣ
bool CClientKernel::OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{

    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_GF_SOCKET_RECV:	//���ݽ���
        {
			//Ч������
			ASSERT(wDataSize>=sizeof(TCP_Command));
			if (wDataSize<sizeof(TCP_Command)) return false;

            //��ȡ����
            WORD wPacketSize=wDataSize-sizeof(TCP_Command);
            IPC_GF_SocketRecv * pSocketRecv=(IPC_GF_SocketRecv *)pIPCBuffer;

            //���⴦��
            if (m_pIClientKernelSink==NULL) return true;

            bool bHandle=m_pIClientKernelSink->OnEventSocket(pSocketRecv->CommandInfo,pSocketRecv->cbBuffer,wPacketSize);

            //�����Ϣ
            if ((bHandle==false)&&(pSocketRecv->CommandInfo.wMainCmdID==MDM_GF_FRAME))
            {
                switch (pSocketRecv->CommandInfo.wSubCmdID)
                {
				case SUB_GF_USER_CHAT:			//�û�����
					{
						return true;
					}
				case SUB_GF_USER_EXPRESSION:	//�û�����
					{
						return true;
					}
				case SUB_GF_GAME_STATUS:		//��Ϸ״̬
					{
						//Ч�����
						ASSERT(wPacketSize==sizeof(CMD_GF_GameStatus));
						if (wPacketSize!=sizeof(CMD_GF_GameStatus)) return false;

						//��Ϣ����
						CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)pSocketRecv->cbBuffer;

						//���ñ���
						m_bGameStatus=pGameStatus->cbGameStatus;

						return true;
					}
				case SUB_GF_GAME_SCENE:			//��Ϸ����
					{
						
						return m_pIClientKernelSink->OnEventGameScene(m_bGameStatus,false,pSocketRecv->cbBuffer,wPacketSize);
					}
                case SUB_GF_LOOKON_STATUS:		//�Թ�״̬
                    {
                        return true;
                    }
                case SUB_GF_ACTION_MESSAGE:		//������Ϣ
                    {
                        return true;
                    }
                case SUB_GF_SYSTEM_MESSAGE:		//ϵͳ��Ϣ
                    {
                        //Ч�����
                        CMD_CM_SystemMessage * pMessage=(CMD_CM_SystemMessage *)pSocketRecv->cbBuffer;
                        if (wDataSize<=(sizeof(CMD_CM_SystemMessage)-sizeof(pMessage->szString))) return false;

                        //�رշ���
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
				case SUB_GF_USER_READY:			//�û�׼��
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

//������Ϣ
bool CClientKernel::OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{

    switch (pHead->wSubCmdID)
    {
	case IPC_SUB_GF_LEVEL_INFO:		//�ȼ���Ϣ
		{
			return true;
		}
	case IPC_SUB_GF_COLUMN_INFO:	//�б���Ϣ
		{
			return true;
		}
    case IPC_SUB_GF_SERVER_INFO:	//������Ϣ
        {
			////Ч�����
			//ASSERT(wDataSize==sizeof(IPC_GF_ServerInfo));
			//if (wDataSize!=sizeof(IPC_GF_ServerInfo)) return false;

			//��������
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

            //���±���
            UpdateGameTitle();

            return true;
        }
// 	case IPC_SUB_GF_PROPERTY_INFO:	//������Ϣ
// 		{
// 			return true;
// 		}
	case IPC_SUB_GF_CONFIG_FINISH:	//�������
		{
			//��������
			CMD_GF_GameOption GameOption;
			ZeroMemory(&GameOption,sizeof(GameOption));

			//��������
			GameOption.dwFrameVersion=VERSION_FRAME;
			GameOption.cbAllowLookon=false;
			GameOption.dwClientVersion=m_GameAttribute.dwClientVersion;

			//��������
			SendSocketData(MDM_GF_FRAME,SUB_GF_GAME_OPTION,&GameOption,sizeof(GameOption));

			return true;
		}
// 	case IPC_SUB_GF_USER_RIGHT:		//���Ȩ��
// 		{
// 			return true;
// 		}
    }
    return true;
}

//�û�֪ͨ
bool CClientKernel::OnIPCUser(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_GF_USER_ENTER:		//�û���Ϣ
        {
			//Ч�����
			ASSERT(wDataSize>=sizeof(IPC_GF_UserInfo));
			if (wDataSize<sizeof(IPC_GF_UserInfo)) return false;

            //��ȡ������Ϣ
			tagUserInfo *pUserData = new tagUserInfo;

			ZeroMemory(pUserData,sizeof(tagUserInfo));

			//��Ϣ����
			IPC_GF_UserInfo * pIPCUserInfo=(IPC_GF_UserInfo *)pIPCBuffer;
			tagUserInfoHead * pUserInfoHead=&pIPCUserInfo->UserInfoHead;

			//��������
			TCHAR szUserNote[LEN_USERNOTE]=TEXT("");

			//��������
// 			tagCustomFaceInfo CustomFaceInfo;
// 			ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

			//�û�����
			pUserData->wFaceID=pUserInfoHead->wFaceID;
//			pUserData->dwGameID=pUserInfoHead->dwGameID;
			pUserData->dwUserID=pUserInfoHead->dwUserID;
//			pUserData->dwGroupID=pUserInfoHead->dwGroupID;
///			pUserData->dwCustomID=pUserInfoHead->dwCustomID;

			//�û�����
			pUserData->cbGender=pUserInfoHead->cbGender;
			pUserData->cbMemberOrder=pUserInfoHead->cbMemberOrder;
			pUserData->cbMasterOrder=pUserInfoHead->cbMasterOrder;

			//�û�״̬
			pUserData->wTableID=pUserInfoHead->wTableID;
			pUserData->wChairID=pUserInfoHead->wChairID;
			pUserData->cbUserStatus=pUserInfoHead->cbUserStatus;

			//�û�����
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

			//��������
			VOID * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pIPCUserInfo+1,wDataSize-sizeof(IPC_GF_UserInfo));

			//��չ��Ϣ
			while (true)
			{
				//��ȡ����
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;

				//��������
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_GR_NICK_NAME:		//�û��ǳ�
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
// 				case DTP_GR_GROUP_NAME:		//�û�����
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
// 				case DTP_GR_UNDER_WRITE:	//����ǩ��
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
				case DTP_GR_USER_NOTE:			//�û���ע
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
// 				case DTP_GR_CUSTOM_FACE:		//�Զ�ͷ��
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

            //�����û�
            ActiveUserItem(pUserData);

            return true;
        }
	case IPC_SUB_GF_USER_SCORE:		//�û�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GF_UserScore));
			if (wDataSize!=sizeof(IPC_GF_UserScore)) return false;

			//Ѱ���û�
			IPC_GF_UserScore * pUserScore=(IPC_GF_UserScore *)pIPCBuffer;

			UpdateUserItem(pUserScore->dwUserID,&pUserScore->UserScore);

			return true;
		}
    case IPC_SUB_GF_USER_STATUS:	//�û�״̬
        {
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GF_UserStatus));
			if (wDataSize!=sizeof(IPC_GF_UserStatus)) return false;

			//Ѱ���û�
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
// 	case IPC_SUB_GF_USER_ATTRIB:	//�û�����
// 		{
// 			//Ч�����
// 			ASSERT(wDataSize==sizeof(IPC_GF_UserAttrib));
// 			if (wDataSize!=sizeof(IPC_GF_UserAttrib)) return false;
// 
// 			//Ѱ���û�
// 			IPC_GF_UserAttrib * pUserAttrib=(IPC_GF_UserAttrib *)pIPCBuffer;
// 
// 			return true;
// 		}
// 	case IPC_SUB_GF_CUSTOM_FACE:
// 		{
// 			//Ч�����
// 			ASSERT(wDataSize==sizeof(IPC_GF_CustomFace));
// 			if (wDataSize!=sizeof(IPC_GF_CustomFace)) return false;
// 
// 			//Ѱ���û�
// 			IPC_GF_CustomFace * pCustomFace=(IPC_GF_CustomFace *)pIPCBuffer;
// 
// 			return true;
// 		}
    }

    return false;
}

//����֪ͨ
bool CClientKernel::OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_GF_ACTIVE_PROCESS:		//�������
        {
			if(m_hWndGameFrame==NULL) return false;

			ShowWindow(m_hWndGameFrame,SW_SHOW);
			BringWindowToTop(m_hWndGameFrame);
			SetForegroundWindow(m_hWndGameFrame);

            return true;
        }
    case IPC_SUB_GF_CLOSE_PROCESS:		//�رտ��
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

//�����û�
bool CClientKernel::ActiveUserItem(tagUserInfo * pUserData)
{
    //֪ͨ�ı�
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

//ɾ���û�
bool CClientKernel::DeleteUserItem(DWORD dwUserID)
{
    //��Ϸ�û�
	tagUserInfo * pUserData=SearchUserItem(dwUserID);
    if(pUserData==NULL)return true;
    if (m_pIClientKernelSink!=NULL) 
        m_pIClientKernelSink->OnEventUserLeave(pUserData,pUserData->wChairID,false);


    int wChairID = pUserData->wChairID ;
    delete m_pUserItem[wChairID];
    m_pUserItem[wChairID] = NULL;

    return true;
}

//�����û�
bool CClientKernel::UpdateUserItem(DWORD dwUserID, const tagUserScore * pUserScore)
{
    //Ѱ���û�
	tagUserInfo * pUserInfo=SearchUserItem(dwUserID);
    if (pUserInfo==NULL) return false;

	//��������
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

    //֪ͨ�ı�
    if (m_pIClientKernelSink!=NULL)
		m_pIClientKernelSink->OnEventUserScore(pUserInfo,pUserInfo->wChairID,false);

    return true;
}

//�����û�
bool CClientKernel::UpdateUserItem(DWORD dwUserID, tagUserStatus * pUserStatus, WORD wNetDelay)
{
    //Ѱ���û�
	tagUserInfo * pUserInfo=SearchUserItem(dwUserID);
    if (pUserInfo==NULL) return false;

	//��������
	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

    //֪ͨ�ı�
	bool bLookonUser=(pUserStatus->cbUserStatus==US_LOOKON);

    if (m_pIClientKernelSink!=NULL) 
        m_pIClientKernelSink->OnEventUserStatus(pUserInfo,pUserInfo->wChairID,bLookonUser);

    return false;
}

//���±���
void CClientKernel::UpdateGameTitle()
{
//     //��������
//     wchar_t szTitle[256];
// 
//     //��ȡ�ļ�
//     wchar_t szProcessName[MAX_PATH];
// 
// 	_sntprintf(szProcessName,CountArray(szProcessName),_T("%s"),_T("�����"));

//     //�������
//     if ((m_ServerAttribute.szServerName[0]!=0)&&(m_wTableID!=INVALID_TABLE))
//     {
//         _sntprintf(szTitle,CountArray(szTitle),_T("%s �� %s - �� %d �� ��"),szProcessName,m_ServerAttribute.szServerName,m_wTableID+1);
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

//�ں�����
void CClientKernel::ResetClientKernel()
{
    //���ñ���
    m_pMeUserItem=NULL;
    m_bGameStatus=GAME_STATUS_FREE;
    m_wTableID=INVALID_TABLE;
    m_wChairID=INVALID_CHAIR;

    //�������
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
    //Ч������
    if (pCopyDataStruct->cbData<sizeof(IPC_Head)) return false;

    //��ȡ����
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
