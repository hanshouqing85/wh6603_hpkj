
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

// �ӿڲ�ѯ
void*  CGameServiceManager::QueryInterface(REFGUID guid, DWORD query_ver) {
  QUERYINTERFACE(IGameServiceManager, guid, query_ver);
  QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager, guid, query_ver);
  return NULL;
}

// ��������
void* CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
  CAndroidUserItemSink* pAndroidUserItemSink = NULL;

	try
	{
		//��������
		pAndroidUserItemSink=new CAndroidUserItemSink();
		if (pAndroidUserItemSink==NULL) throw TEXT("����ʧ��");

		//��ѯ�ӿ�
		VOID * pObject=pAndroidUserItemSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");

		return pObject;
	}
	catch (...) {}

	//ɾ������
	SafeDelete(pAndroidUserItemSink);

	return NULL;
}

//������Ϸ��
VOID *  CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}
//��������
VOID * CGameServiceManager::CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}

//��ȡ����
bool  CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	return true;
}

//�����޸�
bool  CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
	//Ч�����
	ASSERT(&GameServiceOption!=NULL);
	if (&GameServiceOption==NULL) return false;

	//��Ԫ����
	GameServiceOption.lCellScore -=__max(1L,GameServiceOption.lCellScore);

	//��������
	if (GameServiceOption.wServerType==GAME_GENRE_GOLD&&GameServiceOption.wServerType!=0)
	{
		GameServiceOption.lMinTableScore=__max(1000L,GameServiceOption.lMinTableScore);
	}

	//��������
	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore,GameServiceOption.lMinTableScore);
	}

	return true;
}
//��������
DECLARE_CREATE_MODULE(GameServiceManager)
