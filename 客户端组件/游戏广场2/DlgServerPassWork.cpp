#include "stdafx.h"
#include "DlgServerPassWork.h"
//////////////////////////
////EWIN�����д
////2013.4.16
////���ܣ���֤��������
////����ת����ע������
///////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//��Ļλ��
#define LAYERED_SIZE				5									//�ֲ��С
#define CAPTION_SIZE				60									//�����С

//////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgServerPassWork, CDialog)

	//ϵͳ��Ϣ
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

CDlgServerPassWork::CDlgServerPassWork(CWnd* pParent /*=NULL*/): CDialog(CDlgServerPassWork::IDD, pParent)
{
	m_StrPassWork=TEXT("");
}

CDlgServerPassWork::~CDlgServerPassWork()
{
	m_StrPassWork=TEXT("");
}

//���ú���
BOOL CDlgServerPassWork::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�����뷿������"));

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_SERVERPASSWORK_BACK"));
	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);


	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btOk.SetButtonImage(IDB_BT_SERVER_OK,hInstance,false,false);
	m_btCancel.SetButtonImage(IDB_BT_SERVER_CANCEL,hInstance,false,false);

	//���������ı���
	m_ServerPassWork.SetEnableColor(RGB(255,255,0),RGB(11,60,96),RGB(11,60,96));


	//���д���
	CenterWindow(this);

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return FALSE;
}

void CDlgServerPassWork::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//��ť�ؼ�
	DDX_Control(pDX, IDC_OK, m_btOk);
	DDX_Control(pDX, IDC_CANCEL, m_btCancel);
	//��������ؼ�
	DDX_Control(pDX, IDC_SERVER_PASSWORK, m_ServerPassWork);

}


//�滭����
BOOL CDlgServerPassWork::OnEraseBkgnd(CDC * pDC)
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

	//���û���
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_SERVERPASSWORK_BACK"));
	//�滭����
	ImageBack.DrawImage(pBufferDC,0,0);

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//��ʾ��Ϣ
VOID CDlgServerPassWork::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//��ʾ�ֲ�
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//�����Ϣ
VOID CDlgServerPassWork::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�����
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//λ�øı�
VOID CDlgServerPassWork::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//�ƶ��ֲ�
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}


//���÷�������
void CDlgServerPassWork::SetPassWork(CString StrPassWork)
{
	m_StrPassWork=StrPassWork;
	//AfxMessageBox(m_StrPassWork);
	return;
}

//ȷ������
void CDlgServerPassWork::OnBnClickedOk()
{

	//��ȡ����
	CString strPassWork;
	GetDlgItemText(IDC_SERVER_PASSWORK,strPassWork);

	

	//���������뷿��������ȫ��ͬ
	if(strPassWork==m_StrPassWork){
		
		EndDialog(99);
	
	}else{
	
		//��ʾ�����������
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("������ķ������벻��ȷ�����������룡"),MB_ICONERROR,0);
		m_ServerPassWork.SetFocus();
		//�������
		//EndDialog(0);
	
	}
	return;
}

//ȡ����Ť
void CDlgServerPassWork::OnBnClickedCancel()
{
	//�رմ���
	EndDialog(0);
	return;
}
//////////////////////////////////////////////////////////////////////////////////