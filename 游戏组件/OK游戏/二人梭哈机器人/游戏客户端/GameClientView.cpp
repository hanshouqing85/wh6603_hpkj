#include "StdAfx.h"
#include "Resource.h"
#include "GameClientEngine.h"
#include "GameClientView.h"


//////////////////////////////////////////////////////////////////////////

//��ť��ʶ 
#define IDC_START						100								//��ʼ��ť
#define IDC_GIVE_UP						102								//������ť
#define IDC_FOLLOW						103								//��ע��ť
#define IDC_SHOW_HAND					104								//�����ť
#define IDC_ADD_TIMES1					105								//��ְ�ť
#define IDC_ADD_TIMES2					106								//��ְ�ť
#define IDC_ADD_TIMES3					107								//��ְ�ť
#define IDC_LOCK_GAME					108

//��ʱ����ʶ
#define IDI_SEND_CARD					107								//���ƶ�ʱ��
#define IDI_SHOW_CARD					108								//�Է�����
#define IDI_SHOW_CARD_ME				111								//�Լ�����
#define IDI_USER_SHOW_HAND				109								//
#define IDI_SHOW_CARD_HIDE				110								//ȡ����ʾ

#define SPEED_MOVE_CARD					30								//

#define TIME_USER_SHOW_HAND				3000							//

//���ƶ���
#define SEND_STEP_COUNT					6								//����
#define SPEED_SEND_CARD					10								//�����ٶ�

#define MAX_COUNT						5

#define SHOW_CARD_ANIMATE												//���ÿ��ƶ���,���Ҫȡ������,��ע�͵�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)							  
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_FOLLOW, OnFollow)
	ON_BN_CLICKED(IDC_GIVE_UP, OnGiveUp)
	ON_BN_CLICKED(IDC_ADD_TIMES1, OnAddTimes1)
	ON_BN_CLICKED(IDC_ADD_TIMES2, OnAddTimes2)
	ON_BN_CLICKED(IDC_ADD_TIMES3, OnAddTimes3)
	ON_BN_CLICKED(IDC_LOCK_GAME, OnBnClickedLockGame)
	ON_BN_CLICKED(IDC_SHOW_HAND, OnShowHand)
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//���ñ���
	m_lMaxGold=0L;
	m_lBasicGold=0L;
	m_bShowScoreControl = false;
	m_bUserShowHand = false;
	ZeroMemory( m_lUserGold,sizeof(m_lUserGold) );
	m_bShowCard = false;
	//��������
	m_SendCardPos.SetPoint(0,0);
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = SEND_STEP_COUNT;
	m_nXStep = 0;
	m_nYStep = 0;
	memset(&m_UserWinScore, 0, sizeof(m_UserWinScore));
	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageCardBack.LoadFromResource(hInstance,IDB_CARD);
//	m_ImageTitle.LoadFromResource(this,hInstance,IDB_TITLE);
	m_ImageViewFill.LoadFromResource(hInstance,IDB_VIEW_FILL);
	m_ImageUserReady.LoadFromResource(this,hInstance,IDB_USER_READY);
	m_ImageTimeFrame.LoadFromResource(this,hInstance,IDB_TIME_FRAME);
	m_ImageViewCenter.LoadFromResource(this,hInstance,IDB_BACK_GROUND);
	//m_ImageScoreControlBack.LoadFromResource(this,hInstance, IDB_SCORE_CONTROL_BACK );
	m_ImageScoreTable.LoadFromResource(this,hInstance, IDB_SCORE_TABLE );
	m_PngShowHand.LoadImage( hInstance,TEXT("SHOW_HAND_H") );
	m_ImageTableScore.LoadImage(hInstance,TEXT("TABLE_SCORE"));
	ViewFont1.CreateFont(12,6,0,0,0,0,0,0,134,3,2,1,1,TEXT("����"));
	//wServerID=0;
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

	//�����ؼ�
	CRect CreateRect(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,9);

	//������ť
	m_btStart.Create(NULL,WS_CHILD,CreateRect,this,IDC_START);
	m_btFollow.Create(NULL,WS_CHILD,CreateRect,this,IDC_FOLLOW);
	m_btGiveUp.Create(NULL,WS_CHILD,CreateRect,this,IDC_GIVE_UP);
	m_btShowHand.Create(NULL,WS_CHILD,CreateRect,this,IDC_SHOW_HAND);
	m_btAddTimes1.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_ADD_TIMES1);
	m_btAddTimes2.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_ADD_TIMES2);
	m_btAddTimes3.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_ADD_TIMES3);
	m_btLockGame.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_LOCK_GAME);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false,false);
	m_btFollow.SetButtonImage(IDB_BT_FOLLOW,hInstance,false,false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVE_UP,hInstance,false,false);
	m_btShowHand.SetButtonImage(IDB_BT_SHOW_HAND,hInstance,false,false);
	m_btAddTimes1.SetButtonImage(IDB_BT_ADD_GOLD,hInstance,false,false);
	m_btAddTimes2.SetButtonImage(IDB_BT_ADD_GOLD_2,hInstance,false,false);
	m_btAddTimes3.SetButtonImage(IDB_BT_ADD_GOLD_3,hInstance,false,false);	

	ViewFont.CreateFont(16,8,0,0,0,0,0,0,134,3,2,1,1,TEXT("����"));
	ViewFont1.CreateFont(12,6,0,0,0,0,0,0,134,3,2,1,1,TEXT("����"));

	m_btLockGame.SetButtonImage(IDB_BT_UNLOCK,hInstance,false,false);
	//m_btLockGame.ShowWindow(SW_SHOW);
	m_btLockGame.ShowWindow(SW_HIDE);


	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		m_rcCtrlWinner[i].SetRect(0,0,100,100);        // ������Ӯ
	}

	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);

	m_bSuperUser = false;

	//test
	//SetGoldTitleInfo( 0,123456789 );
	//m_btStart.ShowWindow( SW_SHOW );
	//ShowScoreControl( true );
	//BYTE byCard[] = {
	//	0x01,0x09,0x0a,0x0b,0x0c
	//};
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_CardControl[i].SetCardData( byCard,5 );
	//	SetUserGoldInfo( i,false,1 );
	//	SetUserGoldInfo( i,true,456 );
	//}
	//m_CardControl[2].AllowPositively( true );
	//m_CardControl[2].SetDispalyCard( 4,false );
	//BYTE cbCardData[] = {0x01,0x08,0x09,0x0A,0x0B};
	//m_CardControl[2].SetCardData( cbCardData,2 );
	//m_GoldView[0].SetGold( 123456789L );
	//m_GoldView[1].SetGold( 123456789L );
	//SetGoldTitleInfo( 1234567L,123456789L );
	//m_btFollow.ShowWindow( SW_SHOW );
	//m_btGiveUp.ShowWindow( SW_SHOW );
	//m_btShowHand.ShowWindow( SW_SHOW );
	//m_btAddTimes1.ShowWindow( SW_SHOW );
	//m_btAddTimes2.ShowWindow( SW_SHOW );
	//m_btAddTimes3.ShowWindow( SW_SHOW );
	//ShowScoreControl( true );
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_GoldView[i*2].SetGoldIndex(2);
	//	m_GoldView[i*2].SetGold(12345678L);
	//	m_GoldView[i*2+1].SetGold(654321L);
	//}
	//end test

	return 0;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	SendEngineMessage(IDM_START,0,0);
	return;
}

//��ע��ť
void CGameClientView::OnFollow()
{
	SendEngineMessage(IDM_FOLLOW,1,0);
	return;
}

//������ť
void CGameClientView::OnGiveUp()
{
	SendEngineMessage(IDM_GIVE_UP,0,0);
	return;
}

//��ע��ť
void CGameClientView::OnAddTimes1()
{
	SendEngineMessage(IDM_ADD_GOLD,1,0);
	return;
}

//��ע��ť
void CGameClientView::OnAddTimes2()
{
	SendEngineMessage(IDM_ADD_GOLD,2,0);
	return;
}

//��ע��ť
void CGameClientView::OnAddTimes3()
{
	SendEngineMessage(IDM_ADD_GOLD,4,0);
	return;
}
//��ע��ť
void CGameClientView::OnBnClickedLockGame()
{
	//m_pGameClientDlg->SendSocketData(1024);
	//SendEngineMessage(IDM_ADD_GOLD,3,0);
	return;
}
//�����ť
void CGameClientView::OnShowHand()
{
	SendEngineMessage(IDM_SHOWHAND,0,0);
	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//���ñ���
	m_lMaxGold=0L;
	m_lBasicGold=0L;
	m_bShowScoreControl = false;
	m_bUserShowHand = false;
	ZeroMemory( m_lUserGold,sizeof(m_lUserGold) );

	//��������
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = 0;
	m_nXStep = 0;
	m_nYStep = 0;
	m_SendCardItemArray.RemoveAll();

	//���ÿؼ�
	m_btStart.ShowWindow(SW_HIDE);
	m_btFollow.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btShowHand.ShowWindow(SW_HIDE);
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ó���
	for (int i=0;i<CountArray(m_GoldView);i++) m_GoldView[i].SetGold(0L);

	//�����˿�
	for (int i=0;i<CountArray(m_CardControl);i++)
	{
		m_CardControl[i].SetCardData(NULL,0);
		m_CardControl[i].ShowFirstCard(false);
		m_CardControl[i].AllowPositively(false);
	}
	m_bSuperUser = false;

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	INT m_nXFace=32;
	INT m_nYFace=32;
	INT m_nXTimer=60;
	INT m_nYTimer=60;
	INT m_nXBorder=5;
	INT m_nYBorder=5;

	int nPosY = 33;

	//��������
	m_ptAvatar[0].x=nWidth/2;
	m_ptAvatar[0].y=20+m_nYBorder;
	m_ptNickName[0].x=nWidth/2-70;//+m_nXFace+5;
	m_ptNickName[0].y=5+m_nYBorder+45+4-10;
	m_ptClock[0].x=nWidth/2+100;//-m_nXFace/2-m_nXTimer/2+170;
	m_ptClock[0].y=nHeight/2-130-50;//m_nYBorder+m_nYTimer/2+115;
	m_ptReady[0].x=nWidth/2;//-m_nXFace-15;
	m_ptReady[0].y=nHeight/2-250;//+m_nYBorder+m_nYTimer/2;
	//SetFlowerControlInfo( 0,nWidth/2-BIG_FACE_WIDTH/2,m_nYBorder+5+m_nYFace );

	//m_ptAvatar[1].x=nWidth-m_nXBorder-m_nXFace-128;
	//m_ptAvatar[1].y=nHeight/2-m_nYFace-58+nPosY;
	//m_ptNickName[1].x=nWidth-m_nXBorder-m_nXFace-30;
	//m_ptNickName[1].y=nHeight/2-m_nYFace+64+nPosY;//64;//-58;
	//m_ptClock[1].x=nWidth/2+180;//-m_nXBorder-m_nXTimer/2-7;
	//m_ptClock[1].y=nHeight/2+nPosY;//-m_nYFace+76;//-83;
	//m_ptReady[1].x=nWidth/2+300;//nWidth-(m_nXBorder+48);
	//m_ptReady[1].y=nHeight/2+nPosY;//-m_nYFace-83;
	//SetFlowerControlInfo( 1,nWidth-m_nXBorder-m_nXFace-5-BIG_FACE_WIDTH,nHeight/2-BIG_FACE_HEIGHT/2 );

	
	m_ptAvatar[1].x=nWidth/2;
	m_ptAvatar[1].y=nHeight-m_nYBorder-m_nYFace-128;
	m_ptNickName[1].x=nWidth/2-70;//+m_nYFace+5;
	m_ptNickName[1].y=nHeight-m_nYBorder-22-45+6;//-m_nYFace-5;
	m_ptClock[1].x=nWidth/2-85;//-m_nXFace/2-m_nXTimer/2+170;
	m_ptClock[1].y=nHeight/2+110;//-m_nYBorder-m_nYTimer/2-1;
	m_ptReady[1].x=nWidth/2;// + 10;
	m_ptReady[1].y=nHeight/2+250;//-217-m_nYBorder;
	//SetFlowerControlInfo( 2,nWidth/2-BIG_FACE_WIDTH/2,nHeight-m_nYBorder-m_nYFace-5-BIG_FACE_HEIGHT );

	//m_ptAvatar[3].x=m_nXBorder+5;
	//m_ptAvatar[3].y=nHeight/2-m_nYFace-58+nPosY;
	//m_ptNickName[3].x=m_nXBorder+m_nXFace+10;
	//m_ptNickName[3].y=nHeight/2-m_nYFace+64+nPosY;//-58;
	//m_ptClock[3].x=nWidth/2-180;//m_nXBorder+m_nXTimer/2+111;
	//m_ptClock[3].y=nHeight/2+nPosY;//-m_nYFace+76;//-83+159;
	//m_ptReady[3].x=nWidth/2-300;//m_nXBorder+45;
	//m_ptReady[3].y=nHeight/2+nPosY;//-m_nYFace-83;
	//SetFlowerControlInfo( 3,m_nXBorder+5+m_nXFace,nHeight/2-BIG_FACE_HEIGHT/2 );

	//����ؼ�
	int nXCenterPos=nWidth/2;
	int nYCenterPos=nHeight/2;
	m_GoldStation[0].SetPoint(nXCenterPos,nYCenterPos-66);
	m_GoldStation[1].SetPoint(nXCenterPos+79,nYCenterPos-66);

	//m_GoldStation[2].SetPoint(nXCenterPos+88,nYCenterPos);
	//m_GoldStation[3].SetPoint(nXCenterPos+88,nYCenterPos+63);

	m_GoldStation[2].SetPoint(nXCenterPos,nYCenterPos+66);
	m_GoldStation[3].SetPoint(nXCenterPos-79,nYCenterPos+66);

	//m_GoldStation[6].SetPoint(nXCenterPos-88,nYCenterPos+1);
	//m_GoldStation[7].SetPoint(nXCenterPos-88,nYCenterPos-61);

	//��ע��λ��
	m_ptScoreControl.SetPoint(nWidth/2+200,m_ptAvatar[1].y+5);//nHeight/2+200);
	INT nXControl = m_ptScoreControl.x+6-110+20;
	INT nYControl = m_ptScoreControl.y+7-105+30+2+1;

	//��ť�ؼ�
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	
	//��ʼ��ť
	CRect rcStart;
	m_btStart.GetWindowRect(&rcStart);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcStart.Width())/2+1,nHeight/2+156-53,0,0,uFlags);

	//���ư�ť
	m_btFollow.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btFollow,NULL,nXControl,nYControl,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes3,NULL,nXControl,nYControl+rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes1,NULL,nXControl,nYControl+rcButton.Height()*2-12,0,0,uFlags);
	DeferWindowPos(hDwp,m_btShowHand,NULL,nXControl+rcButton.Width(),nYControl,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes2,NULL,nXControl+rcButton.Width(),nYControl+rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nXControl+rcButton.Width(),nYControl+rcButton.Height()*2-12,0,0,uFlags);
	
	//Lock Game
	DeferWindowPos(hDwp,m_btLockGame,NULL,nWidth-60,30,0,0,uFlags);

	//m_btFollow.GetWindowRect(&rcButton);
	//DeferWindowPos(hDwp,m_btFollow,NULL,nXControl,nYControl,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btShowHand,NULL,nXControl+rcButton.Width()+1,nYControl,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btAddTimes3,NULL,nXControl,nYControl+rcButton.Height()+1,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btAddTimes2,NULL,nXControl+rcButton.Width()+1,nYControl+rcButton.Height()+1,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btAddTimes1,NULL,nXControl,nYControl+rcButton.Height()*2+2,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btGiveUp,NULL,nXControl+rcButton.Width()+1,nYControl+rcButton.Height()*2+2,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//�ɼ�����
	CRect rcScoreView;
	m_ScoreView.GetWindowRect(&rcScoreView);
	m_ScoreView.MoveWindow((nWidth-rcScoreView.Width())/2,(nHeight-rcScoreView.Height())/2-10,rcScoreView.Width(),rcScoreView.Height());

	//�˿˿ؼ�
	m_CardControl[0].SetBasicStation(CPoint(nWidth/2,30+m_nYBorder+85),enXCenter,enYTop);
	//m_CardControl[1].SetBasicStation(CPoint(nWidth-m_nXBorder-7,nHeight/2-32+nPosY),enXRight,enYCenter);
	m_CardControl[1].SetBasicStation(CPoint(nWidth/2,nHeight-30-m_nYBorder-75),enXCenter,enYBottom);
	//m_CardControl[3].SetBasicStation(CPoint(m_nXBorder+10,nHeight/2-32+nPosY),enXLeft,enYCenter);

	//������ʼλ��
	m_SendCardPos.SetPoint(nWidth/2-40,nHeight/2-52);

	m_rcCtrlWinner[0].SetRect(0,0, nWidth, nHeight/3);
	//m_rcCtrlWinner[1].SetRect(nWidth/2,nHeight/3, nWidth, (2*nHeight)/3);
	m_rcCtrlWinner[1].SetRect(0,(2*nHeight)/3, nWidth, nHeight);
	//m_rcCtrlWinner[3].SetRect(0,nHeight/3, nWidth/2, (2*nHeight)/3);

	return;
}

void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime)
{
	CBitImageEx ImageTimeNumber;
	HINSTANCE hInstance=AfxGetInstanceHandle();
	ImageTimeNumber.LoadFromResource(this,hInstance,IDB_TIME_NUMBER);
	//ImageTimeNumber.LoadImage(AfxGetInstanceHandle(),TEXT("IDB_TIME_NUMBER"));//TEXT("TIME_NUMBER"));
	//CImageHandle ImageHandle(&ImageTimeNumber);
	//if (!ImageHandle.IsResourceValid())
	//	return;

	//CImageHandle ImageTimeFrameHandle(&m_ImageTimeFrame);
	m_ImageTimeFrame.TransDrawImage(pDC,nXPos-m_ImageTimeFrame.GetWidth()/2,nYPos-m_ImageTimeFrame.GetHeight()/2,RGB(255,0,255));
	
	//��ȡ����
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//������Ŀ
	LONG lNumberCount=0;
	//WORD wNumberTemp=wTime;
	//do
	//{
	//	lNumberCount++;
	//	wNumberTemp/=10;
	//} while (wNumberTemp>0L);

	lNumberCount=2;
	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;


	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		ImageTimeNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0,RGB(255,0,255));
		//m_ImageBack.AlphaDrawImage(pDesDC,0,0,iPartWidth,m_ImageBack.GetHeight(),iDrawPos,0,m_crTrans);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

}
//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
//	CImageHandle HandleTitle(&m_ImageTitle);
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageViewCenter,DRAW_MODE_CENTENT);
	
//	m_ImageTitle.BitBlt(pDC->m_hDC,m_nXBorder,m_nYBorder);

	//
	//CImageHandle HandleScoreTable(&m_ImageScoreTable);
	
	m_ImageScoreTable.TransDrawImage( pDC,nWidth/2-m_ImageScoreTable.GetWidth()/2,nHeight/2-m_ImageScoreTable.GetHeight()/2,
		RGB(255,179,47) );
	m_ImageTableScore.DrawImage(pDC,nWidth - 235,20,m_ImageTableScore.GetWidth(),m_ImageTableScore.GetHeight(),0,0);
	//�滭�û�
	WORD wUserTimer=0;
	TCHAR szBuffer[64];
	pDC->SetTextColor(RGB(255,179,47));
	//CImageHandle HandleUserReady(&m_ImageUserReady);
	CFont * pOldFont=pDC->SelectObject(&ViewFont);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{

		////////////////////////////////////////////////////////
		//tagUserData *pUserData = new tagUserData;
		//strcpy( pUserData->szName, "dkfjdkfj" );
		//pUserData->cbUserStatus = US_READY ;
		//wUserTimer = 50;
		//////////////////////////////////////////////////////////
	
		//const tagUserData * pUserData=GetUserInfo(i);
		IClientUserItem * pUserData=GetClientUserItem(i);

		if (pUserData!=NULL)
		{
			//ֻ��ʾ�����û��ǳ�
			CString strLog,strNick;
			strNick.Format(L"%s",pUserData->GetNickName());
			strLog.Format(L"%s****",strNick.Left(4));	

			if(pUserData->GetUserStatus()==US_READY)
				m_ImageUserReady.TransDrawImage(pDC,m_ptReady[i].x-m_ImageUserReady.GetWidth()/2,m_ptReady[i].y-m_ImageUserReady.GetHeight()/2,RGB(255,0,255));
			//��������
			//m_ImageUserBack.AlphaDrawImage(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,RGB(0,0,0));
			TCHAR szTemp[256]={0};
			SIZE strSize;
			myprintf(szTemp, sizeof(szTemp), _T("%s"), strLog);
			BOOL bRet = ::GetTextExtentPoint32(pDC->GetSafeHdc(), szTemp, lstrlen(szTemp), &strSize);	//��Ϣ��ռ�����س���
			//�û�����
			pDC->SetTextAlign((i==2)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			//pDC->TextOut(m_ptNickName[i].x-strSize.cx/2,m_ptNickName[i].y,pUserData->GetNickName(),lstrlen(pUserData->GetNickName()));

			CRect rcDraw;
			rcDraw.left=m_ptNickName[i].x-strSize.cx/2;
			rcDraw.top=m_ptNickName[i].y;
			rcDraw.right=rcDraw.left+strSize.cx+30;
			rcDraw.bottom=rcDraw.top+18;
			CDFontEx::DrawText(this,pDC,  16, 400, strLog, &rcDraw, RGB(255,179,47), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

			////�û��Ƹ�
			__int64 lUserScore = (pUserData->GetUserStatus()==US_PLAYING)?m_lUserGold[i]-m_GoldView[i*2].GetGold()-m_GoldView[i*2+1].GetGold():pUserData->GetUserInfo()->lScore;//->lScore;
			myprintf(szBuffer,sizeof(szBuffer),TEXT("[%I64d]"),lUserScore);
			//pDC->TextOut(m_ptNickName[i].x,m_ptNickName[i].y+16,szBuffer,lstrlen(szBuffer));
			BOOL bRets = ::GetTextExtentPoint32(pDC->GetSafeHdc(), szBuffer, lstrlen(szBuffer), &strSize);	//��Ϣ��ռ�����س���
			rcDraw.left=rcDraw.right+6;
			rcDraw.right=rcDraw.left+strSize.cx+6;
			CDFontEx::DrawText(this,pDC,  16, 400, szBuffer, &rcDraw, RGB(255,179,47), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
			//������Ϣ
			wUserTimer=GetUserClock(i);
			if (wUserTimer!=0) 
				DrawUserTimerEx(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
			//if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			//DrawUserFace(pDC,pUserData,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserData->cbUserStatus==US_OFFLINE);
			//�滭�����Ӯ�������
			pDC->SelectObject(&ViewFont1);
			CString strScore;
			//��Ӯ������λ��
			CRect recTableScore;
			recTableScore.left=nWidth-200;
			recTableScore.top=28+i*25+15;
			recTableScore.right=recTableScore.left+92/*lstrlen(pUserData->GetUserInfo()->szNickName)*/;
			recTableScore.bottom=recTableScore.top+18;
			strScore.Format(_T("%s"),pUserData->GetUserInfo()->szNickName);
			//pDC->TextOut(nWidth - 200,30+i*25,strScore,strScore.GetLength());
			CDFontEx::DrawText(this,pDC,  16, 400, strLog, &recTableScore, RGB(255,179,47), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
			if(pUserData->GetUserID() == m_UserWinScore[i].dwUserID)
			{
				strScore.Format(L"%I64d",m_UserWinScore[i].lUserWinScore);
			}
			else
			{
				m_UserWinScore[i].dwUserID = pUserData->GetUserID();
				_tcsnccpy_s(m_UserWinScore[i].tszUserName, pUserData->GetUserInfo()->szNickName, sizeof( pUserData->GetUserInfo()->szNickName));
				m_UserWinScore[i].wChairID = i;
				m_UserWinScore[i].lUserWinScore = 0;
				strScore = L"0";
			}
			//pDC->TextOut(nWidth - 100,30+i*25,strScore,strScore.GetLength());
			recTableScore.left=recTableScore.right+10;
			recTableScore.right=recTableScore.left+92;
			CDFontEx::DrawText(this,pDC,  16, 400,strScore.GetBuffer(0), &recTableScore, RGB(255,179,47), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
			pDC->SelectObject(&ViewFont);
		}
		else
		{
			m_UserWinScore[i].dwUserID = 0;
			_tcsnccpy_s(m_UserWinScore[i].tszUserName, L"#", sizeof( pUserData->GetUserInfo()->szNickName));
			m_UserWinScore[i].wChairID = i;
			m_UserWinScore[i].lUserWinScore = 0;
		}
	}

	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{		
		m_CardControl[i].DrawCardControl(pDC);
	}

	//�滭����
	__int64 lTotalGold=0L;
	for (DWORD i=0;i<CountArray(m_GoldView);i++)
	{
		lTotalGold+=m_GoldView[i].GetGold();
		m_GoldView[i].DrawGoldView(this,pDC,m_GoldStation[i].x,m_GoldStation[i].y,i+1);
	}

	//��Ϸ��Ϣ
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(255,179,47));
	const UINT uDrawFormat=DT_END_ELLIPSIS|DT_CENTER|DT_SINGLELINE|DT_VCENTER;

	////��Ϸ��ע
	if (lTotalGold!=0L)
	{
		myprintf(szBuffer,sizeof(szBuffer),_T("%I64d"),lTotalGold);
		//DrawText(pDC->m_hDC,szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80,m_nYBorder+48,m_nXBorder+140,m_nYBorder+60),uDrawFormat);
		m_TotalGoldView.SetGold( lTotalGold );
		m_TotalGoldView.DrawGoldView(this,pDC,nWidth/2,nHeight/2+10);
	}

	////��ʾ��ע���Ʊ���
	//if( m_bShowScoreControl )
	//{
	//	CImageHandle HandleScoreControl(&m_ImageScoreControlBack);
	//	m_ImageScoreControlBack.AlphaDrawImage( pDC,m_ptScoreControl.x,m_ptScoreControl.y,RGB(255,0,255) );
	//}

	//�滭�˿�
	if (m_SendCardItemArray.GetCount()>0)
	{
		//��������
		//CImageHandle HandleCard(&m_ImageCardBack);
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//�滭�˿�
		INT nCardWidth = m_ImageCardBack.GetWidth()/13;
		INT nCardHeight = m_ImageCardBack.GetHeight()/5;
		m_ImageCardBack.TransDrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nCardWidth,nCardHeight,nCardWidth*2,nCardHeight*4,RGB(255,0,255));
	}

	//�û����
	if( m_bUserShowHand )
	{
		m_PngShowHand.DrawImage( pDC,(nWidth-m_PngShowHand.GetWidth())/2,(nHeight-m_PngShowHand.GetHeight())/2 );
		//m_PngShowHand.DrawImage(pDC,nWidth-m_PngShowHand.GetWidth()/2,(nHeight-m_PngShowHand.GetHeight())/2,m_PngShowHand.GetWidth(),m_PngShowHand.GetHeight(),0,0);
		//MessageBox(L"m_bUserShowHand");
	}


	return;
}

//�Ƹ�����
void CGameClientView::SetGoldTitleInfo(__int64 lMaxGold, __int64 lBasicGold,bool bPoor)
{
	//���ñ���
	m_lMaxGold=lMaxGold;
	m_lBasicGold=lBasicGold;


	//���°�ť����
	if(!bPoor)
	{
		TCHAR szNum[19];
		myprintf(szNum,CountArray(szNum),_T("%I64d"),m_lBasicGold);
		m_btAddTimes1.SetWindowText(szNum);
		myprintf(szNum,CountArray(szNum),TEXT("%I64d"),m_lBasicGold*2);
		m_btAddTimes2.SetWindowText(szNum);
		myprintf(szNum,CountArray(szNum),TEXT("%I64d"),m_lBasicGold*4);
		m_btAddTimes3.SetWindowText(szNum);
	}
	else
	{
		TCHAR szNum[19];
		myprintf(szNum,CountArray(szNum),_T("%I64d"),m_lBasicGold/2);
		m_btAddTimes1.SetWindowText(szNum);
		myprintf(szNum,CountArray(szNum),TEXT("%I64d"),m_lBasicGold);
		m_btAddTimes2.SetWindowText(szNum);
		myprintf(szNum,CountArray(szNum),TEXT("%I64d"),m_lBasicGold*2);
		m_btAddTimes3.SetWindowText(szNum);
	}

	//������ͼ
	RefreshGameView();

	return;
}

//�û�����
void CGameClientView::SetUserGoldInfo(WORD wViewChairID, bool bTableGold, __int64 dwGold)
{
	//�����ж�
	WORD wGoldPos=wViewChairID*2+((bTableGold==true)?1:0);
	if (wGoldPos>=CountArray(m_GoldStation)) return;

	//���ñ���
	__int64 m_dwAllGold=0L;
	m_GoldView[wGoldPos].SetGold(dwGold);
	for (int i=0;i<CountArray(m_GoldStation);i++) m_dwAllGold+=m_GoldView[i].GetGold();

	//������ͼ
	RefreshGameView();

	return;
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//���ʹ���
	if (nIDEvent==IDI_SEND_CARD)
	{
		//����λ��
		m_SendCardCurPos.x += m_nXStep;
		m_SendCardCurPos.y += m_nYStep;

		//���½���
		RefreshGameView();

		//ֹͣ�ж�
		if ( --m_nStepCount == 0 )
		{
			//��ȡ�˿�
			BYTE cbCardData[MAX_COUNT];
			WORD wChairID=m_SendCardItemArray[0].wChairID;
			BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

			//�����˿�
			if (cbCardCount<MAX_COUNT)
			{
				cbCardData[cbCardCount++]=m_SendCardItemArray[0].cbCardData;
				m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
			}

			//ɾ������
			m_SendCardItemArray.RemoveAt(0);

			//��������
			if (m_SendCardItemArray.GetCount()>0)
			{
				//
				wChairID = m_SendCardItemArray[0].wChairID;
				m_nStepCount = SEND_STEP_COUNT;
				m_SendCardCurPos = m_SendCardPos;
				m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
				m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

				//��������
				CGameClientEngine * pGameClientDlg=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
				pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

				return;
			}
			else 
			{
				//��ɴ���
				KillTimer(IDI_SEND_CARD);
				SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

				return;
			}
		}

		return;
	}
#ifdef SHOW_CARD_ANIMATE
	else if( nIDEvent == IDI_SHOW_CARD )
	{
// 		if( !m_CardControl[MYSELF_VIEW_ID].PlayMoveCard() )
// 		{
// 			KillTimer( IDI_SHOW_CARD );
// 		}
// 		RefreshGameView();
// 		return;
		if( !m_CardControl[0].PlayMoveCard() )
		{
			m_CardControl[0].ShowFirstCard(m_bShowCard);
			KillTimer( IDI_SHOW_CARD );
			//SetTimer( IDI_SHOW_CARD_HIDE,500,NULL );
		}
		RefreshGameView();
		return;
	}
	else if( nIDEvent == IDI_SHOW_CARD_ME )
	{
		if( !m_CardControl[MYSELF_VIEW_ID].PlayMoveCard() )
		{
			//	m_CardControl[MYSELF_VIEW_ID].ShowFirstCard(m_bShowCard);
			KillTimer( IDI_SHOW_CARD_ME );
			//SetTimer( IDI_SHOW_CARD_HIDE,500,NULL );
		}
		RefreshGameView();
		return;
	}
#endif
	else if (nIDEvent == IDI_SHOW_CARD_HIDE)
	{	
		m_CardControl[MYSELF_VIEW_ID].ShowFirstCard(false);
		KillTimer(IDI_SHOW_CARD_HIDE);
		
	}
	else if( nIDEvent == IDI_USER_SHOW_HAND )
	{
		KillTimer( IDI_USER_SHOW_HAND );
		m_bUserShowHand = false;
		RefreshGameView();
		return;
	}
	__super::OnTimer(nIDEvent);
}

//�����ж�
bool CGameClientView::IsDispatchCard()
{
	return m_SendCardItemArray.GetCount()>0;
}

//��ɷ���
void CGameClientView::FinishDispatchCard()
{
	//����ж�
	if (m_SendCardItemArray.GetCount()==0) return;

	//��ɶ���
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//��ȡ����
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//ɾ������
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

	//���½���
	RefreshGameView();

	return;
}

//�����˿�
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	tagSendCardItem SendCardItem;
	SendCardItem.wChairID=wChairID;
	SendCardItem.cbCardData=cbCardData;
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//���ö�ʱ��
		SetTimer(IDI_SEND_CARD,SPEED_SEND_CARD,NULL);

		//
		m_nStepCount = SEND_STEP_COUNT;
		m_SendCardCurPos = m_SendCardPos;
		m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
		m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

		//
		RefreshGameView();

		//��������
		CGameClientEngine * pGameClientDlg=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
		pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//
VOID CGameClientView::ShowScoreControl( bool bShow )
{
	int nCmdShow = bShow?SW_SHOW:SW_HIDE;
	//���ý���
	m_btFollow.ShowWindow(nCmdShow);
	m_btGiveUp.ShowWindow(nCmdShow);
	m_btShowHand.ShowWindow(nCmdShow);
	m_btAddTimes1.ShowWindow(nCmdShow);
	m_btAddTimes2.ShowWindow(nCmdShow);
	m_btAddTimes3.ShowWindow(nCmdShow);

	m_bShowScoreControl = bShow;
	//RefreshGameView();
}
//����
void CGameClientView::ShowCard(WORD wChairID,bool bShowFirstCard)
{
	m_wShowCardChairID = wChairID;
	m_bShowCard = bShowFirstCard;
	if(!bShowFirstCard)
	{
		m_CardControl[m_wShowCardChairID].SetCardIndex(0);
		if(	m_CardControl[m_wShowCardChairID].BeginMoveCard() )
		{
			SetTimer(IDI_SHOW_CARD,SPEED_MOVE_CARD,NULL);
			//���½���
			RefreshGameView();

		}
	}
	else if(m_CardControl[m_wShowCardChairID].BeginMoveCard() )
	{
		SetTimer(IDI_SHOW_CARD_ME,SPEED_MOVE_CARD,NULL);
		//���½���
		RefreshGameView();

	}
	return;

}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˲���
	if (m_CardControl[MYSELF_VIEW_ID].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˵��
	if (m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseDown(MousePoint)==true)
	{
#ifdef	SHOW_CARD_ANIMATE
// 		if( m_CardControl[MYSELF_VIEW_ID].BeginMoveCard() )
// 		{
// 			SetTimer( IDI_SHOW_CARD,SPEED_MOVE_CARD,NULL );
// 			//���½���
// 			RefreshGameView();
// 		}
		{
		 SendEngineMessage(IDM_SHOWCARD,0,0);
		}
#else
		m_CardControl[MYSELF_VIEW_ID].ShowFirstCard(true);
		RefreshGameView();
#endif
	}

	return;
}

//
VOID CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˵��
	if (m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseUp(MousePoint)==true)
	{
#ifndef	SHOW_CARD_ANIMATE
		m_CardControl[MYSELF_VIEW_ID].ShowFirstCard(false);
		RefreshGameView();
		PostMessage( WM_SETCURSOR );
#endif
	}
}

//
void CGameClientView::SetUserShowHand( bool bShowHand )
{
	if( m_bUserShowHand != bShowHand )
	{
		m_bUserShowHand = bShowHand;
		
		//
		if( m_bUserShowHand )
			SetTimer( IDI_USER_SHOW_HAND,TIME_USER_SHOW_HAND,NULL );

		RefreshGameView();
	}
}

//
void CGameClientView::SetUserGold( WORD wChairId, __int64 lGold )
{
	if( wChairId == INVALID_CHAIR )
		ZeroMemory( m_lUserGold,sizeof(m_lUserGold) );
	else m_lUserGold[wChairId] = lGold;
	RefreshGameView();
}

//////////////////////////////////////////////////////////////////////////

void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//��������
	CGameClientEngine * pGameClientDlg=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	if (NULL != pGameClientDlg)
	{
		if (GAME_STATUS_FREE == pGameClientDlg->GetGameStatus())
		{
			for (int i=0; i<GAME_PLAYER; ++i)
			{
				if (m_rcCtrlWinner[i].PtInRect(point))
				{			
					// ������Ϣ
					SendEngineMessage(IDM_USE_WINNER,i,0);
					break;
				}
			}
		}
	}

	CRect rect(0,0,1200,1200);
	if (rect.PtInRect(point))
	{
		m_pGameClientDlg->SendSocketData(SUB_C_CHECK_SUPER);

	}

	CGameFrameView::OnLButtonDblClk(nFlags, point);
}


struct tagGameLockOpera
{
	WORD		wLockChairID;
	bool		bLocked;
};

void CGameClientView::OnLockGame(const void * pBuffer, WORD wDataSize)
{
	tagGameLockOpera * gameLock=(tagGameLockOpera*)pBuffer;


	//if(gameLock->wLockChairID!=-1)
	//{
	//	IClientUserItem * pUserData=GetClientUserItem(m_pGameClientDlg->SwitchViewChairID(gameLock->wLockChairID));
	//	if(pUserData)
	//	{
	//		CString stip;
	//		stip.Format("��� %s %s ������",pUserData->GetNickName(),(gameLock->bLocked==true)?"��ס":"����");
	//		m_pGameClientDlg->InsertSystemString(stip);
	//	}
	//}
	//else
	//	m_pGameClientDlg->InsertSystemString(TEXT("ϵͳ�Զ��Ա������н���"));

	HINSTANCE hInstance=AfxGetInstanceHandle();

	if(gameLock->bLocked)
		m_btLockGame.SetButtonImage(IDB_BT_LOCK,hInstance,false,false);
	else
		m_btLockGame.SetButtonImage(IDB_BT_UNLOCK,hInstance,false,false);
}

//������ͼ
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());
	return;
}

