#ifndef CLIENT_KERNEL_HEAD_FILE
#define CLIENT_KERNEL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "stdafx.h"
#include <string>
#include <deque>

#include <assert.h>
#define ASSERT(f) assert(f)


//�����ļ�
#include "..\..\..\������\Include\Macro.h"
#include "..\..\..\������\Include\Define.h"

//�ṹ�ļ�
#include "..\..\..\������\Include\Struct.h"
#include "..\..\..\������\Include\Packet.h"
#include "..\..\..\������\Include\Property.h"

//ģ���ļ�
#include "..\..\..\������\Include\Module.h"
#include "..\..\..\������\Include\PacketAide.h"
#include "..\..\..\������\Include\ServerRule.h"
#include "..\..\..\������\Include\RightDefine.h"

#include "..\..\..\������\Include\IPC_GameFrame.h"
#include "..\..\..\������\Include\CMD_GameServer.h"
#include "..\..\..\������\Include\CMD_Commom.h"



typedef DWORD (__stdcall *NtTerminateProcess)(HANDLE,UINT); 

//��Ч��ֵ
#define INVALID_BYTE				((BYTE)(0xFF))						//��Ч��ֵ
#define INVALID_WORD				((WORD)(0xFFFF))					//��Ч��ֵ
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//��Ч��ֵ

//��Ч��ֵ
#define INVALID_TABLE				INVALID_WORD						//��Ч����
#define INVALID_CHAIR				INVALID_WORD						//��Ч����


#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//��ܰ汾

//��������
#define IDT_IMMEDIATE				0x0101								//ͬ������
#define IDT_ASYNCHRONISM			0x0201								//�첽����
////�ں���������
//#define IPC_MIAN_IPC_KERNEL					0							//�ں�����

//�ں���������
//#define IPC_SUB_IPC_CLIENT_CONNECT			1							//����֪ͨ
//#define IPC_SUB_IPC_SERVER_ACCEPT			2							//Ӧ��֪ͨ
//#define IPC_SUB_IPC_CLIENT_CLOSE			3							//�ر�֪ͨ

#define WM_IPC_MESSAGE				(WM_USER+1511)						//IPC ��Ϣ

#define WM_EDIT_ID				   (WM_USER+1611)						//�༭ ��Ϣ


//��������
struct tagServerAttribute
{
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�����ʶ
	WORD							wServerType;						//��Ϸ����
	DWORD							dwServerRule;						//�������
	TCHAR							szServerName[LEN_SERVER];			//��������
	WORD							wAVServerPort;						//��Ƶ�˿�
	DWORD							dwAVServerAddr;						//��Ƶ��ַ
};

//��Ϸ����
struct tagGameAttribute
{
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wChairCount;						//������Ŀ
	DWORD							dwClientVersion;					//��Ϸ�汾
	TCHAR							szGameName[LEN_KIND];				//��Ϸ����
};


//////////////////////////////////////////////////////////////////////////
class CClientKernelSink
{
public:
     virtual void ResetGameFrame()=0;
    virtual void CloseGameFrame()=0;
	virtual bool OnEventSocket(const TCP_Command & Command, const void * pBuffer, WORD wDataSize)=0;
    virtual bool OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)=0;
    virtual void OnEventUserMemberOrder(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)=0;
    virtual void OnEventUserStatus(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)=0;
	virtual bool OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)=0;
	virtual bool OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUserr)=0;
    virtual void OnEventUserScore(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)=0;
};                                     

//////////////////////////////////////////////////////////////////////////
//�ͻ����ں�
class CClientKernel 
{
    //������Ϣ
protected:
    WORD								m_wTableID;						//���Ӻ���
    WORD								m_wChairID;						//���Ӻ���

    //������Ϣ
protected:
    DWORD								m_dwUserID;						//�û� I D
    tagServerAttribute					m_ServerAttribute;				//��������
	tagGameAttribute					m_GameAttribute;				//��Ϸ����

  

    //��Ϸ��Ϣ
protected:
    BYTE								m_bGameStatus;					//��Ϸ״̬

    //�û���Ϣ
protected:
   tagUserInfo				  * m_pMeUserItem;				//�Լ���Ϣ
    tagUserInfo				  * m_pUserItem[MAX_CHAIR];		//�����Ϣ

    //ͨѶ����
protected:
    HANDLE					    m_hShareMemory;					//������
  //  tagShareMemory	    * m_pShareMemory;				//�����ڴ�

    //�ӿ�ָ��
protected:
    CClientKernelSink   *m_pIClientKernelSink;

    //�ŵ�
public:
	HWND				  m_hWndGameFrame;
	HWND				  m_hWndEdit;
	HWND                  m_hWndChannel;
protected:
    HWND                  m_hWndGameServer;
    std::deque<IPC_Buffer *> m_IPCBuffers;
    static CClientKernel *m_ClientKernel;


    //��������
public:
    //���캯��
    CClientKernel();
    //��������
    virtual ~CClientKernel();

    //�����ӿ�
public:
    //���ýӿ�
    virtual bool  DestroyClientKernel();
  //�ں�����
    virtual bool  InitClientKernel(HWND hWnd,LPCWSTR CmdLine, CClientKernelSink* pIUnknownEx);
	//�����н���
	bool SearchCommandItem(LPCWSTR pszCommandLine, LPCWSTR pszCommand, std::wstring &szParameter);

    //��Ϣ�ӿ�
public:
    //��������
    virtual const tagServerAttribute *  GetServerAttribute() { return &m_ServerAttribute; }

    //״̬�ӿ�
public:
    //��Ϸ״̬
    virtual BYTE  GetGameStatus() { return m_bGameStatus; }
    //��Ϸ״̬
    virtual void  SetGameStatus(BYTE bGameStatus) { m_bGameStatus=bGameStatus; }

    //�û��ӿ�
public:
      //�Լ�λ��
    virtual WORD  GetMeChairID() { return m_wChairID; }
    //��ȡ�Լ�
    virtual tagUserInfo *  GetMeUserInfo() { return m_pMeUserItem; }
    //��ȡ���
    virtual tagUserInfo *  GetUserInfo(WORD wChairID);
    //���ͽӿ�
public:
    virtual bool  SendData(WORD wMainCmdID, WORD wSubCmdID);

    virtual bool  SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
    //���ͺ���
    virtual bool  SendSocketData(WORD wMainCmdID, WORD wSubCmdID);
    //���ͺ���
    virtual bool  SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);

    //���ܽӿ�
public:
    //����׼��
    virtual bool  SendUserReady(void * pBuffer, WORD wDataSize);
    //��������
    virtual bool  SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor);
	//����д�����ɼ� 2011.9.17 by gaoshan
	//virtual bool  SendWriteMatchScore(void * pBuffer,WORD wDataSize);

    //�ŵ��ӿ�
public:
    //�ŵ����ݴ���
    virtual bool  OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

    //IPC ������
private:
    //������Ϣ
    bool OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //������Ϣ
    bool OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //�û�֪ͨ
    bool OnIPCUser(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //����֪ͨ
    bool OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

    //�û�����
private:
  	tagUserInfo *  SearchUserItem(DWORD dwUserID);

    bool ActiveUserItem(tagUserInfo * pUserData);
    //ɾ���û�
    bool DeleteUserItem(DWORD dwUserID);
    //�����û�
    bool UpdateUserItem(DWORD dwUerID, const tagUserScore * pUserScore);
    //�����û�
    bool UpdateUserItem(DWORD dwUserID,  tagUserStatus * pUserStatus, WORD wNetDelay);

    //�ڲ�����
private:
    ////���±���
    void UpdateGameTitle();
    //�ں�����
    void ResetClientKernel();

public:
    bool OnCopyData(HWND hWnd, COPYDATASTRUCT * pCopyDataStruct);
    bool OnIPCMessage(WPARAM wParam, LPARAM lParam);

//	bool GetPubIP();
//	bool GetFileVersion(HMODULE hModule, WORD *pBuffer);
//	std::string GetFileVersion(HMODULE hModule);
	BOOL FindInPS(TCHAR ExeFileName[MAX_PATH],BYTE InstallPath[MAX_PATH],int &ID);
	std::string GetCurExeFilePath(ULONG type);
	void RestartMainWindow(char strExePath[MAX_PATH]);

	char m_cbPubIP[200];

private:
    static LRESULT CALLBACK ChannelWndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////

#endif