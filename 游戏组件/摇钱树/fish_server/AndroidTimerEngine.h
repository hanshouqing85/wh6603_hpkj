
#ifndef _ANDROIDTIMERENGINE_H_
#define _ANDROIDTIMERENGINE_H_
#pragma once

#include "Array.h"



class CAndroidTimerEngine;
// ��ʱ���߳�
class CAndroidTimerThread 
{
	//��������
protected:
	DWORD							m_dwLastTickCount;					//����ʱ��
	DWORD							m_dwAndroidTimerSpace;				//ʱ����

	//���ָ��
protected:
	CAndroidTimerEngine *			m_pAndroidTimerEngine;				//ʱ������

	//��������
public:
	//���캯��
	CAndroidTimerThread();
	//��������
	virtual ~CAndroidTimerThread();

	bool StartThread();
	bool ConcludeThread(WORD THREADID);

	//���ܺ���
public:
	//���ú���
	bool InitThread(CAndroidTimerEngine * pAndroidTimerEngine, DWORD dwTimerSpace);

	//���غ���
private:
	//���к���
	bool OnEventThreadRun();
	static unsigned __stdcall TimerThread(void* param);
	HANDLE timer_thread_;
};



class CAndroidUserItemSink;
typedef struct  tagAndoirdTimerItem
{
	CAndroidUserItemSink *pAndroidUserItemSink;
	UINT nTimerID;
	UINT nTimeLeave;
	tagAndoirdTimerItem()
	{
		pAndroidUserItemSink = NULL;
		nTimerID = 0;
		nTimeLeave = 0;
	}
}ANDOIRD_TIMER_ITEM;



class CAndroidTimerEngine : public CWnd
{
  friend class CAndroidTimerThread;

  //״̬����
protected:
	bool							m_bService;							//���б�־

	//���ö���
protected:
	DWORD							m_dwAndroidTimerSpace;				//ʱ����

	//�������
protected:
	CArrayTemplate<ANDOIRD_TIMER_ITEM>	m_TimerItem;					   //��ʱ������
//	CCriticalSection				m_CriticalSection;					//��������
	CMutex							m_ff;
	CAndroidTimerThread				m_AndroidTimerThread;				//�̶߳���

	//��������
public:
	//���캯��
	CAndroidTimerEngine();
	//��������
	virtual ~CAndroidTimerEngine();

	//����ӿ�
public:
	//��������
	bool StartService();
	//ֹͣ����
	bool ConcludeService();
	bool IsServicing(){return m_bService;}

	// �ӿں���
public:
	//ֹͣ��ʱ��
	bool KillAllTimer();
	//ֹͣ��ʱ��
	bool KillGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID);
	//������ʱ��
	bool SetGameTimer(CAndroidUserItemSink *pAndroidSink, UINT nTimerID, UINT nElapse);

	//�ڲ�����
private:
	//��ʱ��֪ͨ
	VOID OnAndroidTimerThreadSink();	

	//��Ϣӳ��
protected:
	//SOCKET ��Ϣ�������
	afx_msg LRESULT	OnAndroidTimerMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif  // _ANDROIDTIMERENGINE_H_
