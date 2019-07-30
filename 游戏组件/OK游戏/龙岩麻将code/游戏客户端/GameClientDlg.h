#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "DirectSound.h"


//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameEngine
{
	//�û�����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	WORD							m_wReplaceUser;						//�����û�
	BYTE							m_cbActionMask;						//��Ҷ���
	BYTE							m_cbActionCard;						//������
	BYTE							m_cbListenStatus;					//����״̬,0ָδ����,1ָֻ����,2ָֻ����,3ָ���ƺ�����
	bool							m_bWillHearStatus;					//��������
	LONG							m_lSiceCount;						//ɫ����ֵ
	bool							m_bMustWanTong;						//�Ƿ��״���ͬ
	
	//��������
	TCHAR							m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS];	//�������
	CMD_S_GameStart					m_GameStart;						//��ʼ��Ϣ
	CMD_S_SendCard					m_SendCard;							//������Ϣ

	//�йܱ���
protected:
	bool							m_bStustee;							//�йܱ�־
	WORD							m_wTimeOutCount;					//��ʱ����

	//�˿˱���
protected:
	BYTE							m_cbLeftCardCount;					//ʣ����Ŀ
	BYTE							m_cbCardIndex[MAX_INDEX];			//�����˿�

	BYTE							m_cbEnjoinCardCount;				//��ֹ����
	BYTE							m_cbEnjoinCardData[GAME_PLAYER];	//��ֹ����

	//����˿�
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//����˿�

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CDirectSound					m_DirectSound[3];					//���ƺͷ�������

	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID);
    virtual bool OnEventGameClockKill(WORD t);
	//�Թ�״̬
	virtual bool OnEventLookonMode(void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID,void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pBuffer, WORD wDataSize);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//������ʾ
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//�������
	bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�û��й�
	bool OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//�û�����
	bool OnSubListen(const void * pBuffer,WORD wDataSize);
	//������Ϣ
	bool OnSubReplaceCard(const void * pBuffer, WORD wDataSize);
	//�Ƿ���ͬ
	bool OnSubWanTong(const void * pBuffer, WORD wDataSize);


	//��������
protected:
	//��������
	void PlayCardSound(WORD wChairID,BYTE cbCardData);
	//��������
	void PlayActionSound(WORD wChairID,BYTE cbAction);
	//�����ж�
	bool VerdictOutCard(BYTE cbCardData);
	//�����˿�
	void SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard);
	//��ȡ������Ϣ
	BYTE GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] );

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//���Ʋ���
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//�˿˲���
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//�ڶ���ҡɫ����Ϣ
	LRESULT OnSiceTwo(WPARAM wParam, LPARAM lParam);
	//ҡɫ�ӽ�����Ϣ
	LRESULT OnSiceFinish(WPARAM wParam, LPARAM lParam);
	//���������Ϣ
	LRESULT OnOutCardFinish( WPARAM wParam, LPARAM lParam );
	//���������Ϣ
	LRESULT OnSendCardFinish( WPARAM wParam, LPARAM lParam );
	//��Ҳ���
	LRESULT OnUserAction( WPARAM wParam, LPARAM lParam );
	//��Ҳ���
	LRESULT OnUserMove( WPARAM wParam, LPARAM lParam );
	//������ʾ
	LRESULT OnSound(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
