#include "Stdafx.h"
#include "GamePlaza.h"

#include "PlatformFrame.h"
#include "DlgLockMachine.h"
#include "WndUserInfoCtrl.h"
#include "ZhuanhuanDlg.h"
//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_USER_INFO_1				100									//�û���ť
#define IDC_USER_INFO_2				101									//�û���ť
#define IDC_USER_INFO_3				102									//�û���ť
#define IDC_BTN_REFRESH				103									//ˢ�°�ť
#define IDC_USER_INFO_4				104									//�û���ť
#define IDC_BTN_CHANGE				105									//ת����ť

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndUserInfoCtrl, CWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()

	//�Զ���Ϣ
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

	

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWndUserInfoCtrl::CWndUserInfoCtrl()
{
	m_bHitUserInfo1 = true;
	m_bHitUserInfo2 = false;
	m_bHitUserInfo3 = false;
	m_bHitUserInfo4 = false;
	m_dwTickCount = 0;
	//��������
	m_pDlgInsurePlaza = NULL;
	m_fUserScore=0.0f;
	m_cbScoreType=1;
	m_Face = NULL;
	theAccount.Scoretype=MoShi_Yuan;
}

//��������
CWndUserInfoCtrl::~CWndUserInfoCtrl()
{
	if (AfxIsValidAddress(m_Face, sizeof(m_Face)) && m_Face != NULL)
	{
	//	delete m_Face;
		m_Face = NULL;
	}

}
void CWndUserInfoCtrl::ReleaseFace()
{
	if (m_Face != NULL)
	{
//		delete m_Face;
		m_Face = NULL;
	}

}
void CWndUserInfoCtrl::SetClicked(BOOL bShow, UINT nStatus)
{

	OnCommand(100,0);

}
//�����
BOOL CWndUserInfoCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_USER_INFO_1:		//��������
		{
			theAccount.Scoretype=MoShi_Yuan;
			m_cbScoreType=1;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
 			m_bHitUserInfo1 = true;
 			m_bHitUserInfo2 = false;
 			m_bHitUserInfo3 = false;
			m_bHitUserInfo4 = false;


			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_FRESH_GAME,0,0);
			}

						
			return TRUE;
		}
	case IDC_USER_INFO_2:		//������Ϣ
		{
			theAccount.Scoretype=MoShi_Jiao;
			m_cbScoreType=2;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

 			m_bHitUserInfo1 = false;
 			m_bHitUserInfo2 = true;
 			m_bHitUserInfo3 = false;
			m_bHitUserInfo4 = false;


			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_FRESH_GAME,0,0);
			}

			break;
		}
	case IDC_USER_INFO_3:		//��ʷ��¼
		{
			theAccount.Scoretype=MoShi_Fen;
			m_cbScoreType=3;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
 			m_bHitUserInfo1 = false;
 			m_bHitUserInfo2 = false;
 			m_bHitUserInfo3 = true;
			m_bHitUserInfo4 = false;


			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_FRESH_GAME,0,0);
			}

			return TRUE;
		}
	case IDC_USER_INFO_4:		//��ʷ��¼
		{
			theAccount.Scoretype=MoShi_Li;
			m_cbScoreType=4;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
			m_bHitUserInfo1 = false;
			m_bHitUserInfo2 = false;
			m_bHitUserInfo3 = false;
			m_bHitUserInfo4 = true;


			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_FRESH_GAME,0,0);
			}

			return TRUE;
		}
	case IDC_BTN_REFRESH:
		{
			DWORD dwTickCount = GetTickCount();
			if ((dwTickCount-m_dwTickCount)<3000)
			{
				return TRUE;
			}
			m_dwTickCount = dwTickCount;

			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_REFRESH_YUE,0,0);
			}
			return TRUE;
		}
	case IDC_BTN_CHANGE:
		{
			//�û���Ϣ
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			CZhuanhuanDlg Zhuanhuan;
			Zhuanhuan.m_fJine = theAccount.yue;
			Zhuanhuan.m_fQpJine = pGlobalUserData->lScore;
			Zhuanhuan.m_nType = 1;
			if(Zhuanhuan.DoModal() == IDOK)
			{
				DOUBLE fJine = 0.0;
				int nType = 0;
				fJine = Zhuanhuan.m_fJiaoYiJine;
				nType = Zhuanhuan.m_nType;

				CWnd* pParent = GetParent();
				if(pParent!=NULL)
				{
					if(nType == 1)
					{
						pParent->PostMessage(IDM_ZHUANHUAN_TO_CAIPIAO,nType,fJine*1000);
					}
					else if(nType == 2)
					{
						pParent->PostMessage(IDM_ZHUANHUAN_TO_QIPAI,nType,fJine*1000);

					}
				}

			}

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//λ����Ϣ
VOID CWndUserInfoCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//������Ϣ
INT CWndUserInfoCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	m_Font.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("΢���ź�"));

	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//������Դ
	CPngImage ImageTitleM;
	ImageTitleM.LoadImage(AfxGetInstanceHandle(),TEXT("USER_INFO_MALE"));
	if(pGlobalUserData!=NULL)
	{
		if(pGlobalUserData->cbGender == 1)
			ImageTitleM.LoadImage(AfxGetInstanceHandle(),TEXT("USER_INFO_FEMALE"));
	}

	//������ť
	CRect rcCreate(0,0,0,0);

	m_btnFlush.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_REFRESH);
	m_btnChange.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_CHANGE);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();

// 	m_btnFlush.SetButtonImage(IDB_BK_FRESH,TEXT("BT_USER_FRESH"),hInstance,true,false);
// 	m_btnChange.SetButtonImage(IDB_BT_CHANGE,TEXT("BT_USER_CHANGE"),hInstance,true,false);
	m_btnFlush.SetButtonImage(IDB_BK_FRESH,hInstance,true,false);
	m_btnChange.SetButtonImage(IDB_BT_CHANGE,hInstance,true,false);

	//�ƶ�����
	CRect rcUserInfo;
	GetWindowRect(&rcUserInfo);
	SetWindowPos(NULL,0,0,rcUserInfo.Width(),ImageTitleM.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//�����ؼ�
VOID CWndUserInfoCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	//��������
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER|SWP_NOSIZE;


	DeferWindowPos(hDwp,m_btnFlush,NULL,183,125,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnChange,NULL,62,185,0,0,uFlags);
	//��������
	EndDeferWindowPos(hDwp);

	return;
}


//ת���ַ�
VOID CWndUserInfoCtrl::SwitchScoreFormat(SCORE lScore, UINT uSpace,UINT uDecimalCount,LPTSTR pszBuffer, WORD wBufferSize)
{
	//ת����ֵ
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%0.3f"),lScore);

	//��������
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore)-uDecimalCount-1;

	//ת���ַ�
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//�ַ��Ƚ�
		if(szSwitchScore[wSourceIndex]=='.') break;

		//�����ַ�
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

	}

	//����С��
	for(BYTE i=0;i<uDecimalCount+1;i++)
		pszBuffer[wTargetIndex++] = szSwitchScore[wSourceIndex++];

	//�����ַ�
	pszBuffer[wTargetIndex++]=0;

	return;
}

//�滭����
BOOL CWndUserInfoCtrl::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//���� DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//��������
	HINSTANCE hInstance=AfxGetInstanceHandle();
	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("USER_INFO_MALE"));
	ImageBack.DrawImage(pBufferDC,0,0);
	if(pGlobalUserData!=NULL)
	{
		Graphics graphics(pBufferDC->m_hDC);

		if(m_Face == NULL)
		{
			if(theAccount.cbgender == 1)
				m_Face = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\female.png"));
			else if (theAccount.cbgender == 0)
			{
				m_Face = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\male.png"));
			}

		}

		CRect rcRect;
		rcRect.SetRect(0,0, m_Face->GetWidth(),  m_Face->GetHeight());
		graphics.DrawImage(m_Face, Rect(11,11, m_Face->GetWidth(),  m_Face->GetHeight()), 0, 0, m_Face->GetWidth(),  m_Face->GetHeight(), UnitPixel);
	}

	//���û���
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextColor(RGB(244,248,161));
	pBufferDC->SelectObject(m_Font);


	//�滭�û�
	if (pGlobalUserData->dwUserID!=0L)
	{
		m_fUserScore = theAccount.dongjie;
		//������Ϣ
		TCHAR szScore[32]=TEXT("");
		TCHAR szInsure[32]=TEXT("");
		TCHAR szGameScore[32]=TEXT("");
		_sntprintf(szInsure,CountArray(szInsure),TEXT("%ld"),pGlobalUserData->dwUserID);
		SwitchScoreFormat(pGlobalUserData->lScore,3,2,szScore,CountArray(szGameScore));
		_tcscat(szScore ,TEXT("���"));

		m_fUserScore = theAccount.yue;
		SwitchScoreFormat(m_fUserScore,3,3,szGameScore,CountArray(szGameScore));
		_tcscat(szGameScore ,TEXT("Ԫ"));
		//����
		CRect rcDongjie;
		rcDongjie.SetRect(85,143,rcClient.Width()-18,189);
		pBufferDC->DrawText(szScore,lstrlen(szScore),&rcDongjie,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//���
		CRect rcYue;
		rcYue.SetRect(85,120,rcClient.Width()-18,160);
		pBufferDC->DrawText(szGameScore,lstrlen(szGameScore),&rcYue,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		pBufferDC->SetTextColor(RGB(255,255,255));

		//��Ϸ�� 
		CRect rcScore;
		rcYue.SetRect(85,93,rcClient.Width()-18,131);
		pBufferDC->DrawText(szInsure,lstrlen(szInsure),&rcYue,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//����ʺ�
		CRect rcAccounts;
		rcAccounts.SetRect(85,70,rcClient.Width()-18,103);
		pBufferDC->DrawText(pGlobalUserData->szAccounts,lstrlen(pGlobalUserData->szAccounts),&rcAccounts,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//�¼���Ϣ
LRESULT CWndUserInfoCtrl::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//��¼�ɹ�
		{
			//���´���
			Invalidate();


			return 0L;
		}
	case EVENT_USER_LOGOUT:			//ע���ɹ�
		{
			//���´���
			Invalidate();


			return 0L;
		}
	case EVENT_USER_INFO_UPDATE:	//���ϸ���
		{
			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			ASSERT(pGlobalUserData!=NULL);

			//��������
			if(m_pDlgInsurePlaza!=NULL) 
				m_pDlgInsurePlaza->OnEventUpdateInsurePlaza(pGlobalUserData->lScore,pGlobalUserData->lInsure);

			//���´���
			Invalidate();

			return 0L;
		}
	}

	return 0L;
}


//////////////////////////////////////////////////////////////////////////////////
