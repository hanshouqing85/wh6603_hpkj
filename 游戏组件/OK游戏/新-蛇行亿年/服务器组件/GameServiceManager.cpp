#include "StdAfx.h"
#include "Resource.h"
#include "TableFrameSink.h"
#include "GameServiceManager.h"
#include "AndroidUserItemSink.h"
//////////////////////////////////////////////////////////////////////////////////

//��������
#ifndef _DEBUG
	#define ANDROID_SERVICE_DLL_NAME	TEXT("sxFishAndroidService.dll")	//�������
#else
	#define ANDROID_SERVICE_DLL_NAME	TEXT("sxFishAndroidServiceD.dll")	//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager()
{
	//�ؼ�����
	m_hDllInstance=NULL;
	m_pDlgCustomRule=NULL;

	//�ں�����
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	m_GameServiceAttrib.wSupporType=(GAME_GENRE_GOLD|GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_EDUCATE);

	//���ܱ�־
	m_GameServiceAttrib.cbDynamicJoin=TRUE;
	m_GameServiceAttrib.cbAndroidUser=TRUE;
	m_GameServiceAttrib.cbOffLineTrustee=FALSE;

	//��������
	m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;
	m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;
	lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("������Ƥ�Ƽ�qq:56248"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("Sxfish.EXE"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("SxServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));

	return;
}

//��������
CGameServiceManager::~CGameServiceManager()
{
	//ɾ������
	SafeDelete(m_pDlgCustomRule);

	//�ͷ� DLL
	if (m_hDllInstance!=NULL)
	{
		AfxFreeLibrary(m_hDllInstance);
		m_hDllInstance=NULL;
	}

	return;
}

//�ӿڲ�ѯ
VOID * CGameServiceManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE(IGameServiceCustomRule,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//��������
VOID * CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	//��������
	CTableFrameSink * pTableFrameSink=NULL;

	try
	{
		//��������
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("����ʧ��");

		//��ѯ�ӿ�
		VOID * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");

		return pObject;
	}
	catch (...) {}

	//ɾ������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��������
VOID * CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	//try
	//{
	//	//����ģ��
	//	if (m_hDllInstance==NULL)
	//	{
	//		m_hDllInstance=AfxLoadLibrary(ANDROID_SERVICE_DLL_NAME);
	//		if (m_hDllInstance==NULL) throw TEXT("�����˷���ģ�鲻����");
	//	}

	//	//Ѱ�Һ���
	//	ModuleCreateProc * CreateProc=(ModuleCreateProc *)GetProcAddress(m_hDllInstance,"CreateAndroidUserItemSink");
	//	if (CreateProc==NULL) throw TEXT("�����˷���ģ��������Ϸ�");

	//	//�������
	//	return CreateProc(Guid,dwQueryVer);
	//}
	//catch(...) {}

    /*
	try
	{
		//�������
		if( m_AndroidServiceHelper.GetInterface() == NULL )
		{
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

	return NULL;*/
	//��������
	CAndroidUserItemSink * pAndroidUserItemSink=NULL;

	try
	{
		//��������
		pAndroidUserItemSink=new CAndroidUserItemSink();
		if (pAndroidUserItemSink==NULL) throw TEXT("����ʧ��");

		//��ѯ�ӿ�
		void * pObject=pAndroidUserItemSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");

		return pObject;
	}
	catch (...) {}

	//ɾ������
	SafeDelete(pAndroidUserItemSink);
	return NULL;
}

//��������
VOID * CGameServiceManager::CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}

//�������
bool CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	//���ñ���
	GameServiceAttrib=m_GameServiceAttrib;

	return true;
}

//��������
bool CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
	//��Ԫ����
	GameServiceOption.lCellScore=__max(1L,GameServiceOption.lCellScore);

	//�����Ϸ
	if (GameServiceOption.wServerType&(GAME_GENRE_GOLD|SCORE_GENRE_POSITIVE))
	{
		tagCustomRule * pCustomRule=(tagCustomRule *)GameServiceOption.cbCustomRule;
		GameServiceOption.lMinTableScore=__max(GameServiceOption.lCellScore*pCustomRule->wMaxScoreTimes,GameServiceOption.lMinTableScore);
	}

	return true;
}

//��ȡ����
bool CGameServiceManager::SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
	
	return true;
}

//Ĭ������
bool CGameServiceManager::DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
	//��������


	return true;
}

//��������
HWND CGameServiceManager::CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize)
{


	return  NULL;
}

//////////////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) VOID * CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	static CGameServiceManager GameServiceManager;
	return GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////////////
