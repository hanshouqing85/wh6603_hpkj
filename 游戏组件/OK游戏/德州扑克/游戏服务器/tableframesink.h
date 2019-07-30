#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////



//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��ұ���
protected:
	WORD							m_wDUser;								//D���
	WORD							m_wCurrentUser;							//��ǰ���

	//���״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
    BYTE							m_cbGiveUpFlag[GAME_PLAYER];			//���Ʊ�־<by hxh>

	//��ע��Ϣ
protected:
	DZPKSCORE							m_lCellScore;							//��Ԫ��ע
	DZPKSCORE							m_lTurnLessScore;						//��С��ע
	DZPKSCORE							m_lAddLessScore;						//����Сע
	DZPKSCORE							m_lTurnMaxScore;						//�����ע
	DZPKSCORE							m_lBalanceScore;						//ƽ����ע
	WORD							    m_wOperaCount;							//��������
	BYTE							    m_cbBalanceCount;						//ƽ�����
	DZPKSCORE							m_lTableScore[GAME_PLAYER];				//������ע
	DZPKSCORE							m_lTotalScore[GAME_PLAYER];				//�ۼ���ע
	DZPKSCORE							m_lUserMaxScore[GAME_PLAYER];			//�����ע
	BYTE							    m_cbShowHand[GAME_PLAYER];				//����û�

	//˰�ձ���
protected:
	//LONGLONG						m_bUserTax[ GAME_PLAYER];				//�û�˰��,�����ô������,�ɲ��ñ���
	//LONGLONG						m_bLastTax[ GAME_PLAYER];				//���˰��

	//�˿���Ϣ
protected:
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbCenterCardData[MAX_CENTERCOUNT];	//�����˿�
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);


	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//��ѯ�����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore) { }

//�����¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) {return true; };
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }


	//��Ϣ�¼�
protected:
	//�����¼�
	bool OnUserGiveUp(WORD wChairID,bool bLeft=false);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, DZPKSCORE lScore, bool bGiveUp,BYTE cbJudgeAction);

	// add by hxh 20161104
	void SendAddScorePacket(CMD_S_AddScore *pAddScore);
};

//////////////////////////////////////////////////////////////////////////

#endif
