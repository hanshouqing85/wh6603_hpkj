#include "StdAfx.h"
#include "gamescreenwnd.h"
#include ".\gamescreenwnd.h"


static BOOL bTrackLeave=FALSE; 
HM_EMITTER cur=0;
//定义
//发射炮弹
#define  TIMERIDSHOOT          1
//子弹运行
#define  TIMERIDBULLTE         2
//鱼儿游动运行
#define  TIMERIDFISHMOVE       3
#define  TIMERIDNETOPEN        4
#define  TIMERIDFISHROUND      5
#define  TIMERIDMONEYACTION    6
//玩家钱堆移动
#define  TIMERIDUSERMONEY      7
//切换场景动画
#define  TIMERIDCHANGESCREEN   8
//自动发射子弹
#define  TIMERIDSENDSHOOT      9
//
//自动发射子弹
#define  TIMERIDOUT            10
//定动作判定
#define  TIMERIDDING           11
//生成鱼
#define  CREATEFISH            12

//子弹自动发射
#define	TIMERIDALWAYSSHOOT		16


//游动图
const int GameScreenWnd::m_FishmoveCount[FISHCOUNT] = {16,16,23,14,16,17,36,17,23,24,24,24,24,23,24,24,24,16,16,20,20,20,9,10,16,8,8,8,10,10,10,7};
//死亡图
const int GameScreenWnd::m_FishdeadCount[FISHCOUNT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//活图大小
const int GameScreenWnd::m_FishmoveRec[FISHCOUNT][2] = {{67,31},{63,28},{332/4,336/6},{351/3,360/5},{536/4,276/4},{351/3,438/6},{720/6,372/6},{435/3,408/6},{800/4,870/6},{675/5,720/5},{780/4,516/6},{780/4,636/6},{780/4,876/6},{1070/5,1145/5},{1380/6,1140/4},{1200/4,684/6},{1200/4,684/6},{2280/4,1080/4},{2280/4,1080/4},{1650/5,1320/4},{1150/5,920/4},{1300/5,1320/4},{1650/3,1320/4},{1020/2,980/5},{1600/4,1648/4},{802/2,720/4},{802/2,720/4},{802/2,720/4},{824/2,1000/5},{824/2,1000/5},{824/2,1000/5},{390/3,390/3}};
//死图大小
const int GameScreenWnd::m_FishDeadRec[FISHCOUNT][2] = {{32,128},{32,64},{64,128},{128,128},{64,256},{128,128},{128,256},{128,256},{143,241},{256,256},{128,256},{256,256},{128,426},{256,512},{512,256},{256,512},{256,512},{256,256},{256,512},{256,512},{512,512},{64,512},{512,1024},{512,256}};
//
const int GameScreenWnd::m_fishscore[FISHCOUNT]={2,2,3,4,5,6,7,8,9,10,12,15,18,20,25,30,40,200,300,80,80,30,350,400,150,12,20,16,36,28,48,60};

//依次为,X坐标,Y坐标,角度,移动时间,移动速度
const int GameScreenWnd::PathIndex[45][5][5] = {

	        //低部左边出来的鱼
	        {{290,900,10,150,7},{100,100,60,250,8},{100,100,30,50,7},{100,100,5,60,6},{100,45,5,100000000,8}},
			{{310,900,10,120,8},{100,100,70,300,8},{100,100,30,100,7},{100,100,1,60,7},{100,100,10,100000000,8}},
			{{320,900,350,150,9},{100,100,300,100,5},{100,100,355,100,7},{100,100,320,60,8},{100,100,300,100000000,8}},
			{{330,900,10,150,10},{100,100,40,100,5},{100,100,90,100,6},{100,100,150,160,9},{100,100,170,100000000,8}},
			{{340,900,10,150,10},{100,100,60,200,7},{100,100,90,100,6},{100,100,150,160,5},{100,100,130,100000000,8}},
	        //低部右间出来的鱼
			{{810,900,350,150,6},{100,100,330,150,7},{100,100,280,150,7},{100,100,350,60,6},{100,45,350,100000000,8}},
			{{820,900,350,150,7},{100,100,330,55,8},{100,100,290,50,8},{100,100,260,60,7},{100,45,280,100000000,8}},
			{{830,900,350,150,8},{100,100,330,60,6},{100,100,295,50,6},{100,100,270,60,6},{100,45,280,100000000,8}},
			{{840,900,350,150,9},{100,100,335,70,9},{100,100,285,50,9},{100,100,200,60,8},{100,45,190,100000000,8}},
			{{850,900,10,150,10},{100,100,60,80,7},{100,100,45,50,8},{100,100,20,60,5},{100,45,10,100000000,8}},

	        {{310,-210,170,100,7},{100,100,120,250,8},{100,100,100,150,7},{100,100,40,60,6},{100,10,10,100000000,8}},
			{{320,-220,165,110,8},{100,100,115,300,8},{100,100,160,160,6},{100,165,1,60,7},{100,100,170,100000000,8}},
			{{330,-205,160,120,9},{100,100,100,100,5},{100,100,60,60,7},{100,100,45,60,8},{100,100,19,100000000,8}},
			{{340,-215,175,90,10},{100,100,125,100,5},{100,100,100,170,8},{100,100,80,160,7},{100,100,60,100000000,8}},
			{{350,-200,190,80,8},{100,100,200,10,7},{100,100,220,10,6},{100,100,181,160,10},{100,190,190,100000000,8}},
	        //低部右间出来的鱼
			{{820,-200,190,150,6},{100,100,240,80,7},{100,100,280,100,7},{100,100,340,60,6},{100,45,350,100000000,8}},
			{{800,-200,181,190,7},{100,100,200,55,8},{100,100,290,50,8},{100,100,260,60,6},{100,45,280,100000000,8}},
			{{850,-200,200,130,8},{100,100,250,80,6},{100,100,220,50,6},{100,100,190,60,6},{100,45,280,100000000,8}},
			{{870,-200,210,170,9},{100,100,260,70,9},{100,100,270,50,9},{100,100,280,60,6},{100,45,290,100000000,8}},
			{{890,-200,200,160,10},{100,100,260,80,7},{100,100,220,50,8},{100,100,190,60,6},{100,45,185,100000000,8}},
	         
	         //右下角出来的鱼
			{{1480,790,325,250,7},{100,100,265,400,8},{100,100,220,600,7},{100,100,300,60,6},{100,45,200,100000000,8}},
			{{1450,800,330,230,8},{100,100,270,300,8},{100,100,325,100,7},{100,100,315,60,6},{100,100,300,100000000,8}},
			{{1460,790,310,245,9},{100,100,260,300,5},{100,100,220,100,7},{100,100,200,60,6},{100,100,200,100000000,8}},
			{{1470,810,315,240,10},{100,100,275,300,5},{100,100,335,300,6},{100,100,310,160,6},{100,100,200,100000000,8}},
			{{1490,780,315,180,8},{100,100,280,450,7},{100,100,325,500,7},{100,100,322,1000060,6},{100,245,340,100000000,8}},

	         //右上角出来的鱼
			{{1480,-50,230,200,9},{100,100,260,300,5},{100,100,220,100,7},{100,100,200,60,6},{100,100,200,100000000,8}},
			{{1490,-50,220,210,10},{100,100,275,300,5},{100,100,235,300,6},{100,100,310,160,6},{100,100,200,100000000,8}},
			{{1470,-100,225,180,8},{100,100,280,250,7},{100,100,225,100,7},{100,100,222,1000060,6},{100,145,340,100000000,8}},
			{{1460,-50,230,180,7},{100,100,265,250,6},{100,100,210,100,6},{100,100,200,60,6},{100,45,200,100000000,8}},
			{{1450,-50,225,220,9},{100,100,270,300,8},{100,100,225,100,7},{100,100,315,60,6},{100,100,300,100000000,8}},

            //左上角出来的鱼
		    {{-210,-50,135,200,9},{100,100,90,300,8},{100,100,60,100,7},{100,100,30,60,6},{100,100,20,100000000,8}},
			{{-220,-50,135,210,9},{100,100,90,300,5},{100,100,130,100,7},{100,100,150,60,6},{100,100,175,100000000,8}},
			{{-203,-50,130,200,10},{100,100,80,300,5},{100,100,100,300,6},{100,100,160,160,6},{100,100,120,100000000,8}},
			{{-240,-100,130,180,8},{100,100,85,250,6},{100,100,100,100,7},{100,100,160,1000060,6},{100,145,45,100000000,8}},
			{{-230,-50,130,180,7},{100,100,120,250,6},{100,100,100,100,7},{100,100,90,60,6},{100,45,45,100000000,8}},

	         //左下角出来的鱼
	        {{-210,790,45,200,9},{100,100,90,300,5},{100,100,30,100,7},{100,100,20,60,6},{100,100,5,100000000,8}},
		    {{-220,790,50,150,10},{100,100,100,300,5},{100,100,80,100,6},{100,100,60,160,6},{100,100,20,100000000,8}},
            {{-230,800,40,180,8},{100,100,85,250,6},{100,100,100,100,7},{100,100,135,160,6},{100,45,45,100000000,8}},
			{{-240,810,40,190,8},{100,100,85,250,6},{100,100,100,100,7},{100,100,90,60,6},{100,45,45,100000000,8}},
			{{-250,790,45,200,9},{100,100,90,300,5},{100,100,145,100,7},{100,100,60,60,6},{100,100,160,100000000,8}},
			//
			{{-250,790,45,100,10},{100,100,60,100,13},{100,100,90,100,14},{100,100,120,60,15},{100,100,160,100000000,16}},
			{{1450,100,260,260,11},{100,100,265,300,12},{100,100,250,100,13},{100,100,240,60,14},{100,100,230,100000000,15}},
			{{1490,680,285,80,14},{100,100,275,50,13},{100,100,290,100,12},{100,100,300,60,11},{100,245,310,100000000,13}},
			{{890,-200,200,100,13},{100,100,260,60,14},{100,100,220,50,15},{100,100,190,60,14},{100,45,185,100000000,13}},
			{{850,900,350,50,12},{100,100,330,80,13},{100,100,320,50,14},{100,100,340,60,15},{100,45,310,100000000,18}}

	        };


#define abcdd  22

GameScreenWnd::GameScreenWnd(void)
{
	//初始数据
	ZeroMemory(m_HaveUser,sizeof(m_HaveUser));
	//ZeroMemory(m_UserPaoJiaodu,sizeof(m_UserPaoJiaodu));
	ZeroMemory(m_UserSendstate,sizeof(m_UserSendstate));
	ZeroMemory(m_UserScore,sizeof(m_UserScore));
	ZeroMemory(m_MuchmoneyAct,sizeof(m_MuchmoneyAct));
	ZeroMemory(m_IsMouseOver,sizeof(m_IsMouseOver));
	ZeroMemory(m_BeiLv,sizeof(m_BeiLv));
	ZeroMemory(m_UserScore,sizeof(m_UserScore));
	ZeroMemory(m_UserSuperPao,sizeof(m_UserSuperPao));
	ZeroMemory(m_SuoPao,sizeof(m_SuoPao));
	ZeroMemory(m_NumArr,sizeof(m_NumArr));
	ZeroMemory(m_SuoFishId,sizeof(m_SuoFishId));
	ZeroMemory(m_FontCurr,sizeof(m_FontCurr));
	ZeroMemory(m_FireStruc,sizeof(m_FireStruc));
	ZeroMemory(m_ChouJiang,sizeof(m_ChouJiang));
	ZeroMemory(m_PowerCount,sizeof(m_PowerCount));
	ZeroMemory(m_IsAndroid,sizeof(m_IsAndroid));
	
	Definex_bli = 1;
	Definey_bli = 1;
	m_IsFullScreen = false;
	m_IsIntro = false;
	m_bLockFish=false;
	bgmusicindex = 0;
	m_SuoDingFishIndex = 0;
	m_SuoDingFishid = -1;
	m_doudongstu.m_have = false;
	m_doudongstu.m_doucount = 0;
	m_doudongstu.m_height = 0;
	m_doudongstu.m_height = 0;
    m_isxiuyuqi =  false;
	m_scienceindex = 0;
	m_ScienceZhuan = false;
	m_DingStruc.m_have = false;
	//已经载入数量
	m_HaveLoadNum = 0;
	m_InitScreen= false;
	m_Bgindex = 0;
	m_ActionIndex = 0;
	m_ActionIndex1 = 0;
	m_SoundIndex =0;
	m_IsMustOut = false;
	m_IsDingzhu = false;
	//获取当前系统启动时间
	m_NowTime =  GetTickCount();
	//是否切换场景
	m_IsChangeScreen  =  false;
	m_bLockShoot=false;
    
	//测试数据
	for(int i=0;i<6;i++)
	{
	   m_IsAndroid[i]=true;
	   m_UserPaoJiaodu[i]=0;
	   if(i>=3)m_UserPaoJiaodu[i]=180;
       m_UserScore[i]=0;
	   m_BeiLv[i] =  0;
	   for(int j=0;j<3;j++)
	   {
		   m_UserStruct[i][j].m_Have = false;
		   m_UserStruct[i][j].m_Money = 1000;
		   m_UserStruct[i][j].m_Time = 100;

	   }
	}
   
	//玩家坐标
	m_UserPT[0].x  =  80;
	m_UserPT[0].y  =  534;
	m_UserPT[1].x  =  455;
	m_UserPT[1].y  =  534;
	m_UserPT[2].x  =  830;
	m_UserPT[2].y  =  534;
	m_UserPT[5].x  =  80;
	m_UserPT[5].y  =  -31;
	m_UserPT[4].x  =  455;
	m_UserPT[4].y  =  -31;
	m_UserPT[3].x  =  830;
	m_UserPT[3].y  =  -31;

   //拷贝坐标
	CopyMemory(m_UserPaoPT,m_UserPT,sizeof(m_UserPaoPT));

	//自己座位好,经过切换的


	m_MeChairID =2;
	m_Username = L"哈皮科技";
	m_MyScore = 56248;
	m_OutTime = 120;
	m_SendTimeN = GetTickCount();
	//初始数据
	for(int i=0;i<100;i++)
	{
	    m_MeBullet[i].m_Have=false;
		m_OterBullet[i].m_Have=false;
		//m_NumArr[i].m_Have = false;
		m_SoundArr[i].m_Have = false;


	}
	
	//测试数据
   m_HaveLoadNum=0;
   
	SetGameTitle(L"哈皮科技：精品游戏！F1:上分；F2:加炮；F3:减炮；F4:下分；F5:自动发炮；S:锁定；Q:取消锁定 ");
}

GameScreenWnd::~GameScreenWnd(void)
{

}
//渲染线程
void DxThreadRender(LPVOID pThreadData)
{
	//效验参数
	ASSERT(pThreadData!=NULL);
	if(pThreadData==NULL) return;

	//类型转换
	GameScreenWnd *pDxDialog=(GameScreenWnd *)pThreadData;

	//降低cpu使用率
    while(NULL!=pDxDialog && pDxDialog->GetSafeHwnd()!=NULL&&pDxDialog->m_RunRender)// && pDxDialog->m_bD3DSucess)
	{
		//SendMessage(pDxDialog->GetSafeHwnd(), WM_DX_RENDER, 0, 0) ;
		pDxDialog->Render();
		Sleep(10);
	}

	return;
}
//渲染线程
void DxThreadRender1(LPVOID pThreadData)
{
	//效验参数
	ASSERT(pThreadData!=NULL);
	if(pThreadData==NULL) return;

	//类型转换
	GameScreenWnd *pDxDialog=(GameScreenWnd *)pThreadData;

	//降低cpu使用率
	while(NULL!=pDxDialog && pDxDialog->GetSafeHwnd()!=NULL&&pDxDialog->m_RunRender)// && pDxDialog->m_bD3DSucess)
	{
		Sleep(1);
		pDxDialog->CheckHit();
		Sleep(10);
		
	}

	return;
}
//载入线程
void DxThreadLoad(LPVOID pThreadData)
{
	//效验参数
	ASSERT(pThreadData!=NULL);
	if(pThreadData==NULL) return;

	//类型转换
	GameScreenWnd *pDxDialog=(GameScreenWnd *)pThreadData;

	//降低cpu使用率
	 pDxDialog->LoadImg();

	return;
}
//载入资源
void GameScreenWnd::LoadImg()
{
   /*****开始加载资源******/	

	//载入背景资源
	CString bgImgPath;

	//载入背景
	char strPath[50] = {0};
	for(int i=0;i<3;i++)
	{
		
		CString strpath;
		strpath.Format(L"MAIN%d.png",i);
		m_HaveLoadNum=m_HaveLoadNum+1;
		LoadNorImg1(&m_bgImgTexture[i],strpath);
		
		
	}
	//加载边框
    LoadNorImg1(&m_BorderImgTexture,L"Border.png");
	m_HaveLoadNum=m_HaveLoadNum+1;
	//加载炮底座
	
	LoadNorImg1(&m_PaoConImgTexture,L"PaoTai.png");


	  //加载炮底座
	LoadNorImg1(&m_PaoBgImgTexture,L"PaoBG.png");
	m_HaveLoadNum=m_HaveLoadNum+1;

	

	LoadNorImg1(&m_GunFireTextTure,L"GunFire.png");
	m_HaveLoadNum=m_HaveLoadNum+3;

	LoadNorImg1(&m_BlackWateTure,L"BlackWater.png");

	LoadNorImg1(&m_IntroImgTexture,L"intro.png");
	//
	LoadNorImg1(&m_LockFishTexture,L"LockFIsh.png");
	//
    for(int i=0;i<6;i++)
	{
	   m_HaveLoadNum=m_HaveLoadNum+3;
	   bgImgPath.Format(L"shotNumBack%d.png",i);
	   LoadNorImg1(&m_allscoreBgImgTexture[i],bgImgPath);
	}
	//数字图片
	for(int i=0;i<5;i++)
	{
		m_HaveLoadNum=m_HaveLoadNum+3;
	    bgImgPath.Format(L"goldNum%d.png",i);
		LoadNorImg1(&m_NumImgTexture[i],bgImgPath);
	}
	//超级状态图
	m_HaveLoadNum=m_HaveLoadNum+3;
    LoadNorImg1(&m_SupStatTexture, L"supPao.png");
	m_HaveLoadNum=m_HaveLoadNum+3;
   
    LoadNorImg1(&m_CoinUp, L"6660530.png");

	//CString strpath;
	//strpath.Format(L"Pao_%d.png",i);
	LoadNorImg1(&m_PaoTwoImgTexture,L"Pao_0_0.png");
   for(int i=0;i<3;i++)
	{
		CString strpath;
		m_HaveLoadNum=m_HaveLoadNum+3;
		//加载蛇图
		strpath.Format(L"snake%d.png",i);
		LoadNorImg1(&m_SankeTure[i],strpath);
      
	}
	for(int i=0;i<3;i++)
	{
		m_HaveLoadNum=m_HaveLoadNum+3;
		CString strpath;
		strpath.Format(L"shot_%d.png",i);
		LoadNorImg1(&m_BulltTwoTexture[i],strpath);
      
	}
	for(int i=0;i<3;i++)
	{
		m_HaveLoadNum=m_HaveLoadNum+3;
		CString strpath;
		strpath.Format(L"net_%d.png",i);
		LoadNorImg1(&m_PaoNetImgTexture[i],strpath);
      
	}
	
	m_HaveLoadNum=m_HaveLoadNum+3;
    LoadNorImg1(&m_OutTexture,L"GAME_TIP.png");
	for(int i=0;i<2;i++)
	{
		m_HaveLoadNum=m_HaveLoadNum+3;
		CString strpath;
		strpath.Format(L"gold_%d.png",i);
		LoadNorImg1(&m_GoldImgTexture[i],strpath);
      
	}
    //钱堆数字背景
	for(int i=0;i<3;i++)
	{
		m_HaveLoadNum=m_HaveLoadNum+3;
		CString strpath;
		strpath.Format(L"SCbg%d.png",i);
		LoadNorImg1(&m_MuchMoneyTexture[i],strpath);
      
	}

	m_HaveLoadNum=m_HaveLoadNum+3;
	LoadNorImg1(&m_BiTexture,L"qianbi.png");

	LoadNorImg1(&PowerFrameTure,L"PowerFrame.png");
	LoadNorImg1(&PowerBarTure,L"PowerBar.png");
    LoadNorImg1(&FontBgTure,L"fontbg.png");


	//波浪图
	//浪潮图
	for(int i=0;i<2;i++)
	{
		m_HaveLoadNum=m_HaveLoadNum+3;
	    bgImgPath.Format(L"WaterWave%d.png",i);
		LoadNorImg1(&m_WaterWaveTexture[i],bgImgPath);
	}
	//超级子弹图
	for(int i=0;i<3;i++)
	{
		m_HaveLoadNum=m_HaveLoadNum+3;
	    bgImgPath.Format(L"SupButtle_%d.png",i);
		LoadNorImg1(&m_SupButTexture[i],bgImgPath);
	}
    //水面波浪
	for(int i=0;i<32;i++)
	{
	  m_HaveLoadNum=m_HaveLoadNum+3;
	  bgImgPath.Format(L"Water%d.png",i);
	  LoadNorImg1(&m_WaterTexture[i],bgImgPath);
	}

	//园环图
	for(int i=0;i<4;i++)
	{
		m_HaveLoadNum=m_HaveLoadNum+3;
	    bgImgPath.Format(L"quan%d.png",i);
		LoadNorImg1(&m_QuanTexture[i],bgImgPath);
	}
	//载入鱼游动图
   for(int i=0;i<FISHCOUNT;i++)
	{
			CString strpath;
			strpath.Format(L"FISH%d.png",i);
            LoadNorImg1(&m_FishMoveTexture[i],strpath);
			m_HaveLoadNum=m_HaveLoadNum+10;
		
	}

		
   for(int i=0;i<GAME_PLAYER;i++)
	{
	   
			CString strpath;
			strpath.Format(L"lock_flag_%d.png",i);
			m_HaveLoadNum=m_HaveLoadNum+1;
			LoadNorImg1(&m_SuoBall[i],strpath);
		    
		
	}
	
	
	
	//子弹和鱼检测
	SetTimer(TIMERIDBULLTE,1,NULL);

	//鱼儿游动图序列
	SetTimer(TIMERIDFISHMOVE,100,NULL);
	//
	SetTimer(13,60,NULL);
	//子弹撒网动画
	SetTimer(TIMERIDNETOPEN,20,NULL);
	
	//
	
	//掉钱动画
	SetTimer(TIMERIDMONEYACTION,100,NULL);
	//
	SetTimer(TIMERIDUSERMONEY,100,NULL);
	//
	//
	SetTimer(TIMERIDOUT,1000,NULL);
	//
	SetTimer(CREATEFISH,1,NULL);
	
    //载入声音
    LoadSound();

    m_InitScreen = true;
	
	 SetTimer(100,100,NULL);
	/*****加载资源结束******/	

}
//初始窗口
bool GameScreenWnd::InitDxDlog()
{
    m_hWnd=this->GetSafeHwnd();

	if(m_hWnd==NULL)AfxMessageBox(L"创建失败!");

	if( SUCCEEDED( InitD3D( m_hWnd ) ) )
    {
	   if( SUCCEEDED( TRUE ) )
        {
			 m_CursorInt = ShowCursor(TRUE);
			 m_RunRender = true;
			 m_hDxThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DxThreadRender,this,0,0);	
			 m_hWorkThread1=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DxThreadRender1,this,0,0);	
			
			 SetTimer(TIMERIDFISHROUND,1,NULL);
			 m_hWorkThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DxThreadLoad,this,0,0);
			 //LoadImg();
		}
	}  
	//按扭区域
	m_BtRect[0].top = 0;m_BtRect[0].left = CLIENT_WIDTH-215;m_BtRect[0].right = m_BtRect[0].left+ 52;m_BtRect[0].bottom = m_BtRect[0].top+28;
	m_BtRect[6].top = 0;m_BtRect[6].left = CLIENT_WIDTH-163;m_BtRect[6].right = m_BtRect[6].left+ 52;m_BtRect[6].bottom = m_BtRect[6].top+28;
	m_BtRect[2].top = 0;m_BtRect[2].left = CLIENT_WIDTH-112;m_BtRect[2].right = m_BtRect[2].left+ 52;m_BtRect[2].bottom = m_BtRect[2].top+28;
	m_BtRect[1].top = 0;m_BtRect[1].left = CLIENT_WIDTH-60;m_BtRect[1].right = m_BtRect[1].left+ 52;m_BtRect[1].bottom = m_BtRect[1].top+28;
	m_BtRect[3].top = 687;m_BtRect[3].left = 575;m_BtRect[3].right = m_BtRect[3].left+ 52;m_BtRect[3].bottom = m_BtRect[3].top+28;
	m_BtRect[4].top = 687;m_BtRect[4].left = 625;m_BtRect[4].right = m_BtRect[4].left+ 52;m_BtRect[4].bottom = m_BtRect[4].top+28;
	m_BtRect[5].top = 687;m_BtRect[5].left = 690;m_BtRect[5].right = m_BtRect[5].left+ 90;m_BtRect[5].bottom = m_BtRect[5].top+30;

	m_BtRect[7].top = 645;m_BtRect[7].left = 115;m_BtRect[7].right = m_BtRect[7].left+ 46;m_BtRect[7].bottom = m_BtRect[7].top+38;
	m_BtRect[8].top = 645;m_BtRect[8].left = 190;m_BtRect[8].right = m_BtRect[8].left+ 46;m_BtRect[8].bottom = m_BtRect[8].top+38;
	
   SetTimer(101,50,NULL);

    return true;
}

//创建DX窗口及载入资源纹理图片
HRESULT GameScreenWnd::InitD3D( HWND hWnd )
{
	//测试版本号,然后根据版本号再来判断是用什么模式
	OSVERSIONINFO os; 
	os.dwOSVersionInfoSize=sizeof(os);  
	GetVersionEx(&os); 
	//设置显示参数//如果于6就表示如果是WIN7以下的版本就用这个参数否则用下面的
    if(os.dwMajorVersion<6)
	{
		
		//返回结果
      	HRESULT hr;
        //取得DX版本,显示模式
        m_pD3D=Direct3DCreate9(D3D_SDK_VERSION);
        ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
        hr=m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_d3ddm);
        if(FAILED(hr)) 
	       {
		      MessageBox(TEXT("获得显示适配器模式失败!"),TEXT("错误提示"),MB_OK);
		      return false;
	       }
	    m_d3dpp.Windowed=true;										//是否窗口模式显示
        m_d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;					//交换缓冲类型(直接翻屏)
	    m_d3dpp.BackBufferFormat=m_d3ddm.Format;					//后缓冲区模式
	    m_d3dpp.hDeviceWindow=hWnd;								    //窗口句柄
    	m_d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;	//显示器刷新率
	    m_d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;	//图像最大刷新速度
	    m_d3dpp.AutoDepthStencilFormat=D3DFMT_D16;					//深度缓冲格式
    	m_d3dpp.EnableAutoDepthStencil=TRUE;						//激活深度缓冲

        //兼容检测
        D3DCAPS9 d3dCaps;
        if (FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps))) return false;
        DWORD dwBehaviorFlags=0;
        if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) dwBehaviorFlags=D3DCREATE_HARDWARE_VERTEXPROCESSING;
        else dwBehaviorFlags=D3DCREATE_SOFTWARE_VERTEXPROCESSING;

        //创建设备
        hr=m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,dwBehaviorFlags, &m_d3dpp, &m_pd3dDevice);
        if (FAILED(hr))
       {
	       if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, dwBehaviorFlags,&m_d3dpp, &m_pd3dDevice))) return false;
       }
	}
	else
	{ 
		if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
             return E_FAIL;

        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory( &d3dpp, sizeof(d3dpp) );
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        //创建Direct3D设备对象
        if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &m_pd3dDevice )))
            {
	           AfxMessageBox(L"设备对象失败!");
                return E_FAIL;
             }
	}
	// 创建点精灵
	if (FAILED(D3DXCreateSprite( m_pd3dDevice, &m_pSprite )))
	{
		AfxMessageBox(L"创建点精灵失败!");
		return E_FAIL;
	}
	//设置文字
	m_Dxfont = NULL;
    D3DXFONT_DESCW lf;
    ZeroMemory(&lf, sizeof(D3DXFONT_DESCW));
    lf.Height = 22; //字体高度
    lf.Width = 0; // 字体宽度
    lf.Weight = 400; 
    lf.Italic = false;
	_sntprintf(lf.FaceName,sizeof(lf.FaceName),TEXT("微软雅黑"));
    D3D_OK!=D3DXCreateFontIndirect(m_pd3dDevice, &lf, &m_Dxfont); //创建字体对象
	//
	m_DxfontUserName = NULL;
    ZeroMemory(&lf, sizeof(D3DXFONT_DESCW));
    lf.Height = 20; //字体高度
    lf.Width = 0; // 字体宽度
    lf.Weight = 400; 
    lf.Italic = false;
	_sntprintf(lf.FaceName,sizeof(lf.FaceName),TEXT("微软雅黑"));
    D3DXCreateFontIndirect(m_pd3dDevice, &lf, &m_DxfontUserName); //创建字体对象
	//设置文字
	m_DxfontA = NULL;
    D3DXFONT_DESCW lAf;
    ZeroMemory(&lAf, sizeof(D3DXFONT_DESCW));
    lAf.Height = 40; //字体高度
    lAf.Width = 0; // 字体宽度
    lAf.Weight = 400; 
    lAf.Italic = false;
	_sntprintf(lAf.FaceName,sizeof(lAf.FaceName),TEXT("微软雅黑"));
    D3D_OK!=D3DXCreateFontIndirect(m_pd3dDevice, &lAf, &m_DxfontA); //创建字体对象

    //按扭图
	for(int i=0;i<9;i++)
	{

		CString bgImgPath;
	    bgImgPath.Format(L"BT%d.png",i);
		LoadNorImg1(&m_BtTexture[i],bgImgPath);
		
	}
	LoadNorImg1(&m_LoadTexture,L"jindu.png");
	LoadNorImg1(&m_LoadBgTexture,L"bg.png");
	
	//初始化设置窗口
	//
	m_configDlg.InitDlg(m_pd3dDevice,370,200);

	
    device = new MP_Device_WRAP(0,0,hWnd);
	device->set1(m_pD3D,m_pd3dDevice);
	device->Create();
	
	MP_Manager& MP=MP_Manager::GetInstance();

	#ifdef MAGIC_3D
	int axis=MAGIC_pXpYpZ;
	#else
	int axis=MAGIC_pXpY;
	#endif
	MP_Platform* platform=new MP_Platform_WIN_POSIX;
	MP.Initialization(axis, platform, MAGIC_INTERPOLATION_ENABLE, MAGIC_NOLOOP, MAGIC_CHANGE_EMITTER_DEFAULT, 1366, 768, 1, 1.f, 0.1f, true);

	// eng: find of all ptc-files in folder
	// rus: поиск всех ptc-файлов в папке
	MP.LoadAllEmitters();
	
	MP.RefreshAtlas();

	MP.CloseFiles();

	MP.Stop();

   // MP.Destroy();
    //device.Destroy();
	return S_OK;

}
//渲染消息
LRESULT GameScreenWnd::OnFrameRender(WPARAM wParam, LPARAM lParam)
{
	if(m_pd3dDevice==NULL) return 0;
	
	Render();

	return 0 ;
}
//渲染界面
void GameScreenWnd::Render()
{
	if(m_pd3dDevice==NULL){return;}
	//清理绘制设备
    m_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(100,100,100),1.0f,0);
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	//准备绘制
    m_pd3dDevice->BeginScene() ;
	//设置精灵模式
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	//还载加载界面
	if(m_InitScreen==false)
	{
		m_pSprite->Draw(m_LoadBgTexture, NULL, NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
		 //绘制进度
		if(m_HaveLoadNum>548)m_HaveLoadNum = 548;
        CRect rcClient;
	    rcClient.top= 0;
	    rcClient.left=0;
		rcClient.right =  m_HaveLoadNum/2;
		rcClient.bottom = 9;
		m_pSprite->Draw(m_LoadTexture, rcClient, NULL, &D3DXVECTOR3(449, 634, 0), D3DCOLOR_ARGB(255,255,255,255));
		//绘制按扭
	    DrawBt();
		//清理缓存
        m_pSprite->Flush();
	    //绘制结束,停止精灵
	    m_pSprite->End();
	    //停止设备
        m_pd3dDevice->EndScene();
	    //更换设备到最前
        HRESULT hr = m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	    if(FAILED(hr))
          {
        
		   HRESULT hr = m_pd3dDevice->TestCooperativeLevel() ;
		   if (hr == D3DERR_DEVICENOTRESET)
            {
              //处理丢失
			  m_pSprite -> OnLostDevice(); 
              m_Dxfont -> OnLostDevice();
			  if (FAILED(m_pd3dDevice -> Reset( & m_d3dpp)))
              {
				    //AfxMessageBox("3");
                    return ;
               }
			  m_pSprite -> OnResetDevice();
              m_Dxfont -> OnResetDevice();

		     }
          }
	    return;
	   
	}

	//绘制背景
	//如果在切换场景，显示浪潮图
	int m_DoudongJUli=0;
	if(m_IsChangeScreen)
	{
		int pRreBgindex = 0;
        if(m_Bgindex==0)pRreBgindex=2;
		else if(m_Bgindex==1)pRreBgindex=0;
		else if(m_Bgindex==2)pRreBgindex=1;

		CRect rcClient;
	    rcClient.top=20;
		rcClient.left=50;
		rcClient.right = rcClient.left +  CLIENT_WIDTH;
		rcClient.bottom =  rcClient.top +  CLIENT_HEIGHT;
        m_pSprite->Draw(m_bgImgTexture[pRreBgindex], rcClient, NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
      
	    rcClient.top=20;
	    rcClient.left=CLIENT_WIDTH-(CLIENT_WIDTH-m_ChanwaveSt.m_ptx-100)+50;
		if( rcClient.left<50) rcClient.left = 50;
		rcClient.right =rcClient.left +  CLIENT_WIDTH;
		rcClient.bottom =  rcClient.top  +  CLIENT_HEIGHT;
		int bgx = m_ChanwaveSt.m_ptx+100;
		if(bgx<0)bgx = 0;
		
		m_pSprite->Draw(m_bgImgTexture[m_Bgindex], rcClient, NULL, &D3DXVECTOR3(bgx, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
         //绘制波纹
		m_pSprite->Draw(m_WaterWaveTexture[m_ChanwaveSt.m_currImgIndex], NULL, NULL, &D3DXVECTOR3(m_ChanwaveSt.m_ptx, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
		 
	     
	}
	else
	{
		//抖动图
		if(m_doudongstu.m_have)
		{

		   int a = m_ActionIndex1%10;
		   if(a==0)
		   { 
			   m_DoudongJUli = 10;
		   }
		   else if(a==1)
		   {
			   m_DoudongJUli = 20;
		   }
		   else if(a==2)
		   {
			   m_DoudongJUli = 30;
		   }
		   else if(a==3)
		   {
			  m_DoudongJUli = 40;
		   }
		    else if(a==4)
		   {
			   m_DoudongJUli =50;
		   }
		    else if(a==5)
		   {
			   m_DoudongJUli = 40;
		   }
		    else if(a==6)
		   {
			   m_DoudongJUli = 30;
		   }
		     else if(a==7)
		   {
			   m_DoudongJUli = 20;
		   }
		     else if(a==8)
		   {
			   m_DoudongJUli = 10;
		   }
		     else if(a==9)
		   {
			  m_DoudongJUli = 0;
		   }
		  
		}
		{
		   CRect rcClient;
	       rcClient.top=20;
		   rcClient.left=50-m_DoudongJUli;
		   rcClient.right = rcClient.left +  CLIENT_WIDTH;
		   rcClient.bottom =  rcClient.top +  CLIENT_HEIGHT;
		   m_pSprite->Draw(m_bgImgTexture[m_Bgindex], rcClient, NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
		}
	}

	D3DXMATRIX mat;
	D3DXMATRIX mat1;
	D3DXMATRIX mat2;
	//绘制鱼儿
	//try
	{
     	INT_PTR nEnumIndex=0;
	    Fish *tfish=NULL;
       do
	   {
		 tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		  //枚举鱼群
		if (NULL==tfish) break;
		if(!tfish->m_Have)
		{
		  m_FishFactory.FreeItem(tfish);continue;

		}
		if(GetTickCount()-tfish->m_creatTime>80000)
		{
		  m_FishFactory.FreeItem(tfish);continue;

		}
		if(!tfish->m_Have)continue;
        int  fishindex = tfish->m_index;

		int pianyijiaodu = 0;
		pianyijiaodu=-90;

		if(fishindex>19)
		{
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(m_FishScale22,m_FishScale22),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*m_FishScale22/2,m_FishmoveRec[fishindex][1]*m_FishScale22/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-m_DoudongJUli,tfish->m_pty));
		  D3DXMatrixTransformation2D(&mat1,NULL,0.0f,&D3DXVECTOR2(m_FishScale22,m_FishScale22),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*m_FishScale22/2,m_FishmoveRec[fishindex][1]*m_FishScale22/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-10-m_DoudongJUli,tfish->m_pty-10));
		}
		else
		{
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(m_FishScale22,m_FishScale22),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*m_FishScale22/2,m_FishmoveRec[fishindex][1]*m_FishScale22/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx,tfish->m_pty));
		  D3DXMatrixTransformation2D(&mat1,NULL,0.0f,&D3DXVECTOR2(m_FishScale22,m_FishScale22),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*m_FishScale22/2,m_FishmoveRec[fishindex][1]*m_FishScale22/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-10-m_DoudongJUli,tfish->m_pty-10));

		}

		if(fishindex==19)
		{
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(0.6,0.6),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*0.6/2,m_FishmoveRec[fishindex][1]*0.6/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-m_DoudongJUli,tfish->m_pty));
		  D3DXMatrixTransformation2D(&mat1,NULL,0.0f,&D3DXVECTOR2(0.6,0.6),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*0.6/2,m_FishmoveRec[fishindex][1]*0.6/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-10-m_DoudongJUli,tfish->m_pty-10));
		}
		if(fishindex==25||fishindex==26||fishindex==27||fishindex==20)
		{
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0,1.0),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*1.0/2,m_FishmoveRec[fishindex][1]*1.0/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-m_DoudongJUli,tfish->m_pty));
		  D3DXMatrixTransformation2D(&mat1,NULL,0.0f,&D3DXVECTOR2(1.0,1.0),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*1.0/2,m_FishmoveRec[fishindex][1]*1.0/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-10-m_DoudongJUli,tfish->m_pty-10));
		}

		if(fishindex==22)
		{
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0,1.0),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*1.0/2,m_FishmoveRec[fishindex][1]*1.0/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-m_DoudongJUli,tfish->m_pty));
		  D3DXMatrixTransformation2D(&mat1,NULL,0.0f,&D3DXVECTOR2(1.0,1.0),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*1.0/2,m_FishmoveRec[fishindex][1]*1.0/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-10-m_DoudongJUli,tfish->m_pty-10));
		}

		if(fishindex<17)
		{
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(m_FishScale,m_FishScale),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*m_FishScale/2,m_FishmoveRec[fishindex][1]*m_FishScale/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-m_DoudongJUli,tfish->m_pty));
		  D3DXMatrixTransformation2D(&mat1,NULL,0.0f,&D3DXVECTOR2(m_FishScale,m_FishScale),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*m_FishScale/2,m_FishmoveRec[fishindex][1]*m_FishScale/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-10-m_DoudongJUli,tfish->m_pty-10));
		}
		else
		{
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(m_FishScale22,m_FishScale22),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*m_FishScale22/2,m_FishmoveRec[fishindex][1]*m_FishScale22/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx,tfish->m_pty));
		  D3DXMatrixTransformation2D(&mat1,NULL,0.0f,&D3DXVECTOR2(m_FishScale22,m_FishScale22),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]*m_FishScale22/2,m_FishmoveRec[fishindex][1]*m_FishScale22/2),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-10-m_DoudongJUli,tfish->m_pty-10));

		}

		if(tfish->m_isHit)
		  D3DXMatrixTransformation2D(&mat2,NULL,0.0f,&D3DXVECTOR2(m_FishScale,m_FishScale),&D3DXVECTOR2(256*m_FishScale/2,256*m_FishScale/2),m_ActionIndex1%360*10*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx+m_FishmoveRec[fishindex][0]*m_FishScale/2-256*m_FishScale/2-m_DoudongJUli,tfish->m_pty+m_FishmoveRec[fishindex][1]*m_FishScale/2-256*m_FishScale/2));
		else
		  D3DXMatrixTransformation2D(&mat2,NULL,0.0f,&D3DXVECTOR2(m_FishScale,m_FishScale),&D3DXVECTOR2(256*m_FishScale/2,256*m_FishScale/2),m_ActionIndex1%360*2*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx+m_FishmoveRec[fishindex][0]*m_FishScale/2-256*m_FishScale/2-m_DoudongJUli,tfish->m_pty+m_FishmoveRec[fishindex][1]*m_FishScale/2-256*m_FishScale/2));



		

        m_pSprite->Flush();
		//如果有原换
		if(tfish->m_smallFish!=-1)
		{
			//如果是蛇头
			if(tfish->m_smallFish==10)
			{
				D3DXMatrixTransformation2D(&mat2,NULL,0.0f,&D3DXVECTOR2(1.0,1.0),&D3DXVECTOR2(197/2,148/2),((tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180)),&D3DXVECTOR2(tfish->m_ptx+m_FishmoveRec[fishindex][0]*m_FishScale22/2-197/2-m_DoudongJUli,tfish->m_pty+m_FishmoveRec[fishindex][1]*m_FishScale22/2-148/2));
				m_pSprite->SetTransform(&mat2);
				CRect rcClient;
				rcClient.left=0; 
		        rcClient.top= 0; 
				rcClient.right = rcClient.left+591/3;
	            rcClient.bottom = rcClient.top+740/5;
			    m_pSprite->Draw(m_SankeTure[0],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			}
			else if(tfish->m_smallFish==11)
			{
				D3DXMatrixTransformation2D(&mat2,NULL,0.0f,&D3DXVECTOR2(1.0,1.0),&D3DXVECTOR2(1056/3/2,740/5/2),((tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180)),&D3DXVECTOR2(tfish->m_ptx+m_FishmoveRec[fishindex][0]*m_FishScale22/2-1056/3/2-m_DoudongJUli,tfish->m_pty+m_FishmoveRec[fishindex][1]*m_FishScale22/2-740/5/2));
				int colcount = m_ActionIndex1%20;
				m_pSprite->SetTransform(&mat2);
				CRect rcClient;
				rcClient.left=(colcount%4)*1056/4; 
		        rcClient.top=colcount/4*740/5; 
				rcClient.right = rcClient.left+1056/4;
	            rcClient.bottom = rcClient.top+740/5;
			    m_pSprite->Draw(m_SankeTure[1],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			}
			else if(tfish->m_smallFish==12)
			{
				D3DXMatrixTransformation2D(&mat2,NULL,0.0f,&D3DXVECTOR2(1.0,1.0),&D3DXVECTOR2(374/2/2,740/5/2),((tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180)),&D3DXVECTOR2(tfish->m_ptx+m_FishmoveRec[fishindex][0]*m_FishScale22/2-374/2/2-m_DoudongJUli,tfish->m_pty+m_FishmoveRec[fishindex][1]*m_FishScale22/2-740/5/2));
				int colcount = m_ActionIndex%10;
				m_pSprite->SetTransform(&mat2);
				CRect rcClient;
				rcClient.left=(colcount%2)*374/2; 
		        rcClient.top=colcount/2*740/5; 
				rcClient.right = rcClient.left+374/2;
	            rcClient.bottom = rcClient.top+740/5;
			    m_pSprite->Draw(m_SankeTure[2],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			}
			else
			{
				m_pSprite->SetTransform(&mat2);
		        m_pSprite->Draw(m_QuanTexture[tfish->m_smallFish],NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			}
		}

      
		CRect rcClient;
    	//if(tfish->m_isHit==false||m_FishdeadCount[fishindex]==0)
		{
		 
	       rcClient.top=0;
	       rcClient.left=tfish->m_actionindex*m_FishmoveRec[fishindex][0]; 
		    if(fishindex==0||fishindex==1||fishindex==2||fishindex==4||fishindex==8||fishindex==10||fishindex==11||fishindex==12||fishindex==15||fishindex==16||fishindex==17||fishindex==18||fishindex==14||fishindex==24)
		   {
		      rcClient.left=tfish->m_actionindex%4*m_FishmoveRec[fishindex][0]; 
		      rcClient.top= tfish->m_actionindex/4*m_FishmoveRec[fishindex][1]; 
		   }
		    if(fishindex==3||fishindex==5||fishindex==7||fishindex==22||fishindex==31)
		   {
		      rcClient.left=tfish->m_actionindex%3*m_FishmoveRec[fishindex][0]; 
		      rcClient.top= tfish->m_actionindex/3*m_FishmoveRec[fishindex][1]; 
		   }
		    if(fishindex==6||fishindex==14)
		   {
		      rcClient.left=tfish->m_actionindex%6*m_FishmoveRec[fishindex][0]; 
		      rcClient.top= tfish->m_actionindex/6*m_FishmoveRec[fishindex][1]; 
		   }
		    if(fishindex==9||fishindex==13||fishindex==19||fishindex==20||fishindex==21)
		   {
		      rcClient.left=tfish->m_actionindex%5*m_FishmoveRec[fishindex][0]; 
		      rcClient.top= tfish->m_actionindex/5*m_FishmoveRec[fishindex][1]; 
		   }
		    if(fishindex==23||fishindex==25||fishindex==26||fishindex==27||fishindex==28||fishindex==29||fishindex==30)
		   {
		      rcClient.left=tfish->m_actionindex%2*m_FishmoveRec[fishindex][0]; 
		      rcClient.top= tfish->m_actionindex/2*m_FishmoveRec[fishindex][1]; 
		   }
           rcClient.right = rcClient.left+m_FishmoveRec[fishindex][0];
	       rcClient.bottom = rcClient.top+m_FishmoveRec[fishindex][1];
          m_pSprite->SetTransform(&mat1);
          m_pSprite->Draw(m_FishMoveTexture[fishindex],rcClient, NULL, NULL, D3DCOLOR_ARGB(80,0,0,0));

		 
		  m_pSprite->SetTransform(&mat);
	      m_pSprite->Draw(m_FishMoveTexture[fishindex],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

		  if(fishindex==25||fishindex==26||fishindex==27)
		  {
			  int m_tempindex[3]={2,5,2};
		      int m_tempindexcol[3]={4,3,4};
			  if(fishindex==26)
			  {
				  m_tempindex[0]=4;
				  m_tempindex[1]=9;
				  m_tempindex[2]=4;
		          m_tempindexcol[0]=4;
				  m_tempindexcol[1]=5;
				  m_tempindexcol[2]=4;
			  }
			  if(fishindex==27)
			  {
                  m_tempindex[0]=3;
				  m_tempindex[1]=7;
				  m_tempindex[2]=3;
		          m_tempindexcol[0]=3;
				  m_tempindexcol[1]=3;
				  m_tempindexcol[2]=3;
			  }
		      int m_tempjuli[3]={100,200,300};
			  int pianyijiaodu = 0;
		      pianyijiaodu=-90;
	           //
	          //旋转的中心点
	          CPoint Center;
	          //中心点就是这个鱼X坐标+这条鱼的宽度的一半
	          Center.x= tfish->m_ptx + m_FishmoveRec[fishindex][0]*1.0 /2;
	          Center.y= tfish->m_pty + m_FishmoveRec[fishindex][1]*1.0 /2;
              for(int i=0;i<3;i++)
			  {
			  int m_tempaction = m_ActionIndex%m_FishmoveCount[m_tempindex[i]];
			  CRect rcClient1;
			  rcClient1.left=m_tempaction%m_tempindexcol[i]*m_FishmoveRec[m_tempindex[i]][0]; 
		      rcClient1.top= m_tempaction/m_tempindexcol[i]*m_FishmoveRec[m_tempindex[i]][1]; 
			  rcClient1.right = rcClient1.left+m_FishmoveRec[m_tempindex[i]][0];
	          rcClient1.bottom = rcClient1.top+m_FishmoveRec[m_tempindex[i]][1];

			  //即将要旋转的点
	          CPoint Center1;
	          Center1.x= tfish->m_ptx+m_FishmoveRec[fishindex][0]*1.0-m_DoudongJUli-m_tempjuli[i]*1.0-m_FishmoveRec[m_tempindex[i]][0]/2*0.6;
	          Center1.y= tfish->m_pty+m_FishmoveRec[fishindex][1]*1.0-90*1.0-m_FishmoveRec[m_tempindex[i]][1]/2*0.6;
			  CPoint  mpointtop=drawing(Center,Center1,tfish->m_Roation+pianyijiaodu);
			  D3DXMatrixTransformation2D(&mat2,NULL,0.0f,&D3DXVECTOR2(0.6,0.6),&D3DXVECTOR2(0,0),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(mpointtop.x,mpointtop.y));
			  m_pSprite->SetTransform(&mat2);
	          m_pSprite->Draw(m_FishMoveTexture[m_tempindex[i]],rcClient1, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			  }
		  }
		  //
		  if(fishindex==28||fishindex==29||fishindex==30)
		  {
			  int m_tempindex[4]={7,9,9,7};
		      int m_tempindexcol[4]={3,5,5,3};
			  if(fishindex==29)
			  {
				  m_tempindex[0]=6;
				  m_tempindex[1]=6;
				  m_tempindex[2]=6;
				  m_tempindex[3]=6;
		          m_tempindexcol[0]=6;
				  m_tempindexcol[1]=6;
				  m_tempindexcol[2]=6;
				  m_tempindexcol[3]=6;
			  }
			  if(fishindex==30)
			  {
                  m_tempindex[0]=10;
				  m_tempindex[1]=10;
				  m_tempindex[2]=10;
				  m_tempindex[3]=10;
		          m_tempindexcol[0]=4;
				  m_tempindexcol[1]=4;
				  m_tempindexcol[2]=4;
				  m_tempindexcol[3]=4;
			  }
		      int m_tempjuli[4]={120,205,205,300};
			  int m_tempjuli1[4]={0,-40,30,0};
			  int pianyijiaodu = 0;
		      pianyijiaodu=-90;
	           //
	          //旋转的中心点
	          CPoint Center;
	          //中心点就是这个鱼X坐标+这条鱼的宽度的一半
	          Center.x= tfish->m_ptx + m_FishmoveRec[fishindex][0]*m_FishScale22 /2;
	          Center.y= tfish->m_pty + m_FishmoveRec[fishindex][1]*m_FishScale22 /2;
              for(int i=0;i<4;i++)
			  {
			  int m_tempaction = m_ActionIndex%m_FishmoveCount[m_tempindex[i]];
			  CRect rcClient1;
			  rcClient1.left=m_tempaction%m_tempindexcol[i]*m_FishmoveRec[m_tempindex[i]][0]; 
		      rcClient1.top= m_tempaction/m_tempindexcol[i]*m_FishmoveRec[m_tempindex[i]][1]; 
			  rcClient1.right = rcClient1.left+m_FishmoveRec[m_tempindex[i]][0];
	          rcClient1.bottom = rcClient1.top+m_FishmoveRec[m_tempindex[i]][1];

			  //即将要旋转的点
	          CPoint Center1;
	          Center1.x= tfish->m_ptx+m_FishmoveRec[fishindex][0]*m_FishScale22-m_DoudongJUli-m_tempjuli[i]*m_FishScale22-m_FishmoveRec[m_tempindex[i]][0]/2*m_FishScale22;
	          Center1.y= tfish->m_pty+m_FishmoveRec[fishindex][1]*m_FishScale22-90*m_FishScale22-m_FishmoveRec[m_tempindex[i]][1]/2*m_FishScale22+m_tempjuli1[i];
			  CPoint  mpointtop=drawing(Center,Center1,tfish->m_Roation+pianyijiaodu);
			  D3DXMatrixTransformation2D(&mat2,NULL,0.0f,&D3DXVECTOR2(m_FishScale22,m_FishScale22),&D3DXVECTOR2(0,0),(tfish->m_Roation+pianyijiaodu)*(D3DX_PI/180),&D3DXVECTOR2(mpointtop.x,mpointtop.y));
			  m_pSprite->SetTransform(&mat2);
	          m_pSprite->Draw(m_FishMoveTexture[m_tempindex[i]],rcClient1, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			  }
		  }

		}
		
		//如果是李逵
		if(fishindex==22)
		{
		int nCellCount = 0;
		BYTE byCell[10];
		LONG lCellScore = tfish->m_LiKuiScore;
		if(lCellScore<=0)continue;

		if(lCellScore>0)
		  {
			  for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
               {          
                  byCell[i]=lCellScore%10;//从个位开始存每位数
			      nCellCount++;
               }
			}
		int temppointx = 0;
		int m_jiaodu = 0;
		//如果是对面那么就要转换180度

		int  m_xPianyi = nCellCount*15;
		int  m_xWidht = 54;
		for( int j = nCellCount-1; j >=0; j-- )
		{
			CRect rcClient;
	        rcClient.top=0;
		    rcClient.left=(byCell[j])*m_xWidht;
		    rcClient.bottom= rcClient.top + m_xWidht ;
	        rcClient.right=  rcClient.left +m_xWidht;
			int now_x = tfish->m_ptx+temppointx-m_xPianyi+m_FishmoveRec[fishindex][0]*m_FishScale/2;
			int now_y = tfish->m_pty+m_FishmoveRec[fishindex][1]*m_FishScale/2;

			
			    D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(0.8f,0.8f),&D3DXVECTOR2(53.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(now_x-m_DoudongJUli,now_y));
		        m_pSprite->SetTransform(&mat);
			    m_pSprite->Draw(m_NumImgTexture[4], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
 			    temppointx = temppointx + 30;
		}

		}
		 
		  
	   }while(true);
	}

	//绘制自己子弹
	for(int i=0;i<200;i++)
	{
		if(!m_MeBullet[i].m_Have)continue;
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(0.8f,0.8f),&D3DXVECTOR2(32,31.0f),m_MeBullet[i].m_Roation*(D3DX_PI/180),&D3DXVECTOR2(m_MeBullet[i].m_ptx-m_DoudongJUli,m_MeBullet[i].m_pty));
	    m_pSprite->Flush();
		m_pSprite->SetTransform(&mat);
	    CRect rcClient;
		if(!m_MeBullet[i].m_Stop)
		{
		  //如果超级炮发色
		  if(m_MeBullet[i].m_issuper)
		  {
			rcClient.top=0;
	        rcClient.left=(m_ActionIndex%2)*64;
	        rcClient.right = rcClient.left+128/2;
	        rcClient.bottom = rcClient.top+64;
		    m_pSprite->Draw(m_SupButTexture[m_MeBullet[i].m_isThree],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
		    
		  }
		  else
		  {
	        rcClient.top=m_MeBullet[i].m_Senduser*384/6;
	        rcClient.left=(m_ActionIndex%2)*64;
	        rcClient.right = rcClient.left+128/2;
	        rcClient.bottom = rcClient.top+384/6;
		    m_pSprite->Draw(m_BulltTwoTexture[m_MeBullet[i].m_isThree],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
		  }
		 
		}
		else
		{

		 
		  int mWidth   = 250;
		  int mHeight  =  250;
          int  now_x = m_MeBullet[i].m_ptx - mWidth/2+10 ;
		  int  now_y = m_MeBullet[i].m_pty - mHeight/2 ;
		 

          D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.1f,1.1f),&D3DXVECTOR2(mWidth/2,mHeight/2),m_MeBullet[i].m_Roation*(D3DX_PI/180),&D3DXVECTOR2(now_x-m_DoudongJUli,now_y));
	      m_pSprite->SetTransform(&mat);
		  rcClient.left=mWidth*(m_MeBullet[i].m_Netindex%5);
	      rcClient.top=mHeight*(m_MeBullet[i].m_Netindex/5);

		  if(m_MeBullet[i].m_Netindex>9)
		  {
		      rcClient.left=mWidth*4;
	          rcClient.top=mHeight*1;
		  }

	      rcClient.right = rcClient.left+mWidth;
	      rcClient.bottom = rcClient.top+mHeight;

		  int m_Alph = 255;
		  if(m_MeBullet[i].m_Netindex>20)
			  m_Alph = m_Alph - (m_MeBullet[i].m_Netindex - 20 ) * 20;

		  if(m_MeBullet[i].m_issuper)
			  m_pSprite->Draw(m_PaoNetImgTexture[m_MeBullet[i].m_isThree],rcClient, NULL, NULL, D3DCOLOR_ARGB(m_Alph,230,140,0));
		  else
			 m_pSprite->Draw(m_PaoNetImgTexture[m_MeBullet[i].m_isThree],rcClient, NULL, NULL, D3DCOLOR_ARGB(m_Alph,255,255,255));

		}
		
		
        
	}
	
	//绘制水面波浪，需要平铺15张图
	int BowenX = 0;
	int BowenY = 0;
	//还原角度为正常

	int a = m_doudongstu.m_doucount%2;
	int b = 0;
	if(m_doudongstu.m_have)
		{
         if(a==0)
	        b=-15;
	     else if(a==1)
		    b=0;
		}

    for(int i=0;i<45;i++)
	{   
        BowenX = i * 300*0.5+b;
		BowenY = 0;
		if(i>8){BowenY=300*0.5;BowenX = i%9 * 300*0.5+b;}
		if(i>17){BowenY=600*0.5;BowenX = i%9 * 300*0.5+b;}
		if(i>26){BowenY=900*0.5;BowenX = i%9 * 300*0.5+b;}
		if(i>35){BowenY=1200*0.5;BowenX = i%9 * 300*0.5+b;}
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(0.5f,0.5f),NULL,NULL,&D3DXVECTOR2(BowenX-m_DoudongJUli,BowenY));
	    m_pSprite->SetTransform(&mat);
        m_pSprite->Draw(m_WaterTexture[m_ActionIndex%32], NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
	
	}
	D3DXMATRIX matRot;
	D3DXMatrixIdentity(&matRot);//矩阵
	m_pSprite->SetTransform(&matRot);
	//绘制捕获中的鱼儿的分数
	for(int i=0;i<50;i++)
	 {
		if(!m_NumArr[i].m_Have)continue;
		int nCellCount = 0;
		BYTE byCell[10];
		LONG lCellScore = m_NumArr[i].m_Numcount;
		if(lCellScore<=0)continue;

		if(lCellScore>0)
		  {
			  for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
               {          
                  byCell[i]=lCellScore%10;//从个位开始存每位数
			      nCellCount++;
               }
			}
		int temppointx = 0;
		int m_jiaodu = 0;
		//如果是对面那么就要转换180度
		if(m_NumArr[i].m_HitUser>=3){m_jiaodu=180;}
		int  m_xPianyi = nCellCount*15*m_NumArr[i].m_beishu/2;
		int  m_xWidht = 54;
		if(m_NumArr[i].m_beishu>1.5)m_xWidht = 54;
		for( int j = nCellCount-1; j >=0; j-- )
		{
			CRect rcClient;
	        rcClient.top=0;
		    rcClient.left=(byCell[j])*m_xWidht;
		    rcClient.bottom= rcClient.top + m_xWidht ;
	        rcClient.right=  rcClient.left +m_xWidht;
			int now_x = m_NumArr[i].m_ptx+temppointx-m_xPianyi;
			int now_y = m_NumArr[i].m_pty;

			if(m_NumArr[i].m_Time<20)
			{
				if(m_NumArr[i].m_HitUser>=3)
			       now_y=now_y+(m_NumArr[i].m_Time*4);
				else
				  now_y=now_y-(m_NumArr[i].m_Time*4);
			}
			else if(m_NumArr[i].m_Time>=20&&m_NumArr[i].m_Time<30)
			{
				if(m_NumArr[i].m_HitUser>=3)
			       now_y=now_y+80-((m_NumArr[i].m_Time-20)*4);
				else
				   now_y=now_y-80+((m_NumArr[i].m_Time-20)*4);
			}
			else
			{
				if(m_NumArr[i].m_HitUser>=3)
			       now_y=now_y+40;
				else
				   now_y=now_y-40;

			}
			int m_juli = 30;
			if(m_NumArr[i].m_beishu>1.5)
			{
			   D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.1f),&D3DXVECTOR2(27.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(now_x,now_y));
			   m_juli = m_juli * 1.22;
			}
			else
			{
			   D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(0.8f,0.9f),&D3DXVECTOR2(27.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(now_x,now_y));
		       m_juli = m_juli * 0.9;
			}
			m_pSprite->SetTransform(&mat);
            //if(m_NumArr[i].m_beishu>1.5)
			    m_pSprite->Draw(m_NumImgTexture[4], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			//else
				//m_pSprite->Draw(m_NumImgTexture[2], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	

			

			if(m_NumArr[i].m_HitUser>=3)
			   temppointx = temppointx - m_juli;
			else
			   temppointx = temppointx + m_juli;
		}
		
	}
	 //定住
	 if(m_DingStruc.m_have)
	 {
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(1024/2,512/2),0*(D3DX_PI/180),&D3DXVECTOR2(m_DingStruc.m_ptx,m_DingStruc.m_pty));
	    m_pSprite->Flush();
		m_pSprite->SetTransform(&mat);
		//如果于22就撒钱
		if(m_DingStruc.m_nowindex<10)
		  {
			m_pSprite->Draw(m_DingTexture[m_DingStruc.m_nowindex],NULL, NULL, NULL, D3DCOLOR_ARGB(255,200,200,255));
		  }
	 }
	  //掉钱动画
    for(int i=0;i<6;i++)
	 {
		if(!m_MuchmoneyAct[i].m_have)continue;
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(1024/2,512/2),0*(D3DX_PI/180),&D3DXVECTOR2(m_MuchmoneyAct[i].m_ptx,m_MuchmoneyAct[i].m_pty));
	    m_pSprite->Flush();
		m_pSprite->SetTransform(&mat);
		//如果于22就撒钱
		if(m_MuchmoneyAct[i].m_nowindex<15)
		  {
			 //m_pSprite->Draw(m_MuchMoneyTexture[m_MuchmoneyAct[i].m_nowindex],NULL, NULL, NULL, D3DCOLOR_ARGB(255,200,200,255));
		  }
		//否则就开始转换了
		else 
		  {
			  int ptx = m_UserPT[i].x-60;
			  int pty = m_UserPT[i].y-240;
			  if(i>=3)pty = pty +380;
			  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(310/2,310/2),m_MuchmoneyAct[i].m_Roation*(D3DX_PI/180),&D3DXVECTOR2(ptx,pty));
	          m_pSprite->SetTransform(&mat);
			  m_pSprite->Draw(m_QuanTexture[2],NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			  //绘制环上数字
			  int nCellCount = 0;
	     	  BYTE byCell[10];
			  LONG lCellScore = m_MuchmoneyAct[i].m_DiuMoney;
		      if(lCellScore>0)
		           {
			           for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
                           {          
                             byCell[i]=lCellScore%10;//从个位开始存每位数
			                  nCellCount++;
                            }
			       }
		     else
			    {
			       nCellCount =1;
			       byCell[0]=0;
			    }
	    	int temppointx = 0;
			 ptx =  ptx  + 310/2-(nCellCount*39)/2;
			 if(i<3)
			 {
	    	  for( int j = nCellCount-1; j >=0; j-- )
		        {
			      CRect rcClient;
	              rcClient.top=0;
		          rcClient.left=(byCell[j])*54;
		          rcClient.bottom= rcClient.top + 54 ;
	              rcClient.right=  rcClient.left +54;
			      D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(27.5f,12.5f),0*(D3DX_PI/180),&D3DXVECTOR2(ptx+temppointx,pty+130));
		          m_pSprite->SetTransform(&mat);
			      m_pSprite->Draw(m_NumImgTexture[4], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			      temppointx = temppointx + 37;
		         }
			 }
			 else
			 {
			    for( int j = 0; j <nCellCount; j++ )
		        {
			      CRect rcClient;
	              rcClient.top=0;
		          rcClient.left=(byCell[j])*54;
		          rcClient.bottom= rcClient.top + 54 ;
	              rcClient.right=  rcClient.left +54;
			      D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(27.5f,12.5f),180*(D3DX_PI/180),&D3DXVECTOR2(ptx+temppointx,pty+150));
		          m_pSprite->SetTransform(&mat);
			      m_pSprite->Draw(m_NumImgTexture[4], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			      temppointx = temppointx + 37;
		         }
			 }

		  }
	}



	   //掉钱动画2
    for(int i=0;i<6;i++)
	 {
		if(!m_zhaaction[i].m_have)continue;
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(1024/2,512/2),0*(D3DX_PI/180),&D3DXVECTOR2(m_zhaaction[i].m_ptx,m_zhaaction[i].m_pty));
	    m_pSprite->Flush();
		m_pSprite->SetTransform(&mat);
		//如果于22就撒钱
		if(m_zhaaction[i].m_nowindex<15)
		  {
			  //if(m_zhaaction[i].m_nowindex<10)
			      //m_pSprite->Draw(m_ZhaTexture[m_zhaaction[i].m_nowindex],NULL, NULL, NULL, D3DCOLOR_ARGB(255,200,200,255));
		  }
		//否则就开始转换了
		else 
		  {
			  int ptx = m_UserPT[i].x-60;
			  int pty = m_UserPT[i].y-240;
			  if(i>=3)pty = pty +380;
			  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(310/2,310/2),m_zhaaction[i].m_Roation*(D3DX_PI/180),&D3DXVECTOR2(ptx,pty));
	          m_pSprite->SetTransform(&mat);
			  m_pSprite->Draw(m_QuanTexture[2],NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			  //绘制环上数字
			  int nCellCount = 0;
	     	  BYTE byCell[10];
			  LONG lCellScore = m_zhaaction[i].m_DiuMoney;
		      if(lCellScore>0)
		           {
			           for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
                           {          
                             byCell[i]=lCellScore%10;//从个位开始存每位数
			                  nCellCount++;
                            }
			       }
		     else
			    {
			       nCellCount =1;
			       byCell[0]=0;
			    }
	    	 int temppointx = 0;
			 ptx =  ptx  + 310/2-(nCellCount*39)/2;
			 if(i<3)
			 {
	    	  for( int j = nCellCount-1; j >=0; j-- )
		        {
			      CRect rcClient;
	              rcClient.top=0;
		          rcClient.left=(byCell[j])*54;
		          rcClient.bottom= rcClient.top + 54 ;
	              rcClient.right=  rcClient.left +54;
			      D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(27.5f,12.5f),0*(D3DX_PI/180),&D3DXVECTOR2(ptx+temppointx,pty+130));
		          m_pSprite->SetTransform(&mat);
			      m_pSprite->Draw(m_NumImgTexture[4], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			      temppointx = temppointx + 37;
		         }
			 }
			 else
			 {
			    for( int j = 0; j <nCellCount; j++ )
		        {
			      CRect rcClient;
	              rcClient.top=0;
		          rcClient.left=(byCell[j])*54;
		          rcClient.bottom= rcClient.top + 54 ;
	              rcClient.right=  rcClient.left +54;
			      D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(27.5f,12.5f),180*(D3DX_PI/180),&D3DXVECTOR2(ptx+temppointx,pty+150));
		          m_pSprite->SetTransform(&mat);
			      m_pSprite->Draw(m_NumImgTexture[4], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			      temppointx = temppointx + 37;
		         }
			 }

		  }
	}
     //玩家抽奖图
     for(int i=0;i<6;i++)
	 {
		 if(m_ChouJiang[i].ishave!=false)
		 {
		 
			 D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(m_ChouJiang[i].roloation,m_ChouJiang[i].roloation),&D3DXVECTOR2(350,350),0,&D3DXVECTOR2(m_ChouJiang[i].quanshu-m_DoudongJUli,m_ChouJiang[i].nowindex));
	      m_pSprite->SetTransform(&mat);

		  m_pSprite->Draw(m_BlackWateTure,NULL, NULL, NULL, D3DCOLOR_ARGB(int(255-m_ChouJiang[i].index),255,255,255));

		 
		 }

	 }
    //
	m_pSprite->SetTransform(&matRot);
	for(int pid=0;pid<USERCOUNT;pid++)
	{  
		//绘制炮台
		if(!m_HaveUser[pid])continue;
        for(int i=0;i<10;i++)
	    {
		  if(m_SuoPao[pid][i].m_have==false)continue;
		   //
		   if(i!=9)
		      m_pSprite->Draw(m_QuanTexture[3], NULL, NULL,  &D3DXVECTOR3(m_SuoPao[pid][i].m_ptx, m_SuoPao[pid][i].m_pty, 0), D3DCOLOR_ARGB(255,255,255,255));
		   if(i==9)
			  m_pSprite->Draw(m_SuoBall[m_RealChairID[pid]], NULL, NULL,  &D3DXVECTOR3(m_SuoPao[pid][i].m_ptx, m_SuoPao[pid][i].m_pty, 0), D3DCOLOR_ARGB(255,255,255,255));
	     } 
	}
	 //绘制玩家
	for(int i=0;i<USERCOUNT;i++)
	{  
		//绘制炮台
		if(!m_HaveUser[i])continue;
		int m_jiaodu = 0;
		int m_userpoajiaodu = int(m_UserPaoJiaodu[i]);
		int m_bgjuli = 60;
		int m_fenjuli  = 100;
		//如果是对面那么就要把炮转换180度
		if(i>=3){m_jiaodu=180;m_bgjuli =  40;m_fenjuli=77;}
		
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(128.0f,128.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x-28-m_DoudongJUli,m_UserPT[i].y-28));
		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(m_PaoConImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
		//炮台
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(128.0f,128.0f),m_userpoajiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPaoPT[i].x-28-m_DoudongJUli,m_UserPaoPT[i].y-28));
		m_pSprite->SetTransform(&mat);
		//炮

		int  mPaoIndex = 0;
		switch (m_CellScore)
		{
		  case 10:
			{
			   if(m_BeiLv[i]<=20)mPaoIndex = 0;
		       else if(m_BeiLv[i]>20&&m_BeiLv[i]<=50)mPaoIndex = 1;
		       else mPaoIndex = 2;
			   break;

			}
		  case 100:
			{
			    if(m_BeiLv[i]<=200)mPaoIndex = 0;
		        else if(m_BeiLv[i]>200&&m_BeiLv[i]<=500)mPaoIndex = 1;
		        else mPaoIndex = 2;
			    break;
			}
		  case 1000:
			{
			    if(m_BeiLv[i]<=2000)mPaoIndex = 0;
		        else if(m_BeiLv[i]>2000&&m_BeiLv[i]<=5000)mPaoIndex = 1;
		        else mPaoIndex = 2;
			    break;
			}
		}

		
		     CRect rcClient;
	         rcClient.top=0;
		     rcClient.left=mPaoIndex*120;
		    
			 D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(60.0f,150.0f),m_userpoajiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPaoPT[i].x+37-m_DoudongJUli,m_UserPaoPT[i].y-50));
		        m_pSprite->SetTransform(&mat);
		 if(m_UserSuperPao[i])
			{
				rcClient.left=(mPaoIndex+3)*120;
		       
			}
		  rcClient.bottom= rcClient.top + 200 ;
	      rcClient.right=  rcClient.left +120;
          m_pSprite->Draw(m_PaoTwoImgTexture, rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

          rcClient.top=140;
		  rcClient.left=(m_UserSendstate[i]-1)*120;
		  rcClient.bottom= rcClient.top + 140 ;
	      rcClient.right=  rcClient.left +120;
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(60.0f,150.0f),m_userpoajiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPaoPT[i].x+37-m_DoudongJUli,m_UserPT[i].y-50));
		  m_pSprite->SetTransform(&mat);
		  if(m_UserSendstate[i]!=0)
		  m_pSprite->Draw(m_GunFireTextTure, rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

		if(m_UserSuperPao[i])
		{
			if(i>=3)
			  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(44.0f,62.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+200-m_DoudongJUli,m_UserPT[i].y+80));
			else
			  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(44.0f,62.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+230-m_DoudongJUli,m_UserPT[i].y));

	        m_pSprite->SetTransform(&mat);
			m_pSprite->Draw(m_SupStatTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
		}

		//倍率背景
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(128.0f,128.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x-28-m_DoudongJUli,m_UserPT[i].y-28));
		m_pSprite->SetTransform(&mat);
		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(m_PaoBgImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
        //玩家倍率
		{
			int nCellCount = 0;
		    BYTE byCell[10];
		    LONG lCellScore = m_BeiLv[i];

		    if(lCellScore>0)
		      {
			      for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
                    {          
                       byCell[i]=lCellScore%10;//从个位开始存每位数
			            nCellCount++;
                    }
			   }
	     	else
			   {
			      nCellCount =1;
			      byCell[0]=0;
			   }
	   	   int temppointx = 0;
		   int ajuli  = 10;
		   if(i>=3)ajuli = -6;

		   for( int j = nCellCount-1; j >=0; j-- )
		      {
			    int Thisx = m_UserPT[i].x + 100 - 16 * nCellCount/2 + temppointx-10/2;
				if(i>=3)Thisx = m_UserPT[i].x + 100 +16 * nCellCount/2 + temppointx-18/2;
			    CRect rcClient;
	            rcClient.top=0;
		        rcClient.left=(byCell[j])*24;
		        rcClient.bottom= rcClient.top + 23 ;
	            rcClient.right=  rcClient.left +24;
			    D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(8.0f,8.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(Thisx-m_DoudongJUli,m_UserPT[i].y+m_fenjuli+ajuli));
		        m_pSprite->SetTransform(&mat);
			    m_pSprite->Draw(m_NumImgTexture[2], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			    if(i>=3)
			       temppointx = temppointx - 16;
			    else
			       temppointx = temppointx + 16;
		     }
		   
		}

		//绘制LOCKFISH
		if(m_SuoFishId[i]!=0)
		{
			Fish *tfish=NULL;
			INT_PTR  nEnumIndex = 0;
            do
	           {
		          tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		          //枚举鱼群
				  if (NULL==tfish){break;}
				  if(tfish->m_Have==false)continue;
				  if(tfish->m_isHit==true)continue;
				  if(tfish->m_fishID == m_SuoFishId[i] )
				  {
				     break;
				  }
						
					    
			}while(true);

		    if(tfish!=NULL)
			{
			  if(tfish->m_index>=0&&tfish->m_index<100)
			    {
				  
			      CRect rcClient;
	              rcClient.top=0;
			      rcClient.left=tfish->m_index*67;
		          rcClient.bottom= rcClient.top + 91 ;
	              rcClient.right=  rcClient.left +67;
				  if(i<3)
			         D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(34.0f,45.0f),0*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+160-m_DoudongJUli,m_UserPT[i].y-1));
				  else
					 D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(34.0f,45.0f),180*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+160-m_DoudongJUli,m_UserPT[i].y+114));

		          m_pSprite->SetTransform(&mat);
			      m_pSprite->Draw(m_LockFishTexture, rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			    }
			}
		    
		}
		//绘制分数背景
        if(i>=3)
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(102.0f,51.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+135-m_DoudongJUli,m_UserPT[i].y+m_bgjuli));
		else
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(102.0f,51.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+110-m_DoudongJUli,m_UserPT[i].y+m_bgjuli));
		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(m_allscoreBgImgTexture[i], NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

        //绘制能量背景
		if(i>=3)
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(102.0f,51.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+100-m_DoudongJUli,m_UserPT[i].y+m_bgjuli-5));
		else
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(102.0f,51.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+140-m_DoudongJUli,m_UserPT[i].y+m_bgjuli+2));
		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(PowerFrameTure, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

		
		
		if(i>=3)
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(m_PowerCount[i],1.0f),&D3DXVECTOR2(102.0f,51.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+100-m_DoudongJUli-54,m_UserPT[i].y+m_bgjuli-5-11));
		else
		  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(m_PowerCount[i],1.0f),&D3DXVECTOR2(102.0f,51.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+140-m_DoudongJUli+54,m_UserPT[i].y+m_bgjuli+2+11));
		m_pSprite->SetTransform(&mat);
		if(m_PowerCount[i]<0.95f)
		   m_pSprite->Draw(PowerBarTure, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
		else if(m_ActionIndex%2==0&&m_PowerCount[i]>=0.95f)
			m_pSprite->Draw(PowerBarTure, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
		
		//玩家上分
		int nCellCount = 0;
		BYTE byCell[10];
		LONG lCellScore = m_UserScore[i];
		if(lCellScore>0)
		  {
			  for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
               {          
                  byCell[i]=lCellScore%10;//从个位开始存每位数
			      nCellCount++;
               }
			}
		else
			{
			   nCellCount =1;
			   byCell[0]=0;
			}
		int temppointx = 0;
		for( int j = nCellCount-1; j >=0; j-- )
		{
			CRect rcClient;
	        rcClient.top=0;
		    rcClient.left=(byCell[j])*16;
		    rcClient.bottom= rcClient.top + 23 ;
	        rcClient.right=  rcClient.left +16;
            if(i>=3)
			   D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(53.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+180+temppointx,m_UserPT[i].y+m_fenjuli));
			else
			  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(53.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+165+temppointx,m_UserPT[i].y+m_fenjuli));
		    m_pSprite->SetTransform(&mat);
			m_pSprite->Draw(m_NumImgTexture[0], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			if(i>=3)
			   temppointx = temppointx - 16;
			else
			   temppointx = temppointx + 16;
		}
		//当前中心点

		D3DXMATRIX matRot;
	    D3DXMatrixIdentity(&matRot);//矩阵
	    m_pSprite->SetTransform(&matRot);
		//玩家钱堆
		for(int j=0;j<3;j++)
			{
	            m_pSprite->SetTransform(&matRot);
				if(!m_UserStruct[i][j].m_Have)continue;
				int m_count = m_UserStruct[i][j].m_Nowscore;
				//设置堆着的币最多50个 要不是就冲到对
				if(m_count>50)m_count = 50;
				int m_henx = j * 30;
				int m_heny = 0-m_count * 4 - 20;
				int m_a=0;
				int m_b=0;
				for(int n=0;n<m_count;n++)
				{
					int m_ydx = 140;
					int m_hen = 2;
					if(i>=3)
					{
						m_hen = 0;
						m_hen = m_hen + n*4;
						m_ydx = 54;
					}
					else 
					{
						m_hen = m_hen - n*4;
					}
                    m_a = m_hen;
					m_b = m_ydx;
					m_pSprite->Draw(m_BiTexture, NULL, NULL, &D3DXVECTOR3(m_UserPT[i].x-m_henx+20-m_DoudongJUli, m_UserPT[i].y +m_ydx+m_hen , 0), D3DCOLOR_ARGB(255,255,255,255));	
				    
				}
				if(m_count<50&&m_count<m_UserStruct[i][j].m_FishScore)
				{
                CRect rcClient;
	            rcClient.top=0;
		        rcClient.left=26*(m_ActionIndex1%10);
		        rcClient.bottom= rcClient.top + 26 ;
	            rcClient.right=  rcClient.left +26;
				if(i>=3)
				   m_pSprite->Draw(m_CoinUp, rcClient, NULL, &D3DXVECTOR3(m_UserPT[i].x-m_henx+20-m_DoudongJUli, m_UserPT[i].y +m_b+m_a , 0), D3DCOLOR_ARGB(255,255,255,255));
				else
				   m_pSprite->Draw(m_CoinUp, rcClient, NULL, &D3DXVECTOR3(m_UserPT[i].x-m_henx+20-m_DoudongJUli, m_UserPT[i].y +m_b+m_a-20 , 0), D3DCOLOR_ARGB(255,255,255,255));

				}
				 //玩家上分
		         int nCellCount = 0;
		         BYTE byCell[10];
		         LONG lCellScore =m_UserStruct[i][j].m_Money;
		         if(lCellScore>0)
		            {
			           for(int n = 0;lCellScore>0;lCellScore=lCellScore/10,n++)
                          {          
                             byCell[n]=lCellScore%10;//从个位开始存每位数
			                 nCellCount++;
                           }
			         }
		        int temppointx = 0;
				if(i>=3)m_heny = -m_heny-148;
		        for( int n = nCellCount-1; n >=0; n-- )
		           {
			          CRect rcClient;
	                  rcClient.top=0;
		              rcClient.left=(byCell[n])*12;
		              rcClient.bottom= rcClient.top + 24 ;
	                  rcClient.right=  rcClient.left +12;
					  int m_ydx = 30;
					  int m_ydy = 140;
					  if(i>=3)
					   {
						  m_ydx = 1;
						  m_ydy = 176;
					   }
					  else 
					   {
						   m_ydx = -30;
						   m_ydy = 148;
					   }
					  if(i>=3)
					  {
						  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(6.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x-m_henx +10/2*nCellCount+temppointx+20-m_DoudongJUli,m_UserPT[i].y +m_ydy+m_heny));
						  if(m_count>=50||m_count==m_UserStruct[i][j].m_FishScore)
					       {
							    m_pSprite->SetTransform(&mat);
						        m_pSprite->Draw(m_MuchMoneyTexture[m_UserStruct[i][j].m_colorindex], NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255)); 
					       }
					      D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(6.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x-m_henx +10/2*nCellCount+temppointx+20-m_DoudongJUli,m_UserPT[i].y +m_ydy+m_heny+3));
                           
                          temppointx = temppointx - 10;
					 
					  }
					  else
					  {
						  D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(6.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x-m_henx + 12 -10/2*nCellCount+temppointx+20-m_DoudongJUli,m_UserPT[i].y +m_ydy+m_heny));
						  if(m_count>=50||m_count==m_UserStruct[i][j].m_FishScore)
					       {
							    m_pSprite->SetTransform(&mat);
						        m_pSprite->Draw(m_MuchMoneyTexture[m_UserStruct[i][j].m_colorindex], NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255)); 
					       }
					      D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(6.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x-m_henx + 12 -10/2*nCellCount+temppointx+20-m_DoudongJUli,m_UserPT[i].y +m_ydy+m_heny-3));
		                  temppointx = temppointx + 10;
					  }
					  m_pSprite->SetTransform(&mat);
					  if(m_count>=50||m_count==m_UserStruct[i][j].m_FishScore)
					  {
						 
			              m_pSprite->Draw(m_NumImgTexture[3], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
					  }
		          }		

			}

	}

	//绘制飘落金币
    for(int i=0;i<100;i++)
	 {
		if(!m_GoldArr[i].m_Have)continue;
		if(m_GoldArr[i].m_goldtype==0)
		    D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(0.85f,0.85f),&D3DXVECTOR2(45,62.0f),0*(D3DX_PI/180),&D3DXVECTOR2(m_GoldArr[i].m_ptx,m_GoldArr[i].m_pty));
		else
			D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(0.8f,0.8f),&D3DXVECTOR2(72,62.0f),0*(D3DX_PI/180),&D3DXVECTOR2(m_GoldArr[i].m_ptx,m_GoldArr[i].m_pty));
	    m_pSprite->Flush();
		m_pSprite->SetTransform(&mat);
	    CRect rcClient;
	    rcClient.top=0;
		if(m_GoldArr[i].m_goldtype==1)
		{
		  rcClient.left=1080/12*m_GoldArr[i].m_actionindex;
          rcClient.right = rcClient.left+1080/12;
	      rcClient.bottom = rcClient.top+90;
		}
		else
		{
		  rcClient.left=936/12*m_GoldArr[i].m_actionindex;
          rcClient.right = rcClient.left+936/12;
	      rcClient.bottom = rcClient.top+78;
		}
		m_pSprite->Draw(m_GoldImgTexture[m_GoldArr[i].m_goldtype],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

		 
	}

    D3DXMatrixIdentity(&matRot);//矩阵
	m_pSprite->SetTransform(&matRot);
	if(m_IsMustOut)
	{
	   m_pSprite->Draw(m_OutTexture, NULL, NULL, &D3DXVECTOR3(110, 300, 0), D3DCOLOR_ARGB(255,255,255,255));
	   int nCellCount = 0;
		BYTE byCell[10];
		LONG lCellScore = m_OutTime;
		if(lCellScore>0)
		  {
			  for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
               {          
                  byCell[i]=lCellScore%10;//从个位开始存每位数
			      nCellCount++;
               }
			}
		else
			{
			   nCellCount =1;
			   byCell[0]=0;
			}
		int temppointx = 0;
		for( int j = nCellCount-1; j >=0; j-- )
		{
			CRect rcClient;
	        rcClient.top=0;
		    rcClient.left=(byCell[j])*16;
		    rcClient.bottom= rcClient.top + 23 ;
	        rcClient.right=  rcClient.left +16;
			D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(53.0f,12.0f),NULL,&D3DXVECTOR2(CLIENT_HEIGHT-45+temppointx,320));
		    m_pSprite->SetTransform(&mat);
			m_pSprite->Draw(m_NumImgTexture[0], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
		    temppointx = temppointx + 16;
		}
	}

	 for(int i=0;i<10;i++)
		  {
			  if(m_FireStruc[i].m_Have==true)
			  {
				for(int j=0;j<40;j++)
				{
					
					CRect rcClient;
					rcClient.top= (m_FireStruc[i].m_index%9)*140;
		            rcClient.left=m_FireStruc[i].m_index/9;
		            rcClient.bottom= rcClient.top + 140 ;
	                rcClient.right=  rcClient.left +140;
					D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(m_FireStruc[i].m_scale,m_FireStruc[i].m_scale),&D3DXVECTOR2(70,70),m_FireStruc[i].smfire[j].m_rontion*(D3DX_PI/180),&D3DXVECTOR2(m_FireStruc[i].smfire[j].m_ptx,m_FireStruc[i].smfire[j].m_pty));
		            m_pSprite->SetTransform(&mat);
			        m_pSprite->Draw(m_FireAllTexture, rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	

				 }
			  }
		  }

	    //还原角度为正常
	m_pSprite->SetTransform(&matRot);
	//
	for(int i=0;i<5;i++)
	{
	   int m_rgb[10][3]={{255,255,100},{255,0,255},{255,255,255},{0,255,255},{255,0,0},{150,150,150},{255,255,0},{100,255,0},{255,0,100},{255,100,0}};
	   if(m_FontCurr[i].m_Have)
	   {
		   CRect rcClient;
		   //rcClient.top=(520+i*20)-m_FontCurr[i].m_Curr*2;
		   rcClient.top = 150+i*40;
		  // if(rcClient.top<=-50)m_FontCurr[i].m_Have=false;
	       rcClient.bottom= rcClient.top + 124 ;
	       rcClient.right=  rcClient.left +CLIENT_WIDTH*2;
	       LPCWSTR strLpc22 = m_FontCurr[i].m_Infomation;

		   CString stra = m_Infomation;
		   int width  = stra.GetLength();
	       rcClient.left=m_FontCurr[i].m_Curr;
		   if(rcClient.left<=-CLIENT_WIDTH){m_FontCurr[i].m_Have=false;}
	       m_DxfontA->DrawTextW(m_pSprite,strLpc22,  wcslen(strLpc22), &rcClient, DT_TOP | DT_LEFT, D3DCOLOR_ARGB(255,m_rgb[i%10][0],m_rgb[i%10][1],m_rgb[i%10][2]));

	   }

	}
	 if(m_FontCurr[19].m_Have)
	   {
		   CRect rcClient;
		   //rcClient.top=(520+i*20)-m_FontCurr[i].m_Curr*2;
		   rcClient.top = 300;
		  // if(rcClient.top<=-50)m_FontCurr[i].m_Have=false;
	       rcClient.bottom= rcClient.top + 124 ;
	       rcClient.right=  rcClient.left +CLIENT_WIDTH*2;
	       LPCWSTR strLpc22 = m_FontCurr[19].m_Infomation;

		   CString stra = m_Infomation;
		   int width  = stra.GetLength();
	       rcClient.left=m_FontCurr[19].m_Curr;
		   if(rcClient.left<=-2*CLIENT_WIDTH){m_FontCurr[19].m_Have=false;}

		    D3DXMatrixTransformation2D(&mat2,NULL,0.0f,&D3DXVECTOR2(111.6,1.0),&D3DXVECTOR2(0,0),0*(D3DX_PI/180),&D3DXVECTOR2(0,285));
		   m_pSprite->SetTransform(&mat2);

		   m_pSprite->Draw(FontBgTure, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
           m_pSprite->SetTransform(&matRot);
	       m_DxfontA->DrawTextW(m_pSprite,strLpc22,  wcslen(strLpc22), &rcClient, DT_TOP | DT_LEFT, D3DCOLOR_ARGB(255,255,255,255));

	}

     //绘制边框
	if(!m_IsFullScreen)
	{
	  m_pSprite->Draw(m_BorderImgTexture, NULL, NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
	  //绘制帐户及积分信息
	  CRect rcClient;
	  rcClient.top=695;
	  rcClient.left=75;
	  rcClient.bottom= rcClient.top + 24 ;
      rcClient.right=  rcClient.left +150;
	  LPCWSTR strLpc = m_Username;
	  m_Dxfont->DrawTextW(m_pSprite,strLpc,  wcslen(strLpc), &rcClient, DT_TOP | DT_LEFT, D3DCOLOR_ARGB(255,145,240,255));
	  //积分
	  rcClient.top=695;
	  rcClient.left=375;
	  rcClient.bottom= rcClient.top + 24 ;
	  rcClient.right=  rcClient.left +150;
	  WCHAR strLpc1[32];
	  _sntprintf(strLpc1,sizeof(strLpc1),TEXT("%.2lf"),m_MyScore);
	  m_Dxfont->DrawTextW(m_pSprite,strLpc1,  wcslen(strLpc1), &rcClient, DT_TOP | DT_LEFT, D3DCOLOR_ARGB(255,0,255,255));
	}


    //绘制介绍图   
	if(m_IsIntro&&!m_IsFullScreen)
	   m_pSprite->Draw(m_IntroImgTexture, NULL, NULL, &D3DXVECTOR3(350, 180, 0), D3DCOLOR_ARGB(200,255,255,255));	
	//绘制设置
	m_configDlg.InitRender(m_pSprite);
    



	//绘制按扭
	DrawBt();
		//绘制标题栏
		CRect Rct;
		Rct.left = 10;
		Rct.right = 780;
		Rct.top = 1;
		Rct.bottom = 80;
		m_Dxfont->DrawTextW(m_pSprite, m_szTitle,  wcslen(m_szTitle), &Rct, DT_TOP | DT_LEFT, D3DCOLOR_ARGB(255,255,255,255));

     //清理缓存
    m_pSprite->Flush();
	//绘制结束,停止精灵
	if(device!=NULL)
	{
	  MP_Manager& MP=MP_Manager::GetInstance();
	  MP.UpdateByTimer();
      HM_EMITTER hmEmitter=MP.GetFirstEmitter();
      while (hmEmitter) {
        MP_Emitter* emitter=MP.GetEmitter(hmEmitter);
        emitter->Render();
        hmEmitter=MP.GetNextEmitter(hmEmitter);
       }
	}
    m_pSprite->End();
	//停止设备
    m_pd3dDevice->EndScene();
	//更换设备到最前
    HRESULT hr = m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	    if(FAILED(hr))
          {
        
		   HRESULT hr = m_pd3dDevice->TestCooperativeLevel() ;
		   if (hr == D3DERR_DEVICENOTRESET)
            {
              //处理丢失
			  m_pSprite -> OnLostDevice(); 
              m_Dxfont -> OnLostDevice();
			  if (FAILED(m_pd3dDevice -> Reset( & m_d3dpp)))
                  {
					    //AfxMessageBox("3");
                        return ;
                   }
			  m_pSprite -> OnResetDevice();
              m_Dxfont -> OnResetDevice();

		     }
          }

}
//绘制按扭
void GameScreenWnd::DrawBt()
{
	if(!m_InitScreen||m_IsFullScreen)return;
	CRect rcClient;
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[6])rcClient.left=52;
	rcClient.bottom= rcClient.top + 28 ;
	rcClient.right=  rcClient.left +52;
	m_pSprite->Draw(m_BtTexture[6], rcClient, NULL, &D3DXVECTOR3(CLIENT_WIDTH-164, -3, 0), D3DCOLOR_ARGB(255,255,255,255));
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[0])rcClient.left=52;
	rcClient.bottom= rcClient.top + 28 ;
	rcClient.right=  rcClient.left +52;
	m_pSprite->Draw(m_BtTexture[0], rcClient, NULL, &D3DXVECTOR3(CLIENT_WIDTH-215, -3, 0), D3DCOLOR_ARGB(255,255,255,255));
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[1])rcClient.left=52;
	rcClient.bottom= rcClient.top + 28 ;
	rcClient.right=  rcClient.left +52;
	m_pSprite->Draw(m_BtTexture[1], rcClient, NULL, &D3DXVECTOR3(CLIENT_WIDTH-60,-3, 0), D3DCOLOR_ARGB(255,255,255,255));
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[2])rcClient.left=52;
	rcClient.bottom= rcClient.top + 28 ;
	rcClient.right=  rcClient.left +52;
	m_pSprite->Draw(m_BtTexture[2], rcClient, NULL, &D3DXVECTOR3(CLIENT_WIDTH-112, -3, 0), D3DCOLOR_ARGB(255,255,255,255));
	
	//如果还未加载完，就不显示下面2个按扭
	
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[3])rcClient.left=104;
	rcClient.bottom= rcClient.top + 28 ;
	rcClient.right=  rcClient.left +52;
	m_pSprite->Draw(m_BtTexture[3], rcClient, NULL, &D3DXVECTOR3(575, 687, 0), D3DCOLOR_ARGB(255,255,255,255));

	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[4])rcClient.left=104;
	rcClient.bottom= rcClient.top + 28 ;
	rcClient.right=  rcClient.left +52;
	m_pSprite->Draw(m_BtTexture[4], rcClient, NULL, &D3DXVECTOR3(625, 687, 0), D3DCOLOR_ARGB(255,255,255,255));

  	rcClient.top=0;
  	rcClient.left=0;
  	if(m_IsMouseOver[7])rcClient.left=46;
  	rcClient.bottom= rcClient.top + 38 ;
  	rcClient.right=  rcClient.left +46;
  	m_pSprite->Draw(m_BtTexture[7], rcClient, NULL, &D3DXVECTOR3(115, 645, 0), D3DCOLOR_ARGB(255,255,255,255));

	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[8])rcClient.left=46;
	rcClient.bottom= rcClient.top + 38 ;
	rcClient.right=  rcClient.left +46;
	m_pSprite->Draw(m_BtTexture[8], rcClient, NULL, &D3DXVECTOR3(190, 645, 0), D3DCOLOR_ARGB(255,255,255,255));
	

}
//发射子弹
void GameScreenWnd::UserShoot(int PlayerID,float Roation,int RealChairID,bool isAndroid,int zidanID)
{
	
    if(m_isxiuyuqi)return;
	m_IsAndroid[PlayerID]=isAndroid;
	if(PlayerID == m_MeChairID)
	{
		if(m_UserScore[m_MeChairID]-m_BeiLv[m_MeChairID]<0)return;
	}
	SetTimer(TIMERIDSHOOT,50,NULL);
	if(m_UserSuperPao[PlayerID])
	{
		if(m_configDlg.m_ptx2>0)
			CBassMusicManager::GetInstance()->Play(61, false);
	}
	else 
	{ 
		//if(CBassMusicManager::GetInstance()->IsPlaying(50+PlayerID))CBassMusicManager::GetInstance()->RePlay(50+PlayerID, false);
		if(m_configDlg.m_ptx2>0)
		   CBassMusicManager::GetInstance()->Play(51, false);
	}
	//设置玩家为发射状态
	m_UserSendstate[PlayerID] = 1;
	//设置炮位置
	m_UserPaoPT[PlayerID] = m_UserPT[PlayerID];
	float jiaodu =  (int(Roation)+360)%360;
	if(PlayerID>=3)
	{
	  if(jiaodu>=270){jiaodu = 90+jiaodu-270;}
	  else if(jiaodu<=90){jiaodu = 180+jiaodu;}
	}
	m_UserPaoJiaodu[PlayerID] = jiaodu;
    //判断发射玩家m,如果是自己
	if(PlayerID == m_MeChairID)
	{
		if(m_UserScore[m_MeChairID]-m_BeiLv[m_MeChairID]<0)return;
		m_OutTime = 120;
	   //设置子弹
		for(int i=0;i<100;i++)
		{
		  if(m_MeBullet[i].m_Have==true)continue;
		  m_MeBullet[i].m_isThree = 0;
		  m_MeBullet[i].m_issuper = false;
		  //如果是超级炮就设置子弹为超级子弹
		  if(m_UserSuperPao[PlayerID])m_MeBullet[i].m_issuper = true;
		  //如果大于500倍率就是3发子弹
		  switch (m_CellScore)
		  {
		   case 10:
			{
			   if(m_BeiLv[PlayerID]<=20)m_MeBullet[i].m_isThree=0;
		       else if(m_BeiLv[PlayerID]>20&&m_BeiLv[PlayerID]<=50)m_MeBullet[i].m_isThree=1;
		       else m_MeBullet[i].m_isThree=2;
			   break;

			}
		   case 100:
			{
			   if(m_BeiLv[PlayerID]<=200)m_MeBullet[i].m_isThree=0;
		       else if(m_BeiLv[PlayerID]>200&&m_BeiLv[PlayerID]<=500)m_MeBullet[i].m_isThree=1;
		       else m_MeBullet[i].m_isThree=2;
			   break;
			}
		   case 1000:
			{
			    if(m_BeiLv[PlayerID]<=2000)m_MeBullet[i].m_isThree=0;
		       else if(m_BeiLv[PlayerID]>2000&&m_BeiLv[PlayerID]<=5000)m_MeBullet[i].m_isThree=1;
		       else m_MeBullet[i].m_isThree=2;
			   break;
			}
		  }
	  	  int bulletwidth = 35;
		  int bulletheight = 44;
		  if(m_MeBullet[i].m_isThree==1)
			{
			     bulletwidth = 40;
			     bulletheight = 34;
			}
		  if(m_MeBullet[i].m_isThree==2)
			{
			     bulletwidth = 46;
			     bulletheight = 38;									//change from 32 to 38
			}
		  if(m_MeBullet[i].m_issuper)
		  {
		         bulletwidth = 64;
			     bulletheight = 64;
		  }
	  	  m_MeBullet[i].m_ptx = m_UserPT[PlayerID].x+97-bulletwidth/2*0.8;
		  m_MeBullet[i].m_pty = m_UserPT[PlayerID].y+90-bulletheight/2*0.8;
		  m_MeBullet[i].m_Have = true;
		  m_MeBullet[i].m_Stop = false;
		  m_MeBullet[i].m_Netindex = 0;
		  m_MeBullet[i].m_Senduser = PlayerID;
		  m_MeBullet[i].m_Roation = jiaodu;
		  m_MeBullet[i].m_isAndroid = false;
		  m_MeBullet[i].m_RealUser = RealChairID;
		  m_MeBullet[i].m_ZiDanID = zidanID;
		  m_MeBullet[i].m_Bilv = m_BeiLv[PlayerID];

		  break;
		}
	}
	else
	{
	  
	   //设置子弹
		for(int i=100;i<200;i++)
		{
		  if(m_MeBullet[i].m_Have==true)continue;
		  m_MeBullet[i].m_isThree = 0;
		  m_MeBullet[i].m_issuper = false;
		  //如果是超级炮就设置子弹为超级子弹
		  if(m_UserSuperPao[PlayerID])m_MeBullet[i].m_issuper = true;
		  //如果大于500倍率就是3发子弹
		  switch (m_CellScore)
		  {
		   case 10:
			{
			   if(m_BeiLv[PlayerID]<=20)m_MeBullet[i].m_isThree=0;
		       else if(m_BeiLv[PlayerID]>20&&m_BeiLv[PlayerID]<=50)m_MeBullet[i].m_isThree=1;
		       else m_MeBullet[i].m_isThree=2;
			   break;

			}
		   case 100:
			{
			   if(m_BeiLv[PlayerID]<=200)m_MeBullet[i].m_isThree=0;
		       else if(m_BeiLv[PlayerID]>200&&m_BeiLv[PlayerID]<=500)m_MeBullet[i].m_isThree=1;
		       else m_MeBullet[i].m_isThree=2;
			   break;
			}
		   case 1000:
			{
			    if(m_BeiLv[PlayerID]<=2000)m_MeBullet[i].m_isThree=0;
		       else if(m_BeiLv[PlayerID]>2000&&m_BeiLv[PlayerID]<=5000)m_MeBullet[i].m_isThree=1;
		       else m_MeBullet[i].m_isThree=2;
			   break;
			}
		  }
	  	  int bulletwidth = 35;
		  int bulletheight = 44;
		  if(m_MeBullet[i].m_isThree==1)
			{
			     bulletwidth = 40;
			     bulletheight = 34;
			}
		  if(m_MeBullet[i].m_isThree==2)
			{
			     bulletwidth = 46;
			     bulletheight = 38;									//change from 32 to 38
			}
		  if(m_MeBullet[i].m_issuper)
		  {
		         bulletwidth = 64;
			     bulletheight = 64;
		  }
	  	  m_MeBullet[i].m_ptx = m_UserPT[PlayerID].x+97-bulletwidth/2*0.8;
		  m_MeBullet[i].m_pty = m_UserPT[PlayerID].y+90-bulletheight/2*0.8;
		  m_MeBullet[i].m_Have = true;
		  m_MeBullet[i].m_Stop = false;
		  m_MeBullet[i].m_Netindex = 0;
		  m_MeBullet[i].m_Senduser = PlayerID;
		  m_MeBullet[i].m_Roation = jiaodu;
		  m_MeBullet[i].m_isAndroid = isAndroid;
		  m_MeBullet[i].m_RealUser = RealChairID;
		  m_MeBullet[i].m_ZiDanID = zidanID;
		  m_MeBullet[i].m_Bilv = m_BeiLv[PlayerID];
		  
		 // if()
		  //if(PlayerID>=3)m_MeBullet[i].m_Roation= Roation+180;
		  break;
		}
	
	}

}
//定住
void GameScreenWnd::SetDingAction(Fish *Deadfish)
{
    m_IsDingzhu =  true;
	MP_Manager& MP=MP_Manager::GetInstance();
    MP_Emitter* emitter = NULL;
	emitter=MP.GetEmitterByName("定海");
	if (emitter != NULL)
		{
            MP_POSITION pos;
            emitter->GetPosition(pos);
            pos.x =  Deadfish->m_ptx + m_FishScale*m_FishmoveRec[Deadfish->m_index][0]/2;
            pos.y =  Deadfish->m_pty +  m_FishScale*m_FishmoveRec[Deadfish->m_index][1]/2;
            emitter->Move(pos);
			emitter->SetState(MAGIC_STATE_STOP);
			emitter->SetState(MAGIC_STATE_UPDATE);
	    }
	SetTimer(TIMERIDDING,13000,NULL);
	if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(43, false);

}
//炸弹爆炸
void GameScreenWnd::SetBomAction(int PlayerId,int Fishid,int fishscore)
{
	INT_PTR nEnumIndex=0;
	Fish *tfish=NULL;
    do{
		tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		if (NULL==tfish) break;
		if(GetTickCount()-tfish->m_creatTime<3000)
		   UserAddMoney(PlayerId,tfish->m_fishID,m_fishscore[tfish->m_index]*m_BeiLv[PlayerId],m_fishscore[tfish->m_index],false);

	}while(true);
}
//抽奖过程
void GameScreenWnd::ChouJiangVoid(int m_index,int score,int player)
{
      m_ChouJiang[player].ishave=true;
	  m_ChouJiang[player].index = m_index;
	  m_ChouJiang[player].nowindex = 0;
	  m_ChouJiang[player].quanshu = 0;
	  m_ChouJiang[player].score=score;
	 

}
void GameScreenWnd::ChouJiangFinshVoid(int player)
{


}
//抽奖完成

//加钱用户
void GameScreenWnd::UserAddMoney(int PlayerID,int Fishid,int Money,int FishKindScord,bool ishavaction )
{
	
	if(m_InitScreen==false)return;
	int Fishindx = -1;
	INT_PTR nEnumIndex=0;
	Fish *Deadfish=NULL;
	Fish *tfish=NULL;
	do{
		tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		if (NULL==tfish) break;
		if(tfish->m_fishID==Fishid)
		{
		  Deadfish = tfish;
		  break;
		}
	}while(true);
	
	if(Deadfish==NULL){return;}
	//是否是棒子需要定住
	if(Deadfish->m_index==19)
	{
	   SetDingAction(Deadfish);
	}
	//设置这条鱼的死亡状态
	//m_FishArr[j].m_DeadMoney = 1000;
	Deadfish->m_HitUser = PlayerID;
	Deadfish->m_isHit = true;
	Deadfish->m_actionindex = 0;
	Deadfish->m_Nowindex = 0;
	//如果只是让鱼死亡 就直接返回
	if(FishKindScord<=0)return;
	//设置完毕
    //播放声音
    char musicPath[32]={0};
	int musciindex= rand()%20;
	if(musciindex<9)
	 {	
		 if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(20+musciindex, false);
	 }
	else
	 {
		 if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(29, false);
		    
	 }	
    if(Deadfish->m_index==24||Deadfish->m_index==14)
	{
	   for(int ci=0;ci<6;ci++)
	   { //break;
	      if(m_ChouJiang[ci].ishave)continue;
		  m_ChouJiang[ci].roloation=1.0;
	      m_ChouJiang[ci].ishave=true;
	      m_ChouJiang[ci].index = 80;
	      m_ChouJiang[ci].quanshu = Deadfish->m_ptx +  m_FishScale*m_FishmoveRec[Deadfish->m_index][0]/2-350;
	      m_ChouJiang[ci].nowindex=  Deadfish->m_pty +  m_FishScale*m_FishmoveRec[Deadfish->m_index][1]/2-350;
	   }
	  
	}

	if(FishKindScord>=25&&ishavaction&&Deadfish->m_smallFish==-1)
	{
		 GiveMuchMoney(Fishid,PlayerID,Money,Deadfish,FishKindScord);
	}
	else if(Deadfish->m_smallFish!=-1&&ishavaction&&FishKindScord>=20)
	{
	   GiveZhaMoney(Fishid,PlayerID,Money,Deadfish,FishKindScord);
	   return;
	}
	 //显示鱼的分数
	 for(int ci=0;ci<50;ci++)
		 {  
			 if(m_NumArr[ci].m_Have==true)continue;
			 m_NumArr[ci].m_Have = true;
			 m_NumArr[ci].m_ptx =  Deadfish->m_ptx +  m_FishScale*m_FishmoveRec[Deadfish->m_index][0]/2;
			 m_NumArr[ci].m_pty =  Deadfish->m_pty +  m_FishScale*m_FishmoveRec[Deadfish->m_index][1]/2;
			 m_NumArr[ci].m_Time = 0;
			 m_NumArr[ci].m_Numcount = Money;
			 m_NumArr[ci].m_HitUser=PlayerID;
			 m_NumArr[ci].m_beishu = 1.4f;
			 if(Money>=10000)m_NumArr[ci].m_beishu = 1.4f;
			 break;

		 }
	int ac = FishKindScord;
	if(ac>20)ac=20;
	int bc = ac/2;
	CBassMusicManager::GetInstance()->Play(42, false);
	//增加飘落金币
	for(int ci=0;ci<100;ci++)
	 {  // continue;
	    //如果已经占用循环下去
		if(m_GoldArr[ci].m_Have==true)continue;
		m_GoldArr[ci].m_Have = true;
		m_GoldArr[ci].m_Player = PlayerID;
		if(Deadfish->m_index<5)
			m_GoldArr[ci].m_goldtype = 1;
		else
			m_GoldArr[ci].m_goldtype = 0;
		m_GoldArr[ci].m_actionindex = rand()%12;
		if(ci % 2 == 0)
		  m_GoldArr[ci].m_ptx =  Deadfish->m_ptx +  m_FishScale*m_FishmoveRec[Deadfish->m_index][0]/2-6 * (rand()%100);
		else
          m_GoldArr[ci].m_ptx =  Deadfish->m_ptx +  m_FishScale*m_FishmoveRec[Deadfish->m_index][0]/2+6 * (rand()%100);
		m_GoldArr[ci].m_pty =  Deadfish->m_pty - (rand()%200);
		m_GoldArr[ci].m_speed = 15+rand()%10;
		 //求角度
		float px = m_GoldArr[ci].m_ptx-m_UserPT[PlayerID].x-60;
        float py = m_UserPT[PlayerID].y-m_GoldArr[ci].m_pty+100;

		if(PlayerID>=3)
		{
		    py = m_UserPT[PlayerID].y-m_GoldArr[ci].m_pty+50;
		}
        float bili = float(px/py);
        float jiaodu=atan2(px,py)*180.0/3.1415926;
        m_GoldArr[ci].m_Roation=jiaodu;
		ac--;
		bc--;
		if(m_GoldArr[ci].m_goldtype == 1 && ci>30)
			break;
		//金币
		if(m_GoldArr[ci].m_goldtype == 0 && FishKindScord < 17 && ci > 50)
			break;
		//if(ac<=0)break;
	}


    m_UserScore[PlayerID] =m_UserScore[PlayerID] + Money;
	if(!m_UserStruct[PlayerID][0].m_Have)
	{
		m_UserStruct[PlayerID][0].m_Have=true;
		m_UserStruct[PlayerID][0].m_Money = Money;
		m_UserStruct[PlayerID][0].m_Time = 0;
		m_UserStruct[PlayerID][0].m_FishScore = FishKindScord;
		m_UserStruct[PlayerID][0].m_Nowscore = 0;

		m_UserStruct[PlayerID][0].m_colorindex = 0;



	}
	else if(m_UserStruct[PlayerID][0].m_Have&&!m_UserStruct[PlayerID][1].m_Have)
	{
		m_UserStruct[PlayerID][1].m_Have=true;
		m_UserStruct[PlayerID][1].m_Money = Money;
		m_UserStruct[PlayerID][1].m_Time = 0;
		m_UserStruct[PlayerID][1].m_FishScore = FishKindScord;
		m_UserStruct[PlayerID][1].m_Nowscore = 0;
		if(m_UserStruct[PlayerID][0].m_colorindex == 0)
		   m_UserStruct[PlayerID][1].m_colorindex = 1;
		else if(m_UserStruct[PlayerID][0].m_colorindex == 1)
			m_UserStruct[PlayerID][1].m_colorindex = 2;
		else 
			m_UserStruct[PlayerID][1].m_colorindex = 0;
	}
	else if(m_UserStruct[PlayerID][0].m_Have&&m_UserStruct[PlayerID][1].m_Have&&!m_UserStruct[PlayerID][2].m_Have)
	{
		m_UserStruct[PlayerID][2].m_Have=true;
		m_UserStruct[PlayerID][2].m_Money = Money;
		m_UserStruct[PlayerID][2].m_Time = 0;
		m_UserStruct[PlayerID][2].m_FishScore = FishKindScord;
		m_UserStruct[PlayerID][2].m_Nowscore = 0;
		if(m_UserStruct[PlayerID][1].m_colorindex == 0)
		   m_UserStruct[PlayerID][2].m_colorindex = 1;
		else if(m_UserStruct[PlayerID][1].m_colorindex == 1)
			m_UserStruct[PlayerID][2].m_colorindex = 2;
		else 
			m_UserStruct[PlayerID][2].m_colorindex = 0;
	}
	else
	{
		m_UserStruct[PlayerID][0].m_Have =  m_UserStruct[PlayerID][1].m_Have;
		m_UserStruct[PlayerID][0].m_Money =  m_UserStruct[PlayerID][1].m_Money;
		m_UserStruct[PlayerID][0].m_Time =  0;
		m_UserStruct[PlayerID][0].m_FishScore = m_UserStruct[PlayerID][1].m_FishScore;
		m_UserStruct[PlayerID][0].m_colorindex = m_UserStruct[PlayerID][1].m_colorindex;
		m_UserStruct[PlayerID][0].m_Nowscore = m_UserStruct[PlayerID][1].m_Nowscore;
        m_UserStruct[PlayerID][1].m_Have =  m_UserStruct[PlayerID][2].m_Have;
		m_UserStruct[PlayerID][1].m_Money =  m_UserStruct[PlayerID][2].m_Money;
		m_UserStruct[PlayerID][1].m_FishScore = m_UserStruct[PlayerID][2].m_FishScore;
		m_UserStruct[PlayerID][1].m_Nowscore = m_UserStruct[PlayerID][2].m_Nowscore;
		m_UserStruct[PlayerID][1].m_colorindex = m_UserStruct[PlayerID][2].m_colorindex;
		m_UserStruct[PlayerID][1].m_Time =  0;
		m_UserStruct[PlayerID][2].m_Have=true;
		m_UserStruct[PlayerID][2].m_Money = Money;
		m_UserStruct[PlayerID][2].m_Time = 0;
		m_UserStruct[PlayerID][2].m_FishScore = FishKindScord;
		m_UserStruct[PlayerID][2].m_Nowscore = 0;
		if(m_UserStruct[PlayerID][1].m_colorindex == 0)
		   m_UserStruct[PlayerID][2].m_colorindex = 1;
		else if(m_UserStruct[PlayerID][1].m_colorindex == 1)
			m_UserStruct[PlayerID][2].m_colorindex = 2;
		else 
			m_UserStruct[PlayerID][2].m_colorindex = 0;

		
	}
	
	
}
//掉落金币
void GameScreenWnd::GiveMuchMoney(int Fishindx,int PlyaerID,int Money,Fish *Deadfish,int fishscore)
{
	//背景抖动
	m_doudongstu.m_have = true;
	m_doudongstu.m_doucount = 0;
	m_doudongstu.m_height = 0;
	m_doudongstu.m_height = 0;
	//抖动结束
	m_MuchmoneyAct[PlyaerID].m_have = true;
//	m_MuchmoneyAct[PlyaerID].m_Alpha = 255;
	m_MuchmoneyAct[PlyaerID].m_nowindex = 0;
	m_MuchmoneyAct[PlyaerID].m_DiuMoney = Money;
	m_MuchmoneyAct[PlyaerID].m_ptx = Deadfish->m_ptx+ m_FishScale*m_FishmoveRec[Deadfish->m_index][0]/2-1024/2;
	m_MuchmoneyAct[PlyaerID].m_pty = Deadfish->m_pty+ m_FishScale*m_FishmoveRec[Deadfish->m_index][1]/2-512/2;

	MP_Manager& MP=MP_Manager::GetInstance();
    MP_Emitter* emitter = NULL;
	emitter=MP.GetEmitterByName("鱼雷爆炸");
	if (emitter != NULL)
		{
            MP_POSITION pos;
            emitter->GetPosition(pos);
            pos.x =  Deadfish->m_ptx + m_FishScale*m_FishmoveRec[Deadfish->m_index][0]/2;
            pos.y =  Deadfish->m_pty +  m_FishScale*m_FishmoveRec[Deadfish->m_index][1]/2;
            emitter->Move(pos);
			emitter->SetState(MAGIC_STATE_STOP);
			emitter->SetState(MAGIC_STATE_UPDATE);
	    }
	if(Deadfish->m_index%2==0)
	{ if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(200+rand()%8, false);}
	else
	{ if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(100+rand()%8, false);}
	if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(43, false);
  
}
///掉落金币
void GameScreenWnd::GiveZhaMoney(int Fishindx,int PlyaerID,int Money,Fish *Deadfish,int fishscore)
{
	//背景抖动
	m_doudongstu.m_have = true;
	m_doudongstu.m_doucount = 0;
	m_doudongstu.m_height = 0;
	m_doudongstu.m_height = 0;
	//抖动结束
    //AddFire(Deadfish->m_ptx+m_FishmoveRec[Deadfish->m_index][0]/2-1024/2,Deadfish->m_pty+m_FishmoveRec[Deadfish->m_index][1]/2-512/2);
	m_zhaaction[PlyaerID].m_have = true;
	m_zhaaction[PlyaerID].m_nowindex = 0;
	m_zhaaction[PlyaerID].m_DiuMoney = Money;
	m_zhaaction[PlyaerID].m_ptx = Deadfish->m_ptx+ m_FishScale*m_FishmoveRec[Deadfish->m_index][0]/2-1024/2;
	m_zhaaction[PlyaerID].m_pty = Deadfish->m_pty+ m_FishScale*m_FishmoveRec[Deadfish->m_index][1]/2-512/2;
	MP_Manager& MP=MP_Manager::GetInstance();
    MP_Emitter* emitter = NULL;
	if(Fishindx%2==0)
	  emitter=MP.GetEmitterByName("一网打尽爆炸");
	else
      emitter=MP.GetEmitterByName("海啸来袭爆炸");
	if (emitter != NULL)
		{
            MP_POSITION pos;
            emitter->GetPosition(pos);
            pos.x =  Deadfish->m_ptx + m_FishScale*m_FishmoveRec[Deadfish->m_index][0]/2;
            pos.y =  Deadfish->m_pty +  m_FishScale*m_FishmoveRec[Deadfish->m_index][1]/2;
            emitter->Move(pos);
			emitter->SetState(MAGIC_STATE_STOP);
			emitter->SetState(MAGIC_STATE_UPDATE);
	    }
	if(Deadfish->m_index%2==0)
	{ if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(200+rand()%8, false);}
	else
	{ if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(100+rand()%8, false);}
	if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(43, false);
  
}
//切换场景
void GameScreenWnd::ChangeScreen(int bgindex)
{

    m_isxiuyuqi = true;
    m_FishStorg.FreeAllItem();
	//m_FishStorg.FreeAllItem();
	if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(40, false);
	CBassMusicManager::GetInstance()->Stop(0);
	CBassMusicManager::GetInstance()->Stop(1);
	CBassMusicManager::GetInstance()->Stop(2);
	CBassMusicManager::GetInstance()->Play(bgindex, false);
	m_ChanwaveSt.m_currImgIndex = 0;
	m_ChanwaveSt.m_ptx = CLIENT_WIDTH;
	m_ChanwaveSt.m_pty = 0;
	m_ChanwaveSt.m_Time = 0;
	m_IsChangeScreen = true;
	m_Bgindex = bgindex;
	//开启动动画
	SetTimer(TIMERIDCHANGESCREEN,50,NULL);


}
bool adda =false;
int adf = 0;
//消息截取
BOOL GameScreenWnd::PreTranslateMessage(MSG* pMsg)
{
	
	//转换到屏幕坐标
	int  Rptx = pMsg->pt.x;
	int  Rpty = pMsg->pt.y;
	ScreenToClient(&pMsg->pt); 
	//鼠标按下
	if ( pMsg->message == WM_LBUTTONDOWN )
	{

		m_configDlg.LMouseDown(pMsg->pt.x,pMsg->pt.y);
		CRect introbt;
		introbt.left =370;
		introbt.top = 200;
		introbt.right = 370 + 569;
		introbt.bottom = 200+358;
        if(introbt.PtInRect(pMsg->pt)&&m_IsIntro)
		{
			m_IsIntro=false;
			return true;
		}

		if(m_configDlg.m_isvisable||m_IsIntro)return true;
		ReleaseCapture();//为当前的应用程序释放鼠标捕获
		SetFocus();
	   //移动窗口
		//移动窗口
		if(pMsg->pt.y<30&&!m_IsFullScreen)
	    {
		   
	       AfxGetMainWnd()->PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(pMsg->pt.x, pMsg->pt.y));
	    }

        if(pMsg->pt.y>30&&pMsg->pt.y<680&&m_InitScreen)
	    {
 			if(m_InitScreen&&m_BtRect[7].PtInRect(pMsg->pt))
 			{
 				m_baseSink->ChangeFireBei(true);
 			}
 			else if(m_InitScreen&&m_BtRect[8].PtInRect(pMsg->pt))
			{
				m_baseSink->ChangeFireBei(false);
			}
			else
			{
				if(GetTickCount() - m_NowTime > 300 && !m_isxiuyuqi)  //发射
				{
              		//如果没钱直接弹出去
					if(m_UserScore[m_MeChairID]<=0)return true;
					DWORD m_BulletID = GetTickCount();
					m_baseSink->OnShoot(false,m_UserPaoJiaodu[m_MeChairID],m_BulletID);
					UserShoot(m_MeChairID,m_UserPaoJiaodu[m_MeChairID],m_MeRellayChairID,false,m_BulletID);
					m_NowTime = GetTickCount() ;

				}
				SetTimer(TIMERIDSENDSHOOT,300,NULL);
				//设置为鼠标按下状态
				m_MouseDown = true;
				//UserShoot(5);
			}
		}
		//切换场景
		//ChangeScreen(1);
		//发送消息	
		if(m_IsFullScreen)return true;
		//按扭事件
		if(m_InitScreen&&m_BtRect[4].PtInRect(pMsg->pt))
		{
		   m_baseSink->AddOrRemoveScore(true,m_BeiLv[m_MeChairID]);
		   if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(15, false);
		}
		if(m_InitScreen&&m_BtRect[3].PtInRect(pMsg->pt))
		{
		   m_baseSink->AddOrRemoveScore(false,m_BeiLv[m_MeChairID]);
		   if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(16, false);
		}
		


		if(m_InitScreen&&m_BtRect[5].PtInRect(pMsg->pt))
		{
			m_bLockShoot = !m_bLockShoot;
			//ShellExecute(NULL,TEXT("OPEN"),L"http://game.waiap.com/Pay/",NULL,NULL,SW_NORMAL);
		}
		if(m_bLockShoot)
		{
			SetTimer(TIMERIDALWAYSSHOOT,300,NULL);
		}
		else
			KillTimer(TIMERIDALWAYSSHOOT);

		if(m_BtRect[0].PtInRect(pMsg->pt))AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
		if(m_BtRect[1].PtInRect(pMsg->pt)){if(!m_InitScreen){m_baseSink->InitClientMessage();}m_baseSink->CloseExit();}
		if(m_BtRect[2].PtInRect(pMsg->pt))m_configDlg.m_isvisable = true;
		if(m_BtRect[6].PtInRect(pMsg->pt)){AfxGetMainWnd()->PostMessage(25,VK_ESCAPE);}


	
	}
	//鼠标松开
	else if ( pMsg->message == WM_LBUTTONUP )
	{
		m_configDlg.LMouseUp(pMsg->pt.x,pMsg->pt.y);
		//设置鼠标状态为松开
        m_MouseDown =  false;
		//停止子弹发射
		KillTimer(TIMERIDSENDSHOOT);

		int nX=625;
		int nY=670;
		CRect RecLock(nX+172, nY, nX+222, nY+44);
		CRect RecF1(nX+255, nY, nX+306, nY+44);
		CRect RecF2(nX+340, nY, nX+390, nY+44);
		CRect RecF3(nX+420, nY, nX+470, nY+44);
		if(RecLock.PtInRect(pMsg->pt))					//'S' lock down
		{
			::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_KEYDOWN, 83 , 1);
		}
		if(RecF1.PtInRect(pMsg->pt))					//F1
		{
			m_baseSink->AddOrRemoveScore(true, m_BeiLv[m_MeChairID]);
			if(m_configDlg.m_ptx2>0)
				CBassMusicManager::GetInstance()->Play(15, false);
		}
		if(RecF2.PtInRect(pMsg->pt))					//F2
		{
			m_baseSink->AddOrRemoveScore(false, m_BeiLv[m_MeChairID]);
			if(m_configDlg.m_ptx2>0)
				CBassMusicManager::GetInstance()->Play(15, false);

			// m_baseSink->ChangeFireBei(true);
		}
// 		if(RecF3.PtInRect(pMsg->pt))					//F3
// 		{
// 			m_baseSink->ChangeFireBei(false);
// 		}
	}
	//键盘按下
	else if ( pMsg->message == WM_KEYDOWN  )//鼠标移动
	{
		//CString str;
		//str.Format(L"%d",pMsg->wParam);
        //AfxMessageBox(str);
		m_configDlg.LMouseDown(pMsg->pt.x,pMsg->pt.y);
		if((m_configDlg.m_isvisable||m_IsIntro)&&((WCHAR)pMsg->wParam!=VK_ESCAPE))return true;
		if(!m_InitScreen&&((WCHAR)pMsg->wParam!=VK_ESCAPE))return true;
	    switch( (WCHAR)pMsg->wParam )
            {
				
                case VK_SPACE://空格发射
                {
                   if(GetTickCount() - m_NowTime > 300 &&!m_isxiuyuqi)
		              {
						  //如果没钱直接弹出去
						  if(m_UserScore[m_MeChairID]<=0)break;
						  DWORD m_BulletID = GetTickCount();
						  m_baseSink->OnShoot(false,m_UserPaoJiaodu[m_MeChairID],m_BulletID);
			              UserShoot(m_MeChairID,m_UserPaoJiaodu[m_MeChairID],m_MeRellayChairID,false,m_BulletID);
			              m_NowTime = GetTickCount() ;
		               }
					//SetTimer(TIMERIDSENDSHOOT,300,NULL);
					break;
				}
				case VK_F2://上键加炮
                {
					
				   m_baseSink->ChangeFireBei(true);
				   break;
				}
				case VK_F3://下减炮
                {
					m_baseSink->ChangeFireBei(false);
					//m_UserScore[m_MeChairID] = m_UserScore[m_MeChairID] + 100000;
					break;
					
				}
				case VK_F1://左上分
                {
					m_baseSink->AddOrRemoveScore(true,m_BeiLv[m_MeChairID]);
					if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(15, false);
                    return true;
					break;
				}
				case VK_F4://左下分
                {
                    m_baseSink->AddOrRemoveScore(false,m_BeiLv[m_MeChairID]);
                    if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(16, false);
					break;

				}case VK_RETURN://屏蔽回车键
				{
					return true;
                      
				}
				case VK_F5:		//自动锁定发炮。
				{
					m_bLockShoot = !m_bLockShoot;
					if(m_bLockShoot)
					{
						SetTimer(TIMERIDALWAYSSHOOT,300,NULL);
					}
					else
						KillTimer(TIMERIDALWAYSSHOOT);

					break;
				}
				case 83://S键琐定
                {
					m_bLockFish=true;

					INT_PTR nEnumIndex=m_SuoDingFishIndex;
	                Fish *tfish=NULL;
                 	do
	                 {
		                 tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		                 //枚举鱼群
						
						 if (NULL==tfish){m_SuoDingFishIndex = 0;break;}		//modify by david, 2016.6.17
				         if(tfish->m_Have==false)continue;
					     if(tfish->m_isHit==true)continue;
						 if((tfish->m_index>11||(tfish->m_smallFish!=-1&&tfish->m_index!=10&&tfish->m_index!=11))&&tfish->m_ptx>=(200- m_FishScale*m_FishmoveRec[tfish->m_index][0])&&tfish->m_ptx<=900&&tfish->m_pty>=(200- m_FishScale*m_FishmoveRec[tfish->m_index][1])&&tfish->m_pty<=600)
						 {
							 m_baseSink->SendLockFishID(tfish->m_fishID,m_MeRellayChairID); 
							 m_SuoDingFishIndex = nEnumIndex;
							 break;
						 }
						
					    
					  }while(true);
					break;
				}
				case 81://Q键 解除琐定
                {
					m_bLockFish = false;
                    m_SuoFishId[m_MeChairID] = 0;
		        	for(int i=0;i<10;i++)
			          {
				         m_SuoPao[m_MeChairID][i].m_have = false;
						
					  }
				}
			 
			}		
	}
	//键盘松开
	else if ( pMsg->message == WM_KEYUP  )//鼠标移动
	{
	    switch( (WCHAR)pMsg->wParam )
            {
                case VK_SPACE://空格发射
                {
					KillTimer(TIMERIDSENDSHOOT);
					break;
				}
			}		
	}

	else if ( pMsg->message == WM_MOUSEMOVE )//鼠标移动
	{


		POINT pt;
		pt.x = Rptx;
		pt.y = Rpty;

		SetCapture();
        CWnd* wfp = WindowFromPoint(pt);//从鼠标位置获取窗口句柄
        if (wfp && wfp->m_hWnd != this->m_hWnd)
          {
               bTrackLeave = false;
               ReleaseCapture();//为当前的应用程序释放鼠标捕获
			   m_MouseDown =  false;
		       //停止子弹发射
		       KillTimer(TIMERIDSENDSHOOT);

          }

		m_configDlg.LMouseMove(pMsg->pt.x,pMsg->pt.y);
		//
		
		if((pMsg->pt.y>670||pMsg->pt.y<30||pMsg->pt.x<5||pMsg->pt.x>1275||m_configDlg.m_isvisable||m_IsIntro)&&!m_IsFullScreen)
		{
		   SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR2)));
		   //m_cursor.m_have = false;
		   //if(m_CursorInt<0)m_CursorInt = ShowCursor(TRUE);
		   

		}
		else 
		{
		   if(m_InitScreen)
		   {
		      SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR1)));
		   }
		   else
			   SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR2)));
		   
		}

	   if(m_SuoFishId[m_MeChairID]==0)
		{
            //处理自己炮的移动角度
	    	float px=pMsg->pt.x+16-m_UserPT[m_MeChairID].x*Definex_bli-100;
	        float py=m_UserPT[m_MeChairID].y*Definex_bli-pMsg->pt.y+100;
	        float bili = float(px/py);
            float jiaodu=atan2(px,py)*180.0/3.1415926;
	        if(jiaodu<-90)jiaodu=-90;
            if(jiaodu>90)jiaodu=90;
		    if(jiaodu<0)jiaodu=360+jiaodu;
	        m_UserPaoJiaodu[m_MeChairID]=jiaodu;
		}
        
		for(int i=0;i<9;i++)
		{
		   m_IsMouseOver[i]=false;
		}
		for(int i=0;i<9;i++)
		{
		   if(m_BtRect[i].PtInRect(pMsg->pt))
			  m_IsMouseOver[i]=true;

		}

       

	}


	return CWnd::PreTranslateMessage(pMsg);
}




BEGIN_MESSAGE_MAP(GameScreenWnd, CWnd)
	ON_WM_TIMER()
	ON_MESSAGE(WM_DX_RENDER,  OnFrameRender)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


//时间事件
void GameScreenWnd::OnTimer(UINT nIDEvent)
{
	if(TIMERIDSHOOT==nIDEvent)
	{  
 
		for(int i= 0 ;i < GAME_PLAYER;i++)
		{
		 if(m_UserSendstate[i]>0)
		  {
		    if(m_UserSendstate[i]<3)
			{
		      	float m_x = 15*sin(2*3.1415926*(m_UserPaoJiaodu[i])/360.0);
	            float m_y = 15*cos(2*3.1415926*(m_UserPaoJiaodu[i])/360.0);
			    m_UserPaoPT[i].y =m_UserPT[i].y+m_y;
			    m_UserPaoPT[i].x =m_UserPT[i].x-m_x;
			}
			else
			{
			    m_UserPaoPT[i].y =m_UserPT[i].y;
			    m_UserPaoPT[i].x =m_UserPT[i].x;
			}
			m_UserSendstate[i]++;
			if(m_UserSendstate[i]>=10){m_UserSendstate[i]=0;}

		  }
		else 
		 {
		    m_UserPaoPT[i].y =m_UserPT[i].y;
			m_UserPaoPT[i].x =m_UserPT[i].x;
			//KillTimer(TIMERIDSHOOT);
		  }
		}
	}
	else if(TIMERIDBULLTE==nIDEvent)
	{
		for(int i=0;i<USERCOUNT;i++)
		{
			if(m_MuchmoneyAct[i].m_have==false)continue;
			
			//如果序列大于21就转换成更改图片角度
			if(m_MuchmoneyAct[i].m_nowindex>10)
			{
				m_MuchmoneyAct[i].m_Roation = m_MuchmoneyAct[i].m_Roation + 10 ;
			}

				

		}
		for(int i=0;i<USERCOUNT;i++)
		{
			if(m_zhaaction[i].m_have==false)continue;
			//如果序列大于21就转换成更改图片角度
			if(m_zhaaction[i].m_nowindex>10)
			{
				m_zhaaction[i].m_Roation = m_zhaaction[i].m_Roation + 15 ;
			}
					

		}
		  //显示文字
		for(int i=0;i<20;i++)
		{
		  if(m_FontCurr[i].m_Have)
	       {
		      m_FontCurr[i].m_Curr=m_FontCurr[i].m_Curr-1;
			 // if(m_FontCurr[i].m_Curr>=250){m_FontCurr[i].m_Have=false;m_FontCurr[i].m_Curr=0;}
		   }
		}
	     for(int i=0;i<200;i++)
		 {
			 if(m_MeBullet[i].m_Have==false)continue;
			 if(m_MeBullet[i].m_Stop==true)continue;		 

			 //判断边缘
			 if(m_MeBullet[i].m_ptx<0)//左边超出
			 {
                 m_MeBullet[i].m_Roation = 360-m_MeBullet[i].m_Roation;
			 }
			 if(m_MeBullet[i].m_ptx+64>CLIENT_WIDTH)//右边超出
			 {
			     m_MeBullet[i].m_Roation = 360-m_MeBullet[i].m_Roation;
			 }
			 if(m_MeBullet[i].m_pty+64>CLIENT_HEIGHT)//底部超出
			 {
			     m_MeBullet[i].m_Roation = 180-m_MeBullet[i].m_Roation;
			 }
			 if(m_MeBullet[i].m_pty<0)//顶部超出
			 {
			     m_MeBullet[i].m_Roation = 180-m_MeBullet[i].m_Roation;
			 }

			 if(m_MeBullet[i].m_ptx<0&&m_MeBullet[i].m_pty<0)
			 {
			    m_MeBullet[i].m_Roation = 135;
				m_MeBullet[i].m_ptx = 20;
			    m_MeBullet[i].m_pty = 20;
			 }
			 else if(m_MeBullet[i].m_ptx<0&&m_MeBullet[i].m_pty>CLIENT_HEIGHT)
			 {
			     m_MeBullet[i].m_Roation = 45;
				m_MeBullet[i].m_ptx = 20;
			    m_MeBullet[i].m_pty = 720;
			 }
			 else if(m_MeBullet[i].m_ptx>CLIENT_WIDTH&&m_MeBullet[i].m_pty>CLIENT_HEIGHT)
			 {
			     m_MeBullet[i].m_Roation = 315;
				m_MeBullet[i].m_ptx = 1260;
			    m_MeBullet[i].m_pty = 720;
			 }
			 else if(m_MeBullet[i].m_ptx>CLIENT_WIDTH&&m_MeBullet[i].m_pty<0)
			 {
			    m_MeBullet[i].m_Roation = 225;
				m_MeBullet[i].m_ptx = 1260;
			    m_MeBullet[i].m_pty = 20;
				

			 }

			 float m_x = 15*sin(2*3.1415926*(m_MeBullet[i].m_Roation)/360.0);
	         float m_y = 15*cos(2*3.1415926*(m_MeBullet[i].m_Roation)/360.0);
			 m_MeBullet[i].m_ptx = m_MeBullet[i].m_ptx + m_x;
			 m_MeBullet[i].m_pty = m_MeBullet[i].m_pty - m_y;
			 
		 }

		 //移动鱼儿坐标
	     Fish *tfish=NULL;
	     for(int i=0;i<m_FishFactory.GetActiveCount();i++)
	      {
		    //枚举鱼群
		    if(m_IsDingzhu)break;//如果目前定住直接弹出去
		    tfish=m_FishFactory.EnumActiveObject(i);
		    if (NULL==tfish) break;
		    //激活判断
			  if(tfish->m_Have==false)continue;
			  if(tfish->m_isHit==true)continue;
			  
			  float m_x = tfish->m_speed*m_speedBili*sin(2*3.1415926*(tfish->m_Roation)/360.0)/3.5;
	          float m_y = tfish->m_speed*m_speedBili*cos(2*3.1415926*(tfish->m_Roation)/360.0)/3.5;
			  tfish->m_ptx = tfish->m_ptx + m_x;
			  tfish->m_pty = tfish->m_pty - m_y;

		  }
         
		  //飘落金币坐标移动
		  for(int i=0;i<100;i++)
		  {
             if(m_GoldArr[i].m_Have==false)continue;
			 float m_x =  m_GoldArr[i].m_speed*sin(2*3.1415926*(m_GoldArr[i].m_Roation)/360.0)/4;
			 float m_y =  m_GoldArr[i].m_speed*cos(2*3.1415926*(m_GoldArr[i].m_Roation)/360.0)/4;
			 m_GoldArr[i].m_ptx = m_GoldArr[i].m_ptx - m_x;
			 m_GoldArr[i].m_pty = m_GoldArr[i].m_pty + m_y;

			 //判定
			 CRect m_rct;

			 m_rct.top = m_UserPT[m_GoldArr[i].m_Player].y+50;
			 m_rct.left = m_UserPT[m_GoldArr[i].m_Player].x+50;
			 if(m_GoldArr[i].m_Player>=3)
			 {
			   m_rct.right =  m_rct.left + 100;
			   m_rct.bottom =  m_rct.top + 50;
			 }
			 else
			 {
			     m_rct.right =  m_rct.left + 100;
			     m_rct.bottom =  m_rct.top + 100;
			 }
			 m_GoldArr[i].m_Point.x = m_GoldArr[i].m_ptx;
			 m_GoldArr[i].m_Point.y = m_GoldArr[i].m_pty;
			 bool is_hit =  m_rct.PtInRect(m_GoldArr[i].m_Point);
			 if(is_hit||m_GoldArr[i].m_ptx<0||m_GoldArr[i].m_ptx>CLIENT_WIDTH||m_GoldArr[i].m_pty<0||m_GoldArr[i].m_pty>CLIENT_HEIGHT)
				 m_GoldArr[i].m_Have = false;
			 

		  }
		 // CheckHit();
		
		 
	}
	//鱼儿动画序列切换
	else if(TIMERIDFISHMOVE==nIDEvent)
	{
		//首先判定自己是否是椅子最前面的那家
		int m_SmallChairID = 0;
		m_IsAndroid[m_MeChairID]=false;
	    for(int i=0;i<6;i++)
		   {
		       if(m_HaveUser[i]==false)continue;
			   if(m_IsAndroid[i]==false)
			   {
			     m_SmallChairID = m_RealChairID[i];
			     break;
			   }
		   }
	      if(m_MeRellayChairID==m_SmallChairID)
		  {
			  for(int i=0;i<GAME_PLAYER;i++)
			  { 
				    if(m_HaveUser[i]==false)continue;
					if(m_SuoFishId[i]!=0)continue;
					if(i==m_MeChairID && !m_bLockFish)
					{
						continue;
					}
		            INT_PTR nEnumIndex=m_SuoDingFishIndex;
	                Fish *tfish=NULL;
                 	do
	                 {
		                 tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		                 //枚举鱼群						
						 if (NULL==tfish)
						 {
							 m_SuoDingFishIndex = 0;
							 break;		//modified by david...20150617继续锁定。
						 }
				         if(tfish->m_Have==false)continue;
					     if(tfish->m_isHit==true)continue;
						 if((tfish->m_index>11||(tfish->m_smallFish!=-1&&tfish->m_index!=10&&tfish->m_index!=11))&&tfish->m_ptx>=(200- m_FishScale*m_FishmoveRec[tfish->m_index][0])&&tfish->m_ptx<=900&&tfish->m_pty>=(200- m_FishScale*m_FishmoveRec[tfish->m_index][1])&&tfish->m_pty<=600)
						 {
							 m_baseSink->SendLockFishID(tfish->m_fishID,m_RealChairID[i]); 
							 m_SuoDingFishIndex = nEnumIndex;
							 break;
						 }
						
					    
					  }while(true);
					
			 }
		  }

		  for(int i=0;i<10;i++)
		  {
			  if(m_FireStruc[i].m_Have==true)
			  {
				m_FireStruc[i].m_scale = m_FireStruc[i].m_scale+0.2f;
				for(int j=0;j<40;j++)
				{
					float m_x =( rand()%60+80)*sin(2*3.1415926*(m_FireStruc[i].smfire[j].m_rontion)/360.0);
	                float m_y = ( rand()%60+80)*cos(2*3.1415926*(m_FireStruc[i].smfire[j].m_rontion)/360.0);
					m_FireStruc[i].smfire[j].m_ptx = m_FireStruc[i].smfire[j].m_ptx+m_x;
					m_FireStruc[i].smfire[j].m_pty = m_FireStruc[i].smfire[j].m_pty-m_y;

				 }
				m_FireStruc[i].m_index++;
				if(m_FireStruc[i].m_index>18){m_FireStruc[i].m_Have=false;m_FireStruc[i].m_index=0;m_FireStruc[i].m_scale=1.0f;}

			  }
			 
	     }

		  //这里处理全局动画序列
		  m_ActionIndex ++ ;
		  if(m_ActionIndex>100000000)m_ActionIndex = 0;
		  INT_PTR nEnumIndex=0;
	      Fish *tfish=NULL;
          do
	        {
		      tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		       //枚举鱼群
              if (NULL==tfish) break;
			  if(tfish->m_Have==false)continue;
			  if(tfish->m_index==17)continue;
			  if(tfish->m_index==23)continue;
			  if(tfish->m_isHit)
			  {
				 
                     tfish->m_actionindex=tfish->m_actionindex+2;
					 if(tfish->m_actionindex>=m_FishmoveCount[tfish->m_index]){
						 tfish->m_Nowindex++;
						 tfish->m_actionindex=0;
					 
					 }
					if(tfish->m_Nowindex>=2)
					{
						tfish->m_Have = false;
					}
				  
			  }
			  else
			  {
                 if(!m_IsDingzhu)
				 {
			       tfish->m_actionindex++;
			       if(tfish->m_actionindex>=m_FishmoveCount[tfish->m_index])tfish->m_actionindex=0;
				 }
			  }

			}while(true);

		 
  
	}
	else if(13==nIDEvent)
	{
	      INT_PTR nEnumIndex=0;
	      Fish *tfish=NULL;
          do
	        {
		      tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		       //枚举鱼群
              if (NULL==tfish) break;
			  if(tfish->m_Have==false)continue;
			  if(tfish->m_index!=17&&tfish->m_index!=23)continue;
			  if(tfish->m_isHit)
			  {
				 
                     tfish->m_actionindex=tfish->m_actionindex+2;
					 if(tfish->m_actionindex>=m_FishmoveCount[tfish->m_index]){
						 tfish->m_Nowindex++;
						 tfish->m_actionindex=0;
					 
					 }
					if(tfish->m_Nowindex>=5)
					{
						tfish->m_Have = false;
					}
				 
				  
			  }
			  else
			  {
                 if(!m_IsDingzhu)
				 {
			       tfish->m_actionindex++;
			       if(tfish->m_actionindex>=m_FishmoveCount[tfish->m_index])tfish->m_actionindex=0;
				 }
			  }

			}while(true);
	}
	//展开子弹网子
	else if(nIDEvent == TIMERIDNETOPEN)
	{
		
		m_doudongstu.m_doucount ++;
		if(m_doudongstu.m_doucount>=30)m_doudongstu.m_have=false;
	   	  //移动自己的子弹
	     for(int i=0;i<200;i++)
		 {
			 if(m_MeBullet[i].m_Have==false)continue;
			 if(m_MeBullet[i].m_Stop==false)continue;
			 m_MeBullet[i].m_Netindex++;
			 if(m_MeBullet[i].m_Netindex>=30){m_MeBullet[i].m_Have = false;}


		 }
	
		
	}
	//展开子弹网子
	else if(nIDEvent == 101)
	{
		      

		 for(int i=0;i<USERCOUNT;i++)
		{
			//判断是否有玩家
			if(!m_HaveUser[i])continue;
			for(int j=0;j<3;j++)
			{
				if(!m_UserStruct[i][j].m_Have)continue;
				if(m_UserStruct[i][j].m_Nowscore<m_UserStruct[i][j].m_FishScore)
					m_UserStruct[i][j].m_Nowscore++;

			}
		}
	
		 //翻滚飘落金币
		 for(int i=0;i<100;i++)
		 {
			 if(m_GoldArr[i].m_Have==false)continue;
			 m_GoldArr[i].m_actionindex++;
			 if(m_GoldArr[i].m_actionindex >= 12){ m_GoldArr[i].m_actionindex = 0;}


		 }
	}
	else if(nIDEvent == CREATEFISH)
	{
		for(int i=0;i<6;i++)
		{
		  if(m_ChouJiang[i].ishave)
		      {

				  
				  m_ChouJiang[i].roloation = m_ChouJiang[i].roloation+0.01;
				  int  tempx =  m_ChouJiang[i].nowindex;
				  int  tempy =  m_ChouJiang[i].quanshu;
				  m_ChouJiang[i].nowindex = tempx -350*(0.01);
				  m_ChouJiang[i].quanshu = tempy -350*(0.01);

				  if(m_ChouJiang[i].roloation>=1.3)
				      m_ChouJiang[i].index =  m_ChouJiang[i].index + 0.5;
				  if(m_ChouJiang[i].index>=255)m_ChouJiang[i].ishave=false;

		     
		      }
		}

		if(m_ActionIndex1%6000==0)
		{
		    bgmusicindex++;
			CBassMusicManager::GetInstance()->Stop(0);
	        CBassMusicManager::GetInstance()->Stop(1);
	        CBassMusicManager::GetInstance()->Stop(2);
			if(bgmusicindex>=3)bgmusicindex=0;
	        CBassMusicManager::GetInstance()->Play(bgmusicindex, false);
		}

		  
		   FishPack *m_fishpack=NULL;
		  INT_PTR nEnumIndex = 0;
          do
	        {
		       m_fishpack=m_FishStorg.EnumActiveObject(nEnumIndex++);
		       //枚举鱼群
		       if (NULL==m_fishpack) break;
			   m_fishpack->m_CreatDelayTime--;
			   if(m_fishpack->m_CreatDelayTime<=0)
			   {
				   //
			       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0],PathIndex[m_fishpack->m_fishpathindex][0][1],PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4],m_fishpack->m_fishpathindex);
				   m_FishStorg.FreeItem(m_fishpack);
				   continue;

			   }
			   
			  
					    
		  }while(true);

	}
    //鱼儿的转换角度动画
	else if(nIDEvent == TIMERIDFISHROUND)
	{

		for(int pid = 0;pid<GAME_PLAYER;pid++)
		{
			if(m_HaveUser[pid]==false)continue;
			if(m_SuoFishId[pid]!=0)
			{
			  Fish *tfish=NULL;
			  INT_PTR nEnumIndex = 0;
			  do
				{
				   tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
				   //枚举鱼群
				   if (NULL==tfish) break;
				   if(tfish->m_Have==false)continue;
				   if(tfish->m_isHit==true)continue;
				   if(tfish->m_ptx>=(80- m_FishScale*m_FishmoveRec[tfish->m_index][0])&&tfish->m_ptx<=1100&&tfish->m_pty>=(80- m_FishScale*m_FishmoveRec[tfish->m_index][1])&&tfish->m_pty<=620&&m_SuoFishId[pid]==tfish->m_fishID)
					   break;
				  
						    
			  }while(true);
			  //处理自己炮的移动角度
			  if(tfish!=NULL)
			  {

				int m_Tempx = 0;
				int m_Tempy = 0;

				if(tfish->m_Roation<180)m_Tempx = 50;
				if(tfish->m_Roation>180)m_Tempx = -50;

				float m_nbili = m_FishScale;
				if(tfish->m_index>=17)
				{
				   m_nbili = m_FishScale22;
				}


				float px=(tfish->m_ptx +  m_nbili*m_FishmoveRec[tfish->m_index][0]/2)-(m_UserPT[pid].x+75);
				float py=(m_UserPT[pid].y+100)-(tfish->m_pty +  m_nbili*m_FishmoveRec[tfish->m_index][1]/2);
				if(pid>=3)
					py=(tfish->m_pty +  m_nbili*m_FishmoveRec[tfish->m_index][1]/2)-(m_UserPT[pid].y+100);		   
				float bili = float(px/py);
				float jiaodu=atan2(px,py)*180.0/3.1415926;
				if(jiaodu<-90)jiaodu=-90;
				if(jiaodu>90)jiaodu=90;
				if(jiaodu<0)jiaodu=360+jiaodu;
				if(pid>=3)
				  {
					  if(jiaodu>=270){jiaodu = jiaodu-180;}
					  else if(jiaodu<=90){jiaodu = 180+jiaodu;}
					  jiaodu = 360 - jiaodu;
				  }
				m_UserPaoJiaodu[pid]=jiaodu;
				for(int i=0;i<200;i++)
				{
				   if(m_MeBullet[i].m_Senduser==pid&&!m_MeBullet[i].m_Stop)
					 {
						float px=tfish->m_ptx + m_FishmoveRec[tfish->m_index][0]/2*m_nbili-20-m_MeBullet[i].m_ptx;
						//float py=m_MeBullet[i].m_pty-tfish->m_pty + m_FishmoveRec[tfish->m_index][1]/2*m_nbili;
						float py=m_MeBullet[i].m_pty-tfish->m_pty - m_nbili*m_FishmoveRec[tfish->m_index][1]/2;
						float bili = float(px/py);
						float jiaodu=atan2(px,py)*180.0/3.1415926;
						m_MeBullet[i].m_Roation=jiaodu;
					 }
				}			
				 
				for(int i=0;i<10;i++)
				{
					m_SuoPao[pid][i].m_have = true;
					int tempxj = sqrt(pow((tfish->m_pty +  m_nbili*m_FishmoveRec[tfish->m_index][1]/2-m_UserPT[pid].y-100),2)+pow((tfish->m_ptx +  m_nbili*m_FishmoveRec[tfish->m_index][0]/2-m_UserPT[pid].x-75),2));
					float m_x = tempxj/9*i*sin(2*3.1415926*(m_UserPaoJiaodu[pid])/360.0);
					float m_y = tempxj/9*i*cos(2*3.1415926*(m_UserPaoJiaodu[pid])/360.0);

					m_SuoPao[pid][i].m_ptx = m_UserPT[pid].x+90 + m_x-16;
					m_SuoPao[pid][i].m_pty = m_UserPT[pid].y+100 - m_y-16;
				}
				m_SuoPao[pid][9].m_have = true;
				m_SuoPao[pid][9].m_ptx = tfish->m_ptx+ m_nbili*m_FishmoveRec[tfish->m_index][0]/2-32;
				m_SuoPao[pid][9].m_pty = tfish->m_pty+ m_nbili*m_FishmoveRec[tfish->m_index][1]/2-32;

			  }
			  else
			  {
				 m_SuoFishId[pid] = 0;
				 for(int i=0;i<10;i++)
					m_SuoPao[pid][i].m_have = false;
			  }

			}
		}

          
		 
		 //Render();
		 m_ActionIndex1 ++;
		 if(m_ActionIndex1>100000000)m_ActionIndex1 = 0;
		// return;
	   	  //循环吧
	     Fish *tfish=NULL;
	     for(int i=0;i<m_FishFactory.GetActiveCount();i++)
	      {
		      //枚举鱼群
			  if(m_IsDingzhu)break;//如果目前定住直接弹出去
		      tfish=m_FishFactory.EnumActiveObject(i);
		      if (NULL==tfish) break;
			  if(tfish->m_Have==false)continue;
			  if(tfish->m_isHit==true)continue;

			  tfish->m_Currtime = tfish->m_Currtime+ 1;
			  if(tfish->m_traceIndex!=-1)
			  {
			    if(tfish->m_Currtime>=PathIndex[tfish->m_traceIndex][tfish->m_PtIndex][3]/m_speedBili)
			     {
			        tfish->m_PtIndex++;
				    tfish->m_Currtime = 0;
				    tfish->m_speed = PathIndex[tfish->m_traceIndex][tfish->m_PtIndex][4];
			      }
			  }
			

			 if(tfish->m_Roation!= tfish->m_RoationArr[tfish->m_PtIndex]&&-1!=tfish->m_traceIndex)
			 {
				  
				 if(tfish->m_Roation>270&&tfish->m_RoationArr[tfish->m_PtIndex]<90)
				 {
				     tfish->m_RoationArr[tfish->m_PtIndex] = 360 + tfish->m_RoationArr[tfish->m_PtIndex];
				 }

				 float tempr = (tfish->m_RoationArr[tfish->m_PtIndex] - tfish->m_Roation) / 50;
				 tfish->m_Roation = tfish->m_Roation +tempr;

			 }

			
			//中间圆圈队型鱼
			  if(m_ScienceZhuan)
			  {

				  m_scienceindex++;
				  if(m_scienceindex>=30000)
				  {
					  if(tfish->m_index<=1)

			            tfish->m_speed= 0;
						
				  }

			  }
			  else 
			  {
				  if(tfish->m_speed==0)
			         tfish->m_speed= 5;
			  }//*/
			 //如果是有附加鱼就是转圈的鱼那么就执行下面的操作		     
		  }
		for(int ci=0;ci<50;ci++)
		 {   
		    //如果已经占用循环下去
			if(m_NumArr[ci].m_Have==false)continue;
			m_NumArr[ci].m_Time++;
			if(m_NumArr[ci].m_Time>=150)
			{
			   m_NumArr[ci].m_Have=false;
			}
			 
		 }
		
			
	}
	//大鱼死后转圈和撒钱动画
   else if(nIDEvent == TIMERIDMONEYACTION)
	{
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_MuchmoneyAct[i].m_have==false)continue;
			m_MuchmoneyAct[i].m_nowindex++;
			//如果序列大于21就转换成更改图片角度
			if(m_MuchmoneyAct[i].m_nowindex>10)
			{
				//m_MuchmoneyAct[i].m_Roation = m_MuchmoneyAct[i].m_Roation + 50 ;
			}
			//如果序列大于了100那么就切换为不显示了
			if(m_MuchmoneyAct[i].m_nowindex>90)
			{
				m_MuchmoneyAct[i].m_have=false;
			  
			}			

		}
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_zhaaction[i].m_have==false)continue;
			m_zhaaction[i].m_nowindex++;
			//如果序列大于21就转换成更改图片角度
			if(m_zhaaction[i].m_nowindex>10)
			{
				//m_zhaaction[i].m_Roation = m_zhaaction[i].m_Roation + 15 ;
			}
			//如果序列大于了100那么就切换为不显示了
			if(m_zhaaction[i].m_nowindex>90)
			{
				m_zhaaction[i].m_have=false;
			  
			}			

		}
		if(m_DingStruc.m_have == true)
		{
			m_DingStruc.m_nowindex++;
			if(m_DingStruc.m_nowindex>60)m_DingStruc.m_have=false;
		}
	    
	}
	//玩家钱堆移动，就是旁边打死鱼儿后的3堆钱
   else if(nIDEvent == TIMERIDUSERMONEY)
	{
		for(int i=0;i<USERCOUNT;i++)
		{
			//判断是否有玩家
			if(!m_HaveUser[i])continue;
			for(int j=0;j<3;j++)
			{
				if(!m_UserStruct[i][j].m_Have)continue;
				if(j==0&&m_UserStruct[i][1].m_Have&&m_UserStruct[i][j].m_Time>=50)
				{
					m_UserStruct[i][j].m_Have = false;
					m_UserStruct[i][j].m_Have =  m_UserStruct[i][j+1].m_Have;
					m_UserStruct[i][j].m_Money =  m_UserStruct[i][j+1].m_Money;
					m_UserStruct[i][j].m_colorindex =  m_UserStruct[i][j+1].m_colorindex;
					m_UserStruct[i][j].m_Time =  0;
					m_UserStruct[i][j].m_FishScore = m_UserStruct[i][j+1].m_FishScore;
		            m_UserStruct[i][j].m_Nowscore =  m_UserStruct[i][j+1].m_Nowscore;
					m_UserStruct[i][j].m_Have = true;
					m_UserStruct[i][1].m_Have = false;

					if(m_UserStruct[i][2].m_Have)
					{
					   m_UserStruct[i][1].m_Have = true;
                       m_UserStruct[i][1].m_Have =  m_UserStruct[i][2].m_Have;
					   m_UserStruct[i][1].m_colorindex =  m_UserStruct[i][2].m_colorindex;
					   m_UserStruct[i][1].m_Money =  m_UserStruct[i][2].m_Money;
					   m_UserStruct[i][1].m_FishScore = m_UserStruct[i][2].m_FishScore;
		               m_UserStruct[i][1].m_Nowscore =  m_UserStruct[i][2].m_Nowscore;
					   m_UserStruct[i][1].m_Time =  0;
					   m_UserStruct[i][2].m_Have = false;

					}
				   
				}
				else if(j==0&&!m_UserStruct[i][1].m_Have&&m_UserStruct[i][j].m_Time>=50)
				{
					m_UserStruct[i][j].m_Have = false;
				}
				m_UserStruct[i][j].m_Time++;
				
			    
			}
			
		}
	    
	}
	//场景切换动画
   else if(TIMERIDCHANGESCREEN==nIDEvent)
   {

	   m_FishStorg.FreeAllItem();
	   m_ChanwaveSt.m_currImgIndex = 1-m_ChanwaveSt.m_currImgIndex;
	   m_ChanwaveSt.m_ptx = m_ChanwaveSt.m_ptx-20;
	   m_ChanwaveSt.m_pty = 0;
	   m_ChanwaveSt.m_Time++;
	   if(m_ChanwaveSt.m_ptx<-500)
	   {
		   //如果已经超出边缘那么停止动画
		   m_FishFactory.FreeAllItem();
	       m_IsChangeScreen = false;
		   KillTimer(TIMERIDCHANGESCREEN);
	   }
	   	//把鱼消失
	Fish *tfish=NULL;
	INT_PTR nEnumIndex = 0;
    do
	  {
		       tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		       //枚举鱼群
		       if (NULL==tfish) break;
			   if(tfish->m_ptx>= m_ChanwaveSt.m_ptx)
				   m_FishFactory.FreeItem(tfish);

		  
					    
	  }while(true);
	  
       
   }
   //发送炮弹
   else if(TIMERIDSENDSHOOT==nIDEvent)
   {
	    //发射子弹
	   if(m_UserScore[m_MeChairID]>0&&!m_isxiuyuqi)
		{
	      	 DWORD m_BulletID = GetTickCount();
			 m_baseSink->OnShoot(false,m_UserPaoJiaodu[m_MeChairID],m_BulletID);
			 UserShoot(m_MeChairID,m_UserPaoJiaodu[m_MeChairID],m_MeRellayChairID,false,m_BulletID);
		}
       else
		{
		   KillTimer(TIMERIDSENDSHOOT);
		}
   }
    
   else if(100==nIDEvent)
   {
	   if(m_InitScreen)
	   { 
		    if(!m_IsFullScreen)
		       m_IsIntro = true;
		    SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR1)));
		   //回调提示已经加再完成
		   m_baseSink->InitClientMessage();
		   //连接服务
	       KillTimer(100);
	   
	   }
     
   }
   else if(TIMERIDOUT==nIDEvent)
   {
	    	//把鱼消失
	    Fish *tfish=NULL;
	    INT_PTR nEnumIndex = 0;
        do
	      {
		       tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		       //枚举鱼群
		       if (NULL==tfish) break;
			   if(tfish->m_index==22)
			   {
				   if(tfish->m_LiKuiScore<350)
				      tfish->m_LiKuiScore= tfish->m_LiKuiScore + 3;

				    if(tfish->m_LiKuiScore>350)
						tfish->m_LiKuiScore = 300;

			   }

				   

		  
					    
	    }while(true); 

       m_OutTime--;
	   if(m_OutTime<=60)
	   {
	      m_IsMustOut =  true;
	   }
	   else
	   {
	      m_IsMustOut =  false;
	   }
	   //退出
	   if( m_OutTime<=0)
	   {
	     m_baseSink->CloseExit();
	   }
   }
   else if(TIMERIDDING==nIDEvent)
   {
       m_IsDingzhu =  false;
	   KillTimer(TIMERIDDING);
   }
	else if (TIMERIDALWAYSSHOOT == nIDEvent)
	{
		if(m_UserScore[m_MeChairID]<=0)return;
		if(m_BeiLv[m_MeChairID]>m_UserScore[m_MeChairID]) return; 
		//需要过滤潮汐期间/休渔期
		if(m_isxiuyuqi)
			return ;
		m_baseSink->OnShoot(false,m_UserPaoJiaodu[m_MeChairID],m_bLockShoot);
		UserShoot(m_MeChairID,m_UserPaoJiaodu[m_MeChairID],m_MeRellayChairID,false,m_bLockShoot);
		m_NowTime = GetTickCount();
	}	
	CWnd::OnTimer(nIDEvent);
}
//玩家进入或者出去
void GameScreenWnd::UserComeInorLeave(int PlayerID,bool ComeOrLeave,int RealUserid)
{  
	m_IsAndroid[PlayerID]=true;
	m_PowerCount[PlayerID]=0;
	//设置桌位是否有玩家
	m_HaveUser[PlayerID] = ComeOrLeave;
	m_RealChairID[PlayerID]=RealUserid;
	if(ComeOrLeave)
	{
	   m_BeiLv[PlayerID] = m_CellScore;
	   m_UserScore[PlayerID] = 0;
	}
   
}
//自己信息
void GameScreenWnd:: SetMeInformation(int MePlayerid,int MeRellid ,LPCTSTR myname , DOUBLE myscore)
{
	//赋值信息
    m_Username = myname;
	m_MyScore = myscore;
	m_MeChairID = MePlayerid;
	m_MeRellayChairID = MeRellid;
}
//清理所有数据内存，退出时候使用
void GameScreenWnd::CleanAllMemory()
{

}
void GameScreenWnd::CreatRegFishTwo(FishPack *m_fishpack)
{
	m_isxiuyuqi = false;
	if(m_fishpack->m_CreatDelayTime<120&&m_fishpack->m_fishtype<=1)
		{
		  //  int tempa[]
			int ptx = -400 + m_fishpack->m_CreatDelayTime%120*15;
		    int pty = 700  ;
		    AddFish(ptx,pty,0,900001008,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
	if(m_fishpack->m_CreatDelayTime>=120&&m_fishpack->m_CreatDelayTime<240&&m_fishpack->m_fishtype<=1)
		{
		
			int ptx = -400 + m_fishpack->m_CreatDelayTime%120*15;
		    int pty = 20 ;
		    AddFish(ptx,pty,180,900001008,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
	if(m_fishpack->m_fishtype>1)
	{
		
		int ptx = -450;
		int pty = 360 ;
		if(m_fishpack->m_CreatDelayTime%2==0)
			pty = 200 ;
		AddFish(ptx,pty,90,900001008,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
		m_scienceindex = 0;
	}

}
void GameScreenWnd::CreatRegFishOne(FishPack *m_fishpack)
{
	   m_isxiuyuqi = false;
       if(m_fishpack->m_CreatDelayTime<120)
		{
			int m_Roloation = 360/120*(m_fishpack->m_CreatDelayTime%120);
		    float m_x = 22*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 22*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -300 + (45+(m_fishpack->m_CreatDelayTime/120)*15) * m_x;
		    int pty = 340 - (45+(m_fishpack->m_CreatDelayTime/120)*15) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}

		if(m_fishpack->m_CreatDelayTime>=120&&m_fishpack->m_CreatDelayTime<160)
		{
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x = 6*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 6*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -220 + (45+(m_fishpack->m_CreatDelayTime/40)*5) * m_x;
		    int pty = 250 - (45+(m_fishpack->m_CreatDelayTime/40)*5) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
		if(m_fishpack->m_CreatDelayTime>=160&&m_fishpack->m_CreatDelayTime<200)
		{
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x =6*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 6*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -220 + (45+(3)*5) * m_x;
		    int pty = 410 - (45+(3)*5) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
		if(m_fishpack->m_CreatDelayTime>=200&&m_fishpack->m_CreatDelayTime<240)
		{
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x =6*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 6*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -390 + (45+(3)*5) * m_x;
		    int pty = 250 - (45+(3)*5) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
		if(m_fishpack->m_CreatDelayTime>=240&&m_fishpack->m_CreatDelayTime<300)
		{
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x =6*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 6*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -390 + (45+(3)*5) * m_x;
		    int pty = 410 - (45+(3)*5) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
		if(m_fishpack->m_CreatDelayTime==301)
		{
		    AddFish(-400,160,90,900000009,-1,21,0,0,m_fishpack->m_fishid,-1,4);
		}
	     

}

void GameScreenWnd::CreatRegFishThree(FishPack *m_fishpack)
{
	    m_isxiuyuqi = false;
        if(m_fishpack->m_CreatDelayTime<40)
		{
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x = 18*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 18*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -300 + (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_x;
		    int pty = 340 - (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
		
		if(m_fishpack->m_CreatDelayTime>=40&&m_fishpack->m_CreatDelayTime<80)
		{
			m_fishpack->m_CreatDelayTime = m_fishpack->m_CreatDelayTime -40;
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x = 18*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 18*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -450 + (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_x;
		    int pty = 340 - (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
		if(m_fishpack->m_CreatDelayTime>=80&&m_fishpack->m_CreatDelayTime<120)
		{
			m_fishpack->m_CreatDelayTime = m_fishpack->m_CreatDelayTime -80;
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x = 18*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 18*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -1000 + (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_x;
		    int pty = 340 - (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
		if(m_fishpack->m_CreatDelayTime>=120&&m_fishpack->m_CreatDelayTime<160)
		{
			m_fishpack->m_CreatDelayTime = m_fishpack->m_CreatDelayTime -120;
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x = 18*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 18*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -1150 + (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_x;
		    int pty = 340 - (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
		if(m_fishpack->m_CreatDelayTime>=160&&m_fishpack->m_CreatDelayTime<200)
		{
			m_fishpack->m_CreatDelayTime = m_fishpack->m_CreatDelayTime -160;
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x = 18*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 18*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -1700 + (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_x;
		    int pty = 340 - (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
		if(m_fishpack->m_CreatDelayTime>=200&&m_fishpack->m_CreatDelayTime<240)
		{
			m_fishpack->m_CreatDelayTime = m_fishpack->m_CreatDelayTime -200;
			int m_Roloation = 360/40*(m_fishpack->m_CreatDelayTime%40);
		    float m_x = 18*sin(2*3.1415926*m_Roloation/360.0)/3.5;
		    float m_y = 18*cos(2*3.1415926*m_Roloation/360.0)/3.5;
		    int ptx = -1850 + (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_x;
		    int pty = 340 - (45+(m_fishpack->m_CreatDelayTime/40)*15) * m_y;
		    AddFish(ptx,pty,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
			
		}
        if(m_fishpack->m_CreatDelayTime==241)
		{
		    AddFish(-1150,250,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
		}
		if(m_fishpack->m_CreatDelayTime==240)
		{
		    AddFish(-490,250,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
		}
		if(m_fishpack->m_CreatDelayTime==242)
		{
		    AddFish(-2000,160,90,900000009,-1,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,-1,4);
		}

		

}
void GameScreenWnd::AddLiKuiScore()
{
  
	  
}
void GameScreenWnd::AddStorgeFish(FishPack *m_fishpack)
{

   //***大三元
	if(m_fishpack->m_CreatDelayTime==0)
	{

	    AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0],PathIndex[m_fishpack->m_fishpathindex][0][1],PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
	}
	else if(m_fishpack->m_CreatDelayTime==1)
	{
		if(m_fishpack->m_fishpathindex<20)
		{
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]+120,PathIndex[m_fishpack->m_fishpathindex][0][1],PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
		else 
		{
		   	AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0],PathIndex[m_fishpack->m_fishpathindex][0][1]+120,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
	}
	else  if(m_fishpack->m_CreatDelayTime==2)
	{
	   if(m_fishpack->m_fishpathindex<20)
		{
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]+240,PathIndex[m_fishpack->m_fishpathindex][0][1],PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
		else 
		{
		   	AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0],PathIndex[m_fishpack->m_fishpathindex][0][1]+240,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
	} //***大三元结束
	//***大四喜
	else if(m_fishpack->m_CreatDelayTime==3)
	{

	     AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0],PathIndex[m_fishpack->m_fishpathindex][0][1],PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
	}
	else if(m_fishpack->m_CreatDelayTime==4)
	{
		if(m_fishpack->m_fishpathindex<20)
		{
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]-70,PathIndex[m_fishpack->m_fishpathindex][0][1]+70,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
		else 
		{
		   	AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]-70,PathIndex[m_fishpack->m_fishpathindex][0][1]-70,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
	}
	else  if(m_fishpack->m_CreatDelayTime==5)
	{
	   if(m_fishpack->m_fishpathindex<20)
		{
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]+70,PathIndex[m_fishpack->m_fishpathindex][0][1]+70,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
		else 
		{
		   	AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]-70,PathIndex[m_fishpack->m_fishpathindex][0][1]+70,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
	}
	else  if(m_fishpack->m_CreatDelayTime==6)
	{
	   if(m_fishpack->m_fishpathindex<20)
		{
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0],PathIndex[m_fishpack->m_fishpathindex][0][1]+130,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
		else 
		{
		   	AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]-130,PathIndex[m_fishpack->m_fishpathindex][0][1],PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		}
	} //***大四喜结束
	else  if(m_fishpack->m_CreatDelayTime==10)
	{
	 
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0],PathIndex[m_fishpack->m_fishpathindex][0][1],PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		
	}
	else  if(m_fishpack->m_CreatDelayTime==11)
	{
	  
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]+10,PathIndex[m_fishpack->m_fishpathindex][0][1]+50,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		
	}
	else  if(m_fishpack->m_CreatDelayTime==12)
	{
	 
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]+20,PathIndex[m_fishpack->m_fishpathindex][0][1]+80,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		
	}
	else  if(m_fishpack->m_CreatDelayTime==13)
	{
	  
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]-20,PathIndex[m_fishpack->m_fishpathindex][0][1]+120,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		
	}
	else  if(m_fishpack->m_CreatDelayTime==14)
	{
	  
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]-30,PathIndex[m_fishpack->m_fishpathindex][0][1]+60,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);
		
	}
	else  if(m_fishpack->m_CreatDelayTime==15)
	{
	   
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]-10,PathIndex[m_fishpack->m_fishpathindex][0][1]+100,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
					   PathIndex[m_fishpack->m_fishpathindex][0][4]);
	}
	else  if(m_fishpack->m_CreatDelayTime==16)
	{
	  
	       AddFish(PathIndex[m_fishpack->m_fishpathindex][0][0]-1,PathIndex[m_fishpack->m_fishpathindex][0][1]+140,PathIndex[m_fishpack->m_fishpathindex][0][2],
					   PathIndex[m_fishpack->m_fishpathindex][0][3],m_fishpack->m_fishpathindex,m_fishpack->m_fishtype,0,0,m_fishpack->m_fishid,m_fishpack->m_fudaifishtype,
						   PathIndex[m_fishpack->m_fishpathindex][0][4]);

	}
	else 
	{
		FishPack *tempFish=m_FishStorg.ActiveItem();
	    if (NULL==tempFish) return ;
	    CopyMemory(tempFish,m_fishpack,sizeof(FishPack));
	}

}
//添加鱼儿
//int aa=0;
void GameScreenWnd::AddFish(int traceX,int traceY,float roation,float movetime,float changetime,int fishtype,int ptindex,int nowindex,int fishid,int smallfishtype,int fishspeed,int likuiscore)
{
	  if(fishtype>FISHCOUNT-1)return;
	 if(movetime==900001008)m_ScienceZhuan=true;
	// if(fishtype==15)fishtype=25; if(fishtype==16)fishtype=26; if(fishtype==17)fishtype=27;
	 // if(fishtype==18)fishtype=28; if(fishtype==19)fishtype=29; if(fishtype==20)fishtype=30;
	// if(m_ScienceZhuan==true&&movetime!=900090008)
	// {
	    //AfxMessageBox(L"d") 
	// }
	 else m_ScienceZhuan=false;
	 int tx = traceX;
	 int ty = traceY;
	 float m_roation = roation;
	 if(m_MeRellayChairID>=3)
	   {
	          float temp = roation+180;
			  if(temp>360)temp=temp-360;
			   //继续判定
			  if(temp<0)temp=360-abs(temp);
	          
	          if(traceX<0&&traceY<0)
	            {
	                 tx = CLIENT_WIDTH +abs(traceX)-m_FishmoveRec[fishtype][0]*m_FishScale;
		             ty = CLIENT_HEIGHT +abs(traceY)-m_FishmoveRec[fishtype][1]*m_FishScale;

	             }
	          else  if(traceX<0&&traceY>0)
	             {
	                 tx = CLIENT_WIDTH +abs(traceX)-m_FishmoveRec[fishtype][0]*m_FishScale;
		             ty = CLIENT_HEIGHT -abs(traceY)-m_FishmoveRec[fishtype][1]*m_FishScale;

	            }
			   else  if(traceX>0&&traceY>0)
	             {
	                 tx = CLIENT_WIDTH -abs(traceX)-m_FishmoveRec[fishtype][0]*m_FishScale;
		             ty = CLIENT_HEIGHT-abs(traceY)-m_FishmoveRec[fishtype][1]*m_FishScale;

	            }
			   else  if(traceX>0&&traceY<0)
	             {
	                 tx = CLIENT_WIDTH -abs(traceX)-m_FishmoveRec[fishtype][0]*m_FishScale;
		             ty = CLIENT_HEIGHT +abs(traceY)-m_FishmoveRec[fishtype][1]*m_FishScale;

	            }
				m_roation = temp;

	   }
	   //如果已经到了第4个轨迹坐标鱼就活了
	  //  if(nowindex==4||movetime==900000009||900090009==movetime)
	      if(fishtype==12)
		  {
		     CString  str;
			 str.Format(L"%d,%d",tx,ty);
			// AfxMessageBox(str);
		  }
		  bool isHace = false;
          if(!isHace)
		  {

			 //获取鱼类
	         Fish *tempFish=m_FishFactory.ActiveItem();
	         if (NULL==tempFish) return ;
			 tempFish->m_actionindex = 0;
		     tempFish->m_Nowindex = 0;
		     tempFish->m_HitUser = -1;
	         tempFish->m_Currtime = 0;
	         tempFish->m_fishID = fishid;// m_FishTrace[ptindex][0].m_fishid;
	         tempFish->m_Have = true;
	         tempFish->m_index = fishtype;//m_FishTrace[ptindex][0].m_fishtype;
	         tempFish->m_PtIndex = 0;
		     tempFish->m_isHit = false;
	         tempFish->m_ptx = tx;// m_FishTrace[ptindex][0].x - m_FishmoveRec[m_FishTrace[ptindex][0].m_fishtype][0]/2 ;
	         tempFish->m_pty = ty;// m_FishTrace[ptindex][0].y - m_FishmoveRec[m_FishTrace[ptindex][0].m_fishtype][1]/2;
	         tempFish->m_Roation = m_roation;//m_FishTrace[ptindex][0].rotation;
	         tempFish->m_speed = fishspeed;//m_FishTrace[ptindex][0].m_speed;
		     tempFish->m_smallFish = 0;
	         tempFish->m_smallFish = smallfishtype;//m_FishTrace[ptindex][0].m_fudaifishtype;
			 tempFish->m_traceIndex = changetime;
			 tempFish->m_creatTime = GetTickCount();
			 tempFish->m_LiKuiScore =  300;
			 for(int i=0;i<5;i++)
			 {
			    if(m_MeRellayChairID>=3)
	                {
	                   float temp = PathIndex[int(changetime)][i][2]+180;
			           if(temp>360)temp=temp-360;
			           //继续判定
			           if(temp<0)temp=360-abs(temp);
					   tempFish->m_RoationArr[i] = temp;
					}
				else
				{
					tempFish->m_RoationArr[i] = PathIndex[int(changetime)][i][2];
				     
				}
			 }		 						
		  } 
       
}

void GameScreenWnd::CheckHit()
{
	if(m_InitScreen==false)return;
    //判断子弹是否击中了鱼
	try{
	     Fish *tfish=NULL;
         for(int i=0;i<200;i++)
		 {
                if(m_MeBullet[i].m_Have==false)continue;
	            INT_PTR nEnumIndex=0;
	            Fish *tfish=NULL;
	           // bool bShoot=false;
               	do
	               {

		            tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		            //枚举鱼群


		            if (NULL==tfish) break;
				    if(tfish->m_Have==false)continue;
					if(tfish->m_isHit==true)continue;
					//if(tfish->m_smallFish==10||tfish->m_smallFish==12)continue;

					//判定是否锁定到鱼
					if(m_SuoFishId[m_MeBullet[i].m_Senduser]!=0)
					{
						if(m_SuoFishId[m_MeBullet[i].m_Senduser]!=tfish->m_fishID)continue;
					}
					//if(GetTickCount()-tfish->m_creatTime>50000)continue;
                     //创建不规则区域
	                CRgn  nit;
	                //定义四个顶点坐标
	                CPoint VertexPoint[4];
	                //算出第一个坐标点
	                long x2;
					float co;
					float si;
				
					int pianyijiaodu = 0;
		            pianyijiaodu=-90;
	                //
	                co=cos(2*3.1415926*(tfish->m_Roation+pianyijiaodu)/360.0);
	                si=sin(2*3.1415926*(tfish->m_Roation+pianyijiaodu)/360.0);

					int m_TempWidth  =  m_FishmoveRec[tfish->m_index][0]*m_FishScale;
					int m_TempHeigth =  m_FishmoveRec[tfish->m_index][1]*m_FishScale;

                   if(m_TempWidth<80)
						m_TempWidth = 80;

					if(m_TempHeigth<80)
						m_TempHeigth =80;

	                 //旋转的中心点
	                CPoint Center;
	                //中心点就是这个鱼X坐标+这条鱼的宽度的一半
	                Center.x= tfish->m_ptx + m_TempWidth/2;
	                Center.y= tfish->m_pty + m_TempHeigth/2;
					//根据鱼儿返回比例
					int m_BliCountx = 2;
					int m_BliCounty = 2;

	                //即将要旋转的点
	                CPoint Center1;
	                Center1.x= Center.x - m_TempWidth/m_BliCountx;
	                Center1.y= Center.y - m_TempHeigth/m_BliCounty;
                    x2=Center1.x;
                    VertexPoint[0].x=(long)((double)(Center1.x-Center.x)*co-(double)(Center1.y-Center.y)*si+Center.x);
                    VertexPoint[0].y=(long)((double)(x2-Center.x)*si+(double)(Center1.y-Center.y)*co+Center.y);

	                Center1.x= Center.x - m_TempWidth/m_BliCountx;
	                Center1.y= Center.y + m_TempHeigth/m_BliCounty;
                    x2=Center1.x;
                    VertexPoint[1].x=(long)((double)(Center1.x-Center.x)*co-(double)(Center1.y-Center.y)*si+Center.x);
                    VertexPoint[1].y=(long)((double)(x2-Center.x)*si+(double)(Center1.y-Center.y)*co+Center.y);

	                Center1.x= Center.x + m_TempWidth/m_BliCountx;
	                Center1.y= Center.y + m_TempHeigth/m_BliCounty;
                    x2=Center1.x;
                    VertexPoint[2].x=(long)((double)(Center1.x-Center.x)*co-(double)(Center1.y-Center.y)*si+Center.x);
                    VertexPoint[2].y=(long)((double)(x2-Center.x)*si+(double)(Center1.y-Center.y)*co+Center.y);

	                Center1.x= Center.x + m_TempWidth/m_BliCountx;
	                Center1.y= Center.y - m_TempHeigth/m_BliCounty;
                    x2=Center1.x;
                    VertexPoint[3].x=(long)((double)(Center1.x-Center.x)*co-(double)(Center1.y-Center.y)*si+Center.x);
                    VertexPoint[3].y=(long)((double)(x2-Center.x)*si+(double)(Center1.y-Center.y)*co+Center.y);
                    nit.CreatePolygonRgn(VertexPoint, 4, ALTERNATE);

					m_MeBullet[i].m_Point.x = m_MeBullet[i].m_ptx+32;
					m_MeBullet[i].m_Point.y = m_MeBullet[i].m_pty+32;

					//判断子弹是否在枪口后面
					CRect m_rect;
					m_rect.left = m_UserPT[m_MeBullet[i].m_Senduser].x+30;
 				    m_rect.top =  m_UserPT[m_MeBullet[i].m_Senduser].y+10;
					m_rect.right = m_rect.left + 100;
				    m_rect.bottom = m_rect.top + 190;
					bool is_No =m_rect.PtInRect(m_MeBullet[i].m_Point);

			
					//获取鱼儿区域完毕
					//开始获取子弹区域       
                    bool is_hit =  nit.PtInRegion(m_MeBullet[i].m_Point);	
					bool fishin = false;
					if(tfish->m_ptx>0&&tfish->m_ptx<1248&&tfish->m_pty>0&&tfish->m_pty<768)fishin=true;
					nit.DeleteObject();
					if(is_hit&&m_MeBullet[i].m_Stop==false&&!is_No)
					{
						//播放声音
						MP_Manager& MP=MP_Manager::GetInstance();
                        MP_Emitter* emitter = NULL;
						char  str[255];
						//str.Format(L"But%d",i);
						sprintf(str,"But%d",i);
	                    emitter=MP.GetEmitterByName(str);
	                    if (emitter != NULL)
		                    {
                                  MP_POSITION pos;
                                  emitter->GetPosition(pos);
                                  pos.x = m_MeBullet[i].m_Point.x-20;
			                      pos.y = m_MeBullet[i].m_Point.y-30;
                                  emitter->Move(pos);
			                      emitter->SetState(MAGIC_STATE_STOP);
			                      emitter->SetState(MAGIC_STATE_UPDATE);
	                         }
						if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(13, false);
						m_MeBullet[i].m_Netindex = 0;
						m_MeBullet[i].m_Stop = true;
						//设置被捕获鱼儿数据
                        //如果这个子弹是我发射的
						if((m_MeBullet[i].m_Senduser==m_MeChairID||m_MeBullet[i].m_isAndroid))
						{
							m_SendTimeN = GetTickCount();
						   //发送消息到服务端
							m_baseSink->HitFirsh(tfish->m_fishID,i,m_MeBullet[i].m_RealUser,m_MeBullet[i].m_isAndroid,m_MeBullet[i].m_ZiDanID);
							
							//bShoot=true;
							//中断判断
			              	break;
						}
						
						
					}
				  

			   }while(true);

		 }
	 }
	catch(...)
	{
	 // AfxMessageBox(L"游戏出错，可能影响游戏运行，请联系开发者QQ：56248");
	}
}
void GameScreenWnd::LoadSound()
{
    CBassMusicManager::GetInstance()->Init(this->m_hWnd);
	//载入声音
	//背景声音
	CBassMusicManager::GetInstance()->LoadFromFile(0, "SXfish/music/bg_01.mp3",true);
	CBassMusicManager::GetInstance()->LoadFromFile(1, "SXfish/music/bg_02.mp3",true);
	CBassMusicManager::GetInstance()->LoadFromFile(2, "SXfish/music/bg_03.mp3",true);
    //发送子弹声音
	CBassMusicManager::GetInstance()->LoadFromFile(51, "SXfish/music/shot8.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(61, "SXfish/music/shot_8.mp3",false);

	//开网声音
    CBassMusicManager::GetInstance()->LoadFromFile(13, "SXfish/music/net_8.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(14, "SXfish/music/net_1.mp3",false);
	//加钱声音
    CBassMusicManager::GetInstance()->LoadFromFile(15, "SXfish/music/jia.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(16, "SXfish/music/jia1.mp3",false);
	//死亡声音
	CBassMusicManager::GetInstance()->LoadFromFile(20, "SXfish/music/die00.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(21, "SXfish/music/die01.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(22, "SXfish/music/die02.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(23, "SXfish/music/die03.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(24, "SXfish/music/die04.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(25, "SXfish/music/die05.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(26, "SXfish/music/die06.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(27, "SXfish/music/die07.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(28, "SXfish/music/die07.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(29, "SXfish/music/silver.wav",false);
	//
	CBassMusicManager::GetInstance()->LoadFromFile(71, "SXfish/music/laserShot.mp3",false);
	//切换场景声音
    CBassMusicManager::GetInstance()->LoadFromFile(40, "SXfish/music/senceConver.mp3",false);
	//大死声音
    CBassMusicManager::GetInstance()->LoadFromFile(100, "SXfish/music/b_0.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(101, "SXfish/music/b_1.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(102, "SXfish/music/b_2.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(103, "SXfish/music/b_3.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(104, "SXfish/music/b_4.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(105, "SXfish/music/b_5.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(106, "SXfish/music/b_6.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(107, "SXfish/music/b_7.mp3",false);

	CBassMusicManager::GetInstance()->LoadFromFile(200, "SXfish/music/g_0.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(201, "SXfish/music/g_1.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(202, "SXfish/music/g_2.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(203, "SXfish/music/g_3.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(204, "SXfish/music/g_4.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(205, "SXfish/music/g_5.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(206, "SXfish/music/g_6.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(207, "SXfish/music/g_7.mp3",false);

	CBassMusicManager::GetInstance()->LoadFromFile(43, "SXfish/music/superarm.wav",false);
	CBassMusicManager::GetInstance()->LoadFromFile(42, "SXfish/music/silver.wav",false);
	CBassMusicManager::GetInstance()->SetVolumn(100);
	CBassMusicManager::GetInstance()->Play(rand()%3, false);
}
void GameScreenWnd::LoadNorImg(LPCWSTR m_path,LPDIRECT3DTEXTURE9* ppTexture)
{
   do{

	    if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,m_path,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffff000,NULL,NULL,ppTexture)))
	            break;

	  }while(true);
}
//载入图片纹理
void GameScreenWnd::LoadImg(const char* m_path,LPDIRECT3DTEXTURE9* ppTexture)
{
                unsigned long nSize = 0;
                FILE *fp = fopen(m_path, "rb");
	            fseek(fp,0,SEEK_END);
				//获取大小
                nSize = ftell(fp);
                //创件缓存区域
                char * buffer = new char[4500000];
				//区域
				FilesParmesA * PngBuffdata = (FilesParmesA *)buffer;
                //读取数据
				fseek(fp,0,SEEK_SET);
                nSize = fread(PngBuffdata,sizeof(char), nSize,fp);
                fclose(fp);
                //拷贝图片信息到区域
				fseek(fp,0,SEEK_SET);
                //创建复制数据内存
				unsigned char* pBuffer = new unsigned char[PngBuffdata->m_PngSize];
				memcpy(pBuffer, PngBuffdata + 1, PngBuffdata->m_PngSize);
				nSize = PngBuffdata->m_PngSize;
				do{

	               if(!FAILED(D3DXCreateTextureFromFileInMemory(m_pd3dDevice,pBuffer,nSize,ppTexture)))
					   break;

				}while(true);
				delete pBuffer;
				delete buffer;
				
}
void GameScreenWnd::AddFire(int px,int py)
{
	for(int i=0;i<10;i++)
		{

			if(m_FireStruc[i].m_Have==false)
			{
				m_FireStruc[i].m_scale=0.5f;
			    m_FireStruc[i].m_Have=true;
				for(int j=0;j<40;j++)
				{
					m_FireStruc[i].smfire[j].m_ptx = px+100*(i%5);
					m_FireStruc[i].smfire[j].m_pty = py+100*(i%5);
					m_FireStruc[i].smfire[j].m_rontion =9*j;

				}
				break;
			}
			 
		}
    
}
//
void GameScreenWnd::SetFontString(LPCWSTR WString)
{
	for(int i=0;i<5;i++)
		{
		  if(m_FontCurr[i].m_Have==false)
	        {
	           lstrcpyn(m_FontCurr[i].m_Infomation,WString,500);
	           m_FontCurr[i].m_Curr=CLIENT_WIDTH;
	           m_FontCurr[i].m_Have=true;
			   break;
	   

	        }
		}	
}


//字符转换
BOOL WCharToMByte(LPCWSTR lpcwszStr, string &str) 
{  
	DWORD dwMinSize = 0;   
	LPSTR lpszStr = NULL;  
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);   
	if(0 == dwMinSize) 
   { 
	   return FALSE;  
   }  
	lpszStr = new char [dwMinSize];
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwMinSize,NULL,FALSE); 
	str = lpszStr;   
	delete [] lpszStr;  
	return TRUE;  
}
void GameScreenWnd::LoadNorImg1(LPDIRECT3DTEXTURE9* ppTexture, LPCWSTR filename)
{
	string tempurl;
	WCharToMByte(filename,tempurl);
	unzipFile(tempurl);
	CString filepath;
	filepath.Format(L"%s%s",DIRPATH,filename);
	OutputDebugString(filepath);
   do{
	    if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,filepath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffff000,NULL,NULL,ppTexture)))
		{
			break;
		}

	 }while(true);

   DeleteFile(filepath);
}

//解压资源
#define C1 528459
#define C2 227191

bool GameScreenWnd::unzipFile(string filenamea)
{
   HZIP hz;
  //载入密码读资源

  CString Result,str,S;
  int i,j;
  WORD Key=56248;
  S=L"QQ44224133QQ" ;
  Result=S;
  for(i=0; i<S.GetLength(); i++) 
    {  
       Result.SetAt(i, S.GetAt(i)^(Key>>8));
       Key = ((BYTE)Result.GetAt(i)+Key)*C1+C2;
    }
    S=Result; 
    Result.Empty();
    for(i=0; i<S.GetLength(); i++) 
    {
      j=(BYTE)S.GetAt(i);
      str="12"; 
      str.SetAt(0, 97+j/26);
      str.SetAt(1, 97+j%26);
      Result += str;
    }

  DWORD dwMinSize = 0;   
  LPSTR lpszStr = NULL;  
  LPCWSTR   m_Password1=Result;
  dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,m_Password1,-1,NULL,0,NULL,FALSE);   
  if(0 == dwMinSize) 
	   { 
		   return FALSE;  
	   }  
  lpszStr = new char [dwMinSize];
  WideCharToMultiByte(CP_OEMCP,NULL,m_Password1,-1,lpszStr,dwMinSize,NULL,FALSE); 
  hz = OpenZip(L"SXfish/Res", lpszStr);
  OutputDebugStringA(lpszStr);
  delete [] lpszStr;  
  //结束读取

  ZIPENTRY ze; GetZipItem(hz,-1,&ze); 
  int numitems=ze.index;
  for (int zi=0; zi<numitems; zi++)
  {
	GetZipItem(hz,zi,&ze);
	string tempurl;
	WCharToMByte(ze.name,tempurl);
	if(filenamea==tempurl)
      UnzipItem(hz,zi,ze.name);	
  }
  CloseZip(hz);
  return 1;
}

CPoint GameScreenWnd::drawing(CPoint a,CPoint b,int c)//该函数实现将点a绕点b旋转c度角
{
  float h=float(c*3.14/180);//角度化成弧度
  CPoint newpoint;
  newpoint.x=long(b.x*cos(h)-b.y*sin(h)+(1-cos(h))*a.x+a.y*sin(h));
  newpoint.y=long(b.x*sin(h)+b.y*cos(h)+(1-cos(h))*a.y-a.x*sin(h));//核心旋转变换矩阵
  return newpoint;
}
void GameScreenWnd::SetFullScreen(bool m_isfull)
{
    m_IsFullScreen = m_isfull;
	if(m_isfull)
	{

	  m_configDlg.m_isvisable = false;
	  m_IsIntro = false;
	  HDC screenDC;screenDC = CreateDC(L"DISPLAY",NULL, NULL, NULL);int xRes = GetDeviceCaps(screenDC, HORZRES);int yRes = GetDeviceCaps(screenDC, VERTRES);
      double x_bli = (double)(xRes/CLIENT_WIDTH);
	  double y_bli = (double)(yRes/CLIENT_HEIGHT);

	  /*m_BtRect[0].top = 0;m_BtRect[0].left = CLIENT_WIDTH*x_bli-163;m_BtRect[0].right = m_BtRect[0].left+ 52;m_BtRect[0].bottom = m_BtRect[0].top+28;
	  m_BtRect[2].top = 0;m_BtRect[2].left = CLIENT_WIDTH*x_bli-112;m_BtRect[2].right = m_BtRect[2].left+ 52;m_BtRect[2].bottom = m_BtRect[2].top+28;
	  m_BtRect[1].top = 0;m_BtRect[1].left = CLIENT_WIDTH*x_bli-60;m_BtRect[1].right = m_BtRect[1].left+ 52;m_BtRect[1].bottom = m_BtRect[1].top+28;
	  m_BtRect[3].top = 687*y_bli;m_BtRect[3].left = 575*x_bli;m_BtRect[3].right = m_BtRect[3].left+ 52;m_BtRect[3].bottom = m_BtRect[3].top+28;
	  m_BtRect[4].top = 687*y_bli;m_BtRect[4].left = 625*x_bli;m_BtRect[4].right = m_BtRect[4].left+ 52;m_BtRect[4].bottom = m_BtRect[4].top+28;
      m_BtRect[5].top = 687*y_bli;m_BtRect[5].left = 690*x_bli;m_BtRect[5].right = m_BtRect[5].left+ 90;m_BtRect[5].bottom = m_BtRect[5].top+30;*/
	  Definex_bli = x_bli;
	  Definey_bli = y_bli;
	  //玩家坐标
	  m_UserPT[0].x  =  80;
	  m_UserPT[0].y  =  574;
	  m_UserPT[1].x  =  455;
	  m_UserPT[1].y  =  574;
	  m_UserPT[2].x  =  830;
	  m_UserPT[2].y  =  574;
	  m_UserPT[5].x  =  80;
	  m_UserPT[5].y  =  -55;
	  m_UserPT[4].x  =  455;
	  m_UserPT[4].y  =  -55;
	  m_UserPT[3].x  =  830;
	  m_UserPT[3].y  =  -55;
	  CopyMemory(m_UserPaoPT,m_UserPT,sizeof(m_UserPT));
	   
	}
	else
	{
	   m_configDlg.m_isvisable = false;
	   m_IsIntro = false;
	   double x_bli = 1;
	   double y_bli = 1;
	   /*m_BtRect[0].top = 0;m_BtRect[0].left = CLIENT_WIDTH*x_bli-163;m_BtRect[0].right = m_BtRect[0].left+ 52;m_BtRect[0].bottom = m_BtRect[0].top+28;
	   m_BtRect[2].top = 0;m_BtRect[2].left = CLIENT_WIDTH*x_bli-112;m_BtRect[2].right = m_BtRect[2].left+ 52;m_BtRect[2].bottom = m_BtRect[2].top+28;
	   m_BtRect[1].top = 0;m_BtRect[1].left = CLIENT_WIDTH*x_bli-60;m_BtRect[1].right = m_BtRect[1].left+ 52;m_BtRect[1].bottom = m_BtRect[1].top+28;

	   m_BtRect[3].top = 687*y_bli;m_BtRect[3].left = 575*x_bli;m_BtRect[3].right = m_BtRect[3].left+ 52;m_BtRect[3].bottom = m_BtRect[3].top+28;
	   m_BtRect[4].top = 687*y_bli;m_BtRect[4].left = 625*x_bli;m_BtRect[4].right = m_BtRect[4].left+ 52;m_BtRect[4].bottom = m_BtRect[4].top+28;
       m_BtRect[5].top = 687*y_bli;m_BtRect[5].left = 690*x_bli;m_BtRect[5].right = m_BtRect[5].left+ 90;m_BtRect[5].bottom = m_BtRect[5].top+30;*/
	   Definex_bli = x_bli;
	   Definey_bli = y_bli;
	   //玩家坐标
	   m_UserPT[0].x  =  80;
	   m_UserPT[0].y  =  534;
	   m_UserPT[1].x  =  455;
	   m_UserPT[1].y  =  534;
	   m_UserPT[2].x  =  830;
	   m_UserPT[2].y  =  534;
	   m_UserPT[5].x  =  80;
	   m_UserPT[5].y  =  -31;
	   m_UserPT[4].x  =  455;
	   m_UserPT[4].y  =  -31;
	   m_UserPT[3].x  =  830;
	   m_UserPT[3].y  =  -31;
	   CopyMemory(m_UserPaoPT,m_UserPT,sizeof(m_UserPT));

	}
	
}

void	GameScreenWnd::SetGameTitle(TCHAR* szTitle)
{
	lstrcpyn(m_szTitle, szTitle, sizeof(m_szTitle)/sizeof(TCHAR));
}

void GameScreenWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->PostMessage(WM_KEYDOWN,83);
	CWnd::OnRButtonDown(nFlags, point);
}
