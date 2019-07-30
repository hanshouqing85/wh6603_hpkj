#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			201									//������ʱ��
#define IDI_REPLACE_CARD			301									//���ƶ�ʱ��

//��Ϸ��ʱ��
#ifdef _DEBUG
#define TIME_START_GAME				300									//��ʼ��ʱ��
#define TIME_OPERATE_CARD			300									//������ʱ��
#define TIME_REPLACE_CARD			500									//���ƶ�ʱ��
#else
#define TIME_START_GAME				30									//��ʼ��ʱ��
#define TIME_OPERATE_CARD			20									//������ʱ��
#define TIME_REPLACE_CARD			1000								//���ƶ�ʱ��
#endif

#define MAX_TIME_OUT				3									//���ʱ����

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

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_SICE_TWO,OnSiceTwo)
	ON_MESSAGE(IDM_SICE_FINISH,OnSiceFinish)
	ON_MESSAGE(IDM_CAI_SHEN_SICE_FINISH,OnCaiShenSiceFinish)
	ON_MESSAGE(IDM_LAND_SICE_FINISH,OnLandSiceFinish)
	ON_MESSAGE(IDM_USER_ACTION,OnUserAction)
	ON_MESSAGE(IDM_MOVE_CARD_FINISH,OnMoveCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg()
{
	//��Ϸ����
	//g_cbChangeCard=0;
	g_CardResource.m_cbChangeCard=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbListenStatus = 0;
	m_bWillHearStatus=false;
	m_lSiceCount = 0;
	m_wCaiShenSice=0;				
	m_wCaiShenIndex=0;				
	m_wCaiShenChairID=INVALID_CHAIR;
	m_wFirstUser=INVALID_CHAIR;		
	m_wLandSice=0;					
	m_cbLandCount=1;					

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory( m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::OnInitGameEngine()
{
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	g_CardResource.LoadResource();

	//��������
	m_DirectSound[0].Create( TEXT("OUT_CARD") );
	m_DirectSound[1].Create( TEXT("SEND_CARD") );
	m_DirectSound[2].Create( TEXT("GAME_START") );

	//test
	//BYTE cbCardData[] = {
	//	0x33,0x33,
	//	0x31,0x31,
	//	0x31,0x31,
	//	0x34,0x37,
	//	0x35,0x37,
	//	0x11,0x11,0x37,
	//		//0x13,0x12,0x16,0x15,0x22,0x23,0x25,0x26,0x05,
	//		//0x03,0x03,0x03,0x03
	//};
	//BYTE cbCardIndex[MAX_INDEX]={};
	//m_GameLogic.SwitchToCardIndex(cbCardData,sizeof(cbCardData),cbCardIndex);
	//BYTE cbValue=m_GameLogic.IsQiXiaoDui(cbCardIndex,NULL,0,0x11);
	//if(cbValue)
	//{
	//	CString str;
	//	str.Format(TEXT("%ld"),cbValue);
	//	MessageBox(str);
	//	PostMessage(WM_CLOSE,0,0);
	//}


	//BYTE cbCardCount = 13;
	//BYTE cbCurrentCard = 0x06;


	//BYTE cbCardIndex[MAX_INDEX];
	//m_GameLogic.SwitchToCardIndex( cbCardData,cbCardCount,cbCardIndex );

	//CChiHuRight chr;
	//DWORD dw1 = GetTickCount();
	//BYTE byResult = m_GameLogic.AnalyseChiHuCard( cbCardIndex,NULL,0,cbCurrentCard,chr );
	//DWORD dw2 = GetTickCount();
	//DWORD dw = dw2-dw1;
	//TCHAR szBuffer[32] = TEXT("");
	//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),dw );
	//MessageBox( szBuffer );
	//end test

	return true;
}

//���ÿ��
bool CGameClientDlg::OnResetGameEngine()
{
	//��Ϸ����
	//g_cbChangeCard=0;
	g_CardResource.m_cbChangeCard=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbListenStatus = 0;
	m_bWillHearStatus=false;
	m_lSiceCount = 0;
	m_wCaiShenSice=0;				
	m_wCaiShenIndex=0;				
	m_wCaiShenChairID=INVALID_CHAIR;
	m_wFirstUser=INVALID_CHAIR;		
	m_wLandSice=0;					
	m_cbLandCount=1;	
	
	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	return true;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
//	GameOption.m_bEnableSound=IsEnableSound();
	//GameOption.m_bAllowLookon = IsAllowUserLookon();
	GameOption.m_bEanbleAnimate = m_GameClientView.IsEnableAnimate();
	
	//��������
	if (GameOption.DoModal()==IDOK)
	{
//		EnableSound(GameOption.m_bEnableSound);
	//	AllowUserLookon(0,GameOption.m_bAllowLookon);
		m_GameClientView.EnableAnimate(GameOption.m_bEanbleAnimate);
		m_DirectSound[0].EnableSound(GameOption.m_bEnableSound);
		m_DirectSound[1].EnableSound(GameOption.m_bEnableSound);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if( m_bStustee && nElapse < TIME_START_GAME )
			{
				OnStart(0,0);
				return true;
			}
			if (nElapse==0)
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return true;
			}
			if (nElapse<=5)
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//�Զ�����
			bool bAutoOutCard=false;
			if ((bAutoOutCard==true)&&(m_GameClientView.m_ControlWnd.IsWindowVisible()))
				bAutoOutCard=false;
			if((bAutoOutCard==false)&&(m_bStustee==true))
			{
				bAutoOutCard=true;
			}

			//��ʱ�ж�
			if ( (IsLookonMode()==false)&&
				( nElapse==0 || bAutoOutCard || (m_cbListenStatus&&!m_bWillHearStatus&&nElapse<TIME_OPERATE_CARD) ) )
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if (wChairID==wMeChairID)
				{
					//���δ����,��δ�й�,���ۼӳ�ʱ����
					if( m_cbListenStatus == 0 && m_bStustee==false && ++m_wTimeOutCount>=MAX_TIME_OUT )
					{
						m_wTimeOutCount = 0;
						OnStusteeControl(TRUE,0);
//						m_pIStringMessage->InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
					}

					if (m_wCurrentUser==wMeChairID)
					{
						//��ȡ�˿�
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();

						//����״̬
						if( m_cbListenStatus >= 2 )
						{
							if( m_cbActionMask & WIK_CHI_HU )
							{
								OnUserAction( WIK_CHI_HU,0 );
								return true;
							}
						}
						//����ڲ���ģʽ��
						if( m_GameClientView.m_HandCardControl.GetSelectMode() )
						{
							OnCardOperate(0,0);
							return true;
						}
						//����Ч��
						if (VerdictOutCard(cbCardData)==false)
						{
							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								//����Ч��
								if (m_cbCardIndex[i]==0) continue;
								if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false) 
									continue;

								//���ñ���
								cbCardData=m_GameLogic.SwitchToCardData(i);
							}
						}

						//���ƶ���
						ASSERT( m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData)] > 0 );
						OnOutCard(cbCardData,cbCardData);
					}
					else 
					{
						//����״̬,���ڻ������͵���
						if( m_cbListenStatus == 1 || m_cbListenStatus == 3 )
						{
							if( m_cbActionMask & WIK_CHI_HU )
							{
								OnUserAction( WIK_CHI_HU,0 );
								return true;
							}
						}
						//����ڲ���ģʽ��
						if( m_GameClientView.m_HandCardControl.GetSelectMode() )
						{
							OnCardOperate(0,0);
							return true;
						}
						OnUserAction(WIK_NULL,0);
					}
				}

				return true;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}

	return true;
}

//�Թ�״̬
bool CGameClientDlg::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//�˿˿���
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//������Ϣ
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	//ֹͣɫ�Ӷ���
	if(SUB_S_GAME_END==wSubCmdID)SetGameStatus(GS_MJ_FREE);
	m_GameClientView.m_DrawSiceWnd.StopSicing();
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
			StopMoveCard();
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			StopMoveCard();
			return OnSubOperateResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			StopMoveCard();
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return OnSubTrustee(pBuffer,wDataSize);
		}
	case SUB_S_LISTEN:			//�������
		{
			return OnSubListen( pBuffer,wDataSize );
		}
	case SUB_S_REPLACE_CARD:	//����
		{
			return OnSubReplaceCard( pBuffer,wDataSize );
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//�Ƿ���������
//			if( !IsEnableSound() )
			{
				m_DirectSound[0].EnableSound(FALSE);
				m_DirectSound[1].EnableSound(FALSE);
				m_DirectSound[2].EnableSound(FALSE);
			}

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
			}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}

			//����Ч��
			m_GameClientView.SetDiscUser(INVALID_CHAIR);

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//�Ƿ���������
//			if( !IsEnableSound() )
			{
				m_DirectSound[0].EnableSound(FALSE);
				m_DirectSound[1].EnableSound(FALSE);
				m_DirectSound[2].EnableSound(FALSE);
			}

			//��������
			WORD wMeChairID=GetMeChairID();
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//���ñ���
			//g_cbChangeCard=pStatusPlay->cbChangeCard;
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount = pStatusPlay->cbLeftCardCount;
			m_cbListenStatus = pStatusPlay->cbHearStatus[wMeChairID];
			m_wReplaceUser = pStatusPlay->wReplaceUser;
			m_wHeapHead = pStatusPlay->wHeapHead;
			m_wHeapTail = pStatusPlay->wHeapTail;
			m_cbLandCount = pStatusPlay->cbLandCount ;
			g_CardResource.m_cbChangeCard = pStatusPlay->cbChangeCard;
			m_GameClientView.SetLandCount(m_cbLandCount);

			//����λ��
			m_wCaiShenIndex = pStatusPlay->wCaiShenIndex;
			m_wCaiShenChairID = pStatusPlay->wCaiShenChairID;
			m_GameClientView.m_HeapCard[SwitchViewChairID(m_wCaiShenChairID)].SetCardData(m_wCaiShenIndex*2,g_CardResource.m_cbChangeCard);

			//�û�����
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				
				IClientUserItem * pUserData = GetTableUserItem(i);
				ASSERT( pUserData != NULL );
				if( pUserData == NULL ) 
				{
					m_szAccounts[i][0] = 0;
					continue;
				}
				lstrcpy( m_szAccounts[i],pUserData->GetNickName() );
			}

			//�Թ�
			if( IsLookonMode()==true )
				m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());

			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusPlay->bTrustee[i]);
			}
			
			if( pStatusPlay->bTrustee[wMeChairID] )
			{
				OnStusteeControl(0,0);
			}

			m_wTimeOutCount=0;
			if(pStatusPlay->bTrustee[wMeChairID])
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);

			//�˿˱���
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wOperateViewID = SwitchViewChairID(i);
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(m_WeaveItemArray[i][j].cbCardData,cbWeaveCardCount);
					if ((cbWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i)) 
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
					m_GameClientView.m_WeaveCard[wOperateViewID][j].SetDirectionCardPos((BYTE)wProviderViewID);

				}
			}

			//�û��˿�
			if (m_wCurrentUser==wMeChairID)
			{
				SetHandCardControl(m_cbCardIndex,pStatusPlay->cbSendCardData);
			}
			else
			{
				SetHandCardControl(m_cbCardIndex,0x00);
			}

			//�˿�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�û��˿�
				if (i!=wMeChairID)
				{
					BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[i]*3-1;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//�����˿�
				m_GameClientView.m_DiscardCard[wViewChairID[i]].SetCardData(pStatusPlay->cbDiscardCard[i],pStatusPlay->cbDiscardCount[i]);
			}

			//����Ч��
			if(pStatusPlay->wOutCardUser != INVALID_CHAIR)
			{
				m_GameClientView.SetDiscUser(SwitchViewChairID(pStatusPlay->wOutCardUser));
				m_GameClientView.SetOutCardInfo( wViewChairID[pStatusPlay->wOutCardUser],pStatusPlay->cbOutCardData );
				m_GameClientView.m_DiscardCard[wViewChairID[pStatusPlay->wOutCardUser]].AddCardItem( pStatusPlay->cbOutCardData );
			}

			//��������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			}

			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_HeapCard[wViewChairID[i]].SetHeapCardInfo(pStatusPlay->cbHeapCardInfo[i][0],pStatusPlay->cbHeapCardInfo[i][1]);
			}

			//����״̬
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( pStatusPlay->cbHearStatus[i] > 0 )
					m_GameClientView.SetUserListenStatus( wViewChairID[i],true );
			}

			//��������
			if ((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
			{
				//��ȡ����
				m_cbActionMask=pStatusPlay->cbActionMask;
				m_cbActionCard=pStatusPlay->cbActionCard;

				//���ý���
				if (m_wCurrentUser==INVALID_CHAIR)
					SetGameClock(wMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				if (IsLookonMode()==false) 
					m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
			}

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR)
			{
				//����ʱ��
				WORD wTimeCount=TIME_OPERATE_CARD;

				//����ʱ��
				SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
			}
			else if( m_wReplaceUser == wMeChairID && !IsLookonMode() )
				SetTimer( IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL );

			//����Ȧ��
			m_GameClientView.SetFengQuan( pStatusPlay->cbQuanFeng );

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);

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
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

#ifdef USE_RS_PRINT
    CMD_S_GameStart GameStart;
	memcpy(&GameStart,pGameStart,sizeof(CMD_S_GameStart));
	SYSTEMTIME timeCreateFile={0};	// ������ʱ��
	GetSystemTime(&timeCreateFile);	// ��õ�ǰʱ��

	wstring  strLogFile=L"";
	strLogFile=s2ws(GetAppPath())+L"\\HZMJClient\\NetMsg.txt";
	wstring wstrDirName=s2ws(GetAppPath())+L"\\HZMJClient";
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
	strLog.Format(_T("CMD_S_GameStart ���ӵ���lSiceCount=[%d],ׯ���û�wBankerUser=[%d],��ǰ�û�wCurrentUser=[%d],�����û�wReplaceUser=[%d],Ȧ��cbQuanFeng=[%d],������ͷwHeapHead=[%d],������βwHeapTail=[%d]"),GameStart.lSiceCount,GameStart.wBankerUser,GameStart.wCurrentUser,GameStart.wReplaceUser,GameStart.cbQuanFeng,GameStart.wHeapHead,GameStart.wHeapTail);
	outf<<CT2A(strLog)<<endl;
#endif

	//�û�����
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		IClientUserItem * pUserData = GetTableUserItem( i );
		ASSERT( pUserData != NULL );
		if( pUserData == NULL ) 
		{
			m_szAccounts[i][0] = 0;
			continue;
		}
		lstrcpy( m_szAccounts[i],pUserData->GetNickName() );
	}

	//����״̬
	SetGameStatus(GS_MJ_PLAY);

	//���ñ���
	m_wBankerUser = pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_wReplaceUser = pGameStart->wReplaceUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	m_cbActionMask = pGameStart->cbUserAction;
	m_lSiceCount = pGameStart->lSiceCount;
	m_wCaiShenSice = pGameStart->wCaiShenSice;
	m_wCaiShenIndex = pGameStart->wCaiShenIndex;
	m_wCaiShenChairID = pGameStart->wCaiShenChairID;
	m_wFirstUser = pGameStart->wFirstUser ;
	m_wLandSice = pGameStart->wLandSice ;
	g_CardResource.m_cbChangeCard = pGameStart->cbChangeCard ;
	m_cbLandCount = pGameStart->cbLandCount ;

	//�����˿�
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//���ö���ͷβ
	m_wHeapHead = pGameStart->wHeapHead;
	m_wHeapTail = pGameStart->wHeapTail;
	CopyMemory( m_cbHeapCardInfo,pGameStart->cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );

	//�Թ۽���
	if (IsLookonMode()==true)
	{
		//���ñ���
		m_cbListenStatus = 0;
		m_bWillHearStatus = false;

		//����˿�
		ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
		ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

		m_GameClientView.m_ScoreControl[0].RestorationData();
		m_GameClientView.m_ScoreControl[1].RestorationData();
		m_GameClientView.m_ScoreControl[2].RestorationData();
		m_GameClientView.m_ScoreControl[3].RestorationData();

		m_GameClientView.SetCurrentUser(INVALID_CHAIR);

		//���ý���
		m_GameClientView.SetDiscUser(INVALID_CHAIR);
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetBankerUser(INVALID_CHAIR);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);

		//�˿�����
		m_GameClientView.m_UserCard[0].SetCardData(0,false);
		m_GameClientView.m_UserCard[1].SetCardData(0,false);
		m_GameClientView.m_UserCard[2].SetCardData(0,false);
		m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

		//�˿�����
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
			for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
				m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
		}

		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[i].ResetCardData();
			m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
		}
	}

	//���ý���
	m_GameClientView.SetFengQuan( pGameStart->cbQuanFeng );
	if( m_wFirstUser == INVALID_CHAIR )
		m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//���ſ�ʼ����
	m_DirectSound[2].Play();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//ҡɫ�Ӷ���
	if( m_wFirstUser != INVALID_CHAIR )
	{
		m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,0,(WORD)(m_lSiceCount>>16)/*m_wLandSice*/,1);
		m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);
		//m_GameClientView.m_DrawSiceWnd.StopSicing();
	}
	else
	{
		m_GameClientView.SetLandCount(m_cbLandCount);

		//ҡɫ�Ӷ���
		m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,0,(WORD)(m_lSiceCount>>16)/*m_wLandSice*/,1);
		//m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,(WORD)(m_lSiceCount>>16),(WORD)m_lSiceCount);
		m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);
		//m_GameClientView.m_DrawSiceWnd.StopSicing();
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

	if( IsLookonMode() || pOutCard->wOutCardUser != GetMeChairID() )
	{
		COutCardItem *pOutCardItem = NULL;
		try
		{
			//����������Ŀ
			pOutCardItem = new COutCardItem(pOutCard);
		}
		catch(...)
		{
			ASSERT(FALSE);
			return false;
		}

		//��Ӷ���
		m_MoveCardItemArray.Add(pOutCardItem);

		//���Ŷ���
		BeginMoveCard();
	}

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��Ϣ����
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	CSendCardItem *pSendCardItem = NULL;
	try
	{
		//����������Ŀ
		pSendCardItem = new CSendCardItem(pSendCard);
	}
	catch(...)
	{
		ASSERT(FALSE);
		return false;
	}

	//��Ӷ���
	m_MoveCardItemArray.Add(pSendCardItem);

	//���Ŷ���
	BeginMoveCard();

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubReplaceCard(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_ReplaceCard));
	if (wDataSize!=sizeof(CMD_S_ReplaceCard)) return false;

	//��������
	CMD_S_ReplaceCard * pReplaceCard=(CMD_S_ReplaceCard *)pBuffer;

	if( IsLookonMode() || pReplaceCard->wReplaceUser != GetMeChairID() )
	{
		CReplaceCardItem *pReplaceCardItem = NULL;
		try
		{
			//����������Ŀ
			pReplaceCardItem = new CReplaceCardItem(pReplaceCard);
		}
		catch(...)
		{
			ASSERT(FALSE);
			return false;
		}

		//��Ӷ���
		m_MoveCardItemArray.Add(pReplaceCardItem);

		//���Ŷ���
		BeginMoveCard();
	}

	return true;
}

//������ʾ
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//�û�����
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//��ȡ����
		WORD wMeChairID=GetMeChairID();
		m_cbActionMask = pOperateNotify->cbActionMask;
		m_cbActionCard = pOperateNotify->cbActionCard;

		//������������ڲ���ģʽ,��ȡ��
		if( m_GameClientView.m_HandCardControl.GetSelectMode() )
			m_GameClientView.m_HandCardControl.SetSelectMode(false);

		//���ý���
		ActiveGameFrame();
		m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);

		//����ʱ��
		m_GameClientView.SetCurrentUser(INVALID_CHAIR);
		SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	return true;
}

//�������
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard[3];
	CopyMemory( cbOperateCard,pOperateResult->cbOperateCard,sizeof(cbOperateCard) );
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);
	WORD wProviderViewID = SwitchViewChairID(pOperateResult->wProvideUser);

	//���ñ���
	m_wCurrentUser=pOperateResult->wOperateUser;

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
			if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[3] = cbCenterCard;
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
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
			for( BYTE i = 0; i < 4; i++ ) m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[i] = cbOperateCard[0];

			m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);
		}

		//��Ͻ���
		BYTE cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=4;
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,
			cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicCard==TRUE)?true:false);

		//�˿�����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbOperateCard[0])]=0;
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			SetHandCardControl( m_cbCardIndex,0x00 );
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
		//�������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		CopyMemory( m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,cbOperateCard,sizeof(cbOperateCard) );

		//��Ͻ���
		BYTE cbWeaveCard[3],cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=3;
		CopyMemory( cbWeaveCard,cbOperateCard,sizeof(cbWeaveCard) );
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);

		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{
			VERIFY( m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard[0],1) );
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1) )
			{
				ASSERT( FALSE );
				return false;
			}
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}
	}

	//���ý���
	//����ṩ��Ҳ����Լ�,��ȥ���ṩ��ҵ��ѳ���
	if( pOperateResult->wProvideUser != wOperateUser )
	{
		if( m_GameClientView.m_DiscardCard[wProviderViewID].GetCardCount() > 0 )
			m_GameClientView.m_DiscardCard[wProviderViewID].RemoveCard(1);
	}
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);
	//������������ڲ���ģʽ,��ȡ��
	if( m_GameClientView.m_HandCardControl.GetSelectMode() )
		m_GameClientView.m_HandCardControl.SetSelectMode(false);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//��������
	PlayActionSound(wOperateUser,pOperateResult->cbOperateCode);

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//�����ж�
		if( !IsLookonMode() && m_cbListenStatus == 0 && m_wCurrentUser==GetMeChairID() )
		{
			WORD wMeChairId = GetMeChairID();

			if(m_cbCardIndex[m_GameLogic.SwitchToCardIndex(CAI_SHEN_CARD)]==0)
			{
				//m_cbActionMask = m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId]);
			}

			if( m_cbActionMask != WIK_NULL )
			{
				m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
			}
		}

		//����ʱ��
		WORD wTimeCount=TIME_OPERATE_CARD;

		//����ʱ��
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
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
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
	}

	//��������
	tagScoreInfo ScoreInfo;
	tagWeaveInfo WeaveInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));

	//�ɼ�����
	ScoreInfo.wMeChairID = GetMeChairID();
	ScoreInfo.wLandUser = m_wBankerUser;
	ScoreInfo.lCellScore = m_GameClientView.GetCellScore();
	ScoreInfo.cbLandCount = m_cbLandCount;
	ScoreInfo.wProvideUser=pGameEnd->wProvideUser[0];
	ScoreInfo.cbProvideCard=pGameEnd->cbProvideCard;
	ScoreInfo.bSecondMagic=pGameEnd->bSecondMagic;
	CopyMemory(ScoreInfo.cbFollowCard,pGameEnd->cbFollowCard,sizeof(ScoreInfo.cbFollowCard));
	CopyMemory(ScoreInfo.cbFollowCount,pGameEnd->cbFollowCount,sizeof(ScoreInfo.cbFollowCount));
	CopyMemory(ScoreInfo.lOutMagicScore,pGameEnd->lOutMagicScore,sizeof(ScoreInfo.lOutMagicScore));
	CopyMemory(ScoreInfo.lFollowScore,pGameEnd->lFollowScore,sizeof(ScoreInfo.lFollowScore));
	CopyMemory(ScoreInfo.lHuScore,pGameEnd->lHuScore,sizeof(ScoreInfo.lHuScore));
	CopyMemory(ScoreInfo.lFanHuScore,pGameEnd->lFanHuScore,sizeof(ScoreInfo.lFanHuScore));

	//���û���
	CString strTemp ,strEnd = TEXT("���ֽ���,�ɼ�ͳ��:\r\n");
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		strTemp.Format(_T("%s: %+ld\n"),m_szAccounts[i],pGameEnd->lGameScore[i]);
		strEnd += strTemp;
	}
	//��Ϣ����
	m_pIStringMessage->InsertSystemString((LPCTSTR)strEnd);

	CChiHuRight chr;
	chr.SetRightData( pGameEnd->dwChiHuRight1,CountArray(pGameEnd->dwChiHuRight1) );
	if( pGameEnd->wProvideUser[0] != INVALID_CHAIR )
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			ScoreInfo.dwChiHuKind[i]=pGameEnd->dwChiHuKind[i];

			//���óɼ�
			ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(ScoreInfo.szUserName[i],m_szAccounts[i],CountArray(ScoreInfo.szUserName[i]));

			//�����˿�
			if ((ScoreInfo.cbCardCount==0)&&(pGameEnd->dwChiHuKind[i]!=WIK_NULL))
			{
				ScoreInfo.wChiHuUser = i;
				//����˿�
				WeaveInfo.cbWeaveCount=m_cbWeaveCount[i];
				for (BYTE j=0;j<WeaveInfo.cbWeaveCount;j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					WeaveInfo.cbPublicWeave[j]=m_WeaveItemArray[i][j].cbPublicCard;
					WeaveInfo.cbCardCount[j]=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					CopyMemory( WeaveInfo.cbCardData[j],m_WeaveItemArray[i][j].cbCardData,sizeof(m_WeaveItemArray[i][j].cbCardData) );
				}

				//�����˿�
				ScoreInfo.cbCardCount=pGameEnd->cbCardCount[i];
				CopyMemory(ScoreInfo.cbCardData,&pGameEnd->cbCardData[i],ScoreInfo.cbCardCount*sizeof(BYTE));

				//��ȡ����
				for (BYTE j=0;j<ScoreInfo.cbCardCount;j++)
				{
					if (ScoreInfo.cbCardData[j]==pGameEnd->cbProvideCard)
					{
						MoveMemory(&ScoreInfo.cbCardData[j],&ScoreInfo.cbCardData[j+1],(ScoreInfo.cbCardCount-j-1)*sizeof(BYTE));
						ScoreInfo.cbCardData[ScoreInfo.cbCardCount-1]=pGameEnd->cbProvideCard;
						break;
					}
				}
			}
			
			PlayActionSound( i,WIK_CHI_HU );
		}

		ScoreInfo.cbFanCount = pGameEnd->cbFanCount;
		ScoreInfo.cbHuaCardCount = pGameEnd->cbHuaCardCount;
		//�ɼ�����
		m_GameClientView.m_ScoreControl[pGameEnd->wProvideUser[0]].SetScoreInfo(ScoreInfo,WeaveInfo,chr);

	}
	else 
	{
		WORD i=0;
		for ( i=0;i<GAME_PLAYER;i++)
		{
			if(ScoreInfo.lFollowScore[i]!=0)break;
			if(ScoreInfo.lOutMagicScore[i]!=0)break;
		}

		if(i==GAME_PLAYER)
		{
			m_GameClientView.SetHuangZhuang(true);
		}
		//�ɼ�����
		else 
		{
			//���óɼ�
			CChiHuRight chr;
			ScoreInfo.cbHuangZhuang=TRUE;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
				lstrcpyn(ScoreInfo.szUserName[i],m_szAccounts[i],CountArray(ScoreInfo.szUserName[i]));
				IClientUserItem * pUserData = GetTableUserItem(i);
				bool bBoy = (pUserData->GetGender() == 2 ? false : true);
				if(bBoy)
					PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_ZI_MO"));
				else
					PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_ZI_MO"));
	
			m_GameClientView.m_ScoreControl[i].SetScoreInfo(ScoreInfo,WeaveInfo,chr);
			}
		}

	}

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->dwChiHuKind[i]!=WIK_NULL) m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//��������
	if( !chr.IsEmpty() )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( pGameEnd->dwChiHuKind[i] != WIK_NULL )
			{
				//��������
				CString strSoundName=_T("");
				CChiHuRight ChiHuRight;
				ChiHuRight.SetRightData( pGameEnd->dwChiHuRight1,CountArray(pGameEnd->dwChiHuRight1) );

				////���
				//if( !(ChiHuRight&CHR_TIAN_HU).IsEmpty() ) 
				//	strSoundName = _T("CHR_TIAN_HU");
				////��һɫ��
				//else if( !(ChiHuRight&CHR_ZI_YI_SE).IsEmpty() ) 
				//	strSoundName = _T("CHR_ZI_YI_SE");
				////��һɫ��
				//else if( !(ChiHuRight&CHR_QING_YI_SE).IsEmpty() ) 
				//	strSoundName = _T("CHR_QING_YI_SE");
				////���ܺ�
				//else if( !(ChiHuRight&CHR_QIANG_GANG).IsEmpty() ) 
				//	strSoundName = _T("CHR_QIANG_GANG");
				////��С����
				//else if( !(ChiHuRight&CHR_QI_DUI).IsEmpty() ) 
				//	strSoundName = _T("CHR_QI_DUI");
				////������
				//else if( !(ChiHuRight&CHR_PENG_PENG).IsEmpty() ) 
				//	strSoundName = _T("CHR_PENG_PENG");
				////ȫ����
				//else if( !(ChiHuRight&CHR_QUAN_BU_KAO).IsEmpty() ) 
				//	strSoundName = _T("CHR_QUAN_BU_KAO");
				////�غ�
				//else if( !(ChiHuRight&CHR_DI_HU).IsEmpty() ) 
				//	strSoundName = _T("CHR_DI_HU");
				////���Ͽ���
				//else if( !(ChiHuRight&CHR_GANG_KAI).IsEmpty() ) 
				//	strSoundName = _T("CHR_GANG_KAI");
				////��������
				//else if( !(ChiHuRight&CHR_HAI_DI).IsEmpty() ) 
				//	strSoundName = _T("CHR_HAI_DI");
				////�����
				//else if( !(ChiHuRight&CHR_CAI_SHEN_HUI).IsEmpty() ) 
				//	strSoundName = _T("CHR_CAI_SHEN_HUI");
				////����
				//else if( !(ChiHuRight&CHR_ZI_MO).IsEmpty() ) 
				//	strSoundName = _T("CHR_ZI_MO");
				////ƽ��
				//else PlayActionSound( i,WIK_CHI_HU );

				if(strSoundName!=_T(""))
				{
					//�ж��Ա�
					IClientUserItem * pUserData = GetTableUserItem(i);
					if(pUserData == 0)break;
					bool bBoy = (pUserData->GetGender() == 2 ? false : true);

					if(bBoy)
					{
						strSoundName = _T("BOY_") +strSoundName;
					}
					else
					{
						strSoundName = _T("GIRL_") + strSoundName;
					}
					PlayGameSound(AfxGetInstanceHandle(), strSoundName);
				}

				break;
			}
		}
	}
	else 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));
	}
		

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	//ȡ���й�
	if(m_bStustee)
		OnStusteeControl(0,0);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//�û��й�
bool CGameClientDlg::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//��Ϣ����
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		IClientUserItem * pUserData = GetTableUserItem(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ѡ�����йܹ���."),pUserData->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ȡ�����йܹ���."),pUserData->GetNickName());
		m_pIStringMessage->InsertSystemString(szBuffer);
	}

	return true;
}

//�û�����
bool CGameClientDlg::OnSubListen(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Listen));
	if (wDataSize!=sizeof(CMD_S_Listen)) return false;

	CMD_S_Listen *pListen = (CMD_S_Listen *)pBuffer;
	WORD wViewId = SwitchViewChairID( pListen->wChairId );

	//��������״̬
	if( IsLookonMode() || pListen->wChairId != GetMeChairID() )
	{
		m_GameClientView.SetUserListenStatus( wViewId,true );
	}

	//��������
	PlayActionSound( pListen->wChairId,WIK_LISTEN );

	m_GameClientView.SetOutCardInfo( INVALID_CHAIR,0 );
	m_GameClientView.SetUserAction( wViewId,WIK_LISTEN );

	//����״̬
	m_GameClientView.SetStatusFlag( (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),m_wCurrentUser!=GetMeChairID() );

	return true;
}

//ת����Ϣ
void CGameClientDlg::ChangeInfo(BYTE bCard,CString &csInfo)
{
	switch(bCard)
	{
	case 0x01:csInfo=TEXT("һ��");break;
	case 0x02:csInfo=TEXT("����");break;
	case 0x03:csInfo=TEXT("����");break;
	case 0x04:csInfo=TEXT("����");break;
	case 0x05:csInfo=TEXT("����");break;
	case 0x06:csInfo=TEXT("����");break;
	case 0x07:csInfo=TEXT("����");break;
	case 0x08:csInfo=TEXT("����");break;
	case 0x09:csInfo=TEXT("����");break;

	case 0x11:csInfo=TEXT("һ��");break;
	case 0x12:csInfo=TEXT("����");break;
	case 0x13:csInfo=TEXT("����");break;
	case 0x14:csInfo=TEXT("����");break;
	case 0x15:csInfo=TEXT("����");break;
	case 0x16:csInfo=TEXT("����");break;
	case 0x17:csInfo=TEXT("����");break;
	case 0x18:csInfo=TEXT("����");break;
	case 0x19:csInfo=TEXT("����");break;

	case 0x21:csInfo=TEXT("һͲ");break;
	case 0x22:csInfo=TEXT("��Ͳ");break;
	case 0x23:csInfo=TEXT("��Ͳ");break;
	case 0x24:csInfo=TEXT("��Ͳ");break;
	case 0x25:csInfo=TEXT("��Ͳ");break;
	case 0x26:csInfo=TEXT("��Ͳ");break;
	case 0x27:csInfo=TEXT("��Ͳ");break;
	case 0x28:csInfo=TEXT("��Ͳ");break;
	case 0x29:csInfo=TEXT("��Ͳ");break;

	case 0x31:csInfo=TEXT("����");break;
	case 0x32:csInfo=TEXT("�Ϸ�");break;
	case 0x33:csInfo=TEXT("����");break;
	case 0x34:csInfo=TEXT("����");break;
	case 0x35:csInfo=TEXT("����");break;
	case 0x36:csInfo=TEXT("����");break;
	case 0x37:csInfo=TEXT("�װ�");break;

	//case 0x38:csInfo=TEXT("�����ﶬ");break;
	//case 0x39:csInfo=TEXT("�����ﶬ");break;
	//case 0x3a:csInfo=TEXT("�����ﶬ");break;
	//case 0x3b:csInfo=TEXT("�����ﶬ");break;
	//case 0x3c:csInfo=TEXT("÷������");break;
	//case 0x3d:csInfo=TEXT("÷������");break;
	//case 0x3e:csInfo=TEXT("÷������");break;
	//case 0x3f:csInfo=TEXT("÷������");break;

	}
	return ;
}

//���ų�������
void CGameClientDlg::PlayCardSound(WORD wChairID, BYTE cbCardData)
{
	if(m_GameLogic.IsValidCard(cbCardData) == false)
	{
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}

	//�ж��Ա�
	IClientUserItem * pUserData = GetTableUserItem(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "�ò��������Ϣ");
		return;
	}

	//ת���˿�
	if(g_CardResource.m_cbChangeCard==cbCardData)
	{
		cbCardData=CAI_SHEN_CARD;
	}
	else if(cbCardData==CAI_SHEN_CARD)
	{
		cbCardData=g_CardResource.m_cbChangeCard;
	}

	bool bBoy = (pUserData->GetGender() == 2 ? false : true);
	BYTE cbType= (cbCardData & MASK_COLOR);
	BYTE cbValue= (cbCardData & MASK_VALUE);
	CString strSoundName;

	switch(cbType)
	{
	case 0X30:	//��
		{
			switch(cbValue) 
			{
			case 1:
				{
					strSoundName = _T("F_1_");
					break;
				}
			case 2:
				{
					strSoundName = _T("F_2_");
					break;
				}
			case 3:
				{
					strSoundName = _T("F_3_");
					break;
				}
			case 4:
				{
					strSoundName = _T("F_4_");
					break;
				}
			case 5:
				{
					strSoundName = _T("F_5_");
					break;
				}
			case 6:
				{
					strSoundName = _T("F_6_");
					break;
				}
			case 7:
				{
					strSoundName = _T("F_7_");
					break;
				}
			default:
				{
					strSoundName=_T("BU_HUA");
				}

			}
			break;
		}		
	case 0X20:	//Ͳ
		{
			strSoundName.Format(_T("T_%d_"), cbValue);
			break;
		}

	case 0X10:	//��
		{
			strSoundName.Format(_T("S_%d_"), cbValue);
			break;
		}
	case 0X00:	//��
		{
			strSoundName.Format(_T("W_%d_"), cbValue);
			break;
		}
	}	

	if(bBoy)
	{
		strSoundName = _T("BOY_") +strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL_") + strSoundName;
	}



	for(int i=0;i<100;i++)
	{
		WORD WaveKind=rand()%4+1;
		CString strWaveKind;
		strWaveKind.Format(_T("%d"),WaveKind);
		HINSTANCE hInstance = AfxGetInstanceHandle();
		strSoundName=strSoundName+strWaveKind;
		HRSRC hResour=FindResource(hInstance,strSoundName,TEXT("WAVE"));
		if(hResour==NULL) continue;
		if (hResour!=NULL)
		{
			HGLOBAL hGlobal=LoadResource(hInstance,hResour);
			if (hGlobal!=NULL) 
			{
				//��������
				LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
				PlayGameSound(hInstance, strSoundName);

				//������Դ
				UnlockResource(hGlobal);
			}
			FreeResource(hGlobal);
		}
	}

	return;
}

//��������
void CGameClientDlg::PlayActionSound(WORD wChairID,BYTE cbAction)
{
	//�ж��Ա�
	IClientUserItem * pUserData = GetTableUserItem(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "�ò��������Ϣ");
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}
	bool bBoy = (pUserData->GetGender() == 2 ? false : true);

	switch (cbAction)
	{
	case WIK_LEFT:
	case WIK_CENTER:
	case WIK_RIGHT:		//����
		{
			WORD WaveKind=rand()%3+1;
			CString	strSoundName;

			if(bBoy)
			{
				strSoundName.Format(_T("BOY_CHI_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);
			}
			else
			{
				strSoundName.Format(_T("GIRL_CHI_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);		
			}
			break;
		}
	case WIK_PENG:		//����
		{
			WORD WaveKind=rand()%3+1;
			CString	strSoundName;

			if(bBoy)
			{
				strSoundName.Format(_T("BOY_PENG_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);
			}
			else
			{
				strSoundName.Format(_T("GIRL_PENG_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);	
			}
			break;
		}
	case WIK_GANG:		//����
		{
			WORD WaveKind=rand()%2+1;
			CString	strSoundName;

			if(bBoy)
			{
				strSoundName.Format(_T("BOY_GANG_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);
			}
			else
			{
				strSoundName.Format(_T("GIRL_GANG_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);	
			}
			break;
		}
	case WIK_CHI_HU:	//�Ժ�
		{

			WORD WaveKind=rand()%2+1;
			CString	strSoundName;

			if(bBoy)
			{
				strSoundName.Format(_T("BOY_CHI_HU_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);
			}
			else
			{
				strSoundName.Format(_T("GIRL_CHI_HU_%d"),WaveKind);
				PlayGameSound(AfxGetInstanceHandle(),strSoundName);		
			}
			break;
		}
	case WIK_REPLACE:	//�滻����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_REPLACE"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_REPLACE"));		
			break;
		}
	case WIK_LISTEN:	//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_TING"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_TING"));		
			break;
		}
	//case WIK_CHI_HU:
	//	{
	//		if(bBoy)
	//			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHR_ZI_MO"));
	//		else
	//			PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHR_ZI_MO"));		
	//		break;	
	//	}
	}

	return;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard(BYTE cbCardData)
{
	//�����ж�
	if ((m_cbListenStatus>0)||(m_bWillHearStatus==true))
	{
		//��������
		CChiHuRight chr;
		WORD wMeChairID=GetMeChairID();
		BYTE cbWeaveCount=m_cbWeaveCount[wMeChairID];

		//�����˿�
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,m_cbCardIndex,sizeof(cbCardIndexTemp));

		//ɾ���˿�
		if( !m_GameLogic.RemoveCard(cbCardIndexTemp,cbCardData) )
		{
			ASSERT( FALSE );
			return false;
		}

		//�����ж�
		BYTE i=0;
		for ( i=0;i<MAX_INDEX;i++)
		{
			//���Ʒ���
			BYTE cbCurrentCard=m_GameLogic.SwitchToCardData(i);
			BYTE cbHuCardKind=m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp,m_WeaveItemArray[wMeChairID],cbWeaveCount,cbCurrentCard,chr);

			//����ж�
			if (cbHuCardKind!=WIK_NULL)
			{
				break;
			}
		}

		//�����ж�
		return (i!=MAX_INDEX);
	}

	return true;
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//��������
	KillGameClock(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreControl[0].RestorationData();
	m_GameClientView.m_ScoreControl[1].RestorationData();
	m_GameClientView.m_ScoreControl[2].RestorationData();
	m_GameClientView.m_ScoreControl[3].RestorationData();

	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//���ý���
	m_GameClientView.SetDiscUser(INVALID_CHAIR);
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);
	
	//�˿�����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HeapCard[i].ResetCardData();
		m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
	}

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbListenStatus = 0;
	m_bWillHearStatus = false;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	//g_cbChangeCard = 0;
	g_CardResource.m_cbChangeCard=0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))
		return 0;

	//�����ж�
	if (((m_cbListenStatus>0)||(m_bWillHearStatus==true))&&(VerdictOutCard((BYTE)wParam)==false))
	{
		m_pIStringMessage->InsertSystemString(TEXT("�����Ʋ�������Ϸ����!"));
		return 0;
	}

	KillGameClock(IDI_OPERATE_CARD);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	BYTE cbOutCardData=(BYTE)wParam;
	if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData) )
	{
		ASSERT( FALSE );
		return 0;
	}

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	ASSERT( (cbCardCount-1)%3==0 );
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);

	//���ý���
	m_GameClientView.SetCurrentUser( INVALID_CHAIR );
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	if( m_GameClientView.m_DlgListen.m_hWnd )
		m_GameClientView.m_DlgListen.EndDialog(IDCANCEL);

	//������ƶ���
	COutCardItem *pOutCardItem = NULL;
	try { pOutCardItem = new COutCardItem(); }
	catch(...) { ASSERT(FALSE); return 0; }

	pOutCardItem->m_cbOutCardData = cbOutCardData;
	pOutCardItem->m_wOutCardUser = GetMeChairID();
	m_MoveCardItemArray.Add( pOutCardItem );

	tagMoveCardItem mci;
	mci.cbCardCount = 1;
	mci.cbCardData = cbOutCardData;
	mci.ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	mci.ptTo = m_GameClientView.m_DiscardCard[MYSELF_VIEW_ID].GetDispatchCardPos();
	mci.wViewChairId = MYSELF_VIEW_ID;
	m_GameClientView.OnMoveCardItem( mci );

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	//��������
	PlayCardSound(GetMeChairID(),cbOutCardData);

	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0;
}

//�˿˲���
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbOperateCode;
	BYTE cbOperateCard[3] = {0,0,0};

	//ɾ��ʱ��
	KillGameClock(IDI_OPERATE_CARD);

	//��ȡѡ����Ϣ
	tagSelectCardInfo si;
	m_GameClientView.m_HandCardControl.GetUserSelectResult( si );
	
	//���������Ϣ
	cbOperateCode = (BYTE)si.wActionMask;
	cbOperateCard[0] = si.cbActionCard;
	CopyMemory( &cbOperateCard[1],si.cbCardData,sizeof(BYTE)*2 );

	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//ȡ������ģʽ
	m_GameClientView.m_HandCardControl.SetSelectMode( false );

	//���������
	if( m_bWillHearStatus && (cbOperateCode&WIK_LISTEN) )
	{
		m_bWillHearStatus = false;
		return OnOutCard( cbOperateCard[0],0 );
	}

	//��������
	m_GameClientView.SetStatusFlag(false,true);
	
	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

//�Ϲܿ���
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	if(IsLookonMode())return 0;

	//���ñ���
	m_wTimeOutCount=0;

	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if( FALSE == (BOOL)wParam ) 
			m_pIStringMessage->InsertSystemString(_T("��ȡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if( FALSE == (BOOL)wParam )
			m_pIStringMessage->InsertSystemString(_T("��ѡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}

	return 0;
}

//�ڶ���ҡɫ����Ϣ
LRESULT CGameClientDlg::OnSiceTwo(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//��ʾ��Ϣ
	CString csInfo;
	csInfo.Format(TEXT("\nׯ��ҡ��������:%d,%d"),HIBYTE((WORD)(m_lSiceCount>>16)),LOBYTE((WORD)(m_lSiceCount>>16)));
	m_pIStringMessage->InsertSystemString(csInfo);

	return 0;
}

//ҡɫ�ӽ�����Ϣ
LRESULT CGameClientDlg::OnSiceFinish(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	//��ʾ��Ϣ
	WORD wChairID = HIBYTE((WORD)(m_lSiceCount>>16))+LOBYTE((WORD)(m_lSiceCount>>16))-1;
	wChairID = (wChairID+m_wBankerUser)%GAME_PLAYER;
	CString csInfo;
	csInfo.Format(TEXT("\n[%s]ҡ��������:%d,%d"),m_szAccounts[wChairID],HIBYTE((WORD)m_lSiceCount),LOBYTE((WORD)m_lSiceCount));
	//InsertGeneralString(csInfo,RGB(0,0,128),true);

	WORD wMeChairID = GetMeChairID();
	bool bPlayerMode = (IsLookonMode()==false);

	//���췢�ƶ���
	BYTE cbHandCardData[MAX_COUNT];
	m_GameLogic.SwitchToCardData( m_cbCardIndex,cbHandCardData );
	//�����˿�
	m_GameLogic.RandCardData(cbHandCardData,(m_wBankerUser==wMeChairID)?MAX_COUNT:MAX_COUNT-1,cbHandCardData);
	//ȷ������ɾ��λ��
	WORD wHeapId = m_wHeapTail;
	WORD wHeapCardIndex = m_cbHeapCardInfo[wHeapId][1]-1;
	if(m_cbHeapCardInfo[m_wHeapTail][1]==0)
	{
		wHeapId = (wHeapId+1)%GAME_PLAYER;
		wHeapCardIndex = HEAP_FULL_COUNT-1;
	}
	//��4Ȧ�����1Ȧ��һ��
	for( BYTE i = 0; i < 4; i++ )
	{
		//��ׯ�ҿ�ʼ
		WORD wId = m_wBankerUser;
		for( WORD j = 0; j < GAME_PLAYER; j++ )
		{
			WORD wChairId = (wId+GAME_PLAYER-j)%GAME_PLAYER;
			CStartCardItem *pStartCardItem = NULL;
			//����������Ŀ
			try{ pStartCardItem = new CStartCardItem(); }
			catch(...) { ASSERT(FALSE); return 0; }
			//������
			pStartCardItem->m_cbCardCount = i<3?4:1;
			//����ֵ
			if( wChairId == wMeChairID )
				CopyMemory( pStartCardItem->m_cbCardData,&cbHandCardData[i*4],sizeof(BYTE)*(i<3?4:1) );
			//�������
			pStartCardItem->m_wChairId = wChairId;
			//����λ��
			pStartCardItem->m_wHeapId = wHeapId;
			//����λ��
			pStartCardItem->m_wHeapCardIndex = wHeapCardIndex;
			//���붯������
			m_MoveCardItemArray.Add(pStartCardItem);

			//�ƽ�
			if( i < 3 )
			{
				if( wHeapCardIndex <= 3 )
				{
					wHeapId = (wHeapId+1)%GAME_PLAYER;
					wHeapCardIndex = HEAP_FULL_COUNT-(3-wHeapCardIndex)-1;
				}
				else wHeapCardIndex -= 4;
			}
			else
			{
				if( wHeapCardIndex == 0 )
				{
					wHeapId = (wHeapId+1)%GAME_PLAYER;
					wHeapCardIndex = HEAP_FULL_COUNT-1;
				}
				else wHeapCardIndex--;
			}
		}
	}
	//���һ�ţ�����ׯ��
	CStartCardItem *pStartCardItem = NULL;
	//����������Ŀ
	try{ pStartCardItem = new CStartCardItem(); }
	catch(...) { ASSERT(FALSE); return 0; }
	//������
	pStartCardItem->m_cbCardCount = 1;
	//����ֵ
	if( m_wBankerUser == wMeChairID )
		pStartCardItem->m_cbCardData[0] = cbHandCardData[MAX_COUNT-1];
	//�������
	pStartCardItem->m_wChairId = m_wBankerUser;
	//����λ��
	pStartCardItem->m_wHeapId = wHeapId;
	//����λ��
	pStartCardItem->m_wHeapCardIndex = wHeapCardIndex;
	//���һ��
	pStartCardItem->m_bLastItem = true;
	//���붯������
	m_MoveCardItemArray.Add(pStartCardItem);

	//��ʼ����
	BeginMoveCard();

	return 0;
}

//ׯ������
LRESULT CGameClientDlg::OnLandSiceFinish(WPARAM wParam, LPARAM lParam)
{
	//��ʾ��Ϣ
	//CString csInfo;
	//csInfo.Format(TEXT("\n[%s]ҡ��������:%d,%d \nׯ����[%s]"),m_szAccounts[m_wFirstUser],
	//	HIBYTE(m_wLandSice),LOBYTE(m_wLandSice),m_szAccounts[m_wBankerUser]);
	//InsertGeneralString(csInfo,RGB(0,0,128),true);

	//����ׯ��
	m_GameClientView.SetLandCount(m_cbLandCount);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//�жϽ���
	if(GetGameStatus()==GS_MJ_FREE)
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetHeapCardInfo(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1]);
		}
		return 0;
	}
	else
	{
		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

		//ҡɫ�Ӷ���
		m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,(WORD)(m_lSiceCount>>16),(WORD)m_lSiceCount);
		m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);
		m_GameClientView.m_DrawSiceWnd.StopSicing();
	}

	return 0;
}

//��������
LRESULT CGameClientDlg::OnCaiShenSiceFinish(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	//��ʾ��Ϣ
	CString csInfo,csCard;
	ChangeInfo(g_CardResource.m_cbChangeCard,csCard);
	csInfo.Format(TEXT("\nׯ��ҡ��������:%d,%d  ����:%s"),HIBYTE(m_wCaiShenSice),LOBYTE(m_wCaiShenSice),csCard);
	m_pIStringMessage->InsertSystemString(csInfo);

	//��������
	//g_cbChangeCard = g_CardResource.m_cbChangeCard ;
	m_GameClientView.m_HeapCard[SwitchViewChairID(m_wCaiShenChairID)].SetCardData(m_wCaiShenIndex*2,g_CardResource.m_cbChangeCard);

	if(GetGameStatus()==GS_MJ_FREE)return 0;
 
	//������
	if (!IsLookonMode())
		ActiveGameFrame();

	//�����˿�
	SetHandCardControl(m_cbCardIndex,0x00);

	m_GameClientView.m_HandCardControl.SetPositively(!IsLookonMode());

	//���Ʋ���
	WORD wMeChairId = GetMeChairID();
	if (!IsLookonMode()&&(m_wReplaceUser==wMeChairId))
	{
		//����ʱ��
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
	}

	//������ʾ,��������
	if (!IsLookonMode()&&(m_wCurrentUser==wMeChairId))
	{
		m_GameClientView.SetStatusFlag(true,false);

		if(m_cbCardIndex[m_GameLogic.SwitchToCardIndex(CAI_SHEN_CARD)]==0)
		{
			//m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,NULL,0);
		}

		if( m_cbActionMask != WIK_NULL )
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
	}
	else if( m_wCurrentUser != wMeChairId && m_wCurrentUser != INVALID_CHAIR )
		m_GameClientView.SetStatusFlag(false,true);

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	return 0;
}

//�����˿�
void CGameClientDlg::SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard)
{
	//�����Ŀ
	WORD wMeChairID=GetMeChairID();
	BYTE cbWeaveCardCount=m_cbWeaveCount[wMeChairID]*3;

	//ת���˿�
	BYTE cbHandCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(cbCardIndex,cbHandCardData);

	//�����˿�
	if ( (cbWeaveCardCount+cbCardCount)==MAX_COUNT )
	{
		if( cbAdvanceCard != 0x00 )
		{
			//ɾ���˿�
			BYTE cbRemoveCard[]={cbAdvanceCard};
			VERIFY( m_GameLogic.RemoveCard(cbHandCardData,cbCardCount,cbRemoveCard,1) );

			//�����˿�
			cbHandCardData[cbCardCount-1]=cbAdvanceCard;
		}
		
		m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount-1,cbHandCardData[cbCardCount-1]);
	}
	else m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount,0x00);

	return;
}

//��ʱ����Ϣ
VOID CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_REPLACE_CARD:	//�û�����
		{
			//ɾ��ʱ��
			KillTimer(IDI_REPLACE_CARD);

			//���ý���
			m_GameClientView.SetUserAction( INVALID_CHAIR,0 );
			//Ѱ���˿�
			for (BYTE i=MAX_INDEX-MAX_HUA_CARD;i<MAX_INDEX;i++)
			{
				if (m_cbCardIndex[i]>0)
				{
					//��������
					BYTE cbReplaceCard=m_GameLogic.SwitchToCardData(i);
					if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard) )
					{
						ASSERT( FALSE );
						return;
					}

					//�����˿�
					SetHandCardControl(m_cbCardIndex,0x00);

					//��������
					PlayActionSound(GetMeChairID(),WIK_REPLACE);

					//���ý���
					m_GameClientView.SetUserAction( MYSELF_VIEW_ID,WIK_REPLACE );

					CReplaceCardItem *pReplaceCardItem = NULL;
					try{ pReplaceCardItem = new CReplaceCardItem(); }
					catch(...) { ASSERT(FALSE); return; }
					pReplaceCardItem->m_cbReplaceCard = cbReplaceCard;
					pReplaceCardItem->m_wReplaceUser = GetMeChairID();
					m_MoveCardItemArray.Add( pReplaceCardItem );

					//������ƶ���
					tagMoveCardItem mci;
					mci.cbCardCount = 1;
					mci.cbCardData = cbReplaceCard;
					mci.wViewChairId = MYSELF_VIEW_ID;
					mci.ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
					mci.ptTo = m_GameClientView.m_DiscardCard[MYSELF_VIEW_ID].GetDispatchCardPos();
					m_GameClientView.OnMoveCardItem( mci );

					//��������
					CMD_C_ReplaceCard ReplaceCard;
					ReplaceCard.cbCardData=cbReplaceCard;
					SendSocketData(SUB_C_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

					break;
				}
			}
		
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//��Ҳ����ؼ�
LRESULT CGameClientDlg::OnUserAction( WPARAM wParam, LPARAM lParam )
{
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard[3] = {0,0,0};

	//���ز����ؼ�
	m_GameClientView.m_ControlWnd.ShowWindow( SW_HIDE );

	//״̬�ж�
	if (cbOperateCode==WIK_NULL)
	{
		//���ñ���
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;
		if( m_wCurrentUser == INVALID_CHAIR )
		{
			//ɾ����ʱ��
			KillGameClock( IDI_OPERATE_CARD );
			//������Ϣ
			CMD_C_OperateCard OperateCard;
			OperateCard.cbOperateCode = WIK_NULL;
			ZeroMemory( OperateCard.cbOperateCard,sizeof(OperateCard.cbOperateCard) );
			SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
		}
		return 0;
	}

	if( cbOperateCode & WIK_LISTEN )
	{
		ASSERT( m_cbListenStatus == 0 );
		if( m_cbListenStatus != 0 ) return 0;

		//����ѡ��
		if( m_GameClientView.m_DlgListen.DoModal() == IDOK )
		{
			//ѡ��������ץ��
			if( m_GameClientView.m_DlgListen.m_bZhuaPao && m_GameClientView.m_DlgListen.m_bZiMo )
				m_cbListenStatus = 3;
			//ֻ����
			else if( m_GameClientView.m_DlgListen.m_bZiMo )
				m_cbListenStatus = 2;
			//ֻץ��
			else if( m_GameClientView.m_DlgListen.m_bZhuaPao )
				m_cbListenStatus = 1;
			//���ѡ����,����������Ϣ
			if( m_cbListenStatus > 0 )
			{
				//���ü������Ʊ�־
				m_bWillHearStatus = true;

				//���ñ���
				m_cbActionMask = WIK_NULL;
				m_cbActionCard = 0;

				//��������״̬
				m_GameClientView.SetUserListenStatus( 2,true );

				//������������
				CMD_C_Listen Listen;
				Listen.cbListen = m_cbListenStatus;
				SendSocketData( SUB_C_LISTEN,&Listen,sizeof(Listen) );

				//�жϲ���
				if(m_cbCardIndex[m_GameLogic.SwitchToCardIndex(CAI_SHEN_CARD)]==0)
				{
					//��ȡѡ�����
					tagSelectCardInfo sci[MAX_WEAVE];
					BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );

					//���ֻ��һ����
					if( cbInfoCount == 1 )
					{
						m_bWillHearStatus = false;
						OnOutCard( sci[0].cbActionCard,0 );
					}
					//���������ѡ��
					else
					{
						//���ò����¼�
						m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );
						//��ʾ��Ϣ
						m_pIStringMessage->InsertSystemString(TEXT("��ѡ�������"));
					}
				}
				else m_bWillHearStatus = false;
			}
		}

		return 0;
	}

	bool bDone = false;
	//����
	if( cbOperateCode & WIK_CHI_HU )
		bDone = true;
	else
	{
		//��ȡѡ�����
		tagSelectCardInfo sci[MAX_WEAVE];
		BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );

		//���ò����¼�
		bDone = m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );

		//�����ɲ���
		if( bDone )
		{
			//���ò������
			tagSelectCardInfo si;
			m_GameClientView.m_HandCardControl.GetUserSelectResult( si );
			cbOperateCode = (BYTE)si.wActionMask;
			cbOperateCard[0] = si.cbActionCard;
			CopyMemory( &cbOperateCard[1],si.cbCardData,2*sizeof(BYTE) );
			m_GameClientView.m_HandCardControl.SetSelectMode( false );
		}
		//�������õȴ�ѡ��
		else m_GameClientView.SetStatusFlag( true,false );
	}

	//���������ɣ�ֱ�ӷ��Ͳ�������
	if( bDone )
	{
		//ɾ����ʱ��
		KillGameClock( IDI_OPERATE_CARD );

		//���ñ���
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;

		CMD_C_OperateCard OperateCard;
		OperateCard.cbOperateCode = cbOperateCode;
		CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );


		BYTE cbValue0=cbOperateCard[0]&MASK_VALUE;
		BYTE cbValue1=cbOperateCard[1]&MASK_VALUE;
		BYTE cbValue2=cbOperateCard[2]&MASK_VALUE;

		SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
	}

	return 0;
}

//��ȡ������Ϣ
BYTE CGameClientDlg::GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] )
{
	//��ʼ��
	BYTE cbSelectCount = 0;

	if( wOperateCode == WIK_NULL ) return 0;

	//����
	if( wOperateCode & WIK_LISTEN )
	{
		//��ȡ�ɶ�����
		BYTE cbCardData[MAX_COUNT],cbCardCount=0;
		BYTE cbCardIndex[MAX_INDEX];
		CopyMemory( cbCardIndex,m_cbCardIndex,sizeof(cbCardIndex) );
		WORD wMeChairId = GetMeChairID();
		CChiHuRight chr;
		for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
		{
			if( cbCardIndex[i] == 0 ) continue;

			cbCardIndex[i]--;
			for( BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++ )
			{
				BYTE cbCurrentCard = m_GameLogic.SwitchToCardData(j);
				if( WIK_CHI_HU == m_GameLogic.AnalyseChiHuCard( cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],
					cbCurrentCard,chr) )
				{
					cbCardData[cbCardCount++] = m_GameLogic.SwitchToCardData(i);
					break;
				}
			}
			cbCardIndex[i]++;
		}
		for( BYTE i = 0; i < cbCardCount; i++ )
		{
			SelectInfo[cbSelectCount].cbActionCard = cbCardData[i];
			SelectInfo[cbSelectCount].wActionMask = WIK_LISTEN;
			SelectInfo[cbSelectCount].cbCardCount = 1;
			SelectInfo[cbSelectCount++].cbCardData[0] = cbCardData[i];
		}
	}
	//����
	else if( wOperateCode&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
	{
		//Ч��
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		if( m_cbActionMask & WIK_LEFT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_LEFT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard+1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+2;
		}
		if( m_cbActionMask & WIK_CENTER )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_CENTER;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+1;
		}
		if( m_cbActionMask & WIK_RIGHT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_RIGHT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-2;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard-1;
		}
	}
	//����
	else if( wOperateCode & WIK_PENG )
	{
		//Ч��
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
		SelectInfo[cbSelectCount].wActionMask = WIK_PENG;
		SelectInfo[cbSelectCount].cbCardCount = 2;
		SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
		SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard;
	}
	//����
	else if( wOperateCode & WIK_GANG )
	{
		//������Լ�����
		if( m_wCurrentUser == GetMeChairID() )
		{
			//Ѱ���Ƿ��ж������
			WORD wMeChairId = GetMeChairID();
			tagGangCardResult gcr;
			ZeroMemory( &gcr,sizeof(gcr) );
			m_GameLogic.AnalyseGangCard( m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],gcr );
			ASSERT( gcr.cbCardCount > 0 );
			for( BYTE i = 0; i < gcr.cbCardCount; i++ )
			{
				SelectInfo[cbSelectCount].cbActionCard = gcr.cbCardData[i];
				SelectInfo[cbSelectCount].wActionMask = WIK_GANG;				
				if( m_cbCardIndex[m_GameLogic.SwitchToCardIndex(gcr.cbCardData[i])] == 1 )
				{
					SelectInfo[cbSelectCount].cbCardCount = 1;
					SelectInfo[cbSelectCount].cbCardData[0] = gcr.cbCardData[i];
				}
				else
				{
					SelectInfo[cbSelectCount].cbCardCount = m_GameLogic.GetWeaveCard(WIK_GANG,gcr.cbCardData[i],
						SelectInfo[cbSelectCount].cbCardData);
				}
				cbSelectCount++;
			}
		}
		else
		{
			ASSERT( m_cbActionCard != 0 );
			if( m_cbActionCard == 0 ) return 0;
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_GANG;
			SelectInfo[cbSelectCount].cbCardCount = 3;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
			SelectInfo[cbSelectCount].cbCardData[1] = m_cbActionCard;
			SelectInfo[cbSelectCount++].cbCardData[2] = m_cbActionCard;
		}
	}

	return cbSelectCount;
}

//�˿˶���
bool CGameClientDlg::BeginMoveCard()
{
	if( m_MoveCardItemArray.GetCount() == 0 ) return false;
	if( m_GameClientView.IsMovingCard() ) return true;

	MoveKind enMoveKind = m_MoveCardItemArray[0]->GetMoveKind();

	//���ַ���
	if( enMoveKind == MK_StartCard )
		return BeginMoveStartCard( m_MoveCardItemArray[0] );
	//����
	else if( enMoveKind == MK_OutCard )
		return BeginMoveOutCard( m_MoveCardItemArray[0] );
	//����
	else if( enMoveKind == MK_SendCard )
		return BeginMoveSendCard( m_MoveCardItemArray[0] );
	//����
	else if( enMoveKind == MK_ReplaceCard )
		return BeginMoveReplaceCard( m_MoveCardItemArray[0] );

	return false;
}

//ֹͣ����
void CGameClientDlg::StopMoveCard()
{
	if( m_GameClientView.IsMovingCard() )
		m_GameClientView.StopMoveCard();

	while( BeginMoveCard() ) 
		m_GameClientView.StopMoveCard();
}

//���ƶ���
bool CGameClientDlg::BeginMoveOutCard( const CMoveCardItem *pMoveCardItem )
{
	const COutCardItem *pOutCardItem = (const COutCardItem *)pMoveCardItem;

	//�������
	WORD wMeChairId = GetMeChairID();
	WORD wOutCardUser = pOutCardItem->m_wOutCardUser;
	WORD wViewOutCardUser = SwitchViewChairID(wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->m_cbOutCardData;
	
	if( IsLookonMode() || wOutCardUser != wMeChairId )
	{
		//��������
		KillGameClock(IDI_OPERATE_CARD); 
		
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;

		//������ʼ,������
		CPoint ptFrom,ptTo;
		if( wOutCardUser == wMeChairId )
			ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
		else
		{
			WORD wViewId = wOutCardUser>=3?2:wOutCardUser;
			ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
		}
		ptTo = m_GameClientView.m_DiscardCard[wViewOutCardUser].GetDispatchCardPos();

		//��������
		PlayCardSound(wOutCardUser,cbOutCardData);

		//���ý���
		m_GameClientView.SetCurrentUser( INVALID_CHAIR );
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wViewOutCardUser,cbOutCardData);
		
		//�����˿�
		if (wViewOutCardUser==MYSELF_VIEW_ID)
		{
			//ɾ���˿�
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData) )
			{
				ASSERT( FALSE );
				return false;
			}

			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wViewOutCardUser>2)?2:wViewOutCardUser;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}

		tagMoveCardItem mci;
		mci.ptFrom = ptFrom;
		mci.ptTo = ptTo;
		mci.cbCardCount = 1;
		mci.cbCardData = cbOutCardData;
		mci.wViewChairId = wViewOutCardUser;
		m_GameClientView.OnMoveCardItem( mci );
	}

	return true;
}

//���ƶ���
bool CGameClientDlg::BeginMoveSendCard( const CMoveCardItem *pMoveCardItem )
{
	const CSendCardItem *pSendCardItem = (const CSendCardItem *)pMoveCardItem;

	WORD wSendUserViewId = SwitchViewChairID(pSendCardItem->m_wSendCardUser);

	//������ʼ,������
	CPoint ptFrom,ptTo;
	if( !pSendCardItem->m_bTail )
	{
		ASSERT( m_wHeapHead != INVALID_CHAIR );
		ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].GetDispatchCardPos(false);
		//ɾ����ͷ
		if( !m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].DeductionHeapCard(false) )
		{
			m_wHeapHead = (m_wHeapHead+1)%GAME_PLAYER;
			ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].GetDispatchCardPos(false);
			VERIFY( m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].DeductionHeapCard(false) );
		}
	}
	else
	{
		ASSERT( m_wHeapTail != INVALID_CHAIR );
		ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].GetDispatchCardPos(true);
		//ɾ����β
		if( !m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].DeductionHeapCard(true) )
		{
			m_wHeapTail = (m_wHeapTail+GAME_PLAYER-1)%GAME_PLAYER;
			ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].GetDispatchCardPos(true);
			VERIFY( m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].DeductionHeapCard(true) );
		}
	}
	if( wSendUserViewId == MYSELF_VIEW_ID )
		ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wViewId = wSendUserViewId>=3?2:wSendUserViewId;
		ptTo = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
	}

	//���ŷ�������
	m_DirectSound[1].Play();

	//���춯��
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = 1;
	mci.cbCardData = 0;
	mci.wViewChairId = SwitchViewChairID(pSendCardItem->m_wSendCardUser);
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//��������
bool CGameClientDlg::BeginMoveReplaceCard( const CMoveCardItem *pMoveCardItem )
{
	const CReplaceCardItem *pReplaceCardItem = (const CReplaceCardItem *)pMoveCardItem;

	//�������
	WORD wMeChairId = GetMeChairID();
	WORD wReplaceUser = pReplaceCardItem->m_wReplaceUser;
	WORD wReplaceUserViewId = SwitchViewChairID(wReplaceUser);
	BYTE cbReplaceCard = pReplaceCardItem->m_cbReplaceCard;
	
	if( IsLookonMode() || wReplaceUser != wMeChairId )
	{
		//��������
		PlayActionSound(wReplaceUser,WIK_REPLACE);

		//������ʼ,������
		CPoint ptFrom,ptTo;
		if( wReplaceUser == wMeChairId )
			ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
		else
		{
			WORD wViewId = wReplaceUserViewId>=3?2:wReplaceUserViewId;
			ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
		}
		ptTo = m_GameClientView.m_DiscardCard[wReplaceUserViewId].GetDispatchCardPos();

		//���ö���
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction( wReplaceUserViewId,WIK_REPLACE );

		//�����˿�
		if (wReplaceUserViewId==MYSELF_VIEW_ID)
		{
			//ɾ���˿�
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard) )
			{
				ASSERT( FALSE );
				return false;
			}

			//�����˿�
			SetHandCardControl(m_cbCardIndex,0x00);
		}
		else
		{
			//�����˿�
			WORD wUserIndex=(wReplaceUserViewId>2)?2:wReplaceUserViewId;
			if( m_GameClientView.m_UserCard[wUserIndex].IsHaveCurrentCard() )
				m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
			else
			{
				WORD wCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount();
				m_GameClientView.m_UserCard[wUserIndex].SetCardData(wCardCount-1,false);
			}
		}

		tagMoveCardItem mci;
		mci.ptFrom = ptFrom;
		mci.ptTo = ptTo;
		mci.cbCardCount = 1;
		mci.cbCardData = cbReplaceCard;
		mci.wViewChairId = wReplaceUserViewId;
		m_GameClientView.OnMoveCardItem( mci );
	}

	return true;
}

//���ַ��ƶ���
bool CGameClientDlg::BeginMoveStartCard( const CMoveCardItem *pMoveCardItem )
{
	const CStartCardItem *pStartCardItem = (const CStartCardItem *)pMoveCardItem;

	//��ʼ,������
	CPoint ptFrom,ptTo;

	//����λ��
	WORD wHeapId = pStartCardItem->m_wHeapId;
	WORD wHeapViewId = SwitchViewChairID(wHeapId);
	//����λ��
	WORD wHeapCardIndex = pStartCardItem->m_wHeapCardIndex;
	if( pStartCardItem->m_cbCardCount > wHeapCardIndex+1 )
	{
		ASSERT( pStartCardItem->m_cbCardCount == 4 );
		//ɾ��ǰ��Ķ���
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,wHeapCardIndex+1 );

		wHeapId = (wHeapId+1)%GAME_PLAYER;
		wHeapViewId = SwitchViewChairID(wHeapId);
		wHeapCardIndex = HEAP_FULL_COUNT-(4-(wHeapCardIndex+1))-1;

		//��ȡ��ʼ��
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(WORD(wHeapCardIndex-1));

		//ɾ������
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( HEAP_FULL_COUNT-1,
			pStartCardItem->m_cbCardCount-pStartCardItem->m_wHeapCardIndex-1 );
	}
	else
	{
		WORD wHeapPos = wHeapCardIndex;
		if( pStartCardItem->m_cbCardCount == 4 )
			wHeapPos -= 3;
		else
		{
			ASSERT( pStartCardItem->m_cbCardCount == 1 );
			if( wHeapPos%2 != 0 ) wHeapPos--;
			else wHeapPos++;
		}
		//��ȡ��ʼ��
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(wHeapPos);

		//ɾ������
		if( pStartCardItem->m_cbCardCount == 4 )
			m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,4 );
		else
		{
			ASSERT( pStartCardItem->m_cbCardCount == 1 );
			if( wHeapCardIndex%2 != 0 )
				m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex-1,pStartCardItem->m_cbCardCount );
			else m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex+1,pStartCardItem->m_cbCardCount );
		}
	}

	//��ȡ������
	WORD wViewChairId = SwitchViewChairID(pStartCardItem->m_wChairId);
	if( wViewChairId == MYSELF_VIEW_ID )
		ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		ptTo = m_GameClientView.m_UserCard[wUserIndex].GetDispatchCardPos();
	}
	
	//���ŷ�������
	m_DirectSound[1].Play();

	//���춯��
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = pStartCardItem->m_cbCardCount;
	mci.cbCardData = 0;
	mci.wViewChairId = wViewChairId;
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//���������Ϣ
LRESULT CGameClientDlg::OnMoveCardFinish( WPARAM wParam, LPARAM lParam )
{
	//У��
	ASSERT( m_MoveCardItemArray.GetCount() > 0 );
	if( m_MoveCardItemArray.GetCount() == 0 ) return 0;

	MoveKind enMoveKind = m_MoveCardItemArray[0]->GetMoveKind();

	//������Ϣ
	if( enMoveKind == MK_StartCard )
	{
		CStartCardItem StartCardItem = *((CStartCardItem *)m_MoveCardItemArray[0]);

		//ɾ��������Ŀ
		SafeDelete(m_MoveCardItemArray[0]);
		m_MoveCardItemArray.RemoveAt(0);

		VERIFY( OnMoveStartCardFinish(&StartCardItem) );
	}
	else if( enMoveKind == MK_OutCard )
	{
		COutCardItem OutCardItem = *((COutCardItem *)m_MoveCardItemArray[0]);

		//ɾ��������Ŀ
		SafeDelete(m_MoveCardItemArray[0]);
		m_MoveCardItemArray.RemoveAt(0);

		VERIFY( OnMoveOutCardFinish(&OutCardItem) );
	}
	else if( enMoveKind == MK_SendCard )
	{
		CSendCardItem SendCardItem = *((CSendCardItem *)m_MoveCardItemArray[0]);

		//ɾ��������Ŀ
		SafeDelete(m_MoveCardItemArray[0]);
		m_MoveCardItemArray.RemoveAt(0);

		VERIFY( OnMoveSendCardFinish(&SendCardItem) );
	}
	else if( enMoveKind == MK_ReplaceCard )
	{
		CReplaceCardItem ReplaceCardItem = *((CReplaceCardItem *)m_MoveCardItemArray[0]);

		//ɾ��������Ŀ
		SafeDelete(m_MoveCardItemArray[0]);
		m_MoveCardItemArray.RemoveAt(0);

		VERIFY( OnMoveReplaceCardFinish(&ReplaceCardItem) );
	}
	else 
	{
		ASSERT(FALSE);
		return 0;
	}

	//������һ������
	BeginMoveCard();

	return 0;
}

//���ƶ������
bool CGameClientDlg::OnMoveOutCardFinish( const COutCardItem *pOutCardItem )
{
	WORD wOutUserViewId = SwitchViewChairID(pOutCardItem->m_wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->m_cbOutCardData;

	//��ӵ��Ѷ�����
	m_GameClientView.m_DiscardCard[wOutUserViewId].AddCardItem( cbOutCardData );

	//���ó���ָʾ
	m_GameClientView.SetDiscUser( wOutUserViewId );

	//��������
	m_DirectSound[0].Play();

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//���ƶ������
bool CGameClientDlg::OnMoveSendCardFinish( const CSendCardItem *pSendCardItem )
{
	//���ñ���
	WORD wMeChairID = GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pSendCardItem->m_wSendCardUser);
	WORD wChairId = pSendCardItem->m_wSendCardUser;
	BYTE cbSendCardData = pSendCardItem->m_cbCardData;
	m_wCurrentUser = pSendCardItem->m_wCurrentUser;

	//ȡ�ƽ���
	if (wViewChairID!=MYSELF_VIEW_ID)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
	}
	else
	{
		//�����˿�
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbSendCardData)]++;

		//�����˿�
		SetHandCardControl(m_cbCardIndex,cbSendCardData);
	}

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		ActiveGameFrame();

		m_cbActionMask = pSendCardItem->m_cbActionMask;
		if( m_cbListenStatus == 0 && m_cbCardIndex[m_GameLogic.SwitchToCardIndex(CAI_SHEN_CARD)]==0 )
		{
			//m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID]);
		}
		m_cbActionCard = pSendCardItem->m_cbCardData;

		//��������
		if (m_cbActionMask!=WIK_NULL)
		{
			//���ý���
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
		}
	}

	//���Ʋ���
	if ((IsLookonMode()==false)&&(pSendCardItem->m_wReplaceUser==wMeChairID))
	{
		//����ʱ��
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
	}

	//������ʾ
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//����ʱ��
	WORD wTimeCount=TIME_OPERATE_CARD;

	//����ʱ��
	if( m_wCurrentUser != INVALID_CHAIR )
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}
	else m_GameClientView.SetCurrentUser( INVALID_CHAIR );

	return true;
}

//�����������
bool CGameClientDlg::OnMoveReplaceCardFinish( const CReplaceCardItem *pReplaceCardItem )
{
	WORD wReplaceUserViewId = SwitchViewChairID(pReplaceCardItem->m_wReplaceUser);
	BYTE cbReplaceCardData = pReplaceCardItem->m_cbReplaceCard;

	//��ӵ��Ѷ�����
	m_GameClientView.m_DiscardCard[wReplaceUserViewId].AddCardItem( cbReplaceCardData );

	//���ó���ָʾ
	m_GameClientView.SetDiscUser( wReplaceUserViewId );

	//��������
	m_DirectSound[0].Play();

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//���ֶ������
bool CGameClientDlg::OnMoveStartCardFinish( const CStartCardItem *pStartCardItem )
{
	//���ñ���
	WORD wMeChairId = GetMeChairID();
	WORD wChairId = pStartCardItem->m_wChairId;
	WORD wViewChairId = SwitchViewChairID(wChairId);

	//����˿�
	if( wViewChairId == MYSELF_VIEW_ID )
		m_GameClientView.m_HandCardControl.AddCardData( pStartCardItem->m_cbCardData,pStartCardItem->m_cbCardCount );
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		m_GameClientView.m_UserCard[wUserIndex].AddCardData( pStartCardItem->m_cbCardCount );
	}

	//��������
	m_DirectSound[1].Play();

	//�ж��Ƿ����һ����
	if( pStartCardItem->m_bLastItem )
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetHeapCardInfo(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1]);
		}

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

		//ҡɫ�Ӷ���
		m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,0,m_wCaiShenSice);
		m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);
		//m_GameClientView.m_DrawSiceWnd.StopSicing();

	}

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//////////////////////////////////////////////////////////////////////////
