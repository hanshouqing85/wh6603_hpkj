
#ifndef _ANDROIDTIMERENGINE_H_
#define _ANDROIDTIMERENGINE_H_
#pragma once

class CAndroidTimerEngine;
// ��ʱ���߳�
class CAndroidTimerThread : public CServiceThread
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

	//���ܺ���
public:
	//���ú���
	bool InitThread(CAndroidTimerEngine * pAndroidTimerEngine, DWORD dwTimerSpace);

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
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
	CCriticalSection				m_CriticalSection;					//��������
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

	// �ӿں���
public:
	//ֹͣ��ʱ��
	bool KillAllTimer();
	//ֹͣ��ʱ��
	bool KillGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID);
	//������ʱ��
	bool SetGameTimer(CAndroidUserItemSink *pAndroidSink, UINT nTimerID, UINT nElapse);
LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//�ڲ�����
private:
	//��ʱ��֪ͨ
	VOID OnAndroidTimerThreadSink(CAndroidTimerEngine *pTs);	
public:
	// for translating Windows messages in main message pump
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	//��Ϣӳ��
protected:
	//SOCKET ��Ϣ�������
	LRESULT	OnAndroidTimerMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#endif  // _ANDROIDTIMERENGINE_H_
