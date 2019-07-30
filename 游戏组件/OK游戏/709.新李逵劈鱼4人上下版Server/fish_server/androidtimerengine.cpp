
#include "StdAfx.h"
#include "AndroidTimerEngine.h"
#include "android_useritem_sink.h"

//////////////////////////////////////////////////////////////////////////////////

//宏定义
#define ANDROID_TIMER					100L							//定时器时间
#define NO_TIME_LEAVE				DWORD(-1)							//忽略时间
#define WM_ANDROID_TIMER                (WM_USER+100)

//////////////////////////////////////////////////////////////////////////////////
static bool bRun = false;

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
bool CAndroidTimerThread::StartThread()
{
	bRun= true;
	timer_thread_ = (HANDLE)::_beginthreadex(NULL, 0, TimerThread, static_cast<void*>(this), 0, NULL);
	return (timer_thread_!=NULL);
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
unsigned __stdcall CAndroidTimerThread::TimerThread(void* param) 
{
	CAndroidTimerThread* android = reinterpret_cast<CAndroidTimerThread*>(param);
	while(bRun)
	{
		::Sleep(500);
		android->OnEventThreadRun();
	}
	return 0;
}
bool CAndroidTimerThread::ConcludeThread(WORD THREADID)
{
	bRun = false;
	WaitForSingleObject(timer_thread_, INFINITE); 
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
	m_pAndroidTimerEngine->OnAndroidTimerThreadSink();
	if (m_pAndroidTimerEngine->GetSafeHwnd() == NULL)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAndroidTimerEngine, CWnd)
	ON_MESSAGE(WM_ANDROID_TIMER, OnAndroidTimerMessage)
END_MESSAGE_MAP()

//构造函数
CAndroidTimerEngine::CAndroidTimerEngine()
{
	//设置变量
	m_bService=false;

	//状态变量
	m_dwAndroidTimerSpace=ANDROID_TIMER;	

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
	if (GetSafeHwnd() == NULL)
	{
		Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),10,NULL);
	}

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

	//删除定时器
	KillAllTimer();

	//停止线程
	m_AndroidTimerThread.ConcludeThread(INFINITE);

	return true;
}

//删除定时器
bool CAndroidTimerEngine::KillAllTimer()
{
	//锁定资源
	CSingleLock  g(&m_ff);
	g.Lock();

	//删除定时器
	m_TimerItem.RemoveAll();

	return true;
}

//删除时间
bool CAndroidTimerEngine::KillGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID)
{
	//锁定资源
	CSingleLock  g(&m_ff);
	g.Lock();

	//查找定时器
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
	CSingleLock  g(&m_ff);
	g.Lock();

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

//定时器通知
VOID CAndroidTimerEngine::OnAndroidTimerThreadSink()
{
	//锁定资源
//	CDataLocker DataLocker(m_CriticalSection);
	CSingleLock  g(&m_ff);
	g.Lock();

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
			PostMessage(WM_ANDROID_TIMER,(WPARAM)wTimerID, (LPARAM)pAndroidSink);
		}
		nIndex++;
	}
	return;
}


LRESULT CAndroidTimerEngine::OnAndroidTimerMessage(WPARAM wParam, LPARAM lParam)
{
	CAndroidUserItemSink *pAndroidSink = (CAndroidUserItemSink *)lParam;
	if ((pAndroidSink!=NULL) && (pAndroidSink->GetAndroidUserID() != 0))
	{
		pAndroidSink->OnEventTimer((UINT)wParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////