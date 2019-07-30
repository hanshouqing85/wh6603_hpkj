#ifndef ANDROID_SERVICE_HEAD_FILE
#define ANDROID_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������������
////class CAndroidService : public IAndroidService
class CGameServiceManager : public IGameServiceManager
{
	//��������
public:
	//���캯��
	////CAndroidService(void);
	CGameServiceManager(void);
	//��������
	////virtual ~CAndroidService(void);
	virtual ~CGameServiceManager(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//����������
	virtual VOID*  CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer);



	//�����ӿ�
public:
	//��������
	virtual VOID * CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer);
	//��������
	virtual VOID * CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer);

	//�����ӿ�
public:
	//�������
	virtual bool GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib);
	//��������
	virtual bool RectifyParameter(tagGameServiceOption & GameServiceOption);

};

//////////////////////////////////////////////////////////////////////////

#endif
