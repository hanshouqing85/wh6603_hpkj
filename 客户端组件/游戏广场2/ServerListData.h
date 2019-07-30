#ifndef SERVER_LIST_DATA_HEAD_FILE
#define SERVER_LIST_DATA_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//��������
enum enItemGenre
{
	ItemGenre_Type,					//��Ϸ����
	ItemGenre_Kind,					//��Ϸ����
	ItemGenre_Server,				//��Ϸ����
};

//����״̬
enum enServerStatus
{
	ServerStatus_Normal,			//����״̬
	ServerStatus_Entrance,			//����ʹ��
	ServerStatus_EverEntrance,		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CGameListItem;
class CGameTypeItem;
class CGameKindItem;
class CGameServerItem;

//����˵��
typedef CWHArray<CGameListItem *> CGameListItemArray;
typedef CWHArray<CGameTypeItem *> CGameTypeItemArray;
typedef CWHArray<CGameKindItem *> CGameKindItemArray;
typedef CWHArray<CGameServerItem *> CGameServerItemArray;

//����˵��
typedef CMap<WORD,WORD,CGameTypeItem *,CGameTypeItem * &> CGameTypeItemMap;
typedef CMap<WORD,WORD,CGameKindItem *,CGameKindItem * &> CGameKindItemMap;
typedef CMap<WORD,WORD,CGameServerItem *,CGameServerItem * &> CGameServerItemMap;

//////////////////////////////////////////////////////////////////////////////////

//�б�ӿ�
interface IServerListDataSink
{
	//״̬֪ͨ
public:
	//���֪ͨ
	virtual VOID OnGameItemFinish()=NULL;
	//���֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID)=NULL;

	//����֪ͨ
public:
	//����֪ͨ
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem)=NULL;
	//����֪ͨ
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem)=NULL;
	//ɾ��֪ͨ
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�б�����
class CGameListItem
{
	//��Ԫ����
	friend class CServerListData;

	//��������
protected:
	enItemGenre						m_ItemGenre;						//��������
	CGameListItem *					m_pParentListItem;					//��������

	//��������
protected:
	//���캯��
	CGameListItem(enItemGenre ItemGenre);
	//��������
	virtual ~CGameListItem();

	//���ܺ���
public:
	//��ȡ����
	enItemGenre GetItemGenre() { return m_ItemGenre; }
	//��ȡ����
	CGameListItem * GetParentListItem() { return m_pParentListItem; }

	//���غ���
public:
	//��������
	virtual WORD GetSortID()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//����ṹ
class CGameTypeItem : public CGameListItem
{
	//��������
public:
	tagGameType						m_GameType;							//������Ϣ

	//��������
public:
	//���캯��
	CGameTypeItem();
	//��������
	virtual ~CGameTypeItem();

	//���غ���
public:
	//��������
	virtual WORD GetSortID() { return m_GameType.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//���ͽṹ
class CGameKindItem : public CGameListItem
{
	//��������
public:
	tagGameKind						m_GameKind;							//������Ϣ

	//���±���
public:
	bool							m_bUpdateItem;						//���±�־
	DWORD							m_dwUpdateTime;						//����ʱ��

	//��չ����
public:
	DWORD							m_dwProcessVersion;					//��Ϸ�汾

	//��������
public:
	//���캯��
	CGameKindItem();
	//��������
	virtual ~CGameKindItem();

	//���غ���
public:
	//��������
	virtual WORD GetSortID() { return m_GameKind.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//����ṹ
class CGameServerItem : public CGameListItem
{
	//��������
public:
	tagGameServer					m_GameServer;						//������Ϣ

	//��չ����
public:
	enServerStatus					m_ServerStatus;						//����״̬

	//��������
public:
	CGameKindItem *					m_pGameKindItem;					//��Ϸ����

	//��������
public:
	//���캯��
	CGameServerItem();
	//��������
	virtual ~CGameServerItem();

	//���غ���
public:
	//��������
	virtual WORD GetSortID() { return m_GameServer.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//�б����
class CServerListData
{
	//��������
protected:
	CGameTypeItemMap				m_GameTypeItemMap;					//��������
	CGameKindItemMap				m_GameKindItemMap;					//��������
	CGameServerItemMap				m_GameServerItemMap;				//��������

	//�ں˱���
protected:
	CGameListItemArray				m_GameListItemWait;					//�ȴ�����
	IServerListDataSink *			m_pIServerListDataSink;				//�ص��ӿ�

	//��̬����
protected:
	static CServerListData *		m_pServerListData;					//�б�����

	//��������
public:
	//���캯��
	CServerListData();
	//��������
	virtual ~CServerListData();

	//���ú���
public:
	//���ýӿ�
	VOID SetServerListDataSink(IServerListDataSink * pIServerListDataSink);

	//״̬֪ͨ
public:
	//���֪ͨ
	VOID OnEventListFinish();
	//���֪ͨ
	VOID OnEventKindFinish(WORD wKindID);
	//����֪ͨ
	VOID OnEventDownLoadFinish(WORD wKindID);

	//��������
public:
	//��������
	VOID SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount);
	//��������
	VOID SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount);

	//���뺯��
public:
	//��������
	bool InsertGameType(tagGameType * pGameType);
	//��������
	bool InsertGameKind(tagGameKind * pGameKind);
	//���뷿��
	bool InsertGameServer(tagGameServer * pGameServer);

	//ɾ������
public:
	//ɾ������
	bool DeleteGameType(WORD wTypeID);
	//ɾ������
	bool DeleteGameKind(WORD wKindID);
	//ɾ������
	bool DeleteGameServer(WORD wServerID);

	//ö�ٺ���
public:
	//ö������
	CGameTypeItem * EmunGameTypeItem(POSITION & Pos);
	//ö������
	CGameKindItem * EmunGameKindItem(POSITION & Pos);
	//ö�ٷ���
	CGameServerItem * EmunGameServerItem(POSITION & Pos);

	//���Һ���
public:
	//��������
	CGameTypeItem * SearchGameType(WORD wTypeID);
	//��������
	CGameKindItem * SearchGameKind(WORD wKindID);
	//���ҷ���
	CGameServerItem * SearchGameServer(WORD wServerID);

	//��Ŀ����
public:
	//������Ŀ
	DWORD GetGameTypeCount() { return (DWORD)m_GameTypeItemMap.GetCount(); }
	//������Ŀ
	DWORD GetGameKindCount() { return (DWORD)m_GameKindItemMap.GetCount(); }
	//������Ŀ
	DWORD GetGameServerCount() { return (DWORD)m_GameServerItemMap.GetCount(); }

	//��̬����
public:
	//��ȡ����
	static CServerListData * GetInstance() { return m_pServerListData; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif
