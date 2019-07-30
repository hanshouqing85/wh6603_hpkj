#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#pragma once

//ϵͳ�ļ�
#include "Stdafx.h"
#include "Resource.h"


//�����ļ�
#include "MissionList.h"
#include "MissionLogon.h"

//�ؼ��ļ�
#include "PlazaViewItem.h"
#include "ServerViewItem.h"
//���ڿؼ�
#include "WndGameTypeCtrl.h"
#include "WndUserInfoCtrl.h"
#include "WndTopNews.h"
#include "PlatformPublicize.h"
#include "ZhuanhuanDlg.h"
#include "ZhangHuDlg.h"
#include "HuiYuanDlg.h"
#include "ChongZhiDlg.h"
#include "QuKuanDlg.h"
#include "HuoDongDlg.h"
#include "TzhLogDlg.h"
#include "WebBrowser.h"
#include	"GridCtrl/GridCtrl.h"
#include	"GridCtrl/GridCellCheck.h"
#include "TiShiDlg.h"
#include "DlgStatus.h"
#include <vector>
using namespace std;
#include "Other/GfxOutBarCtrl.h"
#include "PlatformMessage.h"
//////////////////////////////////////////////////////////////////////////////////

//�ȼ�����
#define IDI_HOT_KEY_BOSS			0x0100								//�ϰ��ȼ�
//////////////////////////////////////////////////////////////////////////////////

//ƽ̨���
class CPlatformFrame : public CFrameWnd, public IServerListDataSink,public CMissionItem, public IStatusViewSink
{
	//״̬����
protected:
	bool							m_bRectify;							//������־
	CRect							m_rcViewItem;						//��ͼ��С

	DOUBLE							m_fJiaoyiScore;						//���׽��
	bool							m_bToCaijin;						//ת�ʽ�
	bool							m_bToJinbi;							//ת���
	bool							m_bChaxunYue;						//ˢ�����
	bool							m_bGetMapBonus;						//bonus
	bool							m_bGetLuckyNums;						//��ȡ��������
	int								m_nTopNewX;
	int								m_nTopNewY;
	CDlgStatus						m_DlgStatus;
	bool							m_bCreate;
	//ȡ������
	virtual VOID OnStatusCancel();
	DWORD m_dwTickCount;
	bool							m_bShowNetAlive;
	//���ư�ť
public:
	CSkinButton						m_btMin;							//��С��ť
	CSkinButton						m_btClose;							//�رհ�ť
	CSkinButton						m_btSound;							//������ť
	CSkinButton						m_btNet;							//�źŰ�ť
	CSkinButton						m_btNavigation[7];					//������ť
	CSkinButton						m_btHome;							//��ҳ��ť
	CSkinButton						m_btPlazaNotice;							//���水ť
	CSkinButton						m_btPlazaMessage;							//���水ť
	static	UINT	GetLuckNumber(LPVOID lpParam);
	CTiShiDlg						m_tishiDlg;
	bool							m_bKillSocket;						//ǿ������
	CGridCtrl						m_Grid;				//���Ź���
	bool							m_bLogonFail;
	bool							m_bSound;
	bool							m_bShutdowSocket;					//�Ƿ�ر�����
	//��ܿؼ�
protected:
	CWndGameTypeCtrl				m_WndGameTypeCtrl;					//��������
	CWndUserInfoCtrl				m_WndUserInfoCtrl;					//�û���Ϣ
	CWndTopNews						m_WndTopNews;						//���Ź���
// 	CGfxOutBarCtrl					m_WndBar;							//������
// 	CImageList						imaLarge, imaSmall;
	BYTE							m_cbShowNewsType;					//����λ��
//�ؼ�����
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��
	CSkinSplitter					m_SkinSplitter;						//��ֿؼ�
	CServerListData					m_ServerListData;					//��Ϸ����
	//CPlatformPublicize              m_PlatformPublicize;				//����ؼ� 
	//CPlatformPublicize              m_PlatFormNoticePublicize;          //����ؼ� 
	//CPlatformPublicize              m_PlatFormLeftPublicize;          //����ؼ� 
	bool							m_bGetNews;
	bool							m_bGetTime;						//��ȡϵͳʱ��
	bool							m_bQuitGame;					//�˳�
	CZhangHuDlg					m_dlgUserAccount;					//�û��˻�
	CHuiYuanDlg					m_dlgHuiyuan;						//��Ա����
	CChongZhiDlg				m_dlgChongzhi;						//��ֵ
	CHuoDongDlg					m_dlgHuodong;						//�
	CQuKuanDlg					m_dlgQukuan;						//ȡ��
	CTzhLogDlg			m_dlgTouZhu;						//Ͷע��¼
	CPlatformMessage		m_dlgPlatformMessage;		//ƽ̨��Ϣ
	//�������
protected:
	CMissionList					m_MissionList;						//�б�����
	CMissionLogon					m_MissionLogon;						//��¼����
public:
	CMissionManager					m_MissionManager;					//�������
	bool							m_bCanSend;							//�Ƿ���Է���
	//�ؼ�����
public:
	CPlazaViewItem					m_PlazaViewItem;					//��Ϸ�㳡
	CServerViewItem					m_ServerViewItem;					//��ǰ����

	//��Դ����
protected:
	//CBitImage						m_ImageFrame;						//���ͼƬ
	CEncirclePNG					m_FrameEncircle;					//�����Դ
	//CEncircleBMP					m_AfficheEncircle;					//�����Դ
	CEncircleBMP					m_ItemFrameEncircle;				//�����Դ
	vector<wstring>					m_vecTitle;
	bool							m_bGame;			//�Ƿ�����Ϸ���棬true��Ϸ��false����
	int								m_nNewsYPos;	//���Ź���ʱ��Y����
	int								m_nTop;
	int								m_nreduce;
	int								m_nNewsHeight;
	CFont m_top8Font;
	CMarkup m_xml;

	//��̬����
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//���ָ��

	//��������
public:
	//���캯��
	CPlatformFrame();
	//��������
	virtual ~CPlatformFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//״̬֪ͨ
public:
	//��ȡ֪ͨ
	virtual VOID OnGameItemFinish();
	//��ȡ֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID);

	//����֪ͨ
public:
	//����֪ͨ
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//������Դ
	VOID RectifyResource(INT nWidth, INT nHeight);
	//�滭����
	VOID DrawControlView(CDC * pDC, INT nWidth, INT nHeight);

	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);

	//�������
public:
	//�����
	bool ActiveServerViewItem();
	//ɾ������
	bool DeleteServerViewItem();
	//���뷿��
	bool EntranceServerItem(CGameServerItem * pGameServerItem);
	void DrawTopUsers(CDC* pDC);

	void	GridCtrlInit();				//��ʼ�����Ź���
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//��Ϣ����
protected:
	//�ر���Ϣ
	VOID OnClose();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	LRESULT OnUpdateButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpDateAccoount(WPARAM wParam, LPARAM lParam);
	LRESULT OnShowXgmm(WPARAM wParam, LPARAM lParam);
	LRESULT OnRefreshYue(WPARAM wParam, LPARAM lParam);
	LRESULT OnFreshGame(WPARAM wParam, LPARAM lParam);
	LRESULT OnTouZhuTishi(WPARAM wParam, LPARAM lParam);

	LRESULT OnZhuanhuanJinbi(WPARAM wParam, LPARAM lParam);
	LRESULT OnZhuanhuanCaibi(WPARAM wParam, LPARAM lParam);
	LRESULT ReturnTouzhu(WPARAM wParam, LPARAM lParam);
	LRESULT ReleaseFace(WPARAM wParam, LPARAM lParam);
	bool m_bCreateType4;
//�Զ���Ϣ
protected:
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
	//���и���
	LRESULT OnMessageInsureUpdate(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnTurnPage(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnSelectUser(WPARAM wParam, LPARAM lParam);

	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
public:
	VOID SendToServer(int nSendType);
	VOID AvtiveMainMissionItem();
	bool CreatePlazaview();
public:
	//CPlatformPublicize			m_logo;			//LOGO IE�ؼ�
	//	CWebBrowser			*m_pnotice;		//������Ϣ

	//CPlatformPublicize			m_Publicizeurl;			//LOGO IE�ؼ�
	void Loadweb();
	void ShowXgmm();

	//���ܺ���
public:
	//��ȡʵ��
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif