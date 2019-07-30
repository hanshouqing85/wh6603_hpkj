
#include "StdAfx.h"
#include "AndroidTimerEngine.h"
#include "android_useritem_sink.h"

//////////////////////////////////////////////////////////////////////////////////

//�궨��
#define ANDROID_TIMER					300L							//����ʱ��
#define NO_TIME_LEAVE				DWORD(-1)							//����ʱ��
#define WM_ANDROID_TIMER                (WM_USER+100)

//////////////////////////////////////////////////////////////////////////////////

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
//���캯��
CAndroidTimerEngine::CAndroidTimerEngine()
{
	//���ñ���
	m_bService=false;

	//״̬����
	m_dwAndroidTimerSpace=ANDROID_TIMER;

	Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),10,NULL);
	g_hWnd =GetSafeHwnd();

	SetTimer(10, 1000, NULL);
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

	//ɾ������
	KillAllTimer();

	//ֹͣ�߳�
	m_AndroidTimerThread.ConcludeThread(INFINITE);

	return true;
}

//ɾ������
bool CAndroidTimerEngine::KillAllTimer()
{
	//������Դ
	CDataLocker DataLocker(m_CriticalSection);

	//ɾ������
	m_TimerItem.RemoveAll();

	return true;
}

//ɾ��ʱ��
bool CAndroidTimerEngine::KillGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID)
{
	//������Դ
	CDataLocker DataLocker(m_CriticalSection);

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
	CDataLocker DataLocker(m_CriticalSection);

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

//����֪ͨ
VOID CAndroidTimerEngine::OnAndroidTimerThreadSink(CAndroidTimerEngine *pTs)
{
	//������Դ
	//CDataLocker DataLocker(m_CriticalSection);

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
