#include "StdAfx.h"
#include "Resource.h"
#include "ApplyUserList.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CApplyUser, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_LBTUP,OnLBtUp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CApplyUser::CApplyUser() : CDialog(IDD_DLG_GAME_RECORD)
{
	   m_viewHandle = NULL;
}
LRESULT CApplyUser::OnLBtUp(WPARAM wParam,LPARAM lParam)
{
	::SendMessage(m_viewHandle,WM_VIEWLBTUP,wParam,lParam);
	return 1;
	      
}
//��������
CApplyUser::~CApplyUser()
{
}
void CApplyUser::MySetRect(CRect aRect)
{
	//m_OrtRect=aRect;
	//m_AppyUserList.m_SkinVerticleScrollbar.set


}
//�ؼ���
void CApplyUser::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECORD_LIST, m_AppyUserList);
}

//��ʼ������
BOOL CApplyUser::OnInitDialog()
{
	__super::OnInitDialog();

	

	//���ñ���
	TCHAR szBuffer[_MAX_PATH];     
	::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH);
	CString   sPath;   
	sPath.Format(_T("res://%s/#2/#193"),szBuffer);

	LVBKIMAGE   bki;   
	bki.ulFlags   =   LVBKIF_STYLE_TILE   |   LVBKIF_SOURCE_URL   ;   
	bki.pszImage   =   sPath.GetBuffer(sPath.GetLength());   
	bki.cchImageMax   =   sPath.GetLength();   
	//BOOL bSuccess =m_AppyUserList.SetBkImage(   &bki); 

	SCROLLINFO info;

	info.cbSize = 20;
	info.fMask = 3;
	info.nMax = 200;
	info.nMin  = 4;
	info.nPage = 1;
	info.nPos = 0;
	info.nTrackPos = 8;

	

	CWnd* pParent = GetParent();
	m_OrtRect.top  = 	m_OrtRect.top+10;
	m_AppyUserList.Init(m_OrtRect,pParent);




	m_AppyUserList.SetScrollInfo(SB_VERT,&info);


	m_AppyUserList.m_viewHandle  = m_hWnd;
	//m_AppyUserList.SendMessage(WM_VSCROLL,MAKEWPARAM(SB_BOTTOM,   0),   0);
	m_AppyUserList.SetTextBkColor(-1);
	m_AppyUserList.SetTextColor(RGB(52,116,23));
	m_AppyUserList.SetRowHeigt(15);     
	m_AppyUserList.SetHeaderHeight(0);
	m_AppyUserList.SetFontHW(13,0);



//	m_AppyUserList.SetScrollInfo(SB_VERT,&info);


	m_AppyUserList.m_viewHandle  = m_hWnd;
	//m_AppyUserList.SendMessage(WM_VSCROLL,MAKEWPARAM(SB_BOTTOM,   0),   0);
	m_AppyUserList.SetTextBkColor(-1);
	m_AppyUserList.SetTextColor(RGB(232, 245, 3));

	//�����б�
	m_AppyUserList.InsertColumn(0,TEXT("�������"),LVCFMT_LEFT,125);
	m_AppyUserList.InsertColumn(1,TEXT("��Ϸ��"),LVCFMT_LEFT,180);

	

 /*        tagApplyUser ApplyUser;
	ApplyUser.strUserName = "�������";
	ApplyUser.lUserScore = 1000000000;

	InserUser(ApplyUser);
	ApplyUser.strUserName = "�������";
	InserUser(ApplyUser);
		ApplyUser.strUserName = "�������";
	InserUser(ApplyUser);
	InserUser(ApplyUser);
		ApplyUser.strUserName = "3434";
	InserUser(ApplyUser);
	InserUser(ApplyUser);
			ApplyUser.strUserName = "3434";
		InserUser(ApplyUser);
				ApplyUser.strUserName = "hgdfgdfg";
	InserUser(ApplyUser);
			ApplyUser.strUserName = "454546";
	InserUser(ApplyUser);
			ApplyUser.strUserName = "3434";
     InserUser(ApplyUser);
	InserUser(ApplyUser);
	InserUser(ApplyUser);
			ApplyUser.strUserName = "3434";
	/*	InserUser(ApplyUser);
	InserUser(ApplyUser);
	InserUser(ApplyUser);
			ApplyUser.strUserName = "3434";
	InserUser(ApplyUser);*/

	return FALSE;
}

int  CApplyUser::GetItemCount()
{
	return m_AppyUserList.GetItemCount();
}
//�����б�
void CApplyUser::InserUser(tagApplyUser & ApplyUser)
{
	//��������
	WORD wListIndex=0;
	WORD wColumnCount=1;
	TCHAR szBuffer[128]=TEXT("");

	//��Ϸ���
	INT nItemIndex=m_AppyUserList.InsertItem(m_AppyUserList.GetItemCount(),ApplyUser.strUserName);

	double dUserScore = 0;
	dUserScore = ApplyUser.lUserScore;
	//��ҽ��
	//myprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),ApplyUser.lUserScore);
	myprintf(szBuffer,CountArray(szBuffer),DrawNumberStringWithDot(dUserScore));
	m_AppyUserList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	m_AppyUserList.PostMessage(WM_ERASEBKGND,0,0);


	

	/*CString   str;   
	CSize     sz;   
	char  temp[100];
	int           dx=0;   
	CDC*         pDC   =   m_AppyUserList.GetDC();   
	for   (int   i=0;i   <   m_AppyUserList.GetItemCount();i++)   
	{   
		m_AppyUserList.GetItemText(  1, i,   (LPTSTR)temp ,100  );   
		sz   =   pDC->GetTextExtent(str);   

		if   (sz.cx   >   dx)   
			dx   =   sz.cx;   
	} 
	SCROLLINFO info;
	m_AppyUserList.ReleaseDC(pDC);   */
	
	return;
}

//λ�ñ仯
void CApplyUser::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�б�ؼ�
	if (m_AppyUserList.m_hWnd!=NULL)
	{
		m_AppyUserList.MoveWindow(0,0,cx,cy);
	}

	return;
}

//�������
bool CApplyUser::FindUser(LPCTSTR lpszUserName)
{
	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags=LVFI_STRING;
	lvFindInfo.psz=lpszUserName;

	//��������
	int nItem=m_AppyUserList.FindItem(&lvFindInfo);

	//���ؽ��
	return -1==nItem ? false : true;
}


//ɾ���б�
void CApplyUser::DeleteUser(tagApplyUser & ApplyUser)
{
	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_STRING;
	lvFindInfo.psz = (LPCTSTR)ApplyUser.strUserName;

	//��������
	int nItem = m_AppyUserList.FindItem( &lvFindInfo );

	//ɾ������
	if ( nItem != -1 ) m_AppyUserList.DeleteItem( nItem );
	m_AppyUserList.PostMessage(WM_ERASEBKGND,0,0);
}
CString CApplyUser::DrawNumberStringWithDot(DOUBLE lScore )
{
	CString strScore;
	CString strReturn;
	DOUBLE lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%0.2f"), lNumber);

	CString strDecimal;
	strDecimal = strScore.Right(3);
	strScore.Delete( strScore.Find('.'), 3);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%3) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}

	if ( lScore < 0 )
		strReturn.Insert(0, '-');

	//if ( bPlus && lScore > 0 )
	//	strReturn.Insert(0, '+');

	strReturn += strDecimal;


	return strReturn;

}

//�����б�
void CApplyUser::UpdateUser( tagApplyUser & ApplyUser )
{
	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_STRING;
	lvFindInfo.psz = (LPCTSTR)ApplyUser.strUserName;

	//��������
	int nItem = m_AppyUserList.FindItem( &lvFindInfo );

	//ɾ������
	if ( nItem != -1 ) 
	{
		TCHAR szBuffer[128]=TEXT("");

		//��ҽ��
		myprintf(szBuffer,CountArray(szBuffer),DrawNumberStringWithDot(ApplyUser.lUserScore));
		
		m_AppyUserList.SetItem(nItem,1,LVIF_TEXT,szBuffer,0,0,0,0);
	}
	m_AppyUserList.PostMessage(WM_ERASEBKGND,0,0);	
}

//����б�
void CApplyUser::ClearAll()
{
	m_AppyUserList.DeleteAllItems();
}
//////////////////////////////////////////////////////////////////////////
