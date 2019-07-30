
#include "StdAfx.h"
#include "table_frame_sink.h"
#include "tinyxml/tinyxml.h"
#include "game_service_manager.h"
// 测试标志 如果定义此标志 库存和概率无效
//#define TEST

const DWORD kBulletIonTimer = 1;
const DWORD kBulletIonTimerEnd = 8;
const DWORD kLockTimer = 9;
const DWORD kClearTraceTimer = 10;
const DWORD kBuildSmallFishTraceTimer = 11;
const DWORD kBuildMediumFishTraceTimer = 12;
const DWORD kBuildBigFishTraceTimer = 13;
const DWORD kWinScoreMaxTimer = 14;
const DWORD kBuildTicketFishTraceTimer = 15;
// const DWORD kBuildFish20TraceTimer = 15;
// const DWORD kBuildFishLKTraceTimer = 16;
// const DWORD kBuildFishBombTraceTimer = 17;
// const DWORD kBuildFishSuperBombTraceTimer = 18;
// const DWORD kBuildFishLockBombTraceTimer = 19;
const DWORD kBuildFishSanTraceTimer = 20;
const DWORD kBuildFishSiTraceTimer = 21;
const DWORD kBuildFishKingTraceTimer = 22;
const DWORD kSwitchSceneTimer = 23;
const DWORD kSceneEndTimer = 24;
const DWORD kLKScoreTimer = 25;
const DWORD kRereadConfigTimer = 26;
const DWORD kFishMsg = 27;
const DWORD kUpdateRoomConfigTimer = 28;								//不能kill
const DWORD kRefreshRegulation = 29;

const DWORD kClearTraceElasped = 60;									//清理前面的鱼
const DWORD kBuildSmallFishTraceElasped = 3;							//小鱼(1-10号鱼)
const DWORD kBuildMediumFishTraceElasped = 5;							//中等的鱼(11-17号鱼)

//没有写用到
const DWORD kBuildFish18TraceElasped = 110;								//黄金鲨鱼
const DWORD kBuildFish19TraceElasped = 185;								//龙
const DWORD kBuildFish20TraceElasped = 250;								//企鹅
const DWORD kBuildFishLKTraceElasped = 37;								//李逵
const DWORD kBuildFishBombTraceElasped = 70;							//炸弹
const DWORD kBuildFishSuperBombTraceElasped = 185;						//超级炸弹
const DWORD kBuildFishLockBombTraceElasped = 47;						//定屏炸弹

/////////////----------

const DWORD kBuildFishSanTraceElasped = 55 + 18;						//三元
const DWORD kBuildFishSiTraceElasped = 65 + 17;							//四喜
const DWORD kBuildFishKingTraceElasped = 250;							//鱼王
#ifdef _DEBUG
const DWORD kSwitchSceneElasped = 600;									//切换场景的时间
#else
const DWORD kSwitchSceneElasped = 600;									//切换场景的时间
#endif
const DWORD kSceneEndElasped = 55;										//场景结束时间
const DWORD kLKScoreElasped = 1;										//悟空分数增长
const DWORD kUpdateRoomConfigElasped = 30;


const DWORD kRepeatTimer = (DWORD)0xFFFFFFFF;
const DWORD kFishAliveTime = 180000;

SCORE g_stock_score_ = 0;												//库存
SCORE g_additional_score_=0;											//附加扣除

__int64 g_lDynamicAddStockScore=0;										//动态增加库存 (正数加，负数减)
__int64 g_lMinStockScore=0;												//库存最小值
int		g_nUserStock=1;


// 当前调整的级别 -1不调整
int   g_cur_increase = -1;
int   g_ctrl_crucial_count_=0;											//调整的个数
int   g_nCtrlScore[15]={0};												//库存
double g_increase_factor[15]={0};										//调整值
double g_increase_factorSmall[15]={0};
int   g_nAdditionalPercent=0;											//附加扣除比例
int   g_nUseHistory=1;
DWORD   g_dwCapFishCount[FISH_KIND_COUNT]={0};

DWORD g_dwLastRead = 0;
SCORE g_user_win_max_=20000000;
int   g_last_stock_crucial = 0;											//大鱼
int   g_last_small_stock_crucial = 0;									//小鱼

CMutex  g_Mutex;
TableFrameSink *g_pTableFrameSink[MAX_TABLE];

static SCORE g_revenue_score = 0;
static int   g_stockDeduct=0;
static int   g_stockStartDeduct=100000;
static int   g_bulletTimeSpace = 1200;
static double g_testProbability=0.6f;
static double g_testMinProbability=0.8f;
static double g_maxBulletProb=0;



static std::vector<DWORD> g_balck_list_;
static std::vector<DWORD> g_white_list_;
static int _uid = 0;

struct Fish20Config {
	DWORD game_id;
	int catch_count;
	double catch_probability;
};

static std::vector<Fish20Config> g_fish20_config_;

void AddUserFilter(DWORD game_id, unsigned char operate_code) {
	std::vector<DWORD>::iterator iter;
	if (operate_code == 0) {
		for (iter = g_balck_list_.begin(); iter != g_balck_list_.end(); ++iter) {
			if ((*iter) == game_id) return;
		}
		g_balck_list_.push_back(game_id);
	} else if (operate_code == 1) {
		for (iter = g_white_list_.begin(); iter != g_white_list_.end(); ++iter) {
			if ((*iter) == game_id) return;
		}
		g_white_list_.push_back(game_id);
	} else {
		for (iter = g_balck_list_.begin(); iter != g_balck_list_.end(); ++iter) {
			if ((*iter) == game_id) {
				iter = g_balck_list_.erase(iter);
				break;
			}
		}
		for (iter = g_white_list_.begin(); iter != g_white_list_.end(); ++iter) {
			if ((*iter) == game_id) {
				iter = g_white_list_.erase(iter);
				break;
			}
		}
	}
}
// 返回值 0：黑名单  1 白名单 -1 正常
int CheckUserFilter(DWORD game_id) {
	std::vector<DWORD>::iterator iter;
	for (iter = g_balck_list_.begin(); iter != g_balck_list_.end(); ++iter) {
		if ((*iter) == game_id) return 0;
	}

	for (iter = g_white_list_.begin(); iter != g_white_list_.end(); ++iter) {
		if ((*iter) == game_id) return 1;
	}

	return -1;
}

void AddFish20Config(DWORD game_id, int catch_count, double catch_probability) 
{
	std::vector<Fish20Config>::iterator iter;
	for (iter = g_fish20_config_.begin(); iter != g_fish20_config_.end(); ++iter) 
	{
		Fish20Config& config = *iter;
		if (game_id == config.game_id)
		{
			if (catch_count == 0) 
			{
				g_fish20_config_.erase(iter);
			} 
			else 
			{
				config.catch_count = catch_count;
				config.catch_probability = catch_probability;
			}
			return;
		}
	}

	Fish20Config config;
	config.game_id = game_id;
	config.catch_count = catch_count;
	config.catch_probability = catch_probability;
	g_fish20_config_.push_back(config);
}

bool CheckFish20Config(DWORD game_id, int* catch_count, double* catch_probability) 
{
	std::vector<Fish20Config>::iterator iter;
	for (iter = g_fish20_config_.begin(); iter != g_fish20_config_.end(); ++iter) 
	{
		Fish20Config& config = *iter;
		if (game_id == config.game_id) 
		{
			if (config.catch_count <= 0) 
			{
				g_fish20_config_.erase(iter);
				return false;
			}
			*catch_count = config.catch_count;
			*catch_probability = config.catch_probability;
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

TableFrameSink::TableFrameSink() : table_frame_(NULL), game_service_attrib_(NULL), game_service_option_(NULL), base_score_(1),
exchange_ratio_userscore_(1), exchange_ratio_fishscore_(1), exchange_count_(1), next_scene_kind_(SCENE_KIND_1), special_scene_(false),
 android_chairid_(INVALID_CHAIR) 
{
	for (WORD i = 0; i < GAME_PLAYER; ++i) {
		fish_score_[i] = 0;
		exchange_fish_score_[i] = 0;
		bullet_id_[i] = 0;
		user_score_[i]=0;
		user_lotteries_[i]=0;
		user_ipdaMini_[i] = 0;
		user_musicPlay3_[i] = 0;
		user_bigWhite_[i] = 0;
		user_zippo_[i] = 0;
		user_score_win_[i]=0;
		nfresh_time_[i]=0;
	}
	bomb_range_width_ = 0;
	bomb_range_height_ = 0;
	m_nloadflag = 0;
	ZeroMemory(m_dwPlayTime, sizeof(m_dwPlayTime));
	ZeroMemory(m_dwLastBulletTime, sizeof(m_dwLastBulletTime));
	ZeroMemory(m_dwLastFireTime, sizeof(m_dwLastFireTime));
	ZeroMemory(m_nNation, sizeof(m_nNation));
	ZeroMemory(fire_count_, sizeof(fire_count_));

	m_nLoserRegionPercent=0;
	ZeroMemory(m_szLoserRigion, sizeof(m_szLoserRigion));

	memset(stock_crucial_score_, 0, sizeof(stock_crucial_score_));
	memset(stock_increase_probability_, 0, sizeof(stock_increase_probability_));
	stock_crucial_count_ = 0;

	memset(ticket_create_probability, 0, sizeof(ticket_create_probability));
	memset(ticket_capture_probability, 0, sizeof(ticket_capture_probability));

	m_tableStartTime = GetTickCount();
	m_id = _uid++;
	srand(GetTickCount() + m_id *17);
	fresh_player=1;
	minElapse=7;
	maxElapse = 23;
	decreasePermillage=500;
	LockPercent = 50;
	LostLockPercent = 50;
	fishtime_ = 0;
	firetime_ = 0;
	timeindex_=0;
	timeindexadd=0;
	bandrior_fire_=true;
	percentlevel[0]=0;
	percentlevel[1]=0;
	percentlevel[2]=20;
	percentlevel[3]=30;
	percentlevel[4]=30;
	percentlevel[5]=20;
	percentlevel[6]=0;
	ticket_score_[0] = 100;
	ticket_score_[1] = 200;
	ticket_score_[2] = 500;
	ticket_score_[3] = 1000;
	ticket_score_[4] = 2000;
	ticket_score_[5] = 5000;
	ticket_score_[6] = 10000;
	ticketCreateTime = 0;
	fish_decreasemul=10;
	ZeroMemory(m_dwLastCatchFishTick, sizeof(m_dwLastCatchFishTick));
	ZeroMemory(m_dwLastSweepFishTick, sizeof(m_dwLastSweepFishTick));
	ZeroMemory(m_nLastBullet_mul, sizeof(m_nLastBullet_mul));
	ZeroMemory(m_dwLastCatchFishTime, sizeof(m_dwLastCatchFishTime)); // 打中鱼，5秒中内不允许加炮
	ZeroMemory(m_nNotCatchFireCount, sizeof(m_nNotCatchFireCount));// 0表示不计数
	ZeroMemory(m_nFishHarvestArray, sizeof(m_nFishHarvestArray));

	m_hMutex=::CreateEvent(NULL,NULL,TRUE, NULL);
}

TableFrameSink::~TableFrameSink() {
	g_pTableFrameSink[table_frame_->GetTableID()]=NULL;
	FishTraceInfoVecor::iterator iter;
	for (iter = storage_fish_trace_vector_.begin(); iter != storage_fish_trace_vector_.end(); ++iter) {
		delete (*iter);
	}
	storage_fish_trace_vector_.clear();
	for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) {
		delete (*iter);
	}
	active_fish_trace_vector_.clear();

	ServerBulletInfoVector::iterator it;
	for (it = storage_bullet_info_vector_.begin(); it != storage_bullet_info_vector_.end(); ++it) {
		delete (*it);
	}
	storage_bullet_info_vector_.clear();
	for (WORD i = 0; i < GAME_PLAYER; ++i) {
		for (it = server_bullet_info_vector_[i].begin(); it != server_bullet_info_vector_[i].end(); ++it) {
			delete (*it);
		}
		server_bullet_info_vector_[i].clear();
	}

	if (g_revenue_score > 0) {
		CString str;
		str.Format(TEXT("吃水%I64d"), g_revenue_score);
		AfxMessageBox(str);
		g_revenue_score = 0;
	}
}

void* TableFrameSink::QueryInterface(REFGUID guid, DWORD query_ver) {
	QUERYINTERFACE(ITableFrameSink, guid, query_ver);
	QUERYINTERFACE(ITableUserAction, guid, query_ver);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink, guid, query_ver);
	return NULL;
}

bool TableFrameSink::Initialization(IUnknownEx* unknownex) {
	table_frame_ = QUERY_OBJECT_PTR_INTERFACE(unknownex, ITableFrame);
	if (!table_frame_) return false;

	table_frame_->SetStartMode(START_MODE_TIME_CONTROL);
	game_service_attrib_ = table_frame_->GetGameServiceAttrib();
	game_service_option_ = table_frame_->GetGameServiceOption();
	base_score_ = max(1, game_service_option_->lCellScore);	
	if( table_frame_->GetTableID() == 0 )
	{
		for (int i=0;i<MAX_TABLE; ++i)
		{
			g_pTableFrameSink[i]=NULL;
		}
		ZeroMemory(g_dwCapFishCount, sizeof(g_dwCapFishCount));
	}
	if (!LoadConfig(true)) {
		AfxMessageBox(TEXT("配置资源解析失败，请检查"));
		return false;
	}

	m_GameLogic.SetTable(this);

	if( table_frame_->GetTableID() == 0 )
	{		
		table_frame_->SetGameTimer(kUpdateRoomConfigTimer, kUpdateRoomConfigElasped*1000, kRepeatTimer, 0);
	}
	g_pTableFrameSink[table_frame_->GetTableID()]=this;
	return true;
}

void TableFrameSink::RepositionSink() {
}

bool TableFrameSink::IsUserPlaying(WORD chair_id) {
	return true;
}

bool TableFrameSink::OnEventGameStart() {
	return true;
}

bool TableFrameSink::OnEventGameConclude(WORD chair_id, IServerUserItem* server_user_item, BYTE reason) {
	if (reason == GER_DISMISS) {
		if (table_frame_->GetGameStatus() == GAME_STATUS_PLAY)
		{
			for (WORD i = 0; i < GAME_PLAYER; ++i) {
				IServerUserItem* user_item = table_frame_->GetTableUserItem(i);
				if (user_item == NULL) continue;
				CalcScore(user_item);
			}

			KillAllGameTimer();
			table_frame_->ConcludeGame(GAME_STATUS_FREE,NULL,608);			
			table_frame_->KillGameTimer(kSwitchSceneTimer);
			table_frame_->KillGameTimer(kClearTraceTimer);
			table_frame_->KillGameTimer(kRefreshRegulation);
			ClearFishTrace(true);
			next_scene_kind_ = SCENE_KIND_1;
			special_scene_ = false;
			android_chairid_ = INVALID_CHAIR;
			timeindex_=0;
			timeindexadd=0;
		}
	} else if (chair_id < GAME_PLAYER && server_user_item != NULL) {
		CalcScore(server_user_item);
	}
	return true;
}

bool TableFrameSink::OnEventSendGameScene(WORD chair_id, IServerUserItem* server_user_item, BYTE game_status, bool send_secret) {
	switch (game_status) {
	case GAME_STATUS_FREE:
	case GAME_STATUS_PLAY:
		if (server_user_item->GetUserInfo()->cbUserStatus != US_LOOKON)
		{
			m_dwPlayTime[chair_id] = (DWORD)time(NULL);
			m_dwLastBulletTime[chair_id]=GetTickCount();
		}
		SendGameConfig(server_user_item);

		CMD_S_GameStatus gamestatus;
		gamestatus.game_version = GAME_VERSION;
		gamestatus.isscene = special_scene_;
		memcpy(gamestatus.fish_score, fish_score_, sizeof(gamestatus.fish_score));
		memcpy(gamestatus.exchange_fish_score, exchange_fish_score_, sizeof(gamestatus.exchange_fish_score));

		table_frame_->SendGameScene(server_user_item, &gamestatus, sizeof(gamestatus));

		table_frame_->SendGameMessage(server_user_item, TEXT("键盘↑↓键加减炮，←→键上下分，空格键或鼠标左键发射子弹，F4关闭声音!"), SMT_CHAT);

		return true;
	}
	return false;
}
bool TableFrameSink::OnTimerFishMsg(WPARAM bind_param)
{
	return true;

	int Msg_fish_id = (int)bind_param;
	TCHAR tips_msg[1024] = { 0 };

//	table_frame_->KillGameTimer(kFishMsg);

	switch(Msg_fish_id)
	{
	case 1:
		_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("Boss 金龙来袭，倍数 %d 到 %d 倍，加油干掉它！"),fish_multiple_[FISH_KIND_19],fish19_max_multiple_);
		table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
		break;
	case 2:
		lstrcpy(tips_msg,TEXT("Boss 超级炸弹来袭，加油干掉它！"));
		table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
		break;
	case 3:
		_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("李逵来袭倍数 %d 到 %d 倍,加油干掉它"),fish_multiple_[FISH_KIND_21],fishLK_max_multiple_);
		table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
		break;
	case 4:
		_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("Boss 企鹅王来袭，倍数 %d 倍，加油干掉它！"),fish_multiple_[FISH_KIND_20]);
		table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
		break;
	case 5:
		_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("Boss 黄金鲨鱼来袭，倍数 %d 到 %d 倍，加油干掉它！"),fish_multiple_[FISH_KIND_18],fish18_max_multiple_);
		table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
		break;
	default:break;
	}

	return true;
}

bool TableFrameSink::OnTimerMessage(DWORD timer_id, WPARAM bind_param) 
{
	switch (timer_id) 
	{
		case kBulletIonTimer:
		case kBulletIonTimer + 1:
		case kBulletIonTimer + 2:
		case kBulletIonTimer + 3:
		case kBulletIonTimer + 4:
		case kBulletIonTimer + 5:
		case kBulletIonTimer + 6:
		case kBulletIonTimer + 7:
			return OnTimerBulletIonTimeout(WPARAM(timer_id - kBulletIonTimer));
		case kLockTimer:
			return OnTimerLockTimeout(bind_param);
		case kClearTraceTimer:
			{
				TCHAR szdebug[MAX_PATH];
				_sntprintf_s(szdebug, MAX_PATH,TEXT("qlog: lkpy %s  flag=%d debug 库存=%I64d  limit=%I64d  us=%d") , game_service_option_->szServerName,
					m_nloadflag, g_stock_score_, g_lMinStockScore, g_nUserStock);
				OutputDebugString(szdebug);
				return OnTimerClearTrace(bind_param);
			}
		case kBuildSmallFishTraceTimer:
			return OnTimerBuildSmallFishTrace(bind_param);
		case kBuildTicketFishTraceTimer:
			return OnTimerBuildFishTicketTrace(bind_param);
		case kBuildMediumFishTraceTimer:
			return OnTimerBuildMediumFishTrace(bind_param);
		case kBuildBigFishTraceTimer:
			{
				timeindex_+=timeindexadd;
				for(int i=0;i<kBuildFish18TraceCount;i++)
				{
					if(BuildFish18[i].bFish && timeindex_%BuildFish18[i].kBuildFish==0)
					{
						BuildFish18[i].bFish=false;
						OnTimerBuildFish18Trace(bind_param);
						break;
					}
				}
				for(int i=0;i<kBuildFish19TraceCount;i++)
				{
					if(BuildFish19[i].bFish && timeindex_%BuildFish19[i].kBuildFish==0)
					{
						BuildFish19[i].bFish=false;
						OnTimerBuildFish19Trace(bind_param);
						break;
					}
				}
				for(int i=0;i<kBuildFish20TraceCount;i++)
				{
					if(BuildFish20[i].bFish && timeindex_%BuildFish20[i].kBuildFish==0)
					{
						BuildFish20[i].bFish=false;
						OnTimerBuildFish20Trace(bind_param);
						break;
					}
				}
				for(int i=0;i<kBuildFishLKTraceCount;i++)
				{
					if(BuildFishLK[i].bFish && timeindex_%BuildFishLK[i].kBuildFish==0)
					{
						BuildFishLK[i].bFish=false;
						OnTimerBuildFishLKTrace(bind_param);
						break;
					}
				}
				for(int i=0;i<kBuildFishBombTraceCount;i++)
				{
					if(BuildFishBomb[i].bFish && timeindex_%BuildFishBomb[i].kBuildFish==0)
					{
						BuildFishBomb[i].bFish=false;
						OnTimerBuildFishBombTrace(bind_param);
						break;
					}
				}

				for(int i=0;i<kBuildFishSuperBombTraceCount;i++)
				{
					if(BuildFishSuperBomb[i].bFish && timeindex_%BuildFishSuperBomb[i].kBuildFish==0)
					{
						BuildFishSuperBomb[i].bFish=false;
						OnTimerBuildFishSuperBombTrace(bind_param);
						break;
					}
				}

				for(int i=0;i<kBuildFishLockBombTraceCount;i++)
				{
					if(BuildFishLockBomb[i].bFish && timeindex_%BuildFishLockBomb[i].kBuildFish==0)
					{
						BuildFishLockBomb[i].bFish=false;
						OnTimerBuildFishLockBombTrace(bind_param);
						break;
					}
				}	
				
				return true;
			}
		case kWinScoreMaxTimer:
			{
				for(int i=0;i<GAME_PLAYER;i++)
				{
					IServerUserItem *pIUserItem=table_frame_->GetTableUserItem(i);
					if (pIUserItem==NULL)
					{
						continue;
					}
					if(!m_GameLogic.bfresh_[i])
					{
						nfresh_time_[i]++;
					}
					if(nfresh_time_[i]>=180)
					{
						nfresh_time_[i]=0;
						m_GameLogic.bfresh_[i]=true;
					}
					
				}
				
				return true;
			}
		case kBuildFishSanTraceTimer:
			return OnTimerBuildFishSanTrace(bind_param);
		case kBuildFishSiTraceTimer:
			return OnTimerBuildFishSiTrace(bind_param);
		case kBuildFishKingTraceTimer:
			return OnTimerBuildFishKingTrace(bind_param);	
		case kSwitchSceneTimer:
			{
				table_frame_->KillGameTimer(kSwitchSceneTimer);
				table_frame_->SetGameTimer(kSwitchSceneTimer, kSwitchSceneElasped * 1000, kRepeatTimer, 0);

				bool bRet = true;
				DWORD dwRet = WaitForSingleObject(m_hMutex, 0);
				if( dwRet == WAIT_OBJECT_0)
				{
					bRet = OnTimerSwitchScene(bind_param);
					::SetEvent(m_hMutex);
				}
				return bRet;
			}
		case kSceneEndTimer:
			return OnTimerSceneEnd(bind_param);
		case kLKScoreTimer:
			return OnTimerLKScore(bind_param);
		case kFishMsg:
			return OnTimerFishMsg(bind_param);
		case kRereadConfigTimer:
			{
				if (!LoadConfig(false))
				{
					TCHAR szdebug[MAX_PATH];
					_sntprintf_s(szdebug, MAX_PATH,TEXT("qlog: lkpy %s   配置资源解析失败，请检查") , game_service_option_->szServerName);
					OutputDebugString(szdebug);
				}
				return true;
			}
		case kUpdateRoomConfigTimer:  // 第0桌处理
			{			
				m_GameLogic._UpdateRoomData();

				if (g_stockDeduct >0 && g_stockDeduct<1000 && g_stock_score_>g_stockStartDeduct)
				{
					CSingleLock dataLocker(&g_Mutex);
					dataLocker.Lock();
					g_stock_score_ -= (g_stock_score_*g_stockDeduct/1000LL);
				}
				return true;
			}
		case kRefreshRegulation:
			{
				TCHAR szFilePath2[_MAX_PATH + 1]={0};
				GetModuleFileName(NULL, szFilePath2, _MAX_PATH);
				(_tcsrchr(szFilePath2, TEXT('\\')))[1] = 0;

				//构造路径
				TCHAR szIniFile[MAX_PATH]=TEXT("");
				_sntprintf_s(szIniFile, CountArray(szIniFile),MAX_PATH, TEXT("%slkpy_server.ini"),szFilePath2);

				fresh_player=GetPrivateProfileInt(TEXT("FreshRegulation"), TEXT("FreshPlayer"), 1, szIniFile);
				minElapse=GetPrivateProfileInt(TEXT("FreshRegulation"), TEXT("minElapse"), 11, szIniFile);
				maxElapse=GetPrivateProfileInt(TEXT("FreshRegulation"), TEXT("maxElapse"), 23, szIniFile);
				decreasePermillage=GetPrivateProfileInt(TEXT("FreshRegulation"), TEXT("ecreasePermillage"), 550, szIniFile);


				TCHAR szBuffer[MAX_PATH]={0}; // 配置信息
				GetPrivateProfileString(TEXT("FreshRegulation"), TEXT("Bigfishdecreasemul"),TEXT("100"), szBuffer,255, szIniFile);
				fish_decreasemul=_tcstod(szBuffer,NULL);
				if(fish_decreasemul>-0.000001 && fish_decreasemul<0.000001)
					fish_decreasemul=100;
				TCHAR szKey[20]={0};

				for (int i=0;i<7;i++)
				{
					_sntprintf_s(szKey,sizeof(szKey),TEXT("Percent%d"),i);
					if(i==0)
						percentlevel[i]=GetPrivateProfileInt(TEXT("FreshRegulation"), szKey, 10, szIniFile);
					else
						percentlevel[i]=GetPrivateProfileInt(TEXT("FreshRegulation"), szKey, 10, szIniFile);
				}
				table_frame_->KillGameTimer(kRefreshRegulation);

				TCHAR szText[MAX_PATH]={0};
				for( WORD i=0; i<GAME_PLAYER; i++ )
				{
					IServerUserItem * pTableUserItem = table_frame_->GetTableUserItem(i);
					if (pTableUserItem == NULL)
					{
						continue;;
					}

					_sntprintf_s(szText, MAX_PATH, TEXT("[lkpy] User:%s, %s控制状态 playerRegulation: %d"), pTableUserItem->GetNickName(),
						m_GameLogic.player[i].alData.bControl?TEXT("在"):TEXT("不在"), m_GameLogic.player[i].alData.playerRegulation);
					OutputDebugString(szText);

					if (!fresh_player)
					{
						continue;
					}

					if ((!m_GameLogic.bfresh_[i]) || m_GameLogic.player[i].alData.bControl)
					{
						continue;
					}

					if (decreasePermillage>0)  // 采用沉降方式
					{
						int n = rand()%1000;
						if( n<=decreasePermillage)	//下沉
						{
							if( m_GameLogic.player[i].alData.playerRegulation == -4 )	//已经触底
							{
								m_GameLogic.player[i].alData.playerRegulation =1;
							}
							else
							{
								m_GameLogic.player[i].alData.playerRegulation -=1;
							}
						}
						else	//上涨
						{
							if( m_GameLogic.player[i].alData.playerRegulation == 2 )	//已经触顶
							{
								m_GameLogic.player[i].alData.playerRegulation =-2;
							}
							else
							{
								m_GameLogic.player[i].alData.playerRegulation +=1;
							}
						}

						CString strMsg;
						strMsg.Format(TEXT("[lkpy] ontimer_playerRegulation: %d"), m_GameLogic.player[i].alData.playerRegulation);
						OutputDebugString(strMsg);

					}
					else   // 采用随机模式
					{
						int ntotalpercent=0;
						for (int j=0;j<7;j++)
						{
							ntotalpercent+=percentlevel[j];
						}
						int n = rand()%ntotalpercent;
						int nLevel[]={2,1,0,-1,-2,-3,-4};
						int nNoePercent=0;
						for (int j=0;j<7;j++)
						{
							nNoePercent+=percentlevel[j];
							if (n<nNoePercent)
							{
								m_GameLogic.player[i].alData.playerRegulation=nLevel[j];
								break;
							}
						}
					}
				}

				m_GameLogic.Tick();
			
				int nNext = minElapse+rand()%(max(1,(maxElapse-minElapse)));
				table_frame_->SetGameTimer(kRefreshRegulation, nNext*1000, 1, 0);
				return true;
			}
		default:
		ASSERT(FALSE);
	}
	return false;
}

bool TableFrameSink::OnGameMessage(WORD sub_cmdid, void* data, WORD data_size, IServerUserItem* server_user_item) {
	switch (sub_cmdid) {
	case SUB_C_EXCHANGE_FISHSCORE: 
		{
			assert(data_size == sizeof(CMD_C_ExchangeFishScore));
			if (data_size != sizeof(CMD_C_ExchangeFishScore)) 
				return false;

			CMD_C_ExchangeFishScore* exchange_fishscore = static_cast<CMD_C_ExchangeFishScore*>(data);
			if (server_user_item->GetUserStatus() == US_LOOKON) 
				return true;

			SendFishScore();
			return OnSubExchangeFishScore(server_user_item, exchange_fishscore->increase, exchange_fishscore->iExchangeCount);
		}
	case SUB_C_USER_FIRE: 
		{
			assert(data_size == sizeof(CMD_C_UserFire));
			if (data_size != sizeof(CMD_C_UserFire)) 
				return false;

			CMD_C_UserFire* user_fire = static_cast<CMD_C_UserFire*>(data);
			if (server_user_item->GetUserStatus() == US_LOOKON)
				return true;

			SendFishScore();
			return OnSubUserFire(server_user_item, user_fire->bullet_kind, user_fire->angle, user_fire->bullet_mulriple, user_fire->lock_fishid);
		}
	case SUB_C_CATCH_FISH: 
		{
			assert(data_size == sizeof(CMD_C_CatchFish));
			if (data_size != sizeof(CMD_C_CatchFish)) return false;
			CMD_C_CatchFish* hit_fish = static_cast<CMD_C_CatchFish*>(data);
			if (server_user_item->GetUserStatus() == US_LOOKON) return true;
			IServerUserItem* user_item = table_frame_->GetTableUserItem(hit_fish->chair_id);
			if (user_item == NULL) return true;
			if (hit_fish->chair_id != server_user_item->GetChairID()) // 如果不同位置，子弹必须是机器人
			{
				if (!user_item->IsAndroidUser())
				{
					return true;
				}
			}
			SendFishScore();

			if( hit_fish->dwCatchFishTick == m_dwLastCatchFishTick[hit_fish->chair_id])
			{
				return true;
			}
			else
				m_dwLastCatchFishTick[hit_fish->chair_id] = hit_fish->dwCatchFishTick;

			return OnSubCatchFish(user_item, hit_fish->fish_id, hit_fish->bullet_kind, hit_fish->bullet_id, hit_fish->bullet_mulriple);
		}
	case SUB_C_CATCH_SWEEP_FISH: 
		{
			assert(data_size == sizeof(CMD_C_CatchSweepFish));
			if (data_size != sizeof(CMD_C_CatchSweepFish)) return false;
			CMD_C_CatchSweepFish* catch_sweep = static_cast<CMD_C_CatchSweepFish*>(data);
			if (server_user_item->GetUserStatus() == US_LOOKON) return true;
			IServerUserItem* user_item = table_frame_->GetTableUserItem(catch_sweep->chair_id);
			if (user_item == NULL) return true;
			if (catch_sweep->chair_id != server_user_item->GetChairID()) // 如果不同位置，子弹必须是机器人
			{
				if (!user_item->IsAndroidUser())
				{
					return true;
				}
			}

			SendFishScore();

			if( catch_sweep->dwCatchFishTick == m_dwLastSweepFishTick[catch_sweep->chair_id])
			{
				return true;
			}
			else
				m_dwLastSweepFishTick[catch_sweep->chair_id] = catch_sweep->dwCatchFishTick;

			return OnSubCatchSweepFish(user_item, catch_sweep->fish_id, catch_sweep->catch_fish_id, catch_sweep->catch_fish_count,catch_sweep->bulletmul);
		}
	case SUB_C_HIT_FISH_I: 
		{
			assert(data_size == sizeof(CMD_C_HitFishLK));
			if (data_size != sizeof(CMD_C_HitFishLK)) return false;
			CMD_C_HitFishLK* hit_fish = static_cast<CMD_C_HitFishLK*>(data);
			if (server_user_item->GetUserStatus() == US_LOOKON) return true;
			return OnSubHitFishLK(server_user_item, hit_fish->fish_id,hit_fish->bulletmul);
		}
	case SUB_C_STOCK_OPERATE: 
		{
			assert(data_size == sizeof(CMD_C_StockOperate));
			if (data_size != sizeof(CMD_C_StockOperate)) return false;
			CMD_C_StockOperate* stock_operate = static_cast<CMD_C_StockOperate*>(data);
			if (server_user_item->GetUserStatus() == US_LOOKON) return true;
			return OnSubStockOperate(server_user_item, stock_operate->operate_code);
		}
	case SUB_C_USER_FILTER: 
		{
			assert(data_size == sizeof(CMD_C_UserFilter));
			if (data_size != sizeof(CMD_C_UserFilter)) return false;
			CMD_C_UserFilter* user_filter = static_cast<CMD_C_UserFilter*>(data);
			if (server_user_item->GetUserStatus() == US_LOOKON) return true;
			return OnSubUserFilter(server_user_item, user_filter->game_id, user_filter->operate_code);
		}
	case SUB_C_ANDROID_STAND_UP: 
		{
			if (!server_user_item->IsAndroidUser()) return true;
			if (server_user_item->GetUserStatus() == US_LOOKON) return true;
			table_frame_->PerformStandUpAction(server_user_item);
			return true;
		}
	case SUB_C_FISH20_CONFIG: 
		{
			assert(data_size == sizeof(CMD_C_Fish20Config));
			if (data_size != sizeof(CMD_C_Fish20Config)) return false;
			CMD_C_Fish20Config* fish20_config = static_cast<CMD_C_Fish20Config*>(data);
			if (server_user_item->GetUserStatus() == US_LOOKON) return true;
			return OnSubFish20Config(server_user_item, fish20_config->game_id, fish20_config->catch_count, fish20_config->catch_probability);
		}
	case SUB_C_ANDROID_BULLET_MUL: 
		{
// 			assert(data_size == sizeof(CMD_C_AndroidBulletMul));
// 			if (data_size != sizeof(CMD_C_AndroidBulletMul)) return false;
// 			CMD_C_AndroidBulletMul* android_bullet_mul = static_cast<CMD_C_AndroidBulletMul*>(data);
// 			IServerUserItem* user_item = table_frame_->GetTableUserItem(android_bullet_mul->chair_id);
// 			if(NULL == user_item)return true;
// 			if (!user_item->IsAndroidUser()) return true;
// 
// 			ServerBulletInfo* bullet_info = GetBulletInfo(android_bullet_mul->chair_id, android_bullet_mul->bullet_id);
// 			if (bullet_info == NULL) return true;
// 			if (fish_score_[android_bullet_mul->chair_id] + bullet_info->bullet_mulriple < android_bullet_mul->bullet_mulriple) {
// 				table_frame_->PerformStandUpAction(user_item);
// 				return true;
// 			}
// 
// 			CMD_S_UserFire user_fire;
// 			user_fire.bullet_kind = bullet_info->bullet_kind;
// 			user_fire.bullet_id = bullet_info->bullet_id;
// 			user_fire.angle = 0.f;
// 			user_fire.chair_id = user_item->GetChairID();
// 			user_fire.android_chairid = android_chairid_;
// 			user_fire.bullet_mulriple = bullet_info->bullet_mulriple;
// 			user_fire.fish_score = bullet_info->bullet_mulriple - android_bullet_mul->bullet_mulriple;
// 			user_fire.lock_fishid = 0;
// 			table_frame_->SendTableData(user_item->GetChairID(), SUB_S_USER_FIRE, &user_fire, sizeof(user_fire));
// 
// 			fish_score_[android_bullet_mul->chair_id] += bullet_info->bullet_mulriple;
// 			fish_score_[android_bullet_mul->chair_id] -= android_bullet_mul->bullet_mulriple;
// 
// 			bullet_info->bullet_kind = android_bullet_mul->bullet_kind;
// 			bullet_info->bullet_mulriple = android_bullet_mul->bullet_mulriple;

			return true;
		}
	case SUB_C_SWITCH_BULLENT:
		{
			if(fire_count_[server_user_item->GetChairID()]>maxElapse*3)
			{
				// 变化数提高难度
				int nRand=rand()%3;
				if(nRand==0)
					m_GameLogic.SetplayerRegulation(server_user_item->GetChairID(),-3, true);
				else if (nRand==1)
				{
					m_GameLogic.SetplayerRegulation(server_user_item->GetChairID(),-4, true);
				}
				else
				{
					m_GameLogic.SetplayerRegulation(server_user_item->GetChairID(),-2, true);
				}

				CString strMsg;
				strMsg.Format(TEXT("[lkpy] User:%s, %s控制状态(换炮) playerRegulation: %d"), server_user_item->GetNickName(),m_GameLogic.player[server_user_item->GetChairID()].alData.bControl?TEXT("在"):TEXT("不在"),
					m_GameLogic.player[server_user_item->GetChairID()].alData.playerRegulation);
				OutputDebugString(strMsg);
			}
			return true;
		}
	case SUB_MOBILE_GET_Harvest:
		{
			if( server_user_item == NULL )
				return false;

			return OnSubMobileGetHarvest(server_user_item);
		}
	case SUB_C_ONLINE_AWARD_GET:
		{
			if( server_user_item == NULL )
				return false;

			m_GameLogic.GetOnLineTime(server_user_item->GetUserID(), server_user_item->GetChairID());
			return true;
		}
	case SUB_C_ONLINE_AWARD_COM:
		{
			if( server_user_item == NULL )
				return false;

			m_GameLogic.ConfirmAward(server_user_item->GetUserID(), server_user_item->GetChairID());
			return true;
		}
	}
	return false;
}
bool TableFrameSink::OnSubMobileGetHarvest(IServerUserItem* server_user_item)
{
	ASSERT(server_user_item != NULL);
	if( server_user_item == NULL )
		return false;

	WORD chair_id = server_user_item->GetChairID();
	ASSERT(chair_id>=0 && chair_id<GAME_PLAYER);
	if( chair_id<0 || chair_id >= GAME_PLAYER )
		return false;

	CMD_S_FireHarvest FireHarvest;
	ZeroMemory(&FireHarvest, sizeof(FireHarvest));
	FireHarvest.iNowScore = user_score_[chair_id];
	FireHarvest.iNowFishScore = fish_score_[chair_id];
	FireHarvest.iCanExchangeGold = FireHarvest.iNowFishScore*exchange_ratio_userscore_/exchange_ratio_fishscore_;
	CopyMemory(FireHarvest.nFishHarvest, m_nFishHarvestArray[chair_id], sizeof(FireHarvest.nFishHarvest));
	table_frame_->SendTableData(chair_id, SUB_S_NOW_HARVEST, &FireHarvest, sizeof(FireHarvest));
	return true;
}

bool TableFrameSink::OnFrameMessage(WORD sub_cmdid, void* data, WORD data_size, IServerUserItem* server_user_item) {
	return false;
}

bool TableFrameSink::OnActionUserSitDown(WORD chair_id, IServerUserItem* server_user_item, bool lookon_user) 
{
	if (!lookon_user) 
	{
		m_dwLastCatchFishTick[chair_id] = 0;
		m_dwLastSweepFishTick[chair_id] = 0;
		m_dwLastCatchFishTime[chair_id] = 0;
		m_nLastBullet_mul[chair_id] = 0;

		if( chair_id >=0 && chair_id < GAME_PLAYER )
		{
			for( int i=0; i<FISH_KIND_COUNT; i++ )
			{
				m_nFishHarvestArray[chair_id][i]=0;
			}
		}


		m_nNation[chair_id]=0;	
		if (!server_user_item->IsAndroidUser())
		{
		
			char szUserAddr[20]={0};
			DWORD dwClientAddr = server_user_item->GetClientAddr();

			BYTE * pClientAddr=(BYTE *)&dwClientAddr;
			_snprintf_s(szUserAddr,CountArray(szUserAddr),"%d.%d.%d.%d",pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

			//查询IP所属区域
			char szNation[MAX_PATH]={0};
			char szRegion[MAX_PATH]={0};

// 			// 尽快释放
// 			CQQWryDataBase	ipAddressDataBase;                // ip 区域获取
// 			bool bRet = ipAddressDataBase.OnGetNationAndRegion(szUserAddr, szNation, MAX_PATH, szRegion, MAX_PATH);
// 			if( bRet )
// 			{
// 				lstrcatA(szNation,szRegion);
// 
// 				///输m_nNation设为1
// 				char *pDelim=",";
// 				char *pLoserRegion="";
// 
// 				if (m_nLoserRegionPercent > 0)
// 				{
// 					pLoserRegion=strtok(m_szLoserRigion,pDelim);
// 
// 
// 					if(pLoserRegion!=NULL)
// 					{
// 						if ((strstr(szNation,pLoserRegion)!=NULL))
// 						{
// 
// 
// 							m_nNation[chair_id]=1;
// 						}
// 						else
// 						{
// 							while(pLoserRegion=strtok(NULL,pDelim))
// 							{
// 								if ((strstr(szNation,pLoserRegion)!=NULL))
// 								{
// 									m_nNation[chair_id]=1;
// 									break;
// 								}
// 							}
// 						}
// 					}
// 
// 				}
// 
// 			}

		}

		__int64 iUserScore = server_user_item->GetUserScore();
		if( iUserScore < 0L )
			return false;
		DWORD dwUserID = server_user_item->GetUserID();
		DWORD dwClientIP = server_user_item->GetClientAddr();
		m_GameLogic.InitChair(dwUserID, dwClientIP, server_user_item->GetNickName(), chair_id, server_user_item->IsAndroidUser(), iUserScore);

		exchange_fish_score_[chair_id] = 0;
		m_dwPlayTime[chair_id] = (DWORD)time(NULL);
		m_dwLastBulletTime[chair_id]=GetTickCount();
		fish_score_[chair_id] = 0;
		m_GameLogic.bfresh_[chair_id]=true;
		nfresh_time_[chair_id]=true;
		user_score_[chair_id] = server_user_item->GetUserScore();
		user_lotteries_[chair_id] = 0;
		user_ipdaMini_[chair_id] = 0;
		user_musicPlay3_[chair_id] = 0;
		user_bigWhite_[chair_id] = 0;
		user_zippo_[chair_id] = 0;

		if (android_chairid_ == INVALID_CHAIR && !server_user_item->IsAndroidUser())
		{
			android_chairid_ = chair_id;
		}

		if (table_frame_->GetGameStatus() == GAME_STATUS_FREE) 
		{
			table_frame_->StartGame();
			table_frame_->SetGameStatus(GAME_STATUS_PLAY);
			StartAllGameTimer();
			ClearFish();
			table_frame_->SetGameTimer(kBuildBigFishTraceTimer, 1000, kRepeatTimer, 0);							//产生大鱼的定时器
			table_frame_->SetGameTimer(kSwitchSceneTimer, kSwitchSceneElasped * 1000, kRepeatTimer, 0);			//切换场景定时器
			table_frame_->SetGameTimer(kClearTraceTimer, kClearTraceElasped * 1000, kRepeatTimer, 0);
			table_frame_->SetGameTimer(kRefreshRegulation, 20*1000, kRepeatTimer, 0);
			table_frame_->SetGameTimer(kWinScoreMaxTimer, 1000, kRepeatTimer, 0);
		}
		
		//用户坐下
		m_GameLogic.RecordLoginTime(dwUserID, 0);
	}

	return true;
}

bool TableFrameSink::OnActionUserStandUp(WORD chair_id, IServerUserItem* server_user_item, bool lookon_user)
{
	if (lookon_user) return true;

	//把实物的值写入数据库
	m_GameLogic.WriteEntityDate(server_user_item->GetUserID(),user_lotteries_[chair_id],  user_ipdaMini_[chair_id], user_musicPlay3_[chair_id], user_bigWhite_[chair_id], user_zippo_[chair_id]);

	exchange_fish_score_[chair_id] = 0;
	fish_score_[chair_id] = 0;
	user_score_[chair_id] = 0;
	user_lotteries_[chair_id] = 0;
	user_ipdaMini_[chair_id] = 0;
	user_musicPlay3_[chair_id] = 0;
	user_bigWhite_[chair_id] = 0;
	user_zippo_[chair_id] = 0;
	m_GameLogic.bfresh_[chair_id]=true;
	nfresh_time_[chair_id]=true;

	WORD user_count = 0;
	WORD player_count = 0;
	WORD android_chair_id[GAME_PLAYER];
	fire_count_[chair_id]=0;
	for (WORD i = 0; i < GAME_PLAYER; ++i) 
	{
		if (i == chair_id) continue;
		IServerUserItem* user_item = table_frame_->GetTableUserItem(i);
		if (user_item) 
		{
			if (!user_item->IsAndroidUser()) 
				android_chair_id[player_count++] = i;
			++user_count;
		}
	}

	if (player_count == 0) 
	{
		android_chairid_ = INVALID_CHAIR;
	}
	else 
	{
		if (!server_user_item->IsAndroidUser() && chair_id == android_chairid_)
		{
			android_chairid_ = android_chair_id[0];
		}
	}

	if (user_count == 0) 
	{
		if (table_frame_->GetGameStatus() == GAME_STATUS_PLAY)
		{
			table_frame_->ConcludeGame(GAME_STATUS_FREE,NULL,608);
			KillAllGameTimer();
			table_frame_->KillGameTimer(kSwitchSceneTimer);
			table_frame_->KillGameTimer(kClearTraceTimer);
			table_frame_->KillGameTimer(kRefreshRegulation);
			ClearFishTrace(true);
			next_scene_kind_ = SCENE_KIND_1;
			special_scene_ = false;
			timeindex_=0;
			timeindexadd=0;
		}
	}

	if( chair_id >=0 && chair_id < GAME_PLAYER )
	{
		for( int i=0; i<FISH_KIND_COUNT; i++ )
		{
			m_nFishHarvestArray[chair_id][i]=0;
		}
	}

	//用户起立
	m_GameLogic.RecordLoginTime(server_user_item->GetUserID(), 1);
	
	return true;
}

FishTraceInfo* TableFrameSink::ActiveFishTrace() {
	FishTraceInfo* fish_trace_info = NULL;
	if (storage_fish_trace_vector_.size() > 0) {
		fish_trace_info = storage_fish_trace_vector_.back();
		storage_fish_trace_vector_.pop_back();
		active_fish_trace_vector_.push_back(fish_trace_info);
	}

	if (fish_trace_info == NULL) {
		fish_trace_info = new FishTraceInfo;
		
		active_fish_trace_vector_.push_back(fish_trace_info);
	}

	ZeroMemory(fish_trace_info,sizeof(fish_trace_info));
	return fish_trace_info;
}

bool TableFrameSink::FreeFishTrace(FishTraceInfo* fish_trace_info) {
	FishTraceInfoVecor::iterator iter;
	for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) {
		if (fish_trace_info == *iter) {
			active_fish_trace_vector_.erase(iter);
			storage_fish_trace_vector_.push_back(fish_trace_info);

			return true;
		}
	}

	assert(!"FreeFishTrace Failed");
	return false;
}

void TableFrameSink::FreeAllFishTrace() {
	std::copy(active_fish_trace_vector_.begin(), active_fish_trace_vector_.end(), std::back_inserter(storage_fish_trace_vector_));
	active_fish_trace_vector_.clear();
}

FishTraceInfo* TableFrameSink::GetFishTraceInfo(int fish_id) {
	FishTraceInfoVecor::iterator iter;
	FishTraceInfo* fish_trace_info = NULL;
	for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) {
		fish_trace_info = *iter;
		if (fish_trace_info->fish_id == fish_id) return fish_trace_info;
	}
	//assert(!"GetFishTraceInfo:not found fish");
	return NULL;
}

void TableFrameSink::SaveSweepFish(FishKind fish_kind, int fish_id, BulletKind bullet_kind, int bullet_mulriple, DWORD dwUserID) {
	SweepFishInfo sweep_fish;
	sweep_fish.fish_kind = fish_kind;
	sweep_fish.fish_id = fish_id;
	sweep_fish.bullet_kind = bullet_kind;
	sweep_fish.bullet_mulriple = bullet_mulriple;
	sweep_fish.dwUserID = dwUserID;
	sweep_fish_info_vector_.push_back(sweep_fish);
}

bool TableFrameSink::FreeSweepFish(int fish_id) {
	std::vector<SweepFishInfo>::iterator iter;
	for (iter = sweep_fish_info_vector_.begin(); iter != sweep_fish_info_vector_.end(); ) {
		if ((*iter).fish_id == fish_id) {
			iter=sweep_fish_info_vector_.erase(iter);
		}
		else
		{
			++iter;
		}
	}

//	assert(!"FreeSweepFish Failed");
	return true;
}

bool TableFrameSink::FreeSweepFish( SweepFishInfo * sweepFishInfo)
{
	std::vector<SweepFishInfo>::iterator iter;
	SweepFishInfo * pSweepFishInfo=NULL;
	for (iter = sweep_fish_info_vector_.begin(); iter != sweep_fish_info_vector_.end(); ) 
	{
		pSweepFishInfo = &(*iter);
		if (pSweepFishInfo->bullet_kind == sweepFishInfo->bullet_kind && pSweepFishInfo->bullet_mulriple == sweepFishInfo->bullet_mulriple &&
			pSweepFishInfo->dwUserID == sweepFishInfo->dwUserID && pSweepFishInfo->fish_id == sweepFishInfo->fish_id &&
			pSweepFishInfo->fish_kind == sweepFishInfo->fish_kind)
		{
			iter=sweep_fish_info_vector_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	return true;
}

SweepFishInfo* TableFrameSink::GetSweepFish(int fish_id, DWORD dwUserID) {
	std::vector<SweepFishInfo>::iterator iter;
	for (iter = sweep_fish_info_vector_.begin(); iter != sweep_fish_info_vector_.end(); ++iter) {
		if ((*iter).fish_id == fish_id && (*iter).dwUserID == dwUserID) {
			return &(*iter);
		}
	}
	return NULL;
}

ServerBulletInfo* TableFrameSink::ActiveBulletInfo(WORD chairid) {
	ServerBulletInfo* bullet_info = NULL;
	if (storage_bullet_info_vector_.size() > 0) {
		bullet_info = storage_bullet_info_vector_.back();
		storage_bullet_info_vector_.pop_back();
		server_bullet_info_vector_[chairid].push_back(bullet_info);
	}

	if (bullet_info == NULL) {
		bullet_info = new ServerBulletInfo();
		server_bullet_info_vector_[chairid].push_back(bullet_info);
	}

	return bullet_info;
}

bool TableFrameSink::FreeBulletInfo(WORD chairid, ServerBulletInfo* bullet_info) {
	ServerBulletInfoVector::iterator iter;
	for (iter = server_bullet_info_vector_[chairid].begin(); iter != server_bullet_info_vector_[chairid].end(); ++iter) {
		if (bullet_info == *iter) {
			server_bullet_info_vector_[chairid].erase(iter);
			storage_bullet_info_vector_.push_back(bullet_info);
			return true;
		}
	}

	assert(!"FreeBulletInfo Failed");
	return false;
}

void TableFrameSink::FreeAllBulletInfo(WORD chairid) {
	std::copy(server_bullet_info_vector_[chairid].begin(), server_bullet_info_vector_[chairid].end(), std::back_inserter(storage_bullet_info_vector_));
	server_bullet_info_vector_[chairid].clear();
}

ServerBulletInfo* TableFrameSink::GetBulletInfo(WORD chairid, int bullet_id) {
	ServerBulletInfoVector::iterator iter;
	ServerBulletInfo* bullet_info = NULL;
	for (iter = server_bullet_info_vector_[chairid].begin(); iter != server_bullet_info_vector_[chairid].end(); ++iter) {
		bullet_info = *iter;
		if (bullet_info->bullet_id == bullet_id) return bullet_info;
	}

	return NULL;
}

void TableFrameSink::StartAllGameTimer() 
{	
	if(special_scene_)
		return;	

	table_frame_->SetGameTimer(kBuildSmallFishTraceTimer, kBuildSmallFishTraceElasped * 1000, kRepeatTimer, 0);				//生成1-10号鱼的定时器
	table_frame_->SetGameTimer(kBuildMediumFishTraceTimer, kBuildMediumFishTraceElasped * 1000, kRepeatTimer, 0);			//生成11-17号鱼的定时器
	table_frame_->SetGameTimer(kBuildFishSanTraceTimer, kBuildFishSanTraceElasped * 1000, kRepeatTimer, 0);
	table_frame_->SetGameTimer(kBuildFishSiTraceTimer,kBuildFishSiTraceElasped * 1000, kRepeatTimer, 0);
	table_frame_->SetGameTimer(kBuildFishKingTraceTimer, kBuildFishKingTraceElasped  * 1000, kRepeatTimer, 0);
	table_frame_->SetGameTimer(kBuildBigFishTraceTimer, 1000, kRepeatTimer, 0);
	if(ticketCreateTime > 0)
	{
		//table_frame_->SetGameTimer(kBuildTicketFishTraceTimer, ticketCreateTime * 1000, kRepeatTimer, 0);
	}
	
	table_frame_->SetGameTimer(kRereadConfigTimer, 30000, kRepeatTimer, 0);

	timeindexadd=1;
}

void TableFrameSink::KillAllGameTimer() {
	
	table_frame_->KillGameTimer(kBuildSmallFishTraceTimer);
	table_frame_->KillGameTimer(kBuildTicketFishTraceTimer);
	table_frame_->KillGameTimer(kBuildMediumFishTraceTimer);
	table_frame_->KillGameTimer(kBuildBigFishTraceTimer);
	table_frame_->KillGameTimer(kBuildFishSanTraceTimer);
	table_frame_->KillGameTimer(kBuildFishSiTraceTimer);
	table_frame_->KillGameTimer(kBuildFishKingTraceTimer);
	table_frame_->KillGameTimer(kRereadConfigTimer);
	timeindexadd=0;
}

bool TableFrameSink::LoadConfig(bool bRead) {

	TCHAR szFilePath[_MAX_PATH + 1]={0};
	GetModuleFileName(NULL, szFilePath, _MAX_PATH);
	(_tcsrchr(szFilePath, TCHAR('\\')))[1] = 0;
	
	//构造路径
	TCHAR szLogFile[MAX_PATH]=TEXT("");
	TCHAR szIniFile[MAX_PATH]=TEXT("");
	_sntprintf_s(szLogFile, CountArray(szLogFile),MAX_PATH, TEXT("%slkpy_server.log"),szFilePath);
	_sntprintf_s(szIniFile, CountArray(szIniFile),MAX_PATH, TEXT("%slkpy_server.ini"),szFilePath);

	fresh_player=GetPrivateProfileInt(TEXT("FreshRegulation"), TEXT("FreshPlayer"), 1, szIniFile);
	minElapse=GetPrivateProfileInt(TEXT("FreshRegulation"), TEXT("minElapse"), 11, szIniFile);
	maxElapse=GetPrivateProfileInt(TEXT("FreshRegulation"), TEXT("maxElapse"), 23, szIniFile);
	decreasePermillage=GetPrivateProfileInt(TEXT("FreshRegulation"), TEXT("ecreasePermillage"), 550, szIniFile);


	TCHAR szBuffer[MAX_PATH]={0}; // 配置信息
	GetPrivateProfileString(TEXT("FreshRegulation"), TEXT("Bigfishdecreasemul"),TEXT("100"), szBuffer,255, szIniFile);
	fish_decreasemul=_tcstod(szBuffer,NULL);
	

	LockPercent=GetPrivateProfileInt(TEXT("LockPercent"), TEXT("lock"), 1, szIniFile);
	LostLockPercent=GetPrivateProfileInt(TEXT("LockPercent"), TEXT("lostlock"), 1, szIniFile);
	TCHAR szKey[100]={0};

	for (int i=0;i<7;i++)
	{
		_sntprintf_s(szKey,sizeof(szKey),TEXT("Percent%d"),i);
		if(i==0)
			percentlevel[i]=GetPrivateProfileInt(TEXT("FreshRegulation"), szKey, 10, szIniFile);
		else
			percentlevel[i]=GetPrivateProfileInt(TEXT("FreshRegulation"), szKey, 10, szIniFile)+percentlevel[i-1];
	}

	// 读取全局信息
	CSingleLock dataLocker(&g_Mutex);
	dataLocker.Lock();
	if (GetTickCount()-g_dwLastRead>10000)
	{
		g_dwLastRead = GetTickCount();

		ZeroMemory(szBuffer,sizeof(szBuffer));
		GetPrivateProfileString(TEXT("FreshRegulation"), TEXT("UserWinMax"),TEXT("200000000"), szBuffer,255, szIniFile);
		g_user_win_max_=_tstoi64((szBuffer));

		// 读取库存值
		ZeroMemory(szBuffer,sizeof(szBuffer));
		GetPrivateProfileString(TEXT("Stock"), TEXT("DynamicAddStock"),TEXT("0"), szBuffer,255, szIniFile);
		__int64  addStock =_tstoi64((szBuffer));
		ZeroMemory(szBuffer,sizeof(szBuffer));
		GetPrivateProfileString(TEXT("Stock"), TEXT("LimitStock"),TEXT("0"), szBuffer,255, szIniFile);
		__int64  minStock =_tstoi64((szBuffer));
		if (addStock != g_lDynamicAddStockScore)
		{
			g_lDynamicAddStockScore = addStock;
			if (addStock != 0)
			{
				g_stock_score_ += g_lDynamicAddStockScore;
			}
		}
		if (minStock != g_lMinStockScore)
		{
			g_lMinStockScore = minStock;
		}
		g_nUserStock=GetPrivateProfileInt(TEXT("Stock"),TEXT("useStock"), 1, szIniFile);
		g_stockDeduct=GetPrivateProfileInt(TEXT("Stock"),TEXT("stockDeduct"), 0, szIniFile);
		g_stockStartDeduct=GetPrivateProfileInt(TEXT("Stock"),TEXT("stockStartDeduct"), 100000, szIniFile);

		// AwardScore
		int i=0;
		for (i=0; i<CountArray(g_nCtrlScore); ++i)
		{
			_sntprintf_s(szKey,sizeof(szKey),TEXT("LevelScore%d"),i);
			ZeroMemory(szBuffer,sizeof(szBuffer));
			GetPrivateProfileString(TEXT("Stock"), szKey,TEXT(""), szBuffer,255, szIniFile);
			if (_tcslen(szBuffer)<3)
			{
				break;
			}

			TCHAR *pValue=NULL;
			int stockScore =  _tcstol(szBuffer, &pValue, 10);
			if (i != 0)
			{
				if (stockScore<g_nCtrlScore[i-1])
				{
					break;
				}
			}

			double increaseFactor=0;
			double increaseFactorSmall=0;
			pValue = _tcschr(szBuffer, TCHAR(','));
			if (pValue != NULL)
			{
				pValue++;
				increaseFactor = _tcstod(pValue, NULL);

				pValue = _tcschr(pValue, TCHAR(','));
				if (pValue != NULL)
				{
					pValue++;
					increaseFactorSmall = _tcstod(pValue, NULL);
				}
			}

			g_nCtrlScore[i]=stockScore;
			g_increase_factor[i]=increaseFactor;
			g_increase_factorSmall[i]=increaseFactorSmall;
		}
		g_ctrl_crucial_count_ = i;
		
		// 附加扣除
		g_nAdditionalPercent = GetPrivateProfileInt(TEXT("Stock"), TEXT("AdditionalPercent"),0, szIniFile);
		if (g_nAdditionalPercent<0)
		{
			g_nAdditionalPercent=0;
		}
		// 使用历史输赢关联
		g_nUseHistory = GetPrivateProfileInt(TEXT("Stock"), TEXT("useHistory"),1, szIniFile);

		TCHAR szLogText[2048]={0};
		szBuffer[0]=0;
		for (int j=0; j<CountArray(g_dwCapFishCount); ++j)
		{
			_sntprintf_s(szBuffer, 32, TEXT("%u,"), g_dwCapFishCount[j]);
			_tcscat(szLogText, szBuffer);
		}
		WritePrivateProfileString(TEXT("public"),TEXT("CaptCount"),szLogText,szLogFile);
	}	
	dataLocker.Unlock();
		

	//////////////////////////////////////////////////////////////////////////
	TCHAR file_name[MAX_PATH] = { 0 };
	_sntprintf_s(file_name, MAX_PATH, TEXT("%slkpy_config_%d.xml"),szFilePath, game_service_option_->wServerID);

	TiXmlDocument xml_doc;
	if (!xml_doc.LoadFile(CT2A(file_name), TIXML_ENCODING_UTF8)) return false;

	const TiXmlElement* xml_element = xml_doc.FirstChildElement("Config");
	if (xml_element == NULL)
	{
		return false;
	}

	const TiXmlElement* xml_child = NULL;

	int fish_count = 0, bullet_kind_count = 0, flag=0;
	for (xml_child = xml_element->FirstChildElement(); xml_child; xml_child = xml_child->NextSiblingElement()) {
		if (!strcmp(xml_child->Value(), "Flag")){
			const char* attri = xml_child->Attribute("loadFlag");
			char* temp = NULL;
			flag = strtol(attri, &temp, 10);
			break;
		}
	}
	if (bRead || (m_nloadflag!=flag))
	{
		for (xml_child = xml_element->FirstChildElement(); xml_child; xml_child = xml_child->NextSiblingElement()) {
			if (!strcmp(xml_child->Value(), "Stock")) {
				
				xml_child->Attribute("maxBulletProb", &g_maxBulletProb); // 炮数大的有点优势
				xml_child->Attribute("testProbability", &g_testProbability); // 测试玩家捕获率折扣
				xml_child->Attribute("testMinProbability", &g_testMinProbability); // 测试玩家小鱼捕获率折扣

				int nStock_crucial_count = 0;
				for (const TiXmlElement* xml_stock = xml_child->FirstChildElement(); xml_stock; xml_stock = xml_stock->NextSiblingElement()) {
					xml_stock->Attribute("stockScore", &stock_crucial_score_[nStock_crucial_count]);
					xml_stock->Attribute("increaseProbability", &stock_increase_probability_[nStock_crucial_count]);
					++nStock_crucial_count;
					if (nStock_crucial_count >= CountArray(stock_crucial_score_)) break;
				}
				stock_crucial_count_ = nStock_crucial_count;
			}
			else if (bRead && (0 == _stricmp(xml_child->Value(), "ScoreExchange"))) {
				const char* attri = xml_child->Attribute("exchangeRatio");
				char* temp = NULL;
				exchange_ratio_userscore_ = strtol(attri, &temp, 10);
				exchange_ratio_fishscore_ = strtol(temp + 1, &temp, 10);
				xml_child->Attribute("exchangeCount", &exchange_count_);
				CGameLogic::SetRoomOnceUpScore(exchange_count_);
			} else if (bRead &&  (0 ==_stricmp(xml_child->Value(), "Cannon"))) {
				const char* attri = xml_child->Attribute("cannonMultiple");
				char* temp = NULL;
				min_bullet_multiple_ = strtol(attri, &temp, 10);
				max_bullet_multiple_ = strtol(temp + 1, &temp, 10);
				CGameLogic::SetRoomMaxCost(max_bullet_multiple_);
			} else if (!_stricmp(xml_child->Value(), "Bomb")) {
				const char* attri = xml_child->Attribute("BombProbability");
				char* temp = NULL;
				bomb_stock_ = strtol(attri, &temp, 10);
				super_bomb_stock_ = strtol(temp + 1, &temp, 10);
			} else if (!_stricmp(xml_child->Value(), "Fish")) {
				int fish_kind;
				xml_child->Attribute("kind", &fish_kind);
				if (fish_kind >= FISH_KIND_COUNT || fish_kind < 0) return false;
				xml_child->Attribute("speed", &fish_speed_[fish_kind]);
				if (fish_kind == FISH_KIND_18) {
					const char* attri = xml_child->Attribute("multiple");
					char* temp = NULL;
					fish_multiple_[fish_kind] = strtol(attri, &temp, 10);
					fish18_max_multiple_ = strtol(temp + 1, &temp, 10);
				} else if (fish_kind == FISH_KIND_19) {
					const char* attri = xml_child->Attribute("multiple");
					char* temp = NULL;
					fish_multiple_[fish_kind] = strtol(attri, &temp, 10);
					fish19_max_multiple_ = strtol(temp + 1, &temp, 10);
				} else if (fish_kind == FISH_KIND_21) {
					const char* attri = xml_child->Attribute("multiple");
					char* temp = NULL;
					fish_multiple_[fish_kind] = strtol(attri, &temp, 10);
					fishLK_max_multiple_ = strtol(temp + 1, &temp, 10);
				} else {
					xml_child->Attribute("multiple", &fish_multiple_[fish_kind]);
				}
				const char* attri = xml_child->Attribute("BoundingBox");
				char* temp = NULL;
				fish_bounding_box_width_[fish_kind] = strtol(attri, &temp, 10);
				fish_bounding_box_height_[fish_kind] = strtol(temp + 1, &temp, 10);
				if (fish_kind == FISH_KIND_23) {
					bomb_range_width_ = strtol(temp + 1, &temp, 10);
					bomb_range_height_ = strtol(temp + 1, &temp, 10);
				}
				xml_child->Attribute("hitRadius", &fish_hit_radius_[fish_kind]);
				xml_child->Attribute("captureProbability", &fish_capture_probability_[fish_kind]);
				++fish_count;
			} else if (!strcmp(xml_child->Value(), "Ticket")){
				int nCount = 0;
				for (const TiXmlElement* xml_stock = xml_child->FirstChildElement(); xml_stock; xml_stock = xml_stock->NextSiblingElement()) {
					xml_stock->Attribute("createProbability", &ticket_create_probability[nCount]);
					xml_stock->Attribute("captureProbability", &ticket_capture_probability[nCount]);
					++nCount;
					if (nCount >= CountArray(ticket_create_probability)) break;
				}
			} else if (!strcmp(xml_child->Value(), "TicketTime")){
				const char* attri = xml_child->Attribute("ticketCreateTime");
				char* temp = NULL;
				ticketCreateTime = strtol(attri, &temp, 10);
			}else if (!strcmp(xml_child->Value(), "Bullet")) {
				int bullet_kind;
				xml_child->Attribute("kind", &bullet_kind);
				if (bullet_kind >= BULLET_KIND_COUNT || bullet_kind < 0) return false;
				xml_child->Attribute("speed", &bullet_speed_[bullet_kind]);
				xml_child->Attribute("netRadius", &net_radius_[bullet_kind]);
				++bullet_kind_count;
			}
			else if (!strcmp(xml_child->Value(), "LoserConfig")) 
			{
				int nLoserCount = 0;
				int nItemCount =0;
				
				const char* attri = xml_child->Attribute("LoserRegion");
				UTF8ToGB(attri,m_szLoserRigion);

				xml_child->Attribute("LoserRegionPercent", &m_nLoserRegionPercent);

				for (int i=0;i<GAME_PLAYER;i++)
				{
					IServerUserItem *server_user_item=table_frame_->GetTableUserItem(i);
					if(server_user_item==NULL)
						continue;
					m_nNation[i]=0;	
					if (!server_user_item->IsAndroidUser())
					{

						char szUserAddr[20]={0};
						DWORD dwClientAddr = server_user_item->GetClientAddr();

						BYTE * pClientAddr=(BYTE *)&dwClientAddr;
						_snprintf_s(szUserAddr,CountArray(szUserAddr),"%d.%d.%d.%d",pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

						//查询IP所属区域
						char szNation[MAX_PATH]={0};
						char szRegion[MAX_PATH]={0};

// 						// 尽快释放
// 						CQQWryDataBase	ipAddressDataBase;                // ip 区域获取
// 						bool bRet = ipAddressDataBase.OnGetNationAndRegion(szUserAddr, szNation, MAX_PATH, szRegion, MAX_PATH);
// 						if( bRet )
// 						{
// 							lstrcatA(szNation,szRegion);
// 
// 							///输m_nNation设为1
// 							char *pDelim=",";
// 							char *pLoserRegion="";
// 
// 							if (m_nLoserRegionPercent > 0)
// 							{
// 								pLoserRegion=strtok(m_szLoserRigion,pDelim);
// 
// 								if(pLoserRegion!=NULL)
// 								{
// 									if ((strstr(szNation,pLoserRegion)!=NULL))
// 									{
// 										m_nNation[i]=1;
// 									}
// 									else
// 									{
// 										while(pLoserRegion=strtok(NULL,pDelim))
// 										{
// 											if ((strstr(szNation,pLoserRegion)!=NULL))
// 											{
// 												m_nNation[i]=1;
// 												break;
// 											}
// 										}
// 									}
// 								}
// 							}
// 
// 						}
					}
				}

				xml_child->Attribute("count", &nItemCount);
				for (const TiXmlElement* xml_loser = xml_child->FirstChildElement(); xml_loser; xml_loser = xml_loser->NextSiblingElement())
				{
					xml_loser->Attribute("gameid", (int *)&m_dwLoserGameID[nLoserCount]);
					xml_loser->Attribute("loserPercent", &m_nLoserPercent[nLoserCount]);
					++nLoserCount;
					if ((nLoserCount>=USER_COUNT)||(nLoserCount>nItemCount))
					{
						break;
					}
				}
				m_nLoserCount = nLoserCount;
			}
			//else if(!strcmp(xml_child->Value(), "LockPercent"))
			//{
			//	xml_child->Attribute("lock",(int *)&LockPercent);
			//	xml_child->Attribute("lostlock",(int *)&LostLockPercent);
			//}
			//else if (!strcmp(xml_child->Value(), "FishCreate"))
			//{
			//	int iCreat_kind = 0;
			//	xml_child->Attribute("kind", &iCreat_kind);
			//	if (iCreat_kind >= CREATE_KIND_COUNT || iCreat_kind < 0)
			//		return false;
			//	xml_child->Attribute("time" , (int*)&CreateFishKind_Time[iCreat_kind]);

			//	const char* attri = xml_child->Attribute("count");
			//	char* temp = NULL;
			//	CreateFishKind_Min_Count[iCreat_kind] = strtol(attri, &temp, 10);
			//	CreateFishKind_Max_Count[iCreat_kind] = strtol(temp + 1, &temp, 10);				
			//}

// 			else if (!strcmp(xml_child->Value(), "WinConfig"))
// 			{
// 				const char* attri = xml_child->Attribute("WinRegion");
// 				UTF8ToGB(attri,m_szWinRigion);
// 			}
			else if (!strcmp(xml_child->Value(), "FishMunber"))
			{
				xml_child->Attribute("fishtime",(int *)&fishtime_);
				xml_child->Attribute("firetime",(int *)&firetime_);
				xml_child->Attribute("maxBulletNum", (int *)&maxBulletNum_);
			}
		}
		if (fish_count != FISH_KIND_COUNT) return false;
		if (bullet_kind_count != BULLET_KIND_COUNT) return false;
		m_nloadflag = flag;
	}

	return true;
}

BOOL TableFrameSink::UTF8ToGB(const char* pszSrc,char szNation[MAX_PATH])
{
	if( pszSrc==NULL )
		return FALSE ;

	ZeroMemory(szNation, sizeof(szNation));
	CString str;
	int		nLength;
	WCHAR *	pBuffer;
	int		nLanguage = CP_UTF8 ;

	nLength = MultiByteToWideChar( nLanguage,0, pszSrc, strlen(pszSrc), NULL, 0 );
	pBuffer=new wchar_t[nLength+1];
	MultiByteToWideChar( nLanguage,0,pszSrc, strlen(pszSrc),pBuffer,nLength);
	pBuffer[nLength]=0;

	int nResultLength=WideCharToMultiByte( CP_ACP,0,pBuffer,nLength,NULL,0,NULL,NULL );
	char* pResult=new char[nResultLength+1];
	WideCharToMultiByte( CP_ACP,0,pBuffer,nLength,pResult,nResultLength,NULL,NULL);
	pResult[nResultLength]=0;

	delete []pBuffer;

	strcpy_s(szNation, MAX_PATH, pResult);

	delete []pResult;

	return TRUE ;

}

bool TableFrameSink::SendGameConfig(IServerUserItem* server_user_item) {
	CMD_S_GameConfig game_config;
	ZeroMemory(&game_config,sizeof(game_config));
	game_config.exchange_ratio_userscore = exchange_ratio_userscore_;
	game_config.exchange_ratio_fishscore = exchange_ratio_fishscore_;
	game_config.exchange_count = exchange_count_;
	game_config.min_bullet_multiple = min_bullet_multiple_;
	game_config.max_bullet_multiple = max_bullet_multiple_;
	game_config.bomb_range_width = bomb_range_width_;
	game_config.bomb_range_height = bomb_range_height_;
	game_config.lock_percent = LockPercent;
	game_config.lostlock_percent = LostLockPercent;
	game_config.firetime_ = firetime_;
	game_config.fish_time = fishtime_;
	game_config.max_bullet_num = maxBulletNum_;

	for (int i = 0; i < FISH_KIND_COUNT; ++i) {
		game_config.fish_multiple[i] = fish_multiple_[i];
		game_config.fish_speed[i] = fish_speed_[i];
		game_config.fish_bounding_box_width[i] = fish_bounding_box_width_[i];
		game_config.fish_bounding_box_height[i] = fish_bounding_box_height_[i];
		game_config.fish_hit_radius[i] = fish_hit_radius_[i];
	}

	for (int i = 0; i < BULLET_KIND_COUNT; ++i) {
		game_config.bullet_speed[i] = bullet_speed_[i];
		game_config.net_radius[i] = net_radius_[i];
	}

	return table_frame_->SendUserItemData(server_user_item, SUB_S_GAME_CONFIG, &game_config, sizeof(game_config));
}

int TableFrameSink::GetNewFishID() {
	++fish_id_;
	if (fish_id_ <= 0) fish_id_ = 1;
	return fish_id_;
}

int TableFrameSink::GetBulletID(WORD chairid) {
	ASSERT(chairid < GAME_PLAYER);
	++bullet_id_[chairid];
	if (bullet_id_[chairid] <= 0) bullet_id_[chairid] = 1;
	return bullet_id_[chairid];
}

bool TableFrameSink::OnTimerBuildSmallFishTrace(WPARAM bind_param) {
	if (CServerRule::IsForfendGameEnter(game_service_option_->dwServerRule)) {
		OnEventGameConclude(GAME_PLAYER, NULL, GER_DISMISS);
		return true;
	}
	int fishcount=4 + rand() % 8;
	BuildFishTrace(fishcount, FISH_KIND_1, FISH_KIND_10);
	return true;
}

bool TableFrameSink::OnTimerBuildMediumFishTrace(WPARAM bind_param) {
	BuildFishTrace(1 + rand() % 2, FISH_KIND_11, FISH_KIND_17);	
	return true;
}

  
bool TableFrameSink::OnTimerBuildFish18Trace(WPARAM bind_param) {
	BuildFishTrace(1, FISH_KIND_18, FISH_KIND_18);
	OnTimerFishMsg(5);
	return true;
}

bool TableFrameSink::OnTimerBuildFish19Trace(WPARAM bind_param) {
	BuildFishTrace(1, FISH_KIND_19, FISH_KIND_19);
	table_frame_->SetGameTimer(kFishMsg, 5 * 1000, 1, 1);
	OnTimerFishMsg(1);
	return true;
}

bool TableFrameSink::OnTimerBuildFish20Trace(WPARAM bind_param) {
	OnTimerFishMsg(4);
	BuildFishTrace(1, FISH_KIND_20, FISH_KIND_20);
	table_frame_->SetGameTimer(kFishMsg, 8 * 1000, 1, 4);
	return true;
}

bool TableFrameSink::OnTimerBuildFishLKTrace(WPARAM bind_param) 
{
	DWORD dwRet = WaitForSingleObject(m_hMutex, 0);
	if( dwRet == WAIT_OBJECT_0)
	{
		OnTimerFishMsg(3);
		CMD_S_FishTrace fish_trace;

		DWORD build_tick = GetTickCount();
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_21;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();


		fish_trace.fish_id = fish_trace_info->fish_id;
		fish_trace.fish_kind = fish_trace_info->fish_kind;
		fish_trace.init_count = 3;
		fish_trace.trace_type = TRACE_BEZIER;
		fish_trace_info->fish_mul=fish_multiple_[FISH_KIND_21];
		fish_trace_info->hit_fish_score=0;
		//BuildInitTrace(fish_trace.init_pos, fish_trace.init_count, fish_trace.fish_kind, fish_trace.trace_type);

		BuildInitTrace(fish_trace.init_pos, fish_trace.init_count, fish_trace.fish_kind, fish_trace.trace_type);

		table_frame_->SendTableData(INVALID_CHAIR, SUB_S_FISH_TRACE, &fish_trace, sizeof(fish_trace));
		table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_FISH_TRACE, &fish_trace, sizeof(fish_trace));

		table_frame_->SetGameTimer(kLKScoreTimer, kLKScoreElasped * 1000, fishLK_max_multiple_ - fish_multiple_[FISH_KIND_21], fish_trace_info->fish_id);

		//table_frame_->SetGameTimer(kFishMsg, 12 * 1000, 1, 3);
		::SetEvent(m_hMutex);
	}
	
	return true;
}

bool TableFrameSink::OnTimerBuildFishBombTrace(WPARAM bind_param) {
	BuildFishTrace(1, FISH_KIND_23, FISH_KIND_23);
	return true;
}

bool TableFrameSink::OnTimerBuildFishLockBombTrace(WPARAM bind_param) {
	BuildFishTrace(1, FISH_KIND_22, FISH_KIND_22);
	return true;
}

bool TableFrameSink::OnTimerBuildFishTicketTrace(WPARAM bind_param) {

	CString csName = game_service_option_->szServerName;
	if(csName.Compare(TEXT("新手房")) == 0)
	{
		return true;
	}

	if(ticket_create_probability[0] + ticket_create_probability[1] + ticket_create_probability[2] + ticket_create_probability[3] +  ticket_create_probability[4] +  ticket_create_probability[5] +  ticket_create_probability[6] <= 0)
	{
		return true;
	}
	BuildFishTrace(1, FISH_KIND_41, FISH_KIND_41);
	return true;
}

bool TableFrameSink::OnTimerBuildFishSuperBombTrace(WPARAM bind_param) {
		OnTimerFishMsg(2);
	BuildFishTrace(1, FISH_KIND_24, FISH_KIND_24);
	table_frame_->SetGameTimer(kFishMsg, 4 * 1000, 1, 2);
	return true;
}

bool TableFrameSink::OnTimerBuildFishSanTrace(WPARAM bind_param) {
	BuildFishTrace(2, FISH_KIND_25, FISH_KIND_27);
	return true;
}

bool TableFrameSink::OnTimerBuildFishSiTrace(WPARAM bind_param) {
	BuildFishTrace(2, FISH_KIND_28, FISH_KIND_30);
	return true;
}

bool TableFrameSink::OnTimerBuildFishKingTrace(WPARAM bind_param) {
	BuildFishTrace(1, FISH_KIND_31, FISH_KIND_40);
	return true;
}

bool TableFrameSink::OnTimerClearTrace(WPARAM bind_param) {
	ClearFishTrace();
	
	return true;
}

bool TableFrameSink::OnTimerBulletIonTimeout(WPARAM bind_param) {
	WORD chair_id = static_cast<WORD>(bind_param);
	CMD_S_BulletIonTimeout bullet_timeout;
	bullet_timeout.chair_id = chair_id;
	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_BULLET_ION_TIMEOUT, &bullet_timeout, sizeof(bullet_timeout));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_BULLET_ION_TIMEOUT, &bullet_timeout, sizeof(bullet_timeout));

	return true;
}

bool TableFrameSink::OnTimerLockTimeout(WPARAM bind_param) {
	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_LOCK_TIMEOUT);
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_LOCK_TIMEOUT);
	StartAllGameTimer();

	bandrior_fire_=true;
	return true;
}

bool TableFrameSink::OnTimerSwitchScene(WPARAM bind_param) 
{
	KillAllGameTimer();
	ClearFishTrace(true);
	table_frame_->KillGameTimer(kBuildBigFishTraceTimer);
	special_scene_ = true;
	if (next_scene_kind_ == SCENE_KIND_1) 
	{
		BuildSceneKind1();
		table_frame_->SetGameTimer(kSceneEndTimer, (kSceneEndElasped-10) * 1000, 1, 0);			//切换场景历经的时间
	} 
	else if (next_scene_kind_ == SCENE_KIND_2) 
	{
		BuildSceneKind2();
		table_frame_->SetGameTimer(kSceneEndTimer, (kSceneEndElasped+5) * 1000, 1, 0);
	}
	else if (next_scene_kind_ == SCENE_KIND_3) 
	{
		BuildSceneKind3();
		table_frame_->SetGameTimer(kSceneEndTimer, (kSceneEndElasped-10) * 1000, 1, 0);
	} 
	else if (next_scene_kind_ == SCENE_KIND_4) 
	{
		BuildSceneKind4();
		table_frame_->SetGameTimer(kSceneEndTimer, (kSceneEndElasped-5) * 1000, 1, 0);	
	}
	else if (next_scene_kind_ == SCENE_KIND_5) 
	{
		BuildSceneKind5();
		table_frame_->SetGameTimer(kSceneEndTimer, (kSceneEndElasped-24) * 1000, 1, 0);
	}

	next_scene_kind_ = static_cast<SceneKind>((next_scene_kind_ + 1) % SCENE_KIND_COUNT);
	timeindex_=0;

	return true;
}

bool TableFrameSink::OnTimerSceneEnd(WPARAM bind_param) 
{
	special_scene_ = false;
	ClearFish();
	StartAllGameTimer();
	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SCENE_END);
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SCENE_END);
	return true;
}

bool TableFrameSink::OnTimerLKScore(WPARAM bind_param) {

	FishTraceInfoVecor::iterator iter;
	FishTraceInfo* fish_trace_info = NULL;
	for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end();++iter) {
		fish_trace_info = *iter;
		if (fish_trace_info->fish_kind == FISH_KIND_21 && fish_trace_info->fish_mul+(fish_trace_info->hit_fish_score/(5*max_bullet_multiple_))<fishLK_max_multiple_) {
			CMD_S_HitFishLK hit_fish;
			hit_fish.chair_id = 3;
			hit_fish.fish_id = fish_trace_info->fish_id;
			fish_trace_info->fish_mul++;
			fish_trace_info->fish_mul = min(fish_trace_info->fish_mul, fishLK_max_multiple_);
			hit_fish.fish_mulriple = min((fish_trace_info->fish_mul+(fish_trace_info->hit_fish_score/(5*max_bullet_multiple_))),fishLK_max_multiple_);

			table_frame_->SendTableData(INVALID_CHAIR, SUB_S_HIT_FISH_LK, &hit_fish, sizeof(hit_fish));
			table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_HIT_FISH_LK, &hit_fish, sizeof(hit_fish));

		} 
	}
	return true;
}

bool TableFrameSink::SendTableData(WORD sub_cmdid, void* data, WORD data_size, IServerUserItem* exclude_user_item) {
	if (exclude_user_item == NULL) {
		table_frame_->SendTableData(INVALID_CHAIR, sub_cmdid, data, data_size);
	} else {
		IServerUserItem* send_user_item = NULL;
		for (WORD i = 0; i < GAME_PLAYER; ++i) {
			send_user_item = table_frame_->GetTableUserItem(i);
			if (send_user_item == NULL) continue;
			if (send_user_item == exclude_user_item) continue;
			table_frame_->SendTableData(send_user_item->GetChairID(), sub_cmdid, data, data_size);
		}
	}
	table_frame_->SendLookonData(INVALID_CHAIR, sub_cmdid, data, data_size);
	return true;
}

bool TableFrameSink::OnSubExchangeFishScore(IServerUserItem* server_user_item, int increase, __int64 iExchangeCount)
{
	WORD chair_id = server_user_item->GetChairID();

	CMD_S_ExchangeFishScore exchange_fish_score;
	exchange_fish_score.chair_id = chair_id;

	SCORE need_user_score = exchange_ratio_userscore_ * exchange_count_ / exchange_ratio_fishscore_;

	SCORE exchangescore=0;
	__int64 iNowPutScore=0L;

//	if (user_score_[chair_id]+fish_score_[chair_id]*exchange_ratio_userscore_/exchange_ratio_fishscore_<max_bullet_multiple_)
	{
		user_score_[chair_id]=server_user_item->GetUserScore();
	}
	if (increase==1) 
	{
		//if (need_user_score > user_leave_score) 
		//{
		//	if (server_user_item->IsAndroidUser()) 
		//	{
		//		table_frame_->PerformStandUpAction(server_user_item);
		//		return true;
		//	} else {
		//		return true;
		//	}
		//}
		//need_user_score = user_leave_score *exchange_ratio_fishscore_/exchange_ratio_userscore_;		//一次性上完分
		//if (need_user_score<1LL)
		//{
		//	return true;
		//}
		//if (need_user_score>200000000LL)
		//{
		//	need_user_score = 200000000LL;
		//}


		if( need_user_score > user_score_[chair_id] )
			need_user_score = user_score_[chair_id];
		exchangescore=need_user_score;
		need_user_score = need_user_score * exchange_ratio_fishscore_/exchange_ratio_userscore_;

		fish_score_[chair_id] += need_user_score; 
		exchange_fish_score_[chair_id] += need_user_score;

//		exchange_fish_score.swap_fish_score = need_user_score; 

		
		m_GameLogic.AddScore(chair_id, need_user_score, true);
		m_GameLogic.AddlScore(chair_id, -need_user_score*exchange_ratio_userscore_/exchange_ratio_fishscore_);
	} 
	else if (increase==2)
	{
		if (need_user_score > user_score_[chair_id]) 
		{
			if (server_user_item->IsAndroidUser()) 
			{
				table_frame_->PerformStandUpAction(server_user_item);
				return true;
			}
		}
		need_user_score = user_score_[chair_id] *exchange_ratio_fishscore_/exchange_ratio_userscore_;		//一次性上完分
		fish_score_[chair_id] += need_user_score; 
		exchange_fish_score_[chair_id] += need_user_score; 
//		exchange_fish_score.swap_fish_score = need_user_score; 

		m_GameLogic.AddScore(chair_id, need_user_score, true);
		m_GameLogic.AddlScore(chair_id, -need_user_score*exchange_ratio_userscore_/exchange_ratio_fishscore_);
// 		if (need_user_score<1LL)
// 		{
// 			return true;
// 		}
// 		if (need_user_score>200000000LL)
// 		{
// 			need_user_score = 200000000LL;
// 		}

		exchangescore=user_score_[chair_id];
	}
	else if( increase == 3 )	//手机上分
	{
		if( iExchangeCount <= 0 || iExchangeCount > user_score_[chair_id] )
			return true;

		need_user_score = iExchangeCount * exchange_ratio_fishscore_ / exchange_ratio_userscore_;
		fish_score_[chair_id] += need_user_score;
		exchange_fish_score_[chair_id] += need_user_score;
		exchangescore = need_user_score;
		m_GameLogic.AddScore(chair_id, need_user_score, true);
		m_GameLogic.AddlScore(chair_id, -need_user_score*exchange_ratio_userscore_/exchange_ratio_fishscore_);

	}
	else
	{
		if (fish_score_[chair_id] <= 0)
			return true;

		/*if (fish_score_[chair_id] > 0 && fish_score_[chair_id] < exchange_count_) 
		{
		exchange_fish_score_[chair_id] -= fish_score_[chair_id];
		exchange_fish_score.swap_fish_score = -fish_score_[chair_id];
		fish_score_[chair_id] = 0;
		} else if (fish_score_[chair_id] >= exchange_count_) {
		fish_score_[chair_id] -= exchange_count_;
		exchange_fish_score_[chair_id] -= exchange_count_;
		exchange_fish_score.swap_fish_score = -exchange_count_;
		}*/
		exchangescore=-(fish_score_[chair_id]*exchange_ratio_userscore_/exchange_ratio_fishscore_);
		exchange_fish_score_[chair_id] -= fish_score_[chair_id];
		
		//exchangescore=-fish_score_[chair_id];
		fish_score_[chair_id] = 0;
		

		
		__int64 nowScore = m_GameLogic.GetNowScore(chair_id);
		if( nowScore > 0 )
		{
			m_GameLogic.AddlScore(chair_id, nowScore*exchange_ratio_userscore_/exchange_ratio_fishscore_);
			iNowPutScore =  m_GameLogic.player[chair_id].alData.playerNowPutScore*exchange_ratio_userscore_/exchange_ratio_fishscore_;	//清理账户之前，记录下来总的上分
			m_GameLogic.AccountChair(server_user_item->GetUserID(), chair_id);
			m_GameLogic.SetlScore(chair_id, server_user_item->GetUserScore());
		}

		if( chair_id >=0 && chair_id < GAME_PLAYER )
		{
			for( int i=0; i<FISH_KIND_COUNT; i++ )
			{
				m_nFishHarvestArray[chair_id][i]=0;
			}
		}

	}
	user_score_[chair_id]-=exchangescore;
	exchange_fish_score.swap_fish_score = fish_score_[chair_id];
	tagScoreInfo score_info;
	memset(&score_info, 0, sizeof(score_info));
	score_info.cbType = SCORE_TYPE_DRAW;
	score_info.lScore = -exchangescore;
	score_info.lGrade = score_info.lScore;
	//score_info.dwLotteries = 0;

//	timeGetTime();
	TCHAR szDetail[256]={0};
	if(score_info.lScore>0)
	{
	//	lstrcpy(szDetail,TEXT("下分(李逵捕鱼) %d:%d:%d.%d"));
		score_info.lGrade = score_info.lScore - iNowPutScore; //下分时刻，使用所下总分，减去所上总分，得出实际的成绩
		if (score_info.lGrade>0)
		{
			score_info.cbType = SCORE_TYPE_WIN;
		}
		else if (score_info.lGrade<0)
		{
			score_info.cbType = SCORE_TYPE_LOSE;
		}
		else
		{
			score_info.cbType = SCORE_TYPE_DRAW;
		}
	}
	else if(score_info.lScore<0)
	{
		lstrcpy(szDetail,TEXT("上分(李逵捕鱼)"));
		score_info.cbType = SCORE_TYPE_NULL;		//上分的情况下，修改类型，以促使写数据库的时候，写入RecordGameScoreBet表
		score_info.lGrade = 0L;
	}
	table_frame_->WriteUserScore(chair_id,  score_info);
	exchange_fish_score.exchange_fish_score = exchange_fish_score_[chair_id];

	
	exchange_fish_score.user_score=user_score_[chair_id];
	SendTableData(SUB_S_EXCHANGE_FISHSCORE, &exchange_fish_score, sizeof(exchange_fish_score), server_user_item->IsAndroidUser() ? NULL : server_user_item);

	if(!server_user_item->IsAndroidUser())
	{
		exchange_fish_score.exchange_fish_score = exchangescore;
		table_frame_->SendTableData(server_user_item->GetChairID(),SUB_S_EXCHANGE_FISHSCORE,&exchange_fish_score,sizeof(exchange_fish_score));
	}

	return true;
}

bool TableFrameSink::OnSubUserFire(IServerUserItem * server_user_item, BulletKind bullet_kind, float angle, int bullet_mul, int lock_fishid) 
{
	// 没真实玩家机器人不打炮
	if (android_chairid_ == INVALID_CHAIR) return true;
	if (bullet_mul < min_bullet_multiple_ || bullet_mul > max_bullet_multiple_) return false;
	WORD chair_id = server_user_item->GetChairID();
	//assert(fish_score_[chair_id] >= bullet_mul);
	if (fish_score_[chair_id]+server_user_item->GetUserScore()*exchange_ratio_fishscore_/exchange_ratio_userscore_ < bullet_mul) 
	{
		if (server_user_item->IsAndroidUser()) 
		{
			table_frame_->PerformStandUpAction(server_user_item);
			return true;
		} 
	}
	if(fish_score_[chair_id]<bullet_mul)
	{
		return true;
	}
	
	if (server_user_item->IsAndroidUser() && !bandrior_fire_)
	{
		return true;
	}
	fish_score_[chair_id] -= bullet_mul;

	user_score_win_[chair_id]=fish_score_[chair_id]-exchange_fish_score_[chair_id];

	DWORD dwFireTime = GetTickCount();
	if (dwFireTime - m_dwLastFireTime[chair_id]<1 )
	{	
		return true;
	}
	else
	{
		m_dwLastFireTime[chair_id] = dwFireTime;
	}

	if (!server_user_item->IsAndroidUser()) 
	{
		int revenue = game_service_option_->cbRevenueRatio * bullet_mul / 1000;
		int lAdd = bullet_mul - revenue;
		g_revenue_score += revenue;
		CSingleLock dataLocker(&g_Mutex);
		dataLocker.Lock();
		g_stock_score_ += lAdd;
	}	

// 	if (lock_fishid > 0 && GetFishTraceInfo(lock_fishid) == NULL) {
//  		lock_fishid = 0;
// 	}
	fire_count_[chair_id]++;
	CMD_S_UserFire user_fire;
	user_fire.bullet_kind = bullet_kind;
	user_fire.bullet_id = GetBulletID(chair_id);
	user_fire.angle = angle;
	user_fire.chair_id = server_user_item->GetChairID();
	user_fire.android_chairid = server_user_item->IsAndroidUser() ? android_chairid_ : INVALID_CHAIR;
	user_fire.bullet_mulriple = bullet_mul;
	user_fire.fish_score = fish_score_[chair_id];
	user_fire.lock_fishid = lock_fishid;

	SendTableData(SUB_S_USER_FIRE, &user_fire, sizeof(user_fire), NULL);
	ServerBulletInfo* bullet_info = ActiveBulletInfo(chair_id);
	bullet_info->bullet_id = user_fire.bullet_id;
	bullet_info->bullet_kind = user_fire.bullet_kind;
	bullet_info->bullet_mulriple = user_fire.bullet_mulriple;
	DWORD dwBulletTime = GetTickCount();
	if (!server_user_item->IsAndroidUser() && (g_bulletTimeSpace>10))
	{		
		bullet_info->bullet_catch=(dwBulletTime>(g_bulletTimeSpace+m_dwLastBulletTime[chair_id]))?false:true;
		m_dwLastBulletTime[chair_id] = dwBulletTime;
	}
	else
	{
		bullet_info->bullet_catch=true;
	}

	// 打到鱼5s内不能变化增加炮弹
	if ((m_nLastBullet_mul[chair_id] != 0) && (m_nLastBullet_mul[chair_id]<bullet_mul) && (dwBulletTime - m_dwLastCatchFishTime[chair_id] < 4200))
	{
		m_nNotCatchFireCount[chair_id]=1;
	}

	m_nLastBullet_mul[chair_id] = bullet_mul;
	if (m_nNotCatchFireCount[chair_id]>=1)
	{
		m_nNotCatchFireCount[chair_id]++;
		OutputDebugString(TEXT("qlog: lkpy  变炮 进入 ++"));
		if (m_nNotCatchFireCount[chair_id]<150)
		{
			if (bullet_info->bullet_catch && (bullet_mul>min_bullet_multiple_*3))
			{
				bullet_info->bullet_catch = (rand()%1000<130)?true:false;
			}
		}
		else
		{
			m_nNotCatchFireCount[chair_id] = 0;
		}
	}

	m_GameLogic.AddScore(chair_id, -bullet_mul);
	m_GameLogic.AddPlayerShoot(chair_id);

	return true;
}

bool TableFrameSink::OnSubCatchFish(IServerUserItem* server_user_item, int fish_id, BulletKind bullet_kind, int bullet_id, int bullet_mul) 
{
	ServerBulletInfo* bullet_info = GetBulletInfo(server_user_item->GetChairID(), bullet_id);
	if (bullet_info == NULL) 
		return true;

	if( server_user_item == NULL)
		return false;

	WORD chair_id = server_user_item->GetChairID();
	if (bullet_mul < min_bullet_multiple_ || bullet_mul > max_bullet_multiple_) 
	{
		FreeBulletInfo(chair_id, bullet_info);
		return true;
	}
	if (!bullet_info->bullet_catch)
	{
		FreeBulletInfo(chair_id, bullet_info);
		return true;
	}

	if( g_nUserStock > 0 )
	{
		if ((!server_user_item->IsAndroidUser()) && (g_stock_score_ < g_lMinStockScore))
		{
			FreeBulletInfo(chair_id, bullet_info);
			return true;
		}
	}

	FishTraceInfo* fish_trace_info = GetFishTraceInfo(fish_id);

	if (fish_trace_info == NULL) 
	{
		FreeBulletInfo(chair_id, bullet_info);
		return true;
	}
	if (fish_trace_info->fish_kind >= FISH_KIND_COUNT) 
	{
		FreeBulletInfo(chair_id, bullet_info);
		return true;
	}

	//没有游出屏幕之前，或者从屏幕消失之后，都不允许打中了
	if( fish_trace_info->fish_kind > FISH_KIND_17 )
	{
		DWORD now_tick = GetTickCount();
		DWORD pass_tick = now_tick - fish_trace_info->build_tick;

		if( fish_trace_info->fish_kind < FISH_KIND_22 )
		{
			if( pass_tick < 10*1000 || pass_tick > 90*1000)
			{
				FreeBulletInfo(chair_id, bullet_info);

				return true;
			}
		}
		else
		{
			if( pass_tick > 90*1000)
			{
				FreeBulletInfo(chair_id, bullet_info);

				return true;
			}
		}
	}

	if(fish_trace_info->hit_fish_score<0 || fish_trace_info->hit_fish_score>max_bullet_multiple_*500)
		fish_trace_info->hit_fish_score=0;


	if (!(bullet_info->bullet_mulriple == bullet_mul && bullet_info->bullet_kind == bullet_kind)) 
	{
		FreeBulletInfo(chair_id, bullet_info);

		return true;
	}
	
	int fish_multiple = fish_multiple_[fish_trace_info->fish_kind];
	SCORE fish_score = fish_multiple_[fish_trace_info->fish_kind] * bullet_mul;


	fish_trace_info->hit_fish_score+=bullet_mul;

	if (fish_trace_info->fish_kind == FISH_KIND_18)
	{
		int fish18_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish18_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
		fish_multiple = fish18_mul;
		fish_score = fish18_mul * bullet_mul;		

	} 
	else if (fish_trace_info->fish_kind == FISH_KIND_19) 
	{
		int fish19_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish19_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
		fish_score = fish19_mul * bullet_mul;
		fish_multiple = fish19_mul;
		//dummy_fish_score=fish19_mul * dummmy_bullet_mul;

	}
	else if (fish_trace_info->fish_kind == FISH_KIND_21)
	{
		int fish_mul=fish_trace_info->fish_mul+fish_trace_info->hit_fish_score/(5*max_bullet_multiple_);
		if(fish_mul>fishLK_max_multiple_)
		{
			fish_mul=fishLK_max_multiple_;
		}
		fish_score = fish_mul * bullet_mul;
		fish_multiple = fish_mul;
		//dummy_fish_score=current_fish_lk_multiple_ * dummmy_bullet_mul;
	}
	if (bullet_kind >= BULLET_KIND_1_ION && fish_trace_info->fish_kind != FISH_KIND_41) 
	{
		fish_score *= 2;
		m_GameLogic.SetCannonTpye(chair_id, 1);
		//dummy_fish_score *=2;
	}

	////////////////////////////////////////////////////////////////////奖券鱼//////////////////////////////////////////////////////////////////////////////////////////////////
	if( fish_trace_info->fish_kind == FISH_KIND_41)  
	{
		double ticketProbability = static_cast<double>(rand()%1000+1)/1000.0f;	
		if(ticket_capture_probability[fish_trace_info->ticket_fish_kind % 7] < ticketProbability)
		{
			FreeBulletInfo(chair_id, bullet_info);
			return true;
		}

		CMD_S_CatchFish catch_fish = {0};
		catch_fish.bullet_ion = false;
		catch_fish.chair_id = server_user_item->GetChairID();
		catch_fish.fish_id = fish_id;
		catch_fish.fish_kind = fish_trace_info->fish_kind;
		//增加奖券
		if (fish_trace_info->ticket_fish_kind < 7)
		{
			user_lotteries_[chair_id] += ticket_score_[fish_trace_info->ticket_fish_kind];
			catch_fish.fish_score = ticket_score_[fish_trace_info->ticket_fish_kind];
			/*catch_fish.user_fish_score = server_user_item->GetUserInfo()->dwLotteries+user_lotteries_[chair_id];*/
			catch_fish.user_fish_score = user_lotteries_[chair_id];
		}
		else if (fish_trace_info->ticket_fish_kind == 7)
		{
			//ipadmini
			user_ipdaMini_[chair_id]++;		
			catch_fish.fish_score = TPYE_IPAD;
			catch_fish.user_fish_score = 0;
		}
		else if (fish_trace_info->ticket_fish_kind == 8)
		{
			//mp3
			user_musicPlay3_[chair_id]++;
			catch_fish.fish_score = TPYE_MP3;
			catch_fish.user_fish_score = 0;
		}
		else if (fish_trace_info->ticket_fish_kind == 9)
		{
			//bigWhite
			user_bigWhite_[chair_id]++;
			catch_fish.fish_score = TPYE_WHITE;
			catch_fish.user_fish_score = 0;
		}
		else
		{
			//zippo
			user_zippo_[chair_id]++;
			catch_fish.fish_score = TPYE_ZIPPO;
			catch_fish.user_fish_score = 0;
		}
		

		table_frame_->SendTableData(INVALID_CHAIR, SUB_S_CATCH_FISH, &catch_fish, sizeof(catch_fish));
		table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_CATCH_FISH, &catch_fish, sizeof(catch_fish));
		m_nFishHarvestArray[chair_id][fish_trace_info->fish_kind]++;
		FreeFishTrace(fish_trace_info);
		FreeBulletInfo(chair_id, bullet_info);
		// 发送给房间总所有的桌子
		if(catch_fish.fish_score == 5000 || catch_fish.fish_score == 10000)
		{
			TCHAR tips_msg[1024] = { 0 };
			_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("%s 的大侠“%s”获得%I64d奖券!"),
				game_service_option_->szServerName, server_user_item->GetNickName(), catch_fish.fish_score);
			for (int i=0;i<MAX_TABLE; ++i)
			{
				if (g_pTableFrameSink[i] == NULL)
				{
					continue;
				}

				g_pTableFrameSink[i]->table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
			}
		}
		return true;
	}
	////////////////////////////////////////////////////////////////////奖券鱼//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TEST
	double fish_probability = fish_capture_probability_[fish_trace_info->fish_kind];
	int fish20_catch_count = 0;
	double fish20_catch_probability = 0.0;
	bool fish20_config = false;
	if (fish_trace_info->fish_kind == FISH_KIND_20 && !server_user_item->IsAndroidUser())
	{
		fish20_config = CheckFish20Config(server_user_item->GetUserID(), &fish20_catch_count, &fish20_catch_probability);
		if (fish20_config) 
		{
			fish_probability = fish20_catch_probability;
		}
	}
	
	if (!server_user_item->IsAndroidUser())
	{
		// 炸弹库根据库存调整概率
		if (fish_trace_info->fish_kind == FISH_KIND_23 && g_stock_score_ < bomb_stock_)
		{
			FreeBulletInfo(chair_id, bullet_info);
			return true;
		}
		if (fish_trace_info->fish_kind == FISH_KIND_24 && g_stock_score_ < super_bomb_stock_)
		{
			FreeBulletInfo(chair_id, bullet_info);
			return true;
		}
		if (fish_trace_info->fish_kind == FISH_KIND_20 && (g_stock_score_ < fish_multiple*max_bullet_multiple_))
		{
			FreeBulletInfo(chair_id, bullet_info);
			return true;
		}

		int nRandUser = rand()%1000;
		if ((m_nLoserRegionPercent>0) && (m_nNation[server_user_item->GetChairID()]==1))
		{
			if (nRandUser < m_nLoserRegionPercent)
			{
				FreeBulletInfo(chair_id, bullet_info);
				return true;
			}
		}
		if (g_maxBulletProb>0 && max_bullet_multiple_>min_bullet_multiple_ && bullet_mul>min_bullet_multiple_)
		{
			// 2000炮一个台阶，更容易
			int nAll = (max_bullet_multiple_-min_bullet_multiple_)/2000;
			int nSelf = (bullet_mul-min_bullet_multiple_)/2000;
			if (nAll > 0 && nSelf > 0)
			{
				fish_probability = fish_probability*(1+(nSelf*g_maxBulletProb)/(double)nAll);
			}
		}
	}
	else
	{
		if (fish_trace_info->fish_kind<FISH_KIND_12)
		{
			fish_probability *= g_testMinProbability;
		}
		else
		{

			fish_probability *= g_testProbability;
		}
	}

	// 如果使用概率控制
	if (g_nUserStock == 2)
	{	
		SCORE stockScore = g_stock_score_;
		double probability = static_cast<double>(rand()%1000+1)/1000.0f;
		if (stockScore<0)
		{
			if ((!server_user_item->IsAndroidUser()) || (server_user_item->IsAndroidUser() && (probability> fish_probability)))
			{
				FreeBulletInfo(chair_id, bullet_info);
				return true;
			}
		}
		else
		{
			FishKind fish_kind=fish_trace_info->fish_kind;
			bool bFind = false;
			int stock_crucial_count = stock_crucial_count_;
			while ((--stock_crucial_count) >= 0) {
				if (( stockScore >= stock_crucial_score_[stock_crucial_count])){					
					bFind = true;
					if (fish_kind>FISH_KIND_10)
					{
						int nOld = g_last_stock_crucial;
						// 比原来的还好打
						if (stock_crucial_count>=g_last_stock_crucial)
						{
							fish_probability = fish_probability * (stock_increase_probability_[stock_crucial_count] + 1);
							g_last_stock_crucial = stock_crucial_count;
						}
						else
						{
							int nCurIndex = max(0, (g_last_stock_crucial-3));
							if (stock_crucial_count>nCurIndex)
							{
								fish_probability = fish_probability * (stock_increase_probability_[g_last_stock_crucial] + 1);
							}
							else
							{
								fish_probability = fish_probability * (stock_increase_probability_[stock_crucial_count] + 1);
								g_last_stock_crucial = stock_crucial_count;
							}
						}

						if (g_last_stock_crucial != nOld)
						{
							TCHAR szText[MAX_PATH]={0};
							_sntprintf_s(szText, MAX_PATH, TEXT("qlog: lkpy_   last l=%d   cur=%d, nOld=%d"), g_last_stock_crucial, stock_crucial_count, nOld);
							OutputDebugString(szText);
						}
					}
					else
					{
						int nOld = g_last_small_stock_crucial;
						int nSmall_crucial = stock_crucial_count/4;
						if (nSmall_crucial>=g_last_small_stock_crucial)
						{
							fish_probability = fish_probability * (stock_increase_probability_[nSmall_crucial] + 1);  // 小于有变化，但是变化比大鱼慢
							g_last_small_stock_crucial = nSmall_crucial;
						}
						else
						{
							int nCurIndex = max(0, (g_last_small_stock_crucial-2));
							if (nSmall_crucial>nCurIndex)
							{
								fish_probability = fish_probability * (stock_increase_probability_[g_last_small_stock_crucial] + 1);
							}
							else
							{
								fish_probability = fish_probability * (stock_increase_probability_[nSmall_crucial] + 1);
								g_last_small_stock_crucial = nSmall_crucial;
							}
						}
						if (g_last_small_stock_crucial != nOld)
						{
							TCHAR szText[MAX_PATH]={0};
							_sntprintf_s(szText, MAX_PATH, TEXT("qlog: lkpy_  small  last l=%d   cur=%d, nOld=%d"), g_last_small_stock_crucial, nSmall_crucial, nOld);
							OutputDebugString(szText);
						}
					}
					if (probability > fish_probability) {
						FreeBulletInfo(chair_id, bullet_info);
						return true;
					} else {
						break;
					}
				}
			}
			if (!bFind)
			{
				if (probability> fish_probability)
				{
					FreeBulletInfo(chair_id, bullet_info);
					return true;
				}
			}
		}

		// 企鹅
		if (fish_trace_info->fish_kind == FISH_KIND_20 ) 
		{
			if (rand()%1000 < 600)
			{
				FreeBulletInfo(chair_id, bullet_info);
				return true;
			}
		}

	}
	else
	{
		 // 换算等级
		bool bFind = false;
		SCORE stockScore = g_stock_score_;
		int stock_ctrl_count = g_ctrl_crucial_count_;
		while ((--stock_ctrl_count) >= 0) {
			if (( stockScore >= g_nCtrlScore[stock_ctrl_count])){					
				bFind = true;
				g_cur_increase = stock_ctrl_count;
				break;
			}
		}
		if (!bFind)
		{
			g_cur_increase = -1;
		}
	}

	if (fish20_config) 
		AddFish20Config(server_user_item->GetUserID(), fish20_catch_count - 1, fish20_catch_probability);
#endif

	if (fish_trace_info->fish_kind == FISH_KIND_23 || fish_trace_info->fish_kind == FISH_KIND_24  || (fish_trace_info->fish_kind >= FISH_KIND_31 && fish_trace_info->fish_kind <= FISH_KIND_40))
	{
		SaveSweepFish(fish_trace_info->fish_kind, fish_id, bullet_kind, bullet_mul,server_user_item->GetUserID());
		CMD_S_CatchSweepFish catch_sweep_fish;
		catch_sweep_fish.chair_id = chair_id;
		catch_sweep_fish.fish_id = fish_id;
		catch_sweep_fish.bulletmul=bullet_mul;
		table_frame_->SendTableData(server_user_item->IsAndroidUser() ? android_chairid_ : chair_id, SUB_S_CATCH_SWEEP_FISH, &catch_sweep_fish, sizeof(catch_sweep_fish));
		table_frame_->SendLookonData(server_user_item->IsAndroidUser() ? android_chairid_ : chair_id, SUB_S_CATCH_SWEEP_FISH, &catch_sweep_fish, sizeof(catch_sweep_fish));
	} 
	else
	{		
		int fish_multemp=fish_multiple;
		if(fish_trace_info->fish_kind>=FISH_KIND_18 && fish_trace_info->fish_kind<=FISH_KIND_22)
		{
			
			if(fish_decreasemul>-0.0000001&&fish_decreasemul<0.00000001)
			{
				fish_decreasemul=100;
			}
			if(fish_trace_info!=NULL)
				fish_multiple-=(int)(fish_trace_info->hit_fish_score/(max_bullet_multiple_*fish_decreasemul));
		}
		if (g_nUserStock != 2)  // 不是使用概率模式才使用
		{
			bool bkillFish =  m_GameLogic.FishGoldByStyle(chair_id, bullet_mul, fish_multemp, true,server_user_item->IsAndroidUser());
			if( !bkillFish )
			{
				FreeBulletInfo(chair_id, bullet_info);
				return true;
			}
		}
		else
		{
			// 计算分数用的
			m_GameLogic.AddScore(chair_id, fish_score);
		}
		
		fish_multiple=fish_multemp;
		m_dwLastCatchFishTime[chair_id] = GetTickCount();

		if (!server_user_item->IsAndroidUser())
		{
			// 附加扣除的
			SCORE  nAdditional = 0;
			if (g_nAdditionalPercent>0)
			{
				nAdditional= (bullet_mul*g_nAdditionalPercent)/1000;
				if (nAdditional<=0)
				{
					nAdditional = 1;
				}
			}
			CSingleLock dataLocker(&g_Mutex);
			dataLocker.Lock();
			g_stock_score_ -= (fish_score+nAdditional);
			g_additional_score_ += nAdditional;
			++g_dwCapFishCount[fish_trace_info->fish_kind];
		}

		fish_score_[chair_id] += fish_score;
		user_score_win_[chair_id]=fish_score_[chair_id]-exchange_fish_score_[chair_id];

		if(user_score_win_[chair_id]*exchange_ratio_userscore_/exchange_ratio_fishscore_>=g_user_win_max_ && m_GameLogic.bfresh_[chair_id])
		{
			m_GameLogic.bfresh_[chair_id]=false;

			nfresh_time_[chair_id]=0;
		}
		CMD_S_CatchFish catch_fish;
		catch_fish.bullet_ion = fish_multiple >= 15 && (rand() % 100 < 5);
		//catch_fish.bullet_ion = true;
		catch_fish.chair_id = server_user_item->GetChairID();
		catch_fish.fish_id = fish_id;
		catch_fish.fish_kind = fish_trace_info->fish_kind;
		catch_fish.user_fish_score = fish_score_[chair_id];
		catch_fish.fish_score=fish_score;
		if (fish_trace_info->fish_kind == FISH_KIND_22 ) 
		{
			table_frame_->SetGameTimer(kLockTimer, kLockTime * 1000, 1, 0);
			KillAllGameTimer();
		}

		if (catch_fish.bullet_ion) 
		{
			table_frame_->SetGameTimer(kBulletIonTimer + chair_id, kBulletIonTime * 1000, 1, 0);
		}

		table_frame_->SendTableData(INVALID_CHAIR, SUB_S_CATCH_FISH, &catch_fish, sizeof(catch_fish));
		table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_CATCH_FISH, &catch_fish, sizeof(catch_fish));
		if (fish_trace_info->fish_kind == FISH_KIND_20)  //
		{
			// 发送给房间总所有的桌子
			TCHAR tips_msg[1024] = { 0 };
			_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("%s %d桌的企鹅王被大侠%s打死了，获得%I64d鱼币奖励!"),
				game_service_option_->szServerName, table_frame_->GetTableID() + 1, server_user_item->GetNickName(), fish_score);
			for (int i=0;i<MAX_TABLE; ++i)
			{
				if (g_pTableFrameSink[i] == NULL)
				{
					continue;
				}
				
				g_pTableFrameSink[i]->table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
			}
		}
		m_nFishHarvestArray[chair_id][fish_trace_info->fish_kind]++;

		FreeFishTrace(fish_trace_info);
	}
	FreeBulletInfo(chair_id, bullet_info);

	return true;
}

bool TableFrameSink::OnSubCatchSweepFish(IServerUserItem* server_user_item, int fish_id, int* catch_fish_id, int catch_fish_count,int bulletmul) 
{
	WORD chair_id = server_user_item->GetChairID();
	DWORD dwUserID = server_user_item->GetUserID();

	SweepFishInfo* sweep_fish_info = GetSweepFish(fish_id, dwUserID);
	FishTraceInfo* fish_trace_info_sweep = GetFishTraceInfo(fish_id);
	if (sweep_fish_info == NULL) 
	{
		return true;
	}
   	assert(sweep_fish_info->fish_kind == FISH_KIND_23 || sweep_fish_info->fish_kind == FISH_KIND_24 || (sweep_fish_info->fish_kind >= FISH_KIND_31 && sweep_fish_info->fish_kind <= FISH_KIND_40));
	if (!(sweep_fish_info->fish_kind == FISH_KIND_23 ||sweep_fish_info->fish_kind == FISH_KIND_24  || (sweep_fish_info->fish_kind >= FISH_KIND_31 && sweep_fish_info->fish_kind <= FISH_KIND_40)))
	{
		FreeSweepFish(sweep_fish_info);
		return false;
	}


	if( g_nUserStock > 0 )
	{
		if ((!server_user_item->IsAndroidUser()) && (g_stock_score_ < g_lMinStockScore))
		{
			FreeSweepFish(sweep_fish_info);
			return true;
		}
	}
	

	SCORE fish_score = fish_multiple_[sweep_fish_info->fish_kind] * bulletmul;

	int nSumMul = fish_multiple_[sweep_fish_info->fish_kind];

	//int  dummmy_bullet_mul=min_bullet_multiple_;//最小炮弹
	//SCORE dummy_score=2 * dummmy_bullet_mul;
	//SCORE stock_fish_score = 2*  sweep_fish_info->bullet_mulriple;
	//SCORE dummy_fish_score= fish_multiple_[sweep_fish_info->fish_kind]  * dummmy_bullet_mul;

	if (sweep_fish_info->fish_kind == FISH_KIND_18) 
	{
		int fish18_mul = fish_multiple_[sweep_fish_info->fish_kind] + rand() % (fish18_max_multiple_ - fish_multiple_[sweep_fish_info->fish_kind] + 1);
		nSumMul = fish18_mul;
		fish_score = fish18_mul * bulletmul;

		//dummy_fish_score=fish18_mul*dummmy_bullet_mul;

	} 
	else if (sweep_fish_info->fish_kind == FISH_KIND_19)
	{
		int fish19_mul = fish_multiple_[sweep_fish_info->fish_kind] + rand() % (fish19_max_multiple_ - fish_multiple_[sweep_fish_info->fish_kind] + 1);
		nSumMul = fish19_mul;
		fish_score = fish19_mul * bulletmul;
		//dummy_fish_score=fish19_mul*dummmy_bullet_mul;
	}
	FishTraceInfoVecor::iterator iter;
	FishTraceInfo* fish_trace_info = NULL;
	
	int nNumber=0;

	int nMul[2]={0};
	for (int i = 0; i < catch_fish_count; ++i) 
	{
		for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) 
		{
			nNumber++;
			fish_trace_info = *iter;
			if (fish_trace_info->fish_id == catch_fish_id[i]) 
			{
				if (fish_trace_info->fish_kind==FISH_KIND_21)
				{
					int fish_mul=fish_trace_info->fish_mul+fish_trace_info->hit_fish_score/(5*max_bullet_multiple_);
					if(fish_mul>fishLK_max_multiple_)
					{
						fish_mul=fishLK_max_multiple_;
					}
					nSumMul += fish_mul;
					fish_score += fish_mul * bulletmul;
					nMul[0]+=fish_mul;
				}
				else
				{
					
					nSumMul += fish_multiple_[fish_trace_info->fish_kind];
					fish_score += fish_multiple_[fish_trace_info->fish_kind] * bulletmul;
					nMul[1]+=fish_multiple_[fish_trace_info->fish_kind] ;
				}
				
				m_nFishHarvestArray[chair_id][fish_trace_info->fish_kind]++;

				//dummy_fish_score+=fish_multiple_[fish_trace_info->fish_kind]*dummmy_bullet_mul;
				break;
			}
		}
	}
// 	if (sweep_fish_info->fish_kind == FISH_KIND_24)
// 	{
// 		nSumMul = fish_multiple_[sweep_fish_info->fish_kind];
// 		fish_score = fish_multiple_[sweep_fish_info->fish_kind] * sweep_fish_info->bullet_mulriple;
// 	} 
	
	if (sweep_fish_info->bullet_kind >= BULLET_KIND_1_ION) 
	{
		fish_score *= 2;	
		m_GameLogic.SetCannonTpye(chair_id, 1);
		//dummy_fish_score*=2;
	}

	if(fish_decreasemul>-0.0000001&&fish_decreasemul<0.00000001)
	{
		fish_decreasemul=100;
	}
	int nSumMulTemp=nSumMul;
	if(fish_trace_info_sweep!=NULL)
		nSumMul-=(int)(fish_trace_info_sweep->hit_fish_score/(max_bullet_multiple_*fish_decreasemul));

	if (g_stock_score_ < fish_score+80000L)
	{
		FreeSweepFish(sweep_fish_info);
		return true;
	}
	if (g_nUserStock != 2)  // 不是使用概率模式才使用
	{
		bool bkillFish = m_GameLogic.FishGoldByStyle(chair_id, bulletmul, nSumMul, true,server_user_item->IsAndroidUser());
		if( !bkillFish )
		{
			FreeSweepFish(sweep_fish_info);
			return true;
		}
	}
	else
	{
		// 计算分数用的
		m_GameLogic.AddScore(chair_id, fish_score);
	}

	m_dwLastCatchFishTime[chair_id] = GetTickCount();

	TCHAR szFilePath2[_MAX_PATH + 1]={0};
	GetModuleFileName(NULL, szFilePath2, _MAX_PATH);
	(_tcsrchr(szFilePath2, TEXT('\\')))[1] = 0;

	//构造路径
	TCHAR szIniFile[MAX_PATH]=TEXT("");
	_sntprintf_s(szIniFile, CountArray(szIniFile),MAX_PATH, TEXT("%slkpy_server_sweep.log"),szFilePath2);

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	TCHAR szKey[100]={0};
	TCHAR szDeatil[100]={0};
	_sntprintf_s(szDeatil,sizeof(szDeatil),TEXT("%04d:%02d:%02d_%02d:%02d:%02d_%d"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond,table_frame_->GetTableID());


	WritePrivateProfileString(szDeatil,TEXT("昵称"),server_user_item->GetNickName(),szIniFile);
	ZeroMemory(szKey,sizeof(szKey));
	_sntprintf_s(szKey,sizeof(szKey),126,TEXT("%d"),server_user_item->GetUserID());
	WritePrivateProfileString(szDeatil,TEXT("ID"),szKey,szIniFile);
	if (sweep_fish_info->fish_kind == FISH_KIND_24) 
	{
		ZeroMemory(szKey,sizeof(szKey));
		_sntprintf_s(szKey,sizeof(szKey),TEXT("%I64d"),fish_score);
		WritePrivateProfileString(szDeatil,TEXT("爆炸总分数"),szKey,szIniFile);
		ZeroMemory(szKey,sizeof(szKey));
		_sntprintf_s(szKey,sizeof(szKey),TEXT("%d"),nSumMulTemp);
		WritePrivateProfileString(szDeatil,TEXT("爆炸总倍数"),szKey,szIniFile);
		ZeroMemory(szKey,sizeof(szKey));
		_sntprintf_s(szKey,sizeof(szKey),TEXT("%d"),bulletmul);
		WritePrivateProfileString(szDeatil,TEXT("炮数"),szKey,szIniFile);

		ZeroMemory(szKey,sizeof(szKey));
		_sntprintf_s(szKey,sizeof(szKey),TEXT("%d"),nMul[0]);
		WritePrivateProfileString(szDeatil,TEXT("李逵倍数"),szKey,szIniFile);

		ZeroMemory(szKey,sizeof(szKey));
		_sntprintf_s(szKey,sizeof(szKey),TEXT("%d"),nMul[1]);
		WritePrivateProfileString(szDeatil,TEXT("普通倍数"),szKey,szIniFile);

		ZeroMemory(szKey,sizeof(szKey));
		_sntprintf_s(szKey,sizeof(szKey),TEXT("%d"),catch_fish_count);
		WritePrivateProfileString(szDeatil,TEXT("捕获鱼的数量"),szKey,szIniFile);

		ZeroMemory(szKey,sizeof(szKey));
		_sntprintf_s(szKey,sizeof(szKey),TEXT("%d"),fish_multiple_[sweep_fish_info->fish_kind]);
		WritePrivateProfileString(szDeatil,TEXT("超级炸弹倍数"),szKey,szIniFile);


		ZeroMemory(szKey,sizeof(szKey));
		_sntprintf_s(szKey,sizeof(szKey),TEXT("%d"),nNumber);
		WritePrivateProfileString(szDeatil,TEXT("次数"),szKey,szIniFile);
	}
	ZeroMemory(szKey,sizeof(szKey));
	_sntprintf_s(szKey,sizeof(szKey),TEXT("%d"),sweep_fish_info->fish_kind);
	WritePrivateProfileString(szDeatil,TEXT("捕获鱼的种类"),szKey,szIniFile);
	for (int i = 0; i < catch_fish_count; ++i) 
	{
		for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) 
		{
			fish_trace_info = *iter;
			if (fish_trace_info->fish_id == catch_fish_id[i]) 
			{
				//dummy_fish_score+=fish_multiple_[fish_trace_info->fish_kind]*dummmy_bullet_mul;
				active_fish_trace_vector_.erase(iter);
				storage_fish_trace_vector_.push_back(fish_trace_info);
				break;
			}
		}
	}
	if (sweep_fish_info->fish_kind == FISH_KIND_24)
	{
		table_frame_->SetGameTimer(kLockTimer, 5 * 1000, 1, 0);
		KillAllGameTimer();
		bandrior_fire_=false;
	}

	if (!server_user_item->IsAndroidUser())
	{
		// 附加扣除的
		SCORE  nAdditional = 0;
		if (g_nAdditionalPercent>0)
		{
			nAdditional= (bulletmul*g_nAdditionalPercent)/1000;
			if (nAdditional<=0)
			{
				nAdditional = 1;
			}
		}

		CSingleLock dataLocker(&g_Mutex);
		dataLocker.Lock();
		g_stock_score_ -= (fish_score+nAdditional);
		g_additional_score_ += nAdditional;
		if (sweep_fish_info->fish_kind<FISH_KIND_COUNT)
		{
			++g_dwCapFishCount[sweep_fish_info->fish_kind];
		}		
	}

	fish_score_[chair_id] += fish_score;

	user_score_win_[chair_id]=fish_score_[chair_id]-exchange_fish_score_[chair_id];

	if(user_score_win_[chair_id]*exchange_ratio_userscore_/exchange_ratio_fishscore_>=g_user_win_max_ && m_GameLogic.bfresh_[chair_id])
	{
		m_GameLogic.bfresh_[chair_id]=false;
		nfresh_time_[chair_id]=0;
	}

	CMD_S_CatchSweepFishResult catch_sweep_result;
	memset(&catch_sweep_result, 0, sizeof(catch_sweep_result));
	catch_sweep_result.fish_id = fish_id;
	catch_sweep_result.chair_id = chair_id;
	catch_sweep_result.fish_score = fish_score;
	catch_sweep_result.catch_fish_count = catch_fish_count;
	catch_sweep_result.bulletmul=bulletmul;
	catch_sweep_result.user_fish_score=fish_score_[chair_id];
	memcpy(catch_sweep_result.catch_fish_id, catch_fish_id, catch_fish_count * sizeof(int));
	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_CATCH_SWEEP_FISH_RESULT, &catch_sweep_result, sizeof(catch_sweep_result));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_CATCH_SWEEP_FISH_RESULT, &catch_sweep_result, sizeof(catch_sweep_result));

	FishTraceInfo* sweepfish_trace_info = GetFishTraceInfo(fish_id);
	if (sweepfish_trace_info == NULL)
	{
		FreeSweepFish(sweep_fish_info);
		return true;
	}
	if (sweepfish_trace_info->fish_kind >= FISH_KIND_COUNT) 
	{
		FreeSweepFish(sweep_fish_info);
		return true;
	}
	FreeFishTrace(sweepfish_trace_info);
	FreeSweepFish(fish_id);

	return true;
}

bool TableFrameSink::OnSubHitFishLK(IServerUserItem* server_user_item, int fish_id,int bulletmul) {
	FishTraceInfo* fish_trace_info = GetFishTraceInfo(fish_id);
	if (fish_trace_info == NULL) return true;
	if (fish_trace_info->fish_kind != FISH_KIND_21 || fish_trace_info->fish_mul+(fish_trace_info->hit_fish_score/(5*max_bullet_multiple_))>=fishLK_max_multiple_) return true;
	//fish_trace_info->hit_fish_score+=bulletmul;
	CMD_S_HitFishLK hit_fish;
	hit_fish.chair_id = server_user_item->GetChairID();
	hit_fish.fish_id = fish_id;

	hit_fish.fish_mulriple = fish_trace_info->fish_mul+(fish_trace_info->hit_fish_score/(5*max_bullet_multiple_));
	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_HIT_FISH_LK, &hit_fish, sizeof(hit_fish));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_HIT_FISH_LK, &hit_fish, sizeof(hit_fish));

	return true;
	
}

bool TableFrameSink::OnSubStockOperate(IServerUserItem* server_user_item, unsigned char operate_code) {
	if (!CUserRight::IsGameCheatUser(server_user_item->GetUserRight())) return false;

	//CMD_S_StockOperateResult stock_operate_result;
	//stock_operate_result.operate_code = operate_code;
	//if (operate_code == 0)	{
	//	stock_operate_result.stock_score = g_stock_score_;
	//} else if (operate_code == 1) {
	//	stock_operate_result.stock_score = g_stock_score_ = 0;
	//} else if (operate_code == 2) {
	//	//if (game_service_option_->lCellScore<100)  // 不能增加库存
	//	//{
	//	//	if (g_stock_score_<100000)
	//	//	{
	//	//		g_stock_score_ += 10000;
	//	//	}
	//	//	else
	//	//	{
	//	//		g_stock_score_ += 100000;
	//	//	}
	//	//}
	//	//else
	//	//{
	//	//	g_stock_score_ += game_service_option_->lCellScore;
	//	//}
	//	stock_operate_result.stock_score = g_stock_score_;
	//} else if (operate_code == 3) {
	//	stock_operate_result.stock_score = g_revenue_score;
	//}
	//table_frame_->SendUserItemData(server_user_item, SUB_S_STOCK_OPERATE_RESULT, &stock_operate_result, sizeof(stock_operate_result));

	return true;
}

bool TableFrameSink::OnSubFish20Config(IServerUserItem* server_user_item, DWORD game_id, int catch_count, double catch_probability) {
	if (!CUserRight::IsGameCheatUser(server_user_item->GetUserRight())) return false;

	AddFish20Config(game_id, catch_count, catch_probability);

	return true;
}

bool TableFrameSink::OnSubUserFilter(IServerUserItem* server_user_item, DWORD game_id, unsigned char operate_code) {
	if (!CUserRight::IsGameCheatUser(server_user_item->GetUserRight())) return false;

	//AddUserFilter(game_id, operate_code);

	return true;
}

void TableFrameSink::BuildInitTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type) {
	assert(init_count >= 2 && init_count <= 3);
	srand(GetTickCount() + rand() % kResolutionWidth);
	WORD chair_id = rand() % 8;
	int center_x = kResolutionWidth / 2;
	int center_y = kResolutionHeight / 2;
	int factor = rand() % 2 == 0 ? 1 : -1;
	float nindex=0.05f;
	switch (chair_id) {
	case 0:
	case 1:
	case 2:
		init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[0].y = 0.f - static_cast<float>(fish_bounding_box_height_[fish_kind]) * 2;
		init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[1].y = static_cast<float>(center_y + (rand() % center_y));
		init_pos[2].x = static_cast<float>(center_x - factor * (rand() % center_x))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[2].y = static_cast<float>(kResolutionHeight + fish_bounding_box_height_[fish_kind] * 2);
		break;
	case 3:
		init_pos[0].x = static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
		init_pos[0].y = static_cast<float>(center_y + factor* (rand() % center_y))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[1].x = static_cast<float>(center_x - (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y + factor* (rand() % center_y))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[2].x = -static_cast<float>(fish_bounding_box_width_[fish_kind] * 2);
		init_pos[2].y = static_cast<float>(center_y - factor* (rand() % center_y))+(rand() % 3-1)*nindex*(rand()%10);
		break;
	case 5:
	case 6:
	case 4:
		init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[0].y = kResolutionHeight + static_cast<float>(fish_bounding_box_height_[fish_kind] * 2);
		init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[1].y = static_cast<float>(center_y - (rand() % center_y));
		init_pos[2].x = static_cast<float>(center_x - factor * (rand() % center_x))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[2].y = static_cast<float>(-fish_bounding_box_height_[fish_kind] * 2);
		break;
	case 7:
		init_pos[0].x = static_cast<float>(-fish_bounding_box_width_[fish_kind] * 2);
		init_pos[0].y = static_cast<float>(center_y + factor* (rand() % center_y))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[1].x = static_cast<float>(center_x + (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y + factor* (rand() % center_y))+(rand() % 3-1)*nindex*(rand()%20);
		init_pos[2].x = static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
		init_pos[2].y = static_cast<float>(center_y - factor* (rand() % center_y))+(rand() % 3-1)*nindex*(rand()%20);
		break;
	}

	if (trace_type == TRACE_LINEAR && init_count == 2) {
		init_pos[1].x = init_pos[2].x;
		init_pos[1].y = init_pos[2].y;
	}
}

void TableFrameSink::BuildFishTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end) 
{
	DWORD dwRet = WaitForSingleObject(m_hMutex, 0);
	if( dwRet == WAIT_OBJECT_0)
	{
		BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
		WORD send_size = 0;
		CMD_S_FishTrace* fish_trace = reinterpret_cast<CMD_S_FishTrace*>(tcp_buffer);

		DWORD build_tick = GetTickCount();
		srand(build_tick + fish_count * 123321);
		for (int i = 0; i < fish_count; ++i) 
		{
			if (send_size + sizeof(CMD_S_FishTrace) > sizeof(tcp_buffer)) 
			{
				table_frame_->SendTableData(INVALID_CHAIR, SUB_S_FISH_TRACE, tcp_buffer, send_size);
				table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_FISH_TRACE, tcp_buffer, send_size);
				send_size = 0;
			}

			FishTraceInfo* fish_trace_info = ActiveFishTrace();
			fish_trace_info->fish_kind = static_cast<FishKind>(fish_kind_start + (rand() + i) % (fish_kind_end - fish_kind_start + 1));
			fish_trace_info->build_tick = build_tick;
			fish_trace_info->fish_id = GetNewFishID();

			fish_trace->fish_id = fish_trace_info->fish_id;
			fish_trace->fish_kind = fish_trace_info->fish_kind;
			fish_trace_info->hit_fish_score=0;
			fish_trace_info->ticket_fish_kind = 0;
			if (fish_trace_info->fish_kind == FISH_KIND_1 || fish_trace_info->fish_kind == FISH_KIND_2) 
			{
				fish_trace->init_count = 2;
				fish_trace->trace_type = TRACE_LINEAR;
			} 
			else
			{
				fish_trace->init_count = 3;
				fish_trace->trace_type = TRACE_BEZIER;
			}

			if(fish_trace_info->fish_kind == FISH_KIND_41)
			{
				int nResult[7];
				nResult[0] = ticket_create_probability[0];
				nResult[1] = ticket_create_probability[0] + ticket_create_probability[1];
				nResult[2] = ticket_create_probability[0] + ticket_create_probability[1] + ticket_create_probability[2];
				nResult[3] = ticket_create_probability[0] + ticket_create_probability[1] + ticket_create_probability[2] + ticket_create_probability[3];
				nResult[4] = ticket_create_probability[0] + ticket_create_probability[1] + ticket_create_probability[2] + ticket_create_probability[3] + ticket_create_probability[4];
				nResult[5] = ticket_create_probability[0] + ticket_create_probability[1] + ticket_create_probability[2] + ticket_create_probability[3] + ticket_create_probability[4] + ticket_create_probability[5];
				nResult[6] = ticket_create_probability[0] + ticket_create_probability[1] + ticket_create_probability[2] + ticket_create_probability[3] + ticket_create_probability[4] + ticket_create_probability[5] + ticket_create_probability[6];

				int nSelect = rand() % 2;
				int nRand = rand() % nResult[6];			
				int ticket_kind =0;
				if (nSelect == 0)
				{
					if(nRand < nResult[0])
					{					
						fish_trace_info->ticket_fish_kind = 0;					
					}
					else if(nRand < nResult[1] && nRand >= nResult[0])
					{					
						fish_trace_info->ticket_fish_kind = 1;					
					}
					else if(nRand < nResult[2] && nRand >= nResult[1])
					{
						fish_trace_info->ticket_fish_kind = 2;					
					}
					else if(nRand < nResult[3] && nRand >= nResult[2])
					{
						fish_trace_info->ticket_fish_kind = 3;					
					}
					else if(nRand < nResult[4] && nRand >= nResult[3])
					{
						fish_trace_info->ticket_fish_kind = 4;					
					}
					else if(nRand < nResult[5] && nRand >= nResult[4])
					{
						fish_trace_info->ticket_fish_kind = 5;					
					}
					else 
					{					
						fish_trace_info->ticket_fish_kind = 6;					
					}
				}
				else
				{
					fish_trace_info->ticket_fish_kind = 7 + rand() % 4;
				}
				//fish_trace_info->ticket_fish_kind = 7;
				
			}

			BuildInitTrace(fish_trace->init_pos, fish_trace->init_count, fish_trace->fish_kind, fish_trace->trace_type);

			send_size += sizeof(CMD_S_FishTrace);
			++fish_trace;
		}

		if (send_size > 0) {
			table_frame_->SendTableData(INVALID_CHAIR, SUB_S_FISH_TRACE, tcp_buffer, send_size);
			table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_FISH_TRACE, tcp_buffer, send_size);
		}
		::SetEvent(m_hMutex);
	}	
}

void TableFrameSink::BuildSceneKind1() 
{
	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	CMD_S_SwitchScene* critial_section_ = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
	critial_section_->scene_kind = next_scene_kind_;
	DWORD build_tick = GetTickCount();
	critial_section_->fish_count = 0;
	for (int i = 0; i < 100; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_1;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 100;
	for (int i = 0; i < 17; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_3;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 17;
	for (int i = 0; i < 17; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_5;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 17;
	for (int i = 0; i < 30; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_2;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 30;
	for (int i = 0; i < 30; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_4;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 30;
	for (int i = 0; i < 15; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_6;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 15;

	FishTraceInfo* fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_20;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();

	critial_section_->fish_id[critial_section_->fish_count] = fish_trace_info->fish_id;
	critial_section_->fish_kind[critial_section_->fish_count] = fish_trace_info->fish_kind;
	critial_section_->fish_count += 1;
	fish_trace_info->hit_fish_score=0;

	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));

	table_frame_->KillGameTimer(kFishMsg);
	TCHAR tips_msg[1024] = { 0 };
	_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("一大波鱼群即将来袭！"));
	table_frame_->SendGameMessage(tips_msg, SMT_CHAT);

}

void TableFrameSink::BuildSceneKind2() 
{
	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	CMD_S_SwitchScene* critial_section_ = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
	critial_section_->scene_kind = next_scene_kind_;
	DWORD build_tick = GetTickCount();
	critial_section_->fish_count = 0;
	for (int i = 0; i < 200; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_1;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 200;
	for (int i = 0; i < 14; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = static_cast<FishKind>(FISH_KIND_12 + i % 7);
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 14;

	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));

	table_frame_->KillGameTimer(kFishMsg);
	TCHAR tips_msg[1024] = { 0 };
	_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("一大波鱼群即将来袭！"));
	table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
}

void TableFrameSink::BuildSceneKind3() 
{
	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	CMD_S_SwitchScene* critial_section_ = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
	critial_section_->scene_kind = next_scene_kind_;
	DWORD build_tick = GetTickCount();
	critial_section_->fish_count = 0;
	for (int i = 0; i < 50; ++i) {
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_1;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 50;

	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_3;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 40;

	for (int i = 0; i < 30; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_4;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 30;

	FishTraceInfo* fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_21;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();
	fish_trace_info->fish_mul=fish_multiple_[FISH_KIND_21];
	fish_trace_info->hit_fish_score=0;

	critial_section_->fish_id[critial_section_->fish_count] = fish_trace_info->fish_id;
	critial_section_->fish_kind[critial_section_->fish_count] = fish_trace_info->fish_kind;
	critial_section_->fish_count += 1;

	for (int i = 0; i < 50; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_1;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 50;

	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_2;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 40;

	for (int i = 0; i < 30; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_5;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 30;

	fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_19;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();
	fish_trace_info->hit_fish_score=0;

	critial_section_->fish_id[critial_section_->fish_count] = fish_trace_info->fish_id;
	critial_section_->fish_kind[critial_section_->fish_count] = fish_trace_info->fish_kind;
	critial_section_->fish_count += 1;

	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));

	table_frame_->KillGameTimer(kFishMsg);
	TCHAR tips_msg[1024] = { 0 };
	_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("一大波鱼群即将来袭！"));
	table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
}

void TableFrameSink::BuildSceneKind4() 
{
	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	CMD_S_SwitchScene* critial_section_ = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
	critial_section_->scene_kind = next_scene_kind_;
	DWORD build_tick = GetTickCount();
	critial_section_->fish_count = 0;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_11;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_12;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_13;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_14;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_15;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_16;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_17;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_18;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 8;

	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));

	table_frame_->KillGameTimer(kFishMsg);
	TCHAR tips_msg[1024] = { 0 };
	_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("一大波鱼群即将来袭！"));
	table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
}

void TableFrameSink::BuildSceneKind5() 
{
	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	CMD_S_SwitchScene* critial_section_ = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
	critial_section_->scene_kind = next_scene_kind_;
	DWORD build_tick = GetTickCount();
	critial_section_->fish_count = 0;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_1;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 40;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_2;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 40;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_5;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 40;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_3;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 40;
	for (int i = 0; i < 24; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_4;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 24;
	for (int i = 0; i < 24; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_6;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 24;
	for (int i = 0; i < 13; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_7;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 13;
	for (int i = 0; i < 13; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_6;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->hit_fish_score=0;

		critial_section_->fish_id[critial_section_->fish_count + i] = fish_trace_info->fish_id;
		critial_section_->fish_kind[critial_section_->fish_count + i] = fish_trace_info->fish_kind;
	}
	critial_section_->fish_count += 13;

	FishTraceInfo* fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_19;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();
	fish_trace_info->hit_fish_score=0;
	critial_section_->fish_id[critial_section_->fish_count] = fish_trace_info->fish_id;
	critial_section_->fish_kind[critial_section_->fish_count] = fish_trace_info->fish_kind;
	critial_section_->fish_count += 1;

	fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_18;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();
	fish_trace_info->hit_fish_score=0;
	critial_section_->fish_id[critial_section_->fish_count] = fish_trace_info->fish_id;
	critial_section_->fish_kind[critial_section_->fish_count] = fish_trace_info->fish_kind;
	critial_section_->fish_count += 1;

	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));

	table_frame_->KillGameTimer(kFishMsg);
	TCHAR tips_msg[1024] = { 0 };
	_sntprintf_s(tips_msg, CountArray(tips_msg), TEXT("一大波鱼群即将来袭！"));
	table_frame_->SendGameMessage(tips_msg, SMT_CHAT);
}

void TableFrameSink::ClearFishTrace(bool force) {
	if (force) {
		std::copy(active_fish_trace_vector_.begin(), active_fish_trace_vector_.end(), std::back_inserter(storage_fish_trace_vector_));
		active_fish_trace_vector_.clear();
	} else {
		FishTraceInfoVecor::iterator iter;
		FishTraceInfo* fish_trace_info = NULL;
		DWORD now_tick = GetTickCount();
		for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end();) {
			fish_trace_info = *iter;
			if (now_tick >= (fish_trace_info->build_tick + kFishAliveTime)) {
				iter = active_fish_trace_vector_.erase(iter);
				storage_fish_trace_vector_.push_back(fish_trace_info);
			} else {
				++iter;
			}
		}
	}
}

void TableFrameSink::CalcScore(IServerUserItem* server_user_item) {
	if (server_user_item == NULL) return;
	WORD chair_id = server_user_item->GetChairID();

	tagScoreInfo score_info;
	memset(&score_info, 0, sizeof(score_info));	
	score_info.lScore = fish_score_[chair_id]  * exchange_ratio_userscore_ / exchange_ratio_fishscore_;
	//score_info.lGrade = score_info.lScore;
	score_info.lGrade = score_info.lScore-m_GameLogic.player[chair_id].alData.playerNowPutScore*exchange_ratio_userscore_/exchange_ratio_fishscore_;	//总的下分，减去总的上分，得出实际的成绩
	if (score_info.lGrade>0)
	{
		score_info.cbType = SCORE_TYPE_WIN;
	}
	else if (score_info.lGrade<0)
	{
		score_info.cbType = SCORE_TYPE_LOSE;
	}
	else
	{
		score_info.cbType = SCORE_TYPE_DRAW;
	}

	DWORD dwPlayTime =(DWORD)  time(NULL);
	if (dwPlayTime> m_dwPlayTime[chair_id])
	{
		dwPlayTime = dwPlayTime - m_dwPlayTime[chair_id];
	}
	else
	{
		dwPlayTime = 0;
	}
	//score_info.dwLotteries = user_lotteries_[chair_id];
	table_frame_->WriteUserScore(chair_id,  score_info);
	m_dwPlayTime[chair_id] = (DWORD) time(NULL);

	fish_score_[chair_id] = 0;
	exchange_fish_score_[chair_id] = 0;
	//user_lotteries_[chair_id] = 0;
	

	m_GameLogic.ClearChair(server_user_item->GetUserID(), chair_id);

	if( chair_id >=0 && chair_id < GAME_PLAYER )
	{
		for( int i=0; i<FISH_KIND_COUNT; i++ )
		{
			m_nFishHarvestArray[chair_id][i]=0;
		}
	}
}

void TableFrameSink::GetExchangeRatio(int & nUserRatio, int & nFishRatio )
{
	nUserRatio = exchange_ratio_userscore_;
	nFishRatio = exchange_ratio_fishscore_;
}

bool TableFrameSink::IsLostPlayer(WORD wChairID)
{
	IServerUserItem * pIServerUserItem = table_frame_->GetTableUserItem(wChairID);
	if( pIServerUserItem != NULL )
	{
		DWORD dwGameID = pIServerUserItem->GetUserID();

		for( int i=0; i<m_nLoserCount; i++ )
		{
			if( dwGameID == m_dwLoserGameID[i] )
				return true;
		}
	}

	return false;
}

void TableFrameSink::SendFishScore()
{
	CMD_S_FishScore fishscore;
	
	for(int i = 0;i< GAME_PLAYER;i++)
	{
		fishscore.fish_score[i] = fish_score_[i];
	}
	
	SendTableData(SUB_S_FIRE_SCORE, &fishscore, sizeof(fishscore), NULL);
}

void TableFrameSink::ClearFish()
{
	ZeroMemory(BuildFish18,sizeof(BuildFish18));
	ZeroMemory(BuildFish19,sizeof(BuildFish19));
	ZeroMemory(BuildFish20,sizeof(BuildFish20));
	ZeroMemory(BuildFishLK,sizeof(BuildFishLK));
	ZeroMemory(BuildFishBomb,sizeof(BuildFishBomb));
	ZeroMemory(BuildFishSuperBomb,sizeof(BuildFishSuperBomb));
	ZeroMemory(BuildFishLockBomb,sizeof(BuildFishLockBomb));
	int nTotalTimes=0;
	for (int i=0;i<kBuildFish18TraceCount;i++)
	{
		BuildFish18[i].bFish=true;
		BuildFish18[i].kBuildFish=580/kBuildFish18TraceCount+rand()%20-20;
		if(nTotalTimes+BuildFish18[i].kBuildFish>580)
		{
			BuildFish18[i].kBuildFish=(580-nTotalTimes)/(kBuildFish18TraceCount-i);
		}
		nTotalTimes+=BuildFish18[i].kBuildFish;
		if(i!=0)
			BuildFish18[i].kBuildFish+=BuildFish18[i-1].kBuildFish;
	}
	nTotalTimes=0;
	for (int i=0;i<kBuildFish19TraceCount;i++)
	{
		BuildFish19[i].bFish=true;
		BuildFish19[i].kBuildFish=580/kBuildFish19TraceCount+rand()%40-40;
		if(nTotalTimes+BuildFish19[i].kBuildFish>580)
		{
			BuildFish19[i].kBuildFish=(580-nTotalTimes)/(kBuildFish19TraceCount-i);
		}
		nTotalTimes+=BuildFish19[i].kBuildFish;
		if(i!=0)
			BuildFish19[i].kBuildFish+=BuildFish19[i-1].kBuildFish;
	}
	nTotalTimes=0;
	for (int i=0;i<kBuildFish20TraceCount;i++)
	{
		BuildFish20[i].bFish=true;
		BuildFish20[i].kBuildFish=580/kBuildFish20TraceCount+rand()%90-90;
		if(nTotalTimes+BuildFish20[i].kBuildFish>580)
		{
			BuildFish20[i].kBuildFish=(580-nTotalTimes)/(kBuildFish20TraceCount-i);
		}
		nTotalTimes+=BuildFish20[i].kBuildFish;
		if(i!=0)
			BuildFish20[i].kBuildFish+=BuildFish20[i-1].kBuildFish;
	}
	nTotalTimes=0;
	for (int i=0;i<kBuildFishLKTraceCount;i++)
	{
		BuildFishLK[i].bFish=true;
		BuildFishLK[i].kBuildFish=580/kBuildFishLKTraceCount+rand()%5-5;
		if(nTotalTimes+BuildFishLK[i].kBuildFish>580)
		{
			BuildFishLK[i].kBuildFish=(580-nTotalTimes)/(kBuildFishLKTraceCount-i);
		}
		nTotalTimes+=BuildFishLK[i].kBuildFish;
		if(i!=0)
			BuildFishLK[i].kBuildFish+=BuildFishLK[i-1].kBuildFish;
	}
	nTotalTimes=0;
	for (int i=0;i<kBuildFishBombTraceCount;i++)
	{
		BuildFishBomb[i].bFish=true;
		BuildFishBomb[i].kBuildFish=580/kBuildFishBombTraceCount+rand()%5-5;
		if(nTotalTimes+BuildFishBomb[i].kBuildFish>580)
		{
			BuildFishBomb[i].kBuildFish=(580-nTotalTimes)/(kBuildFishBombTraceCount-i);
		}
		nTotalTimes+=BuildFishBomb[i].kBuildFish;
		if(i!=0)
			BuildFishBomb[i].kBuildFish+=BuildFishBomb[i-1].kBuildFish;
	}
	nTotalTimes=0;
	int nFishSuperBombTraceCount=rand()%(kBuildFishSuperBombTraceCount-2)+2;
	for (int i=0;i<nFishSuperBombTraceCount;i++)
	{
		BuildFishSuperBomb[i].bFish=true;
		BuildFishSuperBomb[i].kBuildFish=580/kBuildFishSuperBombTraceCount+rand()%90-90;
		if(nTotalTimes+BuildFishSuperBomb[i].kBuildFish>580)
		{
			BuildFishSuperBomb[i].kBuildFish=(580-nTotalTimes)/(kBuildFishSuperBombTraceCount-i);
		}
		nTotalTimes+=BuildFishSuperBomb[i].kBuildFish;
		if(i!=0)
			BuildFishSuperBomb[i].kBuildFish+=BuildFishSuperBomb[i-1].kBuildFish;
	}
	nTotalTimes=0;
	for (int i=0;i<kBuildFishLockBombTraceCount;i++)
	{
		BuildFishLockBomb[i].bFish=true;
		BuildFishLockBomb[i].kBuildFish=580/kBuildFishLockBombTraceCount+rand()%10-10;
		if(nTotalTimes+BuildFishLockBomb[i].kBuildFish>580)
		{
			BuildFishLockBomb[i].kBuildFish=(580-nTotalTimes)/(kBuildFishLockBombTraceCount-i);
		}
		nTotalTimes+=BuildFishLockBomb[i].kBuildFish;
		if(i!=0)
			BuildFishLockBomb[i].kBuildFish+=BuildFishLockBomb[i-1].kBuildFish;
	}

	nTotalTimes=0;
	for (int i=0;i<kBuildFishTicketTraceCount;i++)
	{
		BuildFishTicket[i].bFish=true;
		BuildFishTicket[i].kBuildFish=580/kBuildFishTicketTraceCount+rand()%10-10;
		if(nTotalTimes+BuildFishTicket[i].kBuildFish>580)
		{
			BuildFishTicket[i].kBuildFish=(580-nTotalTimes)/(kBuildFishTicketTraceCount-i);
		}
		nTotalTimes+=BuildFishTicket[i].kBuildFish;
		if(i!=0)
			BuildFishTicket[i].kBuildFish+=BuildFishTicket[i-1].kBuildFish;
	}

	timeindex_=0;
	table_frame_->SetGameTimer(kBuildBigFishTraceTimer, 1000, kRepeatTimer, 0);				//产生大鱼的定时器
	
}

void TableFrameSink::GetOnLineTime(WORD wChairID, int nAllStep, int nCurStep, int nTotalTime)
{
	CMD_S_OnLineAward onLine_Award;
	onLine_Award.chair_id = wChairID;
	onLine_Award.nAllStep = nAllStep;
	onLine_Award.nCurStep = nCurStep;
	onLine_Award.nTotalTime = nTotalTime;
	
	//向客户端发送在线时长结果
	SendTableData(SUB_S_ONLINE_AWARD, &onLine_Award, sizeof(onLine_Award), NULL);
}

void TableFrameSink::OnUpdateUserCoin(DWORD dwUserID, WORD wChairID, int nStep)
{
// 	fish_score_[wChairID] += 1008;
// 	fish_score_[wChairID] += 1000;

	if (nStep <= 0)
	{
		return;
	}

	nStep = nStep - 1;
	int nScore = 0;

	if (nStep == 0)
	{
		nScore = 500;
	}
	else if (nStep == 1)
	{
		nScore = 1000;
	}
	else if (nStep == 2)
	{
		nScore = 2000;
	}
	else if (nStep == 3)
	{
		nScore = 5000;
	}
	else if (nStep == 4)
	{
		nScore = 10000;
	}
	else if (nStep == 5)
	{
		nScore = 20000;
	}

	m_GameLogic.AddScore(wChairID, nScore, true);
	m_GameLogic.AddlScore(wChairID, -nScore * exchange_ratio_userscore_/exchange_ratio_fishscore_);

	fish_score_[wChairID] += nScore;


	//向客户端发送赠送金币
	CMD_S_AwardScore award_score;
	award_score.chair_id = wChairID;
	award_score.curStep = nStep;
	award_score.award_score = fish_score_[wChairID];;

	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_AWARD_RESULT, &award_score, sizeof(award_score));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_AWARD_RESULT, &award_score, sizeof(award_score));
}
