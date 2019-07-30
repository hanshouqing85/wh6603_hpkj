
#include "StdAfx.h"
#include "AndroidTimerEngine.h"
#include "android_useritem_sink.h"

//////////////////////////////////////////////////////////////////////////////////

//�궨��
#define ANDROID_TIMER					100L							//��ʱ��ʱ��
#define NO_TIME_LEAVE				DWORD(-1)							//����ʱ��
#define WM_ANDROID_TIMER                (WM_USER+100)

//////////////////////////////////////////////////////////////////////////////////
static bool bRun = false;

//���캯��
CAndroidTimerThread::CAndroidTimerThread()
{
	//���ñ���
	m_dwLastTickCount=0L;
	m_dwAndroidTimerSpace=ANDROID_TIMER;

	//����ӿ�
	m_pAndroidTimerEngine=NULL;

	return;
}

//��������
CAndroidTimerThread::~CAndroidTimerThread()
{
}
bool CAndroidTimerThread::StartThread()
{
	bRun= true;
	timer_thread_ = (HANDLE)::_beginthreadex(NULL, 0, TimerThread, static_cast<void*>(this), 0, NULL);
	return (timer_thread_!=NULL);
}

//���ú���
bool CAndroidTimerThread::InitThread(CAndroidTimerEngine * pAndroidTimerEngine, DWORD dwTimerSpace)
{
	//���ñ���
	m_dwLastTickCount=0L;
	m_dwAndroidTimerSpace=dwTimerSpace;

	//����ָ��
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

//���к���
bool CAndroidTimerThread::OnEventThreadRun()
{
	//Ч�����
	ASSERT(m_pAndroidTimerEngine!=NULL);

	//��ȡʱ��
	DWORD dwNowTickCount=GetTickCount();
	DWORD dwTimerSpace=m_dwAndroidTimerSpace;

	//�ȴ�����
	if ((m_dwLastTickCount!=0L)&&(dwNowTickCount>m_dwLastTickCount))
	{
		DWORD dwHandleTickCount=dwNowTickCount-m_dwLastTickCount;
		dwTimerSpace=(dwTimerSpace>dwHandleTickCount)?(dwTimerSpace-dwHandleTickCount):0L;
	}

	//��ʱ����
	Sleep(dwTimerSpace);
	m_dwLastTickCount=GetTickCount();

	//ʱ�䴦��
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

//���캯��
CAndroidTimerEngine::CAndroidTimerEngine()
{
	//���ñ���
	m_bService=false;

	//״̬����
	m_dwAndroidTimerSpace=ANDROID_TIMER;	

	return;
}

//��������
CAndroidTimerEngine::~CAndroidTimerEngine()
{
	//ֹͣ����
	ConcludeService();

	return;
}

//��ʼ����
bool CAndroidTimerEngine::StartService()
{
	//״̬Ч��
	ASSERT(m_bService==false);
	if (m_bService==true) return false;
	if (GetSafeHwnd() == NULL)
	{
		Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),10,NULL);
	}

	//���ñ���
	if (m_AndroidTimerThread.InitThread(this,m_dwAndroidTimerSpace)==false) return false;

	//��������
	if (m_AndroidTimerThread.StartThread()==false) return false;

	//���ñ���
	m_bService=true;


	return true;
}

//ֹͣ����
bool CAndroidTimerEngine::ConcludeService()
{
	//���ñ���
	m_bService=false;
	if (GetSafeHwnd() != NULL)
	{
		DestroyWindow();
	}

	//ɾ����ʱ��
	KillAllTimer();

	//ֹͣ�߳�
	m_AndroidTimerThread.ConcludeThread(INFINITE);

	return true;
}

//ɾ����ʱ��
bool CAndroidTimerEngine::KillAllTimer()
{
	//������Դ
	CSingleLock  g(&m_ff);
	g.Lock();

	//ɾ����ʱ��
	m_TimerItem.RemoveAll();

	return true;
}

//ɾ��ʱ��
bool CAndroidTimerEngine::KillGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID)
{
	//������Դ
	CSingleLock  g(&m_ff);
	g.Lock();

	//���Ҷ�ʱ��
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
				//���ö���
				m_TimerItem.RemoveAt(i);
				return true;
			}
		}
	}

	return false;
}

//����ʱ��
bool CAndroidTimerEngine::SetGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID, UINT nElapse)
{
	//������Դ
	CSingleLock  g(&m_ff);
	g.Lock();

	//��������
	bool bTimerExist=false;

	//��������
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

	//��������
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

//��ʱ��֪ͨ
VOID CAndroidTimerEngine::OnAndroidTimerThreadSink()
{
	//������Դ
//	CDataLocker DataLocker(m_CriticalSection);
	CSingleLock  g(&m_ff);
	g.Lock();

	//��ѯ����
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