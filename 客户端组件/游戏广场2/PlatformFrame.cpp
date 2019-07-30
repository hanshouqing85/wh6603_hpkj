#pragma once
#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgEnquire.h"
#include "PlatformFrame.h"
#include "MessageDlg.h"
#include "HapiDump.h"
#include "GameRule.h"
#pragma pack(push,  1)
#pragma pack(show)
//////////////////////////////////////////////////////////////////////////////////
#define IDC_LST_MENU	WM_USER + 1000

//Բ�Ǵ�С
#define ROUND_CX					1									//Բ�ǿ��
#define ROUND_CY					1									//Բ�Ǹ߶�

//��Ӱ����
#define SHADOW_CX					5									//��Ӱ���
#define SHADOW_CY					5									//��Ӱ�߶�

//��Ļλ��
#define BORAD_SIZE					5									//�߿��С
#define CAPTION_SIZE				179									//�����С

//����λ��
#define SPLITTER_CX					0									//��ֿ��
#define LESS_LIST_CX				205									//�б���
#define FRAME_AFFICHE_CX			222									//��ܹ���

//��Ļ����
#define LESS_SCREEN_CY				740									//��С�߶�
#define LESS_SCREEN_CX				1030								//��С���

//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_MIN						100									//��С��ť
#define IDC_CLOSE					101									//�رհ�ť
#define IDC_SOUND					102									//������ť
#define IDC_NET						103									//���簴ť

//�ؼ���ʶ
#define IDC_WEB_PUBLICIZE			300									//����ؼ�
#define IDC_WEB_PUBLICIZE1          301                                 //����ؼ� 
#define IDC_WEB_PUBLICIZE2          302                                 //����ؼ� 
#define IDC_SKIN_SPLITTER			303									//��ֿؼ�
#define IDC_SYSTEM_TRAY_ICON		304									//����ͼ��
#define IDC_WEB_LOGO		        306                                 //LOGO�ؼ� 
#define IDC_WEB_LB_ADD		        307                                 //���½ǹ��

#define	IDC_TOP_NWES				305									//���ſؼ�

//�ؼ���ʶ
#define IDC_GAME_TYPE_CTRL			310									//���Ϳ��
#define IDC_USER_INFO_CTRL			311									//�û���Ϣ
#define IDC_LAST_TOP_NEWS			312									//����
//��������Ŷ�̬�Ĺ�������
static const int TimerNews		= 4;
//�������������ˢ�¿������ݵ�
static const int TimerZhunBeiData = 5;
static const int TimerJiShi = 6;
static const int TimerNews1		= 7;
static const int TimerLuckyNum		= 8;

static const int TimerReconnect		= 9;
static const int TimerTopNews		= 3;

static const int TimerZnx		= 10;//վ����
//��Χ��ť
#define IDC_NAVIGATION				400									//������ť

#define  IDC_ZHUANHUAN_CAIBI		500									//ת���ʱ�
#define  IDC_ZHUANHUAN_JINBI		501									//ת�����
#define	 IDC_HOME					502									//��ҳ��ť
#define	 IDC_NOTICE					503									//���水ť
#define	 IDC_MESSAGE					504									//վ���Ű�ť
//////////////////////////////////////////////////////////////////////////////////
//��̬����

CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//���ָ��
static bool bShowInfomation = true;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame, CFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(IDM_NEXT_PAGE,OnTurnPage)
	ON_MESSAGE(IDM_SELECT_USER,OnSelectUser)
	//�Զ���Ϣ
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)
	ON_MESSAGE(WM_INSUREPLAZA_UPDATA, OnMessageInsureUpdate)
	ON_MESSAGE(IDM_CLICKED_TYPE,OnUpdateButton)
	ON_MESSAGE(IDM_UPDATE_ACCOUNT,OnUpDateAccoount)
	ON_MESSAGE(IDM_SHOW_XGMM,OnShowXgmm)
	ON_NOTIFY(NM_CLICK, IDC_TOP_NWES, OnGridClick)
	ON_MESSAGE(IDM_REFRESH_YUE,OnRefreshYue)
	ON_MESSAGE(IDM_ZHUANHUAN_TO_CAIPIAO,OnZhuanhuanCaibi)
	ON_MESSAGE(IDM_ZHUANHUAN_TO_QIPAI,OnZhuanhuanJinbi)
	ON_MESSAGE(IDM_FRESH_GAME,OnFreshGame)
	ON_MESSAGE(IDM_TOUZHU_TISHI,OnTouZhuTishi)
	ON_MESSAGE(IDM_RETURN_GAME,ReturnTouzhu)
	ON_MESSAGE(IDM_RELEASE_FACE,ReleaseFace)
	ON_WM_TIMER()

END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlatformFrame::CPlatformFrame()
{
	//״̬����
	m_bRectify=false;
	m_bLogonFail=false;
	m_rcViewItem.SetRect(0,0,0,0);
	m_bGame=true;
	m_bGetLuckyNums=false;
	m_nNewsYPos=362+360;
	m_nNewsYPos -= 290;
	m_nreduce=0;
	m_dwTickCount = 0;
	m_cbShowNewsType = 0;
	m_bKillSocket = false;
	m_bCreate = false;
	m_bSound=true;
	m_nTop = 0;//400;
	//Menu.CreateMenu();
	m_bShowNetAlive=true;
	m_bCanSend = false;
	//�������
	m_MissionManager.InsertMissionItem(&m_MissionList);
	m_MissionManager.InsertMissionItem(&m_MissionLogon);
	m_MissionManager.InsertMissionItem(this);

	m_bGetNews=false;
	m_bQuitGame=false;
	m_bGetMapBonus=false;
	//ƽ̨����
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) 
		m_pPlatformFrame=this;

	m_bToCaijin=false;
	m_bChaxunYue=false;
	m_bToJinbi = false;
	//��ܻ���
	tagEncircleResource	EncircleFrame;
	EncircleFrame.pszImageTL=TEXT("FRAME_TL");
	EncircleFrame.pszImageTM=TEXT("FRAME_TM");
	EncircleFrame.pszImageTR=TEXT("FRAME_TR");
	EncircleFrame.pszImageML=TEXT("FRAME_ML");
	EncircleFrame.pszImageMR=TEXT("FRAME_MR");
	EncircleFrame.pszImageBL=TEXT("FRAME_BL");
	EncircleFrame.pszImageBM=TEXT("FRAME_BM");
	EncircleFrame.pszImageBR=TEXT("FRAME_BR");
	m_FrameEncircle.InitEncircleResource(EncircleFrame,AfxGetInstanceHandle());

	//������Դ
	tagEncircleResource	EncircleItemFrame;
	EncircleItemFrame.pszImageTL=MAKEINTRESOURCE(IDB_ITEM_FRAME_TL);
	EncircleItemFrame.pszImageTM=MAKEINTRESOURCE(IDB_ITEM_FRAME_TM);
	EncircleItemFrame.pszImageTR=MAKEINTRESOURCE(IDB_ITEM_FRAME_TR);
	EncircleItemFrame.pszImageML=MAKEINTRESOURCE(IDB_ITEM_FRAME_ML);
	EncircleItemFrame.pszImageMR=MAKEINTRESOURCE(IDB_ITEM_FRAME_MR);
	EncircleItemFrame.pszImageBL=MAKEINTRESOURCE(IDB_ITEM_FRAME_BL);
	EncircleItemFrame.pszImageBM=MAKEINTRESOURCE(IDB_ITEM_FRAME_BM);
	EncircleItemFrame.pszImageBR=MAKEINTRESOURCE(IDB_ITEM_FRAME_BR);
	EncircleItemFrame.pszImageGR=MAKEINTRESOURCE(IDB_AFFICHE_GROUP);
	m_ItemFrameEncircle.InitEncircleResource(EncircleItemFrame,AfxGetInstanceHandle());
	m_bCreateType4 = true;

	DeclareDumpFile();

	return;
}

//��������
CPlatformFrame::~CPlatformFrame()
{
	//ƽ̨����
	if(m_pPlatformFrame == NULL)
		return;
	ASSERT(m_pPlatformFrame==this);
	if (m_pPlatformFrame==this)
	{
		//SafeDelete(m_pPlatformFrame);
	}

	return;
}

//�ӿڲ�ѯ
VOID * CPlatformFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}

//��ȡ֪ͨ
VOID CPlatformFrame::OnGameItemFinish()
{
	//Ĭ����ʾ��Ϸ�б�
	//��ʾ�б�
	//m_PlazaViewItem.ShowKindItemView(0);
	m_PlazaViewItem.ShowTypeItemView();

	m_PlazaViewItem.CreateDlgCaipiao(2);
	return;
}

//��ȡ֪ͨ
VOID CPlatformFrame::OnGameKindFinish(WORD wKindID)
{
	return;
}

//����֪ͨ
VOID CPlatformFrame::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	if(m_bCreateType4)
	{
		m_bCreateType4 = false;
		m_PlazaViewItem.CreateDlgCaipiao(1);
	}
	//���봦��
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//��Ϸ����
		{
			//��������
			m_WndGameTypeCtrl.InsertGameType((CGameTypeItem *)pGameListItem);

			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			break;
		}
	case ItemGenre_Server:	//��Ϸ����
		{
			break;
		}
	}

	return;
}

//����֪ͨ
VOID CPlatformFrame::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	return;
}

//ɾ��֪ͨ
VOID CPlatformFrame::OnGameItemDelete(CGameListItem * pGameListItem)
{
	return;
}

//�����¼�
bool CPlatformFrame::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{

	return true;
}
bool CPlatformFrame::CreatePlazaview()
{
	m_PlazaViewItem.CreateDlgCaipiao(0);
	return true;
}
VOID CPlatformFrame::AvtiveMainMissionItem()
{
	if(m_bCreate == false)
	{
		m_bCreate = true;
		//�����㳡
		CRect rcCreate(0,0,0,0);

		m_dlgUserAccount.Create(IDD_USER_ACCOUNT,this);
		m_dlgUserAccount.ShowWindow(SW_HIDE);
		m_dlgHuiyuan.Create(CHuiYuanDlg::IDD,this);
		m_dlgHuiyuan.ShowWindow(SW_HIDE);
		m_dlgChongzhi.Create(CChongZhiDlg::IDD,this);
		m_dlgChongzhi.ShowWindow(SW_HIDE);
		m_dlgQukuan.Create(CQuKuanDlg::IDD,this);
		m_dlgQukuan.ShowWindow(SW_HIDE);
		m_dlgHuodong.Create(CHuoDongDlg::IDD,this);
		m_dlgHuodong.ShowWindow(SW_HIDE);
		m_dlgTouZhu.Create(CZhangHuTzhLogDlg::IDD,this);
		m_dlgTouZhu.ShowWindow(SW_HIDE);
		m_dlgPlatformMessage.Create(CPlatformMessage::IDD,this);
		m_dlgPlatformMessage.ShowWindow(SW_HIDE);
		m_PlazaViewItem.CreateDlgCaipiao(0);

	}

	//��������
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//��������
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{

		m_bShutdowSocket = true;
		return;
	}

	m_bShutdowSocket = false;
	return;
}
VOID CPlatformFrame::SendToServer(int nSendType)
{
	if(!m_bCanSend)
		return;

	if(nSendType == 1)   //��ע��¼����
	{
		CMD_GP_GetNews GetNews;
		ZeroMemory(&GetNews,sizeof(GetNews));

		GetNews.n_t_id = theAccount.user_id;
		GetNews.n_t_ccount = 5;
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_NEWS,&GetNews,sizeof(GetNews));

	}

	if(nSendType == 2)
	{
		//m_bToJinbi = true;
		CMD_GP_ZhuanHuan Zhuanhuan;
		Zhuanhuan.n_t_userid = theAccount.user_id;
		Zhuanhuan.n_t_type = 1;
		Zhuanhuan.f_t_jine = m_fJiaoyiScore;

		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_ZHUANHUAN,&Zhuanhuan,sizeof(Zhuanhuan));

	}

	if(nSendType == 3)
	{
	//	m_bToCaijin = true;
		CMD_GP_ZhuanHuan Zhuanhuan;
		Zhuanhuan.n_t_userid = theAccount.user_id;
		Zhuanhuan.n_t_type = 2;
		Zhuanhuan.f_t_jine = m_fJiaoyiScore;

		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_ZHUANHUAN,&Zhuanhuan,sizeof(Zhuanhuan));

	}

	if(nSendType == 4)
	{
	//	m_bChaxunYue=true;
		CMD_GP_GetLastYue GetLastYue;
		ZeroMemory(&GetLastYue,sizeof(GetLastYue));

		GetLastYue.dwUserID = theAccount.user_id;

		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_LAST_YUE,&GetLastYue,sizeof(GetLastYue));

	}
	if(nSendType == 5)
	{
		//m_bGetTime=true;
		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_TIME);

	}

	if(nSendType == 6)
	{
		m_bCanSend = false;
	//	m_bQuitGame=true;
		CMD_GP_QuitGame QuitGame;
		ZeroMemory(&QuitGame,sizeof(QuitGame));

		QuitGame.dwUserID = theAccount.user_id;
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUIT_GAME,&QuitGame,sizeof(QuitGame));

		m_MissionManager.ConcludeMissionItem(this,false);

	}

	if(nSendType == 7)
	{
		//m_bGetMapBonus=true;
		CMD_GP_GetMyMapBonus GetMyMapBonus;
		ZeroMemory(&GetMyMapBonus,sizeof(GetMyMapBonus));
		GetMyMapBonus.n_t_userid = theAccount.user_id;
		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_MAP_BONUS,&GetMyMapBonus,sizeof(GetMyMapBonus));

	}

	if(nSendType == 8)
	{
		CMD_GP_QueryGameResult QueryGameResult;
		QueryGameResult.wKindID = CZChongQingSSC;

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_GAME_RESULT,&QueryGameResult,sizeof(QueryGameResult));
	}
	if(nSendType == 9)
	{
		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_KEFU_URL);

	}
	if(nSendType == 10)
	{
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_CANADA_START_QIHAO);
			//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_QIHAO_CHA);
	}
	if(nSendType == 11)
	{
		CMD_GP_GetNotic GetNotice;
		ZeroMemory(&GetNotice,sizeof(GetNotice));

		GetNotice.n_t_type = 1;
		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_NOTICE,&GetNotice,sizeof(GetNotice));

	}
	if(nSendType == 12)
	{
		CMD_GP_GetZnxCount GetZnxCount;
		ZeroMemory(&GetZnxCount,sizeof(GetZnxCount));

		GetZnxCount.n_t_userid = theAccount.user_id;
		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_ZNX_COUNT,&GetZnxCount,sizeof(GetZnxCount));
	}
	return;
}
//�ر��¼�
bool CPlatformFrame::OnEventMissionShut(BYTE cbShutReason)
{
	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL && !m_bKillSocket)
	{
		m_bCanSend = false;
		if(bShowInfomation)
		{
			bShowInfomation = false;

			m_btNet.EnableWindow(FALSE);

			SetTimer(TimerReconnect,1000,NULL);

			CString strLog;

			strLog.Format(L"PINGSECOND  MISSIONSEND  CPlatformFrame");
			OutputDebugString(strLog);

// 			if(!m_bLogonFail)
// 			{
// 				m_bLogonFail = false;
// 				m_DlgStatus.ShowStatusWindow(L"������������Զ�̷�����....");
// 			}
		}


	}
// 	else if(m_bKillSocket)
// 	{
// 		OnClose();
// 	}

	return true;
}
//ȡ������
VOID CPlatformFrame::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}



UINT CPlatformFrame::GetLuckNumber(LPVOID lpParam)
{
	return 1;
	//����ʱʱ�ʿ�����
	CString xmlurl = _T("http://www.cailele.com/static/ssc/newlyopenlist.xml");
	CString strPath;  //�ļ�·��
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	int pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\cqssc.xml"); 
	DeleteUrlCacheEntry(xmlurl);										//������
	if (FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)))
	{
		//OutputDebugString(strPath);
	}

	//����ʱʱ�ʿ�����
	xmlurl = _T("http://www.cailele.com/static/jxssc/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\jxssc.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//������
	if (FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)))
	{
		//OutputDebugString(strPath);
	}

 	xmlurl = _T("http://www.xjflcp.com/servlet/sscSaleEnd?flag=openPrize");
 	pos = strPath.ReverseFind(_T('\\')); 
 	strPath = strPath.Left(pos)+ _T("\\xjssc.xml"); 
 	DeleteUrlCacheEntry(xmlurl);											//������
 	if (FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)))
 	{
 		//OutputDebugString(strPath);
 	}

	//�㶫11ѡ5
	xmlurl = _T("http://www.cailele.com/static/gd11x5/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\gd11x5.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//������
	if(FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)) )
	{
		//OutputDebugString(strPath);
	}

	//����11ѡ5
	xmlurl = _T("http://www.cailele.com/static/cq11x5/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\cq11x5.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//������
	if(FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)) )
	{
		//OutputDebugString(strPath);
	}

	//����11ѡ5
	xmlurl = _T("http://www.cailele.com/static/jxdlc/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\jxdlc.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//������
	if(FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)) )
	{
		//OutputDebugString(strPath);
	}

	//11��
	xmlurl = _T("http://www.cailele.com/static/11yun/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\11yun.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//������
	if(FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)) )
	{
		//OutputDebugString(strPath);
	}
	return 0;
}


bool CPlatformFrame::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if(Command.wMainCmdID == MDM_GP_USER_SERVICE)
	{
		bShowInfomation=true;
		switch (Command.wSubCmdID)
		{
		case SUB_GP_SEND_MESSAGE_RET:
			{
				if(m_dlgPlatformMessage.m_dlgSendMessage.IsWindowVisible())
					m_dlgPlatformMessage.m_dlgSendMessage.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_ZNX_COUNT_RET:
			{
				ASSERT(wDataSize==sizeof(CMD_GP_GetZnxCountRet));
				if(wDataSize!=sizeof(CMD_GP_GetZnxCountRet))
					return false;

				CMD_GP_GetZnxCountRet* pLogCount = (CMD_GP_GetZnxCountRet*)pData;
				int cnt = pLogCount->n_t_count;
				HINSTANCE hInstance=AfxGetInstanceHandle();
				if(cnt >0)
				{
					m_btPlazaMessage.SetButtonImage(IDB_BK_MESSAGE1,hInstance,true,false);
				}
				else
				{
					m_btPlazaMessage.SetButtonImage(IDB_BK_MESSAGE,hInstance,true,false);

				}
				return true;
			}
		case SUB_GP_GET_NOTICE_RET:
			{
				ASSERT(wDataSize%sizeof(CMD_GP_GetNoticRet)==0);
				if(wDataSize%sizeof(CMD_GP_GetNoticRet)!=0)
					 return false;

				int nCount = wDataSize/sizeof(CMD_GP_GetNoticRet);
				for (int i = 0;i < nCount;i++)
				{
					CMD_GP_GetNoticRet* pLogRet = ((CMD_GP_GetNoticRet*)pData+i);

					CTime t(pLogRet->n_t_time);
// 					CString strLog;
// 					strLog.Format(L"NOTICE title:%s,content:%s,time:%s,type:%d",pLogRet->s_t_title,pLogRet->s_t_content,t.Format(L"%Y%m%d %H:%M:%S"),pLogRet->n_t_type);
// 					OutputDebugString(strLog);
				}
				return true;
			}
		case SUB_GP_KILL_SOCKET:
			{
				CMD_GP_KillSocket* pKillSocket = (CMD_GP_KillSocket*)pData;

				m_bKillSocket = true;
				CInformation Information(this);
				if(Information.ShowMessageBox(pKillSocket->szMessage) == IDOK)
				{
					//SetTimer(55555,2000,NULL);
					//OnClose();
					exit(0);
					//SendMessage(WM_CLOSE);
				}

				return true;
			}
		case SUB_GR_GET_HYXX_LOG_COUNT_RET: //��Ա��Ϣ
		case SUB_GP_GET_HYXX_LOG_RET:
		case SUB_GR_XG_HY_FANDIAN_RET:
		case SUB_GR_SW_DAILI_RET:
		case SUB_GR_HYXX_ZHUANZHANG_RET:
		case SUB_GR_GET_XJ_Peie_RET:
		case SUB_GR_SET_XJ_Peie_RET:
			{
				if(m_dlgHuiyuan.m_hyxxDlg.IsWindowVisible())
					m_dlgHuiyuan.m_hyxxDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_SEND_CHECKYANZHENGMA_TRANS_RET:	//��֤��
		case SUB_GP_GET_PHONE_TRASFER_FENHONG_RET:
			{
				if(m_dlgHuiyuan.m_hyxxDlg.IsWindowVisible())
					m_dlgHuiyuan.m_hyxxDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_PARENT_RET:
			{
				if(m_dlgHuiyuan.m_hyxxDlg.IsWindowVisible())
					m_dlgHuiyuan.m_hyxxDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				else if(m_dlgHuiyuan.m_xjtzhtjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_xjtzhtjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				else if(m_dlgHuiyuan.m_yktjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_yktjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				else if(m_dlgHuiyuan.m_yxtjlDlg.IsWindowVisible())
					m_dlgHuiyuan.m_yxtjlDlg.OnEventMissionRead(Command,  pData,  wDataSize);

				return true;
			}
		case SUB_GR_GET_REG_URL_RET: //��Ա����¼�
		case SUB_GR_ADD_HY_RET:
		case SUB_GR_SET_WEB_FANDIAN_RET:
		case SUB_GR_GET_Peie_RET:
			{
				if(m_dlgHuiyuan.m_tjxjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_tjxjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_XJTZH_LOG_COUNT_RET: //��Ա�¼�Ͷעͳ��
		case SUB_GP_GET_XJTZH_LOG_RET:
			{
				if(m_dlgHuiyuan.m_xjtzhtjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_xjtzhtjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_CHK_XJTZH_LOG_COUNT_RET: //��Ա�¼�Ͷע��¼
		case SUB_GP_CHK_XJTZH_LOG_RET:
			{
				if(m_dlgHuiyuan.m_chkxjtzhDlg.IsWindowVisible())
					m_dlgHuiyuan.m_chkxjtzhDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_HYSHJ_RET: //��Ա����
			{
				if(m_dlgHuiyuan.m_hyshjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_hyshjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_XJYK_TJ_COUNT_RET: //�¼�ӯ��ͳ��
		case SUB_GP_GET_XJYK_TJ_RET:
			{
				if(m_dlgHuiyuan.m_yktjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_yktjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
		
			}
		case SUB_GR_GET_XJYK_LOG_COUNT_RET: //�¼�ӯ����ϸ
		case SUB_GP_GET_XJYK_LOG_RET:
			{
				if(m_dlgHuiyuan.m_yklogDlg.IsWindowVisible())
					m_dlgHuiyuan.m_yklogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_XJCHZH_LOG_COUNT_RET: //�¼���ֵ��־
		case SUB_GP_GET_XJCHZH_LOG_RET:
			{
				if(m_dlgHuiyuan.m_chzhlogDlg.IsWindowVisible())
					m_dlgHuiyuan.m_chzhlogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_XJTX_LOG_COUNT_RET: // ��Ա�¼�����
		case SUB_GP_GET_XJTX_LOG_RET:
			{
				if(m_dlgHuiyuan.m_txlogDlg.IsWindowVisible())
					m_dlgHuiyuan.m_txlogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_QIPAI_KIND_RET:
			{
				if(m_dlgHuiyuan.m_xjyxjlDlg.IsWindowVisible())
					m_dlgHuiyuan.m_xjyxjlDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				else if(m_dlgUserAccount.m_qpykdlg.IsWindowVisible())
					m_dlgUserAccount.m_qpykdlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_QP_YINGKUI_COUNT_RET: //��Ա��Ϸͳ��
		case SUB_GP_GET_QP_YINGKUI_RET:
			{
				if(m_dlgHuiyuan.m_xjyxjlDlg.IsWindowVisible())
					m_dlgHuiyuan.m_xjyxjlDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				else if(m_dlgUserAccount.m_qpykdlg.IsWindowVisible())
					m_dlgUserAccount.m_qpykdlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_XJYX_TJ_COUNT_RET:  //��Ա�¼���Ϸ��¼
		case SUB_GP_GET_XJYX_TJ_RET:
			{
				if(m_dlgHuiyuan.m_yxtjlDlg.IsWindowVisible())
					m_dlgHuiyuan.m_yxtjlDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_CHONG_ZHI_TYPE_RET: //��ֵ
			{
				if(m_dlgChongzhi.IsWindowVisible())
					m_dlgChongzhi.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_QU_KUAN_INFO_RET: //ȡ��
		case SUB_GP_DO_QU_KUAN_RET:
		case SUB_GP_GET_QUKUAN_LIMIT_RET:
			{
				if(m_dlgQukuan.IsWindowVisible())
					m_dlgQukuan.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_USER_QIANDAO_RET:     //�ǩ��
		case SUB_GP_GET_USER_HUANLESONG_RET:
		case SUB_GP_GET_LING_JIANG_RET:
			{
				if(m_dlgHuodong.m_huanlesong.IsWindowVisible())
					m_dlgHuodong.m_huanlesong.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_DAILI_HUIKUI_RET:
		case SUB_GP_DAILI_LJ_RET:
			{
				if(m_dlgHuodong.m_daili.IsWindowVisible())
					m_dlgHuodong.m_daili.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_LUCKY_ZHUAN_RET: //���ת��
		case SUB_GP_CAN_LUCKY_ZHUAN_RET:
			{
				if(m_dlgHuodong.m_zhuanzhuan.IsWindowVisible())
					m_dlgHuodong.m_zhuanzhuan.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_USER_INFO_RET: //�˻�������Ϣ
		case SUB_GR_XG_QQ_RET:
		case SUB_GP_ALTER_GENDER_RET:
			{
				if(m_dlgUserAccount.m_baseInfoDlg.IsWindowVisible())
					m_dlgUserAccount.m_baseInfoDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_XG_LOGIN_PASS_RET://�˻��޸�����
			{
				ASSERT(wDataSize == sizeof(CMD_GR_XG_Loginpass_RET));
				if(wDataSize!=sizeof(CMD_GR_XG_Loginpass_RET))
					return false;

				CMD_GR_XG_Loginpass_RET* pCountRet = (CMD_GR_XG_Loginpass_RET*)pData;

				int lResult = pCountRet->lResult;
				if(lResult == 0)
				{
					CString msg;
					msg.Format(_T("���Ѿ��ɹ��޸����룬�����µ�¼��"));
					CInformation Information;
					if(Information.ShowMessageBox(msg,MB_ICONINFORMATION,30)==1)
					{
						//ȫ������
						CGlobalUnits::GetInstance()->DeleteUserCookie();
						CGlobalUserInfo::GetInstance()->ResetUserInfoData();

						m_PlazaViewItem.ShowWindow(SW_HIDE);
						SendToServer(6);
						//�����¼�
						CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
						if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

					}


				}
				else if (lResult == 1)
				{
					MyMessageBox(_T("�����벻��ȷ"));
					return true;
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("�޸�����ʧ�ܣ�������"));
					return true;
				}

				return true;
			}
		case SUB_GR_XG_QUKUAN_PASS_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_XG_Qukuanpass_RET));
				if(wDataSize!=sizeof(CMD_GR_XG_Qukuanpass_RET))
					return false;

				CMD_GR_XG_Qukuanpass_RET* pXGqqRet = (CMD_GR_XG_Qukuanpass_RET*)pData;

				int lResult = pXGqqRet->lResult;
				if(lResult == 0)
				{
					CString msg;
					msg.Format(_T("���Ѿ��ɹ��޸�����"));
					CInformation Information;
					if(Information.ShowMessageBox(msg,MB_ICONINFORMATION,30)==1)
					{
						//ȫ������
						CGlobalUnits::GetInstance()->DeleteUserCookie();
						CGlobalUserInfo::GetInstance()->ResetUserInfoData();

						m_PlazaViewItem.ShowWindow(SW_HIDE);

						SendToServer(6);
						//�����¼�
						CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
						if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

					}

				}
				else if (lResult == 1)
				{
					MyMessageBox(_T("�����벻��ȷ"));
					return true;
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("�޸�����ʧ�ܣ�������"));
					return true;
				}
				return true;

			}
		case SUB_GR_BIND_PHONE_RET:
		case SUB_GR_LOCK_MACHINE_RET:
		case SUB_GR_BIND_PHONE_INFO_RET:
		case SUB_GR_UNBIND_PHONE_RET:
			{
				if(m_dlgUserAccount.m_xgmmDlg.IsWindowVisible())
					m_dlgUserAccount.m_xgmmDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_QUERY_YINHANG_RET:			//�����˻����
		case SUB_GR_QUERY_MY_YINHANG_RET:
		case SUB_GR_SET_QUKUAN_ZHANGHAO_RET:
			{
				if(m_dlgUserAccount.m_shzhTkzhh.IsWindowVisible())
					m_dlgUserAccount.m_shzhTkzhh.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_QUERY_MY_PROTECT_RET://�����˻�����
		case SUB_GR_SET_QUKUAN_PROTECT_RET:
			{
				if(m_dlgUserAccount.m_zhhbhDlg.IsWindowVisible())
					m_dlgUserAccount.m_zhhbhDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_TOUZHU_LOG_COUNT_RET: //�˻�Ͷע��¼
		case SUB_GP_GET_TOUZHU_LOG_RET:
		case SUB_GR_CANCEL_TOUZHU_RET:
			{
				if(m_dlgTouZhu.IsWindowVisible())
				{
					m_dlgTouZhu.OnEventMissionRead(Command,  pData,  wDataSize);
				}
				else if(m_dlgUserAccount.m_tzhlogDlg.IsWindowVisible())
					m_dlgUserAccount.m_tzhlogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GP_GET_INBOX_MESSAGE_RET:
		case SUB_GP_DEL_MESSAGE_RET:
			{
				if(m_dlgPlatformMessage.IsWindowVisible())
				{
					if(m_dlgPlatformMessage.m_dlgInbox.IsWindowVisible())
						m_dlgPlatformMessage.m_dlgInbox.OnEventMissionRead(Command,  pData,  wDataSize);
					if(m_dlgPlatformMessage.m_dlgOutbox.IsWindowVisible())
						m_dlgPlatformMessage.m_dlgOutbox.OnEventMissionRead(Command,  pData,  wDataSize);
					if(m_dlgPlatformMessage.m_dlgSendMessage.IsWindowVisible())
						m_dlgPlatformMessage.m_dlgSendMessage.OnEventMissionRead(Command,  pData,  wDataSize);
				}
				return true;
			}
		case SUB_GP_GET_ALL_USERINFO_RET:
			{
				if(m_dlgPlatformMessage.IsWindowVisible())
				{
					m_dlgPlatformMessage.m_dlgSendMessage.OnEventMissionRead(Command,  pData,  wDataSize);
				}
				return true;
			}
		case SUB_GR_GET_TIXIAN_LOG_COUNT_RET: //�˻�����
		case SUB_GP_GET_TIXIAN_LOG_RET:
			{
				if(m_dlgUserAccount.m_txlogDlg.IsWindowVisible())
					m_dlgUserAccount.m_txlogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_CHONGZHI_LOG_COUNT_RET: //�˻���ֵ
		case SUB_GP_GET_CHONGZHI_LOG_RET:
			{
				if(m_dlgUserAccount.m_chzhlogdlg.IsWindowVisible())
					m_dlgUserAccount.m_chzhlogdlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_YINGKUI_LOG_COUNT_RET: //�˻�ӯ��
		case SUB_GP_GET_YINGKUI_LOG_RET:
			{
				if(m_dlgUserAccount.m_yklogdlg.IsWindowVisible())
					m_dlgUserAccount.m_yklogdlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_YINGKUI_MX_COUNT_RET: //�˻�ӯ����ϸ
		case SUB_GP_GET_YINGKUI_MX_RET:
			{
				if(m_dlgUserAccount.m_ykmxdlg.IsWindowVisible())
					m_dlgUserAccount.m_ykmxdlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_QIHAO_CHA_RET:
			{
				if(m_PlazaViewItem.IsWindowVisible())
					m_PlazaViewItem.OnEventMissionRead(Command,  pData,  wDataSize);
				if(m_dlgTouZhu.IsWindowVisible())
				{
					m_dlgTouZhu.OnEventMissionRead(Command,  pData,  wDataSize);
				}
				else if(m_dlgUserAccount.m_tzhlogDlg.IsWindowVisible())
					m_dlgUserAccount.m_tzhlogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_USER_FANDIAN_RET: //��Ϸ��ȡ����
		case SUB_GP_TOUZHU_CONTINUE_RET:
		case SUB_GP_SET_USER_BONUS_RET:
		case SUB_GP_TOUZHU_CQSSC_RET:
		case SUB_GP_GET_MORE_RECORD_RET:
		case SUB_GR_GET_LHC_QIHAO_RET:
		case SUB_GP_QUERY_STATUS_LOTTERY_RET:
			{
				if(m_PlazaViewItem.IsWindowVisible())
					m_PlazaViewItem.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_CANADA_START_QIHAO_RET:
			{
				if(m_PlazaViewItem.IsWindowVisible())
					m_PlazaViewItem.OnEventMissionRead(Command,  pData,  wDataSize);

				ASSERT((wDataSize == sizeof(CMD_GP_GetCanadaQihaoRet)));
				if((wDataSize != sizeof(CMD_GP_GetCanadaQihaoRet)) )return false;

				CMD_GP_GetCanadaQihaoRet* pLogRet = (CMD_GP_GetCanadaQihaoRet*)pData;

				theCanadaQihao = pLogRet->n_t_start_qihao;
				theCanadaStartTime = pLogRet->n_t_start_time;

				return true;
			}
		case SUB_GP_GET_LAST_YUE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLastYueRet));
				if(wDataSize != sizeof(CMD_GP_GetLastYueRet)) return false;

				CMD_GP_GetLastYueRet* pGetYueRet = (CMD_GP_GetLastYueRet*)pData;

				theAccount.yue = pGetYueRet->f_t_yue;
				theAccount.dongjie = pGetYueRet->f_t_dongjie;

				//�û���Ϣ
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				pGlobalUserData->lScore = pGetYueRet->f_t_score;

				OnUpDateAccoount(0,0);
				return true;
			}
		case SUB_GP_GET_TOUZHU_TISHI_RET:
			{
				m_tishiDlg.OnEventMissionRead(Command,  pData,  wDataSize);
// 
// 				ASSERT((wDataSize% sizeof(CMD_GR_TouzhuTishiRet))==0);
// 				if(( wDataSize% sizeof(CMD_GR_TouzhuTishiRet))!=0) return false;
// 
// 				int nCount = wDataSize/sizeof(CMD_GR_TixianTishiRet);
// 
// 				bool bFresh = false;
// 
// 				for (int i = 0;i < nCount;i++)
// 				{
// 					CMD_GR_TouzhuTishiRet* pTouzhuTishiRet = ((CMD_GR_TouzhuTishiRet*)pData+i);
// 
// 					CString strTemp;
// 					strTemp.Format(L"%s", pTouzhuTishiRet->szQishu);
// 					if(strTemp.IsEmpty())
// 						continue;
// 
// 					CString msg;
// 					if(pTouzhuTishiRet->nWinZhushu <= 0) {
// 						msg.Format(_T("û�н�������Ŭ����\n%s\n%s��\nӯ��:%.2lf"), theGameType[pTouzhuTishiRet->nType], pTouzhuTishiRet->szQishu,pTouzhuTishiRet->f_yingkui);
// 					} else {
// 						bFresh = true;
// 
// 						msg.Format(_T("Ͷע����:%s\n\nͶע�ں�:%s\n\nӯ��:%.2lf"), theGameType[pTouzhuTishiRet->nType], pTouzhuTishiRet->szQishu,pTouzhuTishiRet->f_yingkui);
// 					}
// 
// 					m_tishiDlg.SetTiShiMessage(msg);
// 				}
				return true;

			}

		case SUB_GP_GET_MAP_BONUS_RET:
			{

				ASSERT(wDataSize % sizeof(CMD_GR_GetMapBonusRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetMapBonusRet)!=0) return false;

				int nCount = wDataSize/sizeof(CMD_GR_GetMapBonusRet);
				for(int i = 0;i < nCount;i++)
				{
					CMD_GR_GetMapBonusRet* pGetMapBonus = ((CMD_GR_GetMapBonusRet*)pData+i);

					int index = (pGetMapBonus->n_t_type_id << 16) | (unsigned short)pGetMapBonus->n_t_kind_id;
					
					theBonus[index] = pGetMapBonus->f_t_bonus;

				}
				return true;
			}
		case SUB_GP_GET_NEWS_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GP_GetNewsRet)==0);
				if(wDataSize % sizeof(CMD_GP_GetNewsRet)!=0) return false;

				m_Grid.DeleteAllItems();
				m_vecTitle.clear();
				int nCount = wDataSize/sizeof(CMD_GP_GetNewsRet);
				CString strTemp;

				for(int i =0;i < nCount;i++)
				{
					CMD_GP_GetNewsRet* pNewsRet = ((CMD_GP_GetNewsRet*)pData+i);
					m_vecTitle.push_back(pNewsRet->s_t_news);

				}

				//SetTimer(TimerNews, 50, NULL);	
				CString strText;
				//pDC->SetTextColor(RGB(50,50,50));
				for(unsigned int n=0; n<m_vecTitle.size(); n++)
				{
					strText += m_vecTitle[n].c_str();
					//strText += L"\r\n";
				}
				m_WndTopNews.SetTopNews(strText);

				int nLength = strText.GetLength()/15;
				m_nNewsHeight = nLength*17;
				CRect rcClient;
				GetClientRect(rcClient);
				m_nTop = rcClient.Height() - m_nNewsHeight-300;
				CRect Rct(16,362,235,362+400);

				if(m_bGame)
					InvalidateRect(&Rct);

				GridCtrlInit();
				return true;
			}
		case SUB_GP_QUERY_TIME_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetSystemTime));
				if(wDataSize != sizeof(CMD_GR_GetSystemTime)) return false;

				CMD_GR_GetSystemTime* pSystemTime = (CMD_GR_GetSystemTime*)pData;
				theTimeCS.Lock();
				CTime servertime(pSystemTime->ServerTime);
				theTime = servertime;
				theTimeCS.Unlock();

				return true;
			}
		case SUB_GR_KEFU_URL_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GetKefuUrlRet));
				if(wDataSize != sizeof(CMD_GetKefuUrlRet)) return false;

				CMD_GetKefuUrlRet* pGetKefuUrlRet = (CMD_GetKefuUrlRet*)pData;

				if(pGetKefuUrlRet->n_t_result == 0)
				{
					ShellExecute(NULL,TEXT("OPEN"),pGetKefuUrlRet->s_t_desc,NULL,NULL,SW_NORMAL);
				}
				return true;
			}
		case SUB_GP_ZHUANHUAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_ZhuanHuanRet));
				if(wDataSize != sizeof(CMD_GP_ZhuanHuanRet)) return false;

				CMD_GP_ZhuanHuanRet* pZhuanhuanRet = (CMD_GP_ZhuanHuanRet*)pData;

				if(pZhuanhuanRet->nResult == 0)
				{
					theAccount.yue = pZhuanhuanRet->f_t_yue;
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					pGlobalUserData->lScore = pZhuanhuanRet->f_t_score;

					OnUpDateAccoount(0,0);
					MyMessageBox(L"ת���ɹ�,�����");
				}
				else if(pZhuanhuanRet->nResult == 1)
				{
					MyMessageBox(L"���㣬ת��ʧ��");
				}
				else if(pZhuanhuanRet->nResult == 2)
				{
					MyMessageBox(L"ת��ʧ��,�˻���Ϣ������");
				}
				else if(pZhuanhuanRet->nResult == 3)
				{
					MyMessageBox(L"������������Ϸ�У����˳����ٽ���ת����");
				}
				return true;
			}
		case SUB_GP_QUERY_GAME_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GP_QueryLotResult)==0);
				if(wDataSize % sizeof(CMD_GP_QueryLotResult)!=0)
					return false;

				int nCount = wDataSize/sizeof(CMD_GP_QueryLotResult);
				CMD_GP_QueryLotResult* pQueryNumResult = (CMD_GP_QueryLotResult*) pData;
				int nIndex=0;
				int nFirstTypeID = pQueryNumResult->wKindID;

				for(int nn=0; nn<nCount; nn++)
				{
					CMD_GP_QueryLotResult* pResult  = pQueryNumResult + nn;
					int nTypeid = pResult->wKindID;
					if(nTypeid != nFirstTypeID)
					{
						nIndex=0;
						nFirstTypeID = nTypeid;
					}

					m_PlazaViewItem.AddOpenResult(pResult, nIndex);
					nIndex++;
				}

				return true;
				//����д�ļ������飻ֱ�Ӵ��ݸ�����
				if(0)
				{
					CString strPath;  //�ļ�·��
					GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
					strPath.ReleaseBuffer(); 
					int pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\cqssc.xml"); 

					DeleteFile(strPath);
 					pos = strPath.ReverseFind(_T('\\')); 
 					strPath = strPath.Left(pos)+ _T("\\jxssc.xml"); 
 					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\xjssc.xml"); 
					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\fenfencai.xml"); 
					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\wufencai.xml"); 
					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\gd11x5.xml"); 
					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\cq11x5.xml"); 
					DeleteFile(strPath);
					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\11yun.xml"); 
					DeleteFile(strPath);
					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\jxdlc.xml"); 
					DeleteFile(strPath);

					m_xml.SetDoc(L"<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n");
					m_xml.AddElem(L"xml");

					WORD wCount = wDataSize/sizeof(CMD_GP_QueryLotResult);

					CMD_GP_QueryLotResult* pQueryLotResult  =  ((CMD_GP_QueryLotResult*)pData);
					int nFirstTypeID = pQueryLotResult->wKindID;
					for(int i = 0;i < wCount;i++)
					{
						CMD_GP_QueryLotResult* pQueryLotResult  =  ((CMD_GP_QueryLotResult*)pData+i);


						int nTypeid = pQueryLotResult->wKindID;

						if(nTypeid != nFirstTypeID)
						{
							m_xml.RemoveChildElem();
							m_xml.SetDoc(L"<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n");
							m_xml.AddElem(L"xml");

							nFirstTypeID = nTypeid;
						}
						switch(nTypeid)
						{
						case CZChongQingSSC:
							{
								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"cqssc.xml");

								m_xml.OutOfElem();
								break;
							}
 						case CZJiangXiSSC:
 							{
								
 								CString strTmp;
 								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
 								m_xml.AddChildElem(strTmp);
 								m_xml.IntoElem();						
	 
 								m_xml.Save(L"jxssc.xml");
	 
 								m_xml.OutOfElem();
 								break;
 							}
						case CZXinJiangSSC:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"xjssc.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZ_FENFEN_CAI:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"fenfencai.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZ_WUFEN_CAI:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"wufencai.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZGD11Xuan5:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"gd11x5.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZCQ11Xuan5:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"cq11x5.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZSD11Xuan5:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"11yun.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZJX11Xuan5:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"jxdlc.xml");

								m_xml.OutOfElem();
								break;
							}
						default:
							break;
						}
					}


				}//end of if(0)
				return true;
			}

		}

	}

	return true;
}
//��Ϣ����
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}
 #include <Sensapi.h>
 #pragma comment(lib, "Sensapi.lib")
void CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(TimerNews == nIDEvent)
	{
		m_nNewsYPos -= 290;
		m_nreduce += 1;
		KillTimer(TimerNews);
		CRect Rct(16,362,235,362+400);
		if(m_bGame)
			InvalidateRect(&Rct);

	}
	else if(TimerLuckyNum == nIDEvent)
	{

		SendToServer(8);
		//SendToServer(11);
	}
	else if(TimerNews1 == nIDEvent)
	{
		//SetTimer(TimerNews,50,NULL);
	}
	else if (TimerZhunBeiData == nIDEvent)
	{
		SendToServer(1);
		SendToServer(5);
		//��ȡ��������
		//AfxBeginThread(GetLuckNumber, this);
	}
	else if(TimerJiShi == nIDEvent)
	{
			theTimeCS.Lock();
			theTime += CTimeSpan(0, 0, 0, 1);
			theTimeCS.Unlock();
			DWORD dw;

			if(!IsNetworkAlive(&dw))
			{
				if(m_bShowNetAlive)
				{
					m_bShowNetAlive=false;
					DWORD const e = GetLastError();
					m_btNet.EnableWindow(FALSE);

					SetTimer(TimerReconnect,1000,NULL);


					if(!m_bLogonFail)
					{
						m_bLogonFail = true;
						m_DlgStatus.ShowStatusWindow(L"������������Զ�̷�����....");
					}

					if( e == ERROR_SUCCESS)

					{

						CInformation Information(this);
						Information.ShowMessageBox(L"û����������!",MB_ICONWARNING,4);


					}

					else
					{
						CInformation Information(this);
						Information.ShowMessageBox(L"��������ʧ��!",MB_ICONWARNING,4);



					}

				}
			}
			else 
			{
				m_bLogonFail=false;
				if(!m_bShowNetAlive)
				{
					m_DlgStatus.HideStatusWindow();
					m_bShowNetAlive=true;
					CInformation Information(this);
					Information.ShowMessageBox(L"����������",MB_ICONWARNING,4);

				}

			}


	}
	else if(55555 == nIDEvent)
	{
		KillTimer(nIDEvent);
		OnClose();
	}
	else if(TimerZnx == nIDEvent)
	{
		SendToServer(12);
	}
	else if(TimerReconnect == nIDEvent)
	{
		KillTimer(TimerReconnect);
		//��������
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,false)==true)
		{
			m_btNet.EnableWindow(TRUE);
			m_DlgStatus.HideStatusWindow();
			KillTimer(TimerReconnect);
			m_bCanSend = true;
			return ;
		}
		else
		{
			CMissionManager * pMissionManager=GetMissionManager();
			pMissionManager->ConcludeMissionItem(this,false);

		}
	}

	__super::OnTimer(nIDEvent);
}
LRESULT CPlatformFrame::ReturnTouzhu(WPARAM wParam, LPARAM lParam)
{
	OnRefreshYue(0,0);
	//m_WndBar.ShowWindow(SW_HIDE);
	m_PlazaViewItem.ShowWindow(SW_SHOW);

	m_cbShowNewsType = 0;
	CRect rcClient;
	GetClientRect(rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());
	m_dlgUserAccount.ShowWindow(SW_HIDE);
	m_dlgHuiyuan.ShowWindow(SW_HIDE);
	m_WndUserInfoCtrl.ShowWindow(SW_SHOW);
	//m_Publicizeurl.ShowWindow(SW_SHOW);
	m_Grid.ShowWindow(SW_HIDE);
	m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
	m_dlgChongzhi.ShowWindow(SW_HIDE);
	m_dlgQukuan.ShowWindow(SW_HIDE);
	m_dlgHuodong.ShowWindow(SW_HIDE);
	m_dlgTouZhu.ShowWindow(SW_HIDE);
	m_dlgPlatformMessage.ShowWindow(SW_HIDE);
	m_bGame=true;
	CRect Rct(16,362,235,362+400);
	if(m_bGame)
		InvalidateRect(&Rct);

	m_PlazaViewItem.ReturnTouzhu();
	return 1;
}
LRESULT CPlatformFrame::ReleaseFace(WPARAM wParam, LPARAM lParam)
{
	m_WndUserInfoCtrl.ReleaseFace();
	return true;
}
//�����
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_NOTICE:
		{
			TCHAR szTodayURL[126]={0};
			//����Ŀ¼
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFileName[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

			//��ȡ����
			CWHIniData IniData;
			IniData.SetIniFilePath(szFileName);

			IniData.ReadEncryptString(TEXT("PlazaUrl"),TEXT("PlazaNotice"),TEXT("http://192.168.0.105/WinFromBanner/notices.aspx"),szTodayURL,CountArray(szTodayURL));

			CDlgBrowser dlgBrowser;
			dlgBrowser.CreateBrowserControl(TEXT("���¹���"),szTodayURL,CSize(802,503));

			return true;
		}
	case IDC_HOME:
		{
			OnRefreshYue(0,0);
			m_PlazaViewItem.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 0;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_PlazaViewItem.ShowTypeItemView();
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_SHOW);
			//m_Publicizeurl.ShowWindow(SW_SHOW);
			m_Grid.ShowWindow(SW_HIDE);
			//m_WndGameTypeCtrl.ShowWindow(SW_SHOW);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_HIDE);
			m_dlgQukuan.ShowWindow(SW_HIDE);
			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_HIDE);
			m_dlgPlatformMessage.ShowWindow(SW_HIDE);
			m_bGame=true;
			m_PlazaViewItem.ResetRecordTypeKind();
			CRect Rct(16,362,235,362+400);
			ReturnTouzhu(1,1);
			if(m_bGame)
				InvalidateRect(&Rct);

			return true;
		}
	case IDC_MIN:				//��С��ť
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_CLOSE:				//�رհ�ť
		{
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
		}
	case IDC_SOUND:				//������ť
		{
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_bSound = !m_bSound;
			//��ȡ�����ļ�
			//����Ŀ¼
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFileName[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
			TCHAR szTodayURL[124] = L"";
			_sntprintf(szTodayURL,CountArray(szTodayURL),TEXT("%d"),m_bSound);
			WritePrivateProfileString(TEXT("SOUND"),TEXT("IsPlay"),szTodayURL,szFileName);
			if(m_bSound)
			{
				m_btSound.SetButtonImage(IDB_BT_SOUND,hInstance,true,false);
			}
			else
			{
				m_btSound.SetButtonImage(IDB_BT_MUTE,hInstance,true,false);
			}


			return TRUE;
		}
	case IDC_NET:
		{
			return TRUE;
		}
	case IDC_ZHUANHUAN_JINBI:
		{
			OnZhuanhuanJinbi(wParam,lParam);
			return TRUE;
		}
	case IDC_ZHUANHUAN_CAIBI:
		{
			OnZhuanhuanCaibi(wParam,lParam);
			return TRUE;
		}
	case IDC_NAVIGATION+0:
		{
			if(m_dwTickCount == 0)
			{
				m_dwTickCount = GetTickCount();
			}
			else if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"���������Ƶ�������Ե�Ƭ�̣�");
				return true;
			}


			m_PlazaViewItem.ShowWindow(SW_HIDE);
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			//m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 2;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_dlgQukuan.ShowWindow(SW_HIDE);
			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_HIDE);
			m_dlgPlatformMessage.ShowWindow(SW_HIDE);

			m_bGame=false;

			return TRUE;

		}
	case IDC_NAVIGATION+1:
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"���������Ƶ�������Ե�Ƭ�̣�");
				return true;
			}


			m_PlazaViewItem.ShowWindow(SW_HIDE);
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			//m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_HIDE);
			m_dlgQukuan.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 2;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_HIDE);
			m_dlgPlatformMessage.ShowWindow(SW_HIDE);

			m_bGame=false;
			return TRUE;
		
		}
	case IDC_NAVIGATION+2:
		{
// 			if( GetTickCount()-m_dwTickCount <100)
// 			{
// 				MyMessageBox(L"���������Ƶ�������Ե�Ƭ�̣�");
// 				return true;
// 			}
 			m_PlazaViewItem.ShowWindow(SW_HIDE);
 			m_dlgUserAccount.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 1;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

 			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			//m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
 			m_dlgHuiyuan.ShowWindow(SW_HIDE);
 			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
 			m_dlgChongzhi.ShowWindow(SW_HIDE);
 			m_dlgQukuan.ShowWindow(SW_HIDE);
 			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_HIDE);
			m_dlgPlatformMessage.ShowWindow(SW_HIDE);

			m_bGame=false;
			
			return TRUE;
		
		}
	case IDC_NAVIGATION+3:
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"���������Ƶ�������Ե�Ƭ�̣�");
				return true;
			}

 			m_PlazaViewItem.ShowWindow(SW_HIDE);
 			m_dlgUserAccount.ShowWindow(SW_HIDE);
 			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			//m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
 			m_dlgHuiyuan.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 1;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

 			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
 			m_dlgChongzhi.ShowWindow(SW_HIDE);
 			m_dlgQukuan.ShowWindow(SW_HIDE);
 			m_dlgHuodong.ShowWindow(SW_HIDE);
 
			m_dlgTouZhu.ShowWindow(SW_HIDE);
			m_dlgPlatformMessage.ShowWindow(SW_HIDE);

			m_bGame=false;
			return TRUE;
		}
	case IDC_NAVIGATION+4:
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"���������Ƶ�������Ե�Ƭ�̣�");
				return true;
			}

			m_PlazaViewItem.ShowWindow(SW_HIDE);
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			//m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_HIDE);
			m_dlgQukuan.ShowWindow(SW_HIDE);
			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_SHOW);
			m_dlgPlatformMessage.ShowWindow(SW_HIDE);

			m_cbShowNewsType = 2;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_bGame=false;
			return TRUE;
		}
	case IDC_NAVIGATION+5:		//������ť
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"���������Ƶ�������Ե�Ƭ�̣�");
				return true;
			}

			m_PlazaViewItem.ShowWindow(SW_HIDE);
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			//m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_HIDE);
			m_dlgQukuan.ShowWindow(SW_HIDE);
			m_dlgHuodong.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 1;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_dlgTouZhu.ShowWindow(SW_HIDE);
			m_dlgPlatformMessage.ShowWindow(SW_HIDE);

			m_bGame=false;
			return TRUE;
			//��ȡָ��
			CGlobalUnits * pGlobalUnits = (CGlobalUnits *)CGlobalUnits::GetInstance();
			ASSERT(pGlobalUnits!=NULL);

			//�����ַ
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%sSpreader.aspx"),szPlatformLink);

			//���ӵ�ַ
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

			return TRUE;

		}
	case IDC_NAVIGATION+6:
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"���������Ƶ�������Ե�Ƭ�̣�");
				return true;
			}

// 			//��ȡָ��
// 			CGlobalUnits * pGlobalUnits = (CGlobalUnits *)CGlobalUnits::GetInstance();
// 			ASSERT(pGlobalUnits!=NULL);
// 
// 			//�����ַ
// 			TCHAR szNavigation[256]=TEXT("");
// 		//	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("http://ala.zoossoft.com/LR/Chatpre.aspx?id=ALA22728236&lng=cn"),szPlatformLink);
// 
// 			//��ȡ�����ļ�
// 			//����Ŀ¼
// 			TCHAR szDirectory[MAX_PATH]=TEXT("");
// 			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));
// 
// 			//����·��
// 			TCHAR szFileName[MAX_PATH]=TEXT("");
// 			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
// 
// 			//�����ַ
// 			TCHAR szLogonLink[256]=TEXT("");
// 			//	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%sLogonLink4.aspx"),szPlatformLink);
// 			GetPrivateProfileString(TEXT("kefu"),TEXT("Link"),TEXT(""),szNavigation,CountArray(szNavigation),szFileName);
// 
// 
// 			//���ӵ�ַ
// 			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

			SendToServer(9);
			return TRUE;

		}
	case IDC_MESSAGE:
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"���������Ƶ�������Ե�Ƭ�̣�");
				return true;
			}

			m_PlazaViewItem.ShowWindow(SW_HIDE);
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			//m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_HIDE);
			m_dlgQukuan.ShowWindow(SW_HIDE);
			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_HIDE);
			m_dlgPlatformMessage.ShowWindow(SW_SHOW);

			m_cbShowNewsType = 2;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_bGame=false;
			return TRUE;
		}
	}

	//�˵�����
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//�û���¼
		{
			m_MissionLogon.ShowLogon();
		//	AvtiveMainMissionItem();

			return TRUE;
		}
	case IDM_SWITCH_ACCOUNTS:	//�л��ʺ�
		{
			//�л�ѯ��
			if (m_ServerViewItem.m_hWnd!=NULL)
			{
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("��Ϸ���伴���رգ�ȷʵҪ���л��ʺš��� "),MB_YESNO)!=IDYES) return TRUE;
			}

			//ȫ������
			CGlobalUnits::GetInstance()->DeleteUserCookie();
			CGlobalUserInfo::GetInstance()->ResetUserInfoData();

			m_PlazaViewItem.CreateDlgCaipiao(4);
			m_PlazaViewItem.ShowWindow(SW_HIDE);

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

			
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//�����ؼ�
VOID CPlatformFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;

	//���λ��
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//������Ϣ
	tagEncircleInfo EncircleInfoFrame;
	tagEncircleInfo EncircleInfoAffiche;
	tagEncircleInfo EncircleInfoItemFrame;
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);
	//m_AfficheEncircle.GetEncircleInfo(EncircleInfoAffiche);
	m_ItemFrameEncircle.GetEncircleInfo(EncircleInfoItemFrame);

	//��ȡλ��
	ScreenToClient(&rcSplitter);

	//��ַ�Χ
	INT nLessPos=nWidth*25/100,nMaxPos=nWidth/2;
	if (nLessPos>=LESS_LIST_CX) nLessPos=LESS_LIST_CX;

	//�������
	if (m_bRectify==false)
	{
		m_bRectify=true;
		rcSplitter.left=nLessPos;
		rcSplitter.right=rcSplitter.left+SPLITTER_CX;
	}

	//��ַ�Χ
	m_SkinSplitter.SetSplitterRange(nLessPos,nMaxPos);

	//����λ��
	if ((nWidth!=0)&&(nHeight!=0))
	{
		if (rcSplitter.left<nLessPos)
		{
			rcSplitter.left=nLessPos;
			rcSplitter.right=rcSplitter.left+SPLITTER_CX;
		}
		if (rcSplitter.right>nMaxPos)
		{
			rcSplitter.right=nMaxPos;
			rcSplitter.left=rcSplitter.right-SPLITTER_CX;
		}
	}

	//λ�õ���
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=nHeight-EncircleInfoFrame.nBBorder;

	//�ؼ�λ��
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//���ư�ť
	DeferWindowPos(hDwp,m_btMin,NULL,nWidth-79,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,nWidth-48,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSound,NULL,nWidth-110,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btNet,NULL,nWidth-155,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btPlazaNotice,NULL,nWidth-267,0,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btPlazaMessage,NULL,nWidth-367,0,0,0,uFlags|SWP_NOSIZE);
	//m_btPlazaMessage.ShowWindow(SW_HIDE);
	//������ť
	CRect rcNavigation;
	m_btNavigation[1].GetWindowRect(&rcNavigation);

	//������ť
	INT nNavigationArea=rcNavigation.Width()*CountArray(m_btNavigation);
	INT nSpace=0;//__min(__max((nWidth-nNavigationArea-328)/(CountArray(m_btNavigation)-1),0),49);

	//������ť
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		INT nIndex=CountArray(m_btNavigation)-i;
	//	m_btNavigation[i].GetWindowRect(&rcNavigation);
		INT nXExcursion=nWidth-(rcNavigation.Width()+nSpace)*nIndex-13;
//		if(i!=0)
		//	nXExcursion+=3;
		DeferWindowPos(hDwp,m_btNavigation[i],NULL,nXExcursion,31,0,0,uFlags|SWP_NOSIZE);
	}
	//DeferWindowPos(hDwp,m_btHome,NULL,nWidth-(rcNavigation.Width()+nSpace)*8-16,31,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btHome,NULL,560,2,0,0,uFlags|SWP_NOSIZE);

	//��ͼλ��
	m_rcViewItem.left=rcSplitter.right+EncircleInfoItemFrame.nLBorder-10;
	m_rcViewItem.top=EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-11;
	m_rcViewItem.right=nWidth-EncircleInfoFrame.nRBorder-EncircleInfoItemFrame.nRBorder+8;
	m_rcViewItem.bottom=nHeight-EncircleInfoFrame.nBBorder-EncircleInfoItemFrame.nBBorder+6;
	

	//���Ϳ��
	CSize SizeGameTypeCtrl;
	m_WndGameTypeCtrl.GetControlSize(SizeGameTypeCtrl);
	if(m_cbShowNewsType == 1)
	{
		DeferWindowPos(hDwp,m_WndTopNews,NULL,rcSplitter.right-198,EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-SizeGameTypeCtrl.cy+136,
			SizeGameTypeCtrl.cx-148+135+180,26,uFlags);
	}
	else if(m_cbShowNewsType == 2)
	{
		DeferWindowPos(hDwp,m_WndTopNews,NULL,rcSplitter.right+64-226-33,EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-SizeGameTypeCtrl.cy+136,
			SizeGameTypeCtrl.cx+160,26,uFlags);

	}
	else 
	{
		DeferWindowPos(hDwp,m_WndTopNews,NULL,rcSplitter.right+62-33,EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-SizeGameTypeCtrl.cy+137,
			SizeGameTypeCtrl.cx-56,26,uFlags);

	}


	//��Ϸ�㳡
	DeferWindowPos(hDwp,m_PlazaViewItem,NULL,m_rcViewItem.left+18,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height()+20,uFlags);
	DeferWindowPos(hDwp,m_WndGameTypeCtrl,NULL,rcSplitter.right+16,EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-SizeGameTypeCtrl.cy+500,
		SizeGameTypeCtrl.cx,SizeGameTypeCtrl.cy,uFlags);
	DeferWindowPos(hDwp,m_ServerViewItem,NULL,m_rcViewItem.left,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags);
	DeferWindowPos(hDwp,m_dlgUserAccount,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgHuiyuan,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgChongzhi,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgQukuan,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgHuodong,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgTouZhu,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgPlatformMessage,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	//�����ؼ�
	DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
	DeferWindowPos(hDwp,m_WndUserInfoCtrl,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder+14,rcSplitter.left-EncircleInfoFrame.nLBorder+20,rcUserInfoCtrl.Height(),uFlags);
	
	m_nTopNewX = EncircleInfoFrame.nLBorder+2;
	m_nTopNewY = EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()-25;
	//DeferWindowPos(hDwp,m_PlatformPublicize,NULL,EncircleInfoFrame.nLBorder+EncircleInfoAffiche.nLBorder-5,EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()+EncircleInfoAffiche.nTBorder+10,rcSplitter.left-EncircleInfoAffiche.nLBorder-EncircleInfoAffiche.nRBorder-EncircleInfoFrame.nLBorder+5,nHeight-EncircleInfoAffiche.nTBorder-EncircleInfoAffiche.nBBorder-EncircleInfoFrame.nTBorder-rcUserInfoCtrl.Height()-EncircleInfoFrame.nBBorder,uFlags);
	//DeferWindowPos(hDwp,m_PlatFormNoticePublicize,NULL,238,9,540,18,uFlags);
	//���½ǹ��
	//DeferWindowPos(hDwp,m_PlatFormLeftPublicize,NULL,EncircleInfoFrame.nLBorder+EncircleInfoAffiche.nLBorder-4,EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()+EncircleInfoAffiche.nTBorder+nHeight-EncircleInfoAffiche.nTBorder-EncircleInfoAffiche.nBBorder-EncircleInfoFrame.nTBorder-rcUserInfoCtrl.Height()-EncircleInfoFrame.nBBorder+25,197,147,uFlags);
	//DeferWindowPos(hDwp,m_Grid,NULL,EncircleInfoFrame.nLBorder+EncircleInfoAffiche.nLBorder-5,EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()+EncircleInfoAffiche.nTBorder+45,rcSplitter.left-EncircleInfoAffiche.nLBorder-EncircleInfoAffiche.nRBorder-EncircleInfoFrame.nLBorder+7,nHeight-42-EncircleInfoAffiche.nTBorder-EncircleInfoAffiche.nBBorder-EncircleInfoFrame.nTBorder-rcUserInfoCtrl.Height()-EncircleInfoFrame.nBBorder,uFlags);
	m_Grid.ShowWindow(SW_HIDE);
	//DeferWindowPos(hDwp,m_logo,NULL,4,1,229,130,uFlags);
	//DeferWindowPos(hDwp,m_Publicizeurl,NULL,13,nHeight-191,204,166,uFlags);

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//���½���
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//������Դ
VOID CPlatformFrame::RectifyResource(INT nWidth, INT nHeight)
{
	//�����ж�
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//���λ��
		CRect rcFrame;
		rcFrame.SetRect(SHADOW_CX,SHADOW_CY,nWidth-SHADOW_CX,nHeight-SHADOW_CY);

		//��������
		CRgn RegionWindow;
		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,ROUND_CX,ROUND_CY);

		//��������
		SetWindowRgn(RegionWindow,TRUE);

		//�ֲ㴰��
		if (m_SkinLayered.m_hWnd!=NULL)
		{
			//��������
			CImage ImageBuffer;
			ImageBuffer.Create(nWidth,nHeight,32);

			//���� DC
			CImageDC ImageDC(ImageBuffer);
			CDC * pBufferDC=CDC::FromHandle(ImageDC);

			//�滭����
			DrawControlView(pBufferDC,nWidth,nHeight);

			//���·ֲ�
			m_SkinLayered.InitLayeredArea(pBufferDC,255,rcFrame,CPoint(ROUND_CX,ROUND_CX),false);
		}
	}

	return;
}

//�滭����
VOID CPlatformFrame::DrawControlView(CDC * pDC, INT nWidth, INT nHeight)
{
	pDC->SetBkColor(RGB(255,0,255));
	//���λ��
	tagEncircleInfo FrameEncircleInfo;
	tagEncircleInfo ItemFrameEncircleInfo;
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);
	m_ItemFrameEncircle.GetEncircleInfo(ItemFrameEncircleInfo);

	//���λ��
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//λ��ת��
	ScreenToClient(&rcSplitter);

	//�ؼ�λ��
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//�������
	CRect rcFrame;
	rcFrame.SetRect(0,0,nWidth,nHeight-4);

	//����λ��
	CRect rcAffiche;
	rcAffiche.right=rcSplitter.left;
	rcAffiche.left=FrameEncircleInfo.nLBorder;
	rcAffiche.top=FrameEncircleInfo.nTBorder+rcUserInfoCtrl.Height()+8;
	rcAffiche.bottom=nHeight-FrameEncircleInfo.nBBorder-4;
	m_nTop = 0;//rcAffiche.top+45;

	//��������
	CRect rcItemFrame;
	rcItemFrame.left=rcSplitter.right;
	rcItemFrame.top=FrameEncircleInfo.nTBorder;
	rcItemFrame.right=nWidth-FrameEncircleInfo.nRBorder;
	rcItemFrame.bottom=nHeight-FrameEncircleInfo.nBBorder-4;

	//�滭���
	m_FrameEncircle.DrawEncircleFrame(pDC,rcFrame);
	//m_AfficheEncircle.DrawEncircleFrame(pDC,rcAffiche);
	//m_ItemFrameEncircle.DrawEncircleFrame(pDC,rcItemFrame);
	return;
}

//�����
bool CPlatformFrame::ActiveServerViewItem()
{
	//�����ж�
	ASSERT(m_ServerViewItem.m_hWnd!=NULL);
	if (m_ServerViewItem.m_hWnd==NULL) return false;

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//��Ϸ�㳡
	DeferWindowPos(hDwp,m_PlazaViewItem,NULL,m_rcViewItem.left+18,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags);
	DeferWindowPos(hDwp,m_ServerViewItem,NULL,m_rcViewItem.left,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags|SWP_SHOWWINDOW);

	//��������
	EndDeferWindowPos(hDwp);

	return true;
}

//ɾ������
bool CPlatformFrame::DeleteServerViewItem()
{
	//�����ж�
	if (m_ServerViewItem.m_hWnd==NULL) return false;
	ASSERT(m_ServerViewItem.m_hWnd!=NULL);

	//���ٷ���
	m_ServerViewItem.DestroyWindow();

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//��Ϸ�㳡
	DeferWindowPos(hDwp,m_PlazaViewItem,NULL,m_rcViewItem.left+18,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags|SWP_SHOWWINDOW);
	DeferWindowPos(hDwp,m_ServerViewItem,NULL,m_rcViewItem.left,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags|SWP_HIDEWINDOW);

	//��������
	EndDeferWindowPos(hDwp);

	return true;
}

//���뷿��
bool CPlatformFrame::EntranceServerItem(CGameServerItem * pGameServerItem)
{
	//Ч�����
	ASSERT(pGameServerItem!=NULL);
	if (pGameServerItem==NULL) return NULL;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��¼�ж�
	if (pGlobalUserData->dwUserID==0L) return NULL;

	//��������
	tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
	tagGameKind * pGameKind=&pGameServerItem->m_pGameKindItem->m_GameKind;

	//��ǰ�ж�
	if (m_ServerViewItem.m_hWnd!=NULL)
	{
		m_ServerViewItem.InitServerViewItem(pGameServerItem);
// 		//��ʾ��Ϣ
// 		TCHAR szBuffer[256]=TEXT("");
// 		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ڽ��� [ %s ] ��Ϸ�����У����Ժ�ȴ�Ƭ��..."),m_ServerViewItem.GetServerName());
// 
// 		//��ʾ��Ϣ
// 		CInformation Information(this);
// 		Information.ShowMessageBox(szBuffer,MB_OK|MB_ICONWARNING,30);

		return true;
	}


	//��װ�ж�
	if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
	{
		//��ȡ�汾
		CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
		CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

		//������Ϸ
		if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
		{
			CGlobalUnits * pCGlobalUnits=CGlobalUnits::GetInstance();
			pCGlobalUnits->DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,pGameServer->wServerID);

			return NULL;
		}

		//�����б�
		OnGameItemUpdate(pGameKindItem);
	}

	//��������
	try
	{
		//���÷���
		m_ServerViewItem.Create(IDD_GAME_SERVER,this);
		m_ServerViewItem.InitServerViewItem(pGameServerItem);
		
	}
	catch (LPCTSTR pszString)
	{
		//���ٷ���
		m_ServerViewItem.DestroyWindow();

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(pGameServer->szServerName,pszString,MB_ICONERROR);

		return false;
	}

	return true;
}

//�ر���Ϣ
VOID CPlatformFrame::OnClose()
{

	if(!m_bKillSocket)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//�ر���ʾ
		if (pGlobalUserData->dwUserID!=0L)
		{
			//��ʾ����
			CDlgEnquire DlgEnquire;
			INT_PTR nResult=DlgEnquire.DoModal();

			//�����
			switch (nResult)
			{
			case IDCANCEL:				//ȡ������
				{
					return;
				}
			case IDC_CLOSE_SERVER:		//�˳�����
				{
					//�رշ���
					if (m_ServerViewItem.m_hWnd!=NULL)
					{
						DeleteServerViewItem();
					}

					return;
				}
			case IDC_SWITCH_ACCOUNTS:	//�л��ʺ�
				{
					if(m_dlgHuiyuan.IsWindowVisible())
					{
						m_dlgHuiyuan.OnShowWindow(false,1);
					}
					else if(m_dlgUserAccount.IsWindowVisible())
					{
						m_dlgUserAccount.OnShowWindow(false,1);
					}
					else if(m_dlgTouZhu.IsWindowVisible())
					{
						m_dlgTouZhu.OnShowWindow(false,1);
					}

					m_WndUserInfoCtrl.ReleaseFace();
					//Ͷ����Ϣ
					PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);
					SendToServer(6);

					return;
				}
			}
		}
		SendToServer(6);

	}

	m_MissionManager.ConcludeMissionItem(this,false);
//	m_MissionManager.ClearMissionItem();
	__super::OnClose();
}
void CPlatformFrame::Loadweb()
{

	CString strPath;  //�ļ�·��
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	int pos = strPath.ReverseFind('\\'); 
	strPath = strPath.Left(pos); 

	//m_logo.Navigate(strPath +_T("\\logo.html"));
	//m_logo.EnableWindow(FALSE);

	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	TCHAR szUrl[126] = TEXT("");
	GetPrivateProfileString(TEXT("PlazaUrl"),TEXT("PlazaUrlLbAdd"),TEXT(""),szUrl,CountArray(szUrl),szFileName);

	//m_Publicizeurl.Navigate(szUrl);



}


//�滭����
BOOL CPlatformFrame::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//���� DC
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(ImageDC);

// 	CBitImage Affiche;
// 	Affiche.LoadFromResource(AfxGetInstanceHandle(),IDB_AFFICHE_GROUP);
// 	Affiche.DrawImageTile(pDC,rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height());
	//�滭����
	DrawControlView(pBufferDC,rcClient.Width(),rcClient.Height());

	//������Դ
	CPngImage StationLogo;
	StationLogo.LoadImage(AfxGetInstanceHandle(),TEXT("PLATFORM_LOGO"));
	StationLogo.DrawImage(pBufferDC,1,0);
// 	CPngImage TitleLogo;
// 	TitleLogo.LoadImage(AfxGetInstanceHandle(),TEXT("TITLE_LOGO"));
// 	TitleLogo.DrawImage(pBufferDC,1,0);


	DrawTopUsers(pBufferDC);

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//λ����Ϣ
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	//������Դ
	RectifyResource(cx,cy);

	return;
}

//������Ϣ
INT CPlatformFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	m_DlgStatus.SetStatusViewSink(this);
	//���ô���
	ModifyStyle(WS_CAPTION,WS_MINIMIZEBOX|WS_MAXIMIZEBOX,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	m_top8Font.CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 

	theTime = CTime::GetCurrentTime();
	m_bShutdowSocket = true;
	
	
	//�����㳡
	CRect rcCreate(0,0,0,0);
	m_PlazaViewItem.Create(IDD_GAME_PLAZA,this);

// 	m_dlgUserAccount.Create(IDD_USER_ACCOUNT,this);
// 	m_dlgUserAccount.ShowWindow(SW_HIDE);
// 	m_dlgHuiyuan.Create(CHuiYuanDlg::IDD,this);
// 	m_dlgHuiyuan.ShowWindow(SW_HIDE);
// 	m_dlgChongzhi.Create(CChongZhiDlg::IDD,this);
// 	m_dlgChongzhi.ShowWindow(SW_HIDE);
// 	m_dlgQukuan.Create(CQuKuanDlg::IDD,this);
// 	m_dlgQukuan.ShowWindow(SW_HIDE);
// 	m_dlgHuodong.Create(CHuoDongDlg::IDD,this);
// 	m_dlgHuodong.ShowWindow(SW_HIDE);
// 	m_dlgTouZhu.Create(CZhangHuTzhLogDlg::IDD,this);
// 	m_dlgTouZhu.ShowWindow(SW_HIDE);
	//�����ؼ�
	m_SkinSplitter.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN_SPLITTER);
	m_WndGameTypeCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_GAME_TYPE_CTRL);
	m_WndUserInfoCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_USER_INFO_CTRL);
	

	//m_logo.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_LOGO);
	//m_Publicizeurl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_LB_ADD);

	m_WndTopNews.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_LAST_TOP_NEWS);

	m_Grid.Create(rcCreate,this,IDC_TOP_NWES);
	m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
	m_Grid.ShowWindow(SW_HIDE);
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	//������ť
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MIN);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);
	m_btSound.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SOUND);
	m_btNet.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NET);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btMin.SetButtonImage(IDB_BT_MIN,hInstance,true,false);
	m_btClose.SetButtonImage(IDB_BT_CLOSE,hInstance,true,false);
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
	int nPlay = GetPrivateProfileInt(TEXT("SOUND"),TEXT("IsPlay"),1,szFileName);
	m_bSound = (nPlay==1)?true:false;
	if(nPlay == 1)
		m_btSound.SetButtonImage(IDB_BT_SOUND,hInstance,true,false);
	else
		m_btSound.SetButtonImage(IDB_BT_MUTE,hInstance,true,false);
	m_btNet.SetButtonImage(IDB_BT_NET,hInstance,true,false);


	//������ť
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		m_btNavigation[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NAVIGATION+i);
	}
	m_btHome.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_HOME);
	m_btHome.SetButtonImage(IDB_BK_HOME_BTN,hInstance,true,false);
	m_btPlazaNotice.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NOTICE);
	m_btPlazaNotice.SetButtonImage(IDB_BK_NOTICE,hInstance,true,false);
	m_btPlazaMessage.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MESSAGE);
	m_btPlazaMessage.SetButtonImage(IDB_BK_MESSAGE,hInstance,true,false);
	//m_btPlazaMessage.ShowWindow(SW_HIDE);
	//������ť
	m_btNavigation[0].SetButtonImage(IDB_BK_NAVIGATION1,hInstance,true,false);
	m_btNavigation[1].SetButtonImage(IDB_BK_NAVIGATION2,hInstance,true,false);
	m_btNavigation[2].SetButtonImage(IDB_BK_NAVIGATION3,hInstance,true,false);
	m_btNavigation[3].SetButtonImage(IDB_BK_NAVIGATION4,hInstance,true,false);
	m_btNavigation[4].SetButtonImage(IDB_BK_NAVIGATION5,hInstance,true,false);
	m_btNavigation[5].SetButtonImage(IDB_BK_NAVIGATION6,hInstance,true,false);
	m_btNavigation[6].SetButtonImage(IDB_BK_NAVIGATION7,hInstance,true,false);


	//��Ϸ�б�
	m_ServerListData.SetServerListDataSink(this);

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btNavigation[0],TEXT("��Ҫ��ֵ"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[1],TEXT("��Ҫȡ��"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[2],TEXT("�˺Ź���"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[3],TEXT("�Ŷӹ���"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[4],TEXT("Ͷע��¼"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[5],TEXT("�����"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[6],TEXT("�ͷ�����"));
	m_ToolTipCtrl.AddTool(&m_btMin,TEXT("��С��"));
	m_ToolTipCtrl.AddTool(&m_btClose,TEXT("�رմ���"));
	m_ToolTipCtrl.AddTool(&m_btSound,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btNet,TEXT("����"));

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);


	//����λ��
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//λ�õ���
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//�ƶ�����
	CRect rcFrame;
	rcFrame.top=(rcArce.Height()-SizeRestrict.cy)/2;
	rcFrame.left=(rcArce.Width()-SizeRestrict.cx)/2;
	rcFrame.right=(rcArce.Width()+SizeRestrict.cx)/2;
	rcFrame.bottom=(rcArce.Height()+SizeRestrict.cy)/2;
	SetWindowPos(NULL,rcFrame.left,rcFrame.top,rcFrame.Width(),rcFrame.Height(),SWP_NOZORDER);



	//�����ֲ�
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	m_SkinLayered.CreateLayered(this,rcWindow);

	//������Դ
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyResource(rcClient.Width(),rcClient.Height());


	Loadweb();


	return 0;
}

//��ʾ��Ϣ
VOID CPlatformFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//��ʾ�ֲ�
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_nNewsYPos=362+360;
		m_nNewsYPos -= 290;
	}
	else
	{
		m_tishiDlg.StartKillTimer();
		KillTimer(TimerNews);
		KillTimer(TimerZhunBeiData);
		KillTimer(TimerJiShi);
		KillTimer(TimerNews1);
		KillTimer(TimerLuckyNum);
		KillTimer(TimerReconnect);
	//	KillTimer(TimerTopNews);
	}
	return;
}

//λ�øı�
VOID CPlatformFrame::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//�ƶ��ֲ�
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�ⰴ����
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//ת���ʱ�
LRESULT CPlatformFrame::OnZhuanhuanCaibi(WPARAM wParam, LPARAM lParam)
{
	//ȫ������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	DWORD dwJine = (DWORD)lParam;
	DOUBLE fJine = 0.0;

	fJine = (DOUBLE)dwJine/1000;

	if(fJine > pGlobalUserData->lScore)
	{
		MyMessageBox(L"���㣡");
		return 1;
	}

		m_fJiaoyiScore = fJine;
		
		SendToServer(3);
	return 1;
}
//ת�����
LRESULT CPlatformFrame::OnZhuanhuanJinbi(WPARAM wParam, LPARAM lParam)
{
	DWORD dwJine = (DWORD)lParam;
	DOUBLE fJine = 0.0;

	fJine = (DOUBLE)dwJine/1000;

		if(fJine > theAccount.yue)
		{
			MyMessageBox(L"���㣡");
			return 1;
		}
 
		m_fJiaoyiScore = fJine;

		SendToServer(2);
	return 1;
}

LRESULT CPlatformFrame::OnUpdateButton(WPARAM wParam, LPARAM lParam)
{
	WORD wClickedType = wParam;
	m_WndGameTypeCtrl.SetClicked(wClickedType);
	return 1;
}
LRESULT CPlatformFrame::OnUpDateAccoount(WPARAM wParam, LPARAM lParam)
{
	m_WndUserInfoCtrl.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	return 1;
}
LRESULT CPlatformFrame::OnShowXgmm(WPARAM wParam, LPARAM lParam)
{
	ShowXgmm();
	return 1;
}
LRESULT CPlatformFrame::OnRefreshYue(WPARAM wParam, LPARAM lParam)
{
	SendToServer(4);
	return 1;
}
LRESULT CPlatformFrame::OnFreshGame(WPARAM wParam, LPARAM lParam)
{
	
	m_PlazaViewItem.FreshGame();
	return 1;
}
LRESULT CPlatformFrame::OnTouZhuTishi(WPARAM wParam, LPARAM lParam)
{
	if(!m_bCanSend)
		return 1;
	CMD_GP_GetTouzhuTishi GetTouzhuTishi;
	ZeroMemory(&GetTouzhuTishi,sizeof(GetTouzhuTishi));

	GetTouzhuTishi.dwUserID = theAccount.user_id;
	//��������
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_TOUZHU_TISHI ,&GetTouzhuTishi,sizeof(GetTouzhuTishi));

	return 1;
}
void CPlatformFrame::ShowXgmm()
{
	OnCommand(402,0);
	m_dlgUserAccount.OnBnClickedBtnModifypwd();
}
//�¼���Ϣ
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (wParam)
	{
	case EVENT_USER_EXIT:
		{
			 
			EndDialog(m_pPlatformFrame->GetSafeHwnd(),IDOK);

			return 1;
		}
	case EVENT_USER_LOGON:			//��¼���
		{
			//��ʾ����
			ShowWindow(SW_SHOW);
		//	SetForegroundWindow();
			BringWindowToTop();
			m_PlazaViewItem.ShowWindow(SW_SHOW);
			m_PlazaViewItem.ShowTypeItemView();
			m_PlazaViewItem.SetLogonSuccess(true);

			//��������
			m_MissionList.UpdateMissionStatus(true);

			m_WndUserInfoCtrl.SetClicked(1,0);
			m_bCanSend = true;
			//��ȡָ��
			CGlobalUnits * pGlobalUnits = (CGlobalUnits *)CGlobalUnits::GetInstance();
			if(pGlobalUnits==NULL) return 0L;
			if(m_tishiDlg.GetSafeHwnd() == NULL)
				m_tishiDlg.Create(CTiShiDlg::IDD,this);

			//�û����			
			pGlobalUnits->WriteUserCookie();

			//��ѯ����
			SendToServer(1);

			SendToServer(5);
			SendToServer(7);
			SendToServer(8);
		//	SendToServer(12);


		//	m_tishiDlg.SetTiShiMessage(L"1111111111");

			m_PlazaViewItem.CreateDlgCaipiao(3);
// 			//�����̵߳����ؿ�������
// 			AfxBeginThread(GetLuckNumber, this);

			OnCommand(IDC_HOME,0);
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			if(pGlobalUserInfo != NULL)
			{
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				if(pGlobalUserData!=NULL&&pGlobalUserData->lUserMedal == 1111)
				{
					CInformation information;
					int nResult = information.ShowMessageBox(_T("�������������ù��ڼ򵥣������ø��ӵ�½�����ȡ�����룡"),MB_ICONWARNING,30);
					if(nResult==1)
					{
						ShowXgmm();
					}		
				}
			}
		//	SetTimer(TimerTopNews,300*1000,NULL);
			TCHAR szTodayURL[126]={0};
			//����Ŀ¼
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFileName[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

			//��ȡ����
			CWHIniData IniData;
			IniData.SetIniFilePath(szFileName);

			IniData.ReadEncryptString(TEXT("PlazaUrl"),TEXT("PlazaNotice"),TEXT("http://192.168.0.105/WinFromBanner/notices.aspx"),szTodayURL,CountArray(szTodayURL));

			CDlgBrowser dlgBrowser;
			dlgBrowser.CreateBrowserControl(TEXT("���¹���"),szTodayURL,CSize(802,503));

			SetTimer(TimerZhunBeiData, 5*1000, NULL);
			SetTimer(TimerJiShi, 1000, NULL);
			SetTimer(TimerLuckyNum, 3000, NULL);
			//SetTimer(TimerZnx, 300000, NULL);
			return 0L;
		}
	case EVENT_USER_LOGOUT:			//ע���ɹ�
		{
			//���ش���
			ShowWindow(SW_HIDE);

			//�رշ���
			DeleteServerViewItem();

			//��ʾ��¼
			m_MissionLogon.ShowLogon();
// 			AvtiveMainMissionItem();
			m_WndUserInfoCtrl.ReleaseFace();
			return 0L;
		}
	case EVENT_DOWN_LOAD_FINISH:	//�������
		{
			//��ȡ����
			WORD wKindID=LOWORD(lParam);
			WORD wServerID=HIWORD(lParam);

			//�����б�
			CServerListData * pServerListData=CServerListData::GetInstance();
			if (pServerListData!=NULL) pServerListData->OnEventDownLoadFinish(wKindID);

			//���뷿��
			if ((wKindID!=0)&&(wServerID!=0))
			{
				CGameServerItem * pGameServerItem=m_ServerListData.SearchGameServer(wServerID);
				if (pGameServerItem!=NULL) EntranceServerItem(pGameServerItem);
			}

			return 0L;
		}
	case EVENT_UPDATE_ONLINE: //��������
		{
			//���½���
			m_PlazaViewItem.InValidateWndView(VIEW_MODE_SERVER);

			return 0L;
		}
	}

	return 0L;
}
//�¼���ҳ
LRESULT CPlatformFrame::OnTurnPage(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)lParam;
	if( nType==2)
		m_dlgPlatformMessage.m_dlgSendMessage.SearchUser();
	else
		m_dlgPlatformMessage.m_dlgSendMessage.OnTurnPage(wParam,lParam);
	return 1;
}
//�¼���ҳ
LRESULT CPlatformFrame::OnSelectUser(WPARAM wParam, LPARAM lParam)
{
	m_dlgPlatformMessage.m_dlgSendMessage.SelectUser(wParam,lParam);
	return 1;
}
//���и���
LRESULT CPlatformFrame::OnMessageInsureUpdate(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	tagInsurePlazaUpdata * pInsurePlazaUpdate = (tagInsurePlazaUpdata *)wParam;
	ASSERT(pInsurePlazaUpdate!=NULL);
	if(pInsurePlazaUpdate==NULL) return 0L;

	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if(pGlobalUserData==NULL) return 0;
    
	//��������
	pGlobalUserData->lScore=pInsurePlazaUpdate->lUserScore;
	pGlobalUserData->lInsure=pInsurePlazaUpdate->lUserInsure;

	//ƽ̨�¼�
	CPlatformEvent * pPlatformEvent = CPlatformEvent::GetInstance();
	ASSERT(pPlatformEvent!=NULL);
	pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
//��ʱ��������NEWS
void CPlatformFrame::DrawTopUsers(CDC* pDC)
{
	if(!m_bGame)
		return;

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	CFont *pOldFont = pDC->SelectObject(&m_top8Font);

	//	16,362,235,122
#ifdef _DEBUG
	m_vecTitle.clear();
	m_vecTitle.push_back(L"�ķ��ܷ�ɳ���Ϸ�˳���Ƿ�������˳��������������Ⱦ�� ��Ⱦ������Ⱦ��Ⱦ�����������������Ϊ���������������Ⱦ������Ⱦ����������");
	m_vecTitle.push_back(L"�ķ��ܷ�ɳ���Ϸ�˳���Ƿ�������˳��������������Ⱦ�� ��Ⱦ������Ⱦ��Ⱦ�����������������Ϊ���������������Ⱦ������Ⱦ����������");
	m_vecTitle.push_back(L"�ķ��ܷ�ɳ���Ϸ�˳���Ƿ�������˳��������������Ⱦ�� ��Ⱦ������Ⱦ��Ⱦ�����������������Ϊ���������������Ⱦ������Ⱦ����������");
	m_vecTitle.push_back(L"�ķ��ܷ�ɳ���Ϸ�˳���Ƿ�������˳��������������Ⱦ�� ��Ⱦ������Ⱦ��Ⱦ�����������������Ϊ���������������Ⱦ������Ⱦ����������");
	m_vecTitle.push_back(L"�ķ��ܷ�ɳ���Ϸ�˳���Ƿ�������˳��������������Ⱦ�� ��Ⱦ������Ⱦ��Ⱦ�����������������Ϊ���������������Ⱦ������Ⱦ����������");
#endif
	//���λ��
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//λ��ת��
	ScreenToClient(&rcSplitter);
	tagEncircleInfo FrameEncircleInfo;
	tagEncircleInfo ItemFrameEncircleInfo;
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);
	m_ItemFrameEncircle.GetEncircleInfo(ItemFrameEncircleInfo);
	//�ؼ�λ��
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//����λ��
	CRect rcAffiche;
	rcAffiche.right=rcSplitter.left;
	rcAffiche.left=FrameEncircleInfo.nLBorder;
	rcAffiche.top=FrameEncircleInfo.nTBorder+rcUserInfoCtrl.Height()+15;
	rcAffiche.bottom=rcClient.Height()-FrameEncircleInfo.nBBorder-12;
	m_nTop = 0;//rcAffiche.top+45;

	//m_AfficheEncircle.DrawEncircleFrame(pDC,rcAffiche);
	CPngImage ImageNews;
	ImageNews.LoadImage(AfxGetInstanceHandle(),TEXT("BK_FRAME_NEWS"));
	ImageNews.DrawImage(pDC,rcAffiche.left,rcAffiche.top);

// 	CRect rcText(24, 407, 190, 362+380);
// 	
// 	//pDC->SetTextCharacterExtra(3);
// 	CString strText;
// 	COLORREF crTextColor= pDC->SetTextColor(RGB(88, 78, 77));
// 	for(unsigned int n=0; n<m_vecTitle.size(); n++)
// 	{
// 		strText += m_vecTitle[n].c_str();
// 		strText += L"\r\n";
// 	}
// 	pDC->DrawText( strText, &rcText, DT_LEFT|DT_WORDBREAK);

//	 pDC->SetTextColor(crTextColor);
	pDC->SelectObject(pOldFont);

	return;

}

//���Ź���
void CPlatformFrame::GridCtrlInit()
{
	return;
	m_Grid.SetGridLines(GVL_HORZ);
	m_Grid.ModifyStyle(WS_BORDER|WS_VSCROLL|WS_HSCROLL, NULL, NULL);
	m_Grid.EnableTitleTips();
	m_Grid.SetFixedBkColor(RGB(87,21,38));
	//	m_Grid.SetFixedTextColor(FONT_BG_COLOR);
	m_Grid.SetGridColor(RGB(73, 0, 0));
	m_Grid.SetGridBkColor(RGB(73, 0, 0));
	m_Grid.SetGridLineColor(RGB(127,127,123));
	m_Grid.SetBkColor(RGB(73, 0, 0));
	m_Grid.SetEditable(false);
	m_Grid.SetTextBkColor(RGB(73, 0, 0));			//����ɫ
	m_Grid.SetTextColor(RGB(255,255,255));				//������ɫ
	m_Grid.SetRowCount(5);								//��ʼΪ10��
	m_Grid.SetColumnCount(2);							//��ʼ��Ϊ11��
	//    m_Grid.SetFixedRowCount(1);							//��ͷΪһ��
	//    m_Grid.SetFixedColumnCount(1);					//��ͷΪһ��
	m_Grid.SetColumnWidth(0,0);							//����0�п� 
	m_Grid.SetColumnWidth(1,231);
	

	CString strTxtQiHao;
	for (int row = 0; row < m_Grid.GetRowCount(); row++)
	{
		m_Grid.SetItemState(row,0, m_Grid.GetItemState(row,0) | GVIS_READONLY);			//ֻ������
		m_Grid.SetItemState(row,1, m_Grid.GetItemState(row,1) | GVIS_READONLY);			//ֻ������
		for (int col = 0; col < m_Grid.GetColumnCount(); col++)
		{ 
			//���ñ����ʾ����
			GV_ITEM Item; 
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			m_Grid.SetRowHeight(row, 23);		//���ø��и�          
			if(col ==0)					//��(0��0)��
			{
				Item.nFormat = DT_CENTER|DT_WORDBREAK;
				//Item.strText.Format(_T("%ld"),col+1);
			}
			else if (col == 1)					//����0�б�ͷ��ʾ
			{        
				Item.nFormat = DT_CENTER|DT_WORDBREAK;
				if(row < m_vecTitle.size())
					Item.strText.Format(_T("%s"), m_vecTitle[row].c_str());
			}	
			m_Grid.SetItem(&Item); 

		}
	}
}



void CPlatformFrame::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);


	CGridCell* pCell = (CGridCell*)m_Grid.GetCell(pItem->iRow, pItem->iColumn);
	if(!m_Grid.GetItemText(pItem->iRow, 1).IsEmpty())
		MyMessageBox(m_Grid.GetItemText(pItem->iRow, 1));
}

//�����Ϣ
VOID CPlatformFrame::OnMouseMove(UINT nFlags, CPoint Point)
{

	return;
}
#pragma pack(pop,  1)
#pragma pack()
