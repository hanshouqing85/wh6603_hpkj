#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "TableViewFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//��ť��С
#define ENTER_GAME_CX				142									//��ť���
#define ENTER_GAME_CY				52									//��ť�߶�

//��ť��С
#define CLOSE_SERVER_CX				142									//��ť���
#define CLOSE_SERVER_CY				52									//��ť�߶�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTableViewFrame, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTableView::CTableView()
{
	//����ӿ�
	m_pITableViewFrame=NULL;

	//״̬����
	ZeroMemory(&m_TableAttribute,sizeof(m_TableAttribute));

	return;
}

//��������
CTableView::~CTableView()
{
}

//�ӿڲ�ѯ
VOID * CTableView::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableView,Guid,dwQueryVer);
	return NULL;
}

//��������
WORD CTableView::GetNullChairCount(WORD & wNullChairID)
{
	//���ñ���
	wNullChairID=INVALID_CHAIR;

	//Ѱ��λ��
	WORD wNullCount=0;
	for (WORD i=0;i<m_TableAttribute.wChairCount;i++)
	{
		if (m_TableAttribute.pIClientUserItem[i]==NULL)
		{
			//������Ŀ
			wNullCount++;

			//���ý��
			if (wNullChairID==INVALID_CHAIR) wNullChairID=i;
		}
	}

	return wNullCount;
}

//���ú���
VOID CTableView::InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableViewFrame)
{
	//��������
	m_TableAttribute.wTableID=wTableID;
	m_TableAttribute.wChairCount=wChairCount;

	//���ýӿ�
	m_pITableViewFrame=QUERY_OBJECT_PTR_INTERFACE(pITableViewFrame,ITableViewFrame);

	return;
}

//��ȡ�û�
IClientUserItem * CTableView::GetClientUserItem(WORD wChairID)
{
	//Ч�����
	ASSERT(wChairID<m_TableAttribute.wChairCount);
	if (wChairID>=m_TableAttribute.wChairCount) return NULL;

	//��ȡ�û�
	return m_TableAttribute.pIClientUserItem[wChairID];
}

//������Ϣ
bool CTableView::SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem)
{
	//Ч�����
	ASSERT(wChairID<m_TableAttribute.wChairCount);
	if (wChairID>=m_TableAttribute.wChairCount) return false;

	//�����û�
	m_TableAttribute.pIClientUserItem[wChairID]=pIClientUserItem;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTableViewFrame::CTableViewFrame()
{
	//���Ա���
	m_wTableCount=0;
	m_wChairCount=0;

	//��ť����
	m_cbButtonDown=0;
	m_cbButtonHover=0;
	m_bHovering=false;

	//����ָ��
	m_pITableViewFrameSink=NULL;

	return;
}

//��������
CTableViewFrame::~CTableViewFrame()
{
	//ɾ������
	for (INT_PTR i=0;i<m_TableViewArray.GetCount();i++)
	{
		CTableView * pTableView=m_TableViewArray[i];
		if (pTableView!=NULL) SafeDelete(pTableView);
	}

	//ɾ������
	m_TableViewArray.RemoveAll();

	return;
}

//�ӿڲ�ѯ
VOID * CTableViewFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableViewFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableViewFrame,Guid,dwQueryVer);
	return NULL;
}

//���ú���
bool CTableViewFrame::ConfigTableFrame(WORD wTableCount, WORD wChairCount)
{
	//Ч�����
	ASSERT(wChairCount<=MAX_CHAIR);

	//���ñ���
	m_wTableCount=wTableCount;
	m_wChairCount=wChairCount;
	m_TableViewArray.SetSize(m_wTableCount);
	ZeroMemory(m_TableViewArray.GetData(),m_wTableCount*sizeof(CTableView *));

	//��������
	for (WORD i=0;i<m_wTableCount;i++)
	{
		m_TableViewArray[i]=new CTableView;
		m_TableViewArray[i]->InitTableView(i,wChairCount,this);
	}

	return true;
}

//��������
bool CTableViewFrame::CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx)
{
	//���ñ���
	m_cbButtonDown=0;
	m_cbButtonHover=0;
	m_bHovering=false;

	//���ýӿ�
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink)!=NULL);
	m_pITableViewFrameSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink);

	//��������
	CRect rcCreate(0,0,0,0);
	Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,pParentWnd,uWndID);

	return true;
}

//��ȡ�û�
IClientUserItem * CTableViewFrame::GetClientUserItem(WORD wTableID, WORD wChairID)
{
	//��ȡ����
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//��ȡ�û�
	if (pITableView!=NULL)
	{
		return pITableView->GetClientUserItem(wChairID);
	}

	return NULL;
}

//������Ϣ
bool CTableViewFrame::SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem)
{
	ITableView * pITableView=GetTableViewItem(wTableID);
	if (pITableView!=NULL) pITableView->SetClientUserItem(wChairID,pIClientUserItem);
	return true;
}

//��ȡ����
ITableView * CTableViewFrame::GetTableViewItem(WORD wTableID)
{
	//��ȡ����
	if (wTableID!=INVALID_TABLE)
	{
		//Ч�����
		ASSERT(wTableID<m_TableViewArray.GetCount());
		if (wTableID>=m_TableViewArray.GetCount()) return NULL;

		//��ȡ����
		ITableView * pITableView=m_TableViewArray[wTableID];

		return pITableView;
	}

	return NULL;
}

//��������
WORD CTableViewFrame::GetNullChairCount(WORD wTableID, WORD & wNullChairID)
{
	//��ȡ����
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//��ȡ״̬
	if (pITableView!=NULL)
	{
		return pITableView->GetNullChairCount(wNullChairID);
	}

	return 0;
}

//��ť����
BYTE CTableViewFrame::GetHoverButton(CPoint MousePoint)
{
	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����λ��
	CRect rcEnterGame;
	rcEnterGame.top=400;
	rcEnterGame.bottom=400+ENTER_GAME_CY;
	rcEnterGame.right=rcClient.Width()/2-20;
	rcEnterGame.left=rcClient.Width()/2-20-ENTER_GAME_CX;
	if (rcEnterGame.PtInRect(MousePoint)==TRUE) return BT_ENTER_GAME;

	//�ر�λ��
	CRect rcCloseServer;
	rcCloseServer.top=400;
	rcCloseServer.bottom=400+CLOSE_SERVER_CY;
	rcCloseServer.left=rcClient.Width()/2+20;
	rcCloseServer.right=rcClient.Width()/2+20+CLOSE_SERVER_CX;
	if (rcCloseServer.PtInRect(MousePoint)==TRUE) return BT_CLOSE_SERVER;

	return 0;
}

//�ػ���Ϣ
VOID CTableViewFrame::OnPaint()
{
// 	//���� DC
// 	CPaintDC dc(this);
// 
// 	//��ȡ����
// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 
// 	//��������
// 	CBitImage ImageBuffer;
// 	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);
// 
// 	//�����豸
// 	CImageDC BufferDC(ImageBuffer);
// 	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//������Դ
	//CBitImage TableLogo;
// 	CBitImage TableGround;
//	TableLogo.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_LOGO);
//	TableGround.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_GROUND);

	//�滭����
//	TableGround.DrawImageTile(pBufferDC,0,0,rcClient.Width(),rcClient.Height());
	//TableLogo.BitBlt(BufferDC,(rcClient.Width()-TableLogo.GetWidth())/2,0,SRCCOPY);

// 	//������Դ
// 	CBitImage ImageEnterGame;
// 	CBitImage ImageCloseServer;
// 	ImageEnterGame.LoadFromResource(AfxGetInstanceHandle(),IDB_BT_ENTER_GAME);
// 	ImageCloseServer.LoadFromResource(AfxGetInstanceHandle(),IDB_BT_CLOSE_SERVER);
// 
// 	//��ȡ��С
// 	CSize SizeEnterGame;
// 	CSize SizeCloseServer;
// 	SizeEnterGame.SetSize(ImageEnterGame.GetWidth()/5L,ImageEnterGame.GetHeight());
// 	SizeCloseServer.SetSize(ImageCloseServer.GetWidth()/5L,ImageCloseServer.GetHeight());
// 
// 	//�رշ���
// 	INT nXCloseIndex=0;
// 	bool bCloseButtonDown=(m_cbButtonDown==BT_CLOSE_SERVER);
// 	bool bCloseButtonHover=(m_cbButtonHover==BT_CLOSE_SERVER);
// 
// 	//�رշ���
// 	if ((bCloseButtonHover==true)&&(bCloseButtonDown==true)) nXCloseIndex=1;
// 	if ((bCloseButtonHover==true)&&(bCloseButtonDown==false)) nXCloseIndex=2;
// 	ImageCloseServer.BitBlt(BufferDC,rcClient.Width()/2+20,400,SizeCloseServer.cx,SizeCloseServer.cy,nXCloseIndex*SizeCloseServer.cx,0);
// 
// 	//������Ϸ
// 	INT nXEnterIndex=0;
// 	bool bEnterButtonDown=(m_cbButtonDown==BT_ENTER_GAME);
// 	bool bEnterButtonHover=(m_cbButtonHover==BT_ENTER_GAME);
// 
// 	//�鿴����
// 	if ((bEnterButtonHover==true)&&(bEnterButtonDown==true)) nXEnterIndex=1;
// 	if ((bEnterButtonHover==true)&&(bEnterButtonDown==false)) nXEnterIndex=2;
// 	ImageEnterGame.BitBlt(BufferDC,rcClient.Width()/2-SizeEnterGame.cx-20,400,SizeEnterGame.cx,SizeEnterGame.cy,nXEnterIndex*SizeEnterGame.cx,0);
// 
	//�滭����
//	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return;
}

//�����Ϣ
VOID CTableViewFrame::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//λ�ü���
	BYTE cbButtonHover=GetHoverButton(Point);

	//���ݵ���
	if ((m_cbButtonDown!=0)&&(cbButtonHover!=m_cbButtonDown))
	{
		cbButtonHover=0;
	}

	//��������
	if (m_cbButtonHover!=cbButtonHover)
	{
		//���ñ���
		m_cbButtonHover=cbButtonHover;

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

//�����Ϣ
VOID CTableViewFrame::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//�������
	BYTE cbButtonDown=0;
	WORD wViewItemDown=INVALID_WORD;

	//���ñ���
	if (m_cbButtonDown==m_cbButtonHover)
	{
		cbButtonDown=m_cbButtonDown;
	}

	//������
	if (m_cbButtonDown!=0)
	{
		//�ͷ����
		ReleaseCapture();

		//���ñ���
		m_cbButtonDown=0;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}

	//ִ�ж���
	if (cbButtonDown!=0)
	{
		ASSERT(m_pITableViewFrameSink!=NULL);
		if (m_pITableViewFrameSink!=NULL) m_pITableViewFrameSink->OnHitTableButton(cbButtonDown);
	}

	return;
}

//�����Ϣ
VOID CTableViewFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//���ý���
	SetFocus();

	//���´���
	if (m_cbButtonHover!=0)
	{
		//����˻�
		SetCapture();

		//���ñ���
		m_cbButtonDown=m_cbButtonHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}

	return;
}

//�����Ϣ
LRESULT CTableViewFrame::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	m_bHovering=false;

	//���½���
	if (m_cbButtonHover!=0)
	{
		//���ñ���
		m_cbButtonDown=0;
		m_cbButtonHover=0;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
