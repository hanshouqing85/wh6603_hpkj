#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_DISPATCH_CARD			102									//���Ʊ�ʶ
#define IDI_DISPATCH_CARD2			103
#define IDI_BOMB_EFFECT				200									//��ը��ʶ
#define IDI_TIP_NORMAL				105									//����������ʶ
#define IDI_ADD_CONTINUE			106									//����Ѻע
#define IDI_FLASH_AND_HIT			107									//�����ʹ�ǹ
#define IDI_GOOD_LUCK_FLASH			108									//��������
#define IDI_TIP_GOOD_LUCK			109
#define IDI_PAO_HUO_CHE_HIT			110									//�ܻ𳵴�ǹ
#define IDI_DISPATCH_CARD3			111									//�ܻ�ת��
#define IDI_ACTIVE_BORDER			112									//��ť��̬�߿�
#define IDI_WAIT_5					113									//���д���Ԫ��С��Ԫ������ϲ���ܻ�ʱ���ȴ�5�룬Ȼ��ʼѺ��С

#define IDI_XU_YA_BUTTON			120

//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_BANKER_UP				212
#define IDC_BANKER_DOWN				213
#define IDC_USER_BANK				214									//���а�ť

#define IDC_ADD_STEP1				215									//��Ͷ��ť
#define IDC_ADD_STEP10				216									//��Ͷ��ť
#define IDC_ADD_STEP100				217									//��Ͷ��ť
#define IDC_ADD_STEP1_H				218									//��Ͷ��ť
#define IDC_ADD_STEP10_H			219									//��Ͷ��ť
#define IDC_ADD_STEP100_H			220									//��Ͷ��ť

#define IDC_BET_LAST				221									//��Ѻ��ť
#define	IDC_BET_SMALL				222									//ѺС��ť
#define IDC_BET_BIG					223									//Ѻ��ť
#define IDC_BET_LAST2				227

#define IDC_PLAYER_LIST				230
#define IDC_APPLY_LIST				231
#define IDC_BT_PLAYER_LIST			232
#define IDC_BT_APPLY_LIST			233

#define IDC_ADMIN					234

//��ը��Ŀ
#define BOMB_EFFECT_COUNT			8									//��ը��Ŀ

//��ע�߿��С
#define FRAME_SIZE_WIDTH			71									//���
#define FRAME_SIZE_HIGHT			77									//�߶�
////��ע�߿��С
//#define FRAME_SIZE_WIDTH			128									//���
//#define FRAME_SIZE_HIGHT			138									//�߶�

//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_ADD_STEP1_H,OnAddStep1_H)
	ON_BN_CLICKED(IDC_ADD_STEP10_H,OnAddStep10_H)
	ON_BN_CLICKED(IDC_ADD_STEP100_H,OnAddStep100_H)
	ON_BN_CLICKED(IDC_ADD_STEP1,OnAddStep1)
	ON_BN_CLICKED(IDC_ADD_STEP10,OnAddStep10)
	ON_BN_CLICKED(IDC_ADD_STEP100,OnAddStep100)
	ON_BN_CLICKED(IDC_APPY_BANKER,OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER,OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L,OnChipMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R,OnChipMoveR)
	ON_BN_CLICKED(IDC_USER_BANK,OnClickedBank)
	ON_BN_CLICKED(IDC_BET_SMALL,OnBetSmall)
	ON_BN_CLICKED(IDC_BET_LAST,OnXuYa)
	ON_BN_CLICKED(IDC_BET_BIG,OnBetBig)
	ON_BN_CLICKED(IDC_BT_PLAYER_LIST,OnPlayerList)
	ON_BN_CLICKED(IDC_BT_APPLY_LIST,OnApplyList)
	ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_BET_LAST2,OnXuYa2)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��ע��Ϣ
	ZeroMemory(m_lMeAreaChip, sizeof(m_lMeAreaChip));
	//ȫ����ע
	ZeroMemory(m_lAllAreaChip, sizeof(m_lAllAreaChip));
	//����ע����ǰ���·�
	ZeroMemory(m_lAreaChipLimit, sizeof(m_lAreaChipLimit));
	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerChip=0L;	
	m_lBankerWinChip=0L;
	//���ֳɼ�
	m_lMeCurGameChip=0L;	
	m_lMeCurGameReturnChip=0L;
	m_lBankerCurGameChip=0L;
	m_lGameRevenue=0L;
	//��������
	//ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	//ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));
	ZeroMemory(m_CartoonTimeElapseArray, sizeof(m_CartoonTimeElapseArray));

	memset(m_cbAreaLight,0,sizeof(m_cbAreaLight));

	//״̬��Ϣ
	m_lCurrentJetton=1L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;

	//λ����Ϣ
	m_nChipHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	m_nCurCartoonIndex = 0;
	m_CartoonElapsIndex = 0;
	m_cbWinArea=0xFF;

	//��ʷ�ɼ�
	m_lMeStatisticChip=0;

	//�ؼ�����
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.LoadFromResource(hInstance,IDB_VIEW_FILL);
	m_ImageViewBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageChipNumber.LoadFromResource(hInstance,IDB_SCORE_NUMBER);

	m_ImageHistoryRecord.LoadFromResource(hInstance,IDB_HISTORY_RECORD);
	m_ImageGoodLuckFlash[0].LoadFromResource(hInstance,IDB_GOOD_LUCK_FLASH1);
	m_ImageGoodLuckFlash[1].LoadFromResource(hInstance,IDB_GOOD_LUCK_FLASH2);

	m_ImageActiveButton1.LoadFromResource(hInstance,IDB_BT_ADDSTEP1);
	m_ImageActiveButton10.LoadFromResource(hInstance,IDB_BT_ADDSTEP10);
	m_ImageActiveButton100.LoadFromResource(hInstance,IDB_BT_ADDSTEP100);
	m_ImageBigSmallNumber.LoadFromResource(hInstance,IDB_BIG_SMALL_NUMBER);

	m_ImageCardCartoon.LoadImage(hInstance,TEXT("CARD_CARTOON"));
	m_ImageCardCartoon2.LoadImage(hInstance,TEXT("CARD_CARTOON2"));

	m_pngActiveBorder1.LoadImage(hInstance,TEXT("ACTIVE_BORDER1"));
	m_pngActiveBorder10.LoadImage(hInstance,TEXT("ACTIVE_BORDER10"));
	m_pngActiveBorder100.LoadImage(hInstance,TEXT("ACTIVE_BORDER100"));

	m_ImageApple.LoadImage(hInstance,TEXT("APPLE"));
	m_ImageApple2.LoadImage(hInstance,TEXT("APPLE2"));
	m_ImageBar.LoadImage(hInstance,TEXT("BAR"));
	m_ImageBar2.LoadImage(hInstance,TEXT("BAR2"));
	m_ImageBell.LoadImage(hInstance,TEXT("BELL"));
	m_ImageBell2.LoadImage(hInstance,TEXT("BELL2"));
	m_ImageSeven.LoadImage(hInstance,TEXT("D_SEVEN"));
	m_ImageSeven2.LoadImage(hInstance,TEXT("D_SEVEN2"));
	m_ImageStar.LoadImage(hInstance,TEXT("D_STAR"));
	m_ImageStar2.LoadImage(hInstance,TEXT("D_STAR2"));
	m_ImageOrange.LoadImage(hInstance,TEXT("ORANGE"));
	m_ImageOrange2.LoadImage(hInstance,TEXT("ORANGE2"));
	m_ImagePaw.LoadImage(hInstance,TEXT("PAWPAW"));
	m_ImagePaw2.LoadImage(hInstance,TEXT("PAWPAW2"));
	m_ImageWaterMelon.LoadImage(hInstance,TEXT("WATERMELON"));
	m_ImageWaterMelon2.LoadImage(hInstance,TEXT("WATERMELON2"));

	m_ImageBetNumber.LoadImage(hInstance,TEXT("BET_NUMBER"));
	m_ImageBetBigNumber.LoadImage(hInstance,TEXT("BET_BIG_NUMBER"));

	m_ImageFreeTime.LoadImage(hInstance,TEXT("FREETIME"));
	m_ImageBitSmallTime.LoadImage(hInstance,TEXT("BITSMALLTIME"));
	m_ImageJettonTime.LoadImage(hInstance,TEXT("JETTONTIME"));

	m_pngJettonBack.LoadImage(hInstance,TEXT("PNG_JETTON_BACK"));
	m_pngJettonButton.LoadImage(hInstance,TEXT("PNG_JETTON_BUTTON"));
	m_pngMove.LoadImage(hInstance,TEXT("PNG_MOVE"));
	m_pngSongJiang.LoadImage(hInstance,TEXT("RUNTIME"));

	m_pngGoodLuckTip[0].LoadImage(hInstance,TEXT("GOODLUCK_0"));
	m_pngGoodLuckTip[1].LoadImage(hInstance,TEXT("GOODLUCK_1"));
	m_pngGoodLuckTip[2].LoadImage(hInstance,TEXT("GOODLUCK_2"));
	m_pngGoodLuckTip[3].LoadImage(hInstance,TEXT("GOODLUCK_3"));
	m_pngGoodLuckTip[4].LoadImage(hInstance,TEXT("GOODLUCK_4"));
	m_pngBigSmall[0].LoadImage(hInstance,TEXT("BIGSMALL_LOSE"));
	m_pngBigSmall[1].LoadImage(hInstance,TEXT("BIGSMALL_WIN"));
	m_pngLine.LoadImage(hInstance,TEXT("PNG_LINE"));
	m_pngSGZZLogo.LoadImage(hInstance,TEXT("SGZZ_LOGO"));
	m_pngRecordTip.LoadImage(hInstance,TEXT("RECORD_TIP"));

	m_bisLButtonDown = false;

	m_cbEndCount = 0;
	m_bBigSmallTime=false;
	m_bAlreadyGuess=false;
	m_cbGameEndTime=0;
	m_bGoodLuckTypeTip=false;
	m_cbPaoHuoCheCount=0;
	m_cbPaoHuoCheIndex=0;
	m_cbGoodLuckType=0;
	m_bBigSmallWin=false;
	m_bShowSGZZLogo=false;
	m_bGoodLuckFlash=false;
	m_cbFlashAndHitCount=0;
	m_cbGoodLuckFlashCount=0;
	m_cbGoodLuckTipCount=0;
	m_bShowPlayerList=true;
	ZeroMemory(m_cbPaoHuoCheArea,sizeof(m_cbPaoHuoCheArea));
	ZeroMemory(m_cbMyBigSmallResult,sizeof(m_cbMyBigSmallResult));

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
	CRect rcCreate(0,0,0,0);

	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	//����Ͷע��ť
	m_btAddStep1.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP1);
	m_btAddStep10.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP10);
	m_btAddStep100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP100);

	//m_btAddStep1_H.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP1_H);
	//m_btAddStep10_H.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP10_H);
	//m_btAddStep100_H.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP100_H);

	m_btChipMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btChipMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	m_btBankButton.Create(NULL, WS_CHILD|WS_VISIBLE, rcCreate, this, IDC_USER_BANK);

	m_btBetLast.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BET_LAST);
	m_btBetSmall.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BET_SMALL);
	m_btBetBig.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BET_BIG);
	m_btPlayerList.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_PLAYER_LIST);
	m_btApplyList.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_APPLY_LIST);
	m_btPlayerList_H.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_PLAYER_LIST);
	m_btApplyList_H.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_APPLY_LIST);
	m_btBetLast2.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BET_LAST2);

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	m_btBankButton.SetButtonImage(IDB_BANK, hResInstance, false,false);

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false,false);

	m_btChipMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
	m_btChipMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);

	m_btAddStep1.SetButtonImage(IDB_BT_ADDSTEP1,hResInstance,false,false);
	m_btAddStep10.SetButtonImage(IDB_BT_ADDSTEP10,hResInstance,false,false);
	m_btAddStep100.SetButtonImage(IDB_BT_ADDSTEP100,hResInstance,false,false);
	//m_btAddStep1_H.SetButtonImage(IDB_BT_ADDSTEP1_H,hResInstance,false,false);
	//m_btAddStep10_H.SetButtonImage(IDB_BT_ADDSTEP10_H,hResInstance,false,false);
	//m_btAddStep100_H.SetButtonImage(IDB_BT_ADDSTEP100_H,hResInstance,false,false);

 	m_btBetLast.SetButtonImage(IDB_BT_BET_LAST,hResInstance,false,false);
 	m_btBetSmall.SetButtonImage(IDB_BT_BET_SMALL,hResInstance,false,false);
 	m_btBetBig.SetButtonImage(IDB_BT_BET_BIG,hResInstance,false,false);
	m_btPlayerList.SetButtonImage(IDB_BT_PLAYER_LIST,hResInstance,false,false);
	m_btApplyList.SetButtonImage(IDB_BT_APPLY_LIST,hResInstance,false,false);
	m_btPlayerList_H.SetButtonImage(IDB_BT_PLAYER_LIST_H,hResInstance,false,false);
	m_btApplyList_H.SetButtonImage(IDB_BT_APPLY_LIST_H,hResInstance,false,false);
	m_btBetLast2.SetButtonImage(IDB_BT_BET_LAST2,hResInstance,false,false);

	m_btPlayerList.ShowWindow(SW_HIDE);
	m_btApplyList.ShowWindow(SW_SHOW);
	m_btPlayerList_H.ShowWindow(SW_SHOW);
	m_btApplyList_H.ShowWindow(SW_HIDE);

	m_fontChipLimit.CreateFont(-14,0,0,0,300,0,0,0,134,3,2,1,2,TEXT("����"));

	m_PlayerList.Create(LVS_REPORT|LVS_NOCOLUMNHEADER|WS_CLIPCHILDREN,CRect(0,0,0,0),this,IDC_PLAYER_LIST);
	m_PlayerList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//m_PlayerList.ModifyStyle(LVS_NOSCROLL,0);
	//InitializeFlatSB(m_PlayerList.GetSafeHwnd());
	//FlatSB_EnableScrollBar(m_PlayerList.GetSafeHwnd(),SB_VERT,ESB_DISABLE_RIGHT);
	m_PlayerList.SetBkColor(RGB(20,20,20));
	m_PlayerList.SetTextBkColor(RGB(20,20,20));
	m_PlayerList.SetTextColor(RGB(255,255,255));
	m_PlayerList.InsertColumn(0,TEXT("PlayerName"),0,85);
	m_PlayerList.InsertColumn(1,TEXT("PlayerChip"),0,80);

	m_ApplyList.Create(LVS_REPORT|LVS_NOCOLUMNHEADER|WS_CLIPCHILDREN,CRect(0,0,0,0),this,IDC_APPLY_LIST);
	m_ApplyList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ApplyList.SetBkColor(RGB(20,20,20));
	m_ApplyList.SetTextBkColor(RGB(20,20,20));
	m_ApplyList.SetTextColor(RGB(255,255,255));
	m_ApplyList.InsertColumn(0,TEXT("PlayerName"),0,85);
	m_ApplyList.InsertColumn(1,TEXT("PlayerChip"),0,80);

	m_MyCursor[0]=(HCURSOR)LoadImage(hResInstance,MAKEINTRESOURCE(IDC_CURSOR1),IMAGE_CURSOR,48,48,LR_SHARED);
	m_MyCursor[1]=(HCURSOR)LoadImage(hResInstance,MAKEINTRESOURCE(IDC_CURSOR2),IMAGE_CURSOR,48,48,LR_SHARED);
	m_MyCursor[2]=(HCURSOR)LoadImage(hResInstance,MAKEINTRESOURCE(IDC_CURSOR3),IMAGE_CURSOR,48,48,LR_SHARED);

	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	m_AdminDlg.Create(IDD_DIALOG_SYSTEM,this);

	SetTimer(IDI_ACTIVE_BORDER,100,NULL);

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lMeAreaChip, sizeof(m_lMeAreaChip));
	//ȫ����ע
	ZeroMemory(m_lAllAreaChip, sizeof(m_lAllAreaChip));
	//����ע����ǰ���·�
	ZeroMemory(m_lAreaChipLimit, sizeof(m_lAreaChipLimit));
	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerChip=0L;	
	m_lBankerWinChip=0L;
	//���ֳɼ�
	m_lMeCurGameChip=0L;
	m_lMeCurGameReturnChip=0L;
	m_lBankerCurGameChip=0L;
	m_lGameRevenue=0L;
	//��������
	//ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	//ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));
	ZeroMemory(m_CartoonTimeElapseArray, sizeof(m_CartoonTimeElapseArray));

	//memset(m_bAreaLight,0,sizeof(m_bAreaLight));

	//״̬��Ϣ
	m_lCurrentJetton=1L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	//λ����Ϣ
	m_nChipHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	
	//��ʷ�ɼ�
	m_lMeStatisticChip=0;

	m_nCurCartoonIndex = 0;
	m_CartoonElapsIndex = 0;
	m_cbWinArea=0xFF;

	//�������
	CleanUserJetton();

	//��������
	//ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	//���ð�ť
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

	m_bisLButtonDown = false;

	m_cbEndCount = 0;
	m_bBigSmallTime=false;
	m_bAlreadyGuess=false;
	m_cbGameEndTime=0;
	m_bGoodLuckTypeTip=false;
	m_cbPaoHuoCheCount=0;
	m_cbPaoHuoCheIndex=0;
	m_cbGoodLuckType=0;
	m_bBigSmallWin=false;
	m_bShowSGZZLogo=false;
	m_bGoodLuckFlash=false;
	m_cbFlashAndHitCount=0;
	m_cbGoodLuckFlashCount=0;
	ZeroMemory(m_cbPaoHuoCheArea,sizeof(m_cbPaoHuoCheArea));

	return;
}

//�����ؼ�
//void CGameClientView::RectifyGameView(int nWidth, int nHeight)
void CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	//λ����Ϣ
	//����λ�û�׼��
	int nCenterX=nWidth/2, nCenterY=nHeight/2;

	int iTopTemp=262;
	//BAR��ע����
	m_rcJettonArea[0].left=nCenterX-364;
	m_rcJettonArea[0].top=nCenterY+iTopTemp;
	m_rcJettonArea[0].right=m_rcJettonArea[0].left+FRAME_SIZE_WIDTH;
	m_rcJettonArea[0].bottom=m_rcJettonArea[0].top+FRAME_SIZE_HIGHT;

	//˫����ע����
	m_rcJettonArea[1].left=m_rcJettonArea[0].right;
	m_rcJettonArea[1].top=nCenterY+iTopTemp;
	m_rcJettonArea[1].right=m_rcJettonArea[1].left+FRAME_SIZE_WIDTH;
	m_rcJettonArea[1].bottom=m_rcJettonArea[1].top+FRAME_SIZE_HIGHT;

	//������ע����
	m_rcJettonArea[2].left=m_rcJettonArea[1].right;
	m_rcJettonArea[2].top=nCenterY+iTopTemp;
	m_rcJettonArea[2].right=m_rcJettonArea[2].left+FRAME_SIZE_WIDTH;
	m_rcJettonArea[2].bottom=m_rcJettonArea[2].top+FRAME_SIZE_HIGHT;

	//������ע����
	m_rcJettonArea[3].left=m_rcJettonArea[2].right;
	m_rcJettonArea[3].top=nCenterY+iTopTemp;
	m_rcJettonArea[3].right=m_rcJettonArea[3].left+FRAME_SIZE_WIDTH;
	m_rcJettonArea[3].bottom=m_rcJettonArea[3].top+FRAME_SIZE_HIGHT;

	//������ע����
	m_rcJettonArea[4].left=m_rcJettonArea[3].right+7;
	m_rcJettonArea[4].top=nCenterY+iTopTemp;
	m_rcJettonArea[4].right=m_rcJettonArea[4].left+FRAME_SIZE_WIDTH;
	m_rcJettonArea[4].bottom=m_rcJettonArea[4].top+FRAME_SIZE_HIGHT;

	//ľ����ע����
	m_rcJettonArea[5].left=m_rcJettonArea[4].right-4;
	m_rcJettonArea[5].top=nCenterY+iTopTemp;
	m_rcJettonArea[5].right=m_rcJettonArea[5].left+FRAME_SIZE_WIDTH;
	m_rcJettonArea[5].bottom=m_rcJettonArea[5].top+FRAME_SIZE_HIGHT;

	//������ע����
	m_rcJettonArea[6].left=m_rcJettonArea[5].right-6;
	m_rcJettonArea[6].top=nCenterY+iTopTemp;
	m_rcJettonArea[6].right=m_rcJettonArea[6].left+FRAME_SIZE_WIDTH;
	m_rcJettonArea[6].bottom= m_rcJettonArea[6].top+FRAME_SIZE_HIGHT;

	//ƻ����ע����
	m_rcJettonArea[7].left=m_rcJettonArea[6].right-6;
	m_rcJettonArea[7].top=nCenterY+iTopTemp;
	m_rcJettonArea[7].right=m_rcJettonArea[7].left+FRAME_SIZE_WIDTH;
	m_rcJettonArea[7].bottom=m_rcJettonArea[7].top+FRAME_SIZE_HIGHT;

	m_rcChipLimit[0].SetRect(nCenterX + 281, nCenterY + 195, nCenterX + 381, nCenterY + 215);
	m_rcChipLimit[1].SetRect(nCenterX + 193, nCenterY + 195, nCenterX + 293, nCenterY + 215);
	m_rcChipLimit[2].SetRect(nCenterX + 281, nCenterY + 136, nCenterX + 381, nCenterY + 156);
	m_rcChipLimit[3].SetRect(nCenterX + 193, nCenterY + 136, nCenterX + 293, nCenterY + 156);
	m_rcChipLimit[4].SetRect(nCenterX + 281, nCenterY + 77, nCenterX + 381, nCenterY + 97);
	m_rcChipLimit[5].SetRect(nCenterX + 193, nCenterY + 77, nCenterX + 293, nCenterY + 97);
	m_rcChipLimit[6].SetRect(nCenterX + 281, nCenterY + 18, nCenterX + 381, nCenterY + 38);
	m_rcChipLimit[7].SetRect(nCenterX + 193, nCenterY + 18, nCenterX + 293, nCenterY + 38);

	//������ת·����ÿһ��Ļ�׼��
	CreateTurnPathAreaBasePoint(nWidth, nHeight);

	//����������ׯ����б���Ϣ����
	CreateApplayBankerListRect(nWidth, nHeight);

	//������ʷ��¼��ʾ����
	CreateHistoryRecordRect(nWidth,nHeight);

	m_PointJettonNumber[0].SetPoint(nCenterX-296,nCenterY+289);
	m_PointJettonNumber[1].SetPoint(m_PointJettonNumber[0].x+67,nCenterY+289);
	m_PointJettonNumber[2].SetPoint(m_PointJettonNumber[1].x+66,nCenterY+289);
	m_PointJettonNumber[3].SetPoint(m_PointJettonNumber[2].x+66,nCenterY+289);
	m_PointJettonNumber[4].SetPoint(m_PointJettonNumber[3].x+74,nCenterY+289);
	m_PointJettonNumber[5].SetPoint(m_PointJettonNumber[4].x+66,nCenterY+289);
	m_PointJettonNumber[6].SetPoint(m_PointJettonNumber[5].x+66,nCenterY+289);
	m_PointJettonNumber[7].SetPoint(m_PointJettonNumber[6].x+66,nCenterY+289);

	//for(int i=0; i<JETTON_AREA_COUNT; i++ )
	//{
	//	//����ͼƬ��ʼλ��
	//	m_PointJetton[i].SetPoint(m_rcJettonArea[i].left, m_rcJettonArea[i].top);
	//}
	
	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(24);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nCenterX+331, nCenterY-248,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nCenterX+331, nCenterY-248,0,0,uFlags|SWP_NOSIZE);

	//��ʷ��¼���ϣ����°�ť
	DeferWindowPos(hDwp,m_btChipMoveL,NULL,nCenterX-386,nCenterY-234,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChipMoveR,NULL,nCenterX-386,nCenterY+197,0,0,uFlags|SWP_NOSIZE);

	//���а�ť
	DeferWindowPos(hDwp, m_btBankButton, NULL, nCenterX-420-3+1+30+2, nCenterY+230-8+4+3-2, 0, 0, uFlags|SWP_NOSIZE);

	//��Ͷ��ť
	DeferWindowPos(hDwp,m_btAddStep1,NULL,nCenterX + 300-20-10,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btAddStep10,NULL,nCenterX + 332-20-10,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btAddStep100,NULL,nCenterX + 372-20-10,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);

	//DeferWindowPos(hDwp,m_btAddStep1_H,NULL,nCenterX + 300,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btAddStep10_H,NULL,nCenterX + 332,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btAddStep100_H,NULL,nCenterX + 372,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btBetLast,NULL,nCenterX + 230-15-10+7+7,nCenterY + 270+10+7-1,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBetSmall,NULL,nCenterX + 292-15-10+7,nCenterY + 270+10+7-1,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBetBig,NULL,nCenterX + 354-15-10,nCenterY + 270+10+7-1,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBetLast2,NULL,nCenterX + 230-15-10,nCenterY + 270+10+7-1,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btPlayerList,NULL,nCenterX+188,nCenterY-192,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btApplyList,NULL,nCenterX+188+101,nCenterY-192,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btPlayerList_H,NULL,nCenterX+188,nCenterY-192,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btApplyList_H,NULL,nCenterX+188+101,nCenterY-192,0,0,uFlags|SWP_NOSIZE);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	m_PlayerList.SetWindowPos(NULL,nCenterX+197,nCenterY-138,184,92,SWP_NOZORDER|SWP_NOREDRAW);
	m_ApplyList.SetWindowPos(NULL,nCenterX+197,nCenterY-138,184,92,SWP_NOZORDER|SWP_NOREDRAW);

	if (m_bShowPlayerList)
	{
		m_PlayerList.ShowWindow(SW_SHOW);
		m_ApplyList.ShowWindow(SW_HIDE);
		m_PlayerList.Invalidate(TRUE);
	}
	else
	{
		m_ApplyList.ShowWindow(SW_SHOW);
		m_PlayerList.ShowWindow(SW_HIDE);
		m_ApplyList.Invalidate(TRUE);
	}

	return;
}

//������ת·���ϸ�С��������ĵ�
void CGameClientView::CreateTurnPathAreaBasePoint(int nWidth, int nHeight)
{
	int nCenterX=nWidth/2;
	int nCenterY=nHeight/2;
	int w = 70;	//������
	int h = 70; //����߶�

	//��һ����
	int BaseX = nCenterX-291;
	int BaseY = nCenterY-251;
	for( int i=0; i<7; i++ )
	{
		m_PtCardArray[i].SetPoint(BaseX+i*w, BaseY);
	}

	//�ڶ�����
	BaseX = nCenterX+129;//307
	BaseY = nCenterY-251+h;
	int j=0;
	for(int i=7; i<12; i++ )
	{
		m_PtCardArray[i].SetPoint(BaseX, BaseY+(j++)*h);
	}

	//��������
	BaseX = nCenterX+129;//307
	BaseY = nCenterY+99+h;
	j=0;
	for( int i=12; i<19; i++ )
	{
		m_PtCardArray[i].SetPoint(BaseX-(j++)*w, BaseY);
	}

	//��������
	BaseX = nCenterX-291;
	BaseY = nCenterY+29+h;
	j=0;
	for( int i=19; i<24; i++ )
	{
		m_PtCardArray[i].SetPoint(BaseX, BaseY-(j++)*h);
	}
}

void CGameClientView::CreateApplayBankerListRect(int nBaseX, int nBaseY)
{
	int nCenterX = nBaseX/2;
	int nCenterY = nBaseY/2;

	//����������ׯ����б���Ϣ����
	int vSpace = 15;
	int x1 = nCenterX+170;
	int x2 = nCenterX+200;
	int x3 = nCenterX+295;
	int x4 = nCenterX+395;
	int y = nCenterY - 125;
	for( int i=0; i<CountArray(m_rcApplayBankerNo); i++)
	{
		m_rcApplayBankerNo[i].SetRect(x1, y+i*vSpace, x2, y+(i+1)*vSpace);
	}
	for( int i=0; i<CountArray(m_rcApplayBankerName); i++ )
	{
		m_rcApplayBankerName[i].SetRect(x2,y+i*vSpace,x3, y+(i+1)*vSpace);
	}
	for( int i=0; i<CountArray(m_rcApplayBankerChip); i++ )
	{
		m_rcApplayBankerChip[i].SetRect(x3, y+i*vSpace,x4,y+(i+1)*vSpace);
	}
}

//������ʷ��¼��ʾ��
void CGameClientView::CreateHistoryRecordRect(int nWidth, int nHeight)
{
	int nCenterX=nWidth/2;
	int nCenterY=nHeight/2;
	int nRectWidth=39;
	int nRectHeight=39+3;
	int nBaseX=nCenterX-388-1;
	int nBaseY=nCenterY-221-1;

	for (int i=0; i<CountArray(m_rcHistoryRecord); i++)
	{
		//left��top��right��bottom
		m_rcHistoryRecord[i].SetRect(nBaseX,nBaseY+nRectHeight*i,nBaseX+nRectWidth,nBaseY+nRectHeight*(i+1));
	}
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);

	int nCenterX = nWidth / 2;
	int nCenterY = nHeight / 2;

	DrawGoodLuckFlash(pDC,nWidth,nHeight);

	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	if (m_bShowSGZZLogo)
	{
		m_pngSGZZLogo.DrawImage(pDC,nCenterX-229,nCenterY-199);
	}

	//״̬��ʾ
	//CFont InfoFont;
	//InfoFont.CreateFont(-13,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	//CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,234,0));

	//ʱ����ʾ
	DrawTimeTip(pDC, nWidth, nHeight);

	//��ǰ���·�
	DrawAreaLimitChip(pDC);
	
	//pDC->SelectObject(pOldFont);
	//InfoFont.DeleteObject();

	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	if (m_bGoodLuckTypeTip)
	{
		DrawGoodLuckTip(pDC,nWidth,nHeight,0xFF);
	}

	if (m_bBigSmallTime)
	{
		DrawEndCartoon(pDC,nWidth,nHeight,m_cbWinArea);
	}

	if (!m_bGoodLuckTypeTip && cbGameStatus==GS_GAME_END && !m_bBigSmallTime && m_cbGoodLuckType>0 && m_cbGoodLuckType<5)
	{
		int iXPos=nWidth/2-m_pngSongJiang.GetWidth()/2-81;
		int iYPos=nHeight/2-210;
		m_pngSongJiang.DrawImage(pDC,iXPos,iYPos);
	}

	//���Ƴ���
	//DrawAreaJetton(pDC);

	//ׯ����Ϣ		
	DrawBankerInfo(pDC, nWidth, nHeight);

	//���Ƶ�ǰ��ҳ���
	DrawMyChip(pDC,nWidth,nHeight);

	//�����û��÷�
	DrawCurrentChip(pDC,nWidth,nHeight);

	if (m_bBigSmallTime)
	{
		//��С��ʷ��¼
		DrawMyBigSmallNumber(pDC);
	}
	else
	{
		//�ҵ���ע
		DrawMyJettonNumber(pDC);
	}

	//�滭ʱ��
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END)
		{
			WORD wUserTimer=GetUserClock(m_wMeChairID);
			DrawUserTimer(pDC,nCenterX-42,nCenterY+72,wUserTimer,99);
		}
		else DrawUserTimer(pDC,nCenterX-42,nCenterY+72,m_cbGameEndTime,99);
	}

	//��ʾ���
	//ShowGameResult(pDC, nWidth, nHeight);	

	//��ըЧ��
	//DrawBombEffect(pDC);

	//����������ׯ����б�
	//DrawApplyBankerUserList(pDC);
	
	//������ʷ��¼
	DrawHistoryRecord(pDC);

	//���ƶ���
	DrawCardCartoon(pDC);

	//��������߿�
	DrawAreaLight(pDC);

	//������ע��ť
	DrawAddMoney(pDC, nWidth,nHeight );

	//�滭�û�
	//DrawUserInfo(pDC, nWidth, nHeight);

	//�л�ׯ��
	//DrawChangeBanker(pDC, nWidth, nHeight);

	if (m_lCurrentJetton==1)
	{
		m_rectActiveBorderUpdate=CRect(nCenterX+300-20-10,nCenterY+230,32,22);
		m_ImageActiveButton1.AlphaDrawImage(pDC,nCenterX+300-20-10,nCenterY+230,32,22,3*32,0,255);
		m_pngActiveBorder1.DrawImage(pDC,nCenterX+300-20-10,nCenterY+230,32,22,m_cbActiveBorderFrameCount%4*32,0);
	}
	else if (m_lCurrentJetton==10)
	{
		m_rectActiveBorderUpdate=CRect(nCenterX+332-20-10,nCenterY+230,40,22);
		m_ImageActiveButton10.AlphaDrawImage(pDC,nCenterX+332-20-10,nCenterY+230,40,22,3*40,0,255);
		m_pngActiveBorder10.DrawImage(pDC,nCenterX+332-20-10,nCenterY+230,40,22,m_cbActiveBorderFrameCount%4*40,0);
	}
	else if (m_lCurrentJetton==100)
	{
		m_rectActiveBorderUpdate=CRect(nCenterX+372-20-10,nCenterY+230,48,22);
		m_ImageActiveButton100.AlphaDrawImage(pDC,nCenterX+372-20-10,nCenterY+230,48,22,3*48,0,255);
		m_pngActiveBorder100.DrawImage(pDC,nCenterX+372-20-10,nCenterY+230,48,22,m_cbActiveBorderFrameCount%4*48,0);
	}

	return;
}

//����GoodLuck��˸
void CGameClientView::DrawGoodLuckFlash(CDC * pDC, int nWidth, int nHeight)
{
	if (m_bGoodLuckFlash)
	{
		int nCenterX=nWidth/2;
		int nCenterY=nHeight/2;
		int iXPos=nWidth/2-170;
		int iYPos=nHeight/2-100-110;
		m_ImageGoodLuckFlash[m_cbGoodLuckFlashCount%2].AlphaDrawImage(pDC,nCenterX-328,nCenterY-286,255);
		//if (m_cbGoodLuckFlashCount%4==1 || m_cbGoodLuckFlashCount%4==2)//�ƹ�
		if (m_cbGoodLuckFlashCount%2==0)
		{
			m_pngLine.DrawImage(pDC,iXPos-283,iYPos-111,50,538,0,0);
			m_pngLine.DrawImage(pDC,iXPos+571,iYPos-111,50,538,0,0);
		}
	}
	else
	{
		KillTimer(IDI_GOOD_LUCK_FLASH);
	}
	return;
}

//����ׯ����Ϣ
void CGameClientView::DrawBankerInfo(CDC * pDC, int nWidth, int nHeight)
{
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;
	pDC->SetTextColor(RGB(255,234,0));

	//��ȡ���
	IClientUserItem *pUserItem = ( m_wBankerUser==INVALID_CHAIR ? NULL : GetClientUserItem(m_wBankerUser) );
	tagUserInfo *pUserData = NULL;
	if(pUserItem!=NULL)
		pUserData = pUserItem->GetUserInfo();

	//λ����Ϣ
	CRect StrRect;
	StrRect.left=nCenterX+229;
	StrRect.top=nCenterY-281;
	StrRect.right=StrRect.left+97;
	StrRect.bottom=StrRect.top+13;

	//ׯ������
	pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->szNickName,StrRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);

	//////////
	static WORD wOldBankerTime;
	static __int64 lOldBankerWinChip;
	//if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END && m_cbWinnerSide!=0xFF)
	{
		wOldBankerTime=m_wBankerTime;
		lOldBankerWinChip=m_lBankerWinChip;
	}
	//////////

	//ׯ�Ҿ���
	StrRect.top=nCenterY-259;
	StrRect.bottom=StrRect.top+13;
	DrawNumberStringWithSpace(pDC,wOldBankerTime,StrRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);

	//ׯ�ҳɼ�
	StrRect.top=nCenterY-237;
	StrRect.bottom=StrRect.top+13;
	DrawNumberStringWithSpace(pDC,lOldBankerWinChip,StrRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);

	//ׯ���ܷ�
	StrRect.top=nCenterY-215;
	StrRect.bottom=StrRect.top+13;
#ifndef FUNC_CHIP
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:m_pGameClientDlg->ScoreToChip(pUserData->lScore),StrRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
#else
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:m_pGameClientDlg->ScoreToChip(m_pGameClientDlg->m_lCurrentChip[pUserData->wChairID]),StrRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
#endif
}

//�����м���Ϣ
void CGameClientView::DrawUserInfo(CDC * pDC, int nWidth, int nHeight)
{
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		IClientUserItem *pUserItem =GetClientUserItem(m_wMeChairID);
		tagUserInfo *pMeUserData = NULL;
		if(pUserItem!=NULL)
			pMeUserData = pUserItem->GetUserInfo();
		if ( pMeUserData != NULL )
		{
			//��Ϸ��Ϣ
			TCHAR szResultChip[16]=TEXT("");
			TCHAR szGameChip[16]=TEXT("");
			pDC->SetTextColor(RGB(255,255,255));

			__int64 iMeJetton = 0;
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				iMeJetton += m_lMeAreaChip[i];

			CRect rcAccount(CPoint(nCenterX-295,nCenterY+225),CPoint(nCenterX-195,nCenterY+245));
			CRect rcGameChip(CPoint(nCenterX-295,nCenterY+245),CPoint(nCenterX-195,nCenterY+265));
			CRect rcResultChip(CPoint(nCenterX-295,nCenterY+265),CPoint(nCenterX-195,nCenterY+285));
#ifndef FUNC_CHIP
			DrawNumberStringWithSpace(pDC,m_pGameClientDlg->ScoreToChip(pMeUserData->lScore)-iMeJetton,rcGameChip,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
#endif
			DrawNumberStringWithSpace(pDC,m_lMeStatisticChip,rcResultChip,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

			pDC->DrawText(pMeUserData->szNickName,lstrlen(pMeUserData->szNickName),rcAccount,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		}
	}
}

//����ʱ����ʾ
void CGameClientView::DrawTimeTip(CDC * pDC, int nWidth, int nHeight)
{
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();
	int nCenterX = nWidth/2-6;
	int nCenterY = nHeight/2+45;
	//////////
	WORD wUserTimer=0;
	if (m_wMeChairID!=INVALID_CHAIR)
		wUserTimer=GetUserClock(m_wMeChairID);
	//////////
	//ʱ����ʾ
	if (cbGameStatus==GAME_STATUS_FREE)
	{
		m_ImageFreeTime.DrawImage(pDC,nCenterX-210,nCenterY-100);
		m_bGoodLuckFlash=false;
	}
	else if (cbGameStatus==GS_PLACE_JETTON)
	{
		int nJettonWidth = m_ImageJettonTime.GetWidth()/2;
		int nJettonHeight = m_ImageJettonTime.GetHeight();
		m_ImageJettonTime.DrawImage(pDC,nCenterX-210,nCenterY-100,nJettonWidth,nJettonHeight,(wUserTimer%2)*nJettonWidth,0);
	}
	else if (cbGameStatus==GS_GAME_END)
	{
		if (m_bBigSmallTime)
		{
			if (m_bAlreadyGuess)
			{
				int nSmallWidth = m_pngBigSmall[0].GetWidth()/2;
				int nSmallHeight = m_pngBigSmall[0].GetHeight();
				if (m_bBigSmallWin)
					m_pngBigSmall[1].DrawImage(pDC,nCenterX-210+13,nCenterY-92,nSmallWidth,nSmallHeight,(m_cbEndCount%2)*nSmallWidth,0);
				else
					m_pngBigSmall[0].DrawImage(pDC,nCenterX-210+13,nCenterY-92,nSmallWidth,nSmallHeight,(m_cbEndCount%2)*nSmallWidth,0);
			}
			else
			{
				int nSmallWidth = m_ImageBitSmallTime.GetWidth()/2;
				int nSmallHeight = m_ImageBitSmallTime.GetHeight();
				m_ImageBitSmallTime.DrawImage(pDC,nCenterX-210,nCenterY-100,nSmallWidth,nSmallHeight,(wUserTimer%2)*nSmallWidth,0);
			}
		}
	}
	//m_ImageTimeFlag.AlphaDrawImage(pDC, nCenterX-100, nCenterY-186, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(), nFlagIndex*nTimeFlagWidth, 0, GetPixel(pDC->GetSafeHdc(), 0, 0));
}

//������ע��ť
void CGameClientView::DrawAddMoney(CDC * pDC, int nWidth, int nHeight)
{
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;
	int nImageWidth = m_pngJettonButton.GetWidth()/8;
	int nImageHight = m_pngJettonButton.GetHeight();

	//��ȡ��ע����
	//��ȡ����
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);
	BYTE cbJettonArea= GetJettonArea(MousePoint);

	if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)//ֻ����עʱ��ʹ��ע��ť��Ч
	{
		m_pngJettonBack.DrawImage(pDC,nCenterX-365-9,nCenterY+260+1);
	}

	if( m_bisLButtonDown == true )
	{
		if ( cbJettonArea!=0xFF ) 
		{
			m_pngJettonButton.DrawImage(pDC,m_rcJettonArea[cbJettonArea].left,m_rcJettonArea[cbJettonArea].top,
				nImageWidth,nImageHight,nImageWidth*cbJettonArea,0);
		}
	}
}

//����������ע
void CGameClientView::DrawAreaLimitChip(CDC * pDC)
{
	//CFont * pOldFont = pDC->SelectObject(&m_fontChipLimit);

	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();
	pDC->SetTextColor(RGB(255,255,255));
	__int64 lLimitChip=0;
	CString strMsg;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		lLimitChip = m_lAreaChipLimit[i];
		strMsg.Format(TEXT("%I64d/%I64d"),m_lAllAreaChip[i],lLimitChip);
		pDC->DrawText(strMsg,m_rcChipLimit[i],DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
	}
	//pDC->SelectObject(pOldFont);
}

//������Ϣ
void CGameClientView::SetMeMaxChip(__int64 iMeMaxChip)
{
	if (m_lMeMaxChip!=iMeMaxChip) m_lMeMaxChip=iMeMaxChip;
	return;
}

//�����ע
void CGameClientView::SetAreaLimitChip(__int64 *pLimitArray, int ArraySize)
{
	if( pLimitArray == NULL || ArraySize != JETTON_AREA_COUNT )
		return;

	for( int i=0; i<ArraySize; i++ )
		m_lAreaChipLimit[i] = pLimitArray[i];
	return;
}

//��ʷ��¼
void CGameClientView::SetGameHistory(tagGameRecord * pGameRecord, int nRecordCount)
{
	m_GameRecord.FillGameRecord(pGameRecord,nRecordCount);
	return;
}

//�������
void CGameClientView::CleanUserJetton()
{
	//��ע��Ϣ
	ZeroMemory(m_lMeAreaChip, sizeof(m_lMeAreaChip));
	//ȫ����ע
	ZeroMemory(m_lAllAreaChip, sizeof(m_lAllAreaChip));
	m_strDispatchCardTips=TEXT("");
	//���½���
	InvalidGameView(0,0,0,0);
	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_APPLE);
	if (cbViewIndex>ID_APPLE) return;

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( i == cbViewIndex )
			m_lMeAreaChip[i] = lJettonCount;
	}

	//���½���
	InvalidGameView(0,0,0,0);
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, __int64 iChipCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_APPLE);
	if (cbViewIndex>ID_APPLE)
		return;

	//��������
	bool bPlaceJetton=false;

	for(int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
		{
			m_lAllAreaChip[i] += iChipCount;
			//m_lAreaChipLimit[i]-=iChipCount;//ע�͵�������ע������ʾ��ʱ��˥��
		}
	}

	//���½���
	if (bPlaceJetton==true) 
		InvalidGameView(0,0,0,0);

	return;
}

//���ֳɼ�
void CGameClientView::SetCurGameChip(__int64 lMeCurGameChip, __int64 lMeCurGameReturnChip, __int64 lBankerCurGameChip, __int64 lGameRevenue)
{
	m_lMeCurGameChip=lMeCurGameChip;
	m_lMeCurGameReturnChip=lMeCurGameReturnChip;
	m_lBankerCurGameChip=lBankerCurGameChip;
	m_lGameRevenue=lGameRevenue;
}

//����ʤ��
void CGameClientView::SetWinnerSide(BYTE cbWinnerSide)
{
	//���ñ���
	m_cbWinnerSide=cbWinnerSide;
	m_cbAreaFlash=cbWinnerSide;

	//����ʱ��
	if (cbWinnerSide!=0xFF)
	{
		SetTimer(IDI_FLASH_WINNER,300,NULL);
		SetTimer(IDI_TIP_NORMAL,200,NULL);
	}
	else 
	{
		KillTimer(IDI_FLASH_WINNER);
		KillTimer(IDI_TIP_NORMAL);
		m_nCurCartoonIndex = m_cbWinArea;
	}

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( m_rcJettonArea[i].PtInRect(MousePoint))
			return i;
	}
	return 0xFF;
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, __int64 iNumber, INT nXPos, INT nYPos, bool bYellow)
{
	CSize SizeChipNumber(m_ImageChipNumber.GetWidth()/10,m_ImageChipNumber.GetHeight());

	//������Ŀ
	int iNumberCount=0;
	__int64 iNumberTemp=iNumber;
	do
	{
		iNumberCount++;
		iNumberTemp/=10;
	} while (iNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-SizeChipNumber.cy/2;
	INT nXDrawPos=nXPos-10;//+iNumberCount*SizeChipNumber.cx/2-SizeChipNumber.cx;
	//INT nXDrawPos=nXPos+iNumberCount*SizeChipNumber.cx/2-SizeChipNumber.cx;

	for (int i=0; i<iNumberCount; i++)
	{
		//�滭����
		int iCellNumber=(int)(iNumber%10);
		if (bYellow)
		{
			m_ImageBigSmallNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeChipNumber.cx,SizeChipNumber.cy,
				iCellNumber*SizeChipNumber.cx,0,255/*RGB(255,0,255)*/);
		}
		else
		{
			m_ImageChipNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeChipNumber.cx,SizeChipNumber.cy,
				iCellNumber*SizeChipNumber.cx,0,255/*RGB(255,0,255)*/);
		}
		iNumber/=10;
		nXDrawPos-=SizeChipNumber.cx;
	};

	return;
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, __int64 iNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strNumber.Format(TEXT("%I64d"),iNumber>0?iNumber:-iNumber);
	int p=strNumber.GetLength()-3;
	while(p>0)
	{
		strNumber.Insert(p,TEXT(","));
		p-=3;
	}
	if(iNumber<0)
		strNumber=TEXT("-")+strNumber;
	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, __int64 iNumber, CRect rcRect, INT nFormat)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;

	strNumber.Format(TEXT("%I64d"),iNumber>0?iNumber:-iNumber);
	int p=strNumber.GetLength()-3;
	while(p>0)
	{
		strNumber.Insert(p,TEXT(","));
		p-=3;
	}
	if(iNumber<0)
		strNumber=TEXT("-")+strNumber;

	//�������
	if (nFormat==-1) 
		pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
	else 
		pDC->DrawText(strNumber,rcRect,nFormat);
}

//��Ͷ��ť
void CGameClientView::OnAddStep1()
{
	//���ñ���
	m_lCurrentJetton=1L;
	InvalidGameView(0,0,0,0);

	//m_btAddStep1_H.ShowWindow(SW_SHOW);
	//m_btAddStep1_H.EnableWindow(true);
	m_btAddStep1.ShowWindow(SW_HIDE);

	m_btAddStep10.ShowWindow(SW_SHOW);
	m_btAddStep10.EnableWindow(true);
	//m_btAddStep10_H.ShowWindow(SW_HIDE);

	m_btAddStep100.ShowWindow(SW_SHOW);
	m_btAddStep100.EnableWindow(true);
	//m_btAddStep100_H.ShowWindow(SW_HIDE);
	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep10()
{
	//���ñ���
	m_lCurrentJetton=10L;
	InvalidGameView(0,0,0,0);

	m_btAddStep1.ShowWindow(SW_SHOW);
	m_btAddStep1.EnableWindow(true);
	//m_btAddStep1_H.ShowWindow(SW_HIDE);

	//m_btAddStep10_H.ShowWindow(SW_SHOW);
	//m_btAddStep10_H.EnableWindow(true);
	m_btAddStep10.ShowWindow(SW_HIDE);

	m_btAddStep100.ShowWindow(SW_SHOW);
	m_btAddStep100.EnableWindow(true);
	//m_btAddStep100_H.ShowWindow(SW_HIDE);

	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep100()
{
	//���ñ���
	m_lCurrentJetton=100L;
	InvalidGameView(0,0,0,0);

	m_btAddStep1.ShowWindow(SW_SHOW);
	m_btAddStep1.EnableWindow(true);
	//m_btAddStep1_H.ShowWindow(SW_HIDE);

	m_btAddStep10.ShowWindow(SW_SHOW);
	m_btAddStep10.EnableWindow(true);
	//m_btAddStep10_H.ShowWindow(SW_HIDE);

	//m_btAddStep100_H.ShowWindow(SW_SHOW);
	//m_btAddStep100_H.EnableWindow(true);
	m_btAddStep100.ShowWindow(SW_HIDE);

	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep1_H()
{
	//���ñ���
	m_lCurrentJetton=1L;

	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep10_H()
{
	//���ñ���
	m_lCurrentJetton=10L;

	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep100_H()
{
	//���ñ���
	m_lCurrentJetton=100L;
	return;
}

//����������ʱ��ʱ�������飬������׶������ȼ�
void CGameClientView::UpdateCartoonTimeElapse(BYTE cbStopStep)
{
	m_bGoodLuckTypeTip=false;//��ͨ���򿪽�
	m_cbWinArea=cbStopStep;//��¼�±�����ת����ͣ����λ��
	ZeroMemory(m_CartoonTimeElapseArray, sizeof(m_CartoonTimeElapseArray));
	//m_nCurCartoonIndex = 0;			//���³�ʼ����ͨ��ͼλ�������±�
	m_CartoonElapsIndex = 0;		//���³�ʼ����ͨ��ͼ��ʱ�������±�
	int nTimeElaps0=800;			//��0�͵ڰ˽׶�
	int nTimeElaps1=400;			//��һ�͵��߽׶�
	int nTimeElaps2=200;			//��һ�͵���׶�
	int nTimeElaps3=80;				//�ڶ��͵��Ľ׶�
	int nTimeElaps4=50;				//�����׶Σ�ÿ����ʱ������Ϊ500����

	//int nAllStep = 64+cbStopStep;	//�ܹ���ת��ô���ʱ������
	//int nAllStep = 48+cbStopStep;	//�ܹ���ת��ô���ʱ������
	//////////
	//int nAllStep = 48+(cbStopStep+m_nCurCartoonIndex)%24;	//�ܹ���ת��ô���ʱ������
	//int nAllStep = (48+cbStopStep-m_nCurCartoonIndex+72)%72;	//�ܹ���ת��ô���ʱ������
	int nAllStep = 96+(24+cbStopStep-m_nCurCartoonIndex)%24;	//�ܹ���ת��ô���ʱ������
	//////////
	int n1 = 2;						//��һ�׶�5��ʱ������
	int n2 = 3;						//�ڶ��׶�4��ʱ������
	int n3 = 4;
	int n4 = 5;
	int n6 = 5;
	int n7 = 4;
	int n8 = 3;
	int n9 = 2;
	int n5 = nAllStep-n1-n2-n3-n4-n6-n7-n8-n9;		//����׶���ת5��ʱ������
//printf("m_nCurCartoonIndex=%d\n",m_nCurCartoonIndex);
//printf("nAllStep=%d\n",nAllStep);

	//��һ�׶�ʱ��ֵ(0-5)
	for( int i=0; i<n1; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps0;

	//�ڶ��׶�ʱ��ֵ
	for(int i=n1; i<n1+n2; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps1;

	//�����׶�ʱ��ֵ
	for( int i=n1+n2; i<n1+n2+n3; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps2;

	//���Ľ׶�ʱ��ֵ
	for( int i=n1+n2+n3; i<n1+n2+n3+n4; i++ )
		m_CartoonTimeElapseArray[i] = nTimeElaps3;

	//����׶�ʱ��ֵ
	for( int i=n1+n2+n3+n4; i<n1+n2+n3+n4+n5; i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps4;
	
	//�����׶�
	for( int i=n1+n2+n3+n4+n5;i<n1+n2+n3+n4+n5+n6;i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps3;

	//���߽׶�
	for( int i=n1+n2+n3+n4+n5+n6;i<n1+n2+n3+n4+n5+n6+n7;i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps2;

	for( int i=n1+n2+n3+n4+n5+n6+n7;i<n1+n2+n3+n4+n5+n6+n7+n8;i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps1;

	for( int i=n1+n2+n3+n4+n5+n6+n7+n8;i<=n1+n2+n3+n4+n5+n6+n7+n8+n9;i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps0;

	SetTimer(IDI_DISPATCH_CARD, m_CartoonTimeElapseArray[m_CartoonElapsIndex++], NULL);	//��ʼ�������ƶ���
	m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("TURN_START"));
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_WAIT_5)
	{
		KillTimer(IDI_WAIT_5);
		m_bGoodLuckTypeTip=false;
		m_bBigSmallTime=true;
		EndCartoonEffect();
		return;
	}

	if (nIDEvent==IDI_ACTIVE_BORDER)
	{
		m_cbActiveBorderFrameCount=(m_cbActiveBorderFrameCount+4-1)%4;
		InvalidGameView(m_rectActiveBorderUpdate.left,m_rectActiveBorderUpdate.top,m_rectActiveBorderUpdate.right,m_rectActiveBorderUpdate.bottom);
		return;
	}

	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		if (m_cbAreaFlash!=m_cbWinnerSide)
		{
			m_cbAreaFlash=m_cbWinnerSide;
		}
		else 
		{
			m_cbAreaFlash=0xFF;
		}

		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}

	if (nIDEvent==IDI_TIP_NORMAL)
	{
		m_cbEndCount++;
		if (!m_bAlreadyGuess && m_lMeCurGameChip>0)
			m_cbGameEndTime=rand()%14+1;
//printf("m_cbEndCount=%d\n",m_cbEndCount);
		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}

	if (nIDEvent==IDI_TIP_GOOD_LUCK)
	{
		m_cbGoodLuckTipCount++;
		InvalidGameView(0,0,0,0);
		if (!m_bGoodLuckTypeTip)
		{
			m_cbGoodLuckTipCount=0;
			KillTimer(IDI_TIP_GOOD_LUCK);
		}
		return;
	}

	//�ֻ�ׯ��
	if (nIDEvent==IDI_SHOW_CHANGE_BANKER)
	{
		ShowChangeBanker(false);
		return;
	}

	if (nIDEvent==IDI_DISPATCH_CARD)//���Ƽ�ʱ�����ڴ˸���������ʱ��
	{
		KillTimer(IDI_DISPATCH_CARD);//��ɱ����һ����ʱ��
		PlayingCartoonEffect();
		return;
	}

	//////////
	if (nIDEvent==IDI_DISPATCH_CARD2)
	{
		KillTimer(IDI_DISPATCH_CARD2);
		PlayingCartoonEffect2();
		return;
	}
	//////////

	//////////
	if (nIDEvent==IDI_DISPATCH_CARD3)
	{
		KillTimer(IDI_DISPATCH_CARD3);
		PlayingCartoonEffect3();
		return;
	}
	//////////

	if (nIDEvent==IDI_ADD_CONTINUE)
	{
		KillTimer(IDI_ADD_CONTINUE);
		if (m_bisLButtonDown) SetTimer(IDI_ADD_CONTINUE,30,NULL);
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);
		__int64 lJettonScore=m_lCurrentJetton;
		CString str;
		str.Format(L"sg ��ʱ��%d",cbJettonArea);
		OutputDebugString(str);
		m_pGameClientDlg->OnPlaceJetton(cbJettonArea,lJettonScore,false);
		return;
	}

	if (nIDEvent==IDI_PAO_HUO_CHE_HIT)
	{
		KillTimer(IDI_PAO_HUO_CHE_HIT);
		if (m_cbFlashAndHitCount<m_cbPaoHuoCheCount)
		{
//			m_cbAreaLight[m_cbPaoHuoCheArea[m_cbFlashAndHitCount]]=2;//Ҫ��m_cbFlashAndHitCount++;֮ǰ
//printf("m_cbPaoHuoCheArea[m_cbFlashAndHitCount]=%d\n",m_cbPaoHuoCheArea[m_cbFlashAndHitCount]);
			m_cbAreaLight[(m_cbWinArea+1+m_cbFlashAndHitCount)%24]=2;
			m_cbPaoHuoCheWinIndex=m_cbWinArea;
			m_cbPaoHuoCheWinCount=0;
			InvalidGameView(0,0,0,0);
			m_cbFlashAndHitCount++;
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("LUCKY_HIT"));
			SetTimer(IDI_PAO_HUO_CHE_HIT,200,NULL);
		}
		else
		{
			m_cbFlashAndHitCount=0;
			UpdateCartoonTimeElapse3(m_cbPaoHuoCheArea[m_cbPaoHuoCheIndex++]);
			//m_bBigSmallTime=true;
			//EndCartoonEffect();
		}
		return;
	}

	if (nIDEvent==IDI_FLASH_AND_HIT)
	{
		KillTimer(IDI_FLASH_AND_HIT);
		if (m_cbFlashAndHitCount<6)
		{
			m_cbFlashAndHitCount++;
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("LUCKY_FLASH"));
			SetTimer(IDI_FLASH_AND_HIT,200,NULL);
		}
		else
		{
			m_cbFlashAndHitCount=0;
			//////////
			//�ܻ𳵷�֧
			if (m_cbGoodLuckType==4)
			{
				m_bGoodLuckTypeTip=true;
				SetTimer(IDI_TIP_GOOD_LUCK,200,NULL);

				SetTimer(IDI_PAO_HUO_CHE_HIT,200,NULL);
				return;
			}
			//////////
			if (m_cbPaoHuoCheCount==77)//�Է�
			{
				m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_DU_LOSE"));
				m_bBigSmallTime=true;
				EndCartoonEffect();
				return;
			}
			else
			{
				UpdateCartoonTimeElapse2(m_cbPaoHuoCheArea[m_cbPaoHuoCheIndex++]);
			}
		}
		return;
	}

	if (nIDEvent==IDI_GOOD_LUCK_FLASH)
	{
		m_cbGoodLuckFlashCount++;
		InvalidGameView(0,0,0,0);
		return;
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ת
void CGameClientView::PlayingCartoonEffect()
{
//printf("m_cbPaoHuoCheIndex=%d\n",m_cbPaoHuoCheIndex);
//printf("m_cbPaoHuoCheCount=%d\n",m_cbPaoHuoCheCount);
//printf("m_CartoonElapsIndex=%d\n",m_CartoonElapsIndex);
	InvalidGameView(0,0,0,0);//ˢ�½��棬��ʹ��һ��ͼ������
	if (m_CartoonElapsIndex<CountArray(m_CartoonTimeElapseArray))
	{
		if (m_CartoonTimeElapseArray[m_CartoonElapsIndex]>0)//��Ч��ʱ��ֵ
		{
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
			m_nCurCartoonIndex=(m_nCurCartoonIndex+1)%24;
			SetTimer(IDI_DISPATCH_CARD,m_CartoonTimeElapseArray[m_CartoonElapsIndex++],NULL);
		}
		else if (m_cbPaoHuoCheCount>0)
		{
			m_cbAreaLight[m_cbWinArea]=1;//�����н����򣬱�����UpdateCartoonTimeElapse2()����֮ǰ
			SetTimer(IDI_FLASH_AND_HIT,1,NULL);
			//m_bAreaLight[m_cbWinArea]=true;//�����н����򣬱�����UpdateCartoonTimeElapse2()����֮ǰ
			//UpdateCartoonTimeElapse2(m_cbPaoHuoCheArea[m_cbPaoHuoCheIndex++]);
		}
		else//��ת�������ô�����㲿�ֵ��߼���
		{
			m_bBigSmallTime=true;
			EndCartoonEffect();
		}
	}
}

//����GoodLuckʱ���ŵĶ���
void CGameClientView::SetGoodLuckCartoon(BYTE cbPaoHuoCheArea[], BYTE cbPaoHuoCheCount, BYTE cbWinArea, BYTE cbGoodLuckType)
{
	CopyMemory(m_cbPaoHuoCheArea,cbPaoHuoCheArea,sizeof(m_cbPaoHuoCheArea));
	m_cbPaoHuoCheCount=cbPaoHuoCheCount;
	m_cbGoodLuckType=cbGoodLuckType;
	if (m_cbGoodLuckType>0 && m_cbGoodLuckType<5)
	{
		//////////
		//LPCTSTR pszSound[]={TEXT("SND_BACK0"),TEXT("SND_BACK1"),TEXT("SND_BACK2")};
		//m_pGameClientDlg->StopSound();
		//m_pGameClientDlg->PlayBackGroundSound(AfxGetInstanceHandle(),pszSound[rand()%3]);
		m_pGameClientDlg->OnGameSound(2,0);
		//////////
		m_bGoodLuckFlash=true;
		SetTimer(IDI_GOOD_LUCK_FLASH,400,NULL);
		InvalidGameView(0,0,0,0);
	}
	m_cbPaoHuoCheIndex=0;
	ASSERT(m_cbPaoHuoCheCount>=1);
	UpdateCartoonTimeElapse(cbWinArea);
	//UpdateCartoonTimeElapse2(m_cbPaoHuoCheArea[m_cbPaoHuoCheIndex++]);
}

//��ʱ��ת��ʹ��
void CGameClientView::UpdateCartoonTimeElapse2(BYTE cbWinArea)
{
//printf(__FUNCTION__"\n");
	m_bGoodLuckTypeTip=true;//GoodLuck���򿪽�
	SetTimer(IDI_TIP_GOOD_LUCK,200,NULL);

	int nAllStep=0;
	if (m_cbGoodLuckType!=5)
		nAllStep=(24+m_nCurCartoonIndex-cbWinArea+1)%24;//�ܼ�ʱ������
	else
		nAllStep=24+(24+m_nCurCartoonIndex-cbWinArea+1)%24;//�ܼ�ʱ������

	ZeroMemory(m_CartoonTimeElapseArray,sizeof(m_CartoonTimeElapseArray));
	m_cbWinArea=cbWinArea;//��¼�±�����ת����ͣ����λ��
	m_CartoonElapsIndex=0;//��0
	for (int i=0; i<nAllStep; i++) m_CartoonTimeElapseArray[i]=30;//50;//�ԡ����١�ת��
	SetTimer(IDI_DISPATCH_CARD2,m_CartoonTimeElapseArray[m_CartoonElapsIndex++],NULL);//��ʼ�������ƶ���
}

//��ʱ��ת��ʹ��
void CGameClientView::PlayingCartoonEffect2()
{
	InvalidGameView(0,0,0,0);//ˢ�½��棬��ʹ��һ��ͼ������
	if (m_CartoonElapsIndex<CountArray(m_CartoonTimeElapseArray))
	{
		if (m_CartoonTimeElapseArray[m_CartoonElapsIndex]>0)//��Ч��ʱ��ֵ
		{
			//m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
			m_nCurCartoonIndex=(m_nCurCartoonIndex+24-1)%24;
			SetTimer(IDI_DISPATCH_CARD2,m_CartoonTimeElapseArray[m_CartoonElapsIndex++],NULL);
		}
		else if (m_cbPaoHuoCheIndex<m_cbPaoHuoCheCount)
		{
			if (m_cbPaoHuoCheIndex>=1) m_cbAreaLight[m_cbPaoHuoCheArea[m_cbPaoHuoCheIndex-1]]=2;//�����н�����
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("LUCKY_HIT"));
			SetTimer(IDI_FLASH_AND_HIT,1,NULL);
			//UpdateCartoonTimeElapse2(m_cbPaoHuoCheArea[m_cbPaoHuoCheIndex++]);
		}
		else//��ת�������ô�����㲿�ֵ��߼���
		{
			if (m_cbPaoHuoCheIndex>=1) m_cbAreaLight[m_cbPaoHuoCheArea[m_cbPaoHuoCheIndex-1]]=2;//�����н�����
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("LUCKY_HIT"));
			//m_bBigSmallTime=true;
			//EndCartoonEffect();
			if (m_cbGoodLuckType!=5)
			{
				m_bGoodLuckFlash=false;
				SetTimer(IDI_WAIT_5,5000,NULL);
			}
			else
			{
				m_bBigSmallTime=true;
				EndCartoonEffect();
			}
		}
	}
}

//��ʱ��ת��ʹ��
void CGameClientView::UpdateCartoonTimeElapse3(BYTE cbWinArea)
{
	int nAllStep=48+48+(24+m_nCurCartoonIndex-cbWinArea+2)%24;//�ܼ�ʱ������
	ZeroMemory(m_CartoonTimeElapseArray,sizeof(m_CartoonTimeElapseArray));
	m_cbWinArea=cbWinArea;//��¼�±�����ת����ͣ����λ��
	m_CartoonElapsIndex=0;//��0
	for (int i=0; i<nAllStep; i++) m_CartoonTimeElapseArray[i]=51;//50;//�ԡ����١�ת��
	SetTimer(IDI_DISPATCH_CARD3,m_CartoonTimeElapseArray[m_CartoonElapsIndex++],NULL);//��ʼ�������ƶ���
}

//��ʱ��ת��ʹ��
void CGameClientView::PlayingCartoonEffect3()
{
	InvalidGameView(0,0,0,0);//ˢ�½��棬��ʹ��һ��ͼ������
	if (m_CartoonElapsIndex<CountArray(m_CartoonTimeElapseArray))
	{
		if (m_CartoonTimeElapseArray[m_CartoonElapsIndex]>0)//��Ч��ʱ��ֵ
		{
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
			ZeroMemory(m_cbAreaLight,sizeof(m_cbAreaLight));
			m_cbPaoHuoCheWinCount=(m_cbPaoHuoCheWinCount+24-1)%24;
			for (int i=0; i<m_cbPaoHuoCheCount; i++)
				m_cbAreaLight[(m_cbPaoHuoCheWinIndex+1+m_cbPaoHuoCheWinCount+i)%24]=2;
			SetTimer(IDI_DISPATCH_CARD3,m_CartoonTimeElapseArray[m_CartoonElapsIndex++],NULL);
		}
		else//��ת�������ô�����㲿�ֵ��߼���
		{
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("LUCKY_HIT"));
			//m_bBigSmallTime=true;
			//EndCartoonEffect();
			m_bGoodLuckFlash=false;
			SetTimer(IDI_WAIT_5,5000,NULL);
		}
	}
}

//��������߿�
void CGameClientView::DrawAreaLight(CDC * pDC)
{
	int iWidth=m_ImageCardCartoon.GetWidth();
	int iHeight=m_ImageCardCartoon.GetHeight();
	for (int i=0; i<24; i++)
	{
		if (m_cbAreaLight[i]==1)
			m_ImageCardCartoon.DrawImage(pDC,
			m_PtCardArray[i].x-iWidth/2,
			m_PtCardArray[i].y-iHeight/2,
			iWidth,iHeight,0,0);
		if (m_cbAreaLight[i]==2)
			m_ImageCardCartoon2.DrawImage(pDC,
			m_PtCardArray[i].x-iWidth/2,
			m_PtCardArray[i].y-iHeight/2,
			iWidth,iHeight,0,0);
	}
}

void CGameClientView::EndCartoonEffect()
{
	//����Ӯ��
	SetWinnerSide(m_cbWinArea%JETTON_AREA_COUNT);

	m_btBankButton.ShowWindow(SW_HIDE);

	if (m_cbPaoHuoCheCount>0)
	{
		//if (m_cbGoodLuckType==0)
		//	m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_DU_LOSE"));
	}
	else
	{
		BYTE cbSoundIndex=0;
		switch (m_cbWinArea)
		{
		case 0:
		case 12:
		case 11:
			{
				cbSoundIndex=6;
				break;
			}
		case 1:
		case 13:
		case 23:
			{
				cbSoundIndex=4;
				break;
			}
		case 2:
		case 3:
			{
				cbSoundIndex=0;
				break;
			}
		case 4:
		case 10:
		case 16:
		case 22:
		case 5:
			{
				cbSoundIndex=7;
				break;
			}
		case 6:
		case 18:
		case 17:
			{
				cbSoundIndex=5;
				break;
			}
		case 7:
		case 8:
			{
				cbSoundIndex=3;
				break;
			}
		case 9:
		case 21:
			{
				break;
			}
		case 14:
		case 15:
			{
				cbSoundIndex=1;
				break;
			}
		case 19:
		case 20:
			{
				cbSoundIndex=2;
				break;
			}
		}
		LPCTSTR pszSound[]={TEXT("SND_AREA0"),TEXT("SND_AREA1"),TEXT("SND_AREA2"),TEXT("SND_AREA3")
			,TEXT("SND_AREA4"),TEXT("SND_AREA5"),TEXT("SND_AREA6"),TEXT("SND_AREA7")};

		m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),pszSound[cbSoundIndex]);
	}
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{

	if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
	{
		//��ȡ��ע����
		BYTE cbJettonArea=GetJettonArea(Point);

		if (cbJettonArea!=0xFF)
		{
			if (m_pGameClientDlg!=NULL)
			{
				//////////
				//��ǰ���벻�㣻����ʱ����ť��������
				if (m_pGameClientDlg->m_lMyCurrentChip<=0) return;
				__int64 lMyTotalJetton=0;
				for (int i=0; i<JETTON_AREA_COUNT; i++)
					lMyTotalJetton+=m_lMeAreaChip[i];
				if (m_pGameClientDlg->m_lMyCurrentChip-lMyTotalJetton<=0) return;
				//////////
				//////////
				//�������Ʋ��㣬���Զ���С
				__int64 lJettonScore=m_lCurrentJetton;
				if ((m_lAreaChipLimit[cbJettonArea]-m_lAllAreaChip[cbJettonArea])<lJettonScore && lJettonScore==100)
					lJettonScore=lJettonScore/10;
				if ((m_lAreaChipLimit[cbJettonArea]-m_lAllAreaChip[cbJettonArea])<lJettonScore && lJettonScore==10)
					lJettonScore=lJettonScore/10;
				//////////
				m_bisLButtonDown=true;
				InvalidGameView(0,0,0,0);

				CString str;
				str.Format(L"sg �����Ϣ1111����%d",cbJettonArea);
				OutputDebugString(str);

				m_pGameClientDlg->OnPlaceJetton(cbJettonArea,lJettonScore,false);
				SetTimer(IDI_ADD_CONTINUE,500,NULL);
			}
		}
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	m_bisLButtonDown=false;
	KillTimer(IDI_ADD_CONTINUE);
	InvalidGameView(0,0,0,0);
	__super::OnLButtonDown(nFlags,Point);
}

//��ȡ��ǰ��һ�����ע������
__int64 CGameClientView::GetUserCurrentLeaveChip()
{
	__int64 iTemp = 0;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		iTemp += m_lMeAreaChip[i];
	__int64 iLeaveChip = m_lMeMaxChip - iTemp;
	return  __max(iLeaveChip, 0);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ��ע����
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);
	BYTE cbJettonArea=GetJettonArea(MousePoint);

	if (cbJettonArea==0xFF)
	{
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		return TRUE;
	}

	if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
	{
		switch (m_lCurrentJetton)
		{
		case 1:
			{
				SetCursor(m_MyCursor[0]);
				return TRUE;
			}
		case 10:
			{
				SetCursor(m_MyCursor[1]);
				return TRUE;
			}
		case 100:
			{
				SetCursor(m_MyCursor[2]);
				return TRUE;
			}
		}
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
	InvalidGameView(0,0,0,0);
}

//��ׯ��ť
void CGameClientView::OnApplyBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,1,0);
	return;
}

//��ׯ��ť
void CGameClientView::OnCancelBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,0,0);
	return;
}

//��С��ť
void CGameClientView::OnBetSmall()
{
	if (m_lMeCurGameChip<=0) return;
	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END && m_bBigSmallTime/* && !m_bAlreadyGuess*/)
	{
		m_bAlreadyGuess=true;
		SendEngineMessage(IDM_BIG_SMALL,0,0);
	}
	return;
}

//�´�ť
void CGameClientView::OnBetBig()
{
	if (m_lMeCurGameChip<=0) return;
	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END && m_bBigSmallTime/* && !m_bAlreadyGuess*/)
	{
		m_bAlreadyGuess=true;
		SendEngineMessage(IDM_BIG_SMALL,1,1);
	}
	return;
}

//��Ѻ��ť
void CGameClientView::OnXuYa()
{
	SendEngineMessage(IDM_XU_YA,0,0);
	return;
}

//��Ѻ��ť
void CGameClientView::OnXuYa2()
{
	m_btBetLast2.ShowWindow(SW_HIDE);
	m_btBetLast.ShowWindow(SW_SHOW);
	g_pGameClientDlg->m_bXuYaContinue=false;
	g_pGameClientDlg->m_cbXuYaContinue=0;
	return;
}

//����б�
void CGameClientView::OnPlayerList()
{
	m_bShowPlayerList=true;
	m_btPlayerList.ShowWindow(SW_HIDE);
	m_btApplyList.ShowWindow(SW_SHOW);
	m_btPlayerList_H.ShowWindow(SW_SHOW);
	m_btApplyList_H.ShowWindow(SW_HIDE);
	//�л��б����ʾ
	m_PlayerList.ShowWindow(SW_SHOW);
	m_ApplyList.ShowWindow(SW_HIDE);
}

//��ׯ�б�
void CGameClientView::OnApplyList()
{
	m_bShowPlayerList=false;
	m_btPlayerList.ShowWindow(SW_SHOW);
	m_btApplyList.ShowWindow(SW_HIDE);
	m_btPlayerList_H.ShowWindow(SW_HIDE);
	m_btApplyList_H.ShowWindow(SW_SHOW);
	//�л��б����ʾ
	m_PlayerList.ShowWindow(SW_HIDE);
	m_ApplyList.ShowWindow(SW_SHOW);
}

//ׯ����Ϣ
void CGameClientView::SetBankerInfo(WORD wBankerUser, __int64 iBankerChip) 
{
	//�л��ж�
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinChip=0L;
	}
	m_lBankerChip=iBankerChip;
}

//�ƶ���ť
void CGameClientView::OnChipMoveL()
{
	int n = m_GameRecord.ScrollLeft();
	if( n == -1 )
		m_btChipMoveL.EnableWindow(FALSE);
	if( !m_btChipMoveR.IsWindowEnabled())
		m_btChipMoveR.EnableWindow(TRUE);
	//���½���
	InvalidGameView(0,0,0,0);
}

//�ƶ���ť
void CGameClientView::OnChipMoveR()
{
	int n = m_GameRecord.ScrollRight();
	if( n == -1 )
		m_btChipMoveR.EnableWindow(FALSE);
	if( !m_btChipMoveL.IsWindowEnabled())
		m_btChipMoveL.EnableWindow(TRUE);
	//���½���
	InvalidGameView(0,0,0,0);
}

void CGameClientView::OnClickedBank()
{
	m_ExchangeDlg.DoModal();
}

//�����ҵķ���
void CGameClientView::DrawMyChip(CDC *pDC, int nWidth, int nHeight)
{
	int nImageWidth = m_ImageBetNumber.GetWidth()/10;
	int nImageHeight = m_ImageBetNumber.GetHeight();
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		IClientUserItem *pUserItem=GetClientUserItem(m_wMeChairID);
		tagUserInfo *pMeUserData=NULL;
		if (pUserItem!=NULL) pMeUserData=pUserItem->GetUserInfo();
		if (pMeUserData==NULL) return;

		//////////
		LONGLONG lMyAllJetton=0;
		for (int i=0; i<JETTON_AREA_COUNT; i++) lMyAllJetton+=m_lMeAreaChip[i];
		//////////
//printf(__FUNCTION__"--lMyAllJetton=%I64d\n",lMyAllJetton);
//printf(__FUNCTION__"--m_lMyCurrentChip=%I64d\n",m_pGameClientDlg->m_lMyCurrentChip);
		//������Ŀ
		int iNumberCount=0;
		int nNUmBerChip=0;
#ifndef FUNC_CHIP
		LONGLONG lNumberTemp=m_pGameClientDlg->ScoreToChip(pMeUserData->lScore);
#else
		LONGLONG lNumberTemp=m_pGameClientDlg->m_lMyCurrentChip;
#endif

		//////////
		lNumberTemp-=lMyAllJetton;
		//////////

		do
		{
			iNumberCount++;
			lNumberTemp/=10;
		} while (lNumberTemp>0);
#ifndef FUNC_CHIP
		LONGLONG lUserChip=m_pGameClientDlg->ScoreToChip(pMeUserData->lScore);
#else
		LONGLONG lUserChip=m_pGameClientDlg->m_lMyCurrentChip;
#endif

		//////////
		lUserChip-=lMyAllJetton;
		//////////

		//λ�ö���
		INT nXDrawPos=nWidth/2 + 120;
		INT nYDrawPos=nHeight/2 - 330;

		for (LONG i=0;i<iNumberCount;i++)
		{
			int iCellNumber=(int)(lUserChip%10);

			m_ImageBetNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nImageWidth,nImageHeight,iCellNumber*nImageWidth,0);

			lUserChip/=10;
			nXDrawPos-=nImageWidth;
		}
	}
}

//���Ƶ�ǰ�ɼ�
void CGameClientView::DrawCurrentChip(CDC *pDC, int nWidth, int nHeight)
{
	int nImageWidth = m_ImageBetNumber.GetWidth()/10;
	int nImageHeight = m_ImageBetNumber.GetHeight();
	//������Ŀ
	int iNumberCount=0;
	int nNUmBerChip = 0;
	__int64 lNumberTemp=m_lMeCurGameChip;
	__int64 lUserChip=m_lMeCurGameChip;
//printf("m_lMeCurGameChip=%I64d\n",m_lMeCurGameChip);
//	//////////
//	LONGLONG lMeAllJetton=0;
	if (m_pGameClientDlg->GetGameStatus()==GAME_STATUS_FREE) m_lMeCurGameChip=0;
//	for (int i=0; i<JETTON_AREA_COUNT; i++) lMeAllJetton+=m_lMeAreaChip[i];
//	if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON) lMeAllJetton=0;
//	lNumberTemp+=lMeAllJetton;
//	lUserChip+=lMeAllJetton;
//printf("lMeAllJetton=%I64d\n",lMeAllJetton);
	//////////

	//if(m_lMeCurGameChip <= 0 )
	//{
	//	lNumberTemp = 0;
	//	lUserChip = 0;
	//}
	//else
	{
		do
		{
			iNumberCount++;
			lNumberTemp/=10;
		} while (lNumberTemp>0);
	}

	//λ�ö���
	INT nXDrawPos=nWidth/2 - 115;
	INT nYDrawPos=nHeight/2 - 330;

	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END && m_cbWinnerSide!=0xFF)
	{
		for (LONG i=0;i<iNumberCount;i++)
		{
			int iCellNumber=(int)(lUserChip%10);
			m_ImageBetNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nImageWidth,nImageHeight,iCellNumber*nImageWidth,0);
			lUserChip/=10;
			nXDrawPos-=nImageWidth;
		}
	}
	else//����ʱ�����0
	{
		m_ImageBetNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nImageWidth,nImageHeight,0,0);
	}
}

//�ɼ�����
void CGameClientView::SetGameChip(__int64 iMeCurGameChip, __int64 iMeCurGameReturnChip, __int64 iBankerCurGameChip)
{
	m_lMeCurGameChip=iMeCurGameChip;
	m_lMeCurGameReturnChip=iMeCurGameReturnChip;
	m_lBankerCurGameChip=iBankerCurGameChip;
}

//�滭����
void CGameClientView::DrawMyJettonNumber(CDC *pDC)
{
	for (int i=0; i<JETTON_AREA_COUNT; i++)
	{
		if (m_lMeAreaChip[i]>=0)
		{
			DrawNumberString(pDC,m_lMeAreaChip[i],m_PointJettonNumber[i].x,m_PointJettonNumber[i].y-45);
		}
	}
}

//��С��ʷ��¼
void CGameClientView::DrawMyBigSmallNumber(CDC *pDC)
{
	for (int i=0; i<JETTON_AREA_COUNT; i++)
	{
		if (m_cbMyBigSmallResult[i]>=0)
		{
			DrawNumberString(pDC,m_cbMyBigSmallResult[i],m_PointJettonNumber[i].x,m_PointJettonNumber[i].y-45,true);
		}
	}
}

//���ƶ������Լ�β��
void CGameClientView::DrawCardCartoon(CDC * pDC)
{
	//////////
	int nTailDir;//β�ͷ���
	CPngImage *pngCardCartoon=NULL;//ͼƬָ�룬˳ʱ��ʱΪ��ɫ����ʱ��ʱΪƷ��ɫ
	if (!m_bGoodLuckTypeTip)
	{
		nTailDir=-1;
		pngCardCartoon=&m_ImageCardCartoon;
	}
	else
	{
		nTailDir=1;
		//pngCardCartoon=&m_ImageCardCartoon2;
		pngCardCartoon=&m_ImageCardCartoon;
	}
	ASSERT(pngCardCartoon!=NULL);
	//////////

	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();
	if (m_nCurCartoonIndex<0 || m_nCurCartoonIndex>=24) return;

	int iWidth=pngCardCartoon->GetWidth();
	int iHeight=pngCardCartoon->GetHeight();
	pngCardCartoon->DrawImage(pDC,
		m_PtCardArray[m_nCurCartoonIndex].x-iWidth/2,
		m_PtCardArray[m_nCurCartoonIndex].y-iHeight/2,
		iWidth,iHeight,0,0);

	//����β��Ч��
	if (m_CartoonTimeElapseArray[m_CartoonElapsIndex]==50)
	{
		int iTailIndex=m_nCurCartoonIndex;

		iTailIndex=(iTailIndex+24+nTailDir)%24;
		pngCardCartoon->AlphaDrawImage(pDC,
			m_PtCardArray[iTailIndex].x-iWidth/2,
			m_PtCardArray[iTailIndex].y-iHeight/2,
			iWidth,iHeight,0,0,200);

		iTailIndex=(iTailIndex+24+nTailDir)%24;
		pngCardCartoon->AlphaDrawImage(pDC,
			m_PtCardArray[iTailIndex].x-iWidth/2,
			m_PtCardArray[iTailIndex].y-iHeight/2,
			iWidth,iHeight,0,0,150);

		iTailIndex=(iTailIndex+24+nTailDir)%24;
		pngCardCartoon->AlphaDrawImage(pDC,
			m_PtCardArray[iTailIndex].x-iWidth/2,
			m_PtCardArray[iTailIndex].y-iHeight/2,
			iWidth,iHeight,0,0,100);
	}
}

//���ƽ�������
void CGameClientView::DrawEndCartoon(CDC * pDC, int nWidth, int nHeight, BYTE cbWinArea)
{
	int nImageWidth = 0;
	int nImageHeight = 0;
	int iXPos=nWidth/2-nImageWidth/2-170;
	int iYPos=nHeight/2-100-110;

	if (m_cbPaoHuoCheCount>0)
	{
		if (m_cbGoodLuckType==0)//�Է�
		{
			iYPos-=10;
			iXPos-=17;
			nImageWidth=m_pngGoodLuckTip[0].GetWidth();
			nImageHeight=m_pngGoodLuckTip[0].GetHeight();
			m_pngGoodLuckTip[0].DrawImage(pDC,iXPos,iYPos,
				nImageWidth,nImageHeight,
				0,0);
		}
		return;
	}

	BYTE cbFrameIndex[7]={0,1,2,1,2,1,2};
	BYTE cbCurrentIndex=cbFrameIndex[m_cbEndCount%7];

	bool bLight=false;

	switch(cbWinArea)
	{
	case 0:
	case 12:
		{
			nImageWidth = m_ImageOrange.GetWidth()/3;
			nImageHeight = m_ImageOrange.GetHeight();
			m_ImageOrange.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth *  (cbCurrentIndex%3)  ),0);
			break;
		}
	case 1:
	case 13:
		{
			nImageWidth = m_ImageBell.GetWidth()/3;
			nImageHeight = m_ImageBell.GetHeight();
			m_ImageBell.DrawImage(pDC,iXPos+5,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 2:
		{
			bLight=true;
			nImageWidth = m_ImageBar2.GetWidth()/3;
			nImageHeight = m_ImageBar2.GetHeight();
			m_ImageBar2.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 3:
		{
			bLight=true;
			nImageWidth = m_ImageBar.GetWidth()/3;
			nImageHeight = m_ImageBar.GetHeight();
			m_ImageBar.DrawImage(pDC,iXPos+15,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 4:
	case 10:
	case 16:
	case 22:
		{
			nImageWidth = m_ImageApple.GetWidth()/3;
			nImageHeight = m_ImageApple.GetHeight();
			m_ImageApple.DrawImage(pDC,iXPos+15,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 5:
		{
			nImageWidth = m_ImageApple2.GetWidth()/3;
			nImageHeight = m_ImageApple2.GetHeight();
			m_ImageApple2.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 6:
	case 18:
		{
			nImageWidth = m_ImagePaw.GetWidth()/3;
			nImageHeight = m_ImagePaw.GetHeight();
			m_ImagePaw.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 7:
		{
			bLight=true;
			nImageWidth = m_ImageWaterMelon.GetWidth()/3;
			nImageHeight = m_ImageWaterMelon.GetHeight();
			m_ImageWaterMelon.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 8:
		{
			nImageWidth = m_ImageWaterMelon2.GetWidth()/3;
			nImageHeight = m_ImageWaterMelon2.GetHeight();
			m_ImageWaterMelon2.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 9:
	case 21:
		{
			break;
		}
	case 11:
		{
			nImageWidth = m_ImageOrange2.GetWidth()/3;
			nImageHeight = m_ImageOrange2.GetHeight();
			m_ImageOrange2.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 14:
		{
			nImageWidth = m_ImageSeven2.GetWidth()/3;
			nImageHeight = m_ImageSeven2.GetHeight();
			m_ImageSeven2.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 15:
		{
			bLight=true;
			nImageWidth = m_ImageSeven.GetWidth()/3;
			nImageHeight = m_ImageSeven.GetHeight();
			m_ImageSeven.DrawImage(pDC,iXPos+5,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 17:
		{
			nImageWidth = m_ImagePaw2.GetWidth()/3;
			nImageHeight = m_ImagePaw2.GetHeight();
			m_ImagePaw2.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 19:
		{
			bLight=true;
			nImageWidth = m_ImageStar.GetWidth()/3;
			nImageHeight = m_ImageStar.GetHeight();
			m_ImageStar.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 20:
		{
			nImageWidth = m_ImageStar2.GetWidth()/3;
			nImageHeight = m_ImageStar2.GetHeight();
			m_ImageStar2.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	case 23:
		{
			nImageWidth = m_ImageBell2.GetWidth()/3;
			nImageHeight = m_ImageBell2.GetHeight();
			m_ImageBell2.DrawImage(pDC,iXPos,iYPos,
				nImageWidth, nImageHeight,
				(nImageWidth * (cbCurrentIndex%3)  ),0);
			break;
		}
	default:
		{
			break;
		}
	}

	if (bLight && (m_cbEndCount%4==1 || m_cbEndCount%4==2))
	{
		m_pngLine.DrawImage(pDC,iXPos-283,iYPos-111,50,538,0,0);
		m_pngLine.DrawImage(pDC,iXPos+571,iYPos-111,50,538,0,0);
	}
}

//���ƽ�������
void CGameClientView::DrawGoodLuckTip(CDC * pDC, int nWidth, int nHeight, BYTE cbWinArea)
{
	int nImageWidth=0;
	int nImageHeight=0;
	int iXPos=nWidth/2-nImageWidth/2-170;
	int iYPos=nHeight/2-100-110;

	////////////////////////////////////////////////////////////////////////////////
	//����GoodLuck��
	if (m_cbPaoHuoCheCount>0)
	{
		BYTE cbCurrentIndex=0;
		BYTE cbPictureCount=1;
		if (m_cbGoodLuckType==0)//�Է�
		{
			cbPictureCount=1;
			iYPos-=10;
			iXPos-=17;
			cbCurrentIndex=0;
		}
		if (m_cbGoodLuckType>=1 && m_cbGoodLuckType<=3)
		{
			cbPictureCount=5;
			iXPos-=11+12;
			BYTE cbFrameIndex[7]={0,3,4,3,4,3,4};
			if (m_cbGoodLuckTipCount<4)
				cbCurrentIndex=0;
			else
				cbCurrentIndex=cbFrameIndex[(m_cbGoodLuckTipCount-4)%7];
		}
		if (m_cbGoodLuckType==4)
		{
			cbPictureCount=2;
			iXPos-=11;
			iYPos+=20;
			BYTE cbFrameIndex[2]={0,1};
			cbCurrentIndex=cbFrameIndex[m_cbGoodLuckTipCount%2];
		}
		if (m_cbGoodLuckType==5) return;
		//m_cbEndCount=m_cbEndCount%cbPictureCount;
		nImageWidth=m_pngGoodLuckTip[m_cbGoodLuckType].GetWidth()/cbPictureCount;
		nImageHeight=m_pngGoodLuckTip[m_cbGoodLuckType].GetHeight();
		m_pngGoodLuckTip[m_cbGoodLuckType].DrawImage(pDC,iXPos,iYPos,
			nImageWidth,nImageHeight,
			nImageWidth*(cbCurrentIndex%cbPictureCount),0);
		return;
	}
	////////////////////////////////////////////////////////////////////////////////
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//�����ж�
	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END )
	{
		if (m_nCurCartoonIndex!=0xFF && m_cbAreaFlash!=0xFF)
		{
			//������תֹͣ����
			//int nImageWidth = m_ImageCardCartoon.GetWidth();
			//int nImageHeight = m_ImageCardCartoon.GetHeight();
			//m_ImageCardCartoon.DrawImage(pDC, m_PtCardArray[m_nCurCartoonIndex].x-nImageWidth/2, 
			//	m_PtCardArray[m_nCurCartoonIndex].y-nImageHeight/2, nImageWidth, nImageHeight, 
			//	0, 0);
		}
	}
}

void CGameClientView::DrawHistoryRecord(CDC * pDC)
{
	m_GameRecord.ShowHistory(pDC,m_ImageHistoryRecord,m_pngRecordTip,m_rcHistoryRecord,MAX_SHOW_HISTORY);
}

//�滭ʱ��
void CGameClientView::DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//��ȡ����
	const INT nNumberHeight=m_ImageBetBigNumber.GetHeight();
	const INT nNumberWidth=m_ImageBetBigNumber.GetWidth()/10;

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
	INT nXDrawPos=nXPos-nNumberWidth;
	//INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		m_ImageBetBigNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

BOOL CGameClientView::PreTranslateMessage(MSG* pMsg)
{

	
	BYTE cbJettonArea=0xFF;
	if (pMsg->message==WM_KEYDOWN)
	{
		CString str;
		str.Format(L"sg 3333  %d, %d",tolower((BYTE)pMsg->wParam),tolower((BYTE)pMsg->lParam));
		OutputDebugString(str);
		/*if (tolower((BYTE)pMsg->wParam)<49||tolower((BYTE)pMsg->wParam)>57)
		{
			return false;
		}*/

		switch(tolower((BYTE)pMsg->wParam))
		{
		case '1':
		case VK_NUMPAD1:
			cbJettonArea=ID_BAR;
			break;
		case '2':
		case VK_NUMPAD2:
			cbJettonArea=ID_DOUBLE_SEVEN;
			break;
		case '3':
		case VK_NUMPAD3:
			cbJettonArea=ID_STAR;
			break;
		case '4':
		case VK_NUMPAD4:
			cbJettonArea=ID_WATERMELON;
			break;
		case '5':
		case VK_NUMPAD5:
			cbJettonArea=ID_BELL;
			break;
		case '6':
		case VK_NUMPAD6:
			cbJettonArea=ID_PAWPAW;
			break;
		case '7':
		case VK_NUMPAD7:
			cbJettonArea=ID_ORANGE;
			break;
		case '8':
		case VK_NUMPAD8:
			cbJettonArea=ID_APPLE;
			break;
		case VK_LEFT://��С
			OnBetSmall();
			break;
		case VK_RIGHT://�´�
			OnBetBig();
			break;
		case VK_UP://��Ѻ
			OnXuYa();
			break;
		}
		if (cbJettonArea!=0xFF)
		{
			//////////
			//�������Ʋ��㣬���Զ���С
			__int64 lJettonScore=m_lCurrentJetton;
			if ((m_lAreaChipLimit[cbJettonArea]-m_lAllAreaChip[cbJettonArea])<lJettonScore && lJettonScore==100)
				lJettonScore=lJettonScore/10;
			if ((m_lAreaChipLimit[cbJettonArea]-m_lAllAreaChip[cbJettonArea])<lJettonScore && lJettonScore==10)
				lJettonScore=lJettonScore/10;
			//////////
			/*CString str;
			str.Format(L"sg 0000PreTranslateMessage%d",cbJettonArea);
			OutputDebugString(str);*/
			m_pGameClientDlg->OnPlaceJetton(cbJettonArea,lJettonScore,false);
		}
	}
	return __super::PreTranslateMessage(pMsg);
}

//�����˺ſ���
void CGameClientView::OpenAdminWnd()
{
	tagUserAttribute * UserAttribute=m_pGameClientDlg->m_pIClientKernel->GetUserAttribute();
	ASSERT(UserAttribute!=NULL);
	if (UserAttribute->dwUserRight==1879048192)
	{
		if(m_AdminDlg.IsWindowVisible()) m_AdminDlg.ShowWindow(SW_HIDE);
		else m_AdminDlg.ShowWindow(SW_SHOW);
	}
}
//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nFlags)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
	CRect rcFrame;

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];
		pDC->DrawText(pszString,rcFrame,nFlags);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,rcRect,nFlags);

	return;
}

//������С��ʷ��¼
void CGameClientView::InsertBigSmallRecord(BYTE cbBigSmall)
{
	ASSERT(cbBigSmall>=1 && cbBigSmall<=14);
	if (cbBigSmall<1 || cbBigSmall>14) return;
	for (int i=0; i<JETTON_AREA_COUNT-1; i++)
	{
		m_cbMyBigSmallResult[i]=m_cbMyBigSmallResult[i+1];
	}
	m_cbMyBigSmallResult[7]=cbBigSmall;
	InvalidGameView(0,0,0,0);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMySkinButton, CSkinButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

CMySkinButton::CMySkinButton()
{
	m_nTime=0;
}

CMySkinButton::~CMySkinButton()
{
}

void CMySkinButton::OnLButtonDown(UINT nFlags, CPoint Point)
{
	OutputDebugStringA("CMySkinButton::OnLButtonDown");
	__super::OnLButtonDown(nFlags,Point);
	m_nTime=400;
	SetTimer(IDI_XU_YA_BUTTON,400,NULL);
}

void CMySkinButton::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
	KillTimer(IDI_XU_YA_BUTTON);
	m_nTime=0;
}

void CMySkinButton::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_XU_YA_BUTTON)
	{
		if (m_nTime>=2000)
		{
			KillTimer(IDI_XU_YA_BUTTON);
			m_nTime=0;
			g_pGameClientDlg->OnXuYaContinue();
		}
		else
		{
			m_nTime+=400;
			SetTimer(IDI_XU_YA_BUTTON,400,NULL);
		}
		return;
	}
	__super::OnTimer(nIDEvent);
}
