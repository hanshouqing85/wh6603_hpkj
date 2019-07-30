/*****************************************************/
//����Ϸ������:��Ƥ�Ƽ�,��ϵQQ:56248
/*****************************************************/

#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE
#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "HistoryScore.h"
#include "DlgCustomRule.h"
#include "factory.h"

//�ӵ���Ϣ
struct BulletInfo
{
	long							beilv;							        //�ӵ�����
	bool 							ishave;						            //�ӵ�����
	int                             zidanId;
};
//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	int								m_MuserPower;

	LONG							m_lUserCellScore[GAME_PLAYER];			//��Ԫ����
	LONG							m_lUserAllScore[GAME_PLAYER];			//��Ԫ����
	int                             dintRand;								//���и���
	int                             dintRand1;								//���и���
	FishTrace                       m_fishtrace[250][5];//��Ӧ���������·�ߣ�5������ͽǶ�
	int                             m_bgindex;//��������
	int                             m_regfishcount;    //���������
	static const int	            m_fishscore[FISHCOUNT]; //���ֵ	
	int                             m_fishShoot[FISHCOUNT][3];//��ɱǹ��
	int                             m_Percent;
	bool                            m_userisSuperPao[GAME_PLAYER];
	int                             m_TimeToTimeIn;  //ʱ���ڽ����
	int                             m_TimeToTimeOut; //ʱ���ڳ�ȥ��
	int                             m_UerResultScore[GAME_PLAYER];//ʱ��������ܵķ���Ӯ
	int                             m_UserPercent[GAME_PLAYER];
	float                           m_UserP;
	int                             m_UserShootCount[GAME_PLAYER];
	BulletInfo                      m_UserShoot[GAME_PLAYER][200];
	int                             m_UserOutTime[GAME_PLAYER];//��ҷ����ӵ�
	int  							m_dwFishID;								//��Ⱥ��ʶ
	TCHAR							m_szIniFileName[MAX_PATH];				//�����ļ�
	int  							m_dwZidanID;								//�ӵ���ʶ
	int                             m_BiliDuihuan;//�һ�����
	int                             m_AndroidPrec;
	int                             m_SendTime[GAME_PLAYER];
	int                             m_KuCun;//ȫ�ֿ��
	int								m_nAllChoushui;							//��ʷ���г�ˮ
	int                             m_FuckUserID[10];//ȫ�ֿ��
	int                             m_MaxScore;
	int                             m_MinScore;
	int                             m_Cellscore;
	int                             m_GoodLuckFish;
	int                             m_jisuan;
	int                             m_Kucun[10];
	int                             m_CurrCuCun;
	int                             m_EveryUpScore;
	CFactoryTemplate<FishPack>	    m_FishFactory;						//��Ⱥ����
	int                             m_BomFishStorg;                     //������
	int                             m_ChoushuiScore;
	int                             m_SitTime[GAME_PLAYER];
	int                             m_CanWinScore[GAME_PLAYER];
	int                             m_UserGameTime[GAME_PLAYER];		//��ҽ���ʱ��
	int                             m_IsAndroidHaveSit[GAME_PLAYER];
	bool                            m_isChangeScreen;
	bool                            m_ischoujiang[6];
	int                             m_userPower[6];
	int                             m_suodingindex[GAME_PLAYER];					//�������
	int                             m_suodingcount[GAME_PLAYER];					//������
	int                             m_AndroidSuoyuPer;
	int								m_FishCountMax;						//��ǰ��������������
	int								m_BigFishCount;						//�������


	//ը����Ϣ
protected:
	BYTE							m_cbBombCount;						//ը������
	BYTE							m_cbEachBombCount[GAME_PLAYER];		//ը������

	//�з���Ϣ
protected:
	BYTE							m_cbBankerScore;					//ׯ�ҽз�
	BYTE							m_cbScoreInfo[GAME_PLAYER];			//�з���Ϣ

	//������Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	BYTE							m_cbTurnCardCount;					//������Ŀ


	//�˿���Ϣ
protected:
	BYTE							m_cbBankerCard[3];					//��Ϸ����
	BYTE							m_cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE							m_cbHandCardData[GAME_PLAYER][20];	//�����˿�

	//�������
protected:

	CHistoryScore					m_HistoryScore;						//��ʷ�ɼ�

	//����ӿ�
protected:
	ITableFrame	*					m_pITableFrame;						//��ܽӿ�
	tagCustomRule *					m_pGameCustomRule;					//�Զ�����
	tagGameServiceOption *			m_pGameServiceOption;				//��Ϸ����
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��Ϸ����

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
	//��λ����
	virtual VOID RepositionSink();
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

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
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//����ӿ�
public:
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
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
	bool OnUserPassCard(WORD wChairID);
	//�û��з�
	bool OnUserCallScore(WORD wChairID, BYTE cbCallScore);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount);
	//��������һ
	void RegFishone();
	//
	void RegFishtwo();
	//
	void RegFishthree();
	//
	void CreadFish();
	//
	void CreatSmalFish();
	//
	void CreatRegSmalFish();
	//
	int GetNewFishID();
	//
	int GetNewZiDanID();
	//������Ϣ
	void SendDataExcludeSourceUser(IServerUserItem * pIServerUserItemSource, WORD wSubCmdID, void * pData, WORD wDataSize);
	//
	bool OnSubHitFish(IServerUserItem * pIServerUserItem, DWORD dwFishID, DWORD dwBulletID,int SendUser,bool IsAndroid);
	//

	void ReadConfig();
	//
	//
	double random(double start, double end);
	//
		//IP����
	bool IsReg();
	//
	bool IsRegTwo();
	// ���ܺ���
	CString  Decrypt(CString S, WORD Key);
	//��ȡCPU��
	CString  GetCPUID();
};

//////////////////////////////////////////////////////////////////////////////////

#endif