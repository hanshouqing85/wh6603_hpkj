#pragma once
#include "Stdafx.h"
#include "plazaviewitem.h"
#include "afxwin.h"
// CDlgShowQPServer 对话框




//数组定义
typedef CWHArray<tagGameKindInfo *>	CGameKindInfoArray;					//类型数组
typedef CWHArray<tagGameServerInfo *> CGameServerInfoArray;				//房间数组





class CDlgShowQPServer : public CDialog
{
	DECLARE_DYNAMIC(CDlgShowQPServer)

public:
	CDlgShowQPServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgShowQPServer();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOW_QPSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:

	CPngImage							m_ImageGround;

	CImageButton						m_btOk;								//XX按钮
	CImageButton						m_btQuit;							//回到大厅按钮
	CFont								m_font;
public:
	void SetDataPrt(CGameServerInfoArray& pGameServer, int nViewCount, int nStartIndex, int nServerXCount);
	DWORD	GetSelServerID();
		//初始化
		BOOL OnInitDialog();
		//绘画背景
		BOOL OnEraseBkgnd(CDC * pDC);
		//大小尺寸
		void OnSize(UINT nType, int cx, int cy);
		
		//绘画房间
		VOID DrawServerItem(CDC * pDC, WORD wViewIndex, INT nXPos, INT nYPos, tagGameServerInfo * pGameServerInfo, int nIndex);

protected:
		DWORD							m_nSelServerID;
		map<WORD,DWORD>					m_mapServerID;

		map<int, CRect*>				m_mapServerIDRect;
		CPngImage						m_ImageServer;						//房间图片
		CGameServerInfoArray*			m_GameServerInfoActive;				//房间数组			m_pActiveServerInfo;				//活动房间
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



