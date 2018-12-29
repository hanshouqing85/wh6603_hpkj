#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_DISPATCH_CARD			102									//���Ʊ�ʶ
#define IDI_SHOWDISPATCH_CARD_TIP	103									//������ʾ
#define IDI_OPENCARD				104									//������ʾ
#define IDI_MOVECARD_END			105									//�Ƅ��ƽY��
#define IDI_POSTCARD				106									//������ʾ
#define IDI_OUT_CARD_TIME			107
#define IDI_JETTON_ANIMATION		108
#define IDI_HANDLELEAVE_ANIMATION	109  
#define IDI_OPENBOX_ANIMATION		110  
#define IDI_LEAVHANDLESOUND			111
#define IDI_PLEASEJETTONSOUND		112
#define IDI_SPEEKSOUND				113
#define IDI_FLASH_CARD				115									//������ʶ
#define IDI_FLASH_RAND_SIDE			116									//������ʶ
#define IDI_SHOW_CARD_RESULT		118
#define IDI_RUN_REDCAR              119
#define IDI_CAR						120
#define IDI_CAR1					121
#define IDI_CAR2					122
#define IDI_ZHONGJIANG              123

//��������ע,�������ж�ʱ����������(1000�Ժ�),ר�������ṩ
#define IDI_ANDROID_BET				1000								

//��ť��ʶ
#define IDC_JETTON_BUTTON_100		200									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000		201									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000		202									//��ť��ʶ
#define IDC_JETTON_BUTTON_100000	203									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000000	204									//��ť��ʶ
#define IDC_JETTON_BUTTON_5000000	205									//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_JETTON_BUTTON_50000	    219									//��ť��ʶ
#define IDC_JETTON_BUTTON_500000	212									//��ť��ʶ
#define IDC_AUTO_OPEN_CARD			213									//��ť��ʶ
#define IDC_OPEN_CARD				214									//��ť��ʶ
#define IDC_BANK					215									//��ť��ʶ
#define IDC_CONTINUE_CARD			216									//��ť��ʶ
#define IDC_BANK_STORAGE			217									//��ť��ʶ
#define IDC_BANK_DRAW				218									//��ť��ʶ
#define IDC_UP						223									//��ť��ʶ
#define IDC_DOWN					224									//��ť��ʶ
#define IDC_RADIO					225									//��ť��ʶ
#define IDC_ADMIN					226									//��ť��ʶ
#define IDC_LAST					227									//��ť��ʶ
#define IDC_NEXT					228									//��ť��ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
	ON_BN_CLICKED(IDC_OPEN_CARD, OnOpenCard)
	ON_BN_CLICKED(IDC_AUTO_OPEN_CARD,OnAutoOpenCard)
	ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_LAST, OnLastMusic)
	ON_BN_CLICKED(IDC_NEXT, OnNextMusic)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;
	m_Out_Bao_y = 0;
	m_CarIndex = 0;
	m_lRobotMaxJetton = 5000000l;

	m_nApplyPageCount = 0;
	m_bApplyBanker = false;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	m_JettonQueIndex = 0; 

	m_lLastJetton = 0;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=FALSE;
	m_bNeedSetGameRecord=FALSE;
	m_bWinTianMen=FALSE;
	m_bWinHuangMen=FALSE;
	m_bWinXuanMen=FALSE;
	m_bFlashResult=FALSE;
	m_blMoveFinish = FALSE;
	m_blAutoOpenCard = TRUE;
	m_bShowBao = FALSE;
	m_enDispatchCardTip=enDispatchCardTip_NULL;
	m_szpm = 0;

	m_winflag = false;
	qisx = 0; 
	chsxx = 0;
	chsxy = 0;
	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;	
	m_blCanStore=false;

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	
	m_CheckImagIndex = 0;
	m_ShowImageIndex  = 0;
	mX = 0;
	mX2 = 0;
	mX1 = 0;
	mX3 = 0;
	mY1 = 0;
	mY3 = 0;
	m_CarIndexold = 0;
	iP = -90;     //�Ƕ�
    iP1 = 90;
	tx1 = 0;
	ty1 = 0;
	tx1= 0;
	ty1 = 0;
	m_x1 = 0;
	m_y1 = 0;
	m_x2 = 0;
	m_y2 = 0;

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;
	dbsj = 0;
	dbsjdt = 0;
	xbsj = 0;
	xbsjdt = 0;
	xbm = 0;
	xbmdt = 0;
	dbm = 0;
	dbmdt = 0;
	xbc = 0;
	xbcdt = 0;
	dbc = 0;
	dbcdt = 0;
	xdz = 0;
	xdzdt = 0;
	ddz = 0;
	ddzdt = 0;

	m_carX = 0;
	
	m_carY = 0;
	m_carY2 = 0;



	
	m_carX2 = m_CarRect[18].left;
	m_carX1 = 0;
	
	m_carY1 = 0;
	m_carX3 = 0;
	
	m_carY3 = 0;
	m_CarIndexod = 0;
	m_js = 0;
	
	
	
	//�ؼ�����
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.LoadFromResource(hInstance,IDB_VIEW_FILL);
	//m_ImageViewcentre.LoadFromResource(hInstance,IDB_CENTREBACK);
	//m_ImageBao.LoadFromResource(hInstance,IDB_CENTREBACK);

	
	m_ImageWinFlags.LoadImage(hInstance,_T("IDB_IDB_FLAG_PNG"));

	m_ImageJettonView.LoadImage(hInstance,_T("JETTOM_VIEW"));
	m_ImageCar.LoadImage(hInstance,TEXT("IDB_CAR"));
	
	m_ImageChdzqz.LoadImage(hInstance,TEXT("IDB_CHDZQZ"));
	m_ImageChbcqz.LoadImage(hInstance,TEXT("IDB_CHBCQZ"));
	m_ImageChbmqz.LoadImage(hInstance,TEXT("IDB_CHBMQZ"));
	m_ImageChbsjqz.LoadImage(hInstance,TEXT("IDB_CHBSJQZ"));
	m_ImageChdbcdh.LoadImage(hInstance,TEXT("IDB_CHDBCDH"));
	m_ImageChddzdh.LoadImage(hInstance,TEXT("IDB_CHDDZDH"));
	m_ImageChdbmdh.LoadImage(hInstance,TEXT("IDB_CHDBMDH"));
	m_ImageChdbsdh.LoadImage(hInstance,TEXT("IDB_CHDBSDH"));
	m_ImageChxbci.LoadImage(hInstance,TEXT("IDB_CHXBCI"));
	m_ImageChxdzo.LoadImage(hInstance,TEXT("IDB_CHXDZO"));
	m_ImageChxbma.LoadImage(hInstance,TEXT("IDB_CHXBMA"));
	m_ImageChxbsj.LoadImage(hInstance,TEXT("IDB_CHXBSJ"));
	m_Imagehkjbj.LoadImage(hInstance,TEXT("IDB_CHKJBJ"));

	m_ImageRedcar.LoadImage(hInstance,TEXT("IDB_REDCAR"));

	m_ImageZuoshang.LoadImage(hInstance,TEXT("IDB_ZUOSHANG"));
	m_ImageYoushang.LoadImage(hInstance,TEXT("IDB_YOUSHANG"));
	m_ImageZuoxia.LoadImage(hInstance,TEXT("IDB_ZUOXIA"));
	m_ImageYouxia.LoadImage(hInstance,TEXT("IDB_YOUXIA"));



	
	m_ImageScoreNumber.LoadFromResource(hInstance,IDB_SCORE_NUMBER);
	m_ImageMeScoreNumber.LoadFromResource(hInstance,IDB_ME_SCORE_NUMBER);
	m_ViewBackPng.LoadImage(hInstance,_T("IDB_VIEW_BACK_PNG"));
	m_idb_selPng.LoadImage(hInstance,_T("IDB_IDB_SEL_PNG"));

	for (int i = 0;i<5;i++) 
		m_ImageCardType[i].LoadFromResource(hInstance,IDB_CARDTYPE);

	m_ImageMeBanker.LoadFromResource(hInstance, IDB_ME_BANKER);
	m_ImageChangeBanker.LoadFromResource(hInstance, IDB_CHANGE_BANKER);
	m_ImageNoBanker.LoadFromResource(hInstance, IDB_NO_BANKER);	
	m_ImageTimeFlagPng.LoadImage(hInstance,_T("IDB_TIME_PNG"));
	m_TimerCount_png.LoadImage(hInstance,_T("IDB_TIMERCOUNT_PNG"));
	m_ImageMeScoreNumberPng.LoadImage(hInstance,_T("IDB_MENUM_PNG"));
	m_ImageScoreNumberPng.LoadImage(hInstance,_T("IDB_SCORE_PNG"));
	m_pngGameEnd.LoadImage(hInstance,_T("IDB_GAME_END_PNG"));
	m_ImageCh[7].LoadImage(hInstance,TEXT("IDB_XDZ"));
	m_ImageCh[2].LoadImage(hInstance,TEXT("IDB_DBC"));
	m_ImageCh[1].LoadImage(hInstance,TEXT("IDB_DBM"));
	m_ImageCh[0].LoadImage(hInstance,TEXT("IDB_DBSJ"));
	m_ImageCh[3].LoadImage(hInstance,TEXT("IDB_DDZ"));
	m_ImageCh[6].LoadImage(hInstance,TEXT("IDB_XBC"));
	m_ImageCh[5].LoadImage(hInstance,TEXT("IDB_XBM"));
	m_ImageCh[4].LoadImage(hInstance,TEXT("IDB_XBSJ"));

	//for (int i = 0;i<AREA_COUNT;i++) m_sT_ShowInfo.blShow[i]=false;

	
	TCHAR szBuffer[128]=TEXT("");
	//�����Ʊ���Դ
	for (int i = 0;i<7;i++)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_PUSHBOX%d"),i+1);
		m_PngPushBox[i].LoadImage(hInstance,szBuffer);
	}

	for (int i = 0;i<4;i++)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_JETTON%d"),i+1);
		m_PngShowJetton[i].LoadImage(hInstance,szBuffer);
	}

	for (int i = 0;i<4;i++)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_HANDLELEAVE%d"),i+1);
		m_PngShowLeaveHandle[i].LoadImage(hInstance,szBuffer);
	}


	for (int i = 0;i<4;i++)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_RESULT%d"),i+1);
		m_PngResult[i].LoadImage(hInstance,szBuffer);
	}

	m_bEnablePlaceJetton = false;

	m_pClientControlDlg = NULL;
	m_hInst = NULL;
	return;



	WCHAR szPath[255];
			static int  current = 0;
			::memset(szPath,0,sizeof(szPath));
			::GetCurrentDirectoryW(sizeof(szPath),szPath);
			wsprintfW(szPath,L"%s\\BumperCarBattle\\redcar1.png",szPath);
			//image(szPath, TRUE);

			m_pImage = Image::FromFile(szPath,TRUE);

			WCHAR szPath1[255];
			static int  current1 = 0;
			::memset(szPath1,0,sizeof(szPath1));
			::GetCurrentDirectoryW(sizeof(szPath1),szPath1);
			wsprintfW(szPath1,L"%s\\BumperCarBattle\\redcar2.png",szPath1);

			m_pImage1 = Image::FromFile(szPath1,TRUE);

			
	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
	if( m_pClientControlDlg )
	{
		delete m_pClientControlDlg;
		m_pClientControlDlg = NULL;
	}

	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
}
//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
//	m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );

	//��ע��ť
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	//m_btJetton50000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_50000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);

	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);

	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	m_btLast.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_LAST);
	m_btNext.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NEXT);

	m_btUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_UP);
	m_btDown.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_DOWN);



	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.LoadRgnImage(hResInstance,IDB_BT_JETTON_100,RGB(255,0,255));
	m_btJetton1000.LoadRgnImage(hResInstance,IDB_BT_JETTON_1000,RGB(255,0,255));
	//m_btJetton50000.LoadRgnImage(hResInstance,IDB_BT_JETTON_50000,RGB(255,0,255));
	m_btJetton10000.LoadRgnImage(hResInstance,IDB_BT_JETTON_10000,RGB(255,0,255));
	m_btJetton100000.LoadRgnImage(hResInstance,IDB_BT_JETTON_100000,RGB(255,0,255));
	m_btJetton1000000.LoadRgnImage(hResInstance,IDB_BT_JETTON_1000000,RGB(255,0,255));
	m_btJetton5000000.LoadRgnImage(hResInstance,IDB_BT_JETTON_5000000,RGB(255,0,255));

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false,false);


	m_btUp.SetButtonImage(IDB_BT_BT_S,hResInstance,false,false);
	m_btDown.SetButtonImage(IDB_BT_BT_X,hResInstance,false,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);

	m_btLast.SetButtonImage(IDB_BT_LAST,hResInstance,false,false);
	m_btNext.SetButtonImage(IDB_BT_NEXT,hResInstance,false,false);

	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	SwitchToCheck();
	SetJettonHide(0);

	//����
	m_hInst = NULL;
	m_pClientControlDlg = NULL;
	m_hInst = LoadLibrary(TEXT("BumperCarBattleClientControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(CWnd* pParentWnd); 
		CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl"); 
		if ( ClientControl )
		{
			m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
			m_pClientControlDlg->ShowWindow( SW_HIDE );
		}
	}


	//����
#ifdef __SPECIAL___
	if(m_DlgBank.m_hWnd==NULL) m_DlgBank.Create(IDD_BANK_STORAGE,this);
#endif

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinTianMen=false;
	m_bWinHuangMen=false;
	m_bWinXuanMen=false;
	m_bFlashResult=false;
	m_bShowGameResult=false;
	m_enDispatchCardTip=enDispatchCardTip_NULL;

	m_lMeCurGameScore=0L;
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	
	m_szpm=0;

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;
	qisx = 0; 

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	dbsj = 0;
	dbsjdt = 0;
	xbsj = 0;
	xbsjdt = 0;
	xbm = 0;
	xbmdt = 0;
	dbm = 0;
	dbmdt = 0;
	xbc = 0;
	xbcdt = 0;
	dbc = 0;
	dbcdt = 0;
	xdz = 0;
	xdzdt = 0;
	ddz = 0;
	ddzdt = 0;

	//����б�
//	m_ApplyUser.ClearAll();

	//�������
	CleanUserJetton();

	//���ð�ť
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

	return;
}

//�����ؼ�
void CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	//λ����Ϣ


	CSize Size;

	Size.cy = Size.cy/2;

	//CImageHandle ImageHandle(&m_ImageViewBack[0]);
	int iWidth =756;// m_ImageViewBack[0].GetWidth();
	int iHeight =705;// m_ImageViewBack[0].GetHeight();

	int LifeWidth = nWidth/2-iWidth/2;
	int TopHeight = nHeight/2-iHeight/2;

	int LifeWidth1 = nWidth/2+205;
	int TopHeight1 = -1;


	for (int i = 0; i<32; i++)
	{
		if(i ==0)
		{
			m_CarRect[i].top =nHeight/2-290+89;
			m_CarRect[i].left =nWidth/2-427+96;

		}
		if(i == 1||i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9)
		{
			m_CarRect[i].top =nHeight/2-225;
			m_CarRect[i].left =nWidth/2-344+i*60;
		}
		if(i ==10)
		{
			m_CarRect[i].top =nHeight/2-220+17;
			m_CarRect[i].left =nWidth/2+231+18;

		}
		if(i ==11)
		{
			m_CarRect[i].top =nHeight/2-167-120+50+78;
			m_CarRect[i].left =nWidth/2+371-70-14;

		}
		if(i ==12)
		{
			m_CarRect[i].top =nHeight/2-205-120+53*2+50+63;
			m_CarRect[i].left =nWidth/2+371+36-70-29;

		}
		if(i ==13)
		{
			m_CarRect[i].top =nHeight/2-130-120+53*2+95;
			m_CarRect[i].left =nWidth/2+375+36-96;

		}
		if(i ==14)
		{
			m_CarRect[i].top =nHeight/2-54-120+53*2+50+27;
			m_CarRect[i].left =nWidth/2+370+36-70-26;

		}
		if(i ==15)
		{
			m_CarRect[i].top =nHeight/2-53-120+53*2+68+70-5;
			m_CarRect[i].left =nWidth/2+371-91;

		}
		if(i ==16)
		{
			m_CarRect[i].top =nHeight/2-120+53*2+71+50;
			m_CarRect[i].left =nWidth/2+371-120;

		}
		if(i == 17||i == 18 || i == 19 || i == 20 || i == 21 || i == 22 || i == 23 || i == 24 || i == 25)
		{
			m_CarRect[i].top =nHeight/2-92+53*2+71+45;
			m_CarRect[i].left =nWidth/2-359+(26-i)*60+7;
		}
		if(i ==26)
		{
			m_CarRect[i].top =nHeight/2-120+53*2+74+50;
			m_CarRect[i].left =nWidth/2-362+10;

		}
		if(i ==27)
		{
			m_CarRect[i].top =nHeight/2-52-120+53*2+68+63;
			m_CarRect[i].left =nWidth/2-424+46;

		}
		if(i ==28)
		{
			m_CarRect[i].top =nHeight/2-54-120+53*2+77;
			m_CarRect[i].left =nWidth/2-399-10;

		}
		if(i ==29)
		{
			m_CarRect[i].top =nHeight/2-130-120+53*2+87;
			m_CarRect[i].left =nWidth/2-399-125+17;

		}
		if(i ==30)
		{
			m_CarRect[i].top =nHeight/2-204-120+53*2+113;
			m_CarRect[i].left =nWidth/2-398-120+130-17;

		}
		if(i ==31)
		{
			m_CarRect[i].top =nHeight/2-167+3;
			m_CarRect[i].left =nWidth/2-364-15;

		}

		CString strLog;
		strLog.Format(L"RECT m_CarRect[%d] %d  %d width:%d height:%d",i,m_CarRect[i].left,m_CarRect[i].top,nWidth,nHeight);
		OutputDebugString(strLog);

		
	}


	for (int i = 0;i<8;i++)
	{
		if(i==0)
		{
			m_RectArea[i].top =nHeight/2-116;
			m_RectArea[i].left =nWidth/2-296;
			m_RectArea[i].bottom =m_RectArea[i].top+112;
			m_RectArea[i].right  = m_RectArea[i].left+157;

		}
		if(i == 1)
		{
			m_RectArea[i].top =nHeight/2-116;
			m_RectArea[i].left =nWidth/2-137;
			m_RectArea[i].bottom =m_RectArea[i].top+112;
			m_RectArea[i].right  = m_RectArea[i].left+137;

		}
		if(i==2)
		{
			m_RectArea[i].top =nHeight/2-116;
			m_RectArea[i].left =nWidth/2+2;
			m_RectArea[i].bottom =m_RectArea[i].top+112;
			m_RectArea[i].right  = m_RectArea[i].left+137;

		}
		if(i == 3)
		{
			m_RectArea[i].top =nHeight/2-116;
			m_RectArea[i].left =nWidth/2+141;
			m_RectArea[i].bottom =m_RectArea[i].top+112;
			m_RectArea[i].right  = m_RectArea[i].left+157;

		}
		if(i == 4)
		{
			m_RectArea[i].top =nHeight/2-276-8+133+150;
			m_RectArea[i].left =nWidth/2-296;
			m_RectArea[i].bottom =m_RectArea[i].top+112;
			m_RectArea[i].right  = m_RectArea[i].left+157;

		}
		if(i == 5)
		{
			m_RectArea[i].top =nHeight/2-276-8+133+150;
			m_RectArea[i].left =nWidth/2-137;
			m_RectArea[i].bottom =m_RectArea[i].top+112;
			m_RectArea[i].right  = m_RectArea[i].left+137;

		}


		if(i == 6)
		{
			m_RectArea[i].top =nHeight/2-276-8+133+150;
			m_RectArea[i].left =nWidth/2+2;
			m_RectArea[i].bottom =m_RectArea[i].top+112;
			m_RectArea[i].right  = m_RectArea[i].left+137;

		}
		if(i == 7)
		{
			m_RectArea[i].top =nHeight/2-276-8+133+150;
			m_RectArea[i].left =nWidth/2+141;
			m_RectArea[i].bottom =m_RectArea[i].top+112;
			m_RectArea[i].right  = m_RectArea[i].left+157;

		}
	}

	int ExcursionY=10;
	for (int i = 0;i<AREA_COUNT;i++)
	{
		m_PointJettonNumber[i].SetPoint((m_RectArea[i].right+m_RectArea[i].left)/2, (m_RectArea[i].bottom+m_RectArea[i].top)/2-ExcursionY);
		m_PointJetton[i].SetPoint(m_RectArea[i].left, m_RectArea[i].top);
	}

	m_CardTypePoint[0] =CPoint(nWidth/2+125,nHeight/2-355+135+62); 

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(33);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

//	m_ApplyUser.m_viewHandle = m_hWnd;

	//�б�ؼ�
//	DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 - 308-14-50,nHeight/2-10-35,209,77,uFlags);

	DeferWindowPos(hDwp,m_btUp,NULL,LifeWidth1,TopHeight1+2,130/5,24,uFlags);
	DeferWindowPos(hDwp,m_btDown,NULL,LifeWidth1+138,TopHeight1+2,130/5,24,uFlags);

	m_MeInfoRect.top = TopHeight+10+12;
	m_MeInfoRect.left = LifeWidth+245+20+389;
	m_BaoPosion.top = TopHeight;
	m_BaoPosion.left = nWidth/2;


	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos =TopHeight+40+434+2+16+55;
	int nXPos =  LifeWidth+150 ;
	int nSpace = 7;

	m_TopHeight = TopHeight;
	m_LifeWidth = LifeWidth;

	m_nWinFlagsExcursionX = LifeWidth+152+48+10;	
	m_nWinFlagsExcursionY = TopHeight+545+7+93;

	//nSpace = 0;
	int nChoumaX = nWidth/2-94;
	int nChoumaY = nHeight/2 + 207;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nChoumaX,nChoumaY,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nChoumaX + nSpace + rcJetton.Width(),nChoumaY,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nChoumaX + nSpace * 2 + rcJetton.Width() * 2,nChoumaY,0,0,uFlags|SWP_NOSIZE);


	DeferWindowPos(hDwp,m_btJetton100000,NULL,nChoumaX + nSpace * 3 + rcJetton.Width() * 3,nChoumaY,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nChoumaX + nSpace * 4 + rcJetton.Width() * 4,nChoumaY,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nChoumaX + nSpace * 5 + rcJetton.Width() *5,nChoumaY,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btApplyBanker,NULL,m_LifeWidth+251+7,m_TopHeight+39+22+28-1,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btCancelBanker,NULL,m_LifeWidth+251+7,m_TopHeight+39+22+28-1,0,0,uFlags|SWP_NOSIZE);


	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,LifeWidth+205,TopHeight+562+62,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,LifeWidth+718-2,TopHeight+562+62,0,0,uFlags|SWP_NOSIZE);

	//������ť
	//������ť
	DeferWindowPos(hDwp,m_btLast,NULL,18-12,nHeight-42+7,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btNext,NULL,120-18,nHeight-42+7,0,0,uFlags|SWP_NOSIZE);
	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}
void CGameClientView::SwitchToCheck()
{
	SwithToNormalView();
	return;


//	m_ApplyUser.ShowWindow(SW_HIDE);
	m_btApplyBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btScoreMoveL.ShowWindow(SW_HIDE);
	m_btScoreMoveR.ShowWindow(SW_HIDE);
	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
//	m_btJetton50000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton5000000.ShowWindow(SW_HIDE);
	m_DrawBack = false;
	SwithToNormalView();


}
void CGameClientView::SwithToNormalView()
{

//		m_ApplyUser.ShowWindow(SW_SHOW);
		m_btApplyBanker.ShowWindow(SW_SHOW);
		m_btCancelBanker.ShowWindow(SW_SHOW);
		m_btScoreMoveL.ShowWindow(SW_SHOW);
		m_btScoreMoveR.ShowWindow(SW_SHOW);

		m_btJetton100.ShowWindow(SW_SHOW);
		m_btJetton1000.ShowWindow(SW_SHOW);
		m_btJetton10000.ShowWindow(SW_SHOW);
		//m_btJetton50000.ShowWindow(SW_HIDE);
		m_btJetton100000.ShowWindow(SW_SHOW);
		m_btJetton1000000.ShowWindow(SW_SHOW);
		m_btJetton1000000.ShowWindow(SW_SHOW);
		m_btJetton5000000.ShowWindow(SW_SHOW);
		m_DrawBack = true;
}

void CGameClientView::OnDown()
{

	//������ׯ�б�
	int nApplyUserCount = m_ApplyUser.GetCount();
	if(m_nApplyPageCount < (nApplyUserCount / 5))
	{
		m_nApplyPageCount ++;
	}
}

void CGameClientView::OnUp()
{
	//������ׯ�б�
	int nApplyUserCount = m_ApplyUser.GetCount();
	if(m_nApplyPageCount > 0)
	{
		m_nApplyPageCount --;
	}
}
void CGameClientView::OnLastMusic()
{
	CGameFrameEngine::GetInstance()->SendMessage(IDM_CHANGE_MUSIC,1,NULL);
}

void CGameClientView::OnNextMusic()
{
	CGameFrameEngine::GetInstance()->SendMessage(IDM_CHANGE_MUSIC,0,NULL);
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{

	//��ȡ���
	IClientUserItem  *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetClientUserItem(m_wBankerUser);
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	

	m_ViewBackPng.DrawImage(pDC,(nWidth-m_ViewBackPng.GetWidth())/2,nHeight/2-m_ViewBackPng.GetHeight()/2,\
		m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight(),0,0,m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight());
	m_carY2 = nHeight/2 + 90;
	
	m_carY = nHeight/2 - 182;

	m_carX1 = m_CarRect[8].left+17;
		m_carY1 = m_CarRect[8].top + 35;

//	m_ImageRedcar.DrawImage(pDC,m_carX1 + 20,m_carY1 + 5,m_ImageRedcar.GetWidth()/10,m_ImageRedcar.GetHeight()/12,(m_ImageRedcar.GetWidth()/10)*0,(m_ImageRedcar.GetHeight()/12)*8);
	
	m_carX3 = m_CarRect[24].left-3;
	m_carY3 = m_CarRect[24].top + 53;

// 	m_ImageRedcar.DrawImage(pDC,42,383,m_ImageRedcar.GetWidth()/10,m_ImageRedcar.GetHeight()/12,(m_ImageRedcar.GetWidth()/10)*3,(m_ImageRedcar.GetHeight()/12)*0);
// 	m_ImageRedcar.DrawImage(pDC,37,278,m_ImageRedcar.GetWidth()/10,m_ImageRedcar.GetHeight()/12,(m_ImageRedcar.GetWidth()/10)*7,(m_ImageRedcar.GetHeight()/12)*11);
// 	m_ImageRedcar.DrawImage(pDC,32,263,m_ImageRedcar.GetWidth()/10,m_ImageRedcar.GetHeight()/12,(m_ImageRedcar.GetWidth()/10)*4,(m_ImageRedcar.GetHeight()/12)*11);
// 	m_ImageRedcar.DrawImage(pDC,42,238,m_ImageRedcar.GetWidth()/10,m_ImageRedcar.GetHeight()/12,(m_ImageRedcar.GetWidth()/10)*1,(m_ImageRedcar.GetHeight()/12)*11);
	
	if(flagrrc==true)
	{
		CString strLog;

		if(m_CarIndex >= 2 && m_CarIndex <= 8)
		{
			strLog.Format(L"CARPOINT CarIndex:%d   %d,%d",m_CarIndex,m_carX,m_carY);
			OutputDebugString(strLog);
			m_ImageRedcar.DrawImage(pDC,m_carX,m_carY,m_ImageRedcar.GetWidth()/10,m_ImageRedcar.GetHeight()/12,(m_ImageRedcar.GetWidth()/10)*9,(m_ImageRedcar.GetHeight()/12)*8);
		}
		if(m_CarIndex >= 18 && m_CarIndex <= 24)
		{
			strLog.Format(L"CARPOINT CarIndex:%d   %d,%d",m_CarIndex, m_carX2, m_carY2);
			OutputDebugString(strLog);

			m_ImageRedcar.DrawImage(pDC, m_carX2, m_carY2, 
				m_ImageRedcar.GetWidth()/10,m_ImageRedcar.GetHeight()/12,(m_ImageRedcar.GetWidth()/10)*9,(m_ImageRedcar.GetHeight()/12)*2);
		}

		Graphics graphics(pDC->m_hDC);

		
		
		if(m_CarIndex >8 && m_CarIndex < 18)
		{
// 			strLog.Format(L"CARPOINT CarIndex:%d   %d,%d,mX1:%d,mY1:%d",m_CarIndex, m_carX1 + mX1,m_carY1 + mY1,mX1,mY1 );
// 			OutputDebugString(strLog);

			m_ImageRedcar.DrawImage(pDC,m_carX1 + mX1,m_carY1 + mY1,m_ImageRedcar.GetWidth()/10,m_ImageRedcar.GetHeight()/12,(m_ImageRedcar.GetWidth()/10)*tx1,(m_ImageRedcar.GetHeight()/12)*ty1);
		} 
		

		if(m_CarIndex > 24  || m_CarIndex >= 0&& m_CarIndex < 2)
		{
			strLog.Format(L"CARPOINT CarIndex:%d   %d,%d   mX3:%d,mY3:%d",m_CarIndex,m_carX3 + mX3,m_carY3 + mY3,mX3,mY3);
			OutputDebugString(strLog);

			m_ImageRedcar.DrawImage(pDC,m_carX3 + mX3,m_carY3 + mY3,m_ImageRedcar.GetWidth()/10,m_ImageRedcar.GetHeight()/12,(m_ImageRedcar.GetWidth()/10)*tx2,(m_ImageRedcar.GetHeight()/12)*ty2);
        }
	}

	if(m_blRungingCar)
	{
		m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());

	}
	if(m_bFlashrandShow)
	{
		m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());

	}
	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//״̬��ʾ
	CFont static InfoFont;
	InfoFont.CreateFont(150,20,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,255,0));

	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();

	//ʱ����ʾ
	if(m_DrawBack)
	{

		int nTimeFlagWidth = m_ImageTimeFlagPng.GetWidth()/3;
		int nFlagIndex=0;
		if (cbGameStatus==GAME_STATUS_FREE) nFlagIndex=0;
		else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=1;
		else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;

		/*m_ImageTimeFlagPng.DrawImage(pDC,nWidth/2-348+215+30, m_TopHeight+566+15+7+20+5-441+7-16, nTimeFlagWidth, m_ImageTimeFlagPng.GetHeight(),
			nFlagIndex*nTimeFlagWidth,0);*/

		WORD wUserTimer = GetUserClock(m_wMeChairID);

		if (wUserTimer!=0)
		{
			DrawNumberString(pDC,wUserTimer,m_LifeWidth+424,m_TopHeight+177,true);
		}

	}
	CRect rcDispatchCardTips(m_LifeWidth+612, m_TopHeight+297,m_LifeWidth+612+200, m_TopHeight+297+115);

	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//������Դ
	//CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize static SizeJettonItem(m_ImageJettonView.GetWidth()/6,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<AREA_COUNT&&m_DrawBack;i++)
	{
		//��������
		LONGLONG lScoreCount=0L;
		LONGLONG static lScoreJetton[JETTON_COUNT]={1L,5L,10L,100L,500L,1000L};
		int static nJettonViewIndex=0;

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			nJettonViewIndex=pJettonInfo->cbJettonIndex;

			m_ImageJettonView.DrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
				nJettonViewIndex*SizeJettonItem.cx,0);
			//�滭����
			//m_ImageJettonView.TransDrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
			//	pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
			//	nJettonViewIndex*SizeJettonItem.cx,0,RGB(255,0,255));
		}
		//�滭����
		if (lScoreCount>0L)
		{
			if(i == 0)
			{
				DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x + 15,m_PointJettonNumber[i].y + 10);	

			}
			if(i >=1 && i <= 3)
			{
				DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x-20,m_PointJettonNumber[i].y + 10 );	

			}
			if(i == 7)
			{
				DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x-20,m_PointJettonNumber[i].y );	

			}
			if(i >=4 && i <= 6)
			{
				DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
			}
		}
	}

	//�滭ׯ��
	if(m_DrawBack)
	{
		DrawBankerInfo(pDC,nWidth,nHeight);
		//�滭�û�
		DrawMeInfo(pDC,nWidth,nHeight);
	}
	//�滭������ʷ����
		DrawMemoryDate(pDC, nWidth, nHeight);
	//������ׯ�б�
	int nApplyUserCount = m_ApplyUser.GetCount();


	int nCenterX = nWidth /2+232;
	int nCenterY = 34;
	CRect StrRect[5];

	StrRect[0].left  = nCenterX ;
	StrRect[0].top   = nCenterY ;
	StrRect[0].right = StrRect[0].left  + 200;
	StrRect[0].bottom= StrRect[0].top + 12;

	StrRect[1].left  = nCenterX ;
	StrRect[1].top   = nCenterY +15;
	StrRect[1].right = StrRect[1].left + 200;
	StrRect[1].bottom= StrRect[1].top + 12;

	StrRect[2].left  = nCenterX;
	StrRect[2].top   = nCenterY +30;
	StrRect[2].right = StrRect[2].left+ 200;
	StrRect[2].bottom= StrRect[2].top + 12;

	StrRect[3].left  = nCenterX ;
	StrRect[3].top   = nCenterY + 45;
	StrRect[3].right = StrRect[3].left  + 200;
	StrRect[3].bottom= StrRect[3].top + 12;


	StrRect[4].left  = nCenterX;
	StrRect[4].top   = nCenterY + 60;
	StrRect[4].right = StrRect[4].left  + 200;
	StrRect[4].bottom= StrRect[4].top + 12;

	int j = 0;
	if(nApplyUserCount > 0)
	{
		tagApplyUser** ApplyUser = m_ApplyUser.GetData();
		int i = m_nApplyPageCount * 5;
		int PageCount = (m_nApplyPageCount+ 1) * 5;
		int j = 0;
		//int m = nApplyUserCount % 5;    //����һҳʣ������
		//int n = nApplyUserCount / 5;    //�ɷ�ҳ���� �ϼ� n * 5 + m

		for(; i < PageCount; i++)
		{
			CString str;
			if(ApplyUser[i] != NULL)
			{
				//��ǰ�����Ƿ� < �����б�����
				if(i < nApplyUserCount)
				{
					str.Format(TEXT("%s   %0.2f"), ApplyUser[i]->strUserName, ApplyUser[i]->lUserScore);
					if(i > 4)
					{
						//AfxMessageBox(ApplyUser[i]->strUserName);
						j = i - 5;
					}

					else
						j = i;
					CDFontEx::DrawText(this, pDC,  14, 400, str, StrRect[j], RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
				}
			}
		}


	
		i = 0;

		/*for(; i<nApplyUserCount; i++)
		{

			if(strcmp((char*)(ApplyUser[i]->strUserName).GetBuffer(), (char*)(pUserData->GetNickName())) == 0)
			{}
		}*/
		CRect static StrRect;

		StrRect.left = m_LifeWidth+40;
		StrRect.top = m_TopHeight+630;
		StrRect.right = StrRect.left + 144;
		StrRect.bottom = StrRect.top + 25;
		TCHAR Text[256];


		//���������б��е�λ��
		_sntprintf(Text,256,_TEXT("%s"),NumberStringWithSpace(nApplyUserCount));
		//CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
		if(m_bApplyBanker)
		{
		//	pDC->TextOut(nWidth/2 - 182, nHeight/2+40, Text);
			StrRect.left  = nCenterX - 182 + 275;
			StrRect.top   = nCenterY + 40 + 40;
			StrRect.right = StrRect.left + 120;
			StrRect.bottom= StrRect.top + 12;
			CDFontEx::DrawText(this,pDC,  14, 400, Text, StrRect, RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
		}
	}

	

	IClientUserItem  *pMyUserData=GetClientUserItem(m_wMeChairID);
	


	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160+120;

		//������ׯ
		if ( m_wMeChairID == m_wBankerUser )
		{
			//CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			//CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else
		{
			//CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
	}

    //������ʾ
	m_enDispatchCardTip = enDispatchCardTip_NULL;//��ʱ����
	if (enDispatchCardTip_NULL!=m_enDispatchCardTip)
	{
		if (m_ImageDispatchCardTip.IsNull()==false) m_ImageDispatchCardTip.Destroy();
		if (enDispatchCardTip_Dispatch==m_enDispatchCardTip)
			m_ImageDispatchCardTip.LoadFromResource(AfxGetInstanceHandle(),IDB_DISPATCH_CARD);
// 		else
// 			m_ImageDispatchCardTip.LoadFromResource(AfxGetInstanceHandle(),IDB_CONTINUE_CARD);
		//CImageHandle ImageHandle(&m_ImageDispatchCardTip);
		m_ImageDispatchCardTip.BitBlt(pDC->GetSafeHdc(), (nWidth-m_ImageDispatchCardTip.GetWidth())/2, nHeight/2);
	}

	//�ҵ���ע
	DrawMeJettonNumber(pDC);

	//�滭�н�����
	if(m_winflag == true)
	{
		m_Imagehkjbj.DrawImage(pDC,m_RectArea[0].left, m_RectArea[0].top,m_Imagehkjbj.GetWidth(),m_Imagehkjbj.GetHeight(),0,0);

		for (int i = 0;i<AREA_COUNT;i++)
		{
			if(m_bWinFlag[i])
			{
				if(i == 0 || i == 1 || i == 2 || i == 3)
				{
					/*int static iTickCount = ::GetTickCount();
					int static iTickTriger = 0;
					
					iTickCount = ::GetTickCount();*/
					//if(iTickCount > iTickTriger) 
					{
						//iTickTriger = ::GetTickCount() + 1000/500;
						if(i == 0)
						{
							if(qisx < 13)
							{
								m_ImageChbsjqz.DrawImage(pDC,nWidth/2 - 220, nHeight/2 - 230, 
									m_ImageChbsjqz.GetWidth()/13, m_ImageChbsjqz.GetHeight(), m_ImageChbsjqz.GetWidth()/13*qisx, 0);
							}
							else
							{
								if(chsxy < 3)
								{
									//AfxMessageBox(TEXT("000"));
									m_ImageChdbsdh.DrawImage(pDC,nWidth/2 - 215, nHeight/2 - 100, 
										m_ImageChdbsdh.GetWidth()/3, m_ImageChdbsdh.GetHeight()/3, 
										(m_ImageChdbsdh.GetWidth()/3)*chsxx, (m_ImageChdbsdh.GetHeight()/3)*chsxy);
									//chsxx++;
								}
							}
						}
						if(i == 1)
						{
							if(qisx < 13)
							{
							m_ImageChbmqz.DrawImage(pDC,nWidth/2 - 220, nHeight/2 - 230, 
								m_ImageChbsjqz.GetWidth()/13, m_ImageChbsjqz.GetHeight(), m_ImageChbsjqz.GetWidth()/13*qisx, 0);
							}
							else
							{
								if(chsxy < 3)
								{
									//AfxMessageBox(TEXT("111"));
									m_ImageChdbmdh.DrawImage(pDC,nWidth/2 - 215, nHeight/2 - 100, 
										m_ImageChdbmdh.GetWidth()/3, m_ImageChdbmdh.GetHeight()/3, 
										m_ImageChdbmdh.GetWidth()/3*chsxx, m_ImageChdbmdh.GetHeight()/3*chsxy);
									//chsxx++;
								}
							}
						}
						if(i == 2)
						{
							if(qisx < 13)
							{
							m_ImageChbcqz.DrawImage(pDC,nWidth/2 - 220, nHeight/2 - 230, 
								m_ImageChbsjqz.GetWidth()/13, m_ImageChbsjqz.GetHeight(), m_ImageChbsjqz.GetWidth()/13*qisx, 0);
							}
							else
							{
								if(chsxy < 3)
								{
									//AfxMessageBox(TEXT("222"));
									m_ImageChdbcdh.DrawImage(pDC,nWidth/2 - 215, nHeight/2 - 100, 
										m_ImageChdbcdh.GetWidth()/3, m_ImageChdbcdh.GetHeight()/3, 
										m_ImageChdbcdh.GetWidth()/3*chsxx, m_ImageChdbcdh.GetHeight()/3*chsxy);
									//chsxx++;
								}
							}
						}
						if(i == 3)
						{
							if(qisx < 13)
							{
							m_ImageChdzqz.DrawImage(pDC,nWidth/2 - 220, nHeight/2 - 230 + 50, 
								m_ImageChbsjqz.GetWidth()/13, m_ImageChbsjqz.GetHeight(), m_ImageChbsjqz.GetWidth()/13*qisx, 0);
							}
							else
							{
								if(chsxy < 3)
								{
									//AfxMessageBox(TEXT("333"));
									m_ImageChddzdh.DrawImage(pDC,nWidth/2 - 215, nHeight/2 - 100, 
										m_ImageChddzdh.GetWidth()/3, m_ImageChddzdh.GetHeight()/3, 
										m_ImageChddzdh.GetWidth()/3*chsxx, m_ImageChddzdh.GetHeight()/3*chsxy);
									//chsxx++;
								}
							}
						}
						/*qisx++;
						
						if(chsxx == 3 && chsxy == 0)
						{
							chsxx = 0;
							chsxy = 1;
						}
						if(chsxx == 3 && chsxy == 1)
						{
							chsxx = 0;
							chsxy = 2;
						}*/
						

					}
				}
				if(i == 4)
				{
					m_ImageChxbsj.DrawImage(pDC,m_RectArea[0].left + (m_Imagehkjbj.GetWidth() -m_ImageChxbsj.GetWidth())/2,
						m_RectArea[0].top + (m_Imagehkjbj.GetHeight() - m_ImageChxbsj.GetHeight())/2 ,
						m_ImageChxbsj.GetWidth(),m_ImageChxbsj.GetHeight(),0,0);
				}
				if(i == 5)
				{
					m_ImageChxbma.DrawImage(pDC,m_RectArea[0].left + (m_Imagehkjbj.GetWidth() -m_ImageChxbsj.GetWidth())/2,
						m_RectArea[0].top + (m_Imagehkjbj.GetHeight() - m_ImageChxbsj.GetHeight())/2 ,
						m_ImageChxbsj.GetWidth(),m_ImageChxbsj.GetHeight(),0,0);
				}
				if(i == 6)
				{
					m_ImageChxbci.DrawImage(pDC,m_RectArea[0].left + (m_Imagehkjbj.GetWidth() -m_ImageChxbsj.GetWidth())/2,
						m_RectArea[0].top + (m_Imagehkjbj.GetHeight() - m_ImageChxbsj.GetHeight())/2 ,
						m_ImageChxbsj.GetWidth(),m_ImageChxbsj.GetHeight(),0,0);
				}
				if(i == 7)
				{
					m_ImageChxdzo.DrawImage(pDC,m_RectArea[0].left + (m_Imagehkjbj.GetWidth() -m_ImageChxbsj.GetWidth())/2,
						m_RectArea[0].top + (m_Imagehkjbj.GetHeight() - m_ImageChxbsj.GetHeight())/2 ,
						m_ImageChxbsj.GetWidth(),m_ImageChxbsj.GetHeight(),0,0);
				}
			}
		}
	}
	
	//ʤ����־
	DrawWinFlags(pDC);

	
	if (m_pGameClientDlg->GetGameStatus()!=GAME_STATUS_FREE)
	{
		m_Out_Bao_y = 6;
	}
	if(m_blMoveFinish&&cbGameStatus==GS_GAME_END)
	{
		ShowGameResult(pDC, nWidth, nHeight);
	}
	if(m_DrawBack)
		DrawMoveInfo(pDC,m_rcTianMen);

	//�滭����
	if(m_bShowBao&&0)
	{
		if(m_bEnableSysBanker==0&&pUserData==NULL)
			;
		else
		{
			if(0<=m_Out_Bao_y&&m_Out_Bao_y<7)
			{
				if(m_Out_Bao_y>=6)
				{
					m_PngPushBox[m_Out_Bao_y].DrawImage(pDC,m_BaoPosion.left-m_PngPushBox[m_Out_Bao_y].GetWidth()/2 ,m_BaoPosion.top-120+6*45-50);

				}else
				{
					m_PngPushBox[m_Out_Bao_y].DrawImage(pDC,m_BaoPosion.left-m_PngPushBox[m_Out_Bao_y].GetWidth()/2 ,m_BaoPosion.top-120+m_Out_Bao_y*20-30);
				}
			}

		}
	}
	if(m_bShowJettonAn&&cbGameStatus!=GAME_STATUS_FREE&&0)
	{
		if(0<=m_bShowJettonIndex&&m_bShowJettonIndex<4)
		m_PngShowJetton[m_bShowJettonIndex].DrawImage(pDC,m_BaoPosion.left-m_PngShowJetton[m_bShowJettonIndex].GetWidth()/2 ,m_BaoPosion.top-120+6*20-30);
	}
	if(m_bShowLeaveHandleAn&&cbGameStatus==GS_GAME_END&&0)
	{
		if(0<=m_bShowLeaveHandleIndex&&m_bShowLeaveHandleIndex<4)
			m_PngShowLeaveHandle[m_bShowLeaveHandleIndex].DrawImage(pDC,m_BaoPosion.left-m_PngShowLeaveHandle[m_bShowLeaveHandleIndex].GetWidth()/2 ,m_BaoPosion.top-120+6*20-30);

	}
	return;
}

void CGameClientView::InsertApplyUser(tagApplyUser* pApplyUser)
{
	m_ApplyUser.Add(pApplyUser);
}

void CGameClientView::RemoveApplyUser(tagApplyUser* pApplyUser)
{
	//��ȡ������е��������
	int nApplyUserCount = (int)m_ApplyUser.GetCount();

	tagApplyUser** ApplyUser = m_ApplyUser.GetData();

	TCHAR szDestApplyUser[50] = {0};
	TCHAR szSrcApplyUser[50] = {0};
	wsprintf(szDestApplyUser, TEXT("%s"), pApplyUser->strUserName.GetBuffer());

	if(nApplyUserCount > 0)
	{
		for(int i = 0; i < nApplyUserCount; i++)
		{
			wsprintf(szSrcApplyUser, TEXT("%s"), ApplyUser[i]->strUserName);
			if(wcscmp(szSrcApplyUser, szDestApplyUser) == 0)
			{
				delete ApplyUser[i];
				ApplyUser[i] = NULL;
				m_ApplyUser.RemoveAt(i);
				Invalidate(FALSE);
				break;
			}
		}
	}

}

void CGameClientView::UpdateApplyUser(tagApplyUser* pApplyUser)
{
	//��ȡ������е��������
	int nApplyUserCount = (int)m_ApplyUser.GetCount();

	tagApplyUser** ApplyUser = m_ApplyUser.GetData();

	CString str;
	TCHAR szDestApplyUser[50] = {0};
	TCHAR szSrcApplyUser[50] = {0};
	wsprintf(szDestApplyUser, TEXT("%s"), pApplyUser->strUserName.GetBuffer());
	

	if(nApplyUserCount > 0)
	{

		for(int i = 0; i < nApplyUserCount; i++)
		{
			wsprintf(szSrcApplyUser, TEXT("%s"), ApplyUser[i]->strUserName);
			if(wcscmp(szSrcApplyUser, szDestApplyUser) == 0)
			{
				ApplyUser[i]->lUserScore = pApplyUser->lUserScore;
			//	AfxMessageBox(TEXT("�ҵ������Ѹ���"));
				break;
			}
		}
	}

}

//������Ϣ
void CGameClientView::SetMeMaxScore(SCORE lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(SCORE lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//���ñ���
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//���ó���
void CGameClientView::SetCurrentJetton(SCORE lCurrentJetton)
{
	//���ñ���
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;
	if(lCurrentJetton==0)
	{
		SetJettonHide(0);
	}
	return;
}


//��ʷ��¼
void CGameClientView::SetGameHistory(BYTE *bcResulte)
{
	//��������
	BYTE bcResulteTmp[AREA_COUNT];
	memcpy(bcResulteTmp,bcResulte,AREA_COUNT);
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];


	for (int i = 1;i<=AREA_COUNT;i++)
	{

		if(bcResulteTmp[i-1]>0)
		{
			GameRecord.enOperateMen[i]=enOperateResult_Win;

		}else
		{
			GameRecord.enOperateMen[i]=enOperateResult_Lost;

		}
		/*if (0==m_lUserJettonScore[i]) GameRecord.enOperateMen[i]=enOperateResult_NULL;
		else if (m_lUserJettonScore[i] > 0 && (bcResulte[i-1]==4)) GameRecord.enOperateMen[i]=enOperateResult_Win;
		else if (m_lUserJettonScore[i] > 0 && (0==bcResulte[i-1])) GameRecord.enOperateMen[i]=enOperateResult_Lost;*/

	}
	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( MAX_FALG_COUNT < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//�Ƶ����¼�¼
	if ( MAX_FALG_COUNT < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}

	return;
}

//�������
void CGameClientView::CleanUserJetton()
{
	//��������
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));


	ZeroMemory(m_JettonQue,sizeof(m_JettonQue));


	ZeroMemory(m_JettonQueArea,sizeof(m_JettonQueArea));

	


	m_JettonQueIndex = 0;

	



	//���½���
	UpdateGameView(NULL);

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	bool blHave = false;
	for (int i = 0;i<18;i++)
	{
		if(m_JettonQueArea[i]==cbViewIndex&&m_JettonQueIndex>i)
		{
			m_JettonQue[i] = m_lUserJettonScore[cbViewIndex];

			blHave= true;
			break;

		}
	}
	if(blHave==false)
	{
		if(m_JettonQueIndex>COUNT_DRAWINFO-1)
		{
			for (int i = 0;i<COUNT_DRAWINFO-1;i++)
			{
				m_JettonQueArea[i]= m_JettonQueArea[i+1];
				m_JettonQue[i] = m_JettonQue[i+1];

			}
			m_JettonQue[COUNT_DRAWINFO-1]=m_lUserJettonScore[cbViewIndex];
			m_JettonQueArea[COUNT_DRAWINFO-1] = cbViewIndex;
		}else 
		{
			m_JettonQueArea[m_JettonQueIndex]= cbViewIndex;
			m_JettonQue[m_JettonQueIndex++] = m_lUserJettonScore[cbViewIndex];

		}
		
		//m_JettonQueIndex = m_JettonQueIndex%16;

	}

	



	//���½���
	UpdateGameView(NULL);
}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[1][1])
{
	if (cbTableCardArray!=NULL)
	{
		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

		//��ʼ����
		DispatchCard();
	}
	else
	{
		ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	}
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, SCORE lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	//��������
	bool bPlaceJetton = false;
	LONGLONG lScoreIndex[JETTON_COUNT] = {1L,5L,10L,100L,500L,1000L};

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	m_lAllJettonScore[cbViewIndex] += lScoreCount;
	nFrameWidth = m_RectArea [cbViewIndex-1].right-m_RectArea [cbViewIndex-1].left;
	nFrameHeight = m_RectArea[cbViewIndex-1].bottom-m_RectArea [cbViewIndex-1].top;


	//�����ж�
	bool bAddJetton = lScoreCount>0?true:false;

	if( lScoreCount < 0)
	{
		lScoreCount = -lScoreCount;
	}

	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex = JETTON_COUNT-i-1;
		LONGLONG lCellCount = lScoreCount / lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONGLONG j=0;j<lCellCount;j++)
		{
			if (true == bAddJetton)
			{
				//�������
				tagJettonInfo JettonInfo;
				if(cbViewIndex-1 == 3)
				{
					int nJettonSize3 = 30;
					JettonInfo.cbJettonIndex=cbScoreIndex;
					int iWSize = nFrameWidth-nJettonSize3 - 70;
					int iHSize =  nFrameHeight-nJettonSize3 - 70;
					JettonInfo.nXPos=rand()%(iWSize);
					JettonInfo.nYPos=rand()%(iHSize) + 15;
				}
				if(cbViewIndex-1 == 7 )
				{
					int nJettonSize4 = 40;
					JettonInfo.cbJettonIndex=cbScoreIndex;
					int iWSize = nFrameWidth-nJettonSize4 - 40;
					int iHSize =  nFrameHeight-nJettonSize4-50;
					JettonInfo.nXPos=rand()%(iWSize);
					JettonInfo.nYPos=rand()%(iHSize);
				}
				if(cbViewIndex-1 == 0)
				{
					int nJettonSize2 = 30;
					JettonInfo.cbJettonIndex=cbScoreIndex;
					int iWSize = nFrameWidth-nJettonSize2 - 70;
					int iHSize =  nFrameHeight-nJettonSize2-70;
					JettonInfo.nXPos=rand()%(iWSize) + 50;
					JettonInfo.nYPos=rand()%(iHSize) + 15;
				}
				if(cbViewIndex-1 == 4)
				{
					int nJettonSize1 = 40;
					JettonInfo.cbJettonIndex=cbScoreIndex;
					int iWSize = nFrameWidth-nJettonSize1 - 40;
					int iHSize =  nFrameHeight-nJettonSize1-50;
					JettonInfo.nXPos=rand()%(iWSize) + 30;
					JettonInfo.nYPos=rand()%(iHSize);
				}
				if(cbViewIndex-1 == 1 || cbViewIndex-1 ==2 || cbViewIndex-1 == 5 || cbViewIndex-1 == 6 )
				{
					int nJettonSize=55;
					JettonInfo.cbJettonIndex=cbScoreIndex;
					int iWSize = nFrameWidth-nJettonSize-5;
					int iHSize =  nFrameHeight-nJettonSize-40;
					JettonInfo.nXPos=rand()%(iWSize);
					JettonInfo.nYPos=rand()%(iHSize);
				}

				//��������
				bPlaceJetton = true;
				m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
			}
			else
			{
				for (int nIndex=0; nIndex<m_JettonInfoArray[cbViewIndex-1].GetCount(); ++nIndex)
				{
					//�Ƴ��ж�
					tagJettonInfo &JettonInfo=m_JettonInfoArray[cbViewIndex-1][nIndex];
					if (JettonInfo.cbJettonIndex==cbScoreIndex)
					{
						m_JettonInfoArray[cbViewIndex-1].RemoveAt(nIndex);
						break;
					}
				}
			}
		}

		//������Ŀ
		lScoreCount -= lCellCount*lScoreIndex[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton==true) 
		UpdateGameView(NULL);

	return;
}

//��������ע
void CGameClientView::AndroidBet(BYTE cbViewIndex, SCORE lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) 
		return;

	if ( lScoreCount <= 0L )
		return;

	tagAndroidBet Androi;
	Androi.cbJettonArea = cbViewIndex;
	Androi.lJettonScore = lScoreCount;
	m_ArrayAndroid.Add(Androi);
	SetTimer(IDI_ANDROID_BET,100,NULL);
	int nHaveCount = 0;
	for ( int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
	{
		if(m_ArrayAndroid[i].lJettonScore > 0)
			nHaveCount++;
	}
	UINT nElapse = 0;
	if ( nHaveCount <= 1 )
		nElapse = 260;
	else if ( nHaveCount <= 2 )
		nElapse = 160;
	else
		nElapse = 100;

	SetTimer(IDI_ANDROID_BET+m_ArrayAndroid.GetCount(),nElapse,NULL);
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore, SCORE lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//����ʤ��
void CGameClientView::SetWinnerSide(bool blWin[], bool bSet)
{	
	//����ʱ��
	for (int i= 0;i<AREA_COUNT;i++){

		m_bWinFlag[i]=blWin[i];
	}
	if (true==bSet)
	{
		//���ö�ʱ��
		SetTimer(IDI_FLASH_WINNER,500,NULL);
		SetTimer(IDI_ZHONGJIANG,50,NULL);

		//ȫʤ�ж�
		bool blWinAll = true;

		for (int i= 0;i<AREA_COUNT;i++){

			if(m_bWinFlag[i]==true){
				blWinAll = false;
			}
		}
		if (blWinAll){
			//������Դ
			HINSTANCE hInstance=AfxGetInstanceHandle();
		}
	}
	else 
	{
		//�����ʱ��
		KillTimer(IDI_FLASH_WINNER);

		//ȫʤ�ж�
		bool blWinAll = true;

		for (int i= 0;i<AREA_COUNT;i++){

			if(m_bWinFlag[i]==true){
				blWinAll = false;
			}
		}
		if (blWinAll){
		}
	}

	//���ñ���
	m_bFlashResult=bSet;
	m_bShowGameResult=bSet;
	m_cbAreaFlash=0xFF;
	qisx = 0;
		chsxx = 0;
		chsxy = 0;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for (int i = 0;i<AREA_COUNT;i++)
	{
		if(m_RectArea[i].PtInRect(MousePoint))
		{
			return i+1;
		}
	}
	return 0xFF;
}
//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool blTimer,bool bMeScore)
{
	//������Դ
	//CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	//CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumberPng.GetWidth()/11,m_ImageScoreNumberPng.GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumberPng.GetWidth()/11, m_ImageMeScoreNumberPng.GetHeight());

	if(blTimer)
	{
		SizeScoreNumber.SetSize(m_TimerCount_png.GetWidth()/10, m_TimerCount_png.GetHeight());
	}

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	LONGLONG tmpNum= lNumberCount+lNumberCount/4;

	//λ�ö���
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=(INT)(nXPos+tmpNum*SizeScoreNumber.cx/2-SizeScoreNumber.cx);

	if (!blTimer)
		nXDrawPos = (INT)(nXPos+(SizeScoreNumber.cx-2)*lNumberCount/2+(SizeScoreNumber.cx-5)*(lNumberCount>3?0:(lNumberCount-1)/3)/2);

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		if(i!=0&&i%3==0)
		{
			if(blTimer)
			{
			}
			else
			{
				nXDrawPos += 5;
				if ( bMeScore )
				{
					m_ImageMeScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
						10*SizeScoreNumber.cx,0);
				}
				else
				{
					m_ImageScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
						10*SizeScoreNumber.cx,0);
				}
				nXDrawPos-=(SizeScoreNumber.cx-2);
			}
		}
		LONG lCellNumber=(LONG)(lNumber%10);
		if(blTimer)
		{
			m_TimerCount_png.DrawImage(pDC,nXDrawPos+55,nYDrawPos-68,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0);

		}
		else
		{
			if ( bMeScore )
			{
				m_ImageMeScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
			}
			else
			{
				m_ImageScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
			}

		}
		//���ñ���
		lNumber/=10;

		if (blTimer)
			nXDrawPos-=SizeScoreNumber.cx;
		else
			nXDrawPos-=(SizeScoreNumber.cx-2);
	};

	return;
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	SCORE lTmpNumber = lNumber;
	CString strNumber;
	CString strTmpNumber1;
	CString strTmpNumber2;
	bool blfirst = true;
	bool bLongNum = false;
	int nNumberCount = 0;

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();

	if ( lNumber == 0 )
		strNumber=TEXT("0");

	if ( lNumber < 0 ) 
		lNumber =- lNumber;

	if( lNumber >= 100 )
		bLongNum = true;

	while(lNumber > 0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if ( nNumberCount == 3 )
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}

			strNumber = strTmpNumber2;
			nNumberCount = 0;
			strTmpNumber2 = TEXT("");
		}
		lNumber /= 10;
	}

	if ( strTmpNumber2.IsEmpty() == FALSE )
	{
		if( bLongNum )
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber = strTmpNumber2;
	}

	if ( lTmpNumber < 0 ) 
		strNumber = TEXT("-") + strNumber;
	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}
//�滭����
CString CGameClientView::NumberStringWithSpace(LONGLONG lNumber)
{
	
	CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
     bool blfirst = true;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==3)
		{
			if(blfirst)
			{
				strTmpNumber2 +=strNumber;
				blfirst = false;

			}else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}
			
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(blfirst==false)
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;
	
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	return  strNumber;


}
CString CGameClientView::DrawNumberStringWithDot(DOUBLE lScore)
{
	CString strScore;
	CString strReturn;
	DOUBLE lNumber = lScore;
// 	if ( lScore < 0 )
// 		lNumber = -lNumber;

	strScore.Format(TEXT("%0.2lf"), lNumber);

// 	CString strDecimal;
// 	strDecimal = strScore.Right(3);
// 	strScore.Delete( strScore.Find('.'), 3);
// 
// 	int nStrCount = 0;
// 	for( int i = strScore.GetLength() - 1; i >= 0; )
// 	{
// 		if( (nStrCount%3) == 0 && nStrCount != 0 )
// 		{
// 			strReturn.Insert(0, ',');
// 			nStrCount = 0;
// 		}
// 		else
// 		{
// 			strReturn.Insert(0, strScore.GetAt(i));
// 			nStrCount++;
// 			i--;
// 		}
// 	}
// 
// 	if ( lScore < 0 )
// 		strReturn.Insert(0, '-');
// 
// 	//if ( bPlus && lScore > 0 )
// 	//	strReturn.Insert(0, '+');
// 
// 	strReturn += strDecimal;


	return strScore;

}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, SCORE lNumber, CRect rcRect, INT nFormat)
{
	SCORE lTmpNumber = lNumber;
	CString strNumber;
	CString strTmpNumber1;
	CString strTmpNumber2;
	bool blfirst = true;
	bool bLongNum = false;
	int nNumberCount = 0;

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();

// 	if ( lNumber == 0 )
// 		strNumber=TEXT("0");
// 
// 	if ( lNumber < 0 ) 
// 		lNumber =- lNumber;
// 
// 	if( lNumber >= 100 )
// 		bLongNum = true;
// 
// 	while(lNumber > 0)
// 	{
// 		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
// 		nNumberCount++;
// 		strTmpNumber2 = strTmpNumber1+strTmpNumber2;
// 
// 		if ( nNumberCount == 3 )
// 		{
// 			if(blfirst)
// 			{
// 				strTmpNumber2 += (TEXT("") +strNumber);
// 				blfirst = false;
// 			}
// 			else
// 			{
// 				strTmpNumber2 += (TEXT(",") +strNumber);
// 			}
// 
// 			strNumber = strTmpNumber2;
// 			nNumberCount = 0;
// 			strTmpNumber2 = TEXT("");
// 		}
// 		lNumber /= 10;
// 	}
// 
// 	if ( strTmpNumber2.IsEmpty() == FALSE )
// 	{
// 		if( bLongNum )
// 			strTmpNumber2 += (TEXT(",") +strNumber);
// 		else
// 			strTmpNumber2 += strNumber;
// 
// 		strNumber = strTmpNumber2;
// 	}

	strNumber.Format(L"%.2lf",lNumber);

// 	if ( lTmpNumber < 0 ) 
// 		strNumber = TEXT("-") + strNumber;
	//�������
	if (nFormat==-1) //pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
		CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, RGB(255,255,255), DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else //pDC->DrawText(strNumber,rcRect,nFormat);
		CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, RGB(255,255,255), nFormat);
}
void CGameClientView::SetMoveCardTimer()
{
	KillTimer(IDI_POSTCARD);
	SetTimer(IDI_POSTCARD,300,NULL);
	SetTimer(IDI_DISPATCH_CARD,5000,NULL);

}
void CGameClientView::KillCardTime()
{
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_ZHONGJIANG);
	KillTimer(IDI_POSTCARD);
	KillTimer(IDI_OPENCARD);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_SHOWDISPATCH_CARD_TIP);
	KillTimer(IDI_RUN_REDCAR);

}
void CGameClientView::StartRunCar(int iTimer)
{
	m_bFlashrandShow = false;
	KillTimer(IDI_FLASH_RAND_SIDE);
	KillTimer(IDI_FLASH_CARD);
	KillTimer(IDI_CAR);
	KillTimer(IDI_SHOW_CARD_RESULT);
	iTimerStep = 400;
	iTimerStep1 =iTimerStep/10;

	tx1 = 5;
	ty1 = 8;
	m_x1 = 10;
	mX1 = 25;
	mY1 = 5;
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);
	SetTimer(IDI_CAR,iTimerStep1,NULL);
	
	ifirstTimer = 0;
	iOpenSide = m_cbTableCardArray[0][0];
	iTotoalRun = iOpenSide+32*3-2;
	iRunIndex = 0;
	m_CarIndex = 1;
	mX = 0;
	mX2 = 0;
	mX3 = 0;
	mY3 = 0;
	m_CarIndexod = 0;
	m_blRungingCar = true;
	flagrrc = true;
	
	SetTimer(IDI_SHOW_CARD_RESULT,12*1000,NULL);
	

}
void CGameClientView::RuningCar(int iTimer)
{
	if(iRunIndex<9)
	{
		iTimerStep-=43;
	}
	if(iRunIndex >= iTotoalRun-10)
	{
		iTimerStep+=47 /*8*(iRunIndex-iTotoalRun+15)*/;
	}
	if(iRunIndex==iTotoalRun)
	{
			KillTimer(IDI_FLASH_CARD);
			KillTimer(IDI_SHOW_CARD_RESULT);
			m_blRungingCar = false;
			m_blMoveFinish = true;
			//���ö�ʱ��
			this->DispatchCard();
			FinishDispatchCard();
			return ;

	}
	if(iTimerStep<0)
	{
		return ;
	}
	KillTimer(IDI_FLASH_CARD);
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);

}
void CGameClientView::FlashWinOpenSide()
{

}
void CGameClientView::StartRandShowSide()
{
	KillTimer(IDI_FLASH_RAND_SIDE);
	iTimerStep = 100;
	m_bFlashrandShow = true;
	SetTimer(IDI_FLASH_RAND_SIDE,iTimerStep,NULL);

   
}
//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	if(IDI_FLASH_CARD == nIDEvent)
	{
		m_CarIndex = (m_CarIndex+1)%32;
		iRunIndex++;
		 RuningCar(iTimerStep);
		 if(iRunIndex == iTotoalRun)
		 {
		CGameFrameEngine::GetInstance()->SendMessage(IDM_SOUND,3,3);
			}
		//���½���
		UpdateGameView(NULL);
	}
	
	if(IDI_CAR == nIDEvent)
	{
		OutputDebugString(L"ONTIMER IDI_CAR");
		if(m_CarIndex <= 2 || m_CarIndex > 8)
		{
			mX = 0;
		}
		if(m_CarIndex >= 2 && m_CarIndex <= 8)
		{
			m_carX = m_CarRect[2].left + 4*mX;
		}
		mX +=3;

		if(m_CarIndex == 17)
		{
			mX2 = 0;
		}
		if(m_CarIndex >= 17 && m_CarIndex < m_CarIndex <= 24)
		{
			m_carX2 = m_CarRect[17].left - 50 - 4*mX2;
		}
		mX2+= 3;

			/*if(m_CarIndex == 7)
				iP = -90;
			if(m_CarIndex >8 && m_CarIndex < 18)
				iP = iP + 2;
			if(m_CarIndex < 24 && m_CarIndex >2)
				iP1 = 90;
			if(m_CarIndex > 24  || m_CarIndex >= 0&& m_CarIndex < 2)
				iP1 = iP1 + 2;*/
	
		KillTimer(IDI_CAR);
		SetTimer(IDI_CAR, iTimerStep1, NULL);
		
		if(iRunIndex >= 8 && iRunIndex < iTotoalRun-10)
		{
			KillTimer(IDI_CAR);
			SetTimer(IDI_CAR1, 26, NULL);
		}
		UpdateGameView(NULL);
	}
	if(IDI_CAR1 == nIDEvent)
	{
		if(m_CarIndex <= 2 || m_CarIndex >= 10)
		{
			mX = 0;
			m_carX = m_CarRect[2].left + 15;
		}
		if(m_CarIndex >= 2 && m_CarIndex <= 8)
		{
			m_carX = m_CarRect[2].left + 15 + 18*mX;
		
			mX += 2;
		}
		if(m_CarIndex <= 17)
		{
			mX2 = 0;
		}
		if(m_CarIndex >= 17 && m_CarIndex < m_CarIndex <= 24)
		{
			m_carX2 = m_CarRect[17].left -55 - 13*mX2;
		}
		mX2+= 2;
		//�ұ߰�Բ
		
		if(m_CarIndex <= 8)
		{
			m_x1 = 10;
			mX1 = 35;
			mY1 = 5;
		}
		if(m_CarIndex > 8 && m_CarIndex < 18)
		{
			if(m_x1 == 10)
			{
				mX1 = 45;
				mY1 = 15;
				tx1 = 1;
				ty1 = 8;
			}
			if(m_x1 == 13)
			{
				mX1 = 60;
				mY1 = 18;
				tx1 = 8;
				ty1 = 7;
			}
			if(m_x1 == 16)
			{
				mX1 = 70;
				mY1 = 25;
				tx1 = 5;
				ty1 = 7;
			}
			if(m_x1 == 19)
			{
				mX1 = 90;
				mY1 = 35;
				tx1 = 3;
				ty1 = 7;
			}
			if(m_x1 == 22)
			{
				mX1 = 105;
				mY1 = 40;
				tx1 = 1;
				ty1 = 7;
			}
			if(m_x1 == 25)
			{
				mX1 = 125;
				mY1 = 60;
				tx1 = 9;
				ty1 = 6;
			}
			if(m_x1 == 28)
			{
				mX1 = 140;
				mY1 = 65;
				tx1 = 7;
				ty1 = 6;
			}
			if(m_x1 == 31)
			{
				mX1 = 150;
				mY1 = 110;
				tx1 = 6;
				ty1 = 6;
			}
			if(m_x1 == 34)
			{
				mX1 = 160;
				mY1 = 120;
				tx1 = 3;
				ty1 = 6;
			}
			if(m_x1 == 37)
			{
				mX1 = 160;
				mY1 = 130;
				tx1 = 0;
				ty1 = 6;
			}
			if(m_x1 == 40)
			{
				mX1 = 150;
				mY1 = 160;
				tx1 = 7;
				ty1 = 5;
			}
			if(m_x1 == 43)
			{
				mX1 = 140;
				mY1 = 190;
				tx1 = 4;
				ty1 = 5;
			}
			if(m_x1 == 46)
			{
				mX1 = 125;
				mY1 = 215;
				tx1 = 1;
				ty1 = 5;
			}
			if(m_x1 == 49)
			{
				mX1 = 100;
				mY1 = 230;
				tx1 = 8;
				ty1 = 4;
			}
			if(m_x1 == 52)
			{
				mX1 = 95;
				mY1 = 250;
				tx1 = 5;
				ty1 = 4;
			}
			if(m_x1 == 55)
			{
				mX1 = 95;
				mY1 = 265;
				tx1 = 2;
				ty1 = 4;
			}
			if(m_x1 == 58)
			{
				mX1 = 70;
				mY1 = 285;
				tx1 = 9;
				ty1 = 3;
			}
			if(m_x1 == 61)
			{
				mX1 = 45;
				mY1 = 292;
				tx1 = 6;
				ty1 = 3;
			}
			if(m_x1 == 64)
			{
				mX1 = 35;
				mY1 = 295;
				tx1 = 3;
				ty1 = 3;
			}

			m_x1 += 3;
		}

		
		//��߰�Բ
		


		if(m_CarIndex <= 24 && m_CarIndex >= 2)
		{
			m_x2 = 13;
			mX3 = -15;
				mY3 = -80;
				tx2 = 9;
				ty2 = 2;
		}
		if(m_CarIndex > 24 || m_CarIndex < 2)
		{
			if(m_CarIndex == 25)
			{
				m_carX3 = m_CarRect[24].left;
				m_carY3 = m_CarRect[24].top - 55;
				mX3 = -60;
				mY3 = -95;
				m_x2 = 13;
				tx2 = 6;
				ty2 = 2;

			}


			if(m_x2 == 13)
			{
				mX3 = -70;
				mY3 = -110;
				tx2 = 3;
				ty2 = 2;
			}
			if(m_x2 == 16)
			{
				mX3 = -80;
				mY3 = -120;
				tx2 = 2;
				ty2 = 2;
			}
			if(m_x2 == 19)
			{
				mX3 = -85;
				mY3 = -135;
				tx2 = 1;
				ty2 = 2;
			}
			if(m_x2 == 22)
			{
				mX3 = -90;
				mY3 = -140;
				tx2 = 6;
				ty2 = 1;
			}
			if(m_x2 == 25)
			{
				mX3 = -95;
				mY3 = -150;
				tx2 = 5;
				ty2 = 1;
			}
			if(m_x2 == 28)
			{
				mX3 = -100;
				mY3 = -160;
				tx2 = 2;
				ty2 = 1;
			}
			if(m_x2 == 31)
			{
				mX3 = -105;
				mY3 = -170;
				tx2 = 9;
				ty2 = 0;
			}

			if(m_x2 == 34)
			{
				mX3 = -115;
				mY3 = -180;
				tx2 = 6;
				ty2 = 0;
			}
			if(m_x2 == 37)
			{
				mX3 = -115;
				mY3 = -200;
				tx2 = 3;
				ty2 = 0;
			}
			if(m_x2 == 40)
			{
				mX3 = -135;
				mY3 =- 255;
				tx2 = 7;
				ty2 = 11;
			}
			if(m_x2 == 43)
			{
				mX3 = -125;
				mY3 = -270;
				tx2 = 4;
				ty2 = 11;
			}
			if(m_x2 == 46)
			{
				mX3 =- 115;
				mY3 = -295;
				tx2 = 1;
				ty2 = 11;
			}

			if(m_x2 == 49)
			{
				mX3 = -100;
				mY3 =- 305;
				tx2 = 8;
				ty2 = 10;
			}
			if(m_x2 == 52)
			{
				mX3 = -90;
				mY3 = -315;
				tx2 = 5;
				ty2 = 10;
			}
			if(m_x2 == 55)
			{
				mX3 = -85;
				mY3 = -320;
				tx2 = 2;
				ty2 = 10;
			}
			if(m_x2 == 58)
			{
				mX3 = -80;
				mY3 =- 335;
				tx2 = 9;
				ty2 = 9;
			}

			if(m_x2 == 61)
			{
				mX3 = -55;
				mY3 = -345;
				tx2 = 6;
				ty2 = 9;
			}
			if(m_x2 == 64)
			{
				mX3 = -35;
				mY3 = -350;
				tx2 = 3;
				ty2 = 9;
			}



			m_x2 += 3;
		}
		
		KillTimer(IDI_CAR1);
		SetTimer(IDI_CAR1, 26, NULL);
		if(iRunIndex >= iTotoalRun-9)
		{
			m_x2 += 3;
			m_js = 1;
			mX = 18*mX;
			mX2 = 18*mX2;
			iTimerStep1 = 20;
			KillTimer(IDI_CAR1);
			SetTimer(IDI_CAR2, 20, NULL);
		}
		UpdateGameView(NULL);
	}
	if(IDI_CAR2 == nIDEvent)
	{
		if(m_CarIndex <= 1 || m_CarIndex > 8)
			{
				mX = 0;
			}
			if(m_CarIndex >= 2 && m_CarIndex <= 8)
			{
				m_carX = m_CarRect[2].left - 55 + mX;
			}
			
			mX += 12;

			if(m_CarIndex > 24 || m_CarIndex < 17)
			{
				mX2 = 0;
			}
			if(m_CarIndex >= 17 && m_CarIndex < m_CarIndex <= 24)
			{
				m_carX2 = m_CarRect[17].left + 11 - mX2;
			}
			mX2 += 6;
			
		

			//�ұ߰�Բ
			if(m_CarIndex <= 8 || m_CarIndex >= 18)
			{
				m_x1 = 10;
				ty1 = 8;
			}


			if(m_CarIndex > 8 && m_CarIndex < 18)
			{
				
				if(m_CarIndex == 9)
				{
					mX1 = mX1;
					mY1 = mY1;
				
					mX1 += 2;
					mY1 += 2;

				}
				if(m_CarIndex == 10)
				{
					mX1 = mX1;
					mY1 = mY1;
				
					mX1 += 3;
					mY1 += 3;
				}
				
				if(m_CarIndex == 11)
				{
					mX1 = mX1;
					mY1 = mY1;
					
					mX1 += 5;
					mY1 += 8;
				}
				
				if(m_CarIndex == 12)
				{
					mX1 = mX1;
					mY1 = mY1;
					
					mX1 += 3;
					mY1 += 9;
				}
				
				if(m_CarIndex == 13)
				{
					mX1 = mX1;
					mY1 = mY1;
					
					mX1 += 3;
					mY1 += 5;
				}
				
				if(m_CarIndex == 14)
				{
					mX1 = mX1;
					mY1 = mY1;
					
					mX1 -= 3;
					mY1 += 4;
				}
			
				if(m_CarIndex == 15)
				{
					mX1 = mX1;
					mY1 = mY1;
					
					mX1 -= 3;
					mY1 += 8;
				}
				
				if(m_CarIndex == 16)
				{
					mX1 = mX1;
					mY1 = mY1;
					
					mX1 -= 1;
					mY1 += 5;
				}
				
				if(m_CarIndex == 17)
				{
					mX1 = mX1;
					mY1 = mY1;
					
					mX1 -= 4;
					mY1 += 1;
				}
				
				
				tx1-= 1;
				if(tx1 < 0)
				{
					tx1 += 10;
					ty1 -= 1;
				}
				CString strLog;
				strLog.Format(L"\nIDI_CAR2 %d,%d m_x1:%d,m_CarIndex:%d,mX1:%d,mY1:%d,tx1:%d,ty1:%d",mX1+157,mY1+533 ,m_x2,m_CarIndex,mX1,mY1,tx1,ty1);
				OutputDebugString(strLog);

			}
		
		
		//��߰�Բ
			

			if(m_CarIndex <= 24 && m_CarIndex >= 2)
			{
				tx2 = 6;
				ty2 = 2;
				m_x2 = 13;
				m_js = 0;
				mX3 = -35;
				mY3 = -95;
				mX3 = -15;
				mY3 = -80;

			}

			if(m_CarIndex > 24 || m_CarIndex < 2)
			{
				if(m_x2 >= 10 && m_x2 < 13)//25
				{
					if(m_js == 1)
					{
						mX3 = -45;
						mY3 = -100;
						m_js++;
						tx2 = 4;
						ty2 = 2;
					}
 					else
 					{
 						mX3 -= 1;
 						mY3 -= 1;
 					}
				}

				if(m_x2 >= 13 && m_x2 < 16)  // 25
				{

					if(m_js == 1)
					{
						mX3 = -47;
						mY3 = -103;
						m_js++;
						tx2 = 3;
						ty2 = 2;
					}
					else
					{
						mX3 -= 2;
						mY3 -= 2;
					}
				}
				if(m_x2 >= 16 && m_x2 < 19)//   25
				{

					if(m_js == 1)
					{
						mX3 = -55;
						mY3 = -107;
						m_js++;
						tx2 = 2;
						ty2 = 2;
					}
					else
					{
						mX3 -= 3;
						mY3 -= 1;
					}
				}
				if(m_x2 >= 19 && m_x2 < 22)  //26
				{

					if(m_js == 1)
					{
						mX3 = -65;
						mY3 = -110;
						m_js++;
						tx2 = 1;
						ty2 = 2;					
					}
					else
					{
						mX3 -= 3;
						mY3 -= 5;
					}
				}
				if(m_x2 >= 22 && m_x2 < 25)//  26
				{

					if(m_js == 1)
					{
						mX3 = -75;
						mY3 = -125;
						m_js++;
						tx2 = 8;
						ty2 = 1;					
					}
					else
					{
						mX3 -= 10;
						mY3 -= 5;
					}
				}
				if(m_x2 >= 25 && m_x2 < 28) //  27
				{

					if(m_js == 1)
					{
						mX3 = -100;
						mY3 = -140;
						m_js++;
						tx2 = 2;
						ty2 = 1;
					}
					else
					{
						mX3 -= 4;
						mY3 -= 6;
					}
				}
				if(m_x2 >= 28 && m_x2 < 31)//27   here
				{

					if(m_js == 1)
					{
						mX3 = -110;
						mY3 = -160;
						m_js++;
						tx2 = 2;
						ty2 = 1;
					}
					else
					{
						mX3 -= 2;
						mY3 -= 5;
					}
				}
				if(m_x2 >= 31 && m_x2 < 34) //27 and 28
				{

					if(m_js == 1)
					{
						mX3 = -115;
						mY3 = -175;
						m_js++;
						tx2 = 9;
						ty2 = 0;
					}
					else
					{
						mX3 -= 2;
						mY3 -= 1;
					}
				}

				if(m_x2 >= 34 && m_x2 < 37)//28
				{

					if(m_js == 1)
					{
						mX3 = -120;
						mY3 = -175;
						m_js++;
						tx2 = 8;
						ty2 = 0;
					}
					else
					{
						mX3 -= 6;
						mY3 -= 6;
					}
				}
				if(m_x2 >= 37 && m_x2 < 40) //28 and 29
				{
					if(m_js == 1)
					{
						mX3 = -128;
						mY3 = -220;
						m_js++;
						tx2 = 3;
						ty2 = 0;
					}
					else
					{
						mX3 -= 4;
						mY3 -= 2;
					}
				}
				if(m_x2 >= 40 && m_x2 < 43)//  29
				{
					if(m_js == 1)
					{
						mX3 = -130;
						mY3 = -225;
						m_js++;
						tx2 = 0;
						ty2 = 0;
					}
					else
					{
						mX3 -= 1;
						mY3 -= 9;
					}
				}
				if(m_x2 >= 43 && m_x2 < 46)  //30
				{
					if(m_js == 1)
					{
						mX3 = -125;
						mY3 = -275;
						m_js++;
						tx2 = 2;
						ty2 = 11;
					}
					else
					{
						mX3 += 6;
						mY3 -= 9;
					}
				}
				if(m_x2 >= 46 && m_x2 < 49)//30  and 31
				{
					if(m_js == 1)
					{
						mX3 = -105;
						mY3 = -315;
						m_js++;
						tx2 = 8;
						ty2 = 10;
					}
					else
					{
						mX3 += 2;
						mY3 -= 15;
					}
				}

				if(m_x2 >= 49 && m_x2 < 52) //31  here
				{
					if(m_js == 1)
					{
						mX3 = -100;
						mY3 = -320;
						m_js++;
						tx2 = 5;
						ty2 = 10;
					}
					else
					{
						mX3 += 3;
						mY3 -= 4;
					}
				}
				if(m_x2 >= 52 && m_x2 < 55)// 31  and 0
				{

					if(m_js == 1)
					{
						mX3 = -72;
						mY3 = -342;
						m_js++;
						tx2 = 3;
						ty2 = 10;
					}
					else
					{
						mX3 += 4;
						mY3 -= 2;
					}
				}
				if(m_x2 >= 55 && m_x2 < 58)  //  0
				{
					if(m_js == 1)
					{
						mX3 = -69;
						mY3 = -345;
						m_js++;
						tx2 = 0;
						ty2 = 10;
					}
					else
					{
						mX3 += 3;
						mY3 -= 5;
					}
				}
				if(m_x2 >= 58 && m_x2 < 61)//0  and   1
				{

					if(m_js == 1)
					{
						mX3 = -40;
						mY3 =- 350;
						m_js++;
						tx2 = 4;
						ty2 = 9;
					}
					else
					{
						mX3 += 4;
						mY3 -= 8;
					}
				}

				if(m_x2 >= 61 && m_x2 < 64)  // 1
				{

					if(m_js == 1)
					{
						mX3 = -35;
						mY3 = -358;
						m_js++;
						tx2 = 2;
						ty2 = 9;
					}
					else
					{
						mX3 += 6;
						mY3 -= 5;
					}
				}
				if(m_x2 >= 64)   // 1
				{

					if(m_js == 1)
					{
						mX3 = -30;
						mY3 = -360;
						m_js++;
						tx2 = 0;
						ty2 = 9;
					}
					else
					{
						mX3 += 7;
						mY3 -= 4;
					}
				}


				m_x2++;
				tx2 -= 1;
				if(tx2 < 0 && ty2 > 0)
				{
					tx2 += 10;
					ty2 -= 1;
				}
				if(tx2 < 0 && ty2 == 0)
				{
					tx2 += 8;
					ty2 = 11;
				}

				CString strLog;
				strLog.Format(L"CARFLASH m_CardIndex:%d,m_x1:%d,m_carX1+mX1:%d+%d,m_carY1+mY1:%d+%d,tx1:%d,ty1:%d",m_CarIndex,m_x2,m_carX3,mX3,m_carY3,mY3,tx2,ty2);
				OutputDebugString(strLog);

			}

		iTimerStep1 = iTimerStep1 + 1;
		KillTimer(IDI_CAR2);
		SetTimer(IDI_CAR2, iTimerStep1, NULL);
		if(iRunIndex == iTotoalRun)
		{
			KillTimer(IDI_CAR2);
			return;
		}
		
		UpdateGameView(NULL);
	}
	if(IDI_SHOW_CARD_RESULT==nIDEvent)
	{
		for (int i = iRunIndex;i<iTotoalRun;i++)
		{
			
			m_CarIndex = (m_CarIndex+1)%32;
			iRunIndex++;
			if(iRunIndex==iTotoalRun)
			{
				KillTimer(IDI_FLASH_CARD);
				KillTimer(IDI_CAR);
				KillTimer(IDI_SHOW_CARD_RESULT);
				m_blRungingCar = false;
				m_blMoveFinish = true;
				//flagrrc = false;
				//���ö�ʱ��
				this->DispatchCard();
				FinishDispatchCard();
				return ;
			}
		}
	}

	if(IDI_FLASH_RAND_SIDE ==nIDEvent )
	{
		m_bFlashrandShow=!m_bFlashrandShow;
		m_CarIndex = rand()%32;
		//���½���
		UpdateGameView(NULL);
	}
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		m_winflag = true;
		//flagrrc = false;
		//���ñ���
		
		m_bFlashResult=!m_bFlashResult;
		

		//���½���
		UpdateGameView(NULL);
		return;
	}
	if(IDI_ZHONGJIANG == nIDEvent)
	{
		qisx++;
		if(qisx >= 13 && qisx <= 20)
		{
			chsxx++;
		}

		if(chsxx == 3 && chsxy == 0)
		{
			chsxx = 0;
			chsxy = 1;
		}
		if(chsxx == 3 && chsxy == 1)
		{
			chsxx = 0;
			chsxy = 2;
		}

		KillTimer(IDI_ZHONGJIANG);
		SetTimer(IDI_ZHONGJIANG,150,NULL);
		//���½���
		UpdateGameView(NULL);
		return;
	}
	//�ֻ�ׯ��
	else if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );
		return;
	}
	else if (nIDEvent==IDI_DISPATCH_CARD)
	{
		return;
	}
	else if (IDI_SHOWDISPATCH_CARD_TIP==nIDEvent)
	{
		SetDispatchCardTip(enDispatchCardTip_NULL);
	}
	else if ( nIDEvent == IDI_ANDROID_BET )
	{
		//���½���
		UpdateGameView(NULL);
		return;
	}
	else if ( nIDEvent >= (UINT)(IDI_ANDROID_BET + 1) && nIDEvent < (UINT)(IDI_ANDROID_BET + m_ArrayAndroid.GetCount() + 1) )
	{
		INT_PTR Index = nIDEvent - IDI_ANDROID_BET - 1;
		if (Index < 0 || Index >= m_ArrayAndroid.GetCount())
		{
			ASSERT(FALSE);
			KillTimer(nIDEvent);
			return;
		}

		if ( m_ArrayAndroid[Index].lJettonScore > 0 )
		{
			LONGLONG lScoreIndex[] = {1000L,500L,100L,10L,5L,1L};
			BYTE cbViewIndex = m_ArrayAndroid[Index].cbJettonArea;

			//���ӳ���
			for (BYTE i=0;i<CountArray(lScoreIndex);i++)
			{
				if(  lScoreIndex[i] > m_lRobotMaxJetton)
					continue;

				if ( m_ArrayAndroid[Index].lJettonScore >= lScoreIndex[i] )
				{
					m_ArrayAndroid[Index].lJettonScore -= lScoreIndex[i];
					m_lAllJettonScore[cbViewIndex] += lScoreIndex[i];

					tagJettonInfo JettonInfo;
					int iWSize = m_RectArea[cbViewIndex-1].right - m_RectArea[cbViewIndex-1].left - 60;
					int iHSize = m_RectArea[cbViewIndex-1].bottom - m_RectArea[cbViewIndex-1].top - 95;
					JettonInfo.nXPos=rand()%(iWSize);
					JettonInfo.nYPos=rand()%(iHSize);
					JettonInfo.cbJettonIndex = JETTON_COUNT - i - 1;

					//��������
					m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
					//��������
					CGameFrameEngine::GetInstance()->PostMessage(IDM_SOUND,7,7);
					break;
				}
			}
		}
		return;
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if (m_lCurrentJetton!=0L)
	{
		int iTimer = 1;
		//��ע����
		BYTE cbJettonArea=GetJettonArea(Point);

		//�����ע
		SCORE lMaxJettonScore=GetUserMaxJetton(cbJettonArea);

		if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
		{		
			return ;
		}

		//�Ϸ��ж�
		if (lMaxJettonScore < m_lCurrentJetton)
		{
			SetJettonHide(0);
			return ;
		}

		//������Ϣ
		if (cbJettonArea!=0xFF) CGameFrameEngine::GetInstance()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,(LPARAM)(&m_lCurrentJetton));
	}
	UpdateGameView(NULL);

	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
}
//���������ؼ���������Ϣ
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
	CPoint *pPoint = (CPoint*)lParam;

	ScreenToClient(pPoint);
    OnLButtonUp(1,*pPoint);
	return 1;
}
void CGameClientView::SetEnablePlaceJetton(bool bEnablePlaceJetton)
{
	m_bEnablePlaceJetton = bEnablePlaceJetton;
}
void CGameClientView::UpdataJettonButton()
{
	if(m_CurArea==0xFF||m_bEnablePlaceJetton == false)
	{
		return ;
	}
	//�������
	SCORE lCurrentJetton=GetCurrentJetton();
	SCORE lLeaveScore=m_lMeMaxScore;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];
	//�����ע
	SCORE lUserMaxJetton = 0;

	lUserMaxJetton = GetUserMaxJetton(m_CurArea);

	lLeaveScore = min((lLeaveScore),lUserMaxJetton); //�û����·� �����ֱȽ� �������屶 


	lCurrentJetton = m_lLastJetton ;

	//���ù��
	if (lCurrentJetton>lLeaveScore)
	{
		if (lLeaveScore>=1000L) SetCurrentJetton(1000L);
		else if (lLeaveScore>=500L) SetCurrentJetton(500L);
		else if (lLeaveScore>=100L) SetCurrentJetton(100L);
		else if (lLeaveScore>=10L) SetCurrentJetton(10L);
		else if (lLeaveScore>=5L) SetCurrentJetton(5L);
		else if (lLeaveScore>=1L) SetCurrentJetton(1L);
		else SetCurrentJetton(0L);
	}
	else
	{
		SetCurrentJetton(m_lLastJetton);
	}
}
//����ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	return __super::OnMouseMove(nFlags,point);
}
//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentJetton=0L;

	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		SetJettonHide(0);
		UpdateGameView(NULL);
	}
	m_lLastJetton = 0L;

	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::SetJettonHide(int ID)
{
	CRgnButton *btJetton[]={NULL,&m_btJetton100,&m_btJetton1000,&m_btJetton10000,&m_btJetton100000,&m_btJetton1000000,&m_btJetton5000000};
	
	if(1<=ID&&ID<=6)
	{
		for (int i = 1;i<=6;i++)
		{
			if(i!=ID)
			btJetton[i]->ShowWindow(SW_SHOW);
		}
		btJetton[ID]->ShowWindow(SW_HIDE);

		//OutputDebugString("����ID\r\n");

	}else
	{
		for (int i = 1;i<=6;i++)
		{
			btJetton[i]->ShowWindow(SW_SHOW);	
		}
	}
}
//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton!=0L)
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//���ñ���
		if ( m_cbAreaFlash != cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			UpdateGameView(NULL);
		}

		bool bButtonArea = false;
		CRgnButton* pRgnButton[] = { &m_btJetton100, &m_btJetton1000, &m_btJetton10000, &m_btJetton100000, &m_btJetton1000000, &m_btJetton5000000};
		for (int i = 0 ; i < CountArray(pRgnButton) && cbJettonArea==0xFF; ++i)
		{
			CRgn rgn;
			CRect rect;
			rgn.CreateRectRgn(0,0,0,0);
			pRgnButton[i]->GetWindowRgn(rgn);
			pRgnButton[i]->GetWindowRect(rect);
			ScreenToClient(&rect);
			rgn.OffsetRgn(CPoint(rect.left,rect.top));
			if (rgn.PtInRegion(MousePoint))
			{
				rgn.DeleteObject();
				bButtonArea = true;
				break;
			}
			rgn.DeleteObject();
		}

		//�����ж�
		if ( cbJettonArea==0xFF && !bButtonArea)
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			SetJettonHide(0);
			return TRUE;
		}

		//�����ע
		LONGLONG lMaxJettonScore=GetUserMaxJetton(cbJettonArea);

		//�Ϸ��ж�
		int iTimer = 1;

		if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore  && !bButtonArea)
		{
			SetJettonHide(0);
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;

		}
		if (lMaxJettonScore< m_lCurrentJetton*iTimer && !bButtonArea)
		{
			SetJettonHide(0);
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}

		//���ù��
		switch (m_lCurrentJetton)
		{
		case 1:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
				return TRUE;
			}
		case 5:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));			
				return TRUE;
			}
		case 10:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)));			
				return TRUE;
			}
		case 100:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
				return TRUE;
			}
		case 500:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));			
				return TRUE;
			}
		case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)));
				return TRUE;
			}
		case 1000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));			
				return TRUE;
			}
		case 5000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)));
				return TRUE;
			}
			UpdateGameView(NULL);
		}
	}
	else
	{
		ClearAreaFlash();
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//�ֻ�ׯ��
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//�ֻ�ׯ��
	if ( bChangeBanker )
	{
		SetTimer( IDI_SHOW_CHANGE_BANKER, 3000, NULL );
		m_bShowChangeBanker = true;
	}
	else
	{
		KillTimer( IDI_SHOW_CHANGE_BANKER );
		m_bShowChangeBanker = false ;
	}

	//���½���
	UpdateGameView(NULL);
}

//��ׯ��ť
void CGameClientView::OnApplyBanker()
{
	CGameFrameEngine::GetInstance()->SendMessage(IDM_APPLY_BANKER,1,0);

	m_bApplyBanker = true;
}

void CGameClientView::OnCancelMyApply()
{
	//AfxMessageBox(TEXT("���Ѿ���ׯ..."));

}

//��ׯ��ť
void CGameClientView::OnCancelBanker()
{
	CGameFrameEngine::GetInstance()->SendMessage(IDM_APPLY_BANKER,0,0);
	m_bApplyBanker = false;

}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}
//ׯ����Ϣ
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, SCORE lBankerScore) 
{
	//ׯ�����Ӻ�
	WORD wBankerUser=INVALID_CHAIR;

	//�������Ӻ�
	if (0!=dwBankerUserID)
	{
		for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
		{
			IClientUserItem  *pUserData=GetClientUserItem(wChairID);
			if (NULL!=pUserData && dwBankerUserID==pUserData->GetUserID())
			{
				wBankerUser=wChairID;
				break;
			}
		}
	}

	//�л��ж�
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinScore=0L;	
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
}
void CGameClientView::DrawMoveInfo(CDC* pDC,CRect rcRect)
{
	return ;
}
//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	//�ǿ��ж�
	if (m_nRecordLast==m_nRecordFirst) return;
	int nIndex = m_nScoreHead;
	COLORREF static clrOld ;
	clrOld = pDC->SetTextColor(RGB(52,116,23));
	int nDrawCount=0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_FALG_COUNT )
	{
		//ʤ����ʶ
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];



		for (int i=1; i<=1; ++i)
		{
			//λ�ñ���
			int static nYPos=40,nXPos=0;
			nYPos=m_nWinFlagsExcursionY+i*27-i*1-33 ;
			nXPos=m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * (43) + 117;

			//ʤ����ʶ
			int static nFlagsIndex=0;

			for (int i = 1;i<=AREA_COUNT;i++)
			{

				if(ClientGameRecord.enOperateMen[i]==enOperateResult_Win)
				{
					nFlagsIndex = (i-1)*2;
					if(i==5)
					{
						nFlagsIndex = 1;

					}
					if(i==6)
					{
						nFlagsIndex = 3;

					}
					if(i==7)
					{
						nFlagsIndex = 5;

					}
					if(i==8)
					{
						nFlagsIndex = 7;

					}
					if(nFlagsIndex == 0)
					{
						dbsj++;
						dbsjdt = dbsj/nIndex;
					}
					if(nFlagsIndex == 1)
					{
						xbsj++;
						xbsjdt = xbsj/nIndex;
					}
					if(nFlagsIndex == 2)			{
						dbm++;
						dbmdt = dbm/nIndex;
					}
					if(nFlagsIndex == 3)
					{
						xbm++;
						xbmdt = xbm/nIndex;
					}
					if(nFlagsIndex == 4)
					{
						dbc++;
						dbcdt = dbc/nIndex;
					}
					if(nFlagsIndex == 5)
					{
						xbc++;
						xbcdt = xbc/nIndex;
					}
					if(nFlagsIndex == 6)
					{
						xdz++;
						xdzdt = xdz/nIndex;
					}
					if(nFlagsIndex == 7)
					{
						ddz++;
						ddzdt = ddz/nIndex;
					}

				}

			}

			
		
			if(nFlagsIndex == 0 || nFlagsIndex == 2 || nFlagsIndex == 4 || nFlagsIndex == 6)
			{
				//�滭��ʶ
				m_ImageWinFlags.DrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/8, 
					m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/8 * nFlagsIndex, 0);
			}
			if(nFlagsIndex == 1 || nFlagsIndex == 3 || nFlagsIndex == 5 || nFlagsIndex == 7)
			{
				//�滭��ʶ
				
				m_ImageWinFlags.DrawImage( pDC, nXPos, nYPos - 30, m_ImageWinFlags.GetWidth()/8, 
					m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/8 * nFlagsIndex, 0);


			}

		}

		//�ƶ��±�
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
	pDC->SetTextColor(clrOld);
}
//�ֹ�����
afx_msg void CGameClientView::OnOpenCard()
{
	CGameFrameEngine::GetInstance()->SendMessage(IDM_OPEN_CARD,0,0);

}
//�Զ�����
afx_msg void CGameClientView::OnAutoOpenCard()
{
	CGameFrameEngine::GetInstance()->SendMessage(IDM_AUTO_OPEN_CARD,0,0);

}
//�ƶ���ť
void CGameClientView::OnScoreMoveL()
{
	if ( m_nRecordFirst == m_nScoreHead ) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( m_nScoreHead == m_nRecordFirst ) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);

	//���½���
	UpdateGameView(NULL);
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount == MAX_FALG_COUNT ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == MAX_FALG_COUNT ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//���½���
	UpdateGameView(NULL);
}

//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{

	if (false==m_bShowGameResult) return;

	int	nXPos = nWidth / 2 - 200+20+30;
	int	nYPos = nHeight / 2 - 208;
	m_pngGameEnd.DrawImage( pDC, nXPos+2-10-10, nYPos+90,
							m_pngGameEnd.GetWidth(), m_pngGameEnd.GetHeight(),0, 0);
	pDC->SetTextColor(RGB(255,255,255));


	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40+30;
	rcMeWinScore.top = nYPos+70 + 32+10+25+50;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150+50;
	rcMeReturnScore.top = nYPos+70 + 32+10+25+50;
	rcMeReturnScore.right = rcMeReturnScore.left + 90;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40+30;
	rcBankerWinScore.top =  nYPos+70 + 32+10+35+10+75;
	rcBankerWinScore.right = rcBankerWinScore.left + 90;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%.2lfd"), m_lBankerCurGameScore);
	DrawNumberStringWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//͸���滭
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
	ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
	if(pDC == 0 || lpRect == 0 || clr == CLR_NONE)
	{
		return false;
	}
	//ȫ͸��
	if( abs(fAlpha) <= 0.000001 )
	{
		return true;
	}

	for(LONG l=lpRect->top; l<lpRect->bottom; l++)
	{
		for(LONG k=lpRect->left; k<lpRect->right; k++)
		{
			COLORREF clrBk = pDC->GetPixel(k, l);
			COLORREF clrBlend = RGB(GetRValue(clrBk)*(1-fAlpha)+GetRValue(clr)*fAlpha, 
								GetGValue(clrBk)*(1-fAlpha)+GetGValue(clr)*fAlpha,
								GetBValue(clrBk)*(1-fAlpha)+GetBValue(clr)*fAlpha);
			pDC->SetPixel(k, l, clrBlend);
		}
	}

	return true;
}
void   CGameClientView::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
	if (InArea==0xFF)
	{
		return ;
	}
	ZeroMemory(bcWinArea,bcAreaCount);


	SCORE lMaxSocre = 0;

	for (int i = 0;i<32;i++)
	{
		BYTE bcOutCadDataWin[AREA_COUNT];
		BYTE bcData[1];
		bcData[0]=i+1;
	    m_GameLogic.GetCardType(bcData,1,bcOutCadDataWin);
		for (int j= 0;j<AREA_COUNT;j++)
		{

			if(bcOutCadDataWin[j]>0&&j==InArea-1)
			{
				SCORE Score = 0; 
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
				{
					if(bcOutCadDataWin[nAreaIndex-1]>1)
					{
						 Score += m_lAllJettonScore[nAreaIndex]*(bcOutCadDataWin[nAreaIndex-1]);
					}
				}
				if(Score>=lMaxSocre)
				{
					lMaxSocre = Score;
					CopyMemory(bcWinArea,bcOutCadDataWin,bcAreaCount);

				}
				break;
			}
		}
	}
}
//�����ע
LONGLONG CGameClientView::GetUserMaxJetton(BYTE cbJettonArea)
{
	if (cbJettonArea==0xFF)
		return 0;

	//����ע��
	SCORE lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex];
	//ׯ�ҽ��
	SCORE lBankerScore = 0x7fffffffffffffff;
	if (m_wBankerUser!=INVALID_CHAIR)
		lBankerScore = m_lBankerScore;

	BYTE bcWinArea[AREA_COUNT];
	SCORE LosScore = 0;
	SCORE WinScore = 0;

	GetAllWinArea(bcWinArea,AREA_COUNT,cbJettonArea);

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if(bcWinArea[nAreaIndex-1]>1)
		{
			LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
		}
		else
		{
			if(bcWinArea[nAreaIndex-1]==0)
			{
				WinScore+=m_lAllJettonScore[nAreaIndex];
			}
		}
	}

	SCORE lTemp = lBankerScore;
	lBankerScore = lBankerScore + WinScore - LosScore;

	if ( lBankerScore < 0 )
	{
		if (m_wBankerUser!=INVALID_CHAIR)
		{
			lBankerScore = m_lBankerScore;
		}
		else
		{
			lBankerScore = 0x7fffffffffffffff;
		}
	}

	//��������
	SCORE lMeMaxScore;

	if((m_lMeMaxScore - lNowJetton)>m_lAreaLimitScore)
	{
		lMeMaxScore= m_lAreaLimitScore;
	}
	else
	{
		lMeMaxScore = m_lMeMaxScore-lNowJetton;
		lMeMaxScore = lMeMaxScore;
	}

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,(lBankerScore)/(bcWinArea[cbJettonArea-1]));

	//��������
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//�ɼ�����
void CGameClientView::SetGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[nAreaIndex] > 0 )
		{
			DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x-5,m_PointJettonNumber[nAreaIndex-1].y+30, false,true);
		}
	}
}

//��ʼ����
void CGameClientView::DispatchCard()
{

	//���ñ�ʶ
	m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard( bool bRecord /*= true*/ )
{
	//����ж�
	if (m_bNeedSetGameRecord==false) return;

	//���ñ�ʶ
	m_bNeedSetGameRecord=false;

	//ɾ����ʱ��
	KillTimer(IDI_DISPATCH_CARD);

	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);

	m_GameLogic.GetCardType(&m_cbTableCardArray[0][0],1,bcResulteOut);

	//�����¼
	if (bRecord)
	{
		SetGameHistory(bcResulteOut);
	}

	//�ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;


	bool blWin[AREA_COUNT];
	for (int i = 0;i<AREA_COUNT;i++){

		if(bcResulteOut[i]>0)
		{
			blWin[i]=true;
		}
		else
		{
			blWin[i]=false;
		}
	}

	//����Ӯ��
	SetWinnerSide(blWin, true);

	//��������
	if (m_lMeCurGameScore>0) 
	{
		CGameFrameEngine::GetInstance()->SendMessage(IDM_SOUND,5,5);

	}
	else if (m_lMeCurGameScore<0) 
	{
		CGameFrameEngine::GetInstance()->SendMessage(IDM_SOUND,4,4);

	}
	else 
	{
		CGameFrameEngine::GetInstance()->SendMessage(IDM_SOUND,4,4);

	}
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//�Ϸ��ж�
	if(m_bFlashResult&&!m_blRungingCar)
	{
		m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());
	}

	if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;

	//��ע�ж�
	if (false==m_bFlashResult)
	{
		if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
		{
			
			//AfxMessageBox(TEXT("111"));
			//pDC->Draw3dRect(m_RectArea[m_cbAreaFlash-1].left,m_RectArea[m_cbAreaFlash-1].top,m_RectArea[m_cbAreaFlash-1].Width()  ,m_RectArea[m_cbAreaFlash-1].Height(),RGB(255,255,0),RGB(255,255,0));
			//s[8];
			m_ImageCh[m_cbAreaFlash-1].DrawImage(pDC,m_RectArea[m_cbAreaFlash-1].left, m_RectArea[m_cbAreaFlash-1].top,m_RectArea[m_cbAreaFlash-1].Width() ,m_RectArea[m_cbAreaFlash-1].Height(),0,0);
		}	
	}
	else
	{
			for (int i = 0;i<AREA_COUNT;i++)
			{
				if(m_bWinFlag[i])
				{
					//m_winflag = true;
					if(i >= 0 && i <= 3)
					{
					m_ImageZuoshang.DrawImage(pDC,nWidth/2 - 500 + 100 + 10,nHeight/2 - 347 + 100 + 40);
					m_ImageYoushang.DrawImage(pDC,nWidth/2 + 97 - 20,nHeight/2 - 347 + 100 + 40 );
					m_ImageZuoxia.DrawImage(pDC,nWidth/2 - 500 + 100+10, nHeight/2 -215 + 100 + 20 + 45);
					m_ImageYouxia.DrawImage(pDC,nWidth/2 + 97 - 20, nHeight/2 -215 + 100 + 20 + 15 - 20 );
					}
				
					//pDC->Draw3dRect(m_RectArea[i].left,m_RectArea[i].top,m_RectArea[i].Width()  ,m_RectArea[i].Height(),RGB(255,255,0),RGB(255,255,0));
					//m_ImageCh[i].DrawImage(pDC,m_RectArea[i].left, m_RectArea[i].top,m_RectArea[i].Width() ,m_RectArea[i].Height(),0,0);
				}
			}
	}
}

//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(bool bWinMen[])
{
	BYTE bcData = m_cbTableCardArray[0][0];
	if(1==bcData||bcData==2||bcData==1+8||bcData==2+8||bcData==1+2*8||bcData==2+2*8||bcData==1+3*8||bcData==2+3*8)
	{
		if(bcData%2==1)
			bWinMen[0]= true;
		else
			bWinMen[1]= true;

	}
	else if(3==bcData||bcData==4||bcData==3+8||bcData==4+8||bcData==3+2*8||bcData==4+2*8||bcData==3+3*8||bcData==4+3*8)
	{
		if(bcData%2==1)
			bWinMen[2]= true;
		else
			bWinMen[3]= true;



	}else if(5==bcData||bcData==6||bcData==5+8||bcData==6+8||bcData==5+2*8||bcData==6+2*8||bcData==5+3*8||bcData==6+3*8)
	{
		if(bcData%2==1)
			bWinMen[4]= true;
		else
			bWinMen[5]= true;

	}else if(7==bcData||bcData==8||bcData==7+8||bcData==8+8||bcData==7+2*8||bcData==8+2*8||bcData==7+3*8||bcData==8+3*8)
	{
		if(bcData%2==1)
			bWinMen[6]= true;
		else
			bWinMen[7]= true;

	}
}

//�ؼ�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��ȡID
	WORD wControlID=LOWORD(wParam);

	//�ؼ��ж�
	switch (wControlID)
	{
	case IDC_JETTON_BUTTON_100:
		{
			//���ñ���
			m_lCurrentJetton=1L;
			break;
		}
	case IDC_JETTON_BUTTON_1000:
		{
			//���ñ���
			m_lCurrentJetton=5L;
			break;
		}
	case IDC_JETTON_BUTTON_10000:
		{
			//���ñ���
			m_lCurrentJetton=10L;
			break;
		}
	case IDC_JETTON_BUTTON_50000:
		{
			//���ñ���
			m_lCurrentJetton=100L;
			break;
		}
	case IDC_JETTON_BUTTON_100000:
		{
			//���ñ���
			m_lCurrentJetton=100L;
			break;
		}
	case IDC_JETTON_BUTTON_500000:
		{
			//���ñ���
			m_lCurrentJetton=500L;
			break;
		}
	case IDC_JETTON_BUTTON_1000000:
		{
			//���ñ���
			m_lCurrentJetton=500L;
			break;
		}
	case IDC_JETTON_BUTTON_5000000:
		{
			//���ñ���
			m_lCurrentJetton=1000L;
			break;
		}
	case IDC_AUTO_OPEN_CARD:
		{
			break;
		}
	case IDC_OPEN_CARD:
		{
			break;
		}
	case IDC_BANK:
		{
			break;
		}
	case IDC_CONTINUE_CARD:
		{
			SwithToNormalView();
			CGameFrameEngine::GetInstance()->SendMessage(IDM_CONTINUE_CARD,0,0);
			break;
		}
	case IDC_RADIO:
		{
			m_CheckImagIndex = 0;

		}

		break;
	case IDC_RADIO+1:
		{
			m_CheckImagIndex = 1;
		}
		break;
	case IDC_RADIO+2:
		{
			m_CheckImagIndex = 2;

		}
		break;
	case IDC_RADIO+3:
		{
			m_CheckImagIndex = 3;

		}
		break;
	case IDC_BANK_DRAW:
		{
			OnBankDraw();
			break;
		}
	case IDC_BANK_STORAGE:
		{
			OnBankStorage();
			break;
		}

	}

	m_lLastJetton = m_lCurrentJetton;

	return CGameFrameView::OnCommand(wParam, lParam);
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
	//�������Ӻ�
	for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
	{
		IClientUserItem  *pUserData=GetClientUserItem(wChairID);
		if (NULL!=pUserData && dwMeUserID==pUserData->GetUserID())
		{
			m_wMeChairID=wChairID;
			break;
		}
	}
}

//������ʾ
void CGameClientView::SetDispatchCardTip(enDispatchCardTip DispatchCardTip)
{
	//���ñ���
	m_enDispatchCardTip=DispatchCardTip;

	//���ö�ʱ��
	if (enDispatchCardTip_NULL!=DispatchCardTip) SetTimer(IDI_SHOWDISPATCH_CARD_TIP,2*1000,NULL);		
	else KillTimer(IDI_SHOWDISPATCH_CARD_TIP);

	//���½���
	UpdateGameView(NULL);
}

//�滭ׯ��
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{
	//ׯ����Ϣ																											
	
	pDC->SetTextColor(RGB(225,225,225));

	//��ȡ���
	IClientUserItem  *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetClientUserItem(m_wBankerUser);

	//λ����Ϣ
	m_LifeWidth;	
	m_TopHeight;

	CRect static StrRect;
	StrRect.left = m_LifeWidth+84+50+3-60;
	StrRect.top = m_TopHeight+84+50-3-96;
	StrRect.right = StrRect.left + 171;
	StrRect.bottom = StrRect.top + 18;

	CString strNick;
	if(pUserData!=NULL)
	{
		strNick= pUserData->GetNickName();
		strNick = strNick.Left(4);
		strNick+=_T("****");
	}
	//ׯ������
	TCHAR Text[256];
	if(m_bEnableSysBanker)
	{

		_sntprintf(Text,256,_TEXT("%s"),pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("ϵͳ��ׯ")):strNick);
		//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
		CDFontEx::DrawText(this,pDC,  14, 400, Text, StrRect, RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	}
	else
	{

		_sntprintf(Text,256,_TEXT("%s"),pUserData==NULL?(m_bEnableSysBanker==false?TEXT("������ׯ"):TEXT("������ׯ")):strNick);
		//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
		CDFontEx::DrawText(this,pDC,  14, 400, Text, StrRect,  RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	}
	//�Ƹ�
//	StrRect.left = m_LifeWidth+79+25;
	StrRect.top = StrRect.bottom+12;
//	StrRect.right = StrRect.left + 171;
	StrRect.bottom = StrRect.top + 21;
	_sntprintf(Text,256,_TEXT("%s"),DrawNumberStringWithDot(pUserData==NULL?0:pUserData->GetUserScore()));
	//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	CDFontEx::DrawText(this,pDC,  14, 400, Text, StrRect,  RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	//�ɼ�
	//StrRect.left = m_LifeWidth+79+10;
	StrRect.top = StrRect.bottom+4;
	//StrRect.right = StrRect.left + 171;
	StrRect.bottom = StrRect.top + 20;
	_sntprintf(Text,256,_TEXT("%s"),DrawNumberStringWithDot(m_lBankerWinScore));
	//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	CDFontEx::DrawText(this,pDC,  14, 400, Text, StrRect, RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	//����
//	StrRect.left = m_LifeWidth+79-5;
	StrRect.top = StrRect.bottom+8;
//	StrRect.right = StrRect.left + 171;
	StrRect.bottom = StrRect.top + 18;
	_sntprintf(Text,256,_TEXT("%d"),m_wBankerTime);
	//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	CDFontEx::DrawText(this,pDC,  14, 400, Text, StrRect, RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	
	
}


void CGameClientView::SetFirstShowCard(BYTE bcCard)
{

	
}
//�滭��ʷ�ɼ�����
void CGameClientView::DrawMemoryDate(CDC *pDC,int nWidth,int nHeight)
{
	CRect static StrRect;
	
	StrRect.left = m_LifeWidth+150+40+125;
	StrRect.top = m_TopHeight+52+627;
	StrRect.right = StrRect.left + 144;
	StrRect.bottom = StrRect.top + 25;
	TCHAR Text[256];

	//��ʱ��
	 
	_sntprintf(Text,256,_TEXT("%s%%"),NumberStringWithSpace(dbsjdt));
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	//С��ʱ��
	StrRect.bottom = StrRect.top;
	StrRect.top = StrRect.bottom - 15;
	_sntprintf(Text,256,_TEXT("%s%%"),NumberStringWithSpace(xbsjdt));
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	//С����
	StrRect.left = StrRect.left + 110;
	StrRect.right = StrRect.left + 144;
	_sntprintf(Text,256,_TEXT("%s%%"),NumberStringWithSpace(xbmdt));
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	//����
	StrRect.top = StrRect.bottom  ;
	StrRect.bottom = StrRect.bottom + 15;
	_sntprintf(Text,256,_TEXT("%s%%"),NumberStringWithSpace(dbmdt));
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	//�󱼳�

	StrRect.left = StrRect.left + 110;
	StrRect.right = StrRect.left + 144;
	_sntprintf(Text,256,_TEXT("%s%%"),NumberStringWithSpace(dbcdt));
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	//С����
	
	StrRect.bottom = StrRect.top;
	StrRect.top = StrRect.bottom - 15;
	_sntprintf(Text,256,_TEXT("%s%%"),NumberStringWithSpace(xbcdt));
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	//С����


	StrRect.left = StrRect.left + 110;
	StrRect.right = StrRect.left + 144;
	_sntprintf(Text,256,_TEXT("%s%%"),NumberStringWithSpace(xdzdt));
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	//�����

	StrRect.top = StrRect.bottom  ;
	StrRect.bottom = StrRect.bottom + 15;
	_sntprintf(Text,256,_TEXT("%s%%"),NumberStringWithSpace(ddzdt));
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);


}
//�滭���
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;

	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,255,0));

	//��ȡ���
	IClientUserItem  *pUserData =  GetClientUserItem(m_wMeChairID);
	CRect static StrRect;
	StrRect.left = m_LifeWidth+78;
	StrRect.top = m_TopHeight+42-52+572;
	StrRect.right = StrRect.left + 144;
	StrRect.bottom = StrRect.top + 25;

	//�������
	TCHAR Text[256];
	_sntprintf(Text,256,_TEXT("%s"),pUserData->GetNickName());
	//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	//���
	StrRect.top = StrRect.bottom;
	StrRect.bottom = StrRect.top + 28;
	//_sntprintf(Text,256,_TEXT("%s"),NumberStringWithSpace(pUserData->GetUserScore()));
	_sntprintf(Text,256,_TEXT("%s"),DrawNumberStringWithDot(pUserData->GetUserScore()));
	//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	
	//�ɼ�
	StrRect.top = StrRect.bottom;
	StrRect.bottom = StrRect.top + 28;
	_sntprintf(Text,256,_TEXT("%s"),NumberStringWithSpace(m_lMeStatisticScore) );
	//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	//�����ע
	StrRect.top = StrRect.bottom;
	StrRect.bottom = StrRect.top + 25;
	SCORE lMeJetton=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
		lMeJetton += m_lUserJettonScore[nAreaIndex];
	_sntprintf(Text,256,_TEXT("%s"),NumberStringWithSpace(pUserData==NULL?0:lMeJetton));
	//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,255), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
}
//////////////////////////////////////////////////////////////////////////
//���д��
void CGameClientView::OnBankStorage()
{
#ifdef __SPECIAL___
// 	//��ȡ�ӿ�
// 	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
// 	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;
// 
// 	if (NULL!=pIClientKernel)
// 	{
// 		CRect btRect;
// 		m_btBankStorage.GetWindowRect(&btRect);
// //		ShowInsureSave(pIClientKernel,CPoint(btRect.right,btRect.top));
// 
// 	}
#endif
}

//����ȡ��
void CGameClientView::OnBankDraw()
{
#ifdef __SPECIAL___
// 	//��ȡ�ӿ�
// 	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
// 	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;
// 
// 	if (NULL!=pIClientKernel)
// 	{
// 		CRect btRect;
// 		m_btBankDraw.GetWindowRect(&btRect);
// //		ShowInsureGet(pIClientKernel,CPoint(btRect.right,btRect.top));
// 
// 	}
#endif
}




void CGameClientView::StartMove()
{
	SetTimer(IDI_MOVECARD_END,250,NULL);
	m_Out_Bao_y = 0;
	m_bShowJettonIndex = 0;
	m_bShowLeaveHandleIndex = 0;
	m_bShowBao = TRUE;
	m_bShowJettonAn = false;
	m_bShowLeaveHandleAn = false;
	m_bOPenBoxAn = false;
	m_bOPenBoxIndex = 0;
	m_blShowLastResult = false;
	m_bShowResult = false;

}
void CGameClientView::StartJetton_AniMationN()
{
	SetTimer(IDI_JETTON_ANIMATION,400,NULL);
	m_bShowJettonAn = true;
	m_bShowJettonIndex = 0;


}

void CGameClientView::StartOPenBox()
{
	m_bOPenBoxAn = true;
	m_bOPenBoxIndex = 0;
	m_bShowBao = false;
	m_Out_Bao_y = 6;
	SetJettonHide(0);
	SetTimer(IDI_OPENBOX_ANIMATION,400,NULL);	

}

void CGameClientView::StartHandle_Leave()
{
	m_bShowLeaveHandleAn = true;
	m_bShowLeaveHandleIndex = 0;
	SetTimer(IDI_HANDLELEAVE_ANIMATION,400,NULL);

	CGameFrameEngine::GetInstance()->SendMessage(IDM_SOUND,0,1);
	

}
void CGameClientView::StarShowResult()
{
	m_bShowResult = true;

}
//�滭ʱ��
void CGameClientView::MyDrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//������Դ
	CPngImage ImageTimeBack;
	CPngImage ImageTimeNumber;
	ImageTimeBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_BACK"));
	ImageTimeNumber.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_NUMBER"));

	//��ȡ����
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//������Ŀ
	SCORE lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=static_cast<INT>(nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth);

	//�滭����
	CSize SizeTimeBack(ImageTimeBack.GetWidth(),ImageTimeBack.GetHeight());
	ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

	//�滭����
	for (SCORE i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//����Ա����
void CGameClientView::OpenAdminWnd()
{
	//��Ȩ��
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;
	//if(m_pClientControlDlg != NULL && ((pIClientKernel->GetUserAttribute()->dwUserRight)&UR_GAME_CONTROL)!=0)
	if(m_pClientControlDlg!=NULL && (CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight)))
	{
		if(!m_pClientControlDlg->IsWindowVisible()) 
			m_pClientControlDlg->ShowWindow(SW_SHOW);
		else 
			m_pClientControlDlg->ShowWindow(SW_HIDE);
	}
}

//ִ��ʣ�����еĻ��嶯��
void CGameClientView::PerformAllBetAnimation()
{
	KillTimer(IDI_ANDROID_BET);
	for ( int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
	{
		KillTimer(IDI_ANDROID_BET + i + 1);
		PlaceUserJetton(m_ArrayAndroid[i].cbJettonArea, m_ArrayAndroid[i].lJettonScore);
	}
	m_ArrayAndroid.RemoveAll();
}


void CGameClientView::UpdateGameView( CRect *pRect )
{
	if(pRect == NULL){
		CRect rect;
		GetClientRect(&rect);
		InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());
		return;
	}
	InvalidGameView(pRect->left,pRect->top,pRect->Width(),pRect->Height());
}