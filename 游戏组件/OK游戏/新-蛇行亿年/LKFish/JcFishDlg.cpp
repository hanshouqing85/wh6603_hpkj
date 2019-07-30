#include "stdafx.h"
#include "jcfish.h"
#include "jcfishDlg.h"
#include "..\消息定义\CMD_Game.h"
#include ".\jcfishdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDsfishDlg::CDsfishDlg(CWnd* pParent):CDialog(CDsfishDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//创建内核
	m_pClientKernel = new CClientKernel;
	//
	m_isfullscreen = false;
}

void CDsfishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDsfishDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

BOOL CDsfishDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		
    //载入创建游戏视图界面
	if(m_GameScreen.GetSafeHwnd()==NULL)
	{
		m_GameScreen.Create(AfxRegisterWndClass(0,NULL,NULL,NULL),NULL,WS_CHILD,CRect(0,0,CLIENT_WIDTH,CLIENT_HEIGHT),this,0);
		if(!m_GameScreen.InitDxDlog())
		{
			AfxMessageBox(L"设备创建失败，程序即将退出。");
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
			return TRUE;  
		}
	    
		m_GameScreen.MoveWindow(0,0,CLIENT_WIDTH,CLIENT_HEIGHT);
		m_GameScreen.ShowWindow(SW_SHOW);
	
		
	}
	
    //建立程序通信
	m_bIsConnect = m_pClientKernel->InitClientKernel(this->m_hWnd, AfxGetApp()->m_lpCmdLine, this);
    m_GameScreen.m_baseSink = this;

	MoveWindow(0,0,CLIENT_WIDTH,CLIENT_HEIGHT);
	//名称
	SetWindowText(L"蛇行亿年");
	
	return TRUE;  
}

void CDsfishDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
		CDialog::OnSysCommand(nID, lParam);
	
}
//场景信息
bool CDsfishDlg::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pData, WORD wDataSize)
{
	//场景处理
	switch (cbGameStation)
	{
	  
	  case GAME_SCENE_FREE:
	  case GAME_SCENE_PLAY:
		{
		    CMD_S_GameScene * pGameScene1=(CMD_S_GameScene *)pData;
			pGameScene.CureSeaSceneKind =  pGameScene1->CureSeaSceneKind;
			pGameScene.lBulletCharge = pGameScene1->lBulletCharge;
			for(int i=0;i<6;i++)
			{
				pGameScene.lUserCellScore[i] = pGameScene1->lUserCellScore[i];
				CString strLog;
				strLog.Format(L"CLIENT OnEventGameScene m_BeiLv[%d]:%d",i,pGameScene.lUserCellScore[i]);
				OutputDebugString(strLog);


				pGameScene.m_lUserAllScore[i] = pGameScene1->m_lUserAllScore[i];
				pGameScene.powercount[i] =  pGameScene1->powercount[i];
			}

		   
		}

	}
	return false;

}
//游戏信息
bool CDsfishDlg::OnEventSocket(const TCP_Command & Command, const void * pData, WORD wDataSize)
{
	if(m_HaveInitSceen==false)return false;
	if (MDM_GF_GAME !=Command.wMainCmdID&&Command.wSubCmdID!=SUB_GF_SYSTEM_MESSAGE)return false;
   //消息处理
	switch (Command.wSubCmdID)
	{
		case SUB_GF_SYSTEM_MESSAGE:		//系统消息
            {
				//AfxMessageBox(L"d");
				CMD_CM_SystemMessage * pMessage=(CMD_CM_SystemMessage *)pData;
                if (wDataSize<=(sizeof(CMD_CM_SystemMessage)-sizeof(pMessage->szString))) return false;
				if (pMessage->wType & SMT_CHAT)
				{
				   // AfxMessageBox(pMessage->szString);
					m_GameScreen.SetFontString(pMessage->szString);
				}
				if (pMessage->wType & SMT_GLOBAL)
				{
					CString str;
					str.Format(L"%s",pMessage->szString);
					if(str.Find(L"欢迎")<0)
					    m_GameScreen.SetSYSFontString(pMessage->szString);
				}

				return true;
			}
		case SUB_S_BULLET_COUNT://上下分
		{
			ASSERT(wDataSize==sizeof(CMD_S_BulletCount));
	        if (!(wDataSize==sizeof(CMD_S_BulletCount))) return false;
		    CMD_S_BulletCount * addfen = (CMD_S_BulletCount *)pData;
			const tagUserInfo * pUserData = m_pClientKernel->GetMeUserInfo();

			CString strLog;
			strLog.Format(L"SXFISH SUB_S_BULLET_COUNT wChairID:%d addscore:%d",addfen->wChairID,addfen->score);
			OutputDebugString(strLog);

			if(addfen->isaddorremove)
			{
				m_GameScreen.SetUserScore(SwitchChairID(addfen->wChairID),addfen->score);
				m_GameScreen.SetBaseScore(pUserData->lScore);
				//m_pClientKernel->GetMeChairID()
			    
			}
			else
			{
				m_GameScreen.SetUserScore(SwitchChairID(addfen->wChairID),0);
				m_GameScreen.SetBaseScore(pUserData->lScore);
			}
			return true;

		}
		case SUB_S_BONUS_INFO://倍率切换
			{ 
				 ASSERT(wDataSize==sizeof(CMD_S_BonusInfo));
	             if (!(wDataSize==sizeof(CMD_S_BonusInfo))) return false;
                 CMD_S_BonusInfo *pBeilv = (CMD_S_BonusInfo *)pData;
				 m_GameScreen.SetBeiLv(SwitchChairID(pBeilv->wChairID),pBeilv->lBonus);

				// AfxMessageBox("2");
				 return true;

			   
			}
		case SUB_S_TRACE_POINT:				//发送鱼儿
		{
            //增加鱼儿
			WORD wTraceCount=wDataSize/sizeof(CMD_S_FishTrace);
	        CMD_S_FishTrace *pFishTrace=(CMD_S_FishTrace *)pData;	
			m_GameScreen.AddLiKuiScore();
			for (int j=0; j<wTraceCount; ++j)
	          {
				  if(pFishTrace->bRegular==0)
				  {
				      m_GameScreen.CreatRegFishOne(&pFishTrace->m_fishtrace);
				  }
				  else if(pFishTrace->bRegular==1)
				  {
				      m_GameScreen.CreatRegFishTwo(&pFishTrace->m_fishtrace);
				  }
				  else if(pFishTrace->bRegular==2)
				  {
				      m_GameScreen.CreatRegFishThree(&pFishTrace->m_fishtrace);
				  }
				  else
				  {
				      m_GameScreen.AddStorgeFish(&pFishTrace->m_fishtrace);
				  }
                
				 pFishTrace++;
	          }	
			return true;
		}
		case SUB_S_CAPTURE_FISH:				//捕获到鱼儿了
		{
			 ASSERT(wDataSize==sizeof(CMD_S_CaptureFish));
	        if (!(wDataSize==sizeof(CMD_S_CaptureFish))) return false;
			
	        CMD_S_CaptureFish *phitFish=(CMD_S_CaptureFish *)pData;
			//设置
           // AfxMessageBox(L"2");
            
			if(phitFish->m_canSuperPao)m_GameScreen.SetSuperPao(SwitchChairID(phitFish->wChairID),phitFish->dwFishID);
			else if(phitFish->m_canSuperPao==false&&phitFish->lFishScore==0)m_GameScreen.CancelSuperPao(SwitchChairID(phitFish->wChairID));

			if(phitFish->FishKindscore==-1)
			{
				 m_GameScreen.UserAddMoney(SwitchChairID(phitFish->wChairID),phitFish->dwFishID,phitFish->lFishScore,1000,true);
				 m_GameScreen.SetBomAction(SwitchChairID(phitFish->wChairID),phitFish->dwFishID,phitFish->lFishScore);
			}
			else
			{
			  if(phitFish->lFishScore==0)
			   {
			      //m_GameClientView.m_Cocos2dXWin.runclass->AddMoney(SwitchChairID(phitFish->wChairID),phitFish->dwFishID,phitFish->lFishScore,phitFish->FishKindscore,true);
				   m_GameScreen.UserAddMoney(SwitchChairID(phitFish->wChairID),phitFish->dwFishID,phitFish->lFishScore,phitFish->FishKindscore,true);
			      //分数
			  
			   }
			  else if(phitFish->lFishScore>0)
			   {
				  m_GameScreen.UserAddMoney(SwitchChairID(phitFish->wChairID),phitFish->dwFishID,phitFish->lFishScore,phitFish->FishKindscore,true);
			     //分数
			      m_GameScreen.SetUserScore(SwitchChairID(phitFish->wChairID),phitFish->dwUserScore);
			   }
			}
			
			
			return true;
		}
      case SUB_S_CHANGE_SCENE:				//改变场景
		{

	        CMD_S_ChangeScene *pChangeScene=(CMD_S_ChangeScene *)pData;
			m_GameScreen.ChangeScreen(pChangeScene->SceneIndex);
			return true;
		}
	  case SUB_S_LOCKFISH:				//改变场景
		{
          // AfxMessageBox(L"1");
	        CMD_S_LockFishId *pChangeScene=(CMD_S_LockFishId *)pData;
			m_GameScreen.LockFishID(pChangeScene->lFishID,SwitchChairID(pChangeScene->iChairID));
			return true;
		}
	  case SUB_S_USER_SHOOT:				//发射炮弹
		{
			//合法判断
	        ASSERT(wDataSize==sizeof(CMD_S_UserShoot));
	        if (!(wDataSize==sizeof(CMD_S_UserShoot))) return false;
	        //类型转换
	        CMD_S_UserShoot *pUserShoot=(CMD_S_UserShoot *)pData;
 	       //发射炮弹
			if(pUserShoot->wChairID!=m_pClientKernel->GetMeChairID())
				m_GameScreen.UserShoot(SwitchChairID(pUserShoot->wChairID),pUserShoot->fAngle,pUserShoot->wChairID,pUserShoot->byShootCount,pUserShoot->dwZidanID);
			//分数
			m_GameScreen.SetUserScore(SwitchChairID(pUserShoot->wChairID),pUserShoot->dwUserScore);
			///
			m_GameScreen.SetPower(SwitchChairID(pUserShoot->wChairID),pUserShoot->powercount);

			if(pUserShoot->superpao)m_GameScreen.SetSuperPao(SwitchChairID(pUserShoot->wChairID),0);
			return true;
		}
	}
    return true;

}
//用户离开
bool CDsfishDlg::OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) 
{
    //AfxMessageBox(L"d");
	m_GameScreen.UserComeInorLeave(SwitchChairID(wChairID),false,wChairID);
	return true;
}
//用户进入
bool CDsfishDlg::OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUserr)
{
	m_GameScreen.UserComeInorLeave(SwitchChairID(wChairID),true,wChairID);
	return true;
}
/***************回调事件重写********************/
//连接大厅
bool  CDsfishDlg::InitClientMessage()
{
	 if(!m_bIsConnect)return true;
	 //已经载入
	 m_HaveInitSceen =  true;
	 //当前基础倍率
	 m_CurrBeilv = pGameScene.lBulletCharge;
	 m_GameScreen.SetCellScore(m_CurrBeilv);
	 //设置自己信息
	 const tagUserInfo * pUserData = m_pClientKernel->GetMeUserInfo();
	 if(pUserData!=NULL)
	 {
		 TCHAR szTitle[128];
		 _sntprintf(szTitle,sizeof(szTitle),_T("%s ［ %s - 第 %d 桌 ］F1:上分；F2:加炮；F3:减炮；F4:下分；F5:自动发炮；S:锁定；Q:取消锁定"), GAME_NAME, m_pClientKernel->GetServerAttribute()->szServerName, pUserData->wTableID+1);
		 m_GameScreen.SetGameTitle(szTitle);
	   m_GameScreen.SetMeInformation(SwitchChairID(m_pClientKernel->GetMeChairID()),m_pClientKernel->GetMeChairID(),pUserData->szNickName,pUserData->lScore);
	   for(int i=0;i<6;i++)
		  {   
            const tagUserInfo * pUserData = m_pClientKernel->GetUserInfo(i);
			if(pUserData==NULL)continue;
			//设置玩家上下分
			m_GameScreen.SetUserScore(SwitchChairID(i),pGameScene.m_lUserAllScore[i]);
			//玩家倍率
			m_GameScreen.SetBeiLv(SwitchChairID(i),pGameScene.lUserCellScore[i]);

			int addscore = pUserData->lScore;

			AddOrRemoveScore(true,addscore,true);
			m_GameScreen.SetPower(SwitchChairID(i),pGameScene.powercount[i]);
		  }
	   m_pClientKernel->SendSocketData(MDM_GF_GAME,217);
	 }
	 else
	 {
	    m_bIsConnect = m_pClientKernel->InitClientKernel(this->m_hWnd, AfxGetApp()->m_lpCmdLine, this);
        m_GameScreen.m_baseSink = this;
		InitClientMessage();
		TCHAR szTitle[128];
		 _sntprintf(szTitle,sizeof(szTitle),_T("%s 哈皮科技-精品游戏 F1:上分；F2:加炮；F3:减炮；F4:下分；F5:自动发炮；S:锁定；Q:取消锁定"), GAME_NAME);
		 m_GameScreen.SetGameTitle(szTitle);
	 }

	 return true;

}
//发送子弹消息
bool  CDsfishDlg::OnShoot(bool isSuper,float roalation,DWORD m_bulletID)
{
   CMD_C_UserShoot UserShoot;
   UserShoot.fAngle=roalation;
   UserShoot.dwBulletID = m_bulletID;
   m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_USER_SHOOT,&UserShoot,sizeof(UserShoot));
   return true;
}
		//退出游戏
bool CDsfishDlg::CloseExit()
{
	if(m_HaveInitSceen)
	{
	 AddOrRemoveScore(false,0);
     //发送游戏退出
     m_pClientKernel->SendSocketData(MDM_GF_GAME,216);
     //发送结束信道通信
     m_pClientKernel->SendData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLIENT_CLOSE);
     //销毁所有
     m_pClientKernel->DestroyClientKernel();
	}
   //关闭程序
   AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
  // m_GameScreen.CleanAllMemory();
   return true;
}
		//击中鱼儿
bool CDsfishDlg::HitFirsh(int dwFishID,int dwBulletID,int ShootUSER,bool IsAndroid,int zidanID)
{
   CMD_C_HitFish hitfish;
   hitfish.dwBulletID = zidanID;
   hitfish.dwFishID = dwFishID;
   hitfish.boolisandroid = IsAndroid;
   hitfish.bulletuserid = ShootUSER;
   m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_HIT_FISH,&hitfish,sizeof(hitfish));
   return true;
}
		//上下分
bool CDsfishDlg::AddOrRemoveScore(bool addorremove,int addscore,bool allscore)
{
//	AfxMessageBox(L"1");
	CString strLog;
	strLog.Format(L"SXFISH AddOrRemoveScore addorremove:%d addscore:%d",addorremove,addscore);
	OutputDebugString(strLog);

    CMD_C_BuyBullet  AddBuyscore;
	AddBuyscore.addormove = addorremove;
	AddBuyscore.score = addscore;
	AddBuyscore.allscore = allscore;
	m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_BUY_BULLET,&AddBuyscore,sizeof(AddBuyscore));
	return true;

}
//切换炮倍率
bool CDsfishDlg::ChangeFireBei(bool upordown)
{
	CString strLog;
	strLog.Format(L"SXFISH ChangeFireBei upordown:%d",upordown);
	OutputDebugString(strLog);
	CMD_C_SetProbability UserBeilv;
	UserBeilv.byCptrProbability = upordown;
    m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_SET_PROPABILITY,&UserBeilv,sizeof(UserBeilv));
    return true;
}
//
bool CDsfishDlg:: SendLockFishID(int FishID,int PlayerId)
{   
    CMD_C_LockFishId  Lockfish;
	Lockfish.lFishID = FishID;
	Lockfish.lChairID = PlayerId;
	m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_LOCKFISH,&Lockfish,sizeof(Lockfish));
    return true;
}
	//切换炮的速度
bool CDsfishDlg::ChangeFireSpeed(int index)
{
	CMD_C_CoinCount UserShoot;
	UserShoot.lCoinCount=index;
//	SendSocketData(SUB_C_BUY_BULLETSPEED,&UserShoot,sizeof(UserShoot));
    return true;
}
HCURSOR CDsfishDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
int CDsfishDlg::SwitchChairID(int m_Currchairid)
{
    int changChairID = m_Currchairid;
	switch(m_pClientKernel->GetMeChairID())
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


BOOL CDsfishDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    
	return true;
}
BOOL CDsfishDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
    if ( pMsg->message == WM_KEYDOWN|| pMsg->message == 25 )//鼠标移动
	{
		 switch( (WCHAR)pMsg->wParam )
            {
				case VK_F11://屏蔽ESC
				case VK_F12://屏蔽ESC
				case VK_ESCAPE://屏蔽ESC
				{
					//AfxMessageBox(L"d");
					if(!m_isfullscreen)
	                    {
							this->BringWindowToTop();
	                       m_isfullscreen = true;
                           ShowWindow(SW_MAXIMIZE);
	                       m_GameScreen.ShowWindow(SW_MAXIMIZE);
                           m_GameScreen.SetFullScreen(true);
						   m_GameScreen.SetSYSFontString(L"按ESC键可退出和进入全屏幕状态，请玩家切记！");
	                     }
	               else
	                    {
	                      ShowWindow(1);
		                  m_GameScreen.MoveWindow(0,0,CLIENT_WIDTH,CLIENT_HEIGHT);
	                      m_GameScreen.ShowWindow(1);
	                      m_isfullscreen=false;
		                  m_GameScreen.SetFullScreen(false);
	                    }
					return true;
                      
				}

			}
	}
	return __super::PreTranslateMessage(pMsg);
}

