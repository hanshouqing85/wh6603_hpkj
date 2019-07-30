#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����

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
    m_bGameStart = false;
	m_lSiceCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
	m_cbQuanFeng = 0;
	m_cbQuanCounter = 1;
	ZeroMemory( m_cbHuaCardCount,sizeof(m_cbHuaCardCount) );
	m_wReplaceUser = INVALID_CHAIR;
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 2;

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory( m_dwChiHuKind,sizeof(m_dwChiHuKind) );

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
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
    m_bGameStart = false;
	m_lSiceCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
	ZeroMemory( m_cbHuaCardCount,sizeof(m_cbHuaCardCount) );
	m_wReplaceUser = INVALID_CHAIR;
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 2;

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(m_dwChiHuKind,sizeof(m_dwChiHuKind));

	return;
}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//�����˿�
	if( m_wBankerUser == INVALID_CHAIR )
		m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),MAKEWORD(rand()%6+1,rand()%6+1));
	else
		m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),0);

	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

#ifdef USE_FIXCARD
	//test
 BYTE byTest[] = {
  0x45,0x46,0x47,0x48,//����
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,//ͬ��      
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,      
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,      
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,      
  0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,//����      
  0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,      
  0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,      
  0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,      
  //���Ӻͷ���
  0x32,0x26,0x29,0x37,0x28,0x34,0x31,0x28,
  0x34,0x33,0x42,0x22,
  0x29,//���Ҳ�����
  0x28,//����
  0x33,//ׯ�Ҳ�����
  0x27,//ׯ�Ҳ�����
  0x21,0x25,0x23,0x26,0x27,0x27,0x28,0x32,0x35,0x36,0x37,0x36,0x21,//3
  0x21,0x22,0x24,0x27,0x44,0x33,0x36,0x36,0x37,0x37,0x29,0x31,0x25,//2
  0x22,0x23,0x23,0x24,0x24,0x25,0x26,0x29,0x31,0x32,0x32,0x33,0x31,//1
  0x21,0x22,0x23,0x24,0x25,0x26,0x41,0x43,0x34,0x34,0x35,0x35,0x35,//0
 };
	CopyMemory( m_cbRepertoryCard,byTest,sizeof(byTest) );
	m_wBankerUser = 0;
	//end test
#endif

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	if( m_wBankerUser == INVALID_CHAIR )
	{
		WORD wSice = (WORD)(m_lSiceCount>>16);
		m_wBankerUser = (HIBYTE(wSice)+LOBYTE(wSice))%GAME_PLAYER;
	}

	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_cbProvideCard=m_cbSendCardData;
	m_wProvideUser=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//������Ϣ
	WORD wSice = WORD(m_lSiceCount&0xffff);
	BYTE cbSiceTakeCount= HIBYTE(wSice)+LOBYTE(wSice);
	WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
	while( cbSiceTakeCount*2 > HEAP_FULL_COUNT )
	{
		wTakeChairID = (wTakeChairID+1)%GAME_PLAYER;
		cbSiceTakeCount -= HEAP_FULL_COUNT/2;
	}
	m_wHeapTail = wTakeChairID;
	BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ŀ
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceTakeCount)*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//��ȡ�˿�
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//����ж�
		if (cbTakeCount==0)
		{
			m_wHeapHead=wTakeChairID;
			break;
		}
		//�л�����
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//��������
	bool bAroseAction=false;
    int cardindex=CheckReplaceUser();

	//ׯ���ж�
	if ( m_wReplaceUser==INVALID_CHAIR )
	{
		//�����ж�
		tagGangCardResult GangCardResult;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wBankerUser],NULL,0,GangCardResult);

		//�����ж�
		CChiHuRight chr;
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,m_cbSendCardData,chr);
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
	}

	//��������
	CMD_S_GameStart GameStart;
	GameStart.lSiceCount=m_lSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wReplaceUser = m_wReplaceUser;
	GameStart.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
	GameStart.cbQuanFeng = m_cbQuanFeng;
	GameStart.wHeapHead = m_wHeapHead;
	GameStart.wHeapTail = m_wHeapTail;
	CopyMemory(GameStart.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));
	GameStart.cbHuaCardData=(cardindex==-1?0:m_GameLogic.SwitchToCardData(cardindex));

#ifdef USE_RS_PRINT
	SYSTEMTIME timeCreateFile={0};	// ������ʱ��
	GetSystemTime(&timeCreateFile);	// ��õ�ǰʱ��

	wstring  strLogFile=L"";
	strLogFile=s2ws(GetAppPath())+L"\\MJServer\\NetMsg.txt";
	wstring wstrDirName=s2ws(GetAppPath())+L"\\MJServer";
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
	strLog.Format(_T("CMD_S_GameStart ���ӵ���lSiceCount=[%d],ׯ���û�wBankerUser=[%d],��ǰ�û�wCurrentUser=[%d],�����û�wReplaceUser=[%d],Ȧ��cbQuanFeng=[%d],������ͷwHeapHead=[%d],������βwHeapTail=[%d],�����齫cbHuaCardData=[%x]"),GameStart.lSiceCount,GameStart.wBankerUser,GameStart.wCurrentUser,GameStart.wReplaceUser,GameStart.cbQuanFeng,GameStart.wHeapHead,GameStart.wHeapTail,GameStart.cbHuaCardData);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
	outf<<CT2A(strLog)<<endl;
#endif

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		GameStart.cbUserAction=m_cbUserAction[i];

		ZeroMemory(GameStart.cbCardData,sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		if( m_pITableFrame->GetTableUserItem(i)->IsAndroidUser() )
		{
			BYTE bIndex = 1;
			for( WORD j=0; j<GAME_PLAYER; j++ )
			{
				if( j == i ) continue;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[j],&GameStart.cbCardData[MAX_COUNT*bIndex++]);
			}
		}
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
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			WORD wWinner = INVALID_CHAIR;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.dwChiHuKind[i]=m_dwChiHuKind[i];
				if( m_dwChiHuKind[i] == WIK_CHI_HU )
					wWinner = i;
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//�����������
			if( wWinner != INVALID_CHAIR )
			{
				//Ȩλ����
				FiltrateRight( wWinner,m_ChiHuRight[wWinner] );
			}

			//ͳ�ƻ���
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//������Ϣ
				GameEnd.wProvideUser=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;

				m_ChiHuRight[wWinner].GetRightData( GameEnd.dwChiHuRight,MAX_RIGHT_COUNT );

				GameEnd.cbFanCount = CalScore( wWinner,GameEnd.lGameScore );
				GameEnd.cbHuaCardCount = m_cbHuaCardCount[wWinner];
			}
			else
			{
				//���ֽ���
				GameEnd.wProvideUser=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;
			}
			
			//ͳ�ƻ���
			LONG lGameTax[GAME_PLAYER];
			ZeroMemory(lGameTax,sizeof(lGameTax));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�ۼӻ���
				GameEnd.lGameScore[i]*=m_pGameServiceOption->lCellScore;
				//���û���
				if( GAME_GENRE_SCORE != m_pGameServiceOption->wServerType )
				{
					if (GameEnd.lGameScore[i]>0L)
					{
						///lGameTax[i] = (LONG)(GameEnd.lGameScore[i]*m_pGameServiceOption->cbRevenueRatio/1000L);
						GameEnd.lGameScore[i] -= lGameTax[i];
					}
				}
			}

			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE ScoreKind;
				if( GameEnd.lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;
				//д�����
				////m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lGameTax[i],ScoreKind);

				//������Ϣ
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_GameEnd������� ��Ӧ�û�wProvideUser=[%d],��Ӧ�齫cbProvideCard=[%x],��������dwChiHuKind[0,1,2,3]=[%d,%d,%d,%d],��������dwChiHuRight[0,1,2]=[%d,%d,%d],���Ƹ���cbHuaCardCount=[%d],�ܷ���cbFanCount=[%d],��Ϸ����lGameScore[0,1,2,3]=[%lld,%lld,%lld,%lld]"),GameEnd.wProvideUser,GameEnd.cbProvideCard,GameEnd.dwChiHuKind[0],GameEnd.dwChiHuKind[1],GameEnd.dwChiHuKind[2],GameEnd.dwChiHuKind[3],GameEnd.dwChiHuRight[0],GameEnd.dwChiHuRight[1],GameEnd.dwChiHuRight[2],GameEnd.cbHuaCardCount,GameEnd.cbFanCount,GameEnd.lGameScore[0],GameEnd.lGameScore[1],GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
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
////////////////////////			
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE ScoreKind;
				if( GameEnd.lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;

				ScoreInfoArray[i].lScore   = GameEnd.lGameScore[i];
				ScoreInfoArray[i].lRevenue = lGameTax[i];
				ScoreInfoArray[i].cbType   = ScoreKind;
			}

			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
///////////////////////

			//���ñ���
			m_wBankerUser = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;
			if( ++m_cbQuanCounter == 5 )
			{
				m_cbQuanCounter = 1;
				if( ++m_cbQuanFeng == 4 )
				{
					m_cbQuanFeng = 0;
				}
			}

			//������Ϸ
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE/*,NULL,608*/);
#endif
			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_GameEnd��Ϸ��ɢ ��Ӧ�û�wProvideUser=[%d],��Ӧ�齫cbProvideCard=[%x],��������dwChiHuKind[0,1,2,3]=[%d,%d,%d,%d],��������dwChiHuRight[0,1,2]=[%d,%d,%d],���Ƹ���cbHuaCardCount=[%d],�ܷ���cbFanCount=[%d],��Ϸ����lGameScore[0,1,2,3]=[%lld,%lld,%lld,%lld]"),GameEnd.wProvideUser,GameEnd.cbProvideCard,GameEnd.dwChiHuKind[0],GameEnd.dwChiHuKind[1],GameEnd.dwChiHuKind[2],GameEnd.dwChiHuKind[3],GameEnd.dwChiHuRight[0],GameEnd.dwChiHuRight[1],GameEnd.dwChiHuRight[2],GameEnd.cbHuaCardCount,GameEnd.cbFanCount,GameEnd.lGameScore[0],GameEnd.lGameScore[1],GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			CString strLog;
			strLog.Format(_T("CMD_S_GameEnd��Ϸ��ɢ i=[%d]��,�齫��ĿcbCardCount=[%d],[%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x][%x]"),i,GameEnd.cbCardCount[i],GameEnd.cbCardData[i][0],GameEnd.cbCardData[i][1],GameEnd.cbCardData[i][2],GameEnd.cbCardData[i][3],GameEnd.cbCardData[i][4],GameEnd.cbCardData[i][5],GameEnd.cbCardData[i][6],GameEnd.cbCardData[i][7],GameEnd.cbCardData[i][1+7],GameEnd.cbCardData[i][2+7],GameEnd.cbCardData[i][3+7],GameEnd.cbCardData[i][4+7],GameEnd.cbCardData[i][5+7],GameEnd.cbCardData[i][6+7]);
			CTraceService::TraceString(strLog,TraceLevel_Normal);	
			//ofstream outf(g_wcTmp,ios::app); 
			outf<<CT2A(strLog)<<endl;
		}
#endif

			//������Ϸ
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE/*,NULL,608*/);
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

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//����
			GameEnd.lGameScore[wChairID] = -20L*m_pGameServiceOption->lCellScore;
			
			//֪ͨ��Ϣ
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetNickName());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
				if (pISendUserItem!=NULL) 
					m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_PROMPT);
			}

			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL)
					break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_PROMPT);
			} while (true);

///////////////////////
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			//д�����
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				ScoreInfoArray[i].lScore   = GameEnd.lGameScore[i];
				ScoreInfoArray[i].lRevenue = (i!=wChairID)?GameEnd.lGameTax:0;
				ScoreInfoArray[i].cbType   = (i==wChairID)?SCORE_TYPE_FLEE:SCORE_TYPE_WIN;
			}

			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
//////////////////////

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//д�����
			////m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0,SCORE_TYPE_FLEE);

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_GameEnd[%d] ��Ӧ�û�wProvideUser=[%d],��Ӧ�齫cbProvideCard=[%x],��������dwChiHuKind[0,1,2,3]=[%d,%d,%d,%d],��������dwChiHuRight[0,1,2]=[%d,%d,%d],���Ƹ���cbHuaCardCount=[%d],�ܷ���cbFanCount=[%d],��Ϸ����lGameScore[0,1,2,3]=[%lld,%lld,%lld,%lld]"),cbReason,GameEnd.wProvideUser,GameEnd.cbProvideCard,GameEnd.dwChiHuKind[0],GameEnd.dwChiHuKind[1],GameEnd.dwChiHuKind[2],GameEnd.dwChiHuKind[3],GameEnd.dwChiHuRight[0],GameEnd.dwChiHuRight[1],GameEnd.dwChiHuRight[2],GameEnd.cbHuaCardCount,GameEnd.cbFanCount,GameEnd.lGameScore[0],GameEnd.lGameScore[1],GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
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

			//������Ϸ
#ifdef USE_GB
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE/*,NULL,608*/);
#endif

			return true;
		}
	}

	//�������
	ASSERT(FALSE);
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
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_StatusFree ׯ���û�wBankerUser=[%d],�������lCellScore=[%lld],�Ƿ��й�bTrustee[0,1,2,3]=[%d,%d,%d,%d]"),StatusFree.wBankerUser,StatusFree.lCellScore,StatusFree.bTrustee[0],StatusFree.bTrustee[1],StatusFree.bTrustee[2],StatusFree.bTrustee[3]);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.wReplaceUser = m_wReplaceUser;
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));
			StatusPlay.cbQuanFeng = m_cbQuanFeng;

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//����״̬
			CopyMemory( StatusPlay.cbHearStatus,m_cbListenStatus,sizeof(m_cbListenStatus) );

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//������Ϣ
			StatusPlay.wHeapHead = m_wHeapHead;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

			//�˿�����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);
			if( m_wReplaceUser == wChiarID ) StatusPlay.cbSendCardData = m_cbSendCardData;
			else StatusPlay.cbSendCardData=(m_wProvideUser==wChiarID)?m_cbProvideCard:0x00;

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_StatusPlay ׯ���û�wBankerUser=[%d],��Ԫ����lCellScore=[%lld],�Ƿ��й�bTrustee[0,1,2,3]=[%d,%d,%d,%d],��ǰ�û�wCurrentUser=[%d]"),StatusPlay.wBankerUser,StatusPlay.lCellScore,StatusPlay.bTrustee[0],StatusPlay.bTrustee[1],StatusPlay.bTrustee[2],StatusPlay.bTrustee[3],StatusPlay.wReplaceUser);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
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
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			////tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_C_OutCard �齫����cbCardData=[%x]"),pOutCard->cbCardData);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//�û�Ч��
			////tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_C_OperateCard ��������cbOperateCode=[%d],�����齫cbOperateCard=[%x,%x,%x]"),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard[0],pOperateCard->cbOperateCard[1],pOperateCard->cbOperateCard[2]);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif
			return OnUserOperateCard(pIServerUserItem->GetChairID(),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_REPLACE_CARD:	//�û�����
		{
			//return true;
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ReplaceCard));
			if (wDataSize!=sizeof(CMD_C_ReplaceCard)) return false;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��������
			CMD_C_ReplaceCard * pReplaceCard=(CMD_C_ReplaceCard *)pDataBuffer;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_C_ReplaceCard wChairID=[%d],�齫����cbCardData=[%x]"),wChairID,pReplaceCard->cbCardData);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif
			return OnUserReplaceCard(pIServerUserItem->GetChairID(),pReplaceCard->cbCardData);
		}
	case SUB_C_TRUSTEE:
		{
			return true;
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			//�û�Ч��
			////tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			m_bTrustee[pIServerUserItem->GetChairID()]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pIServerUserItem->GetChairID();
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_C_Trustee �й��û�wChairID=[%d],�Ƿ��й�bTrustee=[%d]"),Trustee.wChairID,Trustee.bTrustee);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif
			return true;
		}
	case SUB_C_LISTEN:
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_Listen));
			if (wDataSize!=sizeof(CMD_C_Listen)) return false;

			//�û�Ч��
			////tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			CMD_C_Listen *pListen = (CMD_C_Listen *)pDataBuffer;
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_C_Listen wChairID=[%d],�����û�cbListen=[%d]"),pIServerUserItem->GetChairID(),pListen->cbListen);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif
			return OnUserListenCard(pIServerUserItem->GetChairID(),pListen->cbListen);
		}
	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if (bLookonUser==false)
	{
		m_wBankerUser = INVALID_CHAIR;

		m_cbQuanCounter = 1;

		m_bTrustee[wChairID]=false;

		CMD_S_Trustee Trustee;
		Trustee.bTrustee=false;
		Trustee.wChairID = wChairID;

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_Trustee �й��û�wChairId=[%d],�Ƿ��й�bTrustee=[%d]"),Trustee.wChairID,Trustee.bTrustee);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserListenCard(WORD wChairID,BYTE cbListen)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//Ч�����
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbListenStatus[wChairID]==0));
	if ((wChairID!=m_wCurrentUser)||(m_cbListenStatus[wChairID]!=0)) return false;

	//���ñ���
	m_cbListenStatus[wChairID] = cbListen;
	m_bEnjoinChiPeng[wChairID] = true;

	//��������
	CMD_S_Listen ListenCard;
	ListenCard.wChairId=wChairID;

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_Listen �����û�wChairId=[%d]"),ListenCard.wChairId);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		//outf<<ws2s(strLog.GetString()).c_str()<<endl;
        outf<<CT2A(strLog)<<endl;
#endif

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));

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

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bSendStatus=true;
	m_bGangStatus = false;
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
		strLog.Format(_T("CMD_S_OutCard �����û�wOutCardUser=[%d],�����齫cbOutCardData=[%x]"),OutCard.wOutCardUser,OutCard.cbOutCardData);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;

	//�û��л�
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//�ɷ��˿�
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3])
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY)
		return true;

	//Ч���û� ע�⣺���л�����ʱ�п��ܻᷢ���˶���
	//ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return true;

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		//ASSERT(m_bResponse[wChairID]==false);
		//ASSERT( m_cbUserAction[wChairID] != WIK_NULL );
		//ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//Ч��״̬
		if (m_bResponse[wChairID]==true) return true;
		if( m_cbUserAction[wChairID] == WIK_NULL ) return true;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return true;

		//��������
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//���ñ���
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		if(cbOperateCard[0]==0)
			m_cbOperateCard[wChairID][0] = m_cbProvideCard;
		else CopyMemory( m_cbOperateCard[wChairID],cbOperateCard,sizeof(m_cbOperateCard[wChairID]) );

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
			return true;

		//�Ժ��ȴ�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
					return true;
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

			//�����˿�
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser][0];

		//���Ʊ���
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//���Ʋ���
		if (cbTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;

			for (WORD i=(m_wProvideUser+m_wPlayerCount-1)%GAME_PLAYER;i!=m_wProvideUser;i = (i+m_wPlayerCount-1)%GAME_PLAYER)
			{
				//�����ж�
				if ((m_cbPerformAction[i]&WIK_CHI_HU)==0)
					continue;

				//�����ж�
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_dwChiHuKind[i] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i]);

				//�����˿�
				if (m_dwChiHuKind[i]!=WIK_NULL) 
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					wTargetUser = i;
					break;
				}
			}

			//������Ϸ
			ASSERT(m_dwChiHuKind[wTargetUser]!=WIK_NULL);
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//����˿�
		ASSERT(m_cbWeaveItemCount[wTargetUser]<MAX_WEAVE);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		m_WeaveItemArray[wTargetUser][wIndex].cbCardData[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = m_cbOperateCard[wTargetUser][1];
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = m_cbOperateCard[wTargetUser][2];
		}
		else
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = cbTargetCard;
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = cbTargetCard;
			if( cbTargetAction & WIK_GANG )
				m_WeaveItemArray[wTargetUser][wIndex].cbCardData[3] = cbTargetCard;
		}

		//ɾ���˿�
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//���Ʋ���
			{
				//ɾ���˿�
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//ɾ���˿�
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//ɾ���˿�
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_GANG:		//���Ʋ���
			{
				//ɾ���˿�,��������ֻ���ڷŸ�
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard)) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		default:
			ASSERT( FALSE );
			return false;
		}

		//������
		CMD_S_OperateResult OperateResult;
		ZeroMemory( &OperateResult,sizeof(OperateResult) );
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		OperateResult.cbOperateCard[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			CopyMemory( &OperateResult.cbOperateCard[1],&m_cbOperateCard[wTargetUser][1],2*sizeof(BYTE) );
		else if( cbTargetAction&WIK_PENG )
		{
			OperateResult.cbOperateCard[1] = cbTargetCard;
			OperateResult.cbOperateCard[2] = cbTargetCard;
		}

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_OperateResult �����û�wOperateUser=[%d],��Ӧ�û�wProvideUser=[%d],��������cbOperateCode=[%d],�����齫cbOperateCard[0]=[%x]"),OperateResult.wOperateUser,OperateResult.wProvideUser,OperateResult.cbOperateCode,OperateResult.cbOperateCard[0]);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//�����û�
		m_wCurrentUser=wTargetUser;

		//���ƴ���
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus = true;
			DispatchCardData(wTargetUser,true);
		}

		return true;
	}

	//��������
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return false;

		//�˿�Ч��
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard[0])==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard[0])==false)) 
			return false;

		//���ñ���
		m_bSendStatus=true;
		m_cbUserAction[m_wCurrentUser]=WIK_NULL;
		m_cbPerformAction[m_wCurrentUser]=WIK_NULL;

		bool bPublic=false;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_GANG:			//���Ʋ���
			{
				//��������
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard[0]);

				//���ƴ���
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//Ѱ�����
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
						{
							bPublic=true;
							cbWeaveIndex=i;
							break;
						}
					}

					//Ч�鶯��
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//����˿�
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[3]=cbOperateCard[0];
				}
				else
				{
					//�˿�Ч��
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
						return false;

					//���ñ���
					bPublic=false;
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					for( BYTE j = 0; j < 4; j++ ) m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[j] = cbOperateCard[0];
				}

				//ɾ���˿�
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				m_bGangStatus = true;

				//������
				CMD_S_OperateResult OperateResult;
				ZeroMemory( &OperateResult,sizeof(OperateResult) );
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard[0]=cbOperateCard[0];

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_OperateResult���Ʋ��� �����û�wOperateUser=[%d],��Ӧ�û�wProvideUser=[%d],��������cbOperateCode=[%d],�����齫cbOperateCard[0]=[%x]"),OperateResult.wOperateUser,OperateResult.wProvideUser,OperateResult.cbOperateCode,OperateResult.cbOperateCard[0]);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//Ч�鶯��
				bool bAroseAction=false;
				if (bPublic==true) bAroseAction=EstimateUserRespond(wChairID,cbOperateCard[0],EstimatKind_GangCard);

				//�����˿�
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID,true);
				}

				return true;
			}
		case WIK_CHI_HU:		//�Ժ�����
			{
				//��ͨ����
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],&m_cbProvideCard,1) )
				{
					ASSERT( FALSE );
					return false;
				}
				m_dwChiHuKind[wChairID] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,
					m_ChiHuRight[wChairID]);
				m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;

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

//�û�����
bool CTableFrameSink::OnUserReplaceCard(WORD wChairID, BYTE cbCardData)
{
	//�������
	ASSERT(wChairID==m_wReplaceUser);
	ASSERT(m_GameLogic.IsHuaCard(cbCardData)==true);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
	if (wChairID!=m_wReplaceUser) return false;
	if (m_GameLogic.IsHuaCard(cbCardData)==false) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false)  return false;

#if 1//def USE_RS_PRINT
		CString strLog1;
		strLog1.Format(_T("OnUserReplaceCard step1:wChairID=[%d]=m_wReplaceUser=[%d],m_wCurrentUser=[%d],m_wOutCardUser=[%d]"),wChairID,m_wReplaceUser,m_wCurrentUser,m_wOutCardUser);
		CTraceService::TraceString(strLog1,TraceLevel_Normal);	
		ofstream outf1(g_wcTmp,ios::app); 
		outf1<<CT2A(strLog1)<<endl;
#endif

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//״̬����
	m_bSendStatus=true;
	m_cbSendCardData=0x00;

	//��������ͳ��
	m_cbHuaCardCount[wChairID]++;

	//�����˿�
	m_cbDiscardCount[wChairID]++;
	m_cbDiscardCard[wChairID][m_cbDiscardCount[wChairID]-1]=cbCardData;

	//��������
	CMD_S_ReplaceCard ReplaceCard;
	ReplaceCard.wReplaceUser=wChairID;
	ReplaceCard.cbReplaceCard=cbCardData;

#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(_T("CMD_S_ReplaceCard wChairID=[%d]��,�����û�wReplaceUser=[%d],�����齫cbReplaceCard=[%x]"),wChairID,ReplaceCard.wReplaceUser,ReplaceCard.cbReplaceCard);
	CTraceService::TraceString(strLog,TraceLevel_Normal);	
	ofstream outf(g_wcTmp,ios::app); 
	outf<<CT2A(strLog)<<endl;
#endif

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

	//�ɷ��˿�
	DispatchCardData(wChairID);

	return true;
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

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bTail)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;
	ASSERT( m_bSendStatus );
	if( m_bSendStatus == false ) return false;

#if 1//def USE_RS_PRINT
		CString strLog1;
		strLog1.Format(_T("DispatchCardData step1:wCurrentUser=[%d],bTail=[%d],m_wCurrentUser=[%d],m_wReplaceUser=[%d],m_wOutCardUser=[%d]"),wCurrentUser,bTail,m_wCurrentUser,m_wReplaceUser,m_wOutCardUser);
		CTraceService::TraceString(strLog1,TraceLevel_Normal);	
		ofstream outf1(g_wcTmp,ios::app); 
		outf1<<CT2A(strLog1)<<endl;
#endif

	//�����˿�
	if ((m_wReplaceUser==INVALID_CHAIR)&&(m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//��ׯ����
	if (m_cbLeftCardCount==m_cbEndLeftCount)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//���ñ���
	if( m_wReplaceUser == INVALID_CHAIR )
	{
		m_cbOutCardData=0;
		m_wCurrentUser=wCurrentUser;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	//����
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//�����ж�
    int cardindex=-1;
	if ((m_wReplaceUser!=INVALID_CHAIR)||(m_GameLogic.IsHuaCard(m_cbSendCardData)==true))
	{
		m_wReplaceUser=INVALID_CHAIR;
        cardindex=CheckReplaceUser();
	}

	if( m_wCurrentUser == wCurrentUser )
	{
		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;
	}

	if( m_wReplaceUser == INVALID_CHAIR )
	{
		if( !m_bEnjoinChiHu[m_wCurrentUser] )
		{
			//�����ж�
			CChiHuRight chr;
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
			m_cbUserAction[m_wCurrentUser] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wCurrentUser],m_WeaveItemArray[m_wCurrentUser],
				m_cbWeaveItemCount[m_wCurrentUser],m_cbProvideCard,chr);
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
		}

		//�����ж�
		if ((m_bEnjoinChiPeng[m_wCurrentUser]==false)&&(m_cbLeftCardCount>m_cbEndLeftCount))
		{
			tagGangCardResult GangCardResult;
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);
		}
	}

	//������Ϣ
	ASSERT( m_wHeapHead != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR );
	if( !bTail )
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHead][0]+m_cbHeapCardInfo[m_wHeapHead][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHead=(m_wHeapHead+1)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapHead][0]++;
	}
	else
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wSendCardUser = wCurrentUser;
	SendCard.wReplaceUser = m_wReplaceUser;
	SendCard.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
	SendCard.bTail = bTail;
	SendCard.cbActionMask=m_wReplaceUser==INVALID_CHAIR?m_cbUserAction[m_wCurrentUser]:WIK_NULL;
	SendCard.cbCardData=(m_wCurrentUser==wCurrentUser)?m_cbProvideCard:m_cbSendCardData;
	SendCard.cbHuaCardData=(cardindex==-1?0:m_GameLogic.SwitchToCardData(cardindex));

#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(_T("CMD_S_SendCard ��ǰ�û�wCurrentUser=[%d],�����û�wSendCardUser=[%d],ĩβ����bTail=[%d],�����û�wReplaceUser=[%d],��������cbActionMask=[%d],�齫����cbCardData=[%x],�����齫cbHuaCardData=[%x]"),SendCard.wCurrentUser,SendCard.wSendCardUser,bTail,SendCard.wReplaceUser,SendCard.cbActionMask,SendCard.cbCardData,SendCard.cbHuaCardData);
		CTraceService::TraceString(strLog,TraceLevel_Normal);	
		ofstream outf(g_wcTmp,ios::app); 
		outf<<CT2A(strLog)<<endl;
#endif

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
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i)
					m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				if (m_cbLeftCardCount>m_cbEndLeftCount) 
				{
					m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				}
			}
		}

		//�����ж�
#ifdef USE_DIAN_PAO_HU
		if (m_bEnjoinChiHu[i]==false )
		{
			//�Ժ��ж�
			CChiHuRight chr;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr);
			m_cbUserAction[i] |= cbAction;
		}
#endif
		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
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

//���
BYTE CTableFrameSink::CalScore( WORD wWinner, LONGLONG lScore[GAME_PLAYER] )
{
	//��ʼ��
	ZeroMemory( lScore,sizeof(LONGLONG)*GAME_PLAYER );

	//���Ʒ���
	WORD wFanShuo = m_GameLogic.GetChiHuActionRank( m_ChiHuRight[wWinner] );
	//���Ʒ���
	wFanShuo += m_cbHuaCardCount[wWinner];
	//���Ƹ���
	ASSERT( wFanShuo > 0 );
	if( wFanShuo == 0 ) return 0;

	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( i != wWinner )
		{
			//�����������Ҳ��۷�
			if( wWinner != m_wProvideUser && i != m_wProvideUser ) continue;

			LONGLONG lGameScore = wFanShuo;
			lScore[i] = -lGameScore;
			lScore[wWinner] += lGameScore;
		}
	}

	return BYTE(wFanShuo);
}

//��ϵͳ���ƻ�ȡ��������
BYTE CTableFrameSink::GetMagicIndex( BYTE cbFanCard )
{
	BYTE cbZPaiIndex = m_GameLogic.SwitchToCardIndex(cbFanCard);
	if( cbZPaiIndex < 27 )
	{
		if( (cbZPaiIndex+1)%9 == 0 )
			cbZPaiIndex -= 8;
		else
			cbZPaiIndex++;
	}
	else
	{
		if( cbZPaiIndex + 1 == MAX_INDEX )
			cbZPaiIndex = 27;
		else
			cbZPaiIndex++;
	}
	return cbZPaiIndex;
}

//��ȡ��������
BYTE CTableFrameSink::GetDiscardCount( BYTE cbCardData )
{
	BYTE cbCount = 0;
	//����������
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		for( BYTE j = 0; j < m_cbDiscardCount[i]; j++ )
		{
			if( m_cbDiscardCard[i][j] == cbCardData )
				if( ++cbCount == 4 ) break;
		}
	}
	if( cbCount < 4 )
	{
		//������
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			for( BYTE j = 0; j < m_cbWeaveItemCount[i]; j++ )
			{
				if( !m_WeaveItemArray[i][j].cbPublicCard ) continue;
				BYTE cbCardBuffer[4];
				BYTE cbCardCount = m_GameLogic.GetWeaveCard( m_WeaveItemArray[i][j].cbWeaveKind,
					m_WeaveItemArray[i][j].cbCenterCard,cbCardBuffer );
				for( BYTE k = 0; k < cbCardCount; k++ )
					if( cbCardBuffer[k] == cbCardData )
						if( ++cbCount == 4 ) break;
			}
		}
	}

	return cbCount;
}

//Ȩλ����
void CTableFrameSink::FiltrateRight( WORD wWinner, CChiHuRight &chr )
{
	/*	����Ȩλ	*/

	//����
	if( wWinner == m_wProvideUser )
		chr |= CHR_ZI_MO;
	//Ȧ���,�ŷ��
	for( BYTE i = 0; i < m_cbWeaveItemCount[wWinner]; i++ )
	{
		if( m_WeaveItemArray[wWinner][i].cbWeaveKind == WIK_PENG &&
			m_WeaveItemArray[wWinner][i].cbCenterCard >= 0x31 )
		{
			BYTE cbCenterCard = m_WeaveItemArray[wWinner][i].cbCenterCard;
			//Ȧ���
			if( m_cbQuanFeng == 0 && cbCenterCard == 0x31 ||
				m_cbQuanFeng == 1 && cbCenterCard == 0x32 ||
				m_cbQuanFeng == 2 && cbCenterCard == 0x33 || 
				m_cbQuanFeng == 3 && cbCenterCard == 0x34 )
				chr |= CHR_QUAN_FENG_KE;
			//�ŷ��
			if( (m_wBankerUser+GAME_PLAYER-wWinner)%GAME_PLAYER == (cbCenterCard&MASK_VALUE)-1 )
				chr |= CHR_MEN_FENG_KE;
		}
	}
	for( BYTE i = 27; i < MAX_INDEX-MAX_HUA_CARD; i++ )
	{
		if( m_cbCardIndex[wWinner][i] == 3 )
		{
			BYTE cbCenterCard = m_GameLogic.SwitchToCardData(i);
			//Ȧ���
			if( m_cbQuanFeng == 0 && cbCenterCard == 0x31 ||
				m_cbQuanFeng == 1 && cbCenterCard == 0x32 ||
				m_cbQuanFeng == 2 && cbCenterCard == 0x33 || 
				m_cbQuanFeng == 3 && cbCenterCard == 0x34 )
				chr |= CHR_QUAN_FENG_KE;
			//�ŷ��
			if( (m_wBankerUser+GAME_PLAYER-wWinner)%GAME_PLAYER == (cbCenterCard&MASK_VALUE)-1 )
				chr |= CHR_MEN_FENG_KE;
		}
	}
	//�;���
	BYTE cbIndexProviderCard = m_GameLogic.SwitchToCardIndex(m_cbProvideCard);
	BYTE cbTableCount = 0;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		//����
		for( BYTE j = 0; j < m_cbWeaveItemCount[i]; j++ )
		{
			if( m_WeaveItemArray[i][j].cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG) )
			{
				for( BYTE k = 0; k < 3; k++ )
					if( m_cbProvideCard == m_WeaveItemArray[i][j].cbCardData[k] )
						cbTableCount++;
			}
		}
		//������
		for( BYTE j = 0; j < m_cbDiscardCount[i]; j++ )
		{
			if( m_cbDiscardCard[i][j] == m_cbProvideCard )
				cbTableCount++;
		}
	}
	ASSERT( cbTableCount < 4 );
	if( cbTableCount == 3 )
		chr |= CHR_JUE_ZHANG;

	if( m_bGangStatus )
	{
		//���ܺ�
		if( (chr&CHR_ZI_MO).IsEmpty() )
			chr |= CHR_QIANG_GANG;
		//���Ͽ���
		else chr |= CHR_GANG_KAI;
	}
	//��������
	if( m_cbLeftCardCount==m_cbEndLeftCount && m_wProvideUser != wWinner )
		chr |= CHR_HAI_DI;
	//���ֻش�
	if( m_cbLeftCardCount==m_cbEndLeftCount && m_wProvideUser == wWinner )
		chr |= CHR_MIAO_SHOU;

	/*	�޸�Ȩλ	*/

	//ȫ���� �����Ǻ�������
	if( !(chr&CHR_QUAN_QIU_REN).IsEmpty() && m_wProvideUser==wWinner )
		chr &= ~CHR_QUAN_QIU_REN;
	//������ ��������
	if( !(chr&CHR_BU_QIU_REN).IsEmpty() && m_wProvideUser!=wWinner )
		chr &= ~CHR_BU_QIU_REN;
	//��ǰ�� ����������
	if( !(chr&CHR_MEN_QI_QING).IsEmpty() && m_wProvideUser != wWinner )
		chr &= ~CHR_MEN_QI_QING;
	//�޷��� ���ƺ󣬰������ƣ��������κη��ַ�	!!!ע:�޷���һ��Ҫ���������
	if( !(chr&CHR_WU_FAN).IsEmpty() )
	{
		//ȥ���޷���
		if( m_cbHuaCardCount[wWinner] > 0 || !(chr&~CHR_WU_FAN).IsEmpty() )
		{
			chr &= ~CHR_WU_FAN;
		}
	}

	/*	����Ȩλ	*/

	//����ϲ	����Ȧ��̡��ŷ�̡�����̡�������
	if( !(chr&CHR_DA_SHI_XI).IsEmpty() )
	{
		chr &= ~CHR_QUAN_FENG_KE;
		chr &= ~CHR_MEN_FENG_KE;
		chr &= ~CHR_SAN_FENG_KE;
		chr &= ~CHR_PENG_PENG;
	}
	//����Ԫ	���Ƽ���
	if( !(chr&CHR_DA_SAN_YUAN).IsEmpty() )
		chr &= ~CHR_JIAN_KE;
	//��һɫ	���ƻ�һɫ
	if( !(chr&CHR_LU_YI_SE).IsEmpty() )
		chr &= ~CHR_HUN_YI_SE;
	//��������	������һɫ
	if( !(chr&CHR_JIU_LIAN_DENG).IsEmpty() )
		chr &= ~CHR_QING_YI_SE;
	//�ĸ�	��������
	if( !(chr&CHR_SI_GANG).IsEmpty() )
		chr &= ~CHR_SAN_GANG;
	//���߶�	������һɫ�������ˡ�����
	if( !(chr&CHR_LIAN_QI_DUI).IsEmpty() )
	{
		chr &= ~CHR_QING_YI_SE;
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//ʮ����	���������롢�����ˡ�����
	if( !(chr&CHR_SHI_SAN_YAO).IsEmpty() )
	{
		chr &= ~CHR_WU_MEN_QI;
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//���۾�	���������͡�ͬ�̡�����
	if( !(chr&CHR_QING_YAO_JIU).IsEmpty() )
	{
		chr &= ~CHR_PENG_PENG;
		chr &= ~CHR_SAN_TONG_KE;
		chr &= ~CHR_SHUANG_TONG_KE;
		chr &= ~CHR_WU_ZI;
	}
	//С��ϲ	���������
	if( !(chr&CHR_XIAO_SHI_XI).IsEmpty() )
		chr &= ~CHR_SAN_FENG_KE;
	//С��Ԫ	���Ƽ���
	if( !(chr&CHR_JIAN_KE).IsEmpty() )
		chr &= ~CHR_JIAN_KE;
	//��һɫ	����������
	if( !(chr&CHR_ZI_YI_SE).IsEmpty() )
		chr &= ~CHR_PENG_PENG;
	//�İ���	������ǰ�塢�����͡������̡�˫����
	if( !(chr&CHR_SI_AN_KE).IsEmpty() )
	{
		chr &= ~CHR_MEN_QI_QING;
		chr &= ~CHR_PENG_PENG;
		chr &= ~CHR_SAN_AN_KE;
		chr &= ~CHR_SHUANG_AN_KE;
	}
	//һɫ˫����	����ƽ�͡��߶ԡ���һɫ
	if( !(chr&CHR_YI_SE_SHUANG).IsEmpty() )
	{
		chr &= ~CHR_PING_HU;
		chr &= ~CHR_QI_DUI;
		chr &= ~CHR_QING_YI_SE;
	}
	//һɫ��ͬ˳	����һɫ���ڸߡ�һ��ߡ��Ĺ�һ
	if( !(chr&CHR_YI_SHI_TONG).IsEmpty() )
	{
		chr &= ~CHR_YI_SE_JIE;
		chr &= ~CHR_YI_BAN_GAO;
		chr &= ~CHR_SHI_GUI_YI;
	}
	//һɫ�Ľڸ�	����һɫ��ͬ˳��������
	if( !(chr&CHR_YI_SHI_JIE).IsEmpty() )
	{
		chr &= ~CHR_YI_SE_TONG;
		chr &= ~CHR_PENG_PENG;
	}
	//���۾�	����������
	if( !(chr&CHR_HUN_YAO).IsEmpty() )
		chr &= ~CHR_PENG_PENG;
	//�߶�	���Ʋ����ˡ�����
	if( !(chr&CHR_QI_DUI).IsEmpty() )
	{
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//���ǲ���	���������롢�����ˡ�����
	if( !(chr&CHR_QI_XING_BU_KAO).IsEmpty() )
	{
		chr &= ~CHR_WU_MEN_QI;
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//ȫ˫��	���������͡�����
	if( !(chr&CHR_QUAN_SHUANG).IsEmpty() )
	{
		chr &= ~CHR_PENG_PENG;
		chr &= ~CHR_DUAN_YAO;
	}
	//��һɫ	������
	if( !(chr&CHR_QING_YI_SE).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//һɫ��ͬ˳	����һɫ���ڸ�
	if( !(chr&CHR_YI_SE_TONG).IsEmpty() )
		chr &= ~CHR_YI_SE_JIE;
	//һɫ���ڸ�	����һɫ��ͬ˳
	if( !(chr&CHR_YI_SE_JIE).IsEmpty() )
		chr &= ~CHR_YI_SE_TONG;
	//ȫ��	��������
	if( !(chr&CHR_QUAN_DA).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//ȫ��	���ƶ���
	if( !(chr&CHR_QUAN_ZHONG).IsEmpty() )
		chr &= ~CHR_DUAN_YAO;
	//ȫС	��������
	if( !(chr&CHR_QUAN_XIAO).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//��ɫ˫����	����ϲ��ꡢ���ٸ������֡�ƽ��
	if( !(chr&CHR_SAN_SE_SHUANG).IsEmpty() )
	{
		chr &= ~CHR_XI_XIANG_FENG;
		chr &= ~CHR_LAO_SHAO_JIANG;
		chr &= ~CHR_WU_ZI;
		chr &= ~CHR_PING_HU;
	}
	//ȫ����	���ƶ���
	if( !(chr&CHR_QUAN_DAI_WU).IsEmpty() )
		chr &= ~CHR_DUAN_YAO;
	//��ͬ��	����˫ͬ��
	if( !(chr&CHR_SAN_TONG_KE).IsEmpty() )
		chr &= ~CHR_SHUANG_TONG_KE;
	//������	����˫����
	if( !(chr&CHR_SAN_AN_KE).IsEmpty() )
		chr &= ~CHR_SHUANG_AN_KE;
	//ȫ����	���������롢�����ˡ�����
	if( !(chr&CHR_QUAN_BU_KAO).IsEmpty() )
	{
		chr &= ~CHR_WU_MEN_QI;
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//������	��������
	if( !(chr&CHR_DA_WU).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//С����	��������
	if( !(chr&CHR_XIAO_WU).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//�Ʋ���	����ȱһ��
	if( !(chr&CHR_TUI_BU_DAO).IsEmpty() )
		chr &= ~CHR_QUE_YI_MEN;
	//���ֻش�	��������
	if( !(chr&CHR_MIAO_SHOU).IsEmpty() )
		chr &= ~CHR_ZI_MO;
	//���Ͽ���	��������
	if( !(chr&CHR_GANG_KAI).IsEmpty() )
		chr &= ~CHR_ZI_MO;
	//���ܺ�	���ƺ;���
	if( !(chr&CHR_QIANG_GANG).IsEmpty() )
		chr &= ~CHR_JUE_ZHANG;
	//ȫ����	���Ƶ���
	if( !(chr&CHR_QUAN_QIU_REN).IsEmpty() )
		chr &= ~CHR_DAN_DIAO_JIANG;
	//˫����	���ư���
	if( !(chr&CHR_SHUANG_AN_GANG).IsEmpty() )
		chr &= ~CHR_AN_GANG;
	//˫����	���Ƽ���
	if( !(chr&CHR_SHUANG_JIAN_KE).IsEmpty() )
		chr &= ~CHR_JIAN_KE;
	//˫����	��������
	if( !(chr&CHR_SHUANG_MING_GANG).IsEmpty() )
		chr &= ~CHR_MING_GANG;
	//������	���Ʊ��š���
	if( !(chr&CHR_DAN_DIAO_JIANG).IsEmpty() )
	{
		chr &= ~CHR_BIAN_ZHANG;
		chr &= ~CHR_KAN_ZHANG;
	}
	//����	���ƿ���
	if( !(chr&CHR_BIAN_ZHANG).IsEmpty() )
		chr &= ~CHR_KAN_ZHANG;
}

// ��ⲹ���û�
int CTableFrameSink::CheckReplaceUser()
{
	int cardindex=-1;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�û�����
		WORD wUserIndex=(m_wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

		for( BYTE j = MAX_INDEX-MAX_HUA_CARD; j < MAX_INDEX; j++ )
		{
			if( m_cbCardIndex[wUserIndex][j] > 0 )
			{
				m_wReplaceUser = wUserIndex;
				cardindex=j;
				break;
			}
		}

		if( m_wReplaceUser != INVALID_CHAIR ) break;
	}
	return cardindex;

	////�û��ж�
	//for (WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	//�û�����
	//	WORD wUserIndex=(wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

	//	//�˿��ж�
	//	for (BYTE j=MAX_INDEX-MAX_HUA_CARD;j<MAX_INDEX;j++)
	//	{
	//		if (m_cbCardIndex[wUserIndex][j]>0)
	//		{
	//			m_wReplaceUser=wUserIndex;
	//			break;
	//		}
	//	}

	//	if( m_wReplaceUser != INVALID_CHAIR ) break;
	//}

	//for (WORD i=0;i<m_wPlayerCount;i++)
	//{
	//	//�û�����
	//	WORD wUserIndex=(m_wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

	//	for( BYTE j = MAX_INDEX-MAX_HUA_CARD; j < MAX_INDEX; j++ )
	//	{
	//		if( m_cbCardIndex[wUserIndex][j] > 0 )
	//		{
	//			m_wReplaceUser = wUserIndex;
	//			break;
	//		}
	//	}

	//	if( m_wReplaceUser != INVALID_CHAIR ) break;
	//}
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
