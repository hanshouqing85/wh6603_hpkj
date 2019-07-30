#include "StdAfx.h"
#include "Resource.h"
#include "PlazaViewItem.h"
#include "GlobalUnits.h"
#include "WndGameTypeCtrl.h"
#include "MessageDlg.h"
//////////////////////////////////////////////////////////////////////////////////

//��������
#define MIN_TYPE_ID					1									//��С����
#define MAX_TYPE_ID					6									//�������

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndGameTypeCtrl, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////
HHOOK g_hMouse = NULL;
HWND g_hWnd=NULL;
CWndGameTypeCtrl   *ptr   =   0;//ȫ�ֱ���
CRect rc;
CRect rcMenu;
WORD m_wMenuTypeID = INVALID_WORD;
LRESULT CALLBACK MouseProc(
						   int nCode,      // hook code
						   WPARAM wParam,  // message identifier
						   LPARAM lParam   // mouse coordinates
						   )
{
	if (wParam==WM_MOUSEMOVE)
	{
		POINT pt;
		::GetCursorPos(&pt);


		if (!rc.PtInRect(pt)&&!rcMenu.PtInRect(pt))
		{
			rcMenu.SetRectEmpty();
			SendMessage(ptr->m_hWnd,WM_CANCELMODE,0,0);
			m_wMenuTypeID = INVALID_WORD;
			UnhookWindowsHookEx(g_hMouse);
		}
	}

	return CallNextHookEx(g_hMouse,nCode,wParam,lParam);
}


//���캯��
CWndGameTypeCtrl::CWndGameTypeCtrl()
{
	//���Ϣ
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	m_bShowType = false;

	//������Դ
	//ImageItemBack.LoadFromResource(AfxGetInstanceHandle(),IDB_ITEM_BACK);
	ImageGameTypeItem.LoadImage(AfxGetInstanceHandle(),TEXT("GAME_TYPE_ITEM"));


	m_SizeButton.SetSize(ImageGameTypeItem.GetWidth()/5,ImageGameTypeItem.GetHeight()/6);
	return;
}

//��������
CWndGameTypeCtrl::~CWndGameTypeCtrl()
{


}

//��ȡ��С
VOID CWndGameTypeCtrl::GetControlSize(CSize & ResultSize)
{
	//���ñ���
	ResultSize.cy=m_SizeButton.cy;
	ResultSize.cx=750;

	return;
}
VOID CWndGameTypeCtrl::ShowMenu(bool bShowMenu,WORD wShowType)
{
	m_bShowType = bShowMenu;
	m_wItemHover = wShowType-1;
	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);


}
VOID CWndGameTypeCtrl::SetClicked(WORD wClickedType)
{
	for (int i = 0;i < m_GameTypeItemArray.GetCount();i++)
	{
		//��ȡ����
		ASSERT(m_GameTypeItemArray[i]!=NULL);
		CGameTypeItem * pGameTypeTemp=m_GameTypeItemArray[i];
		if(pGameTypeTemp->m_GameType.wTypeID == wClickedType)
		{
			m_wItemActive=i;
			//���½���
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
	}
	return;
}
//��������
VOID CWndGameTypeCtrl::InsertGameType(CGameTypeItem * pGameTypeItem)
{
	//Ч�����
	ASSERT(pGameTypeItem!=NULL);
	ASSERT(pGameTypeItem->m_GameType.wTypeID>=MIN_TYPE_ID);
	ASSERT(pGameTypeItem->m_GameType.wTypeID<=MAX_TYPE_ID);

	//Ч�����
	if (pGameTypeItem==NULL) return;
	if (pGameTypeItem->m_GameType.wTypeID<MIN_TYPE_ID) return;
	if (pGameTypeItem->m_GameType.wTypeID>MAX_TYPE_ID) return;

	//��������
	INT i=0;
	for ( i=0;i<m_GameTypeItemArray.GetCount();i++)
	{
		//��ȡ����
		ASSERT(m_GameTypeItemArray[i]!=NULL);
		CGameTypeItem * pGameTypeTemp=m_GameTypeItemArray[i];

		//�����ж�
		if (pGameTypeItem->m_GameType.wSortID<=pGameTypeTemp->m_GameType.wSortID)
		{
			m_GameTypeItemArray.InsertAt(i,pGameTypeItem);
			break;
		}
	}

	//Ĭ�ϲ���
	if (i==m_GameTypeItemArray.GetCount())
	{
		m_GameTypeItemArray.Add(pGameTypeItem);
	}

	//��������
	if (m_wItemActive==INVALID_WORD)
	{
		m_wItemActive=0;
	}

	//�����ؼ�
	if (m_hWnd!=NULL)
	{
		CSize SizeControl;
		GetControlSize(SizeControl);
		SetWindowPos(NULL,0,0,SizeControl.cx,SizeControl.cy,SWP_NOZORDER|SWP_NOMOVE);
	}

	return;
}

//��������
WORD CWndGameTypeCtrl::GetTypeItemIndex(WORD wTypeID)
{
	//��������
	for (INT_PTR i=0;i<m_GameTypeItemArray.GetCount();i++)
	{
		//��ȡ����
		ASSERT(m_GameTypeItemArray[i]!=NULL);
		CGameTypeItem * pGameTypeItem=m_GameTypeItemArray[i];

		//��ʶ�ж�
		if (pGameTypeItem->m_GameType.wTypeID==wTypeID)
		{
			return (WORD)i;
		}
	}

	return INVALID_WORD;
}

//��������
WORD CWndGameTypeCtrl::GetTypeItemIndex(CPoint MousePoint)
{
	//��������
	CRect rcClient(0,0,0,0);
	GetClientRect(rcClient);

	//��������
	WORD wItemIndex = INVALID_WORD;
	WORD wItemCount = (WORD)m_GameTypeItemArray.GetCount();
	INT nXPitch = (m_GameTypeItemArray.GetCount()>1)?((rcClient.Width()-(wItemCount)*m_SizeButton.cx)/wItemCount):0;

	//��������
	if((MousePoint.x%(m_SizeButton.cx+nXPitch)<=m_SizeButton.cx))
	{
		wItemIndex=(WORD)(MousePoint.x/(m_SizeButton.cx+nXPitch));
		if (wItemIndex>=wItemCount+1) wItemIndex=INVALID_WORD;
	}

	return wItemIndex;
}

//�滭����
BOOL CWndGameTypeCtrl::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//�����豸
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	pBufferDC->SetBkMode(TRANSPARENT);

	if(m_GameTypeItemArray.GetCount()>0)
	{
		INT nItemCount = (INT)m_GameTypeItemArray.GetCount();
		INT nXPitch = (nItemCount>1)?((rcClient.Width()-(nItemCount+1)*m_SizeButton.cx)/(nItemCount)):0;

		//�滭����
		for (INT i=0;i<nItemCount+1;i++)
		{
			//��������
			ASSERT((i==0)||(m_GameTypeItemArray[i-1]!=NULL));
			WORD wTypeID=(i==0)?0:m_GameTypeItemArray[i-1]->m_GameType.wTypeID;

			//λ�ñ���
			INT nXImagePos=0;
			INT nYImagePos=(wTypeID+1-MIN_TYPE_ID)*m_SizeButton.cy;

			//����λ��
			if (m_wItemActive==i)
			{
				if (m_wItemHover!=i) nXImagePos=m_SizeButton.cx*3L;
				if (m_wItemHover==i) nXImagePos=m_SizeButton.cx*4L;
			}
			else
			{
				if ((m_wItemHover==i)&&(m_wItemDown!=i)) nXImagePos=m_SizeButton.cx;
				if ((m_wItemHover==i)&&(m_wItemDown==i)) nXImagePos=m_SizeButton.cx*2L;
			}

			//�滭����
		//	ImageItemBack.StretchBlt(BufferDC,i*(m_SizeButton.cx+nXPitch),0,m_SizeButton.cx+nXPitch,m_SizeButton.cy,0,0,m_SizeButton.cx,m_SizeButton.cy,SRCCOPY);
			ImageGameTypeItem.DrawImage(pBufferDC,i*(m_SizeButton.cx),0,m_SizeButton.cx,m_SizeButton.cy,nXImagePos,nYImagePos);
		}
	}

// 	if(m_bShowType)
// 	{
// 		//��ȡλ��
// 		CRect rcClient;
// 		GetClientRect(&rcClient);
// 
// 
// 
// 		//��������
// 		WORD wItemIndex = INVALID_WORD;
// 		WORD wItemCount = (WORD)m_GameTypeItemArray.GetCount();
// 
// 		INT nXPitch = (wItemCount>1)?((rcClient.Width()-(wItemCount)*m_SizeButton.cx)/wItemCount):0;
// 		m_ImageItemBackTop.DrawImage(pBufferDC,nXPitch+m_wItemHover*m_SizeButton.cx,rcClient.top+3);
// 	}


	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}
//��Ϣ����
BOOL CWndGameTypeCtrl::PreTranslateMessage(MSG * pMsg)
{
// 	//��������
// 	if ((pMsg->message>=WM_MOUSEFIRST)&&(pMsg->wParam<=WM_MOUSELAST))
// 	{
// 	//	this->SetFocus();
// 
// 		if(m_wItemHover!=INVALID_WORD)
// 		{
// 			//this->SetFocus();
// 		}
// 		return TRUE;
// 	}
// 
	return __super::PreTranslateMessage(pMsg);
}

//�����Ϣ
VOID CWndGameTypeCtrl::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//λ�ü���
	WORD wItemHover=GetTypeItemIndex(Point);

	//��������
	if (wItemHover!=m_wItemHover)
	{
		//���ñ���
		m_wItemHover=wItemHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

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
	return;
}

BOOL CWndGameTypeCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	if(m_wMenuTypeID == INVALID_WORD)
		return FALSE;
	SetClicked(m_wMenuTypeID);
	int nTemp = nCommandID - 123;
	WORD wKinID = m_GameKindInfoArray[nTemp]->pGameKindItem->m_GameKind.wKindID;

	//��ȡ����
	ASSERT(CPlazaViewItem::GetInstance()!=NULL);
	CPlazaViewItem * pPlazaViewItem=CPlazaViewItem::GetInstance();

	pPlazaViewItem->ShowWindow(SW_SHOW);
	pPlazaViewItem->OnButtonEnterKind(wKinID);
	for(int i = 0;i < m_GameKindInfoArray.GetCount();i++)
	{
		m_GameKindInfoArray.RemoveAt(i);

	}

	return __super::OnCommand(wParam,lParam);
}
//�����Ϣ
VOID CWndGameTypeCtrl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//��������
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover==m_wItemDown))
	{
		//���ñ���
		m_wItemActive=m_wItemDown;

		//��ȡ����
		ASSERT(CPlazaViewItem::GetInstance()!=NULL);
		CPlazaViewItem * pPlazaViewItem=CPlazaViewItem::GetInstance();

		if (m_wItemActive>0)
		{
			CGameTypeItem * pGameTypeItem=m_GameTypeItemArray[m_wItemActive];
			pPlazaViewItem->ShowKindItemView(pGameTypeItem->m_GameType.wTypeID);			
		}
		else
		{
			pPlazaViewItem->ShowKindItemView(0);
		}
	}

	//������
	if (m_wItemDown!=INVALID_WORD)
	{
		//�ͷ����
		ReleaseCapture();

		//���ñ���
		m_wItemDown=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
VOID CWndGameTypeCtrl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//���ý���
	SetFocus();

	//���´���
	if (m_wItemHover!=INVALID_WORD)
	{
		//����˻�
		SetCapture();

		//���ñ���
		m_wItemDown=m_wItemHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
LRESULT CWndGameTypeCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	m_bHovering=false;

	//���½���
	if (m_wItemHover!=INVALID_WORD)
	{
		//���ñ���
		m_wItemHover=INVALID_WORD;

	}


	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	return 0;
}




//////////////////////////////////////////////////////////////////////////////////
