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

//��ʱ����ʶ
#define IDI_CHIP_OP                 (1)           //�ж���ʱ��
#define IDI_START_GAME				(2)									
#define IDI_BANK_OPERATE			(3)                   
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
	m_bGiveUp = false;
	m_bShowHand = false;
	m_bWillWin = false;
	m_lBeforeScore = 0;
	
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
	m_bGiveUp = false;

	//��ע��Ϣ
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;

	m_lBeforeScore = 0;

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
	m_pIAndroidUserItem->KillGameTimer(nTimerID);
	switch (nTimerID)
	{
	case IDI_START_GAME:
		{
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_CHIP_OP:
		{
			if(m_bGiveUp == true) return true;  //����û��Ѿ������Ļ��˳�

 			OnSubAddScoreEx();   

			return true;
		}
	case IDI_BANK_OPERATE:		//���в���
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			//��������
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			LONGLONG lRobotScore = pUserItem->GetUserScore();			
			{

				//�жϴ�ȡ
				if (lRobotScore > m_lRobotScoreRange[1])
				{
					LONGLONG lSaveScore=0L;

					lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0)
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

				}
				else if (lRobotScore < m_lRobotScoreRange[0])
				{
					DZPKSCORE lScore=m_lRobotBankGetScore+m_lRobotBankGetScoreBanker/(rand()%3+3);
					if (lScore > 0)
						m_pIAndroidUserItem->PerformTakeScore(lScore);
				}
			}
			return false;
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
	case SUB_S_ADMIN_GET_CARD:		//������ҵ�������������
		{
			return OnSubGetAllCard(pBuffer,wDataSize);
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
	case SUB_S_GIVE_UP:			//�û�����
		{
            return OnSubGiveUp(pBuffer,wDataSize);
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

			//memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			//��Ϣ����
			ReadConfigInformation();
			//�������
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=CGameLogic::GetRand(1,3);
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			return true;
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

	/*		m_lCellScore = pStatusPlay->lCellScore;

			m_lDrawMaxScore = pStatusPlay->lDrawMaxScore;
			m_lTurnLessScore = pStatusPlay->lTurnLessScore;
			m_lTurnMaxScore = pStatusPlay->lTurnMaxScore;*/

			//memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));
			ReadConfigInformation();
		
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
	m_lCellScore = pGameStart->lCellScore;  //��Ԫ����

	//��ע��Ϣ
	m_lTableScore[pGameStart->wDUser] += m_lCellScore;
	m_lTableScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;
	m_lTotalScore[pGameStart->wDUser] =  m_lCellScore;
	m_lTotalScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;

	m_bShowHand = false;
	m_bGiveUp = false;
	m_nCurCardCount = 2;

	//���ư���
	CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(m_cbHandCardData));
	
	return true;
}

//������ҵ�������������
bool CAndroidUserItemSink::OnSubGetAllCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize<=sizeof(CMD_S_GetAllCard)) return false;
	CMD_S_GetAllCard * pAllCard=(CMD_S_GetAllCard *)pBuffer;

	m_vAllCard.clear();

	std::stringstream ss;
	IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pMeUserItem)
	{
		WORD chairId0=m_pIAndroidUserItem->GetChairID();
		WORD uid0=m_pIAndroidUserItem->GetUserID();//uid0��ֵ�Ǵ��
		WORD tableId0=m_pIAndroidUserItem->GetTableID();
		WORD chairId=pMeUserItem->GetChairID();
		DWORD uid=pMeUserItem->GetUserID();//uid��ֵ�ǶԵ�
		WORD tableId=pMeUserItem->GetTableID();

		char sz1[1024]={0};
		sprintf(sz1,"OnSubGetAllCard������ҵ������������� chairId0=%d,uid0=%d,tableId0=%d,chairId=%d,uid=%d,tableId=%d",chairId0,uid0,tableId0,chairId,uid,tableId);
		ss<<sz1<<"\n";

		for(int i=0;i<pAllCard->cbCount;i++)
		{
		   tagMadeHandsOrder obj=(*pAllCard)[i];
		   m_vAllCard.push_back(obj);
		   if(i==0)
		   {
		      m_bWillWin=(obj.wChairID==chairId);//�Ƿ�ǰ������Ӯ
		   }
	       string strName;
		   IServerUserItem * pItem=m_pIAndroidUserItem->GetTableUserItem(obj.wChairID);
		   if(pItem)
		   {
			   TCHAR szNickName[LEN_NICKNAME]={0};
			   _sntprintf(szNickName,CountArray(szNickName),TEXT("%s"),pItem->GetNickName());
			   strName=ws2s(szNickName);
			   strName=Trim(strName);
		   }
		   ss<<"��"<<i<<"�����"<<strName.c_str()<<"(chairId="<<obj.wChairID<<",uid="<<obj.dwUserID<<")�������������ݣ�order="<<obj.order<<",cbLastCardKind="<<(int)obj.cbLastCardKind<<"\n";
		}
		string s = ss.str();
		ss.str("");
		printLog((char *)s.c_str());
	}

	if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=CGameLogic::GetRand(5,7);
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse);
	}

	return true;
}

//��ע��Ϣ
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//��������
	WORD wAddScoreUser=pAddScore->wAddScoreUser;

	//���ñ���
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;
	m_lTurnMaxScore = pAddScore->lTurnMaxScore;
	m_lAddLessScore = pAddScore->lAddLessScore;

#if _MSC_VER == 1400
	if(abs((double)pAddScore->lAddScoreCount-m_lTurnMaxScore)<0.001)
		m_bShowHand = true;
#else
	if(abs(pAddScore->lAddScoreCount-m_lTurnMaxScore)<0.001)
		m_bShowHand = true;
#endif

	m_lBeforeScore = pAddScore->lAddScoreCount;

	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=CGameLogic::GetRand(3,5);
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse);
	}
	
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
	WORD chairId0=m_pIAndroidUserItem->GetChairID();
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;
	if(m_lTableScore[wGiveUpUser]!=0L)
	{
		m_lTableScore[wGiveUpUser] = 0L;
	}

	//״̬����
	if(wGiveUpUser==chairId0) 
		m_bGiveUp = true;

	for(vector<tagMadeHandsOrder>::iterator it=m_vAllCard.begin(); it!=m_vAllCard.end();)
	{
		if(it->wChairID == wGiveUpUser)
		{
			it=m_vAllCard.erase(it);
		}
		else
		{
			++it;
		}
	}

	int iCount=m_vAllCard.size();
	for(int i=0;i<iCount;i++)
	{
	    m_vAllCard[i].order=i;
		if(i==0)
		{
			m_bWillWin=(chairId0==m_vAllCard[i].wChairID);//�Ƿ�ǰ������Ӯ
		}
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

/*	if((pSendCard->cbSendCardCount < 3))
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,rand()%TIME_START_GAME+2);
	}*/

	m_nCurCardCount = pSendCard->cbSendCardCount;
	
	if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=CGameLogic::GetRand(1,2);
		m_pIAndroidUserItem->SetGameTimer(IDI_CHIP_OP,nElapse);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	UINT nElapse=CGameLogic::GetRand(5,7);
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
	
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////
/////���в���
void CAndroidUserItemSink::OnSubAddScoreEx()
{
	//�γ������޸�
	srand((unsigned)time(NULL));//�������������
	//��ȡ��ǰ�������û�
	WORD MeChairID=m_pIAndroidUserItem->GetChairID();


	//����û�����Ļ�,��ô��?
	if(m_bWillWin)
	{
		TraceString(_T("Ӯ"),TraceLevel_Info);  //�����Ϣ
	}else{
	
		TraceString(_T("��"),TraceLevel_Info);  //�����Ϣ
	}
	
	switch(m_nCurCardCount)   //��ǰ����
	{
	case 2:     //�շ���   //��һ���Ʋ�Ӧ�÷���
		{
				TraceString(_T("û�з���"),TraceLevel_Info);  //�����Ϣ
				if(m_bWillWin)     //������ʤ��
				{
					if (GaiLv(50))	//һ������һ��������������ϵ���С����ҵ������аٷ�֮70��ע
					{
						FollowScore();
						return;
					}
					else 
					{
						//��ע
						AddScore();
						return;
					}
				}
				else  //��������
				{
					if (GaiLv(80))	//�ٷ�֮80��ע
					{
						FollowScore();
						return;
					}
					else 
					{
						//�����Ƿ������ע
						if (GaiLv(80))  //��������,�ٷ�֮80��ע
						{
							AddScore();
							return;
						}
						else	//give up
						{
							TraceString(_T("�����˷���1"),TraceLevel_Info);  //�����Ϣ
							GiveUpScore();//�����������������
							return;
						}
					}
				}


			
		}

	case 3:  //���͵�3����
		{
			
					TraceString(_T("��һ�η���"),TraceLevel_Info);  //�����Ϣ
					if(m_bWillWin)  //������ʤ��
					{
						if (GaiLv(50)) 
						{
							FollowScore();
							return;
						}
						else
						{

							//��ע
							AddScore();
							return;

						}
					}
					else   //��������   
					{
						if (GaiLv(80))    //�ٷ�֮80 ��ע
						{
							FollowScore();  
							return;
						}
						else
						{
							if (GaiLv(80))      //�ٷ�֮80��ע
							{
								AddScore();
								return;
							}
							else
							{
							    TraceString(_T("�����˷���2"),TraceLevel_Info);  //�����Ϣ
								GiveUpScore();  //����
								return;
							}
						}
					}

				
			
		}
	case 4:  //���͵�4����
		{
			TraceString(_T("���Ĵη���"),TraceLevel_Info);  //�����Ϣ
			if (m_bShowHand) //������û����
			{
				if(m_bWillWin)			//������ʤ��
				{


						FollowScore();//��������ϵ��ƴ�����������ϵ�����90%�ĸ�ע
						return;

				}
				else    //��������
				{
					if (GaiLv(5))      //�ٷ�֮5���ܻ��
					{
						FollowScore();//��������ϵ��ƴ�����������ϵ�����90%�ĸ�ע
						return;
					}
					else            
					{
						TraceString(_T("�����˷���3"),TraceLevel_Info);  //�����Ϣ
						GiveUpScore();//��������ϵ��ƴ�����������ϵ�����10%����
						return;
					}
				}
			}
			else        //û���û����
			{
				if(m_bWillWin)     //�û�ʤ��
				{
						if (GaiLv(10))     //�ٷ�֮10���
						{
								SendSuoHand();	//������������׼����������˵������������������80%���
								return;
						}
						else   
						{
								if (GaiLv(50))       
								{
									AddScore();		//������������׼����������˵�����������������У�100-80����70=14%��ע
									return;
								}
								else
								{
									FollowScore();	//������������׼����������˵�����������������У�100-80����30=6%��ע			
									return;
								}
						}
						
					
				}
				else    //��������
				{
					if (GaiLv(10)) //������������׼����������˵�����С������������30%��ע
					{
						FollowScore();
						return;
					}
					else
					{
						TraceString(_T("�����˷���4"),TraceLevel_Info);  //�����Ϣ
						GiveUpScore();//������������׼����������˵�����С������������70%����
						return;
					}
						
				}
			}
				
		}
		
	case 5:    //һ����������
		{
		
			TraceString(_T("�����η���"),TraceLevel_Info);  //�����Ϣ
			if (m_bShowHand)    //���û����
			{
				if(m_bWillWin)      //������ʤ��
				{
						FollowScore();	//����������׼����������˵������������������60%�ĸ��ʸ�ע		
						return;
				}
				else
				{
						TraceString(_T("�����˷���5"),TraceLevel_Info);  //�����Ϣ
						GiveUpScore();//����������׼����������˵�����С������������90%�ĸ��ʷ���
						return;	

				}

			}
			else   //û���û����
			{
				if(m_bWillWin)     //������ʤ��
				{
				
						if (GaiLv(20))
						{
							SendSuoHand();	//������������׼����������˵�����������������У�100-30����90=63%�ĸ������			
							return;
						}
						else
						{
							if (GaiLv(65))
								AddScore();	
							else
								FollowScore();	//������������׼����������˵�����������������У�100-30����10=7%�ĸ��ʸ�ע				
								
							return;
						}

				}
				else      //��������
				{

							if (GaiLv(20))
							{
									FollowScore();	//������������׼����������˵�����������������У�100-30����90=63%�ĸ������			
									return;
							}
							else
							{
									TraceString(_T("�����˷���6"),TraceLevel_Info);  //�����Ϣ
									GiveUpScore();	//������������׼����������˵�����������������У�100-30����10=7%�ĸ��ʸ�ע				
									return;
							}

				
				}
			}
		}
		ASSERT(false);
		
	}
}
void CAndroidUserItemSink::GiveUpScore()
{
		m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
		return;
}

void CAndroidUserItemSink::FollowScore()
{
	if(m_lTurnLessScore<=0.001)
	{
		CMD_C_AddScore AddScore;
		AddScore.lScore=SCORE_ZERO;
		AddScore.cbJudgeAction=GAME_ACTION_PASS;
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
	}
	else
	{
		CMD_C_AddScore AddScore;
		AddScore.lScore=m_lTurnLessScore;
		AddScore.cbJudgeAction=GAME_ACTION_FOLLOW;
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));	
	}
	return;
}

void CAndroidUserItemSink::AddScore()
{
	DZPKSCORE lCurrentAddScore = m_lBeforeScore+(rand()%3+1)*m_lCellScore;
	if(lCurrentAddScore<m_lAddLessScore)
		lCurrentAddScore = m_lAddLessScore;

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentAddScore;
	AddScore.cbJudgeAction=GAME_ACTION_ADD;
	m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
	return;
}

void CAndroidUserItemSink::SendSuoHand()
{
		DZPKSCORE lCurrentScore = m_lTurnMaxScore;
		//������Ϣ
		CMD_C_AddScore AddScore;
		AddScore.lScore=lCurrentScore;
	    AddScore.cbJudgeAction=GAME_ACTION_ALLIN;
		m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
		return;
}

bool CAndroidUserItemSink::GaiLv(BYTE bNum)
{
	if (rand()%100<bNum)
	{
		return true;
	}
	return false;
}
bool CAndroidUserItemSink::WinIsAndroid(BYTE cbStartPos, BYTE cbConcludePos)
{
	WORD iWinner = EstimateWinner(cbStartPos,cbConcludePos);
	if(iWinner==m_pIAndroidUserItem->GetChairID())
		return true;
	return false;
}
WORD CAndroidUserItemSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	return 1;
}

//////////////////////////////////////////////////////////////////////////
//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����


	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\DZShowHandConfig.ini"),szPath);

	//��������
	m_lRobotScoreRange[0] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMin"), 100, szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMax"), 100000, szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������
	m_lRobotBankGetScore = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGet"), 20000000, szConfigFileName);

	//������� 
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGetBanker"), 30000000, szConfigFileName);

	//����
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

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

		OutputDebugString(CA2T(buf));
		ofstream outf(m_strLogFile.c_str(),ios::app); 
		outf<<timeFormat<<buf<<endl;
	}
#endif
}

void CAndroidUserItemSink::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	CString str=pszString;
	printLog((LPSTR)CT2A(str));
	CTraceService::TraceString(pszString,TraceLevel);
}