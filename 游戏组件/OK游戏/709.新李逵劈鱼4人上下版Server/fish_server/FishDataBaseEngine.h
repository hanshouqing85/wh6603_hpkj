#pragma once

#include "YJAlgorithm.h"
#include "GameLogic.h"

class CFishDataBaseEngine : public IGameDataBaseEngineSink, public IGameManagerDataBase
{
public:
	CFishDataBaseEngine(void);
	~CFishDataBaseEngine(void);

	virtual void Release(){delete this;};
	virtual void* QueryInterface(REFGUID guid, DWORD query_ver);

	//���ݿ�ӿ�
public:
	//���ýӿ�
	virtual bool InitializeSink(IUnknownEx * pIUnknownEx);
//	virtual bool OnGameDataBaseRequest(DWORD dwUserID, WORD wTableID, WORD wChairID);

	//��ȡ����	
	bool InitDataBaseParameter(tagDataBaseParameter & DataBaseParameter);

	//�ⲿ���ýӿ�
public:
	virtual bool GetRoomData();
	//ȷ����ȡ����
	virtual bool ConfirmAward(DWORD dwUserID,  WORD wChairID, IDataBaseSkin* pSkin);
	virtual bool GetOnLineTime(DWORD dwUserID, WORD wChairID, IDataBaseSkin* pSkin);
	virtual bool RecordLoginTime(DWORD dwUserID, int nRecord);
	//ʵ�ｱȯд�����ݿ�
	virtual bool WriteEntityDate(DWORD dwUserID, int nTicketCount, int nIpadCount, int nMp3Count, int nWhiteCount, int nZippoCount);
	//�����û�����
	virtual bool SaveUserData(DWORD dwUserID, const SDataBasePlayerData * pPlayer);
	//��ȡ�û�����
	virtual bool GetUserData(DWORD dwUserID, WORD wChairID,  int nExchangeUserRatio, int nExchangeFishRatio, IDataBaseSkin* pSkin);
	//�����û�����
	virtual bool UpdateUserData(DWORD dwUserID, WORD wChairID, int nExchangeUserRatio, int nExchangeFishRatio, SDataBasePlayerData * pPlayer, IDataBaseSkin* pSkin);


public:
	void SetGameServiceOption(tagGameServiceOption & GameServiceOption){game_service_option_ = GameServiceOption;}

	//�������ݿ�
protected:
	CDataBaseAide						m_ParameterDBAide;					//��¼���ݿ�
	CDataBaseHelper						m_ParameterDBModule;				//��Ϸ�����ݿ�

	//�������
public:
	IGameDataBaseEngine *				m_pIGameDataBaseEngine;				//����Э��
	static SRoomData					m_RoomData;							//��������
	static DWORD						m_dwUpdateTime;						//�������ʱ��
	static __int64                      m_lDynamicAddStockScore;            // ��̬���ӿ�� (�����ӣ�������)
	tagGameServiceOption				game_service_option_;
	static CCriticalSection				m_sCriticalSection;					//ͬ������

};
