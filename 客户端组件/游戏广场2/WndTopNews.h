#ifndef WND_TOP_NEWS_HEAD_FILE
#define WND_TOP_NEWS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ����
class CWndTopNews : public CWnd
{
	//�����Ϣ
protected:
	bool							m_bHovering;						//������־
	WORD							m_wItemDown;						//��������
	WORD							m_wItemHover;						//��������
	WORD							m_wItemActive;						//��������
	int								m_nNewsLength;
	int								m_nFreshLength;		
	CFont							m_font;
	int								m_nNewsX;
	CBitImage ImageItemBack;
//	CEncircleBMP					m_ItemFrameEncircle;				//�����Դ
	CString							m_strNews;
	//��������
public:
	//���캯��
	CWndTopNews();
	//��������
	virtual ~CWndTopNews();
	void DrawMultLineText(CDC *pDC, CRect rect, int nRowDis, UINT nFromat, CString strText);
	//��Ϣ����	
protected:
	BOOL PreTranslateMessage(MSG * pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	CPlatformPublicize m_webbrowser;
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
public:
	void SetTopNews(CString strNews);


	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif