
#ifndef _ANDROIDTIMERENGINE_H_
#define _ANDROIDTIMERENGINE_H_
#pragma once

class CAndroidTimerEngine;
// 定时器线程
class CAndroidTimerThread : public CServiceThread
{
	//变量定义
protected:
	DWORD							m_dwLastTickCount;					//处理时间
	DWORD							m_dwAndroidTimerSpace;				//时间间隔

	//组件指针
protected:
	CAndroidTimerEngine *			m_pAndroidTimerEngine;				//时间引擎

	//函数定义
public:
	//构造函数
	CAndroidTimerThread();
	//析构函数
	virtual ~CAndroidTimerThread();

	//功能函数
public:
	//配置函数
	bool InitThread(CAndroidTimerEngine * pAndroidTimerEngine, DWORD dwTimerSpace);

	//重载函数
private:
	//运行函数
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

  //状态变量
protected:
	bool							m_bService;							//运行标志

	//配置定义
protected:
	DWORD							m_dwAndroidTimerSpace;				//时间间隔

	//组件变量
protected:
	CArrayTemplate<ANDOIRD_TIMER_ITEM>	m_TimerItem;					   //定时器子项
	CCriticalSection				m_CriticalSection;					//锁定对象
	CAndroidTimerThread				m_AndroidTimerThread;				//线程对象

	//函数定义
public:
	//构造函数
	CAndroidTimerEngine();
	//析构函数
	virtual ~CAndroidTimerEngine();

	//服务接口
public:
	//启动服务
	bool StartService();
	//停止服务
	bool ConcludeService();

	// 接口函数
public:
	//停止定时器
	bool KillAllTimer();
	//停止定时器
	bool KillGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID);
	//启动定时器
	bool SetGameTimer(CAndroidUserItemSink *pAndroidSink, UINT nTimerID, UINT nElapse);
LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//内部函数
private:
	//定时器通知
	VOID OnAndroidTimerThreadSink(CAndroidTimerEngine *pTs);	
public:
	// for translating Windows messages in main message pump
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	//消息映射
protected:
	//SOCKET 消息处理程序
	LRESULT	OnAndroidTimerMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#endif  // _ANDROIDTIMERENGINE_H_
