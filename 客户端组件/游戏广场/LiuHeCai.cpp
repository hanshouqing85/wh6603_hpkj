#include "stdafx.h"
#include "GamePlaza.h"
#include "LiuHeCai.h"
#include "BmpUtil.h"
/*#include "LuckMeDlg.h"*/
#include "MessageDlg.h"
#include "DlgAddtional.h"
#include "PlatformFrame.h"
#include "DlgFandian.h"
#include "xdate.h"
#include "GDIpng/MemDC.h"
#include   <afxinet.h> 
#include <map>
#include <wininet.h>
#include <fstream>
using namespace std;
map <BYTE,DOUBLE> mapJetton;
extern map <int,CString> mapFandian;
extern map <int,float> mapPercent;
extern map <int,float> mapDanweiFandian;
extern map <int,float> mapBonus;

IMPLEMENT_DYNAMIC(CLiuHeCai, CDialog)

static const int nian_left = 250;
static const int nian_top = 16;

static CRect rc_time_kj(600, 10, 633+200, 12 + 20);
static CRect rc_time_rest(44, 5, 700, 5 + 21);
//static CRect rcYuGao();
static const int btn_startX = 3;				//�淨��ť
static const int btn_startY = 109+16-2;			//�淨��ť

static const int btn_jetton_X = 220;			//�淨��ť

static const int btn_jetton_ok_X = 590;			//�淨��ť
static const int btn_jetton_ok_Y = 500;			//�淨��ť

static const int redraw_rc_left = 0;
static const int redraw_rc_top = 0;
static const int redraw_rc_width = 789;
static const int redraw_rc_height = 110;

static CRect rcRedraw(redraw_rc_left, redraw_rc_top, redraw_rc_left + redraw_rc_width, redraw_rc_top + redraw_rc_height);

static const int edit_tmdm_x = 57-5+1;
static const int edit_tmdm_y = 223+10-2;

static const int edit_tmdxds_x = 146-4+1;
static const int edit_tmdxds_y = 226+8-2;

static const int edit_tmsx_x = 142-4+1+1+1;
static const int edit_tmsx_y = 226+8-2;

static const int edit_tmbs_x = 143-4+1+1;
static const int edit_tmbs_y = 226+8-2;

//ȷ��Ͷע��ť
static const int btn_touzhu_x = 638;
static const int btn_touzhu_y = 490;
//����б�ť
static const int btn_cls_list_x = 672;
static const int btn_cls_list_y = 410;
//����
static const int btn_lock_x = 450;
static const int btn_lock_y = 410;
//��Ӻ��밴ť
static const int btn_chq_add_x = 559;
static const int btn_chq_add_y = 410;
//�����б�
static const int list_haomao_x = 43;
static const int list_haomao_y = 469+25+1;
static const int list_haomao_width = 585-5-2;
static const int list_haomao_height = 58;

//�󿪽���
static const int kj_big_haoma_x = 55-48;
static const int kj_big_haoma_y = 38;

static const int czh_rc_left = 30;
static const int czh_rc_top = 6-8;
static const int czh_rc_width = 85;
static const int czh_rc_height = 30;
static CRect rcCzh(czh_rc_left, czh_rc_top, czh_rc_left + czh_rc_width, czh_rc_top + czh_rc_height);

static const int qihao_rc_left = 125-10;
static const int qihao_rc_top = 6-8;
static const int qihao_rc_width = 125;
static const int qihao_rc_height = 30;
static CRect rcQiHao(qihao_rc_left, qihao_rc_top, qihao_rc_left + qihao_rc_width, qihao_rc_top + qihao_rc_height);
//��ע��
static const int zongzhushu_x = 21;
static const int zongzhushu_y = 411;
static const int zongzhushu_width = 170;
static const int zongzhushu_height = 45;
static CRect rcZongZhuShu(zongzhushu_x, zongzhushu_y, zongzhushu_x+zongzhushu_width, zongzhushu_y+zongzhushu_height);

//�ܽ��
static const int zongjine_x = 165;
static const int zongjine_y = 428;
static const int zongjine_width = 150;
static const int zongjine_height = 45;
static CRect rcZongJinE(zongjine_x, zongjine_y, zongjine_x+zongjine_width, zongjine_y+zongjine_height);

//��ע��
static const int allzongzhushu_x = 638;
static const int allzongzhushu_y = 444;
static const int allzongzhushu_width = 150;
static const int allzongzhushu_height = 25;
static CRect rcAllZongzhushu(allzongzhushu_x, allzongzhushu_y, allzongzhushu_x+allzongzhushu_width, allzongzhushu_y+allzongzhushu_height);

//�淨�ӷ�������
static const int wanfa_sel_x = 20;
static const int wanfa_sel_y = 145+13;
static const int wanfa_sel_width = 80;
static const int wanfa_sel_height = 25;

//С������
static const int kj_small_haoma_x = 630-40;
static const int kj_small_haoma_y = 28;
static const int kj_small_haoma_col_span = 3;
static const int kj_small_haoma_row_span = 1;

char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr)
		delete[] wstr;
	return str;
}

static const int TimerFandian = 5;
static const int timer_id_kaijiangzhong=3;
static const int timer_update_resttime = 6;
static const int timer_get_qihao = 7;
static const int TimerLotteryStatus = 8;

#define IDC_JETTON_BUTTON_1		200									//��ť��ʶ
#define IDC_JETTON_BUTTON_5		201									//��ť��ʶ
#define IDC_JETTON_BUTTON_10	202									//��ť��ʶ
#define IDC_JETTON_BUTTON_50	203									//��ť��ʶ
#define IDC_JETTON_BUTTON_100	204									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000	205									//��ť��ʶ

#define IDC_JETTON_OK			206									//��ť��ʶ
#define IDC_JETTON_FANDIAN		208									//��ť��ʶ

CLiuHeCai::CLiuHeCai(CWnd* pParent)
	: CDialog(CLiuHeCai::IDD, pParent)
	, m_bmpBk(NULL)
	, m_bmpTmdm(NULL)
	, m_bmpTmpm(NULL)
	, m_bmpTmds(NULL)
	, m_bmpTmdx(NULL)
	, m_bmpTmsx(NULL)
	, m_bmpTmbs(NULL)
	, m_kjXinxiCont(0)
	, m_kjNumBig(NULL)
	, m_kjNumSmall(NULL)
	, m_pngShengXiao(NULL)
{
	m_DlgStatus.SetStatusViewSink(this);

	m_gameKind = LiuHeCai_Tmdm;
	m_nShengXiaoWuCha=0;
	
	//����ũ��
	xDate	xD;
	WORD iLunarYear=0;
	WORD iLunarMon=0;
	WORD iLunarDay=0;
	CTime	tm_now = CTime::GetCurrentTime();
	xD.GetLunarDate(tm_now.GetYear(), tm_now.GetMonth(), tm_now.GetDay(), iLunarYear, iLunarMon, iLunarDay);
	m_nShengXiaoWuCha = (iLunarYear - 2017) %12;		//12���һ���ֻ�

	//���������졣
	tm_now += 24*60*60;
	m_NextTime = CTime(tm_now.GetYear(), tm_now.GetMonth(), tm_now.GetDay(), 21, 30, 00);

	for (int i=0; i<Kj_XinXi_Count; i++) 
	{
		memset(&m_kjXinxi[i], 0, sizeof(KjXinXi));
	}
	_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, _T(""));

	InitWanFaMoShi();

	m_nTzhSign = 1;
	m_nTzhSign1 = 0;
}
//ȡ������
VOID CLiuHeCai::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

CLiuHeCai::~CLiuHeCai()
{
	if (m_bmpBk != NULL) 
	{
		SafeDelete(m_bmpBk);
	}
	if (m_bmpTmdm != NULL) 
	{
		SafeDelete(m_bmpTmdm);
	}
	if (m_bmpTmpm != NULL) 
	{
		SafeDelete(m_bmpTmpm);
	}
	if (m_bmpTmds != NULL) 
	{
		SafeDelete(m_bmpTmds);
	}
	if (m_bmpTmdx != NULL) 
	{
		SafeDelete(m_bmpTmdx);
	}
	if (m_bmpTmsx != NULL) 
	{
		SafeDelete(m_bmpTmsx);
	}
	if (m_bmpTmbs != NULL) 
	{
		SafeDelete(m_bmpTmbs);
	}
	if (m_kjNumBig != NULL) 
	{
		SafeDelete(m_kjNumBig);
	}
	if (m_kjNumSmall != NULL) 
	{
		SafeDelete(m_kjNumSmall);
	}
	if (m_imgKaijiang != NULL) 
	{
		SafeDelete(m_imgKaijiang);
	}
	if(m_pngShengXiao)
		SafeDelete(m_pngShengXiao);
}



void CLiuHeCai::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_TMDM, m_btnTmdm);
	DDX_Control(pDX, IDC_BTN_PMDM, m_btnPmdm);
	DDX_Control(pDX, IDC_BTN_TMDS, m_btnTmds);
	DDX_Control(pDX, IDC_BTN_TMDX, m_btnTmdx);
	DDX_Control(pDX, IDC_BTN_TMSX, m_btnTmsx);
	DDX_Control(pDX, IDC_BTN_TMBS, m_btnTmbs);

	DDX_Control(pDX, IDC_BTN_TOUZHU, m_btnTouZhu);
	DDX_Control(pDX, IDC_BTN_3D_ADD, m_btn3DAdd);
	DDX_Control(pDX, IDC_BTN_LOCK, m_btnLock);
	DDX_Control(pDX, IDC_BTN_CLS_LIST, m_btnClsList);
	DDX_Control(pDX, IDC_LIST_3D, m_list3D);
	DDX_Control(pDX, IDC_EDIT_TMDM_1, m_editTmdm1);
	DDX_Control(pDX, IDC_EDIT_TMDM_2, m_editTmdm2);
	DDX_Control(pDX, IDC_EDIT_TMDM_3, m_editTmdm3);
	DDX_Control(pDX, IDC_EDIT_TMDM_4, m_editTmdm4);
	DDX_Control(pDX, IDC_EDIT_TMDM_5, m_editTmdm5);
	DDX_Control(pDX, IDC_EDIT_TMDM_6, m_editTmdm6);
	DDX_Control(pDX, IDC_EDIT_TMDM_7, m_editTmdm7);
	DDX_Control(pDX, IDC_EDIT_TMDM_8, m_editTmdm8);
	DDX_Control(pDX, IDC_EDIT_TMDM_9, m_editTmdm9);
	DDX_Control(pDX, IDC_EDIT_TMDM_10, m_editTmdm10);
	DDX_Control(pDX, IDC_EDIT_TMDM_11, m_editTmdm11);
	DDX_Control(pDX, IDC_EDIT_TMDM_12, m_editTmdm12);
	DDX_Control(pDX, IDC_EDIT_TMDM_13, m_editTmdm13);
	DDX_Control(pDX, IDC_EDIT_TMDM_14, m_editTmdm14);
	DDX_Control(pDX, IDC_EDIT_TMDM_15, m_editTmdm15);
	DDX_Control(pDX, IDC_EDIT_TMDM_16, m_editTmdm16);
	DDX_Control(pDX, IDC_EDIT_TMDM_17, m_editTmdm17);
	DDX_Control(pDX, IDC_EDIT_TMDM_18, m_editTmdm18);
	DDX_Control(pDX, IDC_EDIT_TMDM_19, m_editTmdm19);
	DDX_Control(pDX, IDC_EDIT_TMDM_20, m_editTmdm20);
	DDX_Control(pDX, IDC_EDIT_TMDM_21, m_editTmdm21);
	DDX_Control(pDX, IDC_EDIT_TMDM_22, m_editTmdm22);
	DDX_Control(pDX, IDC_EDIT_TMDM_23, m_editTmdm23);
	DDX_Control(pDX, IDC_EDIT_TMDM_24, m_editTmdm24);
	DDX_Control(pDX, IDC_EDIT_TMDM_25, m_editTmdm25);
	DDX_Control(pDX, IDC_EDIT_TMDM_26, m_editTmdm26);
	DDX_Control(pDX, IDC_EDIT_TMDM_27, m_editTmdm27);
	DDX_Control(pDX, IDC_EDIT_TMDM_28, m_editTmdm28);
	DDX_Control(pDX, IDC_EDIT_TMDM_29, m_editTmdm29);
	DDX_Control(pDX, IDC_EDIT_TMDM_30, m_editTmdm30);
	DDX_Control(pDX, IDC_EDIT_TMDM_31, m_editTmdm31);
	DDX_Control(pDX, IDC_EDIT_TMDM_32, m_editTmdm32);
	DDX_Control(pDX, IDC_EDIT_TMDM_33, m_editTmdm33);
	DDX_Control(pDX, IDC_EDIT_TMDM_34, m_editTmdm34);
	DDX_Control(pDX, IDC_EDIT_TMDM_35, m_editTmdm35);
	DDX_Control(pDX, IDC_EDIT_TMDM_36, m_editTmdm36);
	DDX_Control(pDX, IDC_EDIT_TMDM_37, m_editTmdm37);
	DDX_Control(pDX, IDC_EDIT_TMDM_38, m_editTmdm38);
	DDX_Control(pDX, IDC_EDIT_TMDM_39, m_editTmdm39);
	DDX_Control(pDX, IDC_EDIT_TMDM_40, m_editTmdm40);
	DDX_Control(pDX, IDC_EDIT_TMDM_41, m_editTmdm41);
	DDX_Control(pDX, IDC_EDIT_TMDM_42, m_editTmdm42);
	DDX_Control(pDX, IDC_EDIT_TMDM_43, m_editTmdm43);
	DDX_Control(pDX, IDC_EDIT_TMDM_44, m_editTmdm44);
	DDX_Control(pDX, IDC_EDIT_TMDM_45, m_editTmdm45);
	DDX_Control(pDX, IDC_EDIT_TMDM_46, m_editTmdm46);
	DDX_Control(pDX, IDC_EDIT_TMDM_47, m_editTmdm47);
	DDX_Control(pDX, IDC_EDIT_TMDM_48, m_editTmdm48);
	DDX_Control(pDX, IDC_EDIT_TMDM_49, m_editTmdm49);

	DDX_Control(pDX, IDC_EDIT_TMDXDS_DA, m_editTmdxdsDa);
	DDX_Control(pDX, IDC_EDIT_TMDXDS_XIAO, m_editTmdxdsXiao);
	DDX_Control(pDX, IDC_EDIT_TMDXDS_DAN, m_editTmdxdsDan);
	DDX_Control(pDX, IDC_EDIT_TMDXDS_SHUANG, m_editTmdxdsShuang);

	DDX_Control(pDX, IDC_EDIT_TMSX_SHU, m_editTmsxShu);
	DDX_Control(pDX, IDC_EDIT_TMSX_NIU, m_editTmsxNiu);
	DDX_Control(pDX, IDC_EDIT_TMSX_HU, m_editTmsxHu);
	DDX_Control(pDX, IDC_EDIT_TMSX_TU, m_editTmsxTu);
	DDX_Control(pDX, IDC_EDIT_TMSX_LONG, m_editTmsxLong);
	DDX_Control(pDX, IDC_EDIT_TMSX_SHE, m_editTmsxShe);
	DDX_Control(pDX, IDC_EDIT_TMSX_MA, m_editTmsxMa);
	DDX_Control(pDX, IDC_EDIT_TMSX_YANG, m_editTmsxYang);
	DDX_Control(pDX, IDC_EDIT_TMSX_HOU, m_editTmsxHou);
	DDX_Control(pDX, IDC_EDIT_TMSX_JI, m_editTmsxJi);
	DDX_Control(pDX, IDC_EDIT_TMSX_GOU, m_editTmsxGou);
	DDX_Control(pDX, IDC_EDIT_TMSX_ZHU, m_editTmsxZhu);
	DDX_Control(pDX, IDC_EDIT_TMBS_HB, m_editTmbshb);
	DDX_Control(pDX, IDC_EDIT_TMBS_LB, m_editTmbslb);
	DDX_Control(pDX, IDC_EDIT_TMBS_LVB, m_editTmbslvb);

	DDX_Control(pDX, IDC_BTN_ZM1T, m_btnZmtm1);
	DDX_Control(pDX, IDC_BTN_ZM2T, m_btnZmtm2);
	DDX_Control(pDX, IDC_BTN_ZM3T, m_btnZmtm3);
	DDX_Control(pDX, IDC_BTN_ZM4T, m_btnZmtm4);
	DDX_Control(pDX, IDC_BTN_ZM5T, m_btnZmtm5);
	DDX_Control(pDX, IDC_BTN_ZM6T, m_btnZmtm6);
}

BEGIN_MESSAGE_MAP(CLiuHeCai, CDialog)

	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BTN_TMDM, &CLiuHeCai::OnBnClickedBtnTmdm)
	ON_BN_CLICKED(IDC_BTN_PMDM, &CLiuHeCai::OnBnClickedBtnPmdm)
	ON_BN_CLICKED(IDC_BTN_TMDS, &CLiuHeCai::OnBnClickedBtnTmds)
	ON_BN_CLICKED(IDC_BTN_TMDX, &CLiuHeCai::OnBnClickedBtnTmdx)
	ON_BN_CLICKED(IDC_BTN_TMSX, &CLiuHeCai::OnBnClickedBtnTmsx)
	ON_BN_CLICKED(IDC_BTN_TMBS, &CLiuHeCai::OnBnClickedBtnTmbs)

	ON_BN_CLICKED(IDC_BTN_3D_ADD, &CLiuHeCai::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_TOUZHU, &CLiuHeCai::OnBnClickedBtnTouZhu)

	ON_BN_CLICKED(IDC_BTN_ZM1T, &CLiuHeCai::OnBnClickedBtnZm1t)
	ON_BN_CLICKED(IDC_BTN_ZM2T, &CLiuHeCai::OnBnClickedBtnZm2t)
	ON_BN_CLICKED(IDC_BTN_ZM3T, &CLiuHeCai::OnBnClickedBtnZm3t)
	ON_BN_CLICKED(IDC_BTN_ZM4T, &CLiuHeCai::OnBnClickedBtnZm4t)
	ON_BN_CLICKED(IDC_BTN_ZM5T, &CLiuHeCai::OnBnClickedBtnZm5t)
	ON_BN_CLICKED(IDC_BTN_ZM6T, &CLiuHeCai::OnBnClickedBtnZm6t)

	ON_BN_CLICKED(IDC_JETTON_OK, &CLiuHeCai::OnBnClickedJettonOK)
	ON_BN_CLICKED(IDC_JETTON_FANDIAN, &CLiuHeCai::OnBnClickedFandian)
	ON_BN_CLICKED(IDC_BTN_CLS_LIST, &CLiuHeCai::OnBnClickedBtnClsList)
	ON_MESSAGE( WM_BN_CLICK, &CLiuHeCai::onBnCLick)
END_MESSAGE_MAP()

void CLiuHeCai::ShowTmdmCtrl()
{
	m_editTmdm1.ShowWindow(SW_SHOW);
	m_editTmdm2.ShowWindow(SW_SHOW);
	m_editTmdm3.ShowWindow(SW_SHOW);
	m_editTmdm4.ShowWindow(SW_SHOW);
	m_editTmdm5.ShowWindow(SW_SHOW);
	m_editTmdm6.ShowWindow(SW_SHOW);
	m_editTmdm7.ShowWindow(SW_SHOW);
	m_editTmdm8.ShowWindow(SW_SHOW);
	m_editTmdm9.ShowWindow(SW_SHOW);
	m_editTmdm10.ShowWindow(SW_SHOW);
	m_editTmdm11.ShowWindow(SW_SHOW);
	m_editTmdm12.ShowWindow(SW_SHOW);
	m_editTmdm13.ShowWindow(SW_SHOW);
	m_editTmdm14.ShowWindow(SW_SHOW);
	m_editTmdm15.ShowWindow(SW_SHOW);
	m_editTmdm16.ShowWindow(SW_SHOW);
	m_editTmdm17.ShowWindow(SW_SHOW);
	m_editTmdm18.ShowWindow(SW_SHOW);
	m_editTmdm19.ShowWindow(SW_SHOW);
	m_editTmdm20.ShowWindow(SW_SHOW);
	m_editTmdm21.ShowWindow(SW_SHOW);
	m_editTmdm22.ShowWindow(SW_SHOW);
	m_editTmdm23.ShowWindow(SW_SHOW);
	m_editTmdm24.ShowWindow(SW_SHOW);
	m_editTmdm25.ShowWindow(SW_SHOW);
	m_editTmdm26.ShowWindow(SW_SHOW);
	m_editTmdm27.ShowWindow(SW_SHOW);
	m_editTmdm28.ShowWindow(SW_SHOW);
	m_editTmdm29.ShowWindow(SW_SHOW);
	m_editTmdm30.ShowWindow(SW_SHOW);
	m_editTmdm31.ShowWindow(SW_SHOW);
	m_editTmdm32.ShowWindow(SW_SHOW);
	m_editTmdm33.ShowWindow(SW_SHOW);
	m_editTmdm34.ShowWindow(SW_SHOW);
	m_editTmdm35.ShowWindow(SW_SHOW);
	m_editTmdm36.ShowWindow(SW_SHOW);
	m_editTmdm37.ShowWindow(SW_SHOW);
	m_editTmdm38.ShowWindow(SW_SHOW);
	m_editTmdm39.ShowWindow(SW_SHOW);
	m_editTmdm40.ShowWindow(SW_SHOW);
	m_editTmdm41.ShowWindow(SW_SHOW);
	m_editTmdm42.ShowWindow(SW_SHOW);
	m_editTmdm43.ShowWindow(SW_SHOW);
	m_editTmdm44.ShowWindow(SW_SHOW);
	m_editTmdm45.ShowWindow(SW_SHOW);
	m_editTmdm46.ShowWindow(SW_SHOW);
	m_editTmdm47.ShowWindow(SW_SHOW);
	m_editTmdm48.ShowWindow(SW_SHOW);
	m_editTmdm49.ShowWindow(SW_SHOW);
}
void CLiuHeCai::HideTmdmCtrl()
{
	m_editTmdm1.ShowWindow(SW_HIDE);
	m_editTmdm2.ShowWindow(SW_HIDE);
	m_editTmdm3.ShowWindow(SW_HIDE);
	m_editTmdm4.ShowWindow(SW_HIDE);
	m_editTmdm5.ShowWindow(SW_HIDE);
	m_editTmdm6.ShowWindow(SW_HIDE);
	m_editTmdm7.ShowWindow(SW_HIDE);
	m_editTmdm8.ShowWindow(SW_HIDE);
	m_editTmdm9.ShowWindow(SW_HIDE);
	m_editTmdm10.ShowWindow(SW_HIDE);
	m_editTmdm11.ShowWindow(SW_HIDE);
	m_editTmdm12.ShowWindow(SW_HIDE);
	m_editTmdm13.ShowWindow(SW_HIDE);
	m_editTmdm14.ShowWindow(SW_HIDE);
	m_editTmdm15.ShowWindow(SW_HIDE);
	m_editTmdm16.ShowWindow(SW_HIDE);
	m_editTmdm17.ShowWindow(SW_HIDE);
	m_editTmdm18.ShowWindow(SW_HIDE);
	m_editTmdm19.ShowWindow(SW_HIDE);
	m_editTmdm20.ShowWindow(SW_HIDE);
	m_editTmdm21.ShowWindow(SW_HIDE);
	m_editTmdm22.ShowWindow(SW_HIDE);
	m_editTmdm23.ShowWindow(SW_HIDE);
	m_editTmdm24.ShowWindow(SW_HIDE);
	m_editTmdm25.ShowWindow(SW_HIDE);
	m_editTmdm26.ShowWindow(SW_HIDE);
	m_editTmdm27.ShowWindow(SW_HIDE);
	m_editTmdm28.ShowWindow(SW_HIDE);
	m_editTmdm29.ShowWindow(SW_HIDE);
	m_editTmdm30.ShowWindow(SW_HIDE);
	m_editTmdm31.ShowWindow(SW_HIDE);
	m_editTmdm32.ShowWindow(SW_HIDE);
	m_editTmdm33.ShowWindow(SW_HIDE);
	m_editTmdm34.ShowWindow(SW_HIDE);
	m_editTmdm35.ShowWindow(SW_HIDE);
	m_editTmdm36.ShowWindow(SW_HIDE);
	m_editTmdm37.ShowWindow(SW_HIDE);
	m_editTmdm38.ShowWindow(SW_HIDE);
	m_editTmdm39.ShowWindow(SW_HIDE);
	m_editTmdm40.ShowWindow(SW_HIDE);
	m_editTmdm41.ShowWindow(SW_HIDE);
	m_editTmdm42.ShowWindow(SW_HIDE);
	m_editTmdm43.ShowWindow(SW_HIDE);
	m_editTmdm44.ShowWindow(SW_HIDE);
	m_editTmdm45.ShowWindow(SW_HIDE);
	m_editTmdm46.ShowWindow(SW_HIDE);
	m_editTmdm47.ShowWindow(SW_HIDE);
	m_editTmdm48.ShowWindow(SW_HIDE);
	m_editTmdm49.ShowWindow(SW_HIDE);
}

void CLiuHeCai::ShowTmdxdsCtrl()
{
	m_editTmdxdsXiao.ShowWindow(SW_SHOW);
	m_editTmdxdsDa.ShowWindow(SW_SHOW);
	m_editTmdxdsDan.ShowWindow(SW_SHOW);
	m_editTmdxdsShuang.ShowWindow(SW_SHOW);
	return;
}
void CLiuHeCai::HideTmdxdsCtrl()
{
	m_editTmdxdsXiao.ShowWindow(SW_HIDE);
	m_editTmdxdsDa.ShowWindow(SW_HIDE);
	m_editTmdxdsDan.ShowWindow(SW_HIDE);
	m_editTmdxdsShuang.ShowWindow(SW_HIDE);
	return;
}
void CLiuHeCai::ShowTmsxCtrl()
{
	m_editTmsxShu.ShowWindow(SW_SHOW);
	m_editTmsxNiu.ShowWindow(SW_SHOW);
	m_editTmsxHu.ShowWindow(SW_SHOW);
	m_editTmsxTu.ShowWindow(SW_SHOW);
	m_editTmsxLong.ShowWindow(SW_SHOW);
	m_editTmsxShe.ShowWindow(SW_SHOW);
	m_editTmsxMa.ShowWindow(SW_SHOW);
	m_editTmsxYang.ShowWindow(SW_SHOW);
	m_editTmsxHou.ShowWindow(SW_SHOW);
	m_editTmsxJi.ShowWindow(SW_SHOW);
	m_editTmsxGou.ShowWindow(SW_SHOW);
	m_editTmsxZhu.ShowWindow(SW_SHOW);
	return;
}
void CLiuHeCai::HideTmsxCtrl()
{
	m_editTmsxShu.ShowWindow(SW_HIDE);
	m_editTmsxNiu.ShowWindow(SW_HIDE);
	m_editTmsxHu.ShowWindow(SW_HIDE);
	m_editTmsxTu.ShowWindow(SW_HIDE);
	m_editTmsxLong.ShowWindow(SW_HIDE);
	m_editTmsxShe.ShowWindow(SW_HIDE);
	m_editTmsxMa.ShowWindow(SW_HIDE);
	m_editTmsxYang.ShowWindow(SW_HIDE);
	m_editTmsxHou.ShowWindow(SW_HIDE);
	m_editTmsxJi.ShowWindow(SW_HIDE);
	m_editTmsxGou.ShowWindow(SW_HIDE);
	m_editTmsxZhu.ShowWindow(SW_HIDE);
	return;
}
void CLiuHeCai::ShowZmtCtrl()
{
	m_btnZmtm1.ShowWindow(SW_SHOW);
	m_btnZmtm2.ShowWindow(SW_SHOW);
	m_btnZmtm3.ShowWindow(SW_SHOW);
	m_btnZmtm4.ShowWindow(SW_SHOW);
	m_btnZmtm5.ShowWindow(SW_SHOW);
	m_btnZmtm6.ShowWindow(SW_SHOW);
}
void CLiuHeCai::HideZmtCtrl()
{
	m_btnZmtm1.ShowWindow(SW_HIDE);
	m_btnZmtm2.ShowWindow(SW_HIDE);
	m_btnZmtm3.ShowWindow(SW_HIDE);
	m_btnZmtm4.ShowWindow(SW_HIDE);
	m_btnZmtm5.ShowWindow(SW_HIDE);
	m_btnZmtm6.ShowWindow(SW_HIDE);
}
void CLiuHeCai::ShowTmbsCtrl()
{
	m_editTmbshb.ShowWindow(SW_SHOW);
	m_editTmbslb.ShowWindow(SW_SHOW);
	m_editTmbslvb.ShowWindow(SW_SHOW);
	return;
}
void CLiuHeCai::HideTmbsCtrl()
{
	m_editTmbshb.ShowWindow(SW_HIDE);
	m_editTmbslb.ShowWindow(SW_HIDE);
	m_editTmbslvb.ShowWindow(SW_HIDE);
	return;
}
void CLiuHeCai::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	CDC cacheDC;
	cacheDC.CreateCompatibleDC(&dc);
	cacheDC.SetBkMode(TRANSPARENT);

	CBitmap cacheBmp;
	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

	Graphics graphics(cacheDC.m_hDC);
	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 2, 28, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	if (m_gameKind == LiuHeCai_Tmdm)
	{
		graphics.DrawImage(m_bmpTmdm, Rect(0, 140+19, m_bmpTmdm->GetWidth(), m_bmpTmdm->GetHeight()), 0, 0, m_bmpTmdm->GetWidth(), m_bmpTmdm->GetHeight(), UnitPixel);

	}
	else if(m_gameKind == LiuHeCai_Pmdm)
	{
		graphics.DrawImage(m_bmpTmpm, Rect(0, 140+19, m_bmpTmpm->GetWidth(), m_bmpTmpm->GetHeight()), 0, 0, m_bmpTmpm->GetWidth(), m_bmpTmpm->GetHeight(), UnitPixel);
	}
	else if(m_gameKind == LiuHeCai_Tmds)
	{
		graphics.DrawImage(m_bmpTmds, Rect(0, 140+19, m_bmpTmds->GetWidth(), m_bmpTmds->GetHeight()+5), 0, 0, m_bmpTmds->GetWidth(), m_bmpTmds->GetHeight()+5, UnitPixel);
	}
 	else if(m_gameKind == LiuHeCai_Zm1t||m_gameKind == LiuHeCai_Zm2t||m_gameKind == LiuHeCai_Zm3t||m_gameKind == LiuHeCai_Zm4t||m_gameKind == LiuHeCai_Zm5t||m_gameKind == LiuHeCai_Zm6t)
 	{
 		graphics.DrawImage(m_bmpTmdx, Rect(0, 140+19, m_bmpTmdx->GetWidth(), m_bmpTmdx->GetHeight()), 0, 0, m_bmpTmdx->GetWidth(), m_bmpTmdx->GetHeight(), UnitPixel);
 	}
	else if(m_gameKind == LiuHeCai_Tmsx)
	{
		//������Ф������ͼ
		graphics.DrawImage(m_bmpTmsx, Rect(0, 140+19, m_bmpTmsx->GetWidth(), m_bmpTmsx->GetHeight()), 0, 0, m_bmpTmsx->GetWidth(), m_bmpTmsx->GetHeight(), UnitPixel);
		//������Ф����Фͼ
		if(m_pngShengXiao)
		{
			for(int n=0; n<12; n++)
			{
				CPoint pt;
				GetShengXiaoPos(n, pt);
				int nImgSrcX=0;
				if(n % 2 > 0)
					nImgSrcX = m_pngShengXiao->GetWidth()/2;
				int nImgSrcY= n /2 * (m_pngShengXiao->GetHeight()/6);
				//ע�⣺PC�����ڸ���λ��ƫС�������Ҫ����-5������С��ʾ
				graphics.DrawImage(m_pngShengXiao, Rect(pt.x, pt.y, m_pngShengXiao->GetWidth()/2 , m_pngShengXiao->GetHeight()/6),
					nImgSrcX, nImgSrcY, m_pngShengXiao->GetWidth()/2, m_pngShengXiao->GetHeight()/6, UnitPixel);
				CString strPeiLV = mapFandian[LiuHeCai_Tmsx];
				strPeiLV.Replace(TEXT("����"), TEXT(""));
				strPeiLV.Replace(TEXT(":"), TEXT(""));
				float fPeiLv = _wtof(strPeiLV.GetBuffer(0));
				if( (n+m_nShengXiaoWuCha) == 10)
					fPeiLv = fPeiLv *3/4.0;
				strPeiLV.Format(TEXT("%.02f"), fPeiLv);
				cacheDC.DrawText(strPeiLV,strPeiLV.GetLength(), CRect(pt.x-200, pt.y, pt.x, pt.y+28), DT_CENTER|DT_VCENTER|DT_SINGLELINE); 	
			}
		}
	}
	else if(m_gameKind == LiuHeCai_Tmbs)
	{
		graphics.DrawImage(m_bmpTmbs, Rect(0, 140+18, m_bmpTmbs->GetWidth(), m_bmpTmbs->GetHeight()), 0, 0, m_bmpTmbs->GetWidth(), m_bmpTmbs->GetHeight(), UnitPixel);
	}
 	//���ƿ�����
 	DrawLastKjHaoma(&cacheDC, graphics);
 	DrawTopKjHaoma(&cacheDC, graphics);
// 
// 	//������ע�����ܽ��
// 	DrawZongZhushuJinE(&cacheDC);

	DrawZongZhushuJinE(&cacheDC);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheDC.DeleteDC();
	cacheBmp.DeleteObject();
}

BOOL CLiuHeCai::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


//��Ф����Ļλ��ͼ
//	x1,y1			x2,y1
//	x1,y2			x2,y2
//	x1,y3			x2,y3
//	x1,y4			x2,y4
//	x1,y5			x2,y5
//	x1,y6			x2,y6
//	m_nShengXiaoWuCha:��Ф��λ��2017��ΪԪ�꣬������������
bool CLiuHeCai::GetShengXiaoPos(int Index, CPoint& ptOut)
{
	int nX1	=	200;			//��ʼλ��:���ϵ�һ��X
	int nY1 =	223;			//��ʼλ�ã����ϵ�һ��Y
	int nX2 =nX1 + 390;			//�ұ�X= ���X + һ���̶�ֵ
	int nYOffset=28;			//Yƫ��ֵ
	if(Index < 0 || Index > 12)
		return false;
	CPoint	ArrPt[12];
	for(int k=0; k<12; k++)
	{
		if(k%2 == 0)
			ArrPt[k].x = nX1;
		else
			ArrPt[k].x = nX2;

		ArrPt[k].y = nY1 + k/2 * nYOffset;
	}

	int nNewIndex = Index + m_nShengXiaoWuCha;
	if(nNewIndex >= 12)
		nNewIndex -= 12;
	ptOut.x = ArrPt[nNewIndex].x;
	ptOut.y = ArrPt[nNewIndex].y;

	return true;
}


//��Ф��˳��ת��
int	CLiuHeCai::GetShengXiaoIndex(int nIndex, int nYearXiuZheng)
{
	if((nIndex + nYearXiuZheng) >= 12)
		return 0;
	else
		return nIndex + nYearXiuZheng;
}

void CLiuHeCai::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (GetSafeHwnd() == NULL) 
	{
		return;
	}
	if (m_list3D.GetSafeHwnd() != NULL) 
	{
		m_list3D.SetWindowPos(NULL, list_haomao_x, list_haomao_y, list_haomao_width, list_haomao_height, SWP_NOZORDER);
	}

	if (m_btn3DAdd.GetSafeHwnd() != NULL) 
	{
		m_btn3DAdd.SetWindowPos(NULL, btn_chq_add_x, btn_chq_add_y, m_btn3DAdd.Width(), m_btn3DAdd.Height(), SWP_NOZORDER);
	}

	if(m_btnTouZhu.GetSafeHwnd() != NULL) 
	{
		m_btnTouZhu.SetWindowPos(NULL, btn_touzhu_x, btn_touzhu_y, m_btnTouZhu.Width(), m_btnTouZhu.Height(), SWP_NOZORDER);
	}

	if(m_btnClsList.GetSafeHwnd() != NULL) 
	{
		m_btnClsList.SetWindowPos(NULL, btn_cls_list_x, btn_cls_list_y, m_btnClsList.Width(), m_btnClsList.Height(), SWP_NOZORDER);
	}
	if (m_btnLock.GetSafeHwnd() != NULL) 
	{
		m_btnLock.SetWindowPos(NULL, btn_lock_x, btn_lock_y, m_btnLock.Width(), m_btnLock.Height(), SWP_NOZORDER);
	}

	int nSpace = m_btnTmdm.Width()+10 ;
	if(m_btnTmdm.GetSafeHwnd() != NULL)
	{
		m_btnTmdm.SetWindowPos(NULL, btn_startX, btn_startY, m_btnTmdm.Width(), m_btnTmdm.Height(), SWP_NOZORDER);
	}
	if(m_btnTmds.GetSafeHwnd() != NULL)
	{
		m_btnTmds.SetWindowPos(NULL, btn_startX + nSpace, btn_startY, m_btnTmds.Width(), m_btnTmds.Height(), SWP_NOZORDER);
	}
	if(m_btnTmdx.GetSafeHwnd() != NULL)
	{
		m_btnTmdx.SetWindowPos(NULL, btn_startX + nSpace*5, btn_startY, m_btnTmdx.Width(), m_btnTmdx.Height(), SWP_NOZORDER);
		//m_btnTmdx.ShowWindow(SW_HIDE);
	}
	if(m_btnTmsx.GetSafeHwnd() != NULL)
	{
		m_btnTmsx.SetWindowPos(NULL, btn_startX + nSpace*2, btn_startY, m_btnTmsx.Width(), m_btnTmsx.Height(), SWP_NOZORDER);
	}
	if(m_btnTmbs.GetSafeHwnd() != NULL)
	{
		m_btnTmbs.SetWindowPos(NULL, btn_startX + nSpace*3, btn_startY, m_btnTmbs.Width(), m_btnTmbs.Height(), SWP_NOZORDER);
	}
	if(m_btnPmdm.GetSafeHwnd() != NULL)
	{
		m_btnPmdm.SetWindowPos(NULL, btn_startX + nSpace*4, btn_startY, m_btnPmdm.Width(), m_btnPmdm.Height(), SWP_NOZORDER);
	}
	/*������*/
	int nStartX = btn_startX - 5;

	if(m_btnZmtm1.GetSafeHwnd() != NULL)
	{
		m_btnZmtm1.SetWindowPos(NULL, nStartX + wanfa_sel_width, wanfa_sel_y+5-1,wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);

	}
	if(m_btnZmtm2.GetSafeHwnd() != NULL)
	{
		m_btnZmtm2.SetWindowPos(NULL, nStartX + wanfa_sel_width*2, wanfa_sel_y+5-1, wanfa_sel_width,wanfa_sel_height, SWP_NOZORDER);

	}
	if(m_btnZmtm3.GetSafeHwnd() != NULL)
	{
		m_btnZmtm3.SetWindowPos(NULL, nStartX + wanfa_sel_width*3, wanfa_sel_y+5-1,wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);

	}
	if(m_btnZmtm4.GetSafeHwnd() != NULL)
	{
		m_btnZmtm4.SetWindowPos(NULL, nStartX + wanfa_sel_width*4, wanfa_sel_y+5-1, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);

	}
	if(m_btnZmtm5.GetSafeHwnd() != NULL)
	{
		m_btnZmtm5.SetWindowPos(NULL, nStartX + wanfa_sel_width*5, wanfa_sel_y+5-1, wanfa_sel_width,wanfa_sel_height, SWP_NOZORDER);

	}
	if(m_btnZmtm6.GetSafeHwnd() != NULL)
	{
		m_btnZmtm6.SetWindowPos(NULL, nStartX + wanfa_sel_width*6, wanfa_sel_y+5-1, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);

	}
	/*������*/
	int nEditWidth = 46;
	int nEditHeight = 17;
	int nEditSpanx = 112;
	int nEditSpany = 26;
	if(m_editTmdm1.GetSafeHwnd() != NULL)
	{
		m_editTmdm1.SetWindowPos(NULL, edit_tmdm_x , edit_tmdm_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm2.GetSafeHwnd() != NULL)
	{
		m_editTmdm2.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx , edit_tmdm_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm3.GetSafeHwnd() != NULL)
	{
		m_editTmdm3.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*2 , edit_tmdm_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm4.GetSafeHwnd() != NULL)
	{
		m_editTmdm4.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*3 , edit_tmdm_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm5.GetSafeHwnd() != NULL)
	{
		m_editTmdm5.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*4, edit_tmdm_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm6.GetSafeHwnd() != NULL)
	{
		m_editTmdm6.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*5 , edit_tmdm_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm7.GetSafeHwnd() != NULL)
	{
		m_editTmdm7.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*6 , edit_tmdm_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm8.GetSafeHwnd() != NULL)
	{
		m_editTmdm8.SetWindowPos(NULL, edit_tmdm_x , edit_tmdm_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}

	if(m_editTmdm9.GetSafeHwnd() != NULL)
	{
		m_editTmdm9.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx , edit_tmdm_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm10.GetSafeHwnd() != NULL)
	{
		m_editTmdm10.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*2 , edit_tmdm_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm11.GetSafeHwnd() != NULL)
	{
		m_editTmdm11.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*3 , edit_tmdm_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm12.GetSafeHwnd() != NULL)
	{
		m_editTmdm12.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*4, edit_tmdm_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm13.GetSafeHwnd() != NULL)
	{
		m_editTmdm13.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*5 , edit_tmdm_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm14.GetSafeHwnd() != NULL)
	{
		m_editTmdm14.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*6 , edit_tmdm_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm15.GetSafeHwnd() != NULL)
	{
		m_editTmdm15.SetWindowPos(NULL, edit_tmdm_x  , edit_tmdm_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm16.GetSafeHwnd() != NULL)
	{
		m_editTmdm16.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx , edit_tmdm_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}


	if(m_editTmdm17.GetSafeHwnd() != NULL)
	{
		m_editTmdm17.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*2 , edit_tmdm_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm18.GetSafeHwnd() != NULL)
	{
		m_editTmdm18.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*3 , edit_tmdm_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm19.GetSafeHwnd() != NULL)
	{
		m_editTmdm19.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*4 , edit_tmdm_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm20.GetSafeHwnd() != NULL)
	{
		m_editTmdm20.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*5, edit_tmdm_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm21.GetSafeHwnd() != NULL)
	{
		m_editTmdm21.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*6 , edit_tmdm_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm22.GetSafeHwnd() != NULL)
	{
		m_editTmdm22.SetWindowPos(NULL, edit_tmdm_x  , edit_tmdm_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm23.GetSafeHwnd() != NULL)
	{
		m_editTmdm23.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx , edit_tmdm_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm24.GetSafeHwnd() != NULL)
	{
		m_editTmdm24.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*2 , edit_tmdm_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm25.GetSafeHwnd() != NULL)
	{
		m_editTmdm25.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*3 , edit_tmdm_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm26.GetSafeHwnd() != NULL)
	{
		m_editTmdm26.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*4 , edit_tmdm_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm27.GetSafeHwnd() != NULL)
	{
		m_editTmdm27.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*5 , edit_tmdm_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm28.GetSafeHwnd() != NULL)
	{
		m_editTmdm28.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*6 , edit_tmdm_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm29.GetSafeHwnd() != NULL)
	{
		m_editTmdm29.SetWindowPos(NULL, edit_tmdm_x  , edit_tmdm_y+nEditSpany*4, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm30.GetSafeHwnd() != NULL)
	{
		m_editTmdm30.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx , edit_tmdm_y+nEditSpany*4, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm31.GetSafeHwnd() != NULL)
	{
		m_editTmdm31.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*2 , edit_tmdm_y+nEditSpany*4, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm32.GetSafeHwnd() != NULL)
	{
		m_editTmdm32.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*3  , edit_tmdm_y+nEditSpany*4, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm33.GetSafeHwnd() != NULL)
	{
		m_editTmdm33.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*4 , edit_tmdm_y+nEditSpany*4, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm34.GetSafeHwnd() != NULL)
	{
		m_editTmdm34.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*5 , edit_tmdm_y+nEditSpany*4, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm35.GetSafeHwnd() != NULL)
	{
		m_editTmdm35.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*6 , edit_tmdm_y+nEditSpany*4, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm36.GetSafeHwnd() != NULL)
	{
		m_editTmdm36.SetWindowPos(NULL, edit_tmdm_x  , edit_tmdm_y+nEditSpany*5, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm37.GetSafeHwnd() != NULL)
	{
		m_editTmdm37.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx , edit_tmdm_y+nEditSpany*5, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm38.GetSafeHwnd() != NULL)
	{
		m_editTmdm38.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*2 , edit_tmdm_y+nEditSpany*5, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm39.GetSafeHwnd() != NULL)
	{
		m_editTmdm39.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*3 , edit_tmdm_y+nEditSpany*5, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm40.GetSafeHwnd() != NULL)
	{
		m_editTmdm40.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*4 , edit_tmdm_y+nEditSpany*5, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm41.GetSafeHwnd() != NULL)
	{
		m_editTmdm41.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*5 , edit_tmdm_y+nEditSpany*5, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm42.GetSafeHwnd() != NULL)
	{
		m_editTmdm42.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*6 , edit_tmdm_y+nEditSpany*5, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm43.GetSafeHwnd() != NULL)
	{
		m_editTmdm43.SetWindowPos(NULL, edit_tmdm_x  , edit_tmdm_y+nEditSpany*6, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm44.GetSafeHwnd() != NULL)
	{
		m_editTmdm44.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx , edit_tmdm_y+nEditSpany*6, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm45.GetSafeHwnd() != NULL)
	{
		m_editTmdm45.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*2 , edit_tmdm_y+nEditSpany*6, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm46.GetSafeHwnd() != NULL)
	{
		m_editTmdm46.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*3  , edit_tmdm_y+nEditSpany*6, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm47.GetSafeHwnd() != NULL)
	{
		m_editTmdm47.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*4 , edit_tmdm_y+nEditSpany*6, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm48.GetSafeHwnd() != NULL)
	{
		m_editTmdm48.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*5 , edit_tmdm_y+nEditSpany*6, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdm49.GetSafeHwnd() != NULL)
	{
		m_editTmdm49.SetWindowPos(NULL, edit_tmdm_x + nEditSpanx*6 , edit_tmdm_y+nEditSpany*6, nEditWidth, nEditHeight, SWP_NOZORDER);
	}


	if(m_editTmdxdsDa.GetSafeHwnd() != NULL)
	{
		m_editTmdxdsDa.SetWindowPos(NULL, edit_tmdxds_x  , edit_tmdxds_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdxdsXiao.GetSafeHwnd() != NULL)
	{
		m_editTmdxdsXiao.SetWindowPos(NULL, edit_tmdxds_x , edit_tmdxds_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdxdsDan.GetSafeHwnd() != NULL)
	{
		m_editTmdxdsDan.SetWindowPos(NULL, edit_tmdxds_x  , edit_tmdxds_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmdxdsShuang.GetSafeHwnd() != NULL)
	{
		m_editTmdxdsShuang.SetWindowPos(NULL, edit_tmdxds_x , edit_tmdxds_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	//��Ф
	nEditSpanx = 391;
	if(m_editTmsxShu.GetSafeHwnd() != NULL)
	{
		m_editTmsxShu.SetWindowPos(NULL, edit_tmsx_x  , edit_tmsx_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxNiu.GetSafeHwnd() != NULL)
	{
		m_editTmsxNiu.SetWindowPos(NULL, edit_tmsx_x+nEditSpanx , edit_tmsx_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxHu.GetSafeHwnd() != NULL)
	{
		m_editTmsxHu.SetWindowPos(NULL, edit_tmsx_x  , edit_tmsx_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxTu.GetSafeHwnd() != NULL)
	{
		m_editTmsxTu.SetWindowPos(NULL, edit_tmsx_x+nEditSpanx , edit_tmsx_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxLong.GetSafeHwnd() != NULL)
	{
		m_editTmsxLong.SetWindowPos(NULL, edit_tmsx_x  , edit_tmsx_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxShe.GetSafeHwnd() != NULL)
	{
		m_editTmsxShe.SetWindowPos(NULL, edit_tmsx_x+nEditSpanx , edit_tmsx_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxMa.GetSafeHwnd() != NULL)
	{
		m_editTmsxMa.SetWindowPos(NULL, edit_tmsx_x  , edit_tmsx_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxYang.GetSafeHwnd() != NULL)
	{
		m_editTmsxYang.SetWindowPos(NULL, edit_tmsx_x+nEditSpanx , edit_tmsx_y+nEditSpany*3, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxHou.GetSafeHwnd() != NULL)
	{
		m_editTmsxHou.SetWindowPos(NULL, edit_tmsx_x  , edit_tmsx_y+nEditSpany*4, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxJi.GetSafeHwnd() != NULL)
	{
		m_editTmsxJi.SetWindowPos(NULL, edit_tmsx_x+nEditSpanx , edit_tmsx_y+nEditSpany*4, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxGou.GetSafeHwnd() != NULL)
	{
		m_editTmsxGou.SetWindowPos(NULL, edit_tmsx_x  , edit_tmsx_y+nEditSpany*5, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmsxZhu.GetSafeHwnd() != NULL)
	{
		m_editTmsxZhu.SetWindowPos(NULL, edit_tmsx_x+nEditSpanx , edit_tmsx_y+nEditSpany*5, nEditWidth, nEditHeight, SWP_NOZORDER);
	}

	if(m_editTmbshb.GetSafeHwnd() != NULL)
	{
		m_editTmbshb.SetWindowPos(NULL, edit_tmbs_x , edit_tmbs_y, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmbslb.GetSafeHwnd() != NULL)
	{
		m_editTmbslb.SetWindowPos(NULL, edit_tmbs_x  , edit_tmbs_y+nEditSpany, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
	if(m_editTmbslvb.GetSafeHwnd() != NULL)
	{
		m_editTmbslvb.SetWindowPos(NULL, edit_tmbs_x , edit_tmbs_y+nEditSpany*2, nEditWidth, nEditHeight, SWP_NOZORDER);
	}
}

void CLiuHeCai::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	OnBnClickedBtnClsList();
	KillTimer(TimerLotteryStatus);
	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

 		m_btnLock.EnableWindow(false);
// 
// 		m_nMoshi = theAccount.Scoretype;
// 		DOUBLE fyue = theAccount.yue;
// 
		SendToServer(4);

 		SendToServer(6);
		m_cbIfTs =0;
		SendToServer(11);
		KillTimer(TimerLotteryStatus);
		SetTimer(TimerLotteryStatus, 30000, NULL);

		SetTimer(timer_update_resttime,1000,NULL);
		OnBnClickedBtnTmdm();
	}
}
void CLiuHeCai::FlushZongjine()
{
	DOUBLE fyue = 0.0f;
	DOUBLE fAcyue = 0.0f;
	for(int i = 1;i < AREA_TMDM_MAX;i++)
	{
		fyue += mapJetton[i];
	}

	if(m_nMoshi == MoShi_Yuan)
	{
		fAcyue = theAccount.yue;
	}
	else if(m_nMoshi == MoShi_Jiao)
	{
		fAcyue = theAccount.yue*10;
	}
	else if(m_nMoshi == MoShi_Fen)
	{
		fAcyue = theAccount.yue*100;
	}

	fyue = (fAcyue - fyue);

	//DOUBLE fJetton = 0.0f;
	if(theAccount.Scoretype > m_nMoshi)
	{
		if((theAccount.Scoretype - m_nMoshi)==1)
		{
			fyue = fyue*10;
		}
		else if((theAccount.Scoretype - m_nMoshi)==2)
		{
			fyue = fyue*100;
		}
	}
	else if(theAccount.Scoretype < m_nMoshi)
	{
		if((m_nMoshi - theAccount.Scoretype  )==1)
			fyue = fyue/10;
		else if((m_nMoshi - theAccount.Scoretype)==2)
			fyue = fyue/100;
	}



	for(int i = 1;i < AREA_TMDM_MAX;i++)
	{
		if(theAccount.Scoretype > m_nMoshi)
		{
			if((theAccount.Scoretype - m_nMoshi)==1)
			{
				mapJetton[i] = mapJetton[i]*10;
			}
			else if((theAccount.Scoretype - m_nMoshi)==2)
			{
				mapJetton[i] = mapJetton[i]*100;
			}
		}
		else if(theAccount.Scoretype < m_nMoshi)
		{
			if((m_nMoshi - theAccount.Scoretype  )==1)
				mapJetton[i] = mapJetton[i]/10;
			else if((m_nMoshi - theAccount.Scoretype)==2)
				mapJetton[i] = mapJetton[i]/100;
		}
	}


	m_nMoshi = theAccount.Scoretype;
	InvalidateRect(FALSE);
}

HBRUSH CLiuHeCai::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLiuHeCai::OnTimer(UINT_PTR nIDEvent)
{

	if(TimerFandian == nIDEvent)
	{
		SendToServer(6);
	}
	else if(nIDEvent == timer_update_resttime)
	{
		InvalidateRect(&rc_time_rest);
	}
	else if(TimerLotteryStatus == nIDEvent)
	{
		SendToServer(11);
	}
	else if(timer_id_kaijiangzhong == nIDEvent)
	{
		//������GIF����
		if(m_bKaiJiangzhong )
		{
			CDC* pDC = GetDC();
			if(pDC==NULL)
				return;
			//for(int n=0; n<m_nFrameCount; n++)
			{
				int nLeft=kj_big_haoma_x -17/*+ n *( m_bigNumWidth+8)*/;
				CMemDC	mDC(pDC, CRect(nLeft, kj_big_haoma_y-2, nLeft+m_imgKaijiang->GetWidth(), kj_big_haoma_y+m_imgKaijiang->GetHeight()-2));
				Graphics gh(mDC.m_hDC);
				gh.DrawImage(m_imgKaijiang, nLeft, kj_big_haoma_y-2, m_imgKaijiang->GetWidth(), m_imgKaijiang->GetHeight());
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
	else if(nIDEvent == timer_get_qihao)
	{
		KillTimer(timer_get_qihao);
		SetTimer(timer_get_qihao,3000,NULL);
		SendToServer(4);

	}
	CDialog::OnTimer(nIDEvent);

}
void CLiuHeCai::DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_bigNumWidth, 0, m_bigNumWidth, 60, UnitPixel);
}

void CLiuHeCai::DrawBigShengXiao(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	//��Ф��Ҫ�����������ġ�����
	int nEndShengXiao = num%12 - m_nShengXiaoWuCha;
	if(nEndShengXiao < 0)
		nEndShengXiao = 12;
	graphics.DrawImage(img, rc_dest, nEndShengXiao*m_bigNumWidth, 60, m_bigNumWidth, m_bigNumHeight-60, UnitPixel);
}
	
void CLiuHeCai::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_SmallNumWidth, 0, m_SmallNumWidth, m_SmallNumHeight, UnitPixel);
}

BOOL CLiuHeCai::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bigNumWidth=0;
	m_bigNumHeight=0;
	m_singlezhushu = 0;
	m_fSingleJine = 0.0f;
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\game_bk2.png"));
	m_bmpTmdm = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmdm.png"));
	m_bmpTmpm = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmpm.png"));
	m_bmpTmds = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmds.png"));
	m_bmpTmdx = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmztm.png"));
	m_bmpTmsx = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmsx.png"));
	m_bmpTmbs = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmbs.png"));
	
	//��Фͼ
	m_pngShengXiao = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\itemsx.png"));

	m_bKaiJiangzhong=false;
	m_font.CreateFont(17, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	m_zongFont.CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_KjInfoFont1.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 

	m_kjNumBig = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big5.png"));
	m_bigNumWidth = m_kjNumBig->GetWidth() / 50;
	m_bigNumHeight = m_kjNumBig->GetHeight();
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small5.png"));
	m_SmallNumWidth = m_kjNumSmall->GetWidth() / 50;
	m_SmallNumHeight = m_kjNumSmall->GetHeight();

	m_btnClsList.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_clear2.png"));
	m_btnTouZhu.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_bet.png"));
	m_btnLock.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_lock3.png"));
	m_btnLock.SetTextColor(RGB(51, 45, 42));
	m_btnLock.SetTextFont(&m_zongFont);
	m_btn3DAdd.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_addnum2.png"));
	m_editTmdm1.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm2.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm3.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm4.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm5.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm6.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm7.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm8.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm9.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm10.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm11.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm12.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm13.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm14.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm15.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm16.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm17.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm18.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm19.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm20.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm21.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm22.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm23.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm24.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm25.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm26.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm27.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm28.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm29.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm30.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm31.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm32.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm33.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm34.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm35.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm36.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm37.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm38.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm39.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm40.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm41.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm42.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm43.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm44.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm45.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm46.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm47.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm48.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdm49.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));

	m_editTmdxdsDa.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdxdsXiao.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdxdsDan.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmdxdsShuang.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));

	m_editTmsxShu.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxNiu.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxHu.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxTu.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxLong.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxShe.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxMa.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxYang.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxHou.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxJi.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxGou.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmsxZhu.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmbshb.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmbslb.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_editTmbslvb.SetEnableColor(RGB(113,48,20),RGB(255,255,255),RGB(255,255,255));
	m_nFrameCount=0;
	m_nFrameIndex=0;
	
	m_imgKaijiang = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\KaiJiang.gif"));
	int nCount = m_imgKaijiang->GetFrameDimensionsCount();			//��ȡ֡ά��
	GUID *pGuids = new GUID[nCount];								//����һ��GUID����
	m_imgKaijiang->GetFrameDimensionsList(pGuids,nCount);			//��ȡͼ��֡��GUID
	m_nFrameCount=m_imgKaijiang->GetFrameCount(pGuids);				//��ȡGIF֡��

	m_editTmdm1.SetFont(&m_font);
	m_editTmdm2.SetFont(&m_font);
	m_editTmdm3.SetFont(&m_font);
	m_editTmdm4.SetFont(&m_font);
	m_editTmdm5.SetFont(&m_font);
	m_editTmdm6.SetFont(&m_font);
	m_editTmdm7.SetFont(&m_font);
	m_editTmdm8.SetFont(&m_font);
	m_editTmdm9.SetFont(&m_font);
	m_editTmdm10.SetFont(&m_font);
	m_editTmdm11.SetFont(&m_font);
	m_editTmdm12.SetFont(&m_font);
	m_editTmdm13.SetFont(&m_font);
	m_editTmdm14.SetFont(&m_font);
	m_editTmdm15.SetFont(&m_font);
	m_editTmdm16.SetFont(&m_font);
	m_editTmdm17.SetFont(&m_font);
	m_editTmdm18.SetFont(&m_font);
	m_editTmdm19.SetFont(&m_font);
	m_editTmdm20.SetFont(&m_font);
	m_editTmdm21.SetFont(&m_font);
	m_editTmdm22.SetFont(&m_font);
	m_editTmdm23.SetFont(&m_font);
	m_editTmdm24.SetFont(&m_font);
	m_editTmdm25.SetFont(&m_font);
	m_editTmdm26.SetFont(&m_font);
	m_editTmdm27.SetFont(&m_font);
	m_editTmdm28.SetFont(&m_font);
	m_editTmdm29.SetFont(&m_font);
	m_editTmdm30.SetFont(&m_font);
	m_editTmdm31.SetFont(&m_font);
	m_editTmdm32.SetFont(&m_font);
	m_editTmdm33.SetFont(&m_font);
	m_editTmdm34.SetFont(&m_font);
	m_editTmdm35.SetFont(&m_font);
	m_editTmdm36.SetFont(&m_font);
	m_editTmdm37.SetFont(&m_font);
	m_editTmdm38.SetFont(&m_font);
	m_editTmdm39.SetFont(&m_font);
	m_editTmdm40.SetFont(&m_font);
	m_editTmdm41.SetFont(&m_font);
	m_editTmdm42.SetFont(&m_font);
	m_editTmdm43.SetFont(&m_font);
	m_editTmdm44.SetFont(&m_font);
	m_editTmdm45.SetFont(&m_font);
	m_editTmdm46.SetFont(&m_font);
	m_editTmdm47.SetFont(&m_font);
	m_editTmdm48.SetFont(&m_font);
	m_editTmdm49.SetFont(&m_font);
	m_editTmdxdsDa.SetFont(&m_font);
	m_editTmdxdsXiao.SetFont(&m_font);
	m_editTmdxdsDan.SetFont(&m_font);
	m_editTmdxdsShuang.SetFont(&m_font);
	m_editTmsxShu.SetFont(&m_font);
	m_editTmsxNiu.SetFont(&m_font);
	m_editTmsxHu.SetFont(&m_font);
	m_editTmsxTu.SetFont(&m_font);
	m_editTmsxLong.SetFont(&m_font);
	m_editTmsxShe.SetFont(&m_font);
	m_editTmsxMa.SetFont(&m_font);
	m_editTmsxYang.SetFont(&m_font);
	m_editTmsxHou.SetFont(&m_font);
	m_editTmsxJi.SetFont(&m_font);
	m_editTmsxGou.SetFont(&m_font);
	m_editTmsxZhu.SetFont(&m_font);
	m_editTmbshb.SetFont(&m_font);
	m_editTmbslb.SetFont(&m_font);
	m_editTmbslvb.SetFont(&m_font);
	InitBtns();
	m_list3D.SetExtendedStyle(m_list3D.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);
	m_list3D.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	m_list3D.SetPenColor(RGB(255,255,255));
	m_list3D.InsertColumn(0, _T("����"), LVCFMT_CENTER, 300);
	m_list3D.InsertColumn(1, _T("ģʽ"), LVCFMT_CENTER, 93);
	m_list3D.InsertColumn(2, _T("ע��"), LVCFMT_CENTER, 70);
	m_list3D.InsertColumn(3, _T("����"), LVCFMT_CENTER, 70);
	m_list3D.InsertColumn(4, _T(""), LVCFMT_CENTER, 30);

	return TRUE;
}

void CLiuHeCai::ResetAllEdit()
{
	int nFirst = 0;
	int nSecond = 0;
	if(m_gameKind == LiuHeCai_Tmdm||m_gameKind == LiuHeCai_Pmdm||m_gameKind == LiuHeCai_Zm1t||m_gameKind == LiuHeCai_Zm2t||m_gameKind == LiuHeCai_Zm3t||m_gameKind == LiuHeCai_Zm4t||m_gameKind == LiuHeCai_Zm5t||m_gameKind == LiuHeCai_Zm6t)
	{
		nFirst = IDC_EDIT_TMDM_1;
		nSecond = IDC_EDIT_TMDM_49;
	}
	else if (m_gameKind == LiuHeCai_Tmds)
	{
		nFirst = IDC_EDIT_TMDXDS_DA;
		nSecond = IDC_EDIT_TMDXDS_SHUANG;
	}
	else if (m_gameKind == LiuHeCai_Tmsx)
	{
		nFirst = IDC_EDIT_TMSX_SHU;
		nSecond = IDC_EDIT_TMSX_ZHU;
	}
	else if (m_gameKind == LiuHeCai_Tmbs)
	{
		nFirst = IDC_EDIT_TMBS_HB;
		nSecond = IDC_EDIT_TMBS_LVB;
	}

	for (int i = nFirst;i<=nSecond;i++)
	{
		CWnd* pWnd = GetDlgItem(i);
		if(pWnd!=NULL)
		{
			pWnd->SetWindowText(L"");
		}
	}

}

void CLiuHeCai::ResetJetton()
{
	m_singlezhushu = 0;
	m_fSingleJine = 0.0f;
	//ResetAllEdit();
	CRect rcRect(0,140,791,266+140);
	RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
// 	rcRect.SetRect(192,539,769,561);
// 	RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	return;
}

void CLiuHeCai::InitBtns()
{
 
 	m_btnZmtm1.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_zm1t.png"));
	m_btnZmtm2.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_zm2t.png"));
	m_btnZmtm3.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_zm3t.png"));
	m_btnZmtm4.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_zm4t.png"));
	m_btnZmtm5.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_zm5t.png"));
	m_btnZmtm6.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_zm6t.png"));

}
//���뵥��
void CLiuHeCai::OnBnClickedBtnTmdm()
{
	m_btnLock.ShowWindow(SW_SHOW);
	ResetAllEdit();
	m_gameKind = LiuHeCai_Tmdm;
	m_btnTmdm.SetPushed(true);
	m_btnTmds.SetPushed(false);
	m_btnTmdx.SetPushed(false);
	m_btnTmsx.SetPushed(false);
	m_btnTmbs.SetPushed(false);
	m_btnPmdm.SetPushed(false);
	HideZmtCtrl();
	HideTmbsCtrl();
	HideTmsxCtrl();
	HideTmdxdsCtrl();
	ShowTmdmCtrl();
	SendToServer(3);
	return;
}
//ƽ�뵥��
void CLiuHeCai::OnBnClickedBtnPmdm()
{
	m_btnLock.ShowWindow(SW_SHOW);
	ResetAllEdit();
	m_gameKind = LiuHeCai_Pmdm;
	m_btnTmdm.SetPushed(false);
	m_btnTmds.SetPushed(false);
	m_btnTmdx.SetPushed(false);
	m_btnTmsx.SetPushed(false);
	m_btnTmbs.SetPushed(false);
	m_btnPmdm.SetPushed(true);
	HideZmtCtrl();
	HideTmbsCtrl();
	HideTmsxCtrl();
	HideTmdxdsCtrl();
	ShowTmdmCtrl();
	SendToServer(3);
	return;
}
//���뵥˫
void CLiuHeCai::OnBnClickedBtnTmds()
{
	m_btnLock.ShowWindow(SW_HIDE);
	ResetAllEdit();
	m_gameKind = LiuHeCai_Tmds;
	m_btnTmdm.SetPushed(false);
	m_btnTmds.SetPushed(true);
	m_btnTmdx.SetPushed(false);
	m_btnTmsx.SetPushed(false);
	m_btnTmbs.SetPushed(false);
	m_btnPmdm.SetPushed(false);
	HideZmtCtrl();
	HideTmbsCtrl();
	HideTmsxCtrl();
	HideTmdmCtrl();
	ShowTmdxdsCtrl();
	SendToServer(3);
	return;
}
//������
void CLiuHeCai::OnBnClickedBtnTmdx()
{
	ResetAllEdit();
	m_btnLock.ShowWindow(SW_SHOW);
	
	m_btnTmdm.SetPushed(false);
	m_btnTmds.SetPushed(false);
	m_btnTmdx.SetPushed(true);
	m_btnTmsx.SetPushed(false);
	m_btnTmbs.SetPushed(false);
	m_btnPmdm.SetPushed(false);

	HideTmbsCtrl();
	HideTmsxCtrl();
	HideTmdxdsCtrl();
	ShowTmdmCtrl();
	ShowZmtCtrl();
	m_btnZmtm1.ShowWindow(SW_SHOW);
	m_btnZmtm2.ShowWindow(SW_SHOW);
	m_btnZmtm3.ShowWindow(SW_SHOW);
	m_btnZmtm4.ShowWindow(SW_SHOW);
	m_btnZmtm5.ShowWindow(SW_SHOW);
	m_btnZmtm6.ShowWindow(SW_SHOW);

	OnBnClickedBtnZm1t();
	return;
}
//������Ф
void CLiuHeCai::OnBnClickedBtnTmsx()
{
	ResetAllEdit();
	m_btnLock.ShowWindow(SW_HIDE);
	m_gameKind = LiuHeCai_Tmsx;
	m_btnTmdm.SetPushed(false);
	m_btnTmds.SetPushed(false);
	m_btnTmdx.SetPushed(false);
	m_btnTmsx.SetPushed(true);
	m_btnTmbs.SetPushed(false);
	m_btnPmdm.SetPushed(false);
	HideZmtCtrl();
	HideTmbsCtrl();
	ShowTmsxCtrl();
	HideTmdxdsCtrl();
	HideTmdmCtrl();
	SendToServer(3);

	return;
}
//���벨ɫ
void CLiuHeCai::OnBnClickedBtnTmbs()
{
	ResetAllEdit();
	m_gameKind = LiuHeCai_Tmbs;
	m_btnLock.ShowWindow(SW_HIDE);
	m_btnTmdm.SetPushed(false);
	m_btnTmds.SetPushed(false);
	m_btnTmdx.SetPushed(false);
	m_btnTmsx.SetPushed(false);
	m_btnTmbs.SetPushed(true);
	m_btnPmdm.SetPushed(false);
	HideZmtCtrl();
	HideTmsxCtrl();
	HideTmdxdsCtrl();
	HideTmdmCtrl();
	ShowTmbsCtrl();
	SendToServer(3);

	return;

}
CString CLiuHeCai::GetWanDesc()
{
	return theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)m_gameKind];
}

void CLiuHeCai::OnBnClickedBtnTouZhu()
{
	CTime Now;
	theApp.GetTime(Now);

	CTimeSpan timespan = m_NextTime-Now;
	if(timespan.GetTotalSeconds()<=0)
	{
		MyMessageBox(L"���ڷⵥ");
		return;
	}

	int nCount = m_list3D.GetItemCount();
	if(nCount <= 0)
	{
		MyMessageBox(L"��ѡ��Ͷע���ݣ�");
		return;

	}
	CQrTzhDlg qrDlg;
	qrDlg.m_gamekind = _T("������ϲ�");
	qrDlg.m_zhushu = m_zongZhuShu;
	qrDlg.m_moshi = theAccount.Scoretype;
	qrDlg.m_zongjine = m_zongJine;
	if (qrDlg.DoModal() != IDOK)
	{
		return;
	}


	SendToServer(1);
	return;
}
void CLiuHeCai::OnBnClickedBtnAdd()
{
	int nFirst = 0;
	int nSecond = 0;
	if(m_gameKind == LiuHeCai_Tmdm||m_gameKind == LiuHeCai_Pmdm||m_gameKind == LiuHeCai_Zm1t||m_gameKind == LiuHeCai_Zm2t||m_gameKind == LiuHeCai_Zm3t||m_gameKind == LiuHeCai_Zm4t||m_gameKind == LiuHeCai_Zm5t||m_gameKind == LiuHeCai_Zm6t)
	{
		nFirst = IDC_EDIT_TMDM_1;
		nSecond = IDC_EDIT_TMDM_49;
	}
	else if (m_gameKind == LiuHeCai_Tmds)
	{
		nFirst = IDC_EDIT_TMDXDS_DA;
		nSecond = IDC_EDIT_TMDXDS_SHUANG;
	}
	else if (m_gameKind == LiuHeCai_Tmsx)
	{
		nFirst = IDC_EDIT_TMSX_SHU;
		nSecond = IDC_EDIT_TMSX_ZHU;
	}
	else if (m_gameKind == LiuHeCai_Tmbs)
	{
		nFirst = IDC_EDIT_TMBS_HB;
		nSecond = IDC_EDIT_TMBS_LVB;
	}
	else
	{
		return;
	}
	bool bADD = false;
	for(int i = nFirst;i <= nSecond;i++)
	{
		CString strBeishu;
		GetDlgItemText(i,strBeishu);
		int jine = _ttoi(strBeishu);
		if(jine>0)
		{
			bADD = true;
			CString strAddHaoma;
			if(m_gameKind == LiuHeCai_Tmdm||m_gameKind == LiuHeCai_Pmdm||m_gameKind == LiuHeCai_Zm1t||m_gameKind == LiuHeCai_Zm2t||m_gameKind == LiuHeCai_Zm3t||m_gameKind == LiuHeCai_Zm4t||m_gameKind == LiuHeCai_Zm5t||m_gameKind == LiuHeCai_Zm6t)
				strAddHaoma.Format(L"%02d",(i-nFirst+1));
			else if(m_gameKind == LiuHeCai_Tmds)
			{
				CString strWanfa[4] = {L"��",L"С",L"��",L"˫"};
				strAddHaoma.Format(L"%s",strWanfa[i-nFirst]);
			}
			else if(m_gameKind == LiuHeCai_Tmsx)
			{
				CString strWanfa[12] = {L"��",L"ţ",L"��",L"��",L"��",L"��",L"��",L"��",L"��",L"��",L"��",L"��"};
				strAddHaoma.Format(L"%s",strWanfa[(i-nFirst) ]);
			}
			else if (m_gameKind == LiuHeCai_Tmbs)
			{
				CString strWanfa[3] = {L"�첨",L"����",L"�̲�"};
				strAddHaoma.Format(L"%s",strWanfa[i-nFirst]);

			}

			strAddHaoma +=_T("|");
			int nCount = m_list3D.GetItemCount();
			m_list3D.InsertItem(nCount, strAddHaoma);
			m_list3D.SetItemText(nCount, 1, GetWanDesc());
			m_list3D.SetItemText(nCount, 2,L"1");
			m_list3D.SetItemText(nCount, 3,strBeishu);
			m_list3D.createItemButton(nCount, 4, this->GetSafeHwnd());

		}

	}
	if(!bADD)
	{
		MyMessageBox(L"��δѡ��Ͷע���룡");
		return ;
	}

	
	int nItemCount = m_list3D.GetItemCount();

	m_zongZhuShu = 0;
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_list3D.GetItemText(i,3);
		CString strZhuSHu = m_list3D.GetItemText(i,2);
		int beishu = _ttoi(strBeiSHu);
		int zhushu = _ttoi(strZhuSHu);

		m_zongZhuShu += zhushu;
		m_zongJine += beishu;
	}


	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);



	ResetAllEdit();
	return;
}

void CLiuHeCai::OnBnClickedBtnZm1t()
{
	m_gameKind = LiuHeCai_Zm1t;
	SendToServer(3);
	m_btnZmtm1.SetPushed(true);
	m_btnZmtm2.SetPushed(false);
	m_btnZmtm3.SetPushed(false);
	m_btnZmtm4.SetPushed(false);
	m_btnZmtm5.SetPushed(false);
	m_btnZmtm6.SetPushed(false);
}
void CLiuHeCai::OnBnClickedBtnZm2t()
{
	m_gameKind = LiuHeCai_Zm2t;
	SendToServer(3);

	m_btnZmtm1.SetPushed(false);
	m_btnZmtm2.SetPushed(true);
	m_btnZmtm3.SetPushed(false);
	m_btnZmtm4.SetPushed(false);
	m_btnZmtm5.SetPushed(false);
	m_btnZmtm6.SetPushed(false);

}
void CLiuHeCai::OnBnClickedBtnZm3t()
{
	m_gameKind = LiuHeCai_Zm3t;
	SendToServer(3);

	m_btnZmtm1.SetPushed(false);
	m_btnZmtm2.SetPushed(false);
	m_btnZmtm3.SetPushed(true);
	m_btnZmtm4.SetPushed(false);
	m_btnZmtm5.SetPushed(false);
	m_btnZmtm6.SetPushed(false);

}
void CLiuHeCai::OnBnClickedBtnZm4t()
{
	m_gameKind = LiuHeCai_Zm4t;
	SendToServer(3);

	m_btnZmtm1.SetPushed(false);
	m_btnZmtm2.SetPushed(false);
	m_btnZmtm3.SetPushed(false);
	m_btnZmtm4.SetPushed(true);
	m_btnZmtm5.SetPushed(false);
	m_btnZmtm6.SetPushed(false);

}
void CLiuHeCai::OnBnClickedBtnZm5t()
{
	m_gameKind = LiuHeCai_Zm5t;
	SendToServer(3);

	m_btnZmtm1.SetPushed(false);
	m_btnZmtm2.SetPushed(false);
	m_btnZmtm3.SetPushed(false);
	m_btnZmtm4.SetPushed(false);
	m_btnZmtm5.SetPushed(true);
	m_btnZmtm6.SetPushed(false);

}
void CLiuHeCai::OnBnClickedBtnZm6t()
{
	m_gameKind = LiuHeCai_Zm6t;
	SendToServer(3);

	m_btnZmtm1.SetPushed(false);
	m_btnZmtm2.SetPushed(false);
	m_btnZmtm3.SetPushed(false);
	m_btnZmtm4.SetPushed(false);
	m_btnZmtm5.SetPushed(false);
	m_btnZmtm6.SetPushed(true);

}

void CLiuHeCai::OnBnClickedJettonOK()
{
	int nZhushu = 0;
	for (int i = 1;i < AREA_TMDM_MAX;i++)
	{
		if(mapJetton[i] < 0.0000001)
			continue;
		nZhushu ++;
	}
	if(nZhushu <= 0)
	{
		MyMessageBox(L"��ѡ��Ͷע����");
		return;
	}

	CString strQiShu;
	strQiShu.Format(L"%s",m_kjXinxi[0].qihao);
	if (strQiShu.IsEmpty())
	{
		MyMessageBox(_T("���ڻ�ȡ���ݡ��� "));
		return ;
	}

	CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
	ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
	TouzhuCQSSC.dwUserID = theAccount.user_id;
	TouzhuCQSSC.nBeitou = 1;
	TouzhuCQSSC.nGameKind = m_gameKind;
	TouzhuCQSSC.nGameType = CZ_LIUHECAI;
	TouzhuCQSSC.nMoshi = theAccount.Scoretype;
	TouzhuCQSSC.bZhuihao = 0;
	TouzhuCQSSC.nSign = m_nTzhSign;
	TouzhuCQSSC.nEnd = 0;
	TouzhuCQSSC.nIndex = 0;
	TouzhuCQSSC.nZhushu = 1;

	int nAreaCount = 0;
	CString strHaoma;
	if(m_gameKind == LiuHeCai_Tmdm||m_gameKind == LiuHeCai_Pmdm)
	{
		nAreaCount = AREA_TMDM_MAX;
	}
	else if(m_gameKind == LiuHeCai_Tmds)
	{
		nAreaCount = AREA_TMDS_MAX;
	}
// 	else if(m_gameKind == LiuHeCai_Tmdx)
// 	{
// 		nAreaCount = AREA_TMDX_MAX;
// 	}
	else if(m_gameKind == LiuHeCai_Tmsx)
	{
		nAreaCount = AREA_TMSX_MAX;
	}
	else if(m_gameKind == LiuHeCai_Tmbs)
	{
		nAreaCount = AREA_TMBS_MAX;
	}

	// 	CString strQihao;
	// 	strQihao.Format(L"%s",m_kjXinxi[0].qihao);

	int nQihao = _ttoi(strQiShu.GetBuffer());
	nQihao+=1;

	strQiShu.Format(L"%d",nQihao);
	string strQi;
	ChangeStringToA(strQiShu, strQi);

	memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());
	m_bShowFail = true;
	for(int i = 1;i<nAreaCount;i++)
	{
		if(mapJetton[i]<0.000001)
			continue;
		CString str;
		if(m_gameKind == LiuHeCai_Tmsx)
		{
			BYTE cbArea = 1;
			if(i == 1)
			{
				cbArea = 8;
			}
			else if(i == 2)
			{
				cbArea = 7;
			}
			else if(i == 3)
			{
				cbArea = 6;
			}
			else if(i == 4)
			{
				cbArea = 5;
			}
			else if(i == 5)
			{
				cbArea = 4;
			}
			else if(i == 6)
			{
				cbArea = 3;
			}
			else if(i == 7)
			{
				cbArea = 2;
			}
			else if(i == 8)
			{
				cbArea = 1;
			}
			else if(i == 9)
			{
				cbArea = 12;
			}
			else if(i == 10)
			{
				cbArea = 11;
			}
			else if(i == 11)
			{
				cbArea = 10;
			}
			else if(i == 12)
			{
				cbArea = 9;
			}
			if(m_nMoshi == MoShi_Yuan)
				str.Format(L"%d,%.2lf|",cbArea,mapJetton[i]);
			else if (m_nMoshi == MoShi_Jiao)
			{
				str.Format(L"%d,%.2lf|",cbArea,mapJetton[i]/10);
			}
			else if(m_nMoshi == MoShi_Fen)
			{
				str.Format(L"%d,%.2lf|",cbArea,mapJetton[i]/100);
			}

		}
		else
		{
			if(m_nMoshi == MoShi_Yuan)
				str.Format(L"%d,%.2lf|",i,mapJetton[i]);
			else if (m_nMoshi == MoShi_Jiao)
			{
				str.Format(L"%d,%.2lf|",i,mapJetton[i]/10);
			}
			else if(m_nMoshi == MoShi_Fen)
			{
				str.Format(L"%d,%.2lf|",i,mapJetton[i]/100);
			}

		}
		strHaoma=str;

		string	strAllHaoma;
		ChangeStringToA(strHaoma, strAllHaoma);
		byte pDestBuffer[102400];
		int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));
		TouzhuCQSSC.nHaoMaLen = nLen;

		memset(TouzhuCQSSC.strHaoma, 0, sizeof(TouzhuCQSSC.strHaoma));
		int nSize = sizeof(TouzhuCQSSC.strHaoma);

		memcpy((byte*)TouzhuCQSSC.strHaoma, pDestBuffer, nSize);

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_DAN,&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
		m_nTzhSign++;
	}


	strQiShu.ReleaseBuffer();
	ResetJetton();
	return;
}
bool	CLiuHeCai::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}

void CLiuHeCai::OnBnClickedBtnClsList()
{
	m_singlezhushu = 0;
	m_fSingleJine = 0.0f;
	m_zongZhuShu = 0;
	m_zongJine = 0.0f;
	//ResetAllEdit();
	ResetJetton();
	m_list3D.deleteAllItemEx();
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);

}

void CLiuHeCai::OnBnClickedFandian()
{
	int index = (CZ_LIUHECAI << 16) | (unsigned short)m_gameKind;

	CDlgFandian	dlg;

	dlg.SetBonus(mapBonus[m_gameKind], theBonus[index], mapPercent[m_gameKind], theAccount.fandian*100.0, mapDanweiFandian[m_gameKind]);
	if(IDOK == dlg.DoModal())
	{
		dlg.GetBonus(m_fBonus,  m_fPercent);

		CMD_GP_SetUserBonus SetUserBonus;
		ZeroMemory(&SetUserBonus,sizeof(SetUserBonus));

		SetUserBonus.n_t_userid = theAccount.user_id;
		SetUserBonus.n_t_type_id = CZ_LIUHECAI;
		SetUserBonus.n_t_kind_id = m_gameKind;
		SetUserBonus.f_t_bonus = m_fBonus;
		SetUserBonus.f_bonuspercent = m_fPercent;

		//��������
		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_USER_BONUS,&SetUserBonus,sizeof(SetUserBonus));

	}

	return;
}

BOOL CLiuHeCai::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}



//�����Ϣ
VOID CLiuHeCai::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);


}


//�����Ϣ
VOID CLiuHeCai::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);


	return;
}
// �滭����
void CLiuHeCai::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%.2lf"),lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}
// �滭����
void CLiuHeCai::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, int lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%d"),lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CLiuHeCai::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNumT[128] = {0};
	_sntprintf(szOutNumT,CountArray(szOutNumT),TEXT("%s"),szOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNumT, nXPos, nYPos, uFormat);
}


// �滭����
void CLiuHeCai::DrawNumber(CDC * pDC , CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_LEFT*/)
{
	// ������Դ
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}

//���ڿ�������
void CLiuHeCai::DrawLastKjHaoma(CDC* pDC, Graphics& graphics)
{
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&m_KjInfoFont1);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(255, 255, 255));

	pDC->DrawText(_T("������ϲ�"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	CRect rcDi;
	rcDi.left = rcCzh.right+10;
	rcDi.right = rcDi.left+20;
	rcDi.top = rcCzh.top;
	rcDi.bottom = rcCzh.bottom;
	pDC->DrawText(_T("��"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(253, 191, 6));
	pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(RGB(255, 255, 255));

	rcDi.left = rcQiHao.right-39;
	rcDi.right = rcDi.left+80;
	rcDi.top = rcCzh.top;
	rcDi.bottom = rcCzh.bottom;
	pDC->DrawText(_T("��������"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	CString strYg;
	CString strYg2;
	CRect rcYuGao1;
	CRect rcYuGao2;
	CTime t;
	theApp.GetTime(t);
	CTimeSpan timespan = m_NextTime-t;
	long lTotoalSeconds = timespan.GetTotalSeconds();

	if (lTotoalSeconds<2)
	{
		SetTimer(timer_get_qihao,1000,NULL);
	}
	int day = lTotoalSeconds/(24*3600);
	int nTemp = lTotoalSeconds%(24*3600);
	int hour = nTemp/3600;
	nTemp = nTemp%3600;
	int minute = nTemp/60;
	nTemp = nTemp%60;
	int second = nTemp;

	if(m_cbIfTs == 1)
		strYg.Format(_T("��ͣ����"));
	else
		strYg.Format(_T("%s���µ�ʱ�� "), strNextQihao);
	strYg2.Format(_T("%d��%02d:%02d:%02d"),day,hour,minute,second);  //������ʾ

	rcYuGao2.left = rcDi.left+95;
	rcYuGao2.top = rcDi.top;
	rcYuGao2.bottom = rcDi.bottom+16;
	rcYuGao2.right = rcDi.right+200;
	pDC->DrawText(strYg2, &rcYuGao2, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	rcYuGao1.left = rcDi.left+95;
	rcYuGao1.top = rcDi.top;
	rcYuGao1.bottom = rcDi.bottom-10;
	rcYuGao1.right = rcDi.right+550;
	pDC->DrawText(strYg, &rcYuGao1, DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(&pOldFont);
	pDC->SetBkMode(oldBkMode);

	CString str = m_kjXinxi[0].haoma;

	CString temp;
	temp.Format(L"%s",str.Left(2));

	if(_ttoi(temp) > 49)
	{
		m_bKaiJiangzhong=true;
		KillTimer(timer_id_kaijiangzhong);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);

		return;
	}
	int j = 0;
	int index = 0;
	CString strTemp;
	for (int i=0; i<str.GetLength(); i++) 
	{
		if (iswdigit(str.GetAt(i))) 
		{
			strTemp+=str.GetAt(i);
			int num = 0;//_ttoi(CString(str.GetAt(i)));

			if(strTemp.GetLength()>=2)
			{
				num = _ttoi(strTemp);
			}
			if (num >=1 && num < 50) 
			{
				if(m_bKaiJiangzhong )
				{
					KillTimer(timer_id_kaijiangzhong);
					SetTimer(timer_id_kaijiangzhong, 100, NULL);
				}
				else
				{
					int nNumberHeight=60;
					int nShengXiaoHeight=m_bigNumHeight - nNumberHeight;
					if (index+1==8)
					{
						DrawBigNum(graphics,m_kjNumBig, Rect(kj_big_haoma_x + (index) * (m_bigNumWidth-4), kj_big_haoma_y-2, m_bigNumWidth, nNumberHeight), num-1);
						DrawBigShengXiao(graphics, m_kjNumBig, Rect(kj_big_haoma_x + (index) * (m_bigNumWidth+1), kj_big_haoma_y-2+60, m_bigNumWidth, nShengXiaoHeight), num-1);
					}
					else
					{
						DrawBigNum(		 graphics, m_kjNumBig, Rect(kj_big_haoma_x + (index) * (m_bigNumWidth+1), kj_big_haoma_y-2, m_bigNumWidth, nNumberHeight), num-1);
						DrawBigShengXiao(graphics, m_kjNumBig, Rect(kj_big_haoma_x + (index) * (m_bigNumWidth+1), kj_big_haoma_y-2+60, m_bigNumWidth, nShengXiaoHeight), num-1);
					}
					if(index+1==6)
					{
						index ++;
						DrawBigNum(graphics, m_kjNumBig, Rect(kj_big_haoma_x-20 + (index) * (m_bigNumWidth+1), kj_big_haoma_y-2, m_bigNumWidth, nNumberHeight), 49);
					}
					

				}
			}
			j++;
			if(strTemp.GetLength()>=2)
			{
				//			num = _ttoi(strTemp);
				index ++;
				strTemp.Empty();
			}

		}
	}
	return;
}

//��������
void CLiuHeCai::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	//CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(250 ,250 ,250));

	for (int cnt=0; cnt<4; cnt++) 
	{
		CString str = m_kjXinxi[cnt].qihao;
		if(str.IsEmpty())
		{
			continue;
		}

		CRect rcText(kj_small_haoma_x - 80, 
			kj_small_haoma_y + cnt * (m_SmallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 80, 
			kj_small_haoma_y + cnt * (m_SmallNumHeight + kj_small_haoma_row_span) + m_SmallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[cnt].haoma;
		CString temp;
		temp = str.Left(2);

		if(_ttoi(temp) > 49)
		{
			//	index ++;
			continue;
		}

		int index = 0;
		int j = 0;
		CString strTemp;
		for (int i=0; i<str.GetLength(); i++) 
		{
			if (iswdigit(str.GetAt(i))) 
			{
				strTemp+=str.GetAt(i);
				int num = _ttoi(CString(str.GetAt(i)));
				if(strTemp.GetLength()>=2)
				{
					num = _ttoi(strTemp);
					//	strTemp.Empty();
				}
				if (num >0 && num < 50&& strTemp.GetLength()>=2) 
				{
					Rect rc(kj_small_haoma_x -5 + index * (m_SmallNumWidth + kj_small_haoma_col_span), 
						kj_small_haoma_y + cnt * (m_SmallNumHeight + kj_small_haoma_row_span), 
						m_SmallNumWidth, m_SmallNumHeight);
					DrawSmallNum(graphics, m_kjNumSmall, rc, num-1);
					if(index+1==6)
					{
						index ++;
						DrawSmallNum(graphics, m_kjNumSmall, Rect(kj_small_haoma_x -5 + (index) * (m_SmallNumWidth+ kj_small_haoma_col_span), kj_small_haoma_y + cnt * (m_SmallNumHeight + kj_small_haoma_row_span), m_SmallNumWidth, m_SmallNumHeight), 49);
					}

				}

				j++;
				if(strTemp.GetLength()>=2)
				{
					//			num = _ttoi(strTemp);
					index ++;
					strTemp.Empty();
				}
			}
		}
	}

	pDC->DrawText(strText, &rcZongJinE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(oldTextClr);
	//pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}
LRESULT CLiuHeCai::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	//OnBnClickedBtnDelSel();
	nSubItem--;

	int zhushu = _ttoi(m_list3D.GetItemText(nItem, 2));
	m_zongZhuShu -= zhushu;
	m_list3D.deleteItemEx( nItem );
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
	else if(theAccount.Scoretype == MoShi_Li)
		danzhujine = 0.002;
	int nItemCount = m_list3D.GetItemCount();
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_list3D.GetItemText(i,3);
		CString strZhuSHu = m_list3D.GetItemText(i,2);
		int beishu = _ttoi(strBeiSHu);
		int zhushu = _ttoi(strZhuSHu);


		m_zongJine += zhushu*beishu*danzhujine;
	}
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);

	//	m_listChqSSC.DeleteItem(nItem);

// 	if(vecAllNums.size() >0)
// 	{
// 		int nSize = vecAllNums.size();
// 		nSize -=1;
// 
// 		vector<wstring>::iterator   iter = vecAllNums.begin()+nSize-nItem;
// 
// 		CString strLog;
// 		strLog.Format(L"\n haoma:%s,item:%d",vecAllNums[nItem].c_str(),nItem);
// 		OutputDebugString(strLog);
// 		vecAllNums.erase(iter);
// 	}



	return 0;
}

int CLiuHeCai::GetBoSe(int nNum)
{
	int nIndex = 0;
	int nHong[17] = {1,2,7,8,12,13,18,19,23,24,29,30,34,35,40,45,46};
	int nLan[16] = {3,4,9,10,14,15,20,25,26,31,36,37,41,42,47,48};
	int nLv[16] = {5,6,11,16,17,21,22,27,28,32,33,38,39,43,44,49};

	for(int i = 0;i < 17;i++)
	{
		if(nNum == nHong[i])
		{
			nIndex = 0;
			return nIndex;
		}
	}

	for(int i = 0;i < 16;i++)
	{
		if(nNum == nLan[i])
		{
			nIndex = 1;
			return nIndex;
		}
	}

	for(int i = 0;i < 16;i++)
	{
		if(nNum == nLv[i])
		{
			nIndex = 2;
			return nIndex;
		}
	}
	return 0;
}

//�������˷���ǰN����������
bool CLiuHeCai::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex)
{
	_tcscpy_s(m_kjXinxi[nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);

	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		//��һ�ν���
		//if(m_lastExpect[0] == 0)
			_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);
		m_bKaiJiangzhong=false;											//��������

		SendToServer(4);
		KillTimer(timer_id_kaijiangzhong);
		if(strNextQihao.IsEmpty())
		{
			int nQiHao = _wtoi(m_lastExpect);
			strNextQihao.Format(TEXT("%ld"), nQiHao+1);
		}
	}
	if(nIndex+1 < 4)
		return true;

	m_kjXinxiCont = 4;
	if(!IsWindowVisible())
		return false;

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
//��ȡ�¼�
bool CLiuHeCai::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_SET_USER_BONUS_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_SetUserBonusRet));
				if(wDataSize < sizeof(CMD_GR_SetUserBonusRet)) return false;

				CMD_GR_SetUserBonusRet* pSetUserBonus = (CMD_GR_SetUserBonusRet*)pData;
				int nResult = pSetUserBonus->n_t_result;

				if(nResult==0)
				{
					SetLockText(m_fBonus, m_fPercent);
					MyMessageBox(L"���������޸ĳɹ���", L"��ܰ��ʾ");
					SendToServer(6);

				}
				else if(nResult == 1)
				{
					MyMessageBox(L"���޸ĵķ��㲻���ϣ�", L"��ʾ");
				}
				else if(nResult == 2)
				{
					MyMessageBox(L"���淨��֧���޸ķ��㣡", L"��ʾ");
				}
				else
				{
					MyMessageBox(L"�����޸�ʧ�ܣ�",L"��ʾ");
				}
			//	SendToServer(4);
				return true;
			}
		case SUB_GP_TOUZHU_CQSSC_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_TouzhuRes));
				if(wDataSize < sizeof(CMD_GR_TouzhuRes)) return false;

				CMD_GR_TouzhuRes* pTouzhuRes = (CMD_GR_TouzhuRes*)pData;

				LONG lResult = pTouzhuRes->lResult;

				m_DlgStatus.HideStatusWindow();
				if (lResult == 0)
				{
					if((pTouzhuRes->nSign != m_nTzhSign1))
					{
						m_nTzhSign1 = pTouzhuRes->nSign;
						ResetJetton();
						MyMessageBox(_T("�ɹ�Ͷע��"), _T("��ף������ Ͷע�ɹ���"));
						SendToServer(7);
					}
				}
				else if(lResult == 1)
				{
					MyMessageBox(_T("ע�����󣬻���Ͷע���ݲ���ȷ��"));
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("��ǰ���ѷⵥ��ֹͣ���ۣ�"));
				}
				else if(lResult == 3)
				{
					MyMessageBox(_T("���ں��Ѿ����ڣ�Ͷעʧ�ܣ�"));
				}
				else if(lResult == 4)
				{
					MyMessageBox(_T("�����ںų��ִ���Ͷעʧ�ܣ�"));
				}
				else if(lResult == 5)
				{
					MyMessageBox(_T("�޷���ȡ�淨��Ӧ�Ľ���Ͷעʧ�ܣ�"));
				}
				else if(lResult == 6)
				{
					MyMessageBox(_T("���������ж���û�гɹ��ύ��"));
				}
				else if(lResult == 7)
				{
					MyMessageBox(_T("���������ж���û�гɹ��ύ��"));
				}
				else if(lResult == 8)
				{
					MyMessageBox(_T("Ͷע���̷�����ʾ������"));
				}
				else if(lResult == 9)
				{
					MyMessageBox(_T("���������ж���û�гɹ��ύ��"));
				}
				else if(lResult == 10)
				{
					MyMessageBox(_T("�Բ��������˻�û��ͶעȨ�ޣ�"));
				}
				else if(lResult == 11)
				{
					MyMessageBox(_T("ע������ȷ��Ͷעʧ�ܣ�"));
				}
				else if(lResult == 12)
				{
					MyMessageBox(_T("�Բ��𣬸��淨ά���У���ѡ�������淨����Ͷע��"));
				}
				else if(lResult == 13)
				{
					MyMessageBox(_T("�ò�����ͣ���ۣ�"));
				}
				else if(lResult>=20 && lResult < 30)
				{
					MyMessageBox(_T("���˻�û��ͶעȨ�ޣ�"));

				}
				else if(lResult == 111)
				{
					CInformation information;
					int nResult = information.ShowMessageBox(_T("Ͷעδ�ɹ����������������ù��ڼ򵥣������ø��ӵ�½�����ȡ�����룡"),MB_ICONWARNING,30);
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
					strLog.Format(L"��Ǹ������%d���ϵ���Ҳ���Ͷע��",nFandian);
					MyMessageBox(strLog);
				}
				else
				{
					if(	m_bShowFail == true)
					{
						m_bShowFail = false;

						MyMessageBox(_T("�ж���δ�ɹ��ύ��"));
					}
				}



				return true;
			}
		case SUB_GP_GET_USER_FANDIAN_RET:
			{
				ASSERT((wDataSize % sizeof(CMD_GR_GetUserFandianRet))==0);
				if((wDataSize %sizeof(CMD_GR_GetUserFandianRet)) !=0)return false;

				KillTimer(TimerFandian);

				mapBonus.clear();
				mapDanweiFandian.clear();
				mapPercent.clear();
				mapFandian.clear();
				WORD wCount = wDataSize / sizeof(CMD_GR_GetUserFandianRet);
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetUserFandianRet* pGetUserFandian = ((CMD_GR_GetUserFandianRet*)pData+i);

					int nKindid = pGetUserFandian->n_t_kindid;
					nPercent = pGetUserFandian->f_t_bonusPercent;
					fDanweiFandian = pGetUserFandian->f_t_fandian;
					nBonus = pGetUserFandian->f_t_bonus;
					mapPercent.insert(make_pair(nKindid,nPercent));
					mapDanweiFandian.insert(make_pair(nKindid,fDanweiFandian));
					mapBonus.insert(make_pair(nKindid,nBonus));
					CString strText;
					strText.Format(L"����:%.2lf", nBonus);

					//strText+=_T("%");
					mapFandian.insert(make_pair(nKindid,strText));

				}
 				m_btnLock.SetWindowText(mapFandian[m_gameKind]);
 				m_btnLock.EnableWindow(true);
				return true;
			}
		case SUB_GR_GET_LHC_QIHAO_RET:
			{
				
				ASSERT(wDataSize >= sizeof(CMD_GetLhcQihaoRet));
				if(wDataSize < sizeof(CMD_GetLhcQihaoRet)) return false;

				CMD_GetLhcQihaoRet* pQihao = (CMD_GetLhcQihaoRet*)pData;

				CTime time(pQihao->n_t_fdshijian);

				CTime now = CTime::GetCurrentTime();

				if(time > now)
				{
					KillTimer(timer_get_qihao);
					strNextQihao.Format(L"%d",pQihao->n_t_qihao);
					m_NextTime = time;
					InvalidateRect(&rcRedraw);
				}

				return true;
			}
		case SUB_GP_QUERY_STATUS_LOTTERY_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_QueryStatusLotteryRet));
				if(wDataSize!=sizeof(CMD_GP_QueryStatusLotteryRet)) return false;

				CMD_GP_QueryStatusLotteryRet* pLogRet = (CMD_GP_QueryStatusLotteryRet*)pData;

				if(pLogRet->n_t_kindid != CZ_LIUHECAI)
					return true;

				m_cbIfTs = pLogRet->c_t_ifts;

				return true;
			}

		default:
			break;
		}
		return true;
	}
	return true;
}
void CLiuHeCai::SetLockText(float fBonus, float fBonusPercent)
{
	const float EPSINON = 0.00001;
	if((fBonus >= - EPSINON) && (fBonus <= EPSINON))
	{
	}
	else
	{
		CString strText;
		strText.Format(L"%.2lf/%.2lf", fBonus, fBonusPercent);
//		m_btnFandian.SetWindowText(strText + _T("%"));
	}
}

void CLiuHeCai::InitWanFaMoShi()
{
	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmdm] = _T("���뵥��");		

	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmds] =  _T("�����С��˫");

	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmdx] = _T("�����С");

	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmsx] = _T("������Ф");

	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmbs] = _T("���벨ɫ");
	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Pmdm] = _T("���뵥��");

	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Zm1t] = _T("����1��");		
	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Zm2t] = _T("����2��");		
	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Zm3t] = _T("����3��");		
	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Zm4t] = _T("����4��");		
	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Zm5t] = _T("����5��");		
	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Zm6t] = _T("����6��");		

}
int CLiuHeCai::GetGameKindByDesc(const CString& desc)
{
	if(desc == _T("���뵥��"))
	{
		return LiuHeCai_Tmdm;
	}
	else if(desc == _T("�����С��˫"))
	{
		return LiuHeCai_Tmds;
	}
	else if(desc == _T("������Ф"))
	{
		return LiuHeCai_Tmsx;
	}
	else if(desc == _T("���벨ɫ"))
	{
		return LiuHeCai_Tmbs;
	}
	else if(desc == _T("���뵥��"))
	{
		return LiuHeCai_Pmdm;
	}
	else if(desc == _T("����1��"))
	{
		return LiuHeCai_Zm1t;
	}
	else if(desc == _T("����2��"))
	{
		return LiuHeCai_Zm2t;
	}
	else if(desc == _T("����3��"))
	{
		return LiuHeCai_Zm3t;
	}
	else if(desc == _T("����4��"))
	{
		return LiuHeCai_Zm4t;
	}
	else if(desc == _T("����5��"))
	{
		return LiuHeCai_Zm5t;
	}
	else if(desc == _T("����6��"))
	{
		return LiuHeCai_Zm6t;
	}
	return LiuHeCai_Tmdm;
}

void CLiuHeCai::SendToServer(int nSendType)
{
	if(nSendType == 1)
	{
		if(m_cbIfTs == 1)
		{
			MyMessageBox(L"�˲�����ͣ���ۣ�");
			return;
		}

		int nItemCount = m_list3D.GetItemCount();
		CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
		ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
		TouzhuCQSSC.dwUserID = theAccount.user_id;
		TouzhuCQSSC.nGameType = CZ_LIUHECAI;
		TouzhuCQSSC.nMoshi = theAccount.Scoretype;

		m_nTzhSign++;
		for(int i = 0;i < nItemCount;i++)
		{
			CString strBeishu = m_list3D.GetItemText(i, 3);
			int nBeishu = _ttoi(strBeishu);
			TouzhuCQSSC.nBeitou = nBeishu;
			CString strWanfa = m_list3D.GetItemText(i, 1);
			int nKind = GetGameKindByDesc(strWanfa);
			TouzhuCQSSC.nGameKind = nKind;
			TouzhuCQSSC.nSign = m_nTzhSign;
			TouzhuCQSSC.nIndex = 0;
			TouzhuCQSSC.nEnd = 0;

			CString strZhu = m_list3D.GetItemText(i, 2);
			

			TouzhuCQSSC.nZhushu = _ttoi(strZhu);


			CString strDanzhu = m_list3D.GetItemText(i,0);
			if(strDanzhu.IsEmpty())
			{
				continue;
			}

			if(nKind == LiuHeCai_Tmds)
			{
				strDanzhu.Replace(L"��",L"9");
				strDanzhu.Replace(L"С",L"1");
				strDanzhu.Replace(L"��",L"3");
				strDanzhu.Replace(L"˫",L"2");
			}
			else if(nKind == LiuHeCai_Tmsx)
			{
				CString strWanfa[12] = {L"��",L"ţ",L"��",L"��",L"��",L"��",L"��",L"��",L"��",L"��",L"��",L"��"};
				CString strNumber[12]= {L"10",L"09",L"08",L"07",L"06",L"05",L"04",L"03",L"02",L"01",L"12",L"11"};

				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(0, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"ţ", strNumber[GetShengXiaoIndex(1, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(2, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(3, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(4, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(5, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(6, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(7, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(8, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(9, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(10, m_nShengXiaoWuCha)]);
				strDanzhu.Replace(L"��", strNumber[GetShengXiaoIndex(11, m_nShengXiaoWuCha)]);
			}
			else if(nKind == LiuHeCai_Tmbs)
			{
				strDanzhu.Replace(L"�첨",L"1");
				strDanzhu.Replace(L"����",L"2");
				strDanzhu.Replace(L"�̲�",L"3");
			}
			string	strAllHaoma;
			ChangeStringToA(strDanzhu, strAllHaoma);
			byte pDestBuffer[102400];
			int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));
			memset(TouzhuCQSSC.strHaoma, 0, sizeof(TouzhuCQSSC.strHaoma));
			memcpy((byte*)TouzhuCQSSC.strHaoma, pDestBuffer, nLen);

			string strQi;
			ChangeStringToA(strNextQihao, strQi);

			memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());
			TouzhuCQSSC.nHaoMaLen = nLen;
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_DAN,&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
		}
		OnBnClickedBtnClsList();
	}
	if(nSendType == 4)
	{
// 		CTime NowTime;
// 		theApp.GetTime(NowTime);
// 		int nChaDay = 0;
// 		int day = NowTime.GetDayOfWeek();
// 		if(day<3||(day==3 && (NowTime.GetHour()<21 ||(NowTime.GetHour() == 21 && NowTime.GetMinute()<30 ))))
// 		{
// 			nChaDay = 3 - day;
// 		}
// 		else if(day<5||(day==5 && (NowTime.GetHour()<21 ||(NowTime.GetHour() == 21 && NowTime.GetMinute()<30 ) )))
// 		{
// 			nChaDay = 5 - day;
// 		}
// 		else
// 		{
			//��������
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_LHC_QIHAO);
			return;
	//	}
// 		long lQihao = _ttoi(m_lastExpect);
// 		strNextQihao.Format(L"%d",lQihao+1);
// 		CTime time(NowTime.GetYear(),NowTime.GetMonth(),NowTime.GetDay(),21,30,0);
// 
// 		time += CTimeSpan(nChaDay,0,0,0);
// 		m_NextTime = time;
// 
// 		InvalidateRect(&rcRedraw);
		return;

		
	}

	if(nSendType == 3)
	{
		m_btnLock.SetWindowText(mapFandian[m_gameKind]);
		ResetJetton();
		return;
	}

	if(nSendType == 6)
	{
		CMD_GP_GetUserFandian GetUserFandian;
		ZeroMemory(&GetUserFandian,sizeof(GetUserFandian));

		GetUserFandian.n_t_userid = theAccount.user_id;
		GetUserFandian.n_t_type_id = CZ_LIUHECAI;
		GetUserFandian.n_t_kind_id = m_gameKind;

		//��������
		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_FANDIAN,&GetUserFandian,sizeof(GetUserFandian));

		return;
	}

	if(nSendType == 7)
	{
		return;
	}
	if(nSendType == 11)
	{
		CMD_GP_QueryStatusLottery QueryStatusLottery;
		ZeroMemory(&QueryStatusLottery,sizeof(QueryStatusLottery));

		QueryStatusLottery.n_t_kindid = CZ_LIUHECAI;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_STATUS_LOTTERY,&QueryStatusLottery,sizeof(QueryStatusLottery));
	}

}
BOOL CLiuHeCai::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);
	if(nCommandID>=IDC_EDIT_TMDM_1&&nCommandID<=IDC_EDIT_TMBS_LVB)
	{
		int nZhushu = GetLhcZhushu();
	}
	return __super::OnCommand(wParam,lParam);
}
int CLiuHeCai::GetLhcZhushu()
{
	m_singlezhushu = 0;
	m_fSingleJine = 0.0f;
	for(int i =IDC_EDIT_TMDM_1 ;i<= IDC_EDIT_TMBS_LVB;i++)
	{
		CString strText ;
		GetDlgItemText(i,strText);
		if(strText.IsEmpty())
			continue;
		int nZhushu = _ttoi(strText);
		if(nZhushu>0)
		{
			m_fSingleJine += nZhushu;
			m_singlezhushu ++;
		}
	}
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=200;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
	return m_singlezhushu;
}
void CLiuHeCai::DrawZongZhushuJinE(CDC* pDC)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_zongFont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(88, 78, 79));
	strText.Format(_T("��ѡ����"));

	CRect rcZhu;
	rcZhu.CopyRect(rcZongZhuShu);
//	rcZhu.left += 188;
	rcZhu.right += 492;
	rcZhu.top-=10;
	rcZhu.bottom-=10;
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(160, 8, 8));
	strText.Format(_T("%d"), m_singlezhushu);
	rcZhu.left += 56;
	rcZhu.right += 56;
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(88, 78, 79));
	rcZhu.left += strText.GetLength()*9;
	rcZhu.right += strText.GetLength()*9;
	strText.Format(_T("ע"));
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	CRect rcRect;

	strText.Format(_T("��:"));
	rcZongJinE.top = rcZhu.top;
	rcZongJinE.bottom = rcZhu.bottom;
	pDC->DrawText(strText, &rcZongJinE, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	rcRect.CopyRect(rcZongJinE);
	rcRect.left += 17;
	rcRect.right += 17;
	rcRect.top = rcZhu.top;
	rcRect.bottom = rcZhu.bottom;
	pDC->SetTextColor(RGB(160, 8, 8));

	strText.Format(_T("%.3lf"), m_fSingleJine);
	pDC->DrawText(strText, &rcRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(88, 78, 79));

	rcRect.left += strText.GetLength()*8;
	rcRect.right += strText.GetLength()*8;
	strText.Format(_T("Ԫ"));
	pDC->DrawText(strText, &rcRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strText.Format(_T("ע��:"),m_zongZhuShu);
	pDC->DrawText(strText, &rcAllZongzhushu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	rcRect.CopyRect(rcAllZongzhushu);
	rcRect.left += 32;
	rcRect.right += 32;
	pDC->SetTextColor(RGB(160, 8, 8));

	strText.Format(_T("%d"),m_zongZhuShu);
	pDC->DrawText(strText, &rcRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(88, 78, 79));

	rcZhu.CopyRect(rcAllZongzhushu);
	rcZhu.top += 20;
	rcZhu.bottom += 20;
	strText.Format(_T("�ϼ�:"));
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(160, 8, 8));
	rcZhu.left += 32;
	rcZhu.right += 32;
	strText.Format(_T("%.3lf"),m_zongJine);
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}
