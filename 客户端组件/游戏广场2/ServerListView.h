#ifndef SERVER_LIST_VIEW_HEAD_FILE
#define SERVER_LIST_VIEW_HEAD_FILE

#pragma once
#include "Stdafx.h"
#include "ServerListData.h"
#include "WndGameTypeCtrl.h"
//////////////////////////////////////////////////////////////////////////////////
//�����б�
typedef CList<CGameServerItem *>	 CGameServerList;				//��Ϸ����

//�б�ڵ�
struct tagServerListItem
{
	INT             nPosX;
	INT             nPosY;
	WORD            wKindHeight;
	bool            bShowServer;
	CGameKindItem * pGameKindItem;
	CGameServerList ServerList;

	tagServerListItem()
	{
		nPosX=0;
		nPosY=0;
		wKindHeight=52;
		bShowServer=false;
		pGameKindItem=NULL;
	}

	~tagServerListItem()
	{
		ServerList.RemoveAll();
	}
};

//��Ϸ�б�
typedef CList<tagServerListItem *>	 CServerList;				//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////

//����˵��
typedef CMap<WORD,WORD,UINT,UINT>	CGameImageMap;						//��Ϸͼ��
typedef CList<WORD>	                CLastGameServerList;				//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////

//�����б�
class CServerListView : public CWnd, public IServerListDataSink
{
	//λ�ñ���
protected:
	INT								m_nXScroll;							//����ƫ��
	INT								m_nYScroll;							//����ƫ��
	INT								m_nScrollPos;						//����λ��
	INT								m_nScrollPosMax;					//���λ��
	////��ʾ����
    INT                             m_nShowType;
	//״̬��Ϣ
protected:
	bool							m_bHovering;						//������־
	bool							m_bHoveKind;						//������־
	bool							m_bLMouseDown;						//�����־
	CWndGameTypeCtrl				m_WndGameTypeCtrl;					//��������
	//�б���
protected:
	CServerList                     m_ServerList;
    CServerList                     m_ServerListGameType;
	//��Դ����
protected:
	CFont							m_FontBold;							//��������
	CSkinScrollBar					m_SkinScrollBar;					//��������

	//��Դ����
protected:
	CGameImageMap					m_GameImageMap;						//ͼ������
	CLastGameServerList             m_LastPlayGameList;                 //��������Ϸ

	//��������
protected:
	tagServerListItem *				m_pItemMouseHover;					//��������
	tagServerListItem *				m_pItemMouseDown;					//�������
	CGameServerItem *               m_pServerItemHover;                 //��������
	CGameServerItem *               m_pServerItemDown;                  //�������

	//�������
protected:

	//��������
public:
	//���캯��
	CServerListView();
	//��������
	virtual ~CServerListView();

	//״̬֪ͨ
public:
	//��ȡ֪ͨ
	virtual VOID OnGameItemFinish();
	//��ȡ֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID);
	//����֪ͨ
	virtual VOID OnGameItemUpdateFinish();

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
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//���ں���
	virtual LRESULT DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//����ͨ��
	VOID InitAssistantItem();
	//���ú���
	VOID InitServerTreeView();

	//�滭����
private:
	//�滭����
	VOID DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox);
	//�滭����
	VOID DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox);

	//�滭����
private:
	//�����ı�
	VOID DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected);

	//ͼ�꺯��
private:
	//��ȡͼ��
	UINT GetGameImageIndex(CGameKindItem * pGameKindItem);
	//��ȡͼ��
	UINT GetGameImageIndex(CGameServerItem * pGameServerItem);

	//���⺯��
private:
	//��ȡ����
	LPCTSTR GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount);
	//��ȡ����
	LPCTSTR GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount);

	//��������
private:
	//ɾ������
	VOID DeleteUpdateItem(CGameListItem * pGameListItem);
	//�޸�����
	VOID ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage);
	//��������
	HTREEITEM InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem);
	//��������
	HTREEITEM InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem);
	//��ԴĿ¼
	VOID GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount);
	//��������
	tagServerListItem *HitTest(CPoint &MousePoint);
	//��������
	CGameServerItem *HitSubTest(tagServerListItem *pServerListItem,CPoint &MousePoint);
	//����б�
	void ClearServerList();
	//����λ��
	void ModifyListPos();
	//����λ��
	void SetScrollMaxPos();

	//ϵͳ��Ϣ
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//������Ϣ
	VOID OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	//�б���Ϣ
protected:
	//�Ҽ��б�
	VOID OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult);
	//����б�
	VOID OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�ı�
	VOID OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�չ��
	VOID OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()

protected:
	//��÷��为����Ϣ
	LPCTSTR GetLoadInfoOfServer(DWORD dwOnLineCount, DWORD dwMaxCount, LPTSTR pszBuffer, WORD wBufferSize);
	//��÷��为����Ϣ
	LPCTSTR GetLoadInfoOfServer(tagGameServer * pGameServer, LPTSTR pszBuffer, WORD wBufferSize);
	//��÷��为����Ϣ
	LPCTSTR GetLoadInfoOfServer(tagGameKind * pGameKind, LPTSTR pszBuffer, WORD wBufferSize);
	//���ؼ�¼
	void LoadLastPlayGame();
	//�����¼
	void SaveLastPlayGame();

public:
	VOID ShowGameType(WORD wType);
	VOID ShowGameServer();
};

//////////////////////////////////////////////////////////////////////////////////

#endif