#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	BOOL							m_enableAddScore[4];
	//��Ϸ����
protected:
	__int64							m_lTurnMaxGold;						//�����ע
	__int64							m_lTurnBasicGold;					//��ע��Ŀ
	__int64							m_lBasicGold;						//��Ԫ��Ŀ
	BYTE							m_bPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	WORD							m_wCurrentUser;						//
	__int64							m_lShowHandScore;					//������߷�
	__int64							m_lUserScore[GAME_PLAYER];			//

	//��������
protected:
	bool							m_bShowHand;						//�Ƿ����
	__int64							m_lGoldShow;						//��ע����
	TCHAR							szName[GAME_PLAYER][LEN_ACCOUNTS];	//�������
	CMD_S_GameEnd					m_GameEnd;							//
	bool							m_bAddGold;							//�Ƿ�����ע
	bool							m_bPoor			;					//�Ƿ��������
	

	//�ؼ�����
public:
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	//const tagServerAttribute		*pServerAttribute ;
	
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
	//��Ϸ����
	virtual void OnGameOptionSet();

	//���ÿ��
	virtual bool OnResetGameEngine();
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID);
	//ɾ����ʱ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
	virtual void OnLookonViewChange(bool bLookon){return;};
	//virtual VOID __cdecl OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser);

	//��Ϣ����
protected:
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

public:
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound){return true;}
	//��Ϣӳ��
protected:
	//��ʼ��ť
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	LRESULT	OnShowCard(WPARAM wParam, LPARAM lParam);       //����
	//������ť
	LRESULT OnGiveUp(WPARAM wParam, LPARAM lParam);
	//��ע��ť
	LRESULT OnFollow(WPARAM wParam, LPARAM lParam);
	//�����ť
	LRESULT OnShowHand(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnAddGold(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
	// ʤ������
	LRESULT OnUseWinner(WPARAM wParam, LPARAM lParam);
private:
	//
	void UpdateScoreControl();
	//
	void HideScoreControl();
	//
	void PerformGameEnd();

	DECLARE_MESSAGE_MAP()
	bool IsFreeze(void);
};

//////////////////////////////////////////////////////////////////////////
