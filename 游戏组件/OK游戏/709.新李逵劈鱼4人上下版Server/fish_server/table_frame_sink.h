
#ifndef TABLE_FRAME_SINK_H_
#define TABLE_FRAME_SINK_H_
#pragma once

#include <vector>
#include "GameLogic.h"

#define  USER_COUNT  20

struct FishTraceInfo {
  FishKind fish_kind;
  int fish_id;
  DWORD build_tick;
  int fish_mul;
  int hit_fish_score;
  int ticket_fish_kind;
};

struct SweepFishInfo {
  FishKind fish_kind;
  int fish_id;
  BulletKind bullet_kind;
  int bullet_mulriple;
  DWORD dwUserID;
};

struct ServerBulletInfo {
  BulletKind bullet_kind;
  int bullet_id;
  int bullet_mulriple;
  bool bullet_catch;
};

//大鱼出场时间
struct BuildFish
{
	DWORD kBuildFish;
	bool bFish;
};

const DWORD kBuildFish18TraceCount=7;							//黄金鲨鱼
const DWORD kBuildFish19TraceCount=8;							//龙
const DWORD kBuildFish20TraceCount=5;							//企鹅
const DWORD kBuildFishLKTraceCount=15;							//李逵
const DWORD kBuildFishBombTraceCount=8;						//水浒传
const DWORD kBuildFishSuperBombTraceCount=3;					//超级炸弹
const DWORD kBuildFishLockBombTraceCount=8;						//忠义堂
const DWORD kBuildFishTicketTraceCount = 10;					//奖券

enum 
{
	TPYE_ZIPPO = -4,
	TPYE_WHITE,
	TPYE_MP3,
	TPYE_IPAD
};

class TableFrameSink : public ITableFrameSink, public ITableUserAction 
{
 public:
  TableFrameSink();
  virtual ~TableFrameSink();

  virtual void Release() { delete this; }
  virtual void* QueryInterface(REFGUID guid, DWORD query_ver);

  virtual bool Initialization(IUnknownEx* unknownex);
  virtual void RepositionSink();

  virtual SCORE QueryConsumeQuota(IServerUserItem* server_user_item) { return 0; }
  virtual SCORE QueryLessEnterScore(WORD chair_id, IServerUserItem* server_user_item) { return 0; }
  virtual bool QueryBuckleServiceCharge(WORD chair_id) { return true; }
  virtual void SetGameBaseScore(LONGLONG base_score) {}
  virtual bool OnDataBaseMessage(WORD request_id, void* data, WORD data_size) { return false; }
  virtual bool OnUserScroeNotify(WORD chair_id, IServerUserItem* server_user_item, BYTE reason) { return false; }

  virtual bool IsUserPlaying(WORD chair_id);

  virtual bool OnEventGameStart();
  virtual bool OnEventGameConclude(WORD chair_id, IServerUserItem* server_user_item, BYTE reason);
  virtual bool OnEventSendGameScene(WORD chair_id, IServerUserItem* server_user_item, BYTE game_status, bool send_secret);

  virtual bool OnTimerMessage(DWORD timer_id, WPARAM bind_param);
  virtual bool OnGameMessage(WORD sub_cmdid, void* data, WORD data_size, IServerUserItem* server_user_item);
  virtual bool OnFrameMessage(WORD sub_cmdid, void* data, WORD data_size, IServerUserItem* server_user_item);

  virtual bool OnActionUserSitDown(WORD chair_id, IServerUserItem* server_user_item, bool lookon_user);
  virtual bool OnActionUserStandUp(WORD chair_id, IServerUserItem* server_user_item, bool lookon_user);
  virtual bool OnActionUserOnReady(WORD chair_id, IServerUserItem * server_user_item, void* data, WORD data_size) { return true; }

  virtual bool OnActionUserOffLine(WORD chair_id, IServerUserItem * server_user_item) { return true; }
  //用户重入
  virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }

public:
	inline int GetID()const{return m_id;}
	float getTime()const{return (GetTickCount()-m_tableStartTime)/1000.0f;}
	int GetMaxTable()const{return game_service_option_->wTableCount;}
	void GetExchangeRatio(int & nUserRatio, int & nFishRatio );
	int GetFreshPlayer(){return fresh_player;}
	bool IsLostPlayer(WORD wChairID);
	void GetOnLineTime(WORD wChairID, int nAllStep, int nCurStep, int nTotalTime);
	void OnUpdateUserCoin(DWORD dwUserID, WORD wChairID, int nStep);
 private:
  FishTraceInfo* ActiveFishTrace();
  bool FreeFishTrace(FishTraceInfo* fish_trace_info);
  void FreeAllFishTrace();
  FishTraceInfo* GetFishTraceInfo(int fish_id);

  void SaveSweepFish(FishKind fish_kind, int fish_id, BulletKind bullet_kind, int bullet_mulriple, DWORD dwUserID);
  bool FreeSweepFish(int fish_id);
  bool FreeSweepFish( SweepFishInfo * sweepFishInfo);
  SweepFishInfo* GetSweepFish(int fish_id, DWORD dwUserID);

  ServerBulletInfo* ActiveBulletInfo(WORD chairid);
  bool FreeBulletInfo(WORD chairid, ServerBulletInfo* bullet_info);
  void FreeAllBulletInfo(WORD chairid);
  ServerBulletInfo* GetBulletInfo(WORD chairid, int bullet_id);

 private:
  void StartAllGameTimer();
  void KillAllGameTimer();
  bool LoadConfig(bool bRead);
  bool SendGameConfig(IServerUserItem* server_user_item);
  int GetNewFishID();
  int GetBulletID(WORD chairid);

  bool OnTimerBuildSmallFishTrace(WPARAM bind_param);
  bool OnTimerBuildMediumFishTrace(WPARAM bind_param);
  bool OnTimerBuildFish18Trace(WPARAM bind_param);
  bool OnTimerBuildFish19Trace(WPARAM bind_param);
  bool OnTimerBuildFish20Trace(WPARAM bind_param);
  bool OnTimerBuildFishLKTrace(WPARAM bind_param);
  bool OnTimerBuildFishBombTrace(WPARAM bind_param);
  bool OnTimerBuildFishSuperBombTrace(WPARAM bind_param);
  bool OnTimerBuildFishLockBombTrace(WPARAM bind_param);
  bool OnTimerBuildFishTicketTrace(WPARAM bind_param);
  bool OnTimerBuildFishSanTrace(WPARAM bind_param);
  bool OnTimerBuildFishSiTrace(WPARAM bind_param);
  bool OnTimerBuildFishKingTrace(WPARAM bind_param);
  bool OnTimerClearTrace(WPARAM bind_param);
  bool OnTimerBulletIonTimeout(WPARAM bind_param);
  bool OnTimerLockTimeout(WPARAM bind_param);
  bool OnTimerSwitchScene(WPARAM bind_param);
  bool OnTimerSceneEnd(WPARAM bind_param);
  bool OnTimerLKScore(WPARAM bind_param);

  bool SendTableData(WORD sub_cmdid, void* data, WORD data_size, IServerUserItem* exclude_user_item);
  bool OnSubExchangeFishScore(IServerUserItem* server_user_item, int increase, __int64 iExchangeCount=0);
  bool OnSubUserFire(IServerUserItem* server_user_item, BulletKind bullet_kind, float angle, int bullet_mul, int lock_fishid);
  bool OnSubCatchFish(IServerUserItem* server_user_item, int fish_id, BulletKind bullet_kind, int bullet_id, int bullet_mul);
  bool OnSubCatchSweepFish(IServerUserItem* server_user_item, int fish_id, int* catch_fish_id, int catch_fish_count,int bulletmul);
  bool OnSubHitFishLK(IServerUserItem* server_user_item, int fish_id,int bulletmul);
  bool OnSubStockOperate(IServerUserItem* server_user_item, unsigned char operate_code);
  bool OnSubUserFilter(IServerUserItem* server_user_item, DWORD game_id, unsigned char operate_code);
  bool OnSubFish20Config(IServerUserItem* server_user_item, DWORD game_id, int catch_count, double catch_probability);
  bool OnSubMobileGetHarvest(IServerUserItem* server_user_item);

  void BuildInitTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type);
  void BuildFishTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end);
  void BuildSceneKind1();
  void BuildSceneKind2();
  void BuildSceneKind3();
  void BuildSceneKind4();
  void BuildSceneKind5();
  void ClearFishTrace(bool force = false);

  void CalcScore(IServerUserItem* server_user_item);

  bool OnTimerFishMsg(WPARAM bind_param);


  private:
  ITableFrame* table_frame_;
  tagGameServiceOption* game_service_option_;
  tagGameServiceAttrib* game_service_attrib_;
  SCORE base_score_;

  // 玩家
  int                             m_nLoserCount;
  DWORD                           m_dwLoserGameID[USER_COUNT];
  int                             m_nLoserPercent[USER_COUNT];

  // 上次玩家子弹碰到鱼Tick
  DWORD							m_dwLastCatchFishTick[GAME_PLAYER];
  DWORD							m_dwLastSweepFishTick[GAME_PLAYER];

  // 调整值
  int stock_crucial_score_[15];
  double stock_increase_probability_[15];
  int stock_crucial_count_;

  // 奖券鱼
  int ticket_create_probability[7];
  double ticket_capture_probability[7];

  int exchange_ratio_userscore_;
  int exchange_ratio_fishscore_;
  int exchange_count_;

  int fresh_player;
  int minElapse;
  int maxElapse;
  int decreasePermillage;	//递减千分比
  
  int nfresh_time_[GAME_PLAYER];

  double fish_decreasemul;
  int percentlevel[7];

  int min_bullet_multiple_;
  int max_bullet_multiple_;
  int bomb_stock_;
  int super_bomb_stock_;
  int ticketCreateTime;
  int fish_speed_[FISH_KIND_COUNT];
  int fish_multiple_[FISH_KIND_COUNT];
  int fish18_max_multiple_;
  int fish19_max_multiple_;
  int fishLK_max_multiple_;
  int fish_bounding_box_width_[FISH_KIND_COUNT];
  int fish_bounding_box_height_[FISH_KIND_COUNT];
  int fish_hit_radius_[FISH_KIND_COUNT];
  double fish_capture_probability_[FISH_KIND_COUNT];
  int bomb_range_width_;
  int bomb_range_height_;
  int LockPercent;			//锁定概率
  int LostLockPercent;		//不丢失锁定的概率
  int fishtime_;		//鱼少的时候，机器人打炮速率可调。
  int firetime_;			//可调的时间频率
  int maxBulletNum_;    //同一时刻最多发射子弹数

  int bullet_speed_[BULLET_KIND_COUNT];
  int net_radius_[BULLET_KIND_COUNT];
  int net_bounding_box_width_[BULLET_KIND_COUNT];
  int net_bounding_box_height_[BULLET_KIND_COUNT];

  typedef std::vector<FishTraceInfo*> FishTraceInfoVecor;
  FishTraceInfoVecor active_fish_trace_vector_;
  FishTraceInfoVecor storage_fish_trace_vector_;

  std::vector<SweepFishInfo> sweep_fish_info_vector_;

  typedef std::vector<ServerBulletInfo*> ServerBulletInfoVector;
  ServerBulletInfoVector server_bullet_info_vector_[GAME_PLAYER];
  ServerBulletInfoVector storage_bullet_info_vector_;

  int fish_id_;
  SCORE exchange_fish_score_[GAME_PLAYER];
  SCORE fish_score_[GAME_PLAYER];
  SCORE	user_score_[GAME_PLAYER];
  DWORD	user_lotteries_[GAME_PLAYER];
  DWORD	user_ipdaMini_[GAME_PLAYER];
  DWORD	user_musicPlay3_[GAME_PLAYER];
  DWORD	user_bigWhite_[GAME_PLAYER];
  DWORD	user_zippo_[GAME_PLAYER];
  SCORE user_score_win_[GAME_PLAYER];
  DWORD m_dwPlayTime[GAME_PLAYER];
  DWORD m_dwLastBulletTime[GAME_PLAYER];
  DWORD m_dwLastFireTime[GAME_PLAYER];
  int fire_count_[GAME_PLAYER];

  int bullet_id_[GAME_PLAYER];
  int m_nFishHarvestArray[GAME_PLAYER][FISH_KIND_COUNT];

  // 变化炮数，将只有13%的机会可以打中鱼 （150发子弹只有）
  int  m_nLastBullet_mul[GAME_PLAYER];
  DWORD m_dwLastCatchFishTime[GAME_PLAYER]; // 打中鱼，5秒中内不允许加炮
  int m_nNotCatchFireCount[GAME_PLAYER];  // 0表示不计数 


  SceneKind next_scene_kind_;
  bool special_scene_;

  bool bandrior_fire_;


  int timeindex_;
  int timeindexadd;
  int ticket_score_[7];


  WORD android_chairid_;
  int m_nloadflag;                   // 加载标识

  //大鱼生成概率
  BuildFish BuildFish18[kBuildFish18TraceCount];
  BuildFish BuildFish19[kBuildFish19TraceCount];
  BuildFish BuildFish20[kBuildFish20TraceCount];
  BuildFish BuildFishLK[kBuildFishLKTraceCount];
  BuildFish BuildFishBomb[kBuildFishBombTraceCount];
  BuildFish BuildFishSuperBomb[kBuildFishSuperBombTraceCount];
  BuildFish BuildFishLockBomb[kBuildFishLockBombTraceCount];
  BuildFish BuildFishTicket[kBuildFishTicketTraceCount];

  void ClearFish();
  //输控制区域	zhu by 20140611
  int       m_nLoserRegionPercent;
  char		m_szLoserRigion[MAX_PATH];


  //赢控制区域	
  //int		    m_nWinRegionPercent;
  //int			m_nWinScorePercent;
  int			m_nNation[GAME_PLAYER];//0:普通玩家，1：倒霉玩家(输)，2：幸运玩家(赢)

  //char			m_szWinRigion[MAX_PATH];

 // int         m_nLuckyCount;
  //DWORD       m_dwLuckyGameID[USER_COUNT];
  //int         m_nLuckyPercent[USER_COUNT];

  
protected:
  CGameLogic	m_GameLogic;//游戏逻辑
  int			m_id;
  DWORD			m_tableStartTime;
  HANDLE		m_hMutex;
public:
	BOOL UTF8ToGB(const char* pszSrc,char szNation[MAX_PATH]);
	void SendFishScore();
};

#endif // TABLE_FRAME_SINK_H_
