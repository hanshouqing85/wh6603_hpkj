#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include <map>
//////////////////////////////////////////////////////////////////////////

//����ԭ��
#define GER_NO_PLAYER						0x10						//û�����
#define USER_LEVEL                          3                            //��Ϸ�ȼ�
#define USER_LEVEL_ZERO                     0                            //�޵еȼ�
#define USER_LEVEL_FIRST                    1                           //����ȼ�
#define USER_LEVEL_SECOND                   2                           //�׸��ȼ�
//����ṹ
struct CMD_GF_UserChat
{
	WORD								wChatLength;					//��Ϣ����
	COLORREF							crFontColor;					//��Ϣ��ɫ
	DWORD								dwSendUserID;					//�����û�
	DWORD								dwTargetUserID;					//Ŀ���û�
	TCHAR								szChatMessage[1024];	//������Ϣ
};


typedef struct tagPersonInfo
{
	DWORD dwUserID;           // ���ID
	int iPercent;             // ʤ��
}S_PERSONINFO;
typedef CWHArray<S_PERSONINFO,S_PERSONINFO&> CPensonInfoArray;

struct tagSingleUserCtrl
{
	BYTE							cbCheatUser;
	DWORD							dwUserID;
	WORD							wChairID;
	BYTE							cbWinLosePercent;
	BYTE							cbLoseWin;
	LONGLONG						lCtrlScore;
};
#define MAX_CTRL_USER_COUNT 10

typedef CWHArray<tagSingleUserCtrl*> CSingleUserCtrlArray;
//////////////////////////////////////////////////////////////////////////

struct tagChageInfo 
{
	WORD wChairID;
	BYTE cbCardData;
};
//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	std::map<DWORD,__int64>			m_UserLevel;

	WORD							m_wOperaCount;							//��������
	WORD							m_wCurrentUser;							//��ǰ�û�
	BYTE							m_bSendCardCount;						//������Ŀ
	BYTE							m_cbCardCount[GAME_PLAYER];				//�˿���Ŀ
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];		//�����˿�
	BYTE							m_cbGameEndReason;						//��Ϸ��������
	bool							m_bGameEnd;								//
	bool							m_bShowHand;							//
	LONG							m_lSessionID;

	bool							m_bPoor;								//�Ƿ��޵����¼���
	BYTE							m_cbCardArray[28];							//�˿��б�
	INT								m_nCardIndex;							//�˿�����
	//�����Ϣ
protected:
	DOUBLE							m_lMaxGold;								//�����ע
	DOUBLE							m_lTurnMaxGold;							//
	DOUBLE							m_lBasicGold;							//��Ԫ��ע
	DOUBLE                         m_lMaxShowhandGold;                     // ���ʱ�������
	DOUBLE							m_lTurnBasicGold;						//������ע
	DOUBLE							m_lTableGold[2*GAME_PLAYER];			//��ע���
	DOUBLE							m_lUserScore[GAME_PLAYER];				//
    DOUBLE                         m_lUserLevel[USER_LEVEL];                         //��Ҽ������

	//��������
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	WORD                            m_wCtrlWinner;                          // Ӯ�����
	CPensonInfoArray                m_arPensonInfo;
	CSingleUserCtrlArray			m_SingleUserArray;						//������ҿ���
	tagSingleUserCtrl*				m_tagSingleUserCtrl;					//����ҿ���
	DWORD							m_dwSingleCtrlUser;						//�ܿصĵ������
	bool							m_bCtrlWin;								//�ܿصĵ������

	//���Ʊ���
protected:
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�

	__int64							m_nAllTrunsGold;						//�ܽ����
	__int64							m_nAIWinTrunGold;						//������Ӯ���ܽ����
	float							m_fAIWinPercent;					//�ο�ʧ����
	float							m_fAILostPercent;						//�ο�����

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption *			m_pGameServiceOption;					//��Ϸ����
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//��Ϸ����

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
//	static const enStartMode        m_GameStartMode;						//��ʼģʽ
	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	////��ʼģʽ
	//enStartMode  CTableFrameSink::GetGameStartMode();
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
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize){return true;}

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser){return true;}
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser){return true;}
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//��ѯ�����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore) { }


	bool OnSubSupperApplay(const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��Ϸ�¼�
protected:
	//�����¼�
	bool OnUserGiveUp(WORD wChairID);
	//��ע�¼�
	bool OnUserAddGold(WORD wChairID, __int64 lGold, bool bCancel,bool bFllow);
	//��Ϸ����
protected:
	//�ƶ�ʤ��
	//��Ϸ״̬
	bool IsUserPlaying(WORD wChairID);
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
	WORD EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos);

	//ͳ�ƻ�����Ӯ��
	void CountAIWinTruns(const CMD_S_GameEnd &GameEnd);

	bool IsControlUser(DWORD dwUserID);
	bool WriteSingleUserScore(DWORD dwUserID,LONGLONG lWinScore,bool bWin);

};

//////////////////////////////////////////////////////////////////////////

#endif

