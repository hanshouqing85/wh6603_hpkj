#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////
//�궨��

//�����ע����
#define MAX_CHIP_TIME								50
//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	LONGLONG						m_lMaxChipBanker;					//�����ע (ׯ��)
	LONGLONG						m_lMaxChipUser;						//�����ע (����)
	LONGLONG						m_lAreaChip[AREA_COUNT];			//ÿ������Ŀ���ע����(0~����ֵ) 
	WORD							m_wCurrentBanker;					//ׯ��λ��
	BYTE							m_cbTimeLeave;						//ʣ��ʱ��

	int								m_nChipLimit[2];					//��ע������ֵ������Χ(0~CHIP_COUNT-1)
	int								m_nChipTime;						//��ע���� (����)
	int								m_nChipTimeCount;					//���´��� (����)
	
	//��ׯ����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ
	int								m_nBankerCount;						//�������˵���ׯ����
	int								m_nWaitBanker;						//�ռ���
	static int						m_stlApplyBanker;					//������

	//���ñ���  (ȫ������)
protected:
	tagRobotInfo					m_RobotInfo;						//ȫ������
	TCHAR							m_szRoomName[32];					//���÷���

	//���ñ���	(��Ϸ����)
protected:
	bool							m_bRefreshCfg;						//ÿ��ˢ��
	LONGLONG						m_lAreaLimitScore;					//��������(ÿ��ע������߷ⶥ����)
	LONGLONG						m_lUserLimitScore;					//��ע����
	LONGLONG						m_lBankerCondition;					//��ׯ����		

	//���ñ���  (����������)
protected:
	vector<int>                     m_vRobotChipChance;                 //��������ע������ֵ��С����ĸ���Ȩ��
	LONGLONG						m_lRobotJettonLimit[2];				//��������(ע������ֵ��������ֵ����)	
	int								m_nRobotBetTimeLimit[2];			//��������	
	bool							m_bRobotBanker;						//�Ƿ���ׯ
	int								m_nRobotBankerCount;				//��ׯ����
	int								m_nRobotWaitBanker;					//��������
	int								m_nRobotApplyBanker;				//��ׯ����
	bool							m_bReduceJettonLimit;				//��������
	int								m_nRobotListMinCount;				//������ׯ����
	INT64                           m_nListUserCount;					//�б�����


	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�
	wstring                         m_strLogFile;                       //��־�ļ�

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool RepositionSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool  InitUserItemSink(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool  RepositUserItemSink();

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

	//��Ϣ����
public:
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);

	//���ܺ���
public:
	//��ȡ����
	void ReadConfigInformation(TCHAR szFileName[], TCHAR szRoomName[], bool bReadFresh);

	/* --------------------------------------------------------------------------
	����˵����������ע������ֵ������Χ(0~CHIP_COUNT-1)
	���������
		lMaxScore	�����ע����ֵ����ׯ���⸶���ƺ�����⸶���ƵĽ�С��
		lChipLmt[0]	�����˳��뷶Χ(����)��Ҳ����ע������С����ֵ
		lChipLmt[1]	�����˳��뷶Χ(����)��Ҳ����ע����������ֵ
	���봫��������
		nChipTime	��ע����(����)
	����������
		lJetLmt[0]���������⸶���Ƶ���ע������С����ֵ����
		lJetLmt[1]���������⸶���Ƶ���ע����������ֵ����
	����ֵ��
		�ɹ��򷵻�true����ʾ������ע
	--------------------------------------------------------------------------*/
	bool CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[]);

	//������־
	void createLogFile(tagAndroidUserParameter * pAndroidUserParameter);
	//��ӡ��־
	void printLog(char *szBuff,...);
	void printLog(CString& str);
};

//////////////////////////////////////////////////////////////////////////

#endif
