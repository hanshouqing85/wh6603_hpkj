#include "StdAfx.h"
#include "FishDataBaseEngine.h"

extern SCORE g_stock_score_;
extern SCORE g_additional_score_;

CCriticalSection  CFishDataBaseEngine::m_sCriticalSection;
SRoomData	CFishDataBaseEngine::m_RoomData;							//房间数据
DWORD		CFishDataBaseEngine::m_dwUpdateTime=0;						//房间更新时间
__int64     CFishDataBaseEngine::m_lDynamicAddStockScore=0;            // 动态增加库存 (正数加，负数减)

CFishDataBaseEngine::CFishDataBaseEngine(void)
{
	
}

CFishDataBaseEngine::~CFishDataBaseEngine(void)
{
}

void* CFishDataBaseEngine::QueryInterface(REFGUID guid, DWORD query_ver) 
{
	QUERYINTERFACE(IGameDataBaseEngineSink,guid,query_ver);
	return NULL;
}
// bool CFishDataBaseEngine::OnGameDataBaseRequest(DWORD dwUserID, WORD wTableID, WORD wChairID)
// {
// 	return true;
// }
bool CFishDataBaseEngine::InitializeSink( IUnknownEx * pIUnknownEx )
{
	m_pIGameDataBaseEngine = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameDataBaseEngine);

	//创建对象
	if ((m_ParameterDBModule.GetInterface()==NULL)&&(m_ParameterDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}
	try
	{
		//获取连接
		tagDataBaseParameter DataBaseParameter;
		InitDataBaseParameter(DataBaseParameter);

		//设置连接
		m_ParameterDBModule->SetConnectionInfo(DataBaseParameter.szDataBaseAddr,
			DataBaseParameter.wDataBasePort,
			DataBaseParameter.szDataBaseName,
			DataBaseParameter.szDataBaseUser,
			DataBaseParameter.szDataBasePass);

		//发起连接
		m_ParameterDBModule->OpenConnection();
		m_ParameterDBAide.SetDataBase(m_ParameterDBModule.GetInterface());
		CGameLogic::SetManager(this);
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CString strException;
		strException.Format(TEXT("从数据库加载房间配置信息失败-%s"), pIException->GetExceptionDescribe());
		CTraceService::TraceString(strException,TraceLevel_Exception);
		return false;
	}


	try		//从数据库读取房间配置
	{
		m_ParameterDBAide.ResetParameter();
		m_ParameterDBAide.AddParameter(TEXT("@ServerID"),game_service_option_.wServerID);
		m_ParameterDBAide.AddParameter(TEXT("@StockScore"),g_stock_score_);
		m_ParameterDBAide.AddParameter(TEXT("@AdditionalScore"),g_additional_score_);
		LONG lResultCode=m_ParameterDBAide.ExecuteProcess(TEXT("GSP_GF_ReadRoomDataFromDB"),true);
		if(lResultCode != DB_SUCCESS)
		{
			ASSERT(FALSE);
			return true;
		}

		m_RoomData.roomMaxCost = m_ParameterDBAide.GetValue_INT(TEXT("roomMaxCost"));
		m_RoomData.onceUpScore = m_ParameterDBAide.GetValue_INT(TEXT("onceUpScore"));
		m_RoomData.factor = m_ParameterDBAide.GetValue_DOUBLE(TEXT("factor"));
		m_RoomData.level = m_ParameterDBAide.GetValue_INT(TEXT("roomLevel"));

		m_RoomData.factorsmall = m_ParameterDBAide.GetValue_DOUBLE(TEXT("factorsmall"));
		m_RoomData.levelsmall = m_ParameterDBAide.GetValue_INT(TEXT("roomLevelsmall"));
		
		//// 读取动态增加的库存值
		//__int64 nAddStockScore = m_ParameterDBAide.GetValue_LONGLONG(TEXT("dynamicStock"));
		//if (nAddStockScore != m_lDynamicAddStockScore)  //启动房间时，只保留动态值
		//{
		//	m_lDynamicAddStockScore = nAddStockScore;
		//}

		CGameLogic::ReadRoomData(&m_RoomData);
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CString strException;
		strException.Format(TEXT("从数据库加载房间配置信息失败-%s"), pIException->GetExceptionDescribe());
		CTraceService::TraceString(strException,TraceLevel_Exception);

		return false;
	}

	m_dwUpdateTime = GetTickCount();	

	return true;
}

//获取连接
bool CFishDataBaseEngine::InitDataBaseParameter(tagDataBaseParameter & DataBaseParameter)
{

	TCHAR szFilePath[_MAX_PATH + 1]={0};
	GetModuleFileName(NULL, szFilePath, _MAX_PATH);
	(_tcsrchr(szFilePath, TEXT('\\')))[1] = 0;

	//构造路径
	TCHAR szIniFile[MAX_PATH]=TEXT("");
	_sntprintf_s(szIniFile, CountArray(szIniFile),MAX_PATH, TEXT("%slkpy_server.ini"),szFilePath);


	//读取配置
	CWHIniData IniData;
	IniData.SetIniFilePath(szIniFile);

	//连接信息
	DataBaseParameter.wDataBasePort=(WORD)IniData.ReadInt(TEXT("GameFishDB"),TEXT("DBPort"),1433);
	IniData.ReadEncryptString(TEXT("GameFishDB"),TEXT("DBAddr"),NULL,DataBaseParameter.szDataBaseAddr,CountArray(DataBaseParameter.szDataBaseAddr));
	IniData.ReadEncryptString(TEXT("GameFishDB"),TEXT("DBUser"),NULL,DataBaseParameter.szDataBaseUser,CountArray(DataBaseParameter.szDataBaseUser));
	IniData.ReadEncryptString(TEXT("GameFishDB"),TEXT("DBPass"),NULL,DataBaseParameter.szDataBasePass,CountArray(DataBaseParameter.szDataBasePass));
	IniData.ReadEncryptString(TEXT("GameFishDB"),TEXT("DBName"),szPlatformDB,DataBaseParameter.szDataBaseName,CountArray(DataBaseParameter.szDataBaseName));


	return true;
}


bool CFishDataBaseEngine::GetRoomData()
{
// 	CDataLocker DataLocker(m_sCriticalSection);
// 	//避免重复更新
// 	if(GetTickCount() - m_dwUpdateTime <28000)	// 30刷新一次
// 	{
// 		CGameLogic::ReadRoomData(&m_RoomData);
// 		return true;
// 	}
// 	m_dwUpdateTime = GetTickCount();
// 
// 	if (m_pIGameDataBaseEngine != NULL)
// 	{
// 		m_pIGameDataBaseEngine->PostGameDataBaseResult(DBR_GR_FISH_ROOMDATA,0, NULL, 0 );
// 	}

	return true;
}

bool CFishDataBaseEngine::SaveUserData( DWORD dwUserID, const SDataBasePlayerData * pPlayer )
{
	DBR_SavePlayer SavePlayer;
	SavePlayer.dwDataBaseSkin = 0;
	SavePlayer.dwUserID = dwUserID;
	memcpy(&SavePlayer.PlayerData, pPlayer, sizeof(SDataBasePlayerData));
	if (m_pIGameDataBaseEngine != NULL)
		m_pIGameDataBaseEngine->PostGameDataBaseResult(DBR_GR_FISH_SAVE_SUERDATA,  &SavePlayer,sizeof(DBR_SavePlayer) );

	return true;
}

bool CFishDataBaseEngine::GetUserData( DWORD dwUserID, WORD wChairID,  int nExchangeUserRatio, int nExchangeFishRatio, IDataBaseSkin* pSkin )
{
	DBR_PlayerData PlayerData;
	PlayerData.dwDataBaseSkin = (DWORD)pSkin;
	PlayerData.dwUserID = dwUserID;
	PlayerData.wChairID = wChairID;
	PlayerData.nExchangeUserRatio = nExchangeUserRatio;
	PlayerData.nExchangeFishRatio = nExchangeFishRatio;

	if (m_pIGameDataBaseEngine != NULL)
		m_pIGameDataBaseEngine->PostGameDataBaseResult(DBR_GR_FISH_GET_SUERDATA, &PlayerData,sizeof(PlayerData) );

	return true;
}

bool CFishDataBaseEngine::UpdateUserData( DWORD dwUserID, WORD wChairID, int nExchangeUserRatio, int nExchangeFishRatio, SDataBasePlayerData * pPlayer, IDataBaseSkin* pSkin )
{
	DBR_SavePlayer SavePlayer;
	SavePlayer.dwDataBaseSkin = (DWORD)pSkin;
	SavePlayer.dwUserID = dwUserID;
	SavePlayer.wChairID = wChairID;
	SavePlayer.nExchangeUserRatio = nExchangeUserRatio;
	SavePlayer.nExchangeFishRatio = nExchangeFishRatio;
	memcpy(&SavePlayer.PlayerData, pPlayer, sizeof(SDataBasePlayerData));

	if (m_pIGameDataBaseEngine != NULL)
		m_pIGameDataBaseEngine->PostGameDataBaseResult(DBR_GR_FISH_UPATA_SUERDATA, &SavePlayer,sizeof(DBR_SavePlayer));
	return true;
}

bool CFishDataBaseEngine::GetOnLineTime(DWORD dwUserID, WORD wChairID, IDataBaseSkin* pSkin)
{
	DBR_OnLineTime OnLineTime;
	ZeroMemory(&OnLineTime,sizeof(OnLineTime));

	OnLineTime.dwDataBaseSkin = (DWORD)pSkin;
	OnLineTime.dwUserID = dwUserID;
	OnLineTime.wChairID = wChairID;

	if (m_pIGameDataBaseEngine != NULL)
		m_pIGameDataBaseEngine->PostGameDataBaseResult(DBR_GR_FISH_GET_ONLINETIME, &OnLineTime,sizeof(OnLineTime));
	return true;
}

bool CFishDataBaseEngine::RecordLoginTime(DWORD dwUserID, int nRecord)
{
	DBR_LoginTime LoginTime;
	ZeroMemory(&LoginTime,sizeof(LoginTime));


	LoginTime.dwUserID = dwUserID;
	LoginTime.nRecord = nRecord;

	if (m_pIGameDataBaseEngine != NULL)
		m_pIGameDataBaseEngine->PostGameDataBaseResult(DBR_GR_FISH_RECORD_LOGINTIME, &LoginTime,sizeof(LoginTime));
	return true;
}

bool CFishDataBaseEngine::ConfirmAward(DWORD dwUserID, WORD wChairID, IDataBaseSkin* pSkin)
{
	DBR_ConfirmAward confirmAward;
	ZeroMemory(&confirmAward, sizeof(confirmAward));
	
	confirmAward.dwDataBaseSkin = (DWORD)pSkin;
	confirmAward.dwUserID = dwUserID;
	confirmAward.wChairID = wChairID;
	

	if (m_pIGameDataBaseEngine != NULL)
		m_pIGameDataBaseEngine->PostGameDataBaseResult(DBR_GR_FISH_CONFIRM_AWARD, &confirmAward,sizeof(confirmAward));
	return true;
}

bool CFishDataBaseEngine::WriteEntityDate(DWORD dwUserID, int nTicketCount, int nIpadCount, int nMp3Count, int nWhiteCount, int nZippoCount)
{
	DBR_EntityDate entiryDate;
	ZeroMemory(&entiryDate, sizeof(entiryDate));

	entiryDate.dwUserID = dwUserID;
	entiryDate.nTicketCount = nTicketCount;
	entiryDate.nIpadCount = nIpadCount;
	entiryDate.nMp3Count = nMp3Count;
	entiryDate.nWhiteCount = nWhiteCount;
	entiryDate.nZippoCount = nZippoCount;

	if (m_pIGameDataBaseEngine != NULL)
		m_pIGameDataBaseEngine->PostGameDataBaseResult(DBR_GR_FISH_ENTITY_DATE, &entiryDate,sizeof(entiryDate));

	return true;
}
