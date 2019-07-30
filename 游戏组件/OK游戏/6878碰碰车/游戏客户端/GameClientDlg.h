#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

#include "DirectSound.h"
#include "afxtempl.h"
#include "BassMusicManager.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameEngine
{
	//��Ԫ����
	friend class CGameClientView;

	//������Ϣ
protected:
	SCORE						m_lMeMaxScore;						//�����ע
	SCORE						m_lAreaLimitScore;					//��������
	SCORE						m_lApplyBankerCondition;			//��������
	int                             m_GameEndTime;
	bool							m_blUsing;
	ULONG_PTR m_gdiplusToken;
	//������ע
protected:
	SCORE						m_lUserJettonScore[AREA_COUNT+1];	//������ע
	

	//ׯ����Ϣ
protected:
	SCORE						m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	BYTE							m_cbLeftCardCount;					//�˿���Ŀ
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ


	//״̬����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//�ؼ�����
protected:
	CList<tagAndroidBet,tagAndroidBet>		m_ListAndroid;				//��������ע����

	//������Դ
protected:
	CDirectSound					m_DTSDBackground;					//��������
	CDirectSound                    m_DITDXuanzhuan;                    //��ת����
	CDirectSound					m_DTSDCheer[4];						//��������

	BYTE							m_cbMusic;							//��������ID
	bool							m_bAllowBackSound;					//�Ƿ񲥷�����

	struct SortTemp
	{
		LONGLONG a;
		LONGLONG b;
		LONGLONG c;
		LONGLONG d;
		SortTemp()
		{
			memset(this,0,sizeof(*this));
		}
	};


	SortTemp m_TempData;

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
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);
	//�Թ�״̬
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
	virtual bool AllowBackGroundSound(bool bAllowSound);

	bool OnFrameMessage(WORD wSubCmdID,  void * pBuffer, WORD wDataSize);
	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart( void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameFree( void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton( void * pBuffer, WORD wDataSize,bool bGameMes);
	//��Ϸ����
	bool OnSubGameEnd( void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker( void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker( void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker( void * pBuffer, WORD wDataSize);
	//��Ϸ��¼
	bool OnSubGameRecord( void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail( void * pBuffer, WORD wDataSize);
	//�˿���
	bool OnSubSendCard( void * pBuffer, WORD wDataSize);

	bool OnSubCheckImageIndex( void * pBuffer, WORD wDataSize);
	//����
	bool OnSubAdminControl( void * pBuffer, WORD wDataSize);
	bool OnSubScoreResult(void * pBuffer, WORD wDataSize);
	bool OnSubAccountResult(void * pBuffer, WORD wDataSize);


	//��ֵ����
protected:
	//����ׯ��
	void SetBankerInfo(WORD wBanker,SCORE lScore);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount);

	//���ܺ���
protected:
	void ReSetBankCtrol(int nGameState);
	//���¿���
	void UpdateButtonContron();	

	//��Ϣӳ��
protected:
	//��ע��Ϣ
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnContinueCard(WPARAM wParam, LPARAM lParam);
	//�ֹ�����
	LRESULT  OnOpenCard(WPARAM wParam, LPARAM lParam);
	//�Զ�����
	LRESULT  OnAutoOpenCard(WPARAM wParam, LPARAM lParam);
	//�Զ�����
	LRESULT  OnShowResult(WPARAM wParam, LPARAM lParam);

	LRESULT  OnPlaySound(WPARAM wParam, LPARAM lParam);
	//����
	LRESULT  OnAdminControl(WPARAM wParam, LPARAM lParam);

	LRESULT  OnGetAccount(WPARAM wParam, LPARAM lParam);
	LRESULT  OnChangeMusic(WPARAM wParam, LPARAM lParam);
	LRESULT	 OnCheckAccount(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
