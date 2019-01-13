#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	WORD							m_wMeChairID;						//����λ��
	WORD							m_wDUser;							//D���
	WORD							m_wCurrentUser;						//��ǰ���
	bool							m_bOpenCard;						//����
	bool							m_bExitTag;							//ǿ�˱�־
	bool							m_bReset;							//��λ��־
	TCHAR							m_bUserName[GAME_PLAYER][32];		//�û�����			

	//��ע��Ϣ
protected:
	DZPKSCORE							m_lCellScore;						//��Ԫ��ע
	DZPKSCORE							m_lTurnLessScore;					//��С��ע
	DZPKSCORE							m_lTurnMaxScore;					//�����ע
	DZPKSCORE							m_lAddLessScore;					//����Сע
	DZPKSCORE							m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	DZPKSCORE							m_lTotalScore[GAME_PLAYER];			//�ۼ���ע
	DZPKSCORE							m_lCenterScore;						//���ĳ���
	DZPKSCORE							m_lBalanceScore;					//ƽ�����

	DZPKSCORE							m_lBeforeScore;						//�û�Ѻע
	
	int                             m_nCurCardCount;                    //��ǰ����
	bool                            m_bGiveUp;                          //�Ƿ����

	bool							m_bWillWin;							//���ƽ��  //������ʤ����־
	bool							m_bShowHand;						//�����־

	//״̬����
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BOOL							m_bAutoStart;							//�Զ���ʼ
	DZPKSCORE							m_dEndScore[GAME_PLAYER];				//��������

	//�˿���Ϣ
protected:
	BYTE							m_cbCenterCardData[MAX_CENTERCOUNT];	//�����˿�
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	BYTE							m_cbOverCardData[GAME_PLAYER][MAX_CENTERCOUNT];//�����˿�

	//�ؼ�����
protected:
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�
	wstring                         m_strLogFile;                       //��־�ļ�
//���в���
	LONGLONG						m_lRobotBankGetScore;					//ȡ����С����(ȡ�����Ǹ�ֵ���ϵ�һ�������)
	LONGLONG						m_lRobotBankGetScoreBanker;			//ȡ���������(����ֵһ��Ҫ����RobotBankGet)
	int								m_nRobotBankStorageMul;				//����
	LONGLONG						m_lBankerLostScore;							//ׯ����� (��������Ϊׯ)
	LONGLONG						m_lRobotScoreRange[2];					//���Χ

	TCHAR							m_szRoomName[32];			

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { }
	//�Ƿ���Ч
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CAndroidUserItemSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool  RepositionSink();

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool  OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool  OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	/*
		�����Ϣ
		wCmdID=���ֽ�mid+���ֽ�sid
		���Ǽ����ԣ����ô��鷽��
	*/
	virtual bool  OnEventFrameMessage(WORD wCmdID, void * pData, WORD wDataSize);
	//������Ϣ
	virtual bool  OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual void  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual void  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual void  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û���λ
	virtual void  OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//���ܺ���
private:
	//������־
	void createLogFile(tagAndroidUserParameter * pAndroidUserParameter);
	//��ӡ��־
	void printLog(char *szBuff,...);
	void TraceString(LPCTSTR pszString, enTraceLevel TraceLevel);

//	��Ϸ��Ϣ
private:
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

	//���ܺ���
protected:

	//����Ӯ��
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
	//Ӯ���Ƿ�Ϊ������
	bool WinIsAndroid(BYTE cbStartPos, BYTE cbConcludePos);
	//����
	bool GaiLv(BYTE bNum);
	//��ע
	void FollowScore();
	//��ע
	void AddScore();
	//���
	void SendSuoHand();
	//��ע
	void OnSubAddScoreEx();
	//����
	void GiveUpScore();

	//��ȡ����
	void ReadConfigInformation();

};

//////////////////////////////////////////////////////////////////////////

#endif
