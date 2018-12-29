#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//ʤ�ʿ����˺���Ϣ
struct ControlAccount	
{
	DWORD dwUserID;
	int nOdds;
};

//��ʼģʽ
enum enStartMode
{
	enStartMode_FullReady,				//���˿�ʼ
	enStartMode_AllReady,				//����׼��
	enStartMode_Symmetry,				//�Գƿ�ʼ
	enStartMode_TimeControl,			//ʱ�����
};

//��������
enum enScoreKind
{
	enScoreKind_Win,					//ʤ
	enScoreKind_Lost,					//��
	enScoreKind_Draw,					//��
	enScoreKind_Flee,					//��
	enScoreKind_Service,				//����
	enScoreKind_Present,				//����
};

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//����ע��
protected:
	__int64							m_lTotalJettonScore[JETTON_AREA_COUNT];				//��������ע�ܶ����飻������ҵ���ע
	__int64							m_lUserAreaScore[GAME_PLAYER][JETTON_AREA_COUNT];	//���������������ע������
	__int64							m_lAreaScoreLimit[JETTON_AREA_COUNT];				//������ǰ���·�
	__int64							m_lSysLimitScore;									//ϵͳ��ע����ϵͳ��ׯʱ����ע�޶�
	const int						m_iMaxAreaScore;									//���������������·�
	bool							m_bControlWin;										//�Ƿ����ʤ�ʣ�������
	int								m_nMaxBankerTimes;									//һ��ׯ�������ڿ�������پ֣�������
	int								m_nHasWinTimes;										//ĳׯ����ׯ��������Ӯ�˶��ٴ�

	CWHArray<ControlAccount>		m_CtrlAccountArray;									//�˺ſ�������

	__int64							m_lCurrentJetton[GAME_PLAYER];						//������ҵĵ�ǰ����
	DWORD							m_dwJettonRate;							//�������

	//���Ʊ���
protected:
	__int64							m_lUserLimitScore;						//������ע
	__int64							m_lApplyBankerCondition;				//��ׯ����
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�
	TCHAR							m_szGameRoomName[32];					//��������
	LONGLONG						m_StorageStart;							//��������ÿ���ӵĿ����ֵ����ȡʧ�ܰ� 0 ����
	LONGLONG						m_StorageDeduct;						//ÿ����Ϸ������۳��Ŀ���������ȡʧ�ܰ� 1.00 ����

	BYTE							m_cbFreeTime;							//����ʱ��
	BYTE							m_cbBetTime;							//��עʱ��
	BYTE							m_cbEndTime;							//����ʱ��

	//��ҳɼ�
protected:
	__int64							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	__int64							m_lUserJetton[GAME_PLAYER];				//�����ע
	__int64							m_lUserRevenue[GAME_PLAYER];			//���˰��

	//�˿���Ϣ
protected:
	BYTE							m_cbWinArea;
	BYTE							m_cbBigSmall;							//��С��Ϊ���ֵ
	DWORD							m_dwGameTimes;							//��ǰ����Ϸ���������ĵڼ���
	BYTE							m_cbGoodLuckType;						//����GoodLuck
	BYTE							m_cbPaoHuoCheCount;						//�ܻ𳵴�����3��7��
	BYTE							m_cbPaoHuoCheArea[7];					//�ܻ𳵵�����

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��עʱ��

	//ׯ����Ϣ
protected:
	CWHArray<WORD>					m_ApplyUserArray;						//������ׯ����б�
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	__int64							m_lBankerWinScore;						//�ۼƳɼ�
	__int64							m_lBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ��������

	//////////
	__int64							m_lRobotMoneyLine;
	__int64							m_lRobotAddScore;
	__int64							m_lRoboMaxMoney;
	int								m_nSetlower;
	bool							m_bAddMoney;
	//////////

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	tagServerGameRecord				m_GameRecordMultiple[8];
	BYTE							m_cbGameRecordMultipleCount;
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	//�����˺ſ���
	BYTE							m_cbControlArea;						//��������
	BYTE							m_cbControlTimes;						//���ƴ���

	DWORD							m_dwRateGoodLuck;						//����GoodLuck�ļ��ʣ�ǧ�ֱ�
	DWORD							m_dwRateChiFen;
	DWORD							m_dwRateCaiJin;
	DWORD							m_dwRateDaQiang;

	//�������
protected:
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const enStartMode		m_GameStartMode;						//��ʼģʽ

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
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID)	{return true;}
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore)	{ }

	//�̳нӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem) { return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem) {return 0;}

	//����ӿ�
public:
	//��λ�ӿ�
	virtual VOID RepositionSink();
	//���ýӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode();
	//�ϳº�2��
	virtual bool __cdecl GetGameBGode(WORD nChair){return true;};
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//xxx
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize) {return true;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return true;}

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);
	//�����˺ſ���
	bool OnAdminControl(CMD_C_ControlApplication * pData, IServerUserItem * pIServerUserItem);

	//���ͺ���
private:
	//�����˿�
	bool DispatchTableCard();
	//�����˺ſ���
	bool DispatchTableCardControl();
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//ÿ����Ϸ��ʼʱ�̣������������·�ֵ
	bool __cdecl CalculateAreaInitLimite();
	//	������ׯ�б�
	bool SendApplyBankerList(IServerUserItem *pIServerUserItem);


	//��Ϸͳ��
private:
	//����÷�
    __int64 CalculateScore();
	//��ȡ����
	void ReadConfigInformation(bool bReadFresh);
	//����ת�����Լ����㱶��
	void CalAreaMultiple(const BYTE cbWinArea, BYTE &cbOutArea, BYTE &cbMultiple);
	//���ֵ
	bool ProbeJudge();
	//���ڱȴ�С�Ŀ��ֵ����
	bool ProbeJudge2(BYTE cbGuess, WORD wChairID);
	//�������Զ��ӷ�
	void RobotAddMoney(__int64 lRobotMoneyLine, __int64 lRobotAddScore, __int64 lRoboMaxMoney, int nSetlower, bool bAddMoney);
};

//////////////////////////////////////////////////////////////////////////

#endif
