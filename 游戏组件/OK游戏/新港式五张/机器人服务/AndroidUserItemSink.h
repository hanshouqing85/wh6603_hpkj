#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
struct OtherInfo
{
	BYTE    cbCard[5];
	bool    bGiveUp;
	bool    bStatue;
};

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	SCORE							m_lCellScore;						//��Ϸ��ע
	SCORE						m_lUserScore[GAME_PLAYER];			//�û���ע
	SCORE						m_lTableScore[GAME_PLAYER];			//������ע
    WORD							m_wCurrentUser;						//��ǰ�û�
	bool                            m_bFirstChip;                       //������ע
	SCORE						m_lTurnLessScore;					//��С��ע
	SCORE						m_lDrawMaxScore;					//�����ע
	SCORE						m_lTurnMaxScore;					//�����ע
	int                             m_nCurCardCount;                    //��ǰ����
	bool                            m_bGiveUp;                          //�Ƿ����
	bool							m_bWillWin;							//���ƽ��
	BYTE							m_cbTableCardData[GAME_PLAYER][5];	//�����˿�
	bool							m_bShowHand;						//�����־
	WORD							m_wSpeciaChairID;					//�����û�		

	SCORE						m_lBeforeUserScore;					//��һ���û���ע
	//�˿˱���
protected:
	
	//�����˿�
protected:
	BYTE                           m_cbHandCard[5];                     //�����˿�
	OtherInfo                      m_cbOtherCard[5];                    //��������Ϣ
	bool                           m_bAddChip; 
	bool						   m_bTemp;								//����

	//��ʷ�˿�
protected:

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�

	//���в���
	LONGLONG						m_lRobotBankGetScore;					//ȡ����С����(ȡ�����Ǹ�ֵ���ϵ�һ�������)
	LONGLONG						m_lRobotBankGetScoreBanker;			//ȡ���������(����ֵһ��Ҫ����RobotBankGet)
	int								m_nRobotBankStorageMul;				//����
	LONGLONG						m_lBankerLostScore;							//ׯ����� (��������Ϊׯ)
	LONGLONG						m_lRobotScoreRange[2];					//���Χ

	TCHAR							m_szRoomName[32];					//���÷���
	

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

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual VOID OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual VOID OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û���λ
	virtual VOID OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(VOID * pData, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(VOID * pData, WORD wDataSize);
	//�����˿�
	bool OnSubSendCard(VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(VOID * pData, WORD wDataSize);
	//���
	void SendSuoHand();

	//���ܺ���
protected:
	//��ȡ��ע���
	LONGLONG GetAddChip();
	//����Ӯ��
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
	//Ӯ���Ƿ�Ϊ������
	bool WinIsAndroid(BYTE cbStartPos, BYTE cbConcludePos);
	//����
	bool GaiLv(BYTE bNum);
	//��ע
	void FllowScore();
	//��ע
	void AddScore();
	//��ע
	void OnSubAddScoreEx();
	//����
	void GiveUpScore();

	//��ȡ����
	void ReadConfigInformation();

};

//////////////////////////////////////////////////////////////////////////

#endif