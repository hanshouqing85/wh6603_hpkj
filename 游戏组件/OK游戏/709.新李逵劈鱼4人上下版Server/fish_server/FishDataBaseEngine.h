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

	//数据库接口
public:
	//配置接口
	virtual bool InitializeSink(IUnknownEx * pIUnknownEx);
//	virtual bool OnGameDataBaseRequest(DWORD dwUserID, WORD wTableID, WORD wChairID);

	//获取连接	
	bool InitDataBaseParameter(tagDataBaseParameter & DataBaseParameter);

	//外部调用接口
public:
	virtual bool GetRoomData();
	//确认领取奖励
	virtual bool ConfirmAward(DWORD dwUserID,  WORD wChairID, IDataBaseSkin* pSkin);
	virtual bool GetOnLineTime(DWORD dwUserID, WORD wChairID, IDataBaseSkin* pSkin);
	virtual bool RecordLoginTime(DWORD dwUserID, int nRecord);
	//实物奖券写入数据库
	virtual bool WriteEntityDate(DWORD dwUserID, int nTicketCount, int nIpadCount, int nMp3Count, int nWhiteCount, int nZippoCount);
	//保存用户数据
	virtual bool SaveUserData(DWORD dwUserID, const SDataBasePlayerData * pPlayer);
	//读取用户数据
	virtual bool GetUserData(DWORD dwUserID, WORD wChairID,  int nExchangeUserRatio, int nExchangeFishRatio, IDataBaseSkin* pSkin);
	//更新用户数据
	virtual bool UpdateUserData(DWORD dwUserID, WORD wChairID, int nExchangeUserRatio, int nExchangeFishRatio, SDataBasePlayerData * pPlayer, IDataBaseSkin* pSkin);


public:
	void SetGameServiceOption(tagGameServiceOption & GameServiceOption){game_service_option_ = GameServiceOption;}

	//配置数据库
protected:
	CDataBaseAide						m_ParameterDBAide;					//记录数据库
	CDataBaseHelper						m_ParameterDBModule;				//游戏币数据库

	//组件变量
public:
	IGameDataBaseEngine *				m_pIGameDataBaseEngine;				//数据协调
	static SRoomData					m_RoomData;							//房间数据
	static DWORD						m_dwUpdateTime;						//房间更新时间
	static __int64                      m_lDynamicAddStockScore;            // 动态增加库存 (正数加，负数减)
	tagGameServiceOption				game_service_option_;
	static CCriticalSection				m_sCriticalSection;					//同步对象

};
