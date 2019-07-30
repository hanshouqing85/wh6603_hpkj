#include "StdAfx.h"
#include "GameLogic.h"
#include "GameClient.h"
#include "ControlWnd.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_PENG					100									//���ư�ť
#define IDC_GANG					101									//���ư�ť
#define IDC_HEAR					102									//���ư�ť
#define IDC_CHI_HU					103									//���ư�ť
#define IDC_GIVEUP					104									//���ư�ť
#define IDC_LEFT_CHI				105									//���ư�ť
#define IDC_CENTER_CHI				106									//���ư�ť
#define IDC_RIGHT_CHI				107									//���ư�ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_PENG, OnBnClickedPeng)
	ON_BN_CLICKED(IDC_GANG, OnBnClickedGang)
	ON_BN_CLICKED(IDC_HEAR, OnBnClickedHear)
	ON_BN_CLICKED(IDC_CHI_HU, OnBnClickedChiHu)
	ON_BN_CLICKED(IDC_GIVEUP, OnBnClickedGiveUp)
	ON_BN_CLICKED(IDC_LEFT_CHI, OnBnClickedLeftChi)
	ON_BN_CLICKED(IDC_CENTER_CHI, OnBnClickedCenterChi)
	ON_BN_CLICKED(IDC_RIGHT_CHI, OnBnClickedRightChi)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CControlWnd::CControlWnd()
{
	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageControlBack.LoadFromResource(hResInstance,IDB_CONTROL_BACK);

	return;
}

//��������
CControlWnd::~CControlWnd()
{
}

//����״̬
VOID CControlWnd::SetControlInfo(BYTE cbAcitonMask)
{
	//���ư�ť
	m_btPeng.EnableWindow(((cbAcitonMask&WIK_PENG)!=0)?TRUE:FALSE);
	m_btGang.EnableWindow(((cbAcitonMask&WIK_GANG)!=0)?TRUE:FALSE);
	//m_btHear.EnableWindow(((cbAcitonMask&WIK_LISTEN)!=0)?TRUE:FALSE);
	m_btChiHu.EnableWindow(((cbAcitonMask&WIK_CHI_HU)!=0)?TRUE:FALSE);
	m_btLeftChi.EnableWindow(((cbAcitonMask&WIK_LEFT)!=0)?TRUE:FALSE);
	m_btCenterChi.EnableWindow(((cbAcitonMask&WIK_CENTER)!=0)?TRUE:FALSE);
	m_btRightChi.EnableWindow(((cbAcitonMask&WIK_RIGHT)!=0)?TRUE:FALSE);

	//���ƴ���
	ShowWindow((cbAcitonMask!=WIK_NULL)?SW_SHOW:SW_HIDE);

	return;
}

//�����ؼ�
VOID CControlWnd::RectifyControl(INT nWidth, INT nHeight)
{
	//��ȡλ��
	CRect rcButton;
	m_btPeng.GetWindowRect(&rcButton);

	//��������
	CSkinButton * ControlButton[]={&m_btLeftChi,&m_btCenterChi,&m_btRightChi,&m_btPeng,&m_btGang,/*&m_btHear,*/&m_btChiHu};

	//���ư�ť
	for (INT i=0;i<CountArray(ControlButton);i++)
	{
		INT nXPos=i*rcButton.Width()+16;
		ControlButton[i]->SetWindowPos(NULL,nXPos,11,0,0,SWP_NOZORDER|SWP_NOSIZE);
	}

	//������ť
	m_btGiveUp.GetWindowRect(&rcButton);
	m_btGiveUp.SetWindowPos(NULL,nWidth-rcButton.Width()-9,nHeight-rcButton.Height()-10,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedPeng()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_USER_ACTION,WIK_PENG,0);

	return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedGang()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_USER_ACTION,WIK_GANG,0);

	return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedHear()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_USER_ACTION,WIK_LISTEN,0);

	return;
}

//�Ժ���ť
VOID CControlWnd::OnBnClickedChiHu()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_USER_ACTION,WIK_CHI_HU,0);

	return;
}

//������ť
VOID CControlWnd::OnBnClickedGiveUp()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_USER_ACTION,WIK_NULL,0);

	return;
}

//��԰�ť
VOID CControlWnd::OnBnClickedLeftChi()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_USER_ACTION,WIK_LEFT,0);

	return;
}
//�г԰�ť
VOID CControlWnd::OnBnClickedCenterChi()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_USER_ACTION,WIK_CENTER,0);

	return;
}
//�ҳ԰�ť
VOID CControlWnd::OnBnClickedRightChi()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_USER_ACTION,WIK_RIGHT,0);

	return;
}
//�ػ�����
VOID CControlWnd::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	//CImageHandle HandleControlBack(&m_ImageControlBack);
	m_ImageControlBack.AlphaDrawImage(&dc,0,0,RGB(255,0,255));

	return;
}

//������Ϣ
INT CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btGiveUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_GIVEUP);
	m_btPeng.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_PENG);
	m_btGang.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_GANG);
	//m_btHear.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_HEAR);
	m_btChiHu.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHI_HU);
	m_btLeftChi.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_LEFT_CHI);
	m_btCenterChi.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CENTER_CHI);
	m_btRightChi.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_RIGHT_CHI);

	//����λͼ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btPeng.SetButtonImage(IDB_BT_PENG,hResInstance,false,false);
	m_btGang.SetButtonImage(IDB_BT_GANG,hResInstance,false,false);
	//m_btHear.SetButtonImage(IDB_BT_LISTEN,hResInstance,false,false);
	m_btChiHu.SetButtonImage(IDB_BT_CHIHU,hResInstance,false,false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP,hResInstance,false,false);
	m_btLeftChi.SetButtonImage(IDB_BT_LCHI,hResInstance,false,false);
	m_btCenterChi.SetButtonImage(IDB_BT_CCHI,hResInstance,false,false);
	m_btRightChi.SetButtonImage(IDB_BT_RCHI,hResInstance,false,false);

	//m_btLeftChi.SetMouseEventCallBack(this);
	//m_btCenterChi.SetMouseEventCallBack(this);
	//m_btRightChi.SetMouseEventCallBack(this);
	//m_btPeng.SetMouseEventCallBack(this);
	//m_btGang.SetMouseEventCallBack(this);

	//��������
	CRgn WindowRgn;
	//CImageHandle HandleControlBack(&m_ImageControlBack);
	m_ImageControlBack.CreateImageRegion(WindowRgn,RGB(255,0,255));

	//��������
	SetWindowRgn(WindowRgn,FALSE);
	SetWindowPos(NULL,0,0,m_ImageControlBack.GetWidth(),m_ImageControlBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	//�����ؼ�
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return 0;
}
HRESULT __cdecl CControlWnd:: OnEventMouseMove(UINT uControlID, WPARAM wParam, LPARAM lParam)
{
	if(uControlID==IDC_LEFT_CHI)
	{
		//������Ϣ
		if(m_btLeftChi.IsWindowEnabled()==FALSE) return 0;
		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_LEFT,0);
		return TRUE;
	}
	if(uControlID==IDC_CENTER_CHI)
	{
		//������Ϣ
		if(m_btCenterChi.IsWindowEnabled()==FALSE) return 0;
		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_CENTER,0);
		return TRUE;
	}
	if(uControlID==IDC_RIGHT_CHI)
	{
		//������Ϣ
		if(m_btRightChi.IsWindowEnabled()==FALSE) return 0;
		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_RIGHT,0);
		return TRUE;
	}

	if(uControlID==IDC_PENG)
	{
		//������Ϣ
		if(m_btPeng.IsWindowEnabled()==FALSE) return 0;
		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_PENG,0);
		return TRUE;
	}
	if(uControlID==IDC_GANG)
	{
		//������Ϣ
		if(m_btGang.IsWindowEnabled()==FALSE) return 0;
		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_GANG,0);
		return TRUE;
	}
	return TRUE;
}

HRESULT __cdecl CControlWnd::OnEventMouseLeft(UINT uControlID, WPARAM wParam, LPARAM lParam)
{
	if(uControlID==IDC_LEFT_CHI)
	{
		//������Ϣ
		if(m_btLeftChi.IsWindowEnabled()==FALSE) return 0;
		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_NULL,0);
		return TRUE;
	}
	if(uControlID==IDC_CENTER_CHI)
	{
		//������Ϣ
		if(m_btCenterChi.IsWindowEnabled()==FALSE) return 0;
		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_NULL,0);
		return TRUE;
	}
	if(uControlID==IDC_RIGHT_CHI)
	{
		//������Ϣ
		if(m_btRightChi.IsWindowEnabled()==FALSE) return 0;
		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_NULL,0);
		return TRUE;
	}

	if(uControlID==IDC_PENG)
	{
		//������Ϣ
		if(m_btPeng.IsWindowEnabled()==FALSE) return 0;

		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_NULL,0);
		return TRUE;
	}
	if(uControlID==IDC_GANG)
	{
		//������Ϣ
		if(m_btGang.IsWindowEnabled()==FALSE) return 0;

		AfxGetMainWnd()->PostMessage(IDM_USER_MOVE,WIK_NULL,0);
		return TRUE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
