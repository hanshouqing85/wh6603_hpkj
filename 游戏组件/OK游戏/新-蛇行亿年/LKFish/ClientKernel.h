#ifndef CLIENT_KERNEL_HEAD_FILE
#define CLIENT_KERNEL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "stdafx.h"
#include <string>
#include <deque>

#include <assert.h>
#define ASSERT(f) assert(f)


//定义文件
#include "..\..\..\开发库\Include\Macro.h"
#include "..\..\..\开发库\Include\Define.h"

//结构文件
#include "..\..\..\开发库\Include\Struct.h"
#include "..\..\..\开发库\Include\Packet.h"
#include "..\..\..\开发库\Include\Property.h"

//模板文件
#include "..\..\..\开发库\Include\Module.h"
#include "..\..\..\开发库\Include\PacketAide.h"
#include "..\..\..\开发库\Include\ServerRule.h"
#include "..\..\..\开发库\Include\RightDefine.h"

#include "..\..\..\开发库\Include\IPC_GameFrame.h"
#include "..\..\..\开发库\Include\CMD_GameServer.h"
#include "..\..\..\开发库\Include\CMD_Commom.h"



typedef DWORD (__stdcall *NtTerminateProcess)(HANDLE,UINT); 

//无效数值
#define INVALID_BYTE				((BYTE)(0xFF))						//无效数值
#define INVALID_WORD				((WORD)(0xFFFF))					//无效数值
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//无效数值

//无效数值
#define INVALID_TABLE				INVALID_WORD						//无效桌子
#define INVALID_CHAIR				INVALID_WORD						//无效椅子


#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//框架版本

//处理类型
#define IDT_IMMEDIATE				0x0101								//同步数据
#define IDT_ASYNCHRONISM			0x0201								//异步数据
////内核主命令码
//#define IPC_MIAN_IPC_KERNEL					0							//内核命令

//内核子命令码
//#define IPC_SUB_IPC_CLIENT_CONNECT			1							//连接通知
//#define IPC_SUB_IPC_SERVER_ACCEPT			2							//应答通知
//#define IPC_SUB_IPC_CLIENT_CLOSE			3							//关闭通知

#define WM_IPC_MESSAGE				(WM_USER+1511)						//IPC 消息

#define WM_EDIT_ID				   (WM_USER+1611)						//编辑 消息


//房间属性
struct tagServerAttribute
{
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	WORD							wServerType;						//游戏类型
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名称
	WORD							wAVServerPort;						//视频端口
	DWORD							dwAVServerAddr;						//视频地址
};

//游戏属性
struct tagGameAttribute
{
	WORD							wKindID;							//类型标识
	WORD							wChairCount;						//椅子数目
	DWORD							dwClientVersion;					//游戏版本
	TCHAR							szGameName[LEN_KIND];				//游戏名字
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
//客户端内核
class CClientKernel 
{
    //房间信息
protected:
    WORD								m_wTableID;						//桌子号码
    WORD								m_wChairID;						//椅子号码

    //固有信息
protected:
    DWORD								m_dwUserID;						//用户 I D
    tagServerAttribute					m_ServerAttribute;				//房间属性
	tagGameAttribute					m_GameAttribute;				//游戏属性

  

    //游戏信息
protected:
    BYTE								m_bGameStatus;					//游戏状态

    //用户信息
protected:
   tagUserInfo				  * m_pMeUserItem;				//自己信息
    tagUserInfo				  * m_pUserItem[MAX_CHAIR];		//玩家信息

    //通讯变量
protected:
    HANDLE					    m_hShareMemory;					//共享句柄
  //  tagShareMemory	    * m_pShareMemory;				//共享内存

    //接口指针
protected:
    CClientKernelSink   *m_pIClientKernelSink;

    //信道
public:
	HWND				  m_hWndGameFrame;
	HWND				  m_hWndEdit;
	HWND                  m_hWndChannel;
protected:
    HWND                  m_hWndGameServer;
    std::deque<IPC_Buffer *> m_IPCBuffers;
    static CClientKernel *m_ClientKernel;


    //函数定义
public:
    //构造函数
    CClientKernel();
    //析构函数
    virtual ~CClientKernel();

    //驱动接口
public:
    //设置接口
    virtual bool  DestroyClientKernel();
  //内核配置
    virtual bool  InitClientKernel(HWND hWnd,LPCWSTR CmdLine, CClientKernelSink* pIUnknownEx);
	//命令行解析
	bool SearchCommandItem(LPCWSTR pszCommandLine, LPCWSTR pszCommand, std::wstring &szParameter);

    //信息接口
public:
    //房间属性
    virtual const tagServerAttribute *  GetServerAttribute() { return &m_ServerAttribute; }

    //状态接口
public:
    //游戏状态
    virtual BYTE  GetGameStatus() { return m_bGameStatus; }
    //游戏状态
    virtual void  SetGameStatus(BYTE bGameStatus) { m_bGameStatus=bGameStatus; }

    //用户接口
public:
      //自己位置
    virtual WORD  GetMeChairID() { return m_wChairID; }
    //获取自己
    virtual tagUserInfo *  GetMeUserInfo() { return m_pMeUserItem; }
    //获取玩家
    virtual tagUserInfo *  GetUserInfo(WORD wChairID);
    //发送接口
public:
    virtual bool  SendData(WORD wMainCmdID, WORD wSubCmdID);

    virtual bool  SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
    //发送函数
    virtual bool  SendSocketData(WORD wMainCmdID, WORD wSubCmdID);
    //发送函数
    virtual bool  SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);

    //功能接口
public:
    //发送准备
    virtual bool  SendUserReady(void * pBuffer, WORD wDataSize);
    //发送聊天
    virtual bool  SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor);
	//发送写比赛成绩 2011.9.17 by gaoshan
	//virtual bool  SendWriteMatchScore(void * pBuffer,WORD wDataSize);

    //信道接口
public:
    //信道数据处理
    virtual bool  OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

    //IPC 子命令
private:
    //网络消息
    bool OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //配置信息
    bool OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //用户通知
    bool OnIPCUser(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //控制通知
    bool OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

    //用户管理
private:
  	tagUserInfo *  SearchUserItem(DWORD dwUserID);

    bool ActiveUserItem(tagUserInfo * pUserData);
    //删除用户
    bool DeleteUserItem(DWORD dwUserID);
    //更新用户
    bool UpdateUserItem(DWORD dwUerID, const tagUserScore * pUserScore);
    //更新用户
    bool UpdateUserItem(DWORD dwUserID,  tagUserStatus * pUserStatus, WORD wNetDelay);

    //内部函数
private:
    ////更新标题
    void UpdateGameTitle();
    //内核重置
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