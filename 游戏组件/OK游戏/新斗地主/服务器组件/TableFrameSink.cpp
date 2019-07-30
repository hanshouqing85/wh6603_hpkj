#include "StdAfx.h"
#include "TableFrameSink.h"


/////////////////静态变量

SCORE				CTableFrameSink::m_lStockScore = 0.0;
LONGLONG			CTableFrameSink::m_lStorageOff = 0L;
BYTE				CTableFrameSink::StorageStyle=1;
//////////////////////////////////////////////////////////////////////////////////
#define IDI_CHECK_TABLE				1					//检查桌子
//构造函数
CTableFrameSink::CTableFrameSink()
{
	//组件变量
	m_pITableFrame=NULL;
	m_pGameCustomRule=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;

	//炸弹变量
	m_wFirstUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//游戏变量
	m_cbBombCount=0;
	ZeroMemory(m_cbEachBombCount,sizeof(m_cbEachBombCount));

	//叫分信息
	m_cbBankerScore=0;
	ZeroMemory(m_cbScoreInfo,sizeof(m_cbScoreInfo));

	//出牌信息
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//扑克信息
	ZeroMemory(m_cbBankerCard,sizeof(m_cbBankerCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));


	//服务控制
	m_hControlInst = NULL;
	m_pServerControl = NULL;
	m_hControlInst = LoadLibrary(TEXT("LandServerControl.dll"));
	if ( m_hControlInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hControlInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerControl = static_cast<IServerControl*>(ServerControl());
		}
	}

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink()
{
	if( m_pServerControl )
	{
		delete m_pServerControl;
		m_pServerControl = NULL;
	}

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}
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
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
	ZeroMemory(m_cbEachBombCount,sizeof(m_cbEachBombCount));

	//叫分信息
	m_cbBankerScore=0;
	ZeroMemory(m_cbScoreInfo,sizeof(m_cbScoreInfo));

	//出牌信息
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//扑克信息
	ZeroMemory(m_cbBankerCard,sizeof(m_cbBankerCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

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
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	//游戏配置
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//自定规则
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();


	
	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//读取配置
	_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\LandConfig.ini"),szPath);
	//开始存储数目
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

		//读取库存
		//m_lStockScore=GetPrivateProfileInt(szServerName,TEXT("StorageStart"),0,m_szFileName);
		///////////////////读取小数转换 梦成网络修正
		TCHAR OutBuf[255];
		memset(OutBuf,0,255);
		GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,m_szFileName);
			
		CString TempValue;
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
		m_lStockScore=Mywtof(TempValue);
		///////////////////////////////


	//读取衰减值
	m_lStorageDeduct=GetPrivateProfileInt(szServerName,TEXT("StorageDeduct"),10,m_szFileName);


	//设置最大衰减值
	if( m_lStorageDeduct < 0 || m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 0;
	if ( m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 1000;

	//梦成网络添加
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
		if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);
	//梦成网络添加

	ReadConfigInformation(true);

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
	//读取库存
	ReadConfigInformation(true);

	//梦成网络添加
	//清空出牌信息
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	/////

	//设置状态
	m_pITableFrame->SetGameStatus(GAME_SCENE_CALL);

	//混乱扑克
	BYTE cbRandCard[FULL_COUNT];
	m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

		//比赛房间随机第一个随机叫分
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0 )
	{
		//AfxMessageBox(TEXT("比赛房间首叫随机"));
		m_wFirstUser = INVALID_CHAIR;
	}

	//随机定义叫庄用户
	WORD wStartUser=rand()%GAME_PLAYER;
	WORD wCurrentUser=wStartUser;

	//下方的是控制相关的
	////////////////////////////////////////////////////////
	//判断是否控制指定玩家
	WORD wChairIDCtrl = INVALID_CHAIR;
	bool m_bWinnerCtrl = false;

	//管理员
	WORD IsAdmin = INVALID_CHAIR;
	//好牌用户ID 
	WORD WinCharID = INVALID_CHAIR;
	//好牌机器人
	WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
	//控制机率变量
	bool ReturnValue=false;

	///////////////////////////////
	///////////////////////////////////////////指定玩家控制开关
	if(m_dwCheatCount>0)  //控制次数
	{
		m_bWinnerCtrl = true;  //如果控制次数大于0，指定玩家输赢控制开启

		if(m_dwCheatGameID==0)  //如果玩家ID等于0 指定玩家输赢控制关闭
		{
			m_bWinnerCtrl=false;  
		}
		else   //玩家ID大于0
		{
			//返回ID
			wChairIDCtrl = GetChairIdByGameId(m_dwCheatGameID);  //获取玩家椅子号

			//指定玩家不在
			if( wChairIDCtrl==INVALID_CHAIR)
			{
				m_bWinnerCtrl=false;
			}
		}
	}else{
		//控制次数少于0
		m_bWinnerCtrl=false;
	}

	//管理员是否存在
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		//读取用户
		IServerUserItem * pServerUserItem = m_pITableFrame->GetTableUserItem( wChairID ) ;
		if (pServerUserItem==NULL) continue;
		//如果管理员,记录ID
		if(CUserRight::IsGameCheatUser(pServerUserItem->GetUserRight()))
		{
			IsAdmin=wChairID;
		}
	}

	//如果控制号不存在或者管理员不存在,库存控制
	//库存控制初始化数据
	if(m_bWinnerCtrl==false&&IsAdmin==INVALID_CHAIR){
	
	///梦成网络添加 库存加发送好牌
	///////////////////////////////////////////////////////////////////////
	//库存控制
	//随机种子
	srand( (unsigned)time(NULL));
	//获取随机数
	WORD Ratio = rand()%(100); //产生0~100的数

		//梦成网络添加
	WORD WinRatio = 100;         //定义当前输赢比例 //默认比例
	if(StorageStyle==1) WinRatio = StorageRatio;            //定义当前输赢比例 //默认比例
	if(StorageStyle==2) WinRatio = StorageMinRatio;         //定义当前输赢比例 //杀分
	if(StorageStyle==3) WinRatio = StorageMaxRatio;         //定义当前输赢比例 //送分

	//计算是杀分还是送分
	//吃掉库存
	if(m_lStockScore>=m_lStorageMax && StorageStyle!=3){
		
		SCORE tmp_lStockScore = m_lStockScore;							 //原来库存
		SCORE m_StorageEating=m_lStockScore*(StorageEating/100.00);		 //系统吃分
			
		//减去吃掉的分
		m_lStockScore-=m_StorageEating;
		WinRatio=StorageMaxRatio;  //送分模式
	
		//送分模式
		StorageStyle=3;
		//输出库存值
		CString strLog;
		strLog.Format(TEXT("桌号：%d,当前库存：%0.2f,系统吃分：%0.2f,剩余库存：%0.2f"),m_pITableFrame->GetTableID()+1,tmp_lStockScore,m_StorageEating,m_lStockScore);
		CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息
	
	}
	
	//库存少于最小值,杀分模式
	if(m_lStockScore<=m_lStorageMin && StorageStyle!=2)
	{
		
		WinRatio=StorageMinRatio;
		StorageStyle=2;//杀分模式
	}

	ReturnValue=(WinRatio>Ratio)?true:false; //计算机率

	 //输出当前模式
	CString strLog;
	strLog.Format(TEXT("桌号：%d,游戏模式：%d,当前输赢比率：%d (游戏模式1,正常,2,杀分,3,送分)"),m_pITableFrame->GetTableID()+1,StorageStyle,WinRatio);
	CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息

	//梦成网络
	//发送好牌开始
	//机器人总数
	BYTE cbAndroidUserCount = 0 ;
	//定义机器人
	WORD wAndroidUser[GAME_PLAYER] = {0};  
		
	//机器人个数  
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		//读取用户
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem( wChairID ) ;
		if (pServerUserItem==NULL) continue;
		//如果是机器人
		if ( pServerUserItem->IsAndroidUser())
		{
			//记录机器人ID
			wAndroidUser[ cbAndroidUserCount ] = ( wChairID ) ;
			//机器人总数加一
			cbAndroidUserCount++;
		}
	}
	
	//随机抽取一个机器人发送好牌
	wHaveGoodCardAndroidUser = INVALID_CHAIR ;
	//如果机器人总数大于1 随机抽出一个机器人接收好牌
	if (  cbAndroidUserCount >0 ) wHaveGoodCardAndroidUser = wAndroidUser[ rand() % cbAndroidUserCount ] ;
	
	}


	//管理员控制优化
	if(IsAdmin!=INVALID_CHAIR){
	
		
		//混乱扑克
		BYTE cbRandCard[FULL_COUNT];
		m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

		//获取好牌扑克数据
		BYTE cbGoodCard[ NORMAL_COUNT ] ;
		//获取好牌
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//cbGoodCard现在这个已经是好牌数据了,把他交给管理员即可
		//分发扑克
		m_cbHandCardCount[ IsAdmin ] = NORMAL_COUNT;
		CopyMemory(&m_cbHandCardData[ IsAdmin ], cbGoodCard, NORMAL_COUNT );
		//删除好牌数据
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, cbRandCard, FULL_COUNT ) ;
		
		//其他用户扑克数据
		for ( WORD i = 0, j = 0; i < GAME_PLAYER; i++ )
		{
			//如果当前用户不是管理员即把扑克数据给他
			if ( i != IsAdmin )
			{
				//定义用户扑克数据长度
				m_cbHandCardCount[ i ] = NORMAL_COUNT;
				//复制扑克数据给用户
				CopyMemory( &m_cbHandCardData[ i ],&cbRandCard[j*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
				++j ;
			}
		}

			//因为好牌用户扑克已经删除,扑克数据中只有37张牌2*NORMAL_COUNT=34 取当前长度,3个字符
			//设置底牌
			CopyMemory(m_cbBankerCard,&cbRandCard[2*NORMAL_COUNT],sizeof(m_cbBankerCard));
			//排列底牌
			m_GameLogic.SortCardList(m_cbBankerCard,sizeof(m_cbBankerCard),ST_ORDER);
	

	}
	else if(m_bWinnerCtrl){
		//指定玩家输赢控制
		
		//混乱扑克
		BYTE cbRandCard[FULL_COUNT];
		m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

		//获取好牌扑克数据
		BYTE cbGoodCard[ NORMAL_COUNT ] ;
		//获取好牌
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//cbGoodCard现在这个已经是好牌数据了,把他交给管理员即可
		//分发扑克
		m_cbHandCardCount[ wChairIDCtrl ] = NORMAL_COUNT;
		CopyMemory(&m_cbHandCardData[ wChairIDCtrl ], cbGoodCard, NORMAL_COUNT ) ;
		
		//删除好牌数据
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, cbRandCard, FULL_COUNT) ;
		
		//其他用户扑克数据
		for ( WORD i = 0, j = 0; i < GAME_PLAYER; i++ )
		{
			//如果当前用户不是管理员即把扑克数据给他
			if ( i != wChairIDCtrl )
			{
				//定义用户扑克数据长度
				m_cbHandCardCount[ i ] = NORMAL_COUNT;
				//复制扑克数据给用户
				CopyMemory( &m_cbHandCardData[ i ],&cbRandCard[j*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
				++j ;
			}
		}

			//因为好牌用户扑克已经删除,扑克数据中只有37张牌2*NORMAL_COUNT=34 取当前长度,3个字符
			//设置底牌
			CopyMemory(m_cbBankerCard,&cbRandCard[2*NORMAL_COUNT],sizeof(m_cbBankerCard));
			//排列底牌
			m_GameLogic.SortCardList(m_cbBankerCard,sizeof(m_cbBankerCard),ST_ORDER);

			////////////////////////////
			////控制信息初始化
			if(m_dwCheatCount>0) m_dwCheatCount--;
			if(m_dwCheatCount==0) {m_dwCheatGameID=0;}
			m_bWinnerCtrl = false;
			///////////////////////////

			//梦成网络添加
			//写入配置文件
			CString TempValue;
			
			//写入控制总数
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
		
			//梦成网络添加


	
	}
	//发送好牌 //库存控制
	else if ( ReturnValue && wHaveGoodCardAndroidUser != INVALID_CHAIR )
	{
	
		
		//混乱扑克
		BYTE cbRandCard[FULL_COUNT];
		m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

		//获取好牌扑克数据
		BYTE cbGoodCard[ NORMAL_COUNT ] ;
		//获取好牌
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//cbGoodCard现在这个已经是好牌数据了,把他交给机器人即可
		//分发扑克
		m_cbHandCardCount[ wHaveGoodCardAndroidUser ] = NORMAL_COUNT;
		CopyMemory(&m_cbHandCardData[ wHaveGoodCardAndroidUser ], cbGoodCard, NORMAL_COUNT ) ;
		

		//删除好牌数据
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, cbRandCard, FULL_COUNT ) ;
		
		//其他用户扑克数据
		for ( WORD i = 0, j = 0; i < GAME_PLAYER; i++ )
		{
			//如果当前用户不是好牌机器人即把扑克数据给他
			if ( i != wHaveGoodCardAndroidUser )
			{
				//定义用户扑克数据长度
				m_cbHandCardCount[ i ] = NORMAL_COUNT;
				//复制扑克数据给用户
				CopyMemory( &m_cbHandCardData[ i ],&cbRandCard[j*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
				++j ;
			}
		}

			//因为好牌用户扑克已经删除,扑克数据中只有37张牌2*NORMAL_COUNT=34 取当前长度,3个字符
			//设置底牌
			CopyMemory(m_cbBankerCard,&cbRandCard[2*NORMAL_COUNT],sizeof(m_cbBankerCard));
			//排列底牌
			m_GameLogic.SortCardList(m_cbBankerCard,sizeof(m_cbBankerCard),ST_ORDER);

			//设置变量
			WinCharID=wHaveGoodCardAndroidUser;

	}else{
		
			////////////////////////////////不控制!这里应该是送分给真人的才对的
			////梦成网络
			////发送用户扑克数据
			//用户扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHandCardCount[i]=NORMAL_COUNT;
				CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);

			}
			//设置底牌
			CopyMemory(m_cbBankerCard,&cbRandCard[DISPATCH_COUNT],sizeof(m_cbBankerCard));
			//排列底牌
			m_GameLogic.SortCardList(m_cbBankerCard,sizeof(m_cbBankerCard),ST_ORDER);
			/////结束发送扑克
	}

	
	///梦成网络添加 //发送好牌结束
	///////////////////////////////////////////////////////////////////////
	////////////结束控制

	//排列扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],ST_ORDER);
	}
	//机器人数据
	CMD_S_AndroidCard AndroidCard ;
	ZeroMemory(&AndroidCard, sizeof(AndroidCard)) ;
	//发送机器人底牌数据
	CopyMemory(AndroidCard.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));
	//发送机器人用户扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHandCardCount[i]=NORMAL_COUNT;
		//机器人数据
		CopyMemory(&AndroidCard.cbHandCard[i], &m_cbHandCardData[i], sizeof(BYTE)*m_cbHandCardCount[i]) ;
		//好牌用户ID
		AndroidCard.wAndroidUserID=WinCharID;
	}
	

	//设置用户
	m_wFirstUser=wCurrentUser;
	m_wCurrentUser=wCurrentUser;

	//构造变量
	CMD_S_GameStart GameStart;
	GameStart.wStartUser=wStartUser;
	GameStart.wCurrentUser=wCurrentUser;
	AndroidCard.wCurrentUser = m_wCurrentUser;

	//发送数据
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//构造扑克
		ASSERT(CountArray(GameStart.cbCardData)>=m_cbHandCardCount[i]);
		CopyMemory(GameStart.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//发送数据
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i) ;
		if(!pServerUserItem->IsAndroidUser())
		{
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		else 
		{
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&AndroidCard,sizeof(AndroidCard));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
	}

	for(WORD i = 0; i < GAME_PLAYER ;i++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;
		
		if(pIServerUserItem)
		{
			//作弊用户
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			//if(!pIServerUserItem->IsAndroidUser())
			{
				OnCheatCard(i);
			}
		}
	}

	if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
	{
		m_pITableFrame->SetGameTimer(IDI_CHECK_TABLE,30000,-1,NULL);
		m_cbWaitTime=0;
	}

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
		m_pITableFrame->KillGameTimer(IDI_CHECK_TABLE);
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//设置变量
			GameConclude.cbBankerScore=m_cbBankerScore;
			GameConclude.lCellScore=m_pITableFrame->GetCellScore();

			//炸弹信息
			GameConclude.cbBombCount=m_cbBombCount;
			CopyMemory(GameConclude.cbEachBombCount,m_cbEachBombCount,sizeof(GameConclude.cbEachBombCount));

			//用户扑克
			BYTE cbCardIndex=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//拷贝扑克
				GameConclude.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameConclude.cbHandCardData[cbCardIndex],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//设置索引
				cbCardIndex+=m_cbHandCardCount[i];
			}

			//炸弹统计
			LONG lScoreTimes=1L * m_cbBankerScore;
			for (BYTE i=0;i<m_cbBombCount;i++) lScoreTimes*=2L;

			//春天判断
			if (wChairID==m_wBankerUser)
			{
				//用户定义
				WORD wUser1=(m_wBankerUser+1)%GAME_PLAYER;
				WORD wUser2=(m_wBankerUser+2)%GAME_PLAYER;

				//用户判断
				if ((m_cbOutCardCount[wUser1]==0)&&(m_cbOutCardCount[wUser2]==0)) 
				{
					lScoreTimes*=2L;
					GameConclude.bChunTian=TRUE;
				}
			}

			//春天判断
			if (wChairID!=m_wBankerUser)
			{
				if (m_cbOutCardCount[m_wBankerUser]==1)
				{
					lScoreTimes*=2L;
					GameConclude.bFanChunTian=TRUE;
				}
			}

			//调整倍数
			lScoreTimes=__min(m_pGameCustomRule->wMaxScoreTimes,lScoreTimes);

			//积分变量
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				SCORE lUserScore=0L;
				SCORE lCellScore=m_pITableFrame->GetCellScore();

				//积分基数
				if (i==m_wBankerUser)
				{
					lUserScore=(m_cbHandCardCount[m_wBankerUser]==0)?2L:-2L;
				}
				else
				{
					lUserScore=(m_cbHandCardCount[m_wBankerUser]==0)?-1L:1L;
				}

				//计算积分
				ScoreInfoArray[i].lScore=lUserScore*lCellScore*lScoreTimes;
				ScoreInfoArray[i].cbType=(ScoreInfoArray[i].lScore>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				//计算税收
				if ( ScoreInfoArray[i].lScore > 0.0 )
				{
				//计算税收
				ScoreInfoArray[i].lRevenue=m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
				if (ScoreInfoArray[i].lRevenue>0L) ScoreInfoArray[i].lScore-=ScoreInfoArray[i].lRevenue;
				}
				//设置积分
				GameConclude.lGameScore[i]=ScoreInfoArray[i].lScore;

				//历史积分
				m_HistoryScore.OnEventUserScore(i,GameConclude.lGameScore[i]);
			}

			//发送数据
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//切换用户
			m_wFirstUser=wChairID;

			


						//统计结束写分操作结束
			/////////////////////////////////////////////////////////
			////统计库存信息 只有存在机器人并且存在真实用户的情况下才会计算
			bool  bUserAI = false;  //是否存在机器人
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (pIServerUserItemTemp->IsAndroidUser()){
					//存在机器人
					bUserAI = true;
					//跳出FOR
					break;

				}
			}
			//统计信息与计算库存
			SCORE lSystemScore=0.0;    //玩家得分
			SCORE lCurrentStorgeNum = m_lStockScore; //当前库存值
			SCORE RevenueScore = 0.0;	 //税收
			SCORE lStorageDeduct=0.0;	//库存衰减
			//库存统计 
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//如果用户不存在
				if (pIServerUserIte==NULL)continue;					
				//存在机器人才执行
				if(bUserAI){
					//如果用户不是机器人
					if(!pIServerUserIte->IsAndroidUser())
					{
						//计算税收
						if(ScoreInfoArray[i].lScore>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//计算库存衰减
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//系统库存值
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore,2,1)+lStorageDeduct); 
						//系统得分
						lSystemScore-=ScoreInfoArray[i].lScore;
					}
				}
			}

			////////////////////////////////////////////////////////////

			//判断是否存在真实玩家
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//存在真实用户
					bUser = true;
					//跳出FOR
					break;

				}
			}

			//判断是否存在真实玩家
			if (bUser)
			{
				
				CString strLog;
				strLog.Format(TEXT("桌号：%d,当前库存：%0.2f,系统得分：%0.2f,税收：%0.2f,剩余库存：%0.2f,库存衰减：%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息


				//梦成网络添加
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//记录总输赢记录
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//如果存在控制用户的话//发送游戏信息给控制号
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue;	//如果用户不存在
					if(pUserItem->IsAndroidUser()) continue;  //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //发送消息窗口
				}
			}

			//////////////////////////////////////////////////////
			//扣除库存
			m_lStorageOff=GetPrivateProfileInt(m_pGameServiceOption->szServerName,_T("StorageOff"),0,m_szFileName);
			//如果扣减数不等于0
			if (m_lStorageOff != 0)
			{
				m_lStockScore -= m_lStorageOff;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("人为操作库存减少%I64d，剩余库存：%0.2f"),m_lStorageOff, m_lStockScore);
				//输出信息
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				//设置完之后把该值变成0
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageOff"),TEXT("0"),m_szFileName);

				//发送控制号窗口提示
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue; //如果用户不存在
					if(pUserItem->IsAndroidUser()) continue; //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue; //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_EJECT);  //发送消息窗口

				}
			}

			//库存数据处理结束

			/////////////////////////////////////////////////

			
			//读配置
			ReadConfigInformation(true);

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);


			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//变量定义
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//用户扑克
			BYTE cbCardIndex=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//拷贝扑克
				GameConclude.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameConclude.cbHandCardData[cbCardIndex],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//设置索引
				cbCardIndex+=m_cbHandCardCount[i];
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);

			return true;
		}
	case GER_USER_LEAVE:	//用户强退
	case GER_NETWORK_ERROR:	//网络中断
		{
			//变量定义
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//设置变量
			GameConclude.cbBankerScore=m_cbBankerScore;
			GameConclude.lCellScore=m_pITableFrame->GetCellScore();

			//炸弹信息
			GameConclude.cbBombCount=m_cbBombCount;
			CopyMemory(GameConclude.cbEachBombCount,m_cbEachBombCount,sizeof(GameConclude.cbEachBombCount));

			//用户扑克
			BYTE cbCardIndex=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//拷贝扑克
				GameConclude.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameConclude.cbHandCardData[cbCardIndex],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//设置索引
				cbCardIndex+=m_cbHandCardCount[i];
			}

			//炸弹统计
			WORD lScoreTimes=1;
			for (WORD i=0;i<m_cbBombCount;i++) lScoreTimes*=2L;

			//调整倍数
			lScoreTimes=__min(m_pGameCustomRule->wMaxScoreTimes,lScoreTimes);

			//积分变量
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			//变量定义
			SCORE lCellScore=m_pITableFrame->GetCellScore();
			SCORE lUserScore=lCellScore*__max(lScoreTimes,m_pGameCustomRule->wFleeScoreTimes);

			//金币平衡
			if ((m_pGameServiceOption->wServerType&SCORE_GENRE_POSITIVE)!=0 || (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0)
			{
				lUserScore=__min(pIServerUserItem->GetUserScore(),lUserScore);
			}

			//逃跑罚分
			ScoreInfoArray[wChairID].lScore=-lUserScore;
			ScoreInfoArray[wChairID].cbType=SCORE_TYPE_FLEE;

			//分享罚分
			if (m_pGameCustomRule->cbFleeScorePatch==TRUE)
			{
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//过滤
					if ( i == wChairID )
						continue;

					//设置积分
					ScoreInfoArray[i].lScore=lUserScore/2L;
					ScoreInfoArray[i].cbType=SCORE_TYPE_WIN;

					//计算税收
					ScoreInfoArray[i].lRevenue=m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
					if (ScoreInfoArray[i].lRevenue>0L) ScoreInfoArray[i].lScore-=ScoreInfoArray[i].lRevenue;
				}
			}

			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//设置成绩
				GameConclude.lGameScore[i]=ScoreInfoArray[i].lScore;

				//历史成绩
				m_HistoryScore.OnEventUserScore(i,GameConclude.lGameScore[i]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));



			


						//统计结束写分操作结束
			/////////////////////////////////////////////////////////
			////统计库存信息 只有存在机器人并且存在真实用户的情况下才会计算
			bool  bUserAI = false;  //是否存在机器人
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (pIServerUserItemTemp->IsAndroidUser()){
					//存在机器人
					bUserAI = true;
					//跳出FOR
					break;

				}
			}
			//统计信息与计算库存
			SCORE lSystemScore=0.0;    //玩家得分
			SCORE lCurrentStorgeNum = m_lStockScore; //当前库存值
			SCORE RevenueScore = 0.0;	 //税收
			SCORE lStorageDeduct=0.0;	//库存衰减
			//库存统计 
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//如果用户不存在
				if (pIServerUserIte==NULL)continue;					
				//存在机器人才执行
				if(bUserAI){
					//如果用户不是机器人
					if(!pIServerUserIte->IsAndroidUser())
					{
						//计算税收
						if(ScoreInfoArray[i].lScore>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//计算库存衰减
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//系统库存值
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore,2,1)+lStorageDeduct); 
						//系统得分
						lSystemScore-=ScoreInfoArray[i].lScore;
					}
				}
			}

			////////////////////////////////////////////////////////////

			//判断是否存在真实玩家
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//存在真实用户
					bUser = true;
					//跳出FOR
					break;

				}
			}

			//判断是否存在真实玩家
			if (bUser)
			{
				
				CString strLog;
				strLog.Format(TEXT("桌号：%d,当前库存：%0.2f,系统得分：%0.2f,税收：%0.2f,剩余库存：%0.2f,库存衰减：%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息


				//梦成网络添加
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//记录总输赢记录
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//如果存在控制用户的话//发送游戏信息给控制号
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue;	//如果用户不存在
					if(pUserItem->IsAndroidUser()) continue;  //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //发送消息窗口
				}
			}

			//////////////////////////////////////////////////////
			//扣除库存
			m_lStorageOff=GetPrivateProfileInt(m_pGameServiceOption->szServerName,_T("StorageOff"),0,m_szFileName);
			//如果扣减数不等于0
			if (m_lStorageOff != 0)
			{
				m_lStockScore -= m_lStorageOff;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("人为操作库存减少%I64d，剩余库存：%0.2f"),m_lStorageOff, m_lStockScore);
				//输出信息
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				//设置完之后把该值变成0
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageOff"),TEXT("0"),m_szFileName);

				//发送控制号窗口提示
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue; //如果用户不存在
					if(pUserItem->IsAndroidUser()) continue; //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue; //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_EJECT);  //发送消息窗口

				}
			}

			//库存数据处理结束

			/////////////////////////////////////////////////
									//读配置
			ReadConfigInformation(true);

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);




			return true;
		}
	}
	//错误断言
	ASSERT(FALSE);

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//设置变量
			StatusFree.lCellScore=m_pITableFrame->GetCellScore();

			//自定规则
			StatusFree.cbTimeOutCard=m_pGameCustomRule->cbTimeOutCard;		
			StatusFree.cbTimeCallScore=m_pGameCustomRule->cbTimeCallScore;
			StatusFree.cbTimeStartGame=m_pGameCustomRule->cbTimeStartGame;
			StatusFree.cbTimeHeadOutCard=m_pGameCustomRule->cbTimeHeadOutCard;

						//房间名称
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_CALL:	//叫分状态
		{
			//构造数据
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//单元积分
			StatusCall.lCellScore=m_pITableFrame->GetCellScore();

			//自定规则
			StatusCall.cbTimeOutCard=m_pGameCustomRule->cbTimeOutCard;		
			StatusCall.cbTimeCallScore=m_pGameCustomRule->cbTimeCallScore;
			StatusCall.cbTimeStartGame=m_pGameCustomRule->cbTimeStartGame;
			StatusCall.cbTimeHeadOutCard=m_pGameCustomRule->cbTimeHeadOutCard;

			//游戏信息
			StatusCall.wCurrentUser=m_wCurrentUser;
			StatusCall.cbBankerScore=m_cbBankerScore;
			CopyMemory(StatusCall.cbScoreInfo,m_cbScoreInfo,sizeof(m_cbScoreInfo));
			CopyMemory(StatusCall.cbHandCardData,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID]*sizeof(BYTE));

			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusCall.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusCall.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//发送场景
			bool bSendResult = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));

			if(pIServerUserItem)
			{
				//作弊用户
				if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
				{
					OnCheatCard(wChairID);
				}
			}
			return bSendResult;
		}
	case GAME_SCENE_PLAY:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//单元积分
			StatusPlay.lCellScore=m_pITableFrame->GetCellScore();

			//游戏变量
			StatusPlay.cbBombCount=m_cbBombCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.cbBankerScore=m_cbBankerScore;

			//自定规则
			StatusPlay.cbTimeOutCard=m_pGameCustomRule->cbTimeOutCard;		
			StatusPlay.cbTimeCallScore=m_pGameCustomRule->cbTimeCallScore;
			StatusPlay.cbTimeStartGame=m_pGameCustomRule->cbTimeStartGame;
			StatusPlay.cbTimeHeadOutCard=m_pGameCustomRule->cbTimeHeadOutCard;

			//出牌信息
			StatusPlay.wTurnWiner=m_wTurnWiner;
			StatusPlay.cbTurnCardCount=m_cbTurnCardCount;
			CopyMemory(StatusPlay.cbTurnCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//扑克信息
			CopyMemory(StatusPlay.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));
			CopyMemory(StatusPlay.cbHandCardCount,m_cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(StatusPlay.cbHandCardData,m_cbHandCardData[wChairID],sizeof(BYTE)*m_cbHandCardCount[wChairID]);

									//房间名称
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//发送场景
			bool bSendResult = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			if(pIServerUserItem)
			{
				//作弊用户
				if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
				{
					OnCheatCard(wChairID);
				}
			}
			return bSendResult;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//时间事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
	{
		if(wTimerID==IDI_CHECK_TABLE)
		{
			m_cbWaitTime++;
			if(m_cbWaitTime>=3)
			{
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS);
			}
			else if(m_cbWaitTime>=2)
			{
				for(int i=0;i<GAME_PLAYER;i++)
				{
					IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(i);
					if(pServerUserItem!=NULL && pServerUserItem->GetUserStatus()==US_OFFLINE)return true;				
				}
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS);
			}
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
	m_cbWaitTime=0;
	switch (wSubCmdID)
	{
	case SUB_C_CALL_SCORE:	//用户叫分
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_CallScore));
			if (wDataSize!=sizeof(CMD_C_CallScore)) return false;

			//状态效验
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_CALL);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_CALL) return true;

			//用户效验
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//变量定义
			CMD_C_CallScore * pCallScore=(CMD_C_CallScore *)pData;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserCallScore(wChairID,pCallScore->cbCallScore);
		}
	case SUB_C_OUT_CARD:	//用户出牌
		{
			//变量定义
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//效验数据
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE)))) return false;

			//状态效验
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//用户效验
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserOutCard(wChairID,pOutCard->cbCardData,pOutCard->cbCardCount);
		}
	case SUB_C_PASS_CARD:	//用户放弃
		{
			//状态效验
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//用户效验
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			return OnUserPassCard(pIServerUserItem->GetChairID());
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
	//历史积分
	if (bLookonUser==false)
	{
		ASSERT(wChairID!=INVALID_CHAIR);
		m_HistoryScore.OnEventUserEnter(wChairID);
	}

	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//历史积分
	if (bLookonUser==false)
	{
		ASSERT(wChairID!=INVALID_CHAIR);
		m_HistoryScore.OnEventUserLeave(wChairID);
	}

	return true;
}

//用户放弃
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//效验状态
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbTurnCardCount!=0));
	if ((wChairID!=m_wCurrentUser)||(m_cbTurnCardCount==0)) return false;

	//设置变量
	m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
	if (m_wCurrentUser==m_wTurnWiner) m_cbTurnCardCount=0;

	//构造消息
	CMD_S_PassCard PassCard;
	PassCard.wPassCardUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.cbTurnOver=(m_cbTurnCardCount==0)?TRUE:FALSE;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//用户叫分
bool CTableFrameSink::OnUserCallScore(WORD wChairID, BYTE cbCallScore)
{
	//效验状态
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return false;

	//效验参数
	//ASSERT(((cbCallScore>=1)&&(cbCallScore<=3)&&(cbCallScore>m_cbBankerScore))||(cbCallScore==255));
	if (((cbCallScore<1)||(cbCallScore>3)||(cbCallScore<=m_cbBankerScore))&&(cbCallScore!=255))
		cbCallScore = 255;

	//设置状态
	if (cbCallScore!=0xFF)
	{
		m_cbBankerScore=cbCallScore;
		m_wBankerUser=m_wCurrentUser;
	}

	//设置叫分
	m_cbScoreInfo[wChairID]=cbCallScore;

	//设置用户
	if ((m_cbBankerScore==3)||(m_wFirstUser==(wChairID+1)%GAME_PLAYER))
	{
		m_wCurrentUser=INVALID_CHAIR;
	}
	else
	{
		m_wCurrentUser=(wChairID+1)%GAME_PLAYER;
	}

	//构造变量
	CMD_S_CallScore CallScore;
	CallScore.wCallScoreUser=wChairID;
	CallScore.wCurrentUser=m_wCurrentUser;
	CallScore.cbUserCallScore=cbCallScore;
	CallScore.cbCurrentScore=m_cbBankerScore;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_SCORE,&CallScore,sizeof(CallScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_SCORE,&CallScore,sizeof(CallScore));

	//开始判断
	if ((m_cbBankerScore==3)||(m_wFirstUser==(wChairID+1)%GAME_PLAYER))
	{
		//无人叫分
		if ( m_cbBankerScore == 0 )
		{
			return OnEventGameStart();
			//m_wBankerUser=m_wFirstUser;
			//m_cbBankerScore=1;
		}

		//设置状态
		m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

		//设置变量
		if (m_cbBankerScore==0) m_cbBankerScore=1;
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

		//发送底牌
		m_cbHandCardCount[m_wBankerUser]+=CountArray(m_cbBankerCard);
		CopyMemory(&m_cbHandCardData[m_wBankerUser][NORMAL_COUNT],m_cbBankerCard,sizeof(m_cbBankerCard));

		//排列扑克
		m_GameLogic.SortCardList(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser],ST_ORDER);


		//设置用户
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=m_wBankerUser;

		//发送消息
		CMD_S_BankerInfo BankerInfo;
		BankerInfo.wBankerUser=m_wBankerUser;
		BankerInfo.wCurrentUser=m_wCurrentUser;
		BankerInfo.cbBankerScore=m_cbBankerScore;
		CopyMemory(BankerInfo.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BANKER_INFO,&BankerInfo,sizeof(BankerInfo));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BANKER_INFO,&BankerInfo,sizeof(BankerInfo));

		return true;

	}

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//效验状态
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return false;

	//获取类型
	BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);

	//类型判断
	if (cbCardType==CT_ERROR) 
	{
		ASSERT(FALSE);
		return false;
	}

	//出牌判断
	if (m_cbTurnCardCount!=0)
	{
		//对比扑克
		if (m_GameLogic.CompareCard(m_cbTurnCardData,cbCardData,m_cbTurnCardCount,cbCardCount)==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//删除扑克
	if (m_GameLogic.RemoveCardList(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//出牌变量
	m_cbOutCardCount[wChairID]++;

	//设置变量
	m_cbTurnCardCount=cbCardCount;
	m_cbHandCardCount[wChairID]-=cbCardCount;
	CopyMemory(m_cbTurnCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//炸弹判断
	if ((cbCardType==CT_BOMB_CARD)||(cbCardType==CT_MISSILE_CARD)) 
	{
		m_cbBombCount++;
		m_cbEachBombCount[wChairID]++;
	}
	
	//切换用户
	m_wTurnWiner=wChairID;
	if (m_cbHandCardCount[wChairID]!=0)
	{
		if (cbCardType!=CT_MISSILE_CARD)
		{
			m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
		}
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

	//发送数据
	WORD wHeadSize=sizeof(OutCard)-sizeof(OutCard.cbCardData);
	WORD wSendSize=wHeadSize+OutCard.cbCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//出牌最大
	if (cbCardType==CT_MISSILE_CARD) m_cbTurnCardCount=0;

	//结束判断
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(wChairID,NULL,GER_NORMAL);

	return true;
}

//作弊用户
bool CTableFrameSink::OnCheatCard(WORD wChairID)
{
	//if(m_pServerControl)
	{
		CMD_S_CheatCard CheatCard ;
		ZeroMemory(&CheatCard, sizeof(CheatCard));

		for(WORD i = 0; i < GAME_PLAYER; i++)
		{
			CheatCard.wCardUser[CheatCard.cbUserCount] = i;
			CheatCard.cbCardCount[CheatCard.cbUserCount] = m_cbHandCardCount[i];
			CopyMemory(CheatCard.cbCardData[CheatCard.cbUserCount++],m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);
		}
		//发送作弊
		//WB改 这里这样发包存在BUG
		//m_pITableFrame->SendTableData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
		//m_pITableFrame->SendLookonData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
		//m_pServerControl->ServerControl(&CheatCard, m_pITableFrame, pIServerUserItem);

			//发送作弊
		m_pITableFrame->SendTableData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
		m_pITableFrame->SendLookonData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
	}
	
	return true;
}


//发送作弊信息
//void CTableFrameSink::SendCheatCard(WORD wChairID)
//{
	//if(m_pServerControl)
	//{
//		CMD_S_CheatCard CheatCard ;
//		ZeroMemory(&CheatCard, sizeof(CheatCard));
//
//		for(WORD i = 0; i < GAME_PLAYER; i++)
//		{
//			CheatCard.wCardUser[CheatCard.cbUserCount] = i;
//			CheatCard.cbCardCount[CheatCard.cbUserCount] = m_cbHandCardCount[i];
//			CopyMemory(CheatCard.cbCardData[CheatCard.cbUserCount++],m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);
//		}
		//m_pServerControl->ServerControl(&CheatCard, m_pITableFrame);
			//发送作弊
//		m_pITableFrame->SendTableData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
//		m_pITableFrame->SendLookonData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
	//}
//}


//设置基数
void CTableFrameSink::SetGameBaseScore(SCORE lBaseScore)
{
	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SET_BASESCORE,&lBaseScore,sizeof(lBaseScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SET_BASESCORE,&lBaseScore,sizeof(lBaseScore));
}
//////////////////////////////////////////////////////////////////////////////////


//根据用户Id获取其所坐的椅子Id
WORD CTableFrameSink::GetChairIdByGameId(DWORD dwGameId)
{
	WORD wChairID;
	for (wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue; //用户不存在

		if( pIServerUserItem->GetUserID() == dwGameId)
		{
			return wChairID;
		}
	}

	return INVALID_CHAIR;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//读取配置
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR OutBuf[255];
	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);
	//读取配置
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\LandConfig.ini"),szPath);
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

	//是否读取每盘刷新配置变量
	if (bReadFresh)
	{
		//每盘刷新
		BYTE cbRefreshCfg = GetPrivateProfileInt(szServerName, TEXT("Refresh"), 0, szFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;

		//是否刷新库存
		BYTE cbRefreshStorageCfg = GetPrivateProfileInt(szServerName, TEXT("RefreshStorage"), 0, szFileName);
		m_bRefreshStorageCfg = cbRefreshStorageCfg?true:false;

	}

	//库存设置
	if(m_bRefreshStorageCfg)
	{
		//重置库存
		memset(OutBuf,0,255);
		GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,szFileName);
		CString TempValue;
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
		m_lStockScore=Mywtof(TempValue);
	}


	//重置衰减值
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageDeduct);


		//梦成网络添加
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
		if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);


	//系统总输赢分
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageLimit"),TEXT("0.0"),OutBuf,255,szFileName);
	
	//////////////////////////////////
	//梦成网络读取小数点数值
	CString TempValue;
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
	m_lStockLimit=Mywtof(TempValue);
	/////////////////////////////////

	//梦成网络添加 //最新前台控制
	//需要胜出的玩家UserID
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("WinnerUserID"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%lu"),&m_dwCheatGameID);
	//读取控制次数
	m_dwCheatCount = GetPrivateProfileInt(szServerName, TEXT("CheatCount"), 0, szFileName);
	/////////////////


}