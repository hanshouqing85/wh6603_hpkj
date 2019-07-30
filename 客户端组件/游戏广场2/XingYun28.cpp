// BeiJingKuai8.cpp : 实现文件
//

#include "stdafx.h"
#include "GamePlaza.h"
#include "XingYun28.h"
#include "DlgFandian.h"
#include "MessageDlg.h"
#include "DlgAddtional.h"
#include "PlatformFrame.h"
#include "GDIpng/MemDC.h"
#include <map>

using		namespace		std;


extern map <int,CString> mapFandian;
extern map <int,float> mapPercent;
extern map <int,float> mapDanweiFandian;
extern map <int,float> mapBonus;


extern vector<wstring>	vecData1, vecData2, vecData3;


#define      MAXN			100
extern vector<wstring> vecAllNums;
CString   CXingYun28::m_strHaoma = _T("");


extern int g_arr[MAXN];
extern int g_nPos[512000][5];
extern int g_Counts;


//大开奖号
static const int kj_big_haoma_x = 220+5;
static const int kj_big_haoma_y = 28-4+2;

static const int czh_rc_left = 207;
static const int czh_rc_top = 4;
static const int czh_rc_width = 365;
static const int czh_rc_height = 16;
static CRect rcCzh(czh_rc_left, czh_rc_top, czh_rc_left + czh_rc_width, czh_rc_top + czh_rc_height);

static const int qihao_rc_left = 325+10+20;
static const int qihao_rc_top = 4;
static const int qihao_rc_width = 125;
static const int qihao_rc_height = 30;
static CRect rcQiHao(qihao_rc_left, qihao_rc_top, qihao_rc_left + qihao_rc_width, qihao_rc_top + qihao_rc_height);

static const int yugao_rc_left = 3;
static const int yugao_rc_top = 4;
static const int yugao_rc_width = 196;
static const int yugao_rc_height = 30;
static CRect rcYuGao(yugao_rc_left, yugao_rc_top, yugao_rc_left + yugao_rc_width, yugao_rc_top + yugao_rc_height);


//小开奖号
static const int kj_small_haoma_x = 61+5;
static const int kj_small_haoma_y = 46-12;
static const int kj_small_haoma_col_span = 5+5+3+2+2+2;
static const int kj_small_haoma_row_span = 6+5+2+3;

//总注数
static const int zongzhushu_x = 71;
static const int zongzhushu_y = 411;
static const int zongzhushu_width = 150;
static const int zongzhushu_height = 25;
static CRect rcZongZhuShu(zongzhushu_x, zongzhushu_y, zongzhushu_x+zongzhushu_width, zongzhushu_y+zongzhushu_height);

//总金额
static const int zongjine_x = 295;
static const int zongjine_y = 428;
static const int zongjine_width = 150;
static const int zongjine_height = 25;
static CRect rcZongJinE(zongjine_x, zongjine_y, zongjine_x+zongjine_width, zongjine_y+zongjine_height);

//总注数
static const int allzongzhushu_x = 0;
static const int allzongzhushu_y = 444;
static const int allzongzhushu_width = 150;
static const int allzongzhushu_height = 25;
static CRect rcAllZongzhushu(allzongzhushu_x, allzongzhushu_y, allzongzhushu_x+allzongzhushu_width, allzongzhushu_y+allzongzhushu_height);




//单式选号编辑框
static const int edit_haomao_danshi_x = 20;
static const int edit_haomao_danshi_y = 210;
static const int edit_haomao_danshi_width = 752;
static const int edit_haomao_danshi_height = 195;


/////////////////////////////////////////////////////////////////控制位置
//鼠标输入
static const int mouseinput_x = 597;
static const int mouseinput_y = 175;
//键盘输入
static const int keyboardinput_x = 670;
static const int keyboardinput_y = 175;

static const int text_dx_x = 553+4;
static const int text_dx_y = 18-12;
//玩法分类数据
static const int wanfa_x = 9;
static const int wanfa_y = 109;
static const int wanfa_col_width = 48;
static const int wanfa_row_height = 32;

//玩法子分类数据
static const int wanfa_sel_x = 20;
static const int wanfa_sel_y = 145;
static const int wanfa_sel_width = 71;
static const int wanfa_sel_height = 22;


//号码数据
static const int haoma_btn_x = 100-16;
static const int haoma_btn_y = 134+21;
static const int haoma_btn_col_wdith = 45;
static const int haoma_btn_row_height = 35;



//添加号码按钮
static const int btn_chq_add_x = 571;
static const int btn_chq_add_y = 415;
//
static const int btn_lock_x = 450;
static const int btn_lock_y = 417;



//选号显示区域数据
//删除选中按钮
static const int btn_del_sel_x = 646-40;
static const int btn_del_sel_y = 475+29;
//清空列表按钮
static const int btn_cls_list_x = 616-9;
static const int btn_cls_list_y = 120+4;
//号码列表
static const int list_haomao_x = 43;
static const int list_haomao_y = 469;
static const int list_haomao_width = 577;
static const int list_haomao_height = 58;
//倍投编辑框
static const int edit_beitou_x = 16-5;
static const int edit_beitou_y = 481+15;
static const int edit_beitou_width = 70;
static const int edit_beitou_height = 30;
//确认投注按钮
static const int btn_touzhu_x = 629-22;
static const int btn_touzhu_y = 426+24;
//追号按钮
static const int btn_zhuihao_x = 719;
static const int btn_zhuihao_y = 415;
//更多记录
static const int btn_more_x = 712+50;
static const int btn_more_y = 92-90;

//大筹码位置
static const int btn_jetton_x = 136-15;
static const int btn_jetton_y = 422+15;

static const int btn_yuan_x = 10;
static const int btn_yuan_y = 415;
static const int btn_jiao_x = 37+10;
static const int btn_jiao_y = 415;
static const int btn_fen_x = 64+20;
static const int btn_fen_y = 415;
static const int btn_li_x = 91+30;
static const int btn_li_y = 415;

static const int hitory_x = 608-16+2;
static const int hitory_y = 20-20+8;

static const int history_width = 182;
static const int history_hegiht = 93;

static CRect rcHistory(hitory_x,hitory_y,hitory_x+history_width,hitory_y+history_hegiht);
////////////////////////////////////////////////////////////////////////信息位置
static const int timer_id_get_luck_num = 1;		//获取开奖号码
static const int timer_id_kj_yugao = 2;			//开奖预告

static const int timer_id_kaijiangzhong=3;		//检查是否开奖中
//计时
static const int TimerJiShi = 4;				
static const int TimerStatus = 6;

//这个是用来本地刷新开奖数据的
static const int TimerZhunBeiData = 5;

static const int TimerFandian = 6;
static const int TimerLotteryStatus = 7;

static const int redraw_rc_left = 0;
static const int redraw_rc_top = 0;
static const int redraw_rc_width = 756;
static const int redraw_rc_height = 103;
static CRect rcRedraw(redraw_rc_left, redraw_rc_top, redraw_rc_left + redraw_rc_width, redraw_rc_top + redraw_rc_height);


// CXingYun28 对话框

IMPLEMENT_DYNAMIC(CXingYun28, CDialog)

CXingYun28::CXingYun28(CWnd* pParent /*=NULL*/)
	: CDialog(CXingYun28::IDD, pParent)
	, m_bmpBk(NULL)
	, m_numImageList(NULL)
	, m_numImageList1(NULL)
	, m_numImageList2(NULL)
	, m_kjNumBig(NULL)
	, m_kjNumSmall(NULL)
	, m_kjNumSmall1(NULL)
	, m_bmpDxdsh(NULL)
	, m_bmpDxdsh1(NULL)
	, m_bmpDxdsh2(NULL)
	, m_bmpNumTip(NULL)
	, m_bmpNumTip1(NULL)
	, m_bigNumWidth(0)
	, m_bigNumHeight(0)
	, m_smallNumWidth(0)
	, m_smallNumHeight(0)
	, m_nZongZhuShu(0)
	//, m_zongJinE(0.0)
	, m_beishu(0)
	, m_kjXinxiCont(0)
{

	m_gameKind = xy28_tm0;
	m_SocketID=0;
	m_nTzhSign = 1;
	m_nTzhSign1 = 0;
	m_nFrameIndex=0;
	m_nFrameCount=0;
	for (int i=0; i<Kj_XinXi_Count; i++) 
	{
		memset(&m_kjXinxi[i], 0, sizeof(KjXinXi));
	}

	m_RenxuanKeyboardInput=false;
	_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, _T(""));

	//任务组件
	m_bGetUserFandian=false;
	m_Font.CreateFont(14,0,0,0,700,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("微软雅黑"));
	m_wGameHoverItem=INVALID_WORD;
	m_cbMenuType=0xff;
	InitWanFaMoShi();
	m_bQueryTime=false;
	m_bGetCPUserInfo=false;
	m_bQueryLuckyNum=false;
	m_bTouzhu=false;
	m_bGetTime=false;
	m_bHovering=false;
	nPercent=0;
	m_bSetBonus=false;
	m_bGetLastYue=false;
	m_bGetMapBonus=false;
	m_fBonus = 0.0;
	m_fPercent = 0.0;
	fDanweiFandian=0.0f;
	nBonus = 0.0f;
	m_nTouzhuCount=0;
	m_nZhuihaoCount = 0;
	m_bZhuihao=false;
	//设置组件
	m_DlgStatus.SetStatusViewSink(this);

}


//取消连接
VOID CXingYun28::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}


CXingYun28::~CXingYun28()
{
	if (m_bmpBk != NULL) 
	{
		SafeDelete(m_bmpBk);
	}

	if (m_numImageList != NULL) 
	{
		SafeDelete(m_numImageList);
	}
	if (m_numImageList1 != NULL) 
	{
		SafeDelete(m_numImageList1);
	}
	if (m_numImageList2 != NULL) 
	{
		SafeDelete(m_numImageList2);
	}
	if (m_numImageList3 != NULL)
	{
//		SafeDelete(m_numImageList3);
	}

	if (m_kjNumBig != NULL) 
	{
		SafeDelete(m_kjNumBig);
	}
	if (m_kjNumBig2 != NULL) 
	{
	//	SafeDelete(m_kjNumBig2);
	}
	if (m_kjAdd != NULL) 
	{
//		SafeDelete(m_kjAdd);
	}
	if (m_kjAdd1 != NULL) 
	{
//		SafeDelete(m_kjAdd1);
	}

	if (m_kjNumSmall != NULL) 
	{
		SafeDelete(m_kjNumSmall);
	}
	if (m_kjNumSmall1 != NULL) 
	{
		SafeDelete(m_kjNumSmall1);
	}

	if (m_imgKaijiang != NULL) 
	{
		//SafeDelete(m_imgKaijiang);
	}

	if (m_bmpDxdsh != NULL) 
	{
		SafeDelete(m_bmpDxdsh);
	}
	if (m_bmpDxdsh1 != NULL) 
	{
		SafeDelete(m_bmpDxdsh1);
	}
	if (m_bmpDxdsh2 != NULL) 
	{
		SafeDelete(m_bmpDxdsh2);
	}

	if (m_bmpNumTip != NULL) 
	{
		SafeDelete(m_bmpNumTip);
	}
	if (m_bmpNumTip1 != NULL) 
	{
		SafeDelete(m_bmpNumTip1);
	}
	if (m_imgHistory != NULL) 
	{
//		SafeDelete(m_imgHistory);
	}
}

void CXingYun28::FlushZongjine()
{
	InvalidateRect(&rcZongJinE);
}

void CXingYun28::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_HEZHI_0, m_btnHeZhi_0);
	DDX_Control(pDX, IDC_BTN_HEZHI_1, m_btnHeZhi_1);
	DDX_Control(pDX, IDC_BTN_HEZHI_2, m_btnHeZhi_2);
	DDX_Control(pDX, IDC_BTN_HEZHI_3, m_btnHeZhi_3);
	DDX_Control(pDX, IDC_BTN_HEZHI_4, m_btnHeZhi_4);
	DDX_Control(pDX, IDC_BTN_HEZHI_5, m_btnHeZhi_5);
	DDX_Control(pDX, IDC_BTN_HEZHI_6, m_btnHeZhi_6);
	DDX_Control(pDX, IDC_BTN_HEZHI_7, m_btnHeZhi_7);
	DDX_Control(pDX, IDC_BTN_HEZHI_8, m_btnHeZhi_8);
	DDX_Control(pDX, IDC_BTN_HEZHI_9, m_btnHeZhi_9);
	DDX_Control(pDX, IDC_BTN_HEZHI_10, m_btnHeZhi_10);
	DDX_Control(pDX, IDC_BTN_HEZHI_11, m_btnHeZhi_11);
	DDX_Control(pDX, IDC_BTN_HEZHI_12, m_btnHeZhi_12);
	DDX_Control(pDX, IDC_BTN_HEZHI_13, m_btnHeZhi_13);
	DDX_Control(pDX, IDC_BTN_HEZHI_14, m_btnHeZhi_14);
	DDX_Control(pDX, IDC_BTN_HEZHI_15, m_btnHeZhi_15);
	DDX_Control(pDX, IDC_BTN_HEZHI_16, m_btnHeZhi_16);
	DDX_Control(pDX, IDC_BTN_HEZHI_17, m_btnHeZhi_17);
	DDX_Control(pDX, IDC_BTN_HEZHI_18, m_btnHeZhi_18);
	DDX_Control(pDX, IDC_BTN_HEZHI_19, m_btnHeZhi_19);
	DDX_Control(pDX, IDC_BTN_HEZHI_20, m_btnHeZhi_20);
	DDX_Control(pDX, IDC_BTN_HEZHI_21, m_btnHeZhi_21);
	DDX_Control(pDX, IDC_BTN_HEZHI_22, m_btnHeZhi_22);
	DDX_Control(pDX, IDC_BTN_HEZHI_23, m_btnHeZhi_23);
	DDX_Control(pDX, IDC_BTN_HEZHI_24, m_btnHeZhi_24);
	DDX_Control(pDX, IDC_BTN_HEZHI_25, m_btnHeZhi_25);
	DDX_Control(pDX, IDC_BTN_HEZHI_26, m_btnHeZhi_26);
	DDX_Control(pDX, IDC_BTN_HEZHI_27, m_btnHeZhi_27);



	DDX_Control(pDX, IDC_BTN_K8RX, m_btnRenXuan);		//快8任选
	DDX_Control(pDX, IDC_BTN_TEMA, m_btnTeMa);			
	DDX_Control(pDX, IDC_BTN_DXDS, m_btnDaXiaoDanShuang);
	DDX_Control(pDX, IDC_BTN_FUNNY, m_btnFunny);
	DDX_Control(pDX, IDC_BTN_JIZHI, m_btnJiZhi);

	DDX_Control(pDX, IDC_BTN_DX, m_btnTextDx);
	DDX_Control(pDX, IDC_BTN_DSH, m_btnTextDsh);

	DDX_Control(pDX, IDC_BTN_JETTON_1,	m_btnJetton1);
	DDX_Control(pDX, IDC_BTN_JETTON_2,	m_btnJetton2);
	DDX_Control(pDX, IDC_BTN_JETTON_3,	m_btnJetton3);
	DDX_Control(pDX, IDC_BTN_JETTON_4,	m_btnJetton4);
	DDX_Control(pDX, IDC_BTN_JETTON_5,	m_btnJetton5);



	//大小单双选择
	DDX_Control(pDX, IDC_BTN_DA_GE, m_btnDa);
	DDX_Control(pDX, IDC_BTN_XIAO_GE, m_btnXiao);
	DDX_Control(pDX, IDC_BTN_DAN_GE, m_btnDan);
	DDX_Control(pDX, IDC_BTN_SHUANG_GE, m_btnShuang);
	DDX_Control(pDX, IDC_BTN_XIAODAN, m_btnXiaoDan);
	DDX_Control(pDX, IDC_BTN_XIAOSHUANG, m_btnXiaoShuang);
	DDX_Control(pDX, IDC_BTN_DADAN, m_btnDaDan);
	DDX_Control(pDX, IDC_BTN_DASHUANG, m_btnDaShuang);

	DDX_Control(pDX, IDC_BTN_MAX,	m_btnMax);
	DDX_Control(pDX, IDC_BTN_MIN,	m_btnMin);



	DDX_Control(pDX, IDC_LIST_CHQSSC, m_listChqSSC);
	DDX_Control(pDX, IDC_GRID_LIST, m_GridList);
	DDX_Control(pDX, IDC_BTN_CHQ_ADD, m_btnChqAdd);
	DDX_Control(pDX, IDC_BTN_DEL_SEL, m_btnDelSel);
	DDX_Control(pDX, IDC_BTN_CLS_LIST, m_btnClsList);
	DDX_Control(pDX, IDC_BTN_TOUZHU, m_btnTouZhu);
	DDX_Control(pDX, IDC_BTN_ZHUIHAO, m_btnZhuiHao);
	DDX_Control(pDX, IDC_EDIT_BEISHU, m_editBeiTou);

	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);





	DDX_Text(pDX, IDC_EDIT_BEISHU, m_beishu);
	DDX_Control(pDX, IDC_EDIT_DANSHI_HAOMA, m_editDanShiHaoMa);
	DDX_Control(pDX, IDC_RICHEDIT_DANSHI, m_richDanshiHaoma);
	//DDX_Control(pDX, IDC_BTN_LOCK, m_btnLock);

	DDX_Control(pDX, IDC_EDIT_JETTON_1, m_editJetton1);
	DDX_Control(pDX, IDC_EDIT_JETTON_2, m_editJetton2);
	DDX_Control(pDX, IDC_EDIT_JETTON_3, m_editJetton3);
	DDX_Control(pDX, IDC_EDIT_JETTON_4, m_editJetton4);
	DDX_Control(pDX, IDC_EDIT_JETTON_5, m_editJetton5);

}


BEGIN_MESSAGE_MAP(CXingYun28, CDialog)
// 	ON_BN_CLICKED(IDC_BTN_YUAN, &CXingYun28::OnBnClickedBtnYuan)
// 	ON_BN_CLICKED(IDC_BTN_JIAO, &CXingYun28::OnBnClickedBtnJiao)
// 	ON_BN_CLICKED( IDC_BTN_FEN, &CXingYun28::OnBnClickedBtnFen)
// 	ON_BN_CLICKED(IDC_BTN_LI, &CXingYun28::OnBnClickedBtnLi)
	ON_MESSAGE( WM_BN_CLICK, &CXingYun28::onBnCLick)

	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)

	//玩法选择
	ON_BN_CLICKED(IDC_BTN_K8RX, &CXingYun28::OnBnClinckedBtnRenXuan)//快8任选
	ON_BN_CLICKED(IDC_BTN_TEMA, &CXingYun28::OnBnClinckedBtnTeMa)		//特码
	ON_BN_CLICKED(IDC_BTN_DXDS, &CXingYun28::OnBnClinckedBtnDaXiaoDanShuang)//大小单双
	ON_BN_CLICKED(IDC_BTN_FUNNY, &CXingYun28::OnBnClinckedBtnFunny)	//趣味玩法
	ON_BN_CLICKED(IDC_BTN_JIZHI, &CXingYun28::OnBnClinckedBtnJiZhi)//极值

	ON_BN_CLICKED(IDC_BTN_DX, &CXingYun28::OnBnClinckedBtnDx)	//趣味玩法
	ON_BN_CLICKED(IDC_BTN_DSH, &CXingYun28::OnBnClinckedBtnDsh)//极值
// 
// 	ON_BN_CLICKED(IDC_RIO_DUIZI, &CXingYun28::OnRioDuiZi)
// 	ON_BN_CLICKED(IDC_RIO_SHUNZI, &CXingYun28::OnRioShunZi)
// 	ON_BN_CLICKED(IDC_RIO_BAOZI, &CXingYun28::OnRioBaoZi)

	

	//控制按钮
	ON_BN_CLICKED(IDC_BTN_ZHUIHAO, &CXingYun28::OnBnClickedBtnZhuihao)//追号
	ON_BN_CLICKED(IDC_BTN_TOUZHU, &CXingYun28::OnBnClickedBtnTouzhu)//投注
	ON_BN_CLICKED(IDC_BTN_CHQ_ADD, &CXingYun28::OnBnClickedBtnChqAdd)//添加号码
	ON_BN_CLICKED(IDC_BTN_DEL_SEL, &CXingYun28::OnBnClickedBtnDelSel)//删除
	ON_BN_CLICKED(IDC_BTN_CLS_LIST, &CXingYun28::OnBnClickedBtnClsList)//清空
	ON_BN_CLICKED(IDC_BTN_SAVE, &CXingYun28::OnBnClickedBtnSave)//设置返点
// 	ON_BN_CLICKED(IDC_BTN_BEITOU_JIA, &CXingYun28::OnBnClickedBeiTouJia)
// 	ON_BN_CLICKED(IDC_BTN_BEITOU_JIAN, &CXingYun28::OnBnClickedBeiTouJian)

	//投注修改
	ON_EN_CHANGE(IDC_EDIT_BEISHU, &CXingYun28::OnEnChangeEditBeishu)//修改倍数
	ON_EN_CHANGE(IDC_EDIT_DANSHI_HAOMA, &CXingYun28::OnEnChangeEditDanshiInput)//修改单数

	ON_BN_CLICKED(IDC_BTN_JETTON_1, &CXingYun28::OnBnClickedBtnJetton1)//筹码
	ON_BN_CLICKED(IDC_BTN_JETTON_2, &CXingYun28::OnBnClickedBtnJetton2)//筹码
	ON_BN_CLICKED(IDC_BTN_JETTON_3, &CXingYun28::OnBnClickedBtnJetton3)//筹码
	ON_BN_CLICKED(IDC_BTN_JETTON_4, &CXingYun28::OnBnClickedBtnJetton4)//筹码
	ON_BN_CLICKED(IDC_BTN_JETTON_5, &CXingYun28::OnBnClickedBtnJetton5)//筹码

END_MESSAGE_MAP()





// CXingYun28 消息处理程序
void CXingYun28::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(!IsWindowVisible())
		return;

	// TODO: 在此处添加消息处理程序代码
	if(!::IsWindow(m_hWnd))
	{
		return;
	}
	CRect rect;
	GetClientRect(&rect);

	CDC cacheDC;
	cacheDC.CreateCompatibleDC(&dc);

	CBitmap cacheBmp;
	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

	Graphics graphics(cacheDC.m_hDC);
	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 9,28, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	//绘制开奖号
	DrawLastKjHaoma(&cacheDC, graphics);
	DrawTopKjHaoma(&cacheDC, graphics);

	//绘制总注数和总金额
	DrawZongZhushuJinE(&cacheDC);

	cacheDC.SetBkMode(TRANSPARENT);
	cacheDC.SetTextColor(RGB(88, 78, 77));
	CFont* pOldFont = cacheDC.SelectObject(&m_Font);

	m_FandianView.DrawView(this,&cacheDC,rect.Width(),rect.Height());

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldFont);
	cacheDC.SelectObject(pOldCacheBmp);
	cacheDC.DeleteDC();
	cacheBmp.DeleteObject();

	return;
}

BOOL CXingYun28::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CXingYun28::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL) 
	{
		return;
	}

	AdjustWanFa();
	AdjustRadio();
	AdjustNumBtn();
	AdjustNumView();
	AdjustDanShi();
	AdjustAdd();
}

//校验输入
bool CXingYun28::CheckInput()
{
	if (_tcslen(m_lastExpect) == 0)
	{
		MyMessageBox(_T("正在获取数据…… "));
		return false;
	}

	CString str;
	m_editBeiTou.GetWindowText(str);
	if (str.IsEmpty())
	{
		MyMessageBox(_T("投注金额不可为空"));
		return false;
	}
	if(m_beishu == 0)
	{
		MyMessageBox(_T("投注金额最少为1"));
		return false;
	}

	if (m_strTouzhuHaoma.IsEmpty()) 
	{
		MyMessageBox(_T("您还未选择号码"));
		return false;
	}

	//判断余额是否充足
	double fCellPrice=0.0;
	if(theAccount.Scoretype == MoShi_Yuan)
		fCellPrice = 2.0;
	else if(theAccount.Scoretype == MoShi_Jiao)
		fCellPrice = 0.2;
	else
		fCellPrice = 0.02;
	if(theAccount.yue < m_beishu)
	{
		MyMessageBox(_T("您余额不足，请充值后再试"));
		return false;
	}
	//判断是否临近开奖点
	//重新获取服务器时间
	//theApp.GetTimeDiff();
	long kjing = m_chqRule.GetKjShjDiff(); 
	if (kjing <60)								// || kjing > 540
	{
		MyMessageBox(_T("此期封单,请稍后再试"));
		return false;
	}



	return true;
}

//显示
void CXingYun28::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	//OnBnClickedBtnClsList();
	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
		SendToServer(6);
		m_bShowEdit = false;
		m_editJetton1.ShowWindow(SW_HIDE);
		m_editJetton2.ShowWindow(SW_HIDE);
		m_editJetton3.ShowWindow(SW_HIDE);
		m_editJetton4.ShowWindow(SW_HIDE);
		m_editJetton5.ShowWindow(SW_HIDE);
		m_cbIfTs = 0;
		SendToServer(11);
		KillTimer( TimerLotteryStatus);
		SetTimer(TimerLotteryStatus, 30000, NULL);

		//读取配置文件
		//工作目录
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//构造路径
		TCHAR szFileName[MAX_PATH]=TEXT("");
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
		TCHAR szText[32]=TEXT("");
		GetPrivateProfileString(TEXT("JETTON"),TEXT("jetton1"),L"5",szText,CountArray(szText),szFileName);
		m_btnJetton1.SetWindowText(szText);
		GetPrivateProfileString(TEXT("JETTON"),TEXT("jetton2"),L"50",szText,CountArray(szText),szFileName);
		m_btnJetton2.SetWindowText(szText);
		GetPrivateProfileString(TEXT("JETTON"),TEXT("jetton3"),L"500",szText,CountArray(szText),szFileName);
		m_btnJetton3.SetWindowText(szText);
		GetPrivateProfileString(TEXT("JETTON"),TEXT("jetton4"),L"1000",szText,CountArray(szText),szFileName);
		m_btnJetton4.SetWindowText(szText);
		GetPrivateProfileString(TEXT("JETTON"),TEXT("jetton5"),L"5000",szText,CountArray(szText),szFileName);
		m_btnJetton5.SetWindowText(szText);
		
		m_bKaiJiangzhong = false;
		SetTimer(timer_id_kj_yugao, 1000, NULL);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);

		OnBnClinckedBtnDx();
		OnBnClinckedBtnTeMa();
	}

	//InvalidateRect(CRect(0,0,1000,1000),true);
}

 //定时器
void CXingYun28::OnTimer(UINT_PTR nIDEvent)
{
	if (timer_id_kj_yugao == nIDEvent)
	{
		CRect rcRect;
		rcRect.CopyRect(rcCzh);

		rcRect.left +=30;
		rcRect.right  +=30;
		InvalidateRect(&rcRect);
	}
	else if(TimerJiShi == nIDEvent)
	{
		DWORD newCount = ::GetTickCount();
		int i = 0;
		while (newCount - theBeginCount > 1000) {
			theTimeCS.Lock();
			theTime += CTimeSpan(0, 0, 0, 1);

			theTimeCS.Unlock();
			newCount -= 1000;
			i ++;
		}
		theBeginCount += i * 1000;

	}
	else if(TimerFandian == nIDEvent)
	{
		SendToServer(6);
	}
	else if(TimerLotteryStatus == nIDEvent)
	{
		SendToServer(11);
	}
	else if(timer_id_kaijiangzhong == nIDEvent)
	{
		//处理开奖GIF动画
		int nTimeLeft = GetKjShjDiff();
		if(m_bKaiJiangzhong )
		{
			CDC* pDC = GetDC();
			if(pDC==NULL)
				return;
			//for(int n=0; n<m_nFrameCount; n++)
			{
				int nLeft=kj_big_haoma_x-18 /*+ n *( m_bigNumWidth+8)*/;
				CMemDC	mDC(pDC, CRect(nLeft, kj_big_haoma_y+9, nLeft+m_imgKaijiang->GetWidth()+190+10, kj_big_haoma_y+m_imgKaijiang->GetHeight()+37));
				Graphics gh(mDC.m_hDC);
				gh.DrawImage(m_imgKaijiang, nLeft, kj_big_haoma_y+9, m_imgKaijiang->GetWidth(), m_imgKaijiang->GetHeight());
			}
			GUID Guid = FrameDimensionTime;
			m_imgKaijiang->SelectActiveFrame(&Guid, m_nFrameIndex++);
			if(m_nFrameIndex == m_nFrameCount)
				m_nFrameIndex=0;
			ReleaseDC(pDC);
		}
		else
			KillTimer(timer_id_kaijiangzhong);
	}
	else if(nIDEvent == TimerStatus)
	{
		KillTimer(TimerStatus);
		m_DlgStatus.HideStatusWindow();
		CInformation Information;
		Information.ShowMessageBox(L"温馨提醒",L"服务器繁忙，投注失败",MB_OK,5);
	}


	CDialog::OnTimer(nIDEvent);
}

HBRUSH CXingYun28::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == IDC_EDIT_DANSHI_HAOMA) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(70,0, 0));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(70,0, 0));
		}

		return hBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RICHEDIT_DANSHI) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(70,0, 0));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(70,0, 0));
		}

		return hBrush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//添加号码
void CXingYun28::OnBnClickedBtnChqAdd()
{

}

//追号
void CXingYun28::OnBnClickedBtnZhuihao()
{
	if(!CheckInput())
		return ;
	CDlgAddtional	dlgZhuiHao;
	double danzhujine = 2.0;

	if(theAccount.Scoretype == MoShi_Yuan)
	{	
		danzhujine = 2.0;
	}
	else if (theAccount.Scoretype == MoShi_Jiao)
	{
		danzhujine = 0.2;
	}
	else if(theAccount.Scoretype == MoShi_Fen)
	{
		danzhujine = 0.02;
	}
	CString strBeishu ;
	strBeishu.Format(L"%d",m_beishu);

	//if(m_bKaiJiangzhong)
	dlgZhuiHao.SetCurrentQihaoTime(m_chqRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,0,6, m_nZongZhuShu  * danzhujine,10000000,CZXingYun28);
	// 	else
	// 		dlgZhuiHao.SetCurrentQihaoTime( m_kjXinxi[0].qihao, strBeishu,8,3, m_zongZhuShu * danzhujine,120);
	if(dlgZhuiHao.DoModal() == IDOK)
	{
		m_nZhuihaoCount = dlgZhuiHao.GetZhuiHaoQiShu();
		dlgZhuiHao.GetZhuiHaoData(vecData1, vecData2, vecData3);


		m_bZhuiZjtz = dlgZhuiHao.GetPrizeStop();

		DOUBLE fZongJine = 0.0;
		for(int i = 0;i < m_nZhuihaoCount;i++)
		{
			int nBeishu = _ttoi(vecData1[i].c_str());
			fZongJine += m_nZongZhuShu*nBeishu*danzhujine;
		}

		if(fZongJine > theAccount.yue)
		{
			MyMessageBox(_T("您余额不足，请充值后再试"));
		}
		else
			SendToServer(4);
	}
	else
		return;

	//初始化参数
	m_nZongZhuShu = 0;

	m_beishu = 0;
	//UpdateData(FALSE);
//OnBnClickedBtnClsList();
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
}
//投注
void CXingYun28::OnBnClickedBtnTouzhu()
{
	//校验输入
	if(!CheckInput())
		return;

	//请用户确认信息
	CQrTzhDlg qrDlg;
	qrDlg.m_gamekind = _T("幸运28");
	qrDlg.m_zhushu = 1;
	qrDlg.m_zhushu = m_nZongZhuShu * m_beishu;
	qrDlg.m_moshi = theAccount.Scoretype;
	qrDlg.m_zongjine = m_zongJine;
	if (qrDlg.DoModal() != IDOK)
	{
		return;
	}

	long kjing = m_chqRule.GetKjShjDiff(); 
	if (kjing <60)								// || kjing > 540
	{
		MyMessageBox(_T("此期封单,请稍后再试"));
		return ;
	}

	bool bSucc = true;
	int itm_cnt = m_listChqSSC.GetItemCount();
	m_nTouzhuCount=0;
	m_nTouzhuCount = itm_cnt;

	SendToServer(1);

	m_editBeiTou.SetWindowText(L"0");

	m_nZongZhuShu = 0;

	m_beishu = 0;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);

	InvalidateRect(&rcZongJinE);

}

void CXingYun28::OnBnClickedBtnJetton1()//筹码
{
	CString strText;
	m_btnJetton1.GetWindowText(strText);
	int nJetton = _ttoi(strText);

	m_editBeiTou.GetWindowText(strText);
	int nAllJetton = _ttoi(strText);

	nAllJetton += nJetton;
	m_zongJine = nAllJetton;
	strText.Format(L"%d",nAllJetton);
	m_editBeiTou.SetWindowText(strText);
}
void CXingYun28::OnBnClickedBtnJetton2()//筹码
{
	CString strText;
	m_btnJetton2.GetWindowText(strText);
	int nJetton = _ttoi(strText);

	m_editBeiTou.GetWindowText(strText);
	int nAllJetton = _ttoi(strText);

	nAllJetton += nJetton;
	m_zongJine = nAllJetton;
	strText.Format(L"%d",nAllJetton);
	m_editBeiTou.SetWindowText(strText);
}

void CXingYun28::OnBnClickedBtnJetton3()//筹码
{
	CString strText;
	m_btnJetton3.GetWindowText(strText);
	int nJetton = _ttoi(strText);

	m_editBeiTou.GetWindowText(strText);
	int nAllJetton = _ttoi(strText);

	nAllJetton += nJetton;
	m_zongJine = nAllJetton;

	strText.Format(L"%d",nAllJetton);
	m_editBeiTou.SetWindowText(strText);
}

void CXingYun28::OnBnClickedBtnJetton4()//筹码
{
	CString strText;
	m_btnJetton4.GetWindowText(strText);
	int nJetton = _ttoi(strText);

	m_editBeiTou.GetWindowText(strText);
	int nAllJetton = _ttoi(strText);

	nAllJetton += nJetton;
	m_zongJine = nAllJetton;

	strText.Format(L"%d",nAllJetton);
	m_editBeiTou.SetWindowText(strText);
}
void CXingYun28::OnBnClickedBtnJetton5()//筹码
{
	CString strText;
	m_btnJetton5.GetWindowText(strText);
	int nJetton = _ttoi(strText);

	m_editBeiTou.GetWindowText(strText);
	int nAllJetton = _ttoi(strText);

	nAllJetton += nJetton;
	m_zongJine = nAllJetton;

	strText.Format(L"%d",nAllJetton);
	m_editBeiTou.SetWindowText(strText);
}

void CXingYun28::OnBnClickedBtnSave()
{
	m_btnDelSel.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\xy28_zdy.png"));
	m_btnDelSel.Invalidate();

	//读取配置文件
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	CString strText;
	m_editJetton1.GetWindowText(strText);
	if(!strText.IsEmpty())
	{
		
		WritePrivateProfileString(TEXT("JETTON"),TEXT("jetton1"),strText,szFileName);
		m_btnJetton1.SetWindowText(strText);
	}
	m_editJetton2.GetWindowText(strText);
	if(!strText.IsEmpty())
	{
		WritePrivateProfileString(TEXT("JETTON"),TEXT("jetton2"),strText,szFileName);
		m_btnJetton2.SetWindowText(strText);
	}
	m_editJetton3.GetWindowText(strText);
	if(!strText.IsEmpty())
	{
		WritePrivateProfileString(TEXT("JETTON"),TEXT("jetton3"),strText,szFileName);
		m_btnJetton3.SetWindowText(strText);
	}
	m_editJetton4.GetWindowText(strText);
	if(!strText.IsEmpty())
	{
		WritePrivateProfileString(TEXT("JETTON"),TEXT("jetton4"),strText,szFileName);
		m_btnJetton4.SetWindowText(strText);
	}
	m_editJetton5.GetWindowText(strText);
	if(!strText.IsEmpty())
	{
		WritePrivateProfileString(TEXT("JETTON"),TEXT("jetton5"),strText,szFileName);
		m_btnJetton5.SetWindowText(strText);
	}
	m_btnSave.ShowWindow(SW_HIDE);
	m_editJetton1.ShowWindow(!m_bShowEdit);
	m_editJetton2.ShowWindow(!m_bShowEdit);
	m_editJetton3.ShowWindow(!m_bShowEdit);
	m_editJetton4.ShowWindow(!m_bShowEdit);
	m_editJetton5.ShowWindow(!m_bShowEdit);

	m_bShowEdit = !m_bShowEdit;
}
//删除
void CXingYun28::OnBnClickedBtnDelSel()
{
	if(!m_bShowEdit)
	{
		m_btnDelSel.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\xy28_qxzdy.png"));
		m_btnSave.ShowWindow(SW_SHOW);

		CString strText;
		m_btnJetton1.GetWindowText(strText);
		if(!strText.IsEmpty())
		{
			m_editJetton1.SetWindowText(strText);
		}
		m_btnJetton2.GetWindowText(strText);
		if(!strText.IsEmpty())
		{
			m_editJetton2.SetWindowText(strText);
		}
		m_btnJetton3.GetWindowText(strText);
		if(!strText.IsEmpty())
		{
			m_editJetton3.SetWindowText(strText);
		}
		m_btnJetton4.GetWindowText(strText);
		if(!strText.IsEmpty())
		{
			m_editJetton4.SetWindowText(strText);
		}
		m_btnJetton5.GetWindowText(strText);
		if(!strText.IsEmpty())
		{
			m_editJetton5.SetWindowText(strText);
		}

	}
	else
	{
		m_btnSave.ShowWindow(SW_HIDE);
		m_btnDelSel.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\xy28_zdy.png"));

	}
	m_editJetton1.ShowWindow(!m_bShowEdit);
	m_editJetton2.ShowWindow(!m_bShowEdit);
	m_editJetton3.ShowWindow(!m_bShowEdit);
	m_editJetton4.ShowWindow(!m_bShowEdit);
	m_editJetton5.ShowWindow(!m_bShowEdit);

	m_bShowEdit = !m_bShowEdit;

}
//单数修改
void CXingYun28::OnEnChangeEditDanshiInput()
{
	m_richDanshiHaoma.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
}

//倍数 修改
void CXingYun28::OnEnChangeEditBeishu()
{
	CString str;
	m_editBeiTou.GetWindowText(str);
	if ( (_ttoi(str) < 0)) 
	{
		if(!str.IsEmpty())
		{
			MyMessageBox(_T("投注金额必须是一个正整数！"));
			m_beishu = 0;
			UpdateData(FALSE);
		}
	}
	else if(_ttoi(str)>theAccount.yue)
	{
		MyMessageBox(_T("投注金额不能超过自己的本金！"));
		m_beishu = 0;
		UpdateData(FALSE);

	}
	else
	{
		UpdateData();

		GetXy28Zhushu(CZXingYun28,m_gameKind);
		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=400;
		InvalidateRect(&rcInvali);
		InvalidateRect(&rcZongJinE);
		rcInvali.CopyRect(rcAllZongzhushu);
		rcInvali.bottom += 20;
		InvalidateRect(&rcInvali);
	}

}

//鼠标消息
VOID CXingYun28::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

}

//鼠标消息
VOID CXingYun28::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//设置焦点
	SetFocus();

	return;
}
//鼠标消息
VOID CXingYun28::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
}
//鼠标消息
LRESULT CXingYun28::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置状态
	m_bHovering=false;
	return 0;
}


//清空
void CXingYun28::OnBnClickedBtnClsList()
{

	//读取配置文件
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	//读取配置
	CWHIniData IniData;
	IniData.SetIniFilePath(szFileName);

	TCHAR szPlatformUrl[124] = L"";

	IniData.ReadEncryptString(TEXT("GameRule"),TEXT("XY28"),TEXT("http://192.168.0.104/lucky28.html"),szPlatformUrl,CountArray(szPlatformUrl));
	CDlgBrowser dlgBrowser;
	dlgBrowser.CreateBrowserControl(TEXT("游戏规则"),szPlatformUrl,CSize(670,450));
}

//设置返点
//void CXingYun28::OnBnClickedBtnLock()
//{
//	int index = (CZXingYun28 << 16) | (unsigned short)m_gameKind;
//
//
//	CDlgFandian	dlg;
//	dlg.SetBonus(mapBonus[m_gameKind], theBonus[index], mapPercent[m_gameKind], theAccount.fandian*100.0, mapDanweiFandian[m_gameKind]);
//	if(IDOK == dlg.DoModal())
//	{
//		dlg.GetBonus(m_fBonus,  m_fPercent);
//
//		SendToServer(5);
//	}
//}
//


void CXingYun28::OnBnClinckedBtnDaXiaoDanShuang()		//大小单双
{
}


//北京快8
void CXingYun28::OnBnClinckedBtnRenXuan()	//任选
{

}

//特码
void CXingYun28::OnBnClinckedBtnTeMa()	//特码
{
	CheckWanFaBtn(m_btnTeMa);



	SendToServer(3);

	ResetAllNums();
	HideBJNums();
	ShowDaXiaoDanShuang();
	HideRio();
	ShowJiZji();



	//OnBnClickedBtnClsList();

	HideDanShiEdit();
	ShowHezhiNums();
	//CheckNumBtn(m_btnHeZhi_0);
	OnCommand(IDC_BTN_HEZHI_0,0);
	
	//Invalidate(false);
}
//趣味玩法
void CXingYun28::OnBnClinckedBtnFunny()
{
// 	CheckWanFaBtn(m_btnFunny);
// 	m_gameKind = enXY28_Funny;
// 	SendToServer(3);
// 
// 	ResetAllNums();
// 
// 	HideJiZhi();
// 	HideRio();
// 	HideBJNums();
// 	HideHeZhiNums();
// 	HideDanShiEdit();
// 	HideDaXiaoDanShuang();
// 
// 	m_rioDuiZi.ShowWindow(SW_SHOW);
// 	m_rioShunZi.ShowWindow(SW_SHOW);
// 	m_rioBaoZi.ShowWindow(SW_SHOW);
// 	OnBnClickedBtnClsList();
// 
// 	//OnRioDuiZi();
// 	Invalidate(false);

}
void CXingYun28::OnBnClinckedBtnDx()
{
	m_bDx = true;
	m_btnTextDx.SetPushed(true);
	m_btnTextDsh.SetPushed(false);
	InvalidateRect(&rcHistory);
}

void CXingYun28::OnBnClinckedBtnDsh()
{
	m_bDx = false;
	m_btnTextDx.SetPushed(false);
	m_btnTextDsh.SetPushed(true);

	InvalidateRect(&rcHistory);

}
//极值
void CXingYun28::OnBnClinckedBtnJiZhi()
{
}




//初始化玩法信息
void CXingYun28::InitWanFaMoShi()
{
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm0] = _T("特码0");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm1] = _T("特码1");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm2] = _T("特码2");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm3] = _T("特码3");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm4] = _T("特码4");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm5] = _T("特码5");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm6] = _T("特码6");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm7] = _T("特码7");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm8] = _T("特码8");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm9] = _T("特码9");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm10] = _T("特码10");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm11] = _T("特码11");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm12] = _T("特码12");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm13] = _T("特码13");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm14] = _T("特码14");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm15] = _T("特码15");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm16] = _T("特码16");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm17] = _T("特码17");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm18] = _T("特码18");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm19] = _T("特码19");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm20] = _T("特码20");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm21] = _T("特码21");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm22] = _T("特码22");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm23] = _T("特码23");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm24] = _T("特码24");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm25] = _T("特码25");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm26] = _T("特码26");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)xy28_tm27] = _T("特码27");


	theGameKind[(CZXingYun28 << 16) | (unsigned short)enXY28_DXDS] = _T("大小单双");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enXY28_Funny] = _T("组合");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enXY28_JiZhi] = _T("极值");
}



VOID CXingYun28::SendToServer(int nSendType)
{
	if(nSendType == 1)
	{
		int nSendCount = 0;
		int nEnd = 1;
		if(m_cbIfTs == 1)
		{
			MyMessageBox(L"此彩种暂停销售！");
			return;
		}

		CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
		ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
		TouzhuCQSSC.dwUserID = theAccount.user_id;
		TouzhuCQSSC.nBeitou = m_beishu;
		TouzhuCQSSC.nGameKind = m_gameKind;
		TouzhuCQSSC.nGameType = CZXingYun28;
		TouzhuCQSSC.nMoshi = theAccount.Scoretype;

		TouzhuCQSSC.nSign = m_nTzhSign;

		CString strQiHao = m_chqRule.GetNextExpect();

		string	strAllHaoma;
		ChangeStringToA(m_strTouzhuHaoma, strAllHaoma);

		if (m_gameKind == 0)
		{
			AfxGetMainWnd()->MessageBox(L"kindId = 0");
		}

		string strQi;
		ChangeStringToA(strQiHao, strQi);

		memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());


		if(m_strTouzhuHaoma.IsEmpty())
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(L"你还未选择号码，请选择！");

			return ;
		}

		byte pDestBuffer[102400];
		int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

		int nSendSize = 0;
		TouzhuCQSSC.nEnd = 1;
		m_nZongZhuShu = 1;
		if(m_nZongZhuShu<=0)
		{
			m_DlgStatus.HideStatusWindow();
			//OnBnClickedBtnClsList();
			return;
		}

		TouzhuCQSSC.nZhushu = m_nZongZhuShu;
		int nSize = sizeof(TouzhuCQSSC.strHaoma);
		DWORD dwTick=GetTickCount();

		while (nSendSize < nLen)
		{
			while ((GetTickCount() - dwTick ) < 30000)
			{
				MSG msg;
				if(GetMessage (&msg, NULL, 0, 0))
				{
					TranslateMessage (&msg) ;

					DispatchMessage (&msg) ;

				}
				if(m_bCanSend)
					break;

			}
			int nHaoMaLen = 0;
			nHaoMaLen = min(nSize,nLen-nSendSize);
			TouzhuCQSSC.nHaoMaLen = nHaoMaLen;	//nLen;

			TouzhuCQSSC.nIndex = nSendCount;
			memset(TouzhuCQSSC.strHaoma, 0, sizeof(TouzhuCQSSC.strHaoma));
			memcpy((byte*)TouzhuCQSSC.strHaoma, pDestBuffer+nSendSize, nSize);


			if(nSendSize + nSize >= nLen)
				TouzhuCQSSC.nEnd = 0;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_DAN,&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
			m_bShowFail=true;

			nSendSize += nHaoMaLen;
			m_bCanSend = false;
			nSendCount++;
		}
		m_bCanSend = true;

		m_nTzhSign++;
		SetTimer(TimerStatus,60000,NULL);
		//	m_strHaoma = _T("");
		//OnBnClickedBtnClsList();


		return;

	}

	if(nSendType == 2)   //没用
	{
		m_bGetCPUserInfo = true;
		return;
	}

	if(nSendType == 3)
	{
// 		m_btnLock.SetWindowText(mapFandian[m_gameKind]);
// 
// 
// 		OnBnClickedBtnClsList();
// 
// 		m_bGetUserFandian = true;
// 		m_nZongZhuShu = 0;
// 		m_editBeiTou.SetWindowText(L"1");
// 		m_beishu=1;
// 
// 		CRect rcInvali;
// 		rcInvali.CopyRect(rcZongZhuShu);
// 		rcInvali.right+=400;
// 		InvalidateRect(&rcInvali);
// 
// 		InvalidateRect(&rcZongJinE);
// 		return;
	}

	if(nSendType == 4)
	{
		m_bZhuihao = true;
		if(m_bZhuihao)
		{
			m_bZhuihao = false;
			if(m_cbIfTs == 1)
			{
				MyMessageBox(L"此彩种暂停销售！");
				return;
			}

			m_DlgStatus.ShowStatusWindow(TEXT("正在提交数据，请稍候..."));

			//if(m_RenxuanKeyboardInput/*&&m_strHaoma.GetLength() >10240*/)
			{
				if(m_strHaoma.IsEmpty() && vecAllNums.size()>0)
				{
					for(int i = 0;i < vecAllNums.size();i++)
					{
						m_strHaoma += vecAllNums[i].c_str();
						m_strHaoma += L"|";
					}

				}

				//for(int n=0; n< vecData2.size(); n++)
				{
					int nSendCount = 0;
					int nEnd = 1;

					CMD_GP_TouZhuCQSSC_Zhuihao TouzhuCQSSC;
					ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
					TouzhuCQSSC.dwUserID = theAccount.user_id;
					TouzhuCQSSC.nGameKind = m_gameKind;
					TouzhuCQSSC.nGameType = CZXingYun28;
					TouzhuCQSSC.nMoshi = theAccount.Scoretype;

					TouzhuCQSSC.nSign = m_nTzhSign;
					if(m_bZhuiZjtz)
						TouzhuCQSSC.bZhuihao = GetTickCount();
					else
						TouzhuCQSSC.bZhuihao = 2;

					if(m_strHaoma.IsEmpty())
					{
						m_DlgStatus.HideStatusWindow();
//						MyMessageBox(L"你还未选择号码，请选择！");
						MyMessageBox(L"你还未选择号码，请选择！");
						//OnBnClickedBtnClsList();
						return ;
					}
					TCHAR szMD5Haoma[33];
					memset(szMD5Haoma, 0, sizeof(szMD5Haoma));

// 					CMD5Encrypt::EncryptData(m_strHaoma,szMD5Haoma);
// 
// 					if(lstrcmp(m_szMD5Haoma,szMD5Haoma)!=0)
// 					{
// 						OnBnClickedBtnClsList();
// 						return ;
// 					}



					string	strAllHaoma;
					ChangeStringToA(m_strHaoma, strAllHaoma);

					memset(TouzhuCQSSC.strQishu,0,sizeof(TouzhuCQSSC.strQishu));
					string sNowQi;
					ChangeStringToA(m_chqRule.GetNextExpect(),sNowQi);
					for(int o=0; o< vecData2.size(); o++)
					{
						string strQi;
						ChangeStringToA(vecData2[o].c_str(),strQi);
						if(strQi < sNowQi)
						{

							MyMessageBox(L"订单期号有误！可能原因，该期已经停止投注！");
							//OnBnClickedBtnClsList();
							m_DlgStatus.HideStatusWindow();
							return ;
						}
						memcpy(TouzhuCQSSC.strQishu[o],strQi.c_str(),strQi.length());
						TouzhuCQSSC.nBeitou[o] = _ttoi(vecData1[o].c_str());;
					}


					byte pDestBuffer[102400];
					int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

					int nSendSize = 0;
					TouzhuCQSSC.nEnd = 1;
					if(m_nZongZhuShu<=0)
					{
						m_DlgStatus.HideStatusWindow();
						//OnBnClickedBtnClsList();
						return;
					}

					TouzhuCQSSC.nZhushu = m_nZongZhuShu;
					int nSize = sizeof(TouzhuCQSSC.strHaoma);
					DWORD dwTick=GetTickCount();

					while (nSendSize < nLen)
					{
						while ((GetTickCount() - dwTick ) < 30000)
						{
							MSG msg;
							if(GetMessage (&msg, NULL, 0, 0))
							{
								TranslateMessage (&msg) ;

								DispatchMessage (&msg) ;

							}
							if(m_bCanSend)
								break;

						}
						int nHaoMaLen = 0;
						nHaoMaLen = min(nSize,nLen-nSendSize);
						TouzhuCQSSC.nHaoMaLen = nHaoMaLen;	//nLen;

						TouzhuCQSSC.nIndex = nSendCount;
						memset(TouzhuCQSSC.strHaoma, 0, sizeof(TouzhuCQSSC.strHaoma));
						memcpy((byte*)TouzhuCQSSC.strHaoma, pDestBuffer+nSendSize, nSize);


						if(nSendSize + nSize >= nLen)
							TouzhuCQSSC.nEnd = 0;

						CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
						if(pPlatformFrame!=NULL)
							pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_ZHUIHAO,&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
						m_bShowFail=true;
						nSendSize += nHaoMaLen;
						m_bCanSend = false;
						nSendCount++;
					}

				}

			}

			m_nTzhSign++;


			SetTimer(TimerStatus,60000,NULL);
			m_strHaoma = _T("");
			//OnBnClickedBtnClsList();


		}
		return;


	}

	if(nSendType == 5)
	{
		m_bSetBonus = true;
		if(m_bSetBonus)
		{
			m_bSetBonus=false;
			CMD_GP_SetUserBonus SetUserBonus;
			ZeroMemory(&SetUserBonus,sizeof(SetUserBonus));

			SetUserBonus.n_t_userid = theAccount.user_id;
			SetUserBonus.n_t_type_id = CZXingYun28;
			SetUserBonus.n_t_kind_id = m_gameKind;
			SetUserBonus.f_t_bonus = m_fBonus;
			SetUserBonus.f_bonuspercent = m_fPercent;

			//发送数据
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_USER_BONUS,&SetUserBonus,sizeof(SetUserBonus));
		}
		return;

	}

	if(nSendType == 6)
	{
		m_bGetUserFandian = true;
		//m_btnLock.EnableTextButton(true);
		if(m_bGetUserFandian)
		{
			m_bGetUserFandian=false;
			CMD_GP_GetUserFandian GetUserFandian;
			ZeroMemory(&GetUserFandian,sizeof(GetUserFandian));

			GetUserFandian.n_t_userid = theAccount.user_id;
			GetUserFandian.n_t_type_id = CZXingYun28;
			GetUserFandian.n_t_kind_id = m_gameKind;

			//发送数据
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_FANDIAN,&GetUserFandian,sizeof(GetUserFandian));
		}

		return;
	}
	if(nSendType == 11)
	{
		CMD_GP_QueryStatusLottery QueryStatusLottery;
		ZeroMemory(&QueryStatusLottery,sizeof(QueryStatusLottery));

		QueryStatusLottery.n_t_kindid = CZXingYun28;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_STATUS_LOTTERY,&QueryStatusLottery,sizeof(QueryStatusLottery));
	}
	return;
}



bool CXingYun28::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}



//读取事件
bool CXingYun28::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_USER_FANDIAN_RET:
			{

				return true;

			}
		case SUB_GP_TOUZHU_CONTINUE_RET:
			{
				m_bCanSend = true;
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
		case SUB_GP_QUERY_TIME_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_GetSystemTime));
				if(wDataSize < sizeof(CMD_GR_GetSystemTime)) return false;

				CMD_GR_GetSystemTime* pSystemTime = (CMD_GR_GetSystemTime*)pData;

				theTime = pSystemTime->ServerTime;

				//SendToServer(3);

				return true;
			}
		case SUB_GP_SET_USER_BONUS_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_SetUserBonusRet));
				if(wDataSize < sizeof(CMD_GR_SetUserBonusRet)) return false;

				CMD_GR_SetUserBonusRet* pSetUserBonus = (CMD_GR_SetUserBonusRet*)pData;
				int nResult = pSetUserBonus->n_t_result;

				if(nResult==0)
				{
					SendToServer(6);
					SetLockText(m_fBonus, m_fPercent);
					MyMessageBox(L"返点数据修改成功！", L"温馨提示");
				}
				else if(nResult == 1)
				{
					MyMessageBox(L"你修改的返点不符合！", L"提示");
				}
				else if(nResult == 2)
				{
					MyMessageBox(L"此玩法不支持修改返点！", L"提示");
				}
				else
				{
					MyMessageBox(L"返点修改失败！",L"提示");
				}

				return true;
			}
		case SUB_GP_QUERY_GAME_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GP_QueryLotResult)==0);
				if(wDataSize % sizeof(CMD_GP_QueryLotResult)!=0)
					return false;

				m_kjXinxiCont = 0;
				WORD wCount = wDataSize/sizeof(CMD_GP_QueryLotResult);
				wCount = __min(wCount,Kj_XinXi_Count);

				for(int i = 0;i < wCount;i++)
				{
					CMD_GP_QueryLotResult* pQueryLotResult  =  ((CMD_GP_QueryLotResult*)pData+i);

					CMD_GP_QueryLotResult QueryLotResult;
					ZeroMemory(&QueryLotResult,sizeof(QueryLotResult));

					QueryLotResult.wKindID = pQueryLotResult->wKindID;
					lstrcpyn(QueryLotResult.wPeriod,pQueryLotResult->wPeriod,CountArray(QueryLotResult.wPeriod));
					lstrcpyn(QueryLotResult.szLotNum,pQueryLotResult->szLotNum,CountArray(QueryLotResult.szLotNum));
					lstrcpyn(QueryLotResult.szShijian,pQueryLotResult->szShijian,CountArray(QueryLotResult.szShijian));

					_tcscpy_s(m_kjXinxi[m_kjXinxiCont].qihao, KJ_QIHAO_LENGTH, QueryLotResult.wPeriod);
					_tcscpy_s(m_kjXinxi[m_kjXinxiCont].haoma, KJ_HAOMA_LENGTH, QueryLotResult.szLotNum);

					_tcscpy_s(m_kjXinxi[m_kjXinxiCont].shijian, KJ_HAOMA_LENGTH, QueryLotResult.szShijian);
					m_kjXinxiCont++;
				}


				if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
				{
					//第一次进入
					if(m_lastExpect[0] == 0)
						_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);
					m_bKaiJiangzhong=false;											//开奖结束
					long secDiff = m_chqRule.GetKjShjDiff();

					KillTimer(timer_id_kaijiangzhong);
				}

				if(m_bKaiJiangzhong)
				{
					CRect rcCnt;
					rcCnt.left = rcRedraw.left+200;
					rcCnt.right = rcRedraw.right;
					rcCnt.top = rcRedraw.top;
					rcCnt.bottom = rcRedraw.bottom;
					RedrawWindow(&rcCnt,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
				}
				else
				{
					RedrawWindow(&rcRedraw,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
				}

				return true;
			}
		case SUB_GP_TOUZHU_CQSSC_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_TouzhuRes));
				if(wDataSize < sizeof(CMD_GR_TouzhuRes)) return false;

				KillTimer(TimerStatus);
				CMD_GR_TouzhuRes* pTouzhuRes = (CMD_GR_TouzhuRes*)pData;

				LONG lResult = pTouzhuRes->lResult;

				m_bCanSend = true;

				m_DlgStatus.HideStatusWindow();
				if (lResult == 0)
				{
					if((pTouzhuRes->nSign != m_nTzhSign1))
					{
						m_nTzhSign1 = pTouzhuRes->nSign;
						m_listChqSSC.DeleteAllItems();
						MyMessageBox(_T("成功投注！"), _T("【祝君好运 投注成功】"));
						SendToServer(7);						

					}
				}
				else if(lResult == 1)
				{
					MyMessageBox(_T("注数错误，或者投注内容不正确！"));
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("当前期已封单，停止销售！"));
				}
				else if(lResult == 3)
				{
					MyMessageBox(_T("该期号已经过期，投注失败！"));
				}
				else if(lResult == 4)
				{
					MyMessageBox(_T("由于期号出现错误，投注失败！"));
				}
				else if(lResult == 5)
				{
					MyMessageBox(_T("无法获取玩法对应的奖金，投注失败！"));
				}
				else if(lResult == 6)
				{
					MyMessageBox(_T("其他错误，有订单没有成功提交！"));
				}
				else if(lResult == 7)
				{
					MyMessageBox(_T("其他错误，有订单没有成功提交！"));
				}
				else if(lResult == 8)
				{
					MyMessageBox(_T("投注过程返回提示：余额不足"));
				}
				else if(lResult == 9)
				{
					MyMessageBox(_T("其他错误，有订单没有成功提交！"));
				}
				else if(lResult == 10)
				{
					MyMessageBox(_T("对不起，您的账户没有投注权限！"));
				}
				else if(lResult == 11)
				{
					MyMessageBox(_T("注数不正确，投注失败！"));
				}
				else if(lResult == 12)
				{
					MyMessageBox(_T("对不起，该玩法维护中，请选择其他玩法进行投注！"));
				}
				else if(lResult == 13)
				{
					MyMessageBox(_T("该彩种暂停销售！"));
				}
				else if(lResult>=20 && lResult <= 30)
				{
					MyMessageBox(_T("该账户没有投注权限！"));

				}
				else if(lResult == 111)
				{
					CInformation information;
					int nResult = information.ShowMessageBox(_T("投注未成功！由于您密码设置过于简单，请设置复杂登陆密码和取款密码！"),MB_ICONWARNING,30);
					if(nResult==1)
					{
						CWnd* pParent = GetParent();
						if(pParent!=NULL)
						{
							pParent->SendMessage(IDM_SHOW_XGMM,0,0);
						}
					}		
				}
				else if(lResult > 18)
				{
					CString strLog;
					int nFandian = 1900+lResult*2;
					strLog.Format(L"抱歉返点在%d以上的玩家不能投注！",nFandian);
					MyMessageBox(strLog);
				}
				else
				{
					if(	m_bShowFail == true)
					{
						m_bShowFail = false;

						MyMessageBox(_T("有订单未成功提交！"));
					}
				}


				return true;
			}
		case SUB_GP_GET_LAST_YUE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLastYueRet));
				if(wDataSize!=sizeof(CMD_GP_GetLastYueRet)) return false;

				CMD_GP_GetLastYueRet* pCountRet = (CMD_GP_GetLastYueRet*)pData;

				theAccount.yue = pCountRet->f_t_yue;
				theAccount.dongjie = pCountRet->f_t_dongjie;

				CWnd* pParent = GetParent();
				if(pParent!=NULL)
				{
					pParent->SendMessage(IDM_UPDATE_ACCOUNT,0,0);
				}
				return true;
			}

		case SUB_GP_GET_CP_USER_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_GetCpUserInfoRet));
				if(wDataSize < sizeof(CMD_GR_GetCpUserInfoRet)) return false;

				CMD_GR_GetCpUserInfoRet* pGetCpUser = (CMD_GR_GetCpUserInfoRet*)pData;

				lstrcpyn(theAccount.account,pGetCpUser->s_t_account,sizeof(theAccount.account));
				theAccount.type = pGetCpUser->n_t_type;
				theAccount.fandian = pGetCpUser->f_t_fandian;
				theAccount.yue = pGetCpUser->f_t_yue;
				theAccount.dongjie = pGetCpUser->f_t_dongjie;
				return true;
			}
		case SUB_GP_QUERY_STATUS_LOTTERY_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_QueryStatusLotteryRet));
				if(wDataSize!=sizeof(CMD_GP_QueryStatusLotteryRet)) return false;

				CMD_GP_QueryStatusLotteryRet* pLogRet = (CMD_GP_QueryStatusLotteryRet*)pData;

				if(pLogRet->n_t_kindid != CZXingYun28)
					return true;

				m_cbIfTs = pLogRet->c_t_ifts;
				int nStartQihao = 0;
				nStartQihao = _ttoi(pLogRet->s_t_expect);
				CTime ctm(pLogRet->n_t_shijian);

				m_chqRule.SetStartQihao(nStartQihao,ctm);

				return true;
			}

		}
	}
	return true;

	//错误断言
	ASSERT(FALSE);

	return true;
}


void CXingYun28::SetLockText(float fBonus, float fBonusPercent)
{
// 	const float EPSINON = 0.00001;
// 	if((fBonus >= - EPSINON) && (fBonus <= EPSINON))
// 	{
// 	}
// 	else
// 	{
// 		CString strText;
// 		strText.Format(L"%.2lf/%.2lf", fBonus, fBonusPercent);
// 		m_btnLock.SetWindowText(strText + _T("%"));
// 	}
}

//上期开奖号码
 void CXingYun28::DrawLastKjHaoma(CDC* pDC, Graphics& graphics)
 {
 	long secDiff = m_chqRule.GetKjShjDiff();
	CString strLog;
	strLog.Format(L"XY28 secDiff:%d",secDiff);
	OutputDebugString(strLog);
 	if(IsWindowVisible() && secDiff > 60 && secDiff <=65)
 	{
 		CPlatformFrame* pPlatforFrame = CPlatformFrame::GetInstance();
 		if(pPlatforFrame->m_bSound)
 			sndPlaySound(MAKEINTRESOURCE(IDR_CLOCK),  SND_RESOURCE |SND_ASYNC);
 	}
 
 	if(IsWindowVisible() && secDiff == 61)
 	{
 	//	if(m_listChqSSC.GetItemCount() != 0)
 		{
 			CWnd *pParent = GetParent();
 			if(pParent!=NULL)
 			{
 				pParent->PostMessage(IDM_TANCHUANG,0,0);
 			}
 
 		}
 	}
 	int nTimeLeft = GetKjShjDiff();
 	//时间用尽了，显示开奖动画;
 	//m_kjXinxi 存的是本地最新开奖号码
 	//更新上期的期号
 	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
 	{
 		_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);
		m_bKaiJiangzhong=false;	
 	}
 
 	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
 	CFont* pOldFont = pDC->SelectObject(&m_KjInfoFont1);
 	COLORREF oldTextClr = pDC->SetTextColor(RGB(255, 255, 255));

	CString strTopQi;
	strTopQi.Format(_T("北京 NO.%s"),m_chqRule.GetNextExpect());
 	pDC->DrawText(strTopQi, &rcCzh, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	CString strYg;
	CRect rcYuGao1;

	rcYuGao1.CopyRect(rcCzh);

	pDC->SelectObject(&m_KjInfoFont);
	if(m_cbIfTs == 1)
	{
		strYg.Format(_T("暂停销售"));
	}
	else if(secDiff<60)
		strYg.Format(_T("封单时间：%s"), m_chqRule.GetKjShjDiffDesc());
	else
		strYg.Format(_T("截止时间：%s"), m_chqRule.GetKjShjDiffDesc());
	pDC->SetTextColor(RGB(255, 255, 255));
	rcYuGao1.left +=30;
	rcYuGao1.top = rcYuGao1.top;
	rcYuGao1.bottom = rcYuGao1.bottom;
	rcYuGao1.right  +=10;


	pDC->DrawText(strYg, &rcYuGao1,DT_CENTER|DT_VCENTER|DT_SINGLELINE);


	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(&pOldFont);
	pDC->SetBkMode(oldBkMode);
 	CString str = m_kjXinxi[0].haoma;

 		int index = 0;
 		int Sum = 0;
 		int SumCount = 0;
 		int Sumall = 0;
 		CString strTemp;
 		for (int i=0; i<40; i++) 
 		{
 			if (iswdigit(str.GetAt(i)) && (i%2!=0)) 
 			{
 				strTemp=str.GetAt(i);
 				int num = _ttoi(strTemp);
 
 				if(m_bKaiJiangzhong )
 				{
 					KillTimer(timer_id_kaijiangzhong);
 					SetTimer(timer_id_kaijiangzhong, 100, NULL);
 				}
 				else
 				{
 					SumCount++;
 					Sum += num;
 					while(Sum>=10)
 						Sum = Sum%10;
 
 					if(SumCount == 6)
 					{
 						DrawBigNum(graphics, 
 							m_kjNumBig, 
 							Rect(kj_big_haoma_x + index* (m_bigNumWidth+33),kj_big_haoma_y+30,m_kjNumBig->GetWidth()/28,m_kjNumBig->GetHeight()),
 							Sum,m_kjNumBig->GetWidth()/28,m_kjNumBig->GetHeight());
 
 						Sumall+=Sum;
 
 						Sum = 0;
 						SumCount =0;
 						index ++;
 
//  						DrawBigNum(graphics, 
//  							m_kjAdd, 
//  							Rect(kj_big_haoma_x + index* (m_bigNumWidth+10),kj_big_haoma_y+30,m_bigNumWidth,m_bigNumHeight),
//  							(index/2>1?1:0),m_bigNumWidth,m_bigNumHeight);
 					//	index ++;
 					}
 				}
 			}
			else
			{
				continue;
			}
 
 		}
 
 
 		DrawBigNum(graphics, 
 			m_kjNumBig2, 
 			Rect(kj_big_haoma_x + index* (m_bigNumWidth+36),kj_big_haoma_y+30,m_bigNumWidth,m_bigNumHeight),
 			Sumall,m_bigNumWidth,m_bigNumHeight);
		if(!m_bKaiJiangzhong)
		{
				index++;
				int nImgDxdshWidth = m_imgDxDsh->GetWidth()/4;
				int nImgDxdshHeight = m_imgDxDsh->GetHeight();
				if(Sumall>13)
				{
					graphics.DrawImage(m_imgDxDsh, Rect(kj_big_haoma_x + index* (m_bigNumWidth+33),kj_big_haoma_y+13,m_bigNumWidth,m_bigNumHeight), 0, 0, nImgDxdshWidth,nImgDxdshHeight, UnitPixel);
				}
				else
				{
					graphics.DrawImage(m_imgDxDsh, Rect(kj_big_haoma_x + index* (m_bigNumWidth+33),kj_big_haoma_y+13,m_bigNumWidth,m_bigNumHeight), nImgDxdshWidth, 0,nImgDxdshWidth,nImgDxdshHeight, UnitPixel);
				}

				if(Sumall%2 == 1)
				{
					graphics.DrawImage(m_imgDxDsh, Rect(kj_big_haoma_x + index* (m_bigNumWidth+33),kj_big_haoma_y+11+nImgDxdshHeight,m_bigNumWidth,m_bigNumHeight), nImgDxdshWidth*2, 0, nImgDxdshWidth,nImgDxdshHeight, UnitPixel);
				}
				else
				{
					graphics.DrawImage(m_imgDxDsh, Rect(kj_big_haoma_x + index* (m_bigNumWidth+33),kj_big_haoma_y+13+nImgDxdshHeight,m_bigNumWidth,m_bigNumHeight), nImgDxdshWidth*3, 0, nImgDxdshWidth,nImgDxdshHeight, UnitPixel);
				}
		}
		 
 }
 
//开奖号码
void CXingYun28::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(250 ,250 ,250));

	BYTE cbDxdshType = 0;  //大1  小2  单3  双4
	int nHistoryWidth = m_imgHistory->GetWidth()/4;
	int nHistoryHeight = m_imgHistory->GetHeight();
	int nHang = 0;
	int nLie = 0;
	for(int i = 0;i < 50;i++)
	{
		if(nHang>=9)
			break;
		CString str = m_kjXinxi[i].qihao;
		if(str.IsEmpty())
		{
			continue;
		}


		str = m_kjXinxi[i].haoma;
		int index = 0;
		int Sum = 0;
		int SumCount = 0;
		int Sumall = 0;
		CString strTemp;
		for (int i=0; i<40; i++) 
		{
			if (iswdigit(str.GetAt(i)) && (i%2!=0)) 
			{
				strTemp=str.GetAt(i);
				int num = _ttoi(strTemp);

				SumCount++;
				Sum += num;
				while(Sum>=10)
					Sum = Sum%10;

				if(SumCount == 6)
				{

					Sumall+=Sum;

					Sum = 0;
					SumCount =0;
				}
			}
			else
			{
				continue;
			}

		}

		if(m_bDx)
		{
			if(Sumall>13)
			{
				if(cbDxdshType!=0 && cbDxdshType!=1)
				{
					nHang++;
					nLie = 0;
				}
				else if(cbDxdshType!=0 && cbDxdshType==1)
				{
					nLie++;
					if(nLie>=4)
					{
						nLie = 0;
						nHang++;
					}

				}
				DrawDxdshNum(graphics,m_imgHistory,Rect(hitory_x + nHang*(nHistoryWidth+3),hitory_y + nLie*(nHistoryHeight+3),nHistoryWidth,nHistoryHeight),0);
				cbDxdshType = 1;
			}
			else
			{
				if(cbDxdshType!=0 && cbDxdshType!=2)
				{
					nHang++;
					nLie = 0;
				}
				else if(cbDxdshType!=0 && cbDxdshType==2)
				{
					nLie++;
					if(nLie>=4)
					{
						nLie = 0;
						nHang++;
					}
				}
				DrawDxdshNum(graphics,m_imgHistory,Rect(hitory_x+ nHang*(nHistoryWidth+3),hitory_y+ nLie*(nHistoryHeight+3),nHistoryWidth,nHistoryHeight),1);
				cbDxdshType = 2;
			}
		}
		else
		{
			if(Sumall%2 == 1)
			{
				if(cbDxdshType!=0 && cbDxdshType!=3)
				{
					nHang++;
					nLie = 0;
				}
				else if(cbDxdshType!=0 && cbDxdshType==3)
				{
					nLie++;
					if(nLie>=4)
					{
						nLie = 0;
						nHang++;
					}
				}

				DrawDxdshNum(graphics,m_imgHistory,Rect(hitory_x+ nHang*(nHistoryWidth+3),hitory_y+ nLie*(nHistoryHeight+3),nHistoryWidth,nHistoryHeight),2);
				cbDxdshType = 3;
			}
			else
			{
				if(cbDxdshType!=0 && cbDxdshType!=4)
				{
					nHang++;
					nLie = 0;
				}
				else if(cbDxdshType!=0 && cbDxdshType==4)
				{
					nLie++;
					if(nLie>=4)
					{
						nLie = 0;
						nHang++;
					}
				}
				DrawDxdshNum(graphics,m_imgHistory,Rect(hitory_x+ nHang*(nHistoryWidth+3),hitory_y+ nLie*(nHistoryHeight+3),nHistoryWidth,nHistoryHeight),3);
				cbDxdshType = 4;
			}
		}

	}



	for (int cnt=0; cnt<3; cnt++) 
	{
		CString str = m_kjXinxi[cnt].qihao;
		if(str.IsEmpty())
		{
			continue;
		}

		CRect rcText(kj_small_haoma_x - 50-10, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 10, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[cnt].haoma;
		int index = 0;
		int Sum = 0;
		int SumCount = 0;
		int Sumall = 0;
		CString strTemp;
		for (int i=0; i<40; i++) 
		{
			if (iswdigit(str.GetAt(i)) && (i%2!=0)) 
			{
				strTemp=str.GetAt(i);
				int num = _ttoi(strTemp);

				SumCount++;
				Sum += num;
				while(Sum>=10)
					Sum = Sum%10;

				if(SumCount == 6)
				{
					DrawBigNum(graphics, 
						m_kjNumSmall, 
						Rect(kj_small_haoma_x + index* (m_smallNumWidth+2)+5,kj_small_haoma_y+ cnt * (m_smallNumHeight + kj_small_haoma_row_span),m_smallNumWidth,m_smallNumHeight),
						Sum,m_smallNumWidth,m_smallNumHeight);

					Sumall+=Sum;

					Sum = 0;
					SumCount =0;
					index ++;

// 					DrawBigNum(graphics, 
// 						m_kjAdd1, 
// 						Rect(kj_small_haoma_x + index* (m_smallNumWidth+3),kj_small_haoma_y+ cnt * (m_smallNumHeight + kj_small_haoma_row_span),m_smallNumWidth,m_smallNumHeight),
// 						(index/2>1?1:0),m_smallNumWidth,m_smallNumHeight);
					index ++;
				}
			}
			else
			{
				continue;
			}

		}


		DrawBigNum(graphics, 
			m_kjNumSmall1, 
			Rect(kj_small_haoma_x + index* (m_smallNumWidth),kj_small_haoma_y+ cnt * (m_smallNumHeight + kj_small_haoma_row_span),m_kjNumSmall1->GetWidth()/80,m_kjNumSmall1->GetHeight()),
			Sumall-1,m_kjNumSmall1->GetWidth()/80,m_kjNumSmall1->GetHeight());

	}

	pDC->DrawText(strText, &rcZongJinE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}


void CXingYun28::DrawZongZhushuJinE(CDC* pDC)
{
	return;
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_zongFont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(160, 8, 8));
//	strText.Format(_T("你选择了%d注"), m_singlezhushu);

	CRect rcZhu;

	rcZhu.CopyRect(rcAllZongzhushu);
	rcZhu.top += 40;
	rcZhu.bottom += 48;
	strText.Format(_T("%.3lf"),m_zongJine);
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}

long CXingYun28::GetKjShjDiff()
{
	long secDiff = m_chqRule.GetKjShjDiff();

	CString strLog;
	strLog.Format(L"XY28KAI secDiff:%d,qispan:%d",secDiff,m_chqRule.GetQiSpan());
	OutputDebugString(strLog);
	if (secDiff <= 60 )
	{
		return 0;
	}
	else if(secDiff > (m_chqRule.GetQiSpan() -2)||(secDiff<2))
	{
		m_bKaiJiangzhong=true;
		return 0;
	}
	return secDiff;
}

void CXingYun28::DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num,int nWidth,int nHeight)
{
	graphics.DrawImage(img, rc_dest, num*nWidth, 0, nWidth, nHeight, UnitPixel);
}

void CXingYun28::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);
}
void CXingYun28::DrawDxdshNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	int nWidth = m_imgHistory->GetWidth()/4;
	int nHeight = m_imgHistory->GetHeight();
	graphics.DrawImage(img, rc_dest, num*nWidth, 0, nWidth, nHeight, UnitPixel);
}
void CXingYun28::ShowCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_SHOW);
	}
}
void CXingYun28::HideCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}
void CXingYun28::CheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(true);
}
void CXingYun28::UnCheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(false);
}
bool CXingYun28::IsNumBtnCheck(CNumerButton& btn)
{
	return btn.IsSelect();
}

void CXingYun28::ResetAllNums(void)
{
	m_singlezhushu = 0;
	UnCheckNumBtn(m_btnHeZhi_0);
	UnCheckNumBtn(m_btnHeZhi_1);
	UnCheckNumBtn(m_btnHeZhi_2);
	UnCheckNumBtn(m_btnHeZhi_3);
	UnCheckNumBtn(m_btnHeZhi_4);
	UnCheckNumBtn(m_btnHeZhi_5);
	UnCheckNumBtn(m_btnHeZhi_6);
	UnCheckNumBtn(m_btnHeZhi_7);
	UnCheckNumBtn(m_btnHeZhi_8);
	UnCheckNumBtn(m_btnHeZhi_9);
	UnCheckNumBtn(m_btnHeZhi_10);
	UnCheckNumBtn(m_btnHeZhi_11);
	UnCheckNumBtn(m_btnHeZhi_12);
	UnCheckNumBtn(m_btnHeZhi_13);
	UnCheckNumBtn(m_btnHeZhi_14);
	UnCheckNumBtn(m_btnHeZhi_15);
	UnCheckNumBtn(m_btnHeZhi_16);
	UnCheckNumBtn(m_btnHeZhi_17);
	UnCheckNumBtn(m_btnHeZhi_18);
	UnCheckNumBtn(m_btnHeZhi_19);
	UnCheckNumBtn(m_btnHeZhi_20);
	UnCheckNumBtn(m_btnHeZhi_21);
	UnCheckNumBtn(m_btnHeZhi_22);
	UnCheckNumBtn(m_btnHeZhi_23);
	UnCheckNumBtn(m_btnHeZhi_24);
	UnCheckNumBtn(m_btnHeZhi_25);
	UnCheckNumBtn(m_btnHeZhi_26);
	UnCheckNumBtn(m_btnHeZhi_27);


	UnCheckNumBtn(m_btnDa);
	UnCheckNumBtn(m_btnXiao);
	UnCheckNumBtn(m_btnDan);
	UnCheckNumBtn(m_btnShuang);
	UnCheckNumBtn(m_btnDaDan);
	UnCheckNumBtn(m_btnDaShuang);
	UnCheckNumBtn(m_btnXiaoDan);
	UnCheckNumBtn(m_btnXiaoShuang);

	UnCheckNumBtn(m_btnMax);
	UnCheckNumBtn(m_btnMin);

}

//隐藏玩法
void CXingYun28::HideRio()
{
// 	m_rioDuiZi.ShowWindow(SW_HIDE);
// 	m_rioShunZi.ShowWindow(SW_HIDE);
// 	m_rioBaoZi.ShowWindow(SW_HIDE);
}
//选择玩法
void CXingYun28::SelectRio(CImgRioButton &rio)
{
// 	m_rioDuiZi.SetPushed(false);
// 	m_rioShunZi.SetPushed(false);
// 	m_rioBaoZi.SetPushed(false);
// 
	rio.SetPushed(true);
}


//显示隐藏极值
void CXingYun28::HideJiZhi()
{
	m_btnMax.ShowWindow(SW_HIDE);
	m_btnMin.ShowWindow(SW_HIDE);
}
void CXingYun28::ShowJiZji()
{
	m_btnMax.ShowWindow(SW_SHOW);
	m_btnMin.ShowWindow(SW_SHOW);
}



void CXingYun28::ShowDanShiEdit()
{
	m_RenxuanKeyboardInput = true;
	m_richDanshiHaoma.ShowWindow(SW_SHOW);
}
void CXingYun28::HideDanShiEdit()
{
	m_RenxuanKeyboardInput = false;
	m_richDanshiHaoma.ShowWindow(SW_HIDE);
}

void CXingYun28::ShowDaXiaoDanShuang()
{
	m_btnDa.ShowWindow(SW_SHOW);
	m_btnXiao.ShowWindow(SW_SHOW);
	m_btnDan.ShowWindow(SW_SHOW);
	m_btnShuang.ShowWindow(SW_SHOW);
	m_btnDaDan.ShowWindow(SW_SHOW);
	m_btnDaShuang.ShowWindow(SW_SHOW);
	m_btnXiaoDan.ShowWindow(SW_SHOW);
	m_btnXiaoShuang.ShowWindow(SW_SHOW);
}

void CXingYun28::HideDaXiaoDanShuang()
{
	m_btnDa.ShowWindow(SW_HIDE);
	m_btnXiao.ShowWindow(SW_HIDE);
	m_btnDan.ShowWindow(SW_HIDE);
	m_btnShuang.ShowWindow(SW_HIDE);
	m_btnDaDan.ShowWindow(SW_HIDE);
	m_btnDaShuang.ShowWindow(SW_HIDE);
	m_btnXiaoDan.ShowWindow(SW_HIDE);
	m_btnXiaoShuang.ShowWindow(SW_HIDE);
}
void CXingYun28::ShowBJNums()
{
}
void CXingYun28::HideBJNums()
{


}
void CXingYun28::HideHeZhiNums()
{
	m_btnHeZhi_0.ShowWindow(SW_HIDE);
	m_btnHeZhi_1.ShowWindow(SW_HIDE);
	m_btnHeZhi_2.ShowWindow(SW_HIDE);
	m_btnHeZhi_3.ShowWindow(SW_HIDE);
	m_btnHeZhi_4.ShowWindow(SW_HIDE);
	m_btnHeZhi_5.ShowWindow(SW_HIDE);
	m_btnHeZhi_6.ShowWindow(SW_HIDE);
	m_btnHeZhi_7.ShowWindow(SW_HIDE);
	m_btnHeZhi_8.ShowWindow(SW_HIDE);
	m_btnHeZhi_9.ShowWindow(SW_HIDE);
	m_btnHeZhi_10.ShowWindow(SW_HIDE);
	m_btnHeZhi_11.ShowWindow(SW_HIDE);
	m_btnHeZhi_12.ShowWindow(SW_HIDE);
	m_btnHeZhi_13.ShowWindow(SW_HIDE);
	m_btnHeZhi_14.ShowWindow(SW_HIDE);
	m_btnHeZhi_15.ShowWindow(SW_HIDE);
	m_btnHeZhi_16.ShowWindow(SW_HIDE);
	m_btnHeZhi_17.ShowWindow(SW_HIDE);
	m_btnHeZhi_18.ShowWindow(SW_HIDE);
	m_btnHeZhi_19.ShowWindow(SW_HIDE);
	m_btnHeZhi_20.ShowWindow(SW_HIDE);
	m_btnHeZhi_21.ShowWindow(SW_HIDE);
	m_btnHeZhi_22.ShowWindow(SW_HIDE);
	m_btnHeZhi_23.ShowWindow(SW_HIDE);
	m_btnHeZhi_24.ShowWindow(SW_HIDE);
	m_btnHeZhi_25.ShowWindow(SW_HIDE);
	m_btnHeZhi_26.ShowWindow(SW_HIDE);
	m_btnHeZhi_27.ShowWindow(SW_HIDE);
}
void CXingYun28::ShowHezhiNums()
{
	m_btnHeZhi_0.ShowWindow(SW_SHOW);
	m_btnHeZhi_1.ShowWindow(SW_SHOW);
	m_btnHeZhi_2.ShowWindow(SW_SHOW);
	m_btnHeZhi_3.ShowWindow(SW_SHOW);
	m_btnHeZhi_4.ShowWindow(SW_SHOW);
	m_btnHeZhi_5.ShowWindow(SW_SHOW);
	m_btnHeZhi_6.ShowWindow(SW_SHOW);
	m_btnHeZhi_7.ShowWindow(SW_SHOW);
	m_btnHeZhi_8.ShowWindow(SW_SHOW);
	m_btnHeZhi_9.ShowWindow(SW_SHOW);
	m_btnHeZhi_10.ShowWindow(SW_SHOW);
	m_btnHeZhi_11.ShowWindow(SW_SHOW);
	m_btnHeZhi_12.ShowWindow(SW_SHOW);
	m_btnHeZhi_13.ShowWindow(SW_SHOW);
	m_btnHeZhi_14.ShowWindow(SW_SHOW);
	m_btnHeZhi_15.ShowWindow(SW_SHOW);
	m_btnHeZhi_16.ShowWindow(SW_SHOW);
	m_btnHeZhi_17.ShowWindow(SW_SHOW);
	m_btnHeZhi_18.ShowWindow(SW_SHOW);
	m_btnHeZhi_19.ShowWindow(SW_SHOW);
	m_btnHeZhi_20.ShowWindow(SW_SHOW);
	m_btnHeZhi_21.ShowWindow(SW_SHOW);
	m_btnHeZhi_22.ShowWindow(SW_SHOW);
	m_btnHeZhi_23.ShowWindow(SW_SHOW);
	m_btnHeZhi_24.ShowWindow(SW_SHOW);
	m_btnHeZhi_25.ShowWindow(SW_SHOW);
	m_btnHeZhi_26.ShowWindow(SW_SHOW);
	m_btnHeZhi_27.ShowWindow(SW_SHOW);
}


//调整 单选框
void CXingYun28::AdjustRadio()
{
// 	CWnd *pWnd;
// 
// 
// 	pWnd = GetDlgItem(IDC_RIO_DUIZI);
// 	if (pWnd != NULL) 
// 	{
// 		pWnd->SetWindowPos(NULL, wanfa_sel_x,				wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
// 	}
// 	pWnd = GetDlgItem(IDC_RIO_SHUNZI);
// 	if (pWnd != NULL) 
// 	{
// 		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
// 	}
// 	pWnd = GetDlgItem(IDC_RIO_BAOZI);
// 	if (pWnd != NULL) 
// 	{
// 		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*2, wanfa_sel_y,wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
// 	}
// 

}
//玩法按钮调整
void CXingYun28::AdjustWanFa()
{
	if (m_btnRenXuan.GetSafeHwnd() != NULL) 
	{
		m_btnRenXuan.SetWindowPos(NULL, wanfa_x, wanfa_y, m_btnRenXuan.Width(), m_btnRenXuan.Height(), SWP_NOZORDER);
		m_btnRenXuan.ShowWindow(SW_HIDE);
	}
	if (m_btnTeMa.GetSafeHwnd() != NULL) 
	{
		m_btnTeMa.SetWindowPos(NULL, wanfa_x , wanfa_y , m_btnTeMa.Width(), m_btnTeMa.Height(), SWP_NOZORDER);
		m_btnTeMa.ShowWindow(SW_HIDE);
	}
	if (m_btnDaXiaoDanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnDaXiaoDanShuang.SetWindowPos(NULL, wanfa_x + wanfa_col_width*1, wanfa_y, m_btnDaXiaoDanShuang.Width(), m_btnDaXiaoDanShuang.Height(), SWP_NOZORDER);
		m_btnDaXiaoDanShuang.ShowWindow(SW_HIDE);
	}
	if (m_btnJiZhi.GetSafeHwnd() != NULL) 
	{
		m_btnJiZhi.SetWindowPos(NULL, wanfa_x+wanfa_col_width*2+16, wanfa_y, m_btnJiZhi.Width(), m_btnJiZhi.Height(), SWP_NOZORDER);
		m_btnJiZhi.ShowWindow(SW_HIDE);
	}
	if (m_btnFunny.GetSafeHwnd() != NULL) 
	{
		m_btnFunny.SetWindowPos(NULL, wanfa_x+wanfa_col_width*3+16, wanfa_y, m_btnFunny.Width(), m_btnFunny.Height(), SWP_NOZORDER);
		m_btnFunny.ShowWindow(SW_HIDE);
	}

	if (m_btnTextDx.GetSafeHwnd() != NULL) 
	{
		m_btnTextDx.SetWindowPos(NULL, text_dx_x, text_dx_y, m_btnTextDx.Width(), m_btnTextDx.Height(), SWP_NOZORDER);
	}
	if (m_btnTextDsh.GetSafeHwnd() != NULL) 
	{
		m_btnTextDsh.SetWindowPos(NULL,text_dx_x, text_dx_y+2+ m_btnTextDx.Height(), m_btnTextDsh.Width(), m_btnTextDsh.Height(), SWP_NOZORDER);
	}
}

//按钮调整：删除，清零，倍投，投注，追号
void CXingYun28::AdjustNumView()
{

	if(m_btnDelSel.GetSafeHwnd() != NULL) 
	{
		m_btnDelSel.SetWindowPos(NULL, btn_del_sel_x, btn_del_sel_y, m_btnDelSel.Width(), m_btnDelSel.Height(), SWP_NOZORDER);
		//m_btnDelSel.ShowWindow(SW_HIDE);
	}

	if(m_btnSave.GetSafeHwnd() != NULL) 
	{
		m_btnSave.SetWindowPos(NULL, btn_del_sel_x, btn_del_sel_y+m_btnDelSel.Height()+5, m_btnSave.Width(), m_btnSave.Height(), SWP_NOZORDER);
	}

	if(m_btnClsList.GetSafeHwnd() != NULL) 
	{
		m_btnClsList.SetWindowPos(NULL, btn_cls_list_x, btn_cls_list_y, m_btnClsList.Width(), m_btnClsList.Height(), SWP_NOZORDER);
		//m_btnClsList.ShowWindow(SW_HIDE);
	}

	if (m_listChqSSC.GetSafeHwnd() != NULL) 
	{
		m_listChqSSC.SetWindowPos(NULL, list_haomao_x, list_haomao_y, list_haomao_width, list_haomao_height, SWP_NOZORDER);
		m_listChqSSC.ShowWindow(SW_HIDE);
	}

	if(m_editBeiTou.GetSafeHwnd() != NULL) 
	{
		m_editBeiTou.SetWindowPos(NULL, edit_beitou_x, edit_beitou_y, edit_beitou_width, edit_beitou_height, SWP_NOZORDER);
		//m_editBeiTou.ShowWindow(SW_HIDE);
	}

	if(m_btnTouZhu.GetSafeHwnd() != NULL) 
	{
		m_btnTouZhu.SetWindowPos(NULL, btn_touzhu_x, btn_touzhu_y, m_btnTouZhu.Width(), m_btnTouZhu.Height(), SWP_NOZORDER);
	}

	if(m_btnZhuiHao.GetSafeHwnd() != NULL) 
	{
		m_btnZhuiHao.SetWindowPos(NULL, btn_zhuihao_x, btn_zhuihao_y, m_btnZhuiHao.Width(), m_btnZhuiHao.Height(), SWP_NOZORDER);
		m_btnZhuiHao.ShowWindow(SW_HIDE);
	}

	int nSpanJetton = 95;
	int nJettonWidth = 88;
	int nJettonHeight = 91;
	if(m_btnJetton1.GetSafeHwnd() != NULL) 
	{
		m_btnJetton1.SetWindowPos(NULL, btn_jetton_x, btn_jetton_y,nJettonWidth, nJettonHeight, SWP_NOZORDER);
	}
	if(m_btnJetton2.GetSafeHwnd() != NULL) 
	{
		m_btnJetton2.SetWindowPos(NULL, btn_jetton_x + nSpanJetton, btn_jetton_y,nJettonWidth,nJettonHeight, SWP_NOZORDER);
	}
	if(m_btnJetton3.GetSafeHwnd() != NULL) 
	{
		m_btnJetton3.SetWindowPos(NULL, btn_jetton_x + nSpanJetton*2, btn_jetton_y, nJettonWidth, nJettonHeight, SWP_NOZORDER);
	}
	if(m_btnJetton4.GetSafeHwnd() != NULL) 
	{
		m_btnJetton4.SetWindowPos(NULL, btn_jetton_x + nSpanJetton*3, btn_jetton_y,nJettonWidth, nJettonHeight, SWP_NOZORDER);
	}
	if(m_btnJetton5.GetSafeHwnd() != NULL) 
	{
		m_btnJetton5.SetWindowPos(NULL, btn_jetton_x  + nSpanJetton*4, btn_jetton_y, nJettonWidth, nJettonHeight, SWP_NOZORDER);
	}
}
//单式调整
void CXingYun28::AdjustDanShi()
{
	if (m_richDanshiHaoma.GetSafeHwnd() != NULL) 
	{
		m_richDanshiHaoma.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
		m_richDanshiHaoma.ShowWindow(SW_HIDE);
	}

	int nSpanJetton = 95;
	int nJettonWidth = 88;
	int nJettonHeight = 91;
	int nySpan = btn_jetton_y+nJettonHeight+2;
	int nEditHeight = 20;
	if(m_editJetton1.GetSafeHwnd() != NULL)
	{
		m_editJetton1.SetWindowPos(NULL,btn_jetton_x, nySpan,nJettonWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editJetton2.GetSafeHwnd() != NULL)
	{
		m_editJetton2.SetWindowPos(NULL,btn_jetton_x + nSpanJetton, nySpan,nJettonWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editJetton3.GetSafeHwnd() != NULL)
	{
		m_editJetton3.SetWindowPos(NULL,btn_jetton_x + nSpanJetton*2, nySpan,nJettonWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editJetton4.GetSafeHwnd() != NULL)
	{
		m_editJetton4.SetWindowPos(NULL,btn_jetton_x + nSpanJetton*3, nySpan,nJettonWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editJetton5.GetSafeHwnd() != NULL)
	{
		m_editJetton5.SetWindowPos(NULL,btn_jetton_x + nSpanJetton*4, nySpan,nJettonWidth, nEditHeight, SWP_NOZORDER);
	}
}

//加注按钮
void CXingYun28::AdjustAdd()
{
	if (m_btnChqAdd.GetSafeHwnd() != NULL) 
	{
		m_btnChqAdd.SetWindowPos(NULL, btn_chq_add_x, btn_chq_add_y, m_btnChqAdd.Width(), m_btnChqAdd.Height(), SWP_NOZORDER);
		m_btnChqAdd.ShowWindow(SW_HIDE);
	}

// 	if (m_btnLock.GetSafeHwnd() != NULL) 
// 	{
// 		m_btnLock.SetWindowPos(NULL, btn_lock_x, btn_lock_y, m_btnLock.Width(), m_btnLock.Height(), SWP_NOZORDER);
// 		m_btnLock.ShowWindow(SW_HIDE);
// 	}
// 	if(m_btnYuan.GetSafeHwnd() != NULL)
// 	{
// 		m_btnYuan.SetWindowPos(NULL,btn_yuan_x,btn_yuan_y,m_btnYuan.Width(),m_btnYuan.Height(),SWP_NOZORDER);
// 		m_btnYuan.ShowWindow(SW_HIDE);
// 	}
// 	if(m_btnJiao.GetSafeHwnd() != NULL)
// 	{
// 		m_btnJiao.SetWindowPos(NULL,btn_jiao_x,btn_jiao_y,m_btnJiao.Width(),m_btnJiao.Height(),SWP_NOZORDER);
// 		m_btnJiao.ShowWindow(SW_HIDE);
// 	}
// 	if(m_btnFen.GetSafeHwnd() != NULL)
// 	{
// 		m_btnFen.SetWindowPos(NULL,btn_fen_x,btn_fen_y,m_btnFen.Width(),m_btnFen.Height(),SWP_NOZORDER);
// 		m_btnFen.ShowWindow(SW_HIDE);
// 	}
// 	if(m_btnLi.GetSafeHwnd() != NULL)
// 	{
// 		m_btnLi.SetWindowPos(NULL,btn_li_x,btn_li_y,m_btnLi.Width(),m_btnLi.Height(),SWP_NOZORDER);
// 		m_btnLi.ShowWindow(SW_HIDE);
// 	}

}

//投注框：选择数字，5位数字，0--9的选择
void CXingYun28::AdjustNumBtn()
{
	int n_row_span_y = 30;
	//和值------------------------------
	if(m_btnHeZhi_0.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_0.SetWindowPos(NULL, haoma_btn_x-19, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_0.Width(), m_btnHeZhi_0.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_1.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_1.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_1.Width(), m_btnHeZhi_1.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_2.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_2.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_2.Width(), m_btnHeZhi_2.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_3.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_3.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*3, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_3.Width(), m_btnHeZhi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_4.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_4.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_4.Width(), m_btnHeZhi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_5.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_5.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*5, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_5.Width(), m_btnHeZhi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_6.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_6.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*6, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_6.Width(), m_btnHeZhi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_7.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_7.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*7, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_7.Width(), m_btnHeZhi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_8.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_8.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*8, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_8.Width(), m_btnHeZhi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_9.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_9.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*9, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_9.Width(), m_btnHeZhi_9.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_10.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_10.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*10, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_10.Width(), m_btnHeZhi_10.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_11.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_11.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*11, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_11.Width(), m_btnHeZhi_11.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_12.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_12.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*12, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_12.Width(), m_btnHeZhi_12.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_13.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_13.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*13, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_13.Width(), m_btnHeZhi_13.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_14.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_14.SetWindowPos(NULL, haoma_btn_x-19, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_14.Width(), m_btnHeZhi_14.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_15.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_15.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_15.Width(), m_btnHeZhi_15.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_16.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_16.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_16.Width(), m_btnHeZhi_16.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_17.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_17.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*3, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_3.Width(), m_btnHeZhi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_18.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_18.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_4.Width(), m_btnHeZhi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_19.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_19.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*5, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_5.Width(), m_btnHeZhi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_20.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_20.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*6, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_6.Width(), m_btnHeZhi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_21.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_21.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*7, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_7.Width(), m_btnHeZhi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_22.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_22.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*8, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_8.Width(), m_btnHeZhi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_23.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_23.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*9, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_9.Width(), m_btnHeZhi_9.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_24.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_24.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*10, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_10.Width(), m_btnHeZhi_10.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_25.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_25.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*11, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_11.Width(), m_btnHeZhi_11.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_26.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_26.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*12, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_12.Width(), m_btnHeZhi_12.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_27.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_27.SetWindowPos(NULL, haoma_btn_x-19 + haoma_btn_col_wdith*13, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_13.Width(), m_btnHeZhi_13.Height(), SWP_NOZORDER);
	}


	//大小单双
	if(m_btnDa.GetSafeHwnd() != NULL) 
	{
		m_btnDa.SetWindowPos(NULL, haoma_btn_x-35, haoma_btn_y + haoma_btn_row_height*3+87, m_btnDa.Width(), m_btnDa.Height(), SWP_NOZORDER);
	}
	if(m_btnXiao.GetSafeHwnd() != NULL) 
	{
		m_btnXiao.SetWindowPos(NULL, haoma_btn_x-35 + m_btnDa.Width()+2, haoma_btn_y + haoma_btn_row_height*3+87, m_btnXiao.Width(), m_btnXiao.Height(), SWP_NOZORDER);
	}
	if(m_btnDan.GetSafeHwnd() != NULL) 
	{
		m_btnDan.SetWindowPos(NULL, haoma_btn_x-35 + (m_btnDa.Width()+2)*2, haoma_btn_y + haoma_btn_row_height*3+87, m_btnDan.Width(), m_btnDan.Height(), SWP_NOZORDER);
	}
	if(m_btnShuang.GetSafeHwnd() != NULL) 
	{
		m_btnShuang.SetWindowPos(NULL, haoma_btn_x -35+ (m_btnDa.Width()+2)*3, haoma_btn_y + haoma_btn_row_height*3+87, m_btnShuang.Width(), m_btnShuang.Height(), SWP_NOZORDER);
	}
	if(m_btnDaDan.GetSafeHwnd() != NULL) 
	{
		m_btnDaDan.SetWindowPos(NULL, haoma_btn_x-35, haoma_btn_y + haoma_btn_row_height*3+134, m_btnDaDan.Width(), m_btnDaDan.Height(), SWP_NOZORDER);
	}
	if(m_btnDaShuang.GetSafeHwnd() != NULL) 
	{
		m_btnDaShuang.SetWindowPos(NULL, haoma_btn_x-35  + m_btnDaDan.Width()+2, haoma_btn_y + haoma_btn_row_height*3+134, m_btnDaShuang.Width(), m_btnDaShuang.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoDan.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoDan.SetWindowPos(NULL, haoma_btn_x-35 +  (m_btnDaDan.Width()+2)*2, haoma_btn_y + haoma_btn_row_height*3+134, m_btnXiaoDan.Width(), m_btnXiaoDan.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoShuang.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoShuang.SetWindowPos(NULL, haoma_btn_x-35 + (m_btnDaDan.Width()+2)*3, haoma_btn_y + haoma_btn_row_height*3+134, m_btnXiaoShuang.Width(), m_btnXiaoShuang.Height(), SWP_NOZORDER);
	}



	if(m_btnMax.GetSafeHwnd() != NULL) 
	{
		m_btnMax.SetWindowPos(NULL, haoma_btn_x+293, haoma_btn_y + haoma_btn_row_height*3+134, m_btnMax.Width(), m_btnMax.Height(), SWP_NOZORDER);
	}
	if(m_btnMin.GetSafeHwnd() != NULL) 
	{
		m_btnMin.SetWindowPos(NULL, haoma_btn_x +293+  m_btnMax.Width()+2, haoma_btn_y + haoma_btn_row_height*3+134, m_btnMin.Width(), m_btnMin.Height(), SWP_NOZORDER);
	}
}





BOOL CXingYun28::OnInitDialog()
{
	CDialog::OnInitDialog();
	ModifyStyle(WS_CLIPCHILDREN,0);
	//ZeroMemory(m_szMD5Haoma,sizeof(m_szMD5Haoma));
	hBrush = NULL;
	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\game_bk4.png"));
	m_numImageList = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num.png"));
	m_numImageList1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num9.png"));
	m_numImageList2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num3.png"));
	m_numImageList3 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num5.png"));
	m_kjNumBig = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big6.png"));
	m_kjNumBig2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big6.png"));
	m_kjAdd = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Sign.png"));
	m_kjAdd1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Sign1.png"));
	m_imgKaijiang = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\xy28_kaijiang.gif"));
	m_imgDxDsh = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\imgDxDsh.png"));
	m_imgHistory = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\xy28_history.png"));
	int nCount = m_imgKaijiang->GetFrameDimensionsCount();			//获取帧维数
	GUID *pGuids = new GUID[nCount];								//定义一个GUID数组
	m_imgKaijiang->GetFrameDimensionsList(pGuids,nCount);			//获取图像帧的GUID
	m_nFrameCount=m_imgKaijiang->GetFrameCount(pGuids);				//获取GIF帧数
	m_bZhuiZjtz = true;
// 	m_btnYuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_1.png"));
// 	m_btnJiao.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_2.png"));
// 	m_btnFen.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_3.png"));
// 	m_btnLi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_4.png"));

	m_bShowFail = true;
	m_bigNumWidth = m_kjNumBig2->GetWidth() / 28;
	m_bigNumHeight = m_kjNumBig2->GetHeight();
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small2_2.png"));
	m_kjNumSmall1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small3_2.png"));
	m_smallNumWidth = m_kjNumSmall->GetWidth() / 10;
	m_smallNumHeight = m_kjNumSmall->GetHeight();
	m_bmpDxdsh = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num6.png"));
	m_bmpDxdsh1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num7.png"));
	m_bmpDxdsh2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num8.png"));
	m_bmpNumTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit.png"));
	m_bmpNumTip1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit2.png"));

	m_btnTextDx.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\xy28_bk_dxdsh.png"));
	m_btnTextDsh.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\xy28_bk_dxdsh.png"));
	m_bCanSend=true;
	InitNumberBtns();
	InitListCtrl();
	m_smallNumfont.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_zongFont.CreateFont(30, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_KjInfoFont1.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_KjInfoFont.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	m_btnSave.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\xy28_bc.png"));
	m_btnSave.ShowWindow(SW_HIDE);
	m_btnDelSel.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\xy28_zdy.png"));
	m_btnClsList.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\xy28_yxgz.png"));
	m_btnTouZhu.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_bet_xy28.png"));
	m_btnZhuiHao.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_append.png"));
	m_btnChqAdd.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_addnum.png"));

	m_btnJetton1.SetButtonImage(IDB_BT_XY_JETTON_1,AfxGetInstanceHandle(),true,false);
	m_btnJetton2.SetButtonImage(IDB_BT_XY_JETTON_2,AfxGetInstanceHandle(),true,false);
	m_btnJetton3.SetButtonImage(IDB_BT_XY_JETTON_3,AfxGetInstanceHandle(),true,false);
	m_btnJetton4.SetButtonImage(IDB_BT_XY_JETTON_4,AfxGetInstanceHandle(),true,false);
	m_btnJetton5.SetButtonImage(IDB_BT_XY_JETTON_5,AfxGetInstanceHandle(),true,false);

	m_btnJetton1.SetButtonColor(RGB(38,48,68));
	m_btnJetton1.SetFont(&m_zongFont);
	
	m_btnJetton2.SetButtonColor(RGB(38,48,68));
	m_btnJetton2.SetFont(&m_zongFont);
	m_btnJetton3.SetButtonColor(RGB(38,48,68));
	m_btnJetton3.SetFont(&m_zongFont);
	m_btnJetton4.SetButtonColor(RGB(38,48,68));
	m_btnJetton4.SetFont(&m_zongFont);
	m_btnJetton5.SetButtonColor(RGB(38,48,68));
	m_btnJetton5.SetFont(&m_zongFont);
	m_editBeiTou.SetEnableColor(RGB(114,114,114),RGB(240,240,240),RGB(114,114,114));
	m_editBeiTou.SetFont(&m_zongFont);

	m_editJetton1.SetEnableColor(RGB(250,250,250),RGB(163,153,147),RGB(163,153,147));
	m_editJetton1.SetFont(&m_smallNumfont);
	m_editJetton2.SetEnableColor(RGB(250,250,250),RGB(163,153,147),RGB(163,153,147));
	m_editJetton2.SetFont(&m_smallNumfont);
	m_editJetton3.SetEnableColor(RGB(250,250,250),RGB(163,153,147),RGB(163,153,147));
	m_editJetton3.SetFont(&m_smallNumfont);
	m_editJetton4.SetEnableColor(RGB(250,250,250),RGB(163,153,147),RGB(163,153,147));
	m_editJetton4.SetFont(&m_smallNumfont);
	m_editJetton5.SetEnableColor(RGB(250,250,250),RGB(163,153,147),RGB(163,153,147));
	m_editJetton5.SetFont(&m_smallNumfont);


	m_ImageItemBackMid.LoadImage(AfxGetInstanceHandle(),TEXT("GAME_ITEM_BACK_MID"));

	m_richDanshiHaoma.LimitText(-1);
	m_richDanshiHaoma.SetBackgroundColor(false,RGB(200,200,200));
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(88, 78, 77);
	m_richDanshiHaoma.SetDefaultCharFormat(cf);

	m_bKaiJiangzhong=false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CXingYun28::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CXingYun28::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
//初始化号码按钮
void CXingYun28::InitNumberBtns()
{
	m_btnHeZhi_0.SetImage(m_numImageList1, 0, 28);
	m_btnHeZhi_1.SetImage(m_numImageList1, 1, 28);
	m_btnHeZhi_2.SetImage(m_numImageList1, 2, 28);
	m_btnHeZhi_3.SetImage(m_numImageList1, 3, 28);
	m_btnHeZhi_4.SetImage(m_numImageList1, 4, 28);
	m_btnHeZhi_5.SetImage(m_numImageList1, 5, 28);
	m_btnHeZhi_6.SetImage(m_numImageList1, 6, 28);
	m_btnHeZhi_7.SetImage(m_numImageList1, 7, 28);
	m_btnHeZhi_8.SetImage(m_numImageList1, 8, 28);
	m_btnHeZhi_9.SetImage(m_numImageList1, 9, 28);
	m_btnHeZhi_10.SetImage(m_numImageList1, 10, 28);
	m_btnHeZhi_11.SetImage(m_numImageList1, 11, 28);
	m_btnHeZhi_12.SetImage(m_numImageList1, 12, 28);
	m_btnHeZhi_13.SetImage(m_numImageList1, 13, 28);
	m_btnHeZhi_14.SetImage(m_numImageList1, 14, 28);
	m_btnHeZhi_15.SetImage(m_numImageList1, 15, 28);
	m_btnHeZhi_16.SetImage(m_numImageList1, 16, 28);
	m_btnHeZhi_17.SetImage(m_numImageList1, 17, 28);
	m_btnHeZhi_18.SetImage(m_numImageList1, 18, 28);
	m_btnHeZhi_19.SetImage(m_numImageList1, 19, 28);
	m_btnHeZhi_20.SetImage(m_numImageList1, 20, 28);
	m_btnHeZhi_21.SetImage(m_numImageList1, 21, 28);
	m_btnHeZhi_22.SetImage(m_numImageList1, 22, 28);
	m_btnHeZhi_23.SetImage(m_numImageList1, 23, 28);
	m_btnHeZhi_24.SetImage(m_numImageList1, 24, 28);
	m_btnHeZhi_25.SetImage(m_numImageList1, 25, 28);
	m_btnHeZhi_26.SetImage(m_numImageList1, 26, 28);
	m_btnHeZhi_27.SetImage(m_numImageList1, 27, 28);


	m_btnDa.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiao.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDan.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuang.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaDan.SetImage(m_bmpDxdsh2, 0, 4);
	m_btnXiaoDan.SetImage(m_bmpDxdsh2, 2, 4);
	m_btnDaShuang.SetImage(m_bmpDxdsh2, 1, 4);
	m_btnXiaoShuang.SetImage(m_bmpDxdsh2, 3, 4);

	m_btnMax.SetImage(m_bmpDxdsh1, 0, 2);
	m_btnMin.SetImage(m_bmpDxdsh1, 1, 2);

}

void CXingYun28::InitListCtrl()
{
	m_listChqSSC.SetExtendedStyle(m_listChqSSC.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);
	m_listChqSSC.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	m_listChqSSC.SetPenColor(RGB(247,233,216));
	m_listChqSSC.InsertColumn(0, _T("号码"), LVCFMT_CENTER, 313);
	m_listChqSSC.InsertColumn(1, _T("模式"), LVCFMT_CENTER, 80);
	m_listChqSSC.InsertColumn(2, _T("注数"), LVCFMT_CENTER, 70);
	m_listChqSSC.InsertColumn(3, _T("倍数"), LVCFMT_CENTER, 70);
	m_listChqSSC.InsertColumn(4, _T(""), LVCFMT_CENTER, 30);

}


CString CXingYun28::GetBjNum()//获取北京快8的按钮数值
{
	CString str;
	return str;
}
CString CXingYun28::GetHezhiString()//获取幸运28数值
{
	CString str;
	if (IsNumBtnCheck(m_btnHeZhi_0))
	{
		str += _T("00");
	}
	if (IsNumBtnCheck(m_btnHeZhi_1))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnHeZhi_2))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnHeZhi_3))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnHeZhi_4))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnHeZhi_5))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnHeZhi_6))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnHeZhi_7))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnHeZhi_8))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnHeZhi_9))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnHeZhi_10))
	{
		str += _T("10");
	}
	if (IsNumBtnCheck(m_btnHeZhi_11))
	{
		str += _T("11");
	}
	if (IsNumBtnCheck(m_btnHeZhi_12))
	{
		str += _T("12");
	}
	if (IsNumBtnCheck(m_btnHeZhi_13))
	{
		str += _T("13");
	}
	if (IsNumBtnCheck(m_btnHeZhi_14))
	{
		str += _T("14");
	}
	if (IsNumBtnCheck(m_btnHeZhi_15))
	{
		str += _T("15");
	}
	if (IsNumBtnCheck(m_btnHeZhi_16))
	{
		str += _T("16");
	}
	if (IsNumBtnCheck(m_btnHeZhi_17))
	{
		str += _T("17");
	}
	if (IsNumBtnCheck(m_btnHeZhi_18))
	{
		str += _T("18");
	}
	if (IsNumBtnCheck(m_btnHeZhi_19))
	{
		str += _T("19");
	}
	if (IsNumBtnCheck(m_btnHeZhi_20))
	{
		str += _T("20");
	}
	if (IsNumBtnCheck(m_btnHeZhi_21))
	{
		str += _T("21");
	}
	if (IsNumBtnCheck(m_btnHeZhi_22))
	{
		str += _T("22");
	}
	if (IsNumBtnCheck(m_btnHeZhi_23))
	{
		str += _T("23");
	}
	if (IsNumBtnCheck(m_btnHeZhi_24))
	{
		str += _T("24");
	}
	if (IsNumBtnCheck(m_btnHeZhi_25))
	{
		str += _T("25");
	}
	if (IsNumBtnCheck(m_btnHeZhi_26))
	{
		str += _T("26");
	}
	if (IsNumBtnCheck(m_btnHeZhi_27))
	{
		str += _T("27");
	}
	return str;
}
CString CXingYun28::GetJiZhiString()
{
	CString strGe;
	if (IsNumBtnCheck(m_btnMax)) 
	{
		strGe += _T("18");
	}
	if (IsNumBtnCheck(m_btnMin)) 
	{
		strGe += _T("17");
	}

	return strGe;
}

CString CXingYun28::GetDxdshString()
{
	CString strGe;
	if (IsNumBtnCheck(m_btnDa)) 
	{
		strGe += _T("9,");
	}
	if (IsNumBtnCheck(m_btnXiao)) 
	{
		strGe += _T("1,");
	}
	if (IsNumBtnCheck(m_btnDan)) 
	{
		strGe += _T("3,");
	}
	if (IsNumBtnCheck(m_btnShuang)) 
	{
		strGe += _T("2,");
	}
	if (IsNumBtnCheck(m_btnDaDan)) 
	{
		strGe += _T("12,");
	}
	if (IsNumBtnCheck(m_btnXiaoDan)) 
	{
		strGe += _T("10,");
	}
	if (IsNumBtnCheck(m_btnDaShuang)) 
	{
		strGe += _T("13,");
	}
	if (IsNumBtnCheck(m_btnXiaoShuang)) 
	{
		strGe += _T("11,");
	}


	return strGe;
}
CString CXingYun28::GetFunnyDesc(int nNum)
{
	CString strDesc;
	switch(nNum)
	{
	case 14:
		{
			strDesc = _T("对子");
			break;
		}
	case 15:
		{
			strDesc = _T("顺子");
			break;
		}
	case 16:
		{
			strDesc = _T("豹子");
			break;
		}
	}
	return strDesc;
}
CString CXingYun28::GetDxdshStringDesc(int nNum)
{
	CString strDesc;
	switch (nNum)
	{
	case 1:
		{
			strDesc = _T("小");
			break;
		}
	case 2:
		{
			strDesc = _T("双");
			break;
		}
	case 3:
		{
			strDesc = _T("单");
			break;
		}
	case 9:
		{
			strDesc = _T("大");
			break;
		}
	case 10:
		{
			strDesc = _T("小单");
			break;
		}
	case 11:
		{
			strDesc = _T("小双");
			break;
		}
	case 12:
		{
			strDesc = _T("大单");
			break;
		}
	case 13:
		{
			strDesc = _T("大双");
			break;
		}
	case 17:
		{
			strDesc = _T("极小");
			break;
		}
	case 18:
		{
			strDesc = _T("极大");
			break;
		}
	}
	return strDesc;
}

CString CXingYun28::GetWanDesc()
{
	return theGameKind[(CZXingYun28 << 16) | (unsigned short)m_gameKind];
}

//服务器端返回前N个开奖号码
bool CXingYun28::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex)
{
	_tcscpy_s(m_kjXinxi[nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);

	if(nIndex+1 < TOP_KAIJIANG_NUMBERS)
		return true;

	m_kjXinxiCont = TOP_KAIJIANG_NUMBERS;
	if(!IsWindowVisible())
		return false;

	//return true;

	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		//第一次进入
		if(m_lastExpect[0] == 0)
			_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);
		m_bKaiJiangzhong=false;											//开奖结束
		long secDiff = m_chqRule.GetKjShjDiff();

		KillTimer(timer_id_kaijiangzhong);

		InvalidateRect(&rcHistory);

		if(m_bKaiJiangzhong)
		{
			CRect rcCnt;
			rcCnt.left = rcRedraw.left+200;
			rcCnt.right = rcRedraw.right;
			rcCnt.top = rcRedraw.top;
			rcCnt.bottom = rcRedraw.bottom;
			RedrawWindow(&rcCnt,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
		else
		{
			RedrawWindow(&rcRedraw,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
	}

	return true;
}


void CXingYun28::CheckWanFaBtn(CTextButton& btn)
{
	m_btnRenXuan.SetPushed(false);
	m_btnTeMa.SetPushed(false);
	m_btnDaXiaoDanShuang.SetPushed(false);
	m_btnJiZhi.SetPushed(false);
	m_btnFunny.SetPushed(false);


	btn.SetPushed(true);
}

bool CXingYun28::GetDanShiHaoma(CString& haoma, int size, int nLeast)
{
	CStringArray strArr;
	CString line;
	//m_editDanShiHaoMa.GetWindowText(line);
	m_richDanshiHaoma.GetWindowText(line);

	if(line.IsEmpty())
	{
		MyMessageBox(_T("请输入号码！"));	

		return false;
	}

	CString line1=line;
	CString temp;
	do 
	{
		int n = line1.Find(_T(","));
		if(n == -1)
		{
			n = line1.Find(_T(" "));
		}

		if(n == -1)
		{
			temp = line1;
		}
		else
		{
			temp = line1.Left(n);
		}
		if(temp.GetLength() > size)
		{
			MyMessageBox(_T("您输入的号码太多了"));	
			return false;
		}
		if(n == -1)
		{
			line1.Empty();
		}
		else
		{
			int nAll = line1.GetLength();
			line1 = line1.Right(nAll-n-1);
		}

	} while (!line1.IsEmpty());

	if (!GetHaomaStrs(strArr, line, size,nLeast)) 
	{
		return false;
	}

	int count = strArr.GetCount();
	CString strTemp;
	for(int i=0; i<count; i++)
	{
		strTemp = strArr.GetAt(i);

		int nWeishu = strTemp.GetLength();
		if(nWeishu/2 < nLeast)
		{
			//MyMessageBox(_T("您输入的号码太多了"));	
			return false;

		}
		for(int j = 0; j<strTemp.GetLength();j++)
		{
			haoma += strTemp.GetAt(j);
			if (j%2== 1 && (j!=nWeishu-1))
			{
				haoma += _T(",");
			}

			if(j%2== 1 &&j == nWeishu-1)
			{
				haoma +=_T("|");
			}

		}

	}

	return true;
}

bool CXingYun28::GetHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast)
{
	int len = line.GetLength();
	CString subStr;
	CString strTemp;
	int nTempHaoma[20];
	int nCount = 0;
	ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));


	for (int i=0; i<len; i++) 
	{
		CString str;
		str=line.GetAt(i);
		if (iswdigit(line.GetAt(i))&&!str.IsEmpty()) 
		{
			subStr += line.GetAt(i);
			strTemp+= line.GetAt(i);

			subStr += line.GetAt(++i);
			strTemp+= line.GetAt(i);

			int nTemp = _ttoi(strTemp);
			strTemp.Empty();

			if(nTemp<1||nTemp>80)
			{
				MyMessageBox(L"您的输入有误，数字只能在1～80之间 ");
				return false;
			}

			for (int n = 0;n<nCount;n++)
			{
				if(nTemp == nTempHaoma[n])
				{
					CString strLog;
					strLog.Format(L"%02d 和 %02d重复",nTemp,nTempHaoma[n]);
					MyMessageBox(strLog);
					return false;
				}
			}
			nTempHaoma[nCount++] = nTemp;

			if (subStr.GetLength() > nLeast*2) 
			{
				return false;
			}
		}
		else
		{
			// 			MyMessageBox(L"您的输入有误，输入不需要除数字以外的字符");
			// 			return false;
			nCount=0;

			ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));

			if (subStr.GetLength() > size || subStr.GetLength() == 0 || subStr.GetLength()/2<nLeast) 
			{
				return false;
			}

			//if (subStr.GetLength() == size) 
			{
				strArr.Add(subStr);
				subStr = _T("");
			}
		}
	}

	// 	if (subStr.GetLength() != size && subStr.GetLength() != 0) 
	// 	{
	// 		return false;
	// 	}
	if(( subStr.GetLength() == 0)&&(strArr.GetCount()==0))
	{
		MyMessageBox(_T("请输入号码！"));	

		return false;

	}

	if (subStr.GetLength() != 0) 
	{
		strArr.Add(subStr);
		subStr = _T("");
	}

	return true;
}
LRESULT CXingYun28::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	//OnBnClickedBtnDelSel();
	nSubItem--;

	int zhushu = _ttoi(m_listChqSSC.GetItemText(nItem, 2));
	m_nZongZhuShu -= zhushu;
	m_listChqSSC.deleteItemEx( nItem );
	// 	double danzhujine = 2.0;
	// 
	// 	if(theAccount.Scoretype == MoShi_Yuan)
	// 	{	
	// 		danzhujine = 2.0;
	// 	}
	// 	else if (theAccount.Scoretype == MoShi_Jiao)
	// 	{
	// 		danzhujine = 0.2;
	// 	}
	// 	else if(theAccount.Scoretype == MoShi_Fen)
	// 	{
	// 		danzhujine = 0.02;
	// 	}
	// 	else if(theAccount.Scoretype == MoShi_Li)
	// 		danzhujine = 0.002;
	// 	int nItemCount = m_listChqSSC.GetItemCount();
	// 	m_zongJine = 0.00f;
	// 	for(int i = 0;i < nItemCount;i++)
	// 	{
	// 		CString strBeiSHu = m_listChqSSC.GetItemText(i,3);
	// 		CString strZhuSHu = m_listChqSSC.GetItemText(i,2);
	// 		int beishu = _ttoi(strBeiSHu);
	// 		int zhushu = _ttoi(strZhuSHu);
	// 
	// 
	// 		m_zongJine += zhushu*beishu*danzhujine;
	// 	}
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	// 	rcInvali.CopyRect(rcAllZongzhushu);
	// 	rcInvali.bottom += 20;
	// 	InvalidateRect(&rcInvali);

	//	m_listChqSSC.DeleteItem(nItem);

	if(vecAllNums.size() >0)
	{
		int nSize = vecAllNums.size();
		nSize -=1;

		vector<wstring>::iterator   iter = vecAllNums.begin()+nSize-nItem;

		vecAllNums.erase(iter);
	}



	return 0;
}


BOOL CXingYun28::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	if((nCommandID >= IDC_BTN_HEZHI_0 && nCommandID <= IDC_BTN_HEZHI_27)||(nCommandID >= IDC_BTN_DA_GE && nCommandID <= IDC_BTN_SHUANG_GE)||(nCommandID >= IDC_BTN_XIAODAN && nCommandID <= IDC_BTN_DASHUANG)||(nCommandID >= IDC_BTN_MAX && nCommandID <= IDC_BTN_MIN))
	{
		ResetAllNums();
		CheckNumBtn(*(CNumerButton*)GetDlgItem(nCommandID));
		m_editBeiTou.SetWindowText(L"0");
		m_beishu = 0;
		m_nZongZhuShu = 1;
		m_zongJine = m_beishu;
		if(nCommandID >= IDC_BTN_HEZHI_0 && nCommandID <= IDC_BTN_HEZHI_27)
		{
			m_gameKind = nCommandID-IDC_BTN_HEZHI_0+1;

			m_strTouzhuHaoma.Format(L"%d|",m_gameKind-1);
		}
		else if(nCommandID >= IDC_BTN_DA_GE && nCommandID <= IDC_BTN_SHUANG_GE)
		{
			m_gameKind = enXY28_DXDS;
			int nDxdsh[4] = {NBigNumber,NSmallNumber,NSingleNumber,NDoubleNumber};
			int nIndex = nCommandID-IDC_BTN_DA_GE;
			m_strTouzhuHaoma.Format(L"%d|",nDxdsh[nIndex]);
		}
		else if(nCommandID >= IDC_BTN_XIAODAN && nCommandID <= IDC_BTN_DASHUANG)
		{
			m_gameKind = enXY28_Funny;
			int nDxdsh[4] = {NSmallSingleNumber,NSmallDoubleNumber,NBigSingleNumber,NBigDoubleNumber};
			int nIndex = nCommandID-IDC_BTN_XIAODAN;
			m_strTouzhuHaoma.Format(L"%d|",nDxdsh[nIndex]);
		}
		else if(nCommandID >= IDC_BTN_MAX && nCommandID <= IDC_BTN_MIN)
		{
			m_gameKind = enXY28_JiZhi;
			int nDxdsh[2] = {NLimitBig,NLimitSmall};
			int nIndex = nCommandID-IDC_BTN_MAX;
			m_strTouzhuHaoma.Format(L"%d|",nDxdsh[nIndex]);
		}
	}
	return  __super::OnCommand(wParam,lParam);;
}

int CXingYun28::GetXy28Zhushu(int nTypeID,int nKindID)
{
	m_singlezhushu = 1;


	return m_singlezhushu;
}
