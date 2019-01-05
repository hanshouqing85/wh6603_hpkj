#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
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

#if 0//����һ��д��
template<class T>  
T Trim(const T& str)  
{  
	int length = str.size();  
	int i = 0,j = length -1;  
	// vc��isspaceʵ��> 256�ͱ���
	while(i < length && isspace(str[i] & 0xFF)){i++;}  
	while(j >= 0 && isspace(str[j] & 0xFF)){j--;}  
	if(j<i) return T();  
	return str.substr(i,j-i+1);  
}  
#else
template<typename E,typename TR,typename AL>
inline std::basic_string<E, TR, AL> Trim(const std::basic_string<E, TR, AL>&theString) 
{
	int aStartPos=0;
	while(aStartPos<(int)theString.length() && isspace(theString[aStartPos]))
		aStartPos++;
	int anEndPos=theString.length()-1;
	while(anEndPos>=0 && isspace(theString[anEndPos]))
		anEndPos--;
	return theString.substr(aStartPos,anEndPos-aStartPos+1);
}
#endif

//////////////////////////////////////////////////////////////////////////
//����ʱ��
#define TIME_LESS					2									//����ʱ��
//��ʱ����ʶ
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_USER_ADD_SCORE			201									//��ע��ʱ��

//ʱ���ʶ
#define TIME_START_GAME				10	/*20*/								//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			10	/*20*/								//������ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	ZeroMemory(m_bUserName,sizeof(m_bUserName));
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_bOpenCard = false;
	m_bExitTag	= false;

	//��ע��Ϣ
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//�˿˱���
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbOverCardData,sizeof(m_cbOverCardData));

	//״̬����
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	m_bAutoStart = FALSE;
	m_bReset =true;
	//�ӿڱ���
	m_pIAndroidUserItem=NULL;

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}
  //��ʼ�ӿ�
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}
//���ýӿ�
bool  CAndroidUserItemSink::RepositionSink()
{
	ZeroMemory(m_bUserName,sizeof(m_bUserName));
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_bOpenCard = false;
	m_bExitTag	= false;

	//��ע��Ϣ
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//�˿˱���
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbOverCardData,sizeof(m_cbOverCardData));

	//״̬����
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	m_bAutoStart = FALSE;
	m_bReset =true;
	return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:
		{
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_USER_ADD_SCORE:
		{
			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.lScore=m_lTurnLessScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;
		}
	default:
		{
			break;
		}
	}

	return false;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//�û���ע
		{
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return true;
}

bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wCmdID, void * pData, WORD wDataSize)
{
	BYTE mid=LOBYTE(wCmdID);
	BYTE sid=HIBYTE(wCmdID);

	//��¼���
	if ((mid==MDM_GR_LOGON)&&(sid==SUB_GR_LOGON_FINISH))
	{
		//��������
		tagAndroidUserParameter * pAndroidUserParameter=(tagAndroidUserParameter *)pData;
		createLogFile(pAndroidUserParameter);
		IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
		if(pMeUserItem)
		{
			WORD chairId0=m_pIAndroidUserItem->GetChairID();
			WORD uid0=m_pIAndroidUserItem->GetUserID();//uid0��ֵ�Ǵ��
			WORD tableId0=m_pIAndroidUserItem->GetTableID();
			WORD chairId=pMeUserItem->GetChairID();
			DWORD uid=pMeUserItem->GetUserID();//uid��ֵ�ǶԵ�
			WORD tableId=pMeUserItem->GetTableID();
			printLog("��¼���chairId0=%d,uid0=%d,tableId0=%d,chairId=%d,uid=%d,tableId=%d",chairId0,uid0,tableId0,chairId,uid,tableId);
		}
		return true;
	}


	return true;
}

//������Ϣ
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//�������
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	}
	return false;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void  CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//���ñ���
	m_wDUser = pGameStart->wDUser;
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lAddLessScore = pGameStart->lAddLessScore;
	m_lTurnLessScore = pGameStart->lTurnLessScore;
	m_lTurnMaxScore = pGameStart->lTurnMaxScore;
	m_lCellScore = pGameStart->lCellScore;

	//��ע��Ϣ
	m_lTableScore[pGameStart->wDUser] += m_lCellScore;
	m_lTableScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;
	m_lTotalScore[pGameStart->wDUser] =  m_lCellScore;
	m_lTotalScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;

	//���Ͱ���
	CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(m_cbHandCardData));
	m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,rand()%TIME_USER_ADD_SCORE);

	return true;
}

//��ע��Ϣ
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//��������
	//WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;

	//���ñ���
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;
	m_lTurnMaxScore = pAddScore->lTurnMaxScore;
	m_lAddLessScore = pAddScore->lAddLessScore;

	m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,rand()%TIME_USER_ADD_SCORE+5);
	
	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;

	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;

	if(m_lTableScore[wGiveUpUser]!=0L)
	{
		m_lTableScore[wGiveUpUser] = 0L;
	}

	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//У������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//��ǰ���
	m_wCurrentUser = pSendCard->wCurrentUser;
	CopyMemory(m_cbCenterCardData,pSendCard->cbCenterCardData,sizeof(BYTE)*(pSendCard->cbSendCardCount));

	if((pSendCard->cbSendCardCount < 3))
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,rand()%TIME_START_GAME+5);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,rand()%TIME_START_GAME+5);
	
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	return true;
}

void CAndroidUserItemSink::createLogFile(tagAndroidUserParameter * pAndroidUserParameter)
{
	tagAndroidService *pAndroidService = m_pIAndroidUserItem->GetAndroidService();
	tagAndroidParameter *pAndroidParameter = m_pIAndroidUserItem->GetAndroidParameter();
	IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pMeUserItem && pAndroidUserParameter)
	{
		WORD chairId=pMeUserItem->GetChairID();
		DWORD uid=pMeUserItem->GetUserID();//uid��ֵ�ǶԵ�
		WORD tableId=pMeUserItem->GetTableID();
		wstring wstr=pMeUserItem->GetNickName();
		wstr=Trim(wstr);
		string str=ws2s(wstr);
		WORD wKindID=pAndroidUserParameter->pGameServiceAttrib->wKindID;
		WORD wServerKind=pAndroidUserParameter->pGameServiceAttrib->wServerKind;
		WORD wServerID=pAndroidUserParameter->pGameServiceOption->wServerID;
		wstring szServerName=pAndroidUserParameter->pGameServiceOption->szServerName;
#ifdef USE_RS_PRINT
		wchar_t g_wcDirName[256]={0};
		wchar_t g_wcFileName[256]={0};
		wstring wstrPath=s2ws(GetAppPath());
		//wsprintfW(g_wcDirName,L"%s\\%s_%d_��%d��", 
		//	wstrPath.c_str(),
		//	szServerName.c_str(),
		//	wServerID,
		//	tableId
		//	);
		wsprintfW(g_wcDirName,L"%s\\%s_%d", 
			wstrPath.c_str(),
			szServerName.c_str(),
			wServerID
			);
		wsprintfW(g_wcFileName,L"\\%s_%d.txt", 
			wstr.c_str(),
			uid
			);
		m_strLogFile=g_wcDirName;
		m_strLogFile+=g_wcFileName;
		int ret=create_dir(g_wcDirName,wcslen(g_wcDirName));
#endif
	}
}

void CAndroidUserItemSink::printLog(char *szBuff,...)
{
#ifdef USE_RS_PRINT
	if(m_strLogFile!=L"")
	{
		SYSTEMTIME st={0};
		GetSystemTime(&st);	// ��õ�ǰʱ��
		char timeFormat[256]={0};
		sprintf(timeFormat,"��%04d-%02d-%02d %02d:%02d:%02d %03d��", 
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour+8,	// ��8�Ƕ��뱱��ʱ��
			st.wMinute,
			st.wSecond,
			st.wMilliseconds
			);

		char buf[1024]={0};
		va_list ap;
		va_start(ap,szBuff);
		vsprintf(buf,szBuff,ap);
		va_end(ap);

		//CTraceService::TraceString(CA2T(buf),TraceLevel_Normal);
		OutputDebugString(CA2T(buf));
		ofstream outf(m_strLogFile.c_str(),ios::app); 
		outf<<timeFormat<<buf<<endl;
	}
#endif
}

void CAndroidUserItemSink::printLog(CString& str)
{
	printLog((LPSTR)CT2A(str));
}

//////////////////////////////////////////////////////////////////////////
