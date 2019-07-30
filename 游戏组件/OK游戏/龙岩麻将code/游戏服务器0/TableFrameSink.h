#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "datastream.h"

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//Ч������
enum enEstimatKind
{
	EstimatKind_OutCard,			//����Ч��
	EstimatKind_GangCard,			//����Ч��
	EstimatKind_PengCard,			//����У��
};


enum enGangState
{
	STATE_NULL,
	STATE_GANG,
	STATE_BU

};

//////////////////////////////////////////////////////////////////////////
//���Ƶ÷�
struct tagGangScore
{
	bool		bMingGang;							//�ܸ���
	BYTE		cbGangCount;							//�ܸ���
	int			lScore[MAX_WEAVE][GAME_PLAYER];			//ÿ���ܵ÷�
};
//////////////////////////////////////////////////////////////////////////

struct  HNMJGameRecordPlayer
{
	DWORD dwUserID;
	std::string kHead;
	std::string kNickName;
	std::vector<BYTE> cbCardData;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwUserID);
		Stream_VALUE(kHead);
		Stream_VALUE(kNickName);
		Stream_VECTOR(cbCardData);
	}
};

struct  HNMJGameRecordOperateResult
{
	enum Type
	{
		TYPE_NULL,
		TYPE_OperateResult,
		TYPE_SendCard,
		TYPE_OutCard,
		TYPE_ChiHu,
	};
	HNMJGameRecordOperateResult()
	{
		cbActionType = 0;
		wOperateUser = 0;
		wProvideUser = 0;
		cbOperateCode = 0;
		cbOperateCard = 0;
	}
	BYTE							cbActionType;
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	DWORD							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbActionType);
		Stream_VALUE(wOperateUser);
		Stream_VALUE(wProvideUser);
		Stream_VALUE(cbOperateCode);
		Stream_VALUE(cbOperateCard);
	}
};
struct  HNMJGameRecord
{
	DWORD dwKindID;
	DWORD dwVersion;
	std::vector<HNMJGameRecordPlayer> kPlayers;
	std::vector<HNMJGameRecordOperateResult> kAction;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(HNMJGameRecordPlayer,kPlayers);
		StructVecotrMember(HNMJGameRecordOperateResult,kAction);
		Stream_VALUE(dwKindID);
		Stream_VALUE(dwVersion);
	}

};

struct MasterHandCardInfo
{
	int nChairId;
	std::vector<BYTE>    kMasterHandCard;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(nChairId);
		Stream_VECTOR(kMasterHandCard);
	}
};

struct MasterHandCard
{
	std::vector<MasterHandCardInfo>    kMasterHandCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(MasterHandCardInfo,kMasterHandCardList);
	}
};


//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	HNMJGameRecord					m_kGameRecord;
	DWORD							m_dwGameRuleIdex;						//��Ϸ����
	BYTE                            m_cbMasterCheckCard[GAME_PLAYER];
	TCHAR							m_szPath[MAX_PATH];
	LONG							m_lSiceCount;							//���ӵ���
	WORD							m_wBankerUser;							//ׯ���û�
	int								m_lGameScore[GAME_PLAYER];				//��Ϸ�÷�
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�û��˿�
	bool							m_bTrustee[GAME_PLAYER];				//�Ƿ��й�
	BYTE							m_cbListenStatus[GAME_PLAYER];			//����״̬
	BYTE							m_cbQuanFeng;							//Ȧ��
	BYTE							m_cbQuanCounter;						//Ȧ�����
	BYTE							m_cbHuaCardCount[GAME_PLAYER];			//���Ƹ���
	WORD							m_wReplaceUser;							//�����û�

	WORD							m_lStartHuScore;						//���
	BYTE							m_cbChangeCard;							//������CardIndex
	BYTE							m_cbLandCount;							//ׯ�Ҵ���
	WORD							m_wFirstUser;							//�����û�
	WORD							m_wLandSice;							//ׯ������
	WORD							m_wCaiShenIndex;						//����λ�ã���ɫ�ӵ���֮��ȷ��
	WORD							m_wCaiShenChairID;						//�����ڶ������е�λ��
	WORD							m_wCaiShenSice;							//�������

	//������Ϣ
protected:
	WORD							m_wOutCardUser;							//�����û�
	BYTE							m_cbOutCardData;						//�����˿�
	BYTE							m_cbOutCardCount;						//������Ŀ
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//�����ƶ�����Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][60];		//������¼
	BYTE							m_cbCaiShenOutCnt[GAME_PLAYER];			//������ȥ�ĸ���
	BYTE							m_cbHuangZhuang;						//��ׯ
	//������Ϣ
protected:
	BYTE							m_cbSendCardData;						//�����˿�
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbLeftCardCount;						//ʣ����Ŀ
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//����˿�
	BYTE							m_cbEndLeftCount;						//��ׯ����

	//��������
protected:
	WORD							m_wHeapHead;							//����ͷ��
	WORD							m_wHeapTail;							//����β��
	BYTE							m_cbHeapCardInfo[GAME_PLAYER][2];		//������Ϣ

	//���б���
protected:
	WORD							m_wResumeUser;							//��ԭ�û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wProvideUser;							//��Ӧ�û�
	BYTE							m_cbProvideCard;						//��Ӧ�˿�

	//״̬����
protected:
	bool							m_bSendStatus;							//����״̬
	bool							m_bGangStatus;							//����״̬
	bool							m_bEnjoinChiHu[GAME_PLAYER];			//��ֹ�Ժ�
	bool							m_bEnjoinChiPeng[GAME_PLAYER];			//��ֹ����
	BYTE							m_cbUserMakeEye[GAME_PLAYER];
	bool							m_bOutMagicCard[GAME_PLAYER];			//�������//*
	BYTE							m_cbMagicCardUser;						//����������

	bool							m_bOutGangHuCard[GAME_PLAYER];			//��ܺ���//*
	BYTE							m_cbEnjoinHuCard[GAME_PLAYER][(MAX_COUNT-1)/2];//��ֹ����

	//�û�״̬
public:
	bool							m_bResponse[GAME_PLAYER];				//��Ӧ��־
	BYTE							m_cbUserAction[GAME_PLAYER];			//�û�����
	BYTE							m_cbOperateCard[GAME_PLAYER][3];		//�����˿�
	BYTE							m_cbPerformAction[GAME_PLAYER];			//ִ�ж���
	tagGangScore					m_GangScore[GAME_PLAYER];				//
	//����˿�
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];			//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//����˿�

	//������Ϣ
protected:
	BYTE							m_cbChiHuCard;							//�Ժ��˿�
	DWORD							m_dwChiHuKind[GAME_PLAYER];				//�Ժ����
	CChiHuRight						m_ChiHuRight[GAME_PLAYER];				//�Ժ�Ȩλ

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��Ϣ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){  return 0; };
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){ return 0; };
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:

	void setActionTimeStart(bool bStart,bool Trustee);
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize) { return false; }
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) { return false; }

	//����ӿ�
public:
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize) { return true; }

	virtual void SetPrivateInfo(BYTE bGameTypeIdex,DWORD bGameRuleIdex);
	virtual void SetCreateUserID(DWORD	dwUserID);
	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserListenCard(WORD wChairID,BYTE cbListen);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	bool CanQiangGangHu();
	//�û�����
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3]);
	//�û�����
	bool OnUserReplaceCard(WORD wChairID, BYTE cbCardData);

public:
	bool hasRule(BYTE cbRule);
	bool isHZGuiZe();
	DWORD AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
	DWORD AnalyseChiHuCardZZ(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSendCard);
	DWORD AnalyseChiHuCardCS(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSendCard);
	DWORD AnalyseChiHuCardCS_XIAOHU(const BYTE cbCardIndex[MAX_INDEX], CChiHuRight &ChiHuRight);

	//��������
protected:
	//��̨����
	bool GetPrivateProfile();
	//���Ͳ���
	bool SendOperateNotify();
	//�ɷ��˿�
	bool DispatchCardData(WORD wCurrentUser,bool bTail=false);

	//��Ӧ�ж�
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
	//���
	BYTE CalScore( WORD wWinner, int lScore[GAME_PLAYER] );
	//��ϵͳ���ƻ�ȡ��������
	BYTE GetMagicIndex( BYTE cbFanCard );
	//Ȩλ����
	void FiltrateRight( WORD wWinner, CChiHuRight &chr );
	
	void clearUserActionStates(WORD cbChairID = INVALID_CHAIR);
	bool isUseTuoGuan();
	//�����й�
	void setChairTrustee(WORD iChairID,bool bTrustee);

	struct UserActionStates 
	{
		bool							m_bResponse[GAME_PLAYER];				//��Ӧ��־
		DWORD							m_cbUserAction[GAME_PLAYER];			//�û�����
		BYTE							m_cbOperateCard[GAME_PLAYER];			//�����˿�
		DWORD							m_cbPerformAction[GAME_PLAYER];			//ִ�ж���
		CChiHuRight						m_ChiHuRight[GAME_PLAYER];				//
		BYTE							m_cbProvideCard;						//��Ӧ�˿�
		BYTE							m_cbOutCardData;						//�����˿�
		BYTE							m_cbSendCardData;						//�����˿�
	};
	UserActionStates					m_kUserActionStates1;
	UserActionStates					m_kUserActionStates2;

	void starGameRecord();
	void addGameRecordAction(CMD_S_OperateResult kNetInfo);
	void addGameRecordAction(CMD_S_SendCard kNetInfo);
	void addGameRecordAction(CMD_S_OutCard kNetInfo);
	void addGameRecordAction(CMD_S_ChiHu kNetInfo);
public:
	void sendMasterHandCard();
	void sendMasterLeftCard(int nChairldID);
	BYTE getSendCardData(WORD dwSendUser);
//	void getXiaoHuCard(WORD nChairldID,CMD_S_XiaoHu& kXiaoHu);
	int  getLeftCardNum();
	// ȥ��һ����������
	int RemoveOneMagicCard(BYTE cbMagicCard);

};

//////////////////////////////////////////////////////////////////////////

#endif
