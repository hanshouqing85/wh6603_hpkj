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

//��ʼ��
BOOL CAdminClientDlg::OnInitDialog()
{
	SetWindowText(TEXT("ʤ���������"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƾ�����"));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("������ƣ�"));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵����"));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT("������Ӯ���Ʊ���Ϸ�����Ʋ������ȣ�"));
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƾ�����"));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("������ƣ�"));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵����"));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT("������Ӯ���Ʊ���Ϸ�����Ʋ������ȣ�"));
	SetDlgItemText(IDC_STATIC_INFO,TEXT(""));
	SetDlgItemText(IDC_RADIO_0,TEXT("BAR"));
	SetDlgItemText(IDC_RADIO_1,TEXT("˫��"));
	SetDlgItemText(IDC_RADIO_2,TEXT("����"));
	SetDlgItemText(IDC_RADIO_3,TEXT("����"));
	SetDlgItemText(IDC_RADIO_4,TEXT("����"));
	SetDlgItemText(IDC_RADIO_5,TEXT("ľ��"));
	SetDlgItemText(IDC_RADIO_6,TEXT("����"));
	SetDlgItemText(IDC_RADIO_7,TEXT("ƻ��"));
	SetDlgItemText(IDC_RADIO_10,TEXT("BARx25"));
	SetDlgItemText(IDC_RADIO_11,TEXT("˫��x2"));
	SetDlgItemText(IDC_RADIO_12,TEXT("����x2"));
	SetDlgItemText(IDC_RADIO_13,TEXT("����x2"));
	SetDlgItemText(IDC_RADIO_14,TEXT("����x2"));
	SetDlgItemText(IDC_RADIO_15,TEXT("ľ��x2"));
	SetDlgItemText(IDC_RADIO_16,TEXT("����x2"));
	SetDlgItemText(IDC_RADIO_17,TEXT("ƻ��x2"));
	SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("������Ϣ"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));;
	SetDlgItemText(IDC_STATIC_NOTIC2,TEXT("������ע��"));
	ResetUserBet();
	return TRUE; 
}

//������ɫ
HBRUSH CAdminClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_STATIC_INFO) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	} 
	return hbr;
}

//ȡ������
void CAdminClientDlg::OnBnClickedButtonReset()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication,sizeof(ControlApplication));
	ControlApplication.cbControlAppType=C_CA_CANCELS;
	g_pGameClientDlg->OnAdminControl((WPARAM)&ControlApplication,0);
}

//������Ϣ
void CAdminClientDlg::OnBnClickedButtonSyn()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication,sizeof(ControlApplication));
	ControlApplication.cbControlAppType=C_CA_UPDATE;
	g_pGameClientDlg->OnAdminControl((WPARAM)&ControlApplication,0);
}

//��������
void CAdminClientDlg::OnBnClickedButtonOk()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication,sizeof(ControlApplication));

	BYTE cbSelectedIndex=0xff;//��������
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

	int nSelectTimes=((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();//���ƴ���
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
		SetDlgItemText(IDC_STATIC_INFO,TEXT("��ѡ���ܿش����Լ��ܿ�����"));
	}
}

//ȡ���ر�
void CAdminClientDlg::OnBnClickedButtonCancel()
{
	ShowWindow(SW_HIDE);
}

//���¿���
void __cdecl CAdminClientDlg::UpdateControl( CMD_S_ControlReturns* pControlReturns )
{
	switch(pControlReturns->cbReturnsType)
	{
	case S_CR_FAILURE:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("����ʧ�ܣ�"));
			break;
		}
	case S_CR_UPDATE_SUCCES:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			mysprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_SET_SUCCESS:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			mysprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_CANCEL_SUCCESS:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("ȡ�����óɹ���"));
			break;
		}
	}
}

void CAdminClientDlg::ResetUserBet()
{
	memset(m_lAllUserBet,0,sizeof(m_lAllUserBet));
	SetDlgItemText(IDC_STATIC_0,TEXT(" BAR��-----"));
	SetDlgItemText(IDC_STATIC_1,TEXT("˫�ߣ�-----"));
	SetDlgItemText(IDC_STATIC_2,TEXT("���ǣ�-----"));
	SetDlgItemText(IDC_STATIC_3,TEXT("���ϣ�-----"));
	SetDlgItemText(IDC_STATIC_4,TEXT("������-----"));
	SetDlgItemText(IDC_STATIC_5,TEXT("ľ�ϣ�-----"));
	SetDlgItemText(IDC_STATIC_6,TEXT("���ӣ�-----"));
	SetDlgItemText(IDC_STATIC_7,TEXT("ƻ����-----"));
}

void CAdminClientDlg::SetUserBetScore(BYTE cbArea, LONGLONG lScore)
{
	m_lAllUserBet[cbArea]+=lScore;
	CString strPrint;
	switch(cbArea)
	{
	case 0: 
		strPrint.Format(TEXT(" BAR��%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_0,strPrint);
		break;
	case 1: 
		strPrint.Format(TEXT("˫�ߣ�%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_1,strPrint);
		break;
	case 2: 
		strPrint.Format(TEXT("���ǣ�%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_2,strPrint);
		break;
	case 3: 
		strPrint.Format(TEXT("���ϣ�%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_3,strPrint);
		break;
	case 4: 
		strPrint.Format(TEXT("������%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_4,strPrint);
		break;
	case 5: 
		strPrint.Format(TEXT("ľ�ϣ�%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_5,strPrint);
		break;
	case 6: 
		strPrint.Format(TEXT("���ӣ�%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_6,strPrint);
		break;
	case 7: 
		strPrint.Format(TEXT("ƻ����%I64d"),m_lAllUserBet[cbArea]);
		SetDlgItemText(IDC_STATIC_7,strPrint);
		break;
	}
}

//��Ϣ
void CAdminClientDlg::PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes)
{
	if (cbArea==0xff)
	{
		mysprintf(pText,cbCount,TEXT("��ʱ�޿��ơ�"));
		return;
	}

	CString szInfo=TEXT("ʤ������");
	if (cbArea==3) szInfo+=TEXT(" BAR");
	if (cbArea==15) szInfo+=TEXT("˫��");
	if (cbArea==19) szInfo+=TEXT("����");
	if (cbArea==7) szInfo+=TEXT("����");
	if (cbArea==1) szInfo+=TEXT("����");
	if (cbArea==6) szInfo+=TEXT("ľ��");
	if (cbArea==12) szInfo+=TEXT("����");
	if (cbArea==10) szInfo+=TEXT("ƻ��");

	if (cbArea==2) szInfo+=TEXT("BARx25");
	if (cbArea==14) szInfo+=TEXT("˫��x2");
	if (cbArea==20) szInfo+=TEXT("����x2");
	if (cbArea==8) szInfo+=TEXT("����x2");
	if (cbArea==23) szInfo+=TEXT("����x2");
	if (cbArea==17) szInfo+=TEXT("ľ��x2");
	if (cbArea==11) szInfo+=TEXT("����x2");
	if (cbArea==5) szInfo+=TEXT("ƻ��x2");

	TCHAR szDesc[32]=TEXT("");
	mysprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��"),cbTimes);
	szInfo+=szDesc;
	CopyMemory(pText,szInfo.GetBuffer(),szInfo.GetLength());
	return;
}
