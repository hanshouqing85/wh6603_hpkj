#include "StdAfx.h"
#include "Tableframesink.h"
#include "GameServerManager.h"

//////////////////////////////////////////////////////////////////////////

//ȫ�ֱ���
static CGameServiceManager			g_GameServiceManager;				//�������

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//��������
#ifndef _DEBUG
	#define ANDROID_SERVICE_DLL_NAME	TEXT("SparrowGBAndroidService.dll")	//�������
#else
	#define ANDROID_SERVICE_DLL_NAME	TEXT("SparrowGBAndroidServiceD.dll")	//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager()
{
	//��������
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	////m_GameServiceAttrib.cbJoinInGame=FALSE;
	m_GameServiceAttrib.cbDynamicJoin=FALSE;

	m_GameServiceAttrib.wSupporType=(GAME_GENRE_GOLD|GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_EDUCATE);////////////////
	m_GameServiceAttrib.cbAndroidUser=TRUE;////////////
	m_GameServiceAttrib.cbOffLineTrustee=TRUE;/////////
	m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;/////////
	m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;//////////

	lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));
#ifdef USE_GB
	////lstrcpyn(m_GameServiceAttrib.szDescription,TEXT("CT�齫��Ϸ�������"),CountArray(m_GameServiceAttrib.szDescription));
#ifndef VIDEO_GAME
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPSparrowGBDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("SparrowGB.EXE"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("SparrowGBServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));
#else
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPSparrowGBVDDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("SparrowGBVD.EXE"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("SparrowGBVDServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));
#endif
#else
//	lstrcpyn(m_GameServiceAttrib.sz,TEXT("�����齫��Ϸ�������"),CountArray(m_GameServiceAttrib.szDescription));
#ifndef VIDEO_GAME
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPTREASUREDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("SparrowLY.EXE"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("SparrowLYServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));
#else
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPTREASUREDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientModuleName,TEXT("SparrowHZVD.EXE"),CountArray(m_GameServiceAttrib.szClientModuleName));
	lstrcpyn(m_GameServiceAttrib.szServerModuleName,TEXT("SparrowHZVDServer.DLL"),CountArray(m_GameServiceAttrib.szServerModuleName));
#endif
#endif
	return;
}

//��������
CGameServiceManager::~CGameServiceManager()
{
}

//�ӿڲ�ѯ
void *  CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

////������Ϸ��
//void *  CGameServiceManager::CreateTableFrameSink(const IID & Guid, DWORD dwQueryVer)
//{
//	//��������
//	CTableFrameSink * pTableFrameSink=NULL;
//	try
//	{
//		pTableFrameSink=new CTableFrameSink();
//		if (pTableFrameSink==NULL)
//			throw TEXT("����ʧ��");
//		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
//		if (pObject==NULL) 
//			throw TEXT("�ӿڲ�ѯʧ��");
//		return pObject;
//	}
//	catch (...) {}
//
//	//�������
//	SafeDelete(pTableFrameSink);
//
//	return NULL;
//}
//������Ϸ��
VOID *  CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	//��������
	CTableFrameSink * pTableFrameSink=NULL;
	try
	{
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("����ʧ��");
		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��ȡ����
bool  CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	GameServiceAttrib=m_GameServiceAttrib;
	return true;
}

//�����޸�
bool  CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
	//////Ч�����
	////ASSERT(GameServiceOption!=NULL);
	////if (GameServiceOption==NULL) return false;

	//��Ԫ����
	GameServiceOption.lCellScore=__max(1L,GameServiceOption.lCellScore);

	//��������
	if (GameServiceOption.wServerType!=GAME_GENRE_SCORE)
	{
		GameServiceOption.lMinTableScore=__max(GameServiceOption.lCellScore*20L*2L,GameServiceOption.lMinTableScore);
	}

	//��������
	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore,GameServiceOption.lMinTableScore);
	}

	//�������� lMaxEnterScore
	if (GameServiceOption.wServerType!=GAME_GENRE_SCORE)
	{
		////if(GameServiceOption.lMaxScore<=GameServiceOption.lMinTableScore)
		////	GameServiceOption.lMaxScore=0L;
		////else if(GameServiceOption.lRestrictScore>0)
		////	GameServiceOption.lMaxScore=__min(GameServiceOption.lMaxScore,GameServiceOption->lRestrictScore);
		if(GameServiceOption.lMaxEnterScore<=GameServiceOption.lMinTableScore)
			GameServiceOption.lMaxEnterScore=0L;
		else if(GameServiceOption.lRestrictScore>0)
			GameServiceOption.lMaxEnterScore=__min(GameServiceOption.lMaxEnterScore,GameServiceOption.lRestrictScore);
	}

	return true;
}
//��������
VOID *  CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	try
	{
		//�������
		if( m_AndroidServiceHelper.GetInterface() == NULL )
		{
			////m_AndroidServiceHelper.SetModuleCreateInfo(TEXT("SparrowGBAndroid.dll"),TEXT("CreateAndroidService"));
			m_AndroidServiceHelper.SetModuleCreateInfo(ANDROID_SERVICE_DLL_NAME,"CreateGameServiceManager");

			if( !m_AndroidServiceHelper.CreateInstance() ) throw 0;
		}

		//����������
		VOID *pAndroidObject = m_AndroidServiceHelper->CreateAndroidUserItemSink(Guid,dwQueryVer);
		if( pAndroidObject == NULL ) throw TEXT("����������ʧ��");

		return pAndroidObject;
	}
	catch(...) {}

	return NULL;
}

////////////
void *  CGameServiceManager::CreateGameDataBaseEngineSink(const IID & Guid, DWORD dwQueryVer)
{
	return NULL;
}



//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void *  CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	return g_GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
