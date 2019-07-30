#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameEngine
{
	//��Ԫ����
	friend class CGameClientView;

	//������Ϣ
protected:
	__int64							m_lMeMaxScore;						//�����ע
	__int64							m_lApplyBankerCondition;			//ׯ����������
	__int64							m_lUserChipCount;					//�û��һ���ĳ�����
	__int64							m_lMeMaxScoreAfterEx;				//�һ���Ľ����
	bool							m_bFirstEnter;						//���ƶһ��ĳ�ʼֵ

	//������ע
protected:
	__int64							m_lMeAreaChipArray[JETTON_AREA_COUNT];
	__int64							m_lXuYaArray[JETTON_AREA_COUNT];	//��Ѻ�õ�����

	//ׯ����Ϣ
protected:
	__int64							m_lBankerChip;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//״̬����
protected:
	bool							m_bMeApplyBanker;					//���Ƿ���������ׯ

public:
	__int64							m_lCurrentChip[GAME_PLAYER];
	DWORD							m_dwChipRate;
	__int64							m_lMyCurrentChip;					//�ҵ�ǰ�ĳ���
	CGlobalUnits					*m_pGlobalUnits;
	bool							m_bAlreadyExchange;
	bool							m_bXuYaContinue;					//������Ѻ
	BYTE							m_cbXuYaContinue;					//������Ѻ����

	//�ؼ�����
public:
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�

	//���ó������
	void SetJettonRate(DWORD dwJettonRate);
	//����ת���ɳ���
	__int64 ScoreToChip(__int64 lScore);
	//����ת���ɻ���
	__int64 ChipToScore(__int64 lChip);
	//�һ�����
	void ExchangeChip(__int64 lChip);
	LRESULT OnGameSound(WPARAM wParam, LPARAM lParam);
	//�����˺ſ���
	LRESULT OnAdminControl(WPARAM wParam, LPARAM lParam);
	//������Ѻ20��
	void OnXuYaContinue();

private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID) {return true;}
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize) {return true;}
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound);
	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//��Ϸ��¼
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//�´�С���
	bool OnSubBigSmall(const void * pBuffer, WORD wDataSize);
	//�һ�����
	bool OnSubChipUpdate(const void * pBuffer, WORD wDataSize);
	//�����˺ſ���
	bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);

	//��ֵ����
protected:
	//����ׯ��
	void SetBankerInfo(WORD wBanker,__int64 lChip);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount);

	//���ܺ���
protected:
	//���¿���
	void UpdateButtonContron();	

	//��Ϣӳ��
protected:
	//��ע��Ϣ
	void OnPlaceJetton(BYTE lJettonArea, __int64 lJettonNum, bool bXuYa);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//�ȴ�С
	LRESULT OnBigSmall(WPARAM wParam, LPARAM lParam);
	//��Ѻ
	LRESULT OnXuYa(WPARAM wParam, LPARAM lParam);

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};

extern CGameClientDlg*	g_pGameClientDlg;

//////////////////////////////////////////////////////////////////////////
