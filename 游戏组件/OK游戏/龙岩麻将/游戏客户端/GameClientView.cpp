#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_TRUSTEE_CONTROL				104								//�йܿ���
#define IDC_OPEN_GOLD					114								//����ť

//������ʶ
#define IDI_BOMB_EFFECT					101								//������ʶ
#define IDI_DISC_EFFECT					102								//����Ч��
#define IDI_MOVE_CARD					103								//

//������Ŀ
#define BOMB_EFFECT_COUNT				12								//������Ŀ
#define DISC_EFFECT_COUNT				8								//����Ч��		

#define MOVE_STEP_COUNT					4								//�ƶ�����
#define TIME_MOVE_CARD					5								//�ƶ�����ʱʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OPEN_GOLD,OnOpenGold)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL,OnStusteeControl)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbFengQuan = GAME_PLAYER;

	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//���ƶ�������
	m_bEnableAnimate = true;
	m_bCardMoving = false;

	//////����λͼ
	////HINSTANCE hInstance=AfxGetInstanceHandle();
	////m_ImageWait.LoadImage( hInstance,TEXT("WAIT_TIP") );
	////m_ImageBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	////m_ImageUserFlag.LoadImage( hInstance,TEXT("USER_FLAG") );
	////m_ImageOutCard.LoadImage( hInstance,TEXT("OUT_CARD_TIP") );
	////m_ImageActionBack.LoadImage( hInstance,TEXT("ACTION_BACK") );
	////m_ImageHuangZhuang.LoadFromResource(hInstance,IDB_HUANG_ZHUANG);
	////m_ImageTrustee.LoadImage(hInstance,TEXT("TRUSTEE"));
	////m_ImageActionAni.LoadImage(AfxGetInstanceHandle(),TEXT("ActionAni"));
	////m_ImageDisc.LoadImage(AfxGetInstanceHandle(),TEXT("DISC"));
	////m_ImageArrow.LoadImage(AfxGetInstanceHandle(),TEXT("ARROW"));
	////m_ImageCenter.LoadFromResource(hInstance,IDB_VIEW_CENTER);
	////m_PngFengQuan.LoadImage( hInstance,TEXT("DRAW_WIND") );
	////m_PngListenFlag.LoadImage( hInstance,TEXT("LISTEN_FLAG") );

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//������Դ///////////////
	g_CardResource.LoadResource();

	//����λͼ//////////
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageWait.LoadImage( hInstance,TEXT("WAIT_TIP") );
	m_ImageBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageUserFlag.LoadImage( hInstance,TEXT("USER_FLAG") );
	m_ImageOutCard.LoadImage( hInstance,TEXT("OUT_CARD_TIP") );
	m_ImageActionBack.LoadImage( hInstance,TEXT("ACTION_BACK") );
	m_ImageHuangZhuang.LoadFromResource(hInstance,IDB_HUANG_ZHUANG);
	m_ImageTrustee.LoadImage(hInstance,TEXT("TRUSTEE"));
	m_ImageActionAni.LoadImage(AfxGetInstanceHandle(),TEXT("ActionAni"));
	m_ImageDisc.LoadImage(AfxGetInstanceHandle(),TEXT("DISC"));
	m_ImageArrow.LoadImage(AfxGetInstanceHandle(),TEXT("ARROW"));
	m_ImageCenter.LoadFromResource(hInstance,IDB_VIEW_CENTER);
	m_PngFengQuan.LoadImage( hInstance,TEXT("DRAW_WIND") );
	m_PngListenFlag.LoadImage( hInstance,TEXT("LISTEN_FLAG") );
    m_ImageGoldCard.LoadImage( hInstance,TEXT("CARD_TABLE_BOTTOM") );///

	//��������
	enDirection Direction[]={Direction_North,Direction_West,Direction_South,Direction_East};

	//���ÿؼ�
	for (WORD i=0;i<4;i++)
	{
		//�û��˿�
		m_HeapCard[i].SetDirection(Direction[i]);
		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����˿�
		for( BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++ )
		{
			m_WeaveCard[i][j].SetDisplayItem(true);
			m_WeaveCard[i][j].SetDirection(Direction[i]);
		}
	}

	//���ÿؼ�
	m_UserCard[0].SetDirection(Direction_North);
	m_UserCard[1].SetDirection(Direction_West);
	m_UserCard[2].SetDirection(Direction_East);

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);
	m_DrawSiceWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,11,NULL);
	////m_DrawSiceWnd.SetWindowSink(AfxGetMainWnd());
	m_DrawSiceWnd.SetWindowSink(this);
	m_GameScore.Create( IDD_GAME_SCORE,this );

	//�����ؼ�
	m_btStart.Create(NULL,WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false,false);
	//by hxh
	m_btOpenGold.Create(NULL,WS_CHILD,rcCreate,this,IDC_OPEN_GOLD);
    m_btOpenGold.SetButtonImage(IDB_OPENGOLD,AfxGetInstanceHandle(),false,false);

	//�йܰ�ť
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);
	m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbFengQuan = GAME_PLAYER;

	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//���ƶ�������
	m_bEnableAnimate = true;
	m_bCardMoving = false;

	//��������
	m_btStart.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameScore.RestorationData();
	
	//���ÿؼ�
	m_btStusteeControl.EnableWindow(FALSE);

	//�˿�����
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[1].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);
	m_HandCardControl.SetCardData(NULL,0,0);
    //by hxh
	m_GoldCardControl.SetPositively(false);
	m_GoldCardControl.SetDisplayItem(false);
	m_GoldCardControl.SetCardData(NULL,0,0/*,NULL,0*/);	

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HeapCard[i].SetHeapCardInfo(0,0);
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);
		for( BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++ )
			m_WeaveCard[i][j].SetCardData(NULL,0);
	}

	//���ٶ�ʱ��
	KillTimer( IDI_MOVE_CARD );
	KillTimer(IDI_DISC_EFFECT);
	KillTimer(IDI_BOMB_EFFECT);
	m_DrawSiceWnd.StopSicing(false);

	return;
}

//�����ؼ�
void CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	int m_nXFace = 50,m_nYFace = 50;//////////////
	int m_nXTimer = 60,m_nYTimer = 60;
	int m_nXBorder = 0,m_nYBorder = 0;

	//��������
	m_ptAvatar[0].x=nWidth/2-m_nXFace;
	m_ptAvatar[0].y=5+m_nYBorder;
	m_ptNickName[0].x=nWidth/2+5;
	m_ptNickName[0].y=20+m_nYBorder;
	m_ptClock[0].x=nWidth/2-m_nXFace-m_nXTimer-2;
	m_ptClock[0].y=17+m_nYBorder;
	m_ptReady[0].x=nWidth/2-m_nXFace-110;
	m_ptReady[0].y=m_nYBorder+20;
	m_UserFlagPos[0].x=nWidth/2-m_nXFace-32;
	m_UserFlagPos[0].y=m_nYBorder+8;
	m_PointTrustee[0].x=nWidth/2-m_nXFace-69;
	m_PointTrustee[0].y=5+m_nYBorder;
	m_UserListenPos[0].x=nWidth/2-m_nXFace+5;
	m_UserListenPos[0].y=m_nYBorder+m_nYFace+20;
	////SetFlowerControlInfo( 0,m_ptAvatar[0].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptAvatar[0].y+m_nYFace );

	m_ptAvatar[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptAvatar[1].y=nHeight/2-m_nYFace-50;
	m_ptNickName[1].x=nWidth-m_nXBorder-5;
	m_ptNickName[1].y=nHeight/2-45;
	m_ptClock[1].x=nWidth-m_nXBorder-m_nXFace+5;
	m_ptClock[1].y=nHeight/2-m_nYFace-27-m_nYTimer;
	m_ptReady[1].x=nWidth-m_nXBorder-32;
	m_ptReady[1].y=nHeight/2-15-m_nYTimer/2+m_nYFace;
	m_UserFlagPos[1].x=nWidth-m_nXBorder-30;
	m_UserFlagPos[1].y=nHeight/2-m_nYFace-78;
	m_PointTrustee[1].x=nWidth-m_nXBorder-38;
	m_PointTrustee[1].y=nHeight/2-m_nYFace-118;
	m_UserListenPos[1].x=nWidth-m_nXBorder-m_nXFace-85;
	m_UserListenPos[1].y=nHeight/2-m_nYFace-50;
	////SetFlowerControlInfo( 1,m_ptAvatar[1].x-BIG_FACE_WIDTH,m_ptAvatar[1].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );

	m_ptAvatar[2].x=nWidth/2-m_nXFace;
	m_ptAvatar[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptNickName[2].x=nWidth/2+5;
	m_ptNickName[2].y=nHeight-m_nYBorder-m_nYFace+8;
	m_ptClock[2].x=nWidth/2-m_nXFace/2-m_nXTimer-2;
	m_ptClock[2].y=nHeight-m_nYBorder-m_nYTimer-8+40;
	m_ptReady[2].x=nWidth/2-m_nXFace-110;
	m_ptReady[2].y=nHeight-m_nYBorder-20;
	m_UserFlagPos[2].x=nWidth/2-m_nXFace-32;
	m_UserFlagPos[2].y=nHeight-m_nYBorder-30;
	m_PointTrustee[2].x=nWidth/2-m_nXFace-69;
	m_PointTrustee[2].y=nHeight-m_nYBorder-35;
	m_UserListenPos[2].x=nWidth/2-m_nXFace;
	m_UserListenPos[2].y=nHeight-m_nYBorder-140;
	////SetFlowerControlInfo( 2,m_ptAvatar[2].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptAvatar[2].y-BIG_FACE_HEIGHT );

	m_ptAvatar[3].x=m_nXBorder+5;
	m_ptAvatar[3].y=nHeight/2-m_nYFace-50;
	m_ptNickName[3].x=m_nXBorder+5;
	m_ptNickName[3].y=nHeight/2-45;
	m_ptClock[3].x=m_nXBorder+22;
	m_ptClock[3].y=nHeight/2-m_nYFace-27-m_nYTimer;
	m_ptReady[3].x=m_nXBorder+32;
	m_ptReady[3].y=nHeight/2-15-m_nYTimer/2+m_nYFace;
	m_UserFlagPos[3].x=m_nXBorder+10;
	m_UserFlagPos[3].y=nHeight/2-m_nYFace-78;
	m_PointTrustee[3].x=m_nXBorder+5;
	m_PointTrustee[3].y=nHeight/2-m_nYFace-118;
	m_UserListenPos[3].x=m_nXBorder+m_nXFace+15;
	m_UserListenPos[3].y=nHeight/2-m_nYFace-50;
	////SetFlowerControlInfo( 3,m_ptAvatar[3].x+m_nXFace,m_ptAvatar[3].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );

	//�û��˿�
	m_UserCard[0].SetControlPoint(nWidth/2-240,m_nYBorder+m_nYFace+10);
	m_UserCard[1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-47,nHeight/2-245);
	m_UserCard[2].SetControlPoint(m_nXBorder+m_nXFace+28,nHeight/2+200);
	m_HandCardControl.SetBenchmarkPos(nWidth/2+10,nHeight-m_nYFace-m_nYBorder-9,enXCenter,enYBottom);
	m_GoldCardControl.SetBenchmarkPos(nWidth/2+80,nHeight/2-27,enXRight,enYBottom);///

	//�����˿�
	m_TableCard[0].SetControlPoint(nWidth/2+200,m_nYBorder+m_nYFace+10);
	m_TableCard[1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-50,nHeight/2+130);
	m_TableCard[2].SetControlPoint(nWidth/2-200,nHeight-m_nYFace-m_nYBorder-63);
	m_TableCard[3].SetControlPoint(m_nXBorder+m_nXFace+10,nHeight/2-165);

	//�ѻ��˿�
	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2-40;
	m_HeapCard[0].SetControlPoint(nXCenter-289,nYCenter-220);
	m_HeapCard[1].SetControlPoint(nXCenter+252,nYCenter-220);
	m_HeapCard[2].SetControlPoint(nXCenter+290,nYCenter+222);
	m_HeapCard[3].SetControlPoint(nXCenter-289,nYCenter+275);

	//�����˿�
	m_DiscardCard[0].SetControlPoint(nXCenter+197,nYCenter-160);
	m_DiscardCard[1].SetControlPoint(nXCenter+208,nYCenter+170);
	m_DiscardCard[2].SetControlPoint(nXCenter-195,nYCenter+170);
	m_DiscardCard[3].SetControlPoint(nXCenter-238,nYCenter-115);

	//����˿�
	int nXControl = nWidth/2+255;
	int nYControl = m_nYBorder+m_nYFace+10;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[0]); i++ )
	{
		m_WeaveCard[0][i].SetControlPoint(nXControl,nYControl);
		nXControl -= 105;
	}

	//����˿�
	nXControl = nWidth-m_nXBorder-m_nXFace-49;
	nYControl = nHeight/2+215;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[1]); i++ )
	{
		m_WeaveCard[1][i].SetControlPoint(nXControl,nYControl);
		nYControl -= 95;
	}

	//����˿�
	nXControl = nWidth/2-295;
	nYControl = nHeight-m_nYFace-m_nYBorder-63;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[2]); i++ )
	{
		m_WeaveCard[2][i].SetControlPoint(nXControl,nYControl);
		nXControl += 110;
	}

	//����˿�
	nXControl = m_nXBorder+m_nXFace+10;
	nYControl = nHeight/2-260;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[3]); i++ )
	{
		m_WeaveCard[3][i].SetControlPoint(nXControl,nYControl);
		nYControl += 95;
	}

	//ɫ�Ӵ���
	CRect rcSice;
	m_DrawSiceWnd.GetWindowRect(&rcSice);
	m_DrawSiceWnd.SetBenchmarkPos((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2);
	m_DrawSiceWnd.MoveWindow((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2,rcSice.Width(),rcSice.Height());

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//�ƶ�����
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-m_nYBorder-175,0,0,uFlags);
	//�ƶ�����
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-rcButton.Height(),0,0,uFlags);

	//���ƴ���
	CRect rcControlWnd;
	m_ControlWnd.GetWindowRect(&rcControlWnd);
	DeferWindowPos(hDwp,m_ControlWnd,NULL,nWidth-rcControlWnd.Width()-30,nHeight-rcControlWnd.Height()-132,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	CRect rcGameScore;
	m_GameScore.GetWindowRect(&rcGameScore);
	CPoint ptPos( (nWidth-rcGameScore.Width())/2,(nHeight-rcGameScore.Height())*2/5-30 );
	ClientToScreen( &ptPos );
	m_GameScore.SetWindowPos( NULL,ptPos.x,ptPos.y,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOCOPYBITS );

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	
	int m_nXBorder = 0,m_nYBorder = 0;/////////


	//�滭����
	DrawViewImage(pDC,m_ImageBack,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageCenter,DRAW_MODE_CENTENT);

	//�滭�û�
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		IClientUserItem * pUserData=GetClientUserItem(i);
		//test
		//tagUserData *pUserData = new tagUserData;
		//lstrcpy(pUserData->GetNickName(),TEXT("SSSSSssssssssssss"));
		//pUserData->cbUserStatus = US_READY;
		//end test
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign((i==1)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			DrawTextString(pDC,pUserData->GetNickName(),RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y);

			//������Ϣ
			WORD wUserTimer=GetUserClock(i);
			//test
			//wUserTimer = 30;
			//end test
			if ((wUserTimer!=0)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				DrawUserTimerEx(pDC,nWidth/2,nHeight/2,wUserTimer);
				if(m_wCurrentUser==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				else if(m_wCurrentUser==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				else if(m_wCurrentUser==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				else if(m_wCurrentUser==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
			}
			if((wUserTimer!=0)&&(m_wCurrentUser==INVALID_CHAIR))
			{
				DrawUserTimerEx(pDC,nWidth/2,nHeight/2,wUserTimer);
				if(i==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				else if(i==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				else if(i==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				else if(i==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);

			}

			if (pUserData->GetUserStatus()==US_READY) 
				DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserData);

			//�й�
			if(m_bTrustee[i])
			{
				m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
			}
		}
		//test
		//delete pUserData;
		//end test
	}

	//Ȧ���־
	if( m_cbFengQuan < GAME_PLAYER )
	{
		m_PngFengQuan.DrawImage( pDC,m_nXBorder+10,m_nYBorder+10,m_PngFengQuan.GetWidth(),m_PngFengQuan.GetHeight()/4,
			0,m_cbFengQuan*m_PngFengQuan.GetHeight()/4 );
	}

	//���Ʊ���	
	if(/*m_bGoldCardShow*/0)
	{
		int n=CCardListImage::GetImageIndex(0x2);
		int w=m_ImageGoldCard.GetWidth()/(MAX_INDEX+1);
		m_ImageGoldCard.DrawImage/*AlphaDrawImage*/(pDC,10,10,w,m_ImageGoldCard.GetHeight(),n*w,0/*,RGB(255,0,255)*/);
	}

	//�û���־
	//test
	//m_wBankerUser = 0;
	//end test
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//����λͼ
		int nImageWidth=m_ImageUserFlag.GetWidth()/4;
		int nImageHeight=m_ImageUserFlag.GetHeight();

		//�滭��־
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wIndex=(m_wBankerUser+GAME_PLAYER-i)%GAME_PLAYER;
			m_ImageUserFlag.DrawImage(pDC,m_UserFlagPos[wIndex].x,m_UserFlagPos[wIndex].y,nImageWidth,nImageHeight,nImageWidth*i,0);
		}
	}

	//�����˿�
	m_DiscardCard[0].DrawCardControl( pDC );
	m_DiscardCard[1].DrawCardControl( pDC );
	m_DiscardCard[2].DrawCardControl( pDC );
	m_DiscardCard[3].DrawCardControl( pDC );

	//�û��˿�
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[1].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);
	
	//�ѻ��˿�
	m_HeapCard[0].DrawCardControl(pDC);
	m_HeapCard[1].DrawCardControl(pDC);
	m_HeapCard[3].DrawCardControl(pDC);
	m_HeapCard[2].DrawCardControl(pDC);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_TableCard[i].DrawCardControl(pDC);
		for( BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++ )
			m_WeaveCard[i][j].DrawCardControl(pDC,true);
	}

	//�����˿�
	m_HandCardControl.DrawCardControl(pDC);
	//by hxh
	m_GoldCardControl.DrawCardControl(pDC);

	//������ʾ
	if (m_bOutCard==true)
	{
		m_ImageOutCard.DrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-165);
	}

	//�ȴ���ʾ
	if (m_bWaitOther==true)
	{
		m_ImageWait.DrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-165);
	}

	//��ׯ��־
	if (m_bHuangZhuang==true)
	{
		////CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.TransDrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2-103,RGB(255,0,255));
	}

	//���Ʊ�־
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bListenStatus[i]==true)
		{
			m_PngListenFlag.DrawImage( pDC,m_UserListenPos[i].x,m_UserListenPos[i].y );
		}
	}

	//�û�״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_wOutCardUser==i)||(m_cbUserAction[i]!=0))
		{
			//����λ��
			int nXPos=0,nYPos=0;
			switch (i)
			{
			case 0:	//����
				{
					nXPos=nWidth/2-32;
					nYPos=m_nYBorder+95;
					break;
				}
			case 1:	//����
				{
					nXPos=nWidth-m_nXBorder-170;
					nYPos=nHeight/2-71;
					break;
				}
			case 2:	//����
				{
					nXPos=nWidth/2-32;
					nYPos=nHeight-m_nYBorder-220;
					break;
				}
			case 3:	//����
				{
					nXPos=m_nXBorder+115;
					nYPos=nHeight/2-71;
					break;
				}
			}

			//�滭����
			if (m_cbUserAction[i]!=WIK_NULL)
			{	
				//�滭����
				if (m_bBombEffect==true)
				{
					//�滭Ч��
					INT nItemHeight=m_ImageActionAni.GetHeight()/7;
					INT nItemWidth=m_ImageActionAni.GetWidth()/BOMB_EFFECT_COUNT;

					//�滭����
					int nYImagePos=0;
					if (m_cbUserAction[i]&WIK_PENG) nYImagePos=nItemHeight;
					else if (m_cbUserAction[i]&WIK_GANG) nYImagePos=nItemHeight*2;
					else if( m_cbUserAction[i]&WIK_LISTEN ) nYImagePos = nItemHeight*3;
					else if (m_cbUserAction[i]&WIK_CHI_HU) nYImagePos=nItemHeight*4;
					else if( m_cbUserAction[i]&WIK_REPLACE ) nYImagePos = nItemHeight*6;
					else nYImagePos=0;
					m_ImageActionAni.DrawImage(pDC,nXPos-nItemWidth/2+54,nYPos+42-nItemHeight/2,nItemWidth,nItemHeight,
						nItemWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),nYImagePos,nItemWidth,nItemHeight);
				}
			}
			else
			{	
				//��������
				m_ImageActionBack.DrawImage(pDC,nXPos,nYPos);

				//�滭�˿�
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos+15,nYPos+13,false);
			}

		}
	}

	//����Ч��
	if(m_wDiscUser!=INVALID_CHAIR)
	{
		CSize SizeDisc(m_ImageDisc.GetWidth()/DISC_EFFECT_COUNT,m_ImageDisc.GetHeight());
		CPoint pt=m_DiscardCard[m_wDiscUser].GetLastCardPosition();
		pt.Offset(-SizeDisc.cx/2,-SizeDisc.cy);
		//�滭��Ϣ
		m_ImageDisc.DrawImage(pDC,pt.x,pt.y,SizeDisc.cx,SizeDisc.cy,
			m_cbDiscFrameIndex*SizeDisc.cx,0,SizeDisc.cx,SizeDisc.cy);
	}

	//���ƻ��ƶ���
	DrawMoveCardItem(pDC);

	return;
}

//�滭�˿˶���
void CGameClientView::DrawMoveCardItem( CDC *pDC )
{
	if( m_bCardMoving )
	{
		int nXDraw = m_MoveCardItem.ptFrom.x, nYDraw = m_MoveCardItem.ptFrom.y;
		switch( m_enMoveDirection )
		{
		case Direction_East:
		case Direction_West:
			{
				//����
				if( m_MoveCardItem.cbCardData != 0 )
				{
					if( m_enMoveDirection == Direction_East )
						g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw,false );
					else g_CardResource.m_ImageTableRight.DrawCardItem( pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw,false );
				}
				//����
				else
				{
					if( m_MoveCardItem.cbCardCount == 1 )
					{
						g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,nXDraw,nYDraw );
					}
					else
					{
						ASSERT( m_MoveCardItem.cbCardCount == 4 );
						g_CardResource.m_ImageHeapDoubleV.DrawImage( pDC,nXDraw,nYDraw );
						nYDraw += Y_HEAP_DOUBLE_V_EXCUSION;
						g_CardResource.m_ImageHeapDoubleV.DrawImage( pDC,nXDraw,nYDraw );
					}
				}
			}
			break;
		case Direction_South:
		case Direction_North:
			{
				//����
				if( m_MoveCardItem.cbCardData != 0 )
				{
					if( m_enMoveDirection == Direction_South )
						g_CardResource.m_ImageTableBottom.DrawCardItem( pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw,false );
					else g_CardResource.m_ImageTableTop.DrawCardItem( pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw,false );
				}
				//����
				else
				{
					if( m_MoveCardItem.cbCardCount == 1 )
					{
						g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXDraw,nYDraw );
					}
					else
					{
						ASSERT( m_MoveCardItem.cbCardCount == 4 );
						g_CardResource.m_ImageHeapDoubleH.DrawImage( pDC,nXDraw,nYDraw );
						nXDraw += g_CardResource.m_ImageHeapDoubleH.GetWidth();
						g_CardResource.m_ImageHeapDoubleH.DrawImage( pDC,nXDraw,nYDraw );
					}
				}
			}
			break;
		default:
			ASSERT(FALSE);
		}
	}
}

//��������
void CGameClientView::SetCellScore(LONGLONG lCellScore)
{
	//�����˿�
	if (lCellScore!=m_lCellScore)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return;
}

//�����˿�
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
	//�����˿�
	if (bHuangZhuang!=m_bHuangZhuang)
	{
		//���ñ���
		m_bHuangZhuang=bHuangZhuang;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return;
}

//ׯ���û�
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	//�����û�
	if (wBankerUser!=m_wBankerUser)
	{
		//���ñ���
		m_wBankerUser=wBankerUser;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return;
}

//״̬��־
void CGameClientView::SetStatusFlag(bool bOutCard, bool bWaitOther)
{
	//���ñ���
	m_bOutCard=bOutCard;
	m_bWaitOther=bWaitOther;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//������Ϣ
void CGameClientView::SetOutCardInfo(WORD wViewChairID, BYTE cbCardData)
{
	//���ñ���
	m_cbCardData=cbCardData;
	m_wOutCardUser=wViewChairID;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//������Ϣ
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER)
	{
		m_cbUserAction[wViewChairID]=bUserAction;
		SetBombEffect(true);
	}
	else 
	{
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		if(m_bBombEffect)
			SetBombEffect(false);
	}

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//���ö���
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect=true;
		m_cbBombFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT,250,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBombEffect==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;

			//���½���
			InvalidGameView(0,0,0,0);
		}
	}

	return true;
}

//�����û�
void CGameClientView::SetDiscUser(WORD wDiscUser)
{
	if(m_wDiscUser != wDiscUser)
	{
		//���±���
		m_wDiscUser=wDiscUser;

		if( m_wDiscUser != INVALID_CHAIR )
			SetTimer( IDI_DISC_EFFECT,250,NULL );
		else KillTimer( IDI_DISC_EFFECT );

		//���½���
		InvalidGameView(0,0,0,0);
	}
	return;
}

//��ʱ���
void CGameClientView::SetCurrentUser(WORD wCurrentUser)
{
	if (m_wCurrentUser != wCurrentUser)
	{
		//���±��� 
		m_wCurrentUser=wCurrentUser;
		
		//���½���
		InvalidGameView(0,0,0,0);
	}
	return;
}
//�����й�
void CGameClientView::SetTrustee(WORD wTrusteeUser,bool bTrustee)
{
	//У������ 
	ASSERT(wTrusteeUser>=0&&wTrusteeUser<GAME_PLAYER);

	if(m_bTrustee[wTrusteeUser] !=bTrustee)	
	{
		//��������
		m_bTrustee[wTrusteeUser]=bTrustee;

		//���½���
		InvalidGameView(0,0,0,0);
	}
	return;
}

//���Ʊ�־
void CGameClientView::SetUserListenStatus(WORD wViewChairID, bool bListenStatus)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER)
	{
		SetBombEffect(true);
		m_cbUserAction[wViewChairID]=WIK_LISTEN;
		m_bListenStatus[wViewChairID]=bListenStatus;
	}
	else 
		ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

	//���½���
	InvalidGameView(0,0,0,0);

	return;
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
		////pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
		TextOut(pDC,nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	////pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
	TextOut(pDC,nXPos,nYPos,pszString,nStringLength);

	return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�������
	CRect rcRePaint;
	bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,rcRePaint);

	//�ػ�����
	if (rcRePaint.IsRectEmpty()==false)
		////InvalidGameView(&rcRePaint);
		InvalidGameView(rcRePaint.left,rcRePaint.top,rcRePaint.right,rcRePaint.bottom);

	//������
	if (bHandle==false)
		return __super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//�˿��¼�
	m_HandCardControl.OnEventLeftHitCard();

	//test
	//BYTE byCard[] = {
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,					
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09
	//};
	//BYTE byCardCount = 14;
	//static int n = 0;
	//if( ++n==1 )
	//{
	//	m_HandCardControl.SetDisplayItem(true);
	//	m_HandCardControl.SetPositively(true);
	//	m_HandCardControl.SetCardData(byCard,MAX_COUNT-1-3-3,byCard[MAX_COUNT-1]);
	//	for( BYTE i = 0; i < GAME_PLAYER; i++ )
	//	{
	//		m_HeapCard[i].SetHeapCardInfo(3,0);
	//		m_HeapCard[i].SetCardData( 0,0x01 );
	//		m_HeapCard[i].SetCardEmpty( 19,4 );
	//		m_TableCard[i].SetCardData(byCard,byCardCount-3-3-3-3);
	//		m_DiscardCard[i].SetCardData(byCard,27);
	//		m_WeaveCard[i][0].SetCardData(byCard,3);
	//		m_WeaveCard[i][1].SetCardData( byCard,3 );
	//		m_WeaveCard[i][2].SetCardData( byCard,3 );
	//		m_WeaveCard[i][3].SetCardData( byCard,3 );
	//		m_WeaveCard[i][0].SetDirectionCardPos( 0 );
	//		m_WeaveCard[i][1].SetDirectionCardPos( 1 );
	//		m_WeaveCard[i][2].SetDirectionCardPos( 2 );
	//		m_WeaveCard[i][3].SetDirectionCardPos( 3 );
	//		if( i < 3 )
	//			m_UserCard[i].SetCardData(MAX_COUNT-1-3-3,true);
	//		m_bTrustee[i] = true;
	//		m_cbUserAction[i] = WIK_REPLACE;
	//		m_bListenStatus[i] = true;
	//	}
	//	m_bWaitOther = true;
	//	m_bHuangZhuang = true;
	//	m_bOutCard = true;
	//	m_wOutCardUser = 3;
	//	m_cbCardData = 0x01;
	//	m_cbUserAction[3] = WIK_NULL;
	//	SetDiscUser( 1 );
	//	SetBombEffect(true);
	//	tagScoreInfo ScoreInfo;
	//	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	//	tagWeaveInfo WeaveInfo;
	//	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));
	//	ScoreInfo.wChiHuUser = 0;
	//	ScoreInfo.wProvideUser = 0;
	//	ScoreInfo.cbFanCount = 88;
	//	ScoreInfo.cbCardCount = 13;
	//	CopyMemory( ScoreInfo.cbCardData,byCard,sizeof(BYTE)*ScoreInfo.cbCardCount );
	//	for( WORD i = 0; i < GAME_PLAYER; i++ )
	//	{
	//		lstrcpy( ScoreInfo.szUserName[i],TEXT("SSSSDFSDFSDF") );
	//		ScoreInfo.lGameScore[i] = 123L*((i%2)?-1:1);
	//	}
	//	CChiHuRight chr;
	//	chr |= CHR_ZI_MO;
	//	chr |= CHR_SHI_GUI_YI;
	//	chr |= CHR_PING_HU;
	//	chr |= CHR_MEN_QI_QING;
	//	chr |= CHR_QUAN_QIU_REN;
	//	chr |= CHR_HUN_YI_SE;
	//	ScoreInfo.cbHuaCardCount = 3;
	//	ScoreInfo.cbFanCount = 23;
	//	m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo,chr);
	//	m_GameScore.ShowWindow( SW_SHOW );
	//	SetHuangZhuang(true);
	//	m_cbFengQuan = 0;
	//	m_btStart.ShowWindow( SW_SHOW );

	//	tagSelectCardInfo sci[MAX_WEAVE];
	//	ZeroMemory( sci,sizeof(sci) );
	//	sci[0].cbActionCard = 0x03;
	//	sci[0].wActionMask = WIK_LEFT;
	//	sci[0].cbCardCount = 2;
	//	sci[0].cbCardData[0] = 0x04;
	//	sci[0].cbCardData[1] = 0x05;
	//	sci[1].cbActionCard = 0x03;
	//	sci[1].wActionMask = WIK_CENTER;
	//	sci[1].cbCardCount = 2;
	//	sci[1].cbCardData[0] = 0x02;
	//	sci[1].cbCardData[1] = 0x04;
	//	sci[2].cbActionCard = 0x03;
	//	sci[2].wActionMask = WIK_RIGHT;
	//	sci[2].cbCardCount = 2;
	//	sci[2].cbCardData[0] = 0x01;
	//	sci[2].cbCardData[1] = 0x02;
	//	m_HandCardControl.OnEventUserAction( sci,3 );
	//	m_ControlWnd.SetControlInfo( WIK_PENG|WIK_CHI_HU|WIK_LEFT|WIK_LISTEN|WIK_GANG );

	//	//m_DrawSiceWnd.SetSiceInfo( GetDC(),250,MAKEWORD(4,5),MAKEWORD(2,6) );
	//	//m_DrawSiceWnd.ShowWindow( SW_SHOW );
	//}
	//else
	//{
	//	tagMoveCardItem mci;
	//	mci.cbCardCount = 4;
	//	mci.cbCardData = 0;
	//	mci.wViewChairId = 0;
	//	mci.ptFrom = m_HeapCard[0].GetDispatchCardPos( (WORD)6 );
	//	mci.ptTo = CPoint(500,500);
	//	m_HeapCard[0].SetCardEmpty( 6+1,4 );
	//	OnMoveCardItem( mci );
	//}
	//end test

	return;
}

BOOL CGameClientView::PreTranslateMessage(MSG* pMsg)
{ 
	if(pMsg->message == WM_KEYDOWN) 
	{ 
		switch (pMsg->wParam) 
		{ 
		case VK_LEFT: 
			break; 
		case VK_RIGHT: 
			break; 
		case VK_UP: 
			break; 
		case VK_DOWN: 
			break; 
		case VK_F1: 
		case VK_F2:
		case VK_F3:
			{
				//������Ϣ
				/*AfxGetMainWnd()->SendMessage*/SendEngineMessage(IDM_OPEN_GOLD,0,pMsg->wParam-VK_F1);
				return TRUE;
			}
			break; 
		case 'A':
		case 'B': 
		case 'C': 
			{
				//������Ϣ
				/*AfxGetMainWnd()->SendMessage*/SendEngineMessage(IDM_OPEN_GOLD,0,pMsg->wParam-'A');
				return TRUE;
			}
		} 
	}
	return __super::PreTranslateMessage(pMsg); 
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	//������Ϣ
	SendEngineMessage(IDM_START,0,0);

	return;
}

//������Ϣ
void CGameClientView::OnOpenGold()
{
	//������Ϣ
	SendEngineMessage(IDM_OPEN_GOLD,0,2);
	return;
}

//�Ϲܿ���
void CGameClientView::OnStusteeControl()
{
	SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}

//��ʱ��
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//��������
	if (nIDEvent==IDI_BOMB_EFFECT)
	{
		//ֹͣ�ж�
		if (m_bBombEffect==false)
		{
			KillTimer(IDI_BOMB_EFFECT);
			return;
		}

		//���ñ���
		if ((m_cbBombFrameIndex+1)>=BOMB_EFFECT_COUNT)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;
		}
		else m_cbBombFrameIndex++;

		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}
	else if (nIDEvent==IDI_DISC_EFFECT)
	{
		//���ñ���
		if ((m_cbDiscFrameIndex+1)>=DISC_EFFECT_COUNT)
		{
			m_cbDiscFrameIndex=0;
		}
		else m_cbDiscFrameIndex++;

		//���½���
		InvalidGameView(0,0,0,0);

		return;

	}
	else if( nIDEvent == IDI_MOVE_CARD )
	{
		//�жϵ�ǰ�����Ƿ����
		if( m_nStepCount == 0 )
		{
			//ɾ����ʱ��
			KillTimer( IDI_MOVE_CARD );

			//��FALSE
			m_bCardMoving = false;

			//���Ͷ���������Ϣ
			SendEngineMessage( IDM_USER_ACTION_FINISH,0,0);

			//������ͼ
			InvalidGameView( 0,0,0,0 );
		}
		//������û����
		else
		{
			//���ٲ���
			m_nStepCount--;

			//���û滭����
			CSize size(m_MoveCardItem.cbCardCount==4?80:60,m_MoveCardItem.cbCardCount==4?80:60);
			m_rcCardMove = CRect(m_MoveCardItem.ptFrom,size);

			//�ƶ�
			m_MoveCardItem.ptFrom.x += m_nXStep;
			m_MoveCardItem.ptFrom.y += m_nYStep;

			//���ϻ滭����
			size.SetSize( m_MoveCardItem.cbCardCount==4?80:60,m_MoveCardItem.cbCardCount==4?80:60 );
			CRect rcDraw( m_MoveCardItem.ptFrom,size );
			m_rcCardMove.UnionRect( &m_rcCardMove,&rcDraw );

			//������ͼ
			InvalidGameView( m_rcCardMove.left,m_rcCardMove.top,m_rcCardMove.right,m_rcCardMove.bottom );
		}
		return;
	}

	__super::OnTimer(nIDEvent);
}

//���÷�Ȧ
void CGameClientView::SetFengQuan( BYTE cbFengQuan )
{
	if( m_cbFengQuan != cbFengQuan )
	{
		m_cbFengQuan = cbFengQuan;
		InvalidGameView( 0,0,0,0 );
	}
}

//������
void CGameClientView::EnableAnimate( bool bAnimate )
{
	m_bEnableAnimate = bAnimate;

	if( !m_bEnableAnimate && m_bCardMoving )
		StopMoveCard();
}

//��������,�����Ͷ���������Ϣ
bool CGameClientView::StopMoveCard()
{
	//�ж�
	if( !m_bCardMoving ) return false;

	KillTimer( IDI_MOVE_CARD );

	m_bCardMoving = false;

	//������Ϣ
	SendEngineMessage( IDM_USER_ACTION_FINISH,0,0 );

	InvalidGameView( 0,0,0,0 );

	return true;
}

//�˿˶���
void CGameClientView::OnMoveCardItem( const tagMoveCardItem &MoveCardItem )
{
	if( m_bCardMoving ) return;

	//�Ƿ�������
	if( !m_bEnableAnimate )
	{
		//������Ϣ
		SendEngineMessage( IDM_USER_ACTION_FINISH,0,0 );
		return;
	}

	m_bCardMoving = true;
	//���ñ���
	m_MoveCardItem = MoveCardItem;
	m_nStepCount = MOVE_STEP_COUNT;
	m_nXStep = (MoveCardItem.ptTo.x-MoveCardItem.ptFrom.x)/m_nStepCount;
	m_nYStep = (MoveCardItem.ptTo.y-MoveCardItem.ptFrom.y)/m_nStepCount;
	switch( MoveCardItem.wViewChairId )
	{
	case 0:
		m_enMoveDirection = Direction_North;
		break;
	case 1:
		m_enMoveDirection = Direction_West;
		break;
	case 2:
		m_enMoveDirection = Direction_South;
		break;
	case 3:
		m_enMoveDirection = Direction_East;
	}

	//���û滭����
	CSize size(MoveCardItem.cbCardCount==4?80:50,MoveCardItem.cbCardCount==4?80:50);
	CRect rcDraw(MoveCardItem.ptFrom,size);
	m_rcCardMove = rcDraw;

	//���ö�ʱ��
	SetTimer( IDI_MOVE_CARD,TIME_MOVE_CARD,NULL );
	InvalidGameView( m_rcCardMove.left,m_rcCardMove.top,m_rcCardMove.right,m_rcCardMove.bottom );
}

//�滭ʱ��
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//������Դ
	CPngImage ImageTimeBack;
	CPngImage ImageTimeNumber;
	HINSTANCE hInst = AfxGetInstanceHandle();
	ImageTimeBack.LoadImage(hInst,TEXT("TIME_BACK"));
	ImageTimeNumber.LoadImage(hInst,TEXT("TIME_NUMBER"));

	//��ȡ����
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//������Ŀ
	LONG lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	//�滭����
	CSize SizeTimeBack(ImageTimeBack.GetWidth(),ImageTimeBack.GetHeight());
	ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
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

//�滭׼��
void CGameClientView::DrawUserReadyEx(CDC * pDC, int nXPos, int nYPos)
{
	//������Դ
	CPngImage ImageUserReady;
	ImageUserReady.LoadImage(AfxGetInstanceHandle(),TEXT("USER_READY"));

	//�滭׼��
	CSize SizeImage(ImageUserReady.GetWidth(),ImageUserReady.GetHeight());
	ImageUserReady.DrawImage(pDC,nXPos-SizeImage.cx/2,nYPos-SizeImage.cy/2);

	return;
}

//////////////////////////////////////////////////////////////////////////
