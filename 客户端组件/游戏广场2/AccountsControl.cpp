#include "StdAfx.h"
#include "Resource.h"
#include "AccountsControl.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define ITEM_CY						22									//�����С

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAccountsCtrl, CSkinComboBox)
	//ON_WM_PAINT()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CAccountsCtrl::CAccountsCtrl()
{
	m_Font.CreateFont(26,0,0,0,0,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("Arial"));

}

//��������
CAccountsCtrl::~CAccountsCtrl()
{
}

//�󶨺���
VOID CAccountsCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//���ÿؼ�
	m_SkinComboBoxEdit.LimitText(LEN_ACCOUNTS-1);
	m_SkinComboBoxEdit.SetEnableColor(RGB(254,254,254),RGB(41,51,97),RGB(41,51,97));
	//���ÿؼ�
	SetButtonImage(AfxGetInstanceHandle(),IDB_BT_COMBOBOX,CSize(11,12));

	
	return;
}
// VOID CAccountsCtrl::SetBackColor(COLORREF crBackColor)
// {
// 	SetBackColor(crBackColor);
// 	return;
// }

//��������
VOID CAccountsCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//���ñ���
	lpMeasureItemStruct->itemWidth=0;
	lpMeasureItemStruct->itemHeight=23;

	return;
}
////�ػ���Ϣ
//VOID CAccountsCtrl::OnPaint()
//{
//	CPaintDC dc(this);
//
//	//��ȡλ��
//	CRect rcClient;
//	GetClientRect(&rcClient);
//
//	CRect rcInv;
//	rcInv.CopyRect(rcClient);
//
//	rcInv.right -= 20;
//
//	dc.SetBkMode(TRANSPARENT);
//
//	//�滭����
//	if (IsWindowEnabled()==TRUE)
//	{
//		dc.FillSolidRect(rcInv,RGB(73,62,61));
//	}
//	else
//	{
//		dc.FillSolidRect(rcInv,RGB(73,62,61));
//	}
//
//	return;
//}

//////////////////////////////////////////////////////////////////////////////////
