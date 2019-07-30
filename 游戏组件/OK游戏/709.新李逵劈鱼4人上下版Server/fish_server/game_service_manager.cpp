
#include "StdAfx.h"
#include "game_service_manager.h"
#include "table_frame_sink.h"
#include "android_useritem_sink.h"
#include "AndroidTimerEngine.h"

extern CAndroidTimerEngine g_AndroidTimerEngine;

GameServiceManager::GameServiceManager() 
{
	game_service_attrib_.wKindID = KIND_ID;
	game_service_attrib_.wChairCount = GAME_PLAYER;
	game_service_attrib_.wSupporType = GAME_GENRE_GOLD | GAME_GENRE_EDUCATE|GAME_GENRE_MATCH;
	game_service_attrib_.cbDynamicJoin = TRUE;
	game_service_attrib_.cbAndroidUser = TRUE;
	game_service_attrib_.cbOffLineTrustee = FALSE;
	game_service_attrib_.dwServerVersion = VERSION_SERVER;
	game_service_attrib_.dwClientVersion = VERSION_CLIENT;
	lstrcpyn(game_service_attrib_.szGameName, GAME_NAME, CountArray(game_service_attrib_.szGameName));
	lstrcpyn(game_service_attrib_.szDataBaseName, szTreasureDB, CountArray(game_service_attrib_.szDataBaseName));
	lstrcpyn(game_service_attrib_.szClientEXEName, TEXT("lkpy.exe"), CountArray(game_service_attrib_.szClientEXEName));
	lstrcpyn(game_service_attrib_.szServerDLLName, TEXT("lkpy_server.dll"), CountArray(game_service_attrib_.szServerDLLName));  
	//m_hDBEvent=::CreateEvent(NULL, FALSE, TRUE,FALSE);
}

GameServiceManager::~GameServiceManager() 
{	
	
}

void GameServiceManager::Release() {
	if (g_AndroidTimerEngine.IsServicing())
	{
		g_AndroidTimerEngine.ConcludeService();
	}
}

void* GameServiceManager::QueryInterface(REFGUID guid, DWORD query_ver) 
{
	QUERYINTERFACE(IGameServiceManager, guid, query_ver);
	QUERYINTERFACE(IGameServiceCustomRule,guid,query_ver);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager, guid, query_ver);
	return NULL;
}

void* GameServiceManager::CreateTableFrameSink(REFGUID guid, DWORD query_ver) {
	TableFrameSink* table_frame_sink = new TableFrameSink();
	void* ret = table_frame_sink->QueryInterface(guid, query_ver);
	if (ret == NULL) SafeDelete(table_frame_sink);
	if (!g_AndroidTimerEngine.IsServicing())
	{
		g_AndroidTimerEngine.StartService();
	}
	return ret;
}
void* GameServiceManager::CreateAndroidUserItemSink(REFGUID guid, DWORD query_ver) {
	//变量定义
	CAndroidUserItemSink * pAndroidUserItemSink=NULL;

	try
	{
		//建立对象
		pAndroidUserItemSink=new CAndroidUserItemSink();
		if (pAndroidUserItemSink==NULL) throw TEXT("创建失败");

		//查询接口
		void * pObject=pAndroidUserItemSink->QueryInterface(guid,query_ver);
		if (pObject==NULL) throw TEXT("接口查询失败");

		return pObject;
	}
	catch (...) {}

	//删除对象
	SafeDelete(pAndroidUserItemSink);

	return NULL;
}

void* GameServiceManager::CreateGameDataBaseEngineSink(REFGUID guid, DWORD query_ver) 
{
	try
	{
		CFishDataBaseEngine * pFishDataBaseEngineSink = new CFishDataBaseEngine;
		if( pFishDataBaseEngineSink == NULL )
			throw TEXT("创建捕鱼数据库引擎失败");

		pFishDataBaseEngineSink->SetGameServiceOption(game_service_option_);

		void * pObject = pFishDataBaseEngineSink->QueryInterface(guid, query_ver);
		if( pObject == NULL )
			throw TEXT("捕鱼接口查询失败");

		return pObject;
	}

	catch (...)
	{
	}
	return NULL;
}

bool GameServiceManager::GetServiceAttrib(tagGameServiceAttrib& game_service_attrib) {
	game_service_attrib = game_service_attrib_;
	return true;
}

bool GameServiceManager::RectifyParameter(tagGameServiceOption& game_service_option) 
{
	game_service_option_ = game_service_option;
	return true;
}

//创建机器
bool GameServiceManager::SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
	return true;
}

bool GameServiceManager::DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
	return true;
}

//创建窗口
HWND GameServiceManager::CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize)
{
	return NULL;
}
extern "C" __declspec(dllexport) void* CreateGameServiceManager(REFGUID guid, DWORD interface_ver) 
{
	static GameServiceManager game_service_manager;
	return game_service_manager.QueryInterface(guid, interface_ver);
}




