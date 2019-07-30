#ifndef PLAZA_VIEW_ITEM_HEAD_FILE
#define PLAZA_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"
#include "ChongQingSSC.h"
//#include "JiangXiSSC.h"
//#include "XinJiangSSC.h"
#include "GuangDong11X5.h"
//#include "ChongQing11X5.h"
//#include "JiangXi11X5.h"
//#include "FenFenCai.h"
//#include "WuFenCai.h"
//#include "ShanDong11X5.h"
#include "LiuHeCai.h"
//#include "HeiLongJiang11X5.h"
#include "QiXingCai.h"
#include "PaiLie3.h"
#include "3D.h"
#include "BeiJingPK10.h"
#include "BeiJingKuai8.h"
#include "XingYun28.h"
#include "PlatformPublicize.h"
#include "ScrollBarEx.h"
//////////////////////////////////////////////////////////////////////////////////

#define IDM_SHOW_MENU				1110								//��ʾ�˵�
#define IDM_CLICKED_TYPE			1111								//���°�ť״̬
#define IDM_SEND_QUERY				1112								//��ѯ����
//��������
#define VIEW_MODE_NONE				0									//��Чģʽ
#define VIEW_MODE_KIND				1									//����ģʽ
#define VIEW_MODE_SERVER			2									//����ģʽ
#define VIEW_MODE_GAME				3									//��Ϸģʽ
#define VIEW_MODE_TYPE				4									//����ģʽ

//��ť��ʶ
#define BT_ENTER_KIND				2									//��������
#define BT_ENTER_SERVER				3									//���뷿��
#define BT_OPEN_LOTTERY				4									//������¼

//////////////////////////////////////////////////////////////////////////////////

//���Ͷ���
 struct tagGameKindInfo
 {
 	WORD							wSortID;							//�����ʶ
 	CPngImage						ImageKindItem;						//����ͼƬ
 	CGameKindItem *					pGameKindItem;						//���Ͷ���
 };

//���䶨��
struct tagGameServerInfo
{
	WORD							wSortID;							//�����ʶ
	WORD							wServerID;							//�����ʶ
	CGameServerItem *				pGameServerItem;					//�������
};



//���鶨��
typedef CWHArray<tagGameKindInfo *>	CGameKindInfoArray;					//��������
typedef CWHArray<tagGameServerInfo *> CGameServerInfoArray;				//��������
#define  IDM_RETURN_GAME 11118
struct tagLuckNumXinXi
{
	WORD  wKindID;
	TCHAR qihao[KJ_QIHAO_LENGTH];
	TCHAR haoma[KJ_HAOMA_LENGTH];
	TCHAR shijian[KJ_SHIJIAN_LENGTH];
};
struct tagLuckNum
{
	tagLuckNumXinXi LuckNum[50];
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ�㳡
class CPlazaViewItem : public CDialog, public IUnknownEx
{
	//״̬����
protected:
	bool							m_bHovering;						//������־
	bool							m_bCreateFlag;						//������־
	BYTE							m_cbShowItemMode;					//��ʾģʽ

	CRect							m_GameListRect;						//�б�����
	bool							m_bShowMenu;						//�Ƿ���ʾ�˵�
	int								m_nMenuXPos;
	int								m_nMenuYPos;
	BYTE							m_cbMenuType;						//�˵����
	CPlatformPublicize					m_logo;							//LOGO IE�ؼ�
	TCHAR 							m_strWebUrl[126];
	int								m_nOldTypeID;							//��һ����ϷID
	CRect							m_rcTypeRect[3];						//��������

	int								m_nRecordTypeID;					//���ذ�ť��Ӧ��ID
	int								m_nRecordKindID;					//����ID 
	DWORD							m_dwTanChuangTick;
	CScrollBarEx					m_VerticalScrollBar2;				//������
	CScrollBarEx					m_VerticalScrollBarQp;				//������������
	int								m_nScrollX;
	int								m_nScrollXQp;

	tagLuckNum						m_AllLuckNum[50];					//�������뻺��
	//ʱʱ��
public:
	CFont							m_Font;
// 

	
	//�ƶ�λ��
protected:
	BYTE							m_cbButtonDown;						//��ť����
	BYTE							m_cbButtonHover;					//��ť����
	WORD							m_wViewItemDown;					//�������
	WORD							m_wViewItemHover;					//��������
	CImageButton					m_btnReturn;						//����

	//λ�ñ���
protected:
	WORD							m_wKindXCount;						//��������
	WORD							m_wKindYCount;						//��������
	WORD							m_wKindExcursion;					//����ƫ��
	WORD							m_wKindPageCount;					//����ҳ��
	WORD							m_wKindPageCurrent;					//��ǰҳ��
	WORD                            m_wKindTypeCurrentID;               //��ǰ����
	CMissionManager*				m_MissionManager;					//�������
	//λ�ñ���
protected:
	WORD							m_wServerXCount;					//��������
	WORD							m_wServerYCount;					//��������
	WORD							m_wServerExcursion;					//����ƫ��
	WORD							m_wServerPageCount;					//����ҳ��
	WORD							m_wServerPageCurrent;				//��ǰҳ��

	//��Դ����
protected:
	//CPngImage						m_ImageTitle;						//����ͼƬ
	int								m_nNowTypeID;		
	CPngImage						m_ImageServer;						//����ͼƬ
	BYTE							m_bCreate;
	//��������
protected:
	CGameKindInfoArray				m_GameKindInfoActive;				//��������
	CGameKindInfoArray				m_GameKindInfoInterface;				//��������
	CGameKindInfoArray				m_GameKindInfoActiveMenu;			//��������
	CGameKindInfoArray				m_GameKindInfoBuffer;				//��������
	CGameServerInfoArray			m_GameServerInfoActive;				//��������
	CGameServerInfoArray			m_GameServerInfoBuffer;				//��������
	bool							m_bCreateType1;
	WORD							m_wGameHoverItem;
	//�ؼ�ָ��
protected:
	static CPlazaViewItem *			m_pPlazaViewItem;					//�㳡ָ��
public:
	CChongQingSSC					m_dlgChongQingSSC;					//����ʱʱ��
	CChongQingSSC					m_dlgFenFenCai;						//�ֲַ�
	CChongQingSSC					m_dlgWuFenCai;						//�ֲַ�
	CChongQingSSC					m_dlgJiangXiSSC;					//����ʱʱ��
	CChongQingSSC					m_dlgXinjiangSSC;					//�½�ʱʱ��
	CGuangDong11X5					m_dlgGuangdong11x5;					//�㶫11ѡ5
	CGuangDong11X5					m_dlgChongQing11x5;					//����11ѡ5
	CGuangDong11X5					m_dlgJiangXi11x5;					//����11ѡ5
	CGuangDong11X5					m_dlgShanDong11x5;					//ɽ��11ѡ5
	CGuangDong11X5					m_dlgHeiLongJiang11X5;				//������11ѡ5		
	CLiuHeCai						m_dlgLiuHeCai;						//���ϲ�
	CQiXingCai						m_dlgQiXingCai;						//���ǲ�--lly
	CPaiLie3						m_dlgPaiLie3;						//������
	C3D								m_dlg3D;							//3D��
	CBeiJingPK10					m_dlgBjPK10;						//pk10
	CBeiJingKuai8					m_dlgBjKuai8;						//����8
	CXingYun28						m_dlgXingYun28;						//����28
	bool							m_bLogonSuccess;

	CString							m_strTopNewsMsg;					//TOPWIN��Ϣ


	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
	void CreateDlgCaipiao(int nType);
	//��������
public:
	//���캯��
	CPlazaViewItem();
	//��������
	virtual ~CPlazaViewItem();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK() { return; };
	//ȡ������
	virtual VOID OnCancel() { return; };

	//���ܺ���
public:
	//��ʾ����
	VOID ShowTypeItemView();

	//��ʾ����
	VOID ShowKindItemView(WORD wTypeID,BOOL bResetPageIndex=TRUE);
	//��ʾ����
	VOID ShowKindQpItemView(WORD wTypeID,BOOL bResetPageIndex=TRUE);
	//��ʾ����
	VOID ShowServerItemView(WORD wKindID,BOOL bResetPageIndex=FALSE, bool Invalidate=true);
	//�������
	VOID InValidateWndView(BYTE cbViewType);

	VOID ShowGameMenu(WORD wTypeID);

	VOID FreshGame();

	VOID SetLogonSuccess(bool bSuccess);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg   BOOL   OnMouseWheel(   UINT   nFlags,   short   zDelta,   CPoint   pt   );

	VOID SetMissionManager(CMissionManager* pMissionManager){m_MissionManager = pMissionManager;}

	//���а���Ϣ
	void SetTopNewsMsg(CString strMsg);

	//���ܺ���
public:
	//��ȡʵ��
	static CPlazaViewItem * GetInstance() { return m_pPlazaViewItem; }	

	//�滭����
private:
	//�滭����
	VOID DrawKindItem(CDC * pDC, WORD wViewIndex, INT nXPos, INT nYPos, tagGameKindInfo * pGameKindInfo);
	//�滭����
	VOID DrawServerItem(CDC * pDC, WORD wViewIndex, INT nXPos, INT nYPos, tagGameServerInfo * pGameServerInfo);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	//�ڲ�����
protected:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	VOID RectifyDlg();

	//λ�ò���
protected:
	//��������
	WORD GetHoverIndex(CPoint MousePoint);
	//��ť����
	BYTE GetHoverButton(WORD wViewIndex, CPoint MousePoint);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	WORD GetGameHoverIndex(CPoint MousePoint);
	//��ť��Ϣ
public:
	//��ԴĿ¼
	VOID GetGameDirectory(TCHAR szDirectory[], WORD wBufferCount, tagGameKind & GameKind);
	//�鿴����
	VOID OnButtonViewRule(WORD wKindID);
	//��������
	VOID OnButtonEnterKind(WORD wKindID);
	//���뷿��
	VOID OnButtonEnterServer(WORD wServerID);
	void ResetRecordTypeKind();

	void ReturnTouzhu();
	//��ť��Ϣ
protected:
	//��ҳ��ť
	VOID OnBnClickedLastKind();
	//��ҳ��ť
	VOID OnBnClickedNextKind();
	//���ذ�ť
	VOID OnBnClickedReturnHall();
	//��ҳ��ť
	VOID OnBnClickedLastServer();
	//��ҳ��ť
	VOID OnBnClickedNextServer();	
	afx_msg void OnBnClickedReturnUppage();

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	LRESULT OnUpdateAccount(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnUpdateYue(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnTanChuang(WPARAM wParam, LPARAM lParam);
	//�޸�����
	LRESULT OnShowXgmm(WPARAM wParam, LPARAM lParam);

public:
	VOID AddOpenResult(CMD_GP_QueryLotResult* pQueryLotResult, int wDataSize);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif