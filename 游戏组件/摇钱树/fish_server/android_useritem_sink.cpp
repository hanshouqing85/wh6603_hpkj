#include "Stdafx.h"
#include "android_useritem_sink.h"
#include "math.h"
#include "AndroidTimerEngine.h"


//////////////////////////////////////////////////////////////////////////

// 定时器标识
#define IDI_SWITCH_SCENE									1									//切换场景不能打炮
#define IDI_EXCHANGE_FISHSCORE								2									//交换子弹
//#define IDI_FIRE											3									//发炮（弃用
#define IDI_STAND_UP										4									//起立
#define IDI_BULLET_MUL										5									//子弹切换
#define IDI_CHANGE_LOCK										6									//锁鱼切换
#define IDI_LEAVE											7									//离开

// 时间标识
#define TIME_SWITCH_SCENE									7									//切换场景不能打炮 大概7秒钟 客户端有修改 这个应该也要修改
#define TIME_EXCHANGE_FISHSCORE								2
#define TIME_FIRE											2									//这里不能改成1 会造成死循环, 如果想要加快子弹发射速度只能改平台.



//CAndroidTimerEngine g_AndroidTimerEngine;

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
  exchange_ratio_userscore_ = 1;
  exchange_ratio_fishscore_ = 1;
  exchange_count_ = 1;
  exchange_times_ = 1;
  allow_fire_ = false;
  current_bullet_kind_ = BULLET_KIND_2_NORMAL;
  current_bullet_mulriple_ = 10;
  exchange_fish_score_ = 0;
  fish_score_ = 0;
  last_fire_angle_ = 0.f;
  min_bullet_multiple_ = 1;
  max_bullet_multiple_ = 9900;
  addBullet = 0;
  noScoreCount=0;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;
	
	srand((UINT)time(NULL));
	return true;
}


//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	return true;
}



DWORD CAndroidUserItemSink::GetAndroidUserID()
{
	if (m_pIAndroidUserItem!=NULL)
	{
		return m_pIAndroidUserItem->GetUserID();
	}
	return 0;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	
	switch (nTimerID) 
	{
    case IDI_SWITCH_SCENE:
      allow_fire_ = true;
      return true;
    case IDI_EXCHANGE_FISHSCORE:
      ExchangeFishScore();
      return true;
    case IDI_STAND_UP:
      m_pIAndroidUserItem->SendSocketData(SUB_C_ANDROID_STAND_UP);
      return true;
	case IDI_BULLET_MUL:
		return ChangeBulletMul();
	}
	return false;
}

  //上炮 改变炮的倍数
bool CAndroidUserItemSink::ChangeBulletMul()
{
	//只改变状态。加减炮由发炮结果进行
	addBullet = rand()%3-1;

	if(addBullet <0)
		addBullet = (rand()%2==0)?1:addBullet;


	//炮数超过最大炮数一般，提高几率降炮
	if((current_bullet_mulriple_ > max_bullet_multiple_/2)  && (addBullet >0))
		addBullet = (rand()%2==0)?-1:addBullet;
	//下一次改变状态
	m_pIAndroidUserItem->SetGameTimer(IDI_BULLET_MUL, rand()%15);
	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD sub_cmdid, void * data, WORD data_size)
{
  switch (sub_cmdid) {
    case SUB_S_GAME_CONFIG:
      return OnSubGameConfig(data, data_size);
    case SUB_S_EXCHANGE_FISHSCORE:
      return OnSubExchangeFishScore(data, data_size);
    case SUB_S_USER_FIRE:
      return OnSubUserFire(data, data_size);
    case SUB_S_CATCH_FISH:
      return OnSubCatchFish(data, data_size);
    case SUB_S_BULLET_ION_TIMEOUT:
      return OnSubBulletIonTimeout(data, data_size);
    case SUB_S_CATCH_SWEEP_FISH_RESULT:
      return OnSubCatSweepFishResult(data, data_size);
    case SUB_S_SWITCH_SCENE:
      return OnSubSwitchScene(data, data_size);
	case SUB_S_SHOT:
		Fire();//服务器命令开炮
		return true;
  }

	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * data, WORD data_size)
{
  switch (cbGameStatus) {
    case GAME_STATUS_FREE:
    case GAME_STATUS_PLAY:
      ASSERT(data_size == sizeof(CMD_S_GameStatus));
      if (data_size != sizeof(CMD_S_GameStatus)) return false;
      CMD_S_GameStatus* gamestatus = static_cast<CMD_S_GameStatus*>(data);
      if (gamestatus->game_version != GAME_VERSION) return false;

      fish_score_ = 0;
      exchange_fish_score_ = 0;

      srand((DWORD)time(NULL));
      current_bullet_mulriple_ = min_bullet_multiple_;

      if (current_bullet_mulriple_ < 100) {
        current_bullet_kind_ = BULLET_KIND_1_NORMAL;
      } else if (current_bullet_mulriple_ >= 100 && current_bullet_mulriple_ < 1000) {
        current_bullet_kind_ = BULLET_KIND_2_NORMAL;
      } else if (current_bullet_mulriple_ >= 1000 && current_bullet_mulriple_ < 5000) {
        current_bullet_kind_ = BULLET_KIND_3_NORMAL;
      } else {
        current_bullet_kind_ = BULLET_KIND_4_NORMAL;
      }

      allow_fire_ = true;

      SCORE android_score = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
      SCORE need_user_score = exchange_ratio_userscore_ * exchange_count_ / exchange_ratio_fishscore_;
      SCORE user_leave_score = android_score - exchange_fish_score_ * exchange_ratio_userscore_ / exchange_ratio_fishscore_;
      int times = rand() % 5 + 5;
      exchange_times_ = max(1, min(times, (int)(user_leave_score / need_user_score)));

      DWORD play_time = 10 * 60 + (rand() % 21) * 60;
      //play_time = 30;
      m_pIAndroidUserItem->SetGameTimer(IDI_STAND_UP, play_time);
      m_pIAndroidUserItem->SetGameTimer(IDI_EXCHANGE_FISHSCORE, TIME_EXCHANGE_FISHSCORE);

      return true;
  }
	return true;
}

bool CAndroidUserItemSink::OnSubGameConfig(void* data, WORD data_size) {
  ASSERT(data_size == sizeof(CMD_S_GameConfig));
  if (data_size != sizeof(CMD_S_GameConfig)) return false;
  CMD_S_GameConfig* game_config = static_cast<CMD_S_GameConfig*>(data);

  exchange_ratio_userscore_ = game_config->exchange_ratio_userscore;
  exchange_ratio_fishscore_ = game_config->exchange_ratio_fishscore;
  exchange_count_ = game_config->exchange_count;

  min_bullet_multiple_ = game_config->min_bullet_multiple;
  max_bullet_multiple_ = game_config->max_bullet_multiple;

  return true;
}

bool CAndroidUserItemSink::OnSubExchangeFishScore(void* data, WORD data_size) 
{
	ASSERT(data_size == sizeof(CMD_S_ExchangeFishScore));
	if (data_size != sizeof(CMD_S_ExchangeFishScore)) return false;
	CMD_S_ExchangeFishScore* exchange_fishscore = static_cast<CMD_S_ExchangeFishScore*>(data);

	if (exchange_fishscore->chair_id == m_pIAndroidUserItem->GetChairID()) 
	{
		fish_score_ += exchange_fishscore->swap_fish_score;
		exchange_fish_score_ = exchange_fishscore->exchange_fish_score;
		--exchange_times_;
		if (exchange_times_ <= 0) 
		{
			//m_pIAndroidUserItem->SetGameTimer(IDI_FIRE, 10);
		}
		else 
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_EXCHANGE_FISHSCORE, TIME_EXCHANGE_FISHSCORE);
		}
	}

	return true;
}

bool CAndroidUserItemSink::OnSubUserFire(void* data, WORD data_size) 
{
	ASSERT(data_size == sizeof(CMD_S_UserFire));
	if (data_size != sizeof(CMD_S_UserFire)) return false;
	CMD_S_UserFire* user_fire = static_cast<CMD_S_UserFire*>(data);


	if (user_fire->chair_id == m_pIAndroidUserItem->GetChairID()) 
	{
		fish_score_ += user_fire->fish_score;
	}

	return true;
}

bool CAndroidUserItemSink::OnSubCatchFish(void* data, WORD data_size) {
  ASSERT(data_size == sizeof(CMD_S_CatchFish));
  if (data_size != sizeof(CMD_S_CatchFish)) return false;
  CMD_S_CatchFish* catch_fish = static_cast<CMD_S_CatchFish*>(data);

  if (catch_fish->chair_id == m_pIAndroidUserItem->GetChairID()) {
    fish_score_ += catch_fish->fish_score;
    if (catch_fish->bullet_ion)
	{
		if(rand()%3 != 0)
			addBullet = 1;
		else
			addBullet = 0;

		m_pIAndroidUserItem->KillGameTimer(IDI_BULLET_MUL);

      if (current_bullet_mulriple_ < 100)
	  {
        current_bullet_kind_ = BULLET_KIND_1_ION;
      } 
	  else if (current_bullet_mulriple_ >= 100 && current_bullet_mulriple_ < 1000) 
	  {
        current_bullet_kind_ = BULLET_KIND_2_ION;
      } 
	  else if (current_bullet_mulriple_ >= 1000 && current_bullet_mulriple_ < 5000) 
	  {
        current_bullet_kind_ = BULLET_KIND_3_ION;
      } 
	  else 
	  {
        current_bullet_kind_ = BULLET_KIND_4_ION;
      }
    }
  }

  return true;
}

bool CAndroidUserItemSink::OnSubBulletIonTimeout(void* data, WORD data_size) {
  ASSERT(data_size == sizeof(CMD_S_BulletIonTimeout));
  if (data_size != sizeof(CMD_S_BulletIonTimeout)) return false;
  CMD_S_BulletIonTimeout* bullet_timeout = static_cast<CMD_S_BulletIonTimeout*>(data);
  if (bullet_timeout->chair_id == m_pIAndroidUserItem->GetChairID()) 
  {
	  //决定加减炮
	  addBullet = 0;
	m_pIAndroidUserItem->SetGameTimer(IDI_BULLET_MUL, rand()%15);

    if (current_bullet_mulriple_ < 100) 
	{
      current_bullet_kind_ = BULLET_KIND_1_NORMAL;
    } 
	else if (current_bullet_mulriple_ >= 100 && current_bullet_mulriple_ < 1000) 
	{
      current_bullet_kind_ = BULLET_KIND_2_NORMAL;
    } 
	else if (current_bullet_mulriple_ >= 1000 && current_bullet_mulriple_ < 5000) 
	{
      current_bullet_kind_ = BULLET_KIND_3_NORMAL;
    } 
	else 
	{
      current_bullet_kind_ = BULLET_KIND_4_NORMAL;
    }
  }

  return true;
}

bool CAndroidUserItemSink::OnSubCatSweepFishResult(void* data, WORD data_size) {
  ASSERT(data_size == sizeof(CMD_S_CatchSweepFishResult));
  if (data_size != sizeof(CMD_S_CatchSweepFishResult)) return false;
  CMD_S_CatchSweepFishResult* catch_sweep_result = static_cast<CMD_S_CatchSweepFishResult*>(data);

  if (catch_sweep_result->chair_id == m_pIAndroidUserItem->GetChairID()) {
    fish_score_ += catch_sweep_result->fish_score;
  }

  return true;
}

bool CAndroidUserItemSink::OnSubSwitchScene(void* data, WORD data_size) {
  ASSERT(data_size == sizeof(CMD_S_SwitchScene));
  if (data_size != sizeof(CMD_S_SwitchScene)) return false;
  CMD_S_SwitchScene* switch_scene = static_cast<CMD_S_SwitchScene*>(data);

  allow_fire_ = false;
  m_pIAndroidUserItem->SetGameTimer(IDI_SWITCH_SCENE, TIME_SWITCH_SCENE);

  return true;
}

void CAndroidUserItemSink::ExchangeFishScore() {
  CMD_C_ExchangeFishScore exchange_fishscore;
  exchange_fishscore.increase = 1;

  m_pIAndroidUserItem->SendSocketData(SUB_C_EXCHANGE_FISHSCORE, &exchange_fishscore, sizeof(exchange_fishscore));
}

void CAndroidUserItemSink::Fire() 
{
	//改变炮数
	if(addBullet!=0)
	{
		if (current_bullet_mulriple_ < 10) 
		{
			current_bullet_mulriple_+=addBullet;
			if (current_bullet_mulriple_ > max_bullet_multiple_)
				current_bullet_mulriple_ = max_bullet_multiple_;
			if (current_bullet_mulriple_ < min_bullet_multiple_)
				current_bullet_mulriple_ = min_bullet_multiple_;
		}
		else if (current_bullet_mulriple_ >= 10 && current_bullet_mulriple_ < 100) 
		{
			current_bullet_mulriple_+=addBullet*10;
			if (current_bullet_mulriple_ > max_bullet_multiple_)
				current_bullet_mulriple_ = max_bullet_multiple_;
			if (current_bullet_mulriple_ < min_bullet_multiple_)
				current_bullet_mulriple_ = min_bullet_multiple_;
		} 
		else if (current_bullet_mulriple_ >= 100 && current_bullet_mulriple_ < 1000) 
		{
			current_bullet_mulriple_+=addBullet*100;
			if (current_bullet_mulriple_ > max_bullet_multiple_)
				current_bullet_mulriple_ = max_bullet_multiple_;
			if (current_bullet_mulriple_ < min_bullet_multiple_)
				current_bullet_mulriple_ = min_bullet_multiple_;
		} 
		else 
		{
			current_bullet_mulriple_+=addBullet*1000;
			if (current_bullet_mulriple_ > max_bullet_multiple_)
				current_bullet_mulriple_ = max_bullet_multiple_;
			if (current_bullet_mulriple_ < min_bullet_multiple_)
				current_bullet_mulriple_ = min_bullet_multiple_;
		}
	}

	if(current_bullet_mulriple_ < min_bullet_multiple_*10)
		current_bullet_mulriple_ = min_bullet_multiple_*10;


	int need_fish_score = current_bullet_mulriple_;
	if (fish_score_ < need_fish_score) 
	{
		SCORE android_score = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
		SCORE need_user_score = exchange_ratio_userscore_ * exchange_count_ / exchange_ratio_fishscore_;
		SCORE user_leave_score = android_score - exchange_fish_score_ * exchange_ratio_userscore_ / exchange_ratio_fishscore_;
		int times = rand() % 5 + 5;
		exchange_times_ = max(1, min(times, (int)(user_leave_score / need_user_score)));

		current_bullet_mulriple_ = min_bullet_multiple_;
		//上分
		ExchangeFishScore();
	}
	else 
	{
		CString tempStr;
		tempStr.Format(L"摇钱树 机器人倍数%d",current_bullet_mulriple_);
		::OutputDebugString(tempStr.GetBuffer());
		if (allow_fire_) 
		{
			if (current_bullet_mulriple_ < 100) 
			{
			  current_bullet_kind_ = BULLET_KIND_1_NORMAL;
			} 
			else if (current_bullet_mulriple_ >= 100 && current_bullet_mulriple_ < 1000) 
			{
			  current_bullet_kind_ = BULLET_KIND_2_NORMAL;
			} 
			else if (current_bullet_mulriple_ >= 1000 && current_bullet_mulriple_ < 5000) 
			{
			  current_bullet_kind_ = BULLET_KIND_3_NORMAL;
			} 
			else 
			{
			  current_bullet_kind_ = BULLET_KIND_4_NORMAL;
			}

			CMD_C_UserFire user_fire;
			user_fire.bullet_kind = current_bullet_kind_;
			user_fire.angle = GetAngle(m_pIAndroidUserItem->GetChairID());
			user_fire.bullet_mulriple = current_bullet_mulriple_;
			user_fire.lock_fishid = (rand() % 100) > 50 ? -1 : 0;
			m_pIAndroidUserItem->SendSocketData(SUB_C_USER_FIRE, &user_fire, sizeof(user_fire));
		}

		//m_pIAndroidUserItem->SetGameTimer(IDI_FIRE, TIME_FIRE);
		//m_pIAndroidUserItem->SetGameTimer(IDI_FIRE, 10);
	}
}

float CAndroidUserItemSink::GetAngle(WORD chair_id) {
  //chair_id 0, 1: M_PI_2, M_PI_2 + M_PI;
  //chair_id 3: M_PI, M_PI * 2;
  //chair_id 5, 4: M_PI_2 + M_PI, M_PI_2 + M_PI + M_PI;
  //chair_id 7: M_PI * 2 , M_PI * 2 + M_PI_2, M_PI_2, M_PI;
  static const float kFireAngle[] = { 1.87f, 2.17f, 2.47f, 2.77f, 3.07f, 3.14f, 3.21f, 3.51f, 3.81f, 4.11f, 4.41f, 4.61f };
  static const float kFireAngleR[] = { 5.98f, 5.68f, 5.38f, 5.08f, 4.91f, 4.71f, 4.61f, 4.51f, 4.34f, 4.04f, 3.74f, 3.44f };
  static const float kFireAngleL[] = { 6.58f, 6.88f, 7.18f, 7.40f, 7.60f, 7.80f, 1.57f, 1.77f, 2.08f, 2.38f, 2.48f, 2.78f };
  float angle;
  int idx = 0;
  if (chair_id == 3) {
    for (int i = 0; i < CountArray(kFireAngleR); ++i) {
      if (last_fire_angle_ == kFireAngleR[i]) {
        idx = i;
        break;
      }
    }

    int n = rand() % 5 - 2;
    if (idx - n < 0) {
      idx = rand() % 5;
    } else if (idx + n >= CountArray(kFireAngleR)) {
      idx = CountArray(kFireAngleR) - rand() % 5 - 1;
    } else {
      idx = idx + n;
    }

    last_fire_angle_ = angle = kFireAngleR[idx];
  } else if (chair_id == 7) {
    for (int i = 0; i < CountArray(kFireAngleL); ++i) {
      if (last_fire_angle_ == kFireAngleL[i]) {
        idx = i;
        break;
      }
    }

    int n = rand() % 5 - 2;
    if (idx - n < 0) {
      idx = rand() % 5;
    } else if (idx + n >= CountArray(kFireAngleL)) {
      idx = CountArray(kFireAngleL) - rand() % 5 - 1;
    } else {
      idx = idx + n;
    }

    last_fire_angle_ = angle = kFireAngleL[idx];
  } else {
    for (int i = 0; i < CountArray(kFireAngle); ++i) {
      if (last_fire_angle_ == kFireAngle[i]) {
        idx = i;
        break;
      }
    }

    int n = rand() % 5 - 2;
    if (idx - n < 0) {
      idx = rand() % 5;
    } else if (idx + n >= CountArray(kFireAngle)) {
      idx = CountArray(kFireAngle) - rand() % 5 - 1;
    } else {
      idx = idx + n;
    }

    angle = kFireAngle[idx];
    if (chair_id == 4 || chair_id == 5 || chair_id == 6) {
      angle += M_PI;
    }

    last_fire_angle_ = angle;
  }

  return angle;
}
