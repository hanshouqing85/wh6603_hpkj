#ifndef ACCOUNTS_CONTROL_HEAD_FILE
#define ACCOUNTS_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//�˺ſؼ�
class CAccountsCtrl : public CSkinComboBox
{
	//��������
public:
	//���캯��
	CAccountsCtrl();
	//��������
	virtual ~CAccountsCtrl();
	CFont							m_Font;

	//���غ���
protected:
	//�󶨺���
	virtual VOID PreSubclassWindow();
	//��������
	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
//	VOID SetBackColor(COLORREF crBackColor);

	//�ػ���Ϣ
//	VOID OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif