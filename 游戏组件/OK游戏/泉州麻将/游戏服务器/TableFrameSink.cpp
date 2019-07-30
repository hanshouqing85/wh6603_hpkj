#include "StdAfx.h"
#include "TableFrameSink.h"
#include ".\tableframesink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
//const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////
wchar_t g_wcTmp[256]={0};

//�õ���ǰ�������ڵ�·�� ���һ���ַ��� '\'
string GetAppPath()
{
	HINSTANCE hInst=NULL;
	hInst=(HINSTANCE)GetModuleHandleA(NULL);

	CHAR path_buffer[_MAX_PATH];
	GetModuleFileNameA(hInst,path_buffer,sizeof(path_buffer));//�õ�exe�ļ���ȫ·�� 
	string strPath;

	strPath=path_buffer;

	//ֻ����ļ���·������Ҫ�ļ���
	int pos=strPath.find_last_of("\\");

	strPath=strPath.substr(0,pos);


	return strPath;
}

// ����Ŀ¼
int create_dir(wchar_t* pszDirName, int iDirNameLen)
{
	if(!pszDirName || 0==iDirNameLen)
		return 10;	// Ŀ¼���ǿ�ֵ���߳��Ȳ���ȷ

	int iResult = 0;
	int iFlag = 0;
	DWORD dwError = 0;

	// iMode ֵ�ĺ��壺
	//	00 Existence only
	//	02  Write-only
	//	04 Read-only
	//	06 Read and write
	int iMode = 4;

	// �ж�Ŀ¼���ڷ�
	iFlag = _waccess(pszDirName, iMode);

	if(0==iFlag)	// ���ڸ�Ŀ¼
	{
		return 0;
	}	
	else	// û�и�Ŀ¼
	{
		iFlag = _wmkdir(pszDirName);
		if(0==iFlag)	// ������
		{
		}
		else	// ����ʧ��
		{
			dwError = GetLastError();
			iResult = 30;	// ����ʧ��
		}
	}

	return 0;
}

string ws2s(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

	setlocale(LC_ALL, "chs"); 

	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	string result = _Dest;
	delete []_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

wstring s2ws(const string& s)
{
	setlocale(LC_ALL, "chs"); 

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	wstring result = _Dest;
	delete []_Dest;

	setlocale(LC_ALL, "C");

	return result;
}

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
    m_GameLogic.SetMagicIndex(MAX_INDEX);
	m_wDoorDiceCount = 0;
	m_wGoldDiceCount = 0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	//ZeroMemory(m_cbDisPengIndex,sizeof(m_cbDisPengIndex));
	//ZeroMemory(m_cbDisHuIndex,sizeof(m_cbDisHuIndex));
	msg=NULL;
	m_GoldSwim = GoldSwim_None;
	m_bGameStart = false;
	memset(m_UserGoldSwim,GoldSwim_None,sizeof(m_UserGoldSwim));
	ZeroMemory(m_cbCurCardIndex,sizeof(m_cbCurCardIndex));
	 
	
	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	m_cbRepairFlowerPos = 0;
	m_cbMaxLeftCount = MAX_LEFT_COUNT;

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	ZeroMemory(m_cbFlowerCount,sizeof(m_cbFlowerCount));
	ZeroMemory(m_cbGoldCount,sizeof(m_cbGoldCount));
	ZeroMemory(m_cbGangCount,sizeof(m_cbGangCount));
	m_cbDoorCard=0;


	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;


	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));	
	ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));
	m_bGoOnBankerTime = 1;
	
	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
	ZeroMemory(m_lUserCapital,sizeof(m_lUserCapital));
	for(BYTE i=0;i<4;i++)m_bHuUser[i]=false;

	//����齫
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));


	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	if(msg)
	{
	   delete[]msg;
	}	
}

//�ӿڲ�ѯ
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
		return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ʼģʽ//////////////////
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	return true;
}

//��λ����
void  CTableFrameSink::RepositionSink()
{
	//��Ϸ����
    m_GameLogic.SetMagicIndex(MAX_INDEX);
	m_wDoorDiceCount = 0;
	m_wGoldDiceCount = 0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	//ZeroMemory(m_cbDisPengIndex,sizeof(m_cbDisPengIndex));
	//ZeroMemory(m_cbDisHuIndex,sizeof(m_cbDisHuIndex));
	m_GoldSwim = GoldSwim_None;
	m_bGameStart = false;
	memset(m_UserGoldSwim,GoldSwim_None,sizeof(m_UserGoldSwim));
	ZeroMemory(m_cbCurCardIndex,sizeof(m_cbCurCardIndex));


	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	msg=NULL;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	m_cbRepairFlowerPos = 0;
	m_cbMaxLeftCount = MAX_LEFT_COUNT;

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	ZeroMemory(m_cbFlowerCount,sizeof(m_cbFlowerCount));
	ZeroMemory(m_cbGoldCount,sizeof(m_cbGoldCount));
	ZeroMemory(m_cbGangCount,sizeof(m_cbGangCount));
	m_cbDoorCard=0;


	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;	
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));	
	ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));
	m_bGoOnBankerTime = 1;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
	ZeroMemory(m_lUserCapital,sizeof(m_lUserCapital));

	//����齫
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	return;
}

////��ʼģʽ
//enStartMode  CTableFrameSink::GetGameStartMode()
//{
//	return m_GameStartMode;
//}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_MJ_BEFOREPLAY);

	//���ñ���
	m_cbRepairFlowerPos = 0;

	//�м����
	IServerUserItem *pUserItem;

	//��ȡ����ʱ�
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem!=NULL)
		{
			m_lUserCapital[i]=pUserItem->GetUserScore();
		}

	}

	//������ȷ��ׯ��
	m_wBankerUser = (rand()%6+1)%GAME_PLAYER;

		for(BYTE i=0;i<4;i++)m_bHuUser[i]=false;		
		ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));
		ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	
		//�����齫		
		m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
		m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

#ifdef USE_FIXCARD
	//test
	BYTE byTest[] = {
		0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,//ͬ��						
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,//����						
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
		//���Ӻͷ���
	    0x29,0x31,0x25,0x36,0x21,0x32,0x33,0x26,0x37,
		0x28,
		0x27,//����
		0x27,//����
		0x21,0x21,0x22,0x22,0x24,0x26,0x26,0x28,0x29,0x33,0x35,0x36,0x37,//3
        0x21,0x22,0x23,0x23,0x24,0x31,0x32,0x33,0x34,0x36,0x36,0x37,0x37,//2
        0x22,0x23,0x23,0x24,0x24,0x25,0x25,0x27,0x34,0x34,0x34,0x33,0x35,//1
        0x25,0x26,0x27,0x28,0x28,0x29,0x29,0x31,0x31,0x32,0x32,0x35,0x35,//0
	};
	CopyMemory( m_cbRepertoryCard,byTest,sizeof(byTest) );
	m_wBankerUser = 0;
	//end test
#endif

		//�ַ��齫
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_cbLeftCardCount-=(MAX_COUNT-1);
		
			m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);

		} 


		//���ñ���
		m_cbProvideCard=0;
		m_wProvideUser=INVALID_CHAIR;
		m_wCurrentUser=m_wBankerUser;
		
		
		
		//������Ϣ
		CMD_S_GameStart GameStart;
		
		GameStart.wBankerUser=m_wBankerUser;
		GameStart.wCurrentUser=m_wCurrentUser;	

#ifdef USE_RS_PRINT
	SYSTEMTIME timeCreateFile={0};	// ������ʱ��
	GetSystemTime(&timeCreateFile);	// ��õ�ǰʱ��

	wstring  strLogFile=L"";
	strLogFile=s2ws(GetAppPath())+L"\\QZMJServer\\NetMsg.txt";
	wstring wstrDirName=s2ws(GetAppPath())+L"\\QZMJServer";
	int ret=create_dir((wchar_t *)wstrDirName.c_str(),wstrDirName.length());

	wsprintfW(g_wcTmp,L"%s%04d_%02d_%02d��%02d��%02d��%02d��.txt", 
		strLogFile.c_str(),
		timeCreateFile.wYear,
		timeCreateFile.wMonth,
		timeCreateFile.wDay,
		timeCreateFile.wHour+8,	// ��8�Ƕ��뱱��ʱ��
		timeCreateFile.wMinute,
		timeCreateFile.wSecond
		);
	ofstream outf(g_wcTmp,ios::app);
#ifdef USE_FIXCARD
	int iNum=sizeof(byTest)/sizeof(BYTE);
	CString strLog0;
	strLog0.Format(_T("sizeof(byTest)/sizeof(BYTE)=[%d]"),iNum);
	for(int i=0;i<iNum;i++)
	{
		CString strLog;
		strLog.Format(_T("[%d,%x]"),i,byTest[i]);
		strLog0+=strLog;
	}
	CTraceService::TraceString(strLog0,TraceLevel_Normal);
    outf<<CT2A(strLog0)<<endl;
#endif
	CString strLog;
	strLog.Format(_T("CMD_S_GameStart ׯ���û�wBankerUser=[%d],��ǰ�û�wCurrentUser=[%d]"),GameStart.wBankerUser,GameStart.wCurrentUser);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
	outf<<CT2A(strLog)<<endl;
#endif

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//���ñ���			
			m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("i=[%d]��,[%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x]"),i,GameStart.cbCardData[0],GameStart.cbCardData[1],GameStart.cbCardData[2],GameStart.cbCardData[3],GameStart.cbCardData[4],GameStart.cbCardData[5],GameStart.cbCardData[6],GameStart.cbCardData[7],GameStart.cbCardData[1+7],GameStart.cbCardData[2+7],GameStart.cbCardData[3+7],GameStart.cbCardData[4+7],GameStart.cbCardData[5+7],GameStart.cbCardData[6+7]);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		//ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif	
			//��������
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			

		}		
		
		return true;


	

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{	
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			msg=new char[100];			
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			//LONG lCellScore=m_pGameServiceOption->lCellScore;	
			GameEnd.lCellScore = m_pGameServiceOption->lCellScore;
			GameEnd.bGoOnBankerTime = m_bGoOnBankerTime;
			strcpy(msg,"");

			
			//�����ϱ�
			bool bAllSidesCard[4];
			ZeroMemory(bAllSidesCard,sizeof(bAllSidesCard));
			//�����ﶬ
			bool bAllSeasonCard[4];
			ZeroMemory(bAllSeasonCard,sizeof(bAllSeasonCard));
			//÷�����
			bool bAllPlantCard[4];
			ZeroMemory(bAllPlantCard,sizeof(bAllPlantCard));


			//������Ϣ
			GameEnd.wProvideUser=wChairID;
			GameEnd.cbChiHuCard=m_cbChiHuCard;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.wChiHuKind[i]=m_ChiHuResult[i].wChiHuKind;					
				GameEnd.bFlowerCount[i]=m_cbFlowerCount[i];
				GameEnd.bGangCount[i]=m_cbGangCount[i];

				//ͳ�ƽ�����Ŀ
				m_cbGoldCount[i]+=m_GameLogic.GetGoldCardCount(m_cbCardIndex[i],m_WeaveItemArray[i],m_cbWeaveItemCount[i],m_cbGoldCard,4);
				GameEnd.bGoldCount[i]=m_cbGoldCount[i];

				//÷�����
				if(m_GameLogic.IsExistAllPlantCard(m_cbCardIndex[i]))
				{
					bAllPlantCard[i]=true;
					GameEnd.bAllPlantCard[i]=true;
				}

				//�����ﶬ
				if(m_GameLogic.IsExistAllSeasonCard(m_cbCardIndex[i]))
				{
					bAllSeasonCard[i]=true;
					GameEnd.bAllSeasonCard[i]=true;
				}

				//�����ϱ�
				if(m_GameLogic.IsExistAllSidesCard(m_cbCardIndex[i]))
				{
					bAllSidesCard[i]=true;
					GameEnd.bAllSidesCard[i]=true;
				}
			}			
			
			//ͳ����ҷǺ��Ʒ���
			BYTE bUserTime[GAME_PLAYER];
			ZeroMemory(bUserTime,sizeof(bUserTime));

			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				bUserTime[i]=m_cbGoldCount[i]+m_cbFlowerCount[i]+m_cbGangCount[i]+
					(bAllPlantCard[i]?1:0)+(bAllSidesCard[i]?1:0)+(bAllSeasonCard[i]?1:0);
			}

			LONG lTmpScore=0L;
			LONG lBeiShu=1L;

			//ͳ�ƻ���
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				
				//��������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL)
				{

					//��������
					LONG lCellScore=m_pGameServiceOption->lCellScore*2;
					
					//��������					
					GameEnd.wWinner[m_wProvideUser]=true;

					//ͳ�ƷǺ��������Ӯ����
					BYTE bTmpUserTime=0;		

					for(WORD i=0;i<GAME_PLAYER;i++)
					{
						//�������
						if(i==m_wProvideUser)continue;

						bTmpUserTime=0;

						//ͳ���������ҵķ���
						for(WORD j=0;j<GAME_PLAYER;j++)
						{
							//�������
							if((i==m_wProvideUser)||(i==j))continue;

							bTmpUserTime+=bUserTime[j];

						}

						//ͳ�Ʒ���
						GameEnd.lGameScore[i]+=(2*bUserTime[i]-bTmpUserTime)*lCellScore*m_bGoOnBankerTime;	
				

					}

					

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//Ӯ�ҹ���
						if (i==m_wProvideUser) continue;						

						lBeiShu=1L;	
						lBeiShu*=m_bGoOnBankerTime;				

						//��ʼ����						
						LONG lInitScore=1;

						//���Ʒ���
						LONG lTimeScore = bUserTime[m_wProvideUser]*lCellScore*m_bGoOnBankerTime;
						

						//����ͳ��
						switch(m_ChiHuResult[m_wProvideUser].wChiHuKind|m_ChiHuResult[m_wProvideUser].wChiHuRight)
						{

							//��ͨ����
						case CHK_SKY_SWIM:
							{
								lBeiShu*=8;

								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;	

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];

								GameEnd.lGameScore[i]-=lTmpScore;								
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"����");

								break;
							}

							//���ϻ�����
						case CHR_GANG_HU|CHK_SKY_SWIM:
							{
								lBeiShu*=16;

								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;	

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];

								GameEnd.lGameScore[i]-=lTmpScore;								
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"����,���ϻ�");

								break;
							}

							//��ͨ����					
						case CHK_PING_HU:
							{								
								lInitScore=2;
								
								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;	

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];


								GameEnd.lGameScore[i]-=lTmpScore;								
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"����ƽ��");
								
								break;
							}		
					
						case CHR_GANG_HU|CHK_PING_HU:
							{								
								lInitScore=2;
								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;	

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];
								
								GameEnd.lGameScore[i]-=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"���ϻ�");
								
								break;
							}

							//�ν�����
						case CHR_SINGLE_SWIM|CHK_PING_HU:
						case CHK_SINGLE_SWIM:
						case CHK_SINGLE_SWIM|CHR_SINGLE_SWIM:
						case CHK_SINGLE_SWIM|CHR_SINGLE_SWIM|CHK_PING_HU:
							{
								lInitScore=3;
								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;	

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];
								
								GameEnd.lGameScore[i]-=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"�ν�");
								
								break;

							}
						case CHR_GANG_HU|CHR_SINGLE_SWIM|CHK_PING_HU:
						case CHR_GANG_HU|CHK_SINGLE_SWIM:
						case CHR_GANG_HU|CHK_SINGLE_SWIM|CHR_SINGLE_SWIM:
						case CHR_GANG_HU|CHK_SINGLE_SWIM|CHR_SINGLE_SWIM|CHK_PING_HU:
							{
								lInitScore=6;
								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;	

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];
								
								GameEnd.lGameScore[i]-=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"�ν�,���ϻ�");
								
								break;

							}

							//˫������
						case CHR_DOUBLE_SWIM|CHK_PING_HU:
						case CHK_DOUBLE_SWIM:
						case CHK_DOUBLE_SWIM|CHR_DOUBLE_SWIM:
						case CHK_DOUBLE_SWIM|CHR_DOUBLE_SWIM|CHK_PING_HU:
							{
								lInitScore=4;
								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];
								
								GameEnd.lGameScore[i]-=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"˫��");
								
								break;

							}
						case CHR_GANG_HU|CHR_DOUBLE_SWIM|CHK_PING_HU:
						case CHR_GANG_HU|CHK_DOUBLE_SWIM:
						case CHR_GANG_HU|CHK_DOUBLE_SWIM|CHR_DOUBLE_SWIM:
						case CHR_GANG_HU|CHK_DOUBLE_SWIM|CHR_DOUBLE_SWIM|CHK_PING_HU:
							{
								lInitScore=8;
								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;	

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];
								
								GameEnd.lGameScore[i]-=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"˫��,���ϻ�");
								
								break;

							}
							//��������
						case CHR_THREE_SWIM|CHK_PING_HU:
						case CHK_THREE_SWIM:
						case CHK_THREE_SWIM|CHR_THREE_SWIM:
						case CHK_THREE_SWIM|CHR_THREE_SWIM|CHK_PING_HU:
							{
								lInitScore=16;
								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;	

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];
								
								GameEnd.lGameScore[i]-=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"����");
								
								break;

							}
						case CHR_GANG_HU|CHK_THREE_SWIM|CHK_PING_HU:
						case CHR_GANG_HU|CHK_THREE_SWIM:
						case CHR_GANG_HU|CHK_THREE_SWIM|CHR_THREE_SWIM:
						case CHR_GANG_HU|CHK_THREE_SWIM|CHR_THREE_SWIM|CHK_PING_HU:
							{
								lInitScore=32;
								lTmpScore=lInitScore*lBeiShu*lCellScore+lTimeScore;	

								//�ʱ�����
								if(m_lUserCapital[i]<0)lTmpScore=0;
								else if(m_lUserCapital[i]<lTmpScore)lTmpScore=m_lUserCapital[i];
								
								GameEnd.lGameScore[i]-=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]+=lTmpScore;								
								strcpy(msg,"����,���ϻ�");
								
								break;

							}										
						
						default:
							break;

						}
					}		   

					//������Ŀ
					//wChiHuUserCount=1;

					//ׯ������
					if(m_wBankerUser==m_wProvideUser)//ׯ����ׯ
					{
						m_wBankerUser=m_wProvideUser;

						//��������
						m_bGoOnBankerTime*=2;
					}
					else//�¼ҵ�ׯ
					{
						m_wBankerUser = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;

						//��������
						m_bGoOnBankerTime=1;
					}
				}

				//׽������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind==CHK_NULL)
				{

					//��������
					LONG lCellScore = m_pGameServiceOption->lCellScore;

					//ͳ�ƷǺ��������Ӯ����
					BYTE bTmpUserTime=0;
					for(WORD i=0;i<GAME_PLAYER;i++)
					{
						//�������
						if(m_ChiHuResult[i].wChiHuKind!=CHK_NULL)continue;

						bTmpUserTime=0;

						for(BYTE j=0;j<GAME_PLAYER;j++)
						{
							//�������
							if((i==j)||(m_ChiHuResult[j].wChiHuKind!=CHK_NULL))continue;

							bTmpUserTime+=bUserTime[j];
						}

						//ͳ�Ʒ���
						GameEnd.lGameScore[i]+=(2*bUserTime[i]-bTmpUserTime)*lCellScore*m_bGoOnBankerTime;	

					}

					

				
					


					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//��ҹ���
						if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;	

						

						//Ӯ��							
						GameEnd.wWinner[i]=true;						
						
						lBeiShu=1L;	

						//��ׯ����
						lBeiShu*=m_bGoOnBankerTime;
						

						//���Ʒ���
						LONG lTimeScore = bUserTime[i]*lCellScore*m_bGoOnBankerTime;						


						//test 
						/*char szBuffer[20];
						sprintf(szBuffer,TEXT("%d"),m_ChiHuResult[i].wChiHuKind|m_ChiHuResult[i].wChiHuRight);
						AfxMessageBox(szBuffer);*/
						

						switch(m_ChiHuResult[i].wChiHuKind|m_ChiHuResult[i].wChiHuRight)						
						{
						//��ͨ����
						case CHK_PING_HU:	
							{
								lBeiShu*=2;
								
								lTmpScore =lBeiShu*lCellScore+lTimeScore;
								/*

								//�ʱ�����
								if(m_lUserCapital[m_wProvideUser]<0)lTmpScore=0;
								else if(m_lUserCapital[m_wProvideUser]<lTmpScore)lTmpScore=m_lUserCapital[m_wProvideUser];

								GameEnd.lGameScore[i]+=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]-=lTmpScore;	
								*/

								for(BYTE j=0;j<GAME_PLAYER;j++)
								{
									if((i==j)||(m_ChiHuResult[j].wChiHuKind!=CHK_NULL))continue;
									//�ʱ�����
									if(m_lUserCapital[j]<0)lTmpScore=0;
									else if(m_lUserCapital[j]<lTmpScore)lTmpScore=m_lUserCapital[j];

									GameEnd.lGameScore[i]+=lTmpScore;
									GameEnd.lGameScore[j]-=lTmpScore;	

								}
								strcpy(msg,"ƽ��");	
							
								break;
							}			
						
						case  CHR_GANG_HU|CHK_PING_HU:
							{
								
								lBeiShu*=2;

								lTmpScore =lBeiShu*lCellScore+lTimeScore;

								/*
								//�ʱ�����
								if(m_lUserCapital[m_wProvideUser]<0)lTmpScore=0;
								else if(m_lUserCapital[m_wProvideUser]<lTmpScore)lTmpScore=m_lUserCapital[m_wProvideUser];

								GameEnd.lGameScore[i]+=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]-=lTmpScore;	
								*/

								for(BYTE j=0;j<GAME_PLAYER;j++)
								{
									if((i==j)||(m_ChiHuResult[j].wChiHuKind!=CHK_NULL))continue;
									//�ʱ�����
									if(m_lUserCapital[j]<0)lTmpScore=0;
									else if(m_lUserCapital[j]<lTmpScore)lTmpScore=m_lUserCapital[j];

									GameEnd.lGameScore[i]+=lTmpScore;
									GameEnd.lGameScore[j]-=lTmpScore;	

								}																
								
								strcpy(msg,"������");
								
								break;
							}

						//�ν�����
						case CHR_SINGLE_SWIM|CHK_PING_HU:
						case CHK_SINGLE_SWIM:
						case CHK_SINGLE_SWIM|CHR_SINGLE_SWIM:
						case CHK_SINGLE_SWIM|CHR_SINGLE_SWIM|CHK_PING_HU:
							{
								lBeiShu*=6;
								lTmpScore=lBeiShu*lCellScore+lTimeScore;	

								/*
								//�ʱ�����
								if(m_lUserCapital[m_wProvideUser]<0)lTmpScore=0;
								else if(m_lUserCapital[m_wProvideUser]<lTmpScore)lTmpScore=m_lUserCapital[m_wProvideUser];

								GameEnd.lGameScore[i]+=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]-=lTmpScore;	
								*/

								for(BYTE j=0;j<GAME_PLAYER;j++)
								{
									if((i==j)||(m_ChiHuResult[j].wChiHuKind!=CHK_NULL))continue;
									//�ʱ�����
									if(m_lUserCapital[j]<0)lTmpScore=0;
									else if(m_lUserCapital[j]<lTmpScore)lTmpScore=m_lUserCapital[j];

									GameEnd.lGameScore[i]+=lTmpScore;
									GameEnd.lGameScore[j]-=lTmpScore;	

								}								
								strcpy(msg,"�ν�");
								
								break;

							}
						case CHR_GANG_HU|CHR_SINGLE_SWIM|CHK_PING_HU:
						case CHR_GANG_HU|CHK_SINGLE_SWIM:
						case CHR_GANG_HU|CHK_SINGLE_SWIM|CHR_SINGLE_SWIM:
						case CHR_GANG_HU|CHK_SINGLE_SWIM|CHR_SINGLE_SWIM|CHK_PING_HU:
							{
								lBeiShu*=12;
								lTmpScore=lBeiShu*lCellScore+lTimeScore;	

								/*
								//�ʱ�����
								if(m_lUserCapital[m_wProvideUser]<0)lTmpScore=0;
								else if(m_lUserCapital[m_wProvideUser]<lTmpScore)lTmpScore=m_lUserCapital[m_wProvideUser];

								GameEnd.lGameScore[i]+=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]-=lTmpScore;	
								*/

								for(BYTE j=0;j<GAME_PLAYER;j++)
								{
									if((i==j)||(m_ChiHuResult[j].wChiHuKind!=CHK_NULL))continue;
									//�ʱ�����
									if(m_lUserCapital[j]<0)lTmpScore=0;
									else if(m_lUserCapital[j]<lTmpScore)lTmpScore=m_lUserCapital[j];

									GameEnd.lGameScore[i]+=lTmpScore;
									GameEnd.lGameScore[j]-=lTmpScore;	

								}							
								strcpy(msg,"�ν�,���ϻ�");
								
								break;

							}

						//˫������
						case CHR_DOUBLE_SWIM|CHK_PING_HU:
						case CHK_DOUBLE_SWIM:
						case CHK_DOUBLE_SWIM|CHR_DOUBLE_SWIM:
						case CHK_DOUBLE_SWIM|CHR_DOUBLE_SWIM|CHK_PING_HU:
							{
								lBeiShu*=8;
								lTmpScore=lBeiShu*lCellScore+lTimeScore;	

								/*
								//�ʱ�����
								if(m_lUserCapital[m_wProvideUser]<0)lTmpScore=0;
								else if(m_lUserCapital[m_wProvideUser]<lTmpScore)lTmpScore=m_lUserCapital[m_wProvideUser];

								GameEnd.lGameScore[i]+=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]-=lTmpScore;	
								*/

								for(BYTE j=0;j<GAME_PLAYER;j++)
								{
									if((i==j)||(m_ChiHuResult[j].wChiHuKind!=CHK_NULL))continue;
									//�ʱ�����
									if(m_lUserCapital[j]<0)lTmpScore=0;
									else if(m_lUserCapital[j]<lTmpScore)lTmpScore=m_lUserCapital[j];

									GameEnd.lGameScore[i]+=lTmpScore;
									GameEnd.lGameScore[j]-=lTmpScore;	

								}								
								strcpy(msg,"˫��");
								
								break;

							}
						case CHR_GANG_HU|CHR_DOUBLE_SWIM|CHK_PING_HU:
						case CHR_GANG_HU|CHK_DOUBLE_SWIM:
						case CHR_GANG_HU|CHK_DOUBLE_SWIM|CHR_DOUBLE_SWIM:
						case CHR_GANG_HU|CHK_DOUBLE_SWIM|CHR_DOUBLE_SWIM|CHK_PING_HU:
							{
								lBeiShu*=16;
								lTmpScore=lBeiShu*lCellScore+lTimeScore;	

								/*
								//�ʱ�����
								if(m_lUserCapital[m_wProvideUser]<0)lTmpScore=0;
								else if(m_lUserCapital[m_wProvideUser]<lTmpScore)lTmpScore=m_lUserCapital[m_wProvideUser];

								GameEnd.lGameScore[i]+=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]-=lTmpScore;	
								*/

								for(BYTE j=0;j<GAME_PLAYER;j++)
								{
									if((i==j)||(m_ChiHuResult[j].wChiHuKind!=CHK_NULL))continue;
									//�ʱ�����
									if(m_lUserCapital[j]<0)lTmpScore=0;
									else if(m_lUserCapital[j]<lTmpScore)lTmpScore=m_lUserCapital[j];

									GameEnd.lGameScore[i]+=lTmpScore;
									GameEnd.lGameScore[j]-=lTmpScore;	

								}									
								strcpy(msg,"˫��,���ϻ�");
								
								break;

							}

						//��������
						case CHR_THREE_SWIM|CHK_PING_HU:
						case CHK_THREE_SWIM:
						case CHK_THREE_SWIM|CHR_THREE_SWIM:
						case CHK_THREE_SWIM|CHR_THREE_SWIM|CHK_PING_HU:
							{
								lBeiShu*=32;
								lTmpScore=lBeiShu*lCellScore+lTimeScore;	

								/*
								//�ʱ�����
								if(m_lUserCapital[m_wProvideUser]<0)lTmpScore=0;
								else if(m_lUserCapital[m_wProvideUser]<lTmpScore)lTmpScore=m_lUserCapital[m_wProvideUser];

								GameEnd.lGameScore[i]+=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]-=lTmpScore;	
								*/

								for(BYTE j=0;j<GAME_PLAYER;j++)
								{
									if((i==j)||(m_ChiHuResult[j].wChiHuKind!=CHK_NULL))continue;
									//�ʱ�����
									if(m_lUserCapital[j]<0)lTmpScore=0;
									else if(m_lUserCapital[j]<lTmpScore)lTmpScore=m_lUserCapital[j];

									GameEnd.lGameScore[i]+=lTmpScore;
									GameEnd.lGameScore[j]-=lTmpScore;	

								}								
								strcpy(msg,"����");
								
								break;

							}
						case CHR_GANG_HU|CHK_THREE_SWIM|CHK_PING_HU:
						case CHR_GANG_HU|CHK_THREE_SWIM:
						case CHR_GANG_HU|CHK_THREE_SWIM|CHR_THREE_SWIM:
						case CHR_GANG_HU|CHK_THREE_SWIM|CHR_THREE_SWIM|CHK_PING_HU:
							{
								lBeiShu*=64;
								lTmpScore=lBeiShu*lCellScore+lTimeScore;	
								
								/*
								//�ʱ�����
								if(m_lUserCapital[m_wProvideUser]<0)lTmpScore=0;
								else if(m_lUserCapital[m_wProvideUser]<lTmpScore)lTmpScore=m_lUserCapital[m_wProvideUser];

								GameEnd.lGameScore[i]+=lTmpScore;
								GameEnd.lGameScore[m_wProvideUser]-=lTmpScore;	
								*/

								for(BYTE j=0;j<GAME_PLAYER;j++)
								{
									if((i==j)||(m_ChiHuResult[j].wChiHuKind!=CHK_NULL))continue;
									//�ʱ�����
									if(m_lUserCapital[j]<0)lTmpScore=0;
									else if(m_lUserCapital[j]<lTmpScore)lTmpScore=m_lUserCapital[j];

									GameEnd.lGameScore[i]+=lTmpScore;
									GameEnd.lGameScore[j]-=lTmpScore;	

								}									
								strcpy(msg,"����,���ϻ�");
								
								break;

							}
										
						
						default:							
							break;

						}

						if(i==m_wBankerUser)
						{							
							//��������
							m_bGoOnBankerTime*=2;
						}
						else//�¼ҵ�ׯ
						{
							m_wBankerUser = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;

							//��������
							m_bGoOnBankerTime=1;
						}
						
						//ׯ������
						//if(wNextBankerUser==INVALID_CHAIR)m_wBankerUser=i;

						

						//����ͳ��

						
						//���ñ���
						//wNextBankerUser=i;						
						//wChiHuUserCount++;
						
						
					}
					//ׯ������
					//m_wBankerUser=(wChiHuUserCount>=2)?m_wProvideUser:wNextBankerUser;
				}				
			}			
			else
			{	
							
					GameEnd.cbChiHuCard=0;
				
			}
			
			//�����齫
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(GameEnd.wWinner[i]==false)
					GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
				else 
					GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCurCardIndex[i],GameEnd.cbCardData[i]);
			}		

			if(GameEnd.cbChiHuCard==0)
			{			
				ZeroMemory(GameEnd.strEnd,sizeof(GameEnd.strEnd));
			}
			else
			    strcpy(GameEnd.strEnd,msg) ;

			//������Ϣ

			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/10L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*45L/50L;
					}
				}
			}
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_GameEnd������� �����û�wProvideUser=[%d],�Ժ��齫cbChiHuCard=[%x],��������wChiHuKind[0,1,2,3]=[%d,%d,%d,%d],�������bFlowerCount[0,1,2,3]=[%d,%d,%d,%d],�׷ִ�СlCellScore=[%d],��Ϸ����lGameScore[0,1,2,3]=[%lld,%lld,%lld,%lld]"),GameEnd.wProvideUser,GameEnd.cbChiHuCard,GameEnd.wChiHuKind[0],GameEnd.wChiHuKind[1],GameEnd.wChiHuKind[2],GameEnd.wChiHuKind[3],GameEnd.bFlowerCount[0],GameEnd.bFlowerCount[1],GameEnd.bFlowerCount[2],GameEnd.bFlowerCount[3],GameEnd.lCellScore,GameEnd.lGameScore[0],GameEnd.lGameScore[1],GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			CString strLog;
			strLog.Format(_T("CMD_S_GameEnd������� i=[%d]��,�齫��ĿcbCardCount=[%d],[%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x]"),i,GameEnd.cbCardCount[i],GameEnd.cbCardData[i][0],GameEnd.cbCardData[i][1],GameEnd.cbCardData[i][2],GameEnd.cbCardData[i][3],GameEnd.cbCardData[i][4],GameEnd.cbCardData[i][5],GameEnd.cbCardData[i][6],GameEnd.cbCardData[i][7],GameEnd.cbCardData[i][1+7],GameEnd.cbCardData[i][2+7],GameEnd.cbCardData[i][3+7],GameEnd.cbCardData[i][4+7],GameEnd.cbCardData[i][5+7],GameEnd.cbCardData[i][6+7]);
			CTraceService::TraceString(strLog,TraceLevel_Normal);	
			//ofstream outf(g_wcTmp,ios::app); 
			outf<<CT2A(strLog)<<endl;
		}
#endif

			//�޸Ļ���
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				if (GameEnd.lGameScore[i]==0L) ScoreInfo[i].cbType=SCORE_TYPE_DRAW;
				else ScoreInfo[i].cbType=(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
                m_pITableFrame->WriteUserScore(i,ScoreInfo[i]);
			}
			//m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,0L);

			//������Ϸ
			//m_pITableFrame->ConcludeGame(GS_MJ_FREE);
			//������Ϸ
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:	//�����ж�
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.lGameScore[wChairID]=-50*m_pGameServiceOption->lCellScore;
			GameEnd.lCellScore = m_pGameServiceOption->lCellScore;
			GameEnd.bGoOnBankerTime = m_bGoOnBankerTime;
			//test
			for(WORD k=0;k<GAME_PLAYER;k++)
			{
				if(k==wChairID)continue;
					GameEnd.lGameScore[k]=8*m_pGameServiceOption->lCellScore;
					GameEnd.bFlowerCount[k]=m_cbFlowerCount[k];
					GameEnd.bGoldCount[k]=m_cbGoldCount[k];	
					GameEnd.bGangCount[k]=m_cbGangCount[k];
			}

			//֪ͨ��Ϣ
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetNickName());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
				if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
			}
			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL) break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_EJECT|SMT_CHAT);
			} while (true);

		
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/10L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*45L/50L;
					}
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_GameEnd[%d] �����û�wProvideUser=[%d],�Ժ��齫cbChiHuCard=[%x],��������wChiHuKind[0,1,2,3]=[%d,%d,%d,%d],�������bFlowerCount[0,1,2,3]=[%d,%d,%d,%d],�׷ִ�СlCellScore=[%d],��Ϸ����lGameScore[0,1,2,3]=[%lld,%lld,%lld,%lld]"),cbReason,GameEnd.wProvideUser,GameEnd.cbChiHuCard,GameEnd.wChiHuKind[0],GameEnd.wChiHuKind[1],GameEnd.wChiHuKind[2],GameEnd.wChiHuKind[3],GameEnd.bFlowerCount[0],GameEnd.bFlowerCount[1],GameEnd.bFlowerCount[2],GameEnd.bFlowerCount[3],GameEnd.lCellScore,GameEnd.lGameScore[0],GameEnd.lGameScore[1],GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			CString strLog;
			strLog.Format(_T("CMD_S_GameEnd[%d] i=[%d]��,�齫��ĿcbCardCount=[%d],[%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x]"),cbReason,i,GameEnd.cbCardCount[i],GameEnd.cbCardData[i][0],GameEnd.cbCardData[i][1],GameEnd.cbCardData[i][2],GameEnd.cbCardData[i][3],GameEnd.cbCardData[i][4],GameEnd.cbCardData[i][5],GameEnd.cbCardData[i][6],GameEnd.cbCardData[i][7],GameEnd.cbCardData[i][1+7],GameEnd.cbCardData[i][2+7],GameEnd.cbCardData[i][3+7],GameEnd.cbCardData[i][4+7],GameEnd.cbCardData[i][5+7],GameEnd.cbCardData[i][6+7]);
			CTraceService::TraceString(strLog,TraceLevel_Normal);	
			//ofstream outf(g_wcTmp,ios::app); 
			outf<<CT2A(strLog)<<endl;
		}
#endif

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ScoreInfo.cbType=SCORE_TYPE_FLEE;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

			//������Ϸ
			//m_pITableFrame->ConcludeGame(SCORE_TYPE_FLEE);
			//������Ϸ
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif
			return true;
		}
	}

	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wSiceCount=m_wDoorDiceCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//����齫
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//�齫����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_MJ_BEFOREPLAY:
		{
			return m_pITableFrame->SendGameScene(pIServerUserItem,NULL,0);
		}	
	}

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID,void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_C_OutCard wChairID=[%d],�齫����cbCardData=[%x]"),pUserData->wChairID,pOutCard->cbCardData);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_C_OperateCard wChairID=[%d],��������cbOperateCode=[%d],�����齫cbOperateCard=[%x]"),pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_OPENDOOR://������Ϣ
		{	
			//�û�Ч��
			tagUserInfo *pUserData = pIServerUserItem->GetUserInfo();
			if(pUserData->cbUserStatus!=US_PLAYING)return true;
#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(_T("SUB_C_OPENDOOR wChairID=[%d]"),pUserData->wChairID);
	CTraceService::TraceString(strLog,TraceLevel_Normal);	
	ofstream outf(g_wcTmp,ios::app); 
	outf<<CT2A(strLog)<<endl;
#endif
			return OnUserOpenDoor();
		}

	case SUB_C_OPENGOLD://������Ϣ
		{
			//�û�Ч��
			tagUserInfo *pUserData = pIServerUserItem->GetUserInfo();
			if(pUserData->cbUserStatus!=US_PLAYING)return true;
#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(_T("SUB_C_OPENGOLD wChairID=[%d]"),pUserData->wChairID);
	CTraceService::TraceString(strLog,TraceLevel_Normal);	
	ofstream outf(g_wcTmp,ios::app); 
	outf<<CT2A(strLog)<<endl;
#endif
			return OnUserOpenGold();
		}
	case SUB_C_REPAIRFLOWER://������Ϣ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_RepairFlower));
			if(wDataSize!=sizeof(CMD_C_RepairFlower))return false;

			//�û�Ч��
			tagUserInfo *pUserData = pIServerUserItem->GetUserInfo();
			if(pUserData->cbUserStatus!=US_PLAYING)return true;
#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(_T("SUB_C_REPAIRFLOWER wChairID=[%d]"),pUserData->wChairID);
	CTraceService::TraceString(strLog,TraceLevel_Normal);	
	ofstream outf(g_wcTmp,ios::app); 
	outf<<CT2A(strLog)<<endl;
#endif
			return OnUserRepairFlower(pDataBuffer);


		}

	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID,void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();
	}

	//��������
	m_bGoOnBankerTime = 1;

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(wChairID==m_wBankerUser))
	{
		m_wBankerUser=INVALID_CHAIR;

		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((i!=wChairID)&&(m_pITableFrame->GetTableUserItem(i)!=NULL))
			{
				m_wBankerUser=i;
				break;
			}
		}
	}

	//��������
	m_bGoOnBankerTime = 1;

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�������
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//ɾ���齫
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��Ϸ��ʼ
	if(m_bGameStart&&(wChairID!=m_wBankerUser))m_bGameStart=false;


	//���ñ���
	m_bSendStatus=true;
	m_bEnjoinChiHu[wChairID]=true;
	BYTE cbIndex=m_GameLogic.SwitchToCardIndex(cbCardData);
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(i==wChairID)
		{
			m_bEnjoinChiPeng[i]=true;
			continue;
		}

		//if(m_cbDisPengIndex[i][cbIndex]>0)//��������
			//m_bEnjoinChiPeng[i]=true;
		//else
			m_bEnjoinChiPeng[i]=false;
		//if(m_cbDisHuIndex[i][cbIndex]>0)//��������
			//m_bEnjoinChiHu[i]=true;
		//else
			m_bEnjoinChiHu[i]=false;
	}
	
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//���Ƽ�¼
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_OutCard �����û�wOutCardUser=[%d],�����齫cbOutCardData=[%x]"),wChairID,cbCardData);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//�������
	if(m_GameLogic.IsGoldCard(cbCardData,m_cbGoldCard,4))m_cbGoldCount[wChairID]++;//���ñ���

	if(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY)
	{
		//����״̬
		bool bGoldSwim=false;
		tagChiHuResult ChiHuResult;
		WORD wChiHuRight=0;
		BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
		tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
		/*for(BYTE i=0;i<4;i++)
		{
			bGoldCardEye=false;
			m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbGoldCard[i],WIK_NULL,ChiHuResult,m_cbGoldCard,4,bGoldCardEye);
			if(bGoldCardEye)break;
		}*/
		//�ж�����
		//CopyMemory(m_cbCurCardIndex[wChairID],m_cbCardIndex[wChairID],sizeof(m_cbCardIndex[wChairID]));
		if(m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,cbCardData,wChiHuRight,ChiHuResult,m_cbGoldCard,4,m_cbCurCardIndex[wChairID]))
			bGoldSwim=true;

		if(bGoldSwim==true)
		{
			if(m_UserGoldSwim[wChairID]==GoldSwim_None)//������״̬
			{
				m_UserGoldSwim[wChairID]=GoldSwim_Single;
			}
			else 
			{
				if(m_GameLogic.IsGoldCard(cbCardData,m_cbGoldCard,4))//�������
				{
					//���α�˫��
					if(m_UserGoldSwim[wChairID]==GoldSwim_Single)m_UserGoldSwim[wChairID]=GoldSwim_Double;
					else m_UserGoldSwim[wChairID]=GoldSwim_Three;//˫�α�����

					
				}
				else 
					m_UserGoldSwim[wChairID]=GoldSwim_Single;//����״̬
			}
		}
		else
		{
			m_UserGoldSwim[wChairID]=GoldSwim_None;//������״̬
		}

		//������Ϣ
		CMD_S_GoldSwim GoldSwim;

		for(WORD i=0;i<GAME_PLAYER;i++)
			GoldSwim.GoldSwim[i]=m_UserGoldSwim[i];

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GOLD_SWIM,&GoldSwim,sizeof(GoldSwim));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GOLD_SWIM,&GoldSwim,sizeof(GoldSwim));
		
	}



	//�û��л�
	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;		
	

	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//��������
	if (m_bGangStatus==true)
	{
		WORD i=0;
		for (i=0;i<m_wPlayerCount;i++)
		{
			if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) break;
		}
		if (i==m_wPlayerCount)
		{
			m_bGangStatus=false;
		}
	}

	

	
	//�ɷ��齫
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser,false);
	//else AfxMessageBox("110");

	

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//Ч���û�
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return false;

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		ASSERT(m_bResponse[wChairID]==false);
		ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//Ч��״̬
		if (m_bResponse[wChairID]==true) return false;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//��������
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//���ñ���
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;

		if(m_cbPerformAction[wChairID]==WIK_GANG)
		{			
			m_bGangStatus=false;
		}

		m_cbOperateCard[wChairID]=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;
		
		
		//ִ���ж�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ����
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];	

			

			//���ȼ���
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);				

			//�����ж�
			if (cbUserActionRank>cbTargetActionRank)
			{				
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}

		}		

		if (m_bResponse[wTargetUser]==false)
		{
			return true;
		}

		//�Ժ��ȴ�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU)) return true;
			}
		}

		//��������
		if (cbTargetAction==WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
			for(BYTE i=0;i<4;i++)m_bHuUser[i]=false;

			//���Ƽ�¼
			BYTE cbIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

			//����δ��
			//if(m_GameLogic.EstimatePengCard(m_cbCardIndex[wChairID],cbOperateCard))
				//m_cbDisPengIndex[wChairID][cbIndex]++;

			//�ܺ�δ��			
			/*WORD wChiHuRight=0;
			tagChiHuResult ChiHuResult;
			CopyMemory(m_cbCurCardIndex[wChairID],m_cbCardIndex[wChairID],sizeof(m_cbCardIndex[wChairID]));
			//if(m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID],cbOperateCard,wChiHuRight,ChiHuResult,m_cbGoldCard,4,bGoldCardEye))
			m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wChairID],m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID],cbOperateCard,wChiHuRight,ChiHuResult,m_cbGoldCard,4,m_cbCurCardIndex[wChairID]);
			if(ChiHuResult.wChiHuKind!=WIK_NULL)	
				m_cbDisHuIndex[wChairID][cbIndex]++;*/

			DispatchCardData(m_wResumeUser,false);			
			return true;
		}

		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser];

		//���Ʊ���
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//���Ʋ���
		if (cbTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;
			m_wProvideUser=m_wProvideUser;			

			//����Ȩλ
			WORD wChiHuRight=0;			
			if (m_bGangStatus==true)  wChiHuRight|=CHR_GANG_HU;
			

			//�����ж�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�����ж�
				if ((i==m_wProvideUser)||((m_cbPerformAction[i]&WIK_CHI_HU)==0)) 
				{
                    //����״̬
					if(m_UserGoldSwim[i]!=GoldSwim_None)m_UserGoldSwim[i]=GoldSwim_Single;//˫�Σ�����û���ι�
					continue;
				}

				//��ͨ����
				if (m_cbChiHuCard!=0)
				{
					//�����ж�					
					BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
					//CopyMemory(m_cbCurCardIndex[i],m_cbCardIndex[i],sizeof(m_cbCardIndex[i]));
					//m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,wChiHuRight,m_ChiHuResult[i],m_cbGoldCard,4,bGoldCardEye);	
					m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,wChiHuRight,m_ChiHuResult[i],m_cbGoldCard,4,m_cbCurCardIndex[i]);
					m_bHuUser[i]=true;

					//�����齫
					if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) 
					{
						m_cbCurCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
						m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					}
					
					
				}			
				
			}
			
			//������Ϸ
			ASSERT(m_ChiHuResult[wTargetUser].wChiHuKind!=CHK_NULL);

			

			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;

			
		}

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//����齫
		ASSERT(m_cbWeaveItemCount[wTargetUser]<5);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//ɾ���齫
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//���Ʋ���
			{
				//ɾ���齫
				BYTE cbRemoveCard[]={cbTargetCard+1,cbTargetCard+2};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//ɾ���齫
				BYTE cbRemoveCard[]={cbTargetCard-2,cbTargetCard-1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//ɾ���齫
				BYTE cbRemoveCard[]={cbTargetCard-1,cbTargetCard+1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//ɾ���齫
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				
				break;
			}
		case WIK_GANG:		//���Ʋ���
			{
				
				//ɾ���齫
				if ((m_cbSendCardCount==1)&&(m_cbOutCardData==0))
				{
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}
				else
				{
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}
				break;
			}
		}

		//������
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCard=cbTargetCard;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//����״̬
		if (cbTargetAction==WIK_GANG)
		{
					
			m_bGangStatus=true;
			m_bEnjoinChiPeng[wTargetUser]=true;
		}

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_OperateResult �����û�wOperateUser=[%d],��Ӧ�û�wProvideUser=[%d],��������cbOperateCode=[%d],�����齫cbOperateCard=[%x]"),OperateResult.wOperateUser,OperateResult.wProvideUser,OperateResult.cbOperateCode,OperateResult.cbOperateCard);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif		

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//�����û�
		m_wCurrentUser=wTargetUser;

		//���ƴ���		
		if (cbTargetAction==WIK_GANG)
		{
			//Ч�鶯��
			bool bAroseAction=EstimateUserRespond(wTargetUser,cbTargetCard,EstimatKind_GangCard);
			
			//���Ƽ���
			m_cbGangCount[wChairID]++;

			//��������+1
			if(m_cbMaxLeftCount==MAX_LEFT_COUNT)
			m_cbMaxLeftCount = MAX_LEFT_COUNT+1;
			//if(cbTargetAction==WIK_GANG)
			//{
				//m_GangScore[m_wCurrentUser]=m_GangScore[m_wCurrentUser]+1*m_pGameServiceOption->lCellScore;
				//m_GangScore[m_wProvideUser]=m_GangScore[m_wProvideUser]-1*m_pGameServiceOption->lCellScore;

				//��ܼǷ�

			/*	LONG  lBeiShu=1,lTmpBeiShu=1;
				//if(m_bUserChui[wChairID])lBeiShu*=2;
				for(WORD k=0;k<GAME_PLAYER;k++)
				{
					if(k==wChairID)continue;
					lTmpBeiShu=1;
					//if(m_bUserChui[k]&&(k!=wChairID))
						//lTmpBeiShu*=2;					
					m_GangScore[wChairID]+=m_pGameServiceOption->lCellScore*10*lBeiShu*lTmpBeiShu;						
					if(k!=wChairID)
						m_GangScore[k]-=1*m_pGameServiceOption->lCellScore*10*lBeiShu*lTmpBeiShu;
				}*/
			//}
			//�����齫
			if (bAroseAction==false) DispatchCardData(wTargetUser,false);

			return true;
		}

		//�����ж�
		if ((MAX_REPERTORY-m_cbSendCardCount-64)>m_cbMaxLeftCount)
		{
			//�����ж�
			tagGangCardResult GangCardResult;
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);

			//�������
			if (GangCardResult.cbCardCount>0)
			{
				//���ñ���				
				m_cbUserAction[m_wCurrentUser]|=WIK_GANG;
				

				//���Ͷ���
				SendOperateNotify();
			}
		}

		return true;
	}

	//��������
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//�齫Ч��
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard)==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false)) return false;

		//���ñ���
		m_bSendStatus=true;
		m_cbUserAction[wChairID]=WIK_NULL;
		m_cbPerformAction[wChairID]=WIK_NULL;

		//ִ�ж���
		switch (cbOperateCode)
		{		
		case WIK_GANG:			//���Ʋ���
		{
				//��������
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				//���ƴ���
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//Ѱ�����
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
						{
							cbWeaveIndex=i;

							//���Ƽ���
							m_cbGangCount[wChairID]+=2;

							//��������+1
							if(m_cbMaxLeftCount==MAX_LEFT_COUNT)
								m_cbMaxLeftCount = MAX_LEFT_COUNT+1;						

							break;
						}
					}

					//Ч�鶯��
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//����齫
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}
				else
				{
					//�齫Ч��
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) return false;

					//���ñ���
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;


					//���Ƽ���
					m_cbGangCount[wChairID]+=2;

					//��������+1
					if(m_cbMaxLeftCount==MAX_LEFT_COUNT)
						m_cbMaxLeftCount = MAX_LEFT_COUNT+1;					
				}

				//ɾ���齫
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				//����״̬				
				if (cbOperateCode==WIK_GANG)
				{
										
					m_bGangStatus=true;
					m_bEnjoinChiPeng[wChairID]=true;
					
				}

				

				//������
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard=cbOperateCard;

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_OperateResult���Ʋ��� �����û�wOperateUser=[%d],��Ӧ�û�wProvideUser=[%d],��������cbOperateCode=[%d],�����齫cbOperateCard=[%x]"),OperateResult.wOperateUser,OperateResult.wProvideUser,OperateResult.cbOperateCode,OperateResult.cbOperateCard);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//Ч�鶯��
				bool bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);

				/*BYTE mbHu=WIK_NULL;					
				for(BYTE i=0;i<GAME_PLAYER;i++)
				{

					if(i==wChairID)continue;

					//�����ж�	
					bool bGoldCardEye=false;
					tagChiHuResult ChiHuResult;
					BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
					mbHu=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,cbOperateCard,WIK_NULL,ChiHuResult,m_cbGoldCard,4,bGoldCardEye);	
									
				}	*/			

				//�����齫
				if (bAroseAction==false) DispatchCardData(wChairID,false);

				return true;
			}
		case WIK_CHI_HU:		//�Ժ�����(������������������)
			{
				//����Ȩλ
				WORD wChiHuRight=0;
				if (m_bGangStatus==true) wChiHuRight|=CHR_GANG_HU;	

				//����Ȩλ
				switch(m_UserGoldSwim[wChairID])
				{
				case GoldSwim_Three:
					{
						wChiHuRight|=CHR_THREE_SWIM;
					
						break;
					}
				case GoldSwim_Double:
					{
						wChiHuRight|=CHR_DOUBLE_SWIM;
					
						break;
					}
				case GoldSwim_Single:
					{
						wChiHuRight|=CHR_SINGLE_SWIM;
					
						break;
					}
				}
			
				//��ͨ����				
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				//CopyMemory(m_cbCurCardIndex[wChairID],m_cbCardIndex[wChairID],sizeof(m_cbCardIndex[wChairID]));
				//m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,wChiHuRight,m_ChiHuResult[wChairID],m_cbGoldCard,4,bGoldCardEye);
				m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,wChiHuRight,m_ChiHuResult[wChairID],m_cbGoldCard,4,m_cbCurCardIndex[wChairID]);
				m_bHuUser[wChairID]=true;

				//����״̬
				for(WORD i=0;i<GAME_PLAYER;i++)
				{
					if(i==wChairID)continue;

					//���Ʊ�ֵ,˫�Σ�����δ�ι�
					if(m_UserGoldSwim[i]!=GoldSwim_None)m_UserGoldSwim[i]=GoldSwim_Single;
				}

				//������Ϣ
				m_cbChiHuCard=m_cbProvideCard;

				//������Ϸ				
				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
	//������ʾ
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//��������
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_OperateNotify i=[%d]��,��ԭ�û�wResumeUser=[%d],�����˿�cbActionCard=[%x],��������cbActionMask=[%d]"),i,OperateNotify.wResumeUser,OperateNotify.cbActionCard,OperateNotify.cbActionMask);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//�ɷ��齫
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bRepairFlower)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//�����齫
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//��ׯ����
	if((MAX_REPERTORY-m_cbSendCardCount-64)<=m_cbMaxLeftCount)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}


	//���ƴ���
	if (m_bSendStatus==true)
	{
		//�����齫
		
		
		if(bRepairFlower)//���в���
		{
			//�Ӻ����ȡ�˿�
			

			//���������齫
			if(m_cbRepairFlowerPos==(m_wGoldDiceCount-1))m_cbRepairFlowerPos++;

            //�����齫
			m_cbSendCardData = m_cbRepertoryCard[m_cbRepairFlowerPos++];
			m_cbSendCardCount++;

			m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;			

			
		}
		else
		{			
			m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
			//while((m_cbSendCardData==0x00)&&m_cbLeftCardCount)
			//{
				//m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];				
			//}
			//if(m_cbSendCardData!=0x00)
			//{
				m_cbSendCardCount++;
				m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
			//}			
		}

		
		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;

		//�����ж�
		if ((MAX_REPERTORY-m_cbSendCardCount-64)>m_cbMaxLeftCount)
		{
			tagGangCardResult GangCardResult;
			m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
				m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
		}

		//����Ȩλ
		WORD wChiHuRight=0;		
		if (m_bGangStatus==true) wChiHuRight|=CHR_GANG_HU;	

		//����״̬
		m_GoldSwim =  GetCurrentGoldSwim();

		//�����ж�
		if((m_GoldSwim!=GoldSwim_Three)||//����������
			((m_GoldSwim==m_UserGoldSwim[m_wCurrentUser])||(m_bGangStatus==true)))//�Լ�������
		{
			tagChiHuResult ChiHuResult;		
			//CopyMemory(m_cbCurCardIndex[wCurrentUser],m_cbCardIndex[wCurrentUser],sizeof(m_cbCardIndex[wCurrentUser]));
			//m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],
				//m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],0,wChiHuRight,ChiHuResult,m_cbGoldCard,4,bGoldCardEye);
			m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wCurrentUser],
				m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],0,wChiHuRight,ChiHuResult,m_cbGoldCard,4,m_cbCurCardIndex[wCurrentUser]);
		}
	}

	//���ñ���
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.bRepairFlower = bRepairFlower;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_SendCard ��ǰ�û�wCurrentUser=[%d],��������bRepairFlower=[%d],�齫����cbCardData=[%x],��������cbActionMask=[%d]"),SendCard.wCurrentUser,SendCard.cbCardData,SendCard.bRepairFlower,SendCard.cbActionMask);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

	//״̬�ж�
	if(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY)
		SendCard.bGameSatusPlay = true;
	else
		SendCard.bGameSatusPlay = false;


	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//��������
	bool bAroseAction=false;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�û�����
		if (wCenterUser==i) continue;

		//��������
		if (EstimatKind==EstimatKind_OutCard)
		{
			//�����ж�
			if (m_bEnjoinChiPeng[i]==false)
			{
				//�����ж�
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				//test
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i) m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);
			}

			//�����ж�
			if (((MAX_REPERTORY-m_cbSendCardCount-52)>m_cbMaxLeftCount)&&(m_bGameStart==true)) 
			{
				m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
			}
		}

		//�����ж�
		if (m_bEnjoinChiHu[i]==false)
		{
			//����Ȩλ
			WORD wChiHuRight=0;			
			 if(m_bGangStatus==true) wChiHuRight|=CHR_GANG_HU;			

			 //����״̬
			 m_GoldSwim=GetCurrentGoldSwim();

			 if((m_GoldSwim!=GoldSwim_Double)&&//������˫��
				 (m_GoldSwim!=GoldSwim_Three))//����������
			 {

				 //�Ժ��ж�				 
				tagChiHuResult ChiHuResult;
				BYTE cbWeaveCount=m_cbWeaveItemCount[i];
				//CopyMemory(m_cbCurCardIndex[i],m_cbCardIndex[i],sizeof(m_cbCardIndex[i]));
				//m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult,m_cbGoldCard,4,bGoldCardEye);
				m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult,m_cbGoldCard,4,m_cbCurCardIndex[i]);
				
				//�Ժ�����			
				if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) 
				{
					//�ж��ǲ�������
					if((m_cbUserAction[i]&CHK_SKY_SWIM)!=0)
					{									
						m_cbUserAction[i]^=CHK_SKY_SWIM;
					}

					m_bEnjoinChiHu[i]=true;

				}

				
			 }		

		
		}

		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
	}
	

	//�������
	if (bAroseAction==true) 
	{
		//���ñ���
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//������ʾ
		SendOperateNotify();

		return true;
	}

	return false;
}

// ������Ϣ
bool CTableFrameSink::OnUserOpenDoor()
{
	//���ñ���
	m_wDoorDiceCount  = rand()%6+1+rand()%6+1;

	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];			
	m_cbSendCardCount++;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//�������
	WORD wOpenGoldUser = (m_wBankerUser+m_wDoorDiceCount-2+GAME_PLAYER)%GAME_PLAYER;

	m_cbProvideCard=m_cbSendCardData;
	m_cbDoorCard=m_cbSendCardData;

	//������Ϣ
	CMD_S_OpenDoor OpenDoor;
	OpenDoor.wDiceCount = m_wDoorDiceCount;	
	OpenDoor.cbDoorCard=m_cbSendCardData;
	OpenDoor.wOpenGoldUser = wOpenGoldUser;

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_OpenDoor ���Ӵ�СwDiceCount=[%d],���ƴ�СcbDoorCard=[%x],�������wOpenGoldUser=[%d]"),OpenDoor.wDiceCount,OpenDoor.cbDoorCard,OpenDoor.wOpenGoldUser);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPENDOOR,&OpenDoor,sizeof(OpenDoor));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPENDOOR,&OpenDoor,sizeof(OpenDoor));

	return true;
}

// ������Ϣ
bool CTableFrameSink::OnUserOpenGold()
{
	m_wGoldDiceCount = rand()%6+1+rand()%6+1;

	//��ȡ����
	//BYTE cbGoldCard = m_cbRepertoryCard[m_wGoldDiceCount-1];
    BYTE cbGoldCard=FindMagicCard(&m_wGoldDiceCount);

	//��������
	BYTE cbGoldIndex = m_GameLogic.SwitchToCardIndex(cbGoldCard);
    m_GameLogic.SetMagicIndex(cbGoldIndex);
	if(cbGoldIndex<SPRING_INDEX)//����ΪͲ�������������ϣ����������У��������е�һ��
	{
		// ���Ʋ����ǻ���,����򻯴���,һ����ִ�е�����<by hxh 20170309>
		for(BYTE i=0;i<4;i++)
			m_cbGoldCard[i]=cbGoldCard;

		//�����齫
		ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));
		for(BYTE i=0;i<8;i++)
			m_cbFlowerCard[i]=SPRING_DATA+i;
	}
	else
	{
		if(cbGoldIndex<SPRING_INDEX+4)//�����ﶬΪ����
		{
			for(BYTE i=0;i<4;i++)
				m_cbGoldCard[i]=m_GameLogic.SwitchToCardData(SPRING_INDEX)+i;

			//�����齫
			ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));
			for(BYTE i=0;i<4;i++)
				m_cbFlowerCard[i]=SPRING_DATA+4+i;
		}
		else//÷������Ϊ����
		{
			for(BYTE i=0;i<4;i++)
				m_cbGoldCard[i]=m_GameLogic.SwitchToCardData(38)+i;

			//�����齫
			ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));
			for(BYTE i=0;i<4;i++)
				m_cbFlowerCard[i]=SPRING_DATA+i;			
		}
	}

	//���ñ���
	m_cbSendCardCount++;

	//������Ϣ
	CMD_S_OpenGold OpenGold;
	OpenGold.wDiceCount = m_wGoldDiceCount;	
	CopyMemory(OpenGold.cbGoldCard,m_cbGoldCard,sizeof(m_cbGoldCard));
	OpenGold.cbKeyGoldCard = cbGoldCard;
	CopyMemory(OpenGold.cbFlowerCard,m_cbFlowerCard,sizeof(m_cbFlowerCard));

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_OpenGold ���Ӵ�СwDiceCount=[%d],�����齫cbGoldCard=[%x,%x,%x,%x],�ؼ��齫cbKeyGoldCard=[%x],�����齫wOpenGoldUser=[%x,%x,%x,%x,%x,%x,%x,%x]"),OpenGold.wDiceCount,OpenGold.cbGoldCard[0],OpenGold.cbGoldCard[1],OpenGold.cbGoldCard[2],OpenGold.cbGoldCard[3],OpenGold.cbKeyGoldCard,OpenGold.cbFlowerCard[0],OpenGold.cbFlowerCard[1],OpenGold.cbFlowerCard[2],OpenGold.cbFlowerCard[3],OpenGold.cbFlowerCard[4],OpenGold.cbFlowerCard[5],OpenGold.cbFlowerCard[6],OpenGold.cbFlowerCard[7]);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPENGOLD,&OpenGold,sizeof(OpenGold));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPENGOLD,&OpenGold,sizeof(OpenGold));

    // by hxh 20170310
    bool bRet=OnRepairFlowerStage(m_wBankerUser,false);

	return true;
}



// ������Ϣ
bool CTableFrameSink::OnUserRepairFlower(const void *pBuffer)
{

	//ת������
	CMD_C_RepairFlower *pRepairFlower = (CMD_C_RepairFlower *)pBuffer;

#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(_T("CMD_C_RepairFlower wChairID=[%d],cbCurrentCard=[%x]"),pRepairFlower->wChairID,pRepairFlower->cbCurrentCard);
	CTraceService::TraceString(strLog,TraceLevel_Normal);	
	ofstream outf(g_wcTmp,ios::app); 
	outf<<CT2A(strLog)<<endl;
#endif

	//���ñ���
	m_bSendStatus = true;
	m_wCurrentUser = pRepairFlower->wChairID;
	m_cbOutCardData = pRepairFlower->cbCurrentCard;

	//ɾ���˿�
	m_GameLogic.RemoveCard(m_cbCardIndex[m_wCurrentUser],pRepairFlower->cbCurrentCard);

	//�������
	if(m_GameLogic.IsFlowerCard(pRepairFlower->cbCurrentCard,m_cbFlowerCard,8))
		m_cbFlowerCount[pRepairFlower->wChairID]++;	
	

	//�ж�״̬
	if(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY)//��Ϸ�����У���Ҳ���
	{	
#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(_T("��Ϸ�����У���Ҳ��� m_wCurrentUser=[%d]"),m_wCurrentUser);
	CTraceService::TraceString(strLog,TraceLevel_Normal);	
	ofstream outf(g_wcTmp,ios::app); 
	outf<<CT2A(strLog)<<endl;
#endif

		//�����˿�
		DispatchCardData(m_wCurrentUser,true);

	}
	else if(m_pITableFrame->GetGameStatus()==GS_MJ_BEFOREPLAY)//��Ϸ״̬ǰ,������ҽ��в���
	{

		//��������
		CMD_S_OutCard OutCard;
		OutCard.wOutCardUser=pRepairFlower->wChairID;
		OutCard.cbOutCardData=pRepairFlower->cbCurrentCard;

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("��Ϸ״̬ǰ,������ҽ��в��� CMD_S_OutCard �����û�wOutCardUser=[%d],�����齫cbOutCardData=[%x]"),pRepairFlower->wChairID,pRepairFlower->cbCurrentCard);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

        // by hxh 20170310
        bool bRet=OnRepairFlowerStage(m_wCurrentUser,true);
	}

	///////////////
	return true;
}

bool CTableFrameSink::OnRepairFlowerStage(WORD wChairID,bool bNeed)
{
	//������ʼ	
	bool bNeedRepairFlower = false;	

	//�������
	//WORD wChairID=m_wCurrentUser;	

	//��������
	//BYTE cbFlowerCard[8]={0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};

	//��Ҫ������Ҹ���
	BYTE bNeedCount=GAME_PLAYER;

    if(bNeed)
	{
		//��һ�������
		do
		{
			wChairID = (wChairID+GAME_PLAYER-1)%GAME_PLAYER;								
			if(m_GameLogic.IsHaveFlowerCard(m_cbCardIndex[wChairID],m_cbFlowerCard,8))
				bNeedRepairFlower = true;
			else 
				bNeedCount--;

			if(bNeedCount==0)break;	

		}while(bNeedRepairFlower==false);
	}
	else
	{
		//�жϻ���
		if(!m_GameLogic.IsHaveFlowerCard(m_cbCardIndex[wChairID],m_cbFlowerCard,8))	
		{
			//��һ�������
			do
			{
				wChairID = (wChairID+GAME_PLAYER-1)%GAME_PLAYER;
				if(wChairID==m_wBankerUser)break;
				if(m_GameLogic.IsHaveFlowerCard(m_cbCardIndex[wChairID],m_cbFlowerCard,8))bNeedRepairFlower = true;
			}while(bNeedRepairFlower==false);

		}
		else
		{
			bNeedRepairFlower = true;
		}
	}

	//�Ƿ񲹻�
	if(bNeedRepairFlower==false)//����Ҫ����
	{
		//���ñ���
		m_wCurrentUser = m_wBankerUser;
		m_bGameStart = true;

		//����״̬
		m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

        //OnUserOpenGold();//by hxh 20170310


		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
		if(bNeed)
		{
			m_wProvideUser=m_wBankerUser;
		}
		m_cbProvideCard=m_cbDoorCard;

		//�����ж�		
		tagGangCardResult GangCardResult;
		m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
			m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);		

		//�����ж�		
		tagChiHuResult ChiHuResult;	
		//CopyMemory(m_cbCurCardIndex[m_wCurrentUser],m_cbCardIndex[m_wCurrentUser],sizeof(m_cbCardIndex[m_wCurrentUser]));

		//m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wCurrentUser],
		//m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],0,WIK_NULL,ChiHuResult,m_cbGoldCard,4,bGoldCardEye);
		m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[m_wCurrentUser],
			m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],0,WIK_NULL,ChiHuResult,m_cbGoldCard,4,m_cbCurCardIndex[m_wCurrentUser]);

		//��������
		CMD_S_SendCard SendCard;
		SendCard.wCurrentUser=m_wCurrentUser;
		SendCard.bRepairFlower = false;
		SendCard.cbActionMask=m_cbUserAction[m_wCurrentUser];
		SendCard.cbCardData=0;			
		SendCard.bGameSatusPlay = true;

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("�������� CMD_S_SendCard ��ǰ�û�wCurrentUser=[%d],��������cbActionMask=[%d]"),SendCard.wCurrentUser,SendCard.cbActionMask);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_END_FLOWER,&SendCard,sizeof(SendCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_END_FLOWER,&SendCard,sizeof(SendCard));


	}
	else//��Ҫ����
	{
		//���ñ���
		m_wCurrentUser = wChairID;
		m_bSendStatus = true;

		//�����˿�
		DispatchCardData(m_wCurrentUser,true);		
	}
    return bNeedRepairFlower;
}

// ����ͷ��һ�Ų��ǻ��ƵĲ�����
BYTE CTableFrameSink::FindMagicCard(WORD *pOut)
{
	for (int i=m_cbLeftCardCount-1;i>=0;i--)
	{
		BYTE by=m_cbRepertoryCard[i];
		if ( by< SPRING_DATA)
		{
			if(pOut)
			{
				*pOut=i+1;
			}
			return by;
		}
	}
    return 0;
}

// ����ͷȥ��һ�������ƣ�ʣ���齫��Ŀ��1
bool CTableFrameSink::RemoveOneMagicCard(BYTE cbMagicCard)
{
#define  swapCard(a,b)\
	BYTE nTemp = a;\
	a = b;\
	b = nTemp;\

	bool bValue = false;
	for (int i=m_cbLeftCardCount-1;i>=0;i--)
	{
		if (m_cbRepertoryCard[i] == cbMagicCard)
		{
			bValue = true;
			swapCard(m_cbRepertoryCard[i],m_cbRepertoryCard[m_cbLeftCardCount-1]);
			m_cbLeftCardCount--;
			break;
		}
	}
	return bValue;
#undef swapCard
}

//����״̬
enGoldSwim CTableFrameSink::GetCurrentGoldSwim()
{
	
	//�м����
	bool bGoldSwim[4];
	for(BYTE i=0;i<4;i++)bGoldSwim[i]=false;

	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_UserGoldSwim[i]==GoldSwim_Three)bGoldSwim[3]=true;//����
		else if(m_UserGoldSwim[i]==GoldSwim_Double)bGoldSwim[2]=true;//˫��
		else if(m_UserGoldSwim[i]==GoldSwim_Single)bGoldSwim[1]=true;//����
		
	}

	if(bGoldSwim[3]==true)return GoldSwim_Three;
	else if(bGoldSwim[2]==true)return GoldSwim_Double;
	else if(bGoldSwim[1]==true)return GoldSwim_Single;
	else return GoldSwim_None;
}

//����״̬
enGoldSwim CTableFrameSink::GetUserGoldSwim(WORD wChairID)
{
	//�м����
	BYTE cbGoldCount=0;//��������	
	
	cbGoldCount = m_GameLogic.GetGoldCardCount(m_cbCardIndex[wChairID],m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID],m_cbGoldCard,4);

	if(cbGoldCount==0)return GoldSwim_None;
	else if(cbGoldCount==1)return GoldSwim_Single;
	else if(cbGoldCount==2)return GoldSwim_Double;
	else return GoldSwim_Three;

}

////////////////
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0;
}
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0;
}
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	return false;
}
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}
void CTableFrameSink::SetGameBaseScore(LONG lBaseScore)
{
	return;
}
bool CTableFrameSink::OnActionUserOnReady(WORD,IServerUserItem *,void *,WORD)
{
	return false;
}
//////////////////////////////////////////////////////////////////////////

