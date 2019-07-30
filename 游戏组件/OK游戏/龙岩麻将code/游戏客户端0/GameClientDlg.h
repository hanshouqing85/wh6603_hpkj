#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "DirectSound.h"
#include "MoveCardItem.h"

//////////////////////////////////////////////////////////////////////////

typedef CWHArray<CMoveCardItem *>		MoveCardItemArray;			//������Ŀ

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
	WORD							m_wCaiShenSice;						//�������
	WORD							m_wCaiShenIndex;					//����λ��
	WORD							m_wCaiShenChairID;					//����λ��
	WORD							m_wFirstUser;						//�����û�
	WORD							m_wLandSice;						//ׯ������
	BYTE							m_cbLandCount;						//��ׯ����

	//��������
	TCHAR							m_szAccounts[GAME_PLAYER][32];	//�������

	//��������
	MoveCardItemArray				m_MoveCardItemArray;				//�˿˻���

	//�йܱ���
protected:
	bool							m_bStustee;							//�йܱ�־
	WORD							m_wTimeOutCount;					//��ʱ����

	//�˿˱���
protected:
	BYTE							m_cbLeftCardCount;					//ʣ����Ŀ
	BYTE							m_cbCardIndex[MAX_INDEX];			//�����˿�
	WORD							m_wHeapHead;						//�ƶ�ͷ��
	WORD							m_wHeapTail;						//�ƶ�β��
	BYTE							m_cbHeapCardInfo[GAME_PLAYER][2];		//������Ϣ

	//����˿�
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//����˿�

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CDirectSound					m_DirectSound[3];					//���ơ����ƺͿ�ʼ����
	
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
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	virtual bool OnEventGameClockKill(WORD wChairID){return true;};
	//�Թ�״̬
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pBuffer, WORD wDataSize);

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

	//��������
protected:
	//ת����Ϣ
	void ChangeInfo(BYTE bCard,CString &csInfo);
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
	//�˿˶���
	bool BeginMoveCard();
	//ֹͣ����
	void StopMoveCard();
	//���ƶ���
	bool BeginMoveOutCard( const CMoveCardItem *pMoveCardItem );
	//���ƶ���
	bool BeginMoveSendCard( const CMoveCardItem *pMoveCardItem );
	//��������
	bool BeginMoveReplaceCard( const CMoveCardItem *pMoveCardItem );
	//���ַ��ƶ���
	bool BeginMoveStartCard( const CMoveCardItem *pMoveCardItem );
	//���ƶ������
	bool OnMoveOutCardFinish( const COutCardItem *pOutCardItem );
	//���ƶ������
	bool OnMoveSendCardFinish( const CSendCardItem *pSendCardItem );
	//�����������
	bool OnMoveReplaceCardFinish( const CReplaceCardItem *pReplaceCardItem );
	//���ֶ������
	bool OnMoveStartCardFinish( const CStartCardItem *pStartCardItem );

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
	//ׯ������
	LRESULT OnLandSiceFinish(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnCaiShenSiceFinish(WPARAM wParam, LPARAM lParam);
	//��Ҳ���
	LRESULT OnUserAction( WPARAM wParam, LPARAM lParam );
	//���������Ϣ
	LRESULT OnMoveCardFinish( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
