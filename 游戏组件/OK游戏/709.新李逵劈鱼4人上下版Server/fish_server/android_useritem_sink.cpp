#include "Stdafx.h"
#include "android_useritem_sink.h"
#include "math.h"
#include "AndroidTimerEngine.h"
#include <mmsystem.h>
//////////////////////////////////////////////////////////////////////////

//定时器标识
#define IDI_SWITCH_SCENE									1						//切换场景不能打炮
#define IDI_EXCHANGE_FISHSCORE								2						//交换子弹
#define IDI_FIRE											3
#define IDI_XIA_FEN											4
#define IDI_STAND_UP										5
#define IDI_SWITCH_BULLUT									6
#define IDI_SWITCH_BULLUT_ANGELE							7
#define IID_SWITHC_FIRE_MEMBER								8						//场景更换限制开火时间
#define IDI_SWITCH_ANGELE_SCREEN							9

//时间标识
#define TIME_SWITCH_SCENE									7						//切换场景不能打炮 大概7秒钟 客户端有修改 这个应该也要修改
#define TIME_EXCHANGE_FISHSCORE								3
#define TIME_FIRE											2						//这里不能改成1 会造成死循环, 如果想要加快子弹发射速度只能改平台.
#define TIME_ANGELE											3

CAndroidTimerEngine g_AndroidTimerEngine;

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
  current_bullet_angele=0.f;
  lase_lock_fishid_=0;
  exchange_fish_score_ = 0;
  fish_score_ = 0;
  last_fire_angle_ = 0.f;
  min_bullet_multiple_ = 1;
  max_bullet_multiple_ = 9900;
  firetime_android = 0;
  fishtime_android = 0;
  screen_kind = 0;
  bNofishtime = false;
  ntime=0;
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
	ASSERT(m_pIAndroidUserItem!=NULL);
	WORD wCharidID=m_pIAndroidUserItem->GetChairID();
	switch (nTimerID) 
	{
    case IDI_SWITCH_SCENE:
		{
			if(screen_kind!=SCENE_KIND_2)
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_SWITCH_BULLUT_ANGELE);
			}
			if (screen_kind == SCENE_KIND_1)
			{
				if (wCharidID==0)
				{
					current_bullet_angele = 1.9;			//108.86°
				}
				else if (wCharidID==1)
				{
					current_bullet_angele=7.2;
				}
			}
			else if (screen_kind==SCENE_KIND_3)
			{
				if (wCharidID==0)
				{
					current_bullet_angele = 2.6;			//148.96°
				}
				else if (wCharidID==1)
				{
					current_bullet_angele=5.9;
				}
			}

			allow_fire_ = true;
			m_pIAndroidUserItem->SetGameTimer(IDI_SWITCH_ANGELE_SCREEN,4);
			return true;
		}
	case IDI_SWITCH_ANGELE_SCREEN:
		{
			ntime++;
			if (screen_kind == SCENE_KIND_1)
			{
				if(ntime<10)
				{
					if (wCharidID==0)
					{
						current_bullet_angele = 1.9+ntime*0.2;
					}
					else if (wCharidID==1)
					{
						current_bullet_angele=7.2-ntime*0.2;
					}
				}
			}

 			if(ntime<20 && (screen_kind !=SCENE_KIND_2 && screen_kind!=SCENE_KIND_4 && screen_kind!=SCENE_KIND_5))
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_SWITCH_ANGELE_SCREEN,4);
			}
			else 
			{
				ntime=0;
				m_pIAndroidUserItem->SetGameTimer(IDI_SWITCH_BULLUT_ANGELE,TIME_ANGELE+(rand()%5));
			}
		}
    case IDI_EXCHANGE_FISHSCORE:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_EXCHANGE_FISHSCORE);
			if(fish_score_<current_bullet_mulriple_)
			{
				int nTimes=rand()%5+5;
				for(int i=0;i<nTimes;i++)
				{
					ExchangeFishScore(1);
				}
			}
			m_pIAndroidUserItem->SetGameTimer(IDI_EXCHANGE_FISHSCORE, TIME_EXCHANGE_FISHSCORE);
			return true;
		}
    case IDI_FIRE:
		{
			Fire();
			return true;
		}
	case IDI_SWITCH_BULLUT_ANGELE:
		{
			current_bullet_angele=GetAngle(m_pIAndroidUserItem->GetChairID());
			m_pIAndroidUserItem->SetGameTimer(IDI_SWITCH_BULLUT_ANGELE,TIME_ANGELE+(rand()%5));
			return true;
		}
	case IDI_XIA_FEN:
		{
			ExchangeFishScore(0);
			return true;
		}
    case IDI_STAND_UP:
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_ANDROID_STAND_UP);
			return true;
		}
	case IDI_SWITCH_BULLUT:
		{
			if (max_bullet_multiple_ >= 100 && max_bullet_multiple_ <= 1000) 
			{
				int n = max_bullet_multiple_ / 100;
				int nRand = rand() % 3+3;
				if (nRand == 0) 
				{
					current_bullet_mulriple_ = min_bullet_multiple_;
				}
				else 
				{
					current_bullet_mulriple_ = nRand * 100;
					if (current_bullet_mulriple_ > max_bullet_multiple_) current_bullet_mulriple_ = max_bullet_multiple_;
				}
			} 
			else if (max_bullet_multiple_ > 1000 && max_bullet_multiple_<=10000) 
			{
				int n = max_bullet_multiple_ / 1000;
				int nRand = rand() % (n + 2);
				if (nRand == 0) 
				{
					current_bullet_mulriple_ = min_bullet_multiple_;
				}
				else 
				{
					current_bullet_mulriple_ = nRand * 1000;
					if (current_bullet_mulriple_ > max_bullet_multiple_) current_bullet_mulriple_ = max_bullet_multiple_;
				}
			}
			else if (max_bullet_multiple_ > 10000 && max_bullet_multiple_<=100000) 
			{
				int n = max_bullet_multiple_ / 10000;
				int nRand = rand() % (n + 2);
				if (nRand == 0) 
				{
					current_bullet_mulriple_ = min_bullet_multiple_;
				}
				else 
				{
					current_bullet_mulriple_ = nRand * 10000;
					if (current_bullet_mulriple_ > max_bullet_multiple_) current_bullet_mulriple_ = max_bullet_multiple_;
				}
			} 
			else if (max_bullet_multiple_ > 100000 && max_bullet_multiple_<=1000000) 
			{
				int n = max_bullet_multiple_ / 100000;
				int nRand = rand() % (n + 2);
				if (nRand == 0) 
				{
					current_bullet_mulriple_ = min_bullet_multiple_;
				}
				else 
				{
					current_bullet_mulriple_ = nRand * 100000;
					if (current_bullet_mulriple_ > max_bullet_multiple_) current_bullet_mulriple_ = max_bullet_multiple_;
				}
			}
			else 
			{
				current_bullet_mulriple_ = min_bullet_multiple_;
			}

			m_pIAndroidUserItem->KillGameTimer(IDI_SWITCH_BULLUT);
			m_pIAndroidUserItem->SetGameTimer(IDI_SWITCH_BULLUT,60*5);
			return true;
		}
	case IID_SWITHC_FIRE_MEMBER:
		{
			bNofishtime = false;
			m_pIAndroidUserItem->KillGameTimer(IID_SWITHC_FIRE_MEMBER);
			return true;
		}
	}
	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD sub_cmdid, void * data, WORD data_size)
{
  switch (sub_cmdid) 
  {
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
	case SUB_S_SCENE_END:
		bNofishtime =true;

		screen_kind = 0;
		allow_fire_ = true;
		int time=TIME_FIRE+rand()%5;
		m_pIAndroidUserItem->SetGameTimer(IID_SWITHC_FIRE_MEMBER, time);

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
  switch (cbGameStatus) 
  {
    case GAME_STATUS_FREE:
    case GAME_STATUS_PLAY:
      ASSERT(data_size == sizeof(CMD_S_GameStatus));
      if (data_size != sizeof(CMD_S_GameStatus)) return false;
      CMD_S_GameStatus* gamestatus = static_cast<CMD_S_GameStatus*>(data);
      if (gamestatus->game_version != GAME_VERSION) return false;

      fish_score_ = 0;
      exchange_fish_score_ = 0;

      if (max_bullet_multiple_ >= 100 && max_bullet_multiple_ <= 1000) 
	  {
        int n = max_bullet_multiple_ / 100;
        int nRand = rand() % (n + 1);
        if (nRand == 0) 
		{
          current_bullet_mulriple_ = min_bullet_multiple_;
        }
		else 
		{
          current_bullet_mulriple_ = nRand * 100;
          if (current_bullet_mulriple_ > max_bullet_multiple_) current_bullet_mulriple_ = max_bullet_multiple_;
        }
      } 
	  else if (max_bullet_multiple_ > 1000 && max_bullet_multiple_<=10000) 
	  {
        int n = max_bullet_multiple_ / 1000;
        int nRand = rand() % (n + 2);
        if (nRand == 0) 
		{
          current_bullet_mulriple_ = min_bullet_multiple_;
        }
		else 
		{
          current_bullet_mulriple_ = nRand * 1000;
          if (current_bullet_mulriple_ > max_bullet_multiple_) current_bullet_mulriple_ = max_bullet_multiple_;
        }
	  }
	  else if (max_bullet_multiple_ > 10000 && max_bullet_multiple_<=100000) 
	  {
		  int n = max_bullet_multiple_ / 10000;
		  int nRand = rand() % (n + 2);
		  if (nRand == 0) 
		  {
			  current_bullet_mulriple_ = min_bullet_multiple_;
		  }
		  else 
		  {
			  current_bullet_mulriple_ = nRand * 10000;
			  if (current_bullet_mulriple_ > max_bullet_multiple_) current_bullet_mulriple_ = max_bullet_multiple_;
		  }
	  } 
	  else if (max_bullet_multiple_ > 100000 && max_bullet_multiple_<=1000000) 
	  {
		  int n = max_bullet_multiple_ / 100000;
		  int nRand = rand() % (n + 2);
		  if (nRand == 0) 
		  {
			  current_bullet_mulriple_ = min_bullet_multiple_;
		  }
		  else 
		  {
			  current_bullet_mulriple_ = nRand * 100000;
			  if (current_bullet_mulriple_ > max_bullet_multiple_) current_bullet_mulriple_ = max_bullet_multiple_;
		  }
	  }
	  else 
	  {
        current_bullet_mulriple_ = min_bullet_multiple_;
      }

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

      allow_fire_ = true;

	  IServerUserItem * pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
	  if( pMeUserItem == NULL )
		  return false;

      SCORE android_score = pMeUserItem->GetUserScore();
      SCORE need_user_score = exchange_ratio_userscore_ * min_bullet_multiple_ / exchange_ratio_fishscore_;
      SCORE user_leave_score = android_score - exchange_fish_score_ * exchange_ratio_userscore_ / exchange_ratio_fishscore_;

	  exchange_times_ = 1;

      DWORD play_time = 12 * 60 + (rand() % 40) * 60;
	  if (gamestatus->isscene)
	  {
		   allow_fire_ = false;
	  }

	  m_pIAndroidUserItem->SetGameTimer(IDI_SWITCH_BULLUT,60*5);

      m_pIAndroidUserItem->SetGameTimer(IDI_STAND_UP, play_time);
      m_pIAndroidUserItem->SetGameTimer(IDI_EXCHANGE_FISHSCORE, rand()%TIME_EXCHANGE_FISHSCORE+12);

	  if (android_score<need_user_score)
	  {
		  m_pIAndroidUserItem->SetGameTimer(IDI_STAND_UP, rand()%5+3);

	  }
      return true;
  }
	return true;
}

bool CAndroidUserItemSink::OnSubGameConfig(void* data, WORD data_size) 
{
  ASSERT(data_size == sizeof(CMD_S_GameConfig));
  if (data_size != sizeof(CMD_S_GameConfig)) return false;
  CMD_S_GameConfig* game_config = static_cast<CMD_S_GameConfig*>(data);

  exchange_ratio_userscore_ = game_config->exchange_ratio_userscore;
  exchange_ratio_fishscore_ = game_config->exchange_ratio_fishscore;
  exchange_count_ = game_config->exchange_count;

  min_bullet_multiple_ = game_config->min_bullet_multiple;
  max_bullet_multiple_ = game_config->max_bullet_multiple;
  lockfishpercent = game_config->lock_percent;
  lostlockfishpercent = game_config->lostlock_percent;

  firetime_android = game_config->firetime_;
  fishtime_android = game_config->fish_time;
  max_bullet_multiple_android = game_config->max_bullet_num;

  return true;
}

bool CAndroidUserItemSink::OnSubExchangeFishScore(void* data, WORD data_size) 
{
	ASSERT(data_size == sizeof(CMD_S_ExchangeFishScore));
	if (data_size != sizeof(CMD_S_ExchangeFishScore)) return false;
	CMD_S_ExchangeFishScore* exchange_fishscore = static_cast<CMD_S_ExchangeFishScore*>(data);

	if (exchange_fishscore->chair_id == m_pIAndroidUserItem->GetChairID()) 
	{
		fish_score_ = exchange_fishscore->swap_fish_score;
		exchange_fish_score_ = exchange_fishscore->exchange_fish_score;
		--exchange_times_;
		if (exchange_times_ <= 0) 
		{
			int nTime = TIME_FIRE+ (rand()%5);
			g_AndroidTimerEngine.SetGameTimer(this, IDI_FIRE, 2*150);
			DWORD xiafen_time = 60 + (rand() % 30) * 60;
			m_pIAndroidUserItem->SetGameTimer(IDI_XIA_FEN, xiafen_time);
			m_pIAndroidUserItem->SetGameTimer(IDI_SWITCH_BULLUT_ANGELE,TIME_ANGELE+(rand()%5));

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
    fish_score_ = user_fire->fish_score;
  }

  return true;
}

bool CAndroidUserItemSink::OnSubCatchFish(void* data, WORD data_size) 
{
  ASSERT(data_size == sizeof(CMD_S_CatchFish));
  if (data_size != sizeof(CMD_S_CatchFish)) return false;
  CMD_S_CatchFish* catch_fish = static_cast<CMD_S_CatchFish*>(data);

  if (catch_fish->chair_id == m_pIAndroidUserItem->GetChairID()) 
  {
    fish_score_ = catch_fish->user_fish_score;
    if (catch_fish->bullet_ion) 
	{
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

bool CAndroidUserItemSink::OnSubBulletIonTimeout(void* data, WORD data_size) 
{
  ASSERT(data_size == sizeof(CMD_S_BulletIonTimeout));
  if (data_size != sizeof(CMD_S_BulletIonTimeout)) return false;
  CMD_S_BulletIonTimeout* bullet_timeout = static_cast<CMD_S_BulletIonTimeout*>(data);
  if (bullet_timeout->chair_id == m_pIAndroidUserItem->GetChairID()) 
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
  }

  return true;
}

bool CAndroidUserItemSink::OnSubCatSweepFishResult(void* data, WORD data_size) 
{
  ASSERT(data_size == sizeof(CMD_S_CatchSweepFishResult));
  if (data_size != sizeof(CMD_S_CatchSweepFishResult)) return false;
  CMD_S_CatchSweepFishResult* catch_sweep_result = static_cast<CMD_S_CatchSweepFishResult*>(data);

  if (catch_sweep_result->chair_id == m_pIAndroidUserItem->GetChairID()) 
  {
    fish_score_ = catch_sweep_result->user_fish_score;
  }

  return true;
}

bool CAndroidUserItemSink::OnSubSwitchScene(void* data, WORD data_size) 
{
  ASSERT(data_size == sizeof(CMD_S_SwitchScene));
  if (data_size != sizeof(CMD_S_SwitchScene)) return false;
  CMD_S_SwitchScene* switch_scene = static_cast<CMD_S_SwitchScene*>(data);

  allow_fire_ = false;
  bNofishtime = false;
  screen_kind = switch_scene->scene_kind;
 
  int time=10;
  if(screen_kind==SCENE_KIND_1)
  {
	  time=9+rand()%3;
  }
  else if (screen_kind==SCENE_KIND_2)
  {
	  time=7+rand()%3;
  }
  else if (screen_kind==SCENE_KIND_3)
  {
	  time=7+rand()%3;
  }
  else if (screen_kind==SCENE_KIND_4)
  {
	  time=7+rand()%3;
  }
  else if (screen_kind==SCENE_KIND_5)
  {
	  time=7+rand()%3;
  }

  m_pIAndroidUserItem->SetGameTimer(IDI_SWITCH_SCENE, time);

  return true;
}

void CAndroidUserItemSink::ExchangeFishScore(bool increase) 
{
  CMD_C_ExchangeFishScore exchange_fishscore;
  ZeroMemory(&exchange_fishscore, sizeof(exchange_fishscore));
  exchange_fishscore.increase = increase;

  if(m_pIAndroidUserItem!=NULL)
	  m_pIAndroidUserItem->SendSocketData(SUB_C_EXCHANGE_FISHSCORE, &exchange_fishscore, sizeof(exchange_fishscore));
}

void CAndroidUserItemSink::Fire() 
{
  int need_fish_score = current_bullet_mulriple_;
  if (fish_score_ < need_fish_score) 
  {
	  IServerUserItem * pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
	  if( pMeUserItem == NULL )
		  return;

    SCORE android_score = pMeUserItem->GetUserScore();
    SCORE need_user_score = exchange_ratio_userscore_ * exchange_count_ / exchange_ratio_fishscore_;
    SCORE user_leave_score = android_score - exchange_fish_score_ * exchange_ratio_userscore_ / exchange_ratio_fishscore_;

	
	if (fish_score_ +android_score*exchange_ratio_fishscore_ / exchange_ratio_userscore_<=current_bullet_mulriple_)
	{
		g_AndroidTimerEngine.KillGameTimer(this, IDI_FIRE);
		g_AndroidTimerEngine.SetGameTimer(this, IDI_STAND_UP, (rand()%5+3)*1000);
		return;
	}

	exchange_times_ = 1;
	int nTimes=rand()%5+5;
	for(int i=0;i<nTimes;i++)
	{
		ExchangeFishScore(1);
	}
  } 
  else 
  {
    if (allow_fire_) 
	{
      CMD_C_UserFire user_fire;
      user_fire.bullet_kind = current_bullet_kind_;
	  
	  if(current_bullet_angele<0.00001 && current_bullet_angele>-0.00001)
	  {
		  current_bullet_angele=GetAngle(m_pIAndroidUserItem->GetChairID());
		  user_fire.angle =current_bullet_angele;
	  }
      else
	  {
		  user_fire.angle =current_bullet_angele;
	  }

      user_fire.bullet_mulriple = current_bullet_mulriple_;
	  if(lase_lock_fishid_ == 0)
	  {
		  user_fire.lock_fishid = (rand() % 1000) < lockfishpercent ? -1 : 0;
		  lase_lock_fishid_=user_fire.lock_fishid;
	  }
	  else
	  {
		  if(rand()%1000 < lostlockfishpercent)
		  {
			  user_fire.lock_fishid= lase_lock_fishid_;
		  }
		  else
		  {
			  user_fire.lock_fishid =  0;
			  lase_lock_fishid_=user_fire.lock_fishid;
		  }
	  }
      m_pIAndroidUserItem->SendSocketData(SUB_C_USER_FIRE, &user_fire, sizeof(user_fire));
    }

	int nTime = TIME_FIRE+ (rand()%5);
	if (bNofishtime && screen_kind !=1)
	{
		g_AndroidTimerEngine.SetGameTimer(this, IDI_FIRE, firetime_android*100);
	}
	else
	{
		g_AndroidTimerEngine.SetGameTimer(this, IDI_FIRE, 2*150);
	}	
  }
}

//左上角是零号位，顺时针 0-3
float CAndroidUserItemSink::GetAngle(WORD chair_id) 
{
	static const float kFireAngle[] = { 2.0f, 2.17f, 2.47f, 2.77f, 3.07f, 3.14f, 3.21f, 3.51f, 3.81f, 4.11f, 4.2f, 4.3f };
	static const float kFireAngleUp[] = { 2.0f, 2.17f, 2.47f, 2.77f, 3.07f, 3.14f, 3.21f, 3.51f, 3.81f, 4.11f, 4.2f, 4.3f };
	float angle=0.f;
	int idx = 0;

	if(chair_id == 0) 
	{
		for (int i = 0; i < CountArray(kFireAngle); ++i) 
		{
			if (last_fire_angle_ == kFireAngle[i]) 
			{
				idx = i;
				break;
			}
		}

		int n = rand() % 5 - 2;
		if (idx - n < 0) 
		{
			idx = rand() % 5;
		}
		else if (idx + n >= CountArray(kFireAngle)) 
		{
			idx = CountArray(kFireAngle) - rand() % 5 - 1;
		}
		else
		{
			idx = idx + n;
		}

		angle = kFireAngle[idx];
		angle += M_PI;

		if(screen_kind==SCENE_KIND_3)
		{
			if(ntime<3)
			{
				if(angle<3.81+M_PI && angle>3.21+M_PI)
				{
					if(last_fire_angle_<3.81+M_PI && last_fire_angle_>3.21+M_PI)
					{
						last_fire_angle_=2.4f;
					}
				}
			}
			else if(ntime < 5)
			{
				if(angle<2.47+M_PI )
				{
					if(angle<2.47+M_PI )
					{
						last_fire_angle_=3.3;
					}
				}
			}
			else if (ntime>6&& ntime<9)
			{
				if(angle<3.81+M_PI && angle>3.21+M_PI)
				{
					if(last_fire_angle_<3.81+M_PI && last_fire_angle_>3.21+M_PI)
					{
						last_fire_angle_=2.4f;
					}
				}
			}
		}
		else
		{
			last_fire_angle_ = angle;
		}
	}else if(chair_id == 1) 
	{
		for (int i = 0; i < CountArray(kFireAngle); ++i) 
		{
			if (last_fire_angle_ == kFireAngle[i]) 
			{
				idx = i;
				break;
			}
		}

		int n = rand() % 5 - 2;
		if (idx - n < 0) 
		{
			idx = rand() % 5;
		}
		else if (idx + n >= CountArray(kFireAngle)) 
		{
			idx = CountArray(kFireAngle) - rand() % 5 - 1;
		}
		else
		{
			idx = idx + n;
		}


	    angle = kFireAngle[idx];
		angle += M_PI;

		if(screen_kind==SCENE_KIND_3)
		{
			if(ntime<3)
			{
				if(angle<3.81+M_PI && angle>3.21+M_PI)
				{
					if(last_fire_angle_<3.81+M_PI && last_fire_angle_>3.21+M_PI)
					{
						last_fire_angle_=2.4f;
					}
				}
			}
			else if(ntime < 5)
			{
				if(angle<2.47+M_PI )
				{
					if(angle<2.47+M_PI )
					{
						last_fire_angle_=3.3;
					}
				}
			}
			else if (ntime>6&& ntime<9)
			{
				if(angle<3.81+M_PI && angle>3.21+M_PI)
				{
					if(last_fire_angle_<3.81+M_PI && last_fire_angle_>3.21+M_PI)
					{
						last_fire_angle_=2.4f;
					}
				}
			}
		}
		else
		{
			last_fire_angle_ = angle;
		}
	}
	else
	{
		for (int i = 0; i < CountArray(kFireAngleUp); ++i) 
		{
			if (last_fire_angle_ == kFireAngleUp[i])
			{
				idx = i;
				break;
			}
		}

		int n = rand() % 5 - 2;
		if (idx - n < 0) 
		{
			idx = rand() % 5;
		}
		else if (idx + n >= CountArray(kFireAngleUp))
		{
			idx = CountArray(kFireAngleUp) - rand() % 5 - 1;
		}
		else 
		{
			idx = idx + n;
		}
		angle = kFireAngleUp[idx];
		last_fire_angle_ = angle;
	}

	return angle;

}

int CAndroidUserItemSink::Getfiretime()
{
	int time=10;

	if(screen_kind==SCENE_KIND_1)
	{
		time=5;
	}
	else if (screen_kind==SCENE_KIND_2)
	{
		time=5;
	}
	else if (screen_kind==SCENE_KIND_3)
	{
		time=5;
	}
	else if (screen_kind==SCENE_KIND_4)
	{
		time=5;
	}
	else if (screen_kind==SCENE_KIND_5)
	{
		time=5;
	}

	return time;
}
