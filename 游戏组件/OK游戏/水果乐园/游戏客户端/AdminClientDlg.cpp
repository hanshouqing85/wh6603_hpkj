#include "stdafx.h"
#include "AdminClientDlg.h"
#include "resource.h"
#include "GameClientDlg.h"

IMPLEMENT_DYNAMIC(CAdminClientDlg, CDialog)
BEGIN_MESSAGE_MAP(CAdminClientDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SYN, OnBnClickedButtonSyn)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
END_MESSAGE_MAP()

CAdminClientDlg::CAdminClientDlg()
: CDialog(IDD_DIALOG_SYSTEM)
{
	memset(m_lAllUserBet,0,sizeof(m_lAllUserBet));
}

CAdminClientDlg::~CAdminClientDlg()
{
}

void CAdminClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//初始化
BOOL CAdminClientDlg::OnInitDialog()
{
	SetWindowText(TEXT("胜利区域控制"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("控制局数："));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("区域控制："));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("控制说明："));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT("区域输赢控制比游戏库存控制策略优先！"));
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("控制局数："));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("区域控制："));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("控制说明："));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT("区域输赢控制比游戏库存控制策略优先！"));
	SetDlgItemText(IDC_STATIC_INFO,TEXT(""));
	SetDlgItemText(IDC_RADIO_0,TEXT("BAR"));
	SetDlgItemText(IDC_RADIO_1,TEXT("双七"));
	SetDlgItemText(IDC_RADIO_2,TEXT("星星"));
	SetDlgItemText(IDC_RADIO_3,TEXT("西瓜"));
	SetDlgItemText(IDC_RADIO_4,TEXT("铃铛"));
	SetDlgItemText(IDC_RADIO_5,TEXT("木瓜"));
	SetDlgItemText(IDC_RADIO_6,TEXT("桔子"));
	SetDlgItemText(IDC_RADIO_7,TEXT("苹果"));
	SetDlgItemText(IDC_RADIO_10,TEXT("BARx25"));
	SetDlgItemText(IDC_RADIO_11,TEXT("双七x2"));
	SetDlgItemText(IDC_RADIO_12,TEXT("星星x2"));
	SetDlgItemText(IDC_RADIO_13,TEXT("西瓜x2"));
	SetDlgItemText(IDC_RADIO_14,TEXT("铃铛x2"));
	SetDlgItemText(IDC_RADIO_15,TEXT("木瓜x2"));
	SetDlgItemText(IDC_RADIO_16,TEXT("桔子x2"));
	SetDlgItemText(IDC_RADIO_17,TEXT("苹果x2"));
	SetDlgItemText(IDC_BUTTON_RESET,TEXT("取消控制"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("本局信息"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("执行"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("取消"));;
	SetDlgItemText(IDC_STATIC_NOTIC2,TEXT("真人下注："));
	ResetUserBet();
	return TRUE; 
}

//设置颜色
HBRUSH CAdminClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_STATIC_INFO) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	} 
	return hbr;
}

//取消控制
void CAdminClientDlg::OnBnClickedButtonReset()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication,sizeof(ControlApplication));
	ControlApplication.cbControlAppType=C_CA_CANCELS;
	g_pGameClientDlg->OnAdminControl((WPARAM)&ControlApplication,0);
}

//本局信息
void CAdminClientDlg::OnBnClickedButtonSyn()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication,sizeof(ControlApplication));
	ControlApplication.cbControlAppType=C_CA_UPDATE;
	g_pGameClientDlg->OnAdminControl((WPARAM)&ControlApplication,0);
}

//开启控制
void CAdminClientDlg::OnBnClickedButtonOk()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication,sizeof(ControlApplication));

	BYTE cbSelectedIndex=0xff;//控制区域
	if(((CButton*)GetDlgItem(IDC_RADIO_0))->GetCheck()) cbSelectedIndex=3;
	if(((CButton*)GetDlgItem(IDC_RADIO_1))->GetCheck()) cbSelectedIndex=15;
	if(((CButton*)GetDlgItem(IDC_RADIO_2))->GetCheck()) cbSelectedIndex=19;
	if(((CButton*)GetDlgItem(IDC_RADIO_3))->GetCheck()) cbSelectedIndex=7;
	if(((CButton*)GetDlgItem(IDC_RADIO_4))->GetCheck()) cbSelectedIndex=1;
	if(((CButton*)GetDlgItem(IDC_RADIO_5))->GetCheck()) cbSelectedIndex=6;
	if(((CButton*)GetDlgItem(IDC_RADIO_6))->GetCheck()) cbSelectedIndex=12;
	if(((CButton*)GetDlgItem(IDC_RADIO_7))->GetCheck()) cbSelectedIndex=10;

	if(((CButton*)GetDlgItem(IDC_RADIO_10))->GetCheck()) cbSelectedIndex=2;
	if(((CButton*)GetDlgItem(IDC_RADIO_11))->GetCheck()) cbSelectedIndex=14;
	if(((CButton*)GetDlgItem(IDC_RADIO_12))->GetCheck()) cbSelectedIndex=20;
	if(((CButton*)GetDlgItem(IDC_RADIO_13))->GetCheck()) cbSelectedIndex=8;
	if(((CButton*)GetDlgItem(IDC_RADIO_14))->GetCheck()) cbSelectedIndex=23;
	if(((CButton*)GetDlgItem(IDC_RADIO_15))->GetCheck()) cbSelectedIndex=17;
	if(((CButton*)GetDlgItem(IDC_RADIO_16))->GetCheck()) cbSelectedIndex=11;
	if(((CButton*)GetDlgItem(IDC_RADIO_17))->GetCheck()) cbSelectedIndex=5;

	int nSelectTimes=((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();//控制次数
//printf("cbSelectedIndex=%d\n",cbSelectedIndex);
//printf("nSelectTimes=%d\n",nSelectTimes);
	if (cbSelectedIndex!=0xff && nSelectTimes>=0 && nSelectTimes!=0xffffffff)
	{
		ControlApplication.cbControlAppType=C_CA_SET;
		ControlApplication.cbControlArea=cbSelectedIndex;
		ControlApplication.cbControlTimes=static_cast<BYTE>(nSelectTimes+1);
		g_pGameClientDlg->OnAdminControl((WPARAM)&ControlApplication,0);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_INFO,TEXT("请选择受控次数以及受控区域！"));
	}
}

//取消关闭
void CAdminClientDlg::OnBnClickedButtonCancel()
{
	ShowWindow(SW_HIDE);
}

//更新控制
void __cdecl CAdminClientDlg::UpdateControl( CMD_S_ControlReturns* pControlReturns )
{
	switch(pControlReturns->cbReturnsType)
	{
	case S_CR_FAILURE:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("操作失败！"));
			break;
		}
	case S_CR_UPDATE_SUCCES:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			mysprintf(zsText,CountArray(zsText),TEXT("更新数据成功！\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_SET_SUCCESS:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			mysprintf(zsText,CountArray(zsText),TEXT("设置数据成功！\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_CANCEL_SUCCESS:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("取消设置成功！"));
			break;
		}
	}
}

void CAdminClientDlg::ResetUserBet()
{
	memset(m_lAllUserBet,0,sizeof(m_lAllUserBet));
	SetDlgItemText(IDC_STATIC_0,TEXT(" BAR：-----"));
	SetDlgItemText(IDC_STATIC_1,TEXT("双七：-----"));
	SetDlgItemText(IDC_STATIC_2,TEXT("星星：-----"));
	SetDlgItemText(IDC_STATIC_3,TEXT("西瓜：-----"));
	SetDlgItemText(IDC_STATIC_4,TEXT("铃铛：-----"));
	SetDlgItemText(IDC_STATIC_5,TEXT("木瓜：-----"));
	SetDlgItemText(IDC_STATIC_6,TEXT("桔子：-----"));
	SetDlgItemText(IDC_STATIC_7,TEXT("苹果：-----"));
}

void CAdminClientDlg::SetUserBetScore(BYTE cbArea, LONGLONG lScore)
{
	m_lAllUserBet[cbArea]+=lScore;
	CString strPrint;
	switch(cbArea)
	{
	case 0: 
		strPrint.Format(TEXT(" BAR：%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_0,strPrint);
		break;
	case 1: 
		strPrint.Format(TEXT("双七：%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_1,strPrint);
		break;
	case 2: 
		strPrint.Format(TEXT("星星：%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_2,strPrint);
		break;
	case 3: 
		strPrint.Format(TEXT("西瓜：%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_3,strPrint);
		break;
	case 4: 
		strPrint.Format(TEXT("铃铛：%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_4,strPrint);
		break;
	case 5: 
		strPrint.Format(TEXT("木瓜：%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_5,strPrint);
		break;
	case 6: 
		strPrint.Format(TEXT("桔子：%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_6,strPrint);
		break;
	case 7: 
		strPrint.Format(TEXT("苹果：%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_7,strPrint);
		break;
	}
}

//信息
void CAdminClientDlg::PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes)
{
	if (cbArea==0xff)
	{
		mysprintf(pText,cbCount,TEXT("暂时无控制。"));
		return;
	}

	CString szInfo=TEXT("胜利区域：");
	if (cbArea==3) szInfo+=TEXT(" BAR");
	if (cbArea==15) szInfo+=TEXT("双七");
	if (cbArea==19) szInfo+=TEXT("星星");
	if (cbArea==7) szInfo+=TEXT("西瓜");
	if (cbArea==1) szInfo+=TEXT("铃铛");
	if (cbArea==6) szInfo+=TEXT("木瓜");
	if (cbArea==12) szInfo+=TEXT("桔子");
	if (cbArea==10) szInfo+=TEXT("苹果");

	if (cbArea==2) szInfo+=TEXT("BARx25");
	if (cbArea==14) szInfo+=TEXT("双七x2");
	if (cbArea==20) szInfo+=TEXT("星星x2");
	if (cbArea==8) szInfo+=TEXT("西瓜x2");
	if (cbArea==23) szInfo+=TEXT("铃铛x2");
	if (cbArea==17) szInfo+=TEXT("木瓜x2");
	if (cbArea==11) szInfo+=TEXT("桔子x2");
	if (cbArea==5) szInfo+=TEXT("苹果x2");

	TCHAR szDesc[32]=TEXT("");
	mysprintf(szDesc,CountArray(szDesc),TEXT("执行次数：%d。"),cbTimes);
	szInfo+=szDesc;
	CopyMemory(pText,szInfo.GetBuffer(),szInfo.GetLength());
	return;
}
