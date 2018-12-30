
#include "StdAfx.h"
#include "game_service_manager.h"
#include "table_frame_sink.h"
#include "android_useritem_sink.h"
//#include "AndroidTimerEngine.h"

//extern CAndroidTimerEngine g_AndroidTimerEngine;
//机器定义
#ifndef _DEBUG
	#define ANDROID_SERVICE_DLL_NAME	TEXT("zyqs_android.dll")	//组件名字
#else
	#define ANDROID_SERVICE_DLL_NAME	TEXT("zyqs_android.dll")	//组件名字
#endif

GameServiceManager::GameServiceManager() 
{
  game_service_attrib_.wKindID = KIND_ID;
  game_service_attrib_.wChairCount = GAME_PLAYER;
  game_service_attrib_.wSupporType = GAME_GENRE_GOLD | GAME_GENRE_EDUCATE;
  game_service_attrib_.cbDynamicJoin = TRUE;
  game_service_attrib_.cbAndroidUser = TRUE;
  game_service_attrib_.cbOffLineTrustee = FALSE;
  game_service_attrib_.dwServerVersion = VERSION_SERVER;
  game_service_attrib_.dwClientVersion = VERSION_CLIENT;
  lstrcpyn(game_service_attrib_.szGameName, GAME_NAME, CountArray(game_service_attrib_.szGameName));
  lstrcpyn(game_service_attrib_.szDataBaseName, szTreasureDB, CountArray(game_service_attrib_.szDataBaseName));
  lstrcpyn(game_service_attrib_.szClientEXEName, TEXT("zyqs.exe"), CountArray(game_service_attrib_.szClientEXEName));
  lstrcpyn(game_service_attrib_.szServerDLLName, TEXT("zyqs_server.dll"), CountArray(game_service_attrib_.szServerDLLName));
}

GameServiceManager::~GameServiceManager() 
{

}

void GameServiceManager::Release() 
{
	//if (g_AndroidTimerEngine.IsServicing())
	//{
	//	g_AndroidTimerEngine.ConcludeService();
	//}
}

void* GameServiceManager::QueryInterface(REFGUID guid, DWORD query_ver) {
  QUERYINTERFACE(IGameServiceManager, guid, query_ver);
  QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager, guid, query_ver);
  return NULL;
}

void* GameServiceManager::CreateTableFrameSink(REFGUID guid, DWORD query_ver) 
{
	TableFrameSink* table_frame_sink = new TableFrameSink();
	void* ret = table_frame_sink->QueryInterface(guid, query_ver);
	if (ret == NULL) SafeDelete(table_frame_sink);
	//if (!g_AndroidTimerEngine.IsServicing())
	//{
	//	g_AndroidTimerEngine.StartService();
	//}

	return ret;
}

//void* GameServiceManager::CreateAndroidUserItemSink(REFGUID guid, DWORD query_ver) 
//{
//	if (game_service_manager_helper_.GetInterface() == NULL) 
//	{
//		game_service_manager_helper_.SetModuleCreateInfo(TEXT("zyqs_android.dll"), "CreateGameServiceManager");
//		if (!game_service_manager_helper_.CreateInstance()) return NULL;
//	}
//
//	return game_service_manager_helper_->CreateAndroidUserItemSink(guid, query_ver);
//}

void* GameServiceManager::CreateAndroidUserItemSink(REFGUID guid, DWORD query_ver) 
{
	//变量定义
	CAndroidUserItemSink * pAndroidUserItemSink=NULL;

	try
	{
#if 0
		//建立对象
		pAndroidUserItemSink=new CAndroidUserItemSink();
		if (pAndroidUserItemSink==NULL) throw TEXT("创建失败");

		//查询接口
		void * pObject=pAndroidUserItemSink->QueryInterface(guid,query_ver);
		if (pObject==NULL) throw TEXT("接口查询失败");

		return pObject;
#else
		//创建组件
		if( m_AndroidServiceHelper.GetInterface() == NULL )
		{
			m_AndroidServiceHelper.SetModuleCreateInfo(ANDROID_SERVICE_DLL_NAME,"CreateGameServiceManager");

			if( !m_AndroidServiceHelper.CreateInstance() ) throw 0;
		}

		//创建机器人
		VOID *pAndroidObject = m_AndroidServiceHelper->CreateAndroidUserItemSink(guid,query_ver);
		if( pAndroidObject == NULL ) throw TEXT("创建机器人失败");

		return pAndroidObject;
#endif
	}
	catch (...) {}

#if 0
	//删除对象
	SafeDelete(pAndroidUserItemSink);
#endif

	return NULL;
}

void* GameServiceManager::CreateGameDataBaseEngineSink(REFGUID guid, DWORD query_ver) {
  return NULL;
}

bool GameServiceManager::GetServiceAttrib(tagGameServiceAttrib& game_service_attrib) {
  game_service_attrib = game_service_attrib_;
  return true;
}

bool GameServiceManager::RectifyParameter(tagGameServiceOption& game_service_option) {
  return true;
}

extern "C" __declspec(dllexport) void* CreateGameServiceManager(REFGUID guid, DWORD interface_ver) {
  static GameServiceManager game_service_manager;
  return game_service_manager.QueryInterface(guid, interface_ver);
}