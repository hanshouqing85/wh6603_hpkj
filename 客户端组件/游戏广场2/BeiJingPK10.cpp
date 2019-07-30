#include "stdafx.h"
#include "GamePlaza.h"
#include "BeiJingPK10.h"
#include "BmpUtil.h"
#include "MessageDlg.h"
#include "DlgAddtional.h"
#include "DlgFandian.h"
#include "GDIpng/MemDC.h"
#include "PlatformFrame.h"
using		namespace		std;
#define      MAXN			100
CString   CBeiJingPK10::m_strHaoma = _T("");
#include <map>
extern map <int,CString> mapFandian;
extern map <int,float> mapPercent;
extern map <int,float> mapDanweiFandian;
extern map <int,float> mapBonus;

extern vector<wstring> vecAllNums;

 extern vector<wstring>	vecData1, vecData2, vecData3;
 extern	int g_arr[MAXN];
 extern	int g_nPos[512000][5];
 extern	int g_Counts;


 //�õ���Ϻ��
 extern	int GetComboArray(int DigitnCount, int nPosCount, int* szInput, int szOutput[10][5]);
 extern	int InsertDouhaoAfter(int nDouhaoIndexAfter, wstring&  strLine);
 //�õ�ĳһ�д����ŵ��ַ���
 extern	wstring	GetXiaZhuString(int nLines, int nCols, int nIndex, int nValue [10][5], int nInsertCol=100);

 //��ѡ������
 extern	int nRenXuan2[6];
 extern	int nRenXuan3[6];
 extern	int nRenXuan4[6];
 //�淨��������
 static const int wanfa_x = 9;
 static const int wanfa_y = 109;
 static const int wanfa_col_width = 54;
 static const int wanfa_row_height = 32;

 //�淨�ӷ�������
 static const int wanfa_sel_x = 20;
 static const int wanfa_sel_y = 145;
 static const int wanfa_sel_width = 71;
 static const int wanfa_sel_height = 22;

 //��������
 static const int haoma_btn_x = 80;
 static const int haoma_btn_y = 226;
 static const int haoma_btn_col_wdith = 30;
 static const int haoma_btn_row_height = 32;

 //������ʾ����
 static const int haoma_tip_x = 20;
 static const int haoma_tip_y = haoma_btn_y;

 //��ʽѡ�ű༭��
 static const int edit_haomao_danshi_x = 20;
 static const int edit_haomao_danshi_y = 210;
 static const int edit_haomao_danshi_width = 752;
 static const int edit_haomao_danshi_height = 195;

 //��Ӻ��밴ť
 static const int btn_chq_add_x = 588;
 static const int btn_chq_add_y = 415;
 //
 static const int btn_lock_x = 378;
 static const int btn_lock_y = 417;

 //ѡ����ʾ��������
 //ɾ��ѡ�а�ť
 static const int btn_del_sel_x = 470;
 static const int btn_del_sel_y = 543;
 //����б�ť
 static const int btn_cls_list_x = 654;
 static const int btn_cls_list_y = 415;
 //�����б�
 static const int list_haomao_x = 43;
 static const int list_haomao_y = 469;
 static const int list_haomao_width = 577;
 static const int list_haomao_height = 58;
 //��Ͷ�༭��
 static const int edit_beitou_x = 182+9+10;
 static const int edit_beitou_y = 426;
 static const int edit_beitou_width = 35;
 static const int edit_beitou_height = 27;
 //ȷ��Ͷע��ť
 static const int btn_touzhu_x = 638;
 static const int btn_touzhu_y = 490;
 //һ��Ͷע��ť
 static const int btn_touzhu_onekey_x = 496;
 static const int btn_touzhu_onekey_y = 417;
 //׷�Ű�ť
 static const int btn_zhuihao_x = 721;
 static const int btn_zhuihao_y = 415;
 //�����¼
 //static const int btn_more_x = 712+30;
 //static const int btn_more_y = 92-90;

 static const int btn_more_x = 712+50;
 static const int btn_more_y = 92-90;

 //�󿪽���
 static const int kj_big_haoma_x = 207-21;
 static const int kj_big_haoma_y = 54+14;


 static const int czh_rc_left = 243-35;
 static const int czh_rc_top = 4-5;
 static const int czh_rc_width = 85;
 static const int czh_rc_height = 30;
 static CRect rcCzh(czh_rc_left, czh_rc_top, czh_rc_left + czh_rc_width, czh_rc_top + czh_rc_height);

 static const int qihao_rc_left = 335-45+20;
 static const int qihao_rc_top = 4-5;
 static const int qihao_rc_width = 125;
 static const int qihao_rc_height = 30;
 static CRect rcQiHao(qihao_rc_left, qihao_rc_top, qihao_rc_left + qihao_rc_width, qihao_rc_top + qihao_rc_height);

 static const int yugao_rc_left = 3;
 static const int yugao_rc_top = 4;
 static const int yugao_rc_width = 196;
 static const int yugao_rc_height = 30;
 static CRect rcYuGao(yugao_rc_left, yugao_rc_top, yugao_rc_left + yugao_rc_width, yugao_rc_top + yugao_rc_height);

 //С������
 static const int kj_small_haoma_x = 590-15;
 static const int kj_small_haoma_y = 28;
 static const int kj_small_haoma_col_span = 0;
 static const int kj_small_haoma_row_span = 1;

 //��ע��
 static const int zongzhushu_x = 75;
 static const int zongzhushu_y = 415;
 static const int zongzhushu_width = 150;
 static const int zongzhushu_height = 25;
 static CRect rcZongZhuShu(zongzhushu_x, zongzhushu_y, zongzhushu_x+zongzhushu_width, zongzhushu_y+zongzhushu_height);

 //�ܽ��
 static const int zongjine_x = 261;
 static const int zongjine_y = 433;
 static const int zongjine_width = 150;
 static const int zongjine_height = 25;
 static CRect rcZongJinE(zongjine_x, zongjine_y, zongjine_x+zongjine_width, zongjine_y+zongjine_height);
 //��ע��
 static const int allzongzhushu_x = 638+10;
 static const int allzongzhushu_y = 444;
 static const int allzongzhushu_width = 150;
 static const int allzongzhushu_height = 25;
 static CRect rcAllZongzhushu(allzongzhushu_x, allzongzhushu_y, allzongzhushu_x+allzongzhushu_width, allzongzhushu_y+allzongzhushu_height);

 //�������
 static const int mouseinput_x = 597;
 static const int mouseinput_y = 175;
 //��������
 static const int keyboardinput_x = 670;
 static const int keyboardinput_y = 175;

 static const int redraw_rc_left = 0;
 static const int redraw_rc_top = 0;
 static const int redraw_rc_width = 791;
 static const int redraw_rc_height = 103;
 static CRect rcRedraw(redraw_rc_left, redraw_rc_top, redraw_rc_left + redraw_rc_width, redraw_rc_top + redraw_rc_height);

 static const int btn_yuan_x = 10;
 static const int btn_yuan_y = 415;
 static const int btn_jiao_x = 37+10;
 static const int btn_jiao_y = 415;
 static const int btn_fen_x = 64+20;
 static const int btn_fen_y = 415;
 static const int btn_li_x = 91+30;
 static const int btn_li_y = 415;
static const int timer_id_get_luck_num = 1;
static const int timer_id_kj_yugao = 2;

static const int timer_id_kaijiangzhong=3;
//��ʱ
static const int TimerJiShi = 4;
static const int TimerFandian = 5;
static const int TimerLotteryStatus = 6;

IMPLEMENT_DYNAMIC(CBeiJingPK10, CDialog)

CBeiJingPK10::CBeiJingPK10(CWnd* pParent /*=NULL*/)
	: CDialog(CBeiJingPK10::IDD, pParent)
	, m_bmpBk(NULL)
	, m_numImageList(NULL)
	, m_kjNumBig(NULL)
	, m_kjNumSmall(NULL)
	, m_bmpDxdsh(NULL)
	, m_bmpNumTip(NULL)
	, m_bigNumWidth(0)
	, m_bigNumHeight(0)
	, m_smallNumWidth(0)
	, m_smallNumHeight(0)
	, m_gameKind(WF_GuanJun)
	, m_zongZhuShu(0)
	//, m_zongJinE(0.0)
	, m_beishu(1)
	, m_kjXinxiCont(0)
{
	m_nTzhSign = 1;
	m_nTzhSign1 = 0;
	m_bCanSend=true;
	m_nFrameIndex=0;
	m_nFrameCount=0;
	for (int i=0; i<Kj_XinXi_Count; i++) 
	{
		memset(&m_kjXinxi[i], 0, sizeof(KjXinXi));
	}
	m_bZhuiZjtz = true;

	m_RenxuanKeyboardInput=false;
	_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, _T(""));

	//�������
	m_bGetUserFandian=false;
	m_Font.CreateFont(14,0,0,0,700,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
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
	nBonus = 0.0f;//theDBCmd.GetUserFandian(theAccount.user_id,  CZChongQingSSC, m_gameKind, nPercent, fDanweiFandian);  //��ʱ����  a'long

	m_nZhuihaoCount = 0;
	m_bZhuihao=false;
	//�������
	m_DlgStatus.SetStatusViewSink(this);
}

CBeiJingPK10::~CBeiJingPK10()
{
	if (m_bmpBk != NULL) 
	{
		SafeDelete(m_bmpBk);
	}

	if (m_numImageList != NULL) 
	{
		SafeDelete(m_numImageList);
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
		//SafeDelete(m_imgKaijiang);
	}

	if (m_bmpDxdsh != NULL) 
	{
		SafeDelete(m_bmpDxdsh);
	}

	if (m_bmpNumTip != NULL) 
	{
		SafeDelete(m_bmpNumTip);
	}
}

void CBeiJingPK10::FlushZongjine()
{
	InvalidateRect(&rcZongJinE);
}

void CBeiJingPK10::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TIP_WXZX,m_btWxzxTip);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIA,m_btnBeiTouJia);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIAN,m_btnBeiTouJian);
	DDX_Control(pDX, IDC_BTN_HSZHX, m_btnHouSanZhiXuan);
	DDX_Control(pDX, IDC_BTN_QSZHX, m_btnQianSanZhiXuan);
	DDX_Control(pDX, IDC_BTN_HSZX, m_btnHouSanZuXuan);
	DDX_Control(pDX, IDC_BTN_QSZX, m_btnQianSanZuXuan);
	DDX_Control(pDX, IDC_BTN_WXHE, m_btnWuXingHouEr);
	DDX_Control(pDX, IDC_BTN_WXQE, m_btnWuXingQianEr);
	DDX_Control(pDX, IDC_BTN_HEZX, m_btnHouErZuXuan);
	DDX_Control(pDX, IDC_BTN_QEZX, m_btnQianErZuXuan);
	DDX_Control(pDX, IDC_BTN_DXDSH, m_btnDaXiaoDanShuang);
	DDX_Control(pDX, IDC_BTN_QSDXDS, m_btnQSDaXiaoDanShuang);
	DDX_Control(pDX, IDC_BTN_BDW, m_btnBuDingWei);
	DDX_Control(pDX, IDC_BTN_DWD, m_btnDingWeiDan);

	DDX_Control(pDX, IDC_BTN_LOCK, m_btnLock);

	DDX_Control(pDX, IDC_RIO_HSZHX_FUSHI, m_rioHsZhxFuShi);
	DDX_Control(pDX, IDC_RIO_RENXUAN2,m_rioRenXuan2);
	DDX_Control(pDX, IDC_RIO_RENXUAN3,m_rioRenXuan3);
	DDX_Control(pDX, IDC_RIO_RENXUAN4,m_rioRenXuan4);
	DDX_Control(pDX, IDC_RIO_MOUSE_INPUT,m_rioMouseInput);
	DDX_Control(pDX, IDC_RIO_KEYBOARD_INPUT,m_rioKeyboardInput);
	DDX_Control(pDX, IDC_CHK_WAN,	m_rioWan);
	DDX_Control(pDX, IDC_CHK_QIAN,	m_rioQian);
	DDX_Control(pDX, IDC_CHK_BAI,	m_rioBai);
	DDX_Control(pDX, IDC_CHK_SHI,	m_rioShi);
	DDX_Control(pDX, IDC_CHKGE,		m_rioGe);

	DDX_Control(pDX, IDC_RIO_HSZHX_DANSHI, m_rioHsZhxDanShi);
	DDX_Control(pDX, IDC_RIO_QSZHX_FUSHI, m_rioQsZhxFuShi);
	DDX_Control(pDX, IDC_RIO_QSZHX_DANSHI, m_rioQsZhxDanShi);
	DDX_Control(pDX, IDC_RIO_HSZS_FUSHI, m_rioHsZsFuShi);
	DDX_Control(pDX, IDC_RIO_HSZS_FUSHI_DANSHI, m_rioHsZsFuShiDanshi);
	DDX_Control(pDX, IDC_RIO_HSZS_DANSHI, m_rioHsZsDanShi);
	DDX_Control(pDX, IDC_RIO_HSZL_FUSHI, m_rioHsZlFuShi);
	//DDX_Control(pDX, IDC_RIO_HSZL_DANSHI, m_rioHsZlDanShi);
	DDX_Control(pDX, IDC_RIO_QSZS_FUSHI, m_rioQsZsFuShi);
	DDX_Control(pDX, IDC_RIO_QSZS_DANSHI, m_rioQsZsDanShi);
	DDX_Control(pDX, IDC_RIO_QSZL_FUSHI, m_rioQsZlFuShi);
	//DDX_Control(pDX, IDC_RIO_QSZL_DANSHI, m_rioQsZlDanShi);
	DDX_Control(pDX, IDC_RIO_HEZHX_FUSHI, m_rioHeZhxFuShi);
	DDX_Control(pDX, IDC_RIO_HEZHX_FUSHI_DANSHI, m_rioHeZhxFuShiDan);
	DDX_Control(pDX, IDC_RIO_HEZHX_DANSHI, m_rioHeZhxDanShi);
	DDX_Control(pDX, IDC_RIO_QEZHX_FUSHI, m_rioQeZhxFuShi);
	DDX_Control(pDX, IDC_RIO_QEZHX_DANSHI, m_rioQeZhxDanShi);
	DDX_Control(pDX, IDC_RIO_HEZX_FUSHI, m_rioHeZxFuShi);
	DDX_Control(pDX, IDC_RIO_HEZX_FUSHI_DANSHI, m_rioHeZxFuShiDan);
	DDX_Control(pDX, IDC_RIO_HEZX_DANSHI, m_rioHeZxDanShi);
	DDX_Control(pDX, IDC_RIO_QEZX_FUSHI, m_rioQeZxFuShi);
	DDX_Control(pDX, IDC_RIO_QEZX_DANSHI, m_rioQeZxDanShi);
	DDX_Control(pDX, IDC_RIO_DXDSH, m_rioQeDxdsh);
	DDX_Control(pDX, IDC_RIO_DXDSH_DAN, m_rioQeDxdshDan);
	DDX_Control(pDX, IDC_RIO_QSBDW, m_rioQsbdw);
	DDX_Control(pDX, IDC_RIO_HSBDW, m_rioHsbdw);
	DDX_Control(pDX, IDC_RIO_DWD,	m_rioDwd);
	DDX_Control(pDX, IDC_BTN_WAN_0, m_btnWan_0);
	DDX_Control(pDX, IDC_BTN_WAN_1, m_btnWan_1);
	DDX_Control(pDX, IDC_BTN_WAN_2, m_btnWan_2);
	DDX_Control(pDX, IDC_BTN_WAN_3, m_btnWan_3);
	DDX_Control(pDX, IDC_BTN_WAN_4, m_btnWan_4);
	DDX_Control(pDX, IDC_BTN_WAN_5, m_btnWan_5);
	DDX_Control(pDX, IDC_BTN_WAN_6, m_btnWan_6);
	DDX_Control(pDX, IDC_BTN_WAN_7, m_btnWan_7);
	DDX_Control(pDX, IDC_BTN_WAN_8, m_btnWan_8);
	DDX_Control(pDX, IDC_BTN_WAN_9, m_btnWan_9);

	DDX_Control(pDX, IDC_BTN_QIAN_0, m_btnQian_0);
	DDX_Control(pDX, IDC_BTN_QIAN_1, m_btnQian_1);
	DDX_Control(pDX, IDC_BTN_QIAN_2, m_btnQian_2);
	DDX_Control(pDX, IDC_BTN_QIAN_3, m_btnQian_3);
	DDX_Control(pDX, IDC_BTN_QIAN_4, m_btnQian_4);
	DDX_Control(pDX, IDC_BTN_QIAN_5, m_btnQian_5);
	DDX_Control(pDX, IDC_BTN_QIAN_6, m_btnQian_6);
	DDX_Control(pDX, IDC_BTN_QIAN_7, m_btnQian_7);
	DDX_Control(pDX, IDC_BTN_QIAN_8, m_btnQian_8);
	DDX_Control(pDX, IDC_BTN_QIAN_9, m_btnQian_9);

	DDX_Control(pDX, IDC_BTN_BAI_0, m_btnBai_0);
	DDX_Control(pDX, IDC_BTN_BAI_1, m_btnBai_1);
	DDX_Control(pDX, IDC_BTN_BAI_2, m_btnBai_2);
	DDX_Control(pDX, IDC_BTN_BAI_3, m_btnBai_3);
	DDX_Control(pDX, IDC_BTN_BAI_4, m_btnBai_4);
	DDX_Control(pDX, IDC_BTN_BAI_5, m_btnBai_5);
	DDX_Control(pDX, IDC_BTN_BAI_6, m_btnBai_6);
	DDX_Control(pDX, IDC_BTN_BAI_7, m_btnBai_7);
	DDX_Control(pDX, IDC_BTN_BAI_8, m_btnBai_8);
	DDX_Control(pDX, IDC_BTN_BAI_9, m_btnBai_9);

	DDX_Control(pDX, IDC_BTN_SHI_0, m_btnShi_0);
	DDX_Control(pDX, IDC_BTN_SHI_1, m_btnShi_1);
	DDX_Control(pDX, IDC_BTN_SHI_2, m_btnShi_2);
	DDX_Control(pDX, IDC_BTN_SHI_3, m_btnShi_3);
	DDX_Control(pDX, IDC_BTN_SHI_4, m_btnShi_4);
	DDX_Control(pDX, IDC_BTN_SHI_5, m_btnShi_5);
	DDX_Control(pDX, IDC_BTN_SHI_6, m_btnShi_6);
	DDX_Control(pDX, IDC_BTN_SHI_7, m_btnShi_7);
	DDX_Control(pDX, IDC_BTN_SHI_8, m_btnShi_8);
	DDX_Control(pDX, IDC_BTN_SHI_9, m_btnShi_9);

	DDX_Control(pDX, IDC_BTN_GE_0, m_btnGe_0);
	DDX_Control(pDX, IDC_BTN_GE_1, m_btnGe_1);
	DDX_Control(pDX, IDC_BTN_GE_2, m_btnGe_2);
	DDX_Control(pDX, IDC_BTN_GE_3, m_btnGe_3);
	DDX_Control(pDX, IDC_BTN_GE_4, m_btnGe_4);
	DDX_Control(pDX, IDC_BTN_GE_5, m_btnGe_5);
	DDX_Control(pDX, IDC_BTN_GE_6, m_btnGe_6);
	DDX_Control(pDX, IDC_BTN_GE_7, m_btnGe_7);
	DDX_Control(pDX, IDC_BTN_GE_8, m_btnGe_8);
	DDX_Control(pDX, IDC_BTN_GE_9, m_btnGe_9);

	DDX_Control(pDX, IDC_BTN_LIU_0, m_btnLiu_0);
	DDX_Control(pDX, IDC_BTN_LIU_1, m_btnLiu_1);
	DDX_Control(pDX, IDC_BTN_LIU_2, m_btnLiu_2);
	DDX_Control(pDX, IDC_BTN_LIU_3, m_btnLiu_3);
	DDX_Control(pDX, IDC_BTN_LIU_4, m_btnLiu_4);
	DDX_Control(pDX, IDC_BTN_LIU_5, m_btnLiu_5);
	DDX_Control(pDX, IDC_BTN_LIU_6, m_btnLiu_6);
	DDX_Control(pDX, IDC_BTN_LIU_7, m_btnLiu_7);
	DDX_Control(pDX, IDC_BTN_LIU_8, m_btnLiu_8);
	DDX_Control(pDX, IDC_BTN_LIU_9, m_btnLiu_9);

	DDX_Control(pDX, IDC_BTN_QI_0, m_btnQi_0);
	DDX_Control(pDX, IDC_BTN_QI_1, m_btnQi_1);
	DDX_Control(pDX, IDC_BTN_QI_2, m_btnQi_2);
	DDX_Control(pDX, IDC_BTN_QI_3, m_btnQi_3);
	DDX_Control(pDX, IDC_BTN_QI_4, m_btnQi_4);
	DDX_Control(pDX, IDC_BTN_QI_5, m_btnQi_5);
	DDX_Control(pDX, IDC_BTN_QI_6, m_btnQi_6);
	DDX_Control(pDX, IDC_BTN_QI_7, m_btnQi_7);
	DDX_Control(pDX, IDC_BTN_QI_8, m_btnQi_8);
	DDX_Control(pDX, IDC_BTN_QI_9, m_btnQi_9);

	DDX_Control(pDX, IDC_BTN_BA_0, m_btnBa_0);
	DDX_Control(pDX, IDC_BTN_BA_1, m_btnBa_1);
	DDX_Control(pDX, IDC_BTN_BA_2, m_btnBa_2);
	DDX_Control(pDX, IDC_BTN_BA_3, m_btnBa_3);
	DDX_Control(pDX, IDC_BTN_BA_4, m_btnBa_4);
	DDX_Control(pDX, IDC_BTN_BA_5, m_btnBa_5);
	DDX_Control(pDX, IDC_BTN_BA_6, m_btnBa_6);
	DDX_Control(pDX, IDC_BTN_BA_7, m_btnBa_7);
	DDX_Control(pDX, IDC_BTN_BA_8, m_btnBa_8);
	DDX_Control(pDX, IDC_BTN_BA_9, m_btnBa_9);

	DDX_Control(pDX, IDC_BTN_JIU_0, m_btnJiu_0);
	DDX_Control(pDX, IDC_BTN_JIU_1, m_btnJiu_1);
	DDX_Control(pDX, IDC_BTN_JIU_2, m_btnJiu_2);
	DDX_Control(pDX, IDC_BTN_JIU_3, m_btnJiu_3);
	DDX_Control(pDX, IDC_BTN_JIU_4, m_btnJiu_4);
	DDX_Control(pDX, IDC_BTN_JIU_5, m_btnJiu_5);
	DDX_Control(pDX, IDC_BTN_JIU_6, m_btnJiu_6);
	DDX_Control(pDX, IDC_BTN_JIU_7, m_btnJiu_7);
	DDX_Control(pDX, IDC_BTN_JIU_8, m_btnJiu_8);
	DDX_Control(pDX, IDC_BTN_JIU_9, m_btnJiu_9);

	DDX_Control(pDX, IDC_BTN_SHI10_0, m_btnShi10_0);
	DDX_Control(pDX, IDC_BTN_SHI10_1, m_btnShi10_1);
	DDX_Control(pDX, IDC_BTN_SHI10_2, m_btnShi10_2);
	DDX_Control(pDX, IDC_BTN_SHI10_3, m_btnShi10_3);
	DDX_Control(pDX, IDC_BTN_SHI10_4, m_btnShi10_4);
	DDX_Control(pDX, IDC_BTN_SHI10_5, m_btnShi10_5);
	DDX_Control(pDX, IDC_BTN_SHI10_6, m_btnShi10_6);
	DDX_Control(pDX, IDC_BTN_SHI10_7, m_btnShi10_7);
	DDX_Control(pDX, IDC_BTN_SHI10_8, m_btnShi10_8);
	DDX_Control(pDX, IDC_BTN_SHI10_9, m_btnShi10_9);

	DDX_Text(pDX, IDC_EDIT_BEISHU, m_beishu);
	

	DDX_Control(pDX, IDC_EDIT_DANSHI_HAOMA, m_editDanShiHaoMa);
	DDX_Control(pDX, IDC_RICHEDIT_DANSHI, m_richDanshiHaoma);

	DDX_Control(pDX, IDC_STATIC_WAN, m_staticWan);
	DDX_Control(pDX, IDC_STATIC_QIAN, m_staticQian);
	DDX_Control(pDX, IDC_STATIC_BAI, m_staticBai);
	DDX_Control(pDX, IDC_STATIC_SHI, m_staticShi);
	DDX_Control(pDX, IDC_STATIC_GE, m_staticGe);
	DDX_Control(pDX, IDC_STATIC_LIU, m_staticLiu);
	DDX_Control(pDX, IDC_STATIC_QI, m_staticQi);
	DDX_Control(pDX, IDC_STATIC_BA, m_staticBa);
	DDX_Control(pDX, IDC_STATIC_JIU, m_staticJiu);
	DDX_Control(pDX, IDC_STATIC_SHI10, m_staticShi10);

	DDX_Control(pDX, IDC_LIST_CHQSSC, m_listChqSSC);
	DDX_Control(pDX, IDC_BTN_CHQ_ADD, m_btnChqAdd);
	DDX_Control(pDX, IDC_BTN_DEL_SEL, m_btnDelSel);
	DDX_Control(pDX, IDC_BTN_CLS_LIST, m_btnClsList);
	DDX_Control(pDX, IDC_BTN_TOUZHU, m_btnTouZhu);
	DDX_Control(pDX, IDC_BTN_TOUZHU_ONEKEY, m_btnTouZhuOnekey);
	DDX_Control(pDX, IDC_BTN_ZHUIHAO, m_btnZhuiHao);
	DDX_Control(pDX, IDC_EDIT_BEISHU, m_editBeiTou);
	//DDX_Control(pDX, IDC_STATIC_DNASHI, m_staticDanShi);


	DDX_Control(pDX, IDC_RIO_QSHUNHE, m_rioQsHunhe);
	DDX_Control(pDX, IDC_RIO_ZSHUNHE, m_rioZsHunhe);
	DDX_Control(pDX, IDC_RIO_HSHUNHE, m_rioHsHunhe);

	DDX_Control(pDX, IDC_STATIC_HUNHE_TIP, m_staticHunheTip);

	DDX_Control(pDX, IDC_BTN_DA_GUAN, m_btnDaGUAN);
	DDX_Control(pDX, IDC_BTN_XIAO_GUAN, m_btnXiaoGUAN);
	DDX_Control(pDX, IDC_BTN_DAN_GUAN, m_btnDanGUAN);
	DDX_Control(pDX, IDC_BTN_SHUANG_GUAN, m_btnShuangGUAN);
	DDX_Control(pDX, IDC_BTN_DA_YA, m_btnDaYA);
	DDX_Control(pDX, IDC_BTN_XIAO_YA, m_btnXiaoYA);
	DDX_Control(pDX, IDC_BTN_DAN_YA, m_btnDanYA);
	DDX_Control(pDX, IDC_BTN_SHUANG_YA, m_btnShuangYA);

	DDX_Control(pDX, IDC_BTN_DA_SAN, m_btnDaSAN);
	DDX_Control(pDX, IDC_BTN_XIAO_SAN, m_btnXiaoSAN);
	DDX_Control(pDX, IDC_BTN_DAN_SAN, m_btnDanSAN);
	DDX_Control(pDX, IDC_BTN_SHUANG_SAN, m_btnShuangSAN);
	DDX_Control(pDX, IDC_BTN_DA_SI, m_btnDaSI);
	DDX_Control(pDX, IDC_BTN_XIAO_SI, m_btnXiaoSI);
	DDX_Control(pDX, IDC_BTN_DAN_SI, m_btnDanSI);
	DDX_Control(pDX, IDC_BTN_SHUANG_SI, m_btnShuangSI);

	DDX_Control(pDX, IDC_BTN_DA_WU, m_btnDaWU);
	DDX_Control(pDX, IDC_BTN_XIAO_WU, m_btnXiaoWU);
	DDX_Control(pDX, IDC_BTN_DAN_WU, m_btnDanWU);
	DDX_Control(pDX, IDC_BTN_SHUANG_WU, m_btnShuangWU);
	DDX_Control(pDX, IDC_BTN_DA_LIU, m_btnDaLIU);
	DDX_Control(pDX, IDC_BTN_XIAO_LIU, m_btnXiaoLIU);
	DDX_Control(pDX, IDC_BTN_DAN_LIU, m_btnDanLIU);
	DDX_Control(pDX, IDC_BTN_SHUANG_LIU, m_btnShuangLIU);

	DDX_Control(pDX, IDC_BTN_DA_QI, m_btnDaQI);
	DDX_Control(pDX, IDC_BTN_XIAO_QI, m_btnXiaoQI);
	DDX_Control(pDX, IDC_BTN_DAN_QI, m_btnDanQI);
	DDX_Control(pDX, IDC_BTN_SHUANG_QI, m_btnShuangQI);
	DDX_Control(pDX, IDC_BTN_DA_BA, m_btnDaBA);
	DDX_Control(pDX, IDC_BTN_XIAO_BA, m_btnXiaoBA);
	DDX_Control(pDX, IDC_BTN_DAN_BA, m_btnDanBA);
	DDX_Control(pDX, IDC_BTN_SHUANG_BA, m_btnShuangBA);

	DDX_Control(pDX, IDC_BTN_DA_JIU, m_btnDaJIU);
	DDX_Control(pDX, IDC_BTN_XIAO_JIU, m_btnXiaoJIU);
	DDX_Control(pDX, IDC_BTN_DAN_JIU, m_btnDanJIU);
	DDX_Control(pDX, IDC_BTN_SHUANG_JIU, m_btnShuangJIU);
	DDX_Control(pDX, IDC_BTN_DA_SHI, m_btnDaShi);
	DDX_Control(pDX, IDC_BTN_XIAO_SHI, m_btnXiaoShi);
	DDX_Control(pDX, IDC_BTN_DAN_SHI, m_btnDanShi);
	DDX_Control(pDX, IDC_BTN_SHUANG_SHI, m_btnShuangShi);

	DDX_Control(pDX, IDC_BTN_MORE_RECORD, m_btnMoreRecord);
	DDX_Control(pDX, IDC_BTN_QUSHI, m_btnQushi);

	DDX_Control(pDX, IDC_BTN_YUAN,m_btnYuan);
	DDX_Control(pDX, IDC_BTN_JIAO,m_btnJiao);
	DDX_Control(pDX, IDC_BTN_FEN,m_btnFen);
	DDX_Control(pDX, IDC_BTN_LI,m_btnLi);

	DDX_Control(pDX, IDC_BTN_WAN_QUAN, m_btnWanQuan);
	DDX_Control(pDX, IDC_BTN_WAN_DA, m_btnWanDa);
	DDX_Control(pDX, IDC_BTN_WAN_XIAO, m_btnWanXiao);
	DDX_Control(pDX, IDC_BTN_WAN_DAN, m_btnWanDan);
	DDX_Control(pDX, IDC_BTN_WAN_SHUANG, m_btnWanShuang);
	DDX_Control(pDX, IDC_BTN_WAN_QING, m_btnWanQing);

	DDX_Control(pDX, IDC_BTN_QIAN_QUAN, m_btnQianQuan);
	DDX_Control(pDX, IDC_BTN_QIAN_DA, m_btnQianDa);
	DDX_Control(pDX, IDC_BTN_QIAN_XIAO, m_btnQianXiao);
	DDX_Control(pDX, IDC_BTN_QIAN_DAN, m_btnQianDan);
	DDX_Control(pDX, IDC_BTN_QIAN_SHUANG, m_btnQianShuang);
	DDX_Control(pDX, IDC_BTN_QIAN_QING, m_btnQianQing);

	DDX_Control(pDX, IDC_BTN_BAI_QUAN, m_btnBaiQuan);
	DDX_Control(pDX, IDC_BTN_BAI_DA, m_btnBaiDa);
	DDX_Control(pDX, IDC_BTN_BAI_XIAO, m_btnBaiXiao);
	DDX_Control(pDX, IDC_BTN_BAI_DAN, m_btnBaiDan);
	DDX_Control(pDX, IDC_BTN_BAI_SHUANG, m_btnBaiShuang);
	DDX_Control(pDX, IDC_BTN_BAI_QING, m_btnBaiQing);

	DDX_Control(pDX, IDC_BTN_SHI_QUAN, m_btnShiQuan);
	DDX_Control(pDX, IDC_BTN_SHI_DA, m_btnShiDa);
	DDX_Control(pDX, IDC_BTN_SHI_XIAO, m_btnShiXiao);
	DDX_Control(pDX, IDC_BTN_SHI_DAN, m_btnShiDan);
	DDX_Control(pDX, IDC_BTN_SHI_SHUANG, m_btnShiShuang);
	DDX_Control(pDX, IDC_BTN_SHI_QING, m_btnShiQing);

	DDX_Control(pDX, IDC_BTN_GE_QUAN, m_btnGeQuan);
	DDX_Control(pDX, IDC_BTN_GE_DA, m_btnGeDa);
	DDX_Control(pDX, IDC_BTN_GE_XIAO, m_btnGeXiao);
	DDX_Control(pDX, IDC_BTN_GE_DAN, m_btnGeDan);
	DDX_Control(pDX, IDC_BTN_GE_SHUANG, m_btnGeShuang);
	DDX_Control(pDX, IDC_BTN_GE_QING, m_btnGeQing);

}

BEGIN_MESSAGE_MAP(CBeiJingPK10, CDialog)
	ON_BN_CLICKED(IDC_BTN_YUAN, &CBeiJingPK10::OnBnClickedBtnYuan)
	ON_BN_CLICKED(IDC_BTN_JIAO, &CBeiJingPK10::OnBnClickedBtnJiao)
	ON_BN_CLICKED( IDC_BTN_FEN, &CBeiJingPK10::OnBnClickedBtnFen)
	ON_BN_CLICKED(IDC_BTN_LI, &CBeiJingPK10::OnBnClickedBtnLi)
	ON_BN_CLICKED(IDC_BTN_HSZHX, &CBeiJingPK10::OnBnClickedBtnHszhx)
	ON_BN_CLICKED(IDC_BTN_HSZX, &CBeiJingPK10::OnBnClickedBtnHszx)
	ON_BN_CLICKED(IDC_BTN_WXHE, &CBeiJingPK10::OnBnClickedBtnWxhe)
	ON_BN_CLICKED(IDC_BTN_HEZX, &CBeiJingPK10::OnBnClickedBtnHezx)
	ON_BN_CLICKED(IDC_BTN_DXDSH, &CBeiJingPK10::OnBnClickedBtnDxdsh)
	ON_BN_CLICKED(IDC_BTN_BDW, &CBeiJingPK10::OnBnClickedBtnBdw)
	ON_BN_CLICKED(IDC_BTN_QSZHX, &CBeiJingPK10::OnBnClickedBtnQszhx)
	ON_BN_CLICKED(IDC_BTN_QSZX, &CBeiJingPK10::OnBnClickedBtnQszx)
	ON_BN_CLICKED(IDC_BTN_WXQE, &CBeiJingPK10::OnBnClickedBtnWxqe)
	ON_BN_CLICKED(IDC_BTN_QEZX, &CBeiJingPK10::OnBnClickedBtnQezx)
	ON_BN_CLICKED(IDC_BTN_QSDXDS, &CBeiJingPK10::OnBnClickedBtnQSDXDS)
	ON_BN_CLICKED(IDC_BTN_HSZHXHZH, &CBeiJingPK10::OnBnClickedBtnHszhxhzh)
	ON_BN_CLICKED(IDC_BTN_DWD, &CBeiJingPK10::OnBnClickedBtnDwd)
	ON_BN_CLICKED(IDC_BTN_CHQ_ADD, &CBeiJingPK10::OnBnClickedBtnChqAdd)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RIO_HSZHX_FUSHI, &CBeiJingPK10::OnBnClickedRioHszhxFushi)
	ON_BN_CLICKED(IDC_RIO_RENXUAN2, &CBeiJingPK10::OnBnClickedRioRenXuan2)
	ON_BN_CLICKED(IDC_RIO_HSZS_FUSHI, &CBeiJingPK10::OnBnClickedRioHszsFushi)
	ON_BN_CLICKED(IDC_RIO_HSZS_FUSHI_DANSHI, &CBeiJingPK10::OnBnClickedRioHszsFushiDan)
	ON_BN_CLICKED(IDC_RIO_HSZS_DANSHI, &CBeiJingPK10::OnBnClickedRioHszsDanshi)

	ON_BN_CLICKED(IDC_RIO_QSZS_FUSHI, &CBeiJingPK10::OnBnClickedRioQszsFushi)
	ON_BN_CLICKED(IDC_RIO_QSZS_DANSHI, &CBeiJingPK10::OnBnClickedRioQszsDanshi)//�ڰ�
	ON_BN_CLICKED(IDC_RIO_HEZHX_FUSHI, &CBeiJingPK10::OnBnClickedRioHezhxFushi)
	ON_BN_CLICKED(IDC_RIO_HEZHX_FUSHI_DANSHI, &CBeiJingPK10::OnBnClickedRioHezhxFushiDan)
	ON_BN_CLICKED(IDC_RIO_HEZHX_DANSHI, &CBeiJingPK10::OnBnClickedRioHezhxDanshi)//����
	ON_BN_CLICKED(IDC_RIO_HEZX_FUSHI, &CBeiJingPK10::OnBnClickedRioHezxFushi)
	ON_BN_CLICKED(IDC_RIO_HEZX_FUSHI_DANSHI, &CBeiJingPK10::OnBnClickedRioHezxFushiDan)
	ON_BN_CLICKED(IDC_RIO_HEZX_DANSHI, &CBeiJingPK10::OnBnClickedRioHezxDanshi)	//����
	ON_BN_CLICKED(IDC_RIO_QEZHX_FUSHI, &CBeiJingPK10::OnBnClickedRioQezhxFushi)
	ON_BN_CLICKED(IDC_RIO_QEZHX_DANSHI, &CBeiJingPK10::OnBnClickedRioQezhxDanshi)
	ON_BN_CLICKED(IDC_RIO_QEZX_FUSHI, &CBeiJingPK10::OnBnClickedRioQezxFushi)
	ON_BN_CLICKED(IDC_RIO_QEZX_DANSHI, &CBeiJingPK10::OnBnClickedRioQezxDanshi)
	ON_BN_CLICKED(IDC_RIO_DXDSH, &CBeiJingPK10::OnBnClickedRioDxdsh)
	ON_BN_CLICKED(IDC_RIO_DXDSH_DAN, &CBeiJingPK10::OnBnClickedRioDxdshDan)
	ON_BN_CLICKED(IDC_RIO_QSBDW, &CBeiJingPK10::OnBnClickedRioQsbdw)//����
	ON_BN_CLICKED(IDC_RIO_QSHUNHE, &CBeiJingPK10::OnBnClickedRioQsHunhe)//����
	ON_BN_CLICKED(IDC_RIO_HSBDW, &CBeiJingPK10::OnBnClickedRioHsbdw)
	ON_BN_CLICKED(IDC_RIO_RENXUAN3, &CBeiJingPK10::OnBnClickedRioRenXuan3)//����
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ZHUIHAO, &CBeiJingPK10::OnBnClickedBtnZhuihao)
	ON_BN_CLICKED(IDC_BTN_TOUZHU, &CBeiJingPK10::OnBnClickedBtnTouzhu)
	ON_BN_CLICKED(IDC_BTN_TOUZHU_ONEKEY, &CBeiJingPK10::OnBnClickedBtnTouzhuOneKey)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_DEL_SEL, &CBeiJingPK10::OnBnClickedBtnDelSel)
	ON_BN_CLICKED(IDC_BTN_CLS_LIST, &CBeiJingPK10::OnBnClickedBtnClsList)
	ON_EN_CHANGE(IDC_EDIT_BEISHU, &CBeiJingPK10::OnEnChangeEditBeishu)
	ON_EN_CHANGE(IDC_EDIT_DANSHI_HAOMA, &CBeiJingPK10::OnEnChangeEditDanshiInput)
	ON_EN_CHANGE(IDC_RICHEDIT_DANSHI, &CBeiJingPK10::OnEnChangeRichEditDanshiInput)

	ON_WM_CTLCOLOR()

	ON_WM_MOUSEMOVE()
 	ON_WM_LBUTTONUP()
 	ON_WM_LBUTTONDOWN()
 	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_BN_CLICKED(IDC_RIO_MOUSE_INPUT, &CBeiJingPK10::OnBnClickedRioMouseInput)
	ON_BN_CLICKED(IDC_RIO_KEYBOARD_INPUT, &CBeiJingPK10::OnBnClickedRioKeyboardInput)
	ON_BN_CLICKED(IDC_CHK_WAN, &CBeiJingPK10::OnBnClickedChkWan)
	ON_BN_CLICKED(IDC_CHK_QIAN, &CBeiJingPK10::OnBnClickedChkQian)
	ON_BN_CLICKED(IDC_CHK_BAI, &CBeiJingPK10::OnBnClickedChkBai)
	ON_BN_CLICKED(IDC_CHK_SHI, &CBeiJingPK10::OnBnClickedChkShi)
	ON_BN_CLICKED(IDC_CHKGE, &CBeiJingPK10::OnBnClickedChkge)
	ON_BN_CLICKED(IDC_BTN_LOCK, &CBeiJingPK10::OnBnClickedBtnLock)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIA, &CBeiJingPK10::OnBnClickedBeiTouJia)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIAN, &CBeiJingPK10::OnBnClickedBeiTouJian)

	ON_COMMAND(ID_RBTN_CPY, &CBeiJingPK10::OnRichEditCopy)
	ON_BN_CLICKED(IDC_BTN_MORE_RECORD, &CBeiJingPK10::OnBnClickedBtnMoreRecord)
	ON_BN_CLICKED(IDC_BTN_QUSHI, &CBeiJingPK10::OnBnClickedBtnQuShi)
	ON_MESSAGE( WM_BN_CLICK, &CBeiJingPK10::onBnCLick)

	ON_BN_CLICKED(IDC_BTN_WAN_QUAN, &CBeiJingPK10::OnBnClickedBtnWanQuan)
	ON_BN_CLICKED(IDC_BTN_WAN_DA, &CBeiJingPK10::OnBnClickedBtnWanDa)
	ON_BN_CLICKED(IDC_BTN_WAN_XIAO, &CBeiJingPK10::OnBnClickedBtnWanXiao)
	ON_BN_CLICKED(IDC_BTN_WAN_DAN, &CBeiJingPK10::OnBnClickedBtnWanDan)
	ON_BN_CLICKED(IDC_BTN_WAN_SHUANG, &CBeiJingPK10::OnBnClickedBtnWanShuang)
	ON_BN_CLICKED(IDC_BTN_WAN_QING, &CBeiJingPK10::OnBnClickedBtnWanQing)
	ON_BN_CLICKED(IDC_BTN_QIAN_QUAN, &CBeiJingPK10::OnBnClickedBtnQianQuan)
	ON_BN_CLICKED(IDC_BTN_QIAN_DA, &CBeiJingPK10::OnBnClickedBtnQianDa)
	ON_BN_CLICKED(IDC_BTN_QIAN_XIAO, &CBeiJingPK10::OnBnClickedBtnQianXiao)
	ON_BN_CLICKED(IDC_BTN_QIAN_DAN, &CBeiJingPK10::OnBnClickedBtnQianDan)
	ON_BN_CLICKED(IDC_BTN_QIAN_SHUANG, &CBeiJingPK10::OnBnClickedBtnQianShuang)
	ON_BN_CLICKED(IDC_BTN_QIAN_QING, &CBeiJingPK10::OnBnClickedBtnQianQing)
	ON_BN_CLICKED(IDC_BTN_BAI_QUAN, &CBeiJingPK10::OnBnClickedBtnBaiQuan)
	ON_BN_CLICKED(IDC_BTN_BAI_DA, &CBeiJingPK10::OnBnClickedBtnBaiDa)
	ON_BN_CLICKED(IDC_BTN_BAI_XIAO, &CBeiJingPK10::OnBnClickedBtnBaiXiao)
	ON_BN_CLICKED(IDC_BTN_BAI_DAN, &CBeiJingPK10::OnBnClickedBtnBaiDan)
	ON_BN_CLICKED(IDC_BTN_BAI_SHUANG, &CBeiJingPK10::OnBnClickedBtnBaiShuang)
	ON_BN_CLICKED(IDC_BTN_BAI_QING, &CBeiJingPK10::OnBnClickedBtnBaiQing)
	ON_BN_CLICKED(IDC_BTN_SHI_QUAN, &CBeiJingPK10::OnBnClickedBtnShiQuan)
	ON_BN_CLICKED(IDC_BTN_SHI_DA, &CBeiJingPK10::OnBnClickedBtnShiDa)
	ON_BN_CLICKED(IDC_BTN_SHI_XIAO, &CBeiJingPK10::OnBnClickedBtnShiXiao)
	ON_BN_CLICKED(IDC_BTN_SHI_DAN, &CBeiJingPK10::OnBnClickedBtnShiDan)
	ON_BN_CLICKED(IDC_BTN_SHI_SHUANG, &CBeiJingPK10::OnBnClickedBtnShiShuang)
	ON_BN_CLICKED(IDC_BTN_SHI_QING, &CBeiJingPK10::OnBnClickedBtnShiQing)
	ON_BN_CLICKED(IDC_BTN_GE_QUAN, &CBeiJingPK10::OnBnClickedBtnGeQuan)
	ON_BN_CLICKED(IDC_BTN_GE_DA, &CBeiJingPK10::OnBnClickedBtnGeDa)
	ON_BN_CLICKED(IDC_BTN_GE_XIAO, &CBeiJingPK10::OnBnClickedBtnGeXiao)
	ON_BN_CLICKED(IDC_BTN_GE_DAN, &CBeiJingPK10::OnBnClickedBtnGeDan)
	ON_BN_CLICKED(IDC_BTN_GE_SHUANG, &CBeiJingPK10::OnBnClickedBtnGeShuang)
	ON_BN_CLICKED(IDC_BTN_GE_QING, &CBeiJingPK10::OnBnClickedBtnGeQing)

END_MESSAGE_MAP()

void CBeiJingPK10::OnRichEditCopy()
{
	char *buffer = NULL;
	CString fromClipboard;
	if(OpenClipboard())
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		buffer = (char*)GlobalLock(hData);
		fromClipboard = buffer;
		GlobalUnlock(hData);
		CloseClipboard();
	}

	m_richDanshiHaoma.SetWindowText(fromClipboard);
	//MyMessageBox(fromClipboard);
	return;
}
BOOL CBeiJingPK10::PreTranslateMessage(MSG* pMsg)
{
	//�������
	BOOL  bReturn=TRUE; 
	if(pMsg-> message   ==WM_RBUTTONDOWN) 
	{
		if(pMsg->hwnd == m_richDanshiHaoma.m_hWnd)
		{
			CPoint pt(0,0);
			GetCursorPos(&pt);
			CMenu menu;
			menu.LoadMenu(IDR_MENU_RBTN);
			CMenu* pMenu = menu.GetSubMenu(0);
			if (pMenu != NULL)
			{
				pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
			}

			return true;

		}
	}
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}
	return __super::PreTranslateMessage(pMsg);
}

void CBeiJingPK10::OnBnClickedBtnYuan()
{
	m_btnYuan.SetPushed(true);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Yuan ;

	InvalidateRect(rcZongJinE);

	return;
}
void CBeiJingPK10::OnBnClickedBtnJiao()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(true);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Jiao ;
	InvalidateRect(rcZongJinE);

	return;
}
void CBeiJingPK10::OnBnClickedBtnFen()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(true);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Fen ;
	InvalidateRect(rcZongJinE);
	return;

}
void CBeiJingPK10::OnBnClickedBtnLi()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(true);
	theAccount.Scoretype=MoShi_Li ;
	InvalidateRect(rcZongJinE);

	return;
}


// CBeiJingPK10 ��Ϣ�������
void CBeiJingPK10::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(!IsWindowVisible())
		return;

	// TODO: �ڴ˴������Ϣ����������
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
	graphics.DrawImage(m_bmpBk, Rect(0, 0,m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 28, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	//���ƿ�����
	DrawLastKjHaoma(&cacheDC, graphics);
	DrawTopKjHaoma(&cacheDC, graphics);

	//������ע�����ܽ��
	DrawZongZhushuJinE(&cacheDC);

	cacheDC.SetBkMode(TRANSPARENT);
	cacheDC.SetTextColor(RGB(255,255,255));
	CFont* pOldFont = cacheDC.SelectObject(&m_Font);

	m_FandianView.DrawView(this,&cacheDC,rect.Width(),rect.Height());

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);
	cacheDC.SelectObject(pOldFont);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheDC.DeleteDC();
	cacheBmp.DeleteObject();

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CBeiJingPK10::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CBeiJingPK10::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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

void CBeiJingPK10::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	OnBnClickedBtnClsList();
	ResetAllNums();

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_bKaiJiangzhong = true;
		m_cbIfTs=0;
		SendToServer(11);
		
		SetTimer(timer_id_get_luck_num, 10000, NULL);
		SetTimer(timer_id_kj_yugao, 1000, NULL);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);
		KillTimer(TimerLotteryStatus);
		SetTimer(TimerLotteryStatus,30000,NULL);
		m_btnYuan.SetPushed(false);
		m_btnJiao.SetPushed(false);
		m_btnFen.SetPushed(false);
		m_btnLi.SetPushed(false);
		if(theAccount.Scoretype==MoShi_Yuan )
		{
			m_btnYuan.SetPushed(true);
		}
		else if(theAccount.Scoretype == MoShi_Jiao)
		{
			m_btnJiao.SetPushed(true);
		}
		else if(theAccount.Scoretype == MoShi_Fen)
		{
			m_btnFen.SetPushed(true);
		}
		else if(theAccount.Scoretype == MoShi_Li)
		{
			m_btnLi.SetPushed(true);
		}


		SendToServer(6);
		LuckyNumCQSSC();
		OnBnClickedBtnHszhx();
	}
	else
	{
		KillTimer(TimerLotteryStatus);
		m_DlgOpenRecord.ShowWindow(SW_HIDE);
		KillTimer(timer_id_get_luck_num);
		KillTimer(timer_id_kj_yugao);
		KillTimer(timer_id_kaijiangzhong);
	}
}

HBRUSH CBeiJingPK10::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == IDC_EDIT_DANSHI_HAOMA) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(255,255,255));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}

		return hBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RICHEDIT_DANSHI) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(255,255,255));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}

		return hBrush;
	}

	//���ģʽ����ʾ��Ϣ
	if(CTLCOLOR_STATIC==nCtlColor && pWnd->GetDlgCtrlID() == IDC_STATIC_HUNHE_TIP)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(255,255,255));
// 		HBRUSH B = CreateSolidBrush(RGB(61,47,42)); 
// 		return (HBRUSH) B; 
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}

		return hBrush;
	}
	//��ѡSTATIC
	if(CTLCOLOR_STATIC==nCtlColor && pWnd->GetDlgCtrlID() == IDC_STATIC_RENXUAN_TIP)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(255,255,255));
// 		HBRUSH B = CreateSolidBrush(RGB(61,47,42)); 
// 		return (HBRUSH) B; 
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(245,229,203));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(245,229,203));
		}

		return hBrush;
	}
	//��ѡ����������Ķ�ѡ��
//	if(CTLCOLOR_BTN == nCtlColor )
//	{
		if((pWnd->GetDlgCtrlID() == IDC_CHK_WAN)	||
			(pWnd->GetDlgCtrlID() ==IDC_CHK_QIAN)	||
			(pWnd->GetDlgCtrlID() ==IDC_CHK_BAI)	||
			(pWnd->GetDlgCtrlID() ==IDC_CHK_SHI)	||
			(pWnd->GetDlgCtrlID() ==IDC_CHKGE))
		{
			pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(255,255,255));
// 			HBRUSH B = CreateSolidBrush(RGB(245, 229, 203)); 
// 			return (HBRUSH) B; 
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = CreateSolidBrush(RGB(245, 229, 203));
			}
			else
			{
				hBrush = CreateSolidBrush(RGB(245, 229, 203));
			}

			return hBrush;
		}

//	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBeiJingPK10::OnTimer(UINT_PTR nIDEvent)
{
	if(timer_id_get_luck_num == nIDEvent)
	{
		LuckyNumCQSSC();
	}
	else if(TimerFandian == nIDEvent)
	{
		SendToServer(6);
	}
	else if (timer_id_kj_yugao == nIDEvent)
	{
		CRect rcRect;
		rcRect.CopyRect(rcYuGao);
		//rcRect.right += 80;
		rcRect.bottom += 50;
		InvalidateRect(&rcRect);
	}
	else if(TimerLotteryStatus == nIDEvent)
	{
		SendToServer(11);
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
	else if(timer_id_kaijiangzhong == nIDEvent)
	{
		//������GIF����
		int nTimeLeft = GetKjShjDiff();
		if(m_bKaiJiangzhong )
		{
			CDC* pDC = GetDC();
			if(pDC==NULL)
				return;
			//for(int n=0; n<m_nFrameCount; n++)
			{
				int nLeft=kj_big_haoma_x-3/*+ n *( m_bigNumWidth+8)*/;
				CMemDC	mDC(pDC, CRect(nLeft, kj_big_haoma_y-16, nLeft+m_imgKaijiang->GetWidth()+184, kj_big_haoma_y+m_imgKaijiang->GetHeight()+36));
				Graphics gh(mDC.m_hDC);
				gh.DrawImage(m_imgKaijiang, nLeft, kj_big_haoma_y-16, m_imgKaijiang->GetWidth(), m_imgKaijiang->GetHeight());
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

	CDialog::OnTimer(nIDEvent);
}

//��ѡ��
void CBeiJingPK10::OnBnClickedBtnHszhx()
{
	CheckWanFaBtn(m_btnHouSanZhiXuan);

	HideAllRio();
	ShowCtrl(IDC_RIO_HSZHX_FUSHI);
	

	OnBnClickedRioHszhxFushi();
}
//�¹��Ǿ�
void CBeiJingPK10::OnBnClickedBtnHszx()
{
	CheckWanFaBtn(m_btnHouSanZuXuan);

	HideAllRio();

	ShowCtrl(IDC_RIO_HSZS_FUSHI);
	ShowCtrl(IDC_RIO_HSZS_FUSHI_DANSHI);
	
	ShowCtrl(IDC_RIO_HSZS_DANSHI);

	OnBnClickedRioHszsFushi();
}
//������
void CBeiJingPK10::OnBnClickedBtnWxhe()
{
	CheckWanFaBtn(m_btnWuXingHouEr);

	HideAllRio();

	
	ShowCtrl(IDC_RIO_HEZHX_FUSHI);
	ShowCtrl(IDC_RIO_HEZHX_FUSHI_DANSHI);
	ShowCtrl(IDC_RIO_HEZHX_DANSHI);

	OnBnClickedRioHezhxFushi();
}
//������
void CBeiJingPK10::OnBnClickedBtnHezx()
{
	CheckWanFaBtn(m_btnHouErZuXuan);

	HideAllRio();

	
	ShowCtrl(IDC_RIO_HEZX_FUSHI);
	ShowCtrl(IDC_RIO_HEZX_FUSHI_DANSHI);
	ShowCtrl(IDC_RIO_HEZX_DANSHI);

	OnBnClickedRioHezxFushi();
}
//������
void CBeiJingPK10::OnBnClickedBtnDxdsh()
{
	CheckWanFaBtn(m_btnDaXiaoDanShuang);

	HideAllRio();

	
	ShowCtrl(IDC_RIO_DXDSH);
	ShowCtrl(IDC_RIO_DXDSH_DAN);
	ShowCtrl(IDC_RIO_QSBDW);

	OnBnClickedRioDxdsh();
}
//������
void CBeiJingPK10::OnBnClickedBtnBdw()
{
	CheckWanFaBtn(m_btnBuDingWei);

	HideAllRio();

	
	ShowCtrl(IDC_RIO_HSBDW);
	ShowCtrl(IDC_RIO_QSHUNHE);
	OnBnClickedRioHsbdw();
}

//������
void CBeiJingPK10::OnBnClickedBtnQszhx()
{
	CheckWanFaBtn(m_btnQianSanZhiXuan);

	
	HideAllRio();
	ShowCtrl(IDC_RIO_RENXUAN2);
 	ShowCtrl(IDC_RIO_RENXUAN3);

	OnBnClickedRioRenXuan2();
}
//�°���
void CBeiJingPK10::OnBnClickedBtnQszx()
{
	CheckWanFaBtn(m_btnQianSanZuXuan);

	HideAllRio();

	
	ShowCtrl(IDC_RIO_QSZS_FUSHI);
	ShowCtrl(IDC_RIO_QSZS_DANSHI);

	OnBnClickedRioQszsFushi();
}
//�¾���
void CBeiJingPK10::OnBnClickedBtnWxqe()
{
	CheckWanFaBtn(m_btnWuXingQianEr);

	HideAllRio();

	
	ShowCtrl(IDC_RIO_QEZHX_FUSHI);
	ShowCtrl(IDC_RIO_QEZHX_DANSHI);
	//SelctRadioBtn(IDC_RIO_QEZHX_FUSHI);
	OnBnClickedRioQezhxFushi();
}
//��ʮ��
void CBeiJingPK10::OnBnClickedBtnQezx()
{
	//m_wanfa = QianErZuXuan;
	CheckWanFaBtn(m_btnQianErZuXuan);

	
	HideAllRio();

	ShowCtrl(IDC_RIO_QEZX_FUSHI);
	ShowCtrl(IDC_RIO_QEZX_DANSHI);
	//SelctRadioBtn(IDC_RIO_QEZX_FUSHI);
	OnBnClickedRioQezxFushi();
}
//ǰʮ��С��˫
void CBeiJingPK10::OnBnClickedBtnQSDXDS()
{
	CheckWanFaBtn(m_btnQSDaXiaoDanShuang);

	
	HideAllRio();

	ShowCtrl(IDC_RIO_DWD);

	OnBnClickedRioQsDxds();
}

void CBeiJingPK10::OnBnClickedBtnHszhxhzh()
{
	////m_wanfa = HouSanZhiXuanHeZhi;
	//CheckWanFaBtn(m_btnHouSanHeZhi);

	HideAllRio();
	ShowCtrl(IDC_RIO_QSHUNHE);
	ShowCtrl(IDC_RIO_ZSHUNHE);
	ShowCtrl(IDC_RIO_HSHUNHE);
	ShowCtrl(IDC_STATIC_HUNHE_TIP);

//	SelctRadioBtn(IDC_RIO_HSZHXHZH);
//	OnBnClickedRioQsHunhe();
}

//��λ��
void CBeiJingPK10::OnBnClickedBtnDwd()
{
	CheckWanFaBtn(m_btnDingWeiDan);
	HideAllRio();

	ResetAllNums();

	m_gameKind = WF_DWD;
	SendToServer(3);
	SetLockText();

	m_rioQeZxFuShi.SetPushed(true);
	m_rioQeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
	ShowLiuNums();
	ShowQiNums();
	ShowBaNums();
	ShowJiuNums();
	ShowShi10Nums();

	HideDanShiEdit();

}


BOOL CBeiJingPK10::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\game_bk3.png"));
	m_numImageList = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num4.png"));
	m_kjNumBig = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big4.png"));
	m_imgKaijiang = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\KaiJiang2.gif"));
	int nCount = m_imgKaijiang->GetFrameDimensionsCount();			//��ȡ֡ά��
	GUID *pGuids = new GUID[nCount];								//����һ��GUID����
	m_imgKaijiang->GetFrameDimensionsList(pGuids,nCount);			//��ȡͼ��֡��GUID
	m_nFrameCount=m_imgKaijiang->GetFrameCount(pGuids);				//��ȡGIF֡��
	m_btnMoreRecord.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_more.png"));
	
	m_btnQushi.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_hmzs.png"));

	m_bigNumWidth = m_kjNumBig->GetWidth() / 10;
	m_bigNumHeight = m_kjNumBig->GetHeight();
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small1.png"));
	m_smallNumWidth = m_kjNumSmall->GetWidth() / 11;
	m_smallNumHeight = m_kjNumSmall->GetHeight();
	m_bmpDxdsh = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx4.png"));
	m_bmpNumTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit3.png"));
	ZeroMemory(m_szMD5Haoma,sizeof(m_szMD5Haoma));
	InitNumberBtns();
	InitListCtrl();
	m_btnYuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_1.png"));
	m_btnJiao.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_2.png"));
	m_btnFen.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_3.png"));
	m_btnLi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_4.png"));

	m_bShowFail =true;
	m_DlgOpenRecord.Create(COpenRecord::IDD,this);
	m_smallNumfont.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_zongFont.CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_KjInfoFont1.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_KjInfoFont.CreateFont(35, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 

	m_btnDelSel.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_delate.png"));
	m_btnClsList.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_clear.png"));
	m_btnTouZhu.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_bet.png"));
	m_btnZhuiHao.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_append.png"));
	m_btnChqAdd.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_addnum.png"));
	m_btnLock.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_lock.png"));
	m_btnLock.SetTextColor(RGB(51, 45, 42));
	m_btnBeiTouJia.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jia.png"));
	m_btnBeiTouJian.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jian.png"));
	m_btnLock.SetTextFont(&m_zongFont);
	m_btnTouZhuOnekey.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\yjtz.png"));

	m_richDanshiHaoma.LimitText(-1);
	m_richDanshiHaoma.SetBackgroundColor(false,RGB(200,200,200));
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(88, 78, 77);
	m_richDanshiHaoma.SetDefaultCharFormat(cf);
	m_richDanshiHaoma.SetEventMask(ENM_CHANGE);

	m_editBeiTou.SetEnableColor(RGB(0,0,0),RGB(214,223,254),RGB(214,223,254));
	m_editBeiTou.SetFont(&m_zongFont);
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.SetMaxTipWidth(150);
	m_btWxzxTip.SetImage(CBmpUtil::GetExePath() + _T("skin\\bangzhu.png"));

	LuckyNumCQSSC();

// 	SendQuerySystemTime();
// 	SendQueryLuckyNumCQSSC();

	hBrush=NULL;

	m_bKaiJiangzhong=false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CBeiJingPK10::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
}

void CBeiJingPK10::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}
int SplitString1(const string &srcStr, vector<string> &destVec, const string splitStr="|")
{
	if(srcStr.size()==0 || srcStr.empty())
	{   
		return false;
	}   
	size_t oldPos,newPos;
	oldPos=0;
	newPos=0;
	string tempData;
	while(1)
	{   
		newPos=srcStr.find(splitStr,oldPos);
		if(newPos!=string::npos)
		{   
			tempData = srcStr.substr(oldPos,newPos-oldPos);
			destVec.push_back(tempData);
			oldPos=newPos+splitStr.size();
		}   
		else if(oldPos<=srcStr.size())
		{   
			tempData= srcStr.substr(oldPos);
			destVec.push_back(tempData);
			break;
		}   
		else
		{   
			break;
		}   
	}   
	return true;
}


//��Ӻ���
void CBeiJingPK10::OnBnClickedBtnChqAdd()
{
//	m_DlgStatus.ShowStatusWindow(TEXT("����������ݣ����Ժ�..."));
	CString strWan = GetWanString();
	CString strQian = GetQianString();
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();
	CString strLiu = GetLiuString();
	CString strQi = GetQiString();
	CString strBa = GetBaString();
	CString strJiu = GetJiuString();
	CString strShi10 = GetShi10String();

	CString strTemp;
	CString strHaoMa;
	int zhushu = 1;
	if (WF_GuanJun == m_gameKind)						//�ھ�
	{
		int nLength = strWan.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}


		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;
		if(nLength>16)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();

			MyMessageBox(L"�ھ����ѡ8�����룡");
			return;
		}

		for(int i = 0;i < strWan.GetLength();i++)
		{
			strHaoMa += strWan.GetAt(i);
			if(i != strWan.GetLength()-1 && (i%2==1))
				strHaoMa+=_T(",");
		}


	}
	else if (WF_GuanYaJun == m_gameKind)				//���Ǿ�
	{
		if(m_RenxuanKeyboardInput)
		{
			if (!GetDanShiHaoma(strHaoMa,10240,2))
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
			}

			string sHaoma;
			ChangeStringToA(strHaoMa,sHaoma);
			vector<string> vectHaoma;
			SplitString1(sHaoma,vectHaoma);
			zhushu = vectHaoma.size()-1;
			if(zhushu>72)
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();

				MyMessageBox(L"���Ǿ����Ͷ72ע��");
				return;
			}

		}
		else
		{
			int nGuanLength = strWan.GetLength()/2;
			int nYaLength = strQian.GetLength()/2;

			if (nGuanLength <= 0 || nYaLength <= 0 ) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));

				return;
			}

			int nChongfu = 0;

			CString strGuan;
			CString strYa;

			zhushu=0;
			for(int i = 0;i < (nGuanLength*2);i += 2)
			{
				strGuan = strWan.Mid(i,2);
				for(int j = 0;j < (nYaLength*2);j+=2)
				{
					strYa = strQian.Mid(j,2);
					if(strGuan != strYa)
					{
						zhushu++;
					}
				}
			}
			if (zhushu<=0 ) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
				return;
			}

			if(nGuanLength>8||nYaLength>8)
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();

				MyMessageBox(L"���Ǿ�ÿλ���ѡ8������");
				return;
			}
			strHaoMa += strWan;
			strHaoMa+=_T(",");

			strHaoMa += strQian;

		}

	}
	else if (WF_YaJun == m_gameKind)				//�Ǿ�
	{
		int nLength = strQian.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}

		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;
		if(zhushu>8)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();

			MyMessageBox(L"�Ǿ������ѡ8�����룡");
			return;
		}


		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strQian.GetAt(i);
			if(i != strQian.GetLength()-1 && (i%2==1))
				strHaoMa+=_T(",");
		}
	}
	else if (WF_QianSan == m_gameKind)					//ǰ����
	{
		if(m_RenxuanKeyboardInput)
		{
			if (!GetDanShiHaoma(strHaoMa,10240,3))
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
			}

			string sHaoma;
			ChangeStringToA(strHaoMa,sHaoma);
			vector<string> vectHaoma;
			SplitString1(sHaoma,vectHaoma);
			zhushu = vectHaoma.size()-1;

			if(zhushu>576)
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();

				MyMessageBox(L"ǰ�������Ͷ576ע��");
				return;
			}

		}
		else
		{
			int nGuanLength = strWan.GetLength()/2;
			int nYaLength = strQian.GetLength()/2;
			int nSanLength = strBai.GetLength()/2;

			if (nGuanLength <= 0 || nYaLength <= 0 || nSanLength <= 0 ) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
				return;
			}
			CString strGuan;
			CString strYa;
			CString strSan;

			zhushu = 0;
			for(int i = 0;i < (nGuanLength*2);i+=2)
			{
				strGuan = strWan.Mid(i,2);
				for(int j = 0;j < (nYaLength*2);j+=2)
				{
					strYa = strQian.Mid(j,2);
					if(strGuan == strYa)
						continue;
					for (int k = 0;k < (nSanLength*2);k+=2)
					{
						strSan = strBai.Mid(k,2);
						if(strGuan != strYa && strYa != strSan && strGuan != strSan)
						{
							zhushu++;
						}
					}
				}
			}

			if (zhushu<=0 ) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
				return;
			}
			if (nGuanLength > 8 || nYaLength > 8 || nSanLength > 8 ) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();

				MyMessageBox(L"ǰ����ÿλ���ѡ8�����룡");
				return;
			}

			strHaoMa += strWan;
			strHaoMa+=_T(",");

			strHaoMa += strQian;
			strHaoMa+=_T(",");

			strHaoMa += strBai;

		}

	}
	else if (WF_DiSan == m_gameKind)				//������
	{
		int nLength = strBai.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}

		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;

		if(zhushu>8)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();

			MyMessageBox(L"���������ѡ8�����룡");
			return;
		}

		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strBai.GetAt(i);
			if(i != strBai.GetLength()-1 && (i%2==1))
				strHaoMa+=_T(",");
		}
	}
	else if (WF_QianSi == m_gameKind)					//ǰ����
	{
		if(m_RenxuanKeyboardInput)
		{
			if (!GetDanShiHaoma(strHaoMa,10240,4))
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
			}

			string sHaoma;
			ChangeStringToA(strHaoMa,sHaoma);
			vector<string> vectHaoma;
			SplitString1(sHaoma,vectHaoma);
			zhushu = vectHaoma.size()-1;
			if(zhushu>4032)
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();

				MyMessageBox(L"ǰ�������Ͷ4032ע��");
				return;
			}

		}
		else
		{
			int nGuanLength = strWan.GetLength()/2;
			int nYaLength = strQian.GetLength()/2;
			int nSanLength = strBai.GetLength()/2;
			int nSiLength = strShi.GetLength()/2;

			if (nGuanLength <= 0 || nYaLength <= 0 || nSanLength <= 0 || nSiLength <= 0) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
				return;
			}
			CString strGuan;
			CString strYa;
			CString strSan;
			CString strSi;

			zhushu = 0;
			for(int i = 0;i < (nGuanLength*2);i+=2)
			{
				strGuan = strWan.Mid(i,2);
				for(int j = 0;j < (nYaLength*2);j+=2)
				{
					strYa = strQian.Mid(j,2);
					if(strGuan == strYa)
						continue;
					for (int k = 0;k < (nSanLength*2);k+=2)
					{
						strSan = strBai.Mid(k,2);
						if(strGuan == strSan || strSan == strYa)
							continue;
						for(int l = 0;l < (nSiLength*2);l += 2)
						{
							strSi = strShi.Mid(l,2);
							if(strSi == strGuan || strSi == strYa || strSi == strSan)
								continue;
							zhushu++;
						}
					}
				}
			}

			if (zhushu<=0 ) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
				return;
			}

			if (nGuanLength <= 0 || nYaLength <= 0 || nSanLength <= 0 || nSiLength <= 0) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();

				MyMessageBox(L"ǰ����ÿλ���ѡ8�����룡");
				return;
			}
			strHaoMa += strWan;
			strHaoMa+=_T(",");

			strHaoMa += strQian;
			strHaoMa+=_T(",");

			strHaoMa += strBai;
			strHaoMa+=_T(",");

			strHaoMa += strShi;
		}

	}
	else if (WF_DiSi == m_gameKind)					//������
	{
		int nLength = strShi.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}

		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;
		if(zhushu>8)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();

			MyMessageBox(L"���������ѡ8�����룡");
			return;
		}


		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strShi.GetAt(i);
			if(i != strShi.GetLength()-1 && (i%2==1))
				strHaoMa+=_T(",");
		}

	}
	else if (WF_QianWu == m_gameKind)					//ǰ����
	{
		if(m_RenxuanKeyboardInput)
		{
			if (!GetDanShiHaoma(strHaoMa,10240,5))
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
			}

			string sHaoma;
			ChangeStringToA(strHaoMa,sHaoma);
			vector<string> vectHaoma;
			SplitString1(sHaoma,vectHaoma);
			zhushu = vectHaoma.size()-1;
			if(zhushu>24192)
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();

				MyMessageBox(L"ǰ�������Ͷ24192ע��");
				return;
			}

		}
		else
		{
			int nGuanLength = strWan.GetLength()/2;
			int nYaLength = strQian.GetLength()/2;
			int nSanLength = strBai.GetLength()/2;
			int nSiLength = strShi.GetLength()/2;
			int nWuLength = strGe.GetLength()/2;


			if (nGuanLength <= 0 || nYaLength <= 0 || nSanLength <= 0 || nSiLength <= 0) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();
				MyMessageBox(_T("��ʽ����ȷ��������ѡ��"));
				return;
			}

			CString strGuan;
			CString strYa;
			CString strSan;
			CString strSi;
			CString strWu;

			zhushu = 0;
			for(int i = 0;i < (nGuanLength*2);i+=2)
			{
				strGuan = strWan.Mid(i,2);
				for(int j = 0;j < (nYaLength*2);j+=2)
				{
					strYa = strQian.Mid(j,2);
					if(strGuan == strYa)
						continue;
					for (int k = 0;k < (nSanLength*2);k+=2)
					{
						strSan = strBai.Mid(k,2);
						if(strGuan == strSan || strSan == strYa)
							continue;
						for(int l = 0;l < (nSiLength*2);l += 2)
						{
							strSi = strShi.Mid(l,2);
							if(strSi == strGuan || strSi == strYa || strSi == strSan)
								continue;
							for(int m = 0;m < (nWuLength*2);m+=2)
							{
								strWu = strGe.Mid(m,2);
								if(strWu == strGuan || strWu == strYa || strWu == strSan||strWu == strSi)
									continue;

								zhushu++;
							}
						}
					}
				}
			}
			if (nGuanLength > 8 || nYaLength > 8 || nSanLength > 8 || nSiLength > 8) 
			{
				m_DlgStatus.HideStatusWindow();
				ResetAllNums();

				MyMessageBox(L"ǰ����ÿλ���ѡ8�����룡");
				return;
			}


	// 
	// 		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) )
	// 		{
	// 			m_DlgStatus.HideStatusWindow();
	// 			ResetAllNums();
	// 			MyMessageBox(_T("ǰ5��ǰ10�淨��ÿλֻ��ѡһ�����룡"));
	// 			return;
	// 		}
  			CString str[5] = {strWan,strQian,strBai,strShi,strGe};
	// 		CString strTemp;
	// 		for(int i = 0;i < 5;i++)
	// 		{
	// 			strTemp = str[i];
	// 			for(int j = i+1;j < 5;j++)
	// 			{
	// 				if(strTemp == str[j])
	// 				{
	// 					m_DlgStatus.HideStatusWindow();
	// 					ResetAllNums();
	// 					MyMessageBox(_T("ѡ�Ų����ظ���"));
	// 					return;
	// 				}
	// 			}
	// 		}

	//		zhushu = 1;

			for(int i = 0;i < 5;i++)
			{
				strHaoMa += str[i];
				if(i != 4)
				{
					strHaoMa += _T(",");
				}
			}
		}


	}
	else if (WF_DiWu == m_gameKind)							//������
	{
		int nLength = strGe.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}

		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;

		if(zhushu>8)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();

			MyMessageBox(L"���������ѡ8�����룡");
			return;
		}

		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strGe.GetAt(i);
			if(i != nLength-1 && (i%2==1))
				strHaoMa+=_T(",");
		}
	}
	else if (WF_QianLiu == m_gameKind)				//ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;


		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1))
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("ǰ5��ǰ10�淨��ÿλֻ��ѡһ�����룡"));
			return;
		}
		CString str[6] = {strWan,strQian,strBai,strShi,strGe,strLiu};
		CString strTemp;
		for(int i = 0;i < 6;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 6;j++)
			{
				if(strTemp == str[j])
				{
					m_DlgStatus.HideStatusWindow();
					ResetAllNums();
					MyMessageBox(_T("ѡ�Ų����ظ���"));
					return;
				}
			}
		}

		zhushu = 1;

		for(int i = 0;i < 6;i++)
		{
			strHaoMa += str[i];
			if(i != 5)
			{
				strHaoMa += _T(",");
			}
		}

	}
	else if(WF_DiLiu == m_gameKind)				//������
	{
		int nLength = strLiu.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}
		if(nLength > 16)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("���������ѡ8�����룡"));
			return;
		}

		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;


		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strLiu.GetAt(i);
			if(i != nLength-1 && (i%2==1))
				strHaoMa+=_T(",");
		}

	}
	else if (WF_QianQi == m_gameKind) //ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;

		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1) || (nQiLength != 1))
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("ǰ5��ǰ10�淨��ÿλֻ��ѡһ�����룡"));
			return;
		}
		CString str[7] = {strWan,strQian,strBai,strShi,strGe,strLiu,strQi};
		CString strTemp;
		for(int i = 0;i < 7;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 7;j++)
			{
				if(strTemp == str[j])
				{
					m_DlgStatus.HideStatusWindow();
					ResetAllNums();
					MyMessageBox(_T("ѡ�Ų����ظ���"));
					return;
				}
			}
		}

		zhushu = 1;

		for(int i = 0;i < 7;i++)
		{
			strHaoMa += str[i];
			if(i != 6)
			{
				strHaoMa += _T(",");
			}
		}

	}
	else if(WF_DiQi == m_gameKind)				//������
	{
		int nLength = strQi.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}
		if(nLength > 16)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("���������ѡ8�����룡"));
			return;
		}

		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;


		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strQi.GetAt(i);
			if(i != nLength-1 && (i%2==1))
				strHaoMa+=_T(",");
		}

	}
	else if (WF_QianBa == m_gameKind) //ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;
		int nBaLength = strBa.GetLength()/2;

		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1) || (nQiLength != 1) || (nBaLength != 1))
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("ǰ5��ǰ10�淨��ÿλֻ��ѡһ�����룡"));
			return;
		}
		CString str[8] = {strWan,strQian,strBai,strShi,strGe,strLiu,strQi,strBa};
		CString strTemp;
		for(int i = 0;i < 8;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 8;j++)
			{
				if(strTemp == str[j])
				{
					m_DlgStatus.HideStatusWindow();
					ResetAllNums();
					MyMessageBox(_T("ѡ�Ų����ظ���"));
					return;
				}
			}
		}

		zhushu = 1;

		for(int i = 0;i < 8;i++)
		{
			strHaoMa += str[i];
			if(i != 7)
			{
				strHaoMa += _T(",");
			}
		}

	}
	else if(WF_DiBa == m_gameKind)				//�ڰ���
	{
		int nLength = strBa.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}
		if(nLength > 16)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�ڰ������ѡ8�����룡"));
			return;
		}

		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;


		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strBa.GetAt(i);
			if(i != nLength-1 && (i%2==1))
				strHaoMa+=_T(",");
		}

	}
	else if (WF_QianJiu == m_gameKind) //ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;
		int nBaLength = strBa.GetLength()/2;
		int nJiuLength = strJiu.GetLength()/2;

		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1) || (nQiLength != 1) || (nBaLength != 1) || (nJiuLength != 1))
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("ǰ5��ǰ10�淨��ÿλֻ��ѡһ�����룡"));
			return;
		}
		CString str[9] = {strWan,strQian,strBai,strShi,strGe,strLiu,strQi,strBa,strJiu};
		CString strTemp;
		for(int i = 0;i < 9;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 9;j++)
			{
				if(strTemp == str[j])
				{
					m_DlgStatus.HideStatusWindow();
					ResetAllNums();
					MyMessageBox(_T("ѡ�Ų����ظ���"));
					return;
				}
			}
		}

		zhushu = 1;

		for(int i = 0;i < 9;i++)
		{
			strHaoMa += str[i];
			if(i != 8)
			{
				strHaoMa += _T(",");
			}
		}

	}
	else if(WF_DiJiu == m_gameKind)				//�ھ���
	{
		int nLength = strJiu.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}
		if(nLength > 16)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�ھ������ѡ8�����룡"));
			return;
		}

		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;


		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strJiu.GetAt(i);
			if(i != nLength-1 && (i%2==1))
				strHaoMa+=_T(",");
		}

	}
	else if (WF_QianShi == m_gameKind) //ǰʮ��
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;
		int nBaLength = strBa.GetLength()/2;
		int nJiuLength = strJiu.GetLength()/2;
		int nShiLength = strShi10.GetLength()/2;

		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1) || (nQiLength != 1) || (nBaLength != 1) || (nJiuLength != 1) || (nShiLength != 1))
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("ǰ5��ǰ10�淨��ÿλֻ��ѡһ�����룡"));
			return;
		}
		CString str[10] = {strWan,strQian,strBai,strShi,strGe,strLiu,strQi,strBa,strJiu,strShi10};
		CString strTemp;
		for(int i = 0;i < 10;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 10;j++)
			{
				if(strTemp == str[j])
				{
					m_DlgStatus.HideStatusWindow();
					ResetAllNums();
					MyMessageBox(_T("ѡ�Ų����ظ���"));
					return;
				}
			}
		}

		zhushu = 1;

		for(int i = 0;i < 10;i++)
		{
			strHaoMa += str[i];
			if(i != 9)
			{
				strHaoMa += _T(",");
			}
		}

	}
	else if(WF_DiShi == m_gameKind)				//��ʮ��
	{
		int nLength = strShi10.GetLength();
		if(nLength<2)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;

		}
		if(nLength > 16)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ʮ�����ѡ8�����룡"));
			return;
		}

		if(nLength%2 !=0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("�����ʽ������ѡ����룡"));
			return;
		}

		int nCount = nLength/2;
		zhushu = nCount;


		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strShi10.GetAt(i);
			if(i != nLength-1 && (i%2==1))
				strHaoMa+=_T(",");
		}

	}
	else if(WF_DXDS == m_gameKind)				//��С��˫
	{
		CString strDXDSGuan = GetGuanDxdshString();
		CString strDXDSYa = GetYaDxdshString();
		CString strDXDSSan = GetSanDxdshString();
		CString strDXDSSi = GetSiDxdshString();
		CString strDXDSWu = GetWuDxdshString();
		CString strDXDSLiu = GetLiuDxdshString();
		CString strDXDSQi = GetQiDxdshString();
		CString strDXDSBa = GetBaDxdshString();
		CString strDXDSJiu = GetJiuDxdshString();
		CString strDXDSShi = GetShiDxdshString();

		int nGuanLength = strDXDSGuan.GetLength();
		int nYaLength = strDXDSYa.GetLength();
		int nSanLength = strDXDSSan.GetLength();
		int nSiLength = strDXDSSi.GetLength();
		int nWuLength = strDXDSWu.GetLength();
		int nLiuLength = strDXDSLiu.GetLength();
		int nQiLength = strDXDSQi.GetLength();
		int nBaLength = strDXDSBa.GetLength();
		int nJiuLength = strDXDSJiu.GetLength();
		int nShiLength = strDXDSShi.GetLength();

		if(nGuanLength <= 0 && nYaLength <= 0 && nSanLength <= 0 && nSiLength <= 0 && nWuLength <= 0 && nLiuLength <= 0 && nQiLength <= 0 && nBaLength <= 0 && nJiuLength <= 0 && nShiLength <= 0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;
		}
		else if(nGuanLength > 1 || nYaLength > 1 || nSanLength > 1 || nSiLength > 1 || nWuLength > 1 || nLiuLength > 1 || nQiLength > 1 || nBaLength > 1 || nJiuLength > 1 || nShiLength > 1)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("ÿλ���ѡһ�����룬������ѡ��"));
			return;
		}

		strHaoMa = strDXDSGuan + _T(",") + strDXDSYa + _T(",")+ strDXDSSan + _T(",")+ strDXDSSi + _T(",")+ strDXDSWu + _T(",")+ strDXDSLiu + _T(",")+ strDXDSQi + _T(",")+ strDXDSBa + _T(",")+ strDXDSJiu + _T(",")+ strDXDSShi;
		zhushu = nGuanLength + nYaLength + nSanLength + nSiLength + nWuLength + nLiuLength + nQiLength + nBaLength + nJiuLength + nShiLength;
	}
	else if(m_gameKind == WF_DWD)
	{

		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;
		int nBaLength = strBa.GetLength()/2;
		int nJiuLength = strJiu.GetLength()/2;
		int nShiLength = strShi10.GetLength()/2;

		if(nGuanLength <= 0 && nYaLength <= 0 && nSanLength <= 0 && nSiLength <= 0 && nWuLength <= 0 && nLiuLength <= 0 && nQiLength <= 0 && nBaLength <= 0 && nJiuLength <= 0 && nShiLength <= 0)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��ѡ����룡"));
			return;
		}
		if(nGuanLength > 8 || nYaLength > 8 || nSanLength > 8 || nSiLength > 8 || nWuLength > 8 || nLiuLength > 8 || nQiLength > 8 || nBaLength > 8 || nJiuLength > 8 || nShiLength > 8)
		{
			m_DlgStatus.HideStatusWindow();
			ResetAllNums();
			MyMessageBox(_T("��λ��ÿλ���ѡ��8�����룡"));
			return;

		}

		strHaoMa = strWan + _T(",") + strQian + _T(",")+ strBai + _T(",")+ strShi + _T(",")+ strGe + _T(",")+ strLiu + _T(",")+ strQi + _T(",")+ strBa + _T(",")+ strJiu + _T(",")+ strShi10;
		zhushu = nGuanLength + nYaLength + nSanLength + nSiLength + nWuLength + nLiuLength + nQiLength + nBaLength + nJiuLength + nShiLength;

	}
	UpdateData();

	CStringArray strArr;

	if (!strHaoMa.IsEmpty()) 
	{
		int nLength = strHaoMa.GetLength();
		if(strHaoMa.GetLength()>1024)
		{
			m_DlgStatus.ShowStatusWindow(TEXT("����������ݣ����Ժ�..."));
		}

		int nCount = m_listChqSSC.GetItemCount();
		if(WF_DXDS == m_gameKind)	
		{
			CString strAddHaoma;
			int nAddIndex = 0;
			do 
			{
				CString strAdd;
				if(!AfxExtractSubString(strAdd, strHaoMa, nAddIndex++, ','))
					break;
				if(nAddIndex!=1)
				{
					strAddHaoma += _T(",");
				}
				int nNum = _ttoi(strAdd);
				CString strDesc = GetDxdshStringDesc(nNum);
				strAddHaoma += strDesc;

			} while (nAddIndex < 1000);

			m_listChqSSC.InsertItem(nCount, strAddHaoma);
		}
		else
		{
			if(nLength > 8000)
				m_listChqSSC.InsertItem(nCount, L"������ӳɹ�");
			else
				m_listChqSSC.InsertItem(nCount, strHaoMa);
		}

		wstring strdata;
		strdata = strHaoMa.GetBuffer(0);

		vecAllNums.insert(vecAllNums.begin(),1,strdata);
		m_listChqSSC.SetItemText(nCount, 1, GetScoreMoshi());
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
		DOUBLE fJine = 0.0f;
		fJine = zhushu*m_beishu*danzhujine;
		CString strZongzhu ;
		strZongzhu.Format(L"%.3lf",fJine);
		m_listChqSSC.SetItemText(nCount, 2, strZongzhu);

		m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());
		CString strTmp;
		strTmp.Format(_T("%d"), zhushu);
		m_listChqSSC.SetItemText(nCount, 4, strTmp);
		strTmp.Format(_T("%d"), m_beishu);
		m_listChqSSC.SetItemText(nCount, 5, strTmp);
		m_listChqSSC.createItemButton(nCount, 6, this->GetSafeHwnd());

		m_zongZhuShu += zhushu;

		strHaoMa = _T("");
	}

	int nItemCount = m_listChqSSC.GetItemCount();
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_listChqSSC.GetItemText(i,5);
		CString strZhuSHu = m_listChqSSC.GetItemText(i,4);
		int beishu = _ttoi(strBeiSHu);
		int zhushu = _ttoi(strZhuSHu);
		CString strMoshi = m_listChqSSC.GetItemText(i,1);

		int nScoreType = GetMoshiRet(strMoshi);
		double danzhujine = 2.0;

		if(nScoreType == MoShi_Yuan)
		{	
			danzhujine = 2.0;
		}
		else if (nScoreType == MoShi_Jiao)
		{
			danzhujine = 0.2;
		}
		else if(nScoreType == MoShi_Fen)
		{
			danzhujine = 0.02;
		}
		else if(nScoreType == MoShi_Li)
			danzhujine = 0.002;


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

	m_DlgStatus.HideStatusWindow();

	ResetAllNums();
	return;
}
CString CBeiJingPK10::GetScoreMoshi()
{
	if(theAccount.Scoretype == MoShi_Yuan)
	{
		return _T("Ԫ");
	}
	else 	if(theAccount.Scoretype == MoShi_Jiao)
	{
		return _T("��");
	}
	else 	if(theAccount.Scoretype == MoShi_Fen)
	{
		return _T("��");
	}
	else 	if(theAccount.Scoretype == MoShi_Li)
	{
		return _T("��");
	}

	return _T("Ԫ");
}
int CBeiJingPK10::GetMoshiRet(CString moshi)
{
	if(moshi == _T("Ԫ"))
	{
		return MoShi_Yuan;
	}
	else if(moshi == _T("��"))
	{
		return MoShi_Jiao;
	}
	else if(moshi == _T("��"))
	{
		return MoShi_Fen;
	}
	else if (moshi == _T("��"))
	{
		return MoShi_Li;
	}
	return MoShi_Yuan;
}

CString CBeiJingPK10::GetDxdshStringDesc(int nNum)
{
	CString strDesc;
	switch (nNum)
	{
	case 1:
		{
			strDesc = _T("С");
			break;
		}
	case 2:
		{
			strDesc = _T("˫");
			break;
		}
	case 3:
		{
			strDesc = _T("��");
			break;
		}
	case 9:
		{
			strDesc = _T("��");
			break;
		}
	case 10:
		{
			strDesc = _T("С��");
			break;
		}
	case 11:
		{
			strDesc = _T("С˫");
			break;
		}
	case 12:
		{
			strDesc = _T("��");
			break;
		}
	case 13:
		{
			strDesc = _T("��˫");
			break;
		}
	case 17:
		{
			strDesc = _T("��С");
			break;
		}
	case 18:
		{
			strDesc = _T("����");
			break;
		}
	}
	return strDesc;
}

//ȡ������
VOID CBeiJingPK10::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//��ʼ�����밴ť
void CBeiJingPK10::InitNumberBtns()
{
	m_btnWan_0.SetImage(m_numImageList, 0, 10);
	m_btnWan_1.SetImage(m_numImageList, 1, 10);
	m_btnWan_2.SetImage(m_numImageList, 2, 10);
	m_btnWan_3.SetImage(m_numImageList, 3, 10);
	m_btnWan_4.SetImage(m_numImageList, 4, 10);
	m_btnWan_5.SetImage(m_numImageList, 5, 10);
	m_btnWan_6.SetImage(m_numImageList, 6, 10);
	m_btnWan_7.SetImage(m_numImageList, 7, 10);
	m_btnWan_8.SetImage(m_numImageList, 8, 10);
	m_btnWan_9.SetImage(m_numImageList, 9, 10);

	m_btnQian_0.SetImage(m_numImageList, 0, 10);
	m_btnQian_1.SetImage(m_numImageList, 1, 10);
	m_btnQian_2.SetImage(m_numImageList, 2, 10);
	m_btnQian_3.SetImage(m_numImageList, 3, 10);
	m_btnQian_4.SetImage(m_numImageList, 4, 10);
	m_btnQian_5.SetImage(m_numImageList, 5, 10);
	m_btnQian_6.SetImage(m_numImageList, 6, 10);
	m_btnQian_7.SetImage(m_numImageList, 7, 10);
	m_btnQian_8.SetImage(m_numImageList, 8, 10);
	m_btnQian_9.SetImage(m_numImageList, 9, 10);

	m_btnBai_0.SetImage(m_numImageList, 0, 10);
	m_btnBai_1.SetImage(m_numImageList, 1, 10);
	m_btnBai_2.SetImage(m_numImageList, 2, 10);
	m_btnBai_3.SetImage(m_numImageList, 3, 10);
	m_btnBai_4.SetImage(m_numImageList, 4, 10);
	m_btnBai_5.SetImage(m_numImageList, 5, 10);
	m_btnBai_6.SetImage(m_numImageList, 6, 10);
	m_btnBai_7.SetImage(m_numImageList, 7, 10);
	m_btnBai_8.SetImage(m_numImageList, 8, 10);
	m_btnBai_9.SetImage(m_numImageList, 9, 10);

	m_btnShi_0.SetImage(m_numImageList, 0, 10);
	m_btnShi_1.SetImage(m_numImageList, 1, 10);
	m_btnShi_2.SetImage(m_numImageList, 2, 10);
	m_btnShi_3.SetImage(m_numImageList, 3, 10);
	m_btnShi_4.SetImage(m_numImageList, 4, 10);
	m_btnShi_5.SetImage(m_numImageList, 5, 10);
	m_btnShi_6.SetImage(m_numImageList, 6, 10);
	m_btnShi_7.SetImage(m_numImageList, 7, 10);
	m_btnShi_8.SetImage(m_numImageList, 8, 10);
	m_btnShi_9.SetImage(m_numImageList, 9, 10);

	m_btnGe_0.SetImage(m_numImageList, 0, 10);
	m_btnGe_1.SetImage(m_numImageList, 1, 10);
	m_btnGe_2.SetImage(m_numImageList, 2, 10);
	m_btnGe_3.SetImage(m_numImageList, 3, 10);
	m_btnGe_4.SetImage(m_numImageList, 4, 10);
	m_btnGe_5.SetImage(m_numImageList, 5, 10);
	m_btnGe_6.SetImage(m_numImageList, 6, 10);
	m_btnGe_7.SetImage(m_numImageList, 7, 10);
	m_btnGe_8.SetImage(m_numImageList, 8, 10);
	m_btnGe_9.SetImage(m_numImageList, 9, 10);

	m_btnLiu_0.SetImage(m_numImageList, 0, 10);
	m_btnLiu_1.SetImage(m_numImageList, 1, 10);
	m_btnLiu_2.SetImage(m_numImageList, 2, 10);
	m_btnLiu_3.SetImage(m_numImageList, 3, 10);
	m_btnLiu_4.SetImage(m_numImageList, 4, 10);
	m_btnLiu_5.SetImage(m_numImageList, 5, 10);
	m_btnLiu_6.SetImage(m_numImageList, 6, 10);
	m_btnLiu_7.SetImage(m_numImageList, 7, 10);
	m_btnLiu_8.SetImage(m_numImageList, 8, 10);
	m_btnLiu_9.SetImage(m_numImageList, 9, 10);

	m_btnQi_0.SetImage(m_numImageList, 0, 10);
	m_btnQi_1.SetImage(m_numImageList, 1, 10);
	m_btnQi_2.SetImage(m_numImageList, 2, 10);
	m_btnQi_3.SetImage(m_numImageList, 3, 10);
	m_btnQi_4.SetImage(m_numImageList, 4, 10);
	m_btnQi_5.SetImage(m_numImageList, 5, 10);
	m_btnQi_6.SetImage(m_numImageList, 6, 10);
	m_btnQi_7.SetImage(m_numImageList, 7, 10);
	m_btnQi_8.SetImage(m_numImageList, 8, 10);
	m_btnQi_9.SetImage(m_numImageList, 9, 10);

	m_btnBa_0.SetImage(m_numImageList, 0, 10);
	m_btnBa_1.SetImage(m_numImageList, 1, 10);
	m_btnBa_2.SetImage(m_numImageList, 2, 10);
	m_btnBa_3.SetImage(m_numImageList, 3, 10);
	m_btnBa_4.SetImage(m_numImageList, 4, 10);
	m_btnBa_5.SetImage(m_numImageList, 5, 10);
	m_btnBa_6.SetImage(m_numImageList, 6, 10);
	m_btnBa_7.SetImage(m_numImageList, 7, 10);
	m_btnBa_8.SetImage(m_numImageList, 8, 10);
	m_btnBa_9.SetImage(m_numImageList, 9, 10);

	m_btnJiu_0.SetImage(m_numImageList, 0, 10);
	m_btnJiu_1.SetImage(m_numImageList, 1, 10);
	m_btnJiu_2.SetImage(m_numImageList, 2, 10);
	m_btnJiu_3.SetImage(m_numImageList, 3, 10);
	m_btnJiu_4.SetImage(m_numImageList, 4, 10);
	m_btnJiu_5.SetImage(m_numImageList, 5, 10);
	m_btnJiu_6.SetImage(m_numImageList, 6, 10);
	m_btnJiu_7.SetImage(m_numImageList, 7, 10);
	m_btnJiu_8.SetImage(m_numImageList, 8, 10);
	m_btnJiu_9.SetImage(m_numImageList, 9, 10);

	m_btnShi10_0.SetImage(m_numImageList, 0, 10);
	m_btnShi10_1.SetImage(m_numImageList, 1, 10);
	m_btnShi10_2.SetImage(m_numImageList, 2, 10);
	m_btnShi10_3.SetImage(m_numImageList, 3, 10);
	m_btnShi10_4.SetImage(m_numImageList, 4, 10);
	m_btnShi10_5.SetImage(m_numImageList, 5, 10);
	m_btnShi10_6.SetImage(m_numImageList, 6, 10);
	m_btnShi10_7.SetImage(m_numImageList, 7, 10);
	m_btnShi10_8.SetImage(m_numImageList, 8, 10);
	m_btnShi10_9.SetImage(m_numImageList, 9, 10);

	m_staticWan.SetImage(m_bmpNumTip, 0, 10);
	m_staticQian.SetImage(m_bmpNumTip, 1, 10);
	m_staticBai.SetImage(m_bmpNumTip, 2, 10);
	m_staticShi.SetImage(m_bmpNumTip, 3, 10);
	m_staticGe.SetImage(m_bmpNumTip, 4, 10);
	m_staticLiu.SetImage(m_bmpNumTip, 5, 10);
	m_staticQi.SetImage(m_bmpNumTip, 6, 10);
	m_staticBa.SetImage(m_bmpNumTip, 7, 10);
	m_staticJiu.SetImage(m_bmpNumTip, 8, 10);
	m_staticShi10.SetImage(m_bmpNumTip, 9, 10);

	m_btnDaGUAN.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoGUAN.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanGUAN.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangGUAN.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaYA.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoYA.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanYA.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangYA.SetImage(m_bmpDxdsh, 3, 4);

	m_btnDaSAN.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoSAN.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanSAN.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangSAN.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaSI.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoSI.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanSI.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangSI.SetImage(m_bmpDxdsh, 3, 4);

	m_btnDaWU.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoWU.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanWU.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangWU.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaLIU.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoLIU.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanLIU.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangLIU.SetImage(m_bmpDxdsh, 3, 4);

	m_btnDaQI.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoQI.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanQI.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangQI.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaBA.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoBA.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanBA.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangBA.SetImage(m_bmpDxdsh, 3, 4);

	m_btnDaJIU.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoJIU.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanJIU.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangJIU.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaShi.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoShi.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanShi.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangShi.SetImage(m_bmpDxdsh, 3, 4);



	CString numTextBk = CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx.png");
	m_btnWanQuan.SetBkImage(numTextBk);
	m_btnWanDa.SetBkImage(numTextBk);
	m_btnWanXiao.SetBkImage(numTextBk);
	m_btnWanDan.SetBkImage(numTextBk);
	m_btnWanShuang.SetBkImage(numTextBk);
	m_btnWanQing.SetBkImage(numTextBk);

	m_btnQianQuan.SetBkImage(numTextBk);
	m_btnQianDa.SetBkImage(numTextBk);
	m_btnQianXiao.SetBkImage(numTextBk);
	m_btnQianDan.SetBkImage(numTextBk);
	m_btnQianShuang.SetBkImage(numTextBk);
	m_btnQianQing.SetBkImage(numTextBk);

	m_btnBaiQuan.SetBkImage(numTextBk);
	m_btnBaiDa.SetBkImage(numTextBk);
	m_btnBaiXiao.SetBkImage(numTextBk);
	m_btnBaiDan.SetBkImage(numTextBk);
	m_btnBaiShuang.SetBkImage(numTextBk);
	m_btnBaiQing.SetBkImage(numTextBk);


	m_btnShiQuan.SetBkImage(numTextBk);
	m_btnShiDa.SetBkImage(numTextBk);
	m_btnShiXiao.SetBkImage(numTextBk);
	m_btnShiDan.SetBkImage(numTextBk);
	m_btnShiShuang.SetBkImage(numTextBk);
	m_btnShiQing.SetBkImage(numTextBk);

	m_btnGeQuan.SetBkImage(numTextBk);
	m_btnGeDa.SetBkImage(numTextBk);
	m_btnGeXiao.SetBkImage(numTextBk);
	m_btnGeDan.SetBkImage(numTextBk);
	m_btnGeShuang.SetBkImage(numTextBk);
	m_btnGeQing.SetBkImage(numTextBk);


	m_btnWanQuan.SetTextColor(RGB(97,76,59));
	m_btnWanDa.SetTextColor(RGB(97,76,59));
	m_btnWanXiao.SetTextColor(RGB(97,76,59));
	m_btnWanDan.SetTextColor(RGB(97,76,59));
	m_btnWanShuang.SetTextColor(RGB(97,76,59));
	m_btnWanQing.SetTextColor(RGB(97,76,59));

	m_btnQianQuan.SetTextColor(RGB(97,76,59));
	m_btnQianDa.SetTextColor(RGB(97,76,59));
	m_btnQianXiao.SetTextColor(RGB(97,76,59));
	m_btnQianDan.SetTextColor(RGB(97,76,59));
	m_btnQianShuang.SetTextColor(RGB(97,76,59));
	m_btnQianQing.SetTextColor(RGB(97,76,59));

	m_btnBaiQuan.SetTextColor(RGB(97,76,59));
	m_btnBaiDa.SetTextColor(RGB(97,76,59));
	m_btnBaiXiao.SetTextColor(RGB(97,76,59));
	m_btnBaiDan.SetTextColor(RGB(97,76,59));
	m_btnBaiShuang.SetTextColor(RGB(97,76,59));
	m_btnBaiQing.SetTextColor(RGB(97,76,59));

	m_btnShiQuan.SetTextColor(RGB(97,76,59));
	m_btnShiDa.SetTextColor(RGB(97,76,59));
	m_btnShiXiao.SetTextColor(RGB(97,76,59));
	m_btnShiDan.SetTextColor(RGB(97,76,59));
	m_btnShiShuang.SetTextColor(RGB(97,76,59));
	m_btnShiQing.SetTextColor(RGB(97,76,59));

	m_btnGeQuan.SetTextColor(RGB(97,76,59));
	m_btnGeDa.SetTextColor(RGB(97,76,59));
	m_btnGeXiao.SetTextColor(RGB(97,76,59));
	m_btnGeDan.SetTextColor(RGB(97,76,59));
	m_btnGeShuang.SetTextColor(RGB(97,76,59));
	m_btnGeQing.SetTextColor(RGB(97,76,59));

}

void CBeiJingPK10::InitListCtrl()
{
	m_listChqSSC.SetExtendedStyle(m_listChqSSC.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);
	m_listChqSSC.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	m_listChqSSC.SetPenColor(RGB(247,233,216));
	m_listChqSSC.InsertColumn(0, _T("����"), LVCFMT_CENTER, 240);
	m_listChqSSC.InsertColumn(1, _T("ģʽ"), LVCFMT_CENTER, 25);
	m_listChqSSC.InsertColumn(2, _T("���"), LVCFMT_CENTER, 95);
	m_listChqSSC.InsertColumn(3, _T("�淨"), LVCFMT_CENTER, 80);
	m_listChqSSC.InsertColumn(4, _T("ע��"), LVCFMT_CENTER, 55);
	m_listChqSSC.InsertColumn(5, _T("����"), LVCFMT_CENTER, 50);
	m_listChqSSC.InsertColumn(6, _T("ɾ��"), LVCFMT_CENTER, 31);
}

//�ھ�
void CBeiJingPK10::OnBnClickedRioHszhxFushi()
{
	ResetAllNums();

	m_gameKind = WF_GuanJun;		
	SendToServer(3);
	SetLockText();

	m_rioHsZhxFuShi.SetPushed(true);
	HideDaXiaoDanShuang();
	ShowWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();
	HideDanShiEdit();

	ShowWanNumsQ();
}

//������
void CBeiJingPK10::OnBnClickedRioRenXuan2()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_QianQi;
	SendToServer(3);
	m_rioRenXuan2.SetPushed(true);
	m_rioRenXuan3.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
	ShowLiuNums();
	ShowQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();
}
//�µ�����
void CBeiJingPK10::OnBnClickedRioRenXuan3()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_DiQi;
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan3.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	ShowQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();
}

//�¹��Ǿ�
void CBeiJingPK10::OnBnClickedRioHszsFushi()
{
	ResetAllNums();

	m_gameKind = WF_GuanYaJun;
	SendToServer(3);
	SetLockText();

	m_rioHsZsFuShi.SetPushed(true);
	m_rioHsZsFuShiDanshi.SetPushed(false);
	m_rioHsZsDanShi.SetPushed(false);
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();

	ShowWanNumsQ();
	ShowQianNumsQ();
	
}
//�¹��Ǿ���ʽ
void CBeiJingPK10::OnBnClickedRioHszsFushiDan()
{
	ResetAllNums();

	m_gameKind = WF_GuanYaJun;
	m_RenxuanKeyboardInput = true;
	SendToServer(3);
	SetLockText();
	m_rioHsZsFuShi.SetPushed(false);
	m_rioHsZsFuShiDanshi.SetPushed(true);

	m_rioHsZsDanShi.SetPushed(false);
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	ShowDanShiEdit();

	HideWanNumsQ();
	HideQianNumsQ();
	
}
//���Ǿ�
void CBeiJingPK10::OnBnClickedRioHszsDanshi()
{
	ResetAllNums();

	m_gameKind = WF_YaJun;
	SendToServer(3);
	SetLockText();

	m_rioHsZsFuShi.SetPushed(false);
	m_rioHsZsDanShi.SetPushed(true);
	m_rioHsZsFuShiDanshi.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	ShowQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();
	ShowQianNumsQ();
}


//�°���
void CBeiJingPK10::OnBnClickedRioQszsFushi()
{
	ResetAllNums();

	m_gameKind = WF_QianBa;
	SendToServer(3);
	SetLockText();

	m_rioQsZsFuShi.SetPushed(true);
	m_rioQsZsDanShi.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
	ShowLiuNums();
	ShowQiNums();
	ShowBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();
}
//�ڰ���
void CBeiJingPK10::OnBnClickedRioQszsDanshi()
{
	ResetAllNums();

	m_gameKind = WF_DiBa;
	SendToServer(3);
	SetLockText();

	m_rioQsZsFuShi.SetPushed(false);
	m_rioQsZsDanShi.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	ShowBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();
}


//��ǰ��
void CBeiJingPK10::OnBnClickedRioHezhxFushi()
{
	ResetAllNums();

	m_gameKind = WF_QianSan;
	SetLockText();
	m_rioHeZhxFuShi.SetPushed(true);
	m_rioHeZhxFuShiDan.SetPushed(false);
	m_rioHeZhxDanShi.SetPushed(false);
	SendToServer(3);
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();

	ShowWanNumsQ();
	ShowQianNumsQ();
	ShowBaiNumsQ();
}
//��ǰ����ʽ
void CBeiJingPK10::OnBnClickedRioHezhxFushiDan()
{
	ResetAllNums();

	m_gameKind = WF_QianSan;
	SetLockText();
	m_rioHeZhxFuShi.SetPushed(false);
	m_rioHeZhxFuShiDan.SetPushed(true);
	m_rioHeZhxDanShi.SetPushed(false);
	m_RenxuanKeyboardInput = true;
	SendToServer(3);
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	ShowDanShiEdit();

	HideWanNumsQ();
	HideQianNumsQ();
	HideBaiNumsQ();
}
//�µ���
void CBeiJingPK10::OnBnClickedRioHezhxDanshi()
{
	ResetAllNums();

	m_gameKind = WF_DiSan;
	SetLockText();
	m_rioHeZhxDanShi.SetPushed(true);
	m_rioHeZhxFuShiDan.SetPushed(false);
	m_rioHeZhxFuShi.SetPushed(false);

	SendToServer(3);
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();
	ShowBaiNumsQ();

}

//��ǰ��
void CBeiJingPK10::OnBnClickedRioHezxFushi()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_QianSi;
	SendToServer(3);

	m_rioHeZxFuShi.SetPushed(true);
	m_rioHeZxFuShiDan.SetPushed(false);
	m_rioHeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();
	ShowWanNumsQ();
	ShowQianNumsQ();
	ShowBaiNumsQ();
	ShowShiNumsQ();

}
//��ǰ��
void CBeiJingPK10::OnBnClickedRioHezxFushiDan()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_QianSi;
	m_RenxuanKeyboardInput=true;
	SendToServer(3);
	m_rioHeZxFuShi.SetPushed(false);
	m_rioHeZxFuShiDan.SetPushed(true);
	m_rioHeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	ShowDanShiEdit();
	HideWanNumsQ();
	HideQianNumsQ();
	HideBaiNumsQ();
	HideShiNumsQ();

}
//�µ���
void CBeiJingPK10::OnBnClickedRioHezxDanshi()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_DiSi;
	SendToServer(3);

	m_rioHeZxFuShi.SetPushed(false);
	m_rioHeZxDanShi.SetPushed(true);
	m_rioHeZxFuShiDan.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	ShowShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();

	ShowShiNumsQ();

}

//ǰ��ֱѡѡ��
void CBeiJingPK10::OnBnClickedRioQezhxFushi()
{
	ResetAllNums();

	m_gameKind = WF_QianJiu;
	SendToServer(3);

	SetLockText();

	m_rioQeZhxFuShi.SetPushed(true);
	m_rioQeZhxDanShi.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
	ShowLiuNums();
	ShowQiNums();
	ShowBaNums();
	ShowJiuNums();
	HideShi10Nums();

	HideDanShiEdit();

}

//�ھ���
void CBeiJingPK10::OnBnClickedRioQezhxDanshi()
{
	ResetAllNums();

	m_gameKind = WF_DiJiu;
	SendToServer(3);
	SetLockText();

	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	ShowJiuNums();
	HideShi10Nums();

}

//��ǰʮ
void CBeiJingPK10::OnBnClickedRioQezxFushi()
{
	ResetAllNums();

	m_gameKind = WF_QianShi;
	SendToServer(3);
	SetLockText();

	m_rioQeZxFuShi.SetPushed(true);
	m_rioQeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
	ShowLiuNums();
	ShowQiNums();
	ShowBaNums();
	ShowJiuNums();
	ShowShi10Nums();

	HideDanShiEdit();

}
//��ǰʮ��С��˫
void CBeiJingPK10::OnBnClickedRioQsDxds()
{
	ResetAllNums();

	m_gameKind = WF_DXDS;
	SendToServer(3);
	SetLockText();

	m_rioDwd.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();

	ShowDaXiaoDanShuang();
}

//�µ�ʮ
void CBeiJingPK10::OnBnClickedRioQezxDanshi()
{
	ResetAllNums();

	m_gameKind = WF_DiShi;

	SetLockText();
	SendToServer(3);

	m_rioQeZxFuShi.SetPushed(false);
	m_rioQeZxDanShi.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	ShowShi10Nums();
	
	HideDanShiEdit();

}

//��ǰ��
void CBeiJingPK10::OnBnClickedRioDxdsh()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_QianWu;
	SendToServer(3);

	m_rioQeDxdsh.SetPushed(true);
	m_rioQeDxdshDan.SetPushed(false);

	m_rioQsbdw.SetPushed(false);
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();

	ShowWanNumsQ();
	ShowQianNumsQ();
	ShowBaiNumsQ();
	ShowShiNumsQ();
	ShowGeNumsQ();

}
//��ǰ��
void CBeiJingPK10::OnBnClickedRioDxdshDan()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_QianWu;
	m_RenxuanKeyboardInput = true;
	SendToServer(3);

	m_rioQeDxdsh.SetPushed(false);
	m_rioQeDxdshDan.SetPushed(true);

	m_rioQsbdw.SetPushed(false);
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	ShowDanShiEdit();

	HideWanNumsQ();
	HideQianNumsQ();
	HideBaiNumsQ();
	HideShiNumsQ();
	HideGeNumsQ();

}
//�µ���
void CBeiJingPK10::OnBnClickedRioQsbdw()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_DiWu;
	SendToServer(3);

	m_rioQeDxdsh.SetPushed(false);
	m_rioQsbdw.SetPushed(true);
	m_rioQeDxdshDan.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	ShowGeNums();
	HideLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();

	HideDanShiEdit();
	ShowGeNumsQ();
}


//������
void CBeiJingPK10::OnBnClickedRioHsbdw()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_QianLiu;
	SendToServer(3);

	m_rioHsbdw.SetPushed(true);
	m_rioQsHunhe.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
	ShowLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();
	HideDanShiEdit();

}

//������
void CBeiJingPK10::OnBnClickedRioQsHunhe()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = WF_DiLiu;
	SendToServer(3);

	m_rioHsbdw.SetPushed(false);
	m_rioQsHunhe.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	ShowLiuNums();
	HideQiNums();
	HideBaNums();
	HideJiuNums();
	HideShi10Nums();
	HideDanShiEdit();

}


//׷��
void CBeiJingPK10::OnBnClickedBtnZhuihao()
{

	if(!CheckInput())
		return ;

	CDlgAddtional	dlgZhuiHao;
	CString strBeishu ;
	strBeishu.Format(L"%d",1);
	int nCount = 0 ;
	nCount = m_listChqSSC.GetItemCount();
	DOUBLE fJine = 0.00f;
	for (int i = 0;i < nCount;i++)
	{
		CString strBeishu = m_listChqSSC.GetItemText(i,5);
		int nBeishu = _ttoi(strBeishu);
		CString strJine = m_listChqSSC.GetItemText(i,2);
		DOUBLE fSjine = _wtof(strJine);

		DOUBLE fSingle = fSjine/nBeishu;

		fJine += fSingle;
	}

	dlgZhuiHao.SetCurrentQihaoTime( m_chqRule.GetNextExpect(-1), strBeishu,0,8, fJine,999999,CZ_PK10);
	if(dlgZhuiHao.DoModal() == IDOK)
	{
		long kjing = m_chqRule.GetKjShjDiff(); 
		if (kjing < 30 )						//|| kjing > 540
		{
			MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
			return ;
		}


		m_nZhuihaoCount = dlgZhuiHao.GetZhuiHaoQiShu();
		CString haomao = m_listChqSSC.GetItemText(0, 0);
		CString gamekind = m_listChqSSC.GetItemText(0, 3);
		CString zhushu = m_listChqSSC.GetItemText(0, 4);				//��ǰע���������Ǹ�ʽʱ
		m_bZhuiZjtz = dlgZhuiHao.GetPrizeStop();

		vecData1.clear();
		vecData2.clear();
		vecData3.clear();

	//	bool bSucc=true;
		dlgZhuiHao.GetZhuiHaoData(vecData1, vecData2, vecData3);
		DOUBLE fZongJine = 0.0;
		for(int i = 0;i < m_nZhuihaoCount;i++)
		{
			int nBeishu = _ttoi(vecData1[i].c_str());
			fZongJine += fJine*nBeishu;
		}
		if(fZongJine > theAccount.yue)
		{
			MyMessageBox(_T("�����㣬���ֵ������"));
		}
		else
			SendToServer(4);
// 		if (bSucc)
// 		{
// 			MyMessageBox(_T("�ɹ�Ͷע��"), _T("��ף������ Ͷע�ɹ���"));
// 		}
// 		else
// 		{
// 			MyMessageBox(_T("�ж���δ�ɹ��ύ��"));
// 		}
	}
	else
		return;
	
	//��ʼ������
	//m_zongZhuShu = 0;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
OnBnClickedBtnClsList();
	//m_beishu = 1;
	UpdateData(FALSE);
}

//У������
bool	CBeiJingPK10::CheckInput()
{
	if (_tcslen(m_lastExpect) == 0)
	{
		MyMessageBox(_T("���ڻ�ȡ���ݡ��� "));
		return false;
	}

	CString str;
	m_editBeiTou.GetWindowText(str);
	if (str.IsEmpty())
	{
		MyMessageBox(_T("Ͷע��������Ϊ��"));
		return false;
	}

	int itm_cnt = m_listChqSSC.GetItemCount();
	if (itm_cnt == 0) 
	{
		MyMessageBox(_T("����δѡ�����"));
		return false;
	}


	if(theAccount.yue < m_zongJine)
	{
		MyMessageBox(_T("�����㣬���ֵ������"));
		return false;
	}
	//�ж��Ƿ��ٽ�������
	//���»�ȡ������ʱ��
	//theApp.GetTimeDiff();
	long kjing = m_chqRule.GetKjShjDiff(); 
	if (kjing < 30 )						//|| kjing > 540
	{
		MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
		return false;
	}

	return true;
}
void CBeiJingPK10::OnBnClickedBtnTouzhuOneKey()
{
	if(vecAllNums.size()<=0)
	{
		OnBnClickedBtnChqAdd();
	}
	OnBnClickedBtnTouzhuQ();
}
//��ͨ��ע
void CBeiJingPK10::OnBnClickedBtnTouzhuQ()
{
	CString strHao = m_listChqSSC.GetItemText(0,0);
	if(strHao.IsEmpty())
	{
		OnBnClickedBtnChqAdd();
	}

	//У������
	if(!CheckInput())
		return;


	SendToServer(1);
	OnBnClickedBtnClsList();

	//m_zongZhuShu = 0;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	//m_beishu = 1;
	UpdateData(FALSE);


}

//��ͨ��ע
void CBeiJingPK10::OnBnClickedBtnTouzhu()
{
	CString strHao = m_listChqSSC.GetItemText(0,0);
	if(strHao.IsEmpty())
	{
		OnBnClickedBtnChqAdd();
	}

	//У������
	if(!CheckInput())
		return;

	//���û�ȷ����Ϣ
	CQrTzhDlg qrDlg;
	qrDlg.m_gamekind = _T("����PK10");
	qrDlg.m_zhushu = m_zongZhuShu * m_beishu;
	qrDlg.m_moshi = theAccount.Scoretype;
	qrDlg.m_zongjine = m_zongJine;
	if (qrDlg.DoModal() != IDOK)
	{
		return;
	}
	long kjing = m_chqRule.GetKjShjDiff(); 
	if (kjing < 30 )						//|| kjing > 540
	{
		MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
		return ;
	}

	SendToServer(1);
	OnBnClickedBtnClsList();

	//m_zongZhuShu = 0;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	//m_beishu = 1;
	UpdateData(FALSE);


}

void CBeiJingPK10::OnBnClickedBtnDelSel()
{
	POSITION pos = m_listChqSSC.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listChqSSC.GetNextSelectedItem(pos);

		int zhushu = _ttoi(m_listChqSSC.GetItemText(nItem, 4));
		m_zongZhuShu -= zhushu;
		if(vecAllNums.size() > 0)
		{
			vector<wstring>::iterator   iter = vecAllNums.begin()+nItem;
			vecAllNums.erase(iter);

		}

		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=400;
		InvalidateRect(&rcInvali);
		InvalidateRect(&rcZongJinE);

		m_listChqSSC.DeleteItem(nItem);
	}
}

void CBeiJingPK10::OnBnClickedBtnClsList()
{
	m_zongZhuShu = 0;
	m_zongJine = 0.0;
	m_editBeiTou.SetWindowText(L"1");
	m_listChqSSC.deleteAllItemEx();
	vecAllNums.clear();
	m_strHaoma.Empty();

	//m_editDanShiHaoMa.SetWindowText(L"");
	m_richDanshiHaoma.SetWindowText(L"");
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);

}

void CBeiJingPK10::OnEnChangeEditDanshiInput()
{
	//m_editDanShiHaoMa.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
	m_richDanshiHaoma.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
}

void CBeiJingPK10::OnEnChangeEditBeishu()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString str;
	m_editBeiTou.GetWindowText(str);
	if ( (_ttoi(str) < 1)) 
	{
		if(!str.IsEmpty())
		{
			MyMessageBox(_T("Ͷע����������һ��������"));
			m_beishu = 1;
			UpdateData(FALSE);
		}
	}
	else
	{
		UpdateData();
		if(m_RenxuanKeyboardInput)
			GetDanShiZhusu(m_gameKind);

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

void CBeiJingPK10::CheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(true);
}

void CBeiJingPK10::UnCheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(false);
}

bool CBeiJingPK10::IsNumBtnCheck(CNumerButton& btn)
{
	return btn.IsSelect();
}

void CBeiJingPK10::HideCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}
void CBeiJingPK10::ShowCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_SHOW);
	}
}

void CBeiJingPK10::HideAllRio()
{
	//��ѡ2,3,4
	HideCtrl(IDC_RIO_RENXUAN2);
	HideCtrl(IDC_RIO_RENXUAN3);
	HideCtrl(IDC_RIO_RENXUAN4);

	//����
	HideCtrl(IDC_RIO_HSZHX_FUSHI);
	HideCtrl(IDC_RIO_HSZHX_DANSHI);

	//ǰ��
	HideCtrl(IDC_RIO_QSZHX_FUSHI);
	HideCtrl(IDC_RIO_QSZHX_DANSHI);

	HideCtrl(IDC_RIO_HSZS_FUSHI);
	HideCtrl(IDC_RIO_HSZS_FUSHI_DANSHI);
	HideCtrl(IDC_RIO_HSZS_DANSHI);
	HideCtrl(IDC_RIO_HSZL_FUSHI);
	//HideCtrl(IDC_RIO_HSZL_DANSHI);

	HideCtrl(IDC_RIO_QSZS_FUSHI);
	HideCtrl(IDC_RIO_QSZS_DANSHI);
	HideCtrl(IDC_RIO_QSZL_FUSHI);
	//HideCtrl(IDC_RIO_QSZL_DANSHI);

	HideCtrl(IDC_RIO_HEZHX_FUSHI);
	HideCtrl(IDC_RIO_HEZHX_FUSHI_DANSHI);
	HideCtrl(IDC_RIO_HEZHX_DANSHI);

	HideCtrl(IDC_RIO_QEZHX_FUSHI);
	HideCtrl(IDC_RIO_QEZHX_DANSHI);

	HideCtrl(IDC_RIO_HEZX_FUSHI);
	HideCtrl(IDC_RIO_HEZX_FUSHI_DANSHI);
	HideCtrl(IDC_RIO_HEZX_DANSHI);

	HideCtrl(IDC_RIO_QEZX_FUSHI);
	HideCtrl(IDC_RIO_QEZX_DANSHI);

	HideCtrl(IDC_RIO_DXDSH);
	HideCtrl(IDC_RIO_DXDSH_DAN);

	HideCtrl(IDC_RIO_HSZHXHZH);

	HideCtrl(IDC_RIO_HSBDW);
	HideCtrl(IDC_RIO_QSBDW);

	HideCtrl(IDC_RIO_DWD);

	HideCtrl(IDC_RIO_QSHUNHE);
	HideCtrl(IDC_RIO_ZSHUNHE);
	HideCtrl(IDC_RIO_HSHUNHE);
	HideCtrl(IDC_STATIC_HUNHE_TIP);
	//������������OR��������
	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);

	//��ѡ2,3,4ʱ�������������ǧ���٣�ʮ����
	HideCtrl(IDC_CHK_WAN);
	HideCtrl(IDC_CHK_QIAN);
	HideCtrl(IDC_CHK_BAI);
	HideCtrl(IDC_CHK_SHI);
	HideCtrl(IDC_CHKGE);
	HideCtrl(IDC_STATIC_RENXUAN_TIP);
}

//void CBeiJingPK10::SelctRadioBtn(int ctrl_id)
//{
//	CButton* pRadio = (CButton*)GetDlgItem(ctrl_id);
//	if (pRadio != NULL) 
//	{
//		pRadio->SetCheck(BST_CHECKED);
//	}
//}

CString CBeiJingPK10::GetWanString()
{
	CString str;
	if (IsNumBtnCheck(m_btnWan_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnWan_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnWan_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnWan_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnWan_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnWan_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnWan_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnWan_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnWan_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnWan_9))
	{
		str += _T("10");
	}
	return str;
}

CString CBeiJingPK10::GetQianString()
{
	CString str;
	if (IsNumBtnCheck(m_btnQian_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnQian_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnQian_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnQian_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnQian_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnQian_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnQian_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnQian_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnQian_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnQian_9))
	{
		str += _T("10");
	}
	return str;
}

CString CBeiJingPK10::GetBaiString()
{
	CString str;
	if (IsNumBtnCheck(m_btnBai_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnBai_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnBai_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnBai_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnBai_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnBai_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnBai_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnBai_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnBai_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnBai_9))
	{
		str += _T("10");
	}
	return str;
}

CString CBeiJingPK10::GetShiString()
{
	CString str;
	if (IsNumBtnCheck(m_btnShi_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnShi_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnShi_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnShi_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnShi_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnShi_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnShi_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnShi_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnShi_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnShi_9))
	{
		str += _T("10");
	}
	return str;
}

CString CBeiJingPK10::GetGeString()
{
	CString str;
	if (IsNumBtnCheck(m_btnGe_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnGe_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnGe_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnGe_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnGe_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnGe_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnGe_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnGe_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnGe_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnGe_9))
	{
		str += _T("10");
	}
	return str;
}

CString CBeiJingPK10::GetLiuString()
{
	CString str;
	if (IsNumBtnCheck(m_btnLiu_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnLiu_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnLiu_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnLiu_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnLiu_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnLiu_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnLiu_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnLiu_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnLiu_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnLiu_9))
	{
		str += _T("10");
	}
	return str;
}
CString CBeiJingPK10::GetQiString()
{
	CString str;
	if (IsNumBtnCheck(m_btnQi_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnQi_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnQi_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnQi_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnQi_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnQi_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnQi_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnQi_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnQi_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnQi_9))
	{
		str += _T("10");
	}
	return str;
}
CString CBeiJingPK10::GetBaString()
{
	CString str;
	if (IsNumBtnCheck(m_btnBa_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnBa_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnBa_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnBa_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnBa_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnBa_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnBa_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnBa_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnBa_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnBa_9))
	{
		str += _T("10");
	}
	return str;
}
CString CBeiJingPK10::GetJiuString()
{
	CString str;
	if (IsNumBtnCheck(m_btnJiu_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnJiu_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnJiu_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnJiu_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnJiu_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnJiu_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnJiu_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnJiu_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnJiu_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnJiu_9))
	{
		str += _T("10");
	}
	return str;
}
CString CBeiJingPK10::GetShi10String()
{
	CString str;
	if (IsNumBtnCheck(m_btnShi10_0))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnShi10_1))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnShi10_2))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnShi10_3))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnShi10_4))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnShi10_5))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnShi10_6))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnShi10_7))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnShi10_8))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnShi10_9))
	{
		str += _T("10");
	}
	return str;
}

CString CBeiJingPK10::GetShiDxdshString()
{
	CString strShi;
	if (IsNumBtnCheck(m_btnDaShi)) 
	{
		strShi += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoShi)) 
	{
		strShi += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanShi)) 
	{
		strShi += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangShi)) 
	{
		strShi += _T("2");
	}

	return strShi;
}

CString CBeiJingPK10::GetGuanDxdshString()
{
	CString strGUAN;
	if (IsNumBtnCheck(m_btnDaGUAN)) 
	{
		strGUAN += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoGUAN)) 
	{
		strGUAN += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanGUAN)) 
	{
		strGUAN += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangGUAN)) 
	{
		strGUAN += _T("2");
	}

	return strGUAN;
}
CString CBeiJingPK10::GetYaDxdshString()
{
	CString strYA;
	if (IsNumBtnCheck(m_btnDaYA)) 
	{
		strYA += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoYA)) 
	{
		strYA += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanYA)) 
	{
		strYA += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangYA)) 
	{
		strYA += _T("2");
	}

	return strYA;
}
CString CBeiJingPK10::GetSanDxdshString()
{
	CString strSAN;
	if (IsNumBtnCheck(m_btnDaSAN)) 
	{
		strSAN += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoSAN)) 
	{
		strSAN += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanSAN)) 
	{
		strSAN += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangSAN)) 
	{
		strSAN += _T("2");
	}

	return strSAN;
}
CString CBeiJingPK10::GetSiDxdshString()
{
	CString strSI;
	if (IsNumBtnCheck(m_btnDaSI)) 
	{
		strSI += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoSI)) 
	{
		strSI += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanSI)) 
	{
		strSI += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangSI)) 
	{
		strSI += _T("2");
	}

	return strSI;
}
CString CBeiJingPK10::GetWuDxdshString()
{
	CString strWU;
	if (IsNumBtnCheck(m_btnDaWU)) 
	{
		strWU += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoWU)) 
	{
		strWU += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanWU)) 
	{
		strWU += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangWU)) 
	{
		strWU += _T("2");
	}

	return strWU;
}
CString CBeiJingPK10::GetLiuDxdshString()
{
	CString strLIU;
	if (IsNumBtnCheck(m_btnDaLIU)) 
	{
		strLIU += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoLIU)) 
	{
		strLIU += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanLIU)) 
	{
		strLIU += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangLIU)) 
	{
		strLIU += _T("2");
	}

	return strLIU;
}
CString CBeiJingPK10::GetQiDxdshString()
{
	CString strQI;
	if (IsNumBtnCheck(m_btnDaQI)) 
	{
		strQI += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoQI)) 
	{
		strQI += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanQI)) 
	{
		strQI += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangQI)) 
	{
		strQI += _T("2");
	}

	return strQI;
}
CString CBeiJingPK10::GetBaDxdshString()
{
	CString strBA;
	if (IsNumBtnCheck(m_btnDaBA)) 
	{
		strBA += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoBA)) 
	{
		strBA += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanBA)) 
	{
		strBA += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangBA)) 
	{
		strBA += _T("2");
	}

	return strBA;
}
CString CBeiJingPK10::GetJiuDxdshString()
{
	CString strJIU;
	if (IsNumBtnCheck(m_btnDaJIU)) 
	{
		strJIU += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoJIU)) 
	{
		strJIU += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanJIU)) 
	{
		strJIU += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangJIU)) 
	{
		strJIU += _T("2");
	}

	return strJIU;
}

CString CBeiJingPK10::GetWanDesc()
{
	return theGameKind[(CZ_PK10 << 16) | (unsigned short)m_gameKind];
}

PK10GameKind CBeiJingPK10::GetGameKindByDesc(const CString& desc)
{
	if (desc == _T("�ھ�")) 
	{
		return WF_GuanJun;
	} 
	else if (desc == _T("���Ǿ�")) 
	{
		return WF_GuanYaJun;
	} 
	else if (desc == _T("�Ǿ�")) 
	{
		return WF_YaJun;
	} 
	else if (desc == _T("ǰ����")) 
	{
		return WF_QianSan;
	}
	else if (desc == _T("������")) 
	{
		return WF_DiSan;
	}
	else if (desc == _T("ǰ����")) 
	{
		return WF_QianSi;
	}
	else if (desc == _T("������")) 
	{
		return WF_DiSi;
	}
	else if (desc == _T("ǰ����")) 
	{
		return WF_QianWu;
	}
	else if (desc == _T("������")) 
	{
		return WF_DiWu;
	}
	else if (desc == _T("ǰ����")) 
	{
		return WF_QianLiu;
	}
	else if (desc == _T("������")) 
	{
		return WF_DiLiu;
	}
	else if (desc == _T("ǰ����"))
	{
		return WF_QianQi;
	}
	else if (desc == _T("������")) 
	{
		return WF_DiQi;
	}
	else if (desc == _T("ǰ����")) 
	{
		return WF_QianBa;
	}
	else if (desc == _T("�ڰ���")) 
	{
		return WF_DiBa;
	}
	else if (desc == _T("ǰ����"))
	{
		return WF_QianJiu;
	}
	else if (desc == _T("�ھ���")) 
	{
		return WF_DiJiu;
	}
	else if (desc == _T("ǰʮ��")) 
	{
		return WF_QianShi;
	}
	else if (desc == _T("��ʮ��")) 
	{
		return WF_DiShi;
	}
	else if (desc == _T("��λ��С��˫")) 
	{
		return WF_DXDS;
	}
	else if (desc == _T("��λ��")) 
	{
		return WF_DWD;
	}
	return PK10inValid;
}

void CBeiJingPK10::AdjustWanFa()
{
	if (m_btnHouSanZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouSanZhiXuan.SetWindowPos(NULL, wanfa_x, wanfa_y, m_btnHouSanZhiXuan.Width(), m_btnHouSanZhiXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianSanZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianSanZhiXuan.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*6+10, wanfa_y /*+ wanfa_row_height*/, m_btnQianSanZhiXuan.Width(), m_btnQianSanZhiXuan.Height(), SWP_NOZORDER);
		m_btnQianSanZhiXuan.ShowWindow(SW_HIDE);
	}

	if (m_btnHouSanZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouSanZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width, wanfa_y, m_btnHouSanZuXuan.Width(), m_btnHouSanZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianSanZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianSanZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*7+10, wanfa_y , m_btnQianSanZuXuan.Width(), m_btnQianSanZuXuan.Height(), SWP_NOZORDER);
		m_btnQianSanZuXuan.ShowWindow(SW_HIDE);
	}

	if (m_btnWuXingHouEr.GetSafeHwnd() != NULL) 
	{
		m_btnWuXingHouEr.SetWindowPos(NULL, wanfa_x + wanfa_col_width*2+10, wanfa_y, m_btnWuXingHouEr.Width(), m_btnWuXingHouEr.Height(), SWP_NOZORDER);
	}

	if (m_btnWuXingQianEr.GetSafeHwnd() != NULL) 
	{
		m_btnWuXingQianEr.SetWindowPos(NULL, wanfa_x + wanfa_col_width*8+10, wanfa_y , m_btnWuXingQianEr.Width(), m_btnWuXingQianEr.Height(), SWP_NOZORDER);
		m_btnWuXingQianEr.ShowWindow(SW_HIDE);
	}

	if (m_btnHouErZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouErZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*3+10, wanfa_y, m_btnHouErZuXuan.Width(), m_btnHouErZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianErZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianErZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*9+10, wanfa_y , m_btnQianErZuXuan.Width(), m_btnQianErZuXuan.Height(), SWP_NOZORDER);
		m_btnQianErZuXuan.ShowWindow(SW_HIDE);

	}

	if (m_btnDaXiaoDanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnDaXiaoDanShuang.SetWindowPos(NULL, wanfa_x + wanfa_col_width*4+10, wanfa_y, m_btnDaXiaoDanShuang.Width(), m_btnDaXiaoDanShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnQSDaXiaoDanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnQSDaXiaoDanShuang.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*5+10 , wanfa_y, m_btnQSDaXiaoDanShuang.Width(), m_btnQSDaXiaoDanShuang.Height(), SWP_NOZORDER);
	}

	if (m_btnBuDingWei.GetSafeHwnd() != NULL) 
	{
		m_btnBuDingWei.SetWindowPos(NULL, wanfa_x + wanfa_col_width*5+10, wanfa_y, m_btnBuDingWei.Width(), m_btnBuDingWei.Height(), SWP_NOZORDER);
		m_btnBuDingWei.ShowWindow(SW_HIDE);
	}

 	if (m_btnDingWeiDan.GetSafeHwnd() != NULL) 
 	{
 		m_btnDingWeiDan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*6+10*2, wanfa_y, m_btnDingWeiDan.Width(), m_btnDingWeiDan.Height(), SWP_NOZORDER);
 	}
}

//����Ͷע��ʽ ��ѡ��
void CBeiJingPK10::AdjustRadio()
{
	//��ѡ2 3  4
	CWnd* pWnd = GetDlgItem(IDC_RIO_RENXUAN2);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN3);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN4);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//----����ֱѡ��ʽ-----------����ֱѡ��ʽ--------------------
	pWnd = GetDlgItem(IDC_RIO_HSZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//----------ǰ��ֱѡ��ʽ----------------ǰ��ֱѡ��ʽ---------
	pWnd = GetDlgItem(IDC_RIO_QSZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QSZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HSZS_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSZS_FUSHI_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width+31, wanfa_sel_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSZS_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width+31)*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSZL_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSZL_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_QSZS_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QSZS_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QSZL_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QSZL_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HEZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HEZHX_FUSHI_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width+31, wanfa_sel_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HEZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width+31)*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_QEZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QEZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HEZX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HEZX_FUSHI_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width+31, wanfa_sel_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HEZX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width+31)*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_QEZX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QEZX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_DXDSH);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_DXDSH_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width+31, wanfa_sel_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HSZHXHZH);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HSBDW);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QSBDW);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width+31)*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_DWD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//��ϣ�ǰ��������������
	pWnd = GetDlgItem(IDC_RIO_QSHUNHE);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZSHUNHE);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSHUNHE);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//�����ʾ
	pWnd = GetDlgItem(IDC_STATIC_HUNHE_TIP);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*3, wanfa_sel_y+5, wanfa_sel_width*4, wanfa_sel_height, SWP_NOZORDER);
	}
	//�������
	pWnd = GetDlgItem(IDC_RIO_MOUSE_INPUT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, mouseinput_x, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//��������
	pWnd = GetDlgItem(IDC_RIO_KEYBOARD_INPUT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL,keyboardinput_x, keyboardinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
}

//Ͷע��ѡ�����֣�5λ���֣�0--9��ѡ��
void CBeiJingPK10::AdjustNumBtn()
{
	//��λ------------------------------
	if (m_staticWan.GetSafeHwnd() != NULL) 
	{
		m_staticWan.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y, m_staticWan.Width(), m_staticWan.Height(), SWP_NOZORDER);
	}

	if(m_btnWan_0.GetSafeHwnd() != NULL) 
	{
		m_btnWan_0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y, m_btnWan_0.Width(), m_btnWan_0.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_1.GetSafeHwnd() != NULL) 
	{
		m_btnWan_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y, m_btnWan_1.Width(), m_btnWan_1.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_2.GetSafeHwnd() != NULL) 
	{
		m_btnWan_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y, m_btnWan_2.Width(), m_btnWan_2.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_3.GetSafeHwnd() != NULL) 
	{
		m_btnWan_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y, m_btnWan_3.Width(), m_btnWan_3.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_4.GetSafeHwnd() != NULL) 
	{
		m_btnWan_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y, m_btnWan_4.Width(), m_btnWan_4.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_5.GetSafeHwnd() != NULL) 
	{
		m_btnWan_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y, m_btnWan_5.Width(), m_btnWan_5.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_6.GetSafeHwnd() != NULL) 
	{
		m_btnWan_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y, m_btnWan_6.Width(), m_btnWan_6.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_7.GetSafeHwnd() != NULL) 
	{
		m_btnWan_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y, m_btnWan_7.Width(), m_btnWan_7.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_8.GetSafeHwnd() != NULL) 
	{
		m_btnWan_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y, m_btnWan_8.Width(), m_btnWan_8.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_9.GetSafeHwnd() != NULL) 
	{
		m_btnWan_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y, m_btnWan_9.Width(), m_btnWan_9.Height(), SWP_NOZORDER);
	}



	//ǧλ-------------------------------
	if (m_staticQian.GetSafeHwnd() != NULL) 
	{
		m_staticQian.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height, m_staticQian.Width(), m_staticQian.Height(), SWP_NOZORDER);
	}

	if(m_btnQian_0.GetSafeHwnd() != NULL) 
	{
		m_btnQian_0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height, m_btnQian_0.Width(), m_btnQian_0.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_1.GetSafeHwnd() != NULL) 
	{
		m_btnQian_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height, m_btnQian_1.Width(), m_btnQian_1.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_2.GetSafeHwnd() != NULL) 
	{
		m_btnQian_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height, m_btnQian_2.Width(), m_btnQian_2.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_3.GetSafeHwnd() != NULL) 
	{
		m_btnQian_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height, m_btnQian_3.Width(), m_btnQian_3.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_4.GetSafeHwnd() != NULL) 
	{
		m_btnQian_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height, m_btnQian_4.Width(), m_btnQian_4.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_5.GetSafeHwnd() != NULL) 
	{
		m_btnQian_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height, m_btnQian_5.Width(), m_btnQian_5.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_6.GetSafeHwnd() != NULL) 
	{
		m_btnQian_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height, m_btnQian_6.Width(), m_btnQian_6.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_7.GetSafeHwnd() != NULL) 
	{
		m_btnQian_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height, m_btnQian_7.Width(), m_btnQian_7.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_8.GetSafeHwnd() != NULL) 
	{
		m_btnQian_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height, m_btnQian_8.Width(), m_btnQian_8.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_9.GetSafeHwnd() != NULL) 
	{
		m_btnQian_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height, m_btnQian_9.Width(), m_btnQian_9.Height(), SWP_NOZORDER);
	}


	//��λ-------------------------------

	if (m_staticBai.GetSafeHwnd() != NULL) 
	{
		m_staticBai.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}

	if(m_btnBai_0.GetSafeHwnd() != NULL) 
	{
		m_btnBai_0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_0.Width(), m_btnBai_0.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_1.GetSafeHwnd() != NULL) 
	{
		m_btnBai_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_1.Width(), m_btnBai_1.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_2.GetSafeHwnd() != NULL) 
	{
		m_btnBai_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_2.Width(), m_btnBai_2.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_3.GetSafeHwnd() != NULL) 
	{
		m_btnBai_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_3.Width(), m_btnBai_3.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_4.GetSafeHwnd() != NULL) 
	{
		m_btnBai_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_4.Width(), m_btnBai_4.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_5.GetSafeHwnd() != NULL) 
	{
		m_btnBai_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_5.Width(), m_btnBai_5.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_6.GetSafeHwnd() != NULL) 
	{
		m_btnBai_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_6.Width(), m_btnBai_6.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_7.GetSafeHwnd() != NULL) 
	{
		m_btnBai_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_7.Width(), m_btnBai_7.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_8.GetSafeHwnd() != NULL) 
	{
		m_btnBai_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_8.Width(), m_btnBai_8.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_9.GetSafeHwnd() != NULL) 
	{
		m_btnBai_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_9.Width(), m_btnBai_9.Height(), SWP_NOZORDER);
	}

	//ʮλ-------------------------------
	if (m_staticShi.GetSafeHwnd() != NULL) 
	{
		m_staticShi.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*3, m_staticShi.Width(), m_staticShi.Height(), SWP_NOZORDER);
	}

	if(m_btnShi_0.GetSafeHwnd() != NULL) 
	{
		m_btnShi_0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_0.Width(), m_btnShi_0.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_1.GetSafeHwnd() != NULL) 
	{
		m_btnShi_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_1.Width(), m_btnShi_1.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_2.GetSafeHwnd() != NULL) 
	{
		m_btnShi_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_2.Width(), m_btnShi_2.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_3.GetSafeHwnd() != NULL) 
	{
		m_btnShi_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_3.Width(), m_btnShi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_4.GetSafeHwnd() != NULL) 
	{
		m_btnShi_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_4.Width(), m_btnShi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_5.GetSafeHwnd() != NULL) 
	{
		m_btnShi_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_5.Width(), m_btnShi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_6.GetSafeHwnd() != NULL) 
	{
		m_btnShi_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_6.Width(), m_btnShi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_7.GetSafeHwnd() != NULL) 
	{
		m_btnShi_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_7.Width(), m_btnShi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_8.GetSafeHwnd() != NULL) 
	{
		m_btnShi_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_8.Width(), m_btnShi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_9.GetSafeHwnd() != NULL) 
	{
		m_btnShi_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_9.Width(), m_btnShi_9.Height(), SWP_NOZORDER);
	}



	//��λ-------------------------------
	if (m_staticGe.GetSafeHwnd() != NULL) 
	{
		m_staticGe.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*4, m_staticGe.Width(), m_staticGe.Height(), SWP_NOZORDER);
	}

	if(m_btnGe_0.GetSafeHwnd() != NULL) 
	{
		m_btnGe_0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_0.Width(), m_btnGe_0.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_1.GetSafeHwnd() != NULL) 
	{
		m_btnGe_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_1.Width(), m_btnGe_1.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_2.GetSafeHwnd() != NULL) 
	{
		m_btnGe_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_2.Width(), m_btnGe_2.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_3.GetSafeHwnd() != NULL) 
	{
		m_btnGe_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_3.Width(), m_btnGe_3.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_4.GetSafeHwnd() != NULL) 
	{
		m_btnGe_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_4.Width(), m_btnGe_4.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_5.GetSafeHwnd() != NULL) 
	{
		m_btnGe_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_5.Width(), m_btnGe_5.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_6.GetSafeHwnd() != NULL) 
	{
		m_btnGe_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_6.Width(), m_btnGe_6.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_7.GetSafeHwnd() != NULL) 
	{
		m_btnGe_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_7.Width(), m_btnGe_7.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_8.GetSafeHwnd() != NULL) 
	{
		m_btnGe_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_8.Width(), m_btnGe_8.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_9.GetSafeHwnd() != NULL) 
	{
		m_btnGe_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_9.Width(), m_btnGe_9.Height(), SWP_NOZORDER);
	}

	//////��
	if (m_staticLiu.GetSafeHwnd() != NULL) 
	{
		m_staticLiu.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_tip_y, m_staticLiu.Width(), m_staticLiu.Height(), SWP_NOZORDER);
	}

	if (m_staticQi.GetSafeHwnd() != NULL) 
	{
		m_staticQi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_tip_y + haoma_btn_row_height, m_staticQi.Width(), m_staticQi.Height(), SWP_NOZORDER);
	}
	if (m_staticBa.GetSafeHwnd() != NULL) 
	{
		m_staticBa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_tip_y + haoma_btn_row_height*2, m_staticBa.Width(), m_staticBa.Height(), SWP_NOZORDER);
	}

	if (m_staticJiu.GetSafeHwnd() != NULL) 
	{
		m_staticJiu.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_tip_y + haoma_btn_row_height*3, m_staticJiu.Width(), m_staticJiu.Height(), SWP_NOZORDER);
	}

	if (m_staticShi10.GetSafeHwnd() != NULL) 
	{
		m_staticShi10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_tip_y + haoma_btn_row_height*4, m_staticShi10.Width(), m_staticShi10.Height(), SWP_NOZORDER);
	}


	int second_haoma_btn_x = haoma_btn_x + haoma_btn_col_wdith*13;
	if(m_btnLiu_0.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_0.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y, m_btnLiu_0.Width(), m_btnLiu_0.Height(), SWP_NOZORDER);
	}
	if(m_btnLiu_1.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_1.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y, m_btnLiu_1.Width(), m_btnLiu_1.Height(), SWP_NOZORDER);
	}
	if(m_btnLiu_2.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_2.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y, m_btnLiu_2.Width(), m_btnLiu_2.Height(), SWP_NOZORDER);
	}
	if(m_btnLiu_3.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_3.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y, m_btnLiu_3.Width(), m_btnLiu_3.Height(), SWP_NOZORDER);
	}
	if(m_btnLiu_4.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_4.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y, m_btnLiu_4.Width(), m_btnLiu_4.Height(), SWP_NOZORDER);
	}
	if(m_btnLiu_5.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_5.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y, m_btnLiu_5.Width(), m_btnLiu_5.Height(), SWP_NOZORDER);
	}
	if(m_btnLiu_6.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_6.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y, m_btnLiu_6.Width(), m_btnLiu_6.Height(), SWP_NOZORDER);
	}
	if(m_btnLiu_7.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_7.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y, m_btnLiu_7.Width(), m_btnLiu_7.Height(), SWP_NOZORDER);
	}
	if(m_btnLiu_8.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_8.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y, m_btnLiu_8.Width(), m_btnLiu_8.Height(), SWP_NOZORDER);
	}
	if(m_btnLiu_9.GetSafeHwnd() != NULL) 
	{
		m_btnLiu_9.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y, m_btnLiu_9.Width(), m_btnLiu_9.Height(), SWP_NOZORDER);
	}

	if(m_btnQi_0.GetSafeHwnd() != NULL) 
	{
		m_btnQi_0.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y+ haoma_btn_row_height, m_btnQi_0.Width(), m_btnQi_0.Height(), SWP_NOZORDER);
	}
	if(m_btnQi_1.GetSafeHwnd() != NULL) 
	{
		m_btnQi_1.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y+ haoma_btn_row_height, m_btnQi_1.Width(), m_btnQi_1.Height(), SWP_NOZORDER);
	}
	if(m_btnQi_2.GetSafeHwnd() != NULL) 
	{
		m_btnQi_2.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+ haoma_btn_row_height, m_btnQi_2.Width(), m_btnQi_2.Height(), SWP_NOZORDER);
	}
	if(m_btnQi_3.GetSafeHwnd() != NULL) 
	{
		m_btnQi_3.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y+ haoma_btn_row_height, m_btnQi_3.Width(), m_btnQi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnQi_4.GetSafeHwnd() != NULL) 
	{
		m_btnQi_4.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+ haoma_btn_row_height, m_btnQi_4.Width(), m_btnQi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnQi_5.GetSafeHwnd() != NULL) 
	{
		m_btnQi_5.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y+ haoma_btn_row_height, m_btnQi_5.Width(), m_btnQi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnQi_6.GetSafeHwnd() != NULL) 
	{
		m_btnQi_6.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+ haoma_btn_row_height, m_btnQi_6.Width(), m_btnQi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnQi_7.GetSafeHwnd() != NULL) 
	{
		m_btnQi_7.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y+ haoma_btn_row_height, m_btnQi_7.Width(), m_btnQi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnQi_8.GetSafeHwnd() != NULL) 
	{
		m_btnQi_8.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y+ haoma_btn_row_height, m_btnQi_8.Width(), m_btnQi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnQi_9.GetSafeHwnd() != NULL) 
	{
		m_btnQi_9.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y+ haoma_btn_row_height, m_btnQi_9.Width(), m_btnQi_9.Height(), SWP_NOZORDER);
	}

	if(m_btnBa_0.GetSafeHwnd() != NULL) 
	{
		m_btnBa_0.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_0.Width(), m_btnBa_0.Height(), SWP_NOZORDER);
	}
	if(m_btnBa_1.GetSafeHwnd() != NULL) 
	{
		m_btnBa_1.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_1.Width(), m_btnBa_1.Height(), SWP_NOZORDER);
	}
	if(m_btnBa_2.GetSafeHwnd() != NULL) 
	{
		m_btnBa_2.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_2.Width(), m_btnBa_2.Height(), SWP_NOZORDER);
	}
	if(m_btnBa_3.GetSafeHwnd() != NULL) 
	{
		m_btnBa_3.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_3.Width(), m_btnBa_3.Height(), SWP_NOZORDER);
	}
	if(m_btnBa_4.GetSafeHwnd() != NULL) 
	{
		m_btnBa_4.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_4.Width(), m_btnBa_4.Height(), SWP_NOZORDER);
	}
	if(m_btnBa_5.GetSafeHwnd() != NULL) 
	{
		m_btnBa_5.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_5.Width(), m_btnBa_5.Height(), SWP_NOZORDER);
	}
	if(m_btnBa_6.GetSafeHwnd() != NULL) 
	{
		m_btnBa_6.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_6.Width(), m_btnBa_6.Height(), SWP_NOZORDER);
	}
	if(m_btnBa_7.GetSafeHwnd() != NULL) 
	{
		m_btnBa_7.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_7.Width(), m_btnBa_7.Height(), SWP_NOZORDER);
	}
	if(m_btnBa_8.GetSafeHwnd() != NULL) 
	{
		m_btnBa_8.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_8.Width(), m_btnBa_8.Height(), SWP_NOZORDER);
	}
	if(m_btnBa_9.GetSafeHwnd() != NULL) 
	{
		m_btnBa_9.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y+ haoma_btn_row_height*2, m_btnBa_9.Width(), m_btnBa_9.Height(), SWP_NOZORDER);
	}

	if(m_btnJiu_0.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_0.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_0.Width(), m_btnJiu_0.Height(), SWP_NOZORDER);
	}
	if(m_btnJiu_1.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_1.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_1.Width(), m_btnJiu_1.Height(), SWP_NOZORDER);
	}
	if(m_btnJiu_2.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_2.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_2.Width(), m_btnJiu_2.Height(), SWP_NOZORDER);
	}
	if(m_btnJiu_3.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_3.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_3.Width(), m_btnJiu_3.Height(), SWP_NOZORDER);
	}
	if(m_btnJiu_4.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_4.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_4.Width(), m_btnJiu_4.Height(), SWP_NOZORDER);
	}
	if(m_btnJiu_5.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_5.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_5.Width(), m_btnJiu_5.Height(), SWP_NOZORDER);
	}
	if(m_btnJiu_6.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_6.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_6.Width(), m_btnJiu_6.Height(), SWP_NOZORDER);
	}
	if(m_btnJiu_7.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_7.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_7.Width(), m_btnJiu_7.Height(), SWP_NOZORDER);
	}
	if(m_btnJiu_8.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_8.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_8.Width(), m_btnJiu_8.Height(), SWP_NOZORDER);
	}
	if(m_btnJiu_9.GetSafeHwnd() != NULL) 
	{
		m_btnJiu_9.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y+ haoma_btn_row_height*3, m_btnJiu_9.Width(), m_btnJiu_9.Height(), SWP_NOZORDER);
	}

	if(m_btnShi10_0.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_0.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_0.Width(), m_btnShi10_0.Height(), SWP_NOZORDER);
	}
	if(m_btnShi10_1.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_1.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_1.Width(), m_btnShi10_1.Height(), SWP_NOZORDER);
	}
	if(m_btnShi10_2.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_2.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_2.Width(), m_btnShi10_2.Height(), SWP_NOZORDER);
	}
	if(m_btnShi10_3.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_3.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_3.Width(), m_btnShi10_3.Height(), SWP_NOZORDER);
	}
	if(m_btnShi10_4.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_4.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_4.Width(), m_btnShi10_4.Height(), SWP_NOZORDER);
	}
	if(m_btnShi10_5.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_5.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_5.Width(), m_btnShi10_5.Height(), SWP_NOZORDER);
	}
	if(m_btnShi10_6.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_6.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_6.Width(), m_btnShi10_6.Height(), SWP_NOZORDER);
	}
	if(m_btnShi10_7.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_7.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_7.Width(), m_btnShi10_7.Height(), SWP_NOZORDER);
	}
	if(m_btnShi10_8.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_8.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_8.Width(), m_btnShi10_8.Height(), SWP_NOZORDER);
	}
	if(m_btnShi10_9.GetSafeHwnd() != NULL) 
	{
		m_btnShi10_9.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y+ haoma_btn_row_height*4, m_btnShi10_9.Width(), m_btnShi10_9.Height(), SWP_NOZORDER);
	}
////////////////////////////////////////////////////////////////
	//��С��˫
	if(m_btnDaGUAN.GetSafeHwnd() != NULL) 
	{
		m_btnDaGUAN.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y, m_btnDaGUAN.Width(), m_btnDaGUAN.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoGUAN.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoGUAN.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y , m_btnXiaoGUAN.Width(), m_btnXiaoGUAN.Height(), SWP_NOZORDER);
	}
	if(m_btnDanGUAN.GetSafeHwnd() != NULL) 
	{
		m_btnDanGUAN.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y , m_btnDanGUAN.Width(), m_btnDanGUAN.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangGUAN.GetSafeHwnd() != NULL) 
	{
		m_btnShuangGUAN.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y , m_btnShuangGUAN.Width(), m_btnShuangGUAN.Height(), SWP_NOZORDER);
	}
	//��С��˫
	if(m_btnDaYA.GetSafeHwnd() != NULL) 
	{
		m_btnDaYA.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height, m_btnDaYA.Width(), m_btnDaYA.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoYA.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoYA.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height, m_btnXiaoYA.Width(), m_btnXiaoYA.Height(), SWP_NOZORDER);
	}
	if(m_btnDanYA.GetSafeHwnd() != NULL) 
	{
		m_btnDanYA.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height, m_btnDanYA.Width(), m_btnDanYA.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangYA.GetSafeHwnd() != NULL) 
	{
		m_btnShuangYA.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height, m_btnShuangYA.Width(), m_btnShuangYA.Height(), SWP_NOZORDER);
	}

	//��С��˫
	if(m_btnDaSAN.GetSafeHwnd() != NULL) 
	{
		m_btnDaSAN.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*2, m_btnDaSAN.Width(), m_btnDaSAN.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoSAN.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoSAN.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*2, m_btnXiaoSAN.Width(), m_btnXiaoSAN.Height(), SWP_NOZORDER);
	}
	if(m_btnDanSAN.GetSafeHwnd() != NULL) 
	{
		m_btnDanSAN.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*2, m_btnDanSAN.Width(), m_btnDanSAN.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangSAN.GetSafeHwnd() != NULL) 
	{
		m_btnShuangSAN.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*2, m_btnShuangSAN.Width(), m_btnShuangSAN.Height(), SWP_NOZORDER);
	}
	//��С��˫
	if(m_btnDaSI.GetSafeHwnd() != NULL) 
	{
		m_btnDaSI.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*3, m_btnDaSI.Width(), m_btnDaSI.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoSI.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoSI.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*3, m_btnXiaoSI.Width(), m_btnXiaoSI.Height(), SWP_NOZORDER);
	}
	if(m_btnDanSI.GetSafeHwnd() != NULL) 
	{
		m_btnDanSI.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*3, m_btnDanSI.Width(), m_btnDanSI.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangSI.GetSafeHwnd() != NULL) 
	{
		m_btnShuangSI.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*3, m_btnShuangSI.Width(), m_btnShuangSI.Height(), SWP_NOZORDER);
	}

	//��С��˫
	if(m_btnDaWU.GetSafeHwnd() != NULL) 
	{
		m_btnDaWU.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*4, m_btnDaWU.Width(), m_btnDaWU.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoWU.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoWU.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*4, m_btnXiaoWU.Width(), m_btnXiaoWU.Height(), SWP_NOZORDER);
	}
	if(m_btnDanWU.GetSafeHwnd() != NULL) 
	{
		m_btnDanWU.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*4, m_btnDanWU.Width(), m_btnDanWU.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangWU.GetSafeHwnd() != NULL) 
	{
		m_btnShuangWU.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*4, m_btnShuangWU.Width(), m_btnShuangWU.Height(), SWP_NOZORDER);
	}

	//��С��˫
	if(m_btnDaLIU.GetSafeHwnd() != NULL) 
	{
		m_btnDaLIU.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y, m_btnDaLIU.Width(), m_btnDaLIU.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoLIU.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoLIU.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y , m_btnXiaoLIU.Width(), m_btnXiaoLIU.Height(), SWP_NOZORDER);
	}
	if(m_btnDanLIU.GetSafeHwnd() != NULL) 
	{
		m_btnDanLIU.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y , m_btnDanLIU.Width(), m_btnDanLIU.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangLIU.GetSafeHwnd() != NULL) 
	{
		m_btnShuangLIU.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y , m_btnShuangLIU.Width(), m_btnShuangLIU.Height(), SWP_NOZORDER);
	}
	//��С��˫
	if(m_btnDaQI.GetSafeHwnd() != NULL) 
	{
		m_btnDaQI.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y + haoma_btn_row_height, m_btnDaQI.Width(), m_btnDaQI.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoQI.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoQI.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height, m_btnXiaoQI.Width(), m_btnXiaoQI.Height(), SWP_NOZORDER);
	}
	if(m_btnDanQI.GetSafeHwnd() != NULL) 
	{
		m_btnDanQI.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height, m_btnDanQI.Width(), m_btnDanQI.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangQI.GetSafeHwnd() != NULL) 
	{
		m_btnShuangQI.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height, m_btnShuangQI.Width(), m_btnShuangQI.Height(), SWP_NOZORDER);
	}

	//��С��˫
	if(m_btnDaBA.GetSafeHwnd() != NULL) 
	{
		m_btnDaBA.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y + haoma_btn_row_height*2, m_btnDaBA.Width(), m_btnDaBA.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoBA.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoBA.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*2, m_btnXiaoBA.Width(), m_btnXiaoBA.Height(), SWP_NOZORDER);
	}
	if(m_btnDanBA.GetSafeHwnd() != NULL) 
	{
		m_btnDanBA.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*2, m_btnDanBA.Width(), m_btnDanBA.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangBA.GetSafeHwnd() != NULL) 
	{
		m_btnShuangBA.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*2, m_btnShuangBA.Width(), m_btnShuangBA.Height(), SWP_NOZORDER);
	}
	//��С��˫
	if(m_btnDaJIU.GetSafeHwnd() != NULL) 
	{
		m_btnDaJIU.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y + haoma_btn_row_height*3, m_btnDaJIU.Width(), m_btnDaJIU.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoJIU.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoJIU.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*3, m_btnXiaoJIU.Width(), m_btnXiaoJIU.Height(), SWP_NOZORDER);
	}
	if(m_btnDanJIU.GetSafeHwnd() != NULL) 
	{
		m_btnDanJIU.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*3, m_btnDanJIU.Width(), m_btnDanJIU.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangJIU.GetSafeHwnd() != NULL) 
	{
		m_btnShuangJIU.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*3, m_btnShuangJIU.Width(), m_btnShuangJIU.Height(), SWP_NOZORDER);
	}

	//��С��˫
	if(m_btnDaShi.GetSafeHwnd() != NULL) 
	{
		m_btnDaShi.SetWindowPos(NULL, second_haoma_btn_x, haoma_btn_y + haoma_btn_row_height*4, m_btnDaShi.Width(), m_btnDaShi.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoShi.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoShi.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*4, m_btnXiaoShi.Width(), m_btnXiaoShi.Height(), SWP_NOZORDER);
	}
	if(m_btnDanShi.GetSafeHwnd() != NULL) 
	{
		m_btnDanShi.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*4, m_btnDanShi.Width(), m_btnDanShi.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangShi.GetSafeHwnd() != NULL) 
	{
		m_btnShuangShi.SetWindowPos(NULL, second_haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*4, m_btnShuangShi.Width(), m_btnShuangShi.Height(), SWP_NOZORDER);
	}
	int nBtnWanQuanCol = 40;
	if (m_btnWanQuan.GetSafeHwnd() != NULL) 
	{
		m_btnWanQuan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*10+40, haoma_btn_y, m_btnWanQuan.Width(), m_btnWanQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnWanDa.GetSafeHwnd() != NULL) 
	{
		m_btnWanDa.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*11+40, haoma_btn_y, m_btnWanDa.Width(), m_btnWanDa.Height(), SWP_NOZORDER);
	}
	if (m_btnWanXiao.GetSafeHwnd() != NULL) 
	{
		m_btnWanXiao.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*12+40, haoma_btn_y, m_btnWanXiao.Width(), m_btnWanXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnWanDan.GetSafeHwnd() != NULL) 
	{
		m_btnWanDan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*13+47, haoma_btn_y, m_btnWanDan.Width(), m_btnWanDan.Height(), SWP_NOZORDER);
	}
	if (m_btnWanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnWanShuang.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*14+47, haoma_btn_y, m_btnWanShuang.Width(), m_btnWanShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnWanQing.GetSafeHwnd() != NULL) 
	{
		m_btnWanQing.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*15+47, haoma_btn_y, m_btnWanQing.Width(), m_btnWanQing.Height(), SWP_NOZORDER);
	}

	if (m_btnQianQuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianQuan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*10+40, haoma_btn_y + haoma_btn_row_height, m_btnQianQuan.Width(), m_btnQianQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnQianDa.GetSafeHwnd() != NULL) 
	{
		m_btnQianDa.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*11+40, haoma_btn_y + haoma_btn_row_height, m_btnQianDa.Width(), m_btnQianDa.Height(), SWP_NOZORDER);
	}
	if (m_btnQianXiao.GetSafeHwnd() != NULL) 
	{
		m_btnQianXiao.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*12+40, haoma_btn_y + haoma_btn_row_height, m_btnQianXiao.Width(), m_btnQianXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnQianDan.GetSafeHwnd() != NULL) 
	{
		m_btnQianDan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*13+47, haoma_btn_y + haoma_btn_row_height, m_btnQianDan.Width(), m_btnQianDan.Height(), SWP_NOZORDER);
	}
	if (m_btnQianShuang.GetSafeHwnd() != NULL) 
	{
		m_btnQianShuang.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*14+40+7, haoma_btn_y + haoma_btn_row_height, m_btnQianShuang.Width(), m_btnQianShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnQianQing.GetSafeHwnd() != NULL) 
	{
		m_btnQianQing.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*15+40+7, haoma_btn_y + haoma_btn_row_height, m_btnQianQing.Width(), m_btnQianQing.Height(), SWP_NOZORDER);
	}

	if (m_btnBaiQuan.GetSafeHwnd() != NULL) 
	{
		m_btnBaiQuan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*10+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiQuan.Width(), m_btnBaiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiDa.GetSafeHwnd() != NULL) 
	{
		m_btnBaiDa.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*11+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiDa.Width(), m_btnBaiDa.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiXiao.GetSafeHwnd() != NULL) 
	{
		m_btnBaiXiao.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*12+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiXiao.Width(), m_btnBaiXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiDan.GetSafeHwnd() != NULL) 
	{
		m_btnBaiDan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*13+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiDan.Width(), m_btnBaiDan.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiShuang.GetSafeHwnd() != NULL) 
	{
		m_btnBaiShuang.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*14+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiShuang.Width(), m_btnBaiShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiQing.GetSafeHwnd() != NULL) 
	{
		m_btnBaiQing.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*15+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiQing.Width(), m_btnBaiQing.Height(), SWP_NOZORDER);
	}
	if (m_btnShiQuan.GetSafeHwnd() != NULL) 
	{
		m_btnShiQuan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*10+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiQuan.Width(), m_btnShiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnShiDa.GetSafeHwnd() != NULL) 
	{
		m_btnShiDa.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*11+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiDa.Width(), m_btnShiDa.Height(), SWP_NOZORDER);
	}
	if (m_btnShiXiao.GetSafeHwnd() != NULL) 
	{
		m_btnShiXiao.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*12+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiXiao.Width(), m_btnShiXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnShiDan.GetSafeHwnd() != NULL) 
	{
		m_btnShiDan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*13+20*2+7, haoma_btn_y + haoma_btn_row_height*3, m_btnShiDan.Width(), m_btnShiDan.Height(), SWP_NOZORDER);
	}
	if (m_btnShiShuang.GetSafeHwnd() != NULL) 
	{
		m_btnShiShuang.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*14+20*2+7, haoma_btn_y + haoma_btn_row_height*3, m_btnShiShuang.Width(), m_btnShiShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnShiQing.GetSafeHwnd() != NULL) 
	{
		m_btnShiQing.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*15+20*2+7, haoma_btn_y + haoma_btn_row_height*3, m_btnShiQing.Width(), m_btnShiQing.Height(), SWP_NOZORDER);
	}

	if (m_btnGeQuan.GetSafeHwnd() != NULL) 
	{
		m_btnGeQuan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*10+20*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGeQuan.Width(), m_btnGeQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnGeDa.GetSafeHwnd() != NULL) 
	{
		m_btnGeDa.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*11+20*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGeDa.Width(), m_btnGeDa.Height(), SWP_NOZORDER);
	}
	if (m_btnGeXiao.GetSafeHwnd() != NULL) 
	{
		m_btnGeXiao.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*12+20*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGeXiao.Width(), m_btnGeXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnGeDan.GetSafeHwnd() != NULL) 
	{
		m_btnGeDan.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*13+20*2+7, haoma_btn_y + haoma_btn_row_height*4, m_btnGeDan.Width(), m_btnGeDan.Height(), SWP_NOZORDER);
	}
	if (m_btnGeShuang.GetSafeHwnd() != NULL) 
	{
		m_btnGeShuang.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*14+20*2+7, haoma_btn_y + haoma_btn_row_height*4, m_btnGeShuang.Width(), m_btnGeShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnGeQing.GetSafeHwnd() != NULL) 
	{
		m_btnGeQing.SetWindowPos(NULL, haoma_btn_x + nBtnWanQuanCol*15+20*2+7, haoma_btn_y + haoma_btn_row_height*4, m_btnGeQing.Width(), m_btnGeQing.Height(), SWP_NOZORDER);
	}



}

void CBeiJingPK10::ShowWanNumsQ()
{
	m_btnWanQuan.ShowWindow(SW_SHOW);
	m_btnWanDa.ShowWindow(SW_SHOW);
	m_btnWanXiao.ShowWindow(SW_SHOW);
	m_btnWanDan.ShowWindow(SW_SHOW);
	m_btnWanShuang.ShowWindow(SW_SHOW);
	m_btnWanQing.ShowWindow(SW_SHOW);
}
void CBeiJingPK10::ShowQianNumsQ()
{
	m_btnQianQuan.ShowWindow(SW_SHOW);
	m_btnQianDa.ShowWindow(SW_SHOW);
	m_btnQianXiao.ShowWindow(SW_SHOW);
	m_btnQianDan.ShowWindow(SW_SHOW);
	m_btnQianShuang.ShowWindow(SW_SHOW);
	m_btnQianQing.ShowWindow(SW_SHOW);
}

void CBeiJingPK10::ShowBaiNumsQ()
{
	m_btnBaiQuan.ShowWindow(SW_SHOW);
	m_btnBaiDa.ShowWindow(SW_SHOW);
	m_btnBaiXiao.ShowWindow(SW_SHOW);
	m_btnBaiDan.ShowWindow(SW_SHOW);
	m_btnBaiShuang.ShowWindow(SW_SHOW);
	m_btnBaiQing.ShowWindow(SW_SHOW);
}

void CBeiJingPK10::ShowShiNumsQ()
{
	m_btnShiQuan.ShowWindow(SW_SHOW);
	m_btnShiDa.ShowWindow(SW_SHOW);
	m_btnShiXiao.ShowWindow(SW_SHOW);
	m_btnShiDan.ShowWindow(SW_SHOW);
	m_btnShiShuang.ShowWindow(SW_SHOW);
	m_btnShiQing.ShowWindow(SW_SHOW);
}

void CBeiJingPK10::ShowGeNumsQ()
{
	m_btnGeQuan.ShowWindow(SW_SHOW);
	m_btnGeDa.ShowWindow(SW_SHOW);
	m_btnGeXiao.ShowWindow(SW_SHOW);
	m_btnGeDan.ShowWindow(SW_SHOW);
	m_btnGeShuang.ShowWindow(SW_SHOW);
	m_btnGeQing.ShowWindow(SW_SHOW);
}

void CBeiJingPK10::HideWanNumsQ()
{
	m_btnWanQuan.ShowWindow(SW_HIDE);
	m_btnWanDa.ShowWindow(SW_HIDE);
	m_btnWanXiao.ShowWindow(SW_HIDE);
	m_btnWanDan.ShowWindow(SW_HIDE);
	m_btnWanShuang.ShowWindow(SW_HIDE);
	m_btnWanQing.ShowWindow(SW_HIDE);
}

void CBeiJingPK10::HideQianNumsQ()
{
	m_btnQianQuan.ShowWindow(SW_HIDE);
	m_btnQianDa.ShowWindow(SW_HIDE);
	m_btnQianXiao.ShowWindow(SW_HIDE);
	m_btnQianDan.ShowWindow(SW_HIDE);
	m_btnQianShuang.ShowWindow(SW_HIDE);
	m_btnQianQing.ShowWindow(SW_HIDE);
}

void CBeiJingPK10::HideBaiNumsQ()
{	

	m_btnBaiQuan.ShowWindow(SW_HIDE);
	m_btnBaiDa.ShowWindow(SW_HIDE);
	m_btnBaiXiao.ShowWindow(SW_HIDE);
	m_btnBaiDan.ShowWindow(SW_HIDE);
	m_btnBaiShuang.ShowWindow(SW_HIDE);
	m_btnBaiQing.ShowWindow(SW_HIDE);
}

void CBeiJingPK10::HideShiNumsQ()
{
	m_btnShiQuan.ShowWindow(SW_HIDE);
	m_btnShiDa.ShowWindow(SW_HIDE);
	m_btnShiXiao.ShowWindow(SW_HIDE);
	m_btnShiDan.ShowWindow(SW_HIDE);
	m_btnShiShuang.ShowWindow(SW_HIDE);
	m_btnShiQing.ShowWindow(SW_HIDE);
}

void CBeiJingPK10::HideGeNumsQ()
{
	m_btnGeQuan.ShowWindow(SW_HIDE);
	m_btnGeDa.ShowWindow(SW_HIDE);
	m_btnGeXiao.ShowWindow(SW_HIDE);
	m_btnGeDan.ShowWindow(SW_HIDE);
	m_btnGeShuang.ShowWindow(SW_HIDE);
	m_btnGeQing.ShowWindow(SW_HIDE);
}

//��ť������ɾ�������㣬��Ͷ��Ͷע��׷��
void CBeiJingPK10::AdjustNumView()
{
	if(m_btnBeiTouJia.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJia.SetWindowPos(NULL,edit_beitou_x+edit_beitou_width , edit_beitou_y-3, m_btnBeiTouJia.Width(), m_btnBeiTouJia.Height(), SWP_NOZORDER);
	}
	if(m_btnBeiTouJian.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJian.SetWindowPos(NULL,edit_beitou_x-m_btnBeiTouJian.Width() , edit_beitou_y-3, m_btnBeiTouJian.Width(), m_btnBeiTouJian.Height(), SWP_NOZORDER);
	}

	if(m_btnDelSel.GetSafeHwnd() != NULL) 
	{
		m_btnDelSel.SetWindowPos(NULL, btn_del_sel_x, btn_del_sel_y, m_btnDelSel.Width(), m_btnDelSel.Height(), SWP_NOZORDER);
		m_btnDelSel.ShowWindow(SW_HIDE);
	}

	if(m_btnClsList.GetSafeHwnd() != NULL) 
	{
		m_btnClsList.SetWindowPos(NULL, btn_cls_list_x, btn_cls_list_y, m_btnClsList.Width(), m_btnClsList.Height(), SWP_NOZORDER);
	}

	if (m_listChqSSC.GetSafeHwnd() != NULL) 
	{
		m_listChqSSC.SetWindowPos(NULL, list_haomao_x, list_haomao_y, list_haomao_width, list_haomao_height, SWP_NOZORDER);
	}

	if(m_editBeiTou.GetSafeHwnd() != NULL) 
	{
		m_editBeiTou.SetWindowPos(NULL, edit_beitou_x, edit_beitou_y, edit_beitou_width, edit_beitou_height, SWP_NOZORDER);
	}

	if(m_btnTouZhu.GetSafeHwnd() != NULL) 
	{
		m_btnTouZhu.SetWindowPos(NULL, btn_touzhu_x, btn_touzhu_y, m_btnTouZhu.Width(), m_btnTouZhu.Height(), SWP_NOZORDER);
	}
	if(m_btnTouZhuOnekey.GetSafeHwnd() != NULL) 
	{
		m_btnTouZhuOnekey.SetWindowPos(NULL, btn_touzhu_onekey_x, btn_touzhu_onekey_y, m_btnTouZhuOnekey.Width(), m_btnTouZhuOnekey.Height(), SWP_NOZORDER);
	}


	if(m_btnZhuiHao.GetSafeHwnd() != NULL) 
	{
		m_btnZhuiHao.SetWindowPos(NULL, btn_zhuihao_x, btn_zhuihao_y, m_btnZhuiHao.Width(), m_btnZhuiHao.Height(), SWP_NOZORDER);
	}
	if(m_btnMoreRecord.GetSafeHwnd() != NULL) 
	{
		m_btnMoreRecord.SetWindowPos(NULL, btn_more_x, btn_more_y, m_btnMoreRecord.Width()/4, m_btnMoreRecord.Height(), SWP_NOZORDER);
		m_btnMoreRecord.ShowWindow(SW_HIDE);
	}
// 	if(m_DlgOpenRecord.GetSafeHwnd() != NULL)
// 	{
// 		CRect rcRect;
// 		GetClientRect(rcRect);
// 		ClientToScreen(rcRect);
// 		m_DlgOpenRecord.SetWindowPos(NULL, rcRect.left+475, rcRect.top+25,252, 560, SWP_NOZORDER);
// 
// 	}

}

//��ʽ����
void CBeiJingPK10::AdjustDanShi()
{
	//if (m_staticDanShi.GetSafeHwnd() != NULL) 
	//{
	//	m_staticDanShi.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y, m_staticDanShi.Width(), m_staticDanShi.Height(), SWP_NOZORDER);
	//}

	//if (m_editDanShiHaoMa.GetSafeHwnd() != NULL) 
	//{
	//	m_editDanShiHaoMa.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
	//}
	if (m_richDanshiHaoma.GetSafeHwnd() != NULL) 
	{
		m_richDanshiHaoma.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
	}
}

//��ע��ť
void CBeiJingPK10::AdjustAdd()
{
	if (m_btnChqAdd.GetSafeHwnd() != NULL) 
	{
		m_btnChqAdd.SetWindowPos(NULL, btn_chq_add_x, btn_chq_add_y, m_btnChqAdd.Width(), m_btnChqAdd.Height(), SWP_NOZORDER);
	}

	if (m_btnLock.GetSafeHwnd() != NULL) 
	{
		m_btnLock.SetWindowPos(NULL, btn_lock_x, btn_lock_y, m_btnLock.Width(), m_btnLock.Height(), SWP_NOZORDER);
	}
	if(m_btnYuan.GetSafeHwnd() != NULL)
	{
		m_btnYuan.SetWindowPos(NULL,btn_yuan_x,btn_yuan_y,m_btnYuan.Width(),m_btnYuan.Height(),SWP_NOZORDER);
	}
	if(m_btnJiao.GetSafeHwnd() != NULL)
	{
		m_btnJiao.SetWindowPos(NULL,btn_jiao_x,btn_jiao_y,m_btnJiao.Width(),m_btnJiao.Height(),SWP_NOZORDER);
	}
	if(m_btnFen.GetSafeHwnd() != NULL)
	{
		m_btnFen.SetWindowPos(NULL,btn_fen_x,btn_fen_y,m_btnFen.Width(),m_btnFen.Height(),SWP_NOZORDER);
	}
	if(m_btnLi.GetSafeHwnd() != NULL)
	{
		m_btnLi.SetWindowPos(NULL,btn_li_x,btn_li_y,m_btnLi.Width(),m_btnLi.Height(),SWP_NOZORDER);
	}
	if(m_btWxzxTip.GetSafeHwnd() != NULL)
	{
		m_btWxzxTip.SetWindowPos(NULL,keyboardinput_x,mouseinput_y-30,m_btWxzxTip.Width(),m_btWxzxTip.Height(),SWP_NOZORDER);
	}
	if(m_btnQushi.GetSafeHwnd() != NULL) 
	{
		m_btnQushi.SetWindowPos(NULL, btn_more_x-40, btn_more_y, m_btnMoreRecord.Width(), m_btnMoreRecord.Height(), SWP_NOZORDER);
	}

}

void CBeiJingPK10::HideWanNums()
{
	m_btnWan_0.ShowWindow(SW_HIDE);
	m_btnWan_1.ShowWindow(SW_HIDE);
	m_btnWan_2.ShowWindow(SW_HIDE);
	m_btnWan_3.ShowWindow(SW_HIDE);
	m_btnWan_4.ShowWindow(SW_HIDE);
	m_btnWan_5.ShowWindow(SW_HIDE);
	m_btnWan_6.ShowWindow(SW_HIDE);
	m_btnWan_7.ShowWindow(SW_HIDE);
	m_btnWan_8.ShowWindow(SW_HIDE);
	m_btnWan_9.ShowWindow(SW_HIDE);

	m_staticWan.ShowWindow(SW_HIDE);

}

void CBeiJingPK10::HideQianNums()
{
	m_btnQian_0.ShowWindow(SW_HIDE);
	m_btnQian_1.ShowWindow(SW_HIDE);
	m_btnQian_2.ShowWindow(SW_HIDE);
	m_btnQian_3.ShowWindow(SW_HIDE);
	m_btnQian_4.ShowWindow(SW_HIDE);
	m_btnQian_5.ShowWindow(SW_HIDE);
	m_btnQian_6.ShowWindow(SW_HIDE);
	m_btnQian_7.ShowWindow(SW_HIDE);
	m_btnQian_8.ShowWindow(SW_HIDE);
	m_btnQian_9.ShowWindow(SW_HIDE);

	m_staticQian.ShowWindow(SW_HIDE);

}

void CBeiJingPK10::HideBaiNums()
{	
	m_btnBai_0.ShowWindow(SW_HIDE);
	m_btnBai_1.ShowWindow(SW_HIDE);
	m_btnBai_2.ShowWindow(SW_HIDE);
	m_btnBai_3.ShowWindow(SW_HIDE);
	m_btnBai_4.ShowWindow(SW_HIDE);
	m_btnBai_5.ShowWindow(SW_HIDE);
	m_btnBai_6.ShowWindow(SW_HIDE);
	m_btnBai_7.ShowWindow(SW_HIDE);
	m_btnBai_8.ShowWindow(SW_HIDE);
	m_btnBai_9.ShowWindow(SW_HIDE);

	m_staticBai.ShowWindow(SW_HIDE);

}

void CBeiJingPK10::HideShiNums()
{
	m_btnShi_0.ShowWindow(SW_HIDE);
	m_btnShi_1.ShowWindow(SW_HIDE);
	m_btnShi_2.ShowWindow(SW_HIDE);
	m_btnShi_3.ShowWindow(SW_HIDE);
	m_btnShi_4.ShowWindow(SW_HIDE);
	m_btnShi_5.ShowWindow(SW_HIDE);
	m_btnShi_6.ShowWindow(SW_HIDE);
	m_btnShi_7.ShowWindow(SW_HIDE);
	m_btnShi_8.ShowWindow(SW_HIDE);
	m_btnShi_9.ShowWindow(SW_HIDE);

	m_staticShi.ShowWindow(SW_HIDE);

}

void CBeiJingPK10::HideGeNums()
{
	m_btnGe_0.ShowWindow(SW_HIDE);
	m_btnGe_1.ShowWindow(SW_HIDE);
	m_btnGe_2.ShowWindow(SW_HIDE);
	m_btnGe_3.ShowWindow(SW_HIDE);
	m_btnGe_4.ShowWindow(SW_HIDE);
	m_btnGe_5.ShowWindow(SW_HIDE);
	m_btnGe_6.ShowWindow(SW_HIDE);
	m_btnGe_7.ShowWindow(SW_HIDE);
	m_btnGe_8.ShowWindow(SW_HIDE);
	m_btnGe_9.ShowWindow(SW_HIDE);

	m_staticGe.ShowWindow(SW_HIDE);

}
void CBeiJingPK10::HideLiuNums()
{
	m_btnLiu_0.ShowWindow(SW_HIDE);
	m_btnLiu_1.ShowWindow(SW_HIDE);
	m_btnLiu_2.ShowWindow(SW_HIDE);
	m_btnLiu_3.ShowWindow(SW_HIDE);
	m_btnLiu_4.ShowWindow(SW_HIDE);
	m_btnLiu_5.ShowWindow(SW_HIDE);
	m_btnLiu_6.ShowWindow(SW_HIDE);
	m_btnLiu_7.ShowWindow(SW_HIDE);
	m_btnLiu_8.ShowWindow(SW_HIDE);
	m_btnLiu_9.ShowWindow(SW_HIDE);

	m_staticLiu.ShowWindow(SW_HIDE);

}
void CBeiJingPK10::HideQiNums()
{
	m_btnQi_0.ShowWindow(SW_HIDE);
	m_btnQi_1.ShowWindow(SW_HIDE);
	m_btnQi_2.ShowWindow(SW_HIDE);
	m_btnQi_3.ShowWindow(SW_HIDE);
	m_btnQi_4.ShowWindow(SW_HIDE);
	m_btnQi_5.ShowWindow(SW_HIDE);
	m_btnQi_6.ShowWindow(SW_HIDE);
	m_btnQi_7.ShowWindow(SW_HIDE);
	m_btnQi_8.ShowWindow(SW_HIDE);
	m_btnQi_9.ShowWindow(SW_HIDE);

	m_staticQi.ShowWindow(SW_HIDE);

}
void CBeiJingPK10::HideBaNums()
{
	m_btnBa_0.ShowWindow(SW_HIDE);
	m_btnBa_1.ShowWindow(SW_HIDE);
	m_btnBa_2.ShowWindow(SW_HIDE);
	m_btnBa_3.ShowWindow(SW_HIDE);
	m_btnBa_4.ShowWindow(SW_HIDE);
	m_btnBa_5.ShowWindow(SW_HIDE);
	m_btnBa_6.ShowWindow(SW_HIDE);
	m_btnBa_7.ShowWindow(SW_HIDE);
	m_btnBa_8.ShowWindow(SW_HIDE);
	m_btnBa_9.ShowWindow(SW_HIDE);

	m_staticBa.ShowWindow(SW_HIDE);

}
void CBeiJingPK10::HideJiuNums()
{
	m_btnJiu_0.ShowWindow(SW_HIDE);
	m_btnJiu_1.ShowWindow(SW_HIDE);
	m_btnJiu_2.ShowWindow(SW_HIDE);
	m_btnJiu_3.ShowWindow(SW_HIDE);
	m_btnJiu_4.ShowWindow(SW_HIDE);
	m_btnJiu_5.ShowWindow(SW_HIDE);
	m_btnJiu_6.ShowWindow(SW_HIDE);
	m_btnJiu_7.ShowWindow(SW_HIDE);
	m_btnJiu_8.ShowWindow(SW_HIDE);
	m_btnJiu_9.ShowWindow(SW_HIDE);

	m_staticJiu.ShowWindow(SW_HIDE);

}
void CBeiJingPK10::HideShi10Nums()
{
	m_btnShi10_0.ShowWindow(SW_HIDE);
	m_btnShi10_1.ShowWindow(SW_HIDE);
	m_btnShi10_2.ShowWindow(SW_HIDE);
	m_btnShi10_3.ShowWindow(SW_HIDE);
	m_btnShi10_4.ShowWindow(SW_HIDE);
	m_btnShi10_5.ShowWindow(SW_HIDE);
	m_btnShi10_6.ShowWindow(SW_HIDE);
	m_btnShi10_7.ShowWindow(SW_HIDE);
	m_btnShi10_8.ShowWindow(SW_HIDE);
	m_btnShi10_9.ShowWindow(SW_HIDE);

	m_staticShi10.ShowWindow(SW_HIDE);

}

void CBeiJingPK10::ShowWanNums()
{
	m_btnWan_0.ShowWindow(SW_SHOW);
	m_btnWan_1.ShowWindow(SW_SHOW);
	m_btnWan_2.ShowWindow(SW_SHOW);
	m_btnWan_3.ShowWindow(SW_SHOW);
	m_btnWan_4.ShowWindow(SW_SHOW);
	m_btnWan_5.ShowWindow(SW_SHOW);
	m_btnWan_6.ShowWindow(SW_SHOW);
	m_btnWan_7.ShowWindow(SW_SHOW);
	m_btnWan_8.ShowWindow(SW_SHOW);
	m_btnWan_9.ShowWindow(SW_SHOW);

	m_staticWan.ShowWindow(SW_SHOW);

}

void CBeiJingPK10::ShowQianNums()
{
	m_btnQian_0.ShowWindow(SW_SHOW);
	m_btnQian_1.ShowWindow(SW_SHOW);
	m_btnQian_2.ShowWindow(SW_SHOW);
	m_btnQian_3.ShowWindow(SW_SHOW);
	m_btnQian_4.ShowWindow(SW_SHOW);
	m_btnQian_5.ShowWindow(SW_SHOW);
	m_btnQian_6.ShowWindow(SW_SHOW);
	m_btnQian_7.ShowWindow(SW_SHOW);
	m_btnQian_8.ShowWindow(SW_SHOW);
	m_btnQian_9.ShowWindow(SW_SHOW);

	m_staticQian.ShowWindow(SW_SHOW);

}

void CBeiJingPK10::ShowBaiNums()
{
	m_btnBai_0.ShowWindow(SW_SHOW);
	m_btnBai_1.ShowWindow(SW_SHOW);
	m_btnBai_2.ShowWindow(SW_SHOW);
	m_btnBai_3.ShowWindow(SW_SHOW);
	m_btnBai_4.ShowWindow(SW_SHOW);
	m_btnBai_5.ShowWindow(SW_SHOW);
	m_btnBai_6.ShowWindow(SW_SHOW);
	m_btnBai_7.ShowWindow(SW_SHOW);
	m_btnBai_8.ShowWindow(SW_SHOW);
	m_btnBai_9.ShowWindow(SW_SHOW);

	m_staticBai.ShowWindow(SW_SHOW);

}

void CBeiJingPK10::ShowShiNums()
{
	m_btnShi_0.ShowWindow(SW_SHOW);
	m_btnShi_1.ShowWindow(SW_SHOW);
	m_btnShi_2.ShowWindow(SW_SHOW);
	m_btnShi_3.ShowWindow(SW_SHOW);
	m_btnShi_4.ShowWindow(SW_SHOW);
	m_btnShi_5.ShowWindow(SW_SHOW);
	m_btnShi_6.ShowWindow(SW_SHOW);
	m_btnShi_7.ShowWindow(SW_SHOW);
	m_btnShi_8.ShowWindow(SW_SHOW);
	m_btnShi_9.ShowWindow(SW_SHOW);

	m_staticShi.ShowWindow(SW_SHOW);

}

void CBeiJingPK10::ShowGeNums()
{
	m_btnGe_0.ShowWindow(SW_SHOW);
	m_btnGe_1.ShowWindow(SW_SHOW);
	m_btnGe_2.ShowWindow(SW_SHOW);
	m_btnGe_3.ShowWindow(SW_SHOW);
	m_btnGe_4.ShowWindow(SW_SHOW);
	m_btnGe_5.ShowWindow(SW_SHOW);
	m_btnGe_6.ShowWindow(SW_SHOW);
	m_btnGe_7.ShowWindow(SW_SHOW);
	m_btnGe_8.ShowWindow(SW_SHOW);
	m_btnGe_9.ShowWindow(SW_SHOW);

	m_staticGe.ShowWindow(SW_SHOW);

}
void CBeiJingPK10::ShowLiuNums()
{
	m_btnLiu_0.ShowWindow(SW_SHOW);
	m_btnLiu_1.ShowWindow(SW_SHOW);
	m_btnLiu_2.ShowWindow(SW_SHOW);
	m_btnLiu_3.ShowWindow(SW_SHOW);
	m_btnLiu_4.ShowWindow(SW_SHOW);
	m_btnLiu_5.ShowWindow(SW_SHOW);
	m_btnLiu_6.ShowWindow(SW_SHOW);
	m_btnLiu_7.ShowWindow(SW_SHOW);
	m_btnLiu_8.ShowWindow(SW_SHOW);
	m_btnLiu_9.ShowWindow(SW_SHOW);

	m_staticLiu.ShowWindow(SW_SHOW);

}
void CBeiJingPK10::ShowQiNums()
{
	m_btnQi_0.ShowWindow(SW_SHOW);
	m_btnQi_1.ShowWindow(SW_SHOW);
	m_btnQi_2.ShowWindow(SW_SHOW);
	m_btnQi_3.ShowWindow(SW_SHOW);
	m_btnQi_4.ShowWindow(SW_SHOW);
	m_btnQi_5.ShowWindow(SW_SHOW);
	m_btnQi_6.ShowWindow(SW_SHOW);
	m_btnQi_7.ShowWindow(SW_SHOW);
	m_btnQi_8.ShowWindow(SW_SHOW);
	m_btnQi_9.ShowWindow(SW_SHOW);

	m_staticQi.ShowWindow(SW_SHOW);
}
void CBeiJingPK10::ShowBaNums()
{
	m_btnBa_0.ShowWindow(SW_SHOW);
	m_btnBa_1.ShowWindow(SW_SHOW);
	m_btnBa_2.ShowWindow(SW_SHOW);
	m_btnBa_3.ShowWindow(SW_SHOW);
	m_btnBa_4.ShowWindow(SW_SHOW);
	m_btnBa_5.ShowWindow(SW_SHOW);
	m_btnBa_6.ShowWindow(SW_SHOW);
	m_btnBa_7.ShowWindow(SW_SHOW);
	m_btnBa_8.ShowWindow(SW_SHOW);
	m_btnBa_9.ShowWindow(SW_SHOW);

	m_staticBa.ShowWindow(SW_SHOW);
}
void CBeiJingPK10::ShowJiuNums()
{
	m_btnJiu_0.ShowWindow(SW_SHOW);
	m_btnJiu_1.ShowWindow(SW_SHOW);
	m_btnJiu_2.ShowWindow(SW_SHOW);
	m_btnJiu_3.ShowWindow(SW_SHOW);
	m_btnJiu_4.ShowWindow(SW_SHOW);
	m_btnJiu_5.ShowWindow(SW_SHOW);
	m_btnJiu_6.ShowWindow(SW_SHOW);
	m_btnJiu_7.ShowWindow(SW_SHOW);
	m_btnJiu_8.ShowWindow(SW_SHOW);
	m_btnJiu_9.ShowWindow(SW_SHOW);

	m_staticJiu.ShowWindow(SW_SHOW);
}
void CBeiJingPK10::ShowShi10Nums()
{
	m_btnShi10_0.ShowWindow(SW_SHOW);
	m_btnShi10_1.ShowWindow(SW_SHOW);
	m_btnShi10_2.ShowWindow(SW_SHOW);
	m_btnShi10_3.ShowWindow(SW_SHOW);
	m_btnShi10_4.ShowWindow(SW_SHOW);
	m_btnShi10_5.ShowWindow(SW_SHOW);
	m_btnShi10_6.ShowWindow(SW_SHOW);
	m_btnShi10_7.ShowWindow(SW_SHOW);
	m_btnShi10_8.ShowWindow(SW_SHOW);
	m_btnShi10_9.ShowWindow(SW_SHOW);

	m_staticShi10.ShowWindow(SW_SHOW);
}


void CBeiJingPK10::ShowBuDingWei()
{
	//m_staticBai.ShowWindow(SW_HIDE);
}


void CBeiJingPK10::ShowDaXiaoDanShuang()
{
	m_staticWan.ShowWindow(SW_SHOW);
	m_staticQian.ShowWindow(SW_SHOW);
	m_staticBai.ShowWindow(SW_SHOW);
	m_staticShi.ShowWindow(SW_SHOW);
	m_staticGe.ShowWindow(SW_SHOW);
	m_staticLiu.ShowWindow(SW_SHOW);
	m_staticQi.ShowWindow(SW_SHOW);
	m_staticBa.ShowWindow(SW_SHOW);
	m_staticJiu.ShowWindow(SW_SHOW);
	m_staticShi10.ShowWindow(SW_SHOW);

	m_btnDaGUAN.ShowWindow(SW_SHOW);
	m_btnXiaoGUAN.ShowWindow(SW_SHOW);
	m_btnDanGUAN.ShowWindow(SW_SHOW);
	m_btnShuangGUAN.ShowWindow(SW_SHOW);
	m_btnDaYA.ShowWindow(SW_SHOW);
	m_btnXiaoYA.ShowWindow(SW_SHOW);
	m_btnDanYA.ShowWindow(SW_SHOW);
	m_btnShuangYA.ShowWindow(SW_SHOW);
	m_btnDaSAN.ShowWindow(SW_SHOW);
	m_btnXiaoSAN.ShowWindow(SW_SHOW);
	m_btnDanSAN.ShowWindow(SW_SHOW);
	m_btnShuangSAN.ShowWindow(SW_SHOW);
	m_btnDaSI.ShowWindow(SW_SHOW);
	m_btnXiaoSI.ShowWindow(SW_SHOW);
	m_btnDanSI.ShowWindow(SW_SHOW);
	m_btnShuangSI.ShowWindow(SW_SHOW);
	m_btnDaWU.ShowWindow(SW_SHOW);
	m_btnXiaoWU.ShowWindow(SW_SHOW);
	m_btnDanWU.ShowWindow(SW_SHOW);
	m_btnShuangWU.ShowWindow(SW_SHOW);
	m_btnDaLIU.ShowWindow(SW_SHOW);
	m_btnXiaoLIU.ShowWindow(SW_SHOW);
	m_btnDanLIU.ShowWindow(SW_SHOW);
	m_btnShuangLIU.ShowWindow(SW_SHOW);
	m_btnDaQI.ShowWindow(SW_SHOW);
	m_btnXiaoQI.ShowWindow(SW_SHOW);
	m_btnDanQI.ShowWindow(SW_SHOW);
	m_btnShuangQI.ShowWindow(SW_SHOW);
	m_btnDaBA.ShowWindow(SW_SHOW);
	m_btnXiaoBA.ShowWindow(SW_SHOW);
	m_btnDanBA.ShowWindow(SW_SHOW);
	m_btnShuangBA.ShowWindow(SW_SHOW);
	m_btnDaJIU.ShowWindow(SW_SHOW);
	m_btnXiaoJIU.ShowWindow(SW_SHOW);
	m_btnDanJIU.ShowWindow(SW_SHOW);
	m_btnShuangJIU.ShowWindow(SW_SHOW);
	m_btnDaShi.ShowWindow(SW_SHOW);
	m_btnXiaoShi.ShowWindow(SW_SHOW);
	m_btnDanShi.ShowWindow(SW_SHOW);
	m_btnShuangShi.ShowWindow(SW_SHOW);
}

void CBeiJingPK10::HideDaXiaoDanShuang()
{
	m_staticWan.ShowWindow(SW_HIDE);
	m_staticQian.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticShi.ShowWindow(SW_HIDE);
	m_staticGe.ShowWindow(SW_HIDE);
	m_staticLiu.ShowWindow(SW_HIDE);
	m_staticQi.ShowWindow(SW_HIDE);
	m_staticBa.ShowWindow(SW_HIDE);
	m_staticJiu.ShowWindow(SW_HIDE);
	m_staticShi10.ShowWindow(SW_HIDE);

	m_btnDaGUAN.ShowWindow(SW_HIDE);
	m_btnXiaoGUAN.ShowWindow(SW_HIDE);
	m_btnDanGUAN.ShowWindow(SW_HIDE);
	m_btnShuangGUAN.ShowWindow(SW_HIDE);
	m_btnDaYA.ShowWindow(SW_HIDE);
	m_btnXiaoYA.ShowWindow(SW_HIDE);
	m_btnDanYA.ShowWindow(SW_HIDE);
	m_btnShuangYA.ShowWindow(SW_HIDE);
	m_btnDaSAN.ShowWindow(SW_HIDE);
	m_btnXiaoSAN.ShowWindow(SW_HIDE);
	m_btnDanSAN.ShowWindow(SW_HIDE);
	m_btnShuangSAN.ShowWindow(SW_HIDE);
	m_btnDaSI.ShowWindow(SW_HIDE);
	m_btnXiaoSI.ShowWindow(SW_HIDE);
	m_btnDanSI.ShowWindow(SW_HIDE);
	m_btnShuangSI.ShowWindow(SW_HIDE);
	m_btnDaWU.ShowWindow(SW_HIDE);
	m_btnXiaoWU.ShowWindow(SW_HIDE);
	m_btnDanWU.ShowWindow(SW_HIDE);
	m_btnShuangWU.ShowWindow(SW_HIDE);
	m_btnDaLIU.ShowWindow(SW_HIDE);
	m_btnXiaoLIU.ShowWindow(SW_HIDE);
	m_btnDanLIU.ShowWindow(SW_HIDE);
	m_btnShuangLIU.ShowWindow(SW_HIDE);
	m_btnDaQI.ShowWindow(SW_HIDE);
	m_btnXiaoQI.ShowWindow(SW_HIDE);
	m_btnDanQI.ShowWindow(SW_HIDE);
	m_btnShuangQI.ShowWindow(SW_HIDE);
	m_btnDaBA.ShowWindow(SW_HIDE);
	m_btnXiaoBA.ShowWindow(SW_HIDE);
	m_btnDanBA.ShowWindow(SW_HIDE);
	m_btnShuangBA.ShowWindow(SW_HIDE);
	m_btnDaJIU.ShowWindow(SW_HIDE);
	m_btnXiaoJIU.ShowWindow(SW_HIDE);
	m_btnDanJIU.ShowWindow(SW_HIDE);
	m_btnShuangJIU.ShowWindow(SW_HIDE);
	m_btnDaShi.ShowWindow(SW_HIDE);
	m_btnXiaoShi.ShowWindow(SW_HIDE);
	m_btnDanShi.ShowWindow(SW_HIDE);
	m_btnShuangShi.ShowWindow(SW_HIDE);
}

void CBeiJingPK10::HideDanShiEdit()
{
	m_RenxuanKeyboardInput = false;
	//m_editDanShiHaoMa.ShowWindow(SW_HIDE);
	m_richDanshiHaoma.ShowWindow(SW_HIDE);
}

void CBeiJingPK10::ShowDanShiEdit()
{
	m_richDanshiHaoma.ShowWindow(SW_SHOW);
	//	m_editDanShiHaoMa.ShowWindow(SW_SHOW);
}

	//��ѡ���е���ǧ���٣�ʮ����λ��
void CBeiJingPK10::ShowRenxuanWeiShu()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_RENXUAN_TIP);
	if(pStatic)
		pStatic->ShowWindow(SW_SHOW);
}

void CBeiJingPK10::HideRenxuanWeiShu()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_RENXUAN_TIP);
	if(pStatic)
		pStatic->ShowWindow(SW_HIDE);
}

void CBeiJingPK10::CheckWanFaBtn(CTextButton& btn)
{
	m_btnHouSanZhiXuan.SetPushed(false);
	m_btnQianSanZhiXuan.SetPushed(false);
	m_btnHouSanZuXuan.SetPushed(false);
	m_btnQianSanZuXuan.SetPushed(false);
	m_btnWuXingHouEr.SetPushed(false);
	m_btnWuXingQianEr.SetPushed(false);
	m_btnHouErZuXuan.SetPushed(false);
	m_btnQianErZuXuan.SetPushed(false);
	m_btnDaXiaoDanShuang.SetPushed(false);
	m_btnQSDaXiaoDanShuang.SetPushed(false);
	m_btnBuDingWei.SetPushed(false);
	m_btnDingWeiDan.SetPushed(false);

	btn.SetPushed(true);
}
VOID CBeiJingPK10::SendQuerySystemTime()
{
	return;
}
VOID CBeiJingPK10::SendToServer(int nSendType)
{
	if(nSendType == 1)
	{
		m_bTouzhu = true;
		if(m_bTouzhu)
		{
			if(m_cbIfTs == 1)
			{
				MyMessageBox(L"�˲�����ͣ���ۣ�");
				return;
			}

			m_nTzhSign++;

			m_bTouzhu = false;	
			int itm_cnt = m_listChqSSC.GetItemCount();
			int nNowKind = 10000;
			int nNowBeishu = 100000;
			int nNowMoshi = 10000;
			CString strTouZhuhaoma;
			int first = 0;
			map<int,int> mapHaoma;
			mapHaoma.clear();
			for(int i = 0;i < itm_cnt;i++)
			{
				strTouZhuhaoma.Empty();
				CString strWanfa = m_listChqSSC.GetItemText(i, 3);
				CString strBeishu = m_listChqSSC.GetItemText(i, 5);
				CString strMoshi = m_listChqSSC.GetItemText(i, 1);

				int nKind = GetGameKindByDesc(strWanfa);
				int nBeishu = _ttoi(strBeishu);

				int nMoshi = GetMoshiRet(strMoshi);
				if(nNowKind == nKind && nNowBeishu == nBeishu && nNowMoshi == nMoshi && nNowMoshi!= 10000 && nNowBeishu != 100000 && nNowKind != 10000&&nNowKind != SSC_LongHu)
					continue;
				bool bContinue = false;
				for (map<int,int>::iterator pos = mapHaoma.begin(); pos != mapHaoma.end();++pos)
				{
					if(i == pos->second)
					{
						bContinue = true;
						break;
					}
				}
				if(bContinue)
					continue;
				nNowBeishu = nBeishu;
				nNowKind = nKind;
				nNowMoshi = nMoshi;

				int nZhushu = 0;
				for(int j = i;j<itm_cnt;j++)
				{
					CString wanfa = m_listChqSSC.GetItemText(j, 3);
					int kind = GetGameKindByDesc(wanfa);
					CString sbeishu = m_listChqSSC.GetItemText(j, 5);
					int beishu = _ttoi(sbeishu);

					CString sMoshi = m_listChqSSC.GetItemText(j, 1);
					int scoremoshi = 0;
					scoremoshi = GetMoshiRet(sMoshi);
					if (kind == nNowKind && beishu == nNowBeishu && scoremoshi == nNowMoshi)
					{
						CString strZhu = m_listChqSSC.GetItemText(j, 4);
						nZhushu += _ttoi(strZhu);
						int nSize = mapHaoma.size();
						mapHaoma[nSize] = j;

						CString haoma = m_listChqSSC.GetItemText(j, 0);
						if(nKind == WF_DXDS||haoma==_T("������ӳɹ�"))
						{
							haoma = vecAllNums[itm_cnt-j-1].c_str();
						}
						strTouZhuhaoma+= haoma;
						if(haoma[haoma.GetLength()-1] != '|')
							strTouZhuhaoma+=TEXT("|");
					}
				}

				int nSendCount = 0;
				int nEnd = 1;

				CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
				ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
				TouzhuCQSSC.dwUserID = theAccount.user_id;
				TouzhuCQSSC.nBeitou = nNowBeishu;
				TouzhuCQSSC.nGameKind = nNowKind;
				TouzhuCQSSC.nGameType = CZ_PK10;
				TouzhuCQSSC.nMoshi = nNowMoshi;

				TouzhuCQSSC.nSign = m_nTzhSign;

				CString strQiHao;
				strQiHao = m_chqRule.GetNextExpect();


				if(strTouZhuhaoma.GetLength()>8000)
				{
					if(first == 0)
						m_DlgStatus.ShowStatusWindow(TEXT("�����ύ���ݣ����Ժ�..."));
					first++;

				}
				string	strAllHaoma;
				ChangeStringToA(strTouZhuhaoma, strAllHaoma);

				string strQi;
				ChangeStringToA(strQiHao, strQi);

				memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());


				if(strTouZhuhaoma.IsEmpty())
				{
					m_DlgStatus.HideStatusWindow();
					MyMessageBox(L"�㻹δѡ����룬��ѡ��");

					return ;
				}

				byte pDestBuffer[102400];
				int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

				int nSendSize = 0;
				TouzhuCQSSC.nEnd = 1;
				if(m_zongZhuShu<=0)
				{
					m_DlgStatus.HideStatusWindow();
					OnBnClickedBtnClsList();
					return;
				}

				TouzhuCQSSC.nZhushu = nZhushu;
				int nSize = sizeof(TouzhuCQSSC.strHaoma);
				DWORD dwTick=GetTickCount();

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
				nSendCount++;

				m_bCanSend = true;
			}

			
			OnBnClickedBtnClsList();
		}

		return;
	}

	if(nSendType == 2)   //û��
	{
		m_bGetCPUserInfo = true;
		return;
	}

	if(nSendType == 3)
	{
		m_btnLock.SetWindowText(mapFandian[m_gameKind]);

		m_btWxzxTip.ShowWindow(SW_SHOW);

		HideWanNumsQ();
		HideBaiNumsQ();
		HideQianNumsQ();
		HideShiNumsQ();
		HideGeNumsQ();
		if(m_RenxuanKeyboardInput == false)
		{
			if(m_gameKind == WF_GuanJun)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵ�һλ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_GuanYaJun)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ӹھ����Ǿ������ٸ�ѡ��һ�����룬��ѡ�����뿪�������ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_YaJun)
			{ 
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵڶ�λ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianSan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ӹھ����Ǿ��͵����������ٸ�ѡ��һ�����룬��ѡ�����뿪�������ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_DiSan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵ���λ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianSi)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ÿλ���ٸ�ѡ��һ�����룬��ѡ�����뿪�������ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_DiSi)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵ���λ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianWu)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ÿλ���ٸ�ѡ��һ�����룬��ѡ�����뿪�������ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_DiWu)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵ���λ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianLiu)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ÿλ���ٸ�ѡ��һ�����룬��ѡ�����뿪�������ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_DiLiu)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵ���λ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianQi)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ÿλ���ٸ�ѡ��һ�����룬��ѡ�����뿪�������ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_DiQi)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵ���λ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianBa)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ÿλ���ٸ�ѡ��һ�����룬��ѡ�����뿪�������ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_DiBa)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵڰ�λ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianJiu)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ÿλ���ٸ�ѡ��һ�����룬��ѡ�����뿪�������ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_DiJiu)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵھ�λ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianShi)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ÿλ���ٸ�ѡ��һ�����룬��ѡ�����뿪�������ǰʮλ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_DiShi)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-10������ѡ��һ�����룬��ѡ�����뿪������ĵ�ʮλ��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_DXDS)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ÿλ���ѡ��һ������ѡ�����뿪�������Ӧλ�õĴ�С��˫��ͬ����Ϊ�н�"));
			}
			else if(m_gameKind == WF_DWD)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ÿλ���ѡ��һ������ѡ�����뿪�������Ӧλ����ͬ����Ϊ�н�"));
			}
		}
		else
		{
			if(m_gameKind == WF_GuanYaJun)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�����2����λ���������һע����ѡ�����뿪���ھ����Ǿ���ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianSan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�����3����λ���������һע����ѡ�����뿪��ǰ������ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianSi)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�����4����λ���������һע����ѡ�����뿪��ǰ������ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == WF_QianWu)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�����5����λ���������һע����ѡ�����뿪��ǰ������ͬ����˳��һ�£���Ϊ�н�"));
			}
		}

		//
		//OnBnClickedBtnClsList();
		//m_zongZhuShu = 0;
// 		m_editBeiTou.SetWindowText(L"1");
// 		m_beishu=1;
		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=400;
		InvalidateRect(&rcInvali);
		InvalidateRect(&rcZongJinE);

	}

	if(nSendType == 4)
	{
		m_bZhuihao = true;
		if(m_bZhuihao)
		{
			if(m_cbIfTs == 1)
			{
				MyMessageBox(L"�˲�����ͣ���ۣ�");
				return;
			}

			m_bZhuihao = false;
			m_DlgStatus.ShowStatusWindow(TEXT("�����ύ���ݣ����Ժ�..."));
			m_nTzhSign++;

			int nItemCount = m_listChqSSC.GetItemCount();
			//	for(int i = 0;i < itm_cnt;i++)
			{

				CMD_GP_TouZhuCQSSC_Zhuihao TouzhuCQSSC;
				ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
				TouzhuCQSSC.dwUserID = theAccount.user_id;
				TouzhuCQSSC.nGameType = CZ_PK10;
				//TouzhuCQSSC.nMoshi = theAccount.Scoretype;

				TouzhuCQSSC.nSign = m_nTzhSign;
				if(m_bZhuiZjtz)
					TouzhuCQSSC.bZhuihao = GetTickCount();
				else
					TouzhuCQSSC.bZhuihao = 2;
				for(int n=0; n< nItemCount; n++)
				{
					int nSendCount = 0;
					int nEnd = 1;



					CString wanfa = m_listChqSSC.GetItemText(n, 3);
					int kind = GetGameKindByDesc(wanfa);

					TouzhuCQSSC.nGameKind = kind;
					CString moshi = m_listChqSSC.GetItemText(n, 1);
					int nScoreType = GetMoshiRet(moshi);
					TouzhuCQSSC.nMoshi = nScoreType;

					m_strHaoma = vecAllNums[nItemCount-n-1].c_str();
					if(m_strHaoma.IsEmpty())
					{
						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"�㻹δѡ����룬��ѡ��");
						OnBnClickedBtnClsList();
						return ;
					}
					string	strAllHaoma;
					ChangeStringToA(m_strHaoma, strAllHaoma);

					memset(TouzhuCQSSC.strQishu,0,sizeof(TouzhuCQSSC.strQishu));

					CString strQiHao;
					strQiHao = m_chqRule.GetNextExpect();

					string sNowQi;
					ChangeStringToA(strQiHao,sNowQi);


					for(int o=0; o< vecData2.size(); o++)
					{
						string strQi;
						ChangeStringToA(vecData2[o].c_str(),strQi);
						if(strQi < sNowQi)
						{

							MyMessageBox(L"�����ں����󣡿���ԭ�򣬸����Ѿ�ֹͣͶע��");
							OnBnClickedBtnClsList();
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
					if(m_zongZhuShu<=0)
					{
						m_DlgStatus.HideStatusWindow();
						OnBnClickedBtnClsList();
						return;
					}
					int nZhushu = 0;
					CString strZhu = m_listChqSSC.GetItemText(n, 4);
					nZhushu += _ttoi(strZhu);

					TouzhuCQSSC.nZhushu = nZhushu;


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
	//		m_nTzhSign++;
			

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
			SetUserBonus.n_t_type_id = CZ_PK10;
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

	if(nSendType == 6)
	{
		//m_btnLock.EnableTextButton(true);
		m_bGetUserFandian = true;
		if(m_bGetUserFandian)
		{
			m_bGetUserFandian=false;
			CMD_GP_GetUserFandian GetUserFandian;
			ZeroMemory(&GetUserFandian,sizeof(GetUserFandian));

			GetUserFandian.n_t_userid = theAccount.user_id;
			GetUserFandian.n_t_type_id = CZ_PK10;
			GetUserFandian.n_t_kind_id = m_gameKind;

			//SetTimer(TimerFandian,5000,NULL);
			//��������
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_FANDIAN,&GetUserFandian,sizeof(GetUserFandian));


			return;
		}
		return;
	}

	if(nSendType == 7)
	{

// 		m_bGetLastYue =true;
// 		if(m_bGetLastYue)
// 		{
// 			m_bGetLastYue = true;
// 			CMD_GP_GetLastYue GetLastYue;
// 			ZeroMemory(&GetLastYue,sizeof(GetLastYue));
// 
// 			GetLastYue.dwUserID = theAccount.user_id;
// 
// 			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
// 			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_LAST_YUE,&GetLastYue,sizeof(GetLastYue));
// 		}
		return;
	}

	if(nSendType == 8)
	{
		CMD_GP_GetMoreRecord GetMoreRecord;
		ZeroMemory(&GetMoreRecord,sizeof(GetMoreRecord));

		GetMoreRecord.n_t_type = CZ_PK10;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_MORE_RECORD,&GetMoreRecord,sizeof(GetMoreRecord));
		return;

	}

	if(nSendType == 11)
	{
		CMD_GP_QueryStatusLottery QueryStatusLottery;
		ZeroMemory(&QueryStatusLottery,sizeof(QueryStatusLottery));

		QueryStatusLottery.n_t_kindid = CZ_PK10;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_STATUS_LOTTERY,&QueryStatusLottery,sizeof(QueryStatusLottery));
	}

}
VOID CBeiJingPK10::SendQueryLuckyNumCQSSC()
{
	return;
}


//�������˷���ǰN����������
bool CBeiJingPK10::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex)
{
	_tcscpy_s(m_kjXinxi[nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);

	if(nIndex+1 < TOP_KAIJIANG_NUMBERS)
		return true;
	m_kjXinxiCont = TOP_KAIJIANG_NUMBERS;

	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		//��һ�ν���
		if(m_lastExpect[0] == 0)
			_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);

		_tcscpy_s(CBJPK10Rule::m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);
		_tcscpy_s(CBJPK10Rule::m_lastKjShj, KJ_SHIJIAN_LENGTH, m_kjXinxi[0].shijian);

		m_bKaiJiangzhong=false;											//��������
		KillTimer(timer_id_kaijiangzhong);
	}
	if(IsWindowVisible())
	{
		//	Invalidate(FALSE);
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

bool CBeiJingPK10::LuckyNumCQSSC()
{
		return true;
	CString xmlurl = _T("http://www.cailele.com/static/11yun/newlyopenlist.xml");

	CString strPath;  //�ļ�·��
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	int pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\11yun.xml"); 

//  	if (FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)))
//  	{
//  //		return false;
//  	}

	CString strexpect;
	CString stropencode;
	CString stropentime;
	CString strall;

	if(!m_xml.Load(strPath))
	{
		return false;
	}

	m_xml.ResetMainPos();
	m_xml.FindElem();

	int count = 0;
	while( m_xml.FindChildElem( _T("row") ) && count < Kj_XinXi_Count )
	{
		strexpect = m_xml.GetChildAttrib(_T("expect"));			//�ں�
		stropencode = m_xml.GetChildAttrib(_T("opencode"));	//����
		stropentime = m_xml.GetChildAttrib(_T("opentime"));	//����ʱ��
		stropencode.Remove(',');

		_tcscpy_s(m_kjXinxi[count].qihao, KJ_QIHAO_LENGTH, strexpect);
		_tcscpy_s(m_kjXinxi[count].haoma, KJ_HAOMA_LENGTH, stropencode);
		_tcscpy_s(m_kjXinxi[count].shijian, KJ_SHIJIAN_LENGTH, stropentime);



		count ++;
	}

	m_kjXinxiCont = count;

	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		//��һ�ν���
		if(m_lastExpect[0] == 0)
			_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);

		_tcscpy_s(CBJPK10Rule::m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);
		_tcscpy_s(CBJPK10Rule::m_lastKjShj, KJ_SHIJIAN_LENGTH, m_kjXinxi[0].shijian);

		m_bKaiJiangzhong=false;											//��������
		KillTimer(timer_id_kaijiangzhong);
	//	Invalidate(FALSE);
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


long CBeiJingPK10::GetKjShjDiff()
{
	long secDiff = m_chqRule.GetKjShjDiff();

	if (secDiff == m_chqRule.GetQiSpan())
	{
		m_bKaiJiangzhong=true;
		return 0;
	}
	else if(secDiff <= 30)
		return 0;
	return secDiff;
}


//���ڿ�������
void CBeiJingPK10::DrawLastKjHaoma(CDC* pDC, Graphics& graphics)
{
	long secDiff = m_chqRule.GetKjShjDiff();
	if(IsWindowVisible() && secDiff > 30 && secDiff <=35)
	{
		CPlatformFrame* pPlatforFrame = CPlatformFrame::GetInstance();
		if(pPlatforFrame->m_bSound)
			sndPlaySound(MAKEINTRESOURCE(IDR_CLOCK),  SND_RESOURCE |SND_ASYNC);
	}

	if(secDiff == 30)
	{
		CWnd *pParent = GetParent();
		if(pParent!=NULL)
		{
			pParent->PostMessage(IDM_TANCHUANG,0,0);
		}

	}
	int nTimeLeft = GetKjShjDiff();
	//ʱ���þ��ˣ���ʾ��������;
	//m_kjXinxi ����Ǳ������¿�������
	//�������ڵ��ں�
	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);
		m_bKaiJiangzhong=false;

	}
	else if(nTimeLeft == 0)
	{
		m_bKaiJiangzhong=true;
	}
//	else
//		m_bKaiJiangzhong=false;

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&m_KjInfoFont1);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(_T("����PK10"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	CRect rcDi;
	rcDi.left = rcCzh.right+10;
	rcDi.right = rcDi.left+20;
	rcDi.top = rcCzh.top;
	rcDi.bottom = rcCzh.bottom;
	pDC->DrawText(_T("��"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(253, 191, 6));
	if(m_bKaiJiangzhong)
	{
		pDC->DrawText(m_chqRule.GetNextExpect(-1), &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else
		pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(RGB(255, 255, 255));

	rcDi.left = rcQiHao.right-12;
	rcDi.right = rcDi.left+90;
	rcDi.top = rcCzh.top;
	rcDi.bottom = rcCzh.bottom;
	pDC->DrawText(_T("��������"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(&m_KjInfoFont1);
	CString strYg;
	CRect rcYuGao1;
	//strYg.Format(_T("��������"));
	//pDC->DrawText(strYg, &rcYuGao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);


	if(m_cbIfTs == 1)
	{
		strYg.Format(_T("��ͣ����"));
	}
	else
	{
		if(secDiff < 30)
			strYg.Format(_T("��%s	�ⵥʱ��"), m_chqRule.GetNextExpect());
		else
			strYg.Format(_T("��%s	�µ�ʱ��"), m_chqRule.GetNextExpect());
	}
	//pDC->SetTextColor(RGB(255, 125, 0));
	rcYuGao1.left = rcYuGao.left+15;
	rcYuGao1.top = rcYuGao.top+40;
	rcYuGao1.bottom = rcYuGao.bottom+70;
	rcYuGao1.right = rcYuGao.right+50;
	pDC->DrawText(strYg, &rcYuGao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);


	// 	strYg.Format(_T("��ⵥ����"));
	// 	//pDC->SetTextColor(RGB(255, 226, 109));
	// 	rcYuGao1.left = rcYuGao1.left;
	// 	rcYuGao1.top = rcYuGao1.top+20;
	// 	rcYuGao1.bottom = rcYuGao1.bottom+20;
	// 	rcYuGao1.right = rcYuGao1.right;
	// 	pDC->DrawText(strYg, &rcYuGao1, DT_CENTER|DT_SINGLELINE);
	pDC->SelectObject(&m_KjInfoFont);
	strYg.Format(_T("%s"), m_chqRule.GetKjShjDiffDesc(30));
	pDC->SetTextColor(RGB(219, 219, 219));
	rcYuGao1.left = rcYuGao1.left-75;
	rcYuGao1.top = rcYuGao1.top+12;
	rcYuGao1.bottom = rcYuGao1.bottom+20;
	rcYuGao1.right = rcYuGao1.right-20;
	pDC->DrawText(strYg, &rcYuGao1,DT_CENTER|DT_SINGLELINE);


	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(&pOldFont);
	pDC->SetBkMode(oldBkMode);

	CString str = m_kjXinxi[0].haoma;
	CString temp;
	temp = str.Left(2);

	if(_ttoi(temp) > 11)
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
			int num = _ttoi(CString(str.GetAt(i)));
			if(strTemp.GetLength()>=2)
			{
				num = _ttoi(strTemp);
			}
			if (num >=1 && num < 12) 
			{
				if(m_bKaiJiangzhong )
				{
					KillTimer(timer_id_kaijiangzhong);
					SetTimer(timer_id_kaijiangzhong, 100, NULL);
				}
				else
				{
					DrawBigNum(graphics, m_kjNumBig, Rect(kj_big_haoma_x + index * (m_bigNumWidth), kj_big_haoma_y, m_bigNumWidth, m_bigNumHeight), num);
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

//��������
void CBeiJingPK10::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(250 ,250 ,250));

	for (int cnt=0; cnt<4; cnt++) 
	{
		CString str = m_kjXinxi[cnt].qihao;
		if(str.IsEmpty())
		{
			continue;
		}

		CRect rcText(kj_small_haoma_x - 38, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 12, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[cnt].haoma;
		CString temp;
		temp = str.Left(2);

		if(_ttoi(temp) > 11)
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
				if (num >0 && num < 12&& strTemp.GetLength()>=2) 
				{
					Rect rc(kj_small_haoma_x+10 + index * (m_smallNumWidth + kj_small_haoma_col_span), 
						kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
						m_smallNumWidth, m_smallNumHeight);
					DrawSmallNum(graphics, m_kjNumSmall, rc, num);
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
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}

void CBeiJingPK10::DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, (num-1)*m_bigNumWidth, 0, m_bigNumWidth, m_bigNumHeight, UnitPixel);
}

void CBeiJingPK10::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, (num-1)*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);
}

void CBeiJingPK10::DrawZongZhushuJinE(CDC* pDC)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_zongFont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(139, 137, 136));
	strText.Format(_T("��ѡ����"));

	CRect rcZhu;
	rcZhu.CopyRect(rcZongZhuShu);
	rcZhu.left += 188;
	rcZhu.right += 475;
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(160, 8, 8));
	strText.Format(_T("%d"), m_singlezhushu);
	rcZhu.left += 56;
	rcZhu.right += 56;
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(139, 137, 136));
	rcZhu.left += strText.GetLength()*9;
	rcZhu.right += strText.GetLength()*9;
	strText.Format(_T("ע"));
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	CRect rcRect;

	rcRect.left = rcZongZhuShu.left+39;
	rcRect.right = rcZongZhuShu.right;
	rcRect.top = rcZongZhuShu.top+6;
	rcRect.bottom = rcZongZhuShu.bottom;
	strText.Format(L"��");
	pDC->DrawText(strText, &rcRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	CRect rcRect_down;

	rcRect_down.left = rcZongZhuShu.left+39;
	rcRect_down.right = rcZongZhuShu.right;
	rcRect_down.top = rcZongZhuShu.top+6;
	rcRect_down.bottom = rcZongZhuShu.bottom+30;
	strText.Format(L"��");
	pDC->DrawText(strText, &rcRect_down, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
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

	strText.Format(_T("��:"));
	pDC->DrawText(strText, &rcZongJinE, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	rcRect.CopyRect(rcZongJinE);
	rcRect.left += 17;
	rcRect.right += 17;
	pDC->SetTextColor(RGB(160, 8, 8));

	strText.Format(_T("%.3lf"), m_singlezhushu * m_beishu * danzhujine);
	pDC->DrawText(strText, &rcRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(139, 137, 136));

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
	pDC->SetTextColor(RGB(139, 137, 136));

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

void CBeiJingPK10::ResetAllNums()
{
	m_singlezhushu = 0;
	UnCheckNumBtn(m_btnWan_0);
	UnCheckNumBtn(m_btnWan_1);
	UnCheckNumBtn(m_btnWan_2);
	UnCheckNumBtn(m_btnWan_3);
	UnCheckNumBtn(m_btnWan_4);
	UnCheckNumBtn(m_btnWan_5);
	UnCheckNumBtn(m_btnWan_6);
	UnCheckNumBtn(m_btnWan_7);
	UnCheckNumBtn(m_btnWan_8);
	UnCheckNumBtn(m_btnWan_9);

	UnCheckNumBtn(m_btnQian_0);
	UnCheckNumBtn(m_btnQian_1);
	UnCheckNumBtn(m_btnQian_2);
	UnCheckNumBtn(m_btnQian_3);
	UnCheckNumBtn(m_btnQian_4);
	UnCheckNumBtn(m_btnQian_5);
	UnCheckNumBtn(m_btnQian_6);
	UnCheckNumBtn(m_btnQian_7);
	UnCheckNumBtn(m_btnQian_8);
	UnCheckNumBtn(m_btnQian_9);

	UnCheckNumBtn(m_btnBai_0);
	UnCheckNumBtn(m_btnBai_1);
	UnCheckNumBtn(m_btnBai_2);
	UnCheckNumBtn(m_btnBai_3);
	UnCheckNumBtn(m_btnBai_4);
	UnCheckNumBtn(m_btnBai_5);
	UnCheckNumBtn(m_btnBai_6);
	UnCheckNumBtn(m_btnBai_7);
	UnCheckNumBtn(m_btnBai_8);
	UnCheckNumBtn(m_btnBai_9);

	UnCheckNumBtn(m_btnShi_0);
	UnCheckNumBtn(m_btnShi_1);
	UnCheckNumBtn(m_btnShi_2);
	UnCheckNumBtn(m_btnShi_3);
	UnCheckNumBtn(m_btnShi_4);
	UnCheckNumBtn(m_btnShi_5);
	UnCheckNumBtn(m_btnShi_6);
	UnCheckNumBtn(m_btnShi_7);
	UnCheckNumBtn(m_btnShi_8);
	UnCheckNumBtn(m_btnShi_9);

	UnCheckNumBtn(m_btnGe_0);
	UnCheckNumBtn(m_btnGe_1);
	UnCheckNumBtn(m_btnGe_2);
	UnCheckNumBtn(m_btnGe_3);
	UnCheckNumBtn(m_btnGe_4);
	UnCheckNumBtn(m_btnGe_5);
	UnCheckNumBtn(m_btnGe_6);
	UnCheckNumBtn(m_btnGe_7);
	UnCheckNumBtn(m_btnGe_8);
	UnCheckNumBtn(m_btnGe_9);

	UnCheckNumBtn(m_btnLiu_0);
	UnCheckNumBtn(m_btnLiu_1);
	UnCheckNumBtn(m_btnLiu_2);
	UnCheckNumBtn(m_btnLiu_3);
	UnCheckNumBtn(m_btnLiu_4);
	UnCheckNumBtn(m_btnLiu_5);
	UnCheckNumBtn(m_btnLiu_6);
	UnCheckNumBtn(m_btnLiu_7);
	UnCheckNumBtn(m_btnLiu_8);
	UnCheckNumBtn(m_btnLiu_9);

	UnCheckNumBtn(m_btnQi_0);
	UnCheckNumBtn(m_btnQi_1);
	UnCheckNumBtn(m_btnQi_2);
	UnCheckNumBtn(m_btnQi_3);
	UnCheckNumBtn(m_btnQi_4);
	UnCheckNumBtn(m_btnQi_5);
	UnCheckNumBtn(m_btnQi_6);
	UnCheckNumBtn(m_btnQi_7);
	UnCheckNumBtn(m_btnQi_8);
	UnCheckNumBtn(m_btnQi_9);

	UnCheckNumBtn(m_btnBa_0);
	UnCheckNumBtn(m_btnBa_1);
	UnCheckNumBtn(m_btnBa_2);
	UnCheckNumBtn(m_btnBa_3);
	UnCheckNumBtn(m_btnBa_4);
	UnCheckNumBtn(m_btnBa_5);
	UnCheckNumBtn(m_btnBa_6);
	UnCheckNumBtn(m_btnBa_7);
	UnCheckNumBtn(m_btnBa_8);
	UnCheckNumBtn(m_btnBa_9);

	UnCheckNumBtn(m_btnJiu_0);
	UnCheckNumBtn(m_btnJiu_1);
	UnCheckNumBtn(m_btnJiu_2);
	UnCheckNumBtn(m_btnJiu_3);
	UnCheckNumBtn(m_btnJiu_4);
	UnCheckNumBtn(m_btnJiu_5);
	UnCheckNumBtn(m_btnJiu_6);
	UnCheckNumBtn(m_btnJiu_7);
	UnCheckNumBtn(m_btnJiu_8);
	UnCheckNumBtn(m_btnJiu_9);

	UnCheckNumBtn(m_btnShi10_0);
	UnCheckNumBtn(m_btnShi10_1);
	UnCheckNumBtn(m_btnShi10_2);
	UnCheckNumBtn(m_btnShi10_3);
	UnCheckNumBtn(m_btnShi10_4);
	UnCheckNumBtn(m_btnShi10_5);
	UnCheckNumBtn(m_btnShi10_6);
	UnCheckNumBtn(m_btnShi10_7);
	UnCheckNumBtn(m_btnShi10_8);
	UnCheckNumBtn(m_btnShi10_9);
//////////////////////////////////
	UnCheckNumBtn(m_btnDaGUAN);
	UnCheckNumBtn(m_btnXiaoGUAN);
	UnCheckNumBtn(m_btnDanGUAN);
	UnCheckNumBtn(m_btnShuangGUAN);
	UnCheckNumBtn(m_btnDaYA);
	UnCheckNumBtn(m_btnXiaoYA);
	UnCheckNumBtn(m_btnDanYA);
	UnCheckNumBtn(m_btnShuangYA);

	UnCheckNumBtn(m_btnDaSAN);
	UnCheckNumBtn(m_btnXiaoSAN);
	UnCheckNumBtn(m_btnDanSAN);
	UnCheckNumBtn(m_btnShuangSAN);
	UnCheckNumBtn(m_btnDaSI);
	UnCheckNumBtn(m_btnXiaoSI);
	UnCheckNumBtn(m_btnDanSI);
	UnCheckNumBtn(m_btnShuangSI);

	UnCheckNumBtn(m_btnDaWU);
	UnCheckNumBtn(m_btnXiaoWU);
	UnCheckNumBtn(m_btnDanWU);
	UnCheckNumBtn(m_btnShuangWU);
	UnCheckNumBtn(m_btnDaLIU);
	UnCheckNumBtn(m_btnXiaoLIU);
	UnCheckNumBtn(m_btnDanLIU);
	UnCheckNumBtn(m_btnShuangLIU);

	UnCheckNumBtn(m_btnDaQI);
	UnCheckNumBtn(m_btnXiaoQI);
	UnCheckNumBtn(m_btnDanQI);
	UnCheckNumBtn(m_btnShuangQI);
	UnCheckNumBtn(m_btnDaBA);
	UnCheckNumBtn(m_btnXiaoBA);
	UnCheckNumBtn(m_btnDanBA);
	UnCheckNumBtn(m_btnShuangBA);

	UnCheckNumBtn(m_btnDaShi);
	UnCheckNumBtn(m_btnXiaoShi);
	UnCheckNumBtn(m_btnDanShi);
	UnCheckNumBtn(m_btnShuangShi);
	UnCheckNumBtn(m_btnDaJIU);
	UnCheckNumBtn(m_btnXiaoJIU);
	UnCheckNumBtn(m_btnDanJIU);
	UnCheckNumBtn(m_btnShuangJIU);

	m_richDanshiHaoma.SetWindowText(_T(""));

	//m_editDanShiHaoMa.SetWindowText(_T(""));
}

void CBeiJingPK10::SetLockText(float fBonus, float fBonusPercent)
{
	const float EPSINON = 0.00001;
	if((fBonus >= - EPSINON) && (fBonus <= EPSINON))
	{
	}
	else
	{
		CString strText;
		strText.Format(L"%.2lf/%.2lf", fBonus, fBonusPercent);
		m_btnLock.SetWindowText(strText + _T("%"));
	}
}

//��ȡ������ʽ�ĺ���
bool CBeiJingPK10::GetZuSanHaoma(CStringArray& strArr)
{
	CString line;
	//m_editDanShiHaoMa.GetWindowText(line);
	m_richDanshiHaoma.GetWindowText(line);
	if(line.GetLength() > 2999)
	{
		MyMessageBox(_T("������ĺ���̫����"));
		return false;
	}
	if (!GetHaomaStrs(strArr, line, 3,0)) 
	{
		return false;
	}

	int count = strArr.GetCount();
	for(int i=0; i<count; i++)
	{
		if (!IsZuSanHaoma(strArr[i])) 
		{
			return false;
		}
	}

	return true;
}

bool CBeiJingPK10::GetZuSanHaoma(CString& haoma, int& zhushu)
{
	CStringArray strArr;
	
	if(!GetZuSanHaoma(strArr))
	{
		zhushu = 0;
		return false;
	}

	int count = strArr.GetCount();
	for(int i=0; i<count; i++)
	{
		haoma += strArr.GetAt(i);

		if (i < count - 1)
		{
			haoma += _T(",");
		}
	}
	zhushu = count;

	return true;
}

bool CBeiJingPK10::IsZuSanHaoma(const CString& str)
{
	if(str.GetLength() != 3)
	{
		return false;
	}

	TCHAR ch0 = str.GetAt(0);
	TCHAR ch1 = str.GetAt(1);
	TCHAR ch2 = str.GetAt(2);

	return (ch0 == ch1 || ch0 == ch2 || ch1 == ch2)
		&& (ch0 != ch1 || ch0 != ch2 || ch1 != ch2);
}

bool CBeiJingPK10::GetDanShiHaoma(CStringArray& strArr, int size)
{
	CString line;
	//m_editDanShiHaoMa.GetWindowText(line);
	m_richDanshiHaoma.GetWindowText(line);
	if(line.IsEmpty())
	{
		MyMessageBox(_T("��������룡"));	

		return false;
	}

	if(line.GetLength() > 2999)
	{
		MyMessageBox(_T("������ĺ���̫����"));	
		return false;
	}
	if (!GetHaomaStrs(strArr, line, size,0)) 
	{
		return false;
	}

	return true;
}
#include <algorithm>
#include <functional> 
int isnum(string strinfo)  
{  
	string strset="1234567890";  
	int first = strinfo.find_first_of(strset);  
	if(first == string::npos)   
	{  
		return -1;  
	}   

	return 0;  
}

bool CBeiJingPK10::GetDanShiHaoma(CString& haoma, int size, int nLeast)
{
	
	CStringArray strArr;
	CString line;
	//m_editDanShiHaoMa.GetWindowText(line);
	m_richDanshiHaoma.GetWindowText(line);
	if(line.IsEmpty())
	{
	//	MyMessageBox(_T("��������룡"));	

		return false;
	}
	string strDanHoa;
	ChangeStringToA(line,strDanHoa);
	int nIndex = 0;

	//	strDanHoa = strDanHoa.replace(" ","");
	//string str("  d df df "); 
	
	string::iterator new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), ' ')); 

	strDanHoa.erase(new_end, strDanHoa.end()); 
	
	 new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), '\r')); 

	strDanHoa.erase(new_end, strDanHoa.end()); 
	
	size_t oldPos,newPos;
	oldPos=0;
	newPos=0;
	string splitStr=",";

	int nLeft = 0;
	string strOutHaoma;
	int nCeshiSize = 0;
	splitStr=",";
	newPos = strDanHoa.find(splitStr,oldPos);
	
	if(newPos==-1)
	{
		splitStr = "+";
		newPos = strDanHoa.find(splitStr,oldPos);
		if(newPos==-1)
		{
			splitStr = ";";
			newPos = strDanHoa.find(splitStr,oldPos);
			if(newPos==-1)
			{
				splitStr = "\n";

				new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), '\r')); 

				strDanHoa.erase(new_end, strDanHoa.end()); 
				newPos = strDanHoa.find(splitStr,oldPos);
				nLeft  = 0;
			}
			else
			{
				strDanHoa.erase(new_end, strDanHoa.end()); 
				new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), '\n')); 

				strDanHoa.erase(new_end, strDanHoa.end()); 

				new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), '\r')); 

				strDanHoa.erase(new_end, strDanHoa.end()); 
				newPos = strDanHoa.find(splitStr,oldPos);
				nCeshiSize = strDanHoa.size();
			}

		}
		else
		{
			strDanHoa.erase(new_end, strDanHoa.end()); 
			new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), ';')); 

			strDanHoa.erase(new_end, strDanHoa.end()); 
			new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), '\n')); 

			strDanHoa.erase(new_end, strDanHoa.end()); 
			new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), '\n')); 

			strDanHoa.erase(new_end, strDanHoa.end()); 

			new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), '\r')); 

			strDanHoa.erase(new_end, strDanHoa.end()); 
			newPos = strDanHoa.find(splitStr,oldPos);
			nCeshiSize = strDanHoa.size();
		}

	}
	else
	{
		nCeshiSize = strDanHoa.size();
		string::iterator new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(),'+')); 

		strDanHoa.erase(new_end, strDanHoa.end()); 
		new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), ';')); 

		strDanHoa.erase(new_end, strDanHoa.end()); 
		new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), '\n')); 

		strDanHoa.erase(new_end, strDanHoa.end()); 

		new_end = std::remove_if(strDanHoa.begin(), strDanHoa.end(), bind2nd(equal_to <char>(), '\r')); 

		strDanHoa.erase(new_end, strDanHoa.end()); 

//		int n = strDanHoa.find("\n");
// 		while(n!=-1)
// 		{
// 			strDanHoa.replace(n,1,"");
// 			n = strDanHoa.find("\n",n+1);
// 		}
// 		int n = strDanHoa.find("\r");
// 		if(n!=-1)
// 			strDanHoa.replace(n,1,"");
		newPos = strDanHoa.find(splitStr,oldPos);
		nCeshiSize = strDanHoa.size();
	}
	oldPos=0;
	newPos=0;

	do 
	{

		newPos = strDanHoa.find(splitStr,oldPos);
		

		
		int nSplLength = splitStr.length();
		string strTemp ;
		if(newPos == -1)
		{
			//	string strTemp ;
			
			strTemp= strDanHoa.substr(oldPos,strDanHoa.length()-oldPos);
	
			if(strTemp.empty()&&!strOutHaoma.empty())
			{
				break;
			}
			if(strTemp.length()!=nLeast*2)
			{

	//			MyMessageBox(_T("������ĺ����ʽ���ԣ�"));	
				return false;
			}
			int nLength = strTemp.length();
			int nCount = 0;
			for(int i = 2;i < nLength+nCount;i+=2)
			{
				strTemp.insert(i,",");
				i+=1;
				nCount++;
			}
			
			vector<string> vectTemp;
			SplitString1(strTemp,vectTemp,",");
			int nSize = vectTemp.size();
			for(int j = 0;j < nSize;j++)
			{
				for (int k = j+1;k < nSize;k++)
				{
					if(vectTemp[j] == vectTemp[k])
					{
	//					MyMessageBox(_T("�������ظ��ĺ��룡"));	
						return false;

					}
				}
			}
			

			strTemp = strTemp+"|";
			strOutHaoma+=strTemp;
			break;
		}
		else
			strTemp= strDanHoa.substr(oldPos,newPos-oldPos-nLeft);
		
		int nNum = isnum(strTemp);
	
		
		if(nNum == -1)
		{
	//		MyMessageBox(_T("�������벻�Ϸ�������ȫ�����֣�"));	
			return false;

		}
		int nLength = strTemp.length();
		if(strTemp.length()!=nLeast*2)
		{

	//		MyMessageBox(_T("������ĺ����ʽ���ԣ�"));	
			return false;
		}
		int nCount = 0;

		

		for(int i = 2;i < nLength+nCount;i+=2)
		{
			strTemp.insert(i,",");
			i+=1;
			nCount++;

		}
		

		vector<string> vectTemp;
		SplitString1(strTemp,vectTemp,",");
		int nSize = vectTemp.size();
		for(int j = 0;j < nSize;j++)
		{
			for (int k = j+1;k < nSize;k++)
			{
				if(vectTemp[j] == vectTemp[k])
				{
	//				MyMessageBox(_T("�������ظ��ĺ��룡"));	
					return false;

				}
			}
		}
		
		strTemp = strTemp+"|";


		strOutHaoma+=strTemp;
		oldPos = newPos+splitStr.length();
		nIndex++;
	} while (nIndex<80000);

	haoma =strOutHaoma.c_str();
	return true;
}

bool CBeiJingPK10::GetHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast)
{
	int len = line.GetLength();
	CString subStr;
	CString strTemp;
	int nTempHaoma[11];
	int nCount = 0;
	ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));
	int j = 0;
	for (int i=0; i<len; i++) 
	{
		CString str;
		str=line.GetAt(i);
		if (iswdigit(line.GetAt(i))&&!str.IsEmpty()) 
		{
			subStr += line.GetAt(i);
			strTemp+= line.GetAt(i);
			if(j%2==1)
			{
				int nTemp = _ttoi(strTemp);
				strTemp.Empty();
				if(nTemp<1||nTemp>11)
				{
					MyMessageBox(L"����������������ֻ����1��11֮�� ");
					return false;
				}
				for (int n = 0;n<nCount;n++)
				{
					if(nTemp == nTempHaoma[n])
					{
						CString strLog;
						strLog.Format(L"%02d �� %02d�ظ�",nTemp,nTempHaoma[n]);
						MyMessageBox(strLog);
						return false;

					}
				}
				nTempHaoma[nCount++] = nTemp;
			}
			j++;
			if (subStr.GetLength() > nLeast*2) 
			{
				return false;
			}
		}
		else
		{
			// 			MyMessageBox(L"���������������벻��Ҫ������������ַ�");
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
		MyMessageBox(_T("��������룡"));	

		return false;

	}

	if (subStr.GetLength() != 0) 
	{
		strArr.Add(subStr);
		subStr = _T("");
	}
	return true;
}

//��ʼ���淨��Ϣ
void CBeiJingPK10::InitWanFaMoShi()
{
	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_GuanJun] = _T("�ھ�");		

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_GuanYaJun] =  _T("���Ǿ�");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_YaJun] = _T("�Ǿ�");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_QianSan] = _T("ǰ����");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DiSan] = _T("������");
	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_QianSi] = _T("ǰ����");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DiSi] = _T("������");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_QianWu] = _T("ǰ����");
	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DiWu] = _T("������");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_QianLiu] = _T("ǰ����");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DiLiu] = _T("������");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_QianQi]=_T("ǰ����");					//��ѡ2,3,4
	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DiQi] = _T("������");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_QianBa]=_T("ǰ����");					//��ѡ2,3,4
	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DiBa]=_T("�ڰ���");					//��ѡ2,3,4
	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_QianJiu] = _T("ǰ����");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DiJiu]=_T("�ھ���");					//��ѡ2,3,4
	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_QianShi] = _T("ǰʮ��");

	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DiShi]=_T("��ʮ��");					//��ѡ2,3,4
	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DXDS]=_T("��λ��С��˫");					//��С��˫
	theGameKind[(CZ_PK10 << 16) | (unsigned short)WF_DWD]=_T("��λ��");					//��С��˫
}

bool	CBeiJingPK10::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}
//��ȡ�¼�
bool CBeiJingPK10::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_MORE_RECORD_RET:
			{
				ASSERT((wDataSize % sizeof(CMD_GP_GetMoreRecordRet))==0);
				if((wDataSize %sizeof(CMD_GP_GetMoreRecordRet)) !=0)return false;

				int nCount = wDataSize/sizeof(CMD_GP_GetMoreRecordRet);
				for(int i = 0;i < nCount;i++)
				{
					CMD_GP_GetMoreRecordRet *pGetMoreRecord = ((CMD_GP_GetMoreRecordRet*)pData+i);

					m_DlgOpenRecord.SetOpenRecord(pGetMoreRecord,i,true);
				}

				m_DlgOpenRecord.ShowWindow(SW_SHOW);
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
					strText.Format(L"%.2lf/%.2lf", nBonus, nPercent);

					strText+=_T("%");
					mapFandian.insert(make_pair(nKindid,strText));

				}
				m_btnLock.SetWindowText(mapFandian[m_gameKind]);
				m_btnLock.EnableTextButton(false);
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

				return true;
			}
		case SUB_GP_QUERY_GAME_RET:
			{

				ASSERT(wDataSize % sizeof(CMD_GP_QueryLotResult)==0);
				if(wDataSize % sizeof(CMD_GP_QueryLotResult)!=0)
					return false;
				WORD wCount = wDataSize/sizeof(CMD_GP_QueryLotResult);
				wCount = __min(wCount,Kj_XinXi_Count);

				m_kjXinxiCont = 0;

				for(int i = 0;i < wCount;i++)
				{
	 				CMD_GP_QueryLotResult* pQueryLotResult  =  ((CMD_GP_QueryLotResult*)pData+i);

	 				CMD_GP_QueryLotResult QueryLotResult;
	 				ZeroMemory(&QueryLotResult,sizeof(QueryLotResult));
	 
	 				QueryLotResult.wKindID = pQueryLotResult->wKindID;
	 				lstrcpyn(QueryLotResult.wPeriod,pQueryLotResult->wPeriod+2,CountArray(QueryLotResult.wPeriod));
	 				lstrcpyn(QueryLotResult.szLotNum,pQueryLotResult->szLotNum,CountArray(QueryLotResult.szLotNum));
	 				lstrcpyn(QueryLotResult.szShijian,pQueryLotResult->szShijian,CountArray(QueryLotResult.szShijian));
	 
	 				_tcscpy_s(m_kjXinxi[m_kjXinxiCont].qihao, KJ_QIHAO_LENGTH, QueryLotResult.wPeriod);
	 				_tcscpy_s(m_kjXinxi[m_kjXinxiCont].haoma, KJ_HAOMA_LENGTH, QueryLotResult.szLotNum);
	 				_tcscpy_s(m_kjXinxi[m_kjXinxiCont].shijian, KJ_SHIJIAN_LENGTH, QueryLotResult.szShijian);
					m_kjXinxiCont++;
				}

 				CRect rcClient;
 				GetClientRect(rcClient);

				if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
				{
					//��һ�ν���
					if(m_lastExpect[0] == 0)
						_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);

					_tcscpy_s(CBJPK10Rule::m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[0].qihao);
					_tcscpy_s(CBJPK10Rule::m_lastKjShj, KJ_SHIJIAN_LENGTH, m_kjXinxi[0].shijian);

					m_bKaiJiangzhong=false;											//��������
					KillTimer(timer_id_kaijiangzhong);
					Invalidate(FALSE);
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

				CMD_GR_TouzhuRes* pTouzhuRes = (CMD_GR_TouzhuRes*)pData;

				LONG lResult = pTouzhuRes->lResult;

				m_bCanSend = true;

				m_DlgStatus.HideStatusWindow();
				if (lResult == 0)
				{
					if((pTouzhuRes->nSign != m_nTzhSign1))
					{
						m_nTzhSign1 = pTouzhuRes->nSign;

						
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
				else if(lResult>=20 && lResult <= 30)
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
				break;
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

				if(pLogRet->n_t_kindid != CZ_PK10)
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
}

//	���ݿ��������
//CDBCmd	theDBCmd;
//�淨��Ӧ�Ľ���
//CMap<unsigned int, unsigned int, double, double> theBonus;
//������Ϣ-����ʱʱ��
//CMap<int, int, CString, CString&> theGameType;
//�淨ģʽ-����������ʽ
//CMap<int, int, CString, CString&> theGameKind;
//����ʱ����ϵͳʱ��Ĳ�ֵ
//long theTimeDiff = 0L;
//ϵͳʱ��
//CTime theTime;
extern DWORD theBeginCount;
extern CCriticalSection theTimeCS;
//�û��˺���Ϣ
//BaseUser theAccount;
//�ͷ���ַ
extern CString theKeFuUrl;
/////////////////////////////////��Ϸ�б�/////////////////////////////////////


//�����Ϣ
VOID CBeiJingPK10::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//�����ж�
	if (m_bHovering==false)
	{
		//���ñ���
		m_bHovering=true;

		//��������
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//ע����Ϣ
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//ע���¼�
		_TrackMouseEvent(&TrackMouseEvent);
	}
}

//�����Ϣ
VOID CBeiJingPK10::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//���ý���
	SetFocus();

	return;
}
//�����Ϣ
VOID CBeiJingPK10::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
}

//�����Ϣ
LRESULT CBeiJingPK10::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{

	//����״̬
	m_bHovering=false;
	return 0;
}


//������루��ѡ2����ѡ3����ѡ4��
void CBeiJingPK10::OnBnClickedRioMouseInput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
////��ѡ2,3,4
	
	ResetAllNums();
	if(m_gameKind == IIRenXuan2)			//��ѡ��
	{
		m_rioHsZhxFuShi.SetPushed(true);
		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();

		HideDanShiEdit();
	}

	if(m_gameKind == IIRenXuan3)		//��ѡ��
	{
		m_rioHsZsFuShi.SetPushed(true);

		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();

		HideDanShiEdit();

	}

	if(m_gameKind == IIRenXuan4)				//��ѡ��
	{
		m_rioHeZhxFuShi.SetPushed(true);

		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();

		HideDanShiEdit();

	}

	if(	m_gameKind == IIRenXuan5)				//��ѡ��
	{
		m_rioHeZxFuShi.SetPushed(true);

		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();

		HideDanShiEdit();
	}

	if(m_gameKind == IIRenXuan6)				//��ѡ��
	{
		m_rioQeDxdsh.SetPushed(true);

		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();

		HideDanShiEdit();

	}

	if(m_gameKind == IIRenXuan7)				//��ѡ��	
	{

		m_rioQsbdw.SetPushed(false);
		m_rioHsbdw.SetPushed(true);

		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();

		HideDanShiEdit();
	}

	if(m_gameKind == IIRenXuan8)						//��ѡ��
	{
		m_rioRenXuan2.SetPushed(true);

		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();

		HideDanShiEdit();
	}

	if(m_gameKind == QianEr_ZhiXuan)				//ǰ��ֱѡ
	{
		m_rioQeZhxFuShi.SetPushed(true);
		m_rioQeZhxDanShi.SetPushed(false);

		HideDaXiaoDanShuang();
		ShowWanNums();
		ShowQianNums();
		HideBaiNums();
		HideShiNums();
		HideGeNums();

		HideDanShiEdit();

	}
	

	if(m_gameKind == QianSan_ZhiXuan)
	{

		m_rioQeZxFuShi.SetPushed(true);
		m_rioQeZxDanShi.SetPushed(false);

		HideDaXiaoDanShuang();
		ShowWanNums();
		ShowQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();

		HideDanShiEdit();

	}

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
 	m_rioKeyboardInput.SetPushed(false);

	m_RenxuanKeyboardInput=false;
}

//�������루��ѡ2����ѡ3����ѡ4��
void CBeiJingPK10::OnBnClickedRioKeyboardInput()
{
	
	ResetAllNums();
	m_rioMouseInput.SetPushed(false);
	m_rioKeyboardInput.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_RenxuanKeyboardInput=true;
	SendToServer(3);
}


//�޸���ѡ�� ״̬
void CBeiJingPK10::SetCheckRenXuan(bool bChecked)
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	ValidateTips();
}

void CBeiJingPK10::OnBnClickedChkWan()
{
	ValidateTips();
}
void CBeiJingPK10::OnBnClickedChkQian()
{
	ValidateTips();
}

void CBeiJingPK10::OnBnClickedChkBai()
{
	ValidateTips();
}

void CBeiJingPK10::OnBnClickedChkShi()
{
	ValidateTips();
}

void CBeiJingPK10::OnBnClickedChkge()
{
	ValidateTips();
}

//������ʾ
void CBeiJingPK10::ValidateTips()
{
	return;
}

//��ѡ����ǧ���٣�ʮ����
int CBeiJingPK10::GetRenXuanCheckBox(BOOL& bWan, BOOL& bQian, BOOL& bBai, BOOL& bShi, BOOL& bGe)		
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn)
		bWan = pBtn->GetCheck();
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn)
		bQian = pBtn->GetCheck();
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn)
		bBai = pBtn->GetCheck();
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn)
		bShi = pBtn->GetCheck();
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn)
		bGe = pBtn->GetCheck();
	int nNums=0;
	nNums = bWan + bQian + bBai+ bShi+bGe;
	return nNums;
}

//���÷���
void CBeiJingPK10::OnBnClickedBtnLock()
{
	int index = (CZ_PK10 << 16) | (unsigned short)m_gameKind;

	
	CDlgFandian	dlg;
	dlg.SetBonus(mapBonus[m_gameKind], theBonus[index], mapPercent[m_gameKind], theAccount.fandian*100.0, mapDanweiFandian[m_gameKind]);
	if(IDOK == dlg.DoModal())
	{
		float fBonus=0;
		float fPercent=0;
		dlg.GetBonus(m_fBonus,  m_fPercent);

		SendToServer(5);
	}
}
void CBeiJingPK10::OnBnClickedBtnMoreRecord()
{
	SendToServer(8);
	return;
}
void CBeiJingPK10::OnBnClickedBtnQuShi()
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

	IniData.ReadEncryptString(TEXT("PlazaUrl"),TEXT("PlazaQushi"),TEXT("http://192.168.0.104/"),szTodayURL,CountArray(szTodayURL));

	TCHAR szUrl[126]=TEXT("");
	_sntprintf(szUrl,CountArray(szUrl),TEXT("%s/TrendCharts/pk10/q5_basic.aspx?type=13"),szTodayURL);
	ShellExecute(NULL, _T("open"), szUrl, NULL, NULL, SW_SHOWNORMAL);
	return;
}

//��Ͷ��
void CBeiJingPK10::OnBnClickedBeiTouJia()
{
	CString strBeishu;

	m_beishu += 1;

	strBeishu.Format(L"%d",m_beishu);

	m_editBeiTou.SetWindowText(strBeishu);

	if(m_RenxuanKeyboardInput)
		GetDanShiZhusu(m_gameKind);
	return;

}
//��Ͷ��
void CBeiJingPK10::OnBnClickedBeiTouJian()
{
	CString strBeishu;

	m_beishu -= 1;

	if(m_beishu<1)
	{
		m_beishu = 1;
	}
	strBeishu.Format(L"%d",m_beishu);

	m_editBeiTou.SetWindowText(strBeishu);

	if(m_RenxuanKeyboardInput)
		GetDanShiZhusu(m_gameKind);
	return;
}
LRESULT CBeiJingPK10::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	nSubItem--;

	int zhushu = _ttoi(m_listChqSSC.GetItemText(nItem, 4));
	m_zongZhuShu -= zhushu;
	m_listChqSSC.deleteItemEx( nItem );

	int nItemCount = m_listChqSSC.GetItemCount();
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_listChqSSC.GetItemText(i,5);
		CString strZhuSHu = m_listChqSSC.GetItemText(i,4);
		CString strMoshi = m_listChqSSC.GetItemText(i,1);
		int nMoshi = GetMoshiRet(strMoshi);

		double danzhujine = 2.0;

		if(nMoshi == MoShi_Yuan)
		{	
			danzhujine = 2.0;
		}
		else if (nMoshi == MoShi_Jiao)
		{
			danzhujine = 0.2;
		}
		else if(nMoshi == MoShi_Fen)
		{
			danzhujine = 0.02;
		}
		else if(nMoshi == MoShi_Li)
			danzhujine = 0.002;

		int beishu = _ttoi(strBeiSHu);
		int zhushu = _ttoi(strZhuSHu);


		m_zongJine += zhushu*beishu*danzhujine;
	}

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);


	if(vecAllNums.size() >0)
	{
		int nSize = vecAllNums.size();
		nSize -=1;

		vector<wstring>::iterator   iter = vecAllNums.begin()+nSize-nItem;

		vecAllNums.erase(iter);
	}



	return 0;
}


BOOL CBeiJingPK10::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	if((nCommandID >= IDC_BTN_WAN_0 && nCommandID <= IDC_BTN_SHI10_9)||(nCommandID >= IDC_BTN_DA_GUAN && nCommandID <= IDC_BTN_SHUANG_JIU)||(nCommandID >= IDC_BTN_DA_SHI && nCommandID <= IDC_BTN_WAN_2)||(nCommandID >= IDC_BTN_HEZHI_0 && nCommandID <= IDC_BTN_HEZHI_27)||(nCommandID >= IDC_BTN_DA_WAN && nCommandID <= IDC_BTN_SHUANG_BAI)||(nCommandID >= IDC_BTN_NIU_1 && nCommandID <= IDC_BTN_NIU_10))
	{
		int nZhushu = GetPk10Zhushu(CZ_PK10,m_gameKind);
	}

	return  __super::OnCommand(wParam,lParam);;
}

int CBeiJingPK10::GetPk10Zhushu(int nTypeID,int nKindID)
{
	CString strWan = GetWanString();
	CString strQian = GetQianString();
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();
	CString strLiu = GetLiuString();
	CString strQi = GetQiString();
	CString strBa = GetBaString();
	CString strJiu = GetJiuString();
	CString strShi10 = GetShi10String();
	m_singlezhushu = 0;
	int zhushu = 0;
	if (WF_GuanJun == m_gameKind)						//�ھ�
	{
		int nLength = strWan.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;
	}
	else if (WF_GuanYaJun == m_gameKind)				//���Ǿ�
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;

		if (nGuanLength <= 0 || nYaLength <= 0 ) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		int nChongfu = 0;

		CString strGuan;
		CString strYa;

		zhushu=0;
		for(int i = 0;i < (nGuanLength*2);i += 2)
		{
			strGuan = strWan.Mid(i,2);
			for(int j = 0;j < (nYaLength*2);j+=2)
			{
				strYa = strQian.Mid(j,2);
				if(strGuan != strYa)
				{
					zhushu++;
				}
			}
		}
		if (zhushu<=0 ) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		m_singlezhushu = zhushu;
	}
	else if (WF_YaJun == m_gameKind)				//�Ǿ�
	{
		int nLength = strQian.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;
	}
	else if (WF_QianSan == m_gameKind)					//ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;

		if (nGuanLength <= 0 || nYaLength <= 0 || nSanLength <= 0 ) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}
		CString strGuan;
		CString strYa;
		CString strSan;

		zhushu = 0;
		for(int i = 0;i < (nGuanLength*2);i+=2)
		{
			strGuan = strWan.Mid(i,2);
			for(int j = 0;j < (nYaLength*2);j+=2)
			{
				strYa = strQian.Mid(j,2);
				if(strGuan == strYa)
					continue;
				for (int k = 0;k < (nSanLength*2);k+=2)
				{
					strSan = strBai.Mid(k,2);
					if(strGuan != strYa && strYa != strSan && strGuan != strSan)
					{
						zhushu++;
					}
				}
			}
		}

		if (zhushu<=0 ) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}
		m_singlezhushu = zhushu;

	}
	else if (WF_DiSan == m_gameKind)				//������
	{
		int nLength = strBai.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;
	}
	else if (WF_QianSi == m_gameKind)					//ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;

		if (nGuanLength <= 0 || nYaLength <= 0 || nSanLength <= 0 || nSiLength <= 0) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		CString strGuan;
		CString strYa;
		CString strSan;
		CString strSi;

		zhushu = 0;
		for(int i = 0;i < (nGuanLength*2);i+=2)
		{
			strGuan = strWan.Mid(i,2);
			for(int j = 0;j < (nYaLength*2);j+=2)
			{
				strYa = strQian.Mid(j,2);
				if(strGuan == strYa)
					continue;
				for (int k = 0;k < (nSanLength*2);k+=2)
				{
					strSan = strBai.Mid(k,2);
					if(strGuan == strSan || strSan == strYa)
						continue;
					for(int l = 0;l < (nSiLength*2);l += 2)
					{
						strSi = strShi.Mid(l,2);
						if(strSi == strGuan || strSi == strYa || strSi == strSan)
							continue;
						zhushu++;
					}
				}
			}
		}

		if (zhushu<=0 ) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = zhushu;
	}
	else if (WF_DiSi == m_gameKind)					//������
	{
		int nLength = strShi.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;

	}
	else if (WF_QianWu == m_gameKind)					//ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;


		if (nGuanLength <= 0 || nYaLength <= 0 || nSanLength <= 0 || nSiLength <= 0) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		CString strGuan;
		CString strYa;
		CString strSan;
		CString strSi;
		CString strWu;

		zhushu = 0;
		for(int i = 0;i < (nGuanLength*2);i+=2)
		{
			strGuan = strWan.Mid(i,2);
			for(int j = 0;j < (nYaLength*2);j+=2)
			{
				strYa = strQian.Mid(j,2);
				if(strGuan == strYa)
					continue;
				for (int k = 0;k < (nSanLength*2);k+=2)
				{
					strSan = strBai.Mid(k,2);
					if(strGuan == strSan || strSan == strYa)
						continue;
					for(int l = 0;l < (nSiLength*2);l += 2)
					{
						strSi = strShi.Mid(l,2);
						if(strSi == strGuan || strSi == strYa || strSi == strSan)
							continue;
						for(int m = 0;m < (nWuLength*2);m+=2)
						{
							strWu = strGe.Mid(m,2);
							if(strWu == strGuan || strWu == strYa || strWu == strSan||strWu == strSi)
								continue;

							zhushu++;
						}
					}
				}
			}
		}
		if (zhushu<=0 ) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = zhushu;


	}
	else if (WF_DiWu == m_gameKind)							//������
	{
		int nLength = strGe.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;

	}
	else if (WF_QianLiu == m_gameKind)				//ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;


		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1))
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		CString str[6] = {strWan,strQian,strBai,strShi,strGe,strLiu};
		CString strTemp;
		for(int i = 0;i < 6;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 6;j++)
			{
				if(strTemp == str[j])
				{
					m_singlezhushu = 0;
					CRect rcInvali;
					rcInvali.CopyRect(rcZongZhuShu);
					rcInvali.right+=400;
					InvalidateRect(&rcInvali);
					InvalidateRect(&rcZongJinE);

					return m_singlezhushu;
				}
			}
		}

		m_singlezhushu = 1;


	}
	else if(WF_DiLiu == m_gameKind)				//������
	{
		int nLength = strLiu.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;

	}
	else if (WF_QianQi == m_gameKind) //ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;

		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1) || (nQiLength != 1))
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		CString str[7] = {strWan,strQian,strBai,strShi,strGe,strLiu,strQi};
		CString strTemp;
		for(int i = 0;i < 7;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 7;j++)
			{
				if(strTemp == str[j])
				{
					m_singlezhushu = 0;
					CRect rcInvali;
					rcInvali.CopyRect(rcZongZhuShu);
					rcInvali.right+=400;
					InvalidateRect(&rcInvali);
					InvalidateRect(&rcZongJinE);

					return m_singlezhushu;
				}
			}
		}

		m_singlezhushu = 1;


	}
	else if(WF_DiQi == m_gameKind)				//������
	{
		int nLength = strQi.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;

	}
	else if (WF_QianBa == m_gameKind) //ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;
		int nBaLength = strBa.GetLength()/2;

		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1) || (nQiLength != 1) || (nBaLength != 1))
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		CString str[8] = {strWan,strQian,strBai,strShi,strGe,strLiu,strQi,strBa};
		CString strTemp;
		for(int i = 0;i < 8;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 8;j++)
			{
				if(strTemp == str[j])
				{
					m_singlezhushu = 0;
					CRect rcInvali;
					rcInvali.CopyRect(rcZongZhuShu);
					rcInvali.right+=400;
					InvalidateRect(&rcInvali);
					InvalidateRect(&rcZongJinE);

					return m_singlezhushu;
				}
			}
		}

		m_singlezhushu = 1;


	}
	else if(WF_DiBa == m_gameKind)				//�ڰ���
	{
		int nLength = strBa.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;

	}
	else if (WF_QianJiu == m_gameKind) //ǰ����
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;
		int nBaLength = strBa.GetLength()/2;
		int nJiuLength = strJiu.GetLength()/2;

		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1) || (nQiLength != 1) || (nBaLength != 1) || (nJiuLength != 1))
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		CString str[9] = {strWan,strQian,strBai,strShi,strGe,strLiu,strQi,strBa,strJiu};
		CString strTemp;
		for(int i = 0;i < 9;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 9;j++)
			{
				if(strTemp == str[j])
				{
					m_singlezhushu = 0;
					CRect rcInvali;
					rcInvali.CopyRect(rcZongZhuShu);
					rcInvali.right+=400;
					InvalidateRect(&rcInvali);
					InvalidateRect(&rcZongJinE);

					return m_singlezhushu;
				}
			}
		}

		m_singlezhushu = 1;


	}
	else if(WF_DiJiu == m_gameKind)				//�ھ���
	{
		int nLength = strJiu.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;

	}
	else if (WF_QianShi == m_gameKind) //ǰʮ��
	{
		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;
		int nBaLength = strBa.GetLength()/2;
		int nJiuLength = strJiu.GetLength()/2;
		int nShiLength = strShi10.GetLength()/2;

		if ((nGuanLength != 1 ) || (nYaLength != 1) || (nSanLength != 1) || (nSiLength != 1) || (nWuLength != 1) || (nLiuLength != 1) || (nQiLength != 1) || (nBaLength != 1) || (nJiuLength != 1) || (nShiLength != 1))
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		CString str[10] = {strWan,strQian,strBai,strShi,strGe,strLiu,strQi,strBa,strJiu,strShi10};
		CString strTemp;
		for(int i = 0;i < 10;i++)
		{
			strTemp = str[i];
			for(int j = i+1;j < 10;j++)
			{
				if(strTemp == str[j])
				{
					m_singlezhushu = 0;
					CRect rcInvali;
					rcInvali.CopyRect(rcZongZhuShu);
					rcInvali.right+=400;
					InvalidateRect(&rcInvali);
					InvalidateRect(&rcZongJinE);

					return m_singlezhushu;
				}
			}
		}

		m_singlezhushu = 1;


	}
	else if(WF_DiShi == m_gameKind)				//��ʮ��
	{
		int nLength = strShi10.GetLength();
		if(nLength<2 || nLength > 16 || nLength%2 !=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}

		m_singlezhushu = nLength/2;

	}
	else if(WF_DXDS == m_gameKind)				//��С��˫
	{
		CString strDXDSGuan = GetGuanDxdshString();
		CString strDXDSYa = GetYaDxdshString();
		CString strDXDSSan = GetSanDxdshString();
		CString strDXDSSi = GetSiDxdshString();
		CString strDXDSWu = GetWuDxdshString();
		CString strDXDSLiu = GetLiuDxdshString();
		CString strDXDSQi = GetQiDxdshString();
		CString strDXDSBa = GetBaDxdshString();
		CString strDXDSJiu = GetJiuDxdshString();
		CString strDXDSShi = GetShiDxdshString();

		int nGuanLength = strDXDSGuan.GetLength();
		int nYaLength = strDXDSYa.GetLength();
		int nSanLength = strDXDSSan.GetLength();
		int nSiLength = strDXDSSi.GetLength();
		int nWuLength = strDXDSWu.GetLength();
		int nLiuLength = strDXDSLiu.GetLength();
		int nQiLength = strDXDSQi.GetLength();
		int nBaLength = strDXDSBa.GetLength();
		int nJiuLength = strDXDSJiu.GetLength();
		int nShiLength = strDXDSShi.GetLength();

		if(nGuanLength <= 0 && nYaLength <= 0 && nSanLength <= 0 && nSiLength <= 0 && nWuLength <= 0 && nLiuLength <= 0 && nQiLength <= 0 && nBaLength <= 0 && nJiuLength <= 0 && nShiLength <= 0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		else if(nGuanLength > 1 || nYaLength > 1 || nSanLength > 1 || nSiLength > 1 || nWuLength > 1 || nLiuLength > 1 || nQiLength > 1 || nBaLength > 1 || nJiuLength > 1 || nShiLength > 1)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = nGuanLength + nYaLength + nSanLength + nSiLength + nWuLength + nLiuLength + nQiLength + nBaLength + nJiuLength + nShiLength;
	}
	else if(m_gameKind == WF_DWD)
	{

		int nGuanLength = strWan.GetLength()/2;
		int nYaLength = strQian.GetLength()/2;
		int nSanLength = strBai.GetLength()/2;
		int nSiLength = strShi.GetLength()/2;
		int nWuLength = strGe.GetLength()/2;
		int nLiuLength = strLiu.GetLength()/2;
		int nQiLength = strQi.GetLength()/2;
		int nBaLength = strBa.GetLength()/2;
		int nJiuLength = strJiu.GetLength()/2;
		int nShiLength = strShi10.GetLength()/2;

		if(nGuanLength <= 0 && nYaLength <= 0 && nSanLength <= 0 && nSiLength <= 0 && nWuLength <= 0 && nLiuLength <= 0 && nQiLength <= 0 && nBaLength <= 0 && nJiuLength <= 0 && nShiLength <= 0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);
		}

		m_singlezhushu = nGuanLength + nYaLength + nSanLength + nSiLength + nWuLength + nLiuLength + nQiLength + nBaLength + nJiuLength + nShiLength;

	}

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	return m_singlezhushu;
}


void CBeiJingPK10::OnBnClickedBtnWanQuan()
{
	CheckNumBtn(m_btnWan_0);
	CheckNumBtn(m_btnWan_1);
	CheckNumBtn(m_btnWan_2);
	CheckNumBtn(m_btnWan_3);
	CheckNumBtn(m_btnWan_4);
	CheckNumBtn(m_btnWan_5);
	CheckNumBtn(m_btnWan_6);
	CheckNumBtn(m_btnWan_7);
	CheckNumBtn(m_btnWan_8);
	CheckNumBtn(m_btnWan_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnWanDa()
{
	UnCheckNumBtn(m_btnWan_0);
	UnCheckNumBtn(m_btnWan_1);
	UnCheckNumBtn(m_btnWan_2);
	UnCheckNumBtn(m_btnWan_3);
	UnCheckNumBtn(m_btnWan_4);
	CheckNumBtn(m_btnWan_5);
	CheckNumBtn(m_btnWan_6);
	CheckNumBtn(m_btnWan_7);
	CheckNumBtn(m_btnWan_8);
	CheckNumBtn(m_btnWan_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);

}

void CBeiJingPK10::OnBnClickedBtnWanXiao()
{
	CheckNumBtn(m_btnWan_0);
	CheckNumBtn(m_btnWan_1);
	CheckNumBtn(m_btnWan_2);
	CheckNumBtn(m_btnWan_3);
	CheckNumBtn(m_btnWan_4);
	UnCheckNumBtn(m_btnWan_5);
	UnCheckNumBtn(m_btnWan_6);
	UnCheckNumBtn(m_btnWan_7);
	UnCheckNumBtn(m_btnWan_8);
	UnCheckNumBtn(m_btnWan_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);

}

void CBeiJingPK10::OnBnClickedBtnWanDan()
{
	CheckNumBtn(m_btnWan_0);
	UnCheckNumBtn(m_btnWan_1);
	CheckNumBtn(m_btnWan_2);
	UnCheckNumBtn(m_btnWan_3);
	CheckNumBtn(m_btnWan_4);
	UnCheckNumBtn(m_btnWan_5);
	CheckNumBtn(m_btnWan_6);
	UnCheckNumBtn(m_btnWan_7);
	CheckNumBtn(m_btnWan_8);
	UnCheckNumBtn(m_btnWan_9);

	GetPk10Zhushu(CZ_PK10,m_gameKind);

}

void CBeiJingPK10::OnBnClickedBtnWanShuang()
{
	UnCheckNumBtn(m_btnWan_0);
	CheckNumBtn(m_btnWan_1);
	UnCheckNumBtn(m_btnWan_2);
	CheckNumBtn(m_btnWan_3);
	UnCheckNumBtn(m_btnWan_4);
	CheckNumBtn(m_btnWan_5);
	UnCheckNumBtn(m_btnWan_6);
	CheckNumBtn(m_btnWan_7);
	UnCheckNumBtn(m_btnWan_8);
	CheckNumBtn(m_btnWan_9);


	GetPk10Zhushu(CZ_PK10,m_gameKind);

}

void CBeiJingPK10::OnBnClickedBtnWanQing()
{
	UnCheckNumBtn(m_btnWan_0);
	UnCheckNumBtn(m_btnWan_1);
	UnCheckNumBtn(m_btnWan_2);
	UnCheckNumBtn(m_btnWan_3);
	UnCheckNumBtn(m_btnWan_4);
	UnCheckNumBtn(m_btnWan_5);
	UnCheckNumBtn(m_btnWan_6);
	UnCheckNumBtn(m_btnWan_7);
	UnCheckNumBtn(m_btnWan_8);
	UnCheckNumBtn(m_btnWan_9);

	GetPk10Zhushu(CZ_PK10,m_gameKind);

}

void CBeiJingPK10::OnBnClickedBtnQianQuan()
{
	CheckNumBtn(m_btnQian_0);
	CheckNumBtn(m_btnQian_1);
	CheckNumBtn(m_btnQian_2);
	CheckNumBtn(m_btnQian_3);
	CheckNumBtn(m_btnQian_4);
	CheckNumBtn(m_btnQian_5);
	CheckNumBtn(m_btnQian_6);
	CheckNumBtn(m_btnQian_7);
	CheckNumBtn(m_btnQian_8);
	CheckNumBtn(m_btnQian_9);

	GetPk10Zhushu(CZ_PK10,m_gameKind);

}

void CBeiJingPK10::OnBnClickedBtnQianDa()
{
	UnCheckNumBtn(m_btnQian_0);
	UnCheckNumBtn(m_btnQian_1);
	UnCheckNumBtn(m_btnQian_2);
	UnCheckNumBtn(m_btnQian_3);
	UnCheckNumBtn(m_btnQian_4);
	CheckNumBtn(m_btnQian_5);
	CheckNumBtn(m_btnQian_6);
	CheckNumBtn(m_btnQian_7);
	CheckNumBtn(m_btnQian_8);
	CheckNumBtn(m_btnQian_9);

	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnQianXiao()
{
	CheckNumBtn(m_btnQian_0);
	CheckNumBtn(m_btnQian_1);
	CheckNumBtn(m_btnQian_2);
	CheckNumBtn(m_btnQian_3);
	CheckNumBtn(m_btnQian_4);
	UnCheckNumBtn(m_btnQian_5);
	UnCheckNumBtn(m_btnQian_6);
	UnCheckNumBtn(m_btnQian_7);
	UnCheckNumBtn(m_btnQian_8);
	UnCheckNumBtn(m_btnQian_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnQianDan()
{
	CheckNumBtn(m_btnQian_0);
	UnCheckNumBtn(m_btnQian_1);
	CheckNumBtn(m_btnQian_2);
	UnCheckNumBtn(m_btnQian_3);
	CheckNumBtn(m_btnQian_4);
	UnCheckNumBtn(m_btnQian_5);
	CheckNumBtn(m_btnQian_6);
	UnCheckNumBtn(m_btnQian_7);
	CheckNumBtn(m_btnQian_8);
	UnCheckNumBtn(m_btnQian_9);

	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnQianShuang()
{

	UnCheckNumBtn(m_btnQian_0);
	CheckNumBtn(m_btnQian_1);
	UnCheckNumBtn(m_btnQian_2);
	CheckNumBtn(m_btnQian_3);
	UnCheckNumBtn(m_btnQian_4);
	CheckNumBtn(m_btnQian_5);
	UnCheckNumBtn(m_btnQian_6);
	CheckNumBtn(m_btnQian_7);
	UnCheckNumBtn(m_btnQian_8);
	CheckNumBtn(m_btnQian_9);

	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnQianQing()
{
	UnCheckNumBtn(m_btnQian_0);
	UnCheckNumBtn(m_btnQian_1);
	UnCheckNumBtn(m_btnQian_2);
	UnCheckNumBtn(m_btnQian_3);
	UnCheckNumBtn(m_btnQian_4);
	UnCheckNumBtn(m_btnQian_5);
	UnCheckNumBtn(m_btnQian_6);
	UnCheckNumBtn(m_btnQian_7);
	UnCheckNumBtn(m_btnQian_8);
	UnCheckNumBtn(m_btnQian_9);

	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnBaiQuan()
{
	CheckNumBtn(m_btnBai_0);
	CheckNumBtn(m_btnBai_1);
	CheckNumBtn(m_btnBai_2);
	CheckNumBtn(m_btnBai_3);
	CheckNumBtn(m_btnBai_4);
	CheckNumBtn(m_btnBai_5);
	CheckNumBtn(m_btnBai_6);
	CheckNumBtn(m_btnBai_7);
	CheckNumBtn(m_btnBai_8);
	CheckNumBtn(m_btnBai_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnBaiDa()
{
	UnCheckNumBtn(m_btnBai_0);
	UnCheckNumBtn(m_btnBai_1);
	UnCheckNumBtn(m_btnBai_2);
	UnCheckNumBtn(m_btnBai_3);
	UnCheckNumBtn(m_btnBai_4);
	CheckNumBtn(m_btnBai_5);
	CheckNumBtn(m_btnBai_6);
	CheckNumBtn(m_btnBai_7);
	CheckNumBtn(m_btnBai_8);
	CheckNumBtn(m_btnBai_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnBaiXiao()
{
	CheckNumBtn(m_btnBai_0);
	CheckNumBtn(m_btnBai_1);
	CheckNumBtn(m_btnBai_2);
	CheckNumBtn(m_btnBai_3);
	CheckNumBtn(m_btnBai_4);
	UnCheckNumBtn(m_btnBai_5);
	UnCheckNumBtn(m_btnBai_6);
	UnCheckNumBtn(m_btnBai_7);
	UnCheckNumBtn(m_btnBai_8);
	UnCheckNumBtn(m_btnBai_9);

	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnBaiDan()
{
	CheckNumBtn(m_btnBai_0);
	UnCheckNumBtn(m_btnBai_1);
	CheckNumBtn(m_btnBai_2);
	UnCheckNumBtn(m_btnBai_3);
	CheckNumBtn(m_btnBai_4);
	UnCheckNumBtn(m_btnBai_5);
	CheckNumBtn(m_btnBai_6);
	UnCheckNumBtn(m_btnBai_7);
	CheckNumBtn(m_btnBai_8);
	UnCheckNumBtn(m_btnBai_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnBaiShuang()
{
	UnCheckNumBtn(m_btnBai_0);
	CheckNumBtn(m_btnBai_1);
	UnCheckNumBtn(m_btnBai_2);
	CheckNumBtn(m_btnBai_3);
	UnCheckNumBtn(m_btnBai_4);
	CheckNumBtn(m_btnBai_5);
	UnCheckNumBtn(m_btnBai_6);
	CheckNumBtn(m_btnBai_7);
	UnCheckNumBtn(m_btnBai_8);
	CheckNumBtn(m_btnBai_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnBaiQing()
{
	UnCheckNumBtn(m_btnBai_0);
	UnCheckNumBtn(m_btnBai_1);
	UnCheckNumBtn(m_btnBai_2);
	UnCheckNumBtn(m_btnBai_3);
	UnCheckNumBtn(m_btnBai_4);
	UnCheckNumBtn(m_btnBai_5);
	UnCheckNumBtn(m_btnBai_6);
	UnCheckNumBtn(m_btnBai_7);
	UnCheckNumBtn(m_btnBai_8);
	UnCheckNumBtn(m_btnBai_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnShiQuan()
{
	CheckNumBtn(m_btnShi_0);
	CheckNumBtn(m_btnShi_1);
	CheckNumBtn(m_btnShi_2);
	CheckNumBtn(m_btnShi_3);
	CheckNumBtn(m_btnShi_4);
	CheckNumBtn(m_btnShi_5);
	CheckNumBtn(m_btnShi_6);
	CheckNumBtn(m_btnShi_7);
	CheckNumBtn(m_btnShi_8);
	CheckNumBtn(m_btnShi_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnShiDa()
{
	UnCheckNumBtn(m_btnShi_0);
	UnCheckNumBtn(m_btnShi_1);
	UnCheckNumBtn(m_btnShi_2);
	UnCheckNumBtn(m_btnShi_3);
	UnCheckNumBtn(m_btnShi_4);
	CheckNumBtn(m_btnShi_5);
	CheckNumBtn(m_btnShi_6);
	CheckNumBtn(m_btnShi_7);
	CheckNumBtn(m_btnShi_8);
	CheckNumBtn(m_btnShi_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnShiXiao()
{
	CheckNumBtn(m_btnShi_0);
	CheckNumBtn(m_btnShi_1);
	CheckNumBtn(m_btnShi_2);
	CheckNumBtn(m_btnShi_3);
	CheckNumBtn(m_btnShi_4);
	UnCheckNumBtn(m_btnShi_5);
	UnCheckNumBtn(m_btnShi_6);
	UnCheckNumBtn(m_btnShi_7);
	UnCheckNumBtn(m_btnShi_8);
	UnCheckNumBtn(m_btnShi_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnShiDan()
{
	CheckNumBtn(m_btnShi_0);
	UnCheckNumBtn(m_btnShi_1);
	CheckNumBtn(m_btnShi_2);
	UnCheckNumBtn(m_btnShi_3);
	CheckNumBtn(m_btnShi_4);
	UnCheckNumBtn(m_btnShi_5);
	CheckNumBtn(m_btnShi_6);
	UnCheckNumBtn(m_btnShi_7);
	CheckNumBtn(m_btnShi_8);
	UnCheckNumBtn(m_btnShi_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnShiShuang()
{
	UnCheckNumBtn(m_btnShi_0);
	CheckNumBtn(m_btnShi_1);
	UnCheckNumBtn(m_btnShi_2);
	CheckNumBtn(m_btnShi_3);
	UnCheckNumBtn(m_btnShi_4);
	CheckNumBtn(m_btnShi_5);
	UnCheckNumBtn(m_btnShi_6);
	CheckNumBtn(m_btnShi_7);
	UnCheckNumBtn(m_btnShi_8);
	CheckNumBtn(m_btnShi_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnShiQing()
{
	UnCheckNumBtn(m_btnShi_0);
	UnCheckNumBtn(m_btnShi_1);
	UnCheckNumBtn(m_btnShi_2);
	UnCheckNumBtn(m_btnShi_3);
	UnCheckNumBtn(m_btnShi_4);
	UnCheckNumBtn(m_btnShi_5);
	UnCheckNumBtn(m_btnShi_6);
	UnCheckNumBtn(m_btnShi_7);
	UnCheckNumBtn(m_btnShi_8);
	UnCheckNumBtn(m_btnShi_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnGeQuan()
{
	CheckNumBtn(m_btnGe_0);
	CheckNumBtn(m_btnGe_1);
	CheckNumBtn(m_btnGe_2);
	CheckNumBtn(m_btnGe_3);
	CheckNumBtn(m_btnGe_4);
	CheckNumBtn(m_btnGe_5);
	CheckNumBtn(m_btnGe_6);
	CheckNumBtn(m_btnGe_7);
	CheckNumBtn(m_btnGe_8);
	CheckNumBtn(m_btnGe_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnGeDa()
{
	UnCheckNumBtn(m_btnGe_0);
	UnCheckNumBtn(m_btnGe_1);
	UnCheckNumBtn(m_btnGe_2);
	UnCheckNumBtn(m_btnGe_3);
	UnCheckNumBtn(m_btnGe_4);
	CheckNumBtn(m_btnGe_5);
	CheckNumBtn(m_btnGe_6);
	CheckNumBtn(m_btnGe_7);
	CheckNumBtn(m_btnGe_8);
	CheckNumBtn(m_btnGe_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnGeXiao()
{
	CheckNumBtn(m_btnGe_0);
	CheckNumBtn(m_btnGe_1);
	CheckNumBtn(m_btnGe_2);
	CheckNumBtn(m_btnGe_3);
	CheckNumBtn(m_btnGe_4);
	UnCheckNumBtn(m_btnGe_5);
	UnCheckNumBtn(m_btnGe_6);
	UnCheckNumBtn(m_btnGe_7);
	UnCheckNumBtn(m_btnGe_8);
	UnCheckNumBtn(m_btnGe_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnGeDan()
{
	CheckNumBtn(m_btnGe_0);
	UnCheckNumBtn(m_btnGe_1);
	CheckNumBtn(m_btnGe_2);
	UnCheckNumBtn(m_btnGe_3);
	CheckNumBtn(m_btnGe_4);
	UnCheckNumBtn(m_btnGe_5);
	CheckNumBtn(m_btnGe_6);
	UnCheckNumBtn(m_btnGe_7);
	CheckNumBtn(m_btnGe_8);
	UnCheckNumBtn(m_btnGe_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnGeShuang()
{
	UnCheckNumBtn(m_btnGe_0);
	CheckNumBtn(m_btnGe_1);
	UnCheckNumBtn(m_btnGe_2);
	CheckNumBtn(m_btnGe_3);
	UnCheckNumBtn(m_btnGe_4);
	CheckNumBtn(m_btnGe_5);
	UnCheckNumBtn(m_btnGe_6);
	CheckNumBtn(m_btnGe_7);
	UnCheckNumBtn(m_btnGe_8);
	CheckNumBtn(m_btnGe_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnBnClickedBtnGeQing()
{
	UnCheckNumBtn(m_btnGe_0);
	UnCheckNumBtn(m_btnGe_1);
	UnCheckNumBtn(m_btnGe_2);
	UnCheckNumBtn(m_btnGe_3);
	UnCheckNumBtn(m_btnGe_4);
	UnCheckNumBtn(m_btnGe_5);
	UnCheckNumBtn(m_btnGe_6);
	UnCheckNumBtn(m_btnGe_7);
	UnCheckNumBtn(m_btnGe_8);
	UnCheckNumBtn(m_btnGe_9);
	GetPk10Zhushu(CZ_PK10,m_gameKind);
}

void CBeiJingPK10::OnEnChangeRichEditDanshiInput()
{
	m_singlezhushu = GetDanShiZhusu(m_gameKind);


	return ;
}

int CBeiJingPK10::GetDanShiZhusu(int nKind)
{
	m_singlezhushu = 0;

	if(m_gameKind == WF_QianWu||m_gameKind == WF_QianSi||m_gameKind == WF_QianSan||m_gameKind == WF_GuanYaJun)
	{
		int nSize = 0;
		if(m_gameKind == WF_GuanYaJun)
			nSize = 2;
		else if(m_gameKind == WF_QianSan)
			nSize = 3;
		else if(m_gameKind == WF_QianSi)
			nSize = 4;
		else if(m_gameKind == WF_QianWu)
			nSize = 5;
		CString strHaoMa;
		CString strLog;
		strLog.Format(L"TICKCOUNT1 :%d",GetTickCount());
		OutputDebugString(strLog);

		if (!GetDanShiHaoma(strHaoMa,10240,nSize))
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=200;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		strLog.Format(L"TICKCOUNT2 :%d",GetTickCount());
		OutputDebugString(strLog);

		string sHaoma;
		ChangeStringToA(strHaoMa,sHaoma);
		vector<string> vectHaoma;
		SplitString1(sHaoma,vectHaoma);
		m_singlezhushu = vectHaoma.size()-1;

	}
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=200;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
	return m_singlezhushu;
}
