#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"


//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//Ч������
enum enEstimatKind
{
	EstimatKind_OutCard,			//����Ч��
	EstimatKind_GangCard,			//����Ч��
};



//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wDoorDiceCount;						//�������ӵ���
	WORD							m_wGoldDiceCount;						//�������ӵ���
	WORD							m_wBankerUser;							//ׯ���û�
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�û��齫
	//BYTE							m_cbDisPengIndex[GAME_PLAYER][MAX_INDEX];//�����еĳ���
	//BYTE							m_cbDisHuIndex[GAME_PLAYER][MAX_INDEX];	//�����еĳ���
	enGoldSwim						m_GoldSwim;								//����״̬
	bool							m_bGameStart;							//��ʼ��־
	enGoldSwim						m_UserGoldSwim[GAME_PLAYER];			//����״̬
	BYTE							m_cbCurCardIndex[GAME_PLAYER][MAX_INDEX];//����˿� 



	//������Ϣ
protected:
	WORD							m_wOutCardUser;							//�����û�
	BYTE							m_cbOutCardData;						//�����齫
	BYTE							m_cbOutCardCount;						//������Ŀ
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];		//������¼
	BYTE							m_cbMaxLeftCount;						//��������

	

	//������Ϣ
protected:
	BYTE							m_cbSendCardData;						//�����齫
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbLeftCardCount;						//ʣ���齫
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//����齫
	BYTE							m_cbGoldCard[4];						//�����齫
	BYTE							m_cbRepairFlowerPos;					//����λ��
	BYTE							m_cbFlowerCard[8];						//�����齫
	BYTE							m_cbFlowerCount[GAME_PLAYER];			//�������
	BYTE							m_cbGangCount[GAME_PLAYER];				//���Ʒ���
	BYTE							m_cbGoldCount[GAME_PLAYER];				//���Ƹ���
	BYTE							m_cbDoorCard;							//�����齫


	//���б���
protected:
	WORD							m_wResumeUser;							//��ԭ�û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wProvideUser;							//��Ӧ�û�
	BYTE							m_cbProvideCard;						//��Ӧ�齫
	bool							m_bHuUser[4];							//�����û�

	

	//״̬����
protected:
	bool							m_bSendStatus;							//����״̬
	bool							m_bGangStatus;							//���Ϻ�״̬
	bool							m_bEnjoinChiHu[GAME_PLAYER];			//��ֹ�Ժ�
	bool							m_bEnjoinChiPeng[GAME_PLAYER];			//��ֹ����
	BYTE							m_bGoOnBankerTime;						//��ׯ����

	
	
	

	//�û�״̬
public:
	bool							m_bResponse[GAME_PLAYER];				//��Ӧ��־
	BYTE							m_cbUserAction[GAME_PLAYER];			//�û�����
	BYTE							m_cbOperateCard[GAME_PLAYER];			//�����齫
	BYTE							m_cbPerformAction[GAME_PLAYER];			//ִ�ж���
	LONGLONG						m_lUserCapital[GAME_PLAYER];			//����ʱ�
	

	//����齫
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][5];		//����齫

	//������Ϣ
protected:
	BYTE							m_cbChiHuCard;							//�Ժ��齫
	tagChiHuResult					m_ChiHuResult[GAME_PLAYER];				//�Ժ����

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	//static const enStartMode		m_GameStartMode;						//��ʼģʽ
	//test
	char							*msg;									//������Ϣ


	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() {}
	//�Ƿ���Ч
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void  RepositionSink();

	//��Ϣ�ӿ�
public:
	////��ʼģʽ
	//virtual enStartMode  GetGameStartMode();
	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart();
	//��Ϸ����
	virtual bool  OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool  OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool  OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	//virtual bool  OnActionUserReConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool  OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool  OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool  OnActionUserReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);

	//��������
protected:
	//���Ͳ���
	bool SendOperateNotify();
	//�ɷ��齫
	bool DispatchCardData(WORD wCurrentUser,bool bRepairFlower);
	//��Ӧ�ж�
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);	
	//����״̬
	enGoldSwim GetCurrentGoldSwim();
	//����״̬
	enGoldSwim GetUserGoldSwim(WORD wChairID);
	
public:
	// ������Ϣ
	bool OnUserOpenDoor();
	// ������Ϣ
	bool OnUserOpenGold();	
	// ������Ϣ
	bool OnUserRepairFlower(const void *pBuffer);
	// ��ׯ��Ϻ��ж��Ƿ���Ҫ����<by hxh>
	bool OnRepairFlowerStage(WORD wChairID,bool bNeed);
    // ����ͷ��һ�Ų��ǻ��ƵĲ�����<by hxh>
	BYTE FindMagicCard(WORD *pOut);
	// ����ͷȥ��һ�������ƣ�ʣ���齫��Ŀ��1<by hxh>
	bool RemoveOneMagicCard(BYTE cbMagicCard);
	///////////////////////////
public:
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	virtual void SetGameBaseScore(LONG lBaseScore);
	virtual bool OnActionUserOnReady(WORD,IServerUserItem *,void *,WORD);
};

//////////////////////////////////////////////////////////////////////////

#endif