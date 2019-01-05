#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"
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

//ʱ���ʶ
#define IDI_PLACE_JETTON1			103									//��ע��ʱ
#define IDI_PLACE_JETTON2			104									//��ע��ʱ
#define IDI_PLACE_JETTON3			105									//��ע��ʱ
#define IDI_PLACE_JETTON4			106									//��ע��ʱ
#define IDI_PLACE_JETTON5			107									//��ע��ʱ
#define IDI_CHECK_BANKER			108									//�����ׯ
#define IDI_REQUEST_BANKER			101									//���붨ʱ
#define IDI_GIVEUP_BANKER			102									//��ׯ��ʱ
#define IDI_PLACE_JETTON			110									//��ע���� (Ԥ��110-160)

//////////////////////////////////////////////////////////////////////////

int CAndroidUserItemSink::m_stlApplyBanker = 0L;

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//��ׯ����
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	m_nRobotListMinCount = 2;
	m_nListUserCount=0;

	//�������
	m_RobotInfo.nAreaChance[0] = 5;
	m_RobotInfo.nAreaChance[1] = 0;
	m_RobotInfo.nAreaChance[2] = 5;
	m_RobotInfo.nAreaChance[3] = 1;

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
bool  CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}
//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

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

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{

	return true;
}
//���ýӿ�
bool  CAndroidUserItemSink::RepositUserItemSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//��ׯ����
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_CHECK_BANKER:		//�����ׯ
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
				return false;

			int nMinCount = m_nRobotApplyBanker;
			if ( m_nRobotApplyBanker > m_nRobotListMinCount)
				nMinCount = (rand()%(m_nRobotApplyBanker - m_nRobotListMinCount)) + m_nRobotListMinCount;

			printLog("�����ׯ ��ǰ��ׯ����m_nListUserCount=%I64d,�����������������nMinCount=%d",m_nListUserCount,nMinCount);

			if (m_wCurrentBanker == INVALID_CHAIR||m_nListUserCount<nMinCount)
			{
				//��ׯ
				m_nWaitBanker++;
							
				WORD chairId0=m_pIAndroidUserItem->GetChairID();
				printLog(("��������ׯ(End) chairId0=%d [�ռ���m_nWaitBanker=%d ��������m_nRobotWaitBanker=%d] [������m_stlApplyBanker=%d ��ׯ����m_nRobotApplyBanker=%d"), chairId0,m_nWaitBanker, m_nRobotWaitBanker, m_stlApplyBanker, m_nRobotApplyBanker);

				//��������ׯ
				if ( m_bRobotBanker && m_nWaitBanker >= m_nRobotWaitBanker )
				{
					//�Ϸ��ж�
					IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
					if (pIUserItemBanker->GetUserScore() > m_lBankerCondition) 
					{
						//��������ׯ
						m_nBankerCount = 0;
						m_stlApplyBanker++;
						m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % m_cbTimeLeave) + 1);
					}
				}
			}
			else if ( m_wCurrentBanker != INVALID_CHAIR )
			{
				//��������ׯ
				m_nWaitBanker = 0;
				


			}

			return false;
		}
	case IDI_REQUEST_BANKER:	//������ׯ
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER);

			return false;
		}
	case IDI_GIVEUP_BANKER:		//������ׯ
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER);

			return false;
		}
	default:
		{
			if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON+MAX_CHIP_TIME)
			{
				srand(GetTickCount());

				//��������
				int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
				LONGLONG lMaxChipLmt = __min(m_lMaxChipBanker, m_lMaxChipUser);			//������עֵ
				WORD wMyID = m_pIAndroidUserItem->GetChairID();

				//ͳ�ƴ���
				m_nChipTimeCount++;

				//����˳�	
				int MinJetton=m_RobotInfo.nChip[m_nChipLimit[0]];//�������⸶���Ƶ���ע������С����ֵ
				if (lMaxChipLmt < MinJetton)	return false;
				for (int i = 0; i < AREA_COUNT; i++)
				{
					if (m_lAreaChip[i] >= MinJetton)	break;
					if (i == AREA_COUNT-1)	return false;
				}

				//��ע����
				for (int i = 0; i < AREA_COUNT; i++)
					nACTotal += m_RobotInfo.nAreaChance[i];
				ASSERT(nACTotal>0);
				static int chip_area_arr[AREA_COUNT] = {0,1,2,3};
				do
				{
					nChipArea=CGameLogic::SelectByProb(chip_area_arr,m_RobotInfo.nAreaChance,AREA_COUNT,nACTotal);
				}
				while (m_lAreaChip[nChipArea] < MinJetton);

				//��ע��С
				if (m_nChipLimit[0] == m_nChipLimit[1])
					nCurChip = m_nChipLimit[0];
				else
				{
					//���ñ���
					lMaxChipLmt = __min(lMaxChipLmt, m_lAreaChip[nChipArea]);
					nCurJetLmt[0] = m_nChipLimit[0];
					nCurJetLmt[1] = m_nChipLimit[1];

					//���㵱ǰ������
					for (int i = m_nChipLimit[1]; i > m_nChipLimit[0]; i--)
					{
						if (lMaxChipLmt > m_RobotInfo.nChip[i]) 
						{
							nCurJetLmt[1] = i;
							break;
						}
					}

					//�����ע������ֵ����
					nCurChip = CGameLogic::GetRand(nCurJetLmt[0],nCurJetLmt[1]);

					//���¿��� (��ׯ�ҽ�ҽ���ʱ�ᾡ����֤�������)
					if (m_nChipTimeCount < m_nChipTime)
					{
						LONGLONG lLeftJetton = LONGLONG( (lMaxChipLmt-m_RobotInfo.nChip[nCurChip])/(m_nChipTime-m_nChipTimeCount) );

						//�������� (��ȫ����С���Ƴ�����עҲ����)
						if (lLeftJetton < MinJetton && nCurChip > m_nChipLimit[0])
							nCurChip--;
					}
				}

				printLog(("��������ע wMyID=%d ��ע����[%d/%d] ��ע[nChipArea=%d,nChip=%d] ��Χm_nChipLimit=[%d,%d] ����=[�����ע(ׯ��)=%I64d,�����ע(����)=%I64d,lMaxChipLmt=%I64d]"), \
					wMyID, \
					nTimerID-IDI_PLACE_JETTON, \
					m_nChipTime, \
					nChipArea, \
					m_RobotInfo.nChip[nCurChip], \
					m_nChipLimit[0], \
					m_nChipLimit[1], \
					m_lMaxChipBanker, \
					m_lMaxChipUser, \
					lMaxChipLmt \
					);

				//��������
				CMD_C_PlaceJetton PlaceJetton = {};

				//�������
				PlaceJetton.cbJettonArea = nChipArea+1;		//������1��ʼ
				PlaceJetton.lJettonScore = m_RobotInfo.nChip[nCurChip];

				//������Ϣ
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			}

			m_pIAndroidUserItem->KillGameTimer(nTimerID);
			return false;
		}
	}
	return false;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:			//��Ϸ���� 
		{
			return OnSubGameFree(pBuffer, wDataSize);
		}
	case SUB_S_GAME_START:			//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
	case SUB_S_PLACE_JETTON:		//�û���ע
		{
			return OnSubPlaceJetton(pBuffer, wDataSize);
		}
	case SUB_S_APPLY_BANKER:		//������ׯ 
		{
			return OnSubUserApplyBanker(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER:		//ȡ����ׯ 
		{
			return OnSubUserCancelBanker(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER:		//�л�ׯ�� 
		{
			return OnSubChangeBanker(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:			//��Ϸ���� 
		{
			return OnSubGameEnd(pBuffer, wDataSize);
		}
	case SUB_S_SEND_RECORD:			//��Ϸ��¼ (����)
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ�� (����)
		{
			return true;
		}
	}

	//�������
	ASSERT(FALSE);

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
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lUserLimitScore = pStatusFree->lUserMaxScore;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			m_lBankerCondition = pStatusFree->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);

			WORD chairId0=m_pIAndroidUserItem->GetChairID();
			printLog(("��������ׯ(Free) chairId0=%d [�ռ���m_nWaitBanker=%d ��������m_nRobotWaitBanker=%d] [������m_stlApplyBanker=%d ��ׯ����m_nRobotApplyBanker=%d"), chairId0,m_nWaitBanker, m_nRobotWaitBanker, m_stlApplyBanker, m_nRobotApplyBanker);

			//��ׯ����
			if (pStatusFree->wBankerUser == INVALID_CHAIR)
			{
				if (m_bRobotBanker && m_nRobotWaitBanker == 0  && m_stlApplyBanker < m_nRobotApplyBanker)
				{
					//�Ϸ��ж�
					IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
					if (pIUserItemBanker->GetUserScore() > m_lBankerCondition) 
					{
						//��������ׯ
						m_nBankerCount = 0;
						m_stlApplyBanker++;
						
						BYTE cbTime = (pStatusFree->cbTimeLeave>0?(rand()%pStatusFree->cbTimeLeave+1):2);
						if (cbTime == 0) cbTime = 2;

						m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, cbTime);
					}
				}
			}

			return true;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//ׯ����Ϣ
			m_wCurrentBanker = pStatusPlay->wBankerUser;

			m_lUserLimitScore = pStatusPlay->lUserMaxScore;
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			m_lBankerCondition = pStatusPlay->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);

			return true;
		}
	}

	return true;
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

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//��ȡ����
	if (m_bRefreshCfg)
		ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, false);

	//��Ϣ����
	CMD_S_GameFree* pGameFree=(CMD_S_GameFree *)pBuffer;

	m_cbTimeLeave = pGameFree->cbTimeLeave;
	m_nListUserCount = pGameFree->nListUserCount;

	

	bool bMeGiveUp = false;
	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
	{
		m_nBankerCount++;
		if ( m_nBankerCount >= m_nRobotBankerCount )
		{
			//��������ׯ
			m_nBankerCount = 0;
			m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

			bMeGiveUp = true;
		}
	}

	//�����ׯ
	if (m_wCurrentBanker!= m_pIAndroidUserItem->GetChairID() || bMeGiveUp)
	{
		m_cbTimeLeave = pGameFree->cbTimeLeave - 3;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER, 3);
	}

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	srand(GetTickCount());

	//�Լ���ׯ������ע������
	if (pGameStart->wBankerUser == m_pIAndroidUserItem->GetChairID() || pGameStart->nChipRobotCount <= 0)
		return true;

	//���ñ���
	m_lMaxChipBanker = pGameStart->lBankerScore/m_RobotInfo.nMaxTime;
	m_lMaxChipUser = pGameStart->lUserMaxScore/m_RobotInfo.nMaxTime;
	m_wCurrentBanker = pGameStart->wBankerUser;
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	for (int i = 0; i < AREA_COUNT; i++)
		m_lAreaChip[i] = m_lAreaLimitScore;

	//ϵͳ��ׯ
	if (pGameStart->wBankerUser == INVALID_CHAIR)
	{
		m_stlApplyBanker = 0;
		m_lMaxChipBanker = 2147483647/m_RobotInfo.nMaxTime;
	}
	else
		m_lMaxChipUser = __min( m_lMaxChipUser, m_lMaxChipBanker );

	//������ע����
	int nElapse = 0;												
	WORD wMyID = m_pIAndroidUserItem->GetChairID();

	if (m_nRobotBetTimeLimit[0] == m_nRobotBetTimeLimit[1])
		m_nChipTime = m_nRobotBetTimeLimit[0];
	else
		m_nChipTime = (rand()+wMyID)%(m_nRobotBetTimeLimit[1]-m_nRobotBetTimeLimit[0]+1) + m_nRobotBetTimeLimit[0];
	ASSERT(m_nChipTime>=0);		
	if (m_nChipTime <= 0)	return false;								//��ȷ,2����������
	if (m_nChipTime > MAX_CHIP_TIME)	m_nChipTime = MAX_CHIP_TIME;	//�޶�MAX_CHIP����ע

	//���㷶Χ
	if (!CalcJettonRange(__min(m_lMaxChipBanker, m_lMaxChipUser), m_lRobotJettonLimit, m_nChipTime, m_nChipLimit))
		return true;

	//����ʱ��
	vector<int> vElapse(m_nChipTime);
	for (int i = 0; i < m_nChipTime; i++)
	{
		nElapse = CGameLogic::GetRand(2,pGameStart->cbTimeLeave-1);
		vElapse[i]=nElapse;
	}
	std::sort(vElapse.begin(),vElapse.end());
	std::stringstream ss;
	for (int i = 0; i < m_nChipTime; i++)
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON+i+1, vElapse[i]);
		ss << vElapse[i];
		if(i < m_nChipTime-1)
			ss << ",";
	}
	string s = ss.str();
    ss.str("");

	printLog(("������ wMyID=%d ��ע����m_nChipTime=%d ��עʱ��=[%s] ��ע������ֵ������Χm_nChipLimit=[%d,%d] ������nChipRobotCount=%d ����[�����ע(ׯ��)m_lMaxChipBanker=%I64d,�����ע (����)m_lMaxChipUser=%I64d] ��ׯ[������m_stlApplyBanker=%d,��ׯ����m_nRobotApplyBanker=%d]"),  \
		wMyID,  \
		m_nChipTime,  \
		s.c_str(),  \
		m_nChipLimit[0],  \
		m_nChipLimit[1],  \
		pGameStart->nChipRobotCount, \
		m_lMaxChipBanker,  \
		m_lMaxChipUser,  \
		m_stlApplyBanker,  \
		m_nRobotApplyBanker \
		);

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	//���ñ���
	m_lMaxChipBanker -= pPlaceJetton->lJettonScore;
	m_lAreaChip[pPlaceJetton->cbJettonArea-1] -= pPlaceJetton->lJettonScore;
	if (pPlaceJetton->wChairID == m_pIAndroidUserItem->GetChairID())
		m_lMaxChipUser -= pPlaceJetton->lJettonScore;

	return true;
}

//��עʧ��
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	return true;
}

//������ׯ
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//�Լ��ж�
	if (m_pIAndroidUserItem->GetChairID()==pApplyBanker->wApplyUser) 
		m_bMeApplyBanker=true;

	return true;
}

//ȡ����ׯ
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//�Լ��ж�
	if (lstrcmp(m_pIAndroidUserItem->GetMeUserItem()->GetNickName(),pCancelBanker->szCancelUser)==0) 
		m_bMeApplyBanker=false;

	return true;
}

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	if ( m_wCurrentBanker == m_pIAndroidUserItem->GetChairID() && m_wCurrentBanker != pChangeBanker->wBankerUser )
	{
		m_stlApplyBanker--;
	}
	m_wCurrentBanker = pChangeBanker->wBankerUser;
	m_nWaitBanker = 0;

	return true;
}

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation(TCHAR szFileName[], TCHAR szRoomName[], bool bReadFresh)
{
	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	myprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\%s"), szPath, szFileName);

	//ÿ��ˢ��
	if (bReadFresh)
	{
		//ÿ��ˢ��
		BYTE cbRefreshCfg = GetPrivateProfileInt(szRoomName, TEXT("Refresh"), 0, szConfigFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;
	}

	//��������
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMaxJetton"), _T("5000000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[1]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMinJetton"), _T("100"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[0]);

	if (m_lRobotJettonLimit[1] > 1000)					m_lRobotJettonLimit[1] = 1000;
	if (m_lRobotJettonLimit[0] < 5)						m_lRobotJettonLimit[0] = 1;
	if (m_lRobotJettonLimit[1] < m_lRobotJettonLimit[0])	m_lRobotJettonLimit[1] = m_lRobotJettonLimit[0];

	//��������
	m_nRobotBetTimeLimit[0] = GetPrivateProfileInt(szRoomName, _T("RobotMinBetTime"), 4, szConfigFileName);;
	m_nRobotBetTimeLimit[1] = GetPrivateProfileInt(szRoomName, _T("RobotMaxBetTime"), 8, szConfigFileName);;

	if (m_nRobotBetTimeLimit[0] < 0)							m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0])		m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//�Ƿ���ׯ
	m_bRobotBanker = (GetPrivateProfileInt(szRoomName, _T("RobotBanker"), 0, szConfigFileName) == 1);

	//��ׯ����
	m_nRobotBankerCount = GetPrivateProfileInt(szRoomName, _T("RobotBankerCount"), 3, szConfigFileName);

	//��������
	m_nRobotWaitBanker = GetPrivateProfileInt(szRoomName, _T("RobotWaitBanker"), 3, szConfigFileName);


	//���ٸ���
	m_nRobotListMinCount = GetPrivateProfileInt(szRoomName, TEXT("RobotListMinCount"), 2, szConfigFileName);

	//������
	m_nRobotApplyBanker = GetPrivateProfileInt(szRoomName, _T("RobotApplyBanker"), 3, szConfigFileName);

	//��������
	m_bReduceJettonLimit = (GetPrivateProfileInt(szRoomName, TEXT("RobotReduceLimit"), 0, szConfigFileName)!=0);

	//�������
	m_RobotInfo.nAreaChance[0] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance1"), 3, szConfigFileName);
	m_RobotInfo.nAreaChance[1] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance2"), 0, szConfigFileName);
	m_RobotInfo.nAreaChance[2] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance3"), 3, szConfigFileName);
	m_RobotInfo.nAreaChance[3] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance4"), 1, szConfigFileName);

	printLog(("��ȡini���� ��������m_lRobotJettonLimit=[%I64d,%I64d],��ע��������m_lRobotJettonLimit=[%d,%d],�Ƿ���ׯm_bRobotBanker=%d,��ׯ����=m_nRobotBankerCount=%d,��������m_nRobotWaitBanker=%d,��ׯ����m_nRobotApplyBanker=%d,��������m_bReduceJettonLimit=%d"),  \
		m_lRobotJettonLimit[0],  \
		m_lRobotJettonLimit[1], \
		m_nRobotBetTimeLimit[0],  \
		m_nRobotBetTimeLimit[1],  \
		m_bRobotBanker,  \
		m_nRobotBankerCount,  \
		m_nRobotWaitBanker,  \
		m_nRobotApplyBanker,  \
		m_bReduceJettonLimit \
		);
}

bool CAndroidUserItemSink::CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[])
{

	printLog(("CalcJettonRange step1 lMaxScore=%I64d,lChipLmt=[%I64d,%I64d],nChipTime=%d,lJetLmt=[%d,%d]"),  \
		lMaxScore,  \
		lChipLmt[0],  \
		lChipLmt[1],  \
		nChipTime, \
		lJetLmt[0],  \
		lJetLmt[1]  \
		);

	//�������
	bool bHaveSetMinChip = false;

	//����һע
	if (lMaxScore < m_RobotInfo.nChip[0])	return false;

	//���÷�Χ
	for (int i = 0; i < CHIP_COUNT; i++)
	{
		if (!bHaveSetMinChip && m_RobotInfo.nChip[i] >= lChipLmt[0])
		{ 
			lJetLmt[0] = i;
			bHaveSetMinChip = true;
		}
		if (m_RobotInfo.nChip[i] <= lChipLmt[1])
			lJetLmt[1] = i;
	}
	if (lJetLmt[0] > lJetLmt[1])	lJetLmt[0] = lJetLmt[1];

	printLog(("CalcJettonRange step2 lMaxScore=%I64d,lChipLmt=[%I64d,%I64d],nChipTime=%d,lJetLmt=[%d,%d]"),  \
		lMaxScore,  \
		lChipLmt[0],  \
		lChipLmt[1],  \
		nChipTime, \
		lJetLmt[0],  \
		lJetLmt[1]  \
		);

	//�Ƿ񽵵�����
	if (m_bReduceJettonLimit)
	{
		if (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
		{
			//�Ƿ񽵵���ע����
			if (nChipTime * m_RobotInfo.nChip[0] > lMaxScore)
			{
				nChipTime = int(lMaxScore/m_RobotInfo.nChip[0]);
				lJetLmt[0] = 0;
				lJetLmt[1] = 0;
			}
			else
			{
				//���͵���������
				while (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
				{
					lJetLmt[0]--;
					ASSERT(lJetLmt[0]>=0);
				}
			}
			printLog(("CalcJettonRange step3 lMaxScore=%I64d,lChipLmt=[%I64d,%I64d],nChipTime=%d,lJetLmt=[%d,%d]"),  \
				lMaxScore,  \
				lChipLmt[0],  \
				lChipLmt[1],  \
				nChipTime, \
				lJetLmt[0],  \
				lJetLmt[1]  \
				);
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
