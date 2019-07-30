#ifndef GAME_SERVER_MANAGER_HEAD_FILE
#define GAME_SERVER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������������
class CGameServiceManager : public IGameServiceManager
{
	//��������
protected:
	tagGameServiceAttrib				m_GameServiceAttrib;			//��������

	//�������
protected:
	////CAndroidServiceHelper				m_AndroidServiceHelper;			//�����˷���
	CGameServiceManagerHelper				m_AndroidServiceHelper;			//�����˷���

	//��������
public:
	//���캯��
	CGameServiceManager(void);
	//��������
	virtual ~CGameServiceManager(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�ӿں���
public:
	//��������
	//virtual void *  CreateTableFrameSink(const IID & Guid, DWORD dwQueryVer);
	virtual VOID *  CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer);
	//��ȡ����
	virtual bool  GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib);
	//�޸Ĳ���
	virtual bool  RectifyParameter(tagGameServiceOption & GameServiceOption);
	//��������
	virtual VOID *  CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer);

	///////////////////
	virtual void *  CreateGameDataBaseEngineSink(const IID & Guid, DWORD dwQueryVer);

};

//////////////////////////////////////////////////////////////////////////

#endif
