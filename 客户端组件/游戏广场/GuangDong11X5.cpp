#include "stdafx.h"
#include "GamePlaza.h"
#include "GuangDong11X5.h"
#include "BmpUtil.h"
#include "MessageDlg.h"
#include "DlgAddtional.h"
#include "DlgFandian.h"
#include "GDIpng/MemDC.h"
#include "PlatformFrame.h"
#include <functional> 
CString   CGuangDong11X5::m_strHaoma = _T("");
using		namespace		std;
#include <map>
extern map <int,CString> mapFandian;
extern map <int,float> mapPercent;
extern map <int,float> mapDanweiFandian;
extern map <int,float> mapBonus;

#define      MAXN			100

 extern vector<wstring>	vecData1, vecData2, vecData3;
 extern vector<wstring> vecAllNums;
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
 static const int wanfa_sel_width = 77;
 static const int wanfa_sel_height = 22;

 //��������
 static const int haoma_btn_x = 65-30;
 static const int haoma_btn_y = 226-18;
 static const int haoma_btn_col_wdith = 40;
 static const int haoma_btn_row_height = 35;

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
 static const int edit_beitou_x = 182+19;
 static const int edit_beitou_y = 423+2;
 static const int edit_beitou_width = 36;
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
 static const int btn_more_x = 712+50;
 static const int btn_more_y = 92-90;

 //�󿪽���
 static const int kj_big_haoma_x = 221-20;
 static const int kj_big_haoma_y = 39;

 //static const int czh_rc_left = 223;
 //static const int czh_rc_top = 4;

 static const int czh_rc_left = 243-35;
 static const int czh_rc_top = 4-5;
 static const int czh_rc_width = 85;
 static const int czh_rc_height = 30;
 static CRect rcCzh(czh_rc_left, czh_rc_top, czh_rc_left + czh_rc_width, czh_rc_top + czh_rc_height);

 static const int qihao_rc_left = 335-45+20;
 static const int qihao_rc_top = 4-5;
 static const int qihao_rc_width = 130;
 static const int qihao_rc_height = 30;
 static CRect rcQiHao(qihao_rc_left, qihao_rc_top, qihao_rc_left + qihao_rc_width, qihao_rc_top + qihao_rc_height);

 static const int yugao_rc_left = 3;
 static const int yugao_rc_top = 4;
 static const int yugao_rc_width = 196;
 static const int yugao_rc_height = 30;
 static CRect rcYuGao(yugao_rc_left, yugao_rc_top, yugao_rc_left + yugao_rc_width, yugao_rc_top + yugao_rc_height);

 //С������
 static const int kj_small_haoma_x = 630-25;
 static const int kj_small_haoma_y = 28;
 static const int kj_small_haoma_col_span = 5;
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
 static const int redraw_rc_width = 756;
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
static const int TimerLotteryStatus = 5;


IMPLEMENT_DYNAMIC(CGuangDong11X5, CDialog)

CGuangDong11X5::CGuangDong11X5(CWnd* pParent /*=NULL*/)
	: CDialog(CGuangDong11X5::IDD, pParent)
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
	, m_gameKind(IIRenXuan2)
	, m_zongZhuShu(0)
	//, m_zongJinE(0.0)
	, m_beishu(1)
	, m_kjXinxiCont(0)
{
	m_nTzhSign = 1;
	m_nTzhSign1 = 0;

	m_nFrameIndex=0;
	m_nFrameCount=0;
	for(int j  =0 ;j < 50;j++)
	{
		for (int i=0; i<Kj_XinXi_Count; i++) 
		{
			memset(&m_kjXinxi[j][i], 0, sizeof(KjXinXi));
		}

	}

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
	m_TypeID = CZGD11Xuan5;
	m_nZhuihaoCount = 0;
	m_bCanSend = true;
	m_bZhuihao=false;
	//�������
	m_DlgStatus.SetStatusViewSink(this);
}

CGuangDong11X5::~CGuangDong11X5()
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
	//	SafeDelete(m_imgKaijiang);
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

void CGuangDong11X5::FlushZongjine()
{
	InvalidateRect(&rcZongJinE);
}

void CGuangDong11X5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TIP_WXZX,m_btWxzxTip);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIA,m_btnBeiTouJia);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIAN,m_btnBeiTouJian);

	DDX_Control(pDX, IDC_BTN_DANTUO, m_btnDanTuo);
	DDX_Control(pDX, IDC_BTN_HSZHX, m_btnHouSanZhiXuan);
	DDX_Control(pDX, IDC_BTN_QSZHX, m_btnQianSanZhiXuan);
	DDX_Control(pDX, IDC_BTN_HSZX, m_btnHouSanZuXuan);
	DDX_Control(pDX, IDC_BTN_QSZX, m_btnQianSanZuXuan);
	DDX_Control(pDX, IDC_BTN_WXHE, m_btnWuXingHouEr);
	DDX_Control(pDX, IDC_BTN_WXQE, m_btnWuXingQianEr);
	DDX_Control(pDX, IDC_BTN_HEZX, m_btnHouErZuXuan);
	DDX_Control(pDX, IDC_BTN_QEZX, m_btnQianErZuXuan);
	DDX_Control(pDX, IDC_BTN_DXDSH, m_btnDaXiaoDanShuang);
	//DDX_Control(pDX, IDC_BTN_HSZHXHZH, m_btnHouSanHeZhi);
	DDX_Control(pDX, IDC_BTN_BDW, m_btnBuDingWei);
	//DDX_Control(pDX, IDC_BTN_DWD, m_btnDingWeiDan);

	DDX_Control(pDX, IDC_BTN_LOCK, m_btnLock);

	DDX_Control(pDX, IDC_RIO_RERDT,m_rioRen2DanTuo);
	DDX_Control(pDX, IDC_RIO_RSANDT,m_rioRen3DanTuo);
	DDX_Control(pDX, IDC_RIO_RSIDT,m_rioRen4DanTuo);
	DDX_Control(pDX, IDC_RIO_RWUDT,m_rioRen5DanTuo);
	DDX_Control(pDX, IDC_RIO_RLIUDT,m_rioRen6DanTuo);
	DDX_Control(pDX, IDC_RIO_RQIDT,m_rioRen7DanTuo);
	DDX_Control(pDX, IDC_RIO_RBADT,m_rioRen8DanTuo);

	DDX_Control(pDX, IDC_BTN_RENXUAN1, m_btnRenXuan1);
	DDX_Control(pDX, IDC_RIO_RX1_FUSHI, m_rioRenXuan1);
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
	//DDX_Control(pDX, IDC_RIO_HSZS_DANSHI, m_rioHsZsDanShi);
	DDX_Control(pDX, IDC_RIO_HSZL_FUSHI, m_rioHsZlFuShi);
	//DDX_Control(pDX, IDC_RIO_HSZL_DANSHI, m_rioHsZlDanShi);
	DDX_Control(pDX, IDC_RIO_QSZS_FUSHI, m_rioQsZsFuShi);
	//DDX_Control(pDX, IDC_RIO_QSZS_DANSHI, m_rioQsZsDanShi);
	DDX_Control(pDX, IDC_RIO_QSZL_FUSHI, m_rioQsZlFuShi);
	//DDX_Control(pDX, IDC_RIO_QSZL_DANSHI, m_rioQsZlDanShi);
	DDX_Control(pDX, IDC_RIO_HEZHX_FUSHI, m_rioHeZhxFuShi);
	DDX_Control(pDX, IDC_RIO_HEZHX_DANSHI, m_rioHeZhxDanShi);
	DDX_Control(pDX, IDC_RIO_QEZHX_FUSHI, m_rioQeZhxFuShi);
	DDX_Control(pDX, IDC_RIO_QEZHX_DANSHI, m_rioQeZhxDanShi);
	DDX_Control(pDX, IDC_RIO_HEZX_FUSHI, m_rioHeZxFuShi);
	DDX_Control(pDX, IDC_RIO_HEZX_DANSHI, m_rioHeZxDanShi);
	DDX_Control(pDX, IDC_RIO_QEZX_FUSHI, m_rioQeZxFuShi);
	DDX_Control(pDX, IDC_RIO_QEZX_DANSHI, m_rioQeZxDanShi);
	DDX_Control(pDX, IDC_RIO_DXDSH, m_rioQeDxdsh);
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
	DDX_Control(pDX, IDC_BTN_WAN_10, m_btnWan_10);

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
	DDX_Control(pDX, IDC_BTN_QIAN_10, m_btnQian_10);

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
	DDX_Control(pDX, IDC_BTN_BAI_10, m_btnBai_10);

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
	DDX_Control(pDX, IDC_BTN_SHI_10, m_btnShi_10);

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
	DDX_Control(pDX, IDC_BTN_GE_10, m_btnGe_10);
	DDX_Text(pDX, IDC_EDIT_BEISHU, m_beishu);
	

	DDX_Control(pDX, IDC_EDIT_DANSHI_HAOMA, m_editDanShiHaoMa);
	DDX_Control(pDX, IDC_RICHEDIT_DANSHI, m_richDanshiHaoma);

	DDX_Control(pDX, IDC_STATIC_WAN, m_staticWan);
	DDX_Control(pDX, IDC_STATIC_QIAN, m_staticQian);
	DDX_Control(pDX, IDC_STATIC_BAI, m_staticBai);
	DDX_Control(pDX, IDC_STATIC_SHI, m_staticShi);
	DDX_Control(pDX, IDC_STATIC_GE, m_staticGe);

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

	DDX_Control(pDX, IDC_LIST_CHQSSC, m_listChqSSC);
	DDX_Control(pDX, IDC_BTN_CHQ_ADD, m_btnChqAdd);
	DDX_Control(pDX, IDC_BTN_DEL_SEL, m_btnDelSel);
	DDX_Control(pDX, IDC_BTN_CLS_LIST, m_btnClsList);
	DDX_Control(pDX, IDC_BTN_TOUZHU, m_btnTouZhu);
	DDX_Control(pDX, IDC_BTN_ZHUIHAO, m_btnZhuiHao);
	DDX_Control(pDX, IDC_EDIT_BEISHU, m_editBeiTou);
	//DDX_Control(pDX, IDC_STATIC_DNASHI, m_staticDanShi);


	DDX_Control(pDX, IDC_RIO_QSHUNHE, m_rioQsHunhe);
	DDX_Control(pDX, IDC_RIO_ZSHUNHE, m_rioZsHunhe);
	DDX_Control(pDX, IDC_RIO_HSHUNHE, m_rioHsHunhe);
	DDX_Control(pDX, IDC_STATIC_DANMA, m_staticDanMa);
	DDX_Control(pDX, IDC_STATIC_TUOMA, m_staticTuoMa);

	DDX_Control(pDX, IDC_STATIC_ZUXUAN, m_staticZuXuan);
	DDX_Control(pDX, IDC_STATIC_BUDINGWEI, m_staticBuDingWei);
	DDX_Control(pDX, IDC_STATIC_HUNHE_TIP, m_staticHunheTip);
	DDX_Control(pDX, IDC_BTN_DA_SHI, m_btnDaShi);
	DDX_Control(pDX, IDC_BTN_XIAO_SHI, m_btnXiaoShi);
	DDX_Control(pDX, IDC_BTN_DAN_SHI, m_btnDanShi);
	DDX_Control(pDX, IDC_BTN_SHUANG_SHI, m_btnShuangShi);
	DDX_Control(pDX, IDC_BTN_DA_GE, m_btnDaGe);
	DDX_Control(pDX, IDC_BTN_XIAO_GE, m_btnXiaoGe);
	DDX_Control(pDX, IDC_BTN_DAN_GE, m_btnDanGe);
	DDX_Control(pDX, IDC_BTN_SHUANG_GE, m_btnShuangGe);
	DDX_Control(pDX, IDC_BTN_MORE_RECORD, m_btnMoreRecord);
	DDX_Control(pDX, IDC_BTN_QUSHI, m_btnQushi);

	DDX_Control(pDX, IDC_BTN_YUAN,m_btnYuan);
	DDX_Control(pDX, IDC_BTN_JIAO,m_btnJiao);
	DDX_Control(pDX, IDC_BTN_FEN,m_btnFen);
	DDX_Control(pDX, IDC_BTN_LI,m_btnLi);
	DDX_Control(pDX, IDC_BTN_TOUZHU_ONEKEY, m_btnTouZhuOnekey);
}

BEGIN_MESSAGE_MAP(CGuangDong11X5, CDialog)
	ON_BN_CLICKED(IDC_BTN_YUAN, &CGuangDong11X5::OnBnClickedBtnYuan)
	ON_BN_CLICKED(IDC_BTN_JIAO, &CGuangDong11X5::OnBnClickedBtnJiao)
	ON_BN_CLICKED( IDC_BTN_FEN, &CGuangDong11X5::OnBnClickedBtnFen)
	ON_BN_CLICKED(IDC_BTN_LI, &CGuangDong11X5::OnBnClickedBtnLi)
	ON_BN_CLICKED(IDC_BTN_DANTUO, &CGuangDong11X5::OnBnClickedBtnDanTuo)
	ON_BN_CLICKED(IDC_BTN_RENXUAN1, &CGuangDong11X5::OnBnClickedBtnRx1)
	ON_BN_CLICKED(IDC_BTN_HSZHX, &CGuangDong11X5::OnBnClickedBtnHszhx)
	ON_BN_CLICKED(IDC_BTN_HSZX, &CGuangDong11X5::OnBnClickedBtnHszx)
	ON_BN_CLICKED(IDC_BTN_WXHE, &CGuangDong11X5::OnBnClickedBtnWxhe)
	ON_BN_CLICKED(IDC_BTN_HEZX, &CGuangDong11X5::OnBnClickedBtnHezx)
	ON_BN_CLICKED(IDC_BTN_DXDSH, &CGuangDong11X5::OnBnClickedBtnDxdsh)
	ON_BN_CLICKED(IDC_BTN_BDW, &CGuangDong11X5::OnBnClickedBtnBdw)
	ON_BN_CLICKED(IDC_BTN_QSZHX, &CGuangDong11X5::OnBnClickedBtnQszhx)
	ON_BN_CLICKED(IDC_BTN_QSZX, &CGuangDong11X5::OnBnClickedBtnQszx)
	ON_BN_CLICKED(IDC_BTN_WXQE, &CGuangDong11X5::OnBnClickedBtnWxqe)
	ON_BN_CLICKED(IDC_BTN_QEZX, &CGuangDong11X5::OnBnClickedBtnQezx)
	ON_BN_CLICKED(IDC_BTN_HSZHXHZH, &CGuangDong11X5::OnBnClickedBtnHszhxhzh)
	ON_BN_CLICKED(IDC_BTN_DWD, &CGuangDong11X5::OnBnClickedBtnDwd)
	ON_BN_CLICKED(IDC_BTN_WAN_QUAN, &CGuangDong11X5::OnBnClickedBtnWanQuan)
	ON_BN_CLICKED(IDC_BTN_WAN_DA, &CGuangDong11X5::OnBnClickedBtnWanDa)
	ON_BN_CLICKED(IDC_BTN_WAN_XIAO, &CGuangDong11X5::OnBnClickedBtnWanXiao)
	ON_BN_CLICKED(IDC_BTN_WAN_DAN, &CGuangDong11X5::OnBnClickedBtnWanDan)
	ON_BN_CLICKED(IDC_BTN_WAN_SHUANG, &CGuangDong11X5::OnBnClickedBtnWanShuang)
	ON_BN_CLICKED(IDC_BTN_WAN_QING, &CGuangDong11X5::OnBnClickedBtnWanQing)
	ON_BN_CLICKED(IDC_BTN_QIAN_QUAN, &CGuangDong11X5::OnBnClickedBtnQianQuan)
	ON_BN_CLICKED(IDC_BTN_QIAN_DA, &CGuangDong11X5::OnBnClickedBtnQianDa)
	ON_BN_CLICKED(IDC_BTN_QIAN_XIAO, &CGuangDong11X5::OnBnClickedBtnQianXiao)
	ON_BN_CLICKED(IDC_BTN_QIAN_DAN, &CGuangDong11X5::OnBnClickedBtnQianDan)
	ON_BN_CLICKED(IDC_BTN_QIAN_SHUANG, &CGuangDong11X5::OnBnClickedBtnQianShuang)
	ON_BN_CLICKED(IDC_BTN_QIAN_QING, &CGuangDong11X5::OnBnClickedBtnQianQing)
	ON_BN_CLICKED(IDC_BTN_BAI_QUAN, &CGuangDong11X5::OnBnClickedBtnBaiQuan)
	ON_BN_CLICKED(IDC_BTN_BAI_DA, &CGuangDong11X5::OnBnClickedBtnBaiDa)
	ON_BN_CLICKED(IDC_BTN_BAI_XIAO, &CGuangDong11X5::OnBnClickedBtnBaiXiao)
	ON_BN_CLICKED(IDC_BTN_BAI_DAN, &CGuangDong11X5::OnBnClickedBtnBaiDan)
	ON_BN_CLICKED(IDC_BTN_BAI_SHUANG, &CGuangDong11X5::OnBnClickedBtnBaiShuang)
	ON_BN_CLICKED(IDC_BTN_BAI_QING, &CGuangDong11X5::OnBnClickedBtnBaiQing)
	ON_BN_CLICKED(IDC_BTN_SHI_QUAN, &CGuangDong11X5::OnBnClickedBtnShiQuan)
	ON_BN_CLICKED(IDC_BTN_SHI_DA, &CGuangDong11X5::OnBnClickedBtnShiDa)
	ON_BN_CLICKED(IDC_BTN_SHI_XIAO, &CGuangDong11X5::OnBnClickedBtnShiXiao)
	ON_BN_CLICKED(IDC_BTN_SHI_DAN, &CGuangDong11X5::OnBnClickedBtnShiDan)
	ON_BN_CLICKED(IDC_BTN_SHI_SHUANG, &CGuangDong11X5::OnBnClickedBtnShiShuang)
	ON_BN_CLICKED(IDC_BTN_SHI_QING, &CGuangDong11X5::OnBnClickedBtnShiQing)
	ON_BN_CLICKED(IDC_BTN_GE_QUAN, &CGuangDong11X5::OnBnClickedBtnGeQuan)
	ON_BN_CLICKED(IDC_BTN_GE_DA, &CGuangDong11X5::OnBnClickedBtnGeDa)
	ON_BN_CLICKED(IDC_BTN_GE_XIAO, &CGuangDong11X5::OnBnClickedBtnGeXiao)
	ON_BN_CLICKED(IDC_BTN_GE_DAN, &CGuangDong11X5::OnBnClickedBtnGeDan)
	ON_BN_CLICKED(IDC_BTN_GE_SHUANG, &CGuangDong11X5::OnBnClickedBtnGeShuang)
	ON_BN_CLICKED(IDC_BTN_GE_QING, &CGuangDong11X5::OnBnClickedBtnGeQing)
	ON_BN_CLICKED(IDC_BTN_CHQ_ADD, &CGuangDong11X5::OnBnClickedBtnChqAdd)
	ON_BN_CLICKED(IDC_BTN_TOUZHU_ONEKEY, &CGuangDong11X5::OnBnClickedBtnTouzhuOneKey)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RIO_RX1_FUSHI, &CGuangDong11X5::OnBnClickedRioRenXuan1)
	ON_BN_CLICKED(IDC_RIO_HSZHX_FUSHI, &CGuangDong11X5::OnBnClickedRioHszhxFushi)
	ON_BN_CLICKED(IDC_RIO_RENXUAN2, &CGuangDong11X5::OnBnClickedRioRenXuan2)
	ON_BN_CLICKED(IDC_RIO_HSZS_FUSHI, &CGuangDong11X5::OnBnClickedRioHszsFushi)
	ON_BN_CLICKED(IDC_RIO_QSZS_FUSHI, &CGuangDong11X5::OnBnClickedRioQszsFushi)
	ON_BN_CLICKED(IDC_RIO_HEZHX_FUSHI, &CGuangDong11X5::OnBnClickedRioHezhxFushi)
	ON_BN_CLICKED(IDC_RIO_HEZX_FUSHI, &CGuangDong11X5::OnBnClickedRioHezxFushi)
	ON_BN_CLICKED(IDC_RIO_QEZHX_FUSHI, &CGuangDong11X5::OnBnClickedRioQezhxFushi)
	ON_BN_CLICKED(IDC_RIO_QEZHX_DANSHI, &CGuangDong11X5::OnBnClickedRioQezhxDanshi)
	ON_BN_CLICKED(IDC_RIO_QEZX_FUSHI, &CGuangDong11X5::OnBnClickedRioQezxFushi)
	ON_BN_CLICKED(IDC_RIO_QEZX_DANSHI, &CGuangDong11X5::OnBnClickedRioQezxDanshi)
	ON_BN_CLICKED(IDC_RIO_DXDSH, &CGuangDong11X5::OnBnClickedRioDxdsh)
	ON_BN_CLICKED(IDC_RIO_HSBDW, &CGuangDong11X5::OnBnClickedRioHsbdw)

	ON_BN_CLICKED(IDC_RIO_RERDT, &CGuangDong11X5::OnBnClickedRioRen2DanTuo)
	ON_BN_CLICKED(IDC_RIO_RSANDT, &CGuangDong11X5::OnBnClickedRioRen3DanTuo)
	ON_BN_CLICKED(IDC_RIO_RSIDT, &CGuangDong11X5::OnBnClickedRioRen4DanTuo)
	ON_BN_CLICKED(IDC_RIO_RWUDT, &CGuangDong11X5::OnBnClickedRioRen5DanTuo)
	ON_BN_CLICKED(IDC_RIO_RLIUDT, &CGuangDong11X5::OnBnClickedRioRen6DanTuo)
	ON_BN_CLICKED(IDC_RIO_RQIDT, &CGuangDong11X5::OnBnClickedRioRen7DanTuo)
	ON_BN_CLICKED(IDC_RIO_RBADT, &CGuangDong11X5::OnBnClickedRioRen8DanTuo)
	

	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ZHUIHAO, &CGuangDong11X5::OnBnClickedBtnZhuihao)
	ON_BN_CLICKED(IDC_BTN_TOUZHU, &CGuangDong11X5::OnBnClickedBtnTouzhu)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_DEL_SEL, &CGuangDong11X5::OnBnClickedBtnDelSel)
	ON_BN_CLICKED(IDC_BTN_CLS_LIST, &CGuangDong11X5::OnBnClickedBtnClsList)
	ON_EN_CHANGE(IDC_EDIT_BEISHU, &CGuangDong11X5::OnEnChangeEditBeishu)
	ON_EN_CHANGE(IDC_EDIT_DANSHI_HAOMA, &CGuangDong11X5::OnEnChangeEditDanshiInput)
	ON_WM_CTLCOLOR()
	ON_MESSAGE( WM_BN_CLICK, &CGuangDong11X5::onBnCLick)

	ON_WM_MOUSEMOVE()
 	ON_WM_LBUTTONUP()
 	ON_WM_LBUTTONDOWN()
 	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_BN_CLICKED(IDC_RIO_MOUSE_INPUT, &CGuangDong11X5::OnBnClickedRioMouseInput)
	ON_BN_CLICKED(IDC_RIO_KEYBOARD_INPUT, &CGuangDong11X5::OnBnClickedRioKeyboardInput)
	ON_BN_CLICKED(IDC_CHK_WAN, &CGuangDong11X5::OnBnClickedChkWan)
	ON_BN_CLICKED(IDC_CHK_QIAN, &CGuangDong11X5::OnBnClickedChkQian)
	ON_BN_CLICKED(IDC_CHK_BAI, &CGuangDong11X5::OnBnClickedChkBai)
	ON_BN_CLICKED(IDC_CHK_SHI, &CGuangDong11X5::OnBnClickedChkShi)
	ON_BN_CLICKED(IDC_CHKGE, &CGuangDong11X5::OnBnClickedChkge)
	ON_BN_CLICKED(IDC_BTN_LOCK, &CGuangDong11X5::OnBnClickedBtnLock)

	ON_COMMAND(ID_RBTN_CPY, &CGuangDong11X5::OnRichEditCopy)
	ON_BN_CLICKED(IDC_BTN_MORE_RECORD, &CGuangDong11X5::OnBnClickedBtnMoreRecord)
	ON_BN_CLICKED(IDC_BTN_QUSHI, &CGuangDong11X5::OnBnClickedBtnQuShi)

	ON_BN_CLICKED(IDC_BTN_BEITOU_JIA, &CGuangDong11X5::OnBnClickedBeiTouJia)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIAN, &CGuangDong11X5::OnBnClickedBeiTouJian)
	ON_EN_CHANGE(IDC_RICHEDIT_DANSHI, &CGuangDong11X5::OnEnChangeRichEditDanshiInput)

END_MESSAGE_MAP()

void CGuangDong11X5::OnRichEditCopy()
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
BOOL CGuangDong11X5::PreTranslateMessage(MSG* pMsg)
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


// CGuangDong11X5 ��Ϣ�������
void CGuangDong11X5::OnPaint()
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
	graphics.DrawImage(m_bmpBk, Rect(0, 0,  m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 28, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	//���ƿ�����
	DrawLastKjHaoma(&cacheDC, graphics);
	DrawTopKjHaoma(&cacheDC, graphics);

	//������ע�����ܽ��
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

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CGuangDong11X5::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CGuangDong11X5::OnSize(UINT nType, int cx, int cy)
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

void CGuangDong11X5::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	OnBnClickedBtnClsList();
	ResetAllNums();
	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		OnBnClickedBtnClsList();
		m_bKaiJiangzhong = true;
		SetTimer(timer_id_get_luck_num, 10000, NULL);
		SetTimer(timer_id_kj_yugao, 1000, NULL);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);
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
		OnBnClickedBtnRx1();
	}
	else
	{
		KillTimer(timer_id_get_luck_num);
		KillTimer(timer_id_kj_yugao);
		KillTimer(timer_id_kaijiangzhong);
	}
}

HBRUSH CGuangDong11X5::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == IDC_EDIT_DANSHI_HAOMA) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
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
	if (pWnd->GetDlgCtrlID() == IDC_RICHEDIT_DANSHI) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
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

	//���ģʽ����ʾ��Ϣ
	if(CTLCOLOR_STATIC==nCtlColor && pWnd->GetDlgCtrlID() == IDC_STATIC_HUNHE_TIP)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
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
		pDC->SetTextColor(RGB(88, 78, 77));
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
			pDC->SetTextColor(RGB(88, 78, 77));
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
void CGuangDong11X5::SetTypeID(CaiZhong TypeID)
{
	m_TypeID = TypeID;

	m_cbIfTs = 0;
	SendToServer(11);
	KillTimer(TimerLotteryStatus);
	SetTimer(TimerLotteryStatus,20000,NULL);
	//InitWanFaMoShi();
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

}

void CGuangDong11X5::OnTimer(UINT_PTR nIDEvent)
{
	if(timer_id_get_luck_num == nIDEvent)
	{
		LuckyNumCQSSC();
	}
	else if (timer_id_kj_yugao == nIDEvent)
	{
		CRect rcRect;
		rcRect.CopyRect(rcYuGao);
	//	rcRect.right += 80;
		rcRect.bottom += 50;
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
	else if(TimerLotteryStatus == nIDEvent)
	{
		SendToServer(11);
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
				int nLeft=kj_big_haoma_x -15/*+ n *( m_bigNumWidth+8)*/;
				CMemDC	mDC(pDC, CRect(nLeft, kj_big_haoma_y, nLeft+m_imgKaijiang->GetWidth()+200, kj_big_haoma_y+m_imgKaijiang->GetHeight()+37));
				Graphics gh(mDC.m_hDC);
				gh.DrawImage(m_imgKaijiang, nLeft, kj_big_haoma_y, m_imgKaijiang->GetWidth(), m_imgKaijiang->GetHeight());
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
//����
void CGuangDong11X5::OnBnClickedBtnDanTuo()
{
	CheckWanFaBtn(m_btnDanTuo);
	HideAllRio();

	ShowCtrl(IDC_RIO_RERDT);
	ShowCtrl(IDC_RIO_RSANDT);
	ShowCtrl(IDC_RIO_RSIDT);
	ShowCtrl(IDC_RIO_RWUDT);
	ShowCtrl(IDC_RIO_RLIUDT);
	ShowCtrl(IDC_RIO_RQIDT);
	ShowCtrl(IDC_RIO_RBADT);

	OnBnClickedRioRen2DanTuo();
}
//��ѡһ
void CGuangDong11X5::OnBnClickedBtnRx1()
{
	CheckWanFaBtn(m_btnRenXuan1);

	HideAllRio();
	ShowCtrl(IDC_RIO_RX1_FUSHI);
	
	OnBnClickedRioRenXuan1();
}
//��ѡ��
void CGuangDong11X5::OnBnClickedBtnHszhx()
{
	//m_wanfa = HouSanZhiXuan;
	CheckWanFaBtn(m_btnHouSanZhiXuan);

	HideAllRio();
	ShowCtrl(IDC_RIO_HSZHX_FUSHI);
	//m_listChqSSC.DeleteAllItems();

// 	ShowCtrl(IDC_RIO_HSZHX_DANSHI);
// 
// 	ShowCtrl(IDC_RIO_QSZHX_FUSHI);		//������ǰ��ֱѡ��ʽ
// 	ShowCtrl(IDC_RIO_QSZHX_DANSHI);		//������ǰ��ֱѡ��ʽ
	OnBnClickedRioHszhxFushi();
}
//��ѡ��
void CGuangDong11X5::OnBnClickedBtnHszx()
{
	//m_wanfa = HouSanZuXuan;
	CheckWanFaBtn(m_btnHouSanZuXuan);

	HideAllRio();

	ShowCtrl(IDC_RIO_HSZS_FUSHI);
	//m_listChqSSC.DeleteAllItems();
	//ShowCtrl(IDC_RIO_HSZS_DANSHI);
	//ShowCtrl(IDC_RIO_HSZL_FUSHI);
	//ShowCtrl(IDC_RIO_HSZL_DANSHI);
	//SelctRadioBtn(IDC_RIO_HSZS_FUSHI);
	OnBnClickedRioHszsFushi();
}
//��ѡ��
void CGuangDong11X5::OnBnClickedBtnWxhe()
{
	//m_wanfa = WuXingHouEr;
	CheckWanFaBtn(m_btnWuXingHouEr);

	HideAllRio();

	//m_listChqSSC.DeleteAllItems();
	ShowCtrl(IDC_RIO_HEZHX_FUSHI);
	//ShowCtrl(IDC_RIO_HEZHX_DANSHI);
	//SelctRadioBtn(IDC_RIO_HEZHX_FUSHI);
	OnBnClickedRioHezhxFushi();
}
//��ѡ��
void CGuangDong11X5::OnBnClickedBtnHezx()
{
	//m_wanfa = HouErZuXuan;
	CheckWanFaBtn(m_btnHouErZuXuan);

	HideAllRio();

//	m_listChqSSC.DeleteAllItems();
	ShowCtrl(IDC_RIO_HEZX_FUSHI);
	//ShowCtrl(IDC_RIO_HEZX_DANSHI);
	//SelctRadioBtn(IDC_RIO_HEZX_FUSHI);
	OnBnClickedRioHezxFushi();
}
//��ѡ��
void CGuangDong11X5::OnBnClickedBtnDxdsh()
{
	//m_wanfa = DaXiaoDanShuang;
	CheckWanFaBtn(m_btnDaXiaoDanShuang);

	HideAllRio();

//	m_listChqSSC.DeleteAllItems();
	ShowCtrl(IDC_RIO_DXDSH);
	//SelctRadioBtn(IDC_RIO_DXDSH);
	OnBnClickedRioDxdsh();
}
//��ѡ��
void CGuangDong11X5::OnBnClickedBtnBdw()
{
	//m_wanfa = BuDingWei;
	CheckWanFaBtn(m_btnBuDingWei);

	HideAllRio();

//	m_listChqSSC.DeleteAllItems();
	ShowCtrl(IDC_RIO_HSBDW);
	//ShowCtrl(IDC_RIO_QSBDW);
	//SelctRadioBtn(IDC_RIO_HSBDW);
	OnBnClickedRioHsbdw();
}

//��ѡ��
void CGuangDong11X5::OnBnClickedBtnQszhx()
{
////��ѡ2,3,4
	CheckWanFaBtn(m_btnQianSanZhiXuan);

//	m_listChqSSC.DeleteAllItems();
	HideAllRio();
	ShowCtrl(IDC_RIO_RENXUAN2);
// 	ShowCtrl(IDC_RIO_RENXUAN3);
// 	ShowCtrl(IDC_RIO_RENXUAN4);
// 
// 	ShowCtrl(IDC_RIO_MOUSE_INPUT);
// 	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);
	OnBnClickedRioRenXuan2();
}
//ǰһ
void CGuangDong11X5::OnBnClickedBtnQszx()
{
	//m_wanfa = QianSanZuXuan;
	CheckWanFaBtn(m_btnQianSanZuXuan);

	HideAllRio();

//	m_listChqSSC.DeleteAllItems();
	ShowCtrl(IDC_RIO_QSZS_FUSHI);
	//ShowCtrl(IDC_RIO_QSZS_DANSHI);
	//ShowCtrl(IDC_RIO_QSZL_FUSHI);
	//ShowCtrl(IDC_RIO_QSZL_DANSHI);
	//SelctRadioBtn(IDC_RIO_QSZS_FUSHI);
	OnBnClickedRioQszsFushi();
}
//ǰ��
void CGuangDong11X5::OnBnClickedBtnWxqe()
{
	//m_wanfa = WuXingQianEr;
	CheckWanFaBtn(m_btnWuXingQianEr);

	HideAllRio();

	//m_listChqSSC.DeleteAllItems();
	ShowCtrl(IDC_RIO_QEZHX_FUSHI);
	ShowCtrl(IDC_RIO_QEZHX_DANSHI);
	//SelctRadioBtn(IDC_RIO_QEZHX_FUSHI);
	OnBnClickedRioQezhxFushi();
}
//ǰ��
void CGuangDong11X5::OnBnClickedBtnQezx()
{
	//m_wanfa = QianErZuXuan;
	CheckWanFaBtn(m_btnQianErZuXuan);

//	m_listChqSSC.DeleteAllItems();
	HideAllRio();

	ShowCtrl(IDC_RIO_QEZX_FUSHI);
	ShowCtrl(IDC_RIO_QEZX_DANSHI);
	//SelctRadioBtn(IDC_RIO_QEZX_FUSHI);
	OnBnClickedRioQezxFushi();
}

void CGuangDong11X5::OnBnClickedBtnHszhxhzh()
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
void CGuangDong11X5::OnBnClickedBtnDwd()
{
	//m_wanfa = DingWeiDan;
//	CheckWanFaBtn(m_btnDingWeiDan);

	HideAllRio();

	ShowCtrl(IDC_RIO_DWD);
	//SelctRadioBtn(IDC_RIO_DWD);
//	OnBnClickedRioDwd();
}


BOOL CGuangDong11X5::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\game_bk3.png"));
	m_numImageList = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num1.png"));
	m_kjNumBig = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big1.png"));
	m_imgKaijiang = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\KaiJiang.gif"));
	int nCount = m_imgKaijiang->GetFrameDimensionsCount();			//��ȡ֡ά��
	GUID *pGuids = new GUID[nCount];								//����һ��GUID����
	m_imgKaijiang->GetFrameDimensionsList(pGuids,nCount);			//��ȡͼ��֡��GUID
	m_nFrameCount=m_imgKaijiang->GetFrameCount(pGuids);				//��ȡGIF֡��

	m_btnMoreRecord.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_more.png"));
	m_DlgOpenRecord.Create(COpenRecord::IDD,this);
	m_btnMoreRecord.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_more.png"));
	m_btnQushi.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_hmzs.png"));

	m_bigNumWidth = m_kjNumBig->GetWidth() / 11;
	m_bigNumHeight = m_kjNumBig->GetHeight();
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small1.png"));
	m_smallNumWidth = m_kjNumSmall->GetWidth() / 11;
	m_smallNumHeight = m_kjNumSmall->GetHeight();
	m_bmpDxdsh = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx2.png"));
	m_bmpNumTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit1.png"));
	m_btnTouZhuOnekey.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\yjtz.png"));
	m_bShowFail = true;
	ZeroMemory(m_szMD5Haoma,sizeof(m_szMD5Haoma));
	InitNumberBtns();
	InitListCtrl();

	m_smallNumfont.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_zongFont.CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_KjInfoFont1.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_KjInfoFont.CreateFont(35, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_btnYuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_1.png"));
	m_btnJiao.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_2.png"));
	m_btnFen.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_3.png"));
	m_btnLi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_4.png"));

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

	hBrush=NULL;
	LuckyNumCQSSC();
	m_btWxzxTip.SetImage(CBmpUtil::GetExePath() + _T("skin\\bangzhu.png"));

// 	SendQuerySystemTime();
// 	SendQueryLuckyNumCQSSC();


	m_bKaiJiangzhong=false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGuangDong11X5::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
}

void CGuangDong11X5::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}

void CGuangDong11X5::OnBnClickedBtnWanQuan()
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
	CheckNumBtn(m_btnWan_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnWanDa()
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
	CheckNumBtn(m_btnWan_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnWanXiao()
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
	UnCheckNumBtn(m_btnWan_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnWanDan()
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
	CheckNumBtn(m_btnWan_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnWanShuang()
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
	UnCheckNumBtn(m_btnWan_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnWanQing()
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
	UnCheckNumBtn(m_btnWan_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnQianQuan()
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
	CheckNumBtn(m_btnQian_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnQianDa()
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
	CheckNumBtn(m_btnQian_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnQianXiao()
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
	UnCheckNumBtn(m_btnQian_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnQianDan()
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
	CheckNumBtn(m_btnQian_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnQianShuang()
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
	UnCheckNumBtn(m_btnQian_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnQianQing()
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
	UnCheckNumBtn(m_btnQian_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnBaiQuan()
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
	CheckNumBtn(m_btnBai_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnBaiDa()
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
	CheckNumBtn(m_btnBai_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnBaiXiao()
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
	UnCheckNumBtn(m_btnBai_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnBaiDan()
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
	CheckNumBtn(m_btnBai_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnBaiShuang()
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
	UnCheckNumBtn(m_btnBai_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnBaiQing()
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
	UnCheckNumBtn(m_btnBai_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnShiQuan()
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
	CheckNumBtn(m_btnShi_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnShiDa()
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
	CheckNumBtn(m_btnShi_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnShiXiao()
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
	UnCheckNumBtn(m_btnShi_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnShiDan()
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
	CheckNumBtn(m_btnShi_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnShiShuang()
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
	UnCheckNumBtn(m_btnShi_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnShiQing()
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
	UnCheckNumBtn(m_btnShi_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnGeQuan()
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
	CheckNumBtn(m_btnGe_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnGeDa()
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
	CheckNumBtn(m_btnGe_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnGeXiao()
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
	UnCheckNumBtn(m_btnGe_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnGeDan()
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
	CheckNumBtn(m_btnGe_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnGeShuang()
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
	UnCheckNumBtn(m_btnGe_10);
	Get11x5Zhushu(m_TypeID,m_gameKind);
}

void CGuangDong11X5::OnBnClickedBtnGeQing()
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
	Get11x5Zhushu(m_TypeID,m_gameKind);
}
int SplitString2(const string &srcStr, vector<string> &destVec, const string splitStr="|")
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
void CGuangDong11X5::OnBnClickedBtnChqAdd()
{
	//m_DlgStatus.ShowStatusWindow(TEXT("����������ݣ����Ժ�..."));
	CString strWan = GetWanString();
	CString strQian = GetQianString();
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();

	CString strTemp;
	CString strHaoMa;
	int zhushu = 1;
	if (IIRenXuan1 == m_gameKind)						//��ѡһ
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,22,1))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			//nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = nCount;
		}
		else
		{
			if(strBai.GetLength()<2)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡһ����ѡ��һ������"));
				return;

			}

			int nCount = strBai.GetLength()/2;
			zhushu = nCount;

			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != strBai.GetLength()-1 && (i%2==1))
					strHaoMa+=_T(",");
			}

		}

	}
	else if (IIRenXuan2 == m_gameKind)						//��ѡ��
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,22,2))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;
		}
		else
		{
			if(strBai.GetLength()<4)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ������ѡ����������"));
				return;

			}

			int nCount = strBai.GetLength()/2;
			zhushu = (nCount-1)*nCount/2;

			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != strBai.GetLength()-1 && (i%2==1))
					strHaoMa+=_T(",");
			}

		}

	}
	else if (IIRenXuan3 == m_gameKind)				//��ѡ��
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,22,3))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*(nCount-2)*nCount/6;
		}
		else
		{
			if (strBai.GetLength() < 6) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ������ѡ����������"));
				return;
			}

			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != strBai.GetLength()-1&&(i%2==1))
					strHaoMa+=_T(",");

			}

			int nCount = strBai.GetLength()/2;
			zhushu = (nCount-1)*(nCount-2)*nCount/6;
		}
	}
	else if (QianYi == m_gameKind)				//ǰһ
	{
		if (strWan.GetLength() < 2) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("ǰһ����ѡ��һ������"));
			return;
		}

		for(int i = 0;i < strWan.GetLength();i++)
		{
			strHaoMa += strWan.GetAt(i);
			if(i != strWan.GetLength()-1&&(i%2==1))
				strHaoMa+=_T(",");
		}



		zhushu = strWan.GetLength()/2 /** (strBai.GetLength() - 1)*/;
	}
	else if (IIRenXuan4 == m_gameKind)					//��ѡ��
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,22,4))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*nCount/24;
		}
		else
		{

			if(strBai.GetLength()<8) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != strBai.GetLength()-1 &&(i%2==1))
					strHaoMa+=_T(",");

			}

			int nCount = strBai.GetLength()/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*nCount/24;
		}

	}
	else if (QianEr_ZhiXuan == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,4,2))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;
		}
		else
		{
			zhushu=0;

			if(strWan.IsEmpty() || strQian.IsEmpty()) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ÿλ����ѡ��һ������"));
				return;
			}

			CString strTempWan,strTempQian;
			for(int i = 0;i < strWan.GetLength();i+=2)
			{
				strTempWan = strWan.GetAt(i);
				strTempWan += strWan.GetAt(i+1);
				for(int j = 0;j < strQian.GetLength();j+=2)
				{
					strTempQian = strQian.GetAt(j);
					strTempQian += strQian.GetAt(j+1);
					if (strTempQian != strTempWan)
					{
						zhushu++;
					}
				}
			}

			for(int i = 0;i < strWan.GetLength();i++)
			{
				strHaoMa += strWan.GetAt(i);
				if(i == strWan.GetLength()-1)
				{
					strHaoMa += _T(",");
				}

			}

			for(int i = 0;i < strQian.GetLength();i++)
			{
				strHaoMa += strQian.GetAt(i);
				if(i == strQian.GetLength()-1)
				{
					strHaoMa += _T(",");
				}

			}
		//	zhushu = (strWan.GetLength()/2) * (strQian.GetLength()/2);
		}
	}
	else if (QianEr_ZuXuan == m_gameKind)					//ǰ����ѡ
	{
		if (strBai.GetLength()<4) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("ǰ����ѡ����ѡ����������"));
			return;
		}

		for(int i = 0;i < strBai.GetLength();i++)
		{
			strHaoMa += strBai.GetAt(i);
			if(i != strBai.GetLength()-1 &&(i%2 == 1))
			{
				strHaoMa += _T(",");
			}
		}
		int nCount = strBai.GetLength()/2;
		zhushu = (nCount-1)*nCount/2;

	}
	else if (IIRenXuan5 == m_gameKind)					//��ѡ��
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,22,5))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*nCount/120;
		}
		else
		{

			if(strBai.GetLength()<10) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ������ѡ���������"));
				return;
			}

			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != strBai.GetLength()-1 &&(i%2==1))
					strHaoMa+=_T(",");

			}
			int nCount = strBai.GetLength()/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*nCount/120;
		}
	}
	else if (QianSan_ZhiXuan == m_gameKind)					//ǰ��ֱѡ
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,6,3))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*nCount/120;
		}
		else
		{

			zhushu=0;
			if (strBai.IsEmpty() ||strQian.IsEmpty() || strWan.IsEmpty()) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ǰ��ֱѡÿλ����ѡ��һ������"));
				return;
			}

			CString strTempWan,strTempQian,strTempBai;
			for(int i = 0;i < strWan.GetLength();i+=2)
			{
				strTempWan = strWan.GetAt(i);
				strTempWan += strWan.GetAt(i+1);
				for(int j = 0;j < strQian.GetLength();j+=2)
				{
					strTempQian = strQian.GetAt(j);
					strTempQian += strQian.GetAt(j+1);
					for(int k = 0;k < strBai.GetLength();k+=2)
					{
						strTempBai = strBai.GetAt(k);
						strTempBai += strBai.GetAt(k+1);
						if (strTempQian != strTempWan && strTempBai != strTempQian && strTempBai != strTempWan)
						{
							zhushu++;
						}
					}
				}
			}


			for(int i = 0;i < strWan.GetLength();i++)
			{
				strHaoMa += strWan.GetAt(i);
				if(i == strWan.GetLength()-1)
				{
					strHaoMa += _T(",");
				}

			}

			for(int i = 0;i < strQian.GetLength();i++)
			{
				strHaoMa += strQian.GetAt(i);
				if(i == strQian.GetLength()-1)
				{
					strHaoMa += _T(",");
				}

			}
			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i == strBai.GetLength()-1)
				{
					strHaoMa += _T(",");
				}

			}
		//	zhushu = (strWan.GetLength()/2) * (strQian.GetLength()/2)*(strBai.GetLength()/2);
		}
	}
	else if (QianSan_ZuXuan == m_gameKind)							//ǰ����ѡ
	{
		if (strBai.GetLength()<6) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("ǰ����ѡ����ѡ����������"));
			return;
		}

		for(int i = 0;i < strBai.GetLength();i++)
		{
			strHaoMa += strBai.GetAt(i);
			if(i != strBai.GetLength()-1 && (i%2 == 1))
			{
				strHaoMa += _T(",");
			}
		}
		int nCount = strBai.GetLength()/2;
		zhushu = (nCount-2)*(nCount-1)*nCount/6;
	}
	else if (IIRenXuan6 == m_gameKind)				//��ѡ��
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,22,6))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*nCount/720;
		}
		else
		{
			if(strBai.GetLength()<12) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ������ѡ����������"));
				return;
			}

			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != strBai.GetLength()-1 && (i%2==1))
					strHaoMa+=_T(",");
			}

			int nCount = strBai.GetLength()/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*nCount/720;
		}
	}
	else if(IIRenXuan8 == m_gameKind)				//��ѡ��
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,22,8))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*(nCount-7)*nCount/40320;
		}
		else
		{


			if(strBai.GetLength()<16) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ������ѡ��˸�����"));
				return;
			}

			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != strBai.GetLength()-1&&(i%2==1))
					strHaoMa+=_T(",");

			}

			int nCount = strBai.GetLength()/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*(nCount-7)*nCount/40320;
		}
	}
	else if (IIRenXuan7 == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{

			if(!GetDanShiHaoma(strHaoMa,22,7))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*nCount/5040;
		}
		else
		{


			if(strBai.GetLength()<14) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != strBai.GetLength()-1&&(i%2==1))
					strHaoMa+=_T(",");

			}

			int nCount = strBai.GetLength()/2;
			zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*nCount/5040;
		}
	}
	else if(m_gameKind == Ren2_Dantuo||m_gameKind == Ren3_Dantuo||m_gameKind == Ren4_Dantuo||m_gameKind == Ren5_Dantuo||m_gameKind == Ren6_Dantuo||m_gameKind == Ren7_Dantuo||m_gameKind == Ren8_Dantuo)
	{
		int nMax = 0;
		if(m_gameKind == Ren2_Dantuo)
			nMax = 2;
		else if(m_gameKind == Ren3_Dantuo)
			nMax = 3;
		else if(m_gameKind == Ren4_Dantuo)
			nMax = 4;
		else if(m_gameKind == Ren5_Dantuo)
			nMax = 5;
		else if(m_gameKind == Ren6_Dantuo)
			nMax = 6;
		else if(m_gameKind == Ren7_Dantuo)
			nMax = 7;
		else if(m_gameKind == Ren8_Dantuo)
			nMax = 8;

		int nQianCount = strQian.GetLength()/2;
		int nBaiCount = strBai.GetLength()/2;

		if(nQianCount>(nMax-1)||nBaiCount<1||nQianCount<1)
		{
			CString strMessage;
			strMessage.Format(L"��%s���������ѡ%d��,�������ѡ%d����",theGameKind[(m_TypeID << 16) | (unsigned short)m_gameKind],nMax-1,11-nMax+1);
			MyMessageBox(strMessage);
			return ;

		}
		CString strQianDan,strBaiDan;
		for (int i = 0;i < nQianCount*2;i+=2)
		{
			strQianDan = strQian.Mid(i,2);
			for (int j = 0;j < nBaiCount*2;j+=2)
			{
				strBaiDan = strBai.Mid(j,2);

				if(strQianDan == strBaiDan)
				{
					CString strMessage;
					strMessage.Format(L"��������벻�����ظ����֣�");
					MyMessageBox(strMessage);
					return ;
				}
			}
		}
		zhushu = combo(nBaiCount,nMax-nQianCount);
		for(int i = 0;i < nQianCount*2;i+=2)
		{
			strHaoMa += strQian.Mid(i,2);
			if(i!=(nQianCount*2-2))
				strHaoMa += _T(",");
		}
		strHaoMa += _T(";");
		for(int i = 0;i < nBaiCount*2;i+=2)
		{
			strHaoMa += strBai.Mid(i,2);
			if(i!=(nBaiCount*2-2))
				strHaoMa += _T(",");
		}
		strHaoMa += _T("|");
	}
	UpdateData();

	CStringArray strArr;

	if (!strHaoMa.IsEmpty()) 
	{
		if(strHaoMa.GetLength()>1024)
		{
			m_DlgStatus.ShowStatusWindow(TEXT("����������ݣ����Ժ�..."));
		}
		wstring strdata;
		strdata = strHaoMa.GetBuffer(0);

		vecAllNums.insert(vecAllNums.begin(),1,strdata);


		if(m_RenxuanKeyboardInput)
		{
			int nCount = m_listChqSSC.GetItemCount();
			int nIndex = 0;
			int nZhushu = 0;
// 			CString strAddHaoma;
// 			do
// 			{
// 				CString strHaomaTemp;
// 
// 				if(!AfxExtractSubString(strHaomaTemp, strHaoMa, nIndex++, '|'))
// 					break;
// 				if(strHaomaTemp.IsEmpty())
// 					break;
// 
// 				zhushu = 1;
// 				if(m_gameKind == IIRenXuan2)							//��ѡ��
// 				{
// 					int nCount = strHaomaTemp.GetLength()/3;
// 					nCount = (strHaomaTemp.GetLength()-nCount+1)/2;
// 					zhushu = (nCount-1)*nCount/2;
// 				}
// 				else if(m_gameKind == IIRenXuan3)						//��ѡ��
// 				{
// 					int nCount = strHaomaTemp.GetLength()/3;
// 					nCount = (strHaomaTemp.GetLength()-nCount+1)/2;
// 					zhushu = (nCount-1)*(nCount-2)*nCount/6;
// 				}
// 				else if (IIRenXuan4 == m_gameKind)					//��ѡ��
// 				{
// 					int nCount = strHaomaTemp.GetLength()/3;
// 					nCount = (strHaomaTemp.GetLength()-nCount+1)/2;
// 					zhushu = (nCount-1)*(nCount-2)*(nCount-3)*nCount/24;
// 				}
// 				else if (IIRenXuan5 == m_gameKind)					//��ѡ��
// 				{
// 					int nCount = strHaomaTemp.GetLength()/3;
// 					nCount = (strHaomaTemp.GetLength()-nCount+1)/2;
// 					zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*nCount/120;
// 				}
// 				else if (IIRenXuan6 == m_gameKind)					//��ѡ��
// 				{
// 					int nCount = strHaomaTemp.GetLength()/3;
// 					nCount = (strHaomaTemp.GetLength()-nCount+1)/2;
// 					zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*nCount/720;
// 				}
// 				else if (IIRenXuan7 == m_gameKind)					//��ѡ��
// 				{
// 					int nCount = strHaomaTemp.GetLength()/3;
// 					nCount = (strHaomaTemp.GetLength()-nCount+1)/2;
// 					zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*nCount/5040;
// 				}
// 				else if(IIRenXuan8 == m_gameKind)						//��ѡ��
// 				{
// 					int nCount = strHaomaTemp.GetLength()/3;
// 					nCount = (strHaomaTemp.GetLength()-nCount+1)/2;
// 					zhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*(nCount-7)*nCount/40320;
// 
// 				}
// 				else if(QianEr_ZhiXuan == m_gameKind)
// 				{
// 					zhushu = 1;
// 				}
// 				else if(QianSan_ZhiXuan == m_gameKind)
// 				{
// 					zhushu = 1;
// 				}
// 
// 
// 				strAddHaoma += strHaomaTemp;
// 				strAddHaoma += _T("|");
// 				int nLength = strHaomaTemp.GetLength()/2+1;
// 
// 
// 				nZhushu += zhushu;
// 
// 			}
// 			while(nIndex < 80000);

// 			wstring strdata;
// 			strdata = strAddHaoma.GetBuffer(0);
// 			vecAllNums.insert(vecAllNums.begin(),1,strdata);
			nZhushu = m_singlezhushu;
			int nLength = strHaoMa.GetLength();
			if(nLength > 8000)
				m_listChqSSC.InsertItem(nCount, L"������ӳɹ�");
			else
				m_listChqSSC.InsertItem(nCount, strHaoMa);

		//	m_listChqSSC.InsertItem(nCount, strAddHaoma);
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
			fJine = nZhushu*m_beishu*danzhujine;
			CString strZongzhu ;
			strZongzhu.Format(L"%.3lf",fJine);
			m_listChqSSC.SetItemText(nCount, 2, strZongzhu);

			m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());
			CString strTmp;
			strTmp.Format(_T("%d"), nZhushu);
			m_listChqSSC.SetItemText(nCount, 4, strTmp);
			strTmp.Format(_T("%d"), m_beishu);
			m_listChqSSC.SetItemText(nCount, 5, strTmp);
			m_listChqSSC.createItemButton(nCount++, 6, this->GetSafeHwnd());
			m_zongZhuShu += nZhushu;

		}
		else
		{
			int nCount = m_listChqSSC.GetItemCount();
			m_listChqSSC.InsertItem(nCount, strHaoMa);
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
			CString strMD5Haoma;
			for(int i = 0;i < m_listChqSSC.GetItemCount();i++)
			{
				CString haomao = m_listChqSSC.GetItemText(i, 0);
				strMD5Haoma += haomao;
				strMD5Haoma += L"|";
			}
			strTmp.Format(_T("%d"), m_beishu);
			m_listChqSSC.SetItemText(nCount, 5, strTmp);
			m_listChqSSC.createItemButton(nCount, 6, this->GetSafeHwnd());


			memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
			CMD5Encrypt::EncryptData(strMD5Haoma,m_szMD5Haoma);

			m_zongZhuShu += zhushu;
		}


		strHaoMa = _T("");
	}
	int arrLen = strArr.GetCount();
	if (arrLen > 0) 
	{
		CString strMD5Haoma;
		int nCount = m_listChqSSC.GetItemCount();
		for (int i=0; i<arrLen; i++) 
		{
			m_listChqSSC.InsertItem(nCount+i, strArr.GetAt(i));
			m_listChqSSC.SetItemText(nCount+i, 1, GetScoreMoshi());
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
			m_listChqSSC.SetItemText(nCount+i, 2, strZongzhu);

			m_listChqSSC.SetItemText(nCount+i, 3, GetWanDesc());
			CString strTmp;
			strTmp.Format(_T("%d"), zhushu);
			m_listChqSSC.SetItemText(nCount+i, 4, strTmp);
			strTmp.Format(_T("%d"), m_beishu);
			m_listChqSSC.SetItemText(nCount+i, 5, strTmp);
			m_listChqSSC.createItemButton(nCount+i, 6, this->GetSafeHwnd());
			strMD5Haoma += strArr.GetAt(i);
			strMD5Haoma += L"|";
		}


		memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
		CMD5Encrypt::EncryptData(strMD5Haoma,m_szMD5Haoma);

		m_zongZhuShu += (arrLen*zhushu);
		strArr.RemoveAll();

		memset(&g_arr, 0, sizeof(g_arr));
		memset(&g_nPos, 0, sizeof(g_nPos));
		g_Counts=0;
	}
	int nItemCount = m_listChqSSC.GetItemCount();
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_listChqSSC.GetItemText(i,5);
		CString strZhuSHu = m_listChqSSC.GetItemText(i,4);
		CString strMoshi = m_listChqSSC.GetItemText(i,1);
		int beishu = _ttoi(strBeiSHu);
		int zhushu = _ttoi(strZhuSHu);

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

	ResetAllNums();
	m_DlgStatus.HideStatusWindow();
	return;
}
//ȡ������
VOID CGuangDong11X5::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//��ʼ�����밴ť
void CGuangDong11X5::InitNumberBtns()
{
	m_btnWan_0.SetImage(m_numImageList, 0, 11);
	m_btnWan_1.SetImage(m_numImageList, 1, 11);
	m_btnWan_2.SetImage(m_numImageList, 2, 11);
	m_btnWan_3.SetImage(m_numImageList, 3, 11);
	m_btnWan_4.SetImage(m_numImageList, 4, 11);
	m_btnWan_5.SetImage(m_numImageList, 5, 11);
	m_btnWan_6.SetImage(m_numImageList, 6, 11);
	m_btnWan_7.SetImage(m_numImageList, 7, 11);
	m_btnWan_8.SetImage(m_numImageList, 8, 11);
	m_btnWan_9.SetImage(m_numImageList, 9, 11);
	m_btnWan_10.SetImage(m_numImageList, 10, 11);

	m_btnQian_0.SetImage(m_numImageList, 0, 11);
	m_btnQian_1.SetImage(m_numImageList, 1, 11);
	m_btnQian_2.SetImage(m_numImageList, 2, 11);
	m_btnQian_3.SetImage(m_numImageList, 3, 11);
	m_btnQian_4.SetImage(m_numImageList, 4, 11);
	m_btnQian_5.SetImage(m_numImageList, 5, 11);
	m_btnQian_6.SetImage(m_numImageList, 6, 11);
	m_btnQian_7.SetImage(m_numImageList, 7, 11);
	m_btnQian_8.SetImage(m_numImageList, 8, 11);
	m_btnQian_9.SetImage(m_numImageList, 9, 11);
	m_btnQian_10.SetImage(m_numImageList, 10, 11);

	m_btnBai_0.SetImage(m_numImageList, 0, 11);
	m_btnBai_1.SetImage(m_numImageList, 1, 11);
	m_btnBai_2.SetImage(m_numImageList, 2, 11);
	m_btnBai_3.SetImage(m_numImageList, 3, 11);
	m_btnBai_4.SetImage(m_numImageList, 4, 11);
	m_btnBai_5.SetImage(m_numImageList, 5, 11);
	m_btnBai_6.SetImage(m_numImageList, 6, 11);
	m_btnBai_7.SetImage(m_numImageList, 7, 11);
	m_btnBai_8.SetImage(m_numImageList, 8, 11);
	m_btnBai_9.SetImage(m_numImageList, 9, 11);
	m_btnBai_10.SetImage(m_numImageList, 10, 11);

	m_btnShi_0.SetImage(m_numImageList, 0, 11);
	m_btnShi_1.SetImage(m_numImageList, 1, 11);
	m_btnShi_2.SetImage(m_numImageList, 2, 11);
	m_btnShi_3.SetImage(m_numImageList, 3, 11);
	m_btnShi_4.SetImage(m_numImageList, 4, 11);
	m_btnShi_5.SetImage(m_numImageList, 5, 11);
	m_btnShi_6.SetImage(m_numImageList, 6, 11);
	m_btnShi_7.SetImage(m_numImageList, 7, 11);
	m_btnShi_8.SetImage(m_numImageList, 8, 11);
	m_btnShi_9.SetImage(m_numImageList, 9, 11);
	m_btnShi_10.SetImage(m_numImageList, 10, 11);

	m_btnGe_0.SetImage(m_numImageList, 0, 11);
	m_btnGe_1.SetImage(m_numImageList, 1, 11);
	m_btnGe_2.SetImage(m_numImageList, 2, 11);
	m_btnGe_3.SetImage(m_numImageList, 3, 11);
	m_btnGe_4.SetImage(m_numImageList, 4, 11);
	m_btnGe_5.SetImage(m_numImageList, 5, 11);
	m_btnGe_6.SetImage(m_numImageList, 6, 11);
	m_btnGe_7.SetImage(m_numImageList, 7, 11);
	m_btnGe_8.SetImage(m_numImageList, 8, 11);
	m_btnGe_9.SetImage(m_numImageList, 9, 11);
	m_btnGe_10.SetImage(m_numImageList, 10, 11);

	m_staticZuXuan.SetImage(m_bmpNumTip, 0, 9);
	m_staticBuDingWei.SetImage(m_bmpNumTip, 1, 9);
	m_staticWan.SetImage(m_bmpNumTip, 2, 9);
	m_staticQian.SetImage(m_bmpNumTip, 3, 9);
	m_staticBai.SetImage(m_bmpNumTip, 4, 9);
	m_staticShi.SetImage(m_bmpNumTip, 5, 9);
	m_staticGe.SetImage(m_bmpNumTip, 6, 9);
	m_staticDanMa.SetImage(m_bmpNumTip, 8, 9);
	m_staticTuoMa.SetImage(m_bmpNumTip, 7, 9);

	m_btnDaShi.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoShi.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanShi.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangShi.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaGe.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoGe.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanGe.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangGe.SetImage(m_bmpDxdsh, 3, 4);

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

void CGuangDong11X5::InitListCtrl()
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
//��ѡһ
void CGuangDong11X5::OnBnClickedRioRenXuan1()
{
	ResetAllNums();

	m_gameKind = IIRenXuan1;		//��ѡһ
	SendToServer(3);

	m_rioRenXuan1.SetPushed(true);
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

	ShowZuXuan();
	HideDanShiEdit();
}

//��ѡ��
void CGuangDong11X5::OnBnClickedRioHszhxFushi()
{
	ResetAllNums();

	m_gameKind = IIRenXuan2;		//��ѡ��
	SendToServer(3);

	m_rioHsZhxFuShi.SetPushed(true);
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

	ShowZuXuan();
	HideDanShiEdit();
}
//��2����
void CGuangDong11X5::OnBnClickedRioRen2DanTuo()
{
	ResetAllNums();
	m_gameKind = Ren2_Dantuo;
	SendToServer(3);

	m_rioRen2DanTuo.SetPushed(true);
	m_rioRen3DanTuo.SetPushed(false);
	m_rioRen4DanTuo.SetPushed(false);
	m_rioRen5DanTuo.SetPushed(false);
	m_rioRen6DanTuo.SetPushed(false);
	m_rioRen7DanTuo.SetPushed(false);
	m_rioRen8DanTuo.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanTuo();
}
//��3����
void CGuangDong11X5::OnBnClickedRioRen3DanTuo()
{
	ResetAllNums();
	m_gameKind = Ren3_Dantuo;
	SendToServer(3);
	m_rioRen2DanTuo.SetPushed(false);
	m_rioRen3DanTuo.SetPushed(true);
	m_rioRen4DanTuo.SetPushed(false);
	m_rioRen5DanTuo.SetPushed(false);
	m_rioRen6DanTuo.SetPushed(false);
	m_rioRen7DanTuo.SetPushed(false);
	m_rioRen8DanTuo.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanTuo();
}
//��4����
void CGuangDong11X5::OnBnClickedRioRen4DanTuo()
{
	ResetAllNums();
	m_gameKind = Ren4_Dantuo;
	SendToServer(3);
	m_rioRen2DanTuo.SetPushed(false);
	m_rioRen3DanTuo.SetPushed(false);
	m_rioRen4DanTuo.SetPushed(true);
	m_rioRen5DanTuo.SetPushed(false);
	m_rioRen6DanTuo.SetPushed(false);
	m_rioRen7DanTuo.SetPushed(false);
	m_rioRen8DanTuo.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

ShowDanTuo();
}
//��5����
void CGuangDong11X5::OnBnClickedRioRen5DanTuo()
{
	ResetAllNums();
	m_gameKind = Ren5_Dantuo;
	SendToServer(3);
	m_rioRen2DanTuo.SetPushed(false);
	m_rioRen3DanTuo.SetPushed(false);
	m_rioRen4DanTuo.SetPushed(false);
	m_rioRen5DanTuo.SetPushed(true);
	m_rioRen6DanTuo.SetPushed(false);
	m_rioRen7DanTuo.SetPushed(false);
	m_rioRen8DanTuo.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

ShowDanTuo();
}
//��6����
void CGuangDong11X5::OnBnClickedRioRen6DanTuo()
{
	ResetAllNums();
	m_gameKind = Ren6_Dantuo;
	SendToServer(3);
	m_rioRen2DanTuo.SetPushed(false);
	m_rioRen3DanTuo.SetPushed(false);
	m_rioRen4DanTuo.SetPushed(false);
	m_rioRen5DanTuo.SetPushed(false);
	m_rioRen6DanTuo.SetPushed(true);
	m_rioRen7DanTuo.SetPushed(false);
	m_rioRen8DanTuo.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

ShowDanTuo();
}
//��7����
void CGuangDong11X5::OnBnClickedRioRen7DanTuo()
{
	ResetAllNums();
	m_gameKind = Ren7_Dantuo;
	SendToServer(3);
	m_rioRen2DanTuo.SetPushed(false);
	m_rioRen3DanTuo.SetPushed(false);
	m_rioRen4DanTuo.SetPushed(false);
	m_rioRen5DanTuo.SetPushed(false);
	m_rioRen6DanTuo.SetPushed(false);
	m_rioRen7DanTuo.SetPushed(true);
	m_rioRen8DanTuo.SetPushed(false);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

ShowDanTuo();
}
//��8����
void CGuangDong11X5::OnBnClickedRioRen8DanTuo()
{
	ResetAllNums();
	m_gameKind = Ren8_Dantuo;
	SendToServer(3);
	m_rioRen2DanTuo.SetPushed(false);
	m_rioRen3DanTuo.SetPushed(false);
	m_rioRen4DanTuo.SetPushed(false);
	m_rioRen5DanTuo.SetPushed(false);
	m_rioRen6DanTuo.SetPushed(false);
	m_rioRen7DanTuo.SetPushed(false);
	m_rioRen8DanTuo.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

ShowDanTuo();
}
//��ѡ��
void CGuangDong11X5::OnBnClickedRioRenXuan2()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = IIRenXuan8;
	SendToServer(3);
	m_rioRenXuan2.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	ShowZuXuan();
	HideDanShiEdit();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

}

//��ѡ��
void CGuangDong11X5::OnBnClickedRioHszsFushi()
{
	ResetAllNums();

	m_gameKind = IIRenXuan3;
SendToServer(3);
	SetLockText();

	m_rioHsZsFuShi.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	ShowZuXuan();
	HideDanShiEdit();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);
}


//ǰһ
void CGuangDong11X5::OnBnClickedRioQszsFushi()
{
	ResetAllNums();

	m_gameKind = QianYi;
SendToServer(3);
	SetLockText();

	m_rioQsZsFuShi.SetPushed(true);
	//m_rioQsZsDanShi.SetPushed(false);
	//m_rioQsZlFuShi.SetPushed(false);
	//m_rioQsZlDanShi.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	HideQianNums();
	HideBaiNums();
	//ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}


//��ѡ��
void CGuangDong11X5::OnBnClickedRioHezhxFushi()
{
	ResetAllNums();

	m_gameKind = IIRenXuan4;
	SetLockText();
	m_rioHeZhxFuShi.SetPushed(true);
SendToServer(3);
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	ShowZuXuan();
	HideDanShiEdit();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);
}

//��ѡ��
void CGuangDong11X5::OnBnClickedRioHezxFushi()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = IIRenXuan5;
SendToServer(3);

	m_rioHeZxFuShi.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	ShowZuXuan();
	HideDanShiEdit();
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);


}

//ǰ��ֱѡѡ��
void CGuangDong11X5::OnBnClickedRioQezhxFushi()
{
	ResetAllNums();

	m_gameKind = QianEr_ZhiXuan;
SendToServer(3);
	SetLockText();

	m_rioQeZhxFuShi.SetPushed(true);
	m_rioQeZhxDanShi.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

}

//ǰ����ѡѡ��
void CGuangDong11X5::OnBnClickedRioQezhxDanshi()
{
	ResetAllNums();

	m_gameKind = QianEr_ZuXuan;
SendToServer(3);
	SetLockText();

	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();
	ShowZuXuan();
	HideDanShiEdit();

	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
}

//ǰ��ֱѡѡ��
void CGuangDong11X5::OnBnClickedRioQezxFushi()
{
	ResetAllNums();

	m_gameKind = QianSan_ZhiXuan;
SendToServer(3);
	SetLockText();

	m_rioQeZxFuShi.SetPushed(true);
	m_rioQeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	//ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

}

//ǰ����ѡѡ��
void CGuangDong11X5::OnBnClickedRioQezxDanshi()
{
	ResetAllNums();

	m_gameKind = QianSan_ZuXuan;
SendToServer(3);
	SetLockText();

	m_rioQeZxFuShi.SetPushed(false);
	m_rioQeZxDanShi.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();
	
	HideDanShiEdit();

	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);

}

//��ѡ��
void CGuangDong11X5::OnBnClickedRioDxdsh()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = IIRenXuan6;
SendToServer(3);

	m_rioQeDxdsh.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	ShowZuXuan();
	HideDanShiEdit();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

}


//��ѡ��
void CGuangDong11X5::OnBnClickedRioHsbdw()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = IIRenXuan7;

SendToServer(3);
	m_rioHsbdw.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	ShowZuXuan();
	HideDanShiEdit();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

}


//׷��
void CGuangDong11X5::OnBnClickedBtnZhuihao()
{
	if(m_cbIfTs == 1)
	{
		MyMessageBox(L"�˲�����ͣ���ۣ�");
		return;
	}

	if(!CheckInput())
		return ;
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



	if(m_TypeID == CZGD11Xuan5)
	{
		dlgZhuiHao.SetCurrentQihaoTime( m_chqRule.GetNextExpect(-1), strBeishu,8,2, fJine,84,m_TypeID);
	}
	else if(m_TypeID == CZJX11Xuan5)
	{
			dlgZhuiHao.SetCurrentQihaoTime( m_jxRule.GetNextExpect(-1), strBeishu,8,2, fJine,84,CZJX11Xuan5);
	}
	else if(m_TypeID == CZSD11Xuan5)
	{
			dlgZhuiHao.SetCurrentQihaoTime( m_sdRule.GetNextExpect(-1), strBeishu,8,2, fJine,78,CZSD11Xuan5);
	}
	else if(m_TypeID == CZHLJ11Xuan5)
	{
			dlgZhuiHao.SetCurrentQihaoTime( m_hljRule.GetNextExpect(-1), strBeishu,8,2, fJine,87,CZHLJ11Xuan5);
	}

	if(dlgZhuiHao.DoModal() == IDOK)
	{
		if(m_cbIfTs == 1)
		{
			MyMessageBox(L"�˲�����ͣ���ۣ�");
			return;
		}

		long kjing = m_chqRule.GetKjShjDiff(); 
		if(m_TypeID == CZGD11Xuan5)
			kjing = m_chqRule.GetKjShjDiff(); 
		else if(m_TypeID == CZJX11Xuan5)
			kjing = m_jxRule.GetKjShjDiff(); 
		else if(m_TypeID == CZSD11Xuan5)
			kjing = m_sdRule.GetKjShjDiff(); 
		else if(m_TypeID == CZHLJ11Xuan5)
			kjing = m_hljRule.GetKjShjDiff(); 

		if (kjing < 60 )								//|| kjing > 540
		{
			MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
			return ;
		}

		m_nZhuihaoCount = dlgZhuiHao.GetZhuiHaoQiShu();

		vecData1.clear();
		vecData2.clear();
		vecData3.clear();
		bool bSucc=true;
		dlgZhuiHao.GetZhuiHaoData(vecData1, vecData2, vecData3);
		m_bZhuiZjtz = dlgZhuiHao.GetPrizeStop();

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

	}
	else
		return;
	
	//��ʼ������
	m_zongZhuShu = 0;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	UpdateData(FALSE);

OnBnClickedBtnClsList();
}

//У������
bool	CGuangDong11X5::CheckInput()
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
	if(m_TypeID == CZGD11Xuan5)
		kjing = m_chqRule.GetKjShjDiff(); 
	else if(m_TypeID == CZJX11Xuan5)
		kjing = m_jxRule.GetKjShjDiff(); 
	else if(m_TypeID == CZSD11Xuan5)
		kjing = m_sdRule.GetKjShjDiff(); 
	else if(m_TypeID == CZHLJ11Xuan5)
		kjing = m_hljRule.GetKjShjDiff(); 

	if (kjing < 60 )								//|| kjing > 540
	{
		MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
		return false;
	}

	return true;
}
void CGuangDong11X5::OnBnClickedBtnTouzhuOneKey()
{
	if(m_cbIfTs == 1)
	{
		MyMessageBox(L"�˲�����ͣ���ۣ�");
		return;
	}

	if(vecAllNums.size()<=0)
	{
		OnBnClickedBtnChqAdd();
	}
	OnBnClickedBtnTouzhuQ();
	return;
}
//��ͨ��ע
void CGuangDong11X5::OnBnClickedBtnTouzhuQ()
{
	CString strHao = m_listChqSSC.GetItemText(0,0);
	if(strHao.IsEmpty())
	{
		OnBnClickedBtnChqAdd();
	}

	//У������
	if(!CheckInput())
		return;


	bool bSucc = true;
	int itm_cnt = m_listChqSSC.GetItemCount();
	SendToServer(1);
	OnBnClickedBtnClsList();

	m_zongZhuShu = 0;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);

	InvalidateRect(&rcZongJinE);

	//m_beishu = 1;
	UpdateData(FALSE);
}

//��ͨ��ע
void CGuangDong11X5::OnBnClickedBtnTouzhu()
{
	if(m_cbIfTs == 1)
	{
		MyMessageBox(L"�˲�����ͣ���ۣ�");
		return;
	}

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

	if(m_TypeID == CZGD11Xuan5)
		qrDlg.m_gamekind = _T("�㶫11ѡ5");
	else if(m_TypeID == CZJX11Xuan5)
		qrDlg.m_gamekind = _T("����11ѡ5");
	else if(m_TypeID == CZSD11Xuan5)
		qrDlg.m_gamekind = _T("ɽ��11ѡ5");
	else if(m_TypeID == CZHLJ11Xuan5)
		qrDlg.m_gamekind = _T("������11ѡ5");

	qrDlg.m_zhushu = m_zongZhuShu ;
	qrDlg.m_moshi = theAccount.Scoretype;
	qrDlg.m_zongjine = m_zongJine;
	if (qrDlg.DoModal() != IDOK)
	{
		return;
	}
	if(m_cbIfTs == 1)
	{
		MyMessageBox(L"�˲�����ͣ���ۣ�");
		return;
	}

	long kjing = m_chqRule.GetKjShjDiff(); 
	if(m_TypeID == CZGD11Xuan5)
		kjing = m_chqRule.GetKjShjDiff(); 
	else if(m_TypeID == CZJX11Xuan5)
		kjing = m_jxRule.GetKjShjDiff(); 
	else if(m_TypeID == CZSD11Xuan5)
		kjing = m_sdRule.GetKjShjDiff(); 
	else if(m_TypeID == CZHLJ11Xuan5)
		kjing = m_hljRule.GetKjShjDiff(); 

	if (kjing < 60 )								//|| kjing > 540
	{
		MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
		return ;
	}


	bool bSucc = true;
	int itm_cnt = m_listChqSSC.GetItemCount();
	SendToServer(1);
	OnBnClickedBtnClsList();

	m_zongZhuShu = 0;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);

	InvalidateRect(&rcZongJinE);

	//m_beishu = 1;
	UpdateData(FALSE);
}

void CGuangDong11X5::OnBnClickedBtnDelSel()
{
	POSITION pos = m_listChqSSC.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listChqSSC.GetNextSelectedItem(pos);

		int zhushu = _ttoi(m_listChqSSC.GetItemText(nItem, 4));
		m_zongZhuShu -= zhushu;
		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=400;
		InvalidateRect(&rcInvali);

		InvalidateRect(&rcZongJinE);
		if(vecAllNums.size() > 0)
		{
			vector<wstring>::iterator   iter = vecAllNums.begin()+nItem;
			vecAllNums.erase(iter);

		}

		m_listChqSSC.DeleteItem(nItem);
	}
}

void CGuangDong11X5::OnBnClickedBtnClsList()
{
	m_zongZhuShu = 0;
	m_singlezhushu = 0;
	m_zongJine=0;
//	m_editBeiTou.SetWindowText(L"1");
	m_listChqSSC.deleteAllItemEx();
	//m_editDanShiHaoMa.SetWindowText(L"");
//	m_richDanshiHaoma.SetWindowText(L"");
	vecAllNums.clear();
	m_strHaoma.Empty();
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);

	InvalidateRect(&rcZongJinE);
	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);

}

void CGuangDong11X5::OnEnChangeEditDanshiInput()
{
	//m_editDanShiHaoMa.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
	m_richDanshiHaoma.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
}

void CGuangDong11X5::OnEnChangeEditBeishu()
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

void CGuangDong11X5::CheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(true);
}

void CGuangDong11X5::UnCheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(false);
}

bool CGuangDong11X5::IsNumBtnCheck(CNumerButton& btn)
{
	return btn.IsSelect();
}

void CGuangDong11X5::HideCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}
void CGuangDong11X5::ShowCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_SHOW);
	}
}

void CGuangDong11X5::HideAllRio()
{
	//��ѡ2,3,4
	HideCtrl(IDC_RIO_RENXUAN2);
	HideCtrl(IDC_RIO_RENXUAN3);
	HideCtrl(IDC_RIO_RENXUAN4);

	//����
	HideCtrl(IDC_RIO_HSZHX_FUSHI);
	HideCtrl(IDC_RIO_HSZHX_DANSHI);
	HideCtrl(IDC_RIO_RX1_FUSHI);

	HideCtrl(IDC_RIO_RERDT);
	HideCtrl(IDC_RIO_RSANDT);
	HideCtrl(IDC_RIO_RSIDT);
	HideCtrl(IDC_RIO_RWUDT);
	HideCtrl(IDC_RIO_RLIUDT);
	HideCtrl(IDC_RIO_RQIDT);
	HideCtrl(IDC_RIO_RBADT);

	//ǰ��
	HideCtrl(IDC_RIO_QSZHX_FUSHI);
	HideCtrl(IDC_RIO_QSZHX_DANSHI);

	HideCtrl(IDC_RIO_HSZS_FUSHI);
	//HideCtrl(IDC_RIO_HSZS_DANSHI);
	HideCtrl(IDC_RIO_HSZL_FUSHI);
	//HideCtrl(IDC_RIO_HSZL_DANSHI);

	HideCtrl(IDC_RIO_QSZS_FUSHI);
	//HideCtrl(IDC_RIO_QSZS_DANSHI);
	HideCtrl(IDC_RIO_QSZL_FUSHI);
	//HideCtrl(IDC_RIO_QSZL_DANSHI);

	HideCtrl(IDC_RIO_HEZHX_FUSHI);
	HideCtrl(IDC_RIO_HEZHX_DANSHI);

	HideCtrl(IDC_RIO_QEZHX_FUSHI);
	HideCtrl(IDC_RIO_QEZHX_DANSHI);

	HideCtrl(IDC_RIO_HEZX_FUSHI);
	HideCtrl(IDC_RIO_HEZX_DANSHI);

	HideCtrl(IDC_RIO_QEZX_FUSHI);
	HideCtrl(IDC_RIO_QEZX_DANSHI);

	HideCtrl(IDC_RIO_DXDSH);

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

//void CGuangDong11X5::SelctRadioBtn(int ctrl_id)
//{
//	CButton* pRadio = (CButton*)GetDlgItem(ctrl_id);
//	if (pRadio != NULL) 
//	{
//		pRadio->SetCheck(BST_CHECKED);
//	}
//}

CString CGuangDong11X5::GetWanString()
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
	if (IsNumBtnCheck(m_btnWan_10))
	{
		str += _T("11");
	}
	return str;
}

CString CGuangDong11X5::GetQianString()
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
	if (IsNumBtnCheck(m_btnQian_10))
	{
		str += _T("11");
	}
	return str;
}

CString CGuangDong11X5::GetBaiString()
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
	if (IsNumBtnCheck(m_btnBai_10))
	{
		str += _T("11");
	}
	return str;
}

CString CGuangDong11X5::GetShiString()
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
	if (IsNumBtnCheck(m_btnShi_10))
	{
		str += _T("11");
	}
	return str;
}

CString CGuangDong11X5::GetGeString()
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
	if (IsNumBtnCheck(m_btnGe_10))
	{
		str += _T("11");
	}
	return str;
}

CString CGuangDong11X5::GetShiDxdshString()
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

CString CGuangDong11X5::GetGeDxdshString()
{
	CString strGe;
	if (IsNumBtnCheck(m_btnDaGe)) 
	{
		strGe += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoGe)) 
	{
		strGe += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanGe)) 
	{
		strGe += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangGe)) 
	{
		strGe += _T("2");
	}

	return strGe;
}

CString CGuangDong11X5::GetWanDesc()
{
	return theGameKind[(m_TypeID << 16) | (unsigned short)m_gameKind];
}

CGuangDong11X5::SSCGameKind CGuangDong11X5::GetGameKindByDesc(const CString& desc)
{
	if (desc == _T("��ѡ��")) 
	{
		return IIRenXuan2;
	} 
	else if (desc == _T("��ѡһ")) 
	{
		return IIRenXuan1;
	} 
	else if (desc == _T("��ѡ��")) 
	{
		return IIRenXuan3;
	} 
	else if (desc == _T("ǰһ")) 
	{
		return QianYi;
	} 
	else if (desc == _T("��ѡ��")) 
	{
		return IIRenXuan4;
	}
	else if (desc == _T("ǰ��ֱѡѡ��")) 
	{
		return QianEr_ZhiXuan;
	}
	else if (desc == _T("ǰ����ѡѡ��")) 
	{
		return QianEr_ZuXuan;
	}
	else if (desc == _T("��ѡ��")) 
	{
		return IIRenXuan5;
	}
	else if (desc == _T("ǰ��ֱѡѡ��")) 
	{
		return QianSan_ZhiXuan;
	}
	else if (desc == _T("ǰ����ѡѡ��")) 
	{
		return QianSan_ZuXuan;
	}
	else if (desc == _T("��ѡ��")) 
	{
		return IIRenXuan6;
	}
	else if (desc == _T("��ѡ��")) 
	{
		return IIRenXuan7;
	}
	else if (desc == _T("��ѡ��"))
		return IIRenXuan8;
	else if(desc == _T("��2����"))
		return Ren2_Dantuo;
	else if(desc == _T("��3����"))
		return Ren3_Dantuo;
	else if(desc == _T("��4����"))
		return Ren4_Dantuo;
	else if(desc == _T("��5����"))
		return Ren5_Dantuo;
	else if(desc == _T("��6����"))
		return Ren6_Dantuo;
	else if(desc == _T("��7����"))
		return Ren7_Dantuo;
	else if(desc == _T("��8����"))
		return Ren8_Dantuo;

	return inValid;
}
int CGuangDong11X5::combo(int base, int up)
{
	//�˷����������
	//return factorial(base)/(factorial(up)*factorial(base-up));
	int tempUp = factorial(up);
	int tempInt = 1;
	for(int i = 0;i < up;i ++)
	{
		tempInt *= base - i;
	}
	return tempInt / tempUp;
}

int CGuangDong11X5::factorial(int number)
{
	int total = 1;
	for(int i = 1 ; i <= number; i++){
		total *= i;
	}
	return total;
}

void CGuangDong11X5::AdjustWanFa()
{
	if (m_btnDanTuo.GetSafeHwnd() != NULL) 
	{
		m_btnDanTuo.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*11-6*3, wanfa_y, m_btnDanTuo.Width(), m_btnDanTuo.Height(), SWP_NOZORDER);
	}
	if (m_btnRenXuan1.GetSafeHwnd() != NULL) 
	{
		m_btnRenXuan1.SetWindowPos(NULL, wanfa_x, wanfa_y, m_btnRenXuan1.Width(), m_btnRenXuan1.Height(), SWP_NOZORDER);
	}
	if (m_btnHouSanZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouSanZhiXuan.SetWindowPos(NULL, wanfa_x+ wanfa_col_width, wanfa_y, m_btnHouSanZhiXuan.Width(), m_btnHouSanZhiXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianSanZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianSanZhiXuan.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*7, wanfa_y /*+ wanfa_row_height*/, m_btnQianSanZhiXuan.Width(), m_btnQianSanZhiXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnHouSanZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouSanZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*2, wanfa_y, m_btnHouSanZuXuan.Width(), m_btnHouSanZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianSanZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianSanZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*8, wanfa_y , m_btnQianSanZuXuan.Width(), m_btnQianSanZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnWuXingHouEr.GetSafeHwnd() != NULL) 
	{
		m_btnWuXingHouEr.SetWindowPos(NULL, wanfa_x + wanfa_col_width*3, wanfa_y, m_btnWuXingHouEr.Width(), m_btnWuXingHouEr.Height(), SWP_NOZORDER);
	}

	if (m_btnWuXingQianEr.GetSafeHwnd() != NULL) 
	{
		m_btnWuXingQianEr.SetWindowPos(NULL, wanfa_x + wanfa_col_width*9-6, wanfa_y , m_btnWuXingQianEr.Width(), m_btnWuXingQianEr.Height(), SWP_NOZORDER);
	}

	if (m_btnHouErZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouErZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*4, wanfa_y, m_btnHouErZuXuan.Width(), m_btnHouErZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianErZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianErZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*10-6*2, wanfa_y , m_btnQianErZuXuan.Width(), m_btnQianErZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnDaXiaoDanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnDaXiaoDanShuang.SetWindowPos(NULL, wanfa_x + wanfa_col_width*5, wanfa_y, m_btnDaXiaoDanShuang.Width(), m_btnDaXiaoDanShuang.Height(), SWP_NOZORDER);
	}
	//����Ϊ ���
// 	if (m_btnHouSanHeZhi.GetSafeHwnd() != NULL) 
// 	{
// 		m_btnHouSanHeZhi.SetWindowPos(NULL, wanfa_x + wanfa_col_width*4, wanfa_y + wanfa_row_height, m_btnHouSanHeZhi.Width(), m_btnHouSanHeZhi.Height(), SWP_NOZORDER);
// 	}

	if (m_btnBuDingWei.GetSafeHwnd() != NULL) 
	{
		m_btnBuDingWei.SetWindowPos(NULL, wanfa_x + wanfa_col_width*6, wanfa_y, m_btnBuDingWei.Width(), m_btnBuDingWei.Height(), SWP_NOZORDER);
	}

// 	if (m_btnDingWeiDan.GetSafeHwnd() != NULL) 
// 	{
// 		m_btnDingWeiDan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*3, wanfa_y + wanfa_row_height, m_btnDingWeiDan.Width(), m_btnDingWeiDan.Height(), SWP_NOZORDER);
// 	}
}

//����Ͷע��ʽ ��ѡ��
void CGuangDong11X5::AdjustRadio()
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
	pWnd = GetDlgItem(IDC_RIO_RX1_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
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
	//--------------����--------------------------------------------
	pWnd = GetDlgItem(IDC_RIO_RERDT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RSANDT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RSIDT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RWUDT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RLIUDT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width*4, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RQIDT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width*5, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RBADT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width*6, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSZS_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HEZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HEZX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
void CGuangDong11X5::AdjustNumBtn()
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
	if(m_btnWan_10.GetSafeHwnd() != NULL) 
	{
		m_btnWan_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y, m_btnWan_10.Width(), m_btnWan_10.Height(), SWP_NOZORDER);
	}
	
	if (m_btnWanQuan.GetSafeHwnd() != NULL) 
	{
		m_btnWanQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+40, haoma_btn_y, m_btnWanQuan.Width(), m_btnWanQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnWanDa.GetSafeHwnd() != NULL) 
	{
		m_btnWanDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+40, haoma_btn_y, m_btnWanDa.Width(), m_btnWanDa.Height(), SWP_NOZORDER);
	}
	if (m_btnWanXiao.GetSafeHwnd() != NULL) 
	{
		m_btnWanXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+40, haoma_btn_y, m_btnWanXiao.Width(), m_btnWanXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnWanDan.GetSafeHwnd() != NULL) 
	{
		m_btnWanDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+47, haoma_btn_y, m_btnWanDan.Width(), m_btnWanDan.Height(), SWP_NOZORDER);
	}
	if (m_btnWanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnWanShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+47, haoma_btn_y, m_btnWanShuang.Width(), m_btnWanShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnWanQing.GetSafeHwnd() != NULL) 
	{
		m_btnWanQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+47, haoma_btn_y, m_btnWanQing.Width(), m_btnWanQing.Height(), SWP_NOZORDER);
	}


	//ǧλ-------------------------------
	if (m_staticQian.GetSafeHwnd() != NULL) 
	{
		m_staticQian.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height, m_staticQian.Width(), m_staticQian.Height(), SWP_NOZORDER);
	}
	if (m_staticDanMa.GetSafeHwnd() != NULL) 
	{
		m_staticDanMa.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height, m_staticDanMa.Width(), m_staticDanMa.Height(), SWP_NOZORDER);
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
	if(m_btnQian_10.GetSafeHwnd() != NULL) 
	{
		m_btnQian_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height, m_btnQian_10.Width(), m_btnQian_10.Height(), SWP_NOZORDER);
	}

	if (m_btnQianQuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+40, haoma_btn_y + haoma_btn_row_height, m_btnQianQuan.Width(), m_btnQianQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnQianDa.GetSafeHwnd() != NULL) 
	{
		m_btnQianDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+40, haoma_btn_y + haoma_btn_row_height, m_btnQianDa.Width(), m_btnQianDa.Height(), SWP_NOZORDER);
	}
	if (m_btnQianXiao.GetSafeHwnd() != NULL) 
	{
		m_btnQianXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+40, haoma_btn_y + haoma_btn_row_height, m_btnQianXiao.Width(), m_btnQianXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnQianDan.GetSafeHwnd() != NULL) 
	{
		m_btnQianDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+47, haoma_btn_y + haoma_btn_row_height, m_btnQianDan.Width(), m_btnQianDan.Height(), SWP_NOZORDER);
	}
	if (m_btnQianShuang.GetSafeHwnd() != NULL) 
	{
		m_btnQianShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+40+7, haoma_btn_y + haoma_btn_row_height, m_btnQianShuang.Width(), m_btnQianShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnQianQing.GetSafeHwnd() != NULL) 
	{
		m_btnQianQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+40+7, haoma_btn_y + haoma_btn_row_height, m_btnQianQing.Width(), m_btnQianQing.Height(), SWP_NOZORDER);
	}


	
	//��λ-------------------------------
	//��ѡ
	if (m_staticZuXuan.GetSafeHwnd() != NULL) 
	{
		m_staticZuXuan.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticTuoMa.GetSafeHwnd() != NULL) 
	{
		m_staticTuoMa.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticTuoMa.Width(), m_staticTuoMa.Height(), SWP_NOZORDER);
	}
	//����λ
	if (m_staticBuDingWei.GetSafeHwnd() != NULL) 
	{
		m_staticBuDingWei.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}

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
	if(m_btnBai_10.GetSafeHwnd() != NULL) 
	{
		m_btnBai_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_10.Width(), m_btnBai_10.Height(), SWP_NOZORDER);
	}

	if (m_btnBaiQuan.GetSafeHwnd() != NULL) 
	{
		m_btnBaiQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiQuan.Width(), m_btnBaiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiDa.GetSafeHwnd() != NULL) 
	{
		m_btnBaiDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiDa.Width(), m_btnBaiDa.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiXiao.GetSafeHwnd() != NULL) 
	{
		m_btnBaiXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiXiao.Width(), m_btnBaiXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiDan.GetSafeHwnd() != NULL) 
	{
		m_btnBaiDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiDan.Width(), m_btnBaiDan.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiShuang.GetSafeHwnd() != NULL) 
	{
		m_btnBaiShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiShuang.Width(), m_btnBaiShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiQing.GetSafeHwnd() != NULL) 
	{
		m_btnBaiQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiQing.Width(), m_btnBaiQing.Height(), SWP_NOZORDER);
	}

	//��С��˫
	if(m_btnDaShi.GetSafeHwnd() != NULL) 
	{
		m_btnDaShi.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*2, m_btnDaShi.Width(), m_btnDaShi.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoShi.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoShi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*2, m_btnXiaoShi.Width(), m_btnXiaoShi.Height(), SWP_NOZORDER);
	}
	if(m_btnDanShi.GetSafeHwnd() != NULL) 
	{
		m_btnDanShi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*2, m_btnDanShi.Width(), m_btnDanShi.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangShi.GetSafeHwnd() != NULL) 
	{
		m_btnShuangShi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*2, m_btnShuangShi.Width(), m_btnShuangShi.Height(), SWP_NOZORDER);
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
	if(m_btnShi_10.GetSafeHwnd() != NULL) 
	{
		m_btnShi_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_10.Width(), m_btnShi_10.Height(), SWP_NOZORDER);
	}

	if (m_btnShiQuan.GetSafeHwnd() != NULL) 
	{
		m_btnShiQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiQuan.Width(), m_btnShiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnShiDa.GetSafeHwnd() != NULL) 
	{
		m_btnShiDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiDa.Width(), m_btnShiDa.Height(), SWP_NOZORDER);
	}
	if (m_btnShiXiao.GetSafeHwnd() != NULL) 
	{
		m_btnShiXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiXiao.Width(), m_btnShiXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnShiDan.GetSafeHwnd() != NULL) 
	{
		m_btnShiDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+20*2+7, haoma_btn_y + haoma_btn_row_height*3, m_btnShiDan.Width(), m_btnShiDan.Height(), SWP_NOZORDER);
	}
	if (m_btnShiShuang.GetSafeHwnd() != NULL) 
	{
		m_btnShiShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+20*2+7, haoma_btn_y + haoma_btn_row_height*3, m_btnShiShuang.Width(), m_btnShiShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnShiQing.GetSafeHwnd() != NULL) 
	{
		m_btnShiQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+20*2+7, haoma_btn_y + haoma_btn_row_height*3, m_btnShiQing.Width(), m_btnShiQing.Height(), SWP_NOZORDER);
	}

	//��С��˫
	if(m_btnDaGe.GetSafeHwnd() != NULL) 
	{
		m_btnDaGe.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*3, m_btnDaGe.Width(), m_btnDaGe.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoGe.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoGe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*3, m_btnXiaoGe.Width(), m_btnXiaoGe.Height(), SWP_NOZORDER);
	}
	if(m_btnDanGe.GetSafeHwnd() != NULL) 
	{
		m_btnDanGe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*3, m_btnDanGe.Width(), m_btnDanGe.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangGe.GetSafeHwnd() != NULL) 
	{
		m_btnShuangGe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*3, m_btnShuangGe.Width(), m_btnShuangGe.Height(), SWP_NOZORDER);
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
	if(m_btnGe_10.GetSafeHwnd() != NULL) 
	{
		m_btnGe_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_10.Width(), m_btnGe_10.Height(), SWP_NOZORDER);
	}

	if (m_btnGeQuan.GetSafeHwnd() != NULL) 
	{
		m_btnGeQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGeQuan.Width(), m_btnGeQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnGeDa.GetSafeHwnd() != NULL) 
	{
		m_btnGeDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGeDa.Width(), m_btnGeDa.Height(), SWP_NOZORDER);
	}
	if (m_btnGeXiao.GetSafeHwnd() != NULL) 
	{
		m_btnGeXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGeXiao.Width(), m_btnGeXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnGeDan.GetSafeHwnd() != NULL) 
	{
		m_btnGeDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+20*2+7, haoma_btn_y + haoma_btn_row_height*4, m_btnGeDan.Width(), m_btnGeDan.Height(), SWP_NOZORDER);
	}
	if (m_btnGeShuang.GetSafeHwnd() != NULL) 
	{
		m_btnGeShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+20*2+7, haoma_btn_y + haoma_btn_row_height*4, m_btnGeShuang.Width(), m_btnGeShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnGeQing.GetSafeHwnd() != NULL) 
	{
		m_btnGeQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+20*2+7, haoma_btn_y + haoma_btn_row_height*4, m_btnGeQing.Width(), m_btnGeQing.Height(), SWP_NOZORDER);
	}


}

//��ť������ɾ�������㣬��Ͷ��Ͷע��׷��
void CGuangDong11X5::AdjustNumView()
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
		m_btnMoreRecord.SetWindowPos(NULL, btn_more_x, btn_more_y, m_btnMoreRecord.Width(), m_btnMoreRecord.Height(), SWP_NOZORDER);
	}
	if(m_btnQushi.GetSafeHwnd() != NULL) 
	{
		m_btnQushi.SetWindowPos(NULL, btn_more_x-45, btn_more_y, m_btnMoreRecord.Width(), m_btnMoreRecord.Height(), SWP_NOZORDER);
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
void CGuangDong11X5::AdjustDanShi()
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
void CGuangDong11X5::AdjustAdd()
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

}
void CGuangDong11X5::OnBnClickedBtnYuan()
{
	m_btnYuan.SetPushed(true);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Yuan ;

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);
	return;
}
void CGuangDong11X5::OnBnClickedBtnJiao()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(true);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);

	theAccount.Scoretype=MoShi_Jiao ;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);
	return;
}
void CGuangDong11X5::OnBnClickedBtnFen()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(true);
	m_btnLi.SetPushed(false);

	theAccount.Scoretype=MoShi_Fen ;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);
	return;

}
void CGuangDong11X5::OnBnClickedBtnLi()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(true);
	theAccount.Scoretype=MoShi_Li ;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);

	return;
}

void CGuangDong11X5::HideWanNums()
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
	m_btnWan_10.ShowWindow(SW_HIDE);

	m_staticWan.ShowWindow(SW_HIDE);

	m_btnWanQuan.ShowWindow(SW_HIDE);
	m_btnWanDa.ShowWindow(SW_HIDE);
	m_btnWanXiao.ShowWindow(SW_HIDE);
	m_btnWanDan.ShowWindow(SW_HIDE);
	m_btnWanShuang.ShowWindow(SW_HIDE);
	m_btnWanQing.ShowWindow(SW_HIDE);
}

void CGuangDong11X5::HideQianNums()
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
	m_btnQian_10.ShowWindow(SW_HIDE);

	m_staticQian.ShowWindow(SW_HIDE);
	m_staticDanMa.ShowWindow(SW_HIDE);

	m_staticTuoMa.ShowWindow(SW_HIDE);

	m_btnQianQuan.ShowWindow(SW_HIDE);
	m_btnQianDa.ShowWindow(SW_HIDE);
	m_btnQianXiao.ShowWindow(SW_HIDE);
	m_btnQianDan.ShowWindow(SW_HIDE);
	m_btnQianShuang.ShowWindow(SW_HIDE);
	m_btnQianQing.ShowWindow(SW_HIDE);
}

void CGuangDong11X5::HideBaiNums()
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
	m_btnBai_10.ShowWindow(SW_HIDE);

	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);

	m_btnBaiQuan.ShowWindow(SW_HIDE);
	m_btnBaiDa.ShowWindow(SW_HIDE);
	m_btnBaiXiao.ShowWindow(SW_HIDE);
	m_btnBaiDan.ShowWindow(SW_HIDE);
	m_btnBaiShuang.ShowWindow(SW_HIDE);
	m_btnBaiQing.ShowWindow(SW_HIDE);
}

void CGuangDong11X5::HideShiNums()
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
	m_btnShi_10.ShowWindow(SW_HIDE);

	m_staticShi.ShowWindow(SW_HIDE);

	m_btnShiQuan.ShowWindow(SW_HIDE);
	m_btnShiDa.ShowWindow(SW_HIDE);
	m_btnShiXiao.ShowWindow(SW_HIDE);
	m_btnShiDan.ShowWindow(SW_HIDE);
	m_btnShiShuang.ShowWindow(SW_HIDE);
	m_btnShiQing.ShowWindow(SW_HIDE);
}
void CGuangDong11X5::HideDanTuo()
{
	m_staticDanMa.ShowWindow(SW_HIDE);
	m_staticTuoMa.ShowWindow(SW_HIDE);
	HideBaiNums();
	HideQianNums();
}

void CGuangDong11X5::ShowDanTuo()
{
	m_staticDanMa.ShowWindow(SW_SHOW);
	m_staticTuoMa.ShowWindow(SW_SHOW);
	ShowQianNums();
	ShowBaiNums();

	m_staticQian.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);


}

void CGuangDong11X5::HideGeNums()
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
	m_btnGe_10.ShowWindow(SW_HIDE);

	m_staticGe.ShowWindow(SW_HIDE);

	m_btnGeQuan.ShowWindow(SW_HIDE);
	m_btnGeDa.ShowWindow(SW_HIDE);
	m_btnGeXiao.ShowWindow(SW_HIDE);
	m_btnGeDan.ShowWindow(SW_HIDE);
	m_btnGeShuang.ShowWindow(SW_HIDE);
	m_btnGeQing.ShowWindow(SW_HIDE);
}

void CGuangDong11X5::ShowWanNums()
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
	m_btnWan_10.ShowWindow(SW_SHOW);

	m_staticWan.ShowWindow(SW_SHOW);

	m_btnWanQuan.ShowWindow(SW_SHOW);
	m_btnWanDa.ShowWindow(SW_SHOW);
	m_btnWanXiao.ShowWindow(SW_SHOW);
	m_btnWanDan.ShowWindow(SW_SHOW);
	m_btnWanShuang.ShowWindow(SW_SHOW);
	m_btnWanQing.ShowWindow(SW_SHOW);
}

void CGuangDong11X5::ShowQianNums()
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
	m_btnQian_10.ShowWindow(SW_SHOW);

	m_staticQian.ShowWindow(SW_SHOW);

	m_btnQianQuan.ShowWindow(SW_SHOW);
	m_btnQianDa.ShowWindow(SW_SHOW);
	m_btnQianXiao.ShowWindow(SW_SHOW);
	m_btnQianDan.ShowWindow(SW_SHOW);
	m_btnQianShuang.ShowWindow(SW_SHOW);
	m_btnQianQing.ShowWindow(SW_SHOW);
}

void CGuangDong11X5::ShowBaiNums()
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
	m_btnBai_10.ShowWindow(SW_SHOW);

	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_SHOW);

	m_btnBaiQuan.ShowWindow(SW_SHOW);
	m_btnBaiDa.ShowWindow(SW_SHOW);
	m_btnBaiXiao.ShowWindow(SW_SHOW);
	m_btnBaiDan.ShowWindow(SW_SHOW);
	m_btnBaiShuang.ShowWindow(SW_SHOW);
	m_btnBaiQing.ShowWindow(SW_SHOW);
}

void CGuangDong11X5::ShowShiNums()
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
	m_btnShi_10.ShowWindow(SW_SHOW);

	m_staticShi.ShowWindow(SW_SHOW);

	m_btnShiQuan.ShowWindow(SW_SHOW);
	m_btnShiDa.ShowWindow(SW_SHOW);
	m_btnShiXiao.ShowWindow(SW_SHOW);
	m_btnShiDan.ShowWindow(SW_SHOW);
	m_btnShiShuang.ShowWindow(SW_SHOW);
	m_btnShiQing.ShowWindow(SW_SHOW);
}

void CGuangDong11X5::ShowGeNums()
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
	m_btnGe_10.ShowWindow(SW_SHOW);

	m_staticGe.ShowWindow(SW_SHOW);

	m_btnGeQuan.ShowWindow(SW_SHOW);
	m_btnGeDa.ShowWindow(SW_SHOW);
	m_btnGeXiao.ShowWindow(SW_SHOW);
	m_btnGeDan.ShowWindow(SW_SHOW);
	m_btnGeShuang.ShowWindow(SW_SHOW);
	m_btnGeQing.ShowWindow(SW_SHOW);
}

void CGuangDong11X5::ShowZuXuan()
{
	m_staticZuXuan.ShowWindow(SW_SHOW);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
}

// void CGuangDong11X5::HideZuXuan()
// {
// 	m_staticZuXuan.ShowWindow(SW_HIDE);
// 	m_staticBuDingWei.ShowWindow(SW_HIDE);
// 	m_staticBai.ShowWindow(SW_HIDE);
// }

void CGuangDong11X5::ShowBuDingWei()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_SHOW);
	m_staticBai.ShowWindow(SW_HIDE);
}

// void CGuangDong11X5::HideBuDingwei()
// {
// 	m_staticZuXuan.ShowWindow(SW_HIDE);
// 	m_staticBuDingWei.ShowWindow(SW_HIDE);
// 	m_staticBai.ShowWindow(SW_HIDE);
// }

void CGuangDong11X5::ShowDaXiaoDanShuang()
{
	m_staticBai.ShowWindow(SW_SHOW);
	m_staticShi.ShowWindow(SW_SHOW);

	m_btnDaShi.ShowWindow(SW_SHOW);
	m_btnXiaoShi.ShowWindow(SW_SHOW);
	m_btnDanShi.ShowWindow(SW_SHOW);
	m_btnShuangShi.ShowWindow(SW_SHOW);
	m_btnDaGe.ShowWindow(SW_SHOW);
	m_btnXiaoGe.ShowWindow(SW_SHOW);
	m_btnDanGe.ShowWindow(SW_SHOW);
	m_btnShuangGe.ShowWindow(SW_SHOW);
}

void CGuangDong11X5::HideDaXiaoDanShuang()
{
	m_btnDaShi.ShowWindow(SW_HIDE);
	m_btnXiaoShi.ShowWindow(SW_HIDE);
	m_btnDanShi.ShowWindow(SW_HIDE);
	m_btnShuangShi.ShowWindow(SW_HIDE);
	m_btnDaGe.ShowWindow(SW_HIDE);
	m_btnXiaoGe.ShowWindow(SW_HIDE);
	m_btnDanGe.ShowWindow(SW_HIDE);
	m_btnShuangGe.ShowWindow(SW_HIDE);
}

void CGuangDong11X5::HideDanShiEdit()
{
	//m_editDanShiHaoMa.ShowWindow(SW_HIDE);
	m_richDanshiHaoma.ShowWindow(SW_HIDE);
}

void CGuangDong11X5::ShowDanShiEdit()
{
	m_richDanshiHaoma.ShowWindow(SW_SHOW);
	//	m_editDanShiHaoMa.ShowWindow(SW_SHOW);
}

	//��ѡ���е���ǧ���٣�ʮ����λ��
void CGuangDong11X5::ShowRenxuanWeiShu()
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

void CGuangDong11X5::HideRenxuanWeiShu()
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

void CGuangDong11X5::CheckWanFaBtn(CTextButton& btn)
{
	m_btnRenXuan1.SetPushed(false);
	m_btnHouSanZhiXuan.SetPushed(false);
	m_btnQianSanZhiXuan.SetPushed(false);
	m_btnHouSanZuXuan.SetPushed(false);
	m_btnQianSanZuXuan.SetPushed(false);
	m_btnWuXingHouEr.SetPushed(false);
	m_btnWuXingQianEr.SetPushed(false);
	m_btnHouErZuXuan.SetPushed(false);
	m_btnQianErZuXuan.SetPushed(false);
	m_btnDaXiaoDanShuang.SetPushed(false);
	m_btnDanTuo.SetPushed(false);
	m_btnBuDingWei.SetPushed(false);
	//m_btnDingWeiDan.SetPushed(false);

	btn.SetPushed(true);
}
VOID CGuangDong11X5::SendQuerySystemTime()
{
	return;
}
VOID CGuangDong11X5::SendToServer(int nSendType)
{

	if(nSendType == 1)
	{
		m_bTouzhu = true;
		if(m_bTouzhu)
		{
			m_bTouzhu = false;
			m_nTzhSign++;

			{
			
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

					if(nNowKind == nKind && nNowBeishu == nBeishu && nNowMoshi == nMoshi && nNowMoshi!= 10000 && nNowBeishu != 100000 && nNowKind != 10000)
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

						if (kind == nNowKind && beishu == nNowBeishu&& scoremoshi == nNowMoshi)
						{
							CString strZhu = m_listChqSSC.GetItemText(j, 4);
							nZhushu += _ttoi(strZhu);
							int nSize = mapHaoma.size();
							mapHaoma[nSize] = j;

							CString haoma = m_listChqSSC.GetItemText(j, 0);
							if(haoma==_T("������ӳɹ�"))
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
					TouzhuCQSSC.nGameType = m_TypeID;
					TouzhuCQSSC.nMoshi = nNowMoshi;

					TouzhuCQSSC.nSign = m_nTzhSign;

					CString strQiHao;
					if(m_TypeID == CZGD11Xuan5)
						strQiHao = m_chqRule.GetNextExpect();
					else if(m_TypeID == CZJX11Xuan5)
						strQiHao = m_jxRule.GetNextExpect();
					else if(m_TypeID == CZSD11Xuan5)
						strQiHao = m_sdRule.GetNextExpect();
					else if(m_TypeID == CZHLJ11Xuan5)
						strQiHao = m_hljRule.GetNextExpect();

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
			}
		//	m_nTzhSign++;
			//m_DlgStatus.ShowStatusWindow(TEXT("�����ύ���ݣ����Ժ�..."));
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
		if(m_RenxuanKeyboardInput == false)
		{
			if(m_gameKind == IIRenXuan2)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ���������������Ϻ��룬������ڿ��������а�����ѡ���뼴Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan3)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ���������������Ϻ��룬������ڿ��������а�����ѡ���뼴Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan1)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ��һ����һ�����Ϻ��룬������ڿ��������а�����ѡ���뼴Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan4)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ���ĸ����ĸ����Ϻ��룬������ڿ��������а�����ѡ���뼴Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan5)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ�������������Ϻ��룬������ڿ��������а�����ѡ���뼴Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan6)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ���������������Ϻ��룬������ڿ��������а�����ѡ���뼴Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan7)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ���߸����߸����Ϻ��룬������ڿ��������а�����ѡ���뼴Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan8)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ��˸���˸����Ϻ��룬������ڿ��������а�����ѡ���뼴Ϊ�н�"));
			}
			else if(m_gameKind == QianYi)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ��һ����һ�����Ϻ��룬������ڿ��������е�һλ����ѡ���뼴Ϊ�н�"));
			}
			else if(m_gameKind == QianEr_ZhiXuan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ӵ�һλ�͵ڶ�λ�����ٸ�ѡ��һ�����룬���Ҳ����ظ�����������ǰ��λ����ѡ������ͬ������˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == QianEr_ZuXuan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ���������������Ϻ��룬�����ѡ�����뵱�ڿ���������ǰ��λ��ͬ��˳���ޣ���Ϊ�н�"));
			}
			else if(m_gameKind == QianSan_ZhiXuan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ӵ�һλ���ڶ�λ�͵���λ�����ٸ�ѡ��һ�����룬���Ҳ����ظ�����������ǰ��λ����ѡ������ͬ������˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == QianSan_ZuXuan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��01-11������ѡ���������������Ϻ��룬�����ѡ�����뵱�ڿ���������ǰ��λ��ͬ��˳���ޣ���Ϊ�н�"));
			}
			else if(m_gameKind == Ren2_Dantuo)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֱ�ӵ����������ѡ������1�������1���������һע��ֻҪ5������������ͬʱ������ѡ��1�������1�����룬��Ϊ�н���"));
			}
			else if(m_gameKind == Ren3_Dantuo)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֱ�ӵ����������ѡ������1�������2���������һע��ֻҪ5������������ͬʱ������ѡ��1�������2�����룬��Ϊ�н���"));
			}
			else if(m_gameKind == Ren4_Dantuo)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֱ�ӵ����������ѡ������1�������3���������һע��ֻҪ5������������ͬʱ������ѡ��1�������3�����룬��Ϊ�н���"));
			}
			else if(m_gameKind == Ren5_Dantuo)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֱ�ӵ����������ѡ������1�������4���������һע��ֻҪ5������������ͬʱ������ѡ��1�������4�����룬��Ϊ�н���"));
			}
			else if(m_gameKind == Ren6_Dantuo)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֱ�ӵ����������ѡ������1�������5���������һע��ֻҪ��ѡ��ÿע6�����뵱�У���5���͵��ڵ�5����������ȫ����ͬ����Ϊ�н���"));
			}
			else if(m_gameKind == Ren7_Dantuo)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֱ�ӵ����������ѡ������1�������6���������һע��ֻҪ��ѡ��ÿע7�����뵱�У���5���͵��ڵ�5����������ȫ����ͬ����Ϊ�н���"));
			}
			else if(m_gameKind == Ren8_Dantuo)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֱ�ӵ����������ѡ������1�������7���������һע��ֻҪ��ѡ��ÿע8�����뵱�У���5���͵��ڵ�5����������ȫ����ͬ����Ϊ�н���"));
			}
		}
		else
		{
			if(m_gameKind == IIRenXuan2)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��01-11����������2����λ���������һע��ֻҪ���ڿ��������а���������룬��Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan3)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��01-11����������3����λ���������һע��ֻҪ���ڿ��������а���������룬��Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan1)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��01-11����������1����λ���������һע��ֻҪ���ڿ��������а���������룬��Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan4)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��01-11����������4����λ���������һע��ֻҪ���ڿ��������а���������룬��Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan5)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��01-11����������5����λ���������һע��ֻҪ���ڿ��������а���������룬��Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan6)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��01-11����������6����λ���������һע��ֻҪ���ڿ��������а���������룬��Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan7)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��01-11����������7����λ���������һע��ֻҪ���ڿ��������а���������룬��Ϊ�н�"));
			}
			else if(m_gameKind == IIRenXuan8)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��01-11����������8����λ���������һע��ֻҪ���ڿ��������а���������룬��Ϊ�н�"));
			}
			else if(m_gameKind == QianEr_ZhiXuan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��������2����λ���������һע��������ĺ����뵱�ڿ��������е�ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == QianEr_ZuXuan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��������2����λ���������һע��������ĺ����뵱�ڿ��������е�ǰ��λ��ͬ����˳���ޣ���Ϊ�н�"));
			}
			else if(m_gameKind == QianSan_ZhiXuan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��������3����λ���������һע��������ĺ����뵱�ڿ��������е�ǰ��λ��ͬ����˳��һ�£���Ϊ�н�"));
			}
			else if(m_gameKind == QianSan_ZuXuan)
			{
				m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("�ֶ�������룬��������3����λ���������һע��������ĺ����뵱�ڿ��������е�ǰ��λ��ͬ����˳���ޣ���Ϊ�н�"));
			}
		}

	//	m_zongZhuShu = 0;
// 		m_editBeiTou.SetWindowText(L"1");
// 		m_beishu=1;
		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=400;
		InvalidateRect(&rcInvali);

		InvalidateRect(&rcZongJinE);
		return;
	}

	if(nSendType == 4)
	{
		m_bZhuihao = true;
		if(m_bZhuihao)
		{
			m_nTzhSign++;
			m_DlgStatus.ShowStatusWindow(TEXT("�����ύ���ݣ����Ժ�..."));
			m_bZhuihao = false;
			int itm_cnt = m_listChqSSC.GetItemCount();

			CMD_GP_TouZhuCQSSC_Zhuihao TouzhuCQSSC;
			ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
			TouzhuCQSSC.dwUserID = theAccount.user_id;
			TouzhuCQSSC.nGameType = m_TypeID;

			TouzhuCQSSC.nSign = m_nTzhSign;
			if(m_bZhuiZjtz)
				TouzhuCQSSC.bZhuihao = GetTickCount();
			else
				TouzhuCQSSC.bZhuihao = 2;
			for(int n=0; n< itm_cnt; n++)
			{
				int nSendCount = 0;
				int nEnd = 1;



				CString wanfa = m_listChqSSC.GetItemText(n, 3);
				int kind = GetGameKindByDesc(wanfa);

				TouzhuCQSSC.nGameKind = kind;
				CString moshi = m_listChqSSC.GetItemText(n, 1);
				int nScoreType = GetMoshiRet(moshi);
				TouzhuCQSSC.nMoshi = nScoreType;

				m_strHaoma = vecAllNums[itm_cnt-n-1].c_str();
				if(m_strHaoma.IsEmpty())
				{
					m_DlgStatus.HideStatusWindow();
					MyMessageBox(L"�㻹δѡ����룬��ѡ��");
					OnBnClickedBtnClsList();
					return ;
				}
				if(m_strHaoma[m_strHaoma.GetLength()-1] != '|')
					m_strHaoma+=TEXT("|");

				string	strAllHaoma;
				ChangeStringToA(m_strHaoma, strAllHaoma);

				memset(TouzhuCQSSC.strQishu,0,sizeof(TouzhuCQSSC.strQishu));
				CString strQiHao;
				if(m_TypeID == CZGD11Xuan5)
					strQiHao = m_chqRule.GetNextExpect();
				else if(m_TypeID == CZJX11Xuan5)
					strQiHao = m_jxRule.GetNextExpect();
				else if(m_TypeID == CZSD11Xuan5)
					strQiHao = m_sdRule.GetNextExpect();
				else if(m_TypeID == CZHLJ11Xuan5)
					strQiHao = m_hljRule.GetNextExpect();
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
	//		m_nTzhSign++;
			OnBnClickedBtnClsList();

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
			SetUserBonus.n_t_type_id = m_TypeID;
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
			GetUserFandian.n_t_type_id = m_TypeID;
			GetUserFandian.n_t_kind_id = m_gameKind;

			//��������
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_FANDIAN,&GetUserFandian,sizeof(GetUserFandian));


		}
		return;
	}

	if(nSendType == 7)
	{
		return;
	}
	if(nSendType == 8)
	{
		CMD_GP_GetMoreRecord GetMoreRecord;
		ZeroMemory(&GetMoreRecord,sizeof(GetMoreRecord));

		GetMoreRecord.n_t_type = m_TypeID;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_MORE_RECORD,&GetMoreRecord,sizeof(GetMoreRecord));
		return;

	}
	if(nSendType == 11)
	{
		CMD_GP_QueryStatusLottery QueryStatusLottery;
		ZeroMemory(&QueryStatusLottery,sizeof(QueryStatusLottery));

		QueryStatusLottery.n_t_kindid = m_TypeID;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_STATUS_LOTTERY,&QueryStatusLottery,sizeof(QueryStatusLottery));
	}
}
VOID CGuangDong11X5::SendQueryLuckyNumCQSSC()
{
	return;
}


//�������˷���ǰN����������
bool CGuangDong11X5::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex,int nTypeID)
{
	_tcscpy_s(m_kjXinxi[nTypeID][nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nTypeID][nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nTypeID][nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);

	if(nIndex+1 < 4)
		return true;
	m_kjXinxiCont = 4;

	if (_tcscmp(m_lastExpect, m_kjXinxi[nTypeID][0].qihao) != 0) 
	{
		//��һ�ν���
		//if(m_lastExpect[0] == 0)
			_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[nTypeID][0].qihao);
		_tcscpy_s(CGD11X5Rule::m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[nTypeID][0].qihao);
		_tcscpy_s(CGD11X5Rule::m_lastKjShj, KJ_SHIJIAN_LENGTH, m_kjXinxi[nTypeID][0].shijian);

		m_bKaiJiangzhong=false;											//��������
		KillTimer(timer_id_kaijiangzhong);
	//	Invalidate(FALSE);
	}
	//if(IsWindowVisible())
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

bool CGuangDong11X5::LuckyNumCQSSC()
{
		return true;
	CString xmlurl = _T("http://www.cailele.com/static/gd11x5/newlyopenlist.xml");

	CString strPath;  //�ļ�·��
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	int pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\gd11x5.xml"); 

//  	if (FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)))
//  	{
//  		return false;
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

		_tcscpy_s(m_kjXinxi[m_TypeID][count].qihao, KJ_QIHAO_LENGTH, strexpect);
		_tcscpy_s(m_kjXinxi[m_TypeID][count].haoma, KJ_HAOMA_LENGTH, stropencode);
		_tcscpy_s(m_kjXinxi[m_TypeID][count].shijian, KJ_SHIJIAN_LENGTH, stropentime);

		count ++;
	}

	m_kjXinxiCont = count;

	if (_tcscmp(m_lastExpect, m_kjXinxi[m_TypeID][0].qihao) != 0) 
	{
		//��һ�ν���
		if(m_lastExpect[0] == 0)
			_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[m_TypeID][0].qihao);
		_tcscpy_s(CGD11X5Rule::m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[m_TypeID][0].qihao);
		_tcscpy_s(CGD11X5Rule::m_lastKjShj, KJ_SHIJIAN_LENGTH, m_kjXinxi[m_TypeID][0].shijian);

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


long CGuangDong11X5::GetKjShjDiff()
{
	long secDiff = m_chqRule.GetKjShjDiff();

	if (secDiff == m_chqRule.GetQiSpan() )
	{
		return 0;
	}
	else if(secDiff > 540)
		return -1;
	return secDiff;
}


//���ڿ�������
void CGuangDong11X5::DrawLastKjHaoma(CDC* pDC, Graphics& graphics)
{
	long secDiff = m_chqRule.GetKjShjDiff();
	if(m_TypeID == CZJX11Xuan5)
	{	
		secDiff = m_jxRule.GetKjShjDiff();
	}
	else if(m_TypeID == CZSD11Xuan5)
	{
		secDiff = m_sdRule.GetKjShjDiff();
	}
	else if(m_TypeID == CZHLJ11Xuan5)
	{
		secDiff = m_hljRule.GetKjShjDiff();
	}
	if(IsWindowVisible() && secDiff > 60 && secDiff <=65)
	{
		CPlatformFrame* pPlatforFrame = CPlatformFrame::GetInstance();
		if(pPlatforFrame->m_bSound)
			sndPlaySound(MAKEINTRESOURCE(IDR_CLOCK),  SND_RESOURCE |SND_ASYNC);
	}
	if(secDiff == 60)
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
	if (_tcscmp(m_lastExpect, m_kjXinxi[m_TypeID][0].qihao) != 0) 
	{
		_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[m_TypeID][0].qihao);
		m_bKaiJiangzhong=false;

	}
	else if(nTimeLeft == 0)
	{
		m_bKaiJiangzhong=true;
	}

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&m_KjInfoFont1);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(255, 255, 255));

	if(m_TypeID == CZGD11Xuan5)
		pDC->DrawText(_T("�㶫11ѡ5"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	else if(m_TypeID == CZJX11Xuan5)
		pDC->DrawText(_T("����11ѡ5"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	else if(m_TypeID == CZSD11Xuan5)
		pDC->DrawText(_T("ɽ��11ѡ5"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	else if(m_TypeID == CZHLJ11Xuan5)
		pDC->DrawText(_T("������11ѡ5"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	CRect rcDi;
	rcDi.left = rcCzh.right+10;
	rcDi.right = rcDi.left+20;
	rcDi.top = rcCzh.top;
	rcDi.bottom = rcCzh.bottom;
	pDC->DrawText(_T("��"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(253, 191, 6));

	if(m_TypeID == CZGD11Xuan5)
	{
	//	if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_chqRule.GetNextExpect(-1), &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
// 		else
// 			pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZJX11Xuan5)
	{
//		if(m_bKaiJiangzhong)
//		{
			pDC->DrawText(m_jxRule.GetNextExpect(-1), &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
// 		}
// 		else
// 			pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	}
	else if(m_TypeID == CZSD11Xuan5)
	{
//		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_sdRule.GetNextExpect(-1), &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
// 		else
// 			pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	}
	else if(m_TypeID == CZHLJ11Xuan5)
	{
// 		if(m_bKaiJiangzhong)
// 		{
			pDC->DrawText(m_hljRule.GetNextExpect(-1), &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
// 		}
// 		else
// 			pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	}
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

	if(m_cbIfTs==1)
	{
		strYg.Format(_T("��ͣ����"));
	}
	else
	{
			if(m_TypeID == CZGD11Xuan5)
			{
				if(secDiff > 60)
					strYg.Format(_T("��%s	�µ�ʱ��"), m_chqRule.GetNextExpect());
				else
					strYg.Format(_T("��%s	�ⵥʱ��"), m_chqRule.GetNextExpect());
			}
			else if(m_TypeID == CZJX11Xuan5)
			{
				if(secDiff > 60)
					strYg.Format(_T("��%s	�µ�ʱ��"), m_jxRule.GetNextExpect());
				else
					strYg.Format(_T("��%s	�ⵥʱ��"), m_jxRule.GetNextExpect());
			}
			else if(m_TypeID == CZSD11Xuan5)
			{
				if(secDiff > 60)
					strYg.Format(_T("��%s	�µ�ʱ��"), m_sdRule.GetNextExpect());
				else
					strYg.Format(_T("��%s	�ⵥʱ��"), m_sdRule.GetNextExpect());
			}
			else if(m_TypeID == CZHLJ11Xuan5)
			{
				if(secDiff > 60)
					strYg.Format(_T("��%s	�µ�ʱ��"), m_hljRule.GetNextExpect());
				else
					strYg.Format(_T("��%s	�ⵥʱ��"), m_hljRule.GetNextExpect());
			}

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
	if(m_TypeID == CZGD11Xuan5)
		strYg.Format(_T("%s"), m_chqRule.GetKjShjDiffDesc(60));
	else if(m_TypeID == CZJX11Xuan5)
		strYg.Format(_T("%s"), m_jxRule.GetKjShjDiffDesc(60));
	else if(m_TypeID == CZSD11Xuan5)
		strYg.Format(_T("%s"), m_sdRule.GetKjShjDiffDesc(60));
	else if(m_TypeID == CZHLJ11Xuan5)
		strYg.Format(_T("%s"), m_hljRule.GetKjShjDiffDesc(60));


	pDC->SetTextColor(RGB(219, 219, 219));
	rcYuGao1.left = rcYuGao1.left-75;
	rcYuGao1.top = rcYuGao1.top+12;
	rcYuGao1.bottom = rcYuGao1.bottom+20;
	rcYuGao1.right = rcYuGao1.right-20;
	pDC->DrawText(strYg, &rcYuGao1,DT_CENTER|DT_SINGLELINE);

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(&pOldFont);
	pDC->SetBkMode(oldBkMode);

	CString str = m_kjXinxi[m_TypeID][0].haoma;

	CString temp;
	temp.Format(L"%s",str.Left(2));

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
			int num = 0;//_ttoi(CString(str.GetAt(i)));

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
					DrawBigNum(graphics, m_kjNumBig, Rect(kj_big_haoma_x + index * (m_bigNumWidth+2), kj_big_haoma_y, m_bigNumWidth, m_bigNumHeight), num);
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
void CGuangDong11X5::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(250 ,250 ,250));

	for (int cnt=0; cnt<4; cnt++) 
	{
		CString str = m_kjXinxi[m_TypeID][cnt].qihao;
		if(str.IsEmpty())
		{
			continue;
		}

		CRect rcText(kj_small_haoma_x - 80, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 80, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[m_TypeID][cnt].haoma;
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

void CGuangDong11X5::DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, (num-1)*m_bigNumWidth, 0, m_bigNumWidth, m_bigNumHeight, UnitPixel);
}

void CGuangDong11X5::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, (num-1)*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);
}

void CGuangDong11X5::DrawZongZhushuJinE(CDC* pDC)
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

void CGuangDong11X5::ResetAllNums()
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
	UnCheckNumBtn(m_btnWan_10);

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
	UnCheckNumBtn(m_btnQian_10);

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
	UnCheckNumBtn(m_btnBai_10);

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
	UnCheckNumBtn(m_btnShi_10);

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
	UnCheckNumBtn(m_btnGe_10);

	UnCheckNumBtn(m_btnDaShi);
	UnCheckNumBtn(m_btnXiaoShi);
	UnCheckNumBtn(m_btnDanShi);
	UnCheckNumBtn(m_btnShuangShi);
	UnCheckNumBtn(m_btnDaGe);
	UnCheckNumBtn(m_btnXiaoGe);
	UnCheckNumBtn(m_btnDanGe);
	UnCheckNumBtn(m_btnShuangGe);

	m_richDanshiHaoma.SetWindowText(_T(""));

	//m_editDanShiHaoMa.SetWindowText(_T(""));
}

void CGuangDong11X5::SetLockText(float fBonus, float fBonusPercent)
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
bool CGuangDong11X5::GetZuSanHaoma(CStringArray& strArr)
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

bool CGuangDong11X5::GetZuSanHaoma(CString& haoma, int& zhushu)
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

bool CGuangDong11X5::IsZuSanHaoma(const CString& str)
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

bool CGuangDong11X5::GetDanShiHaoma(CStringArray& strArr, int size)
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
// #include <algorithm>
// int ismun(string strinfo)  
// {  
// 	string strset="1234567890";  
// 	int first = strinfo.find_first_of(strset);  
// 	if(first == string::npos)   
// 	{  
// 		return -1;  
// 	}   
// 
// 	return 0;  
// }
#include <algorithm>
#include <functional> 
int ismun(string strinfo)  
{  
	string strset="1234567890";  
	int first = strinfo.find_first_of(strset);  
	if(first == string::npos)   
	{  
		return -1;  
	}   

	return 0;  
}

bool CGuangDong11X5::GetDanShiHaoma(CString& haoma, int size, int nLeast)
{

	CStringArray strArr;
	CString line;
	m_richDanshiHaoma.GetWindowText(line);
	if(line.IsEmpty())
	{
		return false;
	}
	string strDanHoa;
	ChangeStringToA(line,strDanHoa);
	int nIndex = 0;

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
			SplitString2(strTemp,vectTemp,",");
			int nSize = vectTemp.size();
			for(int j = 0;j < nSize;j++)
			{
				string strMy;
				strMy=vectTemp[j];
				CString strMyNum;
				strMyNum = strMy.c_str();
				int nNum = _ttoi(strMyNum);
				if(nNum>11||nNum<1)
					return false;
				for (int k = j+1;k < nSize;k++)
				{
					strMy=vectTemp[j];
					strMyNum = strMy.c_str();
					int nNum = _ttoi(strMyNum);
					if(nNum>11||nNum<1)
						return false;
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

		int nNum = ismun(strTemp);


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
		SplitString2(strTemp,vectTemp,",");
		int nSize = vectTemp.size();
		for(int j = 0;j < nSize;j++)
		{
			string strMy;
			strMy=vectTemp[j];
			CString strMyNum;
			strMyNum = strMy.c_str();
			int nNum = _ttoi(strMyNum);
			if(nNum>11||nNum<1)
				return false;

			for (int k = j+1;k < nSize;k++)
			{
				strMy=vectTemp[j];
				strMyNum = strMy.c_str();
				int nNum = _ttoi(strMyNum);
				if(nNum>11||nNum<1)
					return false;

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
CString	CGuangDong11X5::ChangeStringToT(CStringA strInput)
{
	CString  strOutput;
	USES_CONVERSION;
	strOutput = A2T(strInput);

	return strOutput;
}

bool CGuangDong11X5::GetHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast)
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
void CGuangDong11X5::InitWanFaMoShi()
{
	CaiZhong nCaiZhong[4] = {CZGD11Xuan5,CZJX11Xuan5,CZSD11Xuan5,CZHLJ11Xuan5};

	for(int i = 0;i < CountArray(nCaiZhong);i++)
	{
		CaiZhong nCai = nCaiZhong[i];

		theGameKind[(nCai << 16) | (unsigned short)IIRenXuan1] = _T("��ѡһ");		
		theGameKind[(nCai << 16) | (unsigned short)IIRenXuan2] = _T("��ѡ��");		

		theGameKind[(nCai << 16) | (unsigned short)IIRenXuan3] =  _T("��ѡ��");

		theGameKind[(nCai << 16) | (unsigned short)QianYi] = _T("ǰһ");

		theGameKind[(nCai << 16) | (unsigned short)IIRenXuan4] = _T("��ѡ��");

		theGameKind[(nCai << 16) | (unsigned short)QianEr_ZhiXuan] = _T("ǰ��ֱѡѡ��");
		theGameKind[(nCai << 16) | (unsigned short)QianEr_ZuXuan] = _T("ǰ����ѡѡ��");

		theGameKind[(nCai << 16) | (unsigned short)IIRenXuan5] = _T("��ѡ��");

		theGameKind[(nCai << 16) | (unsigned short)QianSan_ZhiXuan] = _T("ǰ��ֱѡѡ��");
		theGameKind[(nCai << 16) | (unsigned short)QianSan_ZuXuan] = _T("ǰ����ѡѡ��");

		theGameKind[(nCai << 16) | (unsigned short)IIRenXuan6] = _T("��ѡ��");

		theGameKind[(nCai << 16) | (unsigned short)IIRenXuan7] = _T("��ѡ��");

		theGameKind[(nCai << 16) | (unsigned short)IIRenXuan8]=_T("��ѡ��");					//��ѡ2,3,4
		theGameKind[(nCai << 16) | (unsigned short)Ren2_Dantuo] = _T("��2����");
		theGameKind[(nCai << 16) | (unsigned short)Ren3_Dantuo] = _T("��3����");
		theGameKind[(nCai << 16) | (unsigned short)Ren4_Dantuo] = _T("��4����");
		theGameKind[(nCai << 16) | (unsigned short)Ren5_Dantuo] = _T("��5����");
		theGameKind[(nCai << 16) | (unsigned short)Ren6_Dantuo] = _T("��6����");
		theGameKind[(nCai << 16) | (unsigned short)Ren7_Dantuo] = _T("��7����");
		theGameKind[(nCai << 16) | (unsigned short)Ren8_Dantuo] = _T("��8����");
	}
}

bool	CGuangDong11X5::ChangeStringToA(CString strInput, string&  strOutput)
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
bool CGuangDong11X5::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
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
					CString strLog;
					strLog.Format(L"RENXUAN [%s] : %s",theGameKind[(m_TypeID << 16) | (unsigned short)nKindid],strText);
					OutputDebugString(strLog);
					mapFandian.insert(make_pair(nKindid,strText));

				}
				m_btnLock.SetWindowText(mapFandian[m_gameKind]);
				m_btnLock.EnableTextButton(false);
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
	 
	 				_tcscpy_s(m_kjXinxi[m_TypeID][m_kjXinxiCont].qihao, KJ_QIHAO_LENGTH, QueryLotResult.wPeriod);
	 				_tcscpy_s(m_kjXinxi[m_TypeID][m_kjXinxiCont].haoma, KJ_HAOMA_LENGTH, QueryLotResult.szLotNum);
	 				_tcscpy_s(m_kjXinxi[m_TypeID][m_kjXinxiCont].shijian, KJ_HAOMA_LENGTH, QueryLotResult.szShijian);
					m_kjXinxiCont++;
				}

 				CRect rcClient;
 				GetClientRect(rcClient);
				if (_tcscmp(m_lastExpect, m_kjXinxi[m_TypeID][0].qihao) != 0) 
				{
					//��һ�ν���
					if(m_lastExpect[0] == 0)
						_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[m_TypeID][0].qihao);
					_tcscpy_s(CGD11X5Rule::m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[m_TypeID][0].qihao);
					_tcscpy_s(CGD11X5Rule::m_lastKjShj, KJ_SHIJIAN_LENGTH, m_kjXinxi[m_TypeID][0].shijian);

					m_bKaiJiangzhong=false;											//��������
					KillTimer(timer_id_kaijiangzhong);
				//	Invalidate(FALSE);
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

						m_listChqSSC.DeleteAllItems();
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
				else if(lResult == 112)
				{
					CInformation information;
					int nResult = information.ShowMessageBox(_T("�����쳣�������µ�½��"),MB_ICONWARNING,30);
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

				if(pLogRet->n_t_kindid != m_TypeID)
					return true;

				m_cbIfTs = pLogRet->c_t_ifts;
				return true;
			}
		default:
			{
				return true;
			}
		}
	}

	//�������
	ASSERT(FALSE);

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
DWORD theBeginCount = 0;
CCriticalSection theTimeCS;
//�û��˺���Ϣ
//BaseUser theAccount;
//�ͷ���ַ
CString theKeFuUrl;
/////////////////////////////////��Ϸ�б�/////////////////////////////////////



//�����Ϣ
VOID CGuangDong11X5::OnMouseMove(UINT nFlags, CPoint Point)
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
VOID CGuangDong11X5::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//���ý���
	SetFocus();

	return;
}
//�����Ϣ
VOID CGuangDong11X5::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
}

//�����Ϣ
LRESULT CGuangDong11X5::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{

	//����״̬
	m_bHovering=false;

	return 0;
}


//������루��ѡ2����ѡ3����ѡ4��
void CGuangDong11X5::OnBnClickedRioMouseInput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
////��ѡ2,3,4
//	m_listChqSSC.DeleteAllItems();
	ResetAllNums();
	if(m_gameKind == IIRenXuan1)			//��ѡ��
	{
		m_rioRenXuan1.SetPushed(true);
		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();


		ShowZuXuan();
		HideDanShiEdit();
	}
	if(m_gameKind == IIRenXuan2)			//��ѡ��
	{
		m_rioHsZhxFuShi.SetPushed(true);
		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();


		ShowZuXuan();
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

		ShowZuXuan();
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

		ShowZuXuan();
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

		ShowZuXuan();
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

		ShowZuXuan();
		HideDanShiEdit();

	}

	if(m_gameKind ==IIRenXuan7)				//��ѡ��	
	{

		m_rioQsbdw.SetPushed(false);
		m_rioHsbdw.SetPushed(true);

		HideDaXiaoDanShuang();
		HideWanNums();
		HideQianNums();
		ShowBaiNums();
		HideShiNums();
		HideGeNums();

		ShowZuXuan();
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

		ShowZuXuan();
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
void CGuangDong11X5::OnBnClickedRioKeyboardInput()
{
//	m_listChqSSC.DeleteAllItems();
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
void CGuangDong11X5::SetCheckRenXuan(bool bChecked)
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

void CGuangDong11X5::OnBnClickedChkWan()
{
	ValidateTips();
}
void CGuangDong11X5::OnBnClickedChkQian()
{
	ValidateTips();
}

void CGuangDong11X5::OnBnClickedChkBai()
{
	ValidateTips();
}

void CGuangDong11X5::OnBnClickedChkShi()
{
	ValidateTips();
}

void CGuangDong11X5::OnBnClickedChkge()
{
	ValidateTips();
}

//������ʾ
void CGuangDong11X5::ValidateTips()
{
	return;
}

//��ѡ����ǧ���٣�ʮ����
int CGuangDong11X5::GetRenXuanCheckBox(BOOL& bWan, BOOL& bQian, BOOL& bBai, BOOL& bShi, BOOL& bGe)		
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
void CGuangDong11X5::OnBnClickedBtnLock()
{
	int index = (m_TypeID << 16) | (unsigned short)m_gameKind;

	
	CDlgFandian	dlg;
	dlg.SetBonus(mapBonus[m_gameKind], theBonus[index], mapPercent[m_gameKind], theAccount.fandian*100.0, mapDanweiFandian[m_gameKind]);
	if(IDOK == dlg.DoModal())
	{
		dlg.GetBonus(m_fBonus,  m_fPercent);

		SendToServer(5);
	}
}
void CGuangDong11X5::OnBnClickedBtnQuShi()
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
	_sntprintf(szUrl,CountArray(szUrl),TEXT("%s/TrendCharts/syx5/basic.aspx?type=%d"),szTodayURL,m_TypeID);
	ShellExecute(NULL, _T("open"), szUrl, NULL, NULL, SW_SHOWNORMAL);
	return;
}

void CGuangDong11X5::OnBnClickedBtnMoreRecord()
{
	SendToServer(8);
	return;
}
//��Ͷ��
void CGuangDong11X5::OnBnClickedBeiTouJia()
{
	CString strBeishu;

	m_beishu += 1;

	strBeishu.Format(L"%d",m_beishu);

	m_editBeiTou.SetWindowText(strBeishu);
	return;

}
//��Ͷ��
void CGuangDong11X5::OnBnClickedBeiTouJian()
{
	CString strBeishu;

	m_beishu -= 1;

	if(m_beishu<1)
	{
		m_beishu = 1;
	}
	strBeishu.Format(L"%d",m_beishu);

	m_editBeiTou.SetWindowText(strBeishu);
	return;
}


BOOL CGuangDong11X5::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	if((nCommandID >= IDC_BTN_WAN_0 && nCommandID <= IDC_BTN_GE_9)||(nCommandID >= IDC_BTN_WAN_10 && nCommandID <= IDC_BTN_GE_10)||(nCommandID >= IDC_BTN_DA_SHI && nCommandID <= IDC_BTN_WAN_2)||(nCommandID >= IDC_BTN_HEZHI_0 && nCommandID <= IDC_BTN_HEZHI_27)||(nCommandID >= IDC_BTN_DA_WAN && nCommandID <= IDC_BTN_SHUANG_BAI)||(nCommandID >= IDC_BTN_NIU_1 && nCommandID <= IDC_BTN_NIU_10))
	{
 
 		CString strWan = GetWanString();
 		CString strQian = GetQianString();
 		CString strBai = GetBaiString();
 		CString strShi = GetShiString();
 		CString strGe = GetGeString();
 		CString strHaoma;
 		strHaoma = strWan + TEXT(",") + strQian + TEXT(",") + strBai + TEXT(",") + strShi + TEXT(",") + strGe;
 
 		int nZhushu = Get11x5Zhushu(m_TypeID,m_gameKind);
 
	}

	return  __super::OnCommand(wParam,lParam);;
}
int CGuangDong11X5::Get11x5Zhushu(int nTypeID,int nKindID)
{
	CString strWan = GetWanString();
	CString strQian = GetQianString();
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();

	m_singlezhushu = 0;
	if(nKindID == IIRenXuan2)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu = (nCount-1)*nCount/2;
	}
	else if(nKindID == IIRenXuan1)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu = nCount;
	}
	else if(nKindID == IIRenXuan3)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu = (nCount-1)*(nCount-2)*nCount/6;
	}
	else if(nKindID == IIRenXuan4)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu = (nCount-1)*(nCount-2)*(nCount-3)*nCount/24;
	}
	else if(nKindID == IIRenXuan5)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu =(nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*nCount/120;
	}
	else if(nKindID == IIRenXuan6)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*nCount/720;
	}
	else if(nKindID == IIRenXuan7)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*nCount/5040;
	}
	else if(nKindID == IIRenXuan8)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu = (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*(nCount-7)*nCount/40320;
	}
	else if(nKindID == QianYi)
	{
		m_singlezhushu = strWan.GetLength()/2 ;
	}
	else if(nKindID == QianEr_ZhiXuan)
	{
		CString strTempWan,strTempQian;
		for(int i = 0;i < strWan.GetLength();i+=2)
		{
			strTempWan = strWan.GetAt(i);
			strTempWan += strWan.GetAt(i+1);
			for(int j = 0;j < strQian.GetLength();j+=2)
			{
				strTempQian = strQian.GetAt(j);
				strTempQian += strQian.GetAt(j+1);
				if (strTempQian != strTempWan)
				{
					m_singlezhushu++;
				}
			}
		}

		//m_singlezhushu = (strWan.GetLength()/2) * (strQian.GetLength()/2);
	}
	else if(nKindID == QianEr_ZuXuan)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu = (nCount-1)*nCount/2;
	}
	else if(nKindID == QianSan_ZhiXuan)
	{
		CString strTempWan,strTempQian,strTempBai;
		for(int i = 0;i < strWan.GetLength();i+=2)
		{
			strTempWan = strWan.GetAt(i);
			strTempWan += strWan.GetAt(i+1);
			for(int j = 0;j < strQian.GetLength();j+=2)
			{
				strTempQian = strQian.GetAt(j);
				strTempQian += strQian.GetAt(j+1);
				for(int k = 0;k < strBai.GetLength();k+=2)
				{
					strTempBai = strBai.GetAt(k);
					strTempBai += strBai.GetAt(k+1);
					if (strTempQian != strTempWan && strTempBai != strTempQian && strTempBai != strTempWan)
					{

						m_singlezhushu++;
					}
				}
			}
		}

		//m_singlezhushu = (strWan.GetLength()/2) * (strQian.GetLength()/2)*(strBai.GetLength()/2);
	}
	else if(nKindID == QianSan_ZuXuan)
	{
		int nCount = strBai.GetLength()/2;
		m_singlezhushu = (nCount-2)*(nCount-1)*nCount/6;
	}
	else if(nKindID == Ren2_Dantuo||nKindID == Ren3_Dantuo||nKindID == Ren4_Dantuo||nKindID == Ren5_Dantuo||nKindID == Ren6_Dantuo||nKindID == Ren7_Dantuo||nKindID == Ren8_Dantuo)
	{
		int nMax = 0;
		if(nKindID == Ren2_Dantuo)
			nMax = 2;
		else if(nKindID == Ren3_Dantuo)
			nMax = 3;
		else if(nKindID == Ren4_Dantuo)
			nMax = 4;
		else if(nKindID == Ren5_Dantuo)
			nMax = 5;
		else if(nKindID == Ren6_Dantuo)
			nMax = 6;
		else if(nKindID == Ren7_Dantuo)
			nMax = 7;
		else if(nKindID == Ren8_Dantuo)
			nMax = 8;

		int nQianCount = strQian.GetLength()/2;
		int nBaiCount = strBai.GetLength()/2;
		if(nQianCount>(nMax-1))
		{
			CString strMessage;
			strMessage.Format(L"��%s���������ѡ%d��,�������ѡ%d����",theGameKind[(m_TypeID << 16) | (unsigned short)m_gameKind],nMax-1,11-nMax+1);
			MyMessageBox(strMessage);
			ResetAllNums();

		}
		if(nQianCount>(nMax-1)||nBaiCount<1||nQianCount<1)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=200;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);
			return m_singlezhushu;

		}
		CString strQianDan,strBaiDan;
		for (int i = 0;i < nQianCount*2;i+=2)
		{
			strQianDan = strQian.Mid(i,2);
			for (int j = 0;j < nBaiCount*2;j+=2)
			{
				strBaiDan = strBai.Mid(j,2);

				if(strQianDan == strBaiDan)
				{
					m_singlezhushu = 0;
					CRect rcInvali;
					rcInvali.CopyRect(rcZongZhuShu);
					rcInvali.right+=200;
					InvalidateRect(&rcInvali);
					InvalidateRect(&rcZongJinE);
					return m_singlezhushu;
				}
			}
		}
		m_singlezhushu = combo(nBaiCount,nMax-nQianCount);

	}

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=200;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	return m_singlezhushu;
}
LRESULT CGuangDong11X5::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	//OnBnClickedBtnDelSel();
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

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);

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

CString CGuangDong11X5::GetScoreMoshi()
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
int CGuangDong11X5::GetMoshiRet(CString moshi)
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
void CGuangDong11X5::OnEnChangeRichEditDanshiInput()
{
	m_singlezhushu = GetDanShiZhusu(m_gameKind);


	return ;
}

int CGuangDong11X5::GetDanShiZhusu(int nKind)
{

	m_singlezhushu = 0;

 	if(nKind == IIRenXuan1||nKind == IIRenXuan2||nKind == QianEr_ZhiXuan||nKind == IIRenXuan3||nKind == QianSan_ZhiXuan||nKind == IIRenXuan4||nKind == IIRenXuan5||nKind == IIRenXuan6||nKind == IIRenXuan7||nKind == IIRenXuan8)
 	{
 		int nKindSize = 0;
 		if(nKind == IIRenXuan1)
 			nKindSize = 1;
		else if(nKind == IIRenXuan2||nKind == QianEr_ZhiXuan)
			nKindSize = 2;
		else if(nKind == IIRenXuan3||nKind == QianSan_ZhiXuan)
 			nKindSize = 3;
 		else if(nKind == IIRenXuan4)
 			nKindSize = 4;
 		else if(nKind == IIRenXuan5)
 			nKindSize = 5;
 		else if(nKind == IIRenXuan6)
 			nKindSize = 6;
 		else if(nKind == IIRenXuan7)
 			nKindSize = 7;
 		else if(nKind == IIRenXuan8)
 			nKindSize = 8;
		CString strHaoMa;
		if (!GetDanShiHaoma(strHaoMa,10240,nKindSize))
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=200;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		string sHaoma;
		ChangeStringToA(strHaoMa,sHaoma);
		vector<string> vectHaoma;
		SplitString2(sHaoma,vectHaoma);
		m_singlezhushu = vectHaoma.size()-1;

	}
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=200;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
	return m_singlezhushu;

}
