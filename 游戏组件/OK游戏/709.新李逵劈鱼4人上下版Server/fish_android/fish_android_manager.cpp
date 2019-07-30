
#include "StdAfx.h"
#include "fish_android_manager.h"
#include "android_useritem_sink.h"
#include "AndroidTimerEngine.h"

CAndroidTimerEngine g_AndroidTimerEngine;
bool g_bInitEngine = false;

CGameServiceManager::CGameServiceManager() {
	if (!g_bInitEngine)
	{
		g_bInitEngine = true;
		g_AndroidTimerEngine.StartService();
	}	
}

CGameServiceManager::~CGameServiceManager() {
	if (g_bInitEngine)
	{
		g_bInitEngine = false;
		g_AndroidTimerEngine.ConcludeService();
	}
	
}

// 接口查询
void*  CGameServiceManager::QueryInterface(REFGUID guid, DWORD query_ver) {
  QUERYINTERFACE(IGameServiceManager, guid, query_ver);
  QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager, guid, query_ver);
  return NULL;
}

// 创建机器
void* CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
  CAndroidUserItemSink* pAndroidUserItemSink = NULL;

	try
	{
		//建立对象
		pAndroidUserItemSink=new CAndroidUserItemSink();
		if (pAndroidUserItemSink==NULL) throw TEXT("创建失败");

		//查询接口
		VOID * pObject=pAndroidUserItemSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("接口查询失败");

		return pObject;
	}
	catch (...) {}

	//删除对象
	SafeDelete(pAndroidUserItemSink);

	return NULL;
}

//创建游戏桌
VOID *  CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}
//创建数据
VOID * CGameServiceManager::CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}

//获取属性
bool  CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	return true;
}

//参数修改
bool  CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
	//效验参数
	ASSERT(&GameServiceOption!=NULL);
	if (&GameServiceOption==NULL) return false;

	//单元积分
	GameServiceOption.lCellScore -=__max(1L,GameServiceOption.lCellScore);

	//积分下限
	if (GameServiceOption.wServerType==GAME_GENRE_GOLD&&GameServiceOption.wServerType!=0)
	{
		GameServiceOption.lMinTableScore=__max(1000L,GameServiceOption.lMinTableScore);
	}

	//积分上限
	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore,GameServiceOption.lMinTableScore);
	}

	return true;
}
//建立对象
DECLARE_CREATE_MODULE(GameServiceManager)
