
#include "StdAfx.h"
#include "AndroidTimerEngine.h"
#include "android_useritem_sink.h"

//////////////////////////////////////////////////////////////////////////////////

//宏定义
#define ANDROID_TIMER					300L							//脉冲时间
#define NO_TIME_LEAVE				DWORD(-1)							//忽略时间
#define WM_ANDROID_TIMER                (WM_USER+100)

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidTimerThread::CAndroidTimerThread()
{
	//设置变量
	m_dwLastTickCount=0L;
	m_dwAndroidTimerSpace=ANDROID_TIMER;

	//组件接口
	m_pAndroidTimerEngine=NULL;

	return;
}

//析构函数
CAndroidTimerThread::~CAndroidTimerThread()
{
}

//配置函数
bool CAndroidTimerThread::InitThread(CAndroidTimerEngine * pAndroidTimerEngine, DWORD dwTimerSpace)
{
	//设置变量
	m_dwLastTickCount=0L;
	m_dwAndroidTimerSpace=dwTimerSpace;

	//设置指针
	m_pAndroidTimerEngine=pAndroidTimerEngine;

	return true;
}

//运行函数
bool CAndroidTimerThread::OnEventThreadRun()
{
	//效验参数
	ASSERT(m_pAndroidTimerEngine!=NULL);

	//获取时间
	DWORD dwNowTickCount=GetTickCount();
	DWORD dwTimerSpace=m_dwAndroidTimerSpace;

	//等待调整
	if ((m_dwLastTickCount!=0L)&&(dwNowTickCount>m_dwLastTickCount))
	{
		DWORD dwHandleTickCount=dwNowTickCount-m_dwLastTickCount;
		dwTimerSpace=(dwTimerSpace>dwHandleTickCount)?(dwTimerSpace-dwHandleTickCount):0L;
	}

	//定时处理
	Sleep(dwTimerSpace);
	m_dwLastTickCount=GetTickCount();

	//时间处理
	m_pAndroidTimerEngine->OnAndroidTimerThreadSink(m_pAndroidTimerEngine);
	if (m_pAndroidTimerEngine->GetSafeHwnd() == NULL)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAndroidTimerEngine, CWnd)
	ON_MESSAGE(WM_ANDROID_TIMER, OnAndroidTimerMessage)
	ON_WM_TIMER()
END_MESSAGE_MAP()

static HWND g_hWnd = NULL;
//构造函数
CAndroidTimerEngine::CAndroidTimerEngine()
{
	//设置变量
	m_bService=false;

	//状态变量
	m_dwAndroidTimerSpace=ANDROID_TIMER;

	Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),10,NULL);
	g_hWnd =GetSafeHwnd();

	SetTimer(10, 1000, NULL);
	return;
}

//析构函数
CAndroidTimerEngine::~CAndroidTimerEngine()
{
	//停止服务
	ConcludeService();

	return;
}

//开始服务
bool CAndroidTimerEngine::StartService()
{
	//状态效验
	ASSERT(m_bService==false);
	if (m_bService==true) return false;

	//设置变量
	if (m_AndroidTimerThread.InitThread(this,m_dwAndroidTimerSpace)==false) return false;

	//启动服务
	if (m_AndroidTimerThread.StartThread()==false) return false;

	//设置变量
	m_bService=true;


	return true;
}

//停止服务
bool CAndroidTimerEngine::ConcludeService()
{
	//设置变量
	m_bService=false;
	if (GetSafeHwnd() != NULL)
	{
		DestroyWindow();
	}

	//删除脉冲
	KillAllTimer();

	//停止线程
	m_AndroidTimerThread.ConcludeThread(INFINITE);

	return true;
}

//删除脉冲
bool CAndroidTimerEngine::KillAllTimer()
{
	//锁定资源
	CDataLocker DataLocker(m_CriticalSection);

	//删除脉冲
	m_TimerItem.RemoveAll();

	return true;
}

//删除时间
bool CAndroidTimerEngine::KillGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID)
{
	//锁定资源
	CDataLocker DataLocker(m_CriticalSection);

	//查找脉冲
	for (INT_PTR i=0;i<m_TimerItem.GetCount();i++)
	{
		if (m_TimerItem[i].nTimerID==nTimerID)
		{
			if (m_TimerItem[i].pAndroidUserItemSink == NULL)
			{
				continue;
			}
			DWORD dwUserID = m_TimerItem[i].pAndroidUserItemSink->GetAndroidUserID();
			if ((dwUserID != 0) && (dwUserID== pAndroidSink->GetAndroidUserID()))
			{
				//设置对象
				m_TimerItem.RemoveAt(i);
				return true;
			}
		}
	}

	return false;
}

//设置时间
bool CAndroidTimerEngine::SetGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID, UINT nElapse)
{
	//锁定资源
	CDataLocker DataLocker(m_CriticalSection);

	//变量定义
	bool bTimerExist=false;

	//查找子项
	for (INT_PTR i=0;i<m_TimerItem.GetCount();i++)
	{
		if (m_TimerItem[i].nTimerID==nTimerID)
		{
			if (m_TimerItem[i].pAndroidUserItemSink == NULL)
			{
				continue;
			}
			DWORD dwUserID = m_TimerItem[i].pAndroidUserItemSink->GetAndroidUserID();
			if ((dwUserID != 0) && (dwUserID== pAndroidSink->GetAndroidUserID()))
			{
				bTimerExist=true;
				break;
			}
		}
	}

	//创建子项
	if (bTimerExist==false)
	{
		ANDOIRD_TIMER_ITEM timerItem;
		timerItem.pAndroidUserItemSink = pAndroidSink;
		timerItem.nTimerID = nTimerID;
		if (nElapse<m_dwAndroidTimerSpace) 
			nElapse = m_dwAndroidTimerSpace;
		timerItem.nTimeLeave = nElapse;
		m_TimerItem.Add(timerItem);		
	}
	return false;
}

//脉冲通知
VOID CAndroidTimerEngine::OnAndroidTimerThreadSink(CAndroidTimerEngine *pTs)
{
	//锁定资源
	//CDataLocker DataLocker(m_CriticalSection);

	//查询子项
	int nIndex = 0;
	while (nIndex<m_TimerItem.GetCount())
	{
		if (m_TimerItem[nIndex].nTimeLeave>m_dwAndroidTimerSpace)
		{
			m_TimerItem[nIndex].nTimeLeave -= m_dwAndroidTimerSpace;			
		}
		else
		{
			CAndroidUserItemSink *pAndroidSink = m_TimerItem[nIndex].pAndroidUserItemSink;
			WPARAM wTimerID = (WPARAM)m_TimerItem[nIndex].nTimerID;
			m_TimerItem.RemoveAt(nIndex);
			OutputDebugStringA("qlog: server android send message");
			if (g_hWnd != NULL)
			{
				::PostMessage(g_hWnd, WM_ANDROID_TIMER,(WPARAM)wTimerID, (LPARAM)pAndroidSink);
			}
			//pTs->PostMessage(WM_ANDROID_TIMER,(WPARAM)wTimerID, (LPARAM)pAndroidSink);
		}
		nIndex++;
	}
	return;
}

// for translating Windows messages in main message pump
BOOL CAndroidTimerEngine::PreTranslateMessage(MSG* pMsg)
{
	CString csT;
	csT.Format(TEXT("qlog: server android message PreTranslateMessage %u "),pMsg->message);
	OutputDebugString(csT);
	if (pMsg->message == WM_ANDROID_TIMER)
	{
		OutputDebugStringA("qlog: server anPreTranslateMessage   ge");
	}
	return CWnd::PreTranslateMessage(pMsg);
}

LRESULT CAndroidTimerEngine::OnAndroidTimerMessage(WPARAM wParam, LPARAM lParam)
{
	OutputDebugStringA("qlog: server android on message");
	CAndroidUserItemSink *pAndroidSink = (CAndroidUserItemSink *)lParam;
	if ((pAndroidSink!=NULL) && (pAndroidSink->GetAndroidUserID() != 0))
	{
		pAndroidSink->OnEventTimer((UINT)wParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

LRESULT CAndroidTimerEngine::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CString csT;
	csT.Format(TEXT("qlog: server android message WindowProc %u "),message);
	OutputDebugString(csT);
	if (message == WM_ANDROID_TIMER)
	{
		OutputDebugStringA("qlog: server WindowProc   ge");
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CAndroidTimerEngine::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 10)
	{
		OutputDebugStringA("qlog: server CAndroidTimerEngine::OnTimer");
	}

	CWnd::OnTimer(nIDEvent);
}
