#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
typedef CWHArray<DWORD,DWORD&> CApplyBankerArray;

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�
	int								iTIME_RAND;
	int								iTIME_LESS;

	//��Ϸ����
protected:
	__int64							m_lTurnMaxGold;						//�����ע
	__int64							m_lTurnBasicGold;					//��ע��Ŀ
	__int64							m_lBasicGold;						//��Ԫ��Ŀ
	BYTE							m_bPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	WORD							m_wCurrentUser;						// ��ǰ��ע��
	__int64							m_lShowHandScore;					//������߷�
	bool							m_bShowHand;						//�Ƿ����
	BYTE							m_cbOwnCard[5];						//�Լ����е���
	BYTE							m_byCardCount;                      // ÿ�������з��˼�����

	BYTE							m_cbHumanCard[5];					//�������е���


	__int64                         m_lMeTableGold;				        // �Լ����µ�����
	__int64                         m_lMeTurnGold;				        // �Լ�һ�����µ�����
	__int64                         m_lMeScore;                         // �Լ�Я��������
	bool                            m_bCtrlWinner;                      // �Լ��Ƿ�ΪӮ��
	bool                            m_bFirstUser;                       // �Ƿ�Ϊ��һ����˵��     
	bool							m_bAddGold;							//�Ƿ�����ע
	bool                            m_bWinner;							// �Լ������Ƿ�ΪӮ��
	bool							m_bPoor			;					//�Ƿ��������
	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool RepositionSink();

	//????
public:
	//ʱ����Ϣ
	virtual bool OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//????
	virtual bool OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//????
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//????
public:
	//????
	virtual VOID OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//????
	virtual VOID OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//????
	virtual VOID OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//????
	virtual VOID OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//????
	virtual VOID OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	// ????????
	virtual void OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddGold(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	// ��ע����
	bool OnPlaceJetton(void);
	//д���¼
	void WriteErrorRecord(LPCTSTR pszErrorMsg);
};

//////////////////////////////////////////////////////////////////////////

#endif
