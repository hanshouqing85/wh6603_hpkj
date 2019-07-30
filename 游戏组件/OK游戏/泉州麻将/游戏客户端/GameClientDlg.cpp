#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
#include ".\gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			202									//������ʱ��
#define IDT_AUTO					1									//����ʱ
#define IDT_REPAIR_FLOWER			2									//������ʱ��
#define IDI_OPEN_DOOR				203									//���Ŷ�ʱ��
#define IDI_OPEN_GOLD				204									//����ʱ��
#define IDI_REPAIR_FLOWER			205									//������ʱ��

//��Ϸ��ʱ��
#define TIME_START_GAME				40									//��ʼ��ʱ��
#define TIME_HEAR_STATUS			1									//���ƶ�ʱ��
#define TIME_OPERATE_CARD			150//15									//������ʱ��
#define TIME_OPEN_DOOR				1//15									//������ʱ
#define TIME_OPEN_GOLD				1//15									//������ʱ

//windows��ʱ��
#define TIME_AUTO					2000								//�йܶ�ʱ��

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

BEGIN_MESSAGE_MAP(CGameClientDlg,CGameFrameEngine)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_AUTO,OnAuto)
	ON_MESSAGE(IDM_OPEN_DOOR,OnOpenDoor)
	ON_MESSAGE(IDM_OPEN_GOLD,OnOpenGold)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameEngine()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//״̬����
	m_bAuto=false;
	m_wDoorDiceCount = 0;
	m_wGoldDiceCount = 0;
	m_cbSubtractTailCount = 0;
	m_cbSubtractHeadCount = 0;

	//��������
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_cbGoldCard,sizeof(m_cbGoldCard));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));
	//m_cMsg=NULL;


	//m_GameClientView.m_btAutoOn.EnableWindow(true);
	//m_GameClientView.m_btAutoOn.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btAutoOff.EnableWindow(false);
	//m_GameClientView.m_btAutoOff.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenDoor.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenGold.ShowWindow(SW_HIDE);
	m_GameClientView.SetGoldCardShow(false);
	m_GameClientView.SetGoldOutEnable(false);

	m_GameClientView.SetDoorDice(0);
	m_GameClientView.SetGoldDice(0);

	
	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
	
}

//��ʼ����
bool CGameClientDlg::OnInitGameEngine()
{
	//���ñ���
	SetWindowText(TEXT("Ȫ���齫��Ϸ"));


	//ʱ������
	/* SYSTEMTIME SysTime;
	 GetSystemTime(&SysTime);
	 if(SysTime.wDay>10)
	 {
		MessageBox(TEXT("��ǰϵͳʹ���Ѿ����ڣ�"),TEXT("ʹ�ù���"),MB_OK|MB_ICONWARNING);
		m_GameClientView.ShowWindow(SW_HIDE);
		this->EndDialog(true);
		 return false;
	 }*/

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);//by hxh 20170308
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	g_CardResource.LoadResource();

	return true;
}

//���ÿ��
bool CGameClientDlg::OnResetGameEngine()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//״̬����
	m_bAuto=false;
	m_wDoorDiceCount = 0;
	m_wGoldDiceCount = 0;
	m_cbSubtractTailCount = 0;
	m_cbSubtractHeadCount = 0;

	//��������
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_cbGoldCard,sizeof(m_cbGoldCard));



	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));

	//m_GameClientView.m_btAutoOn.EnableWindow(true);
	//m_GameClientView.m_btAutoOn.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btAutoOff.EnableWindow(false);
	//m_GameClientView.m_btAutoOff.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenDoor.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenGold.ShowWindow(SW_HIDE);
	m_GameClientView.SetGoldCardShow(false);
	m_GameClientView.SetGoldOutEnable(false);

	m_GameClientView.SetDoorDice(0);
	m_GameClientView.SetGoldDice(0);


	return true;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	////��������
	//CGameOption GameOption;
	//GameOption.m_bEnableSound=IsEnableSound();

	////��������
	//if (GameOption.DoModal()==IDOK)
	//{
	//	EnableSound(GameOption.m_bEnableSound);
	//}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return false;
			}
			if (nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//��ʱ�ж�
			if ((nElapse==0)&&(IsLookonMode()==false))
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if (wChairID==wMeChairID)
				{					
					if(m_wCurrentUser==wMeChairID)
					{
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
						OnOutCard(cbCardData,cbCardData);
					}
					else OnCardOperate(WIK_NULL,0);
				}

				return false;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_OPEN_DOOR:
		{

			//��ʱ�ж�
			if((nElapse==0)&&(wChairID==GetMeChairID()))
			{		
				OnOpenDoor(0,0);

				return false;
			}
			return true;
		}

	case IDI_OPEN_GOLD:
		{

			//��ʱ�ж�
			if((nElapse==0)&&(wChairID==GetMeChairID()))
			{

				OnOpenGold(0,0);

				return false;
			}
			return true;
		}
	case IDI_REPAIR_FLOWER://����״̬
		{
			//��ʱ�ж�
			if((nElapse==0)&&(wChairID==GetMeChairID()))
			{
				//ɾ����ʱ��
				KillGameClock(IDI_REPAIR_FLOWER);
				

				//������Ϣ
				CMD_C_RepairFlower RepairFlower;
				RepairFlower.cbCurrentCard = m_cbOutCardData;
				RepairFlower.wChairID = GetMeChairID();

				SendSocketData(SUB_C_REPAIRFLOWER,&RepairFlower,sizeof(RepairFlower));	
				

			}
			return true;
		}
	
	}

	return false;
}

bool CGameClientDlg::OnEventGameClockKill(WORD t)
{
	return true;
}

//�Թ�״̬
bool CGameClientDlg::OnEventLookonMode(void * pBuffer, WORD wDataSize)
{
	//�˿˿���
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//������Ϣ
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			return OnSubOperateResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_OPENDOOR:		//����״̬
		{
			return OnSubOpenDoor(pBuffer,wDataSize);
		}
	case SUB_S_OPENGOLD:		//����״̬
		{
			return OnSubOpenGold(pBuffer,wDataSize);
		}
	case SUB_S_END_FLOWER:		//������ʼ
		{
			return OnSubEndFlower(pBuffer,wDataSize);

		}
	case SUB_S_GOLD_SWIM:		//����״̬
		{
			return OnSubGoldSwim(pBuffer,wDataSize);
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{				
				m_cbHeapCardInfo[i][0]=0;				
				m_cbHeapCardInfo[i][1]=0;
				m_GameClientView.m_InsideHeapCard[SwitchViewChairID(i)].ResetMinusMiddleCount();
				m_GameClientView.m_InsideHeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],INSIDE_HEAP_FULL_COUNT,0);
				m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(i)].ResetMinusMiddleCount();
				m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],OUTSIDE_HEAP_FULL_COUNT,0);
				
			}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
			}

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}	
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;

			//��ʷ����
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));

			//�˿˱���
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//��������
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//����˿�
			BYTE cbWeaveCard[5]={0,0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard);
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);

					if (cbWeaveKind&WIK_GANG)
					{
						if (m_WeaveItemArray[i][j].wProvideUser!=i) m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					}
				}
			}

			//�û��˿�
			if (m_wCurrentUser==GetMeChairID())
			{
				BYTE cbCardCount=pStatusPlay->cbCardCount;
				m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,cbCardCount-1,pStatusPlay->cbCardData[cbCardCount-cbCardCount],m_cbGoldCard,4);
			}
			else m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,0,m_cbGoldCard,4); 

			//�˿�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�û��˿�
				if (i!=GetMeChairID())
				{
					BYTE cbCardCount=17-m_cbWeaveCount[i]*3;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//�����˿�
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(m_cbDiscardCard[i],m_cbDiscardCount[i]);
			}

			//��������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			}

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=0;
			}

			//�ַ��˿�
			BYTE cbTakeCount=136-m_cbLeftCardCount;
			BYTE cbSiceFirst=HIBYTE(pStatusPlay->wSiceCount);
			BYTE cbSiceSecond=LOBYTE(pStatusPlay->wSiceCount);
			WORD wTakeChairID=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//������Ŀ				
				BYTE cbValidCount=INSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceSecond-1)*2:0);
				BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

				//��ȡ�˿�
				cbTakeCount-=cbRemoveCount;
				m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

				//����ж�
				if (cbTakeCount==0)
				{
					m_wHeapHand=wTakeChairID;
					m_wHeapTail=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
					break;
				}

				//�л�����
				wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
			}

			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{				
				m_GameClientView.m_InsideHeapCard[wViewChairID[i]].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],INSIDE_HEAP_FULL_COUNT,0);
			}

			//��ʷ�˿�
			if (m_wOutCardUser!=INVALID_CHAIR)
			{
				WORD wOutChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wOutChairID,m_cbOutCardData);
			}

			//��������
			if (cbGameStation==GS_MJ_PLAY)
			{
				//��������
				if ((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
				{
					//��ȡ����
					BYTE cbActionMask=pStatusPlay->cbActionMask;
					BYTE cbActionCard=pStatusPlay->cbActionCard;

					//��������
					tagGangCardResult GangCardResult;
					ZeroMemory(&GangCardResult,sizeof(GangCardResult));

					//�����ж�

					if ((cbActionMask&WIK_GANG)!=0)
					{
						//�������
						if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
						{
							GangCardResult.cbCardCount=1;
							GangCardResult.cbCardData[0]=cbActionCard;
						}

						//�Լ�����
						if ((m_wCurrentUser==GetMeChairID())||(cbActionCard==0))
						{
							WORD wMeChairID=GetMeChairID();
							m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
						}
					}	


					//���ý���					
					m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
					if (m_wCurrentUser==INVALID_CHAIR)
					{
						if(m_bAuto)
							SetTimer(IDT_AUTO,TIME_AUTO,NULL);
						SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					}
				}

				//����ʱ��
				if (m_wCurrentUser!=INVALID_CHAIR)
				{
					//����ʱ��
					WORD wTimeCount=TIME_OPERATE_CARD;					

					//����ʱ��
					SetGameClock(m_wCurrentUser,TIME_OPERATE_CARD,wTimeCount);
				}
			}

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GS_MJ_BEFOREPLAY:
		{
			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

#ifdef USE_RS_PRINT
    CMD_S_GameStart GameStart;
	memcpy(&GameStart,pGameStart,sizeof(CMD_S_GameStart));
	SYSTEMTIME timeCreateFile={0};	// ������ʱ��
	GetSystemTime(&timeCreateFile);	// ��õ�ǰʱ��

	wstring  strLogFile=L"";
	strLogFile=s2ws(GetAppPath())+L"\\QZMJClient\\NetMsg.txt";
	wstring wstrDirName=s2ws(GetAppPath())+L"\\QZMJClient";
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

	CString strLog;
	strLog.Format(_T("CMD_S_GameStart ׯ���û�wBankerUser=[%d],��ǰ�û�wCurrentUser=[%d]"),GameStart.wBankerUser,GameStart.wCurrentUser);
	outf<<CT2A(strLog)<<endl;
#endif

	//����״̬
	SetGameStatus(GS_MJ_BEFOREPLAY);

	//���ñ���	
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1);

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����˿�
	BYTE cbCardCount = MAX_COUNT-1;
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	m_bAuto=false;
	//m_GameClientView.m_btAutoOn.EnableWindow(true);
	//m_GameClientView.m_btAutoOn.ShowWindow (SW_SHOW);
	//m_GameClientView.m_btAutoOff.EnableWindow(true);
	//m_GameClientView.m_btAutoOff.ShowWindow(SW_HIDE);	

	//�Թ۽���
	if (bPlayerMode==false)
	{
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=0;
	}

	//�ַ��˿�
	BYTE cbTakeCount=(MAX_COUNT-1)*GAME_PLAYER;

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//��Ͻ���
		m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][4].SetDisplayItem(true);
		m_GameClientView.m_InsideHeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],0,0);		
		m_GameClientView.m_OutSideHeapCard[wViewChairID].ResetMinusMiddleCount();
		m_GameClientView.m_OutSideHeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][0],OUTSIDE_HEAP_FULL_COUNT,0);

		//�û��˿�
		if (wViewChairID!=2)
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			
			m_GameClientView.m_UserCard[wIndex].SetCardData(CountArray(pGameStart->cbCardData),0);
		}
		else
		{	
		
			m_GameClientView.m_HandCardControl.SetCardData(pGameStart->cbCardData,MAX_COUNT-1,0,m_cbGoldCard,4);
				
		}

		//�Թ۽���
		if (bPlayerMode==false)
		{
			m_GameClientView.m_TableCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][4].SetCardData(NULL,0);
		}
	}

	//������ʾ
	if ((bPlayerMode!=false)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		WORD wMeChairID=GetMeChairID();
		if (m_wCurrentUser==wMeChairID)
		{
			m_GameClientView.SetStatusFlag(!m_bAuto,false);

			//�ؼ�����
			m_GameClientView.m_btOpenDoor.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOpenDoor.EnableWindow(TRUE);
		}
	}	

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//������
	if (bPlayerMode!=false) ActiveGameFrame();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		SetGameClock(m_wCurrentUser,IDI_OPEN_DOOR,TIME_OPEN_DOOR);
	}

	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;

	//�����û�
	if ((IsLookonMode()!=false)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//��������
		KillGameClock(IDI_OPERATE_CARD); 
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

		//���ƽ���
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

		//�����˿�
		if (wOutViewChairID==2)
		{
			//ɾ���˿�
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);

			//�����˿�
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0,m_cbGoldCard,4);
		}
		else
		{
			WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}		
	}

	if(pOutCard->wOutCardUser==GetMeChairID())
	{
		if(GetGameStatus()==GS_MJ_BEFOREPLAY)
		{
			//ɾ���˿�
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);

			//�����˿�
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0,m_cbGoldCard,4);

		}
	}

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//״̬����
	if(pSendCard->bGameSatusPlay)SetGameStatus(GS_MJ_PLAY);

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);

		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//���Ʊ���
	bool bFlowerCard = false;

	//���ƴ���
	if (pSendCard->cbCardData!=0)
	{
		

		//ȡ�ƽ���
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		if (wViewChairID!=2)
		{
			WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
		}
		else if((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
		{			

			//�ж�״̬
			if(GetGameStatus()!=GS_MJ_PLAY)//����������Ƴ���ʱ�̣�������Ҳ���ʱ��
			{
				
				//��������
				//BYTE cbFlowerCard[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};

				//�ж��Ƿ��ǻ���
				if(m_GameLogic.IsFlowerCard(pSendCard->cbCardData,m_cbFlowerCard,8))
				{	
					//���ñ���
					m_wCurrentUser = pSendCard->wCurrentUser;
					m_cbOutCardData = pSendCard->cbCardData;

					//���ö�ʱ��
					SetTimer(IDT_REPAIR_FLOWER,100,NULL);
					bFlowerCard =  true;
				}

				if(bFlowerCard==false)//��ǰ�齫���ǻ���
				{
					m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;

					//������û�л���
					if(m_GameLogic.IsHaveFlowerCard(m_cbCardIndex,m_cbFlowerCard,8))
					{
						//��ȡ����
						BYTE cbCard = m_GameLogic.GetFlowerCard(m_cbCardIndex,m_cbFlowerCard,8);
						if(cbCard!=0)
						{
							//���ñ���
							m_wCurrentUser = pSendCard->wCurrentUser;
							m_cbOutCardData = cbCard;

							//ɾ���˿�
							m_GameLogic.RemoveCard(m_cbCardIndex,cbCard);

							//���ö�ʱ��
							SetTimer(IDT_REPAIR_FLOWER,100,NULL);
						}
					}
				}
			}
			else //��Ϸ״̬
			{
				//��������
				//BYTE cbFlowerCard[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};

				//�ж��Ƿ��ǻ���				
				if(m_GameLogic.IsFlowerCard(pSendCard->cbCardData,m_cbFlowerCard,8))
				{
					//���ñ���
					m_wOutCardUser = pSendCard->wCurrentUser;
					m_cbOutCardData = pSendCard->cbCardData;

					//���ö�ʱ��
					SetTimer(IDT_REPAIR_FLOWER,100,NULL);
					bFlowerCard = true;
				}

				if(bFlowerCard==false)
				{
					m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;
				}			

			}

			tagCardItem CardItem;
			CardItem.cbCardData = pSendCard->cbCardData;
			CardItem.bShoot = false;
			CardItem.bGoldFlag = m_GameLogic.IsGoldCard(pSendCard->cbCardData,m_cbGoldCard,4);
			m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);

			//m_GameClientView.m_HandCardControl.SetCurrentCard(pSendCard->cbCardData);
			
			


			
		}
		
		
		//�۳��˿�
		if(pSendCard->bRepairFlower)		
			DeductionTableCard(false);
		else
			DeductionTableCard(true);
	}
	BYTE mbHu=WIK_NULL;
	BYTE mbGang=WIK_NULL;

	//��������
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		ActiveGameFrame();


		//��ȡ����
		BYTE cbActionCard=pSendCard->cbCardData;
		BYTE cbActionMask=pSendCard->cbActionMask;
		

		//��������
		if (pSendCard->cbActionMask!=WIK_NULL)
		{
			

			//�����ж�
			if ((cbActionMask&WIK_GANG)!=0)
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}
			

		
			m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);

		}
		m_GameClientView.m_HandCardControl.SetPositively(true);
	}

	if((bFlowerCard==false)&&(GetGameStatus()==GS_MJ_PLAY))//״̬�ж�
	{

		//������ʾ
		m_GameClientView.SetStatusFlag(!m_bAuto && (IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

	

		//����ʱ��
		WORD wTimeCount=TIME_OPERATE_CARD;

		//����ʱ��	
		if(m_bAuto && m_wCurrentUser==GetMeChairID())
			SetTimer(IDT_AUTO,TIME_AUTO,NULL);	
	
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);
	



	return true;
}

//������ʾ
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//�û�����
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//��ȡ����
		BYTE cbActionMask=pOperateNotify->cbActionMask;
		BYTE cbActionCard=pOperateNotify->cbActionCard;

		//��������
		tagGangCardResult GangCardResult;
		ZeroMemory(&GangCardResult,sizeof(GangCardResult));

		//�����ж�
		if ((cbActionMask&WIK_GANG)!=0)
		{
			//�������
			if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
			{
				GangCardResult.cbCardCount=1;
				GangCardResult.cbCardData[0]=cbActionCard;
			}

			//�Լ�����
			if ((m_wCurrentUser==GetMeChairID())||(cbActionCard==0))
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}
		}		
		

		if(m_bAuto)
			SetTimer(IDT_AUTO,TIME_AUTO,NULL);
		

		//���ý���
		ActiveGameFrame();		
		SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		
		
		m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
	}

	return true;
}

//�������
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);

	//���Ʊ���
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//�������
	if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)
	{
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;

		//����˿�
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
				break;
			}
		}

		//����˿�
		if (cbWeaveIndex==0xFF)
		{
			//�����ж�
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;
			
			//�����˿�
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		}

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		bool bShow=(cbPublicCard==TRUE)?true:false;
		if((IsLookonMode()==false)&&(GetMeChairID()==pOperateResult->wProvideUser))bShow=true;
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem(bShow);
		

		//�˿�����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard)]=0;
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0,m_cbGoldCard,4);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
		}
	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		//���ñ���
		m_wCurrentUser=pOperateResult->wOperateUser;

		//�������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);

		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
			m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1],m_cbGoldCard,4);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}
	}

	//���ý���
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	m_GameClientView.SetStatusFlag(!m_bAuto && (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);



	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//����ʱ��
		WORD wTimeCount=TIME_OPERATE_CARD;


		//����ʱ��
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	
	//������
/*	char msg[60];
	memset(msg,0,sizeof(msg));
	char temp_buff[60];
	memset(temp_buff,0,sizeof(temp_buff));*/
	/*for(WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		for(WORD k=0;k<4;k++)
		{
			if(m_WeaveItemArray[i][k].cbWeaveKind==WIK_GANG)
			{
				if(i==m_WeaveItemArray[i][k].wProvideUser)
				{
					sprintf(temp_buff," %s���� ",pUserData->szName);
					strcat(msg,temp_buff);
				}
				else
				{
					sprintf(temp_buff," %s���� ",pUserData->szName);
					strcat(msg,temp_buff);
				}
			}
			else continue;
		}
	}*/

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//����״̬
	SetGameStatus(GS_MJ_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	//ɾ����ʱ��
	KillGameClock(IDI_OPERATE_CARD);

	//���ÿؼ�
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.m_btOpenDoor.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenGold.ShowWindow(SW_HIDE);

	//��������
	if (pGameEnd->cbChiHuCard!=0)
	{
		tagGameScoreInfo GameScoreInfo;
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.cbChiHuCard=pGameEnd->cbChiHuCard;
		GameScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		GameScoreInfo.lCellScore = pGameEnd->lCellScore;
		//strcat(pGameEnd->strEnd,msg);
		CopyMemory(&GameScoreInfo.m_cHu,pGameEnd->strEnd,sizeof(pGameEnd->strEnd) );
		GameScoreInfo.bGoOnBankerTime=pGameEnd->bGoOnBankerTime;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(i);
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
			GameScoreInfo.wCardType[i]=pGameEnd->wChiHuKind[i];
			GameScoreInfo.bFlowerCount[i]=pGameEnd->bFlowerCount[i];
			GameScoreInfo.bGoldCount[i]=pGameEnd->bGoldCount[i];
			GameScoreInfo.bAllPlantCard[i]=pGameEnd->bAllPlantCard[i];
			GameScoreInfo.bAllSeasonCard[i]=pGameEnd->bAllSeasonCard[i];
			GameScoreInfo.bAllSidesCard[i]=pGameEnd->bAllSidesCard[i];
			GameScoreInfo.bGangCount[i]=pGameEnd->bGangCount[i];
			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->GetNickName(),CountArray(GameScoreInfo.szUserName[i]));
		}
		m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);
		
	}
	else 
	{
		DeductionTableCard(true);
		//test
		tagGameScoreInfo GameScoreInfo;
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.lCellScore = pGameEnd->lCellScore;
		GameScoreInfo.cbChiHuCard=NULL;
		GameScoreInfo.wProvideUser=6;
		//strcat(pGameEnd->strEnd,msg);
		CopyMemory(&GameScoreInfo.m_cHu,pGameEnd->strEnd,sizeof(pGameEnd->strEnd) );
		GameScoreInfo.bGoOnBankerTime=pGameEnd->bGoOnBankerTime;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(i);
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
			GameScoreInfo.wCardType[i]=NULL;
			GameScoreInfo.bFlowerCount[i] = pGameEnd->bFlowerCount[i];
			GameScoreInfo.bGoldCount[i] = pGameEnd->bGoldCount[i];
			GameScoreInfo.bAllPlantCard[i]=pGameEnd->bAllPlantCard[i];
			GameScoreInfo.bAllSeasonCard[i]=pGameEnd->bAllSeasonCard[i];
			GameScoreInfo.bAllSidesCard[i]=pGameEnd->bAllSidesCard[i];
			GameScoreInfo.bGangCount[i]=pGameEnd->bGangCount[i];
			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->GetNickName(),CountArray(GameScoreInfo.szUserName[i]));
		}
		m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);

		m_GameClientView.SetHuangZhuang(true);
	}

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->wChiHuKind[i]!=CHK_NULL) m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0,NULL,0);

	//��������
	LONG lScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}
	m_bAuto=false;
	//m_GameClientView.m_btAutoOn.EnableWindow(false);
	//m_GameClientView.m_btAutoOff.EnableWindow(false);
	//m_GameClientView.m_btAutoOn.ShowWindow (SW_HIDE);
	//m_GameClientView.m_btAutoOff.ShowWindow (SW_HIDE);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//�۳��˿�
void CGameClientDlg::DeductionTableCard(bool bHeadCard)
{
	if (bHeadCard==true)
	{		

		//�ж�ׯ�Ҷ����˿�
		if((m_wHeapHand==m_wBankerUser)&&((OUTSIDE_HEAP_FULL_COUNT-m_wDoorDiceCount+1)>=m_cbSubtractHeadCount))
		{
			//�����˿�
			m_cbLeftCardCount--;			

			//�˿˿�ȱ
			WORD wMinusMiddlePos =  OUTSIDE_HEAP_FULL_COUNT-m_wDoorDiceCount-m_cbSubtractHeadCount;

			//�����˿�
			WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
			WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
			WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];

			m_GameClientView.m_OutSideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,OUTSIDE_HEAP_FULL_COUNT,wMinusMiddlePos);

			//���ñ���
			m_cbSubtractHeadCount++;
		}
		else//��ׯ�Ҷ����˿�
		{
			if(m_wHeapHand==m_wBankerUser)
			{
				//�л�����
				m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);

			}
			else
			{
				//�л�����		
				BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
				if (cbHeapCount==OUTSIDE_HEAP_FULL_COUNT) m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);
			}
			//if(SwitchViewChairID(m_wHeapHand)==0)		

			//�����˿�
			m_cbLeftCardCount--;			
			m_cbHeapCardInfo[m_wHeapHand][0]++;

			//�����˿�
			WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
			WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
			WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];			
			m_GameClientView.m_OutSideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,OUTSIDE_HEAP_FULL_COUNT,0);
		}

		
		
	}
	else
	{

		//���ñ���
		m_cbSubtractTailCount++;


		//���������齫
		if(m_cbSubtractTailCount==m_wGoldDiceCount)m_cbSubtractTailCount++;

		//�����˿�
		m_cbLeftCardCount--;

		//�ж�ׯ�Ҷ����˿�
		if((m_wHeapTail==m_wBankerUser)&&(m_cbSubtractTailCount<=m_wDoorDiceCount))
		{
			//�˿˿�ȱ
			WORD wMinusMiddlePos =  OUTSIDE_HEAP_FULL_COUNT-m_wDoorDiceCount+m_cbSubtractTailCount;

			//�����˿�
			WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
			WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
			WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];

			m_GameClientView.m_OutSideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,OUTSIDE_HEAP_FULL_COUNT,wMinusMiddlePos);
		}
		else//��ׯ�Ҷ����˿�
		{
			if(m_wHeapTail==m_wBankerUser)
			{
				//�л�����
				m_wHeapTail = (m_wBankerUser+3)%CountArray(m_cbHeapCardInfo);
			}
			else
			{
				//�л�����		
				BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
				if (cbHeapCount==OUTSIDE_HEAP_FULL_COUNT) m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);	
			}		
					
		
			//�����˿�	
			m_cbHeapCardInfo[m_wHeapTail][1]++;
			if(m_cbSubtractTailCount==m_wDoorDiceCount)m_cbHeapCardInfo[m_wHeapTail][1]++;
			WORD wHeapViewID=SwitchViewChairID(m_wHeapTail);
			WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapTail][0];
			WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapTail][1];		
			//m_GameClientView.m_InsideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,INSIDE_HEAP_FULL_COUNT);
			m_GameClientView.m_OutSideHeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,OUTSIDE_HEAP_FULL_COUNT,0);
	
		}
	}

	return;
}

//��ʾ����
bool CGameClientDlg::ShowOperateControl(BYTE cbUserAction, BYTE cbActionCard)
{
	//��������
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//�����ж�
	if ((cbUserAction&WIK_GANG)!=0)
	{
		//�������
		if (cbActionCard!=0)
		{
			GangCardResult.cbCardCount=1;
			GangCardResult.cbCardData[0]=cbActionCard;
		}

		//�Լ�����
		if (cbActionCard==0)
		{
			WORD wMeChairID=GetMeChairID();
			m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
		}
	}

	
	//��ʾ����
	m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbUserAction,GangCardResult);

	return true;
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//��������
	KillGameClock(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetGoldCardShow(false);

	m_GameClientView.SetDoorDice(0);
	m_GameClientView.SetGoldDice(0);	


	//���ý���
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);


	//�˿�����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0,NULL,0);
	m_GameClientView.m_GoldCardControl.SetCardData(NULL,0,0,NULL,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][0].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][1].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][2].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][3].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][4].SetCardData(NULL,0);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		
		BYTE bHeapFullCount=0;
		m_cbHeapCardInfo[i][0]=0;			
		m_cbHeapCardInfo[i][1]=0;
		m_GameClientView.m_InsideHeapCard[SwitchViewChairID(i)].ResetMinusMiddleCount();
		m_GameClientView.m_InsideHeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],INSIDE_HEAP_FULL_COUNT,0);
		m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(i)].ResetMinusMiddleCount();
		m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],OUTSIDE_HEAP_FULL_COUNT,0);
	}

	//״̬����
	m_wDoorDiceCount = 0;
	m_wGoldDiceCount = 0;
	m_cbSubtractTailCount = 0;
	m_cbSubtractHeadCount = 0;

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	ZeroMemory(m_cbGoldCard,sizeof(m_cbGoldCard));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����˿�
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbFlowerCard,sizeof(m_cbFlowerCard));

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()!=false)||(m_wCurrentUser!=GetMeChairID())) return 0;
	

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	BYTE cbOutCardData=(BYTE)wParam;
	m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData);

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0,m_cbGoldCard,4);

	//���ý���
	KillGameClock(IDI_OPERATE_CARD);
	m_GameClientView.InvalidGameView(0,0,0,0);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);	

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	return 0;
}

//�˿˲���
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard=(BYTE)(LOWORD(lParam));

	//״̬�ж�
	if ((cbOperateCode==WIK_NULL)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
		return 0;
	}

	//��������
	KillGameClock(IDI_OPERATE_CARD);
	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

LRESULT CGameClientDlg::OnAuto(WPARAM wParam, LPARAM lParam)
{
	if(wParam!=0)
	{
		m_bAuto=true;
		if(GetMeChairID()==m_wCurrentUser)
			OnEventGameClockInfo(GetMeChairID(), 0, IDI_OPERATE_CARD);
	}
	else
	{
		m_bAuto=false;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////

void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDT_AUTO:
		KillTimer(IDT_AUTO);
		OnEventGameClockInfo(GetMeChairID(), 0, IDI_OPERATE_CARD);
		CGameFrameEngine::OnTimer(nIDEvent);
		break;	

	case IDT_REPAIR_FLOWER:
		KillTimer(IDT_REPAIR_FLOWER);
		OnEventGameClockInfo(GetMeChairID(), 0, IDI_REPAIR_FLOWER);
		CGameFrameEngine::OnTimer(nIDEvent);
		break;
	
	}
	return;
}

//���Ų���
LRESULT CGameClientDlg::OnOpenDoor(WPARAM wParam,LPARAM lParam)
{

	//�ؼ�����
	m_GameClientView.m_btOpenDoor.ShowWindow(SW_HIDE);

	//ɾ����ʱ��
	KillGameClock(IDI_OPEN_DOOR);

	SendSocketData(SUB_C_OPENDOOR);

	return 0;
}

//�������
LRESULT CGameClientDlg::OnOpenGold(WPARAM wParam,LPARAM lParam)
{

	//�ؼ�����
	m_GameClientView.m_btOpenGold.ShowWindow(SW_HIDE);

	//ɾ����ʱ��
	KillGameClock(IDI_OPEN_GOLD);

	SendSocketData(SUB_C_OPENGOLD);

	return 0;
}



// ����״̬
bool CGameClientDlg::OnSubOpenDoor(const void * pBuffer, WORD wDataSize)
{

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OpenDoor));
	if(wDataSize!=sizeof(CMD_S_OpenDoor))return false;

	//ת������
	CMD_S_OpenDoor *pOpenDoor = (CMD_S_OpenDoor *)pBuffer;


	//��������
	m_wDoorDiceCount = pOpenDoor->wDiceCount;	
	

	//�ؼ�����
	if(GetMeChairID()==pOpenDoor->wOpenGoldUser)
	{
		m_GameClientView.m_btOpenGold.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOpenGold.EnableWindow(TRUE);
	}


	//��������λ��
	BYTE cbDoorCardPos = OUTSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[m_wBankerUser][1]-m_wDoorDiceCount;

	//��������
	m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(m_wBankerUser)].SetCardData(m_cbHeapCardInfo[m_wBankerUser][0],m_cbHeapCardInfo[m_wBankerUser][1],OUTSIDE_HEAP_FULL_COUNT,cbDoorCardPos);
	m_GameClientView.SetDoorDice(m_wDoorDiceCount);

	//�����˿�
	WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
	if (wViewChairID!=2)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
	}
	else if((IsLookonMode()==false)&&(m_wBankerUser==GetMeChairID()))
	{			
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOpenDoor->cbDoorCard)]++;
		m_GameClientView.m_HandCardControl.SetCurrentCard(pOpenDoor->cbDoorCard);
		
	}

	//ʱ������
	SetGameClock(pOpenDoor->wOpenGoldUser,IDI_OPEN_GOLD,TIME_OPEN_GOLD);
	return true;
}

// ����״̬
bool CGameClientDlg::OnSubOpenGold(const void * pBuffer, WORD wDataSize)
{

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OpenGold));
	if(wDataSize!=sizeof(CMD_S_OpenGold))return false;

	//ת������
	CMD_S_OpenGold *pOpenGold = (CMD_S_OpenGold *)pBuffer;

	//���ñ���
	CopyMemory(m_cbGoldCard,pOpenGold->cbGoldCard,sizeof(m_cbGoldCard));
	CopyMemory(m_cbFlowerCard,pOpenGold->cbFlowerCard,sizeof(m_cbFlowerCard));
	m_wGoldDiceCount = pOpenGold->wDiceCount;
	m_wHeapHand = m_wBankerUser;
	m_wHeapTail = m_wBankerUser;

	//�ؼ�����
	BYTE cbGoldCard[2];
	ZeroMemory(cbGoldCard,sizeof(cbGoldCard));

	cbGoldCard[0] = pOpenGold->cbKeyGoldCard;
	m_GameClientView.m_GoldCardControl.SetCardData(cbGoldCard,1,0,NULL,0);
	m_GameClientView.m_GoldCardControl.SetDisplayItem(true);
	m_GameClientView.SetGoldCardShow(true);
	m_GameClientView.SetGoldDice(m_wGoldDiceCount);
	

	//����λ��
	BYTE cbGoldCardPos;

	//������ƽ���λ��
	if(m_wGoldDiceCount<=m_wDoorDiceCount)//���Ƴ�����ׯ�ҷ�
	{
		cbGoldCardPos = OUTSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[m_wBankerUser][1]-m_wDoorDiceCount+m_wGoldDiceCount;

		//��������
		m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(m_wBankerUser)].SetCardData(m_cbHeapCardInfo[m_wBankerUser][0],m_cbHeapCardInfo[m_wBankerUser][1],OUTSIDE_HEAP_FULL_COUNT,cbGoldCardPos);
	}
	else//���Ʋ�������ׯ�ҷ�
	{
		//�����˿�λ��
		WORD wChairID = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;

		//cbGoldCardPos = OUTSIDE_HEAP_FULL_COUNT-m_cbHeapCardInfo[wChairID][1]-(m_wGoldDiceCount-m_wDoorDiceCount);
		cbGoldCardPos = (m_wGoldDiceCount-m_wDoorDiceCount)%OUTSIDE_HEAP_FULL_COUNT;

		//��������
		m_GameClientView.m_OutSideHeapCard[SwitchViewChairID(wChairID)].SetCardData(m_cbHeapCardInfo[wChairID][0],m_cbHeapCardInfo[wChairID][1],OUTSIDE_HEAP_FULL_COUNT,cbGoldCardPos);
	}

	//�����˿�
	m_cbLeftCardCount--;

	return true;
}

// ��������
bool CGameClientDlg::OnSubEndFlower(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if(wDataSize!=sizeof(CMD_S_SendCard))return false;

	//����״̬
	SetGameStatus(GS_MJ_PLAY);

	//���ñ���
	m_wCurrentUser = m_wBankerUser;

	//��ȡ��Ϣ
	CMD_S_SendCard *pSendCard = (CMD_S_SendCard*)pBuffer;

	BYTE mbHu=WIK_NULL;
	BYTE mbGang=WIK_NULL;

	//��������
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//������
		ActiveGameFrame();


		//��ȡ����
		BYTE cbActionCard=pSendCard->cbCardData;
		BYTE cbActionMask=pSendCard->cbActionMask;
		

		//��������
		if (pSendCard->cbActionMask!=WIK_NULL)
		{
			//�����ж�
			if ((cbActionMask&WIK_GANG)!=0)
			{
				WORD wMeChairID=GetMeChairID();
				m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			}
		
			m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);

		}	

		//������ʾ
		//m_GameLogic.RemoveCard(m_cbCardIndex,pSendCard->cbCardData);
		//m_GameClientView.m_HandCardControl.SetCardData(m_cbCardIndex,MAX_COUNT-1,0,m_cbGoldCard,4);
		/*tagCardItem CardItem;
		CardItem.bGoldFlag=m_GameLogic.IsGoldCard(pSendCard->cbCardData,m_cbGoldCard,4);
		CardItem.bShoot=false;
		CardItem.cbCardData=pSendCard->cbCardData;
		m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);*/

		BYTE cbCardData[MAX_COUNT];
		BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
		m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1],m_cbGoldCard,4);
		

		
	}

	m_GameClientView.m_HandCardControl.SetPositively(true);
	//������ʾ
	m_GameClientView.SetStatusFlag(!m_bAuto && (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);

	

	//����ʱ��
	WORD wTimeCount=TIME_OPERATE_CARD;

	//����ʱ��	
	if(m_bAuto && m_wCurrentUser==GetMeChairID())
		SetTimer(IDT_AUTO,TIME_AUTO,NULL);	
	
	SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);


	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

// ����״̬
bool CGameClientDlg::OnSubGoldSwim(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_GoldSwim));
	if(wDataSize!=sizeof(CMD_S_GoldSwim))return false;

	//ת������
	CMD_S_GoldSwim *pGoldSwim = (CMD_S_GoldSwim *)pBuffer;

	//������Ϣ
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		//˫�λ�����
		if((GetMeChairID()==i)&&((pGoldSwim->GoldSwim[i]==GoldSwim_Double)||(pGoldSwim->GoldSwim[i]==GoldSwim_Three)))
		{
			//����ʹ��
			m_GameClientView.SetGoldOutEnable(true);
		}
	}
	return true;
}
