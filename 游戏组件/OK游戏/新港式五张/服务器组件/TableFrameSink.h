#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "HistoryScore.h"

//////////////////////////////////////////////////////////////////////////

//�û�׷��
struct tagUserTracing
{
	SCORE							lWinScore;								//ʤ������
	SCORE							lVarietyScore;							//�仯����
	SYSTEMTIME						SystemTimeTrace;						//��¼ʱ��
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ��¼
protected:
	CString							m_GameRecord;
	TCHAR							m_szNickName[GAME_PLAYER][LEN_NICKNAME];
	//��Ϸ����
protected:
	bool							m_bShowHand;						//�����־
	WORD							m_wCurrentUser;						//��ǰ�û�
	CString							m_strPalyName[GAME_PLAYER];			//�������

	//��ע��Ϣ
protected:
	SCORE							m_lDrawMaxScore;					//�����ע
	SCORE							m_lTurnMaxScore;					//�����ע
	SCORE							m_lTurnLessScore;					//��С��ע
	SCORE							m_lDrawCellScore;					//��ע����

	//�û�״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬
	BYTE							m_cbOperaScore[GAME_PLAYER];		//������־

	//�����Ϣ
protected:
	SCORE							m_lUserScore[GAME_PLAYER];			//�û���ע
	SCORE							m_lTableScore[GAME_PLAYER];			//������ע
	SCORE							m_lUserMaxScore[GAME_PLAYER];		//�����ע

	//�˿˱���
protected:
	BYTE							m_cbSendCardCount;					//������Ŀ
	BYTE							m_cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_cbHandCardData[GAME_PLAYER][5];	//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CHistoryScore					m_HistoryScore;						//��ʷ�ɼ�

	//����ӿ�
protected:
	ITableFrame	*					m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption *			m_pGameServiceOption;				//��������

//AI����
protected:
//	LONGLONG						m_lUserScore[GAME_PLAYER];				//�û�����///////
	bool							m_bNeedWin[GAME_PLAYER];				//��ҪӮ��///////
	bool							m_bAndroid[GAME_PLAYER];				//�Ƿ����/////////
	LONGLONG						m_lScoreLimit[GAME_PLAYER];				//////////////////
	//AI����
protected:
	///////
	bool							m_bRefreshCfg;							//ÿ��ˢ��
	bool							m_bRefreshStorageCfg;					//�Ƿ�ˢ�¿��
	//////
	static SCORE					m_lStockScore;							//����Ӯ��
	LONGLONG						m_lStorageDeduct;						//�ؿ۱��� ǧ�ֱ�

	LONGLONG						m_lStockLimit;							//����Ӯ��

	static LONGLONG					m_lStorageOff;							//������ֵ

	//�γ��������
	LONGLONG						m_lStorageMin;							//�����Сֵ
	LONGLONG						m_lStorageMax;							//������ֵ
	BYTE							StorageRatio;							//����ֵ��Ӯ�����ٷ�֮N		
	BYTE							StorageMinRatio;						//�����Сֵ��Ӯ�����ٷ�֮N		ɱ��ģʽ
	BYTE							StorageMaxRatio;						//������ֵ��Ӯ�����ٷ�֮N		�ͷ�ģʽ
	LONGLONG						StorageEating;							//�Էְٷֱ�,ϵͳ�������ֵ��һ���ԳԵ����ֵ
	//�γ��������
	
	
	TCHAR							m_szFileName[MAX_PATH];
protected:
	//���������Ӯ
	BYTE							m_WinCount;								//������Ҹ���
	bool							m_bWinnerCtrl;							//�Ƿ�������Ӯ


	DWORD							m_dwCheatGameID;						//�����ʺ�
	DWORD							m_dwCheatCount;							//���״���
	BYTE							m_cbCheatType;							//��������
	BYTE							m_WinningRatio;							//������һ���

	DWORD							m_dwCheatGameID1;						//�����ʺ�

	//////�����Ӯ����
	bool							m_bAndroidWinningCtrl;					//�Ƿ���ƻ�������Ӯ����
	LONGLONG				        m_AndroidWinningRatio;					//������Ӯ�ı���
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
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}

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
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserGiveUp(WORD wChairID);
	//�û���ע
	bool OnUserAddScore(WORD wChairID, SCORE lScore);
		//��ȡʤ��
	bool OnUserGetWinner(IServerUserItem * pIServerUserItem);
	//����Ա����
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);

	//��������
	//����������
	bool AndroidCheck();


	//��������
protected:
	//������ע
	VOID RectifyMaxScore();
	//�����˿�
	VOID DispatchUserCard();
		//��ȡ����
	VOID ReadConfigInformation(bool bReadFresh);
	//��ȡ���Ӻ�
	WORD GetChairIDFromGameID(DWORD dwGameID);

	//�߼�����
protected:
	//�ƶ�����
	WORD EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos);
	//�ƶ�ʤ��
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
		//��ת����
	CString TransformCardInfo( BYTE cbCardData );
	WORD EstimateWinnerEx(BYTE cbStartPos, BYTE cbConcludePos);

	WORD GetChairIdByGameId(DWORD dwGameId);

	void AndroidSendUserCard();

	void ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType);
};

//////////////////////////////////////////////////////////////////////////

#endif