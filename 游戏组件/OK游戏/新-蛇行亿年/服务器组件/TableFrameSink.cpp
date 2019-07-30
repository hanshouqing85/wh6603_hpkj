/*****************************************************/
//本游戏开发者:哈皮科技,联系QQ:56248
/*****************************************************/
#include "StdAfx.h"
#include "TableFrameSink.h"
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
//////////////////////////////////////////////////////////////////////////////////
const int CTableFrameSink::m_fishscore[FISHCOUNT]={2,2,3,4,5,6,7,8,9,10,12,15,18,20,25,30,40,200,300,80,80,30,350,400,150,12,20,16,36,28,48,60};

//////////////////////////////////////////////////////////////////////////
//结束时间
#define IDI_BUILD_TRACE				1									//产生轨迹
#define IDI_CLEAR_TRACE				2									//销毁轨迹
#define IDI_CHANGE_SCENE			3									//切换场景
#define IDI_REGULAR_FISH			4									//规则鱼群
#define IDI_BONUS					5									//递增彩金
#define IDI_SYS_MESSAGE0			6									//系统消息
#define IDI_SYS_MESSAGE1			7									//系统消息
#define IDI_SYS_MESSAGE2			8									//系统消息
#define IDI_SYS_MESSAGE3			9									//系统消息
#define IDI_SYS_MESSAGE4			10									//系统消息
#define IDI_SYS_MESSAGE5			11									//系统消息
#define IDI_CHECKSCORD			    12									//系统消息
#define IDI_CREADFISH			    13									//系统消息
#define IDI_CHECKUSER			    14									//系统消息
#define IDI_ANDROSEND			    15									//机器人发送消息
#define IDI_SYS_MESSAGE10				22								//系统消息
#define IDI_SYS_MESSAGE11				23								//系统消息
#define IDI_SYS_MESSAGE12				18								//系统消息
#define IDI_SYS_MESSAGE13				19								//系统消息
#define IDI_SYS_MESSAGE14				20								//系统消息
#define IDI_SYS_MESSAGE15			    21								//系统消息

#define TIME_BUILD_TRACE			3									//产生轨迹
#define TIME_CLEAR_TRACE			3									//销毁轨迹
#define TIME_CHANGE_SCENE			60*10								//切换场景
#define TIME_REGULAR_FISH			2									//规则鱼群
#define TIME_SYS_MESSAGE			60									//系统消息


#define FISH_ALIVE_TIME				80*1000								//存活时间

#define TIMER_REPEAT_TIMER			DWORD(-1)							//重复次数
//构造函数
CTableFrameSink::CTableFrameSink()
{
	m_MuserPower = 5000;
	m_dwZidanID = 0;
	//组件变量
	m_pITableFrame=NULL;
	m_pGameCustomRule=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	for(int i=0;i<6;i++)
	   m_IsAndroidHaveSit[i]=false;
    m_CurrCuCun = 0;
	m_BiliDuihuan = 1;
	m_BomFishStorg = 0;
	m_ChoushuiScore = 0;
	//AfxMessageBox(L"2");
	m_jisuan = 1;
	m_dwFishID = 0;
	m_isChangeScreen = false;
	m_EveryUpScore = 0;
	m_AndroidSuoyuPer = 0;

	for(int i=0;i<50;i++)
		for(int j=0;j<5;j++)
			m_fishtrace[i][j].m_isHave=false;
	
	m_FishCountMax=20;
	m_BigFishCount=50;
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink()
{

}

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//游戏变量


	return;
}

//配置桌子
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//错误判断
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("游戏桌子 CTableFrameSink 查询 ITableFrame 接口失败"),TraceLevel_Exception);
		return false;
	}

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//游戏配置
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	//设置文件名
	TCHAR szPath[MAX_PATH]={0};
	_sntprintf(m_szIniFileName,sizeof(m_szIniFileName),TEXT("FishConfig/fish_%d.ini"),m_pGameServiceOption->wServerID);
	ReadConfig();
	//初始库存
	m_TimeToTimeIn = GetPrivateProfileInt(TEXT("public"),TEXT("InitInScore"),10000,m_szIniFileName);
	m_TimeToTimeOut = 0;
	m_UserP = 1;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		m_lUserCellScore[i]=m_MinScore;
		m_lUserAllScore[i]=0;
		m_userisSuperPao[i]=false;
		m_UerResultScore[i]=0;
		m_UserShootCount[i]=0;
		m_ischoujiang[i]=false;
		m_userPower[i]=0;
		for(int j=0;j<200;j++)
			m_UserShoot[i][j].ishave = false;
	
	}
	//if(!IsReg()){return false;}
	if(!IsRegTwo()){return false;}
	return true;
}

//消费能力
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//最少积分
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	
	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	
	if(cbReason==GER_DISMISS&&pIServerUserItem==NULL)
	{
		for (WORD i = 0; i < GAME_PLAYER; ++i) 
		{
          IServerUserItem* user_item = m_pITableFrame->GetTableUserItem(i);
          if (user_item == NULL) continue;
		  if(m_lUserAllScore[i]>0)
	           {
	            tagScoreInfo ScoreInfoArray;
				ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
				ScoreInfoArray.lScore= m_lUserAllScore[i] / m_BiliDuihuan;
				ScoreInfoArray.lRevenue = 0;
				if ( ScoreInfoArray.lScore > 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_WIN;
				else if ( ScoreInfoArray.lScore < 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_LOSE;
				else
						ScoreInfoArray.cbType=SCORE_TYPE_DRAW;

				if(user_item!=NULL&&!user_item->IsAndroidUser())
				   user_item->WriteUserScore(ScoreInfoArray.lScore,0,0,SCORE_TYPE_WIN,0);
				m_lUserCellScore[i]=m_MinScore;
		        m_lUserAllScore[i] = 0L;
		        m_userisSuperPao[i]=false;
		        m_UerResultScore[i]=0;
		        m_UserShootCount[i]=0;
		        for(int j=0;j<200;j++)
			       m_UserShoot[i][j].ishave = false;
			   }
		  }
         //结束游戏
		 m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);
		 //取消时间
		 m_pITableFrame->KillGameTimer(IDI_BUILD_TRACE);	
		 m_pITableFrame->KillGameTimer(IDI_CLEAR_TRACE);
		 m_pITableFrame->KillGameTimer(IDI_CHANGE_SCENE);
		 m_pITableFrame->KillGameTimer(IDI_CREADFISH);
		 m_pITableFrame->KillGameTimer(IDI_ANDROSEND);
		 m_pITableFrame->KillGameTimer(IDI_CHECKUSER);
		 m_pITableFrame->KillGameTimer(17);
		 m_FishFactory.FreeAllItem();
		 //设置状态
		 m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	 	 //关闭定时
		 m_pITableFrame->KillGameTimer(IDI_BONUS);
		 return true;	
	}
	if(pIServerUserItem!=NULL)
		{
		   if(m_lUserAllScore[wChairID]>0)
	           {
	            tagScoreInfo ScoreInfoArray;
				ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
				ScoreInfoArray.lScore= m_lUserAllScore[wChairID] / m_BiliDuihuan;
				ScoreInfoArray.lRevenue = 0;
				if ( ScoreInfoArray.lScore > 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_WIN;
				else if ( ScoreInfoArray.lScore < 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_LOSE;
				else
						ScoreInfoArray.cbType=SCORE_TYPE_DRAW;

				if(m_pITableFrame->GetTableUserItem(wChairID)!=NULL&&!pIServerUserItem->IsAndroidUser())
				   pIServerUserItem->WriteUserScore(ScoreInfoArray.lScore,0,0,SCORE_TYPE_WIN,0);
				m_lUserCellScore[wChairID]=m_MinScore;
		        m_lUserAllScore[wChairID] = 0L;
		        m_userisSuperPao[wChairID]=false;
		        m_UerResultScore[wChairID]=0;
		        m_UserShootCount[wChairID]=0;
		        for(int j=0;j<200;j++)
			       m_UserShoot[wChairID][j].ishave = false;
	          }
			  
		}

		m_lUserAllScore[wChairID] = 0;
		//


		//玩家数目
		WORD wUserCount=0;
		for (WORD wIndex=0; wIndex<GAME_PLAYER; ++wIndex)
		{
			if (NULL!=m_pITableFrame->GetTableUserItem(wIndex)) ++wUserCount;
		}


		//没有玩家
		if (0==wUserCount) 
		{
			for (WORD wIndex=0; wIndex<GAME_PLAYER; ++wIndex)
		     {
			    m_lUserAllScore[wIndex] = 0;
	     	 }

		}

	return true;
}
//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	m_lUserCellScore[wChairID]=m_MinScore;
    switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:		//空闲状态
	case GAME_SCENE_PLAY:	//游戏状态
		{
			
			//消息变量
			//发送场景
			if(pIServerUserItem->IsAndroidUser())
			{
				CMD_S_AndroidCellScore AndroidCellScore;
				AndroidCellScore.score = m_MaxScore;
				m_pITableFrame->SendTableData(wChairID,SUB_S_ANDROID_CELL,&AndroidCellScore,sizeof(AndroidCellScore));
				m_lUserCellScore[wChairID]=m_MaxScore;
			}
			CMD_S_GameScene GameScene={0};
			GameScene.CureSeaSceneKind=m_bgindex;
			GameScene.lBulletCharge = m_Cellscore;

			//玩家数目
		    WORD wUserCount=0;
		    for (WORD wIndex=0; wIndex<GAME_PLAYER; ++wIndex)
		     {
			    if (NULL!=m_pITableFrame->GetTableUserItem(wIndex))
				{
				   if(!m_pITableFrame->GetTableUserItem(wIndex)->IsAndroidUser())
				   {
					   wUserCount++;
				   }
				}
		     }
            
		    if(wUserCount<=1)
			{
			    for (WORD wIndex=0; wIndex<GAME_PLAYER; ++wIndex)
		          {
			        if (NULL!=m_pITableFrame->GetTableUserItem(wIndex))
			         {
				       if(m_pITableFrame->GetTableUserItem(wIndex)->IsAndroidUser())
				       {
						   m_UserOutTime[wIndex] = 0;
						 //  m_GameTime[wIndex] = GetTickCount();
				           for(int j=0;j<200;j++)
			                   m_UserShoot[wIndex][j].ishave = false;
				       }
			        }
		          }
			}
            


			if(m_IsAndroidHaveSit[wChairID]!=pIServerUserItem->GetUserID())
		     {
			    m_IsAndroidHaveSit[wChairID]=pIServerUserItem->GetUserID();
			    m_lUserAllScore[wChairID] = 0;
			
	     	}
	
			CopyMemory(GameScene.m_lUserAllScore,m_lUserAllScore,sizeof(GameScene.m_lUserAllScore));
			CopyMemory(GameScene.lUserCellScore,m_lUserCellScore,sizeof(GameScene.lUserCellScore));

				
			return m_pITableFrame->SendGameScene(pIServerUserItem,&GameScene,sizeof(GameScene));
		}
	}

	//错误断言
	ASSERT(FALSE);
	return false;
}

//时间事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{

	//事件处理
	switch (wTimerID)
	{

	 case IDI_CHECKUSER:
	    {
			 ReadConfig();									//重新读取配置文件
              for(int i=0;i<6;i++)
			  {
				  IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				  if(pIServerUserItem!=NULL)
				  {
					  m_UserOutTime[i]++;
					  if(m_UserOutTime[i]>=130)
					  {
						 if(pIServerUserItem->IsAndroidUser())
						 {
						    m_lUserAllScore[i]=0;
						 }
						 OnActionUserOffLine(pIServerUserItem->GetChairID(),pIServerUserItem);
					     pIServerUserItem->SetUserStatus(US_FREE,pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
			             m_pITableFrame->PerformStandUpAction(pIServerUserItem);
					  }

				  }
			  }
	          return true;
	    }
	case IDI_SYS_MESSAGE0:				//消除超级炮
		{  
			 // AfxMessageBox("0");
			  WORD wChairID  =  (WORD) wBindParam;
			  CMD_S_CaptureFish CaptureFish={0};
	          CaptureFish.dwFishID=0;
	          CaptureFish.wChairID=wChairID;
	          CaptureFish.m_canSuperPao = false;
	          CaptureFish.lFishScore=0;
	          m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			  m_userisSuperPao[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE1:				//消除超级炮
		{  
			 //  AfxMessageBox("1");
			  WORD wChairID  =  (WORD) wBindParam;
			  CMD_S_CaptureFish CaptureFish={0};
	          CaptureFish.dwFishID=0;
	          CaptureFish.wChairID=wChairID;
	          CaptureFish.m_canSuperPao = false;
	          CaptureFish.lFishScore=0;
	          m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			  m_userisSuperPao[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE2:				//消除超级炮
		{  
			  // AfxMessageBox("2");
			  WORD wChairID  =  (WORD) wBindParam;
			  CMD_S_CaptureFish CaptureFish={0};
	          CaptureFish.dwFishID=0;
	          CaptureFish.wChairID=wChairID;
	          CaptureFish.m_canSuperPao = false;
	          CaptureFish.lFishScore=0;
	          m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			  m_userisSuperPao[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE3:				//消除超级炮
		{  
			 //  AfxMessageBox("3");
			  WORD wChairID  =  (WORD) wBindParam;
			  CMD_S_CaptureFish CaptureFish={0};
	          CaptureFish.dwFishID=0;
	          CaptureFish.wChairID=wChairID;
	          CaptureFish.m_canSuperPao = false;
	          CaptureFish.lFishScore=0;
	          m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			  m_userisSuperPao[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE4:				//消除超级炮
		{  
			 // AfxMessageBox("4");
			  WORD wChairID  =  (WORD) wBindParam;
			  CMD_S_CaptureFish CaptureFish={0};
	          CaptureFish.dwFishID=0;
	          CaptureFish.wChairID=wChairID;
	          CaptureFish.m_canSuperPao = false;
	          CaptureFish.lFishScore=0;
	          m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			  m_userisSuperPao[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE5:				//消除超级炮
		{  
			  // AfxMessageBox("5");
			  WORD wChairID  =  (WORD) wBindParam;
			  CMD_S_CaptureFish CaptureFish={0};
	          CaptureFish.dwFishID=0;
	          CaptureFish.wChairID=wChairID;
	          CaptureFish.m_canSuperPao = false;
	          CaptureFish.lFishScore=0;
	          m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
			  m_userisSuperPao[wChairID]=false;
			  return true;
		}   
		case IDI_SYS_MESSAGE10:				//消除超级炮
		{  
			  WORD wChairID  =  (WORD) wBindParam;
			  m_ischoujiang[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE11:				//消除超级炮
		{  
			  WORD wChairID  =  (WORD) wBindParam;
			  m_ischoujiang[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE12:				//消除超级炮
		{  
			  WORD wChairID  =  (WORD) wBindParam;
			  m_ischoujiang[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE13	:			//消除超级炮
		{  
			  WORD wChairID  =  (WORD) wBindParam;
			  m_ischoujiang[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE14:				//消除超级炮
		{  
			  WORD wChairID  =  (WORD) wBindParam;
			  m_ischoujiang[wChairID]=false;
			  return true;
		}
		case IDI_SYS_MESSAGE15:				//消除超级炮
		{  
			  WORD wChairID  =  (WORD) wBindParam;
			  m_ischoujiang[wChairID]=false;
			  return true;
		}

	case IDI_ANDROSEND:						//机器人操作
		{  
		  for(int i=0;i<GAME_PLAYER;i++)
			{

				IServerUserItem * mpIServerUserItem =  m_pITableFrame->GetTableUserItem(i);
				if( m_lUserAllScore[i]>0&&mpIServerUserItem==NULL)
				{
				   m_lUserAllScore[i]=0;
				}
				if(mpIServerUserItem!=NULL)
				{
					if(mpIServerUserItem->IsAndroidUser()==true&&(GetTickCount() - m_UserGameTime[mpIServerUserItem->GetChairID()])>(15*60*1000+(rand()%50)*60*1000+i*5*60*1000) )
					{
						if(mpIServerUserItem->IsAndroidUser())
						 {
						   // m_lUserAllScore[i]=0;
						 }
					     OnActionUserOffLine(i,mpIServerUserItem);
					     mpIServerUserItem->SetUserStatus(US_FREE,mpIServerUserItem->GetTableID(),mpIServerUserItem->GetChairID());
			             m_pITableFrame->PerformStandUpAction(mpIServerUserItem);
						 continue;
					}
					if(mpIServerUserItem->IsAndroidUser()==true&&m_lUserAllScore[i]<m_lUserCellScore[i]&&(GetTickCount() - m_UserGameTime[mpIServerUserItem->GetChairID()])>30000)
					{
						if(rand()%38!=0)
						{
						    
							int addscore =(1+rand()%5)*m_EveryUpScore;
				            if(mpIServerUserItem->GetUserScore()<addscore*m_BiliDuihuan)
			               {
				              addscore = mpIServerUserItem->GetUserScore()/m_BiliDuihuan;
			               }
				            m_lUserAllScore[i] = m_lUserAllScore[i] + addscore;

						}
						else
						{
							if(mpIServerUserItem->IsAndroidUser())
						    {
						      //m_lUserAllScore[i]=0;
						    }
						    OnActionUserOffLine(i,mpIServerUserItem);
					        mpIServerUserItem->SetUserStatus(US_FREE,mpIServerUserItem->GetTableID(),mpIServerUserItem->GetChairID());
			                m_pITableFrame->PerformStandUpAction(mpIServerUserItem);
							continue;
						}
					    
					}
				
					if(mpIServerUserItem->IsAndroidUser()==true&&m_lUserAllScore[i]>=m_lUserCellScore[i]&&!m_isChangeScreen)
					{
						//if(m_lUserCellScore[i]==m_MaxScore)
						  {
						    CMD_S_UserShoot UserShoot={0};
			                UserShoot.wChairID=i;
			                UserShoot.dwUserScore = m_lUserAllScore[i];
						    m_pITableFrame->SendTableData(i,SUB_S_USER_SHOOT,&UserShoot,sizeof(UserShoot));
						  }
					}


				}
			}
			  return true;
		}  
	case IDI_CHECKSCORD:   //查询进出分
			{
				 return true;

			}
	case IDI_BONUS:						//递增彩金
		{
		
	    	return true;
		}
	case  IDI_CREADFISH:
		{
			//判断目前鱼儿数量，避免数量过多，导致客户端卡住
			FishPack *m_fishpack=NULL;
		    INT_PTR nEnumIndex = 0;
            do
	         {
				m_fishpack=m_FishFactory.EnumActiveObject(nEnumIndex++);
				//枚举鱼群
				if (NULL==m_fishpack) break;
			 }while(true);
			 CString strLog;
			 strLog.Format(L"当前鱼数量：%ld VS 最大限制:%ld", nEnumIndex, m_FishCountMax);
			 OutputDebugString(strLog);

			if(nEnumIndex > m_FishCountMax)
				break;
		   	BYTE byBuffer[20480]={0};
	        int wSendSize=0;
	        CMD_S_FishTrace *pFishTracedx=(CMD_S_FishTrace *)byBuffer;
			bool	bCreateOnce=false;

            //是否产生一排的鱼
	        int IsCreatSmallRegFish = rand()%7;
	        if(IsCreatSmallRegFish==0 && !bCreateOnce)
	         {
				 bCreateOnce=true;
	            //随即路径
	            int m_Pathtemp =  rand()%40;
	            //随即鱼类
	            int m_FishType =  rand()%2;
				//7条；
	            for(int i = 0 ;i<7;i++)
                 {
                      FishPack *tempFish=m_FishFactory.ActiveItem();
                      if (NULL==tempFish) return true;
                      tempFish->m_BuildTime =  GetTickCount();
                      tempFish->m_fishid = GetNewFishID();
                      tempFish->m_fudaifishtype = -1;
                      tempFish->m_fishtype = m_FishType;
                      tempFish->m_fishpathindex = m_Pathtemp;
                      tempFish->m_CreatDelayTime = 10+i;
                       CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
                      pFishTracedx->bRegular = -1;
                      wSendSize+=sizeof(CMD_S_FishTrace);
                      ++pFishTracedx;
                  }
			 }
			//获取鱼类;8条
			for(int i = 0 ;i<8;i++)
			 {
				 int piacount = rand()%5;
				 FishPack *tempFish=m_FishFactory.ActiveItem();
				 if (NULL==tempFish) return  true ;
				 tempFish->m_BuildTime =  GetTickCount();
				 tempFish->m_fishid = GetNewFishID();
				 tempFish->m_fudaifishtype = -1;
				 tempFish->m_fishtype = rand()%10;
				 if(piacount==0)
					 tempFish->m_fishtype = rand()%16;
				 tempFish->m_fishpathindex =  rand()%45;
				 tempFish->m_CreatDelayTime = 1000*(i%5)+10;
				 CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
				 pFishTracedx->bRegular = -1;
				 wSendSize+=sizeof(CMD_S_FishTrace);
				 ++pFishTracedx;
			 }
			//是否产生一排的鱼
			IsCreatSmallRegFish = rand()%5;
			if(IsCreatSmallRegFish==0 && !bCreateOnce)
			{
				bCreateOnce=true;
			  //随即路径
			  int m_Pathtemp =  rand()%45;
			  //随即鱼类
			  int m_FishType = rand()%2;
			  //7条
			  for(int i = 0 ;i<7;i++)
			  {
				 FishPack *tempFish=m_FishFactory.ActiveItem();
				 if (NULL==tempFish) return true;
				 tempFish->m_BuildTime =  GetTickCount();
				 tempFish->m_fishid = GetNewFishID();
				 tempFish->m_fudaifishtype = -1;
				 tempFish->m_fishtype = m_FishType;
				 tempFish->m_fishpathindex = m_Pathtemp;
				 tempFish->m_CreatDelayTime = 10+i;
				 CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
				 pFishTracedx->bRegular = -1;
				 wSendSize+=sizeof(CMD_S_FishTrace);
				 ++pFishTracedx;
			  }
			}

			int IsCreatRegFish = rand()%3;
			if(IsCreatRegFish==0 && !bCreateOnce)
			{
				bCreateOnce=true;
			  //随即路径
			  int m_Pathtemp =  rand()%45;
			  //随即鱼类
			  int m_FishType =  rand()%6+5;
			  int m_Pcount =  rand()%5+5;
			  for(int i = 0 ;i<m_Pcount;i++)
			  {
				 FishPack *tempFish=m_FishFactory.ActiveItem();
				 if (NULL==tempFish) return true;
				 tempFish->m_BuildTime =  GetTickCount();
				 tempFish->m_fishid = GetNewFishID();
				 if(i==0)
					tempFish->m_fudaifishtype = 10;
				 else if(i>0&&i<m_Pcount-1)
					 tempFish->m_fudaifishtype = 11;
				 else
					 tempFish->m_fudaifishtype = 12;
				 tempFish->m_fishtype = m_FishType;
				 tempFish->m_fishpathindex = m_Pathtemp;
				 tempFish->m_CreatDelayTime = 90*i;
				 CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
				 pFishTracedx->bRegular = -1;
				 wSendSize+=sizeof(CMD_S_FishTrace);
				 ++pFishTracedx;
			  }
			}
	        //发送数据
	        if (wSendSize>0)
		        {
		          m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
		        }
				
			if (0!=wBindParam) 
				m_pITableFrame->SetGameTimer(IDI_CREADFISH,10000,TIMER_REPEAT_TIMER,0L);
		   return true;
		}
	case IDI_BUILD_TRACE:			//产生轨迹
		{
			CreadFish();
			//重置时间
			if (0!=wBindParam) 
				m_pITableFrame->SetGameTimer(IDI_BUILD_TRACE,TIME_BUILD_TRACE*3300,TIMER_REPEAT_TIMER,0L);

			return true;
		}
	case IDI_CLEAR_TRACE:			//销毁轨迹
		{
			//枚举变量

			FishPack *m_fishpack=NULL;
		    INT_PTR nEnumIndex = 0;
            do
	         {
		       m_fishpack=m_FishFactory.EnumActiveObject(nEnumIndex++);
		       //枚举鱼群
		       if (NULL==m_fishpack) break;
			   DWORD dwNowTime=GetTickCount();
			   if((m_fishpack->m_BuildTime+FISH_ALIVE_TIME+m_fishpack->m_CreatDelayTime)<=dwNowTime)
			   {
				   m_FishFactory.FreeItem(m_fishpack);
				   continue;

			   }
			   
			  
					    
		  }while(true);
  		  return true;
		}
	case IDI_CHANGE_SCENE:			//切换场景
		{
			//改变场景
			m_bgindex++;
			m_regfishcount = 0;
			if(m_bgindex>=3)m_bgindex=0;
			//变量定义
			CMD_S_ChangeScene ChangeScene={0};
			//设置变量

			ChangeScene.SceneIndex= m_bgindex;

			//发送消息
         	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_SCENE,&ChangeScene,sizeof(CMD_S_ChangeScene));
			//关闭时间(防止刚切换场景就发送新的鱼群)
			m_pITableFrame->KillGameTimer(IDI_BUILD_TRACE);
			//
			m_pITableFrame->KillGameTimer(IDI_CREADFISH);
			//规则鱼群
			m_pITableFrame->SetGameTimer(IDI_REGULAR_FISH,10*1000,1,1L);

			m_isChangeScreen = true;
			return true;
		}
	case IDI_REGULAR_FISH:			//规则鱼群
		{
			//AfxMessageBox(L"d");
			//两排树着的鱼儿
			if(m_bgindex==1)
				RegFishthree();
			else if (m_bgindex==0)
				RegFishone();
			else 
				RegFishtwo();
			//第三种角度线型形状          
			//对角线鱼儿
           return true;
		}
		case 16:			//规则鱼群
		{
			m_pITableFrame->SetGameTimer(IDI_BUILD_TRACE,10000,TIMER_REPEAT_TIMER,1L);
			m_pITableFrame->SetGameTimer(IDI_CREADFISH,10000,TIMER_REPEAT_TIMER,1L);
			m_pITableFrame->KillGameTimer(16);
			return true;
		}
		case 17:			//规则鱼群
		{
			  FishPack *tempFish=NULL;
	          INT_PTR nEnumIndex = 0;
              do
	           {
		         tempFish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		          //枚举鱼群
		          if (NULL==tempFish) break;

				  if(tempFish->m_fishtype==22)
			        {
						
						tempFish->m_SameFishID = tempFish->m_SameFishID + 3;
						if(tempFish->m_SameFishID>350)
							tempFish->m_SameFishID=350;
						
				        
			        }
					    
	           }while(true); 
			return true;
		}

		
	}
	return false;
}

//数据事件
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//积分事件
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}

//游戏消息
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	if(!IsReg()){return false;}
	if(!IsRegTwo()){return false;}
	//消息处理
	switch (wSubCmdID)
	{
	case SUB_C_BUY_BULLET:				//购买子弹
		{
			//合法判断
			ASSERT(wDataSize==sizeof(CMD_C_BuyBullet));
			if (wDataSize!=sizeof(CMD_C_BuyBullet))
			{

				return false;
			}

			//类型转换
			CMD_C_BuyBullet *pBuyBullet=(CMD_C_BuyBullet*)pData;	

			WORD chairID = pIServerUserItem->GetChairID();

			//如果为TRUE表示上分
			if(pBuyBullet->addormove==true)
			{
				if(pBuyBullet->allscore==true)
				{
					if(pBuyBullet->score<=0||pBuyBullet->score>pIServerUserItem->GetUserScore())
						return false;
					int addscore =pIServerUserItem->GetUserScore()*m_BiliDuihuan;
					m_lUserAllScore[chairID] = m_lUserAllScore[chairID] + addscore;
					tagScoreInfo ScoreInfoArray;
					ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
					if(m_BiliDuihuan < 1)
						m_BiliDuihuan=1;
					ScoreInfoArray.lScore=-addscore / m_BiliDuihuan;
					ScoreInfoArray.lRevenue = 0;
					if ( ScoreInfoArray.lScore > 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_WIN;
					else if ( ScoreInfoArray.lScore < 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_LOSE;
					else
						ScoreInfoArray.cbType=SCORE_TYPE_DRAW;
					//增加了处理写分时间项，10.5
					if(m_pITableFrame->GetTableUserItem(chairID)!=NULL&&pIServerUserItem->IsAndroidUser()==false)
						m_pITableFrame->WriteUserScore(chairID, ScoreInfoArray);

					CMD_S_BulletCount BulletCount={0};
					BulletCount.isaddorremove =pBuyBullet->addormove;
					BulletCount.score = m_lUserAllScore[chairID];
					BulletCount.wChairID = chairID;
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));

				}
				else
				{
					//最大与最小值
					if(pBuyBullet->score<=0||pBuyBullet->score>10000000)
						return false;
					int addscore =m_EveryUpScore;


					//不能超过身上钱
					if(pIServerUserItem->GetUserScore()<addscore/m_BiliDuihuan)
					{
						addscore = 0;//pIServerUserItem->GetUserScore()/m_BiliDuihuan;
					}

					m_lUserAllScore[chairID] = m_lUserAllScore[chairID] + addscore;

					tagScoreInfo ScoreInfoArray;
					ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
					if(m_BiliDuihuan < 1)
						m_BiliDuihuan=1;
					ScoreInfoArray.lScore=-addscore / m_BiliDuihuan;
					ScoreInfoArray.lRevenue = 0;
					if ( ScoreInfoArray.lScore > 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_WIN;
					else if ( ScoreInfoArray.lScore < 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_LOSE;
					else
						ScoreInfoArray.cbType=SCORE_TYPE_DRAW;
					//增加了处理写分时间项，10.5
					if(m_pITableFrame->GetTableUserItem(chairID)!=NULL&&pIServerUserItem->IsAndroidUser()==false)
						m_pITableFrame->WriteUserScore(chairID, ScoreInfoArray);

					CMD_S_BulletCount BulletCount={0};
					BulletCount.isaddorremove =pBuyBullet->addormove;
					BulletCount.score = m_lUserAllScore[chairID];
					BulletCount.wChairID = chairID;
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));

				}
			}
			else
			{
				CMD_S_BulletCount BulletCount={0};
			    BulletCount.isaddorremove = pBuyBullet->addormove;
		    	BulletCount.score = m_lUserAllScore[chairID];
			    BulletCount.wChairID = chairID;

				tagScoreInfo ScoreInfoArray;
				ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
				if(m_BiliDuihuan < 1)
					m_BiliDuihuan=1;
				ScoreInfoArray.lScore= (SCORE)(m_lUserAllScore[chairID]) /(SCORE)( m_BiliDuihuan);
				ScoreInfoArray.lRevenue = 0;
				if ( ScoreInfoArray.lScore > 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_WIN;
				else if ( ScoreInfoArray.lScore < 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_LOSE;
				else
						ScoreInfoArray.cbType=SCORE_TYPE_DRAW;
				//增加了处理时间项
				if(m_pITableFrame->GetTableUserItem(chairID)!=NULL&&pIServerUserItem->IsAndroidUser()==false)
				   m_pITableFrame->WriteUserScore(chairID,ScoreInfoArray);


			    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));
			    m_lUserAllScore[chairID] = 0;
				m_UserShootCount[chairID] = 0;
				for(int j=0;j<200;j++)
				{
					m_UserShoot[chairID][j].ishave = false;
					m_UserShoot[chairID][j].beilv = 0;
				}
			 

			}
           
			return true;//OnSubBuyBullet(pIServerUserItem, pBuyBullet->BulletCountKind);
		}
	case SUB_C_USER_SHOOT:
		{
			ASSERT(wDataSize==sizeof(CMD_C_UserShoot));
			if (wDataSize!=sizeof(CMD_C_UserShoot)) return false;
			CMD_C_UserShoot *pUserShoot=(CMD_C_UserShoot*)pData;
			int mchairid = pIServerUserItem->GetChairID();


			if(m_lUserAllScore[mchairid]<=0)return true;
			if(m_lUserAllScore[mchairid] - m_lUserCellScore[mchairid]<0)return true;
			//*******读取最大发射子弹数目***---------
		    int m_TmepShootCount = GetPrivateProfileInt(TEXT("public"),L"CheckShootMax",5,m_szIniFileName);
			int m_NowShootCount = 0;
			for(int j=0;j<200;j++)
			{
				if(m_UserShoot[mchairid][j].ishave == true)
				{
				   m_NowShootCount++;
				}
			}
			if(m_NowShootCount>=m_TmepShootCount)return true;


			m_UserOutTime[mchairid] = 0;
			bool RealUser=false;
			for(int i=0;i<6;i++)
			  {
				  IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				  if(pIServerUserItem!=NULL)
				  {
					  if(pIServerUserItem->IsAndroidUser()==false){RealUser=true;break;}
				  }
			  }
            if(RealUser==false)return true;

			if(GetTickCount()-m_SendTime[mchairid]<200)
			{
			   m_UserShootCount[mchairid] = 0;
			   m_SendTime[mchairid] = GetTickCount();
               for(int j=0;j<200;j++)
				  m_UserShoot[mchairid][j].ishave = false;
               return true;
			}
			m_SendTime[mchairid] = GetTickCount();

			m_lUserAllScore[mchairid] = m_lUserAllScore[mchairid] - m_lUserCellScore[mchairid];
			CMD_S_UserShoot UserShoot={0};
			UserShoot.wChairID=mchairid;
			UserShoot.fAngle=pUserShoot->fAngle;
			//给子弹机器人标志
			if(pIServerUserItem->IsAndroidUser())
			   UserShoot.byShootCount=true;
			else
			   UserShoot.byShootCount=false;
			
			m_userPower[mchairid]++;
			//如果到达能量炮就执行
			if(m_userPower[mchairid]>=m_MuserPower)
			{
				m_userPower[mchairid]=0;
				m_userisSuperPao[mchairid]=true;
				UserShoot.superpao = true;
				switch(mchairid)
		              {
		                  case 0:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE0,15000L,1,mchairid);break;}
		                  case 1:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE1,15000L,1,mchairid);break;}
		                  case 2:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE2,15000L,1,mchairid);break;}
		                  case 3:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE3,15000L,1,mchairid);break;}
		                  case 4:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE4,15000L,1,mchairid);break;}
		                  case 5:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE5,15000L,1,mchairid);break;}

		              }

			}
			int m_TmepZidanId = pUserShoot->dwBulletID;
			UserShoot.dwZidanID = m_TmepZidanId;
			UserShoot.BulletCountKind=pUserShoot->BulletCountKind;
			UserShoot.dwUserScore = m_lUserAllScore[mchairid];	
			UserShoot.powercount = (double)m_userPower[mchairid]/m_MuserPower;
	        SendDataExcludeSourceUser(pIServerUserItem,SUB_S_USER_SHOOT,&UserShoot,sizeof(UserShoot));

			//把玩家发射子弹产生的分数加到进入分里面
			//*******读取最大发射子弹数目***---------
		   int m_TmepPert = GetPrivateProfileInt(TEXT("public"),L"GetOutPercent",99,m_szIniFileName);

            if(!pIServerUserItem->IsAndroidUser())
			{
			   m_TimeToTimeIn = m_TimeToTimeIn + m_lUserCellScore[mchairid]*m_TmepPert/100;
			 
			   m_ChoushuiScore = m_ChoushuiScore + m_lUserCellScore[mchairid]*(100-m_TmepPert)/100;
			   

			}
			m_UserShootCount[mchairid]++;
			for(int j=0;j<200;j++)
			{
				if(m_UserShoot[mchairid][j].ishave == false)
				{
					m_UserShoot[mchairid][j].zidanId = m_TmepZidanId;
				    m_UserShoot[mchairid][j].ishave = true;
					m_UserShoot[mchairid][j].beilv = m_lUserCellScore[mchairid];
					break;
				}
			}
         	return true;

		}
	case SUB_C_HIT_FISH:
		{
			//合法判断
			ASSERT(wDataSize==sizeof(CMD_C_HitFish));
			if (wDataSize!=sizeof(CMD_C_HitFish)) return false;

			//类型转换
			CMD_C_HitFish *pHitFish=(CMD_C_HitFish*)pData;	
			//发射炮弹
			return OnSubHitFish(pIServerUserItem, pHitFish->dwFishID, pHitFish->dwBulletID,pHitFish->bulletuserid,pHitFish->boolisandroid);
		}
	case SUB_C_BUY_BULLETSPEED:
		{
			//合法判断
			ASSERT(wDataSize==sizeof(CMD_C_CoinCount));
			if (wDataSize!=sizeof(CMD_C_CoinCount)) return false;

			//类型转换
			CMD_C_CoinCount *pHitFish=(CMD_C_CoinCount*)pData;	
            CMD_S_CoinCount UserShoot={0};
			UserShoot.wChairID = pIServerUserItem->GetChairID();
			UserShoot.lCoinCount = pHitFish->lCoinCount;
            m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_COIN_COUNT,&UserShoot,sizeof(UserShoot));
			//发射炮弹
			return true;
		}
		
	case SUB_C_SET_PROPABILITY://设置炮的倍率
		{
			//合法判断
			ASSERT(wDataSize==sizeof(CMD_C_SetProbability));
			if (wDataSize!=sizeof(CMD_C_SetProbability)) return false;
            //如果勾了自摸加底就不能切换炮的大小
//			if(m_pGameServiceOption->cbControlStart==TRUE)return true;
			WORD chairID = pIServerUserItem->GetChairID();

			//类型转换
			CMD_C_SetProbability *pSetProbability=(CMD_C_SetProbability*)pData;
			//增加倍率
			if(pSetProbability->byCptrProbability)
			{

				//模拟炮数停止增加（不能无限制增加到最高）
				int		addscore=m_Cellscore;
				CString strLog;
				strLog.Format(L"SXFISH SUB_C_SET_PROPABILITY m_lUserCellScore[%d]:%d addscore:%d",chairID,m_lUserCellScore[chairID],addscore);
				OutputDebugString(strLog);
				
//				if(m_lUserCellScore[chairID] >= m_MaxScore *2/3 && pIServerUserItem->IsAndroidUser())
//				{
//					if(chairID % 2 == 1)
//						addscore=0;
//				}
				if( m_lUserCellScore[chairID] >= m_MaxScore/2 && pIServerUserItem->IsAndroidUser())		//pIServerUserItem->IsAndroidUser() &&
				{
					if(chairID % 2 == 0)
						addscore=0;
					else
						addscore = addscore * (rand()%5+2);
				}
				else if( m_lUserCellScore[chairID] >= m_MaxScore/3 && pIServerUserItem->IsAndroidUser())
				{
					if(chairID % 3 == 0)
						addscore=0;
					else if(chairID % 3 == 1)
						addscore=addscore;
					else
						addscore = addscore * (rand()%5+2);
				}

				m_lUserCellScore[chairID]=m_lUserCellScore[chairID]+addscore;
				if(m_lUserCellScore[chairID]>m_MaxScore)
				{
					addscore=0;
					m_lUserCellScore[chairID] = m_MinScore;
				}
				//分数为0不再进行
// 				if(addscore == 0)
// 					return true;			
			}
			else
			{
				//机器人换炮
				m_lUserCellScore[chairID]=m_lUserCellScore[chairID]-m_Cellscore;
				bool bIsAndroid = pIServerUserItem->IsAndroidUser();
				if(m_lUserCellScore[chairID]<m_MinScore)
				{
					if(bIsAndroid)
					{
						m_lUserCellScore[chairID] = m_MaxScore / (1 + (chairID * m_pITableFrame->GetTableID()) % 10) /100 * 100;
						if((m_lUserAllScore[chairID] / m_MinScore) < 10)							//打不够10炮
							m_lUserCellScore[chairID] = m_MinScore;
						if(m_lUserCellScore[chairID] < m_MinScore)
							m_lUserCellScore[chairID] = m_MinScore;
					}
					else
						m_lUserCellScore[chairID] =m_MaxScore;
				}
			}
			

			CMD_S_BonusInfo UserBei;
			UserBei.wChairID = chairID;
			UserBei.lBonus = m_lUserCellScore[chairID];
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BONUS_INFO,&UserBei,sizeof(UserBei));
			//AfxMessageBox("1");



			//发送消息

			return true;
		}
	case SUB_C_LOCKFISH: //锁鱼
		{
			ASSERT(wDataSize==sizeof(CMD_C_LockFishId));
			if (wDataSize!=sizeof(CMD_C_LockFishId)) return false;
			CMD_C_LockFishId *pMyData=(CMD_C_LockFishId*)pData;
			if(pMyData->lChairID!=pIServerUserItem->GetChairID())
			{
			   if(m_pITableFrame->GetTableUserItem(pMyData->lChairID)!=NULL)
			   {
				   if(!m_pITableFrame->GetTableUserItem(pMyData->lChairID)->IsAndroidUser()){return true;}
			   }
			   else
			   {
			       return true;
			   }
			   m_AndroidSuoyuPer =  GetPrivateProfileInt(TEXT("public"),TEXT("AndroidSuoyuPer"),0,m_szIniFileName);
			   if(rand()%10000>=m_AndroidSuoyuPer) return true;
			}
			
			CMD_S_LockFishId lockFish={0};
			lockFish.iChairID = pMyData->lChairID;
			lockFish.lFishID = pMyData->lFishID;
			return m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LOCKFISH,&lockFish,sizeof(lockFish));

		}
	case SUB_C_SET_COMEINUSER: //切换场景
		{
		  //消息变量
			//发送场景
			CMD_S_GameScene GameScene={0};
			GameScene.CureSeaSceneKind=m_bgindex;
			CopyMemory(GameScene.m_lUserAllScore,m_lUserAllScore,sizeof(GameScene.m_lUserAllScore));
			CopyMemory(GameScene.lUserCellScore,m_lUserCellScore,sizeof(GameScene.lUserCellScore));
			return m_pITableFrame->SendGameScene(pIServerUserItem,&GameScene,sizeof(GameScene));
		}
	case 216:
		{
			//用户效验

			pIServerUserItem->SetUserStatus(US_FREE,pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
			m_pITableFrame->PerformStandUpAction(pIServerUserItem);
			return true;
		}

	   //退出游戏
	case 217:
		{
		    //玩家数目
		    WORD wUserCount=0;
		    for (WORD wIndex=0; wIndex<GAME_PLAYER; ++wIndex)
		     {
			    if (NULL!=m_pITableFrame->GetTableUserItem(wIndex))
				{
				   if(!m_pITableFrame->GetTableUserItem(wIndex)->IsAndroidUser())
				   {
					   wUserCount++;
				   }
				}
		     }

		    if(wUserCount<=1)
			{
			    for (WORD wIndex=0; wIndex<GAME_PLAYER; ++wIndex)
		          {
			        if (NULL!=m_pITableFrame->GetTableUserItem(wIndex))
			         {
				       if(m_pITableFrame->GetTableUserItem(wIndex)->IsAndroidUser())
				       {
				           for(int j=0;j<200;j++)
			                   m_UserShoot[wIndex][j].ishave = false;
				       }
			        }
		          }
			}
			return true;
		}


	}

	return false;
}

//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{

	
	m_UserGameTime[wChairID] =  GetTickCount();
    pIServerUserItem->SetUserStatus(US_PLAYING,pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
	//开始判断
	if (false==bLookonUser && m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE)
	{
		m_ischoujiang[wChairID]=false;
		//开始游戏
		m_pITableFrame->SetGameTimer(IDI_BUILD_TRACE,TIME_BUILD_TRACE*1000,TIMER_REPEAT_TIMER,1L);
		m_pITableFrame->SetGameTimer(IDI_CLEAR_TRACE,TIME_CLEAR_TRACE*3000,TIMER_REPEAT_TIMER,0L);
		m_pITableFrame->SetGameTimer(IDI_CHANGE_SCENE,TIME_CHANGE_SCENE*1000,TIMER_REPEAT_TIMER,1L);
		//m_pITableFrame->SetGameTimer(IDI_CHECKSCORD,1*1000,TIMER_REPEAT_TIMER,1L);
		m_pITableFrame->SetGameTimer(IDI_CREADFISH,9000,TIMER_REPEAT_TIMER,0L);
		m_pITableFrame->SetGameTimer(IDI_ANDROSEND,300,TIMER_REPEAT_TIMER,0L);
		m_pITableFrame->SetGameTimer(IDI_CHECKUSER,5000,TIMER_REPEAT_TIMER,0L);
		m_pITableFrame->SetGameTimer(17,1000,TIMER_REPEAT_TIMER,0L);
		
						
		//设置状态
		m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);
	}
    
	//设置时间
	if (false==bLookonUser)
	{
		
        m_userPower[wChairID]=0;
		m_SitTime[wChairID] = GetTickCount();

		for(int j=0;j<200;j++)
		{
			m_UserShoot[wChairID][j].ishave = false;
			m_UserShoot[wChairID][j].beilv = 0;
		}
		int kucun = m_TimeToTimeIn-m_TimeToTimeOut;
		m_UserOutTime[wChairID] = 0;
		m_userisSuperPao[wChairID]=false;
	
	}
	//设置状态			
    if(m_pITableFrame->IsGameStarted()==false)
	{
	   m_pITableFrame->StartGame();
	   //m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);
	}	
	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(!IsReg()){return false;}
	if(!IsRegTwo()){return false;}
//结束判断
	if (false==bLookonUser)
	{
		 m_userPower[wChairID]=0;
		 m_ischoujiang[wChairID]=false;
		//计算分数
		//CalcScore(pIServerUserItem, wChairID);
         if(m_lUserAllScore[wChairID]>0)
		 {
		       tagScoreInfo ScoreInfoArray;
				ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
				ScoreInfoArray.lScore= m_lUserAllScore[wChairID] / m_BiliDuihuan;
				ScoreInfoArray.lRevenue = 0;
				if ( ScoreInfoArray.lScore > 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_WIN;
				else if ( ScoreInfoArray.lScore < 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_LOSE;
				else
						ScoreInfoArray.cbType=SCORE_TYPE_DRAW;

				if (NULL!=m_pITableFrame->GetTableUserItem(wChairID)&&!pIServerUserItem->IsAndroidUser())
				    pIServerUserItem->WriteUserScore(ScoreInfoArray.lScore,0,0,SCORE_TYPE_WIN,0);
			
		 }
		
		 m_lUserAllScore[wChairID] = 0;
		//玩家数目
		WORD wUserCount=0;
		for (WORD wIndex=0; wIndex<GAME_PLAYER; ++wIndex)
		{
			if (NULL!=m_pITableFrame->GetTableUserItem(wIndex)) ++wUserCount;
		}

		//没有玩家
		if (0==wUserCount) 
		{
			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);

			//取消时间
			m_pITableFrame->KillGameTimer(IDI_BUILD_TRACE);	
			m_pITableFrame->KillGameTimer(IDI_CLEAR_TRACE);
			m_pITableFrame->KillGameTimer(IDI_CHANGE_SCENE);
			//m_pITableFrame->KillGameTimer(IDI_CHECKSCORD);
			m_pITableFrame->KillGameTimer(IDI_CREADFISH);
			m_pITableFrame->KillGameTimer(IDI_ANDROSEND);
			m_pITableFrame->KillGameTimer(IDI_CHECKUSER);
			m_pITableFrame->KillGameTimer(17);
			m_FishFactory.FreeAllItem();
			
			//设置状态
			m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);


			//关闭定时
			m_pITableFrame->KillGameTimer(IDI_BONUS);
			//m_pITableFrame->KillGameTimer(IDI_SYS_MESSAGE);			
		}
		//设置变量
		
	}
	return true;
}

//用户放弃
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	

	return true;
}

//用户叫分
bool CTableFrameSink::OnUserCallScore(WORD wChairID, BYTE cbCallScore)
{
	

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
   return true;
}

//设置基数
void CTableFrameSink::SetGameBaseScore(LONG lBaseScore)
{
	
}

//中间开花队列
void CTableFrameSink::RegFishone()
{
  
    m_isChangeScreen = false;
	BYTE byBuffer[20480]={0};
	int wSendSize=0;
	CMD_S_FishTrace *pFishTracedx=(CMD_S_FishTrace *)byBuffer;

	for(int i =  0;i<120;i++)
	    {
	        FishPack *tempFish=m_FishFactory.ActiveItem();
	        if (NULL==tempFish) return ;
	        tempFish->m_BuildTime =  GetTickCount();
	        tempFish->m_fishid = GetNewFishID();
	        tempFish->m_fudaifishtype = -1;
	        tempFish->m_fishtype = 0;
	        tempFish->m_fishpathindex = -1;//rand()%40;
	        tempFish->m_CreatDelayTime = i;
	        CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
	        pFishTracedx->bRegular = 0;
	        wSendSize+=sizeof(CMD_S_FishTrace);
	        ++pFishTracedx;
	   }
	if (wSendSize>0)
		{
			
		   m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
		}
 
   for(int j=0;j<4;j++)
	   {
		  BYTE byBuffer1[2048]={0};
          int wSendSize1=0;
          CMD_S_FishTrace *pFishTracedx1=(CMD_S_FishTrace *)byBuffer1;
	      for(int i =  120+j*40;i<120+j*40+40;i++)
	           {
	               FishPack *tempFish=m_FishFactory.ActiveItem();
	               if (NULL==tempFish) return ;
	               tempFish->m_BuildTime =  GetTickCount();
	               tempFish->m_fishid = GetNewFishID();
	               tempFish->m_fudaifishtype = -1;
	               tempFish->m_fishtype = 1;
	               tempFish->m_fishpathindex = -1;//rand()%40;
	               tempFish->m_CreatDelayTime = i;
	               CopyMemory(&pFishTracedx1->m_fishtrace,tempFish,sizeof(FishPack));
	               pFishTracedx1->bRegular = 0;
	               wSendSize1+=sizeof(CMD_S_FishTrace);
	               ++pFishTracedx1;
	           }
         
	     if(j==3)
		 {
		           FishPack *tempFish=m_FishFactory.ActiveItem();
	               if (NULL==tempFish) return ;
	               tempFish->m_BuildTime =  GetTickCount();
	               tempFish->m_fishid = GetNewFishID();
	               tempFish->m_fudaifishtype = -1;
	               tempFish->m_fishtype = 22;
	               tempFish->m_fishpathindex = -1;//rand()%40;
	               tempFish->m_CreatDelayTime = 301;
	               CopyMemory(&pFishTracedx1->m_fishtrace,tempFish,sizeof(FishPack));
	               pFishTracedx1->bRegular = 0;
	               wSendSize1+=sizeof(CMD_S_FishTrace);
	               ++pFishTracedx1;
		 }
		 if (wSendSize1>0)
		  {
		     m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer1,wSendSize1);
		  }
	   }
	 m_pITableFrame->SetGameTimer(16,10000,TIMER_REPEAT_TIMER,1L);
	 m_pITableFrame->KillGameTimer(IDI_REGULAR_FISH);
	      
}
//相框队列
void CTableFrameSink::RegFishtwo()
{
   m_isChangeScreen = false;
  
  
   if(m_regfishcount==0)
   {
     for(int j=0;j<6;j++)
     {
	  BYTE byBuffer[20480]={0};
	  int wSendSize=0;
      CMD_S_FishTrace *pFishTracedx=(CMD_S_FishTrace *)byBuffer;
      for(int i = j*40 ;i<j*40+40;i++)
    	{
	       FishPack *tempFish=m_FishFactory.ActiveItem();
	       if (NULL==tempFish) return ;
	       tempFish->m_BuildTime =  GetTickCount();
	       tempFish->m_fishid = GetNewFishID();
	       tempFish->m_fudaifishtype = -1;
	       tempFish->m_fishtype = 0;
	       tempFish->m_fishpathindex = -1;//rand()%40;
	       tempFish->m_CreatDelayTime = i;
	       CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
	       pFishTracedx->bRegular = 1;
	       wSendSize+=sizeof(CMD_S_FishTrace);
	       ++pFishTracedx;
  	    }
	   if (wSendSize>0)
		{
			
		   m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
		}
     }
   }
   {
   int wSendSize=0;
   BYTE byBuffer[20480]={0};
   CMD_S_FishTrace *pFishTracedx=(CMD_S_FishTrace *)byBuffer;
   for(int i =  0;i<2;i++)
	    {
		   FishPack *tempFish=m_FishFactory.ActiveItem();
	       if (NULL==tempFish) return ;
	       tempFish->m_BuildTime =  GetTickCount();
	       tempFish->m_fishid = GetNewFishID();
	       tempFish->m_fudaifishtype = -1;
	       if(m_regfishcount==0)tempFish->m_fishtype=15;
		   else if(m_regfishcount==1)tempFish->m_fishtype=16;
		   else if(m_regfishcount==2)tempFish->m_fishtype=17;
		   else if(m_regfishcount==3)tempFish->m_fishtype=18;
		   else if(m_regfishcount==4)tempFish->m_fishtype=24;
		   else if(m_regfishcount==5)tempFish->m_fishtype=31;
		   else if(m_regfishcount==6)tempFish->m_fishtype=22;
	       tempFish->m_fishpathindex = -1;//rand()%40;
		   tempFish->m_SameFishID = 300;
	       tempFish->m_CreatDelayTime = i;
	       CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
	       pFishTracedx->bRegular = 1;
	       wSendSize+=sizeof(CMD_S_FishTrace);
	       ++pFishTracedx;
		   if(tempFish->m_fishtype==22)break;


	   }
		if (wSendSize>0)
		{
			
		   m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
		}
   }
  
	m_regfishcount++;
	m_pITableFrame->SetGameTimer(IDI_REGULAR_FISH,8*1000,1,1L);
	if(m_regfishcount>6)
			{
				m_regfishcount = 0;
			    m_pITableFrame->SetGameTimer(IDI_BUILD_TRACE,10000,TIMER_REPEAT_TIMER,1L);
			    m_pITableFrame->SetGameTimer(IDI_CREADFISH,10000,TIMER_REPEAT_TIMER,1L);
			   //关闭时间
			   m_pITableFrame->KillGameTimer(IDI_REGULAR_FISH);
			}
}
//圆圈队列
void CTableFrameSink::RegFishthree()
{  
  m_FishFactory.FreeAllItem();
  m_isChangeScreen = false;
  for(int j=0;j<5;j++)
	{
	  BYTE byBuffer[20480]={0};
	  int wSendSize=0;
	  CMD_S_FishTrace *pFishTracedx=(CMD_S_FishTrace *)byBuffer;
	  for(int i = j*40 ;i<j*40+40;i++)
	  {
	     FishPack *tempFish=m_FishFactory.ActiveItem();
	     if (NULL==tempFish) return ;
	     tempFish->m_BuildTime =  GetTickCount();
	     tempFish->m_fishid = GetNewFishID();
	     tempFish->m_fudaifishtype = -1;
		 
	     tempFish->m_fishtype = j;
		 if(j==2)tempFish->m_fishtype = 4;
		 else if(j==3)tempFish->m_fishtype = 7;
		 else if(j==4)tempFish->m_fishtype = 3;
		 
	     tempFish->m_fishpathindex = -1;//rand()%40;
	     tempFish->m_CreatDelayTime = i;
	     CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
	     pFishTracedx->bRegular = 2;
	     wSendSize+=sizeof(CMD_S_FishTrace);
	     ++pFishTracedx;
	  }
	  if (wSendSize>0)
		{
			
		   m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
		}
	}
  for(int j=0;j<1;j++)
	{
	  BYTE byBuffer[20480]={0};
	  int wSendSize=0;
	  CMD_S_FishTrace *pFishTracedx=(CMD_S_FishTrace *)byBuffer;
	  for(int i = j*40+200 ;i<j*40+240;i++)
	  {
	     FishPack *tempFish=m_FishFactory.ActiveItem();
	     if (NULL==tempFish) return ;
	     tempFish->m_BuildTime =  GetTickCount();
	     tempFish->m_fishid = GetNewFishID();
	     tempFish->m_fudaifishtype = -1;
	     tempFish->m_fishtype = 2;
	     tempFish->m_fishpathindex = -1;//rand()%40;
	     tempFish->m_CreatDelayTime = i;
	     CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
	     pFishTracedx->bRegular = 2;
	     wSendSize+=sizeof(CMD_S_FishTrace);
	     ++pFishTracedx;
	  }
	  for(int i =  240;i<243;i++)
	    {
	       FishPack *tempFish=m_FishFactory.ActiveItem();
	       if (NULL==tempFish) return ;
	       tempFish->m_BuildTime =  GetTickCount();
	       tempFish->m_fishid = GetNewFishID();
	       tempFish->m_fudaifishtype = -1;
	       if(i==240)tempFish->m_fishtype = 17;
		   else if(i==241)tempFish->m_fishtype = 18;
		   else if(i==242)tempFish->m_fishtype = 22;
	       tempFish->m_fishpathindex = -1;//rand()%40;
	       tempFish->m_CreatDelayTime = i;
	       CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
	       pFishTracedx->bRegular = 2;
	       wSendSize+=sizeof(CMD_S_FishTrace);
	       ++pFishTracedx;
	   }

	  if (wSendSize>0)
		{
			
		   m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
		}
	}
	 m_pITableFrame->SetGameTimer(16,35000,TIMER_REPEAT_TIMER,1L);
	 m_pITableFrame->KillGameTimer(IDI_REGULAR_FISH);
	
  
	
}
//上鱼
void CTableFrameSink::CreadFish()
{
	FishPack *m_fishpack=NULL;
	INT_PTR nEnumIndex = 0;
	do
	{
		m_fishpack=m_FishFactory.EnumActiveObject(nEnumIndex++);
		//枚举鱼群
		if (NULL==m_fishpack) break;
	}while(true);
	CString strLog;
	strLog.Format(L"当前鱼数量：%ld VS 最大限制:%ld", nEnumIndex, m_FishCountMax);
	OutputDebugString(strLog);

	if(nEnumIndex > m_FishCountMax)
		return;

	BYTE byBuffer[20480]={0};
	int wSendSize=0;
	CMD_S_FishTrace *pFishTracedx=(CMD_S_FishTrace *)byBuffer;


	//获取鱼类
	for(int i = 0 ;i<45;i++)
	{
		int piacount = rand()%5;
		FishPack *tempFish=m_FishFactory.ActiveItem();
		if (NULL==tempFish) return ;
		tempFish->m_BuildTime =  GetTickCount();
		tempFish->m_fishid = GetNewFishID();
		tempFish->m_fudaifishtype = -1;
		if(rand()%100>50)
			tempFish->m_fishtype = rand()%17;
		else
			tempFish->m_fishtype = rand()%10;
		if(piacount==0)
			tempFish->m_fishtype = rand()%16;
		tempFish->m_fishpathindex = i;
		tempFish->m_CreatDelayTime = 1120*(i%5);
		CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
		pFishTracedx->bRegular = -1;
		wSendSize+=sizeof(CMD_S_FishTrace);
		++pFishTracedx;
	}

	//是否产生一排的鱼
	int IsCreatRegFish = rand()%3;
	if(IsCreatRegFish==0)
	{
		//随即路径
		int m_Pathtemp =  rand()%45;
		//随即鱼类
		int m_FishType =  rand()%10;
		int m_Pcount =  rand()%9+3;
		for(int i = 0 ;i<m_Pcount;i++)
		{
			FishPack *tempFish=m_FishFactory.ActiveItem();
			if (NULL==tempFish) return ;
			tempFish->m_BuildTime =  GetTickCount();
			tempFish->m_fishid = GetNewFishID();
			tempFish->m_fudaifishtype = -1;
			tempFish->m_fishtype = m_FishType;
			tempFish->m_fishpathindex = m_Pathtemp;
			tempFish->m_CreatDelayTime = 80*i;
			CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
			pFishTracedx->bRegular = -1;
			wSendSize+=sizeof(CMD_S_FishTrace);
			++pFishTracedx;
		}
	}
	//随机产生0-31之间的鱼,就是产生大鱼
	int IsCreatBigFish = rand()%3;
	if(IsCreatBigFish==1)
	{
		//随即路径

		//随即鱼类
		int m_FishType =  17+rand()%15;
		for(int i = 0 ;i<4;i++)
		{
			int m_Pathtemp =  rand()%45;
			FishPack *tempFish=m_FishFactory.ActiveItem();
			if (NULL==tempFish) return ;
			tempFish->m_BuildTime =  GetTickCount();
			tempFish->m_fishid = GetNewFishID();
			tempFish->m_fudaifishtype = -1;
			tempFish->m_fishtype = 17+rand()%15;
			tempFish->m_fishpathindex = m_Pathtemp;
			tempFish->m_CreatDelayTime = i*1000+100;
			tempFish->m_SameFishID = 300;
			CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
			pFishTracedx->bRegular = -1;
			wSendSize+=sizeof(CMD_S_FishTrace);
			++pFishTracedx;
		}
	}
	//随即路径
	int m_Pathtemp =  rand()%45;
	//随即同类爆炸鱼
	int m_FishType =  rand()%10;
	for(int i = 0 ;i<2;i++)
	{
		FishPack *tempFish=m_FishFactory.ActiveItem();
		if (NULL==tempFish) return ;
		tempFish->m_BuildTime =  GetTickCount();
		tempFish->m_fishid = GetNewFishID();
		tempFish->m_fudaifishtype = 1;//rand()%2;
		tempFish->m_fishtype = m_FishType;
		tempFish->m_fishpathindex = m_Pathtemp;
		tempFish->m_CreatDelayTime = 1000*i;
		tempFish->m_SameFishID=0;
		CopyMemory(&pFishTracedx->m_fishtrace,tempFish,sizeof(FishPack));
		pFishTracedx->bRegular = -1;
		wSendSize+=sizeof(CMD_S_FishTrace);
		++pFishTracedx;
	}



	//发送数据
	if (wSendSize>0)
	{
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
	}
}

//用户断线
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{

	if(m_lUserAllScore[wChairID]>0)
	{
	   tagScoreInfo ScoreInfoArray;
				ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
				ScoreInfoArray.lScore= m_lUserAllScore[wChairID] / m_BiliDuihuan;
		ScoreInfoArray.lRevenue = 0;
		if ( ScoreInfoArray.lScore > 0 )
				ScoreInfoArray.cbType=SCORE_TYPE_WIN;
		else if ( ScoreInfoArray.lScore < 0 )
				ScoreInfoArray.cbType=SCORE_TYPE_LOSE;
		else
				ScoreInfoArray.cbType=SCORE_TYPE_DRAW;

				if(pIServerUserItem->IsAndroidUser()==false&&m_pITableFrame->GetTableUserItem(wChairID)!=NULL)
				   pIServerUserItem->WriteUserScore(ScoreInfoArray.lScore,0,0,SCORE_TYPE_WIN,0);
	}
	m_lUserAllScore[wChairID] = 0;
	m_userisSuperPao[wChairID]=false;
	m_UerResultScore[wChairID]=0;
	m_UserShootCount[wChairID]=0;
	m_ischoujiang[wChairID]=false;
	for(int j=0;j<200;j++)
			m_UserShoot[wChairID][j].ishave = false;
	return true;
}

//一排从左下角到右上角的鱼
void CTableFrameSink::CreatRegSmalFish()
{
  
}
//产生一定数量的小鱼
void CTableFrameSink::CreatSmalFish()
{
   			BYTE byBufferdx[2048]={0};
			int wSendSize=0;
			CMD_S_FishTrace *pFishTracedx=(CMD_S_FishTrace *)byBufferdx;
			//发送数据
			if (wSendSize>0)
			{
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBufferdx,wSendSize);
			}
}
//鱼群标识
int CTableFrameSink::GetNewFishID()
{
	//递增标识
	m_dwFishID+=1;
	if (0==m_dwFishID) m_dwFishID=1;
	return m_dwFishID;
}
//鱼群标识
int CTableFrameSink::GetNewZiDanID()
{
	//递增标识
	m_dwZidanID+=1;
	if (0==m_dwZidanID) m_dwZidanID=1;
	return m_dwZidanID;
}
//捕捉鱼群
bool CTableFrameSink::OnSubHitFish(IServerUserItem * pIServerUserItem, DWORD dwFishID, DWORD dwBulletID,int SendUser,bool IsAndroid)
{
	WORD wChairID = -1;
	if(IsAndroid)
	{

		wChairID = SendUser;
	}
	else
	{
	    wChairID=pIServerUserItem->GetChairID();
	}
	int nowZidanID = -1;
	
	for(int i=0;i<200;i++)
	{
	     if(m_UserShoot[wChairID][i].zidanId==dwBulletID&&m_UserShoot[wChairID][i].ishave==true)
		 {
			 nowZidanID = i;
			 m_UserShoot[wChairID][i].ishave = false;
			 break;
		 }

	}
	if(nowZidanID==-1){return true;}
    
	//判定如果是机器人标志,是否这个机器人存在,是否是机器人
	if(IsAndroid)
	{
	    IServerUserItem * mpIServerUserItem =  m_pITableFrame->GetTableUserItem(SendUser);
		if(mpIServerUserItem==NULL)return true;
        if(mpIServerUserItem->IsAndroidUser()==false)return true;
		m_UserShoot[wChairID][nowZidanID].beilv = m_lUserCellScore[wChairID];
				
	}   
     bool ishavefish = false;
	 int  fishindex = 0;
	 FishPack *m_fishpack=NULL;
	 INT_PTR nEnumIndex = 0;
     do
	    {
		    m_fishpack=m_FishFactory.EnumActiveObject(nEnumIndex++);
		       //枚举鱼群
		       if (NULL==m_fishpack) break;
			  
			   if(m_fishpack->m_fishid==dwFishID)
			   {
				   ishavefish =  true;
				   break;
				

			   }
					    
		  }while(true);
    if(ishavefish==false)return true;

	int m_Androp= GetPrivateProfileInt(TEXT("public"),TEXT("AndroPerCent"),0,m_szIniFileName);
	int percent = int(random(0,10000));
	bool androidcandie = false;

	int a = int(random(0,10000));
	int b = int(random(0,5));
	int c = int(random(0,10));
	int d = int(random(0,15));
	int e = int(random(0,50));
	int f = int(random(0,30));
	int g = int(random(0,40));
    if(IsAndroid)
	{
	
		if(percent<m_Androp&&m_fishpack->m_fishtype<7&&m_fishpack->m_fudaifishtype==-1)androidcandie=true;
	    else if(a<m_Androp&&b==1&&m_fishpack->m_fishtype<11&&m_fishpack->m_fudaifishtype!=-1)androidcandie=true;
	    else if(a<m_Androp&&c==2&&m_fishpack->m_fishtype>=11&&m_fishpack->m_fishtype<15)androidcandie=true;
	    else if(a<m_Androp&&d==3&&m_fishpack->m_fishtype>=15&&m_fishpack->m_fishtype!=23&&m_fishpack->m_fishtype!=17&&m_fishpack->m_fishtype!=18&&m_fishpack->m_fishtype!=20&&m_fishpack->m_fishtype!=21)androidcandie=true;
	    else if(a<m_Androp&&e==4&&m_fishpack->m_fishtype==23)androidcandie=true;
		else if(a<m_Androp&&e==3&&m_fishpack->m_fishtype==17)androidcandie=true;
		else if(a<m_Androp&&f==2&&m_fishpack->m_fishtype==18)androidcandie=true;
		else if(a<m_Androp&&f==5&&m_fishpack->m_fishtype==20)androidcandie=true;
		else if(a<m_Androp&&g==11&&m_fishpack->m_fishtype==21)androidcandie=true;
		else if(a<m_Androp&&g==21&&m_fishpack->m_fishtype>21)androidcandie=true;
	
	}

	//
    //判断当前实际库
	int kucun = m_TimeToTimeIn-m_TimeToTimeOut;
	TCHAR szBuffer1[32];
	_sntprintf(szBuffer1,CountArray(szBuffer1),TEXT("NKuCun_%d"),m_pITableFrame->GetTableID()+1);
	TCHAR szBuffer2[32];
	_sntprintf(szBuffer2,CountArray(szBuffer2),TEXT("%d"),kucun);
	WritePrivateProfileString(TEXT("库存"),szBuffer1,szBuffer2,m_szIniFileName);

	

	//读取鱼儿死亡率
//	_sntprintf(szBuffer1,CountArray(szBuffer1),TEXT("Shootcount_%d"),m_fishpack->m_fishtype);
//	int  m_fishDieP = GetPrivateProfileInt(TEXT("public"),szBuffer1,1,m_szIniFileName);	

	int  m_fishDieP = m_fishShoot[m_fishpack->m_fishtype][0];
	int  musustShoot = m_fishDieP;
	bool isGoodUser =  false;
	int  m_tmepbili = GetPrivateProfileInt(TEXT("public"),TEXT("EasyOrOther"),10,m_szIniFileName);	
	m_fishDieP = m_fishDieP*m_tmepbili/10;
	if(GetPrivateProfileInt(TEXT("public"),TEXT("GoodUserid"),0,m_szIniFileName)==pIServerUserItem->GetUserID())
	{
	    isGoodUser = true;
	}

	if(kucun>=0)
	    musustShoot =  m_fishDieP;
    else  if(kucun<0&&kucun>=-m_MaxScore*10)
		musustShoot = m_fishDieP/2;
	else  if(kucun<-m_MaxScore*10&&kucun>=-m_MaxScore*50)
		musustShoot = m_fishDieP/5;
	else if(kucun<-m_MaxScore*50&&kucun>=-m_MaxScore*200)
		musustShoot = m_fishDieP/10;
	else if(kucun<-m_MaxScore*200&&kucun>=-m_MaxScore*300)
		musustShoot = m_fishDieP/12;
	else if(kucun<-m_MaxScore*300&&kucun>=-m_MaxScore*5000)
		musustShoot = m_fishDieP/15;
	else
	    musustShoot = m_fishDieP/20;
	int nTempShoot=musustShoot;

	//根据库存来预计，打中这条鱼之后，不能为负分
	int nEstimateAmount = kucun - m_fishscore[m_fishpack->m_fishtype] * m_lUserCellScore[wChairID];

	if(nEstimateAmount > 0 && m_fishpack->m_fishtype>=GetPrivateProfileInt(TEXT("public"),TEXT("FISHBEGIN"),0,m_szIniFileName))
	{
		int rand_int=rand()%100;
		if(rand_int<GetPrivateProfileInt(TEXT("public"),TEXT("CANHAVEBEILV"),0,m_szIniFileName))
		 {
		   int scorestorg[6];
	       int scorebeilv[6];
	       for(int i=0;i<6;i++)
	        {
		       TCHAR szBufferScore[32];
		       _sntprintf(szBufferScore,CountArray(szBufferScore),TEXT("SCORESTORG%d"),i);
	           scorestorg[i]= GetPrivateProfileInt(TEXT("public"),szBufferScore,1000000,m_szIniFileName);
		       _sntprintf(szBufferScore,CountArray(szBufferScore),TEXT("SCOREBEILV%d"),i);
	           scorebeilv[i]= GetPrivateProfileInt(TEXT("public"),szBufferScore,50,m_szIniFileName);

			   _sntprintf(szBufferScore,CountArray(szBufferScore),TEXT("Score=%d, Per=%d"), scorestorg[i], scorebeilv[i]);
			   OutputDebugString(szBufferScore);
	        }

		  if(kucun>=scorestorg[5])
	        musustShoot = m_fishDieP*scorebeilv[5];
	      else if(kucun>=scorestorg[4]&&kucun<scorestorg[5])
	        musustShoot = m_fishDieP*scorebeilv[4];
	      else if(kucun>=scorestorg[3]&&kucun<scorestorg[4])
	        musustShoot = m_fishDieP*scorebeilv[3];
	      else if(kucun>=scorestorg[2]&&kucun<scorestorg[3])
	        musustShoot = m_fishDieP*scorebeilv[2];
	      else if(kucun>=scorestorg[1]&&kucun<scorestorg[2])
	        musustShoot = m_fishDieP*scorebeilv[1];
	      else if(kucun>=scorestorg[0]&&kucun<scorestorg[1])
	        musustShoot = m_fishDieP*scorebeilv[0];
		 }
	}

	//同类炸弹
	if(m_fishpack->m_fudaifishtype==1)musustShoot = musustShoot / 20;
	//大或者大四西元
	if(m_fishpack->m_fudaifishtype==0)musustShoot = musustShoot / 6;
	if(isGoodUser==true)
	{
	    musustShoot = m_fishDieP * 2;
	}
	if(percent>=musustShoot&&!IsAndroid)return true;
	//
	//读取垃圾玩家
	for (int i=0; i<10; i++)
	{
	   	_sntprintf(szBuffer1,CountArray(szBuffer1),TEXT("FuckUserID_%d"),i);
	    m_FuckUserID[i]=GetPrivateProfileInt(TEXT("public"),szBuffer1,0,m_szIniFileName);	
	}
	bool  fuckUser = false;
	for(int j=0;j<10;j++)
		 {
			 if(pIServerUserItem->GetUserID()==m_FuckUserID[j])
				{
				  fuckUser =  true;
				  break;
				}
					     
		 }

   
	//黑名单
	if(fuckUser&&m_fishpack->m_fishtype>10 &&!IsAndroid)
		return true;
	if(fuckUser&&!IsAndroid&&m_fishpack->m_fudaifishtype!=-1)return true;
	//如果是双倍炮，死亡概率降低
	if(m_userisSuperPao[wChairID]==true)musustShoot=musustShoot/3;
	if((percent<musustShoot&&!IsAndroid)||androidcandie)
	{

	 int fishkindscord = m_fishscore[m_fishpack->m_fishtype];
	 int FishScore = m_UserShoot[wChairID][nowZidanID].beilv*m_fishscore[m_fishpack->m_fishtype];


	 if(m_fishpack->m_fishtype==22){fishkindscord=m_fishpack->m_SameFishID;FishScore= m_UserShoot[wChairID][nowZidanID].beilv*fishkindscord;}
	 //大三元或者大四喜
	 if(m_fishpack->m_fudaifishtype==0)
	 {
		 int beilva = 3;
		 if(m_fishpack->m_CreatDelayTime>=3&&m_fishpack->m_CreatDelayTime<=6)beilva=4;
		 FishScore=m_UserShoot[wChairID][nowZidanID].beilv*m_fishscore[m_fishpack->m_fishtype]*beilva;fishkindscord =m_fishscore[m_fishpack->m_fishtype]*beilva;
	 }


	 //超级炸弹 m_fishpack->m_fishtype==23
	 if(m_fishpack->m_fishtype==23)
	 {
		 FishScore =0;
	     //fishkindscord = -1;
		 FishPack *m_fishpackBomd=NULL;
	     nEnumIndex = 0;
         do
	       {
		       m_fishpackBomd=m_FishFactory.EnumActiveObject(nEnumIndex++);
		       if (NULL==m_fishpackBomd) break;
			   if(abs(((m_fishpack->m_BuildTime+m_fishpack->m_CreatDelayTime)-(m_fishpackBomd->m_BuildTime+m_fishpackBomd->m_CreatDelayTime)))<1000
				   &&m_fishpackBomd->m_fishtype<18)
			   {
				   fishkindscord = fishkindscord + m_fishscore[m_fishpackBomd->m_fishtype];
				   FishScore=FishScore+m_UserShoot[wChairID][nowZidanID].beilv*m_fishscore[m_fishpackBomd->m_fishtype];
				   m_FishFactory.FreeItem(m_fishpackBomd);
				   CMD_S_CaptureFish CaptureFish={0};
				   CaptureFish.dwFishID=m_fishpackBomd->m_fishid;
	               CaptureFish.wChairID=wChairID;
				   CaptureFish.FishKindscore = m_fishscore[m_fishpackBomd->m_fishtype];
	               CaptureFish.lFishScore=m_UserShoot[wChairID][nowZidanID].beilv*m_fishscore[m_fishpackBomd->m_fishtype];
	               CaptureFish.dwUserScore =m_lUserAllScore[wChairID];
                   //发送消息
	               m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));

			   }
					    
		  }while(true);
	 }
	  //同类炸弹 m_fishpack->m_fishtype==20
	 if(m_fishpack->m_fudaifishtype==1)
	 {
		 FishScore =0;
		 FishPack *m_fishpackBomd=NULL;
	     nEnumIndex = 0;
         do
	       {
		       m_fishpackBomd=m_FishFactory.EnumActiveObject(nEnumIndex++);
		       if (NULL==m_fishpackBomd) break;
			   if(abs(((m_fishpack->m_BuildTime+m_fishpack->m_CreatDelayTime)-(m_fishpackBomd->m_BuildTime+m_fishpackBomd->m_CreatDelayTime)))<1000
				   &&m_fishpack->m_fishtype==m_fishpackBomd->m_fishtype
				   &&m_fishpackBomd->m_fishtype<15)
			   {
				   fishkindscord = fishkindscord + m_fishscore[m_fishpackBomd->m_fishtype];
				   FishScore=FishScore+m_UserShoot[wChairID][nowZidanID].beilv*m_fishscore[m_fishpackBomd->m_fishtype];
				   m_FishFactory.FreeItem(m_fishpackBomd);
				   CMD_S_CaptureFish CaptureFish={0};
				   CaptureFish.dwFishID=m_fishpackBomd->m_fishid;
	               CaptureFish.wChairID=wChairID;
	               CaptureFish.lFishScore=m_UserShoot[wChairID][nowZidanID].beilv*m_fishscore[m_fishpackBomd->m_fishtype];
				   CaptureFish.FishKindscore = m_fishscore[m_fishpackBomd->m_fishtype];
	               CaptureFish.dwUserScore =m_lUserAllScore[wChairID];
                   //发送消息
	               m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));

			   }
					    
		  }while(true);
	 }


	 if(m_userisSuperPao[wChairID]==true)
		 FishScore=FishScore*2;

	 if(IsAndroid==false)
	    m_TimeToTimeOut = m_TimeToTimeOut + FishScore;

	 //如果是大三元或者大四喜就执行下面操作
	 /*if(m_fishpack->m_fudaifishtype==0)
	 {
			   
				  FishPack *m_fishpackBomd=NULL;
	              nEnumIndex = 0;
                  do
	               {
		              m_fishpackBomd=m_FishFactory.EnumActiveObject(nEnumIndex++);
		              if (NULL==m_fishpackBomd) break;
					  if (m_fishpack->m_SameFishID!=m_fishpackBomd->m_SameFishID)continue;
					  if (m_fishpack->m_fishid==m_fishpackBomd->m_fishid)continue;
					  if (m_fishpack->m_fishtype!=m_fishpackBomd->m_fishtype)continue;
			          CMD_S_CaptureFish CaptureFish={0};
					  CaptureFish.dwFishID=m_fishpackBomd->m_fishid;
	                  CaptureFish.wChairID=wChairID;
	                  CaptureFish.lFishScore=0;
	                  CaptureFish.dwUserScore =m_lUserAllScore[wChairID];
                      //发送消息
	                  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
					 // m_FishFactory.FreeItem(m_fishpackBomd);
					 
		            }while(true);
				 
	 }
	 //如果是抽奖18号鱼
   if(m_fishpack->m_fishtype==18)
   {
	   int tempint[20]={0,0,3,3,3,7,7,7,6,0,3,3,3,7,7,7,2,6,7,0} ;
	   int tempscore[8] = {0,10,500,40,0,20,100,25};
       fishkindscord = tempint[rand()%20] ;
	   FishScore = tempscore[fishkindscord]*m_UserShoot[wChairID][nowZidanID].beilv;
	   if(fishkindscord==3||fishkindscord==7)
	   {
		  m_ischoujiang[wChairID] = true;
		  m_userisSuperPao[wChairID]=true;
	      switch(wChairID)
		   {
		  case 0:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE0,20000L,1,wChairID);m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE10,5000L,1,wChairID);break;}
		  case 1:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE1,20000L,1,wChairID);m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE11,5000L,1,wChairID);break;}
		  case 2:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE2,20000L,1,wChairID);m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE12,5000L,1,wChairID);break;}
		  case 3:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE3,20000L,1,wChairID);m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE13,5000L,1,wChairID);break;}
		  case 4:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE4,20000L,1,wChairID);m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE14,5000L,1,wChairID);break;}
		  case 5:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE5,20000L,1,wChairID);m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE15,5000L,1,wChairID);break;}

		  }
	   }
       
   }
	CMD_S_CaptureFish CaptureFish={0};
    if((percent%10==0&&m_userisSuperPao[wChairID]==false)&&(m_fishpack->m_fishtype>=15)&&(m_fishpack->m_fishtype!=18))
	{

		switch(wChairID)
		{
		  case 0:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE0,15000L,1,wChairID);break;}
		  case 1:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE1,15000L,1,wChairID);break;}
		  case 2:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE2,15000L,1,wChairID);break;}
		  case 3:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE3,15000L,1,wChairID);break;}
		  case 4:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE4,15000L,1,wChairID);break;}
		  case 5:{m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE5,15000L,1,wChairID);break;}

		}
		//m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE,10000L,1,wChairID);
	    CaptureFish.m_canSuperPao = true;
		m_userisSuperPao[wChairID]=true;
	
	}   */

		if(m_fishpack->m_fishtype>=17 && FishScore >= 1000000)
	{
		IServerUserItem * mpIServerUserItem =  m_pITableFrame->GetTableUserItem(wChairID);
		CString strmessage;
			strmessage.Format(L"恭喜【%s - %s】玩家〖%s〗打中大奖，获取成绩【%d】,赶紧行动吧，下一个大奖就是你的了！！！",m_pGameServiceAttrib->szGameName, m_pGameServiceOption->szServerName, mpIServerUserItem->GetNickName(),FishScore);
	    m_pITableFrame->SendGameMessage(strmessage, SMT_GLOBAL);
	}
	if(m_fishpack->m_fishtype==21)
        m_userPower[wChairID]=m_userPower[wChairID]+100;
	CMD_S_CaptureFish CaptureFish={0};
	m_FishFactory.FreeItem(m_fishpack);
	CaptureFish.dwFishID=dwFishID;
	CaptureFish.wChairID=wChairID;
	CaptureFish.lFishScore=FishScore;
	CaptureFish.FishKindscore = fishkindscord;
	//结算分数
	m_lUserAllScore[wChairID] = m_lUserAllScore[wChairID] + CaptureFish.lFishScore;
	CaptureFish.dwUserScore = m_lUserAllScore[wChairID];
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
	
   }

	
	
	m_UserShoot[wChairID][nowZidanID].beilv = 0;
	//判定是否产生一个超级炮

	return true;
}
//发送消息
void CTableFrameSink::SendDataExcludeSourceUser(IServerUserItem * pIServerUserItemSource, WORD wSubCmdID,void * pData, WORD wDataSize)
{
	//合法判断
	ASSERT(NULL!=pIServerUserItemSource);
	if (NULL==pIServerUserItemSource) return ;

	for(int i=0;i<6;i++)
			{
				IServerUserItem * mpIServerUserItem =  m_pITableFrame->GetTableUserItem(i);
				if(mpIServerUserItem!=NULL)
				{
					if(mpIServerUserItem->IsAndroidUser()==false)
					{
						
						m_pITableFrame->SendTableData(i,wSubCmdID,pData,wDataSize);
					}

				}
			}
	
}
//读取配置
void CTableFrameSink::ReadConfig()
{
	
	//读取费用
	TCHAR szBuffer[32];
	
	for (int i=0; i<FISHCOUNT; ++i)
	{
		
	     	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("Shootcount_%d"),i);
		    m_fishShoot[i][0]=GetPrivateProfileInt(TEXT("public"),szBuffer,1,m_szIniFileName);
		
	}
	
	//读取垃圾玩家
	for (int i=0; i<10; i++)
	{

	   	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("FuckUserID_%d"),i);
	    m_FuckUserID[i]=GetPrivateProfileInt(TEXT("public"),szBuffer,0,m_szIniFileName);
	}


//	int m_Choushui = GetPrivateProfileInt(TEXT("库存"),TEXT("ALLChouShui"),1,m_szIniFileName);

	//读取总抽水

	//存储总抽水
	TCHAR szBuffer1[32];
	TCHAR szBuffer2[32];
	_sntprintf(szBuffer1,CountArray(szBuffer1),TEXT("NChoushui_%d"),m_pITableFrame->GetTableID()+1);
	//	_sntprintf(szBuffer2,CountArray(szBuffer2),TEXT("%d"),m_AllChoushui+m_TimeToTimeIn*m_Choushui/1000);
	_sntprintf(szBuffer2,CountArray(szBuffer2),TEXT("%ld"), m_ChoushuiScore);

	if(m_ChoushuiScore > 0)
		WritePrivateProfileString(TEXT("库存"),szBuffer1,szBuffer2,m_szIniFileName);

    m_EveryUpScore = GetPrivateProfileInt(TEXT("public"),TEXT("EveryUpScore"),1000,m_szIniFileName);


    m_AndroidPrec = GetPrivateProfileInt(TEXT("public"),TEXT("AndroPerCent"),50,m_szIniFileName);
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("KuCun_%d"),m_pITableFrame->GetTableID()+1);
	m_Percent=GetPrivateProfileInt(TEXT("public"),TEXT("PerCent"),10,m_szIniFileName);
	m_BiliDuihuan  = GetPrivateProfileInt(TEXT("public"),TEXT("DuiHuanBili"),1,m_szIniFileName);

	m_MaxScore = GetPrivateProfileInt(TEXT("public"),TEXT("MaxScore"),100,m_szIniFileName);
	m_MinScore = GetPrivateProfileInt(TEXT("public"),TEXT("MinScore"),100,m_szIniFileName);
	m_Cellscore = GetPrivateProfileInt(TEXT("public"),TEXT("CellScore"),100,m_szIniFileName);
	m_GoodLuckFish = GetPrivateProfileInt(TEXT("public"),TEXT("GoodFishID"),100,m_szIniFileName);
	m_AndroidSuoyuPer =  GetPrivateProfileInt(TEXT("public"),TEXT("AndroidSuoyuPer"),0,m_szIniFileName);

	m_FishCountMax = GetPrivateProfileInt(TEXT("public"),TEXT("MAX_FISH_COUNT"),50,m_szIniFileName);
	m_BigFishCount = GetPrivateProfileInt(TEXT("public"),TEXT("BIG_FISH_COUNT"),50,m_szIniFileName);

}
double  CTableFrameSink::random(double start, double end)
{
   return start+(end-start)*rand()/(RAND_MAX + 1.0);

}

/*********游戏注册部分于本游戏任何逻辑都无关**************/
/*********游戏注册部分于本游戏任何逻辑都无关**************/
/*********游戏注册部分于本游戏任何逻辑都无关**************/
/*********游戏注册部分于本游戏任何逻辑都无关**************/
/*********游戏注册部分于本游戏任何逻辑都无关**************/
//获取CPUID
CString CTableFrameSink::GetCPUID()
{
    CString CPUID;
    return CPUID;
}
/////游戏注册加密码算法
#define C1 528459
#define C2 227191
//加密函数
CString CTableFrameSink:: Decrypt(CString S, WORD Key) 
{
   CString Result,str;
   return Result;
}
bool CTableFrameSink::IsReg()
{
	return true;

}
//注册判定二
bool CTableFrameSink::IsRegTwo()
{
  
	  return true;

}
//////////////////////////////////////////////////////////////////////////////////