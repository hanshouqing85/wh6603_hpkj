#ifndef ANDROID_SERVICE_HEAD_FILE
#define ANDROID_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//游戏服务器管理类
////class CAndroidService : public IAndroidService
class CGameServiceManager : public IGameServiceManager
{
	//函数定义
public:
	//构造函数
	////CAndroidService(void);
	CGameServiceManager(void);
	//析构函数
	////virtual ~CAndroidService(void);
	virtual ~CGameServiceManager(void);

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { delete this; }
	//接口查询
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//功能接口
public:
	//创建机器人
	virtual VOID*  CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer);



	//创建接口
public:
	//创建桌子
	virtual VOID * CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer);
	//创建数据
	virtual VOID * CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer);

	//参数接口
public:
	//组件属性
	virtual bool GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib);
	//调整参数
	virtual bool RectifyParameter(tagGameServiceOption & GameServiceOption);

};

//////////////////////////////////////////////////////////////////////////

#endif
