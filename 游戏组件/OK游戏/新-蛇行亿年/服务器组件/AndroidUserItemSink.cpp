#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					1									//最少时间
#define TIME_DISPATCH				5									//发牌时间

//游戏时间
#define TIME_OUT_CARD				3									//出牌时间
#define TIME_START_GAME				8									//开始时间
#define TIME_CALL_SCORE				5									//叫分时间

//游戏时间
#define IDI_SHOOT			     	(0)			//发射子弹
#define IDI_ADDMONEY				(1)			//上分
#define IDI_CALL_SCORE				(2)			//叫分时间
#define IDI_CHANGE_SCORE				(3)			//切换倍率


//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//自己倍率
	m_BeiLv = 0;
	//自己分数
	m_AllScore = 0;
	//
	m_IsShoot = false;

	//
	m_HaveBuy = false;
	//接口变量
	m_pIAndroidUserItem=NULL;;

	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
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
	//游戏变量

	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	 case IDI_SHOOT:	//发射子弹
		{
			//m_HaveBuy = false;
			SendButtle();
			return true;
		}
		//上分
	 case  IDI_ADDMONEY:
		 {
		     m_HaveBuy = true;
 	         CMD_C_BuyBullet  AddBuyscore;
	         AddBuyscore.addormove = true;
	         AddBuyscore.score = m_BeiLv;
			 CString strLog;
			 strLog.Format(L"SXYN ADD m_BeiLv:%d",m_BeiLv);
			 OutputDebugString(strLog);

	         m_pIAndroidUserItem->SendSocketData(SUB_C_BUY_BULLET,&AddBuyscore,sizeof(AddBuyscore));
			 return true;
		 }
	 case IDI_CHANGE_SCORE:
		 {
		    m_HaveBuy = false; 
		    CMD_C_SetProbability UserBeilv;
	        UserBeilv.byCptrProbability = false;
			m_pIAndroidUserItem->SendSocketData(SUB_C_SET_PROPABILITY,&UserBeilv,sizeof(UserBeilv));
			if(UserBeilv.byCptrProbability == false)
			{
			    m_pIAndroidUserItem->SetGameTimer(IDI_ADDMONEY,rand()%3+2);
				return true;
			}
			if(rand()%5==0)
			{
			     //开始上子弹
	             m_pIAndroidUserItem->SetGameTimer(IDI_ADDMONEY,rand()%3+2);
			}
			else 
			{
			    m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_SCORE,2);
			}


		 }
	}
	return false;
}

//发射子弹
void CAndroidUserItemSink::SendButtle()
{
            CMD_C_UserShoot UserShoot;
			UserShoot.fAngle=rand()%80;
            if(SwitchChairID(m_pIAndroidUserItem->GetChairID())==0)
               UserShoot.fAngle=rand()%80;
			else if(SwitchChairID(m_pIAndroidUserItem->GetChairID())==1)
			{ 
				int a=rand()%2;
				if(a==0)UserShoot.fAngle=-rand()%80;
				else UserShoot.fAngle=rand()%80;
			}
			else if(SwitchChairID(m_pIAndroidUserItem->GetChairID())==2)
               UserShoot.fAngle=-rand()%80;

			UserShoot.dwBulletID=GetTickCount();
			m_pIAndroidUserItem->SendSocketData(SUB_C_USER_SHOOT,&UserShoot,sizeof(UserShoot));
}
//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{

	switch (wSubCmdID)
	{
	  case SUB_S_BULLET_COUNT:	//购买子弹
		{

			srand((unsigned)time(NULL));
			CMD_S_BulletCount * addfen = (CMD_S_BulletCount *)pData;
			if(addfen->isaddorremove)
			{
				if(addfen->wChairID == m_pIAndroidUserItem->GetChairID())
				{
					//自己分数
					m_AllScore = addfen->score;
					//发射子弹
				    
					if(rand()%3==0)
					{
					  if(!m_IsShoot)
					   {

						  m_IsShoot = true;
						  m_pIAndroidUserItem->SetGameTimer(IDI_SHOOT,rand()%4+2);
					   }
					}
					else
					{
						m_pIAndroidUserItem->SetGameTimer(IDI_ADDMONEY,2);
						
						
					   
					}
				}
			    
			}
			else
			{
				
			}
			return true;
		}
	  case SUB_S_ANDROID_CELL:
		  {
			//合法判断
	        ASSERT(wDataSize==sizeof(CMD_S_AndroidCellScore));
	        if (!(wDataSize==sizeof(CMD_S_AndroidCellScore))) return false;
			CMD_S_AndroidCellScore *pCellScore=(CMD_S_AndroidCellScore *)pData;
			m_BeiLv = pCellScore->score;

			//进入后设置倍率
		//	m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_SCORE,rand()%8+6);

			return true;
	
		  }
	case SUB_S_USER_SHOOT:				//发射炮弹
		{
			//合法判断
	        ASSERT(wDataSize==sizeof(CMD_S_UserShoot));
	        if (!(wDataSize==sizeof(CMD_S_UserShoot))) return false;
	        //类型转换
	        CMD_S_UserShoot *pUserShoot=(CMD_S_UserShoot *)pData;
			if(pUserShoot->dwUserScore<m_BeiLv&&m_HaveBuy)
			{
				
				m_HaveBuy= false;
			    m_pIAndroidUserItem->SetGameTimer(IDI_ADDMONEY,2);
			}
			else if(pUserShoot->dwUserScore>=m_BeiLv)
			{
 	       //发射炮弹
			if(pUserShoot->wChairID==m_pIAndroidUserItem->GetChairID())
			{

				m_pIAndroidUserItem->KillGameTimer(IDI_SHOOT);
				if(m_AllScore>=m_BeiLv)
				{
					SendButtle();
				}
				else
				{   

					//退出游戏
					m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_FREE);
				    m_pIAndroidUserItem->SendSocketData(216);
					
				}
			}
			}
			   
			return true;
		}
	}


	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	return true;
	//设置状态
	//m_pIAndroidUserItem->SendUserReady(NULL,0);
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_PLAY);
	CMD_S_GameScene * pGameScene1=(CMD_S_GameScene *)pData;
	//设置倍率
	//m_BeiLv = pGameScene1->lBulletCharge;

	CString strLog;
	strLog.Format(L"SXYN m_BeiLv:%d",m_BeiLv);
	OutputDebugString(strLog);
	//进入后设置倍率
//	m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_SCORE,rand()%8+6);

	return true;
}

//用户进入
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(VOID * pData, WORD wDataSize)
{
  //  AfxMessageBox(L"a");	

	return true;
}

//用户叫分
bool CAndroidUserItemSink::OnSubCallScore(VOID * pData, WORD wDataSize)
{
	

	return true;
}

//庄家信息
bool CAndroidUserItemSink::OnSubBankerInfo(VOID * pData, WORD wDataSize)
{
	
	return true;
}

//用户出牌
bool CAndroidUserItemSink::OnSubOutCard(VOID * pData, WORD wDataSize)
{
	

	return true;
}

//用户放弃
bool CAndroidUserItemSink::OnSubPassCard(VOID * pData, WORD wDataSize)
{
	

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	
	return true;
}
int CAndroidUserItemSink::SwitchChairID(int m_Currchairid)
{
    int changChairID = m_Currchairid;
	switch(m_pIAndroidUserItem->GetChairID())
	{
	   case 0:
	   case 1:
	   case 2:
	   {
	         switch(m_Currchairid)
			 {
			    case 0:return 0;
				case 1:return 1;
				case 2:return 2;
				case 3:return 3;
				case 4:return 4;
				case 5:return 5;
			 }
	   
	   }
	   case 3:
	   case 4:
	   case 5:
	   {
	         switch(m_Currchairid)
			 {
			    case 0:return 3;
				case 1:return 4;
				case 2:return 5;
				case 3:return 0;
				case 4:return 1;
				case 5:return 2;
			 }
	   
	   }
	}
	return changChairID;
}



//////////////////////////////////////////////////////////////////////////
