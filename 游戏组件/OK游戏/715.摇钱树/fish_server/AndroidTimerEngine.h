
#ifndef _ANDROIDTIMERENGINE_H_
#define _ANDROIDTIMERENGINE_H_
#pragma once

#include "Array.h"



class CAndroidTimerEngine;
// 定时器线程
class CAndroidTimerThread 
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

	bool StartThread();
	bool ConcludeThread(WORD THREADID);

	//功能函数
public:
	//配置函数
	bool InitThread(CAndroidTimerEngine * pAndroidTimerEngine, DWORD dwTimerSpace);

	//重载函数
private:
	//运行函数
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

  //状态变量
protected:
	bool							m_bService;							//运行标志

	//配置定义
protected:
	DWORD							m_dwAndroidTimerSpace;				//时间间隔

	//组件变量
protected:
	CArrayTemplate<ANDOIRD_TIMER_ITEM>	m_TimerItem;					   //定时器子项
//	CCriticalSection				m_CriticalSection;					//锁定对象
	CMutex							m_ff;
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
	bool IsServicing(){return m_bService;}

	// 接口函数
public:
	//停止定时器
	bool KillAllTimer();
	//停止定时器
	bool KillGameTimer(CAndroidUserItemSink *pAndroidSink,UINT nTimerID);
	//启动定时器
	bool SetGameTimer(CAndroidUserItemSink *pAndroidSink, UINT nTimerID, UINT nElapse);

	//内部函数
private:
	//定时器通知
	VOID OnAndroidTimerThreadSink();	

	//消息映射
protected:
	//SOCKET 消息处理程序
	afx_msg LRESULT	OnAndroidTimerMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif  // _ANDROIDTIMERENGINE_H_
