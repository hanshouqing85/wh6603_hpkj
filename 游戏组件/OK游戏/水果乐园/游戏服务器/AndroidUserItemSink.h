#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"


////////////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
protected:
	LONGLONG							m_lAreaLimitScore;					//��������
	LONGLONG							m_lUserLimitScore;					//��������
	LONGLONG							m_lApplyBankerCondition;			//��������

	LONGLONG							m_lUserJettonScore[JETTON_AREA_COUNT];	//������ע
	LONGLONG							m_lAllJettonScore[JETTON_AREA_COUNT];	//ȫ����ע

	LONGLONG							m_lMeMaxScore;						//�����ע
	//ׯ����Ϣ
protected:
	LONGLONG							m_lBankerScore;						//ׯ�һ���
	LONGLONG							m_lBankerWinScore;					//ׯ�ҳɼ�
	WORD								m_wBankerUser;						//��ǰׯ��
	WORD								m_wBankerTime;						//��ׯ����

	bool								m_bEnableSysBanker;					//ϵͳ��ׯ

protected:
	BYTE								m_cbJettonArea;
	WORD								m_wJettonCount;

	bool								m_bMeCurrentBanker;
	bool								m_bApplyingBanker;
	bool								m_bCancelingBanker;
	WORD								m_wRandBankerTime;
	static int							m_stlApplyBanker;					//������
	DWORD								m_dwJettonRate;						//�������

	//�����ļ�
protected:
	LONGLONG						    m_lRobotJettonLimit[2];				//��������	
	BOOL								m_bAllowApplyBanker;				//�������Ƿ����������ׯ
	WORD								m_wMaxBankerTime;					//��������ׯ����
	int									m_nJettonRange;						//��������ע��Χ
	int									m_nRobotApplyBanker;				//��ׯ����

	TCHAR								m_szConfigFileName[MAX_PATH];		//�����ļ�
	TCHAR								m_szRoomName[32];					//���÷���

	IAndroidUserItem *					m_pIAndroidUserItem;				//�û��ӿ�

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CAndroidUserItemSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool RepositionSink();

public:
	//��ʼ�ӿ�
	virtual bool  InitUserItemSink(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool  RepositUserItemSink();

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual void OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual void OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual void OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û���λ
	virtual void OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//������Ҷ���ʼ��
	//virtual void OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//��Ϣ����
public:
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);

	//�����ע
	LONGLONG GetUserMaxJetton();
	//��ȡ�����ļ�
	void ReadConfigInformation(bool bFirstRead);
};

////////////////////////////////////////////////////////////////////////////////
#endif
