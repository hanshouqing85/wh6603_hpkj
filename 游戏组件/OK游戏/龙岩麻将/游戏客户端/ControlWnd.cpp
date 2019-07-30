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
#define IDC_CHI						105									//���ư�ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_PENG, OnBnClickedPeng)
	ON_BN_CLICKED(IDC_GANG, OnBnClickedGang)
	ON_BN_CLICKED(IDC_HEAR, OnBnClickedHear)
	ON_BN_CLICKED(IDC_CHI_HU, OnBnClickedChiHu)
	ON_BN_CLICKED(IDC_GIVEUP, OnBnClickedGiveUp)
	ON_BN_CLICKED(IDC_CHI, OnBnClickedChi)
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
	m_btHear.EnableWindow(((cbAcitonMask&WIK_LISTEN)!=0)?TRUE:FALSE);
	m_btChiHu.EnableWindow(((cbAcitonMask&WIK_CHI_HU)!=0)?TRUE:FALSE);
	m_btChi.EnableWindow((cbAcitonMask&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))?TRUE:FALSE);

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
	CSkinButton * ControlButton[]={&m_btChi,&m_btPeng,&m_btGang,&m_btHear,&m_btChiHu};

	//���ư�ť
	for (INT i=0;i<CountArray(ControlButton);i++)
	{
		INT nXPos=i*rcButton.Width()+6;
		ControlButton[i]->SetWindowPos(NULL,nXPos,11,0,0,SWP_NOZORDER|SWP_NOSIZE);
	}

	//������ť
	m_btGiveUp.GetWindowRect(&rcButton);
	m_btGiveUp.SetWindowPos(NULL,nWidth-rcButton.Width()-12,nHeight-rcButton.Height()-5,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedPeng()
{
	//������Ϣ
	m_pSinkWnd->PostEngineMessage(IDM_USER_ACTION,WIK_PENG,0);

	return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedGang()
{
	//������Ϣ
	m_pSinkWnd->PostEngineMessage(IDM_USER_ACTION,WIK_GANG,0);

	return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedHear()
{
	//������Ϣ
	m_pSinkWnd->PostEngineMessage(IDM_USER_ACTION,WIK_LISTEN,0);

	return;
}

//�Ժ���ť
VOID CControlWnd::OnBnClickedChiHu()
{
	//������Ϣ
	m_pSinkWnd->PostEngineMessage(IDM_USER_ACTION,WIK_CHI_HU,0);

	return;
}

//������ť
VOID CControlWnd::OnBnClickedGiveUp()
{
	//������Ϣ
	m_pSinkWnd->PostEngineMessage(IDM_USER_ACTION,WIK_NULL,0);

	return;
}

//������ť
VOID CControlWnd::OnBnClickedChi()
{
	//������Ϣ
	m_pSinkWnd->PostEngineMessage(IDM_USER_ACTION,WIK_LEFT,0);

	return;
}

//�ػ�����
VOID CControlWnd::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	////CImageHandle HandleControlBack(&m_ImageControlBack);
	m_ImageControlBack.TransDrawImage(&dc,0,0,RGB(255,0,255));

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
	m_btHear.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_HEAR);
	m_btChiHu.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHI_HU);
	m_btChi.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHI);

	//����λͼ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btPeng.SetButtonImage(IDB_BT_PENG,hResInstance,false,false);
	m_btGang.SetButtonImage(IDB_BT_GANG,hResInstance,false,false);
	m_btHear.SetButtonImage(IDB_BT_LISTEN,hResInstance,false,false);
	m_btChiHu.SetButtonImage(IDB_BT_CHIHU,hResInstance,false,false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP,hResInstance,false,false);
	m_btChi.SetButtonImage(IDB_BT_CHI,hResInstance,false,false);

	//��������
	CRgn WindowRgn;
	////CImageHandle HandleControlBack(&m_ImageControlBack);
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

//////////////////////////////////////////////////////////////////////////
