#pragma once

#include "Resource.h"
// CCardExtractor �Ի���
#include "CardControl.h"
#include "GameClientEngine.h"

class CCardExtractor : public CDialog
{
	DECLARE_DYNAMIC(CCardExtractor)


	CGameClientEngine			*m_pParentDlg;
public:
	CCardExtractor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCardExtractor();

// �Ի�������
	enum { IDD = IDD_CARD_EXTRACTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
public:
	virtual BOOL OnInitDialog();

protected:
	CCardControl  m_CardCtrl[4];
	BYTE		  m_cbHoverCard;

	CBitImage						m_ImageCard;						//ͼƬ��Դ

};
