// AdminControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "gameclient.h"
#include "AdminControlDlg.h"


// CAdminControlDlg �Ի���

IMPLEMENT_DYNAMIC(CAdminControlDlg, CDialog)
CAdminControlDlg::CAdminControlDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAdminControlDlg::IDD, pParent)
{
	m_dwGameID=0;
}

CAdminControlDlg::~CAdminControlDlg()
{
}

void CAdminControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdminControlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnExcute)
	ON_BN_CLICKED(IDC_BUTTON_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BUTTON_SYN,OnRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL,OnCancel)
	ON_BN_CLICKED(IDC_QUERY_USER,OnQueryUser)
	ON_WM_TIMER()	
END_MESSAGE_MAP()


// CAdminControlDlg ��Ϣ�������
BOOL CAdminControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƴ���:"));
	SetDlgItemText(IDC_STATIC_ID,TEXT("���GameID:"));
	SetDlgItemText(IDC_RADIO_WIN,TEXT("Ӯ"));
	SetDlgItemText(IDC_RADIO_LOST,TEXT("��"));

	((CButton*)(GetDlgItem(IDC_RADIO_WIN)))->SetCheck(1);
	((CButton*)(GetDlgItem(IDC_RADIO_LOST)))->SetCheck(0);

	//���ƴ���
	for(int i=0;i<100;i++)
	{

		CString NumInt;
		NumInt.Format(TEXT("%d"),i);
		((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->InsertString(i,NumInt);
	
	}

	SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("��ѯ"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));
	SetDlgItemText(IDC_QUERY_USER,TEXT("��ȡ"));

	SetDlgItemText(IDC_STATIC,TEXT("ע:���������ұ���û����ע�򱾾���עû��ҲûӮ,��ô�þֽ������¼�ڱ������ڡ�"));


	return TRUE;
}

void CAdminControlDlg::OnReSet()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_OPTION_CANCLE;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}

void CAdminControlDlg::OnRefresh()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_OPTION_QUERYING;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}

void CAdminControlDlg::OnExcute()
{
	CMD_C_AdminReq adminReq={0};


	int nSelect=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();

	//�γ���������
	CString strCount;
	//IDC_GAMEID ��Ҫ�޸ĵ�,
	GetDlgItem(IDC_COMBO_GAMEID)->GetWindowText(strCount);
	_sntscanf(strCount.GetBuffer(),strCount.GetLength(),_TEXT("%d"),&m_dwGameID);
	


	if(strCount.IsEmpty()){


	int nSelect1=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->GetLBText(nSelect1,strCount);

	}



	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(wcscmp(strCount,m_szUserAccounts[i])==0)
		{
			m_dwGameID = m_wUserGameID[i];
		}
	}


	if(nSelect>=0&&nSelect<=100 && m_dwGameID!=0)
	{
		adminReq.cbReqType=RQ_OPTION_SETING;
		adminReq.dwGameID=m_dwGameID;
		adminReq.cbCheatCount=nSelect;

		if(((CButton*)(GetDlgItem(IDC_RADIO_WIN)))->GetCheck()!=0)
			adminReq.cbCheatType=CHEAT_TYPE_WIN;
		else
			adminReq.cbCheatType=CHEAT_TYPE_LOST;


		CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	}
	else
	{
		AfxMessageBox(TEXT("�����������Ĳ���!"));
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CAdminControlDlg::OnQueryUser()
{
	// TODO: �ڴ����ר�ô����/����û���
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_QUERYUSER,0,0);
}

void CAdminControlDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	ShowWindow(SW_HIDE);
	//	CDialog::OnCancel();
}

void CAdminControlDlg::OnTimer(UINT nIDEvent)
{

}



bool CAdminControlDlg::OnSubAdminCommand(const void * pBuffer, WORD wDataSize)
{

	////Ч������
	//ASSERT(wDataSize==sizeof(CMD_S_AdminStore));
	//if (wDataSize!=sizeof(CMD_S_AdminStore)) return false;

	////��Ϣ����
	//CMD_S_AdminStore * pAdminStore=(CMD_S_AdminStore *)pBuffer;

	//switch(pAdminStore->cbReqType)
	//{
	//case RQ_SERCHSTORE:
	//	{
	//		CString strStore;
	//		strStore.Format(TEXT("��ǰ�������%d"),pAdminStore->lStorageStart);

	//		SetDlgItemText(IDC_STATIC_STORE,strStore);
	//		break;
	//	}
	//case RQ_REVIEWSTORE:
	//	{
	//		CString strStore;
	//		strStore.Format(TEXT("��ǰ�������%d"),pAdminStore->lStorageStart);

	//		SetDlgItemText(IDC_STATIC_STORE,strStore);
	//		AfxMessageBox(TEXT("�޸ĳɹ�"));
	//		break;
	//	}
	//}
	return true;
}
void CAdminControlDlg::SetUserInfo(TCHAR m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS],DWORD m_wGameID[GAME_PLAYER])
{
	memset(m_wUserGameID,0,sizeof(m_wUserGameID));
	ZeroMemory(m_szUserAccounts,sizeof(m_szUserAccounts));

	((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->ResetContent();

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		lstrcpyn(m_szUserAccounts[i],m_szAccounts[i],CountArray(m_szAccounts[i]));
		m_wUserGameID[i] = m_wGameID[i];
	}

	int num = 0;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(m_wUserGameID[i]!=0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->InsertString(num,m_szUserAccounts[i]);
			num++;
		}
	}
}