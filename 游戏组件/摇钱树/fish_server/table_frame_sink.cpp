
#include "StdAfx.h"
#include "table_frame_sink.h"
#include "tinyxml/tinyxml.h"
#include <iterator>

// 测试标志 如果定义此标志 库存和概率无效
//#define TEST

// 应当注意：计时器ID的范围是1-20 ( >=1 && < 20 )
// 这里有超过20的 所以要把GameServiceHead.h的TIME_TABLE_SINK_RANGE改大点才行，或者改进计时器，比如共用
const DWORD kBulletIonTimer = 1;
const DWORD kLockTimer = 9;
const DWORD kClearTraceTimer = 10;
const DWORD kBuildSmallFishTraceTimer = 11;
const DWORD kBuildMediumFishTraceTimer = 12;
const DWORD kBuildFish18TraceTimer = 13;
const DWORD kBuildFish19TraceTimer = 14;
const DWORD kBuildFish20TraceTimer = 15;
const DWORD kBuildFishLKTraceTimer = 16;
const DWORD kBuildFishBombTraceTimer = 17;
const DWORD kBuildFishSuperBombTraceTimer = 18;
const DWORD kBuildFishLockBombTraceTimer = 19;
const DWORD kBuildFishSanTraceTimer = 2;
const DWORD kBuildFishSiTraceTimer = 3;
const DWORD kBuildFishKingTraceTimer = 4;
const DWORD kSwitchSceneTimer = 5;
const DWORD kSceneEndTimer = 6;
const DWORD kLKScoreTimer = 7;
const DWORD kRereadConfigTimer = 8;
const DWORD kAndroidShot = 20;

const DWORD kClearTraceElasped = 60;
const DWORD kBuildSmallFishTraceElasped = 4;
const DWORD kBuildMediumFishTraceElasped = 5;
const DWORD kBuildFish18TraceElasped = 122;				//2分钟
const DWORD kBuildFish19TraceElasped = 61;				//1分钟
const DWORD kBuildFish20TraceElasped = 183;				//3分钟
const DWORD kBuildFishLKTraceElasped = 254;				//4分钟
const DWORD kBuildFishBombTraceElasped = 335;			//5分钟
const DWORD kBuildFishSuperBombTraceElasped = 366;		//6分钟
const DWORD kBuildFishLockBombTraceElasped = 437;		//7分钟
const DWORD kBuildFishSanTraceElasped = 1;			//8分半
const DWORD kBuildFishSiTraceElasped = 2;				//9分半				
const DWORD kBuildFishKingTraceElasped = 3;			//9分钟
const DWORD kSwitchSceneElasped = 600;		   //切换场景的时间10分钟
const DWORD kSceneEndElasped = 63;
const DWORD kLKScoreElasped = 1;

const DWORD kRepeatTimer = (DWORD)0xFFFFFFFF;
const DWORD kFishAliveTime = 180000;

static SCORE g_stock_score_ = 0;
static SCORE g_revenue_score = 0;

static std::vector<DWORD> g_balck_list_;
static std::vector<DWORD> g_white_list_;

struct Fish20Config {
  DWORD game_id;
  int catch_count;
  double catch_probability;
};
static std::vector<Fish20Config> g_fish20_config_;

void AddUserFilter(DWORD game_id, unsigned char operate_code) 
{
  std::vector<DWORD>::iterator iter;
  if (operate_code == 0) 
  {
    for (iter = g_balck_list_.begin(); iter != g_balck_list_.end(); ++iter) 
	{
      if ((*iter) == game_id) return;
    }
    g_balck_list_.push_back(game_id);
  } 
  else if (operate_code == 1) 
  {
    for (iter = g_white_list_.begin(); iter != g_white_list_.end(); ++iter) 
	{
      if ((*iter) == game_id) return;
    }
    g_white_list_.push_back(game_id);
  } 
  else 
  {
    for (iter = g_balck_list_.begin(); iter != g_balck_list_.end(); ++iter) 
	{
      if ((*iter) == game_id) {
        iter = g_balck_list_.erase(iter);
        break;
      }
    }
    for (iter = g_white_list_.begin(); iter != g_white_list_.end(); ++iter) 
	{
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
  current_fish_lk_multiple_(50), android_chairid_(INVALID_CHAIR) 
{
  for (WORD i = 0; i < GAME_PLAYER; ++i) {
    fish_score_[i] = 0;
    exchange_fish_score_[i] = 0;
    bullet_id_[i] = 0;
  }
  bomb_range_width_ = 0;
  bomb_range_height_ = 0;
  memset(stock_crucial_score_, 0, sizeof(stock_crucial_score_));
  memset(stock_increase_probability_, 0, sizeof(stock_increase_probability_));
  stock_crucial_count_ = 0;

	ZeroMemory(CreateFishKind_Time, sizeof(CreateFishKind_Time));
	ZeroMemory(CreateFishKind_Min_Count, sizeof(CreateFishKind_Min_Count));
	ZeroMemory(CreateFishKind_Max_Count, sizeof(CreateFishKind_Max_Count));
}

TableFrameSink::~TableFrameSink() 
{
  FishTraceInfoVecor::iterator iter;
  for (iter = storage_fish_trace_vector_.begin(); iter != storage_fish_trace_vector_.end(); ++iter) 
  {
    delete (*iter);
  }
  storage_fish_trace_vector_.clear();
  for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) 
  {
    delete (*iter);
  }
  active_fish_trace_vector_.clear();

  ServerBulletInfoVector::iterator it;
  for (it = storage_bullet_info_vector_.begin(); it != storage_bullet_info_vector_.end(); ++it) {
    delete (*it);
  }
  storage_bullet_info_vector_.clear();
  for (WORD i = 0; i < GAME_PLAYER; ++i) 
  {
    for (it = server_bullet_info_vector_[i].begin(); it != server_bullet_info_vector_[i].end(); ++it) 
	{
      delete (*it);
    }
    server_bullet_info_vector_[i].clear();
  }

  if (g_revenue_score > 0) {
    CString str;
    str.Format(L"吃水%I64d", g_revenue_score);
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

bool TableFrameSink::Initialization(IUnknownEx* unknownex) 
{
  table_frame_ = QUERY_OBJECT_PTR_INTERFACE(unknownex, ITableFrame);
  if (!table_frame_) return false;

  table_frame_->SetStartMode(START_MODE_TIME_CONTROL);
  game_service_attrib_ = table_frame_->GetGameServiceAttrib();
  game_service_option_ = table_frame_->GetGameServiceOption();
  base_score_ = max(1, game_service_option_->lCellScore);

  if (!LoadConfig()) 
  {
    AfxMessageBox(TEXT("配置资源解析失败，请检查"));
    return false;
  }

  current_fish_lk_multiple_ = fish_multiple_[FISH_KIND_21];

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

bool TableFrameSink::OnEventGameConclude(WORD chair_id, IServerUserItem* server_user_item, BYTE reason) 
{
  if (reason == GER_DISMISS) 
  {
    for (WORD i = 0; i < GAME_PLAYER; ++i) 
	{
      IServerUserItem* user_item = table_frame_->GetTableUserItem(i);
      if (user_item == NULL) continue;
      CalcScore(user_item);
    }

    table_frame_->ConcludeGame(GAME_STATUS_FREE,NULL,608);
    KillAllGameTimer();
    table_frame_->KillGameTimer(kSwitchSceneTimer);
    table_frame_->KillGameTimer(kClearTraceTimer);
    ClearFishTrace(true);
    next_scene_kind_ = SCENE_KIND_1;
    special_scene_ = false;
    android_chairid_ = INVALID_CHAIR;
  } 
  else if (chair_id < GAME_PLAYER && server_user_item != NULL) 
  {
    CalcScore(server_user_item);
  }
  return true;
}

bool TableFrameSink::OnEventSendGameScene(WORD chair_id, IServerUserItem* server_user_item, BYTE game_status, bool send_secret) {
  switch (game_status) {
    case GAME_STATUS_FREE:
    case GAME_STATUS_PLAY:
      SendGameConfig(server_user_item);

      CMD_S_GameStatus gamestatus;
      gamestatus.game_version = GAME_VERSION;

	 //由于数据类型问题导致的bug，一个 LONGLONG,一个为DOUBLE
	  LONGLONG temp_fish_score_[GAME_PLAYER];
	  LONGLONG temp_exchange_fish_score[GAME_PLAYER];
	  for(int i = 0;i < GAME_PLAYER;i++)
	  {
		  temp_fish_score_[i] = (LONGLONG)fish_score_[i];
		  temp_exchange_fish_score[i] = (LONGLONG)exchange_fish_score_[i];
	  }
	   
      memcpy(gamestatus.fish_score, temp_fish_score_, sizeof(gamestatus.fish_score));
      memcpy(gamestatus.exchange_fish_score, temp_exchange_fish_score, sizeof(gamestatus.exchange_fish_score));

	  for(int i = 0;i < GAME_PLAYER;i++)
	  {
		  if(gamestatus.exchange_fish_score[i]>10000000)
		  {
			gamestatus.exchange_fish_score[i] = 0;
		  }

		  if(gamestatus.fish_score[i]>10000000)
		  {
			  gamestatus.fish_score[i] = 0;
		  }
	  }
      table_frame_->SendGameScene(server_user_item, &gamestatus, sizeof(gamestatus));

      table_frame_->SendGameMessage(server_user_item, TEXT("键盘↑↓键加减炮，→←键上下分，空格键或鼠标左键发射子弹，F4关闭声音!"), SMT_CHAT);

      return true;
  }
  return false;
}

bool TableFrameSink::OnTimerMessage(DWORD timer_id, WPARAM bind_param) 
{
  switch (timer_id) 
  {
    case kBuildSmallFishTraceTimer:		//小鱼
      return OnTimerBuildSmallFishTrace(bind_param);
    case kBuildMediumFishTraceTimer:	//中鱼
      return OnTimerBuildMediumFishTrace(bind_param);
    case kBuildFish18TraceTimer:		//黄金鲨鱼
      return OnTimerBuildFish18Trace(bind_param);
    case kBuildFish19TraceTimer:		//金龙
      return OnTimerBuildFish19Trace(bind_param);
    case kBuildFish20TraceTimer:		//金龟
      return OnTimerBuildFish20Trace(bind_param);
    case kBuildFishLKTraceTimer:		//金刀
      return OnTimerBuildFishLKTrace(bind_param);
    case kBuildFishBombTraceTimer:		//生成绿色龟
      return OnTimerBuildFishBombTrace(bind_param);
    case kBuildFishSuperBombTraceTimer:	//生成真摇钱树
      return OnTimerBuildFishSuperBombTrace(bind_param);
    case kBuildFishLockBombTraceTimer:	//生成黑色龟
      return OnTimerBuildFishLockBombTrace(bind_param);
    case kBuildFishSanTraceTimer:		//生成大三元
      return OnTimerBuildFishSanTrace(bind_param);
    case kBuildFishSiTraceTimer:		//生成大四喜
      return OnTimerBuildFishSiTrace(bind_param);
    case kBuildFishKingTraceTimer:		//生成鱼王
      return OnTimerBuildFishKingTrace(bind_param);
    case kClearTraceTimer:
      return OnTimerClearTrace(bind_param);
    case kBulletIonTimer:
      return OnTimerBulletIonTimeout(bind_param);
    case kLockTimer:
      return OnTimerLockTimeout(bind_param);
    case kSwitchSceneTimer:		//切换场景
      return OnTimerSwitchScene(bind_param);
    case kSceneEndTimer:
      return OnTimerSceneEnd(bind_param);
    case kLKScoreTimer:
      return OnTimerLKScore(bind_param);
	case kAndroidShot:
		return OnTimerAndroidShot();
		
    default:
      ASSERT(FALSE);
  }
  return false;
}

bool TableFrameSink::OnTimerAndroidShot()
{
	//这里才是机器人发炮的算法
	for(int i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem * mpIServerUserItem =  table_frame_->GetTableUserItem(i);
		if( exchange_fish_score_[i]>0&&mpIServerUserItem==NULL)
		{
			exchange_fish_score_[i]=0;
		}
		if(mpIServerUserItem!=NULL)
		{
			if(mpIServerUserItem->IsAndroidUser()==true&&!special_scene_)
			{
				table_frame_->SendTableData(i,SUB_S_SHOT,NULL,0);
			}
		}
	}
	table_frame_->SetGameTimer(kAndroidShot,300, 1, 0);
	return true;
}



bool TableFrameSink::OnGameMessage(WORD sub_cmdid, void* data, WORD data_size, IServerUserItem* server_user_item) 
{
  switch (sub_cmdid) 
  {
  case SUB_C_ANDROID_LEAVE:
	  {
		  table_frame_->PerformStandUpAction(server_user_item);
		  return true;
	  }
  case SUB_C_TIME_STAMP:
	  {
		  //校验数据
		  assert(data_size == sizeof(CMD_C_TimeStamp));
		  if (data_size != sizeof(CMD_C_TimeStamp)) return false;
		  CMD_C_TimeStamp rsp;
		  memcpy(&rsp,data,sizeof(rsp));
		  rsp.tServerRecvTime=::GetTickCount();
		  rsp.tServerSendTime=rsp.tServerRecvTime;
		  table_frame_->SendTableData(server_user_item->GetChairID(),SUB_C_TIME_STAMP, &rsp, sizeof(rsp));
		  return true;
	  }
    case SUB_C_EXCHANGE_FISHSCORE: 
	{
      assert(data_size == sizeof(CMD_C_ExchangeFishScore));
      if (data_size != sizeof(CMD_C_ExchangeFishScore)) return false;
      CMD_C_ExchangeFishScore* exchange_fishscore = static_cast<CMD_C_ExchangeFishScore*>(data);
      if (server_user_item->GetUserStatus() == US_LOOKON) return true;
      return OnSubExchangeFishScore(server_user_item, exchange_fishscore->increase);
    }
    case SUB_C_USER_FIRE: 
	{
      assert(data_size == sizeof(CMD_C_UserFire));
      if (data_size != sizeof(CMD_C_UserFire)) return false;
      CMD_C_UserFire* user_fire = static_cast<CMD_C_UserFire*>(data);
      if (server_user_item->GetUserStatus() == US_LOOKON) return true;
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
      return OnSubCatchSweepFish(user_item, catch_sweep->fish_id, catch_sweep->catch_fish_id, catch_sweep->catch_fish_count);
    }
    case SUB_C_HIT_FISH_I: 
	{
      assert(data_size == sizeof(CMD_C_HitFishLK));
      if (data_size != sizeof(CMD_C_HitFishLK)) return false;
      CMD_C_HitFishLK* hit_fish = static_cast<CMD_C_HitFishLK*>(data);
      if (server_user_item->GetUserStatus() == US_LOOKON) return true;
      return OnSubHitFishLK(server_user_item, hit_fish->fish_id);
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
		//校验数据
      assert(data_size == sizeof(CMD_C_AndroidBulletMul));
      if (data_size != sizeof(CMD_C_AndroidBulletMul)) return false;
      CMD_C_AndroidBulletMul* android_bullet_mul = static_cast<CMD_C_AndroidBulletMul*>(data);
	//获取玩家信息
      IServerUserItem* user_item = table_frame_->GetTableUserItem(android_bullet_mul->chair_id);
      if (!user_item->IsAndroidUser()) return true;

	  //获取炮数配置
      ServerBulletInfo* bullet_info = GetBulletInfo(android_bullet_mul->chair_id, android_bullet_mul->bullet_id);

      if (bullet_info == NULL) return true;

	  //没分了，直接起来
      if (fish_score_[android_bullet_mul->chair_id] + bullet_info->bullet_mulriple < android_bullet_mul->bullet_mulriple) 
	  {
        table_frame_->PerformStandUpAction(user_item);
        return true;
      }

      CMD_S_UserFire user_fire;
      user_fire.bullet_kind = bullet_info->bullet_kind;
      user_fire.bullet_id = bullet_info->bullet_id;
      user_fire.angle = 0.f;
      user_fire.chair_id = user_item->GetChairID();
      user_fire.android_chairid = android_chairid_;
      user_fire.bullet_mulriple = bullet_info->bullet_mulriple;
      user_fire.fish_score = bullet_info->bullet_mulriple - android_bullet_mul->bullet_mulriple;
      user_fire.lock_fishid = 0;
      table_frame_->SendTableData(user_item->GetChairID(), SUB_S_USER_FIRE, &user_fire, sizeof(user_fire));

      fish_score_[android_bullet_mul->chair_id] += bullet_info->bullet_mulriple;
      fish_score_[android_bullet_mul->chair_id] -= android_bullet_mul->bullet_mulriple;

      bullet_info->bullet_kind = android_bullet_mul->bullet_kind;
      bullet_info->bullet_mulriple = android_bullet_mul->bullet_mulriple;

      return true;
    }
  }
  return false;
}

bool TableFrameSink::OnFrameMessage(WORD sub_cmdid, void* data, WORD data_size, IServerUserItem* server_user_item) {
  return false;
}

bool TableFrameSink::OnActionUserSitDown(WORD chair_id, IServerUserItem* server_user_item, bool lookon_user) 
{
	if (!lookon_user) 
	{
		exchange_fish_score_[chair_id] = 0;
		fish_score_[chair_id] = 0;

		if (android_chairid_ == INVALID_CHAIR && !server_user_item->IsAndroidUser()) 
		{
			android_chairid_ = chair_id;
		}

		if (table_frame_->GetGameStatus() == GAME_STATUS_FREE) 
		{
			table_frame_->StartGame();
			table_frame_->SetGameStatus(GAME_STATUS_PLAY);
			StartAllGameTimer();
			table_frame_->SetGameTimer(kSwitchSceneTimer, kSwitchSceneElasped * 1000, kRepeatTimer, 0);
			table_frame_->SetGameTimer(kClearTraceTimer, kClearTraceElasped * 1000, kRepeatTimer, 0);
		}
	}
	else
	{
		if(!server_user_item->IsAndroidUser())
			lookUserCount++;
	}

	return true;
}

bool TableFrameSink::OnActionUserOffLine(WORD chair_id, IServerUserItem * server_user_item)
{
	// 捕鱼断线时写分<add by hxh 20161216>
	if (chair_id < GAME_PLAYER && server_user_item != NULL) 
	{
		CalcScore(server_user_item);
	}
	return true;
}

bool TableFrameSink::OnActionUserStandUp(WORD chair_id, IServerUserItem* server_user_item, bool lookon_user) {
  if (lookon_user) 
  {
	  if(!server_user_item->IsAndroidUser())
		  lookUserCount--;
	  return true;
  }

  exchange_fish_score_[chair_id] = 0;
  fish_score_[chair_id] = 0;

  WORD user_count = 0;
  WORD player_count = 0;
  WORD android_chair_id[GAME_PLAYER];
  for (WORD i = 0; i < GAME_PLAYER; ++i) 
  {
    if (i == chair_id) continue;
    IServerUserItem* user_item = table_frame_->GetTableUserItem(i);
    if (user_item) 
	{
      if (!user_item->IsAndroidUser()) android_chair_id[player_count++] = i;
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
    table_frame_->ConcludeGame(GAME_STATUS_FREE,NULL,608);
    KillAllGameTimer();
    table_frame_->KillGameTimer(kSwitchSceneTimer);
    table_frame_->KillGameTimer(kClearTraceTimer);
    ClearFishTrace(true);
    next_scene_kind_ = SCENE_KIND_1;
    special_scene_ = false;
  }

  return true;
}

bool TableFrameSink::OnActionUserOnReady(WORD chair_id, IServerUserItem * server_user_item, void* data, WORD data_size)
{
	return true;
}

FishTraceInfo* TableFrameSink::ActiveFishTrace() 
{
  FishTraceInfo* fish_trace_info = NULL;
  if (storage_fish_trace_vector_.size() > 0) 
  {
    fish_trace_info = storage_fish_trace_vector_.back();
    storage_fish_trace_vector_.pop_back();
    active_fish_trace_vector_.push_back(fish_trace_info);
  }

  if (fish_trace_info == NULL) {
    fish_trace_info = new FishTraceInfo;
    active_fish_trace_vector_.push_back(fish_trace_info);
  }

  return fish_trace_info;
}

bool TableFrameSink::FreeFishTrace(FishTraceInfo* fish_trace_info) 
{
  FishTraceInfoVecor::iterator iter;
  for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) 
  {
    if (fish_trace_info == *iter) 
	{
      active_fish_trace_vector_.erase(iter);
      storage_fish_trace_vector_.push_back(fish_trace_info);
      return true;
    }
  }

  assert(!"FreeFishTrace Failed");
  return false;
}

void TableFrameSink::FreeAllFishTrace() 
{
  std::copy(active_fish_trace_vector_.begin(), active_fish_trace_vector_.end(), std::back_inserter(storage_fish_trace_vector_));
  active_fish_trace_vector_.clear();
}

FishTraceInfo* TableFrameSink::GetFishTraceInfo(int fish_id) 
{
  FishTraceInfoVecor::iterator iter;
  FishTraceInfo* fish_trace_info = NULL;
  for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) 
  {
    fish_trace_info = *iter;
    if (fish_trace_info->fish_id == fish_id) return fish_trace_info;
  }
  //assert(!"GetFishTraceInfo:not found fish");
  return NULL;
}

void TableFrameSink::SaveSweepFish(FishKind fish_kind, int fish_id, BulletKind bullet_kind, int bullet_mulriple) {
  SweepFishInfo sweep_fish;
  sweep_fish.fish_kind = fish_kind;
  sweep_fish.fish_id = fish_id;
  sweep_fish.bullet_kind = bullet_kind;
  sweep_fish.bullet_mulriple = bullet_mulriple;
  sweep_fish_info_vector_.push_back(sweep_fish);
}

bool TableFrameSink::FreeSweepFish(int fish_id) 
{
  std::vector<SweepFishInfo>::iterator iter;
  for (iter = sweep_fish_info_vector_.begin(); iter != sweep_fish_info_vector_.end(); ++iter) 
  {
    if ((*iter).fish_id == fish_id) {
      sweep_fish_info_vector_.erase(iter);
      return true;
    }
  }

  assert(!"FreeSweepFish Failed");
  return false;
}

SweepFishInfo* TableFrameSink::GetSweepFish(int fish_id) 
{
  std::vector<SweepFishInfo>::iterator iter;
  for (iter = sweep_fish_info_vector_.begin(); iter != sweep_fish_info_vector_.end(); ++iter) 
  {
    if ((*iter).fish_id == fish_id) 
	{
      return &(*iter);
    }
  }
  return NULL;
}

ServerBulletInfo* TableFrameSink::ActiveBulletInfo(WORD chairid) 
{
  ServerBulletInfo* bullet_info = NULL;
  if (storage_bullet_info_vector_.size() > 0) 
  {
    bullet_info = storage_bullet_info_vector_.back();
    storage_bullet_info_vector_.pop_back();
    server_bullet_info_vector_[chairid].push_back(bullet_info);
  }

  if (bullet_info == NULL) 
  {
    bullet_info = new ServerBulletInfo();
    server_bullet_info_vector_[chairid].push_back(bullet_info);
  }

  return bullet_info;
}

bool TableFrameSink::FreeBulletInfo(WORD chairid, ServerBulletInfo* bullet_info) 
{
  ServerBulletInfoVector::iterator iter;
  for (iter = server_bullet_info_vector_[chairid].begin(); iter != server_bullet_info_vector_[chairid].end(); ++iter) 
  {
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
  //assert(!"GetBulletInfo:not found");
  return NULL;
}

void TableFrameSink::StartAllGameTimer() 
{
	table_frame_->SetGameTimer(kBulletIonTimer,1000, 1, 0);
	table_frame_->SetGameTimer(kAndroidShot,300, 1, 0);
	

	table_frame_->SetGameTimer(kBuildSmallFishTraceTimer, (CreateFishKind_Time[CREATE_KIND_0]==0 ? kBuildSmallFishTraceElasped : CreateFishKind_Time[CREATE_KIND_0]) * 1000, kRepeatTimer, 0);				//生成小鱼
	table_frame_->SetGameTimer(kBuildMediumFishTraceTimer,(CreateFishKind_Time[CREATE_KIND_1]==0 ? kBuildMediumFishTraceElasped : CreateFishKind_Time[CREATE_KIND_1]) * 1000, kRepeatTimer, 0);				//生成中鱼的定时器
	table_frame_->SetGameTimer(kBuildFish18TraceTimer, (CreateFishKind_Time[CREATE_KIND_2]==0 ? kBuildFish18TraceElasped : CreateFishKind_Time[CREATE_KIND_2]) * 1000, kRepeatTimer, 0);					//生成黄金鲨鱼的定时器
	table_frame_->SetGameTimer(kBuildFish19TraceTimer,(CreateFishKind_Time[CREATE_KIND_3]==0 ? kBuildFish19TraceElasped : CreateFishKind_Time[CREATE_KIND_3]) * 1000, kRepeatTimer, 0);					//生成金龙的定时器
	table_frame_->SetGameTimer(kBuildFish20TraceTimer, (CreateFishKind_Time[CREATE_KIND_4]==0 ? kBuildFish20TraceElasped : CreateFishKind_Time[CREATE_KIND_4]) * 1000, kRepeatTimer, 0);					//生成金龟的定时器
	table_frame_->SetGameTimer(kBuildFishLKTraceTimer, (CreateFishKind_Time[CREATE_KIND_5]==0 ? kBuildFishLKTraceElasped : CreateFishKind_Time[CREATE_KIND_5]) * 1000, kRepeatTimer, 0);					//生成金刀的定时器
	table_frame_->SetGameTimer(kBuildFishLockBombTraceTimer, (CreateFishKind_Time[CREATE_KIND_6]==0 ? kBuildFishSuperBombTraceElasped : CreateFishKind_Time[CREATE_KIND_6]) * 1000, kRepeatTimer, 0);		//生成黑色龟
	table_frame_->SetGameTimer(kBuildFishBombTraceTimer, (CreateFishKind_Time[CREATE_KIND_7]==0 ? kBuildFishBombTraceElasped : CreateFishKind_Time[CREATE_KIND_7]) * 1000, kRepeatTimer, 0);				//生成绿色龟
	table_frame_->SetGameTimer(kBuildFishSuperBombTraceTimer, (CreateFishKind_Time[CREATE_KIND_8]==0 ? kBuildFishSanTraceElasped : CreateFishKind_Time[CREATE_KIND_8]) * 1000, kRepeatTimer, 0);			//生成真摇钱树
	table_frame_->SetGameTimer(kBuildFishSanTraceTimer, (CreateFishKind_Time[CREATE_KIND_9]==0 ? kBuildFishKingTraceElasped : CreateFishKind_Time[CREATE_KIND_9]) * 1000, kRepeatTimer, 0);				//生成大三元
	table_frame_->SetGameTimer(kBuildFishSiTraceTimer, (CreateFishKind_Time[CREATE_KIND_10]==0 ? kSwitchSceneElasped : CreateFishKind_Time[CREATE_KIND_10]) * 1000, kRepeatTimer, 0);						//生成大四喜
	table_frame_->SetGameTimer(kBuildFishKingTraceTimer, (CreateFishKind_Time[CREATE_KIND_11]==0 ? kSceneEndElasped : CreateFishKind_Time[CREATE_KIND_11])  * 1000, kRepeatTimer, 0);						//生成鱼王
}

void TableFrameSink::KillAllGameTimer() 
{
	table_frame_->KillGameTimer(kBuildSmallFishTraceTimer);
	table_frame_->KillGameTimer(kBuildMediumFishTraceTimer);
	table_frame_->KillGameTimer(kBuildFish18TraceTimer);
	table_frame_->KillGameTimer(kBuildFish19TraceTimer);
	table_frame_->KillGameTimer(kBuildFish20TraceTimer);
	table_frame_->KillGameTimer(kBuildFishLKTraceTimer);
	table_frame_->KillGameTimer(kBuildFishBombTraceTimer);
	table_frame_->KillGameTimer(kBuildFishLockBombTraceTimer);
	table_frame_->KillGameTimer(kBuildFishSuperBombTraceTimer);
	table_frame_->KillGameTimer(kBuildFishSanTraceTimer);
	table_frame_->KillGameTimer(kBuildFishSiTraceTimer);
	table_frame_->KillGameTimer(kBuildFishKingTraceTimer);
}

bool TableFrameSink::LoadConfig() 
{
	TCHAR szFilePath[_MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szFilePath), szFilePath);

	TCHAR file_name[MAX_PATH] = TEXT("");
	_snwprintf_s(file_name, MAX_PATH, TEXT("%s\\zyqs_config_%d.xml"), szFilePath, game_service_option_->wServerID);

	TiXmlDocument xml_doc;
	if (!xml_doc.LoadFile(CT2A(file_name), TIXML_ENCODING_UTF8)) return false;

	const TiXmlElement* xml_element = xml_doc.FirstChildElement("Config");
	if (xml_element == NULL) 
	{
		return false;
	}

	const TiXmlElement* xml_child = NULL;
	int fish_count = 0, bullet_kind_count = 0;
	for (xml_child = xml_element->FirstChildElement(); xml_child; xml_child = xml_child->NextSiblingElement()) 
	{
		if (!strcmp(xml_child->Value(), "Stock")) 
		{
			for (const TiXmlElement* xml_stock = xml_child->FirstChildElement(); xml_stock; xml_stock = xml_stock->NextSiblingElement()) 
			{
				xml_stock->Attribute("stockScore", &stock_crucial_score_[stock_crucial_count_]);
				xml_stock->Attribute("increaseProbability", &stock_increase_probability_[stock_crucial_count_]);
				++stock_crucial_count_;
				if (stock_crucial_count_ >= 10) break;
			}
		} 
		else if (!strcmp(xml_child->Value(), "ScoreExchange")) 
		{
			const char* attri = xml_child->Attribute("exchangeRatio");
			char* temp = NULL;
			exchange_ratio_userscore_ = strtol(attri, &temp, 10);
			exchange_ratio_fishscore_ = strtol(temp + 1, &temp, 10);
			xml_child->Attribute("exchangeCount", &exchange_count_);
		} 
		else if (!strcmp(xml_child->Value(), "Cannon")) 
		{
			const char* attri = xml_child->Attribute("cannonMultiple");
			char* temp = NULL;
			min_bullet_multiple_ = strtol(attri, &temp, 10);
			max_bullet_multiple_ = strtol(temp + 1, &temp, 10);
		}
		else if (!strcmp(xml_child->Value(), "Bomb")) 
		{
			const char* attri = xml_child->Attribute("BombProbability");
			char* temp = NULL;
			bomb_stock_ = strtol(attri, &temp, 10);
			super_bomb_stock_ = strtol(temp + 1, &temp, 10);
		}
		else if (!strcmp(xml_child->Value(), "Fish")) 
		{
			int fish_kind;
			xml_child->Attribute("kind", &fish_kind);
			if (fish_kind >= FISH_KIND_COUNT || fish_kind < 0) 
				return false;
			xml_child->Attribute("speed", &fish_speed_[fish_kind]);
			if (fish_kind == FISH_KIND_18) 
			{
				const char* attri = xml_child->Attribute("multiple");
				char* temp = NULL;
				fish_multiple_[fish_kind] = strtol(attri, &temp, 10);
				fish18_max_multiple_ = strtol(temp + 1, &temp, 10);
			}
			else if (fish_kind == FISH_KIND_19) 
			{
				const char* attri = xml_child->Attribute("multiple");
				char* temp = NULL;
				fish_multiple_[fish_kind] = strtol(attri, &temp, 10);
				fish19_max_multiple_ = strtol(temp + 1, &temp, 10);
			}
			else if (fish_kind == FISH_KIND_21) 
			{
				const char* attri = xml_child->Attribute("multiple");
				char* temp = NULL;
				fish_multiple_[fish_kind] = strtol(attri, &temp, 10);
				fishLK_max_multiple_ = strtol(temp + 1, &temp, 10);
			}
			else 
			{
				xml_child->Attribute("multiple", &fish_multiple_[fish_kind]);
			}
			const char* attri = xml_child->Attribute("BoundingBox");
			char* temp = NULL;
			fish_bounding_box_width_[fish_kind] = strtol(attri, &temp, 10);
			fish_bounding_box_height_[fish_kind] = strtol(temp + 1, &temp, 10);
			if (fish_kind == FISH_KIND_23) 
			{
				bomb_range_width_ = strtol(temp + 1, &temp, 10);
				bomb_range_height_ = strtol(temp + 1, &temp, 10);
			}
			xml_child->Attribute("hitRadius", &fish_hit_radius_[fish_kind]);
			xml_child->Attribute("captureProbability", &fish_capture_probability_[fish_kind]);
			++fish_count;
		}
		else if (!strcmp(xml_child->Value(), "Bullet")) 
		{
			int bullet_kind;
			xml_child->Attribute("kind", &bullet_kind);
			if (bullet_kind >= BULLET_KIND_COUNT || bullet_kind < 0) return false;
			xml_child->Attribute("speed", &bullet_speed_[bullet_kind]);
			xml_child->Attribute("netRadius", &net_radius_[bullet_kind]);
			++bullet_kind_count;
		}
		else if (!strcmp(xml_child->Value(), "FishCreate"))
		{
			int iCreat_kind = 0;
			xml_child->Attribute("kind", &iCreat_kind);
			if (iCreat_kind >= CREATE_KIND_COUNT || iCreat_kind < 0)
				return false;
			xml_child->Attribute("time" , (int*)&CreateFishKind_Time[iCreat_kind]);

			const char* attri = xml_child->Attribute("count");
			char* temp = NULL;
			CreateFishKind_Min_Count[iCreat_kind] = strtol(attri, &temp, 10);
			CreateFishKind_Max_Count[iCreat_kind] = strtol(temp + 1, &temp, 10);				
		}
		else if(!strcmp(xml_child->Value(), "IonTimer"))
		{
			xml_child->Attribute("time", &IonTimer_);
			if(IonTimer_<0) return false;
		}
	}
	if (fish_count != FISH_KIND_COUNT) return false;
	if (bullet_kind_count != BULLET_KIND_COUNT) return false;

	for(int i = 0; i< GAME_PLAYER; i++)
	{
		BulletIonTimeCount[i] = IonTimer_ + 1;
	}

	return true;
}

bool TableFrameSink::SendGameConfig(IServerUserItem* server_user_item) {
  CMD_S_GameConfig game_config;
  game_config.exchange_ratio_userscore = exchange_ratio_userscore_;
  game_config.exchange_ratio_fishscore = exchange_ratio_fishscore_;
  game_config.exchange_count = exchange_count_;
  game_config.min_bullet_multiple = min_bullet_multiple_;
  game_config.max_bullet_multiple = max_bullet_multiple_;
  game_config.bomb_range_width = bomb_range_width_;
  game_config.bomb_range_height = bomb_range_height_;
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

//产生小鱼
bool TableFrameSink::OnTimerBuildSmallFishTrace(WPARAM bind_param) 
{
	if (CServerRule::IsForfendGameEnter(game_service_option_->dwServerRule)) 
	{
		OnEventGameConclude(GAME_PLAYER, NULL, GER_DISMISS);
		return true;
	}

	if (CreateFishKind_Max_Count[CREATE_KIND_0]-CreateFishKind_Min_Count[CREATE_KIND_0] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_0] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_0]-CreateFishKind_Min_Count[CREATE_KIND_0]+1), FISH_KIND_1, FISH_KIND_10);
	}
	//BuildFishTrace(4 + rand() % 8, FISH_KIND_1, FISH_KIND_10);

	return true;
}

//产生中等大小的鱼
bool TableFrameSink::OnTimerBuildMediumFishTrace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_1]-CreateFishKind_Min_Count[CREATE_KIND_1] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_1] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_1]-CreateFishKind_Min_Count[CREATE_KIND_1]+1), FISH_KIND_11, FISH_KIND_17);
	}
	//BuildFishTrace(1 + rand() % 2, FISH_KIND_11, FISH_KIND_17);
	
	return true;
}

//生成黄金鲨鱼
bool TableFrameSink::OnTimerBuildFish18Trace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_2]-CreateFishKind_Min_Count[CREATE_KIND_2] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_2] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_2]-CreateFishKind_Min_Count[CREATE_KIND_2]+1), FISH_KIND_18, FISH_KIND_18);
	}
	//BuildFishTrace(1, FISH_KIND_18, FISH_KIND_18);
	
	return true;
}

//生成金龙
bool TableFrameSink::OnTimerBuildFish19Trace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_3]-CreateFishKind_Min_Count[CREATE_KIND_3] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_3] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_3]-CreateFishKind_Min_Count[CREATE_KIND_3]+1), FISH_KIND_19, FISH_KIND_19);
	}
	//BuildFishTrace(1, FISH_KIND_19, FISH_KIND_19);

	return true;
}

//生成金龟
bool TableFrameSink::OnTimerBuildFish20Trace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_4]-CreateFishKind_Min_Count[CREATE_KIND_4] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_4] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_4]-CreateFishKind_Min_Count[CREATE_KIND_4]+1), FISH_KIND_20, FISH_KIND_20);
	}
	//BuildFishTrace(1, FISH_KIND_20, FISH_KIND_20);

	return true;
}

//生成金刀
bool TableFrameSink::OnTimerBuildFishLKTrace(WPARAM bind_param) 
{
	CMD_S_FishTrace fish_trace;
  
	DWORD build_tick = GetTickCount();
	FishTraceInfo* fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_21;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();
#ifdef USE_TRACETIME
    fish_trace.FishMoveSecs=0;
    fish_trace.BuildTraceTime=fish_trace_info->build_tick;
#endif
	fish_trace.fish_id = fish_trace_info->fish_id;
	fish_trace.fish_kind = fish_trace_info->fish_kind;
	fish_trace.init_count = 3;
	fish_trace.trace_type = TRACE_BEZIER;
	BuildInitTrace(fish_trace.init_pos, fish_trace.init_count, fish_trace.fish_kind, fish_trace.trace_type);

	table_frame_->SendTableData(INVALID_CHAIR, SUB_S_FISH_TRACE, &fish_trace, sizeof(fish_trace));
	table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_FISH_TRACE, &fish_trace, sizeof(fish_trace));

	current_fish_lk_multiple_ = fish_multiple_[FISH_KIND_21];
	table_frame_->SetGameTimer(kLKScoreTimer, kLKScoreElasped * 500, fishLK_max_multiple_ - fish_multiple_[FISH_KIND_21], fish_trace_info->fish_id);

	return true;
}

//生成黑色龟
bool TableFrameSink::OnTimerBuildFishLockBombTrace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_6]-CreateFishKind_Min_Count[CREATE_KIND_6] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_6] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_6]-CreateFishKind_Min_Count[CREATE_KIND_6]+1), FISH_KIND_22, FISH_KIND_22);
	}
	//BuildFishTrace(1, FISH_KIND_22, FISH_KIND_22);

	return true;
}

//生成绿色龟
bool TableFrameSink::OnTimerBuildFishBombTrace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_7]-CreateFishKind_Min_Count[CREATE_KIND_7] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_7] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_7]-CreateFishKind_Min_Count[CREATE_KIND_7]+1), FISH_KIND_23, FISH_KIND_23);
	}
	//BuildFishTrace(1, FISH_KIND_23, FISH_KIND_23);
	
	return true;
}

//生成真摇钱树
bool TableFrameSink::OnTimerBuildFishSuperBombTrace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_8]-CreateFishKind_Min_Count[CREATE_KIND_8] < 0 )
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_8] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_8]-CreateFishKind_Min_Count[CREATE_KIND_8]+1), FISH_KIND_24, FISH_KIND_24);		
	}
	//BuildFishTrace(1, FISH_KIND_24, FISH_KIND_24);

	return true;
}

//产生大三元
bool TableFrameSink::OnTimerBuildFishSanTrace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_9]-CreateFishKind_Min_Count[CREATE_KIND_9] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_9] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_9]-CreateFishKind_Min_Count[CREATE_KIND_9]+1), FISH_KIND_25, FISH_KIND_27);
	}
	//BuildFishTrace(1, FISH_KIND_25, FISH_KIND_27);
	
	return true;
}

//产生大四喜
bool TableFrameSink::OnTimerBuildFishSiTrace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_10]-CreateFishKind_Min_Count[CREATE_KIND_10] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_10] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_10]-CreateFishKind_Min_Count[CREATE_KIND_10]+1), FISH_KIND_28, FISH_KIND_30);
	}
	//BuildFishTrace(1, FISH_KIND_28, FISH_KIND_30);
	
	return true;
}

//产生鱼王
bool TableFrameSink::OnTimerBuildFishKingTrace(WPARAM bind_param) 
{
	if (CreateFishKind_Max_Count[CREATE_KIND_11]-CreateFishKind_Min_Count[CREATE_KIND_11] < 0)
	{
		return true;
	}
	else
	{
		BuildFishTrace(CreateFishKind_Min_Count[CREATE_KIND_11] + rand() % (CreateFishKind_Max_Count[CREATE_KIND_11]-CreateFishKind_Min_Count[CREATE_KIND_11]+1), FISH_KIND_31, FISH_KIND_40);
	}
	//BuildFishTrace(1, FISH_KIND_31, FISH_KIND_40);
	
	return true;
}

bool TableFrameSink::OnTimerClearTrace(WPARAM bind_param) {
  ClearFishTrace();
  return true;
}

//离子炮结束
bool TableFrameSink::OnTimerBulletIonTimeout(WPARAM bind_param) {
	for(WORD i=0; i<GAME_PLAYER; i++)
	{
		if(BulletIonTimeCount[i]<IonTimer_)
		{
			BulletIonTimeCount[i]++;
			if(BulletIonTimeCount[i]==IonTimer_)
			{
				CMD_S_BulletIonTimeout bullet_timeout;
				bullet_timeout.chair_id = i;
				table_frame_->SendTableData(INVALID_CHAIR, SUB_S_BULLET_ION_TIMEOUT, &bullet_timeout, sizeof(bullet_timeout));
				table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_BULLET_ION_TIMEOUT, &bullet_timeout, sizeof(bullet_timeout));
			}
		}
	}
  table_frame_->SetGameTimer(kBulletIonTimer,1000, 1, 0);

  return true;
}

bool TableFrameSink::OnTimerLockTimeout(WPARAM bind_param) {
  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_LOCK_TIMEOUT);
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_LOCK_TIMEOUT);
  StartAllGameTimer();

  return true;
}

bool TableFrameSink::OnTimerSwitchScene(WPARAM bind_param) 
{

  KillAllGameTimer();
  ClearFishTrace(true);
  special_scene_ = true;
  table_frame_->SetGameTimer(kSceneEndTimer, kSceneEndElasped * 1000, 1, 0);

  if (next_scene_kind_ == SCENE_KIND_1) 
  {
    BuildSceneKind1();
  }
  else if (next_scene_kind_ == SCENE_KIND_2) 
  {
    BuildSceneKind2();
  }
  else if (next_scene_kind_ == SCENE_KIND_3) 
  {
    BuildSceneKind3();
  }
  else if (next_scene_kind_ == SCENE_KIND_4) 
  {
    BuildSceneKind4();
  }
  else if (next_scene_kind_ == SCENE_KIND_5) 
  {
    BuildSceneKind5();
  }

  next_scene_kind_ = static_cast<SceneKind>((next_scene_kind_ + 1) % SCENE_KIND_COUNT);

  return true;
}

bool TableFrameSink::OnTimerSceneEnd(WPARAM bind_param) {
  special_scene_ = false;
  StartAllGameTimer();
  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SCENE_END);
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SCENE_END);
  return true;
}

bool TableFrameSink::OnTimerLKScore(WPARAM bind_param) 
{
  if (current_fish_lk_multiple_ >= fishLK_max_multiple_) 
  {
    table_frame_->KillGameTimer(kLKScoreTimer);
    return true;
  }
  CMD_S_HitFishLK hit_fish;
  hit_fish.chair_id = 3;
  hit_fish.fish_id = (int)bind_param;
  hit_fish.fish_mulriple = ++current_fish_lk_multiple_;
  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_HIT_FISH_LK, &hit_fish, sizeof(hit_fish));
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_HIT_FISH_LK, &hit_fish, sizeof(hit_fish));
  if (current_fish_lk_multiple_ == fishLK_max_multiple_) 
  {
    table_frame_->KillGameTimer(kLKScoreTimer);
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

bool TableFrameSink::OnSubExchangeFishScore(IServerUserItem* server_user_item, int increase) 
{
  WORD chair_id = server_user_item->GetChairID();

  CMD_S_ExchangeFishScore exchange_fish_score;
  exchange_fish_score.chair_id = chair_id;

  SCORE need_user_score = exchange_ratio_userscore_ * exchange_count_ / exchange_ratio_fishscore_;
  SCORE user_leave_score = server_user_item->GetUserScore() - exchange_fish_score_[chair_id] * exchange_ratio_userscore_ / exchange_ratio_fishscore_;

  if (increase == 1) 
  {
	//原本为防止数据包不经过客户端而发送给服务器，使得玩家没有那个多分而能上那么多分
 //   if (need_user_score > user_leave_score) 
	//{
 //     if (server_user_item->IsAndroidUser()) 
	//  {
 //       table_frame_->PerformStandUpAction(server_user_item);
 //       return true;
 //     }
	//  else 
	//  {
 //       return false;
 //     }
 //   }

    //fish_score_[chair_id] += exchange_count_;
    //exchange_fish_score_[chair_id] += exchange_count_;
    //exchange_fish_score.swap_fish_score = exchange_count_;

	  if( need_user_score > user_leave_score )
	  {
		  need_user_score = user_leave_score;
	  }
	  need_user_score = need_user_score * exchange_ratio_fishscore_/exchange_ratio_userscore_;


	  fish_score_[chair_id] += need_user_score; 
	  exchange_fish_score_[chair_id] += need_user_score; 
	  exchange_fish_score.swap_fish_score = need_user_score; 


	  //没分了，直接起来
      if (fish_score_[chair_id] < min_bullet_multiple_) 
	  {
        table_frame_->PerformStandUpAction(server_user_item);
        return true;
      }

	  //m_GameLogic.AddScore(chair_id, need_user_score, true);
	  //m_GameLogic.AddlScore(chair_id, -need_user_score*exchange_ratio_userscore_/exchange_ratio_fishscore_);

	  
  }
  else if (increase==2)
  {
	  if (need_user_score > user_leave_score) 
	  {
		  if (server_user_item->IsAndroidUser()) 
		  {
			  table_frame_->PerformStandUpAction(server_user_item);
			  return true;
		  } else {
			  return true;
		  }
	  }
	  need_user_score = user_leave_score *exchange_ratio_fishscore_/exchange_ratio_userscore_;		//一次性上完分
	  fish_score_[chair_id] += need_user_score; 
	  exchange_fish_score_[chair_id] += need_user_score; 
	  exchange_fish_score.swap_fish_score = need_user_score; 
	  //m_GameLogic.AddScore(chair_id, need_user_score, true);
	  //m_GameLogic.AddlScore(chair_id, -need_user_score*exchange_ratio_userscore_/exchange_ratio_fishscore_);
  }
  else 
  {
	  if (fish_score_[chair_id] <= 0)
		  return true;

	  exchange_fish_score_[chair_id] -= fish_score_[chair_id];
	  exchange_fish_score.swap_fish_score = -fish_score_[chair_id];
	  fish_score_[chair_id] = 0;

	  //__int64 nowScore = m_GameLogic.GetNowScore(chair_id);
	  //if( nowScore > 0 )
	  //{
		 // m_GameLogic.AddlScore(chair_id, nowScore*exchange_ratio_userscore_/exchange_ratio_fishscore_);
		 // m_GameLogic.AccountChair(server_user_item->GetUserID(), chair_id);
		 // m_GameLogic.SetlScore(chair_id, server_user_item->GetUserScore());
	  //}
  }

  exchange_fish_score.exchange_fish_score = exchange_fish_score_[chair_id];
  SendTableData(SUB_S_EXCHANGE_FISHSCORE, &exchange_fish_score, sizeof(exchange_fish_score), server_user_item->IsAndroidUser() ? NULL : server_user_item);

  return true;
}

bool TableFrameSink::OnSubUserFire(IServerUserItem * server_user_item, BulletKind bullet_kind, float angle, int bullet_mul, int lock_fishid) 
{
	// 没真实玩家机器人不打炮   2015.12.8 PS:玩家观战会发现没人打炮 所以添加了实际玩家观看校验
	if (android_chairid_ == INVALID_CHAIR && lookUserCount==0) return true;


	if (bullet_mul < min_bullet_multiple_ || bullet_mul > max_bullet_multiple_) return false;
	WORD chair_id = server_user_item->GetChairID();
	assert(fish_score_[chair_id] >= bullet_mul);
	if (fish_score_[chair_id] < bullet_mul) 
	{
		if (server_user_item->IsAndroidUser()) 
		{
			table_frame_->PerformStandUpAction(server_user_item);
			return true;
		}
		else 
		{
			return false;
		}
	}

	fish_score_[chair_id] -= bullet_mul;
	if (!server_user_item->IsAndroidUser()) 
	{
		int revenue = game_service_option_->cbRevenueRatio * bullet_mul / 100;
		g_stock_score_ += bullet_mul - revenue;
		g_revenue_score += revenue;
	}
	if (lock_fishid > 0 && GetFishTraceInfo(lock_fishid) == NULL) 
	{
		lock_fishid = 0;
	}

	CMD_S_UserFire user_fire;
	user_fire.bullet_kind = bullet_kind;
	user_fire.bullet_id = GetBulletID(chair_id);
	user_fire.angle = angle;
	user_fire.chair_id = server_user_item->GetChairID();
	user_fire.android_chairid = server_user_item->IsAndroidUser() ? android_chairid_ : INVALID_CHAIR;
	user_fire.bullet_mulriple = bullet_mul;
	user_fire.fish_score = -bullet_mul;
	user_fire.lock_fishid = lock_fishid;
	SendTableData(SUB_S_USER_FIRE, &user_fire, sizeof(user_fire), NULL);
	ServerBulletInfo* bullet_info = ActiveBulletInfo(chair_id);
	bullet_info->bullet_id = user_fire.bullet_id;
	bullet_info->bullet_kind = user_fire.bullet_kind;
	bullet_info->bullet_mulriple = user_fire.bullet_mulriple;
	return true;
}

bool TableFrameSink::OnSubCatchFish(IServerUserItem* server_user_item, int fish_id, BulletKind bullet_kind, int bullet_id, int bullet_mul) 
{
  if (bullet_mul < min_bullet_multiple_ || bullet_mul > max_bullet_multiple_) return true;
  FishTraceInfo* fish_trace_info = GetFishTraceInfo(fish_id);
  if (fish_trace_info == NULL) return true;
  if (fish_trace_info->fish_kind >= FISH_KIND_COUNT) return true;
  ServerBulletInfo* bullet_info = GetBulletInfo(server_user_item->GetChairID(), bullet_id);
  if (bullet_info == NULL) return true;
  ASSERT(bullet_info->bullet_mulriple == bullet_mul && bullet_info->bullet_kind == bullet_kind);
  if (!(bullet_info->bullet_mulriple == bullet_mul && bullet_info->bullet_kind == bullet_kind)) return false;

#ifndef TEST
  if (!server_user_item->IsAndroidUser() && g_stock_score_ < 0) 
	  return true;
#endif

  int fish_multiple = fish_multiple_[fish_trace_info->fish_kind];
  SCORE fish_score = fish_multiple_[fish_trace_info->fish_kind] * bullet_mul;
  if (fish_trace_info->fish_kind == FISH_KIND_18) 
  {
    int fish18_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish18_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
    fish_multiple = fish18_mul;
    fish_score = fish18_mul * bullet_mul;
  } else if (fish_trace_info->fish_kind == FISH_KIND_19) 
  {
    int fish19_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish19_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
    fish_score = fish19_mul * bullet_mul;
    fish_multiple = fish19_mul;
  } else if (fish_trace_info->fish_kind == FISH_KIND_21) 
  {
    fish_score = current_fish_lk_multiple_ * bullet_mul;
    fish_multiple = current_fish_lk_multiple_;
  }
  if (bullet_kind >= BULLET_KIND_1_ION) 
	  fish_score *= 2;

#ifndef TEST
  if (!server_user_item->IsAndroidUser() && g_stock_score_ - fish_score < 0) 
	  return true;

  int change_probability = -1;
  if (!server_user_item->IsAndroidUser()) change_probability = CheckUserFilter(server_user_item->GetUserID());
  double probability = static_cast<double>((rand() % 1000 + 1)) / 1000;
  int stock_crucial_count = stock_crucial_count_;
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
  // 机器人能打中企鹅
 // if (fish_trace_info->fish_kind == FISH_KIND_20 && server_user_item->IsAndroidUser()) fish_probability = 0.02;

  if (change_probability == 0) {
    fish_probability *= 0.2;
  } else if (change_probability == 1) {
    fish_probability *= 1.3;
  }
  if (special_scene_ && (fish_trace_info->fish_kind == FISH_KIND_1 || fish_trace_info->fish_kind == FISH_KIND_2)) fish_probability *= 0.7;

  // 炸弹库根据库存调整概率
  if (!server_user_item->IsAndroidUser() && fish_trace_info->fish_kind == FISH_KIND_23 && g_stock_score_ < bomb_stock_)
    fish_probability = 0;
  if (!server_user_item->IsAndroidUser() && fish_trace_info->fish_kind == FISH_KIND_24 && g_stock_score_ < super_bomb_stock_)
    fish_probability = 0;

  //////////////////////////////////////////////////////新加对3-9号鱼的难度处理
  if (!server_user_item->IsAndroidUser())
  {
  static int nFish=0;	//捕中过,10次经过逻辑一次
  static bool bRunFish=true;	//控制周期内是否捕中
  static double m_dRand[7]={0.2,0.3,0.5,0.6,0.8,0.4,1.0};//几率随机

  

  if (bRunFish==false)
	{
	  if(fish_trace_info->fish_kind == FISH_KIND_3 || fish_trace_info->fish_kind == FISH_KIND_4 || fish_trace_info->fish_kind == FISH_KIND_5 || fish_trace_info->fish_kind == FISH_KIND_6 || fish_trace_info->fish_kind == FISH_KIND_7 || fish_trace_info->fish_kind == FISH_KIND_8 || fish_trace_info->fish_kind == FISH_KIND_9)
	  {
	     nFish++;

	     if(nFish>=10)
	     {
		   nFish=0;
		   bRunFish = true;//10次后走正常逻辑，不在走概率随机
	     }
	     else
	     {  
			 fish_probability *= m_dRand[rand()%7];
	     }
	  }
	}

   if(bRunFish)
   {
	  if(fish_trace_info->fish_kind == FISH_KIND_3 || fish_trace_info->fish_kind == FISH_KIND_4 || fish_trace_info->fish_kind == FISH_KIND_5 || fish_trace_info->fish_kind == FISH_KIND_6 || fish_trace_info->fish_kind == FISH_KIND_7 || fish_trace_info->fish_kind == FISH_KIND_8 || fish_trace_info->fish_kind == FISH_KIND_9)
	  {
		  bRunFish = false;
	   }
	}
  }
  //////////////////////////////////////////////////////

  // 机器人打中几率增加
 // if (server_user_item->IsAndroidUser()) fish_probability *= 1.3;

  while ((--stock_crucial_count) >= 0) 
  {
    if (g_stock_score_ >= stock_crucial_score_[stock_crucial_count]) 
	{
      if (probability > (fish_probability * (stock_increase_probability_[stock_crucial_count] + 1))) 
	  {
        return true;
      } 
	  else
	  {
        break;
      }
    }
  }
  if (fish20_config) AddFish20Config(server_user_item->GetUserID(), fish20_catch_count - 1, fish20_catch_probability);
#endif

  WORD chair_id = server_user_item->GetChairID();

  if (fish_trace_info->fish_kind == FISH_KIND_23 || fish_trace_info->fish_kind == FISH_KIND_24 || (fish_trace_info->fish_kind >= FISH_KIND_31 && fish_trace_info->fish_kind <= FISH_KIND_40)) {
    SaveSweepFish(fish_trace_info->fish_kind, fish_id, bullet_kind, bullet_mul);
    CMD_S_CatchSweepFish catch_sweep_fish;
    catch_sweep_fish.chair_id = chair_id;
    catch_sweep_fish.fish_id = fish_id;
    table_frame_->SendTableData(server_user_item->IsAndroidUser() ? android_chairid_ : chair_id, SUB_S_CATCH_SWEEP_FISH, &catch_sweep_fish, sizeof(catch_sweep_fish));
    table_frame_->SendLookonData(server_user_item->IsAndroidUser() ? android_chairid_ : chair_id, SUB_S_CATCH_SWEEP_FISH, &catch_sweep_fish, sizeof(catch_sweep_fish));
  } else {
    fish_score_[chair_id] += fish_score;
    if (!server_user_item->IsAndroidUser()) g_stock_score_ -= fish_score;

    CMD_S_CatchFish catch_fish;
    catch_fish.bullet_ion = fish_multiple >= 15 && (rand() % 100 < 10);
    catch_fish.chair_id = server_user_item->GetChairID();
    catch_fish.fish_id = fish_id;
    catch_fish.fish_kind = fish_trace_info->fish_kind;
    catch_fish.fish_score = fish_score;
    if (fish_trace_info->fish_kind == FISH_KIND_22) {
      table_frame_->SetGameTimer(kLockTimer, kLockTime * 1000, 1, 0);
      KillAllGameTimer();
    }

    if (catch_fish.bullet_ion) {
		BulletIonTimeCount[chair_id]=0;
    }

    table_frame_->SendTableData(INVALID_CHAIR, SUB_S_CATCH_FISH, &catch_fish, sizeof(catch_fish));
    table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_CATCH_FISH, &catch_fish, sizeof(catch_fish));

    if (fish_trace_info->fish_kind == FISH_KIND_21) {
      current_fish_lk_multiple_ = fish_multiple_[FISH_KIND_21];
    } else if (fish_trace_info->fish_kind == FISH_KIND_20) {
      TCHAR tips_msg[1024] = { 0 };
      _snwprintf_s(tips_msg, CountArray(tips_msg), TEXT("%s %d桌的企鹅王被大侠%s打死了，获得%I64d鱼币奖励!"),
        game_service_option_->szServerName, table_frame_->GetTableID() + 1, server_user_item->GetNickName(), fish_score);
      //table_frame_->SendGlobalMessage(tips_msg, SMT_CHAT | SMT_GLOBAL);
    }
  }

  FreeFishTrace(fish_trace_info);
  FreeBulletInfo(chair_id, bullet_info);

  return true;
}

bool TableFrameSink::OnSubCatchSweepFish(IServerUserItem* server_user_item, int fish_id, int* catch_fish_id, int catch_fish_count) {
  SweepFishInfo* sweep_fish_info = GetSweepFish(fish_id);
  if (sweep_fish_info == NULL) return true;
  assert(sweep_fish_info->fish_kind == FISH_KIND_23 || sweep_fish_info->fish_kind == FISH_KIND_24 || (sweep_fish_info->fish_kind >= FISH_KIND_31 && sweep_fish_info->fish_kind <= FISH_KIND_40));
  if (!(sweep_fish_info->fish_kind == FISH_KIND_23 || sweep_fish_info->fish_kind == FISH_KIND_24 || (sweep_fish_info->fish_kind >= FISH_KIND_31 && sweep_fish_info->fish_kind <= FISH_KIND_40))) return false;

  WORD chair_id = server_user_item->GetChairID();

  SCORE fish_score = fish_multiple_[sweep_fish_info->fish_kind] * sweep_fish_info->bullet_mulriple;
  if (sweep_fish_info->fish_kind == FISH_KIND_18) {
    int fish18_mul = fish_multiple_[sweep_fish_info->fish_kind] + rand() % (fish18_max_multiple_ - fish_multiple_[sweep_fish_info->fish_kind] + 1);
    fish_score = fish18_mul * sweep_fish_info->bullet_mulriple;
  } else if (sweep_fish_info->fish_kind == FISH_KIND_19) {
    int fish19_mul = fish_multiple_[sweep_fish_info->fish_kind] + rand() % (fish19_max_multiple_ - fish_multiple_[sweep_fish_info->fish_kind] + 1);
    fish_score = fish19_mul * sweep_fish_info->bullet_mulriple;
  }


#ifndef TEST
  if (!server_user_item->IsAndroidUser() && g_stock_score_ - fish_score < 0) 
	  return true;
#endif

  FishTraceInfoVecor::iterator iter;
  FishTraceInfo* fish_trace_info = NULL;
  for (int i = 0; i < catch_fish_count; ++i) {
    for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) 
	{
      fish_trace_info = *iter;
      if (fish_trace_info->fish_id == catch_fish_id[i]) 
	  {
        fish_score += fish_multiple_[fish_trace_info->fish_kind] * sweep_fish_info->bullet_mulriple;
        active_fish_trace_vector_.erase(iter);
        storage_fish_trace_vector_.push_back(fish_trace_info);
        break;
      }
    }
  }
  if (sweep_fish_info->bullet_kind >= BULLET_KIND_1_ION) 
	  fish_score *= 2;
  fish_score_[chair_id] += fish_score;
  if (!server_user_item->IsAndroidUser()) 
	  g_stock_score_ -= fish_score;

  CMD_S_CatchSweepFishResult catch_sweep_result;
  memset(&catch_sweep_result, 0, sizeof(catch_sweep_result));
  catch_sweep_result.fish_id = fish_id;
  catch_sweep_result.chair_id = chair_id;
  catch_sweep_result.fish_score = fish_score;
  catch_sweep_result.catch_fish_count = catch_fish_count;
  memcpy(catch_sweep_result.catch_fish_id, catch_fish_id, catch_fish_count * sizeof(int));
  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_CATCH_SWEEP_FISH_RESULT, &catch_sweep_result, sizeof(catch_sweep_result));
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_CATCH_SWEEP_FISH_RESULT, &catch_sweep_result, sizeof(catch_sweep_result));

  FreeSweepFish(fish_id);

  return true;
}

bool TableFrameSink::OnSubHitFishLK(IServerUserItem* server_user_item, int fish_id) {
  FishTraceInfo* fish_trace_info = GetFishTraceInfo(fish_id);
  if (fish_trace_info == NULL) return true;
  if (fish_trace_info->fish_kind != FISH_KIND_21) return true;
  if (current_fish_lk_multiple_ >= fishLK_max_multiple_) return true;

  ++current_fish_lk_multiple_;
  CMD_S_HitFishLK hit_fish;
  hit_fish.chair_id = server_user_item->GetChairID();
  hit_fish.fish_id = fish_id;
  hit_fish.fish_mulriple = current_fish_lk_multiple_;
  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_HIT_FISH_LK, &hit_fish, sizeof(hit_fish));
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_HIT_FISH_LK, &hit_fish, sizeof(hit_fish));

  return true;
}

bool TableFrameSink::OnSubStockOperate(IServerUserItem* server_user_item, unsigned char operate_code) {
  if (!CUserRight::IsGameCheatUser(server_user_item->GetUserRight())) return false;

  CMD_S_StockOperateResult stock_operate_result;
  stock_operate_result.operate_code = operate_code;
  if (operate_code == 0) {
    stock_operate_result.stock_score = g_stock_score_;
  } else if (operate_code == 1) {
    stock_operate_result.stock_score = g_stock_score_ = 0;
  } else if (operate_code == 2) {
    g_stock_score_ += game_service_option_->lCellScore;
    stock_operate_result.stock_score = g_stock_score_;
  } else if (operate_code == 3) {
    stock_operate_result.stock_score = g_revenue_score;
  }
  table_frame_->SendUserItemData(server_user_item, SUB_S_STOCK_OPERATE_RESULT, &stock_operate_result, sizeof(stock_operate_result));

  return true;
}

bool TableFrameSink::OnSubFish20Config(IServerUserItem* server_user_item, DWORD game_id, int catch_count, double catch_probability) {
  if (!CUserRight::IsGameCheatUser(server_user_item->GetUserRight())) return false;

  AddFish20Config(game_id, catch_count, catch_probability);

  return true;
}

bool TableFrameSink::OnSubUserFilter(IServerUserItem* server_user_item, DWORD game_id, unsigned char operate_code) {
  if (!CUserRight::IsGameCheatUser(server_user_item->GetUserRight())) return false;

  AddUserFilter(game_id, operate_code);

  return true;
}

void TableFrameSink::BuildInitTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type) {
  assert(init_count >= 2 && init_count <= 3);
  srand(GetTickCount() + rand() % kResolutionWidth);
  WORD chair_id = rand() % GAME_PLAYER;
  int center_x = kResolutionWidth / 2;
  int center_y = kResolutionHeight / 2;
  int factor = rand() % 2 == 0 ? 1 : -1;
  switch (chair_id) {
    case 0:
    case 1:
    case 2:
      init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x));
      init_pos[0].y = 0.f - static_cast<float>(fish_bounding_box_height_[fish_kind]) * 2;
      init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x));
      init_pos[1].y = static_cast<float>(center_y + (rand() % center_y));
      init_pos[2].x = static_cast<float>(center_x - factor * (rand() % center_x));
      init_pos[2].y = static_cast<float>(kResolutionHeight + fish_bounding_box_height_[fish_kind] * 2);
      break;
    case 3:
      init_pos[0].x = static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
      init_pos[0].y = static_cast<float>(center_y + factor* (rand() % center_y));
      init_pos[1].x = static_cast<float>(center_x - (rand() % center_x));
      init_pos[1].y = static_cast<float>(center_y + factor* (rand() % center_y));
      init_pos[2].x = -static_cast<float>(fish_bounding_box_width_[fish_kind] * 2);
      init_pos[2].y = static_cast<float>(center_y - factor* (rand() % center_y));
      break;
    case 5:
    case 6:
    case 4:
      init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x));
      init_pos[0].y = kResolutionHeight + static_cast<float>(fish_bounding_box_height_[fish_kind] * 2);
      init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x));
      init_pos[1].y = static_cast<float>(center_y - (rand() % center_y));
      init_pos[2].x = static_cast<float>(center_x - factor * (rand() % center_x));
      init_pos[2].y = static_cast<float>(-fish_bounding_box_height_[fish_kind] * 2);
      break;
    case 7:
      init_pos[0].x = static_cast<float>(-fish_bounding_box_width_[fish_kind] * 2);
      init_pos[0].y = static_cast<float>(center_y + factor* (rand() % center_y));
      init_pos[1].x = static_cast<float>(center_x + (rand() % center_x));
      init_pos[1].y = static_cast<float>(center_y + factor* (rand() % center_y));
      init_pos[2].x = static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
      init_pos[2].y = static_cast<float>(center_y - factor* (rand() % center_y));
      break;
  }

  if (trace_type == TRACE_LINEAR && init_count == 2) {
    init_pos[1].x = init_pos[2].x;
    init_pos[1].y = init_pos[2].y;
  }
}

void TableFrameSink::BuildFishTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end) 
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
#ifdef USE_TRACETIME
    fish_trace->FishMoveSecs=0;
    fish_trace->BuildTraceTime=fish_trace_info->build_tick;
#endif
    fish_trace->fish_id = fish_trace_info->fish_id;
    fish_trace->fish_kind = fish_trace_info->fish_kind;
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
    BuildInitTrace(fish_trace->init_pos, fish_trace->init_count, fish_trace->fish_kind, fish_trace->trace_type);

    send_size += sizeof(CMD_S_FishTrace);
    ++fish_trace;
  }

  if (send_size > 0) 
  {
    table_frame_->SendTableData(INVALID_CHAIR, SUB_S_FISH_TRACE, tcp_buffer, send_size);
    table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_FISH_TRACE, tcp_buffer, send_size);
  }
}

void TableFrameSink::BuildSceneKind1() 
{
  BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
  CMD_S_SwitchScene* switch_scene = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
  switch_scene->scene_kind = next_scene_kind_;
  DWORD build_tick = GetTickCount();
  switch_scene->fish_count = 0;

  for (int i = 0; i < 100; ++i) 
  {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_1;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 100;
  for (int i = 0; i < 17; ++i) 
  {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_3;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 17;
  for (int i = 0; i < 17; ++i) 
  {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_5;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 17;
  for (int i = 0; i < 30; ++i) 
  {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_2;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 30;
  for (int i = 0; i < 30; ++i) 
  {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_4;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 30;
  for (int i = 0; i < 15; ++i) 
  {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_6;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 15;

  FishTraceInfo* fish_trace_info = ActiveFishTrace();
  fish_trace_info->fish_kind = FISH_KIND_20;
  fish_trace_info->build_tick = build_tick;
  fish_trace_info->fish_id = GetNewFishID();

  switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
  switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
  switch_scene->fish_count += 1;

  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
}

void TableFrameSink::BuildSceneKind2() {
  BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
  CMD_S_SwitchScene* switch_scene = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
  switch_scene->scene_kind = next_scene_kind_;
  DWORD build_tick = GetTickCount();
  switch_scene->fish_count = 0;
  for (int i = 0; i < 200; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_1;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 200;
  for (int i = 0; i < 14; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = static_cast<FishKind>(FISH_KIND_12 + i % 7);
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 14;

  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
}

void TableFrameSink::BuildSceneKind3() {
  BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
  CMD_S_SwitchScene* switch_scene = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
  switch_scene->scene_kind = next_scene_kind_;
  DWORD build_tick = GetTickCount();
  switch_scene->fish_count = 0;
  for (int i = 0; i < 50; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_1;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 50;

  for (int i = 0; i < 40; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_3;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 40;

  for (int i = 0; i < 30; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_4;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 30;

  FishTraceInfo* fish_trace_info = ActiveFishTrace();
  fish_trace_info->fish_kind = FISH_KIND_16;
  fish_trace_info->build_tick = build_tick;
  fish_trace_info->fish_id = GetNewFishID();

  switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
  switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
  switch_scene->fish_count += 1;

  for (int i = 0; i < 50; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_1;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 50;

  for (int i = 0; i < 40; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_2;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 40;

  for (int i = 0; i < 30; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_5;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 30;

  fish_trace_info = ActiveFishTrace();
  fish_trace_info->fish_kind = FISH_KIND_17;
  fish_trace_info->build_tick = build_tick;
  fish_trace_info->fish_id = GetNewFishID();

  switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
  switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
  switch_scene->fish_count += 1;

  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
}

void TableFrameSink::BuildSceneKind4() {
  BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
  CMD_S_SwitchScene* switch_scene = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
  switch_scene->scene_kind = next_scene_kind_;
  DWORD build_tick = GetTickCount();
  switch_scene->fish_count = 0;
  for (int i = 0; i < 8; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_11;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 8;
  for (int i = 0; i < 8; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_12;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 8;
  for (int i = 0; i < 8; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_13;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 8;
  for (int i = 0; i < 8; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_14;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 8;
  for (int i = 0; i < 8; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_15;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 8;
  for (int i = 0; i < 8; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_16;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 8;
  for (int i = 0; i < 8; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_17;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 8;
  for (int i = 0; i < 8; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_18;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 8;

  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
}

void TableFrameSink::BuildSceneKind5() {
  BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
  CMD_S_SwitchScene* switch_scene = reinterpret_cast<CMD_S_SwitchScene*>(tcp_buffer);
  switch_scene->scene_kind = next_scene_kind_;
  DWORD build_tick = GetTickCount();
  switch_scene->fish_count = 0;
  for (int i = 0; i < 40; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_1;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 40;
  for (int i = 0; i < 40; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_2;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 40;
  for (int i = 0; i < 40; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_5;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 40;
  for (int i = 0; i < 40; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_3;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 40;
  for (int i = 0; i < 24; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_4;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 24;
  for (int i = 0; i < 24; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_6;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 24;
  for (int i = 0; i < 13; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_7;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 13;
  for (int i = 0; i < 13; ++i) {
    FishTraceInfo* fish_trace_info = ActiveFishTrace();
    fish_trace_info->fish_kind = FISH_KIND_6;
    fish_trace_info->build_tick = build_tick;
    fish_trace_info->fish_id = GetNewFishID();

    switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
    switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
  }
  switch_scene->fish_count += 13;

  FishTraceInfo* fish_trace_info = ActiveFishTrace();
  fish_trace_info->fish_kind = FISH_KIND_18;
  fish_trace_info->build_tick = build_tick;
  fish_trace_info->fish_id = GetNewFishID();
  switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
  switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
  switch_scene->fish_count += 1;

  fish_trace_info = ActiveFishTrace();
  fish_trace_info->fish_kind = FISH_KIND_17;
  fish_trace_info->build_tick = build_tick;
  fish_trace_info->fish_id = GetNewFishID();
  switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
  switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
  switch_scene->fish_count += 1;

  table_frame_->SendTableData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
  table_frame_->SendLookonData(INVALID_CHAIR, SUB_S_SWITCH_SCENE, tcp_buffer, sizeof(CMD_S_SwitchScene));
}

void TableFrameSink::ClearFishTrace(bool force) 
{
  if (force) 
  {
    std::copy(active_fish_trace_vector_.begin(), active_fish_trace_vector_.end(), std::back_inserter(storage_fish_trace_vector_));
    active_fish_trace_vector_.clear();
  } 
  else 
  {
    FishTraceInfoVecor::iterator iter;
    FishTraceInfo* fish_trace_info = NULL;
    DWORD now_tick = GetTickCount();
    for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end();) 
	{
      fish_trace_info = *iter;
      if (now_tick >= (fish_trace_info->build_tick + kFishAliveTime)) 
	  {
        iter = active_fish_trace_vector_.erase(iter);
        storage_fish_trace_vector_.push_back(fish_trace_info);
      } 
	  else 
	  {
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
  score_info.cbType = SCORE_TYPE_WIN;
  score_info.lScore = (fish_score_[chair_id] - exchange_fish_score_[chair_id]) * exchange_ratio_userscore_ / exchange_ratio_fishscore_;
  if ( !server_user_item->IsAndroidUser() )
	  table_frame_->WriteUserScore(chair_id, score_info);

  fish_score_[chair_id] = 0;
  exchange_fish_score_[chair_id] = 0;
}