#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientEngine : public CGameFrameEngine
{
	friend CGameClientView;
	//��ע��Ϣ
protected:
	SCORE						m_lCellScore;						//��Ϸ��ע
	SCORE						m_lDrawMaxScore;					//�����ע
	SCORE						m_lTurnMaxScore;					//�����ע
	SCORE						m_lTurnLessScore;					//��С��ע
	SCORE						m_lUserScore[GAME_PLAYER];			//�û���ע
	SCORE						m_lTableScore[GAME_PLAYER];			//������ע

	//״̬����
protected:
	bool							m_bShowHand;						//�����־
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬
	TCHAR							m_szAccounts[GAME_PLAYER][32];		//�������

	//��������
protected:
	CMD_S_GameEnd					m_GameEndPacket;					//��������

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//���ƽӿ�
public:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//��������
	virtual bool OnResetGameEngine();

	//�¼��ӿ�
public:
	//�Թ�״̬
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

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

	//��������
protected:
	//�Զ���ʼ
	bool PerformAutoStart();
	//���ؿ���
	VOID HideScoreControl();
	//���¿���
	VOID UpdateScoreControl();
	//ִ�н���
	VOID PerformGameConclude();
	//�㼯���
	VOID CollectionGold();

	//��Ϣӳ��
protected:
	//��ʼ��Ϣ
	LRESULT OnMessageStart(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnMessageFollow(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageGiveUp(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnMessageAddScore(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	LRESULT OnMessageShowHand(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnMessageSendCardFinish(WPARAM wParam, LPARAM lParam);
		//����Ա����
	LRESULT OnAdminCommand(WPARAM wParam, LPARAM lParam);

	//����Ա����
	LRESULT OnAdminQueryUser(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif