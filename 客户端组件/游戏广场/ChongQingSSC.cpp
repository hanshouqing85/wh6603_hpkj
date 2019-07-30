#include "stdafx.h"
#include "GamePlaza.h"
#include "ChongQingSSC.h"
#include "BmpUtil.h"
/*#include "LuckMeDlg.h"*/
#include "MessageDlg.h"
#include "DlgAddtional.h"
#include "PlatformFrame.h"
#include "DlgFandian.h"
#include "GDIpng/MemDC.h"
#include <map>
map <int,CString> mapFandian;
map <int,float> mapPercent;
map <int,float> mapDanweiFandian;
map <int,float> mapBonus;
vector<wstring>	vecData1, vecData2, vecData3;
using		namespace		std;

#define      MAXN			100
vector<wstring> vecAllNums;
CString   CChongQingSSC::m_strHaoma = _T("");

int g_arr[MAXN];
int g_nPos[512000][5];
int g_Counts=0;


// void GetCombination(ref List<string[]> list, string[] t, int n, int m, int[] b, int M)
// {
// 	for (int i = n; i >= m; i--)
// 	{
// 		b[m - 1] = i - 1;
// 		if (m > 1)
// 		{
// 			GetCombination(ref list, t, i - 1, m - 1, b, M);
// 		}
// 		else
// 		{
// 			if (list == null)
// 			{
// 				list = new List<string[]>();
// 			}
// 			string[] temp = new string[M];
// 			for (int j = 0; j < b.Length; j++)
// 			{
// 				temp[j] = t[b[j]];
// 			}
// 			list.Add(temp);
// 		}
// 	}
// }

void      Combo(int m,int k)
{   
	CString strLog;

	int i,j;
	for (i=m;i>=k;i--)
	{
		g_arr[k]=i;
		if (k>1)
			Combo(i-1,k-1);
		else
		{

			for (j=g_arr[0];j>0;j--)
			{
				
				g_nPos[g_Counts][j-1] = g_arr[j];
			}
			g_Counts++;
		}
	}
}

//�õ���Ϻ��
int GetComboArray(int DigitnCount, int nPosCount, int* szInput, int szOutput[10][5])
{
	g_arr[0]=nPosCount;
	Combo(DigitnCount,nPosCount);
	for(int n=0; n<10; n++)
	{
		for(int k=0; k<nPosCount; k++)
		{

			if(g_nPos[n][k] == 0)
				continue;
			szOutput[ n ][g_nPos[n][k] -1] = szInput[k]; 
		}
	}
	return 0;
}

int InsertDouhaoAfter(int nDouhaoIndexAfter, wstring&  strLine)
{
	bool bFound=false;
	int nIndex=1;
	for(int n=0; n<strLine.size(); n++)
	{
		if(strLine[n] == TCHAR(','))
		{
			if(nIndex == nDouhaoIndexAfter)
			{
				bFound=true;
				if((n+1) >= strLine.size())
					strLine.append(L",");
				else if(strLine[n+1] == TCHAR(','))
					strLine.insert(n+1, L",");
				else
					strLine.insert(n+1, L",");
				break;
			}
			nIndex++;
		}
	}
	if(!bFound)
		strLine.append(L",");
	return nIndex;
}

//�õ�ĳһ�д����ŵ��ַ���
wstring	GetXiaZhuString(int nLines, int nCols, int nIndex, int nValue [10][5], int nInsertCol=100)
{
	wstring strLines;
	for(int k=0; k<nCols; k++)
	{
		char sztemp;
		if(nValue[nIndex][k] == -1 )
		{
			strLines.push_back(TCHAR(','));
		}
		else
		{
			sztemp= 0x30+ nValue[ nIndex ][k];
			strLines.push_back(sztemp);
			strLines.push_back(TCHAR(','));
		}
	}
	strLines.erase(strLines.find_last_of(L","));
	return strLines;
}






//��ѡ������
int nRenXuan2[6]={0,0, 1, 3, 6, 10};
int nRenXuan3[6]={0,0, 0, 1, 4, 10};
int nRenXuan4[6]={0,0, 0, 0, 1, 5};


//�淨��������
static const int wanfa_x = 9;
static const int wanfa_y = 109;
static const int wanfa_col_width = 48;
static const int wanfa_row_height = 32;

//�淨�ӷ�������
static const int wanfa_sel_x = 60;
static const int wanfa_sel_y = 145;
static const int wanfa_sel_width = 71;
static const int wanfa_sel_height = 23;

//��������
static const int haoma_btn_x = 65-30;
static const int haoma_btn_y = 226-18;
static const int haoma_btn_col_wdith = 41;				//41->40, 2018.10.10
static const int haoma_btn_row_height = 35;

//������ʾ����
static const int haoma_tip_x = 20-15;
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
static const int list_haomao_y = 469+25;
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
static const int btn_more_x = 712+50;
static const int btn_more_y = 92-90;

//�󿪽���
static const int kj_big_haoma_x = 221;
static const int kj_big_haoma_y = 41;

static const int czh_rc_left = 243-35;
static const int czh_rc_top = 4-5;
static const int czh_rc_width = 65;
static const int czh_rc_height = 30;
static CRect rcCzh(czh_rc_left, czh_rc_top, czh_rc_left + czh_rc_width, czh_rc_top + czh_rc_height);

static const int qihao_rc_left = 325-35;
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
static const int kj_small_haoma_x = 630+5;
static const int kj_small_haoma_y = 28;
static const int kj_small_haoma_col_span = 3;
static const int kj_small_haoma_row_span = 6;

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
static const int TimerStatus = 6;

//�������������ˢ�¿������ݵ�
static const int TimerZhunBeiData = 5;

static const int TimerLotteryStatus = 6;
static const int timer_xgqh	=	7;
static const int timer_getcanadaqihao = 8;
static const int redraw_rc_left = 0;
static const int redraw_rc_top = 0;
static const int redraw_rc_width = 791;
static const int redraw_rc_height = 103;
static CRect rcRedraw(redraw_rc_left, redraw_rc_top, redraw_rc_left + redraw_rc_width, redraw_rc_top + redraw_rc_height);
//static const int timespan_kj_shj = 600;
//static const int timespan_ye_kj_shj = 300;

IMPLEMENT_DYNAMIC(CChongQingSSC, CDialog)

CChongQingSSC::CChongQingSSC(CWnd* pParent /*=NULL*/)
	: CDialog(CChongQingSSC::IDD, pParent)
	, m_bmpBk(NULL)
	, m_numImageList(NULL)
	, m_numImageList1(NULL)
	, m_numImageList2(NULL)
	, m_numImageList3(NULL)
	, m_kjNumBig(NULL)
	, m_kjNumSmall(NULL)
	, m_bmpDxdsh(NULL)
	, m_bmpNumTip(NULL)
	, m_bmpNumTip1(NULL)
	, m_bigNumWidth(0)
	, m_bigNumHeight(0)
	, m_smallNumWidth(0)
	, m_smallNumHeight(0)
	, m_gameKind(HouSanZhiXuan_FuShi)
	, m_zongZhuShu(0)
	//, m_zongJinE(0.0)
	, m_beishu(1)
	, m_kjXinxiCont(0)
{
	m_SocketID=0;
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

	m_nTestCount = 1000;
	m_RenxuanKeyboardInput=false;
	_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, _T(""));

	//�������
	m_bGetUserFandian=false;
	m_Font.CreateFont(14,0,0,0,700,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("΢���ź�"));
	m_TimeFont.CreateFont(18,0,0,0,700,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("΢���ź�"));
	m_wGameHoverItem=INVALID_WORD;
	m_cbMenuType=0xff;
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
	nBonus = 0.0f;//theDBCmd.GetUserFandian(theAccount.user_id,  m_TypeID, m_gameKind, nPercent, fDanweiFandian);  //��ʱ����  a'long
	m_nTouzhuCount=0;
	m_nZhuihaoCount = 0;
	m_bZhuihao=false;
	//�������
	m_DlgStatus.SetStatusViewSink(this);

}
//ȡ������
VOID CChongQingSSC::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}


CChongQingSSC::~CChongQingSSC()
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
		SafeDelete(m_numImageList3);
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
//		SafeDelete(m_imgKaijiang);
	}

	if (m_bmpDxdsh != NULL) 
	{
		SafeDelete(m_bmpDxdsh);
	}

	if (m_bmpNumTip != NULL) 
	{
		SafeDelete(m_bmpNumTip);
	}
	if (m_bmpNumTip1 != NULL) 
	{
		SafeDelete(m_bmpNumTip1);
	}
}

void CChongQingSSC::FlushZongjine()
{
	InvalidateRect(&rcZongJinE);
}

void CChongQingSSC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_HSZHX, m_btnHouSanZhiXuan);
	DDX_Control(pDX, IDC_BTN_NIUNIU, m_btnNiuNiu);
	DDX_Control(pDX, IDC_BTN_LHD, m_btnLhd);
	DDX_Control(pDX, IDC_BTN_QSZHX, m_btnQianSanZhiXuan);
	DDX_Control(pDX, IDC_BTN_HSZX, m_btnHouSanZuXuan);
	DDX_Control(pDX, IDC_BTN_QSZX, m_btnQianSanZuXuan);
	DDX_Control(pDX, IDC_BTN_WXHE, m_btnWuXingHouEr);
	DDX_Control(pDX, IDC_BTN_WXQE, m_btnWuXingQianEr);
	DDX_Control(pDX, IDC_BTN_HEZX, m_btnHouErZuXuan);
	DDX_Control(pDX, IDC_BTN_QEZX, m_btnQianErZuXuan);
	DDX_Control(pDX, IDC_BTN_ERXING, m_btnQianErXing);
	DDX_Control(pDX, IDC_BTN_DXDSH, m_btnDaXiaoDanShuang);
	DDX_Control(pDX, IDC_RIO_DXDSH_DW, m_rioDwDxdsh);
	DDX_Control(pDX, IDC_RIO_DXDSH_HZH, m_rioHzhDxdsh);
	DDX_Control(pDX, IDC_BTN_HSZHXHZH, m_btnHouSanHeZhi);
	DDX_Control(pDX, IDC_BTN_BDW, m_btnBuDingWei);
	DDX_Control(pDX, IDC_BTN_DWD, m_btnDingWeiDan);
	DDX_Control(pDX, IDC_BTN_QW, m_btnQuwei);

	DDX_Control(pDX, IDC_BTN_LOCK, m_btnLock);
	//����ֱѡ
 	DDX_Control(pDX, IDC_RIO_WXZX,m_rioWuXingZX);
	//����ֱѡ
 	DDX_Control(pDX, IDC_RIO_WXZX_DAN,m_rioWuXingZXDan);
	//����ǰ��
 	DDX_Control(pDX, IDC_RIO_WXQS,m_rioWuXingQS);
	//����ǰ��
 	DDX_Control(pDX, IDC_RIO_WXQS_DAN,m_rioWuXingQSDan);
	//���Ǻ���
 	DDX_Control(pDX, IDC_RIO_WXHS,m_rioWuXingHS);
	//���Ǻ��ĵ�ʽ
 	DDX_Control(pDX, IDC_RIO_WXHS_DAN,m_rioWuXingHSDan);
	//��������

	DDX_Control(pDX, IDC_RIO_WN,m_rioWuNiu);
	DDX_Control(pDX, IDC_RIO_NN,m_rioNiuNiu);
	DDX_Control(pDX, IDC_RIO_NS,m_rioNiuShu);
 	DDX_Control(pDX, IDC_RIO_WXZS,m_rioWuXingZS);
	DDX_Control(pDX, IDC_RIO_WXZS_DAN,m_rioWuXingZSDan);
	DDX_Control(pDX, IDC_RIO_HSZHX_FUSHI, m_rioHsZhxFuShi);
	DDX_Control(pDX, IDC_RIO_RENXUAN2,m_rioRenXuan2);
	DDX_Control(pDX, IDC_RIO_RENXUAN2_DAN,m_rioRenXuan2Dan);
	DDX_Control(pDX, IDC_RIO_RENXUAN2_ZUXUAN,m_rioRenXuan2Zx);
	DDX_Control(pDX, IDC_RIO_RENXUAN2_ZUXUAN_DAN,m_rioRenXuan2ZxDan);
	DDX_Control(pDX, IDC_RIO_RENXUAN3,m_rioRenXuan3);
	DDX_Control(pDX, IDC_RIO_RENXUAN3_DAN,m_rioRenXuan3Dan);
	DDX_Control(pDX, IDC_RIO_RENXUAN3_ZUXUAN3,m_rioRenXuan3Zx3);
	DDX_Control(pDX, IDC_RIO_RENXUAN3_ZUXUAN3_DAN,m_rioRenXuan3Zx3Dan);
	DDX_Control(pDX, IDC_RIO_RENXUAN3_ZUXUAN6,m_rioRenXuan3Zx6);
	DDX_Control(pDX, IDC_RIO_RENXUAN3_ZUXUAN6_DAN,m_rioRenXuan3Zx6Dan);
	DDX_Control(pDX, IDC_RIO_RENXUAN3_HUNHE,m_rioRenXuan3Hunhe);
	DDX_Control(pDX, IDC_RIO_RENXUAN4,m_rioRenXuan4);
	DDX_Control(pDX, IDC_RIO_RENXUAN4_DAN,m_rioRenXuan4Dan);
	DDX_Control(pDX, IDC_RIO_MOUSE_INPUT,m_rioMouseInput);
	DDX_Control(pDX, IDC_RIO_KEYBOARD_INPUT,m_rioKeyboardInput);
	DDX_Control(pDX, IDC_CHK_WAN,	m_rioWan);
	DDX_Control(pDX, IDC_CHK_QIAN,	m_rioQian);
	DDX_Control(pDX, IDC_CHK_BAI,	m_rioBai);
	DDX_Control(pDX, IDC_CHK_SHI,	m_rioShi);
	DDX_Control(pDX, IDC_CHKGE,		m_rioGe);
	DDX_Control(pDX, IDC_STATIC_RENXUAN_TIP, m_static_RenXuan_Tip);

	DDX_Control(pDX, IDC_RIO_LHD_WQ,	m_rioLhdWq);
	DDX_Control(pDX, IDC_RIO_LHD_WB,	m_rioLhdWb);
	DDX_Control(pDX, IDC_RIO_LHD_WS,	m_rioLhdWs);
	DDX_Control(pDX, IDC_RIO_LHD_WG,	m_rioLhdWg);
	DDX_Control(pDX, IDC_RIO_LHD_QB,	m_rioLhdQb);
	DDX_Control(pDX, IDC_RIO_LHD_QS,	m_rioLhdQs);
	DDX_Control(pDX, IDC_RIO_LHD_QG,	m_rioLhdQg);
	DDX_Control(pDX, IDC_RIO_LHD_BS,	m_rioLhdBs);
	DDX_Control(pDX, IDC_RIO_LHD_BG,	m_rioLhdBg);
	DDX_Control(pDX, IDC_RIO_LHD_SG,	m_rioLhdSg);

	DDX_Control(pDX, IDC_RIO_ZX_120,m_rioZuXuan120);
	DDX_Control(pDX, IDC_RIO_ZX_60,	m_rioZuXuan60);
	DDX_Control(pDX, IDC_RIO_ZX_30,	m_rioZuXuan30);
	DDX_Control(pDX, IDC_RIO_ZX_20,	m_rioZuXuan20);
	DDX_Control(pDX, IDC_RIO_ZX_10,	m_rioZuXuan10);
	DDX_Control(pDX, IDC_RIO_ZX_5,	m_rioZuXuan5);
	//ǰ����ѡ
	DDX_Control(pDX, IDC_RIO_QS_ZX24,m_rioQsZuXuan24);
	DDX_Control(pDX, IDC_RIO_QS_ZX12,	m_rioQsZuXuan12);
	DDX_Control(pDX, IDC_RIO_QS_ZX6,	m_rioQsZuXuan6);
	DDX_Control(pDX, IDC_RIO_QS_ZX4,	m_rioQsZuXuan4);
	//������ѡ
	DDX_Control(pDX, IDC_RIO_HS_ZX24,m_rioHsZuXuan24);
	DDX_Control(pDX, IDC_RIO_HS_ZX12,	m_rioHsZuXuan12);
	DDX_Control(pDX, IDC_RIO_HS_ZX6,	m_rioHsZuXuan6);
	DDX_Control(pDX, IDC_RIO_HS_ZX4,	m_rioHsZuXuan4);

	DDX_Control(pDX, IDC_RIO_QS_HEZHI,	m_rioQsHezhi);
	DDX_Control(pDX, IDC_RIO_ZS_HEZHI,	m_rioZsHezhi);
	DDX_Control(pDX, IDC_RIO_HS_HEZHI,	m_rioHsHezhi);
	DDX_Control(pDX, IDC_RIO_QE_HEZHI,	m_rioQeHezhi);
	DDX_Control(pDX, IDC_RIO_HE_HEZHI,	m_rioHeHezhi);

	DDX_Control(pDX, IDC_RIO_YFFS,	m_rioYffs);
	DDX_Control(pDX, IDC_RIO_HSCS,	m_rioHscs);
	DDX_Control(pDX, IDC_RIO_SXBX,	m_rioSxbx);
	DDX_Control(pDX, IDC_RIO_SJFC,	m_rioSjfc);

	DDX_Control(pDX, IDC_RIO_HSZHX_DANSHI, m_rioHsZhxDanShi);
	DDX_Control(pDX, IDC_RIO_QSZHX_FUSHI, m_rioQsZhxFuShi);
	DDX_Control(pDX, IDC_RIO_QSZHX_DANSHI, m_rioQsZhxDanShi);
	DDX_Control(pDX, IDC_RIO_ZSZS_FUSHI, m_rioZsZsFuShi);
	DDX_Control(pDX, IDC_RIO_ZSZS_DANSHI, m_rioZsZsDanShi);
	DDX_Control(pDX, IDC_RIO_ZSZL_FUSHI, m_rioZsZlFuShi);
	DDX_Control(pDX, IDC_RIO_ZSZL_DANSHI, m_rioZsZlDanShi);
	DDX_Control(pDX, IDC_RIO_HSZS_FUSHI, m_rioHsZsFuShi);
	DDX_Control(pDX, IDC_RIO_HSZS_DANSHI, m_rioHsZsDanShi);
	DDX_Control(pDX, IDC_RIO_HSZL_FUSHI, m_rioHsZlFuShi);
	DDX_Control(pDX, IDC_RIO_HSZL_DANSHI, m_rioHsZlDanShi);
	DDX_Control(pDX, IDC_RIO_QSZS_FUSHI, m_rioQsZsFuShi);
	DDX_Control(pDX, IDC_RIO_QSZS_DANSHI, m_rioQsZsDanShi);
	DDX_Control(pDX, IDC_RIO_QSZL_FUSHI, m_rioQsZlFuShi);
	DDX_Control(pDX, IDC_RIO_QSZL_DANSHI, m_rioQsZlDanShi);
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
	DDX_Control(pDX, IDC_RIO_ZSBDW, m_rioZsbdw);
	DDX_Control(pDX, IDC_RIO_HSBDW, m_rioHsbdw);
	DDX_Control(pDX, IDC_RIO_DWD,	m_rioDwd);
	DDX_Control(pDX, IDC_RIO_HEBDW, m_rioHebdw);
	DDX_Control(pDX, IDC_RIO_QEBDW,	m_rioQebdw);

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

	DDX_Control(pDX, IDC_BTN_NIU_0, m_btnNiu_0);
	DDX_Control(pDX, IDC_BTN_NIU_1, m_btnNiu_1);
	DDX_Control(pDX, IDC_BTN_NIU_2, m_btnNiu_2);
	DDX_Control(pDX, IDC_BTN_NIU_3, m_btnNiu_3);
	DDX_Control(pDX, IDC_BTN_NIU_4, m_btnNiu_4);
	DDX_Control(pDX, IDC_BTN_NIU_5, m_btnNiu_5);
	DDX_Control(pDX, IDC_BTN_NIU_6, m_btnNiu_6);
	DDX_Control(pDX, IDC_BTN_NIU_7, m_btnNiu_7);
	DDX_Control(pDX, IDC_BTN_NIU_8, m_btnNiu_8);
	DDX_Control(pDX, IDC_BTN_NIU_9, m_btnNiu_9);
	DDX_Control(pDX, IDC_BTN_NIU_10, m_btnNiu_10);

	DDX_Control(pDX, IDC_BTN_LONG, m_btnLong);
	DDX_Control(pDX, IDC_BTN_HU, m_btnHu);
	DDX_Control(pDX, IDC_BTN_HE, m_btnHe);

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
	DDX_Text(pDX, IDC_EDIT_BEISHU, m_beishu);
	

	DDX_Control(pDX, IDC_EDIT_DANSHI_HAOMA, m_editDanShiHaoMa);
	DDX_Control(pDX, IDC_RICHEDIT_DANSHI, m_richDanshiHaoma);

	DDX_Control(pDX, IDC_STATIC_WAN, m_staticWan);
	DDX_Control(pDX, IDC_STATIC_QIAN, m_staticQian);
	DDX_Control(pDX, IDC_STATIC_BAI, m_staticBai);
	DDX_Control(pDX, IDC_STATIC_SHI, m_staticShi);
	DDX_Control(pDX, IDC_STATIC_GE, m_staticGe);
	DDX_Control(pDX, IDC_STATIC_NIU, m_staticNiu);
	DDX_Control(pDX, IDC_STATIC_LHD, m_staticLhd);
	DDX_Control(pDX, IDC_STATIC_DXDS_HZH, m_statichzh);

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

	DDX_Control(pDX, IDC_BTN_NIU_QUAN, m_btnNiuQuan);
	DDX_Control(pDX, IDC_BTN_NIU_DA, m_btnNiuDa);
	DDX_Control(pDX, IDC_BTN_NIU_XIAO, m_btnNiuXiao);
	DDX_Control(pDX, IDC_BTN_NIU_DAN, m_btnNiuDan);
	DDX_Control(pDX, IDC_BTN_NIU_SHUANG, m_btnNiuShuang);
	DDX_Control(pDX, IDC_BTN_NIU_QING, m_btnNiuQing);

	DDX_Control(pDX, IDC_LIST_CHQSSC, m_listChqSSC);
	DDX_Control(pDX, IDC_GRID_LIST, m_GridList);
	DDX_Control(pDX, IDC_BTN_CHQ_ADD, m_btnChqAdd);
	DDX_Control(pDX, IDC_BTN_DEL_SEL, m_btnDelSel);
	DDX_Control(pDX, IDC_BTN_CLS_LIST, m_btnClsList);
	DDX_Control(pDX, IDC_BTN_TOUZHU, m_btnTouZhu);
	DDX_Control(pDX, IDC_BTN_TOUZHU_ONEKEY, m_btnTouZhuOnekey);
	DDX_Control(pDX, IDC_BTN_ZHUIHAO, m_btnZhuiHao);
	DDX_Control(pDX, IDC_EDIT_BEISHU, m_editBeiTou);
	DDX_Control(pDX, IDC_BTN_MORE_RECORD, m_btnMoreRecord);
	DDX_Control(pDX, IDC_BTN_QUSHI, m_btnQushi);

	DDX_Control(pDX,IDC_BTN_BEITOU_JIA,m_btnBeiTouJia);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIAN,m_btnBeiTouJian);

	DDX_Control(pDX, IDC_RIO_QSHUNHE, m_rioQsHunhe);
	DDX_Control(pDX, IDC_RIO_ZSHUNHE, m_rioZsHunhe);
	DDX_Control(pDX, IDC_RIO_HSHUNHE, m_rioHsHunhe);

	DDX_Control(pDX, IDC_STATIC_DANHAO, m_staticDanHao);
	DDX_Control(pDX, IDC_STATIC_XUANHAO, m_staticXuanHao);
	DDX_Control(pDX, IDC_STATIC_ZU120, m_staticZu120);

	DDX_Control(pDX, IDC_STATIC_ERCHONG, m_staticErChong);
	DDX_Control(pDX, IDC_STATIC_ERCHONG1, m_staticErChong1);
	DDX_Control(pDX, IDC_STATIC_SANCHONG, m_staticSanChong);
	DDX_Control(pDX, IDC_STATIC_SICHONG, m_staticSiChong);


	DDX_Control(pDX, IDC_STATIC_ZUXUAN, m_staticZuXuan);
	DDX_Control(pDX, IDC_STATIC_BUDINGWEI, m_staticBuDingWei);
	DDX_Control(pDX, IDC_STATIC_HUNHE_TIP, m_staticHunheTip);
	DDX_Control(pDX, IDC_STATIC_NIUNIU_TIP, m_staticNiuNiuTip);

	DDX_Control(pDX, IDC_BTN_DA_WAN, m_btnDaWan);
	DDX_Control(pDX, IDC_BTN_XIAO_WAN, m_btnXiaoWan);
	DDX_Control(pDX, IDC_BTN_DAN_WAN, m_btnDanWan);
	DDX_Control(pDX, IDC_BTN_SHUANG_WAN, m_btnShuangWan);
	DDX_Control(pDX, IDC_BTN_DA_QIAN, m_btnDaQian);
	DDX_Control(pDX, IDC_BTN_XIAO_QIAN, m_btnXiaoQian);
	DDX_Control(pDX, IDC_BTN_DAN_QIAN, m_btnDanQian);
	DDX_Control(pDX, IDC_BTN_SHUANG_QIAN, m_btnShuangQian);
	DDX_Control(pDX, IDC_BTN_DA_BAI, m_btnDaBai);
	DDX_Control(pDX, IDC_BTN_XIAO_BAI, m_btnXiaoBai);
	DDX_Control(pDX, IDC_BTN_DAN_BAI, m_btnDanBai);
	DDX_Control(pDX, IDC_BTN_SHUANG_BAI, m_btnShuangBai);

	DDX_Control(pDX, IDC_BTN_DA_SHI, m_btnDaShi);
	DDX_Control(pDX, IDC_BTN_XIAO_SHI, m_btnXiaoShi);
	DDX_Control(pDX, IDC_BTN_DAN_SHI, m_btnDanShi);
	DDX_Control(pDX, IDC_BTN_SHUANG_SHI, m_btnShuangShi);
	DDX_Control(pDX, IDC_BTN_DA_GE, m_btnDaGe);
	DDX_Control(pDX, IDC_BTN_XIAO_GE, m_btnXiaoGe);
	DDX_Control(pDX, IDC_BTN_DAN_GE, m_btnDanGe);
	DDX_Control(pDX, IDC_BTN_SHUANG_GE, m_btnShuangGe);

	DDX_Control(pDX, IDC_BTN_YUAN,m_btnYuan);
	DDX_Control(pDX, IDC_BTN_JIAO,m_btnJiao);
	DDX_Control(pDX, IDC_BTN_FEN,m_btnFen);
	DDX_Control(pDX, IDC_BTN_LI,m_btnLi);
	DDX_Control(pDX,IDC_TIP_WXZX,m_btWxzxTip);
	DDX_Control(pDX,IDC_TIP_WXZX_HELP,m_btWxzxHelp);
	DDX_Control(pDX,IDC_STATIC_RIO1,m_staticRio1);
	DDX_Control(pDX,IDC_STATIC_RIO2,m_staticRio2);
	DDX_Control(pDX,IDC_STATIC_RIO3,m_staticRio3);
}

BEGIN_MESSAGE_MAP(CChongQingSSC, CDialog)
	ON_BN_CLICKED(IDC_BTN_YUAN, &CChongQingSSC::OnBnClickedBtnYuan)
	ON_BN_CLICKED(IDC_BTN_JIAO, &CChongQingSSC::OnBnClickedBtnJiao)
	ON_BN_CLICKED( IDC_BTN_FEN, &CChongQingSSC::OnBnClickedBtnFen)
	ON_BN_CLICKED(IDC_BTN_LI, &CChongQingSSC::OnBnClickedBtnLi)

	ON_BN_CLICKED(IDC_BTN_HSZHX, &CChongQingSSC::OnBnClickedBtnHszhx)
	ON_BN_CLICKED(IDC_BTN_NIUNIU, &CChongQingSSC::OnBnClickedBtnNiuNiu)
	ON_BN_CLICKED(IDC_BTN_LHD, &CChongQingSSC::OnBnClickedBtnLhd)
	ON_BN_CLICKED(IDC_BTN_HSZX, &CChongQingSSC::OnBnClickedBtnHszx)
	ON_BN_CLICKED(IDC_BTN_WXHE, &CChongQingSSC::OnBnClickedBtnWxhe)
	ON_BN_CLICKED(IDC_BTN_HEZX, &CChongQingSSC::OnBnClickedBtnHezx)
	ON_BN_CLICKED(IDC_BTN_DXDSH, &CChongQingSSC::OnBnClickedBtnDxdsh)
	ON_BN_CLICKED(IDC_BTN_BDW, &CChongQingSSC::OnBnClickedBtnBdw)
	ON_BN_CLICKED(IDC_BTN_QSZHX, &CChongQingSSC::OnBnClickedBtnQszhx)
	ON_BN_CLICKED(IDC_BTN_QSZX, &CChongQingSSC::OnBnClickedBtnQszx)
	ON_BN_CLICKED(IDC_BTN_WXQE, &CChongQingSSC::OnBnClickedBtnWxqe)
	ON_BN_CLICKED(IDC_BTN_QEZX, &CChongQingSSC::OnBnClickedBtnQezx)
	ON_BN_CLICKED(IDC_BTN_ERXING, &CChongQingSSC::OnBnClickedBtnErXing)
	ON_BN_CLICKED(IDC_BTN_HSZHXHZH, &CChongQingSSC::OnBnClickedBtnHszhxhzh)
	ON_BN_CLICKED(IDC_BTN_DWD, &CChongQingSSC::OnBnClickedBtnDwd)
	ON_BN_CLICKED(IDC_BTN_QW, &CChongQingSSC::OnBnClickedBtnQw)
	ON_BN_CLICKED(IDC_BTN_WAN_QUAN, &CChongQingSSC::OnBnClickedBtnWanQuan)
	ON_BN_CLICKED(IDC_BTN_WAN_DA, &CChongQingSSC::OnBnClickedBtnWanDa)
	ON_BN_CLICKED(IDC_BTN_WAN_XIAO, &CChongQingSSC::OnBnClickedBtnWanXiao)
	ON_BN_CLICKED(IDC_BTN_WAN_DAN, &CChongQingSSC::OnBnClickedBtnWanDan)
	ON_BN_CLICKED(IDC_BTN_WAN_SHUANG, &CChongQingSSC::OnBnClickedBtnWanShuang)
	ON_BN_CLICKED(IDC_BTN_WAN_QING, &CChongQingSSC::OnBnClickedBtnWanQing)
	ON_BN_CLICKED(IDC_BTN_QIAN_QUAN, &CChongQingSSC::OnBnClickedBtnQianQuan)
	ON_BN_CLICKED(IDC_BTN_QIAN_DA, &CChongQingSSC::OnBnClickedBtnQianDa)
	ON_BN_CLICKED(IDC_BTN_QIAN_XIAO, &CChongQingSSC::OnBnClickedBtnQianXiao)
	ON_BN_CLICKED(IDC_BTN_QIAN_DAN, &CChongQingSSC::OnBnClickedBtnQianDan)
	ON_BN_CLICKED(IDC_BTN_QIAN_SHUANG, &CChongQingSSC::OnBnClickedBtnQianShuang)
	ON_BN_CLICKED(IDC_BTN_QIAN_QING, &CChongQingSSC::OnBnClickedBtnQianQing)
	ON_BN_CLICKED(IDC_BTN_BAI_QUAN, &CChongQingSSC::OnBnClickedBtnBaiQuan)
	ON_BN_CLICKED(IDC_BTN_BAI_DA, &CChongQingSSC::OnBnClickedBtnBaiDa)
	ON_BN_CLICKED(IDC_BTN_BAI_XIAO, &CChongQingSSC::OnBnClickedBtnBaiXiao)
	ON_BN_CLICKED(IDC_BTN_BAI_DAN, &CChongQingSSC::OnBnClickedBtnBaiDan)
	ON_BN_CLICKED(IDC_BTN_BAI_SHUANG, &CChongQingSSC::OnBnClickedBtnBaiShuang)
	ON_BN_CLICKED(IDC_BTN_BAI_QING, &CChongQingSSC::OnBnClickedBtnBaiQing)
	ON_BN_CLICKED(IDC_BTN_SHI_QUAN, &CChongQingSSC::OnBnClickedBtnShiQuan)
	ON_BN_CLICKED(IDC_BTN_SHI_DA, &CChongQingSSC::OnBnClickedBtnShiDa)
	ON_BN_CLICKED(IDC_BTN_SHI_XIAO, &CChongQingSSC::OnBnClickedBtnShiXiao)
	ON_BN_CLICKED(IDC_BTN_SHI_DAN, &CChongQingSSC::OnBnClickedBtnShiDan)
	ON_BN_CLICKED(IDC_BTN_SHI_SHUANG, &CChongQingSSC::OnBnClickedBtnShiShuang)
	ON_BN_CLICKED(IDC_BTN_SHI_QING, &CChongQingSSC::OnBnClickedBtnShiQing)
	ON_BN_CLICKED(IDC_BTN_GE_QUAN, &CChongQingSSC::OnBnClickedBtnGeQuan)
	ON_BN_CLICKED(IDC_BTN_GE_DA, &CChongQingSSC::OnBnClickedBtnGeDa)
	ON_BN_CLICKED(IDC_BTN_GE_XIAO, &CChongQingSSC::OnBnClickedBtnGeXiao)
	ON_BN_CLICKED(IDC_BTN_GE_DAN, &CChongQingSSC::OnBnClickedBtnGeDan)
	ON_BN_CLICKED(IDC_BTN_GE_SHUANG, &CChongQingSSC::OnBnClickedBtnGeShuang)
	ON_BN_CLICKED(IDC_BTN_GE_QING, &CChongQingSSC::OnBnClickedBtnGeQing)
	ON_BN_CLICKED(IDC_BTN_NIU_QUAN, &CChongQingSSC::OnBnClickedBtnNiuQuan)
	ON_BN_CLICKED(IDC_BTN_NIU_DA, &CChongQingSSC::OnBnClickedBtnNiuDa)
	ON_BN_CLICKED(IDC_BTN_NIU_XIAO, &CChongQingSSC::OnBnClickedBtnNiuXiao)
	ON_BN_CLICKED(IDC_BTN_NIU_DAN, &CChongQingSSC::OnBnClickedBtnNiuDan)
	ON_BN_CLICKED(IDC_BTN_NIU_SHUANG, &CChongQingSSC::OnBnClickedBtnNiuShuang)
	ON_BN_CLICKED(IDC_BTN_NIU_QING, &CChongQingSSC::OnBnClickedBtnNiuQing)
	ON_BN_CLICKED(IDC_BTN_CHQ_ADD, &CChongQingSSC::OnBnClickedBtnChqAdd)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE( WM_BN_CLICK, &CChongQingSSC::onBnCLick)
 	ON_BN_CLICKED(IDC_RIO_WXZX, &CChongQingSSC::OnBnClickedRioWuXingZhx)
	ON_BN_CLICKED(IDC_RIO_WXZX_DAN, &CChongQingSSC::OnBnClickedRioWuXingZhxDan)
 	ON_BN_CLICKED(IDC_RIO_WXQS, &CChongQingSSC::OnBnClickedRioWuXingQs)				//����ǰ��
	ON_BN_CLICKED(IDC_RIO_WXQS_DAN, &CChongQingSSC::OnBnClickedRioWuXingQsDan)		//����ǰ�ĵ�ʽ
 	ON_BN_CLICKED(IDC_RIO_WXHS, &CChongQingSSC::OnBnClickedRioWuXingHS)				//���Ǻ���
	ON_BN_CLICKED(IDC_RIO_WXHS_DAN, &CChongQingSSC::OnBnClickedRioWuXingHSDan)		//���Ǻ��ĵ�ʽ
 	ON_BN_CLICKED(IDC_RIO_WXZS, &CChongQingSSC::OnBnClickedRioWuXingZS)				//����������ʽ
	ON_BN_CLICKED(IDC_RIO_WXZS_DAN, &CChongQingSSC::OnBnClickedRioWuXingZSDan)		//����������ʽ

	ON_BN_CLICKED(IDC_RIO_NN, &CChongQingSSC::OnBnClickedRioNiuNiu)
	ON_BN_CLICKED(IDC_RIO_WN, &CChongQingSSC::OnBnClickedRioWuNiu)
	ON_BN_CLICKED(IDC_RIO_NS, &CChongQingSSC::OnBnClickedRioNiuShu)
	ON_BN_CLICKED(IDC_RIO_HSZHX_FUSHI, &CChongQingSSC::OnBnClickedRioHszhxFushi)
	ON_BN_CLICKED(IDC_RIO_HSZHX_DANSHI, &CChongQingSSC::OnBnClickedRioHszhxDanshi)
	ON_BN_CLICKED(IDC_RIO_QSZHX_FUSHI, &CChongQingSSC::OnBnClickedRioQszhxFushi)
	ON_BN_CLICKED(IDC_RIO_QSZHX_DANSHI, &CChongQingSSC::OnBnClickedRioQszhxDanshi)
	ON_BN_CLICKED(IDC_RIO_RENXUAN2, &CChongQingSSC::OnBnClickedRioRenXuan2)
	ON_BN_CLICKED(IDC_RIO_RENXUAN2_DAN, &CChongQingSSC::OnBnClickedRioRenXuan2Dan)
	ON_BN_CLICKED(IDC_RIO_RENXUAN3, &CChongQingSSC::OnBnClickedRioRenXuan3)
	ON_BN_CLICKED(IDC_RIO_RENXUAN3_DAN, &CChongQingSSC::OnBnClickedRioRenXuan3Dan)
	ON_BN_CLICKED(IDC_RIO_RENXUAN3_ZUXUAN3, &CChongQingSSC::OnBnClickedRioRenXuan3Zx3)
	ON_BN_CLICKED(IDC_RIO_RENXUAN3_ZUXUAN3_DAN, &CChongQingSSC::OnBnClickedRioRenXuan3Zx3Dan)
	ON_BN_CLICKED(IDC_RIO_RENXUAN3_ZUXUAN6, &CChongQingSSC::OnBnClickedRioRenXuan3Zx6)
	ON_BN_CLICKED(IDC_RIO_RENXUAN3_ZUXUAN6_DAN, &CChongQingSSC::OnBnClickedRioRenXuan3Zx6Dan)
	ON_BN_CLICKED(IDC_RIO_RENXUAN3_HUNHE, &CChongQingSSC::OnBnClickedRioRenXuan3Hunhe)
	ON_BN_CLICKED(IDC_RIO_RENXUAN2_ZUXUAN, &CChongQingSSC::OnBnClickedRioRenXuan2Zx)
	ON_BN_CLICKED(IDC_RIO_RENXUAN2_ZUXUAN_DAN, &CChongQingSSC::OnBnClickedRioRenXuan2ZxDan)
	ON_BN_CLICKED(IDC_RIO_RENXUAN4, &CChongQingSSC::OnBnClickedRioRenXuan4)
	ON_BN_CLICKED(IDC_RIO_RENXUAN4_DAN, &CChongQingSSC::OnBnClickedRioRenXuan4Dan)
	ON_BN_CLICKED(IDC_RIO_HSZS_FUSHI, &CChongQingSSC::OnBnClickedRioHszsFushi)
	ON_BN_CLICKED(IDC_RIO_ZSZS_FUSHI, &CChongQingSSC::OnBnClickedRioZszsFushi)
	ON_BN_CLICKED(IDC_RIO_ZSZS_DANSHI, &CChongQingSSC::OnBnClickedRioZszsDanshi)
	ON_BN_CLICKED(IDC_RIO_ZSZL_FUSHI, &CChongQingSSC::OnBnClickedRioZszlFushi)
	ON_BN_CLICKED(IDC_RIO_ZSZL_DANSHI, &CChongQingSSC::OnBnClickedRioZszlDanshi)
	ON_BN_CLICKED(IDC_RIO_HSZS_DANSHI, &CChongQingSSC::OnBnClickedRioHszsDanshi)
	ON_BN_CLICKED(IDC_RIO_HSZL_FUSHI, &CChongQingSSC::OnBnClickedRioHszlFushi)
	ON_BN_CLICKED(IDC_RIO_HSZL_DANSHI, &CChongQingSSC::OnBnClickedRioHszlDanshi)
	ON_BN_CLICKED(IDC_RIO_QSZS_FUSHI, &CChongQingSSC::OnBnClickedRioQszsFushi)
	ON_BN_CLICKED(IDC_RIO_QSZS_DANSHI, &CChongQingSSC::OnBnClickedRioQszsDanshi)
	ON_BN_CLICKED(IDC_RIO_QSZL_FUSHI, &CChongQingSSC::OnBnClickedRioQszlFushi)
	ON_BN_CLICKED(IDC_RIO_QSZL_DANSHI, &CChongQingSSC::OnBnClickedRioQszlDanshi)
	ON_BN_CLICKED(IDC_RIO_HEZHX_FUSHI, &CChongQingSSC::OnBnClickedRioHezhxFushi)
	ON_BN_CLICKED(IDC_RIO_HEZHX_DANSHI, &CChongQingSSC::OnBnClickedRioHezhxDanshi)
	ON_BN_CLICKED(IDC_RIO_HEZX_FUSHI, &CChongQingSSC::OnBnClickedRioHezxFushi)
	ON_BN_CLICKED(IDC_RIO_HEZX_DANSHI, &CChongQingSSC::OnBnClickedRioHezxDanshi)
	ON_BN_CLICKED(IDC_RIO_QEZHX_FUSHI, &CChongQingSSC::OnBnClickedRioQezhxFushi)
	ON_BN_CLICKED(IDC_RIO_QEZHX_DANSHI, &CChongQingSSC::OnBnClickedRioQezhxDanshi)
	ON_BN_CLICKED(IDC_RIO_QEZX_FUSHI, &CChongQingSSC::OnBnClickedRioQezxFushi)
	ON_BN_CLICKED(IDC_RIO_QEZX_DANSHI, &CChongQingSSC::OnBnClickedRioQezxDanshi)
	ON_BN_CLICKED(IDC_RIO_DXDSH, &CChongQingSSC::OnBnClickedRioDxdsh)
	ON_BN_CLICKED(IDC_RIO_DXDSH_DW, &CChongQingSSC::OnBnClickedRioDxdshDw)
	ON_BN_CLICKED(IDC_RIO_DXDSH_HZH, &CChongQingSSC::OnBnClickedRioDxdshHzh)
	ON_BN_CLICKED(IDC_RIO_QSBDW, &CChongQingSSC::OnBnClickedRioQsbdw)
	ON_BN_CLICKED(IDC_RIO_ZSBDW, &CChongQingSSC::OnBnClickedRioZsbdw)
	ON_BN_CLICKED(IDC_RIO_HSBDW, &CChongQingSSC::OnBnClickedRioHsbdw)
	ON_BN_CLICKED(IDC_RIO_QSHUNHE, &CChongQingSSC::OnBnClickedRioQsHunhe)
	ON_BN_CLICKED(IDC_RIO_ZSHUNHE, &CChongQingSSC::OnBnClickedRioZsHunhe)
	ON_BN_CLICKED(IDC_RIO_HSHUNHE, &CChongQingSSC::OnBnClickedRioHsHunhe)
	ON_BN_CLICKED(IDC_RIO_DWD, &CChongQingSSC::OnBnClickedRioDwd)
	ON_BN_CLICKED(IDC_RIO_HEBDW, &CChongQingSSC::OnBnClickedRioHebdw)
	ON_BN_CLICKED(IDC_RIO_QEBDW, &CChongQingSSC::OnBnClickedRioQebdw)

	ON_BN_CLICKED(IDC_RIO_ZX_120, &CChongQingSSC::OnBnClickedRioZuXuan120)
	ON_BN_CLICKED(IDC_RIO_ZX_60, &CChongQingSSC::OnBnClickedRioZuXuan60)
	ON_BN_CLICKED(IDC_RIO_ZX_30, &CChongQingSSC::OnBnClickedRioZuXuan30)
	ON_BN_CLICKED(IDC_RIO_ZX_20, &CChongQingSSC::OnBnClickedRioZuXuan20)
	ON_BN_CLICKED(IDC_RIO_ZX_10, &CChongQingSSC::OnBnClickedRioZuXuan10)
	ON_BN_CLICKED(IDC_RIO_ZX_5, &CChongQingSSC::OnBnClickedRioZuXuan5)

	ON_BN_CLICKED(IDC_RIO_QS_ZX24, &CChongQingSSC::OnBnClickedRioQsZuXuan24)
	ON_BN_CLICKED(IDC_RIO_QS_ZX12, &CChongQingSSC::OnBnClickedRioQsZuXuan12)
	ON_BN_CLICKED(IDC_RIO_QS_ZX6, &CChongQingSSC::OnBnClickedRioQsZuXuan6)
	ON_BN_CLICKED(IDC_RIO_QS_ZX4, &CChongQingSSC::OnBnClickedRioQsZuXuan4)
	ON_BN_CLICKED(IDC_RIO_HS_ZX24, &CChongQingSSC::OnBnClickedRioHsZuXuan24)
	ON_BN_CLICKED(IDC_RIO_HS_ZX12, &CChongQingSSC::OnBnClickedRioHsZuXuan12)
	ON_BN_CLICKED(IDC_RIO_HS_ZX6, &CChongQingSSC::OnBnClickedRioHsZuXuan6)
	ON_BN_CLICKED(IDC_RIO_HS_ZX4, &CChongQingSSC::OnBnClickedRioHsZuXuan4)

	ON_BN_CLICKED(IDC_RIO_YFFS, &CChongQingSSC::OnBnClickedRioYffs)
	ON_BN_CLICKED(IDC_RIO_HSCS, &CChongQingSSC::OnBnClickedRioHscs)
	ON_BN_CLICKED(IDC_RIO_SXBX, &CChongQingSSC::OnBnClickedRioSxbx)
	ON_BN_CLICKED(IDC_RIO_SJFC, &CChongQingSSC::OnBnClickedRioSjfc)

	ON_BN_CLICKED(IDC_RIO_LHD_WQ, &CChongQingSSC::OnBnClickedRioLhdWq)
	ON_BN_CLICKED(IDC_RIO_LHD_WB, &CChongQingSSC::OnBnClickedRioLhdWb)
	ON_BN_CLICKED(IDC_RIO_LHD_WS, &CChongQingSSC::OnBnClickedRioLhdWs)
	ON_BN_CLICKED(IDC_RIO_LHD_WG, &CChongQingSSC::OnBnClickedRioLhdWg)
	ON_BN_CLICKED(IDC_RIO_LHD_QB, &CChongQingSSC::OnBnClickedRioLhdQb)
	ON_BN_CLICKED(IDC_RIO_LHD_QS, &CChongQingSSC::OnBnClickedRioLhdQs)
	ON_BN_CLICKED(IDC_RIO_LHD_QG, &CChongQingSSC::OnBnClickedRioLhdQg)
	ON_BN_CLICKED(IDC_RIO_LHD_BS, &CChongQingSSC::OnBnClickedRioLhdBs)
	ON_BN_CLICKED(IDC_RIO_LHD_BG, &CChongQingSSC::OnBnClickedRioLhdBg)
	ON_BN_CLICKED(IDC_RIO_LHD_SG, &CChongQingSSC::OnBnClickedRioLhdSg)

	ON_BN_CLICKED(IDC_RIO_QS_HEZHI, &CChongQingSSC::OnBnClickedRioQsHezhi)
	ON_BN_CLICKED(IDC_RIO_ZS_HEZHI, &CChongQingSSC::OnBnClickedRioZsHezhi)
	ON_BN_CLICKED(IDC_RIO_HS_HEZHI, &CChongQingSSC::OnBnClickedRioHsHezhi)
	ON_BN_CLICKED(IDC_RIO_QE_HEZHI, &CChongQingSSC::OnBnClickedRioQeHezhi)
	ON_BN_CLICKED(IDC_RIO_HE_HEZHI, &CChongQingSSC::OnBnClickedRioHeHezhi)

	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ZHUIHAO, &CChongQingSSC::OnBnClickedBtnZhuihao)
	ON_BN_CLICKED(IDC_BTN_MORE_RECORD, &CChongQingSSC::OnBnClickedBtnMoreRecord)
	ON_BN_CLICKED(IDC_BTN_QUSHI, &CChongQingSSC::OnBnClickedBtnQuShi)
	ON_BN_CLICKED(IDC_BTN_TOUZHU, &CChongQingSSC::OnBnClickedBtnTouzhu)
	ON_BN_CLICKED(IDC_BTN_TOUZHU_ONEKEY, &CChongQingSSC::OnBnClickedBtnTouzhuOneKey)
	ON_COMMAND(ID_RBTN_CPY, &CChongQingSSC::OnRichEditCopy)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_DEL_SEL, &CChongQingSSC::OnBnClickedBtnDelSel)
	ON_BN_CLICKED(IDC_BTN_CLS_LIST, &CChongQingSSC::OnBnClickedBtnClsList)
	ON_EN_CHANGE(IDC_EDIT_BEISHU, &CChongQingSSC::OnEnChangeEditBeishu)
	ON_EN_CHANGE(IDC_RICHEDIT_DANSHI, &CChongQingSSC::OnEnChangeRichEditDanshiInput)
	
	//ON_NOTIFY(ENM_CHANGE, IDC_EDIT_DANSHI_HAOMA, &CChongQingSSC::OnEnChangeRichEditDanshiInput)
	//ON_COMMAND(IDC_RICHEDIT_DANSHI, &CChongQingSSC::OnEnChangeRichEditDanshiInput)
	ON_WM_CTLCOLOR()

	ON_WM_MOUSEMOVE()
 	ON_WM_LBUTTONUP()
 	ON_WM_LBUTTONDOWN()
 	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)

	ON_BN_CLICKED(IDC_RIO_MOUSE_INPUT, &CChongQingSSC::OnBnClickedRioMouseInput)
	ON_BN_CLICKED(IDC_RIO_KEYBOARD_INPUT, &CChongQingSSC::OnBnClickedRioKeyboardInput)
	ON_BN_CLICKED(IDC_CHK_WAN, &CChongQingSSC::OnBnClickedChkWan)
	ON_BN_CLICKED(IDC_CHK_QIAN, &CChongQingSSC::OnBnClickedChkQian)
	ON_BN_CLICKED(IDC_CHK_BAI, &CChongQingSSC::OnBnClickedChkBai)
	ON_BN_CLICKED(IDC_CHK_SHI, &CChongQingSSC::OnBnClickedChkShi)
	ON_BN_CLICKED(IDC_CHKGE, &CChongQingSSC::OnBnClickedChkge)
	ON_BN_CLICKED(IDC_BTN_LOCK, &CChongQingSSC::OnBnClickedBtnLock)

	ON_BN_CLICKED(IDC_BTN_BEITOU_JIA, &CChongQingSSC::OnBnClickedBeiTouJia)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIAN, &CChongQingSSC::OnBnClickedBeiTouJian)
	ON_BN_CLICKED(IDC_BTN_QIAN_5, &CChongQingSSC::OnBnClickedBtnQian5)
	END_MESSAGE_MAP()
LRESULT CChongQingSSC::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	//OnBnClickedBtnDelSel();
	nSubItem--;

	CString strText = m_listChqSSC.GetItemText(nItem,0);
	if(strText == _T("������ӳɹ�"))
	{
		m_strHaoma.Empty();
	}

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
void CChongQingSSC::OnRichEditCopy()
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

// CChongQingSSC ��Ϣ�������
void CChongQingSSC::OnPaint()
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
//	cacheDC.TextOut(0, 0, m_strTopNewsMsg,m_strTopNewsMsg.GetLength()); 

	cacheDC.SetBkMode(TRANSPARENT);
	cacheDC.SetTextColor(RGB(88, 78, 77));


	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	
	cacheDC.SelectObject(pOldCacheBmp);
	cacheDC.DeleteDC();
	cacheBmp.DeleteObject();

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CChongQingSSC::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CChongQingSSC::OnSize(UINT nType, int cx, int cy)
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

void CChongQingSSC::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	OnBnClickedBtnClsList();
	ResetAllNums();
	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		SendToServer(6);
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

		m_bKaiJiangzhong = true;
		OutputDebugString(L"KAIJIANG true 1");
// 		SetTimer(timer_id_get_luck_num, 10000, NULL);
		SetTimer(timer_id_kj_yugao, 1000, NULL);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);
		if(m_TypeID == CZ_HGYDWFC)
		{
			SetTimer(timer_xgqh,3000,NULL);
		}
		OnBnClickedBtnHszhx();

	}
	else
	{
		_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, _T(""));
		m_DlgOpenRecord.ShowWindow(SW_HIDE);
	}
}

HBRUSH CChongQingSSC::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	//nCtlColor == CTLCOLOR_EDIT && 
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_DANSHI_HAOMA || pWnd->GetDlgCtrlID() == IDC_EDIT_DANSHI_HAOMA) 
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(m_hBrush)
		{
			DeleteObject(m_hBrush);
			m_hBrush = CreateSolidBrush(RGB(61,47,42));
		}
		else
		{
			m_hBrush = CreateSolidBrush(RGB(61,47,42));
		}

		return m_hBrush;
	}
	//�����
	if( nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		//if(m_hBrush)
		//{
		//	DeleteObject(m_hBrush);
		//	m_hBrush = CreateSolidBrush(RGB(245,229,203));
		//}
		//else
		//{
		//	m_hBrush = CreateSolidBrush(RGB(245,229,203));
		//}

		//return m_hBrush;
	}

// 	if(pWnd->GetDlgCtrlID() == IDC_STATIC_RIO1)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		pDC->SelectObject(&m_Font);
// 		if(m_hBrush)
// 		{
// 			DeleteObject(m_hBrush);
// 			m_hBrush = CreateSolidBrush(RGB(245,229,203));
// 		}
// 		else
// 		{
// 			m_hBrush = CreateSolidBrush(RGB(245,229,203));
// 		}
// 
// 		return m_hBrush;
// 
// 	}
	if(pWnd->GetDlgCtrlID() == IDC_RIO_ZX_60)
	{
		HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CDC* dc = GetDC();
		pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);

		ReleaseDC(dc);
		hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
		return hbr;

	}
	if (pWnd->GetDlgCtrlID() == IDC_RICHEDIT_DANSHI) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(m_hBrush)
		{
			DeleteObject(m_hBrush);
			m_hBrush = CreateSolidBrush(RGB(61,47,42));
		}
		else
		{
			m_hBrush = CreateSolidBrush(RGB(61,47,42));
		}

		return m_hBrush;
	}
// 	//���ģʽ����ʾ��Ϣ
// 	if(CTLCOLOR_STATIC==nCtlColor && (/*pWnd->GetDlgCtrlID() == IDC_STATIC_HUNHE_TIP ||*/ pWnd->GetDlgCtrlID() == IDC_STATIC_NIUNIU_TIP))
// 	{
// 		pDC->SetBkMode(TRANSPARENT); 
// 		pDC->SetTextColor(RGB(250, 250, 250));
// 		if(m_hBrush)
// 		{
// 			DeleteObject(m_hBrush);
// 			m_hBrush = CreateSolidBrush(RGB(75,59,52));
// 		}
// 		else
// 		{
// 			m_hBrush = CreateSolidBrush(RGB(75,59,52));
// 		}
// 
// 		return m_hBrush;
// 	}
	//��ѡ��ʾSTATIC
	if(CTLCOLOR_STATIC==nCtlColor && pWnd->GetDlgCtrlID() == IDC_STATIC_RENXUAN_TIP)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(m_hBrush)
		{
			DeleteObject(m_hBrush);
			m_hBrush = CreateSolidBrush(RGB(214,223,254));
		}
		else
		{
			m_hBrush = CreateSolidBrush(RGB(214,223,254));
		}

		return m_hBrush;
	}

	if(pWnd->GetDlgCtrlID() == IDC_EDIT_BEISHU)
	{
		pDC->SetBkMode(TRANSPARENT); 

		pDC->SetBkColor(RGB(251,245,233));
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
 			if(m_hBrush)
 			{
 				DeleteObject(m_hBrush);
 				m_hBrush = CreateSolidBrush(RGB(214,223,254));
 			}
 			else
 			{
 				m_hBrush = CreateSolidBrush(RGB(214,223,254));
 			}

			return m_hBrush;
 				//return HBRUSH(GetStockObject(NULL_BRUSH));
		}

//	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CChongQingSSC::OnTimer(UINT_PTR nIDEvent)
{

	if (timer_id_kj_yugao == nIDEvent)
	{
		CRect rcRect;
		rcRect.CopyRect(rcYuGao);
		//rcRect.right += 50;
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
	else if(timer_xgqh == nIDEvent)
	{
		SendToServer(9);
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
				int nLeft=kj_big_haoma_x -17/*+ n *( m_bigNumWidth+8)*/;
				CMemDC	mDC(pDC, CRect(nLeft, kj_big_haoma_y-2, nLeft+m_imgKaijiang->GetWidth()+200, kj_big_haoma_y+m_imgKaijiang->GetHeight()+37));
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
	else if(nIDEvent == TimerStatus)
	{
		KillTimer(TimerStatus);
		m_DlgStatus.HideStatusWindow();
		CInformation Information;
		Information.ShowMessageBox(L"��ܰ����",L"��������æ��Ͷעʧ��",MB_OK,5);
	}
	else if(nIDEvent == timer_getcanadaqihao)
	{
		SendToServer(10);
	}
	CDialog::OnTimer(nIDEvent);
}
void CChongQingSSC::OnBnClickedBtnYuan()
{
	m_btnYuan.SetPushed(true);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);

// 	if(theAccount.Scoretype == MoShi_Jiao)
// 	{
// 		m_zongJine *= 10;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Fen)
// 	{
// 		m_zongJine *= 100;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Li)
// 	{
// 		m_zongJine *= 1000;
// 	}
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
void CChongQingSSC::OnBnClickedBtnJiao()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(true);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
// 	if(theAccount.Scoretype == MoShi_Yuan)
// 	{
// 		m_zongJine /= 10;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Fen)
// 	{
// 		m_zongJine *= 10;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Li)
// 	{
// 		m_zongJine *= 100;
// 	}

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
void CChongQingSSC::OnBnClickedBtnFen()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(true);
	m_btnLi.SetPushed(false);
// 	if(theAccount.Scoretype == MoShi_Yuan)
// 	{
// 		m_zongJine /= 100;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Jiao)
// 	{
// 		m_zongJine /= 10;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Li)
// 	{
// 		m_zongJine *= 10;
// 	}

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
void CChongQingSSC::OnBnClickedBtnLi()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(true);
// 	if(theAccount.Scoretype == MoShi_Yuan)
// 	{
// 		m_zongJine /= 1000;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Jiao)
// 	{
// 		m_zongJine /= 100;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Fen)
// 	{
// 		m_zongJine /= 10;
// 	}
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
//����ֱѡ: ������Ϊ����ֱѡ
void CChongQingSSC::OnBnClickedBtnHszhx()
{
	HideHeZhiNums();
	CheckWanFaBtn(m_btnHouSanZhiXuan);

	HideAllRio();
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"����ֱѡ:");
	m_staticRio2.ShowWindow(SW_SHOW);
	m_staticRio2.SetWindowText(L"������ѡ:");
	m_staticRio3.ShowWindow(SW_HIDE);

	ShowCtrl(IDC_RIO_WXZX);				//����ֱѡ

	ShowCtrl(IDC_RIO_WXZX_DAN);				//����ֱѡ
	ShowCtrl(IDC_RIO_ZX_120);
	ShowCtrl(IDC_RIO_ZX_60);
	ShowCtrl(IDC_RIO_ZX_30);
	ShowCtrl(IDC_RIO_ZX_20);
	ShowCtrl(IDC_RIO_ZX_10);
	ShowCtrl(IDC_RIO_ZX_5);

	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);

	OnBnClickedRioWuXingZhx();
}
//ţţ
void CChongQingSSC::OnBnClickedBtnNiuNiu()
{
	HideHeZhiNums();
	CheckWanFaBtn(m_btnNiuNiu);
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"ţţ:");
	m_staticRio2.ShowWindow(SW_HIDE);
	m_staticRio3.ShowWindow(SW_HIDE);

	HideAllRio();

	ShowCtrl(IDC_RIO_NS);				//ţţ
	HideCtrl(IDC_STATIC_NIUNIU_TIP);				//ţţ
	//ShowCtrl(IDC_RIO_NN);				//ţţ
//	ShowCtrl(IDC_RIO_WN);				//ţţ


// 	ShowCtrl(IDC_RIO_MOUSE_INPUT);
// 	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

	OnBnClickedRioNiuShu();
}
//������
void CChongQingSSC::OnBnClickedBtnLhd()
{
	HideHeZhiNums();

	CheckWanFaBtn(m_btnLhd);
	m_staticRio1.ShowWindow(SW_HIDE);
	m_staticRio2.SetWindowText(L"������:");
	m_staticRio2.ShowWindow(SW_SHOW);
	m_staticRio3.ShowWindow(SW_HIDE);

	HideAllRio();
	m_gameKind = SSC_LongHu;
	HideNiuNums();


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	SendToServer(3);
	ShowCtrl(IDC_RIO_LHD_WQ);
	ShowCtrl(IDC_RIO_LHD_WB);
	ShowCtrl(IDC_RIO_LHD_WS);
	ShowCtrl(IDC_RIO_LHD_WG);
	ShowCtrl(IDC_RIO_LHD_QB);
	ShowCtrl(IDC_RIO_LHD_QS);
	ShowCtrl(IDC_RIO_LHD_QG);
	ShowCtrl(IDC_RIO_LHD_BS);
	ShowCtrl(IDC_RIO_LHD_BG);
	ShowCtrl(IDC_RIO_LHD_SG);

	OnBnClickedRioLhdWq();

}
void CChongQingSSC::OnBnClickedBtnHszx()
{
	//m_wanfa = HouSanZuXuan;

	HideHeZhiNums();
	CheckWanFaBtn(m_btnHouSanZuXuan);
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"ֱѡ��ֵ:");
	m_staticRio2.ShowWindow(SW_HIDE);
	m_staticRio3.ShowWindow(SW_HIDE);

	HideAllRio();

	ShowCtrl(IDC_RIO_QS_HEZHI);
	ShowCtrl(IDC_RIO_ZS_HEZHI);
	ShowCtrl(IDC_RIO_HS_HEZHI);
	ShowCtrl(IDC_RIO_QE_HEZHI);
	ShowCtrl(IDC_RIO_HE_HEZHI);

	m_RenxuanKeyboardInput = false;

	OnBnClickedRioQsHezhi();
}
//ǰ��
void CChongQingSSC::OnBnClickedBtnWxhe()
{
	CheckWanFaBtn(m_btnWuXingHouEr);

	HideHeZhiNums();
	HideAllRio();
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"ǰ��ֱѡ:");
	m_staticRio2.ShowWindow(SW_SHOW);
	m_staticRio2.SetWindowText(L"ǰ����ѡ:");
	m_staticRio3.ShowWindow(SW_HIDE);

	ShowCtrl(IDC_RIO_QSZL_FUSHI);
	ShowCtrl(IDC_RIO_QSZL_DANSHI);
	ShowCtrl(IDC_RIO_QSZS_FUSHI);
	ShowCtrl(IDC_RIO_QSZS_DANSHI);
	ShowCtrl(IDC_RIO_QSZHX_FUSHI);
	ShowCtrl(IDC_RIO_QSZHX_DANSHI);

	ShowCtrl(IDC_RIO_QSHUNHE);

	OnBnClickedRioQszhxFushi();
}
//����
void CChongQingSSC::OnBnClickedBtnHezx()
{
	//m_wanfa = HouErZuXuan;
	CheckWanFaBtn(m_btnHouErZuXuan);
	HideHeZhiNums();
	HideAllRio();
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"����ֱѡ:");
	m_staticRio2.ShowWindow(SW_SHOW);
	m_staticRio2.SetWindowText(L"������ѡ:");
	m_staticRio3.ShowWindow(SW_HIDE);

	ShowCtrl(IDC_RIO_HSZL_FUSHI);
	ShowCtrl(IDC_RIO_HSZL_DANSHI);
	ShowCtrl(IDC_RIO_HSZS_FUSHI);
	ShowCtrl(IDC_RIO_HSZS_DANSHI);
	ShowCtrl(IDC_RIO_HSZHX_FUSHI);
	ShowCtrl(IDC_RIO_HSZHX_DANSHI);

	ShowCtrl(IDC_RIO_HSHUNHE);
	
	m_RenxuanKeyboardInput = false;
	OnBnClickedRioHszhxFushi();
}

void CChongQingSSC::OnBnClickedBtnDxdsh()
{
	//m_wanfa = DaXiaoDanShuang;
	CheckWanFaBtn(m_btnDaXiaoDanShuang);
	HideHeZhiNums();
	HideAllRio();
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"��С��˫:");
	m_staticRio2.ShowWindow(SW_HIDE);
	m_staticRio3.ShowWindow(SW_HIDE);

	ShowCtrl(IDC_RIO_DXDSH);
	ShowCtrl(IDC_RIO_DXDSH_DW);
	ShowCtrl(IDC_RIO_DXDSH_HZH);
	//SelctRadioBtn(IDC_RIO_DXDSH);
	m_RenxuanKeyboardInput = false;
	OnBnClickedRioDxdsh();
}

void CChongQingSSC::OnBnClickedBtnBdw()
{
	//m_wanfa = BuDingWei;
	CheckWanFaBtn(m_btnBuDingWei);
	HideHeZhiNums();
	HideAllRio();
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"����λ:");
	m_staticRio2.ShowWindow(SW_HIDE);
	m_staticRio3.ShowWindow(SW_HIDE);

	ShowCtrl(IDC_RIO_HSBDW);
	ShowCtrl(IDC_RIO_ZSBDW);
	ShowCtrl(IDC_RIO_QSBDW);
	ShowCtrl(IDC_RIO_HEBDW);
	ShowCtrl(IDC_RIO_QEBDW);
	//SelctRadioBtn(IDC_RIO_HSBDW);
	m_RenxuanKeyboardInput = false;
	OnBnClickedRioQsbdw();
}

//��ѡ��2 3 4
void CChongQingSSC::OnBnClickedBtnQszhx()
{
////��ѡ2,3,4
	CheckWanFaBtn(m_btnQianSanZhiXuan);
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"����:");
	m_staticRio2.ShowWindow(SW_SHOW);
	m_staticRio2.SetWindowText(L"����:");
	m_staticRio3.ShowWindow(SW_SHOW);
	m_staticRio3.SetWindowText(L"�ζ�:");

	HideHeZhiNums();
	HideAllRio();
	ShowCtrl(IDC_RIO_RENXUAN2);
	ShowCtrl(IDC_RIO_RENXUAN2_DAN);
	ShowCtrl(IDC_RIO_RENXUAN2_ZUXUAN);
	ShowCtrl(IDC_RIO_RENXUAN2_ZUXUAN_DAN);
	ShowCtrl(IDC_RIO_RENXUAN3);
	ShowCtrl(IDC_RIO_RENXUAN3_DAN);
	ShowCtrl(IDC_RIO_RENXUAN4);
	ShowCtrl(IDC_RIO_RENXUAN4_DAN);
	ShowCtrl(IDC_RIO_RENXUAN3_ZUXUAN3);
	ShowCtrl(IDC_RIO_RENXUAN3_ZUXUAN3_DAN);
	ShowCtrl(IDC_RIO_RENXUAN3_ZUXUAN6);
	ShowCtrl(IDC_RIO_RENXUAN3_ZUXUAN6_DAN);
	ShowCtrl(IDC_RIO_RENXUAN3_HUNHE);

	OnBnClickedRioRenXuan4();
}

void CChongQingSSC::OnBnClickedBtnQszx()
{
	//m_wanfa = QianSanZuXuan;
	CheckWanFaBtn(m_btnQianSanZuXuan);

	HideHeZhiNums();
	HideAllRio();

	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"ǰ��ֱѡ:");
	m_staticRio2.ShowWindow(SW_SHOW);
	m_staticRio2.SetWindowText(L"ǰ����ѡ:");
	m_staticRio3.ShowWindow(SW_HIDE);

	ShowCtrl(IDC_RIO_WXQS);
	ShowCtrl(IDC_RIO_WXQS_DAN);
	ShowCtrl(IDC_RIO_QS_ZX24);
	ShowCtrl(IDC_RIO_QS_ZX12);
	ShowCtrl(IDC_RIO_QS_ZX6);
	ShowCtrl(IDC_RIO_QS_ZX4);

	OnBnClickedRioWuXingQs();
// 	ShowCtrl(IDC_RIO_QSZS_FUSHI);
// 	ShowCtrl(IDC_RIO_QSZL_FUSHI);
// 	ShowCtrl(IDC_RIO_HSZS_FUSHI);
// 	ShowCtrl(IDC_RIO_HSZL_FUSHI);
// 	ShowCtrl(IDC_RIO_ZSZS_FUSHI);
// 	ShowCtrl(IDC_RIO_ZSZL_FUSHI);
// 	ShowCtrl(IDC_RIO_ZX_120);
// 	ShowCtrl(IDC_RIO_ZX_60);
// 	ShowCtrl(IDC_RIO_ZX_30);
// 	ShowCtrl(IDC_RIO_ZX_20);
// 	ShowCtrl(IDC_RIO_ZX_10);
// 	ShowCtrl(IDC_RIO_ZX_5);
// 
// 	OnBnClickedRioQszsFushi();
}

void CChongQingSSC::OnBnClickedBtnWxqe()
{
	CheckWanFaBtn(m_btnWuXingQianEr);

	HideAllRio();
	HideHeZhiNums();
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"����ֱѡ:");
	m_staticRio2.ShowWindow(SW_SHOW);
	m_staticRio2.SetWindowText(L"������ѡ:");
	m_staticRio3.ShowWindow(SW_HIDE);

	ShowCtrl(IDC_RIO_WXHS);
	ShowCtrl(IDC_RIO_WXHS_DAN);
	ShowCtrl(IDC_RIO_HS_ZX24);
	ShowCtrl(IDC_RIO_HS_ZX12);
	ShowCtrl(IDC_RIO_HS_ZX6);
	ShowCtrl(IDC_RIO_HS_ZX4);
// 	ShowCtrl(IDC_RIO_WXZS);
// 	ShowCtrl(IDC_RIO_HSZHX_FUSHI);

// 	ShowCtrl(IDC_RIO_MOUSE_INPUT);
// 	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

	OnBnClickedRioWuXingHS();
}
//����
void CChongQingSSC::OnBnClickedBtnQezx()
{
	CheckWanFaBtn(m_btnQianErZuXuan);

	HideHeZhiNums();
	HideAllRio();
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"����ֱѡ:");
	m_staticRio2.ShowWindow(SW_SHOW);
	m_staticRio2.SetWindowText(L"������ѡ:");
	m_staticRio3.ShowWindow(SW_HIDE);

	ShowCtrl(IDC_RIO_WXZS);
	ShowCtrl(IDC_RIO_WXZS_DAN);
	ShowCtrl(IDC_RIO_ZSZS_FUSHI);
	ShowCtrl(IDC_RIO_ZSZS_DANSHI);
	ShowCtrl(IDC_RIO_ZSZL_FUSHI);
	ShowCtrl(IDC_RIO_ZSZL_DANSHI);
	ShowCtrl(IDC_RIO_ZSHUNHE);
	m_RenxuanKeyboardInput = false;
	OnBnClickedRioWuXingZS();
}
//����
void CChongQingSSC::OnBnClickedBtnErXing()
{
	CheckWanFaBtn(m_btnQianErXing);

	HideHeZhiNums();
	HideAllRio();
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"����ֱѡ:");
	m_staticRio2.ShowWindow(SW_SHOW);
	m_staticRio2.SetWindowText(L"������ѡ:");
	m_staticRio3.ShowWindow(SW_HIDE);

	HideHeZhiNums();
	HideAllRio();

	HideHeZhiNums();
	HideAllRio();

	ShowCtrl(IDC_RIO_QEZHX_FUSHI);
	ShowCtrl(IDC_RIO_QEZHX_DANSHI);
	ShowCtrl(IDC_RIO_HEZHX_FUSHI);
	ShowCtrl(IDC_RIO_HEZHX_DANSHI);
	ShowCtrl(IDC_RIO_QEZX_FUSHI);
	ShowCtrl(IDC_RIO_QEZX_DANSHI);

	ShowCtrl(IDC_RIO_HEZX_FUSHI);
	ShowCtrl(IDC_RIO_HEZX_DANSHI);

	m_RenxuanKeyboardInput = false;
	OnBnClickedRioQezhxFushi();
}

void CChongQingSSC::OnBnClickedBtnHszhxhzh()
{
	////m_wanfa = HouSanZhiXuanHeZhi;
	CheckWanFaBtn(m_btnHouSanHeZhi);

	HideHeZhiNums();
	HideAllRio();
	ShowCtrl(IDC_RIO_QSHUNHE);
	ShowCtrl(IDC_RIO_ZSHUNHE);
	ShowCtrl(IDC_RIO_HSHUNHE);
	ShowCtrl(IDC_STATIC_HUNHE_TIP);
	m_RenxuanKeyboardInput = false;

//	SelctRadioBtn(IDC_RIO_HSZHXHZH);
	OnBnClickedRioQsHunhe();
}

//��λ��
void CChongQingSSC::OnBnClickedBtnDwd()
{
	//m_wanfa = DingWeiDan;
	CheckWanFaBtn(m_btnDingWeiDan);
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"��λ��:");
	m_staticRio2.ShowWindow(SW_HIDE);
	m_staticRio3.ShowWindow(SW_HIDE);

	HideHeZhiNums();
	HideAllRio();

	ShowCtrl(IDC_RIO_DWD);
	//SelctRadioBtn(IDC_RIO_DWD);
	m_RenxuanKeyboardInput = false;
	OnBnClickedRioDwd();
}
//Ȥζ
void CChongQingSSC::OnBnClickedBtnQw()
{
	//m_wanfa = DingWeiDan;
	CheckWanFaBtn(m_btnQuwei);
	m_staticRio1.ShowWindow(SW_SHOW);
	m_staticRio1.SetWindowText(L"Ȥζ:");
	m_staticRio2.ShowWindow(SW_HIDE);
	m_staticRio3.ShowWindow(SW_HIDE);

	HideHeZhiNums();
	HideAllRio();

	ShowCtrl(IDC_RIO_YFFS);
	ShowCtrl(IDC_RIO_HSCS);
	ShowCtrl(IDC_RIO_SXBX);
	ShowCtrl(IDC_RIO_SJFC);
	m_RenxuanKeyboardInput = false;

	OnBnClickedRioYffs();
}
BOOL CChongQingSSC::PreTranslateMessage(MSG* pMsg)
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
	if(pMsg-> message   ==ENM_CHANGE) 
	{
		if(pMsg->hwnd == m_richDanshiHaoma.m_hWnd)
		{
			CString strMessage;
			m_richDanshiHaoma.GetWindowText(strMessage);

			MyMessageBox(strMessage);
			return true;

		}
	}
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}
	//��ʾ��Ϣ
	if (m_ToolTipHelp.m_hWnd!=NULL)
	{
		m_ToolTipHelp.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

BOOL CChongQingSSC::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyle(WS_CLIPCHILDREN,0);
	
	ZeroMemory(m_szMD5Haoma,sizeof(m_szMD5Haoma));
	m_DlgOpenRecord.Create(COpenRecord::IDD,this);
	m_hBrush = NULL;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\game_bk3.png"));
	m_numImageList = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num.png"));
	m_numImageList1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num2.png"));
	m_numImageList2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num3.png"));
	m_numImageList3 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_lhd.png"));
	m_kjNumBig = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big.png"));
	m_imgKaijiang = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\KaiJiang.gif"));
	int nCount = m_imgKaijiang->GetFrameDimensionsCount();			//��ȡ֡ά��
	GUID *pGuids = new GUID[nCount];								//����һ��GUID����
	m_imgKaijiang->GetFrameDimensionsList(pGuids,nCount);			//��ȡͼ��֡��GUID
	m_nFrameCount=m_imgKaijiang->GetFrameCount(pGuids);				//��ȡGIF֡��
	m_bZhuiZjtz = true;

	m_bShowFail = true;
	m_bigNumWidth = m_kjNumBig->GetWidth() / 10;
	m_bigNumHeight = m_kjNumBig->GetHeight();
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small.png"));
	m_smallNumWidth = m_kjNumSmall->GetWidth() / 10;
	m_smallNumHeight = m_kjNumSmall->GetHeight();
	m_bmpDxdsh = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx2.png"));
	m_bmpNumTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit.png"));
	m_bmpNumTip1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit2.png"));

	m_bCanSend=true;
	InitNumberBtns();
	InitListCtrl();
	InitWanFaMoShi();
	m_richDanshiHaoma.SetEventMask(ENM_MOUSEEVENTS); 

	m_smallNumfont.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_zongFont.CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_KjInfoFont1.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_KjInfoFont.CreateFont(35, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 
	m_staticFont.CreateFont(18, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("΢���ź�")); 

	m_btnDelSel.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_delate.png"));
	m_btnClsList.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_clear.png"));
	m_btnTouZhu.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_bet.png"));
	m_btnTouZhuOnekey.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\yjtz.png"));
	m_btnZhuiHao.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_append.png"));
	m_btnMoreRecord.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_more.png"));
	m_btnQushi.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_hmzs.png"));
	m_btnChqAdd.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_addnum.png"));
	m_btnLock.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_lock.png"));
	m_btnLock.SetTextColor(RGB(51, 45, 42));
	m_btnLock.SetTextFont(&m_zongFont);

	m_btnBeiTouJia.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jia.png"));
	m_btnBeiTouJian.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jian.png"));

	m_btnYuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_1.png"));
	m_btnJiao.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_2.png"));
	m_btnFen.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_3.png"));
	m_btnLi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_4.png"));

	
	m_richDanshiHaoma.SetBackgroundColor(false,RGB(255,250,224));
	m_richDanshiHaoma.LimitText(-1);
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(56, 45, 56);
	m_richDanshiHaoma.SetDefaultCharFormat(cf);

	m_richDanshiHaoma.SetEventMask(ENM_CHANGE);

	m_editBeiTou.SetEnableColor(RGB(0,0,0),RGB(255,250,224),RGB(214,223,254));
	m_editBeiTou.SetFont(&m_zongFont);

	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.SetMaxTipWidth(150);

	m_ToolTipHelp.Create(this);
	m_ToolTipHelp.Activate(TRUE);
	m_ToolTipHelp.SetMaxTipWidth(150);
	m_ToolTipHelp.SetTipBkColor(RGB(251,40,24));

	m_bKaiJiangzhong=false;
	OutputDebugString(L"TXDONGHUA false 2");
	return TRUE; 
}

void CChongQingSSC::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
}

void CChongQingSSC::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}
void CChongQingSSC::OnBnClickedBtnNiuQuan()
{
	CheckNumBtn(m_btnNiu_1);
	CheckNumBtn(m_btnNiu_2);
	CheckNumBtn(m_btnNiu_3);
	CheckNumBtn(m_btnNiu_4);
	CheckNumBtn(m_btnNiu_5);
	CheckNumBtn(m_btnNiu_6);
	CheckNumBtn(m_btnNiu_7);
	CheckNumBtn(m_btnNiu_8);
	CheckNumBtn(m_btnNiu_9);
	CheckNumBtn(m_btnNiu_10);
	GetSSCZhushu(m_TypeID,m_gameKind);

}
void CChongQingSSC::OnBnClickedBtnNiuDa()
{
	UnCheckNumBtn(m_btnNiu_1);
	UnCheckNumBtn(m_btnNiu_2);
	UnCheckNumBtn(m_btnNiu_3);
	UnCheckNumBtn(m_btnNiu_4);
	UnCheckNumBtn(m_btnNiu_5);
	CheckNumBtn(m_btnNiu_6);
	CheckNumBtn(m_btnNiu_7);
	CheckNumBtn(m_btnNiu_8);
	CheckNumBtn(m_btnNiu_9);
	CheckNumBtn(m_btnNiu_10);
	GetSSCZhushu(m_TypeID,m_gameKind);

}
void CChongQingSSC::OnBnClickedBtnNiuXiao()
{
	CheckNumBtn(m_btnNiu_1);
	CheckNumBtn(m_btnNiu_2);
	CheckNumBtn(m_btnNiu_3);
	CheckNumBtn(m_btnNiu_4);
	CheckNumBtn(m_btnNiu_5);
	UnCheckNumBtn(m_btnNiu_6);
	UnCheckNumBtn(m_btnNiu_7);
	UnCheckNumBtn(m_btnNiu_8);
	UnCheckNumBtn(m_btnNiu_9);
	UnCheckNumBtn(m_btnNiu_10);
	GetSSCZhushu(m_TypeID,m_gameKind);

}
void CChongQingSSC::OnBnClickedBtnNiuDan()
{
	CheckNumBtn(m_btnNiu_1);
	UnCheckNumBtn(m_btnNiu_2);
	CheckNumBtn(m_btnNiu_3);
	UnCheckNumBtn(m_btnNiu_4);
	CheckNumBtn(m_btnNiu_5);
	UnCheckNumBtn(m_btnNiu_6);
	CheckNumBtn(m_btnNiu_7);
	UnCheckNumBtn(m_btnNiu_8);
	CheckNumBtn(m_btnNiu_9);
	UnCheckNumBtn(m_btnNiu_10);
	GetSSCZhushu(m_TypeID,m_gameKind);

}
void CChongQingSSC::OnBnClickedBtnNiuShuang()
{
	UnCheckNumBtn(m_btnNiu_1);
	CheckNumBtn(m_btnNiu_2);
	UnCheckNumBtn(m_btnNiu_3);
	CheckNumBtn(m_btnNiu_4);
	UnCheckNumBtn(m_btnNiu_5);
	CheckNumBtn(m_btnNiu_6);
	UnCheckNumBtn(m_btnNiu_7);
	CheckNumBtn(m_btnNiu_8);
	UnCheckNumBtn(m_btnNiu_9);
	CheckNumBtn(m_btnNiu_10);
	GetSSCZhushu(m_TypeID,m_gameKind);

}
void CChongQingSSC::OnBnClickedBtnNiuQing()
{
	UnCheckNumBtn(m_btnNiu_1);
	UnCheckNumBtn(m_btnNiu_2);
	UnCheckNumBtn(m_btnNiu_3);
	UnCheckNumBtn(m_btnNiu_4);
	UnCheckNumBtn(m_btnNiu_5);
	UnCheckNumBtn(m_btnNiu_6);
	UnCheckNumBtn(m_btnNiu_7);
	UnCheckNumBtn(m_btnNiu_8);
	UnCheckNumBtn(m_btnNiu_9);
	UnCheckNumBtn(m_btnNiu_10);
	GetSSCZhushu(m_TypeID,m_gameKind);

}


void CChongQingSSC::OnBnClickedBtnWanQuan()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnWanDa()
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
	GetSSCZhushu(m_TypeID,m_gameKind);

}

void CChongQingSSC::OnBnClickedBtnWanXiao()
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
	GetSSCZhushu(m_TypeID,m_gameKind);

}

void CChongQingSSC::OnBnClickedBtnWanDan()
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

	GetSSCZhushu(m_TypeID,m_gameKind);

}

void CChongQingSSC::OnBnClickedBtnWanShuang()
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
	GetSSCZhushu(m_TypeID,m_gameKind);

}

void CChongQingSSC::OnBnClickedBtnWanQing()
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

	GetSSCZhushu(m_TypeID,m_gameKind);

}

void CChongQingSSC::OnBnClickedBtnQianQuan()
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

	GetSSCZhushu(m_TypeID,m_gameKind);

}

void CChongQingSSC::OnBnClickedBtnQianDa()
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

	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnQianXiao()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnQianDan()
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

	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnQianShuang()
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

	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnQianQing()
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

	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnBaiQuan()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnBaiDa()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnBaiXiao()
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

	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnBaiDan()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnBaiShuang()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnBaiQing()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnShiQuan()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnShiDa()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnShiXiao()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnShiDan()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnShiShuang()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnShiQing()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnGeQuan()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnGeDa()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnGeXiao()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnGeDan()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnGeShuang()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}

void CChongQingSSC::OnBnClickedBtnGeQing()
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
	GetSSCZhushu(m_TypeID,m_gameKind);
}
CString CChongQingSSC::GetLongHu()
{
	CString strLonghu;

	if (IsNumBtnCheck(m_btnLong))
	{
		strLonghu += _T("6");
	}
	if (IsNumBtnCheck(m_btnHu))
	{
		strLonghu += _T("4");
	}
	if (IsNumBtnCheck(m_btnHe))
	{
		strLonghu += _T("5");
	}

	//strLonghu = L"123";
	return strLonghu;
}
//��Ӻ���
void CChongQingSSC::OnBnClickedBtnChqAdd()
{
	CString strWan = GetWanString();
	CString strQian = GetQianString();
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();
	CString strHezhi = GetHezhiString();
	CString strNiuNiu = GetNiuString();
	CString strNiuNum = GetNiuNumString();
	CString strHaoMa;
	int zhushu = 1;
	if (HouSanZhiXuan_FuShi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,3,3))
			{
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				m_DlgStatus.HideStatusWindow();

				return;
			}

// 			int nLength = strHaoMa.GetLength();
// 			if(nLength > 4800)
// 			{
// //				MyMessageBox(L"������ĺ��볬������ע�������������룡");
// 				MyMessageBox(L"������ĺ��볬������ע�������������룡");
// 				m_DlgStatus.HideStatusWindow();
// 
// 				return;
// 			}
			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;

// 			if(zhushu>800)
// 			{
// 				CString strTips;
// 				strTips.Format(L"����ֱѡ���Ͷ800ע��");
// 				MyMessageBox(strTips);
// 				return;
// 			}

		}
		else
		{
			if(strBai.IsEmpty() || strShi.IsEmpty() || strGe.IsEmpty()) 
			{
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				m_DlgStatus.HideStatusWindow();

				return;
			}

			strHaoMa = strBai + _T(",") + strShi + _T(",") + strGe;

			zhushu = strBai.GetLength() * strShi.GetLength() * strGe.GetLength();
			if(zhushu>800)
			{
				CString strTips;
				strTips.Format(L"����ֱѡ���Ͷ800ע��");
				MyMessageBox(strTips);
				return;
			}

		}
	}
	else if (HouSanZhiXuan_DanShi == m_gameKind) 
	{
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu,3200)) 
		{
			MyMessageBox(_T("����д���ʵĺ���"));
			m_DlgStatus.HideStatusWindow();

			return;
		}
	}
	else if (NiuNiu_Num == m_gameKind) 
	{
		if(strNiuNiu.GetLength()<2)
		{
			CString strTips;
			strTips.Format(L"���ĺ��������������������룡");
			MyMessageBox(strTips);
			m_DlgStatus.HideStatusWindow();

			return;
		}
		for (int i = 0;i < strNiuNum.GetLength();i++)
		{
			strHaoMa += strNiuNum.GetAt(i);
			if(i%2 == 1&&i!=strNiuNum.GetLength()-1)
			{
				strHaoMa += _T(",");
			}
		}



		zhushu = strNiuNiu.GetLength()/2;
		if(zhushu > 8)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("ţţ���Ͷ8ע��"));
			return;

		}

		m_strHaoma.Empty();
		wstring strdata;
		strdata = strHaoMa.GetBuffer(0);

		vecAllNums.insert(vecAllNums.begin(),1,strdata);
		strHaoMa = _T("");
		for (int i = 0;i < strNiuNiu.GetLength();i++)
		{
			strHaoMa += strNiuNiu.GetAt(i);
			if(i%2 == 1&&i!=strNiuNiu.GetLength()-1)
			{
				strHaoMa += _T(",");
			}
		}



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
		DOUBLE fJine = danzhujine*m_beishu*zhushu;
		CString strJine;
		strJine.Format(L"%.3lf",fJine);

		m_listChqSSC.SetItemText(nCount, 2, strJine);
		m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());
		CString strTmp;
		strTmp.Format(_T("%d"), zhushu);
		m_listChqSSC.SetItemText(nCount, 4, strTmp);

		strTmp.Format(_T("%d"), m_beishu);
		m_listChqSSC.SetItemText(nCount, 5, strTmp);

		m_listChqSSC.createItemButton(nCount, 6, this->GetSafeHwnd());

		m_zongZhuShu += zhushu;
	
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
		strHaoMa = _T("");

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
	else if (NiuNiu_None == m_gameKind) 
	{
		if(strNiuNiu.GetLength()<2)
		{
			CString strTips;
			strTips.Format(L"���ĺ��������������������룡");
			MyMessageBox(strTips);
			m_DlgStatus.HideStatusWindow();

			return;
		}
		for (int i = 0;i < strNiuNum.GetLength();i++)
		{
			strHaoMa += strNiuNum.GetAt(i);
			if(i%2 == 1&&i!=strNiuNum.GetLength()-1)
			{
				strHaoMa += _T(",");
			}
		}


		m_strHaoma.Empty();
		wstring strdata;
		strdata = strHaoMa.GetBuffer(0);

		vecAllNums.insert(vecAllNums.begin(),1,strdata);

		strHaoMa = _T("");
		for (int i = 0;i < strNiuNiu.GetLength();i++)
		{
			strHaoMa += strNiuNiu.GetAt(i);
			if(i%2 == 1&&i!=strNiuNiu.GetLength()-1)
			{
				strHaoMa += _T(",");
			}
		}


		zhushu = strNiuNiu.GetLength()/2;




		m_listChqSSC.InsertItem(0, strHaoMa);
		m_listChqSSC.SetItemText(0, 1, GetWanDesc());
		CString strTmp;
		strTmp.Format(_T("%d"), zhushu);
		m_listChqSSC.SetItemText(0, 2, strTmp);
		strTmp.Format(_T("%d"), m_beishu);
		m_listChqSSC.SetItemText(0, 3, strTmp);

		m_zongZhuShu += zhushu;
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

		strHaoMa = _T("");

		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=200;
		InvalidateRect(&rcInvali);
		InvalidateRect(&rcZongJinE);
		m_DlgStatus.HideStatusWindow();
		ResetAllNums();
		return;
	}
	else if (QianSanZhiXuan_FuShi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,3,3))
			{
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				m_DlgStatus.HideStatusWindow();

				return;
			}
// 			int nLength = strHaoMa.GetLength();
// 			if(nLength > 4800)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 
// 				MyMessageBox(L"������ĺ��볬������ע�������������룡");
// 				return;
// 			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;
		}
		else
		{

			if(strWan.IsEmpty() || strQian.IsEmpty() || strBai.IsEmpty()) 
			{
				m_DlgStatus.HideStatusWindow();

				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			strHaoMa = strWan + _T(",") + strQian + _T(",") + strBai;

			zhushu = strWan.GetLength() * strQian.GetLength() * strBai.GetLength();
			if(zhushu>800)
			{
				CString strTips;
				strTips.Format(L"����ֱѡ���Ͷ800ע��");
				MyMessageBox(strTips);
				return;
			}

		}
	}
	else if (WuxingZhixuan == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,5,5))
			{
				m_DlgStatus.HideStatusWindow();

				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

// 			int nLength = strHaoMa.GetLength();
// 			if(nLength > 480000)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"������ĺ��볬������ע�������������룡");
// 				return;
// 			}
// 
		}
		else
		{
			if(strWan.IsEmpty() || strQian.IsEmpty() || strBai.IsEmpty() ||strShi.IsEmpty()||strGe.IsEmpty()) 
			{
				m_DlgStatus.HideStatusWindow();

				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			strHaoMa = strWan + _T(",") + strQian + _T(",") + strBai + _T(",") + strShi  + _T(",") + strGe;

			zhushu = strWan.GetLength() * strQian.GetLength() * strBai.GetLength() * strShi.GetLength() * strGe.GetLength();
			int nZhushuXinzhi = 0;
// 			if (m_TypeID == CZ_FENFEN_CAI||m_TypeID == CZ_ErFenCai)
// 			{
// 				nZhushuXinzhi = 10000;
// 			}
// 			else
			{
				nZhushuXinzhi = 80000;
			}
			if(zhushu>nZhushuXinzhi)
			{
				CString strTips;
				strTips.Format(L"����ֱѡ���Ͷ%dע��",nZhushuXinzhi);
				MyMessageBox(strTips);
				return;
			}


		}
	}
	else if (WuxingQianSi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,4,4))
			{
				m_DlgStatus.HideStatusWindow();

				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}

// 			int nLength = strHaoMa.GetLength();
// 
// 			if(nLength > 64000)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 
// 				MyMessageBox(L"������ĺ��볬������ע�������������룡");
// 				return;
// 
// 			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;

		}
		else
		{
			if(strWan.IsEmpty() || strQian.IsEmpty() || strBai.IsEmpty() ||strShi.IsEmpty()) 
			{
				m_DlgStatus.HideStatusWindow();

				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			strHaoMa = strWan + _T(",") + strQian + _T(",") + strBai + _T(",") + strShi;

			zhushu = strWan.GetLength() * strQian.GetLength() * strBai.GetLength() * strShi.GetLength() ;
			if(zhushu>8000)
			{
				CString strTips;
				strTips.Format(L"����ֱѡ���Ͷ8000ע��");
				MyMessageBox(strTips);
				return;
			}

		}
	}
	else if (WuxingHousi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,4,4))
			{
				m_DlgStatus.HideStatusWindow();

				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}
// 			int nLength = strHaoMa.GetLength();
// 			if(nLength > 64000)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 
// //				MyMessageBox(L"������ĺ��볬������ע�������������룡");
// 				MyMessageBox(L"������ĺ��볬������ע�������������룡");
// 				return;
// 
// 			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;

		}
		else
		{
			if( strQian.IsEmpty() || strBai.IsEmpty() ||strShi.IsEmpty()||strGe.IsEmpty()) 
			{
				m_DlgStatus.HideStatusWindow();

				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			strHaoMa =  strQian + _T(",") + strBai + _T(",") + strShi  + _T(",") + strGe;

			zhushu =  strQian.GetLength() * strBai.GetLength() * strShi.GetLength() * strGe.GetLength();
			if(zhushu>8000)
			{
				CString strTips;
				strTips.Format(L"����ֱѡ���Ͷ8000ע��");
				MyMessageBox(strTips);
				return;
			}

		}
	}
	else if (WuxingZhongsan == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,3,3))
			{
				m_DlgStatus.HideStatusWindow();

				CString strTips;
//				strTips.Format(L"���ĺ��������������������룡");
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}
// 			int nLength = strHaoMa.GetLength();
// 
// 			if(nLength > 4800)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 
// 				MyMessageBox(L"������ĺ��볬������ע�������������룡");
// 				return;
// 			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;

		}
		else
		{
			if( strQian.IsEmpty() || strBai.IsEmpty() ||strShi.IsEmpty()) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			strHaoMa =  strQian + _T(",") + strBai + _T(",") + strShi ;

			zhushu =  strQian.GetLength() * strBai.GetLength() * strShi.GetLength() ;

			if(zhushu > 800)
			{
				MyMessageBox(L"����ֱѡ���Ͷ800ע��");
				return;

			}

		}
	}
	else if (QianSanZhiXuan_DanShi == m_gameKind) 
	{
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();

			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
	}
	else if (HouSanZuXuan_ZuSanDanShi == m_gameKind)
	{
		if(!GetKeyBoardHaoma(strHaoMa,10,3))
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
	else if (HouSanZuXuan_ZuSanFuShi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,10,3))
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
// 			if(zhushu > 72)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ72ע��"));
// 				return;
// 
// 			}

		}
		else
		{
			if (strBai.GetLength() < 2) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			//strHaoMa = strBai;
			int nLength = strBai.GetLength();
			for (int i = 0;i < nLength;i++)
			{
				strHaoMa+=strBai.GetAt(i);
				if(i!=nLength-1)
				{
					strHaoMa+=_T(",");
				}
			}

			zhushu = strBai.GetLength() * (strBai.GetLength() - 1);
// 			if(zhushu > 72)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ72ע��"));
// 				return;
// 
// 			}

		}
	}
	else if (BuDingWei_QianEr == m_gameKind) 
	{
		if (strBai.GetLength() < 1) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}

		zhushu = strBai.GetLength() ;
// 		if(zhushu>8)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"���ǲ���λ���Ͷ8ע��");
// 			return;
// 
// 
// 		}

	}
	else if (BuDingWei_HouEr == m_gameKind) 
	{
		if (strBai.GetLength() < 1) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}


		zhushu = strBai.GetLength() ;
// 		if(zhushu>8)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"���ǲ���λ���Ͷ8ע��");
// 			return;
// 
// 
// 		}

	}
	else if (QianSanZhiXuanHeZhi == m_gameKind) 
	{
		if (strHezhi.GetLength() < 1) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ���ֵ����"));
			return;
		}

		
		zhushu=0;

		int nCount = strHezhi.GetLength();

		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strHezhi[i];
			if(i%2 == 1 && i !=nCount-1)
			{
				strHaoMa+=_T(",");
			}
		}
		int nHezhi = 0;

		CString str;
		for(int i = 0;i < nCount;i+=2)
		{
			str = strHezhi.GetAt(i);
			str+=strHezhi.GetAt(i+1);
			nHezhi = _ttoi(str);
			for(int m = 0;m < 10;m++)
			{
				for(int n = 0;n < 10;n++)
				{
					for (int o = 0;o < 10;o++)
					{
						if((m+n+o) == nHezhi)
						{
							zhushu++;
						}
					}
				}
			}
		}
// 		if(zhushu>800)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"���Ǻ�ֵ���Ͷ800ע��");
// 			return;
// 
// 
// 		}

	}
	else if (ZhongSanZhiXuanHeZhi == m_gameKind) 
	{
		if (strHezhi.GetLength() < 1) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ���ֵ����"));
			return;
		}


		zhushu=0;

		int nCount = strHezhi.GetLength();

		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strHezhi[i];
			if(i%2 == 1 && i !=nCount-1)
			{
				strHaoMa+=_T(",");
			}
		}
		int nHezhi = 0;

		CString str;
		for(int i = 0;i < nCount;i+=2)
		{
			str = strHezhi.GetAt(i);
			str+=strHezhi.GetAt(i+1);
			nHezhi = _ttoi(str);
			for(int m = 0;m < 10;m++)
			{
				for(int n = 0;n < 10;n++)
				{
					for (int o = 0;o < 10;o++)
					{
						if((m+n+o) == nHezhi)
						{
							zhushu++;
						}
					}
				}
			}
		}
// 		if(zhushu>800)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"���Ǻ�ֵ���Ͷ800ע��");
// 			return;
// 
// 
// 		}

	}
	else if (HouSanZhiXuanHeZhi == m_gameKind) 
	{
		if (strHezhi.GetLength() < 1) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ���ֵ����"));
			return;
		}


		zhushu=0;
		int nCount = strHezhi.GetLength();
		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strHezhi[i];
			if(i%2 == 1 && i !=nCount-1)
			{
				strHaoMa+=_T(",");
			}
		}

		int nHezhi = 0;

		CString str;
		for(int i = 0;i < nCount;i+=2)
		{
			str = strHezhi.GetAt(i);
			str+=strHezhi.GetAt(i+1);
			nHezhi = _ttoi(str);
			for(int m = 0;m < 10;m++)
			{
				for(int n = 0;n < 10;n++)
				{
					for (int o = 0;o < 10;o++)
					{
						if((m+n+o) == nHezhi)
						{
							zhushu++;
						}
					}
				}
			}
		}
// 		if(zhushu>800)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"���Ǻ�ֵ���Ͷ800ע��");
// 			return;
// 
// 
// 		}

	}
	else if (HouErZhixuan_Hezhi == m_gameKind || m_gameKind == QianErZhixuan_Hezhi) 
	{
		if (strHezhi.GetLength() < 1) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ���ֵ����"));
			return;
		}


		zhushu=0;
		int nCount = strHezhi.GetLength();
		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strHezhi[i];
			if(i%2 == 1 && i !=nCount-1)
			{
				strHaoMa+=_T(",");
			}
		}

		int nHezhi = 0;

		CString str;
		for(int i = 0;i < nCount;i+=2)
		{
			str = strHezhi.GetAt(i);
			str+=strHezhi.GetAt(i+1);
			nHezhi = _ttoi(str);
			for(int m = 0;m < 10;m++)
			{
				for(int n = 0;n < 10;n++)
				{
					if((m+n) == nHezhi)
					{
						zhushu++;
					}
				}
			}
		}
// 		if(zhushu>80)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"���Ǻ�ֵ���Ͷ80ע��");
// 			return;
// 
// 
// 		}

	}
	else if (HouSanZuXuan_ZuSanDanShi == m_gameKind) 
	{
		if (!GetZuSanHaoma(strHaoMa, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
	}	
	else if (HouSanZuXuan_ZuLiuFuShi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,10,3))
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
// 			if(zhushu > 96)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ96ע��"));
// 				return;
// 
// 			}

		}
		else
		{
			if (strBai.GetLength() < 3) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			int nLength = strBai.GetLength();
			for (int i = 0;i < nLength;i++)
			{
				strHaoMa+=strBai.GetAt(i);
				if(i!=nLength-1)
				{
					strHaoMa+=_T(",");
				}
			}

			zhushu = strBai.GetLength() * (strBai.GetLength() - 1) * (strBai.GetLength() - 2) / 6;
// 			if(zhushu > 96)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ96ע��"));
// 				return;
// 
// 			}

		}
	}
	else if (HouSanZuXuan_ZuLiuDanShi == m_gameKind) 
	{
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
	}
	else if (QianSanZuXuan_ZuSanDanShi == m_gameKind) 
	{
		if(!GetKeyBoardHaoma(strHaoMa,10,3))
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
	else if (QianSanZuXuan_ZuSanFuShi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,10,3))
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

// 			if(zhushu > 72)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ72ע��"));
// 				return;
// 
// 			}
// 
		}
		else
		{

			if (strBai.GetLength() < 2) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			int nLength = strBai.GetLength();
			for(int i = 0;i < nLength;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != nLength-1)
				{
					strHaoMa+=_T(",");
				}
			}

			zhushu = strBai.GetLength() * (strBai.GetLength() - 1);
// 			if(zhushu > 72)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ72ע��"));
// 				return;
// 
// 			}

		}
	}
	else if (Zhongsanzusandan == m_gameKind) 
	{
		if(!GetKeyBoardHaoma(strHaoMa,10,3))
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
	else if (ZhongsanZusan == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,10,3))
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
// 			if(zhushu > 72)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ72ע��"));
// 				return;
// 
// 			}

		}
		else
		{
			if (strBai.GetLength() < 2) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			int nLength = strBai.GetLength();
			for(int i = 0;i < nLength;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != nLength-1)
				{
					strHaoMa+=_T(",");
				}
			}

			zhushu = strBai.GetLength() * (strBai.GetLength() - 1);
// 			if(zhushu > 72)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ72ע��"));
// 				return;
// 
// 			}

		}
	}
	else if (QianSanZuXuan_ZuSanDanShi == m_gameKind) 
	{
		if (!GetZuSanHaoma(strHaoMa, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
	}	
	else if(QW_YiFanFengShun == m_gameKind)
	{
		if(strBai.GetLength()<1)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}

		zhushu = strBai.GetLength();
// 		if(zhushu > 8)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(_T("һ����˳���Ͷ8ע��"));
// 			return;
// 
// 		}
	}
	else if(QW_HaoShiChengShuang == m_gameKind)
	{
		if(strBai.GetLength()<1)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}

		zhushu = strBai.GetLength();
// 		if(zhushu > 8)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(_T("���³�˫���Ͷ8ע��"));
// 			return;
// 
// 		}

	}
	else if(Zuxuan120 == m_gameKind)
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,10,5))
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
			if(strBai.GetLength()<5)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;
			}


			int nCount = strBai.GetLength();
			for (int i = 0;i < nCount;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i!=nCount-1)
				{
					strHaoMa+=_T(",");
				}
			}
			zhushu = (nCount-4)*(nCount-3)*(nCount-2)*(nCount-1)*nCount/120;
		}
	}
	else if(Zuxuan60 == m_gameKind)
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,5,5))
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
			if(strBai.GetLength()<1 || strShi.GetLength()<3)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;
			}

			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();
			CString strTempBai,strTempShi;

			zhushu = 0;
			int nChongfu = 0;
// 			for (int i = 0;i < nCountBai;i++)
// 			{
// 				strTempBai = strBai.GetAt(i);
// 				for (int j = 0;j < nCountShi;j++)
// 				{
// 					strTempShi = strShi.GetAt(j);
// 					if(strTempBai == strTempShi)
// 					{
// 						nChongfu++;
// 						//if((nCountShi-1)<3)
// 						if((nCountShi == nChongfu) || (nCountBai == nChongfu && nCountBai != 1))
// 						{
// 							m_DlgStatus.HideStatusWindow();
// 							MyMessageBox(_T("ѡ������"));
// 							return;
// 						}
// 					}
// 				}
// 			}
			nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
						//if((nCountShi-1)<3)
						if((nCountShi == nChongfu) || (nCountBai == nChongfu && nCountBai != 1))
						{
							m_DlgStatus.HideStatusWindow();
							MyMessageBox(_T("ѡ������"));
							return;
						}
					}
				}

				int nFenzi = 1;
				int nFenmu = 1;
				for(int m = (nCountShi - nChongfu);m >0;m-- )
				{
					nFenzi *= m;
				}
				for(int m = (nCountShi - nChongfu-3);m > 0;m--)
				{
					nFenmu *= m;
				}

				nFenmu*=6;
				zhushu += nFenzi/nFenmu;

				nChongfu=0;
			}
// 			if(zhushu>672)
// 			{
// 				CString strTips;
// 				strTips.Format(L"��ѡ60���Ͷ672ע��");
// 				MyMessageBox(strTips);
// 				return;
// 			}

			
			for (int i = 0;i < nCountBai;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i!=nCountBai-1)
				{
					strHaoMa+=_T(",");
				}
			}

			strHaoMa +=_T(";");
			for (int i = 0;i < nCountShi;i++)
			{
				strHaoMa += strShi.GetAt(i);
				if(i!=nCountShi-1)
				{
					strHaoMa+=_T(",");
				}
			}

		}
	}
	else if(Zuxuan30 == m_gameKind)
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,5,5))
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
			if(strBai.GetLength()<2 || strShi.GetLength()<1)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;
			}

			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();
			CString strTempBai,strTempShi;

			zhushu = 0;
			int nChongfu = 0;
			for (int i = 0;i < nCountShi;i++)
			{
				strTempShi = strShi.GetAt(i);
				for (int j = 0;j < nCountBai;j++)
				{
					strTempBai = strBai.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
						if((nCountBai == nChongfu)||(nCountShi == nChongfu))
						{
							m_DlgStatus.HideStatusWindow();
							MyMessageBox(_T("ѡ������"));
							return;
						}
					}
				}

				int nFenzi = 1;
				int nFenmu = 1;
				for(int m = (nCountBai - nChongfu);m >0;m-- )
				{
					nFenzi *= m;
				}
				for(int m = (nCountBai - nChongfu-2);m > 0;m--)
				{
					nFenmu *= m;
				}

				nFenmu*=2;
				zhushu += nFenzi/nFenmu;
				nChongfu=0;
			}
			if(zhushu == 0)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;

			}

// 			if(zhushu>288)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				CString strTips;
// 				strTips.Format(L"��ѡ30���Ͷ288ע��");
// 				MyMessageBox(strTips);
// 				return;
// 
// 			}

			for (int i = 0;i < nCountBai;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i!=nCountBai-1)
				{
					strHaoMa+=_T(",");
				}
			}

			strHaoMa +=_T(";");
			for (int i = 0;i < nCountShi;i++)
			{
				strHaoMa += strShi.GetAt(i);
				if(i!=nCountShi-1)
				{
					strHaoMa+=_T(",");
				}
			}

		}
	}
	else if(Zuxuan20 == m_gameKind)
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,5,5))
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
			if(strBai.GetLength()<1 || strShi.GetLength()<2)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;
			}

			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();
			CString strTempBai,strTempShi;

			zhushu = 0;
			int nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
						if((strBai == strShi)&&(nCountBai == 2)&&(nCountShi == 2))
						{
							m_DlgStatus.HideStatusWindow();
							MyMessageBox(_T("ѡ������"));
							return;
						}
					} 
				}

				int nFenzi = 1;
				int nFenmu = 1;
				nFenzi = (nCountShi-nChongfu)*(nCountShi-nChongfu-1);

				nFenmu*=2;
				zhushu += nFenzi/nFenmu;
				nChongfu=0;
			}

// 			if(zhushu>288)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				CString strTips;
// 				strTips.Format(L"��ѡ20���Ͷ288ע��");
// 				MyMessageBox(strTips);
// 				return;
// 
// 			}

			for (int i = 0;i < nCountBai;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i!=nCountBai-1)
				{
					strHaoMa+=_T(",");
				}
			}

			strHaoMa +=_T(";");
			for (int i = 0;i < nCountShi;i++)
			{
				strHaoMa += strShi.GetAt(i);
				if(i!=nCountShi-1)
				{
					strHaoMa+=_T(",");
				}
			}
		}
	}
	else if(Zuxuan10 == m_gameKind)
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,5,5))
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
			if(strBai.GetLength()<1 || strShi.GetLength()<1)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;
			}
			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();

			if(nCountShi == 1 && strShi == strBai)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;
			}

			CString strTempBai,strTempShi;

			zhushu = 0;
			int nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
					}
				}
				zhushu+= (nCountShi-nChongfu);
				nChongfu = 0;
			}

// 			if(zhushu>72)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				CString strTips;
// 				strTips.Format(L"��ѡ10���Ͷ72ע��");
// 				MyMessageBox(strTips);
// 				return;
// 
// 			}

			for (int i = 0;i < nCountBai;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i!=nCountBai-1)
				{
					strHaoMa+=_T(",");
				}
			}

			strHaoMa +=_T(";");
			for (int i = 0;i < nCountShi;i++)
			{
				strHaoMa += strShi.GetAt(i);
				if(i!=nCountShi-1)
				{
					strHaoMa+=_T(",");
				}
			}

		}
	}
	else if(Zuxuan5 == m_gameKind)
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,5,5))
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
			if(strBai.GetLength()<1 || strShi.GetLength()<1)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;
			}
			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();

			if(nCountShi == 1 && strShi == strBai)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;
			}
			CString strTempBai,strTempShi;

			zhushu = 0;
			int nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
					}
				}
				zhushu+= (nCountShi-nChongfu);
				nChongfu = 0;
			}

// 			if(zhushu>72)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				CString strTips;
// 				strTips.Format(L"��ѡ5���Ͷ72ע��");
// 				MyMessageBox(strTips);
// 				return;
// 
// 			}
// 
			for (int i = 0;i < nCountBai;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i!=nCountBai-1)
				{
					strHaoMa+=_T(",");
				}
			}

			strHaoMa +=_T(";");
			for (int i = 0;i < nCountShi;i++)
			{
				strHaoMa += strShi.GetAt(i);
				if(i!=nCountShi-1)
				{
					strHaoMa+=_T(",");
				}
			}


		}
	}
	else if(QSZuXuan24 == m_gameKind||HSZuXuan24 == m_gameKind)
	{
		if(strBai.GetLength()<4)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("ѡ������"));
			return;
		}


		int nCount = strBai.GetLength();
		for (int i = 0;i < nCount;i++)
		{
			strHaoMa += strBai.GetAt(i);
			if(i!=nCount-1)
			{
				strHaoMa+=_T(",");
			}
		}
		zhushu = (nCount-3)*(nCount-2)*(nCount-1)*nCount/24;
	}
	else if(QSZuXuan12 == m_gameKind||HSZuXuan12 == m_gameKind)
	{
			if(strBai.GetLength()<1 || strShi.GetLength()<2)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("ѡ������"));
				return;
			}

			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();
			CString strTempBai,strTempShi;

			zhushu = 0;
			int nChongfu = 0;

			nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
						if((nCountShi == nChongfu) || (nCountBai == nChongfu && nCountBai != 1))
						{
							m_DlgStatus.HideStatusWindow();
							MyMessageBox(_T("ѡ������"));
							return;
						}
					}
				}

				int nFenzi = 1;
				int nFenmu = 1;
				for(int m = (nCountShi - nChongfu);m >0;m-- )
				{
					nFenzi *= m;
				}
				for(int m = (nCountShi - nChongfu-2);m > 0;m--)
				{
					nFenmu *= m;
				}

				nFenmu*=2;
				zhushu += nFenzi/nFenmu;

				nChongfu=0;
			}


			for (int i = 0;i < nCountBai;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i!=nCountBai-1)
				{
					strHaoMa+=_T(",");
				}
			}

			strHaoMa +=_T(";");
			for (int i = 0;i < nCountShi;i++)
			{
				strHaoMa += strShi.GetAt(i);
				if(i!=nCountShi-1)
				{
					strHaoMa+=_T(",");
				}
			}

	}
	else if(QSZuXuan6 == m_gameKind||HSZuXuan6 == m_gameKind)
	{
		if(strBai.GetLength()<2)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("ѡ������"));
			return;
		}


		int nCount = strBai.GetLength();
		for (int i = 0;i < nCount;i++)
		{
			strHaoMa += strBai.GetAt(i);
			if(i!=nCount-1)
			{
				strHaoMa+=_T(",");
			}
		}
		zhushu =(nCount-1)*nCount/2;
	}
	else if(QSZuXuan4 == m_gameKind||HSZuXuan4 == m_gameKind)
	{
		if(strBai.GetLength()<1 || strShi.GetLength()<1)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("ѡ������"));
			return;
		}
		int nCountBai = strBai.GetLength();
		int nCountShi = strShi.GetLength();

		if(nCountShi == 1 && strShi == strBai)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("ѡ������"));
			return;
		}
		CString strTempBai,strTempShi;

		zhushu = 0;
		int nChongfu = 0;
		for (int i = 0;i < nCountBai;i++)
		{
			strTempBai = strBai.GetAt(i);
			for (int j = 0;j < nCountShi;j++)
			{
				strTempShi = strShi.GetAt(j);
				if(strTempBai == strTempShi)
				{
					nChongfu++;
				}
			}
			zhushu+= (nCountShi-nChongfu);
			nChongfu = 0;
		}

		for (int i = 0;i < nCountBai;i++)
		{
			strHaoMa += strBai.GetAt(i);
			if(i!=nCountBai-1)
			{
				strHaoMa+=_T(",");
			}
		}

		strHaoMa +=_T(";");
		for (int i = 0;i < nCountShi;i++)
		{
			strHaoMa += strShi.GetAt(i);
			if(i!=nCountShi-1)
			{
				strHaoMa+=_T(",");
			}
		}

	}
	else if(QW_SanXingBaoXi == m_gameKind)
	{
		if(strBai.GetLength()<1)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}

		zhushu = strBai.GetLength();
// 		if(zhushu > 8)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(_T("���Ǳ�ϲ���Ͷ8ע��"));
// 			return;
// 
// 		}

	}
	else if(QW_SiJiFaCai == m_gameKind)
	{
		if(strBai.GetLength()<1)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}

		zhushu = strBai.GetLength();
// 		if(zhushu > 8)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(_T("�ļ��������Ͷ8ע��"));
// 			return;
// 
// 		}
// 
	}
	else if(SSC_LongHu == m_gameKind)
	{
		CString strLhHaoma = GetLongHu();
		if(strLhHaoma.IsEmpty())
		{
			m_DlgStatus.HideStatusWindow();

			MyMessageBox(L"��ѡ����룡");
			return;
		}
		else if(strLhHaoma.GetLength()>1)
		{
			m_DlgStatus.HideStatusWindow();

			MyMessageBox(L"������ֻ��ѡһ�����룡");
			return;
		}

		zhushu = strLhHaoma.GetLength();

		CString str[10] = {_T("11000"),_T("10100"),_T("10010"),_T("10001"),_T("01100"),_T("01010"),_T("01001"),_T("00110"),_T("00101"),_T("00011")};

		if(m_nLhdID<10)
			strHaoMa.Format(L"%s,%s",strLhHaoma,str[m_nLhdID]);
		else
		{
			m_DlgStatus.HideStatusWindow();
			CString strTips;
			strTips.Format(L"�淨����");
			MyMessageBox(strTips);
			return;

		}

	}
	else if (QianSanZuXuan_ZuLiuFuShi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,10,3))
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
// 			if(zhushu > 96)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ96ע��"));
// 				return;
// 
// 			}
		}
		else
		{
			if (strBai.GetLength() < 3) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			int nBaiLength = strBai.GetLength();
			for(int i = 0;i < nBaiLength;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != nBaiLength-1)
				{
					strHaoMa+=_T(",");
				}
			}

			zhushu = strBai.GetLength() * (strBai.GetLength() - 1) * (strBai.GetLength() - 2) / 6;
// 			if(zhushu > 96)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ96ע��"));
// 				return;
// 
// 			}
		}
	}
	else if (ZhongsanZuliu == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,10,3))
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
// 			if(zhushu > 96)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ96ע��"));
// 				return;
// 
// 			}

		}
		else
		{
			if (strBai.GetLength() < 3) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

 			int nBaiLength = strBai.GetLength();
			for(int i = 0;i < nBaiLength;i++)
			{
				strHaoMa += strBai.GetAt(i);
				if(i != nBaiLength-1)
				{
					strHaoMa+=_T(",");
				}
			}


			zhushu = strBai.GetLength() * (strBai.GetLength() - 1) * (strBai.GetLength() - 2) / 6;
// 			if(zhushu > 96)
// 			{
// 				MyMessageBox(_T("��ѡ�����Ͷ96ע��"));
// 				return;
// 
// 			}

		}
	}
	else if (QianSanZuXuan_ZuLiuDanShi == m_gameKind) 
	{
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
	}
	else if (WuXingHouEr_FuShi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,2,2))
			{
				CString strTips;
				m_DlgStatus.HideStatusWindow();
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}
 			int nLength = strHaoMa.GetLength();
			if(nLength > 320)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"������ĺ��볬������ע�������������룡");
				return;

			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;
			if(zhushu>80)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ֱѡ���Ͷ80ע��");
				return;


			}


		}
		else
		{
			if(strShi.IsEmpty() || strGe.IsEmpty()) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			strHaoMa = strShi + _T(",") + strGe;

			zhushu = strShi.GetLength() * strGe.GetLength();
			if(zhushu>80)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ֱѡ���Ͷ80ע��");
				return;


			}

		}
	}
	else if (WuXingHouEr_DanShi == m_gameKind) 
	{
		if (!GetDanShiHaoma(strHaoMa, 2, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
		if(zhushu>80)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(L"����ֱѡ���Ͷ80ע��");
			return;


		}

	}
	else if (WuXingQianEr_FuShi == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,2,2))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"���ĺ��������������������룡");
				MyMessageBox(strTips);
				return;
			}
 			int nLength = strHaoMa.GetLength();
			if(nLength > 320)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"������ĺ��볬������ע�������������룡");
				return;

			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;

			if(zhushu>80)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ֱѡ���Ͷ80ע��");
				return;


			}

		}
		else
		{
			if(strWan.IsEmpty() || strQian.IsEmpty()) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ����ʵĺ���"));
				return;
			}

			strHaoMa = strWan + _T(",") + strQian;

			zhushu = strWan.GetLength() * strQian.GetLength();
			if(zhushu>80)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ֱѡ���Ͷ80ע��");
				return;


			}

		}
	}
	else if (WuXingQianEr_DanShi == m_gameKind) 
	{
		if (!GetDanShiHaoma(strHaoMa, 2, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
		if(zhushu>80)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(L"����ֱѡ���Ͷ80ע��");
			return;


		}

	}
	else if (HouErZuXuan_FuShi == m_gameKind) 
	{
		if (strBai.GetLength() < 2) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}


		zhushu = strBai.GetLength() * (strBai.GetLength() - 1) / 2;
// 		if(zhushu>36)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"������ѡ���Ͷ36ע��");
// 			return;
// 
// 
// 		}

	}
	else if (HouErZuXuan_DanShi == m_gameKind) 
	{
		if (!GetDanShiHaoma(strHaoMa, 2, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();
//			MyMessageBox(_T("��ѡ����ʵĺ���"));
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
// 		if(zhushu>36)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"������ѡ���Ͷ36ע��");
// 			return;
// 
// 
// 		}

	}
	else if (QianErZuXuan_FuShi == m_gameKind) 
	{
		if (strBai.GetLength() < 2) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("����д���ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}

		zhushu = strBai.GetLength() * (strBai.GetLength() - 1) / 2;
// 		if(zhushu>36)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"������ѡ���Ͷ36ע��");
// 			return;
// 
// 
// 		}

	}
	else if (QianErZuXuan_DanShi == m_gameKind) 
	{
		if (!GetDanShiHaoma(strHaoMa, 2, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
// 		if(zhushu>36)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"������ѡ���Ͷ36ע��");
// 			return;
// 
// 
// 		}

	}
	else if (DaXiaoDanShuang == m_gameKind) 
	{
		CString strShi = GetShiDxdshString();
		CString strGe = GetGeDxdshString();
		
		if (strShi.IsEmpty() || strGe.IsEmpty()) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		strHaoMa = strShi + _T(",") + strGe;

		zhushu = strShi.GetLength() * strGe.GetLength();
// 		if(zhushu>13)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"��С��˫���Ͷ13ע��");
// 			return;
// 
// 
// 		}

	}
	else if (DaXiaoDanShuang_dw == m_gameKind) 
	{
		CString strWan = GetWanDxdshString();
		CString strQian = GetQianDxdshString();
		CString strBai = GetBaiDxdshString();
		CString strShi = GetShiDxdshString();
		CString strGe = GetGeDxdshString();

		if (strWan.IsEmpty() && strQian.IsEmpty() && strBai.IsEmpty() && strShi.IsEmpty() && strGe.IsEmpty()) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
		if (strWan.GetLength()>1 || strQian.GetLength()>1 || strBai.GetLength()>1 || strShi.GetLength()>1 || strGe.GetLength()>1 ) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("������ѡ�񣬶�λ��С��˫ÿλֻ��ѡ��һ�����룡"));
			return;
		}

		strHaoMa = strWan + _T(",") + strQian + _T(",") + strBai + _T(",") + strShi + _T(",") + strGe;

		zhushu = strWan.GetLength() + strQian.GetLength() + strBai.GetLength() + strShi.GetLength() + strGe.GetLength();
	}
	else if(Zonghe_DXDSLh == m_gameKind)
	{
		CString strBai = GetBaiDxdshString();
		if(strBai.IsEmpty())
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}
		if(strBai.GetLength()!=1)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ֵ��С��˫ÿ��ֻ��ѡ��һ�����룡"));

			return;
		}
		strHaoMa = strBai;
		zhushu = 1;
	}	
	else if(RenXuan2 == m_gameKind)
	{
		//��ѡ2��������
		if(m_RenxuanKeyboardInput)
		{
			if (!GetDanShiHaoma(strHaoMa, 2, zhushu)) 
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"��ѡ������ѡ����λ�����ֺ��룺%s��", strHaoMa);
				MyMessageBox(strTips);
				return;
			}
			zhushu=1;

 			BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
			int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
			if(nCount < 2)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ȷ��ѡ��ѡλ��");
				return;
			}
// 			else if(nCount == 2)
// 			{
// 				int nLength = strHaoMa.GetLength();
// 				if(nLength >240)
// 				{
// 					m_DlgStatus.HideStatusWindow();
// 					MyMessageBox(L"������ĺ��볬���������ע�������������룡");
// 					return;
// 				}
// 			}
// 			else if(nCount == 3)
// 			{
// 				int nLength = strHaoMa.GetLength();
// 				if(nLength >720)
// 				{
// 					m_DlgStatus.HideStatusWindow();
// 					MyMessageBox(L"������ĺ���ֽ�󳬹��������ע�������������룡");
// 					return;
// 				}
// 			}
// 			else if(nCount == 4)
// 			{
// 				int nLength = strHaoMa.GetLength();
// 				if(nLength >1440)
// 				{
// 					m_DlgStatus.HideStatusWindow();
// 					MyMessageBox(L"������ĺ���ֽ�󳬹��������ע�������������룡");
// 					return;
// 				}
// 			}
// 			else if(nCount == 5)
// 			{
// 				int nLength = strHaoMa.GetLength();
// 				if(nLength >2400)
// 				{
// 					m_DlgStatus.HideStatusWindow();
// 					MyMessageBox(L"������ĺ���ֽ�󳬹��������ע�������������룡");
// 					return;
// 				}
// 			}
		}
		else
		{
			int nCount=0;
			if(!strWan.IsEmpty() )
				nCount++;
			if(!strQian.IsEmpty() )
				nCount++;
			if(! strBai.IsEmpty())
				nCount++;
			if(! strShi.IsEmpty() )
				nCount++;
			if(!strGe.IsEmpty())
				nCount++;
			if(nCount != 2)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ������Ϊ2λλ�������"));
				return;
			}
			
			Combo(5,4);
			
			strHaoMa = strWan + _T(",") + strQian + _T(",") +strBai + _T(",") +strShi + _T(",") +strGe;
			if(strWan.GetLength()>0)
				zhushu = zhushu * strWan.GetLength();
			if(strQian.GetLength() > 0)
				zhushu = zhushu * strQian.GetLength();
			if(strBai.GetLength() > 0)
				zhushu = zhushu * strBai.GetLength();
			if(strShi.GetLength() > 0)
				zhushu = zhushu * strShi.GetLength();
			if(strGe.GetLength() > 0)
				zhushu = zhushu * strGe.GetLength();

// 			if(zhushu > 80)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"��ѡ�����Ͷ80ע��");
// 				return;
// 
// 			}
		}
	}
	else if(Ren3Zu3_Fushi == m_gameKind)
	{
		if(m_RenxuanKeyboardInput)
		{
			BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
			int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
			int nBaiLen = 0;
			if(nCount < 3)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"��������ѡ���������������ϵ���λ!");
				return;
			}

			if(!GetKeyBoardHaoma(strHaoMa,3,3))
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����������ѡ����λ����������һ���غ�!");
				return ;
			}

			CString strTempHao;
			CString temp;
			int nZhu = 0;
			int nIndex =0 ;
			do 
			{
				if(!AfxExtractSubString(strTempHao, strHaoMa, nIndex++, '|'))
					break;
				if(strTempHao.IsEmpty())
					break;

				strTempHao.Replace(_T(","),_T(""));
				int nSingleZhu = 0;
				nBaiLen = strTempHao.GetLength()/2+1;
				if(nCount == 3)
					nSingleZhu = 1;
				else if(nCount == 4)
					nSingleZhu = 4;
				else if(nCount == 5)
					nSingleZhu =10;



				nZhu = nSingleZhu;
				CString str;
				str.Format(L"%d%d%d%d%d",bWan?1:0,bQian?1:0,bBai?1:0,bShi?1:0,bGe?1:0);
				strTempHao+=TEXT(",");
				strTempHao+=str;

				temp += strTempHao;
				temp += TEXT("|");
			} while (nIndex < 10000);


			zhushu = nZhu;
// 			if(zhushu>720)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�����������Ͷ720ע��");
// 				return;
// 			}
			strHaoMa = temp;

		}
		else
		{
			BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
			int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
			int nBaiLen = strBai.GetLength();
			if(nBaiLen<2)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ѡ���������룡");
				return;

			}

			if(nCount < 3)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ȷ��ѡ��ѡλ��");
				return;
			}
			else if(nCount == 3)
			{
				zhushu = nBaiLen* (nBaiLen - 1);
			}
			else if(nCount == 4)
			{
				zhushu = nCount*nBaiLen * (nBaiLen - 1);
			}
			else if(nCount == 5)
			{
				zhushu = nCount*nBaiLen * (nBaiLen - 1)*2;
			}
// 			if(zhushu>720)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�����������Ͷ720ע��");
// 				return;
// 			}

			CString str;
			str.Format(L"%d%d%d%d%d",bWan?1:0,bQian?1:0,bBai?1:0,bShi?1:0,bGe?1:0);
			strHaoMa = strBai+TEXT(",")+str;

		}

	}
	else if(Ren2Zuxuan_Fushi == m_gameKind)
	{
		if(m_RenxuanKeyboardInput)
		{
			BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
			int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
			int nBaiLen = 0;
			if(nCount < 2)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"�ζ�����ѡ���������������ϵ���λ!");
				return;
			}

			if(!GetKeyBoardHaoma(strHaoMa,2,2))
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"�ζ���ѡ����λ��ͬ������!");
				return ;
			}

			CString strTempHao;
			CString temp;
			int nZhu = 0;
			int nIndex =0 ;
			do 
			{
				if(!AfxExtractSubString(strTempHao, strHaoMa, nIndex++, '|'))
					break;
				if(strTempHao.IsEmpty())
					break;

				strTempHao.Replace(_T(","),_T(""));
				int nSingleZhu = 0;
				nBaiLen = strTempHao.GetLength()/2+1;
				if(nCount == 2)
					nSingleZhu = 1;
				else if(nCount == 3)
					nSingleZhu = 3;
				else if(nCount == 4)
					nSingleZhu = 6;
				else if(nCount == 5)
					nSingleZhu = 10;


				nZhu = nSingleZhu;
				CString str;
				str.Format(L"%d%d%d%d%d",bWan?1:0,bQian?1:0,bBai?1:0,bShi?1:0,bGe?1:0);
				strTempHao+=TEXT(",");
				strTempHao+=str;

				temp += strTempHao;
				temp += TEXT("|");
			} while (nIndex < 10000);


			zhushu = nZhu;
// 			if(zhushu > 36&&nCount == 2)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�ζ���ѡ����λ�����Ͷ36ע!");
// 				return ;
// 
// 			}
// 			else if(zhushu > 108&&nCount == 3)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�ζ���ѡ����λ�����Ͷ108ע!");
// 				return ;
// 
// 			}
// 			else if(zhushu > 216&&nCount == 4)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�ζ���ѡ�ĸ�λ�����Ͷ216ע!");
// 				return ;
// 
// 			}
// 			else if(zhushu > 360&&nCount == 5)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�ζ���ѡ���λ�����Ͷ360ע!");
// 				return ;
// 
// 			}

			strHaoMa = temp;

		}
		else
		{
			BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
			int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
			int nBaiLen = strBai.GetLength();
			if(nBaiLen<2)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ѡ���������룡");
				return;

			}
			if(nCount < 2)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ȷ��ѡ��ѡλ��");
				return;
			}
			else if(nCount == 2)
			{
				zhushu = nBaiLen * (nBaiLen - 1) / 2;
			}
			else if(nCount == 3)
			{
				zhushu = nCount*nBaiLen * (nBaiLen - 1) / 2;
			}
			else if(nCount == 4)
			{
				zhushu = (nCount-1)*(nCount-2)*nBaiLen * (nBaiLen - 1) / 2;
			}
			else if(nCount == 5)
			{
				zhushu = nCount*nBaiLen * (nBaiLen - 1) ;
			}
// 			if(zhushu > 36&&nCount == 2)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�ζ���ѡ����λ�����Ͷ36ע!");
// 				return ;
// 
// 			}
// 			else if(zhushu > 108&&nCount == 3)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�ζ���ѡ����λ�����Ͷ108ע!");
// 				return ;
// 
// 			}
// 			else if(zhushu > 216&&nCount == 4)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�ζ���ѡ�ĸ�λ�����Ͷ216ע!");
// 				return ;
// 
// 			}
// 			else if(zhushu > 360&&nCount == 5)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�ζ���ѡ���λ�����Ͷ360ע!");
// 				return ;
// 
// 			}
// 
			CString str;
			str.Format(L"%d%d%d%d%d",bWan?1:0,bQian?1:0,bBai?1:0,bShi?1:0,bGe?1:0);
			strHaoMa = strBai+TEXT(",")+str;

		}

	}
	else if(Ren3Zu6_Fushi == m_gameKind)
	{
		if(m_RenxuanKeyboardInput)
		{
			BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
			int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
			int nBaiLen = 0;
			if(nCount < 3)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"��������ѡ���������������ϵ���λ!");
				return;
			}

			if(!GetKeyBoardHaoma(strHaoMa,3,3))
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����������ѡ����λ�������Ҳ������غ�!");
				return ;
			}

			CString strTempHao;
			CString temp;
			int nZhu = 0;
			int nIndex =0 ;
			do 
			{
				if(!AfxExtractSubString(strTempHao, strHaoMa, nIndex++, '|'))
					break;
				if(strTempHao.IsEmpty())
					break;

				strTempHao.Replace(_T(","),_T(""));
				int nSingleZhu = 0;
				nBaiLen = strTempHao.GetLength()/2+1;
				if(nCount == 3)
					nSingleZhu = 1;
				else if(nCount == 4)
					nSingleZhu = 4;
				else if(nCount == 5)
					nSingleZhu =10;


				nZhu = nSingleZhu;
				CString str;
				str.Format(L"%d%d%d%d%d",bWan?1:0,bQian?1:0,bBai?1:0,bShi?1:0,bGe?1:0);
				strTempHao+=TEXT(",");
				strTempHao+=str;

				temp += strTempHao;
				temp += TEXT("|");
			} while (nIndex < 10000);


			zhushu = nZhu;
// 			if(zhushu>960)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�����������Ͷ960ע��");
// 				return;
// 			}

			strHaoMa = temp;

		}
		else
		{
			BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
			int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
			int nBaiLen = strBai.GetLength();
			if(nBaiLen<3)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ѡ���������룡");
				return;

			}
			if(nCount < 3)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ȷ��ѡ��ѡλ��");
				return;
			}
			else if(nCount == 3)
			{
				zhushu = nBaiLen* (nBaiLen - 1) * (nBaiLen - 2) / 6;
			}
			else if(nCount == 4)
			{
				zhushu = nCount*nBaiLen * (nBaiLen - 1) * (nBaiLen - 2) / 6;
			}
			else if(nCount == 5)
			{
				zhushu = nCount*nBaiLen * (nBaiLen - 1)*2 * (nBaiLen - 2) / 6;
			}
// 			if(zhushu>960)
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				MyMessageBox(L"�����������Ͷ960ע��");
// 				return;
// 			}


			CString str;
			str.Format(L"%d%d%d%d%d",bWan?1:0,bQian?1:0,bBai?1:0,bShi?1:0,bGe?1:0);
			strHaoMa = strBai+TEXT(",")+str;

		}

	}
	else if(Ren3Hunhe_Zuxuan == m_gameKind)
	{
		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
		int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
		if(nCount < 3)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(L"����ȷ��ѡ��ѡλ��");
			return;
		}
		
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu)) 
		{
			m_DlgStatus.HideStatusWindow();
			CString strTips;
			strTips.Format(L"������ϣ���ѡ��3λ�����ֺ��룺%s��", strHaoMa);
			MyMessageBox(strTips);
			return;
		}

		CString strTempHao;
		CString temp;
		int nIndex =0 ;
		do 
		{
			if(!AfxExtractSubString(strTempHao, strHaoMa, nIndex++, '|'))
				break;
			if(strTempHao.IsEmpty())
				break;

			if(nCount == 3)
			{
				zhushu = 1;
			}
			else if(nCount == 4)
			{
				zhushu = 4;
			}
			else if(nCount == 5)
			{
				zhushu = 10;
			}
			CString str;
			str.Format(L"%d%d%d%d%d",bWan?1:0,bQian?1:0,bBai?1:0,bShi?1:0,bGe?1:0);
			strTempHao+=TEXT(",");
			strTempHao+=str;

			temp += strTempHao;
			temp += TEXT("|");
		} while (nIndex < 10000);

		strHaoMa = temp;
	}
	else if(RenXuan3 == m_gameKind)
	{
		//��ѡ3��������
		if(m_RenxuanKeyboardInput)
		{
			if (!GetDanShiHaoma(strHaoMa, 3, zhushu)) 
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"��ѡ������ѡ��3λ�����ֺ��룺%s��", strHaoMa);
				MyMessageBox(strTips);
				return;
			}
			zhushu=1;

			BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
			int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

			if(nCount < 3)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ȷ��ѡ��ѡλ��");
				return;
			}

		}
		else
		{
			int nCount=0;
			if(!strWan.IsEmpty() )
				nCount++;
			if(!strQian.IsEmpty() )
				nCount++;
			if(!strBai.IsEmpty())
				nCount++;
			if(!strShi.IsEmpty() )
				nCount++;
			if(!strGe.IsEmpty())
				nCount++;
			if(nCount != 3)
			{
				m_DlgStatus.HideStatusWindow();
				CString strTip;
				strTip.Format(_T("��ѡ������Ϊ3λλ�������"));
				MyMessageBox(strTip);
				return;
			}
			strHaoMa = strWan + _T(",") + strQian + _T(",") +strBai + _T(",") +strShi + _T(",") +strGe;
			if(strWan.GetLength()>0)
				zhushu = zhushu * strWan.GetLength();
			if(strQian.GetLength() > 0)
				zhushu = zhushu * strQian.GetLength();
			if(strBai.GetLength() > 0)
				zhushu = zhushu * strBai.GetLength();
			if(strShi.GetLength() > 0)
				zhushu = zhushu * strShi.GetLength();
			if(strGe.GetLength() > 0)
				zhushu = zhushu * strGe.GetLength();
			if(zhushu > 800)
			{
				m_DlgStatus.HideStatusWindow();
				CString strTip;
				strTip.Format(_T("��ѡ�����Ͷ800ע��"));
				MyMessageBox(strTip);
				return;

			}
		}
	}
	else if(RenXuan4 == m_gameKind)
	{
		//��ѡ4��������
		if(m_RenxuanKeyboardInput)
		{
			if (!GetDanShiHaoma(strHaoMa, 4, zhushu)) 
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"��ѡ�ģ���ѡ��4λ�����ֺ��룺%s��", strHaoMa);
				MyMessageBox(strTips);
				return;
			}
			zhushu=1;

			BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
			int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
			int nLength = strHaoMa.GetLength();

			if(nCount < 4)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(L"����ȷ��ѡ��ѡλ��");
				return;
			}

		}
		else
		{
			int nCount=0;
			if(!strWan.IsEmpty() )
				nCount++;
			if(!strQian.IsEmpty() )
				nCount++;
			if(!strBai.IsEmpty())
				nCount++;
			if(!strShi.IsEmpty() )
				nCount++;
			if(!strGe.IsEmpty())
				nCount++;
			if(nCount != 4)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ�ı���Ϊ4λλ�������"));
				return;
			}
			strHaoMa = strWan + _T(",") + strQian + _T(",") +strBai + _T(",") +strShi + _T(",") +strGe;
			if(strWan.GetLength()>0)
				zhushu = zhushu * strWan.GetLength();
			if(strQian.GetLength() > 0)
				zhushu = zhushu * strQian.GetLength();
			if(strBai.GetLength() > 0)
				zhushu = zhushu * strBai.GetLength();
			if(strShi.GetLength() > 0)
				zhushu = zhushu * strShi.GetLength();
			if(strGe.GetLength() > 0)
				zhushu = zhushu * strGe.GetLength();
			if(zhushu > 8000)
			{
				m_DlgStatus.HideStatusWindow();
				CString strTip;
				strTip.Format(_T("��ѡ�����Ͷ8000ע��"));
				MyMessageBox(strTip);
				return;

			}

		}
	}
	else if(QianSanHunhe == m_gameKind)
	{
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu,4800)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��λ�����м��ö��Ż�ո�ָ������Ҳ���ѡ��������"));
			return;
		}
		if(zhushu > 180)
		{
			MyMessageBox(_T("ǰ��������Ͷ180ע��"));
			return;
		}
		if((strHaoMa.GetAt(0) == strHaoMa.GetAt(1) ) && ( strHaoMa.GetAt(0) == strHaoMa.GetAt(2)))
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("ǰ����ϵ�3λ���ֲ���ȫ����ͬ"));
			return;
		}
	}
	else if(ZhongSanHunhe == m_gameKind)
	{
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu,4800)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��λ�����м��ö��Ż�ո�ָ������Ҳ���ѡ��������"));
			return;
		}
		if(zhushu > 180)
		{
			MyMessageBox(_T("����������Ͷ180ע��"));
			return;
		}

		if((strHaoMa.GetAt(0) == strHaoMa.GetAt(1) ) && ( strHaoMa.GetAt(0) == strHaoMa.GetAt(2)))
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("������ϵ�3λ���ֲ���ȫ����ͬ"));
			return;
		}
	}
	else if(HouSanHunhe == m_gameKind)
	{
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu,4800)) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��λ�����м��ö��Ż�ո�ָ������Ҳ���ѡ��������"));
			return;
		}
		if(zhushu > 180)
		{
			MyMessageBox(_T("����������Ͷ180ע��"));
			return;
		}

		if((strHaoMa.GetAt(0) == strHaoMa.GetAt(1) ) && ( strHaoMa.GetAt(0) == strHaoMa.GetAt(2)))
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("������ϵ�3λ���ֲ���ȫ����ͬ"));
			return;
		}
	}
	else if (BuDingWei_HouSan == m_gameKind) 
	{
		if(strBai.IsEmpty()) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}


		zhushu = strBai.GetLength();
// 		if(zhushu>8)
// 		{
// 			m_DlgStatus.HideStatusWindow();
// 			MyMessageBox(L"���ǲ���λ���Ͷ8ע��");
// 			return;
// 
// 
// 		}

	}
	else if (BuDingWei_QianSan == m_gameKind) 
	{
		if(strBai.IsEmpty()) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}


		zhushu = strBai.GetLength();
	}
	else if (BuDingWei_ZhongSan == m_gameKind) 
	{
		if(strBai.IsEmpty()) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}


		zhushu = strBai.GetLength();
	}
	else if (DingWeiDan == m_gameKind) 
	{
		if(strWan.IsEmpty() && strQian.IsEmpty() && strBai.IsEmpty() && strShi.IsEmpty() && strGe.IsEmpty()) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��ѡ����ʵĺ���"));
			return;
		}

		strHaoMa = strWan + _T(",") + strQian + _T(",") +strBai + _T(",") +strShi + _T(",") +strGe;
		if(strWan.GetLength()>8 || strQian.GetLength()>8 || strBai.GetLength() > 8||strShi.GetLength()>8||strGe.GetLength()>8)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("��λ��ÿλ���Ͷ8�����֣�"));
			return;

		}

		zhushu = strWan.GetLength() + strQian.GetLength() + strBai.GetLength() + strShi.GetLength() + strGe.GetLength();
	}

	UpdateData();

	CStringArray strArr;

	//��ѡ2,3,4�ļ�������
	if (RenXuan2 == m_gameKind  && m_RenxuanKeyboardInput)
	{
		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
		int nNums=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
		BYTE cbDigit[5]={bWan, bQian, bBai, bShi, bGe};
		CString strLineNumber;
		bool bEnd = false;

		int na=nNums;
		do 
		{
			int nb= strHaoMa.Find(L"|");
			if(nb == -1)
				nb = strHaoMa.GetLength();	
			if(nb == 0)
				return;
			int abc[5]={0,0,0,0,0};

			for(int n=0; n<nb; n++)
			{
				abc[n] = strHaoMa.GetAt(n) - 0x30;
			}
			int nValue[10][5];
			memset(&nValue, -1, sizeof(nValue));

			GetComboArray(na, nb, abc, nValue);

			for(int n=0; n<nRenXuan2[nNums]; n++)
			{
				wstring strLines=GetXiaZhuString(10, na, n, nValue);
				if(bWan==0)
				{
					wstring str1=L",";
					strLines = str1.append(strLines);
				}
				if(bQian==0)
					InsertDouhaoAfter(1, strLines);
				if(bBai==0)
					InsertDouhaoAfter(2, strLines);
				if(bShi==0)
					InsertDouhaoAfter(3, strLines);
				if(bGe==0)
					strLines.append(L",");

				strArr.Add(strLines.c_str());
			}
			int nLeng = strHaoMa.GetLength();
			strHaoMa = strHaoMa.Right(nLeng-nb-1);

			nLeng = strHaoMa.GetLength();
			if(nLeng <= 0)
			{
				bEnd = true;
			}
		} while (!bEnd);


	
	}
	else	if(RenXuan3 == m_gameKind  && m_RenxuanKeyboardInput)
	{
		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
		int nNums=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
		BYTE cbDigit[5]={bWan, bQian, bBai, bShi, bGe};
		int na=nNums;

 		int nIndex=0;
 		CString strNum;
 
 		do
 		{
 			if(!AfxExtractSubString(strNum, strHaoMa, nIndex++, '|'))
 				break;
 			if(strNum.IsEmpty())
 				break;
 			int abc[5]={0,0,0,0,0};
 			for(int n=0; n<3; n++)
 			{
 				abc[n] = strNum.GetAt(n) - 0x30;
 			}
 			int nValue[10][5];
 			memset(&nValue, -1, sizeof(nValue));
 
 			GetComboArray(na, 3, abc, nValue);
 
 			for(int n=0; n<nRenXuan3[nNums]; n++)
 			{
 				wstring strLines=GetXiaZhuString(10, na, n, nValue);
 				if(bWan==0)
 				{
 					wstring str1=L",";
 					strLines = str1.append(strLines);
 				}
 				if(bQian==0)
 					InsertDouhaoAfter(1, strLines);
 				if(bBai==0)
 					InsertDouhaoAfter(2, strLines);
 				if(bShi==0)
 					InsertDouhaoAfter(3, strLines);
 				if(bGe==0)
 					strLines.append(L",");
 				strArr.Add(strLines.c_str());
 
 			}
 		}while(nIndex<100000);
	}
  	else	if(RenXuan4 == m_gameKind  && m_RenxuanKeyboardInput)
  	{
   		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
   		int nNums=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
   		BYTE cbDigit[5]={bWan, bQian, bBai, bShi, bGe};
   		
   		int na=nNums;
   
   		int nIndex=0;
   		CString strNum;
   
   		do
   		{
   			if(!AfxExtractSubString(strNum, strHaoMa, nIndex++, '|'))
   				break;
   			if(strNum.IsEmpty())
   				break;
   			int abc[5]={0,0,0,0,0};
   			for(int n=0; n<4; n++)
   			{
   				abc[n] = strNum.GetAt(n) - 0x30;
   			}
   			int nValue[10][5];
   			memset(&nValue, -1, sizeof(nValue));
   
   			GetComboArray(na, 4, abc, nValue);
   
   			for(int n=0; n<nRenXuan4[nNums]; n++)
   			{
   				wstring strLines=GetXiaZhuString(10, na, n, nValue);
   				if(bWan==0)
   				{
   					wstring str1=L",";
   					strLines = str1.append(strLines);
   				}
   				if(bQian==0)
   					InsertDouhaoAfter(1, strLines);
   				if(bBai==0)
   					InsertDouhaoAfter(2, strLines);
   				if(bShi==0)
   					InsertDouhaoAfter(3, strLines);
   				if(bGe==0)
   					strLines.append(L",");
   				strArr.Add(strLines.c_str());
   
   			}
   		}while(nIndex<100000);
  	}
	else  if (!strHaoMa.IsEmpty()) 
	{
		if(strHaoMa.GetLength()>1024)
		{
			m_DlgStatus.ShowStatusWindow(TEXT("����������ݣ����Ժ�..."));
		}
// 		CString strTip;
// 		strTip = m_listChqSSC.GetItemText(0,0);
// 		if(strTip == _T("������ӳɹ�"))
// 		{
// 			CInformation Information;
// 			if(Information.ShowMessageBox(L"��ܰ����",L"���������������Ӻ��룬��ȷ��",MB_YESNO)==IDYES)
// 			{
// 				OnBnClickedBtnClsList();
// 			}
// 			else
// 			{
// 				m_DlgStatus.HideStatusWindow();
// 				return ;
// 			}
// 
// 		}
		if(m_RenxuanKeyboardInput)
		{
			if(strHaoMa.GetLength()>10240)
			{

				wstring strdata;
				strdata = strHaoMa.GetBuffer(0);
				vecAllNums.insert(vecAllNums.begin(),1,strdata);

				m_strHaoma = strHaoMa;

				int nIndex = 0;
				CString strHaomaTemp;
				int nZongzhushu = 0;

				if(m_gameKind == WuxingZhixuan)
				{
					nZongzhushu += strHaoMa.GetLength()/6;
				}
				else if(m_gameKind == RenXuan4)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					int nZhushu = strHaoMa.GetLength()/5;
					nZongzhushu = nZhushu *nRenXuan4[nCount];
				}
				else
				{
					do
					{

						if(!AfxExtractSubString(strHaomaTemp, strHaoMa, nIndex++, '|'))
							break;
						if(strHaomaTemp.IsEmpty())
							break;

						nZongzhushu ++;

					}
					while(nIndex<80000);

				}
				int nCount = m_listChqSSC.GetItemCount();

				int nZhushuXinzhi = 0;
// 				if (m_TypeID == CZ_FENFEN_CAI||m_TypeID == CZ_ErFenCai)
// 				{
// 					nZhushuXinzhi = 10000;
// 				}
// 				else
				{
					nZhushuXinzhi = 80000;
				}

				if(m_gameKind == WuxingZhixuan&&nZongzhushu > nZhushuXinzhi)
				{
					m_DlgStatus.HideStatusWindow();
					CString strTips;
					strTips.Format(L"����ֱѡ���Ͷ%dע��",nZhushuXinzhi);
					MyMessageBox(strTips);
					return;

				}
				else if((m_gameKind == WuxingQianSi||m_gameKind == WuxingHousi)&&nZongzhushu > 8000)
				{
					m_DlgStatus.HideStatusWindow();
					CString strTips;
					strTips.Format(L"����ֱѡ���Ͷ8000ע��");
					MyMessageBox(strTips);
					return;

				}
				else if((m_gameKind == HouSanZhiXuan_FuShi||m_gameKind == QianSanZhiXuan_FuShi||m_gameKind == WuxingZhongsan)&&nZongzhushu > 800)
				{
					m_DlgStatus.HideStatusWindow();
					CString strTips;
					strTips.Format(L"����ֱѡ���Ͷ800ע��");
					MyMessageBox(strTips);
					return;
				}
// 				else if(m_gameKind == Zuxuan120&&nZongzhushu>202)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ120���Ͷ202ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan60&&nZongzhushu>672)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ60���Ͷ672ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan30&&nZongzhushu>288)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ30���Ͷ288ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan20&&nZongzhushu>288)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ20���Ͷ288ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan10&&nZongzhushu>72)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ10���Ͷ72ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan5&&nZongzhushu>72)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ5���Ͷ72ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
				else if(m_gameKind == Ren3Hunhe_Zuxuan)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nWeiCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					if(nWeiCount==3 && nZongzhushu > 180)
					{
						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"�����������λ�����Ͷ180ע��");
						return;

					}
					else if(nWeiCount == 4 && nZongzhushu>720)
					{

						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"��������ĸ�λ�����Ͷ720ע��");
						return;
					}
					else if(nWeiCount == 5 && nZongzhushu>1800)
					{

						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"����������λ�����Ͷ1800ע��");
						return;
					}
				}

				m_listChqSSC.InsertItem(nCount, L"������ӳɹ�");
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
				fJine = nZongzhushu*m_beishu*danzhujine;
				CString strZongzhu ;
				strZongzhu.Format(L"%.3lf",fJine);
				m_listChqSSC.SetItemText(nCount, 2, strZongzhu);

				m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());
				strZongzhu.Format(L"%d",nZongzhushu);
				m_listChqSSC.SetItemText(nCount, 4, strZongzhu);
				strZongzhu.Format(_T("%d"), m_beishu);
				m_listChqSSC.SetItemText(nCount, 5, strZongzhu);

				m_listChqSSC.createItemButton(nCount, 6, this->GetSafeHwnd());


				m_zongZhuShu += nZongzhushu;
			}
			else
			{
				//m_strHaoma.Empty();
				int nCount = m_listChqSSC.GetItemCount(); 
				int nIndex = 0;

				wstring strdata;
				strdata = strHaoMa.GetBuffer(0);
				vecAllNums.insert(vecAllNums.begin(),1,strdata);
				CString strInsert;
				int nInserZhushu = 0;
				do
				{
					CString strHaomaTemp;

					if(!AfxExtractSubString(strHaomaTemp, strHaoMa, nIndex++, '|'))
						break;
					if(strHaomaTemp.IsEmpty())
						break;

					if(m_gameKind == Ren3Zu3_Fushi||m_gameKind == Ren3Zu6_Fushi||m_gameKind ==Ren2Zuxuan_Fushi||m_gameKind ==Ren3Hunhe_Zuxuan)
					{

						CString strTemphao,strAddHaoma,strT;
						

						strT = strHaomaTemp.Right(5);
						AfxExtractSubString(strAddHaoma,strHaomaTemp,0,',');
						strAddHaoma+=TEXT(",");
						CString str;
						str= strT.GetAt(0);
						if(str ==  TEXT("1"))
						{
							strAddHaoma+=TEXT("��");
						}
						str= strT.GetAt(1);
						if(str == TEXT("1"))
						{
							strAddHaoma+=TEXT("ǧ");
						}
						str= strT.GetAt(2);
						if(str == TEXT("1"))
						{
							strAddHaoma+=TEXT("��");
						}
						str= strT.GetAt(3);
						if(str == TEXT("1"))
						{
							strAddHaoma+=TEXT("ʮ");
						}
						str= strT.GetAt(4);
						if(str == TEXT("1"))
						{
							strAddHaoma+=TEXT("��");
						}
						if(m_gameKind == Ren3Hunhe_Zuxuan)
						{
							BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
							int nCountCheck = GetRenXuanCheckBox(bWan,bQian,bBai,bShi,bGe);
							if(nCountCheck == 3)
								zhushu = 1;
							else if(nCountCheck == 4)
								zhushu = 4;
							else if(nCountCheck == 5)
								zhushu = 10;
						}

						strInsert += strAddHaoma;
						nInserZhushu+=zhushu;
					}
					else
					{
						strInsert += strHaomaTemp;
						zhushu = 1;

						int nLength = strHaomaTemp.GetLength()/2+1;
						if(m_gameKind == QianSanZuXuan_ZuLiuFuShi)
						{
							zhushu = nLength * (nLength - 1) * (nLength - 2) / 6;
						}
						if(m_gameKind == QianSanZuXuan_ZuSanFuShi)
						{
							zhushu = 1;
						}
						if(m_gameKind == ZhongsanZusan)
						{
							zhushu = 1;
						}
						if(m_gameKind == ZhongsanZuliu)
						{
							zhushu = nLength * (nLength - 1) * (nLength - 2) / 6;
						}
						if(m_gameKind == HouSanZuXuan_ZuSanFuShi)
						{
							zhushu = 1;
						}
						if(m_gameKind == HouSanZuXuan_ZuLiuFuShi)
						{
							zhushu = nLength * (nLength - 1) * (nLength - 2) / 6;
						}
						if(m_gameKind == Zuxuan120)
						{
							zhushu = (nLength-4)*(nLength-3)*(nLength-2)*(nLength-1)*nLength/120;
						}
						if(m_gameKind == Zuxuan60)
						{
							zhushu = 1;
						}
						nInserZhushu += zhushu;
					}

					strInsert += _T("|");

				}
				while(nIndex < 80000);

				int nZhushuXinzhi = 0;
// 				if (m_TypeID == CZ_FENFEN_CAI||m_TypeID == CZ_ErFenCai)
// 				{
// 					nZhushuXinzhi = 10000;
// 				}
// 				else
				{
					nZhushuXinzhi = 80000;
				}

				if(m_gameKind == WuxingZhixuan&&nInserZhushu > nZhushuXinzhi)
				{
					m_DlgStatus.HideStatusWindow();
					CString strTips;
					strTips.Format(L"����ֱѡ���Ͷ%dע��",nZhushuXinzhi);
					MyMessageBox(strTips);
					return;

				}
				else if((m_gameKind == WuxingQianSi||m_gameKind == WuxingHousi)&&nInserZhushu > 8000)
				{
					m_DlgStatus.HideStatusWindow();
					CString strTips;
					strTips.Format(L"����ֱѡ���Ͷ8000ע��");
					MyMessageBox(strTips);
					return;

				}
				else if((m_gameKind == HouSanZhiXuan_FuShi||m_gameKind == QianSanZhiXuan_FuShi||m_gameKind == WuxingZhongsan)&&nInserZhushu > 800)
				{
					m_DlgStatus.HideStatusWindow();
					CString strTips;
					strTips.Format(L"����ֱѡ���Ͷ800ע��");
					MyMessageBox(strTips);
					return;
				}

// 				else if(m_gameKind == Zuxuan120&&nInserZhushu>202)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ120���Ͷ202ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan60&&nInserZhushu>672)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ60���Ͷ672ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan30&&nInserZhushu>288)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ30���Ͷ288ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan20&&nInserZhushu>288)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ20���Ͷ288ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan10&&nInserZhushu>72)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ10���Ͷ72ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
// 				else if(m_gameKind == Zuxuan5&&nInserZhushu>72)
// 				{
// 					CString strTips;
// 					strTips.Format(L"��ѡ5���Ͷ72ע��");
// 					MyMessageBox(strTips);
// 					return;
// 				}
				else if(m_gameKind == Ren3Hunhe_Zuxuan)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nWeiCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					if(nWeiCount==3 && nInserZhushu > 180)
					{
						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"�����������λ�����Ͷ180ע��");
						return;

					}
					else if(nWeiCount == 4 && nInserZhushu>720)
					{

						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"��������ĸ�λ�����Ͷ720ע��");
						return;
					}
					else if(nWeiCount == 5 && nInserZhushu>1800)
					{

						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"����������λ�����Ͷ1800ע��");
						return;
					}
				}

				m_zongZhuShu += nInserZhushu;
				int nLenInsert = strInsert.GetLength();

				strInsert = strInsert.Left(nLenInsert-1);

				m_listChqSSC.InsertItem(nCount, strInsert);
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
				fJine = nInserZhushu*m_beishu*danzhujine;
				CString strZongzhu ;
				strZongzhu.Format(L"%.3lf",fJine);
				m_listChqSSC.SetItemText(nCount, 2, strZongzhu);

				m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());
				CString strTmp;
				strTmp.Format(_T("%d"), nInserZhushu);
				m_listChqSSC.SetItemText(nCount, 4, strTmp);
				strTmp.Format(_T("%d"), m_beishu);
				m_listChqSSC.SetItemText(nCount, 5, strTmp);

				m_listChqSSC.createItemButton(nCount++, 6, this->GetSafeHwnd());

			}


		}
		else
		{

		//	m_strHaoma.Empty();


			wstring strdata;
			strdata = strHaoMa.GetBuffer(0);
			int nSize = vecAllNums.size();
			vecAllNums.insert(vecAllNums.begin(),1,strdata);
			//vecAllNums.insert(vecAllNums.end(),1,strdata);

			int iLine = m_listChqSSC.GetItemCount();
			if(m_gameKind == DaXiaoDanShuang ||m_gameKind == DaXiaoDanShuang_dw)
			{
				CString strAddHaoma;
				int nAddLength = strHaoMa.GetLength();
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
					int nAddLen = strAdd.GetLength();
					for(int i = 0;i < nAddLen;i++)
					{
						CString add;
						add = strAdd.GetAt(i);
						int nNum = _ttoi(add);
						CString strDesc = GetDxdshStringDesc(nNum);
						strAddHaoma += strDesc;
					}

				} while (nAddIndex<1000);

				m_listChqSSC.InsertItem(iLine, strAddHaoma);
			}
			else if(m_gameKind == SSC_LongHu)
			{
				CString strAddHaoma,strTe;
				AfxExtractSubString(strTe, strHaoMa, 0, ',');
				if(strTe.IsEmpty())
				{
					m_DlgStatus.HideStatusWindow();

					MyMessageBox(L"����������ѡ��һλ��");
					return;
				}
				strTe.Replace(L"4",L"��");
				strTe.Replace(L"5",L"��");
				strTe.Replace(L"6",L"��");
				strAddHaoma+=strTe;
				strAddHaoma += _T(",");
				AfxExtractSubString(strTe, strHaoMa, 1, ',');
				CString str[10] = {_T("��ǧ"),_T("���"),_T("��ʮ"),_T("���"),_T("ǧ��"),_T("ǧʮ"),_T("ǧ��"),_T("��ʮ"),_T("�ٸ�"),_T("ʮ��")};
				CString strTemp[10] = {_T("11000"),_T("10100"),_T("10010"),_T("10001"),_T("01100"),_T("01010"),_T("01001"),_T("00110"),_T("00101"),_T("00011")};
				int nTemp = 0;
				for(BYTE l = 0;l < 10;l++)
				{
					CString strWei = strTemp[l];
					if(strWei == strTe)
					{
						nTemp = l;
					}
				}
				if(nTemp < 10)
				{
					strAddHaoma += str[nTemp];
				}

				m_listChqSSC.InsertItem(iLine, strAddHaoma);
			}
			else if(m_gameKind == Ren3Zu3_Fushi||m_gameKind == Ren3Zu6_Fushi||m_gameKind ==Ren2Zuxuan_Fushi||m_gameKind ==Ren3Hunhe_Zuxuan)
			{
				CString strAddHaoma,strT;
				strT = strHaoMa.Right(5);
	
				AfxExtractSubString(strAddHaoma, strHaoMa, 0, ',');

				strAddHaoma+=TEXT(",");
				CString str;
				str= strT.GetAt(0);
				if(str ==  TEXT("1"))
				{
					strAddHaoma+=TEXT("��");
				}
				str= strT.GetAt(1);
				if(str == TEXT("1"))
				{
					strAddHaoma+=TEXT("ǧ");
				}
				str= strT.GetAt(2);
				if(str == TEXT("1"))
				{
					strAddHaoma+=TEXT("��");
				}
				str= strT.GetAt(3);
				if(str == TEXT("1"))
				{
					strAddHaoma+=TEXT("ʮ");
				}
				str= strT.GetAt(4);
				if(str == TEXT("1"))
				{
					strAddHaoma+=TEXT("��");
				}
				m_listChqSSC.InsertItem(iLine, strAddHaoma);
			}
			else
			{
				m_listChqSSC.InsertItem(iLine, strHaoMa);
			}
			m_listChqSSC.SetItemText(iLine, 1, GetScoreMoshi());
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
			m_listChqSSC.SetItemText(iLine, 2, strZongzhu);

			m_listChqSSC.SetItemText(iLine, 3, GetWanDesc());
			CString strTmp;
			strTmp.Format(_T("%d"), zhushu);
			m_listChqSSC.SetItemText(iLine, 4, strTmp);
			strTmp.Format(_T("%d"), m_beishu);
			m_listChqSSC.SetItemText(iLine, 5, strTmp);

			m_listChqSSC.createItemButton(iLine, 6, this->GetSafeHwnd());

// 			CString strMD5Haoma;
// 			for(int i = 0;i < vecAllNums.size();i++)
// 			{
// 				strMD5Haoma += vecAllNums[i].c_str();
// 				strMD5Haoma += L"|";
// 			}
// 
// 			memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
// 			CMD5Encrypt::EncryptData(strMD5Haoma,m_szMD5Haoma);

			m_zongZhuShu += zhushu;
		}
		strHaoMa = _T("");
	}

	int arrLen = strArr.GetCount();
	if (arrLen > 0) 
	{
		int nLen = strHaoMa.GetLength();
		strHaoMa.Empty();
		int nZhushu = 0;
		for (int i=0; i<arrLen; i++) 
		{
			strHaoMa += strArr.GetAt(i);
			nZhushu+=zhushu;
			if(i!=(arrLen-1))
				strHaoMa += L"|";

		}
		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
		int nWeiCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

		if(m_gameKind == RenXuan2 )
		{
			if(nWeiCount == 2&&nZhushu>80)
			{
				m_DlgStatus.HideStatusWindow();
				//MyMessageBox(_T("��ѡ������λ�����Ͷ80ע��"));
				MyMessageBox(_T("��ѡ������λ�����Ͷ80ע��"));
				return;
			}
			else if(nWeiCount == 3&&nZhushu > 240)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ������λ�����Ͷ240ע��"));
				return;
			}
			else if(nWeiCount == 4&&nZhushu > 480)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ���ĸ�λ�����Ͷ480ע��"));
				return;
			}
			else if(nWeiCount == 5&&nZhushu > 800)  
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ�����λ�����Ͷ800ע��"));
				return;
			}

		}
		else if(m_gameKind == RenXuan3)
		{

			if(nWeiCount == 3&&nZhushu>800) 
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ������λ�����Ͷ800ע��"));
				return;
			}
			else if(nWeiCount == 4&&nZhushu > 3200)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ���ĸ�λ�����Ͷ3200ע��"));
				return;
			}
			else if(nWeiCount == 5&&nZhushu > 8000)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ�����λ�����Ͷ8000ע��"));
				return;
			}
		}
		else if(m_gameKind == RenXuan4)
		{
			if(nWeiCount == 4&&nZhushu>8000)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ���ĸ�λ�����Ͷ8000ע��"));
				return;
			}
			else if(nWeiCount == 5&&nZhushu>40000)
			{
				m_DlgStatus.HideStatusWindow();
				MyMessageBox(_T("��ѡ�����λ�����Ͷ8000ע��"));
				return;
			}

		}

		if(strHaoMa.GetLength()>8000)
		{
			m_strHaoma = strHaoMa;
			int nCount = m_listChqSSC.GetItemCount();

			wstring strdata;
			strdata = strHaoMa.GetBuffer();
			vecAllNums.insert(vecAllNums.begin(),1,strdata);//.push_back(strdata);


			m_listChqSSC.InsertItem(nCount, L"������ӳɹ�");
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

			m_zongZhuShu += nZhushu;
			CString strTmp;
			strTmp.Format(_T("%d"), nZhushu);
			m_listChqSSC.SetItemText(nCount, 4, strTmp);
			strTmp.Format(_T("%d"), m_beishu);
			m_listChqSSC.SetItemText(nCount, 5, strTmp);
			m_listChqSSC.createItemButton(nCount, 6, this->GetSafeHwnd());

			strArr.RemoveAll();

			memset(&g_arr, 0, sizeof(g_arr));
			memset(&g_nPos, 0, sizeof(g_nPos));
			g_Counts=0;
			
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
			m_listChqSSC.createItemButton(nCount, 6, this->GetSafeHwnd());

			wstring strdata;
			strdata = strHaoMa.GetBuffer();
			vecAllNums.insert(vecAllNums.begin(),1,strdata);//.push_back(strdata);


			m_zongZhuShu += nZhushu;
			strArr.RemoveAll();

			memset(&g_arr, 0, sizeof(g_arr));
			memset(&g_nPos, 0, sizeof(g_nPos));
			g_Counts=0;
		}

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

	memset(&g_arr, 0, sizeof(g_arr));
	memset(&g_nPos, 0, sizeof(g_nPos));
	g_Counts=0;

	m_DlgStatus.HideStatusWindow();
	ResetAllNums();
}

//��ʼ�����밴ť
void CChongQingSSC::InitNumberBtns()
{
	m_btnLong.SetImage(m_numImageList3, 0, 3);
	m_btnHu.SetImage(m_numImageList3, 1, 3);
	m_btnHe.SetImage(m_numImageList3, 2, 3);

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

	m_btnNiu_1.SetImage(m_numImageList2, 0, 11);
	m_btnNiu_2.SetImage(m_numImageList2, 1, 11);
	m_btnNiu_3.SetImage(m_numImageList2, 2, 11);
	m_btnNiu_4.SetImage(m_numImageList2, 3, 11);
	m_btnNiu_5.SetImage(m_numImageList2, 4, 11);
	m_btnNiu_6.SetImage(m_numImageList2, 5, 11);
	m_btnNiu_7.SetImage(m_numImageList2, 6, 11);
	m_btnNiu_8.SetImage(m_numImageList2, 7, 11);
	m_btnNiu_9.SetImage(m_numImageList2, 8, 11);
	m_btnNiu_10.SetImage(m_numImageList2, 9, 11);
	m_btnNiu_0.SetImage(m_numImageList2, 10, 11);

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

	m_staticZuXuan.SetImage(m_bmpNumTip, 0, 10);
	m_staticBuDingWei.SetImage(m_bmpNumTip, 1, 10);
	m_staticWan.SetImage(m_bmpNumTip, 2, 10);
	m_staticQian.SetImage(m_bmpNumTip, 3, 10);
	m_staticBai.SetImage(m_bmpNumTip, 4, 10);
	m_staticShi.SetImage(m_bmpNumTip, 5, 10);
	m_staticGe.SetImage(m_bmpNumTip, 6, 10);
	m_staticNiu.SetImage(m_bmpNumTip, 7, 10);
	m_staticLhd.SetImage(m_bmpNumTip, 8, 10);
	m_statichzh.SetImage(m_bmpNumTip, 9, 10);

	m_staticZu120.SetImage(m_bmpNumTip1, 0, 6);
	m_staticDanHao.SetImage(m_bmpNumTip1, 1, 6);
	m_staticErChong.SetImage(m_bmpNumTip1, 2, 6);
	m_staticErChong1.SetImage(m_bmpNumTip1, 2, 6);
	m_staticSanChong.SetImage(m_bmpNumTip1, 3, 6);
	m_staticSiChong.SetImage(m_bmpNumTip1, 4, 6);
	m_staticXuanHao.SetImage(m_bmpNumTip1, 5, 6);


	m_btnDaWan.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoWan.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanWan.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangWan.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaQian.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoQian.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanQian.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangQian.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaBai.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoBai.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanBai.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangBai.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaShi.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoShi.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanShi.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangShi.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaGe.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoGe.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanGe.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangGe.SetImage(m_bmpDxdsh, 3, 4);

	m_staticRio1.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_static1.png"));
	m_staticRio2.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_static2.png"));
	m_staticRio3.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_static1.png"));
	m_staticRio1.SetTextFont(&m_staticFont);
	m_staticRio2.SetTextFont(&m_staticFont);
	m_staticRio3.SetTextFont(&m_staticFont);
	m_staticRio1.SetTextColor(RGB(156,152,151));
	m_staticRio2.SetTextColor(RGB(156,152,151));
	m_staticRio3.SetTextColor(RGB(156,152,151));
	CString numTextBk_L = CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx_L.png");
	CString numTextBk   = CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx.png");
	CString numTextBk_R = CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx_R.png");

	m_btnWanQuan.SetBkImage(numTextBk_L);
	m_btnWanDa.SetBkImage(numTextBk);
	m_btnWanXiao.SetBkImage(numTextBk);
	m_btnWanDan.SetBkImage(numTextBk);
	m_btnWanShuang.SetBkImage(numTextBk);
	m_btnWanQing.SetBkImage(numTextBk_R);

	m_btnQianQuan.SetBkImage(numTextBk_L);
	m_btnQianDa.SetBkImage(numTextBk);
	m_btnQianXiao.SetBkImage(numTextBk);
	m_btnQianDan.SetBkImage(numTextBk);
	m_btnQianShuang.SetBkImage(numTextBk);
	m_btnQianQing.SetBkImage(numTextBk_R);

	m_btnBaiQuan.SetBkImage(numTextBk_L);
	m_btnBaiDa.SetBkImage(numTextBk);
	m_btnBaiXiao.SetBkImage(numTextBk);
	m_btnBaiDan.SetBkImage(numTextBk);
	m_btnBaiShuang.SetBkImage(numTextBk);
	m_btnBaiQing.SetBkImage(numTextBk_R);

	m_btnNiuQuan.SetBkImage(numTextBk_L);
	m_btnNiuDa.SetBkImage(numTextBk);
	m_btnNiuXiao.SetBkImage(numTextBk);
	m_btnNiuDan.SetBkImage(numTextBk);
	m_btnNiuShuang.SetBkImage(numTextBk);
	m_btnNiuQing.SetBkImage(numTextBk_R);

	m_btnShiQuan.SetBkImage(numTextBk_L);
	m_btnShiDa.SetBkImage(numTextBk);
	m_btnShiXiao.SetBkImage(numTextBk);
	m_btnShiDan.SetBkImage(numTextBk);
	m_btnShiShuang.SetBkImage(numTextBk);
	m_btnShiQing.SetBkImage(numTextBk_R);

	m_btnGeQuan.SetBkImage(numTextBk_L);
	m_btnGeDa.SetBkImage(numTextBk);
	m_btnGeXiao.SetBkImage(numTextBk);
	m_btnGeDan.SetBkImage(numTextBk);
	m_btnGeShuang.SetBkImage(numTextBk);
	m_btnGeQing.SetBkImage(numTextBk_R);

	m_btnNiuQuan.SetTextColor(RGB(97,76,59));
	m_btnNiuDa.SetTextColor(RGB(97,76,59));
	m_btnNiuXiao.SetTextColor(RGB(97,76,59));
	m_btnNiuDan.SetTextColor(RGB(97,76,59));
	m_btnNiuShuang.SetTextColor(RGB(97,76,59));
	m_btnNiuQing.SetTextColor(RGB(97,76,59));

	m_btnWanQuan.SetTextColor(RGB(93,123,187));
	m_btnWanDa.SetTextColor(RGB(93,123,187));
	m_btnWanXiao.SetTextColor(RGB(93,123,187));
	m_btnWanDan.SetTextColor(RGB(93,123,187));
	m_btnWanShuang.SetTextColor(RGB(93,123,187));
	m_btnWanQing.SetTextColor(RGB(93,123,187));

	m_btnQianQuan.SetTextColor(RGB(93,123,187));
	m_btnQianDa.SetTextColor(RGB(93,123,187));
	m_btnQianXiao.SetTextColor(RGB(93,123,187));
	m_btnQianDan.SetTextColor(RGB(93,123,187));
	m_btnQianShuang.SetTextColor(RGB(93,123,187));
	m_btnQianQing.SetTextColor(RGB(93,123,187));

	m_btnBaiQuan.SetTextColor(RGB(93,123,187));
	m_btnBaiDa.SetTextColor(RGB(93,123,187));
	m_btnBaiXiao.SetTextColor(RGB(93,123,187));
	m_btnBaiDan.SetTextColor(RGB(93,123,187));
	m_btnBaiShuang.SetTextColor(RGB(93,123,187));
	m_btnBaiQing.SetTextColor(RGB(93,123,187));

	m_btnShiQuan.SetTextColor(RGB(93,123,187));
	m_btnShiDa.SetTextColor(RGB(93,123,187));
	m_btnShiXiao.SetTextColor(RGB(93,123,187));
	m_btnShiDan.SetTextColor(RGB(93,123,187));
	m_btnShiShuang.SetTextColor(RGB(93,123,187));
	m_btnShiQing.SetTextColor(RGB(93,123,187));

	m_btnGeQuan.SetTextColor(RGB(93,123,187));
	m_btnGeDa.SetTextColor(RGB(93,123,187));
	m_btnGeXiao.SetTextColor(RGB(93,123,187));
	m_btnGeDan.SetTextColor(RGB(93,123,187));
	m_btnGeShuang.SetTextColor(RGB(93,123,187));
	m_btnGeQing.SetTextColor(RGB(93,123,187));

	m_btWxzxTip.SetImage(CBmpUtil::GetExePath() + _T("skin\\bangzhu.png"));
	m_btWxzxHelp.SetImage(CBmpUtil::GetExePath() + _T("skin\\shili.png"));
}

void CChongQingSSC::InitListCtrl()
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

	m_staticHunheTip.SetTextColor(RGB(250, 250, 250));
	m_staticNiuNiuTip.SetTextColor(RGB(250, 250, 250));
}

//����ֱѡ��ʽ
void CChongQingSSC::OnBnClickedRioHszhxFushi()
{
	ResetAllNums();

	m_gameKind = HouSanZhiXuan_FuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioHsZhxFuShi.SetPushed(true);
	m_rioHsZhxDanShi.SetPushed(false);
	m_rioHsZsFuShi.SetPushed(false);
	m_rioHsZsDanShi.SetPushed(false);
	m_rioHsZlFuShi.SetPushed(false);
	m_rioHsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);

	m_RenxuanKeyboardInput = false;
	

	HideDaXiaoDanShuang();
	HideDaXiaoDanShuangDw();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideDanShiEdit();
}
//������
void CChongQingSSC::OnBnClickedRioLhdWq()
{
	m_nLhdID = 0;
	m_rioLhdWq.SetPushed(true);
	m_rioLhdWb.SetPushed(false);
	m_rioLhdWs.SetPushed(false);
	m_rioLhdWg.SetPushed(false);
	m_rioLhdQb.SetPushed(false);
	m_rioLhdQs.SetPushed(false);
	m_rioLhdQg.SetPushed(false);
	m_rioLhdBs.SetPushed(false);
	m_rioLhdBg.SetPushed(false);
	m_rioLhdSg.SetPushed(false);

	ShowLonghudou();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��λ�������ǧλ����Ϊ������λ����С��ǧλ����Ϊ����������ͬ��Ϊ�͡�"));	
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,�������� ��λ ����ǧλ����Ϊ�н���"));	
}
//������
void CChongQingSSC::OnBnClickedRioLhdWb()
{
	m_nLhdID = 1;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��λ������ڰ�λ����Ϊ������λ����С�ڰ�λ����Ϊ����������ͬ��Ϊ�͡�"));	
	m_rioLhdWq.SetPushed(false);
	m_rioLhdWb.SetPushed(true);
	m_rioLhdWs.SetPushed(false);
	m_rioLhdWg.SetPushed(false);
	m_rioLhdQb.SetPushed(false);
	m_rioLhdQs.SetPushed(false);
	m_rioLhdQg.SetPushed(false);
	m_rioLhdBs.SetPushed(false);
	m_rioLhdBg.SetPushed(false);
	m_rioLhdSg.SetPushed(false);
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,�������� ��λ ���ڰ�λ����Ϊ�н���"));	

}
//������
void CChongQingSSC::OnBnClickedRioLhdWs()
{
	m_nLhdID = 2;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��λ�������ʮλ����Ϊ������λ����С��ʮλ����Ϊ����������ͬ��Ϊ�͡�"));	
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,�������� ��λ ����ʮλ����Ϊ�н���"));	
	m_rioLhdWq.SetPushed(false);
	m_rioLhdWb.SetPushed(false);
	m_rioLhdWs.SetPushed(true);
	m_rioLhdWg.SetPushed(false);
	m_rioLhdQb.SetPushed(false);
	m_rioLhdQs.SetPushed(false);
	m_rioLhdQg.SetPushed(false);
	m_rioLhdBs.SetPushed(false);
	m_rioLhdBg.SetPushed(false);
	m_rioLhdSg.SetPushed(false);

}
//������
void CChongQingSSC::OnBnClickedRioLhdWg()
{
	m_nLhdID = 3;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��λ������ڸ�λ����Ϊ������λ����С�ڸ�λ����Ϊ����������ͬ��Ϊ�͡�"));	
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,�������� ��λ ���ڸ�λ����Ϊ�н���"));	
	m_rioLhdWq.SetPushed(false);
	m_rioLhdWb.SetPushed(false);
	m_rioLhdWs.SetPushed(false);
	m_rioLhdWg.SetPushed(true);
	m_rioLhdQb.SetPushed(false);
	m_rioLhdQs.SetPushed(false);
	m_rioLhdQg.SetPushed(false);
	m_rioLhdBs.SetPushed(false);
	m_rioLhdBg.SetPushed(false);
	m_rioLhdSg.SetPushed(false);

}
//������
void CChongQingSSC::OnBnClickedRioLhdQb()
{
	m_nLhdID = 4;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ǧλ������ڰ�λ����Ϊ����ǧλ����С�ڰ�λ����Ϊ����������ͬ��Ϊ�͡�"));	
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,�������� ǧλ ���ڰ�λ����Ϊ�н���"));	

	m_rioLhdWq.SetPushed(false);
	m_rioLhdWb.SetPushed(false);
	m_rioLhdWs.SetPushed(false);
	m_rioLhdWg.SetPushed(false);
	m_rioLhdQb.SetPushed(true);
	m_rioLhdQs.SetPushed(false);
	m_rioLhdQg.SetPushed(false);
	m_rioLhdBs.SetPushed(false);
	m_rioLhdBg.SetPushed(false);
	m_rioLhdSg.SetPushed(false);

}
//������
void CChongQingSSC::OnBnClickedRioLhdQs()
{
	m_nLhdID = 5;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ǧλ�������ʮλ����Ϊ����ǧλ����С��ʮλ����Ϊ����������ͬ��Ϊ�͡�"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,�������� ǧλ ����ʮλ����Ϊ�н���"));	
	m_rioLhdWq.SetPushed(false);
	m_rioLhdWb.SetPushed(false);
	m_rioLhdWs.SetPushed(false);
	m_rioLhdWg.SetPushed(false);
	m_rioLhdQb.SetPushed(false);
	m_rioLhdQs.SetPushed(true);
	m_rioLhdQg.SetPushed(false);
	m_rioLhdBs.SetPushed(false);
	m_rioLhdBg.SetPushed(false);
	m_rioLhdSg.SetPushed(false);

}
//������
void CChongQingSSC::OnBnClickedRioLhdQg()
{
	m_nLhdID = 6;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ǧλ������ڸ�λ����Ϊ����ǧλ����С�ڸ�λ����Ϊ����������ͬ��Ϊ�͡�"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,�������� ǧλ ���ڸ�λ����Ϊ�н���"));	
	m_rioLhdWq.SetPushed(false);
	m_rioLhdWb.SetPushed(false);
	m_rioLhdWs.SetPushed(false);
	m_rioLhdWg.SetPushed(false);
	m_rioLhdQb.SetPushed(false);
	m_rioLhdQs.SetPushed(false);
	m_rioLhdQg.SetPushed(true);
	m_rioLhdBs.SetPushed(false);
	m_rioLhdBg.SetPushed(false);
	m_rioLhdSg.SetPushed(false);

}
//������
void CChongQingSSC::OnBnClickedRioLhdBs()
{
	m_nLhdID = 7;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��λ�������ʮλ����Ϊ������λ����С��ʮλ����Ϊ����������ͬ��Ϊ�͡�"));	
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,���������λ����ʮλ����Ϊ�н���"));	
	m_rioLhdWq.SetPushed(false);
	m_rioLhdWb.SetPushed(false);
	m_rioLhdWs.SetPushed(false);
	m_rioLhdWg.SetPushed(false);
	m_rioLhdQb.SetPushed(false);
	m_rioLhdQs.SetPushed(false);
	m_rioLhdQg.SetPushed(false);
	m_rioLhdBs.SetPushed(true);
	m_rioLhdBg.SetPushed(false);
	m_rioLhdSg.SetPushed(false);

}
//������
void CChongQingSSC::OnBnClickedRioLhdBg()
{
	m_nLhdID = 8;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��λ������ڸ�λ����Ϊ������λ����С�ڸ�λ����Ϊ����������ͬ��Ϊ�͡�"));	
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,�������� ��λ���ڸ�λ����Ϊ�н���"));	
	m_rioLhdWq.SetPushed(false);
	m_rioLhdWb.SetPushed(false);
	m_rioLhdWs.SetPushed(false);
	m_rioLhdWg.SetPushed(false);
	m_rioLhdQb.SetPushed(false);
	m_rioLhdQs.SetPushed(false);
	m_rioLhdQg.SetPushed(false);
	m_rioLhdBs.SetPushed(false);
	m_rioLhdBg.SetPushed(true);
	m_rioLhdSg.SetPushed(false);

}
//������
void CChongQingSSC::OnBnClickedRioLhdSg()
{
	m_nLhdID = 9;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ʮλ������ڸ�λ����Ϊ����ʮλ����С�ڸ�λ����Ϊ����������ͬ��Ϊ�͡�"));	
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��,��������ʮλ���ڸ�λ����Ϊ�н���"));	
	m_rioLhdWq.SetPushed(false);
	m_rioLhdWb.SetPushed(false);
	m_rioLhdWs.SetPushed(false);
	m_rioLhdWg.SetPushed(false);
	m_rioLhdQb.SetPushed(false);
	m_rioLhdQs.SetPushed(false);
	m_rioLhdQg.SetPushed(false);
	m_rioLhdBs.SetPushed(false);
	m_rioLhdBg.SetPushed(false);
	m_rioLhdSg.SetPushed(true);

}

//һ����˳
void CChongQingSSC::OnBnClickedRioYffs()
{
	ResetAllNums();

	m_gameKind = QW_YiFanFengShun;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioYffs.SetPushed(true);
	m_rioHscs.SetPushed(false);
	m_rioSxbx.SetPushed(false);
	m_rioSjfc.SetPushed(false);


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();
	ShowXuanHao();

	HideDanShiEdit();
}
//���³�˫
void CChongQingSSC::OnBnClickedRioHscs()
{
	ResetAllNums();

	m_gameKind = QW_HaoShiChengShuang;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioYffs.SetPushed(false);
	m_rioHscs.SetPushed(true);
	m_rioSxbx.SetPushed(false);
	m_rioSjfc.SetPushed(false);


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();
	ShowXuanHao();

	HideDanShiEdit();
}
//���Ǳ�ϲ
void CChongQingSSC::OnBnClickedRioSxbx()
{
	ResetAllNums();

	m_gameKind = QW_SanXingBaoXi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioYffs.SetPushed(false);
	m_rioHscs.SetPushed(false);
	m_rioSxbx.SetPushed(true);
	m_rioSjfc.SetPushed(false);


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();
	ShowXuanHao();

	HideDanShiEdit();
}
//�ļ�����
void CChongQingSSC::OnBnClickedRioSjfc()
{
	ResetAllNums();

	m_gameKind = QW_SiJiFaCai;
SendToServer(3);
	SetLockText();

	m_rioYffs.SetPushed(false);
	m_rioHscs.SetPushed(false);
	m_rioSxbx.SetPushed(false);
	m_rioSjfc.SetPushed(true);

	HideNiuNums();

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();
	ShowXuanHao();

	HideDanShiEdit();
}
//ǰ����ֵ
void CChongQingSSC::OnBnClickedRioQsHezhi()
{
	ResetAllNums();

	m_gameKind = QianSanZhiXuanHeZhi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsHezhi.SetPushed(true);
	m_rioZsHezhi.SetPushed(false);
	m_rioHsHezhi.SetPushed(false);
	m_rioQeHezhi.SetPushed(false);
	m_rioHeHezhi.SetPushed(false);

	HideNiuNums();

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
	ShowHeZhiNums();
}

//������ֵ
void CChongQingSSC::OnBnClickedRioZsHezhi()
{
	ResetAllNums();

	m_gameKind = ZhongSanZhiXuanHeZhi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsHezhi.SetPushed(false);
	m_rioZsHezhi.SetPushed(true);
	m_rioHsHezhi.SetPushed(false);
	m_rioQeHezhi.SetPushed(false);
	m_rioHeHezhi.SetPushed(false);

	HideNiuNums();

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
	ShowHeZhiNums();
}
//������ֵ
void CChongQingSSC::OnBnClickedRioHsHezhi()
{
	ResetAllNums();

	m_gameKind = HouSanZhiXuanHeZhi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsHezhi.SetPushed(false);
	m_rioZsHezhi.SetPushed(false);
	m_rioHsHezhi.SetPushed(true);
	m_rioQeHezhi.SetPushed(false);
	m_rioHeHezhi.SetPushed(false);


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowHeZhiNums();
	HideDanShiEdit();
}
//ǰ����ֵ
void CChongQingSSC::OnBnClickedRioQeHezhi()
{
	ResetAllNums();

	m_gameKind = QianErZhixuan_Hezhi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsHezhi.SetPushed(false);
	m_rioZsHezhi.SetPushed(false);
	m_rioHsHezhi.SetPushed(false);
	m_rioQeHezhi.SetPushed(true);
	m_rioHeHezhi.SetPushed(false);

	HideNiuNums();

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
	HideHeZhiNums();
	ShowErXingHeZhiNums();
}

//�����ֵ
void CChongQingSSC::OnBnClickedRioHeHezhi()
{
	ResetAllNums();

	m_gameKind = HouErZhixuan_Hezhi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsHezhi.SetPushed(false);
	m_rioZsHezhi.SetPushed(false);
	m_rioHsHezhi.SetPushed(false);
	m_rioQeHezhi.SetPushed(false);
	m_rioHeHezhi.SetPushed(true);


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideHeZhiNums();

	ShowErXingHeZhiNums();
	HideDanShiEdit();
}
//ţ��
void CChongQingSSC::OnBnClickedRioNiuShu()
{
	ResetAllNums();

	m_gameKind = NiuNiu_Num;

	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioNiuShu.SetPushed(true);
	m_rioWuNiu.SetPushed(false);
	m_rioNiuNiu.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideShiNums();
	HideGeNums();

	HideBaiNums();

	ShowNiuNums();

	HideDanShiEdit();
}
//ţţ
void CChongQingSSC::OnBnClickedRioNiuNiu()
{
	ResetAllNums();

	m_gameKind = NiuNiu_Num;

	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuNiu.SetPushed(false);
	m_rioNiuShu.SetPushed(false);
	m_rioNiuNiu.SetPushed(true);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideShiNums();
	HideGeNums();

	HideBaiNums();

	HideNiuNums();

	m_btnNiu_10.ShowWindow(SW_SHOW);
	m_staticNiu.ShowWindow(SW_SHOW);

	m_btnNiuQuan.ShowWindow(SW_SHOW);
	m_btnNiuDa.ShowWindow(SW_SHOW);
	m_btnNiuXiao.ShowWindow(SW_SHOW);
	m_btnNiuDan.ShowWindow(SW_SHOW);
	m_btnNiuShuang.ShowWindow(SW_SHOW);
	m_btnNiuQing.ShowWindow(SW_SHOW);

	HideDanShiEdit();
}
//��ţ
void CChongQingSSC::OnBnClickedRioWuNiu()
{
	ResetAllNums();

	m_gameKind = NiuNiu_None;

	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioNiuShu.SetPushed(false);
	m_rioWuNiu.SetPushed(true);
	m_rioNiuNiu.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideShiNums();
	HideGeNums();

	HideBaiNums();



	HideNiuNums();
	m_staticNiu.ShowWindow(SW_SHOW);

	m_btnNiu_0.ShowWindow(SW_SHOW);

	HideDanShiEdit();
}
//����ֱѡ
void CChongQingSSC::OnBnClickedRioWuXingZhxDan()
{
	ResetAllNums();

	m_gameKind = WuxingZhixuan;
	SendToServer(3);
	SetLockText();
	m_rioWuXingZX.SetPushed(false);
	m_rioWuXingZXDan.SetPushed(true);
	m_rioZuXuan120.SetPushed(false);
	m_rioZuXuan60.SetPushed(false);
	m_rioZuXuan30.SetPushed(false);
	m_rioZuXuan20.SetPushed(false);
	m_rioZuXuan10.SetPushed(false);
	m_rioZuXuan5.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_RenxuanKeyboardInput=true;
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:23456;��������:23456;��������ֱѡ��"));
//	OnBnClickedRioKeyboardInput();
}
//����ֱѡ
void CChongQingSSC::OnBnClickedRioWuXingZhx()
{
	if(m_gameKind == WuxingZhixuan && m_RenxuanKeyboardInput==false)
	{
		return;
	}
	ResetAllNums();

	m_gameKind = WuxingZhixuan;

	SendToServer(3);
	SetLockText();

	m_btWxzxTip.ShowWindow(SW_SHOW);
	m_rioWuXingZX.SetPushed(true);
	m_rioWuXingZXDan.SetPushed(false);
	m_rioZuXuan120.SetPushed(false);
	m_rioZuXuan60.SetPushed(false);
	m_rioZuXuan30.SetPushed(false);
	m_rioZuXuan20.SetPushed(false);
	m_rioZuXuan10.SetPushed(false);
	m_rioZuXuan5.SetPushed(false);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideNiuNums();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideDanShiEdit();
}
//����ǰ��
void CChongQingSSC::OnBnClickedRioWuXingQs()
{
	if(m_gameKind == WuxingQianSi&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = WuxingQianSi;
	SendToServer(3);
	SetLockText();

	m_rioWuXingQS.SetPushed(true);
	m_rioWuXingHS.SetPushed(false);
	m_rioQsZuXuan24.SetPushed(false);
	m_rioQsZuXuan12.SetPushed(false);
	m_rioQsZuXuan6.SetPushed(false);
	m_rioQsZuXuan4.SetPushed(false);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	HideNiuNums();

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	HideGeNums();

	HideDanShiEdit();
}
//����ǰ��
void CChongQingSSC::OnBnClickedRioWuXingQsDan()
{
	if(m_gameKind == WuxingQianSi&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = WuxingQianSi;
	SendToServer(3);
	SetLockText();

	m_rioWuXingQS.SetPushed(false);
	m_rioWuXingQSDan.SetPushed(true);
	m_rioQsZuXuan24.SetPushed(false);
	m_rioQsZuXuan12.SetPushed(false);
	m_rioQsZuXuan6.SetPushed(false);
	m_rioQsZuXuan4.SetPushed(false);

	m_RenxuanKeyboardInput = true;

	HideNiuNums();

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:3456;��������:3456*,��������ǰ��ֱѡ��"));

}
//���Ǻ���
void CChongQingSSC::OnBnClickedRioWuXingHS()
{
	if(m_gameKind == WuxingHousi&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = WuxingHousi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingHSDan.SetPushed(false);
	m_rioWuXingHS.SetPushed(true);

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideDanShiEdit();
}
//���Ǻ��ĵ�ʽ
void CChongQingSSC::OnBnClickedRioWuXingHSDan()
{
	if(m_gameKind == WuxingHousi&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = WuxingHousi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingHS.SetPushed(false);
	m_rioWuXingHSDan.SetPushed(true);

	m_RenxuanKeyboardInput=true;
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();

	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:3456;��������:*3456,�������Ǻ���ֱѡ��"));

}
//��������
void CChongQingSSC::OnBnClickedRioWuXingZS()
{
	if(m_gameKind == WuxingZhongsan&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = WuxingZhongsan;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZS.SetPushed(true);
	m_rioWuXingZSDan.SetPushed(false);
	m_rioZsZsFuShi.SetPushed(false);
	m_rioZsZsDanShi.SetPushed(false);
	m_rioZsZlFuShi.SetPushed(false);
	m_rioZsZlDanShi.SetPushed(false);
	m_rioZsHunhe.SetPushed(false);


	m_RenxuanKeyboardInput = false;
	

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	HideGeNums();

	HideDanShiEdit();
}
//����������ʽ
void CChongQingSSC::OnBnClickedRioWuXingZSDan()
{
	if(m_gameKind == WuxingZhongsan&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = WuxingZhongsan;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZS.SetPushed(false);
	m_rioWuXingZSDan.SetPushed(true);
	m_rioZsZsFuShi.SetPushed(false);
	m_rioZsZsDanShi.SetPushed(false);
	m_rioZsZlFuShi.SetPushed(false);
	m_rioZsZlDanShi.SetPushed(false);
	m_rioZsHunhe.SetPushed(false);


	m_RenxuanKeyboardInput = true;
	

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:345;��������:*345*,������������ֱѡ��"));

}

//����ֱѡ��ʽ
void CChongQingSSC::OnBnClickedRioHszhxDanshi()
{
	if(m_gameKind == HouSanZhiXuan_FuShi&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = HouSanZhiXuan_FuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioHsZhxFuShi.SetPushed(false);
	m_rioHsZhxDanShi.SetPushed(true);
	m_rioHsZsFuShi.SetPushed(false);
	m_rioHsZsDanShi.SetPushed(false);
	m_rioHsZlFuShi.SetPushed(false);
	m_rioHsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);
	
	m_RenxuanKeyboardInput=true;
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:345;��������:**345,�������Ǻ���ֱѡ��"));

}

//ǰ��ֱѡ��ʽ
void CChongQingSSC::OnBnClickedRioQszhxFushi()
{
	if(m_gameKind == QianSanZhiXuan_FuShi&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = QianSanZhiXuan_FuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsZhxFuShi.SetPushed(true);
	m_rioQsZhxDanShi.SetPushed(false);
	m_rioQsZsFuShi.SetPushed(false);
	m_rioQsZsDanShi.SetPushed(false);
	m_rioQsZlFuShi.SetPushed(false);
	m_rioQsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);
	m_RenxuanKeyboardInput = false;
	

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}

//ǰ��ֱѡ��ʽ
void CChongQingSSC::OnBnClickedRioQszhxDanshi()
{
	if(m_gameKind == QianSanZhiXuan_FuShi&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = QianSanZhiXuan_FuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsZhxFuShi.SetPushed(false);
	m_rioQsZhxDanShi.SetPushed(true);
	m_rioQsZsFuShi.SetPushed(false);
	m_rioQsZsDanShi.SetPushed(false);
	m_rioQsZlFuShi.SetPushed(false);
	m_rioQsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);
	m_RenxuanKeyboardInput = true;

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:345;��������:345**,��������ǰ��ֱѡ��"));

}

//��ѡ2
void CChongQingSSC::OnBnClickedRioRenXuan2()
{
	if(m_gameKind == RenXuan2&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = RenXuan2;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(true);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);

	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	HideNiuNums();

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
	HideRenxuanWeiShu();
	HideDanShiEdit();
}
//��ѡ2
void CChongQingSSC::OnBnClickedRioRenXuan2Dan()
{
	if(m_gameKind == RenXuan2&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = RenXuan2;
	m_RenxuanKeyboardInput=true;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(true);

	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);

	HideNiuNums();

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	ShowRenxuanWeiShu();
	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע����ѡ���Ӧλ��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;���������ӦͶע������ѡ��λ�ã�5,8,������ѡ��һ�Ƚ���"));

}

void CChongQingSSC::OnBnClickedRioRenXuan3()
{
	if(m_gameKind == RenXuan3&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = RenXuan3;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(true);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_RenxuanKeyboardInput = false;
	
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideRenxuanWeiShu();
	HideDanShiEdit();
}
void CChongQingSSC::OnBnClickedRioRenXuan3Dan()
{
	if(m_gameKind == RenXuan3&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = RenXuan3;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(true);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);
	m_RenxuanKeyboardInput = true;

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowRenxuanWeiShu();
	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨�������ֶ����빺����룬3������Ϊһע����ѡ����������϶�Ӧ��ѡλ�ü��н���"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,8;���������ӦͶע������ѡ��λ�ã�2,5,8,������ѡ��һ�Ƚ���"));

}
void CChongQingSSC::OnBnClickedRioRenXuan3Zx3()
{
	if(m_gameKind == Ren3Zu3_Fushi&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Ren3Zu3_Fushi;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(true);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();

	ShowRenxuanWeiShu();
	HideDanShiEdit();
}
void CChongQingSSC::OnBnClickedRioRenXuan3Zx3Dan()
{
	if(m_gameKind == Ren3Zu3_Fushi&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Ren3Zu3_Fushi;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(true);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);

	m_RenxuanKeyboardInput = true;
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();

	ShowRenxuanWeiShu();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������λ��ǧλ����λ��ʮλ����λ������ѡ������λ�ã������ֶ�����1����λ���������һע������λ���б�������λ��ͬ��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,5;���������ӦͶע������ѡ��λ�ã�2,5,5,������������һ�Ƚ���"));

}
void CChongQingSSC::OnBnClickedRioRenXuan2Zx()
{
	if(m_gameKind == Ren2Zuxuan_Fushi&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Ren2Zuxuan_Fushi;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(true);

	m_rioRenXuan3Hunhe.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();

	ShowRenxuanWeiShu();
	HideDanShiEdit();

}
void CChongQingSSC::OnBnClickedRioRenXuan2ZxDan()
{
	if(m_gameKind == Ren2Zuxuan_Fushi&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Ren2Zuxuan_Fushi;
	m_RenxuanKeyboardInput=true;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(true);

	m_rioRenXuan3Hunhe.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();


	ShowRenxuanWeiShu();
	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������λ��ǧλ����λ��ʮλ����λ������ѡ������λ�ã������ֶ�����һ����λ���ĺ��빹��һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5;���������ӦͶע������ѡ��λ�ã�2,5,�����ζ���ѡһ�Ƚ���"));

}
void CChongQingSSC::OnBnClickedRioRenXuan3Zx6()
{
	if(m_gameKind == Ren3Zu6_Fushi&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Ren3Zu6_Fushi;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(true);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();

	ShowRenxuanWeiShu();
	HideDanShiEdit();
}
void CChongQingSSC::OnBnClickedRioRenXuan3Zx6Dan()
{
	if(m_gameKind == Ren3Zu6_Fushi&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Ren3Zu6_Fushi;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(true);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);

	m_RenxuanKeyboardInput=true;
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();


	ShowRenxuanWeiShu();
	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������λ��ǧλ����λ��ʮλ����λ������ѡ������λ�ã������ֶ�����1������ͬ����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,8;���������ӦͶע������ѡ��λ�ã�2,5,8,������������һ�Ƚ���"));

}
void CChongQingSSC::OnBnClickedRioRenXuan3Hunhe()
{
	if(m_gameKind == Ren3Hunhe_Zuxuan&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();
	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);

	m_gameKind = Ren3Hunhe_Zuxuan;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(true);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	m_rioMouseInput.ShowWindow(SW_HIDE);
	m_rioKeyboardInput.ShowWindow(SW_HIDE);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	//ShowZuXuan();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();

	ShowRenxuanWeiShu();
	//HideDanShiEdit();
}

void CChongQingSSC::OnBnClickedRioRenXuan4()
{
	if(m_gameKind == RenXuan4&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = RenXuan4;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	HideNiuNums();

	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan4.SetPushed(true);
	m_rioRenXuan4Dan.SetPushed(false);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);

	m_RenxuanKeyboardInput = false;
	

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideRenxuanWeiShu();
	HideDanShiEdit();
}
//��ѡ4��ʽ
void CChongQingSSC::OnBnClickedRioRenXuan4Dan()
{
	if(m_gameKind == RenXuan4&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = RenXuan4;
	m_RenxuanKeyboardInput=false;
	SetLockText();
	SendToServer(3);
	HideNiuNums();

	m_rioRenXuan2.SetPushed(false);
	m_rioRenXuan2Dan.SetPushed(false);
	m_rioRenXuan3.SetPushed(false);
	m_rioRenXuan3Dan.SetPushed(false);
	m_rioRenXuan4.SetPushed(false);
	m_rioRenXuan4Dan.SetPushed(true);
	m_rioRenXuan3Zx3.SetPushed(false);
	m_rioRenXuan3Zx3Dan.SetPushed(false);
	m_rioRenXuan3Zx6.SetPushed(false);
	m_rioRenXuan3Zx6Dan.SetPushed(false);
	m_rioRenXuan2Zx.SetPushed(false);
	m_rioRenXuan2ZxDan.SetPushed(false);
	m_rioRenXuan3Hunhe.SetPushed(false);

	m_RenxuanKeyboardInput = true;
	

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();

	ShowRenxuanWeiShu();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨�������ֶ����빺����룬4������Ϊһע����ѡ����������϶�Ӧ��ѡλ�ü��н���"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:0,2,5,8;���������ӦͶע������ѡ��λ�ã�0,2,5,8,������ѡ��һ�Ƚ���"));

}

//����������ʽ
void CChongQingSSC::OnBnClickedRioHszsFushi()
{
	if(m_gameKind == HouSanZuXuan_ZuSanFuShi&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = HouSanZuXuan_ZuSanFuShi;

	SetLockText();
	SendToServer(3);
	HideNiuNums();

	m_rioHsZhxFuShi.SetPushed(false);
	m_rioHsZhxDanShi.SetPushed(false);
	m_rioHsZsFuShi.SetPushed(true);
	m_rioHsZsDanShi.SetPushed(false);
	m_rioHsZlFuShi.SetPushed(false);
	m_rioHsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);

	m_RenxuanKeyboardInput = false;

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}

//����������ʽ
void CChongQingSSC::OnBnClickedRioHszsDanshi()
{
	if(m_gameKind == HouSanZuXuan_ZuSanDanShi)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = HouSanZuXuan_ZuSanDanShi;

	SetLockText();
	HideNiuNums();

	SendToServer(3);
	m_rioHsZhxFuShi.SetPushed(false);
	m_rioHsZhxDanShi.SetPushed(false);
	m_rioHsZsFuShi.SetPushed(false);
	m_rioHsZsDanShi.SetPushed(true);
	m_rioHsZlFuShi.SetPushed(false);
	m_rioHsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);

	m_RenxuanKeyboardInput=true;
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע������λ���б�������λ��ͬ��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8,8;�����������λ:1��5��2��8(˳����),���к�����ѡ��һ�Ƚ���"));

}

//����������ʽ
void CChongQingSSC::OnBnClickedRioHszlFushi()
{
	if(m_gameKind == HouSanZuXuan_ZuLiuFuShi&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = HouSanZuXuan_ZuLiuFuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioHsZhxFuShi.SetPushed(false);
	m_rioHsZhxDanShi.SetPushed(false);
	m_rioHsZsFuShi.SetPushed(false);
	m_rioHsZsDanShi.SetPushed(false);
	m_rioHsZlFuShi.SetPushed(true);
	m_rioHsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);

	m_RenxuanKeyboardInput = false;
	

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();


	HideDanShiEdit();
}
//����������ʽ
void CChongQingSSC::OnBnClickedRioZszlFushi()
{
	if(m_gameKind == ZhongsanZuliu&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = ZhongsanZuliu;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZS.SetPushed(false);
	m_rioWuXingZSDan.SetPushed(false);
	m_rioZsZsFuShi.SetPushed(false);
	m_rioZsZsDanShi.SetPushed(false);
	m_rioZsZlFuShi.SetPushed(true);
	m_rioZsZlDanShi.SetPushed(false);
	m_rioZsHunhe.SetPushed(false);

	m_RenxuanKeyboardInput = false;

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}
//����������ʽ
void CChongQingSSC::OnBnClickedRioZszlDanshi()
{
	if(m_gameKind == ZhongsanZuliu&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = ZhongsanZuliu;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZS.SetPushed(false);
	m_rioWuXingZSDan.SetPushed(false);
	m_rioZsZsFuShi.SetPushed(false);
	m_rioZsZsDanShi.SetPushed(false);
	m_rioZsZlFuShi.SetPushed(false);
	m_rioZsZlDanShi.SetPushed(true);
	m_rioZsHunhe.SetPushed(false);

	m_RenxuanKeyboardInput = true;
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	//HideZuXuan();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();

	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1������ͬ����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,8;���������м���λ:1��2��1��5��1��8(˳����),����������ѡ��һ�Ƚ���"));

}
//����������ʽ
void CChongQingSSC::OnBnClickedRioZszsFushi()
{
	if(m_gameKind == ZhongsanZusan)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = ZhongsanZusan;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZS.SetPushed(false);
	m_rioWuXingZSDan.SetPushed(false);
	m_rioZsZsFuShi.SetPushed(true);
	m_rioZsZsDanShi.SetPushed(false);
	m_rioZsZlFuShi.SetPushed(false);
	m_rioZsZlDanShi.SetPushed(false);
	m_rioZsHunhe.SetPushed(false);

	m_RenxuanKeyboardInput = false;

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}
//����������ʽ
void CChongQingSSC::OnBnClickedRioZszsDanshi()
{
	if(m_gameKind == Zhongsanzusandan)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Zhongsanzusandan;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZS.SetPushed(false);
	m_rioWuXingZSDan.SetPushed(false);
	m_rioZsZsFuShi.SetPushed(false);
	m_rioZsZsDanShi.SetPushed(true);
	m_rioZsZlFuShi.SetPushed(false);
	m_rioZsZlDanShi.SetPushed(false);
	m_rioZsHunhe.SetPushed(false);
	m_RenxuanKeyboardInput = true;


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	//HideZuXuan();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע������λ���б�������λ��ͬ��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8,8;���������м���λ:1��5��2��8(˳����),����������ѡ��һ�Ƚ���"));

}
//��ѡ120
void CChongQingSSC::OnBnClickedRioZuXuan120()
{
	if(m_gameKind == Zuxuan120&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Zuxuan120;

	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZX.SetPushed(false);
	m_rioZuXuan120.SetPushed(true);
	m_rioZuXuan60.SetPushed(false);
	m_rioZuXuan30.SetPushed(false);
	m_rioZuXuan20.SetPushed(false);
	m_rioZuXuan10.SetPushed(false);
	m_rioZuXuan5.SetPushed(false);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	m_rioWuXingZXDan.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	//ShowZuXuan();
	ShowZu120();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}
//��ѡ60
void CChongQingSSC::OnBnClickedRioZuXuan60()
{
	if(m_gameKind == Zuxuan60&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Zuxuan60;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZX.SetPushed(false);
	m_rioZuXuan120.SetPushed(false);
	m_rioZuXuan60.SetPushed(true);
	m_rioZuXuan30.SetPushed(false);
	m_rioZuXuan20.SetPushed(false);
	m_rioZuXuan10.SetPushed(false);
	m_rioZuXuan5.SetPushed(false);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	m_rioWuXingZXDan.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowErChong();
	ShowShiNums();
	ShowDanHao();
	HideGeNums();

	HideDanShiEdit();
}
//��ѡ30
void CChongQingSSC::OnBnClickedRioZuXuan30()
{
	if(m_gameKind == Zuxuan30&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Zuxuan30;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZX.SetPushed(false);
	m_rioZuXuan120.SetPushed(false);
	m_rioZuXuan60.SetPushed(false);
	m_rioZuXuan30.SetPushed(true);
	m_rioZuXuan20.SetPushed(false);
	m_rioZuXuan10.SetPushed(false);
	m_rioZuXuan5.SetPushed(false);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	m_rioWuXingZXDan.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowErChong();
	ShowShiNums();
	ShowDanHao();
	HideGeNums();

	HideDanShiEdit();
}

//��ѡ20
void CChongQingSSC::OnBnClickedRioZuXuan20()
{
	if(m_gameKind == Zuxuan20&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Zuxuan20;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZX.SetPushed(false);
	m_rioZuXuan120.SetPushed(false);
	m_rioZuXuan60.SetPushed(false);
	m_rioZuXuan30.SetPushed(false);
	m_rioZuXuan20.SetPushed(true);
	m_rioZuXuan10.SetPushed(false);
	m_rioZuXuan5.SetPushed(false);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	m_rioWuXingZXDan.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowSanChong();
	ShowShiNums();
	ShowDanHao();
	HideGeNums();

	HideDanShiEdit();
}
//��ѡ10
void CChongQingSSC::OnBnClickedRioZuXuan10()
{
	if(m_gameKind == Zuxuan10&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Zuxuan10;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZX.SetPushed(false);
	m_rioZuXuan120.SetPushed(false);
	m_rioZuXuan60.SetPushed(false);
	m_rioZuXuan30.SetPushed(false);
	m_rioZuXuan20.SetPushed(false);
	m_rioZuXuan10.SetPushed(true);
	m_rioZuXuan5.SetPushed(false);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	m_rioWuXingZXDan.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowSanChong();
	ShowShiNums();
	ShowErChong1();
	HideGeNums();

	HideDanShiEdit();
}
//��ѡ5
void CChongQingSSC::OnBnClickedRioZuXuan5()
{
	if(m_gameKind == Zuxuan5&& m_RenxuanKeyboardInput==false)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = Zuxuan5;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioWuXingZX.SetPushed(false);
	m_rioZuXuan120.SetPushed(false);
	m_rioZuXuan60.SetPushed(false);
	m_rioZuXuan30.SetPushed(false);
	m_rioZuXuan20.SetPushed(false);
	m_rioZuXuan10.SetPushed(false);
	m_rioZuXuan5.SetPushed(true);
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	m_rioWuXingZXDan.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowSiChong();
	ShowShiNums();
	ShowDanHao();
	HideGeNums();

	HideDanShiEdit();
}
//ǰ����ѡ24
void CChongQingSSC::OnBnClickedRioQsZuXuan24()
{
	m_gameKind = QSZuXuan24;

	ResetAllNums();

	SendToServer(3);
	SetLockText();

	m_rioWuXingQS.SetPushed(false);
	m_rioWuXingQSDan.SetPushed(false);
	m_rioQsZuXuan24.SetPushed(true);
	m_rioQsZuXuan12.SetPushed(false);
	m_rioQsZuXuan6.SetPushed(false);
	m_rioQsZuXuan4.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	//ShowZuXuan();
	ShowZu120();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();

}
//ǰ����ѡ12
void CChongQingSSC::OnBnClickedRioQsZuXuan12()
{
	m_gameKind = QSZuXuan12;
	ResetAllNums();

	SendToServer(3);
	SetLockText();

	m_rioWuXingQS.SetPushed(false);
	m_rioWuXingQSDan.SetPushed(false);
	m_rioQsZuXuan24.SetPushed(false);
	m_rioQsZuXuan12.SetPushed(true);
	m_rioQsZuXuan6.SetPushed(false);
	m_rioQsZuXuan4.SetPushed(false);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowErChong();
	ShowShiNums();
	ShowDanHao();
	HideGeNums();

	HideDanShiEdit();
}
//ǰ����ѡ6
void CChongQingSSC::OnBnClickedRioQsZuXuan6()
{
	m_gameKind = QSZuXuan6;
	ResetAllNums();

	SendToServer(3);
	SetLockText();

	m_rioWuXingQS.SetPushed(false);
	m_rioWuXingQSDan.SetPushed(false);
	m_rioQsZuXuan24.SetPushed(false);
	m_rioQsZuXuan12.SetPushed(false);
	m_rioQsZuXuan6.SetPushed(true);
	m_rioQsZuXuan4.SetPushed(false);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowSxErChong();
	HideShiNums();
	//ShowDanHao();
	HideGeNums();

	HideDanShiEdit();

}
//ǰ����ѡ4
void CChongQingSSC::OnBnClickedRioQsZuXuan4()
{
	m_gameKind = QSZuXuan4;
	ResetAllNums();

	SendToServer(3);
	SetLockText();

	m_rioWuXingQS.SetPushed(false);
	m_rioWuXingQSDan.SetPushed(false);
	m_rioQsZuXuan24.SetPushed(false);
	m_rioQsZuXuan12.SetPushed(false);
	m_rioQsZuXuan6.SetPushed(false);
	m_rioQsZuXuan4.SetPushed(true);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowSanChong();
	ShowShiNums();
	ShowDanHao();
	HideGeNums();

	HideDanShiEdit();

}
//������ѡ24
void CChongQingSSC::OnBnClickedRioHsZuXuan24()
{
	m_gameKind = HSZuXuan24;

	ResetAllNums();

	SendToServer(3);
	SetLockText();

	m_rioWuXingHS.SetPushed(false);
	m_rioWuXingHSDan.SetPushed(false);
	m_rioHsZuXuan24.SetPushed(true);
	m_rioHsZuXuan12.SetPushed(false);
	m_rioHsZuXuan6.SetPushed(false);
	m_rioHsZuXuan4.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	//ShowZuXuan();
	ShowZu120();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();

}
//ǰ����ѡ12
void CChongQingSSC::OnBnClickedRioHsZuXuan12()
{
	m_gameKind = HSZuXuan12;
	ResetAllNums();

	SendToServer(3);
	SetLockText();

	m_rioWuXingHS.SetPushed(false);
	m_rioWuXingHSDan.SetPushed(false);
	m_rioHsZuXuan24.SetPushed(false);
	m_rioHsZuXuan12.SetPushed(true);
	m_rioHsZuXuan6.SetPushed(false);
	m_rioHsZuXuan4.SetPushed(false);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowErChong();
	ShowShiNums();
	ShowDanHao();
	HideGeNums();

	HideDanShiEdit();
}
//ǰ����ѡ6
void CChongQingSSC::OnBnClickedRioHsZuXuan6()
{
	m_gameKind = HSZuXuan6;
	ResetAllNums();

	SendToServer(3);
	SetLockText();

	m_rioWuXingHS.SetPushed(false);
	m_rioWuXingHSDan.SetPushed(false);
	m_rioHsZuXuan24.SetPushed(false);
	m_rioHsZuXuan12.SetPushed(false);
	m_rioHsZuXuan6.SetPushed(true);
	m_rioHsZuXuan4.SetPushed(false);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowSxErChong();
	HideShiNums();
	//ShowDanHao();
	HideGeNums();

	HideDanShiEdit();

}
//ǰ����ѡ4
void CChongQingSSC::OnBnClickedRioHsZuXuan4()
{
	m_gameKind = HSZuXuan4;
	ResetAllNums();

	SendToServer(3);
	SetLockText();

	m_rioWuXingHS.SetPushed(false);
	m_rioWuXingHSDan.SetPushed(false);
	m_rioHsZuXuan24.SetPushed(false);
	m_rioHsZuXuan12.SetPushed(false);
	m_rioHsZuXuan6.SetPushed(false);
	m_rioHsZuXuan4.SetPushed(true);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowSanChong();
	ShowShiNums();
	ShowDanHao();
	HideGeNums();

	HideDanShiEdit();

}
//����������ʽ
void CChongQingSSC::OnBnClickedRioHszlDanshi()
{
	if(m_gameKind == HouSanZuXuan_ZuLiuFuShi&& m_RenxuanKeyboardInput==true)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = HouSanZuXuan_ZuLiuFuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();
	m_RenxuanKeyboardInput = true;
	m_rioHsZhxFuShi.SetPushed(false);
	m_rioHsZhxDanShi.SetPushed(false);
	m_rioHsZsFuShi.SetPushed(false);
	m_rioHsZsDanShi.SetPushed(false);
	m_rioHsZlFuShi.SetPushed(false);
	m_rioHsZlDanShi.SetPushed(true);
	m_rioQsHunhe.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1������ͬ����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,8;�����������λ:1��2��1��5��1��8(˳����),���к�����ѡ��һ�Ƚ���"));

}

//ǰ��������ʽ
void CChongQingSSC::OnBnClickedRioQszsFushi()
{
	if(m_gameKind == QianSanZuXuan_ZuSanFuShi)
	{
		return;
	}

	ResetAllNums();

	m_gameKind = QianSanZuXuan_ZuSanFuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsZhxFuShi.SetPushed(false);
	m_rioQsZhxDanShi.SetPushed(false);
	m_rioQsZsFuShi.SetPushed(true);
	m_rioQsZsDanShi.SetPushed(false);
	m_rioQsZlFuShi.SetPushed(false);
	m_rioQsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);
	m_RenxuanKeyboardInput = false;

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}

//ǰ��������ʽ
void CChongQingSSC::OnBnClickedRioQszsDanshi()
{
	ResetAllNums();

	m_gameKind = QianSanZuXuan_ZuSanDanShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsZhxFuShi.SetPushed(false);
	m_rioQsZhxDanShi.SetPushed(false);
	m_rioQsZsFuShi.SetPushed(false);
	m_rioQsZsDanShi.SetPushed(true);
	m_rioQsZlFuShi.SetPushed(false);
	m_rioQsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);
	m_RenxuanKeyboardInput = true;

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע������λ���б�������λ��ͬ��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8,8;��������ǰ��λ:1��5��2��8(˳����),����ǰ����ѡ��һ�Ƚ���"));

}

//ǰ��������ʽ
void CChongQingSSC::OnBnClickedRioQszlFushi()
{
	ResetAllNums();

	m_gameKind = QianSanZuXuan_ZuLiuFuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsZhxFuShi.SetPushed(false);
	m_rioQsZhxDanShi.SetPushed(false);
	m_rioQsZsFuShi.SetPushed(false);
	m_rioQsZsDanShi.SetPushed(false);
	m_rioQsZlFuShi.SetPushed(true);
	m_rioQsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(false);
	m_RenxuanKeyboardInput = false;

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}

//ǰ��������ʽ
void CChongQingSSC::OnBnClickedRioQszlDanshi()
{
	ResetAllNums();

	m_gameKind = QianSanZuXuan_ZuLiuFuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsZhxFuShi.SetPushed(false);
	m_rioQsZhxDanShi.SetPushed(false);
	m_rioQsZsFuShi.SetPushed(false);
	m_rioQsZsDanShi.SetPushed(false);
	m_rioQsZlFuShi.SetPushed(false);
	m_rioQsZlDanShi.SetPushed(true);
	m_rioQsHunhe.SetPushed(false);
	m_RenxuanKeyboardInput = true;


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1������ͬ����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:258;��������ǰ��λ:1��2��1��5��1��8(˳����),����ǰ����ѡ��һ�Ƚ���"));

}

//���ֱѡ��ʽ
void CChongQingSSC::OnBnClickedRioHezhxFushi()
{
	ResetAllNums();

	m_gameKind = WuXingHouEr_FuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(false);
	m_rioHeZhxFuShi.SetPushed(true);
	m_rioHeZhxDanShi.SetPushed(false);
	m_rioQeZxFuShi.SetPushed(false);
	m_rioQeZxDanShi.SetPushed(false);
	m_rioHeZxFuShi.SetPushed(false);
	m_rioHeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideDanShiEdit();
}

//���ֱѡ��ʽ
void CChongQingSSC::OnBnClickedRioHezhxDanshi()
{
	ResetAllNums();

	m_gameKind = WuXingHouEr_DanShi;

	SetLockText();
	HideNiuNums();
	SendToServer(3);

	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(false);
	m_rioHeZhxFuShi.SetPushed(false);
	m_rioHeZhxDanShi.SetPushed(true);
	m_rioQeZxFuShi.SetPushed(false);
	m_rioQeZxDanShi.SetPushed(false);
	m_rioHeZxFuShi.SetPushed(false);
	m_rioHeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;����������λ:58,���к��ֱѡһ�Ƚ���"));

}

//�����ѡ��ʽ
void CChongQingSSC::OnBnClickedRioHezxFushi()
{
	ResetAllNums();

	m_gameKind = HouErZuXuan_FuShi;
	HideNiuNums();
	SendToServer(3);
	SetLockText();
	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(false);
	m_rioHeZhxFuShi.SetPushed(false);
	m_rioHeZhxDanShi.SetPushed(false);

	m_rioQeZxFuShi.SetPushed(false);
	m_rioQeZxDanShi.SetPushed(false);
	m_rioHeZxFuShi.SetPushed(true);
	m_rioHeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}

//�����ѡ��ʽ
void CChongQingSSC::OnBnClickedRioHezxDanshi()
{
	ResetAllNums();

	m_gameKind = HouErZuXuan_DanShi;
	SendToServer(3);
	
	HideNiuNums();
	SetLockText();
	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(false);
	m_rioHeZhxFuShi.SetPushed(false);
	m_rioHeZhxDanShi.SetPushed(false);

	m_rioQeZxFuShi.SetPushed(false);
	m_rioQeZxDanShi.SetPushed(false);
	m_rioHeZxFuShi.SetPushed(false);
	m_rioHeZxDanShi.SetPushed(true);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;�����������λ:1��5��1��8(˳����),���к����ѡһ�Ƚ���"));

}

//ǰ��ֱѡ��ʽ
void CChongQingSSC::OnBnClickedRioQezhxFushi()
{
	ResetAllNums();

	m_gameKind = WuXingQianEr_FuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQeZhxFuShi.SetPushed(true);
	m_rioQeZhxDanShi.SetPushed(false);
	m_rioHeZhxFuShi.SetPushed(false);
	m_rioHeZhxDanShi.SetPushed(false);
	m_rioQeZxFuShi.SetPushed(false);
	m_rioQeZxDanShi.SetPushed(false);
	m_rioHeZxFuShi.SetPushed(false);
	m_rioHeZxDanShi.SetPushed(false);


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}

//ǰ��ֱѡ��ʽ
void CChongQingSSC::OnBnClickedRioQezhxDanshi()
{
	ResetAllNums();

	m_gameKind = WuXingQianEr_DanShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(true);
	m_rioHeZhxFuShi.SetPushed(false);
	m_rioHeZhxDanShi.SetPushed(false);
	m_rioQeZxFuShi.SetPushed(false);
	m_rioQeZxDanShi.SetPushed(false);
	m_rioHeZxFuShi.SetPushed(false);
	m_rioHeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;��������ǰ��λ:58,����ǰ��ֱѡһ�Ƚ���"));

}

//ǰ����ѡ��ʽ
void CChongQingSSC::OnBnClickedRioQezxFushi()
{
	ResetAllNums();

	m_gameKind = QianErZuXuan_FuShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();
	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(false);
	m_rioHeZhxFuShi.SetPushed(false);
	m_rioHeZhxDanShi.SetPushed(false);

	m_rioQeZxFuShi.SetPushed(true);
	m_rioQeZxDanShi.SetPushed(false);
	m_rioHeZxFuShi.SetPushed(false);
	m_rioHeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}

//ǰ����ѡ��ʽ
void CChongQingSSC::OnBnClickedRioQezxDanshi()
{
	ResetAllNums();

	m_gameKind = QianErZuXuan_DanShi;
	SendToServer(3);
	SetLockText();
	HideNiuNums();
	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(false);
	m_rioHeZhxFuShi.SetPushed(false);
	m_rioHeZhxDanShi.SetPushed(false);

	m_rioQeZxFuShi.SetPushed(false);
	m_rioQeZxDanShi.SetPushed(true);
	m_rioHeZxFuShi.SetPushed(false);
	m_rioHeZxDanShi.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
	m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;��������ǰ��λ:1��5��1��8(˳����),����ǰ����ѡһ�Ƚ���"));

}

//��С��˫
void CChongQingSSC::OnBnClickedRioDxdsh()
{
	ResetAllNums();

	m_gameKind = DaXiaoDanShuang;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQeDxdsh.SetPushed(true);
	m_rioDwDxdsh.SetPushed(false);
	m_rioHzhDxdsh.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	ShowDaXiaoDanShuang();
	HideDanShiEdit();
}
//��С��˫ ��λ
void CChongQingSSC::OnBnClickedRioDxdshDw()
{
	ResetAllNums();

	m_gameKind = DaXiaoDanShuang_dw;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQeDxdsh.SetPushed(false);
	m_rioDwDxdsh.SetPushed(true);
	m_rioHzhDxdsh.SetPushed(false);

	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	ShowDaXiaoDanShuangDw();

	HideDanShiEdit();
}
//��С��˫ ��λ
void CChongQingSSC::OnBnClickedRioDxdshHzh()
{
	ResetAllNums();

	m_gameKind = Zonghe_DXDSLh;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQeDxdsh.SetPushed(false);
	m_rioDwDxdsh.SetPushed(false);
	m_rioHzhDxdsh.SetPushed(true);

	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	HideDaXiaoDanShuangDw();
	HideDanShiEdit();
	ShowDaXiaoDanShuangHzh();
}

//ǰ������λ
void CChongQingSSC::OnBnClickedRioQsbdw()
{
	ResetAllNums();

	m_gameKind = BuDingWei_QianSan;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsbdw.SetPushed(true);
	m_rioZsbdw.SetPushed(false);
	m_rioHsbdw.SetPushed(false);
	m_rioQebdw.SetPushed(false);
	m_rioHebdw.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowBuDingWei();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}
//��������λ
void CChongQingSSC::OnBnClickedRioZsbdw()
{
	ResetAllNums();

	m_gameKind = BuDingWei_ZhongSan;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsbdw.SetPushed(false);
	m_rioZsbdw.SetPushed(true);
	m_rioHsbdw.SetPushed(false);
	m_rioQebdw.SetPushed(false);
	m_rioHebdw.SetPushed(false);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowBuDingWei();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}

//��������λ
void CChongQingSSC::OnBnClickedRioHsbdw()
{
	ResetAllNums();

	m_gameKind = BuDingWei_HouSan;
	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsbdw.SetPushed(false);
	m_rioZsbdw.SetPushed(false);

	m_rioHsbdw.SetPushed(true);
	m_rioQebdw.SetPushed(false);
	m_rioHebdw.SetPushed(false);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowBuDingWei();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();
}

//ǰ�����
void CChongQingSSC::OnBnClickedRioQsHunhe()
{
	ResetAllNums();
	SetLockText();
	m_rioQsZhxFuShi.SetPushed(false);
	m_rioQsZhxDanShi.SetPushed(false);
	m_rioQsZsFuShi.SetPushed(false);
	m_rioQsZsDanShi.SetPushed(false);
	m_rioQsZlFuShi.SetPushed(false);
	m_rioQsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(true);

	HideNiuNums();

	m_gameKind = QianSanHunhe;

	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	SendToServer(3);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	m_RenxuanKeyboardInput = true;
	ShowDanShiEdit();
}


//�������
void CChongQingSSC::OnBnClickedRioZsHunhe()
{
	ResetAllNums();
	SetLockText();
	m_rioWuXingZS.SetPushed(false);
	m_rioWuXingZSDan.SetPushed(false);
	m_rioZsZsFuShi.SetPushed(false);
	m_rioZsZsDanShi.SetPushed(false);
	m_rioZsZlFuShi.SetPushed(false);
	m_rioZsZlDanShi.SetPushed(false);
	m_rioZsHunhe.SetPushed(true);
	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);

	m_gameKind = ZhongSanHunhe;
	SendToServer(3);
	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideNiuNums();

	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();
	m_RenxuanKeyboardInput = true;
	ShowDanShiEdit();
}



//�������
void CChongQingSSC::OnBnClickedRioHsHunhe()
{
	ResetAllNums();
	SetLockText();
	m_RenxuanKeyboardInput = true;
	m_rioHsZhxFuShi.SetPushed(false);
	m_rioHsZhxDanShi.SetPushed(false);
	m_rioHsZsFuShi.SetPushed(false);
	m_rioHsZsDanShi.SetPushed(false);
	m_rioHsZlFuShi.SetPushed(false);
	m_rioHsZlDanShi.SetPushed(false);
	m_rioQsHunhe.SetPushed(true);
	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);

	m_gameKind = HouSanHunhe;
	SendToServer(3);
	HideNiuNums();

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
}

//��λ��
void CChongQingSSC::OnBnClickedRioDwd()
{
	ResetAllNums();

	m_gameKind = DingWeiDan;
SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioDwd.SetPushed(true);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	ShowWanNums();
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideDanShiEdit();
}
//�������λ
void CChongQingSSC::OnBnClickedRioHebdw()
{
	ResetAllNums();

	m_gameKind = BuDingWei_HouEr;
SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsbdw.SetPushed(false);
	m_rioZsbdw.SetPushed(false);

	m_rioHsbdw.SetPushed(false);
	m_rioQebdw.SetPushed(false);
	m_rioHebdw.SetPushed(true);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowBuDingWei();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();

}
//ǰ������λ
void CChongQingSSC::OnBnClickedRioQebdw()
{
	ResetAllNums();

	m_gameKind = BuDingWei_QianEr;
SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioQsbdw.SetPushed(false);
	m_rioZsbdw.SetPushed(false);

	m_rioHsbdw.SetPushed(false);
	m_rioQebdw.SetPushed(true);
	m_rioHebdw.SetPushed(false);


	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	ShowBaiNums();
	ShowBuDingWei();
	HideShiNums();
	HideGeNums();

	HideDanShiEdit();

}
void CChongQingSSC::OnBnClickedBtnMoreRecord()
{
	SendToServer(8);
	return;
}
void CChongQingSSC::OnBnClickedBtnQuShi()
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
	_sntprintf(szUrl,CountArray(szUrl),TEXT("%s/TrendCharts/ssc/c5x_basic.aspx?type=%d"),szTodayURL,m_TypeID);
	ShellExecute(NULL, _T("open"), szUrl, NULL, NULL, SW_SHOWNORMAL);
	return;
}

//׷��
void CChongQingSSC::OnBnClickedBtnZhuihao()
{
	if(m_cbIfTs == 1)
	{
		MyMessageBox(L"�˲�����ͣ���ۣ�");
		return;
	}
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
	else if(theAccount.Scoretype == MoShi_Li)
		danzhujine = 0.002;

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

	if(m_TypeID == CZChongQingSSC)
		dlgZhuiHao.SetCurrentQihaoTime(m_chqRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,8,3, fJine,120,m_TypeID);
 	else if(m_TypeID == CZ_TianJinSSC)
		dlgZhuiHao.SetCurrentQihaoTime( m_jxRule.GetNextExpect(-1), strBeishu,8,3, fJine,84,m_TypeID);
	else if(m_TypeID == CZXinJiangSSC)
	{
		dlgZhuiHao.SetCurrentQihaoTime( m_xjRule.GetNextExpect(-1), strBeishu,8,2, fJine,96,CZXinJiangSSC);
	}
	else if(m_TypeID == CZ_FENFEN_CAI)
	{
		dlgZhuiHao.SetCurrentQihaoTime(m_ffcRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,8,4, fJine,1440,CZ_FENFEN_CAI);
	}
	else if(m_TypeID == CZ_TXfenfencai)//1205
	{
		dlgZhuiHao.SetCurrentQihaoTime(m_txffcRule.GetNextExpect(-1), strBeishu,8,4, fJine,1440,CZ_TXfenfencai);
	}
	else if(m_TypeID == CZ_QQfenfencai)//1205
	{
		dlgZhuiHao.SetCurrentQihaoTime(m_qqffcRule.GetNextExpect(-1), strBeishu,8,4, fJine,1440,CZ_QQfenfencai);
	}
	else if(m_TypeID == CZ_ErFenCai) //9.28
	{
		dlgZhuiHao.SetCurrentQihaoTime(m_efcRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,8,3, fJine,720,CZ_ErFenCai);
	}
	else if(m_TypeID == CZ_WUFEN_CAI)
	{
		dlgZhuiHao.SetCurrentQihaoTime(m_wfcRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,8,3, fJine,288,CZ_WUFEN_CAI);
	}
	else if(m_TypeID == CZ_BJ5FC)
	{
		dlgZhuiHao.SetCurrentQihaoTime(m_bj5fcRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,0,6, fJine,1000000000,CZ_BJ5FC);
	}
	else if(m_TypeID == CZ_HGYDWFC)
	{
		long kjing = GetKjShjDiff(); 
		if (kjing <= 0)								// || kjing > 540
		{
			MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
			return ;
		}

		dlgZhuiHao.SetCurrentQihaoTime(m_hgRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,0,7, fJine,9999999999999,CZ_HGYDWFC);

	}
	else if(m_TypeID == CZ_JiaNaDaSSC)
	{
		long kjing = GetKjShjDiff(); 
		if (kjing <= 0)								// || kjing > 540
		{
			MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
			return ;
		}
	//	int nCount = m_jndRule.GetZhuihaoQishu();
		
		if(m_nCanadaStartQihao== 0)
		{
			MyMessageBox(L"��ͣ���ۣ����Ժ����ԣ�");
			return;
		}
		dlgZhuiHao.SetCurrentQihaoTime(m_jndRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,0,7, fJine,999999999,CZ_JiaNaDaSSC);

	}
	if(dlgZhuiHao.DoModal() == IDOK)
	{
		if(m_cbIfTs == 1)
		{
			MyMessageBox(L"�˲�����ͣ���ۣ�");
			return;
		}

		if(m_TypeID == CZ_TXfenfencai||m_TypeID == CZ_QQfenfencai)
		{
			int nCha = GetTXQihaoCha();
			if(nCha==2)
			{
				MyMessageBox(L"ά��ʱ�䣬���Ժ���ע��");
				return;
			}
		}

		long kjing = GetKjShjDiff(); 
		int nSecond = 0;
		if(m_TypeID == CZChongQingSSC)
			nSecond = 35;
		else if(m_TypeID == CZ_WUFEN_CAI)
			nSecond = 45;
		else if (m_TypeID == CZ_FENFEN_CAI)
		{
			nSecond = 5;
		}
		else if(m_TypeID == CZ_HGYDWFC)
		{
			nSecond = 0;
		}
		else if (m_TypeID == CZ_TXfenfencai||m_TypeID == CZ_QQfenfencai)
		{
			nSecond = 0;
		}
		else if(m_TypeID == CZ_JiaNaDaSSC)
		{
			nSecond = 45;
		}
		else if(m_TypeID == CZ_ErFenCai)
		{
			nSecond = 15;
		}
		else if (m_TypeID == CZ_TianJinSSC)
		{
			nSecond = 200;
		}
		else if (m_TypeID == CZXinJiangSSC)
		{
			nSecond = 60;
		}
		else if(m_TypeID == CZ_BJ5FC)
		{
			nSecond = 60;
		}

		if (kjing <= nSecond)								// || kjing > 540
		{
			MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
			return ;
		}


		m_nZhuihaoCount = dlgZhuiHao.GetZhuiHaoQiShu();
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

	//m_beishu = 1;
	//UpdateData(FALSE);
OnBnClickedBtnClsList();
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
}

//У������
bool	CChongQingSSC::CheckInput()
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
	if (m_strHaoma.IsEmpty() && vecAllNums.size()==0) 
	{
		MyMessageBox(_T("����δѡ�����"));
		return false;
	}

	//�ж�����Ƿ����
	double fCellPrice=0.0;
	if(theAccount.Scoretype == MoShi_Yuan)
		fCellPrice = 2.0;
	else if(theAccount.Scoretype == MoShi_Jiao)
		fCellPrice = 0.2;
	else if(theAccount.Scoretype == MoShi_Fen)
		fCellPrice = 0.02;
	else if(theAccount.Scoretype == MoShi_Li)
		fCellPrice = 0.002;
	if(theAccount.yue < m_zongJine)
	{
		MyMessageBox(_T("�����㣬���ֵ������"));
		return false;
	}
	//�ж��Ƿ��ٽ�������
	//���»�ȡ������ʱ��
	//theApp.GetTimeDiff();
	long kjing = GetKjShjDiff(); 
	int nSecond = 0;
	if(m_TypeID == CZChongQingSSC)
		nSecond = 35;
	else if(m_TypeID == CZ_WUFEN_CAI)
		nSecond = 45;
	else if (m_TypeID == CZ_FENFEN_CAI)
	{
		nSecond = 5;
	}
	else if(m_TypeID == CZ_HGYDWFC)
	{
		nSecond = 0;
	}
	else if (m_TypeID == CZ_TXfenfencai||m_TypeID == CZ_QQfenfencai)
	{
		nSecond = 0;
	}
	else if(m_TypeID == CZ_JiaNaDaSSC)
	{
		nSecond = 45;
	}
	else if(m_TypeID == CZ_ErFenCai)
	{
		nSecond = 15;
	}
	else if (m_TypeID == CZ_TianJinSSC)
	{
		nSecond = 140;
	}
	else if (m_TypeID == CZXinJiangSSC)
	{
		nSecond = 60;
	}
	else if (m_TypeID == CZ_BJ5FC)
	{
		nSecond = 60;
	}

	CString strLog;
	strLog.Format(L"CHQSSC2 touzhu:%d,nSecond:%d",kjing,nSecond);
	OutputDebugString(strLog);

	if (kjing <= nSecond)								// || kjing > 540
	{
		MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
		return false;
	}

	return true;
}
void CChongQingSSC::OnBnClickedBtnTouzhuOneKey()
{
	if(m_cbIfTs == 1)
	{
		MyMessageBox(L"�˲�����ͣ���ۣ�");
		return;
	}
	if (vecAllNums.size()<=0)
	{
		OnBnClickedBtnChqAdd();
	}
	if(m_zongZhuShu>0)
		OnBnClickedBtnTouzhuQ();
}
//��ͨ��ע
void CChongQingSSC::OnBnClickedBtnTouzhuQ()
{

	//У������
	if(!CheckInput())
		return;



	if(m_TypeID == CZ_TXfenfencai||m_TypeID == CZ_QQfenfencai)
	{
		int nCha = GetTXQihaoCha();
// 		if(nCha==2)
// 		{
// 			MyMessageBox(L"ά��ʱ�䣬���Ժ���ע��");
// 			return;
// 		}
	}


	bool bSucc = true;
	int itm_cnt = m_listChqSSC.GetItemCount();
	m_nTouzhuCount=0;
	m_nTouzhuCount = itm_cnt;


	SendToServer(1);


	m_zongZhuShu = 0;
	OnBnClickedBtnClsList();
	//m_beishu = 1;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

}

//��ͨ��ע
void CChongQingSSC::OnBnClickedBtnTouzhu()
{
	if(m_cbIfTs == 1)
	{
		MyMessageBox(L"�˲�����ͣ���ۣ�");
		return;
	}

	//У������
	if(!CheckInput())
		return;

	//���û�ȷ����Ϣ
	CQrTzhDlg qrDlg;
	if(m_TypeID == CZChongQingSSC)
		qrDlg.m_gamekind = _T("�����");
	else if(m_TypeID == CZ_TianJinSSC)
	{
		qrDlg.m_gamekind = _T("����");
		long kjing = m_jxRule.GetKjShjDiff(); 
		if(kjing<140)
		{
			MyMessageBox(L"���ڷⵥ��");
			return;
		}
	}
	else if(m_TypeID == CZXinJiangSSC)
		qrDlg.m_gamekind = _T("�½���");
	else if(m_TypeID == CZ_FENFEN_CAI)
		qrDlg.m_gamekind = _T("̨��ֲַ�");
	else if(m_TypeID == CZ_ErFenCai)
		qrDlg.m_gamekind = _T("ӡ����ֲ�");
	else if(m_TypeID == CZ_WUFEN_CAI)
		qrDlg.m_gamekind = _T("������ֲ�");
	else if(m_TypeID == CZ_BJ5FC)
		qrDlg.m_gamekind = _T("����5�ֲ�");
	else if(m_TypeID == CZ_HGYDWFC)
		qrDlg.m_gamekind = _T("����1.5�ֲ�");
	else if(m_TypeID == CZ_JiaNaDaSSC)
		qrDlg.m_gamekind = _T("���ô�3.5�ֲ�");
	else if(m_TypeID == CZ_TXfenfencai)
		qrDlg.m_gamekind = _T("��Ѷ�ֲַ�");
	else if(m_TypeID == CZ_QQfenfencai)
		qrDlg.m_gamekind = _T("QQ�ֲַ�");

	qrDlg.m_zhushu = m_zongZhuShu /** m_beishu*/;
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

	if(m_TypeID == CZ_TXfenfencai||m_TypeID == CZ_QQfenfencai)
	{
		int nCha = GetTXQihaoCha();
		if(nCha==2)
		{
			MyMessageBox(L"ά��ʱ�䣬���Ժ���ע��");
			return;
		}
	}
	long kjing = GetKjShjDiff(); 
	int nSecond = 0;
	if(m_TypeID == CZChongQingSSC)
		nSecond = 35;
	else if(m_TypeID == CZ_WUFEN_CAI)
		nSecond = 45;
	else if (m_TypeID == CZ_FENFEN_CAI)
	{
		nSecond = 5;
	}
	else if(m_TypeID == CZ_HGYDWFC)
	{
		nSecond = 0;
	}
	else if (m_TypeID == CZ_TXfenfencai||m_TypeID == CZ_QQfenfencai)
	{
		nSecond = 0;
	}
	else if(m_TypeID == CZ_JiaNaDaSSC)
	{
		nSecond = 45;
	}
	else if(m_TypeID == CZ_ErFenCai)
	{
		nSecond = 15;
	}
	else if (m_TypeID == CZ_TianJinSSC)
	{
		nSecond = 140;
	}
	else if (m_TypeID == CZXinJiangSSC)
	{
		nSecond = 60;
	}
	else if (m_TypeID == CZ_BJ5FC)
	{
		nSecond = 60;
	}

	if (kjing <= nSecond)								// || kjing > 540
	{
		MyMessageBox(_T("���ڷⵥ,���Ժ�����"));
		return ;
	}


	bool bSucc = true;
	int itm_cnt = m_listChqSSC.GetItemCount();
	m_nTouzhuCount=0;
	m_nTouzhuCount = itm_cnt;


	SendToServer(1);


	m_zongZhuShu = 0;
	OnBnClickedBtnClsList();
	//m_beishu = 1;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

}

void CChongQingSSC::OnBnClickedBtnDelSel()
{
	if(!m_strHaoma.IsEmpty())
	{
		m_strHaoma.Empty();
	}
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

		m_listChqSSC.DeleteItem(nItem);

		if(vecAllNums.size() >0)
		{
			vector<wstring>::iterator   iter = vecAllNums.begin()+nItem;
			vecAllNums.erase(iter);
		}
	}

}

void CChongQingSSC::OnBnClickedBtnClsList()
{
	m_zongZhuShu = 0;
	m_singlezhushu = 0;
	m_zongJine = 0.0;
	m_listChqSSC.deleteAllItemEx();
//	m_listChqSSC.DeleteAllItems();

	vecAllNums.clear();
	vecData1.clear();
	vecData2.clear();
	vecData3.clear();
//	m_editBeiTou.SetWindowText(L"1");
	//m_richDanshiHaoma.SetWindowText(L"");
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

void CChongQingSSC::OnEnChangeEditDanshiInput()
{
	//m_editDanShiHaoMa.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
	m_richDanshiHaoma.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
}
void CChongQingSSC::OnEnChangeRichEditDanshiInput()
{
	m_singlezhushu = GetDanShiZhusu(m_gameKind);


	return ;
}

int SplitString(const string &srcStr, vector<string> &destVec, const string splitStr="|")
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


int CChongQingSSC::GetDanShiZhusu(int nKind)
{

	m_singlezhushu = 0;
	CString strHaoma;

	m_richDanshiHaoma.GetWindowText(strHaoma);

	int nFind = strHaoma.Find(L"��");
	if(nFind != -1)
	{
		strHaoma.Replace(_T("��"),_T(","));
	}

	strHaoma.Replace(L"\r",L" ");
	strHaoma.Replace(L"\n",L" ");


	string strGetHaoma;

	ChangeStringToA(strHaoma,strGetHaoma);

	int nFirst = 0;
	int nSecond = 0;

	string sDouhao = "��";
	while (1)
	{
		nFirst=strGetHaoma.find(sDouhao,nSecond);

		if(nFirst!=string::npos)
		{
			strGetHaoma.replace(nFirst,1,",");
			nSecond=nFirst+sDouhao.size();
		}
		else if(nSecond<=strGetHaoma.size())
		{
			break;
		}
	}

	sDouhao = "\r";
	while (1)
	{
		nFirst=strGetHaoma.find(sDouhao,nSecond);

		if(nFirst!=string::npos)
		{
			strGetHaoma.replace(nFirst,1," ");
			nSecond=nFirst+sDouhao.size();

		}
		else if(nSecond<=strGetHaoma.size())
		{
			break;
		}
	}

	sDouhao = "\n";
	while (1)
	{
		nFirst=strGetHaoma.find(sDouhao,nSecond);

		if(nFirst!=string::npos)
		{
			strGetHaoma.replace(nFirst,1," ");
			nSecond=nFirst+sDouhao.size();

		}
		else if(nSecond<=strGetHaoma.size())
		{
			break;
		}
	}
	if(nKind == Ren3Hunhe_Zuxuan )
	{

		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
		int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

		string splitStr=" ";

		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			bool bSuit = true;
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();

				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
					//	m_singlezhushu = 0;
						bSuit = false;
						break;
					}
				}
				if (nSize > 3 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 3)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				if(tempData[0] == tempData[1]&&tempData[0] == tempData[2])
				{
					oldPos=newPos+splitStr.size();
					continue;

				}

	 			if(nCount == 3)
	 			{
	 				nZhushu = 1;
	 			}
	 			else if (nCount == 4)
	 			{
	 				nZhushu = 4;
	 			}
	 			else if (nCount == 5)
	 			{
	 				nZhushu = 10;
	 			}
	 			else
	 			{
					m_singlezhushu = 0;
					break;
	 			}
	 
	 			m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
 			else if(oldPos<=strGetHaoma.size())
 			{   
 				tempData= strGetHaoma.substr(oldPos);
				if(tempData.empty())
					break;

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
					//	m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
			//		m_singlezhushu = 0;
					break;
				}
				if (nSize != 3)
				{
			//		m_singlezhushu = 0;
					break;
				}
				if(tempData[0] == tempData[1]&&tempData[0] == tempData[2])
				{

					break;
				}

				if(nCount == 3)
				{
					nZhushu = 1;
				}
				else if (nCount == 4)
				{
					nZhushu = 4;
				}
				else if (nCount == 5)
				{
					nZhushu = 10;
				}
				else
				{
					m_singlezhushu = 0;
					break;
				}

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
 			}   
 			else
 			{   
 				break;
 			}   
		}   
	}

	if(nKind == WuxingZhixuan)
	{

// 		string strGetHaoma;
// 
// 		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";

		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			bool bSuit = true;
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
					//	m_singlezhushu = 0;
						bSuit = false;
						break;
					}
				}


				if (nSize > 5 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 5)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				nZhushu = 1;
				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
					break;
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
				//		m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 5|| !bSuit)
				{
			//		m_singlezhushu = 0;
					break;
				}
				if (nSize != 5)
				{
				//	m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   

	}

	if(nKind == WuxingQianSi || nKind == WuxingHousi || nKind == RenXuan4)
	{
// 		string strGetHaoma;
// 
// 		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			bool bSuit = true;
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
			//			m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 4 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 4)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				nZhushu = 1;

				if(nKind == RenXuan4)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					 if(nCount == 4)
					{
						nZhushu = 1;
					}
					else if(nCount == 5)
					{
						nZhushu = 5;
					}
					else
					{
						oldPos=newPos+splitStr.size();
						continue;
					}

				}

				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
					break;
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
					//	m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 4 || !bSuit)
				{
		//			m_singlezhushu = 0;
					break;
				}
				if (nSize != 4)
				{
	//				m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;
				if(nKind == RenXuan4)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					if(nCount == 4)
					{
						nZhushu = 1;
					}
					else if(nCount == 5)
					{
						nZhushu = 5;
					}
					else
					{
		//				m_singlezhushu = 0;
						break;
					}

				}

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}
	if(nKind == QianSanZhiXuan_FuShi || nKind == WuxingZhongsan || nKind == HouSanZhiXuan_FuShi || nKind == RenXuan3)
	{
// 		string strGetHaoma;
// 
// 		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			bool bSuit = true;
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);

				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						//m_singlezhushu = 0;
						bSuit = false;

						break;

					}
				}

				if (nSize > 3 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 3)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				nZhushu = 1;

				if(nKind == RenXuan3)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					if(nCount == 3)
					{
						nZhushu = 1;
					}
					else if(nCount == 4)
					{
						nZhushu = 4;
					}
					else if(nCount == 5)
					{
						nZhushu = 10;
					}
					else
					{
						oldPos=newPos+splitStr.size();
						continue;
					}

				}
				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						//m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
					break;
				}
				if (nSize != 3)
				{
					break;
				}

				nZhushu = 1;
				if(nKind == RenXuan3)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					if(nCount == 3)
					{
						nZhushu = 1;
					}
					else if(nCount == 4)
					{
						nZhushu = 4;
					}
					else if(nCount == 5)
					{
						nZhushu = 10;
					}
					else
					{
	//					m_singlezhushu = 0;
						break;
					}

				}

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}
	if(nKind == QianSanHunhe || nKind == HouSanHunhe || nKind == ZhongSanHunhe )
	{
		// 		string strGetHaoma;
		// 
		// 		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			bool bSuit = true;
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);

				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						//m_singlezhushu = 0;
						bSuit = false;

						break;

					}
				}

				if (nSize > 3 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 3)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				if(tempData[0] == tempData[1] && tempData[0] == tempData[2])
				{
					oldPos=newPos+splitStr.size();
					continue;

				}
				CString strLog,templog;
				templog = ChangeStringToT(tempData.c_str());
				strLog.Format(L"\nDANSHI2 tempData:%s",templog);
				OutputDebugString(strLog);
				if(templog == _T("055"))
				{
					strLog.Format(L"\nDANSHI2 tempData:%s",templog);
					OutputDebugString(strLog);

				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						//m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
					break;
				}
				if (nSize != 3)
				{
					break;
				}
				if(tempData[0] == tempData[1] && tempData[0] == tempData[2])
				{
					break;
				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}

	if(nKind == QianSanZuXuan_ZuLiuFuShi || nKind == HouSanZuXuan_ZuLiuFuShi || nKind == ZhongsanZuliu || nKind == Ren3Zu6_Fushi)
	{
// 		string strGetHaoma;
// 
// 		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			bool bSuit = true;
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						oldPos=newPos+splitStr.size();
						continue;
					}
				}

				if (nSize > 3 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 3)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				if(tempData[0] == tempData[1] || tempData[0] == tempData[2] || tempData[2] == tempData[1] )
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				nZhushu = 1;

				if(nKind == Ren3Zu6_Fushi)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					if(nCount == 3)
					{
						nZhushu = 1;
					}
					else if(nCount == 4)
					{
						nZhushu = 4;
					}
					else if(nCount == 5)
					{
						nZhushu = 10;
					}
					else
					{
						oldPos=newPos+splitStr.size();
						continue;
					}

				}

				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						bSuit = false;

						//m_singlezhushu = 0;
						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
				//	m_singlezhushu = 0;
					break;
				}
				if (nSize != 3)
				{
				//	m_singlezhushu = 0;
					break;
				}
				if(tempData[0] == tempData[1] || tempData[0] == tempData[2] || tempData[2] == tempData[1] )
				{
				//	m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;
				if(nKind == Ren3Zu6_Fushi)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					if(nCount == 3)
					{
						nZhushu = 1;
					}
					else if(nCount == 4)
					{
						nZhushu = 4;
					}
					else if(nCount == 5)
					{
						nZhushu = 10;
					}
					else
					{
				//		m_singlezhushu = 0;
						break;
					}

				}

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}

	if(nKind == QianSanZuXuan_ZuSanDanShi || nKind == HouSanZuXuan_ZuSanDanShi || nKind == Zhongsanzusandan || nKind == Ren3Zu3_Fushi)
	{
// 		string strGetHaoma;
// 
// 		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			bool bSuit = true;
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
					//	m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 3)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				if(tempData[0] == tempData[1] && tempData[0] == tempData[2] && tempData[2] == tempData[1] )
				{
					oldPos=newPos+splitStr.size();
					continue;

				}
				if(tempData[0]!= tempData[1] && tempData[0] != tempData[2] && tempData[2] != tempData[1] )
				{
					oldPos=newPos+splitStr.size();
					continue;

				}
				nZhushu = 1;
				if(nKind == Ren3Zu3_Fushi)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					if(nCount == 3)
					{
						nZhushu = 1;
					}
					else if(nCount == 4)
					{
						nZhushu = 4;
					}
					else if(nCount == 5)
					{
						nZhushu = 10;
					}
					else
					{
						oldPos=newPos+splitStr.size();
						continue;
					}

				}
				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						bSuit = false;

					//	m_singlezhushu = 0;
						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
				//	m_singlezhushu = 0;
					break;
				}
				if (nSize != 3)
				{
				//	m_singlezhushu = 0;
					break;
				}
				if(tempData[0] == tempData[1] && tempData[0] == tempData[2] && tempData[2] == tempData[1] )
				{
					//m_singlezhushu = 0;
					break;
				}
				if(tempData[0]!= tempData[1] && tempData[0] != tempData[2] && tempData[2] != tempData[1] )
				{
				//	m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;
				if(nKind == Ren3Zu3_Fushi)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					if(nCount == 3)
					{
						nZhushu = 1;
					}
					else if(nCount == 4)
					{
						nZhushu = 4;
					}
					else if(nCount == 5)
					{
						nZhushu = 10;
					}
					else
					{
						m_singlezhushu = 0;
						break;
					}

				}

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}

	if(nKind == WuXingQianEr_DanShi || nKind == WuXingHouEr_DanShi)
	{
// 		string strGetHaoma;
// 
// 		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			int nZhushu = 0;
			bool bSuit = true;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
				//		m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 2 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 2)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				nZhushu = 1;
				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						bSuit = false;

					//	m_singlezhushu = 0;
						break;
					}
				}

				if (nSize > 2 || !bSuit)
				{
				//	m_singlezhushu = 0;
					break;
				}
				if (nSize != 2)
				{
				//	m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}

	if(nKind == QianErZuXuan_DanShi || nKind == HouErZuXuan_DanShi)
	{
// 		string strGetHaoma;
// 
// 		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			bool bSuit = true;
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
					//	m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 2 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 2)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				if(tempData[0] == tempData[1])
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				nZhushu = 1;
				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						bSuit = false;

					//	m_singlezhushu = 0;
						continue;
					}
				}

				if (nSize > 2 || !bSuit)
				{
			//		m_singlezhushu = 0;
					break;
				}
				if (nSize != 2)
				{
			//		m_singlezhushu = 0;
					break;
				}
				if(tempData[0] == tempData[1])
				{
			//		m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}

	if(nKind == RenXuan2 || nKind == Ren2Zuxuan_Fushi)
	{
// 		string strGetHaoma;
// 
// 		ChangeStringToA(strHaoma,strGetHaoma);

		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
		int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

		string splitStr=" ";
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			bool bSuit = true;
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
			//			m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 2 || !bSuit)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}
				if (nSize != 2)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				if(tempData[0] == tempData[1] && nKind == Ren2Zuxuan_Fushi)
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				if(nCount == 2)
					nZhushu = 1;
				else if(nCount == 3)
					nZhushu = 3;
				else if(nCount == 4)
					nZhushu = 6;
				else if(nCount == 5)
					nZhushu = 10;
				else 
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						bSuit = false;

			//			m_singlezhushu = 0;
						break;
					}
				}

				if (nSize > 2 || !bSuit)
				{
			//		m_singlezhushu = 0;
					break;
				}
				if (nSize != 2)
				{
				//	m_singlezhushu = 0;
					break;
				}
				if(tempData[0] == tempData[1] && nKind == Ren2Zuxuan_Fushi)
				{
				//	m_singlezhushu = 0;
					break;
				}

				if(nCount == 2)
					nZhushu = 1;
				else if(nCount == 3)
					nZhushu = 3;
				else if(nCount == 4)
					nZhushu = 6;
				else if(nCount == 5)
					nZhushu = 10;
				else 
				{
				//	m_singlezhushu = 0;
					break;
				}

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=200;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	return m_singlezhushu;
	
}
void CChongQingSSC::OnEnChangeEditBeishu()
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
			//m_beishu = 1;
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

void CChongQingSSC::CheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(true);
}

void CChongQingSSC::UnCheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(false);
}

bool CChongQingSSC::IsNumBtnCheck(CNumerButton& btn)
{
	return btn.IsSelect();
}

void CChongQingSSC::HideCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}
void CChongQingSSC::ShowCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_SHOW);
	}
}

void CChongQingSSC::HideAllRio()
{
	//m_btWxzxTip.ShowWindow(SW_HIDE);
	//��ѡ2,3,4
	HideCtrl(IDC_RIO_RENXUAN2);
	HideCtrl(IDC_RIO_RENXUAN2_DAN);
	HideCtrl(IDC_RIO_RENXUAN3);
	HideCtrl(IDC_RIO_RENXUAN4);
	HideCtrl(IDC_RIO_RENXUAN4_DAN);
	HideCtrl(IDC_RIO_RENXUAN3_DAN);
	HideCtrl(IDC_RIO_RENXUAN3_ZUXUAN3);
	HideCtrl(IDC_RIO_RENXUAN3_ZUXUAN3_DAN);
	HideCtrl(IDC_RIO_RENXUAN3_ZUXUAN6);
	HideCtrl(IDC_RIO_RENXUAN3_ZUXUAN6_DAN);
	HideCtrl(IDC_RIO_RENXUAN2_ZUXUAN);
	HideCtrl(IDC_RIO_RENXUAN2_ZUXUAN_DAN);
	HideCtrl(IDC_RIO_RENXUAN3_HUNHE);
	//����
	HideCtrl(IDC_RIO_HSZHX_FUSHI);
	HideCtrl(IDC_RIO_HSZHX_DANSHI);

	HideCtrl(IDC_RIO_YFFS);				//һ����˳
	HideCtrl(IDC_RIO_HSCS);				//���³�˫
	HideCtrl(IDC_RIO_SXBX);				//���Ǳ�ϲ
	HideCtrl(IDC_RIO_SJFC);				//�ļ�����

	HideCtrl(IDC_RIO_WXZX);				//����ֱѡ
	HideCtrl(IDC_RIO_WXZX_DAN);				//����ֱѡ
	HideCtrl(IDC_RIO_WXQS);				//����ǰ��
	HideCtrl(IDC_RIO_WXQS_DAN);				//����ǰ��
	HideCtrl(IDC_RIO_WXHS);				//���Ǻ���
	HideCtrl(IDC_RIO_WXHS_DAN);				//���Ǻ���
	HideCtrl(IDC_RIO_WXZS);				//��������
	HideCtrl(IDC_RIO_WXZS_DAN);				//��������
	HideCtrl(IDC_RIO_QS_HEZHI);				//���Ǻ���
	HideCtrl(IDC_RIO_ZS_HEZHI);				//���Ǻ���
	HideCtrl(IDC_RIO_HS_HEZHI);				//��������
	HideCtrl(IDC_RIO_QE_HEZHI);				//���Ǻ���
	HideCtrl(IDC_RIO_HE_HEZHI);				//��������
	HideCtrl(IDC_RIO_NS);				//ţ��
	HideCtrl(IDC_RIO_NN);				//ţţ
	HideCtrl(IDC_RIO_WN);				//��ţ
	HideCtrl(IDC_STATIC_NIUNIU_TIP);				//ţ��

	HideCtrl(IDC_RIO_LHD_WQ);
	HideCtrl(IDC_RIO_LHD_WB);
	HideCtrl(IDC_RIO_LHD_WS);
	HideCtrl(IDC_RIO_LHD_WG);
	HideCtrl(IDC_RIO_LHD_QB);
	HideCtrl(IDC_RIO_LHD_QS);
	HideCtrl(IDC_RIO_LHD_QG);
	HideCtrl(IDC_RIO_LHD_BS);
	HideCtrl(IDC_RIO_LHD_BG);
	HideCtrl(IDC_RIO_LHD_SG);

	//ǰ��
	HideCtrl(IDC_RIO_QSZHX_FUSHI);
	HideCtrl(IDC_RIO_QSZHX_DANSHI);

	HideCtrl(IDC_RIO_HSZS_FUSHI);
	HideCtrl(IDC_RIO_HSZS_DANSHI);
	HideCtrl(IDC_RIO_HSZL_FUSHI);
	HideCtrl(IDC_RIO_HSZL_DANSHI);

	HideCtrl(IDC_RIO_QSZS_FUSHI);
	HideCtrl(IDC_RIO_QSZS_DANSHI);
	HideCtrl(IDC_RIO_QSZL_FUSHI);
	HideCtrl(IDC_RIO_QSZL_DANSHI);

	HideCtrl(IDC_RIO_ZSZS_FUSHI);
	HideCtrl(IDC_RIO_ZSZS_DANSHI);
	HideCtrl(IDC_RIO_ZSZL_FUSHI);
	HideCtrl(IDC_RIO_ZSZL_DANSHI);

	HideCtrl(IDC_RIO_HEZHX_FUSHI);
	HideCtrl(IDC_RIO_HEZHX_DANSHI);

	HideCtrl(IDC_RIO_QEZHX_FUSHI);
	HideCtrl(IDC_RIO_QEZHX_DANSHI);

	HideCtrl(IDC_RIO_HEZX_FUSHI);
	HideCtrl(IDC_RIO_HEZX_DANSHI);

	HideCtrl(IDC_RIO_QEZX_FUSHI);
	HideCtrl(IDC_RIO_QEZX_DANSHI);

	HideCtrl(IDC_RIO_ZX_120);
	HideCtrl(IDC_RIO_ZX_60);
	HideCtrl(IDC_RIO_ZX_30);
	HideCtrl(IDC_RIO_ZX_20);
	HideCtrl(IDC_RIO_ZX_10);
	HideCtrl(IDC_RIO_ZX_5);
	HideCtrl(IDC_RIO_QS_ZX24);
	HideCtrl(IDC_RIO_QS_ZX12);
	HideCtrl(IDC_RIO_QS_ZX6);
	HideCtrl(IDC_RIO_QS_ZX4);
	HideCtrl(IDC_RIO_HS_ZX24);
	HideCtrl(IDC_RIO_HS_ZX12);
	HideCtrl(IDC_RIO_HS_ZX6);
	HideCtrl(IDC_RIO_HS_ZX4);

	HideCtrl(IDC_RIO_YFFS);
	HideCtrl(IDC_RIO_HSCS);
	HideCtrl(IDC_RIO_SXBX);
	HideCtrl(IDC_RIO_SJFC);

	HideCtrl(IDC_RIO_DXDSH);
	HideCtrl(IDC_RIO_DXDSH_DW);
	HideCtrl(IDC_RIO_DXDSH_HZH);

	HideCtrl(IDC_RIO_HSZHXHZH);

	HideCtrl(IDC_RIO_HSBDW);
	HideCtrl(IDC_RIO_QSBDW);
	HideCtrl(IDC_RIO_ZSBDW);
	HideCtrl(IDC_RIO_HEBDW);
	HideCtrl(IDC_RIO_QEBDW);

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

//void CChongQingSSC::SelctRadioBtn(int ctrl_id)
//{
//	CButton* pRadio = (CButton*)GetDlgItem(ctrl_id);
//	if (pRadio != NULL) 
//	{
//		pRadio->SetCheck(BST_CHECKED);
//	}
//}
CString CChongQingSSC::GetNiuString(int nNiu)
{
	CString str;
	switch(nNiu)
	{
	case 0:
		{
			str = _T("ţţ");
			break;
		}
	case 1:
		{
			str = _T("ţһ");
			break;
		}
	case 2:
		{
			str = _T("ţ��");
			break;
		}
	case 3:
		{
			str = _T("ţ��");
			break;
		}
	case 4:
		{
			str = _T("ţ��");
			break;
		}
	case 5:
		{
			str = _T("ţ��");
			break;
		}
	case 6:
		{
			str = _T("ţ��");
			break;
		}
	case 7:
		{
			str = _T("ţ��");
			break;
		}
	case 8:
		{
			str = _T("ţ��");
			break;
		}
	case 9:
		{
			str = _T("ţ��");
			break;
		}
	}

	return str;
}
CString CChongQingSSC::GetNiuString()
{
	CString str;
	if (IsNumBtnCheck(m_btnNiu_0))
	{
		str += _T("��ţ");
	}
	if (IsNumBtnCheck(m_btnNiu_1))
	{
		str += _T("ţһ");
	}
	if (IsNumBtnCheck(m_btnNiu_2))
	{
		str += _T("ţ��");
	}
	if (IsNumBtnCheck(m_btnNiu_3))
	{
		str += _T("ţ��");
	}
	if (IsNumBtnCheck(m_btnNiu_4))
	{
		str += _T("ţ��");
	}
	if (IsNumBtnCheck(m_btnNiu_5))
	{
		str += _T("ţ��");
	}
	if (IsNumBtnCheck(m_btnNiu_6))
	{
		str += _T("ţ��");
	}
	if (IsNumBtnCheck(m_btnNiu_7))
	{
		str += _T("ţ��");
	}
	if (IsNumBtnCheck(m_btnNiu_8))
	{
		str += _T("ţ��");
	}
	if (IsNumBtnCheck(m_btnNiu_9))
	{
		str += _T("ţ��");
	}
	if (IsNumBtnCheck(m_btnNiu_10))
	{
		str += _T("ţţ");
	}

	return str;
}
CString CChongQingSSC::GetNiuNumString()
{
	CString str;
	if (IsNumBtnCheck(m_btnNiu_0))
	{
		str += _T("00");
	}
	if (IsNumBtnCheck(m_btnNiu_1))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnNiu_2))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnNiu_3))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnNiu_4))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnNiu_5))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnNiu_6))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnNiu_7))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnNiu_8))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnNiu_9))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnNiu_10))
	{
		str += _T("10");
	}

	return str;
}
CString CChongQingSSC::GetNiuNum(CString strNiu)
{
	CString str;
	if (strNiu == _T("��ţ"))
	{
		str = _T("00");
	}
	if (strNiu == _T("ţһ"))
	{
		str = _T("01");
	}
	if (strNiu == _T("ţ��"))
	{
		str = _T("02");
	}
	if (strNiu == _T("ţ��"))
	{
		str = _T("03");
	}
	if (strNiu == _T("ţ��"))
	{
		str = _T("04");
	}
	if (strNiu == _T("ţ��"))
	{
		str = _T("05");
	}
	if (strNiu == _T("ţ��"))
	{
		str = _T("06");
	}
	if (strNiu == _T("ţ��"))
	{
		str = _T("07");
	}
	if (strNiu == _T("ţ��"))
	{
		str = _T("08");
	}
	if (strNiu == _T("ţ��"))
	{
		str = _T("09");
	}
	if (strNiu == _T("ţţ"))
	{
		str = _T("10");
	}

	return str;
}

CString CChongQingSSC::GetHezhiString()
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


CString CChongQingSSC::GetWanString()
{
	CString str;
	if (IsNumBtnCheck(m_btnWan_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnWan_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnWan_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnWan_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnWan_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnWan_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnWan_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnWan_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnWan_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnWan_9))
	{
		str += _T("9");
	}
	return str;
}

CString CChongQingSSC::GetQianString()
{
	CString str;
	if (IsNumBtnCheck(m_btnQian_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnQian_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnQian_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnQian_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnQian_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnQian_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnQian_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnQian_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnQian_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnQian_9))
	{
		str += _T("9");
	}
	return str;
}

CString CChongQingSSC::GetBaiString()
{
	CString str;
	if (IsNumBtnCheck(m_btnBai_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnBai_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnBai_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnBai_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnBai_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnBai_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnBai_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnBai_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnBai_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnBai_9))
	{
		str += _T("9");
	}
	return str;
}

CString CChongQingSSC::GetShiString()
{
	CString str;
	if (IsNumBtnCheck(m_btnShi_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnShi_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnShi_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnShi_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnShi_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnShi_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnShi_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnShi_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnShi_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnShi_9))
	{
		str += _T("9");
	}
	return str;
}

CString CChongQingSSC::GetGeString()
{
	CString str;
	if (IsNumBtnCheck(m_btnGe_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnGe_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnGe_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnGe_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnGe_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnGe_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnGe_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnGe_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnGe_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnGe_9))
	{
		str += _T("9");
	}
	return str;
}
CString CChongQingSSC::GetWanDxdshString()
{
	CString strWan;
	if (IsNumBtnCheck(m_btnDaWan)) 
	{
		strWan += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoWan)) 
	{
		strWan += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanWan)) 
	{
		strWan += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangWan)) 
	{
		strWan += _T("2");
	}

	return strWan;
}
CString CChongQingSSC::GetQianDxdshString()
{
	CString strQian;
	if (IsNumBtnCheck(m_btnDaQian)) 
	{
		strQian += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoQian)) 
	{
		strQian += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanQian)) 
	{
		strQian += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangQian)) 
	{
		strQian += _T("2");
	}

	return strQian;
}
CString CChongQingSSC::GetBaiDxdshString()
{
	CString strBai;
	if (IsNumBtnCheck(m_btnDaBai)) 
	{
		strBai += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoBai)) 
	{
		strBai += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanBai)) 
	{
		strBai += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangBai)) 
	{
		strBai += _T("2");
	}

	return strBai;
}

CString CChongQingSSC::GetShiDxdshString()
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

CString CChongQingSSC::GetGeDxdshString()
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

CString CChongQingSSC::GetWanDesc()
{
	return theGameKind[(m_TypeID << 16) | (unsigned short)m_gameKind];
}

SSCWanFaKind CChongQingSSC::GetGameKindByDesc(const CString& desc)
{
	if (desc == _T("����ֱѡ")) 
	{
		return HouSanZhiXuan_FuShi;
	} 
	else if (desc == _T("����ֱѡ")) 
	{
		return WuxingZhixuan;
	} 
	else if(desc == _T("������"))
	{
		return SSC_LongHu;
	}
	else if (desc == _T("��������")) 
	{
		return Ren3Zu3_Fushi;
	} 
	else if (desc == _T("��������")) 
	{
		return Ren3Zu6_Fushi;
	} 
	else if (desc == _T("�������")) 
	{
		return Ren3Hunhe_Zuxuan;
	} 
	else if(desc == _T("�ζ���ѡ"))
	{
		return Ren2Zuxuan_Fushi;
	}
	else if (desc == _T("����ǰ��")) 
	{
		return WuxingQianSi;
	} 
	else if (desc == _T("���Ǻ���")) 
	{
		return WuxingHousi;
	} 
	else if (desc == _T("ǰ����ֵ")) 
	{
		return QianSanZhiXuanHeZhi;
	} 
	else if (desc == _T("������ֵ")) 
	{
		return ZhongSanZhiXuanHeZhi;
	} 
	else if (desc == _T("ǰ����ֵ")) 
	{
		return QianErZhixuan_Hezhi;
	} 
	else if (desc == _T("������ֵ")) 
	{
		return HouSanZhiXuanHeZhi;
	} 
	else if (desc == _T("�����ֵ")) 
	{
		return HouErZhixuan_Hezhi;
	} 
	else if (desc == _T("΢��ţţ")) 
	{
		return NiuNiu_Num;
	} 
	else if (desc == _T("����ֱѡ")) 
	{
		return WuxingZhongsan;
	} 
	else if (desc == _T("����ֱѡ��ʽ")) 
	{
		return HouSanZhiXuan_DanShi;
	} 
	else if (desc == _T("��������")) 
	{
		return ZhongsanZusan;
	} 
	else if (desc == _T("����������ʽ")) 
	{
		return Zhongsanzusandan;
	} 
	else if (desc == _T("��������")) 
	{
		return ZhongsanZuliu;
	} 
	else if (desc == _T("ǰ��ֱѡ")) 
	{
		return QianSanZhiXuan_FuShi;
	} 
	else if (desc == _T("ǰ��ֱѡ��ʽ")) 
	{
		return QianSanZhiXuan_DanShi;
	} 
	else if (desc == _T("��������")) 
	{
		return HouSanZuXuan_ZuSanFuShi;
	} 
	else if (desc == _T("��ѡ120")) 
	{
		return Zuxuan120;
	} 
	else if (desc == _T("��ѡ60")) 
	{
		return Zuxuan60;
	} 
	else if (desc == _T("��ѡ30")) 
	{
		return Zuxuan30;
	} 
	else if (desc == _T("��ѡ20")) 
	{
		return Zuxuan20;
	} 
	else if (desc == _T("��ѡ10")) 
	{
		return Zuxuan10;
	} 
	else if (desc == _T("��ѡ5")) 
	{
		return Zuxuan5;
	} 
	else if (desc == _T("ǰ����ѡ24")) 
	{
		return QSZuXuan24;
	} 
	else if (desc == _T("ǰ����ѡ12")) 
	{
		return QSZuXuan12;
	} 
	else if (desc == _T("ǰ����ѡ6")) 
	{
		return QSZuXuan6;
	} 
	else if (desc == _T("ǰ����ѡ4")) 
	{
		return QSZuXuan4;
	} 
	else if (desc == _T("������ѡ24")) 
	{
		return HSZuXuan24;
	} 
	else if (desc == _T("������ѡ12")) 
	{
		return HSZuXuan12;
	} 
	else if (desc == _T("������ѡ6")) 
	{
		return HSZuXuan6;
	} 
	else if (desc == _T("������ѡ4")) 
	{
		return HSZuXuan4;
	} 
	else if (desc == _T("����������ʽ")) 
	{
		return HouSanZuXuan_ZuSanDanShi;
	}
	else if (desc == _T("��������")) 
	{
		return HouSanZuXuan_ZuLiuFuShi;
	} 
	else if (desc == _T("����������ʽ")) 
	{
		return HouSanZuXuan_ZuLiuDanShi;
	} 
	else if (desc == _T("ǰ������")) 
	{
		return QianSanZuXuan_ZuSanFuShi;
	} 
	else if (desc == _T("ǰ��������ʽ")) 
	{
		return QianSanZuXuan_ZuSanDanShi;
	}
	else if (desc == _T("ǰ������")) 
	{
		return QianSanZuXuan_ZuLiuFuShi;
	}
	else if (desc == _T("ǰ��������ʽ")) 
	{
		return QianSanZuXuan_ZuLiuDanShi;
	}
	else if (desc == _T("���ֱѡ")) 
	{
		return WuXingHouEr_FuShi;
	}
	else if (desc == _T("���ֱѡ��ʽ")) 
	{
		return WuXingHouEr_DanShi;
	}
	else if (desc == _T("ǰ��ֱѡ")) 
	{
		return WuXingQianEr_FuShi;
	}
	else if (desc == _T("ǰ��ֱѡ��ʽ")) 
	{
		return WuXingQianEr_DanShi;
	}
	else if (desc == _T("�����ѡ")) 
	{
		return HouErZuXuan_FuShi;
	}
	else if (desc == _T("�����ѡ��ʽ")) 
	{
		return HouErZuXuan_DanShi;
	}
	else if (desc == _T("ǰ����ѡ")) 
	{
		return QianErZuXuan_FuShi;
	}
	else if (desc == _T("ǰ����ѡ��ʽ")) 
	{
		return QianErZuXuan_DanShi;
	}
	else if (desc == _T("��С��˫")) 
	{
		return DaXiaoDanShuang;
	}
	else if (desc == _T("�����С��˫")) 
	{
		return DaXiaoDanShuang;
	}
	else if (desc == _T("��λ��С��˫")) 
	{
		return DaXiaoDanShuang_dw;
	}
	else if(desc == _T("��ֵ��С��˫"))
	{
		return Zonghe_DXDSLh;
	}
	else if (desc == _T("��������λ")) 
	{
		return BuDingWei_HouSan;
	}
	else if (desc == _T("ǰ������λ")) 
	{
		return BuDingWei_QianSan;
	}
	else if (desc == _T("��������λ")) 
	{
		return BuDingWei_ZhongSan;
	}
	else if (desc == _T("���Ƕ�λ��")) 
	{
		return DingWeiDan;
	}
	else if (desc == _T("һ����˳")) 
	{
		return QW_YiFanFengShun;
	}
	else if (desc == _T("���³�˫")) 
	{
		return QW_HaoShiChengShuang;
	}
	else if (desc == _T("���Ǳ�ϲ")) 
	{
		return QW_SanXingBaoXi;
	}
	else if (desc == _T("�ļ�����")) 
	{
		return QW_SiJiFaCai;
	}
	else if (desc == _T("ǰ������λ")) 
	{
		return BuDingWei_QianEr;
	}
	else if (desc == _T("�������λ")) 
	{
		return BuDingWei_HouEr;
	}
	else if (desc == _T("��ѡ��"))
		return RenXuan2;
	else if (desc == _T("��ѡ��"))
		return RenXuan3;

	else if (desc == _T("��ѡ��"))
		return RenXuan4;
	else if (desc == _T("ǰ�����"))
		return QianSanHunhe;
	else if (desc == _T("�������"))
		return ZhongSanHunhe;
	else if (desc == _T("�������"))
		return HouSanHunhe;

	return inValid;
}

void CChongQingSSC::AdjustWanFa()
{
	if (m_btnHouSanZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouSanZhiXuan.SetWindowPos(NULL, wanfa_x, wanfa_y, m_btnHouSanZhiXuan.Width(), m_btnHouSanZhiXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianSanZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianSanZhiXuan.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*10+16*6+6*2, wanfa_y, m_btnQianSanZhiXuan.Width(), m_btnQianSanZhiXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnNiuNiu.GetSafeHwnd() != NULL) 
	{
		m_btnNiuNiu.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*12+16*6+6*2, wanfa_y , m_btnNiuNiu.Width(), m_btnNiuNiu.Height(), SWP_NOZORDER);
		//m_btnNiuNiu.ShowWindow(SW_HIDE);
	}
	if (m_btnLhd.GetSafeHwnd() != NULL) 
	{
		m_btnLhd.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*13+16*6+6*2, wanfa_y , m_btnNiuNiu.Width(), m_btnNiuNiu.Height(), SWP_NOZORDER);
		//m_btnNiuNiu.ShowWindow(SW_HIDE);
	}

	if (m_btnHouSanZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouSanZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*6+14*4, wanfa_y, m_btnHouSanZuXuan.Width(), m_btnHouSanZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianSanZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianSanZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width, wanfa_y, m_btnQianSanZuXuan.Width(), m_btnQianSanZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnWuXingHouEr.GetSafeHwnd() != NULL) 
	{
		m_btnWuXingHouEr.SetWindowPos(NULL, wanfa_x + wanfa_col_width*3, wanfa_y , m_btnWuXingHouEr.Width(), m_btnWuXingHouEr.Height(), SWP_NOZORDER);
	}

	if (m_btnWuXingQianEr.GetSafeHwnd() != NULL) 
	{
		m_btnWuXingQianEr.SetWindowPos(NULL, wanfa_x + wanfa_col_width*2, wanfa_y /*+ wanfa_row_height*/, m_btnWuXingQianEr.Width(), m_btnWuXingQianEr.Height(), SWP_NOZORDER);
	}

	if (m_btnHouErZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouErZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*5, wanfa_y, m_btnHouErZuXuan.Width(), m_btnHouErZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianErZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianErZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*4, wanfa_y /*+ wanfa_row_height*/, m_btnQianErZuXuan.Width(), m_btnQianErZuXuan.Height(), SWP_NOZORDER);
	}
	if (m_btnQianErXing.GetSafeHwnd() != NULL) 
	{
		m_btnQianErXing.SetWindowPos(NULL, wanfa_x + wanfa_col_width*6, wanfa_y /*+ wanfa_row_height*/, m_btnQianErXing.Width(), m_btnQianErXing.Height(), SWP_NOZORDER);
	}

	if (m_btnDaXiaoDanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnDaXiaoDanShuang.SetWindowPos(NULL, wanfa_x + wanfa_col_width*7+15*5, wanfa_y, m_btnDaXiaoDanShuang.Width(), m_btnDaXiaoDanShuang.Height(), SWP_NOZORDER);
	}
	//����Ϊ ���
	if (m_btnHouSanHeZhi.GetSafeHwnd() != NULL) 
	{
		m_btnHouSanHeZhi.SetWindowPos(NULL, wanfa_x + wanfa_col_width*11+16*6+6*2, wanfa_y , m_btnHouSanHeZhi.Width(), m_btnHouSanHeZhi.Height(), SWP_NOZORDER);
		m_btnHouSanHeZhi.ShowWindow(SW_HIDE);
	}

	if (m_btnBuDingWei.GetSafeHwnd() != NULL) 
	{
		m_btnBuDingWei.SetWindowPos(NULL, wanfa_x + wanfa_col_width*8+16*6, wanfa_y, m_btnBuDingWei.Width(), m_btnBuDingWei.Height(), SWP_NOZORDER);
	}

	if (m_btnDingWeiDan.GetSafeHwnd() != NULL) 
	{
		m_btnDingWeiDan.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*9+16*6+6, wanfa_y, m_btnDingWeiDan.Width(), m_btnDingWeiDan.Height(), SWP_NOZORDER);
	}
	if (m_btnQuwei.GetSafeHwnd() != NULL) 
	{
		m_btnQuwei.SetWindowPos(NULL, wanfa_x + wanfa_col_width*11+16*6+6*2, wanfa_y, m_btnQuwei.Width(), m_btnQuwei.Height(), SWP_NOZORDER);
	}
}

//����Ͷע��ʽ ��ѡ��
void CChongQingSSC::AdjustRadio()
{
	//��ѡ2 3  4
	CWnd* pWnd = GetDlgItem(IDC_RIO_RENXUAN2);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN2_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*4, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN3);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN3_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN3_ZUXUAN3);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*2, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN3_ZUXUAN3_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*3, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN3_ZUXUAN6);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*4, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN3_ZUXUAN6_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*5, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN3_HUNHE);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*6, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN2_ZUXUAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*5, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN2_ZUXUAN_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*6, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN4);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x/*+wanfa_sel_width*2*/, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_RENXUAN4_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//----����ֱѡ-----------����ǰ��--------------------
	pWnd = GetDlgItem(IDC_RIO_WXZX);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_WXZX_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width+31, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_WXQS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x /*+ wanfa_sel_width*/, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_WXQS_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//----------���Ǻ���----------------��������---------
	pWnd = GetDlgItem(IDC_RIO_WXHS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x , wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_WXHS_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_WXZS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x , wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_WXZS_DAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width , wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_NS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x /*+ wanfa_sel_width*/, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_STATIC_NIUNIU_TIP);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y+1, wanfa_sel_width*4, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_NN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2 , wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_WN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------

	//----����ֱѡ��ʽ-----------����ֱѡ��ʽ--------------------
	pWnd = GetDlgItem(IDC_RIO_HSZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//
	pWnd = GetDlgItem(IDC_RIO_HSZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//----------ǰ��ֱѡ��ʽ----------------ǰ��ֱѡ��ʽ---------
	pWnd = GetDlgItem(IDC_RIO_QSZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x/* + wanfa_sel_width*5*/, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//��ʽ��ʱ����
	pWnd = GetDlgItem(IDC_RIO_QSZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y/*+wanfa_sel_height*2*/, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HSZS_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x , mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSZS_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSZL_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSZL_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_RIO_ZSZS_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x , mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZSZS_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width , mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZSZL_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZSZL_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//-------------------------------------------------------------------
	pWnd = GetDlgItem(IDC_RIO_ZX_120);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x , mouseinput_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZX_60);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width+31, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZX_30);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2+31, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZX_20);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3+31, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZX_10);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*4+31, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZX_5);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*5+31, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_QS_ZX24);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x , mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QS_ZX12);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QS_ZX6);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QS_ZX4);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HS_ZX24);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x , mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HS_ZX12);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HS_ZX6);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HS_ZX4);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//////////////////////////////////////////////////////////////////////////
	pWnd = GetDlgItem(IDC_RIO_QSZS_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x , mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QSZS_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QSZL_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QSZL_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_YFFS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSCS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_SXBX);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_SJFC);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_QS_HEZHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZS_HEZHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HS_HEZHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QE_HEZHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HE_HEZHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*4, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HEZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HEZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width*2, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HEZX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_QEZX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QEZX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_DXDSH);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_DXDSH_DW);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_DXDSH_HZH);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ (wanfa_sel_width)*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ (wanfa_sel_width)*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZSBDW);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QSBDW);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x /*+ wanfa_sel_width*2*/, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_RIO_HEBDW);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ (wanfa_sel_width)*4, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QEBDW);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width)*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
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
		pWnd->SetWindowPos(NULL, wanfa_sel_x+ wanfa_sel_width*4, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_ZSHUNHE);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*4, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HSHUNHE);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*4, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_MOUSE_INPUT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, mouseinput_x, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_KEYBOARD_INPUT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL,keyboardinput_x, keyboardinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	//�����ʾ
	pWnd = GetDlgItem(IDC_STATIC_HUNHE_TIP);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*3, wanfa_sel_y+5, wanfa_sel_width*4, wanfa_sel_height, SWP_NOZORDER);
	}
/////////////////////////////////////������///////////////////////////////////////////////////////////////////////
	pWnd = GetDlgItem(IDC_RIO_LHD_WQ);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x /*+ wanfa_sel_width*/, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_LHD_WB);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width), mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_LHD_WS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width)*2, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_LHD_WG);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width)*3, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_LHD_QB);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width)*4, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_LHD_QS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x +(wanfa_sel_width)*5, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_LHD_QG);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width)*6 , mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_LHD_BS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width)*7, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_LHD_BG);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width)*8, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_LHD_SG);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width)*9, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	if(m_staticRio1.GetSafeHwnd()!=NULL)
	{
		m_staticRio1.SetWindowPos(NULL, wanfa_sel_x - m_staticRio1.Width() , wanfa_sel_y, m_staticRio1.Width(), m_staticRio1.Height(), SWP_NOZORDER);
	}
	if(m_staticRio2.GetSafeHwnd()!=NULL)
	{
		m_staticRio2.SetWindowPos(NULL, wanfa_sel_x - m_staticRio2.Width() , mouseinput_y, m_staticRio2.Width(), m_staticRio2.Height(), SWP_NOZORDER);
	}
	if(m_staticRio3.GetSafeHwnd()!=NULL)
	{
		m_staticRio3.SetWindowPos(NULL, wanfa_sel_x + (wanfa_sel_width)*2+20 , wanfa_sel_y, m_staticRio3.Width(), m_staticRio3.Height(), SWP_NOZORDER);
	}
}

//Ͷע��ѡ�����֣�5λ���֣�0--9��ѡ��
void CChongQingSSC::AdjustNumBtn()
{
	//��ֵ------------------------------
	if(m_btnLong.GetSafeHwnd() != NULL) 
	{
		m_btnLong.SetWindowPos(NULL, haoma_btn_x+25, haoma_btn_y+haoma_btn_row_height*2, m_btnLong.Width(), m_btnLong.Height(), SWP_NOZORDER);
	}
	if(m_btnHu.GetSafeHwnd() != NULL) 
	{
		m_btnHu.SetWindowPos(NULL, haoma_btn_x+25 + haoma_btn_col_wdith, haoma_btn_y+haoma_btn_row_height*2, m_btnHu.Width(), m_btnHu.Height(), SWP_NOZORDER);
	}
	if(m_btnHe.GetSafeHwnd() != NULL) 
	{
		m_btnHe.SetWindowPos(NULL, haoma_btn_x+25 + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height*2, m_btnHe.Width(), m_btnHe.Height(), SWP_NOZORDER);
	}

	//��ֵ------------------------------
	if(m_btnHeZhi_0.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_0.Width(), m_btnHeZhi_0.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_1.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_1.Width(), m_btnHeZhi_1.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_2.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_2.Width(), m_btnHeZhi_2.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_3.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_3.Width(), m_btnHeZhi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_4.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_4.Width(), m_btnHeZhi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_5.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_5.Width(), m_btnHeZhi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_6.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_6.Width(), m_btnHeZhi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_7.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_7.Width(), m_btnHeZhi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_8.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_8.Width(), m_btnHeZhi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_9.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_9.Width(), m_btnHeZhi_9.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_10.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_10.Width(), m_btnHeZhi_10.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_11.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_11.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_11.Width(), m_btnHeZhi_11.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_12.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_12.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_12.Width(), m_btnHeZhi_12.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_13.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_13.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_13.Width(), m_btnHeZhi_13.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_14.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_14.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_14.Width(), m_btnHeZhi_14.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_15.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_15.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_15.Width(), m_btnHeZhi_15.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_16.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_16.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_16.Width(), m_btnHeZhi_16.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_17.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_17.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_3.Width(), m_btnHeZhi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_18.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_18.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_4.Width(), m_btnHeZhi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_19.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_19.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_5.Width(), m_btnHeZhi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_20.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_20.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_6.Width(), m_btnHeZhi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_21.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_21.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_7.Width(), m_btnHeZhi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_22.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_22.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_8.Width(), m_btnHeZhi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_23.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_23.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_9.Width(), m_btnHeZhi_9.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_24.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_24.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_10.Width(), m_btnHeZhi_10.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_25.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_25.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_11.Width(), m_btnHeZhi_11.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_26.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_26.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_12.Width(), m_btnHeZhi_12.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_27.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_27.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_13.Width(), m_btnHeZhi_13.Height(), SWP_NOZORDER);
	}
	//ţţ----------------------------
	if(m_btnNiu_0.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_0.Width(), m_btnNiu_0.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_1.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_1.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_1.Width(), m_btnNiu_1.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_2.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_2.Width(), m_btnNiu_2.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_3.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_3.Width(), m_btnNiu_3.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_4.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_4.Width(), m_btnNiu_4.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_5.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_5.Width(), m_btnNiu_5.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_6.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_6.Width(), m_btnNiu_6.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_7.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_7.Width(), m_btnNiu_7.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_8.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_8.Width(), m_btnNiu_8.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_9.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_9.Width(), m_btnNiu_9.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_10.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height*2, m_btnNiu_10.Width(), m_btnNiu_10.Height(), SWP_NOZORDER);
	}

	if (m_btnNiuQuan.GetSafeHwnd() != NULL) 
	{
		m_btnNiuQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuQuan.Width(), m_btnNiuQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuDa.GetSafeHwnd() != NULL) 
	{
		m_btnNiuDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuDa.Width(), m_btnNiuDa.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuXiao.GetSafeHwnd() != NULL) 
	{
		m_btnNiuXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuXiao.Width(), m_btnNiuXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuDan.GetSafeHwnd() != NULL) 
	{
		m_btnNiuDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuDan.Width(), m_btnNiuDan.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuShuang.GetSafeHwnd() != NULL) 
	{
		m_btnNiuShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuShuang.Width(), m_btnNiuShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuQing.GetSafeHwnd() != NULL) 
	{
		m_btnNiuQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuQing.Width(), m_btnNiuQing.Height(), SWP_NOZORDER);
	}

	//��λ------------------------------
	if (m_staticWan.GetSafeHwnd() != NULL) 
	{
		m_staticWan.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y, m_staticWan.Width(), m_staticWan.Height(), SWP_NOZORDER);
	}

	if(m_btnWan_0.GetSafeHwnd() != NULL) 
	{
		m_btnWan_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y, m_btnWan_0.Width(), m_btnWan_0.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_1.GetSafeHwnd() != NULL) 
	{
		m_btnWan_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y, m_btnWan_1.Width(), m_btnWan_1.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_2.GetSafeHwnd() != NULL) 
	{
		m_btnWan_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y, m_btnWan_2.Width(), m_btnWan_2.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_3.GetSafeHwnd() != NULL) 
	{
		m_btnWan_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y, m_btnWan_3.Width(), m_btnWan_3.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_4.GetSafeHwnd() != NULL) 
	{
		m_btnWan_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y, m_btnWan_4.Width(), m_btnWan_4.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_5.GetSafeHwnd() != NULL) 
	{
		m_btnWan_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y, m_btnWan_5.Width(), m_btnWan_5.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_6.GetSafeHwnd() != NULL) 
	{
		m_btnWan_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y, m_btnWan_6.Width(), m_btnWan_6.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_7.GetSafeHwnd() != NULL) 
	{
		m_btnWan_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y, m_btnWan_7.Width(), m_btnWan_7.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_8.GetSafeHwnd() != NULL) 
	{
		m_btnWan_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y, m_btnWan_8.Width(), m_btnWan_8.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_9.GetSafeHwnd() != NULL) 
	{
		m_btnWan_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y, m_btnWan_9.Width(), m_btnWan_9.Height(), SWP_NOZORDER);
	}
	
	//��λ��ȫ����С������˫����
	if (m_btnWanQuan.GetSafeHwnd() != NULL) 
	{
		m_btnWanQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+40, haoma_btn_y, m_btnWanQuan.Width(), m_btnWanQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnWanDa.GetSafeHwnd() != NULL) 
	{
		m_btnWanDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+40+2, haoma_btn_y, m_btnWanDa.Width(), m_btnWanDa.Height(), SWP_NOZORDER);
	}
	if (m_btnWanXiao.GetSafeHwnd() != NULL) 
	{
		m_btnWanXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+40+2, haoma_btn_y, m_btnWanXiao.Width(), m_btnWanXiao.Height(), SWP_NOZORDER);
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

	if(m_btnQian_0.GetSafeHwnd() != NULL) 
	{
		m_btnQian_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_0.Width(), m_btnQian_0.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_1.GetSafeHwnd() != NULL) 
	{
		m_btnQian_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_1.Width(), m_btnQian_1.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_2.GetSafeHwnd() != NULL) 
	{
		m_btnQian_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_2.Width(), m_btnQian_2.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_3.GetSafeHwnd() != NULL) 
	{
		m_btnQian_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_3.Width(), m_btnQian_3.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_4.GetSafeHwnd() != NULL) 
	{
		m_btnQian_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_4.Width(), m_btnQian_4.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_5.GetSafeHwnd() != NULL) 
	{
		m_btnQian_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_5.Width(), m_btnQian_5.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_6.GetSafeHwnd() != NULL) 
	{
		m_btnQian_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_6.Width(), m_btnQian_6.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_7.GetSafeHwnd() != NULL) 
	{
		m_btnQian_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_7.Width(), m_btnQian_7.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_8.GetSafeHwnd() != NULL) 
	{
		m_btnQian_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_8.Width(), m_btnQian_8.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_9.GetSafeHwnd() != NULL) 
	{
		m_btnQian_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_9.Width(), m_btnQian_9.Height(), SWP_NOZORDER);
	}

	//ǧλ��ȫ����С������˫����
	if (m_btnQianQuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+40, haoma_btn_y + haoma_btn_row_height, m_btnQianQuan.Width(), m_btnQianQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnQianDa.GetSafeHwnd() != NULL) 
	{
		m_btnQianDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+40+2, haoma_btn_y + haoma_btn_row_height, m_btnQianDa.Width(), m_btnQianDa.Height(), SWP_NOZORDER);
	}
	if (m_btnQianXiao.GetSafeHwnd() != NULL) 
	{
		m_btnQianXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+40+2, haoma_btn_y + haoma_btn_row_height, m_btnQianXiao.Width(), m_btnQianXiao.Height(), SWP_NOZORDER);
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

	//ţţstatic
	if (m_staticNiu.GetSafeHwnd() != NULL) 
	{
		m_staticNiu.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticNiu.Width(), m_staticNiu.Height(), SWP_NOZORDER);
	}
	if (m_staticLhd.GetSafeHwnd() != NULL) 
	{
		m_staticLhd.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticLhd.Width(), m_staticLhd.Height(), SWP_NOZORDER);
	}
	if (m_statichzh.GetSafeHwnd() != NULL) 
	{
		m_statichzh.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_statichzh.Width(), m_statichzh.Height(), SWP_NOZORDER);
	}
	
	//��λ-------------------------------
	//��ѡ
	if (m_staticZuXuan.GetSafeHwnd() != NULL) 
	{
		m_staticZuXuan.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticDanHao.GetSafeHwnd() != NULL) 
	{
		m_staticDanHao.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*3, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticXuanHao.GetSafeHwnd() != NULL) 
	{
		m_staticXuanHao.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticZu120.GetSafeHwnd() != NULL) 
	{
		m_staticZu120.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticErChong.GetSafeHwnd() != NULL) 
	{
		m_staticErChong.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticErChong1.GetSafeHwnd() != NULL) 
	{
		m_staticErChong1.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*3, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticSanChong.GetSafeHwnd() != NULL) 
	{
		m_staticSanChong.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticSiChong.GetSafeHwnd() != NULL) 
	{
		m_staticSiChong.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
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
		m_btnBai_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_0.Width(), m_btnBai_0.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_1.GetSafeHwnd() != NULL) 
	{
		m_btnBai_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_1.Width(), m_btnBai_1.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_2.GetSafeHwnd() != NULL) 
	{
		m_btnBai_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_2.Width(), m_btnBai_2.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_3.GetSafeHwnd() != NULL) 
	{
		m_btnBai_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_3.Width(), m_btnBai_3.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_4.GetSafeHwnd() != NULL) 
	{
		m_btnBai_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_4.Width(), m_btnBai_4.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_5.GetSafeHwnd() != NULL) 
	{
		m_btnBai_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_5.Width(), m_btnBai_5.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_6.GetSafeHwnd() != NULL) 
	{
		m_btnBai_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_6.Width(), m_btnBai_6.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_7.GetSafeHwnd() != NULL) 
	{
		m_btnBai_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_7.Width(), m_btnBai_7.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_8.GetSafeHwnd() != NULL) 
	{
		m_btnBai_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_8.Width(), m_btnBai_8.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_9.GetSafeHwnd() != NULL) 
	{
		m_btnBai_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_9.Width(), m_btnBai_9.Height(), SWP_NOZORDER);
	}

	//��λ��ȫ����С������˫����
	if (m_btnBaiQuan.GetSafeHwnd() != NULL) 
	{
		m_btnBaiQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiQuan.Width(), m_btnBaiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiDa.GetSafeHwnd() != NULL) 
	{
		m_btnBaiDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2+2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiDa.Width(), m_btnBaiDa.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiXiao.GetSafeHwnd() != NULL) 
	{
		m_btnBaiXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2+2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiXiao.Width(), m_btnBaiXiao.Height(), SWP_NOZORDER);
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

	//��С��˫  -- ��
	if(m_btnDaWan.GetSafeHwnd() != NULL) 
	{
		m_btnDaWan.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y , m_btnDaWan.Width(), m_btnDaWan.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoWan.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoWan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y , m_btnXiaoWan.Width(), m_btnXiaoWan.Height(), SWP_NOZORDER);
	}
	if(m_btnDanWan.GetSafeHwnd() != NULL) 
	{
		m_btnDanWan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y, m_btnDanWan.Width(), m_btnDanWan.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangWan.GetSafeHwnd() != NULL) 
	{
		m_btnShuangWan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y , m_btnShuangWan.Width(), m_btnShuangWan.Height(), SWP_NOZORDER);
	}
	//��С��˫  -- ǧ
	if(m_btnDaQian.GetSafeHwnd() != NULL) 
	{
		m_btnDaQian.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height, m_btnDaQian.Width(), m_btnDaQian.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoQian.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoQian.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height, m_btnXiaoQian.Width(), m_btnXiaoQian.Height(), SWP_NOZORDER);
	}
	if(m_btnDanQian.GetSafeHwnd() != NULL) 
	{
		m_btnDanQian.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height, m_btnDanQian.Width(), m_btnDanQian.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangQian.GetSafeHwnd() != NULL) 
	{
		m_btnShuangQian.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height, m_btnShuangQian.Width(), m_btnShuangQian.Height(), SWP_NOZORDER);
	}
	//��С��˫  -- ��
	if(m_btnDaBai.GetSafeHwnd() != NULL) 
	{
		m_btnDaBai.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*2, m_btnDaBai.Width(), m_btnDaBai.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoBai.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoBai.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*2, m_btnXiaoBai.Width(), m_btnXiaoBai.Height(), SWP_NOZORDER);
	}
	if(m_btnDanBai.GetSafeHwnd() != NULL) 
	{
		m_btnDanBai.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*2, m_btnDanBai.Width(), m_btnDanBai.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangBai.GetSafeHwnd() != NULL) 
	{
		m_btnShuangBai.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*2, m_btnShuangBai.Width(), m_btnShuangBai.Height(), SWP_NOZORDER);
	}

	//��С��˫
	if(m_btnDaShi.GetSafeHwnd() != NULL) 
	{
		m_btnDaShi.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*3, m_btnDaShi.Width(), m_btnDaShi.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoShi.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoShi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*3, m_btnXiaoShi.Width(), m_btnXiaoShi.Height(), SWP_NOZORDER);
	}
	if(m_btnDanShi.GetSafeHwnd() != NULL) 
	{
		m_btnDanShi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*3, m_btnDanShi.Width(), m_btnDanShi.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangShi.GetSafeHwnd() != NULL) 
	{
		m_btnShuangShi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShuangShi.Width(), m_btnShuangShi.Height(), SWP_NOZORDER);
	}

	
	//ʮλ-------------------------------
	if (m_staticShi.GetSafeHwnd() != NULL) 
	{
		m_staticShi.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*3, m_staticShi.Width(), m_staticShi.Height(), SWP_NOZORDER);
	}

	if(m_btnShi_0.GetSafeHwnd() != NULL) 
	{
		m_btnShi_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_0.Width(), m_btnShi_0.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_1.GetSafeHwnd() != NULL) 
	{
		m_btnShi_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_1.Width(), m_btnShi_1.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_2.GetSafeHwnd() != NULL) 
	{
		m_btnShi_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_2.Width(), m_btnShi_2.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_3.GetSafeHwnd() != NULL) 
	{
		m_btnShi_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_3.Width(), m_btnShi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_4.GetSafeHwnd() != NULL) 
	{
		m_btnShi_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_4.Width(), m_btnShi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_5.GetSafeHwnd() != NULL) 
	{
		m_btnShi_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_5.Width(), m_btnShi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_6.GetSafeHwnd() != NULL) 
	{
		m_btnShi_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_6.Width(), m_btnShi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_7.GetSafeHwnd() != NULL) 
	{
		m_btnShi_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_7.Width(), m_btnShi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_8.GetSafeHwnd() != NULL) 
	{
		m_btnShi_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_8.Width(), m_btnShi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_9.GetSafeHwnd() != NULL) 
	{
		m_btnShi_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_9.Width(), m_btnShi_9.Height(), SWP_NOZORDER);
	}

	//ʮλ��ȫ����С������˫����
	if (m_btnShiQuan.GetSafeHwnd() != NULL) 
	{
		m_btnShiQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiQuan.Width(), m_btnShiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnShiDa.GetSafeHwnd() != NULL) 
	{
		m_btnShiDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2+2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiDa.Width(), m_btnShiDa.Height(), SWP_NOZORDER);
	}
	if (m_btnShiXiao.GetSafeHwnd() != NULL) 
	{
		m_btnShiXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2+2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiXiao.Width(), m_btnShiXiao.Height(), SWP_NOZORDER);
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
		m_btnDaGe.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*4, m_btnDaGe.Width(), m_btnDaGe.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoGe.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoGe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*4, m_btnXiaoGe.Width(), m_btnXiaoGe.Height(), SWP_NOZORDER);
	}
	if(m_btnDanGe.GetSafeHwnd() != NULL) 
	{
		m_btnDanGe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*4, m_btnDanGe.Width(), m_btnDanGe.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangGe.GetSafeHwnd() != NULL) 
	{
		m_btnShuangGe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*4, m_btnShuangGe.Width(), m_btnShuangGe.Height(), SWP_NOZORDER);
	}
	
	//��λ-------------------------------
	if (m_staticGe.GetSafeHwnd() != NULL) 
	{
		m_staticGe.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*4, m_staticGe.Width(), m_staticGe.Height(), SWP_NOZORDER);
	}

	if(m_btnGe_0.GetSafeHwnd() != NULL) 
	{
		m_btnGe_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_0.Width(), m_btnGe_0.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_1.GetSafeHwnd() != NULL) 
	{
		m_btnGe_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_1.Width(), m_btnGe_1.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_2.GetSafeHwnd() != NULL) 
	{
		m_btnGe_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_2.Width(), m_btnGe_2.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_3.GetSafeHwnd() != NULL) 
	{
		m_btnGe_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_3.Width(), m_btnGe_3.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_4.GetSafeHwnd() != NULL) 
	{
		m_btnGe_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_4.Width(), m_btnGe_4.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_5.GetSafeHwnd() != NULL) 
	{
		m_btnGe_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_5.Width(), m_btnGe_5.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_6.GetSafeHwnd() != NULL) 
	{
		m_btnGe_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_6.Width(), m_btnGe_6.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_7.GetSafeHwnd() != NULL) 
	{
		m_btnGe_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_7.Width(), m_btnGe_7.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_8.GetSafeHwnd() != NULL) 
	{
		m_btnGe_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_8.Width(), m_btnGe_8.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_9.GetSafeHwnd() != NULL) 
	{
		m_btnGe_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_9.Width(), m_btnGe_9.Height(), SWP_NOZORDER);
	}

	//��λ��ȫ����С������˫����
	if (m_btnGeQuan.GetSafeHwnd() != NULL) 
	{
		m_btnGeQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+40, haoma_btn_y + haoma_btn_row_height*4, m_btnGeQuan.Width(), m_btnGeQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnGeDa.GetSafeHwnd() != NULL) 
	{
		m_btnGeDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+42, haoma_btn_y + haoma_btn_row_height*4, m_btnGeDa.Width(), m_btnGeDa.Height(), SWP_NOZORDER);
	}
	if (m_btnGeXiao.GetSafeHwnd() != NULL) 
	{
		m_btnGeXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+42, haoma_btn_y + haoma_btn_row_height*4, m_btnGeXiao.Width(), m_btnGeXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnGeDan.GetSafeHwnd() != NULL) 
	{
		m_btnGeDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+47, haoma_btn_y + haoma_btn_row_height*4, m_btnGeDan.Width(), m_btnGeDan.Height(), SWP_NOZORDER);
	}
	if (m_btnGeShuang.GetSafeHwnd() != NULL) 
	{
		m_btnGeShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+47, haoma_btn_y + haoma_btn_row_height*4, m_btnGeShuang.Width(), m_btnGeShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnGeQing.GetSafeHwnd() != NULL) 
	{
		m_btnGeQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+47, haoma_btn_y + haoma_btn_row_height*4, m_btnGeQing.Width(), m_btnGeQing.Height(), SWP_NOZORDER);
	}

}

//��ť������ɾ�������㣬��Ͷ��Ͷע��׷��
void CChongQingSSC::AdjustNumView()
{
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
	//	m_listChqSSC.ShowWindow(SW_HIDE);
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
		m_btnQushi.SetWindowPos(NULL, btn_more_x-45, btn_more_y, m_btnQushi.Width(), m_btnQushi.Height(), SWP_NOZORDER);
	}

	if(m_btnBeiTouJia.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJia.SetWindowPos(NULL,edit_beitou_x+edit_beitou_width, edit_beitou_y-3, m_btnBeiTouJia.Width(), m_btnBeiTouJia.Height(), SWP_NOZORDER);
	}
	if(m_btnBeiTouJian.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJian.SetWindowPos(NULL,edit_beitou_x-m_btnBeiTouJian.Width(), edit_beitou_y-3, m_btnBeiTouJian.Width(), m_btnBeiTouJian.Height(), SWP_NOZORDER);
	}
}

//��ʽ����
void CChongQingSSC::AdjustDanShi()
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
	CWnd* pWnd = GetDlgItem(IDC_CHK_WAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y-5, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_CHK_QIAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+60, edit_haomao_danshi_y-5, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_CHK_BAI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+120, edit_haomao_danshi_y-5, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_CHK_SHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+180, edit_haomao_danshi_y-5, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_CHKGE);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+240, edit_haomao_danshi_y-5, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_STATIC_RENXUAN_TIP);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+300, edit_haomao_danshi_y, 300, wanfa_sel_height, SWP_NOZORDER);
	}


}

//��ע��ť
void CChongQingSSC::AdjustAdd()
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
	if(m_btWxzxHelp.GetSafeHwnd() != NULL)
	{
		m_btWxzxHelp.SetWindowPos(NULL,keyboardinput_x+m_btWxzxTip.Width()+5,mouseinput_y-30,m_btWxzxHelp.Width(),m_btWxzxHelp.Height(),SWP_NOZORDER);
	}
}
void CChongQingSSC::HideLonghudou()
{
	m_btnLong.ShowWindow(SW_HIDE);
	m_btnHu.ShowWindow(SW_HIDE);
	m_btnHe.ShowWindow(SW_HIDE);
	m_staticLhd.ShowWindow(SW_HIDE);
}
void CChongQingSSC::ShowLonghudou()
{
	m_btnLong.ShowWindow(SW_SHOW);
	m_btnHu.ShowWindow(SW_SHOW);
	m_btnHe.ShowWindow(SW_SHOW);

	m_staticLhd.ShowWindow(SW_SHOW);
}
void CChongQingSSC::HideHeZhiNums()
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
void CChongQingSSC::ShowHeZhiNums()
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
void CChongQingSSC::HideErXingHeZhiNums()
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
}
void CChongQingSSC::ShowErXingHeZhiNums()
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
}
void CChongQingSSC::HideWanNums()
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

	m_btnWanQuan.ShowWindow(SW_HIDE);
	m_btnWanDa.ShowWindow(SW_HIDE);
	m_btnWanXiao.ShowWindow(SW_HIDE);
	m_btnWanDan.ShowWindow(SW_HIDE);
	m_btnWanShuang.ShowWindow(SW_HIDE);
	m_btnWanQing.ShowWindow(SW_HIDE);
}

void CChongQingSSC::HideQianNums()
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

	m_btnQianQuan.ShowWindow(SW_HIDE);
	m_btnQianDa.ShowWindow(SW_HIDE);
	m_btnQianXiao.ShowWindow(SW_HIDE);
	m_btnQianDan.ShowWindow(SW_HIDE);
	m_btnQianShuang.ShowWindow(SW_HIDE);
	m_btnQianQing.ShowWindow(SW_HIDE);
}

void CChongQingSSC::HideBaiNums()
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

	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticErChong1.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

	m_btnBaiQuan.ShowWindow(SW_HIDE);
	m_btnBaiDa.ShowWindow(SW_HIDE);
	m_btnBaiXiao.ShowWindow(SW_HIDE);
	m_btnBaiDan.ShowWindow(SW_HIDE);
	m_btnBaiShuang.ShowWindow(SW_HIDE);
	m_btnBaiQing.ShowWindow(SW_HIDE);
}

void CChongQingSSC::HideShiNums()
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
	m_staticDanHao.ShowWindow(SW_HIDE);
	m_btnShiQuan.ShowWindow(SW_HIDE);
	m_btnShiDa.ShowWindow(SW_HIDE);
	m_btnShiXiao.ShowWindow(SW_HIDE);
	m_btnShiDan.ShowWindow(SW_HIDE);
	m_btnShiShuang.ShowWindow(SW_HIDE);
	m_btnShiQing.ShowWindow(SW_HIDE);
}

void CChongQingSSC::HideGeNums()
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

	m_btnGeQuan.ShowWindow(SW_HIDE);
	m_btnGeDa.ShowWindow(SW_HIDE);
	m_btnGeXiao.ShowWindow(SW_HIDE);
	m_btnGeDan.ShowWindow(SW_HIDE);
	m_btnGeShuang.ShowWindow(SW_HIDE);
	m_btnGeQing.ShowWindow(SW_HIDE);
}

void CChongQingSSC::ShowWanNums()
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

	m_btnWanQuan.ShowWindow(SW_SHOW);
	m_btnWanDa.ShowWindow(SW_SHOW);
	m_btnWanXiao.ShowWindow(SW_SHOW);
	m_btnWanDan.ShowWindow(SW_SHOW);
	m_btnWanShuang.ShowWindow(SW_SHOW);
	m_btnWanQing.ShowWindow(SW_SHOW);
}

void CChongQingSSC::ShowNiuNums()
{
	m_btnNiu_1.ShowWindow(SW_SHOW);
	m_btnNiu_2.ShowWindow(SW_SHOW);
	m_btnNiu_3.ShowWindow(SW_SHOW);
	m_btnNiu_4.ShowWindow(SW_SHOW);
	m_btnNiu_5.ShowWindow(SW_SHOW);
	m_btnNiu_6.ShowWindow(SW_SHOW);
	m_btnNiu_7.ShowWindow(SW_SHOW);
	m_btnNiu_8.ShowWindow(SW_SHOW);
	m_btnNiu_9.ShowWindow(SW_SHOW);
	m_btnNiu_10.ShowWindow(SW_SHOW);

	m_staticNiu.ShowWindow(SW_SHOW);

	m_btnNiuQuan.ShowWindow(SW_SHOW);
	m_btnNiuDa.ShowWindow(SW_SHOW);
	m_btnNiuXiao.ShowWindow(SW_SHOW);
	m_btnNiuDan.ShowWindow(SW_SHOW);
	m_btnNiuShuang.ShowWindow(SW_SHOW);
	m_btnNiuQing.ShowWindow(SW_SHOW);
}
void CChongQingSSC::HideNiuNums()
{
	m_btnNiu_0.ShowWindow(SW_HIDE);
	m_btnNiu_1.ShowWindow(SW_HIDE);
	m_btnNiu_2.ShowWindow(SW_HIDE);
	m_btnNiu_3.ShowWindow(SW_HIDE);
	m_btnNiu_4.ShowWindow(SW_HIDE);
	m_btnNiu_5.ShowWindow(SW_HIDE);
	m_btnNiu_6.ShowWindow(SW_HIDE);
	m_btnNiu_7.ShowWindow(SW_HIDE);
	m_btnNiu_8.ShowWindow(SW_HIDE);
	m_btnNiu_9.ShowWindow(SW_HIDE);
	m_btnNiu_10.ShowWindow(SW_HIDE);

	m_staticNiu.ShowWindow(SW_HIDE);

	m_btnNiuQuan.ShowWindow(SW_HIDE);
	m_btnNiuDa.ShowWindow(SW_HIDE);
	m_btnNiuXiao.ShowWindow(SW_HIDE);
	m_btnNiuDan.ShowWindow(SW_HIDE);
	m_btnNiuShuang.ShowWindow(SW_HIDE);
	m_btnNiuQing.ShowWindow(SW_HIDE);
}

void CChongQingSSC::ShowQianNums()
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

	m_btnQianQuan.ShowWindow(SW_SHOW);
	m_btnQianDa.ShowWindow(SW_SHOW);
	m_btnQianXiao.ShowWindow(SW_SHOW);
	m_btnQianDan.ShowWindow(SW_SHOW);
	m_btnQianShuang.ShowWindow(SW_SHOW);
	m_btnQianQing.ShowWindow(SW_SHOW);
}

void CChongQingSSC::ShowBaiNums()
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

	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_SHOW);

	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticErChong1.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

	m_btnBaiQuan.ShowWindow(SW_SHOW);
	m_btnBaiDa.ShowWindow(SW_SHOW);
	m_btnBaiXiao.ShowWindow(SW_SHOW);
	m_btnBaiDan.ShowWindow(SW_SHOW);
	m_btnBaiShuang.ShowWindow(SW_SHOW);
	m_btnBaiQing.ShowWindow(SW_SHOW);
}

void CChongQingSSC::ShowShiNums()
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
	m_staticDanHao.ShowWindow(SW_HIDE);
	m_btnShiQuan.ShowWindow(SW_SHOW);
	m_btnShiDa.ShowWindow(SW_SHOW);
	m_btnShiXiao.ShowWindow(SW_SHOW);
	m_btnShiDan.ShowWindow(SW_SHOW);
	m_btnShiShuang.ShowWindow(SW_SHOW);
	m_btnShiQing.ShowWindow(SW_SHOW);
	m_staticErChong1.ShowWindow(SW_HIDE);
}

void CChongQingSSC::ShowGeNums()
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

	m_btnGeQuan.ShowWindow(SW_SHOW);
	m_btnGeDa.ShowWindow(SW_SHOW);
	m_btnGeXiao.ShowWindow(SW_SHOW);
	m_btnGeDan.ShowWindow(SW_SHOW);
	m_btnGeShuang.ShowWindow(SW_SHOW);
	m_btnGeQing.ShowWindow(SW_SHOW);
}

void CChongQingSSC::ShowZuXuan()
{
	m_staticZuXuan.ShowWindow(SW_SHOW);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

	m_staticBai.ShowWindow(SW_HIDE);
}
void CChongQingSSC::ShowDanHao()
{
	m_staticDanHao.ShowWindow(SW_SHOW);
	m_staticShi.ShowWindow(SW_HIDE);
	m_staticErChong1.ShowWindow(SW_HIDE);
}
void CChongQingSSC::ShowErChong1()
{
	m_staticDanHao.ShowWindow(SW_HIDE);
	m_staticShi.ShowWindow(SW_HIDE);
	m_staticErChong1.ShowWindow(SW_SHOW);
}
void CChongQingSSC::ShowXuanHao()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_SHOW);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

}
void CChongQingSSC::ShowZu120()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_SHOW);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

}

// void CChongQingSSC::HideZuXuan()
// {
// 	m_staticZuXuan.ShowWindow(SW_HIDE);
// 	m_staticBuDingWei.ShowWindow(SW_HIDE);
// 	m_staticBai.ShowWindow(SW_HIDE);
// }

void CChongQingSSC::ShowBuDingWei()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_SHOW);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

}

void CChongQingSSC::ShowErChong()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_SHOW);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

}
void CChongQingSSC::ShowSxErChong()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_SHOW);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

}
void CChongQingSSC::ShowSanChong()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_SHOW);
	m_staticSiChong.ShowWindow(SW_HIDE);

}
void CChongQingSSC::ShowSiChong()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_SHOW);

}
void CChongQingSSC::ShowSxSanChong()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_SHOW);
	m_staticSiChong.ShowWindow(SW_HIDE);

}

void CChongQingSSC::ShowDaXiaoDanShuang()
{
	m_staticShi.ShowWindow(SW_SHOW);
	m_staticGe.ShowWindow(SW_SHOW);

	m_btnDaShi.ShowWindow(SW_SHOW);
	m_btnXiaoShi.ShowWindow(SW_SHOW);
	m_btnDanShi.ShowWindow(SW_SHOW);
	m_btnShuangShi.ShowWindow(SW_SHOW);
	m_btnDaGe.ShowWindow(SW_SHOW);
	m_btnXiaoGe.ShowWindow(SW_SHOW);
	m_btnDanGe.ShowWindow(SW_SHOW);
	m_btnShuangGe.ShowWindow(SW_SHOW);
}
void CChongQingSSC::HideDaXiaoDanShuang()
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
void CChongQingSSC::ShowDaXiaoDanShuangHzh()
{
	m_statichzh.ShowWindow(SW_SHOW);
	m_btnDaBai.ShowWindow(SW_SHOW);
	m_btnXiaoBai.ShowWindow(SW_SHOW);
	m_btnDanBai.ShowWindow(SW_SHOW);
	m_btnShuangBai.ShowWindow(SW_SHOW);

}
void CChongQingSSC::HideDaXiaoDanShuangHzh()
{
	m_statichzh.ShowWindow(SW_HIDE);
	m_btnDaBai.ShowWindow(SW_HIDE);
	m_btnXiaoBai.ShowWindow(SW_HIDE);
	m_btnDanBai.ShowWindow(SW_HIDE);
	m_btnShuangBai.ShowWindow(SW_HIDE);

}
void CChongQingSSC::ShowDaXiaoDanShuangDw()
{
	m_statichzh.ShowWindow(SW_HIDE);

	m_staticWan.ShowWindow(SW_SHOW);
	m_staticQian.ShowWindow(SW_SHOW);
	m_staticBai.ShowWindow(SW_SHOW);
	m_staticShi.ShowWindow(SW_SHOW);
	m_staticGe.ShowWindow(SW_SHOW);

	m_btnDaWan.ShowWindow(SW_SHOW);
	m_btnXiaoWan.ShowWindow(SW_SHOW);
	m_btnDanWan.ShowWindow(SW_SHOW);
	m_btnShuangWan.ShowWindow(SW_SHOW);
	m_btnDaQian.ShowWindow(SW_SHOW);
	m_btnXiaoQian.ShowWindow(SW_SHOW);
	m_btnDanQian.ShowWindow(SW_SHOW);
	m_btnShuangQian.ShowWindow(SW_SHOW);
	m_btnDaBai.ShowWindow(SW_SHOW);
	m_btnXiaoBai.ShowWindow(SW_SHOW);
	m_btnDanBai.ShowWindow(SW_SHOW);
	m_btnShuangBai.ShowWindow(SW_SHOW);
	m_btnDaShi.ShowWindow(SW_SHOW);
	m_btnXiaoShi.ShowWindow(SW_SHOW);
	m_btnDanShi.ShowWindow(SW_SHOW);
	m_btnShuangShi.ShowWindow(SW_SHOW);
	m_btnDaGe.ShowWindow(SW_SHOW);
	m_btnXiaoGe.ShowWindow(SW_SHOW);
	m_btnDanGe.ShowWindow(SW_SHOW);
	m_btnShuangGe.ShowWindow(SW_SHOW);
}

void CChongQingSSC::HideDaXiaoDanShuangDw()
{
	m_statichzh.ShowWindow(SW_HIDE);

	m_btnDaWan.ShowWindow(SW_HIDE);
	m_btnXiaoWan.ShowWindow(SW_HIDE);
	m_btnDanWan.ShowWindow(SW_HIDE);
	m_btnShuangWan.ShowWindow(SW_HIDE);
	m_btnDaQian.ShowWindow(SW_HIDE);
	m_btnXiaoQian.ShowWindow(SW_HIDE);
	m_btnDanQian.ShowWindow(SW_HIDE);
	m_btnShuangQian.ShowWindow(SW_HIDE);
	m_btnDaBai.ShowWindow(SW_HIDE);
	m_btnXiaoBai.ShowWindow(SW_HIDE);
	m_btnDanBai.ShowWindow(SW_HIDE);
	m_btnShuangBai.ShowWindow(SW_HIDE);
	m_btnDaShi.ShowWindow(SW_HIDE);
	m_btnXiaoShi.ShowWindow(SW_HIDE);
	m_btnDanShi.ShowWindow(SW_HIDE);
	m_btnShuangShi.ShowWindow(SW_HIDE);
	m_btnDaGe.ShowWindow(SW_HIDE);
	m_btnXiaoGe.ShowWindow(SW_HIDE);
	m_btnDanGe.ShowWindow(SW_HIDE);
	m_btnShuangGe.ShowWindow(SW_HIDE);
}

void CChongQingSSC::HideDanShiEdit()
{
	m_RenxuanKeyboardInput = false;
	//m_editDanShiHaoMa.ShowWindow(SW_HIDE);
	m_richDanshiHaoma.ShowWindow(SW_HIDE);
}

void CChongQingSSC::ShowDanShiEdit()
{
	m_RenxuanKeyboardInput = true;
	m_richDanshiHaoma.ShowWindow(SW_SHOW);

	if (m_richDanshiHaoma.GetSafeHwnd() != NULL) 
	{
		if(m_gameKind == RenXuan4 || m_gameKind == RenXuan3 || m_gameKind == RenXuan2|| m_gameKind == Ren2Zuxuan_Fushi|| m_gameKind == Ren3Zu3_Fushi|| m_gameKind == Ren3Zu6_Fushi|| m_gameKind == Ren3Hunhe_Zuxuan)
			m_richDanshiHaoma.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y+25, edit_haomao_danshi_width, edit_haomao_danshi_height-25, SWP_NOZORDER);
		else
			m_richDanshiHaoma.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
	}

 //	m_editDanShiHaoMa.ShowWindow(SW_SHOW);
}

	//��ѡ���е���ǧ���٣�ʮ����λ��
void CChongQingSSC::ShowRenxuanWeiShu()
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
	SetCheckRenXuan(true);
}

void CChongQingSSC::HideRenxuanWeiShu()
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

	SetCheckRenXuan(false);
}

void CChongQingSSC::CheckWanFaBtn(CTextButton& btn)
{
	m_btnHouSanZhiXuan.SetPushed(false);
	m_btnQianSanZhiXuan.SetPushed(false);
	m_btnHouSanZuXuan.SetPushed(false);
	m_btnQianSanZuXuan.SetPushed(false);
	m_btnWuXingHouEr.SetPushed(false);
	m_btnWuXingQianEr.SetPushed(false);
	m_btnHouErZuXuan.SetPushed(false);
	m_btnQianErZuXuan.SetPushed(false);
	m_btnQianErXing.SetPushed(false);
	m_btnDaXiaoDanShuang.SetPushed(false);
	m_btnHouSanHeZhi.SetPushed(false);
	m_btnBuDingWei.SetPushed(false);
	m_btnDingWeiDan.SetPushed(false);
	m_btnQuwei.SetPushed(false);
	m_btnNiuNiu.SetPushed(false);
	m_btnLhd.SetPushed(false);

	btn.SetPushed(true);
}
VOID CChongQingSSC::SendQuerySystemTime()
{
	return;
}
VOID CChongQingSSC::SendToServer(int nSendType)
{
	
	if(nSendType == 1)
	{
		m_bTouzhu = true;
		if(m_bTouzhu)
		{
			m_bTouzhu = false;

			//��ȡ�����ļ�
			//����Ŀ¼
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFileName[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);


			m_nTestCount = GetPrivateProfileInt(_T("TESTVERSION"),_T("TZHTESTCOUNT"),1,szFileName);

			int nSleepCount = GetPrivateProfileInt(_T("TESTVERSION"),_T("TZHTESTSLEEP"),1,szFileName);
			m_nTouzhu=0;
			CString strLog;
		//	for(int i = 0; i< m_nTestCount;i++)
			{
				CTime now;
				theApp.GetTime(now);
// 				if(m_RenxuanKeyboardInput)
//				{
				int nCount = m_listChqSSC.GetItemCount();
				int first = 0;
				m_nTzhSign++;

				for(int j = 0;j < nCount;j++)
				{
					CString strDazhu = vecAllNums[nCount-j-1].c_str();
					int nLen = strDazhu.GetLength();
					if(nLen > 8000)
					{
						int nSendCount = 0;
						int nEnd = 1;

						CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
						ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
						TouzhuCQSSC.dwUserID = theAccount.user_id;
						CString strBeishu = m_listChqSSC.GetItemText(j, 5);
						int nBeishu = _ttoi(strBeishu);
						TouzhuCQSSC.nBeitou = nBeishu;
						CString strWanfa = m_listChqSSC.GetItemText(j, 3);
						int nKind = GetGameKindByDesc(strWanfa);
						TouzhuCQSSC.nGameKind = nKind;
						TouzhuCQSSC.nGameType = m_TypeID;

						CString strMoshi = m_listChqSSC.GetItemText(j,1);

						int nScoreType = GetMoshiRet(strMoshi);

						TouzhuCQSSC.nMoshi = nScoreType;

						TouzhuCQSSC.nSign = m_nTzhSign;

						CString strQiHao;
						if(m_TypeID == CZChongQingSSC)
							strQiHao = m_chqRule.GetNextExpect();
						else if(m_TypeID == CZ_TianJinSSC)
							strQiHao = m_jxRule.GetNextExpect();
						else if(m_TypeID == CZXinJiangSSC)
							strQiHao = m_xjRule.GetNextExpect();
						else if(m_TypeID == CZ_FENFEN_CAI)
							strQiHao = m_ffcRule.GetNextExpect();
						else if(m_TypeID == CZ_TXfenfencai)
							strQiHao = m_txffcRule.GetNextExpect();
						else if(m_TypeID == CZ_QQfenfencai)
							strQiHao = m_qqffcRule.GetNextExpect();
						else if(m_TypeID == CZ_ErFenCai)//���ֲ�δ�� 9.28
							strQiHao = m_efcRule.GetNextExpect();
						else if(m_TypeID == CZ_WUFEN_CAI)
							strQiHao = m_wfcRule.GetNextExpect();
						else if(m_TypeID == CZ_BJ5FC)
							strQiHao = m_bj5fcRule.GetNextExpect();
						else if(m_TypeID == CZ_HGYDWFC)
							strQiHao = m_hgRule.GetNextExpect();
						else if(m_TypeID == CZ_JiaNaDaSSC)
							strQiHao = m_jndRule.GetNextExpect();

						if(strDazhu.IsEmpty())
						{
							m_DlgStatus.HideStatusWindow();
							MyMessageBox(L"�㻹δѡ����룬��ѡ��");
							OnBnClickedBtnClsList();
							return ;
						}

						string	strAllHaoma;
						ChangeStringToA(strDazhu, strAllHaoma);

						string strQi;
						ChangeStringToA(strQiHao, strQi);

						memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());


						byte pDestBuffer[102400];
						int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

						int nSendSize = 0;
						TouzhuCQSSC.nEnd = 1;
						int nZhushu = 0;
						CString strZhu = m_listChqSSC.GetItemText(j, 4);
						nZhushu += _ttoi(strZhu);

						TouzhuCQSSC.nZhushu = nZhushu;

						if(nZhushu<=0)
						{
							m_DlgStatus.HideStatusWindow();
							OnBnClickedBtnClsList();
							return;
						}
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
							nSendSize += nHaoMaLen;
							m_bCanSend = false;
							nSendCount++;
						}


						if(first == 0)
							m_DlgStatus.ShowStatusWindow(TEXT("�����ύ���ݣ����Ժ�..."));
						first++;
					}
					m_bCanSend = true;
				}



				int itm_cnt = m_listChqSSC.GetItemCount();
				int nNowKind = 10000;
				int nNowBeishu = 100000;
				int nNowMoshi = 10000;
				CString strTouZhuhaoma;

				map<int,int> mapHaoma;
				mapHaoma.clear();
				for(int i = 0;i < itm_cnt;i++)
				{
					strTouZhuhaoma.Empty();
					CString haoma = m_listChqSSC.GetItemText(i,0);
					if(haoma == _T("������ӳɹ�"))
						continue;
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
						haoma = m_listChqSSC.GetItemText(j,0);
						if(haoma == _T("������ӳɹ�"))
							continue;
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
							if(nKind == DaXiaoDanShuang || nKind == DaXiaoDanShuang_dw)
							{
								CString strTemp = haoma;
								strTemp.Replace(L"��",L"9");
								strTemp.Replace(L"С",L"1");
								strTemp.Replace(L"��",L"3");
								strTemp.Replace(L"˫",L"2");

								haoma = strTemp;
							}
							else if(nKind == Ren3Zu3_Fushi||nKind==Ren3Zu6_Fushi||m_gameKind ==Ren2Zuxuan_Fushi||m_gameKind ==Ren3Hunhe_Zuxuan || nKind == NiuNiu_Num)
							{
								CString strVect = vecAllNums[itm_cnt-j-1].c_str();

								haoma = strVect;
							}
							else if( kind == SSC_LongHu)
							{
								CString strVect = vecAllNums[itm_cnt-j-1].c_str();

								haoma = strVect;

								strTouZhuhaoma+= haoma;
								if(haoma[haoma.GetLength()-1] != '|')
									strTouZhuhaoma+=TEXT("|");
								break;
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
					if(m_TypeID == CZChongQingSSC)
						strQiHao = m_chqRule.GetNextExpect();
					else if(m_TypeID == CZ_TianJinSSC)
						strQiHao = m_jxRule.GetNextExpect();
					else if(m_TypeID == CZXinJiangSSC)
						strQiHao = m_xjRule.GetNextExpect();
					else if(m_TypeID == CZ_FENFEN_CAI)
						strQiHao = m_ffcRule.GetNextExpect();
					else if(m_TypeID == CZ_TXfenfencai)
						strQiHao = m_txffcRule.GetNextExpect();
					else if(m_TypeID == CZ_QQfenfencai)
						strQiHao = m_qqffcRule.GetNextExpect();
					else if(m_TypeID == CZ_ErFenCai)
						strQiHao = m_efcRule.GetNextExpect();
					else if(m_TypeID == CZ_WUFEN_CAI)
						strQiHao = m_wfcRule.GetNextExpect();
					else if(m_TypeID == CZ_BJ5FC)
						strQiHao = m_bj5fcRule.GetNextExpect();
					else if(m_TypeID == CZ_HGYDWFC)
						strQiHao = m_hgRule.GetNextExpect();
					else if(m_TypeID == CZ_JiaNaDaSSC)
						strQiHao = m_jndRule.GetNextExpect();

				//	strQiHao.Format(L"1617398");


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


				DWORD dwTick=GetTickCount();
				while ((GetTickCount() - dwTick ) < nSleepCount)
				{
					MSG msg;
					if(GetMessage (&msg, NULL, 0, 0))
					{
						TranslateMessage (&msg) ;

						DispatchMessage (&msg) ;

					}
// 					if(m_bCanSend)
// 						break;

				}

			}
//			m_nTzhSign++;

			SetTimer(TimerStatus,60000,NULL);
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
		HideLonghudou();
		m_btnLock.SetWindowText(mapFandian[m_gameKind]);
		m_btnLock.EnableWindow(TRUE);
		m_btWxzxTip.ShowWindow(SW_SHOW);
		if (m_gameKind == WuxingZhixuan)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���Ӹ���ʮ���١�ǧ����λ��ѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:23456;��������:23456;��������ֱѡ��"));
		}
		else if(m_gameKind == WuxingQianSi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ǧ���١�ʮλ��ѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:3456;��������:3456*,��������ǰ��ֱѡ��"));
		}
		else if(m_gameKind == WuxingHousi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨����ǧ���١�ʮ����λ��ѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:3456;��������:*3456,�������Ǻ���ֱѡ��"));
		}
		else if(m_gameKind == QSZuXuan24||m_gameKind == HSZuXuan24)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��0-9������ѡ��4���������һע����ѡ�����뿪�������ǧλ����λ��ʮλ����λ��ͬ����˳���ޣ���Ϊ�н���"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע������0356������������ĸ�����ֻҪ����0��3��5��6��Ϊ�н���"));
		}
		else if(m_gameKind == QSZuXuan12||m_gameKind == HSZuXuan12)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ѡ��һ�����غ�����������ź������һע����ѡ���ź����뿪��������ͬ������ѡ���غ����ڿ��������г�����2�Σ���Ϊ�н���"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע���������غţ�3 ���ţ�5��6ֻҪ���������а���5��6��3��3����Ϊ�н���"));
		}
		else if(m_gameKind == QSZuXuan6||m_gameKind == HSZuXuan6)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ѡ��2�����غ������һע����ѡ��2�����غ����ڿ��������зֱ������2�Σ���Ϊ�н���"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע���������غ�:5��6��ֻҪ���������а���5��5��6��6����Ϊ�н���"));
		}
		else if(m_gameKind == QSZuXuan4||m_gameKind == HSZuXuan4)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("ѡ��һ�����غ����һ�����ź������һע����ѡ���ź����뿪��������ͬ������ѡ���غ����ڿ��������г�����3�Σ���Ϊ�н���"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע���������غţ�3 ���ţ�5��ֻҪ���������а���3��3��3��5����Ϊ�н���"));
		}
		else if(m_gameKind == QianSanZhiXuan_FuShi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ǧ����λ��ѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:3,4,5;��������:345**,��������ǰ��ֱѡ��"));
		}
		else if(m_gameKind == HouSanZhiXuan_FuShi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���Ӱ١�ʮ����λ��ѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:3,4,5;��������:**345,�������Ǻ���ֱѡ��"));
		}
		else if(m_gameKind == WuxingZhongsan)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨����ǧ���١�ʮλ��ѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:3,4,5;��������:*345*,������������ֱѡ��"));
		}
		else if(m_gameKind == QianSanZuXuan_ZuLiuFuShi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,8;��������ǰ��λ:1��2��1��5��1��8(˳����),����ǰ����ѡ��һ�Ƚ���"));
		}
		else if(m_gameKind == QianSanZuXuan_ZuSanFuShi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8,8;��������ǰ��λ:1��5��2��8(˳����),����ǰ����ѡ��һ�Ƚ���"));
		}
		else if(m_gameKind == ZhongsanZuliu)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,8;���������м���λ:1��2��1��5��1��8(˳����),����������ѡ��һ�Ƚ���"));
		}
		else if(m_gameKind == ZhongsanZusan)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8,8;���������м���λ:1��5��2��8(˳����),����������ѡ��һ�Ƚ���"));
		}
		else if(m_gameKind == HouSanZuXuan_ZuLiuFuShi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,8;�����������λ:1��2��1��5��1��8(˳����),���к�����ѡ��һ�Ƚ���"));
		}
		else if(m_gameKind == HouSanZuXuan_ZuSanFuShi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8,8;��������ǰ��λ:1��5��2��8(˳����),����ǰ����ѡ��һ�Ƚ���"));
		}
		else if(m_gameKind == WuXingQianEr_FuShi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ǧλ��ѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;��������ǰ��λ:58,����ǰ��ֱѡһ�Ƚ���"));
		}
		else if(m_gameKind == WuXingHouEr_FuShi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨����ʮ����λ��ѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;����������λ:58,���к��ֱѡһ�Ƚ���"));
		}
		else if(m_gameKind == QianErZuXuan_FuShi )
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;��������ǰ��λ:1��5��1��8(˳����),����ǰ����ѡһ�Ƚ���"));
		}
		else if(m_gameKind == HouErZuXuan_FuShi )
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;�����������λ:1��5��1��8(˳����),���к����ѡһ�Ƚ���"));
		}
		else if(m_gameKind == QianSanZhiXuanHeZhi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��ֵ1;��������ǰ��λ:001,010,100,����ǰ����ֵһ�Ƚ���"));
		}
		else if(m_gameKind == ZhongSanZhiXuanHeZhi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��ֵ1;���������м���λ:001,010,100,����������ֵһ�Ƚ���"));
		}
		else if(m_gameKind == HouSanZhiXuanHeZhi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��ֵ1;�����������λ:001,010,100,���к�����ֵһ�Ƚ���"));
		}
		else if(m_gameKind == QianErZhixuan_Hezhi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��ֵ1;��������ǰ��λ:01,10,����ǰ����ֵһ�Ƚ���"));
		}
		else if(m_gameKind == HouErZhixuan_Hezhi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��ֵ1;�����������λ:01,10,���к����ֵһ�Ƚ���"));
		}
		else if(m_gameKind == DaXiaoDanShuang)
		{
			//m_btnLock.EnableWindow(FALSE);
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨����ʮ����λ��ѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��;��������ʮλ���λ����,���к����С��˫һ�Ƚ���"));
		}
		else if(m_gameKind == DaXiaoDanShuang_dw)
		{
		//	m_btnLock.EnableWindow(FALSE);
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������λ��ǧλ����λ��ʮλ����λ����λ��������ѡ��1������С������˫�����һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��λ��;�������룺��λ��,���ж�λ��С��˫һ�Ƚ���"));
		}
		else if(m_gameKind == BuDingWei_HouSan)
		{
			//m_btnLock.EnableWindow(FALSE);
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:1;�����������λ�����ٳ���1��1,���к�������λһ�Ƚ���"));
		}
		else if(m_gameKind == BuDingWei_QianSan)
		{
			//m_btnLock.EnableWindow(FALSE);
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:1;��������ǰ��λ�����ٳ���1��1,���к�������λһ�Ƚ���"));
		}
		else if(m_gameKind == BuDingWei_ZhongSan)
		{
			//m_btnLock.EnableWindow(FALSE);
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:1;���������м���λ�����ٳ���1��1,������������λһ�Ƚ���"));
		}
		else if(m_gameKind == BuDingWei_QianEr)
		{
			//m_btnLock.EnableWindow(FALSE);
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:1;��������ǰ��λ�����ٳ���1��1,����ǰ������λһ�Ƚ���"));
		}
		else if(m_gameKind == BuDingWei_HouEr)
		{
		//	m_btnLock.EnableWindow(FALSE);
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ��һ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:1;�����������λ�����ٳ���1��1,���к������λһ�Ƚ���"));
		}
		else if(m_gameKind == DingWeiDan)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ǧ���١�ʮ����λ����λ��������ѡ��1����1�����Ϻ��롣"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��λ 1;����������λ��1,���ж�λ����λһ�Ƚ���"));
		}
		else if(m_gameKind == QW_YiFanFengShun)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨����0-9������ѡ��1�����Ϻ��롣"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:8,��������:���ٳ���1��8,����һ����˳��"));
		}
		else if(m_gameKind == QW_HaoShiChengShuang)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨����0-9������ѡ��1�����ϵĶ��غ��롣"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:8,��������:���ٳ���2��8,���к��³�˫��"));
		}
		else if(m_gameKind == QW_SanXingBaoXi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨����0-9������ѡ��1�����ϵ����غ��롣"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:8,��������:���ٳ���3��8,�������Ǳ�ϲ��"));
		}
		else if(m_gameKind == QW_SiJiFaCai)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨����0-9������ѡ��1�����ϵ����غ��롣"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:8,��������:���ٳ���4��8,�����ļ����ơ�"));
		}
		else if(m_gameKind == NiuNiu_Num)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨����ţ1��ţ9����ѡ��һ����1�����Ϻ��룬��ѡ�����뿪�����������λ������ú���һ�¼�Ϊ�н���"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:ţ1,��������:����λ���,�����ĸ�λΪ1,����ţţ��"));
		}
		else if(m_gameKind == RenXuan2)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ǧ���١�ʮ��������λѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:5,8;���������ӦͶע������ѡ��λ�ã�5,8,������ѡ��һ�Ƚ���"));
		}
		else if(m_gameKind == Ren2Zuxuan_Fushi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע����ѡ��������Ӧλ��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:��λǧλ 5,8;����������λ��ǧλ:1��5��1��8(˳����),�����ζ���ѡһ�Ƚ���"));
		}
		else if(m_gameKind == RenXuan3)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ǧ���١�ʮ��������λѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,8;���������ӦͶע������ѡ��λ�ã�2,5,8,������ѡ��һ�Ƚ���"));
		}
		else if(m_gameKind == Ren3Hunhe_Zuxuan)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨��3������Ϊһע����ѡ����������϶�Ӧ��ѡλ�õ�������������Ϊ�н���"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:Ͷע��λǧλ��λ (0,0,1),����������ǧ����λ����:0,0,1,˳����,���е��������һ�Ƚ���"));
		}
		else if(m_gameKind == RenXuan4)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ǧ���١�ʮ��������λѡһ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:0,2,5,8;���������ӦͶע������ѡ��λ�ã�0,2,5,8,������ѡ��һ�Ƚ���"));
		}
		else if(m_gameKind == QianSanHunhe)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:�ֱ�Ͷע(0,0,1),�Լ�(1,2,3),��������ǰ��λ����:(1)0,0,1,˳����,���е�����һ�Ƚ�;����(2)1,2,3,˳����,���е�����һ�Ƚ���"));
		}
		else if( m_gameKind == HouSanHunhe )
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:�ֱ�Ͷע(0,0,1),�Լ�(1,2,3),�����������λ����:(1)0,0,1,˳����,���е�����һ�Ƚ�;����(2)1,2,3,˳����,���е�����һ�Ƚ���"));
		}
		else if(m_gameKind == ZhongSanHunhe)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ֶ�������룬��������1����λ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:�ֱ�Ͷע(0,0,1),�Լ�(1,2,3),���������м���λ����:(1)0,0,1,˳����,���е�����һ�Ƚ�;����(2)1,2,3,˳����,���е�����һ�Ƚ���"));
		}
		else if(m_gameKind == Ren3Zu3_Fushi)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע����ѡ��������Ӧλ��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,8,8;���������ӦͶע������ѡ��λ�ã�2,8,8,������������һ�Ƚ���"));
		}
 		else if(m_gameKind == Ren3Zu6_Fushi)
 		{
 			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע����ѡ��������Ӧλ��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5,8;���������ӦͶע������ѡ��λ�ã�2,5,8,������������һ�Ƚ���"));
 		}
 		else if(m_gameKind == Ren2Zuxuan_Fushi)
 		{
 			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ�������������һע����ѡ��������Ӧλ��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:2,5;���������ӦͶע������ѡ��λ�ã�2,5,�����ζ���ѡһ�Ƚ���"));
 		}
		else if(m_gameKind == Zuxuan120)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨������ѡ������������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:02568,���������5������ֻҪ����0��2��5��6��8,������������ѡ120һ�Ƚ���"));
		}
		else if(m_gameKind == Zuxuan60)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ӡ����غš���ѡ��һ�����֣��ӡ����š���ѡ�������������һע������ѡ�����뿪������ͬ������˳�򣩣����н���"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:���غ�:8,����:0��2��5,ֻҪ������5�����ְ���0��2��5��8��8,������������ѡ60һ�Ƚ���"));
		}
		else if(m_gameKind == Zuxuan30)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ӡ����غš���ѡ���������֣��ӡ����š���ѡ��һ���������һע������ѡ�����뿪��������ͬ������ѡ���غŶ��������Σ����н���"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:���غ�:2��8,����:0,ֻҪ������5�����ְ���0��2��2��8��8,������������ѡ30һ�Ƚ���"));
		}
		else if(m_gameKind == Zuxuan20)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ӡ����غš���ѡ��һ�����ӡ����š���ѡ���������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:���غ�:8,����:0��2,ֻҪ������5�����ְ���0��2��8��8��8,������������ѡ20һ�Ƚ���"));
		}
		else if(m_gameKind == Zuxuan10)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ӡ����غš���ѡ��һ�����ӡ����غš���ѡ��һ�������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:���غ�:8,����:2,ֻҪ������5�����ְ���2��2��8��8��8,������������ѡ10һ�Ƚ���"));
		}
		else if(m_gameKind == Zuxuan5)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("��Ϸ�淨���ӡ����غš���ѡ��һ�����ӡ����š���ѡ��һ�������һע��"));
			m_ToolTipHelp.AddTool(&m_btWxzxHelp,TEXT("Ͷע����:���غ�:8,����:2,ֻҪ������5�����ְ���2��8��8��8��8,������������ѡ5һ�Ƚ���"));
		}

		m_bGetUserFandian = true;
		//m_zongZhuShu = 0;
		//m_editBeiTou.SetWindowText(L"1");
		//m_beishu=1;

		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=500;
		InvalidateRect(&rcInvali);
		InvalidateRect(&rcZongJinE);
		return;
	}

	if(nSendType == 4)
	{
		m_bZhuihao = true;
		if(m_bZhuihao)
		{
			m_bZhuihao = false;
			m_DlgStatus.ShowStatusWindow(TEXT("�����ύ���ݣ����Ժ�..."));

			//if(m_RenxuanKeyboardInput/*&&m_strHaoma.GetLength() >10240*/)
			{
				m_nTzhSign++;

				if(m_strHaoma.IsEmpty() && vecAllNums.size()>0)
				{
					for(int i = 0;i < vecAllNums.size();i++)
					{
						m_strHaoma += vecAllNums[i].c_str();
						m_strHaoma += L"|";
					}

				}

				int nSel = m_listChqSSC.GetSelectionMark();
				if(nSel<0)
					nSel = 0;
				int nItemCount = m_listChqSSC.GetItemCount();

				CMD_GP_TouZhuCQSSC_Zhuihao TouzhuCQSSC;
				ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
				TouzhuCQSSC.dwUserID = theAccount.user_id;
				TouzhuCQSSC.nGameType = m_TypeID;

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
					if(m_strHaoma[m_strHaoma.GetLength()-1] != '|')
						m_strHaoma+=TEXT("|");

					string	strAllHaoma;
					ChangeStringToA(m_strHaoma, strAllHaoma);

					memset(TouzhuCQSSC.strQishu,0,sizeof(TouzhuCQSSC.strQishu));

					CString strQiHao;
					if(m_TypeID == CZChongQingSSC)
						strQiHao = m_chqRule.GetNextExpect();
					else if(m_TypeID == CZ_TianJinSSC)
						strQiHao = m_jxRule.GetNextExpect();
					else if(m_TypeID == CZXinJiangSSC)
						strQiHao = m_xjRule.GetNextExpect();
					else if(m_TypeID == CZ_FENFEN_CAI)
						strQiHao = m_ffcRule.GetNextExpect();
					else if(m_TypeID == CZ_TXfenfencai)
						strQiHao = m_txffcRule.GetNextExpect();
					else if(m_TypeID == CZ_QQfenfencai)
						strQiHao = m_qqffcRule.GetNextExpect();
					else if(m_TypeID == CZ_ErFenCai)//9.28
						strQiHao = m_efcRule.GetNextExpect();
					else if(m_TypeID == CZ_WUFEN_CAI)
						strQiHao = m_wfcRule.GetNextExpect();
					else if(m_TypeID == CZ_BJ5FC)
						strQiHao = m_bj5fcRule.GetNextExpect();
					else if(m_TypeID == CZ_HGYDWFC)
						strQiHao = m_hgRule.GetNextExpect();
					else if(m_TypeID == CZ_JiaNaDaSSC)
						strQiHao = m_jndRule.GetNextExpect();

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



			SetTimer(TimerStatus,60000,NULL);
			m_strHaoma = _T("");
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
		m_bGetUserFandian = true;
		//m_btnLock.EnableTextButton(true);
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
// 		m_bGetLastYue =true;
// 		if(m_bGetLastYue)
// 		{
// 			m_bGetLastYue = false;
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
		m_bGetMoreRecord =true;
		if(m_bGetMoreRecord)
		{
			m_bGetMoreRecord = false;
			CMD_GP_GetMoreRecord GetMoreRecord;
			ZeroMemory(&GetMoreRecord,sizeof(GetMoreRecord));

			GetMoreRecord.n_t_type = m_TypeID;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			if(pPlatformFrame!=NULL)
				pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_MORE_RECORD,&GetMoreRecord,sizeof(GetMoreRecord));
		}
		return;

	}
	if(nSendType == 9)
	{

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_QIHAO_CHA);

	}
	if(nSendType == 10)
	{
		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		if(pPlatformFrame!=NULL)
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_CANADA_START_QIHAO);
		
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
VOID CChongQingSSC::SendQueryLuckyNumCQSSC()
{
	return;
}


//�������˷���ǰN����������
bool CChongQingSSC::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex,int nTypeID)
{
	_tcscpy_s(m_kjXinxi[nTypeID][nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nTypeID][nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nTypeID][nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);
	
	if(nIndex+1 < 4)
	{
		return true;
	}

	m_kjXinxiCont = 4;
	if(!IsWindowVisible())
		return false;

	if (_tcscmp(m_lastExpect, m_kjXinxi[nTypeID][0].qihao) != 0) 
	{
		//��һ�ν���
	//	if(m_lastExpect[0] == 0)
		if(m_TypeID == CZ_JiaNaDaSSC)
		{
			_tcscpy_s(CCanadaRule::m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[nTypeID][0].qihao);
			_tcscpy_s(CCanadaRule::m_lastKjShj, KJ_SHIJIAN_LENGTH, m_kjXinxi[nTypeID][0].shijian);
		}
		if(nTypeID == m_TypeID)
		{
			_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[nTypeID][0].qihao);
			m_bKaiJiangzhong=false;											//��������
			OutputDebugString(L"CHQSSCKAIJ 1");

			KillTimer(timer_id_kaijiangzhong);
			RedrawWindow(&rcRedraw,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
	}

	return true;
}




long CChongQingSSC::GetKjShjDiff()
{
	if(m_TypeID == CZ_FENFEN_CAI)
	{
		long secDiff = m_ffcRule.GetKjShjDiff();

		if (secDiff <= 5 )
		{
			m_bKaiJiangzhong=true;

			return 0;
		}
		else if(secDiff > (m_ffcRule.GetQiSpan() ))
		{
			m_bKaiJiangzhong=true;
			return 0;
		}
		return secDiff;

	}
	else if(m_TypeID == CZ_TXfenfencai)
	{
		long secDiff = m_txffcRule.GetKjShjDiff();

		CString strLog;
		strLog.Format(L"TXDONGHUA secDiff:%d",secDiff);
		OutputDebugString(strLog);
		if (secDiff <= 1 )
		{
			strLog.Format(L"TXDONGHUA secDiff:%d m_bKaiJiangzhong true",secDiff);
			OutputDebugString(strLog);
			m_bKaiJiangzhong=true;

			return 0;
		}

		return secDiff;

	}
	else if(m_TypeID == CZ_QQfenfencai)
	{
		long secDiff = m_qqffcRule.GetKjShjDiff();

		if (secDiff <= 1 )
		{
			m_bKaiJiangzhong=true;

			return 0;
		}

		return secDiff;

	}
	else if(m_TypeID == CZ_ErFenCai)//9.28
	{
		long secDiff = m_efcRule.GetKjShjDiff();

		if (secDiff <= 5 )
		{
			m_bKaiJiangzhong=true;

			return 0;
		}
		else if(secDiff > (m_efcRule.GetQiSpan() ))
		{
			m_bKaiJiangzhong=true;
			return 0;
		}
		return secDiff;

	}
	else if(m_TypeID == CZ_HGYDWFC)
	{
		long secDiff = m_hgRule.GetKjShjDiff();

		if (secDiff <= 0 )
		{
			m_bKaiJiangzhong=true;

			return 0;
		}
		else if(secDiff > (m_hgRule.GetQiSpan() ))
		{
			OutputDebugString(L"KAIJIANG true 3");
			m_bKaiJiangzhong=true;
			return 0;
		}
		return secDiff;

	}
	else if(m_TypeID == CZ_JiaNaDaSSC)
	{
		long secDiff = m_jndRule.GetKjShjDiff();

		if (secDiff <= 45 )
		{
			m_bKaiJiangzhong=true;

			return 0;
		}
		else if(secDiff > (m_jndRule.GetQiSpan() ))
		{
			OutputDebugString(L"KAIJIANG true 3");
			m_bKaiJiangzhong=true;
			return 0;
		}
		return secDiff;

	}
	else if(m_TypeID == CZ_WUFEN_CAI)
	{
		long secDiff = m_wfcRule.GetKjShjDiff();

		if (secDiff <= 45 )
		{
			
			return 0;
		}
		else if(secDiff == (m_wfcRule.GetQiSpan() ))
		{
			m_bKaiJiangzhong=true;
			return 0;
		}
		return secDiff;
	}
	else if(m_TypeID == CZ_BJ5FC)
	{
		long secDiff = m_bj5fcRule.GetKjShjDiff();

		if (secDiff <= 60 )
		{

			return 0;
		}
		else if(secDiff == (m_bj5fcRule.GetQiSpan() ))
		{
			m_bKaiJiangzhong=true;
			return 0;
		}
		return secDiff;
	}
	else if(m_TypeID == CZ_TianJinSSC)
	{
		long secDiff = m_jxRule.GetKjShjDiff();
		if (secDiff <= 140 )
		{
			return 0;
		}
		else if(secDiff == 0)//m_jxRule.GetQiSpan())
		{

			m_bKaiJiangzhong=true;
			return 0;
		}
// 		else if(secDiff-20 > m_jxRule.GetQiSpan())
// 			return -1;
		return secDiff;
	}
	else if(m_TypeID == CZXinJiangSSC)
	{
		long secDiff = m_xjRule.GetKjShjDiff();
		if (secDiff <= 60 )
		{
			return 0;
		}
		else if(secDiff ==m_xjRule.GetQiSpan())
		{
			m_bKaiJiangzhong=true;
			return 0;
		}
		return secDiff;
	}
	else
	{
		long secDiff = m_chqRule.GetKjShjDiff(); 

		if (secDiff <= 35 )
		{
			return 0;
		}
		else if(secDiff ==m_chqRule.GetQiSpan())
		{
			OutputDebugString(L"KAIJIANG true 1");

			m_bKaiJiangzhong=true;
			return 0;
		}
		return secDiff;

	}
}


//���ڿ�������
void CChongQingSSC::DrawLastKjHaoma(CDC* pDC, Graphics& graphics)
{
	long secDiff = m_chqRule.GetKjShjDiff();
	int nStartD = 0;
	int nEndD = 0;
	if(m_TypeID == CZChongQingSSC)
	{
		secDiff = m_chqRule.GetKjShjDiff();
		nStartD = 35;
		nEndD = 40;
	}
	else if(m_TypeID == CZ_TianJinSSC)
	{
		secDiff = m_jxRule.GetKjShjDiff();
		nStartD = 140;
		nEndD = 145;

	}
	else if(m_TypeID == CZXinJiangSSC)
	{
		secDiff = m_xjRule.GetKjShjDiff();
		nStartD = 60;
		nEndD = 65;

	}
	else if(m_TypeID == CZ_FENFEN_CAI)
	{
		secDiff = m_ffcRule.GetKjShjDiff();
		nStartD = 5;
		nEndD = 10;

	}
	else if(m_TypeID == CZ_TXfenfencai )
	{
		secDiff = m_txffcRule.GetKjShjDiff();
		nStartD = 0;
		nEndD = 5;

	}
	else if(m_TypeID == CZ_QQfenfencai )
	{
		secDiff = m_qqffcRule.GetKjShjDiff();
		nStartD = 0;
		nEndD = 5;

	}
	else if(m_TypeID == CZ_ErFenCai)
	{
		secDiff = m_efcRule.GetKjShjDiff();
		nStartD = 15;
		nEndD = 20;

	}
	else if(m_TypeID == CZ_WUFEN_CAI)
	{
		secDiff = m_wfcRule.GetKjShjDiff();
		nStartD = 45;
		nEndD = 50;

	}
	else if(m_TypeID == CZ_BJ5FC)
	{
		secDiff = m_bj5fcRule.GetKjShjDiff();
		nStartD = 60;
		nEndD = 65;

	}
	else if(m_TypeID == CZ_HGYDWFC)
	{
		secDiff = m_hgRule.GetKjShjDiff();
		nStartD = 1;
		nEndD = 5;

	}
	else if(m_TypeID == CZ_JiaNaDaSSC)
	{
		secDiff = m_jndRule.GetKjShjDiff();
		nStartD = 45;
		nEndD = 50;

	}

	CString strLog;
	strLog.Format(L"HGYDWFC secDiff:%d",secDiff);
	OutputDebugString(strLog);
	if(IsWindowVisible() && secDiff > nStartD && secDiff <=nEndD)
	{
		CPlatformFrame* pPlatforFrame = CPlatformFrame::GetInstance();
		if(pPlatforFrame->m_bSound)
			sndPlaySound(MAKEINTRESOURCE(IDR_CLOCK),  SND_RESOURCE |SND_ASYNC);
	}

	if(IsWindowVisible() )
	{
		if((m_TypeID == CZChongQingSSC&& secDiff == nStartD)||(m_TypeID == CZ_FENFEN_CAI&& secDiff == nStartD)||(m_TypeID == CZ_TXfenfencai&& secDiff == nStartD)||(m_TypeID == CZ_QQfenfencai&& secDiff == nStartD)||(m_TypeID == CZ_ErFenCai&& secDiff == nStartD)||(m_TypeID == CZ_WUFEN_CAI&& secDiff == nStartD)||(m_TypeID == CZ_TianJinSSC&& secDiff == nStartD)||(m_TypeID == CZXinJiangSSC&& secDiff == nStartD)||(m_TypeID == CZ_HGYDWFC&& secDiff == nStartD)||(m_TypeID == CZ_JiaNaDaSSC&& secDiff == nStartD)||(m_TypeID == CZ_BJ5FC&& secDiff == nStartD))
		{
			CWnd *pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_TANCHUANG,0,0);
			}

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
		OutputDebugString(L"TXDONGHUA false 4");

	}
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&m_KjInfoFont1);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(255, 255, 255));
	if(m_TypeID == CZChongQingSSC)
		pDC->DrawText(_T("�����"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	else if(m_TypeID == CZ_TianJinSSC)
		pDC->DrawText(_T("����"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	else if(m_TypeID == CZXinJiangSSC)
		pDC->DrawText(_T("�½���"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	else if(m_TypeID == CZ_TXfenfencai)
	{
		CRect rcFFC;
		rcFFC.CopyRect(rcCzh);
		rcFFC.left -= 20;
		rcFFC.right += 20;
		pDC->DrawText(_T("��Ѷ�ֲַ�"), &rcFFC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_QQfenfencai)
	{
		CRect rcFFC;
		rcFFC.CopyRect(rcCzh);
		rcFFC.left -= 20;
		rcFFC.right += 20;
		pDC->DrawText(_T("QQ�ֲַ�"), &rcFFC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_FENFEN_CAI)
	{
		CRect rcFFC;
		rcFFC.CopyRect(rcCzh);
		rcFFC.left -= 20;
		rcFFC.right += 20;
		pDC->DrawText(_T("̨��ֲַ�"), &rcFFC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_ErFenCai)
	{
		CRect rcFFC;
		rcFFC.CopyRect(rcCzh);
		rcFFC.left -= 20;
		rcFFC.right += 20;
		pDC->DrawText(_T("ӡ����ֲ�"), &rcFFC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_WUFEN_CAI)
	{
		CRect rcFFC;
		rcFFC.CopyRect(rcCzh);
		rcFFC.left -= 20;
		rcFFC.right += 20;
		pDC->DrawText(_T("������ֲ�"), &rcFFC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_BJ5FC)
	{
		CRect rcFFC;
		rcFFC.CopyRect(rcCzh);
		rcFFC.left -= 20;
		rcFFC.right += 20;
		pDC->DrawText(_T("����5�ֲ�"), &rcFFC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_HGYDWFC)
	{
		CRect rcFFC;
		rcFFC.CopyRect(rcCzh);
		rcFFC.left -= 20;
		rcFFC.right += 20;
		pDC->DrawText(_T("����1.5�ֲ�"), &rcFFC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_JiaNaDaSSC)
	{
		CRect rcFFC;
		rcFFC.CopyRect(rcCzh);
		rcFFC.left -= 30;
		rcFFC.right += 20;
		pDC->DrawText(_T("���ô�3.5�ֲ�"), &rcFFC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	CRect rcDi;
	rcDi.left = rcCzh.right+10;
	rcDi.right = rcDi.left+20;
	rcDi.top = rcCzh.top;
	rcDi.bottom = rcCzh.bottom;
	pDC->DrawText(_T("��"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(253, 191, 6));

	if(m_TypeID == CZChongQingSSC)
	{
		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_chqRule.GetNextExpect(-1), &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
			pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	}
	else if(m_TypeID == CZ_TianJinSSC)
	{
		pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZXinJiangSSC)
	{
		pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_FENFEN_CAI)
	{
		CRect rcFFcQi;
		rcFFcQi.CopyRect(rcQiHao);

		rcFFcQi.left+=10;
		rcFFcQi.right += 15;
		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_ffcRule.GetNextExpect(-1), &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
			pDC->DrawText(m_lastExpect, &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_TXfenfencai)
	{
		CRect rcFFcQi;
		rcFFcQi.CopyRect(rcQiHao);

		rcFFcQi.left+=10;
		rcFFcQi.right += 15;
		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_txffcRule.GetNextExpect(-1), &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
			pDC->DrawText(m_lastExpect, &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_QQfenfencai)
	{
		CRect rcFFcQi;
		rcFFcQi.CopyRect(rcQiHao);

		rcFFcQi.left+=10;
		rcFFcQi.right += 15;
		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_qqffcRule.GetNextExpect(-1), &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
			pDC->DrawText(m_lastExpect, &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_ErFenCai)//9.28
	{
		CRect rcFFcQi;
		rcFFcQi.CopyRect(rcQiHao);

		rcFFcQi.left+=10;
		rcFFcQi.right += 15;
		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_efcRule.GetNextExpect(-1), &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
			pDC->DrawText(m_lastExpect, &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_WUFEN_CAI)
	{
		CRect rcFFcQi;
		rcFFcQi.CopyRect(rcQiHao);

		rcFFcQi.left+=10;
		rcFFcQi.right += 15;
		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_wfcRule.GetNextExpect(-1), &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
			pDC->DrawText(m_lastExpect, &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_BJ5FC)
	{
		CRect rcFFcQi;
		rcFFcQi.CopyRect(rcQiHao);

		rcFFcQi.left+=10;
		rcFFcQi.right += 15;
		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_bj5fcRule.GetNextExpect(-1), &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
			pDC->DrawText(m_lastExpect, &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_HGYDWFC)
	{
		CRect rcFFcQi;
		rcFFcQi.CopyRect(rcQiHao);

		rcFFcQi.left+=10;
		rcFFcQi.right += 15;
		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_hgRule.GetNextExpect(-1), &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
			pDC->DrawText(m_lastExpect, &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_TypeID == CZ_JiaNaDaSSC)
	{
		CRect rcFFcQi;
		rcFFcQi.CopyRect(rcQiHao);

		rcFFcQi.left+=10;
		rcFFcQi.right += 15;
		if(m_bKaiJiangzhong)
		{
			pDC->DrawText(m_jndRule.GetNextExpect(-1), &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
			pDC->DrawText(m_lastExpect, &rcFFcQi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
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

	if(m_cbIfTs == 1)
	{
		strYg.Format(_T("��ͣ����"));
	}
	else
	{
		if(m_TypeID == CZChongQingSSC)
		{
			//long kjing = m_ffcRule.GetKjShjDiff(); 
			if(secDiff<nStartD)
				strYg.Format(_T("��%s	�ⵥʱ��"), m_chqRule.GetNextExpect());
			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_chqRule.GetNextExpect());

		}
		else if(m_TypeID == CZ_TianJinSSC)
		{
			long kjing = m_jxRule.GetKjShjDiff(); 
			if(kjing>nStartD)
				strYg.Format(_T("��%s	�µ�ʱ��"), m_jxRule.GetNextExpect());
			else
				strYg.Format(_T("��%s	�ⵥʱ��"), m_jxRule.GetNextExpect());
				
		}
		else if(m_TypeID == CZXinJiangSSC)
		{
			if(secDiff<nStartD)
				strYg.Format(_T("��%s	�ⵥʱ��"), m_xjRule.GetNextExpect());
			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_xjRule.GetNextExpect());
		}
		else if(m_TypeID == CZ_FENFEN_CAI)
		{
			long kjing = m_ffcRule.GetKjShjDiff(); 
			if (kjing < nStartD)								// || kjing > 540
				strYg.Format(_T("��%s	�ⵥʱ��"), m_ffcRule.GetNextExpect());
			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_ffcRule.GetNextExpect());

		}
		else if(m_TypeID == CZ_TXfenfencai)
		{
			int nCha = GetTXQihaoCha();
// 			if(nCha==2)
// 				strYg.Format(_T("ά��ʱ��"));
// 			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_txffcRule.GetNextExpect());

		}
		else if(m_TypeID == CZ_QQfenfencai)
		{
			int nCha = GetTXQihaoCha();
			if(nCha==2)
				strYg.Format(_T("ά��ʱ��"));
			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_qqffcRule.GetNextExpect());
		}
		else if(m_TypeID == CZ_ErFenCai)//9.28
		{
			long kjing = m_efcRule.GetKjShjDiff(); 
			if (kjing < nStartD)								// || kjing > 540
				strYg.Format(_T("��%s	�ⵥʱ��"), m_efcRule.GetNextExpect());
			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_efcRule.GetNextExpect());

		}
		else if(m_TypeID == CZ_WUFEN_CAI)
		{
			if(secDiff<nStartD)
				strYg.Format(_T("��%s	�ⵥʱ��"), m_wfcRule.GetNextExpect());
			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_wfcRule.GetNextExpect());
		}
		else if(m_TypeID == CZ_BJ5FC)
		{
			if(secDiff<nStartD)
				strYg.Format(_T("��%s	�ⵥʱ��"), m_bj5fcRule.GetNextExpect());
			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_bj5fcRule.GetNextExpect());
		}
		else if(m_TypeID == CZ_HGYDWFC)
		{
			if(secDiff<nStartD)
				strYg.Format(_T("��%s	�ⵥʱ��"), m_hgRule.GetNextExpect());
			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_hgRule.GetNextExpect());
		}
		else if(m_TypeID == CZ_JiaNaDaSSC)
		{
			if(secDiff<nStartD)
				strYg.Format(_T("��%s	�ⵥʱ��"), m_jndRule.GetNextExpect());
			else
				strYg.Format(_T("��%s	�µ�ʱ��"), m_jndRule.GetNextExpect());

			//int nCount = m_jndRule.GetZhuihaoQishu();

			if(m_nCanadaStartQihao == 0)
				strYg.Format(_T("��%06d ��ͣ����"), 0);
		}
	}

	CString strTrm;
	strTrm = strYg;
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
	if(m_TypeID == CZChongQingSSC)
	{
		strYg.Format(_T("%s"), m_chqRule.GetKjShjDiffDesc(35));
	}
	else if(m_TypeID == CZ_TianJinSSC)
		strYg.Format(_T("%s"), m_jxRule.GetKjShjDiffDesc(20));
	else if(m_TypeID == CZXinJiangSSC)
		strYg.Format(_T("%s"), m_xjRule.GetKjShjDiffDesc(60));
	else if(m_TypeID == CZ_FENFEN_CAI)
		strYg.Format(_T("%s"), m_ffcRule.GetKjShjDiffDesc(5));
	else if(m_TypeID == CZ_TXfenfencai)
		strYg.Format(_T("%s"), m_txffcRule.GetKjShjDiffDesc(0));
	else if(m_TypeID == CZ_QQfenfencai)
		strYg.Format(_T("%s"), m_qqffcRule.GetKjShjDiffDesc(0));
	else if(m_TypeID == CZ_ErFenCai)	//9.28
		strYg.Format(_T("%s"), m_efcRule.GetKjShjDiffDesc(15));
	else if(m_TypeID == CZ_WUFEN_CAI)
		strYg.Format(_T("%s"), m_wfcRule.GetKjShjDiffDesc(45));
	else if(m_TypeID == CZ_BJ5FC)
		strYg.Format(_T("%s"), m_bj5fcRule.GetKjShjDiffDesc(60));
	else if(m_TypeID == CZ_HGYDWFC)
		strYg.Format(_T("%s"), m_hgRule.GetKjShjDiffDesc(0));
	else if(m_TypeID == CZ_JiaNaDaSSC)
		strYg.Format(_T("%s"), m_jndRule.GetKjShjDiffDesc(45));

	pDC->SetTextColor(RGB(219, 219, 219));
	rcYuGao1.left = rcYuGao1.left-50-2-15;
	rcYuGao1.top = rcYuGao1.top+12;
	rcYuGao1.bottom = rcYuGao1.bottom+20;
	rcYuGao1.right = rcYuGao1.right-20;
	pDC->DrawText(strYg, &rcYuGao1,DT_CENTER|DT_SINGLELINE);

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(&pOldFont);
	pDC->SetBkMode(oldBkMode);

	CString str = m_kjXinxi[m_TypeID][0].haoma;

	CString temp;
	temp.Format(L"%c",m_kjXinxi[m_TypeID][0].haoma[0]);

	if(_ttoi(temp) > 9)
	{
		OutputDebugString(L"KAIJIANG true 2");
		m_bKaiJiangzhong=true;
		KillTimer(timer_id_kaijiangzhong);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);

		return;
	}
	int index = 0;
	for (int i=0; i<str.GetLength(); i++) 
	{
		if (iswdigit(str.GetAt(i))) 
		{
			int num = _ttoi(CString(str.GetAt(i)));
			if (num >=0 && num < 10) 
			{
				if(m_bKaiJiangzhong )
				{
					KillTimer(timer_id_kaijiangzhong);
					SetTimer(timer_id_kaijiangzhong, 100, NULL);
				}
				else
				{
					DrawBigNum(graphics, m_kjNumBig, Rect(kj_big_haoma_x + index * (m_bigNumWidth)-20, kj_big_haoma_y, m_bigNumWidth, m_bigNumHeight), num);
				}
			}

			index ++;
		}
	}
}
int CChongQingSSC::GetTXQihaoCha()
{
	if(m_TypeID != CZ_TXfenfencai&&m_TypeID != CZ_QQfenfencai)
		return 0;

	LONGLONG nNextQihao = 0;
	if(m_TypeID == CZ_TXfenfencai)
	{
		CTXFenFenCaiRule rule;
		CString strNextQihao = rule.GetNextExpect();
		 nNextQihao = _wtoi64(LPCTSTR(strNextQihao));
	}
	else if (m_TypeID == CZ_QQfenfencai)
	{
		CQQFenFenCaiRule rule;
		CString strNextQihao = rule.GetNextExpect();
		nNextQihao = _wtoi64(LPCTSTR(strNextQihao));
	}
	LONGLONG nLastQihao = _wtoi64(m_lastExpect);

	CString strLog;
	strLog.Format(L"\nQIHAO next:%I64d,last:%I64d",nNextQihao,nLastQihao);
	OutputDebugString(strLog);
	int nQhao = nNextQihao-nLastQihao;
	if(nQhao>8563)
		return 2;
	else if(nQhao<8561&&nQhao>2)
	{
		return 2;
	}
	return 1;
}
//��������
void CChongQingSSC::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
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
		CRect rcText(kj_small_haoma_x - 80-20, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 80, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[m_TypeID][cnt].haoma;
		CString temp;
		temp = str.GetAt(cnt);

		if(_ttoi(temp) > 9)
		{
		//	index ++;
			continue;
		}

		int index = 0;
		for (int i=0; i<str.GetLength(); i++) 
		{
			if (iswdigit(str.GetAt(i))) 
			{
				int num = _ttoi(CString(str.GetAt(i)));
				if (num >=0 && num < 10) 
				{
					Rect rc(kj_small_haoma_x+10 + index * (m_smallNumWidth + kj_small_haoma_col_span), 
						kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
						m_smallNumWidth, m_smallNumHeight);
					DrawSmallNum(graphics, m_kjNumSmall, rc, num);
				}

				index ++;
			}
		}

		CString strNiu;
		strNiu = str.Right(2);

		int n1 = _ttoi(strNiu.Left(1));
		int n2 = _ttoi(strNiu.Right(1));
		int n = (n1+n2)%10;

		CRect rcNiuText(kj_small_haoma_x + 90, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 210, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(GetNiuString(n), &rcNiuText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	}

	pDC->DrawText(strText, &rcZongJinE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}

void CChongQingSSC::DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_bigNumWidth, 0, m_bigNumWidth, m_bigNumHeight, UnitPixel);
}

void CChongQingSSC::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);
}
CString CChongQingSSC::GetDxdshStringDesc(int nNum)
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

void CChongQingSSC::DrawZongZhushuJinE(CDC* pDC)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_zongFont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(139, 137, 136));
	strText.Format(_T("��ѡ����"));

	CRect rcZhu;
	rcZhu.CopyRect(rcZongZhuShu);
	rcZhu.left += 188;
	rcZhu.right += 433;
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

void CChongQingSSC::ResetAllNums()
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

	UnCheckNumBtn(m_btnNiu_0);
	UnCheckNumBtn(m_btnNiu_1);
	UnCheckNumBtn(m_btnNiu_2);
	UnCheckNumBtn(m_btnNiu_3);
	UnCheckNumBtn(m_btnNiu_4);
	UnCheckNumBtn(m_btnNiu_5);
	UnCheckNumBtn(m_btnNiu_6);
	UnCheckNumBtn(m_btnNiu_7);
	UnCheckNumBtn(m_btnNiu_8);
	UnCheckNumBtn(m_btnNiu_9);
	UnCheckNumBtn(m_btnNiu_10);

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

	UnCheckNumBtn(m_btnLong);
	UnCheckNumBtn(m_btnHu);
	UnCheckNumBtn(m_btnHe);

	UnCheckNumBtn(m_btnDaWan);
	UnCheckNumBtn(m_btnXiaoWan);
	UnCheckNumBtn(m_btnDanWan);
	UnCheckNumBtn(m_btnShuangWan);
	UnCheckNumBtn(m_btnDaQian);
	UnCheckNumBtn(m_btnXiaoQian);
	UnCheckNumBtn(m_btnDanQian);
	UnCheckNumBtn(m_btnShuangQian);
	UnCheckNumBtn(m_btnDaBai);
	UnCheckNumBtn(m_btnXiaoBai);
	UnCheckNumBtn(m_btnDanBai);
	UnCheckNumBtn(m_btnShuangBai);
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

void CChongQingSSC::SetLockText(float fBonus, float fBonusPercent)
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
bool CChongQingSSC::GetZuSanHaoma(CStringArray& strArr)
{
	CString line;
	//m_editDanShiHaoMa.GetWindowText(line);
	m_richDanshiHaoma.GetWindowText(line);
	if(line.GetLength() > 2999)
	{
//		MyMessageBox(_T("������ĺ���̫����"));
		MyMessageBox(_T("������ĺ���̫����"));
		return false;
	}
	if (!GetHaomaStrs(strArr, line, 3)) 
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

bool CChongQingSSC::GetZuSanHaoma(CString& haoma, int& zhushu)
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
		CString strMyHao ;
		//haoma += strArr.GetAt(i);
		strMyHao = strArr.GetAt(i);

		for(int j = 0; j<strMyHao.GetLength();j++)
		{
			haoma += strMyHao.GetAt(j);
			if (j!=strMyHao.GetLength()-1)
			{
				haoma += _T(",");
			}

			if(j == strMyHao.GetLength()-1)
			{
				haoma +=_T("|");
			}

		}
// 
// 		if (i < count - 1)
// 		{
// 			haoma += _T(",");
// 		}
	}
	zhushu = count;

	return true;
}

bool CChongQingSSC::IsZuSanHaoma(const CString& str)
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

bool CChongQingSSC::GetDanShiHaoma(CStringArray& strArr, int size)
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
//		MyMessageBox(_T("������ĺ���̫����"));	
		MyMessageBox(_T("������ĺ���̫����"));	
		return false;
	}
	if (!GetHaomaStrs(strArr, line, size)) 
	{
		return false;
	}

	return true;
}

bool CChongQingSSC::GetDanShiHaoma(CString& haoma, int size, int& zhushu, int nMax)
{
	CStringArray strArr;
	CString line;
	if(nMax == 0)
		nMax = 512000;
	m_richDanshiHaoma.GetWindowText(line);
	if(line.IsEmpty())
	{
		MyMessageBox(_T("��������룡"));	

		return false;
	}
	if(line.GetLength() > nMax)
	{
		MyMessageBox(_T("������ĺ���̫����"));	
		return false;
	}
	if (!GetHaomaStrs(strArr, line, size)) 
	{
		return false;
	}


	int count = strArr.GetCount();
	for(int i=0; i<count; i++)
	{
		if(m_gameKind == WuXingQianEr_DanShi || m_gameKind == WuXingHouEr_DanShi|| m_gameKind == QianErZuXuan_DanShi|| m_gameKind == HouErZuXuan_DanShi)
		{
			CString strTempHaoma;
			strTempHaoma += strArr.GetAt(i);

			int nLength = strTempHaoma.GetLength();
			for (int j = 0;j < nLength;j++)
			{
				haoma += strTempHaoma.GetAt(j);
				if(j!=nLength-1)
				{
					haoma+=_T(",");
				}
			}

		}
		else
		{
			haoma += strArr.GetAt(i);
		}
	//	if (i < count - 1)
		{
			haoma += _T("|");
		}
	}
	zhushu = count;


	return true;
}

bool CChongQingSSC::GetHaomaStrs(CStringArray& strArr, CString line, int size)
{

	int len = line.GetLength();
	CString subStr,str;
	for (int i=0; i<len; i++) 
	{
		str = line.GetAt(i);
		if (iswdigit(line.GetAt(i))&&!str.IsEmpty()) 
		{
			subStr += line.GetAt(i);

			if(m_gameKind == QianErZuXuan_DanShi || m_gameKind == HouErZuXuan_DanShi ||m_gameKind == Ren2Zuxuan_Fushi)
			{
				if (subStr.GetLength() == size)
				{
					if(subStr[0] == subStr[1])
					{
						subStr = _T("");
						continue;
// 						MyMessageBox(L"��ѡ������ѡ�ظ����֣�");
// 						return false;
					}
				}
			}
			else if(m_gameKind == QianSanHunhe || m_gameKind == ZhongSanHunhe|| m_gameKind == HouSanHunhe|| m_gameKind == Ren3Hunhe_Zuxuan)
			{
				if (subStr.GetLength() == size)
				{
					if(subStr[0] == subStr[1]&&subStr[2] == subStr[1])
					{
						subStr = _T("");
						continue;

// 						MyMessageBox(L"��ϲ���ѡ�������֣�");
// 
// 						return false;
					}
				}
			}
			if (subStr.GetLength() > size) 
			{
				subStr = _T("");
				continue;
			}
		}
		else
		{

			if (subStr.GetLength() != size && subStr.GetLength() != 0) 
			{
				subStr = _T("");
				continue;
			}

			if (subStr.GetLength() == size) 
			{
				strArr.Add(subStr);
				subStr = _T("");
			}
		}
	}
	if(( subStr.GetLength() == 0)&&(strArr.GetCount()==0))
	{
		MyMessageBox(_T("��������룡"));	

		return false;

	}

	if (subStr.GetLength() != size && subStr.GetLength() != 0) 
	{
		subStr = _T("");
		return true;
	}

	if (subStr.GetLength() == size) 
	{
		strArr.Add(subStr);
		subStr = _T("");
	}

	return true;
}

//��ʼ���淨��Ϣ
void CChongQingSSC::InitWanFaMoShi()
{
	theGameKind[(CZ_CaiZhangdie << 16) | (unsigned short)CaiZhangDie] = _T("���ǵ�");		
	CaiZhong nCaiZhong[11] = {CZChongQingSSC,CZ_TianJinSSC,CZXinJiangSSC,CZ_FENFEN_CAI,CZ_WUFEN_CAI,CZ_HGYDWFC,CZ_JiaNaDaSSC,CZ_ErFenCai,CZ_BJ5FC,CZ_TXfenfencai,CZ_QQfenfencai};

	for(int i = 0;i < CountArray(nCaiZhong);i++)
	{
		CaiZhong nCai = nCaiZhong[i];
		theGameKind[(nCai << 16) | (unsigned short)HouSanZhiXuan_FuShi] = _T("����ֱѡ");		
		theGameKind[(nCai << 16) | (unsigned short)HouSanZhiXuan_DanShi] = _T("����ֱѡ��ʽ");

		//	theGameKind[(nCai << 16) | (unsigned short)QianSanZhiXuan_FuShi] = _T("ǰ��ֱѡ");
		theGameKind[(nCai << 16) | (unsigned short)QianSanZhiXuan_FuShi] = _T("ǰ��ֱѡ");
		theGameKind[(nCai << 16) | (unsigned short)QianSanZhiXuan_DanShi] = _T("ǰ��ֱѡ��ʽ");

		theGameKind[(nCai << 16) | (unsigned short)QianSanZhiXuanHeZhi] = _T("ǰ����ֵ");
		theGameKind[(nCai << 16) | (unsigned short)ZhongSanZhiXuanHeZhi] = _T("������ֵ");
		theGameKind[(nCai << 16) | (unsigned short)HouSanZhiXuanHeZhi] = _T("������ֵ");
		theGameKind[(nCai << 16) | (unsigned short)QianErZhixuan_Hezhi] = _T("ǰ����ֵ");
		theGameKind[(nCai << 16) | (unsigned short)HouErZhixuan_Hezhi] = _T("�����ֵ");
		theGameKind[(nCai << 16) | (unsigned short)NiuNiu_Num] = _T("΢��ţţ");
		theGameKind[(nCai << 16) | (unsigned short)NiuNiu_None] = _T("��ţ");
		theGameKind[(nCai << 16) | (unsigned short)SSC_LongHu] = _T("������");
		theGameKind[(nCai << 16) | (unsigned short)Zhongsanzusandan] = _T("����������ʽ");

		theGameKind[(nCai << 16) | (unsigned short)QW_YiFanFengShun] = _T("һ����˳");		
		theGameKind[(nCai << 16) | (unsigned short)QW_HaoShiChengShuang] = _T("���³�˫");
		theGameKind[(nCai << 16) | (unsigned short)QW_SanXingBaoXi] = _T("���Ǳ�ϲ");
		theGameKind[(nCai << 16) | (unsigned short)QW_SiJiFaCai] = _T("�ļ�����");

		theGameKind[(nCai << 16) | (unsigned short)WuxingZhixuan] = _T("����ֱѡ");		
		theGameKind[(nCai << 16) | (unsigned short)WuxingQianSi] = _T("����ǰ��");
		theGameKind[(nCai << 16) | (unsigned short)Ren3Zu3_Fushi] = _T("��������");
		theGameKind[(nCai << 16) | (unsigned short)Ren3Zu6_Fushi] = _T("��������");
		theGameKind[(nCai << 16) | (unsigned short)Ren3Hunhe_Zuxuan] = _T("�������");
		theGameKind[(nCai << 16) | (unsigned short)Ren2Zuxuan_Fushi] = _T("�ζ���ѡ");

		theGameKind[(nCai << 16) | (unsigned short)WuxingHousi] = _T("���Ǻ���");
		theGameKind[(nCai << 16) | (unsigned short)WuxingZhongsan] = _T("����ֱѡ");

		theGameKind[(nCai << 16) | (unsigned short)HouSanZuXuan_ZuSanFuShi] =  _T("��������");
		theGameKind[(nCai << 16) | (unsigned short)HouSanZuXuan_ZuSanDanShi] = _T("����������ʽ");
		theGameKind[(nCai << 16) | (unsigned short)HouSanZuXuan_ZuLiuFuShi] = _T("��������");
		theGameKind[(nCai << 16) | (unsigned short)HouSanZuXuan_ZuLiuDanShi] = _T("����������ʽ");

		theGameKind[(nCai << 16) | (unsigned short)ZhongsanZusan] = _T("��������");
		theGameKind[(nCai << 16) | (unsigned short)ZhongsanZuliu] = _T("��������");

		theGameKind[(nCai << 16) | (unsigned short)QianSanZuXuan_ZuSanFuShi] = _T("ǰ������");
		theGameKind[(nCai << 16) | (unsigned short)QianSanZuXuan_ZuSanDanShi] = _T("ǰ��������ʽ");
		theGameKind[(nCai << 16) | (unsigned short)QianSanZuXuan_ZuLiuFuShi] = _T("ǰ������");
		theGameKind[(nCai << 16) | (unsigned short)QianSanZuXuan_ZuLiuDanShi] = _T("ǰ��������ʽ");

		theGameKind[(nCai << 16) | (unsigned short)Zuxuan120] = _T("��ѡ120");
		theGameKind[(nCai << 16) | (unsigned short)Zuxuan60] = _T("��ѡ60");
		theGameKind[(nCai << 16) | (unsigned short)Zuxuan30] = _T("��ѡ30");
		theGameKind[(nCai << 16) | (unsigned short)Zuxuan20] = _T("��ѡ20");
		theGameKind[(nCai << 16) | (unsigned short)Zuxuan10] = _T("��ѡ10");
		theGameKind[(nCai << 16) | (unsigned short)Zuxuan5] = _T("��ѡ5");

		theGameKind[(nCai << 16) | (unsigned short)QSZuXuan24] = _T("ǰ����ѡ24");
		theGameKind[(nCai << 16) | (unsigned short)QSZuXuan12] = _T("ǰ����ѡ12");
		theGameKind[(nCai << 16) | (unsigned short)QSZuXuan6] = _T("ǰ����ѡ6");
		theGameKind[(nCai << 16) | (unsigned short)QSZuXuan4] = _T("ǰ����ѡ4");
		theGameKind[(nCai << 16) | (unsigned short)HSZuXuan24] = _T("������ѡ24");
		theGameKind[(nCai << 16) | (unsigned short)HSZuXuan12] = _T("������ѡ12");
		theGameKind[(nCai << 16) | (unsigned short)HSZuXuan6] = _T("������ѡ6");
		theGameKind[(nCai << 16) | (unsigned short)HSZuXuan4] = _T("������ѡ4");

		theGameKind[(nCai << 16) | (unsigned short)WuXingHouEr_FuShi] = _T("���ֱѡ");
		theGameKind[(nCai << 16) | (unsigned short)WuXingHouEr_DanShi] = _T("���ֱѡ��ʽ");

		theGameKind[(nCai << 16) | (unsigned short)WuXingQianEr_FuShi] = _T("ǰ��ֱѡ");
		theGameKind[(nCai << 16) | (unsigned short)WuXingQianEr_DanShi] = _T("ǰ��ֱѡ��ʽ");

		theGameKind[(nCai << 16) | (unsigned short)HouErZuXuan_FuShi] = _T("�����ѡ");
		theGameKind[(nCai << 16) | (unsigned short)HouErZuXuan_DanShi] = _T("�����ѡ��ʽ");

		theGameKind[(nCai << 16) | (unsigned short)QianErZuXuan_FuShi] = _T("ǰ����ѡ");
		theGameKind[(nCai << 16) | (unsigned short)QianErZuXuan_DanShi] = _T("ǰ����ѡ��ʽ");

		theGameKind[(nCai << 16) | (unsigned short)DaXiaoDanShuang] = _T("�����С��˫");
		theGameKind[(nCai << 16) | (unsigned short)DaXiaoDanShuang_dw] = _T("��λ��С��˫");
		theGameKind[(nCai << 16) | (unsigned short)Zonghe_DXDSLh] = _T("��ֵ��С��˫");

		theGameKind[(nCai << 16) | (unsigned short)BuDingWei_HouSan] = _T("��������λ");
		theGameKind[(nCai << 16) | (unsigned short)BuDingWei_QianSan] = _T("ǰ������λ");
		theGameKind[(nCai << 16) | (unsigned short)BuDingWei_ZhongSan] = _T("��������λ");

		theGameKind[(nCai << 16) | (unsigned short)DingWeiDan] = _T("���Ƕ�λ��");

		theGameKind[(nCai << 16) | (unsigned short)RenXuan2]=_T("��ѡ��");					//��ѡ2,3,4
		theGameKind[(nCai << 16) | (unsigned short)RenXuan3]=_T("��ѡ��");
		theGameKind[(nCai << 16) | (unsigned short)RenXuan4]=_T("��ѡ��");
		theGameKind[(nCai << 16) | (unsigned short)ZhongSanHunhe]=_T("�������");				//�������
		theGameKind[(nCai << 16) | (unsigned short)HouSanHunhe]=_T("�������");				//�������

		theGameKind[(nCai << 16) | (unsigned short)BuDingWei_HouEr]=_T("�������λ");				//�������λ
		theGameKind[(nCai << 16) | (unsigned short)BuDingWei_QianEr]=_T("ǰ������λ");				//ǰ������λ


		theGameKind[(nCai << 16) | (unsigned short)QianSanHunhe]=_T("ǰ�����");//ǰ�����

	}	

}

bool	CChongQingSSC::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}
// bool	CChongQingSSC::ChangeStringToA(CString strInput, string&  strOutput)
// {
// 	USES_CONVERSION;
// 	
// 	strOutput = T2A(strInput);
// 	return true;
// }

void	CChongQingSSC::GetSubStringA(CStringA& strInput, int nPos, int nLen, CStringA& strOutput)
{

	strOutput = strInput.Mid(nPos, nLen);
	
}
CString	CChongQingSSC::ChangeStringToT(CStringA strInput)
{
	CString  strOutput;
	USES_CONVERSION;
	strOutput = A2T(strInput);

	return strOutput;
}
CString CChongQingSSC::GetScoreMoshi()
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
int CChongQingSSC::GetMoshiRet(CString moshi)
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
void CChongQingSSC::SetTypeID(CaiZhong TypeID)
{
 	m_TypeID = TypeID;
	m_cbIfTs = 0;
	SendToServer(11);

	KillTimer(TimerLotteryStatus);
	SetTimer(TimerLotteryStatus,20000,NULL);
	if(TypeID == CZ_JiaNaDaSSC)
	{
		SendToServer(10);
		SetTimer(timer_getcanadaqihao, 60000, NULL);

	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
}
//��ȡ�¼�
bool CChongQingSSC::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_CANADA_START_QIHAO_RET:
			{
				if(m_TypeID != CZ_JiaNaDaSSC)
					return TRUE;
				ASSERT((wDataSize == sizeof(CMD_GP_GetCanadaQihaoRet)));
				if((wDataSize != sizeof(CMD_GP_GetCanadaQihaoRet)) )return false;

				CMD_GP_GetCanadaQihaoRet* pLogRet = (CMD_GP_GetCanadaQihaoRet*)pData;
				m_nCanadaStartQihao = pLogRet->n_t_start_qihao;

				m_jndRule.SetQihaoStart(pLogRet->n_t_start_qihao,pLogRet->n_t_start_time);

				theCanadaQihao = m_nCanadaStartQihao;
				theCanadaStartTime = pLogRet->n_t_start_time;
				return TRUE;
			}
		case SUB_GP_GET_QIHAO_CHA_RET:
			{
				ASSERT((wDataSize == sizeof(CMD_GP_GetQihaoCha)));
				if((wDataSize != sizeof(CMD_GP_GetQihaoCha)) )return false;

				CMD_GP_GetQihaoCha* pGetQihao = (CMD_GP_GetQihaoCha*)pData;
				if(m_TypeID == CZ_HGYDWFC)
				{
					m_hgRule.SetQihaocha(pGetQihao->n_t_qishu);
				}
				return true;
			}
		case SUB_GP_GET_MORE_RECORD_RET:
			{
				ASSERT((wDataSize % sizeof(CMD_GP_GetMoreRecordRet))==0);
				if((wDataSize %sizeof(CMD_GP_GetMoreRecordRet)) !=0)return false;

				int nCount = wDataSize/sizeof(CMD_GP_GetMoreRecordRet);
				for(int i = 0;i < nCount;i++)
				{
					CMD_GP_GetMoreRecordRet *pGetMoreRecord = ((CMD_GP_GetMoreRecordRet*)pData+i);

					m_DlgOpenRecord.SetOpenRecord(pGetMoreRecord,i);
				}

				m_DlgOpenRecord.ShowWindow(SW_SHOW);
				return true;
			}
		case SUB_GP_GET_USER_FANDIAN_RET:
			{
				ASSERT((wDataSize % sizeof(CMD_GR_GetUserFandianRet))==0);
				if((wDataSize %sizeof(CMD_GR_GetUserFandianRet)) !=0)return false;
				//KillTimer(TimerFandian);

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
// 					if(nKindid == DaXiaoDanShuang || nKindid == DaXiaoDanShuang_dw || nKindid == BuDingWei_QianEr || nKindid == BuDingWei_HouEr || nKindid == BuDingWei_HouSan || nKindid == BuDingWei_QianSan|| nKindid == BuDingWei_ZhongSan)
// 					{
// 						nBonus -= (theAccount.fandian*100 - nPercent )*fDanweiFandian;
// 						nPercent = theAccount.fandian*100;
// 					}
					CString strText;
					strText.Format(L"%.2lf/%.2lf", nBonus, nPercent);

					strText+=_T("%");
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
					SendToServer(6);
					SetLockText(m_fBonus, m_fPercent);
					MyMessageBox(L"���������޸ĳɹ���", L"��ܰ��ʾ");
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


				if (_tcscmp(m_lastExpect, m_kjXinxi[m_TypeID][0].qihao) != 0) 
				{
					//��һ�ν���
					if(m_lastExpect[0] == 0)
						_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, m_kjXinxi[m_TypeID][0].qihao);
					m_bKaiJiangzhong=false;											//��������
					OutputDebugString(L"KAIJIANG false 4");
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

				m_DlgStatus.HideStatusWindow();

				m_bCanSend = true;
				if (lResult == 0)
				{
					if((pTouzhuRes->nSign != m_nTzhSign1))
					{
						m_nTzhSign1 = pTouzhuRes->nSign;
					//	m_listChqSSC.DeleteAllItems();
						MyMessageBox(_T("�ɹ�Ͷע��"), _T("��ף������ Ͷע�ɹ���"));
						SendToServer(7);						

					}
				}
				else if(lResult == 1)
				{
					//MyMessageBox(_T("ע�����󣬻���Ͷע���ݲ���ȷ��"));
					MyMessageBox(_T("ע�����󣬻���Ͷע���ݲ���ȷ��"));
				}
				else if(lResult == 2)
				{
					//MyMessageBox(_T("��ǰ���ѷⵥ��ֹͣ���ۣ�"));
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
					MyMessageBox(_T("ע��У�鲻��ȷ��Ͷעʧ�ܣ�"));
				}
				else if(lResult == 12)
				{
					MyMessageBox(_T("�Բ��𣬸��淨ά���У���ѡ�������淨����Ͷע��"));
				}
				else if(lResult == 13)
				{
					MyMessageBox(_T("�Բ��𣬸ò�����ͣ���ۣ�"));
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
					if(m_bShowFail)
					{
						MyMessageBox(_T("�ж���δ�ɹ��ύ��"));
						m_bShowFail=false;
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

				if(pLogRet->n_t_kindid != m_TypeID)
					return true;

				m_cbIfTs = pLogRet->c_t_ifts;

				if(pLogRet->n_t_kindid == CZ_HGYDWFC)
				{
					int nStartQihao = 0;
					nStartQihao = _ttoi(pLogRet->s_t_expect);
					CTime ctm(pLogRet->n_t_shijian);

					m_hgRule.SetStartQihao(nStartQihao,ctm);
				}
				else if(pLogRet->n_t_kindid == CZ_BJ5FC)
				{
					int nStartQihao = 0;
					nStartQihao = _ttoi(pLogRet->s_t_expect);
					CTime ctm(pLogRet->n_t_shijian);

					m_bj5fcRule.SetStartQihao(nStartQihao,ctm);

				}
				return true;
			}
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}


//�����Ϣ
VOID CChongQingSSC::OnMouseMove(UINT nFlags, CPoint Point)
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
VOID CChongQingSSC::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//���ý���
	SetFocus();
	return;
}
//�����Ϣ
VOID CChongQingSSC::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
}

//�����Ϣ
LRESULT CChongQingSSC::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{

	//����״̬
	m_bHovering=false;

	return 0;
}


//������루��ѡ2����ѡ3����ѡ4��
void CChongQingSSC::OnBnClickedRioMouseInput()
{
	//OnBnClickedBtnClsList();
	if(	m_gameKind == WuxingZhixuan)
	{
		OnBnClickedRioWuXingZhx();
	}
	else if(m_gameKind == WuxingQianSi)
	{
		OnBnClickedRioWuXingQs();
	}
	else if(m_gameKind == WuxingHousi)
	{
		OnBnClickedRioWuXingHS();
	}
	else if(m_gameKind == WuxingZhongsan)
	{
		OnBnClickedRioWuXingZS();
	}
	else if(m_gameKind == HouSanZhiXuan_FuShi)
	{
		OnBnClickedRioHszhxFushi();
	}
	else if(m_gameKind == QianSanZhiXuan_FuShi)
	{
		OnBnClickedRioQszhxFushi();
	}
	else if(m_gameKind == QianSanZuXuan_ZuLiuFuShi)
	{
		OnBnClickedRioQszlFushi();
	}
	else if(m_gameKind == QianSanZuXuan_ZuSanFuShi)
	{
		OnBnClickedRioQszsFushi();
	}
	else if(m_gameKind == ZhongsanZuliu)
	{
		OnBnClickedRioZszlFushi();
	}
	else if(m_gameKind == ZhongsanZusan)
	{
		OnBnClickedRioZszsFushi();
	}
	else if(m_gameKind == HouSanZuXuan_ZuLiuFuShi)
	{
		OnBnClickedRioHszlFushi();
	}
	else if(m_gameKind == HouSanZuXuan_ZuSanFuShi)
	{
		OnBnClickedRioHszsFushi();
	}
	else if(m_gameKind ==  RenXuan2)
	{
		OnBnClickedRioRenXuan2();
	}
	else if(m_gameKind ==  RenXuan3)
	{
		OnBnClickedRioRenXuan3();
	}
	else if(m_gameKind ==  RenXuan4)
	{
		OnBnClickedRioRenXuan4();
	}
	else if(m_gameKind == Zuxuan120)
	{
		OnBnClickedRioZuXuan120();
	}
	else if(m_gameKind == Zuxuan60)
	{
		OnBnClickedRioZuXuan60();
	}
	else if(m_gameKind == Zuxuan30)
	{
		OnBnClickedRioZuXuan30();
	}
	else if(m_gameKind ==  Zuxuan20)
	{
		OnBnClickedRioZuXuan20();
	}
	else if(m_gameKind ==  Zuxuan10)
	{
		OnBnClickedRioZuXuan10();
	}
	else if(m_gameKind ==  Zuxuan5)
	{
		OnBnClickedRioZuXuan5();
	}
	else if(m_gameKind == Ren2Zuxuan_Fushi)
	{
		OnBnClickedRioRenXuan2Zx();
	}
	else if(m_gameKind == Ren3Zu3_Fushi)
	{
		OnBnClickedRioRenXuan3Zx3();
	}
	else if(m_gameKind == Ren3Zu6_Fushi)
	{
		OnBnClickedRioRenXuan3Zx6();
	}


	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);
	ResetAllNums();
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);


	HideDanShiEdit();
	m_RenxuanKeyboardInput=false;
}

//�������루��ѡ2����ѡ3����ѡ4��
void CChongQingSSC::OnBnClickedRioKeyboardInput()
{
//	OnBnClickedBtnClsList();
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

	//��ѡ2,3,4ʱ�������������ǧ���٣�ʮ����
	if(m_gameKind == RenXuan2 || m_gameKind == RenXuan4||m_gameKind == RenXuan3)
		ShowRenxuanWeiShu();
	ResetAllNums();
	m_rioMouseInput.SetPushed(false);
	m_rioKeyboardInput.SetPushed(true);

	HideDaXiaoDanShuangDw();
	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_RenxuanKeyboardInput=true;
}


//�޸���ѡ�� ״̬
void CChongQingSSC::SetCheckRenXuan(bool bChecked)
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

void CChongQingSSC::OnBnClickedChkWan()
{
	ValidateTips();
}
void CChongQingSSC::OnBnClickedChkQian()
{
	ValidateTips();
}

void CChongQingSSC::OnBnClickedChkBai()
{
	ValidateTips();
}

void CChongQingSSC::OnBnClickedChkShi()
{
	ValidateTips();
}

void CChongQingSSC::OnBnClickedChkge()
{
	ValidateTips();
}

//������ʾ
void CChongQingSSC::ValidateTips()
{
	int nCheckCount=0;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;

	CString strTips;
	strTips = TEXT("ϵͳ���Զ����ݹ�ѡλ�ã���϶�Ӧ�ķ���");
	//�ж�����ѡ��
	CImgStatic* pSta = (CImgStatic*)GetDlgItem(IDC_STATIC_RENXUAN_TIP);
	if(RenXuan2 == m_gameKind||Ren2Zuxuan_Fushi == m_gameKind)
	{
		if(nRenXuan2[nCheckCount] == 0)
		{
			strTips = TEXT("ϵͳ���Զ����ݹ�ѡλ�ã���϶�Ӧ�ķ���");
//			if(pSta)
//				pSta->SetWindowText(L"ϵͳ���Զ����ݹ�ѡλ�ã���϶�Ӧ�ķ���");
		}
		else
		{
			
			strTips.Format(L"��ѡ����%ld��λ�ã�ϵͳ�Զ�����λ�����Ϊ%ld��", nCheckCount, nRenXuan2[nCheckCount]);
//			if(pSta)
//				pSta->SetWindowText(strTips);
		}
	}
	else if(RenXuan3 == m_gameKind||Ren3Hunhe_Zuxuan == m_gameKind||Ren3Zu3_Fushi == m_gameKind||Ren3Zu6_Fushi == m_gameKind)
	{
		if(nRenXuan3[nCheckCount] == 0)
		{
			strTips = TEXT("ϵͳ���Զ����ݹ�ѡλ�ã���϶�Ӧ�ķ���");
//			if(pSta)
//				pSta->SetWindowText(L"ϵͳ���Զ����ݹ�ѡλ�ã���϶�Ӧ�ķ���");
		}
		else
		{
//			CString strTips;
			strTips.Format(L"��ѡ����%ld��λ�ã�ϵͳ�Զ�����λ�����Ϊ%ld��", nCheckCount, nRenXuan3[nCheckCount]);
//			if(pSta)
//				pSta->SetWindowText(strTips);
		}
	}
	else if(RenXuan4 == m_gameKind)
	{
		if(nRenXuan4[nCheckCount] == 0)
		{
			strTips = TEXT("ϵͳ���Զ����ݹ�ѡλ�ã���϶�Ӧ�ķ���");
//			if(pSta)
//				pSta->SetWindowText(L"ϵͳ���Զ����ݹ�ѡλ�ã���϶�Ӧ�ķ���");
		}
		else
		{
			strTips.Format(L"��ѡ����%ld��λ�ã�ϵͳ�Զ�����λ�����Ϊ%ld��", nCheckCount, nRenXuan4[nCheckCount]);
//			if(pSta)
//				pSta->SetWindowText(strTips);
		}
	}
	m_static_RenXuan_Tip.Invalidate();
	m_static_RenXuan_Tip.SetWindowText(strTips);

	m_static_RenXuan_Tip.Invalidate();

	if(m_RenxuanKeyboardInput)
		GetDanShiZhusu(m_gameKind);
	else
		GetSSCZhushu(m_TypeID,m_gameKind);
}

//��ѡ����ǧ���٣�ʮ����
int CChongQingSSC::GetRenXuanCheckBox(BOOL& bWan, BOOL& bQian, BOOL& bBai, BOOL& bShi, BOOL& bGe)		
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
void CChongQingSSC::OnBnClickedBtnLock()
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

//��Ͷ��
void CChongQingSSC::OnBnClickedBeiTouJia()
{
 	CString strBeishu;

	m_beishu += 1;

	strBeishu.Format(L"%d",m_beishu);

	m_editBeiTou.SetWindowText(strBeishu);
	return;

}
//��Ͷ��
void CChongQingSSC::OnBnClickedBeiTouJian()
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





/////////////////////////////////////////////////////////////////////////////////////
bool CChongQingSSC::GetKeyBoardHaoma(CString& haoma, int size, int nLeast)
{
	CStringArray strArr;
	CString line;

	m_richDanshiHaoma.SetSel(0,-1);
	line = m_richDanshiHaoma.GetSelText();

	int nFind = line.Find(L"��");
	if(nFind != -1)
	{
		line.Replace(_T("��"),_T(","));
	}

	line.Replace(L"\r",L" ");
	line.Replace(L"\n",L" ");

	if (!GetKeyHaomaStrs(strArr, line, size,nLeast)) 
	{
		return false;
	}

	int count = strArr.GetCount();
	CString strTemp;
	for(int i=0; i<count; i++)
	{
		strTemp = strArr.GetAt(i);
		int nWeishu = strTemp.GetLength();
		if(nWeishu < nLeast)
		{
			MyMessageBox(_T("������ĺ���̫����"));	
			return false;

		}
		for(int j = 0; j<strTemp.GetLength();j++)
		{
			haoma += strTemp.GetAt(j);
			if (m_gameKind!=WuxingZhixuan && (j!=nWeishu-1))
			{
				haoma += _T(",");
			}

			if(j == nWeishu-1)
			{
				haoma +=_T("|");
			}

		}

	}

	return true;
}

bool CChongQingSSC::GetKeyHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast)
{
	int len = line.GetLength();
	CString subStr;
	CString strTemp;
	int nTempHaoma[10];
	int nCount = 0;
	ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));

	for (int i=0; i<len; i++) 
	{
		CString str;
		str=line.GetAt(i);

		int nTemp = 10;
		if (iswdigit(line.GetAt(i))&&!str.IsEmpty()/*&&str.Compare(L"\r\n")!=0*/)
		{
			subStr += line.GetAt(i);

			strTemp+= line.GetAt(i);

			nTemp = _ttoi(strTemp);
			strTemp=_T("");
			if(nTemp<0||nTemp>9)
			{
				nTemp = 10;
				nCount=0;
				ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));

				subStr = _T("");
				continue;
			}
			if((QianSanZuXuan_ZuLiuFuShi == m_gameKind)||(ZhongsanZuliu == m_gameKind) ||(HouSanZuXuan_ZuLiuFuShi == m_gameKind)||(Ren3Zu6_Fushi == m_gameKind))
			{
 				for (int n = 0;n<nCount;n++)
 				{
 					if(nTemp == nTempHaoma[n])
 					{
						nTemp = 10;
						nCount=0;
						ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));

						subStr = _T("");
						continue;
 					}
 				}

			}
			if(m_gameKind == Ren2Zuxuan_Fushi)
			{
				if(nCount == 1)
				{
					if(nTempHaoma[0] == nTemp)
					{
						return false;
					}
				}
			}
			if((QianSanZuXuan_ZuSanDanShi == m_gameKind)||(Zhongsanzusandan == m_gameKind) ||(HouSanZuXuan_ZuSanDanShi == m_gameKind)||(Ren3Zu3_Fushi == m_gameKind)||(Ren3Hunhe_Zuxuan == m_gameKind))
			{
				if(nCount == 2)
				{
					bool bSuit = false;
					bool bOne = false;
					bool bTwo = false;
					bool bThree = false;
					if(nTempHaoma[0] == nTempHaoma[1])
					{
						bOne = true;
					}
					if(nTempHaoma[1] == nTemp)
					{
						bTwo = true;
					}
					if(nTempHaoma[0] == nTemp)
					{
						bThree = true;
					}

					if(bOne || bTwo ||bThree)
					{
						bSuit = true;
					}
					if(!bSuit)
					{
						nTemp = 10;
						nCount=0;
						ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));

						subStr = _T("");
						continue;
					}
					if(bOne && bTwo &&bThree)
					{
// 						CString strLog;
// 						strLog.Format(L"����ѡ���Ӻ���",nTempHaoma[0],nTempHaoma[1],nTemp);
// 						MyMessageBox(strLog);
						nTemp = 10;
						nCount=0;
						ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));

						subStr = _T("");
						continue;
					}
				}

			}
			if(m_gameKind == Zuxuan60)
			{
				if((nCount == 1) && (nTemp != nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"�����һλ�͵ڶ�λ��һ�£�������Ͷע���� ");
					return false;
				}
				else if (nCount > 1 && (nTemp == nTempHaoma[0]))
				{
					MyMessageBox(L"����λ���ܸ��غ�һ��");
					return false;
				}
			}
			if(m_gameKind == Zuxuan30)
			{
				if((nCount == 1) && (nTemp != nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"�����һλ�͵ڶ�λ��һ�£�������Ͷע���� ");
					return false;
				}
				if((nCount == 3) && (nTemp != nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"�������λ�͵���λ��һ�£�������Ͷע���� ");
					return false;
				}
				else if (nCount > 3 &&((nTemp == nTempHaoma[0])||(nTemp == nTempHaoma[2])))
				{
					MyMessageBox(L"���һλ���ܸ��غ�һ��");
					return false;
				}
			}
			if(m_gameKind == Zuxuan20)
			{
				if((nCount > 0&&nCount<3) && (nTemp != nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"ǰ��λ��һ�£�������Ͷע���� ");
					return false;
				}
				else if((nCount == 3) && (nTemp == nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"�������λ���ܺ��غ�һ�£�������Ͷע���� ");
					return false;
				}
				else if((nCount == 4) && (nTemp == nTempHaoma[0]))
				{
					MyMessageBox(L"�������λ���ܺ��غ�һ�£�������Ͷע���� ");
					return false;
				}
			}
			if(m_gameKind == Zuxuan10)
			{
				if((nCount > 0&&nCount<3) && (nTemp != nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"ǰ��λ��һ�£�������Ͷע���� ");
					return false;
				}
				else if((nCount == 3) && (nTemp == nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"�������λ���ܺ��غ�һ�£�������Ͷע���� ");
					return false;
				}
				else if((nCount == 4) && (nTemp != nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"�������λ�͵���λ��һ�£�������Ͷע���� ");
					return false;
				}
			}
			if(m_gameKind == Zuxuan5)
			{
				if((nCount > 0&&nCount<4) && (nTemp != nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"ǰ��λ��һ�£�������Ͷע���� ");
					return false;
				}
				else if((nCount == 4) && (nTemp == nTempHaoma[nCount-1]))
				{
					MyMessageBox(L"�������λ���غŲ���һ�£�������Ͷע���� ");
					return false;
				}
			}
			nTempHaoma[nCount++] = nTemp;

			if (subStr.GetLength() > nLeast) 
			{
				nTemp = 10;
				nCount=0;
				ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));

				subStr = _T("");
				continue;
			}
		}
		else
		{
			nTemp = 10;
			nCount=0;
			ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));

			if (subStr.GetLength() > size  || (subStr.GetLength()<nLeast&&subStr.GetLength() != 0) )
			{
				subStr = _T("");
				continue;
			}

			if (subStr.GetLength() == nLeast) 
			{
				if(subStr == _T("055"))
				{
					CString strLog;
					strLog.Format(L"\nDANSHI subStr:%s",subStr);
					OutputDebugString(strLog);

				}
				CString strLog;
				strLog.Format(L"\nDANSHI subStr:%s",subStr);
				OutputDebugString(strLog);
				strArr.Add(subStr);
				subStr = _T("");
			}
		}
	}
	if(( subStr.GetLength() == 0)&&(strArr.GetCount()==0))
	{
		MyMessageBox(_T("��������룡"));	

		return false;

	}


	if (subStr.GetLength() == nLeast) 
	{
		strArr.Add(subStr);
		subStr = _T("");

	}

	return true;
}

void CChongQingSSC::OnBnClickedBtnQian5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

BOOL CChongQingSSC::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	long nParam = lParam;

	if(nCommandID == IDC_RICHEDIT_DANSHI)
	{
		if(nParam == ENM_CHANGE)
		{
			CString strMessage;
			m_richDanshiHaoma.GetWindowText(strMessage);
			MyMessageBox(strMessage);
		}
	}
	if((nCommandID >= IDC_BTN_LONG && nCommandID <= IDC_BTN_HE)||(nCommandID >= IDC_BTN_WAN_0 && nCommandID <= IDC_BTN_GE_9)||(nCommandID >= IDC_BTN_DA_SHI && nCommandID <= IDC_BTN_WAN_2)||(nCommandID >= IDC_BTN_HEZHI_0 && nCommandID <= IDC_BTN_HEZHI_27)||(nCommandID >= IDC_BTN_DA_WAN && nCommandID <= IDC_BTN_SHUANG_BAI)||(nCommandID >= IDC_BTN_NIU_1 && nCommandID <= IDC_BTN_NIU_10))
	{
		int nZhushu = GetSSCZhushu(m_TypeID,m_gameKind);
	}


	return  __super::OnCommand(wParam,lParam);;
}
int CChongQingSSC::GetSSCZhushu(int nTypeID,int nKindID)
{
	CString strWan = GetWanString();
	CString strQian = GetQianString();
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();
	int nZongZhu = 0;
	int nIndex = 0;
	CString strHaoma;
	m_singlezhushu = 0;
	CString strLog;

	if(nKindID == WuxingZhixuan)  //����
	{
		strHaoma = strWan + TEXT(",") + strQian + TEXT(",") + strBai + TEXT(",") + strShi + TEXT(",") + strGe;

		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 1;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=200;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;

			}

			int nLength = strDanHao.GetLength();
			nSingleZhushu *=nLength;

		} while (nWei<5);
		m_singlezhushu+=nSingleZhushu;
	}
	else if(nKindID == Zonghe_DXDSLh)
	{
		CString strDxds = GetBaiDxdshString();
		if(strDxds.GetLength()==1)
			m_singlezhushu = 1;
		else
			m_singlezhushu = 0;

	}
	else if(nKindID == SSC_LongHu)
	{
		CString strLonghu = GetLongHu();
		if(strLonghu.GetLength()==1)
		{
			m_singlezhushu = 1;
		}
		else
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
	else if(nKindID == Ren3Zu3_Fushi)
	{
		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
		int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
		int nBaiLen = strBai.GetLength();
		if(nCount < 3)
		{
			
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=200;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		else if(nCount == 3)
		{
			m_singlezhushu = nBaiLen* (nBaiLen - 1);
		}
		else if(nCount == 4)
		{
			m_singlezhushu = nCount*nBaiLen * (nBaiLen - 1);
		}
		else if(nCount == 5)
		{
			m_singlezhushu = nCount*nBaiLen * (nBaiLen - 1)*2;
		}

	}
	else if(nKindID == Ren2Zuxuan_Fushi)
	{
		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
		int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
		int nBaiLen = strBai.GetLength();
		if(nCount < 2)
		{

			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=200;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		else if(nCount == 2)
		{
			m_singlezhushu = nBaiLen * (nBaiLen - 1) / 2;
		}
		else if(nCount == 3)
		{
			m_singlezhushu = nCount*nBaiLen * (nBaiLen - 1) / 2;
		}
		else if(nCount == 4)
		{
			m_singlezhushu = (nCount-1)*(nCount-2)*nBaiLen * (nBaiLen - 1)/ 2;
		}
		else if(nCount == 5)
		{
			m_singlezhushu = nCount*nBaiLen * (nBaiLen - 1);
		}

	}
	else if(nKindID == Ren3Zu6_Fushi)
	{
		BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
		int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);
		int nBaiLen = strBai.GetLength();
		if(nCount < 3)
		{

			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=200;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		else if(nCount == 3)
		{
			m_singlezhushu = nBaiLen* (nBaiLen - 1) * (nBaiLen - 2) / 6;
		}
		else if(nCount == 4)
		{
			m_singlezhushu = nCount*nBaiLen * (nBaiLen - 1) * (nBaiLen - 2) / 6;
		}
		else if(nCount == 5)
		{
			m_singlezhushu = nCount*nBaiLen * (nBaiLen - 1)*2 * (nBaiLen - 2) / 6;
		}
	}
	else if(nKindID == WuxingQianSi || nKindID == WuxingHousi)  // ����
	{
		if(nKindID == WuxingQianSi)
			strHaoma = strWan + TEXT(",") + strQian + TEXT(",") + strBai + TEXT(",") + strShi ;
		else
			strHaoma = strQian + TEXT(",") + strBai + TEXT(",") + strShi + TEXT(",") + strGe;


		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 1;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=200;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;

			}
			int nLength = strDanHao.GetLength();
			nSingleZhushu *=nLength;

		} while (nWei<4);
		m_singlezhushu+=nSingleZhushu;

	}
	else if(nKindID == WuxingZhongsan || nKindID == QianSanZhiXuan_FuShi || nKindID == HouSanZhiXuan_FuShi) // ����
	{
		if(nKindID == WuxingZhongsan)
			strHaoma = strQian + TEXT(",") + strBai + TEXT(",") + strShi;
		else if(nKindID == QianSanZhiXuan_FuShi)
			strHaoma = strWan + TEXT(",") + strQian + TEXT(",") + strBai;
		else if(nKindID == HouSanZhiXuan_FuShi)
			strHaoma = strBai + TEXT(",") + strShi + TEXT(",") + strGe;

		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 1;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=200;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;

			}

			int nLength = strDanHao.GetLength();
			nSingleZhushu *=nLength;

		} while (nWei<3);
		m_singlezhushu+=nSingleZhushu;

	}
	else if(nKindID == WuXingQianEr_FuShi || nKindID == WuXingHouEr_FuShi)   //����
	{
		if(nKindID == WuXingHouEr_FuShi)
			strHaoma = strShi + TEXT(",") + strGe;
		else if(nKindID == WuXingQianEr_FuShi)
			strHaoma = strWan + TEXT(",") + strQian ;
		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 1;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=200;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;

			}
			int nLength = strDanHao.GetLength();
			nSingleZhushu *=nLength;
		} while (nWei<2);
		m_singlezhushu+=nSingleZhushu;

	}
	else if(nKindID == QianSanZuXuan_ZuLiuFuShi || nKindID == HouSanZuXuan_ZuLiuFuShi || nKindID == ZhongsanZuliu) //ǰ����������   ��ѡ��
	{
	//	strHaoma = strBai;
		int nBaiLength = strBai.GetLength();
		for(int i = 0 ;i < nBaiLength;i++)
		{
			strHaoma += strBai.GetAt(i);
			if(i == nBaiLength-1)
				break;
			strHaoma += TEXT(",");
		}
		int nLength = strHaoma.GetLength()/2+1;
		int nSingleZhu = (nLength*(nLength-1)*(nLength-2))/6;

		m_singlezhushu +=nSingleZhu;
	}
	else if(nKindID == QianSanZuXuan_ZuSanFuShi || nKindID == HouSanZuXuan_ZuSanFuShi || nKindID == ZhongsanZusan)  //ǰ����������   ��ѡ��
	{
		int nBaiLength = strBai.GetLength();
		for(int i = 0 ;i < nBaiLength;i++)
		{
			strHaoma += strBai.GetAt(i);
			if(i == nBaiLength-1)
				break;
			strHaoma += TEXT(",");
		}
		int nLength = strHaoma.GetLength()/2+1;

		bool bKeyboard = false;
		if(nLength == 3)
		{
			int nWei = 0;
			CString strDanHao;

			CString strSingleHao[3];
			do 
			{
				if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
					break;
				if(strDanHao.IsEmpty())
					break;
				strSingleHao[nWei-1] = strDanHao;
			}
			while(nWei<3);
			if((strSingleHao[0] == strSingleHao[1])||(strSingleHao[1] == strSingleHao[2])||(strSingleHao[0] == strSingleHao[2]))
			{
				bKeyboard = true;
			}
		}
		int nSingleZhu = 0;
		if(bKeyboard)
			nSingleZhu = 1;
		else
			nSingleZhu = (nLength*(nLength-1));


		m_singlezhushu +=nSingleZhu;
	}
	else if(nKindID == QianErZuXuan_FuShi || nKindID == HouErZuXuan_FuShi)  //ǰ�� �����ѡ
	{
		int nBaiLength = strBai.GetLength();
		for(int i = 0 ;i < nBaiLength;i++)
		{
			strHaoma += strBai.GetAt(i);
			if(i == nBaiLength-1)
				break;
			strHaoma += TEXT(",");
		}

		int nLength = strHaoma.GetLength()/2+1;
		int nSingleZhu = 0;

		nSingleZhu = (nLength*(nLength-1))/2;
		m_singlezhushu += nSingleZhu;

	}
	else if(nKindID == QianSanZhiXuanHeZhi || nKindID == HouSanZhiXuanHeZhi || nKindID == ZhongSanZhiXuanHeZhi)
	{
		CString strHezhi;
		strHezhi = GetHezhiString();

		int nHezhiLength = strHezhi.GetLength();
		for(int i = 0;i < nHezhiLength;i+=2)
		{
			strHaoma += strHezhi.GetAt(i);
			strHaoma += strHezhi.GetAt(i+1);
			if(i != nHezhiLength -2)
				strHaoma += TEXT(",");
		}
		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 0;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
				break;

			int nHezhi = 0;
			nHezhi = _ttoi(strDanHao);
			for(int m = 0;m < 10;m++)
			{
				for(int n = 0;n < 10;n++)
				{
					for (int o = 0;o < 10;o++)
					{
						if((m+n+o) == nHezhi)
						{
							nSingleZhushu++;
						}
					}
				}
			}

		} while (nWei<28);
		m_singlezhushu+=nSingleZhushu;

	}
	else if(nKindID == QianErZhixuan_Hezhi || nKindID == HouErZhixuan_Hezhi)
	{
		CString strHezhi;
		strHezhi = GetHezhiString();

		int nHezhiLength = strHezhi.GetLength();
		for(int i = 0;i < nHezhiLength;i+=2)
		{
			strHaoma += strHezhi.GetAt(i);
			strHaoma += strHezhi.GetAt(i+1);
			if(i != nHezhiLength -2)
				strHaoma += TEXT(",");
		}
		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 0;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
				break;

			int nHezhi = 0;
			nHezhi = _ttoi(strDanHao);
			for(int m = 0;m < 10;m++)
			{
				for(int n = 0;n < 10;n++)
				{
					if((m+n) == nHezhi)
					{
						nSingleZhushu++;
					}
				}
			}


		} while (nWei<=18);
		m_singlezhushu+=nSingleZhushu;

	}
	else if(nKindID == DaXiaoDanShuang)
	{
		CString strDxdshShi = GetShiDxdshString();
		CString strDxdshGe = GetGeDxdshString();

		strHaoma = strDxdshShi + TEXT(",") + strDxdshGe;
		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 1;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=200;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;

			}
			int nLength = strDanHao.GetLength();
			nSingleZhushu *= nLength;
		}
		while(nWei<2);
		m_singlezhushu+=nSingleZhushu;

	}
	else if(nKindID == DaXiaoDanShuang_dw)
	{
		CString strDxdshWan = GetWanDxdshString();
		CString strDxdshQian = GetQianDxdshString();
		CString strDxdshBai = GetBaiDxdshString();
		CString strDxdshShi = GetShiDxdshString();
		CString strDxdshGe = GetGeDxdshString();

		if (strDxdshWan.GetLength()>1 || strDxdshQian.GetLength()>1 || strDxdshBai.GetLength()>1 || strDxdshShi.GetLength()>1 || strDxdshGe.GetLength()>1 ) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=200;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		strHaoma = strDxdshWan + TEXT(",") + strDxdshQian + TEXT(",") + strDxdshBai + TEXT(",") +strDxdshShi + TEXT(",") + strDxdshGe;
		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 0;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

 			if(!strDanHao.IsEmpty())
				nSingleZhushu += 1;
		}
		while(nWei<5);
		m_singlezhushu+=nSingleZhushu;

	}
	else if(nKindID == BuDingWei_HouSan || nKindID == BuDingWei_QianSan || nKindID == BuDingWei_QianEr || nKindID == BuDingWei_HouEr || nKindID == BuDingWei_ZhongSan)
	{
		int nBaiLength = strBai.GetLength();
		for(int i = 0 ;i < nBaiLength;i++)
		{
			strHaoma += strBai.GetAt(i);
			if(i!=nBaiLength-1)
				strHaoma += TEXT(",");
		}
		int nLength = strHaoma.GetLength()/2+1;
		if(nLength<=1)
			nLength = 0;
		int nSingleZhu = 0;

		nSingleZhu = nLength;

		m_singlezhushu +=nSingleZhu;
	}
	else if(nKindID == DingWeiDan)
	{
		strHaoma = strWan + TEXT(",") + strQian + TEXT(",") + strBai + TEXT(",") + strShi + TEXT(",") + strGe;
		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 0;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
				continue;
			int nLength = strDanHao.GetLength();
			nSingleZhushu += nLength;
		}
		while(nWei<5);
		m_singlezhushu+=nSingleZhushu;

	}
// 		else if(nKindID == QianSanHunhe || nKindID == HouSanHunhe || nKindID == ZhongSanHunhe)
// 		{
// 			nZongZhu+=1;
// 		}
	else if(nKindID == QW_YiFanFengShun || nKindID == QW_HaoShiChengShuang || nKindID == QW_SanXingBaoXi || nKindID == QW_SiJiFaCai)
	{
		int nBaiLength = strBai.GetLength();
		for(int i = 0 ;i < nBaiLength;i++)
		{
			strHaoma += strBai.GetAt(i);
			if(i!=nBaiLength-1)
				strHaoma += TEXT(",");
		}

		int nLength = strHaoma.GetLength()/2+1;
		if(nLength<=1&&nBaiLength<=0)
			nLength = 0;

		int nSingleZhushu = nLength;
		m_singlezhushu+=nSingleZhushu;

	}
	else if(nKindID == RenXuan2 || nKindID == RenXuan3 || nKindID == RenXuan4)
	{
		strHaoma = strWan + TEXT(",") + strQian + TEXT(",") + strBai + TEXT(",") + strShi + TEXT(",") + strGe;

		int nWeishu = 0;

		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 1;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
				continue;
			nWeishu++;
			int nLength = strDanHao.GetLength();
			nSingleZhushu *= nLength;
		}
		while(nWei<5);
		if(nKindID == RenXuan2 && nWeishu < 2 || nKindID == RenXuan3 && nWeishu < 3 || nKindID == RenXuan4 && nWeishu < 4)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=200;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}
// 		if(nWeishu < 2)
// 			return 0;
		 if(nKindID == RenXuan2 && nWeishu > 2)
		 {
			 m_singlezhushu = 0;
			 CRect rcInvali;
			 rcInvali.CopyRect(rcZongZhuShu);
			 rcInvali.right+=200;
			 InvalidateRect(&rcInvali);
			 InvalidateRect(&rcZongJinE);

			 return m_singlezhushu;

		 }
		 else if(nKindID == RenXuan3 && nWeishu > 3)
		 {
			 m_singlezhushu = 0;
			 CRect rcInvali;
			 rcInvali.CopyRect(rcZongZhuShu);
			 rcInvali.right+=200;
			 InvalidateRect(&rcInvali);
			 InvalidateRect(&rcZongJinE);

			 return m_singlezhushu;

		 }
		 else if(nKindID == RenXuan4 && nWeishu > 4)
		 {
			 m_singlezhushu = 0;
			 CRect rcInvali;
			 rcInvali.CopyRect(rcZongZhuShu);
			 rcInvali.right+=200;
			 InvalidateRect(&rcInvali);
			 InvalidateRect(&rcZongJinE);

			 return m_singlezhushu;

		 }

		m_singlezhushu+=nSingleZhushu;
	}
	else if(nKindID == NiuNiu_Num)
	{
		CString strNiuNum = GetNiuNumString();
		int nNiuLength = strNiuNum.GetLength();
		strHaoma = strNiuNum;
		CString strDanHao;
		int nWei = 0;
		int nSingleZhushu = 0;
		do 
		{
			if(!AfxExtractSubString(strDanHao,strHaoma,  nWei++, ','))
				break;

			if(strDanHao.IsEmpty())
				continue;
			int nLength = strDanHao.GetLength();
			nSingleZhushu += 1;
		}
		while(nWei<10);
		m_singlezhushu=nNiuLength/2;

	}
	else if(nKindID == QSZuXuan24||nKindID == HSZuXuan24)
	{
		if(strBai.GetLength()<4)
		{
			m_singlezhushu = 0;
		}
		else
		{
			int nCount = strBai.GetLength();
			m_singlezhushu = (nCount-3)*(nCount-2)*(nCount-1)*nCount/24;
		}
	}
	else if(nKindID == QSZuXuan12||nKindID == HSZuXuan12)
	{
		if(strBai.GetLength()<1 || strShi.GetLength()<2)
		{
			m_singlezhushu = 0;
		}
		else
		{
			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();
			CString strTempBai,strTempShi;

			int nChongfu = 0;
			nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
						if((nCountShi == nChongfu) || (nCountBai == nChongfu && nCountBai != 1))
						{
							return m_singlezhushu;
						}
					}
				}

				int nFenzi = 1;
				int nFenmu = 1;
				for(int m = (nCountShi - nChongfu);m >0;m-- )
				{
					nFenzi *= m;
				}
				for(int m = (nCountShi - nChongfu-2);m > 0;m--)
				{
					nFenmu *= m;
				}

				nFenmu*=2;
				m_singlezhushu += nFenzi/nFenmu;

				nChongfu=0;
			}

		}
	}
	else if(nKindID == QSZuXuan6||nKindID == HSZuXuan6)
	{
		if(strBai.GetLength()<2)
		{
			m_singlezhushu = 0;
		}
		else
		{
			int nCount = strBai.GetLength();
			m_singlezhushu = (nCount-1)*nCount/2;

		}
	}
	else if(nKindID == QSZuXuan4||HSZuXuan4==nKindID)
	{
		if(strBai.GetLength()<1 || strShi.GetLength()<1)
		{
			m_singlezhushu = 0;
		}
		else
		{
			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();

			if(nCountShi == 1 && strShi == strBai)
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=200;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;
			}
			CString strTempBai,strTempShi;

			int nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
					}
				}
				m_singlezhushu+= (nCountShi-nChongfu);
				nChongfu = 0;
			}

		}	
	}
	else if(nKindID == Zuxuan120)
	{
		if(strBai.GetLength()<5)
		{
			 m_singlezhushu = 0;
		}
		else
		{
			int nCount = strBai.GetLength();
			m_singlezhushu = (nCount-4)*(nCount-3)*(nCount-2)*(nCount-1)*nCount/120;
		}
	}
	else if(nKindID == Zuxuan60)
	{
		if(strBai.GetLength()<1 || strShi.GetLength()<3)
		{
			m_singlezhushu = 0;
		}
		else
		{
			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();
			CString strTempBai,strTempShi;

			int nChongfu = 0;
			nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
						if((nCountShi == nChongfu) || (nCountBai == nChongfu && nCountBai != 1))
						{
							return m_singlezhushu;
						}
					}
				}

				int nFenzi = 1;
				int nFenmu = 1;
				for(int m = (nCountShi - nChongfu);m >0;m-- )
				{
					nFenzi *= m;
				}
				for(int m = (nCountShi - nChongfu-3);m > 0;m--)
				{
					nFenmu *= m;
				}

				nFenmu*=6;
				m_singlezhushu += nFenzi/nFenmu;

				nChongfu=0;
			}

		}
	}
	else if(nKindID == Zuxuan30)
	{
		if(strBai.GetLength()<2 || strShi.GetLength()<1)
		{
			m_singlezhushu = 0;
		}
		else
		{
			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();
			CString strTempBai,strTempShi;

			//zhushu = 0;
			int nChongfu = 0;
			for (int i = 0;i < nCountShi;i++)
			{
				strTempShi = strShi.GetAt(i);
				for (int j = 0;j < nCountBai;j++)
				{
					strTempBai = strBai.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
						if((nCountBai == nChongfu)||(nCountShi == nChongfu))
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

				int nFenzi = 1;
				int nFenmu = 1;
				for(int m = (nCountBai - nChongfu);m >0;m-- )
				{
					nFenzi *= m;
				}
				for(int m = (nCountBai - nChongfu-2);m > 0;m--)
				{
					nFenmu *= m;
				}

				nFenmu*=2;
				m_singlezhushu += nFenzi/nFenmu;
				nChongfu=0;
			}
			if(m_singlezhushu == 0)
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
	else if(nKindID == Zuxuan20)
	{
		if(strBai.GetLength()<1 || strShi.GetLength()<2)
		{
			m_singlezhushu	= 0;
		}
		else
		{
			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();
			CString strTempBai,strTempShi;

			int nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
						if((strBai == strShi)&&(nCountBai == 2)&&(nCountShi == 2))
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

				int nFenzi = 1;
				int nFenmu = 1;
				nFenzi = (nCountShi-nChongfu)*(nCountShi-nChongfu-1);

				nFenmu*=2;
				m_singlezhushu += nFenzi/nFenmu;
				nChongfu=0;
			}
		}


	}
	else if(nKindID == Zuxuan10)
	{
		if(strBai.GetLength()<1 || strShi.GetLength()<1)
		{
			m_singlezhushu	= 0;
		}
		else 
		{
			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();

			if(nCountShi == 1 && strShi == strBai)
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=200;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;
			}

			CString strTempBai,strTempShi;

			int nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
					}
				}
				m_singlezhushu+= (nCountShi-nChongfu);
				nChongfu = 0;
			}
		}

	}
	else if(nKindID == Zuxuan5)
	{
		if(strBai.GetLength()<1 || strShi.GetLength()<1)
		{
			m_singlezhushu = 0;
		}
		else
		{
			int nCountBai = strBai.GetLength();
			int nCountShi = strShi.GetLength();

			if(nCountShi == 1 && strShi == strBai)
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=200;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;
			}
			CString strTempBai,strTempShi;

			int nChongfu = 0;
			for (int i = 0;i < nCountBai;i++)
			{
				strTempBai = strBai.GetAt(i);
				for (int j = 0;j < nCountShi;j++)
				{
					strTempShi = strShi.GetAt(j);
					if(strTempBai == strTempShi)
					{
						nChongfu++;
					}
				}
				m_singlezhushu+= (nCountShi-nChongfu);
				nChongfu = 0;
			}

		}


	}
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=200;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	return m_singlezhushu;
}
