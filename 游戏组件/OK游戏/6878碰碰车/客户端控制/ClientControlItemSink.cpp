// ClientControlItemSink.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ClientControlItemSink.h"
#include ".\clientcontrolitemsink.h"


// CClientControlItemSinkDlg �Ի���

IMPLEMENT_DYNAMIC(CClientControlItemSinkDlg, IClientControlDlg)

CClientControlItemSinkDlg::CClientControlItemSinkDlg(CWnd* pParent /*=NULL*/)
	: IClientControlDlg(CClientControlItemSinkDlg::IDD, pParent)
{
}

CClientControlItemSinkDlg::~CClientControlItemSinkDlg()
{
}

void CClientControlItemSinkDlg::DoDataExchange(CDataExchange* pDX)
{
	IClientControlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientControlItemSinkDlg, IClientControlDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SYN, OnBnClickedButtonSyn)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BT_GET, OnBnClickedBtGet)
	ON_BN_CLICKED(IDC_BT_CHEAK, OnBnClickedBtCheak)
END_MESSAGE_MAP()

// CClientControlItemSinkDlg ��Ϣ�������

//��ʼ��
BOOL CClientControlItemSinkDlg::OnInitDialog()
{
	IClientControlDlg::OnInitDialog();

	//������Ϣ
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
	SetDlgItemText(IDC_RADIO_BEN_40,TEXT("��ʱ�ݡ�40"));
	SetDlgItemText(IDC_RADIO_BAO_30,TEXT("�����30"));
	SetDlgItemText(IDC_RADIO_AO_20,TEXT("���ۡ�20"));
	SetDlgItemText(IDC_RADIO_DA_10,TEXT("���ڡ�10"));
	SetDlgItemText(IDC_RADIO_BEN_5,TEXT("��ʱ�ݡ�5"));
	SetDlgItemText(IDC_RADIO_BAO_5,TEXT("�����5"));
	SetDlgItemText(IDC_RADIO_AO_5,TEXT("���ۡ�5"));
	SetDlgItemText(IDC_RADIO_DA_5,TEXT("���ڡ�5"));
	SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("������Ϣ"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));;

	return TRUE; 
}

//������ɫ
HBRUSH CClientControlItemSinkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = IClientControlDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_STATIC_INFO) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	} 
	return hbr;
}

//ȡ������
void CClientControlItemSinkDlg::OnBnClickedButtonReset()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_CANCELS;
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//���ֿ���
void CClientControlItemSinkDlg::OnBnClickedButtonSyn()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_UPDATE;
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//��������
void CClientControlItemSinkDlg::OnBnClickedButtonOk()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));

	BYTE cbSelectedIndex = 0xff;
	if(((CButton*)GetDlgItem(IDC_RADIO_BEN_40))->GetCheck())
		cbSelectedIndex = 0;
	else if(((CButton*)GetDlgItem(IDC_RADIO_BAO_30))->GetCheck())
		cbSelectedIndex = 1;
	else if(((CButton*)GetDlgItem(IDC_RADIO_AO_20))->GetCheck())
		cbSelectedIndex = 2;
	else if(((CButton*)GetDlgItem(IDC_RADIO_DA_10))->GetCheck())
		cbSelectedIndex = 3;
	else if(((CButton*)GetDlgItem(IDC_RADIO_BEN_5))->GetCheck())
		cbSelectedIndex = 4;
	else if(((CButton*)GetDlgItem(IDC_RADIO_BAO_5))->GetCheck())
		cbSelectedIndex = 5;
	else if(((CButton*)GetDlgItem(IDC_RADIO_AO_5))->GetCheck())
		cbSelectedIndex = 6;
	else if(((CButton*)GetDlgItem(IDC_RADIO_DA_5))->GetCheck())
		cbSelectedIndex = 7;

	int nSelectTimes = ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();

	if ( cbSelectedIndex != 0xff && nSelectTimes >= 0 && nSelectTimes != 0xffffffff)
	{
		ControlApplication.cbControlAppType = C_CA_SET;
		ControlApplication.cbControlArea = cbSelectedIndex;
		ControlApplication.cbControlTimes = static_cast<BYTE>(nSelectTimes + 1);
		CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
		if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_INFO,TEXT("��ѡ���ܿش����Լ��ܿ�����"));
	}
}

//ȡ���ر�
void CClientControlItemSinkDlg::OnBnClickedButtonCancel()
{
	ShowWindow(SW_HIDE);
}

//���¿���
void __cdecl CClientControlItemSinkDlg::UpdateControl( CMD_S_ControlReturns* pControlReturns )
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
			_sntprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_SET_SUCCESS:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			_sntprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
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

//��Ϣ
void CClientControlItemSinkDlg::PrintingInfo( TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes )
{
	if (cbArea == 0xff )
	{
		_sntprintf(pText,cbCount,TEXT("��ʱ�޿��ơ�"));
		return;
	}
	TCHAR szDesc[32] = TEXT("");
	_sntprintf(pText,cbCount,TEXT("ʤ������"));
	if ( cbArea == 0 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��ʱ�ݡ�40��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 1 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("�����30��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 2 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ۡ�20��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 3 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ڡ�10��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 4 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��ʱ�ݡ�5��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 5 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("�����5��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 6 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ۡ�5��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 7 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ڡ�5��") );
		lstrcat( pText,szDesc );
	}
	_sntprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��"), cbTimes);
	lstrcat( pText,szDesc );
}

void CClientControlItemSinkDlg::OnBnClickedBtGet()
{
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->PostMessage(IDM_GET_ACCOUNT,0,0);
}

void CClientControlItemSinkDlg::OnBnClickedBtCheak()
{
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->PostMessage(IDM_CHEAK_ACCOUNT,0,0);
}


//���ý���
void __cdecl CClientControlItemSinkDlg::ResetUserBet()
{
	memset(m_lAllUserBet,0,sizeof(SCORE)*AREA_COUNT);

	SetDlgItemText(IDC_ST_AREA1,TEXT("��ʱ�ݡ�40:-----"));
	SetDlgItemText(IDC_ST_AREA2,TEXT("�����30:-----"));
	SetDlgItemText(IDC_ST_AREA3,TEXT("���ۡ�20:-----"));
	SetDlgItemText(IDC_ST_AREA4,TEXT("���ڡ�10:-----"));
	SetDlgItemText(IDC_ST_AREA5,TEXT("��ʱ�ݡ�5:-----"));
	SetDlgItemText(IDC_ST_AREA6,TEXT("�����5:-----"));
	SetDlgItemText(IDC_ST_AREA7,TEXT("���ۡ�5:-----"));
	SetDlgItemText(IDC_ST_AREA8,TEXT("���ڡ�5:-----"));

	SetDlgItemText(IDC_ST_AREA9, TEXT("��ʱ�ݡ�40:-----"));
	SetDlgItemText(IDC_ST_AREA10,TEXT("�����30:-----"));
	SetDlgItemText(IDC_ST_AREA11,TEXT("���ۡ�20:-----"));
	SetDlgItemText(IDC_ST_AREA12,TEXT("���ڡ�10:-----"));
	SetDlgItemText(IDC_ST_AREA13,TEXT("��ʱ�ݡ�5:-----"));
	SetDlgItemText(IDC_ST_AREA14,TEXT("�����5:-----"));
	SetDlgItemText(IDC_ST_AREA15,TEXT("���ۡ�5:-----"));
	SetDlgItemText(IDC_ST_AREA16,TEXT("���ڡ�5:-----"));
}

//����ǳ�
void __cdecl CClientControlItemSinkDlg::ResetUserNickName()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->ResetContent();
}

//��ȡ�ǳ�
void __cdecl CClientControlItemSinkDlg::GetCheckNickName(CString &strNickName)
{
	((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->GetWindowText(strNickName); 
}

//����ǳ�
void __cdecl CClientControlItemSinkDlg::SetUserNickName(LPCTSTR lpszNickName)
{
	((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->AddString(lpszNickName);
	((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->SetCurSel(0);
}

//��ע��Ϣ
void __cdecl CClientControlItemSinkDlg::SetUserBetScore(BYTE cbArea,SCORE lScore)
{
	CString strPrint;
	if(cbArea>0)cbArea=cbArea-1;

	TRACE("cbArea=%d\n",cbArea);

	switch(cbArea)
	{
	case 0: strPrint.Format(TEXT("��ʱ�ݡ�40:%.2lf"),lScore);break;
	case 1: strPrint.Format(TEXT("�����30:%.2lf"),lScore);break;
	case 2: strPrint.Format(TEXT("���ۡ�20:%.2lf"),lScore);break;
	case 3: strPrint.Format(TEXT("���ڡ�10:%.2lf"),lScore);break;
	case 4: strPrint.Format(TEXT("��ʱ�ݡ�5:%.2lf"),lScore);break;
	case 5: strPrint.Format(TEXT("�����5:%.2lf"),lScore);break;
	case 6: strPrint.Format(TEXT("���ۡ�5:%.2lf"),lScore);break;
	case 7: strPrint.Format(TEXT("���ڡ�5:%.2lf"),lScore);break;

	default:false;
	}
	SetDlgItemText(IDC_ST_AREA9+cbArea,strPrint);
}
//��ע��Ϣ
void __cdecl CClientControlItemSinkDlg::SetAllUserBetScore(BYTE cbArea,SCORE lScore)
{
	TRACE("AllUserBet[cbArea=%d Score=%I64d]\n",cbArea,lScore);

	m_lAllUserBet[cbArea]+=lScore;
	CString strPrint;
	switch(cbArea)
	{
	case 0: strPrint.Format(TEXT("��ʱ�ݡ�40:%.2lf"),m_lAllUserBet[cbArea]);break;
	case 1: strPrint.Format(TEXT("�����30:%.2lf"),m_lAllUserBet[cbArea]);break;
	case 2: strPrint.Format(TEXT("���ۡ�20:%.2lf"),m_lAllUserBet[cbArea]);break;
	case 3: strPrint.Format(TEXT("���ڡ�10:%.2lf"),m_lAllUserBet[cbArea]);break;
	case 4: strPrint.Format(TEXT("��ʱ�ݡ�5:%.2lf"),m_lAllUserBet[cbArea]);break;
	case 5: strPrint.Format(TEXT("�����5:%.2lf"),m_lAllUserBet[cbArea]);break;
	case 6: strPrint.Format(TEXT("���ۡ�5:%.2lf"),m_lAllUserBet[cbArea]);break;
	case 7: strPrint.Format(TEXT("���ڡ�5:%.2lf"),m_lAllUserBet[cbArea]);break;

	default:false;
	}
	SetDlgItemText(IDC_ST_AREA1+cbArea,strPrint);
}
