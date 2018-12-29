#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "GameLogic.h"
#include "ShowHand.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//��Ϸ����
protected:
	WORD							m_wMeChairID;						//����λ��
	WORD							m_wDUser;							//D���
	WORD							m_wMinChipInUser;					//��äע
	WORD							m_wMaxChipInUser;					//��äע
	WORD							m_wCurrentUser;						//��ǰ���
	bool							m_bOpenCard;						//����
	bool							m_bExitTag;							//ǿ�˱�־
	bool							m_bReset;							//��λ��־
	TCHAR							m_bUserName[GAME_PLAYER][32];		//�û�����			

	//��ע��Ϣ
protected:
	DOUBLE						m_lCellScore;						//��Ԫ��ע
	DOUBLE						m_lTurnLessScore;					//��С��ע
	DOUBLE						m_lTurnMaxScore;					//�����ע
	DOUBLE						m_lAddLessScore;					//����Сע
	DOUBLE						m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	DOUBLE						m_lTotalScore[GAME_PLAYER];			//�ۼ���ע
	DOUBLE						m_lEachScore[GAME_PLAYER];			//ÿ����ע
	DOUBLE						m_lCenterScore;						//���ĳ���
	DOUBLE						m_lBalanceScore;					//ƽ�����

	//״̬����
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬
	BOOL							m_bAutoStart;						//�Զ���ʼ
	double							m_dEndScore[GAME_PLAYER];			//��������//zrh

	//�˿���Ϣ
public:
	BYTE							m_cbCenterCardData[MAX_CENTERCOUNT];	//�����˿�
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	BYTE							m_cbOverCardData[GAME_PLAYER][MAX_CENTERCOUNT];//�����˿�

	//�ؼ�����
public:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	//CGlobalUnits *                  m_pGlobalUnits;                     //ȫ�ֵ�Ԫ

	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//����̳�
private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();
	//��Ϸ����
	virtual void OnGameOptionSet();

    //ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

	//�����ӿ�
public:
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound){ return true; }

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//���ؿ���
	void HideScoreControl();
	//���¿���
	void UpdateScoreControl();
	//��������Ϣ
	void ProcFrameMessage();

	//��Ϣӳ��
protected:
	//��ʼ��ť
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	//�뿪��Ŧ
	LRESULT OnExit(WPARAM wParam,LPARAM lParam);
	//������Ŧ
	LRESULT OnGiveUp(WPARAM wParam,LPARAM lParam);
	//��ע��ť 
	LRESULT	OnAddScore(WPARAM wParam, LPARAM lParam);
	//���°�ť
	LRESULT	OnMinScore(WPARAM wParam, LPARAM lParam);
	//���ť
	LRESULT	OnMaxScore(WPARAM wParam, LPARAM lParam);
	//ȷ����ť
	LRESULT OnOKScore(WPARAM wParam,LPARAM lParam);
	//ȡ����ť
	LRESULT OnCancelScore(WPARAM wParam,LPARAM lParam);
	//���ư�ť
	LRESULT OnPassCard(WPARAM wParam,LPARAM lParam);
	//��ע��ť
	LRESULT OnFollow(WPARAM wParam,LPARAM lParam);
	//�����ť
	LRESULT OnShowHand(WPARAM wParam,LPARAM lParam);
	//��ʼ����
	LRESULT OnStartTimes(WPARAM wParam,LPARAM lParam);
	//�Զ���ʼ
	LRESULT OnAutoStart(WPARAM wParam,LPARAM lParam);
	//���°�ť
	LRESULT OnSitDown(WPARAM wParam,LPARAM lParam);
	//��Ϸ����
	LRESULT OnGameOver(WPARAM wParam,LPARAM lParam);
	//��Ϸ����
	LRESULT OnSendFinish(WPARAM wParam,LPARAM lParam);
	//������Ϣ
	LRESULT OnOpenCard(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
