#pragma once
#include "Stdafx.h"
#include "plazaviewitem.h"
#include "afxwin.h"
// CDlgShowQPServer �Ի���




//���鶨��
typedef CWHArray<tagGameKindInfo *>	CGameKindInfoArray;					//��������
typedef CWHArray<tagGameServerInfo *> CGameServerInfoArray;				//��������





class CDlgShowQPServer : public CDialog
{
	DECLARE_DYNAMIC(CDlgShowQPServer)

public:
	CDlgShowQPServer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgShowQPServer();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHOW_QPSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:

	CPngImage							m_ImageGround;

	CImageButton						m_btOk;								//XX��ť
	CImageButton						m_btQuit;							//�ص�������ť
	CFont								m_font;
public:
	void SetDataPrt(CGameServerInfoArray& pGameServer, int nViewCount, int nStartIndex, int nServerXCount);
	DWORD	GetSelServerID();
		//��ʼ��
		BOOL OnInitDialog();
		//�滭����
		BOOL OnEraseBkgnd(CDC * pDC);
		//��С�ߴ�
		void OnSize(UINT nType, int cx, int cy);
		
		//�滭����
		VOID DrawServerItem(CDC * pDC, WORD wViewIndex, INT nXPos, INT nYPos, tagGameServerInfo * pGameServerInfo, int nIndex);

protected:
		DWORD							m_nSelServerID;
		map<WORD,DWORD>					m_mapServerID;

		map<int, CRect*>				m_mapServerIDRect;
		CPngImage						m_ImageServer;						//����ͼƬ
		CGameServerInfoArray*			m_GameServerInfoActive;				//��������			m_pActiveServerInfo;				//�����
private:
	int									m_wServerXCount;
	int									m_nViewCount;
	int									m_nStartIndex;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CImageButton							m_btn_enter1;
	CImageButton							m_btn_enter2;
	CImageButton							m_btn_enter3;
	CImageButton							m_btn_enter4;
};



