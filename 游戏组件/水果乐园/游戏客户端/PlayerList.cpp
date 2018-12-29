#include "StdAfx.h"
#include "PlayerList.h"
#include "Resource.h"

#define TIMER_UPDATE 100

BEGIN_MESSAGE_MAP(CPlayerList,CListCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

//构造函数
CPlayerList::CPlayerList()
{
	ZeroMemory(m_PlayerRecordArray,sizeof(m_PlayerRecordArray));
	m_iRecordCount=0;
}

//析构函数
CPlayerList::~CPlayerList()
{
}

//插入
void CPlayerList::InserUser(tagPlayerRecord & Record)
{
	if (m_iRecordCount<GAME_PLAYER)
	{
		m_PlayerRecordArray[m_iRecordCount].dwUserID=Record.dwUserID;
		m_PlayerRecordArray[m_iRecordCount].lUserScore=Record.lUserScore;
		lstrcpyn(m_PlayerRecordArray[m_iRecordCount].szUserName,Record.szUserName,sizeof(m_PlayerRecordArray[m_iRecordCount].szUserName));

		int iIndex=InsertItem(GetItemCount(),m_PlayerRecordArray[m_iRecordCount].szUserName);
		SetItemText(iIndex,1,FormatNumber(m_PlayerRecordArray[m_iRecordCount].lUserScore));

		m_iRecordCount++;
		ASSERT(m_iRecordCount==GetItemCount());
	}
	return;
}

//删除
void CPlayerList::DeleteUser(DWORD dwUserID)
{
	for (int i=0; i<m_iRecordCount; i++)
	{
		if (m_PlayerRecordArray[i].dwUserID==dwUserID)
		{
			int iIndex=FindUser(m_PlayerRecordArray[i]);
			DeleteItem(iIndex);

			for (int j=i+1; j<m_iRecordCount; j++)
			{
				m_PlayerRecordArray[j-1]=m_PlayerRecordArray[j];
			}
			m_iRecordCount--;

			ASSERT(m_iRecordCount==GetItemCount());
			break;
		}
	}
}

//更新
void CPlayerList::UpdateUser(tagPlayerRecord & Record)
{
	for (int i=0; i<m_iRecordCount; i++)
	{
		if (m_PlayerRecordArray[i].dwUserID==Record.dwUserID)
		{
			m_PlayerRecordArray[i].lUserScore=Record.lUserScore;

			int iIndex=FindUser(m_PlayerRecordArray[i]);
			SetItemText(iIndex,1,FormatNumber(m_PlayerRecordArray[i].lUserScore));
			break;
		}
	}
}

//清空
void CPlayerList::ClearAll()
{
	ZeroMemory(&m_PlayerRecordArray,sizeof(m_PlayerRecordArray));
	m_iRecordCount=0;
	DeleteAllItems();
}

//格式化
CString CPlayerList::FormatNumber(__int64 lScore)
{
	//ASSERT(lScore>=0);
	CString strNumber=TEXT(""),strTmpNumber1,strTmpNumber2;
	//if (lScore==0) strNumber=TEXT("0");
	if (lScore<=0) strNumber=TEXT("0");
	int nNumberCount=0;
	//LONGLONG lTmpNumber=lScore;
	//if (lScore<0) lScore=-lScore;
	while (lScore>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lScore%10);
		nNumberCount++;
		strTmpNumber2=strTmpNumber1+strTmpNumber2;
		if (nNumberCount==3)
		{
			if (strNumber.GetLength()>1)
				strTmpNumber2+=(TEXT(", ")+strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lScore/=10;
	}
	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if (strNumber.GetLength()>1)
			strTmpNumber2+=(TEXT(", ")+strNumber);
		strNumber=strTmpNumber2;
	}
	//if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;
	return strNumber;
}

//查找
int CPlayerList::FindUser(tagPlayerRecord & Record)
{
	LVFINDINFO Info;
	Info.flags=LVFI_PARTIAL|LVFI_STRING;
	Info.psz=Record.szUserName;
	int iIndex=FindItem(&Info);//找到返回序号，未找到返回-1
	ASSERT(iIndex!=-1);
	return iIndex;
}

int CPlayerList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	//////////
	CBitmap bmp;
	bmp.LoadBitmap(IDB_SCROLL_BAR);
	BITMAP bm;
	bmp.GetBitmap(&bm);
	m_hBmpScrollBar=(HBITMAP)bmp.Detach();

	m_ctrlScroll.Create(WS_CHILD|WS_VISIBLE|SBS_VERT,CRect(0,0,0,0),this,1000);
	m_ctrlScroll.SetBitmap(m_hBmpScrollBar);
	m_ctrlScroll.SetScrollRange(1,100,TRUE);
	RECT rc1;
	rc1.left=100+70;
	rc1.top=0;
	rc1.right=rc1.left+14;
	rc1.bottom=rc1.top+92;
	m_ctrlScroll.MoveWindow(&rc1);
	m_ctrlScroll.SetWindowPos(this,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	m_ctrlScroll.ShowWindow(SW_SHOW);

	SetTimer(TIMER_UPDATE,500,NULL);
	//////////
	return 0;
}

void CPlayerList::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
//static int iTemp=0;
//iTemp++;
//printf(__FUNCTION__"[%d]\n",iTemp);
	ModifyStyle(WS_VSCROLL,0);
	CListCtrl::OnNcCalcSize(bCalcValidRects,lpncsp);
}

BOOL CPlayerList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//printf("zDelta=%d\n",zDelta);
//	int nPos=GetScrollPos(SB_VERT);
////printf("nPos=%d\n",nPos);
//	if (zDelta==120) nPos--;
//	if (zDelta==-120) nPos++;
//	SetScrollPos(SB_VERT,nPos);
//
//	//确保滚动条的实时更新
//	SCROLLINFO si={0};
//	si.cbSize=sizeof(si);
//	si.fMask=SIF_ALL;
//	GetScrollInfo(SB_VERT,&si);
//	m_ctrlScroll.SetScrollInfo(&si);
	//return 0;
	return CListCtrl::OnMouseWheel(nFlags,zDelta,pt);
}

void CPlayerList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (nSBCode==SB_THUMBTRACK || nSBCode==SB_THUMBPOSITION)
	{
	}
	else
	{
		CListCtrl::OnVScroll(nSBCode,nPos,pScrollBar);
	}
	//SCROLLINFO si={0};
	//si.cbSize=sizeof(si);
	//si.fMask=SIF_ALL;
	//GetScrollInfo(SB_VERT,&si);
	//m_ctrlScroll.SetScrollInfo(&si);
	//////////
	//SCROLLINFO si;
	//si.fMask=SIF_ALL;
	//m_ctrlScroll.GetScrollInfo(&si,SIF_ALL);
	//SIZE size={0,0};
	//switch(nSBCode)
	//{
	//case SB_LINEUP:
	//	si.nPos--;
	//	break;
	//case SB_LINEDOWN:
	//	si.nPos++;
	//	break;
	//case SB_PAGEUP:
	//	si.nPos-=si.nPage;
	//	break;
	//case SB_PAGEDOWN:
	//	si.nPos+=si.nPage;
	//	break;
	//case SB_THUMBTRACK:
	//	si.nPos=nPos;
	//	break;
	//}
	//if(si.nPos>(int)(si.nMax-si.nMin-si.nPage+1)) si.nPos=si.nMax-si.nMin-si.nPage+1;
	//if(si.nPos<si.nMin) si.nPos=si.nMin;
	//si.fMask=SIF_POS;
	//m_ctrlScroll.SetScrollInfo(&si);
	//SetScrollInfo(SB_VERT,&si);
	//////////
	//CWnd *pWnd=GetWindow(GW_CHILD);
	////m_bOp=TRUE;
	//pWnd->SendMessage(WM_VSCROLL,MAKELONG(nSBCode,nPos),0);
	////m_bOp=FALSE;
	//if(nSBCode==SB_THUMBTRACK) return;
	//SCROLLINFO si={0};
	//si.cbSize=sizeof(si);
	//si.fMask=SIF_ALL;
	//pWnd->GetScrollInfo(SB_VERT,&si);
	//pScrollBar->SetScrollInfo(&si,TRUE);
}

void CPlayerList::OnTimer(UINT nIDEvent)
{
	//CWnd *pWnd=GetWindow(GW_CHILD);
	SCROLLINFO si1={0};
	si1.cbSize=sizeof(SCROLLINFO);
	si1.fMask=SIF_ALL;
	SCROLLINFO si2={0};
	if(GetWindowLong(m_ctrlScroll.m_hWnd,GWL_STYLE)&WS_VISIBLE && !m_ctrlScroll.m_bDrag)
	{
		//pWnd->GetScrollInfo(SB_VERT,&si1);
		GetScrollInfo(SB_VERT,&si1);
		m_ctrlScroll.GetScrollInfo(&si2,SIF_ALL);
		if(si1.nMax!=si2.nMax||si1.nMin!=si2.nMin||si1.nPos!=si2.nPos||si1.nPage!=si2.nPage)
			m_ctrlScroll.SetScrollInfo(&si1);
	}
}

LRESULT CPlayerList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	//if (message!=0x0363) printf(__FUNCTION__"[%08x]\n",message);
	if (message==WM_ERASEBKGND)
	{
		SCROLLINFO si={0};
		si.cbSize=sizeof(si);
		si.fMask=SIF_ALL;
		GetScrollInfo(SB_VERT,&si);
		m_ctrlScroll.SetScrollInfo(&si);
	}
	//if (message==WM_NCCALCSIZE)
	//{
	//	UpdateWindow();
	//	ShowScrollBar(SB_VERT,FALSE);
	//	//CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
	//}
	if (message==WM_VSCROLL)
	{
		SCROLLINFO si={0};
		si.cbSize=sizeof(si);
		si.fMask=SIF_ALL;
		GetScrollInfo(SB_VERT,&si);
		m_ctrlScroll.SetScrollInfo(&si);
		WORD sbCode=LOWORD(wParam);
		if (sbCode==SB_THUMBTRACK || sbCode==SB_THUMBPOSITION)
		{
			SCROLLINFO siv={0};
			siv.cbSize=sizeof(SCROLLINFO);
			siv.fMask=SIF_ALL;
			int nPos=HIWORD(wParam);
			CRect rcClient;
			SIZE sizeAll;
			GetClientRect(&rcClient);
			GetScrollInfo(SB_VERT,&siv);
			if(siv.nPage==0)
				sizeAll.cy=rcClient.bottom;
			else
				sizeAll.cy=rcClient.bottom*(siv.nMax+1)/siv.nPage;

			SIZE size={0,0};
			//size.cx=sizeAll.cx*sih.nPos/(sih.nMax+1);
			size.cy=sizeAll.cy*(nPos-siv.nPos)/(siv.nMax+1);
			Scroll(size);
			return 1;
		}
		else
		{
			CListCtrl::WindowProc(message,wParam,lParam);
		}
	}
	return CListCtrl::WindowProc(message,wParam,lParam);
}
