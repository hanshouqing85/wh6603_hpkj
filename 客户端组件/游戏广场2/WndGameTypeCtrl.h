#ifndef WND_GAME_TYPE_CTRL_HEAD_FILE
#define WND_GAME_TYPE_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ����
class CWndGameTypeCtrl : public CWnd
{
	//�����Ϣ
protected:
	bool							m_bHovering;						//������־
	WORD							m_wItemDown;						//��������
	WORD							m_wItemHover;						//��������
	WORD							m_wItemActive;						//��������
		CBitImage ImageItemBack;
	CPngImage ImageGameTypeItem;
	
	//CSkinMenu						*m_pGameMenu;
	//������Ϣ
protected:
	CSize							m_SizeButton;						//��ť��С
	CGameTypeItemArray				m_GameTypeItemArray;				//��������
	CGameKindInfoArray				m_GameKindInfoArray;
	bool							m_bShowType;						//�Ƿ���ʾ
	//��������
public:
	//���캯��
	CWndGameTypeCtrl();
	//��������
	virtual ~CWndGameTypeCtrl();


	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��ȡ��С
	VOID GetControlSize(CSize & ResultSize);
	//��������
	VOID InsertGameType(CGameTypeItem * pGameTypeItem);

	VOID ShowMenu(bool bShowMenu,WORD wShowType);
	VOID SetClicked(WORD wClickedType);
	//�ڲ�����
private:
	//��������
	WORD GetTypeItemIndex(WORD wTypeID);
	//��������
	WORD GetTypeItemIndex(CPoint MousePoint);

	//��Ϣ����
protected:
	BOOL PreTranslateMessage(MSG * pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif